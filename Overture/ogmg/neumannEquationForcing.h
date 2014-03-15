! This Ogmg macro defines the forcing for fourth-order Neumann boundary conditions

! define derivatives in the r, s, and t directions:

! 2nd-order centered: 
#defineMacro FR(i1,i2,i3) ((f(i1+1,i2,i3)-f(i1-1,i2,i3))*D12(0))
#defineMacro FS(i1,i2,i3) ((f(i1,i2+1,i3)-f(i1,i2-1,i3))*D12(1))
#defineMacro FT(i1,i2,i3) ((f(i1,i2,i3+1)-f(i1,i2,i3-1))*D12(2))

#defineMacro FRR(i1,i2,i3) ((f(i1+1,i2,i3)-2.*f(i1,i2,i3)+f(i1-1,i2,i3))*D22(0))
#defineMacro FSS(i1,i2,i3) ((f(i1,i2+1,i3)-2.*f(i1,i2,i3)+f(i1,i2-1,i3))*D22(1))
#defineMacro FTT(i1,i2,i3) ((f(i1,i2,i3+1)-2.*f(i1,i2,i3)+f(i1,i2,i3-1))*D22(2))


! one-sided approximations on the left:
#defineMacro FRa(i1,i2,i3) ((-f(i1+2,i2,i3)+4.*f(i1+1,i2,i3)-3.*f(i1,i2,i3))*D12(0))
#defineMacro FSa(i1,i2,i3) ((-f(i1,i2+2,i3)+4.*f(i1,i2+1,i3)-3.*f(i1,i2,i3))*D12(1))
#defineMacro FTa(i1,i2,i3) ((-f(i1,i2,i3+2)+4.*f(i1,i2,i3+1)-3.*f(i1,i2,i3))*D12(2))

#defineMacro FRRa(i1,i2,i3) ((2.*f(i1,i2,i3)-5.*f(i1+1,i2,i3)+4.*f(i1+2,i2,i3)-f(i1+3,i2,i3))*D22(0))
#defineMacro FSSa(i1,i2,i3) ((2.*f(i1,i2,i3)-5.*f(i1,i2+1,i3)+4.*f(i1,i2+2,i3)-f(i1,i2+3,i3))*D22(1))
#defineMacro FTTa(i1,i2,i3) ((2.*f(i1,i2,i3)-5.*f(i1,i2,i3+1)+4.*f(i1,i2,i3+2)-f(i1,i2,i3+3))*D22(2))

! one-sided approximations on the right:
#defineMacro FRb(i1,i2,i3) (( f(i1-2,i2,i3)-4.*f(i1-1,i2,i3)+3.*f(i1,i2,i3))*D12(0))  
#defineMacro FSb(i1,i2,i3) (( f(i1,i2-2,i3)-4.*f(i1,i2-1,i3)+3.*f(i1,i2,i3))*D12(1))  
#defineMacro FTb(i1,i2,i3) (( f(i1,i2,i3-2)-4.*f(i1,i2,i3-1)+3.*f(i1,i2,i3))*D12(2))  

#defineMacro FRRb(i1,i2,i3) ((2.*f(i1,i2,i3)-5.*f(i1-1,i2,i3)+4.*f(i1-2,i2,i3)-f(i1-3,i2,i3))*D22(0))
#defineMacro FSSb(i1,i2,i3) ((2.*f(i1,i2,i3)-5.*f(i1,i2-1,i3)+4.*f(i1,i2-2,i3)-f(i1,i2-3,i3))*D22(1))
#defineMacro FTTb(i1,i2,i3) ((2.*f(i1,i2,i3)-5.*f(i1,i2,i3-1)+4.*f(i1,i2,i3-2)-f(i1,i2,i3-3))*D22(2))

! 2nd-order centered: 
#defineMacro GVR() ((gv(+1,0,0)-gv(-1,0,0))*D12(0))
#defineMacro GVS() ((gv(0,+1,0)-gv(0,-1,0))*D12(1))
#defineMacro GVT() ((gv(0,0,+1)-gv(0,0,-1))*D12(2))

#defineMacro GVRR() ((gv(+1,0,0)-2.*gv(0,0,0)+gv(-1,0,0))*D22(0))
#defineMacro GVSS() ((gv(0,+1,0)-2.*gv(0,0,0)+gv(0,-1,0))*D22(1))
#defineMacro GVTT() ((gv(0,0,+1)-2.*gv(0,0,0)+gv(0,0,-1))*D22(2))

#defineMacro GVRS() ( ((gv(+1,+1,0)-gv(+1,-1,0)) - (gv(-1,+1,0)-gv(-1,-1,0)))*d12(0)*d12(1) )
#defineMacro GVRT() ( ((gv(+1,0,+1)-gv(+1,0,-1)) - (gv(-1,0,+1)-gv(-1,0,-1)))*d12(0)*d12(2) )
#defineMacro GVST() ( ((gv(0,+1,+1)-gv(0,+1,-1)) - (gv(0,-1,+1)-gv(0,-1,-1)))*d12(1)*d12(2) )


#defineMacro FVR() ((fv(+1,0,0)-fv(-1,0,0))*D12(0))
#defineMacro FVS() ((fv(0,+1,0)-fv(0,-1,0))*D12(1))
#defineMacro FVT() ((fv(0,0,+1)-fv(0,0,-1))*D12(2))

