c *******************************************************************************
c   Interface boundary conditions
c *******************************************************************************

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffNewerOrder2f.h"
#Include "defineDiffNewerOrder4f.h"


#beginMacro beginLoops(n1a,n1b,n2a,n2b,n3a,n3b,na,nb)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
do n=na,nb
  ! write(*,'(" periodic i1,i2,i3,n=",4i4)') i1,i2,i3,n
#endMacro

#beginMacro endLoops()
end do
end do
end do
end do
#endMacro


#beginMacro beginLoops2d()
 i3=n3a
 j3=m3a

 j2=m2a
 do i2=n2a,n2b
  j1=m1a
  do i1=n1a,n1b
#endMacro
#beginMacro endLoops2d()
   j1=j1+1
  end do
  j2=j2+1
 end do
#endMacro

#beginMacro beginGhostLoops2d()
 i3=n3a
 j3=m3a
 j2=mm2a
 do i2=nn2a,nn2b
  j1=mm1a
  do i1=nn1a,nn1b
#endMacro

#defineMacro extrap2(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (2.*uu(k1,k2,k3,kc)-uu(k1+ks1,k2+ks2,k3+ks3,kc))

#defineMacro extrap3(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (3.*uu(k1,k2,k3,kc)-3.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +   uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc))

#defineMacro extrap4(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (4.*uu(k1,k2,k3,kc)-6.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +4.*uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc)-uu(k1+3*ks1,k2+3*ks2,k3+3*ks3,kc))

#defineMacro extrap5(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (5.*uu(k1,k2,k3,kc)-10.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +10.*uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc)-5.*uu(k1+3*ks1,k2+3*ks2,k3+3*ks3,kc)\
            +uu(k1+4*ks1,k2+4*ks2,k3+4*ks3,kc))

#defineMacro extrap6(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (6.*uu(k1,k2,k3,kc)-15.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +20.*uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc)-15.*uu(k1+3*ks1,k2+3*ks2,k3+3*ks3,kc)\
            +6.*uu(k1+4*ks1,k2+4*ks2,k3+4*ks3,kc)-uu(k1+5*ks1,k2+5*ks2,k3+5*ks3,kc))

