!         -*- mode: F90 -*-
! Macros that are common to different orders of accuracy

#beginMacro OGF3D(i1,i2,i3,t,u0,v0,w0)
 call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,u0,v0,w0)
#endMacro

#beginMacro OGF2D(i1,i2,i3,t,u0,v0,w0)
 call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,u0,v0,w0)
#endMacro

! This version can optionally eval time-derivative:
#beginMacro OGF3DFO(i1,i2,i3,t,u0,v0,w0)
  call ogf3dfo(ep,fieldOption,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,u0,v0,w0)
#endMacro

! This version can optionally eval time-derivative:
#beginMacro OGF2DFO(i1,i2,i3,t,u0,v0,w0)
 call ogf2dfo(ep,fieldOption,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,u0,v0,w0)
#endMacro

#beginMacro OGDERIV3D(ntd,nxd,nyd,nzd,i1,i2,i3,t,ux,vx,wx)
  call ogDeriv3(ep, ntd,nxd,nyd,nzd, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t, ex,ux, ey,vx, ez,wx)
#endMacro

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

! Tangent vectors (un-normalized)
#defineMacro TAU11(i1,i2,i3) rsxy(i1,i2,i3,axisp1,0)
#defineMacro TAU12(i1,i2,i3) rsxy(i1,i2,i3,axisp1,1)
#defineMacro TAU13(i1,i2,i3) rsxy(i1,i2,i3,axisp1,2)
			     
#defineMacro TAU21(i1,i2,i3) rsxy(i1,i2,i3,axisp2,0)
#defineMacro TAU22(i1,i2,i3) rsxy(i1,i2,i3,axisp2,1)
#defineMacro TAU23(i1,i2,i3) rsxy(i1,i2,i3,axisp2,2)


#defineMacro RXDET2D(i1,i2,i3) (rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3))

#defineMacro A11(i1,i2,i3) (rsxy(i1,i2,i3,axis  ,0)/RXDET2D(i1,i2,i3))
#defineMacro A12(i1,i2,i3) (rsxy(i1,i2,i3,axis  ,1)/RXDET2D(i1,i2,i3))
                                        
#defineMacro A21(i1,i2,i3) (rsxy(i1,i2,i3,axisp1,0)/RXDET2D(i1,i2,i3))
#defineMacro A22(i1,i2,i3) (rsxy(i1,i2,i3,axisp1,1)/RXDET2D(i1,i2,i3))

#defineMacro C11(i1,i2,i3) (rsxy(i1,i2,i3,axis  ,0)**2+rsxy(i1,i2,i3,axis  ,1)**2)
#defineMacro C22(i1,i2,i3) (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,axisp1,1)**2)

#defineMacro C1(i1,i2,i3) (rsxyx42(i1,i2,i3,axis  ,0)+rsxyy42(i1,i2,i3,axis  ,1))
#defineMacro C2(i1,i2,i3) (rsxyx42(i1,i2,i3,axisp1,0)+rsxyy42(i1,i2,i3,axisp1,1))

#defineMacro C1Order4(i1,i2,i3) (rsxyx42(i1,i2,i3,axis  ,0)+rsxyy42(i1,i2,i3,axis  ,1))
#defineMacro C2Order4(i1,i2,i3) (rsxyx42(i1,i2,i3,axisp1,0)+rsxyy42(i1,i2,i3,axisp1,1))

#defineMacro C1Order2(i1,i2,i3) (rsxyx22(i1,i2,i3,axis  ,0)+rsxyy22(i1,i2,i3,axis  ,1))
#defineMacro C2Order2(i1,i2,i3) (rsxyx22(i1,i2,i3,axisp1,0)+rsxyy22(i1,i2,i3,axisp1,1))

! ======== WARNING: These next derivatives are really R and S derivatives ===============
#defineMacro C1r2(i1,i2,i3) (rsxyxr22(i1,i2,i3,axis  ,0)+rsxyyr22(i1,i2,i3,axis  ,1))
#defineMacro C2r2(i1,i2,i3) (rsxyxr22(i1,i2,i3,axisp1,0)+rsxyyr22(i1,i2,i3,axisp1,1))

#defineMacro C1s2(i1,i2,i3) (rsxyxs22(i1,i2,i3,axis  ,0)+rsxyys22(i1,i2,i3,axis  ,1))
#defineMacro C2s2(i1,i2,i3) (rsxyxs22(i1,i2,i3,axisp1,0)+rsxyys22(i1,i2,i3,axisp1,1))

#defineMacro C1r4(i1,i2,i3) (rsxyxr42(i1,i2,i3,axis  ,0)+rsxyyr42(i1,i2,i3,axis  ,1))
#defineMacro C2r4(i1,i2,i3) (rsxyxr42(i1,i2,i3,axisp1,0)+rsxyyr42(i1,i2,i3,axisp1,1))

#defineMacro C1s4(i1,i2,i3) (rsxyxs42(i1,i2,i3,axis  ,0)+rsxyys42(i1,i2,i3,axis  ,1))
#defineMacro C2s4(i1,i2,i3) (rsxyxs42(i1,i2,i3,axisp1,0)+rsxyys42(i1,i2,i3,axisp1,1))



! ***************************************************************************************************
! *************************  here are versions for 3d **********************************************
! ***************************************************************************************************
#defineMacro RXDET3D(i1,i2,i3) (rx(i1,i2,i3)*(sy(i1,i2,i3)*tz(i1,i2,i3)-sz(i1,i2,i3)*ty(i1,i2,i3))\
                               +ry(i1,i2,i3)*(sz(i1,i2,i3)*tx(i1,i2,i3)-sx(i1,i2,i3)*tz(i1,i2,i3))\
                               +rz(i1,i2,i3)*(sx(i1,i2,i3)*ty(i1,i2,i3)-sy(i1,i2,i3)*tx(i1,i2,i3)) )

#defineMacro A11D3(i1,i2,i3) (rsxy(i1,i2,i3,axis  ,0)/RXDET3D(i1,i2,i3))
#defineMacro A12D3(i1,i2,i3) (rsxy(i1,i2,i3,axis  ,1)/RXDET3D(i1,i2,i3))
#defineMacro A13D3(i1,i2,i3) (rsxy(i1,i2,i3,axis  ,2)/RXDET3D(i1,i2,i3))
                                        
#defineMacro A21D3(i1,i2,i3) (rsxy(i1,i2,i3,axisp1,0)/RXDET3D(i1,i2,i3))
#defineMacro A22D3(i1,i2,i3) (rsxy(i1,i2,i3,axisp1,1)/RXDET3D(i1,i2,i3))
#defineMacro A23D3(i1,i2,i3) (rsxy(i1,i2,i3,axisp1,2)/RXDET3D(i1,i2,i3))

#defineMacro A31D3(i1,i2,i3) (rsxy(i1,i2,i3,axisp2,0)/RXDET3D(i1,i2,i3))
#defineMacro A32D3(i1,i2,i3) (rsxy(i1,i2,i3,axisp2,1)/RXDET3D(i1,i2,i3))
#defineMacro A33D3(i1,i2,i3) (rsxy(i1,i2,i3,axisp2,2)/RXDET3D(i1,i2,i3))

! Here are versions that use a precomputed jacobian
#defineMacro A11D3J(i1,i2,i3) (rsxy(i1,i2,i3,axis  ,0)*jac3di(i1-i10,i2-i20,i3-i30))
#defineMacro A12D3J(i1,i2,i3) (rsxy(i1,i2,i3,axis  ,1)*jac3di(i1-i10,i2-i20,i3-i30))
#defineMacro A13D3J(i1,i2,i3) (rsxy(i1,i2,i3,axis  ,2)*jac3di(i1-i10,i2-i20,i3-i30))
                                        	      
#defineMacro A21D3J(i1,i2,i3) (rsxy(i1,i2,i3,axisp1,0)*jac3di(i1-i10,i2-i20,i3-i30))
#defineMacro A22D3J(i1,i2,i3) (rsxy(i1,i2,i3,axisp1,1)*jac3di(i1-i10,i2-i20,i3-i30))
#defineMacro A23D3J(i1,i2,i3) (rsxy(i1,i2,i3,axisp1,2)*jac3di(i1-i10,i2-i20,i3-i30))
						      
#defineMacro A31D3J(i1,i2,i3) (rsxy(i1,i2,i3,axisp2,0)*jac3di(i1-i10,i2-i20,i3-i30))
#defineMacro A32D3J(i1,i2,i3) (rsxy(i1,i2,i3,axisp2,1)*jac3di(i1-i10,i2-i20,i3-i30))
#defineMacro A33D3J(i1,i2,i3) (rsxy(i1,i2,i3,axisp2,2)*jac3di(i1-i10,i2-i20,i3-i30))


#defineMacro C11D3(i1,i2,i3) (rsxy(i1,i2,i3,axis  ,0)**2+rsxy(i1,i2,i3,axis  ,1)**2+rsxy(i1,i2,i3,axis  ,2)**2)
#defineMacro C22D3(i1,i2,i3) (rsxy(i1,i2,i3,axisp1,0)**2+rsxy(i1,i2,i3,axisp1,1)**2+rsxy(i1,i2,i3,axisp1,2)**2)
#defineMacro C33D3(i1,i2,i3) (rsxy(i1,i2,i3,axisp2,0)**2+rsxy(i1,i2,i3,axisp2,1)**2+rsxy(i1,i2,i3,axisp2,2)**2)

#defineMacro C11D3r4(i1,i2,i3) (2.*(rsxy(i1,i2,i3,axis  ,0)*rsxyr4(i1,i2,i3,axis  ,0)\
                                   +rsxy(i1,i2,i3,axis  ,1)*rsxyr4(i1,i2,i3,axis  ,1)\
                                   +rsxy(i1,i2,i3,axis  ,2)*rsxyr4(i1,i2,i3,axis  ,2)))
#defineMacro C22D3r4(i1,i2,i3) (2.*(rsxy(i1,i2,i3,axisp1,0)*rsxyr4(i1,i2,i3,axisp1,0)\
                                   +rsxy(i1,i2,i3,axisp1,1)*rsxyr4(i1,i2,i3,axisp1,1)\
                                   +rsxy(i1,i2,i3,axisp1,2)*rsxyr4(i1,i2,i3,axisp1,2)))
#defineMacro C33D3r4(i1,i2,i3) (2.*(rsxy(i1,i2,i3,axisp2,0)*rsxyr4(i1,i2,i3,axisp2,0)\
                                   +rsxy(i1,i2,i3,axisp2,1)*rsxyr4(i1,i2,i3,axisp2,1)\
                                   +rsxy(i1,i2,i3,axisp2,2)*rsxyr4(i1,i2,i3,axisp2,2)))

#defineMacro C11D3s4(i1,i2,i3) (2.*(rsxy(i1,i2,i3,axis  ,0)*rsxys4(i1,i2,i3,axis  ,0)\
                                   +rsxy(i1,i2,i3,axis  ,1)*rsxys4(i1,i2,i3,axis  ,1)\
                                   +rsxy(i1,i2,i3,axis  ,2)*rsxys4(i1,i2,i3,axis  ,2)))
#defineMacro C22D3s4(i1,i2,i3) (2.*(rsxy(i1,i2,i3,axisp1,0)*rsxys4(i1,i2,i3,axisp1,0)\
                                   +rsxy(i1,i2,i3,axisp1,1)*rsxys4(i1,i2,i3,axisp1,1)\
                                   +rsxy(i1,i2,i3,axisp1,2)*rsxys4(i1,i2,i3,axisp1,2)))
#defineMacro C33D3s4(i1,i2,i3) (2.*(rsxy(i1,i2,i3,axisp2,0)*rsxys4(i1,i2,i3,axisp2,0)\
                                   +rsxy(i1,i2,i3,axisp2,1)*rsxys4(i1,i2,i3,axisp2,1)\
                                   +rsxy(i1,i2,i3,axisp2,2)*rsxys4(i1,i2,i3,axisp2,2)))

