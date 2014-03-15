c You should preprocess this file with the bpp preprocessor before compiling.

c *** macros for extrapolation ***
#defineMacro UX1(i1,i2,i3,js1,js2,js3,c)  \
                         ( u(i1+  (js1),i2+  (js2),i3+  (js3),c))
#defineMacro UX2(i1,i2,i3,js1,js2,js3,c) \
                       (2.*u(i1+  (js1),i2+  (js2),i3+  (js3),c) \
                        -  u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c))
#defineMacro UX3(i1,i2,i3,js1,js2,js3,c) \
                      ( 3.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      - 3.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                         + u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c))
#defineMacro UX4(i1,i2,i3,js1,js2,js3,c)  \
                     (  4.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      - 6.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      + 4.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      -    u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c))
#defineMacro UX5(i1,i2,i3,js1,js2,js3,c)  \
                     (  5.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -10.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +10.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      - 5.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                      +    u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c))
#defineMacro UX6(i1,i2,i3,js1,js2,js3,c) \
                    (   6.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -15.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +20.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      -15.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                      + 6.*u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c)  \
                      -    u(i1+6*(js1),i2+6*(js2),i3+6*(js3),c))
#defineMacro UX7(i1,i2,i3,js1,js2,js3,c) \
                    (   7.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -21.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +35.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      -35.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                      +21.*u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c)  \
                      - 7.*u(i1+6*(js1),i2+6*(js2),i3+6*(js3),c)  \
                      +    u(i1+7*(js1),i2+7*(js2),i3+7*(js3),c))
#defineMacro UX8(i1,i2,i3,js1,js2,js3,c) \
                    (   8.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -28.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +56.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      -70.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                      +56.*u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c)  \
                      -28.*u(i1+6*(js1),i2+6*(js2),i3+6*(js3),c)  \
                      + 8.*u(i1+7*(js1),i2+7*(js2),i3+7*(js3),c)  \
                      -    u(i1+8*(js1),i2+8*(js2),i3+8*(js3),c))
#defineMacro UX9(i1,i2,i3,js1,js2,js3,c)  \
                    (   9.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -36.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +84.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                     -126.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                     +126.*u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c)  \
                      -84.*u(i1+6*(js1),i2+6*(js2),i3+6*(js3),c)  \
                      +36.*u(i1+7*(js1),i2+7*(js2),i3+7*(js3),c)  \
                      - 9.*u(i1+8*(js1),i2+8*(js2),i3+8*(js3),c)  \
                      +    u(i1+9*(js1),i2+9*(js2),i3+9*(js3),c))

#beginMacro beginLoopsWithMask()
  do c=ca,cb
    do i3=n3a,n3b,n3c
      do i2=n2a,n2b,n2c
        do i1=n1a,n1b,n1c
          if( mask(i1,i2,i3).ne.0 )then
#endMacro
#beginMacro endLoopsWithMask()
          end if
        end do
      end do
    end do
  end do
#endMacro

#beginMacro beginLoops()
  do c=ca,cb
    do i3=n3a,n3b,n3c
      do i2=n2a,n2b,n2c
        do i1=n1a,n1b,n1c
#endMacro
#beginMacro endLoops()
        end do
      end do
    end do
  end do
#endMacro

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

#beginMacro loopse8(e1,e2,e3,e4,e5,e6,e7,e8)
if( useWhereMask.ne.0 )then
  do c=ca,cb
    do i3=n3a,n3b,n3c
      do i2=n2a,n2b,n2c
        do i1=n1a,n1b,n1c
          if( mask(i1,i2,i3).ne.0 )then
            e1
            e2
            e3
            e4
            e5
            e6
            e7
            e8
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
            e1
            e2
            e3
            e4
            e5
            e6
            e7
            e8
        end do
      end do
    end do
  end do
end if
#endMacro

#beginMacro evenSymmetryBCMacro()
 do m3=ng3a,ng3b
 do m2=ng2a,ng2b
 do m1=ng1a,ng1b
   u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=u(i1+m1*is1,i2+m2*is2,i3+m3*is3,c)
 end do
 end do
 end do
#endMacro 

#beginMacro oddSymmetryBCMacro()
 do m3=ng3a,ng3b
 do m2=ng2a,ng2b
 do m1=ng1a,ng1b
   u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=2.*u(i1,i2,i3,c)-u(i1+m1*is1,i2+m2*is2,i3+m3*is3,c)
 end do
 end do
 end do
#endMacro

! *************** Vector Symmetry BC Macros ********************

#beginMacro vLoops(expression)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b,n3c
  do i2=n2a,n2b,n2c
  do i1=n1a,n1b,n1c
  if( mask(i1,i2,i3).ne.0 )then

    do m3=ng3a,ng3b
    do m2=ng2a,ng2b
    do m1=ng1a,ng1b

      expression

    end do
    end do
    end do

  end if
  end do
  end do
  end do
else
  do i3=n3a,n3b,n3c
  do i2=n2a,n2b,n2c
  do i1=n1a,n1b,n1c

    do m3=ng3a,ng3b
    do m2=ng2a,ng2b
    do m1=ng1a,ng1b

      expression

    end do
    end do
    end do

  end do
  end do
  end do
end if
#endMacro


#beginMacro vectorSymmetryCartesianBCMacro()
 ! first even symmetry on all components
 k1=i1+m1*ks1
 k2=i2+m2*ks2
 k3=i3+m3*ks3
 do c=ca,cb
   u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=u(k1,k2,k3,c)          
c   write(*,'(" ++ even: Set pt ",4i3," from pt ",4i3)') i1-m1*is1,i2-m2*is2,i3-m3*is3,c,k1,k2,k3,c
 end do
 ! odd symmetry on the normal component
 u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cn)=-u(k1,k2,k3,cn)
#endMacro
          
#beginMacro vectorSymmetryCurvilinearBCMacro(DIM)
 ! ** curvilinear grid ***
 ! DIM : number of dimensions, 2 or 3

 ! first even symmetry on all components
 k1=i1+m1*ks1
 k2=i2+m2*ks2
 k3=i3+m3*ks3
 do c=ca,cb
   u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=u(k1,k2,k3,c)          
 end do
 ! odd symmetry on the normal component
 #If #DIM eq "2" 
  nv(0) = rsxy(i1,i2,i3,axisn,0)
  nv(1) = rsxy(i1,i2,i3,axisn,1)
  nvNorm=sqrt( nv(0)**2 + nv(1)**2 ) + normEps
  nv(0)=nv(0)/nvNorm
  nv(1)=nv(1)/nvNorm
 
  ! nv.uv
  ndum = nv(0)*u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv  )+\
         nv(1)*u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv+1)

  ndup = nv(0)*u(k1,k2,k3,cv  )+\
         nv(1)*u(k1,k2,k3,cv+1)
 
  u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv  )=u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv  )-nv(0)*(ndup+ndum)
  u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv+1)=u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv+1)-nv(1)*(ndup+ndum)

 #Else
  nv(0) = rsxy(i1,i2,i3,axisn,0)
  nv(1) = rsxy(i1,i2,i3,axisn,1)
  nv(2) = rsxy(i1,i2,i3,axisn,2)
  nvNorm=sqrt( nv(0)**2 + nv(1)**2 + nv(2)**2 ) + normEps
  nv(0)=nv(0)/nvNorm
  nv(1)=nv(1)/nvNorm
  nv(2)=nv(2)/nvNorm
 
  ! nv.uv
  ndum = nv(0)*u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv  )+\
         nv(1)*u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv+1)+\
         nv(2)*u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv+2)
 
  ndup = nv(0)*u(k1,k2,k3,cv  )+\
         nv(1)*u(k1,k2,k3,cv+1)+\
         nv(2)*u(k1,k2,k3,cv+2)
 
  u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv  )=u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv  )-nv(0)*(ndup+ndum)
  u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv+1)=u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv+1)-nv(1)*(ndup+ndum)
  u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv+2)=u(i1-m1*is1,i2-m2*is2,i3-m3*is3,cv+2)-nv(2)*(ndup+ndum)
 #End

#endMacro



c ================================================================================================
c  /Description:
c     Apply an extrapolation, symmetry or Taylor-series approximation boundary condition.
c  /i1,i2,i3,n: Indexs of points to assign.
c ===============================================================================================
#beginMacro assignCorners(side1,side2,side3)

if( cornerBC(side1,side2,side3).eq.extrapolateCorner )then

  if( orderOfExtrapolation.gt.9 )then
    write(*,*) 'fixBoundaryCorners:assignCorners:Error: '
    write(*,*) 'unable to extrapolate '
    write(*,*) ' to orderOfExtrapolation',orderOfExtrapolation
    write(*,*) ' can only do orders 1 to 9.'
    stop 1
  end if

  if( is1.eq.0 )then
   n1a=indexRange(0,axis1)
   n1b=indexRange(1,axis1)
   n1c=1
  else
   ! loop from inside to outside
   n1a=indexRange(side1,axis1)-is1
   n1b= dimension(side1,axis1)
   n1c=-is1
  end if

  if( is2.eq.0 )then
   n2a=indexRange(0,axis2)
   n2b=indexRange(1,axis2)
   n2c=1
  else
   ! loop from inside to outside
   n2a=indexRange(side2,axis2)-is2
   n2b= dimension(side2,axis2)
   n2c=-is2
  end if

  if( is3.eq.0 )then
   n3a=indexRange(0,axis3)
   n3b=indexRange(1,axis3)
   n3c=1
  else
   ! loop from inside to outside
   n3a=indexRange(side3,axis3)-is3
   n3b= dimension(side3,axis3)
   n3c=-is3
  end if

  js1=is1
  js2=is2
  js3=is3
  checkForValidExtrapolation=0 ! set to 1 if we need to check the mask for valid extrapolation
  if( cornerExtrapolationOption.ne.0 )then
    ! Use this option to avoid extrapolating corners along a diagonal
    if( cornerExtrapolationOption.eq.1 )then
      js1=0  ! do not extrap along axis1
    else if( cornerExtrapolationOption.eq.2 )then
      js2=0  ! do not extrap along axis2
    else if( cornerExtrapolationOption.eq.3 )then
      js3=0  ! do not extrap along axis3
    end if
  else 
    ! *wdh* 070506 -- At a "corner" where one side is interpolation, do NOT extrap along the diagonal, instead 
    !   extrap in the "normal" direction. (diagonal extrap can fail since the grid generator only makes sure there
    !   are "boundaryDiscretizationWidth" points in the normal direction, -- see quarterSphere.cmd plus tcm3)
    if(      (side1.eq.0 .or. side1.eq.1) .and. bc(side1,0).eq.0 )then
      js1=0
      checkForValidExtrapolation=1
      ! write(*,'("Extrap corner next to interp -- do not extrap along the diagonal")')
    end if
    if( (side2.eq.0 .or. side2.eq.1) .and. bc(side2,1).eq.0 )then
      js2=0
      checkForValidExtrapolation=1
      ! write(*,'("Extrap corner next to interp -- do not extrap along the diagonal")')
    end if
    if( (side3.eq.0 .or. side3.eq.1) .and. bc(side3,2).eq.0 )then
      js3=0
      checkForValidExtrapolation=1
      ! write(*,'("Extrap corner next to interp -- do not extrap along the diagonal")')
    end if
  end if
  if( js1.eq.0 .and. js2.eq.0 .and. js3.eq.0  )then
    write(*,'(''ERROR: extrapolating corners js1.eq.0 .and. js2.eq.0 .and. js3.eq.0'')')
    stop 55
  end if

