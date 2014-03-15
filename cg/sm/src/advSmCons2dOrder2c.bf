! ogf2d, ogf3d, ogDeriv2, etc. are foundin forcing.bC
#beginMacro OGF2D(i1,i2,i3,t,u0,v0)
  call ogf2d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),t,u0,v0)
#endMacro
	
#beginMacro OGF3D(i1,i2,i3,t,u0,v0,w0)
  call ogf3d(ep,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,u0,v0,w0)
#endMacro
	
!       ntd,nxd,nyd,nzd : number of derivatives to evaluate in t,x,y,z
#beginMacro OGDERIV2D(ntd,nxd,nyd,nzd,i1,i2,i3,t,ux,vx)
  call ogDeriv2(epep, ntd,nxd,nyd,nzd, xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t, uc,ux, vc,vx)
#endMacro
	
#beginMacro OGDERIV3D(ntd,nxd,nyd,nzd,i1,i2,i3,t,ux,vx,wx)
  call ogDeriv3(epep, ntd,nxd,nyd,nzd, xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t, uc,ux, vc,vx, wc,wx)
#endMacro
		
! ====================================================================================
! ====================================================================================
#beginMacro DqDr(i,j,im,jm,ip,jp,dc,w,t1,t2,c1,rh)
  Ep=Jac(ip,j)*t1(ip,j)*t2(ip,j)*c1(ip,j)
  Em=Jac(im,j)*t1(im,j)*t2(im,j)*c1(im,j)
  rh(i,j)=rh(i,j)+dc*(Ep*(w(ip,jp)-w(ip,jm))-Em*(w(im,jp)-w(im,jm)))
#endMacro
	
! ====================================================================================
! ====================================================================================
#beginMacro DrDq(i,j,im,jm,ip,jp,dc,w,t1,t2,c1,rh)
  Ep=Jac(i,jp)*t1(i,jp)*t2(i,jp)*c1(i,jp)
  Em=Jac(i,jm)*t1(i,jm)*t2(i,jm)*c1(i,jm)
  rh(i,j)=rh(i,j)+dc*(Ep*(w(ip,jp)-w(im,jp))-Em*(w(ip,jm)-w(im,jm)))
#endMacro
	
! ====================================================================================
! ====================================================================================
#beginMacro Dqq(i,j,w,t1,t2,c1,rh)
  Ep=Jac(ip,j)*t1(ip,j)*t2(ip,j)*c1(ip,j)+Jac(i,j)*t1(i,j)*t2(i,j)*c1(i,j) 
  Em=Jac(im,j)*t1(im,j)*t2(im,j)*c1(im,j)+Jac(i,j)*t1(i,j)*t2(i,j)*c1(i,j)  
  rh(i,j)=rh(i,j)+0.5*dri(0)**2*(Ep*(w(ip,j)-w(i,j))-Em*(w(i,j)-w(im,j)))
#endMacro
	
! ====================================================================================
! ====================================================================================
#beginMacro Drr(i,j,w,t1,t2,c1,rh)
  Ep=Jac(i,jp)*t1(i,jp)*t2(i,jp)*c1(i,jp)+Jac(i,j)*t1(i,j)*t2(i,j)*c1(i,j)
  Em=Jac(i,jm)*t1(i,jm)*t2(i,jm)*c1(i,jm)+Jac(i,j)*t1(i,j)*t2(i,j)*c1(i,j)
  rh(i,j)=rh(i,j)+0.5*dri(1)**2*(Ep*(w(i,jp)-w(i,j))-Em*(w(i,j)-w(i,jm)))
#endMacro


