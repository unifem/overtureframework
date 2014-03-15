! This file automatically generated from extrapolateOpt.bf with bpp.
c You should preprocess this file with the bpp preprocessor before compiling.





! reduce the 3rd-order extrapolation to first order where the solution is not smooth

! reduce the 2nd-order extrapolation to first order where the solution is not smooth


      subroutine extrapInterpNeighboursOpt(nd,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & nda1a,nda1b,ndd1a,ndd1b,
     & ia,id, vew, u,ca,cb, ipar, rpar  )
c======================================================================
c  Optimised Boundary Conditions ** extrapolate interpolation neighbours ***
c         
c nd : number of space dimensions
c ca,cb : assign components c=uC(ca),..,uC(cb)
c ia : extrapolateInterpolationNeighbourPoints
c id : extrapolateInterpolationNeighboursDirection
c vew(i) : variable extrapolation width
c======================================================================
      implicit none
      integer nd, nda1a,nda1b,ndd1a,ndd1b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      integer ia(nda1a:nda1b,0:*), id(ndd1a:ndd1b,0:*), vew(
     & ndd1a:ndd1b,0:*)
      integer ca,cb,orderOfExtrapolation
      integer ipar(0:*)
      real rpar(0:*)

c     --- local variables 
      integer i,i1,i2,i3,c,n1a,n1b,width,is1,is2,is3
      integer maxExtrapWidth,useVariableExtrapolationWidth
      real ue1,ue2,ue3
      real alpha,uEps,denom

      real limiterPar0,limiterPar1
      integer extrapolationOption
      integer polynomialExtrapolation,extrapolateWithLimiter
      parameter( polynomialExtrapolation=0,extrapolateWithLimiter=1 )

      n1a=nda1a
      n1b=nda1b

      i2=ndu2a
      i3=ndu3a
      is2=0
      is3=0

      maxExtrapWidth=ipar(0)
      orderOfExtrapolation=ipar(1)
      extrapolationOption=ipar(2)
      useVariableExtrapolationWidth=ipar(3)

      limiterPar0=rpar(0)
      limiterPar1=rpar(1)
      uEps       =rpar(2)

      ! write(*,'("extrapInterpNeighboursOpt: orderOfExtrapolation=",i4)') orderOfExtrapolation

      if( .false. .and. nd.eq.2 ) then
        ! check the extrapolation formula
        width=orderOfExtrapolation-1
        if( width.le.0 )then
          width=2 ! default
        end if
        do i=n1a,n1b
          i1=ia(i,0)
          i2=ia(i,1)
          if( i1.lt.ndu1a .or. i1.gt.ndu1b .or. i1+width*id(i,0)
     & .lt.ndu1a .or. i1+width*id(i,0).gt.ndu1b .or.i2.lt.ndu2a .or. 
     & i2.gt.ndu2b .or. i2+width*id(i,1).lt.ndu2a .or. i2+width*id(i,
     & 1).gt.ndu2b )then
            write(*,'("extrapInterpNeighboursOpt:ERROR: i=",2i4," id=",
     & 2i4," ndu1a,...=",4i4)') i1,i2,id(i,0),id(i,1),ndu1a,ndu1b,
     & ndu2a,ndu2b
            ! '
          end if
        end do
      end if

      if( .false. .and. nd.eq.3 ) then
        ! check the extrapolation formula
        width=orderOfExtrapolation-1
        if( width.le.0 )then
          width=2 ! default
        end if
        do i=n1a,n1b
          i1=ia(i,0)
          i2=ia(i,1)
          i3=ia(i,2)
          if( i1.lt.ndu1a .or. i1.gt.ndu1b .or. i1+width*id(i,0)
     & .lt.ndu1a .or. i1+width*id(i,0).gt.ndu1b .or.i2.lt.ndu2a .or. 
     & i2.gt.ndu2b .or. i2+width*id(i,1).lt.ndu2a .or. i2+width*id(i,
     & 1).gt.ndu2b .or.i3.lt.ndu3a .or. i3.gt.ndu3b .or. i3+width*id(
     & i,2).lt.ndu3a .or. i3+width*id(i,2).gt.ndu3b )then
            write(*,'("extrapInterpNeighboursOpt:ERROR: i=",3i4," id=",
     & 3i4," ndu1a,...=",6i4)') i1,i2,i3,id(i,0),id(i,1),id(i,2),
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b
            ! '
          end if
        end do
      end if

      if( extrapolationOption.eq.polynomialExtrapolation )then
        ! normal polynomial extrapolation

       if( orderOfExtrapolation.eq.3 .or. orderOfExtrapolation.le.0 ) 
     & then
        if( nd.eq.2 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),i3,c)=(3.*u(ia(i,0)+id(i,0),ia(i,1)+id(
     & i,1),i3,c)-    3.*u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),i3,c)+ 
     &    u(ia(i,0)+3*id(i,0),ia(i,1)+3*id(i,1),i3,c))
           end do
           end do
        else if( nd.eq.3 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),ia(i,2),c)=(3.*u(ia(i,0)+id(i,0),ia(i,1)
     & +id(i,1),ia(i,2)+id(i,2),c)-     3.*u(ia(i,0)+2*id(i,0),ia(i,1)
     & +2*id(i,1),ia(i,2)+2*id(i,2),c)+     u(ia(i,0)+3*id(i,0),ia(i,
     & 1)+3*id(i,1),ia(i,2)+3*id(i,2),c))
           end do
           end do
        else
         do c=ca,cb
         do i=n1a,n1b
           u(ia(i,0),i2,i3,c)=(3.*u(ia(i,0)+id(i,0),i2,i3,c)-    3.*u(
     & ia(i,0)+2*id(i,0),i2,i3,c)+    u(ia(i,0)+3*id(i,0),i2,i3,c))
         end do
         end do
        end if

       else if( orderOfExtrapolation.eq.2 ) then

        if( nd.eq.2 ) then
         do c=ca,cb
         do i=n1a,n1b
           u(ia(i,0),ia(i,1),i3,c)=(2.*u(ia(i,0)+id(i,0),ia(i,1)+id(i,
     & 1),i3,c)-    u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),i3,c))
         end do
         end do
        else if( nd.eq.3 ) then

         do c=ca,cb
         do i=n1a,n1b
           u(ia(i,0),ia(i,1),ia(i,2),c)=(2.*u(ia(i,0)+id(i,0),ia(i,1)+
     & id(i,1),ia(i,2)+id(i,2),c)- u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,
     & 1),ia(i,2)+2*id(i,2),c))
         end do
         end do
       else
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),i2,i3,c)=(2.*u(ia(i,0)+id(i,0),i2,i3,c)-   u(ia(
     & i,0)+2*id(i,0),i2,i3,c))
           end do
           end do
        end if

       else if( orderOfExtrapolation.eq.1 ) then

       if( nd.eq.2 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),i3,c)=u(ia(i,0)+id(i,0),ia(i,1)+id(i,1),
     & i3,c)
           end do
           end do
       else if( nd.eq.3 ) then
         do c=ca,cb
         do i=n1a,n1b
           u(ia(i,0),ia(i,1),ia(i,2),c)=u(ia(i,0)+id(i,0),ia(i,1)+id(i,
     & 1),ia(i,2)+id(i,2),c)
         end do
         end do
       else
         do c=ca,cb
         do i=n1a,n1b
           u(ia(i,0),i2,i3,c)=u(ia(i,0)+id(i,0),i2,i3,c)
         end do
         end do
         end if

       else if( orderOfExtrapolation.eq.4 )then

        if( nd.eq.2 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),i3,c)=(4.*u(ia(i,0)+id(i,0),ia(i,1)+id(
     & i,1),i3,c)-    6.*u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),i3,c)+ 
     &    4.*u(ia(i,0)+3*id(i,0),ia(i,1)+3*id(i,1),i3,c)-    u(ia(i,0)
     & +4*id(i,0),ia(i,1)+4*id(i,1),i3,c))
           end do
           end do
        else if( nd.eq.3 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),ia(i,2),c)=(4.*u(ia(i,0)+id(i,0),ia(i,1)
     & +id(i,1),ia(i,2)+id(i,2),c)-     6.*u(ia(i,0)+2*id(i,0),ia(i,1)
     & +2*id(i,1),ia(i,2)+2*id(i,2),c)+     4.*u(ia(i,0)+3*id(i,0),ia(
     & i,1)+3*id(i,1),ia(i,2)+3*id(i,2),c)-     u(ia(i,0)+4*id(i,0),
     & ia(i,1)+4*id(i,1),ia(i,2)+4*id(i,2),c))
           end do
           end do
        else
         do c=ca,cb
         do i=n1a,n1b
           u(ia(i,0),i2,i3,c)=(4.*u(ia(i,0)+id(i,0),i2,i3,c)-    6.*u(
     & ia(i,0)+2*id(i,0),i2,i3,c)+    4.*u(ia(i,0)+3*id(i,0),i2,i3,c)-
     &     u(ia(i,0)+4*id(i,0),i2,i3,c))
         end do
         end do
        end if

       else if( orderOfExtrapolation.eq.5 )then

        if( nd.eq.2 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),i3,c)=(5.*u(ia(i,0)+id(i,0),ia(i,1)+id(
     & i,1),i3,c)-    10.*u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),i3,c)+
     &     10.*u(ia(i,0)+3*id(i,0),ia(i,1)+3*id(i,1),i3,c)-    5.*u(
     & ia(i,0)+4*id(i,0),ia(i,1)+4*id(i,1),i3,c)+    u(ia(i,0)+5*id(i,
     & 0),ia(i,1)+5*id(i,1),i3,c))
           end do
           end do
        else if( nd.eq.3 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),ia(i,2),c)=(5.*u(ia(i,0)+id(i,0),ia(i,1)
     & +id(i,1),ia(i,2)+id(i,2),c)-     10.*u(ia(i,0)+2*id(i,0),ia(i,
     & 1)+2*id(i,1),ia(i,2)+2*id(i,2),c)+     10.*u(ia(i,0)+3*id(i,0),
     & ia(i,1)+3*id(i,1),ia(i,2)+3*id(i,2),c)-     5.*u(ia(i,0)+4*id(
     & i,0),ia(i,1)+4*id(i,1),ia(i,2)+4*id(i,2),c)+     u(ia(i,0)+5*
     & id(i,0),ia(i,1)+5*id(i,1),ia(i,2)+5*id(i,2),c))
           end do
           end do
        else
         do c=ca,cb
         do i=n1a,n1b
           u(ia(i,0),i2,i3,c)=(5.*u(ia(i,0)+id(i,0),i2,i3,c)-    10.*u(
     & ia(i,0)+2*id(i,0),i2,i3,c)+    10.*u(ia(i,0)+3*id(i,0),i2,i3,c)
     & -    5.*u(ia(i,0)+4*id(i,0),i2,i3,c)+    u(ia(i,0)+5*id(i,0),
     & i2,i3,c))
         end do
         end do
        end if

       else if( orderOfExtrapolation.eq.6 )then

        if( nd.eq.2 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),i3,c)=(6.*u(ia(i,0)+id(i,0),ia(i,1)+id(
     & i,1),i3,c)-    15.*u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),i3,c)+
     &     20.*u(ia(i,0)+3*id(i,0),ia(i,1)+3*id(i,1),i3,c)-    15.*u(
     & ia(i,0)+4*id(i,0),ia(i,1)+4*id(i,1),i3,c)+    6.*u(ia(i,0)+5*
     & id(i,0),ia(i,1)+5*id(i,1),i3,c)-    u(ia(i,0)+6*id(i,0),ia(i,1)
     & +6*id(i,1),i3,c))
           end do
           end do
        else if( nd.eq.3 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),ia(i,2),c)=(6.*u(ia(i,0)+id(i,0),ia(i,1)
     & +id(i,1),ia(i,2)+id(i,2),c)-     15.*u(ia(i,0)+2*id(i,0),ia(i,
     & 1)+2*id(i,1),ia(i,2)+2*id(i,2),c)+     20.*u(ia(i,0)+3*id(i,0),
     & ia(i,1)+3*id(i,1),ia(i,2)+3*id(i,2),c)-     15.*u(ia(i,0)+4*id(
     & i,0),ia(i,1)+4*id(i,1),ia(i,2)+4*id(i,2),c)+     6.*u(ia(i,0)+
     & 5*id(i,0),ia(i,1)+5*id(i,1),ia(i,2)+5*id(i,2),c)-     u(ia(i,0)
     & +6*id(i,0),ia(i,1)+6*id(i,1),ia(i,2)+6*id(i,2),c))
           end do
           end do
        else
         do c=ca,cb
         do i=n1a,n1b
           u(ia(i,0),i2,i3,c)=(6.*u(ia(i,0)+id(i,0),i2,i3,c)-    15.*u(
     & ia(i,0)+2*id(i,0),i2,i3,c)+    20.*u(ia(i,0)+3*id(i,0),i2,i3,c)
     & -    15.*u(ia(i,0)+4*id(i,0),i2,i3,c)+    6.*u(ia(i,0)+5*id(i,
     & 0),i2,i3,c)-    u(ia(i,0)+6*id(i,0),i2,i3,c))
         end do
         end do
        end if

       else if( orderOfExtrapolation.eq.7 )then

        if( nd.eq.2 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),i3,c)=(7.*u(ia(i,0)+id(i,0),ia(i,1)+id(
     & i,1),i3,c)-    21.*u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),i3,c)+
     &     35.*u(ia(i,0)+3*id(i,0),ia(i,1)+3*id(i,1),i3,c)-    35.*u(
     & ia(i,0)+4*id(i,0),ia(i,1)+4*id(i,1),i3,c)+    21.*u(ia(i,0)+5*
     & id(i,0),ia(i,1)+5*id(i,1),i3,c)-    7.*u(ia(i,0)+6*id(i,0),ia(
     & i,1)+6*id(i,1),i3,c)+    u(ia(i,0)+7*id(i,0),ia(i,1)+7*id(i,1),
     & i3,c))
           end do
           end do
        else if( nd.eq.3 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),ia(i,2),c)=(7.*u(ia(i,0)+id(i,0),ia(i,1)
     & +id(i,1),ia(i,2)+id(i,2),c)-     21.*u(ia(i,0)+2*id(i,0),ia(i,
     & 1)+2*id(i,1),ia(i,2)+2*id(i,2),c)+     35.*u(ia(i,0)+3*id(i,0),
     & ia(i,1)+3*id(i,1),ia(i,2)+3*id(i,2),c)-     35.*u(ia(i,0)+4*id(
     & i,0),ia(i,1)+4*id(i,1),ia(i,2)+4*id(i,2),c)+     21.*u(ia(i,0)+
     & 5*id(i,0),ia(i,1)+5*id(i,1),ia(i,2)+5*id(i,2),c)-     7.*u(ia(
     & i,0)+6*id(i,0),ia(i,1)+6*id(i,1),ia(i,2)+6*id(i,2),c)+     u(
     & ia(i,0)+7*id(i,0),ia(i,1)+7*id(i,1),ia(i,2)+7*id(i,2),c))
           end do
           end do
        else
         do c=ca,cb
         do i=n1a,n1b
           u(ia(i,0),i2,i3,c)=(7.*u(ia(i,0)+id(i,0),i2,i3,c)-    21.*u(
     & ia(i,0)+2*id(i,0),i2,i3,c)+    35.*u(ia(i,0)+3*id(i,0),i2,i3,c)
     & -    35.*u(ia(i,0)+4*id(i,0),i2,i3,c)+    21.*u(ia(i,0)+5*id(i,
     & 0),i2,i3,c)-    7.*u(ia(i,0)+6*id(i,0),i2,i3,c)+    u(ia(i,0)+
     & 7*id(i,0),i2,i3,c))
         end do
         end do
        end if

       else if( orderOfExtrapolation.eq.8 )then

        if( nd.eq.2 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),i3,c)=(8.*u(ia(i,0)+id(i,0),ia(i,1)+id(
     & i,1),i3,c)-    28.*u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),i3,c)+
     &     56.*u(ia(i,0)+3*id(i,0),ia(i,1)+3*id(i,1),i3,c)-    70.*u(
     & ia(i,0)+4*id(i,0),ia(i,1)+4*id(i,1),i3,c)+    56.*u(ia(i,0)+5*
     & id(i,0),ia(i,1)+5*id(i,1),i3,c)-    28.*u(ia(i,0)+6*id(i,0),ia(
     & i,1)+6*id(i,1),i3,c)+    8.*u(ia(i,0)+7*id(i,0),ia(i,1)+7*id(i,
     & 1),i3,c)-    u(ia(i,0)+8*id(i,0),ia(i,1)+8*id(i,1),i3,c))
           end do
           end do
        else if( nd.eq.3 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),ia(i,2),c)=(8.*u(ia(i,0)+id(i,0),ia(i,1)
     & +id(i,1),ia(i,2)+id(i,2),c)-     28.*u(ia(i,0)+2*id(i,0),ia(i,
     & 1)+2*id(i,1),ia(i,2)+2*id(i,2),c)+     56.*u(ia(i,0)+3*id(i,0),
     & ia(i,1)+3*id(i,1),ia(i,2)+3*id(i,2),c)-     70.*u(ia(i,0)+4*id(
     & i,0),ia(i,1)+4*id(i,1),ia(i,2)+4*id(i,2),c)+     56.*u(ia(i,0)+
     & 5*id(i,0),ia(i,1)+5*id(i,1),ia(i,2)+5*id(i,2),c)-     28.*u(ia(
     & i,0)+6*id(i,0),ia(i,1)+6*id(i,1),ia(i,2)+6*id(i,2),c)+     8.*
     & u(ia(i,0)+7*id(i,0),ia(i,1)+7*id(i,1),ia(i,2)+7*id(i,2),c)-    
     &  u(ia(i,0)+8*id(i,0),ia(i,1)+8*id(i,1),ia(i,2)+8*id(i,2),c))
           end do
           end do
        else
         do c=ca,cb
         do i=n1a,n1b
           u(ia(i,0),i2,i3,c)=(8.*u(ia(i,0)+id(i,0),i2,i3,c)-    28.*u(
     & ia(i,0)+2*id(i,0),i2,i3,c)+    56.*u(ia(i,0)+3*id(i,0),i2,i3,c)
     & -    70.*u(ia(i,0)+4*id(i,0),i2,i3,c)+    56.*u(ia(i,0)+5*id(i,
     & 0),i2,i3,c)-    28.*u(ia(i,0)+6*id(i,0),i2,i3,c)+    8.*u(ia(i,
     & 0)+7*id(i,0),i2,i3,c)-    u(ia(i,0)+8*id(i,0),i2,i3,c))
         end do
         end do
        end if

       else if( orderOfExtrapolation.eq.9 )then

        if( nd.eq.2 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),i3,c)=(9.*u(ia(i,0)+id(i,0),ia(i,1)+id(
     & i,1),i3,c)-    36.*u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),i3,c)+
     &     84.*u(ia(i,0)+3*id(i,0),ia(i,1)+3*id(i,1),i3,c)-    126.*u(
     & ia(i,0)+4*id(i,0),ia(i,1)+4*id(i,1),i3,c)+    126.*u(ia(i,0)+5*
     & id(i,0),ia(i,1)+5*id(i,1),i3,c)-    84.*u(ia(i,0)+6*id(i,0),ia(
     & i,1)+6*id(i,1),i3,c)+    36.*u(ia(i,0)+7*id(i,0),ia(i,1)+7*id(
     & i,1),i3,c)-    9.*u(ia(i,0)+8*id(i,0),ia(i,1)+8*id(i,1),i3,c)+ 
     &    u(ia(i,0)+9*id(i,0),ia(i,1)+9*id(i,1),i3,c))
           end do
           end do
        else if( nd.eq.3 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),ia(i,2),c)=(9.*u(ia(i,0)+id(i,0),ia(i,1)
     & +id(i,1),ia(i,2)+id(i,2),c)-     36.*u(ia(i,0)+2*id(i,0),ia(i,
     & 1)+2*id(i,1),ia(i,2)+2*id(i,2),c)+     84.*u(ia(i,0)+3*id(i,0),
     & ia(i,1)+3*id(i,1),ia(i,2)+3*id(i,2),c)-     126.*u(ia(i,0)+4*
     & id(i,0),ia(i,1)+4*id(i,1),ia(i,2)+4*id(i,2),c)+     126.*u(ia(
     & i,0)+5*id(i,0),ia(i,1)+5*id(i,1),ia(i,2)+5*id(i,2),c)-     84.*
     & u(ia(i,0)+6*id(i,0),ia(i,1)+6*id(i,1),ia(i,2)+6*id(i,2),c)+    
     &  36.*u(ia(i,0)+7*id(i,0),ia(i,1)+7*id(i,1),ia(i,2)+7*id(i,2),c)
     & -     9.*u(ia(i,0)+8*id(i,0),ia(i,1)+8*id(i,1),ia(i,2)+8*id(i,
     & 2),c)+     u(ia(i,0)+9*id(i,0),ia(i,1)+9*id(i,1),ia(i,2)+9*id(
     & i,2),c))
           end do
           end do
        else
         do c=ca,cb
         do i=n1a,n1b
           u(ia(i,0),i2,i3,c)=(9.*u(ia(i,0)+id(i,0),i2,i3,c)-    36.*u(
     & ia(i,0)+2*id(i,0),i2,i3,c)+    84.*u(ia(i,0)+3*id(i,0),i2,i3,c)
     & -    126.*u(ia(i,0)+4*id(i,0),i2,i3,c)+    126.*u(ia(i,0)+5*id(
     & i,0),i2,i3,c)-    84.*u(ia(i,0)+6*id(i,0),i2,i3,c)+    36.*u(
     & ia(i,0)+7*id(i,0),i2,i3,c)-    9.*u(ia(i,0)+8*id(i,0),i2,i3,c)+
     &     u(ia(i,0)+9*id(i,0),i2,i3,c))
         end do
         end do
        end if

       else
       write(*,*) 'extrapInterpNeighboursOpt:ERROR:Not implemented: '
         write(*,*) ' order of extrapolation=',orderOfExtrapolation
         stop 1
       end if

      else
        ! *********************************
        ! *** extrapolate with limiters ***
        ! *********************************

       ! write(*,'("extrapInterpNeighbours: use limiter, order=",i2," limiterPar0=",e9.2)')!    orderOfExtrapolation,limiterPar0
       ! '

       if( orderOfExtrapolation.eq.3 .or. orderOfExtrapolation.le.0 ) 
     & then
       if( nd.eq.2 ) then
           ! loops(u(ia(i,0),ia(i,1),i3,c)=(3.*u(ia(i,0)+  id(i,0),ia(i,1)+  id(i,1),i3,c)- 	   ! 			         3.*u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),i3,c)+ 	   ! 			            u(ia(i,0)+3*id(i,0),ia(i,1)+3*id(i,1),i3,c)) )
           do c=ca,cb
           do i=n1a,n1b
             i1=ia(i,0)
             i2=ia(i,1)
             is1=id(i,0)
             is2=id(i,1)

              ue3 = 3.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  - 3.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  +    u(i1+3*(is1),i2+3*(
     & is2),i3+3*(is3),c)
              ue1 =    u(i1+is1,i2+is2,i3+is3,c)
              ue2 = 2.*u(i1+is1,i2+is2,i3+is3,c)-u(i1+2*is1,i2+2*is2,
     & i3+2*is3,c)
              denom =abs(ue3)+abs(u(i1+is1,i2+is2,i3+is3,c))+abs(u(i1+
     & 2*is1,i2+2*is2,i3+2*is3,c))+uEps
              ! alpha measures the difference between the high-order extrapolated value, ue, and
              !    the first and second-order extrapolated values. 
              alpha = limiterPar0*(abs(ue3-ue1)+abs(ue3-ue2))/denom
              alpha =min(1.,alpha)
             !** u(i1,i2,i3,c)=(1.-alpha)*ue3+alpha*ue2
              u(i1,i2,i3,c)=(1.-alpha)*ue3+alpha*ue1

           end do
           end do
       else if( nd.eq.3 ) then
           ! loops(u(ia(i,0),ia(i,1),ia(i,2),c)=(3.*u(ia(i,0)+  id(i,0),ia(i,1)+  id(i,1),ia(i,2)+  id(i,2),c)-	   !				      3.*u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),ia(i,2)+2*id(i,2),c)+	   ! 				         u(ia(i,0)+3*id(i,0),ia(i,1)+3*id(i,1),ia(i,2)+3*id(i,2),c)))
           do c=ca,cb
           do i=n1a,n1b
             i1=ia(i,0)
             i2=ia(i,1)
             i3=ia(i,2)
             is1=id(i,0)
             is2=id(i,1)
             is3=id(i,2)

              ue3 = 3.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  - 3.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  +    u(i1+3*(is1),i2+3*(
     & is2),i3+3*(is3),c)
              ue1 =    u(i1+is1,i2+is2,i3+is3,c)
              ue2 = 2.*u(i1+is1,i2+is2,i3+is3,c)-u(i1+2*is1,i2+2*is2,
     & i3+2*is3,c)
              denom =abs(ue3)+abs(u(i1+is1,i2+is2,i3+is3,c))+abs(u(i1+
     & 2*is1,i2+2*is2,i3+2*is3,c))+uEps
              ! alpha measures the difference between the high-order extrapolated value, ue, and
              !    the first and second-order extrapolated values. 
              alpha = limiterPar0*(abs(ue3-ue1)+abs(ue3-ue2))/denom
              alpha =min(1.,alpha)
             !** u(i1,i2,i3,c)=(1.-alpha)*ue3+alpha*ue2
              u(i1,i2,i3,c)=(1.-alpha)*ue3+alpha*ue1

           end do
           end do
       else
           do c=ca,cb
           do i=n1a,n1b
             i1=ia(i,0)
             is1=id(i,0)
              ue3 = 3.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  - 3.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  +    u(i1+3*(is1),i2+3*(
     & is2),i3+3*(is3),c)
              ue1 =    u(i1+is1,i2+is2,i3+is3,c)
              ue2 = 2.*u(i1+is1,i2+is2,i3+is3,c)-u(i1+2*is1,i2+2*is2,
     & i3+2*is3,c)
              denom =abs(ue3)+abs(u(i1+is1,i2+is2,i3+is3,c))+abs(u(i1+
     & 2*is1,i2+2*is2,i3+2*is3,c))+uEps
              ! alpha measures the difference between the high-order extrapolated value, ue, and
              !    the first and second-order extrapolated values. 
              alpha = limiterPar0*(abs(ue3-ue1)+abs(ue3-ue2))/denom
              alpha =min(1.,alpha)
             !** u(i1,i2,i3,c)=(1.-alpha)*ue3+alpha*ue2
              u(i1,i2,i3,c)=(1.-alpha)*ue3+alpha*ue1
           end do
           end do
         end if
       else if( orderOfExtrapolation.eq.2 ) then

       if( nd.eq.2 ) then
           do c=ca,cb
           do i=n1a,n1b
             i1=ia(i,0)
             i2=ia(i,1)
             is1=id(i,0)
             is2=id(i,1)
              ue1 =    u(i1+is1,i2+is2,i3+is3,c)
              ue2 = 2.*u(i1+is1,i2+is2,i3+is3,c)-u(i1+2*is1,i2+2*is2,
     & i3+2*is3,c)
              denom =abs(ue2)+abs(u(i1+is1,i2+is2,i3+is3,c))+abs(u(i1+
     & 2*is1,i2+2*is2,i3+2*is3,c))+uEps
              ! alpha measures the difference between the second-order extrapolated value, ue2, and
              !    the first order extrapolated value ue1.
              alpha = limiterPar0*abs(ue2-ue1)/denom
              alpha =min(1.,alpha)
              u(i1,i2,i3,c)=(1.-alpha)*ue2+alpha*ue1
           end do
           end do
       else if( nd.eq.3 ) then
           do c=ca,cb
           do i=n1a,n1b
             i1=ia(i,0)
             i2=ia(i,1)
             i3=ia(i,2)
             is1=id(i,0)
             is2=id(i,1)
             is3=id(i,2)
              ue1 =    u(i1+is1,i2+is2,i3+is3,c)
              ue2 = 2.*u(i1+is1,i2+is2,i3+is3,c)-u(i1+2*is1,i2+2*is2,
     & i3+2*is3,c)
              denom =abs(ue2)+abs(u(i1+is1,i2+is2,i3+is3,c))+abs(u(i1+
     & 2*is1,i2+2*is2,i3+2*is3,c))+uEps
              ! alpha measures the difference between the second-order extrapolated value, ue2, and
              !    the first order extrapolated value ue1.
              alpha = limiterPar0*abs(ue2-ue1)/denom
              alpha =min(1.,alpha)
              u(i1,i2,i3,c)=(1.-alpha)*ue2+alpha*ue1
           end do
           end do
       else
           do c=ca,cb
           do i=n1a,n1b
             i1=ia(i,0)
             is1=id(i,0)
              ue1 =    u(i1+is1,i2+is2,i3+is3,c)
              ue2 = 2.*u(i1+is1,i2+is2,i3+is3,c)-u(i1+2*is1,i2+2*is2,
     & i3+2*is3,c)
              denom =abs(ue2)+abs(u(i1+is1,i2+is2,i3+is3,c))+abs(u(i1+
     & 2*is1,i2+2*is2,i3+2*is3,c))+uEps
              ! alpha measures the difference between the second-order extrapolated value, ue2, and
              !    the first order extrapolated value ue1.
              alpha = limiterPar0*abs(ue2-ue1)/denom
              alpha =min(1.,alpha)
              u(i1,i2,i3,c)=(1.-alpha)*ue2+alpha*ue1
           end do
           end do
         end if

       else if( orderOfExtrapolation.eq.1 ) then

       if( nd.eq.2 ) then
           do c=ca,cb
           do i=n1a,n1b
             u(ia(i,0),ia(i,1),i3,c)=u(ia(i,0)+id(i,0),ia(i,1)+id(i,1),
     & i3,c)
           end do
           end do
       else if( nd.eq.3 ) then
         do c=ca,cb
         do i=n1a,n1b
           u(ia(i,0),ia(i,1),ia(i,2),c)=u(ia(i,0)+id(i,0),ia(i,1)+id(i,
     & 1),ia(i,2)+id(i,2),c)
         end do
         end do
       else
         do c=ca,cb
         do i=n1a,n1b
           u(ia(i,0),i2,i3,c)=u(ia(i,0)+id(i,0),i2,i3,c)
         end do
         end do
         end if
       else
       write(*,*) 'extrapInterpNeighboursOpt:ERROR: '
         write(*,*) ' order of extrapolation=',orderOfExtrapolation
         stop 1
       end if


      end if

      if( .false. .and. nd.eq.3 ) then
        do i=n1a,n1b
          write(*,'(" EIN: i=",i4," ia=",3i4," u=",e12.6)') i,ia(i,0),
     & ia(i,1),ia(i,2),u(ia(i,0),ia(i,1),ia(i,2),0)
        end do
      end if

      return
      end








      subroutine extrapolateOpt(nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask, v, ipar, rpar, uC )
