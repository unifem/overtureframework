c *******************************************************************************
c   Interface boundary conditions **new version**
c
c   This file uses .h files generated from interfaceMacros.bf
c
c *******************************************************************************


      subroutine interfaceDerivatives( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                               gridIndexRange1, u1, mask1,rsxy1, xy1, boundaryCondition1, \
                               md1a,md1b,md2a,md2b,md3a,md3b,\
                               gridIndexRange2, u2, mask2,rsxy2, xy2, boundaryCondition2, \
                               ipar, rpar, \
                               option, i1,i2,i3, j1,j2,j3, f, \
                               c1x6,c1y6, c1xx6,c1xy6,c1yy6, c1Lap6,\
                               c2x6,c2y6, c2xx6,c2xy6,c2yy6, c2Lap6,\
                               c1xLap4,c1yLap4,c1LapSq4, c1xLapSq2,c1yLapSq2,c1LapCubed2,\
                               c2xLap4,c2yLap4,c2LapSq4, c2xLapSq2,c2yLapSq2,c2LapCubed2,\
       c1xxx4,c1xxy4,c1xyy4,c1yyy4,c1xxxxy2,c1xxyyy2,c1yyyyy2, c1xxxyy2,c1xyyyy2,\
       c2xxx4,c2xxy4,c2xyy4,c2yyy4,c2xxxxy2,c2xxyyy2,c2yyyyy2, c2xxxyy2,c2xyyyy2,\
                               ierr )
c ===================================================================================
c  Interface boundary conditions for Maxwell's Equations.
c
c  gridType : 0=rectangular, 1=curvilinear
c
c  u1: solution on the "left" of the interface
c  u2: solution on the "right" of the interface
c
c ===================================================================================

      implicit none

      integer nd, \
              nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, \
              md1a,md1b,md2a,md2b,md3a,md3b, \
              n1a,n1b,n2a,n2b,n3a,n3b,  \
              m1a,m1b,m2a,m2b,m3a,m3b,  \
              option,ierr

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

      real f(0:11)
      real c1x6(3),c1y6(3), c1xx6(3),c1xy6(3),c1yy6(3), c1Lap6(3)
      real c2x6(3),c2y6(3), c2xx6(3),c2xy6(3),c2yy6(3), c2Lap6(3)

      real c1xLap4(3),c1yLap4(3),c1LapSq4(3), c1xLapSq2(3),c1yLapSq2(3),c1LapCubed2(3)
      real c2xLap4(3),c2yLap4(3),c2LapSq4(3), c2xLapSq2(3),c2yLapSq2(3),c2LapCubed2(3)

      real c1xxx4(3),c1xxy4(3),c1xyy4(3),c1yyy4(3),c1xxxxy2(3),c1xxyyy2(3),c1yyyyy2(3), c1xxxyy2(3),c1xyyyy2(3)
      real c2xxx4(3),c2xxy4(3),c2xyy4(3),c2yyy4(3),c2xxxxy2(3),c2xxyyy2(3),c2yyyyy2(3), c2xxxyy2(3),c2xyyyy2(3)

c     --- local variables ----
      
      integer side1,axis1,grid1,side2,axis2,grid2,gridType,orderOfAccuracy,orderOfExtrapolation,useForcing,\
        ex,ey,ez,hx,hy,hz,useWhereMask,debug,solveForE,solveForH,axis1p1,axis2p1
      real dx1(0:2),dr1(0:2),dx2(0:2),dr2(0:2)
      real dx(0:2),dr(0:2)
      real t,ep,dt,eps1,mu1,c1,eps2,mu2,c2
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,j1,j2,j3,js1,js2,js3,ks1,ks2,ks3,is,js,it,nit

      integer numGhost,giveDiv
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
      integer mm1a,mm1b,mm2a,mm2b,mm3a,mm3b

      real rx1,ry1,rx2,ry2

      real aLap0,aLap1,bLap0,bLap1,aLapX0,aLapX1,bLapY0,bLapY1,cLapX0,cLapX1,dLapY0,dLapY1,aLapSq0,aLapSq1,bLapSq0,bLapSq1
      real a0,a1,b0,b1,cc0,cc1,d0,d1,dr0,ds0
      real aNormSq,divu

      real epsRatio,an1,an2,aNorm,ua,ub,nDotU,tau1a,tau2a
      real epsx

      real tau1,tau2,clap1,clap2,ulap1,vlap1,wlap1,ulap2,vlap2,wlap2,an1Cartesian,an2Cartesian
      real ulapSq1,vlapSq1,ulapSq2,vlapSq2,wlapSq1,wlapSq2
      real ulapCubed1,vlapCubed1,ulapCubed2,vlapCubed2,wlapCubed1,wlapCubed2

      integer np1a,np1b,np2a,np2b,np3a,np3b,diff(0:2)

      real rx,ry,rxx,rxy,ryy,rxxx,rxxy,rxyy,ryyy,rxxxx,rxxyy,ryyyy
      real sx,sy,sxx,sxy,syy,sxxx,sxxy,sxyy,syyy,sxxxx,sxxyy,syyyy


      integer numberOfEquations,job
      real a2(0:1,0:1),a4(0:3,0:3),a8(0:7,0:7),aa(0:11,0:11),q(0:11),ipvt(0:11),rcond,work(0:11)
      real scale(0:11)

      ! boundary conditions parameters
      #Include "bcDefineFortranInclude.h"
 
      integer rectangular,curvilinear
      parameter(\
        rectangular=0,\
        curvilinear=1)

      include 'declareTemporaryVariablesOrder6.h'