! ====================================================================================
!  Evaluate the RHS to the elasticity equations
! ====================================================================================
#beginMacro ComputeRhside(rho,lam,mu,lam2mu)
  ip=i1+1
  im=i1-1
  jp=i2+1
  jm=i2-1
  ! Dx( (2 mu+lam) ux )
  Dqq(i1,i2,u1,qx,qx,lam2mu,rh1)
  DqDr(i1,i2,im,jm,ip,jp,dc,u1,qx,rx,lam2mu,rh1)
  DrDq(i1,i2,im,jm,ip,jp,dc,u1,rx,qx,lam2mu,rh1)
  Drr(i1,i2,u1,rx,rx,lam2mu,rh1)
  ! write(1,'(" rhs1=",e14.8," (",i2,",",i2,") uxx ")') rh1(i1,i2),i1,i2
  ! Dy( mu uy )
  Dqq(i1,i2,u1,qy,qy,mu,rh1)
  DqDr(i1,i2,im,jm,ip,jp,dc,u1,qy,ry,mu,rh1)
  DrDq(i1,i2,im,jm,ip,jp,dc,u1,ry,qy,mu,rh1)
  Drr(i1,i2,u1,ry,ry,mu,rh1)
  ! write(1,'(" rhs1=",e14.8," (",i2,",",i2,") uyy ")') rh1(i1,i2),i1,i2
  ! Dx( lam v_y )
  Dqq(i1,i2,u2,qx,qy,lam,rh1)
  DqDr(i1,i2,im,jm,ip,jp,dc,u2,qx,ry,lam,rh1)
  DrDq(i1,i2,im,jm,ip,jp,dc,u2,rx,qy,lam,rh1)
  Drr(i1,i2,u2,rx,ry,lam,rh1)
  ! Dy( mu v_x )
  Dqq(i1,i2,u2,qx,qy,mu,rh1)
  DqDr(i1,i2,im,jm,ip,jp,dc,u2,qy,rx,mu,rh1)
  DrDq(i1,i2,im,jm,ip,jp,dc,u2,ry,qx,mu,rh1)
  Drr(i1,i2,u2,rx,ry,mu,rh1)
  ! write(1,'(" rhs1=",e14.8," (",i2,",",i2,") done ")') rh1(i1,i2),i1,i2

  ! Dx( mu vx )
  Dqq(i1,i2,u2,qx,qx,mu,rh2)
  DqDr(i1,i2,im,jm,ip,jp,dc,u2,qx,rx,mu,rh2)
  DrDq(i1,i2,im,jm,ip,jp,dc,u2,rx,qx,mu,rh2)
  Drr(i1,i2,u2,rx,rx,mu,rh2)
  ! write(1,'(" rhs2=",e14.8," (",i2,",",i2,") vxx ")') rh2(i1,i2),i1,i2

  ! Dy( (2 mu + lam) vy )
  Dqq(i1,i2,u2,qy,qy,lam2mu,rh2)
  DqDr(i1,i2,im,jm,ip,jp,dc,u2,qy,ry,lam2mu,rh2)
  DrDq(i1,i2,im,jm,ip,jp,dc,u2,ry,qy,lam2mu,rh2)
  Drr(i1,i2,u2,ry,ry,lam2mu,rh2)
  ! write(1,'(" rhs2=",e14.8," (",i2,",",i2,") vyy ")') rh2(i1,i2),i1,i2

  ! D_x(mu u_y)
  Dqq(i1,i2,u1,qx,qy,mu,rh2)
  ! *wdh* 2011/10/15 x-y should be reversed 
  ! DqDr(i1,i2,im,jm,ip,jp,dc,u1,qy,rx,mu,rh2)
  ! DrDq(i1,i2,im,jm,ip,jp,dc,u1,ry,qx,mu,rh2)
  DqDr(i1,i2,im,jm,ip,jp,dc,u1,qx,ry,mu,rh2)
  DrDq(i1,i2,im,jm,ip,jp,dc,u1,rx,qy,mu,rh2)
  Drr(i1,i2,u1,rx,ry,mu,rh2)
  ! write(1,'(" rhs2=",e14.8," (",i2,",",i2,") (m uy).x ")') rh2(i1,i2),i1,i2

  ! D_y(lam u_x)
  Dqq(i1,i2,u1,qx,qy,lam,rh2)
  ! *wdh* 2011/10/15 x-y should be reversed 
  ! DqDr(i1,i2,im,jm,ip,jp,dc,u1,qx,ry,lam,rh2)
  ! DrDq(i1,i2,im,jm,ip,jp,dc,u1,rx,qy,lam,rh2)
  DqDr(i1,i2,im,jm,ip,jp,dc,u1,qy,rx,lam,rh2)
  DrDq(i1,i2,im,jm,ip,jp,dc,u1,ry,qx,lam,rh2)
  Drr(i1,i2,u1,rx,ry,lam,rh2)
  ! write(1,'(" rhs2=",e14.8," (",i2,",",i2,") done ")') rh2(i1,i2),i1,i2

#endMacro

