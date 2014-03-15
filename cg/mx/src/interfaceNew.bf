c *******************************************************************************
c   Interface boundary conditions **new version**
c
c    This file calls functions from interfaceDeriavtives.bf
c
c *******************************************************************************

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

#defineMacro extrap7(uu,k1,k2,k3,kc,ks1,ks2,ks3) \
            (7.*uu(k1,k2,k3,kc)-21.*uu(k1+ks1,k2+ks2,k3+ks3,kc)\
            +35.*uu(k1+2*ks1,k2+2*ks2,k3+2*ks3,kc)-35.*uu(k1+3*ks1,k2+3*ks2,k3+3*ks3,kc)\
            +21.*uu(k1+4*ks1,k2+4*ks2,k3+4*ks3,kc)-7.*uu(k1+5*ks1,k2+5*ks2,k3+5*ks3,kc) \
            +uu(k1+6*ks1,k2+6*ks2,k3+6*ks3,kc))

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


! update the periodic ghost points
#beginMacro periodicUpdate2d(u,bc,gid,side,axis)

axisp1=mod(axis+1,nd)
if( bc(0,axisp1).lt.0 )then
  ! direction axisp1 is periodic
  diff(axis)=0
  diff(axisp1)=gid(1,axisp1)-gid(0,axisp1)

  if( side.eq.0 )then
    ! assign 4 ghost points outside lower corner
    np1a=gid(0,0)-numGhost
    np1b=gid(0,0)-1
    np2a=gid(0,1)-numGhost
    np2b=gid(0,1)-1

    beginLoops(np1a,np1b,np2a,np2b,n3a,n3b,ex,hz)
     u(i1,i2,i3,n) = u(i1+diff(0),i2+diff(1),i3,n)
    endLoops()

    ! assign 4 ghost points outside upper corner
    if( axis.eq.0 )then
      np2a=gid(1,axisp1)+1
      np2b=gid(1,axisp1)+numGhost
    else
      np1a=gid(1,axisp1)+1
      np1b=gid(1,axisp1)+numGhost
    end if

    beginLoops(np1a,np1b,np2a,np2b,n3a,n3b,ex,hz)
     u(i1,i2,i3,n) = u(i1-diff(0),i2-diff(1),i3,n)
    endLoops()

  else

    ! assign 4 ghost points outside upper corner
    np1a=gid(1,0)+1
    np1b=gid(1,0)+numGhost
    np2a=gid(1,1)+1
    np2b=gid(1,1)+numGhost

    beginLoops(np1a,np1b,np2a,np2b,n3a,n3b,ex,hz)
     u(i1,i2,i3,n) = u(i1-diff(0),i2-diff(1),i3,n)
    endLoops()

    if( axis.eq.0 )then
      np2a=gid(0,axisp1)-numGhost
      np2b=gid(0,axisp1)-1
    else
      np1a=gid(0,axisp1)-numGhost
      np1b=gid(0,axisp1)-1
    end if

    beginLoops(np1a,np1b,np2a,np2b,n3a,n3b,ex,hz)
     u(i1,i2,i3,n) = u(i1+diff(0),i2+diff(1),i3,n)
    endLoops()
  end if

endif


#endMacro


c MAT: 1 or 2 for material 1 or 2
c DIR: 0 or 1 for r or s direction
#beginMacro evalCoeffOrder6(MAT,DIR,dr,ds)

#If DIR == 0 
 c ## MAT ##x6(1) = xCoeff2dOrder6Ghost10(a ## MAT ##j6,dr,ds)
 c ## MAT ##x6(2) = xCoeff2dOrder6Ghost20(a ## MAT ##j6,dr,ds)
 c ## MAT ##x6(3) = xCoeff2dOrder6Ghost30(a ## MAT ##j6,dr,ds)

 c ## MAT ##y6(1) = yCoeff2dOrder6Ghost10(a ## MAT ##j6,dr,ds)
 c ## MAT ##y6(2) = yCoeff2dOrder6Ghost20(a ## MAT ##j6,dr,ds)
 c ## MAT ##y6(3) = yCoeff2dOrder6Ghost30(a ## MAT ##j6,dr,ds)

 c ## MAT ##Lap6(1) = lapCoeff2dOrder6Ghost10(a ## MAT ##j6,dr,ds)
 c ## MAT ##Lap6(2) = lapCoeff2dOrder6Ghost20(a ## MAT ##j6,dr,ds)
 c ## MAT ##Lap6(3) = lapCoeff2dOrder6Ghost30(a ## MAT ##j6,dr,ds)

 ! 4th order values:

 c ## MAT ##xLap4(1) = xLapCoeff2dOrder4Ghost10(a ## MAT ##j4,dr,ds)
 c ## MAT ##xLap4(2) = xLapCoeff2dOrder4Ghost20(a ## MAT ##j4,dr,ds)
 c ## MAT ##xLap4(3) = xLapCoeff2dOrder4Ghost30(a ## MAT ##j4,dr,ds)

 c ## MAT ##yLap4(1) = yLapCoeff2dOrder4Ghost10(a ## MAT ##j4,dr,ds)
 c ## MAT ##yLap4(2) = yLapCoeff2dOrder4Ghost20(a ## MAT ##j4,dr,ds)
 c ## MAT ##yLap4(3) = yLapCoeff2dOrder4Ghost30(a ## MAT ##j4,dr,ds)

 c ## MAT ##LapSq4(1) = lapSqCoeff2dOrder4Ghost10(a ## MAT ##j4,dr,ds)
 c ## MAT ##LapSq4(2) = lapSqCoeff2dOrder4Ghost20(a ## MAT ##j4,dr,ds)
 c ## MAT ##LapSq4(3) = lapSqCoeff2dOrder4Ghost30(a ## MAT ##j4,dr,ds)

 ! 2nd order values:

 c ## MAT ##xLapSq2(1) = xLapSqCoeff2dOrder2Ghost10(a ## MAT ##j2,dr,ds)
 c ## MAT ##xLapSq2(2) = xLapSqCoeff2dOrder2Ghost20(a ## MAT ##j2,dr,ds)
 c ## MAT ##xLapSq2(3) = xLapSqCoeff2dOrder2Ghost30(a ## MAT ##j2,dr,ds)

 c ## MAT ##yLapSq2(1) = yLapSqCoeff2dOrder2Ghost10(a ## MAT ##j2,dr,ds)
 c ## MAT ##yLapSq2(2) = yLapSqCoeff2dOrder2Ghost20(a ## MAT ##j2,dr,ds)
 c ## MAT ##yLapSq2(3) = yLapSqCoeff2dOrder2Ghost30(a ## MAT ##j2,dr,ds)

 c ## MAT ##LapCubed2(1) = lapCubedCoeff2dOrder2Ghost10(a ## MAT ##j2,dr,ds)
 c ## MAT ##LapCubed2(2) = lapCubedCoeff2dOrder2Ghost20(a ## MAT ##j2,dr,ds)
 c ## MAT ##LapCubed2(3) = lapCubedCoeff2dOrder2Ghost30(a ## MAT ##j2,dr,ds)

#Elif DIR == 1

 c ## MAT ##x6(1) = xCoeff2dOrder6Ghost01(a ## MAT ##j6,dr,ds)
 c ## MAT ##x6(2) = xCoeff2dOrder6Ghost02(a ## MAT ##j6,dr,ds)
 c ## MAT ##x6(3) = xCoeff2dOrder6Ghost03(a ## MAT ##j6,dr,ds)

 c ## MAT ##y6(1) = yCoeff2dOrder6Ghost01(a ## MAT ##j6,dr,ds)
 c ## MAT ##y6(2) = yCoeff2dOrder6Ghost02(a ## MAT ##j6,dr,ds)
 c ## MAT ##y6(3) = yCoeff2dOrder6Ghost03(a ## MAT ##j6,dr,ds)

 c ## MAT ##Lap6(1) = lapCoeff2dOrder6Ghost01(a ## MAT ##j6,dr,ds)
 c ## MAT ##Lap6(2) = lapCoeff2dOrder6Ghost02(a ## MAT ##j6,dr,ds)
 c ## MAT ##Lap6(3) = lapCoeff2dOrder6Ghost03(a ## MAT ##j6,dr,ds)

 ! 4th order values:

 c ## MAT ##xLap4(1) = xLapCoeff2dOrder4Ghost01(a ## MAT ##j4,dr,ds)
 c ## MAT ##xLap4(2) = xLapCoeff2dOrder4Ghost02(a ## MAT ##j4,dr,ds)
 c ## MAT ##xLap4(3) = xLapCoeff2dOrder4Ghost03(a ## MAT ##j4,dr,ds)

 c ## MAT ##yLap4(1) = yLapCoeff2dOrder4Ghost01(a ## MAT ##j4,dr,ds)
 c ## MAT ##yLap4(2) = yLapCoeff2dOrder4Ghost02(a ## MAT ##j4,dr,ds)
 c ## MAT ##yLap4(3) = yLapCoeff2dOrder4Ghost03(a ## MAT ##j4,dr,ds)

 c ## MAT ##LapSq4(1) = lapSqCoeff2dOrder4Ghost01(a ## MAT ##j4,dr,ds)
 c ## MAT ##LapSq4(2) = lapSqCoeff2dOrder4Ghost02(a ## MAT ##j4,dr,ds)
 c ## MAT ##LapSq4(3) = lapSqCoeff2dOrder4Ghost03(a ## MAT ##j4,dr,ds)

 ! 2nd order values:

 c ## MAT ##xLapSq2(1) = xLapSqCoeff2dOrder2Ghost01(a ## MAT ##j2,dr,ds)
 c ## MAT ##xLapSq2(2) = xLapSqCoeff2dOrder2Ghost02(a ## MAT ##j2,dr,ds)
 c ## MAT ##xLapSq2(3) = xLapSqCoeff2dOrder2Ghost03(a ## MAT ##j2,dr,ds)

 c ## MAT ##yLapSq2(1) = yLapSqCoeff2dOrder2Ghost01(a ## MAT ##j2,dr,ds)
 c ## MAT ##yLapSq2(2) = yLapSqCoeff2dOrder2Ghost02(a ## MAT ##j2,dr,ds)
 c ## MAT ##yLapSq2(3) = yLapSqCoeff2dOrder2Ghost03(a ## MAT ##j2,dr,ds)

 c ## MAT ##LapCubed2(1) = lapCubedCoeff2dOrder2Ghost01(a ## MAT ##j2,dr,ds)
 c ## MAT ##LapCubed2(2) = lapCubedCoeff2dOrder2Ghost02(a ## MAT ##j2,dr,ds)
 c ## MAT ##LapCubed2(3) = lapCubedCoeff2dOrder2Ghost03(a ## MAT ##j2,dr,ds)
#Else
  stop 8843
#End

#endMacro


