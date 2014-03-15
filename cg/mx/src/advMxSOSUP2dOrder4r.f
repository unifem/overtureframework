! This file automatically generated from advSOSUP.bf with bpp.
        subroutine advMxSOSUP2dOrder4r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,ndf4a,ndf4b,mask,rsxy,  u,
     & un,f0, bc, ipar, rpar, ierr )
       !======================================================================
       !   Advance a time step for Maxwells equations
       !      SOSUP: SECOND-4-SYSTEM UPWIND SCHEME 
       !     
       ! nd : number of space dimensions
       !
       ! ipar(0)  = option : option=0 - Maxwell+Artificial diffusion
       !                           =1 - AD only
       !
       !  dis(i1,i2,i3) : temp space to hold artificial dissipation
       !  varDis(i1,i2,i3) : coefficient of the variable artificial dissipation
       !======================================================================
        implicit none
        integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b,ndf4a,ndf4b
        real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real f0(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,ndf4a:ndf4b,0:*)
        real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
        integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        integer bc(0:1,0:2),ierr
        integer ipar(0:*)
        real rpar(0:*)
       !     ---- local variables -----
        integer c,i1,i2,i3,n,gridType,orderOfAccuracy,orderInTime
        integer addForcing,orderOfDissipation,option
        integer useWhereMask,useWhereMaskSave,solveForE,solveForH,grid,
     & useVariableDissipation
        integer useCurvilinearOpt,useConservative,
     & combineDissipationWithAdvance,useDivergenceCleaning
        integer ex,ey,ez, hx,hy,hz, ext,eyt,ezt, hxt,hyt,hzt
        real t,cc,dt,dy,dz,cdt,cdtdx,cdtdy,cdtdz,adc,adcdt,add,adddt
        real dt4by12
        real eps,mu,sigmaE,sigmaH,kx,ky,kz,
     & divergenceCleaningCoefficient
        logical addDissipation
        real dx(0:2),dr(0:2)
        real dx2i,dy2i,dz2i,dxsqi,dysqi,dzsqi,dxi,dyi,dzi
        real dx12i,dy12i,dz12i,dxsq12i,dysq12i,dzsq12i,dxy4i,dxz4i,
     & dyz4,time0,time1
        real dxi4,dyi4,dzi4,dxdyi2,dxdzi2,dydzi2
        real c0,c1,csq,dtsq,cdtsq,cdtsq12,lap(0:20)
        integer rectangular,curvilinear
        parameter( rectangular=0, curvilinear=1 )
        integer timeSteppingMethod
        integer defaultTimeStepping,adamsSymmetricOrder3,
     & rungeKuttaFourthOrder,stoermerTimeStepping,
     & modifiedEquationTimeStepping
        parameter(defaultTimeStepping=0,adamsSymmetricOrder3=1,
     & rungeKuttaFourthOrder=2,stoermerTimeStepping=3,
     & modifiedEquationTimeStepping=4)
       !...........start statement function
        integer kd,m
        real rx,ry,rz,sx,sy,sz,tx,ty,tz
        ! temporary statement function for f for backward compatibility
        real f
        f(i1,i2,i3,n) = f0(i1,i2,i3,n,0)
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
       !...........end   statement functions
        ! write(*,*) 'Inside advSOSUP...'
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
        ext                =ipar(21)
        eyt                =ipar(22)
        ezt                =ipar(23)
        hxt                =ipar(24)
        hyt                =ipar(25)
        hzt                =ipar(26)
        ! addDissipation=.true. if we add the dissipation in the dis(i1,i2,i3,c) array
        !  if combineDissipationWithAdvance.ne.0 we compute the dissipation on the fly in the time step
        !  rather than pre-computing it in diss(i1,i2,i3,c)
        addDissipation = adc.gt.0. .and. 
     & combineDissipationWithAdvance.eq.0
        adcdt=adc*dt
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
        if( option.eq.1 ) then
          ! dissipation only --  this should not happen 
          stop 1298
        end if
       ! write(*,'(" advSOSUP: timeSteppingMethod=",i2)') timeSteppingMethod
        if( timeSteppingMethod.eq.defaultTimeStepping )then
         write(*,'(" advSOSUP:ERROR: 
     & timeSteppingMethod=defaultTimeStepping -- this should be set")
     & ')
           ! '
         stop 83322
        end if
        if( gridType.eq.rectangular )then
       !       **********************************************
       !       *************** rectangular ******************
       !       **********************************************
             call duWaveGen2d4rc( nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     & ndf4a,ndf4b,ex,addForcing,u(nd1a,nd2a,nd3a,ex),u(nd1a,nd2a,
     & nd3a,ext),un(nd1a,nd2a,nd3a,ex),un(nd1a,nd2a,nd3a,ext),f0,dx(0)
     & ,dx(1),dt,cc,useWhereMask,mask )
             call duWaveGen2d4rc( nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     & ndf4a,ndf4b,ey,addForcing,u(nd1a,nd2a,nd3a,ey),u(nd1a,nd2a,
     & nd3a,eyt),un(nd1a,nd2a,nd3a,ey),un(nd1a,nd2a,nd3a,eyt),f0,dx(0)
     & ,dx(1),dt,cc,useWhereMask,mask )
             call duWaveGen2d4rc( nd1a,nd1b,nd2a,nd2b,n1a,n1b,n2a,n2b,
     & ndf4a,ndf4b,hz,addForcing,u(nd1a,nd2a,nd3a,hz),u(nd1a,nd2a,
     & nd3a,hzt),un(nd1a,nd2a,nd3a,hz),un(nd1a,nd2a,nd3a,hzt),f0,dx(0)
     & ,dx(1),dt,cc,useWhereMask,mask )
        else
        end if
        return
        end
