c
c Advance the equations of solid mechanics
c
c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"

! ogf2d, ogf3d, ogDeriv2, etc. are foundin forcing.bC
#beginMacro OGF2D(i1,i2,i3,t,u0,v0)
 call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,u0,v0)
#endMacro

#beginMacro OGF3D(i1,i2,i3,t,u0,v0,w0)
 call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,u0,v0,w0)
#endMacro

! ntd,nxd,nyd,nzd : number of derivatives to evaluate in t,x,y,z
#beginMacro OGDERIV2D(ntd,nxd,nyd,nzd,i1,i2,i3,t,ux,vx)
  call ogDeriv2(ep, ntd,nxd,nyd,nzd, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t, uc,ux, vc,vx)
#endMacro

#beginMacro OGDERIV3D(ntd,nxd,nyd,nzd,i1,i2,i3,t,ux,vx,wx)
  call ogDeriv3(ep, ntd,nxd,nyd,nzd, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t, uc,ux, vc,vx, wc,wx)
#endMacro


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
c     write(*,'(" i=",3i3," varDis=",e10.2," diss=",3e10.2)') i1,i2,i3,varDis(i1,i2,i3),dis(i1,i2,i3,uc),\
c         dis(i1,i2,i3,vc),dis(i1,i2,i3,wc)
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

c This macro is used for variable dissipation in 3D
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

c This macro is used for variable dissipation in 3D
#beginMacro loopsVarDis3D(e1,e2,e3,e4,e5,e6)

 loopse6VarDis(e1,e2,e3,e4,e5,e6)

#endMacro



c Optionally add the forcing terms
#beginMacro loopsF2D(f1,f2,f3,e1,e2,e3,e4,e5,e6,e7,e8,e9)
if( addForcing.eq.0 )then
  loopse9(e1,e2,e3,e4,e5,e6,e7,e8,e9)
else
c add forcing to the first 3 equations
  loopse9(e1+f1,e2+f2,e3+f3,e4,e5,e6,e7,e8,e9)
end if
#endMacro

c Optionally add the forcing terms
c Optionally solve for E or H or both
#beginMacro loopsF3D(fe1,fe2,fe3,e1,e2,e3,e4,e5,e6,e7,e8,e9,fh1,fh2,fh3,h1,h2,h3,h4,h5,h6,h7,h8,h9)
if( addForcing.eq.0 )then

  loopse9(e1,e2,e3,e4,e5,e6,e7,e8,e9)

else
c add forcing to the equations

  loopse9(e1+fe1,e2+fe2,e3+fe3,e4,e5,e6,e7,e8,e9)

end if
#endMacro


c Optionally add the dissipation and or forcing terms
#beginMacro loopsF2DD(f1,f2,f3,e1,e2,e3,e4,e5,e6,e7,e8,e9)
if( addForcing.eq.0 .and. adc.le.0. )then
  loopse9(e1,e2,e3,e4,e5,e6,e7,e8,e9)
else if( addForcing.ne.0 .and. adc.le.0. )then
c add forcing to the first 2 equations
  loopse9(e1+f1,e2+f2,e3,e4,e5,e6,e7,e8,e9)
else if( addForcing.eq.0 .and. adc.gt.0. )then
c add dissipation to the first 3 equations
  loopse9(e1+dis(i1,i2,i3,uc),e2+dis(i1,i2,i3,vc),e3,e4,e5,e6,e7,e8,e9)
else
c  add forcing and dissipation
  loopse9(e1+f1+dis(i1,i2,i3,uc),e2+f2+dis(i1,i2,i3,vc),e3,e4,e5,e6,e7,e8,e9)  
end if
#endMacro


c Optionally add add the dissipation and or forcing terms
c Optionally solve for E or H or both
#beginMacro loopsF3DD(fe1,fe2,fe3,e1,e2,e3,e4,e5,e6,e7,e8,e9)
if( addForcing.eq.0 .and. adc.le.0. )then

  loopse9(e1,e2,e3,e4,e5,e6,e7,e8,e9)

else if( addForcing.ne.0 .and. adc.le.0. )then
c add forcing to the equations

  loopse9(e1+fe1,e2+fe2,e3+fe3,e4,e5,e6,e7,e8,e9)

else if( addForcing.eq.0 .and. adc.gt.0. )then
c add dissipation to the equations

  loopse9(e1+dis(i1,i2,i3,uc),e2+dis(i1,i2,i3,vc),e3+dis(i1,i2,i3,wc),e4,e5,e6,e7,e8,e9)

else
c add dissipation and forcing to the equations

  loopse9(e1+fe1+dis(i1,i2,i3,uc),e2+fe2+dis(i1,i2,i3,vc),e3+fe3+dis(i1,i2,i3,wc),e4,e5,e6,e7,e8,e9)

end if
#endMacro

c The next macro is used for curvilinear girds where the Laplacian term is precomputed.
#beginMacro loopsFC(e1,e2,e3,e4,e5,e6,h1,h2,h3,h4,h5,h6)

if( nd.eq.2 )then
  ! This next line assumes we solve for ex,ey and hz
  loopse9(e1,e2,e4,e5,h3,h6,,,)

else

  if( solveForE.ne.0 .and. solveForH.ne.0 )then
    loopse18(e1,e2,e3,e4,e5,e6,h1,h2,h3,h4,h5,h6,,,,,,)
  else if( solveForE.ne.0 ) then
    loopse9(e1,e2,e3,e4,e5,e6,,,)
  else
    loopse9(h1,h2,h3,h4,h5,h6,,,)
  end if

end if
#endMacro


#defineMacro LAP2D2(U,i1,i2,i3,c) \
                       (U(i1+1,i2,i3,c)-2.*U(i1,i2,i3,c)+U(i1-1,i2,i3,c))*dxsqi\
                      +(U(i1,i2+1,i3,c)-2.*U(i1,i2,i3,c)+U(i1,i2-1,i3,c))*dysqi
#defineMacro LAP3D2(U,i1,i2,i3,c) \
                       (U(i1+1,i2,i3,c)-2.*U(i1,i2,i3,c)+U(i1-1,i2,i3,c))*dxsqi\
                      +(U(i1,i2+1,i3,c)-2.*U(i1,i2,i3,c)+U(i1,i2-1,i3,c))*dysqi\
                      +(U(i1,i2,i3+1,c)-2.*U(i1,i2,i3,c)+U(i1,i2,i3-1,c))*dzsqi

#defineMacro LAP2D2POW2(U,i1,i2,i3,c) ( 6.*U(i1,i2,i3,c)   \
                      - 4.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c))    \
                      +(U(i1+2,i2,i3,c)+U(i1-2,i2,i3,c)) )*dxi4 \
                      +( 6.*U(i1,i2,i3,c)    \
                      -4.*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c))    \
                      +(U(i1,i2+2,i3,c)+U(i1,i2-2,i3,c)) )*dyi4  \
                      +( 8.*U(i1,i2,i3,c)     \
                      -4.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c)+U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c))   \
                      +2.*(U(i1+1,i2+1,i3,c)+U(i1-1,i2+1,i3,c)+U(i1+1,i2-1,i3,c)+U(i1-1,i2-1,i3,c)) )*dxdyi2