! =========================================================================
! Compute the RHS only including the cross-derivative terms
! =========================================================================
#beginMacro ComputeRhsideCross(rho,lam,mu,lam2mu)
  ! Dx( (2 mu+lam) ux)
  DqDr(i1,i2,im,jm,ip,jp,dc,u1,qx,rx,lam2mu,rh1)
  DrDq(i1,i2,im,jm,ip,jp,dc,u1,rx,qx,lam2mu,rh1)
  ! Dy( mu uy )
  DqDr(i1,i2,im,jm,ip,jp,dc,u1,qy,ry,mu,rh1)
  DrDq(i1,i2,im,jm,ip,jp,dc,u1,ry,qy,mu,rh1)
  ! Dx( lam v_y )
  DqDr(i1,i2,im,jm,ip,jp,dc,u2,qx,ry,lam,rh1)
  DrDq(i1,i2,im,jm,ip,jp,dc,u2,rx,qy,lam,rh1)
  ! Dy( mu v_x )
  DqDr(i1,i2,im,jm,ip,jp,dc,u2,qy,rx,mu,rh1)
  DrDq(i1,i2,im,jm,ip,jp,dc,u2,ry,qx,mu,rh1)

  ! Dx( mu vx )
  DqDr(i1,i2,im,jm,ip,jp,dc,u2,qx,rx,mu,rh2)
  DrDq(i1,i2,im,jm,ip,jp,dc,u2,rx,qx,mu,rh2)
  ! Dy( (2 mu + lam) vy )
  DqDr(i1,i2,im,jm,ip,jp,dc,u2,qy,ry,lam2mu,rh2)
  DrDq(i1,i2,im,jm,ip,jp,dc,u2,ry,qy,lam2mu,rh2)
  ! Dx( mu uy ) ! *wdh* 2011/10/15 x-y should be reversed 
  DqDr(i1,i2,im,jm,ip,jp,dc,u1,qx,ry,mu,rh2)
  DrDq(i1,i2,im,jm,ip,jp,dc,u1,rx,qy,mu,rh2)
  ! Dy( lam ux ) ! *wdh* 2011/10/15 x-y should be reversed 
  DqDr(i1,i2,im,jm,ip,jp,dc,u1,qy,rx,lam,rh2)
  DrDq(i1,i2,im,jm,ip,jp,dc,u1,ry,qx,lam,rh2)
#endMacro

! =========================================================================
! Compute the RHS excluding the cross-derivative terms
! =========================================================================
#beginMacro ComputeRhsideNonCross(rho,lam,mu,lam2mu)
  ip=i1+1
  im=i1-1
  jp=i2+1
  jm=i2-1
  !  Dx( (2 mu+lam) ux)
  Dqq(i1,i2,u1,qx,qx,lam2mu,rh1)
  Drr(i1,i2,u1,rx,rx,lam2mu,rh1)
  ! Dy( mu uy )
  Dqq(i1,i2,u1,qy,qy,mu,rh1)
  Drr(i1,i2,u1,ry,ry,mu,rh1)
  ! Dx( lam v_y )
  Dqq(i1,i2,u2,qx,qy,lam,rh1)
  Drr(i1,i2,u2,rx,ry,lam,rh1)
  ! Dy( mu v_x ) ! *wdh* 2011/10/15 x-y should be reversed 
  Dqq(i1,i2,u2,qy,qx,mu,rh1)
  Drr(i1,i2,u2,ry,rx,mu,rh1)

  ! Dx( mu vx )
  Dqq(i1,i2,u2,qx,qx,mu,rh2)
  Drr(i1,i2,u2,rx,rx,mu,rh2)
  ! Dy( (2 mu + lam) vy )
  Dqq(i1,i2,u2,qy,qy,lam2mu,rh2)
  Drr(i1,i2,u2,ry,ry,lam2mu,rh2)
  ! Dx( mu uy ) 
  Dqq(i1,i2,u1,qx,qy,mu,rh2)
  Drr(i1,i2,u1,rx,ry,mu,rh2)
  ! Dy( lam ux )   ! *wdh* 2011/10/15 x-y should be reversed 
  Dqq(i1,i2,u1,qy,qx,lam,rh2)
  Drr(i1,i2,u1,ry,rx,lam,rh2)
#endMacro


#beginMacro beginLoops()
do i2=n2a,n2b
do i1=n1a,n1b
#endMacro

#beginMacro endLoops()
end do
end do
#endMacro

