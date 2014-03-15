! These next include files will define the macros that will define the difference approximations
! The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"


#beginMacro beginLoops()
! Loop over boundary points
do i3=n3a+is3,n3b+is3
do i2=n2a+is2,n2b+is2
do i1=n1a+is1,n1b+is1
#endMacro

#beginMacro endLoops()
end do
end do
end do
#endMacro

! Extrapolation of u(i1,i2,i3) in direction (is1,is2,is3)
! WARNING : a macro named extrap3 is defined in neumannEquationForcing.h
#defineMacro extrapolate3(u,i1,i2,i3,is1,is2,is3) (3.*u(i1,i2,i3)-3.*u(i1+is1,i2+is2,i3+is3)+u(i1+2*is1,i2+2*is2,i3+2*is3))
#defineMacro extrapolate2(u,i1,i2,i3,is1,is2,is3) (2.*u(i1,i2,i3)-u(i1+is1,i2+is2,i3+is3))


#beginMacro loops(expression)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
  expression
end do
end do
end do
#endMacro

#beginMacro loops2(e1,e2)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
  e1
  e2
end do
end do
end do
#endMacro

! use the mask 
#beginMacro loopsMaskGT(expression)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
    expression
  end if
end do
end do
end do
#endMacro

#beginMacro loops2MaskGT(e1,e2)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
    e1
    e2
  end if
end do
end do
end do
#endMacro

#beginMacro extrapolatePoint()
 if( orderOfExtrapolation.eq.3 )then
   u(i1-is1,i2-is2,i3-is3)=\
    3.*u(i1      ,i2      ,i3      )\
   -3.*u(i1+  is1,i2+  is2,i3+  is3)\
      +u(i1+2*is1,i2+2*is2,i3+2*is3)
 else if( orderOfExtrapolation.eq.4 )then
   u(i1-is1,i2-is2,i3-is3)=\
     4.*u(i1      ,i2      ,i3      )\
    -6.*u(i1+  is1,i2+  is2,i3+  is3)\
    +4.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
       -u(i1+3*is1,i2+3*is2,i3+3*is3)
 else if( orderOfExtrapolation.eq.5 )then
   u(i1-is1,i2-is2,i3-is3)=\
     5.*u(i1      ,i2      ,i3      )\
   -10.*u(i1+  is1,i2+  is2,i3+  is3)\
   +10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
    -5.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
       +u(i1+4*is1,i2+4*is2,i3+4*is3)
 else if( orderOfExtrapolation.eq.6 )then
   u(i1-is1,i2-is2,i3-is3)=\
     6.*u(i1      ,i2      ,i3      )\
   -15.*u(i1+  is1,i2+  is2,i3+  is3)\
   +20.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
   -15.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
    +6.*u(i1+4*is1,i2+4*is2,i3+4*is3)\
       -u(i1+5*is1,i2+5*is2,i3+5*is3)
 else if( orderOfExtrapolation.eq.7 )then
   u(i1-is1,i2-is2,i3-is3)=\
     7.*u(i1      ,i2      ,i3      )\
   -21.*u(i1+  is1,i2+  is2,i3+  is3)\
   +35.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
   -35.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
   +21.*u(i1+4*is1,i2+4*is2,i3+4*is3)\
    -7.*u(i1+5*is1,i2+5*is2,i3+5*is3)\
       +u(i1+6*is1,i2+6*is2,i3+6*is3)
 else if( orderOfExtrapolation.eq.2 )then
   u(i1-is1,i2-is2,i3-is3)=\
    2.*u(i1      ,i2      ,i3      )\
      -u(i1+  is1,i2+  is2,i3+  is3)
 else
   write(*,*) 'bcOpt:ERROR:'
   write(*,*) ' orderOfExtrapolation=',orderOfExtrapolation
   stop 1
 end if
#endMacro


! Optionally using the forcing
#beginMacro loopsMaskForce(e1,e2)
if( useForcing.eq.1 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
   if( mask(i1,i2,i3).gt.0 )then
     e1 
   else if( mask(i1,i2,i3).lt.0 )then
     extrapolatePoint()
   end if
  end do  
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
   if( mask(i1,i2,i3).gt.0 )then
     e2
   else if( mask(i1,i2,i3).lt.0 )then
     extrapolatePoint()
   end if
  end do  
  end do
  end do
end if 
#endMacro

#beginMacro extrapolateSecondGhostPoint()
 u(i1-2*is1,i2-2*is2,i3-2*is3)=\
            5.*u(i1-  is1,i2-  is2,i3-  is3)\
          -10.*u(i1      ,i2      ,i3      )\
          +10.*u(i1+  is1,i2+  is2,i3+  is3)\
           -5.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
              +u(i1+3*is1,i2+3*is2,i3+3*is3)
#endMacro

! Optionally using the forcing, extrapolate 2nd ghost line
#beginMacro loopsMaskForceAndExtrapolate(e1,e2)
if( useForcing.eq.1 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
   if( mask(i1,i2,i3).gt.0 )then
     e1 
     extrapolateSecondGhostPoint()
   else if( mask(i1,i2,i3).lt.0 )then
     extrapolatePoint()
     extrapolateSecondGhostPoint()
   end if
  end do  
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
   if( mask(i1,i2,i3).gt.0 )then
     e2
   else if( mask(i1,i2,i3).lt.0 )then
     extrapolatePoint()
   end if
  end do  
  end do
  end do
end if 
#endMacro

#beginMacro loops2Expressions(expression1,expression2)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
  expression1
  expression2
end do
end do
end do
#endMacro

#beginMacro loops3Expressions(expression1,expression2,expression3)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
  expression1
  expression2
  expression3
end do
end do
end do
#endMacro



! ============================================================================
! Assign values of an expression where the mask is greater than zero
! 
! NOTE:
!   n1a,n1b,n2a,n2b,n3a,n3c : indicies of the GHOST points
! ============================================================================
#beginMacro loopsMaskIS(expression)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
 if( mask(i1+is1,i2+is2,i3+is3).gt.0 )then    
!    if( abs(c(mGhost,i1,i2,i3)).lt.1.e-10 )then
!      write(*,*) 'bcOpt:ERROR: i1,i2,i3,mGhost,c=', i1,i2,i3,mGhost,c(mGhost,i1,i2,i3)
!      write(*,*) ' c(0:2)=',c(0,i1,i2,i3),c(1,i1,i2,i3),c(2,i1,i2,i3)
!      write(*,*) ' c(3:5)=',c(3,i1,i2,i3),c(4,i1,i2,i3),c(5,i1,i2,i3)
!      write(*,*) ' c(6:9)=',c(6,i1,i2,i3),c(7,i1,i2,i3),c(8,i1,i2,i3)
!    end if
    expression
 else if( mask(i1+is1,i2+is2,i3+is3).lt.0 )then    
   u(i1,i2,i3)=extrapolate3(u,i1+is1,i2+is2,i3+is3,is1,is2,is3)
 end if
end do
end do
end do
#endMacro



! =================================================================================================
! Macro: define the fourth-order accurate neumann BC which uses extrapolation of the 2nd ghost line
! =================================================================================================
#beginMacro neumannAndExtrapolation()

 ! In this case the neumann/mixed BC is coupled with the extrapolation of the
 ! second ghost line -- we solve the coupled equations to give the two values

 if( gridType.eq.rectangular )then

   if( a1.eq.0. )then
     write(*,*) 'bcOpt:ERROR: a1=0!'
     stop 2
   end if

   ! write(*,*) '@@@ bcOpt:orderOfExtrapolationForNeumann=',orderOfExtrapolationForNeumann

   dxn=dx(axis)
   if( orderOfExtrapolationForNeumann.eq.5 .and. useForcing.eq.1 )then
     ! use f at the ghost line as the RHS for the mixed BC
     b0=-4.*dxn*a0/a1-10./3.
     b1=4.*(dxn/a1)
     loops2Expressions(\
     u(i1,i2,i3)=  b0*u(i1+  is1,i2+  is2,i3+  is3)\
                  +6.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
                  -2.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
                     +u(i1+4*is1,i2+4*is2,i3+4*is3)/3.\
                     +b1*f(i1,i2,i3),\
     u(i1-is1,i2-is2,i3-is3)=\
       5.*u(i1      ,i2      ,i3      )\
     -10.*u(i1+  is1,i2+  is2,i3+  is3)\
     +10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
      -5.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
         +u(i1+4*is1,i2+4*is2,i3+4*is3))
   else if( orderOfExtrapolationForNeumann.eq.5 .and. useForcing.eq.0 )then
     b0=-4.*dxn*a0/a1-10./3.
     b1=4.*(dxn/a1)
     loops2Expressions(\
     u(i1,i2,i3)=  b0*u(i1+  is1,i2+  is2,i3+  is3)\
                  +6.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
                  -2.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
                     +u(i1+4*is1,i2+4*is2,i3+4*is3)/3.,\
     u(i1-is1,i2-is2,i3-is3)=\
       5.*u(i1      ,i2      ,i3      )\
     -10.*u(i1+  is1,i2+  is2,i3+  is3)\
     +10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
      -5.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
         +u(i1+4*is1,i2+4*is2,i3+4*is3))
   else if( orderOfExtrapolationForNeumann.eq.4 .and. useForcing.eq.1 )then
     ! use f at the ghost line as the RHS for the mixed BC
     b0=-3.*dxn*a0/a1-1.5
     b1=3.*(dxn/a1)
     loops2Expressions(\
     u(i1,i2,i3)=  b0*u(i1+  is1,i2+  is2,i3+  is3)\
                  +3.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
                  -.5*u(i1+3*is1,i2+3*is2,i3+3*is3)\
                     +b1*f(i1,i2,i3),\
     u(i1-is1,i2-is2,i3-is3)=\
       4.*u(i1      ,i2      ,i3      )\
     - 6.*u(i1+  is1,i2+  is2,i3+  is3)\
     + 4.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
      -   u(i1+3*is1,i2+3*is2,i3+3*is3))
   else if( orderOfExtrapolationForNeumann.eq.4 .and. useForcing.eq.0 )then
     b0=-3.*dxn*a0/a1-1.5
     b1=3.*(dxn/a1)
     loops2Expressions(\
     u(i1,i2,i3)=  b0*u(i1+  is1,i2+  is2,i3+  is3)\
                  +3.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
                  -.5*u(i1+3*is1,i2+3*is2,i3+3*is3),\
     u(i1-is1,i2-is2,i3-is3)=\
       4.*u(i1      ,i2      ,i3      )\
     - 6.*u(i1+  is1,i2+  is2,i3+  is3)\
     + 4.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
      -   u(i1+3*is1,i2+3*is2,i3+3*is3))
   else
     write(*,*) 'bcOpt:ERROR: orderOfExtrapolationForNeumann=',orderOfExtrapolationForNeumann,' useForcing=',useForcing
     stop 1
   end if
 else
   ! **** curvilinear case ****

   if( nd.eq.2 )then

     m11=1     
     m21=2     
     m31=3     
     m41=4     
     m51=5     
     m12=6     
     m22=7     
     m32=8     
     m42=9     
     m52=10    
     m13=11    
     m23=12    
     m33=13    
     m43=14    
     m53=15    
     m14=16    
     m24=17    
     m34=18    
     m44=19    
     m54=20    
     m15=21    
     m25=22    
     m35=23    
     m45=24    
     m55=25    

     ! mg2 is the index of the 2nd ghost point 
     ! mg1 is the index of the 1st ghost point
     mg2 = m33-2*is1-2*5*is2          ! m13 or m53 or m31 or m35
     mg1 = mg2+is1+5*is2              ! m23 or m43 or m32 or m34

!     write(*,*) 'bcOpt: 4th order neumann BC -- curvilinear'
!     write(*,*) 'orderOfExtrapolation=',orderOfExtrapolation

     if( orderOfExtrapolationForNeumann.eq.5 .and. useForcing.eq.1 )then
       ! use f at the ghost line as the RHS for the mixed BC
       loops3Expressions(\
       t1=op2dSparse4(i1,i2,i3,i1+is1,i2+is2,i3+is3)-c(mg1,i1,i2,i3)*u(i1,i2,i3)-c(mg2,i1,i2,i3)*u(i1-is1,i2-is2,i3-is3),\
       u(i1,i2,i3)=(f(i1,i2,i3)-t1\
          +(10.*u(i1+  is1,i2+  is2,i3+  is3)\
           -10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
            +5.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
               -u(i1+4*is1,i2+4*is2,i3+4*is3))*c(mg2,i1,i2,i3))\
               /(5.*c(mg2,i1,i2,i3)+c(mg1,i1,i2,i3)),\
      u(i1-is1,i2-is2,i3-is3)=\
        5.*u(i1      ,i2      ,i3      )\
      -10.*u(i1+  is1,i2+  is2,i3+  is3)\
      +10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
       -5.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
          +u(i1+4*is1,i2+4*is2,i3+4*is3))
     else if( orderOfExtrapolationForNeumann.eq.5 .and. useForcing.eq.0 )then
       loops3Expressions(\
       t1=op2dSparse4(i1,i2,i3,i1+is1,i2+is2,i3+is3)-c(mg1,i1,i2,i3)*u(i1,i2,i3)-c(mg2,i1,i2,i3)*u(i1-is1,i2-is2,i3-is3),\
       u(i1,i2,i3)=(           -t1\
          +(10.*u(i1+  is1,i2+  is2,i3+  is3)\
           -10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
            +5.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
               -u(i1+4*is1,i2+4*is2,i3+4*is3))*c(mg2,i1,i2,i3))\
               /(5.*c(mg2,i1,i2,i3)+c(mg1,i1,i2,i3)),\
      u(i1-is1,i2-is2,i3-is3)=\
        5.*u(i1      ,i2      ,i3      )\
      -10.*u(i1+  is1,i2+  is2,i3+  is3)\
      +10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
       -5.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
          +u(i1+4*is1,i2+4*is2,i3+4*is3))

     else if( orderOfExtrapolationForNeumann.eq.4 .and. useForcing.eq.1 )then
       loops3Expressions(\
       t1=op2dSparse4(i1,i2,i3,i1+is1,i2+is2,i3+is3)-c(mg1,i1,i2,i3)*u(i1,i2,i3)-c(mg2,i1,i2,i3)*u(i1-is1,i2-is2,i3-is3),\
       u(i1,i2,i3)=(f(i1,i2,i3)-t1\
          +( 6.*u(i1+  is1,i2+  is2,i3+  is3)\
            -4.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
               +u(i1+3*is1,i2+3*is2,i3+3*is3))*c(mg2,i1,i2,i3))\
               /(4.*c(mg2,i1,i2,i3)+c(mg1,i1,i2,i3)),\
      u(i1-is1,i2-is2,i3-is3)=\
        4.*u(i1      ,i2      ,i3      )\
      - 6.*u(i1+  is1,i2+  is2,i3+  is3)\
      + 4.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
         - u(i1+3*is1,i2+3*is2,i3+3*is3))

     else if( orderOfExtrapolationForNeumann.eq.4 .and. useForcing.eq.0 )then

       loops3Expressions(\
       t1=op2dSparse4(i1,i2,i3,i1+is1,i2+is2,i3+is3)-c(mg1,i1,i2,i3)*u(i1,i2,i3)-c(mg2,i1,i2,i3)*u(i1-is1,i2-is2,i3-is3),\
       u(i1,i2,i3)=(           -t1\
          +( 6.*u(i1+  is1,i2+  is2,i3+  is3)\
            -4.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
               +u(i1+3*is1,i2+3*is2,i3+3*is3))*c(mg2,i1,i2,i3))\
               /(4.*c(mg2,i1,i2,i3)+c(mg1,i1,i2,i3)),\
      u(i1-is1,i2-is2,i3-is3)=\
        4.*u(i1      ,i2      ,i3      )\
      - 6.*u(i1+  is1,i2+  is2,i3+  is3)\
      + 4.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
         - u(i1+3*is1,i2+3*is2,i3+3*is3))

     else
       write(*,*) 'bcOpt:ERROR: orderOfExtrapolationForNeumann=',orderOfExtrapolationForNeumann,' useForcing=',useForcing
       stop 1
     end if

   else if( nd.eq.3 )then

      !  ***** 4th order **** these were added 030925
      m111=1 
      m211=2 
      m311=3 
      m411=4 
      m511=5 
      m121=6 
      m221=7 
      m321=8 
      m421=9 
      m521=10
      m131=11
      m231=12
      m331=13
      m431=14
      m531=15
      m141=16
      m241=17
      m341=18
      m441=19
      m541=20
      m151=21
      m251=22
      m351=23
      m451=24
      m551=25

      m112=26
      m212=27
      m312=28
      m412=29
      m512=30
      m122=31 
      m222=32
      m322=33
      m422=34
      m522=35
      m132=36
      m232=37
      m332=38
      m432=39
      m532=40
      m142=41
      m242=42
      m342=43
      m442=44
      m542=45
      m152=46
      m252=47
      m352=48
      m452=49
      m552=50

      m113=51 
      m213=52 
      m313=53 
      m413=54 
      m513=55 
      m123=56 
      m223=57 
      m323=58 
      m423=59 
      m523=60
      m133=61
      m233=62
      m333=63
      m433=64
      m533=65
      m143=66
      m243=67
      m343=68
      m443=69
      m543=70
      m153=71
      m253=72
      m353=73
      m453=74
      m553=75

      m114=76
      m214=77
      m314=78
      m414=79
      m514=80
      m124=81 
      m224=82
      m324=83
      m424=84
      m524=85
      m134=86
      m234=87
      m334=88
      m434=89
      m534=90
      m144=91
      m244=92
      m344=93
      m444=94
      m544=95
      m154=96
      m254=97
      m354=98
      m454=99
      m554=100

      m115=101 
      m215=102 
      m315=103 
      m415=104 
      m515=105 
      m125=106 
      m225=107 
      m325=108 
      m425=109 
      m525=110
      m135=111
      m235=112
      m335=113
      m435=114
      m535=115
      m145=116
      m245=117
      m345=118
      m445=119
      m545=120
      m155=121
      m255=122
      m355=123
      m455=124
      m555=125

     ! mg2 is the index of the 2nd ghost point 
     ! mg1 is the index of the 1st ghost point
     mg2 = m333-2*is1-2*5*is2-2*5*5*is3       ! m133 or m533 or m313 or m353 or m331 or m335
     mg1 = mg2+is1+5*is2+5*5*is3              ! m233 or m433 or m323 or m343 or m332 or m334

     if( orderOfExtrapolationForNeumann.eq.5 .and. useForcing.eq.1 )then
       ! use f at the ghost line as the RHS for the mixed BC
       loops3Expressions(\
       t1=(op3dSparse4(i1,i2,i3,i1+is1,i2+is2,i3+is3)-c(mg1,i1,i2,i3)*u(i1,i2,i3))/c(mg2,i1,i2,i3)-u(i1-is1,i2-is2,i3-is3),\
       u(i1,i2,i3)=(t1\
          -10.*u(i1+  is1,i2+  is2,i3+  is3)\
          +10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
           -5.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
              +u(i1+4*is1,i2+4*is2,i3+4*is3)\
               -f(i1,i2,i3)/c(mg2,i1,i2,i3))/(-5.-c(mg1,i1,i2,i3)/c(mg2,i1,i2,i3)),\
      u(i1-is1,i2-is2,i3-is3)=\
        5.*u(i1      ,i2      ,i3      )\
      -10.*u(i1+  is1,i2+  is2,i3+  is3)\
      +10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
       -5.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
          +u(i1+4*is1,i2+4*is2,i3+4*is3))
     else if( orderOfExtrapolationForNeumann.eq.5 .and. useForcing.eq.0 )then
       loops3Expressions(\
       t1=(op3dSparse4(i1,i2,i3,i1+is1,i2+is2,i3+is3)-c(mg1,i1,i2,i3)*u(i1,i2,i3))/c(mg2,i1,i2,i3)-u(i1-is1,i2-is2,i3-is3),\
       u(i1,i2,i3)=(t1\
          -10.*u(i1+  is1,i2+  is2,i3+  is3)\
          +10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
           -5.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
              +u(i1+4*is1,i2+4*is2,i3+4*is3))/(-5.-c(mg1,i1,i2,i3)/c(mg2,i1,i2,i3)),\
      u(i1-is1,i2-is2,i3-is3)=\
        5.*u(i1      ,i2      ,i3      )\
      -10.*u(i1+  is1,i2+  is2,i3+  is3)\
      +10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
       -5.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
          +u(i1+4*is1,i2+4*is2,i3+4*is3))


     else if( orderOfExtrapolationForNeumann.eq.4 .and. useForcing.eq.1 )then
       ! *wdh* order=4 added 110223

       ! use f at the ghost line as the RHS for the mixed BC
       loops3Expressions(\
       t1=(op3dSparse4(i1,i2,i3,i1+is1,i2+is2,i3+is3)-c(mg1,i1,i2,i3)*u(i1,i2,i3))/c(mg2,i1,i2,i3)-u(i1-is1,i2-is2,i3-is3),\
       u(i1,i2,i3)=(t1\
          - 6.*u(i1+  is1,i2+  is2,i3+  is3)\
          + 4.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
           -   u(i1+3*is1,i2+3*is2,i3+3*is3)\
               -f(i1,i2,i3)/c(mg2,i1,i2,i3))/(-4.-c(mg1,i1,i2,i3)/c(mg2,i1,i2,i3)),\
      u(i1-is1,i2-is2,i3-is3)=\
        4.*u(i1      ,i2      ,i3      )\
      - 6.*u(i1+  is1,i2+  is2,i3+  is3)\
      + 4.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
       -   u(i1+3*is1,i2+3*is2,i3+3*is3))
     else if( orderOfExtrapolationForNeumann.eq.4 .and. useForcing.eq.0 )then
       loops3Expressions(\
       t1=(op3dSparse4(i1,i2,i3,i1+is1,i2+is2,i3+is3)-c(mg1,i1,i2,i3)*u(i1,i2,i3))/c(mg2,i1,i2,i3)-u(i1-is1,i2-is2,i3-is3),\
       u(i1,i2,i3)=(t1\
          - 6.*u(i1+  is1,i2+  is2,i3+  is3)\
          + 4.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
           -   u(i1+3*is1,i2+3*is2,i3+3*is3))/(-4.-c(mg1,i1,i2,i3)/c(mg2,i1,i2,i3)),\
      u(i1-is1,i2-is2,i3-is3)=\
        4.*u(i1      ,i2      ,i3      )\
      - 6.*u(i1+  is1,i2+  is2,i3+  is3)\
      + 4.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
       -   u(i1+3*is1,i2+3*is2,i3+3*is3))
     else
       write(*,*) 'bcOpt:ERROR: orderOfExtrapolationForNeumann=',orderOfExtrapolationForNeumann,' useForcing=',useForcing
       stop 1
     end if

   else
     write(*,*) 'bcOpt:ERROR: nd=',nd
     stop 4
   end if


 end if
#endMacro



! This next file defines the macro call appearing in the next function
#Include "neumannEquationBC.h"
#Include "neumannEquationForcing.h"

!***************************NEW VERSION consistent with lineSmoothOpt.bf *******************
! Define the fourth-order accurate neumann BC which uses the normal derivative of the equation on the
! boundary. 
#beginMacro neumannAndEquationOld(FORCING,DIR,DIM)
 if( gridType.eq.rectangular )then
   if( a1.eq.0. )then
     write(*,*) 'bcOpt:ERROR: a1=0!'
     stop 2
   end if
   
!   write(*,'(''bcOpt:4th-order neumannAndEqnNew (rect) n2a,n2b='',2i3)') n2a,n2b

   drn=dx(axis)
   nsign = 2*side-1
   cf1= nsign*(drn**3)/3. !  030525: add nsign to cf1,cg1,cf2,cg2
   cg1= 2.*nsign*drn

   cf2=nsign*(drn**3)*8/3.
   cg2=nsign*4.*drn

   ! (i1,i2,i3) = boundary point
   ! (j1,j2,j3) = ghost point
   do i3=n3a+is3,n3b+is3
    j3=i3-is3
   do i2=n2a+is2,n2b+is2
    j2=i2-is2
   do i1=n1a+is1,n1b+is1
     if( mask(i1,i2,i3).gt.0 )then
      j1=i1-is1

    ! the rhs for the mixed BC is stored in the ghost point value of 
    defineNeumannEquationForcing(n1a,n1b,n2a,n2b,n3a,n3b,FORCING,rectangular,DIR,DIM)

    #If #DIR eq "R"
      uss=uyy22r(i1,i2,i3)
    #Elif #DIR eq "S"
      urr=uxx22r(i1,i2,i3)  ! need to 2nd-order
    #Else
      stop 7
    #End

    #If #DIR eq "R"
      ur=( g - a0*u(i1,i2,i3) )/(a1*nsign)
      urrr= ffr - (gss - a0*uss )/(a1*nsign) ! 030525
      u(j1,j2,j3)=u(i1+is1,i2,i3)+cf1*urrr+cg1*ur
      u(i1-2*is1,i2,i3)=u(i1+2*is1,i2,i3)+cf2*urrr+cg2*ur
    #Else
      us=( g - a0*u(i1,i2,i3) )/(a1*nsign)
      usss= ffs - (grr - a0*urr )/(a1*nsign) ! 030525
      u(j1,j2,j3)=u(i1,i2+is2,i3)+cf1*usss+cg1*us
      u(i1,i2-2*is2,i3)=u(i1,i2+2*is2,i3)+cf2*usss+cg2*us
    #End
   
!   write(*,'(''bcopt: i1,i2,i3,g,ffr,gss,u,uss ='',3i3,5f11.6)') i1,i2,i3,g,ffr,gss,u(i1,i2,i3),uss
!  write(*,'('' i1,i2,i3,ur,urrr,ffr,gss ='',3i3,4e11.2)') i1,i2,i3,ur,urrr,ffr,gss
!  write(*,'('' i1,i2,i3,f,f,f='',3i3,4e11.2)') i1,i2,i3,f(i1,i2,i3),f(i1+2*is1,i2+2*is2,i3),f(i1+is1,i2+is2,i3)
!      u(j1,j2,j3)=u(i1+is1,i2+is2,i3+is3)
!      u(i1-2*is1,i2-2*is2,i3-2*is3)=u(i1+2*is1,i2+2*is2,i3+2*is3)

    end if
  end do
  end do
  end do



 else
   ! **** curvilinear case ****

   if( axis.gt.1 )then
     write(*,*) 'bcOpt:ERROR: this option not implemented yet'
     write(*,*) 'axis=',axis
     stop 12
   end if

   ! write(*,*) 'bcOpt:4th-order neumann (curvilinear- DIR)'

   nsign = 2*side-1
   drn=dr(axis)
   cf1=nsign*(drn**3)/3.
   cg1=nsign*2.*drn
   cf2=nsign*(drn**3)*8/3.
   cg2=nsign*4.*drn
   alpha1=a1*nsign

   ! (i1,i2,i3) = boundary point
   ! (j1,j2,j3) = ghost point
   do i3=n3a+is3,n3b+is3
     j3=i3-is3
   do i2=n2a+is2,n2b+is2
     j2=i2-is2
   do i1=n1a+is1,n1b+is1
     if( mask(i1,i2,i3).gt.0 )then
      j1=i1-is1

    ! the rhs for the mixed BC is stored in the ghost point value of f
    defineNeumannEquationForcing(n1a,n1b,n2a,n2b,n3a,n3b,FORCING,curvilinear,DIR,DIM)

    #If #DIR eq "R"
      fourthOrderNeumannEquationBC(R,2)
      us=us4(i1,i2,i3)
      uss=uss2(i1,i2,i3) 
      usss=usss2(i1,i2,i3)
    #Elif #DIR eq "S"

      fourthOrderNeumannEquationBC(S,2)

      ur=ur4(i1,i2,i3)
      urr=urr2(i1,i2,i3)  ! need to 2nd-order
      urrr=urrr2(i1,i2,i3)
    #Else
      stop 7
    #End


! *****************
!$$$    ! gdExact(ntd,nxd,nyd,nzd,x,y,z,n,t,value)
!$$$    call gdExact(0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,0,0.,ue0)
!$$$    
!$$$    call gdExact(0,0,0,0,xy(i1-2,i2,i3,0),xy(i1-2,i2,i3,1),0.,0,0.,ue1)
!$$$    call gdExact(0,0,0,0,xy(i1-1,i2,i3,0),xy(i1-1,i2,i3,1),0.,0,0.,ue2)
!$$$    call gdExact(0,0,0,0,xy(i1+1,i2,i3,0),xy(i1+1,i2,i3,1),0.,0,0.,ue3)
!$$$    call gdExact(0,0,0,0,xy(i1+2,i2,i3,0),xy(i1+2,i2,i3,1),0.,0,0.,ue4)
!$$$
!$$$
!$$$    ure   = (ue1   -8.*ue2  +8.*ue3 -ue4)/(12.*dr(0))
!$$$    urre=(-ue1 +16.*ue2 -30.*ue0+16.*ue3 -ue4 )/(12.*dr(0)**2)
!$$$    urrre = (-ue1+2.*ue2-2.*ue3+ue4)/(2.*dr(0)**3)
!$$$
!$$$
!$$$    call gdExact(0,0,0,0,xy(i1,i2-2,i3,0),xy(i1,i2-2,i3,1),0.,0,0.,ue1)
!$$$    call gdExact(0,0,0,0,xy(i1,i2-1,i3,0),xy(i1,i2-1,i3,1),0.,0,0.,ue2)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+1,i3,0),xy(i1,i2+1,i3,1),0.,0,0.,ue3)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+2,i3,0),xy(i1,i2+2,i3,1),0.,0,0.,ue4)
!$$$
!$$$    use=(ue1   -8.*ue2  +8.*ue3 -ue4)/(12.*dr(1))
!$$$    usse=(-ue1 +16.*ue2 -30.*ue0+16.*ue3 -ue4 )/(12.*dr(1)**2)
!$$$    ussse=(-ue1+2.*ue2-2.*ue3+ue4)/(2.*dr(1)**3)
!$$$
!$$$
!$$$    call gdExact(0,0,0,0,xy(i1,i2-1,i3,0),xy(i1,i2-1,i3,1),0.,0,0.,ue1)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+1,i3,0),xy(i1,i2-1,i3,1),0.,0,0.,ue2)
!$$$    call gdExact(0,0,0,0,xy(i1,i2-1,i3,0),xy(i1,i2+1,i3,1),0.,0,0.,ue3)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+1,i3,0),xy(i1,i2+1,i3,1),0.,0,0.,ue4)
!$$$    urse=(ue4-ue3-ue2+ue1)/(4.*dr(0)*dr(1))
! *****************

    #If #DIR eq "R"
      ur= (g - an2*us -a0*u(i1,i2,i3))/an1
      urrr=b0*u(i1,i2,i3)+b1*us+b2*uss+b3*usss+bf 

