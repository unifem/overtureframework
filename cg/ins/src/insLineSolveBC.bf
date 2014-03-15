c***********************************************************************************************
c 
c   *NEW* Steady-state line-solver BOUNDARY CONDITION routines for the incompressible NS plus some turbulence models
c
c***********************************************************************************************

c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"

#Include "commonMacros.h"

! define the INITIALZE macro: 
#Include "initLineSolveParameters.h"


! --- Macros to define INS and Temperature line-solve functions:---
!          fillEquationsRectangularGridINS
!          fillEquationsCurvilinearGridINS
#Include "lineSolveINS.h"

! --- Macros to define Visco Plastic line-solve functions:---
!     fillEquationsRectangularGridVP, computeResidualVP
#Include "lineSolveVP.h"

! --- Macros for the Spalart-Almaras turbulence model
#Include lineSolveSA.h

! --- Macros for the Baldwin-Lomax approx. 
! define computeBLNuT() :   Macro to compute Baldwin-Lomax Turbulent viscosity
#Include lineSolveBL.h


#beginMacro beginLoops()
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
#endMacro

#beginMacro endLoops()
 end do
 end do
 end do
#endMacro



c ***********************************************************************
c Fill in the matrix and RHS on the boundary
c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c ***********************************************************************
#beginMacro loopsBC(dim,EQN, e1,e2,e3,e4,e5,e6)
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
  else
c for interpolation points or unused:
   am(i1,i2,i3)=0.
   bm(i1,i2,i3)=1.
   cm(i1,i2,i3)=0.
#If #EQN == "TEMPERATURE"
   f(i1,i2,i3,fct)=uu(tc)
#Else
   f(i1,i2,i3,fcu)=uu(uc)
   f(i1,i2,i3,fcv)=uu(vc)
#If #dim == "3"
   f(i1,i2,i3,fcw)=uu(wc)
#End
#End
#If #EQN == "SA"
   f(i1,i2,i3,nc)=uu(nc)
#End      
  end if
 end do
 end do
 end do
#endMacro

c ***********************************************************************
c Fill in the matrix and RHS on the boundary
c  SOLVER: INS, SPAL
c ***********************************************************************
#beginMacro loopsMatrixBC(SOLVER,e1,e2,e3,e4,e5,e6)
#If #SOLVER == "INS" || #SOLVER == "INSVP"
 if( nd.eq.2 )then
  if( option.eq.assignINS )then
   loopsBC(2,INS,e1,e2,e3, e4,e5,e6)
  else if( option.eq.assignTemperature )then
   loopsBC(2,TEMPERATURE,e1,e2,e3, e4,e5,e6)
  end if
 else
  if( option.eq.assignINS )then
   loopsBC(3,INS,e1,e2,e3, e4,e5,e6)
  else if( option.eq.assignTemperature )then
   loopsBC(3,TEMPERATURE,e1,e2,e3, e4,e5,e6)
  end if
 end if
#Elif #SOLVER == "SPAL"
 if( nd.eq.2 )then
  if( option.eq.assignSpalartAllmaras )then
   loopsBC(2,SA,e1,e2,e3, e4,e5,e6)
  end if
 else
  if( option.eq.assignSpalartAllmaras )then
   loopsBC(3,SA,e1,e2,e3, e4,e5,e6)
  end if
 end if
#Else
  stop 8862
#End
#endMacro


c ***********************************************************************
c Fill in the matrix and RHS on the boundary, fourth-order version
c EQN : INS=incompressible N-S, SA=Spalart-Allmaras, TEMPERATURE
c ***********************************************************************
#beginMacro loopsBC4(dim,EQN, e1,e2,e3,e4,e5,e6)
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
    else
c for interpolation points or unused:
      am(i1,i2,i3)=0.
      bm(i1,i2,i3)=0.
      cm(i1,i2,i3)=1.
      dm(i1,i2,i3)=0.
      em(i1,i2,i3)=0.
      am(i1-is1,i2-is2,i3-is3)=0.
      bm(i1-is1,i2-is2,i3-is3)=0.
      cm(i1-is1,i2-is2,i3-is3)=1.
      dm(i1-is1,i2-is2,i3-is3)=0.
      em(i1-is1,i2-is2,i3-is3)=0.
#If #EQN == "TEMPERATURE"
      f(i1,i2,i3,fct)=uu(tc)
      f(i1-is1,i2-is2,i3-is3,fct)=u(i1-is1,i2-is2,i3-is3,tc)
#Else
      f(i1,i2,i3,fcu)=uu(uc)
      f(i1,i2,i3,fcv)=uu(vc)
      f(i1-is1,i2-is2,i3-is3,fcu)=u(i1-is1,i2-is2,i3-is3,uc)
      f(i1-is1,i2-is2,i3-is3,fcv)=u(i1-is1,i2-is2,i3-is3,vc)
#If #dim == "3"
      f(i1,i2,i3,fcw)=uu(wc)
      f(i1-is1,i2-is2,i3-is3,fcw)=u(i1-is1,i2-is2,i3-is3,wc)
#End
#End
#If #EQN == "SA"
      f(i1,i2,i3,nc)=uu(nc)
      f(i1-is1,i2-is2,i3-is3,nc)=u(i1-is1,i2-is2,i3-is3,nc)
