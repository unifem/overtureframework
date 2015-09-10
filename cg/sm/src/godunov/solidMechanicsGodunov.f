! ====================================================================================
!  solidMechanicsGodunov : 
!     Fortran interface to godunov solvers
!
! Parameters:
!   nd (input) : number of dimensions
!   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b (input) : array dimensions
!   mask (input) : mask array
!   rx (input) : jacobian matrix
!   xy (input) : vertex coordinates
!   det (input) : jacobian
!   u (input) : solution at time t
!   up (output) : du/dt 
!   f1, f2 (input) : forcing functions f1=f(t), f2=f(t+dt)
!   ipar (input) : integer parameters (see below)
!   rpar (input) : real parameters (see below)
! 
! ====================================================================================
      subroutine solidMechanicsGodunov(
     &                nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &                mask,rx,xy,det, u,up, f1,f2, 
     &                ndMatProp, matIndex, matValpc, matVal,
     &                ad,ad2dt,ad4,ad4dt, ipar,rpar,
     &                niwk,iwk, nrwk,rwk,ierr ) 

      implicit none

      integer niprm,nrprm,m,ier
      parameter (nrprm=20,niprm=20)
      integer iparam(niprm)
      real rparam(nrprm)

      integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndMatProp

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd)
      real det(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)
      real up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)

      real f1(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)
      real f2(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)

      ! Arrays for variable material properties:
      integer matIndex(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real matValpc(ndMatProp,0:*)
      real matVal(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)

      real ad(1:*),ad2dt(1:*),ad4(1:*),ad4dt(1:*)
      integer niwk,nrwk,ierr
      real rpar(0:*),rwk(nrwk)
      integer ipar(0:*),iwk(niwk)

      integer n1a,n1b,n2a,n2b,n3a,n3b, numberOfComponents
      integer gridType,grid,iorder,idebug,tzflow,i1,i2,i3,n
      integer method,ilimit,iupwind,itype,addForcing,materialFormat
      real t,dt,dx(0:2),dr(0:2),rho,lambda,mu,ep

      ! relaxation of stress to stress derived from position ... jwb 11 Aug 2010
      integer stressRelaxation
      real relaxAlpha, relaxDelta
      real tangentialStressDissipation,tsdissdt ! for SVK 

      n1a = ipar(0)
      n1b = ipar(1)
      n2a = ipar(2)
      n2b = ipar(3)
      n3a = ipar(4)
      n3b = ipar(5)
      numberOfComponents=ipar(6)
      gridType=ipar(7)
      grid =ipar(8)
      iorder=ipar(9)
      idebug=ipar(10)
      tzflow=ipar(11) ! 0=off 1=on 

      method=ipar(13)
      ilimit=ipar(14)
      iupwind=ipar(15)
      itype=ipar(16)    ! pdeTypeForGodunovMethod : 0=linear, 1=SVK

      addForcing=ipar(17) ! =1 -> add forcing 
      stressRelaxation = ipar(18)

      materialFormat=ipar(19)  ! 0=constant-material-properties, 1=piece-wise constant, 2=variable

c      write(6,*)'gridType =',gridType
c      pause

      t    = rpar(0)
      dt   = rpar(1)
      dx(0)= rpar(2)
      dx(1)= rpar(3)
      dx(2)= rpar(4)
      dr(0)= rpar(5)
      dr(1)= rpar(6)
      dr(2)= rpar(7)
      rho  = rpar(8)
      lambda=rpar(9)
      mu   = rpar(10)
      ep   = rpar(11) ! exact solution identifier for tzflow
      relaxAlpha = rpar(12)
      relaxDelta = rpar(13) ! *wdh* NOTE: no-longer used as relaxAlpha is now the full coefficient
      tangentialStressDissipation = rpar(14)  ! tangential-stress diss coeff
      tsdissdt = rpar(15)                     ! tangential-stress dt diss coeff

      ! return these: 
      ! rpar(20) = reLambda
      ! rpar(21) = imLambda 


      ! z0=0.
      ! call ogDeriv(ep, ntd,nxd,nyd,nzd, xy(i1,i2,i3,0),xy(i1,i2,i3,1),z0,t, n,ud )

      ! write(*,'("smg: materialFormat=",i2)') materialFormat

c      write(*,'("smg: t=",e9.3," (mu,lam)=(",f4.2,",",f4.2,")")') 
c     & t,mu,lambda

c      write(*,'("smg: gridType,grid,iorder,tz=",4i4)') gridType,grid,
c     & iorder,tzflow
c      write(*,'("smg: niwk,nrwk=",i8,2x,i8)') niwk,nrwk
c      write(*,'("smg: n1a,n1b,...=",6i4)') n1a,n1b,n2a,n2b,n3a,n3b
c      write(*,'("smg: dx=",3e9.2)') dx(0),dx(1),dx(2)
c      write(*,'("smg: dr=",3e9.2)') dr(0),dr(1),dr(2)

      iparam(10)=idebug ! *wdh* 090905 
      iparam(11)=materialFormat

      if( nd.eq.3 ) then
        m = numberOfComponents
        rparam(3) = mu
        rparam(4) = lambda
        rparam(5) = rho
        rparam(6) = ep
        rparam(7) = relaxAlpha
        rparam(8) = relaxDelta  ! *wdh* NOTE: no-longer used as relaxAlpha is now the full coefficient
        rparam(9) = tangentialStressDissipation
        rparam(10) = tsdissdt

        iparam(1) = iorder
        iparam(3) = tzflow
        iparam(5) = ilimit
        iparam(7) = itype
        iparam(8) = addForcing
        iparam(9) = stressRelaxation

        if( itype.eq.0 ) then ! linear elasticity
          if( gridType.eq.0 ) then
            iparam(2) = 1       ! Cartesian case, icart=1
            call smg3d( m,nd1a,nd1b,n1a,n1b,
     *                  nd2a,nd2b,n2a,n2b,
     *                  nd3a,nd3b,n3a,n3b,
     *                  dx(0),dx(1),dx(2),dt,t,xy,rx,det,
     *                  u,up,mask,ad, ! *wdh* 091113 -- add "ad"
     *                  nrprm,rparam,niprm,iparam,nrwk,rwk,
     *                  niwk,iwk,idebug,ier )
          else
            iparam(2) = 0       ! non-Cartesian case, icart=0
            call smg3d( m,nd1a,nd1b,n1a,n1b,
     *                  nd2a,nd2b,n2a,n2b,
     *                  nd3a,nd3b,n3a,n3b,
     *                  dr(0),dr(1),dr(2),dt,t,xy,rx,det,
     *                  u,up,mask,ad,  ! *wdh* 091113 -- add "ad"
     *                  nrprm,rparam,niprm,iparam,nrwk,rwk,
     *                  niwk,iwk,idebug,ier )
          end if
          rpar(20) = rparam(1)
          rpar(21) = rparam(2)
          rpar(22) = rparam(3)
        else ! use nonlinear code
          if( gridType.eq.0 ) then
            iparam(2) = 1       ! Cartesian case, icart=1
            call smg3dNL( m,nd1a,nd1b,n1a,n1b,
     *                    nd2a,nd2b,n2a,n2b,
     *                    nd3a,nd3b,n3a,n3b,
     *                    dx(0),dx(1),dx(2),dt,t,xy,rx,det,
     *                    u,up,f1,f2,mask,ad,
     *                    nrprm,rparam,niprm,iparam,nrwk,rwk,
     *                    niwk,iwk,idebug,ier )
          else
            iparam(2) = 0       ! non-Cartesian case, icart=0
            call smg3dNL( m,nd1a,nd1b,n1a,n1b,
     *                    nd2a,nd2b,n2a,n2b,
     *                    nd3a,nd3b,n3a,n3b,
     *                    dr(0),dr(1),dr(2),dt,t,xy,rx,det,
     *                    u,up,f1,f2,mask,ad,
     *                    nrprm,rparam,niprm,iparam,nrwk,rwk,
     *                    niwk,iwk,idebug,ier )
          end if
          rpar(20) = rparam(1)
          rpar(21) = rparam(2)
          rpar(22) = rparam(3)
        end if
      else if (nd.eq.2) then
        m=8
        rparam(3)=mu
        rparam(4)=lambda
        rparam(5)=rho
        rparam(6)=ep
        rparam(7) = relaxAlpha
        rparam(8) = relaxDelta  ! *wdh* NOTE: no-longer used as relaxAlpha is now the full coefficient
        rparam(9)=tangentialStressDissipation
        rparam(10) = tsdissdt

        iparam(1)=iorder
        iparam(3)=tzflow
        iparam(4)=method
        iparam(5)=ilimit
        iparam(6)=iupwind
        iparam(7)=itype
        iparam(8)=addForcing
        iparam(9) = stressRelaxation

        if( tzflow.ne.0 ) iparam(8)=0 ! turn off array forcing if TZ is on

c        if( .false. ) then
        if( .true. ) then ! Godunov
        if (gridType.eq.0) then
          iparam(2)=1             ! Cartesian case, icart=1
          if (itype.le.0) then
            ! linear elasticity
            if( materialFormat.eq.0 )then
             ! constant material properties
              call smg2d  (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                     dx(0),dx(1),dt,t,xy,rx,det,u,up,f1,f2,ad,
     *                     mask,nrprm,rparam,niprm,iparam,nrwk,rwk,
     *                     niwk,iwk,idebug,ier)
            else
             ! variable material properties
              call smgvc2d(m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                     dx(0),dx(1),dt,t,xy,rx,det,u,up,f1,f2,ad,
     *                     ndMatProp,matIndex,matValpc,matVal,
     *                     mask,nrprm,rparam,niprm,iparam,nrwk,rwk,
     *                     niwk,iwk,idebug,ier)
            end if
          else
            ! nonlinear elasticity (SVK model):
            call smg2dn (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   dx(0),dx(1),dt,t,xy,rx,det,u,up,f1,f2,ad,
     *                   ad2dt,ad4,ad4dt,mask,nrprm,rparam,niprm,iparam,
     *                   nrwk,rwk,niwk,iwk,idebug,ier)
          end if
        else
          iparam(2)=0             ! non-Cartesian case, icart=0
          if (itype.le.0) then
            ! linear elasticity
            if( materialFormat.eq.0 )then
             ! constant material properties
              call smg2d  (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                     dr(0),dr(1),dt,t,xy,rx,det,u,up,f1,f2,ad,
     *                     mask,nrprm,rparam,niprm,iparam,nrwk,rwk,
     *                     niwk,iwk,idebug,ier)
            else
             ! variable material properties
              call smgvc2d(m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                     dr(0),dr(1),dt,t,xy,rx,det,u,up,f1,f2,ad,
     *                     ndMatProp,matIndex,matValpc,matVal,
     *                     mask,nrprm,rparam,niprm,iparam,nrwk,rwk,
     *                     niwk,iwk,idebug,ier)
            end if
          else
            ! nonlinear elasticity (SVK model):
            call smg2dn (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   dr(0),dr(1),dt,t,xy,rx,det,u,up,f1,f2,ad,
     *                   ad2dt,ad4,ad4dt,mask,nrprm,rparam,niprm,iparam,
     *                   nrwk,rwk,niwk,iwk,idebug,ier)
          end if
        end if
        else ! centered scheme with 4th order diffusion
          call smcent2d( m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *       dt,t,xy,u,up,nrprm,rparam,niprm,iparam,ier )
        end if
c        write(*,'(" solidMechGod: itype,eigs=",i4,3e10.2)') itype,
c     & rparam(1),rparam(2),rparam(3)
        rpar(20) = rparam(1)
        rpar(21) = rparam(2)
        rpar(22) = rparam(3)

      else
        write(*,100)
  100   format('Error (smg) : value for nd not supported')
        stop
      end if

      return 
      end
