! This file automatically generated from bcOptSmFOS.bf with bpp.
        subroutine bcSmFOS3d( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     & gridIndexRange, u, mask,rx, xy, ndMatProp,matIndex,matValpc,
     & matVal, det, boundaryCondition, addBoundaryForcing, 
     & interfaceType, dim, bcf00,bcf10,bcf01,bcf11,bcf02,bcf12,bcf0,
     & bcOffset, ndpin, pinbc, ndpv, pinValues, ipar, rpar, pdb, ierr 
     & )
  ! ===================================================================================
  !  Boundary conditions for solid mechanics : First Order System
  !
  !  gridType : 0=rectangular, 1=curvilinear
  !
  !  c2= mu/rho, c1=(mu+lambda)/rho;
  ! 
  ! The forcing for the boundary conditions can be accessed in two ways. One can either 
  ! use the arrays: 
  !       bcf00(i1,i2,i3,m), bcf10(i1,i2,i3,m), bcf01(i1,i2,i3,m), bcf11(i1,i2,i3,m), 
  !       bcf02(i1,i2,i3,m), bcf12(i1,i2,i3,m)
  ! which provide values for the 6 different faces in 6 different arrays. One can also
  ! access the same values using the single statement function
  !         bcf(side,axis,i1,i2,i3,m)
  ! which is defined below. 
  ! ===================================================================================
        implicit none
        integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndpin,ndpv, ierr
        real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
        integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
        real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
        real det(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        integer gridIndexRange(0:1,0:2),boundaryCondition(0:1,0:2)
        integer addBoundaryForcing(0:1,0:2)
        integer interfaceType(0:1,0:2,0:*)
        integer dim(0:1,0:2,0:1,0:2)
        integer pinbc(0:ndpin-1,0:*)
        real pinValues(0:ndpv-1,0:*)
        integer dir,stride1,stride2,stride3
        integer sidea,sideb,bc1,bc2,bc3,edgeDirection
        real bcf00(dim(0,0,0,0):dim(1,0,0,0), dim(0,1,0,0):dim(1,1,0,0)
     & , dim(0,2,0,0):dim(1,2,0,0),0:*)
        real bcf10(dim(0,0,1,0):dim(1,0,1,0), dim(0,1,1,0):dim(1,1,1,0)
     & , dim(0,2,1,0):dim(1,2,1,0),0:*)
        real bcf01(dim(0,0,0,1):dim(1,0,0,1), dim(0,1,0,1):dim(1,1,0,1)
     & , dim(0,2,0,1):dim(1,2,0,1),0:*)
        real bcf11(dim(0,0,1,1):dim(1,0,1,1), dim(0,1,1,1):dim(1,1,1,1)
     & , dim(0,2,1,1):dim(1,2,1,1),0:*)
        real bcf02(dim(0,0,0,2):dim(1,0,0,2), dim(0,1,0,2):dim(1,1,0,2)
     & , dim(0,2,0,2):dim(1,2,0,2),0:*)
        real bcf12(dim(0,0,1,2):dim(1,0,1,2), dim(0,1,1,2):dim(1,1,1,2)
     & , dim(0,2,1,2):dim(1,2,1,2),0:*)
        real bcf0(0:*)
        integer*8 bcOffset(0:1,0:2)
        integer ipar(0:*)
        real rpar(0:*)
        integer sc(3,3),isc,delta(3,3),idot
        integer tan1c,tan2c,ipiv(0:2,0:1),info
        ! -- Declare arrays for variable material properties --
        include 'declareVarMatProp.h'
        double precision pdb  ! pointer to data base
c     --- local variables ----
        integer numberOfComponents,applyInterfaceBoundaryConditions,
     & projectInterface,numToPin
        real ue,ve,we,v1e,v2e,v3e,uet,vet,wet,v1et,v2et,v3et, uem,uep
        real uex,uey,uez, vex,vey,vez, wex,wey,wez
        real v1ex,v1ey,v1ez, v2ex,v2ey,v2ez, v3ex,v3ey,v3ez
        real tau11,tau21,tau31
        real tau12,tau22,tau32
        real tau13,tau23,tau33
        real tau11e,tau21e,tau31e
        real tau12e,tau22e,tau32e
        real tau13e,tau23e,tau33e
        real tau11x,tau12x,tau13x
        real tau11y,tau12y,tau13y
        real tau11z,tau12z,tau13z
        real tau21x,tau22x,tau23x
        real tau21y,tau22y,tau23y
        real tau21z,tau22z,tau23z
        real tau31x,tau32x,tau33x
        real tau31y,tau32y,tau33y
        real tau31z,tau32z,tau33z
        real accel1,accel2,accel3,accel(3)
        real met(0:2,0:2)
        real s11tilde,s12tilde,s13tilde
        real s21tilde,s22tilde,s23tilde
        real s31tilde,s32tilde,s33tilde
        real stilde(3)
        real dux,duy,duz
        real dvx,dvy,dvz
        real dwx,dwy,dwz
        real dur(0:2),dvr(0:2),dwr(0:2)
        real dv1r(0:2),dv2r(0:2),dv3r(0:2)
        real mat(0:2,0:2),lhs(0:2,0:2)
        real rhs(0:2,0:1)
        real norm1(0:2),norm2(0:2)
        real norma(0:2),normb(0:2)
        integer side,axis,grid,gridType,orderOfAccuracy,
     & orderOfExtrapolation,twilightZone,uc,vc,wc,useWhereMask,debug,
     & nn,n1,n2
        real dx(0:2),dr(0:2)
        real t,ep,dt,c1,c2,mu,lambda,kappa,rho
        integer axisp1,axisp2,i1,i2,i3,is1,is2,is3,j1,j2,j3,js1,js2,
     & js3,ks1,ks2,ks3,is,js,it,nit
        integer option,initialized
        integer numGhost,numberOfGhostPoints
        integer side1,side2,side3
        integer n1a,n1b,n2a,n2b,n3a,n3b
        integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b
        integer extra1a,extra1b,extra2a,extra2b,extra3a,extra3b
        integer extra,numGhostExtrap
        integer ok,getInt,getReal
        integer v1c,v2c,v3c,s11c,s12c,s13c,s21c,s22c,s23c,s31c,s32c,
     & s33c,rhoc,muc,lambdac
        integer s21cSave
        real ux0,uy0,uz0,vx0,vy0,vz0,wx0,wy0,wz0
        real an1,an2,an3,aNormi,epsx,f1,f2,f3,b1,b2,b3
        real ns1,ns2,ns3,ss1,ss2,ss3,ts1,ts2,ts3
        real ss1d,ss2d,ss3d,ts1d,ts2d,ts3d
        real ss1e,ss2e,ss3e,ts1e,ts2e,ts3e
        real sn1,sn2,sn3,tn1,tn2,tn3
        real an11,an21,an12,an22,f11,f21,f12,f22,b11,b21,b12,b22,dot1,
     & dot2,f31,f32,b31,b32
        real rad,fdot1,fdot2,fdot3,adet,a(0:1,0:1,0:1)
        real a11,a12,a21,a22,deti, stautau
        integer itype,bctype   ! problem type flag (NEW)
        real p(2,2),pe(2,2),dpdf(4,4),determ,du1y,du2y,du1x,du2x,du1s,
     & du2s,du1r,du2r
        real v1r,v1s,v2r,v2s,du(2,2),cpar(10)
        real du1,du2,du3,cdl,uEps,uNorm
        real err
        integer axis1,axis2,axis3
        real v1x,v2x,v3x,v1y,v2y,v3y,v1z,v2z,v3z
        real u1x,u2x,u3x,u1y,u2y,u3y,u1z,u2z,u3z
        real u1r,u2r,u3r,u1s,u2s,u3s,u1t,u2t,u3t
        real s11t,s12t,s13t,s21t,s22t,s23t,s31t,s32t,s33t
        real u1xe,u2xe,u1ye,u2ye,u1re,u2re,u1se,u2se
        real s11e,s12e,s13e
        real s21e,s22e,s23e
        real s31e,s32e,s33e
        real u1rr,u1rs,u1ss,u2rr,u2rs,u2ss,rxr,rxs,ryr,rys,sxr,sxs,syr,
     & sys
        real u1xx,u1xy,u1yy,u2xx,u2xy,u2yy
        real u1xxe,u1xye,u1yye,u2xxe,u2xye,u2yye
        real s11xe,s12xe,s21xe,s22xe,s11ye,s12ye,s21ye,s22ye
        real u1xpe,u1ype,u2xpe,u2ype,u1xme,u2xme,u1yme,u2yme,s11pe,
     & s12pe,s21pe,s22pe,s11me,s12me,s21me,s22me
        real u1xp,u2xp,u1yp,u2yp,u1xm,u2xm,u1ym,u2ym
        integer ier
        real anormi1,anormi2,coef11,coef21,coef12,coef22,alpha1,alpha2
        real dalpha11,dalpha12,dalpha21,dalpha22,fact,aa(4,4),bb(4)
        integer mc,icart
        real ds1,ds2
        integer iter,istop,itmax,ideriv
        real bmax,toler,u1x0,u2x0,u1y0,u2y0,u1r0,u2r0,u1s0,u2s0
        real alpha,dalpha,coef1,coef2
        logical setCornersWithTZ
c      logical newBCs     this flag is not needed anymore
        ! this flag determines whether the secondary tangent stress assignment is done (default should be .false. ??)
        logical assignTangentStress
        ! boundary conditions parameters
! define BC parameters for fortran routines
! boundary conditions
c123456789012345678901234567890123456789012345678901234567890123456789
      integer interpolation,displacementBC,tractionBC
      integer slipWall,symmetry,interfaceBC
      integer abcEM2,abcPML,abc3,abc4,abc5,rbcNonLocal,rbcLocal,lastBC
      integer dirichletBoundaryCondition
      parameter( interpolation=0,displacementBC=1,tractionBC=2)
      parameter( slipWall=3,symmetry=4 )
      parameter( interfaceBC=5,abcEM2=6,abcPML=7,abc3=8,abc4=9 )
      parameter( abc5=10,rbcNonLocal=11,rbcLocal=12 )
      parameter( dirichletBoundaryCondition=13 )
      parameter( lastBC=14 )
! define interfaceType values for fortran routines
      integer noInterface                     ! no interface conditions are imposed
      integer heatFluxInterface               ! [ T.n ] = g
      integer tractionInterface               ! [ n.tau ] = g
      integer tractionAndHeatFluxInterface
      parameter( noInterface=0, heatFluxInterface=1 )
      parameter( tractionInterface=2,tractionAndHeatFluxInterface=3 )
        integer rectangular,curvilinear
        parameter(rectangular=0,curvilinear=1)
        integer linearBoundaryCondition, nonLinearBoundaryCondition
        parameter(linearBoundaryCondition=0,
     & nonLinearBoundaryCondition=1 )
c     --- start statement function ----
        real bcf
        integer kd,m,n
        real rhopc,mupc,lambdapc, rhov,muv,lambdav
        ! Here is the the generic boundary condition forcing array. It uses the bcOffset(side,axis) values as an
        ! an offset from the bcf0 array to access the bcf10, bcf01, bcf11, ... arrays
        bcf(side,axis,i1,i2,i3,m) = bcf0(bcOffset(side,axis) + (i1-dim(
     & 0,0,side,axis)+(dim(1,0,side,axis)-dim(0,0,side,axis)+1)* (i2-
     & dim(0,1,side,axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)* (
     & i3-dim(0,2,side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)
     & *(m)))))
c     --- statement functions for variable material parameters
        ! (rho,mu,lambda) for materialFormat=piecewiseConstantMaterialProperties
        rhopc(i1,i2)    = matValpc( 0, matIndex(i1,i2))
        mupc(i1,i2)     = matValpc( 1, matIndex(i1,i2))
        lambdapc(i1,i2) = matValpc( 2, matIndex(i1,i2))
        ! (rho,mu,lambda) for materialFormat=variableMaterialProperties
        rhov(i1,i2)    = matVal(i1,i2,0)
        muv(i1,i2)     = matVal(i1,i2,1)
        lambdav(i1,i2) = matVal(i1,i2,2)
c............... end statement functions
c       write(6,*)'bcs',(ipar(i1),i1=0,10),(rpar(i2),i2=0,12)
c       pause
c      mc=8
c      n1a=gridIndexRange(0,0)
c      n1b=gridIndexRange(1,0)
c      n2a=gridIndexRange(0,1)
c      n2b=gridIndexRange(1,1)
c      do i2=nd2a,nd2b
c      do i1=nd1a,nd1b
c        write(1,321)i1,i2,(u(i1,i2,nd3a,i3),i3=0,7)
c  321   format(2(1x,i3),8(1x,1pe10.3))
c      end do
c      end do
c      pause
c************** Setting parameters for local Newton iteration **************
        itmax=10
        toler=1.e-5
c***************************************************************************
        ierr=0
        nd                   =ipar(0)
        grid                 =ipar(1)
        uc                   =ipar(2)
        vc                   =ipar(3)
        wc                   =ipar(4)
        gridType             =ipar(5)
        orderOfAccuracy      =ipar(6)
        orderOfExtrapolation =ipar(7)
        twilightZone         =ipar(8)
        useWhereMask         =ipar(9)
        debug                =ipar(10)
        itype                =ipar(11)   ! =0 for linear elasticity
                                         ! =1 for SVK code with linear reduction
                                         ! =2 for SVK code with full SVK model
                                         ! =3 for SVK code with rotated linear model
c      write(6,*)'itype=',itype
c      pause
c      if (itype.eq.0.or.itype.eq.1) then
c        bctype=0
c      elseif (itype.eq.2) then
c        bctype=1
c      else
c        write(6,*)'Error (bcOptSmFOS) : invalid value for itype'
c        stop 2431
c      end if
        if (itype.eq.0.or.itype.eq.1) then
          bctype=linearBoundaryCondition       ! linear elasticity
        else
          bctype=nonLinearBoundaryCondition    ! not assumed to be linear
        end if
        applyInterfaceBoundaryConditions=ipar(12)
        projectInterface     =ipar(13)
        numToPin             =ipar(14)
        materialFormat       =ipar(15)
        assignTangentStress  =.false.  ! new option *dws* added 2015/07/13
        dx(0)                =rpar(0)
        dx(1)                =rpar(1)
        dx(2)                =rpar(2)
        dr(0)                =rpar(3)
        dr(1)                =rpar(4)
        dr(2)                =rpar(5)
        t                    =rpar(6)
        ep                   =rpar(7) ! pointer for exact solution
        dt                   =rpar(8)
        mu                   =rpar(9)
        lambda               =rpar(10)
        c1                   =rpar(11)
        c2                   =rpar(12)
        kappa                = lambda+2.0*mu
c New constitutive parameters array for smgetdp
        cpar(1)=lambda
        cpar(2)=mu
        epsx=1.e-20 ! for aNormi -- fix me ---
c       write(6,*)'bcs'
c       pause
        ! debug = 15 ! *** turn on temporarily ***
c*************** Setting parameters for limited extrapolation ****************
        cdl=2.
        if (twilightZone.ne.0) then
          cdl=0.
        end if
        uEps=1.e-4
c****************************************************************************
        axis1=0  ! *wdh*
        axis2=1
        axis3=2
        ! *wdh* 
        setCornersWithTZ = .false. ! *wdh* use this to turn off temporary fixes at corners in 3D
        ! look up the component numbers for the velocity and stress from the C++ data base
         ok=getInt(pdb,'v1c',v1c)
         if( ok.eq.0 )then
           write(*,'("*** bcOptSmFOS:ERROR: unable to find v1c")')
           stop 1122
         end if
         ok=getInt(pdb,'v2c',v2c)
         if( ok.eq.0 )then
           write(*,'("*** bcOptSmFOS:ERROR: unable to find v2c")')
           stop 1122
         end if
         ok=getInt(pdb,'v3c',v3c)
         if( ok.eq.0 )then
           write(*,'("*** bcOptSmFOS:ERROR: unable to find v3c")')
           stop 1122
         end if
         ok=getInt(pdb,'s11c',s11c)
         if( ok.eq.0 )then
           write(*,'("*** bcOptSmFOS:ERROR: unable to find s11c")')
           stop 1122
         end if
         ok=getInt(pdb,'s12c',s12c)
         if( ok.eq.0 )then
           write(*,'("*** bcOptSmFOS:ERROR: unable to find s12c")')
           stop 1122
         end if
         ok=getInt(pdb,'s13c',s13c)
         if( ok.eq.0 )then
           write(*,'("*** bcOptSmFOS:ERROR: unable to find s13c")')
           stop 1122
         end if
         ok=getInt(pdb,'s21c',s21c)
         if( ok.eq.0 )then
           write(*,'("*** bcOptSmFOS:ERROR: unable to find s21c")')
           stop 1122
         end if
         ok=getInt(pdb,'s22c',s22c)
         if( ok.eq.0 )then
           write(*,'("*** bcOptSmFOS:ERROR: unable to find s22c")')
           stop 1122
         end if
         ok=getInt(pdb,'s23c',s23c)
         if( ok.eq.0 )then
           write(*,'("*** bcOptSmFOS:ERROR: unable to find s23c")')
           stop 1122
         end if
         ok=getInt(pdb,'s31c',s31c)
         if( ok.eq.0 )then
           write(*,'("*** bcOptSmFOS:ERROR: unable to find s31c")')
           stop 1122
         end if
         ok=getInt(pdb,'s32c',s32c)
         if( ok.eq.0 )then
           write(*,'("*** bcOptSmFOS:ERROR: unable to find s32c")')
           stop 1122
         end if
         ok=getInt(pdb,'s33c',s33c)
         if( ok.eq.0 )then
           write(*,'("*** bcOptSmFOS:ERROR: unable to find s33c")')
           stop 1122
         end if
        if (materialFormat.ne.constantMaterialProperties) then
           ok=getInt(pdb,'rhoc',rhoc)
           if( ok.eq.0 )then
             write(*,'("*** bcOptSmFOS:ERROR: unable to find rhoc")')
             stop 1122
           end if
           ok=getInt(pdb,'muc',muc)
           if( ok.eq.0 )then
             write(*,'("*** bcOptSmFOS:ERROR: unable to find muc")')
             stop 1122
           end if
           ok=getInt(pdb,'lambdac',lambdac)
           if( ok.eq.0 )then
             write(*,'("*** bcOptSmFOS:ERROR: unable to find lambdac")
     & ')
             stop 1122
           end if
        end if
  ! Don's new BCs for the linear case.  The idea is to fix the symmetry issue with the stress tensor.
  ! If the new BCs case is implemented correctly, then the stress tensor will be symmetric after the
  ! bcs are applied assuming the stress tensor is symmetric before the bcs are applied.
c      newBCs=.true.   No longer using the newBCs flag.  The "newbcs" are now hardwired into the code.
  ! newBCs is set to .false. for twilight zone flow and for the SVK case...
c      if (twilightZone.ne.0 .or. bctype.ne.0) then
c        newBCs=.false.
c      end if
        sc(1,1) = s11c
        sc(2,1) = s21c
        sc(3,1) = s31c
        sc(1,2) = s12c
        sc(2,2) = s22c
        sc(3,2) = s32c
        sc(1,3) = s13c
        sc(2,3) = s23c
        sc(3,3) = s33c
        delta(1,1) = 1
        delta(1,2) = 0
        delta(1,3) = 0
        delta(2,1) = 0
        delta(2,2) = 1
        delta(2,3) = 0
        delta(3,1) = 0
        delta(3,2) = 0
        delta(3,3) = 1
         ok=getReal(pdb,'rho',rho)
         if( ok.eq.0 )then
           write(*,'("*** bcOptSmFOS:ERROR: unable to find rho")')
           stop 1133
         end if
        if( debug.gt.2 )then
          write(*,'(" bcOptSmFOS: grid=",i5," 
     & applyInterfaceBoundaryConditions=",i2," numToPin=",i3)') grid,
     & applyInterfaceBoundaryConditions,numToPin
        end if
        if( debug.gt.3 )then
          write(*,'(" bcOptSmFOS: mu,lambda,rho,c1,c2=",5f10.5," 
     & gridType=",i2)') mu,lambda,rho,c1,c2,gridType
             ! '
        end if
        if( debug.gt.7 )then
          write(*,'(" bcOptSmFOS: **START** grid=",i4," uc,vc,wc=",3i2)
     & ') grid,uc,vc,wc
             ! '
        end if
        if( debug.gt.7 )then
         n1a=gridIndexRange(0,0)
         n1b=gridIndexRange(1,0)
         n2a=gridIndexRange(0,1)
         n2b=gridIndexRange(1,1)
         n3a=gridIndexRange(0,2)
         n3b=gridIndexRange(1,2)
         write(*,'(" bcOptSmFOS: grid=",i3,",n1a,n1b,n2a,n2b,n3a,n3b=",
     & 6i3)') grid,n1a,n1b,n2a,n2b,n3a,n3b
          ! ' 
         ! write(*,*) 'bcOptSmFOS: u=',((((u(i1,i2,i3,m),m=0,nd-1),i1=n1a,n1b),i2=n2a,n2b),i3=n3a,n3b)
        end if
c      write(6,*)'bctype = ',bctype
c      pause
          n1a=gridIndexRange(0,0)
          n1b=gridIndexRange(1,0)
          n2a=gridIndexRange(0,1)
          n2b=gridIndexRange(1,1)
        if( .false. )then
          ! *** Don: call your BC routine here ****
          mc=8
          n1a=gridIndexRange(0,0)
          n1b=gridIndexRange(1,0)
          n2a=gridIndexRange(0,1)
          n2b=gridIndexRange(1,1)
          ds1=dr(0)
          ds2=dr(1)
          if ( bctype.eq.linearBoundaryCondition ) then
            if( materialFormat.eq.constantMaterialProperties )then
              call smgbcsn (mc,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,ds1,
     & ds2,t,xy,u,boundaryCondition)
            else
              call smgbcsn (mc,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,ds1,
     & ds2,t,xy,u,boundaryCondition)
            end if
          else
c          write(6,*)'hello'
c          pause
            call smgbcsn (mc,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,ds1,
     & ds2,t,xy,u,boundaryCondition)
c          call smgbcst (mc,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,ds1,ds2,rx,u)
          end if
c        return
        end if
c      if( materialFormat.ne.constantMaterialProperties )then
c        write(*,'(" ***bcOptSmFOS:ERROR: Finish me for variable material")')
c        stop 6645
c      end if
        numGhost=orderOfAccuracy/2
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (S11,S12,S21,S22)
c            u(j1,j2,7:8)=displacement
c      write(*,'(" -------- bcOptSmFOS: use new BCs orderOfExtrapolation=",i3)') orderOfExtrapolation
c      ! ' 
        numberOfComponents= nd + nd + nd*nd ! displacement, velocity and stress components
        ! if( nd.eq.2 )then
        ! else if( nd.eq.3 )then
         !    *************************
         !    ********** 3D ***********
         !    *************************
c*******
c******* Fill in forcing arrays if they are not provided ***********
c*******
       extra1a=numGhost
       extra1b=numGhost
       extra2a=numGhost
       extra2b=numGhost
       if( nd.eq.3 )then
         extra3a=numGhost
         extra3b=numGhost
       else
         extra3a=0
         extra3b=0
       end if
       if( boundaryCondition(0,0).lt.0 )then
         extra1a=max(0,extra1a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
       else if( boundaryCondition(0,0).eq.0 )then
         extra1a=numGhost  ! include interpolation points since we assign ghost points outside these
       end if
       ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
       if( boundaryCondition(1,0).lt.0 )then
         extra1b=max(0,extra1b) ! over-ride numGhost=-1 : assign ends in periodic directions
       else if( boundaryCondition(1,0).eq.0 )then
         extra1b=numGhost
       end if
       if( boundaryCondition(0,1).lt.0 )then
         extra2a=max(0,extra2a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
       else if( boundaryCondition(0,1).eq.0 )then
         extra2a=numGhost  ! include interpolation points since we assign ghost points outside these
       end if
       ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
       if( boundaryCondition(1,1).lt.0 )then
         extra2b=max(0,extra2b) ! over-ride numGhost=-1 : assign ends in periodic directions
       else if( boundaryCondition(1,1).eq.0 )then
         extra2b=numGhost
       end if
       if(  nd.eq.3 )then
        if( boundaryCondition(0,2).lt.0 )then
          extra3a=max(0,extra3a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
        else if( boundaryCondition(0,2).eq.0 )then
          extra3a=numGhost  ! include interpolation points since we assign ghost points outside these
        end if
        ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
        if( boundaryCondition(1,2).lt.0 )then
          extra3b=max(0,extra3b) ! over-ride numGhost=-1 : assign ends in periodic directions
        else if( boundaryCondition(1,2).eq.0 )then
          extra3b=numGhost
        end if
       end if
       do axis=0,nd-1
       do side=0,1
         if( boundaryCondition(side,axis).gt.0 )then
           ! write(*,'(" bcOpt: side,axis,bc=",3i2)') side,axis,boundaryCondition(side,axis)
           n1a=gridIndexRange(0,0)
           n1b=gridIndexRange(1,0)
           n2a=gridIndexRange(0,1)
           n2b=gridIndexRange(1,1)
           n3a=gridIndexRange(0,2)
           n3b=gridIndexRange(1,2)
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
           nn1a=gridIndexRange(0,0)-extra1a
           nn1b=gridIndexRange(1,0)+extra1b
           nn2a=gridIndexRange(0,1)-extra2a
           nn2b=gridIndexRange(1,1)+extra2b
           nn3a=gridIndexRange(0,2)-extra3a
           nn3b=gridIndexRange(1,2)+extra3b
           if( axis.eq.0 )then
             nn1a=gridIndexRange(side,axis)
             nn1b=gridIndexRange(side,axis)
           else if( axis.eq.1 )then
             nn2a=gridIndexRange(side,axis)
             nn2b=gridIndexRange(side,axis)
           else
             nn3a=gridIndexRange(side,axis)
             nn3b=gridIndexRange(side,axis)
           end if
           is=1-2*side
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
           i3=n3a
      !*      ! (js1,js2,js3) used to compute tangential derivatives
      !*      js1=0
      !*      js2=0
      !*      js3=0
      !*      if( axisp1.eq.0 )then
      !*        js1=1-2*side
      !*      else if( axisp1.eq.1 )then
      !*        js2=1-2*side
      !*      else if( axisp1.eq.2 )then
      !*        js3=1-2*side
      !*      else
      !*        stop 5
      !*      end if
      !* 
      !*      ! (ks1,ks2,ks3) used to compute second tangential derivative
      !*      ks1=0
      !*      ks2=0
      !*      ks3=0
      !*      if( axisp2.eq.0 )then
      !*        ks1=1-2*side
      !*      else if( axisp2.eq.1 )then
      !*        ks2=1-2*side
      !*      else if( axisp2.eq.2 )then
      !*        ks3=1-2*side
      !*      else
      !*        stop 5
      !*      end if
           if( debug.gt.7 )then
             write(*,'(" bcOpt: grid,side,axis=",3i3,", loop bounds: 
     & n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,n1a,n1b,n2a,
     & n2b,n3a,n3b
           end if
         end if ! if bc>0
        if( boundaryCondition(side,axis).eq.displacementBC )then
          if( addBoundaryForcing(side,axis).eq.0 ) then
             do i3=nn3a,nn3b
             do i2=nn2a,nn2b
             do i1=nn1a,nn1b
               if( mask(i1,i2,i3).ne.0 )then
                ! given displacements
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(uc))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(vc))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(wc))))) = 0.0
                ! given velocities
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v1c))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v2c))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v3c))))) = 0.0
                ! given acceleration
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s13c))))) = 0.0
             end if
             end do
             end do
             end do
          else if( twilightZone.ne.0 ) then
             do i3=nn3a,nn3b
             do i2=nn2a,nn2b
             do i1=nn1a,nn1b
               if( mask(i1,i2,i3).ne.0 )then
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,uc,ue )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,vc,ve )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,wc,we )

                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v1c,v1e )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v2c,v2e )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v3c,v3e )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s11c,tau11x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s21c,tau21y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s31c,tau31z )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s12c,tau12x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s22c,tau22y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s32c,tau32z )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s13c,tau13x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s23c,tau23y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s33c,tau33z )

                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(uc))))) = ue
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(vc))))) = ve
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(wc))))) = we

                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v1c))))) = v1e
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v2c))))) = v2e
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v3c))))) = v3e

                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c))))) = tau11x+
     & tau21y+tau31z
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c))))) = tau12x+
     & tau22y+tau32z
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s13c))))) = tau13x+
     & tau23y+tau33z
             end if
             end do
             end do
             end do
          end if
        else if( boundaryCondition(side,axis).eq.tractionBC ) then
          if( addBoundaryForcing(side,axis).eq.0 )then
             do i3=nn3a,nn3b
             do i2=nn2a,nn2b
             do i1=nn1a,nn1b
               if( mask(i1,i2,i3).ne.0 )then
                ! given traction (for the traction BC)
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s13c))))) = 0.0

                ! given traction (for determining displacements). Normally this is equal to the above
                ! traction values except when using twilight-zone
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(uc))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(vc))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(wc))))) = 0.0

                ! given rate of change of traction (for determining the velocity)
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v1c))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v2c))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v3c))))) = 0.0
             end if
             end do
             end do
             end do
          else if( twilightZone.ne.0 )then
             do i3=nn3a,nn3b
             do i2=nn2a,nn2b
             do i1=nn1a,nn1b
               if( mask(i1,i2,i3).ne.0 )then
                ! (an1,an2,an3) = outward normal
                if( gridType.eq.rectangular )then
                  if( axis.eq.0 )then
                    an1 = -is
                    an2 = 0.0
                    an3 = 0.0
                  else if( axis.eq.1 ) then
                    an1 = 0.0
                    an2 = -is
                    an3 = 0.0
                  else
                    an1 = 0.0
                    an2 = 0.0
                    an3 = -is
                  end if
                else
                  aNormi = 1.0/max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                  an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                  an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                  an3 = -is*rx(i1,i2,i3,axis,2)*aNormi
                end if
                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,uc,u1x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,uc,u1y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,uc,u1z )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,vc,u2x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,vc,u2y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,vc,u2z )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,wc,u3x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,wc,u3y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,wc,u3z )

                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(uc))))) = an1*( 
     & kappa*u1x+lambda*(u2y+u3z) ) + an2*( mu*(u2x+u1y) )            
     &    + an3*( mu*(u3x+u1z) )
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(vc))))) = an1*( mu*(
     & u2x+u1y) )               + an2*( kappa*u2y+lambda*(u1x+u3z) ) +
     &  an3*( mu*(u3y+u2z) )
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(wc))))) = an1*( mu*(
     & u3x+u1z) )               + an2*( mu*(u3y+u2z) )               +
     &  an3*( kappa*u3z+lambda*(u1x+u2y) )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v1c,v1x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v1c,v1y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v1c,v1z )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v2c,v2x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v2c,v2y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v2c,v2z )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v3c,v3x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v3c,v3y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v3c,v3z )

                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v1c))))) = an1*( 
     & kappa*v1x+lambda*(v2y+v3z) ) + an2*( mu*(v2x+v1y) )            
     &     + an3*( mu*(v3x+v1z) )
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v2c))))) = an1*( mu*(
     & v2x+v1y) )               + an2*( kappa*v2y+lambda*(v1x+v3z) )  
     & + an3*( mu*(v3y+v2z) )
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v3c))))) = an1*( mu*(
     & v3x+v1z) )               + an2*( mu*(v3y+v2z) )                
     & + an3*( kappa*v3z+lambda*(v1x+v2y) )

                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s11c,tau11 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s21c,tau21 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s31c,tau31 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s12c,tau12 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s22c,tau22 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s32c,tau32 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s13c,tau13 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s23c,tau23 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s33c,tau33 )

                ! note : n_j sigma_ji  : sum over first index 
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c))))) = an1*
     & tau11+an2*tau21+an3*tau31
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c))))) = an1*
     & tau12+an2*tau22+an3*tau32
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s13c))))) = an1*
     & tau13+an2*tau23+an3*tau33

             end if
             end do
             end do
             end do

          else
            ! fill in the traction BC into the stress components  
            ! (this is needed since for TZ flow these values are different)
             do i3=nn3a,nn3b
             do i2=nn2a,nn2b
             do i1=nn1a,nn1b
               if( mask(i1,i2,i3).ne.0 )then
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c))))) = bcf0(
     & bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-
     & dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(1,1,side,
     & axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(dim(1,2,
     & side,axis)-dim(0,2,side,axis)+1)*(uc)))))
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c))))) = bcf0(
     & bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-
     & dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(1,1,side,
     & axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(dim(1,2,
     & side,axis)-dim(0,2,side,axis)+1)*(vc)))))
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s13c))))) = bcf0(
     & bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-
     & dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(1,1,side,
     & axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(dim(1,2,
     & side,axis)-dim(0,2,side,axis)+1)*(wc)))))
             end if
             end do
             end do
             end do
          end if

        else if( boundaryCondition(side,axis).eq.slipWall ) then
          if( addBoundaryForcing(side,axis).eq.0 ) then
             do i3=nn3a,nn3b
             do i2=nn2a,nn2b
             do i1=nn1a,nn1b
               if( mask(i1,i2,i3).ne.0 )then
                !! check these components with Bill ... FIX ME!! ...
                ! given tangential stresses (often zero)
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c))))) = 0.0

                ! time rate of change of tangential stresses
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s21c))))) = 0.0
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s22c))))) = 0.0

                ! given normal displacement
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(uc))))) = 0.0

                ! time rate of change of normal displacement
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v1c))))) = 0.0
             end if
             end do
             end do
             end do
          else if( twilightZone.ne.0 ) then
             do i3=nn3a,nn3b
             do i2=nn2a,nn2b
             do i1=nn1a,nn1b
               if( mask(i1,i2,i3).ne.0 )then
                ! (an1,an2,an3) = outward normal
                if( gridType.eq.rectangular ) then
                  an1 = 0.0
                  an2 = 0.0
                  an3 = 0.0

                  sn1 = 0.0
                  sn2 = 0.0
                  sn3 = 0.0

                  tn1 = 0.0
                  tn2 = 0.0
                  tn3 = 0.0
                  if( axis.eq.0 ) then
                    an1 = -is
                    sn2 = -is
                    tn3 = -is
                  else if( axis.eq.1 ) then
                    an2 = -is
                    sn1 = -is
                    tn3 = -is
                  else
                    an3 = -is
                    sn1 = -is
                    tn2 = -is
                  end if
                else
                  if( axis.eq.0 ) then
                    tan1c = 1
                    tan2c = 2
                  else if( axis.eq.1 ) then
                    tan1c = 0
                    tan2c = 2
                  else
                    tan1c = 0
                    tan2c = 1
                  end if
                  aNormi = 1.0/max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                  an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                  an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                  an3 = -is*rx(i1,i2,i3,axis,2)*aNormi

                  sn1 = rx(i1,i2,i3,tan1c,0)
                  sn2 = rx(i1,i2,i3,tan1c,1)
                  sn3 = rx(i1,i2,i3,tan1c,2)

                  tn1 = rx(i1,i2,i3,tan2c,0)
                  tn2 = rx(i1,i2,i3,tan2c,1)
                  tn3 = rx(i1,i2,i3,tan2c,2)

                  ! set sn to be part of sn which is orthogonal to an
                  alpha = an1*sn1+an2*sn2+an3*sn3
                  sn1 = sn1-alpha*an1
                  sn2 = sn2-alpha*an2
                  sn3 = sn3-alpha*an3
                  ! normalize sn
                  aNormi = 1.0/max(epsx,sqrt(sn1**2+sn2**2+sn3**2))
                  sn1 = sn1*aNormi
                  sn2 = sn2*aNormi
                  sn3 = sn3*aNormi

                  ! set tn to be part of tn which is orthogonal to an and sn
                  alpha = an1*tn1+an2*tn2+an3*tn3
                  tn1 = tn1-alpha*an1
                  tn2 = tn2-alpha*an2
                  tn3 = tn3-alpha*an3
                  alpha = sn1*tn1+sn2*tn2+sn3*tn3
                  tn1 = tn1-alpha*sn1
                  tn2 = tn2-alpha*sn2
                  tn3 = tn3-alpha*sn3
                  ! normalize tn
                  aNormi = 1.0/max(epsx,sqrt(tn1**2+tn2**2+tn3**2))
                  tn1 = tn1*aNormi
                  tn2 = tn2*aNormi
                  tn3 = tn3*aNormi
                end if

                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,uc,ue)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,vc,ve)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,wc,we)

                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(uc))))) = an1*ue+an2*
     & ve+an3*we

                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v1c,ue)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v2c,ve)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v3c,we)

                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v1c))))) = an1*ue+
     & an2*ve+an3*we

                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s11c,tau11)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s21c,tau21)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s31c,tau31)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s12c,tau12)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s22c,tau22)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s32c,tau32)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s13c,tau13)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s23c,tau23)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s33c,tau33)

                ! check indicies ... FIX ME!! ...
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c))))) = an1*(sn1*
     & tau11+sn2*tau12+sn3*tau13)+ an2*(sn1*tau21+sn2*tau22+sn3*tau23)
     & + an3*(sn1*tau31+sn2*tau32+sn3*tau33)
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c))))) = an1*(tn1*
     & tau11+tn2*tau12+tn3*tau13)+ an2*(tn1*tau21+tn2*tau22+tn3*tau23)
     & + an3*(tn1*tau31+tn2*tau32+tn3*tau33)

                call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,uc,u1x)
                call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,uc,u1y)
                call ogDeriv(ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,uc,u1z)
                call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,vc,u2x)
                call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,vc,u2y)
                call ogDeriv(ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,vc,u2z)
                call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,wc,u3x)
                call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,wc,u3y)
                call ogDeriv(ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,wc,u3z)

                tau11 = kappa*u1x+lambda*(u2y+u3z)
                tau21 = mu*(u2x+u1y)
                tau31 = mu*(u3x+u1z)
                tau12 = tau21
                tau22 = kappa*u2y+lambda*(u1x+u3z)
                tau32 = mu*(u3y+u2z)
                tau13 = tau31
                tau23 = tau32
                tau33 = kappa*u3z+lambda*(u1x+u2y)
