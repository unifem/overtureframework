c
c Compute du/dt for the incompressible NS on rectangular AND curvilinear grids
c


c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"


#beginMacro loopsMask5(e1,e2,e3,e4,e5)
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
   if( mask(i1,i2,i3).gt.0 )then
     e1
     e2
     e3
     e4
     e5
   end if
 end do
 end do
 end do
#endMacro

#beginMacro beginLoopsMask()
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
   if( mask(i1,i2,i3).gt.0 )then
#endMacro

#beginMacro endLoopsMask()
   end if
 end do
 end do
 end do
#endMacro



c Use this macro to compute the divergence and norms
c Optionally save the divergence by setting the argument to div(i1,i2,i3)
#beginMacro divAndNorms(div)
if( nd.eq.1 )then
    stop 1
else if( nd.eq.2 )then

 if( gridType.eq.rectangular )then

  if( orderOfAccuracy.eq.2 ) then
    loopsMask5(div=ux22r(i1,i2,i3,uc)+uy22r(i1,i2,i3,vc),\
               divMax=max(divMax,abs(div)),\
               vorMax=max(vorMax,abs(uy22r(i1,i2,i3,uc)-ux22r(i1,i2,i3,vc))),\
               divl2Norm=divl2Norm+div**2,numPoints=numPoints+1  )
  else ! order==4
    loopsMask5(div=ux42r(i1,i2,i3,uc)+uy42r(i1,i2,i3,vc),\
               divMax=max(divMax,abs(div)),\
               vorMax=max(vorMax,abs(uy42r(i1,i2,i3,uc)-ux42r(i1,i2,i3,vc))),\
               divl2Norm=divl2Norm+div**2,numPoints=numPoints+1 )
  end if

 else ! curvilinear

  if( orderOfAccuracy.eq.2 ) then
    loopsMask5(div=ux22(i1,i2,i3,uc)+uy22(i1,i2,i3,vc),\
               divMax=max(divMax,abs(div)),\
               vorMax=max(vorMax,abs(uy22(i1,i2,i3,uc)-ux22(i1,i2,i3,vc))),\
               divl2Norm=divl2Norm+div**2,numPoints=numPoints+1 )
  else ! order==4
    loopsMask5(div=ux42(i1,i2,i3,uc)+uy42(i1,i2,i3,vc),\
               divMax=max(divMax,abs(div)),\
               vorMax=max(vorMax,abs(uy42(i1,i2,i3,uc)-ux42(i1,i2,i3,vc))),\
               divl2Norm=divl2Norm+div**2,numPoints=numPoints+1 )
  end if


 end if

else ! nd==3

 if( gridType.eq.rectangular )then

  if( orderOfAccuracy.eq.2 ) then
    loopsMask5(div=ux23r(i1,i2,i3,uc)+uy23r(i1,i2,i3,vc)+uz23r(i1,i2,i3,wc),\
               divMax=max(divMax,abs(div)),\
               vorMax=max(vorMax,abs(uy23r(i1,i2,i3,uc)-ux23r(i1,i2,i3,vc)),\
                                 abs(uz23r(i1,i2,i3,vc)-uy23r(i1,i2,i3,wc)),\
                                 abs(ux23r(i1,i2,i3,wc)-uz23r(i1,i2,i3,uc))),\
               divl2Norm=divl2Norm+div**2,numPoints=numPoints+1 )
  else ! order==4
    loopsMask5(div=ux43r(i1,i2,i3,uc)+uy43r(i1,i2,i3,vc)+uz43r(i1,i2,i3,wc),\
               divMax=max(divMax,abs(div)),\
               vorMax=max(vorMax,abs(uy43r(i1,i2,i3,uc)-ux43r(i1,i2,i3,vc)),\
                                 abs(uz43r(i1,i2,i3,vc)-uy43r(i1,i2,i3,wc)),\
                                 abs(ux43r(i1,i2,i3,wc)-uz43r(i1,i2,i3,uc))),\
               divl2Norm=divl2Norm+div**2,numPoints=numPoints+1 )
  end if

 else ! curvilinear

  if( orderOfAccuracy.eq.2 ) then
    loopsMask5(div=ux23(i1,i2,i3,uc)+uy23(i1,i2,i3,vc)+uz23(i1,i2,i3,wc),\
               divMax=max(divMax,abs(div)),\
               vorMax=max(vorMax,abs(uy23(i1,i2,i3,uc)-ux23(i1,i2,i3,vc)),\
                                 abs(uz23(i1,i2,i3,vc)-uy23(i1,i2,i3,wc)),\
                                 abs(ux23(i1,i2,i3,wc)-uz23(i1,i2,i3,uc))),\
               divl2Norm=divl2Norm+div**2,numPoints=numPoints+1 )
  else ! order==4
    loopsMask5(div=ux43(i1,i2,i3,uc)+uy43(i1,i2,i3,vc)+uz43(i1,i2,i3,wc),\
               divMax=max(divMax,abs(div)),\
               vorMax=max(vorMax,abs(uy43(i1,i2,i3,uc)-ux43(i1,i2,i3,vc)),\
                                 abs(uz43(i1,i2,i3,vc)-uy43(i1,i2,i3,wc)),\
                                 abs(ux43(i1,i2,i3,wc)-uz43(i1,i2,i3,uc))),\
               divl2Norm=divl2Norm+div**2,numPoints=numPoints+1 )
  end if

 end if ! end curvilinear

