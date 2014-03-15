#beginMacro setupMaterial()
l2mip=lam2mu(ip,j,k)
l2m0=lam2mu(i,j,k)
l2mim=lam2mu(im,j,k)
lip=lam(ip,j,k)
l0=lam(i,j,k)
lim=lam(im,j,k)
mip=mu(ip,j,k)
m0=mu(i,j,k)
mim=mu(im,j,k)
l2mjp=lam2mu(i,jp,k)
l2mjm=lam2mu(i,jm,k)
ljp=lam(i,jp,k)
ljm=lam(i,jm,k)
mjp=mu(i,jp,k)
mjm=mu(i,jm,k)
l2mkp=lam2mu(i,j,kp)
l2mkm=lam2mu(i,j,km)
lkp=lam(i,j,kp)
lkm=lam(i,j,km)
mkp=mu(i,j,kp)
mkm=mu(i,j,km)
#endMacro

#beginMacro setupU()
u0=u(i,j,k,uc)
uipjp=u(ip,jp,k,uc)
uipjm=u(ip,jm,k,uc)
uimjp=u(im,jp,k,uc)
uimjm=u(im,jm,k,uc)
uipkp=u(ip,j,kp,uc)
uipkm=u(ip,j,km,uc)
uimkp=u(im,j,kp,uc)
uimkm=u(im,j,km,uc)
ujpkp=u(i,jp,kp,uc)
ujpkm=u(i,jp,km,uc)
ujmkp=u(i,jm,kp,uc)
ujmkm=u(i,jm,km,uc)
duip=dri(0)*(u(ip,j,k,uc)-u0)
duim=dri(0)*(u0-u(im,j,k,uc))
dujp=dri(1)*(u(i,jp,k,uc)-u0)
dujm=dri(1)*(u0-u(i,jm,k,uc))
dukp=dri(2)*(u(i,j,kp,uc)-u0)
dukm=dri(2)*(u0-u(i,j,km,uc))
#endMacro

#beginMacro setupV()
u0=u(i,j,k,vc)
uipjp=u(ip,jp,k,vc)
uipjm=u(ip,jm,k,vc)
uimjp=u(im,jp,k,vc)
uimjm=u(im,jm,k,vc)
uipkp=u(ip,j,kp,vc)
uipkm=u(ip,j,km,vc)
uimkp=u(im,j,kp,vc)
uimkm=u(im,j,km,vc)
ujpkp=u(i,jp,kp,vc)
ujpkm=u(i,jp,km,vc)
ujmkp=u(i,jm,kp,vc)
ujmkm=u(i,jm,km,vc)
duip=dri(0)*(u(ip,j,k,vc)-u0)
duim=dri(0)*(u0-u(im,j,k,vc))
dujp=dri(1)*(u(i,jp,k,vc)-u0)
dujm=dri(1)*(u0-u(i,jm,k,vc))
dukp=dri(2)*(u(i,j,kp,vc)-u0)
dukm=dri(2)*(u0-u(i,j,km,vc))
#endMacro

#beginMacro setupW()
u0=u(i,j,k,wc) 
uipjp=u(ip,jp,k,wc)
uipjm=u(ip,jm,k,wc)
uimjp=u(im,jp,k,wc)
uimjm=u(im,jm,k,wc)
uipkp=u(ip,j,kp,wc)
uipkm=u(ip,j,km,wc)
uimkp=u(im,j,kp,wc)
uimkm=u(im,j,km,wc)
ujpkp=u(i,jp,kp,wc)
ujpkm=u(i,jp,km,wc)
ujmkp=u(i,jm,kp,wc)
ujmkm=u(i,jm,km,wc)
duip=dri(0)*(u(ip,j,k,wc)-u0)
duim=dri(0)*(u0-u(im,j,k,wc))
dujp=dri(1)*(u(i,jp,k,wc)-u0)
dujm=dri(1)*(u0-u(i,jm,k,wc))
dukp=dri(2)*(u(i,j,kp,wc)-u0)
dukm=dri(2)*(u0-u(i,j,km,wc))
#endMacro

#beginMacro addUCrossterms()
!// u terms in v eq. 
rhtmpv = rhtmpv + fq*fr*(mip*(uipjp-uipjm)-mim*(uimjp-uimjm))
rhtmpv = rhtmpv + fq*fr*(ljp*(uipjp-uimjp)-ljm*(uipjm-uimjm))
!// u terms in w eq.
rhtmpw = rhtmpw + fq*fs*(mip*(uipkp-uipkm)-mim*(uimkp-uimkm))
rhtmpw = rhtmpw + fq*fs*(lkp*(uipkp-uimkp)-lkm*(uipkm-uimkm))	
#endMacro

#beginMacro addUterms()
! u terms in u eq.
rhtmpu = rhtmpu +  0.5*dri(0)*(l2mip*duip-l2mim*duim+l2m0*(duip-duim))
rhtmpu = rhtmpu +  0.5*dri(1)*(mjp*dujp-mjm*dujm+m0*(dujp-dujm))
rhtmpu = rhtmpu +  0.5*dri(2)*(mkp*dukp-mkm*dukm+m0*(dukp-dukm))
#endMacro	

#beginMacro addVCrossterms()
!// v terms in u eq.
rhtmpu = rhtmpu + fq*fr*(lip*(uipjp-uipjm)-lim*(uimjp-uimjm))
rhtmpu = rhtmpu + fq*fr*(mjp*(uipjp-uimjp)-mjm*(uipjm-uimjm))
!// v terms in w eq.
rhtmpw = rhtmpw + fr*fs*(mjp*(ujpkp-ujpkm)-mjm*(ujmkp-ujmkm))
rhtmpw = rhtmpw + fr*fs*(lkp*(ujpkp-ujmkp)-lkm*(ujpkm-ujmkm))
#endMacro	