c!  declareTemporaryVariables(DIM,MAXDERIV)
c      declareTemporaryVariables(2,8)
c
c! declareParametricDerivativeVariables(v,DIM)
c      declareParametricDerivativeVariables(uu1,2)
c      declareParametricDerivativeVariables(vv1,2)
c      declareParametricDerivativeVariables(ww1,2)
c      declareJacobianDerivativeVariables(a1j2,2)
c      declareJacobianDerivativeVariables(a1j4,2)
c      declareJacobianDerivativeVariables(a1j6,2)
c     
c      declareParametricDerivativeVariables(uu2,2)
c      declareParametricDerivativeVariables(vv2,2)
c      declareParametricDerivativeVariables(ww2,2)
c      declareJacobianDerivativeVariables(a2j2,2)
c      declareJacobianDerivativeVariables(a2j4,2)
c      declareJacobianDerivativeVariables(a2j6,2)

      real u1LapSq2,u2LapSq2
      real uu1x6,uu1y6,uu1xx6,uu1yy6
      real vv1x6,vv1y6,vv1xx6,vv1yy6      
      real ww1x6,ww1y6,ww1xx6,ww1yy6      

      real uu2x6,uu2y6,uu2xx6,uu2yy6      
      real vv2x6,vv2y6,vv2xx6,vv2yy6      
      real ww2x6,ww2y6,ww2xx6,ww2yy6      

      real uu1xxx4,uu1xxy4,uu1xyy4,uu1yyy4, uu1xxxx4,uu1xxyy4,uu1yyyy4
      real vv1xxx4,vv1xxy4,vv1xyy4,vv1yyy4, vv1xxxx4,vv1xxyy4,vv1yyyy4
      real ww1xxx4,ww1xxy4,ww1xyy4,ww1yyy4, ww1xxxx4,ww1xxyy4,ww1yyyy4

      real uu2xxx4,uu2xxy4,uu2xyy4,uu2yyy4, uu2xxxx4,uu2xxyy4,uu2yyyy4
      real vv2xxx4,vv2xxy4,vv2xyy4,vv2yyy4, vv2xxxx4,vv2xxyy4,vv2yyyy4
      real ww2xxx4,ww2xxy4,ww2xyy4,ww2yyy4, ww2xxxx4,ww2xxyy4,ww2yyyy4

      real uu1xxxxx2,uu1xxxxy2,uu1xxxyy2,uu1xxyyy2,uu1xyyyy2,uu1yyyyy2, uu1xxxxxx2,uu1xxxxyy2,uu1xxyyyy2,uu1yyyyyy2
      real vv1xxxxx2,vv1xxxxy2,vv1xxxyy2,vv1xxyyy2,vv1xyyyy2,vv1yyyyy2, vv1xxxxxx2,vv1xxxxyy2,vv1xxyyyy2,vv1yyyyyy2
      real ww1xxxxx2,ww1xxxxy2,ww1xxxyy2,ww1xxyyy2,ww1xyyyy2,ww1yyyyy2, ww1xxxxxx2,ww1xxxxyy2,ww1xxyyyy2,ww1yyyyyy2

      real uu2xxxxx2,uu2xxxxy2,uu2xxxyy2,uu2xxyyy2,uu2xyyyy2,uu2yyyyy2, uu2xxxxxx2,uu2xxxxyy2,uu2xxyyyy2,uu2yyyyyy2
      real vv2xxxxx2,vv2xxxxy2,vv2xxxyy2,vv2xxyyy2,vv2xyyyy2,vv2yyyyy2, vv2xxxxxx2,vv2xxxxyy2,vv2xxyyyy2,vv2yyyyyy2
      real ww2xxxxx2,ww2xxxxy2,ww2xxxyy2,ww2xxyyy2,ww2xyyyy2,ww2yyyyy2, ww2xxxxxx2,ww2xxxxyy2,ww2xxyyyy2,ww2yyyyyy2

      real dr1a,ds1a,dr2a,ds2a

      real err,err1,err2,err3,omega
      integer ne,interfaceOption

      real dx141,dx142,dx112,dx122
      real dx241,dx242,dx212,dx222