#defineMacro C11D3t4(i1,i2,i3) (2.*(rsxy(i1,i2,i3,axis  ,0)*rsxyt4(i1,i2,i3,axis  ,0)\
                                   +rsxy(i1,i2,i3,axis  ,1)*rsxyt4(i1,i2,i3,axis  ,1)\
                                   +rsxy(i1,i2,i3,axis  ,2)*rsxyt4(i1,i2,i3,axis  ,2)))
#defineMacro C22D3t4(i1,i2,i3) (2.*(rsxy(i1,i2,i3,axisp1,0)*rsxyt4(i1,i2,i3,axisp1,0)\
                                   +rsxy(i1,i2,i3,axisp1,1)*rsxyt4(i1,i2,i3,axisp1,1)\
                                   +rsxy(i1,i2,i3,axisp1,2)*rsxyt4(i1,i2,i3,axisp1,2)))
#defineMacro C33D3t4(i1,i2,i3) (2.*(rsxy(i1,i2,i3,axisp2,0)*rsxyt4(i1,i2,i3,axisp2,0)\
                                   +rsxy(i1,i2,i3,axisp2,1)*rsxyt4(i1,i2,i3,axisp2,1)\
                                   +rsxy(i1,i2,i3,axisp2,2)*rsxyt4(i1,i2,i3,axisp2,2)))

#defineMacro C1D3(i1,i2,i3) (rsxyx43(i1,i2,i3,axis  ,0)+rsxyy43(i1,i2,i3,axis  ,1)+rsxyz43(i1,i2,i3,axis  ,2))
#defineMacro C2D3(i1,i2,i3) (rsxyx43(i1,i2,i3,axisp1,0)+rsxyy43(i1,i2,i3,axisp1,1)+rsxyz43(i1,i2,i3,axisp1,2))
#defineMacro C3D3(i1,i2,i3) (rsxyx43(i1,i2,i3,axisp2,0)+rsxyy43(i1,i2,i3,axisp2,1)+rsxyz43(i1,i2,i3,axisp2,2))

#defineMacro C1D3Order4(i1,i2,i3) (rsxyx43(i1,i2,i3,axis  ,0)+rsxyy43(i1,i2,i3,axis  ,1)+rsxyz43(i1,i2,i3,axis  ,2))
#defineMacro C2D3Order4(i1,i2,i3) (rsxyx43(i1,i2,i3,axisp1,0)+rsxyy43(i1,i2,i3,axisp1,1)+rsxyz43(i1,i2,i3,axisp1,2))
#defineMacro C3D3Order4(i1,i2,i3) (rsxyx43(i1,i2,i3,axisp2,0)+rsxyy43(i1,i2,i3,axisp2,1)+rsxyz43(i1,i2,i3,axisp2,2))

#defineMacro C1D3Order2(i1,i2,i3) (rsxyx23(i1,i2,i3,axis  ,0)+rsxyy23(i1,i2,i3,axis  ,1)+rsxyz23(i1,i2,i3,axis  ,2))
#defineMacro C2D3Order2(i1,i2,i3) (rsxyx23(i1,i2,i3,axisp1,0)+rsxyy23(i1,i2,i3,axisp1,1)+rsxyz23(i1,i2,i3,axisp1,2))
#defineMacro C3D3Order2(i1,i2,i3) (rsxyx23(i1,i2,i3,axisp2,0)+rsxyy23(i1,i2,i3,axisp2,1)+rsxyz23(i1,i2,i3,axisp2,2))


#defineMacro C1D3r2(i1,i2,i3) (rsxyxr23(i1,i2,i3,axis  ,0)+rsxyyr23(i1,i2,i3,axis  ,1)+rsxyzr23(i1,i2,i3,axis  ,2))
#defineMacro C2D3r2(i1,i2,i3) (rsxyxr23(i1,i2,i3,axisp1,0)+rsxyyr23(i1,i2,i3,axisp1,1)+rsxyzr23(i1,i2,i3,axisp1,2))
#defineMacro C3D3r2(i1,i2,i3) (rsxyxr23(i1,i2,i3,axisp2,0)+rsxyyr23(i1,i2,i3,axisp2,1)+rsxyzr23(i1,i2,i3,axisp2,2))

#defineMacro C1D3s2(i1,i2,i3) (rsxyxs23(i1,i2,i3,axis  ,0)+rsxyys23(i1,i2,i3,axis  ,1)+rsxyzs23(i1,i2,i3,axis  ,2))
#defineMacro C2D3s2(i1,i2,i3) (rsxyxs23(i1,i2,i3,axisp1,0)+rsxyys23(i1,i2,i3,axisp1,1)+rsxyzs23(i1,i2,i3,axisp1,2))
#defineMacro C3D3s2(i1,i2,i3) (rsxyxs23(i1,i2,i3,axisp2,0)+rsxyys23(i1,i2,i3,axisp2,1)+rsxyzs23(i1,i2,i3,axisp2,2))

#defineMacro C1D3t2(i1,i2,i3) (rsxyxt23(i1,i2,i3,axis  ,0)+rsxyyt23(i1,i2,i3,axis  ,1)+rsxyzt23(i1,i2,i3,axis  ,2))
#defineMacro C2D3t2(i1,i2,i3) (rsxyxt23(i1,i2,i3,axisp1,0)+rsxyyt23(i1,i2,i3,axisp1,1)+rsxyzt23(i1,i2,i3,axisp1,2))
#defineMacro C3D3t2(i1,i2,i3) (rsxyxt23(i1,i2,i3,axisp2,0)+rsxyyt23(i1,i2,i3,axisp2,1)+rsxyzt43(i1,i2,i3,axisp2,2))


#defineMacro C1D3r4(i1,i2,i3) (rsxyxr43(i1,i2,i3,axis  ,0)+rsxyyr43(i1,i2,i3,axis  ,1)+rsxyzr43(i1,i2,i3,axis  ,2))
#defineMacro C2D3r4(i1,i2,i3) (rsxyxr43(i1,i2,i3,axisp1,0)+rsxyyr43(i1,i2,i3,axisp1,1)+rsxyzr43(i1,i2,i3,axisp1,2))
#defineMacro C3D3r4(i1,i2,i3) (rsxyxr43(i1,i2,i3,axisp2,0)+rsxyyr43(i1,i2,i3,axisp2,1)+rsxyzr43(i1,i2,i3,axisp2,2))

#defineMacro C1D3s4(i1,i2,i3) (rsxyxs43(i1,i2,i3,axis  ,0)+rsxyys43(i1,i2,i3,axis  ,1)+rsxyzs43(i1,i2,i3,axis  ,2))
#defineMacro C2D3s4(i1,i2,i3) (rsxyxs43(i1,i2,i3,axisp1,0)+rsxyys43(i1,i2,i3,axisp1,1)+rsxyzs43(i1,i2,i3,axisp1,2))
#defineMacro C3D3s4(i1,i2,i3) (rsxyxs43(i1,i2,i3,axisp2,0)+rsxyys43(i1,i2,i3,axisp2,1)+rsxyzs43(i1,i2,i3,axisp2,2))

#defineMacro C1D3t4(i1,i2,i3) (rsxyxt43(i1,i2,i3,axis  ,0)+rsxyyt43(i1,i2,i3,axis  ,1)+rsxyzt43(i1,i2,i3,axis  ,2))
#defineMacro C2D3t4(i1,i2,i3) (rsxyxt43(i1,i2,i3,axisp1,0)+rsxyyt43(i1,i2,i3,axisp1,1)+rsxyzt43(i1,i2,i3,axisp1,2))
#defineMacro C3D3t4(i1,i2,i3) (rsxyxt43(i1,i2,i3,axisp2,0)+rsxyyt43(i1,i2,i3,axisp2,1)+rsxyzt43(i1,i2,i3,axisp2,2))


! ***************************************************************************************************



#defineMacro Dr(A) (A(i1+1,i2,i3)-A(i1-1,i2,i3))/(2.*dr(0)) 
#defineMacro Ds(A) (A(i1,i2+1,i3)-A(i1,i2-1,i3))/(2.*dr(1)) 
#defineMacro Dt(A) (A(i1,i2,i3+1)-A(i1,i2,i3-1))/(2.*dr(2)) 

#defineMacro Drs(A) ( (A(i1+1,i2+1,i3)-A(i1-1,i2+1,i3)) - \
                      (A(i1+1,i2-1,i3)-A(i1-1,i2-1,i3)) )/(4.*dr(0)*dr(1))
#defineMacro Drt(A) ( (A(i1+1,i2,i3+1)-A(i1-1,i2,i3+1)) - \
                      (A(i1+1,i2,i3-1)-A(i1-1,i2,i3-1)) )/(4.*dr(0)*dr(2))
#defineMacro Dst(A) ( (A(i1,i2+1,i3+1)-A(i1,i2-1,i3+1)) - \
                      (A(i1,i2+1,i3-1)-A(i1,i2-1,i3-1)) )/(4.*dr(1)*dr(2))

#defineMacro Drr(A) (A(i1+1,i2,i3)-2.*A(i1,i2,i3)+A(i1-1,i2,i3))/(dr(0)**2)
#defineMacro Dss(A) (A(i1,i2+1,i3)-2.*A(i1,i2,i3)+A(i1,i2-1,i3))/(dr(1)**2)
#defineMacro Dtt(A) (A(i1,i2,i3+1)-2.*A(i1,i2,i3)+A(i1,i2,i3-1))/(dr(2)**2)

#defineMacro Drrs(A) ( (A(i1+1,i2+1,i3)-2.*A(i1,i2+1,i3)+A(i1-1,i2+1,i3)) \
                      -(A(i1+1,i2-1,i3)-2.*A(i1,i2-1,i3)+A(i1-1,i2-1,i3)) \
                         )/(2.*dr(1)*dr(0)**2)
#defineMacro Drrt(A) ( (A(i1+1,i2,i3+1)-2.*A(i1,i2,i3+1)+A(i1-1,i2,i3+1)) \
                      -(A(i1+1,i2,i3-1)-2.*A(i1,i2,i3-1)+A(i1-1,i2,i3-1)) \
                         )/(2.*dr(2)*dr(0)**2)

#defineMacro Dr4(A) ( 8.*(A(i1+1,i2,i3)-A(i1-1,i2,i3)) -(A(i1+2,i2,i3)-A(i1-2,i2,i3)) )/(12.*dr(0))
#defineMacro Ds4(A) ( 8.*(A(i1,i2+1,i3)-A(i1,i2-1,i3)) -(A(i1,i2+2,i3)-A(i1,i2-2,i3)) )/(12.*dr(1))
#defineMacro Dt4(A) ( 8.*(A(i1,i2,i3+1)-A(i1,i2,i3-1)) -(A(i1,i2,i3+2)-A(i1,i2,i3-2)) )/(12.*dr(2))

#defineMacro Drr4(A) ( (-30.*A(i1,i2,i3)+16.*(A(i1+1,i2,i3)+A(i1-1,i2,i3))\
                                         -(A(i1+2,i2,i3)+A(i1-2,i2,i3)) )/(12.*dr(0)**2) )
#defineMacro Dss4(A) ( (-30.*A(i1,i2,i3)+16.*(A(i1,i2+1,i3)+A(i1,i2-1,i3))\
                                         -(A(i1,i2+2,i3)+A(i1,i2-2,i3)) )/(12.*dr(1)**2) )
#defineMacro Dtt4(A) ( (-30.*A(i1,i2,i3)+16.*(A(i1,i2,i3+1)+A(i1,i2,i3-1))\
                                         -(A(i1,i2,i3+2)+A(i1,i2,i3-2)) )/(12.*dr(2)**2) )

#defineMacro Drs4(A) (8.*((8.*(A(i1+1,i2+1,i3)-A(i1-1,i2+1,i3))\
          -(A(i1+2,i2+1,i3)-A(i1-2,i2+1,i3)))/(12.*dr(0))\
      -(8.*(A(i1+1,i2-1,i3)-A(i1-1,i2-1,i3))-(A(i1+2,i2-1,i3)-A(i1-2,i2-1,i3)))/(12.*dr(0)))\
     -((8.*(A(i1+1,i2+2,i3)-A(i1-1,i2+2,i3))-(A(i1+2,i2+2,i3)-A(i1-2,i2+2,i3)))/(12.*dr(0))\
      -(8.*(A(i1+1,i2-2,i3)-A(i1-1,i2-2,i3))-(A(i1+2,i2-2,i3)-A(i1-2,i2-2,i3)))/(12.*dr(0))))/(12.*dr(1)) 