#beginMacro addVterms()
! v terms in v eq. 
rhtmpv = rhtmpv +  0.5*dri(0)*(mip*duip-mim*duim+m0*(duip-duim))
rhtmpv = rhtmpv +  0.5*dri(1)*(l2mjp*dujp-l2mjm*dujm+l2m0*(dujp-dujm))
rhtmpv = rhtmpv +  0.5*dri(2)*(mkp*dukp-mkm*dukm+m0*(dukp-dukm))
#endMacro

#beginMacro addWCrossterms()
!// w terms in u eq.
rhtmpu = rhtmpu + fq*fs*(lip*(uipkp-uipkm)-lim*(uimkp-uimkm))
rhtmpu = rhtmpu + fq*fs*(mip*(uipkp-uipkm)-mim*(uimkp-uimkm))
!// w terms in v eq.
rhtmpv = rhtmpv + fr*fs*(ljp*(ujpkp-ujpkm)-ljm*(ujmkp-ujmkm))
rhtmpv = rhtmpv + fr*fs*(mkp*(ujpkp-ujmkp)-mkm*(ujpkm-ujmkm))
#endMacro

#beginMacro addWterms()
! w terms in w eq.
rhtmpw = rhtmpw +  0.5*dri(0)*(mip*duip-mim*duim+m0*(duip-duim))
rhtmpw = rhtmpw +  0.5*dri(1)*(mjp*dujp-mjm*dujm+m0*(dujp-dujm))
rhtmpw = rhtmpw +  0.5*dri(2)*(l2mkp*dukp-l2mkm*dukm+l2m0*(dukp-dukm))
#endMacro

#beginMacro beginLoops()
do k=n3a,n3b
do j=n2a,n2b
do i=n1a,n1b
#endMacro

#beginMacro beginLoopsD()
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
#endMacro

#beginMacro endLoops()
end do
end do
end do
#endMacro

#beginMacro correctCorners(nn1a,nn1b,nn2a,nn2b,nn3a,nn3b,imm,ipp,jmm,jpp,kmm,kpp,fqq,frr,fss)
do k=nn3a,nn3b 
do j=nn2a,nn2b
do i=nn1a,nn1b
rhtmpu=0.
rhtmpv=0.
rhtmpw=0.
im=i-imm
ip=i+ipp
jm=j-jmm
jp=j+jpp
km=k-kmm
kp=k+kpp
fq=fqq*dri(0)
fr=frr*dri(1)
fs=fss*dri(2)
setupMaterial()
setupU()
addUCrossterms()	
setupV()
addVCrossterms()	
setupW()
addWCrossterms()
im=i-1
ip=i+1
jm=j-1
jp=j+1
km=k-1
kp=k+1
fq=0.5*dri(0)
fr=0.5*dri(1)
fs=0.5*dri(2)
setupMaterial()
setupU()
addUterms()	
setupV()
addVterms()	
setupW()
addWterms()
if(addForcing.ne.0) then
un(i,j,k,uc)=cu*u(i,j,k,uc)+cum*um(i,j,k,uc)+dtsq*(rhtmpu+f(i,j,k,uc))
un(i,j,k,vc)=cu*u(i,j,k,vc)+cum*um(i,j,k,vc)+dtsq*(rhtmpv+f(i,j,k,vc))
un(i,j,k,wc)=cu*u(i,j,k,wc)+cum*um(i,j,k,wc)+dtsq*(rhtmpw+f(i,j,k,wc))
else
un(i,j,k,uc)=cu*u(i,j,k,uc)+cum*um(i,j,k,uc)+dtsq*rhtmpu
un(i,j,k,vc)=cu*u(i,j,k,vc)+cum*um(i,j,k,vc)+dtsq*rhtmpv
un(i,j,k,wc)=cu*u(i,j,k,wc)+cum*um(i,j,k,wc)+dtsq*rhtmpw
endif
end do
end do
end do
#endMacro

c     
c     Advance the equations of solid mechanics
c       
      subroutine advSmCons3dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     &nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,xy, um,u,un,f, 
     & ndMatProp,matIndex,matValpc,matVal, bc, dis, 
     &varDis, ipar, rpar, ierr )
