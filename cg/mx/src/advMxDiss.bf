c
c Define the functions that compute the dissipation and divergence cleaning as
c separate steps
c
c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"


#beginMacro loopse6(e1,e2,e3,e4,e5,e6)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
      e4
      e5
      e6
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
      e1
      e2
      e3
      e4
      e5
      e6
  end do
  end do
  end do
end if
#endMacro


#beginMacro loopse9(e1,e2,e3,e4,e5,e6,e7,e8,e9)
if( useWhereMask.ne.0 )then
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
   e1
   e2
   e3
   e4
   e5
   e6
   e7
   e8
   e9
  end if
 end do
 end do
 end do
else
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  e1
  e2
  e3
  e4
  e5
  e6
  e7
  e8
  e9
 end do
 end do
 end do
end if
#endMacro

#beginMacro loopse18(e1,e2,e3,e4,e5,e6,e7,e8,e9,e10,e11,e12,e13,e14,e15,e16,e17,e18)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
      e4
      e5
      e6
      e7
      e8
      e9
      e10
      e11
      e12
      e13
      e14
      e15
      e16
      e17
      e18
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
      e1
      e2
      e3
      e4
      e5
      e6
      e7
      e8
      e9
      e10
      e11
      e12
      e13
      e14
      e15
      e16
      e17
      e18
  end do
  end do
  end do
end if
#endMacro


c This macro is used for variable dissipation in 2D
#beginMacro loopse6VarDis(e1,e2,e3,e4,e5,e6)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( varDis(i1,i2,i3).gt.0. .and. mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
c     write(*,'(" i=",3i3," varDis=",e10.2," diss=",3e10.2)') i1,i2,i3,varDis(i1,i2,i3),dis(i1,i2,i3,ex),\
c         dis(i1,i2,i3,ey),dis(i1,i2,i3,ez)
    else
      e4
      e5
      e6
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( varDis(i1,i2,i3).gt.0. )then
      e1
      e2
      e3
    else
      e4
      e5
      e6
    end if
  end do
  end do
  end do
end if
#endMacro

c =================================================================================================
c This macro is used for variable dissipation in 3D
c =================================================================================================
#beginMacro loopse12VarDis(e1,e2,e3,e4,e5,e6,e7,e8,e9,e10,e11,e12)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( varDis(i1,i2,i3).gt.0. .and. mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
      e7
      e8
      e9
    else
      e4
      e5
      e6
      e10
      e11
      e12
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( varDis(i1,i2,i3).gt.0. )then
      e1
      e2
      e3
      e7
      e8
      e9
    else
      e4
      e5
      e6
      e10
      e11
      e12
    end if
  end do
  end do
  end do
end if
#endMacro

c =================================================================================================
c This macro is used for variable dissipation in 3D
c =================================================================================================
#beginMacro loopsVarDis3D(e1,e2,e3,e4,e5,e6,h1,h2,h3,h4,h5,h6)

 if( solveForE.ne.0 .and. solveForH.ne.0 )then
   loopse12VarDis(e1,e2,e3,e4,e5,e6,h1,h2,h3,h4,h5,h6)
 else if( solveForE.ne.0 ) then
   loopse6VarDis(e1,e2,e3,e4,e5,e6)
 else
   loopse6VarDis(h1,h2,h3,h4,h5,h6)
 end if

#endMacro

c =================================================================================================
c Optionally add the forcing terms
c Optionally solve for E or H or both
c =================================================================================================
#beginMacro loopsF3D(fe1,fe2,fe3,e1,e2,e3,e4,e5,e6,e7,e8,e9,fh1,fh2,fh3,h1,h2,h3,h4,h5,h6,h7,h8,h9)
if( addForcing.eq.0 )then

  if( solveForE.ne.0 .and. solveForH.ne.0 )then
    loopse18(e1,e2,e3,e4,e5,e6,e7,e8,e9,h1,h2,h3,h4,h5,h6,h7,h8,h9)
  else if( solveForE.ne.0 ) then
    loopse9(e1,e2,e3,e4,e5,e6,e7,e8,e9)
  else
    loopse9(h1,h2,h3,h4,h5,h6,h7,h8,h9)
  end if