ccccccccccccccccccccccccccccccccccccccccccccccccccc

                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s21c))))) = an1*(-
     & an2*tau11+an1*tau12)+an2*(-an2*tau21+an1*tau22)

                call ogDeriv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s11c,tau11)
                call ogDeriv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s21c,tau21)
                call ogDeriv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s12c,tau12)
                call ogDeriv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s22c,tau22)

                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c))))) = an1*(-
     & an2*tau11+an1*tau12)+an2*(-an2*tau21+an1*tau22)

                call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v1c,v1ex)
                call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v1c,v1ey)
                call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v2c,v2ex)
                call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,v2c,v2ey)

                tau11=(lambda+2.*mu)*v1ex+lambda*v2ey
                tau12=mu*(v1ey+v2ex)
                tau21=tau12
                tau22=lambda*v1ex+(lambda+2.*mu)*v2ey
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s22c))))) = an1*(-
     & an2*tau11+an1*tau12)+an2*(-an2*tau21+an1*tau22)

             end if
             end do
             end do
             end do
          else
            ! fill in the traction BC into the stress components  *wdh* 081109
            ! (this is needed since for TZ flow these values are different)
             do i3=nn3a,nn3b
             do i2=nn2a,nn2b
             do i1=nn1a,nn1b
               if( mask(i1,i2,i3).ne.0 )then
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c))))) = bcf0(
     & bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-
     & dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(1,1,side,
     & axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(dim(1,2,
     & side,axis)-dim(0,2,side,axis)+1)*(uc)))))
                bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,
     & 0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(
     & 1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(
     & dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c))))) = bcf0(
     & bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-
     & dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(1,1,side,
     & axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(dim(1,2,
     & side,axis)-dim(0,2,side,axis)+1)*(vc)))))
             end if
             end do
             end do
             end do
          end if

        else if( boundaryCondition(side,axis).gt.0 .and. 
     & boundaryCondition(side,axis).ne.dirichletBoundaryCondition ) 
     & then
        write(*,'("smg3d:BC: unknown BC: side,axis,grid, 
     & boundaryCondition=",i2,i2,i4,i8)') side,axis,grid,
     & boundaryCondition(side,axis)
        end if
       end do ! end side
       end do ! end axis

c*******
c******* Primary Dirichlet boundary conditions ***********
c*******
       extra1a=numGhost
       extra1b=numGhost
       extra2a=numGhost
       extra2b=numGhost
       if( nd.eq.3 )then
         extra3a=numGhost
         extra3b=numGhost
       else
         extra3a=0
         extra3b=0
       end if
       if( boundaryCondition(0,0).lt.0 )then
         extra1a=max(0,extra1a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
       else if( boundaryCondition(0,0).eq.0 )then
         extra1a=numGhost  ! include interpolation points since we assign ghost points outside these
       end if
       ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
       if( boundaryCondition(1,0).lt.0 )then
         extra1b=max(0,extra1b) ! over-ride numGhost=-1 : assign ends in periodic directions
       else if( boundaryCondition(1,0).eq.0 )then
         extra1b=numGhost
       end if
       if( boundaryCondition(0,1).lt.0 )then
         extra2a=max(0,extra2a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
       else if( boundaryCondition(0,1).eq.0 )then
         extra2a=numGhost  ! include interpolation points since we assign ghost points outside these
       end if
       ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
       if( boundaryCondition(1,1).lt.0 )then
         extra2b=max(0,extra2b) ! over-ride numGhost=-1 : assign ends in periodic directions
       else if( boundaryCondition(1,1).eq.0 )then
         extra2b=numGhost
       end if
       if(  nd.eq.3 )then
        if( boundaryCondition(0,2).lt.0 )then
          extra3a=max(0,extra3a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
        else if( boundaryCondition(0,2).eq.0 )then
          extra3a=numGhost  ! include interpolation points since we assign ghost points outside these
        end if
        ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
        if( boundaryCondition(1,2).lt.0 )then
          extra3b=max(0,extra3b) ! over-ride numGhost=-1 : assign ends in periodic directions
        else if( boundaryCondition(1,2).eq.0 )then
          extra3b=numGhost
        end if
       end if
       do axis=0,nd-1
       do side=0,1
         if( boundaryCondition(side,axis).gt.0 )then
           ! write(*,'(" bcOpt: side,axis,bc=",3i2)') side,axis,boundaryCondition(side,axis)
           n1a=gridIndexRange(0,0)
           n1b=gridIndexRange(1,0)
           n2a=gridIndexRange(0,1)
           n2b=gridIndexRange(1,1)
           n3a=gridIndexRange(0,2)
           n3b=gridIndexRange(1,2)
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
           nn1a=gridIndexRange(0,0)-extra1a
           nn1b=gridIndexRange(1,0)+extra1b
           nn2a=gridIndexRange(0,1)-extra2a
           nn2b=gridIndexRange(1,1)+extra2b
           nn3a=gridIndexRange(0,2)-extra3a
           nn3b=gridIndexRange(1,2)+extra3b
           if( axis.eq.0 )then
             nn1a=gridIndexRange(side,axis)
             nn1b=gridIndexRange(side,axis)
           else if( axis.eq.1 )then
             nn2a=gridIndexRange(side,axis)
             nn2b=gridIndexRange(side,axis)
           else
             nn3a=gridIndexRange(side,axis)
             nn3b=gridIndexRange(side,axis)
           end if
           is=1-2*side
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
           i3=n3a
      !*      ! (js1,js2,js3) used to compute tangential derivatives
      !*      js1=0
      !*      js2=0
      !*      js3=0
      !*      if( axisp1.eq.0 )then
      !*        js1=1-2*side
      !*      else if( axisp1.eq.1 )then
      !*        js2=1-2*side
      !*      else if( axisp1.eq.2 )then
      !*        js3=1-2*side
      !*      else
      !*        stop 5
      !*      end if
      !* 
      !*      ! (ks1,ks2,ks3) used to compute second tangential derivative
      !*      ks1=0
      !*      ks2=0
      !*      ks3=0
      !*      if( axisp2.eq.0 )then
      !*        ks1=1-2*side
      !*      else if( axisp2.eq.1 )then
      !*        ks2=1-2*side
      !*      else if( axisp2.eq.2 )then
      !*        ks3=1-2*side
      !*      else
      !*        stop 5
      !*      end if
           if( debug.gt.7 )then
             write(*,'(" bcOpt: grid,side,axis=",3i3,", loop bounds: 
     & n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,n1a,n1b,n2a,
     & n2b,n3a,n3b
           end if
         end if ! if bc>0
        if( boundaryCondition(side,axis).eq.displacementBC )then
          ! ..step 0: Dirichlet bcs for displacement and velocity
           do i3=nn3a,nn3b
           do i2=nn2a,nn2b
           do i1=nn1a,nn1b
             if( mask(i1,i2,i3).ne.0 )then
              ! given displacements
              u(i1,i2,i3,uc)  = bcf(side,axis,i1,i2,i3,uc)
              u(i1,i2,i3,vc)  = bcf(side,axis,i1,i2,i3,vc)
              u(i1,i2,i3,wc)  = bcf(side,axis,i1,i2,i3,wc)

              ! given velocities
              u(i1,i2,i3,v1c) = bcf(side,axis,i1,i2,i3,v1c)
              u(i1,i2,i3,v2c) = bcf(side,axis,i1,i2,i3,v2c)
              u(i1,i2,i3,v3c) = bcf(side,axis,i1,i2,i3,v3c)
           end if
           end do
           end do
           end do
        else if( boundaryCondition(side,axis).eq.tractionBC )then
          ! dirichlet portion of traction BC
          if( gridType.eq.rectangular ) then
            if( axis.eq.0 )then
               do i3=nn3a,nn3b
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
                 if( mask(i1,i2,i3).ne.0 )then
                  ! set normal components of the stress, n=(-is,0,0)
                  u(i1,i2,i3,s11c) = -is*bcf(side,axis,i1,i2,i3,s11c)
                  u(i1,i2,i3,s12c) = -is*bcf(side,axis,i1,i2,i3,s12c)
                  u(i1,i2,i3,s13c) = -is*bcf(side,axis,i1,i2,i3,s13c)
               end if
               end do
               end do
               end do
            else if( axis.eq.1 ) then
               do i3=nn3a,nn3b
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
                 if( mask(i1,i2,i3).ne.0 )then
                  ! set normal components of the stress, n=(0,-is,0)
                  u(i1,i2,i3,s21c) = -is*bcf(side,axis,i1,i2,i3,s11c)
                  u(i1,i2,i3,s22c) = -is*bcf(side,axis,i1,i2,i3,s12c)
                  u(i1,i2,i3,s23c) = -is*bcf(side,axis,i1,i2,i3,s13c)
               end if
               end do
               end do
               end do
            else
               do i3=nn3a,nn3b
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
                 if( mask(i1,i2,i3).ne.0 )then
                  ! set normal components of the stress, n=(0,0,-is)
                  u(i1,i2,i3,s31c) = -is*bcf(side,axis,i1,i2,i3,s11c)
                  u(i1,i2,i3,s32c) = -is*bcf(side,axis,i1,i2,i3,s12c)
                  u(i1,i2,i3,s33c) = -is*bcf(side,axis,i1,i2,i3,s13c)
               end if
               end do
               end do
               end do
            end if

          else ! curvilinear
             do i3=nn3a,nn3b
             do i2=nn2a,nn2b
             do i1=nn1a,nn1b
               if( mask(i1,i2,i3).ne.0 )then
                f1 = bcf(side,axis,i1,i2,i3,s11c)
                f2 = bcf(side,axis,i1,i2,i3,s12c)
                f3 = bcf(side,axis,i1,i2,i3,s13c)

                ! (an1,an2,an3) = outward normal 
                aNormi = 1.0/max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                an3 = -is*rx(i1,i2,i3,axis,2)*aNormi

                b1 = f1-(an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,i3,s21c)+an3*
     & u(i1,i2,i3,s31c))
                b2 = f2-(an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,i3,s22c)+an3*
     & u(i1,i2,i3,s32c))
                b3 = f3-(an1*u(i1,i2,i3,s13c)+an2*u(i1,i2,i3,s23c)+an3*
     & u(i1,i2,i3,s33c))

                u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)+an1*b1
                u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)+an1*b2
                u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)+an1*b3

                u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)+an2*b1
                u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)+an2*b2
                u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)+an2*b3

                u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)+an3*b1
                u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)+an3*b2
                u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)+an3*b3
             end if
             end do
             end do
             end do
          end if ! grid type

        else if( boundaryCondition(side,axis).eq.slipWall ) then
           ! ********* SlipWall BC ********
           ! set "dirichlet" parts of the slipwall BC
          if( gridType.eq.rectangular ) then
            if( axis.eq.0 ) then
               do i3=nn3a,nn3b
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
                 if( mask(i1,i2,i3).ne.0 )then
                  ! set n.tau.t and the normal component of displacement, n=(-is,0,0)
                  u(i1,i2,i3,s12c) = bcf(side,axis,i1,i2,i3,s11c)
                  u(i1,i2,i3,s13c) = bcf(side,axis,i1,i2,i3,s12c)
                  u(i1,i2,i3,uc)   = -is*bcf(side,axis,i1,i2,i3,uc)
                  u(i1,i2,i3,v1c)  = -is*bcf(side,axis,i1,i2,i3,v1c)
               end if
               end do
               end do
               end do
            else if( axis.eq.1 ) then
               do i3=nn3a,nn3b
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
                 if( mask(i1,i2,i3).ne.0 )then
                  ! set n.tau.t and the normal component of displacement, n=(-is,0,0)
                  u(i1,i2,i3,s21c) = bcf(side,axis,i1,i2,i3,s11c)
                  u(i1,i2,i3,s23c) = bcf(side,axis,i1,i2,i3,s12c)
                  u(i1,i2,i3,vc)   = -is*bcf(side,axis,i1,i2,i3,uc)
                  u(i1,i2,i3,v2c)  = -is*bcf(side,axis,i1,i2,i3,v1c)
               end if
               end do
               end do
               end do
            else
               do i3=nn3a,nn3b
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
                 if( mask(i1,i2,i3).ne.0 )then
                  ! set n.tau.t and the normal component of displacement, n=(-is,0,0)
                  u(i1,i2,i3,s31c) = bcf(side,axis,i1,i2,i3,s11c)
                  u(i1,i2,i3,s32c) = bcf(side,axis,i1,i2,i3,s12c)
                  u(i1,i2,i3,wc)   = -is*bcf(side,axis,i1,i2,i3,uc)
                  u(i1,i2,i3,v3c)  = -is*bcf(side,axis,i1,i2,i3,v1c)
               end if
               end do
               end do
               end do
            end if

          else  ! curvilinear
             do i3=nn3a,nn3b
             do i2=nn2a,nn2b
             do i1=nn1a,nn1b
               if( mask(i1,i2,i3).ne.0 )then
                ! given tangential traction forces
                f1 = bcf(side,axis,i1,i2,i3,s11c)
                f2 = bcf(side,axis,i1,i2,i3,s12c)

                ! (an1,an2,an3) = outward normal 
                if( axis.eq.0 ) then
                  tan1c = 1
                  tan2c = 2
                else if( axis.eq.1 ) then
                  tan1c = 0
                  tan2c = 2
                else
                  tan1c = 0
                  tan2c = 1
                end if
                aNormi = 1.0/max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                an3 = -is*rx(i1,i2,i3,axis,2)*aNormi

                sn1 = rx(i1,i2,i3,tan1c,0)
                sn2 = rx(i1,i2,i3,tan1c,1)
                sn3 = rx(i1,i2,i3,tan1c,2)

                tn1 = rx(i1,i2,i3,tan2c,0)
                tn2 = rx(i1,i2,i3,tan2c,1)
                tn3 = rx(i1,i2,i3,tan2c,2)

                ! set sn to be part of sn which is orthogonal to an
                alpha = an1*sn1+an2*sn2+an3*sn3
                sn1 = sn1-alpha*an1
                sn2 = sn2-alpha*an2
                sn3 = sn3-alpha*an3
                ! normalize sn
                aNormi = 1.0/max(epsx,sqrt(sn1**2+sn2**2+sn3**2))
                sn1 = sn1*aNormi
                sn2 = sn2*aNormi
                sn3 = sn3*aNormi

                ! set tn to be part of tn which is orthogonal to an and sn
                alpha = an1*tn1+an2*tn2+an3*tn3
                tn1 = tn1-alpha*an1
                tn2 = tn2-alpha*an2
                tn3 = tn3-alpha*an3
                alpha = sn1*tn1+sn2*tn2+sn3*tn3
                tn1 = tn1-alpha*sn1
                tn2 = tn2-alpha*sn2
                tn3 = tn3-alpha*sn3
                ! normalize tn
                aNormi = 1.0/max(epsx,sqrt(tn1**2+tn2**2+tn3**2))
                tn1 = tn1*aNormi
                tn2 = tn2*aNormi
                tn3 = tn3*aNormi

                b1 = f1-an1*(u(i1,i2,i3,s11c)*sn1+u(i1,i2,i3,s12c)*sn2+
     & u(i1,i2,i3,s13c)*sn3)- an2*(u(i1,i2,i3,s21c)*sn1+u(i1,i2,i3,
     & s22c)*sn2+u(i1,i2,i3,s23c)*sn3)- an3*(u(i1,i2,i3,s31c)*sn1+u(
     & i1,i2,i3,s32c)*sn2+u(i1,i2,i3,s33c)*sn3)
                b2 = f2-an1*(u(i1,i2,i3,s11c)*tn1+u(i1,i2,i3,s12c)*tn2+
     & u(i1,i2,i3,s13c)*tn3)- an2*(u(i1,i2,i3,s21c)*tn1+u(i1,i2,i3,
     & s22c)*tn2+u(i1,i2,i3,s23c)*tn3)- an3*(u(i1,i2,i3,s31c)*tn1+u(
     & i1,i2,i3,s32c)*tn2+u(i1,i2,i3,s33c)*tn3)


                u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)+an1*b1*sn1+an1*b2*
     & tn1
                u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)+an1*b1*sn2+an1*b2*
     & tn2
                u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)+an1*b1*sn3+an1*b2*
     & tn3

                u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)+an2*b1*sn1+an2*b2*
     & tn1
                u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)+an2*b1*sn2+an2*b2*
     & tn2
                u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)+an2*b1*sn3+an2*b2*
     & tn3

                u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)+an3*b1*sn1+an3*b2*
     & tn1
                u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)+an3*b1*sn2+an3*b2*
     & tn2
                u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)+an3*b1*sn3+an3*b2*
     & tn3


                ! given normal displacement
                f1 = bcf(side,axis,i1,i2,i3,uc)

                ! given normal velocity
                f2 = bcf(side,axis,i1,i2,i3,v1c)

                b1 = f1-an1*u(i1,i2,i3,uc)-an2*u(i1,i2,i3,vc)-an3*u(i1,
     & i2,i3,wc)
                b2 = f2-an1*u(i1,i2,i3,v1c)-an2*u(i1,i2,i3,v2c)-an3*u(
     & i1,i2,i3,v3c)

                u(i1,i2,i3,uc) = u(i1,i2,i3,uc)+an1*b1
                u(i1,i2,i3,vc) = u(i1,i2,i3,vc)+an2*b1
                u(i1,i2,i3,wc) = u(i1,i2,i3,wc)+an3*b1

                u(i1,i2,i3,v1c) = u(i1,i2,i3,v1c)+an1*b2
                u(i1,i2,i3,v2c) = u(i1,i2,i3,v2c)+an2*b2
                u(i1,i2,i3,v3c) = u(i1,i2,i3,v3c)+an3*b2
             end if
             end do
             end do
             end do

          end if  ! end gridType

        else if( boundaryCondition(side,axis).gt.0 .and. 
     & boundaryCondition(side,axis).ne.dirichletBoundaryCondition ) 
     & then
        write(*,'("smg3d:BC: unknown BC: side,axis,grid, 
     & boundaryCondition=",i2,i2,i4,i8)') side,axis,grid,
     & boundaryCondition(side,axis)

        end if ! bc type
       end do ! end side
       end do ! end axis

c*******
c******* Extrapolate to the first ghost cells (only for physical sides) ********
c*******
      ! *wdh* For now assign 2 ghost lines and points outside edges and corners
      extra = 2
      numGhostExtrap=2
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
         extra1a=numGhostExtrap  ! include interpolation points since we assign ghost points outside these
       end if
       ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
       if( boundaryCondition(1,0).lt.0 )then
         extra1b=max(0,extra1b) ! over-ride extra=-1 : assign ends in periodic directions
       else if( boundaryCondition(1,0).eq.0 )then
         extra1b=numGhostExtrap
       end if
       if( boundaryCondition(0,1).lt.0 )then
         extra2a=max(0,extra2a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
       else if( boundaryCondition(0,1).eq.0 )then
         extra2a=numGhostExtrap  ! include interpolation points since we assign ghost points outside these
       end if
       ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
       if( boundaryCondition(1,1).lt.0 )then
         extra2b=max(0,extra2b) ! over-ride extra=-1 : assign ends in periodic directions
       else if( boundaryCondition(1,1).eq.0 )then
         extra2b=numGhostExtrap
       end if
       if(  nd.eq.3 )then
        if( boundaryCondition(0,2).lt.0 )then
          extra3a=max(0,extra3a) ! over-ride extra=-1 : assign ends in periodic directions (or internal parallel boundaries)
        else if( boundaryCondition(0,2).eq.0 )then
          extra3a=numGhostExtrap  ! include interpolation points since we assign ghost points outside these
        end if
        ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
        if( boundaryCondition(1,2).lt.0 )then
          extra3b=max(0,extra3b) ! over-ride extra=-1 : assign ends in periodic directions
        else if( boundaryCondition(1,2).eq.0 )then
          extra3b=numGhostExtrap
        end if
       end if
       do axis=0,nd-1
       do side=0,1
         if( boundaryCondition(side,axis).gt.0 )then
           ! write(*,'(" bcOpt: side,axis,bc=",3i2)') side,axis,boundaryCondition(side,axis)
           n1a=gridIndexRange(0,0)
           n1b=gridIndexRange(1,0)
           n2a=gridIndexRange(0,1)
           n2b=gridIndexRange(1,1)
           n3a=gridIndexRange(0,2)
           n3b=gridIndexRange(1,2)
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
           nn1a=gridIndexRange(0,0)-extra1a
           nn1b=gridIndexRange(1,0)+extra1b
           nn2a=gridIndexRange(0,1)-extra2a
           nn2b=gridIndexRange(1,1)+extra2b
           nn3a=gridIndexRange(0,2)-extra3a
           nn3b=gridIndexRange(1,2)+extra3b
           if( axis.eq.0 )then
             nn1a=gridIndexRange(side,axis)
             nn1b=gridIndexRange(side,axis)
           else if( axis.eq.1 )then
             nn2a=gridIndexRange(side,axis)
             nn2b=gridIndexRange(side,axis)
           else
             nn3a=gridIndexRange(side,axis)
             nn3b=gridIndexRange(side,axis)
           end if
           is=1-2*side
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
           i3=n3a
      !*      ! (js1,js2,js3) used to compute tangential derivatives
      !*      js1=0
      !*      js2=0
      !*      js3=0
      !*      if( axisp1.eq.0 )then
      !*        js1=1-2*side
      !*      else if( axisp1.eq.1 )then
      !*        js2=1-2*side
      !*      else if( axisp1.eq.2 )then
      !*        js3=1-2*side
      !*      else
      !*        stop 5
      !*      end if
      !* 
      !*      ! (ks1,ks2,ks3) used to compute second tangential derivative
      !*      ks1=0
      !*      ks2=0
      !*      ks3=0
      !*      if( axisp2.eq.0 )then
      !*        ks1=1-2*side
      !*      else if( axisp2.eq.1 )then
      !*        ks2=1-2*side
      !*      else if( axisp2.eq.2 )then
      !*        ks3=1-2*side
      !*      else
      !*        stop 5
      !*      end if
           if( debug.gt.7 )then
             write(*,'(" bcOpt: grid,side,axis=",3i3,", loop bounds: 
     & n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,n1a,n1b,n2a,
     & n2b,n3a,n3b
           end if
         end if ! if bc>0
        if( boundaryCondition(side,axis).gt.0.and.boundaryCondition(
     & side,axis).ne.dirichletBoundaryCondition )then

       if( .false. )then
         write(*,'(" bcOpt: Extrap ghost: grid,side,axis=",3i3,", loop 
     & bounds: nn1a,nn1b,nn2a,nn2b,nn3a,nn3b=",6i3)') grid,side,axis,
     & nn1a,nn1b,nn2a,nn2b,nn3a,nn3b

       end if

           do i3=nn3a,nn3b
           do i2=nn2a,nn2b
           do i1=nn1a,nn1b
          if( mask(i1,i2,i3).ne.0 ) then
              do n=0,numberOfComponents-1
                u(i1-is1,i2-is2,i3-is3,n)=(3.*u(i1,i2,i3,n)-3.*u(i1+
     & is1,i2+is2,i3+is3,n)+u(i1+2*is1,i2+2*is2,i3+2*is3,n))
                u(i1-2*is1,i2-2*is2,i3-2*is3,n)=(3.*u(i1-is1,i2-is2,i3-
     & is3,n)-3.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,n)+u(i1-is1+2*is1,
     & i2-is2+2*is2,i3-is3+2*is3,n))
              end do
            end if
           end do
           end do
           end do
        end if
       end do ! end side
       end do ! end axis

c*******
c******* Fix up components of stress along the edges
c*******
c*******
c******* Fix up components of stress along the edges
c*******
      if( .false. )then ! *wdh* 090910 -- turn this off for now  --------------------

        write(*,'(" bcOptSmFOS3DEdge: do NOT apply edge fixup, grid,
     & gridType=",i4,i2)')  grid,gridType

      else

       ! write(*,'(" bcOptSmFOS3DEdge: DO apply edge fixup, grid,gridType=",i4,i2)')  grid,gridType 

            do edgeDirection = 0,2 ! direction parallel to the edge
              do sidea = 0,1
              do sideb = 0,1
                if( edgeDirection.eq.0 ) then
                  side1 = 0
                  side2 = sidea
                  side3 = sideb
                else if( edgeDirection.eq.1 ) then
                  side1 = sideb
                  side2 = 0
                  side3 = sidea
                else
                  side1 = sidea
                  side2 = sideb
                  side3 = 0
                end if
                is1 = 1-2*(side1)
                is2 = 1-2*(side2)
                is3 = 1-2*(side3)
                if( edgeDirection.eq.2 ) then
                  is3 = 0
                  n1a = gridIndexRange(side1,0)
                  n1b = gridIndexRange(side1,0)
                  n2a = gridIndexRange(side2,1)
                  n2b = gridIndexRange(side2,1)
                  n3a = gridIndexRange(    0,2)
                  n3b = gridIndexRange(    1,2)
                  bc1 = boundaryCondition(side1,0)
                  bc2 = boundaryCondition(side2,1)
                else if( edgeDirection.eq.1 )then
                  is2 = 0
                  n1a = gridIndexRange(side1,0)
                  n1b = gridIndexRange(side1,0)
                  n2a = gridIndexRange(    0,1)
                  n2b = gridIndexRange(    1,1)
                  n3a = gridIndexRange(side3,2)
                  n3b = gridIndexRange(side3,2)
                  bc1 = boundaryCondition(side1,0)
                  bc2 = boundaryCondition(side3,2)
                else
                  is1 = 0
                  n1a = gridIndexRange(    0,0)
                  n1b = gridIndexRange(    1,0)
                  n2a = gridIndexRange(side2,1)
                  n2b = gridIndexRange(side2,1)
                  n3a = gridIndexRange(side3,2)
                  n3b = gridIndexRange(side3,2)
                  bc1 = boundaryCondition(side2,1)
                  bc2 = boundaryCondition(side3,2)
                end if
        if( bc1.eq.displacementBC .and. bc2.eq.displacementBC ) then
          if( gridType.eq.rectangular ) then
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then
c              if( mask(i1,i2,i3).gt.0 ) then
c                write(6,*)i1,i2,i3
                u1x = ((u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(0)))
                u1y = ((u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(1)))
                u1z = ((u(i1,i2,i3+1,uc)-u(i1,i2,i3-1,uc))/(2.0*dx(2)))

                u2x = ((u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(0)))
                u2y = ((u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(1)))
                u2z = ((u(i1,i2,i3+1,vc)-u(i1,i2,i3-1,vc))/(2.0*dx(2)))

                u3x = ((u(i1+1,i2,i3,wc)-u(i1-1,i2,i3,wc))/(2.0*dx(0)))
                u3y = ((u(i1,i2+1,i3,wc)-u(i1,i2-1,i3,wc))/(2.0*dx(1)))
                u3z = ((u(i1,i2,i3+1,wc)-u(i1,i2,i3-1,wc))/(2.0*dx(2)))

                u(i1,i2,i3,s11c) = kappa*u1x+lambda*(u2y+u3z)
                u(i1,i2,i3,s21c) = mu*(u2x+u1y)
                u(i1,i2,i3,s31c) = mu*(u3x+u1z)
                u(i1,i2,i3,s12c) = mu*(u2x+u1y)
                u(i1,i2,i3,s22c) = kappa*u2y+lambda*(u1x+u3z)
                u(i1,i2,i3,s32c) = mu*(u3y+u2z)
                u(i1,i2,i3,s13c) = mu*(u3x+u1z)
                u(i1,i2,i3,s23c) = mu*(u3y+u2z)
                u(i1,i2,i3,s33c) = kappa*u3z+lambda*(u1x+u2y)
c              end if
             end if
             end do
             end do
             end do
          else
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then
c              if( mask(i1,i2,i3).gt.0 ) then
                u1r = ((u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(0)))
                u1s = ((u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(1)))
                u1t = ((u(i1,i2,i3+1,uc)-u(i1,i2,i3-1,uc))/(2.0*dr(2)))

                u2r = ((u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(0)))
                u2s = ((u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(1)))
                u2t = ((u(i1,i2,i3+1,vc)-u(i1,i2,i3-1,vc))/(2.0*dr(2)))

                u3r = ((u(i1+1,i2,i3,wc)-u(i1-1,i2,i3,wc))/(2.0*dr(0)))
                u3s = ((u(i1,i2+1,i3,wc)-u(i1,i2-1,i3,wc))/(2.0*dr(1)))
                u3t = ((u(i1,i2,i3+1,wc)-u(i1,i2,i3-1,wc))/(2.0*dr(2)))

                u1x = u1r*rx(i1,i2,i3,0,0)+u1s*rx(i1,i2,i3,1,0)+u1t*rx(
     & i1,i2,i3,2,0)
                u1y = u1r*rx(i1,i2,i3,0,1)+u1s*rx(i1,i2,i3,1,1)+u1t*rx(
     & i1,i2,i3,2,1)
                u1z = u1r*rx(i1,i2,i3,0,2)+u1s*rx(i1,i2,i3,1,2)+u1t*rx(
     & i1,i2,i3,2,2)

                u2x = u2r*rx(i1,i2,i3,0,0)+u2s*rx(i1,i2,i3,1,0)+u2t*rx(
     & i1,i2,i3,2,0)
                u2y = u2r*rx(i1,i2,i3,0,1)+u2s*rx(i1,i2,i3,1,1)+u2t*rx(
     & i1,i2,i3,2,1)
                u2z = u2r*rx(i1,i2,i3,0,2)+u2s*rx(i1,i2,i3,1,2)+u2t*rx(
     & i1,i2,i3,2,2)

                u3x = u3r*rx(i1,i2,i3,0,0)+u3s*rx(i1,i2,i3,1,0)+u3t*rx(
     & i1,i2,i3,2,0)
                u3y = u3r*rx(i1,i2,i3,0,1)+u3s*rx(i1,i2,i3,1,1)+u3t*rx(
     & i1,i2,i3,2,1)
                u3z = u3r*rx(i1,i2,i3,0,2)+u3s*rx(i1,i2,i3,1,2)+u3t*rx(
     & i1,i2,i3,2,2)

                u(i1,i2,i3,s11c) = kappa*u1x+lambda*(u2y+u3z)
                u(i1,i2,i3,s21c) = mu*(u2x+u1y)
                u(i1,i2,i3,s31c) = mu*(u3x+u1z)
                u(i1,i2,i3,s12c) = mu*(u2x+u1y)
                u(i1,i2,i3,s22c) = kappa*u2y+lambda*(u1x+u3z)
                u(i1,i2,i3,s32c) = mu*(u3y+u2z)
                u(i1,i2,i3,s13c) = mu*(u3x+u1z)
                u(i1,i2,i3,s23c) = mu*(u3y+u2z)
                u(i1,i2,i3,s33c) = kappa*u3z+lambda*(u1x+u2y)
c              end if
             end if
             end do
             end do
             end do
          end if ! gridType

	  ! *wdh* we need to adjust for TZ here (not in a separate edgeMacro loop for then
	  ! corner points are corrected twice)
          if( twilightZone.ne.0 ) then
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
            if( mask(i1,i2,i3).gt.0 )then
c            if( mask(i1,i2,i3).gt.0 ) then
              call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,uc,u1x )
              call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,uc,u1y )
              call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,uc,u1z )

              call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,vc,u2x )
              call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,vc,u2y )
              call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,vc,u2z )

              call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,wc,u3x )
              call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,wc,u3y )
              call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,wc,u3z )

              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s11c,s11e )
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s12c,s12e )
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s13c,s13e )

              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s21c,s21e )
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s22c,s22e )
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s23c,s23e )

              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s31c,s31e )
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s32c,s32e )
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & xy(i1,i2,i3,2),t,s33c,s33e )

              tau11 = kappa*u1x+lambda*(u2y+u3z)
              tau21 = mu*(u2x+u1y)
              tau31 = mu*(u3x+u1z)
              tau12 = tau21
              tau22 = kappa*u2y+lambda*(u1x+u3z)
              tau32 = mu*(u3y+u2z)
              tau13 = tau31
              tau23 = tau32
              tau33 = kappa*u3z+lambda*(u1x+u2y)

              u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)-tau11+s11e
              u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)-tau21+s21e
              u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)-tau31+s31e
                                    
              u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)-tau12+s12e
              u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)-tau22+s22e
              u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)-tau32+s32e
                                    
              u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)-tau13+s13e
              u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)-tau23+s23e
              u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)-tau33+s33e