c======================================================================
c     Advance a time step for the equations of Solid Mechanics (linear elasticity for now)
c     
c     nd : number of space dimensions
c     
c     ipar(0)  = option : option=0 - Elasticity+Artificial diffusion
c     =1 - AD only
c     
c     dis(i1,i2,i3) : temp space to hold artificial dissipation
c     varDis(i1,i2,i3) : coefficient of the variable artificial dissipation
c======================================================================
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
      integer useWhereMask,useWhereMaskSave,grid,
     &     useVariableDissipation,timeSteppingMethod
      integer useConservative,combineDissipationWithAdvance
      integer uc,vc,wc,s1,s2,s3,a1,a2,a3,nc1a,nc1b,nc2a,nc2b,nc3a,nc3b
      integer i,j,k,im,ip,jm,jp,km,kp,icm,icp,jcm,jcp,kcm,kcp
      real dt,dx(0:3),adc,dr(0:3),c1,c2,kx,ky,kz,t,fcq,fcr,fcs
      real qx,qy,qz,rx,ry,rz,sx,sy,sz
      real lam2mu,lam,mu,uxy0,uy0,vxy0,vy0,epep
      real Dup, Dum, Dvp, Dvm, Ep, Em ,dcons,dc,u1,u2,u3
      real rh1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rh2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rh3(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real dri(0:3)
      real dtsq,errmaxu,errtmpu,exsolu,errmaxv,errtmpv,exsolv
      real u0,v0,w0,uipjp,uipjm,uimjp,uimjm
      real uipkp,uipkm,uimkp,uimkm
      real ujpkp,ujpkm,ujmkp,ujmkm
      real duip,duim,dujm,dujp,dukm,dukp
      real lip,l0,lim,mip,mim,m0,l2mip,l2mim,l2m0
      real ljp,ljm,mjp,mjm,l2mjp,l2mjm
      real lkp,lkm,mkp,mkm,l2mkp,l2mkm
      real rhtmpu,rhtmpv,rhtmpw,fr,fq,fs,fqq,frr,fss
      real weight,energy
      integer dirichlet,stressFree,debug,version,constmaterial,myid
      parameter( dirichlet=1,stressFree=2 )
      real du,fd23d,fd43d,adcdt,lamcons,mucons,lam2mucons,mulamcons
      real fxxl2m,fyym,fzzm,fxyml,fxzml,fxxm,fyyl2m,fyzml,fzzl2m
      real dtOld,cu,cum
      integer computeUt
!     Bills stuff 
      integer kd
      real c1dtsq,c2dtsq,ulaplacian23r,h12,h22,sm23ru,sm23rv,sm23rw
      real ux23r
      real uy23r
      real uz23r
      real uxx23r
      real uyy23r
      real uxy23r
      real uzz23r
      real uxz23r
      real uyz23r
      
!     material matter
      lam(i1,i2,i3)=(c1-c2)
      mu(i1,i2,i3)=c2
      lam2mu(i1,i2,i3)=(lam(i1,i2,i3)+2.0*mu(i1,12,i3))
      
!     --- 3D --- Bills macros
!      sm23ru(i1,i2,i3)=cu*u(i1,i2,i3,uc)+cum*um(i1,i2,i3,uc)+c2dtsq*(ulaplacian23r(i1,i2,i3,uc) )+c1dtsq*( uxx23r(i1,i2,i3,uc) + uxy23r(i1,i2,i3,vc)+ uxz23r(i1,i2,i3,wc) )
!      sm23rv(i1,i2,i3)=cu*u(i1,i2,i3,vc)+cum*um(i1,i2,i3,vc)+c2dtsq*(ulaplacian23r(i1,i2,i3,vc) )+c1dtsq*( uxy23r(i1,i2,i3,uc) + uyy23r(i1,i2,i3,vc)+ uyz23r(i1,i2,i3,wc) )
!      sm23rw(i1,i2,i3)=cu*u(i1,i2,i3,wc)+cum*um(i1,i2,i3,wc)+c2dtsq*(ulaplacian23r(i1,i2,i3,wc) )+c1dtsq*( uxz23r(i1,i2,i3,uc) + uyz23r(i1,i2,i3,vc)+ uzz23r(i1,i2,i3,wc) )

!     --- 3D --- Bills macros

      h12(kd) = 1./(2.*dx(kd))
      h22(kd) = 1./(dx(kd)**2)
      ux23r(i1,i2,i3,kd)=(u(i1+1,i2,i3,kd)-u(i1-1,i2,i3,kd))*h12(0)
      uy23r(i1,i2,i3,kd)=(u(i1,i2+1,i3,kd)-u(i1,i2-1,i3,kd))*h12(1)
      uz23r(i1,i2,i3,kd)=(u(i1,i2,i3+1,kd)-u(i1,i2,i3-1,kd))*h12(2)
      uxx23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd)) )*h22(0)
      uyy23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2+1,i3,kd)+u(i1,i2-1,i3,kd)) )*h22(1)
      uxy23r(i1,i2,i3,kd)=(ux23r(i1,i2+1,i3,kd)-ux23r(i1,i2-1,i3,kd))*h12(1)
      uzz23r(i1,i2,i3,kd)=(-2.*u(i1,i2,i3,kd)+(u(i1,i2,i3+1,kd)+u(i1,i2,i3-1,kd)) )*h22(2)
      uxz23r(i1,i2,i3,kd)=(ux23r(i1,i2,i3+1,kd)-ux23r(i1,i2,i3-1,kd))*h12(2)
      uyz23r(i1,i2,i3,kd)=(uy23r(i1,i2,i3+1,kd)-uy23r(i1,i2,i3-1,kd))*h12(2)
      
      ! *wdh* 091201 -- reordered statement functions for pgf compiler : ulaplacian23r must appear after ux23r etc.
      ulaplacian23r(i1,i2,i3,kd)=uxx23r(i1,i2,i3,kd)+uyy23r(i1,i2,i3,kd)+uzz23r(i1,i2,i3,kd)

      sm23ru(i1,i2,i3)=c2*(ulaplacian23r(i1,i2,i3,uc))+c1*(uxx23r(i1,i2,i3,uc) + uxy23r(i1,i2,i3,vc)+ uxz23r(i1,i2,i3,wc))
      sm23rv(i1,i2,i3)=c2*(ulaplacian23r(i1,i2,i3,vc))+c1*(uxy23r(i1,i2,i3,uc) + uyy23r(i1,i2,i3,vc)+ uyz23r(i1,i2,i3,wc))
      sm23rw(i1,i2,i3)=c2*(ulaplacian23r(i1,i2,i3,wc))+c1*(uxz23r(i1,i2,i3,uc) + uyz23r(i1,i2,i3,vc)+ uzz23r(i1,i2,i3,wc))


c     ******* artificial dissipation ******
      du(i1,i2,i3,c)=u(i1,i2,i3,c)-um(i1,i2,i3,c)
      fd23d(i1,i2,i3,c)=\
      (     ( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) \
      -6.*du(i1,i2,i3,c) )
      