#defineMacro Drt4(A) (8.*(\
       (8.*(A(i1+1,i2,i3+1)-A(i1-1,i2,i3+1))-(A(i1+2,i2,i3+1)-A(i1-2,i2,i3+1)))/(12.*dr(0))\
      -(8.*(A(i1+1,i2,i3-1)-A(i1-1,i2,i3-1))-(A(i1+2,i2,i3-1)-A(i1-2,i2,i3-1)))/(12.*dr(0)))\
     -((8.*(A(i1+1,i2,i3+2)-A(i1-1,i2,i3+2))-(A(i1+2,i2,i3+2)-A(i1-2,i2,i3+2)))/(12.*dr(0))\
      -(8.*(A(i1+1,i2,i3-2)-A(i1-1,i2,i3-2))-(A(i1+2,i2,i3-2)-A(i1-2,i2,i3-2)))/(12.*dr(0))))/(12.*dr(2)) 

#defineMacro Dst4(A) (8.*(\
       (8.*(A(i1,i2+1,i3+1)-A(i1,i2-1,i3+1))-(A(i1,i2+2,i3+1)-A(i1,i2-2,i3+1)))/(12.*dr(1))\
      -(8.*(A(i1,i2+1,i3-1)-A(i1,i2-1,i3-1))-(A(i1,i2+2,i3-1)-A(i1,i2-2,i3-1)))/(12.*dr(1)))\
     -((8.*(A(i1,i2+1,i3+2)-A(i1,i2-1,i3+2))-(A(i1,i2+2,i3+2)-A(i1,i2-2,i3+2)))/(12.*dr(1))\
      -(8.*(A(i1,i2+1,i3-2)-A(i1,i2-1,i3-2))-(A(i1,i2+2,i3-2)-A(i1,i2-2,i3-2)))/(12.*dr(1))))/(12.*dr(2)) 

#defineMacro Drrr2(A) (A(i1+2,i2,i3)-2.*A(i1+1,i2,i3)+2.*A(i1-1,i2,i3)-A(i1-2,i2,i3))/(2.*dr(0)**3)
#defineMacro Dsss2(A) (A(i1,i2+2,i3)-2.*A(i1,i2+1,i3)+2.*A(i1,i2-1,i3)-A(i1,i2-2,i3))/(2.*dr(1)**3)
#defineMacro Dttt2(A) (A(i1,i2,i3+2)-2.*A(i1,i2,i3+1)+2.*A(i1,i2,i3-1)-A(i1,i2,i3-2))/(2.*dr(2)**3)

! These next approximations are from diff.maple
#defineMacro Drrs4(A) (128*A(i1+1,i2+1,i3)-240*A(i1,i2+1,i3)+128*A(i1-1,i2+1,i3)-8*A(i1+2,i2+1,i3)-8*A(i1-2,i2+1,i3)-128*A(i1+1,i2-1,i3)+240*A(i1,i2-1,i3)-128*A(i1-1,i2-1,i3)+8*A(i1+2,i2-1,i3)+8*A(i1-2,i2-1,i3)+30*A(i1,i2+2,i3)-16*A(i1-1,i2+2,i3)+A(i1+2,i2+2,i3)+A(i1-2,i2+2,i3)+16*A(i1+1,i2-2,i3)-30*A(i1,i2-2,i3)+16*A(i1-1,i2-2,i3)-A(i1+2,i2-2,i3)-A(i1-2,i2-2,i3)-16*A(i1+1,i2+2,i3))/(144.*dr(0)**2*dr(1))

#defineMacro Drrt4(A) (128*A(i1+1,i2,i3+1)+240*A(i1,i2,i3-1)+8*A(i1+2,i2,i3-1)+8*A(i1-2,i2,i3-1)-128*A(i1-1,i2,i3-1)-240*A(i1,i2,i3+1)-8*A(i1+2,i2,i3+1)-8*A(i1-2,i2,i3+1)-128*A(i1+1,i2,i3-1)+128*A(i1-1,i2,i3+1)-16*A(i1+1,i2,i3+2)+30*A(i1,i2,i3+2)-16*A(i1-1,i2,i3+2)+A(i1+2,i2,i3+2)+A(i1-2,i2,i3+2)+16*A(i1+1,i2,i3-2)-30*A(i1,i2,i3-2)+16*A(i1-1,i2,i3-2)-A(i1+2,i2,i3-2)-A(i1-2,i2,i3-2))/(144.*dr(0)**2*dr(2))

#defineMacro Dsst4(A) (240*A(i1,i2,i3-1)-240*A(i1,i2,i3+1)-16*A(i1,i2+1,i3+2)-16*A(i1,i2-1,i3+2)+A(i1,i2+2,i3+2)+A(i1,i2-2,i3+2)+16*A(i1,i2+1,i3-2)+16*A(i1,i2-1,i3-2)-A(i1,i2+2,i3-2)-A(i1,i2-2,i3-2)+30*A(i1,i2,i3+2)-30*A(i1,i2,i3-2)+128*A(i1,i2+1,i3+1)+128*A(i1,i2-1,i3+1)-8*A(i1,i2+2,i3+1)-8*A(i1,i2-2,i3+1)-128*A(i1,i2+1,i3-1)-128*A(i1,i2-1,i3-1)+8*A(i1,i2+2,i3-1)+8*A(i1,i2-2,i3-1))/(144.*dr(1)**2*dr(2))

#defineMacro Drss4(A) (128*A(i1+1,i2+1,i3)-128*A(i1-1,i2+1,i3)-16*A(i1+2,i2+1,i3)+16*A(i1-2,i2+1,i3)+128*A(i1+1,i2-1,i3)-128*A(i1-1,i2-1,i3)-16*A(i1+2,i2-1,i3)+16*A(i1-2,i2-1,i3)+30*A(i1+2,i2,i3)+8*A(i1-1,i2+2,i3)+A(i1+2,i2+2,i3)-A(i1-2,i2+2,i3)-8*A(i1+1,i2-2,i3)+8*A(i1-1,i2-2,i3)+A(i1+2,i2-2,i3)-A(i1-2,i2-2,i3)-8*A(i1+1,i2+2,i3)-30*A(i1-2,i2,i3)-240*A(i1+1,i2,i3)+240*A(i1-1,i2,i3))/(144.*dr(1)**2*dr(0))

#defineMacro Dstt4(A) (-240*A(i1,i2+1,i3)+240*A(i1,i2-1,i3)-8*A(i1,i2+1,i3+2)+8*A(i1,i2-1,i3+2)+A(i1,i2+2,i3+2)-A(i1,i2-2,i3+2)-8*A(i1,i2+1,i3-2)+8*A(i1,i2-1,i3-2)+A(i1,i2+2,i3-2)-A(i1,i2-2,i3-2)+30*A(i1,i2+2,i3)-30*A(i1,i2-2,i3)+128*A(i1,i2+1,i3+1)-128*A(i1,i2-1,i3+1)-16*A(i1,i2+2,i3+1)+16*A(i1,i2-2,i3+1)+128*A(i1,i2+1,i3-1)-128*A(i1,i2-1,i3-1)-16*A(i1,i2+2,i3-1)+16*A(i1,i2-2,i3-1))/(144.*dr(2)**2*dr(1))

#defineMacro Drtt4(A) (128*A(i1+1,i2,i3+1)-16*A(i1+2,i2,i3-1)+16*A(i1-2,i2,i3-1)-128*A(i1-1,i2,i3-1)-16*A(i1+2,i2,i3+1)+16*A(i1-2,i2,i3+1)+128*A(i1+1,i2,i3-1)-128*A(i1-1,i2,i3+1)-8*A(i1+1,i2,i3+2)+8*A(i1-1,i2,i3+2)+A(i1+2,i2,i3+2)-A(i1-2,i2,i3+2)-8*A(i1+1,i2,i3-2)+8*A(i1-1,i2,i3-2)+A(i1+2,i2,i3-2)-A(i1-2,i2,i3-2)+30*A(i1+2,i2,i3)-30*A(i1-2,i2,i3)-240*A(i1+1,i2,i3)+240*A(i1-1,i2,i3))/(144.*dr(2)**2*dr(0))


! ========================================================================================

#defineMacro DR(A) (A(i1+is1,i2+is2,i3+is3)-A(i1-is1,i2-is2,i3-is3))/(2.*dra) 
#defineMacro DS(A) (A(i1+js1,i2+js2,i3+js3)-A(i1-js1,i2-js2,i3-js3))/(2.*dsa) 
#defineMacro DT(A) (A(i1+ks1,i2+ks2,i3+ks3)-A(i1-ks1,i2-ks2,i3-ks3))/(2.*dta) 

! **note** this next one only works in 2D
#defineMacro DRS(A) ( (A(i1+1,i2+1,i3)-A(i1-1,i2+1,i3)) - \
                      (A(i1+1,i2-1,i3)-A(i1-1,i2-1,i3)) )/(4.*dra*dsa)

#defineMacro DRR(A) (A(i1+is1,i2+is2,i3+is3)-2.*A(i1,i2,i3)+A(i1-is1,i2-is2,i3-is3))/(dra**2)
#defineMacro DSS(A) (A(i1+js1,i2+js2,i3+js3)-2.*A(i1,i2,i3)+A(i1-js1,i2-js2,i3-js3))/(dsa**2)
#defineMacro DTT(A) (A(i1+ks1,i2+ks2,i3+ks3)-2.*A(i1,i2,i3)+A(i1-ks1,i2-ks2,i3-ks3))/(dta**2)

! **note** this next one only works in 2D
#defineMacro DRRS(A) ( (A(i1+is1+js1,i2+is2+js2,i3)-2.*A(i1+js1,i2+js2,i3)+A(i1-is1+js1,i2-is2+js2,i3)) \
                      -(A(i1+is1-js1,i2+is2-js2,i3)-2.*A(i1-js1,i2-js2,i3)+A(i1-is1-js1,i2-is2-js2,i3)) \
                         )/(2.*dsa*dra**2)

#defineMacro DR4(A) ( 8.*(A(i1+is1,i2+is2,i3+is3)-A(i1-is1,i2-is2,i3-is3)) -(A(i1+2*is1,i2+2*is2,i3+2*is3)-A(i1-2*is1,i2-2*is2,i3-2*is3)) )/(12.*dra)
#defineMacro DS4(A) ( 8.*(A(i1+js1,i2+js2,i3+js3)-A(i1-js1,i2-js2,i3-js3)) -(A(i1+2*js1,i2+2*js2,i3+2*js3)-A(i1-2*js1,i2-2*js2,i3-2*js3)) )/(12.*dsa)
#defineMacro DT4(A) ( 8.*(A(i1+ks1,i2+ks2,i3+ks3)-A(i1-ks1,i2-ks2,i3-ks3)) -(A(i1+2*ks1,i2+2*ks2,i3+2*ks3)-A(i1-2*ks1,i2-2*ks2,i3-2*ks3)) )/(12.*dta)

#defineMacro DRR4(A) ( (-30.*A(i1,i2,i3)+16.*(A(i1+is1,i2+is2,i3+is3)+A(i1-is1,i2-is2,i3-is3))\
                                        -(A(i1+2*is1,i2+2*is2,i3+2*is3)+A(i1-2*is1,i2-2*is2,i3-2*is3)) )/(12.*dra**2) )
#defineMacro DSS4(A) ( (-30.*A(i1,i2,i3)+16.*(A(i1+js1,i2+js2,i3+js3)+A(i1-js1,i2-js2,i3-js3))\
                                        -(A(i1+2*js1,i2+2*js2,i3+2*js3)+A(i1-2*js1,i2-2*js2,i3-2*js3)) )/(12.*dsa**2) )
#defineMacro DTT4(A) ( (-30.*A(i1,i2,i3)+16.*(A(i1+ks1,i2+ks2,i3+ks3)+A(i1-ks1,i2-ks2,i3-ks3))\
                                        -(A(i1+2*ks1,i2+2*ks2,i3+2*ks3)+A(i1-2*ks1,i2-2*ks2,i3-2*ks3)) )/(12.*dta**2) )