#defineMacro LAP3D2POW2(U,i1,i2,i3,c) ( 6.*U(i1,i2,i3,c)   \
        - 4.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c))    \
            +(U(i1+2,i2,i3,c)+U(i1-2,i2,i3,c)) )*dxi4 \
       +(  +6.*U(i1,i2,i3,c)    \
         -4.*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c))    \
            +(U(i1,i2+2,i3,c)+U(i1,i2-2,i3,c)) )*dyi4\
       +(  +6.*U(i1,i2,i3,c)    \
         -4.*(U(i1,i2,i3+1,c)+U(i1,i2,i3-1,c))    \
            +(U(i1,i2,i3+2,c)+U(i1,i2,i3-2,c)) )*dzi4\
        +(8.*U(i1,i2,i3,c)     \
         -4.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c)+U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c))   \
         +2.*(U(i1+1,i2+1,i3,c)+U(i1-1,i2+1,i3,c)+U(i1+1,i2-1,i3,c)+U(i1-1,i2-1,i3,c)) )*dxdyi2 \
        +(8.*U(i1,i2,i3,c)     \
         -4.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c)+U(i1,i2,i3+1,c)+U(i1,i2,i3-1,c))   \
         +2.*(U(i1+1,i2,i3+1,c)+U(i1-1,i2,i3+1,c)+U(i1+1,i2,i3-1,c)+U(i1-1,i2,i3-1,c)) )*dxdzi2 \
        +(8.*U(i1,i2,i3,c)     \
         -4.*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c)+U(i1,i2,i3+1,c)+U(i1,i2,i3-1,c))   \
         +2.*(U(i1,i2+1,i3+1,c)+U(i1,i2-1,i3+1,c)+U(i1,i2+1,i3-1,c)+U(i1,i2-1,i3-1,c)) )*dydzi2 

#defineMacro LAP2D4(U,i1,i2,i3,c) ( -30.*U(i1,i2,i3,c)     \
        +16.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c))     \
            -(U(i1+2,i2,i3,c)+U(i1-2,i2,i3,c)) )*dxsq12i + \
       ( -30.*U(i1,i2,i3,c)     \
        +16.*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c))     \
            -(U(i1,i2+2,i3,c)+U(i1,i2-2,i3,c)) )*dysq12i

#defineMacro LAP3D4(U,i1,i2,i3,c) ( -30.*U(i1,i2,i3,c)     \
        +16.*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c))     \
            -(U(i1+2,i2,i3,c)+U(i1-2,i2,i3,c)) )*dxsq12i + \
       ( -30.*U(i1,i2,i3,c)     \
        +16.*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c))     \
            -(U(i1,i2+2,i3,c)+U(i1,i2-2,i3,c)) )*dysq12i+ \
       ( -30.*U(i1,i2,i3,c)      \
        +16.*(U(i1,i2,i3+1,c)+U(i1,i2,i3-1,c))      \
            -(U(i1,i2,i3+2,c)+U(i1,i2,i3-2,c)) )*dzsq12i

#defineMacro LAP2D6(U,i1,i2,i3,c) \
               c00lap2d6*U(i1,i2,i3,c)     \
              +c10lap2d6*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c)) \
              +c01lap2d6*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c)) \
              +c20lap2d6*(U(i1+2,i2,i3,c)+U(i1-2,i2,i3,c)) \
              +c02lap2d6*(U(i1,i2+2,i3,c)+U(i1,i2-2,i3,c)) \
              +c30lap2d6*(U(i1+3,i2,i3,c)+U(i1-3,i2,i3,c)) \
              +c03lap2d6*(U(i1,i2+3,i3,c)+U(i1,i2-3,i3,c))

#defineMacro LAP3D6(U,i1,i2,i3,c) \
               c000lap3d6*U(i1,i2,i3,c) \
              +c100lap3d6*(U(i1+1,i2,i3,c)+U(i1-1,i2,i3,c)) \
              +c010lap3d6*(U(i1,i2+1,i3,c)+U(i1,i2-1,i3,c)) \
              +c001lap3d6*(U(i1,i2,i3+1,c)+U(i1,i2,i3-1,c)) \
              +c200lap3d6*(U(i1+2,i2,i3,c)+U(i1-2,i2,i3,c)) \
              +c020lap3d6*(U(i1,i2+2,i3,c)+U(i1,i2-2,i3,c)) \
              +c002lap3d6*(U(i1,i2,i3+2,c)+U(i1,i2,i3-2,c)) \
              +c300lap3d6*(U(i1+3,i2,i3,c)+U(i1-3,i2,i3,c)) \
              +c030lap3d6*(U(i1,i2+3,i3,c)+U(i1,i2-3,i3,c)) \
              +c003lap3d6*(U(i1,i2,i3+3,c)+U(i1,i2,i3-3,c))


c ** evaluate the laplacian on the 9 points centred at (i1,i2,i3)
#beginMacro getLapValues2dOrder2(n)
 uLap(-1,-1,n) = uLaplacian22(i1-1,i2-1,i3,n)
 uLap( 0,-1,n) = uLaplacian22(i1  ,i2-1,i3,n)
 uLap(+1,-1,n) = uLaplacian22(i1+1,i2-1,i3,n)

 uLap(-1, 0,n) = uLaplacian22(i1-1,i2  ,i3,n)
 uLap( 0, 0,n) = uLaplacian22(i1  ,i2  ,i3,n)
 uLap(+1, 0,n) = uLaplacian22(i1+1,i2  ,i3,n)

 uLap(-1,+1,n) = uLaplacian22(i1-1,i2+1,i3,n)
 uLap( 0,+1,n) = uLaplacian22(i1  ,i2+1,i3,n)
 uLap(+1,+1,n) = uLaplacian22(i1+1,i2+1,i3,n)
#endMacro