#End    
    end if
  end do
  end do
  end do
#endMacro

c ***********************************************************************
c Fill in the matrix and RHS on the boundary -- fourth-order version
c  SOLVER: INS, SPAL
c ***********************************************************************
#beginMacro loopsMatrixBC4(SOLVER,e1,e2,e3,e4,e5,e6)
#If #SOLVER == "INS"
 if( nd.eq.2 )then
  if( option.eq.assignINS )then
   loopsBC4(2,INS,e1,e2,e3, e4,e5,e6)
  else if( option.eq.assignTemperature )then
   loopsBC4(2,TEMPERATURE,e1,e2,e3, e4,e5,e6)
  end if
 else
  if( option.eq.assignINS )then
   loopsBC4(3,INS,e1,e2,e3, e4,e5,e6)
  else if( option.eq.assignTemperature )then
   loopsBC4(3,TEMPERATURE,e1,e2,e3, e4,e5,e6)
  end if
 end if
#Elif #SOLVER == "SPAL"
 if( nd.eq.2 )then
  if( option.eq.assignSpalartAllmaras )then
   loopsBC4(2,SA,e1,e2,e3, e4,e5,e6)
  end if
 else
  if( option.eq.assignSpalartAllmaras )then
   loopsBC4(3,SA,e1,e2,e3, e4,e5,e6)
  end if
 end if
#Else
  stop 7715
#End
!  if( nd.eq.2 )then
!    if( option.eq.assignINS )then
!      loopsBC4(2,INS,e1,e2,e3, e4,e5,e6)
!    else if( option.eq.assignTemperature )then
!      loopsBC4(2,TEMPERATURE,e1,e2,e3, e4,e5,e6)
!    else if( option.eq.assignSpalartAllmaras )then
!      loopsBC4(2,SA,e1,e2,e3, e4,e5,e6)
!    end if
!  else
!    if( option.eq.assignINS )then
!      loopsBC4(3,INS,e1,e2,e3, e4,e5,e6)
!    else if( option.eq.assignTemperature )then
!      loopsBC4(3,TEMPERATURE,e1,e2,e3, e4,e5,e6)
!    else if( option.eq.assignSpalartAllmaras )then
!      loopsBC4(3,SA,e1,e2,e3, e4,e5,e6)
!    end if
!  end if
#endMacro

#beginMacro assignDirichletFourthOrder()
c write(*,'(" fill am,bm,...,em: i1,i2,i3=",3i3)') i1,i2,i3
 am(i1,i2,i3)=0.
 bm(i1,i2,i3)=0.
 cm(i1,i2,i3)=1.
 dm(i1,i2,i3)=0.
 em(i1,i2,i3)=0.
 am(i1-is1,i2-is2,i3-is3)=0.
 bm(i1-is1,i2-is2,i3-is3)=0.
 cm(i1-is1,i2-is2,i3-is3)=1.
 dm(i1-is1,i2-is2,i3-is3)=0.
 em(i1-is1,i2-is2,i3-is3)=0.
#endMacro

#beginMacro assignFourthOrder()
 am(i1,i2,i3)=cexa
 bm(i1,i2,i3)=cexb
 cm(i1,i2,i3)=cexc
 dm(i1,i2,i3)=cexd
 em(i1,i2,i3)=cexe
 am(i1-is1,i2-is2,i3-is3)=c4exa
 bm(i1-is1,i2-is2,i3-is3)=c4exb
 cm(i1-is1,i2-is2,i3-is3)=c4exc
 dm(i1-is1,i2-is2,i3-is3)=c4exd
 em(i1-is1,i2-is2,i3-is3)=c4exe
#endMacro



c ================================================================================
c Define the line solver BOUNDARY CONDITION routine for a given solver.
c
c SOLVER: INS, INSSPAL, INSBL, INSVP
c=================================================================================
#beginMacro INS_LINE_BC(SOLVER,NAME)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
      md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw,\
      dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
      ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c======================================================================
