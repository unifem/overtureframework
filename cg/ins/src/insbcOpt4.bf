!
! routines for applying fourth order boundary conditions
!

#Include "defineDiffOrder4f.h"
!!kkc#Include "defineDiffOrder2f.h"


#beginMacro loopse4(e1,e2,e3,e4)
 n1a=nr(0,0)
 n1b=nr(1,0)
 n2a=nr(0,1)
 n2b=nr(1,1)
 n3a=nr(0,2)
 n3b=nr(1,2)
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
   e1
   e2
   e3
   e4
  end if
 end do
 end do
 end do
#endMacro

#beginMacro loopse4NoMask(e1,e2,e3,e4)
 n1a=nr(0,0)
 n1b=nr(1,0)
 n2a=nr(0,1)
 n2b=nr(1,1)
 n3a=nr(0,2)
 n3b=nr(1,2)
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
     e1
     e2
     e3
     e4
 end do
 end do
 end do
#endMacro

! Extrapolate the normal component on the second ghost line
#beginMacro extrapNormal(DIM,rxi,ryi,rzi,is1,is2,is3)
 #If #DIM == "2"
   ! extrapolate the normal component:
   u2 = 4.*(rxi*u(i1-  is1,i2-  is2,i3,uc)+ryi*u(i1-  is1,i2-  is2,i3,vc))\
       -6.*(rxi*u(i1      ,i2      ,i3,uc)+ryi*u(i1      ,i2      ,i3,vc))\
       +4.*(rxi*u(i1+  is1,i2+  is2,i3,uc)+ryi*u(i1+  is1,i2+  is2,i3,vc))\
          -(rxi*u(i1+2*is1,i2+2*is2,i3,uc)+ryi*u(i1+2*is1,i2+2*is2,i3,vc))
   uDotN2=      u(i1-2*is1,i2-2*is2,i3,uc)*rxi+u(i1-2*is1,i2-2*is2,i3,vc)*ryi
 #Else
   ! extrapolate the normal component:
   u2 = 4.*(rxi*u(i1-  is1,i2-  is2,i3-  is3,uc)+ryi*u(i1-  is1,i2-  is2,i3-  is3,vc)+rzi*u(i1-  is1,i2-  is2,i3-  is3,wc))\
       -6.*(rxi*u(i1      ,i2      ,i3      ,uc)+ryi*u(i1      ,i2      ,i3      ,vc)+rzi*u(i1      ,i2      ,i3      ,wc))\
       +4.*(rxi*u(i1+  is1,i2+  is2,i3+  is3,uc)+ryi*u(i1+  is1,i2+  is2,i3+  is3,vc)+rzi*u(i1+  is1,i2+  is2,i3+  is3,wc))\
          -(rxi*u(i1+2*is1,i2+2*is2,i3+2*is3,uc)+ryi*u(i1+2*is1,i2+2*is2,i3+2*is3,vc)+rzi*u(i1+2*is1,i2+2*is2,i3+2*is3,wc))
   uDotN2=      u(i1-2*is1,i2-2*is2,i3-2*is3,uc)*rxi+u(i1-2*is1,i2-2*is2,i3-2*is3,vc)*ryi+u(i1-2*is1,i2-2*is2,i3-2*is3,wc)*rzi
   u(i1-2*is1,i2-2*is2,i3-2*is3,wc)=u(i1-2*is1,i2-2*is2,i3-2*is3,wc) + (u2-uDotN2)*rzi/rxsq
 #End

 u(i1-2*is1,i2-2*is2,i3-2*is3,uc)=u(i1-2*is1,i2-2*is2,i3-2*is3,uc) + (u2-uDotN2)*rxi/rxsq
 u(i1-2*is1,i2-2*is2,i3-2*is3,vc)=u(i1-2*is1,i2-2*is2,i3-2*is3,vc) + (u2-uDotN2)*ryi/rxsq
#endMacro

!   On the slip wall extended boundary points we solve for the normal components from:
!        u.x + v.y = 0
!       D+^p ( n.u ) = 0
! DIR = r,s,t
! DIM = 2,3
#beginMacro divAndExtrap(DIR,DIM)

 rxi = rx(i1,i2,i3)
 ryi = ry(i1,i2,i3)
 sxi = sx(i1,i2,i3)
 syi = sy(i1,i2,i3)
 #If #DIM == "3"
   rzi = rz(i1,i2,i3)
   szi = sz(i1,i2,i3)
   txi = tx(i1,i2,i3)
   tyi = ty(i1,i2,i3)
   tzi = tz(i1,i2,i3)
 #End

 ! solve for:   u1 = the new normal component of the velocity
 #If #DIR == "r" 
   #If #DIM == "2"
     rxsq=rxi**2+ryi**2
     ! Solve div(u)=0 and 4th-order extrapolationof the normal component ( rx*u+ry*v )
     u1 = -1.5*(rxi*u(i1     ,i2,i3,uc)+ryi*u(i1     ,i2,i3,vc) )\
           +3.*(rxi*u(i1+  is,i2,i3,uc)+ryi*u(i1+  is,i2,i3,vc))\
           -.5*(rxi*u(i1+2*is,i2,i3,uc)+ryi*u(i1+2*is,i2,i3,vc))\
  	 +is*.25*dr(0)*12.*( sxi*us4(i1,i2,i3,uc)+syi*us4(i1,i2,i3,vc) )
   #Else
     rxsq=rxi**2+ryi**2+rzi**2

     u1 = -1.5*(rxi*u(i1     ,i2,i3,uc)+ryi*u(i1     ,i2,i3,vc)+rzi*u(i1     ,i2,i3,wc) )\
           +3.*(rxi*u(i1+  is,i2,i3,uc)+ryi*u(i1+  is,i2,i3,vc)+rzi*u(i1+  is,i2,i3,wc))\
           -.5*(rxi*u(i1+2*is,i2,i3,uc)+ryi*u(i1+2*is,i2,i3,vc)+rzi*u(i1+2*is,i2,i3,wc))\
  	 +is*.25*dr(0)*12.*( sxi*us4(i1,i2,i3,uc)+syi*us4(i1,i2,i3,vc)+szi*us4(i1,i2,i3,wc)\
                            +txi*ut4(i1,i2,i3,uc)+tyi*ut4(i1,i2,i3,vc)+tzi*ut4(i1,i2,i3,wc) )
   #End
 #Elif #DIR == "s"
   #If #DIM == "2"
     rxsq=sxi**2+syi**2

     u1 = -1.5*(sxi*u(i1,i2     ,i3,uc)+syi*u(i1,i2     ,i3,vc) )\
           +3.*(sxi*u(i1,i2+  is,i3,uc)+syi*u(i1,i2+  is,i3,vc))\
           -.5*(sxi*u(i1,i2+2*is,i3,uc)+syi*u(i1,i2+2*is,i3,vc))\
  	 +is*.25*dr(1)*12.*( rxi*ur4(i1,i2,i3,uc)+ryi*ur4(i1,i2,i3,vc) )
   #Else
     rxsq=sxi**2+syi**2+szi**2

     u1 = -1.5*(sxi*u(i1,i2     ,i3,uc)+syi*u(i1,i2     ,i3,vc)+szi*u(i1,i2     ,i3,wc) )\
           +3.*(sxi*u(i1,i2+  is,i3,uc)+syi*u(i1,i2+  is,i3,vc)+szi*u(i1,i2+  is,i3,wc))\
           -.5*(sxi*u(i1,i2+2*is,i3,uc)+syi*u(i1,i2+2*is,i3,vc)+szi*u(i1,i2+2*is,i3,wc))\
  	 +is*.25*dr(1)*12.*( rxi*ur4(i1,i2,i3,uc)+ryi*ur4(i1,i2,i3,vc)+rzi*ur4(i1,i2,i3,wc)\
                            +txi*ut4(i1,i2,i3,uc)+tyi*ut4(i1,i2,i3,vc)+tzi*ut4(i1,i2,i3,wc) )
   #End
 #Elif #DIR == "t"
     rxsq=txi**2+tyi**2+tzi**2

     u1 = -1.5*(txi*u(i1,i2,i3     ,uc)+tyi*u(i1,i2,i3     ,vc)+tzi*u(i1,i2,i3     ,wc) )\
           +3.*(txi*u(i1,i2,i3+  is,uc)+tyi*u(i1,i2,i3+  is,vc)+tzi*u(i1,i2,i3+  is,wc))\
           -.5*(txi*u(i1,i2,i3+2*is,uc)+tyi*u(i1,i2,i3+2*is,vc)+tzi*u(i1,i2,i3+2*is,wc))\
  	 +is*.25*dr(2)*12.*( rxi*ur4(i1,i2,i3,uc)+ryi*ur4(i1,i2,i3,vc)+rzi*ur4(i1,i2,i3,wc)\
                            +sxi*us4(i1,i2,i3,uc)+syi*us4(i1,i2,i3,vc)+szi*us4(i1,i2,i3,wc) )
 #Else
   write(*,*) 'ERROR: unknown #DIR'
   stop 5
 #End

 ! write(*,'(''divExtrap:#DIR i1,i2,is,rxi,ryi='',i4,i4,i4,2e10.2)') i1,i2,is,rxi,ryi
 ! write(*,'(''  u,v,u1,un='',4e10.2)') u(i1-is,i2,i3,uc),u(i1-is,i2,i3,vc),u1,(u(i1-is,i2,i3,uc)*rxi+u(i1-is,i2,i3,vc)*ryi)

 ! Update the normal component only
 #If #DIR == "r" 
   #If #DIM == "2"
     uDotN1=u1-(u(i1-is,i2,i3,uc)*rxi+u(i1-is,i2,i3,vc)*ryi)
   #Else
     uDotN1=u1-(u(i1-is,i2,i3,uc)*rxi+u(i1-is,i2,i3,vc)*ryi+u(i1-is,i2,i3,wc)*rzi)
     u(i1-is,i2,i3,wc)=u(i1-is,i2,i3,wc) + uDotN1*rzi/rxsq
   #End

   u(i1-is,i2,i3,uc)=u(i1-is,i2,i3,uc) + uDotN1*rxi/rxsq
   u(i1-is,i2,i3,vc)=u(i1-is,i2,i3,vc) + uDotN1*ryi/rxsq
 #Elif #DIR == "s"
   #If #DIM == "2"
     uDotN1=u1-(u(i1,i2-is,i3,uc)*sxi+u(i1,i2-is,i3,vc)*syi)
   #Else
     uDotN1=u1-(u(i1,i2-is,i3,uc)*sxi+u(i1,i2-is,i3,vc)*syi+u(i1,i2-is,i3,wc)*szi)
     u(i1,i2-is,i3,wc)=u(i1,i2-is,i3,wc) + uDotN1*szi/rxsq
   #End

   u(i1,i2-is,i3,uc)=u(i1,i2-is,i3,uc) + uDotN1*sxi/rxsq
   u(i1,i2-is,i3,vc)=u(i1,i2-is,i3,vc) + uDotN1*syi/rxsq
 #Else
   uDotN1=u1-(u(i1,i2,i3-is,uc)*txi+u(i1,i2,i3-is,vc)*tyi+u(i1,i2,i3-is,wc)*tzi)
   u(i1,i2,i3-is,uc)=u(i1,i2,i3-is,uc) + uDotN1*txi/rxsq
   u(i1,i2,i3-is,vc)=u(i1,i2,i3-is,vc) + uDotN1*tyi/rxsq
   u(i1,i2,i3-is,wc)=u(i1,i2,i3-is,wc) + uDotN1*tzi/rxsq
 #End

 ! Assign the value on the second ghost line by extrapolation
  #If #DIR == "r" 
   extrapNormal(DIM,rxi,ryi,rzi,is,0,0)
 #Elif #DIR == "s"
   extrapNormal(DIM,sxi,syi,szi,0,is,0)
 #Else
   extrapNormal(DIM,txi,tyi,tzi,0,0,is)
 #End       


#endMacro

! Determine the tangential components of the velocity from 
!                D+D-
!                D+^6( tv.uv ) = o
!  **curvilinear grid case ***
#beginMacro boundaryCondition2ndDifferenceAndExtrap(is1,is2,is3,DIR,FORCING,DIM)

 u1=2.*u(i1,i2,i3,uc)-u(i1+is1,i2+is2,i3+is3,uc)
 v1=2.*u(i1,i2,i3,vc)-u(i1+is1,i2+is2,i3+is3,vc)

 rxi = DIR ## x(i1,i2,i3)
 ryi = DIR ## y(i1,i2,i3)
 #If #DIM == "2"
  rxsq=rxi**2+ryi**2

  #If #FORCING == "tz"
    u1=u1+ogf(exact,x(i1-is1,i2-is2,i3-is3,0),x(i1-is1,i2-is2,i3-is3,1),0.,uc,t)\
      -2.*ogf(exact,x(i1    ,i2    ,i3    ,0),x(i1    ,i2    ,i3    ,1),0.,uc,t)\
         +ogf(exact,x(i1+is1,i2+is2,i3+is3,0),x(i1+is1,i2+is2,i3+is3,1),0.,uc,t) 

    v1=v1+ogf(exact,x(i1-is1,i2-is2,i3-is3,0),x(i1-is1,i2-is2,i3-is3,1),0.,vc,t)\
      -2.*ogf(exact,x(i1    ,i2    ,i3    ,0),x(i1    ,i2    ,i3    ,1),0.,vc,t)\
         +ogf(exact,x(i1+is1,i2+is2,i3+is3,0),x(i1+is1,i2+is2,i3+is3,1),0.,vc,t) 

  #Elif #FORCING == "none"
  #Else
    write(*,*) 'ERROR'
    stop 9
  #End 
  uDotN1=( (u(i1-is1,i2-is2,i3-is3,uc)-u1)*rxi \
         + (u(i1-is1,i2-is2,i3-is3,vc)-v1)*ryi )/rxsq
 #Else
  rzi = DIR ## z(i1,i2,i3)
  rxsq=rxi**2+ryi**2+rzi**2

  w1=2.*u(i1,i2,i3,wc)-u(i1+is1,i2+is2,i3+is3,wc)

  #If #FORCING == "tz"
    u1=u1+ogf(exact,x(i1-is1,i2-is2,i3-is3,0),x(i1-is1,i2-is2,i3-is3,1),x(i1-is1,i2-is2,i3-is3,2),uc,t)\
      -2.*ogf(exact,x(i1    ,i2    ,i3    ,0),x(i1    ,i2    ,i3    ,1),x(i1    ,i2    ,i3    ,2),uc,t)\
         +ogf(exact,x(i1+is1,i2+is2,i3+is3,0),x(i1+is1,i2+is2,i3+is3,1),x(i1+is1,i2+is2,i3+is3,2),uc,t) 
    v1=v1+ogf(exact,x(i1-is1,i2-is2,i3-is3,0),x(i1-is1,i2-is2,i3-is3,1),x(i1-is1,i2-is2,i3-is3,2),vc,t)\
      -2.*ogf(exact,x(i1    ,i2    ,i3    ,0),x(i1    ,i2    ,i3    ,1),x(i1    ,i2    ,i3    ,2),vc,t)\
         +ogf(exact,x(i1+is1,i2+is2,i3+is3,0),x(i1+is1,i2+is2,i3+is3,1),x(i1+is1,i2+is2,i3+is3,2),vc,t) 
    w1=w1+ogf(exact,x(i1-is1,i2-is2,i3-is3,0),x(i1-is1,i2-is2,i3-is3,1),x(i1-is1,i2-is2,i3-is3,2),wc,t)\
      -2.*ogf(exact,x(i1    ,i2    ,i3    ,0),x(i1    ,i2    ,i3    ,1),x(i1    ,i2    ,i3    ,2),wc,t)\
         +ogf(exact,x(i1+is1,i2+is2,i3+is3,0),x(i1+is1,i2+is2,i3+is3,1),x(i1+is1,i2+is2,i3+is3,2),wc,t) 

  #End 

  uDotN1=( (u(i1-is1,i2-is2,i3-is3,uc)-u1)*rxi \
         + (u(i1-is1,i2-is2,i3-is3,vc)-v1)*ryi \
         + (u(i1-is1,i2-is2,i3-is3,wc)-w1)*rzi )/rxsq
 #End

 u(i1-is1,i2-is2,i3-is3,uc)=u1 + uDotN1*rxi 
 u(i1-is1,i2-is2,i3-is3,vc)=v1 + uDotN1*ryi 
 #If #DIM == "3"
   u(i1-  is1,i2-  is2,i3-  is3,wc)=w1 + uDotN1*rzi 
 #End

 ! now compute 2nd ghost line value given the first
 u2=  6.*u(i1-is1,i2-is2,i3-is3,uc)-15.*u(i1,i2,i3,uc)+20.*u(i1+is1,i2+is2,i3+is3,uc)\
    -15.*u(i1+2*is1,i2+2*is2,i3+2*is3,uc)+6.*u(i1+3*is1,i2+3*is2,i3+3*is3,uc)-u(i1+4*is1,i2+4*is2,i3+4*is3,uc)
 v2=  6.*u(i1-is1,i2-is2,i3-is3,vc)-15.*u(i1,i2,i3,vc)+20.*u(i1+is1,i2+is2,i3+is3,vc)\
    -15.*u(i1+2*is1,i2+2*is2,i3+2*is3,vc)+6.*u(i1+3*is1,i2+3*is2,i3+3*is3,vc)-u(i1+4*is1,i2+4*is2,i3+4*is3,vc)

 #If #DIM == "2"
  uDotN2=( (u(i1-2*is1,i2-2*is2,i3-2*is3,uc)-u2)*rxi \
         + (u(i1-2*is1,i2-2*is2,i3-2*is3,vc)-v2)*ryi )/rxsq
 #Else
  w2=  6.*u(i1-  is1,i2-  is2,i3-  is3,wc)-15.*u(i1,i2,i3,wc)+20.*u(i1+is1,i2+is2,i3+is3,wc)\
     -15.*u(i1+2*is1,i2+2*is2,i3+2*is3,wc) +6.*u(i1+3*is1,i2+3*is2,i3+3*is3,wc)-u(i1+4*is1,i2+4*is2,i3+4*is3,wc)
  uDotN2=( (u(i1-2*is1,i2-2*is2,i3-2*is3,uc)-u2)*rxi \
         + (u(i1-2*is1,i2-2*is2,i3-2*is3,vc)-v2)*ryi \
         + (u(i1-2*is1,i2-2*is2,i3-2*is3,wc)-w2)*rzi )/rxsq
   u(i1-2*is1,i2-2*is2,i3-2*is3,wc)=w2 + uDotN2*rzi
 #End

 u(i1-2*is1,i2-2*is2,i3-2*is3,uc)=u2 + uDotN2*rxi
 u(i1-2*is1,i2-2*is2,i3-2*is3,vc)=v2 + uDotN2*ryi

#endMacro

! ==========================================================================================================
! Determine the tangential components of the velocity from 
!                D+D-
!                D+^6( tv.uv ) = o
! ==========================================================================================================
#beginMacro boundaryCondition2ndDifferenceAndExtrapRectangular(DIR,FORCING,DIM)

 #If #FORCING == "tz"
 #Elif #FORCING == "none"
 #Else
    write(*,*) 'ERROR'
    stop 9
 #End
 #If #DIR == "r"
   u(i1-  is,i2,i3,vc)=2.*u(i1   ,i2,i3,vc)-u(i1+is,i2,i3,vc)
   #If #DIM == "2"
     #If #FORCING == "tz"
       u(i1-is,i2,i3,vc)=u(i1-is,i2,i3,vc)\
                    +ogf(exact,x(i1-is,i2,i3,0),x(i1-is,i2,i3,1),0.,vc,t)\
                 -2.*ogf(exact,x(i1   ,i2,i3,0),x(i1   ,i2,i3,1),0.,vc,t)\
                    +ogf(exact,x(i1+is,i2,i3,0),x(i1+is,i2,i3,1),0.,vc,t)
     #End
   #Else
     u(i1-  is,i2,i3,wc)=2.*u(i1   ,i2,i3,wc)-u(i1+is,i2,i3,wc)
     #If #FORCING == "tz"
       u(i1-is,i2,i3,vc)=u(i1-is,i2,i3,vc)\
                    +ogf(exact,x(i1-is,i2,i3,0),x(i1-is,i2,i3,1),x(i1-is,i2,i3,2),vc,t)\
                 -2.*ogf(exact,x(i1   ,i2,i3,0),x(i1   ,i2,i3,1),x(i1   ,i2,i3,2),vc,t)\
                    +ogf(exact,x(i1+is,i2,i3,0),x(i1+is,i2,i3,1),x(i1+is,i2,i3,2),vc,t)
       u(i1-is,i2,i3,wc)=u(i1-is,i2,i3,wc)\
                    +ogf(exact,x(i1-is,i2,i3,0),x(i1-is,i2,i3,1),x(i1-is,i2,i3,2),wc,t)\
                 -2.*ogf(exact,x(i1   ,i2,i3,0),x(i1   ,i2,i3,1),x(i1   ,i2,i3,2),wc,t)\
                    +ogf(exact,x(i1+is,i2,i3,0),x(i1+is,i2,i3,1),x(i1+is,i2,i3,2),wc,t)
     #End
     u(i1-2*is,i2,i3,wc)=6.*u(i1-is,i2,i3,wc)-15.*u(i1,i2,i3,wc)+20.*u(i1+is,i2,i3,wc)-15.*u(i1+2*is,i2,i3,wc)\
                        +6.*u(i1+3*is,i2,i3,wc)-u(i1+4*is,i2,i3,wc)
   #End
   u(i1-2*is,i2,i3,vc)=6.*u(i1-is,i2,i3,vc)-15.*u(i1,i2,i3,vc)+20.*u(i1+is,i2,i3,vc)-15.*u(i1+2*is,i2,i3,vc)\
                      +6.*u(i1+3*is,i2,i3,vc)-u(i1+4*is,i2,i3,vc)

!    write(*,*) 'outflow i1,i2=',i1,i2
!    u(i1-2*is,i2,i3,vc)=ogf(exact,x(i1-2*is,i2,i3,0),x(i1-2*is,i2,i3,1),x(i1-2*is,i2,i3,2),vc,t)

!    write(*,*) 'outflow i1,i2,v1,v2=',i1,i2,u(i1-  is,i2,i3,vc),u(i1-2*is,i2,i3,vc)
!    write(*,*) 'outflow i1,i2,ev1,ev2=',i1,i2,u(i1-  is,i2,i3,vc)-ogf(exact,x(i1-is,i2,i3,0),x(i1-is,i2,i3,1),x(i1-is,i2,i3,2),vc,t),u(i1-2*is,i2,i3,vc)-ogf(exact,x(i1-2*is,i2,i3,0),x(i1-2*is,i2,i3,1),x(i1-2*is,i2,i3,2),vc,t)
!  u(i1-2*is,i2,i3,vc)=5.*u(i1-is,i2,i3,vc)-10.*u(i1,i2,i3,vc)+10.*u(i1+is,i2,i3,vc)-5.*u(i1+2*is,i2,i3,vc)\
!                     +u(i1+3*is,i2,i3,vc)
 #Elif #DIR == "s" 
   u(i1,i2-  is,i3,uc)=2.*u(i1,i2   ,i3,uc)-u(i1,i2+is,i3,uc)
   #If #DIM == "2"
     #If #FORCING == "tz"
       u(i1,i2-is,i3,uc)=      u(i1,i2-is,i3,uc)\
                    +ogf(exact,x(i1,i2-is,i3,0),x(i1,i2-is,i3,1),0.,uc,t)\
                 -2.*ogf(exact,x(i1,i2   ,i3,0),x(i1,i2   ,i3,1),0.,uc,t)\
                    +ogf(exact,x(i1,i2+is,i3,0),x(i1,i2+is,i3,1),0.,uc,t)
     #End
   #Else
     u(i1,i2-  is,i3,wc)=2.*u(i1,i2   ,i3,wc)-u(i1,i2+is,i3,wc)
     #If #FORCING == "tz"
       u(i1,i2-is,i3,uc)=      u(i1,i2-is,i3,uc)\
                    +ogf(exact,x(i1,i2-is,i3,0),x(i1,i2-is,i3,1),x(i1,i2-is,i3,2),uc,t)\
                 -2.*ogf(exact,x(i1,i2   ,i3,0),x(i1,i2   ,i3,1),x(i1,i2   ,i3,2),uc,t)\
                    +ogf(exact,x(i1,i2+is,i3,0),x(i1,i2+is,i3,1),x(i1,i2+is,i3,2),uc,t)
       u(i1,i2-is,i3,wc)=      u(i1,i2-is,i3,wc)\
                    +ogf(exact,x(i1,i2-is,i3,0),x(i1,i2-is,i3,1),x(i1,i2-is,i3,2),wc,t)\
                 -2.*ogf(exact,x(i1,i2   ,i3,0),x(i1,i2   ,i3,1),x(i1,i2   ,i3,2),wc,t)\
                    +ogf(exact,x(i1,i2+is,i3,0),x(i1,i2+is,i3,1),x(i1,i2+is,i3,2),wc,t)
     #End
     u(i1,i2-2*is,i3,wc)=6.*u(i1,i2-is,i3,wc)-15.*u(i1,i2,i3,wc)+20.*u(i1,i2+is,i3,wc)-15.*u(i1,i2+2*is,i3,wc)\
                        +6.*u(i1,i2+3*is,i3,wc)-u(i1,i2+4*is,i3,wc)
   #End
   u(i1,i2-2*is,i3,uc)=6.*u(i1,i2-is,i3,uc)-15.*u(i1,i2,i3,uc)+20.*u(i1,i2+is,i3,uc)-15.*u(i1,i2+2*is,i3,uc)\
                      +6.*u(i1,i2+3*is,i3,uc)-u(i1,i2+4*is,i3,uc)
 #Else
   u(i1,i2,i3-  is,uc)=2.*u(i1   ,i2,i3,uc)-u(i1,i2,i3+is,uc)
   u(i1,i2,i3-  is,vc)=2.*u(i1   ,i2,i3,vc)-u(i1,i2,i3+is,vc)
   #If #FORCING == "tz"
     u(i1,i2,i3-is,uc)=      u(i1,i2,i3-is,uc)\
                  +ogf(exact,x(i1,i2,i3-is,0),x(i1,i2,i3-is,1),x(i1,i2,i3-is,2),uc,t)\
               -2.*ogf(exact,x(i1,i2,i3   ,0),x(i1,i2,i3   ,1),x(i1,i2,i3   ,2),uc,t)\
                  +ogf(exact,x(i1,i2,i3+is,0),x(i1,i2,i3+is,1),x(i1,i2,i3+is,2),uc,t)
     u(i1,i2,i3-is,vc)=      u(i1,i2,i3-is,vc) \
                  +ogf(exact,x(i1,i2,i3-is,0),x(i1,i2,i3-is,1),x(i1,i2,i3-is,2),vc,t)\
               -2.*ogf(exact,x(i1,i2,i3   ,0),x(i1,i2,i3   ,1),x(i1,i2,i3   ,2),vc,t)\
                  +ogf(exact,x(i1,i2,i3+is,0),x(i1,i2,i3+is,1),x(i1,i2,i3+is,2),vc,t)
   #End
   u(i1,i2,i3-2*is,uc)=6.*u(i1,i2,i3-is,uc)-15.*u(i1,i2,i3,uc)+20.*u(i1,i2,i3+is,uc)-15.*u(i1,i2,i3+2*is,uc)\
                      +6.*u(i1,i2,i3+3*is,uc)-u(i1,i2,i3+4*is,uc)
   u(i1,i2,i3-2*is,vc)=6.*u(i1,i2,i3-is,vc)-15.*u(i1,i2,i3,vc)+20.*u(i1,i2,i3+is,vc)-15.*u(i1,i2,i3+2*is,vc)\
                      +6.*u(i1,i2,i3+3*is,vc)-u(i1,i2,i3+4*is,vc)
 #End 