c ** evaluate the square of the Laplacian for a component ****
#beginMacro evalLapSq2dOrder2(n)
 getLapValues2dOrder2(n)
 uLaprr2 = (uLap(+1, 0,n)-2.*uLap( 0, 0,n)+uLap(-1, 0,n))/(dr(0)**2)
 uLapss2 = (uLap( 0,+1,n)-2.*uLap( 0, 0,n)+uLap( 0,-1,n))/(dr(1)**2)
 uLaprs2 = (uLap(+1,+1,n)-uLap(-1,+1,n)-uLap(+1,-1,n)+uLap(-1,-1,n))/(4.*dr(0)*dr(1))
 uLapr2  = (uLap(+1, 0,n)-uLap(-1, 0,n))/(2.*dr(0))
 uLaps2  = (uLap( 0,+1,n)-uLap( 0,-1,n))/(2.*dr(1))

 uLapSq(n) =(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*uLaprr2\
        +2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*uLaprs2\
        +(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*uLapss2\
        +(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*uLapr2\
        +(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*uLaps2
 ! write(*,'(" n : uLaprr2,uLapss2,uLaprs2,uLapr2,uLaps2=",5f6.2)') uLaprr2,uLapss2,uLaprs2,uLapr2,uLaps2
#endMacro

c ** evaluate the square of the Laplacian for [ex,ey,hz] ****
#beginMacro getLapSq2dOrder2()
 evalLapSq2dOrder2(ex)
 evalLapSq2dOrder2(ey)
 evalLapSq2dOrder2(hz)
 ! write(*,'("addForcing,adc=",i2,f5.2,", uLapSq(n)=",3e9.2)') addForcing,adc,uLapSq(ex),uLapSq(ey),uLapSq(hz)
#endMacro

c **********************************************************************************
c NAME: name of the subroutine
c DIM : 2 or 3
c ORDER : 2 ,4, 6 or 8
c GRIDTYPE : rectangular, curvilinear
c **********************************************************************************
#beginMacro ADV_SM(NAME,DIM,ORDER,GRIDTYPE)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                 mask,rsxy,xy,  um,u,un,f, bc, dis, varDis, ipar, rpar, ierr )
c======================================================================
c   Advance a time step for the equations of Solid Mechanics (linear elasticity for now)
c 
c nd : number of space dimensions
c
c ipar(0)  = option : option=0 - Elasticity+Artificial diffusion
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
 real dis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real varDis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
 real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:2),ierr

 integer ipar(0:*)
 real rpar(0:*)
      
c     ---- local variables -----
 integer c,i1,i2,i3,n,gridType,orderOfAccuracy,orderInTime,debug,computeUt
 integer addForcing,orderOfDissipation,option
 integer useWhereMask,useWhereMaskSave,grid,useVariableDissipation
 integer useConservative,combineDissipationWithAdvance
 integer uc,vc,wc
 integer materialFormat,myid

 real cc,dt,dy,dz,cdt,cdtdx,cdtdy,cdtdz,adc,adcdt,add,adddt,dtOld,cu,cum
 real dt4by12
 real kx,ky,kz
 real t,ep
 real dx(0:2),dr(0:2)

 real ux0,vx0,wx0,uy0,vy0,wy0,uz0,vz0,wz0
 real dx2i,dy2i,dz2i,dxsqi,dysqi,dzsqi,dxi,dyi,dzi
 real dx12i,dy12i,dz12i,dxsq12i,dysq12i,dzsq12i,dxy4i,dxz4i,dyz4,time0,time1

 real dxi4,dyi4,dzi4,dxdyi2,dxdzi2,dydzi2

 real uLap(-1:1,-1:1,0:5),uLapSq(0:5)
 real uLaprr2,uLapss2,uLaprs2,uLapr2,uLaps2

 real c0,csq,dtsq,cdtsq,cdtsq12,lap(0:20)
 real c40,c41,c42,c43
 real c60,c61,c62,c63,c64,c65
 real c80,c81,c82,c83,c84,c85,c86,c87

 real c00lap2d6,c10lap2d6,c01lap2d6,c20lap2d6,c02lap2d6,c30lap2d6,c03lap2d6
 real c00lap2d8,c10lap2d8,c01lap2d8,c20lap2d8,c02lap2d8,c30lap2d8,c03lap2d8,c40lap2d8,c04lap2d8
 real c000lap3d6,c100lap3d6,c010lap3d6,c001lap3d6,\
                 c200lap3d6,c020lap3d6,c002lap3d6,\
                 c300lap3d6,c030lap3d6,c003lap3d6
 real c000lap3d8,c100lap3d8,c010lap3d8,c001lap3d8,\
                 c200lap3d8,c020lap3d8,c002lap3d8,\
                 c300lap3d8,c030lap3d8,c003lap3d8,\
                 c400lap3d8,c040lap3d8,c004lap3d8

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

c include 'declareDiffOrder2f.h'
c include 'declareDiffOrder4f.h'
 declareDifferenceOrder2(u,RX)
 declareDifferenceOrder2(un,none)
 declareDifferenceOrder2(v,none)

 declareDifferenceOrder4(u,RX)
 declareDifferenceOrder4(un,none)
 declareDifferenceOrder4(v,none)

 real sm22ru,sm22rv,       sm22u,sm22v
 real sm23ru,sm23rv,sm23rw,sm23u,sm23v,sm23w

 real sm42ru,sm42rv,       sm42u,sm42v
 real sm43ru,sm43rv,sm43rw,sm43u,sm43v,sm43w

 real sm22rut,sm22rvt,       sm22ut,sm22vt
 real sm23rut,sm23rvt,sm23rwt,sm23ut,sm23vt,sm23wt

 real sm42rut,sm42rvt,       sm42ut,sm42vt
 real sm43rut,sm43rvt,sm43rwt,sm43ut,sm43vt,sm43wt


 real c1,c2,c1dtsq, c2dtsq

 real maxwell2dr,maxwell3dr,maxwellr44,maxwellr66,maxwellr88
 real maxwellc22,maxwellc44,maxwellc66,maxwellc88
 real maxwell2dr44me,maxwell2dr66me,maxwell2dr88me
 real maxwell3dr44me,maxwell3dr66me,maxwell3dr88me
 real maxwellc44me,maxwellc66me,maxwellc88me
 real max2dc44me,max2dc44me2,max3dc44me

c real vr2,vs2,vrr2,vss2,vrs2,vLaplacian22

 real cdt4by360,cdt6by20160

 real lap2d2,lap3d2,lap2d4,lap3d4,lap2d6,lap3d6,lap2d8,lap3d8,lap2d2Pow2,lap3d2Pow2,lap2d2Pow3,lap3d2Pow3,\
      lap2d2Pow4,lap3d2Pow4,lap2d4Pow2,lap3d4Pow2,lap2d4Pow3,lap3d4Pow3,lap2d6Pow2,lap3d6Pow2
 real du,fd22d,fd23d,fd42d,fd43d,fd62d,fd63d,fd82d,fd83d

c real unxx22r,unyy22r,unxy22r,unx22r

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

c     The next macro call will define the difference approximation statement functions
 defineDifferenceOrder2Components1(u,RX)
 defineDifferenceOrder4Components1(u,RX)

c** defineDifferenceOrder2Components1(un,none)
c** defineDifferenceOrder4Components1(un,none)

c** defineDifferenceOrder2Components1(v,none)
c** defineDifferenceOrder4Components1(v,none)

 ! *************************************************
 ! *********2nd-order in space and time*************
 ! *************************************************
 
 ! --- 2D ---

 sm22ru(i1,i2,i3)=cu*u(i1,i2,i3,uc)+cum*um(i1,i2,i3,uc)+\
          c2dtsq*( ulaplacian22r(i1,i2,i3,uc) )+\
          c1dtsq*( uxx22r(i1,i2,i3,uc) + uxy22r(i1,i2,i3,vc) )

 sm22rv(i1,i2,i3)=cu*u(i1,i2,i3,vc)+cum*um(i1,i2,i3,vc)+\
          c2dtsq*( ulaplacian22r(i1,i2,i3,vc) )+ \
          c1dtsq*( uxy22r(i1,i2,i3,uc) + uyy22r(i1,i2,i3,vc) )

 sm22u(i1,i2,i3)=cu*u(i1,i2,i3,uc)+cum*um(i1,i2,i3,uc)+\
          c2dtsq*( ulaplacian22(i1,i2,i3,uc) )+\
          c1dtsq*( uxx22(i1,i2,i3,uc) + uxy22(i1,i2,i3,vc) )

 sm22v(i1,i2,i3)=cu*u(i1,i2,i3,vc)+cum*um(i1,i2,i3,vc)+\
          c2dtsq*( ulaplacian22(i1,i2,i3,vc) ) + \
          c1dtsq*( uxy22(i1,i2,i3,uc) + uyy22(i1,i2,i3,vc) )

   ! time derivatives only for MOL
 sm22rut(i1,i2,i3)=\
          c2    *( ulaplacian22r(i1,i2,i3,uc) )+\
          c1    *( uxx22r(i1,i2,i3,uc) + uxy22r(i1,i2,i3,vc) )

 sm22rvt(i1,i2,i3)=\
          c2    *( ulaplacian22r(i1,i2,i3,vc) )+ \
          c1    *( uxy22r(i1,i2,i3,uc) + uyy22r(i1,i2,i3,vc) )

 sm22ut(i1,i2,i3)=\
          c2    *( ulaplacian22(i1,i2,i3,uc) )+\
          c1    *( uxx22(i1,i2,i3,uc) + uxy22(i1,i2,i3,vc) )

 sm22vt(i1,i2,i3)=\
          c2    *( ulaplacian22(i1,i2,i3,vc) ) + \
          c1    *( uxy22(i1,i2,i3,uc) + uyy22(i1,i2,i3,vc) )

 ! --- 3D ---
 sm23ru(i1,i2,i3)=cu*u(i1,i2,i3,uc)+cum*um(i1,i2,i3,uc)+\
          c2dtsq*( ulaplacian23r(i1,i2,i3,uc) )+\
          c1dtsq*( uxx23r(i1,i2,i3,uc) + uxy23r(i1,i2,i3,vc)+ uxz23r(i1,i2,i3,wc) )

 sm23rv(i1,i2,i3)=cu*u(i1,i2,i3,vc)+cum*um(i1,i2,i3,vc)+\
          c2dtsq*( ulaplacian23r(i1,i2,i3,vc) )+\
          c1dtsq*( uxy23r(i1,i2,i3,uc) + uyy23r(i1,i2,i3,vc)+ uyz23r(i1,i2,i3,wc) )

 sm23rw(i1,i2,i3)=cu*u(i1,i2,i3,wc)+cum*um(i1,i2,i3,wc)+\
          c2dtsq*( ulaplacian23r(i1,i2,i3,wc) )+\
          c1dtsq*( uxz23r(i1,i2,i3,uc) + uyz23r(i1,i2,i3,vc)+ uzz23r(i1,i2,i3,wc) )

 sm23u(i1,i2,i3)=cu*u(i1,i2,i3,uc)+cum*um(i1,i2,i3,uc)+\
          c2dtsq*( ulaplacian23(i1,i2,i3,uc) )+\
          c1dtsq*( uxx23(i1,i2,i3,uc) + uxy23(i1,i2,i3,vc)+ uxz23(i1,i2,i3,wc) )

 sm23v(i1,i2,i3)=cu*u(i1,i2,i3,vc)+cum*um(i1,i2,i3,vc)+\
          c2dtsq*( ulaplacian23(i1,i2,i3,vc) )+\
          c1dtsq*( uxy23(i1,i2,i3,uc) + uyy23(i1,i2,i3,vc)+ uyz23(i1,i2,i3,wc) )

 sm23w(i1,i2,i3)=cu*u(i1,i2,i3,wc)+cum*um(i1,i2,i3,wc)+\
          c2dtsq*( ulaplacian23(i1,i2,i3,wc) )+\
          c1dtsq*( uxz23(i1,i2,i3,uc) + uyz23(i1,i2,i3,vc)+ uzz23(i1,i2,i3,wc) )

  ! -- time derivatives

 sm23rut(i1,i2,i3)=\
          c2    *( ulaplacian23r(i1,i2,i3,uc) )+\
          c1    *( uxx23r(i1,i2,i3,uc) + uxy23r(i1,i2,i3,vc)+ uxz23r(i1,i2,i3,wc) )

 sm23rvt(i1,i2,i3)=\
          c2    *( ulaplacian23r(i1,i2,i3,vc) )+\
          c1    *( uxy23r(i1,i2,i3,uc) + uyy23r(i1,i2,i3,vc)+ uyz23r(i1,i2,i3,wc) )

 sm23rwt(i1,i2,i3)=\
          c2    *( ulaplacian23r(i1,i2,i3,wc) )+\
          c1    *( uxz23r(i1,i2,i3,uc) + uyz23r(i1,i2,i3,vc)+ uzz23r(i1,i2,i3,wc) )

 sm23ut(i1,i2,i3)=\
          c2    *( ulaplacian23(i1,i2,i3,uc) )+\
          c1    *( uxx23(i1,i2,i3,uc) + uxy23(i1,i2,i3,vc)+ uxz23(i1,i2,i3,wc) )

 sm23vt(i1,i2,i3)=\
          c2    *( ulaplacian23(i1,i2,i3,vc) )+\
          c1    *( uxy23(i1,i2,i3,uc) + uyy23(i1,i2,i3,vc)+ uyz23(i1,i2,i3,wc) )

 sm23wt(i1,i2,i3)=\
          c2    *( ulaplacian23(i1,i2,i3,wc) )+\
          c1    *( uxz23(i1,i2,i3,uc) + uyz23(i1,i2,i3,vc)+ uzz23(i1,i2,i3,wc) )


 ! *************************************************
 ! *********4th-order in space and time*************
 ! *************************************************
 
 ! --- 2D ---

 sm42ru(i1,i2,i3)=2.*u(i1,i2,i3,uc)-um(i1,i2,i3,uc)+\
          c2dtsq*( ulaplacian42r(i1,i2,i3,uc) )+\
          c1dtsq*( uxx42r(i1,i2,i3,uc) + uxy42r(i1,i2,i3,vc) )

 sm42rv(i1,i2,i3)=2.*u(i1,i2,i3,vc)-um(i1,i2,i3,vc)+\
          c2dtsq*( ulaplacian42r(i1,i2,i3,vc) )+ \
          c1dtsq*( uxy42r(i1,i2,i3,uc) + uyy42r(i1,i2,i3,vc) )

 sm42u(i1,i2,i3)=2.*u(i1,i2,i3,uc)-um(i1,i2,i3,uc)+\
          c2dtsq*( ulaplacian42(i1,i2,i3,uc) )+\
          c1dtsq*( uxx42(i1,i2,i3,uc) + uxy42(i1,i2,i3,vc) )

 sm42v(i1,i2,i3)=2.*u(i1,i2,i3,vc)-um(i1,i2,i3,vc)+\
          c2dtsq*( ulaplacian42(i1,i2,i3,vc) ) + \
          c1dtsq*( uxy42(i1,i2,i3,uc) + uyy42(i1,i2,i3,vc) )

 ! --- 3D ---
 sm43ru(i1,i2,i3)=2.*u(i1,i2,i3,uc)-um(i1,i2,i3,uc)+\
          c2dtsq*( ulaplacian43r(i1,i2,i3,uc) )+\
          c1dtsq*( uxx43r(i1,i2,i3,uc) + uxy43r(i1,i2,i3,vc)+ uxz43r(i1,i2,i3,wc) )

 sm43rv(i1,i2,i3)=2.*u(i1,i2,i3,vc)-um(i1,i2,i3,vc)+\
          c2dtsq*( ulaplacian43r(i1,i2,i3,vc) )+\
          c1dtsq*( uxy43r(i1,i2,i3,uc) + uyy43r(i1,i2,i3,vc)+ uyz43r(i1,i2,i3,wc) )

 sm43rw(i1,i2,i3)=2.*u(i1,i2,i3,wc)-um(i1,i2,i3,wc)+\
          c2dtsq*( ulaplacian43r(i1,i2,i3,wc) )+\
          c1dtsq*( uxz43r(i1,i2,i3,uc) + uyz43r(i1,i2,i3,vc)+ uzz43r(i1,i2,i3,wc) )

 sm43u(i1,i2,i3)=2.*u(i1,i2,i3,uc)-um(i1,i2,i3,uc)+\
          c2dtsq*( ulaplacian43(i1,i2,i3,uc) )+\
          c1dtsq*( uxx43(i1,i2,i3,uc) + uxy43(i1,i2,i3,vc)+ uxz43(i1,i2,i3,wc) )

 sm43v(i1,i2,i3)=2.*u(i1,i2,i3,vc)-um(i1,i2,i3,vc)+\
          c2dtsq*( ulaplacian43(i1,i2,i3,vc) )+\
          c1dtsq*( uxy43(i1,i2,i3,uc) + uyy43(i1,i2,i3,vc)+ uyz43(i1,i2,i3,wc) )

 sm43w(i1,i2,i3)=2.*u(i1,i2,i3,wc)-um(i1,i2,i3,wc)+\
          c2dtsq*( ulaplacian43(i1,i2,i3,wc) )+\
          c1dtsq*( uxz43(i1,i2,i3,uc) + uyz43(i1,i2,i3,vc)+ uzz43(i1,i2,i3,wc) )



c    *** 2nd order ***
 lap2d2(i1,i2,i3,c)=(u(i1+1,i2,i3,c)-2.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c))*dxsqi\
                   +(u(i1,i2+1,i3,c)-2.*u(i1,i2,i3,c)+u(i1,i2-1,i3,c))*dysqi

 lap3d2(i1,i2,i3,c)=(u(i1+1,i2,i3,c)-2.*u(i1,i2,i3,c)+u(i1-1,i2,i3,c))*dxsqi\
                   +(u(i1,i2+1,i3,c)-2.*u(i1,i2,i3,c)+u(i1,i2-1,i3,c))*dysqi\
                   +(u(i1,i2,i3+1,c)-2.*u(i1,i2,i3,c)+u(i1,i2,i3-1,c))*dzsqi

 ! 2D laplacian squared = u.xxxx + 2 u.xxyy + u.yyyy
 lap2d2Pow2(i1,i2,i3,c)= ( 6.*u(i1,i2,i3,c)   \
   - 4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))    \
       +(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) )*dxi4 \
   +( 6.*u(i1,i2,i3,c)    \
    -4.*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))    \
       +(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) )*dyi4  \
   +( 8.*u(i1,i2,i3,c)     \
    -4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   \
    +2.*(u(i1+1,i2+1,i3,c)+u(i1-1,i2+1,i3,c)+u(i1+1,i2-1,i3,c)+u(i1-1,i2-1,i3,c)) )*dxdyi2

 ! 3D laplacian squared = u.xxxx + u.yyyy + u.zzzz + 2 (u.xxyy + u.xxzz + u.yyzz )
 lap3d2Pow2(i1,i2,i3,c)= ( 6.*u(i1,i2,i3,c)   \
   - 4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))    \
       +(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) )*dxi4 \
  +(  +6.*u(i1,i2,i3,c)    \
    -4.*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))    \
       +(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) )*dyi4\
  +(  +6.*u(i1,i2,i3,c)    \
    -4.*(u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))    \
       +(u(i1,i2,i3+2,c)+u(i1,i2,i3-2,c)) )*dzi4\
   +(8.*u(i1,i2,i3,c)     \
    -4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))   \
    +2.*(u(i1+1,i2+1,i3,c)+u(i1-1,i2+1,i3,c)+u(i1+1,i2-1,i3,c)+u(i1-1,i2-1,i3,c)) )*dxdyi2 \
   +(8.*u(i1,i2,i3,c)     \
    -4.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))   \
    +2.*(u(i1+1,i2,i3+1,c)+u(i1-1,i2,i3+1,c)+u(i1+1,i2,i3-1,c)+u(i1-1,i2,i3-1,c)) )*dxdzi2 \
   +(8.*u(i1,i2,i3,c)     \
    -4.*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))   \
    +2.*(u(i1,i2+1,i3+1,c)+u(i1,i2-1,i3+1,c)+u(i1,i2+1,i3-1,c)+u(i1,i2-1,i3-1,c)) )*dydzi2 

 lap2d2Pow3(i1,i2,i3,c)=LAP2D2(lap2d2Pow2,i1,i2,i3,c)

 lap3d2Pow3(i1,i2,i3,c)=LAP3D2(lap3d2Pow2,i1,i2,i3,c)

 lap2d2Pow4(i1,i2,i3,c)=LAP2D2POW2(lap2d2Pow2,i1,i2,i3,c)
 lap3d2Pow4(i1,i2,i3,c)=LAP3D2POW2(lap3d2Pow2,i1,i2,i3,c)
 