! These next two came from ov/bpp/test11.bf
! **note** THESE ARE WRONG ****
#defineMacro DRS4WRONG(A) (8.*((8.*(A(i1+is1,i2+js2+is2,i3)-A(i1-is1,i2+js2- \
      is2,i3))-(A(i1+2*is1,i2+js2+2*is2,i3)-A(i1-2*is1,i2+js2- \
      2*is2,i3)))/(12.*dra)-(8.*(A(i1+is1,i2-js2+is2,i3)-A(i1- \
      is1,i2-js2-is2,i3))-(A(i1+2*is1,i2-js2+2*is2,i3)-A(i1-2* \
      is1,i2-js2-2*is2,i3)))/(12.*dra))-((8.*(A(i1+is1,i2+2*js2+ \
      is2,i3)-A(i1-is1,i2+2*js2-is2,i3))-(A(i1+2*is1,i2+2*js2+ \
      2*is2,i3)-A(i1-2*is1,i2+2*js2-2*is2,i3)))/(12.*dra)-(8.*( \
      A(i1+is1,i2-2*js2+is2,i3)-A(i1-is1,i2-2*js2-is2,i3))-( \
      A(i1+2*is1,i2-2*js2+2*is2,i3)-A(i1-2*is1,i2-2*js2-2*is2,i3)))/(12.*dra)))/(12.*dsa) 


#defineMacro DRSS4WRONG(A) (8.*(((-30.*A(i1+is1,i2,i3)+16.*(A(i1+is1+js1,i2+js2,i3)+ \
      A(i1+is1-js1,i2-js2,i3))-(A(i1+is1+2*js1,i2+2*js2,i3)+A(i1+is1- \
      2*js1,i2-2*js2,i3)))/(12.*dsa**2))-((-30.*A(i1-is1,i2,i3)+16.*( \
      A(i1-is1+js1,i2+js2,i3)+A(i1-is1-js1,i2-js2,i3))-(A(i1-is1+2* \
      js1,i2+2*js2,i3)+A(i1-is1-2*js1,i2-2*js2,i3)))/(12.*dsa**2)))-( \
      ((-30.*A(i1+2*is1,i2,i3)+16.*(A(i1+2*is1+js1,i2+js2,i3)+A(i1+2* \
      is1-js1,i2-js2,i3))-(A(i1+2*is1+2*js1,i2+2*js2,i3)+A(i1+2*is1- \
      2*js1,i2-2*js2,i3)))/(12.*dsa**2))-((-30.*A(i1-2*is1,i2,i3)+ \
      16.*(A(i1-2*is1+js1,i2+js2,i3)+A(i1-2*is1-js1,i2-js2,i3))-(A( \
      i1-2*is1+2*js1,i2+2*js2,i3)+A(i1-2*is1-2*js1,i2-2*js2,i3)))/( \
      12.*dsa**2))))/(12.*dra)

#defineMacro DRRS4WRONG(A) (8.*((-30.*A(i1,i2+js2,i3)+16.*(A(i1+is1,i2+js2+is2,i3)+ \
      A(i1-is1,i2+js2-is2,i3))-(A(i1+2*is1,i2+js2+2*is2,i3)+A(i1-2* \
      is1,i2+js2-2*is2,i3)))/(12.*dra**2)-(-30.*A(i1,i2-js2,i3)+16.*( \
      A(i1+is1,i2-js2+is2,i3)+A(i1-is1,i2-js2-is2,i3))-(A(i1+2*is1, \
      i2-js2+2*is2,i3)+A(i1-2*is1,i2-js2-2*is2,i3)))/(12.*dra**2))-(( \
      -30.*A(i1,i2+2*js2,i3)+16.*(A(i1+is1,i2+2*js2+is2,i3)+A(i1-is1, \
      i2+2*js2-is2,i3))-(A(i1+2*is1,i2+2*js2+2*is2,i3)+A(i1-2*is1,i2+ \
      2*js2-2*is2,i3)))/(12.*dra**2)-(-30.*A(i1,i2-2*js2,i3)+16.*(A( \
      i1+is1,i2-2*js2+is2,i3)+A(i1-is1,i2-2*js2-is2,i3))-(A(i1+2*is1, \
      i2-2*js2+2*is2,i3)+A(i1-2*is1,i2-2*js2-2*is2,i3)))/(12.*dra**2) \
      ))/(12.*dsa)


#defineMacro DSSS2(A) (A(i1+2*js1,i2+2*js2,i3+2*js3)-2.*A(i1+js1,i2+js2,i3+js3)+2.*A(i1-js1,i2-js2,i3-js3)\
                      -A(i1-2*js1,i2-2*js2,i3-2*js3))/(2.*dsa**3)
#defineMacro DTTT2(A) (A(i1+2*ks1,i2+2*ks2,i3+2*ks3)-2.*A(i1+ks1,i2+ks2,i3+ks3)+2.*A(i1-ks1,i2-ks2,i3-ks3)\
                      -A(i1-2*ks1,i2-2*ks2,i3-2*ks3))/(2.*dta**3)


#defineMacro DSSS4(A) (8*A(i1+2*js1,i2+2*js2,i3+2*js3)-13*A(i1+js1,i2+js2,i3+js3)+13*A(i1-js1,i2-js2,i3-js3)\
             -8*A(i1-2*js1,i2-2*js2,i3-2*js3)-A(i1+3*js1,i2+3*js2,i3+3*js3)+A(i1-3*js1,i2-3*js2,i3-3*js3))/(8.*dsa**3)



#defineMacro UR2(c)   (u(i1+is1,i2+is2,i3+is3,c)-u(i1-is1,i2-is2,i3-is3,c))/(2.*dra)
#defineMacro URR2(c)  (u(i1+is1,i2+is2,i3+is3,c)-2.*u(i1,i2,i3,c)+u(i1-is1,i2-is2,i3-is3,c))/(dra**2)
#defineMacro URRR2(c) (u(i1+2*is1,i2+2*is2,i3+2*is3,c)-2.*u(i1+is1,i2+is2,i3+is3,c)+2.*u(i1-is1,i2-is2,i3-is3,c)\
                      -u(i1-2*is1,i2-2*is2,i3-2*is3,c))/(2.*dra**3)

#defineMacro US2(c)   (u(i1+js1,i2+js2,i3+js3,c)-u(i1-js1,i2-js2,i3-js3,c))/(2.*dsa)
#defineMacro USS2(c)  (u(i1+js1,i2+js2,i3+js3,c)-2.*u(i1,i2,i3,c)+u(i1-js1,i2-js2,i3-js3,c))/(dsa**2)
#defineMacro USSS2(c) (u(i1+2*js1,i2+2*js2,i3+2*js3,c)-2.*u(i1+js1,i2+js2,i3+js3,c)+2.*u(i1-js1,i2-js2,i3-js3,c)\
                      -u(i1-2*js1,i2-2*js2,i3-2*js3,c))/(2.*dsa**3)

#defineMacro UT2(c)   (u(i1+ks1,i2+ks2,i3+ks3,c)-u(i1-ks1,i2-ks2,i3-ks3,c))/(2.*dta)
#defineMacro UTT2(c)  (u(i1+ks1,i2+ks2,i3+ks3,c)-2.*u(i1,i2,i3,c)+u(i1-ks1,i2-ks2,i3-ks3,c))/(dta**2)
#defineMacro UTTT2(c) (u(i1+2*ks1,i2+2*ks2,i3+2*ks3,c)-2.*u(i1+ks1,i2+ks2,i3+ks3,c)+2.*u(i1-ks1,i2-ks2,i3-ks3,c)\
                      -u(i1-2*ks1,i2-2*ks2,i3-2*ks3,c))/(2.*dta**3)

#defineMacro UR4(c)   (8.*(u(i1+  is1,i2+  is2,i3+  is3,c)-u(i1-  is1,i2-  is2,i3-  is3,c))   \
                         -(u(i1+2*is1,i2+2*is2,i3+2*is3,c)-u(i1-2*is1,i2-2*is2,i3-2*is3,c))   )/(12.*dra)
#defineMacro URR4(c) (-30.*u(i1,i2,i3,c)+16.*(u(i1+is1,i2+is2,i3+is3,c)+u(i1-is1,i2-is2,i3-is3,c))  \
                          -(u(i1+2*is1,i2+2*is2,i3+2*is3,c)+u(i1-2*is1,i2-2*is2,i3-2*is3,c))  )/(12.*dra**2)


#defineMacro US4(c)   (8.*(u(i1+  js1,i2+  js2,i3+  js3,c)-u(i1-  js1,i2-  js2,i3-  js3,c))   \
                         -(u(i1+2*js1,i2+2*js2,i3+2*js3,c)-u(i1-2*js1,i2-2*js2,i3-2*js3,c))   )/(12.*dsa)
#defineMacro USS4(c)  (-30.*u(i1,i2,i3,c)+16.*(u(i1+js1,i2+js2,i3+js3,c)+u(i1-js1,i2-js2,i3-js3,c))  \
                          -(u(i1+2*js1,i2+2*js2,i3+2*js3,c)+u(i1-2*js1,i2-2*js2,i3-2*js3,c))  )/(12.*dsa**2)

#defineMacro UT4(c)   (8.*(u(i1+  ks1,i2+  ks2,i3+  ks3,c)-u(i1-  ks1,i2-  ks2,i3-  ks3,c))   \
                         -(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,c)-u(i1-2*ks1,i2-2*ks2,i3-2*ks3,c))   )/(12.*dta)
#defineMacro UTT4(c)  (-30.*u(i1,i2,i3,c)+16.*(u(i1+ks1,i2+ks2,i3+ks3,c)+u(i1-ks1,i2-ks2,i3-ks3,c))  \
                          -(u(i1+2*ks1,i2+2*ks2,i3+2*ks3,c)+u(i1-2*ks1,i2-2*ks2,i3-2*ks3,c))  )/(12.*dta**2)
#defineMacro UTTT2(c) (u(i1+2*ks1,i2+2*ks2,i3+2*ks3,c)-2.*u(i1+ks1,i2+ks2,i3+ks3,c)+2.*u(i1-ks1,i2-ks2,i3-ks3,c)\
                      -u(i1-2*ks1,i2-2*ks2,i3-2*ks3,c))/(2.*dta**3)

#defineMacro URS4(c) (8.*((8.*(u(i1+is1,i2+js2+is2,i3,c)-u(i1-is1,i2+js2- \
      is2,i3,c))-(u(i1+2*is1,i2+js2+2*is2,i3,c)-u(i1-2*is1,i2+js2- \
      2*is2,i3,c)))/(12.*dra)-(8.*(u(i1+is1,i2-js2+is2,i3,c)-u(i1- \
      is1,i2-js2-is2,i3,c))-(u(i1+2*is1,i2-js2+2*is2,i3,c)-u(i1-2* \
      is1,i2-js2-2*is2,i3,c)))/(12.*dra))-((8.*(u(i1+is1,i2+2*js2+ \
      is2,i3,c)-u(i1-is1,i2+2*js2-is2,i3,c))-(u(i1+2*is1,i2+2*js2+ \
      2*is2,i3,c)-u(i1-2*is1,i2+2*js2-2*is2,i3,c)))/(12.*dra)-(8.*( \
      u(i1+is1,i2-2*js2+is2,i3,c)-u(i1-is1,i2-2*js2-is2,i3,c))-( \
      u(i1+2*is1,i2-2*js2+2*is2,i3,c)-u(i1-2*is1,i2-2*js2-2*is2,i3,c)))/(12.*dra)))/(12.*dsa) 