c     -(fourth difference)
      fd43d(i1,i2,i3,c)=\
      (    -( du(i1-2,i2,i3,c)+du(i1+2,i2,i3,c)+du(i1,i2-2,i3,c)+du(i1,i2+2,i3,c)+du(i1,i2,i3-2,c)+du(i1,i2,i3+2,c) ) \
      +4.*( du(i1-1,i2,i3,c)+du(i1+1,i2,i3,c)+du(i1,i2-1,i3,c)+du(i1,i2+1,i3,c)+du(i1,i2,i3-1,c)+du(i1,i2,i3+1,c) ) \
      -18.*du(i1,i2,i3,c) )
      
      dt    =rpar(0)
      dx(0) =rpar(1)
      dx(1) =rpar(2)
      dx(2) =rpar(3)
      adc   =rpar(4)            ! coefficient of artificial dissipation
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
 
      if( materialFormat.eq.constantMaterialProperties )then
        constmaterial=1 ! 1 means const. material parameters
      else
        constmaterial=0
      end if

      if( materialFormat.ne.constantMaterialProperties )then
        write(*,'(" ***advSmCons3dOrder2r:ERROR: Finish me for variable material")')
        stop 11122
      end if

      if( constmaterial.eq.1 )then
        ! version < 4 is variable
        ! version 1 : from sos-nc 
        ! version 2 : 
        ! version 3 : 
        version = 2
      else
        version = 4 ! variable material
      end if
      
      if( t.lt.dt )then
        write(*,'(" ***advSmCons3dOrder2r:INFO: constmaterial=",i4," version=",i2," (4=var)")') constmaterial,version
      end if

      dtsq=dt*dt
      do i=0,2	
       dri(i)=1.0d0/dx(i)
      enddo
      
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

      if(version.eq.1) then           ! Bills version constant coeff
       if(addForcing.ne.0) then
        beginLoops() 
        un(i,j,k,uc)=cu*u(i,j,k,uc)+cum*um(i,j,k,uc)+dtsq*(sm23ru(i,j,k)+f(i,j,k,uc))
        un(i,j,k,vc)=cu*u(i,j,k,vc)+cum*um(i,j,k,vc)+dtsq*(sm23rv(i,j,k)+f(i,j,k,vc))
        un(i,j,k,wc)=cu*u(i,j,k,wc)+cum*um(i,j,k,wc)+dtsq*(sm23rw(i,j,k)+f(i,j,k,wc))
        endLoops()
       else
        beginLoops() 
        un(i,j,k,uc)=cu*u(i,j,k,uc)+cum*um(i,j,k,uc)+dtsq*sm23ru(i,j,k)
        un(i,j,k,vc)=cu*u(i,j,k,vc)+cum*um(i,j,k,vc)+dtsq*sm23rv(i,j,k)
        un(i,j,k,wc)=cu*u(i,j,k,wc)+cum*um(i,j,k,wc)+dtsq*sm23rw(i,j,k)
        endLoops()
       end if
      endif