c======================================================================
c  Optimised Boundary Conditions ** extrapolate ***
c         
c nd : number of space dimensions
c ca,cb : assign components c=ca...cb
c======================================================================
      implicit none
      integer nd, nda1a,nda1b,ndd1a,ndd1b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & n1a,n1b,n2a,n2b,n3a,n3b
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real v(ndv1a:ndv1b,ndv2a:ndv2b,ndv3a:ndv3b,ndv4a:ndv4b)
      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)
      integer ipar(0:*),uC(0:*)
      real rpar(0:*)

      integer extrapolate,extrapolateNormalComponent,
     & extrapolateTangentialComponent0,
     & extrapolateTangentialComponent1
      parameter( extrapolate=2,
     & extrapolateNormalComponent=20,
     & extrapolateTangentialComponent0=21,
     & extrapolateTangentialComponent1=22 )
c     --- local variables 
      integer i1,i2,i3,c0,c,bcType,ca,cb,orderOfExtrapolation,
     & is1,is2,is3,useWhereMask
      real ue1,ue2,ue3
      real alpha,uEps,denom

      real limiterPar0,limiterPar1
      integer extrapolationOption
      integer polynomialExtrapolation,extrapolateWithLimiter
      parameter( polynomialExtrapolation=0,extrapolateWithLimiter=1 )


      bcType=ipar(0)
      useWhereMask=ipar(1)
      orderOfExtrapolation=ipar(2)
      ca=ipar(3)
      cb=ipar(4)
      is1=ipar(5)
      is2=ipar(6)
      is3=ipar(7)
      extrapolationOption=ipar(8)

      limiterPar0=rpar(0)
      limiterPar1=rpar(1)
      uEps       =rpar(2)

      if( bcType.eq.extrapolate .and. 
     & extrapolationOption.eq.polynomialExtrapolation )then

        if( orderOfExtrapolation.eq.1 )then
          if( useWhereMask.ne.0 )then
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 u(i1,i2,i3,c)=u(i1+is1,i2+is2,i3+is3,c)
               end if
             end do
             end do
             end do
            end do
          else
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               u(i1,i2,i3,c)=u(i1+is1,i2+is2,i3+is3,c)
             end do
             end do
             end do
            end do
          end if
        else if( orderOfExtrapolation.eq.2 )then
          if( useWhereMask.ne.0 )then
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 u(i1,i2,i3,c)=2.*u(i1+(is1),i2+(is2),i3+(is3),c)-u(i1+
     & 2*(is1),i2+2*(is2),i3+2*(is3),c)
               end if
             end do
             end do
             end do
            end do
          else
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               u(i1,i2,i3,c)=2.*u(i1+(is1),i2+(is2),i3+(is3),c)-u(i1+2*
     & (is1),i2+2*(is2),i3+2*(is3),c)
             end do
             end do
             end do
            end do
          end if
        else if( orderOfExtrapolation.eq.3 )then
          if( useWhereMask.ne.0 )then
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 u(i1,i2,i3,c)=3.*u(i1+(is1),i2+(is2),i3+(is3),c)-3.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+u(i1+3*(is1),i2+3*(is2),i3+
     & 3*(is3),c)
               end if
             end do
             end do
             end do
            end do
          else
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               u(i1,i2,i3,c)=3.*u(i1+(is1),i2+(is2),i3+(is3),c)-3.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+u(i1+3*(is1),i2+3*(is2),i3+
     & 3*(is3),c)
             end do
             end do
             end do
            end do
          end if
        else if( orderOfExtrapolation.eq.4 )then
          if( useWhereMask.ne.0 )then
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 u(i1,i2,i3,c)=4.*u(i1+(is1),i2+(is2),i3+(is3),c)-6.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+4.*u(i1+3*(is1),i2+3*(is2),
     & i3+3*(is3),c)-u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)
               end if
             end do
             end do
             end do
            end do
          else
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               u(i1,i2,i3,c)=4.*u(i1+(is1),i2+(is2),i3+(is3),c)-6.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+4.*u(i1+3*(is1),i2+3*(is2),
     & i3+3*(is3),c)-u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)
             end do
             end do
             end do
            end do
          end if
        else if( orderOfExtrapolation.eq.5 )then
          if( useWhereMask.ne.0 )then
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 u(i1,i2,i3,c)=5.*u(i1+(is1),i2+(is2),i3+(is3),c)-10.*
     & u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+10.*u(i1+3*(is1),i2+3*(
     & is2),i3+3*(is3),c)-5.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+u(
     & i1+5*(is1),i2+5*(is2),i3+5*(is3),c)
               end if
             end do
             end do
             end do
            end do
          else
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               u(i1,i2,i3,c)=5.*u(i1+(is1),i2+(is2),i3+(is3),c)-10.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+10.*u(i1+3*(is1),i2+3*(is2)
     & ,i3+3*(is3),c)-5.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+u(i1+5*
     & (is1),i2+5*(is2),i3+5*(is3),c)
             end do
             end do
             end do
            end do
          end if
        else if( orderOfExtrapolation.eq.6 )then
          if( useWhereMask.ne.0 )then
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 u(i1,i2,i3,c)=6.*u(i1+(is1),i2+(is2),i3+(is3),c)-15.*
     & u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+20.*u(i1+3*(is1),i2+3*(
     & is2),i3+3*(is3),c)-15.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+
     & 6.*u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c)-u(i1+6*(is1),i2+6*(
     & is2),i3+6*(is3),c)
               end if
             end do
             end do
             end do
            end do
          else
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               u(i1,i2,i3,c)=6.*u(i1+(is1),i2+(is2),i3+(is3),c)-15.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+20.*u(i1+3*(is1),i2+3*(is2)
     & ,i3+3*(is3),c)-15.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+6.*u(
     & i1+5*(is1),i2+5*(is2),i3+5*(is3),c)-u(i1+6*(is1),i2+6*(is2),i3+
     & 6*(is3),c)
             end do
             end do
             end do
            end do
          end if
        else if( orderOfExtrapolation.eq.7 )then
          if( useWhereMask.ne.0 )then
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 u(i1,i2,i3,c)=7.*u(i1+(is1),i2+(is2),i3+(is3),c)-21.*
     & u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+35.*u(i1+3*(is1),i2+3*(
     & is2),i3+3*(is3),c)-35.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+
     & 21.*u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c)-7.*u(i1+6*(is1),i2+6*
     & (is2),i3+6*(is3),c)+u(i1+7*(is1),i2+7*(is2),i3+7*(is3),c)
               end if
             end do
             end do
             end do
            end do
          else
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               u(i1,i2,i3,c)=7.*u(i1+(is1),i2+(is2),i3+(is3),c)-21.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+35.*u(i1+3*(is1),i2+3*(is2)
     & ,i3+3*(is3),c)-35.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+21.*u(
     & i1+5*(is1),i2+5*(is2),i3+5*(is3),c)-7.*u(i1+6*(is1),i2+6*(is2),
     & i3+6*(is3),c)+u(i1+7*(is1),i2+7*(is2),i3+7*(is3),c)
             end do
             end do
             end do
            end do
          end if
        else if( orderOfExtrapolation.eq.8 )then
          if( useWhereMask.ne.0 )then
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 u(i1,i2,i3,c)=8.*u(i1+(is1),i2+(is2),i3+(is3),c)-28.*
     & u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+56.*u(i1+3*(is1),i2+3*(
     & is2),i3+3*(is3),c)-70.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+
     & 56.*u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c)-28.*u(i1+6*(is1),i2+
     & 6*(is2),i3+6*(is3),c)+8.*u(i1+7*(is1),i2+7*(is2),i3+7*(is3),c)-
     & u(i1+8*(is1),i2+8*(is2),i3+8*(is3),c)
               end if
             end do
             end do
             end do
            end do
          else
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               u(i1,i2,i3,c)=8.*u(i1+(is1),i2+(is2),i3+(is3),c)-28.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+56.*u(i1+3*(is1),i2+3*(is2)
     & ,i3+3*(is3),c)-70.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+56.*u(
     & i1+5*(is1),i2+5*(is2),i3+5*(is3),c)-28.*u(i1+6*(is1),i2+6*(is2)
     & ,i3+6*(is3),c)+8.*u(i1+7*(is1),i2+7*(is2),i3+7*(is3),c)-u(i1+8*
     & (is1),i2+8*(is2),i3+8*(is3),c)
             end do
             end do
             end do
            end do
          end if
        else if( orderOfExtrapolation.eq.9 )then
          if( useWhereMask.ne.0 )then
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 u(i1,i2,i3,c)=9.*u(i1+(is1),i2+(is2),i3+(is3),c)-36.*
     & u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+84.*u(i1+3*(is1),i2+3*(
     & is2),i3+3*(is3),c)-126.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+
     & 126.*u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c)-84.*u(i1+6*(is1),i2+
     & 6*(is2),i3+6*(is3),c)+36.*u(i1+7*(is1),i2+7*(is2),i3+7*(is3),c)
     & -9.*u(i1+8*(is1),i2+8*(is2),i3+8*(is3),c)+u(i1+9*(is1),i2+9*(
     & is2),i3+9*(is3),c)
               end if
             end do
             end do
             end do
            end do
          else
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               u(i1,i2,i3,c)=9.*u(i1+(is1),i2+(is2),i3+(is3),c)-36.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+84.*u(i1+3*(is1),i2+3*(is2)
     & ,i3+3*(is3),c)-126.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+126.*
     & u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c)-84.*u(i1+6*(is1),i2+6*(
     & is2),i3+6*(is3),c)+36.*u(i1+7*(is1),i2+7*(is2),i3+7*(is3),c)-
     & 9.*u(i1+8*(is1),i2+8*(is2),i3+8*(is3),c)+u(i1+9*(is1),i2+9*(
     & is2),i3+9*(is3),c)
             end do
             end do
             end do
            end do
          end if
        else
          write(*,*) 'extrapolateOpt:Error: '
          write(*,*) 'unable to extrapolate '
          write(*,*) ' to orderOfExtrapolation',orderOfExtrapolation
          write(*,*) ' can only do orders 1 to 9.'
          stop 1
        end if


      else if( bcType.eq.extrapolate .and. 
     & extrapolationOption.eq.extrapolateWithLimiter )then

        ! Extrapolate with a limiter -- reduce the order of the extrapolation if the solution
        !   is not smooth

        ! write(*,'("INFO:extrap with limiter, order=",i2," limiterPar0=",e9.2)') orderOfExtrapolation,limiterPar0
        ! '
        if( orderOfExtrapolation.eq.1 )then

          ! no need to limit the first order extrapolation
          if( useWhereMask.ne.0 )then
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 u(i1,i2,i3,c)=u(i1+is1,i2+is2,i3+is3,c)
               end if
             end do
             end do
             end do
            end do
          else
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               u(i1,i2,i3,c)=u(i1+is1,i2+is2,i3+is3,c)
             end do
             end do
             end do
            end do
          end if

        else if( orderOfExtrapolation.eq.2 )then

          ! reduce the 2nd-order extrapolation to first order where the solution is not smooth
          if( useWhereMask.eq.0 )then
             do c0=ca,cb
              c=uC(c0)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
               ue1 =    u(i1+is1,i2+is2,i3+is3,c)
               ue2 = 2.*u(i1+is1,i2+is2,i3+is3,c)-u(i1+2*is1,i2+2*is2,
     & i3+2*is3,c)
               denom =abs(ue2)+abs(u(i1+is1,i2+is2,i3+is3,c))+abs(u(i1+
     & 2*is1,i2+2*is2,i3+2*is3,c))+uEps
               ! alpha measures the difference between the second-order extrapolated value, ue2, and
               !    the first order extrapolated value ue1.
               alpha = limiterPar0*abs(ue2-ue1)/denom
               alpha =min(1.,alpha)
               u(i1,i2,i3,c)=(1.-alpha)*ue2+alpha*ue1
              end do
              end do
              end do
             end do
          else
             do c0=ca,cb
              c=uC(c0)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
               ue1 =    u(i1+is1,i2+is2,i3+is3,c)
               ue2 = 2.*u(i1+is1,i2+is2,i3+is3,c)-u(i1+2*is1,i2+2*is2,
     & i3+2*is3,c)
               denom =abs(ue2)+abs(u(i1+is1,i2+is2,i3+is3,c))+abs(u(i1+
     & 2*is1,i2+2*is2,i3+2*is3,c))+uEps
               ! alpha measures the difference between the second-order extrapolated value, ue2, and
               !    the first order extrapolated value ue1.
               alpha = limiterPar0*abs(ue2-ue1)/denom
               alpha =min(1.,alpha)
               u(i1,i2,i3,c)=(1.-alpha)*ue2+alpha*ue1
                end if
              end do
              end do
              end do
             end do
          end if

        else if( orderOfExtrapolation.eq.3 )then

          ! reduce the 3rd-order extrapolation to first order where the solution is not smooth
          ! if alpha==1 we could potentially reduce the order even further by checking abs(ue2-ue1)
          if( useWhereMask.eq.0 )then
             do c0=ca,cb
              c=uC(c0)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
               ue3 = 3.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  - 3.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  +    u(i1+3*(is1),i2+3*(
     & is2),i3+3*(is3),c)
               ue1 =    u(i1+is1,i2+is2,i3+is3,c)
               ue2 = 2.*u(i1+is1,i2+is2,i3+is3,c)-u(i1+2*is1,i2+2*is2,
     & i3+2*is3,c)
               denom =abs(ue3)+abs(u(i1+is1,i2+is2,i3+is3,c))+abs(u(i1+
     & 2*is1,i2+2*is2,i3+2*is3,c))+uEps
               ! alpha measures the difference between the high-order extrapolated value, ue, and
               !    the first and second-order extrapolated values. 
               alpha = limiterPar0*(abs(ue3-ue1)+abs(ue3-ue2))/denom
               alpha =min(1.,alpha)
              !** u(i1,i2,i3,c)=(1.-alpha)*ue3+alpha*ue2
               u(i1,i2,i3,c)=(1.-alpha)*ue3+alpha*ue1
              end do
              end do
              end do
             end do
          else
             do c0=ca,cb
              c=uC(c0)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
               ue3 = 3.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  - 3.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  +    u(i1+3*(is1),i2+3*(
     & is2),i3+3*(is3),c)
               ue1 =    u(i1+is1,i2+is2,i3+is3,c)
               ue2 = 2.*u(i1+is1,i2+is2,i3+is3,c)-u(i1+2*is1,i2+2*is2,
     & i3+2*is3,c)
               denom =abs(ue3)+abs(u(i1+is1,i2+is2,i3+is3,c))+abs(u(i1+
     & 2*is1,i2+2*is2,i3+2*is3,c))+uEps
               ! alpha measures the difference between the high-order extrapolated value, ue, and
               !    the first and second-order extrapolated values. 
               alpha = limiterPar0*(abs(ue3-ue1)+abs(ue3-ue2))/denom
               alpha =min(1.,alpha)
              !** u(i1,i2,i3,c)=(1.-alpha)*ue3+alpha*ue2
               u(i1,i2,i3,c)=(1.-alpha)*ue3+alpha*ue1
                end if
              end do
              end do
              end do
             end do
          end if

        else if( orderOfExtrapolation.eq.4 )then
          write(*,'("WARNING: orderOfExtrapolation.eq.4 NOT limited!")')

          if( useWhereMask.ne.0 )then
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 u(i1,i2,i3,c)=4.*u(i1+(is1),i2+(is2),i3+(is3),c)-6.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+4.*u(i1+3*(is1),i2+3*(is2),
     & i3+3*(is3),c)-u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)
               end if
             end do
             end do
             end do
            end do
          else
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               u(i1,i2,i3,c)=4.*u(i1+(is1),i2+(is2),i3+(is3),c)-6.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+4.*u(i1+3*(is1),i2+3*(is2),
     & i3+3*(is3),c)-u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)
             end do
             end do
             end do
            end do
          end if
        else if( orderOfExtrapolation.eq.5 )then
          write(*,'("WARNING: orderOfExtrapolation.eq.5 NOT limited!")')

          if( useWhereMask.ne.0 )then
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 u(i1,i2,i3,c)=5.*u(i1+(is1),i2+(is2),i3+(is3),c)-10.*
     & u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+10.*u(i1+3*(is1),i2+3*(
     & is2),i3+3*(is3),c)-5.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+u(
     & i1+5*(is1),i2+5*(is2),i3+5*(is3),c)
               end if
             end do
             end do
             end do
            end do
          else
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               u(i1,i2,i3,c)=5.*u(i1+(is1),i2+(is2),i3+(is3),c)-10.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+10.*u(i1+3*(is1),i2+3*(is2)
     & ,i3+3*(is3),c)-5.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+u(i1+5*
     & (is1),i2+5*(is2),i3+5*(is3),c)
             end do
             end do
             end do
            end do
          end if
        else if( orderOfExtrapolation.eq.6 )then
          write(*,'("WARNING: orderOfExtrapolation.eq.5 NOT limited!")')

          if( useWhereMask.ne.0 )then
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               if( mask(i1,i2,i3).ne.0 )then
                 u(i1,i2,i3,c)=6.*u(i1+(is1),i2+(is2),i3+(is3),c)-15.*
     & u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+20.*u(i1+3*(is1),i2+3*(
     & is2),i3+3*(is3),c)-15.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+
     & 6.*u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c)-u(i1+6*(is1),i2+6*(
     & is2),i3+6*(is3),c)
               end if
             end do
             end do
             end do
            end do
          else
            do c0=ca,cb
             c=uC(c0)
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
               u(i1,i2,i3,c)=6.*u(i1+(is1),i2+(is2),i3+(is3),c)-15.*u(
     & i1+2*(is1),i2+2*(is2),i3+2*(is3),c)+20.*u(i1+3*(is1),i2+3*(is2)
     & ,i3+3*(is3),c)-15.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)+6.*u(
     & i1+5*(is1),i2+5*(is2),i3+5*(is3),c)-u(i1+6*(is1),i2+6*(is2),i3+
     & 6*(is3),c)
             end do
             end do
             end do
            end do
          end if
        else
          write(*,*) 'extrapolateOpt:Error: '
          write(*,*) 'unable to extrapolate '
          write(*,*) ' to orderOfExtrapolation',orderOfExtrapolation
          write(*,*) ' can only do orders 1 to 6.'
          stop 1
        end if