c              u(i1,i2,i3,s11c) = s11e
c              u(i1,i2,i3,s21c) = s21e
c              u(i1,i2,i3,s31c) = s31e
c              			 				      
c              u(i1,i2,i3,s12c) = s12e
c              u(i1,i2,i3,s22c) = s22e
c              u(i1,i2,i3,s32c) = s32e
c              			 				      
c              u(i1,i2,i3,s13c) = s13e
c              u(i1,i2,i3,s23c) = s23e
c              u(i1,i2,i3,s33c) = s33e
c            end if ! end if mask
            end if
            end do
            end do
            end do
        end if ! end if TZ


        else if( bc1.eq.tractionBC .and. bc2.eq.tractionBC ) then
          if( gridType.eq.rectangular ) then
            ! do nothing because normals are perpendicular and so no part of the force is counted twice
          else
            ! do stuff because normals are not perpendicular and some part of the stress might be counted twice
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then
              if( edgeDirection.eq.0 ) then
                norm1(0) = -is2*rx(i1,i2,i3,1,0)
                norm1(1) = -is2*rx(i1,i2,i3,1,1)
                norm1(2) = -is2*rx(i1,i2,i3,1,2)
                f11 = bcf(side2,1,i1,i2,i3,s11c)
                f21 = bcf(side2,1,i1,i2,i3,s12c)
                f31 = bcf(side2,1,i1,i2,i3,s13c)

                norm2(0) = -is3*rx(i1,i2,i3,2,0)
                norm2(1) = -is3*rx(i1,i2,i3,2,1)
                norm2(2) = -is3*rx(i1,i2,i3,2,2)
                f12 = bcf(side3,2,i1,i2,i3,s11c)
                f22 = bcf(side3,2,i1,i2,i3,s12c)
                f32 = bcf(side3,2,i1,i2,i3,s13c)
              else if( edgeDirection.eq.1 ) then
                norm1(0) = -is3*rx(i1,i2,i3,2,0)
                norm1(1) = -is3*rx(i1,i2,i3,2,1)
                norm1(2) = -is3*rx(i1,i2,i3,2,2)
                f11 = bcf(side3,2,i1,i2,i3,s11c)
                f21 = bcf(side3,2,i1,i2,i3,s12c)
                f31 = bcf(side3,2,i1,i2,i3,s13c)

                norm2(0) = -is1*rx(i1,i2,i3,0,0)
                norm2(1) = -is1*rx(i1,i2,i3,0,1)
                norm2(2) = -is1*rx(i1,i2,i3,0,2)
                f12 = bcf(side1,0,i1,i2,i3,s11c)
                f22 = bcf(side1,0,i1,i2,i3,s12c)
                f32 = bcf(side1,0,i1,i2,i3,s13c)
              else
                norm1(0) = -is1*rx(i1,i2,i3,0,0)
                norm1(1) = -is1*rx(i1,i2,i3,0,1)
                norm1(2) = -is1*rx(i1,i2,i3,0,2)
                f11 = bcf(side1,0,i1,i2,i3,s11c)
                f21 = bcf(side1,0,i1,i2,i3,s12c)
                f31 = bcf(side1,0,i1,i2,i3,s13c)

                norm2(0) = -is2*rx(i1,i2,i3,1,0)
                norm2(1) = -is2*rx(i1,i2,i3,1,1)
                norm2(2) = -is2*rx(i1,i2,i3,1,2)
                f12 = bcf(side2,1,i1,i2,i3,s11c)
                f22 = bcf(side2,1,i1,i2,i3,s12c)
                f32 = bcf(side2,1,i1,i2,i3,s13c)
              end if

              aNormi = 1.0/max(epsx,sqrt(norm1(0)**2+norm1(1)**2+norm1(
     & 2)**2))
              norm1(0) = norm1(0)*aNormi
              norm1(1) = norm1(1)*aNormi
              norm1(2) = norm1(2)*aNormi

              aNormi = 1.0/max(epsx,sqrt(norm2(0)**2+norm2(1)**2+norm2(
     & 2)**2))
              norm2(0) = norm2(0)*aNormi
              norm2(1) = norm2(1)*aNormi
              norm2(2) = norm2(2)*aNormi

              b11 = f11-(norm1(0)*u(i1,i2,i3,s11c)+norm1(1)*u(i1,i2,i3,
     & s21c)+norm1(2)*u(i1,i2,i3,s31c))
              b21 = f21-(norm1(0)*u(i1,i2,i3,s12c)+norm1(1)*u(i1,i2,i3,
     & s22c)+norm1(2)*u(i1,i2,i3,s32c))
              b31 = f31-(norm1(0)*u(i1,i2,i3,s13c)+norm1(1)*u(i1,i2,i3,
     & s23c)+norm1(2)*u(i1,i2,i3,s33c))

              dot1 = norm1(0)*norm2(0)+norm1(1)*norm2(1)+norm1(2)*
     & norm2(2)
              dot2 = -sin(acos(dot1))

              b12 = (f12-(norm2(0)*u(i1,i2,i3,s11c)+norm2(1)*u(i1,i2,
     & i3,s21c)+norm2(2)*u(i1,i2,i3,s31c))-dot1*b11)/dot2
              b22 = (f22-(norm2(0)*u(i1,i2,i3,s12c)+norm2(1)*u(i1,i2,
     & i3,s22c)+norm2(2)*u(i1,i2,i3,s32c))-dot1*b21)/dot2
              b32 = (f32-(norm2(0)*u(i1,i2,i3,s13c)+norm2(1)*u(i1,i2,
     & i3,s23c)+norm2(2)*u(i1,i2,i3,s33c))-dot1*b31)/dot2

              u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)+norm1(0)*b11+norm2(0)
     & *b12
              u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)+norm1(0)*b21+norm2(0)
     & *b22
              u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)+norm1(0)*b31+norm2(0)
     & *b32

              u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)+norm1(1)*b11+norm2(1)
     & *b12
              u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)+norm1(1)*b21+norm2(1)
     & *b22
              u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)+norm1(1)*b31+norm2(1)
     & *b32

              u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)+norm1(2)*b11+norm2(2)
     & *b12
              u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)+norm1(2)*b21+norm2(2)
     & *b22
              u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)+norm1(2)*b31+norm2(2)
     & *b32
             end if
             end do
             end do
             end do
          end if ! gridType

        end if ! bcType
              end do
              end do
            end do

      end if