#endMacro

! ==========================================================================================================
! Limit the ghost values:
!   Compare 2 extrapolated approximations to the ghost values. If these do not agree,
!   then limit the ghost value to a lower order extrapolation.
! ==========================================================================================================
#beginMacro limitGhostVelocity( u1,u2,uc,DIR )
 ! extrap to 2nd and 3rd order
 #If #DIR == "r"
   u1 = u(i1-is,i2,i3,uc)
   uExtrap2 = 2.*u(i1,i2,i3,uc)-u(i1+is,i2,i3,uc)
   uExtrap3 = 3.*u(i1,i2,i3,uc)-3.*u(i1+is,i2,i3,uc)+u(i1+2*is,i2,i3,uc)
   ! uLim = u(i1+is,i2,i3,uc)
   ! uLim = uExtrap2
   uLim=u(i1,i2,i3,uc)
 #Elif #DIR == "s"
   u1 = u(i1,i2-is,i3,uc)
   uExtrap2 = 2.*u(i1,i2,i3,uc)-u(i1,i2+is,i3,uc)
   uExtrap3 = 3.*u(i1,i2,i3,uc)-3.*u(i1,i2+is,i3,uc)+u(i1,i2+2*is,i3,uc)
   ! uLim=u(i1,i2+is,i3,uc)
   ! uLim=uExtrap2
   uLim=u(i1,i2,i3,uc)
 #Else
   stop 4411
 #End
 ! alpha = O(h^2) for smooth solution
 alpha = min( 1., clim*abs(uExtrap2-uExtrap3)/( abs(uExtrap2) + abs(uExtrap3) + epsu ) )

 u1a=u1
 u1 = (1.-alpha)*u1 + alpha*uLim

 #If #DIR == "r"
   u(i1-  is,i2,i3,uc)=u1
 #Elif #DIR == "s"
   u(i1,i2-  is,i3,uc)=u1
 #Else
   stop 4411
 #End

 ! -- limit 2nd ghost line 
 #If #DIR == "r"
   u2 = u(i1-2*is,i2,i3,uc)
   uExtrap2 = 2.*u(i1,i2,i3,uc)-u(i1+2*is,i2,i3,uc)
   uExtrap3 = 3.*u(i1-is,i2,i3,uc)-3.*u(i1,i2,i3,uc)+u(i1+is,i2,i3,uc)
   ! uLim = u(i1+2*is,i2,i3,uc)
   ! uLim = uExtrap2
   uLim=u(i1,i2,i3,uc)

 #Elif #DIR == "s"
   u2 = u(i1,i2-2*is,i3,uc)
   uExtrap2 = 2.*u(i1,i2,i3,uc)-u(i1,i2+2*is,i3,uc)
   uExtrap3 = 3.*u(i1,i2-is,i3,uc)-3.*u(i1,i2,i3,uc)+u(i1,i2+is,i3,uc)
   ! uLim = u(i1,i2+2*is,i3,uc)
   ! uLim = uExtrap2
   uLim=u(i1,i2,i3,uc)

 #Else
   stop 4412
 #End
 alpha = min( 1., clim*abs(uExtrap2-uExtrap3)/( abs(uExtrap2) + abs(uExtrap3) + epsu ) )
 u2a=u2
 u2 = (1.-alpha)*u2 + alpha*uLim

 write(*,'(" (i1,i2)=(",i3,",",i3,") u1,u1Lim=",2f6.2," u2,u2Lim=",2f6.2)') i1,i2,u1a,u1,u2a,u2

 #If #DIR == "r"
   u(i1-2*is,i2,i3,uc)=u2
 #Elif #DIR == "s"
   u(i1,i2-2*is,i3,uc)=u2
 #Else
   stop 4411
 #End


#endMacro

! ==========================================================================================================
! Determine the tangential components of the velocity from the NS equations and extrapolation : 2D version
!
!  We assume the equations for the tangential components decouple (which they do except for cross terms
!   on non-orthogonal grids) -- We first solve for all components of uv and then just set the tangential ones.
! ==========================================================================================================
#beginMacro boundaryConditionNavierStokesAndExtrap2d(DIR,FORCING)
 rxi = DIR ## x(i1,i2,i3)
 ryi = DIR ## y(i1,i2,i3)
 rxsq=rxi**2+ryi**2
 rxxi=DIR ## xx42(i1,i2,i3)
 ryyi=DIR ## yy42(i1,i2,i3)

 ! Include artificial dissipation terms *wdh* 100817 
 u0 = u(i1,i2,i3,uc)
 v0 = u(i1,i2,i3,vc)
 if( gridIsMoving.eq.0 )then
  ! grid is NOT moving
  ug0 = u0
  vg0 = v0
  gtt0 = 0.
  gtt1 = 0.
 else
  ! grid is moving
  !  ug0 = u - gridVelocity
  !  gtt0 = grid acceleration = u.t 
  ug0 = u0-gv(i1,i2,i3,0)
  vg0 = v0-gv(i1,i2,i3,1)
  gtt0 = gtt(i1,i2,i3,0)
  gtt1 = gtt(i1,i2,i3,1)
 end if
 ux0 = ux42(i1,i2,i3,uc)  
 uy0 = uy42(i1,i2,i3,uc)
 vx0 = ux42(i1,i2,i3,vc)
 vy0 = uy42(i1,i2,i3,vc)

!!kkc ux0 = ux22(i1,i2,i3,uc)  
!!kkc uy0 = uy22(i1,i2,i3,uc)
!!kkc vx0 = ux22(i1,i2,i3,vc)
!!kkc vy0 = uy22(i1,i2,i3,vc)

 if( use4thOrderAD.ne.0 )then
   adCoeff4 = ad41+cd42*( abs(ux0)+abs(uy0)+abs(vx0)+abs(vy0) )

   ! try this 
   ! uxa = ux22(i1,i2,i3,uc)  
   ! uya = uy22(i1,i2,i3,uc)
   ! vxa = ux22(i1,i2,i3,vc)
   ! vya = uy22(i1,i2,i3,vc)
   ! adCoeff4 = ad41+cd42*( abs(uxa)+abs(uya)+abs(vxa)+abs(vya) )
   ! write(*,'("insbc4:tan: (i1,i2)=",2i3," adCoeff4=",e9.3)') i1,i2,adCoeff4
   ! write(*,'("          : ux0,uy0,vx0,vy0=",4e9.2)') ux0,uy0,vx0,vy0 
   ! write(*,'("          : uxa,uya,vxa,vya=",4e9.2)') uxa,uya,vxa,vya 
 end if
 if( use2ndOrderAD.ne.0 )then
   adCoeff2 = ad21+cd22*( abs(ux0)+abs(uy0)+abs(vx0)+abs(vy0) )
   ! try this 
   ! uxa = ux22(i1,i2,i3,uc)  
   ! uya = uy22(i1,i2,i3,uc)
   ! vxa = ux22(i1,i2,i3,vc)
   ! vya = uy22(i1,i2,i3,vc)
   ! adCoeff2 = ad21+cd22*( abs(uxa)+abs(uya)+abs(vxa)+abs(vya) )
 end if

 #If #DIR == "r"
   !   uDotN1=( u(i1-  is,i2,i3,uc)*rxi + u(i1-  is,i2,i3,vc)*ryi )/rxsq
   !   uDotN2=( u(i1-2*is,i2,i3,uc)*rxi + u(i1-2*is,i2,i3,vc)*ryi )/rxsq

   ! *** set tangential components to zero for testing ****
   !   u(i1-  is,i2,i3,uc)=uDotN1*rxi
   !   u(i1-  is,i2,i3,vc)=uDotN1*ryi
   !   u(i1-2*is,i2,i3,uc)=uDotN2*rxi
   !   u(i1-2*is,i2,i3,vc)=uDotN2*ryi
 #Else
   !   uDotN1=( u(i1,i2-  is,i3,uc)*rxi + u(i1,i2-  is,i3,vc)*ryi )/rxsq
   !   uDotN2=( u(i1,i2-2*is,i3,uc)*rxi + u(i1,i2-2*is,i3,vc)*ryi )/rxsq

   ! *** set tangential components to zero for testing ****
   ! write(*,*) 's: rxi,ryi',rxi,ryi
   ! write(*,*) 's:start: v,vn=',u(i1,i2-  is,i3,vc),uDotN1*ryi
   !   u(i1,i2-  is,i3,uc)=uDotN1*rxi
   !   u(i1,i2-  is,i3,vc)=uDotN1*ryi
    !  u(i1,i2-2*is,i3,uc)=uDotN2*rxi
    !  u(i1,i2-2*is,i3,vc)=uDotN2*ryi
 #End
 ! ********************************************************

 ! There can be trouble here if nu is too small and the second term gets too big -- errors may grow
 ! a11 = nu*rxsq*(16.)/(12.*dr(axis)**2)+is*(nu*(rxxi+ryyi)-(u(i1,i2,i3,uc)*rxi+u(i1,i2,i3,vc)*ryi))*(-8.)/(12.*dr(axis))
 ! a12 = nu*rxsq*(-1.)/(12.*dr(axis)**2)+is*(nu*(rxxi+ryyi)-(u(i1,i2,i3,uc)*rxi+u(i1,i2,i3,vc)*ryi))*( 1.)/(12.*dr(axis))

 ! a11, a12 = coefficients of u(-1) and u(-2) in f1u or f1v below

 a11 = nu*rxsq*(16.)/(12.*dr(axis)**2)+is*(nu*(rxxi+ryyi))*(-8.)/(12.*dr(axis)) + adCoeff4*4. + adCoeff2
 a12 = nu*rxsq*(-1.)/(12.*dr(axis)**2)+is*(nu*(rxxi+ryyi))*( 1.)/(12.*dr(axis)) - adCoeff4
 a21 = -6.
 a22 = 1.
 det=a11*a22-a12*a21

 ! write(*,*) 'insbc4:tan:i=',i1,i2,i3
 ! write(*,*) 'f1u,f1v=',f1u,f1v
 ! corrections added for moving grids (*wdh* 111124)
 f1u=nu*ulaplacian42(i1,i2,i3,uc)-ug0*ux0-vg0*uy0-ux42(i1,i2,i3,pc) +adCoeff4*delta42(i1,i2,i3,uc) +adCoeff2*delta22(i1,i2,i3,uc) -gtt0
 f1v=nu*ulaplacian42(i1,i2,i3,vc)-ug0*vx0-vg0*vy0-uy42(i1,i2,i3,pc) +adCoeff4*delta42(i1,i2,i3,vc) +adCoeff2*delta22(i1,i2,i3,vc) -gtt1
 #If #FORCING == "tz" 
   ! write(*,'(" insbc4: add TZ new way C2D")') 
   call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,uc,ue)
   call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,vc,ve)

   call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,pc,pxe)
   call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,pc,pye)

   call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,uc,uxe)
   call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,uc,uye)
   call ogDeriv(exact,0,2,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,uc,uxxe)
   call ogDeriv(exact,0,0,2,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,uc,uyye)

   call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,vc,vxe)
   call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,vc,vye)
   call ogDeriv(exact,0,2,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,vc,vxxe)
   call ogDeriv(exact,0,0,2,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,vc,vyye)
   if( gridIsMoving.eq.1 )then
    ue = ue - gv(i1,i2,i3,0)
    ve = ve - gv(i1,i2,i3,1)
   end if
   ! Note: do not add ute, vte
   f1u=f1u -nu*(uxxe+uyye) + ue*uxe + ve*uye + pxe + gtt0 
   f1v=f1v -nu*(vxxe+vyye) + ue*vxe + ve*vye + pye + gtt1 
 #End

 ! OLD: 111127
 ! #If #FORCING == "tz" 
 !  f1u=nu*ulaplacian42(i1,i2,i3,uc)-ux42(i1,i2,i3,pc) +adCoeff4*delta42(i1,i2,i3,uc) 
 !  f1v=nu*ulaplacian42(i1,i2,i3,vc)-uy42(i1,i2,i3,pc) +adCoeff4*delta42(i1,i2,i3,vc) 
 !  f1u=f1u+insbfu2d(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,nu,pc,uc,vc)
 !  f1v=f1v+insbfv2d(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,nu,pc,uc,vc)
 !  ! write(*,*) 'tz:f1u,f1v,f2u,f2v=',f1u,f1v,f2u,f2v
 ! #Else
 !  ! corrections added for moving grids (*wdh* 111124)
 !  f1u=nu*ulaplacian42(i1,i2,i3,uc)-ug0*ux0-vg0*uy0-ux42(i1,i2,i3,pc) +adCoeff4*delta42(i1,i2,i3,uc) -gtt0
 !  f1v=nu*ulaplacian42(i1,i2,i3,vc)-ug0*vx0-vg0*vy0-uy42(i1,i2,i3,pc) +adCoeff4*delta42(i1,i2,i3,vc) -gtt1
 ! #End

 if( assignTemperature.ne.0 )then
  ! *wdh* 110311 - include Boussinesq terms
  f1u=f1u-thermalExpansivity*gravity(0)*u(i1,i2,i3,tc)
  f1v=f1v-thermalExpansivity*gravity(1)*u(i1,i2,i3,tc)
  #If #FORCING == "tz"
    te = ogf(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,tc,t)
    f1u=f1u+thermalExpansivity*gravity(0)*te
    f1v=f1v+thermalExpansivity*gravity(1)*te
  #End
 end if

 #If #DIR == "r"
   f2u=u(i1-2*is,i2,i3,uc)-6.*u(i1-is,i2,i3,uc)+15.*u(i1,i2,i3,uc)-20.*u(i1+is,i2,i3,uc)+\
                          15.*u(i1+2*is,i2,i3,uc)-6.*u(i1+3*is,i2,i3,uc)+u(i1+4*is,i2,i3,uc)
   f2v=u(i1-2*is,i2,i3,vc)-6.*u(i1-is,i2,i3,vc)+15.*u(i1,i2,i3,vc)-20.*u(i1+is,i2,i3,vc)+\
                          15.*u(i1+2*is,i2,i3,vc)-6.*u(i1+3*is,i2,i3,vc)+u(i1+4*is,i2,i3,vc)
!  f2u=u(i1-2*is,i2,i3,uc)-5.*u(i1-is,i2,i3,uc)+10.*u(i1,i2,i3,uc)-10.*u(i1+is,i2,i3,uc)+\
!                          5.*u(i1+2*is,i2,i3,uc)-u(i1+3*is,i2,i3,uc)
!  f2v=u(i1-2*is,i2,i3,vc)-5.*u(i1-is,i2,i3,vc)+10.*u(i1,i2,i3,vc)-10.*u(i1+is,i2,i3,vc)+\
!                          5.*u(i1+2*is,i2,i3,vc)-u(i1+3*is,i2,i3,vc)
 #Else
  f2u=u(i1,i2-2*is,i3,uc)-6.*u(i1,i2-is,i3,uc)+15.*u(i1,i2,i3,uc)-20.*u(i1,i2+is,i3,uc)\
                        +15.*u(i1,i2+2*is,i3,uc)-6.*u(i1,i2+3*is,i3,uc)+u(i1,i2+4*is,i3,uc)
  f2v=u(i1,i2-2*is,i3,vc)-6.*u(i1,i2-is,i3,vc)+15.*u(i1,i2,i3,vc)-20.*u(i1,i2+is,i3,vc)\
                        +15.*u(i1,i2+2*is,i3,vc)-6.*u(i1,i2+3*is,i3,vc)+u(i1,i2+4*is,i3,vc)
!  f2u=u(i1,i2-2*is,i3,uc)-5.*u(i1,i2-is,i3,uc)+10.*u(i1,i2,i3,uc)-10.*u(i1,i2+is,i3,uc)+\
!                          5.*u(i1,i2+2*is,i3,uc)-u(i1,i2+3*is,i3,uc)
!  f2v=u(i1,i2-2*is,i3,vc)-5.*u(i1,i2-is,i3,vc)+10.*u(i1,i2,i3,vc)-10.*u(i1,i2+is,i3,vc)+\
!                          5.*u(i1,i2+2*is,i3,vc)-u(i1,i2+3*is,i3,vc)
 #End


 ! u1 = u(-1), u2=u(-2)
 u1=(-a22*f1u+a12*f2u)/det
 u2=(-a11*f2u+a21*f1u)/det
 v1=(-a22*f1v+a12*f2v)/det
 v2=(-a11*f2v+a21*f1v)/det


 ! Now set all components but keep the normal component the same:

 ! uDotN1 = u.n(-1),  uDotN2 = u.n(-2)
 uDotN1=( u1*rxi + v1*ryi )/rxsq
 uDotN2=( u2*rxi + v2*ryi )/rxsq

 ! rxi=rxi/sqrt(rxsq)
 ! ryi=ryi/sqrt(rxsq)
 ! uDotN1=( u1*rxi + v1*ryi )
 ! uDotN2=( u2*rxi + v2*ryi )

 #If #DIR == "r"
   ! write(*,*) 'r: u1,v1,u2,v2=',u1,v1,u2,v2
   ! write(*,*) 'r: uDotN1,uDotN2,rxi,ryi=',uDotN1,uDotN2,rxi,ryi
   ! write(*,*) 'before err:',u(i1-is,i2,i3,uc)-ogf(exact,x(i1-is,i2,i3,0),x(i1-is,i2,i3,1),0.,uc,t),
   !        u(i1-is,i2,i3,vc)-ogf(exact,x(i1-is,i2,i3,0),x(i1-is,i2,i3,1),0.,vc,t),
   !        u(i1-2*is,i2,i3,uc)-ogf(exact,x(i1-2*is,i2,i3,0),x(i1-2*is,i2,i3,1),0.,uc,t),
   !        u(i1-2*is,i2,i3,vc)-ogf(exact,x(i1-2*is,i2,i3,0),x(i1-2*is,i2,i3,1),0.,vc,t)
   u(i1-  is,i2,i3,uc)=u(i1-  is,i2,i3,uc) + u1 - uDotN1*rxi
   u(i1-  is,i2,i3,vc)=u(i1-  is,i2,i3,vc) + v1 - uDotN1*ryi
   u(i1-2*is,i2,i3,uc)=u(i1-2*is,i2,i3,uc) + u2 - uDotN2*rxi
   u(i1-2*is,i2,i3,vc)=u(i1-2*is,i2,i3,vc) + v2 - uDotN2*ryi
   ! write(*,*) 'r: err=',u(i1-is,i2,i3,uc)-ogf(exact,x(i1-is,i2,i3,0),x(i1-is,i2,i3,1),0.,uc,t),
   !        u(i1-is,i2,i3,vc)-ogf(exact,x(i1-is,i2,i3,0),x(i1-is,i2,i3,1),0.,vc,t),
   !        u(i1-2*is,i2,i3,uc)-ogf(exact,x(i1-2*is,i2,i3,0),x(i1-2*is,i2,i3,1),0.,uc,t),
   !        u(i1-2*is,i2,i3,vc)-ogf(exact,x(i1-2*is,i2,i3,0),x(i1-2*is,i2,i3,1),0.,vc,t)
 #Else                                
   ! write(*,*) 's: u1,v1,u2,v2=',u1,v1,u2,v2
   ! write(*,*) 's: uDotN1,uDotN2,rxi,ryi=',uDotN1,uDotN2,rxi,ryi
   ! write(*,*) 's: v,v1,uDotN1*ryi',u(i1,i2-  is,i3,vc),v1,uDotN1*ryi
   u(i1,i2-  is,i3,uc)=u(i1,i2-  is,i3,uc) + u1 - uDotN1*rxi
   u(i1,i2-  is,i3,vc)=u(i1,i2-  is,i3,vc) + v1 - uDotN1*ryi
   u(i1,i2-2*is,i3,uc)=u(i1,i2-2*is,i3,uc) + u2 - uDotN2*rxi
   u(i1,i2-2*is,i3,vc)=u(i1,i2-2*is,i3,vc) + v2 - uDotN2*ryi
   ! write(*,*) 's: err=',u(i1,i2-is,i3,uc)-ogf(exact,x(i1,i2-is,i3,0),x(i1,i2-is,i3,1),0.,uc,t),
   !        u(i1,i2-is  ,i3,vc)-ogf(exact,x(i1,i2-  is,i3,0),x(i1,i2-  is,i3,1),0.,vc,t),
   !        u(i1,i2-2*is,i3,uc)-ogf(exact,x(i1,i2-2*is,i3,0),x(i1,i2-2*is,i3,1),0.,uc,t),
   !        u(i1,i2-2*is,i3,vc)-ogf(exact,x(i1,i2-2*is,i3,0),x(i1,i2-2*is,i3,1),0.,vc,t)
 #End
#endMacro