c
c nd : number of space dimensions
c
c n1a,n1b,n2a,n2b,n3a,n3b : INTERIOR points (does not include boundary points along axis=dir)
c
c dir : 0,1,2 - direction of line 
c a,b,c : output: tridiagonal matrix
c a,b,c,d,e  : output: penta-diagonal matrix (for fourth-order)
c
c ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b : dimensions for the bcData array
c bcData : holds coefficients for BC's
c 
c bc(0:1,0:nd-1) : line solver BC's 
c boundaryCondition(0:1,0:nd-1) : MappedGrid boundary conditions
c dw: distance to wall for SA TM
c======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
  md1a,md1b,md2a,md2b,md3a,md3b,dir

 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

 real am(md1a:md1b,md2a:md2b,md3a:md3b)
 real bm(md1a:md1b,md2a:md2b,md3a:md3b)
 real cm(md1a:md1b,md2a:md2b,md3a:md3b)
 real dm(md1a:md1b,md2a:md2b,md3a:md3b)
 real em(md1a:md1b,md2a:md2b,md3a:md3b)

 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
 real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

 real dt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:*),boundaryCondition(0:1,0:*), ierr
 real dtScale,cfl

 ! bcData(component+numberOfComponents*(0),side,axis,grid)
 integer numberOfComponents,systemComponent
 integer ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b
 real bcData(ndbcd1a:ndbcd1b,ndbcd2a:ndbcd2b,ndbcd3a:ndbcd3b,ndbcd4a:ndbcd4b)

 integer ipar(0:*)
 real rpar(0:*)
 
 !     ---- local variables -----
 integer m,n,c,i1,i2,i3,j1,j2,j3,orderOfAccuracy,gridIsMoving,useWhereMask
 integer gridIsImplicit,implicitOption,implicitMethod,ibc,\
 isAxisymmetric,use2ndOrderAD,use4thOrderAD,useSelfAdjointDiffusion,\
 orderOfExtrapolation,fourthOrder,dirp1,dirp2
 integer pc,uc,vc,wc,tc,vsc,fc,fcu,fcv,fcw,fcn,fct,grid,side,gridType
 integer computeMatrix,computeRHS,computeMatrixBC
 integer twilightZoneFlow,computeTemperature
 integer indexRange(0:1,0:2),gid(0:1,0:2),is1,is2,is3
 real nu,kThermal,thermalExpansivity,gravity(0:2)
 real dx(0:2),dx0,dy,dz,dxi,dyi,dzi,dri,dsi,dti
 real dxv2i(0:2),dx2i,dy2i,dz2i
 real dxvsqi(0:2),dxsqi,dysqi,dzsqi
 real drv2i(0:2),dr2i,ds2i,dt2i
 real drvsqi(0:2),drsqi,dssqi,dtsqi
 real dx12i,dy12i,dz12i,dxsq12i,dysq12i,dzsq12i,dxy4i,dxz4i,dyz4i
 real ad21,ad22,ad41,ad42,cd22,cd42,adc,sn
 real ad21n,ad22n,ad41n,ad42n,cd22n,cd42n
 real dr(0:2)

 real adCoeff2,adCoeff4
 real cexa,cexb,cexc,cexd,cexe
 real c4exa,c4exb,c4exc,c4exd,c4exe
 real cna,cnb,cnc

 integer option
 integer assignINS,assignSpalartAllmaras,setupSweep,assignTemperature
 parameter( assignINS=0, assignSpalartAllmaras=1, setupSweep=2, assignTemperature=3 )

 integer turbulenceModel,noTurbulenceModel
 integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
 parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

 real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0
 real dd,dndx(0:2)

 integer axis,kd
 real kbl,alpha,a0p,ccp,ckleb,cwk !baldwin-lomax constants
 real magu,magumax,ymax,ulmax,lmixw,lmixmax,lmix2max,vto,vort,fdotn,tawu ! baldwin-lomax tmp variables
 real yscale,yplus,nmag,ftan(3),norm(3),tauw,maxumag,maxvt,ctrans,ditrip ! more baldwin-lomax tmp variables
 integer iswitch, ibb, ibe, i, ii1,ii2,ii3,io(3) ! baldwin-lomax loop variables
 integer itrip,jtrip,ktrip !baldwin-lomax trip location
 real chi,fnu1,fnu2,s,r,g,fw,dKappaSq,nBydSqLhs,nSqBydSq,nutb
 real nuTilde,nuT,nuTx(0:2),fv1,fv1x,fv1y,fv1z
 real nuTSA,chi3,nuTd

 real urr0,uss0,utt0

! #If #SOLVER == "INSVP"
 ! --- visco plastic variables ---
 ! declareViscoPlasticVariables()
! #End

 double precision pdb
 character *50 name
 integer ok,getInt,getReal

 integer nc

 integer \
     noSlipWall,\
     inflowWithVelocityGiven,\
     slipWall,\
     outflow,\
     convectiveOutflow,\
     tractionFree,\
     inflowWithPandTV,\
     dirichletBoundaryCondition,\
     symmetry,\
     axisymmetric
 parameter( noSlipWall=1,inflowWithVelocityGiven=2,slipWall=4,outflow=5,convectiveOutflow=14,tractionFree=15,\
  inflowWithPandTV=3,dirichletBoundaryCondition=12,symmetry=11,axisymmetric=13 )

 integer rectangular,curvilinear
 parameter( rectangular=0, curvilinear=1 )

 integer interpolate,dirichlet,neumann,extrapolate
 parameter( interpolate=0, dirichlet=1, neumann=2, extrapolate=3 )

 integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
 parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 )

 !     --- begin statement functions
 real t1,t2
 real uAve0,uAve1,uAve2,uAve3d0,uAve3d1,uAve3d2

 real rxi,rxr,rxs,rxt,rxx,rxy,ryy,rxx3,rxy3,rxz3
 real ur,us,ut,urs,urt,ust,urr,uss,utt
 real uxx0,uyy0,uzz0,ux2c,uy2c,ux3c,uy3c,uz3c
 real lap2d2c,lap3d2c

 real uu, ux2,uy2,uz2,uxx2,uyy2,uzz2,lap2d2,lap3d2
 real ux4,uy4,uz4,uxx4,lap2d4,lap3d4,uxy2,uxz2,uyz2,uxy4,uxz4,uyz4,uyy4,uzz4

 real mixedRHS,mixedCoeff,mixedNormalCoeff,a0,a1
 real an1,an2,an3,aNormi,cnm,cnz,cnp,epsx

 real rx,ry,rz,sx,sy,sz,tx,ty,tz