c Evaluate the BC equations and fill in f(i) 
#beginMacro evaluateEquationsOrder6()

 ! ******************* 6th order ******************
 evalParametricDerivativesComponents1(u1,i1,i2,i3,ex, uu1,2,6,2)
 evalParametricDerivativesComponents1(u1,i1,i2,i3,ey, vv1,2,6,2)

 evalParametricDerivativesComponents1(u2,j1,j2,j3,ex, uu2,2,6,2)
 evalParametricDerivativesComponents1(u2,j1,j2,j3,ey, vv2,2,6,2)

 ! 1st derivatives, 6th order
 getDuDx2(uu1,a1j6,uu1x6)
 getDuDy2(uu1,a1j6,uu1y6)
 getDuDx2(vv1,a1j6,vv1x6)
 getDuDy2(vv1,a1j6,vv1y6)

 getDuDx2(uu2,a2j6,uu2x6)
 getDuDy2(uu2,a2j6,uu2y6)
 getDuDx2(vv2,a2j6,vv2x6)
 getDuDy2(vv2,a2j6,vv2y6)

 ! 2nd derivatives, 6th order
 getDuDxx2(uu1,a1j6,uu1xx6)
 getDuDyy2(uu1,a1j6,uu1yy6)
 getDuDxx2(vv1,a1j6,vv1xx6)
 getDuDyy2(vv1,a1j6,vv1yy6)

 getDuDxx2(uu2,a2j6,uu2xx6)
 getDuDyy2(uu2,a2j6,uu2yy6)
 getDuDxx2(vv2,a2j6,vv2xx6)
 getDuDyy2(vv2,a2j6,vv2yy6)

 ulap1=uu1xx6+uu1yy6
 vlap1=vv1xx6+vv1yy6

 ulap2=uu2xx6+uu2yy6
 vlap2=vv2xx6+vv2yy6

 ! ****** fourth order ******

 evalParametricDerivativesComponents1(u1,i1,i2,i3,ex, uu1,2,4,4)
 evalParametricDerivativesComponents1(u1,i1,i2,i3,ey, vv1,2,4,4)

 evalParametricDerivativesComponents1(u2,j1,j2,j3,ex, uu2,2,4,4)
 evalParametricDerivativesComponents1(u2,j1,j2,j3,ey, vv2,2,4,4)

 ! 3rd derivatives, 4th order
 getDuDxxx2(uu1,a1j4,uu1xxx4)
 getDuDxxy2(uu1,a1j4,uu1xxy4)
 getDuDxyy2(uu1,a1j4,uu1xyy4)
 getDuDyyy2(uu1,a1j4,uu1yyy4)

 getDuDxxx2(vv1,a1j4,vv1xxx4)
 getDuDxxy2(vv1,a1j4,vv1xxy4)
 getDuDxyy2(vv1,a1j4,vv1xyy4)
 getDuDyyy2(vv1,a1j4,vv1yyy4)

 getDuDxxx2(uu2,a2j4,uu2xxx4)
 getDuDxxy2(uu2,a2j4,uu2xxy4)
 getDuDxyy2(uu2,a2j4,uu2xyy4)
 getDuDyyy2(uu2,a2j4,uu2yyy4)

 getDuDxxx2(vv2,a2j4,vv2xxx4)
 getDuDxxy2(vv2,a2j4,vv2xxy4)
 getDuDxyy2(vv2,a2j4,vv2xyy4)
 getDuDyyy2(vv2,a2j4,vv2yyy4)

 ! 4th derivatives, 4th order
 getDuDxxxx2(uu1,a1j4,uu1xxxx4)
 getDuDxxyy2(uu1,a1j4,uu1xxyy4)
 getDuDyyyy2(uu1,a1j4,uu1yyyy4)

 getDuDxxxx2(vv1,a1j4,vv1xxxx4)
 getDuDxxyy2(vv1,a1j4,vv1xxyy4)
 getDuDyyyy2(vv1,a1j4,vv1yyyy4)

 getDuDxxxx2(uu2,a2j4,uu2xxxx4)
 getDuDxxyy2(uu2,a2j4,uu2xxyy4)
 getDuDyyyy2(uu2,a2j4,uu2yyyy4)

 getDuDxxxx2(vv2,a2j4,vv2xxxx4)
 getDuDxxyy2(vv2,a2j4,vv2xxyy4)
 getDuDyyyy2(vv2,a2j4,vv2yyyy4)



 ulapSq1=uu1xxxx4+2.*uu1xxyy4+uu1yyyy4
 vlapSq1=vv1xxxx4+2.*vv1xxyy4+vv1yyyy4

 ulapSq2=uu2xxxx4+2.*uu2xxyy4+uu2yyyy4
 vlapSq2=vv2xxxx4+2.*vv2xxyy4+vv2yyyy4


 ! ****** 2nd order ******

 evalParametricDerivativesComponents1(u1,i1,i2,i3,ex, uu1,2,2,6)
 evalParametricDerivativesComponents1(u1,i1,i2,i3,ey, vv1,2,2,6)

 evalParametricDerivativesComponents1(u2,j1,j2,j3,ex, uu2,2,2,6)
 evalParametricDerivativesComponents1(u2,j1,j2,j3,ey, vv2,2,2,6)

 ! 5th derivatives, 2nd order
 getDuDxxxxx2(uu1,a1j2,uu1xxxxx2)
 getDuDxxxxy2(uu1,a1j2,uu1xxxxy2)
 getDuDxxxyy2(uu1,a1j2,uu1xxxyy2)
 getDuDxxyyy2(uu1,a1j2,uu1xxyyy2)
 getDuDxyyyy2(uu1,a1j2,uu1xyyyy2)
 getDuDyyyyy2(uu1,a1j2,uu1yyyyy2)

 getDuDxxxxx2(vv1,a1j2,vv1xxxxx2)
 getDuDxxxxy2(vv1,a1j2,vv1xxxxy2)
 getDuDxxxyy2(vv1,a1j2,vv1xxxyy2)
 getDuDxxyyy2(vv1,a1j2,vv1xxyyy2)
 getDuDxyyyy2(vv1,a1j2,vv1xyyyy2)
 getDuDyyyyy2(vv1,a1j2,vv1yyyyy2)

 getDuDxxxxx2(uu2,a2j2,uu2xxxxx2)
 getDuDxxxxy2(uu2,a2j2,uu2xxxxy2)
 getDuDxxxyy2(uu2,a2j2,uu2xxxyy2)
 getDuDxxyyy2(uu2,a2j2,uu2xxyyy2)
 getDuDxyyyy2(uu2,a2j2,uu2xyyyy2)
 getDuDyyyyy2(uu2,a2j2,uu2yyyyy2)

 getDuDxxxxx2(vv2,a2j2,vv2xxxxx2)
 getDuDxxxxy2(vv2,a2j2,vv2xxxxy2)
 getDuDxxxyy2(vv2,a2j2,vv2xxxyy2)
 getDuDxxyyy2(vv2,a2j2,vv2xxyyy2)
 getDuDxyyyy2(vv2,a2j2,vv2xyyyy2)
 getDuDyyyyy2(vv2,a2j2,vv2yyyyy2)

 ! 6th derivatives, 2nd order
 getDuDxxxxxx2(uu1,a1j2,uu1xxxxxx2)
 getDuDxxxxyy2(uu1,a1j2,uu1xxxxyy2)
 getDuDxxyyyy2(uu1,a1j2,uu1xxyyyy2)
 getDuDyyyyyy2(uu1,a1j2,uu1yyyyyy2)

 getDuDxxxxxx2(vv1,a1j2,vv1xxxxxx2)
 getDuDxxxxyy2(vv1,a1j2,vv1xxxxyy2)
 getDuDxxyyyy2(vv1,a1j2,vv1xxyyyy2)
 getDuDyyyyyy2(vv1,a1j2,vv1yyyyyy2)

 getDuDxxxxxx2(uu2,a2j2,uu2xxxxxx2)
 getDuDxxxxyy2(uu2,a2j2,uu2xxxxyy2)
 getDuDxxyyyy2(uu2,a2j2,uu2xxyyyy2)
 getDuDyyyyyy2(uu2,a2j2,uu2yyyyyy2)

 getDuDxxxxxx2(vv2,a2j2,vv2xxxxxx2)
 getDuDxxxxyy2(vv2,a2j2,vv2xxxxyy2)
 getDuDxxyyyy2(vv2,a2j2,vv2xxyyyy2)
 getDuDyyyyyy2(vv2,a2j2,vv2yyyyyy2)

 ulapCubed1=uu1xxxxxx2+3.*(uu1xxxxyy2+uu1xxyyyy2)+uu1yyyyyy2
 vlapCubed1=vv1xxxxxx2+3.*(vv1xxxxyy2+vv1xxyyyy2)+vv1yyyyyy2

 ulapCubed2=uu2xxxxxx2+3.*(uu2xxxxyy2+uu2xxyyyy2)+uu2yyyyyy2
 vlapCubed2=vv2xxxxxx2+3.*(vv2xxxxyy2+vv2xxyyyy2)+vv2yyyyyy2


 ! first evaluate the equations we want to solve with the wrong values at the ghost points:
 f(0)=(uu1x6+vv1y6) - \
      (uu2x6+vv2y6)

 f(1)=(an1*ulap1+an2*vlap1) - \
      (an1*ulap2+an2*vlap2)

 f(2)=(vv1x6-uu1y6) - \
      (vv2x6-uu2y6)
 
 f(3)=(tau1*ulap1+tau2*vlap1)/eps1 - \
      (tau1*ulap2+tau2*vlap2)/eps2

 ! These next we can do to 4th order 
 f(4)=(uu1xxx4+uu1xyy4 + vv1xxy4+vv1yyy4) - \
      (uu2xxx4+uu2xyy4 + vv2xxy4+vv2yyy4)

 f(5)=(an1*ulapSq1 + an2*vlapSq1)/eps1 - \
      (an1*ulapSq2 + an2*vlapSq2)/eps2

 f(6)=((vv1xxx4+vv1xyy4)-(uu1xxy4+uu1yyy4))/eps1 - \
      ((vv2xxx4+vv2xyy4)-(uu2xxy4+uu2yyy4))/eps2

 f(7)=(tau1*ulapSq1 + tau2*vlapSq1)/eps1**2 - \
      (tau1*ulapSq2 + tau2*vlapSq2)/eps2**2
 

 ! These last we do to 2nd order
 f(8)=((uu1xxxxx2+2.*uu1xxxyy2+uu1xyyyy2) +(vv1xxxxy2+2.*vv1xxyyy2+vv1yyyyy2)) - \
      ((uu2xxxxx2+2.*uu2xxxyy2+uu2xyyyy2) +(vv2xxxxy2+2.*vv2xxyyy2+vv2yyyyy2))

 f(9) =(an1*ulapCubed1+an2*vlapCubed1)/eps1**2 - \
       (an1*ulapCubed2+an2*vlapCubed2)/eps2**2

 f(10)=((vv1xxxxx2+2.*vv1xxxyy2+vv1xyyyy2)-(uu1xxxxy2+2.*uu1xxyyy2+uu1yyyyy2))/eps1**2 - \
       ((vv2xxxxx2+2.*vv2xxxyy2+vv2xyyyy2)-(uu2xxxxy2+2.*uu2xxyyy2+uu2yyyyy2))/eps2**2

 f(11)=(tau1*ulapCubed1+tau2*vlapCubed1)/eps1**3 - \
       (tau1*ulapCubed2+tau2*vlapCubed2)/eps2**3

#endMacro