!  write(*,'(''side1,side2,side3, n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c='',12i4)') side1,side2,side3,n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c
 if( checkForValidExtrapolation.eq.0 )then
  if( orderOfExtrapolation.eq.1 )then
    loops(u(i1,i2,i3,c)=u(i1+  (js1),i2+  (js2),i3+  (js3),c))
  else if( orderOfExtrapolation.eq.2 )then
    loops(u(i1,i2,i3,c)=2.*u(i1+  (js1),i2+  (js2),i3+  (js3),c) \
                      -    u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c))
  else if( orderOfExtrapolation.eq.3 )then
    loops(u(i1,i2,i3,c)= 3.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                       - 3.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                       +    u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c))
  else if( orderOfExtrapolation.eq.4 )then
    loops(u(i1,i2,i3,c)=4.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      - 6.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      + 4.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      -    u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c))
  else if( orderOfExtrapolation.eq.5 )then
    loops(u(i1,i2,i3,c)=5.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -10.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +10.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      - 5.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                      +    u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c))
  else if( orderOfExtrapolation.eq.6 )then
    loops(u(i1,i2,i3,c)=6.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -15.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +20.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      -15.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                      + 6.*u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c)  \
                      -    u(i1+6*(js1),i2+6*(js2),i3+6*(js3),c))
  else if( orderOfExtrapolation.eq.7 )then
    loops(u(i1,i2,i3,c)=7.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -21.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +35.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      -35.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                      +21.*u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c)  \
                      - 7.*u(i1+6*(js1),i2+6*(js2),i3+6*(js3),c)  \
                      +    u(i1+7*(js1),i2+7*(js2),i3+7*(js3),c))
  else if( orderOfExtrapolation.eq.8 )then
    loops(u(i1,i2,i3,c)=8.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -28.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +56.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      -70.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                      +56.*u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c)  \
                      -28.*u(i1+6*(js1),i2+6*(js2),i3+6*(js3),c)  \
                      + 8.*u(i1+7*(js1),i2+7*(js2),i3+7*(js3),c)  \
                      -    u(i1+8*(js1),i2+8*(js2),i3+8*(js3),c))
  else if( orderOfExtrapolation.eq.9 )then
    loops(u(i1,i2,i3,c)=9.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -36.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +84.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                     -126.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                     +126.*u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c)  \
                      -84.*u(i1+6*(js1),i2+6*(js2),i3+6*(js3),c)  \
                      +36.*u(i1+7*(js1),i2+7*(js2),i3+7*(js3),c)  \
                      - 9.*u(i1+8*(js1),i2+8*(js2),i3+8*(js3),c)  \
                      +    u(i1+9*(js1),i2+9*(js2),i3+9*(js3),c))
  end if

 else

  ! check the mask for valid extrapolation and reduce the order of extrapolation as necessary

  do i3=n3a,n3b,n3c
    do i2=n2a,n2b,n2c
      do i1=n1a,n1b,n1c
        m=1
        do while( m.le.orderOfExtrapolation .and. mask(i1+m*(js1),i2+m*(js2),i3+m*(js3)).ne.0 )
          m=m+1
        end do
        orderOfExtrap=m-1  
        if( .false. .and. mask(i1,i2,i3).gt.0 .and. orderOfExtrap.ne.orderOfExtrapolation )then
          write(*,'("Extrap corner:INFO reduce order to ",i3," from ",i3)') orderOfExtrap,orderOfExtrapolation
          write(*,'(" : mask = ",4(i12,1x))') mask(i1,i2,i3),mask(i1+js1,i2+js2,i3+js3),mask(i1+2*(js1),i2+2*(js2),i3+2*(js3)),mask(i1+3*(js1),i2+3*(js2),i3+3*(js3))
          ! '
        end if
        if( orderOfExtrap.gt.0 )then
          do c=ca,cb
            if( orderOfExtrap.eq.1 )then
              u(i1,i2,i3,c)=UX1(i1,i2,i3,js1,js2,js3,c)
            else if( orderOfExtrap.eq.2 )then
              u(i1,i2,i3,c)=UX2(i1,i2,i3,js1,js2,js3,c)
            else if( orderOfExtrap.eq.3 )then
              u(i1,i2,i3,c)=UX3(i1,i2,i3,js1,js2,js3,c)
            else if( orderOfExtrap.eq.4 )then
              u(i1,i2,i3,c)=UX4(i1,i2,i3,js1,js2,js3,c)
            else if( orderOfExtrap.eq.5 )then
              u(i1,i2,i3,c)=UX5(i1,i2,i3,js1,js2,js3,c)
            else if( orderOfExtrap.eq.6 )then
              u(i1,i2,i3,c)=UX6(i1,i2,i3,js1,js2,js3,c)
            else if( orderOfExtrap.eq.7 )then
              u(i1,i2,i3,c)=UX7(i1,i2,i3,js1,js2,js3,c)
            else if( orderOfExtrap.eq.8 )then
              u(i1,i2,i3,c)=UX8(i1,i2,i3,js1,js2,js3,c)
            else if( orderOfExtrap.eq.9 )then
              u(i1,i2,i3,c)=UX9(i1,i2,i3,js1,js2,js3,c)
            end if
          end do
        end if ! orderOfExtrap.gt.0 
      end do
    end do
  end do


 endif