#defineMacro FVRR() ((fv(+1,0,0)-2.*fv(0,0,0)+fv(-1,0,0))*D22(0))
#defineMacro FVSS() ((fv(0,+1,0)-2.*fv(0,0,0)+fv(0,-1,0))*D22(1))
#defineMacro FVTT() ((fv(0,0,+1)-2.*fv(0,0,0)+fv(0,0,-1))*D22(2))

! Extrapolation of f(i1,i2,i3) in direction (is1,is2,is3)
#defineMacro extrap1(f,i1,i2,i3,is1,is2,is3) (f(i1+(is1),i2+(is2),i3+(is3)))

#defineMacro extrap2(f,i1,i2,i3,is1,is2,is3) (2.*f(i1+(is1),i2+(is2),i3+(is3))-f(i1+2*(is1),i2+2*(is2),i3+2*(is3)))

#defineMacro extrap3(f,i1,i2,i3,is1,is2,is3) (3.*f(i1+(is1),i2+(is2),i3+(is3))-3.*f(i1+2*(is1),i2+2*(is2),i3+2*(is3))+f(i1+3*(is1),i2+3*(is2),i3+3*(is3)))

#defineMacro extrap4(f,i1,i2,i3,is1,is2,is3) (4.*f(i1+(is1),i2+(is2),i3+(is3))-6.*f(i1+2*(is1),i2+2*(is2),i3+2*(is3))+4.*f(i1+3*(is1),i2+3*(is2),i3+3*(is3))-f(i1+4*(is1),i2+4*(is2),i3+4*(is3)))


! ================================================================================================
!  Extrapolate a point. Choose the order of extrapolation based on how many 
! valid points exist (mask>0)
!
! fe : put result here 
! (k1,k2,k3) : check mask at points (i1+m*is1,i1+m*is2,i3+m*is3) m=1,2,..
! (l1,l2,l3) : Extrapolate point (l1,l2,l3) using points (l1+m*is1,l1+m*is2,l3+m*is3)
! (is1,is2,is3) : direction (shift) of extrapolation 
! 
! ================================================================================================
#beginMacro extrapWithMask(fe, f, k1,k2,k3, l1,l2,l3, is1,is2,is3 )
  if( mask(k1+  (is1),k2+  (is2),k3+  (is3)).gt.0 .and. \
      mask(k1+2*(is1),k2+2*(is2),k3+2*(is3)).gt.0 .and.\
      mask(k1+3*(is1),k2+3*(is2),k3+3*(is3)).gt.0 .and.\
      mask(k1+4*(is1),k2+4*(is2),k3+4*(is3)).gt.0 )then
   fe=extrap4(f,l1,l2,l3,is1,is2,is3)
  else if( mask(k1+  (is1),k2+  (is2),k3+  (is3)).gt.0 .and. \
           mask(k1+2*(is1),k2+2*(is2),k3+2*(is3)).gt.0 .and.\
           mask(k1+3*(is1),k2+3*(is2),k3+3*(is3)).gt.0 )then
   fe=extrap3(f,l1,l2,l3,is1,is2,is3)
  else if( mask(k1+  (is1),k2+  (is2),k3+  (is3)).gt.0 .and. \
           mask(k1+2*(is1),k2+2*(is2),k3+2*(is3)).gt.0 )then
   fe=extrap2(f,l1,l2,l3,is1,is2,is3)
  else 
   fe=extrap1(f,l1,l2,l3,is1,is2,is3)
  end if
#endMacro