!   urrree=b0*u(i1,i2,i3)+b1*use+b2*usse+b3*ussse+bf
!
!    write(*,'(''bcopt:side='',i2,'' i1,i2,i3,g,ffr,gss,uss ='',3i3,6f10.4)') side,i1,i2,i3,g,ffr,gss,uss
!    write(*,'("  (x,y)=(",f6.3,",",f6.3,")  ur=",e11.3," exact=",e11.3," urrr =",e11.3," exact=",e11.3," urrree=",e11.3)')\
!               xy(i1,i2,i3,0),xy(i1,i2,i3,1),ur,ure,urrr,urrre,urrree
!    write(*,'("  us=",e11.3," use=",e11.3," uss =",e11.3," usse=",e11.3," usss=",e11.3," ussse=",e11.3)') \
!     us,use,uss,usse,usss,ussse
!
!    urrf=( ff - (c12*urse +c22*usse + c1*ure + c2*use) )/c11
!
!    write(*,'(" c11,c12,c22,c1,c2=",5f10.3,"  urre=",e11.3," urrf=[ff- (c12*urs+...)]/c11=",e11.3') c11,c12,c22,c1,c2,urre,urrf
!    write(*,'(" rxx,ryy,sxx,syy,rxr,rxs,ryr,rys=",8f10.3') rxx,ryy,sxx,syy,rxr,rxs,ryr,rys

!   urrr=urrre

!   write(*,'(''bcopt:side='',i2,'' cg1,cg2,cf1,cf2,um,um2 ='',6f10.6)') side,cg1,cg2,cf1,cf2,u(i1-is1,i2-is2,i3-is3),u(i1-2*is1,i2-2*is2,i3-2*is3)
!    write(*,'(''  n1,n2,an1,an2,c11,b0,b1,b2,b3,bf  ='',10e9.2)') n1,n2,an1,an2,c11,b0,b1,b2,b3,bf
!    write(*,'(''  rxi,ryi,rxNormI  ='',6e11.2)') rxi,ryi,rxNormI

      u(i1-is1,i2-is2,i3-is3)=u(i1+is1,i2+is2,i3+is3)+cf1*urrr+cg1*ur
      u(i1-2*is1,i2-2*is2,i3-2*is3)=u(i1+2*is1,i2+2*is2,i3+2*is3)+cf2*urrr+cg2*ur

    #Else

      us= (g - an1*ur -a0*u(i1,i2,i3))/an2
      usss=b0*u(i1,i2,i3)+b1*ur+b2*urr+b3*urrr+bf 


!   usssee=b0*u(i1,i2,i3)+b1*ure+b2*urre+b3*urrre+bf

!  write(*,'(''bcopt:side='',i2,'' i1,i2,i3,g,ff ='',3i3,2f10.4)') side,i1,i2,i3,g,ff
!  write(*,'("  (x,y)=(",f6.3,",",f6.3,")  us=",e11.3," use=",e11.3," usss =",e11.3," ussse=",e11.3," usssee=",e11.3)')\
!            xy(i1,i2,i3,0),xy(i1,i2,i3,1),us,use,usss,ussse,usssee
!  write(*,'("  ur=",e11.3," ure=",e11.3," urr =",e11.3," urre=",e11.3," urrr=",e11.3," urrre=",e11.3)') \
!     ur,ure,urr,urre,urrr,urrre
!  ussf=( ff - (c12*urse +c11*urre + c1*ure + c2*use) )/c22
!  write(*,'(" c11,c12,c22,c1,c2=",5f9.3,"  usse=",e11.3," ussf=[ff- (c12*urs+...)]/c22=",e11.3') c11,c12,c22,c1,c2,usse,ussf
!    write(*,'(" rxx,ryy,sxx,syy,rxr,rxs,ryr,rys=",8f10.3') rxx,ryy,sxx,syy,rxr,rxs,ryr,rys

!  usss=ussse


      u(i1-is1,i2-is2,i3-is3)=u(i1+is1,i2+is2,i3+is3)+cf1*usss+cg1*us
      u(i1-2*is1,i2-2*is2,i3-2*is3)=u(i1+2*is1,i2+2*is2,i3+2*is3)+cf2*usss+cg2*us
    #End

    end if
  end do
  end do
  end do

 end if
#endMacro



#Include "neumannEquationBC.new.h"