c This macro will assign the jump conditions on the boundary
c DIM (input): number of dimensions (2 or 3)
c GRIDTYPE (input) : curvilinear or rectangular
#beginMacro boundaryJumpConditions(DIM,GRIDTYPE)
 #If #DIM eq "2"
  if( eps1.lt.eps2 )then
    epsRatio=eps1/eps2
    beginGhostLoops2d()
      ! eps2 n.u2 = eps1 n.u1
      !     tau.u2 = tau.u1

      #If #GRIDTYPE eq "curvilinear"
       an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
       an2=rsxy1(i1,i2,i3,axis1,1)
       aNorm=max(epsx,sqrt(an1**2+an2**2))
       an1=an1/aNorm
       an2=an2/aNorm
      #Elif #GRIDTYPE eq "rectangular"
       an1=an1Cartesian
       an2=an2Cartesian
      #Else
         stop 1111
      #End
      ua=u1(i1,i2,i3,ex)
      ub=u1(i1,i2,i3,ey)
      nDotU = an1*ua+an2*ub
      ! u2 equals u1 but with normal component = eps1/eps2*(n.u1)
      u2(j1,j2,j3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
      u2(j1,j2,j3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
      u2(j1,j2,j3,hz) = u1(i1,i2,i3,hz)
    endLoops2d()
  else
    epsRatio=eps2/eps1
    beginGhostLoops2d()
      ! eps2 n.u2 = eps1 n.u1
      !     tau.u2 = tau.u1

      #If #GRIDTYPE eq "curvilinear"
       an1=rsxy1(i1,i2,i3,axis1,0)
       an2=rsxy1(i1,i2,i3,axis1,1)
       aNorm=max(epsx,sqrt(an1**2+an2**2))
       an1=an1/aNorm
       an2=an2/aNorm
      #Elif #GRIDTYPE eq "rectangular"
       an1=an1Cartesian
       an2=an2Cartesian
      #Else
        stop 1112
      #End
      ua=u2(j1,j2,j3,ex)
      ub=u2(j1,j2,j3,ey)

      nDotU = an1*ua+an2*ub

      u1(i1,i2,i3,ex) = ua + (nDotU*epsRatio - nDotU)*an1
      u1(i1,i2,i3,ey) = ub + (nDotU*epsRatio - nDotU)*an2
      u1(i1,i2,i3,hz) = u2(j1,j2,j3,hz)
    endLoops2d()
  end if
 #Else
   stop 7742
 #End
#endMacro

c ** Precompute the derivatives of rsxy ***
c assign rvx(m) = (rx,sy)
c        rvxx(m) = (rxx,sxx)
#beginMacro computeRxDerivatives(rv,rsxy,i1,i2,i3)
do m=0,nd-1
 rv ## x(m)   =rsxy(i1,i2,i3,m,0)
 rv ## y(m)   =rsxy(i1,i2,i3,m,1)

 rv ## xx(m)  =rsxy ## x22(i1,i2,i3,m,0)
 rv ## xy(m)  =rsxy ## x22(i1,i2,i3,m,1)
 rv ## yy(m)  =rsxy ## y22(i1,i2,i3,m,1)

 rv ## xxx(m) =rsxy ## xx22(i1,i2,i3,m,0)
 rv ## xxy(m) =rsxy ## xx22(i1,i2,i3,m,1)
 rv ## xyy(m) =rsxy ## xy22(i1,i2,i3,m,1)
 rv ## yyy(m) =rsxy ## yy22(i1,i2,i3,m,1)

 rv ## xxxx(m)=rsxy ## xxx22(i1,i2,i3,m,0)
 rv ## xxyy(m)=rsxy ## xyy22(i1,i2,i3,m,0)
 rv ## yyyy(m)=rsxy ## yyy22(i1,i2,i3,m,1)
end do
#endMacro

c assign some temporary variables that are used in the evaluation of the operators
#beginMacro setJacobian(rv,axis1,axisp1)
 rx   =rv ## x(axis1)   
 ry   =rv ## y(axis1)   
                    
 rxx  =rv ## xx(axis1)  
 rxy  =rv ## xy(axis1)  
 ryy  =rv ## yy(axis1)  
                    
 rxxx =rv ## xxx(axis1) 
 rxxy =rv ## xxy(axis1) 
 rxyy =rv ## xyy(axis1) 
 ryyy =rv ## yyy(axis1) 
                    
 rxxxx=rv ## xxxx(axis1)
 rxxyy=rv ## xxyy(axis1)
 ryyyy=rv ## yyyy(axis1)

 sx   =rv ## x(axis1p1)   
 sy   =rv ## y(axis1p1)   
                    
 sxx  =rv ## xx(axis1p1)  
 sxy  =rv ## xy(axis1p1)  
 syy  =rv ## yy(axis1p1)  
                    
 sxxx =rv ## xxx(axis1p1) 
 sxxy =rv ## xxy(axis1p1) 
 sxyy =rv ## xyy(axis1p1) 
 syyy =rv ## yyy(axis1p1) 
                    
 sxxxx=rv ## xxxx(axis1p1)
 sxxyy=rv ## xxyy(axis1p1)
 syyyy=rv ## yyyy(axis1p1)

#endMacro


! update the periodic ghost points  -- serial only --
#beginMacro periodicUpdate2d(u,bc,gid,side,axis)
if( parallel.eq.0 )then
 axisp1=mod(axis+1,nd)
 if( bc(0,axisp1).lt.0 )then
  ! direction axisp1 is periodic
  diff(axis)=0
  diff(axisp1)=gid(1,axisp1)-gid(0,axisp1)

  if( side.eq.0 )then
    ! assign 4 ghost points outside lower corner
    np1a=gid(0,0)-2
    np1b=gid(0,0)-1
    np2a=gid(0,1)-2
    np2b=gid(0,1)-1

    beginLoops(np1a,np1b,np2a,np2b,n3a,n3b,ex,hz)
     u(i1,i2,i3,n) = u(i1+diff(0),i2+diff(1),i3,n)
    endLoops()

    ! assign 4 ghost points outside upper corner
    if( axis.eq.0 )then
      np2a=gid(1,axisp1)+1
      np2b=gid(1,axisp1)+2
    else
      np1a=gid(1,axisp1)+1
      np1b=gid(1,axisp1)+2
    end if

    beginLoops(np1a,np1b,np2a,np2b,n3a,n3b,ex,hz)
     u(i1,i2,i3,n) = u(i1-diff(0),i2-diff(1),i3,n)
    endLoops()

  else

    ! assign 4 ghost points outside upper corner
    np1a=gid(1,0)+1
    np1b=gid(1,0)+2
    np2a=gid(1,1)+1
    np2b=gid(1,1)+2

    beginLoops(np1a,np1b,np2a,np2b,n3a,n3b,ex,hz)
     u(i1,i2,i3,n) = u(i1-diff(0),i2-diff(1),i3,n)
    endLoops()

    if( axis.eq.0 )then
      np2a=gid(0,axisp1)-2
      np2b=gid(0,axisp1)-1
    else
      np1a=gid(0,axisp1)-2
      np1b=gid(0,axisp1)-1
    end if

    beginLoops(np1a,np1b,np2a,np2b,n3a,n3b,ex,hz)
     u(i1,i2,i3,n) = u(i1+diff(0),i2+diff(1),i3,n)
    endLoops()
  end if

 endif
end if
#endMacro




      subroutine interfaceMaxwell( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                               gridIndexRange1, u1, mask1,rsxy1, xy1, boundaryCondition1, \
                               md1a,md1b,md2a,md2b,md3a,md3b,\
                               gridIndexRange2, u2, mask2,rsxy2, xy2, boundaryCondition2, \
                               ipar, rpar, \
                               aa2,aa4,aa8, ipvt2,ipvt4,ipvt8, \
                               ierr )
! ===================================================================================
!  Interface boundary conditions for Maxwell's Equations.
!
!  gridType : 0=rectangular, 1=curvilinear
!
!  u1: solution on the "left" of the interface
!  u2: solution on the "right" of the interface
!
!  aa2,aa4,aa8 : real work space arrays that must be saved from call to call
!  ipvt2,ipvt4,ipvt8: integer work space arrays that must be saved from call to call
! ===================================================================================

      implicit none

      integer nd, \
              nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, \
              md1a,md1b,md2a,md2b,md3a,md3b, \
              n1a,n1b,n2a,n2b,n3a,n3b,  \
              m1a,m1b,m2a,m2b,m3a,m3b,  \
              ierr

      real u1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer mask1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      integer gridIndexRange1(0:1,0:2),boundaryCondition1(0:1,0:2)

      real u2(md1a:md1b,md2a:md2b,md3a:md3b,0:*)
      integer mask2(md1a:md1b,md2a:md2b,md3a:md3b)
      real rsxy2(md1a:md1b,md2a:md2b,md3a:md3b,0:nd-1,0:nd-1)
      real xy2(md1a:md1b,md2a:md2b,md3a:md3b,0:nd-1)
      integer gridIndexRange2(0:1,0:2),boundaryCondition2(0:1,0:2)

      integer ipar(0:*)
      real rpar(0:*)

      ! work space arrays that must be saved from call to call:
      real aa2(0:1,0:1,0:1,0:*),aa4(0:3,0:3,0:1,0:*),aa8(0:7,0:7,0:1,0:*)
      integer ipvt2(0:1,0:*), ipvt4(0:3,0:*), ipvt8(0:7,0:*)

!     --- local variables ----
      
      integer side1,axis1,grid1,side2,axis2,grid2,gridType,orderOfAccuracy,orderOfExtrapolation,useForcing,\
        ex,ey,ez,hx,hy,hz,useWhereMask,debug,solveForE,solveForH,axis1p1,axis2p1,nn,n1,n2
      real dx1(0:2),dr1(0:2),dx2(0:2),dr2(0:2)
      real dx(0:2),dr(0:2)
      real t,ep,dt,eps1,mu1,c1,eps2,mu2,c2
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,j1,j2,j3,js1,js2,js3,ks1,ks2,ks3,is,js,it,nit
      integer option,initialized,myid,parallel,bcOption,forcingOption,bc0

      integer numGhost,giveDiv
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
      integer mm1a,mm1b,mm2a,mm2b,mm3a,mm3b

      real rx1,ry1,rx2,ry2

      real aLap0,aLap1,bLap0,bLap1,aLapX0,aLapX1,bLapY0,bLapY1,cLapX0,cLapX1,dLapY0,dLapY1,aLapSq0,aLapSq1,bLapSq0,bLapSq1
      real a0,a1,b0,b1,cc0,cc1,d0,d1,dr0,ds0
      real aNormSq,divu

      integer ipar1(0:30),ipar2(0:30)
      real rpar1(0:30),rpar2(0:30)

      real epsRatio,an1,an2,aNorm,ua,ub,nDotU
      real epsx

      real tau1,tau2,clap1,clap2,ulap1,vlap1,wlap1,ulap2,vlap2,wlap2,an1Cartesian,an2Cartesian
      real ulapSq1,vlapSq1,ulapSq2,vlapSq2,wlapSq1,wlapSq2

      integer np1a,np1b,np2a,np2b,np3a,np3b,diff(0:2)

      real rx,ry,rxx,rxy,ryy,rxxx,rxxy,rxyy,ryyy,rxxxx,rxxyy,ryyyy
      real sx,sy,sxx,sxy,syy,sxxx,sxxy,sxyy,syyy,sxxxx,sxxyy,syyyy

      real rv1x(0:2),rv1y(0:2),rv1xx(0:2),rv1xy(0:2),rv1yy(0:2),rv1xxx(0:2),rv1xxy(0:2),rv1xyy(0:2),rv1yyy(0:2),\
           rv1xxxx(0:2),rv1xxyy(0:2),rv1yyyy(0:2)
      real sv1x(0:2),sv1y(0:2),sv1xx(0:2),sv1xy(0:2),sv1yy(0:2),sv1xxx(0:2),sv1xxy(0:2),sv1xyy(0:2),sv1yyy(0:2),\
           sv1xxxx(0:2),sv1xxyy(0:2),sv1yyyy(0:2)
      real rv2x(0:2),rv2y(0:2),rv2xx(0:2),rv2xy(0:2),rv2yy(0:2),rv2xxx(0:2),rv2xxy(0:2),rv2xyy(0:2),rv2yyy(0:2),\
           rv2xxxx(0:2),rv2xxyy(0:2),rv2yyyy(0:2)
      real sv2x(0:2),sv2y(0:2),sv2xx(0:2),sv2xy(0:2),sv2yy(0:2),sv2xxx(0:2),sv2xxy(0:2),sv2xyy(0:2),sv2yyy(0:2),\
           sv2xxxx(0:2),sv2xxyy(0:2),sv2yyyy(0:2)

      integer numberOfEquations,job
      real a2(0:1,0:1),a4(0:3,0:3),a8(0:7,0:7),a12(0:11,0:11),q(0:11),f(0:11),rcond,work(0:11)
      integer ipvt(0:11)

      real err

      ! boundary conditions parameters
      #Include "bcDefineFortranInclude.h"
 
      integer rectangular,curvilinear
      parameter(\
        rectangular=0,\
        curvilinear=1)


!     --- start statement function ----
      integer kd,m,n
!     real rx,ry,rz,sx,sy,sz,tx,ty,tz
      declareDifferenceNewOrder2(u1,rsxy1,dr1,dx1,RX)
      declareDifferenceNewOrder2(u2,rsxy2,dr2,dx2,RX)

      declareDifferenceNewOrder4(u1,rsxy1,dr1,dx1,RX)
      declareDifferenceNewOrder4(u2,rsxy2,dr2,dx2,RX)

!.......statement functions for jacobian
!     rx(i1,i2,i3)=rsxy1(i1,i2,i3,0,0)
!     ry(i1,i2,i3)=rsxy1(i1,i2,i3,0,1)
!     rz(i1,i2,i3)=rsxy1(i1,i2,i3,0,2)
!     sx(i1,i2,i3)=rsxy1(i1,i2,i3,1,0)
!     sy(i1,i2,i3)=rsxy1(i1,i2,i3,1,1)
!     sz(i1,i2,i3)=rsxy1(i1,i2,i3,1,2)
!     tx(i1,i2,i3)=rsxy1(i1,i2,i3,2,0)
!     ty(i1,i2,i3)=rsxy1(i1,i2,i3,2,1)
!     tz(i1,i2,i3)=rsxy1(i1,i2,i3,2,2) 


!     The next macro call will define the difference approximation statement functions
      defineDifferenceNewOrder2Components1(u1,rsxy1,dr1,dx1,RX)
      defineDifferenceNewOrder2Components1(u2,rsxy2,dr2,dx2,RX)

      defineDifferenceNewOrder4Components1(u1,rsxy1,dr1,dx1,RX)
      defineDifferenceNewOrder4Components1(u2,rsxy2,dr2,dx2,RX)

!............... end statement functions

      ierr=0

      side1                =ipar(0)
      axis1                =ipar(1)
      grid1                =ipar(2)
      n1a                  =ipar(3)
      n1b                  =ipar(4)
      n2a                  =ipar(5)
      n2b                  =ipar(6)
      n3a                  =ipar(7)
      n3b                  =ipar(8)

      side2                =ipar(9)
      axis2                =ipar(10)
      grid2                =ipar(11)
      m1a                  =ipar(12)
      m1b                  =ipar(13)
      m2a                  =ipar(14)
      m2b                  =ipar(15)
      m3a                  =ipar(16)
      m3b                  =ipar(17)

      gridType             =ipar(18)
      orderOfAccuracy      =ipar(19)
      orderOfExtrapolation =ipar(20)
      useForcing           =ipar(21)
      ex                   =ipar(22)
      ey                   =ipar(23)
      ez                   =ipar(24)
      hx                   =ipar(25)
      hy                   =ipar(26)
      hz                   =ipar(27)
      solveForE            =ipar(28)
      solveForH            =ipar(29)
      useWhereMask         =ipar(30)
      debug                =ipar(31)
      nit                  =ipar(32)
      option               =ipar(33)
      initialized          =ipar(34)
      myid                 =ipar(35)
      parallel             =ipar(36)
      forcingOption        =ipar(37) ! *new* 090509
     
      dx1(0)                =rpar(0)
      dx1(1)                =rpar(1)
      dx1(2)                =rpar(2)
      dr1(0)                =rpar(3)
      dr1(1)                =rpar(4)
      dr1(2)                =rpar(5)

      dx2(0)                =rpar(6)
      dx2(1)                =rpar(7)
      dx2(2)                =rpar(8)
      dr2(0)                =rpar(9)
      dr2(1)                =rpar(10)
      dr2(2)                =rpar(11)

      t                    =rpar(12)
      ep                   =rpar(13) ! pointer for exact solution
      dt                   =rpar(14)
      eps1                 =rpar(15)
      mu1                  =rpar(16)
      c1                   =rpar(17)
      eps2                 =rpar(18)
      mu2                  =rpar(19)
      c2                   =rpar(20)
     
      if( abs(c1*c1-1./(mu1*eps1)).gt. 1.e-10 )then
        write(*,'(" interfaceMaxwell:ERROR: c1,eps1,mu1=",3e10.2," not consistent")') c1,eps1,mu1
           ! '
        stop 11
      end if
      if( abs(c2*c2-1./(mu2*eps2)).gt. 1.e-10 )then
        write(*,'(" interfaceMaxwell:ERROR: c2,eps2,mu2=",3e10.2," not consistent")') c2,eps2,mu2
           ! '
        stop 11
      end if

      if( t.le.dt .and. myid.eq.0 )then
        write(*,'(" interfaceMaxwell: eps1,eps2=",2f10.5," c1,c2=",2f10.5)') eps1,eps2,c1,c2
           ! '
      end if

      if( nit.lt.0 .or. nit.gt.100 )then
        write(*,'(" interfaceBC: ERROR: nit=",i9)') nit
        nit=max(1,min(100,nit))
      end if

      if( debug.gt.0 )then
        write(*,'(" interfaceMaxwell: **START** grid1=",i4," side1,axis1=",2i2)') grid1,side1,axis1
           ! '
        write(*,'(" interfaceMaxwell: **START** grid2=",i4," side2,axis2=",2i2)') grid2,side2,axis2
           ! '
        write(*,'("n1a,n1b,...=",6i5)') n1a,n1b,n2a,n2b,n3a,n3b
        write(*,'("m1a,m1b,...=",6i5)') m1a,m1b,m2a,m2b,m3a,m3b

        if( debug.gt.4 )then
         write(*,*) 'u1=',((((u1(i1,i2,i3,m),m=0,2),i1=n1a,n1b),i2=n2a,n2b),i3=n3a,n3b)
         write(*,*) 'u2=',((((u2(i1,i2,i3,m),m=0,2),i1=m1a,m1b),i2=m2a,m2b),i3=m3a,m3b)
        end if
      end if
     
      ! *** do this for now --- assume grids have equal spacing
      dx(0)=dx1(0)
      dx(1)=dx1(1)
      dx(2)=dx1(2)

      dr(0)=dr1(0)
      dr(1)=dr1(1)
      dr(2)=dr1(2)

      epsx=1.e-20  ! fix this 

      ! ipar1, rpar1 and ipar2, rpar2 are for the symmetry BC
      bcOption=1 ! fixup corners 
      ipar1(0)  =side1
      ipar1(1)  =axis1                
      ipar1(2)  =n1a                  
      ipar1(3)  =n1b                  
      ipar1(4)  =n2a                  
      ipar1(5)  =n2b                  
      ipar1(6)  =n3a                  
      ipar1(7)  =n3b                  
      ipar1(8)  =gridType             
      ipar1(9)  =orderOfAccuracy      
      ipar1(10) =orderOfExtrapolation 
      ipar1(11) =useForcing           
      ipar1(12) =ex                   
      ipar1(13) =ey                   
      ipar1(14) =ez                   
      ipar1(15) =hx                   
      ipar1(16) =hy                   
      ipar1(17) =hz                   
      ipar1(18) =useWhereMask         
      ipar1(19) =grid1                
      ipar1(20) =debug                
      ipar1(21) =forcingOption        
      ipar1(26) =bcOption             

      rpar1(0) =dx1(0)
      rpar1(1) =dx1(1)
      rpar1(2) =dx1(2)
      rpar1(3) =dr1(0)
      rpar1(4) =dr1(1)
      rpar1(5) =dr1(2)
      rpar1(6) =t                    
      rpar1(7) =ep                   
      rpar1(8) =dt
      rpar1(9) =c1
      rpar1(10)=eps1        
      rpar1(11)=mu1         

      ipar2(0)  =side2                
      ipar2(1)  =axis2                
      ipar2(2)  =m1a                  
      ipar2(3)  =m1b                  
      ipar2(4)  =m2a                  
      ipar2(5)  =m2b                  
      ipar2(6)  =m3a                  
      ipar2(7)  =m3b                  
      ipar2(8)  =gridType             
      ipar2(9)  =orderOfAccuracy      
      ipar2(10) =orderOfExtrapolation 
      ipar2(11) =useForcing           
      ipar2(12) =ex                   
      ipar2(13) =ey                   
      ipar2(14) =ez                   
      ipar2(15) =hx                   
      ipar2(16) =hy                   
      ipar2(17) =hz                   
      ipar2(18) =useWhereMask         
      ipar2(19) =grid2                
      ipar2(20) =debug                
      ipar2(21) =forcingOption        
      ipar2(26) =bcOption             

      rpar2(0) =dx2(0)
      rpar2(1) =dx2(1)
      rpar2(2) =dx2(2)
      rpar2(3) =dr2(0)
      rpar2(4) =dr2(1)
      rpar2(5) =dr2(2)
      rpar2(6) =t                    
      rpar2(7) =ep                   
      rpar2(8) =dt
      rpar2(9) =c2
      rpar2(10)=eps2        
      rpar2(11)=mu2         




      numGhost=orderOfAccuracy/2
      giveDiv=0   ! set to 1 to give div(u) on both sides, rather than setting the jump in div(u)

      ! bounds for loops that include ghost points in the tangential directions:
      nn1a=n1a
      nn1b=n1b
      nn2a=n2a
      nn2b=n2b
      nn3a=n3a
      nn3b=n3b

      mm1a=m1a
      mm1b=m1b
      mm2a=m2a
      mm2b=m2b
      mm3a=m3a
      mm3b=m3b

      if( nd.eq.2 )then

       i3=n3a
       j3=m3a

       axis1p1=mod(axis1+1,nd)
       axis2p1=mod(axis2+1,nd)

       is1=0
       is2=0
       is3=0

       if( axis1.eq.0 ) then
         is1=1-2*side1
         if( boundaryCondition1(0,axis1p1).le.0 )then ! *wdh* 090509 also extrap outside adjacent interp .lt. -> .le.
           ! include ghost lines in tangential directions (for extrapolating)
           nn2a=nn2a-numGhost
           nn2b=nn2b+numGhost
         end if
         an1Cartesian=1. ! normal for a cartesian grid
         an2Cartesian=0.
       else
         is2=1-2*side1
         if( boundaryCondition1(0,axis1p1).le.0 )then
           ! include ghost lines in tangential directions (for extrapolating)
           nn1a=nn1a-numGhost
           nn1b=nn1b+numGhost
         end if
         an1Cartesian=0.
         an2Cartesian=1.
       end if


       js1=0
       js2=0
       js3=0
       if( axis2.eq.0 ) then
         js1=1-2*side2
         if( boundaryCondition1(0,axis2p1).le.0 )then
           mm2a=mm2a-numGhost
           mm2b=mm2b+numGhost
         end if
       else
         js2=1-2*side2
         if( boundaryCondition1(0,axis2p1).le.0 )then
           mm1a=mm1a-numGhost
           mm1b=mm1b+numGhost
         end if
       end if

       is=1-2*side1
       js=1-2*side2

       if( axis1.eq.0 )then
         rx1=1.
         ry1=0.
       else
         rx1=0.
         ry1=1.
       endif
       if( axis2.eq.0 )then
         rx2=1.
         ry2=0.
       else
         rx2=0.
         ry2=1.
       endif

       if( orderOfAccuracy.eq.2 .and. gridType.eq.rectangular )then
  
        if( .false. )then
         ! just copy values from ghost points for now
         beginLoops2d()
           u1(i1-is1,i2-is2,i3,ex)=u2(j1+js1,j2+js2,j3,ex)
           u1(i1-is1,i2-is2,i3,ey)=u2(j1+js1,j2+js2,j3,ey)
           u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 

           u2(j1-js1,j2-js2,j3,ex)=u1(i1+is1,i2+is2,i3,ex)
           u2(j1-js1,j2-js2,j3,ey)=u1(i1+is1,i2+is2,i3,ey)
           u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)
         endLoops2d()
       else

         ! ---- first satisfy the jump conditions on the boundary --------
         !    [ eps n.u ] = 0
         !    [ tau.u ] = 0
         boundaryJumpConditions(2,rectangular)

         ! initialization step: assign first ghost line by extrapolation
         ! NOTE: assign ghost points outside the ends
         beginGhostLoops2d()
            u1(i1-is1,i2-is2,i3,ex)=extrap3(u1,i1,i2,i3,ex,is1,is2,is3)
            u1(i1-is1,i2-is2,i3,ey)=extrap3(u1,i1,i2,i3,ey,is1,is2,is3)
            u1(i1-is1,i2-is2,i3,hz)=extrap3(u1,i1,i2,i3,hz,is1,is2,is3)
!
            u2(j1-js1,j2-js2,j3,ex)=extrap3(u2,j1,j2,j3,ex,js1,js2,js3)
            u2(j1-js1,j2-js2,j3,ey)=extrap3(u2,j1,j2,j3,ey,js1,js2,js3)
            u2(j1-js1,j2-js2,j3,hz)=extrap3(u2,j1,j2,j3,hz,js1,js2,js3)

         endLoops2d()

         ! here are the real jump conditions
         !   [ u.x + v.y ] = 0
         !   [ u.xx + u.yy ] = 0
         !   [ v.x - u.y ] =0 
         !   [ (v.xx+v.yy)/eps ] = 0
         beginLoops2d()
           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
           f(0)=(u1x22r(i1,i2,i3,ex)+u1y22r(i1,i2,i3,ey)) - \
                (u2x22r(j1,j2,j3,ex)+u2y22r(j1,j2,j3,ey))
           f(1)=(u1xx22r(i1,i2,i3,ex)+u1yy22r(i1,i2,i3,ex)) - \
                (u2xx22r(j1,j2,j3,ex)+u2yy22r(j1,j2,j3,ex))

           f(2)=(u1x22r(i1,i2,i3,ey)-u1y22r(i1,i2,i3,ex)) - \
                (u2x22r(j1,j2,j3,ey)-u2y22r(j1,j2,j3,ex))
           
           f(3)=(u1xx22r(i1,i2,i3,ey)+u1yy22r(i1,i2,i3,ey))/eps1 - \
                (u2xx22r(j1,j2,j3,ey)+u2yy22r(j1,j2,j3,ey))/eps2
    
      ! write(*,'(" --> i1,i2=",2i4," f(start)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)

           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]
           if( axis1.eq.0 )then
             a4(0,0) = -is1/(2.*dx1(0))    ! coeff of u1(-1) from [u.x+v.y] 
             a4(0,1) = 0.                  ! coeff of v1(-1) from [u.x+v.y] 
           
             a4(2,0) = 0.
             a4(2,1) = -is1/(2.*dx1(0))    ! coeff of v1(-1) from [v.x - u.y] 
           else 
             a4(0,0) = 0.                 
             a4(0,1) = -is2/(2.*dx1(1))    ! coeff of v1(-1) from [u.x+v.y] 

             a4(2,0) =  is2/(2.*dx1(1))    ! coeff of u1(-1) from [v.x - u.y] 
             a4(2,1) = 0.
           end if
           if( axis2.eq.0 )then
             a4(0,2) = js1/(2.*dx2(0))    ! coeff of u2(-1) from [u.x+v.y] 
             a4(0,3) = 0. 
           
             a4(2,2) = 0.
             a4(2,3) = js1/(2.*dx2(0))    ! coeff of v2(-1) from [v.x - u.y]
           else
             a4(0,2) = 0. 
             a4(0,3) = js2/(2.*dx2(1))    ! coeff of v2(-1) from [u.x+v.y] 

             a4(2,2) =-js2/(2.*dx2(1))    ! coeff of u2(-1) from [v.x - u.y] 
             a4(2,3) = 0.
           end if

           a4(1,0) = 1./(dx1(axis1)**2)   ! coeff of u1(-1) from [u.xx + u.yy]
           a4(1,1) = 0. 
           a4(1,2) =-1./(dx2(axis2)**2)   ! coeff of u2(-1) from [u.xx + u.yy]
           a4(1,3) = 0. 
             
           a4(3,0) = 0.                      
           a4(3,1) = 1./(dx1(axis1)**2)/eps1 ! coeff of v1(-1) from [(v.xx+v.yy)/eps]
           a4(3,2) = 0. 
           a4(3,3) =-1./(dx2(axis2)**2)/eps2 ! coeff of v2(-1) from [(v.xx+v.yy)/eps]
             

           q(0) = u1(i1-is1,i2-is2,i3,ex)
           q(1) = u1(i1-is1,i2-is2,i3,ey)
           q(2) = u2(j1-js1,j2-js2,j3,ex)
           q(3) = u2(j1-js1,j2-js2,j3,ey)

           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,3
             f(n) = (a4(n,0)*q(0)+a4(n,1)*q(1)+a4(n,2)*q(2)+a4(n,3)*q(3)) - f(n)
           end do
      ! write(*,'(" --> i1,i2=",2i4," f(subtract)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)
           ! solve A Q = F
           ! factor the matrix
           numberOfEquations=4
           call dgeco( a4(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0))
           ! solve
      ! write(*,'(" --> i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
           job=0
           call dgesl( a4(0,0), numberOfEquations, numberOfEquations, ipvt(0), f(0), job)
      ! write(*,'(" --> i1,i2=",2i4," f(solve)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)

           u1(i1-is1,i2-is2,i3,ex)=f(0)
           u1(i1-is1,i2-is2,i3,ey)=f(1)
           u2(j1-js1,j2-js2,j3,ex)=f(2)
           u2(j1-js1,j2-js2,j3,ey)=f(3)

!       if( debug.gt.0 )then ! re-evaluate
!            f(0)=(u1x22r(i1,i2,i3,ex)+u1y22r(i1,i2,i3,ey)) - \
!                 (u2x22r(j1,j2,j3,ex)+u2y22r(j1,j2,j3,ey))
!            f(1)=(u1xx22r(i1,i2,i3,ex)+u1yy22r(i1,i2,i3,ex)) - \
!                 (u2xx22r(j1,j2,j3,ex)+u2yy22r(j1,j2,j3,ex))
! 
!            f(2)=(u1x22r(i1,i2,i3,ey)-u1y22r(i1,i2,i3,ex)) - \
!                 (u2x22r(j1,j2,j3,ey)-u2y22r(j1,j2,j3,ex))
!            
!            f(3)=(u1xx22r(i1,i2,i3,ey)+u1yy22r(i1,i2,i3,ey))/eps1 - \
!                 (u2xx22r(j1,j2,j3,ey)+u2yy22r(j1,j2,j3,ey))/eps2
!     
!         write(*,'(" --> i1,i2=",2i4," f(re-eval)=",4e10.2)') i1,i2,f(0),f(1),f(2),f(3)
!       end if

           ! do this for now
           u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 
           u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)


         endLoops2d()

         ! periodic update
         periodicUpdate2d(u1,boundaryCondition1,gridIndexRange1,side1,axis1)
         periodicUpdate2d(u2,boundaryCondition2,gridIndexRange2,side2,axis2)

       end if

       else if( orderOfAccuracy.eq.2 .and. gridType.eq.curvilinear )then
         ! ***** curvilinear case *****

         ! ---- first satisfy the jump conditions on the boundary --------
         !    [ eps n.u ] = 0
         !    [ tau.u ] = 0
         boundaryJumpConditions(2,curvilinear)

         ! initialization step: assign first ghost line by extrapolation
         ! NOTE: assign ghost points outside the ends
         beginGhostLoops2d()
            u1(i1-is1,i2-is2,i3,ex)=extrap3(u1,i1,i2,i3,ex,is1,is2,is3)
            u1(i1-is1,i2-is2,i3,ey)=extrap3(u1,i1,i2,i3,ey,is1,is2,is3)
            u1(i1-is1,i2-is2,i3,hz)=extrap3(u1,i1,i2,i3,hz,is1,is2,is3)
!
            u2(j1-js1,j2-js2,j3,ex)=extrap3(u2,j1,j2,j3,ex,js1,js2,js3)
            u2(j1-js1,j2-js2,j3,ey)=extrap3(u2,j1,j2,j3,ey,js1,js2,js3)
            u2(j1-js1,j2-js2,j3,hz)=extrap3(u2,j1,j2,j3,hz,js1,js2,js3)

         endLoops2d()

         ! here are the real jump conditions for the ghost points
         !   [ u.x + v.y ] = 0 = [ rx*ur + ry*vr + sx*us + sy*vs ] 
         !   [ n.(uv.xx + uv.yy) ] = 0
         !   [ v.x - u.y ] =0 
         !   [ tau.(uv.xx+uv.yy)/eps ] = 0


         beginLoops2d()

           ! here is the normal (assumed to be the same on both sides)
           an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
           an2=rsxy1(i1,i2,i3,axis1,1)
           aNorm=max(epsx,sqrt(an1**2+an2**2))
           an1=an1/aNorm
           an2=an2/aNorm
           tau1=-an2
           tau2= an1

           ulap1=u1Laplacian22(i1,i2,i3,ex)
           vlap1=u1Laplacian22(i1,i2,i3,ey)
           ulap2=u2Laplacian22(j1,j2,j3,ex)
           vlap2=u2Laplacian22(j1,j2,j3,ey)

           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
           if( giveDiv.eq.0 )then
             f(0)=(u1x22(i1,i2,i3,ex)+u1y22(i1,i2,i3,ey)) - \
                  (u2x22(j1,j2,j3,ex)+u2y22(j1,j2,j3,ey))
             f(1)=( an1*ulap1 +an2*vlap1 )- \
                  ( an1*ulap2 +an2*vlap2 )
           else
             ! *** give div(u)=0 on both sides ***
             f(0)=u1x22(i1,i2,i3,ex)+u1y22(i1,i2,i3,ey)
             f(1)=u2x22(j1,j2,j3,ex)+u2y22(j1,j2,j3,ey)
           end if

           f(2)=(u1x22(i1,i2,i3,ey)-u1y22(i1,i2,i3,ex)) - \
                (u2x22(j1,j2,j3,ey)-u2y22(j1,j2,j3,ex))
           
           f(3)=( tau1*ulap1 +tau2*vlap1 )/eps1 - \
                ( tau1*ulap2 +tau2*vlap2 )/eps2
    
      ! write(*,'(" --> order2-curv: i1,i2=",2i4," f(start)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)

           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]
           if( giveDiv.eq.0 )then
             a4(0,0) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))    ! coeff of u1(-1) from [u.x+v.y] 
             a4(0,1) = -is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))    ! coeff of v1(-1) from [u.x+v.y] 
             a4(0,2) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))    ! coeff of u2(-1) from [u.x+v.y] 
             a4(0,3) =  js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))    ! coeff of v2(-1) from [u.x+v.y] 
           else
             a4(0,0) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))    ! coeff of u1(-1) from u.x+v.y=0
             a4(0,1) = -is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))    ! coeff of v1(-1) from u.x+v.y=0
             a4(0,2) =  0.
             a4(0,3) =  0.

             a4(1,0) = 0.
             a4(1,1) = 0.
             a4(1,2) = -js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))    ! coeff of u2(-1) from u.x+v.y=0
             a4(1,3) = -js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))    ! coeff of v2(-1) from u.x+v.y=0
           end if

           a4(2,0) =  is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))   ! coeff of u1(-1) from [v.x - u.y] 
           a4(2,1) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))   ! coeff of v1(-1) from [v.x - u.y] 

           a4(2,2) = -js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))   ! coeff of u2(-1) from [v.x - u.y] 
           a4(2,3) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))   ! coeff of v2(-1) from [v.x - u.y] 


           ! coeff of u(-1) from lap = u.xx + u.yy
           clap1=(rsxy1(i1,i2,i3,axis1,0)**2+rsxy1(i1,i2,i3,axis1,1)**2)/(dr1(axis1)**2) \
                     -is*(rsxy1x22(i1,i2,i3,axis1,0)+rsxy1y22(i1,i2,i3,axis1,1))/(2.*dr1(axis1))
           clap2=(rsxy2(j1,j2,j3,axis2,0)**2+rsxy2(j1,j2,j3,axis2,1)**2)/(dr2(axis2)**2) \
                       -js*(rsxy2x22(j1,j2,j3,axis2,0)+rsxy2y22(j1,j2,j3,axis2,1))/(2.*dr2(axis2)) 

           !   [ n.(uv.xx + u.yy) ] = 0
           if( giveDiv.eq.0 )then
             a4(1,0) = an1*clap1
             a4(1,1) = an2*clap1
             a4(1,2) =-an1*clap2
             a4(1,3) =-an2*clap2
           end if 
           !   [ tau.(uv.xx+uv.yy)/eps ] = 0
           a4(3,0) = tau1*clap1/eps1
           a4(3,1) = tau2*clap1/eps1
           a4(3,2) =-tau1*clap2/eps2
           a4(3,3) =-tau2*clap2/eps2
             

           q(0) = u1(i1-is1,i2-is2,i3,ex)
           q(1) = u1(i1-is1,i2-is2,i3,ey)
           q(2) = u2(j1-js1,j2-js2,j3,ex)
           q(3) = u2(j1-js1,j2-js2,j3,ey)

           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,3
             f(n) = (a4(n,0)*q(0)+a4(n,1)*q(1)+a4(n,2)*q(2)+a4(n,3)*q(3)) - f(n)
           end do
      ! write(*,'(" --> order2-curv: i1,i2=",2i4," f(subtract)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)
           ! solve A Q = F
           ! factor the matrix
           numberOfEquations=4
           call dgeco( a4(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0))
           ! solve
      !   write(*,'(" --> order2-curv: i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
           job=0
           call dgesl( a4(0,0), numberOfEquations, numberOfEquations, ipvt(0), f(0), job)
      ! write(*,'(" --> order2-curv: i1,i2=",2i4," f(solve)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)

           u1(i1-is1,i2-is2,i3,ex)=f(0)
           u1(i1-is1,i2-is2,i3,ey)=f(1)
           u2(j1-js1,j2-js2,j3,ex)=f(2)
           u2(j1-js1,j2-js2,j3,ey)=f(3)

!            if( debug.gt.0 )then ! re-evaluate
!              ulap1=u1Laplacian22(i1,i2,i3,ex)
!              vlap1=u1Laplacian22(i1,i2,i3,ey)
!              ulap2=u2Laplacian22(j1,j2,j3,ex)
!              vlap2=u2Laplacian22(j1,j2,j3,ey)
!   
!              if( giveDiv.eq.0 )then
!                f(0)=(u1x22(i1,i2,i3,ex)+u1y22(i1,i2,i3,ey)) - \
!                     (u2x22(j1,j2,j3,ex)+u2y22(j1,j2,j3,ey))
!                f(1)=( an1*ulap1 +an2*vlap1 )- \
!                     ( an1*ulap2 +an2*vlap2 )
!              else
!                ! *** give div(u)=0 on both sides ***
!                f(0)=u1x22(i1,i2,i3,ex)+u1y22(i1,i2,i3,ey)
!                f(1)=u2x22(j1,j2,j3,ex)+u2y22(j1,j2,j3,ey)
!              end if
!              f(2)=(u1x22(i1,i2,i3,ey)-u1y22(i1,i2,i3,ex)) - \
!                   (u2x22(j1,j2,j3,ey)-u2y22(j1,j2,j3,ex))
!              f(3)=( tau1*ulap1 +tau2*vlap1 )/eps1 - \
!                   ( tau1*ulap2 +tau2*vlap2 )/eps2
!              write(*,'(" --> order2-curv: i1,i2=",2i4," f(re-eval)=",4e10.2)') i1,i2,f(0),f(1),f(2),f(3)
!                ! '
!            end if

           ! solve for Hz
           !  [ w.n/eps] = 0
           !  [ Lap(w)/eps] = 0

           wlap1=u1Laplacian22(i1,i2,i3,hz)
           wlap2=u2Laplacian22(j1,j2,j3,hz)

           f(0) = (an1*u1x22(i1,i2,i3,hz)+an2*u1y22(i1,i2,i3,hz))/eps1 -\
                  (an1*u2x22(j1,j2,j3,hz)+an2*u2y22(j1,j2,j3,hz))/eps2
           f(1) = wlap1/eps1 - wlap2/eps2

           a2(0,0)=-is*(an1*rsxy1(i1,i2,i3,axis1,0)+an2*rsxy1(i1,i2,i3,axis1,1))/(2.*dr1(axis1)*eps1)
           a2(0,1)= js*(an1*rsxy2(j1,j2,j3,axis2,0)+an2*rsxy2(j1,j2,j3,axis2,1))/(2.*dr2(axis2)*eps2)

           a2(1,0)= clap1/eps1
           a2(1,1)=-clap2/eps2

           q(0) = u1(i1-is1,i2-is2,i3,hz)
           q(1) = u2(j1-js1,j2-js2,j3,hz)

           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,1
             f(n) = (a2(n,0)*q(0)+a2(n,1)*q(1)) - f(n)
           end do

           call dgeco( a2(0,0), 2, 2, ipvt(0),rcond,work(0))
           job=0
           call dgesl( a2(0,0), 2, 2, ipvt(0), f(0), job)

           u1(i1-is1,i2-is2,i3,hz)=f(0)
           u2(j1-js1,j2-js2,j3,hz)=f(1)

           ! u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 
           ! u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)

!            if( debug.gt.0 )then ! re-evaluate
! 
!              wlap1=u1Laplacian22(i1,i2,i3,hz)
!              wlap2=u2Laplacian22(j1,j2,j3,hz)
! 
!              f(0) = (an1*u1x22(i1,i2,i3,hz)+an2*u1y22(i1,i2,i3,hz))/eps1 -\
!                     (an1*u2x22(j1,j2,j3,hz)+an2*u2y22(j1,j2,j3,hz))/eps2
!              f(1) = wlap1/eps1 - wlap2/eps2
! 
!              write(*,'(" --> order2-curv: i1,i2=",2i4," hz-f(re-eval)=",4e10.2)') i1,i2,f(0),f(1)
!                ! '
!            end if

         endLoops2d()

         ! now make sure that div(u)=0 etc.
         if( .false. )then
         beginLoops2d() ! =============== start loops =======================

           ! 0  [ u.x + v.y ] = 0
           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
           divu=u1x22(i1,i2,i3,ex)+u1y22(i1,i2,i3,ey)
           a0=-is*rsxy1(i1,i2,i3,axis1,0)*dr112(axis1)
           a1=-is*rsxy1(i1,i2,i3,axis1,1)*dr112(axis1)
           aNormSq=a0**2+a1**2
           ! now project:  a.uNew = a.uOld - div  ->  (div-a.uOld)+a.uNew = div(uNew) = 0
           u1(i1-is1,i2-is2,i3,ex)=u1(i1-is1,i2-is2,i3,ex)-divu*a0/aNormSq
           u1(i1-is1,i2-is2,i3,ey)=u1(i1-is1,i2-is2,i3,ey)-divu*a1/aNormSq

           divu=u2x22(j1,j2,j3,ex)+u2y22(j1,j2,j3,ey)
           a0=-js*rsxy2(j1,j2,j3,axis2,0)*dr212(axis2) 
           a1=-js*rsxy2(j1,j2,j3,axis2,1)*dr212(axis2) 
           aNormSq=a0**2+a1**2

           u2(j1-js1,j2-js2,j3,ex)=u2(j1-js1,j2-js2,j3,ex)-divu*a0/aNormSq
           u2(j1-js1,j2-js2,j3,ey)=u2(j1-js1,j2-js2,j3,ey)-divu*a1/aNormSq

           if( debug.gt.0 )then
             write(*,'(" --> 2cth: eval div1,div2=",2e10.2)') u1x22(i1,i2,i3,ex)+u1y22(i1,i2,i3,ey),u2x22(j1,j2,j3,ex)+u2y22(j1,j2,j3,ey)
           end if
         endLoops2d()
         end if

         ! periodic update
         periodicUpdate2d(u1,boundaryCondition1,gridIndexRange1,side1,axis1)
         periodicUpdate2d(u2,boundaryCondition2,gridIndexRange2,side2,axis2)

       else if( .false. .and. orderOfAccuracy.eq.4 )then

         ! for testing -- just assign from the other ghost points

         beginLoops2d()
           u1(i1-is1,i2-is2,i3,ex)=u2(j1+js1,j2+js2,j3,ex)
           u1(i1-is1,i2-is2,i3,ey)=u2(j1+js1,j2+js2,j3,ey)
           u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 

           u2(j1-js1,j2-js2,j3,ex)=u1(i1+is1,i2+is2,i3,ex)
           u2(j1-js1,j2-js2,j3,ey)=u1(i1+is1,i2+is2,i3,ey)
           u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)

           u1(i1-2*is1,i2-2*is2,i3,ex)=u2(j1+2*js1,j2+2*js2,j3,ex)
           u1(i1-2*is1,i2-2*is2,i3,ey)=u2(j1+2*js1,j2+2*js2,j3,ey)
           u1(i1-2*is1,i2-2*is2,i3,hz)=u2(j1+2*js1,j2+2*js2,j3,hz) 

           u2(j1-2*js1,j2-2*js2,j3,ex)=u1(i1+2*is1,i2+2*is2,i3,ex)
           u2(j1-2*js1,j2-2*js2,j3,ey)=u1(i1+2*is1,i2+2*is2,i3,ey)
           u2(j1-2*js1,j2-2*js2,j3,hz)=u1(i1+2*is1,i2+2*is2,i3,hz)

         endLoops2d()

       else if( orderOfAccuracy.eq.4 .and. gridType.eq.rectangular )then
  
         ! --------------- 4th Order Rectangular ---------------
         ! ---- first satisfy the jump conditions on the boundary --------
         !    [ eps n.u ] = 0
         !    [ tau.u ] = 0
         boundaryJumpConditions(2,rectangular)

         ! here are the real jump conditions for the ghost points
         ! 0  [ u.x + v.y ] = 0
         ! 1  [ u.xx + u.yy ] = 0
         ! 2  [ v.x - u.y ] =0 
         ! 3  [ (v.xx+v.yy)/eps ] = 0
         ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0  OR [ (u.xx).x + (v.xx).y ] = 0 OR  [ (u.yy).x + (v.yy).y ] = 0 
         ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0
         ! 6  [ Delta^2 u/eps ] = 0
         ! 7  [ Delta^2 v/eps^2 ] = 0 


         ! initialization step: assign first ghost line by extrapolation
         ! NOTE: assign ghost points outside the ends
         beginGhostLoops2d()
           ! *wdh* 090612 change extrap4 to extrap5 
           u1(i1-is1,i2-is2,i3,ex)=extrap5(u1,i1,i2,i3,ex,is1,is2,is3)
           u1(i1-is1,i2-is2,i3,ey)=extrap5(u1,i1,i2,i3,ey,is1,is2,is3)
           u1(i1-is1,i2-is2,i3,hz)=extrap5(u1,i1,i2,i3,hz,is1,is2,is3)

           u2(j1-js1,j2-js2,j3,ex)=extrap5(u2,j1,j2,j3,ex,js1,js2,js3)
           u2(j1-js1,j2-js2,j3,ey)=extrap5(u2,j1,j2,j3,ey,js1,js2,js3)
           u2(j1-js1,j2-js2,j3,hz)=extrap5(u2,j1,j2,j3,hz,js1,js2,js3)

           ! --- also extrap 2nd line for now
           ! u1(i1-2*is1,i2-2*is2,i3,ex)=extrap4(u1,i1-is1,i2-is2,i3,ex,is1,is2,is3)
           ! u1(i1-2*is1,i2-2*is2,i3,ey)=extrap4(u1,i1-is1,i2-is2,i3,ey,is1,is2,is3)
           ! u1(i1-2*is1,i2-2*is2,i3,hz)=extrap4(u1,i1-is1,i2-is2,i3,hz,is1,is2,is3)

           ! u2(j1-2*js1,j2-2*js2,j3,ex)=extrap4(u2,j1-js1,j2-js2,j3,ex,js1,js2,js3)
           ! u2(j1-2*js1,j2-2*js2,j3,ey)=extrap4(u2,j1-js1,j2-js2,j3,ey,js1,js2,js3)
           ! u2(j1-2*js1,j2-2*js2,j3,hz)=extrap4(u2,j1-js1,j2-js2,j3,hz,js1,js2,js3)
         endLoops2d()

         beginLoops2d() ! =============== start loops =======================

           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
           f(0)=(u1x42r(i1,i2,i3,ex)+u1y42r(i1,i2,i3,ey)) - \
                (u2x42r(j1,j2,j3,ex)+u2y42r(j1,j2,j3,ey))

           f(1)=(u1xx42r(i1,i2,i3,ex)+u1yy42r(i1,i2,i3,ex)) - \
                (u2xx42r(j1,j2,j3,ex)+u2yy42r(j1,j2,j3,ex))

           f(2)=(u1x42r(i1,i2,i3,ey)-u1y42r(i1,i2,i3,ex)) - \
                (u2x42r(j1,j2,j3,ey)-u2y42r(j1,j2,j3,ex))
           
           f(3)=(u1xx42r(i1,i2,i3,ey)+u1yy42r(i1,i2,i3,ey))/eps1 - \
                (u2xx42r(j1,j2,j3,ey)+u2yy42r(j1,j2,j3,ey))/eps2
    
           ! These next we can do to 2nd order -- these need a value on the first ghost line --
           f(4)=(u1xxx22r(i1,i2,i3,ex)+u1xyy22r(i1,i2,i3,ex)+u1xxy22r(i1,i2,i3,ey)+u1yyy22r(i1,i2,i3,ey)) - \
                (u2xxx22r(j1,j2,j3,ex)+u2xyy22r(j1,j2,j3,ex)+u2xxy22r(j1,j2,j3,ey)+u2yyy22r(j1,j2,j3,ey))

           f(5)=((u1xxx22r(i1,i2,i3,ey)+u1xyy22r(i1,i2,i3,ey))-(u1xxy22r(i1,i2,i3,ex)+u1yyy22r(i1,i2,i3,ex)))/eps1 - \
                ((u2xxx22r(j1,j2,j3,ey)+u2xyy22r(j1,j2,j3,ey))-(u2xxy22r(j1,j2,j3,ex)+u2yyy22r(j1,j2,j3,ex)))/eps2

           f(6)=(u1LapSq22r(i1,i2,i3,ex))/eps1 - \
                (u2LapSq22r(j1,j2,j3,ex))/eps2

           f(7)=(u1LapSq22r(i1,i2,i3,ey))/eps1**2 - \
                (u2LapSq22r(j1,j2,j3,ey))/eps2**2
           
       write(*,'(" --> 4th: j1,j2=",2i4," u1xx,u1yy,u2xx,u2yy=",4e10.2)') j1,j2,u1xx42r(i1,i2,i3,ex),\
           u1yy42r(i1,i2,i3,ex),u2xx42r(j1,j2,j3,ex),u2yy42r(j1,j2,j3,ex)
       write(*,'(" --> 4th: i1,i2=",2i4," f(start)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)

           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]
!      u1x43r(i1,i2,i3,kd)=(8.*(u1(i1+1,i2,i3,kd)-u1(i1-1,i2,i3,kd))-(
!     & u1(i1+2,i2,i3,kd)-u1(i1-2,i2,i3,kd)))*dx141(0)


           ! 0  [ u.x + v.y ] = 0
           a8(0,0) = -is*8.*rx1*dx141(axis1)     ! coeff of u1(-1) from [u.x+v.y] 
           a8(0,1) = -is*8.*ry1*dx141(axis1)     ! coeff of v1(-1) from [u.x+v.y] 
           a8(0,4) =  is*rx1*dx141(axis1)        ! u1(-2)
           a8(0,5) =  is*ry1*dx141(axis1)        ! v1(-2) 

           a8(0,2) =  js*8.*rx2*dx241(axis2)     ! coeff of u2(-1) from [u.x+v.y] 
           a8(0,3) =  js*8.*ry2*dx241(axis2) 
           a8(0,6) = -js*   rx2*dx241(axis2) 
           a8(0,7) = -js*   ry2*dx241(axis2) 

           ! 1  [ u.xx + u.yy ] = 0
!      u1xx43r(i1,i2,i3,kd)=( -30.*u1(i1,i2,i3,kd)+16.*(u1(i1+1,i2,i3,
!     & kd)+u1(i1-1,i2,i3,kd))-(u1(i1+2,i2,i3,kd)+u1(i1-2,i2,i3,kd)) )*
!     & dx142(0)
           
           a8(1,0) = 16.*dx142(axis1)         ! coeff of u1(-1) from [u.xx + u.yy]
           a8(1,1) = 0. 
           a8(1,4) =    -dx142(axis1)         ! coeff of u1(-2) from [u.xx + u.yy]
           a8(1,5) = 0. 

           a8(1,2) =-16.*dx242(axis2)         ! coeff of u2(-1) from [u.xx + u.yy]
           a8(1,3) = 0. 
           a8(1,6) =     dx242(axis2)         ! coeff of u2(-2) from [u.xx + u.yy]
           a8(1,7) = 0. 


           ! 2  [ v.x - u.y ] =0 
           a8(2,0) =  is*8.*ry1*dx141(axis1)
           a8(2,1) = -is*8.*rx1*dx141(axis1)    ! coeff of v1(-1) from [v.x - u.y] 
           a8(2,4) = -is*   ry1*dx141(axis1)
           a8(2,5) =  is*   rx1*dx141(axis1)

           a8(2,2) = -js*8.*ry2*dx241(axis2)
           a8(2,3) =  js*8.*rx2*dx241(axis2)
           a8(2,6) =  js*   ry2*dx241(axis2)
           a8(2,7) = -js*   rx2*dx241(axis2)

           ! 3  [ (v.xx+v.yy)/eps ] = 0
           a8(3,0) = 0.                      
           a8(3,1) = 16.*dx142(axis1)/eps1 ! coeff of v1(-1) from [(v.xx+v.yy)/eps]
           a8(3,4) = 0.                      
           a8(3,5) =    -dx142(axis1)/eps1 ! coeff of v1(-2) from [(v.xx+v.yy)/eps]

           a8(3,2) = 0. 
           a8(3,3) =-16.*dx242(axis2)/eps2 ! coeff of v2(-1) from [(v.xx+v.yy)/eps]
           a8(3,6) = 0. 
           a8(3,7) =     dx242(axis2)/eps2 ! coeff of v2(-2) from [(v.xx+v.yy)/eps]

           ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0
!     u1xxx22r(i1,i2,i3,kd)=(-2.*(u1(i1+1,i2,i3,kd)-u1(i1-1,i2,i3,kd))+
!    & (u1(i1+2,i2,i3,kd)-u1(i1-2,i2,i3,kd)) )*dx122(0)*dx112(0)
!    u1xxy22r(i1,i2,i3,kd)=( u1xx22r(i1,i2+1,i3,kd)-u1xx22r(i1,i2-1,
!     & i3,kd))/(2.*dx1(1))
!      u1yy23r(i1,i2,i3,kd)=(-2.*u1(i1,i2,i3,kd)+(u1(i1,i2+1,i3,kd)+u1(
!     & i1,i2-1,i3,kd)) )*dx122(1)
!     u1xyy22r(i1,i2,i3,kd)=( u1yy22r(i1+1,i2,i3,kd)-u1yy22r(i1-1,i2,
!     & i3,kd))/(2.*dx1(0))
          a8(4,0)= ( is*rx1*2.*dx122(axis1)*dx112(axis1)+is*rx1*2.*dx122(1)/(2.*dx1(0)))
          a8(4,1)= ( is*ry1*2.*dx122(axis1)*dx112(axis1)+is*ry1*2.*dx122(0)/(2.*dx1(1)))
          a8(4,4)= (-is*rx1   *dx122(axis1)*dx112(axis1) )  
          a8(4,5)= (-is*ry1   *dx122(axis1)*dx112(axis1))

          a8(4,2)=-( js*rx2*2.*dx222(axis2)*dx212(axis2)+js*rx2*2.*dx222(1)/(2.*dx2(0)))
          a8(4,3)=-( js*ry2*2.*dx222(axis2)*dx212(axis2)+js*ry2*2.*dx222(0)/(2.*dx2(1)))
          a8(4,6)=-(-js*rx2   *dx222(axis2)*dx212(axis2))   
          a8(4,7)=-(-js*ry2   *dx222(axis2)*dx212(axis2))

          ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0

          a8(5,0)=-( is*ry1*2.*dx122(axis1)*dx112(axis1)+is*ry1*2.*dx122(0)/(2.*dx1(1)))/eps1
          a8(5,1)= ( is*rx1*2.*dx122(axis1)*dx112(axis1)+is*rx1*2.*dx122(1)/(2.*dx1(0)))/eps1
          a8(5,4)=-(-is*ry1   *dx122(axis1)*dx112(axis1))/eps1
          a8(5,5)= (-is*rx1   *dx122(axis1)*dx112(axis1))/eps1   

          a8(5,2)= ( js*ry2*2.*dx222(axis2)*dx212(axis2)+js*ry2*2.*dx222(0)/(2.*dx2(1)))/eps2
          a8(5,3)=-( js*rx2*2.*dx222(axis2)*dx212(axis2)+js*rx2*2.*dx222(1)/(2.*dx2(0)))/eps2
          a8(5,6)= (-js*ry2   *dx222(axis2)*dx212(axis2))/eps2
          a8(5,7)=-(-js*rx2   *dx222(axis2)*dx212(axis2))/eps2   

           ! 6  [ Delta^2 u/eps ] = 0
!     u1LapSq22r(i1,i2,i3,kd)= ( 6.*u1(i1,i2,i3,kd)- 4.*(u1(i1+1,i2,i3,
!    & kd)+u1(i1-1,i2,i3,kd))+(u1(i1+2,i2,i3,kd)+u1(i1-2,i2,i3,kd)) )
!    & /(dx1(0)**4)+( 6.*u1(i1,i2,i3,kd)-4.*(u1(i1,i2+1,i3,kd)+u1(i1,
!    & i2-1,i3,kd)) +(u1(i1,i2+2,i3,kd)+u1(i1,i2-2,i3,kd)) )/(dx1(1)**
!    & 4)+( 8.*u1(i1,i2,i3,kd)-4.*(u1(i1+1,i2,i3,kd)+u1(i1-1,i2,i3,kd)
!    & +u1(i1,i2+1,i3,kd)+u1(i1,i2-1,i3,kd))+2.*(u1(i1+1,i2+1,i3,kd)+
!    & u1(i1-1,i2+1,i3,kd)+u1(i1+1,i2-1,i3,kd)+u1(i1-1,i2-1,i3,kd)) )
!    & /(dx1(0)**2*dx1(1)**2)

           a8(6,0) = -(4./(dx1(axis1)**4) +4./(dx1(0)**2*dx1(1)**2) )/eps1
           a8(6,1) = 0.
           a8(6,4) =   1./(dx1(axis1)**4)/eps1
           a8(6,5) = 0.

           a8(6,2) = (4./(dx2(axis2)**4) +4./(dx1(0)**2*dx1(1)**2) )/eps2
           a8(6,3) = 0.
           a8(6,6) =  -1./(dx2(axis2)**4)/eps2
           a8(6,7) = 0.

           ! 7  [ Delta^2 v/eps^2 ] = 0 
           a8(7,0) = 0.
           a8(7,1) = -(4./(dx1(axis1)**4) +4./(dx2(0)**2*dx2(1)**2) )/eps1**2
           a8(7,4) = 0.
           a8(7,5) =   1./(dx1(axis1)**4)/eps1**2

           a8(7,2) = 0.
           a8(7,3) =  (4./(dx2(axis2)**4) +4./(dx2(0)**2*dx2(1)**2) )/eps2**2
           a8(7,6) = 0.
           a8(7,7) =  -1./(dx2(axis2)**4)/eps2**2

           q(0) = u1(i1-is1,i2-is2,i3,ex)
           q(1) = u1(i1-is1,i2-is2,i3,ey)
           q(2) = u2(j1-js1,j2-js2,j3,ex)
           q(3) = u2(j1-js1,j2-js2,j3,ey)

           q(4) = u1(i1-2*is1,i2-2*is2,i3,ex)
           q(5) = u1(i1-2*is1,i2-2*is2,i3,ey)
           q(6) = u2(j1-2*js1,j2-2*js2,j3,ex)
           q(7) = u2(j1-2*js1,j2-2*js2,j3,ey)

       write(*,'(" --> 4th: i1,i2=",2i4," q=",8e10.2)') i1,i2,q(0),q(1),q(2),q(3),q(4),q(5),q(6),q(7)

           ! subtract off the contributions from the initial (wrong) values at the ghost points:
           do n=0,7
             f(n) = (a8(n,0)*q(0)+a8(n,1)*q(1)+a8(n,2)*q(2)+a8(n,3)*q(3)+\
                     a8(n,4)*q(4)+a8(n,5)*q(5)+a8(n,6)*q(6)+a8(n,7)*q(7)) - f(n)
           end do

           ! solve A Q = F
           ! factor the matrix
           numberOfEquations=8
           call dgeco( a8(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0))
           ! solve
       write(*,'(" --> 4th: i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
           job=0
           call dgesl( a8(0,0), numberOfEquations, numberOfEquations, ipvt(0), f(0), job)

       write(*,'(" --> 4th: i1,i2=",2i4," f(solve)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)

           if( .true. )then
           u1(i1-is1,i2-is2,i3,ex)=f(0)
           u1(i1-is1,i2-is2,i3,ey)=f(1)
           u2(j1-js1,j2-js2,j3,ex)=f(2)
           u2(j1-js1,j2-js2,j3,ey)=f(3)

           u1(i1-2*is1,i2-2*is2,i3,ex)=f(4)
           u1(i1-2*is1,i2-2*is2,i3,ey)=f(5)
           u2(j1-2*js1,j2-2*js2,j3,ex)=f(6)
           u2(j1-2*js1,j2-2*js2,j3,ey)=f(7)
           end if

          if( debug.gt.0 )then ! re-evaluate
           f(0)=(u1x42r(i1,i2,i3,ex)+u1y42r(i1,i2,i3,ey)) - \
                (u2x42r(j1,j2,j3,ex)+u2y42r(j1,j2,j3,ey))
           f(1)=(u1xx42r(i1,i2,i3,ex)+u1yy42r(i1,i2,i3,ex)) - \
                (u2xx42r(j1,j2,j3,ex)+u2yy42r(j1,j2,j3,ex))

           f(2)=(u1x42r(i1,i2,i3,ey)-u1y42r(i1,i2,i3,ex)) - \
                (u2x42r(j1,j2,j3,ey)-u2y42r(j1,j2,j3,ex))
           
           f(3)=(u1xx42r(i1,i2,i3,ey)+u1yy42r(i1,i2,i3,ey))/eps1 - \
                (u2xx42r(j1,j2,j3,ey)+u2yy42r(j1,j2,j3,ey))/eps2
    
           ! These next we can do to 2nd order -- these need a value on the first ghost line --
           f(4)=(u1xxx22r(i1,i2,i3,ex)+u1xyy22r(i1,i2,i3,ex)+u1xxy22r(i1,i2,i3,ey)+u1yyy22r(i1,i2,i3,ey)) - \
                (u2xxx22r(j1,j2,j3,ex)+u2xyy22r(j1,j2,j3,ex)+u2xxy22r(j1,j2,j3,ey)+u2yyy22r(j1,j2,j3,ey))

           f(5)=((u1xxx22r(i1,i2,i3,ey)+u1xyy22r(i1,i2,i3,ey))-(u1xxy22r(i1,i2,i3,ex)+u1yyy22r(i1,i2,i3,ex)))/eps1 - \
                ((u2xxx22r(j1,j2,j3,ey)+u2xyy22r(j1,j2,j3,ey))-(u2xxy22r(j1,j2,j3,ex)+u2yyy22r(j1,j2,j3,ex)))/eps2

           f(6)=(u1LapSq22r(i1,i2,i3,ex))/eps1 - \
                (u2LapSq22r(j1,j2,j3,ex))/eps2

           f(7)=(u1LapSq22r(i1,i2,i3,ey))/eps1**2 - \
                (u2LapSq22r(j1,j2,j3,ey))/eps2**2
    
           write(*,'(" --> 4th: i1,i2=",2i4," f(re-eval)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)
          end if

           ! do this for now
           u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 
           u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)

           u1(i1-2*is1,i2-2*is2,i3,hz)=u2(j1+2*js1,j2+2*js2,j3,hz) 
           u2(j1-2*js1,j2-2*js2,j3,hz)=u1(i1+2*is1,i2+2*is2,i3,hz)

         endLoops2d()

         ! periodic update
         periodicUpdate2d(u1,boundaryCondition1,gridIndexRange1,side1,axis1)
         periodicUpdate2d(u2,boundaryCondition2,gridIndexRange2,side2,axis2)

       else if( orderOfAccuracy.eq.4 .and. gridType.eq.curvilinear )then
  
         ! --------------- 4th Order Curvilinear ---------------

         ! ---- first satisfy the jump conditions on the boundary --------
         !    [ eps n.u ] = 0
         !    [ tau.u ] = 0
         !    [ w ] = 0 
         boundaryJumpConditions(2,curvilinear)

         ! here are the real jump conditions for the ghost points
         ! 0  [ u.x + v.y ] = 0
         ! 1  [ n.(uv.xx + uv.yy) ] = 0
         ! 2  [ v.x - u.y ] =0 
         ! 3  [ tau.(v.xx+v.yy)/eps ] = 0
         ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0  OR [ (u.xx).x + (v.xx).y ] = 0 OR  [ (u.yy).x + (v.yy).y ] = 0 
         ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0
         ! 6  [ n.Delta^2 uv/eps ] = 0
         ! 7  [ tau.Delta^2 uv/eps^2 ] = 0 



         ! initialization step: assign first ghost line by extrapolation
         ! NOTE: assign ghost points outside the ends

         
         beginGhostLoops2d()
!
           ! *wdh* 090612 change extrap4 to extrap5 
            u1(i1-is1,i2-is2,i3,ex)=extrap6(u1,i1,i2,i3,ex,is1,is2,is3)
            u1(i1-is1,i2-is2,i3,ey)=extrap6(u1,i1,i2,i3,ey,is1,is2,is3)
            u1(i1-is1,i2-is2,i3,hz)=extrap6(u1,i1,i2,i3,hz,is1,is2,is3)
!
            u2(j1-js1,j2-js2,j3,ex)=extrap6(u2,j1,j2,j3,ex,js1,js2,js3)
            u2(j1-js1,j2-js2,j3,ey)=extrap6(u2,j1,j2,j3,ey,js1,js2,js3)
            u2(j1-js1,j2-js2,j3,hz)=extrap6(u2,j1,j2,j3,hz,js1,js2,js3)

           ! --- also extrap 2nd line for now
           u1(i1-2*is1,i2-2*is2,i3,ex)=extrap6(u1,i1-is1,i2-is2,i3,ex,is1,is2,is3)
           u1(i1-2*is1,i2-2*is2,i3,ey)=extrap6(u1,i1-is1,i2-is2,i3,ey,is1,is2,is3)
           u1(i1-2*is1,i2-2*is2,i3,hz)=extrap6(u1,i1-is1,i2-is2,i3,hz,is1,is2,is3)

           u2(j1-2*js1,j2-2*js2,j3,ex)=extrap6(u2,j1-js1,j2-js2,j3,ex,js1,js2,js3)
           u2(j1-2*js1,j2-2*js2,j3,ey)=extrap6(u2,j1-js1,j2-js2,j3,ey,js1,js2,js3)
           u2(j1-2*js1,j2-2*js2,j3,hz)=extrap6(u2,j1-js1,j2-js2,j3,hz,js1,js2,js3)
         endLoops2d()

         ! write(*,'(">>> interface: order=4 initialized=",i4)') initialized

         if( .false. ) then ! *wdh* 090720
           call bcAdjacent( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                               gridIndexRange1, u1, u1, mask1,rsxy1, xy1,\
                               boundaryCondition1, ipar1, rpar1, ierr )
           call bcAdjacent( nd, md1a,md1b,md2a,md2b,md3a,md3b,\
                               gridIndexRange2, u2, u2, mask2,rsxy2, xy2,\
                               boundaryCondition2, ipar2, rpar2, ierr )
         end if

         do it=1,nit ! *** begin iteration ****

           err=0.
         ! =============== start loops ======================
         nn=-1 ! counts points on the interface
         beginLoops2d() 

           nn=nn+1

           ! here is the normal (assumed to be the same on both sides)
           an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
           an2=rsxy1(i1,i2,i3,axis1,1)
           aNorm=max(epsx,sqrt(an1**2+an2**2))
           an1=an1/aNorm
           an2=an2/aNorm
           tau1=-an2
           tau2= an1

           ulap1=u1Laplacian42(i1,i2,i3,ex)
           vlap1=u1Laplacian42(i1,i2,i3,ey)
           ulap2=u2Laplacian42(j1,j2,j3,ex)
           vlap2=u2Laplacian42(j1,j2,j3,ey)

           ulapSq1=u1LapSq22(i1,i2,i3,ex)
           vlapSq1=u1LapSq22(i1,i2,i3,ey)
           ulapSq2=u2LapSq22(j1,j2,j3,ex)
           vlapSq2=u2LapSq22(j1,j2,j3,ey)

         

           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
           f(0)=(u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey)) - \
                (u2x42(j1,j2,j3,ex)+u2y42(j1,j2,j3,ey))

           f(1)=(an1*ulap1+an2*vlap1) - \
                (an1*ulap2+an2*vlap2)

           f(2)=(u1x42(i1,i2,i3,ey)-u1y42(i1,i2,i3,ex)) - \
                (u2x42(j1,j2,j3,ey)-u2y42(j1,j2,j3,ex))
           
           f(3)=(tau1*ulap1+tau2*vlap1)/eps1 - \
                (tau1*ulap2+tau2*vlap2)/eps2
    
           ! These next we can do to 2nd order -- these need a value on the first ghost line --
           f(4)=(u1xxx22(i1,i2,i3,ex)+u1xyy22(i1,i2,i3,ex)+u1xxy22(i1,i2,i3,ey)+u1yyy22(i1,i2,i3,ey)) - \
                (u2xxx22(j1,j2,j3,ex)+u2xyy22(j1,j2,j3,ex)+u2xxy22(j1,j2,j3,ey)+u2yyy22(j1,j2,j3,ey))

           f(5)=((u1xxx22(i1,i2,i3,ey)+u1xyy22(i1,i2,i3,ey))-(u1xxy22(i1,i2,i3,ex)+u1yyy22(i1,i2,i3,ex)))/eps1 - \
                ((u2xxx22(j1,j2,j3,ey)+u2xyy22(j1,j2,j3,ey))-(u2xxy22(j1,j2,j3,ex)+u2yyy22(j1,j2,j3,ex)))/eps2

           f(6)=(an1*ulapSq1+an2*vlapSq1)/eps1 - \
                (an1*ulapSq2+an2*vlapSq2)/eps2

           f(7)=(tau1*ulapSq1+tau2*vlapSq1)/eps1**2 - \
                (tau1*ulapSq2+tau2*vlapSq2)/eps2**2
           
!       if( debug.gt.0 ) write(*,'(" --> 4cth: j1,j2=",2i4," u1xx,u1yy,u2xx,u2yy=",4e10.2)') j1,j2,u1xx42(i1,i2,i3,ex),\
!            u1yy42(i1,i2,i3,ex),u2xx42(j1,j2,j3,ex),u2yy42(j1,j2,j3,ex)
!        if( debug.gt.0 ) write(*,'(" --> 4cth: i1,i2=",2i4," f(start)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)


! here are the macros from deriv.maple (file=derivMacros.h)

#defineMacro lapCoeff4a(is,dr,ds) ( (-2/3.*rxx*is-2/3.*ryy*is)/dr+(4/3.*rx**2+4/3.*ry**2)/dr**2 )

#defineMacro lapCoeff4b(is,dr,ds) ( (1/12.*rxx*is+1/12.*ryy*is)/dr+(-1/12.*rx**2-1/12.*ry**2)/dr**2 )

#defineMacro xLapCoeff4a(is,dr,ds) ( (-1/2.*rxyy*is-1/2.*rxxx*is+(sy*(ry*sx*is+sy*rx*is)+3*rx*sx**2*is+ry*sy*sx*is)/ds**2)/dr+(2*ry*rxy+3*rx*rxx+ryy*rx)/dr**2+(ry**2*rx*is+rx**3*is)/dr**3 )

#defineMacro xLapCoeff4b(is,dr,ds) ( (-1/2.*rx**3*is-1/2.*ry**2*rx*is)/dr**3 )

#defineMacro yLapCoeff4a(is,dr,ds) ( (-1/2.*ryyy*is-1/2.*rxxy*is+(3*ry*sy**2*is+ry*sx**2*is+2*sy*rx*sx*is)/ds**2)/dr+(2*rxy*rx+ry*rxx+3*ry*ryy)/dr**2+(ry**3*is+ry*rx**2*is)/dr**3 )

#defineMacro yLapCoeff4b(is,dr,ds) ( (-1/2.*ry*rx**2*is-1/2.*ry**3*is)/dr**3 )

#defineMacro lapSqCoeff4a(is,dr,ds) ( (-1/2.*rxxxx*is-rxxyy*is-1/2.*ryyyy*is+(2*sy*(2*rxy*sx*is+2*rx*sxy*is)+2*ry*(2*sxy*sx*is+sy*sxx*is)+7*rx*sxx*sx*is+sy*(3*ry*syy*is+3*sy*ryy*is)+sx*(3*rx*sxx*is+3*rxx*sx*is)+sx*(2*rxx*sx*is+2*rx*sxx*is)+2*sy*(2*rx*sxy*is+ry*sxx*is+2*rxy*sx*is+sy*rxx*is)+7*ry*sy*syy*is+rxx*sx**2*is+4*ry*sxy*sx*is+4*syy*rx*sx*is+2*ryy*sx**2*is+ryy*sy**2*is+sy*(2*sy*ryy*is+2*ry*syy*is))/ds**2)/dr+(3*ryy**2+3*rxx**2+4*rxy**2+4*ry*rxxy+4*rx*rxxx+4*ry*ryyy+2*ryy*rxx+4*rx*rxyy+(2*ry*(-4*sy*rx*sx-2*ry*sx**2)-12*ry**2*sy**2+2*sy*(-2*sy*rx**2-4*ry*rx*sx)-12*rx**2*sx**2)/ds**2)/dr**2+(6*ry**2*ryy*is+4*ry*rxy*rx*is+2*ry*(ry*rxx*is+2*rxy*rx*is)+6*rxx*rx**2*is+2*ryy*rx**2*is)/dr**3+(-8*ry**2*rx**2-4*ry**4-4*rx**4)/dr**4 )

#defineMacro lapSqCoeff4b(is,dr,ds) ( (-3*rxx*rx**2*is-ryy*rx**2*is-2*ry*rxy*rx*is-3*ry**2*ryy*is+2*ry*(-rxy*rx*is-1/2.*ry*rxx*is))/dr**3+(rx**4+2*ry**2*rx**2+ry**4)/dr**4 )


           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
           ! Solve:
           !     
           !       A [ U ] = A [ U(old) ] - [ f ]
!      u1r4(i1,i2,i3,kd)=(8.*(u1(i1+1,i2,i3,kd)-u1(i1-1,i2,i3,kd))-(u1(
!     & i1+2,i2,i3,kd)-u1(i1-2,i2,i3,kd)))*dr114(0)
!      u1x42(i1,i2,i3,kd)= rsxy1(i1,i2,i3,0,0)*u1r4(i1,i2,i3,kd)+rsxy1(
!     & i1,i2,i3,1,0)*u1s4(i1,i2,i3,kd)
!      u1y42(i1,i2,i3,kd)= rsxy1(i1,i2,i3,0,1)*u1r4(i1,i2,i3,kd)+rsxy1(
!     & i1,i2,i3,1,1)*u1s4(i1,i2,i3,kd)
!          a4(0,0) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))    ! coeff of u1(-1) from [u.x+v.y] 
!          a4(0,1) = -is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))    ! coeff of v1(-1) from [u.x+v.y] 
!
!          a4(2,0) =  is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))   ! coeff of u1(-1) from [v.x - u.y] 
!          a4(2,1) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))   ! coeff of v1(-1) from [v.x - u.y] 
!
!          a4(0,2) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))    ! coeff of u2(-1) from [u.x+v.y] 
!          a4(0,3) =  js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))    ! coeff of v2(-1) from [u.x+v.y] 
!
!          a4(2,2) = -js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))   ! coeff of u2(-1) from [v.x - u.y] 
!          a4(2,3) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))   ! coeff of v2(-1) from [v.x - u.y] 


           ! write(*,'(" interface:E: initialized,it=",2i4)') initialized,it
           if( initialized.eq.0 .and. it.eq.1 )then
             ! form the matrix (and save factor for later use)

             computeRxDerivatives(rv1,rsxy1,i1,i2,i3)
             computeRxDerivatives(rv2,rsxy2,j1,j2,j3)
 
             ! 0  [ u.x + v.y ] = 0
             aa8(0,0,0,nn) = -is*8.*rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)     ! coeff of u1(-1) from [u.x+v.y] 
             aa8(0,1,0,nn) = -is*8.*rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)     ! coeff of v1(-1) from [u.x+v.y] 
             aa8(0,4,0,nn) =  is*   rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)     ! u1(-2)
             aa8(0,5,0,nn) =  is*   rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)     ! v1(-2) 
  
             aa8(0,2,0,nn) =  js*8.*rsxy2(j1,j2,j3,axis2,0)*dr214(axis2)     ! coeff of u2(-1) from [u.x+v.y] 
             aa8(0,3,0,nn) =  js*8.*rsxy2(j1,j2,j3,axis2,1)*dr214(axis2)  
             aa8(0,6,0,nn) = -js*   rsxy2(j1,j2,j3,axis2,0)*dr214(axis2) 
             aa8(0,7,0,nn) = -js*   rsxy2(j1,j2,j3,axis2,1)*dr214(axis2)  

           ! 1  [ u.xx + u.yy ] = 0