else
  if( is1.eq.0 )then
    n1a=indexRange(0,axis1)
    n1b=indexRange(1,axis1)
    n1c=1
    ng1a=0
    ng1b=0
  else
    n1a=indexRange(side1,axis1)
    n1b=n1a
    n1c=1
    ng1a=1
    ng1b=abs(indexRange(side1,axis1)-dimension(side1,axis1))
  end if
  if( is2.eq.0 )then
    n2a=indexRange(0,axis2)
    n2b=indexRange(1,axis2)
    n2c=1
    ng2a=0
    ng2b=0
  else
    n2a=indexRange(side2,axis2)
    n2b=n2a
    n2c=1
    ng2a=1
    ng2b=abs(indexRange(side2,axis2)-dimension(side2,axis2))
  end if
  if( is3.eq.0 )then
    n3a=indexRange(0,axis3)
    n3b=indexRange(1,axis3)
    n3c=1
    ng3a=0
    ng3b=0
  else
    n3a=indexRange(side3,axis3)
    n3b=n3a
    n3c=1
    ng3a=1
    ng3b=abs(indexRange(side3,axis3)-dimension(side3,axis3))
  end if



 if( cornerBC(side1,side2,side3).eq.evenSymmetryCorner .or. \
          cornerBC(side1,side2,side3).eq.symmetryCorner )then
   !  even symmetry boundary condition 

    loops($evenSymmetryBCMacro())

 else if( cornerBC(side1,side2,side3).eq.oddSymmetryCorner )then
   !  odd symmetry boundary condition 

   loops($oddSymmetryBCMacro())
 
 else if( cornerBC(side1,side2,side3).eq.taylor2ndOrderEvenCorner .or. \
          cornerBC(side1,side2,side3).eq.taylor2ndOrder )then
   !  Use a 2nd-order taylor approximation that preserves even symmetry if present
   if( nd.eq.2 )then
     if( ncg.eq.1 )then
       ! assign 1 line of corner ghost points
       loops(u(i1-is1,i2-is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,is1,is2,c))
     else if( ncg.eq.2 )then
       ! assign 2 lines of corner ghost points
       loopse8(\
         u(i1-  is1,i2-  is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,  is1,  is2,c),\
         u(i1-2*is1,i2-  is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,2*is1,  is2,c),\
         u(i1-  is1,i2-2*is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,  is1,2*is2,c),\
         u(i1-2*is1,i2-2*is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,2*is1,2*is2,c),,,,)
     else if( ncg.gt.2 )then
       ! general case : do all
       if( useWhereMask.ne.0 )then
        beginLoopsWithMask()
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           u(i1-m1*is1,i2-m2*is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,m1*is1,m2*is2,c)
         end do
         end do
        endLoopsWithMask()
       else
        beginLoops()
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           u(i1-m1*is1,i2-m2*is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,m1*is1,m2*is2,c)
         end do
         end do
        endLoops()
       end if
     end if

   else if( nd.eq.3 )then

     if( ncg.eq.1 )then
       ! assign 1 line of corner ghost points
       loops(u(i1-is1,i2-is2,i3-is3,c)=taylor2ndOrderEven3d(i1,i2,i3,is1,is2,is3,c))
     else if( ncg.eq.2 )then
       ! assign 2 lines of corner ghost points
       if( is1.ne.0 .and. is2.ne.0 .and. is3.ne.0 )then
         loopse8(\
           u(i1-  is1,i2-  is2,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1-2*is1,i2-  is2,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,  is2,  is3,c),\
           u(i1-  is1,i2-2*is2,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,2*is2,  is3,c),\
           u(i1-2*is1,i2-2*is2,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,2*is2,  is3,c),\
           u(i1-  is1,i2-  is2,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,2*is3,c),\
           u(i1-2*is1,i2-  is2,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,  is2,2*is3,c),\
           u(i1-  is1,i2-2*is2,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,2*is2,2*is3,c),\
           u(i1-2*is1,i2-2*is2,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,2*is2,2*is3,c))
       else if( is1.ne.0 .and. is2.ne.0 )then
         loopse8(\
           u(i1-  is1,i2-  is2,i3      ,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1-2*is1,i2-  is2,i3      ,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,  is2,  is3,c),\
           u(i1-  is1,i2-2*is2,i3      ,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,2*is2,  is3,c),\
           u(i1-2*is1,i2-2*is2,i3      ,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,2*is2,  is3,c),,,,)
       else if( is1.ne.0 .and. is3.ne.0 )then
         loopse8(\
           u(i1-  is1,i2      ,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1-2*is1,i2      ,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,  is2,  is3,c),\
           u(i1-  is1,i2      ,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,2*is3,c),\
           u(i1-2*is1,i2      ,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,  is2,2*is3,c),,,,)
       else if( is2.ne.0 .and. is3.ne.0 )then
         loopse8(\
           u(i1      ,i2-  is2,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1      ,i2-2*is2,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,2*is2,  is3,c),\
           u(i1      ,i2-  is2,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,2*is3,c),\
           u(i1      ,i2-2*is2,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,2*is2,2*is3,c),,,,)
       else
         write(*,*) 'ERROR: is1=is2=is3=0!'
         stop 21
       end if
     else if( ncg.gt.2 )then
       ! general case : do all
       if( useWhereMask.ne.0 )then
        beginLoopsWithMask()
         do m3=ng3a,ng3b
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,m1*is1,m2*is2,m3*is3,c)
         end do
         end do
         end do
        endLoopsWithMask()
       else
        beginLoops()
         do m3=ng3a,ng3b
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,m1*is1,m2*is2,m3*is3,c)
         end do
         end do
         end do
        endLoops()
      end if
     end if
   end if

 else if( cornerBC(side1,side2,side3).eq.taylor4thOrderEvenCorner )then
   !  Use a 4th-order taylor approximation that preserves even symmetry if present

   if( nd.eq.2 )then
     if( ncg.eq.1 )then
       ! assign 1 line of corner ghost points
       loops(u(i1-is1,i2-is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,is1,is2,c))
     else if( ncg.eq.2 )then
       ! assign 2 lines of corner ghost points
       loopse8(\
         u(i1-  is1,i2-  is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,  is1,  is2,c),\
         u(i1-2*is1,i2-  is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,2*is1,  is2,c),\
         u(i1-  is1,i2-2*is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,  is1,2*is2,c),\
         u(i1-2*is1,i2-2*is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,2*is1,2*is2,c),,,,)
     else if( ncg.gt.2 )then
       ! general case : do all
       if( useWhereMask.ne.0 )then
        beginLoopsWithMask()
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           u(i1-m1*is1,i2-m2*is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,m1*is1,m2*is2,c)
         end do
         end do
        endLoopsWithMask()
       else
        beginLoops()
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           u(i1-m1*is1,i2-m2*is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,m1*is1,m2*is2,c)
         end do
         end do
        endLoops()
       end if
     end if

   else if( nd.eq.3 )then

     mmm(-is1,-is2,-is3)=0 ! for zeroing out the proper term in taylor4thOrderEven3dVertex

     if( ncg.eq.1 )then
       ! assign 1 line of corner ghost points
       loops(u(i1-is1,i2-is2,i3-is3,c)=taylor4thOrderEven3dVertex(i1,i2,i3,is1,is2,is3,c))
     else if( ncg.eq.2 )then
       ! assign 2 lines of corner ghost points
       if( is1.ne.0 .and. is2.ne.0 .and. is3.ne.0 )then
         loopse8(\
           u(i1-  is1,i2-  is2,i3-  is3,c)=taylor4thOrderEven3dVertex(i1,i2,i3,is1,is2,is3,c),\
           u(i1-2*is1,i2-  is2,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,  is2,  is3,c),\
           u(i1-  is1,i2-2*is2,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,2*is2,  is3,c),\
           u(i1-2*is1,i2-2*is2,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,2*is2,  is3,c),\
           u(i1-  is1,i2-  is2,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,  is2,2*is3,c),\
           u(i1-2*is1,i2-  is2,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,  is2,2*is3,c),\
           u(i1-  is1,i2-2*is2,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,2*is2,2*is3,c),\
           u(i1-2*is1,i2-2*is2,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,2*is2,2*is3,c))
       else if( is1.ne.0 .and. is2.ne.0 )then
         loopse8(\
           u(i1-  is1,i2-  is2,i3      ,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1-2*is1,i2-  is2,i3      ,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,  is2,  is3,c),\
           u(i1-  is1,i2-2*is2,i3      ,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,2*is2,  is3,c),\
           u(i1-2*is1,i2-2*is2,i3      ,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,2*is2,  is3,c),,,,)
       else if( is1.ne.0 .and. is3.ne.0 )then
         loopse8(\
           u(i1-  is1,i2      ,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1-2*is1,i2      ,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,  is2,  is3,c),\
           u(i1-  is1,i2      ,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,  is2,2*is3,c),\
           u(i1-2*is1,i2      ,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,  is2,2*is3,c),,,,)
       else if( is2.ne.0 .and. is3.ne.0 )then
         loopse8(\
           u(i1      ,i2-  is2,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1      ,i2-2*is2,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,2*is2,  is3,c),\
           u(i1      ,i2-  is2,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,  is2,2*is3,c),\
           u(i1      ,i2-2*is2,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,2*is2,2*is3,c),,,,)
       else
         write(*,*) 'ERROR: is1=is2=is3=0!'
         stop 21
       end if
     else if( ncg.gt.2 )then
       ! general case : do all
       if( useWhereMask.ne.0 )then
        beginLoopsWithMask()
         do m3=ng3a,ng3b
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           if( m1.eq.1 .and. m2.eq.1 .and. m3.eq.1 )then
             u(i1-  is1,i2-  is2,i3-  is3,c)=taylor4thOrderEven3dVertex(i1,i2,i3,is1,is2,is3,c)
           else
             u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=taylor4thOrderEven3d(i1,i2,i3,m1*is1,m2*is2,m3*is3,c)
           end if
         end do
         end do
         end do
        endLoopsWithMask()
       else
        beginLoops()
         do m3=ng3a,ng3b
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           if( m1.eq.1 .and. m2.eq.1 .and. m3.eq.1 )then
             u(i1-  is1,i2-  is2,i3-  is3,c)=taylor4thOrderEven3dVertex(i1,i2,i3,is1,is2,is3,c)
           else
             u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=taylor4thOrderEven3d(i1,i2,i3,m1*is1,m2*is2,m3*is3,c)
           end if
         end do
         end do
         end do
        endLoops()
       end if
     end if
     mmm(-is1,-is2,-is3)=1 ! reset

   end if

 else if( cornerBC(side1,side2,side3).ge.vectorSymmetryAxis1Corner .and. \
          cornerBC(side1,side2,side3).le.vectorSymmetryAxis3Corner )then
   !  vector symmetry boundary condition 

    ! axisn : axis in the normal direction for vectorSymmetry
    axisn=cornerBC(side1,side2,side3)-vectorSymmetryAxis1Corner ! base 0
    if( axisn.lt.0 .or. axisn.ge.nd )then
      write(*,'("ERROR: vectorSymmetryCorner BC invalid axisn")')
      stop 6254
    end if
    if( cv.lt.ca .or. cv.gt.(cb-nd) )then
      write(*,'("ERROR: vectorSymmetryCorner BC invalid cv")')
      stop 6255
    end if

    ! always take values from direction = axisn:
    if( axisn.eq.0 )then
      js1=is1
      js2=0
      js3=0
    else if( axisn.eq.1 )then
      js1=0
      js2=is2
      js3=0
    else
      js1=0
      js2=0
      js3=is3
    end if
    ks1=2*js1-is1
    ks2=2*js2-is2
    ks3=2*js3-is3

    if( gridType.eq.rectangular )then
      cn=cv+axisn   ! normal component
c      write(*,'(" ** vectSym: side1,side2,side3,axisn,cv,cn =",6i4)') side1,side2,side3,axisn,cv,cn

      vLoops($vectorSymmetryCartesianBCMacro())
    else if( nd.eq.2 )then
      vLoops($vectorSymmetryCurvilinearBCMacro(2))
    else 
      vLoops($vectorSymmetryCurvilinearBCMacro(3))
    end if




 else if( cornerBC(side1,side2,side3).ne.doNothingCorner )then
   write(*,*)'fixBoundaryCorners:Error:'
   write(*,*)' unknown cornerBC=',cornerBC(side1,side2,side3)
 end if
end if
#endMacro



      subroutine fixBoundaryCornersOpt( nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension, 
     & isPeriodic, bc, cornerBC, ipar, rpar )
c======================================================================
c  Optimised Boundary Conditions
c         
c nd : number of space dimensions
c ca,cb : assign components c=uC(ca),..,uC(cb)
c cv : start of vector for vectorSymmetryCorner BC
c useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
c
c ncg: number of corner ghost points to assign
c cornerExtrapolationOption : used to extrapolate corners along certain directions (or really
c  to not extrapolate in certain directions).
c======================================================================
      implicit none
      integer nd, orderOfExtrapolation,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,ncg,cornerExtrapolationOption

      integer useWhereMask,bc(0:1,0:2),isPeriodic(0:2)
      integer indexRange(0:1,0:2),dimension(0:1,0:2)
      integer cornerBC(0:2,0:2,0:2)
      integer ipar(0:*)
      real rpar(0:*),normEps

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real rsxy(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b,0:nd-1,0:nd-1)
      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)

      integer c,ca,cb,n

c........end statement functions

c        ---extrapolate or otherwise assign values outside edges---

      call fixBCOptEdge3( nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension, 
     & isPeriodic, bc, cornerBC, ipar, rpar )

      if( nd.le.2 )then
        return
      end if

      call fixBCOptEdge2( nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension, 
     & isPeriodic, bc, cornerBC, ipar, rpar )

      call fixBCOptEdge1( nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension, 
     & isPeriodic, bc, cornerBC, ipar, rpar )

      call fixBCOptVerticies( nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension, 
     & isPeriodic, bc, cornerBC, ipar, rpar )


      return
      end

#beginFile fixBCOptEdge1.f
      subroutine fixBCOptEdge1( nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension, 
     & isPeriodic, bc, cornerBC, ipar, rpar )