! ==========================================================================================================
! Determine the tangential components of the velocity from the NS equations and extrapolation 
! ******************** 3D Version ***************************
! ==========================================================================================================
#beginMacro boundaryConditionNavierStokesAndExtrap3d(DIR,FORCING)
 rxi = DIR ## x(i1,i2,i3)
 ryi = DIR ## y(i1,i2,i3)
 rzi = DIR ## z(i1,i2,i3)
 rxsq=rxi**2+ryi**2+rzi**2
 rxxi=DIR ## xx43(i1,i2,i3)
 ryyi=DIR ## yy43(i1,i2,i3)
 rzzi=DIR ## zz43(i1,i2,i3)

 ! Include artificial dissipation terms *wdh* 100817 
 u0 = u(i1,i2,i3,uc)
 v0 = u(i1,i2,i3,vc)
 w0 = u(i1,i2,i3,wc)
 if( gridIsMoving.eq.0 )then
  ! grid is NOT moving
  ug0 = u0
  vg0 = v0
  wg0 = w0
  gtt0 = 0.
  gtt1 = 0.
  gtt2 = 0.
 else
  ! grid is moving
  !  ug0 = u - gridVelocity
  !  gtt0 = grid acceleration = u.t 
  ug0 = u0-gv(i1,i2,i3,0)
  vg0 = v0-gv(i1,i2,i3,1)
  wg0 = w0-gv(i1,i2,i3,2)
  gtt0 = gtt(i1,i2,i3,0)
  gtt1 = gtt(i1,i2,i3,1)
  gtt2 = gtt(i1,i2,i3,2)
 end if
 ux0 = ux43(i1,i2,i3,uc)  
 uy0 = uy43(i1,i2,i3,uc)
 uz0 = uz43(i1,i2,i3,uc)

 vx0 = ux43(i1,i2,i3,vc)
 vy0 = uy43(i1,i2,i3,vc)
 vz0 = uz43(i1,i2,i3,vc)

 wx0 = ux43(i1,i2,i3,wc)
 wy0 = uy43(i1,i2,i3,wc)
 wz0 = uz43(i1,i2,i3,wc)
 if( use4thOrderAD.ne.0 )then
   adCoeff4 = ad41+cd42*( abs(ux0)+abs(uy0)+abs(uz0) +abs(vx0)+abs(vy0)+abs(vz0) +abs(wx0)+abs(wy0)+abs(wz0) )
 end if
 if( use2ndOrderAD.ne.0 )then
   adCoeff2 = ad21+cd22*( abs(ux0)+abs(uy0)+abs(uz0) +abs(vx0)+abs(vy0)+abs(vz0) +abs(wx0)+abs(wy0)+abs(wz0) )
 end if

 ! a11 = nu*rxsq*(16.)/(12.*dr(axis)**2)+is*(nu*(rxxi+ryyi+rzzi)\
 !        -(u(i1,i2,i3,uc)*rxi+u(i1,i2,i3,vc)*ryi+u(i1,i2,i3,wc)*rzi))*(-8.)/(12.*dr(axis))
 ! a12 = nu*rxsq*(-1.)/(12.*dr(axis)**2)+is*(nu*(rxxi+ryyi+rzzi)\
 !        -(u(i1,i2,i3,uc)*rxi+u(i1,i2,i3,vc)*ryi+u(i1,i2,i3,wc)*rzi))*( 1.)/(12.*dr(axis))
 a11 = nu*rxsq*(16.)/(12.*dr(axis)**2)+is*(nu*(rxxi+ryyi+rzzi))*(-8.)/(12.*dr(axis)) + adCoeff4*4. + adCoeff2
 a12 = nu*rxsq*(-1.)/(12.*dr(axis)**2)+is*(nu*(rxxi+ryyi+rzzi))*( 1.)/(12.*dr(axis)) - adCoeff4
 a21 = -6.
 a22 =  1.
 det=a11*a22-a12*a21

 ! write(*,*) 'insbc4:tan:i=',i1,i2,i3
 ! write(*,*) 'f1u,f1v=',f1u,f1v
  ! corrections added for moving grids (*wdh* 111124)
 f1u=nu*ulaplacian43(i1,i2,i3,uc)-ug0*ux0-vg0*uy0-wg0*uz0-ux43(i1,i2,i3,pc) + adCoeff4*delta43(i1,i2,i3,uc) + adCoeff2*delta23(i1,i2,i3,uc) -gtt0
 f1v=nu*ulaplacian43(i1,i2,i3,vc)-ug0*vx0-vg0*vy0-wg0*vz0-uy43(i1,i2,i3,pc) + adCoeff4*delta43(i1,i2,i3,vc) + adCoeff2*delta23(i1,i2,i3,vc) -gtt1
 f1w=nu*ulaplacian43(i1,i2,i3,wc)-ug0*wx0-vg0*wy0-wg0*wz0-uz43(i1,i2,i3,pc) + adCoeff4*delta43(i1,i2,i3,wc) + adCoeff2*delta23(i1,i2,i3,wc) -gtt2
 #If #FORCING == "tz" 
   ! write(*,'(" insbc4: add TZ new way C3D")') 
   call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,ue)
   call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,ve)
   call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,we)

   call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,pc,pxe)
   call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,pc,pye)
   call ogDeriv(exact,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,pc,pze)

   call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uxe)
   call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uye)
   call ogDeriv(exact,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uze)
   call ogDeriv(exact,0,2,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uxxe)
   call ogDeriv(exact,0,0,2,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uyye)
   call ogDeriv(exact,0,0,0,2,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uzze)

   call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vxe)
   call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vye)
   call ogDeriv(exact,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vze)
   call ogDeriv(exact,0,2,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vxxe)
   call ogDeriv(exact,0,0,2,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vyye)
   call ogDeriv(exact,0,0,0,2,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vzze)

   call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wxe)
   call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wye)
   call ogDeriv(exact,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wze)
   call ogDeriv(exact,0,2,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wxxe)
   call ogDeriv(exact,0,0,2,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wyye)
   call ogDeriv(exact,0,0,0,2,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wzze)

   if( gridIsMoving.eq.1 )then
    ue = ue - gv(i1,i2,i3,0)
    ve = ve - gv(i1,i2,i3,1)
    we = we - gv(i1,i2,i3,2)
   end if
   ! Note: do not add ute, vte, wte
   f1u=f1u -nu*(uxxe+uyye+uzze) + ue*uxe + ve*uye + we*uze + pxe + gtt0 
   f1v=f1v -nu*(vxxe+vyye+vzze) + ue*vxe + ve*vye + we*vze + pye + gtt1 
   f1w=f1w -nu*(wxxe+wyye+wzze) + ue*wxe + ve*wye + we*wze + pze + gtt2 
 #End

 ! OLD way 111127
 !#If #FORCING == "tz" 
 ! f1u=nu*ulaplacian43(i1,i2,i3,uc)-ux43(i1,i2,i3,pc) + adCoeff4*delta43(i1,i2,i3,uc)
 ! f1v=nu*ulaplacian43(i1,i2,i3,vc)-uy43(i1,i2,i3,pc) + adCoeff4*delta43(i1,i2,i3,vc)
 ! f1w=nu*ulaplacian43(i1,i2,i3,wc)-uz43(i1,i2,i3,pc) + adCoeff4*delta43(i1,i2,i3,wc)
 ! f1u=f1u+insbfu3d(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,nu,pc,uc,vc,wc)
 ! f1v=f1v+insbfv3d(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,nu,pc,uc,vc,wc)
 ! f1w=f1w+insbfw3d(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,nu,pc,uc,vc,wc)
 ! ! write(*,*) 'tz:f1u,f1v,f2u,f2v=',f1u,f1v,f2u,f2v
 !#Else
 ! ! corrections added for moving grids (*wdh* 111124)
 ! f1u=nu*ulaplacian43(i1,i2,i3,uc)-ug0*ux0-vg0*uy0-wg0*uz0-ux43(i1,i2,i3,pc) + adCoeff4*delta43(i1,i2,i3,uc) -gtt0
 ! f1v=nu*ulaplacian43(i1,i2,i3,vc)-ug0*vx0-vg0*vy0-wg0*vz0-uy43(i1,i2,i3,pc) + adCoeff4*delta43(i1,i2,i3,vc) -gtt1
 ! f1w=nu*ulaplacian43(i1,i2,i3,wc)-ug0*wx0-vg0*wy0-wg0*wz0-uz43(i1,i2,i3,pc) + adCoeff4*delta43(i1,i2,i3,wc) -gtt2
 !#End

 if( assignTemperature.ne.0 )then
  ! *wdh* 110311 - include Boussinesq terms
  f1u=f1u-thermalExpansivity*gravity(0)*u(i1,i2,i3,tc)
  f1v=f1v-thermalExpansivity*gravity(1)*u(i1,i2,i3,tc)
  f1w=f1w-thermalExpansivity*gravity(2)*u(i1,i2,i3,tc)
  #If #FORCING == "tz"
    te = ogf(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),tc,t)
    f1u=f1u+thermalExpansivity*gravity(0)*te
    f1v=f1v+thermalExpansivity*gravity(1)*te
    f1w=f1w-thermalExpansivity*gravity(2)*te
  #End
 end if
 #If #DIR == "r"
  f2u=u(i1-2*is,i2,i3,uc)-6.*u(i1-is,i2,i3,uc)+15.*u(i1,i2,i3,uc)-20.*u(i1+is,i2,i3,uc)+\
                         15.*u(i1+2*is,i2,i3,uc)-6.*u(i1+3*is,i2,i3,uc)+u(i1+4*is,i2,i3,uc)
  f2v=u(i1-2*is,i2,i3,vc)-6.*u(i1-is,i2,i3,vc)+15.*u(i1,i2,i3,vc)-20.*u(i1+is,i2,i3,vc)+\
                         15.*u(i1+2*is,i2,i3,vc)-6.*u(i1+3*is,i2,i3,vc)+u(i1+4*is,i2,i3,vc)
  f2w=u(i1-2*is,i2,i3,wc)-6.*u(i1-is,i2,i3,wc)+15.*u(i1,i2,i3,wc)-20.*u(i1+is,i2,i3,wc)+\
                         15.*u(i1+2*is,i2,i3,wc)-6.*u(i1+3*is,i2,i3,wc)+u(i1+4*is,i2,i3,wc)
!  f2u=u(i1-2*is,i2,i3,uc)-5.*u(i1-is,i2,i3,uc)+10.*u(i1,i2,i3,uc)-10.*u(i1+is,i2,i3,uc)+\
!                          5.*u(i1+2*is,i2,i3,uc)-u(i1+3*is,i2,i3,uc)
!  f2v=u(i1-2*is,i2,i3,vc)-5.*u(i1-is,i2,i3,vc)+10.*u(i1,i2,i3,vc)-10.*u(i1+is,i2,i3,vc)+\
!                          5.*u(i1+2*is,i2,i3,vc)-u(i1+3*is,i2,i3,vc)
!  f2w=u(i1-2*is,i2,i3,wc)-5.*u(i1-is,i2,i3,wc)+10.*u(i1,i2,i3,wc)-10.*u(i1+is,i2,i3,wc)+\
!                          5.*u(i1+2*is,i2,i3,wc)-u(i1+3*is,i2,i3,wc)
 #Elif #DIR == "s"
  f2u=u(i1,i2-2*is,i3,uc)-6.*u(i1,i2-is,i3,uc)+15.*u(i1,i2,i3,uc)-20.*u(i1,i2+is,i3,uc)\
                        +15.*u(i1,i2+2*is,i3,uc)-6.*u(i1,i2+3*is,i3,uc)+u(i1,i2+4*is,i3,uc)
  f2v=u(i1,i2-2*is,i3,vc)-6.*u(i1,i2-is,i3,vc)+15.*u(i1,i2,i3,vc)-20.*u(i1,i2+is,i3,vc)\
                        +15.*u(i1,i2+2*is,i3,vc)-6.*u(i1,i2+3*is,i3,vc)+u(i1,i2+4*is,i3,vc)
  f2w=u(i1,i2-2*is,i3,wc)-6.*u(i1,i2-is,i3,wc)+15.*u(i1,i2,i3,wc)-20.*u(i1,i2+is,i3,wc)\
                        +15.*u(i1,i2+2*is,i3,wc)-6.*u(i1,i2+3*is,i3,wc)+u(i1,i2+4*is,i3,wc)
!  f2u=u(i1,i2-2*is,i3,uc)-5.*u(i1,i2-is,i3,uc)+10.*u(i1,i2,i3,uc)-10.*u(i1,i2+is,i3,uc)+\
!                          5.*u(i1,i2+2*is,i3,uc)-u(i1,i2+3*is,i3,uc)
!  f2v=u(i1,i2-2*is,i3,vc)-5.*u(i1,i2-is,i3,vc)+10.*u(i1,i2,i3,vc)-10.*u(i1,i2+is,i3,vc)+\
!                          5.*u(i1,i2+2*is,i3,vc)-u(i1,i2+3*is,i3,vc)
!  f2w=u(i1,i2-2*is,i3,wc)-5.*u(i1,i2-is,i3,wc)+10.*u(i1,i2,i3,wc)-10.*u(i1,i2+is,i3,wc)+\
!                          5.*u(i1,i2+2*is,i3,wc)-u(i1,i2+3*is,i3,wc)
 #Else
  f2u=u(i1,i2,i3-2*is,uc)-6.*u(i1,i2,i3-is,uc)+15.*u(i1,i2,i3,uc)-20.*u(i1,i2,i3+is,uc)\
                        +15.*u(i1,i2,i3+2*is,uc)-6.*u(i1,i2,i3+3*is,uc)+u(i1,i2,i3+4*is,uc)
  f2v=u(i1,i2,i3-2*is,vc)-6.*u(i1,i2,i3-is,vc)+15.*u(i1,i2,i3,vc)-20.*u(i1,i2,i3+is,vc)\
                        +15.*u(i1,i2,i3+2*is,vc)-6.*u(i1,i2,i3+3*is,vc)+u(i1,i2,i3+4*is,vc)
  f2w=u(i1,i2,i3-2*is,wc)-6.*u(i1,i2,i3-is,wc)+15.*u(i1,i2,i3,wc)-20.*u(i1,i2,i3+is,wc)\
                        +15.*u(i1,i2,i3+2*is,wc)-6.*u(i1,i2,i3+3*is,wc)+u(i1,i2,i3+4*is,wc)
!  f2u=u(i1,i2,i3-2*is,uc)-5.*u(i1,i2,i3-is,uc)+10.*u(i1,i2,i3,uc)-10.*u(i1,i2,i3+is,uc)+\
!                          5.*u(i1,i2,i3+2*is,uc)-u(i1,i2,i3+3*is,uc)
!  f2v=u(i1,i2,i3-2*is,vc)-5.*u(i1,i2,i3-is,vc)+10.*u(i1,i2,i3,vc)-10.*u(i1,i2,i3+is,vc)+\
!                          5.*u(i1,i2,i3+2*is,vc)-u(i1,i2,i3+3*is,vc)
!  f2w=u(i1,i2,i3-2*is,wc)-5.*u(i1,i2,i3-is,wc)+10.*u(i1,i2,i3,wc)-10.*u(i1,i2,i3+is,wc)+\
!                          5.*u(i1,i2,i3+2*is,wc)-u(i1,i2,i3+3*is,wc)
 #End


 u1=(-a22*f1u+a12*f2u)/det
 u2=(-a11*f2u+a21*f1u)/det
 v1=(-a22*f1v+a12*f2v)/det
 v2=(-a11*f2v+a21*f1v)/det
 w1=(-a22*f1w+a12*f2w)/det
 w2=(-a11*f2w+a21*f1w)/det

 ! Now set all components but keep the normal component the same:
 uDotN1=( u1*rxi + v1*ryi + w1*rzi )/rxsq
 uDotN2=( u2*rxi + v2*ryi + w2*rzi )/rxsq
 #If #DIR == "r"
   u(i1-  is,i2,i3,uc)=u(i1-  is,i2,i3,uc) + u1 - uDotN1*rxi
   u(i1-  is,i2,i3,vc)=u(i1-  is,i2,i3,vc) + v1 - uDotN1*ryi
   u(i1-  is,i2,i3,wc)=u(i1-  is,i2,i3,wc) + w1 - uDotN1*rzi

   u(i1-2*is,i2,i3,uc)=u(i1-2*is,i2,i3,uc) + u2 - uDotN2*rxi
   u(i1-2*is,i2,i3,vc)=u(i1-2*is,i2,i3,vc) + v2 - uDotN2*ryi
   u(i1-2*is,i2,i3,wc)=u(i1-2*is,i2,i3,wc) + w2 - uDotN2*rzi
 #Elif #DIR == "s"
   u(i1,i2-  is,i3,uc)=u(i1,i2-  is,i3,uc) + u1 - uDotN1*rxi
   u(i1,i2-  is,i3,vc)=u(i1,i2-  is,i3,vc) + v1 - uDotN1*ryi
   u(i1,i2-  is,i3,wc)=u(i1,i2-  is,i3,wc) + w1 - uDotN1*rzi
                                   
   u(i1,i2-2*is,i3,uc)=u(i1,i2-2*is,i3,uc) + u2 - uDotN2*rxi
   u(i1,i2-2*is,i3,vc)=u(i1,i2-2*is,i3,vc) + v2 - uDotN2*ryi
   u(i1,i2-2*is,i3,wc)=u(i1,i2-2*is,i3,wc) + w2 - uDotN2*rzi
 #Else
   u(i1,i2,i3-  is,uc)=u(i1,i2,i3-  is,uc) + u1 - uDotN1*rxi
   u(i1,i2,i3-  is,vc)=u(i1,i2,i3-  is,vc) + v1 - uDotN1*ryi
   u(i1,i2,i3-  is,wc)=u(i1,i2,i3-  is,wc) + w1 - uDotN1*rzi
                                      
   u(i1,i2,i3-2*is,uc)=u(i1,i2,i3-2*is,uc) + u2 - uDotN2*rxi
   u(i1,i2,i3-2*is,vc)=u(i1,i2,i3-2*is,vc) + v2 - uDotN2*ryi
   u(i1,i2,i3-2*is,wc)=u(i1,i2,i3-2*is,wc) + w2 - uDotN2*rzi
 #End
#endMacro



! ==========================================================================================================
! ************* rectangular grid version *****************
!   In this case we only need to compute and assign the tangential components
! ==========================================================================================================
#beginMacro boundaryConditionNavierStokesAndExtrapRectangular(DIR,FORCING,DIM)

 ! Include artificial dissipation terms *wdh* 100817 
 u0 = u(i1,i2,i3,uc)
 v0 = u(i1,i2,i3,vc)
#If #DIM == "2"
 if( gridIsMoving.eq.0 )then
  ! grid is NOT moving
  ug0 = u0
  vg0 = v0
  gtt0 = 0.
  gtt1 = 0.
 else
  ! grid is moving
  !  ug0 = u - gridVelocity
  !  gtt0 = grid acceleration = u.t 
  ug0 = u0-gv(i1,i2,i3,0)
  vg0 = v0-gv(i1,i2,i3,1)
  gtt0 = gtt(i1,i2,i3,0)
  gtt1 = gtt(i1,i2,i3,1)
 end if

 ux0 = ux42r(i1,i2,i3,uc)  
 uy0 = uy42r(i1,i2,i3,uc)
 vx0 = ux42r(i1,i2,i3,vc)
 vy0 = uy42r(i1,i2,i3,vc)

!!kkc ux0 = ux22r(i1,i2,i3,uc)  
!!kkc uy0 = uy22r(i1,i2,i3,uc)
!!kkc vx0 = ux22r(i1,i2,i3,vc)
!!kkc vy0 = uy22r(i1,i2,i3,vc)

 if( use4thOrderAD.ne.0 )then
   adCoeff4 = ad41+cd42*( abs(ux0)+abs(uy0)+abs(vx0)+abs(vy0) )
 end if
 if( use2ndOrderAD.ne.0 )then
   adCoeff2 = ad21+cd22*( abs(ux0)+abs(uy0)+abs(vx0)+abs(vy0) )
 end if
#Else
 w0 = u(i1,i2,i3,wc)
 if( gridIsMoving.eq.0 )then
  ! grid is NOT moving
  ug0 = u0
  vg0 = v0
  wg0 = w0
  gtt0 = 0.
  gtt1 = 0.
  gtt2 = 0.
 else
  ! grid is moving
  !  ug0 = u - gridVelocity
  !  gtt0 = grid acceleration = u.t 
  ug0 = u0-gv(i1,i2,i3,0)
  vg0 = v0-gv(i1,i2,i3,1)
  wg0 = w0-gv(i1,i2,i3,2)
  gtt0 = gtt(i1,i2,i3,0)
  gtt1 = gtt(i1,i2,i3,1)
  gtt2 = gtt(i1,i2,i3,2)
 end if

 ux0 = ux43r(i1,i2,i3,uc)  
 uy0 = uy43r(i1,i2,i3,uc)
 uz0 = uz43r(i1,i2,i3,uc)

 vx0 = ux43r(i1,i2,i3,vc)
 vy0 = uy43r(i1,i2,i3,vc)
 vz0 = uz43r(i1,i2,i3,vc)

 wx0 = ux43r(i1,i2,i3,wc)
 wy0 = uy43r(i1,i2,i3,wc)
 wz0 = uz43r(i1,i2,i3,wc)
 if( use4thOrderAD.ne.0 )then
   adCoeff4 = ad41+cd42*( abs(ux0)+abs(uy0)+abs(uz0) +abs(vx0)+abs(vy0)+abs(vz0) +abs(wx0)+abs(wy0)+abs(wz0) )
 end if
 if( use2ndOrderAD.ne.0 )then
   adCoeff2 = ad21+cd22*( abs(ux0)+abs(uy0)+abs(uz0) +abs(vx0)+abs(vy0)+abs(vz0) +abs(wx0)+abs(wy0)+abs(wz0) )
 end if
#End

 ! a11 = nu*(16.)/(12.*dx(axis)**2)+is*(-u(i1,i2,i3,uc+axis))*(-8.)/(12.*dx(axis))
 ! a12 = nu*(-1.)/(12.*dx(axis)**2)+is*(-u(i1,i2,i3,uc+axis))*( 1.)/(12.*dx(axis))
 ! -- for now lag the nonlinear term
 a11 = nu*(16.)/(12.*dx(axis)**2) + adCoeff4*4. + adCoeff2
 a12 = nu*(-1.)/(12.*dx(axis)**2) - adCoeff4
 a21 = -6.
 a22 =  1.
 det=a11*a22-a12*a21

 ! ---------------------------------------------------
 #If #DIR == "r"
   !   ===== Boundary x=constant =====
   #If #DIM == "2"
    ! -- 2D --

    f1v=nu*ulaplacian42r(i1,i2,i3,vc)-ug0*vx0-vg0*vy0-uy42r(i1,i2,i3,pc)+adCoeff4*delta42(i1,i2,i3,vc)+adCoeff2*delta22(i1,i2,i3,vc) -gtt1
    #If #FORCING == "tz"
      ! Add TZ forcing (new way)
      ! write(*,'(" insbc4: add TZ new way")') 

      call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,uc,ue)
      call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,pc,pye)

      call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,vc,ve)
      call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,vc,vxe)
      call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,vc,vye)
      call ogDeriv(exact,0,2,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,vc,vxxe)
      call ogDeriv(exact,0,0,2,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,vc,vyye)
      if( gridIsMoving.eq.1 )then
       ue = ue - gv(i1,i2,i3,0)
       ve = ve - gv(i1,i2,i3,1)
      end if
      ! Note: do not add vte 
      f1v=f1v -nu*(vxxe+vyye) + ue*vxe + ve*vye + pye + gtt1 
    #End

    ! OLD:
    ! #If #FORCING == "tz"
    !   f1v=nu*ulaplacian42r(i1,i2,i3,vc)-uy42r(i1,i2,i3,pc) +adCoeff4*delta42(i1,i2,i3,vc)
    !   f1v=f1v+insbfv2d(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,nu,pc,uc,vc)
    ! #Else
    !   f1v=nu*ulaplacian42r(i1,i2,i3,vc)-ug0*vx0-vg0*vy0-uy42r(i1,i2,i3,pc)+adCoeff4*delta42(i1,i2,i3,vc) -gtt1
    ! #End

   #Else
    ! -- 3D --

     f1v=nu*ulaplacian43r(i1,i2,i3,vc)-ug0*vx0-vg0*vy0-wg0*vz0-uy43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,vc) +adCoeff2*delta23(i1,i2,i3,vc) -gtt1
     f1w=nu*ulaplacian43r(i1,i2,i3,wc)-ug0*wx0-vg0*wy0-wg0*wz0-uz43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,wc) +adCoeff2*delta23(i1,i2,i3,wc) -gtt2
     #If #FORCING == "tz" 
      ! write(*,'(" insbc4: add TZ new way R3D-X")') 
      call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,ue)
      call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,ve)
      call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,we)
   
      call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,pc,pye)
      call ogDeriv(exact,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,pc,pze)
   
      call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vxe)
      call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vye)
      call ogDeriv(exact,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vze)
      call ogDeriv(exact,0,2,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vxxe)
      call ogDeriv(exact,0,0,2,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vyye)
      call ogDeriv(exact,0,0,0,2,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vzze)
   
      call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wxe)
      call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wye)
      call ogDeriv(exact,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wze)
      call ogDeriv(exact,0,2,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wxxe)
      call ogDeriv(exact,0,0,2,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wyye)
      call ogDeriv(exact,0,0,0,2,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wzze)
   
      if( gridIsMoving.eq.1 )then
       ue = ue - gv(i1,i2,i3,0)
       ve = ve - gv(i1,i2,i3,1)
       we = we - gv(i1,i2,i3,2)
      end if
      ! Note: do not add ute, vte, wte
      f1v=f1v -nu*(vxxe+vyye+vzze) + ue*vxe + ve*vye + we*vze + pye + gtt1 
      f1w=f1w -nu*(wxxe+wyye+wzze) + ue*wxe + ve*wye + we*wze + pze + gtt2 

     #End
     ! OLD 
     ! #If #FORCING == "tz" 
     !   f1v=nu*ulaplacian43r(i1,i2,i3,vc)-uy43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,vc)
     !   f1w=nu*ulaplacian43r(i1,i2,i3,wc)-uz43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,wc)
     !   f1v=f1v+insbfv3d(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,nu,pc,uc,vc,wc)
     !   f1w=f1w+insbfw3d(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,nu,pc,uc,vc,wc)
     ! #Else
     !  f1v=nu*ulaplacian43r(i1,i2,i3,vc)-ug0*vx0-vg0*vy0-wg0*vz0-uy43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,vc) -gtt1
     !  f1w=nu*ulaplacian43r(i1,i2,i3,wc)-ug0*wx0-vg0*wy0-wg0*wz0-uz43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,wc) -gtt2
     ! #End

     f2w=u(i1-2*is,i2,i3,wc)-6.*u(i1-  is,i2,i3,wc)+15.*u(i1     ,i2,i3,wc)-20.*u(i1+  is,i2,i3,wc)+\
                            15.*u(i1+2*is,i2,i3,wc) -6.*u(i1+3*is,i2,i3,wc)+    u(i1+4*is,i2,i3,wc)
   #End

   f2v=u(i1-2*is,i2,i3,vc)-6.*u(i1-is,i2,i3,vc)+15.*u(i1,i2,i3,vc)-20.*u(i1+is,i2,i3,vc)+\
                          15.*u(i1+2*is,i2,i3,vc)-6.*u(i1+3*is,i2,i3,vc)+u(i1+4*is,i2,i3,vc)