! ===========================================================================
! Advance the solution and update boundaries
! ===========================================================================
#beginMacro advanceSolutionMacro(rho,lam,mu,lam2mu)

  ! -- evaluate the interior equations at all points ---
  !  -- the cross terms on boundaries are fixed up afterward --

  ! do the cross terms everywhere and overwrite if necessary
  dc=0.25*dri(0)*dri(1)
  beginLoops() 
    rh1(i1,i2)=0.d0
    rh2(i1,i2)=0.d0      
    ComputeRhside(rho,lam,mu,lam2mu)
  endLoops()
	
  ! We correct the sides
  dc=0.5*dri(0)*dri(1)
  if( bc(0,0).eq.stressFree)then
     do i2=n2a,n2b
        do i1=n1a,n1a
           rh1(i1,i2)=0.d0
           rh2(i1,i2)=0.d0      
           ComputeRhsideNonCross(rho,lam,mu,lam2mu)
           ip=i1+1
           im=i1	
           jp=i2+1	
           jm=i2-1
           ComputeRhsideCross(rho,lam,mu,lam2mu)
        enddo
     enddo
  endif
  if( bc(1,0).eq.stressFree)then
     do i2=n2a,n2b
        do i1=n1b,n1b
           rh1(i1,i2)=0.d0
           rh2(i1,i2)=0.d0      
           ComputeRhsideNonCross(rho,lam,mu,lam2mu)
           ip=i1
           im=i1-1
           jp=i2+1	
           jm=i2-1
           ComputeRhsideCross(rho,lam,mu,lam2mu)
        enddo
     enddo
  endif
  if( bc(0,1).eq.stressFree)then
     do i2=n2a,n2a
        do i1=n1a,n1b
           rh1(i1,i2)=0.d0
           rh2(i1,i2)=0.d0      
           ComputeRhsideNonCross(rho,lam,mu,lam2mu)
           ip=i1+1
           im=i1-1	
           jp=i2+1
           jm=i2	
           ComputeRhsideCross(rho,lam,mu,lam2mu)
        enddo
     enddo
  endif
  if( bc(1,1).eq.stressFree)then
     do i2=n2b,n2b
        do i1=n1a,n1b
           rh1(i1,i2)=0.d0
           rh2(i1,i2)=0.d0      
           ComputeRhsideNonCross(rho,lam,mu,lam2mu)
           ip=i1+1
           im=i1-1
           jp=i2
           jm=i2-1
           ComputeRhsideCross(rho,lam,mu,lam2mu)
        enddo
     enddo
  endif
  ! Finally we correct the corners
  dc=dri(0)*dri(1)
  if((bc(0,0).eq.stressFree).and.(bc(0,1).eq.stressFree))then
     do i2=n2a,n2a
        do i1=n1a,n1a
           rh1(i1,i2)=0.d0
           rh2(i1,i2)=0.d0      
           ComputeRhsideNonCross(rho,lam,mu,lam2mu)
           ip=i1+1	
           im=i1
           jp=i2+1
           jm=i2
           ComputeRhsideCross(rho,lam,mu,lam2mu)
        enddo
     enddo
  endif
  if((bc(0,0).eq.stressFree).and.(bc(1,1).eq.stressFree))then
     do i2=n2b,n2b
        do i1=n1a,n1a
           rh1(i1,i2)=0.d0
           rh2(i1,i2)=0.d0      
           ComputeRhsideNonCross(rho,lam,mu,lam2mu)
           ip=i1+1
           im=i1
           jp=i2
           jm=i2-1
           ComputeRhsideCross(rho,lam,mu,lam2mu)
        enddo
     enddo
  endif
  if((bc(1,0).eq.stressFree).and.(bc(0,1).eq.stressFree))then
     do i2=n2a,n2a
        do i1=n1b,n1b
           rh1(i1,i2)=0.d0
           rh2(i1,i2)=0.d0      
           ComputeRhsideNonCross(rho,lam,mu,lam2mu)
           ip=i1
           im=i1-1
           jp=i2+1
           jm=i2
           ComputeRhsideCross(rho,lam,mu,lam2mu)
        enddo
     enddo
  endif
  if((bc(1,1).eq.stressFree).and.(bc(1,0).eq.stressFree))then
     do i2=n2b,n2b
        do i1=n1b,n1b
           rh1(i1,i2)=0.d0
           rh2(i1,i2)=0.d0      
           ComputeRhsideNonCross(rho,lam,mu,lam2mu)
           ip=i1
           im=i1-1
           jp=i2
           jm=i2-1
           ComputeRhsideCross(rho,lam,mu,lam2mu)
        enddo
     enddo
  endif
  
  !    Assign next time level            
  beginLoops() 
    dtsqOverRhoJac = dtsq/(rho(i1,i2)*Jac(i1,i2))
    un(i1,i2,nd3a,uc)=cu*u(i1,i2,nd3a,uc)+cum*um(i1,i2,nd3a,uc)+rh1(i1,i2)*dtsqOverRhoJac
    un(i1,i2,nd3a,vc)=cu*u(i1,i2,nd3a,vc)+cum*um(i1,i2,nd3a,vc)+rh2(i1,i2)*dtsqOverRhoJac
  endLoops()