!     inner loop
      if(version.eq.2) then           ! constant coeff
       lamcons = (c1-c2)
       mucons = c2
       lam2mucons = (c1-c2) +2.0*c2
       mulamcons = mucons+lamcons
       fq=0.5*dri(0)
       fr=0.5*dri(1)
       fs=0.5*dri(2)
       fqq=0.5*dri(0)*dri(0)
       frr=0.5*dri(1)*dri(1)
       fss=0.5*dri(2)*dri(2)
       fxxl2m=fqq*lam2mucons
       fyym=frr*mucons
       fzzm=fss*mucons
       fxyml=fq*fr*mulamcons
       fxzml=fq*fs*mulamcons
       fxxm=fqq*mucons
       fyyl2m=frr*lam2mucons
       fyzml=fr*fs*mulamcons
       fzzl2m=fss*lam2mucons
       if(addForcing.ne.0) then
        beginLoops() 
        im=i-1
        ip=i+1
        jm=j-1
        jp=j+1
        km=k-1
        kp=k+1
        u0=-4.d0*u(i,j,k,uc)*(fxxl2m+fyym+fzzm)
        v0=-4.d0*u(i,j,k,vc)*(fxxm+fyyl2m+fzzm)
        w0=-4.d0*u(i,j,k,wc)*(fxxm+fyym+fzzl2m)
        un(i,j,k,uc)=cu*u(i,j,k,uc)+cum*um(i,j,k,uc)+dtsq*(f(i,j,k,uc) + u0 + fxxl2m*(u(ip,j,k,uc)+u(im,j,k,uc)+u(ip,j,k,uc)+u(im,j,k,uc)) + fyym*(u(i,jp,k,uc)+u(i,jm,k,uc)+u(i,jp,k,uc)+u(i,jm,k,uc)) + fzzm*(u(i,j,kp,uc)+u(i,j,km,uc)+u(i,j,kp,uc)+u(i,j,km,uc)) + fxyml*(u(ip,jp,k,vc)-u(ip,jm,k,vc)-u(im,jp,k,vc)+u(im,jm,k,vc)) + fxzml*(u(ip,j,kp,wc)-u(ip,j,km,wc)-u(im,j,kp,wc)+u(im,j,km,wc)))
        un(i,j,k,vc)=cu*u(i,j,k,vc)+cum*um(i,j,k,vc)+dtsq*(f(i,j,k,vc) + v0 + fxxm*(u(ip,j,k,vc)+u(im,j,k,vc)+u(ip,j,k,vc)+u(im,j,k,vc)) + fyyl2m*(u(i,jp,k,vc)+u(i,jm,k,vc)+u(i,jp,k,vc)+u(i,jm,k,vc)) + fzzm*(u(i,j,kp,vc)+u(i,j,km,vc)+u(i,j,kp,vc)+u(i,j,km,vc)) + fyzml*(u(i,jp,kp,wc)-u(i,jp,km,wc)-u(i,jm,kp,wc)+u(i,jm,km,wc)) + fxyml*(u(ip,jp,k,uc)-u(ip,jm,k,uc)-u(im,jp,k,uc)+u(im,jm,k,uc)))
        un(i,j,k,wc)=cu*u(i,j,k,wc)+cum*um(i,j,k,wc)+dtsq*(f(i,j,k,wc) + w0 + fxxm*(u(ip,j,k,wc)+u(im,j,k,wc)+u(ip,j,k,wc)+u(im,j,k,wc)) + fyym*(u(i,jp,k,wc)+u(i,jm,k,wc)+u(i,jp,k,wc)+u(i,jm,k,wc)) + fxxl2m*(u(i,j,kp,wc)+u(i,j,km,wc)+u(i,j,kp,wc)+u(i,j,km,wc)) + fyzml*(u(i,jp,kp,vc)-u(i,jp,km,vc)-u(i,jm,kp,vc)+u(i,jm,km,vc)) + fxzml*(u(ip,j,kp,uc)-u(ip,j,km,uc)-u(im,j,kp,uc)+u(im,j,km,uc)))
        endLoops()
       else
        beginLoops() 
        im=i-1
        ip=i+1
        jm=j-1
        jp=j+1
        km=k-1
        kp=k+1
        u0=-4.d0*u(i,j,k,uc)*(fxxl2m+fyym+fzzm)
        v0=-4.d0*u(i,j,k,vc)*(fxxm+fyyl2m+fzzm)
        w0=-4.d0*u(i,j,k,wc)*(fxxm+fyym+fzzl2m)
        un(i,j,k,uc)=cu*u(i,j,k,uc)+cum*um(i,j,k,uc)+dtsq*(u0 + fxxl2m*(u(ip,j,k,uc)+u(im,j,k,uc)+u(ip,j,k,uc)+u(im,j,k,uc)) + fyym*(u(i,jp,k,uc)+u(i,jm,k,uc)+u(i,jp,k,uc)+u(i,jm,k,uc)) + fzzm*(u(i,j,kp,uc)+u(i,j,km,uc)+u(i,j,kp,uc)+u(i,j,km,uc)) + fxyml*(u(ip,jp,k,vc)-u(ip,jm,k,vc)-u(im,jp,k,vc)+u(im,jm,k,vc)) + fxzml*(u(ip,j,kp,wc)-u(ip,j,km,wc)-u(im,j,kp,wc)+u(im,j,km,wc)))
        un(i,j,k,vc)=cu*u(i,j,k,vc)+cum*um(i,j,k,vc)+dtsq*(v0 + fxxm*(u(ip,j,k,vc)+u(im,j,k,vc)+u(ip,j,k,vc)+u(im,j,k,vc)) + fyyl2m*(u(i,jp,k,vc)+u(i,jm,k,vc)+u(i,jp,k,vc)+u(i,jm,k,vc)) + fzzm*(u(i,j,kp,vc)+u(i,j,km,vc)+u(i,j,kp,vc)+u(i,j,km,vc)) + fyzml*(u(i,jp,kp,wc)-u(i,jp,km,wc)-u(i,jm,kp,wc)+u(i,jm,km,wc)) + fxyml*(u(ip,jp,k,uc)-u(ip,jm,k,uc)-u(im,jp,k,uc)+u(im,jm,k,uc)))
        un(i,j,k,wc)=cu*u(i,j,k,wc)+cum*um(i,j,k,wc)+dtsq*(w0 + fxxm*(u(ip,j,k,wc)+u(im,j,k,wc)+u(ip,j,k,wc)+u(im,j,k,wc)) + fyym*(u(i,jp,k,wc)+u(i,jm,k,wc)+u(i,jp,k,wc)+u(i,jm,k,wc)) + fxxl2m*(u(i,j,kp,wc)+u(i,j,km,wc)+u(i,j,kp,wc)+u(i,j,km,wc)) + fyzml*(u(i,jp,kp,vc)-u(i,jp,km,vc)-u(i,jm,kp,vc)+u(i,jm,km,vc)) + fxzml*(u(ip,j,kp,uc)-u(ip,j,km,uc)-u(im,j,kp,uc)+u(im,j,km,uc)))
        endLoops()
       endif
      endif