#defineMacro URRS4(c) (8.*((-30.*u(i1,i2+js2,i3,c)+16.*(u(i1+is1,i2+js2+is2,i3,c)+ \
      u(i1-is1,i2+js2-is2,i3,c))-(u(i1+2*is1,i2+js2+2*is2,i3,c)+u(i1-2* \
      is1,i2+js2-2*is2,i3,c)))/(12.*dra**2)-(-30.*u(i1,i2-js2,i3,c)+16.*( \
      u(i1+is1,i2-js2+is2,i3,c)+u(i1-is1,i2-js2-is2,i3,c))-(u(i1+2*is1, \
      i2-js2+2*is2,i3,c)+u(i1-2*is1,i2-js2-2*is2,i3,c)))/(12.*dra**2))-(( \
      -30.*u(i1,i2+2*js2,i3,c)+16.*(u(i1+is1,i2+2*js2+is2,i3,c)+u(i1-is1, \
      i2+2*js2-is2,i3,c))-(u(i1+2*is1,i2+2*js2+2*is2,i3,c)+u(i1-2*is1,i2+ \
      2*js2-2*is2,i3,c)))/(12.*dra**2)-(-30.*u(i1,i2-2*js2,i3,c)+16.*(u( \
      i1+is1,i2-2*js2+is2,i3,c)+u(i1-is1,i2-2*js2-is2,i3,c))-(u(i1+2*is1, \
      i2-2*js2+2*is2,i3,c)+u(i1-2*is1,i2-2*js2-2*is2,i3,c)))/(12.*dra**2) \
      ))/(12.*dsa)

#defineMacro URSS4(c) (8.*(((-30.*u(i1+is1,i2,i3,c)+16.*(u(i1+is1+js1,i2+js2,i3,c)+ \
      u(i1+is1-js1,i2-js2,i3,c))-(u(i1+is1+2*js1,i2+2*js2,i3,c)+u(i1+is1- \
      2*js1,i2-2*js2,i3,c)))/(12.*dsa**2))-((-30.*u(i1-is1,i2,i3,c)+16.*( \
      u(i1-is1+js1,i2+js2,i3,c)+u(i1-is1-js1,i2-js2,i3,c))-(u(i1-is1+2* \
      js1,i2+2*js2,i3,c)+u(i1-is1-2*js1,i2-2*js2,i3,c)))/(12.*dsa**2)))-( \
      ((-30.*u(i1+2*is1,i2,i3,c)+16.*(u(i1+2*is1+js1,i2+js2,i3,c)+u(i1+2* \
      is1-js1,i2-js2,i3,c))-(u(i1+2*is1+2*js1,i2+2*js2,i3,c)+u(i1+2*is1- \
      2*js1,i2-2*js2,i3,c)))/(12.*dsa**2))-((-30.*u(i1-2*is1,i2,i3,c)+ \
      16.*(u(i1-2*is1+js1,i2+js2,i3,c)+u(i1-2*is1-js1,i2-js2,i3,c))-(u( \
      i1-2*is1+2*js1,i2+2*js2,i3,c)+u(i1-2*is1-2*js1,i2-2*js2,i3,c)))/( \
      12.*dsa**2))))/(12.*dra)


#defineMacro URS2(c) ( (u(i1+1,i2+1,i3,c)-u(i1-1,i2+1,i3,c)) - \
                       (u(i1+1,i2-1,i3,c)-u(i1-1,i2-1,i3,c)) )/(4.*dra*dsa)

#defineMacro URRS2(c) ( (u(i1+is1+js1,i2+is2+js2,i3,c)-2.*u(i1+js1,i2+js2,i3,c)+u(i1-is1+js1,i2-is2+js2,i3,c)) \
                       -(u(i1+is1-js1,i2+is2-js2,i3,c)-2.*u(i1-js1,i2-js2,i3,c)+u(i1-is1-js1,i2-is2-js2,i3,c)) \
                         )/(2.*dsa*dra**2)



!=====================================================================================
! Boundary conditions for a rectangular grid:
!   Normal component of E is even symmetry
!   Tangential components of E are odd symmetry
! In 2d: normal component of Hz is even symmetry (Neumann BC)
!
! DIM: 2,3
! ORDER: 2,4,6,8
! FORCING: none,twilightZone
!=====================================================================================
#beginMacro bcRectangular(DIM,ORDER,FORCING)
 if( debug.gt.1 )then
   write(*,'(" bc4r: **START** grid=",i4," side,axis=",2i2)') grid,side,axis
 end if

 beginLoops()
  if( mask(i1,i2,i3).ne.0 )then
   ! ** u(i1,i2,i3,et1)=0.
   u(i1-is1,i2-is2,i3-is3,en1)= u(i1+is1,i2+is2,i3+is3,en1)
   u(i1-is1,i2-is2,i3-is3,et1)=2.*u(i1,i2,i3,et1)-u(i1+is1,i2+is2,i3+is3,et1)
   #If #DIM == "3" 
     u(i1-is1,i2-is2,i3-is3,et2)=2.*u(i1,i2,i3,et2)-u(i1+is1,i2+is2,i3+is3,et2)
   #End
   #If #DIM == "2" 
     u(i1-is1,i2-is2,i3-is3,hz)=u(i1+is1,i2+is2,i3+is3,hz)
   #End

   if( useChargeDensity.eq.1 )then
    ! div(eps*E) = rho , rho is saved in f(i1,i2,i3,0)
    u(i1-is1,i2-is2,i3-is3,en1)=u(i1-is1,i2-is2,i3-is3,en1) - 2.*dx(axis)*(1-2*side)*f(i1,i2,i3,0)/eps
   end if

   #If #FORCING == "twilightZone" 
     #If #DIM == "2"
       OGF2DFO(i1-is1,i2-is2,i3,t, uvm(ex),uvm(ey),uvm(hz))
       OGF2DFO(i1    ,i2    ,i3,t, uv0(ex),uv0(ey),uv0(hz))
       OGF2DFO(i1+is1,i2+is2,i3,t, uvp(ex),uvp(ey),uvp(hz))

! write(*,'("..bcRectangular: side,axis=",2i3," i1,i2,i3=",3i3," en1,uvm(en1),uvp(en1)=",3e12.4)')\
!            side,axis,i1,i2,i3,u(i1-is1,i2-is2,i3,en1),uvm(en1),uvp(en1)
       

       u(i1-is1,i2-is2,i3,en1)=u(i1-is1,i2-is2,i3,en1) + uvm(en1) - uvp(en1)
       u(i1-is1,i2-is2,i3,et1)=u(i1-is1,i2-is2,i3,et1) + uvm(et1) -2.*uv0(et1) + uvp(et1)
       u(i1-is1,i2-is2,i3,hz )=u(i1-is1,i2-is2,i3,hz ) + uvm(hz)-uvp(hz)
     #Else
       OGF3DFO(i1-is1,i2-is2,i3-is3,t,uvm(ex),uvm(ey),uvm(ez)) 
       OGF3DFO(i1    ,i2    ,i3    ,t,uv0(ex),uv0(ey),uv0(ez))
       OGF3DFO(i1+is1,i2+is2,i3+is3,t,uvp(ex),uvp(ey),uvp(ez))

       u(i1-is1,i2-is2,i3-is3,en1)=u(i1-is1,i2-is2,i3-is3,en1) + uvm(en1) - uvp(en1)
       u(i1-is1,i2-is2,i3-is3,et1)=u(i1-is1,i2-is2,i3-is3,et1) + uvm(et1) -2.*uv0(et1) + uvp(et1)
       u(i1-is1,i2-is2,i3-is3,et2)=u(i1-is1,i2-is2,i3-is3,et2) + uvm(et2) -2.*uv0(et2) + uvp(et2)
     #End

   #Elif #FORCING == "none"
   #Else
      stop 112233
   #End

   #If #ORDER == "4" || #ORDER == "6" || #ORDER == "8"
     u(i1-2*is1,i2-2*is2,i3-2*is3,en1)= u(i1+2*is1,i2+2*is2,i3+2*is3,en1)
     u(i1-2*is1,i2-2*is2,i3-2*is3,et1)=2.*u(i1,i2,i3,et1)-u(i1+2*is1,i2+2*is2,i3+2*is3,et1)
     #If #DIM == "2"
       u(i1-2*is1,i2-2*is2,i3-2*is3,hz)=u(i1+2*is1,i2+2*is2,i3+2*is3,hz)
     #Else
       u(i1-2*is1,i2-2*is2,i3-2*is3,et2)=2.*u(i1,i2,i3,et2)-u(i1+2*is1,i2+2*is2,i3+2*is3,et2)
     #End
     #If #FORCING == "twilightZone" 
      #If #DIM == "2"
       OGF2DFO(i1-2*is1,i2-2*is2,i3,t, uvm(ex),uvm(ey),uvm(hz))
       OGF2DFO(i1      ,i2      ,i3,t, uv0(ex),uv0(ey),uv0(hz))
       OGF2DFO(i1+2*is1,i2+2*is2,i3,t, uvp(ex),uvp(ey),uvp(hz))

       u(i1-2*is1,i2-2*is2,i3,en1)=u(i1-2*is1,i2-2*is2,i3,en1) + uvm(en1) - uvp(en1)
       u(i1-2*is1,i2-2*is2,i3,et1)=u(i1-2*is1,i2-2*is2,i3,et1) + uvm(et1) -2.*uv0(et1) + uvp(et1)
       u(i1-2*is1,i2-2*is2,i3,hz )=u(i1-2*is1,i2-2*is2,i3,hz ) + uvm(hz)-uvp(hz)
      #Else
       OGF3DFO(i1-2*is1,i2-2*is2,i3-2*is3,t,uvm(ex),uvm(ey),uvm(ez)) 
       OGF3DFO(i1      ,i2      ,i3      ,t,uv0(ex),uv0(ey),uv0(ez))
       OGF3DFO(i1+2*is1,i2+2*is2,i3+2*is3,t,uvp(ex),uvp(ey),uvp(ez))

       u(i1-2*is1,i2-2*is2,i3-2*is3,en1)=u(i1-2*is1,i2-2*is2,i3-2*is3,en1) + uvm(en1) - uvp(en1)
       u(i1-2*is1,i2-2*is2,i3-2*is3,et1)=u(i1-2*is1,i2-2*is2,i3-2*is3,et1) + uvm(et1) -2.*uv0(et1) + uvp(et1)
       u(i1-2*is1,i2-2*is2,i3-2*is3,et2)=u(i1-2*is1,i2-2*is2,i3-2*is3,et2) + uvm(et2) -2.*uv0(et2) + uvp(et2)
       ! if( debug.gt.1 )then
       !  write(*,'(" bc4r: i=",3i4," err(-2)=",3e10.2)') i1,i2,i3,u(i1-2*is1,i2-2*is2,i3-2*is3,ex)-uvm(ex),\
       !       u(i1-2*is1,i2-2*is2,i3-2*is3,ey)-uvm(ey), u(i1-2*is1,i2-2*is2,i3-2*is3,ez)-uvm(ez)
       ! end if
      #End
     #End

   #End

   #If #ORDER == "6" || #ORDER == "8"
     u(i1-3*is1,i2-3*is2,i3-3*is3,en1)= u(i1+3*is1,i2+3*is2,i3+3*is3,en1)
     u(i1-3*is1,i2-3*is2,i3-3*is3,et1)=2.*u(i1,i2,i3,et1)-u(i1+3*is1,i2+3*is2,i3+3*is3,et1)
     #If #DIM == "2"
       u(i1-3*is1,i2-3*is2,i3-3*is3,hz)=u(i1+3*is1,i2+3*is2,i3+3*is3,hz)
     #Else
       u(i1-3*is1,i2-3*is2,i3-3*is3,et2)=2.*u(i1,i2,i3,et2)-u(i1+3*is1,i2+3*is2,i3+3*is3,et2)
     #End
     #If #FORCING == "twilightZone" 
      #If #DIM == "2"
       OGF2DFO(i1-3*is1,i2-3*is2,i3,t, uvm(ex),uvm(ey),uvm(hz))
       OGF2DFO(i1      ,i2      ,i3,t, uv0(ex),uv0(ey),uv0(hz))
       OGF2DFO(i1+3*is1,i2+3*is2,i3,t, uvp(ex),uvp(ey),uvp(hz))

       u(i1-3*is1,i2-3*is2,i3,en1)=u(i1-3*is1,i2-3*is2,i3,en1) + uvm(en1) - uvp(en1)
       u(i1-3*is1,i2-3*is2,i3,et1)=u(i1-3*is1,i2-3*is2,i3,et1) + uvm(et1) -2.*uv0(et1) + uvp(et1)
       u(i1-3*is1,i2-3*is2,i3,hz )=u(i1-3*is1,i2-3*is2,i3,hz ) + uvm(hz)-uvp(hz)
      #Else
       OGF3DFO(i1-3*is1,i2-3*is2,i3-3*is3,t,uvm(ex),uvm(ey),uvm(ez)) 
       OGF3DFO(i1      ,i2      ,i3      ,t,uv0(ex),uv0(ey),uv0(ez))
       OGF3DFO(i1+3*is1,i2+3*is2,i3+3*is3,t,uvp(ex),uvp(ey),uvp(ez))

       u(i1-3*is1,i2-3*is2,i3-3*is3,en1)=u(i1-3*is1,i2-3*is2,i3-3*is3,en1) + uvm(en1) - uvp(en1)
       u(i1-3*is1,i2-3*is2,i3-3*is3,et1)=u(i1-3*is1,i2-3*is2,i3-3*is3,et1) + uvm(et1) -2.*uv0(et1) + uvp(et1)
       u(i1-3*is1,i2-3*is2,i3-3*is3,et2)=u(i1-3*is1,i2-3*is2,i3-3*is3,et2) + uvm(et2) -2.*uv0(et2) + uvp(et2)
      #End
     #End

   #End

   #If #ORDER == "8"
     u(i1-4*is1,i2-4*is2,i3-4*is3,en1)= u(i1+4*is1,i2+4*is2,i3+4*is3,en1)
     u(i1-4*is1,i2-4*is2,i3-4*is3,et1)=2.*u(i1,i2,i3,et1)-u(i1+4*is1,i2+4*is2,i3+4*is3,et1)
     #If #DIM == "2"
       u(i1-4*is1,i2-4*is2,i3-4*is3,hz)=2.*u(i1,i2,i3,hz)-u(i1+4*is1,i2+4*is2,i3+4*is3,hz)
     #Else
       u(i1-4*is1,i2-4*is2,i3-4*is3,et2)=2.*u(i1,i2,i3,et2)-u(i1+4*is1,i2+4*is2,i3+4*is3,et2)
     #End
     #If #FORCING == "twilightZone" 
      #If #DIM == "2"
       OGF2DFO(i1-4*is1,i2-4*is2,i3,t, uvm(ex),uvm(ey),uvm(hz))
       OGF2DFO(i1      ,i2      ,i3,t, uv0(ex),uv0(ey),uv0(hz))
       OGF2DFO(i1+4*is1,i2+4*is2,i3,t, uvp(ex),uvp(ey),uvp(hz))

       u(i1-4*is1,i2-4*is2,i3,en1)=u(i1-4*is1,i2-4*is2,i3,en1) + uvm(en1) - uvp(en1)
       u(i1-4*is1,i2-4*is2,i3,et1)=u(i1-4*is1,i2-4*is2,i3,et1) + uvm(et1) -2.*uv0(et1) + uvp(et1)
       u(i1-4*is1,i2-4*is2,i3,hz )=u(i1-4*is1,i2-4*is2,i3,hz ) + uvm(hz)-uvp(hz)
      #Else
       OGF3DFO(i1-4*is1,i2-4*is2,i3-4*is3,t,uvm(ex),uvm(ey),uvm(ez)) 
       OGF3DFO(i1      ,i2      ,i3      ,t,uv0(ex),uv0(ey),uv0(ez))
       OGF3DFO(i1+4*is1,i2+4*is2,i3+4*is3,t,uvp(ex),uvp(ey),uvp(ez))

       u(i1-4*is1,i2-4*is2,i3-4*is3,en1)=u(i1-4*is1,i2-4*is2,i3-4*is3,en1) + uvm(en1) - uvp(en1)
       u(i1-4*is1,i2-4*is2,i3-4*is3,et1)=u(i1-4*is1,i2-4*is2,i3-4*is3,et1) + uvm(et1) -2.*uv0(et1) + uvp(et1)
       u(i1-4*is1,i2-4*is2,i3-4*is3,et2)=u(i1-4*is1,i2-4*is2,i3-4*is3,et2) + uvm(et2) -2.*uv0(et2) + uvp(et2)
      #End
     #End
   #End
  end if ! mask
 endLoops()