cccccccccccccccccccccccccccccccccccccccccccccccccc
c .. set exact solution for corners for now
      if( setCornersWithTZ .and. twilightZone.ne.0 ) then ! *wdh* 090909
       write(*,'(" bcOptSmFOS3D: INFO set exact values on corners")')
            n1a = gridIndexRange(0,0)-0
            n1b = gridIndexRange(1,0)+0
            n2a = gridIndexRange(0,1)-0
            n2b = gridIndexRange(1,1)+0
            n3a = gridIndexRange(0,2)-0
            n3b = gridIndexRange(1,2)+0
c
            stride1 = n1b-n1a
            stride2 = n2b-n2a
            stride3 = n3b-n3a
c
            side1 = 0
            side2 = 1
c
            do i3 = n3a,n3b,stride3
            do i2 = n2a,n2b,stride2
            do i1 = n1a,n1b,stride1
              if( i1.eq.n1a ) then
                if( i2.eq.n2a ) then
                  if( i3.eq.n3a ) then
                    ! (0,0,0)
                    bc1 = boundaryCondition( side1,axis1 )
                    bc2 = boundaryCondition( side1,axis2 )
                    bc3 = boundaryCondition( side1,axis3 )
                  else
                    ! (0,0,1)
                    bc1 = boundaryCondition( side1,axis1 )
                    bc2 = boundaryCondition( side1,axis2 )
                    bc3 = boundaryCondition( side2,axis3 )
                  end if
                else
                  if( i3.eq.n3a ) then
                    ! (0,1,0)
                    bc1 = boundaryCondition( side1,axis1 )
                    bc2 = boundaryCondition( side2,axis2 )
                    bc3 = boundaryCondition( side1,axis3 )
                  else
                    ! (0,1,1)
                    bc1 = boundaryCondition( side1,axis1 )
                    bc2 = boundaryCondition( side2,axis2 )
                    bc3 = boundaryCondition( side2,axis3 )
                  end if
                end if
              else
                if( i2.eq.n2a ) then
                  if( i3.eq.n3a ) then
                    ! (1,0,0)
                    bc1 = boundaryCondition( side2,axis1 )
                    bc2 = boundaryCondition( side1,axis2 )
                    bc3 = boundaryCondition( side1,axis3 )
                  else
                    ! (1,0,1)
                    bc1 = boundaryCondition( side2,axis1 )
                    bc2 = boundaryCondition( side1,axis2 )
                    bc3 = boundaryCondition( side2,axis3 )
                  end if
                else
                  if( i3.eq.n3a ) then
                    ! (1,1,0)
                    bc1 = boundaryCondition( side2,axis1 )
                    bc2 = boundaryCondition( side2,axis2 )
                    bc3 = boundaryCondition( side1,axis3 )
                  else
                    ! (1,1,1)
                    bc1 = boundaryCondition( side2,axis1 )
                    bc2 = boundaryCondition( side2,axis2 )
                    bc3 = boundaryCondition( side2,axis3 )
                  end if
                end if
              end if
        if( mask(i1,i2,i3).gt.0 ) then
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,uc,ue )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,vc,ve )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,wc,we )

          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,v1c,v1e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,v2c,v2e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,v3c,v3e )

          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s11c,tau11e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s21c,tau21e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s31c,tau31e )

          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s12c,tau12e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s22c,tau22e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s32c,tau32e )

          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s13c,tau13e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s23c,tau23e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s33c,tau33e )

          u(i1,i2,i3,uc) = ue
          u(i1,i2,i3,vc) = ve
          u(i1,i2,i3,wc) = we

          u(i1,i2,i3,v1c) = v1e
          u(i1,i2,i3,v2c) = v2e
          u(i1,i2,i3,v3c) = v3e

          u(i1,i2,i3,s11c) = tau11e
          u(i1,i2,i3,s21c) = tau21e
          u(i1,i2,i3,s31c) = tau31e

          u(i1,i2,i3,s12c) = tau12e
          u(i1,i2,i3,s22c) = tau22e
          u(i1,i2,i3,s32c) = tau32e

          u(i1,i2,i3,s13c) = tau13e
          u(i1,i2,i3,s23c) = tau23e
          u(i1,i2,i3,s33c) = tau33e
        end if
            end do
            end do
            end do
      end if
cccccccccccccccccccccccccccccccccccccccccccccccccc

c*******
c******* Fix up components of stress in the corners 
c*******
c.. for now we are going to ignore this too