!     inner loop
      if(version.eq.3) then           ! constant coeff
       lamcons = (c1-c2)
       mucons = c2
       lam2mucons = (c1-c2) +2.0*c2
       mulamcons = mucons+lamcons
       fq=0.5*dri(0)
       fr=0.5*dri(1)
       fs=0.5*dri(2)
       fqq=0.5*dri(0)*dri(0)
       frr=0.5*dri(1)*dri(1)
       fss=0.5*dri(2)*dri(2)
       fxxl2m=fqq*lam2mucons
       fyym=frr*mucons
       fzzm=fss*mucons
       fxyml=fq*fr*mulamcons
       fxzml=fq*fs*mulamcons
       fxxm=fqq*mucons
       fyyl2m=frr*lam2mucons
       fyzml=fr*fs*mulamcons
       fzzl2m=fss*lam2mucons
       if(addForcing.ne.0) then
        beginLoops() 
        im=i-1
        ip=i+1
        jm=j-1
        jp=j+1
        km=k-1
        kp=k+1
        u0=-4.d0*u(i,j,k,uc)*(fxxl2m+fyym+fzzm)
        v0=-4.d0*u(i,j,k,vc)*(fxxm+fyyl2m+fzzm)
        w0=-4.d0*u(i,j,k,wc)*(fxxm+fyym+fzzl2m)
        
        un(i,j,k,uc)=cu*u(i,j,k,uc)+cum*um(i,j,k,uc)+dtsq*(f(i,j,k,uc) + u0 + fxxl2m*(u(ip,j,k,uc)+u(im,j,k,uc)+u(ip,j,k,uc)+u(im,j,k,uc)) + fyym*(u(i,jp,k,uc)+u(i,jm,k,uc)+u(i,jp,k,uc)+u(i,jm,k,uc)) + fzzm*(u(i,j,kp,uc)+u(i,j,km,uc)+u(i,j,kp,uc)+u(i,j,km,uc))) 
        un(i,j,k,vc)=cu*u(i,j,k,vc)+cum*um(i,j,k,vc)+dtsq*(f(i,j,k,vc) + fxyml*(u(ip,jp,k,uc)-u(ip,jm,k,uc)-u(im,jp,k,uc)+u(im,jm,k,uc)))
        un(i,j,k,wc)=cu*u(i,j,k,wc)+cum*um(i,j,k,wc)+dtsq*(f(i,j,k,wc) + fxzml*(u(ip,j,kp,uc)-u(ip,j,km,uc)-u(im,j,kp,uc)+u(im,j,km,uc)))
        
        un(i,j,k,uc) = un(i,j,k,uc) + dtsq*fxyml*(u(ip,jp,k,vc)-u(ip,jm,k,vc)-u(im,jp,k,vc)+u(im,jm,k,vc)) 
        un(i,j,k,vc) = un(i,j,k,vc) + dtsq*(v0 + fxxm*(u(ip,j,k,vc)+u(im,j,k,vc)+u(ip,j,k,vc)+u(im,j,k,vc)) + fyyl2m*(u(i,jp,k,vc)+u(i,jm,k,vc)+u(i,jp,k,vc)+u(i,jm,k,vc)) + fzzm*(u(i,j,kp,vc)+u(i,j,km,vc)+u(i,j,kp,vc)+u(i,j,km,vc)))
        un(i,j,k,wc) = un(i,j,k,wc) + dtsq*fyzml*(u(i,jp,kp,vc)-u(i,jp,km,vc)-u(i,jm,kp,vc)+u(i,jm,km,vc)) 
        
        un(i,j,k,uc) = un(i,j,k,uc) + dtsq*fxzml*(u(ip,j,kp,wc)-u(ip,j,km,wc)-u(im,j,kp,wc)+u(im,j,km,wc))
        un(i,j,k,vc) = un(i,j,k,vc) + dtsq*fyzml*(u(i,jp,kp,wc)-u(i,jp,km,wc)-u(i,jm,kp,wc)+u(i,jm,km,wc)) 
        un(i,j,k,wc) = un(i,j,k,wc) + dtsq*(w0 + fxxm*(u(ip,j,k,wc)+u(im,j,k,wc)+u(ip,j,k,wc)+u(im,j,k,wc)) + fyym*(u(i,jp,k,wc)+u(i,jm,k,wc)+u(i,jp,k,wc)+u(i,jm,k,wc)) + fxxl2m*(u(i,j,kp,wc)+u(i,j,km,wc)+u(i,j,kp,wc)+u(i,j,km,wc)))
        
        endLoops()
       else
        beginLoops() 
        im=i-1
        ip=i+1
        jm=j-1
        jp=j+1
        km=k-1
        kp=k+1
        u0=-4.d0*u(i,j,k,uc)*(fxxl2m+fyym+fzzm)
        v0=-4.d0*u(i,j,k,vc)*(fxxm+fyyl2m+fzzm)
        w0=-4.d0*u(i,j,k,wc)*(fxxm+fyym+fzzl2m)

        un(i,j,k,uc)=cu*u(i,j,k,uc)+cum*um(i,j,k,uc)+dtsq*(u0 + fxxl2m*(u(ip,j,k,uc)+u(im,j,k,uc)+u(ip,j,k,uc)+u(im,j,k,uc)) + fyym*(u(i,jp,k,uc)+u(i,jm,k,uc)+u(i,jp,k,uc)+u(i,jm,k,uc)) + fzzm*(u(i,j,kp,uc)+u(i,j,km,uc)+u(i,j,kp,uc)+u(i,j,km,uc))) 
        un(i,j,k,vc)=cu*u(i,j,k,vc)+cum*um(i,j,k,vc)+dtsq*(fxyml*(u(ip,jp,k,uc)-u(ip,jm,k,uc)-u(im,jp,k,uc)+u(im,jm,k,uc)))
        un(i,j,k,wc)=cu*u(i,j,k,wc)+cum*um(i,j,k,wc)+dtsq*(fxzml*(u(ip,j,kp,uc)-u(ip,j,km,uc)-u(im,j,kp,uc)+u(im,j,km,uc)))
        
        un(i,j,k,uc) = un(i,j,k,uc) + dtsq*fxyml*(u(ip,jp,k,vc)-u(ip,jm,k,vc)-u(im,jp,k,vc)+u(im,jm,k,vc)) 
        un(i,j,k,vc) = un(i,j,k,vc) + dtsq*(v0 + fxxm*(u(ip,j,k,vc)+u(im,j,k,vc)+u(ip,j,k,vc)+u(im,j,k,vc)) + fyyl2m*(u(i,jp,k,vc)+u(i,jm,k,vc)+u(i,jp,k,vc)+u(i,jm,k,vc)) + fzzm*(u(i,j,kp,vc)+u(i,j,km,vc)+u(i,j,kp,vc)+u(i,j,km,vc)))
        un(i,j,k,wc) = un(i,j,k,wc) + dtsq*fyzml*(u(i,jp,kp,vc)-u(i,jp,km,vc)-u(i,jm,kp,vc)+u(i,jm,km,vc)) 
        
        un(i,j,k,uc) = un(i,j,k,uc) + dtsq*fxzml*(u(ip,j,kp,wc)-u(ip,j,km,wc)-u(im,j,kp,wc)+u(im,j,km,wc))
        un(i,j,k,vc) = un(i,j,k,vc) + dtsq*fyzml*(u(i,jp,kp,wc)-u(i,jp,km,wc)-u(i,jm,kp,wc)+u(i,jm,km,wc)) 
        un(i,j,k,wc) = un(i,j,k,wc) + dtsq*(w0 + fxxm*(u(ip,j,k,wc)+u(im,j,k,wc)+u(ip,j,k,wc)+u(im,j,k,wc)) + fyym*(u(i,jp,k,wc)+u(i,jm,k,wc)+u(i,jp,k,wc)+u(i,jm,k,wc)) + fxxl2m*(u(i,j,kp,wc)+u(i,j,km,wc)+u(i,j,kp,wc)+u(i,j,km,wc)))
         endLoops()
       endif
      endif