c======================================================================
c  Optimised Boundary Conditions
c         
c nd : number of space dimensions
c ca,cb : assign components c=uC(ca),..,uC(cb)
c cv : start of vector for vectorSymmetryCorner BC
c useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
c
c ncg: number of corner ghost points to assign
c cornerExtrapolationOption : used to extrapolate corners along certain directions (or really
c  to not extrapolate in certain directions).
c======================================================================
      implicit none
      integer nd, orderOfExtrapolation,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,ncg,cornerExtrapolationOption

      integer useWhereMask,bc(0:1,0:2),isPeriodic(0:2)
      integer indexRange(0:1,0:2),dimension(0:1,0:2)
      integer cornerBC(0:2,0:2,0:2)
      integer ipar(0:*)
      real rpar(0:*),normEps

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real rsxy(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b,0:nd-1,0:nd-1)
      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)

      integer c,ca,cb,n,checkForValidExtrapolation,orderOfExtrap,m

      integer doNothingCorner,extrapolateCorner,symmetryCorner,taylor2ndOrder
      integer evenSymmetryCorner,oddSymmetryCorner,taylor2ndOrderEvenCorner,taylor4thOrderEvenCorner,\
              vectorSymmetryAxis1Corner,vectorSymmetryAxis2Corner,vectorSymmetryAxis3Corner

      parameter(doNothingCorner=-1,extrapolateCorner=0,symmetryCorner=1,taylor2ndOrder=2, \
       evenSymmetryCorner=3,oddSymmetryCorner=4,taylor2ndOrderEvenCorner=5,taylor4thOrderEvenCorner=6, \
       vectorSymmetryAxis1Corner=7,vectorSymmetryAxis2Corner=8,vectorSymmetryAxis3Corner=9 )

      integer rectangular,curvilinear
      parameter( rectangular=0,curvilinear=1 )

c     --- local variables 
      integer side1,side2,side3,is1,is2,is3,i1,i2,i3  
      integer n1a,n1b,n1c, n2a,n2b,n2c, n3a,n3b,n3c
      integer ng1a,ng1b,ng2a,ng2b,ng3a,ng3b
      integer js1,js2,js3,cv,gridType
      integer k1,k2,k3,ks1,ks2,ks3

      integer mmm(-1:1,-1:1,-1:1)

      integer cn,axisn
      real nv(0:2),nvNorm,ndum,ndup

      integer axis1,axis2,axis3
      parameter( axis1=0,axis2=1,axis3=2 )
c........begin statement functions
      integer m1,m2,m3
      real taylor2ndOrderEven2d,taylor2ndOrderEven3d
      real taylor4thOrderEven2d,taylor4thOrderEven3d
      real taylor4thOrderEven3dVertex

c     These approximations come from ogmg/bc.maple
      taylor2ndOrderEven2d(i1,i2,i3,m1,m2,c)=-m1*u(i1+1,i2,i3,c)+m1*u(i1-1,i2,i3,c)\
                     -m2*u(i1,i2+1,i3,c)+m2*u(i1,i2-1,i3,c)+u(i1+m1,i2+m2,i3,c)
      taylor2ndOrderEven3d(i1,i2,i3,m1,m2,m3,c)= -m1*u(i1+1,i2,i3,c)+m1*u(i1-1,i2,i3,c)-m2*u(i1,i2+1,i3,c)\
                          +m2*u(i1,i2-1,i3,c)-m3*u(i1,i2,i3+1,c)+m3*u(i1,i2,i3-1,c)+u(i1+m1,i2+m2,i3+m3,c)
      


      taylor4thOrderEven2d(i1,i2,i3,m1,m2,n)=(-3*m1**2*m2*u(i1,i2+1,i3,n)+3*m1**2*m2*u(i1+2*is1,i2-1,i3,n)-6*m1**2*m2*u(i1+is1,i2-1,i3,n)+3*m1**2*m2*u(i1,i2-1,i3,n)-3*m1*m2**2*u(i1+1,i2+2*is2,i3,n)+6*m1*m2**2*u(i1+1,i2+is2,i3,n)-3*m1*m2**2*u(i1+1,i2,i3,n)+3*m1*m2**2*u(i1-1,i2+2*is2,i3,n)-6*m1*m2**2*u(i1-1,i2+is2,i3,n)+3*m1*m2**2*u(i1-1,i2,i3,n)+6*m1**2*m2*u(i1+is1,i2+1,i3,n)-3*m1**2*m2*u(i1+2*is1,i2+1,i3,n)-8*m2*u(i1,i2+1,i3,n)+8*m2*u(i1,i2-1,i3,n)+6*u(i1+m1,i2+m2,i3,n)-m2*u(i1,i2-2,i3,n)-m1**3*u(i1+2,i2,i3,n)+2*m1**3*u(i1+1,i2,i3,n)-2*m1**3*u(i1-1,i2,i3,n)+m1**3*u(i1-2,i2,i3,n)-m2**3*u(i1,i2+2,i3,n)+2*m2**3*u(i1,i2+1,i3,n)-2*m2**3*u(i1,i2-1,i3,n)+m2**3*u(i1,i2-2,i3,n)-6*m1*u(i1+1,i2,i3,n)+6*m1*u(i1-1,i2,i3,n)+m1*u(i1+2,i2,i3,n)-2*m1*u(i1+1,i2,i3,n)+2*m1*u(i1-1,i2,i3,n)-m1*u(i1-2,i2,i3,n)+m2*u(i1,i2+2,i3,n))/6.0


      taylor4thOrderEven3d(i1,i2,i3,m1,m2,m3,n)=(6*m2*m3**2*u(i1,i2-1,i3,n)+6*m2*m3**2*u(i1,i2-1,i3+2*is3,n)+6*m2**2*m3*u(i1,i2,i3-1,n)-12*m2**2*m3*u(i1,i2+is2,i3-1,n)+6*m1*m3**2*u(i1-1,i2,i3,n)-6*m2*m3**2*u(i1,i2+1,i3,n)+12*m2**2*m3*u(i1,i2+is2,i3+1,n)-6*m1**2*m3*u(i1+2*is1,i2,i3+1,n)+12*m1**2*m3*u(i1+is1,i2,i3+1,n)-6*m1**2*m3*u(i1,i2,i3+1,n)+6*m1**2*m3*u(i1+2*is1,i2,i3-1,n)-12*m1**2*m3*u(i1+is1,i2,i3-1,n)+6*m1**2*m3*u(i1,i2,i3-1,n)-6*m1**2*m2*u(i1+2*is1,i2+1,i3,n)+12*m1**2*m2*u(i1+is1,i2+1,i3,n)-6*m1**2*m2*u(i1,i2+1,i3,n)+6*m1**2*m2*u(i1+2*is1,i2-1,i3,n)-12*m1**2*m2*u(i1+is1,i2-1,i3,n)+6*m1**2*m2*u(i1,i2-1,i3,n)-6*m1*m2**2*u(i1+1,i2+2*is2,i3,n)+12*m1*m2**2*u(i1+1,i2+is2,i3,n)-6*m1*m2**2*u(i1+1,i2,i3,n)+6*m1*m2**2*u(i1-1,i2+2*is2,i3,n)-12*m1*m2**2*u(i1-1,i2+is2,i3,n)+6*m1*m2**2*u(i1-1,i2,i3,n)+12*m2*m3**2*u(i1,i2+1,i3+is3,n)+12*m1*m3**2*u(i1+1,i2,i3+is3,n)-12*m1*m3**2*u(i1-1,i2,i3+is3,n)-6*m1*m3**2*u(i1+1,i2,i3,n)-12*m2*m3**2*u(i1,i2-1,i3+is3,n)-6*m2**2*m3*u(i1,i2,i3+1,n)-6*m1*m3**2*u(i1+1,i2,i3+2*is3,n)+6*m1*m3**2*u(i1-1,i2,i3+2*is3,n)-6*m2**2*m3*u(i1,i2+2*is2,i3+1,n)-6*m2*m3**2*u(i1,i2+1,i3+2*is3,n)+6*m2**2*m3*u(i1,i2+2*is2,i3-1,n)+12*u(i1+m1,i2+m2,i3+m3,n)-16*m2*u(i1,i2+1,i3,n)+16*m2*u(i1,i2-1,i3,n)-16*m3*u(i1,i2,i3+1,n)+16*m3*u(i1,i2,i3-1,n)-3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2+is2,i3+is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2+is2,i3-is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2-is2,i3+is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2-is2,i3-is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2+is2,i3+is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2+is2,i3-is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2-is2,i3+is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2-is2,i3-is3,n)-12*m1*u(i1+1,i2,i3,n)+12*m1*u(i1-1,i2,i3,n)+2*m1*u(i1+2,i2,i3,n)-4*m1*u(i1+1,i2,i3,n)+4*m1*u(i1-1,i2,i3,n)-2*m1*u(i1-2,i2,i3,n)+2*m2*u(i1,i2+2,i3,n)-2*m2*u(i1,i2-2,i3,n)+2*m3*u(i1,i2,i3+2,n)-2*m3*u(i1,i2,i3-2,n)-2*m1**3*u(i1+2,i2,i3,n)+4*m1**3*u(i1+1,i2,i3,n)-4*m1**3*u(i1-1,i2,i3,n)+2*m1**3*u(i1-2,i2,i3,n)-2*m2**3*u(i1,i2+2,i3,n)+4*m2**3*u(i1,i2+1,i3,n)-4*m2**3*u(i1,i2-1,i3,n)+2*m2**3*u(i1,i2-2,i3,n)-2*m3**3*u(i1,i2,i3+2,n)+4*m3**3*u(i1,i2,i3+1,n)-4*m3**3*u(i1,i2,i3-1,n)+2*m3**3*u(i1,i2,i3-2,n))/12.0

      taylor4thOrderEven3dVertex(i1,i2,i3,m1,m2,m3,n)=(u(i1+is1,i2+is2,i3-is3,n)+8*is3*u(i1,i2,i3-1,n)-8*is3*u(i1,i2,i3+1,n)+8*is2*u(i1,i2-1,i3,n)-8*is2*u(i1,i2+1,i3,n)-4*is1*u(i1+1,i2,i3,n)+u(i1+is1,i2-is2,i3+is3,n)-u(i1+is1,i2-is2,i3-is3,n)+u(i1-is1,i2+is2,i3+is3,n)-u(i1-is1,i2+is2,i3-is3,n)-u(i1-is1,i2-is2,i3+is3,n)+3*u(i1+is1,i2+is2,i3+is3,n)-2*is2*u(i1+2*is1,i2+1,i3,n)+2*is2*u(i1+2*is1,i2-1,i3,n)-4*is2*u(i1+is1,i2-1,i3,n)+4*is2*u(i1+is1,i2+1,i3,n)-4*is3*u(i1+is1,i2,i3-1,n)+2*is3*u(i1+2*is1,i2,i3-1,n)+4*is3*u(i1+is1,i2,i3+1,n)-2*is3*u(i1+2*is1,i2,i3+1,n)+2*is2*u(i1,i2-1,i3+2*is3,n)+4*is2*u(i1,i2+1,i3+is3,n)+2*is1*u(i1-1,i2,i3+2*is3,n)+2*is1*u(i1-1,i2+2*is2,i3,n)-2*is1*u(i1+1,i2,i3+2*is3,n)+4*is1*u(i1+1,i2,i3+is3,n)-4*is1*u(i1-1,i2+is2,i3,n)+4*is1*u(i1+1,i2+is2,i3,n)-2*is1*u(i1+1,i2+2*is2,i3,n)-4*is1*u(i1-1,i2,i3+is3,n)-2*is2*u(i1,i2+1,i3+2*is3,n)-4*is2*u(i1,i2-1,i3+is3,n)+4*is1*u(i1-1,i2,i3,n)-4*is1*u(i1+1,i2,i3,n)+4*is1*u(i1-1,i2,i3,n)-2*is3*u(i1,i2+2*is2,i3+1,n)+4*is3*u(i1,i2+is2,i3+1,n)-4*is3*u(i1,i2+is2,i3-1,n)+2*is3*u(i1,i2+2*is2,i3-1,n))/3.0


      data mmm/1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1/