!   f2u=u(i1-2*is,i2,i3,uc)-5.*u(i1-is,i2,i3,uc)+10.*u(i1,i2,i3,uc)-10.*u(i1+is,i2,i3,uc)+\
!                           5.*u(i1+2*is,i2,i3,uc)-u(i1+3*is,i2,i3,uc)
!   f2v=u(i1,i2-2*is,i3,vc)-5.*u(i1,i2-is,i3,vc)+10.*u(i1,i2,i3,vc)-10.*u(i1,i2+is,i3,vc)+\
!                           5.*u(i1,i2+2*is,i3,vc)-u(i1,i2+3*is,i3,vc)
!   f2w=u(i1,i2,i3-2*is,wc)-5.*u(i1,i2,i3-is,wc)+10.*u(i1,i2,i3,wc)-10.*u(i1,i2,i3+is,wc)+\
!                           5.*u(i1,i2,i3+2*is,wc)-u(i1,i2,i3+3*is,wc)
 ! ---------------------------------------------------
 #Elif #DIR == "s"
   !   ===== Boundary y=constant =====
   #If #DIM == "2"
    ! -- 2D --

    f1u=nu*ulaplacian42r(i1,i2,i3,uc)-ug0*ux0-vg0*uy0-ux42r(i1,i2,i3,pc) +adCoeff4*delta42(i1,i2,i3,uc) +adCoeff2*delta22(i1,i2,i3,uc) -gtt0
    #If #FORCING == "tz" 
      ! Add TZ forcing (new way)
      ! write(*,'(" insbc4: add TZ new way (r-y)")') 

      call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,uc,ue)
      call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,vc,ve)

      call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,pc,pxe)

      call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,uc,uxe)
      call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,uc,uye)
      call ogDeriv(exact,0,2,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,uc,uxxe)
      call ogDeriv(exact,0,0,2,0,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,uc,uyye)
      if( gridIsMoving.eq.1 )then
       ue = ue - gv(i1,i2,i3,0)
       ve = ve - gv(i1,i2,i3,1)
      end if
      ! Note: do not add ute 
      f1u=f1u -nu*(uxxe+uyye) + ue*uxe + ve*uye + pxe + gtt0 
    #End

    ! OLD: 
    ! #If #FORCING == "tz" 
    !   f1u=nu*ulaplacian42r(i1,i2,i3,uc)-ux42r(i1,i2,i3,pc) +adCoeff4*delta42(i1,i2,i3,uc)
    !   f1u=f1u+insbfu2d(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,t,nu,pc,uc,vc)
    ! #Else
    !   f1u=nu*ulaplacian42r(i1,i2,i3,uc)-ug0*ux0-vg0*uy0-ux42r(i1,i2,i3,pc) +adCoeff4*delta42(i1,i2,i3,uc)-gtt0
    ! #End

   #Else
    ! -- 3D --

     f1u=nu*ulaplacian43r(i1,i2,i3,uc)-ug0*ux0-vg0*uy0-wg0*uz0-ux43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,uc) +adCoeff2*delta23(i1,i2,i3,uc) -gtt0
     f1w=nu*ulaplacian43r(i1,i2,i3,wc)-ug0*wx0-vg0*wy0-wg0*wz0-uz43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,wc) +adCoeff2*delta23(i1,i2,i3,wc) -gtt2
     #If #FORCING == "tz" 
      ! write(*,'(" insbc4: add TZ new way R3D-Y")') 
      call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,ue)
      call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,ve)
      call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,we)
   
      call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,pc,pxe)
      call ogDeriv(exact,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,pc,pze)
   
      call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uxe)
      call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uye)
      call ogDeriv(exact,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uze)
      call ogDeriv(exact,0,2,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uxxe)
      call ogDeriv(exact,0,0,2,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uyye)
      call ogDeriv(exact,0,0,0,2,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uzze)
   
      call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wxe)
      call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wye)
      call ogDeriv(exact,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wze)
      call ogDeriv(exact,0,2,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wxxe)
      call ogDeriv(exact,0,0,2,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wyye)
      call ogDeriv(exact,0,0,0,2,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,wzze)
   
      if( gridIsMoving.eq.1 )then
       ue = ue - gv(i1,i2,i3,0)
       ve = ve - gv(i1,i2,i3,1)
       we = we - gv(i1,i2,i3,2)
      end if
      ! Note: do not add ute, vte, wte
      f1u=f1u -nu*(uxxe+uyye+uzze) + ue*uxe + ve*uye + we*uze + pxe + gtt0 
      f1w=f1w -nu*(wxxe+wyye+wzze) + ue*wxe + ve*wye + we*wze + pze + gtt2 

     #End
     ! OLD
     ! #If #FORCING == "tz" 
     !  f1u=nu*ulaplacian43r(i1,i2,i3,uc)-ux43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,uc)
     !  f1w=nu*ulaplacian43r(i1,i2,i3,wc)-uz43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,wc)
     !  f1u=f1u+insbfu3d(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,nu,pc,uc,vc,wc)
     !  f1w=f1w+insbfw3d(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,nu,pc,uc,vc,wc)
     ! #Else
     !  f1u=nu*ulaplacian43r(i1,i2,i3,uc)-ug0*ux0-vg0*uy0-wg0*uz0-ux43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,uc)-gtt0
     !  f1w=nu*ulaplacian43r(i1,i2,i3,wc)-ug0*wx0-vg0*wy0-wg0*wz0-uz43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,wc)-gtt2
     ! #End

     f2w=u(i1,i2-2*is,i3,wc)-6.*u(i1,i2-is,i3,wc)+15.*u(i1,i2,i3,wc)-20.*u(i1,i2+is,i3,wc)\
                           +15.*u(i1,i2+2*is,i3,wc)-6.*u(i1,i2+3*is,i3,wc)+u(i1,i2+4*is,i3,wc)
   #End
   f2u=u(i1,i2-2*is,i3,uc)-6.*u(i1,i2-is,i3,uc)+15.*u(i1,i2,i3,uc)-20.*u(i1,i2+is,i3,uc)\
                         +15.*u(i1,i2+2*is,i3,uc)-6.*u(i1,i2+3*is,i3,uc)+u(i1,i2+4*is,i3,uc)
 ! ---------------------------------------------------
 #Else
   !   ===== Boundary z=constant =====
   f1u=nu*ulaplacian43r(i1,i2,i3,uc)-ug0*ux0-vg0*uy0-wg0*uz0-ux43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,uc) +adCoeff2*delta23(i1,i2,i3,uc) -gtt0
   f1v=nu*ulaplacian43r(i1,i2,i3,vc)-ug0*vx0-vg0*vy0-wg0*vz0-uy43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,vc) +adCoeff2*delta23(i1,i2,i3,vc) -gtt1
   #If #FORCING == "tz" 
    ! write(*,'(" insbc4: add TZ new way R3D-Z")') 
    call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,ue)
    call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,ve)
    call ogDeriv(exact,0,0,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,wc,we)
 
    call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,pc,pxe)
    call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,pc,pye)
 
    call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uxe)
    call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uye)
    call ogDeriv(exact,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uze)
    call ogDeriv(exact,0,2,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uxxe)
    call ogDeriv(exact,0,0,2,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uyye)
    call ogDeriv(exact,0,0,0,2,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,uc,uzze)
 
    call ogDeriv(exact,0,1,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vxe)
    call ogDeriv(exact,0,0,1,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vye)
    call ogDeriv(exact,0,0,0,1,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vze)
    call ogDeriv(exact,0,2,0,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vxxe)
    call ogDeriv(exact,0,0,2,0,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vyye)
    call ogDeriv(exact,0,0,0,2,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,vc,vzze)
 
    if( gridIsMoving.eq.1 )then
     ue = ue - gv(i1,i2,i3,0)
     ve = ve - gv(i1,i2,i3,1)
     we = we - gv(i1,i2,i3,2)
    end if
    ! Note: do not add ute, vte, wte
    f1u=f1u -nu*(uxxe+uyye+uzze) + ue*uxe + ve*uye + we*uze + pxe + gtt0 
    f1v=f1v -nu*(vxxe+vyye+vzze) + ue*vxe + ve*vye + we*vze + pye + gtt1 

   #End
   ! OLD
   ! #If #FORCING == "tz" 
   !  f1u=nu*ulaplacian43r(i1,i2,i3,uc)-ux43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,uc)
   !  f1v=nu*ulaplacian43r(i1,i2,i3,vc)-uy43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,vc)
   !  f1u=f1u+insbfu3d(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,nu,pc,uc,vc,wc)
   !  f1v=f1v+insbfv3d(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),t,nu,pc,uc,vc,wc)
   ! #Else
   !  f1u=nu*ulaplacian43r(i1,i2,i3,uc)-ug0*ux0-vg0*uy0-wg0*uz0-ux43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,uc) -gtt0
   !  f1v=nu*ulaplacian43r(i1,i2,i3,vc)-ug0*vx0-vg0*vy0-wg0*vz0-uy43r(i1,i2,i3,pc) +adCoeff4*delta43(i1,i2,i3,vc) -gtt1
   ! #End

   f2u=u(i1,i2,i3-2*is,uc)-6.*u(i1,i2,i3-is,uc)+15.*u(i1,i2,i3,uc)-20.*u(i1,i2,i3+is,uc)\
                         +15.*u(i1,i2,i3+2*is,uc)-6.*u(i1,i2,i3+3*is,uc)+u(i1,i2,i3+4*is,uc)
   f2v=u(i1,i2,i3-2*is,vc)-6.*u(i1,i2,i3-is,vc)+15.*u(i1,i2,i3,vc)-20.*u(i1,i2,i3+is,vc)\
                         +15.*u(i1,i2,i3+2*is,vc)-6.*u(i1,i2,i3+3*is,vc)+u(i1,i2,i3+4*is,vc)
 #End

 #If #DIM == "2"
  if( assignTemperature.ne.0 )then
    ! *wdh* 110311 - include Boussinesq terms
    f1u=f1u-thermalExpansivity*gravity(0)*u(i1,i2,i3,tc)
    f1v=f1v-thermalExpansivity*gravity(1)*u(i1,i2,i3,tc)
    #If #FORCING == "tz"
      te = ogf(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,tc,t)
      f1u=f1u+thermalExpansivity*gravity(0)*te
      f1v=f1v+thermalExpansivity*gravity(1)*te
    #End
  end if
 #Else
  if( assignTemperature.ne.0 )then
    ! *wdh* 110311 - include Boussinesq terms
    f1u=f1u-thermalExpansivity*gravity(0)*u(i1,i2,i3,tc)
    f1v=f1v-thermalExpansivity*gravity(1)*u(i1,i2,i3,tc)
    f1w=f1w-thermalExpansivity*gravity(2)*u(i1,i2,i3,tc)
    #If #FORCING == "tz"
     te = ogf(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),tc,t)
     f1u=f1u+thermalExpansivity*gravity(0)*te
     f1v=f1v+thermalExpansivity*gravity(1)*te
     f1w=f1w-thermalExpansivity*gravity(2)*te
    #End
  end if
 #End

 #If #DIR == "r"
   v1=(-a22*f1v+a12*f2v)/det
   v2=(-a11*f2v+a21*f1v)/det
   ! write(*,'(" bc4:tan: i1,i2=",2i4," nu,a11,a12,det,f1v,f2v,v1,v2=",8e10.2)') i1,i2,nu,a11,a12,det,f1v,f2v,v1,v2
!   write(*,*) 'i1,i2,f1v,f2v,v1,v2=',i1,i2,f1v,f2v,v1,v2
 !   write(*,*) ' true v',ogf(exact,x(i1-is,i2,i3,0),x(i1-is,i2,i3,1),0.,vc,t),ogf(exact,x(i1-2*is,i2,i3,0),x(i1-2*is,i2,i3,1),0.,vc,t)
   u(i1-  is,i2,i3,vc)=u(i1-  is,i2,i3,vc)+v1
   u(i1-2*is,i2,i3,vc)=u(i1-2*is,i2,i3,vc)+v2

   #If #DIM == "3"
     w1=(-a22*f1w+a12*f2w)/det
     w2=(-a11*f2w+a21*f1w)/det
     u(i1-  is,i2,i3,wc)=u(i1-  is,i2,i3,wc)+w1
     u(i1-2*is,i2,i3,wc)=u(i1-2*is,i2,i3,wc)+w2
   #End
 #Elif #DIR == "s"
   u1=(-a22*f1u+a12*f2u)/det
   u2=(-a11*f2u+a21*f1u)/det
   ! write(*,'("insbc4r:NS: i1,i2=",2i4," f1u,f2u,det,u1,u2=",5e10.2)') i1,i2,f1u,f2u,det,u1,u2
   ! write(*,*) ' true u',ogf(exact,x(i1,i2-is,i3,0),x(i1,i2-is,i3,1),0.,uc,t),ogf(exact,x(i1,i2-2*is,i3,0),x(i1,i2-2*is,i3,1),0.,uc,t)
   u(i1,i2-  is,i3,uc)=u(i1,i2-  is,i3,uc)+u1
   u(i1,i2-2*is,i3,uc)=u(i1,i2-2*is,i3,uc)+u2
   #If #DIM == "3"
     w1=(-a22*f1w+a12*f2w)/det
     w2=(-a11*f2w+a21*f1w)/det
     u(i1,i2-  is,i3,wc)=u(i1,i2-  is,i3,wc)+w1
     u(i1,i2-2*is,i3,wc)=u(i1,i2-2*is,i3,wc)+w2
   #End
 #Else
   u1=(-a22*f1u+a12*f2u)/det
   u2=(-a11*f2u+a21*f1u)/det
   v1=(-a22*f1v+a12*f2v)/det
   v2=(-a11*f2v+a21*f1v)/det
   u(i1,i2,i3-  is,uc)=u(i1,i2,i3-  is,uc)+u1
   u(i1,i2,i3-2*is,uc)=u(i1,i2,i3-2*is,uc)+u2
   u(i1,i2,i3-  is,vc)=u(i1,i2,i3-  is,vc)+v1
   u(i1,i2,i3-2*is,vc)=u(i1,i2,i3-2*is,vc)+v2
 #End
#endMacro



! ==========================================================================================================
! Apply the boundary condition div(u)=0 div(u).n=0 to determine the normal compoennts of the 2 ghost points
!  Curvilinear grid case
! DIR = r,s,t
! ==========================================================================================================
#beginMacro boundaryConditionDivAndDivN(DIR)
 rxi = rx(i1,i2,i3)
 ryi = ry(i1,i2,i3)
 sxi = sx(i1,i2,i3)
 syi = sy(i1,i2,i3)
 rxd = rx ## DIR ## 4(i1,i2,i3)
 ryd = ry ## DIR ## 4(i1,i2,i3)
 sxd = sx ## DIR ## 4(i1,i2,i3)
 syd = sy ## DIR ## 4(i1,i2,i3)
 rxsq=DIR ## xi**2+DIR ## yi**2
 rxsqd=DIR ## xi*DIR ## xd+DIR ## yi*DIR ## yd

 f1=ux42(i1,i2,i3,uc)+uy42(i1,i2,i3,vc)
 #If #DIR == "r"
  f2=rxi*urr4(i1,i2,i3,uc)+rxd*ur4(i1,i2,i3,uc)+sxi*urs4(i1,i2,i3,uc)+sxd*us4(i1,i2,i3,uc)+\
     ryi*urr4(i1,i2,i3,vc)+ryd*ur4(i1,i2,i3,vc)+syi*urs4(i1,i2,i3,vc)+syd*us4(i1,i2,i3,vc)
 #Else
  f2=rxi*urs4(i1,i2,i3,uc)+rxd*ur4(i1,i2,i3,uc)+sxi*uss4(i1,i2,i3,uc)+sxd*us4(i1,i2,i3,uc)+\
     ryi*urs4(i1,i2,i3,vc)+ryd*ur4(i1,i2,i3,vc)+syi*uss4(i1,i2,i3,vc)+syd*us4(i1,i2,i3,vc)
 #End
 a11 = -8.*is*rxsq/(12.*dr(axis))
 a12 =     is*rxsq/(12.*dr(axis))
 a21 = 16.*rxsq/(12.*dr(axis)**2)-8.*is*rxsqd/(12.*dr(axis))
 a22 = -1.*rxsq/(12.*dr(axis)**2)+   is*rxsqd/(12.*dr(axis))

 det=a11*a22-a12*a21
 alpha=(-a22*f1+a12*f2)/det
 beta =(-a11*f2+a21*f1)/det
 #If #DIR == "r"
   u(i1-  is,i2,i3,uc)=u(i1-  is,i2,i3,uc)+alpha*rxi
   u(i1-  is,i2,i3,vc)=u(i1-  is,i2,i3,vc)+alpha*ryi
   u(i1-2*is,i2,i3,uc)=u(i1-2*is,i2,i3,uc)+ beta*rxi
   u(i1-2*is,i2,i3,vc)=u(i1-2*is,i2,i3,vc)+ beta*ryi
 #Else
   u(i1,i2-  is,i3,uc)=u(i1,i2-  is,i3,uc)+alpha*sxi
   u(i1,i2-  is,i3,vc)=u(i1,i2-  is,i3,vc)+alpha*syi
   u(i1,i2-2*is,i3,uc)=u(i1,i2-2*is,i3,uc)+ beta*sxi
   u(i1,i2-2*is,i3,vc)=u(i1,i2-2*is,i3,vc)+ beta*syi
 #End

 ! Limiter:
 if( .false. )then
    
   epsu=1.e-3  ! fix me 
   clim=2. 

   limitGhostVelocity( u1,u2,uc,DIR )
   limitGhostVelocity( v1,v2,vc,DIR )

 end if

#endMacro

