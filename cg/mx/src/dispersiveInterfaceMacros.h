!         -*- mode: F90 -*-
! *********************************************************************
! ********** MACROS FOR DISPERSIVE INTERFACE CONDITIONS ***************
!    This file is included into interface3d.bf 
! *********************************************************************


! -------------------------------------------------------------------------
! Macro: Evaluate DISPERSIVE forcing terms, 2nd-order accuracy 
!   This macro can be usedto eval values in either domain 1 or domain 2
!
! Input:
!   fev(n) : forcing on E equation: E_{tt} = c^2 Delta(E) + ... + fev
!   fpv(n,jv) : forcing on equation for P_{n,jv} 
! Output
!   fp(n) : 
!   beta = 1 - alphaP*Sum_k{ C_k }
! ------------------------------------------------------------------------
#beginMacro getDispersiveForcingOrder2(k1,k2,k3, fp, fpv,fev, p,pn,pm, u,un,um, dispersionModel,numberOfPolarizationVectors,alphaP,beta,a0v,a1v,b0v,b1v)
  do n=0,nd-1
    fp(n)=0.
  end do
  if( dispersionModel.ne.noDispersion )then
   Csum=0.
   do jv=0,numberOfPolarizationVectors-1
     Bk = 1 + .5*dt*( b1v(jv) + alphaP*a1v(jv) )
     Ck = (1./Bk)*a1v(jv)*dt*.5
     Csum = Csum + Ck 

     do n=0,nd-1
       pc = n + jv*nd 
       ec = ex +n
       ! P at new time t+dt
       ! Pt, Ptt at time t
       pv   =  p(k1,k2,k3,pc)
       pvt  = (p(k1,k2,k3,pc)                   -pm(k1,k2,k3,pc))/(2.*dt)
       pvtt = (p(k1,k2,k3,pc)-2.*pn(k1,k2,k3,pc)+pm(k1,k2,k3,pc))/(dt**2)

       ! E at new time t+dt
       ! Et, Ett at time t
       ev    =  u(k1,k2,k3,ec)
       evt   = (u(k1,k2,k3,ec)                    -um(k1,k2,k3,ec))/(2.*dt) 
       evtt  = (u(k1,k2,k3,ec)-2.*un(k1,k2,k3,ec )+um(k1,k2,k3,ec))/(dt**2)

       fp(n) =fp(n) + (1./Bk)*( \
                    - b1v(jv)*( pvt + .5*dt*pvtt )  \
                    - b0v(jv)*pv + a0v(jv)*ev + a1v(jv)*( evt + .5*dt*(evtt+fev(n)) ) \
                   + fpv(n,jv) \
                              )

       ! write(*,'(" k1,k2,k3=",3i3)') k1,k2,k3
       ! write(*,'(" pc=",i3," p,pn,pm=",3e12.2)') pc, p(k1,k2,k3,pc),pn(k1,k2,k3,pc),pm(k1,k2,k3,pc)
       ! write(*,'(" dt=",e12.2," pv,pvt,pvtt, ev,evt,evtt=",6e12.2)') dt,pv,pvt,pvtt, ev,evt,evtt
       ! write(*,'(" jv=",i2," a0,a1,b0,b1=",4e12.2," Bk,Ck=",2e12.2)') jv,a0v(jv),a1v(jv),b0v(jv),b1v(jv),Bk,Ck
       ! write(*,'(" n=",i2," fev(n)=",e12.2," fp(n)=",e12.2," fpv(n,jv)=",e12.2)') n,fev(n),fp(n),fpv(n,jv)
     end do
   end do
   ! we could precompute D
   beta = 1. -alphaP*Csum
  end if
#endMacro