! ===================================================================================================
! Macro: Evaluate the boundary forcing g at the ghost point (k1,k2,k3) next to the target 
!     ghost point (l1,l2,l3) (where we are evaluating derivatives of g) and the boundary point (b1,b2,b3)
!
!                  |   |   |
!               ---B---L---X----  <- boundary 
!                  |   |   |
!               ---K---+---+----  <- ghost 
! 
!  This macro expects the following to be already set:
!  ax1, ax2 :  tangential directions: 
!            ax1= mod(axis+1,nd)
!            ax2= mod(axis+2,nd)
!  mdim(0:1,0:2) : index bounds on valid points where g is defined. 
! ===================================================================================================
#beginMacro getAdjacentBoundaryForcing(k1,k2,k3, l1,l2,l3, b1,b2,b3)

 ! Add these checks -- comment out later
 if( abs(k1-l1).gt.1 .or. abs(k2-l2).gt.1 .or. abs(k3-l3).gt.1 )then
   stop 3338
 end if
 ! iv(0:2) = boundary point next to (k1,k2,k3)
 iv(0)=b1
 iv(1)=b2
 iv(2)=b3 
 ! dv(0:2) : extrapolate in direction dv (if dv[a]=0 a=0,1,2 then there is no extrapolation)
 dv(0)=0
 dv(1)=0
 dv(2)=0
 if( iv(ax1).lt.mdim(0,ax1) )then
  dv(ax1)=1
 else if( iv(ax1).gt.mdim(1,ax1) )then
  dv(ax1)=-1
 end if
 if( iv(ax2).lt.mdim(0,ax2) )then
   dv(ax2)=1
 else if( iv(ax2).gt.mdim(1,ax2) )then
   dv(ax2)=-1
 end if
 if( mask(iv(0)+dv(0),iv(1)+dv(1),iv(2)+dv(2)).le.0 )then
   ! The neighbouring pt kv or the point we start the extrapolation from is not valid
   ! Find a direction to extrapolate in: The target point must be valid so start the 
   ! extrpolation in that direction.
   dv(0)=l1-k1
   dv(1)=l2-k2
   dv(2)=l3-k3
 end if
 if( dv(0).eq.0 .and. dv(1).eq.0. .and. dv(2).eq.0 )then
   ! We can use the value from the adjacent point:
   gv(k1-l1,k2-l2,k3-l3)=f(k1,k2,k3)
 else if( mask(iv(0)+  dv(0),iv(1)+  dv(1),iv(2)+  dv(2)).gt.0 .and. \
          mask(iv(0)+2*dv(0),iv(1)+2*dv(1),iv(2)+2*dv(2)).gt.0 .and. \
          mask(iv(0)+3*dv(0),iv(1)+3*dv(1),iv(2)+3*dv(2)).gt.0 .and. \
          mask(iv(0)+4*dv(0),iv(1)+4*dv(1),iv(2)+4*dv(2)).gt.0 )then
   ! we can extrapolate with 4 points:
   gv(k1-l1,k2-l2,k3-l3) = extrap4(f,k1,k2,k3, dv(0), dv(1), dv(2))
 else if( mask(iv(0)+  dv(0),iv(1)+  dv(1),iv(2)+  dv(2)).gt.0 .and. \
          mask(iv(0)+2*dv(0),iv(1)+2*dv(1),iv(2)+2*dv(2)).gt.0 .and. \
          mask(iv(0)+3*dv(0),iv(1)+3*dv(1),iv(2)+3*dv(2)).gt.0 )then
   ! we can extrapolate with 3 points:
   gv(k1-l1,k2-l2,k3-l3) = extrap3(f,k1,k2,k3, dv(0), dv(1), dv(2))
 else if( mask(iv(0)+  dv(0),iv(1)+  dv(1),iv(2)+  dv(2)).gt.0 .and. \
          mask(iv(0)+2*dv(0),iv(1)+2*dv(1),iv(2)+2*dv(2)).gt.0 )then
   ! we can extrapolate with 2 points: 
   gv(k1-l1,k2-l2,k3-l3) = extrap2(f,k1,k2,k3, dv(0), dv(1), dv(2))
 else
   ! as a backup just use the value from the target point
   gv(k1-l1,k2-l2,k3-l3)=f(l1,l2,l3)
 end if
#endMacro


! ========================================================================================================================
! This Ogmg macro defines the forcing for fourth-order Neumann boundary conditions
!    Lu = ff , Bu=g 
! Input: 
!  [mm1a,mm1b][mm2a,mm2b][mm3a,mm3b] : indexes for the boundary 
!  (i1,i2,i3) : point on the boundary 
!  (j1,j2,j3) : ghost point
!  (is1,is2,is3) : usual
! FORCING: forcing or no forcing  (if noForcing then return values are all zero)
! GRIDTYPE = rectangular or curvilinear
! DIR = R or S or T 
! DIM = 2 or 3 
! Return: DIM=2, DIR=R : ff, ffr, g, gss
!         DIM=2, DIR=S : ff, ffs, g, grr
!         DIM=3, DIR=R : ff, ffr, g, gss, gtt  + curvilinear: ffs, fft, gst 
!         DIM=3, DIR=S : ff, ffs, g, grr, gtt  + curvilinear: ffr, fft, grt
!         DIM=3, DIR=T : ff, fft, g, grr, gss  + curvilinear: ffr, ffs, grs
! ========================================================================================================================
#beginMacro defineNeumannEquationForcing(mm1a,mm1b,mm2a,mm2b,mm3a,mm3b,FORCING,GRIDTYPE,DIR,DIM)
 ! the rhs for the mixed BC is stored in the ghost point value of f
#If #GRIDTYPE eq "rectangular" 