! this macro comes from deriv.maple
! return the coefficient of u(-1) in uxxx+uxyy
!#defineMacro lapCoeff4a(is,dr,ds) ((-1/3.*rxx*is-1/3.*ryy*is)/dr+(4/3.*rx**2+4/3.*ry**2)/dr**2)

! return the coefficient of u(-2) in uxxx+uxyy
!#defineMacro lapCoeff4b(is,dr,ds) ((1/24.*rxx*is+1/24.*ryy*is)/dr+(-1/12.*rx**2-1/12.*ry**2)/dr**2 )

             setJacobian(rv1,axis1,axis1p1)
             dr0=dr1(axis1)
             ds0=dr1(axis1p1)
             aLap0 = lapCoeff4a(is,dr0,ds0)
             aLap1 = lapCoeff4b(is,dr0,ds0)
  
             setJacobian(rv2,axis2,axis2p1)
             dr0=dr2(axis2)
             ds0=dr2(axis2p1)
             bLap0 = lapCoeff4a(js,dr0,ds0)
             bLap1 = lapCoeff4b(js,dr0,ds0)
  
!             if( debug.gt.0 )then
!              aa8(1,0,0,nn) = 16.*dx142(axis1)         ! coeff of u1(-1) from [u.xx + u.yy]
!              aa8(1,4,0,nn) =    -dx142(axis1)         ! coeff of u1(-2) from [u.xx + u.yy]
!               write(*,'(" 4th: lap4: aLap0: rect=",e12.4," curv=",e12.4)') aLap0,aa8(1,0,0,nn)
!               write(*,'(" 4th: lap4: aLap1: rect=",e12.4," curv=",e12.4)') aLap1,aa8(1,4,0,nn)
!             end if
  
             aa8(1,0,0,nn) = an1*aLap0       ! coeff of u1(-1) from [n.(u.xx + u.yy)]
             aa8(1,1,0,nn) = an2*aLap0 
             aa8(1,4,0,nn) = an1*aLap1       ! coeff of u1(-2) from [n.(u.xx + u.yy)]
             aa8(1,5,0,nn) = an2*aLap1  
             
             aa8(1,2,0,nn) =-an1*bLap0       ! coeff of u2(-1) from [n.(u.xx + u.yy)]
             aa8(1,3,0,nn) =-an2*bLap0
             aa8(1,6,0,nn) =-an1*bLap1       ! coeff of u2(-2) from [n.(u.xx + u.yy)]
             aa8(1,7,0,nn) =-an2*bLap1
  
           ! 2  [ v.x - u.y ] =0 