! include 'declareDiffOrder2f.h'
! include 'declareDiffOrder4f.h'
 declareDifferenceOrder2(u,RX)
! declareDifferenceOrder4(u,RX)

 ! This include file (created above) declares variables needed by the getDuDx() macros. (
!** include 'insLSdeclareTemporaryVariablesOrder2.h'

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
 defineDifferenceOrder2Components1(u,RX)
 ! defineDifferenceOrder4Components1(u,RX)

 !*      include 'insDeriv.h'
 !*      include 'insDerivc.h'


 uu(c)    = u(i1,i2,i3,c)

 ux2(c)   = ux22r(i1,i2,i3,c)
 uy2(c)   = uy22r(i1,i2,i3,c)
 uz2(c)   = uz23r(i1,i2,i3,c)
 uxy2(c)  = uxy22r(i1,i2,i3,c)
 uxz2(c)  = uxz23r(i1,i2,i3,c) 
 uyz2(c)  = uyz23r(i1,i2,i3,c) 
 uxx2(c)  = uxx22r(i1,i2,i3,c) 
 uyy2(c)  = uyy22r(i1,i2,i3,c) 
 uzz2(c)  = uzz23r(i1,i2,i3,c) 
 lap2d2(c)= ulaplacian22r(i1,i2,i3,c)
 lap3d2(c)= ulaplacian23r(i1,i2,i3,c)

!* ux4(c)   = ux42r(i1,i2,i3,c)
!* uy4(c)   = uy42r(i1,i2,i3,c)
!* uz4(c)   = uz43r(i1,i2,i3,c)
!* uxy4(c)  = uxy42r(i1,i2,i3,c)
!* uxz4(c)  = uxz43r(i1,i2,i3,c) 
!* uyz4(c)  = uyz43r(i1,i2,i3,c) 
!* uxx4(c)  = uxx42r(i1,i2,i3,c) 
!* uyy4(c)  = uyy42r(i1,i2,i3,c) 
!* uzz4(c)  = uzz43r(i1,i2,i3,c) 
!* lap2d4(c)= ulaplacian42r(i1,i2,i3,c)
!* lap3d4(c)= ulaplacian43r(i1,i2,i3,c)

 rxi(m,n) = rsxy(i1,i2,i3,m,n)
 rxr(m,n) = (rsxy(i1+1,i2,i3,m,n)-rsxy(i1-1,i2,i3,m,n))*dr2i
 rxs(m,n) = (rsxy(i1,i2+1,i3,m,n)-rsxy(i1,i2-1,i3,m,n))*ds2i
 rxt(m,n) = (rsxy(i1,i2,i3+1,m,n)-rsxy(i1,i2,i3-1,m,n))*dt2i
 rxx(m,n) = rxi(0,0)*rxr(m,n)+rxi(1,0)*rxs(m,n)
 rxy(m,n) = rxi(0,1)*rxr(m,n)+rxi(1,1)*rxs(m,n)
 ryy(m,n) = rxy(m,n)

 rxx3(m,n)= rxi(0,0)*rxr(m,n)+rxi(1,0)*rxs(m,n)+rxi(2,0)*rxt(m,n)
 rxy3(m,n)= rxi(0,1)*rxr(m,n)+rxi(1,1)*rxs(m,n)+rxi(2,1)*rxt(m,n)
 rxz3(m,n)= rxi(0,2)*rxr(m,n)+rxi(1,2)*rxs(m,n)+rxi(2,2)*rxt(m,n)

 ur(m) = ur2(i1,i2,i3,m)
 us(m) = us2(i1,i2,i3,m) 
 ut(m) = ut2(i1,i2,i3,m) 
 urs(m)= urs2(i1,i2,i3,m) 
 urt(m)= urt2(i1,i2,i3,m) 
 ust(m)= ust2(i1,i2,i3,m) 
 urr(m)= urr2(i1,i2,i3,m)
 uss(m)= uss2(i1,i2,i3,m)
 utt(m)= utt2(i1,i2,i3,m)

 ux2c(m) = ux22(i1,i2,i3,m)
 uy2c(m) = uy22(i1,i2,i3,m)

 ux3c(m) = ux23(i1,i2,i3,m)
 uy3c(m) = uy23(i1,i2,i3,m)
 uz3c(m) = uz23(i1,i2,i3,m)

 lap2d2c(m) = ulaplacian22(i1,i2,i3,m)
 lap3d2c(m) = ulaplacian23(i1,i2,i3,m)                            


 uxx0(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))*dxsqi  ! without diagonal term
 uyy0(c) = (u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))*dysqi  ! without diagonal term
 uzz0(c) = (u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))*dzsqi  ! without diagonal term

 urr0(m)  = (u(i1+1,i2,i3,m)+u(i1-1,i2,i3,m))*drsqi  ! without diagonal term
 uss0(m)  = (u(i1,i2+1,i3,m)+u(i1,i2-1,i3,m))*dssqi  ! without diagonal term
 utt0(m)  = (u(i1,i2,i3+1,m)+u(i1,i2,i3-1,m))*dtsqi  ! without diagonal term

 uAve0(c) = (u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))
 uAve1(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c))
 uAve2(c) = 0.

 uAve3d0(c) = (u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))
 uAve3d1(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2,i3+1,c)+u(i1,i2,i3-1,c))
 uAve3d2(c) = (u(i1+1,i2,i3,c)+u(i1-1,i2,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2-1,i3,c))