#endMacro
! --- end advanceSolutionMacro



	subroutine advSmCons2dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     &     nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,xy, um,u,un,f, 
     &     ndMatProp,matIndex,matValpc,matVal,bc, dis, 
     &     varDis, ipar, rpar, ierr )
!======================================================================
!     Advance a time step for the equations of Solid Mechanics (linear elasticity for now)
!     
!     nd : number of space dimensions
!     
!     ipar(0)  = option : option=0 - Elasticity+Artificial diffusion
!     =1 - AD only
!     
!     dis(i1,i2,i3) : temp space to hold artificial dissipation
!     varDis(i1,i2,i3) : coefficient of the variable artificial dissipation
!======================================================================
	implicit none
	integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,
     &     nd3b,nd4a,nd4b
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

      ! -- Declare arrays for variable material properties --
      include 'declareVarMatProp.h'

c     ---- local variables -----
      integer c,i1,i2,i3,n,gridType,orderOfAccuracy,orderInTime
      integer addForcing,orderOfDissipation,option
      integer useWhereMask,useWhereMaskSave,grid,myid,
     &     useVariableDissipation,timeSteppingMethod
      integer useConservative,combineDissipationWithAdvance
      integer uc,vc,wc
      real dt,dx(0:3),adc,dr(0:3),c1,c2,kx,ky,kz,t,dtsqOverRhoJac
      real qx,qy,qz,rx,ry,rz,sx,sy,sz
      real rho,lam2mu,lam,mu,uxy0,uy0,vxy0,vy0,epep,rho0,mu0,lambda0
      integer ipp,ip,im,jp,jm,i
      real Dup, Dum, Dvp, Dvm, Ep, Em ,dcons,dc,Jac,u1,u2
      real rh1(nd1a:nd1b,nd2a:nd2b)
      real rh2(nd1a:nd1b,nd2a:nd2b)
      real dri(0:3)
      real dtsq,errmaxu,errtmpu,exsolu,errmaxv,errtmpv,exsolv
      real du,fd22d,fd42d,adcdt,energy,weight,etmp
      real dtOld,cu,cum
      integer computeUt
      integer dirichlet,stressFree,debug
      parameter( dirichlet=1,stressFree=2 )
      
      ! -- begin statement functions
      real rhopc,mupc,lambdapc,lam2mupc, rhov,muv,lambdav,lam2muv
      ! (rho,mu,lambda) for materialFormat=piecewiseConstantMaterialProperties
      rhopc(i1,i2)    = matValpc( 0, matIndex(i1,i2))
      mupc(i1,i2)     = matValpc( 1, matIndex(i1,i2))
      lambdapc(i1,i2) = matValpc( 2, matIndex(i1,i2))
      lam2mupc(i1,i2) = (matValpc( 2, matIndex(i1,i2))+2.0*matValpc( 1, matIndex(i1,i2)))

      ! (rho,mu,lambda) for materialFormat=variableMaterialProperties
      rhov(i1,i2)    = matVal(i1,i2,0)
      muv(i1,i2)     = matVal(i1,i2,1)
      lambdav(i1,i2) = matVal(i1,i2,2)
      lam2muv(i1,i2) = (matVal(i1,i2,2)+2.0*matVal(i1,i2,1))

      ! lam(i1,i2)=(c1-c2)
      ! mu(i1,i2)=c2
      ! lam2mu(i1,i2)=(lam(i1,i2)+2.0*mu(i1,12))

      rho(i1,i2)=rho0
      lam(i1,i2)=lambda0
      mu(i1,i2)=mu0
      lam2mu(i1,i2)=(lambda0+2.0*mu0)

      qx(i1,i2)=rsxy(i1,i2,nd3a,0,0)
      qy(i1,i2)=rsxy(i1,i2,nd3a,0,1)
      rx(i1,i2)=rsxy(i1,i2,nd3a,1,0)
      ry(i1,i2)=rsxy(i1,i2,nd3a,1,1)
      Jac(i1,i2)=1.d0/(qx(i1,i2)*ry(i1,i2)-rx(i1,i2)*qy(i1,i2))