!          a8(2,0) =  is*8.*ry1*dx114(axis1)
!          a8(2,1) = -is*8.*rx1*dx114(axis1)    ! coeff of v1(-1) from [v.x - u.y] 
!          a8(2,4) = -is*   ry1*dx114(axis1)
!          a8(2,5) =  is*   rx1*dx114(axis1)
!          a8(2,2) = -js*8.*ry2*dx214(axis2)
!          a8(2,3) =  js*8.*rx2*dx214(axis2)
!          a8(2,6) =  js*   ry2*dx214(axis2)
!          a8(2,7) = -js*   rx2*dx214(axis2)

             aa8(2,0,0,nn) =  is*8.*rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)    
             aa8(2,1,0,nn) = -is*8.*rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)    
             aa8(2,4,0,nn) = -is*   rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)       
             aa8(2,5,0,nn) =  is*   rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)       
  
             aa8(2,2,0,nn) = -js*8.*rsxy2(j1,j2,j3,axis2,1)*dr214(axis2)  
             aa8(2,3,0,nn) =  js*8.*rsxy2(j1,j2,j3,axis2,0)*dr214(axis2)    
             aa8(2,6,0,nn) =  js*   rsxy2(j1,j2,j3,axis2,1)*dr214(axis2)  
             aa8(2,7,0,nn) = -js*   rsxy2(j1,j2,j3,axis2,0)*dr214(axis2) 
  
             ! 3  [ tau.(uv.xx+uv.yy)/eps ] = 0
             aa8(3,0,0,nn) =tau1*aLap0/eps1
             aa8(3,1,0,nn) =tau2*aLap0/eps1
             aa8(3,4,0,nn) =tau1*aLap1/eps1
             aa8(3,5,0,nn) =tau2*aLap1/eps1
  
             aa8(3,2,0,nn) =-tau1*bLap0/eps2
             aa8(3,3,0,nn) =-tau2*bLap0/eps2
             aa8(3,6,0,nn) =-tau1*bLap1/eps2
             aa8(3,7,0,nn) =-tau2*bLap1/eps2
  
  
             ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0
  
            setJacobian(rv1,axis1,axis1p1)
  
  
            dr0=dr1(axis1)
            ds0=dr1(axis1p1)
            aLapX0 = xLapCoeff4a(is,dr0,ds0)
            aLapX1 = xLapCoeff4b(is,dr0,ds0)
  
            bLapY0 = yLapCoeff4a(is,dr0,ds0)
            bLapY1 = yLapCoeff4b(is,dr0,ds0)
  
            setJacobian(rv2,axis2,axis2p1)
  
            dr0=dr2(axis2)
            ds0=dr2(axis2p1)
            cLapX0 = xLapCoeff4a(js,dr0,ds0)
            cLapX1 = xLapCoeff4b(js,dr0,ds0)
  
            dLapY0 = yLapCoeff4a(js,dr0,ds0)
            dLapY1 = yLapCoeff4b(js,dr0,ds0)
  
  
            ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0