!***************************EVEN NEWER VERSION consistent with lineSmoothOpt.bf *******************
! Define the fourth-order accurate neumann BC which uses the normal derivative of the equation on the
! boundary. 
#beginMacro neumannAndEquation(FORCING,DIR,DIM)

 ! *wdh* 110213 -- FINISH ME for the heat operator 
 ! if( equationToSolve.ne.laplaceEquation .and. equationToSolve.ne.heatEquationOperator)then
 !   write(*,'("Ogmg:bcOpt:neumannAndEqn(order4):ERROR: only implemented for equation=laplace or heatEquationOperator")')
 if( equationToSolve.ne.laplaceEquation )then
   write(*,'("Ogmg:bcOpt:neumannAndEqn(order4):ERROR: only implemented for equation=laplace")')
   write(*,'("equationToSolve=",i2)') equationToSolve
   write(*,'("gridType=",i2)') gridType
   stop 6064
 end if


 if( gridType.eq.rectangular )then
   if( a1.eq.0. )then
     write(*,*) 'bcOpt:ERROR: a1=0!'
     stop 2
   end if
   
   ! write(*,'("bcOpt:order4 neumannAndEqnEvenNewer (rect) grid=",i3," n1a,n1b,n2a,n2b=",4i3)') grid,n1a,n1b,n2a,n2b

   drn=dx(axis)
   nsign = 2*side-1
   br2=-nsign*a0/(a1*nsign)

   ! (i1,i2,i3) = boundary point
   ! (j1,j2,j3) = ghost point
   do i3=n3a+is3,n3b+is3
    j3=i3-is3
   do i2=n2a+is2,n2b+is2
    j2=i2-is2
   do i1=n1a+is1,n1b+is1
     if( mask(i1,i2,i3).gt.0 )then
      j1=i1-is1

    ! Note: the rhs for the mixed BC is stored in the ghost point value of f
    defineNeumannEquationForcing(n1a,n1b,n2a,n2b,n3a,n3b,FORCING,rectangular,DIR,DIM)


    #If #DIR eq "R"

      ! gdExact(ntd,nxd,nyd,nzd,x,y,z,n,t,value)
      ! call gdExact(0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,0,0.,ue0)
      ! call gdExact(0,1,2,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,0,0.,ue1)
      ! call gdExact(0,3,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,0,0.,ue2)
      ! call gdExact(0,1,2,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,0,0.,ue3)

      ! write(*,'(''bcopt: i1,i2,i3, x,y, g,ge, gss,gsse ='',3i3,2f8.2,5f11.6)') i1,i2,i3,xy(i1,i2,i3,0),xy(i1,i2,i3,1),g,ue0,gss,ue1
      ! write(*,'(''     : ffr, ffre ='',5f11.6)') ffr,ue2+ue3
      ! ********************** TEMP ******************
      ! g=ue0
      ! gss=ue1
      ! ffr=ue2+ue3
      ! ********************************************


      ur=( g - a0*u(i1,i2,i3) )/(a1*nsign)
      gb= ffr - (gss - a0*ff )/(a1*nsign) ! This is u_xxx + a0/(a1*nsign)*( u_xx )

      u(i1-is1,i2,i3) = (3.+br2*drn)/(3.-br2*drn)*u(i1+is1,i2,i3)-2*br2*drn/(3.-br2*drn)*u(i1,i2,i3)\
                        +nsign*(gb*drn**3+6*ur*drn)/(3.-br2*drn)
      u(i1-2*is1,i2,i3) = u(i1+2*is1,i2,i3) +16*br2*drn/(3.-br2*drn)*u(i1+is1,i2,i3)\
           -16*br2*drn/(3.-br2*drn)*u(i1,i2,i3)+nsign*(12*ur*drn**2*br2+12*ur*drn+8*gb*drn**3)/(3.-br2*drn)

      
      ! call gdExact(0,0,0,0,xy(i1-is1,i2,i3,0),xy(i1-is1,i2,i3,1),0.,0,0.,ue0)
      ! call gdExact(0,0,0,0,xy(i1-2*is1,i2,i3,0),xy(i1-2*is1,i2,i3,1),0.,0,0.,ue1)
      ! write(*,'(''     : u(-1),ue(-1), u(-2),ue(-2) ='',5f11.6)') u(i1-is1,i2,i3),ue0, u(i1-2*is1,i2,i3),ue1
      ! if( abs( u(i1-is1,i2,i3)-ue0).gt. .01 )then
      !  write(*,'(''   BCOPT:ERROR  : u(-1),ue(-1), u(-2),ue(-2) ='',5f11.6)')u(i1-is1,i2,i3),ue0, u(i1-2*is1,i2,i3),ue1
      ! else
      !  ! u(i1-is1,i2,i3)=ue0
      !  ! u(i1-2*is1,i2,i3) = ue1
      ! end if

    #Else
      us=( g - a0*u(i1,i2,i3) )/(a1*nsign)
      gb= ffs - (grr - a0*ff )/(a1*nsign) ! This is u_yyy + a0/(a1*nsign)*( u_yy )

      u(i1,i2-is2,i3) = (3.+br2*drn)/(3.-br2*drn)*u(i1,i2+is2,i3)-2*br2*drn/(3.-br2*drn)*u(i1,i2,i3)\
                        +nsign*(gb*drn**3+6*us*drn)/(3.-br2*drn)
      u(i1,i2-2*is2,i3) = u(i1,i2+2*is2,i3) +16*br2*drn/(3.-br2*drn)*u(i1,i2+is2,i3)\
           -16*br2*drn/(3.-br2*drn)*u(i1,i2,i3)+nsign*(12*us*drn**2*br2+12*us*drn+8*gb*drn**3)/(3.-br2*drn)


    #End
   

    ! for testing: 
    !  u(j1,j2,j3)=u(i1+is1,i2+is2,i3+is3)
    !  u(i1-2*is1,i2-2*is2,i3-2*is3)=u(i1+2*is1,i2+2*is2,i3+2*is3)

    #If #DIR eq "R"
     ! write(*,'(''bcopt: i1,i2,i3,g,ffr,gss,u,ur ='',3i3,6e12.3)') i1,i2,i3,g,ffr,gss,u(i1,i2,i3),ur,gb
    #Else
     ! write(*,'(''bcopt: i1,i2,i3,g,ffs,grr,u,us ='',3i3,6e12.3)') i1,i2,i3,g,ffs,grr,u(i1,i2,i3),us,gb
    #End
     ! write(*,'('' i1,i2,i3,f,f,f='',3i3,4e11.2)') i1,i2,i3,f(i1,i2,i3),f(i1+2*is1,i2+2*is2,i3),f(i1+is1,i2+is2,i3)
     ! write(*,'('' u(-1),u(-2)='',4e11.2)') u(j1,j2,j3),u(i1-2*is1,i2-2*is2,i3-2*is3)

    else if( mask(i1,i2,i3).lt.0 )then
      ! *wdh* 100609 -- extrap ghost outside interp 
     extrapolatePoint()
     extrapolateSecondGhostPoint()
     ! write(*,'("NeumEqn: extrap interp-ghost i1,i2=",2i3," u(-1),u(-2)=",2e12.4)') i1,i2,u(i1-  is1,i2-  is2,i3-  is3),u(i1-2*is1,i2-2*is2,i3-2*is3)
    end if
  end do
  end do
  end do



 else
   ! **** curvilinear case ****

   if( axis.gt.1 )then
     write(*,*) 'bcOpt:ERROR: this option not implemented yet'
     write(*,*) 'axis=',axis
     stop 12
   end if

   ! write(*,'("bcOpt:order4 neumannAndEqnEvenNewer (curv) grid=",i3," n1a,n1b,n2a,n2b=",4i3)') grid,n1a,n1b,n2a,n2b

   nsign = 2*side-1
   drn=dr(axis)
   cf1=3.*nsign
   alpha1=a1*nsign

   ! (i1,i2,i3) = boundary point
   ! (j1,j2,j3) = ghost point
   do i3=n3a+is3,n3b+is3
     j3=i3-is3
   do i2=n2a+is2,n2b+is2
     j2=i2-is2
   do i1=n1a+is1,n1b+is1
     if( mask(i1,i2,i3).gt.0 )then
      j1=i1-is1

    ! the rhs for the mixed BC is stored in the ghost point value of f
    defineNeumannEquationForcing(n1a,n1b,n2a,n2b,n3a,n3b,FORCING,curvilinear,DIR,DIM)

    #If #DIR eq "R"
      ! NOTE: *** The next include file only works for Laplace equation ***
      fourthOrderNeumannEquationBCNew(R,2)
      us=us4(i1,i2,i3)
      ! uss=uss2(i1,i2,i3) 
      usss=usss2(i1,i2,i3)
    #Elif #DIR eq "S"

      fourthOrderNeumannEquationBCNew(S,2)

      ur=ur4(i1,i2,i3)
      ! urr=urr2(i1,i2,i3)  ! need to 2nd-order
      urrr=urrr2(i1,i2,i3)
    #Else
      stop 7
    #End


! *****************
!$$$    ! gdExact(ntd,nxd,nyd,nzd,x,y,z,n,t,value)
!$$$    call gdExact(0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,0,0.,ue0)
!$$$    
!$$$    call gdExact(0,0,0,0,xy(i1-2,i2,i3,0),xy(i1-2,i2,i3,1),0.,0,0.,ue1)
!$$$    call gdExact(0,0,0,0,xy(i1-1,i2,i3,0),xy(i1-1,i2,i3,1),0.,0,0.,ue2)
!$$$    call gdExact(0,0,0,0,xy(i1+1,i2,i3,0),xy(i1+1,i2,i3,1),0.,0,0.,ue3)
!$$$    call gdExact(0,0,0,0,xy(i1+2,i2,i3,0),xy(i1+2,i2,i3,1),0.,0,0.,ue4)
!$$$
!$$$
!$$$    ure   = (ue1   -8.*ue2  +8.*ue3 -ue4)/(12.*dr(0))
!$$$    urre=(-ue1 +16.*ue2 -30.*ue0+16.*ue3 -ue4 )/(12.*dr(0)**2)
!$$$    urrre = (-ue1+2.*ue2-2.*ue3+ue4)/(2.*dr(0)**3)
!$$$
!$$$
!$$$    call gdExact(0,0,0,0,xy(i1,i2-2,i3,0),xy(i1,i2-2,i3,1),0.,0,0.,ue1)
!$$$    call gdExact(0,0,0,0,xy(i1,i2-1,i3,0),xy(i1,i2-1,i3,1),0.,0,0.,ue2)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+1,i3,0),xy(i1,i2+1,i3,1),0.,0,0.,ue3)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+2,i3,0),xy(i1,i2+2,i3,1),0.,0,0.,ue4)
!$$$
!$$$    use=(ue1   -8.*ue2  +8.*ue3 -ue4)/(12.*dr(1))
!$$$    usse=(-ue1 +16.*ue2 -30.*ue0+16.*ue3 -ue4 )/(12.*dr(1)**2)
!$$$    ussse=(-ue1+2.*ue2-2.*ue3+ue4)/(2.*dr(1)**3)
!$$$
!$$$
!$$$    call gdExact(0,0,0,0,xy(i1,i2-1,i3,0),xy(i1,i2-1,i3,1),0.,0,0.,ue1)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+1,i3,0),xy(i1,i2-1,i3,1),0.,0,0.,ue2)
!$$$    call gdExact(0,0,0,0,xy(i1,i2-1,i3,0),xy(i1,i2+1,i3,1),0.,0,0.,ue3)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+1,i3,0),xy(i1,i2+1,i3,1),0.,0,0.,ue4)
!$$$    urse=(ue4-ue3-ue2+ue1)/(4.*dr(0)*dr(1))
! *****************

    #If #DIR eq "R"
      ur= (g - an2*us -a0*u(i1,i2,i3))/an1
      gb=b0*u(i1,i2,i3)+b1*us+b3*usss+bf ! this is really: urrr+br2*urr

  ! write(*,'("bcopt:side,is1=",i2,i3," i1,i2,i3=",3i3," b0,b1,b3,bf,br2 =",6f10.4)') side,is1,i1,i2,i3,b0,b1,b3,bf,br2
  ! write(*,'("      g,gr,grr,ff,ffr,ffs    =",6f11.6)') g,gr,grr,ff,ffr,ffs
  ! write(*,'("      an1,an2,an1r,an2r,an2rr  =",5f10.6)') an1,an2,an1r,an2r,an2rr

!   urrree=b0*u(i1,i2,i3)+b1*use+b2*usse+b3*ussse+bf
!
!    write(*,'("  (x,y)=(",f6.3,",",f6.3,")  ur=",e11.3," exact=",e11.3," gb =",e11.3," exact=",e11.3," urrree=",e11.3)')\
!               xy(i1,i2,i3,0),xy(i1,i2,i3,1),ur,ure,gb,urrre,urrree
!    write(*,'("  us=",e11.3," use=",e11.3," uss =",e11.3," usse=",e11.3," usss=",e11.3," ussse=",e11.3)') \
!     us,use,uss,usse,usss,ussse
!
!    urrf=( ff - (c12*urse +c22*usse + c1*ure + c2*use) )/c11
!
!    write(*,'(" c11,c12,c22,c1,c2=",5f10.3,"  urre=",e11.3," urrf=[ff- (c12*urs+...)]/c11=",e11.3') c11,c12,c22,c1,c2,urre,urrf
!    write(*,'(" rxx,ryy,sxx,syy,rxr,rxs,ryr,rys=",8f10.3') rxx,ryy,sxx,syy,rxr,rxs,ryr,rys


      ! write(*,'("bcopt:side=",i2," i1,i2,i3=",3i3," b0,b1,b3,bf,br2 =",6f10.4)') side,i1,i2,i3,b0,b1,b3,bf,br2
 
      ! On side==1 we change the sign of br2 and the forcing
      br2=-nsign*br2

      u(i1-is1,i2,i3) = (3.+br2*drn)/(3.-br2*drn)*u(i1+is1,i2,i3)-2*br2*drn/(3.-br2*drn)*u(i1,i2,i3)\
                        +nsign*(gb*drn**3+6*ur*drn)/(3.-br2*drn)
      u(i1-2*is1,i2,i3) = u(i1+2*is1,i2,i3) +16*br2*drn/(3.-br2*drn)*u(i1+is1,i2,i3)\
           -16*br2*drn/(3.-br2*drn)*u(i1,i2,i3)+nsign*(12*ur*drn**2*br2+12*ur*drn+8*gb*drn**3)/(3.-br2*drn)

    #Else

      us= (g - an1*ur -a0*u(i1,i2,i3))/an2
      gb=b0*u(i1,i2,i3)+b1*ur+b3*urrr+bf ! this is: usss+br2*uss

      ! On side==1 we change the sign of br2 and the forcing
      br2=-nsign*br2

      u(i1,i2-is2,i3) = (3.+br2*drn)/(3.-br2*drn)*u(i1,i2+is2,i3)-2*br2*drn/(3.-br2*drn)*u(i1,i2,i3)\
                        +nsign*(gb*drn**3+6*us*drn)/(3.-br2*drn)
      u(i1,i2-2*is2,i3) = u(i1,i2+2*is2,i3) +16*br2*drn/(3.-br2*drn)*u(i1,i2+is2,i3)\
           -16*br2*drn/(3.-br2*drn)*u(i1,i2,i3)+nsign*(12*us*drn**2*br2+12*us*drn+8*gb*drn**3)/(3.-br2*drn)

!   usssee=b0*u(i1,i2,i3)+b1*ure+b2*urre+b3*urrre+bf

  ! write(*,'(''bcopt: i1,i2,i3,g,ffs,grr,u,us ='',3i3,6e12.3)') i1,i2,i3,g,ffs,grr,u(i1,i2,i3),us,gb
  ! write(*,'('' u(-1),u(-2)='',4e11.2)') u(i1,i2-is2,i3),u(i1,i2-2*is2,i3)

!  write(*,'(''bcopt:side='',i2,'' i1,i2,i3,g,ff ='',3i3,2f10.4)') side,i1,i2,i3,g,ff
!  write(*,'("  (x,y)=(",f6.3,",",f6.3,")  us=",e11.3," use=",e11.3," usss =",e11.3," ussse=",e11.3," usssee=",e11.3)')\
!            xy(i1,i2,i3,0),xy(i1,i2,i3,1),us,use,usss,ussse,usssee
!  write(*,'("  ur=",e11.3," ure=",e11.3," urr =",e11.3," urre=",e11.3," urrr=",e11.3," urrre=",e11.3)') \
!     ur,ure,urr,urre,urrr,urrre
!  ussf=( ff - (c12*urse +c11*urre + c1*ure + c2*use) )/c22
!  write(*,'(" c11,c12,c22,c1,c2=",5f9.3,"  usse=",e11.3," ussf=[ff- (c12*urs+...)]/c22=",e11.3') c11,c12,c22,c1,c2,usse,ussf
!    write(*,'(" rxx,ryy,sxx,syy,rxr,rxs,ryr,rys=",8f10.3') rxx,ryy,sxx,syy,rxr,rxs,ryr,rys

!  usss=ussse


!**      u(i1-is1,i2-is2,i3-is3)=u(i1+is1,i2+is2,i3+is3)+cf1*usss+cg1*us
!**      u(i1-2*is1,i2-2*is2,i3-2*is3)=u(i1+2*is1,i2+2*is2,i3+2*is3)+cf2*usss+cg2*us



!   call gdExact(0,0,0,0,xy(i1,i2-  is2,i3,0),xy(i1,i2-  is2,i3,1),0.,0,0.,ue1)
!   call gdExact(0,0,0,0,xy(i1,i2-2*is2,i3,0),xy(i1,i2-2*is2,i3,1),0.,0,0.,ue2)
!    u(i1,i2-is2,i3) = ue1
!    u(i1,i2-2*is2,i3) = ue2

!    write(*,'("bcopt:side,is2=",i2,i3," i1,i2,i3=",3i3," b0,b1,b3,bf,br2 =",6f10.4)') side,is2,i1,i2,i3,b0,b1,b3,bf,br2
!    write(*,'("      ue1,ue2,u(i1,i2-is2,i3),u(i1,i2-2*is2,i3)=",6f10.4)') ue1,ue2,u(i1,i2-is2,i3),u(i1,i2-2*is2,i3)
!    ue3=nsign*(gb*drn**3+6*us*drn)/(3.-br2*drn)
!    ue4=nsign*(12*us*drn**2*br2+12*us*drn+8*gb*drn**3)/(3.-br2*drn)
!    write(*,'("      fa1,fa2,us,gb,drn=",5f10.4)') ue3,ue4,us,gb,drn
!    write(*,'("      g,gr,grr,ff,ffr,ffs    =",6f11.6)') g,gr,grr,ff,ffr,ffs
!    write(*,'("      an1,an2,an1r,an2r,an2rr  =",5f10.6)') an1,an2,an1r,an2r,an2rr