end if ! end nd
#endMacro

c =============== Axisymmetric Case ==============================
c Use this macro to compute the divergence and norms
c Optionally save the divergence by setting the argument to div(i1,i2,i3)
#beginMacro divAndNormsAxisymmetric(div)
if( nd.eq.1 )then
    stop 1
else if( nd.eq.2 )then

 if( gridType.eq.rectangular )then

  if( orderOfAccuracy.eq.2 ) then
    beginLoopsMask()
     yy = yc(i2)
     if( abs(yy).gt.yEps )then
       div=ux22r(i1,i2,i3,uc)+uy22r(i1,i2,i3,vc)+u(i1,i2,i3,vc)/yy
     else
       div=ux22r(i1,i2,i3,uc)+2.*uy22r(i1,i2,i3,vc)
     end if
     divMax=max(divMax,abs(div))
     vorMax=max(vorMax,abs(uy22r(i1,i2,i3,uc)-ux22r(i1,i2,i3,vc)))
     divl2Norm=divl2Norm+div**2
     numPoints=numPoints+1
    endLoopsMask()
  else ! order==4
    beginLoopsMask()
     yy = yc(i2)
     if( abs(yy).gt.yEps )then
       div=ux42r(i1,i2,i3,uc)+uy42r(i1,i2,i3,vc)+u(i1,i2,i3,vc)/yy
     else
       div=ux42r(i1,i2,i3,uc)+2.*uy42r(i1,i2,i3,vc)
     end if
     divMax=max(divMax,abs(div))
     vorMax=max(vorMax,abs(uy42r(i1,i2,i3,uc)-ux42r(i1,i2,i3,vc)))
     divl2Norm=divl2Norm+div**2
     numPoints=numPoints+1
    endLoopsMask()
  end if

 else ! curvilinear

  if( orderOfAccuracy.eq.2 ) then
    beginLoopsMask()
     yy = xy(i1,i2,i3,1)
     if( abs(yy).gt.yEps )then
       div=ux22(i1,i2,i3,uc)+uy22(i1,i2,i3,vc)+u(i1,i2,i3,vc)/yy
     else
       div=ux22(i1,i2,i3,uc)+2.*uy22(i1,i2,i3,vc)
     end if
     divMax=max(divMax,abs(div))
     vorMax=max(vorMax,abs(uy22(i1,i2,i3,uc)-ux22(i1,i2,i3,vc)))
     divl2Norm=divl2Norm+div**2
     numPoints=numPoints+1
    endLoopsMask()
  else ! order==4
    beginLoopsMask()
     yy = xy(i1,i2,i3,1)
     if( abs(yy).gt.yEps )then
       div=ux42(i1,i2,i3,uc)+uy42(i1,i2,i3,vc)+u(i1,i2,i3,vc)/yy
     else
       div=ux42(i1,i2,i3,uc)+2.*uy42(i1,i2,i3,vc)
     end if
     divMax=max(divMax,abs(div))
     vorMax=max(vorMax,abs(uy42(i1,i2,i3,uc)-ux42(i1,i2,i3,vc)))
     divl2Norm=divl2Norm+div**2
     numPoints=numPoints+1
    endLoopsMask()
  end if

 end if