!             if( debug.gt.0 )then
!             aa8(4,0,0,nn)= ( is*rx1*2.*dx122(axis1)*dx112(axis1)+is*rx1*2.*dx122(1)/(2.*dx1(0)))
!             aa8(4,1,0,nn)= ( is*ry1*2.*dx122(axis1)*dx112(axis1)+is*ry1*2.*dx122(0)/(2.*dx1(1)))
!             aa8(4,4,0,nn)= (-is*rx1   *dx122(axis1)*dx112(axis1) )  
!             aa8(4,5,0,nn)= (-is*ry1   *dx122(axis1)*dx112(axis1))
!               write(*,'(" 4th: xlap4: aLapX0: rect=",e12.4," curv=",e12.4)') aLapX0,aa8(4,0,0,nn)
!               write(*,'(" 4th: xlap4: aLapX1: rect=",e12.4," curv=",e12.4)') aLapX1,aa8(4,4,0,nn)
!               write(*,'(" 4th: ylap4: bLapY0: rect=",e12.4," curv=",e12.4)') bLapY0,aa8(4,1,0,nn)
!               write(*,'(" 4th: ylap4: bLapY1: rect=",e12.4," curv=",e12.4)') bLapY1,aa8(4,5,0,nn)
!             end if
  
            aa8(4,0,0,nn)= aLapX0
            aa8(4,1,0,nn)= bLapY0
            aa8(4,4,0,nn)= aLapX1
            aa8(4,5,0,nn)= bLapY1
  
            aa8(4,2,0,nn)=-cLapX0
            aa8(4,3,0,nn)=-dLapY0
            aa8(4,6,0,nn)=-cLapX1
            aa8(4,7,0,nn)=-dLapY1
  
            ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0
  
            aa8(5,0,0,nn)=-bLapY0/eps1
            aa8(5,1,0,nn)= aLapX0/eps1
            aa8(5,4,0,nn)=-bLapY1/eps1
            aa8(5,5,0,nn)= aLapX1/eps1
  
            aa8(5,2,0,nn)= dLapY0/eps2
            aa8(5,3,0,nn)=-cLapX0/eps2
            aa8(5,6,0,nn)= dLapY1/eps2
            aa8(5,7,0,nn)=-cLapX1/eps2
  
  
             ! 6  [ n.Delta^2 u/eps ] = 0
  
             ! assign rx,ry,rxx,rxy,... 
             setJacobian(rv1,axis1,axis1p1)
             dr0=dr1(axis1)
             ds0=dr1(axis1p1)
             aLapSq0 = lapSqCoeff4a(is,dr0,ds0)
             aLapSq1 = lapSqCoeff4b(is,dr0,ds0)
  