!   call gdExact(0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,0,0.,ue0)
!   call gdExact(0,0,0,0,xy(i1,i2-2,i3,0),xy(i1,i2-2,i3,1),0.,0,0.,ue1)
!   call gdExact(0,0,0,0,xy(i1,i2-1,i3,0),xy(i1,i2-1,i3,1),0.,0,0.,ue2)
!   call gdExact(0,0,0,0,xy(i1,i2+1,i3,0),xy(i1,i2+1,i3,1),0.,0,0.,ue3)
!   call gdExact(0,0,0,0,xy(i1,i2+2,i3,0),xy(i1,i2+2,i3,1),0.,0,0.,ue4)

!   use=(ue1   -8.*ue2  +8.*ue3 -ue4)/(12.*dr(1))
!   usse=(-ue1 +16.*ue2 -30.*ue0+16.*ue3 -ue4 )/(12.*dr(1)**2)
!   ussse=(-ue1+2.*ue2-2.*ue3+ue4)/(2.*dr(1)**3)

!  write(*,'("   use,usse=",2f10.5," ussse,gb-br2*usse  =",5f10.5)') use,usse,ussse,gb-br2*usse

    #End

    else if( mask(i1,i2,i3).lt.0 )then
      ! *wdh* 100609 -- extrap ghost outside interp 
     extrapolatePoint()
     extrapolateSecondGhostPoint()
     ! write(*,'("NeumEqn: extrap interp-ghost i1,i2=",2i3," u(-1),u(-2)=",2e12.4)') i1,i2,u(i1-  is1,i2-  is2,i3-  is3),u(i1-2*is1,i2-2*is2,i3-2*is3)
    end if
  end do
  end do
  end do

 end if
#endMacro








!***************************EVEN NEWER VERSION consistent with lineSmoothOpt.bf *******************
! Define the fourth-order accurate neumann BC which uses the normal derivative of the equation on the
! boundary PLUS solve for the PDE on the boundary
#beginMacro neumannPDE(FORCING,DIR,DIM)
 if( gridType.eq.rectangular )then
   if( a1.eq.0. )then
     write(*,*) 'bcOpt:ERROR: a1=0!'
     stop 2
   end if
   
!   write(*,'(''bcOpt:4th-order neumannPDE (rect) n2a,n2b='',2i3)') n2a,n2b

   drn=dx(axis)
   nsign = 2*side-1
   br2=-nsign*a0/(a1*nsign)

   ! (i1,i2,i3) = boundary point
   ! (j1,j2,j3) = ghost point
   do i3=n3a+is3,n3b+is3
    j3=i3-is3
   do i2=n2a+is2,n2b+is2
    j2=i2-is2
   do i1=n1a+is1,n1b+is1
     if( mask(i1,i2,i3).gt.0 )then
      j1=i1-is1

    ! the rhs for the mixed BC is stored in the ghost point value of 
    defineNeumannEquationForcing(n1a,n1b,n2a,n2b,n3a,n3b,FORCING,rectangular,DIR,DIM)


    #If #DIR eq "R"
      ur=( g - a0*u(i1,i2,i3) )/(a1*nsign)
      gb= ffr - (gss - a0*ff )/(a1*nsign) ! This is u_xxx + a0/(a1*nsign)*( u_xx )

      u(i1-is1,i2,i3) = (3.+br2*drn)/(3.-br2*drn)*u(i1+is1,i2,i3)-2*br2*drn/(3.-br2*drn)*u(i1,i2,i3)\
                        +nsign*(gb*drn**3+6*ur*drn)/(3.-br2*drn)
      u(i1-2*is1,i2,i3) = u(i1+2*is1,i2,i3) +16*br2*drn/(3.-br2*drn)*u(i1+is1,i2,i3)\
           -16*br2*drn/(3.-br2*drn)*u(i1,i2,i3)+nsign*(12*ur*drn**2*br2+12*ur*drn+8*gb*drn**3)/(3.-br2*drn)

    #Else
      us=( g - a0*u(i1,i2,i3) )/(a1*nsign)
      gb= ffs - (grr - a0*ff )/(a1*nsign) ! This is u_yyy + a0/(a1*nsign)*( u_yy )

      u(i1,i2-is2,i3) = (3.+br2*drn)/(3.-br2*drn)*u(i1,i2+is2,i3)-2*br2*drn/(3.-br2*drn)*u(i1,i2,i3)\
                        +nsign*(gb*drn**3+6*us*drn)/(3.-br2*drn)
      u(i1,i2-2*is2,i3) = u(i1,i2+2*is2,i3) +16*br2*drn/(3.-br2*drn)*u(i1,i2+is2,i3)\
           -16*br2*drn/(3.-br2*drn)*u(i1,i2,i3)+nsign*(12*us*drn**2*br2+12*us*drn+8*gb*drn**3)/(3.-br2*drn)


    #End
   
!   write(*,'(''bcopt: i1,i2,i3,g,ffr,gss,u,uss ='',3i3,5f11.6)') i1,i2,i3,g,ffr,gss,u(i1,i2,i3),uss
!  write(*,'('' i1,i2,i3,ur,urrr,ffr,gss ='',3i3,4e11.2)') i1,i2,i3,ur,urrr,ffr,gss
!  write(*,'('' i1,i2,i3,f,f,f='',3i3,4e11.2)') i1,i2,i3,f(i1,i2,i3),f(i1+2*is1,i2+2*is2,i3),f(i1+is1,i2+is2,i3)
!      u(j1,j2,j3)=u(i1+is1,i2+is2,i3+is3)
!      u(i1-2*is1,i2-2*is2,i3-2*is3)=u(i1+2*is1,i2+2*is2,i3+2*is3)

    else if( mask(i1,i2,i3).lt.0 )then
      ! *wdh* 100609 -- extrap ghost outside interp 
     extrapolatePoint()
     extrapolateSecondGhostPoint()
    end if
  end do
  end do
  end do



 else
   ! **** curvilinear case ****

   if( axis.gt.1 )then
     write(*,*) 'bcOpt:ERROR: this option not implemented yet'
     write(*,*) 'axis=',axis
     stop 12
   end if

   ! write(*,*) 'bcOpt:4th-order neumann (curvilinear- DIR)'

   nsign = 2*side-1
   drn=dr(axis)
   cf1=3.*nsign
   alpha1=a1*nsign

   ma = m33-2*is1 - 2*5*is2  ! 2nd ghost
   mb = m33-  is1 - 5*is2    ! 1st ghost
   mc = m33                  ! diagonal

   ! (i1,i2,i3) = boundary point
   ! (j1,j2,j3) = ghost point
   do i3=n3a+is3,n3b+is3
     j3=i3-is3
   do i2=n2a+is2,n2b+is2
     j2=i2-is2
   do i1=n1a+is1,n1b+is1
     if( mask(i1,i2,i3).gt.0 )then
      j1=i1-is1

    ! the rhs for the mixed BC is stored in the ghost point value of f
    defineNeumannEquationForcing(n1a,n1b,n2a,n2b,n3a,n3b,FORCING,curvilinear,DIR,DIM)

    #If #DIR eq "R"
      fourthOrderNeumannEquationBCNew(R,2)
      us=us4(i1,i2,i3)
      ! uss=uss2(i1,i2,i3) 
      usss=usss2(i1,i2,i3)
    #Elif #DIR eq "S"

      fourthOrderNeumannEquationBCNew(S,2)

      ur=ur4(i1,i2,i3)
      ! urr=urr2(i1,i2,i3)  ! need to 2nd-order
      urrr=urrr2(i1,i2,i3)
    #Else
      stop 7
    #End


! *****************
!$$$    ! gdExact(ntd,nxd,nyd,nzd,x,y,z,n,t,value)
!$$$    call gdExact(0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,0,0.,ue0)
!$$$    
!$$$    call gdExact(0,0,0,0,xy(i1-2,i2,i3,0),xy(i1-2,i2,i3,1),0.,0,0.,ue1)
!$$$    call gdExact(0,0,0,0,xy(i1-1,i2,i3,0),xy(i1-1,i2,i3,1),0.,0,0.,ue2)
!$$$    call gdExact(0,0,0,0,xy(i1+1,i2,i3,0),xy(i1+1,i2,i3,1),0.,0,0.,ue3)
!$$$    call gdExact(0,0,0,0,xy(i1+2,i2,i3,0),xy(i1+2,i2,i3,1),0.,0,0.,ue4)
!$$$
!$$$
!$$$    ure   = (ue1   -8.*ue2  +8.*ue3 -ue4)/(12.*dr(0))
!$$$    urre=(-ue1 +16.*ue2 -30.*ue0+16.*ue3 -ue4 )/(12.*dr(0)**2)
!$$$    urrre = (-ue1+2.*ue2-2.*ue3+ue4)/(2.*dr(0)**3)
!$$$
!$$$
!$$$    call gdExact(0,0,0,0,xy(i1,i2-2,i3,0),xy(i1,i2-2,i3,1),0.,0,0.,ue1)
!$$$    call gdExact(0,0,0,0,xy(i1,i2-1,i3,0),xy(i1,i2-1,i3,1),0.,0,0.,ue2)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+1,i3,0),xy(i1,i2+1,i3,1),0.,0,0.,ue3)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+2,i3,0),xy(i1,i2+2,i3,1),0.,0,0.,ue4)
!$$$
!$$$    use=(ue1   -8.*ue2  +8.*ue3 -ue4)/(12.*dr(1))
!$$$    usse=(-ue1 +16.*ue2 -30.*ue0+16.*ue3 -ue4 )/(12.*dr(1)**2)
!$$$    ussse=(-ue1+2.*ue2-2.*ue3+ue4)/(2.*dr(1)**3)
!$$$
!$$$
!$$$    call gdExact(0,0,0,0,xy(i1,i2-1,i3,0),xy(i1,i2-1,i3,1),0.,0,0.,ue1)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+1,i3,0),xy(i1,i2-1,i3,1),0.,0,0.,ue2)
!$$$    call gdExact(0,0,0,0,xy(i1,i2-1,i3,0),xy(i1,i2+1,i3,1),0.,0,0.,ue3)
!$$$    call gdExact(0,0,0,0,xy(i1,i2+1,i3,0),xy(i1,i2+1,i3,1),0.,0,0.,ue4)
!$$$    urse=(ue4-ue3-ue2+ue1)/(4.*dr(0)*dr(1))
! *****************


    ! evaulate the defect when the 2-ghost pts and bndry pt are zero:
    u(i1,i2,i3)=0.
    u(i1-is1,i2-is2,i3)=0.
    u(i1-2*is1,i2-2*is2,i3)=0.
    gc=ff-op2d4(i1,i2,i3)
    ca=c(ma,i1,i2,i3)
    cb=c(mb,i1,i2,i3)
    cc=c(mc,i1,i2,i3)

    #If #DIR eq "R"
      ga= (g - an2*us -a0*u(i1,i2,i3))/an1  ! ur
      gb=b0*u(i1,i2,i3)+b1*us+b3*usss+bf ! this is really: urrr+br2*urr


      ! write(*,'("bcopt:side=",i2," i1,i2,i3=",3i3," b0,b1,b3,bf,br2 =",6f10.4)') side,i1,i2,i3,b0,b1,b3,bf,br2
 
      ! On side==1 we change the sign of br2 and the forcing
      br2=-nsign*br2
      ga=-nsign*ga
      gb=-nsign*gb

      u(i1-is1,i2,i3) = -(3*cc-16*br2*drn*ca+cc*br2*drn)/(-3*cc+cc*br2*drn+16*br2*drn*ca+2*br2*drn*cb)*u(i1+is1,i2,i3)-2*br2*drn*ca/(-3*cc+cc*br2*drn+16*br2*drn*ca+2*br2*drn*cb)*u(i1+2*is1,i2,i3)-(24*br2*drn**2*ca*ga-cc*gb*drn**3-6*cc*ga*drn-2*br2*drn*gc)/(-3*cc+cc*br2*drn+16*br2*drn*ca+2*br2*drn*cb)

      u(i1-2*is1,i2,i3) = (-16*cc*br2*drn-16*br2*drn*cb)/(-3*cc+cc*br2*drn+16*br2*drn*ca+2*br2*drn*cb)*u(i1+is1,i2,i3)+(-3*cc+cc*br2*drn+2*br2*drn*cb)/(-3*cc+cc*br2*drn+16*br2*drn*ca+2*br2*drn*cb)*u(i1+2*is1,i2,i3)+(16*br2*drn*gc+24*ga*drn**2*br2*cb+8*cc*gb*drn**3+12*cc*ga*drn+12*ga*drn**2*cc*br2)/(-3*cc+cc*br2*drn+16*br2*drn*ca+2*br2*drn*cb)

      u(i1,i2,i3) = -cb/cc*u(i1-is1,i2,i3)-ca/cc*u(i1-2*is1,i2,i3)+gc/cc

!     write(*,'("neumannPDE:side=",i2," i1,i2,i3=",3i3," b0,b1,b3,bf,br2 =",6f10.4)') side,i1,i2,i3,b0,b1,b3,bf,br2
!     write(*,'(" ca,cb,cc=",3e11.2,"   after: res(PDE) = ",6e10.2)') ca,cb,cc,op2d4(i1,i2,i3)-ff


    #Else

      ga= (g - an1*ur -a0*u(i1,i2,i3))/an2  ! us
      gb=b0*u(i1,i2,i3)+b1*ur+b3*urrr+bf ! this is: usss+br2*uss

      ! On side==1 we change the sign of br2 and the forcing
      br2=-nsign*br2
      ga=-nsign*ga
      gb=-nsign*gb

      u(i1,i2-is2,i3) = -(3*cc-16*br2*drn*ca+cc*br2*drn)/(-3*cc+cc*br2*drn+16*br2*drn*ca+2*br2*drn*cb)*u(i1,i2+is2,i3)-2*br2*drn*ca/(-3*cc+cc*br2*drn+16*br2*drn*ca+2*br2*drn*cb)*u(i1,i2+2*is2,i3)-(24*br2*drn**2*ca*ga-cc*gb*drn**3-6*cc*ga*drn-2*br2*drn*gc)/(-3*cc+cc*br2*drn+16*br2*drn*ca+2*br2*drn*cb)

      u(i1,i2-2*is2,i3) = (-16*cc*br2*drn-16*br2*drn*cb)/(-3*cc+cc*br2*drn+16*br2*drn*ca+2*br2*drn*cb)*u(i1,i2+is2,i3)+(-3*cc+cc*br2*drn+2*br2*drn*cb)/(-3*cc+cc*br2*drn+16*br2*drn*ca+2*br2*drn*cb)*u(i1,i2+2*is2,i3)+(16*br2*drn*gc+24*ga*drn**2*br2*cb+8*cc*gb*drn**3+12*cc*ga*drn+12*ga*drn**2*cc*br2)/(-3*cc+cc*br2*drn+16*br2*drn*ca+2*br2*drn*cb)

      u(i1,i2,i3) = -cb/cc*u(i1,i2-is2,i3)-ca/cc*u(i1,i2-2*is2,i3)+gc/cc

    #End

    else if( mask(i1,i2,i3).lt.0 )then
      ! *wdh* 100609 -- extrap ghost outside interp 
     extrapolatePoint()
     extrapolateSecondGhostPoint()
    end if
  end do
  end do
  end do

 end if
#endMacro












! =====================================================================================================
! Apply the Mixed BC to second order on the two ghost lines -- this will reduce to an even symmetry
!  condition on a Cartesian grid if a0==0 and there is no forcing
!
! We discretize the following BC to second order: 
! 
! a1*( n1*ux + n2*ux + n3*uz ) + a0*u = f 
! a1*( (n1*rx+n2*ry+n3*rz)*ur + (n1*sx+n2*sy+n3*sz)*us + (n1*tx+n2*ty+n3*st)*ut ) + a0*u = f 
!
! On the second ghost line we use a wide formula in the normal direction: e.g.
!           ur = (u(i1+2,i2,i3)-u(i1-2,i2,i3))/(4*dr(0)) 
!
! GRIDTYPE = rectangular or curvilinear
! DIM = 2 or 3
! DIR = r or s or t
! FORCING: forcing or noForcing  
! 
! =====================================================================================================
#beginMacro assignMixedToSecondOrder(GRIDTYPE,DIM,DIR,FORCING)
! write(*,'(''AssignMixedToSecondOrder: side,axis='',2i2)') side,axis
! write(*,'('' n1a,n1b,...'',6i3)') n1a,n1b,n2a,n2b,n3a,n3b
! write(*,'('' a0,a1,dx(axis)='',3f6.2)') a0,a1,dx(axis)
#If #GRIDTYPE == "curvilinear"
! write(*,'('' a0,a1,dr,dim=DIM,(dir=DIR)='',3f6.2)') a0,a1,dr(axis)
#End

#If #FORCING == "forcing" 
  ! The forcing for the Neumann equation is on the first ghost line 
  #defineMacro force(i1,i2,i3) (f(i1-is1,i2-is2,i3-is3)-a0*u(i1,i2,i3))
#Else
  #defineMacro force(i1,i2,i3) (-a0*u(i1,i2,i3))