#endMacro


! ************************************************************************************************
!  This macro is used for looping over the faces of a grid to assign boundary conditions
!
! extra: extra points to assign
!          Case 1: extra=numberOfGhostPoints -- for assigning extended boundaries
!          Case 2: extra=-1 -- for assigning ghost points but not including extended boundaries
! numberOfGhostPoints : number of ghost points (1 for 2nd order, 2 for fourth-order ...)
! ***********************************************************************************************
#beginMacro beginLoopOverSides(extra,numberOfGhostPoints)
 extra1a=extra
 extra1b=extra
 extra2a=extra
 extra2b=extra
 if( nd.eq.3 )then
   extra3a=extra
   extra3b=extra
 else
   extra3a=0
   extra3b=0
 end if
 if( boundaryCondition(0,0).lt.0 )then
   extra1a=max(0,extra1a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
 else if( boundaryCondition(0,0).eq.0 )then
   extra1a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
 end if
 ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
 if( boundaryCondition(1,0).lt.0 )then
   extra1b=max(0,extra1b) ! over-ride extra=-1 : assign ends in periodic directions
 else if( boundaryCondition(1,0).eq.0 )then
   extra1b=numberOfGhostPoints
 end if

 if( boundaryCondition(0,1).lt.0 )then
   extra2a=max(0,extra2a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
 else if( boundaryCondition(0,1).eq.0 )then
   extra2a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
 end if
 ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
 if( boundaryCondition(1,1).lt.0 )then
   extra2b=max(0,extra2b) ! over-ride extra=-1 : assign ends in periodic directions
 else if( boundaryCondition(1,1).eq.0 )then
   extra2b=numberOfGhostPoints
 end if

 if(  nd.eq.3 )then
  if( boundaryCondition(0,2).lt.0 )then
    extra3a=max(0,extra3a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
  else if( boundaryCondition(0,2).eq.0 )then
    extra3a=numberOfGhostPoints  ! include interpolation points since we assign ghost points outside these
  end if
  ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
  if( boundaryCondition(1,2).lt.0 )then
    extra3b=max(0,extra3b) ! over-ride extra=-1 : assign ends in periodic directions
  else if( boundaryCondition(1,2).eq.0 )then
    extra3b=numberOfGhostPoints
  end if
 end if

 do axis=0,nd-1
 do side=0,1

   if( boundaryCondition(side,axis).eq.perfectElectricalConductor )then

     ! write(*,'(" bcOpt: side,axis,bc=",3i2)') side,axis,boundaryCondition(side,axis)

     n1a=gridIndexRange(0,0)-extra1a
     n1b=gridIndexRange(1,0)+extra1b
     n2a=gridIndexRange(0,1)-extra2a
     n2b=gridIndexRange(1,1)+extra2b
     n3a=gridIndexRange(0,2)-extra3a
     n3b=gridIndexRange(1,2)+extra3b
     if( axis.eq.0 )then
       n1a=gridIndexRange(side,axis)
       n1b=gridIndexRange(side,axis)
     else if( axis.eq.1 )then
       n2a=gridIndexRange(side,axis)
       n2b=gridIndexRange(side,axis)
     else
       n3a=gridIndexRange(side,axis)
       n3b=gridIndexRange(side,axis)
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
     
     axisp1=mod(axis+1,nd)
     axisp2=mod(axis+2,nd)
     
     ! (js1,js2,js3) used to compute tangential derivatives
     js1=0
     js2=0
     js3=0
     if( axisp1.eq.0 )then
       js1=1-2*side
     else if( axisp1.eq.1 )then
       js2=1-2*side
     else if( axisp1.eq.2 )then
       js3=1-2*side
     else
       stop 5
     end if

     ! (ks1,ks2,ks3) used to compute second tangential derivative
     ks1=0
     ks2=0
     ks3=0
     if( axisp2.eq.0 )then
       ks1=1-2*side
     else if( axisp2.eq.1 )then
       ks2=1-2*side
     else if( axisp2.eq.2 )then
       ks3=1-2*side
     else
       stop 5
     end if

 if( debug.gt.7 )then
   write(*,'(" bcOpt: grid,side,axis=",3i3,", loop bounds: n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,\
     n1a,n1b,n2a,n2b,n3a,n3b
 end if
#endMacro

#beginMacro endLoopOverSides()
   else if( boundaryCondition(side,axis).gt.0 .and. \
            boundaryCondition(side,axis).ne.dirichlet .and. \
            boundaryCondition(side,axis).ne.planeWaveBoundaryCondition .and. \
            boundaryCondition(side,axis).ne.symmetryBoundaryCondition .and. \
            boundaryCondition(side,axis).gt.lastBC )then
   ! Note: some BC's such as dirichlet are done in assignBoundaryConditions.C
     write(*,'(" endLoopOverSides:ERROR: unknown boundaryCondition=",i6)') boundaryCondition(side,axis)
   ! '
     stop 7733
   end if
 end do
 end do
#endMacro





#beginMacro BC_MAXWELL(NAME,DIM,ORDER)
 subroutine NAME( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,\
                  ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
                  gridIndexRange,dimension,u,f,mask,rsxy, xy,\
                  bc, boundaryCondition, ipar, rpar, ierr )
! ===================================================================================
!  Optimised Boundary conditions for Maxwell's Equations. '
!
!  gridType : 0=rectangular, 1=curvilinear
!  useForcing : 1=use f for RHS to BC
!  side,axis : 0:1 and 0:2
! ===================================================================================

 implicit none

 integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,\
         n1a,n1b,n2a,n2b,n3a,n3b, ndc, bc,ierr

 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
 real f(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b,0:*)
 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
 real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
 integer gridIndexRange(0:1,0:2),dimension(0:1,0:2)

 integer ipar(0:*),boundaryCondition(0:1,0:2)
  real rpar(0:*),pwc(0:5)

!     --- local variables ----
      
 integer md1a,md1b,md2a,md2b,md3a,md3b
 integer indexRange(0:1,0:2),isPeriodic(0:2) ! used in call to periodic update

 real ep ! holds the pointer to the TZ function
 real pu ! holds pointer to P++ array
 real dt,kx,ky,kz,eps,mu,c,cc,twoPi,slowStartInterval,ssf,ssft,ssftt,ssfttt,ssftttt,tt

 integer is1,is2,is3,js1,js2,js3,ks1,ks2,ks3,orderOfAccuracy,gridType,debug,grid,\
   side,axis,useForcing,ex,ey,ez,hx,hy,hz,useWhereMask,side1,side2,side3,m1,m2,m3,bc1,bc2, \
  js1a,js2a,js3a,ks1a,ks2a,ks3a,forcingOption,useChargeDensity,fieldOption

 real dr(0:2), dx(0:2), t, uv(0:5), uvm(0:5), uv0(0:5), uvp(0:5), uvm2(0:5), uvp2(0:5) 
 real uvmm(0:2),uvzm(0:2),uvpm(0:2)
 real uvmz(0:2),uvzz(0:2),uvpz(0:2)
 real uvmp(0:2),uvzp(0:2),uvpp(0:2)

 integer i10,i20,i30
 real jac3di(-2:2,-2:2,-2:2)

 integer orderOfExtrapolation
 logical setCornersToExact
 logical extrapInterpGhost   ! extrapolate ghost points next to boundary interpolation points 

 ! boundary conditions parameters
 #Include "bcDefineFortranInclude.h"

 integer rectangular,curvilinear
 parameter(\
     rectangular=0,\
     curvilinear=1)

 ! forcing options
 #Include "forcingDefineFortranInclude.h"

 integer i1,i2,i3,j1,j2,j3,axisp1,axisp2,en1,et1,et2,hn1,ht1,ht2,numberOfGhostPoints
 integer extra,extra1a,extra1b,extra2a,extra2b,extra3a,extra3b
 real det,dra,dsa,dta,dxa,dya,dza,drb,dsb,dtb

 real uttp1,uttp2,uttm1,uttm2, vttp1,vttp2,vttm1,vttm2, utts, vtts

 real tau1,tau2,tau11,tau12,tau13, tau21,tau22,tau23 
 real tau11s,tau12s,tau13s, tau21s,tau22s,tau23s
 real tau11t,tau12t,tau13t, tau21t,tau22t,tau23t
 real tau1u,tau2u,tau1Up1,tau1Up2,tau1Up3,tau2Up1,tau2Up2,tau2Up3

 real tau1Dotu,tau2Dotu,tauU,tauUp1,tauUp2,tauUp3,ttu1,ttu2
 real ttu11,ttu12,ttu13, ttu21,ttu22,ttu23

 real DtTau1DotUvr,DtTau2DotUvr,DsTau1DotUvr,DsTau2DotUvr,tau1DotUtt,tau2DotUtt,Da1DotU,a1DotU,a1Dotur
 real drA1DotDeltaU
! real tau1DotUvrs, tau2DotUvrs, tau1DotUvrt, tau2DotUvrt

 real gx1,gx2,g1a,g2a
 real g1,g2,g3
 real tauDotExtrap
 real u0t,v0t,w0t

 real jac,jacm1,jacp1,jacp2,jacm2,jac0,detnt

 real a11,a12,a13,a21,a22,a23,a31,a32,a33
 real a11r,a12r,a13r,a21r,a22r,a23r,a31r,a32r,a33r
 real a11s,a12s,a13s,a21s,a22s,a23s,a31s,a32s,a33s
 real a11t,a12t,a13t,a21t,a22t,a23t,a31t,a32t,a33t

 real a11rr,a12rr,a13rr,a21rr,a22rr,a23rr,a31rr,a32rr,a33rr
 real a11ss,a12ss,a13ss,a21ss,a22ss,a23ss,a31ss,a32ss,a33ss
 real a11tt,a12tt,a13tt,a21tt,a22tt,a23tt,a31tt,a32tt,a33tt
 real a11rs,a12rs,a13rs,a21rs,a22rs,a23rs,a31rs,a32rs,a33rs
 real a11rt,a12rt,a13rt,a21rt,a22rt,a23rt,a31rt,a32rt,a33rt
 real a11st,a12st,a13st,a21st,a22st,a23st,a31st,a32st,a33st

 real a11rrs,a12rrs,a13rrs,a21rrs,a22rrs,a23rrs,a31rrs,a32rrs,a33rrs
 real a11sss,a12sss,a13sss,a21sss,a22sss,a23sss,a31sss,a32sss,a33sss
 real a11rss,a12rss,a13rss,a21rss,a22rss,a23rss,a31rss,a32rss,a33rss
 real a11ttt,a12ttt,a13ttt,a21ttt,a22ttt,a23ttt,a31ttt,a32ttt,a33ttt
 real a11rtt,a12rtt,a13rtt,a21rtt,a22rtt,a23rtt,a31rtt,a32rtt,a33rtt
 real a11sst,a12sst,a13sst,a21sst,a22sst,a23sst,a31sst,a32sst,a33sst
 real a11stt,a12stt,a13stt,a21stt,a22stt,a23stt,a31stt,a32stt,a33stt

 real a11zm1,a12zm1,a13zm1,a21zm1,a22zm1,a23zm1,a31zm1,a32zm1,a33zm1
 real a11zp1,a12zp1,a13zp1,a21zp1,a22zp1,a23zp1,a31zp1,a32zp1,a33zp1
 real a11zm2,a12zm2,a13zm2,a21zm2,a22zm2,a23zm2,a31zm2,a32zm2,a33zm2
 real a11zp2,a12zp2,a13zp2,a21zp2,a22zp2,a23zp2,a31zp2,a32zp2,a33zp2

 real a11m,a12m,a13m,a21m,a22m,a23m,a31m,a32m,a33m
 real a11p,a12p,a13p,a21p,a22p,a23p,a31p,a32p,a33p

 real a11m1,a12m1,a13m1,a21m1,a22m1,a23m1,a31m1,a32m1,a33m1
 real a11p1,a12p1,a13p1,a21p1,a22p1,a23p1,a31p1,a32p1,a33p1
 real a11m2,a12m2,a13m2,a21m2,a22m2,a23m2,a31m2,a32m2,a33m2
 real a11p2,a12p2,a13p2,a21p2,a22p2,a23p2,a31p2,a32p2,a33p2

 real c11,c22,c33,c1,c2,c3
 real c11r,c22r,c33r,c1r,c2r,c3r
 real c11s,c22s,c33s,c1s,c2s,c3s
 real c11t,c22t,c33t,c1t,c2t,c3t

 real uex,uey,uez
 real ur,us,ut,urr, uss,utt,urs,urt,ust, urrr,usss,uttt,urrs,urss,urtt,usst,ustt, urrrr,ussss,urrss,urrrs,ursss
 real vr,vs,vt,vrr, vss,vtt,vrs,vrt,vst, vrrr,vsss,vttt,vrrs,vrss,vrtt,vsst,vstt, vrrrr,vssss,vrrss,vrrrs,vrsss
 real wr,ws,wt,wrr, wss,wtt,wrs,wrt,wst, wrrr,wsss,wttt,wrrs,wrss,wrtt,wsst,wstt, wrrrr,wssss,wrrss,wrrrs,wrsss

 real ursm,urrsm,vrsm,vrrsm, urrm,vrrm

 real uxx,uyy,uzz, vxx,vyy,vzz, wxx,wyy,wzz
 real uxxm2,uyym2,uzzm2, vxxm2,vyym2,vzzm2, wxxm2,wyym2,wzzm2
 real uxxm1,uyym1,uzzm1, vxxm1,vyym1,vzzm1, wxxm1,wyym1,wzzm1
 real uxxp1,uyyp1,uzzp1, vxxp1,vyyp1,vzzp1, wxxp1,wyyp1,wzzp1
 real uxxp2,uyyp2,uzzp2, vxxp2,vyyp2,vzzp2, wxxp2,wyyp2,wzzp2

 real cur,cvr,gI,gIa,gIII,gIV,gIVf

 real uTmTm,vTmTm,wTmTm
 real uTmTmr,vTmTmr,wTmTmr

 real ut0,vt0,utp1,vtp1,utm1,vtm1,uttt0,vttt0
 real uzm,uzp,vzm,vzp,wzm,wzp,wx,wy

 real b3u,b3v,b3w, b2u,b2v,b2w, b1u,b1v,b1w, bf,divtt
 real cw1,cw2,bfw2,fw1,fw2,fw3,fw4
 real fw1m1,fw1p1,wsm1,wsp1

 real f1um1,f1um2,f1vm1,f1vm2,f1wm1,f1wm2,f1f
 real f2um1,f2um2,f2vm1,f2vm2,f2wm1,f2wm2,f2f

 real cursu,cursv,cursw, cvrsu,cvrsv,cvrsw,  cwrsu,cwrsv,cwrsw
 real curtu,curtv,curtw, cvrtu,cvrtv,cvrtw,  cwrtu,cwrtv,cwrtw
 real furs,fvrs,fwrs, furt,fvrt,fwrt 
 real a1DotUvrsRHS,a1DotUvrtRHS, a1DotUvrssRHS,a1DotUvrttRHS
 real gIII1,gIII2,gIVf1,gIVf2,gIV1,gIV2

 real uLap,vLap,wLap,tau1DotLap,tau2DotLap
 real cgI,gIf

 real aNorm,aDotUp,aDotUm,ctlrr,ctlr,div,divc,divc2,tauDotLap,errLapex,errLapey,errLapez

 real aDot1,aDot2,aDotUm2,aDotUm1,aDotU,aDotUp1,aDotUp2,aDotUp3

 real xm,ym,x0,y0,z0,xp,yp,um,vm,wm,u0,v0,w0,up,vp,wp,x00,y00,z00

 real tdu10,tdu01,tdu20,tdu02,gLu,gLv,utt00,vtt00,wtt00
 real cu10,cu01,cu20,cu02,cv10,cv01,cv20,cv02

 ! Here are time derivatives which are denoted using "d"
 real udd,vdd,wdd,uddp1,vddp1,wddp1,uddm1,vddm1,wddm1,uddp2,vddp2,wddp2,uddm2,vddm2,wddm2
 real udds,vdds,wdds,uddt,vddt,wddt

 real maxDivc,maxTauDotLapu,maxExtrap,maxDr3aDotU,dr3aDotU,a1Doturss


! real uxxx22r,uyyy22r,uxxx42r,uyyy42r,uxxxx22r,uyyyy22r, urrrr2,ussss2
 real urrrr2,ussss2
 real urrs4,urrt4,usst4,urss4,ustt4,urtt4
 real urrs2,urrt2,usst2,urss2,ustt2,urtt2

#Include "declareJacobianDerivatives.h"

!     --- start statement function ----
 integer kd,m,n
 real rx,ry,rz,sx,sy,sz,tx,ty,tz
! old: include 'declareDiffOrder2f.h'
! old: include 'declareDiffOrder4f.h'
 declareDifferenceOrder2(u,RX)
 declareDifferenceOrder4(u,RX)

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
 defineDifferenceOrder4Components1(u,RX)

! define derivatives of rsxy
#Include "jacobianDerivatives.h"

! uxxx22r(i1,i2,i3,kd)=(-2.*(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))+(u(i1+2,i2,i3,kd)-u(i1-2,i2,i3,kd)) )*h22(0)*h12(0)
! uyyy22r(i1,i2,i3,kd)=(-2.*(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))+(u(i1,i2+2,i3,kd)-u(i1,i2-2,i3,kd)) )*h22(1)*h12(1)

! uxxxx22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))\
!                         +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dx(0)**4)