else ! nd==3

 stop 14532

end if ! end nd
#endMacro


      subroutine getDivAndNorms(nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & mask,xy,rsxy,  u,div,  ipar, rpar, ierr )
c======================================================================
c   *********** determine the divergence, divMax and vorMax ******************    
c
c nd : number of space dimensions
c
c Input:
c   option = ipar(8) 
c          = 0 : compute norms but do not save the divergence
c          = 1 : compute norms and save the divergence
c Output:
c  rpar(10)=divMax
c  rpar(11)=vorMax
c  rpar(12)=divl2Norm
c  ipar(10)=numPoints  : number of active points (mask>0)
c
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real div(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer ierr

      integer ipar(0:*)
      real rpar(0:*)
      

c     ---- local variables -----
      real divMax,vorMax,divTemp,divl2Norm
      real yy,yEps,xa,ya,za
      integer c,i1,i2,i3,orderOfAccuracy,gridIsMoving,numPoints
      integer option,isAxisymmetric,gridType
      integer pc,uc,vc,wc,grid,i1a,i2a,i3a

      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer m,n,kd,kdd,kd3,ndc
      real dr(0:2), dx(0:2)

c ---------------- start statement functions ----------------------
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real xc,yc,zc

      ! include 'declareDiffOrder2f.h'
      ! include 'declareDiffOrder4f.h'

      declareDifferenceOrder2(u,RX)
      declareDifferenceOrder4(u,RX)

c.......statement functions for jacobian
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)

      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

      ! for cartesian coordinates
      xc(i1) = xa + dx(0)*(i1-i1a)
      yc(i2) = ya + dx(1)*(i2-i2a)
      zc(i3) = za + dx(2)*(i3-i3a)
c ---------------- end statement functions ----------------------

c     The next macro call will define the difference approximation statement functions
      defineDifferenceOrder2Components1(u,RX)
      defineDifferenceOrder4Components1(u,RX)
c     --- end statement functions

      ierr=0
      ! write(*,*) 'Inside getDivAndNorms'

      pc                =ipar(0)
      uc                =ipar(1)
      vc                =ipar(2)
      wc                =ipar(3)
      grid              =ipar(4)
      orderOfAccuracy   =ipar(5)
      isAxisymmetric    =ipar(6)
      gridType          =ipar(7)
      option            =ipar(8)
      i1a               =ipar(9)
      i2a               =ipar(10)
      i3a               =ipar(11)

      dr(0)               =rpar(0)
      dr(1)               =rpar(1)
      dr(2)               =rpar(2)
      dx(0)               =rpar(3)
      dx(1)               =rpar(4)
      dx(2)               =rpar(5)
      xa                  =rpar(6)
      ya                  =rpar(7)
      za                  =rpar(8)
      yEps                =rpar(9) ! for axisymmetric y<yEps => y is on the axis

c     *********** determine the divergence, divMax and vorMax ******************    
      divMax=0.
      vorMax=0.
      divl2Norm=0.
      numPoints=0

      if( isAxisymmetric.eq.0 )then
       if( option.eq.0 )then
         ! compute norms but do not save the divergence
         divAndNorms(divTemp)
       else
        ! compute norms and save the divergence
         divAndNorms(div(i1,i2,i3))
       end if
      else
       ! axisymmetric
       if( option.eq.0 )then
         ! compute norms but do not save the divergence
         divAndNormsAxisymmetric(divTemp)
       else
        ! compute norms and save the divergence
         divAndNormsAxisymmetric(div(i1,i2,i3))
       end if
      end if

      rpar(10)=divMax
      rpar(11)=vorMax
      rpar(12)=divl2Norm
      ipar(10)=numPoints
      return 
      end