else
c add forcing to the equations

  if( solveForE.ne.0 .and. solveForH.ne.0 )then
    loopse18(e1+fe1,e2+fe2,e3+fe3,e4,e5,e6,e7,e8,e9,h1+fh1,h2+fh2,h3+fh3,h4,h5,h6,h7,h8,h9)
  else if( solveForE.ne.0 ) then
    loopse9(e1+fe1,e2+fe2,e3+fe3,e4,e5,e6,e7,e8,e9)
  else
    loopse9(h1+fh1,h2+fh2,h3+fh3,h4,h5,h6,h7,h8,h9)
  end if

end if
#endMacro

c **********************************************************************************
c NAME: name of the subroutine
c DIM : 2 or 3
c ORDER : 2 ,4, 6 or 8
c **********************************************************************************
#beginMacro DIS_MAXWELL(NAME,DIM,ORDER)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                 mask,rsxy,  um,u,un,f, v,vvt2,ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
c======================================================================
c   This function computes the dissipation for the time advance of Maxwells equations
c 
c nd : number of space dimensions
c
c ipar(0)  = option : option=0 - Maxwell+Artificial diffusion
c                           =1 - AD only
c
c  dis(i1,i2,i3) : temp space to hold artificial dissipation
c  varDis(i1,i2,i3) : coefficient of the variable artificial dissipation
c======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

 real um(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real vvt2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real ut3(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real vvt4(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real ut5(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real ut6(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real ut7(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real dis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real varDis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:2),ierr

 integer ipar(0:*)
 real rpar(0:*)
      
c     ---- local variables -----
 integer c,i1,i2,i3,n,gridType,orderOfAccuracy,orderInTime
 integer addForcing,orderOfDissipation,option
 integer useWhereMask,useWhereMaskSave,solveForE,solveForH,grid,useVariableDissipation
 integer useCurvilinearOpt,useConservative,combineDissipationWithAdvance,useDivergenceCleaning
 integer ex,ey,ez, hx,hy,hz
 real t,cc,dt,dy,dz,cdt,cdtdx,cdtdy,cdtdz,adc,adcdt,add,adddt
 real dt4by12
 real eps,mu,sigmaE,sigmaH,kx,ky,kz,divergenceCleaningCoefficient

 real dx(0:2),dr(0:2)

 real dx2i,dy2i,dz2i,dxsqi,dysqi,dzsqi,dxi,dyi,dzi
 real dx12i,dy12i,dz12i,dxsq12i,dysq12i,dzsq12i,dxy4i,dxz4i,dyz4,time0,time1

 real dxi4,dyi4,dzi4,dxdyi2,dxdzi2,dydzi2

 real c0,c1,csq,dtsq,cdtsq,cdtsq12

 integer rectangular,curvilinear
 parameter( rectangular=0, curvilinear=1 )

 integer timeSteppingMethod
 integer defaultTimeStepping,adamsSymmetricOrder3,rungeKuttaFourthOrder,\
         stoermerTimeStepping,modifiedEquationTimeStepping
 parameter(defaultTimeStepping=0,adamsSymmetricOrder3=1,\
           rungeKuttaFourthOrder=2,stoermerTimeStepping=3,modifiedEquationTimeStepping=4)


c...........start statement function
 integer kd,m
 real rx,ry,rz,sx,sy,sz,tx,ty,tz

 declareDifferenceOrder2(u,RX)
 declareDifferenceOrder2(un,none)
! declareDifferenceOrder2(v,none)

 declareDifferenceOrder4(u,RX)
 declareDifferenceOrder4(un,none)
! declareDifferenceOrder4(v,none)


 real du,fd22d,fd23d,fd42d,fd43d,fd62d,fd63d,fd82d,fd83d

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

c     The next macro will define the difference approximation statement functions
 defineDifferenceOrder2Components1(u,RX)
 defineDifferenceOrder4Components1(u,RX)

 defineDifferenceOrder2Components1(un,none)
 defineDifferenceOrder4Components1(un,none)

! defineDifferenceOrder2Components1(v,none)
! defineDifferenceOrder4Components1(v,none)


c ******* artificial dissipation ******
 du(i1,i2,i3,c)=u(i1,i2,i3,c)-um(i1,i2,i3,c)

c      (2nd difference)
 fd22d(i1,i2,i3,c)= \
 (     ( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) ) \
  -4.*du(i1,i2,i3,c) )
c
 fd23d(i1,i2,i3,c)=\
 (     ( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) \
   -6.*du(i1,i2,i3,c) )

c     -(fourth difference)
 fd42d(i1,i2,i3,c)= \
 (    -( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c) ) \
   +4.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) ) \
  -12.*du(i1,i2,i3,c) )
c
 fd43d(i1,i2,i3,c)=\
 (    -( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c)+du(i1,i2,i3-2,c)+du(i1,i2,i3+2,c) ) \
   +4.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) \
  -18.*du(i1,i2,i3,c) )

 ! (sixth  difference)
 fd62d(i1,i2,i3,c)= \
 (     ( du(i1-3,i2,i3,c)+du(i1+3,i2,i3,c)+du(i1,i2-3,i3,c)+du(i1,i2+3,i3,c) ) \
   -6.*( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c) ) \
  +15.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) ) \
  -40.*du(i1,i2,i3,c) )

 fd63d(i1,i2,i3,c)=\
 (     ( du(i1-3,i2,i3,c)+du(i1+3,i2,i3,c)+du(i1,i2-3,i3,c)+du(i1,i2+3,i3,c)+du(i1,i2,i3-3,c)+du(i1,i2,i3+3,c) ) \
   -6.*( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c)+du(i1,i2,i3-2,c)+du(i1,i2,i3+2,c) ) \
  +15.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) \
  -60.*du(i1,i2,i3,c) )

 ! -(eighth  difference)
 fd82d(i1,i2,i3,c)= \
 (    -( du(i1-4,i2,i3,c)+du(i1+4,i2,i3,c)+du(i1,i2-4,i3,c)+du(i1,i2+4,i3,c) ) \
   +8.*( du(i1-3,i2,i3,c)+du(i1+3,i2,i3,c)+du(i1,i2-3,i3,c)+du(i1,i2+3,i3,c) ) \
  -28.*( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c) ) \
  +56.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) ) \
 -140.*du(i1,i2,i3,c) )

 fd83d(i1,i2,i3,c)=\
 (    -( du(i1-4,i2,i3,c)+du(i1+4,i2,i3,c)+du(i1,i2-4,i3,c)+du(i1,i2+4,i3,c)+du(i1,i2,i3-4,c)+du(i1,i2,i3+4,c) ) \
   +8.*( du(i1-3,i2,i3,c)+du(i1+3,i2,i3,c)+du(i1,i2-3,i3,c)+du(i1,i2+3,i3,c)+du(i1,i2,i3-3,c)+du(i1,i2,i3+3,c) ) \
  -28.*( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c)+du(i1,i2,i3-2,c)+du(i1,i2,i3+2,c) ) \
  +56.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) \
 -210.*du(i1,i2,i3,c) )