! uyyyy22r(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))\
!                         +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dx(1)**4)

 urrrr2(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))\
                         +(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )/(dr(0)**4)

 ussss2(i1,i2,i3,kd)=(6.*u(i1,i2,i3,kd)-4.*(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd))\
                         +(u(i1,i2+2,i3,kd)+u(i1,i2-2,i3,kd)) )/(dr(1)**4)

! add these to the derivatives include file

 urrs2(i1,i2,i3,kd)=(urr2(i1,i2+1,i3,kd)-urr2(i1,i2-1,i3,kd))/(2.*dr(1))
 urrt2(i1,i2,i3,kd)=(urr2(i1,i2,i3+1,kd)-urr2(i1,i2,i3-1,kd))/(2.*dr(2))

 urss2(i1,i2,i3,kd)=(uss2(i1+1,i2,i3,kd)-uss2(i1-1,i2,i3,kd))/(2.*dr(0))
 usst2(i1,i2,i3,kd)=(uss2(i1,i2,i3+1,kd)-uss2(i1,i2,i3-1,kd))/(2.*dr(2))

 urtt2(i1,i2,i3,kd)=(utt2(i1+1,i2,i3,kd)-utt2(i1-1,i2,i3,kd))/(2.*dr(0))
 ustt2(i1,i2,i3,kd)=(utt2(i1,i2+1,i3,kd)-utt2(i1,i2-1,i3,kd))/(2.*dr(1))