! statement functions to access coefficients of mixed-boundary conditions
 mixedRHS(c,side,axis,grid)         =bcData(c+numberOfComponents*(0),side,axis,grid)
 mixedCoeff(c,side,axis,grid)       =bcData(c+numberOfComponents*(1),side,axis,grid)
 mixedNormalCoeff(c,side,axis,grid) =bcData(c+numberOfComponents*(2),side,axis,grid)
 !     --- end statement functions

 ierr=0
 ! write(*,*) 'Inside insLineSolve'


 ! This next macro is defined in initLineSolveParameters.h 
 INITIALIZE(SOLVER)

 ! write(*,'(" entering NAME ")') 


 ! ****** Boundary Conditions ******
 indexRange(0,0)=n1a
 indexRange(1,0)=n1b
 indexRange(0,1)=n2a
 indexRange(1,1)=n2b
 indexRange(0,2)=n3a
 indexRange(1,2)=n3b


 ! Assign BC's on tangential directions -- 

 do axis=0,nd-1
  if( axis.ne.dir )then
  do side=0,1

   if( axis.eq.0 )then
     is1=1-2*side
     n1a=indexRange(side,axis)
     n1b=n1a
   else if( axis.eq.1 )then
     is2=1-2*side
     n2a=indexRange(side,axis)
     n2b=n2a
   else
     is3=1-2*side
     n3a=indexRange(side,axis)
     n3b=n3a
   end if

   if( boundaryCondition(side,axis).eq.dirichletBoundaryCondition .or.\
       boundaryCondition(side,axis).eq.noSlipWall.or.\
       boundaryCondition(side,axis).eq.inflowWithVelocityGiven )then
    
    if( systemComponent.eq.uc .or. systemComponent.eq.vc .or. systemComponent.eq.wc )then

     beginLoops()
      ! fill in the identity matrix on the boundary (dicihlet BC)
      am(i1,i2,i3)=0.
      bm(i1,i2,i3)=1.
      cm(i1,i2,i3)=0.
     endLoops()

    else if( computeTemperature.ne.0 .and. systemComponent.eq.tc )then

     a0 = mixedCoeff(tc,side,axis,grid)
     a1 = mixedNormalCoeff(tc,side,axis,grid)
     if( a1.eq.0. )then
      ! Dirichlet BC for T
      beginLoops()
       ! fill in the identity matrix on the boundary (dicihlet BC)
       am(i1,i2,i3)=0.
       bm(i1,i2,i3)=1.
       cm(i1,i2,i3)=0.
      endLoops()

     else 
      if( boundaryCondition(side,axis).eq.inflowWithVelocityGiven )then
        write(*,'(" insLineBC: mixed BC at inflow!")') 
      end if
     end if

    end if

   else if( boundaryCondition(side,axis).eq.outflow )then
    ! leave as is

   else if( boundaryCondition(side,axis).eq.slipWall )then

    if( systemComponent.eq.(uc+axis) )then
      ! normal component is dirichlet (leave other components as the eqn)
   
     beginLoops()
      ! fill in the identity matrix on the boundary 
      am(i1,i2,i3)=0.
      bm(i1,i2,i3)=1.
      cm(i1,i2,i3)=0.
     endLoops()
    
    end if
   else if( boundaryCondition(side,axis).gt.0 )then
     write(*,'("insLineBC: ERROR unknown bc=",i4)') boundaryCondition(side,axis)
   end if

   ! reset values
   if( axis.eq.0 )then
     n1a=indexRange(0,axis)
     n1b=indexRange(1,axis)
   else if( axis.eq.1 )then
     n2a=indexRange(0,axis)
     n2b=indexRange(1,axis)
   else
     n3a=indexRange(0,axis)
     n3b=indexRange(1,axis)
   end if

  end do ! side
  end if
 end do ! axis