c...........end   statement functions


 ! write(*,*) 'Inside advMxDiss...'

 cc    =rpar(0)  ! this is c
 dt    =rpar(1)
 dx(0) =rpar(2)
 dx(1) =rpar(3)
 dx(2) =rpar(4)
 adc   =rpar(5)  ! coefficient of artificial dissipation
 add   =rpar(6)  ! coefficient of divergence damping    
 dr(0) =rpar(7)
 dr(1) =rpar(8)
 dr(2) =rpar(9)
 eps   =rpar(10)
 mu    =rpar(11) 
 kx    =rpar(12) 
 ky    =rpar(13) 
 kz    =rpar(14) 
 sigmaE=rpar(15)  ! electric conductivity (for lossy materials, complex index of refraction)
 sigmaH=rpar(16)  ! magnetic conductivity
 divergenceCleaningCoefficient=rpar(17)
 t     =rpar(18)

 rpar(20)=0.  ! return the time used for adding dissipation

 dy=dx(1)  ! Are these needed?
 dz=dx(2)

 ! timeForArtificialDissipation=rpar(6) ! return value

 option             =ipar(0)
 gridType           =ipar(1)
 orderOfAccuracy    =ipar(2)
 orderInTime        =ipar(3)
 addForcing         =ipar(4)
 orderOfDissipation =ipar(5)
 ex                 =ipar(6)
 ey                 =ipar(7)
 ez                 =ipar(8)
 hx                 =ipar(9)
 hy                 =ipar(10)
 hz                 =ipar(11)
 solveForE          =ipar(12)
 solveForH          =ipar(13)
 useWhereMask       =ipar(14)
 timeSteppingMethod =ipar(15)
 useVariableDissipation=ipar(16)
 useCurvilinearOpt  =ipar(17)
 useConservative    =ipar(18)   
 combineDissipationWithAdvance = ipar(19)
 useDivergenceCleaning=ipar(20)

 csq=cc**2
 dtsq=dt**2

 cdt=cc*dt

 cdtsq=(cc**2)*(dt**2)
 cdtsq12=cdtsq*cdtsq/12.

 dt4by12=dtsq*dtsq/12.

 cdtdx = (cc*dt/dx(0))**2
 cdtdy = (cc*dt/dy)**2
 cdtdz = (cc*dt/dz)**2

 dxsqi=1./(dx(0)**2)
 dysqi=1./(dy**2)
 dzsqi=1./(dz**2)

 dxsq12i=1./(12.*dx(0)**2)
 dysq12i=1./(12.*dy**2)
 dzsq12i=1./(12.*dz**2)

 dxi4=1./(dx(0)**4)
 dyi4=1./(dy**4)
 dxdyi2=1./(dx(0)*dx(0)*dy*dy)

 dzi4=1./(dz**4)
 dxdzi2=1./(dx(0)*dx(0)*dz*dz)
 dydzi2=1./(dy*dy*dz*dz)


 #If #DIM eq "2" 
   if( nd.ne.2 )then
     stop 70707
   end if
 #End
 #If #DIM eq "3" 
   if( nd.ne.3 )then
     stop 80808
   end if
 #End
 #If #ORDER eq "2"
   if( orderOfAccuracy.ne.2 )then
     stop 71717
   end if
 #End
 #If #ORDER eq "4"
   if( orderOfAccuracy.ne.4 )then
     stop 71717
   end if
 #End

 if( adc.gt.0. .and. combineDissipationWithAdvance.eq.0 )then
   ! ********************************************************************************************************
   ! ********************* Compute the dissipation and fill in the dis(i1,i2,i3,c) array ********************
   ! ********************************************************************************************************

   call ovtime( time0 )

  ! Here we assume that a (2m)th order method will only use dissipation of (2m) or (2m+2)
  if( orderOfDissipation.eq.4 )then
  #If #ORDER eq "2" || #ORDER eq "4"

     ! write(*,*) 'advMxDiss: add dissipation separately... orderOfDissipation=4, orderOfAccuracy=#ORDER'


     adcdt=adc*dt
     #If #DIM eq "2"
      if( useVariableDissipation.eq.0 )then
       loopse9(dis(i1,i2,i3,ex)=adcdt*fd42d(i1,i2,i3,ex),\
               dis(i1,i2,i3,ey)=adcdt*fd42d(i1,i2,i3,ey),\
               dis(i1,i2,i3,hz)=adcdt*fd42d(i1,i2,i3,hz),,,,,,)
      else
       ! write(*,'(" advOpt: apply 4th-order variable dissipation...")') 
       loopse6VarDis(dis(i1,i2,i3,ex)=adcdt*varDis(i1,i2,i3)*fd42d(i1,i2,i3,ex),\
                     dis(i1,i2,i3,ey)=adcdt*varDis(i1,i2,i3)*fd42d(i1,i2,i3,ey),\
                     dis(i1,i2,i3,hz)=adcdt*varDis(i1,i2,i3)*fd42d(i1,i2,i3,hz),\
                     dis(i1,i2,i3,ex)=0.,dis(i1,i2,i3,ey)=0.,dis(i1,i2,i3,hz)=0.)
      end if
     #Else
      if( useVariableDissipation.eq.0 )then
       loopsF3D(0,0,0,\
                dis(i1,i2,i3,ex)=adcdt*fd43d(i1,i2,i3,ex),\
                dis(i1,i2,i3,ey)=adcdt*fd43d(i1,i2,i3,ey),\
                dis(i1,i2,i3,ez)=adcdt*fd43d(i1,i2,i3,ez),,,,,,,\
                0,0,0,\
                dis(i1,i2,i3,hx)=adcdt*fd43d(i1,i2,i3,hx),\
                dis(i1,i2,i3,hy)=adcdt*fd43d(i1,i2,i3,hy),\
                dis(i1,i2,i3,hz)=adcdt*fd43d(i1,i2,i3,hz),,,,,,)
      else
       loopsVarDis3D(\
                dis(i1,i2,i3,ex)=adcdt*varDis(i1,i2,i3)*fd43d(i1,i2,i3,ex),\
                dis(i1,i2,i3,ey)=adcdt*varDis(i1,i2,i3)*fd43d(i1,i2,i3,ey),\
                dis(i1,i2,i3,ez)=adcdt*varDis(i1,i2,i3)*fd43d(i1,i2,i3,ez),\
                dis(i1,i2,i3,ex)=0.,dis(i1,i2,i3,ey)=0.,dis(i1,i2,i3,ez)=0.,\
                dis(i1,i2,i3,hx)=adcdt*varDis(i1,i2,i3)*fd43d(i1,i2,i3,hx),\
                dis(i1,i2,i3,hy)=adcdt*varDis(i1,i2,i3)*fd43d(i1,i2,i3,hy),\
                dis(i1,i2,i3,hz)=adcdt*varDis(i1,i2,i3)*fd43d(i1,i2,i3,hz),\
                dis(i1,i2,i3,hx)=0.,dis(i1,i2,i3,hy)=0.,dis(i1,i2,i3,hz)=0.)
      end if
     #End  
  #End
  #If #ORDER eq "4" || #ORDER eq "6"
   else if( orderOfDissipation.eq.6 )then
     adcdt=adc*dt
     #If #DIM eq "2"
       loopse9(dis(i1,i2,i3,ex)=adcdt*fd62d(i1,i2,i3,ex),\
               dis(i1,i2,i3,ey)=adcdt*fd62d(i1,i2,i3,ey),\
               dis(i1,i2,i3,hz)=adcdt*fd62d(i1,i2,i3,hz),,,,,,)
     #Else
       loopsF3D(0,0,0,\
                dis(i1,i2,i3,ex)=adcdt*fd63d(i1,i2,i3,ex),\
                dis(i1,i2,i3,ey)=adcdt*fd63d(i1,i2,i3,ey),\
                dis(i1,i2,i3,ez)=adcdt*fd63d(i1,i2,i3,ez),,,,,,,\
                0,0,0,\
                dis(i1,i2,i3,hx)=adcdt*fd63d(i1,i2,i3,hx),\
                dis(i1,i2,i3,hy)=adcdt*fd63d(i1,i2,i3,hy),\
                dis(i1,i2,i3,hz)=adcdt*fd63d(i1,i2,i3,hz),,,,,,)
     #End
  #End
  #If #ORDER eq "6" || #ORDER eq "8"
   else if( orderOfDissipation.eq.8 )then
     adcdt=adc*dt
     #If #DIM eq "2"
       loopse9(dis(i1,i2,i3,ex)=adcdt*fd82d(i1,i2,i3,ex),\
               dis(i1,i2,i3,ey)=adcdt*fd82d(i1,i2,i3,ey),\
               dis(i1,i2,i3,hz)=adcdt*fd82d(i1,i2,i3,hz),,,,,,)
     #Else
       loopsF3D(0,0,0,\
                dis(i1,i2,i3,ex)=adcdt*fd83d(i1,i2,i3,ex),\
                dis(i1,i2,i3,ey)=adcdt*fd83d(i1,i2,i3,ey),\
                dis(i1,i2,i3,ez)=adcdt*fd83d(i1,i2,i3,ez),,,,,,,\
                0,0,0,\
                dis(i1,i2,i3,hx)=adcdt*fd83d(i1,i2,i3,hx),\
                dis(i1,i2,i3,hy)=adcdt*fd83d(i1,i2,i3,hy),\
                dis(i1,i2,i3,hz)=adcdt*fd83d(i1,i2,i3,hz),,,,,,)
     #End
  #End
  #If #ORDER eq "2" 
   else if( orderOfDissipation.eq.2 )then
     adcdt=adc*dt
     #If #DIM eq "2"
      if( useVariableDissipation.eq.0 )then
       loopse9(dis(i1,i2,i3,ex)=adcdt*fd22d(i1,i2,i3,ex),\
               dis(i1,i2,i3,ey)=adcdt*fd22d(i1,i2,i3,ey),\
               dis(i1,i2,i3,hz)=adcdt*fd22d(i1,i2,i3,hz),,,,,,)
      else
        stop 33333
      end if
    #Else
      if( useVariableDissipation.eq.0 )then
       loopsF3D(0,0,0,\
                dis(i1,i2,i3,ex)=adcdt*fd23d(i1,i2,i3,ex),\
                dis(i1,i2,i3,ey)=adcdt*fd23d(i1,i2,i3,ey),\
                dis(i1,i2,i3,ez)=adcdt*fd23d(i1,i2,i3,ez),,,,,,,\
                0,0,0,\
                dis(i1,i2,i3,hx)=adcdt*fd23d(i1,i2,i3,hx),\
                dis(i1,i2,i3,hy)=adcdt*fd23d(i1,i2,i3,hy),\
                dis(i1,i2,i3,hz)=adcdt*fd23d(i1,i2,i3,hz),,,,,,)
      else
        stop 22855
      end if
     #End
  #End
   else if( orderOfAccuracy.eq.4 .and. orderOfDissipation.ge.6 )then
    ! this case is done elsewhere
   else
     write(*,*) 'advMxDiss:ERROR orderOfDissipation=',orderOfDissipation
     write(*,*) 'advMxDiss:orderOfAccuracy=',orderOfAccuracy
     stop 5
   end if
   call ovtime( time1 )
   rpar(20)=time1-time0
 end if