! Cartesian grids use dx: 
#defineMacro D12(axis) h12(axis)
#defineMacro D22(axis) h22(axis)

 #If #FORCING eq "forcing"
   g = f(j1,j2,j3)
   ff=f(i1,i2,i3)

   #If #DIR eq "R"
     ! Note "g" is located on the ghost point "j1" of f

     ! 2nd-order one sided:
     ! ffr=(-f(i1+2*is1,i2,i3)+4.*f(i1+is1,i2,i3)-3.*ff)/(2.*dx(0))  
     ! 3rd-order one sided: 100510 -- added is1
     ffr=is1*(-11.*ff+18.*f(i1+is1,i2,i3)-9.*f(i1+2*is1,i2,i3)+2.*f(i1+3*is1,i2,i3))/(6.*dx(0)) 

     ! 100610: Check the mask for computing valid tangential derivatives:

     ! NOTE: the forcing f and g are only assumed to be given where mask>0
     ! In order to compute tangential derivatives of the forcing we may need to fill in
     ! neighbouring values of the forcing at interp and unused points
     gv( 0, 0, 0)=f(j1,i2,i3)
     i2m1 = i2-1
     if( i2m1.lt.mm2a .or. mask(i1,i2m1,i3).le.0 )then
       ! f(j1,i2m1,i3)= extrap3(f,j1,i2m1,i3, 0,1,0)
       ! gv( 0,-1, 0)=extrap3(f,j1,i2m1,i3, 0,1,0)
       ! extrapWithMask
       extrapWithMask( gv( 0,-1, 0), f, i1,i2m1,i3, j1,i2m1,i3, 0,1,0 )
     else
       gv( 0,-1, 0)=f(j1,i2m1,i3)
     end if
     i2p1 = i2+1
     if( i2p1.gt.mm2b .or. mask(i1,i2p1,i3).le.0 )then
       ! f(j1,i2p1,i3)= extrap3(f,j1,i2p1,i3, 0,-1,0)
       ! gv( 0,+1, 0)=extrap3(f,j1,i2p1,i3, 0,-1,0)
       extrapWithMask(gv( 0,+1, 0), f, i1,i2p1,i3, j1,i2p1,i3, 0,-1,0 )
     else
       gv( 0,+1, 0)=f(j1,i2p1,i3)
     end if
     ! gss=FSS(j1,i2,i3)
     gss = GVSS()

    #If #DIM eq "3"
     i3m1 = i3-1
     if( i3m1.lt.mm3a .or. mask(i1,i2,i3m1).le.0 )then
       ! f(j1,i2,i3m1)= extrap3(f,j1,i2,i3m1, 0,0,1)
       ! gv( 0, 0,-1) = extrap3(f,j1,i2,i3m1, 0,0,1)
       extrapWithMask(gv( 0, 0,-1), f, i1,i2,i3m1, j1,i2,i3m1, 0,0,1 )
     else
       gv( 0, 0,-1) = f(j1,i2,i3m1)
     end if
     i3p1 = i3+1
     if( i3p1.gt.mm3b .or. mask(i1,i2,i3p1).le.0 )then
      ! f(j1,i2,i3p1)= extrap3(f,j1,i2,i3p1, 0,0,-1)
      ! gv( 0, 0,+1) = extrap3(f,j1,i2,i3p1, 0,0,-1)
      extrapWithMask(gv( 0, 0,+1), f, i1,i2,i3p1, j1,i2,i3p1, 0,0,-1 )
     else
      gv( 0, 0,+1) = f(j1,i2,i3p1)
     end if
     ! gtt=FTT(j1,i2,i3)
     gtt = GVTT()

    #End

   #Elif #DIR eq "S"
     ! 2nd-order one sided:
     ! ffs=(-f(i1,i2+2*is2,i3)+4.*f(i1,i2+is2,i3)-3.*ff)/(2.*dx(1)) 
     ! 3rd-order one sided:
     ffs=is2*(-11.*ff+18.*f(i1,i2+is2,i3)-9.*f(i1,i2+2*is2,i3)+2.*f(i1,i2+3*is2,i3))/(6.*dx(1))  

     ! NOTE: the forcing f and g are only assumed to be given where mask>0
     ! In order to compute tangential derivatives of the forcing we may need to fill in
     ! neighbouring values of the forcing at interp and unused points
     gv( 0, 0, 0)=f(i1,j2,i3)
     i1m1 = i1-1
     if( i1m1.lt.mm1a .or. mask(i1m1,i2,i3).le.0 )then
      ! f(i1m1,j2,i3)= extrap3(f,i1m1,j2,i3, 1,0,0)
      ! gv(-1, 0, 0) = extrap3(f,i1m1,j2,i3, 1,0,0)
      extrapWithMask(gv(-1, 0, 0), f, i1m1,i2,i3, i1m1,j2,i3, 1,0,0 )
     else
       gv(-1, 0, 0) = f(i1m1,j2,i3)
     end if
     i1p1 = i1+1
     if( i1p1.gt.mm1b .or. mask(i1p1,i2,i3).le.0 )then
      ! f(i1p1,j2,i3)= extrap3(f,i1p1,j2,i3,-1,0,0)
      ! gv(+1, 0, 0) = extrap3(f,i1p1,j2,i3,-1,0,0)
      extrapWithMask(gv(+1, 0, 0), f, i1p1,i2,i3, i1p1,j2,i3, -1,0,0 ) 
     else
      gv(+1, 0, 0) = f(i1p1,j2,i3)
     end if
     ! grr=FRR(i1,j2,i3)
     grr = GVRR()

    #If #DIM eq "3"
     i3m1 = i3-1
     if( i3m1.lt.mm3a .or. mask(i1,i2,i3m1).le.0 )then
      ! f(i1,j2,i3m1)= extrap3(f,i1,j2,i3m1, 0,0,1)
      ! gv( 0, 0,-1) = extrap3(f,i1,j2,i3m1, 0,0,1)
      extrapWithMask(gv( 0, 0,-1), f, i1,i2,i3m1, i1,j2,i3m1, 0,0,1 )
     else
      gv( 0, 0,-1) = f(i1,j2,i3m1)
     end if
     i3p1 = i3+1
     if( i3p1.gt.mm3b .or. mask(i1,i2,i3p1).le.0 )then
      ! f(i1,j2,i3p1)= extrap3(f,i1,j2,i3p1, 0,0,-1)
      ! gv( 0, 0,+1) = extrap3(f,i1,j2,i3p1, 0,0,-1)
      extrapWithMask(gv( 0, 0,+1), f, i1,i2,i3p1, i1,j2,i3p1, 0,0,-1 )
     else
      gv( 0, 0,+1) = f(i1,j2,i3p1)
     end if
     ! gtt=FTT(i1,j2,i3)
     gtt = GVTT()

    #End

   #Elif #DIR eq "T"
     ! 3rd-order one sided:
     fft=is3*(-11.*ff+18.*f(i1,i2,i3+is3)-9.*f(i1,i2,i3+2*is3)+2.*f(i1,i2,i3+3*is3))/(6.*dx(2))  

     gv( 0, 0, 0)=f(i1,i2,j3)
     i1m1 = i1-1
     if( i1m1.lt.mm1a .or. mask(i1m1,i2,i3).le.0 )then
      ! f(i1m1,i2,j3)= extrap3(f,i1m1,i2,j3, 1,0,0)
      ! gv(-1, 0, 0) = extrap3(f,i1m1,i2,j3, 1,0,0)
      extrapWithMask(gv(-1, 0, 0), f, i1m1,i2,i3, i1m1,i2,j3, 1,0,0 )
     else
      gv(-1, 0, 0) = f(i1m1,i2,j3)
     end if
     i1p1 = i1+1
     if( i1p1.gt.mm1b .or. mask(i1p1,i2,i3).le.0 )then
      ! f(i1p1,i2,j3)= extrap3(f,i1p1,i2,j3,-1,0,0)
      ! gv(+1, 0, 0) = extrap3(f,i1p1,i2,j3,-1,0,0)
      extrapWithMask(gv(+1, 0, 0), f, i1p1,i2,i3, i1p1,i2,j3, -1,0,0 )
     else
      gv(+1, 0, 0) = f(i1p1,i2,j3)
     end if
     ! grr=FRR(i1,i2,j3)
     grr = GVRR()

     i2m1 = i2-1
     if( i2m1.lt.mm2a .or. mask(i1,i2m1,i3).le.0 )then
      ! f(i1,i2m1,j3)= extrap3(f,i1,i2m1,j3, 0,1,0)
      ! gv( 0,-1, 0) = extrap3(f,i1,i2m1,j3, 0,1,0)
      extrapWithMask(gv( 0,-1, 0), f, i1,i2m1,i3, i1,i2m1,j3, 0,1,0 )
     else
      gv( 0,-1, 0) = f(i1,i2m1,j3)
     end if
     i2p1 = i2+1
     if( i2p1.gt.mm2b .or. mask(i1,i2p1,i3).le.0 )then
      ! f(i1,i2p1,j3)= extrap3(f,i1,i2p1,j3, 0,-1,0)
      ! gv( 0,+1, 0) = extrap3(f,i1,i2p1,j3, 0,-1,0)
      extrapWithMask(gv( 0,+1, 0), f, i1,i2p1,i3, i1,i2p1,j3, 0,-1,0 )
     else
      gv( 0,+1, 0) = f(i1,i2p1,j3)
     end if
     ! gss=FSS(i1,i2,j3)
     gss = GVSS()

     ! if( i1.eq.mm1a )then
     !   grr =FRRa(i1,i2,j3)
     ! else if( i1.eq.mm1b )then
     !   grr =FRRb(i1,i2,j3)
     ! else 
     !   grr=FRR(i1,i2,j3)
     ! end if
     ! if( i2.eq.mm2a )then
     !   gss =FSSa(i1,i2,j3)
     ! else if( i2.eq.mm2b )then
     !   gss =FSSb(i1,i2,j3)
     ! else 
     !   gss=FSS(i1,i2,j3)
     ! end if

   #Else
     stop 7
   #End

 #Else
   g=0.
   ff=0.
   #If #DIR eq "R"
    ffr=0.
     gs=0.
     gss=0.
   #Else
     ffs=0.
     gr=0.
     grr=0.
   #End

 #End