!  Three-dimensional version
#beginMacro boundaryConditionDivAndDivN3d(DIR)
 rxi = rx(i1,i2,i3)
 ryi = ry(i1,i2,i3)
 rzi = rz(i1,i2,i3)
 sxi = sx(i1,i2,i3)
 syi = sy(i1,i2,i3)
 szi = sz(i1,i2,i3)
 txi = tx(i1,i2,i3)
 tyi = ty(i1,i2,i3)
 tzi = tz(i1,i2,i3)
 rxd = rx ## DIR ## 4(i1,i2,i3)
 ryd = ry ## DIR ## 4(i1,i2,i3)
 rzd = rz ## DIR ## 4(i1,i2,i3)
 sxd = sx ## DIR ## 4(i1,i2,i3)
 syd = sy ## DIR ## 4(i1,i2,i3)
 szd = sz ## DIR ## 4(i1,i2,i3)
 txd = tx ## DIR ## 4(i1,i2,i3)
 tyd = ty ## DIR ## 4(i1,i2,i3)
 tzd = tz ## DIR ## 4(i1,i2,i3)
 rxsq=(DIR ## xi**2) + (DIR ## yi**2) + (DIR ## zi**2)
 rxsqd=(DIR ## xi*DIR ## xd) + (DIR ## yi*DIR ## yd) + (DIR ## zi*DIR ## zd)

 f1=ux43(i1,i2,i3,uc)+uy43(i1,i2,i3,vc)+uz43(i1,i2,i3,wc)
 #If #DIR == "r"
  f2=rxi*urr4(i1,i2,i3,uc)+rxd*ur4(i1,i2,i3,uc)+\
     sxi*urs4(i1,i2,i3,uc)+sxd*us4(i1,i2,i3,uc)+\
     txi*urt4(i1,i2,i3,uc)+txd*ut4(i1,i2,i3,uc)+\
     ryi*urr4(i1,i2,i3,vc)+ryd*ur4(i1,i2,i3,vc)+\
     syi*urs4(i1,i2,i3,vc)+syd*us4(i1,i2,i3,vc)+\
     tyi*urt4(i1,i2,i3,vc)+tyd*ut4(i1,i2,i3,vc)+\
     rzi*urr4(i1,i2,i3,wc)+rzd*ur4(i1,i2,i3,wc)+\
     szi*urs4(i1,i2,i3,wc)+szd*us4(i1,i2,i3,wc)+\
     tzi*urt4(i1,i2,i3,wc)+tzd*ut4(i1,i2,i3,wc)
 #Elif #DIR == "s"
  f2=rxi*urs4(i1,i2,i3,uc)+rxd*ur4(i1,i2,i3,uc)+\
     sxi*uss4(i1,i2,i3,uc)+sxd*us4(i1,i2,i3,uc)+\
     txi*ust4(i1,i2,i3,uc)+txd*ut4(i1,i2,i3,uc)+\
     ryi*urs4(i1,i2,i3,vc)+ryd*ur4(i1,i2,i3,vc)+\
     syi*uss4(i1,i2,i3,vc)+syd*us4(i1,i2,i3,vc)+\
     tyi*ust4(i1,i2,i3,vc)+tyd*ut4(i1,i2,i3,vc)+\
     rzi*urs4(i1,i2,i3,wc)+rzd*ur4(i1,i2,i3,wc)+\
     szi*uss4(i1,i2,i3,wc)+szd*us4(i1,i2,i3,wc)+\
     tzi*ust4(i1,i2,i3,wc)+tzd*ut4(i1,i2,i3,wc)
 #Else
  f2=rxi*urt4(i1,i2,i3,uc)+rxd*ur4(i1,i2,i3,uc)+\
     sxi*ust4(i1,i2,i3,uc)+sxd*us4(i1,i2,i3,uc)+\
     txi*utt4(i1,i2,i3,uc)+txd*ut4(i1,i2,i3,uc)+\
     ryi*urt4(i1,i2,i3,vc)+ryd*ur4(i1,i2,i3,vc)+\
     syi*ust4(i1,i2,i3,vc)+syd*us4(i1,i2,i3,vc)+\
     tyi*utt4(i1,i2,i3,vc)+tyd*ut4(i1,i2,i3,vc)+\
     rzi*urt4(i1,i2,i3,wc)+rzd*ur4(i1,i2,i3,wc)+\
     szi*ust4(i1,i2,i3,wc)+szd*us4(i1,i2,i3,wc)+\
     tzi*utt4(i1,i2,i3,wc)+tzd*ut4(i1,i2,i3,wc)
 #End
 a11 = -8.*is*rxsq/(12.*dr(axis))
 a12 =     is*rxsq/(12.*dr(axis))
 a21 = 16.*rxsq/(12.*dr(axis)**2)-8.*is*rxsqd/(12.*dr(axis))
 a22 = -1.*rxsq/(12.*dr(axis)**2)+   is*rxsqd/(12.*dr(axis))

 det=a11*a22-a12*a21
 alpha=(-a22*f1+a12*f2)/det
 beta =(-a11*f2+a21*f1)/det

 #If #DIR == "r"
   ! write(*,'(''divn:DIR: i='',3i3,'' f1,f2,alpha,beta='',4e10.2)') i1,i2,i3,f1,f2,alpha,beta
   u(i1-  is,i2,i3,uc)=u(i1-  is,i2,i3,uc)+alpha*rxi
   u(i1-  is,i2,i3,vc)=u(i1-  is,i2,i3,vc)+alpha*ryi
   u(i1-  is,i2,i3,wc)=u(i1-  is,i2,i3,wc)+alpha*rzi
   u(i1-2*is,i2,i3,uc)=u(i1-2*is,i2,i3,uc)+ beta*rxi
   u(i1-2*is,i2,i3,vc)=u(i1-2*is,i2,i3,vc)+ beta*ryi
   u(i1-2*is,i2,i3,wc)=u(i1-2*is,i2,i3,wc)+ beta*rzi
 #Elif #DIR == "s"
   u(i1,i2-  is,i3,uc)=u(i1,i2-  is,i3,uc)+alpha*sxi
   u(i1,i2-  is,i3,vc)=u(i1,i2-  is,i3,vc)+alpha*syi
   u(i1,i2-  is,i3,wc)=u(i1,i2-  is,i3,wc)+alpha*szi
   u(i1,i2-2*is,i3,uc)=u(i1,i2-2*is,i3,uc)+ beta*sxi
   u(i1,i2-2*is,i3,vc)=u(i1,i2-2*is,i3,vc)+ beta*syi
   u(i1,i2-2*is,i3,wc)=u(i1,i2-2*is,i3,wc)+ beta*szi
 #Else
   u(i1,i2,i3-  is,uc)=u(i1,i2,i3-  is,uc)+alpha*txi
   u(i1,i2,i3-  is,vc)=u(i1,i2,i3-  is,vc)+alpha*tyi
   u(i1,i2,i3-  is,wc)=u(i1,i2,i3-  is,wc)+alpha*tzi
   u(i1,i2,i3-2*is,uc)=u(i1,i2,i3-2*is,uc)+ beta*txi
   u(i1,i2,i3-2*is,vc)=u(i1,i2,i3-2*is,vc)+ beta*tyi
   u(i1,i2,i3-2*is,wc)=u(i1,i2,i3-2*is,wc)+ beta*tzi
 #End
#endMacro


! ==========================================================================================================
! Extrapolate two ghost values
! ==========================================================================================================
#beginMacro extrapTwoGhost(ORDER,DIR)
 do c=uc,uc+nd-1
 #If #ORDER == "5" 
  #If #DIR == "r"
    u(i1-is,i2,i3,c)=5.*(u(i1   ,i2,i3,c)-u(i1+3*is,i2,i3,c))\
                   -10.*(u(i1+is,i2,i3,c)-u(i1+2*is,i2,i3,c))+u(i1+4*is,i2,i3,c)
    u(i1-2*is,i2,i3,c)=5.*(u(i1-is,i2,i3,c)-u(i1+2*is,i2,i3,c))\
                     -10.*(u(i1   ,i2,i3,c)-u(i1+  is,i2,i3,c))+u(i1+3*is,i2,i3,c)
  #Elif #DIR == "s"
    u(i1,i2-is,i3,c)=5.*(u(i1,i2   ,i3,c)-u(i1,i2+3*is,i3,c))\
                   -10.*(u(i1,i2+is,i3,c)-u(i1,i2+2*is,i3,c))+u(i1,i2+4*is,i3,c)
    u(i1,i2-2*is,i3,c)=5.*(u(i1,i2-is,i3,c)-u(i1,i2+2*is,i3,c))\
                     -10.*(u(i1,i2   ,i3,c)-u(i1,i2+  is,i3,c))+u(i1,i2+3*is,i3,c)

    ! write(*,'(''extrap (DIR) c='',i2,''i='',i4,i4,2x,i4,i4)') c,i1,i2-is,i1,i2-2*is

    ! write(*,'(''extrap (DIR) c='',i2,''i='',i4,i4,2x,i4,i4)') c,i1,i2-is,i1,i2-2*is
    ! u(i1,i2-is,i3,c)=ogf(exact,x(i1,i2-is,i3,0),x(i1,i2-is,i3,1),0.,c,t)
    ! u(i1,i2-2*is,i3,c)=ogf(exact,x(i1,i2-2*is,i3,0),x(i1,i2-2*is,i3,1),0.,c,t)
  #Elif #DIR == "t"
    u(i1,i2,i3-is,c)=5.*(u(i1,i2,i3   ,c)-u(i1,i2,i3+3*is,c))\
                   -10.*(u(i1,i2,i3+is,c)-u(i1,i2,i3+2*is,c))+u(i1,i2,i3+4*is,c)
    u(i1,i2,i3-2*is,c)=5.*(u(i1,i2,i3-is,c)-u(i1,i2,i3+2*is,c))\
                     -10.*(u(i1,i2,i3   ,c)-u(i1,i2,i3+  is,c))+u(i1,i2,i3+3*is,c)
  #Else
   write(*,*) 'ERROR:unknown dir'
   stop 8
  #End
 #Else
   write(*,*) 'ERROR:unknown extrap order'
   stop 7
 #End
 end do
#endMacro

! ==========================================================================================================
! ==========================================================================================================
#beginMacro extrapolate(ORDER)
 if( kd2.eq.0 )then
   loopse4($extrapTwoGhost(ORDER,r),,,)
 else if( kd2.eq.1 )then
   loopse4($extrapTwoGhost(ORDER,s),,,)
 else
   loopse4($extrapTwoGhost(ORDER,t),,,)
 end if
#endMacro


! ===============================================================================================
!  Macro: Assign an even-symmetry condition:
!    u(i1m,i2m,i3m,uc)=u(i1p,i2p,i3p,uc)
! Input:
!   FORCING : tz or none
!   DIM : 2 or 3, number of space dimensions
! ===============================================================================================
#beginMacro applyEvenSymmetry(uc,i1m,i2m,i3m,i1p,i2p,i3p,FORCING,DIM)
  u(i1m,i2m,i3m,uc)=u(i1p,i2p,i3p,uc)
  #If #FORCING == "tz"
    #If #DIM eq "2"
      u(i1m,i2m,i3m,uc)=u(i1m,i2m,i3m,uc)\
                   +ogf(exact,x(i1m,i2m,i3m,0),x(i1m,i2m,i3m,1),0.,uc,t)\
                   -ogf(exact,x(i1p,i2p,i3p,0),x(i1p,i2p,i3p,1),0.,uc,t)
    #Elif #DIM eq "3"
      u(i1m,i2m,i3m,uc)=u(i1m,i2m,i3m,uc)\
                   +ogf(exact,x(i1m,i2m,i3m,0),x(i1m,i2m,i3m,1),x(i1m,i2m,i3m,2),uc,t)\
                   -ogf(exact,x(i1p,i2p,i3p,0),x(i1p,i2p,i3p,1),x(i1p,i2p,i3p,2),uc,t)
    #Else
       stop 1044
    #End
  #Elif #FORCING == "none"
  #Else
    stop 1045
  #End
#endMacro

! ===============================================================================================
!  Macro: Outflow Neumann-like BC  (where we might expect local inflow)
! Input:
!   FORCING : tz or none
!   DIM : 2 or 3, number of space dimensions
! ===============================================================================================
#beginMacro boundaryConditionNeumannOutflow(FORCING,DIM)
 ! Ghost line 1:
 i1m=i1-is1
 i2m=i2-is2
 i3m=i3-is3
 i1p=i1+is1
 i2p=i2+is2
 i3p=i3+is3
 applyEvenSymmetry(uc,i1m,i2m,i3m,i1p,i2p,i3p,FORCING,DIM)
 applyEvenSymmetry(vc,i1m,i2m,i3m,i1p,i2p,i3p,FORCING,DIM)
 #If #DIM eq "3"
  applyEvenSymmetry(wc,i1m,i2m,i3m,i1p,i2p,i3p,FORCING,DIM)
 #End

 ! Ghost line 2:
 i1m=i1-2*is1
 i2m=i2-2*is2
 i3m=i3-2*is3
 i1p=i1+2*is1
 i2p=i2+2*is2
 i3p=i3+2*is3

 applyEvenSymmetry(uc,i1m,i2m,i3m,i1p,i2p,i3p,FORCING,DIM)
 applyEvenSymmetry(vc,i1m,i2m,i3m,i1p,i2p,i3p,FORCING,DIM)
 #If #DIM eq "3"
  applyEvenSymmetry(wc,i1m,i2m,i3m,i1p,i2p,i3p,FORCING,DIM)
 #End

#endMacro


! -- OLD --
#beginMacro boundaryConditionNeumannOutflowOLD(FORCING,DIM)
 ! Try this *wdh* 100613 
 u(i1-  is1,i2-  is2,i3-  is3,uc)=u(i1+  is1,i2+  is2,i3+  is3,uc)
 u(i1-2*is1,i2-2*is2,i3-2*is3,uc)=u(i1+2*is1,i2+2*is2,i3+2*is3,uc)
 u(i1-  is1,i2-  is2,i3-  is3,vc)=u(i1+  is1,i2+  is2,i3+  is3,vc)
 u(i1-2*is1,i2-2*is2,i3-2*is3,vc)=u(i1+2*is1,i2+2*is2,i3+2*is3,vc)
#If #DIM eq "3"
 u(i1-  is1,i2-  is2,i3-  is3,wc)=u(i1+  is1,i2+  is2,i3+  is3,wc)
 u(i1-2*is1,i2-2*is2,i3-2*is3,wc)=u(i1+2*is1,i2+2*is2,i3+2*is3,wc)
#End
#endMacro

      subroutine insbc4(bcOption, nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & ipar,rpar, u, mask, x,rsxy, gv, gtt, bc, indexRange, ierr )         
!=============================================================================================================
!     Apply 4th order Boundary conditions
!
! Notes:
!  ipar(18) = outflowOption (input) : if outflowOption=1 then apply a Neumann
!            boundary condition at outflow (appropriate if there may be local inflow at an outflow boundary
!  ipar(19) = orderOfExtrapolationForOutflow, 0 means use default
!============================================================================================================
      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b
!     integer *8 exact ! holds pointer to OGFunction
      real exact ! holds pointer to OGFunction
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real gtt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
      real rpar(0:*)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer indexRange(0:1,0:2), bc(0:1,0:2)
      integer ipar(0:*),ierr

!.......local
      logical useWallBC,useOutflowBC
      integer numberOfProcessors,outflowOption,orderOfExtrapolationForOutflow,debug,myid
      integer kd,kd3,i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b,c,nr0,nr1
      integer is,j1,j2,j3,side,axis,twilightZone,bcOption
      integer i1m,i2m,i3m,i1p,i2p,i3p
      integer pc,uc,vc,wc,sc,grid,orderOfAccuracy,gridIsMoving,useWhereMask,tc,assignTemperature
      integer gridType,gridIsImplicit,implicitMethod,implicitOption,isAxisymmetric
      integer use2ndOrderAD,use4thOrderAD,advectPassiveScalar
      integer nr(0:1,0:2)
      integer bcOptionWallNormal
      integer bc1,bc2,extrapOrder,ks1,kd1,ks2,kd2,is1,is2,is3

      real t,nu,ad21,ad22,ad41,ad42,nuPassiveScalar,adcPassiveScalar,thermalExpansivity,te
      real cd42,adCoeff4, cd22, adCoeff2
      real dr(0:2),dx(0:2),d14v(0:2),d24v(0:2), gravity(0:2)
      real vy,vxy,ux,uxy
      real f1,f2,a11,a12,a21,a22,det,alpha,beta,rxsq,rxsqr,ajs
      real rxd,ryd,rzd,sxd,syd,szd,txd,tyd,tzd,rxsqd
      real rxi,ryi,rzi,sxi,syi,szi,txi,tyi,tzi,rxxi,ryyi,rzzi
      real u1,u2,v1,v2,w1,w2,f1u,f2u,f1v,f2v,f1w,f2w,uDotN1,uDotN2

      real u0,v0,w0, ux0,uy0,uz0, vx0,vy0,vz0, wx0,wy0,wz0
      real ug0,vg0,wg0, gtt0, gtt1, gtt2

      ! variables to hold the exact solution:
      real ue,uxe,uye,uze,uxxe,uyye,uzze,ute
      real ve,vxe,vye,vze,vxxe,vyye,vzze,vte
      real we,wxe,wye,wze,wxxe,wyye,wzze,wte
      real pe,pxe,pye,pze,pxxe,pyye,pzze,pte

      real uxa,uya,uza, vxa,vya,vza, wxa,wya,wza
      real dr12,dr22, dx12,dx22
      real ur2,us2,ut2, ux22,uy22, ux23,uy23,uz23, ux22r,uy22r, ux23r, uy23r, uz23r

      real uExtrap2,uExtrap3,epsu, u1a,u2a, uLim, clim

!..................
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )

      integer doubleDiv,divAndDivN
      parameter( doubleDiv=0, divAndDivN=1 )
      integer 
     &     noSlipWall,
     &     inflowWithVelocityGiven,
     &     slipWall,
     &     outflow,
     &     convectiveOutflow,
     &     tractionFree,
     &     inflowWithPandTV,
     &     dirichletBoundaryCondition,
     &     symmetry,
     &     axisymmetric,
     &     penaltyBoundaryCondition
      parameter( noSlipWall=1,inflowWithVelocityGiven=2,
     & slipWall=4,outflow=5,convectiveOutflow=14,tractionFree=15,
     & inflowWithPandTV=3,
     &  dirichletBoundaryCondition=12,
     &  symmetry=11,axisymmetric=13, penaltyBoundaryCondition=100 )

      ! outflowOption values:
      integer extrapolateOutflow,neumannAtOuflow
      parameter( extrapolateOutflow=0,neumannAtOuflow=1 )

      ! declare variables for difference approximations
      ! include 'declareDiffOrder4f.h'
      declareDifferenceOrder4(u,RX)
!!kkc      declareDifferenceOrder2(u,RX)

! .............. begin statement functions
      real divBCr2d,divBCs2d, divBCr3d,divBCs3d,divBCt3d
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real insbfu2d,insbfv2d,insbfu3d,insbfv3d,insbfw3d,ogf
      real delta42,delta43, delta22, delta23

!.......statement functions for jacobian
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

!     The next macro call will define the difference approximation statement functions
      defineDifferenceOrder4Components1(u,RX)
!!kkc      defineDifferenceOrder2Components1(u,RX)

      ! *** div(u) =0 *** for rectangular 2D
      divBCr2d(i1)=8.*(u(i1-is,i2,i3,uc)-u(i1+is,i2,i3,uc))+u(i1+2*is,i2,i3,uc) \
        -is*dx(0)/dx(1)*( 8.*(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))+  \
       	 	              u(i1,i2-2,i3,vc)-u(i1,i2+2,i3,vc))
      divBCs2d(i2)=8.*(u(i1,i2-is,i3,vc)-u(i1,i2+is,i3,vc))+u(i1,i2+2*is,i3,vc) \
        -is*dx(1)/dx(0)*( 8.*(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))+  \
       	 	              u(i1-2,i2,i3,uc)-u(i1+2,i2,i3,uc))

      divBCr3d(i1)=8.*(u(i1-is,i2,i3,uc)-u(i1+is,i2,i3,uc))+u(i1+2*is,i2,i3,uc) \
        -is*dx(0)*12.*( uy43r(i1,i2,i3,vc)+uz43r(i1,i2,i3,wc) )

      divBCs3d(i2)=8.*(u(i1,i2-is,i3,vc)-u(i1,i2+is,i3,vc))+u(i1,i2+2*is,i3,vc) \
        -is*dx(1)*12.*( ux43r(i1,i2,i3,uc)+uz43r(i1,i2,i3,wc) )

      divBCt3d(i2)=8.*(u(i1,i2,i3-is,wc)-u(i1,i2,i3+is,wc))+u(i1,i2,i3+2*is,wc) \
        -is*dx(2)*12.*( ux43r(i1,i2,i3,uc)+uy43r(i1,i2,i3,vc) )


!     ---For fourth-order artificial diffusion in 2D
      delta42(i1,i2,i3,c)= \
        (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)   \
            -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)   \
        +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   \
            +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))  \
         -12.*u(i1,i2,i3,c) ) 
!     ---For fourth-order artificial diffusion in 3D
      delta43(i1,i2,i3,c)= \
        (   -u(i1+2,i2,i3,c)-u(i1-2,i2,i3,c)  \
            -u(i1,i2+2,i3,c)-u(i1,i2-2,i3,c)  \
            -u(i1,i2,i3+2,c)-u(i1,i2,i3-2,c)  \
        +4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  \
            +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)  \
            +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) \
         -18.*u(i1,i2,i3,c) )

!     ---For second-order artificial diffusion in 2D
      delta22(i1,i2,i3,c)= \
        (   (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)   \
            +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))  \
         -4.*u(i1,i2,i3,c) ) 