!-------------------------------------------------------------------------------------------
! Macro: Eval twilight-zone forcing for GDM equations
! Output
!  fpv(n,jv) : RHS To Pv_{n,jv} equation 
!  fev(n)    : RHS to E_{n} equation
!-------------------------------------------------------------------------------------------
#beginMacro evalTZforcingGDM(xy,i1,i2,i3,dispersionModel,numberOfPolarizationVectors,c,alphaP,a0v,a1v,b0v,b1v,fpv,fpSum,fev)
  if( dispersionModel.ne.noDispersion )then
    do n=0,nd-1
      fpSum(n)=0.
      pettSum(n)=0.

      call ogderiv(ep, 0,0,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex+n, es(n)   ) 
      call ogderiv(ep, 1,0,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex+n, est(n)  )
      call ogderiv(ep, 2,0,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex+n, estt(n) )
      call ogderiv(ep, 0,2,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex+n, esxx(n) )
      call ogderiv(ep, 0,0,2,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,ex+n, esyy(n) )

      do jv=0,numberOfPolarizationVectors-1
        ! The TZ component is offset by pxc
        pc = pxc + jv*nd
        call ogderiv(ep, 0,0,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,pc+n, pe(n)   )
        call ogderiv(ep, 1,0,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,pc+n, pet(n)  )
        call ogderiv(ep, 2,0,0,0, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,pc+n, pett(n) )
        ! Normal TZ forcing for P_{n,jv} equation: 
        fpv(n,jv) = pett(n) + b1v(jv)*pet(n) + b0v(jv)*pe(n) - a0v(jv)*es(n) - a1v(jv)*est(n)
        ! Keep sum: 
        fpSum(n)  = fpSum(n)  + fpv(n,jv)
        pettSum(n) = pettSum(n) + pett(n) 
      end do 

      ! TZ forcing for E_{n} equation:
      ! E_tt - c^2 Delta E + alphaP*Ptt  = 
      fev(n) = estt(n) - c**2*( esxx(n) + esyy(n) ) + alphaP*pettSum(n)
    end do
  end if
#endMacro

!-------------------------------------------------------------------------------------------
! Macro: Evaluate TZ forcing for dispersive equations in 2D 
!
! Output
!    fpv1(n,jv) : RHS To Pv_{n,jv} equation on domain 1
!    fpv2(n,jv) : RHS To Pv_{n,jv} equation on domain 2
!    fev1(n)    : RHS to E_{n} equation on domain 1
!    fev2(n)    : RHS to E_{n} equation on domain 2
!-------------------------------------------------------------------------------------------
#beginMacro getDispersiveTZForcing(fpv1,fpv2,fev1,fev2)

  if( twilightZone.eq.1 )then
    evalTZforcingGDM(xy1,i1,i2,i3,dispersionModel1,numberOfPolarizationVectors1,c1,alphaP1,a0v1,a1v1,b0v1,b1v1,fpv1,fpSum1,fev1)
    evalTZforcingGDM(xy2,j1,j2,j3,dispersionModel2,numberOfPolarizationVectors2,c2,alphaP2,a0v2,a1v2,b0v2,b1v2,fpv2,fpSum2,fev2)
  end if

#endMacro 

! ---------------------------------------------------------------------------------------
! Macro: Assign DISPERSIVE interface ghost values, DIM=2, ORDER=2, GRID=Rectangular
! 
! Here are the jump conditions (See notes in DMX_ADE)
!   [ u.x + v.y ] = 0
!   [ (1/mu)* tv,.( curl(E) ) ]
!   [ tv.( c^2*Delta(E) -alphaP*P_tt) ] = 0  --> [ tv.( beta*c^2*Delta(E) - alphaP* F) ]=0 
!   [ (1/mu)* nv.( Delta(E) ) ]=0
! 
! -------------------------------------------------------------------------------------------
#beginMacro assignDispersiveInterfaceGhost22r()

 ! ****************************************************
 ! ***********  2D, ORDER=2, RECTANGULAR **************
 ! ****************************************************

INFO("22r-GDM")

! For rectangular, both sides must axis axis1==axis2: 
if( axis1.ne.axis2 )then
  stop 8826
end if

! 
! Solve for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
!     
!       A [ U ] = A [ U(old) ] - [ f ]
!
!               [ u1(-1) ]
!       [ U ] = [ v1(-1) ]
!               [ u2(-1) ]
!               [ v2(-1) ]
!             