#beginMacro computeErrors()
  ! compute the maximum change in the solution for this iteration
  do n=0,11
    err=max(err,abs(q(n)-f(n)))
  end do
  
  do n=0,11,3
    err1=max(err1,abs(q(n)-f(n)))  ! error on ghost line 1
  end do
  do n=1,11,3
    err2=max(err2,abs(q(n)-f(n)))  ! error on ghost line 2
  end do
  do n=2,11,3
    err3=max(err3,abs(q(n)-f(n)))  ! error on ghost line 3
  end do

 if( debug.gt.0 )then ! re-evaluate

    ! evaluate the equations we want to solve using the current solution and assign f(i)
  option=0
  call interfaceDerivatives( nd, nd1a,nd1b,nd2a,nd2b,nd3a, \
     nd3b,gridIndexRange1, u1, mask1,rsxy1, xy1, boundaryCondition1, \
     md1a,md1b,md2a,md2b,md3a,md3b,gridIndexRange2, u2, mask2, \
     rsxy2, xy2, boundaryCondition2, ipar, rpar, option, i1,i2,i3, j1,j2,j3, f, c1x6,c1y6,  \
     c1xx6,c1xy6,c1yy6, c1Lap6,c2x6,c2y6, c2xx6,c2xy6,c2yy6, c2Lap6, \
     c1xLap4,c1yLap4,c1LapSq4, c1xLapSq2,c1yLapSq2,c1LapCubed2, \
     c2xLap4,c2yLap4,c2LapSq4, c2xLapSq2,c2yLapSq2,c2LapCubed2,\
     c1xxx4,c1xxy4,c1xyy4,c1yyy4,c1xxxxy2,c1xxyyy2,c1yyyyy2, c1xxxyy2,c1xyyyy2,\
     c2xxx4,c2xxy4,c2xyy4,c2yyy4,c2xxxxy2,c2xxyyy2,c2yyyyy2, c2xxxyy2,c2xyyyy2,\
     ierr )
           
   ! do n=0,11
   !   f(n)=f(n)*scale(n)
   ! end do
  write(*,'(" --> 6c: i1,i2=",2i4," f(re-eval)=",12e9.1)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),\
      f(6),f(7),f(8),f(9),f(10),f(11)

 end if 
#endMacro


      subroutine newInterfaceMaxwell( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                               gridIndexRange1, u1, mask1,rsxy1, xy1, boundaryCondition1, \
                               md1a,md1b,md2a,md2b,md3a,md3b,\
                               gridIndexRange2, u2, mask2,rsxy2, xy2, boundaryCondition2, \
                               ipar, rpar, ierr )
! ===================================================================================
!  Interface boundary conditions for Maxwell's Equations.
!
!  gridType : 0=rectangular, 1=curvilinear
!
!  u1: solution on the "left" of the interface
!  u2: solution on the "right" of the interface
!
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

!     --- local variables ----
      
      integer side1,axis1,grid1,side2,axis2,grid2,gridType,orderOfAccuracy,orderOfExtrapolation,useForcing,\
        ex,ey,ez,hx,hy,hz,useWhereMask,debug,solveForE,solveForH,axis1p1,axis2p1
      real dx1(0:2),dr1(0:2),dx2(0:2),dr2(0:2)
      real dx(0:2),dr(0:2)
      real t,ep,dt,eps1,mu1,c1,eps2,mu2,c2
      integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,j1,j2,j3,js1,js2,js3,ks1,ks2,ks3,is,js,it,nit,myid

      integer numGhost,giveDiv
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
      integer mm1a,mm1b,mm2a,mm2b,mm3a,mm3b

      real rx1,ry1,rx2,ry2

      real aLap0,aLap1,bLap0,bLap1,aLapX0,aLapX1,bLapY0,bLapY1,cLapX0,cLapX1,dLapY0,dLapY1,aLapSq0,aLapSq1,bLapSq0,bLapSq1
      real a0,a1,b0,b1,cc0,cc1,d0,d1,dr0,ds0
      real aNormSq,divu

      real epsRatio,an1,an2,aNorm,ua,ub,nDotU
      real epsx

      real tau1,tau2,clap1,clap2,ulap1,vlap1,wlap1,ulap2,vlap2,wlap2,an1Cartesian,an2Cartesian
      real ulapSq1,vlapSq1,ulapSq2,vlapSq2,wlapSq1,wlapSq2
      real ulapCubed1,vlapCubed1,ulapCubed2,vlapCubed2,wlapCubed1,wlapCubed2

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
      real a2(0:1,0:1),a4(0:3,0:3),a8(0:7,0:7),aa(0:11,0:11),q(0:11),f(0:11),ipvt(0:11),rcond,work(0:11)
      real scale(0:11)
      real uj(-3:103,0:11) 

      ! boundary conditions parameters
      #Include "bcDefineFortranInclude.h"
 
      integer rectangular,curvilinear
      parameter(\
        rectangular=0,\
        curvilinear=1)

      include 'declareTemporaryVariablesOrder6.h'
!!  declareTemporaryVariables(DIM,MAXDERIV)
!      declareTemporaryVariables(2,8)
!
!! declareParametricDerivativeVariables(v,DIM)
!      declareParametricDerivativeVariables(uu1,2)
!      declareParametricDerivativeVariables(vv1,2)
!      declareParametricDerivativeVariables(ww1,2)
!      declareJacobianDerivativeVariables(a1j2,2)
!      declareJacobianDerivativeVariables(a1j4,2)
!      declareJacobianDerivativeVariables(a1j6,2)
!     
!      declareParametricDerivativeVariables(uu2,2)
!      declareParametricDerivativeVariables(vv2,2)
!      declareParametricDerivativeVariables(ww2,2)
!      declareJacobianDerivativeVariables(a2j2,2)
!      declareJacobianDerivativeVariables(a2j4,2)
!      declareJacobianDerivativeVariables(a2j6,2)

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
      real c1x6(3),c1y6(3), c1xx6(3),c1xy6(3),c1yy6(3), c1Lap6(3)
      real c2x6(3),c2y6(3), c2xx6(3),c2xy6(3),c2yy6(3), c2Lap6(3)

      real c1xLap4(3),c1yLap4(3),c1LapSq4(3), c1xLapSq2(3),c1yLapSq2(3),c1LapCubed2(3)
      real c2xLap4(3),c2yLap4(3),c2LapSq4(3), c2xLapSq2(3),c2yLapSq2(3),c2LapCubed2(3)

      ! for fixup:
      real c1xxx4(3),c1xxy4(3),c1xyy4(3),c1yyy4(3),c1xxxxy2(3),c1xxyyy2(3),c1yyyyy2(3), c1xxxyy2(3),c1xyyyy2(3)
      real c2xxx4(3),c2xxy4(3),c2xyy4(3),c2yyy4(3),c2xxxxy2(3),c2xxyyy2(3),c2yyyyy2(3), c2xxxyy2(3),c2xyyyy2(3)


      real err,err1,err2,err3,omega
      integer ne,interfaceOption,option
      integer useJacobi

      real dx141,dx142,dx112,dx122
      real dx241,dx242,dx212,dx222

!     --- start statement function ----
      integer kd,m,n
!     real rx,ry,rz,sx,sy,sz,tx,ty,tz
!      declareDifferenceNewOrder2(u1,rsxy1,dr1,dx1,RX)
!      declareDifferenceNewOrder2(u2,rsxy2,dr2,dx2,RX)

!      declareDifferenceNewOrder4(u1,rsxy1,dr1,dx1,RX)
!      declareDifferenceNewOrder4(u2,rsxy2,dr2,dx2,RX)

!.......statement functions for jacobian
!     The next macro call will define the difference approximation statement functions
!      defineDifferenceNewOrder2Components1(u1,rsxy1,dr1,dx1,RX)
!      defineDifferenceNewOrder2Components1(u2,rsxy2,dr2,dx2,RX)c

!      defineDifferenceNewOrder4Components1(u1,rsxy1,dr1,dx1,RX)
!      defineDifferenceNewOrder4Components1(u2,rsxy2,dr2,dx2,RX)

!      u1LapSq2(i1,i2,i3,n)=u1xxxx2(i1,i2,i3,n)+2.*u1xxyy2(i1,i2,i3,n)+u1yyyy2(i1,i2,i3,n)
!      u2LapSq2(i1,i2,i3,n)=u2xxxx2(i1,i2,i3,n)+2.*u2xxyy2(i1,i2,i3,n)+u2yyyy2(i1,i2,i3,n)


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
      interfaceOption      =ipar(33)

      myid                 =ipar(35)
     
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
     
      useJacobi=0

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
         ! '
        write(*,'("m1a,m1b,...=",6i5)') m1a,m1b,m2a,m2b,m3a,m3b
         ! '

      ! write(*,*) 'u1=',((((u1(i1,i2,i3,m),m=0,2),i1=n1a,n1b),i2=n2a,n2b),i3=n3a,n3b)
      ! write(*,*) 'u2=',((((u2(i1,i2,i3,m),m=0,2),i1=m1a,m1b),i2=m2a,m2b),i3=m3a,m3b)

      end if
     
      ! *** do this for now --- assume grids have equal spacing
      dx(0)=dx1(0)
      dx(1)=dx1(1)
      dx(2)=dx1(2)

      dr(0)=dr1(0)
      dr(1)=dr1(1)
      dr(2)=dr1(2)

      epsx=1.e-20  ! fix this 


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

  
       if( orderOfAccuracy.eq.6 .and. gridType.eq.rectangular )then
  
         stop 1143

!*         ! --------------- 6th Order Rectangular ---------------
!*         ! ---- first satisfy the jump conditions on the boundary --------
!*         !    [ eps n.u ] = 0
!*         !    [ tau.u ] = 0
!*         boundaryJumpConditions(2,rectangular)
!*
!*         ! here are the real jump conditions for the ghost points
!*         ! 0  [ u.x + v.y ] = 0
!*         ! 1  [ u.xx + u.yy ] = 0
!*         ! 2  [ v.x - u.y ] =0 
!*         ! 3  [ (v.xx+v.yy)/eps ] = 0
!*         ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0  OR [ (u.xx).x + (v.xx).y ] = 0 OR  [ (u.yy).x + (v.yy).y ] = 0 
!*         ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0
!*         ! 6  [ Delta^2 u/eps ] = 0
!*         ! 7  [ Delta^2 v/eps^2 ] = 0 
!*
!*
!*         ! initialization step: assign first ghost line by extrapolation
!*         ! NOTE: assign ghost points outside the ends
!*         beginGhostLoops2d()
!*           u1(i1-is1,i2-is2,i3,ex)=extrap4(u1,i1,i2,i3,ex,is1,is2,is3)
!*           u1(i1-is1,i2-is2,i3,ey)=extrap4(u1,i1,i2,i3,ey,is1,is2,is3)
!*           u1(i1-is1,i2-is2,i3,hz)=extrap4(u1,i1,i2,i3,hz,is1,is2,is3)
!*
!*           u2(j1-js1,j2-js2,j3,ex)=extrap4(u2,j1,j2,j3,ex,js1,js2,js3)
!*           u2(j1-js1,j2-js2,j3,ey)=extrap4(u2,j1,j2,j3,ey,js1,js2,js3)
!*           u2(j1-js1,j2-js2,j3,hz)=extrap4(u2,j1,j2,j3,hz,js1,js2,js3)
!*
!*           ! --- also extrap 2nd line for now
!*           ! u1(i1-2*is1,i2-2*is2,i3,ex)=extrap4(u1,i1-is1,i2-is2,i3,ex,is1,is2,is3)
!*           ! u1(i1-2*is1,i2-2*is2,i3,ey)=extrap4(u1,i1-is1,i2-is2,i3,ey,is1,is2,is3)
!*           ! u1(i1-2*is1,i2-2*is2,i3,hz)=extrap4(u1,i1-is1,i2-is2,i3,hz,is1,is2,is3)
!*
!*           ! u2(j1-2*js1,j2-2*js2,j3,ex)=extrap4(u2,j1-js1,j2-js2,j3,ex,js1,js2,js3)
!*           ! u2(j1-2*js1,j2-2*js2,j3,ey)=extrap4(u2,j1-js1,j2-js2,j3,ey,js1,js2,js3)
!*           ! u2(j1-2*js1,j2-2*js2,j3,hz)=extrap4(u2,j1-js1,j2-js2,j3,hz,js1,js2,js3)
!*         endLoops2d()
!*
!*         beginLoops2d() ! =============== start loops =======================
!*
!*           ! first evaluate the equations we want to solve with the wrong values at the ghost points:
!*           f(0)=(u1x4(i1,i2,i3,ex)+u1y4(i1,i2,i3,ey)) - \
!*                (u2x4(j1,j2,j3,ex)+u2y4(j1,j2,j3,ey))
!*
!*           f(1)=(u1xx4(i1,i2,i3,ex)+u1yy4(i1,i2,i3,ex)) - \
!*                (u2xx4(j1,j2,j3,ex)+u2yy4(j1,j2,j3,ex))
!*
!*           f(2)=(u1x4(i1,i2,i3,ey)-u1y4(i1,i2,i3,ex)) - \
!*                (u2x4(j1,j2,j3,ey)-u2y4(j1,j2,j3,ex))
!*           
!*           f(3)=(u1xx4(i1,i2,i3,ey)+u1yy4(i1,i2,i3,ey))/eps1 - \
!*                (u2xx4(j1,j2,j3,ey)+u2yy4(j1,j2,j3,ey))/eps2
!*    
!*           ! These next we can do to 2nd order -- these need a value on the first ghost line --
!*           f(4)=(u1xxx2(i1,i2,i3,ex)+u1xyy2(i1,i2,i3,ex)+u1xxy2(i1,i2,i3,ey)+u1yyy2(i1,i2,i3,ey)) - \
!*                (u2xxx2(j1,j2,j3,ex)+u2xyy2(j1,j2,j3,ex)+u2xxy2(j1,j2,j3,ey)+u2yyy2(j1,j2,j3,ey))
!*
!*           f(5)=((u1xxx2(i1,i2,i3,ey)+u1xyy2(i1,i2,i3,ey))-(u1xxy2(i1,i2,i3,ex)+u1yyy2(i1,i2,i3,ex)))/eps1 - \
!*                ((u2xxx2(j1,j2,j3,ey)+u2xyy2(j1,j2,j3,ey))-(u2xxy2(j1,j2,j3,ex)+u2yyy2(j1,j2,j3,ex)))/eps2
!*
!*           f(6)=(u1LapSq2(i1,i2,i3,ex))/eps1 - \
!*                (u2LapSq2(j1,j2,j3,ex))/eps2
!*
!*           f(7)=(u1LapSq2(i1,i2,i3,ey))/eps1**2 - \
!*                (u2LapSq2(j1,j2,j3,ey))/eps2**2
!*           
!*       write(*,'(" --> 6th: j1,j2=",2i4," u1xx,u1yy,u2xx,u2yy=",4e10.2)') j1,j2,u1xx4(i1,i2,i3,ex),\
!*           u1yy4(i1,i2,i3,ex),u2xx4(j1,j2,j3,ex),u2yy4(j1,j2,j3,ex)
!*       write(*,'(" --> 6th: i1,i2=",2i4," f(start)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)
!*
!*           ! here is the matrix of coefficients for the unknowns u1(-1),v1(-1),u2(-1),v2(-1)
!*           ! Solve:
!*           !     
!*           !       A [ U ] = A [ U(old) ] - [ f ]
!*c      u1x43r(i1,i2,i3,kd)=(8.*(u1(i1+1,i2,i3,kd)-u1(i1-1,i2,i3,kd))-(
!*c     & u1(i1+2,i2,i3,kd)-u1(i1-2,i2,i3,kd)))*dx141(0)
!*
!*
!*           ! 0  [ u.x + v.y ] = 0
!*           a8(0,0) = -is*8.*rx1*dx141(axis1)     ! coeff of u1(-1) from [u.x+v.y] 
!*           a8(0,1) = -is*8.*ry1*dx141(axis1)     ! coeff of v1(-1) from [u.x+v.y] 
!*           a8(0,4) =  is*rx1*dx141(axis1)        ! u1(-2)
!*           a8(0,5) =  is*ry1*dx141(axis1)        ! v1(-2) 
!*
!*           a8(0,2) =  js*8.*rx2*dx241(axis2)     ! coeff of u2(-1) from [u.x+v.y] 
!*           a8(0,3) =  js*8.*ry2*dx241(axis2) 
!*           a8(0,6) = -js*   rx2*dx241(axis2) 
!*           a8(0,7) = -js*   ry2*dx241(axis2) 
!*
!*           ! 1  [ u.xx + u.yy ] = 0
!*c      u1xx43r(i1,i2,i3,kd)=( -30.*u1(i1,i2,i3,kd)+16.*(u1(i1+1,i2,i3,
!*c     & kd)+u1(i1-1,i2,i3,kd))-(u1(i1+2,i2,i3,kd)+u1(i1-2,i2,i3,kd)) )*
!*c     & dx142(0)
!*           
!*           a8(1,0) = 16.*dx142(axis1)         ! coeff of u1(-1) from [u.xx + u.yy]
!*           a8(1,1) = 0. 
!*           a8(1,4) =    -dx142(axis1)         ! coeff of u1(-2) from [u.xx + u.yy]
!*           a8(1,5) = 0. 
!*
!*           a8(1,2) =-16.*dx242(axis2)         ! coeff of u2(-1) from [u.xx + u.yy]
!*           a8(1,3) = 0. 
!*           a8(1,6) =     dx242(axis2)         ! coeff of u2(-2) from [u.xx + u.yy]
!*           a8(1,7) = 0. 
!*
!*
!*           ! 2  [ v.x - u.y ] =0 
!*           a8(2,0) =  is*8.*ry1*dx141(axis1)
!*           a8(2,1) = -is*8.*rx1*dx141(axis1)    ! coeff of v1(-1) from [v.x - u.y] 
!*           a8(2,4) = -is*   ry1*dx141(axis1)
!*           a8(2,5) =  is*   rx1*dx141(axis1)
!*
!*           a8(2,2) = -js*8.*ry2*dx241(axis2)
!*           a8(2,3) =  js*8.*rx2*dx241(axis2)
!*           a8(2,6) =  js*   ry2*dx241(axis2)
!*           a8(2,7) = -js*   rx2*dx241(axis2)
!*
!*           ! 3  [ (v.xx+v.yy)/eps ] = 0
!*           a8(3,0) = 0.                      
!*           a8(3,1) = 16.*dx142(axis1)/eps1 ! coeff of v1(-1) from [(v.xx+v.yy)/eps]
!*           a8(3,4) = 0.                      
!*           a8(3,5) =    -dx142(axis1)/eps1 ! coeff of v1(-2) from [(v.xx+v.yy)/eps]
!*
!*           a8(3,2) = 0. 
!*           a8(3,3) =-16.*dx242(axis2)/eps2 ! coeff of v2(-1) from [(v.xx+v.yy)/eps]
!*           a8(3,6) = 0. 
!*           a8(3,7) =     dx242(axis2)/eps2 ! coeff of v2(-2) from [(v.xx+v.yy)/eps]
!*
!*           ! 4  [ (u.xx+u.yy).x + (v.xx+v.yy).y ] = 0
!*c     u1xxx2(i1,i2,i3,kd)=(-2.*(u1(i1+1,i2,i3,kd)-u1(i1-1,i2,i3,kd))+
!*c    & (u1(i1+2,i2,i3,kd)-u1(i1-2,i2,i3,kd)) )*dx122(0)*dx112(0)
!*c    u1xxy2(i1,i2,i3,kd)=( u1xx2(i1,i2+1,i3,kd)-u1xx2(i1,i2-1,
!*c     & i3,kd))/(2.*dx1(1))
!*c      u1yy23r(i1,i2,i3,kd)=(-2.*u1(i1,i2,i3,kd)+(u1(i1,i2+1,i3,kd)+u1(
!*c     & i1,i2-1,i3,kd)) )*dx122(1)
!*c     u1xyy2(i1,i2,i3,kd)=( u1yy2(i1+1,i2,i3,kd)-u1yy2(i1-1,i2,
!*c     & i3,kd))/(2.*dx1(0))
!*          a8(4,0)= ( is*rx1*2.*dx122(axis1)*dx112(axis1)+is*rx1*2.*dx122(1)/(2.*dx1(0)))
!*          a8(4,1)= ( is*ry1*2.*dx122(axis1)*dx112(axis1)+is*ry1*2.*dx122(0)/(2.*dx1(1)))
!*          a8(4,4)= (-is*rx1   *dx122(axis1)*dx112(axis1) )  
!*          a8(4,5)= (-is*ry1   *dx122(axis1)*dx112(axis1))
!*
!*          a8(4,2)=-( js*rx2*2.*dx222(axis2)*dx212(axis2)+js*rx2*2.*dx222(1)/(2.*dx2(0)))
!*          a8(4,3)=-( js*ry2*2.*dx222(axis2)*dx212(axis2)+js*ry2*2.*dx222(0)/(2.*dx2(1)))
!*          a8(4,6)=-(-js*rx2   *dx222(axis2)*dx212(axis2))   
!*          a8(4,7)=-(-js*ry2   *dx222(axis2)*dx212(axis2))
!*
!*          ! 5  [ {(Delta v).x - (Delta u).y}/eps ] =0  -> [ {(v.xxx+v.xyy)-(u.xxy+u.yyy)}/eps ] = 0
!*
!*          a8(5,0)=-( is*ry1*2.*dx122(axis1)*dx112(axis1)+is*ry1*2.*dx122(0)/(2.*dx1(1)))/eps1
!*          a8(5,1)= ( is*rx1*2.*dx122(axis1)*dx112(axis1)+is*rx1*2.*dx122(1)/(2.*dx1(0)))/eps1
!*          a8(5,4)=-(-is*ry1   *dx122(axis1)*dx112(axis1))/eps1
!*          a8(5,5)= (-is*rx1   *dx122(axis1)*dx112(axis1))/eps1   
!*
!*          a8(5,2)= ( js*ry2*2.*dx222(axis2)*dx212(axis2)+js*ry2*2.*dx222(0)/(2.*dx2(1)))/eps2
!*          a8(5,3)=-( js*rx2*2.*dx222(axis2)*dx212(axis2)+js*rx2*2.*dx222(1)/(2.*dx2(0)))/eps2
!*          a8(5,6)= (-js*ry2   *dx222(axis2)*dx212(axis2))/eps2
!*          a8(5,7)=-(-js*rx2   *dx222(axis2)*dx212(axis2))/eps2   
!*
!*           ! 6  [ Delta^2 u/eps ] = 0
!*c     u1LapSq2(i1,i2,i3,kd)= ( 6.*u1(i1,i2,i3,kd)- 4.*(u1(i1+1,i2,i3,
!*c    & kd)+u1(i1-1,i2,i3,kd))+(u1(i1+2,i2,i3,kd)+u1(i1-2,i2,i3,kd)) )
!*c    & /(dx1(0)**4)+( 6.*u1(i1,i2,i3,kd)-4.*(u1(i1,i2+1,i3,kd)+u1(i1,
!*c    & i2-1,i3,kd)) +(u1(i1,i2+2,i3,kd)+u1(i1,i2-2,i3,kd)) )/(dx1(1)**
!*c    & 4)+( 8.*u1(i1,i2,i3,kd)-4.*(u1(i1+1,i2,i3,kd)+u1(i1-1,i2,i3,kd)
!*c    & +u1(i1,i2+1,i3,kd)+u1(i1,i2-1,i3,kd))+2.*(u1(i1+1,i2+1,i3,kd)+
!*c    & u1(i1-1,i2+1,i3,kd)+u1(i1+1,i2-1,i3,kd)+u1(i1-1,i2-1,i3,kd)) )
!*c    & /(dx1(0)**2*dx1(1)**2)
!*
!*           a8(6,0) = -(4./(dx1(axis1)**4) +4./(dx1(0)**2*dx1(1)**2) )/eps1
!*           a8(6,1) = 0.
!*           a8(6,4) =   1./(dx1(axis1)**4)/eps1
!*           a8(6,5) = 0.
!*
!*           a8(6,2) = (4./(dx2(axis2)**4) +4./(dx1(0)**2*dx1(1)**2) )/eps2
!*           a8(6,3) = 0.
!*           a8(6,6) =  -1./(dx2(axis2)**4)/eps2
!*           a8(6,7) = 0.
!*
!*           ! 7  [ Delta^2 v/eps^2 ] = 0 
!*           a8(7,0) = 0.
!*           a8(7,1) = -(4./(dx1(axis1)**4) +4./(dx2(0)**2*dx2(1)**2) )/eps1**2
!*           a8(7,4) = 0.
!*           a8(7,5) =   1./(dx1(axis1)**4)/eps1**2
!*
!*           a8(7,2) = 0.
!*           a8(7,3) =  (4./(dx2(axis2)**4) +4./(dx2(0)**2*dx2(1)**2) )/eps2**2
!*           a8(7,6) = 0.
!*           a8(7,7) =  -1./(dx2(axis2)**4)/eps2**2
!*
!*           q(0) = u1(i1-is1,i2-is2,i3,ex)
!*           q(1) = u1(i1-is1,i2-is2,i3,ey)
!*           q(2) = u2(j1-js1,j2-js2,j3,ex)
!*           q(3) = u2(j1-js1,j2-js2,j3,ey)
!*
!*           q(4) = u1(i1-2*is1,i2-2*is2,i3,ex)
!*           q(5) = u1(i1-2*is1,i2-2*is2,i3,ey)
!*           q(6) = u2(j1-2*js1,j2-2*js2,j3,ex)
!*           q(7) = u2(j1-2*js1,j2-2*js2,j3,ey)
!*
!*       write(*,'(" --> 6th: i1,i2=",2i4," q=",8e10.2)') i1,i2,q(0),q(1),q(2),q(3),q(4),q(5),q(6),q(7)
!*
!*           ! subtract off the contributions from the initial (wrong) values at the ghost points:
!*           do n=0,7
!*             f(n) = (a8(n,0)*q(0)+a8(n,1)*q(1)+a8(n,2)*q(2)+a8(n,3)*q(3)+\
!*                     a8(n,4)*q(4)+a8(n,5)*q(5)+a8(n,6)*q(6)+a8(n,7)*q(7)) - f(n)
!*           end do
!*
!*           ! solve A Q = F
!*           ! factor the matrix
!*           numberOfEquations=8
!*           call dgeco( a8(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0))
!*           ! solve
!*       write(*,'(" --> 6th: i1,i2=",2i4," rcond=",e10.2)') i1,i2,rcond
!*           job=0
!*           call dgesl( a8(0,0), numberOfEquations, numberOfEquations, ipvt(0), f(0), job)
!*
!*       write(*,'(" --> 6th: i1,i2=",2i4," f(solve)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)
!*
!*           if( .true. )then
!*           u1(i1-is1,i2-is2,i3,ex)=f(0)
!*           u1(i1-is1,i2-is2,i3,ey)=f(1)
!*           u2(j1-js1,j2-js2,j3,ex)=f(2)
!*           u2(j1-js1,j2-js2,j3,ey)=f(3)
!*
!*           u1(i1-2*is1,i2-2*is2,i3,ex)=f(4)
!*           u1(i1-2*is1,i2-2*is2,i3,ey)=f(5)
!*           u2(j1-2*js1,j2-2*js2,j3,ex)=f(6)
!*           u2(j1-2*js1,j2-2*js2,j3,ey)=f(7)
!*           end if
!*
!*          if( debug.gt.0 )then ! re-evaluate
!*           f(0)=(u1x4(i1,i2,i3,ex)+u1y4(i1,i2,i3,ey)) - \
!*                (u2x4(j1,j2,j3,ex)+u2y4(j1,j2,j3,ey))
!*           f(1)=(u1xx4(i1,i2,i3,ex)+u1yy4(i1,i2,i3,ex)) - \
!*                (u2xx4(j1,j2,j3,ex)+u2yy4(j1,j2,j3,ex))
!*
!*           f(2)=(u1x4(i1,i2,i3,ey)-u1y4(i1,i2,i3,ex)) - \
!*                (u2x4(j1,j2,j3,ey)-u2y4(j1,j2,j3,ex))
!*           
!*           f(3)=(u1xx4(i1,i2,i3,ey)+u1yy4(i1,i2,i3,ey))/eps1 - \
!*                (u2xx4(j1,j2,j3,ey)+u2yy4(j1,j2,j3,ey))/eps2
!*    
!*           ! These next we can do to 2nd order -- these need a value on the first ghost line --
!*           f(4)=(u1xxx2(i1,i2,i3,ex)+u1xyy2(i1,i2,i3,ex)+u1xxy2(i1,i2,i3,ey)+u1yyy2(i1,i2,i3,ey)) - \
!*                (u2xxx2(j1,j2,j3,ex)+u2xyy2(j1,j2,j3,ex)+u2xxy2(j1,j2,j3,ey)+u2yyy2(j1,j2,j3,ey))
!*
!*           f(5)=((u1xxx2(i1,i2,i3,ey)+u1xyy2(i1,i2,i3,ey))-(u1xxy2(i1,i2,i3,ex)+u1yyy2(i1,i2,i3,ex)))/eps1 - \
!*                ((u2xxx2(j1,j2,j3,ey)+u2xyy2(j1,j2,j3,ey))-(u2xxy2(j1,j2,j3,ex)+u2yyy2(j1,j2,j3,ex)))/eps2
!*
!*           f(6)=(u1LapSq2(i1,i2,i3,ex))/eps1 - \
!*                (u2LapSq2(j1,j2,j3,ex))/eps2
!*
!*           f(7)=(u1LapSq2(i1,i2,i3,ey))/eps1**2 - \
!*                (u2LapSq2(j1,j2,j3,ey))/eps2**2
!*    
!*           write(*,'(" --> 6th: i1,i2=",2i4," f(re-eval)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)
!*          end if
!*
!*           ! do this for now
!*           u1(i1-is1,i2-is2,i3,hz)=u2(j1+js1,j2+js2,j3,hz) 
!*           u2(j1-js1,j2-js2,j3,hz)=u1(i1+is1,i2+is2,i3,hz)
!*
!*           u1(i1-2*is1,i2-2*is2,i3,hz)=u2(j1+2*js1,j2+2*js2,j3,hz) 
!*           u2(j1-2*js1,j2-2*js2,j3,hz)=u1(i1+2*is1,i2+2*is2,i3,hz)
!*
!*         endLoops2d()
!*
!*         ! periodic update
!*         periodicUpdate2d(u1,boundaryCondition1,gridIndexRange1,side1,axis1)
!*         periodicUpdate2d(u2,boundaryCondition2,gridIndexRange2,side2,axis2)

       else if( orderOfAccuracy.eq.6 .and. gridType.eq.curvilinear )then
  
         ! --------------- 6th Order Curvilinear ---------------

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
         if( interfaceOption.eq.1 )then
         beginGhostLoops2d()
!
            u1(i1-is1,i2-is2,i3,ex)=extrap6(u1,i1,i2,i3,ex,is1,is2,is3)
            u1(i1-is1,i2-is2,i3,ey)=extrap6(u1,i1,i2,i3,ey,is1,is2,is3)
            u1(i1-is1,i2-is2,i3,hz)=extrap6(u1,i1,i2,i3,hz,is1,is2,is3)
!
            u2(j1-js1,j2-js2,j3,ex)=extrap6(u2,j1,j2,j3,ex,js1,js2,js3)
            u2(j1-js1,j2-js2,j3,ey)=extrap6(u2,j1,j2,j3,ey,js1,js2,js3)
            u2(j1-js1,j2-js2,j3,hz)=extrap6(u2,j1,j2,j3,hz,js1,js2,js3)

           ! --- also extrap 2nd line for now
           if( .true. )then
           u1(i1-2*is1,i2-2*is2,i3,ex)=extrap5(u1,i1-is1,i2-is2,i3,ex,is1,is2,is3)
           u1(i1-2*is1,i2-2*is2,i3,ey)=extrap5(u1,i1-is1,i2-is2,i3,ey,is1,is2,is3)
           u1(i1-2*is1,i2-2*is2,i3,hz)=extrap5(u1,i1-is1,i2-is2,i3,hz,is1,is2,is3)

           u2(j1-2*js1,j2-2*js2,j3,ex)=extrap5(u2,j1-js1,j2-js2,j3,ex,js1,js2,js3)
           u2(j1-2*js1,j2-2*js2,j3,ey)=extrap5(u2,j1-js1,j2-js2,j3,ey,js1,js2,js3)
           u2(j1-2*js1,j2-2*js2,j3,hz)=extrap5(u2,j1-js1,j2-js2,j3,hz,js1,js2,js3)

           ! --- also extrap 3rd line for now
           u1(i1-3*is1,i2-3*is2,i3,ex)=extrap4(u1,i1-2*is1,i2-2*is2,i3,ex,is1,is2,is3)
           u1(i1-3*is1,i2-3*is2,i3,ey)=extrap4(u1,i1-2*is1,i2-2*is2,i3,ey,is1,is2,is3)
           u1(i1-3*is1,i2-3*is2,i3,hz)=extrap4(u1,i1-2*is1,i2-2*is2,i3,hz,is1,is2,is3)

           u2(j1-3*js1,j2-3*js2,j3,ex)=extrap4(u2,j1-2*js1,j2-2*js2,j3,ex,js1,js2,js3)
           u2(j1-3*js1,j2-3*js2,j3,ey)=extrap4(u2,j1-2*js1,j2-2*js2,j3,ey,js1,js2,js3)
           u2(j1-3*js1,j2-3*js2,j3,hz)=extrap4(u2,j1-2*js1,j2-2*js2,j3,hz,js1,js2,js3)
           end if
         endLoops2d()
         end if

         ! omega=.5


         scale(0) =dr1(axis1)
         scale(1) =dr1(axis1)**2
         scale(2) =dr1(axis1)   
         scale(3) =dr1(axis1)**2

         scale(4) =dr1(axis1)**2
         scale(5) =dr1(axis1)**4
         scale(6) =dr1(axis1)**2
         scale(7) =dr1(axis1)**4

         scale(8) =dr1(axis1)**4
         scale(9) =dr1(axis1)**6
         scale(10)=dr1(axis1)**4
         scale(11)=dr1(axis1)**6

         do it=1,nit ! *** begin iteration ****

         err=0.
         err1=0.
         err2=0.
         err3=0.

         ! =============== start loops ======================
         beginLoops2d() 

           ! here is the normal (assumed to be the same on both sides)
           an1=rsxy1(i1,i2,i3,axis1,0)   ! normal (an1,an2)
           an2=rsxy1(i1,i2,i3,axis1,1)
           aNorm=max(epsx,sqrt(an1**2+an2**2))
           an1=an1/aNorm
           an2=an2/aNorm
           tau1=-an2
           tau2= an1

           option=1
           call interfaceDerivatives( nd, nd1a,nd1b,nd2a,nd2b,nd3a, \
      nd3b,gridIndexRange1, u1, mask1,rsxy1, xy1, boundaryCondition1, \
      md1a,md1b,md2a,md2b,md3a,md3b,gridIndexRange2, u2, mask2, \
      rsxy2, xy2, boundaryCondition2, ipar, rpar, option, i1,i2,i3, j1,j2,j3, f, c1x6,c1y6,  \
      c1xx6,c1xy6,c1yy6, c1Lap6,c2x6,c2y6, c2xx6,c2xy6,c2yy6, c2Lap6, \
      c1xLap4,c1yLap4,c1LapSq4, c1xLapSq2,c1yLapSq2,c1LapCubed2, \
      c2xLap4,c2yLap4,c2LapSq4, c2xLapSq2,c2yLapSq2,c2LapCubed2,\
      c1xxx4,c1xxy4,c1xyy4,c1yyy4,c1xxxxy2,c1xxyyy2,c1yyyyy2, c1xxxyy2,c1xyyyy2,\
      c2xxx4,c2xxy4,c2xyy4,c2yyy4,c2xxxxy2,c2xxyyy2,c2yyyyy2, c2xxxyy2,c2xyyyy2,\
      ierr )

           if( debug.gt.0 )then
             write(*,'(" --> 6c: i1,i2=",2i4," f(start)=",12e9.1)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),\
                f(6),f(7),f(8),f(9),f(10),f(11)
           end if
!**      include 'evaluateJacobianDerivativesOrder6.h'
!           evalJacobianDerivatives(rsxy1,i1,i2,i3,a1j6,2,6,1)
!           evalJacobianDerivatives(rsxy1,i1,i2,i3,a1j4,2,4,3)
!           evalJacobianDerivatives(rsxy1,i1,i2,i3,a1j2,2,2,5)

!           evalJacobianDerivatives(rsxy2,j1,j2,j3,a2j6,2,6,1)
!           evalJacobianDerivatives(rsxy2,j1,j2,j3,a2j4,2,4,3)
!           evalJacobianDerivatives(rsxy2,j1,j2,j3,a2j2,2,2,5)


           ! evaluate the equations we want to solve using the current solution and assign f(i)
!**      include 'evaluateEquationsOrder6.h'
!           evaluateEquationsOrder6()


!**      include 'evaluateCoefficientsOrder6.h'

!           ! Get coefficients for material 1
!           dr1a=dr1(0)
!           ds1a=dr1(1)
!           if( axis1.eq.0 )then
!             dr1a=dr1(0)*is
!             ! macro: evalCoeffOrder6(MAT,DIR,dr,ds)
!             evalCoeffOrder6(1,0,dr1a,ds1a)
!           else
!             ds1a=dr1(1)*is
!             evalCoeffOrder6(1,1,dr1a,ds1a)
!           end if
!           
!           ! Get coefficients for material 2
!           dr2a=dr2(0)
!           ds2a=dr2(1)
!           if( axis2.eq.0 )then
!             dr2a=dr2(0)*js
!             ! macro: evalCoeffOrder6(MATERIAL,DIR,dr,ds)
!             evalCoeffOrder6(2,0,dr2a,ds2a)
!           else
!             ds2a=dr2(1)*js
!             evalCoeffOrder6(2,1,dr2a,ds2a)
!           end if

           ! --- equations using sixth order approximations  ----
           ! 0  [ u.x + v.y ] = 0
           ne=0
           aa(ne,0) = c1x6(1)     ! coeff of u1(-1) from [u.x+v.y] 
           aa(ne,1) = c1x6(2)     ! u1(-2)
           aa(ne,2) = c1x6(3)     ! u1(-3)
           aa(ne,3) = c1y6(1)     ! coeff of v1(-1) from [u.x+v.y] 
           aa(ne,4) = c1y6(2)     ! v1(-2) 
           aa(ne,5) = c1y6(3)     ! v1(-3) 

           aa(ne,6) =-c2x6(1)
           aa(ne,7) =-c2x6(2)
           aa(ne,8) =-c2x6(3)
           aa(ne,9) =-c2y6(1)
           aa(ne,10)=-c2y6(2)
           aa(ne,11)=-c2y6(3)

           ! 1  [n.(u.xx + u.yy)] = 0
           ne=ne+1
           aa(ne,0) = an1*c1Lap6(1)
           aa(ne,1) = an1*c1Lap6(2)
           aa(ne,2) = an1*c1Lap6(3)
           aa(ne,3) = an2*c1Lap6(1)
           aa(ne,4) = an2*c1Lap6(2)
           aa(ne,5) = an2*c1Lap6(3)

           aa(ne,6) =-an1*c2Lap6(1)
           aa(ne,7) =-an1*c2Lap6(2)
           aa(ne,8) =-an1*c2Lap6(3)
           aa(ne,9) =-an2*c2Lap6(1)
           aa(ne,10)=-an2*c2Lap6(2)
           aa(ne,11)=-an2*c2Lap6(3)

           ! 2  [ v.x - u.y ] =0 
           ne=ne+1
           aa(ne,0) =-c1y6(1)     ! coeff of u1(-1) 
           aa(ne,1) =-c1y6(2)     ! u1(-2)
           aa(ne,2) =-c1y6(3)     ! u1(-3)
           aa(ne,3) = c1x6(1)     ! coeff of v1(-1) 
           aa(ne,4) = c1x6(2)     ! v1(-2) 
           aa(ne,5) = c1x6(3)     ! v1(-3) 

           aa(ne,6) = c2y6(1)
           aa(ne,7) = c2y6(2)
           aa(ne,8) = c2y6(3)
           aa(ne,9) =-c2x6(1)
           aa(ne,10)=-c2x6(2)
           aa(ne,11)=-c2x6(3)

           ! 3  [tau.(u.xx + u.yy)/eps] = 0
           ne=ne+1
           aa(ne,0) = tau1*c1Lap6(1)/eps1
           aa(ne,1) = tau1*c1Lap6(2)/eps1
           aa(ne,2) = tau1*c1Lap6(3)/eps1
           aa(ne,3) = tau2*c1Lap6(1)/eps1
           aa(ne,4) = tau2*c1Lap6(2)/eps1
           aa(ne,5) = tau2*c1Lap6(3)/eps1

           aa(ne,6) =-tau1*c2Lap6(1)/eps2
           aa(ne,7) =-tau1*c2Lap6(2)/eps2
           aa(ne,8) =-tau1*c2Lap6(3)/eps2
           aa(ne,9) =-tau2*c2Lap6(1)/eps2
           aa(ne,10)=-tau2*c2Lap6(2)/eps2
           aa(ne,11)=-tau2*c2Lap6(3)/eps2


           ! --- equations using 4th order approximations  ----
           ! 4 [div(Lap(uv))]=0
 ! f(4)=( (uu1xxx4         + vv1xxy4        ) - ( tau1a*(uu1xxy4+uu1yyy4)+tau2a*(vv1xxy4+vv1yyy4) ) )/eps1 - \
 !      ( (uu2xxx4         + vv2xxy4        ) - ( tau1a*(uu2xxy4+uu2yyy4)+tau2a*(vv2xxy4+vv2yyy4) ) )/eps2
           ne=ne+1
           aa(ne,0) = (c1xxx4(1)                        )/eps1    ! coeff of u1(-1)
           aa(ne,1) = (c1xxx4(2)                        )/eps1        ! u1(-2)
           aa(ne,2) = (c1xxx4(3)                        )/eps1        ! u1(-3)
           aa(ne,3) =-(c1yyy4(1))/eps1        ! coeff of v1(-1)
           aa(ne,4) =-(c1yyy4(2))/eps1        ! v1(-2) 
           aa(ne,5) =-(c1yyy4(3))/eps1        ! v1(-3) 

           aa(ne,6) =-(c2xxx4(1)                        )/eps2   
           aa(ne,7) =-(c2xxx4(2)                        )/eps2   
           aa(ne,8) =-(c2xxx4(3)                        )/eps2   
           aa(ne,9) = (c2yyy4(1))/eps2   
           aa(ne,10)= (c2yyy4(2))/eps2   
           aa(ne,11)= (c2yyy4(3))/eps2   


 !  f(4)=( (uu1xxx4+uu1xyy4 + vv1xxy4+vv1yyy4) - ( (tau1a*(uu1xxy4+uu1yyy4)+tau2a*(vv1xxy4+vv1yyy4) ) )/eps1 - \
 !       ( (uu2xxx4+uu2xyy4 + vv2xxy4+vv2yyy4) - ( (tau1a*(uu2xxy4+uu2yyy4)+tau2a*(vv2xxy4+vv2yyy4) ) )/eps2
!           ne=ne+1
!           aa(ne,0) = (c1xLap4(1)                        )/eps1    ! coeff of u1(-1)
!           aa(ne,1) = (c1xLap4(2)                        )/eps1        ! u1(-2)
!           aa(ne,2) = (c1xLap4(3)                        )/eps1        ! u1(-3)
!           aa(ne,3) = (c1yLap4(1) - (c1xxy4(1)+c1yyy4(1)))/eps1        ! coeff of v1(-1)
!           aa(ne,4) = (c1yLap4(2) - (c1xxy4(2)+c1yyy4(2)))/eps1        ! v1(-2) 
!           aa(ne,5) = (c1yLap4(3) - (c1xxy4(3)+c1yyy4(3)))/eps1        ! v1(-3) 
!
!           aa(ne,6) =-(c2xLap4(1)                        )/eps2   
!           aa(ne,7) =-(c2xLap4(2)                        )/eps2   
!           aa(ne,8) =-(c2xLap4(3)                        )/eps2   
!           aa(ne,9) =-(c2yLap4(1) - (c2xxy4(1)+c2yyy4(1)))/eps2   
!           aa(ne,10)=-(c2yLap4(2) - (c2xxy4(2)+c2yyy4(2)))/eps2   
!           aa(ne,11)=-(c2yLap4(3) - (c2xxy4(3)+c2yyy4(3)))/eps2   

!           ne=ne+1
!           aa(ne,0) = (c1xLap4(1))/eps1    ! coeff of u1(-1)
!           aa(ne,1) = (c1xLap4(2))/eps1        ! u1(-2)
!           aa(ne,2) = (c1xLap4(3))/eps1        ! u1(-3)
!           aa(ne,3) = (c1yLap4(1))/eps1        ! coeff of v1(-1)
!           aa(ne,4) = (c1yLap4(2))/eps1        ! v1(-2) 
!           aa(ne,5) = (c1yLap4(3))/eps1        ! v1(-3) 
!
!           aa(ne,6) =-(c2xLap4(1))/eps2   
!           aa(ne,7) =-(c2xLap4(2))/eps2   
!           aa(ne,8) =-(c2xLap4(3))/eps2   
!           aa(ne,9) =-(c2yLap4(1))/eps2   
!           aa(ne,10)=-(c2yLap4(2))/eps2   
!           aa(ne,11)=-(c2yLap4(3))/eps2   

           
           ! 5 [n.Lap^2(uv)/eps] = 0
           ne=ne+1
           aa(ne,0) = an1*c1LapSq4(1)/eps1 
           aa(ne,1) = an1*c1LapSq4(2)/eps1 
           aa(ne,2) = an1*c1LapSq4(3)/eps1 
           aa(ne,3) = an2*c1LapSq4(1)/eps1 
           aa(ne,4) = an2*c1LapSq4(2)/eps1 
           aa(ne,5) = an2*c1LapSq4(3)/eps1 

           aa(ne,6) =-an1*c2LapSq4(1)/eps2
           aa(ne,7) =-an1*c2LapSq4(2)/eps2
           aa(ne,8) =-an1*c2LapSq4(3)/eps2
           aa(ne,9) =-an2*c2LapSq4(1)/eps2
           aa(ne,10)=-an2*c2LapSq4(2)/eps2
           aa(ne,11)=-an2*c2LapSq4(3)/eps2

           ! 6 [ Lap(v.x - u.y)/eps ] =0 

 ! f(6)=( ((vv1xxx4+vv1xyy4)-(uu1xxy4+uu1yyy4)) -(uu1xxy4+vv1xyy4) )/eps1 - \
 !      ( ((vv2xxx4+vv2xyy4)-(uu2xxy4+uu2yyy4)) -(uu2xxy4+vv2xyy4) )/eps2

!           ne=ne+1
!           aa(ne,0) =-(c1yLap4(1) +c1xxy4(1))/eps1     ! coeff of u1(-1) 
!           aa(ne,1) =-(c1yLap4(2) +c1xxy4(2))/eps1     ! u1(-2)
!           aa(ne,2) =-(c1yLap4(3) +c1xxy4(3))/eps1     ! u1(-3)
!           aa(ne,3) = (c1xLap4(1) -c1xyy4(1))/eps1     ! coeff of v1(-1) 
!           aa(ne,4) = (c1xLap4(2) -c1xyy4(2))/eps1     ! v1(-2) 
!           aa(ne,5) = (c1xLap4(3) -c1xyy4(3))/eps1     ! v1(-3) 
!
!           aa(ne,6) = (c2yLap4(1) +c2xxy4(1))/eps2
!           aa(ne,7) = (c2yLap4(2) +c2xxy4(2))/eps2
!           aa(ne,8) = (c2yLap4(3) +c2xxy4(3))/eps2
!           aa(ne,9) =-(c2xLap4(1) -c2xyy4(1))/eps2
!           aa(ne,10)=-(c2xLap4(2) -c2xyy4(2))/eps2
!           aa(ne,11)=-(c2xLap4(3) -c2xyy4(3))/eps2

 ! f(6)=( ((vv1xxx4+vv1xyy4)-(uu1xxy4+uu1yyy4)) +(uu1xxy4+vv1xyy4) )/eps1 - \
 !      ( ((vv2xxx4+vv2xyy4)-(uu2xxy4+uu2yyy4)) +(uu2xxy4+vv2xyy4) )/eps2

           ne=ne+1
           aa(ne,0) =-(c1yLap4(1) -c1xxy4(1))/eps1     ! coeff of u1(-1) 
           aa(ne,1) =-(c1yLap4(2) -c1xxy4(2))/eps1     ! u1(-2)
           aa(ne,2) =-(c1yLap4(3) -c1xxy4(3))/eps1     ! u1(-3)
           aa(ne,3) = (c1xLap4(1) +c1xyy4(1))/eps1     ! coeff of v1(-1) 
           aa(ne,4) = (c1xLap4(2) +c1xyy4(2))/eps1     ! v1(-2) 
           aa(ne,5) = (c1xLap4(3) +c1xyy4(3))/eps1     ! v1(-3) 

           aa(ne,6) = (c2yLap4(1) -c2xxy4(1))/eps2
           aa(ne,7) = (c2yLap4(2) -c2xxy4(2))/eps2
           aa(ne,8) = (c2yLap4(3) -c2xxy4(3))/eps2
           aa(ne,9) =-(c2xLap4(1) +c2xyy4(1))/eps2
           aa(ne,10)=-(c2xLap4(2) +c2xyy4(2))/eps2
           aa(ne,11)=-(c2xLap4(3) +c2xyy4(3))/eps2

!           ne=ne+1
!           aa(ne,0) =-(c1yLap4(1))/eps1     ! coeff of u1(-1) 
!           aa(ne,1) =-(c1yLap4(2))/eps1     ! u1(-2)
!           aa(ne,2) =-(c1yLap4(3))/eps1     ! u1(-3)
!           aa(ne,3) = (c1xLap4(1))/eps1     ! coeff of v1(-1) 
!           aa(ne,4) = (c1xLap4(2))/eps1     ! v1(-2) 
!           aa(ne,5) = (c1xLap4(3))/eps1     ! v1(-3) 
!
!           aa(ne,6) = (c2yLap4(1))/eps2
!           aa(ne,7) = (c2yLap4(2))/eps2
!           aa(ne,8) = (c2yLap4(3))/eps2
!           aa(ne,9) =-(c2xLap4(1))/eps2
!           aa(ne,10)=-(c2xLap4(2))/eps2
!           aa(ne,11)=-(c2xLap4(3))/eps2

           ! 7  [tau.Lap^2(uv)/eps**2] = 0
           ne=ne+1
           aa(ne,0) = tau1*c1LapSq4(1)/eps1**2
           aa(ne,1) = tau1*c1LapSq4(2)/eps1**2
           aa(ne,2) = tau1*c1LapSq4(3)/eps1**2
           aa(ne,3) = tau2*c1LapSq4(1)/eps1**2
           aa(ne,4) = tau2*c1LapSq4(2)/eps1**2
           aa(ne,5) = tau2*c1LapSq4(3)/eps1**2

           aa(ne,6) =-tau1*c2LapSq4(1)/eps2**2
           aa(ne,7) =-tau1*c2LapSq4(2)/eps2**2
           aa(ne,8) =-tau1*c2LapSq4(3)/eps2**2
           aa(ne,9) =-tau2*c2LapSq4(1)/eps2**2
           aa(ne,10)=-tau2*c2LapSq4(2)/eps2**2
           aa(ne,11)=-tau2*c2LapSq4(3)/eps2**2


           ! --- equations using 2nd order approximations  ----
           ! 8 [div(Lap^2(uv))/eps^2]=0
 !  f(8)=((uu1xxxxx2+2.*uu1xxxyy2+uu1xyyyy2)+(vv1xxxxy2+2.*vv1xxyyy2+vv1yyyyy2) - \
 !        (tau1a*(uu1xxxxy2+2.*uu1xxyyy2+uu1yyyyy2)+tau2a*(vv1xxxxy2+2.*vv1xxyyy2+ vv1yyyyy2)) )/eps1**2 - \
 !       ((uu2xxxxx2+2.*uu2xxxyy2+uu2xyyyy2)+(vv2xxxxy2+2.*vv2xxyyy2+vv2yyyyy2)- \
 !        (tau1a*(uu2xxxxy2+2.*uu2xxyyy2+uu2yyyyy2)+tau2a*(vv2xxxxy2+2.*vv2xxyyy2+ vv2yyyyy2)) )/eps2**2

           ne=ne+1
           aa(ne,0) = (c1xLapSq2(1))/eps1**2     ! coeff of u1(-1)
           aa(ne,1) = (c1xLapSq2(2))/eps1**2     ! u1(-2)
           aa(ne,2) = (c1xLapSq2(3))/eps1**2     ! u1(-3)
           aa(ne,3) = (c1yLapSq2(1) - (c1xxxxy2(1)+2.*c1xxyyy2(1)+c1yyyyy2(1)) )/eps1**2     ! coeff of v1(-1)
           aa(ne,4) = (c1yLapSq2(2) - (c1xxxxy2(2)+2.*c1xxyyy2(2)+c1yyyyy2(2)) )/eps1**2     ! v1(-2) 
           aa(ne,5) = (c1yLapSq2(3) - (c1xxxxy2(3)+2.*c1xxyyy2(3)+c1yyyyy2(3)) )/eps1**2     ! v1(-3) 

           aa(ne,6) =-(c2xLapSq2(1))/eps2**2
           aa(ne,7) =-(c2xLapSq2(2))/eps2**2
           aa(ne,8) =-(c2xLapSq2(3))/eps2**2
           aa(ne,9) =-(c2yLapSq2(1) - (c2xxxxy2(1)+2.*c2xxyyy2(1)+c2yyyyy2(1)) )/eps2**2
           aa(ne,10)=-(c2yLapSq2(2) - (c2xxxxy2(2)+2.*c2xxyyy2(2)+c2yyyyy2(2)) )/eps2**2
           aa(ne,11)=-(c2yLapSq2(3) - (c2xxxxy2(3)+2.*c2xxyyy2(3)+c2yyyyy2(3)) )/eps2**2

!           ne=ne+1
!           aa(ne,0) = (c1xLapSq2(1))/eps1**2     ! coeff of u1(-1)
!           aa(ne,1) = (c1xLapSq2(2))/eps1**2     ! u1(-2)
!           aa(ne,2) = (c1xLapSq2(3))/eps1**2     ! u1(-3)
!           aa(ne,3) = (c1yLapSq2(1) )/eps1**2     ! coeff of v1(-1)
!           aa(ne,4) = (c1yLapSq2(2) )/eps1**2     ! v1(-2) 
!           aa(ne,5) = (c1yLapSq2(3) )/eps1**2     ! v1(-3) 
!
!           aa(ne,6) =-(c2xLapSq2(1))/eps2**2
!           aa(ne,7) =-(c2xLapSq2(2))/eps2**2
!           aa(ne,8) =-(c2xLapSq2(3))/eps2**2
!           aa(ne,9) =-(c2yLapSq2(1) )/eps2**2
!           aa(ne,10)=-(c2yLapSq2(2) )/eps2**2
!           aa(ne,11)=-(c2yLapSq2(3) )/eps2**2

           
           ! 9 [n.Lap^3(uv)/eps^2] = 0
           ne=ne+1
           aa(ne,0) = an1*c1LapCubed2(1)/eps1**2 
           aa(ne,1) = an1*c1LapCubed2(2)/eps1**2 
           aa(ne,2) = an1*c1LapCubed2(3)/eps1**2 
           aa(ne,3) = an2*c1LapCubed2(1)/eps1**2 
           aa(ne,4) = an2*c1LapCubed2(2)/eps1**2 
           aa(ne,5) = an2*c1LapCubed2(3)/eps1**2 

           aa(ne,6) =-an1*c2LapCubed2(1)/eps2**2
           aa(ne,7) =-an1*c2LapCubed2(2)/eps2**2
           aa(ne,8) =-an1*c2LapCubed2(3)/eps2**2
           aa(ne,9) =-an2*c2LapCubed2(1)/eps2**2
           aa(ne,10)=-an2*c2LapCubed2(2)/eps2**2
           aa(ne,11)=-an2*c2LapCubed2(3)/eps2**2

           !  10  [ Lap^2(v.x - u.y)/eps^2 ] =0 
 !  f(10)=( ((vv1xxxxx2+2.*vv1xxxyy2+vv1xyyyy2)-(uu1xxxxy2+2.*uu1xxyyy2+uu1yyyyy2)) + \
 !                (uu1xxxxy2+vv1xxxyy2) +2.*(uu1xxyyy2+vv1xyyyy2)                )/eps1**2 - \
 !        ( ((vv2xxxxx2+2.*vv2xxxyy2+vv2xyyyy2)-(uu2xxxxy2+2.*uu2xxyyy2+uu2yyyyy2) + \
 !                (uu2xxxxy2+vv2xxxyy2) +2.*(uu2xxyyy2+vv2xyyyy2)                )/eps2**2
           ne=ne+1
           aa(ne,0) =-(c1yLapSq2(1) -(c1xxxxy2(1)+2.*c1xxyyy2(1)) )/eps1**2     ! coeff of u1(-1) 
           aa(ne,1) =-(c1yLapSq2(2) -(c1xxxxy2(2)+2.*c1xxyyy2(2)) )/eps1**2     ! u1(-2)
           aa(ne,2) =-(c1yLapSq2(3) -(c1xxxxy2(3)+2.*c1xxyyy2(3)) )/eps1**2     ! u1(-3)
           aa(ne,3) = (c1xLapSq2(1) +(c1xxxyy2(1)+2.*c1xyyyy2(1)) )/eps1**2     ! coeff of v1(-1) 
           aa(ne,4) = (c1xLapSq2(2) +(c1xxxyy2(2)+2.*c1xyyyy2(2)) )/eps1**2     ! v1(-2) 
           aa(ne,5) = (c1xLapSq2(3) +(c1xxxyy2(3)+2.*c1xyyyy2(3)) )/eps1**2     ! v1(-3) 

           aa(ne,6) = (c2yLapSq2(1) -(c2xxxxy2(1)+2.*c2xxyyy2(1)) )/eps2**2
           aa(ne,7) = (c2yLapSq2(2) -(c2xxxxy2(2)+2.*c2xxyyy2(2)) )/eps2**2
           aa(ne,8) = (c2yLapSq2(3) -(c2xxxxy2(3)+2.*c2xxyyy2(3)) )/eps2**2
           aa(ne,9) =-(c2xLapSq2(1) +(c2xxxyy2(1)+2.*c2xyyyy2(1)) )/eps2**2
           aa(ne,10)=-(c2xLapSq2(2) +(c2xxxyy2(2)+2.*c2xyyyy2(2)) )/eps2**2
           aa(ne,11)=-(c2xLapSq2(3) +(c2xxxyy2(3)+2.*c2xyyyy2(3)) )/eps2**2

!           ne=ne+1
!           aa(ne,0) =-(c1yLapSq2(1) )/eps1**2     ! coeff of u1(-1) 
!           aa(ne,1) =-(c1yLapSq2(2) )/eps1**2     ! u1(-2)
!           aa(ne,2) =-(c1yLapSq2(3) )/eps1**2     ! u1(-3)
!           aa(ne,3) = (c1xLapSq2(1) )/eps1**2     ! coeff of v1(-1) 
!           aa(ne,4) = (c1xLapSq2(2) )/eps1**2     ! v1(-2) 
!           aa(ne,5) = (c1xLapSq2(3) )/eps1**2     ! v1(-3) 
!
!           aa(ne,6) = (c2yLapSq2(1) )/eps2**2
!           aa(ne,7) = (c2yLapSq2(2) )/eps2**2
!           aa(ne,8) = (c2yLapSq2(3) )/eps2**2
!           aa(ne,9) =-(c2xLapSq2(1) )/eps2**2
!           aa(ne,10)=-(c2xLapSq2(2) )/eps2**2
!           aa(ne,11)=-(c2xLapSq2(3) )/eps2**2

           ! 11 [tau.Lap^3(uv)/eps**3] = 0
           ne=ne+1
           aa(ne,0) = tau1*c1LapCubed2(1)/eps1**3
           aa(ne,1) = tau1*c1LapCubed2(2)/eps1**3
           aa(ne,2) = tau1*c1LapCubed2(3)/eps1**3
           aa(ne,3) = tau2*c1LapCubed2(1)/eps1**3
           aa(ne,4) = tau2*c1LapCubed2(2)/eps1**3
           aa(ne,5) = tau2*c1LapCubed2(3)/eps1**3

           aa(ne,6) =-tau1*c2LapCubed2(1)/eps2**3
           aa(ne,7) =-tau1*c2LapCubed2(2)/eps2**3
           aa(ne,8) =-tau1*c2LapCubed2(3)/eps2**3
           aa(ne,9) =-tau2*c2LapCubed2(1)/eps2**3
           aa(ne,10)=-tau2*c2LapCubed2(2)/eps2**3
           aa(ne,11)=-tau2*c2LapCubed2(3)/eps2**3


!*        if( debug.gt.0 ) write(*,'(" --> 6c: j1,j2=",2i4," u1xx,u1yy,u2xx,u2yy=",4e10.2)') j1,j2,u1xx42(i1,i2,i3,ex),\
!*            u1yy42(i1,i2,i3,ex),u2xx42(j1,j2,j3,ex),u2yy42(j1,j2,j3,ex)
!*        if( debug.gt.0 ) write(*,'(" --> 6c: i1,i2=",2i4," f(start)=",8e10.2)') i1,i2,f(0),f(1),f(2),f(3),f(4),f(5),f(6),f(7)
!* 
!* 
           q(0) = u1(i1-  is1,i2-  is2,i3,ex)
           q(1) = u1(i1-2*is1,i2-2*is2,i3,ex)
           q(2) = u1(i1-3*is1,i2-3*is2,i3,ex)

           q(3) = u1(i1-  is1,i2-  is2,i3,ey)
           q(4) = u1(i1-2*is1,i2-2*is2,i3,ey)
           q(5) = u1(i1-3*is1,i2-3*is2,i3,ey)

           q(6) = u2(j1-  js1,j2-  js2,j3,ex)
           q(7) = u2(j1-2*js1,j2-2*js2,j3,ex)
           q(8) = u2(j1-3*js1,j2-3*js2,j3,ex)

           q(9) = u2(j1-  js1,j2-  js2,j3,ey)
           q(10)= u2(j1-2*js1,j2-2*js2,j3,ey)
           q(11)= u2(j1-3*js1,j2-3*js2,j3,ey)


!* 
!*        if( debug.gt.0 ) write(*,'(" --> 6c: i1,i2=",2i4," q=",8e10.2)') i1,i2,q(0),q(1),q(2),q(3),q(4),q(5),q(6),q(7)
!* 
           ! subtract off the contributions from the initial (wrong) values at the ghost points:
           do n=0,11
             f(n) = (aa(n,0)*q(0)+aa(n,1)*q(1)+aa(n,2)*q(2)+aa(n,3)*q(3)+\
                     aa(n,4)*q(4)+aa(n,5)*q(5)+aa(n,6)*q(6)+aa(n,7)*q(7)+\
                     aa(n,8)*q(8)+aa(n,9)*q(9)+aa(n,10)*q(10)+aa(n,11)*q(11)) - f(n)
           end do

           ! scale equations to make better conditionned
            do n=0,11
              f(n)=f(n)*scale(n)
              do m=0,11
                aa(n,m)=aa(n,m)*scale(n)
              end do
            end do
           ! solve A Q = F
           ! factor the matrix
           numberOfEquations=12
           call dgeco( aa(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0))
           ! solve
       if( debug.gt.0 ) write(*,'(" --> 6c: i1,i2=",2i4," rcond=",e10.2," omega=",f4.2)') i1,i2,rcond,omega
            ! '
           job=0
           call dgesl( aa(0,0), numberOfEquations, numberOfEquations, ipvt(0), f(0), job)

           if( useJacobi.eq.1 )then

             ! save the solution to assign later
             do n=0,11
               uj(i2,n)=f(n)
             end do

           else
            u1(i1-  is1,i2-  is2,i3,ex)=q(0) +omega*(f(0)-q(0))   
            u1(i1-2*is1,i2-2*is2,i3,ex)=q(1) +omega*(f(1)-q(1))   
            u1(i1-3*is1,i2-3*is2,i3,ex)=q(2) +omega*(f(2)-q(2))   
                                                                  
            u1(i1-  is1,i2-  is2,i3,ey)=q(3) +omega*(f(3)-q(3))   
            u1(i1-2*is1,i2-2*is2,i3,ey)=q(4) +omega*(f(4)-q(4))   
            u1(i1-3*is1,i2-3*is2,i3,ey)=q(5) +omega*(f(5)-q(5))   
                                                                  
            u2(j1-  js1,j2-  js2,j3,ex)=q(6) +omega*(f(6)-q(6))   
            u2(j1-2*js1,j2-2*js2,j3,ex)=q(7) +omega*(f(7)-q(7))   
            u2(j1-3*js1,j2-3*js2,j3,ex)=q(8) +omega*(f(8)-q(8))   
                                                                  
            u2(j1-  js1,j2-  js2,j3,ey)=q(9) +omega*(f(9)-q(9))   
            u2(j1-2*js1,j2-2*js2,j3,ey)=q(10)+omega*(f(10)-q(10)) 
            u2(j1-3*js1,j2-3*js2,j3,ey)=q(11)+omega*(f(11)-q(11)) 
 
            ! compute the maximum change in the solution for this iteration
            computeErrors()

           end if ! end not jacobi

           ! smooth last line in tangential direction 
           if( .false. .and. axis1.eq.0 )then
             write(*,'(" ...smooth line 3...")') 
             do n=0,2
              u1(i1-3*is1,i2,i3,n)= .5*u1(i1-3*is1,i2,i3,n) + .25*(u1(i1-3*is1,i2+1,i3,n) + u1(i1-3*is1,i2-1,i3,n))
              u2(j1-3*js1,j2,j3,n)= .5*u2(j1-3*js1,j2,j3,n) + .25*(u2(j1-3*js1,j2+1,j3,n) + u2(j1-3*js1,j2-1,j3,n))
             end do
           end if

         endLoops2d()
         ! =============== end loops =======================
      
         if( useJacobi.eq.1 )then
           beginLoops2d() 
            ! for errors:
            q(0) = u1(i1-  is1,i2-  is2,i3,ex)
            q(1) = u1(i1-2*is1,i2-2*is2,i3,ex)
            q(2) = u1(i1-3*is1,i2-3*is2,i3,ex)
 
            q(3) = u1(i1-  is1,i2-  is2,i3,ey)
            q(4) = u1(i1-2*is1,i2-2*is2,i3,ey)
            q(5) = u1(i1-3*is1,i2-3*is2,i3,ey)
 
            q(6) = u2(j1-  js1,j2-  js2,j3,ex)
            q(7) = u2(j1-2*js1,j2-2*js2,j3,ex)
            q(8) = u2(j1-3*js1,j2-3*js2,j3,ex)
 
            q(9) = u2(j1-  js1,j2-  js2,j3,ey)
            q(10)= u2(j1-2*js1,j2-2*js2,j3,ey)
            q(11)= u2(j1-3*js1,j2-3*js2,j3,ey)

            ! for errors:
            do n=0,11 
              f(n)=uj(i2,n) 
            end do

            u1(i1-  is1,i2-  is2,i3,ex)=q(0) +omega*(f(0)-q(0))   
            u1(i1-2*is1,i2-2*is2,i3,ex)=q(1) +omega*(f(1)-q(1))   
            u1(i1-3*is1,i2-3*is2,i3,ex)=q(2) +omega*(f(2)-q(2))   
                                                                  
            u1(i1-  is1,i2-  is2,i3,ey)=q(3) +omega*(f(3)-q(3))   
            u1(i1-2*is1,i2-2*is2,i3,ey)=q(4) +omega*(f(4)-q(4))   
            u1(i1-3*is1,i2-3*is2,i3,ey)=q(5) +omega*(f(5)-q(5))   
                                                                  
            u2(j1-  js1,j2-  js2,j3,ex)=q(6) +omega*(f(6)-q(6))   
            u2(j1-2*js1,j2-2*js2,j3,ex)=q(7) +omega*(f(7)-q(7))   
            u2(j1-3*js1,j2-3*js2,j3,ex)=q(8) +omega*(f(8)-q(8))   
                                                                  
            u2(j1-  js1,j2-  js2,j3,ey)=q(9) +omega*(f(9)-q(9))   
            u2(j1-2*js1,j2-2*js2,j3,ey)=q(10)+omega*(f(10)-q(10)) 
            u2(j1-3*js1,j2-3*js2,j3,ey)=q(11)+omega*(f(11)-q(11)) 

            computeErrors()
           endLoops2d()
         end if



         periodicUpdate2d(u1,boundaryCondition1,gridIndexRange1,side1,axis1)
         periodicUpdate2d(u2,boundaryCondition2,gridIndexRange2,side2,axis2)

           if( debug.gt.0 )then 
             write(*,'(" ***it=",i2," max-diff = ",e11.2," err1,err2,err3=",3e12.2)') it,err,err1,err2,err3
               ! '
           end if
         end do ! ************** end iteration **************
!* 
!* 
!*          ! now make sure that div(u)=0 etc.
!*          if( .false. )then
!*          beginLoops2d() ! =============== start loops =======================
!* 
!*            ! 0  [ u.x + v.y ] = 0
!* c           a8(0,0) = -is*8.*rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)     ! coeff of u1(-1) from [u.x+v.y] 
!* c           a8(0,1) = -is*8.*rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)     ! coeff of v1(-1) from [u.x+v.y] 
!* c           a8(0,4) =  is*   rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)     ! u1(-2)
!* c           a8(0,5) =  is*   rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)     ! v1(-2) 
!* 
!* c           a8(0,2) =  js*8.*rsxy2(j1,j2,j3,axis2,0)*dr214(axis2)     ! coeff of u2(-1) from [u.x+v.y] 
!* c           a8(0,3) =  js*8.*rsxy2(j1,j2,j3,axis2,1)*dr214(axis2)  
!* c           a8(0,6) = -js*   rsxy2(j1,j2,j3,axis2,0)*dr214(axis2) 
!* c           a8(0,7) = -js*   rsxy2(j1,j2,j3,axis2,1)*dr214(axis2) 
!* 
!*            ! first evaluate the equations we want to solve with the wrong values at the ghost points:
!*            divu=u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey)
!*            a0=is*   rsxy1(i1,i2,i3,axis1,0)*dr114(axis1)
!*            a1=is*   rsxy1(i1,i2,i3,axis1,1)*dr114(axis1)
!*            aNormSq=a0**2+a1**2
!*            ! now project:  a.uNew = a.uOld - div  ->  (div-a.uOld)+a.uNew = div(uNew) = 0
!*            u1(i1-2*is1,i2-2*is2,i3,ex)=u1(i1-2*is1,i2-2*is2,i3,ex)-divu*a0/aNormSq
!*            u1(i1-2*is1,i2-2*is2,i3,ey)=u1(i1-2*is1,i2-2*is2,i3,ey)-divu*a1/aNormSq
!* 
!*            divu=u2x42(j1,j2,j3,ex)+u2y42(j1,j2,j3,ey)
!*            a0=js*   rsxy2(j1,j2,j3,axis2,0)*dr214(axis2) 
!*            a1=js*   rsxy2(j1,j2,j3,axis2,1)*dr214(axis2) 
!*            aNormSq=a0**2+a1**2
!* 
!*            u2(j1-2*js1,j2-2*js2,j3,ex)=u2(j1-2*js1,j2-2*js2,j3,ex)-divu*a0/aNormSq
!*            u2(j1-2*js1,j2-2*js2,j3,ey)=u2(j1-2*js1,j2-2*js2,j3,ey)-divu*a1/aNormSq
!* 
!*            if( debug.gt.0 )then
!*              divu=u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey)
!*               write(*,'(" --> 6c: eval div1,div2=",2e10.2)') u1x42(i1,i2,i3,ex)+u1y42(i1,i2,i3,ey),u2x42(j1,j2,j3,ex)+u2y42(j1,j2,j3,ey)
!*            end if
!*          endLoops2d()

!*         end if
       else
         stop 3214
       end if
      else  
         ! 3D
        stop 6676
      end if

      return
      end