! these are from diff.maple
 urrs4(i1,i2,i3,kd) = (u(i1-2,i2+2,i3,kd)+16*u(i1+1,i2-2,i3,kd)-30*u(i1,i2-2,i3,kd)+16*u(i1-1,i2-2,i3,kd)-u(i1+2,i2-2,i3,kd)-u(i1-2,i2-2,i3,kd)-16*u(i1+1,i2+2,i3,kd)+30*u(i1,i2+2,i3,kd)-16*u(i1-1,i2+2,i3,kd)+u(i1+2,i2+2,i3,kd)-240*u(i1,i2+1,i3,kd)-8*u(i1+2,i2+1,i3,kd)-8*u(i1-2,i2+1,i3,kd)-128*u(i1+1,i2-1,i3,kd)+240*u(i1,i2-1,i3,kd)-128*u(i1-1,i2-1,i3,kd)+8*u(i1+2,i2-1,i3,kd)+8*u(i1-2,i2-1,i3,kd)+128*u(i1-1,i2+1,i3,kd)+128*u(i1+1,i2+1,i3,kd))/(144.*dr(0)**2*dr(1))

 urrt4(i1,i2,i3,kd) = (30*u(i1,i2,i3+2,kd)-16*u(i1-1,i2,i3+2,kd)+u(i1+2,i2,i3+2,kd)-16*u(i1+1,i2,i3+2,kd)-30*u(i1,i2,i3-2,kd)+16*u(i1+1,i2,i3-2,kd)+u(i1-2,i2,i3+2,kd)-u(i1+2,i2,i3-2,kd)-u(i1-2,i2,i3-2,kd)+16*u(i1-1,i2,i3-2,kd)+128*u(i1+1,i2,i3+1,kd)-240*u(i1,i2,i3+1,kd)+128*u(i1-1,i2,i3+1,kd)-8*u(i1+2,i2,i3+1,kd)-8*u(i1-2,i2,i3+1,kd)-128*u(i1+1,i2,i3-1,kd)+240*u(i1,i2,i3-1,kd)-128*u(i1-1,i2,i3-1,kd)+8*u(i1+2,i2,i3-1,kd)+8*u(i1-2,i2,i3-1,kd))/(144.*dr(0)**2*dr(2))

 usst4(i1,i2,i3,kd) = (30*u(i1,i2,i3+2,kd)-30*u(i1,i2,i3-2,kd)+128*u(i1,i2+1,i3+1,kd)+128*u(i1,i2-1,i3+1,kd)-8*u(i1,i2+2,i3+1,kd)-8*u(i1,i2-2,i3+1,kd)-128*u(i1,i2+1,i3-1,kd)-128*u(i1,i2-1,i3-1,kd)+8*u(i1,i2+2,i3-1,kd)+8*u(i1,i2-2,i3-1,kd)-240*u(i1,i2,i3+1,kd)+240*u(i1,i2,i3-1,kd)+16*u(i1,i2+1,i3-2,kd)-16*u(i1,i2+1,i3+2,kd)-16*u(i1,i2-1,i3+2,kd)+u(i1,i2+2,i3+2,kd)+u(i1,i2-2,i3+2,kd)+16*u(i1,i2-1,i3-2,kd)-u(i1,i2+2,i3-2,kd)-u(i1,i2-2,i3-2,kd))/(144.*dr(1)**2*dr(2))

 urss4(i1,i2,i3,kd) = (-240*u(i1+1,i2,i3,kd)+240*u(i1-1,i2,i3,kd)-u(i1-2,i2+2,i3,kd)-8*u(i1+1,i2-2,i3,kd)+8*u(i1-1,i2-2,i3,kd)+u(i1+2,i2-2,i3,kd)-u(i1-2,i2-2,i3,kd)-8*u(i1+1,i2+2,i3,kd)+8*u(i1-1,i2+2,i3,kd)+u(i1+2,i2+2,i3,kd)-16*u(i1+2,i2+1,i3,kd)+16*u(i1-2,i2+1,i3,kd)+128*u(i1+1,i2-1,i3,kd)-128*u(i1-1,i2-1,i3,kd)-16*u(i1+2,i2-1,i3,kd)+16*u(i1-2,i2-1,i3,kd)-128*u(i1-1,i2+1,i3,kd)+128*u(i1+1,i2+1,i3,kd)-30*u(i1-2,i2,i3,kd)+30*u(i1+2,i2,i3,kd))/(144.*dr(1)**2*dr(0))

 ustt4(i1,i2,i3,kd) = (-30*u(i1,i2-2,i3,kd)+30*u(i1,i2+2,i3,kd)-240*u(i1,i2+1,i3,kd)+240*u(i1,i2-1,i3,kd)+128*u(i1,i2+1,i3+1,kd)-128*u(i1,i2-1,i3+1,kd)-16*u(i1,i2+2,i3+1,kd)+16*u(i1,i2-2,i3+1,kd)+128*u(i1,i2+1,i3-1,kd)-128*u(i1,i2-1,i3-1,kd)-16*u(i1,i2+2,i3-1,kd)+16*u(i1,i2-2,i3-1,kd)-8*u(i1,i2+1,i3-2,kd)-8*u(i1,i2+1,i3+2,kd)+8*u(i1,i2-1,i3+2,kd)+u(i1,i2+2,i3+2,kd)-u(i1,i2-2,i3+2,kd)+8*u(i1,i2-1,i3-2,kd)+u(i1,i2+2,i3-2,kd)-u(i1,i2-2,i3-2,kd))/(144.*dr(2)**2*dr(1))

 urtt4(i1,i2,i3,kd) = (-240*u(i1+1,i2,i3,kd)+240*u(i1-1,i2,i3,kd)+8*u(i1-1,i2,i3+2,kd)+u(i1+2,i2,i3+2,kd)-8*u(i1+1,i2,i3+2,kd)-8*u(i1+1,i2,i3-2,kd)-u(i1-2,i2,i3+2,kd)+u(i1+2,i2,i3-2,kd)-u(i1-2,i2,i3-2,kd)+8*u(i1-1,i2,i3-2,kd)+128*u(i1+1,i2,i3+1,kd)-128*u(i1-1,i2,i3+1,kd)-16*u(i1+2,i2,i3+1,kd)+16*u(i1-2,i2,i3+1,kd)+128*u(i1+1,i2,i3-1,kd)-128*u(i1-1,i2,i3-1,kd)-16*u(i1+2,i2,i3-1,kd)+16*u(i1-2,i2,i3-1,kd)-30*u(i1-2,i2,i3,kd)+30*u(i1+2,i2,i3,kd))/(144.*dr(2)**2*dr(0))

!     --- end statement functions ----

 ierr=0

 side                 =ipar(0)
 axis                 =ipar(1)
 n1a                  =ipar(2)
 n1b                  =ipar(3)
 n2a                  =ipar(4)
 n2b                  =ipar(5)
 n3a                  =ipar(6)
 n3b                  =ipar(7)
 gridType             =ipar(8)
 orderOfAccuracy      =ipar(9)
 orderOfExtrapolation =ipar(10)
 useForcing           =ipar(11)
 ex                   =ipar(12)
 ey                   =ipar(13)
 ez                   =ipar(14)
 hx                   =ipar(15)
 hy                   =ipar(16)
 hz                   =ipar(17)
 useWhereMask         =ipar(18)
 grid                 =ipar(19)
 debug                =ipar(20)
 forcingOption        =ipar(21)
 useChargeDensity     =ipar(24)

 fieldOption          =ipar(29)  ! 0=assign field, 1=assign time derivatives

 dx(0)                =rpar(0)
 dx(1)                =rpar(1)
 dx(2)                =rpar(2)
 dr(0)                =rpar(3)
 dr(1)                =rpar(4)
 dr(2)                =rpar(5)
 t                    =rpar(6)
 ep                   =rpar(7)
 dt                   =rpar(8)
 c                    =rpar(9)
 eps                  =rpar(10)
 mu                   =rpar(11)
 kx                   =rpar(12)  ! for plane wave forcing
 ky                   =rpar(13)
 kz                   =rpar(14)
 slowStartInterval    =rpar(15)
 ! pmlLayerStrength   =rpar(16)
 pu                   =rpar(17)   ! for to P++ array

 pwc(0)               =rpar(20) ! coeffs. for plane wave 
 pwc(1)               =rpar(21)
 pwc(2)               =rpar(22)
 pwc(3)               =rpar(23)
 pwc(4)               =rpar(24)
 pwc(5)               =rpar(25)

 if( abs(pwc(0))+abs(pwc(1))+abs(pwc(2)) .eq. 0. )then
   ! sanity check
   stop 12345
 end if

 dxa=dx(0)
 dya=dx(1)
 dza=dx(2)
    
   ! In parallel the dimension may not be the same as the bounds nd1a,nd1b,...
 md1a=dimension(0,0)
 md1b=dimension(1,0)
 md2a=dimension(0,1)
 md2b=dimension(1,1)
 md3a=dimension(0,2)
 md3b=dimension(1,2)

 twoPi=8.*atan2(1.,1.)
 cc= c*sqrt( kx*kx+ky*ky+kz*kz )

 initializeBoundaryForcing(t,slowStartInterval)

 ! ****
 ! write(*,'(" bcOpt: t=",e10.2," fieldOption=",i2," ex,ey,hz=",3i3)') t,fieldOption,ex,ey,hz

 !  write(*,'(" ***bcOpt: slowStartInterval,t=",2f10.4," ssf,ssft,ssftt,sfttt=",4f9.4)') slowStartInterval,t,ssf,ssft,ssftt,ssfttt

 !  --- NOT: extra determines "extra points" in the tangential directions  ----
 !  extra=-1 by default (if adjacent BC>0) no need to do corners -- these are already done
 !  extra=numberOfGhostPoints, if bc==0, (set in begin loop over sides)
 !  extra=0 if bc<0  (set in begin loop over sides)
 extra=-1  
 numberOfGhostPoints=orderOfAccuracy/2


 if( gridType.eq.curvilinear )then
  ! the 4th-order 3d BCs require two steps -- the first step gives initial values at all ghost points
  #If #ORDER == "4" && #DIM == "3"
   beginLoopOverSides(extra,numberOfGhostPoints)
    if( useForcing.eq.0 )then
      bcCurvilinear3dOrder4Step1(none)
    else
      bcCurvilinear3dOrder4Step1(twilightZone)
    end if
   endLoopOverSides()

   ! ok if( .true. ) return ! **********************************************************

   ! In parallel we need to update ghost boundaries after stage 1
   ! **call updateGhostBoundaries(pu)
   call updateGhostAndPeriodic(pu)
  #End
 end if

 ! ok if( .true. ) return ! **********************************************************

 ! ==================================================================================
 beginLoopOverSides(extra,numberOfGhostPoints)

  if( gridType.eq.rectangular )then
    ! ***********************************************
    ! ************rectangular grid*******************
    ! ***********************************************
    
    ! odd symmetry for the normal components
    ! even symmetry for tangential components
    
    ! en1=normal component of E
    ! et1=tangential component 1 of E
    ! et2=tangential component 2 of E
    
    ! hn1=normal component of H
    ! ht1=tangential component 1 of H
    ! ht2=tangential component 2 of H

    ! write(*,'(" bcOpt: called for rectangular side,axis=",2i2)') side,axis

    if( axis.eq.0 )then
      en1=ex
      et1=ey
      et2=ez
      hn1=hx
      ht1=hy
      ht2=hz
    else if( axis.eq.1 )then
      et1=ex
      en1=ey
      et2=ez
      ht1=hx
      hn1=hy
      ht2=hz
    else
      et1=ex
      et2=ey
      en1=ez
      ht1=hx
      ht2=hy
      hn1=hz
    end if

    if( useForcing.eq.0 )then
      bcRectangular(DIM,ORDER,none)
    else
      bcRectangular(DIM,ORDER,twilightZone)
    end if

  else

    ! ***********************************************
    ! ************curvilinear grid*******************
    ! ***********************************************

    ! write(*,'(" bcOpt: called for curvilinear, order=",i2," side,axis=",2i2)') orderOfAccuracy,side,axis

    #If #ORDER == "2" 
      #If #DIM == "2"
        if( useForcing.eq.0 )then
          bcCurvilinear2dOrder2(none)
        else
          ! write(*,'(" bcOpt: called for curvilinear twilightZone, order=",i2," side,axis=",2i2)') orderOfAccuracy,side,axis
          bcCurvilinear2dOrder2(twilightZone)
        end if
      #Else
        if( useForcing.eq.0 )then
          bcCurvilinear3dOrder2(none)
        else
          bcCurvilinear3dOrder2(twilightZone)
        end if
      #End
    #Elif #ORDER == "4"
      #If #DIM == "2"
        if( useForcing.eq.0 )then
          bcCurvilinear2dOrder4(none)
        else
          bcCurvilinear2dOrder4(twilightZone)
        end if
      #Else
        if( useForcing.eq.0 )then
          bcCurvilinear3dOrder4(none)
        !   stop 11122
        else
        ! This next instance does both ??
          bcCurvilinear3dOrder4(twilightZone)
        end if
      #End
    #Else
      stop 9876
    #End



  end if

 endLoopOverSides()
!     **************************************************************************

 return
 end
#endMacro