!     ---For second-order artificial diffusion in 3D
      delta23(i1,i2,i3,c)= \
        (   (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)  \
            +u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)  \
            +u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) \
         -6.*u(i1,i2,i3,c) )

      ! define some second-order derivatives used for the artificial dissipation
      dr12(kd) = 1./(2.*dr(kd))
      dr22(kd) = 1./(dr(kd)**2)
      ur2(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*dr12(0)
      us2(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*dr12(1)
      ut2(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*dr12(2)
      ux22(i1,i2,i3,kd)= rsxy(i1,i2,i3,0,0)*ur2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*us2(i1,i2,i3,kd)
      uy22(i1,i2,i3,kd)= rsxy(i1,i2,i3,0,1)*ur2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*us2(i1,i2,i3,kd)

      ux23(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,0)*ur2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,0)*us2(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,0)*ut2(i1,i2,i3,kd)
      uy23(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,1)*ur2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,1)*us2(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,1)*ut2(i1,i2,i3,kd)
      uz23(i1,i2,i3,kd)=rsxy(i1,i2,i3,0,2)*ur2(i1,i2,i3,kd)+rsxy(i1,i2,i3,1,2)*us2(i1,i2,i3,kd)+rsxy(i1,i2,i3,2,2)*ut2(i1,i2,i3,kd)

      dx12(kd) = 1./(2.*dx(kd))
      dx22(kd) = 1./(dx(kd)**2)
      ux23r(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*dx12(0)
      uy23r(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*dx12(1)
      uz23r(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*dx12(2)

      ux22r(i1,i2,i3,kd)= ux23r(i1,i2,i3,kd)
      uy22r(i1,i2,i3,kd)= uy23r(i1,i2,i3,kd)

!     --- end statement functions

! .............. end statement functions


      ierr=0
      ! write(*,*) 'Inside insbc4'


      ! bcOptionWallNormal= doubleDiv : apply discrete div at -1 and -2
      !                   = divAndDivN : apply div(u)=0 and div(u).n=0
      bcOptionWallNormal=divAndDivN !  doubleDiv ! divAndDivN

      pc                =ipar(0)
      uc                =ipar(1)
      vc                =ipar(2)
      wc                =ipar(3)
      sc                =ipar(4)
      grid              =ipar(5)
      gridType          =ipar(6)
      orderOfAccuracy   =ipar(7)
      gridIsMoving      =ipar(8)
      useWhereMask      =ipar(9)
      gridIsImplicit    =ipar(10)
      implicitMethod    =ipar(11)
      implicitOption    =ipar(12)
      isAxisymmetric    =ipar(13)
      use2ndOrderAD     =ipar(14)
      use4thOrderAD     =ipar(15)
      twilightZone      =ipar(16)
      numberOfProcessors=ipar(17)
      outflowOption     =ipar(18)
      orderOfExtrapolationForOutflow=ipar(19) ! new *wdh* 100827 -- finish me --
      debug             =ipar(20)
      myid              =ipar(21)
      assignTemperature =ipar(22)
      tc                =ipar(23)
      

!     advectPassiveScalar=ipar(16)

      dx(0)             =rpar(0)
      dx(1)             =rpar(1)
      dx(2)             =rpar(2)
      dr(0)             =rpar(3)
      dr(1)             =rpar(4)
      dr(2)             =rpar(5)
      nu                =rpar(6)
      t                 =rpar(7)
      ad21              =rpar(8)
      ad22              =rpar(9)
      ad41              =rpar(10)
      ad42              =rpar(11)
      nuPassiveScalar   =rpar(12)
      adcPassiveScalar  =rpar(13)
      ajs               =rpar(14)
      gravity(0)        =rpar(15)
      gravity(1)        =rpar(16)
      gravity(2)        =rpar(17)
      thermalExpansivity=rpar(18)

      exact             =rpar(19)

      ! for fourth-order dissipation:
      cd42=ad42/(nd**2)
      ! cd42=0. ! for testing
      adCoeff4=0.

      ! For second-order dissipation:
      cd22=ad22/(nd**2)
      adCoeff2=0.

      if( .false. .and. use4thOrderAD.ne.0 .and. t.le.0. )then
        write(*,'(" insbc4: t=",e10.2," use4thOrderAD=",i2," ad41,ad42=",2e10.2," outflowOption=",i2)') t,use4thOrderAD,ad41,ad42,outflowOption
      end if
      if( .false. .and. use2ndOrderAD.ne.0  .and. t.le.0. )then
        write(*,'(" insbc4: t=",e10.2," use2ndOrderAD=",i2," ad21,ad22=",2e10.2)') t,use2ndOrderAD,ad21,ad22
      end if

!       i1=2
!       i2=2
!       i3=0
!       write(*,*) 'insbc4: x,y,u,err = ',x(i1,i2,i3,0),x(i1,i2,i3,1),ogf(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,uc,t),\
!                                     u(i1,i2,i3,uc)-ogf(exact,x(i1,i2,i3,0),x(i1,i2,i3,1),0.,uc,t)

      ! if( t.le.0.0001 .and. gridIsMoving.ne.0 .and. mod(bcOption,2).eq.1 )then
      !   write(*,'("insbc4: *** Moving grids is on ***")')
      ! end if

      if( outflowOption.ne.0 .and. outflowOption.ne.1 )then
        write(*,'("insbc4: ERROR: unexpected outflowOption=",i6)') outflowOption
        stop 1706
      end if

      if( assignTemperature.ne.0 .and. tc.lt.0 .or. tc.gt.10000 )then
        write(*,'("insbc4: ERROR: assignTemperature.ne.0 but tc=",i6)') tc
        stop 1744
      end if


      if( mod(bcOption,2).eq.1  )then

      ! *************************************************************
      ! ********Update ghost pts outside interpolation points********
      ! *************************************************************


      ! We cannot apply the standard BC's to get the points marked 'E' below
      ! where the boundary points of a grid are interpolated
      !  i2=0   ----I----I----X----X----X------------------------
      !  i2=-1  ----E----E----G----G----G
      !  i2=-2  ----E----E----G----G----G

      ! Include ghost points on interpolation boundaries
      do axis=0,2
      do side=0,1
        is=1-2*side
        if( axis.lt.nd .and. bc(side,axis).eq.0 )then
          nr(side,axis)=indexRange(side,axis)-2*is
        else
          nr(side,axis)=indexRange(side,axis)
        end if
      end do
      end do

      ! write(*,'(''*** insbc4 grid='',i4,'' nr='',6i3,'' bc='',6i3)') grid,nr,bc

      do kd1=0,nd-1
      do ks1=0,1
        nr0=nr(0,kd1)  ! save these values
        nr1=nr(1,kd1)
        nr(0,kd1)=indexRange(ks1,kd1)
        nr(1,kd1)=nr(0,kd1)
        bc1=bc(ks1,kd1)
        is=1-2*ks1
        if( bc1.eq.noSlipWall .or. bc1.eq.outflow .or. bc1.eq.inflowWithVelocityGiven )then
   
          ! For now extrapolate these points
          ! We could do better -- on a noSlipWall we could use u.x=0 or v.y=0
          if( kd1.eq.0 )then
            loopse4NoMask(if( mask(i1,i2,i3).lt.0 )then,\
                          $extrapTwoGhost(5,r),\
                          end if,)
          else if( kd1.eq.1 )then
            loopse4NoMask(if( mask(i1,i2,i3).lt.0 )then,\
                          $extrapTwoGhost(5,s),\
                          end if,)
          else
            loopse4NoMask(if( mask(i1,i2,i3).lt.0 )then,\
                          $extrapTwoGhost(5,t),\
                          end if,)
          end if     
        end if
        ! reset
        nr(0,kd1)=nr0
        nr(1,kd1)=nr1
      end do
      end do

      do axis=0,2
      do side=0,1
         nr(side,axis)=indexRange(side,axis)
      end do
      end do

      ! *************************************************************
      ! *****************Update extended boundaries*****************
      ! *************************************************************
      do kd1=0,nd-1
      do ks1=0,1
       bc1=bc(ks1,kd1)
       if( bc1.eq.slipWall .or. bc1.eq.outflow .or. bc1.eq.inflowWithVelocityGiven )then
	! In some cases we may need to assign values on the ghost points on the extended boundary
        ! For a noSlipWall these values are already set (u=0)
        !
        !                |                      |
        !                |                      |
        !      X----X----|----------------------|----X----X
        !                |                      |
        !                |                      |


        nr(0,kd1)=indexRange(ks1,kd1)
        nr(1,kd1)=nr(0,kd1)

	do kd2=0,nd-1
	if( kd2.ne.kd1 )then
	do ks2=0,1
          bc2=bc(ks2,kd2)

          nr(0,kd2)=indexRange(ks2,kd2)
          nr(1,kd2)=nr(0,kd2)
          
          is=1-2*ks2

          if( bc1.eq.slipWall .and. ( bc2.eq.outflow .or. bc2.eq.inflowWithVelocityGiven) )then
            !  On the slip wall ghost points solve for the normal components:
            !       u.x + v.y = 0
            !      D+^p ( n.u ) = 0
		
            !  printf(" Set points (%i,%i,%i),(%i,%i,%i) where slip wall meets outflow\n",
            !                  i1+is1,i2+is2,i3,i1+2*is1,i2+2*is2,i3)
	    
            !  u.x+v.y=0
            !  D+4(u)=0
            if( bc2.eq.outflow .and. outflowOption.eq.neumannAtOuflow )then
            ! kkc 110311 added this adjustment for the special case of neumannAtOutflow
               is1=0
               is2=0
               is3=0
               if( kd2.eq.0 )then
                  is1=is
               else if( kd2.eq.1 )then
                  is2=is
               else
                  is3=is
               end if
               loopse4($boundaryConditionNeumannOutflow(none,2),,,)

            else 
             if( gridType.eq.rectangular )then

              if( nd.eq.2 )then
                if( kd2.eq.0 )then
	          loopse4(u(i1-is,i2,i3,uc)=-1.5*u(i1,i2,i3,uc)+3.*u(i1+is,i2,i3,uc)-.5*u(i1+2*is,i2,i3,uc)\
	                +is*.25*dx(0)*12.*uy42r(i1,i2,i3,vc),\
                          u(i1-2*is,i2,i3,uc)=4.*(u(i1-is,i2,i3,uc)+u(i1+is,i2,i3,uc))-6.*u(i1,i2,i3,uc)-u(i1+2*is,i2,i3,uc),,)
                else 
	          loopse4(u(i1,i2-is,i3,vc)=-1.5*u(i1,i2,i3,vc)+3.*u(i1,i2+is,i3,vc)-.5*u(i1,i2+2*is,i3,vc)\
                          +is*.25*dx(1)*12.*ux42r(i1,i2,i3,uc),\
                          u(i1,i2-2*is,i3,vc)=4.*(u(i1,i2-is,i3,vc)+u(i1,i2+is,i3,vc))-6.*u(i1,i2,i3,vc)-u(i1,i2+2*is,i3,vc),,)
                end if
              else ! 3D
                if( kd2.eq.0 )then
	          loopse4(u(i1-is,i2,i3,uc)=-1.5*u(i1,i2,i3,uc)+3.*u(i1+is,i2,i3,uc)-.5*u(i1+2*is,i2,i3,uc)\
	                +is*.25*dx(0)*12.*(uy43r(i1,i2,i3,vc)+uz43r(i1,i2,i3,wc)),\
                          u(i1-2*is,i2,i3,uc)=4.*(u(i1-is,i2,i3,uc)+u(i1+is,i2,i3,uc))-6.*u(i1,i2,i3,uc)-u(i1+2*is,i2,i3,uc),,)
                else if( kd2.eq.1 )then
	          loopse4(u(i1,i2-is,i3,vc)=-1.5*u(i1,i2,i3,vc)+3.*u(i1,i2+is,i3,vc)-.5*u(i1,i2+2*is,i3,vc)\
	                +is*.25*dx(1)*12.*(ux43r(i1,i2,i3,uc)+uz43r(i1,i2,i3,wc)),\
                         u(i1,i2-2*is,i3,vc)=4.*(u(i1,i2-is,i3,vc)+u(i1,i2+is,i3,vc))-6.*u(i1,i2,i3,vc)-u(i1,i2+2*is,i3,vc),,)
                else
	          loopse4(u(i1,i2,i3-is,wc)=-1.5*u(i1,i2,i3,wc)+3.*u(i1,i2,i3+is,wc)-.5*u(i1,i2,i3+2*is,wc)\
	                +is*.25*dx(2)*12.*(ux43r(i1,i2,i3,uc)+uy43r(i1,i2,i3,vc)),\
                          u(i1,i2,i3-2*is,wc)=4.*(u(i1,i2,i3-is,wc)+u(i1,i2,i3+is,wc))-6.*u(i1,i2,i3,wc)-u(i1,i2,i3+2*is,wc),,)
                end if
              end if

             else ! curvilinear

              extrapOrder=5
 	      if( extrapOrder.eq.5 )then
                extrapolate(5)
              else
	        write(*,*) 'insbc4:ERROR'
                stop 3
              end if

              if( nd.eq.2 )then
               if( kd2.eq.0 )then
                 loopse4($divAndExtrap(r,2),,,)
               else 
                 loopse4($divAndExtrap(s,2),,,)
               end if
              else ! 3d
               if( kd2.eq.0 )then
                 loopse4($divAndExtrap(r,3),,,)
               else if( kd2.eq.1 )then
                 loopse4($divAndExtrap(s,3),,,)
               else
                 loopse4($divAndExtrap(t,3),,,)
               end if
              end if
             end if

            end if ! end if block for neumannAtOutflow option

          else if( (bc1.eq.outflow .and. (bc2.eq.outflow .or. bc2.eq.noSlipWall)) .or. bc1.eq.inflowWithVelocityGiven )then

            ! printf(" Set points (%i,%i,%i),(%i,%i,%i) on outflow extended boundary...\n",
            !     //                 i1+is,i2+is2,i3,i1+2*is1,i2+2*is2,i3)
		
            ! if( bc1.eq.inflowWithVelocityGiven )then
            !  write(*,'('' Set extended inflow boundary, nr='',6i3)') nr
            ! end if

            ! write(*,*) 'Set outflow extended boundary, nr=',nr
            extrapOrder=5
	    if( extrapOrder.eq.5 )then
              extrapolate(5)
            else
	      write(*,*) 'insbc4:ERROR'
              stop 3
            end if
              		
	  else 
          end if
          nr(0,kd2)=indexRange(0,kd2) ! reset
          nr(1,kd2)=indexRange(1,kd2)

        end do
        end if
        end do

        nr(0,kd1)=indexRange(0,kd1) ! reset
        nr(1,kd1)=indexRange(1,kd1)
       end if
      end do
      end do  
      end if ! update extended boundaries

      ! ...Get values outside corners in 2D,3D and edges in 3D using values on the extended boundary
      !      and values in the interior
      !      The corner or edge is labelled as (kd1,ks1),(kd2,ks2)
      if( mod(bcOption/2,2).eq.1 )then
      if( gridType.eq.curvilinear )then
        do axis=0,2
          d14v(axis)=1./(12.*dr(axis))
          d24v(axis)=1./(12.*dr(axis)**2)
        end do
      else
        do axis=0,2
          d14v(axis)=1./(12.*dx(axis))
          d24v(axis)=1./(12.*dx(axis)**2)
        end do
      end if
      do kd1=0,nd-2
      do kd2=kd1+1,nd-1
      do ks1=0,1
      do ks2=0,1

        if( bc(ks1,kd1).gt.0 .and. bc(ks2,kd2).gt.0 )then
          if( .true. )then
            ! new version 
            call inscr4( kd1+1,ks1+1,kd2+1,ks2+1,nd,indexRange,bc,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, \
                      ipar,rpar,u,t,d14v,d24v,ajs,x,rsxy,gridType )
          else
            call inscr( kd1+1,ks1+1,kd2+1,ks2+1,nd,indexRange,bc,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, \
                      u,t,d14v,d24v,ajs,x,rsxy )
          end if
        end if

      end do
      end do
      end do
      end do

      end if ! update corners 


      do axis=0,2
      do side=0,1
         nr(side,axis)=indexRange(side,axis)
      end do
      end do

      ! ***********************************************************
      ! ***********Assign the tangential components****************
      ! ***********************************************************
      if( mod(bcOption/4,2).eq.1 )then

      do axis=0,nd-1
       do kd=0,nd-1
       do side=0,1
         nr(side,kd)=indexRange(side,kd)
	 if( kd.ne.axis .and. 
     &       (bc(side,kd).eq.noSlipWall .or.
     &        bc(side,kd).eq.inflowWithVelocityGiven .or.
     &        bc(side,kd).eq.slipWall) )then

           ! If the adjacent BC is a noSlipWall or inflow or slipWall then we do not need to assign
           ! ghost points on extended boundaries because these have already been assigned (e.g. u=0 for a noSlipWall)
        
           nr(side,kd)=nr(side,kd)+1-2*side   
         end if
       end do
       end do
       do side=0,1

        is=1-2*side
        nr(0,axis)=indexRange(side,axis)
        nr(1,axis)=nr(0,axis)



        useWallBC = bc(side,axis).eq.noSlipWall .or. bc(side,axis).eq.inflowWithVelocityGiven
        useOutflowBC = bc(side,axis).eq.outflow 

        if( .not.useWallBC .and. .not.useOutflowBC .and. bc(side,axis).ne.slipWall .and. \
            bc(side,axis).gt.0 .and. bc(side,axis).ne.dirichletBoundaryCondition .and. \
            bc(side,axis).ne.penaltyBoundaryCondition .and. bc(side,axis).ne.inflowWithPandTV )then
          write(*,*) 'insbc4:ERROR: unknown boundary condition=',bc(side,axis)
          stop 6
        end if


        ! Tangential components:
        !   Wall:
        !     Use equation plus extrapolation
        !   Outflow:
        !     outflowOption=0:
        !       D+D_(t.u(0)) = 0 and ((D+)^6)u(-2) = 0 
        !     outflowOption=1: (*wdh* 100613)
        !       
        !
        !

        if( useOutflowBC .and. outflowOption.eq.neumannAtOuflow )then
          ! Apply a Neumman like condition at outflow (Good for where there might be inflow locally)
          is1=0
          is2=0
          is3=0
          if( axis.eq.0 )then
           is1=is
          else if( axis.eq.1 )then
           is2=is
          else
           is3=is
          end if
          if( t.le.0 .and. debug.gt.3 )then
            if( myid.le.0 )then
              write(*,'("insbc4: apply neumman outflow: side,axis,grid=",3i4," at t=",e10.2)') side,axis,grid,t
            end if
          end if
          if( nd.eq.2 )then
            if( twilightZone.eq.0 )then
              loopse4($boundaryConditionNeumannOutflow(none,2),,,) 
            else
              loopse4($boundaryConditionNeumannOutflow(tz,2),,,) 
            end if
          else
            if( twilightZone.eq.0 )then
              loopse4($boundaryConditionNeumannOutflow(none,3),,,) 
            else
              loopse4($boundaryConditionNeumannOutflow(tz,3),,,) 
            end if
          end if
        end if

        if( gridType.eq.rectangular )then

          if( axis.eq.0 )then
            if( nd.eq.2 )then
              if( twilightZone.eq.0 )then
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrapRectangular(r,none,2),,,)
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                   loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(r,none,2),,,)
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              else
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrapRectangular(r,tz,2),,,)
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(r,tz,2),,,)
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              end if
            else ! nd==3
              if( twilightZone.eq.0 )then
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrapRectangular(r,none,3),,,)
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(r,none,3),,,)
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              else
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrapRectangular(r,tz,3),,,)
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(r,tz,3),,,)
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              end if

            end if

          else if( axis.eq.1 )then
            if( nd.eq.2 )then
              if( twilightZone.eq.0 )then
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrapRectangular(s,none,2),,,)
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(s,none,2),,,)
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              else
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrapRectangular(s,tz,2),,,)
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(s,tz,2),,,)
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              end if
            else ! nd==3
              if( twilightZone.eq.0 )then
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrapRectangular(s,none,3),,,)
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(s,none,3),,,)
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              else
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrapRectangular(s,tz,3),,,)
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(s,tz,3),,,)
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              end if
            end if

          else ! axis==2
            if( twilightZone.eq.0 )then
              if( useWallBC )then
                loopse4($boundaryConditionNavierStokesAndExtrapRectangular(t,none,3),,,)
              else if( useOutflowBC )then
                if( outflowOption.eq.extrapolateOutflow )then
                 loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(t,none,3),,,)
                else if( outflowOption.eq.neumannAtOuflow )then
                  ! done above
                else
                 write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                 stop 5105
                end if
              end if
            else
              if( useWallBC )then
                loopse4($boundaryConditionNavierStokesAndExtrapRectangular(t,tz,3),,,)
              else if( useOutflowBC )then
               if( outflowOption.eq.extrapolateOutflow )then
                loopse4($boundaryCondition2ndDifferenceAndExtrapRectangular(t,tz,3),,,)
               else if( outflowOption.eq.neumannAtOuflow )then
                 ! done above
               else
                write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                stop 5105
               end if
              end if
            end if

          end if

        else ! curvilinear

          ! *************************************************************************
          ! *******************  Curvilinear  ***************************************
          ! *************************************************************************
          if( axis.eq.0 )then
            if( nd.eq.2 )then
              ! Solve
              !   F1(u(-1),u(-2)) = a11.u(-1) + a12.u(-2) + g1 = nu*(u.xx+u.yy) - u*u.x - v*u.y - u.t
              !   F2(u(-1),u(-2)) = a21.u(-1) + a22.u(-2) + g2 = D+^m( u(-2) ) 
              ! for (u(-1),u(-2)) and (v(-1),v(-2))
              ! Then adjust the tangential components
              !    \uv <- \uv + (\uv_old-\uv).nv
              
              ! write(*,*) 'insbc4: curvilinear: assign wall tangential axis=0 wall nr=',nr 
              if( twilightZone.eq.0 )then
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrap2d(r,none),,,)                
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrap(is,0,0,r,none,2),,,)                
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              else
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrap2d(r,tz),,,)                
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrap(is,0,0,r,tz,2),,,)                
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              end if
            else ! nd==3
              if( twilightZone.eq.0 )then
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrap3d(r,none),,,)                
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrap(is,0,0,r,none,3),,,)                
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              else
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrap3d(r,tz),,,)                
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrap(is,0,0,r,tz,3),,,)                
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              end if
            end if

          else if( axis.eq.1 )then
            if( nd.eq.2 )then
              if( twilightZone.eq.0 )then
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrap2d(s,none),,,)                
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrap(0,is,0,s,none,2),,,)                
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              else
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrap2d(s,tz),,,)                
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrap(0,is,0,s,tz,2),,,)                
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              end if
            else ! nd==3
              if( twilightZone.eq.0 )then
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrap3d(s,none),,,)                
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrap(0,is,0,s,none,3),,,)                
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              else
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrap3d(s,tz),,,)                
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrap(0,is,0,s,tz,3),,,)                
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              end if
            end if

          else ! axis==2
              if( twilightZone.eq.0 )then
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrap3d(t,none),,,)                
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrap(0,0,is,t,none,3),,,)                
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              else
                if( useWallBC )then
                  loopse4($boundaryConditionNavierStokesAndExtrap3d(t,tz),,,)                
                else if( useOutflowBC )then
                 if( outflowOption.eq.extrapolateOutflow )then
                  loopse4($boundaryCondition2ndDifferenceAndExtrap(0,0,is,t,tz,3),,,)                
                 else if( outflowOption.eq.neumannAtOuflow )then
                   ! done above
                 else
                  write(*,'("insbc4: unknown outflowOption=",i6)') outflowOption
                  stop 5105
                 end if
                end if
              end if

          end if

        end if ! end if gridType

      end do
      end do
      end if


      ! ***********************************************************
      ! **************Assign the normal component******************
      ! ***********************************************************
      if( mod(bcOption/8,2).eq.1 )then

      

      do axis=0,nd-1
       do kd=0,nd-1
       do side=0,1
         nr(side,kd)=indexRange(side,kd)
	 if( kd.ne.axis .and. 
     &       (bc(side,kd).eq.noSlipWall .or.
     &        bc(side,kd).eq.inflowWithVelocityGiven .or.
     &        bc(side,kd).eq.slipWall) )then

           ! If the adjacent BC is a noSlipWall or inflow or slipWall then we do not need to assign
           ! ghost points on extended boundaries because these have already been assigned (e.g. u=0 for a noSlipWall)
        
           nr(side,kd)=nr(side,kd)+1-2*side   
         end if
       end do
       end do
       do side=0,1

        is=1-2*side
        nr(0,axis)=indexRange(side,axis)
        nr(1,axis)=nr(0,axis)


        if( bc(side,axis).eq.outflow .and. outflowOption.eq.neumannAtOuflow )then
          ! do nothing in this case, Neumann BC's have already been applied above *wdh* 100827

        else if( bc(side,axis).eq.noSlipWall .or. bc(side,axis).eq.inflowWithVelocityGiven .or. \
            bc(side,axis).eq.outflow )then

          ! set 2 ghost lines from div(u)=0

          if( gridType.eq.rectangular )then

            if( axis.eq.0 )then
              if( nd.eq.2 )then
               if( bcOptionWallNormal.eq.doubleDiv )then
                 loopse4(u(i1-  is,i2,i3,uc)=divBCr2d(i1+is),\
                         u(i1-2*is,i2,i3,uc)=divBCr2d(i1),,)
               else
                ! u.x = -v.y
                ! u.xx = -v.xy
                ! write(*,*) 'assign axis==0 wall nr=',nr                
                loopse4(vy=uy42r(i1,i2,i3,vc),\
                        vxy=uxy42r(i1,i2,i3,vc),\
                        u(i1-  is,i2,i3,uc)=3.75*u(i1,i2,i3,uc)-3.*u(i1+is,i2,i3,uc)+.25*u(i1+2*is,i2,i3,uc)\
                                           -1.5*(is*dx(0)*vy+dx(0)**2*vxy),\
                        u(i1-2*is,i2,i3,uc)=30.*u(i1,i2,i3,uc)-32.*u(i1+is,i2,i3,uc)+3.*u(i1+2*is,i2,i3,uc)\
                                           -(24.*is*dx(0)*vy+12.*dx(0)**2*vxy))
               end if
              else ! nd==3
               if( bcOptionWallNormal.eq.doubleDiv )then
                 loopse4(u(i1-  is,i2,i3,uc)=divBCr3d(i1+is),\
                         u(i1-2*is,i2,i3,uc)=divBCr3d(i1),,)
               else
                ! u.x = -v.y-w.z
                ! u.xx = -v.xy-w.xz
                ! write(*,*) 'assign axis==0 wall nr=',nr                
                loopse4(vy=  uy43r(i1,i2,i3,vc)+ uz43r(i1,i2,i3,wc),\
                        vxy=uxy43r(i1,i2,i3,vc)+uxz43r(i1,i2,i3,wc),\
                        u(i1-  is,i2,i3,uc)=3.75*u(i1,i2,i3,uc)-3.*u(i1+is,i2,i3,uc)+.25*u(i1+2*is,i2,i3,uc)\
                                           -1.5*(is*dx(0)*vy+dx(0)**2*vxy),\
                        u(i1-2*is,i2,i3,uc)=30.*u(i1,i2,i3,uc)-32.*u(i1+is,i2,i3,uc)+3.*u(i1+2*is,i2,i3,uc)\
                                           -(24.*is*dx(0)*vy+12.*dx(0)**2*vxy))
               end if

              end if

            else if( axis.eq.1 )then
              if( nd.eq.2 )then
               if( bcOptionWallNormal.eq.doubleDiv )then
                loopse4(u(i1,i2-  is,i3,vc)=divBCs2d(i2+is),\
                        u(i1,i2-2*is,i3,vc)=divBCs2d(i2),,)
               else
                ! write(*,*) 'assign axis==1 wall nr=',nr                
                loopse4(ux=ux42r(i1,i2,i3,uc),\
                        uxy=uxy42r(i1,i2,i3,uc),\
                        u(i1,i2-  is,i3,vc)=3.75*u(i1,i2,i3,vc)-3.*u(i1,i2+is,i3,vc)+.25*u(i1,i2+2*is,i3,vc)\
                                           -1.5*(is*dx(1)*ux+dx(1)**2*uxy),\
                        u(i1,i2-2*is,i3,vc)=30.*u(i1,i2,i3,vc)-32.*u(i1,i2+is,i3,vc)+3.*u(i1,i2+2*is,i3,vc)\
                                           -(24.*is*dx(1)*ux+12.*dx(1)**2*uxy))
               end if
              else ! nd==3
               if( bcOptionWallNormal.eq.doubleDiv )then
                loopse4(u(i1,i2-  is,i3,vc)=divBCs3d(i2+is),\
                        u(i1,i2-2*is,i3,vc)=divBCs3d(i2),,)
               else
                ! v.y  = -u.x-w.z
                ! v.yy = -u.xy-w.yz
                ! write(*,*) 'assign axis==1 wall nr=',nr                
                loopse4(ux=  ux43r(i1,i2,i3,uc)+ uz43r(i1,i2,i3,wc),\
                        uxy=uxy43r(i1,i2,i3,uc)+uyz43r(i1,i2,i3,wc),\
                        u(i1,i2-  is,i3,vc)=3.75*u(i1,i2,i3,vc)-3.*u(i1,i2+is,i3,vc)+.25*u(i1,i2+2*is,i3,vc)\
                                           -1.5*(is*dx(1)*ux+dx(1)**2*uxy),\
                        u(i1,i2-2*is,i3,vc)=30.*u(i1,i2,i3,vc)-32.*u(i1,i2+is,i3,vc)+3.*u(i1,i2+2*is,i3,vc)\
                                           -(24.*is*dx(1)*ux+12.*dx(1)**2*uxy))
               end if
              end if

            else ! axis==2
               if( bcOptionWallNormal.eq.doubleDiv )then
                 loopse4(u(i1,i2,i3,wc-  is)=divBCt3d(i3+is),\
                         u(i1,i2,i3,wc-2*is)=divBCt3d(i3),,)
               else
                ! w.z = -u.x-v.y
                ! w.zz =-u.xz-v.yz 
                ! write(*,*) 'assign axis==0 wall nr=',nr                
                loopse4(vy=  ux43r(i1,i2,i3,uc)+ uy43r(i1,i2,i3,vc),\
                        vxy=uxz43r(i1,i2,i3,uc)+uyz43r(i1,i2,i3,vc),\
                        u(i1,i2,i3-  is,wc)=3.75*u(i1,i2,i3,wc)-3.*u(i1,i2,i3+is,wc)+.25*u(i1,i2,i3+2*is,wc)\
                                           -1.5*(is*dx(2)*vy+dx(2)**2*vxy),\
                        u(i1,i2,i3-2*is,wc)=30.*u(i1,i2,i3,wc)-32.*u(i1,i2,i3+is,wc)+3.*u(i1,i2,i3+2*is,wc)\
                                           -(24.*is*dx(2)*vy+12.*dx(2)**2*vxy))
               end if

            end if

          else ! curvilinear

            ! *************************************************************************
            ! *******************  Curvilinear  ***************************************
            ! *************************************************************************
            if( axis.eq.0 )then
              if( nd.eq.2 )then
               if( bcOptionWallNormal.eq.doubleDiv )then
                !* loopse4(u(i1-  is,i2,i3,uc)=divBCr2d(i1+is),\
                !*         u(i1-2*is,i2,i3,uc)=divBCr2d(i1),,)
               else
                ! F1(uv(-1),uv(-2)) = a11.uv(-1) + a12.uv(-2) + g1 = div(u) = rx*ur+sx*us + ry*vr+sy*vs
                ! F2(uv(-1),uv(-2)) = a21.uv(-1) + a22.uv(-2) + g2 = div(u).r =rx*u.rr+rx.r*u.r+...
                !   Choose  uv(-1) =  uv_old(-1) + alpha*(rx,ry)
                !           uv(-2) =  uv_old(-2) + beta *(rx,ry)
                ! So that F1=0 and F2=0 (note: (rx,ry) is parallel to the normal
                !   -> solve for 
                !    a11.(rx,ry)*alpha + a12.(rx,ry)*beta + F1(\uv_old) = 0 
                !    a21.(rx,ry)*alpha + a22.(rx,ry)*beta + F2(\uv_old) = 0 
                
                ! write(*,*) 'insbc4: curvilinear: assign wall normal axis=0 wall nr=',nr   
                loopse4($boundaryConditionDivAndDivN(r),,,)                

               end if
              else ! nd==3
               if( bcOptionWallNormal.eq.doubleDiv )then
                 !* loopse4(u(i1-  is,i2,i3,uc)=divBCr3d(i1+is),\
                 !*         u(i1-2*is,i2,i3,uc)=divBCr3d(i1),,)
               else
                ! u.x = -v.y-w.z
                ! u.xx = -v.xy-w.xz
                ! write(*,*) 'assign axis==0 wall nr=',nr                

                loopse4($boundaryConditionDivAndDivN3d(r),,,)
               end if

              end if

            else if( axis.eq.1 )then
              if( nd.eq.2 )then
               if( bcOptionWallNormal.eq.doubleDiv )then
                !* loopse4(u(i1,i2-  is,i3,vc)=divBCs2d(i2+is),\
                !*         u(i1,i2-2*is,i3,vc)=divBCs2d(i2),,)
               else
                loopse4($boundaryConditionDivAndDivN(s),,,)
               end if
              else ! nd==3
               if( bcOptionWallNormal.eq.doubleDiv )then
                !* loopse4(u(i1,i2-  is,i3,vc)=divBCs3d(i2+is),\
                !*         u(i1,i2-2*is,i3,vc)=divBCs3d(i2),,)
               else
                ! v.y  = -u.x-w.z
                ! v.yy = -u.xy-w.yz
                loopse4($boundaryConditionDivAndDivN3d(s),,,)
               end if
              end if

            else ! axis==2
               if( bcOptionWallNormal.eq.doubleDiv )then
                !*  loopse4(u(i1,i2,i3,wc-  is)=divBCt3d(i3+is),\
                !*          u(i1,i2,i3,wc-2*is)=divBCt3d(i3),,)
               else
                ! w.z = -u.x-v.y
                ! w.zz =-u.xz-v.yz 
                ! write(*,*) 'assign axis==0 wall nr=',nr                
                loopse4($boundaryConditionDivAndDivN3d(t),,,)
               end if

            end if


          end if

        else if( bc(side,axis).ne.slipWall .and. bc(side,axis).gt.0 .and. bc(side,axis).ne.dirichletBoundaryCondition .and. bc(side,axis).ne.penaltyBoundaryCondition .and. bc(side,axis).ne.inflowWithPandTV )then

          write(*,*) 'insbc4:ERROR: unknown boundary condition=',bc(side,axis)
          stop 6
        end if

      end do
      end do
      end if

      return
      end


      subroutine inscr( kd1,ks1,kd2,ks2,nd,gridIndexRange,bc,
     & ndra,ndrb,ndsa,ndsb,ndta,ndtb,u,t,d14,d24,ajs,xy,rsxy )
!======================================================================
!      Get Values for u outside corners in 2D or Edges in 3D
!
!  Input -
!   (kd1,ks1),(kd2,ks2) : defines the corner or edge
!    u :
!
! NOTE: This approximation is 4th order accurate but NOT exact for 4th degree polynomials
! NEW NOTE: the new version is exact for 4th degree polynomials
!
!  Corners are labelled (in 2d) as (kd,ks)=
!
!           (1,2)          (2,2)
!                +--------+
!                |        |
!                |        |
!                +--------+
!           (1,1)          (2,1)
!
!  To get the value at the corner use:
! u(r,s) = u(0) + r*u.r(0) + s*u.s(0) + .5*r**2*u.rr(0) + ...
!  which implies
! u(r,s)+u(-r,-s) = 2u(0) + r**2*u.rr+2r*s*u.rs(0)+s**2*u.ss(0)+O(h**4)
!  At a corner we know u(0), and all non mixed derivatives, u.r, u.s,
! u.rr, u.ss, ...
!   To get u.rs and v.rs we use u.x+v.y=0 and (u.x+v.y).r=0 and
!   (u.x+v.y).s=0, which gives
!
! (r.y*s.x-r.x*s.y)u.rs + r.y*r.x*u.rr - s.y*s.x*u.ss
!       +r.y**2*v.rr - s.y**2*v.ss + r.y a_1 - s.y a_2 = 0
! (r.x*s.y-r.y*s.x)v.rs + r.x*r.x*u.rr - s.x*s.x*u.ss
!       +r.x*r.y*v.rr - s.x*s.y*v.ss + r.x a_1 - s.x a_2 = 0
!  a_1 = (r.x).r*u.r + (s.x).r*u.s + (r.y).r*v.r + (s.y).r*v.s
!  a_2 = (r.x).s*u.r + (s.x).s*u.s + (r.y).s*v.r + (s.y).s*v.s
!
!
!  In 3D, for an edge parallel to "t", we use
!
! u(r,s)+u(-r,-s) = 2u(0) + r**2*u.rr+2r*s*u.rs(0)+s**2*u.ss(0)+O(h**4)
!
! and to get (u,v,w)_rs we use
!
!       (u_x+v_y+w_z)_r=0
!       (u_x+v_y+w_z)_s=0
!       Extrapolate( (t_1,t_2,t_3).(u,v,w))
! where (t_1,t_2,t_3) is the tangent to the edge.
!
!
!======================================================================
      implicit none
      integer kd1,ks1,kd2,ks2,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb
      real t,ajs
      real u(ndra:ndrb,ndsa:ndsb,ndta:ndtb,0:*),d14(3),d24(3),
     &    xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd),
     &  rsxy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd,nd)
      integer gridIndexRange(2,3), bc(2,3)
!.......local
      real vr(3,3),vrr(3,3,3),drs(3)
      integer iv(3),is(3)
      logical period,oldway
      integer nrsab,nrs
      integer kd,kdd,kd3,i1,i2,i3,is1,is2,is3,kdn,ks,j1,j2,j3,
     & i11,i12,i21,i22,i31,i32,js3

      real uc,uc0,uc33,uc32,uc31,ubr,ubs,ubt,ubrr,ubss,ubtt,ubrs,ubrt,
     & ubst,uv3,rx,ry,rz,sx,sy,sz,tx,ty,tz
      real taylor2d1,taylor2d2,uc2d11,uc2d21,uc2d12,uc2d22
      real taylor3d3e1,taylor3d3e2,taylor3d2e1,taylor3d2e2,
     &     taylor3d1e1,taylor3d1e2,uc3d3e11,uc3d3e22,
     &     uc3d2e11,uc3d2e22,uc3d1e11,uc3d1e22,
     &     taylor3d1,taylor3d2,uc3d111,uc3d222