! assign loop variables to correspond to the boundary
 
 epsx = 1.e-20   ! for normal, fix me, use REAL_MIN*100 ??

 do side=0,1
  is1=0
  is2=0
  is3=0
  if( dir.eq.0 )then
    is1=1-2*side
    n1a=indexRange(side,dir)-is1    ! boundary is 1 pt outside
    n1b=n1a
  else if( dir.eq.1 )then
    is2=1-2*side
    n2a=indexRange(side,dir)-is2
    n2b=n2a
  else
    is3=1-2*side
    n3a=indexRange(side,dir)-is3
    n3b=n3a
  end if
    
  sn=2*side-1 ! sign for normal

  if( bc(side,ibc).eq.dirichlet .and. boundaryCondition(side,dir).gt.0 )then

   beginLoops()
    j1=i1+is1
    j2=i2+is2
    j3=i3+is3
    if( mask(i1,i2,i3).gt.0 )then
     ! fill in the identity matrix on the boundary and ghost line 
     am(i1,i2,i3)=0.
     bm(i1,i2,i3)=1.
     cm(i1,i2,i3)=0.

     am(j1,j2,j3)=0.
     bm(j1,j2,j3)=1.
     cm(j1,j2,j3)=0.
    else
     ! for interpolation points or unused:
     am(i1,i2,i3)=0.
     bm(i1,i2,i3)=1.
     cm(i1,i2,i3)=0.

     am(j1,j2,j3)=0.
     bm(j1,j2,j3)=1.
     cm(j1,j2,j3)=0.
    end if
   endLoops()

  else if( bc(side,ibc).eq.dirichlet .and. boundaryCondition(side,dir).le.0 )then
   ! this must be an internal parallel boundary
   beginLoops()
    if( mask(i1,i2,i3).gt.0 )then
     ! fill in the identity matrix on the boundary 
     am(i1,i2,i3)=0.
     bm(i1,i2,i3)=1.
     cm(i1,i2,i3)=0.
    else
     ! for interpolation points or unused:
     am(i1,i2,i3)=0.
     bm(i1,i2,i3)=1.
     cm(i1,i2,i3)=0.
    end if
   endLoops()

  else if( bc(side,ibc).eq.neumann )then 
   
    ! apply a neumann BC on this side.
    !             | b[0] c[0] a[0]                |
    !             | a[1] b[1] c[1]                |
    !         A = |      a[2] b[2] c[2]           |
    !             |            .    .    .        |
    !             |                a[.] b[.] c[.] |
    !             |                c[n] a[n] b[n] |

   ! write(*,'(">>>insLineSolveBC: neumann BC for side,dir,grid=",3i3, " ibc,computeT,systemComponent,nc=",4i3)') side,dir,grid,ibc,computeTemperature,systemComponent,numberOfComponents
   ! ' 

   if( computeTemperature.ne.0 .and. systemComponent.eq.tc .and. boundaryCondition(side,dir).ne.slipWall)then
    ! mixed boundary condition on T 

    a0 = mixedCoeff(tc,side,dir,grid)
    a1 = mixedNormalCoeff(tc,side,dir,grid)
    ! write(*,'(" insLineSolveBC: T BC: (a0,a1)=(",f3.1,",",f3.1,") for side,dir,grid=",3i3)') a0,a1,side,dir,grid
    ! '
    ! write(*,*) 'bcData=',bcData

    ! a0*u + a1*u.n = 
    ! a0*u + a1*( n1*( rx*ur + sx*us ) + n2*( ry*ur + sy*us ) )
    !  n1 = rsxy(dir,0), n2=rsxy(dir,1)

    if( gridType.eq.rectangular )then
     ! mixed-BC : rectangular
     if( side.eq.0 )then
      ! left side with outward normal : [1 0 -1] = [b c a]
      cnb= a1/(2.*dx(dir))
      cnc= a0
      cna=-cnb 
     else
      ! right side with outward normal : [-1 0 1] = [c a b]
      cnc=-a1/(2.*dx(dir))
      cna= a0
      cnb=-cnc
     end if
  
     beginLoops()
      if( mask(i1,i2,i3).gt.0 )then
       am(i1,i2,i3)=cna
       bm(i1,i2,i3)=cnb
       cm(i1,i2,i3)=cnc
      else
       ! for interpolation points or unused:
       am(i1,i2,i3)=0.
       bm(i1,i2,i3)=1.
       cm(i1,i2,i3)=0.
      end if
     endLoops()


    else if( gridType.eq.curvilinear )then
     if( nd.eq.2 )then
      ! mixed-BC : 2D curvilinear  
      beginLoops()
      if( mask(i1,i2,i3).gt.0 )then
 
       an1 = rsxy(i1,i2,i3,dir,0)
       an2 = rsxy(i1,i2,i3,dir,1)
       aNormi = sn/max( epsx,sqrt(an1**2 + an2**2) )  ! note: multiply by the sign of the normal 
       an1=an1*aNormi
       an2=an2*aNormi
 
       ! cnm : coeff of ghost point
       ! cnp : coeff of first point in 
       cnm = -a1*( an1*rsxy(i1,i2,i3,dir,0) + an2*rsxy(i1,i2,i3,dir,1) )/(2.*dr(dir))
       cnz = a0 
       cnp = -cnm 

       if( side.eq.0 )then
         bm(i1,i2,i3)=cnm
         cm(i1,i2,i3)=cnz
         am(i1,i2,i3)=cnp
       else
         cm(i1,i2,i3)=cnm
         am(i1,i2,i3)=cnz
         bm(i1,i2,i3)=cnp
       end if
      else
       ! for interpolation points or unused:
       am(i1,i2,i3)=0.
       bm(i1,i2,i3)=1.
       cm(i1,i2,i3)=0.
      end if
      endLoops()

     else if( nd.eq.3 )then
      ! mixed-BC : 3D curvilinear  
      beginLoops()
      if( mask(i1,i2,i3).gt.0 )then
 
       an1 = rsxy(i1,i2,i3,dir,0)
       an2 = rsxy(i1,i2,i3,dir,1)
       an3 = rsxy(i1,i2,i3,dir,2)
       aNormi = sn/max( epsx,sqrt(an1**2 + an2**2 + an3**2) )  ! note: multiply by the sign of the normal 
       an1=an1*aNormi
       an2=an2*aNormi
       an3=an3*aNormi
 
       cnm = -a1*( an1*rsxy(i1,i2,i3,dir,0) + an2*rsxy(i1,i2,i3,dir,1) + an3*rsxy(i1,i2,i3,dir,2))/(2.*dr(dir))
       cnz = a0 
       cnp = -cnm 

       if( side.eq.0 )then
         bm(i1,i2,i3)=cnm
         cm(i1,i2,i3)=cnz
         am(i1,i2,i3)=cnp
       else
         cm(i1,i2,i3)=cnm
         am(i1,i2,i3)=cnz
         bm(i1,i2,i3)=cnp
       end if
      else
       ! for interpolation points or unused:
       am(i1,i2,i3)=0.
       bm(i1,i2,i3)=1.
       cm(i1,i2,i3)=0.
      end if
      endLoops()

     end if

    else
      write(*,'(" lineSolveBC: unknown gridType=",i6)') gridType
      stop 10013
    end if

   else ! not Temperature

    if( side.eq.0 )then
     ! left side with outward normal : [1 0 -1] = [b c a]
     cnb= 1.
     cnc= 0.
     cna=-1.
     !      loopsMatrixBC(SOLVER,\
     !                    bm(i1,i2,i3)= 1.,\
     !                    cm(i1,i2,i3)=0.,\
     !                    am(i1,i2,i3)=-1.,,,)
    else
     ! right side with outward normal : [-1 0 1] = [c a b]
     cnc=-1.
     cna= 0.
     cnb= 1.
     !     loopsMatrixBC(SOLVER,\
     !                    cm(i1,i2,i3)=-1.,\
     !                    am(i1,i2,i3)=0.,\
     !                    bm(i1,i2,i3)= 1.,,,)
    end if

    beginLoops()
     if( mask(i1,i2,i3).gt.0 )then
      am(i1,i2,i3)=cna
      bm(i1,i2,i3)=cnb
      cm(i1,i2,i3)=cnc
     else
      ! for interpolation points or unused:
      am(i1,i2,i3)=0.
      bm(i1,i2,i3)=1.
      cm(i1,i2,i3)=0.
     end if
    endLoops()

   end if

  else if( bc(side,ibc).eq.extrapolate )then 

   ! **** second order ****
   if( orderOfExtrapolation.eq.2 )then
     if( side.eq.0 )then
       cexa= 1.
       cexb= 1.
       cexc=-2.
     else
       cexa=-2.
       cexb= 1.
       cexc= 1.
     end if
   else if( orderOfExtrapolation.eq.3 )then 
     if( side.eq.0 )then
       cexa= 3.
       cexb= 1.
       cexc=-3.
     else
       cexa=-3.
       cexb= 1.
       cexc= 3.
     end if
   else
     write(*,*) 'ERROR: not implemeted: orderOfExtrapolation=',orderOfExtrapolation
     stop 1111
   end if

   beginLoops()
    if( mask(i1,i2,i3).gt.0 )then
     am(i1,i2,i3)=cexa
     bm(i1,i2,i3)=cexb
     cm(i1,i2,i3)=cexc
    else
     ! for interpolation points or unused:
     am(i1,i2,i3)=0.
     bm(i1,i2,i3)=1.
     cm(i1,i2,i3)=0.
    end if
   endLoops()

   ! loopsMatrixBC( SOLVER,\
   !                am(i1,i2,i3)=cexa,\
   !                bm(i1,i2,i3)=cexb,\
   !                cm(i1,i2,i3)=cexc,,,)

  else if( bc(side,ibc).gt.0 )then
   write(*,'(" lineSolve:BC: unknown bc=",i2)') bc(side,ibc)
   stop 7102

  end if


  ! reset values
  if( dir.eq.0 )then
    n1a=indexRange(0,dir)
    n1b=indexRange(1,dir)
  else if( dir.eq.1 )then
    n2a=indexRange(0,dir)
    n2b=indexRange(1,dir)
  else
    n3a=indexRange(0,dir)
    n3b=indexRange(1,dir)
  end if
 end do ! do side



 ! Now fix up the "ends" of some boundary conditions