c*******
c******* Secondary Neumann boundary conditions (compatibility conditions) ********
c*******
       extra1a=numGhost
       extra1b=numGhost
       extra2a=numGhost
       extra2b=numGhost
       if( nd.eq.3 )then
         extra3a=numGhost
         extra3b=numGhost
       else
         extra3a=0
         extra3b=0
       end if
       if( boundaryCondition(0,0).lt.0 )then
         extra1a=max(0,extra1a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
       else if( boundaryCondition(0,0).eq.0 )then
         extra1a=numGhost  ! include interpolation points since we assign ghost points outside these
       end if
       ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
       if( boundaryCondition(1,0).lt.0 )then
         extra1b=max(0,extra1b) ! over-ride numGhost=-1 : assign ends in periodic directions
       else if( boundaryCondition(1,0).eq.0 )then
         extra1b=numGhost
       end if
       if( boundaryCondition(0,1).lt.0 )then
         extra2a=max(0,extra2a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
       else if( boundaryCondition(0,1).eq.0 )then
         extra2a=numGhost  ! include interpolation points since we assign ghost points outside these
       end if
       ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
       if( boundaryCondition(1,1).lt.0 )then
         extra2b=max(0,extra2b) ! over-ride numGhost=-1 : assign ends in periodic directions
       else if( boundaryCondition(1,1).eq.0 )then
         extra2b=numGhost
       end if
       if(  nd.eq.3 )then
        if( boundaryCondition(0,2).lt.0 )then
          extra3a=max(0,extra3a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
        else if( boundaryCondition(0,2).eq.0 )then
          extra3a=numGhost  ! include interpolation points since we assign ghost points outside these
        end if
        ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
        if( boundaryCondition(1,2).lt.0 )then
          extra3b=max(0,extra3b) ! over-ride numGhost=-1 : assign ends in periodic directions
        else if( boundaryCondition(1,2).eq.0 )then
          extra3b=numGhost
        end if
       end if
       do axis=0,nd-1
       do side=0,1
         if( boundaryCondition(side,axis).gt.0 )then
           ! write(*,'(" bcOpt: side,axis,bc=",3i2)') side,axis,boundaryCondition(side,axis)
           n1a=gridIndexRange(0,0)
           n1b=gridIndexRange(1,0)
           n2a=gridIndexRange(0,1)
           n2b=gridIndexRange(1,1)
           n3a=gridIndexRange(0,2)
           n3b=gridIndexRange(1,2)
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
           nn1a=gridIndexRange(0,0)-extra1a
           nn1b=gridIndexRange(1,0)+extra1b
           nn2a=gridIndexRange(0,1)-extra2a
           nn2b=gridIndexRange(1,1)+extra2b
           nn3a=gridIndexRange(0,2)-extra3a
           nn3b=gridIndexRange(1,2)+extra3b
           if( axis.eq.0 )then
             nn1a=gridIndexRange(side,axis)
             nn1b=gridIndexRange(side,axis)
           else if( axis.eq.1 )then
             nn2a=gridIndexRange(side,axis)
             nn2b=gridIndexRange(side,axis)
           else
             nn3a=gridIndexRange(side,axis)
             nn3b=gridIndexRange(side,axis)
           end if
           is=1-2*side
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
           i3=n3a
      !*      ! (js1,js2,js3) used to compute tangential derivatives
      !*      js1=0
      !*      js2=0
      !*      js3=0
      !*      if( axisp1.eq.0 )then
      !*        js1=1-2*side
      !*      else if( axisp1.eq.1 )then
      !*        js2=1-2*side
      !*      else if( axisp1.eq.2 )then
      !*        js3=1-2*side
      !*      else
      !*        stop 5
      !*      end if
      !* 
      !*      ! (ks1,ks2,ks3) used to compute second tangential derivative
      !*      ks1=0
      !*      ks2=0
      !*      ks3=0
      !*      if( axisp2.eq.0 )then
      !*        ks1=1-2*side
      !*      else if( axisp2.eq.1 )then
      !*        ks2=1-2*side
      !*      else if( axisp2.eq.2 )then
      !*        ks3=1-2*side
      !*      else
      !*        stop 5
      !*      end if
           if( debug.gt.7 )then
             write(*,'(" bcOpt: grid,side,axis=",3i3,", loop bounds: 
     & n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,n1a,n1b,n2a,
     & n2b,n3a,n3b
           end if
         end if ! if bc>0

        if( boundaryCondition(side,axis).eq.displacementBC ) then

          if( gridType.eq.rectangular ) then

            ! ********* DISPLACEMENT : Cartesian Grid **********

            ! Use momentum equations ... 
            !   s11_x + s21_y + s31_z = rho * u_tt  
            !   s12_x + s22_y + s32_z = rho * v_tt  
            !   s13_x + s23_y + s33_z = rho * w_tt  
            ! *wdh* 090909 -- only assign pts where mask > 0 since we assume values at adjacent points.
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then
                accel(1) = rho*bcf(side,axis,i1,i2,i3,s11c)
                accel(2) = rho*bcf(side,axis,i1,i2,i3,s12c)
                accel(3) = rho*bcf(side,axis,i1,i2,i3,s13c)

c                write(6,*)is1,is2,is3,is,axis
c                write(6,*)accel(1),accel(2),accel(3)

                do isc = 1,3
                  u(i1-is1,i2-is2,i3-is3,sc(axis+1,isc)) = u(i1+is1,i2+
     & is2,i3+is3,sc(axis+1,isc))-2.0*is*dx(axis)*(accel(isc)- (1.0-
     & delta(axis+1,1))*(u(i1+1,i2,i3,sc(1,isc))-u(i1-1,i2,i3,sc(1,
     & isc)))/(2.0*dx(0))- (1.0-delta(axis+1,2))*(u(i1,i2+1,i3,sc(2,
     & isc))-u(i1,i2-1,i3,sc(2,isc)))/(2.0*dx(1))- (1.0-delta(axis+1,
     & 3))*(u(i1,i2,i3+1,sc(3,isc))-u(i1,i2,i3-1,sc(3,isc)))/(2.0*dx(
     & 2)))
                end do
             end if
             end do
             end do
             end do
          else

            if( .false. ) then ! non-free stream preserving method
            ! *********** DISPLACEMENT : Curvilinear Grid (not free stream preserving) ****************

            ! Use momentum equations to get J*(rx,ry,rz).(s11,s21,s31)(-1) = s11tilde
            !    (1)   D_r1[ J*(rx,ry,rz).(s11,s21,s31)] + D_r2[J*(sx,sy,sz).(s11,s21,s31)] + D_r3[J*(tx,ty,tz).(s11,s21,s31)] = J * rho * u_tt  
            !                        s11tilde                         s21tilde                          s31tilde
            !    (2)   Use extrapolated values to get  J*(sx,sy,sz).(s11,s21,s31)(-1) = s21tilde
            !    (3)   Use extrapolated values to get  J*(tx,ty,tz).(s11,s21,s31)(-1) = s31tilde
            ! To give 3 equations for (s11,s21,s31) on the ghost point:
            !   (J rx) s11(-1) + (J ry) s21(-1) + (J rz) s31(-1) = f1 = s11tilde  (from momentum eqn)
            !   (J sx) s11(-1) + (J sy) s21(-1) + (J sz) s31(-1) = f2 = s21tilde  (from extrapolated values)
            !   (J tx) s11(-1) + (J ty) s21(-1) + (J tz) s31(-1) = f3 = s31tilde  (from extrapolated values)
            ! Solve: (note the Jacobian cancels when the matrix inversion is determined)
            !     s11(-1) = (sy*tz-sz*ty)*f1 + (rz*ty-ry*tz)*f2 + (ry*sz-rz*sy)*f3
            !     s21(-1) = (tx*sz-sx*tz)*f1 + (rx*tz-rz*tx)*f2 + (rz*sx-rx*sz)*f3
            !     s31(-1) = (sx*ty-sy*tx)*f1 + (ry*tx-rx*ty)*f2 + (rx*sy-ry*sx)*f3
            !
            ! (A similar expression holds for other stresses) 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then
                accel(1) = rho*bcf(side,axis,i1,i2,i3,s11c)
                accel(2) = rho*bcf(side,axis,i1,i2,i3,s12c)
                accel(3) = rho*bcf(side,axis,i1,i2,i3,s13c)

                met(0,0) = rx(i1,i2,i3,0,0)
                met(0,1) = rx(i1,i2,i3,0,1)
                met(0,2) = rx(i1,i2,i3,0,2)
                met(1,0) = rx(i1,i2,i3,1,0)
                met(1,1) = rx(i1,i2,i3,1,1)
                met(1,2) = rx(i1,i2,i3,1,2)
                met(2,0) = rx(i1,i2,i3,2,0)
                met(2,1) = rx(i1,i2,i3,2,1)
                met(2,2) = rx(i1,i2,i3,2,2)

                ! loop over stress components
                do isc = 1,3
                  do idot = 1,3
                    ! these are extrapolated components
                    stilde(idot) = det(i1-is1,i2-is2,i3-is3)*(rx(i1-
     & is1,i2-is2,i3-is3,idot-1,0)*u(i1-is1,i2-is2,i3-is3,sc(1,isc))+ 
     & rx(i1-is1,i2-is2,i3-is3,idot-1,1)*u(i1-is1,i2-is2,i3-is3,sc(2,
     & isc))+ rx(i1-is1,i2-is2,i3-is3,idot-1,2)*u(i1-is1,i2-is2,i3-
     & is3,sc(3,isc)))
                  end do
                  ! now override in the direction we are currently looking
                  stilde(axis+1) = det(i1+is1,i2+is2,i3+is3)*(rx(i1+
     & is1,i2+is2,i3+is3,axis,0)*u(i1+is1,i2+is2,i3+is3,sc(1,isc))+ 
     & rx(i1+is1,i2+is2,i3+is3,axis,1)*u(i1+is1,i2+is2,i3+is3,sc(2,
     & isc))+ rx(i1+is1,i2+is2,i3+is3,axis,2)*u(i1+is1,i2+is2,i3+is3,
     & sc(3,isc)))- 2.0*dr(axis)*is*(det(i1,i2,i3)*accel(isc)- (1.0-
     & delta(axis+1,1))* (det(i1+1,i2,i3)*(rx(i1+1,i2,i3,0,0)*u(i1+1,
     & i2,i3,sc(1,isc))+ rx(i1+1,i2,i3,0,1)*u(i1+1,i2,i3,sc(2,isc))+ 
     & rx(i1+1,i2,i3,0,2)*u(i1+1,i2,i3,sc(3,isc)))- det(i1-1,i2,i3)*(
     & rx(i1-1,i2,i3,0,0)*u(i1-1,i2,i3,sc(1,isc))+ rx(i1-1,i2,i3,0,1)*
     & u(i1-1,i2,i3,sc(2,isc))+ rx(i1-1,i2,i3,0,2)*u(i1-1,i2,i3,sc(3,
     & isc))))/(2.0*dr(0))- (1.0-delta(axis+1,2))* (det(i1,i2+1,i3)*(
     & rx(i1,i2+1,i3,1,0)*u(i1,i2+1,i3,sc(1,isc))+ rx(i1,i2+1,i3,1,1)*
     & u(i1,i2+1,i3,sc(2,isc))+ rx(i1,i2+1,i3,1,2)*u(i1,i2+1,i3,sc(3,
     & isc)))- det(i1,i2-1,i3)*(rx(i1,i2-1,i3,1,0)*u(i1,i2-1,i3,sc(1,
     & isc))+ rx(i1,i2-1,i3,1,1)*u(i1,i2-1,i3,sc(2,isc))+ rx(i1,i2-1,
     & i3,1,2)*u(i1,i2-1,i3,sc(3,isc))))/(2.0*dr(1))- (1.0-delta(axis+
     & 1,3))* (det(i1,i2,i3+1)*(rx(i1,i2,i3+1,2,0)*u(i1,i2,i3+1,sc(1,
     & isc))+ rx(i1,i2,i3+1,2,1)*u(i1,i2,i3+1,sc(2,isc))+ rx(i1,i2,i3+
     & 1,2,2)*u(i1,i2,i3+1,sc(3,isc)))- det(i1,i2,i3-1)*(rx(i1,i2,i3-
     & 1,2,0)*u(i1,i2,i3-1,sc(1,isc))+ rx(i1,i2,i3-1,2,1)*u(i1,i2,i3-
     & 1,sc(2,isc))+ rx(i1,i2,i3-1,2,2)*u(i1,i2,i3-1,sc(3,isc))))/(
     & 2.0*dr(2)))

                  u(i1-is1,i2-is2,i3-is3,sc(1,isc)) = (met(1,1)*met(2,
     & 2)-met(1,2)*met(2,1))*stilde(1)+ (met(0,2)*met(2,1)-met(0,1)*
     & met(2,2))*stilde(2)+ (met(0,1)*met(1,2)-met(0,2)*met(1,1))*
     & stilde(3)
                  u(i1-is1,i2-is2,i3-is3,sc(2,isc)) = (met(1,2)*met(2,
     & 0)-met(1,0)*met(2,2))*stilde(1)+ (met(0,0)*met(2,2)-met(0,2)*
     & met(2,0))*stilde(2)+ (met(0,2)*met(1,0)-met(0,0)*met(1,2))*
     & stilde(3)
                  u(i1-is1,i2-is2,i3-is3,sc(3,isc)) = (met(1,0)*met(2,
     & 1)-met(1,1)*met(2,0))*stilde(1)+ (met(0,1)*met(2,0)-met(0,0)*
     & met(2,1))*stilde(2)+ (met(0,0)*met(1,1)-met(0,1)*met(1,0))*
     & stilde(3)
                end do
             end if
             end do
             end do
             end do
            else ! free stream preserving method
cccccccccccc
            ! *********** DISPLACEMENT : Curvilinear Grid (free stream preserving) ****************

            ! Use momentum equations to get (rx,ry,rz)(0).(s11,s21,s31)(-1) = s11tilde
            !    (1)   (rx,ry,rz).D_r1[(s11,s21,s31)] + (sx,sy,sz).D_r2[(s11,s21,s31)] + (tx,ty,tz).D_r3[(s11,s21,s31)] = rho * u_tt  
            !                      s11tilde                         s21tilde                         s31tilde
            !    (2)   Use extrapolated values to get  (sx,sy,sz)(0).(s11,s21,s31)(-1) = s21tilde
            !    (3)   Use extrapolated values to get  (tx,ty,tz)(0).(s11,s21,s31)(-1) = s31tilde
            ! To give 3 equations for (s11,s21,s31) on the ghost point:
            !   (rx)(0) s11(-1) + (ry)(0) s21(-1) + (rz)(0) s31(-1) = f1 = s11tilde  (from momentum eqn)
            !   (sx)(0) s11(-1) + (sy)(0) s21(-1) + (sz)(0) s31(-1) = f2 = s21tilde  (from extrapolated values)
            !   (tx)(0) s11(-1) + (ty)(0) s21(-1) + (tz)(0) s31(-1) = f3 = s31tilde  (from extrapolated values)
            ! Solve: (note that det is the inverse of the determinant det[rx,ry,rz; sx,sy,sz; tx,ty,tz])
            !     s11(-1) = ((sy*tz-sz*ty)*f1 + (rz*ty-ry*tz)*f2 + (ry*sz-rz*sy)*f3)*det
            !     s21(-1) = ((tx*sz-sx*tz)*f1 + (rx*tz-rz*tx)*f2 + (rz*sx-rx*sz)*f3)*det
            !     s31(-1) = ((sx*ty-sy*tx)*f1 + (ry*tx-rx*ty)*f2 + (rx*sy-ry*sx)*f3)*det
            !
            ! (A similar expression holds for other stresses) 
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then
                accel(1) = rho*bcf(side,axis,i1,i2,i3,s11c)
                accel(2) = rho*bcf(side,axis,i1,i2,i3,s12c)
                accel(3) = rho*bcf(side,axis,i1,i2,i3,s13c)

                met(0,0) = rx(i1,i2,i3,0,0)
                met(0,1) = rx(i1,i2,i3,0,1)
                met(0,2) = rx(i1,i2,i3,0,2)
                met(1,0) = rx(i1,i2,i3,1,0)
                met(1,1) = rx(i1,i2,i3,1,1)
                met(1,2) = rx(i1,i2,i3,1,2)
                met(2,0) = rx(i1,i2,i3,2,0)
                met(2,1) = rx(i1,i2,i3,2,1)
                met(2,2) = rx(i1,i2,i3,2,2)

                ! loop over stress components
                do isc = 1,3
                  do idot = 1,3
                    ! these are extrapolated components
                    stilde(idot) = rx(i1,i2,i3,idot-1,0)*u(i1-is1,i2-
     & is2,i3-is3,sc(1,isc))+ rx(i1,i2,i3,idot-1,1)*u(i1-is1,i2-is2,
     & i3-is3,sc(2,isc))+ rx(i1,i2,i3,idot-1,2)*u(i1-is1,i2-is2,i3-
     & is3,sc(3,isc))
                  end do
                  ! now override in the direction we are currently looking
                  stilde(axis+1) = (rx(i1,i2,i3,axis,0)*u(i1+is1,i2+
     & is2,i3+is3,sc(1,isc))+ rx(i1,i2,i3,axis,1)*u(i1+is1,i2+is2,i3+
     & is3,sc(2,isc))+ rx(i1,i2,i3,axis,2)*u(i1+is1,i2+is2,i3+is3,sc(
     & 3,isc)))- 2.0*dr(axis)*is*(accel(isc)- (1.0-delta(axis+1,1))* (
     & rx(i1,i2,i3,0,0)*(u(i1+1,i2,i3,sc(1,isc))-u(i1-1,i2,i3,sc(1,
     & isc)))+ rx(i1,i2,i3,0,1)*(u(i1+1,i2,i3,sc(2,isc))-u(i1-1,i2,i3,
     & sc(2,isc)))+ rx(i1,i2,i3,0,2)*(u(i1+1,i2,i3,sc(3,isc))-u(i1-1,
     & i2,i3,sc(3,isc))))/(2.0*dr(0))- (1.0-delta(axis+1,2))* (rx(i1,
     & i2,i3,1,0)*(u(i1,i2+1,i3,sc(1,isc))-u(i1,i2-1,i3,sc(1,isc)))+ 
     & rx(i1,i2,i3,1,1)*(u(i1,i2+1,i3,sc(2,isc))-u(i1,i2-1,i3,sc(2,
     & isc)))+ rx(i1,i2,i3,1,2)*(u(i1,i2+1,i3,sc(3,isc))-u(i1,i2-1,i3,
     & sc(3,isc))))/(2.0*dr(1))- (1.0-delta(axis+1,3))* (rx(i1,i2,i3,
     & 2,0)*(u(i1,i2,i3+1,sc(1,isc))-u(i1,i2,i3-1,sc(1,isc)))+ rx(i1,
     & i2,i3,2,1)*(u(i1,i2,i3+1,sc(2,isc))-u(i1,i2,i3-1,sc(2,isc)))+ 
     & rx(i1,i2,i3,2,2)*(u(i1,i2,i3+1,sc(3,isc))-u(i1,i2,i3-1,sc(3,
     & isc))))/(2.0*dr(2)))

                  u(i1-is1,i2-is2,i3-is3,sc(1,isc)) = ((met(1,1)*met(2,
     & 2)-met(1,2)*met(2,1))*stilde(1)+ (met(0,2)*met(2,1)-met(0,1)*
     & met(2,2))*stilde(2)+ (met(0,1)*met(1,2)-met(0,2)*met(1,1))*
     & stilde(3))*det(i1,i2,i3)
                  u(i1-is1,i2-is2,i3-is3,sc(2,isc)) = ((met(1,2)*met(2,
     & 0)-met(1,0)*met(2,2))*stilde(1)+ (met(0,0)*met(2,2)-met(0,2)*
     & met(2,0))*stilde(2)+ (met(0,2)*met(1,0)-met(0,0)*met(1,2))*
     & stilde(3))*det(i1,i2,i3)
                  u(i1-is1,i2-is2,i3-is3,sc(3,isc)) = ((met(1,0)*met(2,
     & 1)-met(1,1)*met(2,0))*stilde(1)+ (met(0,1)*met(2,0)-met(0,0)*
     & met(2,1))*stilde(2)+ (met(0,0)*met(1,1)-met(0,1)*met(1,0))*
     & stilde(3))*det(i1,i2,i3)
                end do
             end if
             end do
             end do
             end do
cccccccccccc
            end if
          end if ! end gridType

        else if( boundaryCondition(side,axis).eq.tractionBC ) then
           ! **************** TRACTION : Neumann type conditions ******************
          if( gridType.eq.rectangular )then

            ! ********* TRACTION : Cartesian Grid **********

            ! Assign displacements on the ghost points from given tractions on the boundary
            !   s11 = kappa*u.x + lambda*( v.y + w.z )
            !   s22 = kappa*v.y + lambda*( u.x + w.z )
            !   s33 = kappa*w.z + lambda*( u.x + v.y )
            !   s12 = s21 = mu*( u.y + v.x )
            !   s13 = s31 = mu*( w.x + u.z )
            !
            !   an1*s11 + an2*s21 + an3*s31 = f1
            !   an1*s12 + an2*s22 + an3*s32 = f2
            !   an1*s13 + an2*s23 + an3*s33 = f3

            ! Assign velocities on the ghost points from given time derivatives of the tractions on the boundary
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then
                f1 = bcf(side,axis,i1,i2,i3,uc)
                f2 = bcf(side,axis,i1,i2,i3,vc)
                f3 = bcf(side,axis,i1,i2,i3,wc)

                if( axis.eq.0 )then
                  an1 = -is
                  an2 = 0.0
                  an3 = 0.0
                else if( axis.eq.1 ) then
                  an1 = 0.0
                  an2 = -is
                  an3 = 0.0
                else
                  an1 = 0.0
                  an2 = 0.0
                  an3 = -is
                end if

                dux = ((u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(0)))
     & *(1.0-delta(axis+1,1))
                duy = ((u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(1)))
     & *(1.0-delta(axis+1,2))
                duz = ((u(i1,i2,i3+1,uc)-u(i1,i2,i3-1,uc))/(2.0*dx(2)))
     & *(1.0-delta(axis+1,3))

                dvx = ((u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(0)))
     & *(1.0-delta(axis+1,1))
                dvy = ((u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(1)))
     & *(1.0-delta(axis+1,2))
                dvz = ((u(i1,i2,i3+1,vc)-u(i1,i2,i3-1,vc))/(2.0*dx(2)))
     & *(1.0-delta(axis+1,3))

                dwx = ((u(i1+1,i2,i3,wc)-u(i1-1,i2,i3,wc))/(2.0*dx(0)))
     & *(1.0-delta(axis+1,1))
                dwy = ((u(i1,i2+1,i3,wc)-u(i1,i2-1,i3,wc))/(2.0*dx(1)))
     & *(1.0-delta(axis+1,2))
                dwz = ((u(i1,i2,i3+1,wc)-u(i1,i2,i3-1,wc))/(2.0*dx(2)))
     & *(1.0-delta(axis+1,3))

                f1 = f1-an1*(kappa*dux+lambda*(dvy+dwz))- an2*(mu*(dvx+
     & duy))- an3*(mu*(dwx+duz))
                f2 = f2-an1*(mu*(dvx+duy))- an2*(kappa*dvy+lambda*(dux+
     & dwz))- an3*(mu*(dwy+dvz))
                f3 = f3-an1*(mu*(dwx+duz))- an2*(mu*(dwy+dvz))- an3*(
     & kappa*dwz+lambda*(dux+dvy))

                ! in the Cartesian case all that survives in the Matrix are the diagonal terms
                f1 = f1/(an1*kappa +an2*mu    +an3*mu)
                f2 = f2/(an1*mu    +an2*kappa +an3*mu)
                f3 = f3/(an1*mu    +an2*mu    +an3*kappa)

                u(i1-is1,i2-is2,i3-is3,uc) = -is*2.0*dx(axis)*f1+u(i1+
     & is1,i2+is2,i3+is3,uc)
                u(i1-is1,i2-is2,i3-is3,vc) = -is*2.0*dx(axis)*f2+u(i1+
     & is1,i2+is2,i3+is3,vc)
                u(i1-is1,i2-is2,i3-is3,wc) = -is*2.0*dx(axis)*f3+u(i1+
     & is1,i2+is2,i3+is3,wc)

                !!! now do velocities
                fdot1 = bcf(side,axis,i1,i2,i3,v1c)
                fdot2 = bcf(side,axis,i1,i2,i3,v2c)
                fdot3 = bcf(side,axis,i1,i2,i3,v3c)

                dux = ((u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,v1c))/(2.0*dx(0)
     & ))*(1.0-delta(axis+1,1))
                duy = ((u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,v1c))/(2.0*dx(1)
     & ))*(1.0-delta(axis+1,2))
                duz = ((u(i1,i2,i3+1,v1c)-u(i1,i2,i3-1,v1c))/(2.0*dx(2)
     & ))*(1.0-delta(axis+1,3))

                dvx = ((u(i1+1,i2,i3,v2c)-u(i1-1,i2,i3,v2c))/(2.0*dx(0)
     & ))*(1.0-delta(axis+1,1))
                dvy = ((u(i1,i2+1,i3,v2c)-u(i1,i2-1,i3,v2c))/(2.0*dx(1)
     & ))*(1.0-delta(axis+1,2))
                dvz = ((u(i1,i2,i3+1,v2c)-u(i1,i2,i3-1,v2c))/(2.0*dx(2)
     & ))*(1.0-delta(axis+1,3))

                dwx = ((u(i1+1,i2,i3,v3c)-u(i1-1,i2,i3,v3c))/(2.0*dx(0)
     & ))*(1.0-delta(axis+1,1))
                dwy = ((u(i1,i2+1,i3,v3c)-u(i1,i2-1,i3,v3c))/(2.0*dx(1)
     & ))*(1.0-delta(axis+1,2))
                dwz = ((u(i1,i2,i3+1,v3c)-u(i1,i2,i3-1,v3c))/(2.0*dx(2)
     & ))*(1.0-delta(axis+1,3))

                fdot1 = fdot1-an1*(kappa*dux+lambda*(dvy+dwz))- an2*(
     & mu*(dvx+duy))- an3*(mu*(dwx+duz))
                fdot2 = fdot2-an1*(mu*(dvx+duy))- an2*(kappa*dvy+
     & lambda*(dux+dwz))- an3*(mu*(dwy+dvz))
                fdot3 = fdot3-an1*(mu*(dwx+duz))- an2*(mu*(dwy+dvz))- 
     & an3*(kappa*dwz+lambda*(dux+dvy))

                ! in the Cartesian case all that survives in the Matrix are the diagonal terms
                fdot1 = fdot1/(an1*kappa +an2*mu    +an3*mu)
                fdot2 = fdot2/(an1*mu    +an2*kappa +an3*mu)
                fdot3 = fdot3/(an1*mu    +an2*mu    +an3*kappa)

                u(i1-is1,i2-is2,i3-is3,v1c) = -is*2.0*dx(axis)*fdot1+u(
     & i1+is1,i2+is2,i3+is3,v1c)
                u(i1-is1,i2-is2,i3-is3,v2c) = -is*2.0*dx(axis)*fdot2+u(
     & i1+is1,i2+is2,i3+is3,v2c)
                u(i1-is1,i2-is2,i3-is3,v3c) = -is*2.0*dx(axis)*fdot3+u(
     & i1+is1,i2+is2,i3+is3,v3c)

             end if
             end do
             end do
             end do
          else
            ! *********** TRACTION : Curvilinear Grid ****************
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
             if( mask(i1,i2,i3).gt.0 )then
                f1 = bcf(side,axis,i1,i2,i3,uc)
                f2 = bcf(side,axis,i1,i2,i3,vc)
                f3 = bcf(side,axis,i1,i2,i3,wc)

                fdot1 = bcf(side,axis,i1,i2,i3,v1c)
                fdot2 = bcf(side,axis,i1,i2,i3,v2c)
                fdot3 = bcf(side,axis,i1,i2,i3,v3c)

                met(0,0) = rx(i1,i2,i3,0,0)
                met(0,1) = rx(i1,i2,i3,0,1)
                met(0,2) = rx(i1,i2,i3,0,2)
                met(1,0) = rx(i1,i2,i3,1,0)
                met(1,1) = rx(i1,i2,i3,1,1)
                met(1,2) = rx(i1,i2,i3,1,2)
                met(2,0) = rx(i1,i2,i3,2,0)
                met(2,1) = rx(i1,i2,i3,2,1)
                met(2,2) = rx(i1,i2,i3,2,2)

                ! (an1,an2,an3) = outward normal 
                aNormi = 1.0/max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                an3 = -is*rx(i1,i2,i3,axis,2)*aNormi

                dur(0) = ((u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(
     & 0)))*(1.0-delta(axis+1,1))
                dur(1) = ((u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(
     & 1)))*(1.0-delta(axis+1,2))
                dur(2) = ((u(i1,i2,i3+1,uc)-u(i1,i2,i3-1,uc))/(2.0*dr(
     & 2)))*(1.0-delta(axis+1,3))

                dvr(0) = ((u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(
     & 0)))*(1.0-delta(axis+1,1))
                dvr(1) = ((u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(
     & 1)))*(1.0-delta(axis+1,2))
                dvr(2) = ((u(i1,i2,i3+1,vc)-u(i1,i2,i3-1,vc))/(2.0*dr(
     & 2)))*(1.0-delta(axis+1,3))

                dwr(0) = ((u(i1+1,i2,i3,wc)-u(i1-1,i2,i3,wc))/(2.0*dr(
     & 0)))*(1.0-delta(axis+1,1))
                dwr(1) = ((u(i1,i2+1,i3,wc)-u(i1,i2-1,i3,wc))/(2.0*dr(
     & 1)))*(1.0-delta(axis+1,2))
                dwr(2) = ((u(i1,i2,i3+1,wc)-u(i1,i2,i3-1,wc))/(2.0*dr(
     & 2)))*(1.0-delta(axis+1,3))

                dv1r(0) = ((u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,v1c))/(2.0*
     & dr(0)))*(1.0-delta(axis+1,1))
                dv1r(1) = ((u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,v1c))/(2.0*
     & dr(1)))*(1.0-delta(axis+1,2))
                dv1r(2) = ((u(i1,i2,i3+1,v1c)-u(i1,i2,i3-1,v1c))/(2.0*
     & dr(2)))*(1.0-delta(axis+1,3))

                dv2r(0) = ((u(i1+1,i2,i3,v2c)-u(i1-1,i2,i3,v2c))/(2.0*
     & dr(0)))*(1.0-delta(axis+1,1))
                dv2r(1) = ((u(i1,i2+1,i3,v2c)-u(i1,i2-1,i3,v2c))/(2.0*
     & dr(1)))*(1.0-delta(axis+1,2))
                dv2r(2) = ((u(i1,i2,i3+1,v2c)-u(i1,i2,i3-1,v2c))/(2.0*
     & dr(2)))*(1.0-delta(axis+1,3))

                dv3r(0) = ((u(i1+1,i2,i3,v3c)-u(i1-1,i2,i3,v3c))/(2.0*
     & dr(0)))*(1.0-delta(axis+1,1))
                dv3r(1) = ((u(i1,i2+1,i3,v3c)-u(i1,i2-1,i3,v3c))/(2.0*
     & dr(1)))*(1.0-delta(axis+1,2))
                dv3r(2) = ((u(i1,i2,i3+1,v3c)-u(i1,i2,i3-1,v3c))/(2.0*
     & dr(2)))*(1.0-delta(axis+1,3))

                do isc = 0,2
                  mat(0,0) = an1*kappa*met(isc,0) +an2*mu*met(isc,1)   
     &  +an3*mu*met(isc,2)
                  mat(0,1) = an1*lambda*met(isc,1)+an2*mu*met(isc,0)
                  mat(0,2) = an1*lambda*met(isc,2)                     
     &  +an3*mu*met(isc,0)
                  mat(1,0) = an1*mu*met(isc,1)    +an2*lambda*met(isc,
     & 0)
                  mat(1,1) = an1*mu*met(isc,0)    +an2*kappa*met(isc,1)
     &  +an3*mu*met(isc,2)
                  mat(1,2) =                       an2*lambda*met(isc,
     & 2)+an3*mu*met(isc,1)
                  mat(2,0) = an1*mu*met(isc,2)                         
     &  +an3*lambda*met(isc,0)
                  mat(2,1) =                       an2*mu*met(isc,2)   
     &  +an3*lambda*met(isc,1)
                  mat(2,2) = an1*mu*met(isc,0)    +an2*mu*met(isc,1)   
     &  +an3*kappa*met(isc,2)

                  f1 = f1-(mat(0,0)*dur(isc)+mat(0,1)*dvr(isc)+mat(0,2)
     & *dwr(isc))
                  f2 = f2-(mat(1,0)*dur(isc)+mat(1,1)*dvr(isc)+mat(1,2)
     & *dwr(isc))
                  f3 = f3-(mat(2,0)*dur(isc)+mat(2,1)*dvr(isc)+mat(2,2)
     & *dwr(isc))

                  fdot1 = fdot1-(mat(0,0)*dv1r(isc)+mat(0,1)*dv2r(isc)+
     & mat(0,2)*dv3r(isc))
                  fdot2 = fdot2-(mat(1,0)*dv1r(isc)+mat(1,1)*dv2r(isc)+
     & mat(1,2)*dv3r(isc))
                  fdot3 = fdot3-(mat(2,0)*dv1r(isc)+mat(2,1)*dv2r(isc)+
     & mat(2,2)*dv3r(isc))

                  if( axis.eq.isc ) then
                    lhs(0,0) = mat(0,0)
                    lhs(0,1) = mat(0,1)
                    lhs(0,2) = mat(0,2)
                    lhs(1,0) = mat(1,0)
                    lhs(1,1) = mat(1,1)
                    lhs(1,2) = mat(1,2)
                    lhs(2,0) = mat(2,0)
                    lhs(2,1) = mat(2,1)
                    lhs(2,2) = mat(2,2)
                  end if
                end do


                !! solve linear systems to get the solution (grid derivatives)
                rhs(0,0) = f1
                rhs(1,0) = f2
                rhs(2,0) = f3
                rhs(0,1) = fdot1
                rhs(1,1) = fdot2
                rhs(2,1) = fdot3

                call dgesv( 3,2,lhs,3,ipiv,rhs,3,info )
                if( info.ne.0 ) then
                  write(6,*)'Error (compat3D) : error in  linear 
     & system'
                  stop
                end if

                u(i1-is1,i2-is2,i3-is3,uc)  = -is*2.0*rhs(0,0)*dr(axis)
     & +u(i1+is1,i2+is2,i3+is3,uc)
                u(i1-is1,i2-is2,i3-is3,vc)  = -is*2.0*rhs(1,0)*dr(axis)
     & +u(i1+is1,i2+is2,i3+is3,vc)
                u(i1-is1,i2-is2,i3-is3,wc)  = -is*2.0*rhs(2,0)*dr(axis)
     & +u(i1+is1,i2+is2,i3+is3,wc)

                u(i1-is1,i2-is2,i3-is3,v1c) = -is*2.0*rhs(0,1)*dr(axis)
     & +u(i1+is1,i2+is2,i3+is3,v1c)
                u(i1-is1,i2-is2,i3-is3,v2c) = -is*2.0*rhs(1,1)*dr(axis)
     & +u(i1+is1,i2+is2,i3+is3,v2c)
                u(i1-is1,i2-is2,i3-is3,v3c) = -is*2.0*rhs(2,1)*dr(axis)
     & +u(i1+is1,i2+is2,i3+is3,v3c)

             end if
             end do
             end do
             end do
          end if ! gridType
        else if( boundaryCondition(side,axis).eq.slipWall ) then
          ! **************** SLIPWALL : Neumann type conditions ******************
          if( gridType.eq.rectangular ) then
            ! ********* SLIPWALL : Cartesian Grid **********
          else
            ! ********* SLIPWALL : Curvilinear Grid **********
          end if ! gridType

        else if( boundaryCondition(side,axis).gt.0 .and. 
     & boundaryCondition(side,axis).ne.dirichletBoundaryCondition ) 
     & then
        write(*,'("smg3d:BC: unknown BC: side,axis,grid, 
     & boundaryCondition=",i2,i2,i4,i8)') side,axis,grid,
     & boundaryCondition(side,axis)

        end if ! bc
       end do ! end side
       end do ! end axis

c*******
c******* Secondary Dirichlet conditions for the tangential components of stress (tractionBC only) ********
c*******
c      if( .false. ) then
         extra1a=numGhost
         extra1b=numGhost
         extra2a=numGhost
         extra2b=numGhost
         if( nd.eq.3 )then
           extra3a=numGhost
           extra3b=numGhost
         else
           extra3a=0
           extra3b=0
         end if
         if( boundaryCondition(0,0).lt.0 )then
           extra1a=max(0,extra1a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
         else if( boundaryCondition(0,0).eq.0 )then
           extra1a=numGhost  ! include interpolation points since we assign ghost points outside these
         end if
         ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
         if( boundaryCondition(1,0).lt.0 )then
           extra1b=max(0,extra1b) ! over-ride numGhost=-1 : assign ends in periodic directions
         else if( boundaryCondition(1,0).eq.0 )then
           extra1b=numGhost
         end if
         if( boundaryCondition(0,1).lt.0 )then
           extra2a=max(0,extra2a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
         else if( boundaryCondition(0,1).eq.0 )then
           extra2a=numGhost  ! include interpolation points since we assign ghost points outside these
         end if
         ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
         if( boundaryCondition(1,1).lt.0 )then
           extra2b=max(0,extra2b) ! over-ride numGhost=-1 : assign ends in periodic directions
         else if( boundaryCondition(1,1).eq.0 )then
           extra2b=numGhost
         end if
         if(  nd.eq.3 )then
          if( boundaryCondition(0,2).lt.0 )then
            extra3a=max(0,extra3a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
          else if( boundaryCondition(0,2).eq.0 )then
            extra3a=numGhost  ! include interpolation points since we assign ghost points outside these
          end if
          ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
          if( boundaryCondition(1,2).lt.0 )then
            extra3b=max(0,extra3b) ! over-ride numGhost=-1 : assign ends in periodic directions
          else if( boundaryCondition(1,2).eq.0 )then
            extra3b=numGhost
          end if
         end if
         do axis=0,nd-1
         do side=0,1
           if( boundaryCondition(side,axis).gt.0 )then
             ! write(*,'(" bcOpt: side,axis,bc=",3i2)') side,axis,boundaryCondition(side,axis)
             n1a=gridIndexRange(0,0)
             n1b=gridIndexRange(1,0)
             n2a=gridIndexRange(0,1)
             n2b=gridIndexRange(1,1)
             n3a=gridIndexRange(0,2)
             n3b=gridIndexRange(1,2)
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
             nn1a=gridIndexRange(0,0)-extra1a
             nn1b=gridIndexRange(1,0)+extra1b
             nn2a=gridIndexRange(0,1)-extra2a
             nn2b=gridIndexRange(1,1)+extra2b
             nn3a=gridIndexRange(0,2)-extra3a
             nn3b=gridIndexRange(1,2)+extra3b
             if( axis.eq.0 )then
               nn1a=gridIndexRange(side,axis)
               nn1b=gridIndexRange(side,axis)
             else if( axis.eq.1 )then
               nn2a=gridIndexRange(side,axis)
               nn2b=gridIndexRange(side,axis)
             else
               nn3a=gridIndexRange(side,axis)
               nn3b=gridIndexRange(side,axis)
             end if
             is=1-2*side
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
             i3=n3a
        !*      ! (js1,js2,js3) used to compute tangential derivatives
        !*      js1=0
        !*      js2=0
        !*      js3=0
        !*      if( axisp1.eq.0 )then
        !*        js1=1-2*side
        !*      else if( axisp1.eq.1 )then
        !*        js2=1-2*side
        !*      else if( axisp1.eq.2 )then
        !*        js3=1-2*side
        !*      else
        !*        stop 5
        !*      end if
        !* 
        !*      ! (ks1,ks2,ks3) used to compute second tangential derivative
        !*      ks1=0
        !*      ks2=0
        !*      ks3=0
        !*      if( axisp2.eq.0 )then
        !*        ks1=1-2*side
        !*      else if( axisp2.eq.1 )then
        !*        ks2=1-2*side
        !*      else if( axisp2.eq.2 )then
        !*        ks3=1-2*side
        !*      else
        !*        stop 5
        !*      end if
             if( debug.gt.7 )then
               write(*,'(" bcOpt: grid,side,axis=",3i3,", loop bounds: 
     & n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,n1a,n1b,n2a,
     & n2b,n3a,n3b
             end if
           end if ! if bc>0
          if( boundaryCondition(side,axis).eq.tractionBC )then
            if( gridType.eq.rectangular )then
              if( axis.eq.0 )then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    u1x = ((u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(
     & 0)))
                    u1y = ((u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(
     & 1)))
                    u1z = ((u(i1,i2,i3+1,uc)-u(i1,i2,i3-1,uc))/(2.0*dx(
     & 2)))

                    u2x = ((u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(
     & 0)))
                    u2y = ((u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(
     & 1)))
                    u2z = ((u(i1,i2,i3+1,vc)-u(i1,i2,i3-1,vc))/(2.0*dx(
     & 2)))

                    u3x = ((u(i1+1,i2,i3,wc)-u(i1-1,i2,i3,wc))/(2.0*dx(
     & 0)))
                    u3y = ((u(i1,i2+1,i3,wc)-u(i1,i2-1,i3,wc))/(2.0*dx(
     & 1)))
                    u3z = ((u(i1,i2,i3+1,wc)-u(i1,i2,i3-1,wc))/(2.0*dx(
     & 2)))

                    u(i1,i2,i3,s21c) = mu*(u1y+u2x)
                    u(i1,i2,i3,s22c) = kappa*u2y+lambda*(u1x+u3z)
                    u(i1,i2,i3,s23c) = mu*(u3y+u2z)

                    u(i1,i2,i3,s31c) = mu*(u3x+u1z)
                    u(i1,i2,i3,s32c) = mu*(u3y+u2z)
                    u(i1,i2,i3,s33c) = kappa*u3z+lambda*(u1x+u2y)
                 end if
                 end do
                 end do
                 end do
              else if( axis.eq.1 ) then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    u1x = ((u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(
     & 0)))
                    u1y = ((u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(
     & 1)))
                    u1z = ((u(i1,i2,i3+1,uc)-u(i1,i2,i3-1,uc))/(2.0*dx(
     & 2)))

                    u2x = ((u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(
     & 0)))
                    u2y = ((u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(
     & 1)))
                    u2z = ((u(i1,i2,i3+1,vc)-u(i1,i2,i3-1,vc))/(2.0*dx(
     & 2)))

                    u3x = ((u(i1+1,i2,i3,wc)-u(i1-1,i2,i3,wc))/(2.0*dx(
     & 0)))
                    u3y = ((u(i1,i2+1,i3,wc)-u(i1,i2-1,i3,wc))/(2.0*dx(
     & 1)))
                    u3z = ((u(i1,i2,i3+1,wc)-u(i1,i2,i3-1,wc))/(2.0*dx(
     & 2)))

                    u(i1,i2,i3,s11c) = kappa*u1x+lambda*(u2y+u3z)
                    u(i1,i2,i3,s12c) = mu*(u2x+u1y)
                    u(i1,i2,i3,s13c) = mu*(u3x+u1z)

                    u(i1,i2,i3,s31c) = mu*(u3x+u1z)
                    u(i1,i2,i3,s32c) = mu*(u3y+u2z)
                    u(i1,i2,i3,s33c) = kappa*u3z+lambda*(u1x+u2y)
                 end if
                 end do
                 end do
                 end do
              else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    u1x = ((u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(
     & 0)))
                    u1y = ((u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(
     & 1)))
                    u1z = ((u(i1,i2,i3+1,uc)-u(i1,i2,i3-1,uc))/(2.0*dx(
     & 2)))

                    u2x = ((u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(
     & 0)))
                    u2y = ((u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(
     & 1)))
                    u2z = ((u(i1,i2,i3+1,vc)-u(i1,i2,i3-1,vc))/(2.0*dx(
     & 2)))

                    u3x = ((u(i1+1,i2,i3,wc)-u(i1-1,i2,i3,wc))/(2.0*dx(
     & 0)))
                    u3y = ((u(i1,i2+1,i3,wc)-u(i1,i2-1,i3,wc))/(2.0*dx(
     & 1)))
                    u3z = ((u(i1,i2,i3+1,wc)-u(i1,i2,i3-1,wc))/(2.0*dx(
     & 2)))

                    u(i1,i2,i3,s11c) = kappa*u1x+lambda*(u2y+u3z)
                    u(i1,i2,i3,s12c) = mu*(u2x+u1y)
                    u(i1,i2,i3,s13c) = mu*(u3x+u1z)

                    u(i1,i2,i3,s21c) = mu*(u1y+u2x)
                    u(i1,i2,i3,s22c) = kappa*u2y+lambda*(u1x+u3z)
                    u(i1,i2,i3,s23c) = mu*(u3y+u2z)
                 end if
                 end do
                 end do
                 end do
              end if

            else  ! curvilinear
              if( axis.eq.0 ) then
                tan1c = 1
                tan2c = 2
              else if( axis.eq.1 ) then
                tan1c = 2
                tan2c = 0
              else
                tan1c = 0
                tan2c = 1
              end if
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
               if( mask(i1,i2,i3).gt.0 )then
                  u1r = ((u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(0)
     & ))
                  u1s = ((u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(1)
     & ))
                  u1t = ((u(i1,i2,i3+1,uc)-u(i1,i2,i3-1,uc))/(2.0*dr(2)
     & ))

                  u2r = ((u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(0)
     & ))
                  u2s = ((u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(1)
     & ))
                  u2t = ((u(i1,i2,i3+1,vc)-u(i1,i2,i3-1,vc))/(2.0*dr(2)
     & ))

                  u3r = ((u(i1+1,i2,i3,wc)-u(i1-1,i2,i3,wc))/(2.0*dr(0)
     & ))
                  u3s = ((u(i1,i2+1,i3,wc)-u(i1,i2-1,i3,wc))/(2.0*dr(1)
     & ))
                  u3t = ((u(i1,i2,i3+1,wc)-u(i1,i2,i3-1,wc))/(2.0*dr(2)
     & ))

                  u1x = u1r*rx(i1,i2,i3,0,0)+u1s*rx(i1,i2,i3,1,0)+u1t*
     & rx(i1,i2,i3,2,0)
                  u1y = u1r*rx(i1,i2,i3,0,1)+u1s*rx(i1,i2,i3,1,1)+u1t*
     & rx(i1,i2,i3,2,1)
                  u1z = u1r*rx(i1,i2,i3,0,2)+u1s*rx(i1,i2,i3,1,2)+u1t*
     & rx(i1,i2,i3,2,2)

                  u2x = u2r*rx(i1,i2,i3,0,0)+u2s*rx(i1,i2,i3,1,0)+u2t*
     & rx(i1,i2,i3,2,0)
                  u2y = u2r*rx(i1,i2,i3,0,1)+u2s*rx(i1,i2,i3,1,1)+u2t*
     & rx(i1,i2,i3,2,1)
                  u2z = u2r*rx(i1,i2,i3,0,2)+u2s*rx(i1,i2,i3,1,2)+u2t*
     & rx(i1,i2,i3,2,2)

                  u3x = u3r*rx(i1,i2,i3,0,0)+u3s*rx(i1,i2,i3,1,0)+u3t*
     & rx(i1,i2,i3,2,0)
                  u3y = u3r*rx(i1,i2,i3,0,1)+u3s*rx(i1,i2,i3,1,1)+u3t*
     & rx(i1,i2,i3,2,1)
                  u3z = u3r*rx(i1,i2,i3,0,2)+u3s*rx(i1,i2,i3,1,2)+u3t*
     & rx(i1,i2,i3,2,2)

                  s11t = kappa*u1x+lambda*(u2y+u3z)
                  s12t = mu*(u2x+u1y)
                  s13t = mu*(u3x+u1z)

                  s21t = mu*(u2x+u1y)
                  s22t = kappa*u2y+lambda*(u1x+u3z)
                  s23t = mu*(u3y+u2z)

                  s31t = mu*(u3x+u1z)
                  s32t = mu*(u3y+u2z)
                  s33t = kappa*u3z+lambda*(u1x+u2y)

                  ! (an1,an2,3) = outward unit normal 
                  aNormi = 1.0/max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                  an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                  an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                  an3 = -is*rx(i1,i2,i3,axis,2)*aNormi

                  !! do the signs of sn and tn matter?? ... I do not think so
                  sn1 = rx(i1,i2,i3,tan1c,0)
                  sn2 = rx(i1,i2,i3,tan1c,1)
                  sn3 = rx(i1,i2,i3,tan1c,2)

                  tn1 = rx(i1,i2,i3,tan2c,0)
                  tn2 = rx(i1,i2,i3,tan2c,1)
                  tn3 = rx(i1,i2,i3,tan2c,2)

                  ! set sn to be part of sn which is orthogonal to an
                  alpha = an1*sn1+an2*sn2+an3*sn3
                  sn1 = sn1-alpha*an1
                  sn2 = sn2-alpha*an2
                  sn3 = sn3-alpha*an3
                  ! normalize sn
                  aNormi = 1.0/max(epsx,sqrt(sn1**2+sn2**2+sn3**2))
                  sn1 = sn1*aNormi
                  sn2 = sn2*aNormi
                  sn3 = sn3*aNormi

                  ! set tn to be part of tn which is orthogonal to an and sn
                  alpha = an1*tn1+an2*tn2+an3*tn3
                  tn1 = tn1-alpha*an1
                  tn2 = tn2-alpha*an2
                  tn3 = tn3-alpha*an3
                  alpha = sn1*tn1+sn2*tn2+sn3*tn3
                  tn1 = tn1-alpha*sn1
                  tn2 = tn2-alpha*sn2
                  tn3 = tn3-alpha*sn3
                  ! normalize tn
                  aNormi = 1.0/max(epsx,sqrt(tn1**2+tn2**2+tn3**2))
                  tn1 = tn1*aNormi
                  tn2 = tn2*aNormi
                  tn3 = tn3*aNormi

                  ! compute components of stress in normal direction (primary condition)
                  ns1 = an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,i3,s21c)+an3*
     & u(i1,i2,i3,s31c)
                  ns2 = an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,i3,s22c)+an3*
     & u(i1,i2,i3,s32c)
                  ns3 = an1*u(i1,i2,i3,s13c)+an2*u(i1,i2,i3,s23c)+an3*
     & u(i1,i2,i3,s33c)

                  ! compute componenets of stress in 1st tangential direction (secondary condition)
                  ss1 = sn1*s11t+sn2*s21t+sn3*s31t
                  ss2 = sn1*s12t+sn2*s22t+sn3*s32t
                  ss3 = sn1*s13t+sn2*s23t+sn3*s33t

                  ! compute componenets of stress in 2nd tangential direction (secondary condition)
                  ts1 = tn1*s11t+tn2*s21t+tn3*s31t
                  ts2 = tn1*s12t+tn2*s22t+tn3*s32t
                  ts3 = tn1*s13t+tn2*s23t+tn3*s33t

                  u(i1,i2,i3,s11c) = an1*ns1+sn1*ss1+tn1*ts1
                  u(i1,i2,i3,s12c) = an1*ns2+sn1*ss2+tn1*ts2
                  u(i1,i2,i3,s13c) = an1*ns3+sn1*ss3+tn1*ts3

                  u(i1,i2,i3,s21c) = an2*ns1+sn2*ss1+tn2*ts1
                  u(i1,i2,i3,s22c) = an2*ns2+sn2*ss2+tn2*ts2
                  u(i1,i2,i3,s23c) = an2*ns3+sn2*ss3+tn2*ts3

                  u(i1,i2,i3,s31c) = an3*ns1+sn3*ss1+tn3*ts1
                  u(i1,i2,i3,s32c) = an3*ns2+sn3*ss2+tn3*ts2
                  u(i1,i2,i3,s33c) = an3*ns3+sn3*ss3+tn3*ts3
               end if
               end do
               end do
               end do

            end if  ! end gridType

          end if ! bc
         end do ! end side
         end do ! end axis

c set tangential components of stress on the boundary  (TZ forcing, if necessary)
        if( twilightZone.ne.0 ) then
c        if( .false. ) then

           extra1a=numGhost
           extra1b=numGhost
           extra2a=numGhost
           extra2b=numGhost
           if( nd.eq.3 )then
             extra3a=numGhost
             extra3b=numGhost
           else
             extra3a=0
             extra3b=0
           end if
           if( boundaryCondition(0,0).lt.0 )then
             extra1a=max(0,extra1a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
           else if( boundaryCondition(0,0).eq.0 )then
             extra1a=numGhost  ! include interpolation points since we assign ghost points outside these
           end if
           ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
           if( boundaryCondition(1,0).lt.0 )then
             extra1b=max(0,extra1b) ! over-ride numGhost=-1 : assign ends in periodic directions
           else if( boundaryCondition(1,0).eq.0 )then
             extra1b=numGhost
           end if
           if( boundaryCondition(0,1).lt.0 )then
             extra2a=max(0,extra2a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
           else if( boundaryCondition(0,1).eq.0 )then
             extra2a=numGhost  ! include interpolation points since we assign ghost points outside these
           end if
           ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
           if( boundaryCondition(1,1).lt.0 )then
             extra2b=max(0,extra2b) ! over-ride numGhost=-1 : assign ends in periodic directions
           else if( boundaryCondition(1,1).eq.0 )then
             extra2b=numGhost
           end if
           if(  nd.eq.3 )then
            if( boundaryCondition(0,2).lt.0 )then
              extra3a=max(0,extra3a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
            else if( boundaryCondition(0,2).eq.0 )then
              extra3a=numGhost  ! include interpolation points since we assign ghost points outside these
            end if
            ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
            if( boundaryCondition(1,2).lt.0 )then
              extra3b=max(0,extra3b) ! over-ride numGhost=-1 : assign ends in periodic directions
            else if( boundaryCondition(1,2).eq.0 )then
              extra3b=numGhost
            end if
           end if
           do axis=0,nd-1
           do side=0,1
             if( boundaryCondition(side,axis).gt.0 )then
               ! write(*,'(" bcOpt: side,axis,bc=",3i2)') side,axis,boundaryCondition(side,axis)
               n1a=gridIndexRange(0,0)
               n1b=gridIndexRange(1,0)
               n2a=gridIndexRange(0,1)
               n2b=gridIndexRange(1,1)
               n3a=gridIndexRange(0,2)
               n3b=gridIndexRange(1,2)
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
               nn1a=gridIndexRange(0,0)-extra1a
               nn1b=gridIndexRange(1,0)+extra1b
               nn2a=gridIndexRange(0,1)-extra2a
               nn2b=gridIndexRange(1,1)+extra2b
               nn3a=gridIndexRange(0,2)-extra3a
               nn3b=gridIndexRange(1,2)+extra3b
               if( axis.eq.0 )then
                 nn1a=gridIndexRange(side,axis)
                 nn1b=gridIndexRange(side,axis)
               else if( axis.eq.1 )then
                 nn2a=gridIndexRange(side,axis)
                 nn2b=gridIndexRange(side,axis)
               else
                 nn3a=gridIndexRange(side,axis)
                 nn3b=gridIndexRange(side,axis)
               end if
               is=1-2*side
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
               i3=n3a
          !*      ! (js1,js2,js3) used to compute tangential derivatives
          !*      js1=0
          !*      js2=0
          !*      js3=0
          !*      if( axisp1.eq.0 )then
          !*        js1=1-2*side
          !*      else if( axisp1.eq.1 )then
          !*        js2=1-2*side
          !*      else if( axisp1.eq.2 )then
          !*        js3=1-2*side
          !*      else
          !*        stop 5
          !*      end if
          !* 
          !*      ! (ks1,ks2,ks3) used to compute second tangential derivative
          !*      ks1=0
          !*      ks2=0
          !*      ks3=0
          !*      if( axisp2.eq.0 )then
          !*        ks1=1-2*side
          !*      else if( axisp2.eq.1 )then
          !*        ks2=1-2*side
          !*      else if( axisp2.eq.2 )then
          !*        ks3=1-2*side
          !*      else
          !*        stop 5
          !*      end if
               if( debug.gt.7 )then
                 write(*,'(" bcOpt: grid,side,axis=",3i3,", loop 
     & bounds: n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,n1a,
     & n1b,n2a,n2b,n3a,n3b
               end if
             end if ! if bc>0
            if( boundaryCondition(side,axis).eq.tractionBC )then

              if( gridType.eq.rectangular )then
                if( axis.eq.0 )then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                      call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,uc,u1x )
                      call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,uc,u1y )
                      call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,uc,u1z )

                      call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,vc,u2x )
                      call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,vc,u2y )
                      call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,vc,u2z )

                      call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,wc,u3x )
                      call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,wc,u3y )
                      call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,wc,u3z )

                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s21c,s21e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s22c,s22e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s23c,s23e )

                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s31c,s31e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s32c,s32e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s33c,s33e )

                      u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)+s21e-mu*(u1y+
     & u2x)
                      u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)+s22e-(kappa*
     & u2y+lambda*(u1x+u3z))
                      u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)+s23e-mu*(u3y+
     & u2z)

                      u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)+s31e-mu*(u3x+
     & u1z)
                      u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)+s32e-mu*(u3y+
     & u2z)
                      u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)+s33e-(kappa*
     & u3z+lambda*(u1x+u2y))
                   end if
                   end do
                   end do
                   end do
                else if( axis.eq.1 ) then
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                      call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,uc,u1x )
                      call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,uc,u1y )
                      call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,uc,u1z )

                      call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,vc,u2x )
                      call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,vc,u2y )
                      call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,vc,u2z )

                      call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,wc,u3x )
                      call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,wc,u3y )
                      call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,wc,u3z )

                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s11c,s11e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s12c,s12e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s13c,s13e )

                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s31c,s31e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s32c,s32e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s33c,s33e )

                      u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)+s11e-(kappa*
     & u1x+lambda*(u2y+u3z))
                      u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)+s12e-mu*(u2x+
     & u1y)
                      u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)+s13e-mu*(u3x+
     & u1z)

                      u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)+s31e-mu*(u3x+
     & u1z)
                      u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)+s32e-mu*(u3y+
     & u2z)
                      u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)+s33e-(kappa*
     & u3z+lambda*(u1x+u2y))
                   end if
                   end do
                   end do
                   end do
                else
                   do i3=n3a,n3b
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                      call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,uc,u1x )
                      call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,uc,u1y )
                      call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,uc,u1z )

                      call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,vc,u2x )
                      call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,vc,u2y )
                      call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,vc,u2z )

                      call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,wc,u3x )
                      call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,wc,u3y )
                      call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,wc,u3z )

                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s11c,s11e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s12c,s12e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s13c,s13e )

                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s21c,s21e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s22c,s22e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s23c,s23e )

                      u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)+s11e-(kappa*
     & u1x+lambda*(u2y+u3z))
                      u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)+s12e-mu*(u2x+
     & u1y)
                      u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)+s13e-mu*(u3x+
     & u1z)

                      u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)+s21e-mu*(u1y+
     & u2x)
                      u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)+s22e-(kappa*
     & u2y+lambda*(u1x+u3z))
                      u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)+s23e-mu*(u3y+
     & u2z)
                   end if
                   end do
                   end do
                   end do
                end if

              else ! curvilinear
                if( axis.eq.0 ) then
                  tan1c = 1
                  tan2c = 2
                else if( axis.eq.1 ) then
                  tan1c = 2
                  tan2c = 0
                else
                  tan1c = 0
                  tan2c = 1
                end if
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                    call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,uc,u1x )
                    call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,uc,u1y )
                    call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,uc,u1z )

                    call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,vc,u2x )
                    call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,vc,u2y )
                    call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,vc,u2z )

                    call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,wc,u3x )
                    call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,wc,u3y )
                    call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,wc,u3z )

                    call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s11c,s11e )
                    call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s12c,s12e )
                    call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s13c,s13e )

                    call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s21c,s21e )
                    call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s22c,s22e )
                    call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s23c,s23e )

                    call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s31c,s31e )
                    call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s32c,s32e )
                    call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),xy(i1,i2,i3,2),t,s33c,s33e )

                    s11t = kappa*u1x+lambda*(u2y+u3z)
                    s12t = mu*(u2x+u1y)
                    s13t = mu*(u3x+u1z)

                    s21t = mu*(u2x+u1y)
                    s22t = kappa*u2y+lambda*(u1x+u3z)
                    s23t = mu*(u3y+u2z)

                    s31t = mu*(u3x+u1z)
                    s32t = mu*(u3y+u2z)
                    s33t = kappa*u3z+lambda*(u1x+u2y)

                    ! (an1,an2,3) = outward unit normal 
                    aNormi = 1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+
     & rx(i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                    an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                    an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                    an3 = -is*rx(i1,i2,i3,axis,2)*aNormi

                    sn1 = rx(i1,i2,i3,tan1c,0)
                    sn2 = rx(i1,i2,i3,tan1c,1)
                    sn3 = rx(i1,i2,i3,tan1c,2)

                    tn1 = rx(i1,i2,i3,tan2c,0)
                    tn2 = rx(i1,i2,i3,tan2c,1)
                    tn3 = rx(i1,i2,i3,tan2c,2)

                    ! set sn to be part of sn which is orthogonal to an
                    alpha = an1*sn1+an2*sn2+an3*sn3
                    sn1 = sn1-alpha*an1
                    sn2 = sn2-alpha*an2
                    sn3 = sn3-alpha*an3
                    ! normalize sn
                    aNormi = 1./max(epsx,sqrt(sn1**2+sn2**2+sn3**2))
                    sn1 = sn1*aNormi
                    sn2 = sn2*aNormi
                    sn3 = sn3*aNormi

                    ! set tn to be part of tn which is orthogonal to an and sn
                    alpha = an1*tn1+an2*tn2+an3*tn3
                    tn1 = tn1-alpha*an1
                    tn2 = tn2-alpha*an2
                    tn3 = tn3-alpha*an3
                    alpha = sn1*tn1+sn2*tn2+sn3*tn3
                    tn1 = tn1-alpha*sn1
                    tn2 = tn2-alpha*sn2
                    tn3 = tn3-alpha*sn3
                    ! normalize tn
                    aNormi = 1./max(epsx,sqrt(tn1**2+tn2**2+tn3**2))
                    tn1 = tn1*aNormi
                    tn2 = tn2*aNormi
                    tn3 = tn3*aNormi

                    ! compute components of stress in normal direction (leave these alone)
                    ns1 = an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,i3,s21c)+
     & an3*u(i1,i2,i3,s31c)
                    ns2 = an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,i3,s22c)+
     & an3*u(i1,i2,i3,s32c)
                    ns3 = an1*u(i1,i2,i3,s13c)+an2*u(i1,i2,i3,s23c)+
     & an3*u(i1,i2,i3,s33c)

                    ! compute componenets of stress in tangential directions (add forcing to these)
                    ss1 = sn1*u(i1,i2,i3,s11c)+sn2*u(i1,i2,i3,s21c)+
     & sn3*u(i1,i2,i3,s31c)
                    ss2 = sn1*u(i1,i2,i3,s12c)+sn2*u(i1,i2,i3,s22c)+
     & sn3*u(i1,i2,i3,s32c)
                    ss3 = sn1*u(i1,i2,i3,s13c)+sn2*u(i1,i2,i3,s23c)+
     & sn3*u(i1,i2,i3,s33c)
                    ts1 = tn1*u(i1,i2,i3,s11c)+tn2*u(i1,i2,i3,s21c)+
     & tn3*u(i1,i2,i3,s31c)
                    ts2 = tn1*u(i1,i2,i3,s12c)+tn2*u(i1,i2,i3,s22c)+
     & tn3*u(i1,i2,i3,s32c)
                    ts3 = tn1*u(i1,i2,i3,s13c)+tn2*u(i1,i2,i3,s23c)+
     & tn3*u(i1,i2,i3,s33c)

                    ! compute componenets of derived stress in tangential directions
                    ss1d = sn1*s11t+sn2*s21t+sn3*s31t
                    ss2d = sn1*s12t+sn2*s22t+sn3*s32t
                    ss3d = sn1*s13t+sn2*s23t+sn3*s33t
                    ts1d = tn1*s11t+tn2*s21t+tn3*s31t
                    ts2d = tn1*s12t+tn2*s22t+tn3*s32t
                    ts3d = tn1*s13t+tn2*s23t+tn3*s33t

                    ! compute componenets of exact stress in tangential directions
                    ss1e = sn1*s11e+sn2*s21e+sn3*s31e
                    ss2e = sn1*s12e+sn2*s22e+sn3*s32e
                    ss3e = sn1*s13e+sn2*s23e+sn3*s33e
                    ts1e = tn1*s11e+tn2*s21e+tn3*s31e
                    ts2e = tn1*s12e+tn2*s22e+tn3*s32e
                    ts3e = tn1*s13e+tn2*s23e+tn3*s33e

                    ss1 = ss1+ss1e-ss1d
                    ss2 = ss2+ss2e-ss2d
                    ss3 = ss3+ss3e-ss3d
                    ts1 = ts1+ts1e-ts1d
                    ts2 = ts2+ts2e-ts2d
                    ts3 = ts3+ts3e-ts3d

                    u(i1,i2,i3,s11c) = an1*ns1+sn1*ss1+tn1*ts1
                    u(i1,i2,i3,s12c) = an1*ns2+sn1*ss2+tn1*ts2
                    u(i1,i2,i3,s13c) = an1*ns3+sn1*ss3+tn1*ts3

                    u(i1,i2,i3,s21c) = an2*ns1+sn2*ss1+tn2*ts1
                    u(i1,i2,i3,s22c) = an2*ns2+sn2*ss2+tn2*ts2
                    u(i1,i2,i3,s23c) = an2*ns3+sn2*ss3+tn2*ts3

                    u(i1,i2,i3,s31c) = an3*ns1+sn3*ss1+tn3*ts1
                    u(i1,i2,i3,s32c) = an3*ns2+sn3*ss2+tn3*ts2
                    u(i1,i2,i3,s33c) = an3*ns3+sn3*ss3+tn3*ts3