c    ** 4th order ****

 lap2d4(i1,i2,i3,c)=( -30.*u(i1,i2,i3,c)     \
   +16.*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))     \
       -(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) )*dxsq12i + \
  ( -30.*u(i1,i2,i3,c)     \
   +16.*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))     \
       -(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) )*dysq12i 

 lap3d4(i1,i2,i3,c)=lap2d4(i1,i2,i3,c)+ \
  ( -30.*u(i1,i2,i3,c)      \
   +16.*(u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))      \
       -(u(i1,i2,i3+2,c)+u(i1,i2,i3-2,c)) )*dzsq12i 

 lap2d4Pow2(i1,i2,i3,c)=LAP2D4(lap2d4,i1,i2,i3,c)
 lap3d4Pow2(i1,i2,i3,c)=LAP3D4(lap3d4,i1,i2,i3,c)

 lap2d4Pow3(i1,i2,i3,c)=LAP2D4(lap2d4Pow2,i1,i2,i3,c)
 lap3d4Pow3(i1,i2,i3,c)=LAP3D4(lap3d4Pow2,i1,i2,i3,c)

c     *** 6th order ***

 lap2d6(i1,i2,i3,c)= \
          c00lap2d6*u(i1,i2,i3,c)     \
         +c10lap2d6*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)) \
         +c01lap2d6*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)) \
         +c20lap2d6*(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) \
         +c02lap2d6*(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) \
         +c30lap2d6*(u(i1+3,i2,i3,c)+u(i1-3,i2,i3,c)) \
         +c03lap2d6*(u(i1,i2+3,i3,c)+u(i1,i2-3,i3,c)) 

 lap3d6(i1,i2,i3,c)=\
          c000lap3d6*u(i1,i2,i3,c) \
         +c100lap3d6*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)) \
         +c010lap3d6*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)) \
         +c001lap3d6*(u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) \
         +c200lap3d6*(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c)) \
         +c020lap3d6*(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) \
         +c002lap3d6*(u(i1,i2,i3+2,c)+u(i1,i2,i3-2,c)) \
         +c300lap3d6*(u(i1+3,i2,i3,c)+u(i1-3,i2,i3,c)) \
         +c030lap3d6*(u(i1,i2+3,i3,c)+u(i1,i2-3,i3,c)) \
         +c003lap3d6*(u(i1,i2,i3+3,c)+u(i1,i2,i3-3,c))

 lap2d6Pow2(i1,i2,i3,c)=LAP2D6(lap2d6,i1,i2,i3,c)
 lap3d6Pow2(i1,i2,i3,c)=LAP3D6(lap3d6,i1,i2,i3,c)