!              if( debug.gt.0 )then
!                aa8(6,0,0,nn) = -(4./(dx1(axis1)**4) +4./(dx1(0)**2*dx1(1)**2) )
!                aa8(6,4,0,nn) =   1./(dx1(axis1)**4)
!                write(*,'(" 4th: lapSq: aLapSq0: rect=",e12.4," curv=",e12.4)') aLapSq0,aa8(6,0,0,nn)
!                write(*,'(" 4th: lapSq: aLapSq1: rect=",e12.4," curv=",e12.4)') aLapSq1,aa8(6,4,0,nn)
!              end if
  
             aa8(6,0,0,nn) = an1*aLapSq0/eps1
             aa8(6,1,0,nn) = an2*aLapSq0/eps1
             aa8(6,4,0,nn) = an1*aLapSq1/eps1
             aa8(6,5,0,nn) = an2*aLapSq1/eps1
  
             setJacobian(rv2,axis2,axis2p1)
             dr0=dr2(axis2)
             ds0=dr2(axis2p1)
             bLapSq0 = lapSqCoeff4a(js,dr0,ds0)
             bLapSq1 = lapSqCoeff4b(js,dr0,ds0)
  
             aa8(6,2,0,nn) = -an1*bLapSq0/eps2
             aa8(6,3,0,nn) = -an2*bLapSq0/eps2
             aa8(6,6,0,nn) = -an1*bLapSq1/eps2
             aa8(6,7,0,nn) = -an2*bLapSq1/eps2
  
             ! 7  [ tau.Delta^2 v/eps^2 ] = 0 
             aa8(7,0,0,nn) = tau1*aLapSq0/eps1**2
             aa8(7,1,0,nn) = tau2*aLapSq0/eps1**2
             aa8(7,4,0,nn) = tau1*aLapSq1/eps1**2
             aa8(7,5,0,nn) = tau2*aLapSq1/eps1**2
  
             aa8(7,2,0,nn) = -tau1*bLapSq0/eps2**2
             aa8(7,3,0,nn) = -tau2*bLapSq0/eps2**2
             aa8(7,6,0,nn) = -tau1*bLapSq1/eps2**2
             aa8(7,7,0,nn) = -tau2*bLapSq1/eps2**2
  
             ! save a copy of the matrix
             do n2=0,7
             do n1=0,7
               aa8(n1,n2,1,nn)=aa8(n1,n2,0,nn)
             end do
             end do
  
             ! solve A Q = F
             ! factor the matrix
             numberOfEquations=8
             call dgeco( aa8(0,0,0,nn), numberOfEquations, numberOfEquations, ipvt8(0,nn),rcond,work(0))

             ! if( debug.gt.0 ) write(*,'(" --> 4cth: i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
             ! '
           end if


           q(0) = u1(i1-is1,i2-is2,i3,ex)
           q(1) = u1(i1-is1,i2-is2,i3,ey)
           q(2) = u2(j1-js1,j2-js2,j3,ex)
           q(3) = u2(j1-js1,j2-js2,j3,ey)

           q(4) = u1(i1-2*is1,i2-2*is2,i3,ex)
           q(5) = u1(i1-2*is1,i2-2*is2,i3,ey)
           q(6) = u2(j1-2*js1,j2-2*js2,j3,ex)
           q(7) = u2(j1-2*js1,j2-2*js2,j3,ey)

!       if( debug.gt.0 ) write(*,'(" --> 4cth: i1,i2=",2i4," q=",8e10.2)') i1,i2,q(0),q(1),q(2),q(3),q(4),q(5),q(6),q(7)

           ! subtract off the contributions from the initial (wrong) values at the ghost points:
           do n=0,7
             f(n) = (aa8(n,0,1,nn)*q(0)+aa8(n,1,1,nn)*q(1)+aa8(n,2,1,nn)*q(2)+aa8(n,3,1,nn)*q(3)+\
                     aa8(n,4,1,nn)*q(4)+aa8(n,5,1,nn)*q(5)+aa8(n,6,1,nn)*q(6)+aa8(n,7,1,nn)*q(7)) - f(n)
           end do

                                ! '

           ! solve A Q = F
           job=0
           numberOfEquations=8
           call dgesl( aa8(0,0,0,nn), numberOfEquations, numberOfEquations, ipvt8(0,nn), f(0), job)

!       if( debug.gt.0 ) write(*,'(" --> 4cth: i1,i2=",2i4," f(solve)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)
           ! '

!            if( .true. )then
           u1(i1-is1,i2-is2,i3,ex)=f(0)
           u1(i1-is1,i2-is2,i3,ey)=f(1)
           u2(j1-js1,j2-js2,j3,ex)=f(2)
           u2(j1-js1,j2-js2,j3,ey)=f(3)

           u1(i1-2*is1,i2-2*is2,i3,ex)=f(4)
           u1(i1-2*is1,i2-2*is2,i3,ey)=f(5)
           u2(j1-2*js1,j2-2*js2,j3,ex)=f(6)
           u2(j1-2*js1,j2-2*js2,j3,ey)=f(7)
 !           end if

           ! compute the maximum change in the solution for this iteration
           ! if( .true. )then
           do n=0,7
              err=max(err,abs(q(n)-f(n)))
            end do
           ! end if

!!$          if( debug.gt.0 )then ! re-evaluate
!!$
!!$
!!$           ulap1=u1Laplacian42(i1,i2,i3,ex)
!!$           vlap1=u1Laplacian42(i1,i2,i3,ey)
!!$           ulap2=u2Laplacian42(j1,j2,j3,ex)
!!$           vlap2=u2Laplacian42(j1,j2,j3,ey)
!!$
!!$           ulapSq1=u1LapSq22(i1,i2,i3,ex)
!!$           vlapSq1=u1LapSq22(i1,i2,i3,ey)
!!$           ulapSq2=u2LapSq22(j1,j2,j3,ex)
!!$           vlapSq2=u2LapSq22(j1,j2,j3,ey)
!!$
!!$           f(0)=(u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey)) - \
!!$                (u2x42(j1,j2,j3,ex)+u2y42(j1,j2,j3,ey))
!!$
!!$           f(1)=(an1*ulap1+an2*vlap1) - \
!!$                (an1*ulap2+an2*vlap2)
!!$
!!$           f(2)=(u1x42(i1,i2,i3,ey)-u1y42(i1,i2,i3,ex)) - \
!!$                (u2x42(j1,j2,j3,ey)-u2y42(j1,j2,j3,ex))
!!$           
!!$           f(3)=(tau1*ulap1+tau2*vlap1)/eps1 - \
!!$                (tau1*ulap2+tau2*vlap2)/eps2
!!$    
!!$           ! These next we can do to 2nd order -- these need a value on the first ghost line --
!!$           f(4)=(u1xxx22(i1,i2,i3,ex)+u1xyy22(i1,i2,i3,ex)+u1xxy22(i1,i2,i3,ey)+u1yyy22(i1,i2,i3,ey)) - \
!!$                (u2xxx22(j1,j2,j3,ex)+u2xyy22(j1,j2,j3,ex)+u2xxy22(j1,j2,j3,ey)+u2yyy22(j1,j2,j3,ey))
!!$
!!$           f(5)=((u1xxx22(i1,i2,i3,ey)+u1xyy22(i1,i2,i3,ey))-(u1xxy22(i1,i2,i3,ex)+u1yyy22(i1,i2,i3,ex)))/eps1 - \
!!$                ((u2xxx22(j1,j2,j3,ey)+u2xyy22(j1,j2,j3,ey))-(u2xxy22(j1,j2,j3,ex)+u2yyy22(j1,j2,j3,ex)))/eps2
!!$
!!$           f(6)=(an1*ulapSq1+an2*vlapSq1)/eps1 - \
!!$                (an1*ulapSq2+an2*vlapSq2)/eps2
!!$
!!$           f(7)=(tau1*ulapSq1+tau2*vlapSq1)/eps1**2 - \
!!$                (tau1*ulapSq2+tau2*vlapSq2)/eps2**2
!!$
!!$    
!!$           if( debug.gt.0 ) write(*,'(" --> 4cth: i1,i2=",2i4," f(re-eval)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)
!!$             ! '
!!$          end if

           ! ******************************************************
           ! solve for Hz
           !  [ w.n/eps ] = 0
           !  [ lap(w)/eps ] = 0
           !  [ lap(w).n/eps**2 ] = 0
           !  [ lapSq(w)/eps**2 ] = 0

           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
           wlap1=u1Laplacian42(i1,i2,i3,hz)
           wlap2=u2Laplacian42(j1,j2,j3,hz)

           wlapSq1=u1LapSq22(i1,i2,i3,hz)
           wlapSq2=u2LapSq22(j1,j2,j3,hz)

           f(0)=(an1*u1x42(i1,i2,i3,hz)+an2*u1y42(i1,i2,i3,hz))/eps1 - \
                (an1*u2x42(j1,j2,j3,hz)+an2*u2y42(j1,j2,j3,hz))/eps2

           f(1)=wlap1/eps1 - \
                wlap2/eps2

           ! These next we can do to 2nd order -- these need a value on the first ghost line --
           f(2)=(an1*(u1xxx22(i1,i2,i3,hz)+u1xyy22(i1,i2,i3,hz))+an2*(u1xxy22(i1,i2,i3,hz)+u1yyy22(i1,i2,i3,hz)))/eps1**2 - \
                (an1*(u2xxx22(j1,j2,j3,hz)+u2xyy22(j1,j2,j3,hz))+an2*(u2xxy22(j1,j2,j3,hz)+u2yyy22(j1,j2,j3,hz)))/eps2**2

           f(3)=wlapSq1/eps1**2 - \
                wlapSq2/eps2**2

           if( initialized.eq.0 .and. it.eq.1 )then
             ! form the matrix for computing Hz (and save factor for later use)

             ! 1: [ w.n/eps ] = 0
             a0 = (an1*rsxy1(i1,i2,i3,axis1,0)+an2*rsxy1(i1,i2,i3,axis1,1))*dr114(axis1)/eps1
             b0 = (an1*rsxy2(j1,j2,j3,axis2,0)+an2*rsxy2(j1,j2,j3,axis2,1))*dr214(axis2)/eps2
             aa4(0,0,0,nn) = -is*8.*a0
             aa4(0,2,0,nn) =  is*   a0
             aa4(0,1,0,nn) =  js*8.*b0
             aa4(0,3,0,nn) = -js*   b0
  
             ! 2: [ lap(w)/eps ] = 0 
             aa4(1,0,0,nn) = aLap0/eps1
             aa4(1,2,0,nn) = aLap1/eps1
             aa4(1,1,0,nn) =-bLap0/eps2
             aa4(1,3,0,nn) =-bLap1/eps2
  
             ! 3  [ (an1*(w.xx+w.yy).x + an2.(w.xx+w.yy).y)/eps**2 ] = 0
             aa4(2,0,0,nn)= (an1*aLapX0+an2*bLapY0)/eps1**2
             aa4(2,2,0,nn)= (an1*aLapX1+an2*bLapY1)/eps1**2
             aa4(2,1,0,nn)=-(an1*cLapX0+an2*dLapY0)/eps2**2
             aa4(2,3,0,nn)=-(an1*cLapX1+an2*dLapY1)/eps2**2
  
             ! 4 [ lapSq(w)/eps**2 ] = 0 
             aa4(3,0,0,nn) = aLapSq0/eps1**2
             aa4(3,2,0,nn) = aLapSq1/eps1**2
             aa4(3,1,0,nn) =-bLapSq0/eps2**2
             aa4(3,3,0,nn) =-bLapSq1/eps2**2

             ! save a copy of the matrix
             do n2=0,3
             do n1=0,3
               aa4(n1,n2,1,nn)=aa4(n1,n2,0,nn)
             end do
             end do
  
             ! factor the matrix
             numberOfEquations=4
             call dgeco( aa4(0,0,0,nn), numberOfEquations, numberOfEquations, ipvt4(0,nn),rcond,work(0))
           end if

           q(0) = u1(i1-is1,i2-is2,i3,hz)
           q(1) = u2(j1-js1,j2-js2,j3,hz)
           q(2) = u1(i1-2*is1,i2-2*is2,i3,hz)
           q(3) = u2(j1-2*js1,j2-2*js2,j3,hz)

           ! subtract off the contributions from the wrong values at the ghost points:
           do n=0,3
             f(n) = (aa4(n,0,1,nn)*q(0)+aa4(n,1,1,nn)*q(1)+aa4(n,2,1,nn)*q(2)+aa4(n,3,1,nn)*q(3)) - f(n)
           end do
           ! solve
           numberOfEquations=4
           job=0
           call dgesl( aa4(0,0,0,nn), numberOfEquations, numberOfEquations, ipvt4(0,nn), f(0), job)

           u1(i1-is1,i2-is2,i3,hz)=f(0)
           u2(j1-js1,j2-js2,j3,hz)=f(1)
           u1(i1-2*is1,i2-2*is2,i3,hz)=f(2)
           u2(j1-2*js1,j2-2*js2,j3,hz)=f(3)

!          if( debug.gt.0 )then ! re-evaluate
!
!
!           wlap1=u1Laplacian42(i1,i2,i3,hz)
!           wlap2=u2Laplacian42(j1,j2,j3,hz)
!
!           wlapSq1=u1LapSq22(i1,i2,i3,hz)
!           wlapSq2=u2LapSq22(j1,j2,j3,hz)
!
!           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
!           f(0)=(an1*u1x42(i1,i2,i3,hz)+an2*u1y42(i1,i2,i3,hz))/eps1 - \
!                (an1*u2x42(j1,j2,j3,hz)+an2*u2y42(j1,j2,j3,hz))/eps2
!
!           f(1)=wlap1/eps1 - \
!                wlap2/eps2
!
!           ! These next we can do to 2nd order -- these need a value on the first ghost line --
!           f(2)=(an1*(u1xxx22(i1,i2,i3,hz)+u1xyy22(i1,i2,i3,hz))+an2*(u1xxy22(i1,i2,i3,hz)+u1yyy22(i1,i2,i3,hz)))/eps1**2 - \
!                (an1*(u2xxx22(j1,j2,j3,hz)+u2xyy22(j1,j2,j3,hz))+an2*(u2xxy22(j1,j2,j3,hz)+u2yyy22(j1,j2,j3,hz)))/eps2**2
!
!           f(3)=wlapSq1/eps1**2 - \
!                wlapSq2/eps2**2
!    
!           if( debug.gt.0 ) write(*,'(" --> 4cth: i1,i2=",2i4," hz-f(re-eval)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3)
!             ! '
!          end if



           ! ***********************

           ! u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 
           ! u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)
           ! u1(i1-2*is1,i2-2*is2,i3,hz)=u2(j1+2*js1,j2+2*js2,j3,hz) 
           ! u2(j1-2*js1,j2-2*js2,j3,hz)=u1(i1+2*is1,i2+2*is2,i3,hz)

         endLoops2d()
         ! =============== end loops =======================
      
         ! *wdh* 090509 -- fix up ends that match to symmetry BC's -- we should do PEC too!
         if( .false. )then
           ! old way: 
           bc0=0
           call bcSymmetry( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                            gridIndexRange1, u1, mask1,rsxy1, xy1,\
                            bc0, boundaryCondition1, ipar1, rpar1, ierr )
           call bcSymmetry( nd, md1a,md1b,md2a,md2b,md3a,md3b,\
                            gridIndexRange2, u2, mask2,rsxy2, xy2,\
                            bc0, boundaryCondition2, ipar2, rpar2, ierr )
         else
           ! new way *wdh* 090719
           ! -- we may need u1 at t-dt in the future: 
           call bcAdjacent( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                               gridIndexRange1, u1, u1, mask1,rsxy1, xy1,\
                               boundaryCondition1, ipar1, rpar1, ierr )
           call bcAdjacent( nd, md1a,md1b,md2a,md2b,md3a,md3b,\
                               gridIndexRange2, u2, u2, mask2,rsxy2, xy2,\
                               boundaryCondition2, ipar2, rpar2, ierr )
         end if

         periodicUpdate2d(u1,boundaryCondition1,gridIndexRange1,side1,axis1)
         periodicUpdate2d(u2,boundaryCondition2,gridIndexRange2,side2,axis2)

           if( debug.gt.0 )then 
             write(*,'(" ***interface:2d order 4 curv: it=",i2," max-diff = ",e11.2)') it,err
               ! '
           end if
         end do ! ************** end iteration **************


         ! now make sure that div(u)=0 etc.
         if( .true. )then
           if( debug.gt.1 )then 
             write(*,'(" ***interface:2d order 4 curv: PROJECT div(u) ")') 
           end if
         beginLoops2d() ! =============== start loops =======================

           ! 0  [ u.x + v.y ] = 0
!           a8(0,0) = -is*8.*rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)     ! coeff of u1(-1) from [u.x+v.y] 
!           a8(0,1) = -is*8.*rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)     ! coeff of v1(-1) from [u.x+v.y] 
!           a8(0,4) =  is*   rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)     ! u1(-2)
!           a8(0,5) =  is*   rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)     ! v1(-2) 