c........end statement functions

      ca=ipar(0)
      cb=ipar(1)
      useWhereMask=ipar(2)
      orderOfExtrapolation=ipar(3)
      ncg=ipar(4)
      cornerExtrapolationOption=ipar(5)
      cv=ipar(6)   ! for vector symmetry
      gridType=ipar(7)  

      normEps=rpar(0) ! add this to the normal computation to prevent division by zero.

c        ---extrapolate or otherwise assign values outside edges---

      if( isPeriodic(1).eq.0 .and. isPeriodic(2).eq.0 )then
c          ...Do the four edges parallel to i1
        side1=2 ! this means we are on an edge
        is1=0
        do side2=0,1
          is2=1-2*side2
          do side3=0,1
            is3=1-2*side3
            if( bc(side2,1).gt.0 .or. bc(side3,2).gt.0 )then
c             We have to loop over i3 from inside to outside since later points depend on previous ones.
              assignCorners(side1,side2,side3)
            end if
          end do
        end do
      end if
  

      return
      end
#endFile


#beginFile fixBCOptEdge2.f
      subroutine fixBCOptEdge2( nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension, 
     & isPeriodic, bc, cornerBC, ipar, rpar )
c======================================================================
c  Optimised Boundary Conditions
c         
c nd : number of space dimensions
c ca,cb : assign components c=uC(ca),..,uC(cb)
c cv : start of vector for vectorSymmetryCorner BC
c useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
c
c ncg: number of corner ghost points to assign
c cornerExtrapolationOption : used to extrapolate corners along certain directions (or really
c  to not extrapolate in certain directions).
c======================================================================
      implicit none
      integer nd, orderOfExtrapolation,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,ncg,cornerExtrapolationOption

      integer useWhereMask,bc(0:1,0:2),isPeriodic(0:2)
      integer indexRange(0:1,0:2),dimension(0:1,0:2)
      integer cornerBC(0:2,0:2,0:2)
      integer ipar(0:*)
      real rpar(0:*),normEps

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real rsxy(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b,0:nd-1,0:nd-1)
      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)

      integer c,ca,cb,n,checkForValidExtrapolation,orderOfExtrap,m

      integer doNothingCorner,extrapolateCorner,symmetryCorner,taylor2ndOrder
      integer evenSymmetryCorner,oddSymmetryCorner,taylor2ndOrderEvenCorner,taylor4thOrderEvenCorner,\
              vectorSymmetryAxis1Corner,vectorSymmetryAxis2Corner,vectorSymmetryAxis3Corner

      parameter(doNothingCorner=-1,extrapolateCorner=0,symmetryCorner=1,taylor2ndOrder=2, \
       evenSymmetryCorner=3,oddSymmetryCorner=4,taylor2ndOrderEvenCorner=5,taylor4thOrderEvenCorner=6, \
       vectorSymmetryAxis1Corner=7,vectorSymmetryAxis2Corner=8,vectorSymmetryAxis3Corner=9 )

      integer rectangular,curvilinear
      parameter( rectangular=0,curvilinear=1 )

c     --- local variables 
      integer side1,side2,side3,is1,is2,is3,i1,i2,i3  
      integer n1a,n1b,n1c, n2a,n2b,n2c, n3a,n3b,n3c
      integer ng1a,ng1b,ng2a,ng2b,ng3a,ng3b
      integer js1,js2,js3,cv,gridType
      integer k1,k2,k3,ks1,ks2,ks3

      integer mmm(-1:1,-1:1,-1:1)

      integer cn,axisn
      real nv(0:2),nvNorm,ndum,ndup

      integer axis1,axis2,axis3
      parameter( axis1=0,axis2=1,axis3=2 )
c........begin statement functions
      integer m1,m2,m3
      real taylor2ndOrderEven2d,taylor2ndOrderEven3d
      real taylor4thOrderEven2d,taylor4thOrderEven3d
      real taylor4thOrderEven3dVertex

c     These approximations come from ogmg/bc.maple
      taylor2ndOrderEven2d(i1,i2,i3,m1,m2,c)=-m1*u(i1+1,i2,i3,c)+m1*u(i1-1,i2,i3,c)\
                     -m2*u(i1,i2+1,i3,c)+m2*u(i1,i2-1,i3,c)+u(i1+m1,i2+m2,i3,c)
      taylor2ndOrderEven3d(i1,i2,i3,m1,m2,m3,c)= -m1*u(i1+1,i2,i3,c)+m1*u(i1-1,i2,i3,c)-m2*u(i1,i2+1,i3,c)\
                          +m2*u(i1,i2-1,i3,c)-m3*u(i1,i2,i3+1,c)+m3*u(i1,i2,i3-1,c)+u(i1+m1,i2+m2,i3+m3,c)
      


      taylor4thOrderEven2d(i1,i2,i3,m1,m2,n)=(-3*m1**2*m2*u(i1,i2+1,i3,n)+3*m1**2*m2*u(i1+2*is1,i2-1,i3,n)-6*m1**2*m2*u(i1+is1,i2-1,i3,n)+3*m1**2*m2*u(i1,i2-1,i3,n)-3*m1*m2**2*u(i1+1,i2+2*is2,i3,n)+6*m1*m2**2*u(i1+1,i2+is2,i3,n)-3*m1*m2**2*u(i1+1,i2,i3,n)+3*m1*m2**2*u(i1-1,i2+2*is2,i3,n)-6*m1*m2**2*u(i1-1,i2+is2,i3,n)+3*m1*m2**2*u(i1-1,i2,i3,n)+6*m1**2*m2*u(i1+is1,i2+1,i3,n)-3*m1**2*m2*u(i1+2*is1,i2+1,i3,n)-8*m2*u(i1,i2+1,i3,n)+8*m2*u(i1,i2-1,i3,n)+6*u(i1+m1,i2+m2,i3,n)-m2*u(i1,i2-2,i3,n)-m1**3*u(i1+2,i2,i3,n)+2*m1**3*u(i1+1,i2,i3,n)-2*m1**3*u(i1-1,i2,i3,n)+m1**3*u(i1-2,i2,i3,n)-m2**3*u(i1,i2+2,i3,n)+2*m2**3*u(i1,i2+1,i3,n)-2*m2**3*u(i1,i2-1,i3,n)+m2**3*u(i1,i2-2,i3,n)-6*m1*u(i1+1,i2,i3,n)+6*m1*u(i1-1,i2,i3,n)+m1*u(i1+2,i2,i3,n)-2*m1*u(i1+1,i2,i3,n)+2*m1*u(i1-1,i2,i3,n)-m1*u(i1-2,i2,i3,n)+m2*u(i1,i2+2,i3,n))/6.0


      taylor4thOrderEven3d(i1,i2,i3,m1,m2,m3,n)=(6*m2*m3**2*u(i1,i2-1,i3,n)+6*m2*m3**2*u(i1,i2-1,i3+2*is3,n)+6*m2**2*m3*u(i1,i2,i3-1,n)-12*m2**2*m3*u(i1,i2+is2,i3-1,n)+6*m1*m3**2*u(i1-1,i2,i3,n)-6*m2*m3**2*u(i1,i2+1,i3,n)+12*m2**2*m3*u(i1,i2+is2,i3+1,n)-6*m1**2*m3*u(i1+2*is1,i2,i3+1,n)+12*m1**2*m3*u(i1+is1,i2,i3+1,n)-6*m1**2*m3*u(i1,i2,i3+1,n)+6*m1**2*m3*u(i1+2*is1,i2,i3-1,n)-12*m1**2*m3*u(i1+is1,i2,i3-1,n)+6*m1**2*m3*u(i1,i2,i3-1,n)-6*m1**2*m2*u(i1+2*is1,i2+1,i3,n)+12*m1**2*m2*u(i1+is1,i2+1,i3,n)-6*m1**2*m2*u(i1,i2+1,i3,n)+6*m1**2*m2*u(i1+2*is1,i2-1,i3,n)-12*m1**2*m2*u(i1+is1,i2-1,i3,n)+6*m1**2*m2*u(i1,i2-1,i3,n)-6*m1*m2**2*u(i1+1,i2+2*is2,i3,n)+12*m1*m2**2*u(i1+1,i2+is2,i3,n)-6*m1*m2**2*u(i1+1,i2,i3,n)+6*m1*m2**2*u(i1-1,i2+2*is2,i3,n)-12*m1*m2**2*u(i1-1,i2+is2,i3,n)+6*m1*m2**2*u(i1-1,i2,i3,n)+12*m2*m3**2*u(i1,i2+1,i3+is3,n)+12*m1*m3**2*u(i1+1,i2,i3+is3,n)-12*m1*m3**2*u(i1-1,i2,i3+is3,n)-6*m1*m3**2*u(i1+1,i2,i3,n)-12*m2*m3**2*u(i1,i2-1,i3+is3,n)-6*m2**2*m3*u(i1,i2,i3+1,n)-6*m1*m3**2*u(i1+1,i2,i3+2*is3,n)+6*m1*m3**2*u(i1-1,i2,i3+2*is3,n)-6*m2**2*m3*u(i1,i2+2*is2,i3+1,n)-6*m2*m3**2*u(i1,i2+1,i3+2*is3,n)+6*m2**2*m3*u(i1,i2+2*is2,i3-1,n)+12*u(i1+m1,i2+m2,i3+m3,n)-16*m2*u(i1,i2+1,i3,n)+16*m2*u(i1,i2-1,i3,n)-16*m3*u(i1,i2,i3+1,n)+16*m3*u(i1,i2,i3-1,n)-3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2+is2,i3+is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2+is2,i3-is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2-is2,i3+is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2-is2,i3-is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2+is2,i3+is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2+is2,i3-is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2-is2,i3+is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2-is2,i3-is3,n)-12*m1*u(i1+1,i2,i3,n)+12*m1*u(i1-1,i2,i3,n)+2*m1*u(i1+2,i2,i3,n)-4*m1*u(i1+1,i2,i3,n)+4*m1*u(i1-1,i2,i3,n)-2*m1*u(i1-2,i2,i3,n)+2*m2*u(i1,i2+2,i3,n)-2*m2*u(i1,i2-2,i3,n)+2*m3*u(i1,i2,i3+2,n)-2*m3*u(i1,i2,i3-2,n)-2*m1**3*u(i1+2,i2,i3,n)+4*m1**3*u(i1+1,i2,i3,n)-4*m1**3*u(i1-1,i2,i3,n)+2*m1**3*u(i1-2,i2,i3,n)-2*m2**3*u(i1,i2+2,i3,n)+4*m2**3*u(i1,i2+1,i3,n)-4*m2**3*u(i1,i2-1,i3,n)+2*m2**3*u(i1,i2-2,i3,n)-2*m3**3*u(i1,i2,i3+2,n)+4*m3**3*u(i1,i2,i3+1,n)-4*m3**3*u(i1,i2,i3-1,n)+2*m3**3*u(i1,i2,i3-2,n))/12.0

      taylor4thOrderEven3dVertex(i1,i2,i3,m1,m2,m3,n)=(u(i1+is1,i2+is2,i3-is3,n)+8*is3*u(i1,i2,i3-1,n)-8*is3*u(i1,i2,i3+1,n)+8*is2*u(i1,i2-1,i3,n)-8*is2*u(i1,i2+1,i3,n)-4*is1*u(i1+1,i2,i3,n)+u(i1+is1,i2-is2,i3+is3,n)-u(i1+is1,i2-is2,i3-is3,n)+u(i1-is1,i2+is2,i3+is3,n)-u(i1-is1,i2+is2,i3-is3,n)-u(i1-is1,i2-is2,i3+is3,n)+3*u(i1+is1,i2+is2,i3+is3,n)-2*is2*u(i1+2*is1,i2+1,i3,n)+2*is2*u(i1+2*is1,i2-1,i3,n)-4*is2*u(i1+is1,i2-1,i3,n)+4*is2*u(i1+is1,i2+1,i3,n)-4*is3*u(i1+is1,i2,i3-1,n)+2*is3*u(i1+2*is1,i2,i3-1,n)+4*is3*u(i1+is1,i2,i3+1,n)-2*is3*u(i1+2*is1,i2,i3+1,n)+2*is2*u(i1,i2-1,i3+2*is3,n)+4*is2*u(i1,i2+1,i3+is3,n)+2*is1*u(i1-1,i2,i3+2*is3,n)+2*is1*u(i1-1,i2+2*is2,i3,n)-2*is1*u(i1+1,i2,i3+2*is3,n)+4*is1*u(i1+1,i2,i3+is3,n)-4*is1*u(i1-1,i2+is2,i3,n)+4*is1*u(i1+1,i2+is2,i3,n)-2*is1*u(i1+1,i2+2*is2,i3,n)-4*is1*u(i1-1,i2,i3+is3,n)-2*is2*u(i1,i2+1,i3+2*is3,n)-4*is2*u(i1,i2-1,i3+is3,n)+4*is1*u(i1-1,i2,i3,n)-4*is1*u(i1+1,i2,i3,n)+4*is1*u(i1-1,i2,i3,n)-2*is3*u(i1,i2+2*is2,i3+1,n)+4*is3*u(i1,i2+is2,i3+1,n)-4*is3*u(i1,i2+is2,i3-1,n)+2*is3*u(i1,i2+2*is2,i3-1,n))/3.0


      data mmm/1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1/