c     *** 8th order ***

 lap2d8(i1,i2,i3,c)=c00lap2d8*u(i1,i2,i3,c)      \
          +c10lap2d8*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))     \
          +c01lap2d8*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)) \
          +c20lap2d8*(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c))  \
          +c02lap2d8*(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) \
          +c30lap2d8*(u(i1+3,i2,i3,c)+u(i1-3,i2,i3,c))  \
          +c03lap2d8*(u(i1,i2+3,i3,c)+u(i1,i2-3,i3,c)) \
          +c40lap2d8*(u(i1+4,i2,i3,c)+u(i1-4,i2,i3,c))  \
          +c04lap2d8*(u(i1,i2+4,i3,c)+u(i1,i2-4,i3,c))

 lap3d8(i1,i2,i3,c)=c000lap3d8*u(i1,i2,i3,c)      \
          +c100lap3d8*(u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))     \
          +c010lap3d8*(u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)) \
          +c001lap3d8*(u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c)) \
          +c200lap3d8*(u(i1+2,i2,i3,c)+u(i1-2,i2,i3,c))  \
          +c020lap3d8*(u(i1,i2+2,i3,c)+u(i1,i2-2,i3,c)) \
          +c002lap3d8*(u(i1,i2,i3+2,c)+u(i1,i2,i3-2,c)) \
          +c300lap3d8*(u(i1+3,i2,i3,c)+u(i1-3,i2,i3,c))  \
          +c030lap3d8*(u(i1,i2+3,i3,c)+u(i1,i2-3,i3,c)) \
          +c003lap3d8*(u(i1,i2,i3+3,c)+u(i1,i2,i3-3,c)) \
          +c400lap3d8*(u(i1+4,i2,i3,c)+u(i1-4,i2,i3,c))  \
          +c040lap3d8*(u(i1,i2+4,i3,c)+u(i1,i2-4,i3,c)) \
          +c004lap3d8*(u(i1,i2,i3+4,c)+u(i1,i2,i3-4,c))

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


 ! write(*,*) 'Inside advSM...'

 dt    =rpar(0)
 dx(0) =rpar(1)
 dx(1) =rpar(2)
 dx(2) =rpar(3)
 adc   =rpar(4)  ! coefficient of artificial dissipation
 dr(0) =rpar(5)
 dr(1) =rpar(6)
 dr(2) =rpar(7)
 c1    =rpar(8)
 c2    =rpar(9) 
 kx    =rpar(10) 
 ky    =rpar(11) 
 kz    =rpar(12) 
 ep    =rpar(13)
 t     =rpar(14)
 dtOld =rpar(15) ! dt used on the previous time step 

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
 uc                 =ipar(6)
 vc                 =ipar(7)
 wc                 =ipar(8)
 useWhereMask       =ipar(9)
 timeSteppingMethod =ipar(10)
 useVariableDissipation=ipar(11)
 useConservative    =ipar(12)   
 combineDissipationWithAdvance = ipar(13)
 debug              =ipar(14)
 computeUt          =ipar(15)  

 materialFormat     =ipar(16)   ! 0=const, 1=piece-wise const, 2=varaiable
 myid               =ipar(17)

 cu=  2.     ! coeff. of u(t) in the time-step formula
 cum=-1.     ! coeff. of u(t-dtOld)
 csq=cc**2
 dtsq=dt**2

 cdt=cc*dt

 c1dtsq=c1*dtsq
 c2dtsq=c2*dtsq

 if( dtOld.le.0 )then
   write(*,'(" advSM:ERROR : dtOld<=0 ")')
   stop 8167
 end if
 if( dt.ne.dtOld )then
   write(*,'(" advSM:INFO: dt=",e12.4," <> dtOld=",e12.4," diff=",e9.2)') dt,dtOld,dt-dtOld
   if( orderOfAccuracy.ne.2 )then
     write(*,'(" advSM:ERROR: variable dt not implemented for orderOfAccuracy=",i4)') orderOfAccuracy
     ! '
     stop 8168
   end if

   ! adjust the coefficients for a variable time step : this is locally second order accurate
   cu= 1.+dt/dtOld     ! coeff. of u(t) in the time-step formula
   cum=-dt/dtOld       ! coeff. of u(t-dtOld)
   
   c1dtsq=c1*dt*(dt+dtOld)*.5
   c2dtsq=c2*dt*(dt+dtOld)*.5
 end if