c *****************************************

 if( add.gt.0. )then
c         Here we add the divergence damping 
   call ovtime( time0 )

  if( adc.le.0. )then
    write(*,'(" ERROR: art. dissipation should be on if div. damping is on -- this could be fixed")')
      ! '
    stop 12345
  end if

  ! write(*,*) 'Inside advMxDiss: divergence damping add=',add

  adddt=add*dt  ! we should probably scale by c here as well ??
  #If #ORDER eq "2" 
   #If #DIM eq "2"
     if( gridType.eq.rectangular )then
       loopse9(dis(i1,i2,i3,ex)=dis(i1,i2,i3,ex)+adddt*(( uxx22r(i1,i2,i3,ex)+ uxy22r(i1,i2,i3,ey))\
                                                       -(unxx22r(i1,i2,i3,ex)+unxy22r(i1,i2,i3,ey))),\
               dis(i1,i2,i3,ey)=dis(i1,i2,i3,ey)+adddt*(( uxy22r(i1,i2,i3,ex)+ uyy22r(i1,i2,i3,ey))\
                                                       -(unxy22r(i1,i2,i3,ex)+unyy22r(i1,i2,i3,ey))),\
               ,,,,,,)
     else
       loopse9(dis(i1,i2,i3,ex)=dis(i1,i2,i3,ex)+adddt*(( uxx22(i1,i2,i3,ex)+ uxy22(i1,i2,i3,ey))\
                                                       -(unxx22(i1,i2,i3,ex)+unxy22(i1,i2,i3,ey))),\
               dis(i1,i2,i3,ey)=dis(i1,i2,i3,ey)+adddt*(( uxy22(i1,i2,i3,ex)+ uyy22(i1,i2,i3,ey))\
                                                       -(unxy22(i1,i2,i3,ex)+unyy22(i1,i2,i3,ey))),\
               ,,,,,,)
     end if
   #Else
     if( gridType.eq.rectangular )then
       loopse9(dis(i1,i2,i3,ex)=dis(i1,i2,i3,ex)+adddt*(( uxx23r(i1,i2,i3,ex)+ uxy23r(i1,i2,i3,ey)+ uxz23r(i1,i2,i3,ez))\
                                                       -(unxx23r(i1,i2,i3,ex)+unxy23r(i1,i2,i3,ey)+unxz23r(i1,i2,i3,ez))),\
               dis(i1,i2,i3,ey)=dis(i1,i2,i3,ey)+adddt*(( uxy23r(i1,i2,i3,ex)+ uyy23r(i1,i2,i3,ey)+ uyz23r(i1,i2,i3,ez))\
                                                       -(unxy23r(i1,i2,i3,ex)+unyy23r(i1,i2,i3,ey)+unyz23r(i1,i2,i3,ez))),\
               dis(i1,i2,i3,ez)=dis(i1,i2,i3,ez)+adddt*(( uxz23r(i1,i2,i3,ex)+ uyz23r(i1,i2,i3,ey)+ uzz23r(i1,i2,i3,ez))\
                                                       -(unxz23r(i1,i2,i3,ex)+unyz23r(i1,i2,i3,ey)+unzz23r(i1,i2,i3,ez))),\
               ,,,,,)
     else
       loopse9(dis(i1,i2,i3,ex)=dis(i1,i2,i3,ex)+adddt*(( uxx23(i1,i2,i3,ex)+ uxy23(i1,i2,i3,ey)+ uxz23(i1,i2,i3,ez))\
                                                       -(unxx23(i1,i2,i3,ex)+unxy23(i1,i2,i3,ey)+unxz23(i1,i2,i3,ez))),\
               dis(i1,i2,i3,ey)=dis(i1,i2,i3,ey)+adddt*(( uxy23(i1,i2,i3,ex)+ uyy23(i1,i2,i3,ey)+ uyz23(i1,i2,i3,ez))\
                                                       -(unxy23(i1,i2,i3,ex)+unyy23(i1,i2,i3,ey)+unyz23(i1,i2,i3,ez))),\
               dis(i1,i2,i3,ez)=dis(i1,i2,i3,ez)+adddt*(( uxz23(i1,i2,i3,ex)+ uyz23(i1,i2,i3,ey)+ uzz23(i1,i2,i3,ez))\
                                                       -(unxz23(i1,i2,i3,ex)+unyz23(i1,i2,i3,ey)+unzz23(i1,i2,i3,ez))),\
               ,,,,,)
     end if
   #End
  #Elif #ORDER eq "4"

   #If #DIM eq "2"