c........end statement functions

      ca=ipar(0)
      cb=ipar(1)
      useWhereMask=ipar(2)
      orderOfExtrapolation=ipar(3)
      ncg=ipar(4)
      cornerExtrapolationOption=ipar(5)
      cv=ipar(6)   ! for vector symmetry
      gridType=ipar(7)  

      normEps=rpar(0) ! add this to the normal computation to prevent division by zero.

c        ---extrapolate or otherwise assign values outside edges---

      if( isPeriodic(0).eq.0 .and. isPeriodic(2).eq.0 )then
c     ...Do the four edges parallel to i2
        side2=2 ! this means we are on an edge
        is2=0
        do side1=0,1
          is1=1-2*side1
          do side3=0,1
            is3=1-2*side3
            if( bc(side1,0).gt.0 .or. bc(side3,2).gt.0 )then
	      assignCorners(side1,side2,side3)
            end if
          end do
        end do
      end if


      return
      end

#endFile

#beginFile fixBCOptEdge3.f
      subroutine fixBCOptEdge3( nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension, 
     & isPeriodic, bc, cornerBC, ipar, rpar )
c======================================================================
c  Optimised Boundary Conditions
c         
c nd : number of space dimensions
c ca,cb : assign components c=uC(ca),..,uC(cb)
c cv : start of vector for vectorSymmetryCorner BC
c useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
c
c ncg: number of corner ghost points to assign
c cornerExtrapolationOption : used to extrapolate corners along certain directions (or really
c  to not extrapolate in certain directions).
c======================================================================
      implicit none
      integer nd, orderOfExtrapolation,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,ncg,cornerExtrapolationOption

      integer useWhereMask,bc(0:1,0:2),isPeriodic(0:2)
      integer indexRange(0:1,0:2),dimension(0:1,0:2)
      integer cornerBC(0:2,0:2,0:2)
      integer ipar(0:*)
      real rpar(0:*),normEps

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real rsxy(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b,0:nd-1,0:nd-1)
      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)

      integer c,ca,cb,n,checkForValidExtrapolation,orderOfExtrap,m

      integer doNothingCorner,extrapolateCorner,symmetryCorner,taylor2ndOrder
      integer evenSymmetryCorner,oddSymmetryCorner,taylor2ndOrderEvenCorner,taylor4thOrderEvenCorner,\
              vectorSymmetryAxis1Corner,vectorSymmetryAxis2Corner,vectorSymmetryAxis3Corner

      parameter(doNothingCorner=-1,extrapolateCorner=0,symmetryCorner=1,taylor2ndOrder=2, \
       evenSymmetryCorner=3,oddSymmetryCorner=4,taylor2ndOrderEvenCorner=5,taylor4thOrderEvenCorner=6, \
       vectorSymmetryAxis1Corner=7,vectorSymmetryAxis2Corner=8,vectorSymmetryAxis3Corner=9 )

      integer rectangular,curvilinear
      parameter( rectangular=0,curvilinear=1 )

c     --- local variables 
      integer side1,side2,side3,is1,is2,is3,i1,i2,i3  
      integer n1a,n1b,n1c, n2a,n2b,n2c, n3a,n3b,n3c
      integer ng1a,ng1b,ng2a,ng2b,ng3a,ng3b
      integer js1,js2,js3,cv,gridType
      integer k1,k2,k3,ks1,ks2,ks3

      integer mmm(-1:1,-1:1,-1:1)

      integer cn,axisn
      real nv(0:2),nvNorm,ndum,ndup

      integer axis1,axis2,axis3
      parameter( axis1=0,axis2=1,axis3=2 )
c........begin statement functions
      integer m1,m2,m3
      real taylor2ndOrderEven2d,taylor2ndOrderEven3d
      real taylor4thOrderEven2d,taylor4thOrderEven3d
      real taylor4thOrderEven3dVertex