c cdtsq=(cc**2)*(dt**2)
c cdtsq12=cdtsq*cdtsq/12.
c cdt4by360=(cdt)**4/360.
c cdt6by20160=cdt**6/(8.*7.*6.*5.*4.*3.)

 dt4by12=dtsq*dtsq/12.

c cdtdx = (cc*dt/dx(0))**2
c cdtdy = (cc*dt/dy)**2
c cdtdz = (cc*dt/dz)**2

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



 if( .false. .and. debug.gt.0 )then
  ! evaluate derivatives of the exact solution

  i1=5
  i2=5
  i3=0
  ! 
  OGDERIV2D(0,1,0,0,i1,i2,i3,t,ux0,vx0)
  OGDERIV2D(0,0,1,0,i1,i2,i3,t,uy0,vy0)
  write(*,'(" advOpt: i=",3i3," t,x,y=",3f6.2," ux,vx,uy,vy=",4f6.2)') i1,i2,i3,t,xy(i1,i2,i3,0),xy(i1,i2,i3,1),ux0,vx0,uy0,vy0
 end if

 if( adc.gt.0. .and. combineDissipationWithAdvance.eq.0 )then
   ! ********************************************************************************************************
   ! ********************* Compute the dissipation and fill in the dis(i1,i2,i3,c) array ********************
   ! ********************************************************************************************************

   call ovtime( time0 )


  ! Here we assume that a (2m)th order method will only use dissipation of (2m) or (2m+2)

  if( computeUt.eq.0 )then
    !   adcdt=adc*dt
    adcdt = adc*(dt*(dt+dtOld)/2.)/dtOld  ! for variable time step *wdh* 100203 
  else
   ! adcdt=adc/dt
   adcdt= adc/dtOld                    ! for variable time step *wdh* 100203
  end if


  if( orderOfDissipation.eq.4 )then
  #If #ORDER eq "2" || #ORDER eq "4"
     ! write(*,*) 'Inside advSM: add dissipation order=4... option=',option
     #If #DIM eq "2"
      if( useVariableDissipation.eq.0 )then
       loopse9(dis(i1,i2,i3,uc)=adcdt*fd42d(i1,i2,i3,uc),\
               dis(i1,i2,i3,vc)=adcdt*fd42d(i1,i2,i3,vc),\
               ,,,,,,)
      else
       ! write(*,'(" advOpt: apply 4th-order variable dissipation...")') 
       loopse6VarDis(dis(i1,i2,i3,uc)=adcdt*varDis(i1,i2,i3)*fd42d(i1,i2,i3,uc),\
                     dis(i1,i2,i3,vc)=adcdt*varDis(i1,i2,i3)*fd42d(i1,i2,i3,vc),\
                     ,\
                     dis(i1,i2,i3,uc)=0.,dis(i1,i2,i3,vc)=0.,)
      end if
     #Else
      if( useVariableDissipation.eq.0 )then
       loopsF3D(0,0,0,\
                dis(i1,i2,i3,uc)=adcdt*fd43d(i1,i2,i3,uc),\
                dis(i1,i2,i3,vc)=adcdt*fd43d(i1,i2,i3,vc),\
                dis(i1,i2,i3,wc)=adcdt*fd43d(i1,i2,i3,wc),,,,,,,\
                0,0,0,\
                ,\
                ,\
                ,,,,,,)
      else
       loopsVarDis3D(\
                dis(i1,i2,i3,uc)=adcdt*varDis(i1,i2,i3)*fd43d(i1,i2,i3,uc),\
                dis(i1,i2,i3,vc)=adcdt*varDis(i1,i2,i3)*fd43d(i1,i2,i3,vc),\
                dis(i1,i2,i3,wc)=adcdt*varDis(i1,i2,i3)*fd43d(i1,i2,i3,wc),\
                dis(i1,i2,i3,uc)=0.,dis(i1,i2,i3,vc)=0.,dis(i1,i2,i3,wc)=0.)
      end if
     #End  
  #End
  #If #ORDER eq "4" || #ORDER eq "6"
   else if( orderOfDissipation.eq.6 )then
     #If #DIM eq "2"
       loopse9(dis(i1,i2,i3,uc)=adcdt*fd62d(i1,i2,i3,uc),\
               dis(i1,i2,i3,vc)=adcdt*fd62d(i1,i2,i3,vc),\
               ,,,,,,)
     #Else
       loopsF3D(0,0,0,\
                dis(i1,i2,i3,uc)=adcdt*fd63d(i1,i2,i3,uc),\
                dis(i1,i2,i3,vc)=adcdt*fd63d(i1,i2,i3,vc),\
                dis(i1,i2,i3,wc)=adcdt*fd63d(i1,i2,i3,wc),,,,,,,\
                0,0,0,\
                ,\
                ,\
                ,,,,,,)
     #End
  #End
  #If #ORDER eq "6" || #ORDER eq "8"
   else if( orderOfDissipation.eq.8 )then
     #If #DIM eq "2"
       loopse9(dis(i1,i2,i3,uc)=adcdt*fd82d(i1,i2,i3,uc),\
               dis(i1,i2,i3,vc)=adcdt*fd82d(i1,i2,i3,vc),\
               ,,,,,,)
     #Else
       loopsF3D(0,0,0,\
                dis(i1,i2,i3,uc)=adcdt*fd83d(i1,i2,i3,uc),\
                dis(i1,i2,i3,vc)=adcdt*fd83d(i1,i2,i3,vc),\
                dis(i1,i2,i3,wc)=adcdt*fd83d(i1,i2,i3,wc),,,,,,,\
                0,0,0,\
                ,\
                ,\
                ,,,,,,)
     #End
  #End
  #If #ORDER eq "2" 
   else if( orderOfDissipation.eq.2 )then
     #If #DIM eq "2"
      if( useVariableDissipation.eq.0 )then
       loopse9(dis(i1,i2,i3,uc)=adcdt*fd22d(i1,i2,i3,uc),\
               dis(i1,i2,i3,vc)=adcdt*fd22d(i1,i2,i3,vc),\
               ,,,,,,)
      else
        stop 33333
      end if
    #Else
      if( useVariableDissipation.eq.0 )then
       loopsF3D(0,0,0,\
                dis(i1,i2,i3,uc)=adcdt*fd23d(i1,i2,i3,uc),\
                dis(i1,i2,i3,vc)=adcdt*fd23d(i1,i2,i3,vc),\
                dis(i1,i2,i3,wc)=adcdt*fd23d(i1,i2,i3,wc),,,,,,,\
                0,0,0,\
                ,\
                ,\
                ,,,,,,)
      else
        stop 28855
      end if
     #End
  #End
   else
     write(*,*) 'advSM:ERROR orderOfDissipation=',orderOfDissipation
     stop 5
   end if
   call ovtime( time1 )
   rpar(20)=time1-time0
 end if


 if( option.eq.1 ) then
   return
 end if