c                    u(i1,i2,i3,s11c) = s11e
c                    u(i1,i2,i3,s12c) = s12e
c                    u(i1,i2,i3,s13c) = s13e
c                                                                        
c                    u(i1,i2,i3,s21c) = s21e
c                    u(i1,i2,i3,s22c) = s22e
c                    u(i1,i2,i3,s23c) = s23e
c                                                                        
c                    u(i1,i2,i3,s31c) = s31e
c                    u(i1,i2,i3,s32c) = s32e
c                    u(i1,i2,i3,s33c) = s33e
                 end if
                 end do
                 end do
                 end do

              end if  ! end gridType

            end if ! bc
           end do ! end side
           end do ! end axis
c
c.. substract off components of TZ force that were added twice
                do edgeDirection = 0,2 ! direction parallel to the edge
                  do sidea = 0,1
                  do sideb = 0,1
                    if( edgeDirection.eq.0 ) then
                      side1 = 0
                      side2 = sidea
                      side3 = sideb
                    else if( edgeDirection.eq.1 ) then
                      side1 = sideb
                      side2 = 0
                      side3 = sidea
                    else
                      side1 = sidea
                      side2 = sideb
                      side3 = 0
                    end if
                    is1 = 1-2*(side1)
                    is2 = 1-2*(side2)
                    is3 = 1-2*(side3)
                    if( edgeDirection.eq.2 ) then
                      is3 = 0
                      n1a = gridIndexRange(side1,0)
                      n1b = gridIndexRange(side1,0)
                      n2a = gridIndexRange(side2,1)
                      n2b = gridIndexRange(side2,1)
                      n3a = gridIndexRange(    0,2)
                      n3b = gridIndexRange(    1,2)
                      bc1 = boundaryCondition(side1,0)
                      bc2 = boundaryCondition(side2,1)
                    else if( edgeDirection.eq.1 )then
                      is2 = 0
                      n1a = gridIndexRange(side1,0)
                      n1b = gridIndexRange(side1,0)
                      n2a = gridIndexRange(    0,1)
                      n2b = gridIndexRange(    1,1)
                      n3a = gridIndexRange(side3,2)
                      n3b = gridIndexRange(side3,2)
                      bc1 = boundaryCondition(side1,0)
                      bc2 = boundaryCondition(side3,2)
                    else
                      is1 = 0
                      n1a = gridIndexRange(    0,0)
                      n1b = gridIndexRange(    1,0)
                      n2a = gridIndexRange(side2,1)
                      n2b = gridIndexRange(side2,1)
                      n3a = gridIndexRange(side3,2)
                      n3b = gridIndexRange(side3,2)
                      bc1 = boundaryCondition(side2,1)
                      bc2 = boundaryCondition(side3,2)
                    end if
            if( bc1.eq.tractionBC .and. bc2.eq.tractionBC ) then
              if( gridType.eq.rectangular ) then
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                  call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,uc,u1x )
                  call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,uc,u1y )
                  call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,uc,u1z )

                  call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,vc,u2x )
                  call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,vc,u2y )
                  call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,vc,u2z )

                  call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,wc,u3x )
                  call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,wc,u3y )
                  call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,wc,u3z )

                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s11c,s11e )
                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s12c,s12e )
                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s13c,s13e )

                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s21c,s21e )
                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s22c,s22e )
                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s23c,s23e )

                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s31c,s31e )
                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s32c,s32e )
                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s33c,s33e )

                  if( edgeDirection.eq.0 ) then
                    u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)-(s11e-(kappa*
     & u1x+lambda*(u2y+u3z)))
                    u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)-(s12e-mu*(u2x+
     & u1y))
                    u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)-(s13e-mu*(u3x+
     & u1z))
                  else if( edgeDirection.eq.1 ) then
                    u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)-(s21e-mu*(u1y+
     & u2x))
                    u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)-(s22e-(kappa*
     & u2y+lambda*(u1x+u3z)))
                    u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)-(s23e-mu*(u3y+
     & u2z))
                  else
                    u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)-(s31e-mu*(u3x+
     & u1z))
                    u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)-(s32e-mu*(u3y+
     & u2z))
                    u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)-(s33e-(kappa*
     & u3z+lambda*(u1x+u2y)))
                  end if
                 end if
                 end do
                 end do
                 end do
              else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                 if( mask(i1,i2,i3).gt.0 )then
                  call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,uc,u1x )
                  call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,uc,u1y )
                  call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,uc,u1z )

                  call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,vc,u2x )
                  call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,vc,u2y )
                  call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,vc,u2z )

                  call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,wc,u3x )
                  call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,wc,u3y )
                  call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,wc,u3z )

                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s11c,s11e )
                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s12c,s12e )
                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s13c,s13e )

                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s21c,s21e )
                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s22c,s22e )
                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s23c,s23e )

                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s31c,s31e )
                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s32c,s32e )
                  call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),xy(i1,i2,i3,2),t,s33c,s33e )

                  tn1 = rx(i1,i2,i3,edgeDirection,0)
                  tn2 = rx(i1,i2,i3,edgeDirection,1)
                  tn3 = rx(i1,i2,i3,edgeDirection,2)
                  aNormi = 1.0/max(epsx,sqrt(tn1**2+tn2**2+tn3**2))
                  tn1 = tn1*aNormi
                  tn2 = tn2*aNormi
                  tn3 = tn3*aNormi

                  f1 = tn1*(s11e-(kappa*u1x+lambda*(u2y+u3z)))+tn2*(
     & s21e-mu*(u1y+u2x))                +tn3*(s31e-mu*(u3x+u1z))
                  f2 = tn1*(s12e-mu*(u2x+u1y))                +tn2*(
     & s22e-(kappa*u2y+lambda*(u1x+u3z)))+tn3*(s32e-mu*(u3y+u2z))
                  f3 = tn1*(s13e-mu*(u3x+u1z))                +tn2*(
     & s23e-mu*(u3y+u2z))                +tn3*(s33e-(kappa*u3z+lambda*
     & (u1x+u2y)))