! --- initialize some forcing functions ---
do n=0,nd-1
  fev1(n)=0.
  fev2(n)=0.
  do jv=0,numberOfPolarizationVectors1-1
    fpv1(n,jv)=0.
  end do
  do jv=0,numberOfPolarizationVectors2-1
    fpv2(n,jv)=0.
  end do
end do

! ----------------- START LOOP OVER INTERFACE -------------------------
beginLoopsMask2d()

  ! first evaluate the equations we want to solve with the wrong values at the ghost points:

  evalInterfaceDerivatives2d()
 
  ! Evaluate TZ forcing for dispersive equations in 2D 
  getDispersiveTZForcing(fpv1,fpv2,fev1,fev2)

  ! eval dispersive forcings for domain 1
  getDispersiveForcingOrder2(i1,i2,i3, fp1, fpv1,fev1,p1,p1n,p1m, u1,u1n,u1m, dispersionModel1,numberOfPolarizationVectors1,alphaP1,beta1,a0v1,a1v1,b0v1,b1v1)

  ! eval dispersive forcings for domain 2
  getDispersiveForcingOrder2(j1,j2,j3, fp2, fpv2,fev2,p2,p2n,p2m, u2,u2n,u2m, dispersionModel2,numberOfPolarizationVectors2,alphaP2,beta2,a0v2,a1v2,b0v2,b1v2)


  if( axis1.eq.0 )then
    ! Interface equations for a boundary at x = 0 or x=1

    ! ---- EQUATION 0 -----
    ! NOTE: if mu==mu2 then we do not need TZ forcing for this eqn:
    f(0)=(u1x+v1y) - \
         (u2x+v2y)
    a4(0,0) = -is1/(2.*dx1(axis1))    ! coeff of u1(-1) from [u.x+v.y] 
    a4(0,1) = 0.                      ! coeff of v1(-1) from [u.x+v.y] 
    a4(0,2) =  js1/(2.*dx2(axis2))    ! coeff of u2(-1) from [u.x+v.y] 
    a4(0,3) = 0.                      ! coeff of v2(-1) from [u.x+v.y]
  
    ! ---- EQUATION 1 -----
    ! NOTE: if mu==mu2 then we do not need TZ forcing for this eqn:
    f(1)=(v1x-u1y)/mu1 - \
         (v2x-u2y)/mu2
    a4(1,0) = 0.
    a4(1,1) = -is1/(2.*dx1(axis1))    ! coeff of v1(-1) from [v.x - u.y] 
    a4(1,2) = 0.
    a4(1,3) =  js1/(2.*dx2(axis2))    ! coeff of v2(-1) from [v.x - u.y]
   
    ! ---- EQUATION 2 -----    
    ! NOTE: if mu==mu2 then we do not need TZ forcing for this eqn:
    f(2)=( (u1xx+u1yy)/mu1 ) - \
         ( (u2xx+u2yy)/mu2 )
    a4(2,0) = 1./(dx1(axis1)**2)/mu1   ! coeff of u1(-1) from [(u.xx + u.yy)/mu]
    a4(2,1) = 0. 
    a4(2,2) =-1./(dx2(axis2)**2)/mu2   ! coeff of u2(-1) from [(u.xx + u.yy)/mu]
    a4(2,3) = 0. 
  
    ! ---- EQUATION 3 -----    
    ! The coefficient of Delta(E) in this equation is altered due to Ptt term 
    f(3)=( (v1xx+v1yy)*beta1/epsmu1 -alphaP1*fp1(1) ) - \
         ( (v2xx+v2yy)*beta2/epsmu2 -alphaP2*fp2(1))

    ! TEST 
    if( .false. )then
      f(3)=( (v1xx+v1yy)*beta1/epsmu1 ) - \
           ( (v2xx+v2yy)*beta2/epsmu2 )
    end if
    a4(3,0) = 0.                      
    a4(3,1) = (beta1/epsmu1)/(dx1(axis1)**2) ! coeff of v1(-1) from [beta*c^2*(v.xx+v.yy)]
    a4(3,2) = 0. 
    a4(3,3) =-(beta2/epsmu2)/(dx2(axis2)**2) ! coeff of v2(-1) from [beta*c^2*(v.xx+v.yy)]
  else

    ! Interface equations for a boundary at y = 0 or y=1
    ! Switch u <-> v,  x<-> y in above equations 

    ! ---- EQUATION 0 -----
    f(0)=(v1y+u1x) - \
         (v2y+u2x)
    a4(0,0) = 0.                      ! coeff of u1(-1) from [u.x+v.y] 
    a4(0,1) = -is1/(2.*dx1(axis1))    ! coeff of v1(-1) from [u.x+v.y] 

    a4(0,2) = 0.                      ! coeff of u2(-1) from [u.x+v.y] 
    a4(0,3) = js1/(2.*dx2(axis2))     ! coeff of v2(-1) from [u.x+v.y]
  
    ! ---- EQUATION 1 -----
    f(1)=(u1y-v1x)/mu1 - \
         (u2y-v2x)/mu2
    a4(1,0) = -is1/(2.*dx1(axis1))
    a4(1,1) = 0.
    a4(1,2) =  js1/(2.*dx2(axis2))  
    a4(1,3) = 0.
   
    ! ---- EQUATION 2 -----    
    f(2)=( (v1xx+v1yy)/mu1 ) - \
         ( (v2xx+v2yy)/mu2 )
    a4(2,0) = 0.
    a4(2,1) = 1./(dx1(axis1)**2)/mu1  
    a4(2,2) = 0.
    a4(2,3) =-1./(dx2(axis2)**2)/mu2 
  
    ! ---- EQUATION 3 -----    
    ! The coefficient of Delta(E) in this equation is altered due to Ptt term 
    f(3)=( (u1xx+u1yy)*beta1/epsmu1 -alphaP1*fp1(0) ) - \
         ( (u2xx+u2yy)*beta2/epsmu2 -alphaP2*fp2(0))
    a4(3,0) = (beta1/epsmu1)/(dx1(axis1)**2)
    a4(3,1) = 0.
    a4(3,2) =-(beta2/epsmu2)/(dx2(axis2)**2) 
    a4(3,3) = 0.


  end if


   q(0) = u1(i1-is1,i2-is2,i3,ex)
   q(1) = u1(i1-is1,i2-is2,i3,ey)
   q(2) = u2(j1-js1,j2-js2,j3,ex)
   q(3) = u2(j1-js1,j2-js2,j3,ey)

   if( .false. .or. debug.gt.4 )then 
     write(*,'("BEFORE: --> i1,i2=",2i4," j1,j2=",2i4," f()=",4e10.2)') i1,i2,j1,j2,f(0),f(1),f(2),f(3)
     write(*,'("     beta1,beta2=",2e10.2," fp1=",2e10.2," fp2=",2e10.2)') beta1,beta2,fp1(0),fp1(1),fp2(0),fp2(1)
     write(*,'("     mu1,mu2=",2e10.2," v1y,u1x,v2y,u2x=",4e10.2)') mu1,mu2,v1y,u1x,v2y,u2x
   end if


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

   if( .false. .or. debug.gt.4 )then 
     ! CHECK: re-evaluate the jump conditions
     evalInterfaceDerivatives2d()

     if( axis1.eq.0 )then
        f(0)=(u1x+v1y) - \
             (u2x+v2y)
        f(1)=(v1x-u1y)/mu1 - \
             (v2x-u2y)/mu2
        f(2)=( (u1xx+u1yy)/mu1 ) - \
             ( (u2xx+u2yy)/mu2 )
        f(3)=( (v1xx+v1yy)*beta1/epsmu1 -alphaP1*fp1(1) ) - \
             ( (v2xx+v2yy)*beta2/epsmu2 -alphaP2*fp2(1))
       ! TEST 
        if( .false. )then
          f(3)=( (v1xx+v1yy)*beta1/epsmu1 ) - \
               ( (v2xx+v2yy)*beta2/epsmu2 )
        end if
      else
        f(0)=(v1y+u1x) - \
             (v2y+u2x)
        f(1)=(u1y-v1x)/mu1 - \
             (u2y-v2x)/mu2
        f(2)=( (v1xx+v1yy)/mu1 ) - \
             ( (v2xx+v2yy)/mu2 )    
        f(3)=( (u1xx+u1yy)*beta1/epsmu1 -alphaP1*fp1(0) ) - \
             ( (u2xx+u2yy)*beta2/epsmu2 -alphaP2*fp2(0))
      end if 
      write(*,'("AFTER: --> i1,i2=",2i4," j1,j2=",2i4," f(re-eval)=",4e10.2)') i1,i2,j1,j2,f(0),f(1),f(2),f(3)


      if( twilightZone.eq.1 )then
        ! check errors in the ghost 
          k1=i1-is1
          k2=i2-is2
          k3=i3
          do n=0,nd-1
            call ogderiv(ep, 0,0,0,0, xy1(k1,k2,k3,0),xy1(k1,k2,k3,1),0.,t,ex+n, es(n)   ) 
          end do
          f(0) =  u1(i1-is1,i2-is2,i3,ex) -es(0)
          f(1) =  u1(i1-is1,i2-is2,i3,ey) -es(1)
          k1=j1-js1
          k2=j2-js2
          k3=j3
          do n=0,nd-1
            call ogderiv(ep, 0,0,0,0, xy2(k1,k2,k3,0),xy2(k1,k2,k3,1),0.,t,ex+n, est(n)   ) 
          end do
          f(2) =  u2(j1-js1,j2-js2,j3,ex) -est(0)
          f(3) =  u2(j1-js1,j2-js2,j3,ey) -est(1)
          write(*,'(" ghost err =",4e10.2)') f(0),f(1),f(2),f(3) 
  
      end if
   end if


   ! -------------------------------------------------------
   ! No need to solve for Hz as it is just an ODE
   ! -------------------------------------------------------
 endLoopsMask2d()
 
 ! stop 7777