c      else if( bcType.eq.extrapolateNormalComponent .or.
c               bcType.eq.extrapolateTangentialComponent0 .or.
c               bcType.eq.extrapolateTangentialComponent1 )then
c
c
c#beginMacro assignExtrap(rhs)
c        if( nd.eq.1 )
c        loops(temp=rhs,c              u(i1,i2,i3,n1)=u(i1,i2,i3,n1)+temp*v(i1,i2,i3,v0),,)
c        else if( nd.eq.2 )then
c        loops(temp=rhs,c              u(i1,i2,i3,n1)=u(i1,i2,i3,n1)+temp*v(i1,i2,i3,v0),c              u(i1,i2,i3,n2)=u(i1,i2,i3,n2)+temp*v(i1,i2,i3,v1),)
c        else if( nd.eq.3 )then
c        loops(temp=rhs,c              u(i1,i2,i3,n1)=u(i1,i2,i3,n1)+temp*v(i1,i2,i3,v0),c              u(i1,i2,i3,n2)=u(i1,i2,i3,n2)+temp*v(i1,i2,i3,v1),c              u(i1,i2,i3,n3)=u(i1,i2,i3,n3)+temp*v(i1,i2,i3,v2))
c        end if
c#endMacro
c
c       
c        if( orderOfExtrapolation.eq.1 )then
c          assignExtrap(-(u(i1,i2,i3,n1)*v(i1,i2,i3,v0)+u(i1,i2,i3,n2)*v(i1,i2,i3,v1)+u(i1,i2,i3,n3)*v(i1,i2,i3,v2))+c        else if( orderOfExtrapolation.eq.2 )then
c          loopse(u(i1,i2,i3,c)=2.*u(i1+  (is1),i2+  (is2),i3+  (is3),c) c          -    u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c))
c        else if( orderOfExtrapolation.eq.3 )then
c          loopse(u(i1,i2,i3,c)= 3.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  c          - 3.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  c          +    u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c))
c        else if( orderOfExtrapolation.eq.4 )then
c          loopse(u(i1,i2,i3,c)=4.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  c          - 6.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  c          + 4.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  c          -    u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c))
c        else if( orderOfExtrapolation.eq.5 )then
c          loopse(u(i1,i2,i3,c)=5.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  c          -10.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  c          +10.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  c          - 5.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)  c          +    u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c))
c        else if( orderOfExtrapolation.eq.6 )then
c          loopse(u(i1,i2,i3,c)=6.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  c          -15.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  c          +20.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  c          -15.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)  c          + 6.*u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c)  c          -    u(i1+6*(is1),i2+6*(is2),i3+6*(is3),c))
c        else 
c          write(*,*) 'extrapolateOpt:Error: '
c          write(*,*) 'unable to extrapolate '
c          write(*,*) ' to orderOfExtrapolation',orderOfExtrapolation
c          write(*,*) ' can only do orders 1 to 6.'
c          stop 1
c        end if


      else
      write(*,*) 'extrapolateOpt:ERROR: '
        write(*,*) ' unknown bcType=',bcType
        stop 1
      end if

      return
      end