c     These approximations come from ogmg/bc.maple
      taylor2ndOrderEven2d(i1,i2,i3,m1,m2,c)=-m1*u(i1+1,i2,i3,c)+m1*u(i1-1,i2,i3,c)\
                     -m2*u(i1,i2+1,i3,c)+m2*u(i1,i2-1,i3,c)+u(i1+m1,i2+m2,i3,c)
      taylor2ndOrderEven3d(i1,i2,i3,m1,m2,m3,c)= -m1*u(i1+1,i2,i3,c)+m1*u(i1-1,i2,i3,c)-m2*u(i1,i2+1,i3,c)\
                          +m2*u(i1,i2-1,i3,c)-m3*u(i1,i2,i3+1,c)+m3*u(i1,i2,i3-1,c)+u(i1+m1,i2+m2,i3+m3,c)
      


      taylor4thOrderEven2d(i1,i2,i3,m1,m2,n)=(-3*m1**2*m2*u(i1,i2+1,i3,n)+3*m1**2*m2*u(i1+2*is1,i2-1,i3,n)-6*m1**2*m2*u(i1+is1,i2-1,i3,n)+3*m1**2*m2*u(i1,i2-1,i3,n)-3*m1*m2**2*u(i1+1,i2+2*is2,i3,n)+6*m1*m2**2*u(i1+1,i2+is2,i3,n)-3*m1*m2**2*u(i1+1,i2,i3,n)+3*m1*m2**2*u(i1-1,i2+2*is2,i3,n)-6*m1*m2**2*u(i1-1,i2+is2,i3,n)+3*m1*m2**2*u(i1-1,i2,i3,n)+6*m1**2*m2*u(i1+is1,i2+1,i3,n)-3*m1**2*m2*u(i1+2*is1,i2+1,i3,n)-8*m2*u(i1,i2+1,i3,n)+8*m2*u(i1,i2-1,i3,n)+6*u(i1+m1,i2+m2,i3,n)-m2*u(i1,i2-2,i3,n)-m1**3*u(i1+2,i2,i3,n)+2*m1**3*u(i1+1,i2,i3,n)-2*m1**3*u(i1-1,i2,i3,n)+m1**3*u(i1-2,i2,i3,n)-m2**3*u(i1,i2+2,i3,n)+2*m2**3*u(i1,i2+1,i3,n)-2*m2**3*u(i1,i2-1,i3,n)+m2**3*u(i1,i2-2,i3,n)-6*m1*u(i1+1,i2,i3,n)+6*m1*u(i1-1,i2,i3,n)+m1*u(i1+2,i2,i3,n)-2*m1*u(i1+1,i2,i3,n)+2*m1*u(i1-1,i2,i3,n)-m1*u(i1-2,i2,i3,n)+m2*u(i1,i2+2,i3,n))/6.0


      taylor4thOrderEven3d(i1,i2,i3,m1,m2,m3,n)=(6*m2*m3**2*u(i1,i2-1,i3,n)+6*m2*m3**2*u(i1,i2-1,i3+2*is3,n)+6*m2**2*m3*u(i1,i2,i3-1,n)-12*m2**2*m3*u(i1,i2+is2,i3-1,n)+6*m1*m3**2*u(i1-1,i2,i3,n)-6*m2*m3**2*u(i1,i2+1,i3,n)+12*m2**2*m3*u(i1,i2+is2,i3+1,n)-6*m1**2*m3*u(i1+2*is1,i2,i3+1,n)+12*m1**2*m3*u(i1+is1,i2,i3+1,n)-6*m1**2*m3*u(i1,i2,i3+1,n)+6*m1**2*m3*u(i1+2*is1,i2,i3-1,n)-12*m1**2*m3*u(i1+is1,i2,i3-1,n)+6*m1**2*m3*u(i1,i2,i3-1,n)-6*m1**2*m2*u(i1+2*is1,i2+1,i3,n)+12*m1**2*m2*u(i1+is1,i2+1,i3,n)-6*m1**2*m2*u(i1,i2+1,i3,n)+6*m1**2*m2*u(i1+2*is1,i2-1,i3,n)-12*m1**2*m2*u(i1+is1,i2-1,i3,n)+6*m1**2*m2*u(i1,i2-1,i3,n)-6*m1*m2**2*u(i1+1,i2+2*is2,i3,n)+12*m1*m2**2*u(i1+1,i2+is2,i3,n)-6*m1*m2**2*u(i1+1,i2,i3,n)+6*m1*m2**2*u(i1-1,i2+2*is2,i3,n)-12*m1*m2**2*u(i1-1,i2+is2,i3,n)+6*m1*m2**2*u(i1-1,i2,i3,n)+12*m2*m3**2*u(i1,i2+1,i3+is3,n)+12*m1*m3**2*u(i1+1,i2,i3+is3,n)-12*m1*m3**2*u(i1-1,i2,i3+is3,n)-6*m1*m3**2*u(i1+1,i2,i3,n)-12*m2*m3**2*u(i1,i2-1,i3+is3,n)-6*m2**2*m3*u(i1,i2,i3+1,n)-6*m1*m3**2*u(i1+1,i2,i3+2*is3,n)+6*m1*m3**2*u(i1-1,i2,i3+2*is3,n)-6*m2**2*m3*u(i1,i2+2*is2,i3+1,n)-6*m2*m3**2*u(i1,i2+1,i3+2*is3,n)+6*m2**2*m3*u(i1,i2+2*is2,i3-1,n)+12*u(i1+m1,i2+m2,i3+m3,n)-16*m2*u(i1,i2+1,i3,n)+16*m2*u(i1,i2-1,i3,n)-16*m3*u(i1,i2,i3+1,n)+16*m3*u(i1,i2,i3-1,n)-3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2+is2,i3+is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2+is2,i3-is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2-is2,i3+is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2-is2,i3-is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2+is2,i3+is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2+is2,i3-is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2-is2,i3+is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2-is2,i3-is3,n)-12*m1*u(i1+1,i2,i3,n)+12*m1*u(i1-1,i2,i3,n)+2*m1*u(i1+2,i2,i3,n)-4*m1*u(i1+1,i2,i3,n)+4*m1*u(i1-1,i2,i3,n)-2*m1*u(i1-2,i2,i3,n)+2*m2*u(i1,i2+2,i3,n)-2*m2*u(i1,i2-2,i3,n)+2*m3*u(i1,i2,i3+2,n)-2*m3*u(i1,i2,i3-2,n)-2*m1**3*u(i1+2,i2,i3,n)+4*m1**3*u(i1+1,i2,i3,n)-4*m1**3*u(i1-1,i2,i3,n)+2*m1**3*u(i1-2,i2,i3,n)-2*m2**3*u(i1,i2+2,i3,n)+4*m2**3*u(i1,i2+1,i3,n)-4*m2**3*u(i1,i2-1,i3,n)+2*m2**3*u(i1,i2-2,i3,n)-2*m3**3*u(i1,i2,i3+2,n)+4*m3**3*u(i1,i2,i3+1,n)-4*m3**3*u(i1,i2,i3-1,n)+2*m3**3*u(i1,i2,i3-2,n))/12.0

      taylor4thOrderEven3dVertex(i1,i2,i3,m1,m2,m3,n)=(u(i1+is1,i2+is2,i3-is3,n)+8*is3*u(i1,i2,i3-1,n)-8*is3*u(i1,i2,i3+1,n)+8*is2*u(i1,i2-1,i3,n)-8*is2*u(i1,i2+1,i3,n)-4*is1*u(i1+1,i2,i3,n)+u(i1+is1,i2-is2,i3+is3,n)-u(i1+is1,i2-is2,i3-is3,n)+u(i1-is1,i2+is2,i3+is3,n)-u(i1-is1,i2+is2,i3-is3,n)-u(i1-is1,i2-is2,i3+is3,n)+3*u(i1+is1,i2+is2,i3+is3,n)-2*is2*u(i1+2*is1,i2+1,i3,n)+2*is2*u(i1+2*is1,i2-1,i3,n)-4*is2*u(i1+is1,i2-1,i3,n)+4*is2*u(i1+is1,i2+1,i3,n)-4*is3*u(i1+is1,i2,i3-1,n)+2*is3*u(i1+2*is1,i2,i3-1,n)+4*is3*u(i1+is1,i2,i3+1,n)-2*is3*u(i1+2*is1,i2,i3+1,n)+2*is2*u(i1,i2-1,i3+2*is3,n)+4*is2*u(i1,i2+1,i3+is3,n)+2*is1*u(i1-1,i2,i3+2*is3,n)+2*is1*u(i1-1,i2+2*is2,i3,n)-2*is1*u(i1+1,i2,i3+2*is3,n)+4*is1*u(i1+1,i2,i3+is3,n)-4*is1*u(i1-1,i2+is2,i3,n)+4*is1*u(i1+1,i2+is2,i3,n)-2*is1*u(i1+1,i2+2*is2,i3,n)-4*is1*u(i1-1,i2,i3+is3,n)-2*is2*u(i1,i2+1,i3+2*is3,n)-4*is2*u(i1,i2-1,i3+is3,n)+4*is1*u(i1-1,i2,i3,n)-4*is1*u(i1+1,i2,i3,n)+4*is1*u(i1-1,i2,i3,n)-2*is3*u(i1,i2+2*is2,i3+1,n)+4*is3*u(i1,i2+is2,i3+1,n)-4*is3*u(i1,i2+is2,i3-1,n)+2*is3*u(i1,i2+2*is2,i3-1,n))/3.0


      data mmm/1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1/

c........end statement functions

      ca=ipar(0)
      cb=ipar(1)
      useWhereMask=ipar(2)
      orderOfExtrapolation=ipar(3)
      ncg=ipar(4)
      cornerExtrapolationOption=ipar(5)
      cv=ipar(6)   ! for vector symmetry
      gridType=ipar(7)  

      normEps=rpar(0) ! add this to the normal computation to prevent division by zero.

c        ---extrapolate or otherwise assign values outside edges---

      if( isPeriodic(0).eq.0 .and. isPeriodic(1).eq.0 )then
c     ...Do the four edges parallel to i3
        side3=2 ! this means we are on an edge
        is3=0
        do side1=0,1
          is1=1-2*side1
          do side2=0,1
   	    is2=1-2*side2
	    if( bc(side1,0).gt.0 .or. bc(side2,1).gt.0 )then
	      assignCorners(side1,side2,side3)
            end if
          end do
        end do
 
      end if

      return
      end

#endFile

#beginFile fixBCOptVerticies.f
      subroutine fixBCOptVerticies( nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension, 
     & isPeriodic, bc, cornerBC, ipar, rpar )
c======================================================================
c  Optimised Boundary Conditions
c         
c nd : number of space dimensions
c ca,cb : assign components c=uC(ca),..,uC(cb)
c cv : start of vector for vectorSymmetryCorner BC
c useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
c
c ncg: number of corner ghost points to assign
c cornerExtrapolationOption : used to extrapolate corners along certain directions (or really
c  to not extrapolate in certain directions).
c======================================================================
      implicit none
      integer nd, orderOfExtrapolation,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,ncg,cornerExtrapolationOption

      integer useWhereMask,bc(0:1,0:2),isPeriodic(0:2)
      integer indexRange(0:1,0:2),dimension(0:1,0:2)
      integer cornerBC(0:2,0:2,0:2)
      integer ipar(0:*)
      real rpar(0:*),normEps

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real rsxy(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b,0:nd-1,0:nd-1)
      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)

      integer c,ca,cb,n,checkForValidExtrapolation,orderOfExtrap,m

      integer doNothingCorner,extrapolateCorner,symmetryCorner,taylor2ndOrder
      integer evenSymmetryCorner,oddSymmetryCorner,taylor2ndOrderEvenCorner,taylor4thOrderEvenCorner,\
              vectorSymmetryAxis1Corner,vectorSymmetryAxis2Corner,vectorSymmetryAxis3Corner

      parameter(doNothingCorner=-1,extrapolateCorner=0,symmetryCorner=1,taylor2ndOrder=2, \
       evenSymmetryCorner=3,oddSymmetryCorner=4,taylor2ndOrderEvenCorner=5,taylor4thOrderEvenCorner=6, \
       vectorSymmetryAxis1Corner=7,vectorSymmetryAxis2Corner=8,vectorSymmetryAxis3Corner=9 )

      integer rectangular,curvilinear
      parameter( rectangular=0,curvilinear=1 )

c     --- local variables 
      integer side1,side2,side3,is1,is2,is3,i1,i2,i3  
      integer n1a,n1b,n1c, n2a,n2b,n2c, n3a,n3b,n3c
      integer ng1a,ng1b,ng2a,ng2b,ng3a,ng3b
      integer js1,js2,js3,cv,gridType
      integer k1,k2,k3,ks1,ks2,ks3

      integer mmm(-1:1,-1:1,-1:1)

      integer cn,axisn
      real nv(0:2),nvNorm,ndum,ndup

      integer axis1,axis2,axis3
      parameter( axis1=0,axis2=1,axis3=2 )
c........begin statement functions
      integer m1,m2,m3
      real taylor2ndOrderEven2d,taylor2ndOrderEven3d
      real taylor4thOrderEven2d,taylor4thOrderEven3d
      real taylor4thOrderEven3dVertex