c$$$ do side=0,1
c$$$  is1=0
c$$$  is2=0
c$$$  is3=0
c$$$  if( dir.eq.0 )then
c$$$    is1=1-2*side
c$$$    n1a=indexRange(side,dir)-is1    ! boundary is 1 pt outside
c$$$    n1b=n1a
c$$$  else if( dir.eq.1 )then
c$$$    is2=1-2*side
c$$$    n2a=indexRange(side,dir)-is2
c$$$    n2b=n2a
c$$$  else
c$$$    is3=1-2*side
c$$$    n3a=indexRange(side,dir)-is3
c$$$    n3b=n3a
c$$$  end if
c$$$    
c$$$  sn=2*side-1 ! sign for normal
c$$$
c$$$  if( boundaryCondition(side,axis).eq.slipWall )then
c$$$
c$$$    ! The last point on slip walls: extrapolate the tang. velocity rather than using 
c$$$    if( systemComponent.eq.(uc+axis) )then
c$$$      ! normal component is dirichlet (leave other components as the eqn)
c$$$   
c$$$     beginLoops()
c$$$      ! fill in the identity matrix on the boundary 
c$$$      am(i1,i2,i3)=0.
c$$$      bm(i1,i2,i3)=1.
c$$$      cm(i1,i2,i3)=0.
c$$$     endLoops()
c$$$    
c$$$    end if
c$$$
c$$$  end if
c$$$
c$$$
c$$$  ! reset values
c$$$  if( dir.eq.0 )then
c$$$    n1a=indexRange(0,dir)
c$$$    n1b=indexRange(1,dir)
c$$$  else if( dir.eq.1 )then
c$$$    n2a=indexRange(0,dir)
c$$$    n2b=indexRange(1,dir)
c$$$  else
c$$$    n3a=indexRange(0,dir)
c$$$    n3b=indexRange(1,dir)
c$$$  end if
c$$$ end do ! do side



 return
 end