#End

 is = 1-2*side

 ! (i1,i2,i3) = boundary point since [n1a,n1b][n2a,n2b]... defines the ghost line
 do i3=n3a+is3,n3b+is3
 do i2=n2a+is2,n2b+is2
 do i1=n1a+is1,n1b+is1
  if( mask(i1,i2,i3).ne.0 )then
 
   #If #GRIDTYPE == "rectangular"
     ! Cartesian: 
     t1=(-is)*(2.*dx(axis)/a1)*force(i1,i2,i3)
   #Else 
     ! Curvilinear:
     ! (an1,an2,an3) is the outward normal
    an1 = DIR ## x(i1,i2,i3)
    an2 = DIR ## y(i1,i2,i3)
    #If #DIM == "2" 
     aNormi = (-is)/sqrt(an1**2+an2**2)
     an1=an1*aNormi
     an2=an2*aNormi
    #Else
     an3 = DIR ## z(i1,i2,i3)
     aNormi = (-is)/sqrt(an1**2+an2**2+an3**2)
     an1=an1*aNormi
     an2=an2*aNormi
     an3=an3*aNormi
    #End

    #If #DIM == "2" 
     ! -- 2D --
     #If #DIR == "r"
       t1=(2.*dr(axis)/(an1*rx(i1,i2,i3)+an2*ry(i1,i2,i3)))*( force(i1,i2,i3)/a1 \
             - (an1*sx(i1,i2,i3)+an2*sy(i1,i2,i3))*(u(i1,i2+1,i3)-u(i1,i2-1,i3))/(2.*dr(1)) )
     #Elif #DIR eq "s"
       t1=(2.*dr(axis)/(an1*sx(i1,i2,i3)+an2*sy(i1,i2,i3)))*( force(i1,i2,i3)/a1 \
             - (an1*rx(i1,i2,i3)+an2*ry(i1,i2,i3))*(u(i1+1,i2,i3)-u(i1-1,i2,i3))/(2.*dr(0)) )
     #Else
      stop 2276
     #End

    #Else
     ! -- 3D ---
     #If #DIR == "r"
       t1=(2.*dr(axis)/(an1*rx(i1,i2,i3)+an2*ry(i1,i2,i3)+an3*rz(i1,i2,i3)))*( force(i1,i2,i3)/a1 \
             - (an1*sx(i1,i2,i3)+an2*sy(i1,i2,i3)+an3*sz(i1,i2,i3))*(u(i1,i2+1,i3)-u(i1,i2-1,i3))/(2.*dr(1)) \
             - (an1*tx(i1,i2,i3)+an2*ty(i1,i2,i3)+an3*tz(i1,i2,i3))*(u(i1,i2,i3+1)-u(i1,i2,i3-1))/(2.*dr(2)) )
     #Elif #DIR == "s" 
       t1=(2.*dr(axis)/(an1*sx(i1,i2,i3)+an2*sy(i1,i2,i3)+an3*sz(i1,i2,i3)))*( force(i1,i2,i3)/a1 \
             - (an1*rx(i1,i2,i3)+an2*ry(i1,i2,i3)+an3*rz(i1,i2,i3))*(u(i1+1,i2,i3)-u(i1-1,i2,i3))/(2.*dr(0)) \
             - (an1*tx(i1,i2,i3)+an2*ty(i1,i2,i3)+an3*tz(i1,i2,i3))*(u(i1,i2,i3+1)-u(i1,i2,i3-1))/(2.*dr(2)) )
     #Elif #DIR == "t"
       t1=(2.*dr(axis)/(an1*tx(i1,i2,i3)+an2*ty(i1,i2,i3)+an3*tz(i1,i2,i3)))*( force(i1,i2,i3)/a1 \
             - (an1*rx(i1,i2,i3)+an2*ry(i1,i2,i3)+an3*rz(i1,i2,i3))*(u(i1+1,i2,i3)-u(i1-1,i2,i3))/(2.*dr(0)) \
             - (an1*sx(i1,i2,i3)+an2*sy(i1,i2,i3)+an3*sz(i1,i2,i3))*(u(i1,i2+1,i3)-u(i1,i2-1,i3))/(2.*dr(1)) )
     #Else
       stop 2277
     #End
      
    #End
   #End 

   u(i1-  is1,i2-  is2,i3-  is3)=u(i1+  is1,i2+  is2,i3+  is3)-   t1*is
   if( orderOfAccuracy.eq.4 )then
     u(i1-2*is1,i2-2*is2,i3-2*is3)=u(i1+2*is1,i2+2*is2,i3+2*is3)-2.*t1*is
   end if 
  end if
 end do
 end do
 end do

#endMacro

! =====================================================================================================
!  Apply an odd symmetry BC
! =====================================================================================================
#beginMacro assignOddSymmetry()
 do i3=n3a+is3,n3b+is3
 do i2=n2a+is2,n2b+is2
 do i1=n1a+is1,n1b+is1
   if( mask(i1,i2,i3).ne.0 )then
     u(i1-  is1,i2-  is2,i3-  is3)=2.*u(i1,i2,i3)-u(i1+  is1,i2+  is2,i3+  is3)
     u(i1-2*is1,i2-2*is2,i3-2*is3)=2.*u(i1,i2,i3)-u(i1+2*is1,i2+2*is2,i3+2*is3)
   end if
 end do
 end do
 end do

#endMacro


! =====================================================================================================
!  Apply a Neumann or mixed BC to second order on a rectangular grid
!           (a0 + a1 n.grad) u = f
! =====================================================================================================
#beginMacro neumann2ndOrderRectangular()
 
 ! write(*,'("bcOpt:neumann2ndOrderRectangular: grid=",i4," level=",i2," side,axis=",2i2," useForcing=",i2)') grid,level,side,axis,useForcing
 ! write(*,'(" a0,a1,dx(axis)=",3f6.2)') a0,a1,dx(axis)
 ! write(*,'(" n1a,n1b,...",6i3," is1,is2,is3=",3i2)') n1a,n1b,n2a,n2b,n3a,n3b,is1,is2,is3

 ! On a Cartesian grid the Mixed condition is:
 !  a1*nSign*(u(1)-u(-1))/(2*dx) + a0*u = f 
 !   nSign = 2*side-1
 if( useForcing.eq.1 )then
  beginLoops()
   if( mask(i1,i2,i3).gt.0 )then
     u(i1-is1,i2-is2,i3-is3)=u(i1+is1,i2+is2,i3+is3) - (a0*u(i1,i2,i3)-f(i1-is1,i2-is2,i3-is3))*(2.*dx(axis)/a1)
     ! write(*,'(" i1,i2=",2i3," f=",e10.2," u(-1)=",e10.2)') i1,i2,f(i1-is1,i2-is2,i3-is3),u(i1-is1,i2-is2,i3-is3)
   else if( mask(i1,i2,i3).lt.0 )then
     ! should we use 2nd or 3rd order extrap here?
     u(i1-is1,i2-is2,i3-is3)=extrapolate3(u,i1,i2,i3,is1,is2,is3)
     ! write(*,'("bcOpt:neumann:extrap ighost:  i1,i2=",2i3," u(-1)=",e12.4)') i1,i2,u(i1-is1,i2-is2,i3-is3)
   end if
  endLoops()
 else
  beginLoops()
   if( mask(i1,i2,i3).gt.0 )then
     u(i1-is1,i2-is2,i3-is3)=u(i1+is1,i2+is2,i3+is3) - (a0*u(i1,i2,i3)            )*(2.*dx(axis)/a1)
   else if( mask(i1,i2,i3).lt.0 )then
     u(i1-is1,i2-is2,i3-is3)=extrapolate2(u,i1,i2,i3,is1,is2,is3)
   end if
  endLoops()
 end if
#endMacro


      subroutine bcOpt( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    n1a,n1b,n2a,n2b,n3a,n3b, ndc, c, u,f,mask,rsxy, xy,
     &    bc, boundaryCondition, ipar, rpar )
! ===================================================================================
!  Optimised Boundary conditions.
!
!  n1a,n1b,n2a,n2b,n3a,n3b :  
!     "extrapolate" : indicies of points on the boundary.
!     Neumann/mixed : indicies of points on the first GHOST line (why did I do this?)
!
!  useCoefficients: 1=use the c array.
!  gridType : 0=rectangular, 1=curvilinear
!  useForcing : 1=use f for RHS to BC
!  side,axis : 0:1 and 0:2
! ===================================================================================

      implicit none

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real c(1:ndc,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)

      integer ipar(0:*),boundaryCondition(0:1,0:2)
      real rpar(0:*)


!     --- local variables ----
      
      real ue1,ue2,ue3,ue4,ure,urrre
      real ue0,use,usse,ussse,urrree
      real urre,urrf,urse,usssee,ussf


      integer is,is1,is2,is3,orderOfAccuracy,gridType,level,debug,myid,
     &        side,axis,useForcing,bcOption4,solveEquationWithBC,grid,isNeumannBC
      integer m1a,m1b,m2a,m2b,m3a,m3b,nn
      integer i1m1,i1p1,i2m1,i2p1,i3m1,i3p1
      real dr(0:2), dx(0:2)

      real dxn,t1
      real nsign,cd,cg,cf,ga,gb,gc

      integer useCoefficients,orderOfExtrapolation,orderOfExtrapolationForNeumann
      integer dirichlet,neumann,mixed,equation,extrapolation,
     &        combination,equationToSecondOrder,mixedToSecondOrder,
     &        evenSymmetry,oddSymmetry,extrapolateTwoGhostLines
      parameter( 
     &     dirichlet=1,
     &     neumann=2,
     &     mixed=3,
     &     equation=4,
     &     extrapolation=5,
     &     combination=6,
     &     equationToSecondOrder=7,
     &     mixedToSecondOrder=8,
     &     evenSymmetry=9,
     &     oddSymmetry=10,
     &     extrapolateTwoGhostLines=11 )

      integer rectangular,curvilinear
      parameter(
     &     rectangular=0,
     &     curvilinear=1)

       integer equationToSolve
       integer userDefined,laplaceEquation,divScalarGradOperator,
     &  heatEquationOperator,variableHeatEquationOperator,
     &   divScalarGradHeatOperator,secondOrderConstantCoefficients,
     & axisymmetricLaplaceEquation
      parameter(
     & userDefined=0,
     & laplaceEquation=1,
     & divScalarGradOperator=2,              ! div[ s[x] grad ]
     & heatEquationOperator=3,               ! I + c0*Delta
     & variableHeatEquationOperator=4,       ! I + s[x]*Delta
     & divScalarGradHeatOperator=5,  ! I + div[ s[x] grad ]
     & secondOrderConstantCoefficients=6,
     & axisymmetricLaplaceEquation=7 )

      integer dirichletFirstGhostLineBC,neumannFirstGhostLineBC
      integer dirichletSecondGhostLineBC,neumannSecondGhostLineBC
      integer useSymmetry,useEquationToFourthOrder,
     & useEquationToSecondOrder,useExtrapolation
      parameter(
     &  useSymmetry=0,
     &  useEquationToFourthOrder=1,
     &  useEquationToSecondOrder=2,
     &  useExtrapolation=3 )

      integer i1,i2,i3,j1,j2,j3,mGhost,mg1,mg2
      integer m11,m12,m13,m14,m15,
     &        m21,m22,m23,m24,m25,
     &        m31,m32,m33,m34,m35,
     &        m41,m42,m43,m44,m45,
     &        m51,m52,m53,m54,m55
      integer    m111,m211,m311,m411,m511,
     &           m121,m221,m321,m421,m521,
     &           m131,m231,m331,m431,m531,
     &           m141,m241,m341,m441,m541,
     &           m151,m251,m351,m451,m551,
     &           m112,m212,m312,m412,m512,
     &           m122,m222,m322,m422,m522,
     &           m132,m232,m332,m432,m532,
     &           m142,m242,m342,m442,m542,
     &           m152,m252,m352,m452,m552,
     &           m113,m213,m313,m413,m513,
     &           m123,m223,m323,m423,m523,
     &           m133,m233,m333,m433,m533,
     &           m143,m243,m343,m443,m543,
     &           m153,m253,m353,m453,m553,
     &           m114,m214,m314,m414,m514,
     &           m124,m224,m324,m424,m524,
     &           m134,m234,m334,m434,m534,
     &           m144,m244,m344,m444,m544,
     &           m154,m254,m354,m454,m554,
     &           m115,m215,m315,m415,m515,
     &           m125,m225,m325,m425,m525,
     &           m135,m235,m335,m435,m535,
     &           m145,m245,m345,m445,m545,
     &           m155,m255,m355,m455,m555
      real op2d, op3d,op2dSparse4,op3dSparse4, op2d4

!     ...variables for 4th-order Neumann BC    
      real drn
      real cf1,cf2,cg1,cg2
      real uu,us,uss,usss,ur,urr,urrr,urs,urss,urrs

      real rxSq,rxNorm,rxNorms,rxNormss,rxNormr,rxNormrr
      real a1s,a1ss,a2s,a2ss,a0s,a0ss, a1r,a1rr,a2r,a2rr,a0r,a0rr

      real a0,a1,a2,alpha1,alpha2
      real rxi,ryi,sxi,syi,rxr,rxs,sxr,sxs,ryr,rys,syr,sys
      real rxxi,ryyi,sxxi,syyi
      real rxrr,rxrs,rxss,ryrr,ryrs,ryss
      real sxrr,sxrs,sxss,syrr,syrs,syss
      real rxx,ryy,sxx,syy
      real rxxr,ryyr,rxxs,ryys, sxxr,syyr,sxxs,syys
      real rxNormI,rxNormIs,rxNormIss,rxNormIr,rxNormIrr
      real sxNormI,sxNormIs,sxNormIss,sxNormIr,sxNormIrr
      real n1,n1s,n1ss,n2,n2s,n2ss,n1r,n2r,n1rr,n2rr
      real an1,an1s,an1ss,an2,an2s,an2ss,an1r,an1rr,an2r,an2rs,an2rr,an3,aNormi
      real ff,ffs,ffr,fft, g,gs,gss,gr,grr, gt,gtt, grt,gst
      real c11,c11r,c11s,c12,c12r,c12s,c22,c22r,c22s,c1,c1r,c1s,c2,c2r,c2s,c0,c0r,c0s
      real b0,b1,b2,b3,bf,br2

      real fv(-1:1,-1:1,-1:1), gv(-1:1,-1:1,-1:1)

      integer ma,mb,mc
      real ca,cb,cc

!     --- start statement function ----
      integer kd
      real rx,ry,rz,sx,sy,sz,tx,ty,tz

      declareDifferenceOrder2(u,RX)
      declareDifferenceOrder4(u,RX)

!     include 'declareDiffOrder2f.h'
!     include 'declareDiffOrder4f.h'

      op2d(i1,i2,i3,j1,j2,j3)=
     &     c(m11,i1,i2,i3)*u(j1-1,j2-1,j3)+
     &     c(m21,i1,i2,i3)*u(j1  ,j2-1,j3)+
     &     c(m31,i1,i2,i3)*u(j1+1,j2-1,j3)+
     &     c(m12,i1,i2,i3)*u(j1-1,j2  ,j3)+
     &     c(m22,i1,i2,i3)*u(j1  ,j2  ,j3)+
     &     c(m32,i1,i2,i3)*u(j1+1,j2  ,j3)+
     &     c(m13,i1,i2,i3)*u(j1-1,j2+1,j3)+
     &     c(m23,i1,i2,i3)*u(j1  ,j2+1,j3)+
     &     c(m33,i1,i2,i3)*u(j1+1,j2+1,j3)
      op3d(i1,i2,i3,j1,j2,j3)=
     &     c(m111,i1,i2,i3)*u(j1-1,j2-1,j3-1)+
     &     c(m211,i1,i2,i3)*u(j1  ,j2-1,j3-1)+
     &     c(m311,i1,i2,i3)*u(j1+1,j2-1,j3-1)+
     &     c(m121,i1,i2,i3)*u(j1-1,j2  ,j3-1)+
     &     c(m221,i1,i2,i3)*u(j1  ,j2  ,j3-1)+
     &     c(m321,i1,i2,i3)*u(j1+1,j2  ,j3-1)+
     &     c(m131,i1,i2,i3)*u(j1-1,j2+1,j3-1)+
     &     c(m231,i1,i2,i3)*u(j1  ,j2+1,j3-1)+
     &     c(m331,i1,i2,i3)*u(j1+1,j2+1,j3-1)+
     &     c(m112,i1,i2,i3)*u(j1-1,j2-1,j3  )+
     &     c(m212,i1,i2,i3)*u(j1  ,j2-1,j3  )+
     &     c(m312,i1,i2,i3)*u(j1+1,j2-1,j3  )+
     &     c(m122,i1,i2,i3)*u(j1-1,j2  ,j3  )+
     &     c(m222,i1,i2,i3)*u(j1  ,j2  ,j3  )+
     &     c(m322,i1,i2,i3)*u(j1+1,j2  ,j3  )+
     &     c(m132,i1,i2,i3)*u(j1-1,j2+1,j3  )+
     &     c(m232,i1,i2,i3)*u(j1  ,j2+1,j3  )+
     &     c(m332,i1,i2,i3)*u(j1+1,j2+1,j3  )+
     &     c(m113,i1,i2,i3)*u(j1-1,j2-1,j3+1)+
     &     c(m213,i1,i2,i3)*u(j1  ,j2-1,j3+1)+
     &     c(m313,i1,i2,i3)*u(j1+1,j2-1,j3+1)+
     &     c(m123,i1,i2,i3)*u(j1-1,j2  ,j3+1)+
     &     c(m223,i1,i2,i3)*u(j1  ,j2  ,j3+1)+
     &     c(m323,i1,i2,i3)*u(j1+1,j2  ,j3+1)+
     &     c(m133,i1,i2,i3)*u(j1-1,j2+1,j3+1)+
     &     c(m233,i1,i2,i3)*u(j1  ,j2+1,j3+1)+
     &     c(m333,i1,i2,i3)*u(j1+1,j2+1,j3+1)