c write(*,'(" advSM: timeSteppingMethod=",i2)') timeSteppingMethod
 if( timeSteppingMethod.eq.defaultTimeStepping )then
  write(*,'(" advSM:ERROR: timeSteppingMethod=defaultTimeStepping -- this should be set")')
    ! '
  stop 83322
 end if

 if( gridType.eq.rectangular )then

 #If #GRIDTYPE eq "rectangular"

c       **********************************************
c       *************** rectangular ******************
c       **********************************************

 #If #ORDER eq "2" 

   #If #DIM eq "2"
    if( computeUt.eq.0 )then
     loopsF2DD(dtsq*f(i1,i2,i3,uc),dtsq*f(i1,i2,i3,vc),,\
              un(i1,i2,i3,uc)=sm22ru(i1,i2,i3),\
              un(i1,i2,i3,vc)=sm22rv(i1,i2,i3),,,,,,,)
    else
     ! return du/dt in un : 
     loopsF2DD(f(i1,i2,i3,uc),f(i1,i2,i3,vc),,\
              un(i1,i2,i3,uc)=sm22rut(i1,i2,i3),\
              un(i1,i2,i3,vc)=sm22rvt(i1,i2,i3),,,,,,,)
    end if
   #Else
    if( computeUt.eq.0 )then
     loopsF3DD(dtsq*f(i1,i2,i3,uc),dtsq*f(i1,i2,i3,vc),dtsq*f(i1,i2,i3,wc),\
              un(i1,i2,i3,uc)=sm23ru(i1,i2,i3),\
              un(i1,i2,i3,vc)=sm23rv(i1,i2,i3),\
              un(i1,i2,i3,wc)=sm23rw(i1,i2,i3),,,,,,)
    else
     loopsF3DD(f(i1,i2,i3,uc),f(i1,i2,i3,vc),f(i1,i2,i3,wc),\
              un(i1,i2,i3,uc)=sm23rut(i1,i2,i3),\
              un(i1,i2,i3,vc)=sm23rvt(i1,i2,i3),\
              un(i1,i2,i3,wc)=sm23rwt(i1,i2,i3),,,,,,)
    end if
   #End

 #Elif #ORDER eq "4" 

   ! 4th order in space and 4th order in time:

   ! write(*,*) 'Inside advSM order=4...'


   if( timeSteppingMethod.eq.modifiedEquationTimeStepping )then

   #If #DIM eq "2"
    if( computeUt.eq.0 )then
     loopsF2DD(dtsq*f(i1,i2,i3,uc),dtsq*f(i1,i2,i3,vc),,\
              un(i1,i2,i3,uc)=sm42ru(i1,i2,i3),\
              un(i1,i2,i3,vc)=sm42rv(i1,i2,i3),,,,,,,)
    else
     stop 101
    end if
   #Else
    if( computeUt.eq.0 )then
     loopsF3DD(dtsq*f(i1,i2,i3,uc),dtsq*f(i1,i2,i3,vc),dtsq*f(i1,i2,i3,wc),\
              un(i1,i2,i3,uc)=sm43ru(i1,i2,i3),\
              un(i1,i2,i3,vc)=sm43rv(i1,i2,i3),\
              un(i1,i2,i3,wc)=sm43rw(i1,i2,i3),,,,,,)
    else
     stop 102
    end if
   #End

     #If #DIM eq "2"
       ! 4th order modified equation 

       if( combineDissipationWithAdvance.eq.0 )then
        ! write(*,*) 'advOpt: 2d, rect, modified equation'

       else
        ! modified equation and dissipation in one loop
       end if

     #Else
       if( combineDissipationWithAdvance.eq.0 )then
         ! 4th order modified equation 
        else
c         ! 4th order modified equation and dissipation in one loop
        end if

     #End

   else  ! not modified equation

     #If #DIM eq "2"
       ! 4th order in space and 4th order Stoermer
     #Else
       ! 4th order in space and 4th order Stoermer
       stop 5555
       ! comment this ou to shorten the code

     #End
   end if

 #Else
   write(*,*) 'advSM:ERROR orderOfAccuracy,orderInTime=',orderOfAccuracy,orderInTime
   stop 1

 #End

 #End

 else               

 #If #GRIDTYPE eq "curvilinear"