#endMacro

#beginMacro INS_LINE_BC_NULL(SOLVER,NAME)
 subroutine NAME(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
      md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw,\
      dir,am,bm,cm,dm,em,  bc, boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c======================================================================
c
c        ****** NULL version **********
c 
c Used if we don't want to compile the real file for a given case
c======================================================================
 return
 end
#endMacro


#beginMacro buildFile(SOLVER,NAME)
#beginFile src/NAME.f
 INS_LINE_BC(SOLVER,NAME)
#endFile
! #beginFile src/NAME ## Null.f
!  INS_LINE_BC_NULL(SOLVER,NAME)
! #endFile
#endMacro


      buildFile(INS,lineSolveBcINS)
      buildFile(INSVP,lineSolveBcINSVP)

c      buildFile(INSSPAL,lineSolveINSSPAL)
c      buildFile(INSBL,lineSolveINSBL)



      subroutine insLineSolveBC(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
       md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
       ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c======================================================================
c
c         ************* INS Line Solver BC Function ***************
c
c This function can:
c  (1) Fill in the matrix coefficents for BC's for line solvers
c  (2) Assign the BC right-hand-side values in f
c
c NOTES:
c   Fill in the interior equation for points (n1a:n1b,n2a:n2b,n3a:n3b)
c   Fill in the BC equations for points outside this (along the line solver direction)
c   
c nd : number of space dimensions
c
c n1a,n1b,n2a,n2b,n3a,n3b : INTERIOR points (does not include boundary points along axis=dir)
c
c dir : 0,1,2 - direction of line 
c a,b,c : output: tridiagonal matrix
c a,b,c,d,e  : output: penta-diagonal matrix (for fourth-order)
c
c bc(0:1,0:nd-1) : line solver BC's 
c boundaryCondition(0:1,0:nd-1) : MappedGrid boundary conditions

c ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b : dimensions for the bcData array
c bcData : holds coefficients for BC's
c
c dw: distance to wall for SA TM
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     & md1a,md1b,md2a,md2b,md3a,md3b,
     & dir

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real am(md1a:md1b,md2a:md2b,md3a:md3b)
      real bm(md1a:md1b,md2a:md2b,md3a:md3b)
      real cm(md1a:md1b,md2a:md2b,md3a:md3b)
      real dm(md1a:md1b,md2a:md2b,md3a:md3b)
      real em(md1a:md1b,md2a:md2b,md3a:md3b)

      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      real dt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:*),boundaryCondition(0:1,0:*), ierr

      ! bcData(component+numberOfComponents*(0),side,axis,grid)  
      integer ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b
      real bcData(ndbcd1a:ndbcd1b,ndbcd2a:ndbcd2b,ndbcd3a:ndbcd3b,ndbcd4a:ndbcd4b)

      integer ipar(0:*)
      real rpar(0:*)
      
c     ---- local variables -----
      integer option
      integer assignINS,assignSpalartAllmaras,setupSweep,assignTemperature
      parameter( assignINS=0, assignSpalartAllmaras=1, setupSweep=2, assignTemperature=3 )

      integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
      parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 )

      integer turbulenceModel,noTurbulenceModel
      integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
      parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

c     --- end statement functions

      ierr=0
      ! write(*,*) 'Inside insLineSolve'

      option            = ipar(21)
      turbulenceModel   = ipar(23)
      pdeModel          = ipar(27)

      if( turbulenceModel.eq.noTurbulenceModel )then
        if( pdeModel.eq.standardModel .or. pdeModel.eq.BoussinesqModel )then
          call lineSolveBcINS(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
           md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
           ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
        else if( pdeModel.eq.viscoPlasticModel )then
          call lineSolveBcINSVP(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
           md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
           ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
        else
          stop 5533
        end if
        
c      else if( turbulenceModel.eq.spalartAllmaras )then
c        call lineSolveNewINSSPAL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c          md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
c          ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c      else if( turbulenceModel.eq.baldwinLomax )then
c        call lineSolveNewINSBL(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
c          md1a,md1b,md2a,md2b,md3a,md3b, mask,rsxy,  u,gv,dt,f,dw, dir,am,bm,cm,dm,em,  bc, boundaryCondition, \
c          ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
      else
        write(*,*) 'insLineSolveBC:Unknown turbulenceModel=',turbulenceModel
        stop 444
      end if      

      return
      end



