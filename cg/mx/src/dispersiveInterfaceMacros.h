!         -*- mode: F90 -*-
! *********************************************************************
! ********** MACROS FOR DISPERSIVE INTERFACE CONDITIONS ***************
!    This file is included into interface3d.bf 
! *********************************************************************

! --------------------------------------------------------------------
! Macro: Assign DISPERSIVE interface ghost values, DIM=2, ORDER=2, GRID=Rectangular
! 
! Here are the jump conditions
!   [ u.x + v.y +w.z ] = 0
!   [ u.xx + u.yy +u.zz ] = 0
! 
!   [ tau1.(w.y-v.z, u.z-w.x, v.x-u.y)/mu] = 0 
!   [ (v.xx+v.yy+v.zz)/eps ] = 0
! 
!   [ tau2.(w.y-v.z, u.z-w.x, v.x-u.y)/mu] = 0 
!   [ (w.xx+w.yy+w.zz)/eps ] = 0
! ---------------------------------------------------------------------
#beginMacro assignDispersiveInterfaceGhost22r()

 ! ****************************************************
 ! ***********  2D, ORDER=2, RECTANGULAR **************
 ! ****************************************************

INFO("22r-Dispersive")

beginLoopsMask2d()
 ! first evaluate the equations we want to solve with the wrong values at the ghost points:

  evalInterfaceDerivatives2d()
 
  f(0)=(u1x+v1y) - \
       (u2x+v2y)

  if( setDivergenceAtInterfaces.eq.0 )then
    f(1)=(u1xx+u1yy) - \
         (u2xx+u2yy)
  else
    ! set div(E)=0 at both intefaces 
    f(1)=(u1x+v1y)
  end if

  f(2)=(v1x-u1y)/mu1 - \
       (v2x-u2y)/mu2
  
  f(3)=(v1xx+v1yy)/epsmu1 - \
       (v2xx+v2yy)/epsmu2

  ! write(debugFile,'(" --> i1,i2=",2i4," f(start)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)

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

   ! equation 1:
   if( setDivergenceAtInterfaces.eq.0 )then
     a4(1,0) = 1./(dx1(axis1)**2)   ! coeff of u1(-1) from [u.xx + u.yy]
     a4(1,1) = 0. 
     a4(1,2) =-1./(dx2(axis2)**2)   ! coeff of u2(-1) from [u.xx + u.yy]
     a4(1,3) = 0. 
   else
     ! u1x+v1y=0
     if( axis1.eq.0 )then
       a4(1,0) = -is1/(2.*dx1(0))    ! coeff of u1(-1) from [u.x+v.y] 
       a4(1,1) = 0.                  ! coeff of v1(-1) from [u.x+v.y] 
     else 
       a4(1,0) = 0.                 
       a4(1,1) = -is2/(2.*dx1(1))    ! coeff of v1(-1) from [u.x+v.y] 
     end if
     a4(1,2) = 0.
     a4(1,3) = 0.
   end if 
     
   ! equation 3: 
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
   ! write(debugFile,'(" --> i1,i2=",2i4," f(subtract)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)
   ! solve A Q = F
   ! factor the matrix
   numberOfEquations=4
   call dgeco( a4(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0))
   ! solve
   ! write(debugFile,'(" --> i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
   job=0
   call dgesl( a4(0,0), numberOfEquations, numberOfEquations, ipvt(0), f(0), job)
   ! write(debugFile,'(" --> i1,i2=",2i4," f(solve)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)

   u1(i1-is1,i2-is2,i3,ex)=f(0)
   u1(i1-is1,i2-is2,i3,ey)=f(1)
   u2(j1-js1,j2-js2,j3,ex)=f(2)
   u2(j1-js1,j2-js2,j3,ey)=f(3)

   if( debug.gt.2 )then ! re-evaluate
    evalInterfaceDerivatives2d()
    f(0)=(u1x+v1y) - \
         (u2x+v2y)
    if( setDivergenceAtInterfaces.eq.0 )then
      f(1)=(u1xx+u1yy) - \
           (u2xx+u2yy)
    else
      f(1)=(u1x+v1y)
    end if
    f(2)=(v1x-u1y)/mu1 - \
         (v2x-u2y)/mu2
    f(3)=(v1xx+v1yy)/epsmu1 - \
         (v2xx+v2yy)/epsmu2
    write(debugFile,'("i3d: --> i1,i2=",2i4," f(re-eval)=",4e10.2)') i1,i2,f(0),f(1),f(2),f(3)
   end if


   ! -------------------------------------------------------
   ! solve for Hz         *fixed* *wdh* June 24, 2016
   !  [ w.n/eps] = 0
   !  [ Lap(w)/eps] = 0

   evalMagneticFieldInterfaceDerivatives2d()
   evalMagneticField2dJumpOrder2()
 
   ! a2(0,0)=-is*(an1*rsxy1(i1,i2,i3,axis1,0)+an2*rsxy1(i1,i2,i3,axis1,1))/(2.*dr1(axis1)*eps1)
   ! a2(0,1)= js*(an1*rsxy2(j1,j2,j3,axis2,0)+an2*rsxy2(j1,j2,j3,axis2,1))/(2.*dr2(axis2)*eps2)

   a2(0,0)=-is*(1./(2.*dx1(axis1)*eps1)) ! coeff of w1(-1) in [w.n/eps]=0 
   a2(0,1)= js*(1./(2.*dx2(axis2)*eps2)) ! coeff of w2(-1) in [w.n/eps]=0 

 
   a2(1,0)= 1./(dx1(axis1)**2*eps1)    ! coeff of w1(-1) in [Lap(w)/eps ]=0 
   ! *wdh* Sept 15, 2017 a2(1,1)=-1./(dx2(axis2)**2*eps2)    ! coeff of w2(-1) in [Lap(w)/eps ]=0 
   a2(1,1)= 1./(dx2(axis2)**2*eps2)    ! coeff of w2(-1) in [Lap(w)/eps ]=0 
 
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

   ! do this for now
   !u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 
   !u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)

 endLoopsMask2d()
#endMacro
