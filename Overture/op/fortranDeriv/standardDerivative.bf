c Define the standard finite difference derivatives 


c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "../src/defineDiffOrder2f.h"
#Include "../src/defineDiffOrder4f.h"
#Include "../src/defineDiffOrder6f.h"
#Include "../src/defineDiffOrder8f.h"


#beginMacro loops(e1)
  do c=ca,cb
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    e1
  end do
  end do
  end do
  end do
#endMacro


#beginMacro loops3(e1,e2,e3)
  do c=ca,cb
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    e1
    e2
    e3
  end do
  end do
  end do
  end do
#endMacro

c Note: no loop for c
#beginMacro loops3nc(e1,e2,e3)
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    e1
    e2
    e3
  end do
  end do
  end do
#endMacro


#beginMacro gridLoops(XX,DIMENSION)
if( order.eq.2 )then
 if( gridType .eq. rectangular )then
   loops(deriv(i1,i2,i3,c)=u ## XX ## 2 ## DIMENSION ## r(i1,i2,i3,c))
 else
   loops(deriv(i1,i2,i3,c)=u ## XX ## 2 ## DIMENSION(i1,i2,i3,c))
 endif 
else if( order.eq.4 )then
 if( gridType .eq. rectangular )then
   loops(deriv(i1,i2,i3,c)=u ## XX ## 4 ## DIMENSION ## r(i1,i2,i3,c))
 else
   loops(deriv(i1,i2,i3,c)=u ## XX ## 4 ## DIMENSION(i1,i2,i3,c))
 endif 
else if( order.eq.6 )then
 if( gridType .eq. rectangular )then
   loops(deriv(i1,i2,i3,c)=u ## XX ## 6 ## DIMENSION ## r(i1,i2,i3,c))
 else
   loops(deriv(i1,i2,i3,c)=u ## XX ## 6 ## DIMENSION(i1,i2,i3,c))
 endif 
else if( order.eq.8 )then
 if( gridType .eq. rectangular )then
   loops(deriv(i1,i2,i3,c)=u ## XX ## 8 ## DIMENSION ## r(i1,i2,i3,c))
 else
   loops(deriv(i1,i2,i3,c)=u ## XX ## 8 ## DIMENSION(i1,i2,i3,c))
 endif 
end if
#endMacro

c -- define the gradient --
#beginMacro gradLoop(ORDER)
 if( gridType .eq. rectangular )then
   if( nd.eq.2 )then
     loops3(deriv(i1,i2,i3,c      )=ux ## ORDER ## 2r(i1,i2,i3,c),\
            deriv(i1,i2,i3,c+ndc  )=uy ## ORDER ## 2r(i1,i2,i3,c),)
   else if( nd.eq.3 )then
     loops3(deriv(i1,i2,i3,c      )=ux ## ORDER ## 3r(i1,i2,i3,c),\
            deriv(i1,i2,i3,c+ndc  )=uy ## ORDER ## 3r(i1,i2,i3,c),\
            deriv(i1,i2,i3,c+ndc*2)=uz ## ORDER ## 3r(i1,i2,i3,c))
   else
     loops3(deriv(i1,i2,i3,c      )=ux ## ORDER ## 1r(i1,i2,i3,c),,)
   end if
 else
   if( nd.eq.2 )then
     loops3(deriv(i1,i2,i3,c      )=ux ## ORDER ## 2(i1,i2,i3,c),\
            deriv(i1,i2,i3,c+ndc  )=uy ## ORDER ## 2(i1,i2,i3,c),)
   else if( nd.eq.3 )then                   
     loops3(deriv(i1,i2,i3,c      )=ux ## ORDER ## 3(i1,i2,i3,c),\
            deriv(i1,i2,i3,c+ndc  )=uy ## ORDER ## 3(i1,i2,i3,c),\
            deriv(i1,i2,i3,c+ndc*2)=uz ## ORDER ## 3(i1,i2,i3,c))
   else                                     
     loops3(deriv(i1,i2,i3,c      )=ux ## ORDER ## 1(i1,i2,i3,c),,)
   end if
 endif 
#endMacro

c -- define the divergence --
#beginMacro divLoop(ORDER)
 if( gridType .eq. rectangular )then
   if( nd.eq.2 )then
     loops3nc(deriv(i1,i2,i3,0)=ux ## ORDER ## 2r(i1,i2,i3,ca  )+\
                                uy ## ORDER ## 2r(i1,i2,i3,ca+1),,)
   else if( nd.eq.3 )then
     loops3nc(deriv(i1,i2,i3,0)=ux ## ORDER ## 3r(i1,i2,i3,ca  )+\
                                uy ## ORDER ## 3r(i1,i2,i3,ca+1)+\
                                uz ## ORDER ## 3r(i1,i2,i3,ca+2),,)
   else
     loops3nc(deriv(i1,i2,i3,0)=ux ## ORDER ## 1r(i1,i2,i3,ca  ),,)
   end if
 else
   if( nd.eq.2 )then
     loops3nc(deriv(i1,i2,i3,0)=ux ## ORDER ## 2(i1,i2,i3,ca  )+\
                                uy ## ORDER ## 2(i1,i2,i3,ca+1),,)
   else if( nd.eq.3 )then                   
     loops3nc(deriv(i1,i2,i3,0)=ux ## ORDER ## 3(i1,i2,i3,ca  )+\
                                uy ## ORDER ## 3(i1,i2,i3,ca+1)+\
                                uz ## ORDER ## 3(i1,i2,i3,ca+2),,)
   else                                     
     loops3nc(deriv(i1,i2,i3,0)=ux ## ORDER ## 1(i1,i2,i3,ca  ),,)
   end if
 endif 
#endMacro

c -- define the vorticity --
#beginMacro vorticityLoop(ORDER)
c if( ca.ne.0 .or. cb.ne.0 )then
c   write(*,*) 'deriv of vorticity:ERROR ca,cb!=0'
c   stop 1
c end if
 if( gridType .eq. rectangular )then
   if( nd.eq.2 )then
     loops3nc(deriv(i1,i2,i3,0)=ux ## ORDER ## 2r(i1,i2,i3,1)-uy ## ORDER ## 2r(i1,i2,i3,0),,)
   else if( nd.eq.3 )then
     loops3nc(deriv(i1,i2,i3,0)=uy ## ORDER ## 3r(i1,i2,i3,2)-uz ## ORDER ## 3r(i1,i2,i3,1),\
              deriv(i1,i2,i3,1)=uz ## ORDER ## 3r(i1,i2,i3,0)-ux ## ORDER ## 3r(i1,i2,i3,2),\
              deriv(i1,i2,i3,2)=ux ## ORDER ## 3r(i1,i2,i3,1)-uy ## ORDER ## 3r(i1,i2,i3,0))
   end if
 else
   if( nd.eq.2 )then
     loops3nc(deriv(i1,i2,i3,0)=ux ## ORDER ## 2(i1,i2,i3,1)-uy ## ORDER ## 2(i1,i2,i3,0),,)
   else if( nd.eq.3 )then
     loops3nc(deriv(i1,i2,i3,0)=uy ## ORDER ## 3(i1,i2,i3,2)-uz ## ORDER ## 3(i1,i2,i3,1),\
              deriv(i1,i2,i3,1)=uz ## ORDER ## 3(i1,i2,i3,0)-ux ## ORDER ## 3(i1,i2,i3,2),\
              deriv(i1,i2,i3,2)=ux ## ORDER ## 3(i1,i2,i3,1)-uy ## ORDER ## 3(i1,i2,i3,0))
   end if
 endif 
#endMacro

#beginMacro finiteDifferenceDerivative(XX)
subroutine XX ## FiniteDiffDeriv( nd, \
    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, \
    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, \
    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, \
    ipar, rpar, \
    u,deriv, rsxy, mask )
c ===============================================================
c    Evaluate the difference approximation to a derivative
c
c nd1a,nd1b,nd2a,nd2b,nd3a,nd3b: dimensions for rsxy and mask
c ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b :  dimensions for u
c ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b :  dimensions for deriv
c rsxy : not used if rectangular
c
c ca,cb : assign components c=ca,..,cb (base 0)
c gridType: 0=rectangular, 1=non-rectangular
c order : order of accuracy
c dr(0:2), dx(0:2) 
c ===============================================================

 implicit none
 integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, \
  ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,\
  ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b

 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
 real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
 real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)
 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)

 real rpar(0:*)
 integer ipar(0:*)

c  ....local variables
 integer i1,i2,i3,kd,c,kdd,kd3,ndc
 real dr(0:2), dx(0:2)
 integer n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, gridType, order

 integer rectangular,curvilinear
 parameter( rectangular=0,curvilinear=1 )

 real rx,ry,rz,sx,sy,sz,tx,ty,tz

 include 'declareDiffOrder2f.h'
 include 'declareDiffOrder4f.h'
 include 'declareDiffOrder6f.h'
 include 'declareDiffOrder8f.h'

c.......statement functions 
c.......statement functions for jacobian
 rx(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
 ry(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
 rz(i1,i2,i3)=rsxy(i1,i2,i3,1,3)
 sx(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
 sy(i1,i2,i3)=rsxy(i1,i2,i3,2,2)
 sz(i1,i2,i3)=rsxy(i1,i2,i3,2,3)
 tx(i1,i2,i3)=rsxy(i1,i2,i3,3,1)
 ty(i1,i2,i3)=rsxy(i1,i2,i3,3,2)
 tz(i1,i2,i3)=rsxy(i1,i2,i3,3,3)

c     The next macro call will define the difference approximation statement functions
 defineDifferenceOrder2Components1(u,RX)
 defineDifferenceOrder4Components1(u,RX)
 defineDifferenceOrder6Components1(u,RX)
 defineDifferenceOrder8Components1(u,RX)

 n1a     =ipar(0)
 n1b     =ipar(1)
 n2a     =ipar(2)
 n2b     =ipar(3)
 n3a     =ipar(4)
 n3b     =ipar(5)
 ca      =ipar(6)
 cb      =ipar(7)
 gridType=ipar(8)
 order   =ipar(9)

 dr(0)=rpar(0)
 dr(1)=rpar(1)
 dr(2)=rpar(2)
 dx(0)=rpar(3)
 dx(1)=rpar(4)
 dx(2)=rpar(5)

 ndc=ndu4b-ndu4a+1 ! number of components

#If #XX == "grad" 
c --- gradient ----
  if( order.eq.2 )then
    gradLoop(2)
  else if( order.eq.4 )then
    gradLoop(4)
  else if( order.eq.6 )then
    gradLoop(6)
  else if( order.eq.8 )then
    gradLoop(8)
  end if

#Elif #XX == div
c  --- divergence ---
  if( order.eq.2 )then
    divLoop(2)
  else if( order.eq.4 )then
    divLoop(4)
  else if( order.eq.6 )then
    divLoop(6)
  else if( order.eq.8 )then
    divLoop(8)
  end if

#Elif #XX == vorticity
c  --- vorticity ---
  if( order.eq.2 )then
    vorticityLoop(2)
  else if( order.eq.4 )then
    vorticityLoop(4)
  else if( order.eq.6 )then
    vorticityLoop(6)
  else if( order.eq.8 )then
    vorticityLoop(8)
  end if


#Else

  if( nd .eq. 2 )then
    gridLoops(XX,2)
  else if( nd.eq.3 )then
    gridLoops(XX,3)
  else if( nd.eq.1 )then
    gridLoops(XX,1)
  else
  end if
#End

c if( nd .eq. 2 )then
cc       ******* 2D *************      
c   if( gridType .eq. rectangular )then
cc         rectangular
c     loops(deriv(i1,i2,i3,c)=u ## XX ## ORDER ## 2r(i1,i2,i3,c))
c   else
cc         ***** not rectangular *****
c     loops(deriv(i1,i2,i3,c)=u ## XX ## ORDER ## 2(i1,i2,i3,c))
c   endif 
c elseif( nd.eq.3 )then
cc  ******* 3D *************      
c   if( gridType .eq. rectangular )then
cc    rectangular
c     loops(deriv(i1,i2,i3,c)=u ## XX ## ORDER ## 3r(i1,i2,i3,c))
c   else
cc    ***** not rectangular *****
c     loops(deriv(i1,i2,i3,c)=u ## XX ## ORDER ## 3(i1,i2,i3,c))
c   endif 
c 
c elseif( nd.eq.1 )then
cc  ******* 1D *************      
c   if( gridType .eq. rectangular )then
cc    rectangular
c     loops(deriv(i1,i2,i3,c)=u ## XX ## ORDER ## 1r(i1,i2,i3,c))
c   else
cc    ***** not rectangular *****
c     loops(deriv(i1,i2,i3,c)=u ## XX ## ORDER ## 1(i1,i2,i3,c))
c   endif 
c 
c else if( nd.eq.0 )then
cc  *** add these lines to avoid warnings about unused statement functions
cc  include "cgux2afNoWarnings.h" 
cc  include "cgux4afNoWarnings.h" 
c end if

 return
 end
#endMacro



      finiteDifferenceDerivative(x)
      finiteDifferenceDerivative(y)
      finiteDifferenceDerivative(z)
      finiteDifferenceDerivative(xx)
      finiteDifferenceDerivative(xy)
      finiteDifferenceDerivative(xz)
      finiteDifferenceDerivative(yy)
      finiteDifferenceDerivative(yz)
      finiteDifferenceDerivative(zz)
      finiteDifferenceDerivative(laplacian)
      finiteDifferenceDerivative(grad)
      finiteDifferenceDerivative(div)
      finiteDifferenceDerivative(vorticity)

c
c      finiteDifferenceDerivative(laplacian)
c      finiteDifferenceDerivative(laplacian,4)
c      finiteDifferenceDerivative(x,8)
c      finiteDifferenceDerivative(y,8)
c      finiteDifferenceDerivative(z,8)
c      finiteDifferenceDerivative(xx,8)
c      finiteDifferenceDerivative(xy,8)
c      finiteDifferenceDerivative(xz,8)
c      finiteDifferenceDerivative(yy,8)
c      finiteDifferenceDerivative(yz,8)
c      finiteDifferenceDerivative(zz,8)
c      finiteDifferenceDerivative(laplacian,8)