!      lam(i1,i2)=(c1-c2)
!      mu(i1,i2)=c2
!      lam2mu(i1,i2)=(lam(i1,i2)+2.0*mu(i1,12))
      u1(i1,i2)=u(i1,i2,0,uc)   !!???
      u2(i1,i2)=u(i1,i2,0,vc)   !!??? 
c ******* artificial dissipation ******
      du(i1,i2,i3,c)=u(i1,i2,i3,c)-um(i1,i2,i3,c)
c      (2nd difference)
      fd22d(i1,i2,i3,c)=(( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) )-4.*du(i1,i2,i3,c))
c     -(fourth difference)
      fd42d(i1,i2,i3,c)=(-( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c) )+4.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c) )-12.*du(i1,i2,i3,c) )
      ! -- end statement functions
	
      dt    =rpar(0)
      dx(0) =rpar(1)
      dx(1) =rpar(2)
      dx(2) =rpar(3)
      adc   =rpar(4)		! coefficient of artificial dissipation
      dr(0) =rpar(5)
      dr(1) =rpar(6)
      dr(2) =rpar(7)
      c1    =rpar(8)
      c2    =rpar(9) 
      kx    =rpar(10) 
      ky    =rpar(11) 
      kz    =rpar(12) 
      epep  =rpar(13)
      t     =rpar(14)
      dtOld =rpar(15) ! dt used on the previous time step 
      rho0  =rpar(16)  ! for constant coefficients 
      mu0   =rpar(17)
      lambda0=rpar(18)  
      
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
      materialFormat     =ipar(16) 
      myid               =ipar(17)

      if( t.lt.dt )then
         write(*,'(" ***advSmCons2dOrder2c:INFO: materialFormat=",i2," (0=const, 1=pc, 2=var)")') materialFormat
         write(*,'(" ***advSmCons2dOrder2c:INFO: rho0,mu0,lambda0=",3f5.2)') rho0,mu0,lambda0
      end if

      ! --- Output rho, mu and lambda at t=0 for testing ---
      if( materialFormat.ne.0 .and. t.le.0 .and. (nd1b-nd1a)*(nd2b-nd2a).lt. 10000 )then

       write(*,'("advSmCons2dOrder2c: rho:")')
       do i2=nd2b,nd2a,-1
         if( materialFormat.eq.piecewiseConstantMaterialProperties )then
          write(*,9000) (rhopc(i1,i2),i1=nd1a,nd1b)
         else
          write(*,9000) (rhov(i1,i2),i1=nd1a,nd1b)
         end if
       end do 
       write(*,'("smgvc2d: mu:")')
       do i2=nd2b,nd2a,-1
         if( materialFormat.eq.piecewiseConstantMaterialProperties )then
          write(*,9000) (mupc(i1,i2),i1=nd1a,nd1b)
         else
          write(*,9000) (muv(i1,i2),i1=nd1a,nd1b)
         end if
       end do 
       write(*,'("smgvc2d: lambda:")')
       do i2=nd2b,nd2a,-1
         if( materialFormat.eq.piecewiseConstantMaterialProperties )then
          write(*,9000) (lambdapc(i1,i2),i1=nd1a,nd1b)
         else
          write(*,9000) (lambdav(i1,i2),i1=nd1a,nd1b)
         end if
       end do 
 9000  format(100(f5.1))

      end if


      dtsq=dt*dt
      do i=0,2	
        dri(i)=1.0/dr(i)
      end do

      ! *wdh* 100201 -- fixes for variable time step : locally 2nd order --
      cu=  2.     ! coeff. of u(t) in the time-step formula
      cum=-1.     ! coeff. of u(t-dtOld)
      if( dtOld.le.0 )then
         write(*,'(" advSmCons:ERROR : dtOld<=0 ")')
         stop 8167
      end if
      if( dt.ne.dtOld )then
         write(*,'(" advSmCons:INFO: dt=",e12.4," <> dtOld=",e12.4," diff=",e9.2)') dt,dtOld,dt-dtOld
         ! adjust the coefficients for a variable time step : this is locally second order accurate
         cu= 1.+dt/dtOld     ! coeff. of u(t) in the time-step formula
         cum=-dt/dtOld       ! coeff. of u(t-dtOld)
         dtsq=dt*(dt+dtOld)*.5
      end if
      ! for variable time step: ( *wdh* 100203 )
      if( computeUt.eq.0 )then
         adcdt = adc*(dt*(dt+dtOld)/2.)/dtOld  
      else
         adcdt= adc/dtOld                    
         write(*,*) 'ERROR: finish me'
         stop 12345
      end if

      !        write(*,'(" advSmCons2dOrder2c t=",e10.2)') t

      if(debug.gt.2) then
         errmaxu=0.0
         errmaxv=0.0
      endif

      if( materialFormat.eq.constantMaterialProperties ) then 
        ! --- constant material properties ---
        advanceSolutionMacro(rho,lam,mu,lam2mu)

      else if( materialFormat.eq.piecewiseConstantMaterialProperties ) then 

        ! --- piecewise constant material properties ---
        advanceSolutionMacro(rhopc,lambdapc,mupc,lam2mupc)

      else if( materialFormat.eq.variableMaterialProperties ) then 

        ! --- variable material properties ---
        advanceSolutionMacro(rhov,lambdav,muv,lam2muv)

      else
         stop 6677
      end if