#endMacro


! --------------------------------------------------------------------------------------------
! Macro:  Evaluate the RHS to the jump conditons: 2D, Order=2, Dispersive
!
! --------------------------------------------------------------------------------------------
#beginMacro eval2dJumpDispersiveOrder2()
 f(0)=(u1x+v1y) - \
      (u2x+v2y)
 f(1)=( an1*u1Lap +an2*v1Lap )/mu1 - \
      ( an1*u2Lap +an2*v2Lap )/mu2 
 f(2)=(v1x-u1y)/mu1 - \
      (v2x-u2y)/mu2
 f(3)=( ( tau1*u1Lap +tau2*v1Lap )*beta1/epsmu1 - alphaP1*(tau1*fp1(0)+tau2*fp1(1)) ) - \
      ( ( tau1*u2Lap +tau2*v2Lap )*beta2/epsmu2 - alphaP2*(tau1*fp2(0)+tau2*fp2(1)) )

#endMacro

! --------------------------------------------------------------------
! Macro: Assign  DISPERSIVE interface ghost values, DIM=2, ORDER=2, GRID=Curvilinear
! 
! Here are the jump conditions (See notes in DMX_ADE)
!   [ u.x + v.y ] = 0
!   [ (1/mu)* tv,.( curl(E) ) ]
!   [ tv.( c^2*Delta(E) -alphaP*P_tt) ] = 0  --> [ tv.( beta*c^2*Delta(E) - alphaP* F) ]=0 
!   [ (1/mu)* nv.( Delta(E) ) ]=0
! 
! -------------------------------------------------------------------------------------------
#beginMacro assignDispersiveInterfaceGhost22c()

  ! ****************************************************
  ! ***********  2D, ORDER=2, CURVILINEAR **************
  ! ****************************************************