c     These approximations come from ogmg/bc.maple
      taylor2ndOrderEven2d(i1,i2,i3,m1,m2,c)=-m1*u(i1+1,i2,i3,c)+m1*u(i1-1,i2,i3,c)\
                     -m2*u(i1,i2+1,i3,c)+m2*u(i1,i2-1,i3,c)+u(i1+m1,i2+m2,i3,c)
      taylor2ndOrderEven3d(i1,i2,i3,m1,m2,m3,c)= -m1*u(i1+1,i2,i3,c)+m1*u(i1-1,i2,i3,c)-m2*u(i1,i2+1,i3,c)\
                          +m2*u(i1,i2-1,i3,c)-m3*u(i1,i2,i3+1,c)+m3*u(i1,i2,i3-1,c)+u(i1+m1,i2+m2,i3+m3,c)
      


      taylor4thOrderEven2d(i1,i2,i3,m1,m2,n)=(-3*m1**2*m2*u(i1,i2+1,i3,n)+3*m1**2*m2*u(i1+2*is1,i2-1,i3,n)-6*m1**2*m2*u(i1+is1,i2-1,i3,n)+3*m1**2*m2*u(i1,i2-1,i3,n)-3*m1*m2**2*u(i1+1,i2+2*is2,i3,n)+6*m1*m2**2*u(i1+1,i2+is2,i3,n)-3*m1*m2**2*u(i1+1,i2,i3,n)+3*m1*m2**2*u(i1-1,i2+2*is2,i3,n)-6*m1*m2**2*u(i1-1,i2+is2,i3,n)+3*m1*m2**2*u(i1-1,i2,i3,n)+6*m1**2*m2*u(i1+is1,i2+1,i3,n)-3*m1**2*m2*u(i1+2*is1,i2+1,i3,n)-8*m2*u(i1,i2+1,i3,n)+8*m2*u(i1,i2-1,i3,n)+6*u(i1+m1,i2+m2,i3,n)-m2*u(i1,i2-2,i3,n)-m1**3*u(i1+2,i2,i3,n)+2*m1**3*u(i1+1,i2,i3,n)-2*m1**3*u(i1-1,i2,i3,n)+m1**3*u(i1-2,i2,i3,n)-m2**3*u(i1,i2+2,i3,n)+2*m2**3*u(i1,i2+1,i3,n)-2*m2**3*u(i1,i2-1,i3,n)+m2**3*u(i1,i2-2,i3,n)-6*m1*u(i1+1,i2,i3,n)+6*m1*u(i1-1,i2,i3,n)+m1*u(i1+2,i2,i3,n)-2*m1*u(i1+1,i2,i3,n)+2*m1*u(i1-1,i2,i3,n)-m1*u(i1-2,i2,i3,n)+m2*u(i1,i2+2,i3,n))/6.0


      taylor4thOrderEven3d(i1,i2,i3,m1,m2,m3,n)=(6*m2*m3**2*u(i1,i2-1,i3,n)+6*m2*m3**2*u(i1,i2-1,i3+2*is3,n)+6*m2**2*m3*u(i1,i2,i3-1,n)-12*m2**2*m3*u(i1,i2+is2,i3-1,n)+6*m1*m3**2*u(i1-1,i2,i3,n)-6*m2*m3**2*u(i1,i2+1,i3,n)+12*m2**2*m3*u(i1,i2+is2,i3+1,n)-6*m1**2*m3*u(i1+2*is1,i2,i3+1,n)+12*m1**2*m3*u(i1+is1,i2,i3+1,n)-6*m1**2*m3*u(i1,i2,i3+1,n)+6*m1**2*m3*u(i1+2*is1,i2,i3-1,n)-12*m1**2*m3*u(i1+is1,i2,i3-1,n)+6*m1**2*m3*u(i1,i2,i3-1,n)-6*m1**2*m2*u(i1+2*is1,i2+1,i3,n)+12*m1**2*m2*u(i1+is1,i2+1,i3,n)-6*m1**2*m2*u(i1,i2+1,i3,n)+6*m1**2*m2*u(i1+2*is1,i2-1,i3,n)-12*m1**2*m2*u(i1+is1,i2-1,i3,n)+6*m1**2*m2*u(i1,i2-1,i3,n)-6*m1*m2**2*u(i1+1,i2+2*is2,i3,n)+12*m1*m2**2*u(i1+1,i2+is2,i3,n)-6*m1*m2**2*u(i1+1,i2,i3,n)+6*m1*m2**2*u(i1-1,i2+2*is2,i3,n)-12*m1*m2**2*u(i1-1,i2+is2,i3,n)+6*m1*m2**2*u(i1-1,i2,i3,n)+12*m2*m3**2*u(i1,i2+1,i3+is3,n)+12*m1*m3**2*u(i1+1,i2,i3+is3,n)-12*m1*m3**2*u(i1-1,i2,i3+is3,n)-6*m1*m3**2*u(i1+1,i2,i3,n)-12*m2*m3**2*u(i1,i2-1,i3+is3,n)-6*m2**2*m3*u(i1,i2,i3+1,n)-6*m1*m3**2*u(i1+1,i2,i3+2*is3,n)+6*m1*m3**2*u(i1-1,i2,i3+2*is3,n)-6*m2**2*m3*u(i1,i2+2*is2,i3+1,n)-6*m2*m3**2*u(i1,i2+1,i3+2*is3,n)+6*m2**2*m3*u(i1,i2+2*is2,i3-1,n)+12*u(i1+m1,i2+m2,i3+m3,n)-16*m2*u(i1,i2+1,i3,n)+16*m2*u(i1,i2-1,i3,n)-16*m3*u(i1,i2,i3+1,n)+16*m3*u(i1,i2,i3-1,n)-3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2+is2,i3+is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2+is2,i3-is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2-is2,i3+is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2-is2,i3-is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2+is2,i3+is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2+is2,i3-is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2-is2,i3+is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2-is2,i3-is3,n)-12*m1*u(i1+1,i2,i3,n)+12*m1*u(i1-1,i2,i3,n)+2*m1*u(i1+2,i2,i3,n)-4*m1*u(i1+1,i2,i3,n)+4*m1*u(i1-1,i2,i3,n)-2*m1*u(i1-2,i2,i3,n)+2*m2*u(i1,i2+2,i3,n)-2*m2*u(i1,i2-2,i3,n)+2*m3*u(i1,i2,i3+2,n)-2*m3*u(i1,i2,i3-2,n)-2*m1**3*u(i1+2,i2,i3,n)+4*m1**3*u(i1+1,i2,i3,n)-4*m1**3*u(i1-1,i2,i3,n)+2*m1**3*u(i1-2,i2,i3,n)-2*m2**3*u(i1,i2+2,i3,n)+4*m2**3*u(i1,i2+1,i3,n)-4*m2**3*u(i1,i2-1,i3,n)+2*m2**3*u(i1,i2-2,i3,n)-2*m3**3*u(i1,i2,i3+2,n)+4*m3**3*u(i1,i2,i3+1,n)-4*m3**3*u(i1,i2,i3-1,n)+2*m3**3*u(i1,i2,i3-2,n))/12.0

      taylor4thOrderEven3dVertex(i1,i2,i3,m1,m2,m3,n)=(u(i1+is1,i2+is2,i3-is3,n)+8*is3*u(i1,i2,i3-1,n)-8*is3*u(i1,i2,i3+1,n)+8*is2*u(i1,i2-1,i3,n)-8*is2*u(i1,i2+1,i3,n)-4*is1*u(i1+1,i2,i3,n)+u(i1+is1,i2-is2,i3+is3,n)-u(i1+is1,i2-is2,i3-is3,n)+u(i1-is1,i2+is2,i3+is3,n)-u(i1-is1,i2+is2,i3-is3,n)-u(i1-is1,i2-is2,i3+is3,n)+3*u(i1+is1,i2+is2,i3+is3,n)-2*is2*u(i1+2*is1,i2+1,i3,n)+2*is2*u(i1+2*is1,i2-1,i3,n)-4*is2*u(i1+is1,i2-1,i3,n)+4*is2*u(i1+is1,i2+1,i3,n)-4*is3*u(i1+is1,i2,i3-1,n)+2*is3*u(i1+2*is1,i2,i3-1,n)+4*is3*u(i1+is1,i2,i3+1,n)-2*is3*u(i1+2*is1,i2,i3+1,n)+2*is2*u(i1,i2-1,i3+2*is3,n)+4*is2*u(i1,i2+1,i3+is3,n)+2*is1*u(i1-1,i2,i3+2*is3,n)+2*is1*u(i1-1,i2+2*is2,i3,n)-2*is1*u(i1+1,i2,i3+2*is3,n)+4*is1*u(i1+1,i2,i3+is3,n)-4*is1*u(i1-1,i2+is2,i3,n)+4*is1*u(i1+1,i2+is2,i3,n)-2*is1*u(i1+1,i2+2*is2,i3,n)-4*is1*u(i1-1,i2,i3+is3,n)-2*is2*u(i1,i2+1,i3+2*is3,n)-4*is2*u(i1,i2-1,i3+is3,n)+4*is1*u(i1-1,i2,i3,n)-4*is1*u(i1+1,i2,i3,n)+4*is1*u(i1-1,i2,i3,n)-2*is3*u(i1,i2+2*is2,i3+1,n)+4*is3*u(i1,i2+is2,i3+1,n)-4*is3*u(i1,i2+is2,i3-1,n)+2*is3*u(i1,i2+2*is2,i3-1,n))/3.0


      data mmm/1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1/

c........end statement functions

      ca=ipar(0)
      cb=ipar(1)
      useWhereMask=ipar(2)
      orderOfExtrapolation=ipar(3)
      ncg=ipar(4)
      cornerExtrapolationOption=ipar(5)
      cv=ipar(6)   ! for vector symmetry
      gridType=ipar(7)  

      normEps=rpar(0) ! add this to the normal computation to prevent division by zero.

c        ---extrapolate or otherwise assign values outside edges---

      if( isPeriodic(0).eq.0 .and. isPeriodic(1).eq.0 .and. 
     &    isPeriodic(2).eq.0 )then
c           ...Do the points outside vertices in 3D
        do side1=0,1
          is1=1-2*side1
          do side2=0,1 
            is2=1-2*side2
            do side3=0,1
              is3=1-2*side3
              if( bc(side1,0).gt.0 .or. bc(side2,1).gt.0 .or. bc(side3,2).gt.0 )then
c     write(*,'(''n1a,n1b,n2a,n2b,n3a,n3b,n3c='',6i4,'' cornerBC='',i4)') n1a,n1b,n2a,n2b,n3a,n3b,n3c,cornerBC(side1,side2,side3)                
c     write(*,'(''orderOfExtrapolation='',i4)') orderOfExtrapolation
                assignCorners(side1,side2,side3)
              end if
            end do
          end do
        end do
      end if


      return
      end

#endFile