c$$$     if( gridType.eq.rectangular )then
c$$$       ! here is linear damping
c$$$       loopse9(dis(i1,i2,i3,ex)=dis(i1,i2,i3,ex)-adddt*(( u(i1,i2,i3,ex)- uy42r(i1,i2,i3,hz))\
c$$$                                                     -(un(i1,i2,i3,ex)-uny42r(i1,i2,i3,hz))),\
c$$$               dis(i1,i2,i3,ey)=dis(i1,i2,i3,ey)-adddt*(( u(i1,i2,i3,ey)+ ux42r(i1,i2,i3,hz))\
c$$$                                                     -(un(i1,i2,i3,ey)+unx42r(i1,i2,i3,hz))),\
c$$$               ,,,,,,)
c$$$     else
c$$$       loopse9(dis(i1,i2,i3,ex)=dis(i1,i2,i3,ex)-adddt*(( u(i1,i2,i3,ex)- uy42(i1,i2,i3,hz))\
c$$$                                                     -(un(i1,i2,i3,ex)-uny42(i1,i2,i3,hz))),\
c$$$               dis(i1,i2,i3,ey)=dis(i1,i2,i3,ey)-adddt*(( u(i1,i2,i3,ey)+ ux42(i1,i2,i3,hz))\
c$$$                                                     -(un(i1,i2,i3,ey)+unx42(i1,i2,i3,hz))),\
c$$$               ,,,,,,)
c$$$     end if
     if( gridType.eq.rectangular )then
       loopse9(dis(i1,i2,i3,ex)=dis(i1,i2,i3,ex)+adddt*(( uxx42r(i1,i2,i3,ex)+ uxy42r(i1,i2,i3,ey))\
                                                       -(unxx42r(i1,i2,i3,ex)+unxy42r(i1,i2,i3,ey))),\
               dis(i1,i2,i3,ey)=dis(i1,i2,i3,ey)+adddt*(( uxy42r(i1,i2,i3,ex)+ uyy42r(i1,i2,i3,ey))\
                                                       -(unxy42r(i1,i2,i3,ex)+unyy42r(i1,i2,i3,ey))),\
               ,,,,,,)
     else
       loopse9(dis(i1,i2,i3,ex)=dis(i1,i2,i3,ex)+adddt*(( uxx42(i1,i2,i3,ex)+ uxy42(i1,i2,i3,ey))\
                                                       -(unxx42(i1,i2,i3,ex)+unxy42(i1,i2,i3,ey))),\
               dis(i1,i2,i3,ey)=dis(i1,i2,i3,ey)+adddt*(( uxy42(i1,i2,i3,ex)+ uyy42(i1,i2,i3,ey))\
                                                       -(unxy42(i1,i2,i3,ex)+unyy42(i1,i2,i3,ey))),\
               ,,,,,,)
     end if
   #Else
     if( gridType.eq.rectangular )then
       loopse9(dis(i1,i2,i3,ex)=dis(i1,i2,i3,ex)+adddt*(( uxx43r(i1,i2,i3,ex)+ uxy43r(i1,i2,i3,ey)+ uxz43r(i1,i2,i3,ez))\
                                                       -(unxx43r(i1,i2,i3,ex)+unxy43r(i1,i2,i3,ey)+unxz43r(i1,i2,i3,ez))),\
               dis(i1,i2,i3,ey)=dis(i1,i2,i3,ey)+adddt*(( uxy43r(i1,i2,i3,ex)+ uyy43r(i1,i2,i3,ey)+ uyz43r(i1,i2,i3,ez))\
                                                       -(unxy43r(i1,i2,i3,ex)+unyy43r(i1,i2,i3,ey)+unyz43r(i1,i2,i3,ez))),\
               dis(i1,i2,i3,ez)=dis(i1,i2,i3,ez)+adddt*(( uxz43r(i1,i2,i3,ex)+ uyz43r(i1,i2,i3,ey)+ uzz43r(i1,i2,i3,ez))\
                                                       -(unxz43r(i1,i2,i3,ex)+unyz43r(i1,i2,i3,ey)+unzz43r(i1,i2,i3,ez))),\
               ,,,,,)
     else
       loopse9(dis(i1,i2,i3,ex)=dis(i1,i2,i3,ex)+adddt*(( uxx43(i1,i2,i3,ex)+ uxy43(i1,i2,i3,ey)+ uxz43(i1,i2,i3,ez))\
                                                       -(unxx43(i1,i2,i3,ex)+unxy43(i1,i2,i3,ey)+unxz43(i1,i2,i3,ez))),\
               dis(i1,i2,i3,ey)=dis(i1,i2,i3,ey)+adddt*(( uxy43(i1,i2,i3,ex)+ uyy43(i1,i2,i3,ey)+ uyz43(i1,i2,i3,ez))\
                                                       -(unxy43(i1,i2,i3,ex)+unyy43(i1,i2,i3,ey)+unyz43(i1,i2,i3,ez))),\
               dis(i1,i2,i3,ez)=dis(i1,i2,i3,ez)+adddt*(( uxz43(i1,i2,i3,ex)+ uyz43(i1,i2,i3,ey)+ uzz43(i1,i2,i3,ez))\
                                                       -(unxz43(i1,i2,i3,ex)+unyz43(i1,i2,i3,ey)+unzz43(i1,i2,i3,ez))),\
               ,,,,,)
     end if
   #End

  #Else
    write(*,*) 'advMxDiss:ERROR order = ORDER'
    stop 5
  #End

   call ovtime( time1 )
   ! ** rpar(10)=time1-time0
 end if


 return
 end

#endMacro


c ************** Now build the different subroutines: ****************


      DIS_MAXWELL(advMxDiss2dOrder2,2,2)
      DIS_MAXWELL(advMxDiss3dOrder2,3,2)

      DIS_MAXWELL(advMxDiss2dOrder4,2,4)
      DIS_MAXWELL(advMxDiss3dOrder4,3,4)