!           a8(0,2) =  js*8.*rsxy2(j1,j2,j3,axis2,0)*dr214(axis2)     ! coeff of u2(-1) from [u.x+v.y] 
!           a8(0,3) =  js*8.*rsxy2(j1,j2,j3,axis2,1)*dr214(axis2)  
!           a8(0,6) = -js*   rsxy2(j1,j2,j3,axis2,0)*dr214(axis2) 
!           a8(0,7) = -js*   rsxy2(j1,j2,j3,axis2,1)*dr214(axis2) 

           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
           divu=u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey)
           a0=is*   rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)
           a1=is*   rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)
           aNormSq=a0**2+a1**2
           ! now project:  a.uNew = a.uOld - div  ->  (div-a.uOld)+a.uNew = div(uNew) = 0
           u1(i1-2*is1,i2-2*is2,i3,ex)=u1(i1-2*is1,i2-2*is2,i3,ex)-divu*a0/aNormSq
           u1(i1-2*is1,i2-2*is2,i3,ey)=u1(i1-2*is1,i2-2*is2,i3,ey)-divu*a1/aNormSq

           divu=u2x42(j1,j2,j3,ex)+u2y42(j1,j2,j3,ey)
           a0=js*   rsxy2(j1,j2,j3,axis2,0)*dr214(axis2) 
           a1=js*   rsxy2(j1,j2,j3,axis2,1)*dr214(axis2) 
           aNormSq=a0**2+a1**2

           u2(j1-2*js1,j2-2*js2,j3,ex)=u2(j1-2*js1,j2-2*js2,j3,ex)-divu*a0/aNormSq
           u2(j1-2*js1,j2-2*js2,j3,ey)=u2(j1-2*js1,j2-2*js2,j3,ey)-divu*a1/aNormSq

!           if( debug.gt.0 )then
!             divu=u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey)
!              write(*,'(" --> 4cth: eval div1,div2=",2e10.2)') u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey),u2x42(j1,j2,j3,ex)+u2y42(j1,j2,j3,ey)
!           end if
         endLoops2d()
         end if
       else
         stop 3214
       end if
      else  
         ! 3D
        stop 6676
      end if

      return
      end