c       **********************************************
c       *************** curvilinear ******************
c       **********************************************

   if( useConservative.eq.0 )then

    ! *************** non-conservative *****************    

    #If #ORDER eq "2" 

     #If #DIM eq "2"
      if( computeUt.eq.0 )then
       loopsF2DD(dtsq*f(i1,i2,i3,uc),dtsq*f(i1,i2,i3,vc),,\
                un(i1,i2,i3,uc)=sm22u(i1,i2,i3),\
                un(i1,i2,i3,vc)=sm22v(i1,i2,i3),,,,,,,)
      else
       loopsF2DD(f(i1,i2,i3,uc),f(i1,i2,i3,vc),,\
                un(i1,i2,i3,uc)=sm22ut(i1,i2,i3),\
                un(i1,i2,i3,vc)=sm22vt(i1,i2,i3),,,,,,,)
      end if
   #Else
      if( computeUt.eq.0 )then
       loopsF3DD(dtsq*f(i1,i2,i3,uc),dtsq*f(i1,i2,i3,vc),dtsq*f(i1,i2,i3,wc),\
                un(i1,i2,i3,uc)=sm23u(i1,i2,i3),\
                un(i1,i2,i3,vc)=sm23v(i1,i2,i3),\
                un(i1,i2,i3,wc)=sm23w(i1,i2,i3),,,,,,)
      else
       loopsF3DD(f(i1,i2,i3,uc),f(i1,i2,i3,vc),f(i1,i2,i3,wc),\
                un(i1,i2,i3,uc)=sm23ut(i1,i2,i3),\
                un(i1,i2,i3,vc)=sm23vt(i1,i2,i3),\
                un(i1,i2,i3,wc)=sm23wt(i1,i2,i3),,,,,,)
      end if
     #End

   #Elif #ORDER eq "4"
     
     if( timeSteppingMethod.eq.modifiedEquationTimeStepping )then
       ! 4th order in space and 4th order in time:

       !   cdtsq*uLaplacian42(i1,i2,i3,n)+cdtsq12*uLapSq(n)
       ! write(*,*) 'advOpt: 2d, curv, FULL modified equation'

      #If #DIM eq "2"


      #Elif #DIM == "3"


      #End

     else
 
      stop 27430

     end if

   #Else
     stop 11155
   #End


   else if( useConservative.eq.1 )then

    ! *************** conservative *****************    

    stop 99422



   else
     ! *****************************************************
     ! ****************Old way******************************
     ! *****************************************************

  end if

 #End
 end if

 return
 end

#endMacro


#beginMacro buildFile(NAME,DIM,ORDER,GRIDTYPE)
#beginFile src/NAME.f
 ADV_SM(NAME,DIM,ORDER,GRIDTYPE)
#endFile
#endMacro

      buildFile(advSm2dOrder2r,2,2,rectangular)
      buildFile(advSm3dOrder2r,3,2,rectangular)
      buildFile(advSm2dOrder2c,2,2,curvilinear)
      buildFile(advSm3dOrder2c,3,2,curvilinear)

      buildFile(advSm2dOrder4r,2,4,rectangular)
      buildFile(advSm3dOrder4r,3,4,rectangular)
      buildFile(advSm2dOrder4c,2,4,curvilinear)
      buildFile(advSm3dOrder4c,3,4,curvilinear)
c**
c**      buildFile(advSm22Order6r,2,6,rectangular)
c**      buildFile(advSm23Order6r,3,6,rectangular)
c**
c**       ! build these for testing symmetric operators -- BC's not implemented yet
c**      buildFile(advSm22Order6c,2,6,curvilinear)
c**      buildFile(advSm23Order6c,3,6,curvilinear)
c**
c**      buildFile(advSm22Order8r,2,8,rectangular)
c**      buildFile(advSm23Order8r,3,8,rectangular)
c**
c**       ! build these for testing symmetric operators -- BC's not implemented yet
c**      buildFile(advSm22Order8c,2,8,curvilinear)
c**      buildFile(advSm23Order8c,3,8,curvilinear)






      subroutine advSM(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                       mask,rx,xy,  um,u,un,f, ndMatProp,matIndex,matValpc,matVal,\
                       bc, dis, varDis, ipar, rpar, ierr )
c======================================================================
c   Advance a time step for the equations of Solid Mechanics (linear elasticity for now)
c
c nd : number of space dimensions
c
c ipar(0)  = option : option=0 - SM+Artificial diffusion
c                           =1 - AD only
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real um(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real varDis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)
      
      ! -- Declare arrays for variable material properties --
      include 'declareVarMatProp.h'

c     ---- local variables -----
      real dt,dtOld
      integer c,i1,i2,i3,n,gridType,orderOfAccuracy,orderInTime,useConservative
      integer addForcing,orderOfDissipation,option
      integer useWhereMask,solveForE,solveForH,grid

      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )
c...........end   statement functions


      ! write(*,*) 'Inside advSM...'
      dt    =rpar(0)
      dtOld =rpar(15) ! dt used on the previous time step 

      gridType           =ipar(1)
      orderOfAccuracy    =ipar(2)
      useConservative    =ipar(12)

      ! write(*,'(" advOpt: gridType=",i2," useConservative=",i2)') gridType,useConservative
      if( abs(dt-dtOld).gt.dt*.001 .and. orderOfAccuracy.ne.2 )then
       write(*,'(" advSM:ERROR: variable dt not implemented yet for this case")')
       write(*,'("            : dt,dtOld,diff=",3e9.3)') dt,dtOld,dt-dtOld
       write(*,'("              orderOfAccuracy=",i4," useConservative=",i4)') orderOfAccuracy,useConservative
       ! '
       stop 9027
      end if


      if( orderOfAccuracy.eq.2 )then
       if( useConservative.eq.1 )then
        ! Conservative (self-adjoint) approximations from Daniel
        if( nd.eq.2 .and. gridType.eq.rectangular ) then
          call advSmCons2dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                                  mask,rx,xy, um,u,un,f, ndMatProp,matIndex,matValpc,matVal, \
                                  bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.2 .and. gridType.eq.curvilinear ) then
          call advSmCons2dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                                  mask,rx,xy, um,u,un,f, ndMatProp,matIndex,matValpc,matVal,\
                                  bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.3 .and. gridType.eq.rectangular ) then
          call advSmCons3dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                                  mask,rx,xy, um,u,un,f, ndMatProp,matIndex,matValpc,matVal,\
                                  bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.3 .and. gridType.eq.curvilinear ) then
          call advSmCons3dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                                  mask,rx,xy, um,u,un,f, ndMatProp,matIndex,matValpc,matVal,\
                                  bc, dis,varDis, ipar, rpar, ierr )
        else
          stop 2271
        end if
       else
        ! non-conservative approximations 
        if( nd.eq.2 .and. gridType.eq.rectangular ) then
          call advSm2dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.2 .and. gridType.eq.curvilinear ) then
          call advSm2dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.3 .and. gridType.eq.rectangular ) then
          call advSm3dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                             mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.3 .and. gridType.eq.curvilinear ) then
          call advSm3dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                             mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
        else
          stop 2271
        end if
       end if

      else if( orderOfAccuracy.eq.4 ) then
        if( nd.eq.2 .and. gridType.eq.rectangular )then
          call advSm2dOrder4r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
          call advSm2dOrder4c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
          call advSm3dOrder4r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
          call advSm3dOrder4c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
       else
         stop 8843
       end if

c**c
c**      else if( orderOfAccuracy.eq.6 ) then
c**        if( nd.eq.2 .and. gridType.eq.rectangular )then
c**          call advSm2dOrder6r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
c**          call advSm2dOrder6c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
c**          call advSm3dOrder6r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
c**          call advSm3dOrder6c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**       else
c**         stop 8843
c**       end if
c**
c**      else if( orderOfAccuracy.eq.8 ) then
c**
c**        if( nd.eq.2 .and. gridType.eq.rectangular )then
c**          call advSm2dOrder8r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
c**          call advSm2dOrder8c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
c**          call advSm3dOrder8r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
c**          call advSm3dOrder8c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**       else
c**         stop 8843
c**       end if

      else
        write(*,'(" advSM:ERROR: un-implemented order of accuracy =",i6)') orderOfAccuracy
          ! '
        stop 11222
      end if

      return
      end