!c      include 'cgins1.h'
!c      include 'cgins.h'
!.......start statement functions
!       equation to get values outside corners:
!       corner = (i1,i2), point=(i1-is1,i2-is2)
      nrsab(kd,ks) = gridIndexRange(ks,kd)
      nrs(kd,ks)=gridIndexRange(ks,kd)

      period(kd) = bc(1,kd).lt.0

      uc(is1,is2,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1+is1,i2+is2,i3,kd)
     &            +(is1*drs(1))**2*vrr(kd,1,1)
     & +2.*(is1*drs(1)*is2*drs(2))*vrr(kd,1,2)
     &            +(is2*drs(2))**2*vrr(kd,2,2)

! Here are more accurate expressions for 2D -- exact for 4th order polys
      taylor2d1(is1,is2,i1,i2,i3,kd)=(is1*drs(1))*vr(kd,1)
     &                       +(is2*drs(2))*vr(kd,2)
      taylor2d2(is1,is2,i1,i2,i3,kd)=.5*(is1*drs(1))**2*vrr(kd,1,1)
     &                  +(is1*drs(1)*is2*drs(2))*vrr(kd,1,2)
     &                       +.5*(is2*drs(2))**2*vrr(kd,2,2)

      uc2d11(is1,is2,i1,i2,i3,kd)= ! for u(-1,-1) 
     &   3.75*u(i1,i2,i3,kd)  
     &    -3.*u(i1+is1,i2+is2,i3,kd)
     &   +.25*u(i1+2*is1,i2+2*is2,i3,kd)
     &  +1.5*taylor2d1(is1,is2,i1,i2,i3,kd)
     &  + 3.*taylor2d2(is1,is2,i1,i2,i3,kd)

      uc2d21(is1,is2,i1,i2,i3,kd)=uc2d11(2*is1,  is2,i1,i2,i3,kd) ! for u(-2,-1) 
      uc2d12(is1,is2,i1,i2,i3,kd)=uc2d11(  is1,2*is2,i1,i2,i3,kd) ! for u(-2,-1) 

      uc2d22(is1,is2,i1,i2,i3,kd)= ! for u(-2,-2) 
     &    30.*u(i1,i2,i3,kd)  
     &   -32.*u(i1+is1,i2+is2,i3,kd)
     &    +3.*u(i1+2*is1,i2+2*is2,i3,kd)
     &  +24.*taylor2d1(is1,is2,i1,i2,i3,kd)
     &  +24.*taylor2d2(is1,is2,i1,i2,i3,kd)

      uc0(is1,is2,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1+is1,i2+is2,i3,kd)

!     --- old 3d ----
      uc33(is1,is2,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1+is1,i2+is2,i3,kd)
     &            +(is1*drs(1))**2*vrr(kd,1,1)
     & +2.*(is1*drs(1)*is2*drs(2))*vrr(kd,1,2)
     &            +(is2*drs(2))**2*vrr(kd,2,2)
      uc32(is1,is3,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1+is1,i2,i3+is3,kd)
     &            +(is1*drs(1))**2*vrr(kd,1,1)
     & +2.*(is1*drs(1)*is3*drs(3))*vrr(kd,1,3)
     &            +(is3*drs(3))**2*vrr(kd,3,3)
      uc31(is2,is3,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1,i2+is2,i3+is3,kd)
     &            +(is2*drs(2))**2*vrr(kd,2,2)
     & +2.*(is2*drs(2)*is3*drs(3))*vrr(kd,2,3)
     &            +(is3*drs(3))**2*vrr(kd,3,3)

!  Here are more accurate expressions for 3D -- exact for 4th order polynomials
      taylor3d3e1(is1,is2,i1,i2,i3,kd)=       ! 3d, edge along direction 3, 1st derivative term in Taylor series
     &     (is1*drs(1))*vr(kd,1)
     &    +(is2*drs(2))*vr(kd,2)              
      taylor3d3e2(is1,is2,i1,i2,i3,kd)=       ! 3d, edge along direction 3, 2nd derivative term in Taylor series
     &          .5*(is1*drs(1))**2*vrr(kd,1,1)
     &    +(is1*drs(1)*is2*drs(2))*vrr(kd,1,2)
     &         +.5*(is2*drs(2))**2*vrr(kd,2,2)

      taylor3d2e1(is1,is3,i1,i2,i3,kd)=       ! 3d, edge along direction 2, 1st derivative term in Taylor series
     &     (is1*drs(1))*vr(kd,1)
     &    +(is3*drs(3))*vr(kd,3)              
      taylor3d2e2(is1,is3,i1,i2,i3,kd)=       ! 3d, edge along direction 2, 2nd derivative term in Taylor series
     &          .5*(is1*drs(1))**2*vrr(kd,1,1)
     &    +(is1*drs(1)*is3*drs(3))*vrr(kd,1,3)
     &         +.5*(is3*drs(3))**2*vrr(kd,3,3)

      taylor3d1e1(is2,is3,i1,i2,i3,kd)=       ! 3d, edge along direction 1, 1st derivative term in Taylor series
     &     (is2*drs(2))*vr(kd,2)
     &    +(is3*drs(3))*vr(kd,3)              
      taylor3d1e2(is2,is3,i1,i2,i3,kd)=       ! 3d, edge along direction 1, 2nd derivative term in Taylor series
     &          .5*(is2*drs(2))**2*vrr(kd,2,2)
     &    +(is2*drs(2)*is3*drs(3))*vrr(kd,2,3)
     &         +.5*(is3*drs(3))**2*vrr(kd,3,3)

      uc3d3e11(is1,is2,i1,i2,i3,kd)=           !  3d, edge along direction 3,for u(-1,-1,*) 
     &   3.75*u(i1,i2,i3,kd)  
     &    -3.*u(i1+is1,i2+is2,i3,kd)
     &   +.25*u(i1+2*is1,i2+2*is2,i3,kd)
     &  +1.5*taylor3d3e1(is1,is2,i1,i2,i3,kd)
     &  + 3.*taylor3d3e2(is1,is2,i1,i2,i3,kd)

      uc3d3e22(is1,is2,i1,i2,i3,kd)=           ! 3d, edge along direction 3, for u(-2,-2,*) 
     &    30.*u(i1,i2,i3,kd)  
     &   -32.*u(i1+is1,i2+is2,i3,kd)
     &    +3.*u(i1+2*is1,i2+2*is2,i3,kd)
     &  +24.*taylor3d3e1(is1,is2,i1,i2,i3,kd)
     &  +24.*taylor3d3e2(is1,is2,i1,i2,i3,kd)

      uc3d2e11(is1,is3,i1,i2,i3,kd)=           !  3d, edge along direction 2,for u(-1,*,-1) 
     &   3.75*u(i1,i2,i3,kd)  
     &    -3.*u(i1+is1,i2,i3+is3,kd)
     &   +.25*u(i1+2*is1,i2,i3+2*is3,kd)
     &  +1.5*taylor3d2e1(is1,is3,i1,i2,i3,kd)
     &  + 3.*taylor3d2e2(is1,is3,i1,i2,i3,kd)

      uc3d2e22(is1,is3,i1,i2,i3,kd)=           ! 3d, edge along direction 2, for u(-2,*,-2) 
     &    30.*u(i1,i2,i3,kd)  
     &   -32.*u(i1+is1,i2,i3+is3,kd)
     &    +3.*u(i1+2*is1,i2,i3+2*is3,kd)
     &  +24.*taylor3d2e1(is1,is3,i1,i2,i3,kd)
     &  +24.*taylor3d2e2(is1,is3,i1,i2,i3,kd)

      uc3d1e11(is2,is3,i1,i2,i3,kd)=           !  3d, edge along direction 1,for u(*,-1,-1) 
     &   3.75*u(i1,i2,i3,kd)  
     &    -3.*u(i1,i2+is2,i3+is3,kd)
     &   +.25*u(i1,i2+2*is2,i3+2*is3,kd)
     &  +1.5*taylor3d1e1(is2,is3,i1,i2,i3,kd)
     &  + 3.*taylor3d1e2(is2,is3,i1,i2,i3,kd)

      uc3d1e22(is2,is3,i1,i2,i3,kd)=           ! 3d, edge along direction 1, for u(*,-2,-2) 
     &    30.*u(i1,i2,i3,kd)  
     &   -32.*u(i1,i2+is2,i3+is3,kd)
     &    +3.*u(i1,i2+2*is2,i3+2*is3,kd)
     &  +24.*taylor3d1e1(is2,is3,i1,i2,i3,kd)
     &  +24.*taylor3d1e2(is2,is3,i1,i2,i3,kd)




!.......parametric derivatives on the boundary used by uv3(is1,is2,...)
      ubr(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))
     &                    -(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(1)
      ubs(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))
     &                    -(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*d14(2)
      ubt(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))
     &                    -(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*d14(3)
      ubrr(i1,i2,i3,kd)=
     & ( -30.*u(i1,i2,i3,kd)
     &  +16.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))
     &      -(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*d24(1)
      ubss(i1,i2,i3,kd)=
     &+( -30.*u(i1,i2,i3,kd)
     &  +16.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))
     &      -(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*d24(2)
      ubtt(i1,i2,i3,kd)=
     &+( -30.*u(i1,i2,i3,kd)
     &  +16.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))
     &      -(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*d24(3)
      ubrs(i1,i2,i3,kd)=
     &   (8.*(ubs(i1+1,i2,i3,kd)-ubs(i1-1,i2,i3,kd))
     &      -(ubs(i1+2,i2,i3,kd)-ubs(i1-2,i2,i3,kd)))*d14(1)
      ubrt(i1,i2,i3,kd)=
     &   (8.*(ubt(i1+1,i2,i3,kd)-ubt(i1-1,i2,i3,kd))
     &      -(ubt(i1+2,i2,i3,kd)-ubt(i1-2,i2,i3,kd)))*d14(1)
      ubst(i1,i2,i3,kd)=
     &   (8.*(ubt(i1,i2+1,i3,kd)-ubt(i1,i2-1,i3,kd))
     &      -(ubt(i1,i2+2,i3,kd)-ubt(i1,i2-2,i3,kd)))*d14(2)
!.........................................................
!        Values outside of a vertex in 3D:
!.........................................................
!    ** old **
      uv3(is1,is2,is3,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1+is1,i2+is2,i3+is3,kd)
     &            +(is1*drs(1))**2*ubrr(i1,i2,i3,kd)
     &            +(is2*drs(2))**2*ubss(i1,i2,i3,kd)
     &            +(is3*drs(3))**2*ubtt(i1,i2,i3,kd)
     & +2.*(is1*drs(1)*is2*drs(2))*ubrs(i1,i2,i3,kd)
     & +2.*(is1*drs(1)*is3*drs(3))*ubrt(i1,i2,i3,kd)
     & +2.*(is2*drs(2)*is3*drs(3))*ubst(i1,i2,i3,kd)

!   ** new **
      taylor3d1(is1,is2,is3,i1,i2,i3,kd)=       ! 3d, full 1st derivative term in Taylor series
     &     (is1*drs(1))*ubr(i1,i2,i3,kd)
     &    +(is2*drs(2))*ubs(i1,i2,i3,kd)
     &    +(is3*drs(3))*ubt(i1,i2,i3,kd)
      taylor3d2(is1,is2,is3,i1,i2,i3,kd)=           ! 3d, full 2nd derivative in Taylor series
     &          .5*(is1*drs(1))**2*ubrr(i1,i2,i3,kd)
     &         +.5*(is2*drs(2))**2*ubss(i1,i2,i3,kd)
     &         +.5*(is3*drs(3))**2*ubtt(i1,i2,i3,kd)
     &    +(is1*drs(1)*is2*drs(2))*ubrs(i1,i2,i3,kd)
     &    +(is1*drs(1)*is3*drs(3))*ubrt(i1,i2,i3,kd)
     &    +(is2*drs(2)*is3*drs(3))*ubst(i1,i2,i3,kd)

      uc3d111(is1,is2,is3,i1,i2,i3,kd)= ! for u(-1,-1,-1), u(-2,-1,-1), u(-1,-2,-1), u(-1,-1,-2)
     &   3.75*u(i1,i2,i3,kd)  
     &    -3.*u(i1+is1,i2+is2,i3+is3,kd)
     &   +.25*u(i1+2*is1,i2+2*is2,i3+2*is3,kd)
     &  +1.5*taylor3d1(is1,is2,is3,i1,i2,i3,kd)
     &  + 3.*taylor3d2(is1,is2,is3,i1,i2,i3,kd)

      uc3d222(is1,is2,is3,i1,i2,i3,kd)= !   3d for u(-2,-2,-2) 
     &    30.*u(i1,i2,i3,kd)  
     &   -32.*u(i1+is1,i2+is2,i3+is3,kd)
     &    +3.*u(i1+2*is1,i2+2*is2,i3+2*is3,kd)
     &  +24.*taylor3d1(is1,is2,is3,i1,i2,i3,kd)
     &  +24.*taylor3d2(is1,is2,is3,i1,i2,i3,kd)

      rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)

!.......end  statement functions
!........Interpolate corners of u
!  (i1,i2) is the corner
!  (is1,is2) is in the normal direction into the domain
!
!                |  |  |
!                |  |  |
!                |  |  X------
!                |  +------        X=(i1,i2)
!                +---------
!

      kd3=min(nd,3)

      oldway=.false. ! .true.

!*** do this some where else ****
      do kdd=1,nd
        drs(kdd)=1./(nrsab(kdd,2)-nrsab(kdd,1))
      end do

      if( nd.eq.2 )then
!         here we assume (kd1,kd2)=(1,2)
        i1=nrsab(kd1,ks1)
        i2=nrsab(kd2,ks2)
        i3=nrsab(3,1)
        is1=3-2*ks1
        is2=3-2*ks2
!       ...get derivatives at corner
!          u.r,u.s,u.t;  u.rr, u.ss, u.tt, u.rs
          call insbv( i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,t,vr,vrr,
     &     d14,d24,drs,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb, xy,rsxy,u )
          do kdd=1,nd
             if( oldway )then
             u(i1-  is1,i2-  is2,i3,kdd)=uc(  is1,  is2,i1,i2,i3,kdd)
             u(i1-2*is1,i2-  is2,i3,kdd)=uc(2*is1,  is2,i1,i2,i3,kdd)
             u(i1-  is1,i2-2*is2,i3,kdd)=uc(  is1,2*is2,i1,i2,i3,kdd)
             u(i1-2*is1,i2-2*is2,i3,kdd)=uc(2*is1,2*is2,i1,i2,i3,kdd)
!           here is the new, more accurate way:
            else
            u(i1-  is1,i2-  is2,i3,kdd)=uc2d11(is1,is2,i1,i2,i3,kdd)
            u(i1-2*is1,i2-  is2,i3,kdd)=uc2d21(is1,is2,i1,i2,i3,kdd)
            u(i1-  is1,i2-2*is2,i3,kdd)=uc2d12(is1,is2,i1,i2,i3,kdd)
            u(i1-2*is1,i2-2*is2,i3,kdd)=uc2d22(is1,is2,i1,i2,i3,kdd)
            end if
          end do

      else
!       ************* 3D ************
        iv(1)=0
        iv(2)=0
        iv(3)=0
        iv(kd1)=nrsab(kd1,ks1)
        iv(kd2)=nrsab(kd2,ks2)
        if( kd1+kd2.eq.5 )then
          kdn=1
        elseif( kd1+kd2.eq.4 )then
          kdn=2
        else
          kdn=3
        end if
        is(kd1)=3-2*ks1
        is(kd2)=3-2*ks2
        is(kdn)=0
        i1=iv(1)
        i2=iv(2)
        i3=iv(3)
        is1=is(1)
        is2=is(2)
        is3=is(3)
*         write(*,*) 'INSCR: kdn,is1,is2,is3=',kdn,is1,is2,is3
        if( kdn.eq.3 )then
!           kdn=3 is the direction tangential to the edge
          do 320 i3=nrs(3,1),nrs(3,2)
!       ...get derivatives along an edge
!          u.r,u.s,u.t;  u.rr, u.ss, u.tt, u.rs
             call insbv( i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,t,
     &        vr,vrr,d14,d24,drs,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
     &        xy,rsxy,u )
            do 320 kdd=1,nd
              if( oldway )then
               u(i1-  is1,i2-  is2,i3,kdd)=uc33(  is1,  is2,i1,i2,i3,kdd)
               u(i1-2*is1,i2-  is2,i3,kdd)=uc33(2*is1,  is2,i1,i2,i3,kdd)
               u(i1-  is1,i2-2*is2,i3,kdd)=uc33(  is1,2*is2,i1,i2,i3,kdd)
               u(i1-2*is1,i2-2*is2,i3,kdd)=uc33(2*is1,2*is2,i1,i2,i3,kdd)
              else
!              new:
              u(i1-  is1,i2-  is2,i3,kdd)=
     &                    uc3d3e11(  is1,  is2,i1,i2,i3,kdd)
              u(i1-2*is1,i2-  is2,i3,kdd)=
     &                    uc3d3e11(2*is1,  is2,i1,i2,i3,kdd)
              u(i1-  is1,i2-2*is2,i3,kdd)=
     &                    uc3d3e11(  is1,2*is2,i1,i2,i3,kdd)
              u(i1-2*is1,i2-2*is2,i3,kdd)=
     &                    uc3d3e22(  is1,  is2,i1,i2,i3,kdd)
              end if
            continue
 320      continue
          if( period(3) )then
!           ...swap periodic edges
            i31=nrsab(3,1)
            i32=nrsab(3,2)
            do 340 j2=i2-2*is2,i2-is2,is2
              do 340 j1=i1-2*is1,i1-is1,is1
                do 340 kd=1,nd
                  u(j1,j2,i31-1,kd)=u(j1,j2,i32-1,kd)
                  u(j1,j2,i31-2,kd)=u(j1,j2,i32-2,kd)
                  u(j1,j2,i32  ,kd)=u(j1,j2,i31  ,kd)
                  u(j1,j2,i32+1,kd)=u(j1,j2,i31+1,kd)
                  u(j1,j2,i32+2,kd)=u(j1,j2,i31+1,kd)
                continue
              continue
 340        continue
          else
!
!           ...assign values outside vertices in 3D
!              use Taylor series (derivatives u.rr, u.rs ... are known)
!                u(-r)=2*u(0)-u(r)+ r**2u.rr+...
!
            do ks=1,2
              if( bc(ks,3).gt.0 )then
                i3=nrsab(3,ks)
                if( .not.oldway )then
                is3=3-2*ks
                do kdd=1,nd
                  u(i1-  is1,i2-  is2,i3-  is3,kdd)=
     &                    uc3d111(  is1,  is2,  is3,i1,i2,i3,kdd)
                  u(i1-2*is1,i2-  is2,i3-  is3,kdd)=
     &                    uc3d111(2*is1,  is2,  is3,i1,i2,i3,kdd)
                  u(i1-  is1,i2-2*is2,i3-  is3,kdd)=
     &                    uc3d111(  is1,2*is2,  is3,i1,i2,i3,kdd)
                  u(i1-  is1,i2-  is2,i3-2*is3,kdd)=
     &                    uc3d111(  is1,  is2,2*is3,i1,i2,i3,kdd)
                  u(i1-2*is1,i2-2*is2,i3-  is3,kdd)=
     &                    uc3d111(2*is1,2*is2,  is3,i1,i2,i3,kdd)
                  u(i1-  is1,i2-2*is2,i3-2*is3,kdd)=
     &                    uc3d111(  is1,2*is2,2*is3,i1,i2,i3,kdd)
                  u(i1-2*is1,i2-  is2,i3-2*is3,kdd)=
     &                    uc3d111(2*is1,  is2,2*is3,i1,i2,i3,kdd)
                  u(i1-2*is1,i2-2*is2,i3-2*is3,kdd)=
     &                    uc3d222(  is1,  is2,  is3,i1,i2,i3,kdd)
                end do
                else
                  ! old way
                 js3=3-2*ks
                 do j3=i3-2*js3,i3-js3,js3
                 do j2=i2-2*is2,i2-is2,is2
                 do j1=i1-2*is1,i1-is1,is1
                 do kdd=1,nd
                   u(j1,j2,j3,kdd)=uv3(i1-j1,i2-j2,i3-j3,i1,i2,i3,kdd)
                 end do
                 end do
                 end do
                 end do
                 end if
              end if
            end do
          end if
*             if( i3.gt.nrsab(3,1).and.i3.lt.nrsab(3,2) )then
*               write(*,9500) j1,j2,j3,(u(j1,j2,j3,kdd),kdd=1,nd),
*      &         ue(j1,j2,j3),ve(j1,j2,j3),we(j1,j2,j3)
*             end if
*  9500 format(' j1,j2,j3=',3i3,' uc=',3e8.2,' ue=',3e8.2)

        elseif( kdn.eq.2 )then

          do 420 i2=nrs(2,1),nrs(2,2)
            call insbv( i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,t,
     &        vr,vrr,d14,d24,drs,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
     &        xy,rsxy,u )
            do 420 kdd=1,nd
              if( oldway )then
              u(i1-  is1,i2,i3-  is3,kdd)=uc32(  is1,  is3,i1,i2,i3,kdd)
              u(i1-2*is1,i2,i3-  is3,kdd)=uc32(2*is1,  is3,i1,i2,i3,kdd)
              u(i1-  is1,i2,i3-2*is3,kdd)=uc32(  is1,2*is3,i1,i2,i3,kdd)
              u(i1-2*is1,i2,i3-2*is3,kdd)=uc32(2*is1,2*is3,i1,i2,i3,kdd)
              else
!              new
              u(i1-  is1,i2,i3-  is3,kdd)=
     &                    uc3d2e11(  is1,  is3,i1,i2,i3,kdd)
              u(i1-2*is1,i2,i3-  is3,kdd)=
     &                    uc3d2e11(2*is1,  is3,i1,i2,i3,kdd)
              u(i1-  is1,i2,i3-2*is3,kdd)=
     &                    uc3d2e11(  is1,2*is3,i1,i2,i3,kdd)
              u(i1-2*is1,i2,i3-2*is3,kdd)=
     &                    uc3d2e22(  is1,  is3,i1,i2,i3,kdd)
            end if
            continue
 420      continue
          if( period(2) )then
!           ...swap periodic edges
            i21=nrsab(2,1)
            i22=nrsab(2,2)
            do 440 j3=i3-2*is3,i3-is3,is3
              do 440 j1=i1-2*is1,i1-is1,is1
                do 440 kd=1,nd
                  u(j1,i21-1,j3,kd)=u(j1,i22-1,j3,kd)
                  u(j1,i21-2,j3,kd)=u(j1,i22-2,j3,kd)
                  u(j1,i22  ,j3,kd)=u(j1,i21  ,j3,kd)
                  u(j1,i22+1,j3,kd)=u(j1,i21+1,j3,kd)
                  u(j1,i22+2,j3,kd)=u(j1,i21+1,j3,kd)
 440        continue
          end if

        else ! kdn==1

          do 520 i1=nrs(1,1),nrs(1,2)
            call insbv( i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,t,
     &        vr,vrr,d14,d24,drs,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
     &        xy,rsxy,u )
            do 520 kdd=1,nd
              if( oldway )then
              u(i1,i2-  is2,i3-  is3,kdd)=uc31(  is2,  is3,i1,i2,i3,kdd)
              u(i1,i2-2*is2,i3-  is3,kdd)=uc31(2*is2,  is3,i1,i2,i3,kdd)
              u(i1,i2-  is2,i3-2*is3,kdd)=uc31(  is2,2*is3,i1,i2,i3,kdd)
              u(i1,i2-2*is2,i3-2*is3,kdd)=uc31(2*is2,2*is3,i1,i2,i3,kdd)
              else
!              new
              u(i1,i2-  is2,i3-  is3,kdd)=
     &                    uc3d1e11(  is2,  is3,i1,i2,i3,kdd)
              u(i1,i2-2*is2,i3-  is3,kdd)=
     &                    uc3d1e11(2*is2,  is3,i1,i2,i3,kdd)
              u(i1,i2-  is2,i3-2*is3,kdd)=
     &                    uc3d1e11(  is2,2*is3,i1,i2,i3,kdd)
              u(i1,i2-2*is2,i3-2*is3,kdd)=
     &                    uc3d1e22(  is2,  is3,i1,i2,i3,kdd)
              endif
            continue
 520      continue
          if( period(1) )then
!           ...swap periodic edges
            i11=nrsab(1,1)
            i12=nrsab(1,2)
            do 540 j3=i3-2*is3,i3-is3,is3
              do 540 j2=i2-2*is2,i2-is2,is2
                do 540 kd=1,nd
                  u(i11-1,j2,j3,kd)=u(i12-1,j2,j3,kd)
                  u(i11-2,j2,j3,kd)=u(i12-2,j2,j3,kd)
                  u(i12  ,j2,j3,kd)=u(i11  ,j2,j3,kd)
                  u(i12+1,j2,j3,kd)=u(i11+1,j2,j3,kd)
                  u(i12+2,j2,j3,kd)=u(i11+1,j2,j3,kd)
 540        continue
          end if
        end if
      end if

      return
      end


      subroutine insbv( i1,i2,i3,kd1,kd2,kdn,is1,is2,is3,t,
     & vr,vrr,d14,d24,drs,nd,ndra,ndrb,ndsa,ndsb,ndta,ndtb, xy,rsxy,u )