INFO("22c-GDM")

! --- initialize some forcing functions ---
do n=0,nd-1
  fev1(n)=0.
  fev2(n)=0.
  do jv=0,numberOfPolarizationVectors1-1
    fpv1(n,jv)=0.
  end do
  do jv=0,numberOfPolarizationVectors2-1
    fpv2(n,jv)=0.
  end do
end do

! ----------------- START LOOP OVER INTERFACE -------------------------
beginLoopsMask2d()

  ! here is the normal (assumed to be the same on both sides)
  an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
  an2=rsxy1(i1,i2,i3,axis1,1)
  aNorm=max(epsx,sqrt(an1**2+an2**2))
  an1=an1/aNorm
  an2=an2/aNorm
  tau1=-an2
  tau2= an1

  ! first evaluate the equations we want to solve with the wrong values at the ghost points:
  evalInterfaceDerivatives2d()
  ! if( .true. .or. debug.gt.4 )then 
  !    write(*,'(" START  (i1,i2)=",2i3," v=Ey(-1:1,-1:1)",9e14.6)') i1,i2, ((u1(i1+k1,i2+k2,i3,ey),k1=-1,1),k2=-1,1)
  !    write(*,'(" START    mu1,mu2=",2e10.2," v1y,u1x,v2y,u2x=",4e10.2)') mu1,mu2,v1y,u1x,v2y,u2x
  !  end if

  ! Evaluate TZ forcing for dispersive equations in 2D 
  getDispersiveTZForcing(fpv1,fpv2,fev1,fev2)

  ! eval dispersive forcings for domain 1
  getDispersiveForcingOrder2(i1,i2,i3, fp1, fpv1,fev1,p1,p1n,p1m, u1,u1n,u1m, dispersionModel1,numberOfPolarizationVectors1,alphaP1,beta1,a0v1,a1v1,b0v1,b1v1)

  ! eval dispersive forcings for domain 2
  getDispersiveForcingOrder2(j1,j2,j3, fp2, fpv2,fev2,p2,p2n,p2m, u2,u2n,u2m, dispersionModel2,numberOfPolarizationVectors2,alphaP2,beta2,a0v2,a1v2,b0v2,b1v2)

  ! Evaulate RHS, f(n),n=0,1,2,3 using current ghost values: 
  eval2dJumpDispersiveOrder2()

  ! write(debugFile,'(" --> order2-curv: i1,i2=",2i4," f(start)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)
  ! write(debugFile,'(" --> u1(ghost),u1=",4f8.3)') u1(i1-is1,i2-is2,i3,ex),u1(i1,i2,i3,ex)
  ! write(debugFile,'(" --> u2(ghost),u2=",4f8.3)') u2(j1-js1,j2-js2,j3,ex),u2(j1,j2,j3,ex)
  ! '

  ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
  ! Solve:
  !     
  !       A [ U ] = A [ U(old) ] - [ f ]
  ! ---- EQUATION 0 ----- 
  a4(0,0) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))    ! coeff of u1(-1) from [u.x+v.y] 
  a4(0,1) = -is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))    ! coeff of v1(-1) from [u.x+v.y] 
  a4(0,2) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))    ! coeff of u2(-1) from [u.x+v.y] 
  a4(0,3) =  js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))    ! coeff of v2(-1) from [u.x+v.y] 

  ! ---- EQUATION 2 ----- 
  a4(2,0) =  is*rsxy1(i1,i2,i3,axis1,1)/(2.*dr1(axis1))/mu1   ! coeff of u1(-1) from [(v.x - u.y)/mu] 
  a4(2,1) = -is*rsxy1(i1,i2,i3,axis1,0)/(2.*dr1(axis1))/mu1   ! coeff of v1(-1) from [(v.x - u.y)/mu] 

  a4(2,2) = -js*rsxy2(j1,j2,j3,axis2,1)/(2.*dr2(axis2))/mu2   ! coeff of u2(-1) from [(v.x - u.y)/mu] 
  a4(2,3) =  js*rsxy2(j1,j2,j3,axis2,0)/(2.*dr2(axis2))/mu2   ! coeff of v2(-1) from [(v.x - u.y)/mu] 


  ! coeff of u(-1) from lap = u.xx + u.yy
  rxx1(0,0,0)=aj1rxx
  rxx1(1,0,0)=aj1sxx
  rxx1(0,1,1)=aj1ryy
  rxx1(1,1,1)=aj1syy

  rxx2(0,0,0)=aj2rxx
  rxx2(1,0,0)=aj2sxx
  rxx2(0,1,1)=aj2ryy
  rxx2(1,1,1)=aj2syy

  ! clap1=(rsxy1(i1,i2,i3,axis1,0)**2+rsxy1(i1,i2,i3,axis1,1)**2)/(dr1(axis1)**2) \
  !           -is*(rsxy1x22(i1,i2,i3,axis1,0)+rsxy1y22(i1,i2,i3,axis1,1))/(2.*dr1(axis1))
  ! clap2=(rsxy2(j1,j2,j3,axis2,0)**2+rsxy2(j1,j2,j3,axis2,1)**2)/(dr2(axis2)**2) \
  !             -js*(rsxy2x22(j1,j2,j3,axis2,0)+rsxy2y22(j1,j2,j3,axis2,1))/(2.*dr2(axis2)) 
  clap1=(rsxy1(i1,i2,i3,axis1,0)**2+rsxy1(i1,i2,i3,axis1,1)**2)/(dr1(axis1)**2) \
            -is*(rxx1(axis1,0,0)+rxx1(axis1,1,1))/(2.*dr1(axis1))
  clap2=(rsxy2(j1,j2,j3,axis2,0)**2+rsxy2(j1,j2,j3,axis2,1)**2)/(dr2(axis2)**2) \
            -js*(rxx2(axis2,0,0)+rxx2(axis2,1,1))/(2.*dr2(axis2)) 

  ! ---- EQUATION 1 ----- 
  !   [ n.(uv.xx + u.yy)/mu ] = 0
  a4(1,0) = an1*clap1/mu1
  a4(1,1) = an2*clap1/mu1
  a4(1,2) =-an1*clap2/mu2
  a4(1,3) =-an2*clap2/mu2

  ! ---- EQUATION 3 ----- 
  !   [ tau.(uv.xx+uv.yy)*beta/(eps*mu) + ... ] = 0
  a4(3,0) = tau1*clap1*beta1/epsmu1
  a4(3,1) = tau2*clap1*beta1/epsmu1
  a4(3,2) =-tau1*clap2*beta2/epsmu2
  a4(3,3) =-tau2*clap2*beta2/epsmu2
    

   if( .false. .or. debug.gt.4 )then 
     write(*,'("BEFORE: --> i1,i2=",2i4," j1,j2=",2i4," f()=",4e10.2)') i1,i2,j1,j2,f(0),f(1),f(2),f(3)
     write(*,'("     beta1,beta2=",2e10.2," fp1=",2e10.2," fp2=",2e10.2)') beta1,beta2,fp1(0),fp1(1),fp2(0),fp2(1)
     write(*,'("     mu1,mu2=",2e10.2," v1y,u1x,v2y,u2x=",4e10.2)') mu1,mu2,v1y,u1x,v2y,u2x
   end if

  q(0) = u1(i1-is1,i2-is2,i3,ex)
  q(1) = u1(i1-is1,i2-is2,i3,ey)
  q(2) = u2(j1-js1,j2-js2,j3,ex)
  q(3) = u2(j1-js1,j2-js2,j3,ey)

  ! write(debugFile,'(" --> xy1=",4f8.3)') xy1(i1,i2,i3,0),xy1(i1,i2,i3,1)
  ! write(debugFile,'(" --> rsxy1=",4f8.3)') rsxy1(i1,i2,i3,0,0),rsxy1(i1,i2,i3,1,0),rsxy1(i1,i2,i3,0,1),rsxy1(i1,i2,i3,1,1)
  ! write(debugFile,'(" --> rsxy2=",4f8.3)') rsxy2(j1,j2,j3,0,0),rsxy2(j1,j2,j3,1,0),rsxy2(j1,j2,j3,0,1),rsxy2(j1,j2,j3,1,1)

  ! write(debugFile,'(" --> rxx1=",2f8.3)') rxx1(axis1,0,0),rxx1(axis1,1,1)
  ! write(debugFile,'(" --> rxx2=",2f8.3)') rxx2(axis2,0,0),rxx2(axis1,1,1)

  ! write(debugFile,'(" --> a4(0,.)=",4f8.3)') a4(0,0),a4(0,1),a4(0,2),a4(0,3)
  ! write(debugFile,'(" --> a4(1,.)=",4f8.3)') a4(1,0),a4(1,1),a4(1,2),a4(1,3)
  ! write(debugFile,'(" --> a4(2,.)=",4f8.3)') a4(2,0),a4(2,1),a4(2,2),a4(2,3)
  ! write(debugFile,'(" --> a4(3,.)=",4f8.3)') a4(3,0),a4(3,1),a4(3,2),a4(3,3)
  ! write(debugFile,'(" --> an1,an2=",2f8.3)') an1,an2
  ! write(debugFile,'(" --> clap1,clap2=",2f8.3)') clap1,clap2
  ! subtract off the contributions from the wrong values at the ghost points:
  do n=0,3
    f(n) = (a4(n,0)*q(0)+a4(n,1)*q(1)+a4(n,2)*q(2)+a4(n,3)*q(3)) - f(n)
  end do
  ! write(debugFile,'(" --> order2-curv: i1,i2=",2i4," f(subtract)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)
  ! solve A Q = F
  ! factor the matrix
  numberOfEquations=4
  call dgeco( a4(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0))
  ! solve
  ! write(debugFile,'(" --> order2-curv: i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
  job=0
  call dgesl( a4(0,0), numberOfEquations, numberOfEquations, ipvt(0), f(0), job)
  ! write(debugFile,'(" --> order2-curv: i1,i2=",2i4," f(solve)=",4f8.3)') i1,i2,f(0),f(1),f(2),f(3)

  u1(i1-is1,i2-is2,i3,ex)=f(0)
  u1(i1-is1,i2-is2,i3,ey)=f(1)
  u2(j1-js1,j2-js2,j3,ex)=f(2)
  u2(j1-js1,j2-js2,j3,ey)=f(3)

  if( .false. .or. debug.gt.3 )then ! re-evaluate
    evalInterfaceDerivatives2d()
    eval2dJumpDispersiveOrder2()
    !write(debugFile,'(" --> order2-curv: xy1(ghost)=",2e11.3)') xy1(i1-is1,i2-is2,i3,0),xy1(i1-is1,i2-is2,i3,1)
    !write(debugFile,'(" --> order2-curv: xy2(ghost)=",2e11.3)') xy2(j1-js1,j2-js2,j3,0),xy2(j1-js1,j2-js2,j3,1)

    write(*,'("AFTER: --> i1,i2=",2i4," j1,j2=",2i4," f(re-eval)=",4e10.2)') i1,i2,j1,j2,f(0),f(1),f(2),f(3)

    if( twilightZone.eq.1 )then
      ! check errors in the ghost 
        k1=i1-is1
        k2=i2-is2
        k3=i3
        do n=0,nd-1
          call ogderiv(ep, 0,0,0,0, xy1(k1,k2,k3,0),xy1(k1,k2,k3,1),0.,t,ex+n, es(n)   ) 
        end do
        f(0) =  u1(i1-is1,i2-is2,i3,ex) -es(0)
        f(1) =  u1(i1-is1,i2-is2,i3,ey) -es(1)
        k1=j1-js1
        k2=j2-js2
        k3=j3
        do n=0,nd-1
          call ogderiv(ep, 0,0,0,0, xy2(k1,k2,k3,0),xy2(k1,k2,k3,1),0.,t,ex+n, est(n)   ) 
        end do
        f(2) =  u2(j1-js1,j2-js2,j3,ex) -est(0)
        f(3) =  u2(j1-js1,j2-js2,j3,ey) -est(1)
        write(*,'(" ghost err =",4e10.2)') f(0),f(1),f(2),f(3) 

    end if

  end if

  ! -- Hz has already been filled in by extrapolation ----

endLoopsMask2d()

 ! stop 9876

#endMacro