c     Add on forcing
      if(addForcing.ne.0) then
      beginLoops() 
        un(i1,i2,nd3a,uc)=un(i1,i2,nd3a,uc)+dtsq*f(i1,i2,nd3a,uc)
        un(i1,i2,nd3a,vc)=un(i1,i2,nd3a,vc)+dtsq*f(i1,i2,nd3a,vc)
      endLoops()       
	end if
      if( (orderOfDissipation.eq.4 ).and.(adc.gt.0))then
      ! *wdh* 100203 adcdt=adc*dt
      beginLoops() 
        un(i1,i2,nd3a,uc)=un(i1,i2,nd3a,uc)+adcdt*fd42d(i1,i2,nd3a,uc)
        un(i1,i2,nd3a,vc)=un(i1,i2,nd3a,vc)+adcdt*fd42d(i1,i2,nd3a,vc),\
      endLoops()       
      end if
      if( (orderOfDissipation.eq.2 ).and.(adc.gt.0))then
      ! *wdh* 100203 adcdt=adc*dt
      beginLoops() 
        un(i1,i2,nd3a,uc)=un(i1,i2,nd3a,uc)+adcdt*fd22d(i1,i2,nd3a,uc)
        un(i1,i2,nd3a,vc)=un(i1,i2,nd3a,vc)+adcdt*fd22d(i1,i2,nd3a,vc),\
      endLoops()       
      end if

      if(debug.eq.3) then	
      energy=0.d0
      ! DEAA ENERGY
      beginLoops() 
      	weight=1.d0
	if ((i1.eq.n1a).and.((bc(0,0).eq.stressFree))) weight=weight*0.5d0
	if ((i1.eq.n1b).and.((bc(0,1).eq.stressFree))) weight=weight*0.5d0
	if ((i2.eq.n2a).and.((bc(1,0).eq.stressFree))) weight=weight*0.5d0
	if ((i2.eq.n2b).and.((bc(1,1).eq.stressFree))) weight=weight*0.5d0
	energy=energy-weight*un(i1,i2,nd3a,uc)*rh1(i1,i2)*Jac(i1,i2)
	energy=energy-weight*un(i1,i2,nd3a,vc)*rh2(i1,i2)*Jac(i1,i2)
	!       we use f to store u_t
        rh1(i1,i2)=(un(i1,i2,nd3a,uc)-u(i1,i2,nd3a,uc))/dt
        rh2(i1,i2)=(un(i1,i2,nd3a,vc)-u(i1,i2,nd3a,vc))/dt
	energy=energy+weight*rh1(i1,i2)*rh1(i1,i2)*Jac(i1,i2)
	energy=energy+weight*rh2(i1,i2)*rh2(i1,i2)*Jac(i1,i2)
      endLoops() 
      write(*,*) "Discrete energy  ",energy
      end if
      end 