c                  u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)-tn1*f1
c                  u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)-tn1*f2
c                  u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)-tn1*f3
c
c                  u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)-tn2*f1
c                  u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)-tn2*f2
c                  u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)-tn2*f3
c
c                  u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)-tn3*f1
c                  u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)-tn3*f2
c                  u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)-tn3*f3
ccc
                  u(i1,i2,i3,s11c) = s11e
                  u(i1,i2,i3,s12c) = s12e
                  u(i1,i2,i3,s13c) = s13e
c
                  u(i1,i2,i3,s21c) = s21e
                  u(i1,i2,i3,s22c) = s22e
                  u(i1,i2,i3,s23c) = s23e
c
                  u(i1,i2,i3,s31c) = s31e
                  u(i1,i2,i3,s32c) = s32e
                  u(i1,i2,i3,s33c) = s33e
                 end if
                 end do
                 end do
                 end do
              end if ! gridType
            end if ! bcTypes
                  end do
                  end do
                end do
        end if
c
c.. re-compute stress at traction-traction edges
              do edgeDirection = 0,2 ! direction parallel to the edge
                do sidea = 0,1
                do sideb = 0,1
                  if( edgeDirection.eq.0 ) then
                    side1 = 0
                    side2 = sidea
                    side3 = sideb
                  else if( edgeDirection.eq.1 ) then
                    side1 = sideb
                    side2 = 0
                    side3 = sidea
                  else
                    side1 = sidea
                    side2 = sideb
                    side3 = 0
                  end if
                  is1 = 1-2*(side1)
                  is2 = 1-2*(side2)
                  is3 = 1-2*(side3)
                  if( edgeDirection.eq.2 ) then
                    is3 = 0
                    n1a = gridIndexRange(side1,0)
                    n1b = gridIndexRange(side1,0)
                    n2a = gridIndexRange(side2,1)
                    n2b = gridIndexRange(side2,1)
                    n3a = gridIndexRange(    0,2)
                    n3b = gridIndexRange(    1,2)
                    bc1 = boundaryCondition(side1,0)
                    bc2 = boundaryCondition(side2,1)
                  else if( edgeDirection.eq.1 )then
                    is2 = 0
                    n1a = gridIndexRange(side1,0)
                    n1b = gridIndexRange(side1,0)
                    n2a = gridIndexRange(    0,1)
                    n2b = gridIndexRange(    1,1)
                    n3a = gridIndexRange(side3,2)
                    n3b = gridIndexRange(side3,2)
                    bc1 = boundaryCondition(side1,0)
                    bc2 = boundaryCondition(side3,2)
                  else
                    is1 = 0
                    n1a = gridIndexRange(    0,0)
                    n1b = gridIndexRange(    1,0)
                    n2a = gridIndexRange(side2,1)
                    n2b = gridIndexRange(side2,1)
                    n3a = gridIndexRange(side3,2)
                    n3b = gridIndexRange(side3,2)
                    bc1 = boundaryCondition(side2,1)
                    bc2 = boundaryCondition(side3,2)
                  end if
          if( gridType.eq.rectangular ) then
            ! do nothing ...
          else
            if( bc1.eq.tractionBC .and. bc2.eq.tractionBC ) then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
               if( mask(i1,i2,i3).gt.0 )then
                if( edgeDirection.eq.0 ) then
                  norm1(0) = -is2*rx(i1,i2,i3,1,0)
                  norm1(1) = -is2*rx(i1,i2,i3,1,1)
                  norm1(2) = -is2*rx(i1,i2,i3,1,2)
                  f11 = bcf(side2,1,i1,i2,i3,s11c)
                  f21 = bcf(side2,1,i1,i2,i3,s12c)
                  f31 = bcf(side2,1,i1,i2,i3,s13c)

                  norm2(0) = -is3*rx(i1,i2,i3,2,0)
                  norm2(1) = -is3*rx(i1,i2,i3,2,1)
                  norm2(2) = -is3*rx(i1,i2,i3,2,2)
                  f12 = bcf(side3,2,i1,i2,i3,s11c)
                  f22 = bcf(side3,2,i1,i2,i3,s12c)
                  f32 = bcf(side3,2,i1,i2,i3,s13c)
                else if( edgeDirection.eq.1 ) then
                  norm1(0) = -is3*rx(i1,i2,i3,2,0)
                  norm1(1) = -is3*rx(i1,i2,i3,2,1)
                  norm1(2) = -is3*rx(i1,i2,i3,2,2)
                  f11 = bcf(side3,2,i1,i2,i3,s11c)
                  f21 = bcf(side3,2,i1,i2,i3,s12c)
                  f31 = bcf(side3,2,i1,i2,i3,s13c)

                  norm2(0) = -is1*rx(i1,i2,i3,0,0)
                  norm2(1) = -is1*rx(i1,i2,i3,0,1)
                  norm2(2) = -is1*rx(i1,i2,i3,0,2)
                  f12 = bcf(side1,0,i1,i2,i3,s11c)
                  f22 = bcf(side1,0,i1,i2,i3,s12c)
                  f32 = bcf(side1,0,i1,i2,i3,s13c)
                else
                  norm1(0) = -is1*rx(i1,i2,i3,0,0)
                  norm1(1) = -is1*rx(i1,i2,i3,0,1)
                  norm1(2) = -is1*rx(i1,i2,i3,0,2)
                  f11 = bcf(side1,0,i1,i2,i3,s11c)
                  f21 = bcf(side1,0,i1,i2,i3,s12c)
                  f31 = bcf(side1,0,i1,i2,i3,s13c)

                  norm2(0) = -is2*rx(i1,i2,i3,1,0)
                  norm2(1) = -is2*rx(i1,i2,i3,1,1)
                  norm2(2) = -is2*rx(i1,i2,i3,1,2)
                  f12 = bcf(side2,1,i1,i2,i3,s11c)
                  f22 = bcf(side2,1,i1,i2,i3,s12c)
                  f32 = bcf(side2,1,i1,i2,i3,s13c)
                end if

                aNormi = 1.0/max(epsx,sqrt(norm1(0)**2+norm1(1)**2+
     & norm1(2)**2))
                norm1(0) = norm1(0)*aNormi
                norm1(1) = norm1(1)*aNormi
                norm1(2) = norm1(2)*aNormi

                aNormi = 1.0/max(epsx,sqrt(norm2(0)**2+norm2(1)**2+
     & norm2(2)**2))
                norm2(0) = norm2(0)*aNormi
                norm2(1) = norm2(1)*aNormi
                norm2(2) = norm2(2)*aNormi

                b11 = f11-(norm1(0)*u(i1,i2,i3,s11c)+norm1(1)*u(i1,i2,
     & i3,s21c)+norm1(2)*u(i1,i2,i3,s31c))
                b21 = f21-(norm1(0)*u(i1,i2,i3,s12c)+norm1(1)*u(i1,i2,
     & i3,s22c)+norm1(2)*u(i1,i2,i3,s32c))
                b31 = f31-(norm1(0)*u(i1,i2,i3,s13c)+norm1(1)*u(i1,i2,
     & i3,s23c)+norm1(2)*u(i1,i2,i3,s33c))

                dot1 = norm1(0)*norm2(0)+norm1(1)*norm2(1)+norm1(2)*
     & norm2(2)
                dot2 = -sin(acos(dot1))

                b12 = (f12-(norm2(0)*u(i1,i2,i3,s11c)+norm2(1)*u(i1,i2,
     & i3,s21c)+norm2(2)*u(i1,i2,i3,s31c))-dot1*b11)/dot2
                b22 = (f22-(norm2(0)*u(i1,i2,i3,s12c)+norm2(1)*u(i1,i2,
     & i3,s22c)+norm2(2)*u(i1,i2,i3,s32c))-dot1*b21)/dot2
                b32 = (f32-(norm2(0)*u(i1,i2,i3,s13c)+norm2(1)*u(i1,i2,
     & i3,s23c)+norm2(2)*u(i1,i2,i3,s33c))-dot1*b31)/dot2

                u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)+norm1(0)*b11+norm2(
     & 0)*b12
                u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)+norm1(0)*b21+norm2(
     & 0)*b22
                u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)+norm1(0)*b31+norm2(
     & 0)*b32

                u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)+norm1(1)*b11+norm2(
     & 1)*b12
                u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)+norm1(1)*b21+norm2(
     & 1)*b22
                u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)+norm1(1)*b31+norm2(
     & 1)*b32

                u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)+norm1(2)*b11+norm2(
     & 2)*b12
                u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)+norm1(2)*b21+norm2(
     & 2)*b22
                u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)+norm1(2)*b31+norm2(
     & 2)*b32

               end if
               end do
               end do
               end do
            end if ! bcTypes
          end if ! gridType
                end do
                end do
              end do
c        end if