!======================================================================
!        Return Tangential and Mixed Derivatives
!          at a Corner in 2D or and edge in 3D
!
! Input
!  u  : solution with correct boundary values
! Output -
!  vr(.,.), vrr(.,.,.)
!======================================================================
      implicit none
      integer ndra,ndrb,ndsa,ndsb,ndta,ndtb,nd
      real t,vr(3,3),vrr(3,3,3),d14(3),d24(3),drs(3),
     &    u(ndra:ndrb,ndsa:ndsb,ndta:ndtb,0:*),
     &   xy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd),
     & rsxy(ndra:ndrb,ndsa:ndsb,ndta:ndtb,nd,nd)
      integer i1,i2,i3,kd1,kd2,kdn,is1,is2,is3
!.......local
      real a(3,3),b(3),tn(3)
      logical debug,oldway

      integer kd,kdd,kdp1,kdp2,kd3,m1,m2,m3,j1,j2,j3
      integer n1,n2,n3
      real det,deti,ajac,a1,a2,a3
      real rx,ry,rz,sx,sy,sz,tx,ty,tz,ubr,ubs,ubt,ubrr,ubss,ubtt,
     & ubrs,ubrt,ubst,rx3,rxr3,rxs3,rxt3,divr0,divs0,divt0,
     & uc31,uc32,uc33,ux6m,rsxyr,rsxys,rsxyt,trsi
      real rxr,rxs,rxt, ryr,rys,ryt, rzr,rzs,rzt,
     &     sxr,sxs,sxt, syr,sys,syt, szr,szs,szt,
     &     txr,txs,txt, tyr,tys,tyt, tzr,tzs,tzt
 
      real taylor3d3e1,taylor3d3e2,taylor3d2e1,taylor3d2e2,
     & taylor3d1e1,taylor3d1e2,uc3d1e11,uc3d2e11,uc3d3e11

!c      include 'cgins.h'
!.......start statement functions
!c    include 'cginsts.h'
      rx(i1,i2,i3)=rsxy(i1,i2,i3,  1,  1)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,  1,  2)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,  1,kd3)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,  2,  1)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,  2,  2)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,  2,kd3)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  1)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,kd3,  2)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,kd3,kd3)
!c      include 'cginsd.h'
!.......
      ubr(i1,i2,i3,kd)=(8.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))
     &                    -(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)))*d14(1)
      ubs(i1,i2,i3,kd)=(8.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))
     &                    -(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)))*d14(2)
      ubt(i1,i2,i3,kd)=(8.*(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))
     &                    -(u(i1,i2,i3+2,kd)-u(i1,i2,i3-2,kd)))*d14(3)
      ubrr(i1,i2,i3,kd)=
     & ( -30.*u(i1,i2,i3,kd)
     &  +16.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))
     &      -(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*d24(1)
      ubss(i1,i2,i3,kd)=
     &+( -30.*u(i1,i2,i3,kd)
     &  +16.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))
     &      -(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )*d24(2)
      ubtt(i1,i2,i3,kd)=
     &+( -30.*u(i1,i2,i3,kd)
     &  +16.*(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd))
     &      -(u(i1,i2,i3+2,kd)+u(i1,i2,i3-2,kd)) )*d24(3)
      ubrs(i1,i2,i3,kd)=
     &   (8.*(ubs(i1+1,i2,i3,kd)-ubs(i1-1,i2,i3,kd))
     &      -(ubs(i1+2,i2,i3,kd)-ubs(i1-2,i2,i3,kd)))*d14(1)
      ubrt(i1,i2,i3,kd)=
     &   (8.*(ubt(i1+1,i2,i3,kd)-ubt(i1-1,i2,i3,kd))
     &      -(ubt(i1+2,i2,i3,kd)-ubt(i1-2,i2,i3,kd)))*d14(1)
      ubst(i1,i2,i3,kd)=
     &   (8.*(ubt(i1,i2+1,i3,kd)-ubt(i1,i2-1,i3,kd))
     &      -(ubt(i1,i2+2,i3,kd)-ubt(i1,i2-2,i3,kd)))*d14(2)
!     ...short forms for jacobian entries and derivatives
      rsxyr(i1,i2,i3,m1,m2)=(8.*(rsxy(i1+1,i2,i3,m1,m2)-rsxy(i1-1,i2,i3,m1,m2))
     &                         -(rsxy(i1+2,i2,i3,m1,m2)-rsxy(i1-2,i2,i3,m1,m2)))*d14(1)
      rsxys(i1,i2,i3,m1,m2)=(8.*(rsxy(i1,i2+1,i3,m1,m2)-rsxy(i1,i2-1,i3,m1,m2))
     &                         -(rsxy(i1,i2+2,i3,m1,m2)-rsxy(i1,i2-2,i3,m1,m2)))*d14(2)
      rsxyt(i1,i2,i3,m1,m2)=(8.*(rsxy(i1,i2,i3+1,m1,m2)-rsxy(i1,i2,i3-1,m1,m2))
     &                         -(rsxy(i1,i2,i3+2,m1,m2)-rsxy(i1,i2,i3-2,m1,m2)))*d14(3)
      rxr(i1,i2,i3)=rsxyr(i1,i2,i3,1,1)
      sxr(i1,i2,i3)=rsxyr(i1,i2,i3,2,1)
      txr(i1,i2,i3)=rsxyr(i1,i2,i3,3,1)
      ryr(i1,i2,i3)=rsxyr(i1,i2,i3,1,2)
      syr(i1,i2,i3)=rsxyr(i1,i2,i3,2,2)
      tyr(i1,i2,i3)=rsxyr(i1,i2,i3,3,2)
      rzr(i1,i2,i3)=rsxyr(i1,i2,i3,1,3)
      szr(i1,i2,i3)=rsxyr(i1,i2,i3,2,3)
      tzr(i1,i2,i3)=rsxyr(i1,i2,i3,3,3)

      rxs(i1,i2,i3)=rsxys(i1,i2,i3,1,1)
      sxs(i1,i2,i3)=rsxys(i1,i2,i3,2,1)
      txs(i1,i2,i3)=rsxys(i1,i2,i3,3,1)
      rys(i1,i2,i3)=rsxys(i1,i2,i3,1,2)
      sys(i1,i2,i3)=rsxys(i1,i2,i3,2,2)
      tys(i1,i2,i3)=rsxys(i1,i2,i3,3,2)
      rzs(i1,i2,i3)=rsxys(i1,i2,i3,1,3)
      szs(i1,i2,i3)=rsxys(i1,i2,i3,2,3)
      tzs(i1,i2,i3)=rsxys(i1,i2,i3,3,3)

      rxt(i1,i2,i3)=rsxyt(i1,i2,i3,1,1)
      sxt(i1,i2,i3)=rsxyt(i1,i2,i3,2,1)
      txt(i1,i2,i3)=rsxyt(i1,i2,i3,3,1)
      ryt(i1,i2,i3)=rsxyt(i1,i2,i3,1,2)
      syt(i1,i2,i3)=rsxyt(i1,i2,i3,2,2)
      tyt(i1,i2,i3)=rsxyt(i1,i2,i3,3,2)
      rzt(i1,i2,i3)=rsxyt(i1,i2,i3,1,3)
      szt(i1,i2,i3)=rsxyt(i1,i2,i3,2,3)
      tzt(i1,i2,i3)=rsxyt(i1,i2,i3,3,3)

      rx3(m1,m2) =rsxy (i1,i2,i3,m1,m2)
      rxr3(m1,m2)=rsxyr(i1,i2,i3,m1,m2)
      rxs3(m1,m2)=rsxys(i1,i2,i3,m1,m2)
      rxt3(m1,m2)=rsxyt(i1,i2,i3,m1,m2)
!       rhs for div.r=0, div.s=0 and div.t=0
      divr0(m1,m2,m3)=
     &   rxr3(m1,1)*vr(1,m1)+rxr3(m1,2)*vr(2,m1)+rxr3(m1,3)*vr(3,m1)
     & + rxr3(m2,1)*vr(1,m2)+rxr3(m2,2)*vr(2,m2)+rxr3(m2,3)*vr(3,m2)
     & + rxr3(m3,1)*vr(1,m3)+rxr3(m3,2)*vr(2,m3)+rxr3(m3,3)*vr(3,m3)
     & + rx3(m1,1)*vrr(1,m1,m1)+rx3(m1,2)*vrr(2,m1,m1)
     &                         +rx3(m1,3)*vrr(3,m1,m1)
     & + rx3(m3,1)*vrr(1,m1,m3)+rx3(m3,2)*vrr(2,m1,m3)
     &                         +rx3(m3,3)*vrr(3,m1,m3)
      divs0(m1,m2,m3)=
     &   rxs3(m1,1)*vr(1,m1)+rxs3(m1,2)*vr(2,m1)+rxs3(m1,3)*vr(3,m1)
     & + rxs3(m2,1)*vr(1,m2)+rxs3(m2,2)*vr(2,m2)+rxs3(m2,3)*vr(3,m2)
     & + rxs3(m3,1)*vr(1,m3)+rxs3(m3,2)*vr(2,m3)+rxs3(m3,3)*vr(3,m3)
     & + rx3(m1,1)*vrr(1,m1,m1)+rx3(m1,2)*vrr(2,m1,m1)
     &                         +rx3(m1,3)*vrr(3,m1,m1)
     & + rx3(m3,1)*vrr(1,m1,m3)+rx3(m3,2)*vrr(2,m1,m3)
     &                         +rx3(m3,3)*vrr(3,m1,m3)
      divt0(m1,m2,m3)=
     &   rxt3(m1,1)*vr(1,m1)+rxt3(m1,2)*vr(2,m1)+rxt3(m1,3)*vr(3,m1)
     & + rxt3(m2,1)*vr(1,m2)+rxt3(m2,2)*vr(2,m2)+rxt3(m2,3)*vr(3,m2)
     & + rxt3(m3,1)*vr(1,m3)+rxt3(m3,2)*vr(2,m3)+rxt3(m3,3)*vr(3,m3)
     & + rx3(m1,1)*vrr(1,m1,m1)+rx3(m1,2)*vrr(2,m1,m1)
     &                         +rx3(m1,3)*vrr(3,m1,m1)
     & + rx3(m3,1)*vrr(1,m1,m3)+rx3(m3,2)*vrr(2,m1,m3)
     &                         +rx3(m3,3)*vrr(3,m1,m3)
      uc33(is1,is2,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1+is1,i2+is2,i3,kd)
     &            +(is1*drs(1))**2*vrr(kd,1,1)
*      & +2.*(is1*drs(1)*is2*drs(2))*vrr(kd,1,2)
     &            +(is2*drs(2))**2*vrr(kd,2,2)
      uc32(is1,is3,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1+is1,i2,i3+is3,kd)
     &            +(is1*drs(1))**2*vrr(kd,1,1)
*      & +2.*(is1*drs(1)*is3*drs(3))*vrr(kd,1,3)
     &            +(is3*drs(3))**2*vrr(kd,3,3)
      uc31(is2,is3,i1,i2,i3,kd)=
     & 2.*u(i1,i2,i3,kd)-u(i1,i2+is2,i3+is3,kd)
     &            +(is2*drs(2))**2*vrr(kd,2,2)
*      & +2.*(is2*drs(2)*is3*drs(3))*vrr(kd,2,3)
     &            +(is3*drs(3))**2*vrr(kd,3,3)

!  Here are more accurate expressions for 3D -- exact for 4th order polynomials
      taylor3d3e1(is1,is2,i1,i2,i3,kd)=       ! 3d, edge along direction 3, 1st derivative term in Taylor series
     &     (is1*drs(1))*vr(kd,1)
     &    +(is2*drs(2))*vr(kd,2)              
      taylor3d3e2(is1,is2,i1,i2,i3,kd)=       ! 3d, edge along direction 3, 2nd derivative term in Taylor series
     &          .5*(is1*drs(1))**2*vrr(kd,1,1)
*     &    +(is1*drs(1)*is2*drs(2))*vrr(kd,1,2)
     &         +.5*(is2*drs(2))**2*vrr(kd,2,2)

      taylor3d2e1(is1,is3,i1,i2,i3,kd)=       ! 3d, edge along direction 2, 1st derivative term in Taylor series
     &     (is1*drs(1))*vr(kd,1)
     &    +(is3*drs(3))*vr(kd,3)              
      taylor3d2e2(is1,is3,i1,i2,i3,kd)=       ! 3d, edge along direction 2, 2nd derivative term in Taylor series
     &          .5*(is1*drs(1))**2*vrr(kd,1,1)
*     &    +(is1*drs(1)*is3*drs(3))*vrr(kd,1,3)
     &         +.5*(is3*drs(3))**2*vrr(kd,3,3)

      taylor3d1e1(is2,is3,i1,i2,i3,kd)=       ! 3d, edge along direction 1, 1st derivative term in Taylor series
     &     (is2*drs(2))*vr(kd,2)
     &    +(is3*drs(3))*vr(kd,3)              
      taylor3d1e2(is2,is3,i1,i2,i3,kd)=       ! 3d, edge along direction 1, 2nd derivative term in Taylor series
     &          .5*(is2*drs(2))**2*vrr(kd,2,2)
*     &    +(is2*drs(2)*is3*drs(3))*vrr(kd,2,3)
     &         +.5*(is3*drs(3))**2*vrr(kd,3,3)

      uc3d3e11(is1,is2,i1,i2,i3,kd)=           !  3d, edge along direction 3,for u(-1,-1,*) 
     &   3.75*u(i1,i2,i3,kd)  
     &    -3.*u(i1+is1,i2+is2,i3,kd)
     &   +.25*u(i1+2*is1,i2+2*is2,i3,kd)
     &  +1.5*taylor3d3e1(is1,is2,i1,i2,i3,kd)
     &  + 3.*taylor3d3e2(is1,is2,i1,i2,i3,kd)

      uc3d2e11(is1,is3,i1,i2,i3,kd)=           !  3d, edge along direction 2,for u(-1,*,-1) 
     &   3.75*u(i1,i2,i3,kd)  
     &    -3.*u(i1+is1,i2,i3+is3,kd)
     &   +.25*u(i1+2*is1,i2,i3+2*is3,kd)
     &  +1.5*taylor3d2e1(is1,is3,i1,i2,i3,kd)
     &  + 3.*taylor3d2e2(is1,is3,i1,i2,i3,kd)

      uc3d1e11(is2,is3,i1,i2,i3,kd)=           !  3d, edge along direction 1,for u(*,-1,-1) 
     &   3.75*u(i1,i2,i3,kd)  
     &    -3.*u(i1,i2+is2,i3+is3,kd)
     &   +.25*u(i1,i2+2*is2,i3+2*is3,kd)
     &  +1.5*taylor3d1e1(is2,is3,i1,i2,i3,kd)
     &  + 3.*taylor3d1e2(is2,is3,i1,i2,i3,kd)

!     ...extrapolate velocity in 3D (extrap u(i1-n1,i2-n2,i3-n3)
      ux6m(n1,n2,n3,kd)=
     &   + 6.*u(i1     ,i2     ,i3     ,kd)
     &   -15.*u(i1+  n1,i2+  n2,i3+  n3,kd)
     &   +20.*u(i1+2*n1,i2+2*n2,i3+2*n3,kd)
     &   -15.*u(i1+3*n1,i2+3*n2,i3+3*n3,kd)
     &   + 6.*u(i1+4*n1,i2+4*n2,i3+4*n3,kd)
     &   -    u(i1+5*n1,i2+5*n2,i3+5*n3,kd)
!.......end statement functions

      oldway=.false. ! .true.

      debug=.false.
      kd3=min(nd,3)

      do 100 kd=1,nd
        vr(kd,1)   =ubr(i1,i2,i3,kd)
        vr(kd,2)   =ubs(i1,i2,i3,kd)
        vrr(kd,1,1)=ubrr(i1,i2,i3,kd)
        vrr(kd,2,2)=ubss(i1,i2,i3,kd)
        if( nd.eq.3 )then
          vr(kd,3)   =ubt(i1,i2,i3,kd)
          vrr(kd,3,3)=ubtt(i1,i2,i3,kd)
        end if
 100  continue

      if( nd.eq.2 )then
!     ...get mixed derivatives at corners
        ajac=rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)
        a1=rxr(i1,i2,i3)*vr(1,1)+sxr(i1,i2,i3)*vr(1,2)
     &    +ryr(i1,i2,i3)*vr(2,1)+syr(i1,i2,i3)*vr(2,2)
        a2=rxs(i1,i2,i3)*vr(1,1)+sxs(i1,i2,i3)*vr(1,2)
     &    +rys(i1,i2,i3)*vr(2,1)+sys(i1,i2,i3)*vr(2,2)
        vrr(1,1,2)=(+ry(i1,i2,i3)*rx(i1,i2,i3)*vrr(1,1,1)
     &              -sy(i1,i2,i3)*sx(i1,i2,i3)*vrr(1,2,2)
     &              +ry(i1,i2,i3)*ry(i1,i2,i3)*vrr(2,1,1)
     &              -sy(i1,i2,i3)*sy(i1,i2,i3)*vrr(2,2,2)
     &              +ry(i1,i2,i3)*a1-sy(i1,i2,i3)*a2)/ajac
        vrr(2,1,2)=(-rx(i1,i2,i3)*rx(i1,i2,i3)*vrr(1,1,1)
     &              +sx(i1,i2,i3)*sx(i1,i2,i3)*vrr(1,2,2)
     &              -rx(i1,i2,i3)*ry(i1,i2,i3)*vrr(2,1,1)
     &              +sx(i1,i2,i3)*sy(i1,i2,i3)*vrr(2,2,2)
     &              -rx(i1,i2,i3)*a1+sx(i1,i2,i3)*a2)/ajac
      else
!       ...3D
!         ...tangent in the direction kdn
        tn(1)=rx3(kd1,2)*rx3(kd2,3)-rx3(kd1,3)*rx3(kd2,2)
        tn(2)=rx3(kd1,3)*rx3(kd2,1)-rx3(kd1,1)*rx3(kd2,3)
        tn(3)=rx3(kd1,1)*rx3(kd2,2)-rx3(kd1,2)*rx3(kd2,1)
      if( debug .and.kdn.eq.1 )then
        write(1,9100) i1,i2,i3,is1,is2,is3,kd1,kd2,kdn,tn,
     &  (vrr(kdd,1,1),kdd=1,nd),(vrr(kdd,2,2),kdd=1,nd),
     &  (vrr(kdd,3,3),kdd=1,nd)
      end if
 9100 format(' INSBV: i1,i2,i3 =',3i3,' is1,is2,is3=',3i3,
     & ' kd1,kd2,kdn=',3i3,/,' tn =',3e10.2,' u.rr=',3e10.2,/,
     & ' v.ss=',3e10.2,' v.tt=',3e10.2)
        if( kdn.eq.3 )then
!          direction 3 derivatives are also known:
!****** watch out here for i3 near boundaries****
          do 210 kd=1,nd
            vrr(kd,1,3)=ubrt(i1,i2,i3,kd)
            vrr(kd,2,3)=ubst(i1,i2,i3,kd)
            vrr(kd,3,1)=vrr(kd,1,3)
            vrr(kd,3,2)=vrr(kd,2,3)
 210      continue
          b(1)=-divr0(kd1,kd2,kdn)
          b(2)=-divs0(kd2,kd1,kdn)
!         ...determine the rhs from the extrapolation condition
          if( oldway )then
          trsi=1./(2.*(is1*drs(kd1)*is2*drs(kd2)))
          b(3)=
     &       tn(1)*(ux6m(is1,is2,0,1)-uc33(is1,is2,i1,i2,i3,1))*trsi
     &      +tn(2)*(ux6m(is1,is2,0,2)-uc33(is1,is2,i1,i2,i3,2))*trsi
     &      +tn(3)*(ux6m(is1,is2,0,3)-uc33(is1,is2,i1,i2,i3,3))*trsi
          else
          trsi=1./(3.*(is1*drs(kd1)*is2*drs(kd2)))
          b(3)=
     &       tn(1)*(ux6m(is1,is2,0,1)-uc3d3e11(is1,is2,i1,i2,i3,1))*trsi
     &      +tn(2)*(ux6m(is1,is2,0,2)-uc3d3e11(is1,is2,i1,i2,i3,2))*trsi
     &      +tn(3)*(ux6m(is1,is2,0,3)-uc3d3e11(is1,is2,i1,i2,i3,3))*trsi
          end if
        elseif( kdn.eq.2 )then
          do 220 kd=1,nd
            vrr(kd,1,2)=ubrs(i1,i2,i3,kd)
            vrr(kd,2,3)=ubst(i1,i2,i3,kd)
            vrr(kd,2,1)=vrr(kd,1,2)
            vrr(kd,3,2)=vrr(kd,2,3)
 220      continue
          b(1)=-divr0(kd1,kd2,kdn)
          b(2)=-divt0(kd2,kd1,kdn)
!         ...determine the rhs from the extrapolation condition
          if( oldway )then
          trsi=1./(2.*(is1*drs(kd1)*is3*drs(kd2)))
          b(3)=
     &       tn(1)*(ux6m(is1,0,is3,1)-uc32(is1,is3,i1,i2,i3,1))*trsi
     &      +tn(2)*(ux6m(is1,0,is3,2)-uc32(is1,is3,i1,i2,i3,2))*trsi
     &      +tn(3)*(ux6m(is1,0,is3,3)-uc32(is1,is3,i1,i2,i3,3))*trsi
          else
          trsi=1./(3.*(is1*drs(kd1)*is3*drs(kd2)))
          b(3)=
     &       tn(1)*(ux6m(is1,0,is3,1)-uc3d2e11(is1,is3,i1,i2,i3,1))*trsi
     &      +tn(2)*(ux6m(is1,0,is3,2)-uc3d2e11(is1,is3,i1,i2,i3,2))*trsi
     &      +tn(3)*(ux6m(is1,0,is3,3)-uc3d2e11(is1,is3,i1,i2,i3,3))*trsi
          end if
        elseif( kdn.eq.1 )then
          do 230 kd=1,nd
            vrr(kd,1,2)=ubrs(i1,i2,i3,kd)
            vrr(kd,1,3)=ubrt(i1,i2,i3,kd)
            vrr(kd,2,1)=vrr(kd,1,2)
            vrr(kd,3,1)=vrr(kd,1,3)
 230      continue
          b(1)=-divs0(kd1,kd2,kdn)
          b(2)=-divt0(kd2,kd1,kdn)
!         ...determine the rhs from the extrapolation condition
          if( oldway )then
          trsi=1./(2.*(is2*drs(kd1)*is3*drs(kd2)))
          b(3)=
     &       tn(1)*(ux6m(0,is2,is3,1)-uc31(is2,is3,i1,i2,i3,1))*trsi
     &      +tn(2)*(ux6m(0,is2,is3,2)-uc31(is2,is3,i1,i2,i3,2))*trsi
     &      +tn(3)*(ux6m(0,is2,is3,3)-uc31(is2,is3,i1,i2,i3,3))*trsi
          else
          trsi=1./(3.*(is2*drs(kd1)*is3*drs(kd2)))
          b(3)=
     &       tn(1)*(ux6m(0,is2,is3,1)-uc3d1e11(is2,is3,i1,i2,i3,1))*trsi
     &      +tn(2)*(ux6m(0,is2,is3,2)-uc3d1e11(is2,is3,i1,i2,i3,2))*trsi
     &      +tn(3)*(ux6m(0,is2,is3,3)-uc3d1e11(is2,is3,i1,i2,i3,3))*trsi
          end if
!      if( debug )then
!        j2=i2-is2
!        j3=i3-is3
!        write(1,9700) ux6m(0,is2,is3,1),uc31(is2,is3,i1,i2,i3,1),
!     &   u0(xy(i1,j2,j3,1),xy(i1,j2,j3,2),xy(i1,j2,j3,3),t)
!      end if
! 9700 format(' ux6m(0,is2,is3,1)=',e12.4,' uc31 =',e12.4,' u0=',e12.4)
        else
          stop 'INSBV: Invalid value for kdn'
        end if

        a(1,1)=rx3(kd2,1)
        a(1,2)=rx3(kd2,2)
        a(1,3)=rx3(kd2,3)
        a(2,1)=rx3(kd1,1)
        a(2,2)=rx3(kd1,2)
        a(2,3)=rx3(kd1,3)
        a(3,1)=tn(1)
        a(3,2)=tn(2)
        a(3,3)=tn(3)
        det=a(1,1)*(a(2,2)*a(3,3)-a(3,2)*a(2,3))
     &     +a(2,1)*(a(3,2)*a(1,3)-a(1,2)*a(3,3))
     &     +a(3,1)*(a(1,2)*a(2,3)-a(2,2)*a(1,3))
        if( det.eq.0. )then
          stop 'INSBV: det=0'
        end if
        deti=1./det
        do 400 kd=1,nd
          kdp1=mod(kd  ,nd)+1
          kdp2=mod(kd+1,nd)+1
          vrr(kd,kd1,kd2)= deti*
     &     (  b(1)*(a(2,kdp1)*a(3,kdp2)-a(3,kdp1)*a(2,kdp2))
     &       +b(2)*(a(3,kdp1)*a(1,kdp2)-a(1,kdp1)*a(3,kdp2))
     &       +b(3)*(a(1,kdp1)*a(2,kdp2)-a(2,kdp1)*a(1,kdp2)) )
          vrr(kd,kd2,kd1)=vrr(kd,kd1,kd2)
 400  continue
      if( debug .and.kdn.eq.1 )then
        write(1,9200) a,b,
     &  kd1,kd2, (vrr(kd,kd1,kd2),kd=1,nd),(ubst(i1,i2,i3,kd),kd=1,nd)
      end if
 9200 format(' a=',9e9.1,/,' b=',3e12.4,/,
     &       ' kd1,kd2=',2i2,' vrr(kd1,kd2)=',3e12.4,/,
     &       '             ubrr=',3e12.4)

      end if

      return
      end