! ********** 4th order ****************
      op2dSparse4(i1,i2,i3,j1,j2,j3)=
     &     c(m31,i1,i2,i3)*u(j1  ,j2-2,j3)+
     &     c(m32,i1,i2,i3)*u(j1  ,j2-1,j3)+
     &     c(m13,i1,i2,i3)*u(j1-2,j2  ,j3)+
     &     c(m23,i1,i2,i3)*u(j1-1,j2  ,j3)+
     &     c(m33,i1,i2,i3)*u(j1  ,j2  ,j3)+
     &     c(m43,i1,i2,i3)*u(j1+1,j2  ,j3)+
     &     c(m53,i1,i2,i3)*u(j1+2,j2  ,j3)+
     &     c(m34,i1,i2,i3)*u(j1  ,j2+1,j3)+
     &     c(m35,i1,i2,i3)*u(j1  ,j2+2,j3)

      op3dSparse4(i1,i2,i3,j1,j2,j3) = 
     &     c(m331,i1,i2,i3)*u(j1  ,j2  ,j3-2)+
     &     c(m332,i1,i2,i3)*u(j1  ,j2  ,j3-1)+
     &     c(m313,i1,i2,i3)*u(j1  ,j2-2,j3  )+
     &     c(m323,i1,i2,i3)*u(j1  ,j2-1,j3  )+
     &     c(m133,i1,i2,i3)*u(j1-2,j2  ,j3  )+
     &     c(m233,i1,i2,i3)*u(j1-1,j2  ,j3  )+
     &     c(m333,i1,i2,i3)*u(j1  ,j2  ,j3  )+
     &     c(m433,i1,i2,i3)*u(j1+1,j2  ,j3  )+
     &     c(m533,i1,i2,i3)*u(j1+2,j2  ,j3  )+
     &     c(m343,i1,i2,i3)*u(j1  ,j2+1,j3  )+
     &     c(m353,i1,i2,i3)*u(j1  ,j2+2,j3  )+
     &     c(m334,i1,i2,i3)*u(j1  ,j2  ,j3+1)+
     &     c(m335,i1,i2,i3)*u(j1  ,j2  ,j3+2)

! ============= fourth order =================
      op2d4(i1,i2,i3)=
     &     c(m11,i1,i2,i3)*u(i1-2,i2-2,i3)+
     &     c(m21,i1,i2,i3)*u(i1-1,i2-2,i3)+
     &     c(m31,i1,i2,i3)*u(i1  ,i2-2,i3)+
     &     c(m41,i1,i2,i3)*u(i1+1,i2-2,i3)+
     &     c(m51,i1,i2,i3)*u(i1+2,i2-2,i3)+
     &     c(m12,i1,i2,i3)*u(i1-2,i2-1,i3)+
     &     c(m22,i1,i2,i3)*u(i1-1,i2-1,i3)+
     &     c(m32,i1,i2,i3)*u(i1  ,i2-1,i3)+
     &     c(m42,i1,i2,i3)*u(i1+1,i2-1,i3)+
     &     c(m52,i1,i2,i3)*u(i1+2,i2-1,i3)+
     &     c(m13,i1,i2,i3)*u(i1-2,i2  ,i3)+
     &     c(m23,i1,i2,i3)*u(i1-1,i2  ,i3)+
     &     c(m33,i1,i2,i3)*u(i1  ,i2  ,i3)+
     &     c(m43,i1,i2,i3)*u(i1+1,i2  ,i3)+
     &     c(m53,i1,i2,i3)*u(i1+2,i2  ,i3)+
     &     c(m14,i1,i2,i3)*u(i1-2,i2+1,i3)+
     &     c(m24,i1,i2,i3)*u(i1-1,i2+1,i3)+
     &     c(m34,i1,i2,i3)*u(i1  ,i2+1,i3)+
     &     c(m44,i1,i2,i3)*u(i1+1,i2+1,i3)+
     &     c(m54,i1,i2,i3)*u(i1+2,i2+1,i3)+
     &     c(m15,i1,i2,i3)*u(i1-2,i2+2,i3)+
     &     c(m25,i1,i2,i3)*u(i1-1,i2+2,i3)+
     &     c(m35,i1,i2,i3)*u(i1  ,i2+2,i3)+
     &     c(m45,i1,i2,i3)*u(i1+1,i2+2,i3)+
     &     c(m55,i1,i2,i3)*u(i1+2,i2+2,i3)

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
      defineDifferenceOrder2Components0(u,RX)
      defineDifferenceOrder4Components0(u,RX)
!     --- end statement functions ----

      
      side                 =ipar(0)
      axis                 =ipar(1)
      useCoefficients      =ipar(2)
      orderOfExtrapolation =ipar(3)
      gridType             =ipar(4)
      orderOfAccuracy      =ipar(5)
      useForcing           =ipar(6)
      equationToSolve      =ipar(7)
      bcOption4            =ipar(8)
      solveEquationWithBC  =ipar(9)
      level                =ipar(10)
      debug                =ipar(11)

      dirichletFirstGhostLineBC =ipar(12)
      neumannFirstGhostLineBC   =ipar(13)
      dirichletSecondGhostLineBC=ipar(14)
      neumannSecondGhostLineBC  =ipar(15)

      grid                 =ipar(16)

      isNeumannBC          =ipar(17)  ! true if this is really a Neumann or mixed BC
      orderOfExtrapolationForNeumann = ipar(18)
      myid                 =ipar(19)

      dx(0)                =rpar(0)
      dx(1)                =rpar(1)
      dx(2)                =rpar(2)
      a0                   =rpar(3)
      a1                   =rpar(4)
      dr(0)                =rpar(5)
      dr(1)                =rpar(6)
      dr(2)                =rpar(7)
!**      signForJacobian      =rpar(8)

      !write(*,'("bcOpt: side,axis,grid=",2i2,i3," bc=",i2," useCoefficients=",i2," equationToSolve=",i2)') side,axis,grid,bc,useCoefficients,equationToSolve
      !write(*,'("bcOpt: isNeumannBC=",i2," neumannFirstGhostLineBC=",i2," useForcing=",i2)') isNeumannBC,neumannFirstGhostLineBC,useForcing


      if( nd.eq.3 .and. bc.eq.equation .and. orderOfAccuracy.eq.4 .and. \
          neumannSecondGhostLineBC.eq.useEquationToSecondOrder )then

        ! 3D, order=4, Neumann BC + use equation:
        call bc3dOrder4( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
          n1a,n1b,n2a,n2b,n3a,n3b, ndc, c, u,f,mask,rsxy, xy,\
          bc, boundaryCondition, ipar, rpar )

        return 

      end if

      if( orderOfExtrapolationForNeumann.lt.1 .or. orderOfExtrapolationForNeumann.gt.100 )then
         write(*,'("bcOpt:ERROR: orderOfExtrapolationForNeumann=",i6)') orderOfExtrapolationForNeumann
         stop 7051
      end if

      if( debug.gt.7 )then
        write(*,'(" bcOpt: bc=",i2," level=",i1," order=",i1," n1bc=",i2," n2bc=",i2)') 
     & bc,level,orderOfAccuracy,
     & neumannFirstGhostLineBC,neumannSecondGhostLineBC
      end if


      is1=0
      is2=0
      is3=0
      if( axis.eq.0 )then
        is1=1-2*side
      else if( axis.eq.1 )then
        is2=1-2*side
      else if( axis.eq.2 )then
        is3=1-2*side
      else
        stop 5
      end if

      if( orderOfACcuracy.eq.4 )then
        m11=1     
        m21=2     
        m31=3     
        m41=4     
        m51=5     
        m12=6     
        m22=7     
        m32=8     
        m42=9     
        m52=10    
        m13=11    
        m23=12    
        m33=13    
        m43=14    
        m53=15    
        m14=16    
        m24=17    
        m34=18    
        m44=19    
        m54=20    
        m15=21    
        m25=22    
        m35=23    
        m45=24    
        m55=25   
      end if

      ! write(*,*) 'bcOpt:side,axis,bc,bcOption4,orderOfAccuracy=',side,axis,bc,bcOption4,orderOfAccuracy

      if( bc.eq.dirichlet )then
         
        if( useForcing.eq.1 )then
          loopsMaskGT(u(i1,i2,i3)=f(i1,i2,i3))
        else
          loopsMaskGT(u(i1,i2,i3)=0.)
        end if


      else if( bc .eq. extrapolation )then 

        if( useCoefficients.eq.1 )then
          ! write(*,*) 'bcOpt:useCoeff extrap side,axis,is1,is2,is3=',side,axis,is1,is2,is3
          if( orderOfExtrapolation.le.3 )then
            loops(u(i1,i2,i3)=-(\
            c(2,i1,i2,i3)*u(i1+  is1,i2+  is2,i3+  is3)+\
            c(3,i1,i2,i3)*u(i1+2*is1,i2+2*is2,i3+2*is3)+\
            c(4,i1,i2,i3)*u(i1+3*is1,i2+3*is2,i3+3*is3)\
            )/c(1,i1,i2,i3))
          else if( orderOfExtrapolation.le.4 )then
            loops(u(i1,i2,i3)=-(\
            c(2,i1,i2,i3)*u(i1+  is1,i2+  is2,i3+  is3)+\
            c(3,i1,i2,i3)*u(i1+2*is1,i2+2*is2,i3+2*is3)+\
            c(4,i1,i2,i3)*u(i1+3*is1,i2+3*is2,i3+3*is3)+\
            c(5,i1,i2,i3)*u(i1+4*is1,i2+4*is2,i3+4*is3)\
            )/c(1,i1,i2,i3))
          else if( orderOfExtrapolation.le.5 )then
            loops(u(i1,i2,i3)=-(\
            c(2,i1,i2,i3)*u(i1+  is1,i2+  is2,i3+  is3)+\
            c(3,i1,i2,i3)*u(i1+2*is1,i2+2*is2,i3+2*is3)+\
            c(4,i1,i2,i3)*u(i1+3*is1,i2+3*is2,i3+3*is3)+\
            c(5,i1,i2,i3)*u(i1+4*is1,i2+4*is2,i3+4*is3)+\
            c(6,i1,i2,i3)*u(i1+5*is1,i2+5*is2,i3+5*is3)\
            )/c(1,i1,i2,i3))
          else
            write(*,*) 'bcOpt:ERROR: orderOfExtrapolation=',
     &           orderOfExtrapolation
            stop 1
          end if

        else
          ! explicit application of extrapolation equations
          ! write(*,*) 'bcOpt: extrap side,axis,is1,is2,is3,order=',side,axis,is1,is2,is3,orderOfExtrapolation

          if( orderOfExtrapolation.eq.3 )then
            loops(u(i1,i2,i3)=\
             3.*u(i1+  is1,i2+  is2,i3+  is3)\
            -3.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
               +u(i1+3*is1,i2+3*is2,i3+3*is3))
          else if( orderOfExtrapolation.eq.4 )then
            loops(u(i1,i2,i3)=\
              4.*u(i1+  is1,i2+  is2,i3+  is3)\
             -6.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
             +4.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
                -u(i1+4*is1,i2+4*is2,i3+4*is3))
          else if( orderOfExtrapolation.eq.5 )then
            loops(u(i1,i2,i3)=\
              5.*u(i1+  is1,i2+  is2,i3+  is3)\
            -10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
            +10.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
             -5.*u(i1+4*is1,i2+4*is2,i3+4*is3)\
                +u(i1+5*is1,i2+5*is2,i3+5*is3))
          else if( orderOfExtrapolation.eq.6 )then
            loops(u(i1,i2,i3)=\
              6.*u(i1+  is1,i2+  is2,i3+  is3)\
            -15.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
            +20.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
            -15.*u(i1+4*is1,i2+4*is2,i3+4*is3)\
             +6.*u(i1+5*is1,i2+5*is2,i3+5*is3)\
                -u(i1+6*is1,i2+6*is2,i3+6*is3))
          else if( orderOfExtrapolation.eq.7 )then
            loops(u(i1,i2,i3)=\
              7.*u(i1+  is1,i2+  is2,i3+  is3)\
            -21.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
            +35.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
            -35.*u(i1+4*is1,i2+4*is2,i3+4*is3)\
            +21.*u(i1+5*is1,i2+5*is2,i3+5*is3)\
             -7.*u(i1+6*is1,i2+6*is2,i3+6*is3)\
                +u(i1+7*is1,i2+7*is2,i3+7*is3))
          else if( orderOfExtrapolation.eq.2 )then
            loops(u(i1,i2,i3)=\
             2.*u(i1+  is1,i2+  is2,i3+  is3)\
               -u(i1+2*is1,i2+2*is2,i3+2*is3))
          else
            write(*,*) 'bcOpt:ERROR: orderOfExtrapolation=',
     &           orderOfExtrapolation
            stop 1
          end if
        end if

      else if( bc .eq. extrapolateTwoGhostLines )then 
        ! extrapolate two ghost 
        ! write(*,*) 'bcOpt: extrapTwo side,axis,is1,is2,is3,order=',side,axis,is1,is2,is3,orderOfExtrapolation

        if( orderOfExtrapolation.eq.3 )then
          loops2(u(i1,i2,i3)=\
           3.*u(i1+  is1,i2+  is2,i3+  is3)\
          -3.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
             +u(i1+3*is1,i2+3*is2,i3+3*is3),\
                 u(i1-is1,i2-is2,i3-is3)=\
           3.*u(i1      ,i2      ,i3      )\
          -3.*u(i1+  is1,i2+  is2,i3+  is3)\
             +u(i1+2*is1,i2+2*is2,i3+2*is3))
        else if( orderOfExtrapolation.eq.4 )then
          loops2(u(i1,i2,i3)=\
            4.*u(i1+  is1,i2+  is2,i3+  is3)\
           -6.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
           +4.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
              -u(i1+4*is1,i2+4*is2,i3+4*is3),\
                 u(i1-is1,i2-is2,i3-is3)=\
            4.*u(i1      ,i2      ,i3      )\
           -6.*u(i1+  is1,i2+  is2,i3+  is3)\
           +4.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
              -u(i1+3*is1,i2+3*is2,i3+3*is3)  )
        else if( orderOfExtrapolation.eq.5 )then
          loops2(u(i1,i2,i3)=\
            5.*u(i1+  is1,i2+  is2,i3+  is3)\
          -10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
          +10.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
           -5.*u(i1+4*is1,i2+4*is2,i3+4*is3)\
              +u(i1+5*is1,i2+5*is2,i3+5*is3),\
                u(i1-is1,i2-is2,i3-is3)=\
            5.*u(i1      ,i2      ,i3      )\
          -10.*u(i1+  is1,i2+  is2,i3+  is3)\
          +10.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
           -5.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
              +u(i1+4*is1,i2+4*is2,i3+4*is3)     )
        else if( orderOfExtrapolation.eq.6 )then
          loops2(u(i1,i2,i3)=\
            6.*u(i1+  is1,i2+  is2,i3+  is3)\
          -15.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
          +20.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
          -15.*u(i1+4*is1,i2+4*is2,i3+4*is3)\
           +6.*u(i1+5*is1,i2+5*is2,i3+5*is3)\
              -u(i1+6*is1,i2+6*is2,i3+6*is3),\
                 u(i1-is1,i2-is2,i3-is3)=\
            6.*u(i1      ,i2      ,i3      )\
          -15.*u(i1+  is1,i2+  is2,i3+  is3)\
          +20.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
          -15.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
           +6.*u(i1+4*is1,i2+4*is2,i3+4*is3)\
              -u(i1+5*is1,i2+5*is2,i3+5*is3)     )
        else if( orderOfExtrapolation.eq.7 )then
          loops2(u(i1,i2,i3)=\
            7.*u(i1+  is1,i2+  is2,i3+  is3)\
          -21.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
          +35.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
          -35.*u(i1+4*is1,i2+4*is2,i3+4*is3)\
          +21.*u(i1+5*is1,i2+5*is2,i3+5*is3)\
           -7.*u(i1+6*is1,i2+6*is2,i3+6*is3)\
              +u(i1+7*is1,i2+7*is2,i3+7*is3),\
                 u(i1-is1,i2-is2,i3-is3)=\
            7.*u(i1      ,i2      ,i3      )\
          -21.*u(i1+  is1,i2+  is2,i3+  is3)\
          +35.*u(i1+2*is1,i2+2*is2,i3+2*is3)\
          -35.*u(i1+3*is1,i2+3*is2,i3+3*is3)\
          +21.*u(i1+4*is1,i2+4*is2,i3+4*is3)\
           -7.*u(i1+5*is1,i2+5*is2,i3+5*is3)\
              +u(i1+6*is1,i2+6*is2,i3+6*is3)   )
        else if( orderOfExtrapolation.eq.2 )then
          loops2(u(i1,i2,i3)=\
           2.*u(i1+  is1,i2+  is2,i3+  is3)\
             -u(i1+2*is1,i2+2*is2,i3+2*is3),\
                u(i1-is1,i2-is2,i3-is3)=\
           2.*u(i1      ,i2      ,i3      )\
             -u(i1+  is1,i2+  is2,i3+  is3)    )
        else
          write(*,*) 'bcOpt:ERROR: orderOfExtrapolation=',orderOfExtrapolation
          stop 1
        end if

      else if( isNeumannBC.eq.1 .and. orderOfAccuracy.eq.2 .and. gridType.eq.rectangular )then

        ! Neumann or mixed BC, 2nd-order
        neumann2ndOrderRectangular()

      else if( bc.eq.equation .and. orderOfAccuracy.eq.2 )then

        ! 2nd order "Neumann" or mixed type boundary condition 
        ! The BC is defined by the coefficient matrix

        ! write(*,'("bcOpt: neumann/mixed useCoeff: grid=",i4," level=",i2," side,axis=",2i2," useForcing=",i2)') grid,level,side,axis,useForcing

        if( useCoefficients.eq.0 )then
          stop 2930
        end if

        ! if( gridType.ne.curvilinear )then
        !   stop 2930
        ! end if

        if( nd.eq.2 )then
          m11=1                 ! MCE(-1,-1, 0)
          m21=2                 ! MCE( 0,-1, 0)
          m31=3                 ! MCE(+1,-1, 0)
          m12=4                 ! MCE(-1, 0, 0)
          m22=5                 ! MCE( 0, 0, 0)
          m32=6                 ! MCE(+1, 0, 0)
          m13=7                 ! MCE(-1,+1, 0)
          m23=8                 ! MCE( 0,+1, 0)
          m33=9                 ! MCE(+1,+1, 0)



          mGhost=m22-is1+3*(-is2)

         ! j1=-1 
         ! j2=1
         ! j3=0
         ! write(*,'("bcOpt:Neumann: j1,j2=",2i3,"  c11,c12,c13=",3e10.2," c21,c22,c23=",3e10.2," c31,c32,c33=",3e10.2, "cGhost=",e10.2)') j1,j2,\
        ! c(m11,j1,j2,j3),c(m12,j1,j2,j3),c(m13,j1,j2,j3),\
        ! c(m21,j1,j2,j3),c(m22,j1,j2,j3),c(m23,j1,j2,j3),\
        ! c(m31,j1,j2,j3),c(m32,j1,j2,j3),c(m33,j1,j2,j3),c(mGhost,j1,j2,j3)

          loopsMaskIS( u(i1,i2,i3)=(f(i1,i2,i3)-op2d(i1,i2,i3,i1+is1,i2+is2,i3))/c(mGhost,i1,i2,i3)+u(i1,i2,i3) ) 
        else
          m111=1 
          m211=2 
          m311=3 
          m121=4 
          m221=5 
          m321=6 
          m131=7 
          m231=8 
          m331=9 
          m112=10
          m212=11
          m312=12
          m122=13
          m222=14
          m322=15 
          m132=16
          m232=17
          m332=18
          m113=19
          m213=20
          m313=21
          m123=22
          m223=23
          m323=24 
          m133=25
          m233=26
          m333=27

          mGhost=m222-is1+3*(-is2)+9*(-is3)
          loopsMaskIS(u(i1,i2,i3)=(f(i1,i2,i3)-op3d(i1,i2,i3,i1+is1,i2+is2,i3+is3))/c(mGhost,i1,i2,i3)+u(i1,i2,i3))

        end if

      else if( bc.eq.equation .and. orderOfAccuracy.eq.4 )then

        ! *************************************************************
        ! *********  Fourth-order accurate Neumann or mixed BC's ******
        ! *************************************************************
        ! write(*,*) 'bcOpt:equation4, bcOption4=',bcOption4

        ! *wdh* 100507 if( bcOption4.eq.0 )then
        if( neumannSecondGhostLineBC.eq.useExtrapolation )then

          if( debug.gt.15 .and. myid.eq.0 )then
           write(*,'("  bcOpt:order4:l=",i2," neumann-AndExtrap",i1,"...")') level,orderOfExtrapolation
          end if
          neumannAndExtrapolation()

        else if( neumannSecondGhostLineBC.eq.useEquationToSecondOrder )then


          if( debug.gt.15 )then
            write(*,'("  bcOpt:order4:l=",i2," neumann-AndEqn...")') level
          end if
          ! write(*,*) 'bcOpt:NE n1a,n1b,n2a,n2b=',n1a,n1b,n2a,n2b

          ! define m1a,m1b,.. to equal n1a,n1b,.. except for periodic directions
          m1a=n1a
          m1b=n1b
          m2a=n2a
          m2b=n2b
          m3a=n3a
          m3b=n3b
          if( boundaryCondition(0,0).lt.0 )then
            m1a=m1a-1
            m1b=m1b+1
          end if
          if( boundaryCondition(0,1).lt.0 )then
            m2a=m2a-1
            m2b=m2b+1
          end if
          if( boundaryCondition(0,2).lt.0 )then
            m3a=m3a-1
            m3b=m3b+1
          end if
          if( useForcing.eq.1 )then
            if( axis.eq.0 .and. nd.eq.2 )then
              if( solveEquationWithBC.eq.0 )then
               neumannAndEquation(forcing,R,2)
              else 
               ! assign 2-ghost and boundary pt
               neumannPDE(forcing,R,2) 
              end if
            else if( axis.eq.1 .and. nd.eq.2 )then
              if( solveEquationWithBC.eq.0 )then
               neumannAndEquation(forcing,S,2)
              else 
               ! assign 2-ghost and boundary pt
               neumannPDE(forcing,S,2) 
              end if