c     --- start statement function ----
      integer kd,m,n
c     real rx,ry,rz,sx,sy,sz,tx,ty,tz
c      declareDifferenceNewOrder2(u1,rsxy1,dr1,dx1,RX)
c      declareDifferenceNewOrder2(u2,rsxy2,dr2,dx2,RX)

c      declareDifferenceNewOrder4(u1,rsxy1,dr1,dx1,RX)
c      declareDifferenceNewOrder4(u2,rsxy2,dr2,dx2,RX)

c.......statement functions for jacobian
c     The next macro call will define the difference approximation statement functions
c      defineDifferenceNewOrder2Components1(u1,rsxy1,dr1,dx1,RX)
c      defineDifferenceNewOrder2Components1(u2,rsxy2,dr2,dx2,RX)c

c      defineDifferenceNewOrder4Components1(u1,rsxy1,dr1,dx1,RX)
c      defineDifferenceNewOrder4Components1(u2,rsxy2,dr2,dx2,RX)

c      u1LapSq2(i1,i2,i3,n)=u1xxxx2(i1,i2,i3,n)+2.*u1xxyy2(i1,i2,i3,n)+u1yyyy2(i1,i2,i3,n)
c      u2LapSq2(i1,i2,i3,n)=u2xxxx2(i1,i2,i3,n)+2.*u2xxyy2(i1,i2,i3,n)+u2yyyy2(i1,i2,i3,n)


c............... end statement functions

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
      interfaceOption      =ipar(33)
     
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
      omega                =rpar(21)
     

      epsx=1.e-20  ! fix this 


      numGhost=orderOfAccuracy/2
      giveDiv=0   ! set to 1 to give div(u) on both sides, rather than setting the jump in div(u)

      if( nd.eq.2 )then

       axis1p1=mod(axis1+1,nd)
       axis2p1=mod(axis2+1,nd)

       is1=0
       is2=0
       is3=0

       if( axis1.eq.0 ) then
         is1=1-2*side1
         if( boundaryCondition1(0,axis1p1).lt.0 )then
           ! include ghost lines in tangential directions (for extrapolating)
           nn2a=nn2a-numGhost
           nn2b=nn2b+numGhost
         end if
         an1Cartesian=1. ! normal for a cartesian grid
         an2Cartesian=0.
       else
         is2=1-2*side1
         if( boundaryCondition1(0,axis1p1).lt.0 )then
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
         if( boundaryCondition1(0,axis2p1).lt.0 )then
           mm2a=mm2a-numGhost
           mm2b=mm2b+numGhost
         end if
       else
         js2=1-2*side2
         if( boundaryCondition1(0,axis2p1).lt.0 )then
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

  
       if( orderOfAccuracy.eq.6 .and. gridType.eq.rectangular )then
  
         stop 1143


       else if( orderOfAccuracy.eq.6 .and. gridType.eq.curvilinear )then
  
         ! --------------- 6th Order Curvilinear ---------------


           ! here is the normal (assumed to be the same on both sides)
           an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
           an2=rsxy1(i1,i2,i3,axis1,1)
           aNorm=max(epsx,sqrt(an1**2+an2**2))
           an1=an1/aNorm
           an2=an2/aNorm
           tau1=-an2
           tau2= an1
           tau1a=abs(tau1)
           tau2a=abs(tau2)

         include 'evaluateJacobianDerivativesOrder6.h'
c           evalJacobianDerivatives(rsxy1,i1,i2,i3,a1j6,2,6,1)
c           evalJacobianDerivatives(rsxy1,i1,i2,i3,a1j4,2,4,3)
c           evalJacobianDerivatives(rsxy1,i1,i2,i3,a1j2,2,2,5)

c           evalJacobianDerivatives(rsxy2,j1,j2,j3,a2j6,2,6,1)
c           evalJacobianDerivatives(rsxy2,j1,j2,j3,a2j4,2,4,3)
c           evalJacobianDerivatives(rsxy2,j1,j2,j3,a2j2,2,2,5)


           ! evaluate the equations we want to solve using the current solution and assign f(i)
        include 'evaluateEquationsOrder6.h'

c       write(*,'(" uu1xxx4,uu1xyy4,vv1xxy4,vv1yyy4=",4e12.2)') uu1xxx4,uu1xyy4,vv1xxy4,vv1yyy4

c       write(*,'(" uu2xxx4,uu2xyy4,vv2xxy4,vv2yyy4=",4e12.2)') uu2xxx4,uu2xyy4,vv2xxy4,vv2yyy4

c           evaluateEquationsOrder6()

        
        if( option.eq.1 )then
          include 'evaluateCoefficientsOrder6.h'
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