c.. set exact corner conditions for now
      if( setCornersWithTZ .and.twilightZone.ne.0 ) then ! *wdh* 090909
       write(*,'(" bcOptSmFOS3D: INFO set exact values on corners")')
            n1a = gridIndexRange(0,0)-0
            n1b = gridIndexRange(1,0)+0
            n2a = gridIndexRange(0,1)-0
            n2b = gridIndexRange(1,1)+0
            n3a = gridIndexRange(0,2)-0
            n3b = gridIndexRange(1,2)+0
c
            stride1 = n1b-n1a
            stride2 = n2b-n2a
            stride3 = n3b-n3a
c
            side1 = 0
            side2 = 1
c
            do i3 = n3a,n3b,stride3
            do i2 = n2a,n2b,stride2
            do i1 = n1a,n1b,stride1
              if( i1.eq.n1a ) then
                if( i2.eq.n2a ) then
                  if( i3.eq.n3a ) then
                    ! (0,0,0)
                    bc1 = boundaryCondition( side1,axis1 )
                    bc2 = boundaryCondition( side1,axis2 )
                    bc3 = boundaryCondition( side1,axis3 )
                  else
                    ! (0,0,1)
                    bc1 = boundaryCondition( side1,axis1 )
                    bc2 = boundaryCondition( side1,axis2 )
                    bc3 = boundaryCondition( side2,axis3 )
                  end if
                else
                  if( i3.eq.n3a ) then
                    ! (0,1,0)
                    bc1 = boundaryCondition( side1,axis1 )
                    bc2 = boundaryCondition( side2,axis2 )
                    bc3 = boundaryCondition( side1,axis3 )
                  else
                    ! (0,1,1)
                    bc1 = boundaryCondition( side1,axis1 )
                    bc2 = boundaryCondition( side2,axis2 )
                    bc3 = boundaryCondition( side2,axis3 )
                  end if
                end if
              else
                if( i2.eq.n2a ) then
                  if( i3.eq.n3a ) then
                    ! (1,0,0)
                    bc1 = boundaryCondition( side2,axis1 )
                    bc2 = boundaryCondition( side1,axis2 )
                    bc3 = boundaryCondition( side1,axis3 )
                  else
                    ! (1,0,1)
                    bc1 = boundaryCondition( side2,axis1 )
                    bc2 = boundaryCondition( side1,axis2 )
                    bc3 = boundaryCondition( side2,axis3 )
                  end if
                else
                  if( i3.eq.n3a ) then
                    ! (1,1,0)
                    bc1 = boundaryCondition( side2,axis1 )
                    bc2 = boundaryCondition( side2,axis2 )
                    bc3 = boundaryCondition( side1,axis3 )
                  else
                    ! (1,1,1)
                    bc1 = boundaryCondition( side2,axis1 )
                    bc2 = boundaryCondition( side2,axis2 )
                    bc3 = boundaryCondition( side2,axis3 )
                  end if
                end if
              end if
        ! *wdh* Need to check the boundaryConditions on the adjacent faces before applying these values: 
        if( mask(i1,i2,i3).gt.0 ) then
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,uc,ue )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,vc,ve )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,wc,we )

          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,v1c,v1e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,v2c,v2e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,v3c,v3e )

          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s11c,tau11e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s21c,tau21e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s31c,tau31e )

          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s12c,tau12e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s22c,tau22e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s32c,tau32e )

          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s13c,tau13e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s23c,tau23e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,
     & i2,i3,2),t,s33c,tau33e )

          u(i1,i2,i3,uc) = ue
          u(i1,i2,i3,vc) = ve
          u(i1,i2,i3,wc) = we

          u(i1,i2,i3,v1c) = v1e
          u(i1,i2,i3,v2c) = v2e
          u(i1,i2,i3,v3c) = v3e

          u(i1,i2,i3,s11c) = tau11e
          u(i1,i2,i3,s21c) = tau21e
          u(i1,i2,i3,s31c) = tau31e

          u(i1,i2,i3,s12c) = tau12e
          u(i1,i2,i3,s22c) = tau22e
          u(i1,i2,i3,s32c) = tau32e

          u(i1,i2,i3,s13c) = tau13e
          u(i1,i2,i3,s23c) = tau23e
          u(i1,i2,i3,s33c) = tau33e
        end if
            end do
            end do
            end do
      end if
c*******
c******* re-extrapolation components of stress to first ghost line ********
c*******

         extra1a=numGhost
         extra1b=numGhost
         extra2a=numGhost
         extra2b=numGhost
         if( nd.eq.3 )then
           extra3a=numGhost
           extra3b=numGhost
         else
           extra3a=0
           extra3b=0
         end if
         if( boundaryCondition(0,0).lt.0 )then
           extra1a=max(0,extra1a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
         else if( boundaryCondition(0,0).eq.0 )then
           extra1a=numGhost  ! include interpolation points since we assign ghost points outside these
         end if
         ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
         if( boundaryCondition(1,0).lt.0 )then
           extra1b=max(0,extra1b) ! over-ride numGhost=-1 : assign ends in periodic directions
         else if( boundaryCondition(1,0).eq.0 )then
           extra1b=numGhost
         end if
         if( boundaryCondition(0,1).lt.0 )then
           extra2a=max(0,extra2a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
         else if( boundaryCondition(0,1).eq.0 )then
           extra2a=numGhost  ! include interpolation points since we assign ghost points outside these
         end if
         ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
         if( boundaryCondition(1,1).lt.0 )then
           extra2b=max(0,extra2b) ! over-ride numGhost=-1 : assign ends in periodic directions
         else if( boundaryCondition(1,1).eq.0 )then
           extra2b=numGhost
         end if
         if(  nd.eq.3 )then
          if( boundaryCondition(0,2).lt.0 )then
            extra3a=max(0,extra3a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
          else if( boundaryCondition(0,2).eq.0 )then
            extra3a=numGhost  ! include interpolation points since we assign ghost points outside these
          end if
          ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
          if( boundaryCondition(1,2).lt.0 )then
            extra3b=max(0,extra3b) ! over-ride numGhost=-1 : assign ends in periodic directions
          else if( boundaryCondition(1,2).eq.0 )then
            extra3b=numGhost
          end if
         end if
         do axis=0,nd-1
         do side=0,1
           if( boundaryCondition(side,axis).gt.0 )then
             ! write(*,'(" bcOpt: side,axis,bc=",3i2)') side,axis,boundaryCondition(side,axis)
             n1a=gridIndexRange(0,0)
             n1b=gridIndexRange(1,0)
             n2a=gridIndexRange(0,1)
             n2b=gridIndexRange(1,1)
             n3a=gridIndexRange(0,2)
             n3b=gridIndexRange(1,2)
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
             nn1a=gridIndexRange(0,0)-extra1a
             nn1b=gridIndexRange(1,0)+extra1b
             nn2a=gridIndexRange(0,1)-extra2a
             nn2b=gridIndexRange(1,1)+extra2b
             nn3a=gridIndexRange(0,2)-extra3a
             nn3b=gridIndexRange(1,2)+extra3b
             if( axis.eq.0 )then
               nn1a=gridIndexRange(side,axis)
               nn1b=gridIndexRange(side,axis)
             else if( axis.eq.1 )then
               nn2a=gridIndexRange(side,axis)
               nn2b=gridIndexRange(side,axis)
             else
               nn3a=gridIndexRange(side,axis)
               nn3b=gridIndexRange(side,axis)
             end if
             is=1-2*side
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
             i3=n3a
        !*      ! (js1,js2,js3) used to compute tangential derivatives
        !*      js1=0
        !*      js2=0
        !*      js3=0
        !*      if( axisp1.eq.0 )then
        !*        js1=1-2*side
        !*      else if( axisp1.eq.1 )then
        !*        js2=1-2*side
        !*      else if( axisp1.eq.2 )then
        !*        js3=1-2*side
        !*      else
        !*        stop 5
        !*      end if
        !* 
        !*      ! (ks1,ks2,ks3) used to compute second tangential derivative
        !*      ks1=0
        !*      ks2=0
        !*      ks3=0
        !*      if( axisp2.eq.0 )then
        !*        ks1=1-2*side
        !*      else if( axisp2.eq.1 )then
        !*        ks2=1-2*side
        !*      else if( axisp2.eq.2 )then
        !*        ks3=1-2*side
        !*      else
        !*        stop 5
        !*      end if
             if( debug.gt.7 )then
               write(*,'(" bcOpt: grid,side,axis=",3i3,", loop bounds: 
     & n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,n1a,n1b,n2a,
     & n2b,n3a,n3b
             end if
           end if ! if bc>0
          if( boundaryCondition(side,axis)
     & .eq.tractionBC.or.boundaryCondition(side,axis).eq.slipWall ) 
     & then
             do i3=n3a,n3b
             do i2=n2a,n2b
             do i1=n1a,n1b
            if( mask(i1,i2,i3).ne.0 ) then
                u(i1-is1,i2-is2,i3-is3,s11c) = (3.*u(i1,i2,i3,s11c)-3.*
     & u(i1+is1,i2+is2,i3+is3,s11c)+u(i1+2*is1,i2+2*is2,i3+2*is3,s11c)
     & )
                u(i1-is1,i2-is2,i3-is3,s12c) = (3.*u(i1,i2,i3,s12c)-3.*
     & u(i1+is1,i2+is2,i3+is3,s12c)+u(i1+2*is1,i2+2*is2,i3+2*is3,s12c)
     & )
                u(i1-is1,i2-is2,i3-is3,s13c) = (3.*u(i1,i2,i3,s13c)-3.*
     & u(i1+is1,i2+is2,i3+is3,s13c)+u(i1+2*is1,i2+2*is2,i3+2*is3,s13c)
     & )

                u(i1-is1,i2-is2,i3-is3,s21c) = (3.*u(i1,i2,i3,s21c)-3.*
     & u(i1+is1,i2+is2,i3+is3,s21c)+u(i1+2*is1,i2+2*is2,i3+2*is3,s21c)
     & )
                u(i1-is1,i2-is2,i3-is3,s22c) = (3.*u(i1,i2,i3,s22c)-3.*
     & u(i1+is1,i2+is2,i3+is3,s22c)+u(i1+2*is1,i2+2*is2,i3+2*is3,s22c)
     & )
                u(i1-is1,i2-is2,i3-is3,s23c) = (3.*u(i1,i2,i3,s23c)-3.*
     & u(i1+is1,i2+is2,i3+is3,s23c)+u(i1+2*is1,i2+2*is2,i3+2*is3,s23c)
     & )

                u(i1-is1,i2-is2,i3-is3,s31c) = (3.*u(i1,i2,i3,s31c)-3.*
     & u(i1+is1,i2+is2,i3+is3,s31c)+u(i1+2*is1,i2+2*is2,i3+2*is3,s31c)
     & )
                u(i1-is1,i2-is2,i3-is3,s32c) = (3.*u(i1,i2,i3,s32c)-3.*
     & u(i1+is1,i2+is2,i3+is3,s32c)+u(i1+2*is1,i2+2*is2,i3+2*is3,s32c)
     & )
                u(i1-is1,i2-is2,i3-is3,s33c) = (3.*u(i1,i2,i3,s33c)-3.*
     & u(i1+is1,i2+is2,i3+is3,s33c)+u(i1+2*is1,i2+2*is2,i3+2*is3,s33c)
     & )
              end if
             end do
             end do
             end do

          end if ! bc
         end do ! end side
         end do ! end axis

c.. set the corners to the exact twilight zone function for testing ... CHANGE ME!! ...
        if( .false. ) then
              n1a = gridIndexRange(0,0)-1
              n1b = gridIndexRange(1,0)+1
              n2a = gridIndexRange(0,1)-1
              n2b = gridIndexRange(1,1)+1
              n3a = gridIndexRange(0,2)-1
              n3b = gridIndexRange(1,2)+1
c
              do dir = 0,nd-1
                stride1 = n1b-n1a
                stride2 = n2b-n2a
                stride3 = n3b-n3a
                if( dir.eq.0 ) then
                  stride1 = 1
                else if( dir.eq.1 ) then
                  stride2 = 1
                else if( dir.eq.2 ) then
                  stride3 = 1
                else
                  stop 5
                end if
c
                do i3 = n3a,n3b,stride3
                do i2 = n2a,n2b,stride2
                do i1 = n1a,n1b,stride1
          if( mask(i1,i2,i3).gt.0 ) then
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,uc,ue )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,vc,ve )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,wc,we )

            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,v1c,v1e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,v2c,v2e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,v3c,v3e )

            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s11c,tau11e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s21c,tau21e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s31c,tau31e )

            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s12c,tau12e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s22c,tau22e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s32c,tau32e )

            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s13c,tau13e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s23c,tau23e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s33c,tau33e )

            u(i1,i2,i3,uc) = ue
            u(i1,i2,i3,vc) = ve
            u(i1,i2,i3,wc) = we

            u(i1,i2,i3,v1c) = v1e
            u(i1,i2,i3,v2c) = v2e
            u(i1,i2,i3,v3c) = v3e

            u(i1,i2,i3,s11c) = tau11e
            u(i1,i2,i3,s21c) = tau21e
            u(i1,i2,i3,s31c) = tau31e

            u(i1,i2,i3,s12c) = tau12e
            u(i1,i2,i3,s22c) = tau22e
            u(i1,i2,i3,s32c) = tau32e

            u(i1,i2,i3,s13c) = tau13e
            u(i1,i2,i3,s23c) = tau23e
            u(i1,i2,i3,s33c) = tau33e
          end if
                end do
                end do
                end do
              end do
      end if

c*******
c******* Extrapolation to the second ghost line ********
c*******

         extra1a=numGhost
         extra1b=numGhost
         extra2a=numGhost
         extra2b=numGhost
         if( nd.eq.3 )then
           extra3a=numGhost
           extra3b=numGhost
         else
           extra3a=0
           extra3b=0
         end if
         if( boundaryCondition(0,0).lt.0 )then
           extra1a=max(0,extra1a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
         else if( boundaryCondition(0,0).eq.0 )then
           extra1a=numGhost  ! include interpolation points since we assign ghost points outside these
         end if
         ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
         if( boundaryCondition(1,0).lt.0 )then
           extra1b=max(0,extra1b) ! over-ride numGhost=-1 : assign ends in periodic directions
         else if( boundaryCondition(1,0).eq.0 )then
           extra1b=numGhost
         end if
         if( boundaryCondition(0,1).lt.0 )then
           extra2a=max(0,extra2a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
         else if( boundaryCondition(0,1).eq.0 )then
           extra2a=numGhost  ! include interpolation points since we assign ghost points outside these
         end if
         ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
         if( boundaryCondition(1,1).lt.0 )then
           extra2b=max(0,extra2b) ! over-ride numGhost=-1 : assign ends in periodic directions
         else if( boundaryCondition(1,1).eq.0 )then
           extra2b=numGhost
         end if
         if(  nd.eq.3 )then
          if( boundaryCondition(0,2).lt.0 )then
            extra3a=max(0,extra3a) ! over-ride numGhost=-1 : assign ends in periodic directions (or internal parallel boundaries)
          else if( boundaryCondition(0,2).eq.0 )then
            extra3a=numGhost  ! include interpolation points since we assign ghost points outside these
          end if
          ! **NOTE** the bc on the right may be negative even it is not on the left (for parallel)
          if( boundaryCondition(1,2).lt.0 )then
            extra3b=max(0,extra3b) ! over-ride numGhost=-1 : assign ends in periodic directions
          else if( boundaryCondition(1,2).eq.0 )then
            extra3b=numGhost
          end if
         end if
         do axis=0,nd-1
         do side=0,1
           if( boundaryCondition(side,axis).gt.0 )then
             ! write(*,'(" bcOpt: side,axis,bc=",3i2)') side,axis,boundaryCondition(side,axis)
             n1a=gridIndexRange(0,0)
             n1b=gridIndexRange(1,0)
             n2a=gridIndexRange(0,1)
             n2b=gridIndexRange(1,1)
             n3a=gridIndexRange(0,2)
             n3b=gridIndexRange(1,2)
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
             nn1a=gridIndexRange(0,0)-extra1a
             nn1b=gridIndexRange(1,0)+extra1b
             nn2a=gridIndexRange(0,1)-extra2a
             nn2b=gridIndexRange(1,1)+extra2b
             nn3a=gridIndexRange(0,2)-extra3a
             nn3b=gridIndexRange(1,2)+extra3b
             if( axis.eq.0 )then
               nn1a=gridIndexRange(side,axis)
               nn1b=gridIndexRange(side,axis)
             else if( axis.eq.1 )then
               nn2a=gridIndexRange(side,axis)
               nn2b=gridIndexRange(side,axis)
             else
               nn3a=gridIndexRange(side,axis)
               nn3b=gridIndexRange(side,axis)
             end if
             is=1-2*side
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
             i3=n3a
        !*      ! (js1,js2,js3) used to compute tangential derivatives
        !*      js1=0
        !*      js2=0
        !*      js3=0
        !*      if( axisp1.eq.0 )then
        !*        js1=1-2*side
        !*      else if( axisp1.eq.1 )then
        !*        js2=1-2*side
        !*      else if( axisp1.eq.2 )then
        !*        js3=1-2*side
        !*      else
        !*        stop 5
        !*      end if
        !* 
        !*      ! (ks1,ks2,ks3) used to compute second tangential derivative
        !*      ks1=0
        !*      ks2=0
        !*      ks3=0
        !*      if( axisp2.eq.0 )then
        !*        ks1=1-2*side
        !*      else if( axisp2.eq.1 )then
        !*        ks2=1-2*side
        !*      else if( axisp2.eq.2 )then
        !*        ks3=1-2*side
        !*      else
        !*        stop 5
        !*      end if
             if( debug.gt.7 )then
               write(*,'(" bcOpt: grid,side,axis=",3i3,", loop bounds: 
     & n1a,n1b,n2a,n2b,n3a,n3b=",6i3)') grid,side,axis,n1a,n1b,n2a,
     & n2b,n3a,n3b
             end if
           end if ! if bc>0
         if( boundaryCondition(side,axis).gt.0 ) then
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
             if( mask(i1,i2,i3).ne.0 ) then
               do n=0,numberOfComponents-1
                 u(i1-2*is1,i2-2*is2,i3-2*is3,n)=(3.*u(i1-is1,i2-is2,
     & i3-is3,n)-3.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,n)+u(i1-is1+2*
     & is1,i2-is2+2*is2,i3-is3+2*is3,n))
               end do
             end if
            end do
            end do
            end do
         end if ! bc
         end do ! end side
         end do ! end axis

c..extrapolate the 2nd ghost line near the corners.
        do side1=0,1
          i1 = gridIndexRange(side1,axis1)
          is1 = 1-2*side1
          do side2=0,1
            i2 = gridIndexRange(side2,axis2)
            is2 = 1-2*side2
            do side3=0,1
              i3 = gridIndexRange(side3,axis3)
              is3 = 1-2*side3

c extrapolate in the i1 direction
              if( boundaryCondition(side1,axis1).gt.0 ) then
                if( mask(i1,i2,i3).ne.0 ) then
                  do n=0,numberOfComponents-1
                    u(i1-2*is1,i2-is2,i3-is3,n)=(3.*u(i1-is1,i2-is2,i3-
     & is3,n)-3.*u(i1-is1+is1,i2-is2+0,i3-is3+0,n)+u(i1-is1+2*is1,i2-
     & is2+2*0,i3-is3+2*0,n))
                  end do
                end if
              end if

c extrapolate in the i2 direction
              if( boundaryCondition(side2,axis2).gt.0 ) then
                if( mask(i1,i2,i3).ne.0 ) then
                  do n=0,numberOfComponents-1
                    u(i1-is1,i2-2*is2,i3-is3,n)=(3.*u(i1-is1,i2-is2,i3-
     & is3,n)-3.*u(i1-is1+0,i2-is2+is2,i3-is3+0,n)+u(i1-is1+2*0,i2-
     & is2+2*is2,i3-is3+2*0,n))
                  end do
                end if
              end if

c extrapolate in the i3 direction
              if( boundaryCondition(side3,axis3).gt.0 ) then
                if( mask(i1,i2,i3).ne.0 ) then
                  do n=0,numberOfComponents-1
                    u(i1-is1,i2-is2,i3-2*is3,n)=(3.*u(i1-is1,i2-is2,i3-
     & is3,n)-3.*u(i1-is1+0,i2-is2+0,i3-is3+is3,n)+u(i1-is1+2*0,i2-
     & is2+2*0,i3-is3+2*is3,n))
                  end do
                end if
              end if

c extrapolate in the diagonal direction
              if( boundaryCondition(side1,axis1)
     & .gt.0.and.boundaryCondition(side2,axis2)
     & .gt.0.and.boundaryCondition(side3,axis3).gt.0) then
                if( mask(i1,i2,i3).ne.0 ) then
                  do n=0,numberOfComponents-1
                    u(i1-2*is1,i2-2*is2,i3-2*is3,n)=(3.*u(i1-is1,i2-
     & is2,i3-is3,n)-3.*u(i1-is1+is1,i2-is2+is2,i3-is3+is3,n)+u(i1-
     & is1+2*is1,i2-is2+2*is2,i3-is3+2*is3,n))
                  end do
                end if
              end if
            end do
          end do
        end do

        if( .false. ) then
c.. set the corners to the exact twilight zone function for testing ... CHANGE ME!! ...
              n1a = gridIndexRange(0,0)-1
              n1b = gridIndexRange(1,0)+1
              n2a = gridIndexRange(0,1)-1
              n2b = gridIndexRange(1,1)+1
              n3a = gridIndexRange(0,2)-1
              n3b = gridIndexRange(1,2)+1
c
              do dir = 0,nd-1
                stride1 = n1b-n1a
                stride2 = n2b-n2a
                stride3 = n3b-n3a
                if( dir.eq.0 ) then
                  stride1 = 1
                else if( dir.eq.1 ) then
                  stride2 = 1
                else if( dir.eq.2 ) then
                  stride3 = 1
                else
                  stop 5
                end if
c
                do i3 = n3a,n3b,stride3
                do i2 = n2a,n2b,stride2
                do i1 = n1a,n1b,stride1
          if( mask(i1,i2,i3).gt.0 ) then
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,uc,ue )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,vc,ve )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,wc,we )

            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,v1c,v1e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,v2c,v2e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,v3c,v3e )

            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s11c,tau11e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s21c,tau21e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s31c,tau31e )

            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s12c,tau12e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s22c,tau22e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s32c,tau32e )

            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s13c,tau13e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s23c,tau23e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(
     & i1,i2,i3,2),t,s33c,tau33e )

            u(i1,i2,i3,uc) = ue
            u(i1,i2,i3,vc) = ve
            u(i1,i2,i3,wc) = we

            u(i1,i2,i3,v1c) = v1e
            u(i1,i2,i3,v2c) = v2e
            u(i1,i2,i3,v3c) = v3e

            u(i1,i2,i3,s11c) = tau11e
            u(i1,i2,i3,s21c) = tau21e
            u(i1,i2,i3,s31c) = tau31e

            u(i1,i2,i3,s12c) = tau12e
            u(i1,i2,i3,s22c) = tau22e
            u(i1,i2,i3,s32c) = tau32e

            u(i1,i2,i3,s13c) = tau13e
            u(i1,i2,i3,s23c) = tau23e
            u(i1,i2,i3,s33c) = tau33e
          end if
                end do
                end do
                end do
              end do
      end if

c        stop 3366
        return
        end