!           else if( axis.eq.0 .and. nd.eq.3 )then
!             neumannAndEquation(forcing,R,3)
!           else if( axis.eq.1 .and. nd.eq.3 )then
!             neumannAndEquation(forcing,S,3)
!           else if( axis.eq.2 .and. nd.eq.3 )then
!             neumannAndEquation(forcing,T,3)
            else
              stop 10
            end if
          else
            if( axis.eq.0 .and. nd.eq.2 )then
              if( solveEquationWithBC.eq.0 )then
               neumannAndEquation(noForcing,R,2)
              else 
               ! assign 2-ghost and boundary pt
               neumannPDE(noForcing,R,2) 
              end if
            else if( axis.eq.1 .and. nd.eq.2 )then
              if( solveEquationWithBC.eq.0 )then
               neumannAndEquation(noForcing,S,2)
              else 
               ! assign 2-ghost and boundary pt
               neumannPDE(noForcing,S,2) 
              end if
!           else if( axis.eq.0 .and. nd.eq.3 )then
!             neumannAndEquation(noForcing,R,3)
!           else if( axis.eq.1 .and. nd.eq.3 )then
!             neumannAndEquation(noForcing,S,3)
!           else if( axis.eq.2 .and. nd.eq.3 )then
!             neumannAndEquation(noForcing,T,3)
            else
              stop 10
            end if
          end if

        else 
          write(*,*) 'bcOpt:order4:ERROR:neumannSecondGhostLineBC=',neumannSecondGhostLineBC
          stop 7711

        end if

      else if( bc.eq.oddSymmetry )then

        assignOddSymmetry()

      else if( bc.eq.mixedToSecondOrder .or. bc.eq.evenSymmetry )then

        if( bc.eq.evenSymmetry )then
          if( debug.gt.15 )then
            write(*,'("  bcOpt:evenSymmetry side,axis=",2i2)') side,axis 
          end if

          a0=0.
          a1=1.
        else if( a1.eq.0 )then
          if( debug.gt.15 )then
            write(*,'("  bcOpt:mixedToSecondOrder")') 
          end if
          write(*,*) 'ERROR bcOpt: a1.eq.0 for mixedToSecondOrder'
        end if
         
        if( useForcing.eq.1 )then
         ! forcing
         if( gridType.eq.rectangular )then
           ! The next function does not depend on DIM or DIR
           assignMixedToSecondOrder(rectangular,2,r,forcing)
         else if( nd.eq.2 .and. axis.eq.0 )then
           assignMixedToSecondOrder(curvilinear,2,r,forcing)
         else if( nd.eq.2 .and. axis.eq.1 )then
           assignMixedToSecondOrder(curvilinear,2,s,forcing)
         else if( nd.eq.3 .and. axis.eq.0 )then
           assignMixedToSecondOrder(curvilinear,3,r,forcing)
         else if( nd.eq.3 .and. axis.eq.1 )then
           assignMixedToSecondOrder(curvilinear,3,s,forcing)
         else if( nd.eq.3 .and. axis.eq.2 )then
           assignMixedToSecondOrder(curvilinear,3,t,forcing)
         else
           stop 4466
         end if
        else
         ! no forcing
         if( gridType.eq.rectangular )then
           ! The next function does not depend on DIM or DIR
           assignMixedToSecondOrder(rectangular,2,r,noForcing)
         else if( nd.eq.2 .and. axis.eq.0 )then
           assignMixedToSecondOrder(curvilinear,2,r,noForcing)
         else if( nd.eq.2 .and. axis.eq.1 )then
           assignMixedToSecondOrder(curvilinear,2,s,noForcing)
         else if( nd.eq.3 .and. axis.eq.0 )then
           assignMixedToSecondOrder(curvilinear,3,r,noForcing)
         else if( nd.eq.3 .and. axis.eq.1 )then
           assignMixedToSecondOrder(curvilinear,3,s,noForcing)
         else if( nd.eq.3 .and. axis.eq.2 )then
           assignMixedToSecondOrder(curvilinear,3,t,noForcing)
         else
           stop 4466
         end if
        end if
      else if( bc.eq.equationToSecondOrder )then

        ! For the fourth order method we can apply the equation to 2nd order on the boundary
        ! in order to define the ghost point value


        ! write(*,'(" bcOpt:equationToSecondOrder: orderOfExtrapolation=",i4)') orderOfExtrapolation

        if( orderOfAccuracy.ne.4 )then 
          write(*,*) 'bcOpt:ERROR: bc.eq.equationToSecondOrder but orderOfAccuracy.ne.4'
          stop 6 
        end if

        if( equationToSolve.eq.laplaceEquation )then ! .or. equationToSolve.eq.heatEquationOperator )then

          ! ****** laplace equation or heat operator ****

          if( gridType.eq.rectangular .and. nd.eq.2 )then
  
           ! write(*,*) 'bcOpt: equationToSecondOrder side,axis=',side,axis
           ! write(*,*) 'bcOpt: is1,is2,dx=',is1,is2,dx(0),dx(1)
           if( axis.eq.0 )then
            ! put the equation twice, once with forcing and once without
            loopsMaskForceAndExtrapolate(u(i1-is1,i2,i3)=2.*u(i1,i2,i3)-u(i1+is1,i2,i3) \
               +(dx(0)**2)*( f(i1-is1,i2,i3) - (u(i1,i2+1,i3)-2.*u(i1,i2,i3)+u(i1,i2-1,i3) )/dx(1)**2 ),\
                           u(i1-is1,i2,i3)=2.*u(i1,i2,i3)-u(i1+is1,i2,i3) \
               +(dx(0)**2)*(                 - (u(i1,i2+1,i3)-2.*u(i1,i2,i3)+u(i1,i2-1,i3) )/dx(1)**2 ))
           else 
            loopsMaskForceAndExtrapolate(u(i1,i2-is2,i3)=2.*u(i1,i2,i3)-u(i1,i2+is2,i3) \
               +(dx(1)**2)*( f(i1,i2-is2,i3) - (u(i1+1,i2,i3)-2.*u(i1,i2,i3)+u(i1-1,i2,i3) )/dx(0)**2 ),\
                           u(i1,i2-is2,i3)=2.*u(i1,i2,i3)-u(i1,i2+is2,i3) \
               +(dx(1)**2)*(                 - (u(i1+1,i2,i3)-2.*u(i1,i2,i3)+u(i1-1,i2,i3) )/dx(0)**2 ))
           end if 
  
          else if( gridType.eq.rectangular .and. nd.eq.3 )then
  
           if( axis.eq.0 )then
            loopsMaskForceAndExtrapolate(u(i1-is1,i2,i3)=2.*u(i1,i2,i3)-u(i1+is1,i2,i3) \
               +(dx(0)**2)*( f(i1-is1,i2,i3) - (u(i1,i2+1,i3)-2.*u(i1,i2,i3)+u(i1,i2-1,i3) )/dx(1)**2 \
                                             - (u(i1,i2,i3+1)-2.*u(i1,i2,i3)+u(i1,i2,i3-1) )/dx(2)**2  ),\
                           u(i1-is1,i2,i3)=2.*u(i1,i2,i3)-u(i1+is1,i2,i3) \
               +(dx(0)**2)*(                 - (u(i1,i2+1,i3)-2.*u(i1,i2,i3)+u(i1,i2-1,i3) )/dx(1)**2 \
                                             - (u(i1,i2,i3+1)-2.*u(i1,i2,i3)+u(i1,i2,i3-1) )/dx(2)**2  ))
           else if( axis.eq.1 )then
            loopsMaskForceAndExtrapolate(u(i1,i2-is2,i3)=2.*u(i1,i2,i3)-u(i1,i2+is2,i3) \
               +(dx(1)**2)*( f(i1,i2-is2,i3) - (u(i1+1,i2,i3)-2.*u(i1,i2,i3)+u(i1-1,i2,i3) )/dx(0)**2 \
                                             - (u(i1,i2,i3+1)-2.*u(i1,i2,i3)+u(i1,i2,i3-1) )/dx(2)**2  ),\
                           u(i1,i2-is2,i3)=2.*u(i1,i2,i3)-u(i1,i2+is2,i3) \
               +(dx(1)**2)*(                 - (u(i1+1,i2,i3)-2.*u(i1,i2,i3)+u(i1-1,i2,i3) )/dx(0)**2 \
                                             - (u(i1,i2,i3+1)-2.*u(i1,i2,i3)+u(i1,i2,i3-1) )/dx(2)**2  ))
           else 
            loopsMaskForceAndExtrapolate(u(i1,i2,i3-is3)=2.*u(i1,i2,i3)-u(i1,i2,i3+is3) \
               +(dx(2)**2)*( f(i1,i2,i3-is3) - (u(i1+1,i2,i3)-2.*u(i1,i2,i3)+u(i1-1,i2,i3) )/dx(0)**2 \
                                             - (u(i1,i2+1,i3)-2.*u(i1,i2,i3)+u(i1,i2-1,i3) )/dx(1)**2  ),\
                           u(i1,i2,i3-is3)=2.*u(i1,i2,i3)-u(i1,i2,i3+is3) \
               +(dx(2)**2)*(                 - (u(i1+1,i2,i3)-2.*u(i1,i2,i3)+u(i1-1,i2,i3) )/dx(0)**2 \
                                             - (u(i1,i2+1,i3)-2.*u(i1,i2,i3)+u(i1,i2-1,i3) )/dx(1)**2  ))
           end if 

          else if( gridType.eq.curvilinear .and.nd.eq.2 )then

           ! If Lap(u) = SUM c(m) u(m)
           ! We use :
           !    u(-1) <- u(-1) - Lap(u)/c(-1)
           ! So that Lap(u)=0
           !   rx**2 u.rr + rxx*ur 