!     variable coeff
      if(version.eq.4) then
       if(addForcing.ne.0) then
       fq=0.5*dri(0)
       fr=0.5*dri(1)
       fs=0.5*dri(2)
       beginLoops() 
       rhtmpu=0.
       rhtmpv=0.
       rhtmpw=0.
       im=i-1
       ip=i+1
       jm=j-1
       jp=j+1
       km=k-1
       kp=k+1
       setupMaterial()
       setupU()
       addUterms()	
       addUCrossterms()	
       setupV()
       addVterms()	
       addVCrossterms()	
       setupW()
       addWterms()
       addWCrossterms()	
       un(i,j,k,uc)=cu*u(i,j,k,uc)+cum*um(i,j,k,uc)+dtsq*rhtmpu
       un(i,j,k,vc)=cu*u(i,j,k,vc)+cum*um(i,j,k,vc)+dtsq*rhtmpv
       un(i,j,k,wc)=cu*u(i,j,k,wc)+cum*um(i,j,k,wc)+dtsq*rhtmpw
       endLoops()
      else
       fq=0.5*dri(0)
       fr=0.5*dri(1)
       fs=0.5*dri(2)
       beginLoops() 
       rhtmpu=0.
       rhtmpv=0.
       rhtmpw=0.
       im=i-1
       ip=i+1
       jm=j-1
       jp=j+1
       km=k-1
       kp=k+1
       setupMaterial()
       setupU()
       addUterms()	
       addUCrossterms()	
       setupV()
       addVterms()	
       addVCrossterms()	
       setupW()
       addWterms()
       addWCrossterms()	
       un(i,j,k,uc)=cu*u(i,j,k,uc)+cum*um(i,j,k,uc)+dtsq*(rhtmpu+f(i,j,k,uc))
       un(i,j,k,vc)=cu*u(i,j,k,vc)+cum*um(i,j,k,vc)+dtsq*(rhtmpv+f(i,j,k,vc))
       un(i,j,k,wc)=cu*u(i,j,k,wc)+cum*um(i,j,k,wc)+dtsq*(rhtmpw+f(i,j,k,wc))
       endLoops()
      end if
      endif
!     end inner loop        
! Correct the sides if necessary
      if(bc(0,0).eq.stressFree) then
       correctCorners(n1a,n1a,n2a,n2b,n3a,n3b,0,1,1,1,1,1,1.0,0.5,0.5)
      end if
      if(bc(1,0).eq.stressFree) then 
       correctCorners(n1b,n1b,n2a,n2b,n3a,n3b,1,0,1,1,1,1,1.0,0.5,0.5)
      end if
      if(bc(0,1).eq.stressFree) then
       correctCorners(n1a,n1b,n2a,n2a,n3a,n3b,1,1,0,1,1,1,0.5,1.0,0.5)
      end if
      if(bc(1,1).eq.stressFree) then 
       correctCorners(n1a,n1b,n2b,n2b,n3a,n3b,1,1,1,0,1,1,0.5,1.0,0.5)
      end if
      if(bc(0,2).eq.stressFree) then 
       correctCorners(n1a,n1b,n2a,n2b,n3a,n3a,1,1,1,1,0,1,0.5,0.5,1.0)
      end if
      if(bc(1,2).eq.stressFree) then 
       correctCorners(n1a,n1b,n2a,n2b,n3b,n3b,1,1,1,1,1,0,0.5,0.5,1.0)
      end if