#Elif #GRIDTYPE eq "curvilinear" 

! Curvilinear grids use dr:
#defineMacro D12(axis) d12(axis)
#defineMacro D22(axis) d22(axis)

 #If #FORCING eq "forcing"
   g = f(j1,j2,j3)
   ff= f(i1,i2,i3)

   #If #DIM eq "3"
    ax1 = mod(axis+1,nd)
    ax2 = mod(axis+2,nd)
    mdim(0,0)=mm1a
    mdim(1,0)=mm1b
    mdim(0,1)=mm2a
    mdim(1,1)=mm2b
    mdim(0,2)=mm3a
    mdim(1,2)=mm3b
   #End



   #If #DIR eq "R"
     ! 2nd-order one sided:
     ! ffr=is1*(-f(i1+2*is1,i2,i3)+4.*f(i1+is1,i2,i3)-3.*ff)*d12(0)  
     ! 3rd-order one sided:
     ffr=is1*(-11.*ff+18.*f(i1+is1,i2,i3)-9.*f(i1+2*is1,i2,i3)+2.*f(i1+3*is1,i2,i3))/(6.*dr(0))  

     ! NOTE: the forcing f and g are only assumed to be given where mask>0
     ! In order to compute tangential derivatives of the forcing we may need to fill in
     ! neighbouring values of the forcing at interp and unused points
     fv( 0, 0, 0) = f(i1,i2,i3)
     gv( 0, 0, 0) = f(j1,i2,i3)
     i2m1 = i2-1
     if( i2m1.lt.mm2a .or. mask(i1,i2m1,i3).le.0 )then
      ! NOTE: We DO need to extrap f and g 
      ! f(i1,i2m1,i3)= extrap3(f,i1,i2m1,i3, 0,1,0)
      ! f(j1,i2m1,i3)= extrap3(f,j1,i2m1,i3, 0,1,0)
      ! fv( 0,-1, 0) = extrap3(f,i1,i2m1,i3, 0,1,0)
      ! gv( 0,-1, 0) = extrap3(f,j1,i2m1,i3, 0,1,0)

      extrapWithMask( fv( 0,-1, 0), f, i1,i2m1,i3, i1,i2m1,i3, 0,1,0 )      
      extrapWithMask( gv( 0,-1, 0), f, i1,i2m1,i3, j1,i2m1,i3, 0,1,0 )      

     else
      fv( 0,-1, 0) = f(i1,i2m1,i3)
      gv( 0,-1, 0) = f(j1,i2m1,i3)
     end if
     i2p1 = i2+1
     if( i2p1.gt.mm2b .or. mask(i1,i2p1,i3).le.0 )then
      !  f(i1,i2p1,i3)= extrap3(f,i1,i2p1,i3, 0,-1,0)
      !  f(j1,i2p1,i3)= extrap3(f,j1,i2p1,i3, 0,-1,0)
      ! fv( 0,+1, 0) = extrap3(f,i1,i2p1,i3, 0,-1,0)
      ! gv( 0,+1, 0) = extrap3(f,j1,i2p1,i3, 0,-1,0)
      extrapWithMask(fv( 0,+1, 0), f, i1,i2p1,i3, i1,i2p1,i3, 0,-1,0 )
      extrapWithMask(gv( 0,+1, 0), f, i1,i2p1,i3, j1,i2p1,i3, 0,-1,0 )

     else
      fv( 0,+1, 0) = f(i1,i2p1,i3)
      gv( 0,+1, 0) = f(j1,i2p1,i3)
     end if
     ! ffs= FS(i1,i2,i3)
     ! gs = FS(j1,i2,i3)
     ! gss=FSS(j1,i2,i3)
     ffs = FVS()
     gs  = GVS()
     gss = GVSS()

    #If #DIM eq "3"

     i3m1 = i3-1
     if( i3m1.lt.mm3a .or. mask(i1,i2,i3m1).le.0 )then
      ! f(i1,i2,i3m1)= extrap3(f,i1,i2,i3m1, 0,0,1)
      ! f(j1,i2,i3m1)= extrap3(f,j1,i2,i3m1, 0,0,1)
      ! fv( 0, 0,-1) = extrap3(f,i1,i2,i3m1, 0,0,1)
      ! gv( 0, 0,-1) = extrap3(f,j1,i2,i3m1, 0,0,1)
      extrapWithMask(fv( 0, 0,-1), f, i1,i2,i3m1, i1,i2,i3m1, 0,0,1 )
      extrapWithMask(gv( 0, 0,-1), f, i1,i2,i3m1, j1,i2,i3m1, 0,0,1 )

     else
      fv( 0, 0,-1) = f(i1,i2,i3m1)
      gv( 0, 0,-1) = f(j1,i2,i3m1)
     end if
     i3p1 = i3+1
     if( i3p1.gt.mm3b .or. mask(i1,i2,i3p1).le.0 )then
      ! f(i1,i2,i3p1)= extrap3(f,i1,i2,i3p1, 0,0,-1)
      ! f(j1,i2,i3p1)= extrap3(f,j1,i2,i3p1, 0,0,-1)
      ! fv( 0, 0,+1) = extrap3(f,i1,i2,i3p1, 0,0,-1)
      ! gv( 0, 0,+1) = extrap3(f,j1,i2,i3p1, 0,0,-1)
      extrapWithMask(fv( 0, 0,+1), f, i1,i2,i3p1, i1,i2,i3p1, 0,0,-1 )
      extrapWithMask(gv( 0, 0,+1), f, i1,i2,i3p1, j1,i2,i3p1, 0,0,-1 )
     else
      fv( 0, 0,+1) = f(i1,i2,i3p1)
      gv( 0, 0,+1) = f(j1,i2,i3p1)
     end if
     ! fft= FT(i1,i2,i3)
     ! gt = FT(j1,i2,i3)
     ! gtt=FTT(j1,i2,i3)
     fft = FVT()
     gt  = GVT()
     gtt = GVTT()
     
     ! compute the cross derivative: gst 
     ! Near physical or interpolation boundaries we may need to use a one sided approximation

     ! Evaluate g at neighbouring points so we can evaluate the cross derivative 
     getAdjacentBoundaryForcing(j1,i2m1,i3m1, j1,i2,i3, i1,i2m1,i3m1)
     getAdjacentBoundaryForcing(j1,i2p1,i3m1, j1,i2,i3, i1,i2p1,i3m1)
     getAdjacentBoundaryForcing(j1,i2m1,i3p1, j1,i2,i3, i1,i2m1,i3p1)
     getAdjacentBoundaryForcing(j1,i2p1,i3p1, j1,i2,i3, i1,i2p1,i3p1)
     gst = GVST()

    #End

   #Elif #DIR eq "S"
     ! 2nd-order one sided:
     ! is2*ffs=(-f(i1,i2+2*is2,i3)+4.*f(i1,i2+is2,i3)-3.*ff)*d12(1) 
     ! 3rd-order one sided:
     ffs=is2*(-11.*ff+18.*f(i1,i2+is2,i3)-9.*f(i1,i2+2*is2,i3)+2.*f(i1,i2+3*is2,i3))/(6.*dr(1)) 

     fv( 0, 0, 0) = f(i1,i2,i3)
     gv( 0, 0, 0) = f(i1,j2,i3)

     i1m1 = i1-1
     if( i1m1.lt.mm1a .or. mask(i1m1,i2,i3).le.0 )then
      ! f(i1m1,i2,i3)= extrap3(f,i1m1,i2,i3, 1,0,0)
      ! f(i1m1,j2,i3)= extrap3(f,i1m1,j2,i3, 1,0,0)
      ! fv(-1, 0, 0) = extrap3(f,i1m1,i2,i3, 1,0,0)
      ! gv(-1, 0, 0) = extrap3(f,i1m1,j2,i3, 1,0,0)
      extrapWithMask(fv(-1, 0, 0), f, i1m1,i2,i3, i1m1,i2,i3, 1,0,0 )
      extrapWithMask(gv(-1, 0, 0), f, i1m1,i2,i3, i1m1,j2,i3, 1,0,0 )
     else
      fv(-1, 0, 0) = f(i1m1,i2,i3)
      gv(-1, 0, 0) = f(i1m1,j2,i3)
     end if
     i1p1 = i1+1
     if( i1p1.gt.mm1b .or. mask(i1p1,i2,i3).le.0 )then
      ! f(i1p1,i2,i3)= extrap3(f,i1p1,i2,i3,-1,0,0)
      ! f(i1p1,j2,i3)= extrap3(f,i1p1,j2,i3,-1,0,0)
      ! fv(+1, 0, 0) = extrap3(f,i1p1,i2,i3,-1,0,0)
      ! gv(+1, 0, 0) = extrap3(f,i1p1,j2,i3,-1,0,0)
      extrapWithMask(fv(+1, 0, 0), f, i1p1,i2,i3, i1p1,i2,i3, -1,0,0 ) 
      extrapWithMask(gv(+1, 0, 0), f, i1p1,i2,i3, i1p1,j2,i3, -1,0,0 ) 
     else
      fv(+1, 0, 0) = f(i1p1,i2,i3)
      gv(+1, 0, 0) = f(i1p1,j2,i3)
     end if
     ! ffr= FR(i1,i2,i3)
     ! gr = FR(i1,j2,i3)
     ! grr=FRR(i1,j2,i3)
     ffr = FVR()
     gr  = GVR()
     grr = GVRR()

    #If #DIM eq "3"

     i3m1 = i3-1
     if( i3m1.lt.mm3a .or. mask(i1,i2,i3m1).le.0 )then
      ! f(i1,i2,i3m1)= extrap3(f,i1,i2,i3m1, 0,0, 1)
      ! f(i1,j2,i3m1)= extrap3(f,i1,j2,i3m1, 0,0, 1)
      ! fv( 0, 0,-1) = extrap3(f,i1,i2,i3m1, 0,0, 1)
      ! gv( 0, 0,-1) = extrap3(f,i1,j2,i3m1, 0,0, 1)
      extrapWithMask(fv( 0, 0,-1), f, i1,i2,i3m1, i1,i2,i3m1, 0,0,1 )
      extrapWithMask(gv( 0, 0,-1), f, i1,i2,i3m1, i1,j2,i3m1, 0,0,1 )
     else
      fv( 0, 0,-1) = f(i1,i2,i3m1) 
      gv( 0, 0,-1) = f(i1,j2,i3m1)
     end if
     i3p1 = i3+1
     if( i3p1.gt.mm3b .or. mask(i1,i2,i3p1).le.0 )then
      ! f(i1,i2,i3p1)= extrap3(f,i1,i2,i3p1, 0,0,-1)
      ! f(i1,j2,i3p1)= extrap3(f,i1,j2,i3p1, 0,0,-1)
      ! fv( 0, 0,+1) = extrap3(f,i1,i2,i3p1, 0,0,-1) 
      ! gv( 0, 0,+1) = extrap3(f,i1,j2,i3p1, 0,0,-1) 
      extrapWithMask(fv( 0, 0,+1), f, i1,i2,i3p1, i1,i2,i3p1, 0,0,-1 )
      extrapWithMask(gv( 0, 0,+1), f, i1,i2,i3p1, i1,j2,i3p1, 0,0,-1 )
     else
      fv( 0, 0,+1) = f(i1,i2,i3p1)
      gv( 0, 0,+1) = f(i1,j2,i3p1)
     end if
     ! fft= FT(i1,i2,i3)
     ! gt = FT(i1,j2,i3)
     ! gtt=FTT(i1,j2,i3)
     fft = FVT()
     gt  = GVT()
     gtt = GVTT()

     ! Evaluate g at neighbouring points so we can evaluate the cross derivative 
     getAdjacentBoundaryForcing(i1m1,j2,i3m1, i1,j2,i3, i1m1,i2,i3m1)
     getAdjacentBoundaryForcing(i1p1,j2,i3m1, i1,j2,i3, i1p1,i2,i3m1)
     getAdjacentBoundaryForcing(i1m1,j2,i3p1, i1,j2,i3, i1m1,i2,i3p1)
     getAdjacentBoundaryForcing(i1p1,j2,i3p1, i1,j2,i3, i1p1,i2,i3p1)

     grt = GVRT()

    #End


   #Elif #DIR eq "T"

     ! 3rd-order one sided:
     fft=is3*(-11.*ff+18.*f(i1,i2,i3+is3)-9.*f(i1,i2,i3+2*is3)+2.*f(i1,i2,i3+3*is3))/(6.*dr(2))

     fv( 0, 0, 0) = f(i1,i2,i3)
     gv( 0, 0, 0) = f(i1,i2,j3)

     i1m1 = i1-1
     if( i1m1.lt.mm1a .or. mask(i1m1,i2,i3).le.0 )then
      ! f(i1m1,i2,i3)= extrap3(f,i1m1,i2,i3, 1,0,0)
      ! f(i1m1,i2,j3)= extrap3(f,i1m1,i2,j3, 1,0,0)
      ! fv(-1, 0, 0) = extrap3(f,i1m1,i2,i3, 1,0,0)
      ! gv(-1, 0, 0) = extrap3(f,i1m1,i2,j3, 1,0,0)
      extrapWithMask(fv(-1, 0, 0), f, i1m1,i2,i3, i1m1,i2,i3, 1,0,0 )
      extrapWithMask(gv(-1, 0, 0), f, i1m1,i2,i3, i1m1,i2,j3, 1,0,0 )
     else
      fv(-1, 0, 0) = f(i1m1,i2,i3)
      gv(-1, 0, 0) = f(i1m1,i2,j3)
     endif
     i1p1 = i1+1
     if( i1p1.gt.mm1b .or. mask(i1p1,i2,i3).le.0 )then
      ! f(i1p1,i2,i3)= extrap3(f,i1p1,i2,i3,-1,0,0)
      ! f(i1p1,i2,j3)= extrap3(f,i1p1,i2,j3,-1,0,0)
      ! fv(+1, 0, 0) = extrap3(f,i1p1,i2,i3,-1,0,0)
      ! gv(+1, 0, 0) = extrap3(f,i1p1,i2,j3,-1,0,0)
      extrapWithMask(fv(+1, 0, 0), f, i1p1,i2,i3, i1p1,i2,i3, -1,0,0 )
      extrapWithMask(gv(+1, 0, 0), f, i1p1,i2,i3, i1p1,i2,j3, -1,0,0 )
     else
      fv(+1, 0, 0) = f(i1p1,i2,i3)
      gv(+1, 0, 0) = f(i1p1,i2,j3)
     endif
     ! ffr= FR(i1,i2,i3)
     ! gr = FR(i1,i2,j3)
     ! grr=FRR(i1,i2,j3)
     ffr = FVR()
     gr  = GVR()
     grr = GVRR()

     i2m1 = i2-1
     if( i2m1.lt.mm2a .or. mask(i1,i2m1,i3).le.0 )then
      ! f(i1,i2m1,i3)= extrap3(f,i1,i2m1,i3, 0,1,0)
      ! f(i1,i2m1,j3)= extrap3(f,i1,i2m1,j3, 0,1,0)
      ! fv( 0,-1, 0) = extrap3(f,i1,i2m1,i3, 0,1,0)
      ! gv( 0,-1, 0) = extrap3(f,i1,i2m1,j3, 0,1,0)
      extrapWithMask(fv( 0,-1, 0), f, i1,i2m1,i3, i1,i2m1,i3, 0,1,0 )
      extrapWithMask(gv( 0,-1, 0), f, i1,i2m1,i3, i1,i2m1,j3, 0,1,0 )
     else
      fv( 0,-1, 0) = f(i1,i2m1,i3)
      gv( 0,-1, 0) = f(i1,i2m1,j3)
     endif
     i2p1 = i2+1
     if( i2p1.gt.mm2b .or. mask(i1,i2p1,i3).le.0 )then
      ! f(i1,i2p1,i3)= extrap3(f,i1,i2p1,i3, 0,-1,0)
      ! f(i1,i2p1,j3)= extrap3(f,i1,i2p1,j3, 0,-1,0)
      ! fv( 0,+1, 0) = extrap3(f,i1,i2p1,i3, 0,-1,0)
      ! gv( 0,+1, 0) = extrap3(f,i1,i2p1,j3, 0,-1,0)
      extrapWithMask(fv( 0,+1, 0), f, i1,i2p1,i3, i1,i2p1,i3, 0,-1,0 )
      extrapWithMask(gv( 0,+1, 0), f, i1,i2p1,i3, i1,i2p1,j3, 0,-1,0 )
     else
      fv( 0,+1, 0) = f(i1,i2p1,i3)
      gv( 0,+1, 0) = f(i1,i2p1,j3)
     endif
     ! ffs= FS(i1,i2,i3)
     ! gs = FS(i1,i2,j3)
     ! gss=FSS(i1,i2,j3)
     ffs = FVS()
     gs  = GVS()
     gss = GVSS()

     ! Evaluate g at neighbouring points so we can evaluate the cross derivative 
     getAdjacentBoundaryForcing(i1m1,i2m1,j3, i1,i2,j3, i1m1,i2m1,i3 )
     getAdjacentBoundaryForcing(i1p1,i2m1,j3, i1,i2,j3, i1p1,i2m1,i3 )
     getAdjacentBoundaryForcing(i1m1,i2p1,j3, i1,i2,j3, i1m1,i2p1,i3 )
     getAdjacentBoundaryForcing(i1p1,i2p1,j3, i1,i2,j3, i1p1,i2p1,i3 )
     grs = GVRS()

   #Else
     stop 48480
   #End

 #Else
   ff=0.
   ffr=0.
   ffs=0.
   fft=0.
   g=0.
   #If #DIR eq "R"
     gs=0.
     gss=0.
     gt=0.
     gtt=0.
     gst=0.
   #Elif #DIR eq "S"
     gr=0.
     grr=0.
     gt=0.
     gtt=0.
     grt=0.
   #Elif #DIR eq "T"
     gr=0.
     grr=0.
     gs=0.
     gss=0.
     grs=0.
   #Else
     stop 48481   
   #End

 #End

#Else
 write(*,*) "Ogmg:NeuEqn:ERROR unknown gridType: GRIDTYPE"
 stop 1863
#End

#endMacro