!      ulaplacian22(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*urr2(i1,
!     & i2,i3)+2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3)
!     & )*urs2(i1,i2,i3)+(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*uss2(i1,i2,
!     & i3)+(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*ur2(i1,i2,i3)+(sxx22(i1,
!     & i2,i3)+syy22(i1,i2,i3))*us2(i1,i2,i3)

           ! NOTE: On non-orthogonal grids the cross-derivative term will be lagged
!$$$           if( axis.eq.0 )then
!$$$            loopsMaskForceAndExtrapolate(u(i1-is1,i2,i3)=u(i1-is1,i2,i3)\
!$$$                +(f(i1-is1,i2,i3)-ulaplacian22(i1,i2,i3))/( \
!$$$                 (rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)/dr(axis)**2-is1*(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))/(2.*dr(axis)) ),\
!$$$                           u(i1-is1,i2,i3)=u(i1-is1,i2,i3)\
!$$$                +(               -ulaplacian22(i1,i2,i3))/( \
!$$$                 (rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)/dr(axis)**2-is1*(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))/(2.*dr(axis)) ))
!$$$           else 
!$$$            loopsMaskForceAndExtrapolate(u(i1,i2-is2,i3)=u(i1,i2-is2,i3)\
!$$$                +(f(i1,i2-is2,i3)-ulaplacian22(i1,i2,i3) )/( \
!$$$                 (sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)/dr(axis)**2-is2*(sxx22(i1,i2,i3)+syy22(i1,i2,i3))/(2.*dr(axis)) ),\
!$$$                           u(i1,i2-is2,i3)=u(i1,i2-is2,i3)\
!$$$                +(               -ulaplacian22(i1,i2,i3) )/( \
!$$$                 (sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)/dr(axis)**2-is2*(sxx22(i1,i2,i3)+syy22(i1,i2,i3))/(2.*dr(axis)) ))
!$$$           end if 

           ! This next version seems to give the same results as above
           if( axis.eq.0 )then
            loopsMaskForceAndExtrapolate(\
                   u(i1-is1,i2,i3)=( \
                      (rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*(-2.*u(i1,i2,i3)+u(i1+is1,i2,i3))/dr(axis)**2\
                      +(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*(is1*u(i1+is1,i2,i3))/(2.*dr(axis)) \
                      +2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*urs2(i1,i2,i3)\
                      +(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*uss2(i1,i2,i3)\
                      +(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*us2(i1,i2,i3) - f(i1-is1,i2,i3) )/ \
                      (-(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)/dr(axis)**2 \
                       +is1*(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))/(2.*dr(axis))), \
                   u(i1-is1,i2,i3)=( \
                      (rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*(-2.*u(i1,i2,i3)+u(i1+is1,i2,i3))/dr(axis)**2\
                      +(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*(is1*u(i1+is1,i2,i3))/(2.*dr(axis)) \
                      +2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*urs2(i1,i2,i3)\
                      +(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*uss2(i1,i2,i3)\
                      +(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*us2(i1,i2,i3) - 0.*f(i1+is1,i2,i3) )/ \
                      (-(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)/dr(axis)**2 \
                       +is1*(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))/(2.*dr(axis))) )
           else 
            loopsMaskForceAndExtrapolate(\
                   u(i1,i2-is2,i3)=( \
                      (sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*(-2.*u(i1,i2,i3)+u(i1,i2+is2,i3))/dr(axis)**2\
                      +(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*(is2*u(i1,i2+is2,i3))/(2.*dr(axis)) \
                      +2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*urs2(i1,i2,i3)\
                      +(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*urr2(i1,i2,i3)\
                      +(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*ur2(i1,i2,i3) - f(i1,i2-is2,i3) )/ \
                      (-(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)/dr(axis)**2 \
                       +is2*(sxx22(i1,i2,i3)+syy22(i1,i2,i3))/(2.*dr(axis))), \
                   u(i1,i2-is2,i3)=( \
                      (sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*(-2.*u(i1,i2,i3)+u(i1,i2+is2,i3))/dr(axis)**2\
                      +(sxx22(i1,i2,i3)+syy22(i1,i2,i3))*(is2*u(i1,i2+is2,i3))/(2.*dr(axis)) \
                      +2.*(rx(i1,i2,i3)*sx(i1,i2,i3)+ ry(i1,i2,i3)*sy(i1,i2,i3))*urs2(i1,i2,i3)\
                      +(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*urr2(i1,i2,i3)\
                      +(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))*ur2(i1,i2,i3) - 0.*f(i1,i2+is2,i3) )/ \
                      (-(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)/dr(axis)**2 \
                       +is2*(sxx22(i1,i2,i3)+syy22(i1,i2,i3))/(2.*dr(axis))) )
           end if 


!$$$           if( axis.eq.0 )then
!$$$            loopsMaskForceAndExtrapolate(u(i1-is1,i2,i3)=u(i1-is1,i2,i3)\
!$$$                +(f(i1-is1,i2,i3)-ulaplacian22(i1,i2,i3))/( \
!$$$                 (rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)/dr(axis)**2-is1*(rxx22(i1,i2,i3)+ryy22(i1,i2,i3))/(2.*dr(axis)) ),\
!$$$                           u(i1-is1,i2,i3)=4.*u(i1,i2,i3)-6.*u(i1+is1,i2,i3)+4.*u(i1+2*is1,i2,i3)-u(i1+3*is1,i2,i3))
!$$$c                           u(i1-is1,i2,i3)=3.*u(i1,i2,i3)-3.*u(i1+is1,i2,i3)+u(i1+2*is1,i2,i3))
!$$$c                           u(i1-is1,i2,i3)=2.*u(i1,i2,i3)-u(i1+is1,i2,i3))
!$$$           else 
!$$$            loopsMaskForceAndExtrapolate(u(i1,i2-is2,i3)=u(i1,i2-is2,i3)\
!$$$                +(f(i1,i2-is2,i3)-ulaplacian22(i1,i2,i3) )/( \
!$$$                 (sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)/dr(axis)**2-is2*(sxx22(i1,i2,i3)+syy22(i1,i2,i3))/(2.*dr(axis)) ),\
!$$$                           u(i1,i2-is2,i3)=4.*u(i1,i2,i3)-6.*u(i1,i2+is2,i3)+4.*u(i1,i2+2*is2,i3)-u(i1,i2+3*is2,i3))
!$$$c                          u(i1,i2-is2,i3)=3.*u(i1,i2,i3)-3.*u(i1,i2+is2,i3)+u(i1,i2+2*is2,i3))
!$$$c                           u(i1,i2-is2,i3)=2.*u(i1,i2,i3)-u(i1,i2+is2,i3))
!$$$           end if 


          else if( gridType.eq.curvilinear .and.nd.eq.3 )then

           if( axis.eq.0 )then
            loopsMaskForceAndExtrapolate(u(i1-is1,i2,i3)=u(i1-is1,i2,i3)\
                +(f(i1-is1,i2,i3)-ulaplacian23(i1,i2,i3))/( \
                   (rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)/dr(axis)**2\
                       -is1*(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))/(2.*dr(axis)) ),\
                           u(i1-is1,i2,i3)=u(i1-is1,i2,i3)\
                +(               -ulaplacian23(i1,i2,i3))/( \
                   (rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)/dr(axis)**2\
                       -is1*(rxx23(i1,i2,i3)+ryy23(i1,i2,i3)+rzz23(i1,i2,i3))/(2.*dr(axis)) ))
           else if( axis.eq.1 )then
            loopsMaskForceAndExtrapolate(u(i1,i2-is2,i3)=u(i1,i2-is2,i3)\
                +(f(i1,i2-is2,i3)-ulaplacian23(i1,i2,i3))/( \
                   (sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)/dr(axis)**2\
                   -is2*(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))/(2.*dr(axis)) ),\
                           u(i1,i2-is2,i3)=u(i1,i2-is2,i3)\
                +(               -ulaplacian23(i1,i2,i3))/( \
                   (sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)/dr(axis)**2\
                   -is2*(sxx23(i1,i2,i3)+syy23(i1,i2,i3)+szz23(i1,i2,i3))/(2.*dr(axis)) ))
           else
            loopsMaskForceAndExtrapolate(u(i1,i2,i3-is3)=u(i1,i2,i3-is3)\
                +(f(i1,i2,i3-is3)-ulaplacian23(i1,i2,i3))/( \
                   (tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)/dr(axis)**2\
                   -is3*(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))/(2.*dr(axis)) ),\
                           u(i1,i2,i3-is3)=u(i1,i2,i3-is3)\
                +(               -ulaplacian23(i1,i2,i3))/( \
                   (tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)/dr(axis)**2\
                   -is3*(txx23(i1,i2,i3)+tyy23(i1,i2,i3)+tzz23(i1,i2,i3))/(2.*dr(axis)) ))
           end if 

          else
            write(*,*) 'bcOpt:ERROR:Unknown gridType =',gridType
            stop 6
          end if
     
        else
            write(*,*) 'bcOpt:ERROR:Unknown equationToSolve =',equationToSolve
            stop 6
        end if
    
      else
        write(*,*) 'bcOpt:ERROR: bc,orderOfAccuracy=',bc,
     &     orderOfAccuracy
        stop 3
      end if


      return
      end


! *************************************************************************************************************


#beginMacro loops(expression)
if( useWhereMask.ne.0 )then
  do c=ca,cb
    do i3=n3a,n3b,n3c
      do i2=n2a,n2b,n2c
        do i1=n1a,n1b,n1c
          if( mask(i1,i2,i3).ne.0 )then
            expression
          end if
        end do
      end do
    end do
  end do
else
  do c=ca,cb
    do i3=n3a,n3b,n3c
      do i2=n2a,n2b,n2c
        do i1=n1a,n1b,n1c
          expression
        end do
      end do
    end do
  end do
end if
#endMacro

! ================================================================================================
!  /Description:
!     Apply an extrapolation or symmetry boundary condition.
!  /i1,i2,i3,n: Indexes of points to assign.
! ===============================================================================================
#beginMacro assignCorners(side1,side2,side3)

if( cornerBC(side1,side2,side3).eq.extrapolateCorner )then
  if( orderOfExtrapolation.eq.1 )then
    loops(u(i1,i2,i3,c)=u(i1+  (is1),i2+  (is2),i3+  (is3),c))
  else if( orderOfExtrapolation.eq.2 )then
    loops(u(i1,i2,i3,c)=2.*u(i1+  (is1),i2+  (is2),i3+  (is3),c) \
                      -    u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c))
  else if( orderOfExtrapolation.eq.3 )then
    loops(u(i1,i2,i3,c)= 3.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
                       - 3.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
                       +    u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c))
  else if( orderOfExtrapolation.eq.4 )then
    loops(u(i1,i2,i3,c)=4.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
                      - 6.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
                      + 4.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  \
                      -    u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c))
  else if( orderOfExtrapolation.eq.5 )then
    loops(u(i1,i2,i3,c)=5.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
                      -10.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
                      +10.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  \
                      - 5.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)  \
                      +    u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c))
  else if( orderOfExtrapolation.eq.6 )then
    loops(u(i1,i2,i3,c)=6.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
                      -15.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
                      +20.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  \
                      -15.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)  \
                      + 6.*u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c)  \
                      -    u(i1+6*(is1),i2+6*(is2),i3+6*(is3),c))
  else if( orderOfExtrapolation.eq.7 )then
    loops(u(i1,i2,i3,c)=7.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
                      -21.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
                      +35.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  \
                      -35.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)  \
                      +21.*u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c)  \
                      - 7.*u(i1+6*(is1),i2+6*(is2),i3+6*(is3),c)  \
                      +    u(i1+7*(is1),i2+7*(is2),i3+7*(is3),c))
  else if( orderOfExtrapolation.eq.8 )then
    loops(u(i1,i2,i3,c)=8.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
                      -28.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
                      +56.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  \
                      -70.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)  \
                      +56.*u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c)  \
                      -28.*u(i1+6*(is1),i2+6*(is2),i3+6*(is3),c)  \
                      + 8.*u(i1+7*(is1),i2+7*(is2),i3+7*(is3),c)  \
                      -    u(i1+8*(is1),i2+8*(is2),i3+8*(is3),c))
  else if( orderOfExtrapolation.eq.9 )then
    loops(u(i1,i2,i3,c)=9.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
                      -36.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
                      +84.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  \
                     -126.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)  \
                     +126.*u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c)  \
                      -84.*u(i1+6*(is1),i2+6*(is2),i3+6*(is3),c)  \
                      +36.*u(i1+7*(is1),i2+7*(is2),i3+7*(is3),c)  \
                      - 9.*u(i1+8*(is1),i2+8*(is2),i3+8*(is3),c)  \
                      +    u(i1+9*(is1),i2+9*(is2),i3+9*(is3),c))
  else 
    write(*,*) 'fixBoundaryCorners:Error: '
    write(*,*) 'unable to extrapolate '
    write(*,*) ' to orderOfExtrapolation',orderOfExtrapolation
    write(*,*) ' can only do orders 1 to 9.'
    stop 1
  end if

else if( cornerBC(side1,side2,side3).eq.symmetryCorner )then
!  symmetry boundary condition 
  loops(u(i1,i2,i3,c)=u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c))

else if( cornerBC(side1,side2,side3).eq.taylor2ndOrder )then
!   Using a taylor approximation:
!    u(+1,+1) = u(0,0) +dr*ur + ds*us + dr^2/2 urr + dr*ds*urs + ds^2/2 uss + ...
!    u(-1,-1) = u(0,0) -dr*ur - ds*us + dr^2/2 urr + ...
!    u(-1,-1) = u(1,1) -2dr*ur -2*ds*us + O(dr^3+...)
!    ur = (u(1,0)-u(-1,0))/(2dr)
!   gives
!     u(-1,-1) = u(1,1) -( u(1,0)-u(-1,0) ) - (u(0,1)-u(0,-1))
  if( nd.eq.2 )then
    loops(u(i1,i2,i3,c)=(u(i1+2*(is1),i2+2*(is2),i3,c)-  \
                     u(i1+2*(is1),i2+  (is2),i3,c)+  \
                     u(i1        ,i2+  (is2),i3,c)-  \
                     u(i1+  (is1),i2+2*(is2),i3,c)+  \
		     u(i1+  (is1),i2        ,i3,c)))
  else if( nd.eq.3 )then
    loops(u(i1,i2,i3,c)=(u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)-  \
                   u(i1+2*(is1),i2+  (is2),i3+  (is3),c)+  \
                   u(i1        ,i2+  (is2),i3+  (is3),c)-  \
                   u(i1+  (is1),i2+2*(is2),i3+  (is3),c)+  \
		   u(i1+  (is1),i2        ,i3+  (is3),c)-  \
                   u(i1+  (is1),i2+  (is2),i3+2*(is3),c)+  \
                   u(i1+  (is1),i2+  (is2),i3        ,c)))
  end if
else
  write(*,*)'fixBoundaryCorners:Error:'
  write(*,*)' unknown cornerBC=',cornerBC(side1,side2,side3)
end if
#endMacro


! ******************************** new *************************

      subroutine updateCorners( nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask, dimension, indexRange, 
     & isPeriodic, bc, cornerBC, ipar, rpar )    
!======================================================================
!  Update corners (and edges in 3D) 
!         
! nd : number of space dimensions
! ca,cb : assign components c=uC(ca),..,uC(cb)
! useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
!======================================================================
      implicit none
      integer nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b

      integer bc(0:1,0:2),isPeriodic(0:2)
      integer indexRange(0:1,0:2),dimension(0:1,0:2)
      integer cornerBC(0:2,0:2,0:2)

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)
      integer ipar(0:*)
      real rpar(0:*)

!     **** corner boundary conditions *****
      integer extrapolateCorner,symmetryCorner,taylor2ndOrder
      parameter(extrapolateCorner=0,symmetryCorner=1,taylor2ndOrder=2)

!     --- local variables 
      integer c,ca,cb,orderOfExtrapolation,useWhereMask
      integer side1,side2,side3,is1,is2,is3,i1,i2,i3  
      integer n1a,n1b,n1c, n2a,n2b,n2c, n3a,n3b,n3c
  

      ca                   = ipar(0)
      cb                   = ipar(1)
      useWhereMask         = ipar(2)
      orderOfExtrapolation = ipar(3)

!        ---extrapolate or otherwise assign values outside edges---

      if( isPeriodic(0).eq.0 .and. isPeriodic(1).eq.0 )then
!     ...Do the four edges parallel to i3
        n3a=indexRange(0,2)
        n3b=indexRange(1,2)
        n3c=1
        side3=2 ! this means we are on an edge
        is3=0
        do side1=0,1
          is1=1-2*side1
          do side2=0,1
   	    is2=1-2*side2
	    if( bc(side1,0).gt.0 .or. bc(side2,1).gt.0 )then
              if( side2.eq.0 )then
                n2a=dimension(side2,1)+1-is2
                n2b=indexRange(side2,1)-is2
                n2c=1
              else
                n2a=indexRange(side2,1)-is2
                n2b=dimension(side2,1)-1-is2
                n2c=1
              end if
!	      I2m= side2==Start ? Range(c.dimension(side2,1)+1-is2,indexRange(side2,1)-is2) :
!	        Range(indexRange(side2,1)-is2,c.dimension(side2,1)-1-is2)
!             We have to loop over i1 from inside to outside since later points depend on previous ones.
!   	      for( i1=indexRange[side1][axis1]; i1!=c.dimension(side1,axis1); i1-=is1 )
!               I1m=i1-is1;
	      n1a=indexRange(side1,0)-is1
              n1b=dimension(side1,0)
              n1c=-is1
	      assignCorners(side1,side2,side3)
            end if
          end do
        end do
 
      end if
      if( nd.le.2 )then
        return
      end if

      if( isPeriodic(0).eq.0 .and. isPeriodic(2).eq.0 )then
!     ...Do the four edges parallel to i2
        n2a=indexRange(0,1)
        n2b=indexRange(1,1)
        n2c=1
        side2=2 ! this means we are on an edge
        is2=0
        do side1=0,1
          is1=1-2*side1
          do side3=0,1
            is3=1-2*side3
            if( bc(side1,0).gt.0 .or. bc(side3,2).gt.0 )then
!	      I3m= side3==Start ? Range(c.dimension(side3,2)+1-is3,indexRange(side3,2)-is3) :
!	      Range(indexRange(side3,2)-is3,c.dimension(side3,2)-1-is3)
              if( side3.eq.0 )then
                n3a=dimension(side3,2)+1-is3
                n3b=indexRange(side3,2)-is3
                n3c=1
              else
                n3a=indexRange(side3,2)-is3
                n3b=dimension(side3,2)-1-is3
                n3c=1
              end if
	      n1a=indexRange(side1,0)-is1 
              n1b=dimension(side1,0)
              n1c=-is1
	      assignCorners(side1,side2,side3)
            end if
          end do
        end do
      end if

      if( isPeriodic(1).eq.0 .and. isPeriodic(2).eq.0 )then
!          ...Do the four edges parallel to i1
        n1a=indexRange(0,0)
        n1b=indexRange(1,0)
        n1c=1
        side1=2 ! this means we are on an edge
        is1=0
        do side2=0,1
          is2=1-2*side2
!          I2m= side2==Start ? Range(c.dimension(side2,1)+1-is2,indexRange(side2,1)-is2) :
!          Range(indexRange(side2,1)-is2,c.dimension(side2,1)-1-is2)
          if( side2.eq.0 )then
            n2a=dimension(side2,1)+1-is2
            n2b=indexRange(side2,1)-is2
            n2c=1
          else
            n2a=indexRange(side2,1)-is2
            n2b=dimension(side2,1)-1-is2
            n2c=1
          end if
          
          do side3=0,1
            is3=1-2*side3
            if( bc(side2,1).gt.0 .or. bc(side3,2).gt.0 )then
!             We have to loop over i3 from inside to outside since later points depend on previous ones.
	      n3a=indexRange(side3,2)-is3 
              n3b=dimension(side3,2)
              n3c=-is3
              assignCorners(side1,side2,side3)
            end if
          end do
        end do
      end if
  
      if( isPeriodic(0).eq.0 .and. isPeriodic(1).eq.0 .and. 
     &    isPeriodic(2).eq.0 )then
!           ...Do the points outside vertices in 3D
        do side1=0,1
          is1=1-2*side1
!          I1m= side1==Start ? Range(c.dimension(side1,0)+1-is1,indexRange(side1,0)-is1) :
!       	Range(indexRange(side1,0)-is1,c.dimension(side1,0)-1-is1)
!          Range(indexRange(side2,1)-is2,c.dimension(side2,1)-1-is2)
          if( side1.eq.0 )then
            n1a=dimension(side1,0)+1-is1
            n1b=indexRange(side1,0)-is1
            n1c=1
          else
            n1a=indexRange(side1,0)-is1
            n1b=dimension(side1,0)-1-is1
            n1c=1
          end if
          do side2=0,1 
            is2=1-2*side2
!            I2m= side2==Start ? Range(c.dimension(side2,1)+1-is2,indexRange(side2,1)-is2) :
!            Range(indexRange(side2,1)-is2,c.dimension(side2,1)-1-is2)
            if( side2.eq.0 )then
              n2a=dimension(side2,1)+1-is2
              n2b=indexRange(side2,1)-is2
              n2c=1
            else
              n2a=indexRange(side2,1)-is2
              n2b=dimension(side2,1)-1-is2
              n2c=1
            end if
            do side3=0,1
              is3=1-2*side3
              if( bc(side1,0).gt.0 .or.
     &            bc(side2,1).gt.0 .or.
     &            bc(side3,2).gt.0 )then

                n3a=indexRange(side3,2)-is3 
                n3b=dimension(side3,2)
                n3c=-is3
                assignCorners(side1,side2,side3)
              end if
            end do
          end do
        end do
      end if


      return
      end