!     And the edge
      if((bc(0,0).eq.stressFree).and.(bc(0,1).eq.stressFree)) then
       correctCorners(n1a,n1a,n2a,n2a,n3a,n3b,0,1,0,1,1,1,1.0,1.0,0.5)
      end if
      if((bc(0,0).eq.stressFree).and.(bc(1,1).eq.stressFree)) then
       correctCorners(n1a,n1a,n2b,n2b,n3a,n3b,0,1,1,0,1,1,1.0,1.0,0.5)
      end if
      if((bc(0,0).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
       correctCorners(n1a,n1a,n2a,n2b,n3a,n3a,0,1,1,1,0,1,1.0,0.5,1.0)
      end if
      if((bc(0,0).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
       correctCorners(n1a,n1a,n2a,n2b,n3b,n3b,0,1,1,1,1,0,1.0,0.5,1.0)
      end if
      
      if((bc(1,0).eq.stressFree).and.(bc(0,1).eq.stressFree)) then
       correctCorners(n1b,n1b,n2a,n2a,n3a,n3b,1,0,0,1,1,1,1.0,1.0,0.5)
      end if
      if((bc(1,0).eq.stressFree).and.(bc(1,1).eq.stressFree)) then
       correctCorners(n1b,n1b,n2b,n2b,n3a,n3b,1,0,1,0,1,1,1.0,1.0,0.5)
      end if
      if((bc(1,0).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
       correctCorners(n1b,n1b,n2a,n2b,n3a,n3a,1,0,1,1,0,1,1.0,0.5,1.0)
      end if
      if((bc(1,0).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
       correctCorners(n1b,n1b,n2a,n2b,n3b,n3b,1,0,1,1,1,0,1.0,0.5,1.0)
      end if
      
      if((bc(0,1).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
       correctCorners(n1a,n1b,n2a,n2a,n3a,n3a,1,1,0,1,0,1,0.5,1.0,1.0)
      end if
      if((bc(0,1).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
       correctCorners(n1a,n1b,n2a,n2a,n3b,n3b,1,1,0,1,1,0,0.5,1.0,1.0)
      end if
      if((bc(1,1).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
       correctCorners(n1a,n1b,n2b,n2b,n3a,n3a,1,1,1,0,0,1,0.5,1.0,1.0)
      end if
      if((bc(1,1).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
       correctCorners(n1a,n1b,n2b,n2b,n3b,n3b,1,1,1,0,1,0,0.5,1.0,1.0)
      end if
      
!     Finally Corners
      if((bc(0,0).eq.stressFree).and.(bc(0,1).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
       correctCorners(n1a,n1a,n2a,n2a,n3a,n3a,0,1,0,1,0,1,1.0,1.0,1.0)
      end if
      if((bc(0,0).eq.stressFree).and.(bc(0,1).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
       correctCorners(n1a,n1a,n2a,n2a,n3b,n3b,0,1,0,1,1,0,1.0,1.0,1.0)
      end if
      if((bc(0,0).eq.stressFree).and.(bc(1,1).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
       correctCorners(n1a,n1a,n2b,n2b,n3a,n3a,0,1,1,0,0,1,1.0,1.0,1.0)
      end if
      if((bc(0,0).eq.stressFree).and.(bc(1,1).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
       correctCorners(n1a,n1a,n2b,n2b,n3b,n3b,0,1,1,0,1,0,1.0,1.0,1.0)
      end if
      if((bc(1,0).eq.stressFree).and.(bc(0,1).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
       correctCorners(n1b,n1b,n2a,n2a,n3a,n3a,1,0,0,1,0,1,1.0,1.0,1.0)
      end if
      if((bc(1,0).eq.stressFree).and.(bc(0,1).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
       correctCorners(n1b,n1b,n2a,n2a,n3b,n3b,1,0,0,1,1,0,1.0,1.0,1.0)
      end if
      if((bc(1,0).eq.stressFree).and.(bc(1,1).eq.stressFree).and.(bc(0,2).eq.stressFree)) then
       correctCorners(n1b,n1b,n2b,n2b,n3a,n3a,1,0,1,0,0,1,1.0,1.0,1.0)
      end if
      if((bc(1,0).eq.stressFree).and.(bc(1,1).eq.stressFree).and.(bc(1,2).eq.stressFree)) then
       correctCorners(n1b,n1b,n2b,n2b,n3b,n3b,1,0,1,0,1,0,1.0,1.0,1.0)
      end if
      
      if( (orderOfDissipation.eq.4 ).and.(adc.gt.0))then
      ! *wdh* 100203 adcdt=adc*dt
      beginLoopsD() 
      un(i1,i2,i3,uc)=un(i1,i2,i3,uc)+adcdt*fd43d(i1,i2,i3,uc)
      un(i1,i2,i3,vc)=un(i1,i2,i3,vc)+adcdt*fd43d(i1,i2,i3,vc)
      un(i1,i2,i3,wc)=un(i1,i2,i3,wc)+adcdt*fd43d(i1,i2,i3,wc)
      endLoops()       
      end if
      if( (orderOfDissipation.eq.2 ).and.(adc.gt.0))then
      ! *wdh* 100203 adcdt=adc*dt
      beginLoopsD() 

      un(i1,i2,i3,uc)=un(i1,i2,i3,uc)+adcdt*fd23d(i1,i2,i3,uc)
      un(i1,i2,i3,vc)=un(i1,i2,i3,vc)+adcdt*fd23d(i1,i2,i3,vc)
      un(i1,i2,i3,wc)=un(i1,i2,i3,wc)+adcdt*fd23d(i1,i2,i3,wc)
      endLoops()       
      end if

      if(debug.eq.3) then	
      energy=0.d0
      ! DEAA ENERGY
      do i3=n3a,n3b
      do i2=n2a,n2b
      do i1=n1a,n1b
      	weight=1.d0
	if ((i1.eq.n1a).and.((bc(0,0).eq.stressFree))) weight=weight*0.5d0
	if ((i1.eq.n1b).and.((bc(1,0).eq.stressFree))) weight=weight*0.5d0
	if ((i2.eq.n2a).and.((bc(0,1).eq.stressFree))) weight=weight*0.5d0
	if ((i2.eq.n2b).and.((bc(1,1).eq.stressFree))) weight=weight*0.5d0
	if ((i3.eq.n3a).and.((bc(0,2).eq.stressFree))) weight=weight*0.5d0
	if ((i3.eq.n3b).and.((bc(1,2).eq.stressFree))) weight=weight*0.5d0
!        write (*,*),i1,i2,weight
	energy=energy-weight*un(i1,i2,i3,uc)*rh1(i1,i2,i3)
	energy=energy-weight*un(i1,i2,i3,vc)*rh2(i1,i2,i3)
	energy=energy-weight*un(i1,i2,i3,wc)*rh3(i1,i2,i3)
	!       we use f to store u_t
        rh1(i1,i2,i3)=(un(i1,i2,i3,uc)-u(i1,i2,i3,uc))/dt
        rh2(i1,i2,i3)=(un(i1,i2,i3,vc)-u(i1,i2,i3,vc))/dt
        rh3(i1,i2,i3)=(un(i1,i2,i3,wc)-u(i1,i2,i3,wc))/dt
	energy=energy+weight*rh1(i1,i2,i3)*rh1(i1,i2,i3)
	energy=energy+weight*rh2(i1,i2,i3)*rh2(i1,i2,i3)
	energy=energy+weight*rh3(i1,i2,i3)*rh3(i1,i2,i3)
      endLoops() 
      write(*,*) "Discrete energy  ",energy
      end if

      end 

