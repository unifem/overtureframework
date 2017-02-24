! This file automatically generated from bcOptSmFOS.bf with bpp.
        subroutine bcSmFOS2d( nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
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
        logical assignTangentStress,fixupTractionDisplacementCorners,
     & computeTractionOnDisplacementBoundaries
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
        ! ================================================================================
        assignTangentStress  =.false.  ! new option *dws* added 2015/07/13
        ! assignTangentStress  =.true. ! 
        ! Traction-displacement corners can have singularities in the traction.
        ! By default we now turn off the corner compatibility for non-linear solids
        fixupTractionDisplacementCorners = .true. ! *new* option *wdh* 2015/07/16
        if( bctype .eq. nonLinearBoundaryCondition )then
          fixupTractionDisplacementCorners = .false.
        end if
        ! Optionally compute the traction on ghost points next to displacement boundaries
        computeTractionOnDisplacementBoundaries=.true.
        if( bctype .eq. nonLinearBoundaryCondition )then
          computeTractionOnDisplacementBoundaries = .true. !   .false. is worse than true
        end if
        ! ==================================================================================
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
        if( debug.gt.3 )then
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
          ! *********************************** 
          ! **************** 2D ***************
          ! *********************************** 
           if( debug.gt.32 )then
            n1a=gridIndexRange(0,0)
            n1b=gridIndexRange(1,0)
            n2a=gridIndexRange(0,1)
            n2b=gridIndexRange(1,1)
            n3a=gridIndexRange(0,2)
            n3b=gridIndexRange(1,2)
             write(*,'("v1c",1x,"START")')
            ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,v1c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,v1c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'("v2c",1x,"START")')
            ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,v2c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,v2c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'("s11c",1x,"START")')
            ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s11c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s11c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'("s12c",1x,"START")')
            ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s12c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s12c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'("s22c",1x,"START")')
            ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s22c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s22c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
           end if
          !*******
          !******* Extrapolate to the first ghost cells (only for physical sides) ********
          !*******    Only for displacement and only for the SVK case
          !*******
          if ( bctype.ne.linearBoundaryCondition ) then
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
             ! *wdh* 101029 -- do not extrap dirichletBoundaryCondition
             if( boundaryCondition(side,axis).gt.0 .and. 
     & boundaryCondition(side,axis).ne.dirichletBoundaryCondition 
     & .and. boundaryCondition(side,axis).ne.symmetry )then
               i3=n3a
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
               if (mask(i1,i2,i3).ne.0) then  ! note: extrap outside interp pts too
                 ! u(i1-is1,i2-is2,i3,uc)=extrap3(u,i1,i2,i3,uc,is1,is2,is3)
                 ! u(i1-is1,i2-is2,i3,vc)=extrap3(u,i1,i2,i3,vc,is1,is2,is3)
                 ! *wdh* 2015/07/15 
                   ! here du2=2nd-order approximation, du3=third order
                   ! Blend the 2nd and 3rd order based on the difference 
                   !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                   du1 = u(i1,i2,i3,uc)
                   du2 = 2.*u(i1,i2,i3,uc)-u(i1+is1,i2+is2,i3,uc)
                   du3 = 3.*u(i1,i2,i3,uc)-3.*u(i1+is1,i2+is2,i3,uc)+u(
     & i1+2*is1,i2+2*is2,i3,uc)
                   !   alpha = cdl*(abs(du3-u(i1+is1,i2+is2,i3,uc))+abs(du3-du2))/(uEps+abs(u(i1+is1,i2+is2,i3,uc))+abs(u(i1+2*is1,i2+2*is2,i3,uc)))
                   ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1+is1,i2+is2,i3,uc))+abs(u(i1+2*is1,i2+2*is2,i3,uc)))
                   uNorm= uEps+ abs(du3) + abs(u(i1,i2,i3,uc))+abs(u(
     & i1+is1,i2+is2,i3,uc))
                   ! **  du = abs(du3-u(i1+is1,i2+is2,i3,uc))/uNorm  ! changed 050711
                   ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                   alpha = cdl*( abs(du3-du2)/uNorm )
                   alpha =min(1.,alpha)
                   ! if( mm.eq.1 )then
                 !  if (alpha.gt.0.9) then
                 !    write(6,*)'limiting, uc,du1,du3=',uc,du1,du3
                 !    write(6,*)'i1,i2,i3=',i1,i2,i3
                 !    write(6,*)'is1,is2,is3=',is1,is2,is3
                 !  end if
                   !   u(i1,i2,i3,uc)=(1.-alpha)*du3+alpha*du2
                   u(i1-is1,i2-is2,i3,uc)=(1.-alpha)*du3+alpha*du1
                   ! here du2=2nd-order approximation, du3=third order
                   ! Blend the 2nd and 3rd order based on the difference 
                   !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                   du1 = u(i1,i2,i3,vc)
                   du2 = 2.*u(i1,i2,i3,vc)-u(i1+is1,i2+is2,i3,vc)
                   du3 = 3.*u(i1,i2,i3,vc)-3.*u(i1+is1,i2+is2,i3,vc)+u(
     & i1+2*is1,i2+2*is2,i3,vc)
                   !   alpha = cdl*(abs(du3-u(i1+is1,i2+is2,i3,vc))+abs(du3-du2))/(uEps+abs(u(i1+is1,i2+is2,i3,vc))+abs(u(i1+2*is1,i2+2*is2,i3,vc)))
                   ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1+is1,i2+is2,i3,vc))+abs(u(i1+2*is1,i2+2*is2,i3,vc)))
                   uNorm= uEps+ abs(du3) + abs(u(i1,i2,i3,vc))+abs(u(
     & i1+is1,i2+is2,i3,vc))
                   ! **  du = abs(du3-u(i1+is1,i2+is2,i3,vc))/uNorm  ! changed 050711
                   ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                   alpha = cdl*( abs(du3-du2)/uNorm )
                   alpha =min(1.,alpha)
                   ! if( mm.eq.1 )then
                 !  if (alpha.gt.0.9) then
                 !    write(6,*)'limiting, vc,du1,du3=',vc,du1,du3
                 !    write(6,*)'i1,i2,i3=',i1,i2,i3
                 !    write(6,*)'is1,is2,is3=',is1,is2,is3
                 !  end if
                   !   u(i1,i2,i3,vc)=(1.-alpha)*du3+alpha*du2
                   u(i1-is1,i2-is2,i3,vc)=(1.-alpha)*du3+alpha*du1
               end if
               end do
               end do
             else if( boundaryCondition(side,axis).eq.symmetry )then  ! *wdh* 101108
              ! even symmetry
              if( twilightZone.eq.0 )then
                i3=n3a
                do i2=nn2a,nn2b
                do i1=nn1a,nn1b
                if (mask(i1,i2,i3).ne.0) then
                  u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc)
                  u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc)
  !      write(*,'("START symmetry: j1,j2, u=",2i4,2e14.6)') i1-is1,i2-is2, u(i1-is1,i2-is2,i3,uc),u(i1+is1,i2+is2,i3,uc)
                end if
                end do
                end do
              else
               ! TZ :
                i3=n3a
                do i2=nn2a,nn2b
                do i1=nn1a,nn1b
                if (mask(i1,i2,i3).ne.0) then
                  call ogDeriv(ep,0,0,0,0,xy(i1-is1,i2-is2,i3,0),xy(i1-
     & is1,i2-is2,i3,1),0.,t,uc,uem)
                  call ogDeriv(ep,0,0,0,0,xy(i1+is1,i2+is2,i3,0),xy(i1+
     & is1,i2+is2,i3,1),0.,t,uc,uep)
                  u(i1-is1,i2-is2,i3,uc)=u(i1+is1,i2+is2,i3,uc) + uem -
     &  uep
                  call ogDeriv(ep,0,0,0,0,xy(i1-is1,i2-is2,i3,0),xy(i1-
     & is1,i2-is2,i3,1),0.,t,vc,uem)
                  call ogDeriv(ep,0,0,0,0,xy(i1+is1,i2+is2,i3,0),xy(i1+
     & is1,i2+is2,i3,1),0.,t,vc,uep)
                  u(i1-is1,i2-is2,i3,vc)=u(i1+is1,i2+is2,i3,vc) + uem -
     &  uep
                end if
                end do
                end do
              end if
             end if ! bc
             end do ! end side
             end do ! end axis
          end if
          !*******
          !******* Extrapolation to the second ghost line ********
          !******* 
          ! ***NEW** wdh 2015/0715 
          !  We need to set the second ghost line for (u,v) for computing the
          !  the traction on the extended boundary (first ghost point) for nonlinear models
          !   -- this could be optiomized, no need to do all points ---
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
            if( boundaryCondition(side,axis).gt.0 .and. 
     & boundaryCondition(side,axis).ne.symmetry )then
              i3=n3a
              do i2=nn2a,nn2b
              do i1=nn1a,nn1b
              if (mask(i1,i2,i3).ne.0) then
               do n=0,numberOfComponents-1
           !      u(i1-2*is1,i2-2*is2,i3,n)=extrap3(u,i1-is1,i2-is2,i3,n,is1,is2,is3)
                    ! here du2=2nd-order approximation, du3=third order
                    ! Blend the 2nd and 3rd order based on the difference 
                    !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                    du1 = u(i1-is1,i2-is2,i3,n)
                    du2 = 2.*u(i1-is1,i2-is2,i3,n)-u(i1-is1+is1,i2-is2+
     & is2,i3,n)
                    du3 = 3.*u(i1-is1,i2-is2,i3,n)-3.*u(i1-is1+is1,i2-
     & is2+is2,i3,n)+u(i1-is1+2*is1,i2-is2+2*is2,i3,n)
                    !   alpha = cdl*(abs(du3-u(i1-is1+is1,i2-is2+is2,i3,n))+abs(du3-du2))/(uEps+abs(u(i1-is1+is1,i2-is2+is2,i3,n))+abs(u(i1-is1+2*is1,i2-is2+2*is2,i3,n)))
                    ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1-is1+is1,i2-is2+is2,i3,n))+abs(u(i1-is1+2*is1,i2-is2+2*is2,i3,n)))
                    uNorm= uEps+ abs(du3) + abs(u(i1-is1,i2-is2,i3,n))+
     & abs(u(i1-is1+is1,i2-is2+is2,i3,n))
                    ! **  du = abs(du3-u(i1-is1+is1,i2-is2+is2,i3,n))/uNorm  ! changed 050711
                    ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                    alpha = cdl*( abs(du3-du2)/uNorm )
                    alpha =min(1.,alpha)
                    ! if( mm.eq.1 )then
                  !  if (alpha.gt.0.9) then
                  !    write(6,*)'limiting, n,du1,du3=',n,du1,du3
                  !    write(6,*)'i1-is1,i2-is2,i3=',i1-is1,i2-is2,i3
                  !    write(6,*)'is1,is2,is3=',is1,is2,is3
                  !  end if
                    !   u(i1-is1,i2-is2,i3,n)=(1.-alpha)*du3+alpha*du2
                    u(i1-is1-is1,i2-is2-is2,i3,n)=(1.-alpha)*du3+alpha*
     & du1
               end do
              end if
              end do
              end do
            else if( boundaryCondition(side,axis).eq.symmetry )then  ! *wdh* 101108
             ! even symmetry 
             if( twilightZone.eq.0 )then
               i3=n3a
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
               if (mask(i1,i2,i3).ne.0) then
                do n=0,numberOfComponents-1
                  u(i1-2*is1,i2-2*is2,i3,n)=u(i1+2*is1,i2+2*is2,i3,n)
                end do
               end if
               end do
               end do
             else
              ! TZ :
               i3=n3a
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
               if (mask(i1,i2,i3).ne.0) then
                do n=0,numberOfComponents-1
                  call ogDeriv(ep,0,0,0,0,xy(i1-2*is1,i2-2*is2,i3,0),
     & xy(i1-2*is1,i2-2*is2,i3,1),0.,t,n,uem)
                  call ogDeriv(ep,0,0,0,0,xy(i1+2*is1,i2+2*is2,i3,0),
     & xy(i1+2*is1,i2+2*is2,i3,1),0.,t,n,uep)
                  u(i1-2*is1,i2-2*is2,i3,n)=u(i1+2*is1,i2+2*is2,i3,n) +
     &  uem - uep
                end do
               end if
               end do
               end do
             end if
            end if ! bc
            end do ! end side
            end do ! end axis
           !..extrapolate the 2nd ghost line near the corners
           i3=gridIndexRange(0,2)
           do side1=0,1
             i1=gridIndexRange(side1,axis1)
             is1=1-2*side1
             do side2=0,1
               i2=gridIndexRange(side2,axis2)
               is2=1-2*side2
               ! extrapolate in the i1 direction
               if (boundaryCondition(side1,axis1).gt.0) then
                 if (mask(i1,i2,i3).ne.0) then
                   do n=0,numberOfComponents-1
                    ! u(i1-2*is1,i2-is2,i3,n)=extrap3(u,i1-is1,i2-is2,i3,n,is1,0,0)
                       ! here du2=2nd-order approximation, du3=third order
                       ! Blend the 2nd and 3rd order based on the difference 
                       !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                       du1 = u(i1-is1,i2-0,i3,n)
                       du2 = 2.*u(i1-is1,i2-0,i3,n)-u(i1-is1+is1,i2-0+
     & 0,i3,n)
                       du3 = 3.*u(i1-is1,i2-0,i3,n)-3.*u(i1-is1+is1,i2-
     & 0+0,i3,n)+u(i1-is1+2*is1,i2-0+2*0,i3,n)
                       !   alpha = cdl*(abs(du3-u(i1-is1+is1,i2-0+0,i3,n))+abs(du3-du2))/(uEps+abs(u(i1-is1+is1,i2-0+0,i3,n))+abs(u(i1-is1+2*is1,i2-0+2*0,i3,n)))
                       ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1-is1+is1,i2-0+0,i3,n))+abs(u(i1-is1+2*is1,i2-0+2*0,i3,n)))
                       uNorm= uEps+ abs(du3) + abs(u(i1-is1,i2-0,i3,n))
     & +abs(u(i1-is1+is1,i2-0+0,i3,n))
                       ! **  du = abs(du3-u(i1-is1+is1,i2-0+0,i3,n))/uNorm  ! changed 050711
                       ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                       alpha = cdl*( abs(du3-du2)/uNorm )
                       alpha =min(1.,alpha)
                       ! if( mm.eq.1 )then
                     !  if (alpha.gt.0.9) then
                     !    write(6,*)'limiting, n,du1,du3=',n,du1,du3
                     !    write(6,*)'i1-is1,i2-0,i3=',i1-is1,i2-0,i3
                     !    write(6,*)'is1,0,0=',is1,0,0
                     !  end if
                       !   u(i1-is1,i2-0,i3,n)=(1.-alpha)*du3+alpha*du2
                       u(i1-is1-is1,i2-0-0,i3,n)=(1.-alpha)*du3+alpha*
     & du1
                   end do
                 end if
               end if
               !  extrapolate in the i2 direction
               if (boundaryCondition(side2,axis2).gt.0) then
                 if (mask(i1,i2,i3).ne.0) then
                   do n=0,numberOfComponents-1
                    ! u(i1-is1,i2-2*is2,i3,n)=extrap3(u,i1-is1,i2-is2,i3,n,0,is2,0)
                       ! here du2=2nd-order approximation, du3=third order
                       ! Blend the 2nd and 3rd order based on the difference 
                       !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                       du1 = u(i1-0,i2-is2,i3,n)
                       du2 = 2.*u(i1-0,i2-is2,i3,n)-u(i1-0+0,i2-is2+
     & is2,i3,n)
                       du3 = 3.*u(i1-0,i2-is2,i3,n)-3.*u(i1-0+0,i2-is2+
     & is2,i3,n)+u(i1-0+2*0,i2-is2+2*is2,i3,n)
                       !   alpha = cdl*(abs(du3-u(i1-0+0,i2-is2+is2,i3,n))+abs(du3-du2))/(uEps+abs(u(i1-0+0,i2-is2+is2,i3,n))+abs(u(i1-0+2*0,i2-is2+2*is2,i3,n)))
                       ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1-0+0,i2-is2+is2,i3,n))+abs(u(i1-0+2*0,i2-is2+2*is2,i3,n)))
                       uNorm= uEps+ abs(du3) + abs(u(i1-0,i2-is2,i3,n))
     & +abs(u(i1-0+0,i2-is2+is2,i3,n))
                       ! **  du = abs(du3-u(i1-0+0,i2-is2+is2,i3,n))/uNorm  ! changed 050711
                       ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                       alpha = cdl*( abs(du3-du2)/uNorm )
                       alpha =min(1.,alpha)
                       ! if( mm.eq.1 )then
                     !  if (alpha.gt.0.9) then
                     !    write(6,*)'limiting, n,du1,du3=',n,du1,du3
                     !    write(6,*)'i1-0,i2-is2,i3=',i1-0,i2-is2,i3
                     !    write(6,*)'0,is2,0=',0,is2,0
                     !  end if
                       !   u(i1-0,i2-is2,i3,n)=(1.-alpha)*du3+alpha*du2
                       u(i1-0-0,i2-is2-is2,i3,n)=(1.-alpha)*du3+alpha*
     & du1
                   end do
                 end if
               end if
               !  extrapolate in the diagonal direction
               if (boundaryCondition(side1,axis1)
     & .gt.0.and.boundaryCondition(side2,axis2).gt.0) then
                 if (mask(i1,i2,i3).ne.0) then
                   do n=0,numberOfComponents-1
                    ! u(i1-2*is1,i2-2*is2,i3,n)=extrap3(u,i1-is1,i2-is2,i3,n,is1,is2,0)
                       ! here du2=2nd-order approximation, du3=third order
                       ! Blend the 2nd and 3rd order based on the difference 
                       !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                       du1 = u(i1-is1,i2-is2,i3,n)
                       du2 = 2.*u(i1-is1,i2-is2,i3,n)-u(i1-is1+is1,i2-
     & is2+is2,i3,n)
                       du3 = 3.*u(i1-is1,i2-is2,i3,n)-3.*u(i1-is1+is1,
     & i2-is2+is2,i3,n)+u(i1-is1+2*is1,i2-is2+2*is2,i3,n)
                       !   alpha = cdl*(abs(du3-u(i1-is1+is1,i2-is2+is2,i3,n))+abs(du3-du2))/(uEps+abs(u(i1-is1+is1,i2-is2+is2,i3,n))+abs(u(i1-is1+2*is1,i2-is2+2*is2,i3,n)))
                       ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1-is1+is1,i2-is2+is2,i3,n))+abs(u(i1-is1+2*is1,i2-is2+2*is2,i3,n)))
                       uNorm= uEps+ abs(du3) + abs(u(i1-is1,i2-is2,i3,
     & n))+abs(u(i1-is1+is1,i2-is2+is2,i3,n))
                       ! **  du = abs(du3-u(i1-is1+is1,i2-is2+is2,i3,n))/uNorm  ! changed 050711
                       ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                       alpha = cdl*( abs(du3-du2)/uNorm )
                       alpha =min(1.,alpha)
                       ! if( mm.eq.1 )then
                     !  if (alpha.gt.0.9) then
                     !    write(6,*)'limiting, n,du1,du3=',n,du1,du3
                     !    write(6,*)'i1-is1,i2-is2,i3=',i1-is1,i2-is2,i3
                     !    write(6,*)'is1,is2,0=',is1,is2,0
                     !  end if
                       !   u(i1-is1,i2-is2,i3,n)=(1.-alpha)*du3+alpha*du2
                       u(i1-is1-is1,i2-is2-is2,i3,n)=(1.-alpha)*du3+
     & alpha*du1
                   end do
                 end if
               end if
             end do
           end do
          !*******
          !*****   Fill-in the boundary forcing array if they are not provided:
          !*****       bcfa(side,axis,i1,i2,i3,uc:*)
          !*******
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
             if( boundaryCondition(side,axis).eq.displacementBC )then
              if( addBoundaryForcing(side,axis).eq.0 )then
                i3=n3a
                do i2=nn2a,nn2b
                do i1=nn1a,nn1b
                if (mask(i1,i2,i3).ne.0) then
                 ! given displacements:
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(uc)))))  =0.
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(vc)))))  =0.
                 ! given velocities:
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v1c))))) =0.
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v2c))))) =0.
                 ! given acceleration: 
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c)))))=0.
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c)))))=0.
                end if
                end do
                end do
              else if( twilightZone.ne.0 )then
                i3=n3a
                do i2=nn2a,nn2b
                do i1=nn1a,nn1b
                if (mask(i1,i2,i3).ne.0) then
                 call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,uc,ue)
                 call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,vc,ve)
                 call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,v1c,v1e)
                 call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,v2c,v2e)
                 call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,s11c,tau11x)
                 call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,s21c,tau21y)
                 call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,s12c,tau12x)
                 call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,s22c,tau22y)
                 if (materialFormat.ne.constantMaterialProperties) then
                   call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,rhoc,rho)
                 end if
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(uc)))))=ue
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(vc)))))=ve
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v1c)))))=v1e
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v2c)))))=v2e
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c))))) = (
     & tau11x+tau21y)/rho
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c))))) = (
     & tau12x+tau22y)/rho
                 !       write(6,'(2(1x,i2),6(1x,1pe12.5))')i1,i2,ue,ve,v1e,v2e,tau11x+tau21y,tau12x+tau22y
                 ! write(*,'(" i1,i2=",2i3," set ue,bcf(uc)=",2e10.2)') i1,i2,ue,bcf(side,axis,i1,i2,i3,uc)
                end if
                end do
                end do
              end if
             else if( boundaryCondition(side,axis).eq.tractionBC )then
              if( addBoundaryForcing(side,axis).eq.0 )then
                i3=n3a
                do i2=nn2a,nn2b
                do i1=nn1a,nn1b
                if (mask(i1,i2,i3).ne.0) then
                 ! given traction (for the traction BC)
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c)))))=0.
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c)))))=0.
                 ! given traction (for determining displacements). Normally this is equal to the above
                 ! traction values except when using twilight-zone
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(uc)))))=0.
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(vc)))))=0.
                 ! given rate of change of traction (for determining the velocity)
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v1c)))))=0.
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v2c)))))=0.
                end if
                end do
                end do
              else if( twilightZone.ne.0 )then
                if ( bctype.eq.linearBoundaryCondition ) then          
     &     ! linear case
                  i3=n3a
                  do i2=nn2a,nn2b
                  do i1=nn1a,nn1b
                  if (mask(i1,i2,i3).ne.0) then
                   ! (an1,an2) = outward normal
                   if( gridType.eq.rectangular )then
                     if( axis.eq.0 )then
                       an1=-is
                       an2=0.
                     else
                       an1=0.
                       an2=-is
                     end if
                   else
                     aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2))
                     an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                     an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                   end if
                   if (materialFormat.ne.constantMaterialProperties) 
     & then
                     call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,muc,mu)
                     call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.,t,lambdac,lambda)
                   end if
                   call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,uc,uex)
                   call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,uc,uey)
                   call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,vc,vex)
                   call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,vc,vey)
                   bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(uc))))) = an1*
     & ( (lambda+2.*mu)*uex+lambda*vey ) + an2*( mu*(uey+vex) )
                   bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(vc))))) = an1*
     & ( mu*(uey+vex) ) + an2*( lambda*uex + (lambda+2.*mu)*vey )
                   call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,v1c,v1ex)
                   call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,v1c,v1ey)
                   call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,v2c,v2ex)
                   call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,v2c,v2ey)
                   bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v1c))))) = 
     & an1*( (lambda+2.*mu)*v1ex+lambda*v2ey ) + an2*( mu*(v1ey+v2ex) 
     & )
                   bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v2c))))) = 
     & an1*( mu*(v1ey+v2ex) ) + an2*( lambda*v1ex + (lambda+2.*mu)*
     & v2ey )
                   call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,s11c,tau11)
                   call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,s21c,tau21)
                   call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,s12c,tau12)
                   call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,s22c,tau22)
                   ! note : n_j sigma_ji  : sum over first index 
                   bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c))))) = 
     & an1*tau11+an2*tau21
                   bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c))))) = 
     & an1*tau12+an2*tau22
                  end if
                  end do
                  end do
                else              ! SVK case
                  i3=n3a
                  do i2=nn2a,nn2b
                  do i1=nn1a,nn1b
                  if (mask(i1,i2,i3).ne.0) then
                   call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,uc,uex)
                   call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,uc,uey)
                   call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,vc,vex)
                   call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,vc,vey)
                   call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,v1c,v1ex)
                   call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,v1c,v1ey)
                   call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,v2c,v2ex)
                   call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,v2c,v2ey)
                   call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,s11c,tau11)
                   call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,s21c,tau21)
                   call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,s12c,tau12)
                   call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,s22c,tau22)
                   ! (an1,an2) = outward normal
                   if( gridType.eq.rectangular )then
                     if( axis.eq.0 )then
                       an1=-is
                       an2=0.
                       alpha=sqrt(uey**2+(1.0+vey)**2)
                       dalpha=(uey*v1ey+(1.0+vey)*v2ey)/alpha
                     else
                       an1=0.
                       an2=-is
                       alpha=sqrt((1.0+uex)**2+vex**2)
                       dalpha=((1.0+uex)*v1ex+vex*v2ex)/alpha
                     end if
                   else
                     aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2))
                     an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                     an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                     coef1= an2*(1.0+uex)-an1*uey
                     coef2=-an2*vex+an1*(1.0+vey)
                     alpha=sqrt(coef1**2+coef2**2)
                     dalpha=(coef1*(an2*v1ex-an1*v1ey)+coef2*(-an2*
     & v2ex+an1*v2ey))/alpha
                   end if
                   ! note : n_j sigma_ji  : sum over first index 
                   bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c))))) = (
     & an1*tau11+an2*tau21)/alpha
                   bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c))))) = (
     & an1*tau12+an2*tau22)/alpha
                   !  call smbcsdp (uex,uey,vex,vey,lambda,mu,p,dpdf,1)
                   du(1,1)=uex
                   du(1,2)=uey
                   du(2,1)=vex
                   du(2,2)=vey
                   ideriv=1
                   call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                   bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(uc))))) = (
     & an1*p(1,1)+an2*p(2,1))/alpha
                   bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(vc))))) = (
     & an1*p(1,2)+an2*p(2,2))/alpha
                   coef1 = an1*(dpdf(1,1)*v1ex+dpdf(1,2)*v1ey+dpdf(1,3)
     & *v2ex+dpdf(1,4)*v2ey)+an2*(dpdf(3,1)*v1ex+dpdf(3,2)*v1ey+dpdf(
     & 3,3)*v2ex+dpdf(3,4)*v2ey)
                   coef2 = an1*(dpdf(2,1)*v1ex+dpdf(2,2)*v1ey+dpdf(2,3)
     & *v2ex+dpdf(2,4)*v2ey)+an2*(dpdf(4,1)*v1ex+dpdf(4,2)*v1ey+dpdf(
     & 4,3)*v2ex+dpdf(4,4)*v2ey)
                   bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v1c))))) = (
     & coef1-bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,0,
     & side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(1,
     & 1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(dim(
     & 1,2,side,axis)-dim(0,2,side,axis)+1)*(uc)))))*dalpha)/alpha
                   bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v2c))))) = (
     & coef2-bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,0,
     & side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(1,
     & 1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(dim(
     & 1,2,side,axis)-dim(0,2,side,axis)+1)*(vc)))))*dalpha)/alpha
                  end if
                  end do
                  end do
                end if
              else
               ! fill in the traction BC into the stress components  *wdh* 081109
               ! (this is needed since for TZ flow these values are different)
                i3=n3a
                do i2=nn2a,nn2b
                do i1=nn1a,nn1b
                if (mask(i1,i2,i3).ne.0) then
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c)))))=bcf0(
     & bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-
     & dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(1,1,side,
     & axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(dim(1,2,
     & side,axis)-dim(0,2,side,axis)+1)*(uc)))))
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c)))))=bcf0(
     & bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-
     & dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(1,1,side,
     & axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(dim(1,2,
     & side,axis)-dim(0,2,side,axis)+1)*(vc)))))
                end if
                end do
                end do
              end if
             else if( boundaryCondition(side,axis).eq.slipWall )then
               !  write(6,*)side,axis,addBoundaryForcing(side,axis)
               !  pause
              if( addBoundaryForcing(side,axis).eq.0 )then
                i3=n3a
                do i2=nn2a,nn2b
                do i1=nn1a,nn1b
                if (mask(i1,i2,i3).ne.0) then
                 ! given tangential stress (often zero)
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c)))))=0.
                 ! time rate of change of tangential stress
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c)))))=0.
                 ! given normal displacement
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(uc)))))=0.
                 ! time rate of change of normal displacement
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v1c)))))=0.
                end if
                end do
                end do
              else if( twilightZone.ne.0 )then
                i3=n3a
                do i2=nn2a,nn2b
                do i1=nn1a,nn1b
                if (mask(i1,i2,i3).ne.0) then
                 ! (an1,an2) = outward normal,  (-an2,an1) = tangent
                 if( gridType.eq.rectangular )then
                   if( axis.eq.0 )then
                     an1=-is
                     an2=0.
                   else
                     an1=0.
                     an2=-is
                   end if
                 else
                   aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2))
                   an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                   an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                 end if
                 call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,uc,ue)
                 call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,vc,ve)
                 ! save n.ue 
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(uc))))) = an1*ue+
     & an2*ve
                 call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,v1c,ue)
                 call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,v2c,ve)
                 ! save n.ve 
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(v1c))))) = an1*ue+
     & an2*ve
                 call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,s11c,tau11)
                 call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,s21c,tau21)
                 call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,s12c,tau12)
                 call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,s22c,tau22)
                 ! save n.sigma.tau  (tau=tangent) 
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c))))) = an1*(-
     & an2*tau11+an1*tau12)+an2*(-an2*tau21+an1*tau22)
                 call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,uc,uex)
                 call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,uc,uey)
                 call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,vc,vex)
                 call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,vc,vey)
                 if (materialFormat.ne.constantMaterialProperties) then
                   call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,muc,mu)
                   call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,
     & 1),0.,t,lambdac,lambda)
                 end if
                 tau11=(lambda+2.*mu)*uex+lambda*vey
                 tau12=mu*(uey+vex)
                 tau21=tau12
                 tau22=lambda*uex+(lambda+2.*mu)*vey
                 ! save n.S(grad u).tau
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s21c))))) = an1*(-
     & an2*tau11+an1*tau12)+an2*(-an2*tau21+an1*tau22)
                 call ogDeriv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,s11c,tau11)
                 call ogDeriv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,s21c,tau21)
                 call ogDeriv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,s12c,tau12)
                 call ogDeriv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,s22c,tau22)
                 ! save n.(sigma_t).tau -- note: here we use s12c!=s21c 
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c))))) = an1*(-
     & an2*tau11+an1*tau12)+an2*(-an2*tau21+an1*tau22)
                 call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,v1c,v1ex)
                 call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,v1c,v1ey)
                 call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,v2c,v2ex)
                 call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),
     & 0.,t,v2c,v2ey)
                 tau11=(lambda+2.*mu)*v1ex+lambda*v2ey
                 tau12=mu*(v1ey+v2ex)
                 tau21=tau12
                 tau22=lambda*v1ex+(lambda+2.*mu)*v2ey
                 ! save n.S(grad v).tau
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s22c))))) = an1*(-
     & an2*tau11+an1*tau12)+an2*(-an2*tau21+an1*tau22)
                end if
                end do
                end do
              else
               ! fill in the traction BC into the stress components  *wdh* 081109
               ! (this is needed since for TZ flow these values are different)
                i3=n3a
                do i2=nn2a,nn2b
                do i1=nn1a,nn1b
                if (mask(i1,i2,i3).ne.0) then
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c)))))=bcf0(
     & bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-
     & dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(1,1,side,
     & axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(dim(1,2,
     & side,axis)-dim(0,2,side,axis)+1)*(uc)))))
                 bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(
     & 1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(
     & dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)
     & +(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c)))))=bcf0(
     & bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-
     & dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(1,1,side,
     & axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(dim(1,2,
     & side,axis)-dim(0,2,side,axis)+1)*(vc)))))
                end if
                end do
                end do
              end if
             end if ! bc
             ! -- extrapolate ghost values for boundary data ---
             !    These values are now needed  *wdh* 2015/07/15 
             ! adjacent
             if( boundaryCondition(side,axis).gt.0 .and. 
     & addBoundaryForcing(side,axis).ne.0 )then
               i3=n3a
               do sidea=0,1 ! loop over adjacent sides
                if( sidea.eq.0 )then
                  i1=n1a
                  i2=n2a
                else
                  i1=n1b
                  i2=n2b
                end if
                ! (js1,js2,js3) used to compute tangential derivatives
                js1=0
                js2=0
                js3=0
                if( axisp1.eq.0 )then
                  js1=1-2*sidea
                else if( axisp1.eq.1 )then
                  js2=1-2*sidea
                else
                  stop 516
                end if
                !! write(*,'(" extrap bc data array: side,axis,sidea,i1,i2,js1,js2=",7i5)') side,axis,sidea,i1,i2,js1,js2
                do n=0,numberOfComponents-1
                   bcf0(bcOffset(side,axis)+(i1-js1-dim(0,0,side,axis)+
     & (dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-js2-dim(0,1,side,
     & axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-js3-dim(0,
     & 2,side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(n)))))
     & =3.*bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,0,
     & side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(1,
     & 1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(dim(
     & 1,2,side,axis)-dim(0,2,side,axis)+1)*(n)))))-3.*bcf0(bcOffset(
     & side,axis)+(i1+js1-dim(0,0,side,axis)+(dim(1,0,side,axis)-dim(
     & 0,0,side,axis)+1)*(i2+js2-dim(0,1,side,axis)+(dim(1,1,side,
     & axis)-dim(0,1,side,axis)+1)*(i3+js3-dim(0,2,side,axis)+(dim(1,
     & 2,side,axis)-dim(0,2,side,axis)+1)*(n)))))+bcf0(bcOffset(side,
     & axis)+(i1+2*js1-dim(0,0,side,axis)+(dim(1,0,side,axis)-dim(0,0,
     & side,axis)+1)*(i2+2*js2-dim(0,1,side,axis)+(dim(1,1,side,axis)-
     & dim(0,1,side,axis)+1)*(i3+2*js3-dim(0,2,side,axis)+(dim(1,2,
     & side,axis)-dim(0,2,side,axis)+1)*(n)))))
                  !! write(*,'(" n, j1,j2,j3, bc-value",i4,3i4,e16.8)') n,i1-js1,i2-js2,i3-js3,bcfa(side,axis,i1-js1,i2-js2,i3-js3,n)
                end do
               end do ! end do sidea
             end if
             end do ! end side
             end do ! end axis
          !*******
          !******* Primary Dirichlet boundary conditions ***********
          !*******
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
             if( boundaryCondition(side,axis).eq.displacementBC )then
              ! *************** Displacement BC *****************
              ! ..step 0: Dirichlet bcs for displacement and velocity
               i3=n3a
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
               if (mask(i1,i2,i3).ne.0) then
                u(i1,i2,i3,uc) =bcf(side,axis,i1,i2,i3,uc)    ! given displacements
                u(i1,i2,i3,vc) =bcf(side,axis,i1,i2,i3,vc)
                u(i1,i2,i3,v1c)=bcf(side,axis,i1,i2,i3,v1c)   ! given velocities
                u(i1,i2,i3,v2c)=bcf(side,axis,i1,i2,i3,v2c)
                !call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,uc,ue)
                !write(*,'(" i1,i2=",2i3," u,ue=",2e10.2)') i1,i2,u(i1,i2,i3,uc),ue
               end if
               end do
               end do
             else if( boundaryCondition(side,axis).eq.tractionBC )then
              if( applyInterfaceBoundaryConditions.eq.0 .and. 
     & interfaceType(side,axis,grid).eq.tractionInterface )then
               write(*,'("SMBC: skip traction BC on an interface, (
     & side,axis,grid)=(",3i3,")")') side,axis,grid
              else
               ! ********* Traction BC ********
               ! put "dirichlet parts of the traction BC here
              if( debug.gt.3. .and. interfaceType(side,axis,grid)
     & .eq.tractionInterface )then
               write(*,'("SMBC:INFO: assignPrimaryDirichletBC for an 
     & interface, (side,axis,grid)=(",3i3,")")') side,axis,grid
              end if
              if( gridType.eq.rectangular )then
                if (bctype.eq.linearBoundaryCondition) then      ! linear
                  ! new
                  if( axis.eq.0 )then
                    i3=n3a
                    do i2=nn2a,nn2b
                    do i1=nn1a,nn1b
                    if (mask(i1,i2,i3).ne.0) then
                     ! set normal components of the stress, n=(-is,0)
                      f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                      f2=bcf(side,axis,i1,i2,i3,s12c)
                      f1=f1+is*u(i1,i2,i3,s11c)
                      f2=f2+is*u(i1,i2,i3,s12c)
                      u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)-is*f1
                      u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)-is*f2
                      u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)-is*f2
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s11c,tau11)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s21c,tau21)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s12c,tau12)
                      ! if (abs(tau11-u(i1,i2,i3,s11c)).gt.1.e-14) then
                      !   write(6,*)i1,i2,i3,t,s11c,abs(tau11-u(i1,i2,i3,s11c))
                      !   pause
                      ! end if
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s22c,tau22)
                      !  write(6,'(2(1x,i2),4(1x,f8.4),/,6x,4(1x,f8.4))')i1,i2,u(i1,i2,i3,s11c),u(i1,i2,i3,s12c),u(i1,i2,i3,s21c),u(i1,i2,i3,s22c),tau11,tau12,tau21,tau22
                      !  333            format(2(1x,i2),4(1x,f8.4),/,6x,4(1x,f8.4))
                    end if
                    end do
                    end do
                  else
                    i3=n3a
                    do i2=nn2a,nn2b
                    do i1=nn1a,nn1b
                    if (mask(i1,i2,i3).ne.0) then
                     ! set normal components of the stress, n=(0,-is)
                      f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                      f2=bcf(side,axis,i1,i2,i3,s12c)
                      f1=f1+is*u(i1,i2,i3,s21c)
                      f2=f2+is*u(i1,i2,i3,s22c)
                      !   u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)
                      u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)-is*f1
                      u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)-is*f1
                      u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)-is*f2
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s11c,tau11)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s21c,tau21)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s12c,tau12)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s22c,tau22)
                      ! write(6,'(2(1x,i2),4(1x,f8.4),/,6x,4(1x,f8.4))')i1,i2,u(i1,i2,i3,s11c),u(i1,i2,i3,s12c),u(i1,i2,i3,s21c),u(i1,i2,i3,s22c),tau11,tau12,tau21,tau22
                    end if
                    end do
                    end do
                  end if
                else    ! SVK
                  if( axis.eq.0 )then
                    i3=n3a
                    do i2=nn2a,nn2b
                    do i1=nn1a,nn1b
                    if (mask(i1,i2,i3).ne.0) then
                     ! set normal components of the stress, n=(-is,0)
                     u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(1)
     & )
                     u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(1)
     & )
                     alpha=sqrt(u1y**2+(1.0+u2y)**2)
                     u(i1,i2,i3,s11c) =-is*bcf(side,axis,i1,i2,i3,s11c)
     & *alpha
                     u(i1,i2,i3,s12c) =-is*bcf(side,axis,i1,i2,i3,s12c)
     & *alpha
          !!      write(*,'(" primary: set i1,i2,i3 alpha, bc, s11=",3i4,3e16.8)')  i1,i2,i3,alpha,bcf(side,axis,i1,i2,i3,s11c),u(i1,i2,i3,s11c)        
          !!      write(*,'(" primary: u,v=",4e16.8)') u(i1,i2+1,i3,uc),u(i1,i2-1,i3,uc),u(i1,i2+1,i3,vc),u(i1,i2-1,i3,vc)
                    end if
                    end do
                    end do
                  else
                    i3=n3a
                    do i2=nn2a,nn2b
                    do i1=nn1a,nn1b
                    if (mask(i1,i2,i3).ne.0) then
                     ! set normal components of the stress, n=(0,-is)
                     u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(0)
     & )
                     u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(0)
     & )
                     alpha=sqrt((1.0+u1x)**2+u2x**2)
                     u(i1,i2,i3,s21c) =-is*bcf(side,axis,i1,i2,i3,s11c)
     & *alpha
                     u(i1,i2,i3,s22c) =-is*bcf(side,axis,i1,i2,i3,s12c)
     & *alpha
                    end if
                    end do
                    end do
                  end if
                end if
              else  ! curvilinear
                if (bctype.eq.linearBoundaryCondition) then   ! linear
                  ! new
                   i3=n3a
                   do i2=nn2a,nn2b
                   do i1=nn1a,nn1b
                   if (mask(i1,i2,i3).ne.0) then
                    f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                    f2=bcf(side,axis,i1,i2,i3,s12c)
                    ! (an1,an2) = outward normal 
                    aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2))
                    an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                    an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                    f1=f1-(an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,i3,s21c))
                    f2=f2-(an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,i3,s22c))
                    b1=((1.0+an2**2)*f1-an1*an2*f2)/2.0
                    b2=((1.0+an1**2)*f2-an1*an2*f1)/2.0
                    u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+2.0*b1*an1
                    u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+b2*an1+b1*an2
                    u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+b2*an1+b1*an2
                    u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+2.0*b2*an2
                   end if
                   end do
                   end do
                else     ! SVK
                  if (axis.eq.0) then
                     i3=n3a
                     do i2=nn2a,nn2b
                     do i1=nn1a,nn1b
                      if (mask(i1,i2,i3).ne.0) then
                        ! (an1,an2) = outward normal 
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+
     & rx(i1,i2,i3,axis,1)**2))
                        an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                        an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                        u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*
     & dr(1))
                        u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*
     & dr(1))
                        alpha=sqrt((rx(i1,i2,i3,0,1)-u1s/det(i1,i2,i3))
     & **2+(rx(i1,i2,i3,0,0)+u2s/det(i1,i2,i3))**2)*aNormi
                        f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                        f2=bcf(side,axis,i1,i2,i3,s12c)
                        b1=f1*alpha-(an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,
     & i3,s21c))
                        b2=f2*alpha-(an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,
     & i3,s22c))
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+an1*b1
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+an1*b2
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+an2*b1
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+an2*b2
                      end if
                     end do
                     end do
                  else
                     i3=n3a
                     do i2=nn2a,nn2b
                     do i1=nn1a,nn1b
                      if (mask(i1,i2,i3).ne.0) then
                        ! (an1,an2) = outward normal 
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+
     & rx(i1,i2,i3,axis,1)**2))
                        an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                        an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                        u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*
     & dr(0))
                        u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*
     & dr(0))
                        alpha=sqrt((rx(i1,i2,i3,1,1)+u1r/det(i1,i2,i3))
     & **2+(rx(i1,i2,i3,1,0)-u2r/det(i1,i2,i3))**2)*aNormi
                        f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                        f2=bcf(side,axis,i1,i2,i3,s12c)
                        b1=f1*alpha-(an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,
     & i3,s21c))
                        b2=f2*alpha-(an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,
     & i3,s22c))
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+an1*b1
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+an1*b2
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+an2*b1
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+an2*b2
                      end if
                     end do
                     end do
                  end if
                end if
              end if  ! end gridType
              end if ! not interface
             else if( boundaryCondition(side,axis).eq.slipWall )then
               ! ********* SlipWall BC ********
               ! put "dirichlet parts of the slipwall BC here
              if( gridType.eq.rectangular )then
                ! new
                if( axis.eq.0 )then
                  i3=n3a
                  do i2=nn2a,nn2b
                  do i1=nn1a,nn1b
                  if (mask(i1,i2,i3).ne.0) then
                   ! set n.tau.t and the normal component of displacement, n=(-is,0), t=(0,-is)
                   u(i1,i2,i3,s12c) = bcf(side,axis,i1,i2,i3,s11c)
                   u(i1,i2,i3,s21c) = bcf(side,axis,i1,i2,i3,s11c)
                   u(i1,i2,i3,uc) = -is*bcf(side,axis,i1,i2,i3,uc)
                   u(i1,i2,i3,v1c) = -is*bcf(side,axis,i1,i2,i3,v1c)
                  end if
                  end do
                  end do
                else
                  i3=n3a
                  do i2=nn2a,nn2b
                  do i1=nn1a,nn1b
                  if (mask(i1,i2,i3).ne.0) then
                   ! set n.tau.t and the normal component of displacement, n=(0,-is), t=(+is,0)
                   u(i1,i2,i3,s12c) = -bcf(side,axis,i1,i2,i3,s11c)
                   u(i1,i2,i3,s21c) = -bcf(side,axis,i1,i2,i3,s11c)
                   u(i1,i2,i3,vc) = -is*bcf(side,axis,i1,i2,i3,uc)
                   u(i1,i2,i3,v2c) = -is*bcf(side,axis,i1,i2,i3,v1c)
                  end if
                  end do
                  end do
                end if
              else  ! curvilinear
                ! new
                 i3=n3a
                 do i2=nn2a,nn2b
                 do i1=nn1a,nn1b
                 if (mask(i1,i2,i3).ne.0) then
                  f1=bcf(side,axis,i1,i2,i3,s11c)              ! given tangential traction force
                  f2=bcf(side,axis,i1,i2,i3,uc)                ! given normal displacement
                  f3=bcf(side,axis,i1,i2,i3,v1c)               ! given normal velocity
                  ! (an1,an2) = outward normal and (-an2,an1) = unit tangent
                  aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,
     & i2,i3,axis,1)**2))
                  an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                  an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                  b1=f1-an1*(-u(i1,i2,i3,s11c)*an2+u(i1,i2,i3,s12c)*
     & an1)-an2*(-u(i1,i2,i3,s21c)*an2+u(i1,i2,i3,s22c)*an1)
                  b2=f2-an1*u(i1,i2,i3,uc)-an2*u(i1,i2,i3,vc)
                  b3=f3-an1*u(i1,i2,i3,v1c)-an2*u(i1,i2,i3,v2c)
                  u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)-2.0*b1*an1*an2
                  u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+b1*(an1**2-an2**2)
                  u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+b1*(an1**2-an2**2)
                  u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+2.0*b1*an1*an2
                  u(i1,i2,i3,uc)=u(i1,i2,i3,uc)+an1*b2
                  u(i1,i2,i3,vc)=u(i1,i2,i3,vc)+an2*b2
                  u(i1,i2,i3,v1c)=u(i1,i2,i3,v1c)+an1*b3
                  u(i1,i2,i3,v2c)=u(i1,i2,i3,v2c)+an2*b3
                 end if
                 end do
                 end do
              end if  ! end gridType
             end if ! bc
             end do ! end side
             end do ! end axis
           if( debug.gt.32 )then
            n1a=gridIndexRange(0,0)
            n1b=gridIndexRange(1,0)
            n2a=gridIndexRange(0,1)
            n2b=gridIndexRange(1,1)
            n3a=gridIndexRange(0,2)
            n3b=gridIndexRange(1,2)
             write(*,'("v1c",1x,"Afterprimarydirichlet")')
            ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,v1c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,v1c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'("v2c",1x,"Afterprimarydirichlet")')
            ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,v2c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,v2c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'("s11c",1x,"Afterprimarydirichlet")')
            ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s11c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s11c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'("s12c",1x,"Afterprimarydirichlet")')
            ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s12c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s12c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'("s22c",1x,"Afterprimarydirichlet")')
            ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s22c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
             write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s22c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
           end if
          ! --- Optionally Pin (fix the location of ) corners and edges ---
          ! *wdh* 110707
           do n=0,numToPin-1
             ! write(*,'("bcOptSmFOS: pin: grid,side,side2,side3,option=",5i3)') pinbc(0,n),pinbc(1,n),pinbc(2,n),pinbc(3,n),pinbc(4,n)
             if( pinbc(0,n).eq.grid )then
               side1=pinbc(1,n)
               if( side1.eq.0 .or. side1.eq.1 )then
                n1a=gridIndexRange(side1,0)
                n1b=n1a
               else
                n1a=gridIndexRange(0,0)
                n1b=gridIndexRange(1,0)
               end if
               side2=pinbc(2,n)
               if( side2.eq.0 .or. side2.eq.1 )then
                n2a=gridIndexRange(side2,1)
                n2b=n2a
               else
                n2a=gridIndexRange(0,1)
                n2b=gridIndexRange(1,1)
               end if
               side3=pinbc(3,n)
               if( side3.eq.0 .or. side3.eq.1 )then
                n3a=gridIndexRange(side3,2)
                n3b=n3a
               else
                n3a=gridIndexRange(0,2)
                n3b=gridIndexRange(1,2)
               end if
               ! ** FIX ME for parallel **
               ! We set all solution values at the pinned points -- is this correct ??
               if( nd.eq.2 )then
                 ! Pin values:
                 !    u1,u2,u3, v1,v2,v3, s11, s12, s13, s22, s23, s33
                 !     0  1  2   3  4  5    6    7    8    9   10   11
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  u(i1,i2,i3,uc ) =pinValues( 0,n)
                  u(i1,i2,i3,vc ) =pinValues( 1,n)
                  u(i1,i2,i3,v1c) =pinValues( 3,n)
                  u(i1,i2,i3,v2c) =pinValues( 4,n)
                  u(i1,i2,i3,s11c)=pinValues( 6,n)
                  u(i1,i2,i3,s12c)=pinValues( 7,n)
                  u(i1,i2,i3,s22c)=pinValues( 9,n)
                  ! symmetric values:
                  u(i1,i2,i3,s21c)=u(i1,i2,i3,s12c)
                  ! write(*,'(" pin point (i1,i2,i3)=(",3i4,")")') i1,i2,i3
                  ! write(*,'(" u,v=",2e11.3," v1,v2=",2e11.3," s11,s12,s22=",3e11.3)') u(i1,i2,i3,uc ),u(i1,i2,i3,vc ), u(i1,i2,i3,v1c),u(i1,i2,i3,v2c),u(i1,i2,i3,s11c),u(i1,i2,i3,s12c),u(i1,i2,i3,s22c)
                  end do
                  end do
                  end do
               else
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  u(i1,i2,i3,uc ) =pinValues( 0,n)
                  u(i1,i2,i3,vc ) =pinValues( 1,n)
                  u(i1,i2,i3,wc ) =pinValues( 2,n)
                  u(i1,i2,i3,v1c) =pinValues( 3,n)
                  u(i1,i2,i3,v2c) =pinValues( 4,n)
                  u(i1,i2,i3,v3c) =pinValues( 5,n)
                  u(i1,i2,i3,s11c)=pinValues( 6,n)
                  u(i1,i2,i3,s12c)=pinValues( 7,n)
                  u(i1,i2,i3,s13c)=pinValues( 8,n)
                  u(i1,i2,i3,s22c)=pinValues( 9,n)
                  u(i1,i2,i3,s23c)=pinValues(10,n)
                  u(i1,i2,i3,s33c)=pinValues(11,n)
                  ! symmetric values:
                  u(i1,i2,i3,s21c)=u(i1,i2,i3,s12c)
                  u(i1,i2,i3,s31c)=u(i1,i2,i3,s13c)
                  u(i1,i2,i3,s32c)=u(i1,i2,i3,s23c)
                  end do
                  end do
                  end do
               end if
             end if
           end do
          ! return after setting primary bcs for debugging
          if (.false.) return
          !*******
          !******* Extrapolate to the first ghost cells (only for physical sides) ********
          !*******
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
            ! *wdh* 101029 -- do not extrap dirichletBoundaryCondition
            if( boundaryCondition(side,axis).gt.0 .and. 
     & boundaryCondition(side,axis).ne.dirichletBoundaryCondition 
     & .and. boundaryCondition(side,axis).ne.symmetry )then
              i3=n3a
              do i2=nn2a,nn2b
              do i1=nn1a,nn1b
              if (mask(i1,i2,i3).ne.0) then  ! note: extrap outside interp pts too
               do n=0,numberOfComponents-1
                 ! *wdh* use limited extrapolation for the first ghost line 2015/07/15
                 ! u(i1-is1,i2-is2,i3,n)=extrap3(u,i1,i2,i3,n,is1,is2,is3)
                   ! here du2=2nd-order approximation, du3=third order
                   ! Blend the 2nd and 3rd order based on the difference 
                   !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                   du1 = u(i1,i2,i3,n)
                   du2 = 2.*u(i1,i2,i3,n)-u(i1+is1,i2+is2,i3,n)
                   du3 = 3.*u(i1,i2,i3,n)-3.*u(i1+is1,i2+is2,i3,n)+u(
     & i1+2*is1,i2+2*is2,i3,n)
                   !   alpha = cdl*(abs(du3-u(i1+is1,i2+is2,i3,n))+abs(du3-du2))/(uEps+abs(u(i1+is1,i2+is2,i3,n))+abs(u(i1+2*is1,i2+2*is2,i3,n)))
                   ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1+is1,i2+is2,i3,n))+abs(u(i1+2*is1,i2+2*is2,i3,n)))
                   uNorm= uEps+ abs(du3) + abs(u(i1,i2,i3,n))+abs(u(i1+
     & is1,i2+is2,i3,n))
                   ! **  du = abs(du3-u(i1+is1,i2+is2,i3,n))/uNorm  ! changed 050711
                   ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                   alpha = cdl*( abs(du3-du2)/uNorm )
                   alpha =min(1.,alpha)
                   ! if( mm.eq.1 )then
                 !  if (alpha.gt.0.9) then
                 !    write(6,*)'limiting, n,du1,du3=',n,du1,du3
                 !    write(6,*)'i1,i2,i3=',i1,i2,i3
                 !    write(6,*)'is1,is2,is3=',is1,is2,is3
                 !  end if
                   !   u(i1,i2,i3,n)=(1.-alpha)*du3+alpha*du2
                   u(i1-is1,i2-is2,i3,n)=(1.-alpha)*du3+alpha*du1
               end do
              end if
              end do
              end do
          !  else if( boundaryCondition(side,axis).eq.symmetry )then  ! *wdh* 101108
          !   ! even symmetry 
          !   if( twilightZone.eq.0 )then
          !    beginGhostLoops2d()
          !     if (mask(i1,i2,i3).ne.0) then
          !      do n=0,numberOfComponents-1
          !        u(i1-is1,i2-is2,i3,n)=u(i1+is1,i2+is2,i3,n)
          !      end do
          !     end if
          !    endGhostLoops2d()
          !   else
          !    ! TZ :
          !    beginGhostLoops2d()
          !     if (mask(i1,i2,i3).ne.0) then
          !      do n=0,numberOfComponents-1
          !        call ogDeriv(ep,0,0,0,0,xy(i1-is1,i2-is2,i3,0),xy(i1-is1,i2-is2,i3,1),0.,t,n,uem)
          !        call ogDeriv(ep,0,0,0,0,xy(i1+is1,i2+is2,i3,0),xy(i1+is1,i2+is2,i3,1),0.,t,n,uep)
          !        u(i1-is1,i2-is2,i3,n)=u(i1+is1,i2+is2,i3,n) + uem - uep
          !      end do
          !     end if
          !    endGhostLoops2d()
          !   end if
            end if ! bc
            end do ! end side
            end do ! end axis
          !******* Assign symmetry BC on ghost line 1 (note: do this after extrap ghost)
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
            if( boundaryCondition(side,axis).eq.symmetry )then
             ! even symmetry 
             js1=is1*1
             js2=is2*1
             if( twilightZone.eq.0 )then
               i3=n3a
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
               if (mask(i1,i2,i3).ne.0) then
                do n=0,numberOfComponents-1
                  u(i1-js1,i2-js2,i3,n)=u(i1+js1,i2+js2,i3,n)
                end do
               end if
               end do
               end do
             else
              ! TZ :
               i3=n3a
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
               if (mask(i1,i2,i3).ne.0) then
                do n=0,numberOfComponents-1
                  call ogDeriv(ep,0,0,0,0,xy(i1-js1,i2-js2,i3,0),xy(i1-
     & js1,i2-js2,i3,1),0.,t,n,uem)
                  call ogDeriv(ep,0,0,0,0,xy(i1+js1,i2+js2,i3,0),xy(i1+
     & js1,i2+js2,i3,1),0.,t,n,uep)
                  u(i1-js1,i2-js2,i3,n)=u(i1+js1,i2+js2,i3,n) + uem - 
     & uep
                end do
               end if
               end do
               end do
             end if
            end if ! bc
            end do ! end side
            end do ! end axis
          ! return after first extrapolation for debugging
          if (.false.) return
          !*******
          !******* Fix up components of stress in the corners (such as n1a,n2a) ********
          !*******
            !  Note: it does not appear to be possible to set the components of stress in the corner
            !        if one of the sides is a slipwall bc
            !  Note: new implementation of traction bcs for SVK case leads to Dirichlet bcs for *all*
            !        components of stress on the boundary.  (Two components are set by the physical
            !        bcs and the other two are set by compatibility conditions.)  Thus, no corner
            !        stress fix is needed for the SVK case if any bc is a traction bc.  DWS, 2/28/12
            ! 
            !  Update: the above is not true.  DWS, 3/28/12.  :)
            !
            !  Additional changes:  DWS, 7/8/15
            !    The mixed displacement-traction corner cases for the nonlinear (SVK) cases now
            !    set the tangent components of the stress in the corner and set ghost points
            !    for displacement and velocity.  The basic configuration is this.  If the North
            !    face is traction while the East face is displacement, then ghost points for
            !    displacement and velocity would be set in the first ghost line to the east of
            !    of the corner.  The displacement and velocity in the first ghost line to the
            !    north of the corner are known already because of the displacement bcs.  So,
            !    by setting the east ghost points, centered differences of displacement lead
            !    to compatible stress components in the corner. 
            i3=gridIndexRange(0,2)
            if (gridType.eq.rectangular) then
              ! -----------------------------------------------------------------------
              ! --------------------- CARTESIAN FIXUP CORNER STRESS -------------------
              ! -----------------------------------------------------------------------
              do side1=0,1
                i1=gridIndexRange(side1,axis1)
                do side2=0,1
                  i2=gridIndexRange(side2,axis2)
                  if (mask(i1,i2,i3).ne.0) then
                    if (bctype.eq.linearBoundaryCondition) then    ! linear case only
                      if (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                        mu=mupc(i1,i2)
                        lambda=lambdapc(i1,i2)
                      elseif (
     & materialFormat.eq.variableMaterialProperties) then
                        mu=muv(i1,i2)
                        lambda=lambdav(i1,i2)
                      end if
                    end if
                    if (boundaryCondition(side1,axis1)
     & .eq.displacementBC.and.boundaryCondition(side2,axis2)
     & .eq.displacementBC) then
                      !  Cartesian grid, pure displacement/velocity bcs
                      u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(
     & 0))
                      u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(
     & 0))
                      u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(
     & 1))
                      u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(
     & 1))
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        u(i1,i2,i3,s11c)=lambda*(u1x+u2y)+2.0*mu*u1x
                        u(i1,i2,i3,s12c)=mu*(u1y+u2x)
                        u(i1,i2,i3,s21c)=mu*(u1y+u2x)
                        u(i1,i2,i3,s22c)=lambda*(u1x+u2y)+2.0*mu*u2y
                      else                                             
     &   ! SVK case
                        !   call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,0)
                        du(1,1)=u1x
                        du(1,2)=u1y
                        du(2,1)=u2x
                        du(2,2)=u2y
                        ideriv=0
                        call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                        u(i1,i2,i3,s11c)=p(1,1)
                        u(i1,i2,i3,s12c)=p(1,2)
                        u(i1,i2,i3,s21c)=p(2,1)
                        u(i1,i2,i3,s22c)=p(2,2)
                      end if
                   elseif (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC) then
                   !  Cartesian grid, pure traction bcs
                   !  No fix is needed since the normals for (side1,axis1) and (side2,axis2) are orthogonal (linear case only)
                      if (bctype.ne.linearBoundaryCondition) then    ! SVK case
                        !  initialize
                        is1=1-2*side1
                        is2=1-2*side2
                        u1x0=is1*(u(i1+is1,i2,i3,uc)-u(i1,i2,i3,uc))
     & /dx(0)
                        u2x0=is1*(u(i1+is1,i2,i3,vc)-u(i1,i2,i3,vc))
     & /dx(0)
                        u1y0=is2*(u(i1,i2+is2,i3,uc)-u(i1,i2,i3,uc))
     & /dx(1)
                        u2y0=is2*(u(i1,i2+is2,i3,vc)-u(i1,i2,i3,vc))
     & /dx(1)
                        u1x=u1x0
                        u2x=u2x0
                        u1y=u1y0
                        u2y=u2y0
                        ! u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(0))
                        ! u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(0))
                        ! u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(1))
                        ! u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(1))
                        ! Newton iteration for u1x,u2x,u1y,u2y
                        ier=0
                        iter=1
                        istop=0
                        bmax=10.*toler
                        do while (bmax.gt.toler)
                         !  compute stress and the deriv based on current deformation gradient
                         !   ideriv=1
                         !   call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1x
                          du(1,2)=u1y
                          du(2,1)=u2x
                          du(2,2)=u2y
                          ideriv=1
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          alpha1=sqrt(u1y**2+(1.0+u2y)**2)
                          ! given traction forces (adjust here for sign of normal)
                          f11=-is1*bcf(side1,axis1,i1,i2,i3,uc)*alpha1
                          f21=-is1*bcf(side1,axis1,i1,i2,i3,vc)*alpha1
                          dalpha11=u1y/alpha1
                          dalpha12=(1.0+u2y)/alpha1
                          alpha2=sqrt((1.0+u1x)**2+u2x**2)
                          ! given traction forces (adjust here for sign of normal)
                          f12=-is2*bcf(side2,axis2,i1,i2,i3,uc)*alpha2
                          f22=-is2*bcf(side2,axis2,i1,i2,i3,vc)*alpha2
                          dalpha21=(1.0+u1x)/alpha2
                          dalpha22=u2x/alpha2
                          !  set up the 4x4 system
                          bb(1)=p(1,1)-f11
                          bb(2)=p(1,2)-f21
                          bb(3)=p(2,1)-f12
                          bb(4)=p(2,2)-f22
                          aa(1,1)=dpdf(1,1)
                          aa(1,2)=dpdf(1,2)+is1*bcf(side1,axis1,i1,i2,
     & i3,uc)*dalpha11
                          aa(1,3)=dpdf(1,3)
                          aa(1,4)=dpdf(1,4)+is1*bcf(side1,axis1,i1,i2,
     & i3,uc)*dalpha12
                          aa(2,1)=dpdf(2,1)
                          aa(2,2)=dpdf(2,2)+is1*bcf(side1,axis1,i1,i2,
     & i3,vc)*dalpha11
                          aa(2,3)=dpdf(2,3)
                          aa(2,4)=dpdf(2,4)+is1*bcf(side1,axis1,i1,i2,
     & i3,vc)*dalpha12
                          aa(3,1)=dpdf(3,1)+is2*bcf(side2,axis2,i1,i2,
     & i3,uc)*dalpha21
                          aa(3,2)=dpdf(3,2)
                          aa(3,3)=dpdf(3,3)+is2*bcf(side2,axis2,i1,i2,
     & i3,uc)*dalpha22
                          aa(3,4)=dpdf(3,4)
                          aa(4,1)=dpdf(4,1)+is2*bcf(side2,axis2,i1,i2,
     & i3,vc)*dalpha21
                          aa(4,2)=dpdf(4,2)
                          aa(4,3)=dpdf(4,3)+is2*bcf(side2,axis2,i1,i2,
     & i3,vc)*dalpha22
                          aa(4,4)=dpdf(4,4)
                          !  solve the 4x4 system
                          bmax=max(abs(bb(1)),abs(bb(2)),abs(bb(3)),
     & abs(bb(4)))/lambda
                          call smsolve (aa,bb,ier)
                          if (istop.ne.0) then
                            write(6,'(1x,i2,5(1x,1pe15.8))')iter,bb(1),
     & bb(2),bb(3),bb(4),bmax
                          end if
                          !  update
                          u1x=u1x-bb(1)
                          u1y=u1y-bb(2)
                          u2x=u2x-bb(3)
                          u2y=u2y-bb(4)
                          iter=iter+1
                          !  check for convergence
                          if (iter.gt.itmax.or.ier.ne.0) then
                            write(6,*)'Error (bcOptSmFOS) : Newton 
     & failed to converge'
                            if (istop.eq.0) then
                              ier=0
                              iter=1
                              istop=1
                              u1x=u1x0
                              u2x=u2x0
                              u1y=u1y0
                              u2y=u2y0
                            else
                              stop 8881
                            end if
                          end if
                        end do
                        !  set displacement in the ghost point
                        u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*
     & dx(0)*u1x
                        u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*
     & dx(0)*u2x
                        u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is2*
     & dx(1)*u1y
                        u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is2*
     & dx(1)*u2y
                      end if   ! end bctype
                    elseif (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.displacementBC .and. fixupTractionDisplacementCorners ) 
     & then
                      !  Cartesian grid, mix bcs, case 1
                      u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(
     & 0))
                      u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(
     & 0))
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        u1y=(u(i1,i2,i3,s12c)-mu*u2x)/mu
                        u2y=(u(i1,i2,i3,s11c)-(lambda+2.0*mu)*u1x)
     & /lambda
                        u(i1,i2,i3,s22c)=lambda*(u1x+u2y)+2.0*mu*u2y
                        !  write(6,*)'here (1), side1,side2=',side1,side2
                        !  write(6,*)boundaryCondition(0,0),boundaryCondition(1,0),boundaryCondition(0,1),boundaryCondition(1,1)
                        !  pause
                      else                                             
     &                          ! nonlinear case
                        if (.true.) then   ! true/false switch here is for testing Cartesian grids
                        !  initialize
                        is1=1-2*side1
                        is2=1-2*side2
                        u1y0=is2*(u(i1,i2+is2,i3,uc)-u(i1,i2,i3,uc))
     & /dx(1)
                        u2y0=is2*(u(i1,i2+is2,i3,vc)-u(i1,i2,i3,vc))
     & /dx(1)
                        u1y=u1y0
                        u2y=u2y0
                        ! Newton iteration for u1y,u2y
                        ier=0
                        iter=1
                        istop=0
                        bmax=10.*toler
                        do while (bmax.gt.toler)
                         !  compute stress and the deriv based on current deformation gradient
                         !   ideriv=1
                         !   call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1x
                          du(1,2)=u1y
                          du(2,1)=u2x
                          du(2,2)=u2y
                          ideriv=1
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          alpha1=sqrt(u1y**2+(1.0+u2y)**2)
                          ! given traction forces (adjust here for sign of normal)
                          f11=-is1*bcf(side1,axis1,i1,i2,i3,uc)*alpha1
                          f21=-is1*bcf(side1,axis1,i1,i2,i3,vc)*alpha1
                          dalpha11=u1y/alpha1
                          dalpha12=(1.0+u2y)/alpha1
                          !  set up the 2x2 system
                          bb(1)=p(1,1)-f11
                          bb(2)=p(1,2)-f21
                          aa(1,1)=dpdf(1,2)+is1*bcf(side1,axis1,i1,i2,
     & i3,uc)*dalpha11
                          aa(1,2)=dpdf(1,4)+is1*bcf(side1,axis1,i1,i2,
     & i3,uc)*dalpha12
                          aa(2,1)=dpdf(2,2)+is1*bcf(side1,axis1,i1,i2,
     & i3,vc)*dalpha11
                          aa(2,2)=dpdf(2,4)+is1*bcf(side1,axis1,i1,i2,
     & i3,vc)*dalpha12
                          !  solve the 2x2 system
                          determ=aa(1,1)*aa(2,2)-aa(1,2)*aa(2,1)
                          du1y=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                          du2y=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                          bmax=max(abs(bb(1)),abs(bb(2)))/lambda
                          if (istop.ne.0) then
                            write(6,'(1x,i2,3(1x,1pe15.8))')iter,bb(1),
     & bb(2),bmax
                          end if
                          !  update
                          u1y=u1y-du1y
                          u2y=u2y-du2y
                          iter=iter+1
                          !  check for convergence
                          if (iter.gt.itmax) then
                            write(6,*)'Error (bcOptSmFOS) : Newton 
     & failed to converge'
                            if (istop.eq.0) then
                              ier=0
                              iter=1
                              istop=1
                              u1y=u1y0
                              u2y=u2y0
                            else
                              stop 7881
                            end if
                          end if
                        end do
                        !  set displacement in the ghost point and the tangent components of stress
                        u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is2*
     & dx(1)*u1y
                        u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is2*
     & dx(1)*u2y
                        u(i1,i2,i3,s21c)=p(2,1)
                        u(i1,i2,i3,s22c)=p(2,2)
                        !  compute v1y and v2y
                        v1x=(u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,v1c))/(2.*
     & dx(0))
                        v2x=(u(i1+1,i2,i3,v2c)-u(i1-1,i2,i3,v2c))/(2.*
     & dx(0))
                        bb(1)=-dpdf(1,1)*v1x-dpdf(1,3)*v2x-is1*bcf(
     & side1,axis1,i1,i2,i3,v1c)*alpha1
                        bb(2)=-dpdf(2,1)*v1x-dpdf(2,3)*v2x-is1*bcf(
     & side1,axis1,i1,i2,i3,v2c)*alpha1
                        v1y=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                        v2y=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                        !  set velocity in the ghost point
                        u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)-2.*is2*
     & dx(1)*v1y
                        u(i1,i2-is2,i3,v2c)=u(i1,i2+is2,i3,v2c)-2.*is2*
     & dx(1)*v2y
                        else    ! else true/false testing
                        is2=1-2*side2
                        u1y=0.
                        u2y=0.
                        u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is2*
     & dx(1)*u1y
                        u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is2*
     & dx(1)*u2y
                        u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)
                        u(i1,i2-is2,i3,v2c)=u(i1,i2+is2,i3,v2c)
                        u(i1,i2,i3,s11c)=0.
                        u(i1,i2,i3,s12c)=0.
                        u(i1,i2,i3,s21c)=0.
                        u(i1,i2,i3,s22c)=0.
                        end if   ! end true/false testing
                      end if
                    elseif (boundaryCondition(side1,axis1)
     & .eq.displacementBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC .and. fixupTractionDisplacementCorners ) then
                      ! Cartesian grid, mix bcs, case 2
                      u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(
     & 1))
                      u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(
     & 1))
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        u1x=(u(i1,i2,i3,s22c)-(lambda+2.0*mu)*u2y)
     & /lambda
                        u2x=(u(i1,i2,i3,s21c)-mu*u1y)/mu
                        u(i1,i2,i3,s11c)=lambda*(u1x+u2y)+2.0*mu*u1x
                        !  write(6,*)'here (2), side1,side2=',side1,side2
                        !  pause
                      else                                             
     &                          ! nonlinear case
                        if (.true.) then   ! true/false switch here is for testing Cartesian grids
                        !  initialize
                        is1=1-2*side1
                        is2=1-2*side2
                        u1x0=is1*(u(i1+is1,i2,i3,uc)-u(i1,i2,i3,uc))
     & /dx(0)
                        u2x0=is1*(u(i1+is1,i2,i3,vc)-u(i1,i2,i3,vc))
     & /dx(0)
                        u1x=u1x0
                        u2x=u2x0
                        ! Newton iteration for u1x,u2x
                        ier=0
                        iter=1
                        istop=0
                        bmax=10.*toler
                        do while (bmax.gt.toler)
                         !  compute stress and the deriv based on current deformation gradient
                         !   ideriv=1
                         !   call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1x
                          du(1,2)=u1y
                          du(2,1)=u2x
                          du(2,2)=u2y
                          ideriv=1
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          alpha2=sqrt((1.0+u1x)**2+u2x**2)
                          ! given traction forces (adjust here for sign of normal)
                          f12=-is2*bcf(side2,axis2,i1,i2,i3,uc)*alpha2
                          f22=-is2*bcf(side2,axis2,i1,i2,i3,vc)*alpha2
                          dalpha21=(1.0+u1x)/alpha2
                          dalpha22=u2x/alpha2
                          !  set up the 2x2 system
                          bb(1)=p(2,1)-f12
                          bb(2)=p(2,2)-f22
                          aa(1,1)=dpdf(3,1)+is2*bcf(side2,axis2,i1,i2,
     & i3,uc)*dalpha21
                          aa(1,2)=dpdf(3,3)+is2*bcf(side2,axis2,i1,i2,
     & i3,uc)*dalpha22
                          aa(2,1)=dpdf(4,1)+is2*bcf(side2,axis2,i1,i2,
     & i3,vc)*dalpha21
                          aa(2,2)=dpdf(4,3)+is2*bcf(side2,axis2,i1,i2,
     & i3,vc)*dalpha22
                          !  solve the 2x2 system
                          determ=aa(1,1)*aa(2,2)-aa(1,2)*aa(2,1)
                          du1x=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                          du2x=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                          bmax=max(abs(bb(1)),abs(bb(2)))/lambda
                          if (istop.ne.0) then
                            write(6,'(1x,i2,3(1x,1pe15.8))')iter,bb(1),
     & bb(2),bmax
                          end if
                          !  update
                          u1x=u1x-du1x
                          u2x=u2x-du2x
                          iter=iter+1
                          !  check for convergence
                          if (iter.gt.itmax) then
                            write(6,*)'Error (bcOptSmFOS) : Newton 
     & failed to converge'
                            if (istop.eq.0) then
                              ier=0
                              iter=1
                              istop=1
                              u1x=u1x0
                              u2x=u2x0
                            else
                              stop 7882
                            end if
                          end if
                        end do
                        !  set displacement in the ghost point and the tangent components of stress
                        u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*
     & dx(0)*u1x
                        u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*
     & dx(0)*u2x
                        u(i1,i2,i3,s11c)=p(1,1)
                        u(i1,i2,i3,s12c)=p(1,2)
                        !  compute v1x and v2x
                        v1y=(u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,v1c))/(2.*
     & dx(1))
                        v2y=(u(i1,i2+1,i3,v2c)-u(i1,i2-1,i3,v2c))/(2.*
     & dx(1))
                        bb(1)=-dpdf(3,2)*v1y-dpdf(3,4)*v2y-is2*bcf(
     & side2,axis2,i1,i2,i3,v1c)*alpha2
                        bb(2)=-dpdf(4,2)*v1y-dpdf(4,4)*v2y-is2*bcf(
     & side2,axis2,i1,i2,i3,v2c)*alpha2
                        v1x=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                        v2x=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                        !  set velocity in the ghost point
                        u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)-2.*is1*
     & dx(0)*v1x
                        u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)-2.*is1*
     & dx(0)*v2x
c              u1x=0.
c              u2x=0.
c              u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*dx(0)*u1x
c              u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*dx(0)*u2x
c              u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)
c              u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)
c              u(i1,i2,i3,s11c)=0.
c              u(i1,i2,i3,s12c)=0.
c              u(i1,i2,i3,s21c)=0.
c              u(i1,i2,i3,s22c)=0.
                        else   ! else true/false testing
                        is1=1-2*side1
                        u1x=0.
                        u2x=0.
                        u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*
     & dx(0)*u1x
                        u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*
     & dx(0)*u2x
                        u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)
                        u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)
                        u(i1,i2,i3,s11c)=0.
                        u(i1,i2,i3,s12c)=0.
                        u(i1,i2,i3,s21c)=0.
                        u(i1,i2,i3,s22c)=0.
                        end if   ! end true/false testing
                      end if
                    end if
                  end if
                end do
              end do
            else    ! non-Cartesian cases
              ! -----------------------------------------------------------------------
              ! ------------------- CURVILINEAR FIXUP CORNER STRESS -------------------
              ! -----------------------------------------------------------------------
              do side1=0,1
                i1=gridIndexRange(side1,axis1)
                do side2=0,1
                  i2=gridIndexRange(side2,axis2)
                  if (mask(i1,i2,i3).ne.0) then
                    if (bctype.eq.linearBoundaryCondition) then    ! linear case only
                      if (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                        mu=mupc(i1,i2)
                        lambda=lambdapc(i1,i2)
                      elseif (
     & materialFormat.eq.variableMaterialProperties) then
                        mu=muv(i1,i2)
                        lambda=lambdav(i1,i2)
                      end if
                    end if
                    if (boundaryCondition(side1,axis1)
     & .eq.displacementBC.and.boundaryCondition(side2,axis2)
     & .eq.displacementBC) then
                      ! non-Cartesian grid, pure displacement/velocity bcs
                      u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(
     & 0))
                      u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(
     & 0))
                      u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(
     & 1))
                      u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(
     & 1))
                      u1x=u1r*rx(i1,i2,i3,0,0)+u1s*rx(i1,i2,i3,1,0)
                      u2x=u2r*rx(i1,i2,i3,0,0)+u2s*rx(i1,i2,i3,1,0)
                      u1y=u1r*rx(i1,i2,i3,0,1)+u1s*rx(i1,i2,i3,1,1)
                      u2y=u2r*rx(i1,i2,i3,0,1)+u2s*rx(i1,i2,i3,1,1)
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        u(i1,i2,i3,s11c)=lambda*(u1x+u2y)+2.0*mu*u1x
                        u(i1,i2,i3,s12c)=mu*(u1y+u2x)
                        u(i1,i2,i3,s21c)=mu*(u1y+u2x)
                        u(i1,i2,i3,s22c)=lambda*(u1x+u2y)+2.0*mu*u2y
                      else                                             
     &   ! SVK case
                        !  call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,0)
                        du(1,1)=u1x
                        du(1,2)=u1y
                        du(2,1)=u2x
                        du(2,2)=u2y
                        ideriv=0
                        call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                        u(i1,i2,i3,s11c)=p(1,1)
                        u(i1,i2,i3,s12c)=p(1,2)
                        u(i1,i2,i3,s21c)=p(2,1)
                        u(i1,i2,i3,s22c)=p(2,2)
                      end if
                    elseif (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC) then
                      ! non-Cartesian grid, pure traction bcs.  For the newBCs case, there is nothing to be done for traction-traction
                      ! corners.  Here is the situation.  Assuming that the stress tensor is symmetric, there are 3 components to set,
                      ! i.e. s11, s12=s21 and s22.  Two bcs would be used for one traction side and two bcs would be used for the other
                      ! traction side.  This makes 4 bcs at the corner.  Suppose sigma.n=f for one side and sigma.m=g for the other.  The
                      ! compatibility condition is m.f=n.g.  If this condition is satisfied, then it does matter which traction bc on a
                      ! side is applied first.  When the other traction bc is applied, it does not destroy the bcs already applied.  (I have
                      ! checked the algebra on this, DWS 12/4/10)
                      ! non-Cartesian grid, pure traction bcs (needed since grid lines may not be orthogonal)
                      if (bctype.eq.linearBoundaryCondition) then   ! linear
                      else    ! SVK
                       if (.false.) then  ! old stuff
                        is=1-2*side1
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis1,0)**
     & 2+rx(i1,i2,i3,axis1,1)**2))
                        an11=-is*rx(i1,i2,i3,axis1,0)*aNormi          ! normals for axis1,side1
                        an21=-is*rx(i1,i2,i3,axis1,1)*aNormi
                        u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*
     & dr(1))
                        u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*
     & dr(1))
                        alpha=sqrt((rx(i1,i2,i3,0,1)-u1s/det(i1,i2,i3))
     & **2+(rx(i1,i2,i3,0,0)+u2s/det(i1,i2,i3))**2)*aNormi
                        f11=bcf(side1,axis1,i1,i2,i3,s11c)*alpha      ! given traction forces for axis1,side1
                        f21=bcf(side1,axis1,i1,i2,i3,s12c)*alpha
                        is=1-2*side2
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis2,0)**
     & 2+rx(i1,i2,i3,axis2,1)**2))
                        an12=-is*rx(i1,i2,i3,axis2,0)*aNormi          ! normals for axis2,side2
                        an22=-is*rx(i1,i2,i3,axis2,1)*aNormi
                        u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*
     & dr(0))
                        u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*
     & dr(0))
                        alpha=sqrt((rx(i1,i2,i3,1,1)+u1r/det(i1,i2,i3))
     & **2+(rx(i1,i2,i3,1,0)-u2r/det(i1,i2,i3))**2)*aNormi
                        f12=bcf(side2,axis2,i1,i2,i3,s11c)*alpha      ! given traction forces for axis2,side2
                        f22=bcf(side2,axis2,i1,i2,i3,s12c)*alpha
                        b11=f11-(an11*u(i1,i2,i3,s11c)+an21*u(i1,i2,i3,
     & s21c))
                        b21=f21-(an11*u(i1,i2,i3,s12c)+an21*u(i1,i2,i3,
     & s22c))
                        dot1=an11*an12+an21*an22                      ! cosine of the angle between the normals
                        dot2=an21*an12-an11*an22                      ! cosine of the angle between tangent(1) and normal(2)
                        b12=(f12-(an12*u(i1,i2,i3,s11c)+an22*u(i1,i2,
     & i3,s21c))-dot1*b11)/dot2
                        b22=(f22-(an12*u(i1,i2,i3,s12c)+an22*u(i1,i2,
     & i3,s22c))-dot1*b21)/dot2
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+an11*b11+
     & an21*b12
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+an11*b21+
     & an21*b22
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+an21*b11-
     & an11*b12
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+an21*b21-
     & an11*b22
                       else   ! new stuff: iterate on u1r,u1s,u2r,u2s until traction bcs on both sides are satisfied
                        ! initialize
                        is1=1-2*side1
                        aNormi1=1./max(epsx,sqrt(rx(i1,i2,i3,axis1,0)**
     & 2+rx(i1,i2,i3,axis1,1)**2))
                        an11=-is1*rx(i1,i2,i3,axis1,0)*aNormi1          ! normals for axis1,side1
                        an21=-is1*rx(i1,i2,i3,axis1,1)*aNormi1
                        is2=1-2*side2
                        aNormi2=1./max(epsx,sqrt(rx(i1,i2,i3,axis2,0)**
     & 2+rx(i1,i2,i3,axis2,1)**2))
                        an12=-is2*rx(i1,i2,i3,axis2,0)*aNormi2          ! normals for axis2,side2
                        an22=-is2*rx(i1,i2,i3,axis2,1)*aNormi2
                        u1r0=is1*(u(i1+is1,i2,i3,uc)-u(i1,i2,i3,uc))
     & /dr(0)
                        u2r0=is1*(u(i1+is1,i2,i3,vc)-u(i1,i2,i3,vc))
     & /dr(0)
                        u1s0=is2*(u(i1,i2+is2,i3,uc)-u(i1,i2,i3,uc))
     & /dr(1)
                        u2s0=is2*(u(i1,i2+is2,i3,vc)-u(i1,i2,i3,vc))
     & /dr(1)
                        u1r=u1r0
                        u2r=u2r0
                        u1s=u1s0
                        u2s=u2s0
                        ! u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(0))
                        ! u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(0))
                        ! u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(1))
                        ! u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(1))
                        ! Newton iteration for u1r,u2r,u1s,u2s
                        ier=0
                        iter=1
                        istop=0
                        bmax=10.*toler
                        do while (bmax.gt.toler)
                          u1x=rx(i1,i2,i3,0,0)*u1r+rx(i1,i2,i3,1,0)*u1s
                          u1y=rx(i1,i2,i3,0,1)*u1r+rx(i1,i2,i3,1,1)*u1s
                          u2x=rx(i1,i2,i3,0,0)*u2r+rx(i1,i2,i3,1,0)*u2s
                          u2y=rx(i1,i2,i3,0,1)*u2r+rx(i1,i2,i3,1,1)*u2s
                          ! compute stress and the deriv based on current deformation gradient
                          !                      ideriv=1
                          !                      call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1x
                          du(1,2)=u1y
                          du(2,1)=u2x
                          du(2,2)=u2y
                          ideriv=1
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          coef11=rx(i1,i2,i3,0,1)-u1s/det(i1,i2,i3)
                          coef21=rx(i1,i2,i3,0,0)+u2s/det(i1,i2,i3)
                          alpha1=sqrt(coef11**2+coef21**2)*aNormi1
                          f11=bcf(side1,axis1,i1,i2,i3,uc)*alpha1      
     &   ! given traction forces
                          f21=bcf(side1,axis1,i1,i2,i3,vc)*alpha1
                          fact=aNormi1/(det(i1,i2,i3)*sqrt(coef11**2+
     & coef21**2))
                          dalpha11=-coef11*fact
                          dalpha12= coef21*fact
                          coef12=rx(i1,i2,i3,1,1)+u1r/det(i1,i2,i3)
                          coef22=rx(i1,i2,i3,1,0)-u2r/det(i1,i2,i3)
                          alpha2=sqrt(coef12**2+coef22**2)*aNormi2
                          f12=bcf(side2,axis2,i1,i2,i3,uc)*alpha2      
     &    ! given traction forces
                          f22=bcf(side2,axis2,i1,i2,i3,vc)*alpha2
                          fact=aNormi2/(det(i1,i2,i3)*sqrt(coef12**2+
     & coef22**2))
                          dalpha21= coef12*fact
                          dalpha22=-coef22*fact
                          ! construct linear system
                          bb(1)=an11*p(1,1)+an21*p(2,1)-f11
                          bb(2)=an11*p(1,2)+an21*p(2,2)-f21
                          bb(3)=an12*p(1,1)+an22*p(2,1)-f12
                          bb(4)=an12*p(1,2)+an22*p(2,2)-f22
                          aa(1,1)= an11*(dpdf(1,1)*rx(i1,i2,i3,0,0)+
     & dpdf(1,2)*rx(i1,i2,i3,0,1)) +an21*(dpdf(3,1)*rx(i1,i2,i3,0,0)+
     & dpdf(3,2)*rx(i1,i2,i3,0,1))
                          aa(1,2)= an11*(dpdf(1,3)*rx(i1,i2,i3,0,0)+
     & dpdf(1,4)*rx(i1,i2,i3,0,1)) +an21*(dpdf(3,3)*rx(i1,i2,i3,0,0)+
     & dpdf(3,4)*rx(i1,i2,i3,0,1))
                          aa(1,3)= an11*(dpdf(1,1)*rx(i1,i2,i3,1,0)+
     & dpdf(1,2)*rx(i1,i2,i3,1,1)) +an21*(dpdf(3,1)*rx(i1,i2,i3,1,0)+
     & dpdf(3,2)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,uc)*
     & dalpha11
                          aa(1,4)= an11*(dpdf(1,3)*rx(i1,i2,i3,1,0)+
     & dpdf(1,4)*rx(i1,i2,i3,1,1)) +an21*(dpdf(3,3)*rx(i1,i2,i3,1,0)+
     & dpdf(3,4)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,uc)*
     & dalpha12
                          aa(2,1)= an11*(dpdf(2,1)*rx(i1,i2,i3,0,0)+
     & dpdf(2,2)*rx(i1,i2,i3,0,1)) +an21*(dpdf(4,1)*rx(i1,i2,i3,0,0)+
     & dpdf(4,2)*rx(i1,i2,i3,0,1))
                          aa(2,2)= an11*(dpdf(2,3)*rx(i1,i2,i3,0,0)+
     & dpdf(2,4)*rx(i1,i2,i3,0,1)) +an21*(dpdf(4,3)*rx(i1,i2,i3,0,0)+
     & dpdf(4,4)*rx(i1,i2,i3,0,1))
                          aa(2,3)= an11*(dpdf(2,1)*rx(i1,i2,i3,1,0)+
     & dpdf(2,2)*rx(i1,i2,i3,1,1)) +an21*(dpdf(4,1)*rx(i1,i2,i3,1,0)+
     & dpdf(4,2)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,vc)*
     & dalpha11
                          aa(2,4)= an11*(dpdf(2,3)*rx(i1,i2,i3,1,0)+
     & dpdf(2,4)*rx(i1,i2,i3,1,1)) +an21*(dpdf(4,3)*rx(i1,i2,i3,1,0)+
     & dpdf(4,4)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,vc)*
     & dalpha12
                          aa(3,1)= an12*(dpdf(1,1)*rx(i1,i2,i3,0,0)+
     & dpdf(1,2)*rx(i1,i2,i3,0,1)) +an22*(dpdf(3,1)*rx(i1,i2,i3,0,0)+
     & dpdf(3,2)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,uc)*
     & dalpha21
                          aa(3,2)= an12*(dpdf(1,3)*rx(i1,i2,i3,0,0)+
     & dpdf(1,4)*rx(i1,i2,i3,0,1)) +an22*(dpdf(3,3)*rx(i1,i2,i3,0,0)+
     & dpdf(3,4)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,uc)*
     & dalpha22
                          aa(3,3)= an12*(dpdf(1,1)*rx(i1,i2,i3,1,0)+
     & dpdf(1,2)*rx(i1,i2,i3,1,1)) +an22*(dpdf(3,1)*rx(i1,i2,i3,1,0)+
     & dpdf(3,2)*rx(i1,i2,i3,1,1))
                          aa(3,4)= an12*(dpdf(1,3)*rx(i1,i2,i3,1,0)+
     & dpdf(1,4)*rx(i1,i2,i3,1,1)) +an22*(dpdf(3,3)*rx(i1,i2,i3,1,0)+
     & dpdf(3,4)*rx(i1,i2,i3,1,1))
                          aa(4,1)= an12*(dpdf(2,1)*rx(i1,i2,i3,0,0)+
     & dpdf(2,2)*rx(i1,i2,i3,0,1)) +an22*(dpdf(4,1)*rx(i1,i2,i3,0,0)+
     & dpdf(4,2)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,vc)*
     & dalpha21
                          aa(4,2)= an12*(dpdf(2,3)*rx(i1,i2,i3,0,0)+
     & dpdf(2,4)*rx(i1,i2,i3,0,1)) +an22*(dpdf(4,3)*rx(i1,i2,i3,0,0)+
     & dpdf(4,4)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,vc)*
     & dalpha22
                          aa(4,3)= an12*(dpdf(2,1)*rx(i1,i2,i3,1,0)+
     & dpdf(2,2)*rx(i1,i2,i3,1,1)) +an22*(dpdf(4,1)*rx(i1,i2,i3,1,0)+
     & dpdf(4,2)*rx(i1,i2,i3,1,1))
                          aa(4,4)= an12*(dpdf(2,3)*rx(i1,i2,i3,1,0)+
     & dpdf(2,4)*rx(i1,i2,i3,1,1)) +an22*(dpdf(4,3)*rx(i1,i2,i3,1,0)+
     & dpdf(4,4)*rx(i1,i2,i3,1,1))
                          ! solve the 4x4 system
                          bmax=max(abs(bb(1)),abs(bb(2)),abs(bb(3)),
     & abs(bb(4)))/lambda
                          call smsolve (aa,bb,ier)
                          if (istop.ne.0) then
                            write(6,'(1x,i2,5(1x,1pe15.8))')iter,bb(1),
     & bb(2),bb(3),bb(4),bmax
                          end if
                          ! update
                          u1r=u1r-bb(1)
                          u2r=u2r-bb(2)
                          u1s=u1s-bb(3)
                          u2s=u2s-bb(4)
                          iter=iter+1
                          ! check for convergence
                          if (iter.gt.itmax.or.ier.ne.0) then
                            write(6,*)'Error (bcOptSmFOS) : Newton 
     & failed to converge'
                            if (istop.eq.0) then
                              ier=0
                              iter=1
                              istop=1
                              u1r=u1r0
                              u2r=u2r0
                              u1s=u1s0
                              u2s=u2s0
                            else
                              stop 8882
                            end if
                          end if
                        end do
                        ! set displacement in the ghost point
                        u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*
     & dr(0)*u1r
                        u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*
     & dr(0)*u2r
                        u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is2*
     & dr(1)*u1s
                        u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is2*
     & dr(1)*u2s
                       end if   ! end old/new
                      end if
                    elseif (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.displacementBC .and. fixupTractionDisplacementCorners ) 
     & then
                      ! non-Cartesian grid, mix bcs, case 1  (Should be okay for both new and old bcs)
                      is=1-2*side1
                      aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis1,0)**2+
     & rx(i1,i2,i3,axis1,1)**2))
                      an1=-is*rx(i1,i2,i3,axis1,0)*aNormi
                      an2=-is*rx(i1,i2,i3,axis1,1)*aNormi
                      u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(
     & 0))
                      u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(
     & 0))
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        a11=an1*(lambda+2.0*mu)*rx(i1,i2,i3,axis2,0)+
     & an2*mu*rx(i1,i2,i3,axis2,1)
                        a12=an1*lambda*rx(i1,i2,i3,axis2,1)+an2*mu*rx(
     & i1,i2,i3,axis2,0)
                        b1=an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,i3,s21c)-(
     & an1*(lambda+2.0*mu)*rx(i1,i2,i3,axis1,0)+an2*mu*rx(i1,i2,i3,
     & axis1,1))*u1r-(an1*lambda*rx(i1,i2,i3,axis1,1)+an2*mu*rx(i1,i2,
     & i3,axis1,0))*u2r
                        a21=an1*mu*rx(i1,i2,i3,axis2,1)+an2*lambda*rx(
     & i1,i2,i3,axis2,0)
                        a22=an1*mu*rx(i1,i2,i3,axis2,0)+an2*(lambda+
     & 2.0*mu)*rx(i1,i2,i3,axis2,1)
                        b2=an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,i3,s22c)-(
     & an1*mu*rx(i1,i2,i3,axis1,1)+an2*lambda*rx(i1,i2,i3,axis1,0))*
     & u1r-(an1*mu*rx(i1,i2,i3,axis1,0)+an2*(lambda+2.0*mu)*rx(i1,i2,
     & i3,axis1,1))*u2r
                        deti=1.0/(a11*a22-a21*a12)
                        u1s=( b1*a22-b2*a12)*deti
                        u2s=(-b1*a21+b2*a11)*deti
                        u1x=rx(i1,i2,i3,0,0)*u1r+rx(i1,i2,i3,1,0)*u1s
                        u1y=rx(i1,i2,i3,0,1)*u1r+rx(i1,i2,i3,1,1)*u1s
                        u2x=rx(i1,i2,i3,0,0)*u2r+rx(i1,i2,i3,1,0)*u2s
                        u2y=rx(i1,i2,i3,0,1)*u2r+rx(i1,i2,i3,1,1)*u2s
                        u(i1,i2,i3,s11c)=(lambda+2.0*mu)*u1x+lambda*u2y
                        u(i1,i2,i3,s21c)=mu*(u1y+u2x)
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s21c)
                        u(i1,i2,i3,s22c)=(lambda+2.0*mu)*u2y+lambda*u1x
                      else
                        ! initialize
                        is1=1-2*side1
                        is2=1-2*side2
                        if (.true.) then   ! true/false switch here is for testing Cartesian grids
c              aNormi1=1./max(epsx,sqrt(rx(i1,i2,i3,axis1,0)**2+rx(i1,i2,i3,axis1,1)**2))
c              an11=-is1*rx(i1,i2,i3,axis1,0)*aNormi1          ! normals for axis1,side1
c              an21=-is1*rx(i1,i2,i3,axis1,1)*aNormi1
                        aNormi1=aNormi
                        an11=an1
                        an21=an2
                        u1s0=is2*(u(i1,i2+is2,i3,uc)-u(i1,i2,i3,uc))
     & /dr(1)
                        u2s0=is2*(u(i1,i2+is2,i3,vc)-u(i1,i2,i3,vc))
     & /dr(1)
                        u1s=u1s0
                        u2s=u2s0
                        ! Newton iteration for u1s,u2s
                        ier=0
                        iter=1
                        istop=0
                        bmax=10.*toler
                        do while (bmax.gt.toler)
                          u1x=rx(i1,i2,i3,0,0)*u1r+rx(i1,i2,i3,1,0)*u1s
                          u1y=rx(i1,i2,i3,0,1)*u1r+rx(i1,i2,i3,1,1)*u1s
                          u2x=rx(i1,i2,i3,0,0)*u2r+rx(i1,i2,i3,1,0)*u2s
                          u2y=rx(i1,i2,i3,0,1)*u2r+rx(i1,i2,i3,1,1)*u2s
                          ! compute stress and the deriv based on current deformation gradient
                          !                      ideriv=1
                          !                      call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1x
                          du(1,2)=u1y
                          du(2,1)=u2x
                          du(2,2)=u2y
                          ideriv=1
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          coef11=rx(i1,i2,i3,0,1)-u1s/det(i1,i2,i3)
                          coef21=rx(i1,i2,i3,0,0)+u2s/det(i1,i2,i3)
                          alpha1=sqrt(coef11**2+coef21**2)*aNormi1
                          f11=bcf(side1,axis1,i1,i2,i3,uc)*alpha1      
     &   ! given traction forces
                          f21=bcf(side1,axis1,i1,i2,i3,vc)*alpha1
                          fact=aNormi1/(det(i1,i2,i3)*sqrt(coef11**2+
     & coef21**2))
                          dalpha11=-coef11*fact
                          dalpha12= coef21*fact
                          ! construct linear system
                          bb(1)=an11*p(1,1)+an21*p(2,1)-f11
                          bb(2)=an11*p(1,2)+an21*p(2,2)-f21
                          aa(1,1)= an11*(dpdf(1,1)*rx(i1,i2,i3,1,0)+
     & dpdf(1,2)*rx(i1,i2,i3,1,1)) +an21*(dpdf(3,1)*rx(i1,i2,i3,1,0)+
     & dpdf(3,2)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,uc)*
     & dalpha11
                          aa(1,2)= an11*(dpdf(1,3)*rx(i1,i2,i3,1,0)+
     & dpdf(1,4)*rx(i1,i2,i3,1,1)) +an21*(dpdf(3,3)*rx(i1,i2,i3,1,0)+
     & dpdf(3,4)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,uc)*
     & dalpha12
                          aa(2,1)= an11*(dpdf(2,1)*rx(i1,i2,i3,1,0)+
     & dpdf(2,2)*rx(i1,i2,i3,1,1)) +an21*(dpdf(4,1)*rx(i1,i2,i3,1,0)+
     & dpdf(4,2)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,vc)*
     & dalpha11
                          aa(2,2)= an11*(dpdf(2,3)*rx(i1,i2,i3,1,0)+
     & dpdf(2,4)*rx(i1,i2,i3,1,1)) +an21*(dpdf(4,3)*rx(i1,i2,i3,1,0)+
     & dpdf(4,4)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,vc)*
     & dalpha12
                          ! solve the 2x2 system
                          determ=aa(1,1)*aa(2,2)-aa(1,2)*aa(2,1)
                          du1s=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                          du2s=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                          bmax=max(abs(bb(1)),abs(bb(2)))/lambda
                          if (istop.ne.0) then
                            write(6,'(1x,i2,3(1x,1pe15.8))')iter,bb(1),
     & bb(2),bmax
                          end if
                          ! update
                          u1s=u1s-du1s
                          u2s=u2s-du2s
                          iter=iter+1
                          ! check for convergence
                          if (iter.gt.itmax) then
                            write(6,*)'Error (bcOptSmFOS) : Newton 
     & failed to converge'
                            if (istop.eq.0) then
                              ier=0
                              iter=1
                              istop=1
                              u1s=u1s0
                              u2s=u2s0
                            else
                              stop 7782
                            end if
                          end if
                        end do
                        ! set displacement in the ghost point and set stress in the corner
                        u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is2*
     & dr(1)*u1s
                        u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is2*
     & dr(1)*u2s
                        u(i1,i2,i3,s11c)=p(1,1)
                        u(i1,i2,i3,s12c)=p(1,2)
                        u(i1,i2,i3,s21c)=p(2,1)
                        u(i1,i2,i3,s22c)=p(2,2)
                        !  compute v1s and v2s
                        v1r=(u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,v1c))/(2.0*
     & dr(0))
                        v2r=(u(i1+1,i2,i3,v2c)-u(i1-1,i2,i3,v2c))/(2.0*
     & dr(0))
                        aa(1,3)= an11*(dpdf(1,1)*rx(i1,i2,i3,0,0)+dpdf(
     & 1,2)*rx(i1,i2,i3,0,1)) +an21*(dpdf(3,1)*rx(i1,i2,i3,0,0)+dpdf(
     & 3,2)*rx(i1,i2,i3,0,1))
                        aa(1,4)= an11*(dpdf(1,3)*rx(i1,i2,i3,0,0)+dpdf(
     & 1,4)*rx(i1,i2,i3,0,1)) +an21*(dpdf(3,3)*rx(i1,i2,i3,0,0)+dpdf(
     & 3,4)*rx(i1,i2,i3,0,1))
                        bb(1)=bcf(side1,axis1,i1,i2,i3,v1c)*alpha1-aa(
     & 1,3)*v1r-aa(1,4)*v2r
                        aa(2,3)= an11*(dpdf(2,1)*rx(i1,i2,i3,0,0)+dpdf(
     & 2,2)*rx(i1,i2,i3,0,1)) +an21*(dpdf(4,1)*rx(i1,i2,i3,0,0)+dpdf(
     & 4,2)*rx(i1,i2,i3,0,1))
                        aa(2,4)= an11*(dpdf(2,3)*rx(i1,i2,i3,0,0)+dpdf(
     & 2,4)*rx(i1,i2,i3,0,1)) +an21*(dpdf(4,3)*rx(i1,i2,i3,0,0)+dpdf(
     & 4,4)*rx(i1,i2,i3,0,1))
                        bb(2)=bcf(side1,axis1,i1,i2,i3,v2c)*alpha1-aa(
     & 2,3)*v1r-aa(2,4)*v2r
                        v1s=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                        v2s=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                        !  set velocity in the ghost point
                        u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)-2.*is2*
     & dr(1)*v1s
                        u(i1,i2-is2,i3,v2c)=u(i1,i2+is2,i3,v2c)-2.*is2*
     & dr(1)*v2s
                        else   ! else true/false testing
                        u1s=0.
                        u2s=0.
                        u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is2*
     & dr(1)*u1s
                        u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is2*
     & dr(1)*u2s
                        u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)
                        u(i1,i2-is2,i3,v2c)=u(i1,i2+is2,i3,v2c)
                        u(i1,i2,i3,s11c)=0.
                        u(i1,i2,i3,s12c)=0.
                        u(i1,i2,i3,s21c)=0.
                        u(i1,i2,i3,s22c)=0.
                        end if   ! end true/false testing
                      end if
                    elseif (boundaryCondition(side1,axis1)
     & .eq.displacementBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC .and. fixupTractionDisplacementCorners ) then
                      ! non-Cartesian grid, mix bcs, case 2  (Should be okay for both new and old bcs)
                      is=1-2*side2
                      aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis2,0)**2+
     & rx(i1,i2,i3,axis2,1)**2))
                      an1=-is*rx(i1,i2,i3,axis2,0)*aNormi
                      an2=-is*rx(i1,i2,i3,axis2,1)*aNormi
                      u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(
     & 1))
                      u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(
     & 1))
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        a11=an1*(lambda+2.0*mu)*rx(i1,i2,i3,axis1,0)+
     & an2*mu*rx(i1,i2,i3,axis1,1)
                        a12=an1*lambda*rx(i1,i2,i3,axis1,1)+an2*mu*rx(
     & i1,i2,i3,axis1,0)
                        b1=an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,i3,s21c)-(
     & an1*(lambda+2.0*mu)*rx(i1,i2,i3,axis2,0)+an2*mu*rx(i1,i2,i3,
     & axis2,1))*u1s-(an1*lambda*rx(i1,i2,i3,axis2,1)+an2*mu*rx(i1,i2,
     & i3,axis2,0))*u2s
                        a21=an1*mu*rx(i1,i2,i3,axis1,1)+an2*lambda*rx(
     & i1,i2,i3,axis1,0)
                        a22=an1*mu*rx(i1,i2,i3,axis1,0)+an2*(lambda+
     & 2.0*mu)*rx(i1,i2,i3,axis1,1)
                        b2=an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,i3,s22c)-(
     & an1*mu*rx(i1,i2,i3,axis2,1)+an2*lambda*rx(i1,i2,i3,axis2,0))*
     & u1s-(an1*mu*rx(i1,i2,i3,axis2,0)+an2*(lambda+2.0*mu)*rx(i1,i2,
     & i3,axis2,1))*u2s
                        deti=1.0/(a11*a22-a21*a12)
                        u1r=( b1*a22-b2*a12)*deti
                        u2r=(-b1*a21+b2*a11)*deti
                        u1x=rx(i1,i2,i3,0,0)*u1r+rx(i1,i2,i3,1,0)*u1s
                        u1y=rx(i1,i2,i3,0,1)*u1r+rx(i1,i2,i3,1,1)*u1s
                        u2x=rx(i1,i2,i3,0,0)*u2r+rx(i1,i2,i3,1,0)*u2s
                        u2y=rx(i1,i2,i3,0,1)*u2r+rx(i1,i2,i3,1,1)*u2s
                        u(i1,i2,i3,s11c)=(lambda+2.0*mu)*u1x+lambda*u2y
                        u(i1,i2,i3,s21c)=mu*(u1y+u2x)
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s21c)
                        u(i1,i2,i3,s22c)=(lambda+2.0*mu)*u2y+lambda*u1x
                      else
                        ! initialize
                        is1=1-2*side1
                        is2=1-2*side2
                        if (.true.) then   ! true/false switch here is for testing Cartesian grids
c              aNormi2=1./max(epsx,sqrt(rx(i1,i2,i3,axis2,0)**2+rx(i1,i2,i3,axis2,1)**2))
c              an12=-is2*rx(i1,i2,i3,axis2,0)*aNormi2          ! normals for axis2,side2
c              an22=-is2*rx(i1,i2,i3,axis2,1)*aNormi2
                        aNormi2=aNormi
                        an12=an1
                        an22=an2
                        u1r0=is1*(u(i1+is1,i2,i3,uc)-u(i1,i2,i3,uc))
     & /dr(0)
                        u2r0=is1*(u(i1+is1,i2,i3,vc)-u(i1,i2,i3,vc))
     & /dr(0)
                        u1r=u1r0
                        u2r=u2r0
                        ! Newton iteration for u1r,u2r
                        ier=0
                        iter=1
                        istop=0
                        bmax=10.*toler
                        do while (bmax.gt.toler)
                          u1x=rx(i1,i2,i3,0,0)*u1r+rx(i1,i2,i3,1,0)*u1s
                          u1y=rx(i1,i2,i3,0,1)*u1r+rx(i1,i2,i3,1,1)*u1s
                          u2x=rx(i1,i2,i3,0,0)*u2r+rx(i1,i2,i3,1,0)*u2s
                          u2y=rx(i1,i2,i3,0,1)*u2r+rx(i1,i2,i3,1,1)*u2s
                          ! compute stress and the deriv based on current deformation gradient
                          !                      ideriv=1
                          !                      call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1x
                          du(1,2)=u1y
                          du(2,1)=u2x
                          du(2,2)=u2y
                          ideriv=1
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          coef12=rx(i1,i2,i3,1,1)+u1r/det(i1,i2,i3)
                          coef22=rx(i1,i2,i3,1,0)-u2r/det(i1,i2,i3)
                          alpha2=sqrt(coef12**2+coef22**2)*aNormi2
                          f12=bcf(side2,axis2,i1,i2,i3,uc)*alpha2      
     &    ! given traction forces
                          f22=bcf(side2,axis2,i1,i2,i3,vc)*alpha2
                          fact=aNormi2/(det(i1,i2,i3)*sqrt(coef12**2+
     & coef22**2))
                          dalpha21= coef12*fact
                          dalpha22=-coef22*fact
                          ! construct linear system
                          bb(1)=an12*p(1,1)+an22*p(2,1)-f12
                          bb(2)=an12*p(1,2)+an22*p(2,2)-f22
                          aa(1,1)= an12*(dpdf(1,1)*rx(i1,i2,i3,0,0)+
     & dpdf(1,2)*rx(i1,i2,i3,0,1)) +an22*(dpdf(3,1)*rx(i1,i2,i3,0,0)+
     & dpdf(3,2)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,uc)*
     & dalpha21
                          aa(1,2)= an12*(dpdf(1,3)*rx(i1,i2,i3,0,0)+
     & dpdf(1,4)*rx(i1,i2,i3,0,1)) +an22*(dpdf(3,3)*rx(i1,i2,i3,0,0)+
     & dpdf(3,4)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,uc)*
     & dalpha22
                          aa(2,1)= an12*(dpdf(2,1)*rx(i1,i2,i3,0,0)+
     & dpdf(2,2)*rx(i1,i2,i3,0,1)) +an22*(dpdf(4,1)*rx(i1,i2,i3,0,0)+
     & dpdf(4,2)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,vc)*
     & dalpha21
                          aa(2,2)= an12*(dpdf(2,3)*rx(i1,i2,i3,0,0)+
     & dpdf(2,4)*rx(i1,i2,i3,0,1)) +an22*(dpdf(4,3)*rx(i1,i2,i3,0,0)+
     & dpdf(4,4)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,vc)*
     & dalpha22
                          ! solve the 2x2 system
                          determ=aa(1,1)*aa(2,2)-aa(1,2)*aa(2,1)
                          du1r=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                          du2r=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                          bmax=max(abs(bb(1)),abs(bb(2)))/lambda
                          if (istop.ne.0) then
                            write(6,'(1x,i2,3(1x,1pe15.8))')iter,bb(1),
     & bb(2),bmax
                          end if
                          ! update
                          u1r=u1r-du1r
                          u2r=u2r-du2r
                          iter=iter+1
                          ! check for convergence
                          if (iter.gt.itmax) then
                            write(6,*)'Error (bcOptSmFOS) : Newton 
     & failed to converge'
                            if (istop.eq.0) then
                              ier=0
                              iter=1
                              istop=1
                              u1r=u1r0
                              u2r=u2r0
                            else
                              stop 7783
                            end if
                          end if
                        end do
                        ! set displacement in the ghost point and stress in the corner
                        u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*
     & dr(0)*u1r
                        u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*
     & dr(0)*u2r
                        u(i1,i2,i3,s11c)=p(1,1)
                        u(i1,i2,i3,s12c)=p(1,2)
                        u(i1,i2,i3,s21c)=p(2,1)
                        u(i1,i2,i3,s22c)=p(2,2)
                        !  compute v1r and v2r
                        v1s=(u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,v1c))/(2.0*
     & dr(1))
                        v2s=(u(i1,i2+1,i3,v2c)-u(i1,i2-1,i3,v2c))/(2.0*
     & dr(1))
                        aa(1,3)= an12*(dpdf(1,1)*rx(i1,i2,i3,1,0)+dpdf(
     & 1,2)*rx(i1,i2,i3,1,1)) +an22*(dpdf(3,1)*rx(i1,i2,i3,1,0)+dpdf(
     & 3,2)*rx(i1,i2,i3,1,1))
                        aa(1,4)= an12*(dpdf(1,3)*rx(i1,i2,i3,1,0)+dpdf(
     & 1,4)*rx(i1,i2,i3,1,1)) +an22*(dpdf(3,3)*rx(i1,i2,i3,1,0)+dpdf(
     & 3,4)*rx(i1,i2,i3,1,1))
                        bb(1)=bcf(side2,axis2,i1,i2,i3,v1c)*alpha2-aa(
     & 1,3)*v1s-aa(1,4)*v2s
                        aa(2,3)= an12*(dpdf(2,1)*rx(i1,i2,i3,1,0)+dpdf(
     & 2,2)*rx(i1,i2,i3,1,1)) +an22*(dpdf(4,1)*rx(i1,i2,i3,1,0)+dpdf(
     & 4,2)*rx(i1,i2,i3,1,1))
                        aa(2,4)= an12*(dpdf(2,3)*rx(i1,i2,i3,1,0)+dpdf(
     & 2,4)*rx(i1,i2,i3,1,1)) +an22*(dpdf(4,3)*rx(i1,i2,i3,1,0)+dpdf(
     & 4,4)*rx(i1,i2,i3,1,1))
                        bb(2)=bcf(side2,axis2,i1,i2,i3,v2c)*alpha2-aa(
     & 2,3)*v1s-aa(2,4)*v2s
                        v1r=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                        v2r=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                        !  set velocity in the ghost point
                        u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)-2.*is1*
     & dr(0)*v1r
                        u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)-2.*is1*
     & dr(0)*v2r
                        else   ! else true/false testing
                        u1r=0.
                        u2r=0.
                        u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*
     & dr(0)*u1r
                        u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*
     & dr(0)*u2r
                        u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)
                        u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)
                        u(i1,i2,i3,s11c)=0.
                        u(i1,i2,i3,s12c)=0.
                        u(i1,i2,i3,s21c)=0.
                        u(i1,i2,i3,s22c)=0.
                        end if   ! end true/false testing
                      end if
                    end if
                  end if
                end do
              end do
            end if
            ! ..add on TZ flow contribution (if necessary)
            if (twilightZone.ne.0) then
              do side1=0,1
                i1=gridIndexRange(side1,axis1)
                do side2=0,1
                  i2=gridIndexRange(side2,axis2)
                  if (mask(i1,i2,i3).ne.0) then
                    if (bctype.eq.linearBoundaryCondition) then     ! linear case only
                      if (materialFormat.ne.constantMaterialProperties)
     &  then
                        call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.,t,muc,mu)
                        call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.,t,lambdac,lambda)
                      end if
                    end if
                    if (boundaryCondition(side1,axis1)
     & .eq.displacementBC.and.boundaryCondition(side2,axis2)
     & .eq.displacementBC) then
                      ! pure displacement/velocity bcs
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s11c,s11e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s12c,s12e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s21c,s21e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s22c,s22e)
                      call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,uc,u1xe)
                      call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,vc,u2xe)
                      call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,uc,u1ye)
                      call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,vc,u2ye)
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+s11e-(lambda*
     & (u1xe+u2ye)+2.0*mu*u1xe)
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+s12e-mu*(
     & u1ye+u2xe)
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+s21e-mu*(
     & u1ye+u2xe)
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+s22e-(lambda*
     & (u1xe+u2ye)+2.0*mu*u2ye)
                      else                                             
     &   ! SVK case
                        ! call smbcsdp (u1xe,u1ye,u2xe,u2ye,lambda,mu,p,dpdf,0)
                        du(1,1)=u1xe
                        du(1,2)=u1ye
                        du(2,1)=u2xe
                        du(2,2)=u2ye
                        ideriv=0
                        call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+s11e-p(1,1)
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+s12e-p(1,2)
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+s21e-p(2,1)
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+s22e-p(2,2)
                      end if
                    elseif (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC) then
                      ! pure traction bcs
                      !                  No TZ forcing needed here.  For the Cartesian case, no fix was done, and for the
                      !                  non-Cartesian case, the forcing was already included in the bcf array.
                    elseif (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.displacementBC) then
                      ! mix bcs, case 1
                      call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,uc,u1xe)
                      call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,vc,u2xe)
                      call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,uc,u1ye)
                      call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,vc,u2ye)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s11c,s11e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s12c,s12e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s21c,s21e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s22c,s22e)
                      if (gridType.eq.rectangular) then
                      !   Cartesian case
                        if (bctype.eq.linearBoundaryCondition) then    
     &                            ! linear case
                          u1ye=(s12e-mu*u2xe)/mu
                          u2ye=(s11e-(lambda+2.0*mu)*u1xe)/lambda
                          u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+s21e-mu*(
     & u1ye+u2xe)
                          u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+s22e-
     & lambda*(u1xe+u2ye)-2.0*mu*u2ye
                        end if
                      else
                        !   non-Cartesian case
                        is=1-2*side1
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis1,0)**
     & 2+rx(i1,i2,i3,axis1,1)**2))
                        an1=-is*rx(i1,i2,i3,axis1,0)*aNormi
                        an2=-is*rx(i1,i2,i3,axis1,1)*aNormi
                        if (bctype.eq.linearBoundaryCondition) then    
     &                            ! linear case
                          deti=1.0/(rx(i1,i2,i3,axis2,1)*rx(i1,i2,i3,
     & axis1,0)-rx(i1,i2,i3,axis2,0)*rx(i1,i2,i3,axis1,1))
                          u1re=(rx(i1,i2,i3,axis2,1)*u1xe-rx(i1,i2,i3,
     & axis2,0)*u1ye)*deti
                          u2re=(rx(i1,i2,i3,axis2,1)*u2xe-rx(i1,i2,i3,
     & axis2,0)*u2ye)*deti
                          a11=an1*(lambda+2.0*mu)*rx(i1,i2,i3,axis2,0)+
     & an2*mu*rx(i1,i2,i3,axis2,1)
                          a12=an1*lambda*rx(i1,i2,i3,axis2,1)+an2*mu*
     & rx(i1,i2,i3,axis2,0)
                          b1=an1*s11e+an2*s21e-(an1*(lambda+2.0*mu)*rx(
     & i1,i2,i3,axis1,0)+an2*mu*rx(i1,i2,i3,axis1,1))*u1re-(an1*
     & lambda*rx(i1,i2,i3,axis1,1)+an2*mu*rx(i1,i2,i3,axis1,0))*u2re
                          a21=an1*mu*rx(i1,i2,i3,axis2,1)+an2*lambda*
     & rx(i1,i2,i3,axis2,0)
                          a22=an1*mu*rx(i1,i2,i3,axis2,0)+an2*(lambda+
     & 2.0*mu)*rx(i1,i2,i3,axis2,1)
                          b2=an1*s12e+an2*s22e-(an1*mu*rx(i1,i2,i3,
     & axis1,1)+an2*lambda*rx(i1,i2,i3,axis1,0))*u1re-(an1*mu*rx(i1,
     & i2,i3,axis1,0)+an2*(lambda+2.0*mu)*rx(i1,i2,i3,axis1,1))*u2re
                          deti=1.0/(a11*a22-a21*a12)
                          u1se=( b1*a22-b2*a12)*deti
                          u2se=(-b1*a21+b2*a11)*deti
                          u1xe=rx(i1,i2,i3,0,0)*u1re+rx(i1,i2,i3,1,0)*
     & u1se
                          u1ye=rx(i1,i2,i3,0,1)*u1re+rx(i1,i2,i3,1,1)*
     & u1se
                          u2xe=rx(i1,i2,i3,0,0)*u2re+rx(i1,i2,i3,1,0)*
     & u2se
                          u2ye=rx(i1,i2,i3,0,1)*u2re+rx(i1,i2,i3,1,1)*
     & u2se
                          u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+s11e-(
     & lambda+2.0*mu)*u1xe-lambda*u2ye
                          u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+s21e-mu*(
     & u1ye+u2xe)
                          u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+s12e-mu*(
     & u1ye+u2xe)
                          u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+s22e-(
     & lambda+2.0*mu)*u2ye-lambda*u1xe
                        end if
                      end if
                    elseif (boundaryCondition(side1,axis1)
     & .eq.displacementBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC) then
                      ! mix bcs, case 2
                      call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,uc,u1xe)
                      call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,vc,u2xe)
                      call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,uc,u1ye)
                      call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,vc,u2ye)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s11c,s11e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s12c,s12e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s21c,s21e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s22c,s22e)
                      if (gridType.eq.rectangular) then
                        !  Cartesian case
                        if (bctype.eq.linearBoundaryCondition) then    
     &                            ! linear case
                          u1xe=(s22e-(lambda+2.0*mu)*u2ye)/lambda
                          u2xe=(s21e-mu*u1ye)/mu
                          u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+s11e-
     & lambda*(u1xe+u2ye)-2.0*mu*u1xe
                          u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+s12e-mu*(
     & u1ye+u2xe)
                        end if
                      else
                        ! non-Cartesian case
                        is=1-2*side2
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis2,0)**
     & 2+rx(i1,i2,i3,axis2,1)**2))
                        an1=-is*rx(i1,i2,i3,axis2,0)*aNormi
                        an2=-is*rx(i1,i2,i3,axis2,1)*aNormi
                        if (bctype.eq.linearBoundaryCondition) then    
     &                            ! linear case
                          deti=1.0/(rx(i1,i2,i3,axis1,0)*rx(i1,i2,i3,
     & axis2,1)-rx(i1,i2,i3,axis1,1)*rx(i1,i2,i3,axis2,0))
                          u1se=(rx(i1,i2,i3,axis1,0)*u1ye-rx(i1,i2,i3,
     & axis1,1)*u1xe)*deti
                          u2se=(rx(i1,i2,i3,axis1,0)*u2ye-rx(i1,i2,i3,
     & axis1,1)*u2xe)*deti
                          a11=an1*(lambda+2.0*mu)*rx(i1,i2,i3,axis1,0)+
     & an2*mu*rx(i1,i2,i3,axis1,1)
                          a12=an1*lambda*rx(i1,i2,i3,axis1,1)+an2*mu*
     & rx(i1,i2,i3,axis1,0)
                          b1=an1*s11e+an2*s21e-(an1*(lambda+2.0*mu)*rx(
     & i1,i2,i3,axis2,0)+an2*mu*rx(i1,i2,i3,axis2,1))*u1se-(an1*
     & lambda*rx(i1,i2,i3,axis2,1)+an2*mu*rx(i1,i2,i3,axis2,0))*u2se
                          a21=an1*mu*rx(i1,i2,i3,axis1,1)+an2*lambda*
     & rx(i1,i2,i3,axis1,0)
                          a22=an1*mu*rx(i1,i2,i3,axis1,0)+an2*(lambda+
     & 2.0*mu)*rx(i1,i2,i3,axis1,1)
                          b2=an1*s12e+an2*s22e-(an1*mu*rx(i1,i2,i3,
     & axis2,1)+an2*lambda*rx(i1,i2,i3,axis2,0))*u1se-(an1*mu*rx(i1,
     & i2,i3,axis2,0)+an2*(lambda+2.0*mu)*rx(i1,i2,i3,axis2,1))*u2se
                          deti=1.0/(a11*a22-a21*a12)
                          u1re=( b1*a22-b2*a12)*deti
                          u2re=(-b1*a21+b2*a11)*deti
                          u1xe=rx(i1,i2,i3,0,0)*u1re+rx(i1,i2,i3,1,0)*
     & u1se
                          u1ye=rx(i1,i2,i3,0,1)*u1re+rx(i1,i2,i3,1,1)*
     & u1se
                          u2xe=rx(i1,i2,i3,0,0)*u2re+rx(i1,i2,i3,1,0)*
     & u2se
                          u2ye=rx(i1,i2,i3,0,1)*u2re+rx(i1,i2,i3,1,1)*
     & u2se
                          u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+s11e-(
     & lambda+2.0*mu)*u1xe-lambda*u2ye
                          u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+s21e-mu*(
     & u1ye+u2xe)
                          u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+s12e-mu*(
     & u1ye+u2xe)
                          u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+s22e-(
     & lambda+2.0*mu)*u2ye-lambda*u1xe
                        end if
                      end if
                    end if
                  end if
                end do
              end do
            end if
c       !*******
c       !******* re-extrapolate the stress to first ghost line near corners ********
c       !         (1) Extrapolate points A and B (below) on displacement sides
c       !         (2) Extrapolate corner point C on all physical sides 
c       !*******
c
c       !               |
c       !               |
c       !            A--+---+---+
c       !               |
c       !            C  B
c
c       i3=gridIndexRange(0,2)
c       do side1=0,1
c         i1=gridIndexRange(side1,axis1)
c         is1=1-2*side1
c         do side2=0,1
c           i2=gridIndexRange(side2,axis2)
c           is2=1-2*side2
c
c           ! extrapolate in the i1 direction
c           !*wdh       if (boundaryCondition(side1,axis1).eq.tractionBC) then
c           if (boundaryCondition(side1,axis1).eq.tractionBC.and.boundaryCondition(side2,axis2).gt.0) then
c             if (mask(i1,i2,i3).ne.0) then
cc               do n=0,numberOfComponents-1
c               do n=2,5
c                 u(i1-is1,i2,i3,n)=extrap3(u,i1,i2,i3,n,is1,0,0)
c               end do
c             end if
c           end if
c
c           ! extrapolate in the i2 direction
c           !*wdh       if (boundaryCondition(side2,axis2).eq.tractionBC) then
c           if (boundaryCondition(side2,axis2).eq.tractionBC.and.boundaryCondition(side1,axis1).gt.0) then
c             if (mask(i1,i2,i3).ne.0) then
cc               do n=0,numberOfComponents-1
c               do n=2,5
c                 u(i1,i2-is2,i3,n)=extrap3(u,i1,i2,i3,n,0,is2,0)
c               end do
c             end if
c           end if
c
c           ! extrapolate in the diagonal direction
c           if (boundaryCondition(side1,axis1).gt.0.and.boundaryCondition(side2,axis2).gt.0) then
c             if (mask(i1,i2,i3).ne.0) then
cc               do n=0,numberOfComponents-1
c               do n=2,5
c                 u(i1-is1,i2-is2,i3,n)=extrap3(u,i1,i2,i3,n,is1,is2,0)
c               end do
c             end if
c           end if
c         end do
c       end do
          ! return after corner stress fix-up for debugging
          if (.false.) return
         !*******
         !******* Secondary Neumann boundary conditions (compatibility conditions) ********
         !*******
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
            if( boundaryCondition(side,axis).eq.displacementBC .and. 
     & computeTractionOnDisplacementBoundaries )then
             if( gridType.eq.rectangular )then
              ! ****************************************************************
              ! ********* DISPLACEMENT COMPATIBILITY : Cartesian Grid **********
              ! ****************************************************************
              !      u(j1,j2,3:6)=stress (S11,S12,S21,S22)
              ! Use:
              !   s11_x + s21_y = rho * u_tt  (from momentum eqn)
              !   s12_x + s22_y = rho * v_tt  (from momentum eqn)
              if (bctype.eq.linearBoundaryCondition) then   ! linear case
                ! new (all materialFormat cases)
                if (materialFormat.eq.constantMaterialProperties) then
                  if( axis.eq.0 )then
                   ! *wdh* 090909 -- only set ghost points where mask > 0 since we assume values at neighbours
                    i3=n3a
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
                     accel1=rho*bcf(side,axis,i1,i2,i3,s11c)   ! rho times given acceleration
                     accel2=rho*bcf(side,axis,i1,i2,i3,s12c)
                     u(i1-is1,i2,i3,s11c)=u(i1+is1,i2,i3,s11c)-2.*is*
     & dx(0)*(accel1-(u(i1,i2+1,i3,s21c)-u(i1,i2-1,i3,s21c))/(2.*dx(1)
     & ))
                     u(i1-is1,i2,i3,s12c)=u(i1+is1,i2,i3,s12c)-2.*is*
     & dx(0)*(accel2-(u(i1,i2+1,i3,s22c)-u(i1,i2-1,i3,s22c))/(2.*dx(1)
     & ))
                     u(i1-is1,i2,i3,s21c)=u(i1-is1,i2,i3,s12c)
                    end if
                    end do
                    end do
                  else
                    i3=n3a
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
                     accel1=rho*bcf(side,axis,i1,i2,i3,s11c)  ! rho times given acceleration
                     accel2=rho*bcf(side,axis,i1,i2,i3,s12c)
                     u(i1,i2-is2,i3,s21c)=u(i1,i2+is2,i3,s21c)-2.*is*
     & dx(1)*(accel1-(u(i1+1,i2,i3,s11c)-u(i1-1,i2,i3,s11c))/(2.*dx(0)
     & ))
                     u(i1,i2-is2,i3,s22c)=u(i1,i2+is2,i3,s22c)-2.*is*
     & dx(1)*(accel2-(u(i1+1,i2,i3,s12c)-u(i1-1,i2,i3,s12c))/(2.*dx(0)
     & ))
                     u(i1,i2-is2,i3,s12c)=u(i1,i2-is2,i3,s21c)
                    end if
                    end do
                    end do
                  end if
                elseif (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                  if( axis.eq.0 )then
                   ! *wdh* 090909 -- only set ghost points where mask > 0 since we assume values at neighbours
                    i3=n3a
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
                     accel1=rhopc(i1,i2)*bcf(side,axis,i1,i2,i3,s11c)   ! rho times given acceleration
                     accel2=rhopc(i1,i2)*bcf(side,axis,i1,i2,i3,s12c)
                     u(i1-is1,i2,i3,s11c)=u(i1+is1,i2,i3,s11c)-2.*is*
     & dx(0)*(accel1-(u(i1,i2+1,i3,s21c)-u(i1,i2-1,i3,s21c))/(2.*dx(1)
     & ))
                     u(i1-is1,i2,i3,s12c)=u(i1+is1,i2,i3,s12c)-2.*is*
     & dx(0)*(accel2-(u(i1,i2+1,i3,s22c)-u(i1,i2-1,i3,s22c))/(2.*dx(1)
     & ))
                     u(i1-is1,i2,i3,s21c)=u(i1-is1,i2,i3,s12c)
                    end if
                    end do
                    end do
                  else
                    i3=n3a
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
                     accel1=rhopc(i1,i2)*bcf(side,axis,i1,i2,i3,s11c)  ! rho times given acceleration
                     accel2=rhopc(i1,i2)*bcf(side,axis,i1,i2,i3,s12c)
                     u(i1,i2-is2,i3,s21c)=u(i1,i2+is2,i3,s21c)-2.*is*
     & dx(1)*(accel1-(u(i1+1,i2,i3,s11c)-u(i1-1,i2,i3,s11c))/(2.*dx(0)
     & ))
                     u(i1,i2-is2,i3,s22c)=u(i1,i2+is2,i3,s22c)-2.*is*
     & dx(1)*(accel2-(u(i1+1,i2,i3,s12c)-u(i1-1,i2,i3,s12c))/(2.*dx(0)
     & ))
                     u(i1,i2-is2,i3,s12c)=u(i1,i2-is2,i3,s21c)
                    end if
                    end do
                    end do
                  end if
                elseif (materialFormat.eq.variableMaterialProperties) 
     & then
                  if( axis.eq.0 )then
                   ! *wdh* 090909 -- only set ghost points where mask > 0 since we assume values at neighbours
                    i3=n3a
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
                     accel1=rhov(i1,i2)*bcf(side,axis,i1,i2,i3,s11c)   ! rho times given acceleration
                     accel2=rhov(i1,i2)*bcf(side,axis,i1,i2,i3,s12c)
                     u(i1-is1,i2,i3,s11c)=u(i1+is1,i2,i3,s11c)-2.*is*
     & dx(0)*(accel1-(u(i1,i2+1,i3,s21c)-u(i1,i2-1,i3,s21c))/(2.*dx(1)
     & ))
                     u(i1-is1,i2,i3,s12c)=u(i1+is1,i2,i3,s12c)-2.*is*
     & dx(0)*(accel2-(u(i1,i2+1,i3,s22c)-u(i1,i2-1,i3,s22c))/(2.*dx(1)
     & ))
                     u(i1-is1,i2,i3,s21c)=u(i1-is1,i2,i3,s12c)
                     ! write(6,*)'1',i1,i2,rhov(i1,i2)
                     ! pause
                    end if
                    end do
                    end do
                  else
                    i3=n3a
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                    if( mask(i1,i2,i3).gt.0 )then
                     ! rho times given acceleration
                     accel1=rhov(i1,i2)*bcf(side,axis,i1,i2,i3,s11c)
                     accel2=rhov(i1,i2)*bcf(side,axis,i1,i2,i3,s12c)
                     u(i1,i2-is2,i3,s21c)=u(i1,i2+is2,i3,s21c)-2.*is*
     & dx(1)*(accel1-(u(i1+1,i2,i3,s11c)-u(i1-1,i2,i3,s11c))/(2.*dx(0)
     & ))
                     u(i1,i2-is2,i3,s22c)=u(i1,i2+is2,i3,s22c)-2.*is*
     & dx(1)*(accel2-(u(i1+1,i2,i3,s12c)-u(i1-1,i2,i3,s12c))/(2.*dx(0)
     & ))
                     u(i1,i2-is2,i3,s12c)=u(i1,i2-is2,i3,s21c)
                     ! write(6,*)'2',i1,i2,rhov(i1,i2)
                     ! pause
                    end if
                    end do
                    end do
                  end if
                else
                  write(6,*)'Error (bcOptSmFOS) : materialFormat not 
     & supported'
                  stop 4321
                end if
              else     ! SVK case
                ! old (This is the old linear case as well)
                if( axis.eq.0 )then
                 ! *wdh* 090909 -- only set ghost points where mask > 0 since we assume values at neighbours
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   ! rho times given acceleration
                   accel1=rho*bcf(side,axis,i1,i2,i3,s11c)
                   accel2=rho*bcf(side,axis,i1,i2,i3,s12c)
                   u(i1-is1,i2,i3,s11c)=u(i1+is1,i2,i3,s11c)-2.*is*dx(
     & 0)*(accel1-(u(i1,i2+1,i3,s21c)-u(i1,i2-1,i3,s21c))/(2.*dx(1)))
                   u(i1-is1,i2,i3,s12c)=u(i1+is1,i2,i3,s12c)-2.*is*dx(
     & 0)*(accel2-(u(i1,i2+1,i3,s22c)-u(i1,i2-1,i3,s22c))/(2.*dx(1)))
                  end if
                  end do
                  end do
                else
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   ! rho times given acceleration
                   accel1=rho*bcf(side,axis,i1,i2,i3,s11c)
                   accel2=rho*bcf(side,axis,i1,i2,i3,s12c)
                   u(i1,i2-is2,i3,s21c)=u(i1,i2+is2,i3,s21c)-2.*is*dx(
     & 1)*(accel1-(u(i1+1,i2,i3,s11c)-u(i1-1,i2,i3,s11c))/(2.*dx(0)))
                   u(i1,i2-is2,i3,s22c)=u(i1,i2+is2,i3,s22c)-2.*is*dx(
     & 1)*(accel2-(u(i1+1,i2,i3,s12c)-u(i1-1,i2,i3,s12c))/(2.*dx(0)))
                  end if
                  end do
                  end do
                end if
              end if
             else
              ! ******************************************************************
              ! ********* DISPLACEMENT COMPATIBILITY : Curvilinear Grid **********
              ! ******************************************************************
              if( .false. ) then ! choice
              ! To compute (s11,s21) use: 
              !    (1)   D_r[ J*(rx,ry).(s11,s21)] + D_s[J*(sx,sy).(s11,s21)] = J * rho * u_tt  (normal component from momentum eqn)
              !    (2)   Use extrapolated values to get  J*(sx,sy).(s11,s21)(-1)   ("tangential component" from extrapolation)
              ! To give 2 equations for (s11,s21) on the ghost point:
              !   (J rx) s11(-1) + (J ry) s21(-1) = f1 = s11tilde  (from momentum eqn)
              !   (J sx) s11(-1) + (J sy) s21(-1) = f2 = s21tilde  (from extrapolated values)
              ! Solve:
              !     s11(-1) = sy*f1 - ry*f2 
              !     s21(-1) =-sx*f1 + rx*f2
              !
              ! A similar expression holds for (s12,s22) 
              if( axis.eq.0 )then
                i3=n3a
                do i2=n2a,n2b
                do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                 accel1=rho*bcf(side,axis,i1,i2,i3,s11c)   ! rho times given acceleration
                 accel2=rho*bcf(side,axis,i1,i2,i3,s12c)
                 s11tilde=det(i1+is1,i2,i3)*(rx(i1+is1,i2,i3,0,0)*u(i1+
     & is1,i2,i3,s11c)+rx(i1+is1,i2,i3,0,1)*u(i1+is1,i2,i3,s21c)) -2.*
     & dr(0)*is*(det(i1,i2,i3)*accel1 -(det(i1,i2+1,i3)*(rx(i1,i2+1,
     & i3,1,0)*u(i1,i2+1,i3,s11c)+rx(i1,i2+1,i3,1,1)*u(i1,i2+1,i3,
     & s21c)) -det(i1,i2-1,i3)*(rx(i1,i2-1,i3,1,0)*u(i1,i2-1,i3,s11c)+
     & rx(i1,i2-1,i3,1,1)*u(i1,i2-1,i3,s21c)))/(2.*dr(1)))
                 s21tilde=det(i1-is1,i2,i3)*(rx(i1-is1,i2,i3,1,0)*u(i1-
     & is1,i2,i3,s11c)+rx(i1-is1,i2,i3,1,1)*u(i1-is1,i2,i3,s21c))
                 u(i1-is1,i2,i3,s11c)= rx(i1-is1,i2,i3,1,1)*s11tilde-
     & rx(i1-is1,i2,i3,0,1)*s21tilde
                 u(i1-is1,i2,i3,s21c)=-rx(i1-is1,i2,i3,1,0)*s11tilde+
     & rx(i1-is1,i2,i3,0,0)*s21tilde
                 s12tilde=det(i1+is1,i2,i3)*(rx(i1+is1,i2,i3,0,0)*u(i1+
     & is1,i2,i3,s12c)+rx(i1+is1,i2,i3,0,1)*u(i1+is1,i2,i3,s22c)) -2.*
     & dr(0)*is*(det(i1,i2,i3)*accel2 -(det(i1,i2+1,i3)*(rx(i1,i2+1,
     & i3,1,0)*u(i1,i2+1,i3,s12c)+rx(i1,i2+1,i3,1,1)*u(i1,i2+1,i3,
     & s22c)) -det(i1,i2-1,i3)*(rx(i1,i2-1,i3,1,0)*u(i1,i2-1,i3,s12c)+
     & rx(i1,i2-1,i3,1,1)*u(i1,i2-1,i3,s22c)))/(2.*dr(1)))
                 s22tilde=det(i1-is1,i2,i3)*(rx(i1-is1,i2,i3,1,0)*u(i1-
     & is1,i2,i3,s12c)+rx(i1-is1,i2,i3,1,1)*u(i1-is1,i2,i3,s22c))
                 u(i1-is1,i2,i3,s12c)= rx(i1-is1,i2,i3,1,1)*s12tilde-
     & rx(i1-is1,i2,i3,0,1)*s22tilde
                 u(i1-is1,i2,i3,s22c)=-rx(i1-is1,i2,i3,1,0)*s12tilde+
     & rx(i1-is1,i2,i3,0,0)*s22tilde
                end if
                end do
                end do
              else ! axis .eq. 1
                i3=n3a
                do i2=n2a,n2b
                do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                 accel1=rho*bcf(side,axis,i1,i2,i3,s11c)   ! rho times given acceleration
                 accel2=rho*bcf(side,axis,i1,i2,i3,s12c)
                 s11tilde=det(i1,i2-is2,i3)*(rx(i1,i2-is2,i3,0,0)*u(i1,
     & i2-is2,i3,s11c)+rx(i1,i2-is2,i3,0,1)*u(i1,i2-is2,i3,s21c))
                 ! *dws  s11tilde=det(i1,i2+is2,i3)*(rx(i1,i2+is2,i3,0,0)*u(i1,i2+is2,i3,s11c)+rx(i1,i2+is2,i3,0,1)*u(i1,i2+is2,i3,s21c))
                 s21tilde=det(i1,i2+is2,i3)*(rx(i1,i2+is2,i3,1,0)*u(i1,
     & i2+is2,i3,s11c)+rx(i1,i2+is2,i3,1,1)*u(i1,i2+is2,i3,s21c)) -2.*
     & dr(1)*is*(det(i1,i2,i3)*accel1 -(det(i1+1,i2,i3)*(rx(i1+1,i2,
     & i3,0,0)*u(i1+1,i2,i3,s11c)+rx(i1+1,i2,i3,0,1)*u(i1+1,i2,i3,
     & s21c)) -det(i1-1,i2,i3)*(rx(i1-1,i2,i3,0,0)*u(i1-1,i2,i3,s11c)+
     & rx(i1-1,i2,i3,0,1)*u(i1-1,i2,i3,s21c)))/(2.*dr(0)))
                 u(i1,i2-is2,i3,s11c)= rx(i1,i2-is2,i3,1,1)*s11tilde-
     & rx(i1,i2-is2,i3,0,1)*s21tilde
                 u(i1,i2-is2,i3,s21c)=-rx(i1,i2-is2,i3,1,0)*s11tilde+
     & rx(i1,i2-is2,i3,0,0)*s21tilde
                 ! write(*,'(" i1,i2,i3,is1,is2,is,s11,s21(0,1)=",3i4,3i3,2e10.2)') i1,i2,i3,is1,is2,is,u(i1,i2+is2,i3,s11c),u(i1,i2+is2,i3,s21c)
                 ! write(*,'(" det(0,1),s11t,s12t,s11,s21=",5e10.2)') det(i1,i2+is2,i3),s11tilde, s21tilde,u(i1,i2-is2,i3,s11c),u(i1,i2-is2,i3,s21c)
                 s12tilde=det(i1,i2-is2,i3)*(rx(i1,i2-is2,i3,0,0)*u(i1,
     & i2-is2,i3,s12c)+rx(i1,i2-is2,i3,0,1)*u(i1,i2-is2,i3,s22c))
                 ! *dws    s12tilde=det(i1,i2+is2,i3)*(rx(i1,i2+is2,i3,0,0)*u(i1,i2+is2,i3,s12c)+rx(i1,i2+is2,i3,0,1)*u(i1,i2+is2,i3,s22c))
                 s22tilde=det(i1,i2+is2,i3)*(rx(i1,i2+is2,i3,1,0)*u(i1,
     & i2+is2,i3,s12c)+rx(i1,i2+is2,i3,1,1)*u(i1,i2+is2,i3,s22c)) -2.*
     & dr(1)*is*(det(i1,i2,i3)*accel2 -(det(i1+1,i2,i3)*(rx(i1+1,i2,
     & i3,0,0)*u(i1+1,i2,i3,s12c)+rx(i1+1,i2,i3,0,1)*u(i1+1,i2,i3,
     & s22c)) -det(i1-1,i2,i3)*(rx(i1-1,i2,i3,0,0)*u(i1-1,i2,i3,s12c)+
     & rx(i1-1,i2,i3,0,1)*u(i1-1,i2,i3,s22c)))/(2.*dr(0)))
                 u(i1,i2-is2,i3,s12c)= rx(i1,i2-is2,i3,1,1)*s12tilde-
     & rx(i1,i2-is2,i3,0,1)*s22tilde
                 u(i1,i2-is2,i3,s22c)=-rx(i1,i2-is2,i3,1,0)*s12tilde+
     & rx(i1,i2-is2,i3,0,0)*s22tilde
                end if
                end do
                end do
              end if ! axis
              else ! choice
              if (bctype.eq.linearBoundaryCondition) then   ! linear case
                !   new
                if( axis.eq.0 )then
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   if (materialFormat.eq.constantMaterialProperties) 
     & then
                     accel1=rho*bcf(side,axis,i1,i2,i3,s11c)   ! rho times given acceleration
                     accel2=rho*bcf(side,axis,i1,i2,i3,s12c)
                   elseif (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                     accel1=rhopc(i1,i2)*bcf(side,axis,i1,i2,i3,s11c)   ! rho times given acceleration
                     accel2=rhopc(i1,i2)*bcf(side,axis,i1,i2,i3,s12c)
                   elseif (
     & materialFormat.eq.variableMaterialProperties) then
                     accel1=rhov(i1,i2)*bcf(side,axis,i1,i2,i3,s11c)   ! rho times given acceleration
                     accel2=rhov(i1,i2)*bcf(side,axis,i1,i2,i3,s12c)
                   else
                     write(6,*)'Error (bcOptSmFOS) : materialFormat 
     & not supported'
                     stop 4322
                   end if
                   deti=1.0/(rx(i1,i2,i3,0,0)**2+rx(i1,i2,i3,0,1)**2)**
     & 2
                   s11tilde=rx(i1,i2,i3,0,0)*u(i1+is1,i2,i3,s11c)+rx(
     & i1,i2,i3,0,1)*u(i1+is1,i2,i3,s21c) -2.*dr(0)*is*(accel1-(rx(i1,
     & i2,i3,1,0)*(u(i1,i2+1,i3,s11c)-u(i1,i2-1,i3,s11c)) +rx(i1,i2,
     & i3,1,1)*(u(i1,i2+1,i3,s21c)-u(i1,i2-1,i3,s21c)))/(2.*dr(1)))
                   s12tilde=rx(i1,i2,i3,0,0)*u(i1+is1,i2,i3,s12c)+rx(
     & i1,i2,i3,0,1)*u(i1+is1,i2,i3,s22c) -2.*dr(0)*is*(accel2-(rx(i1,
     & i2,i3,1,0)*(u(i1,i2+1,i3,s12c)-u(i1,i2-1,i3,s12c)) +rx(i1,i2,
     & i3,1,1)*(u(i1,i2+1,i3,s22c)-u(i1,i2-1,i3,s22c)))/(2.*dr(1)))
                   stautau=u(i1-is1,i2,i3,s11c)*rx(i1,i2,i3,0,1)**2-
     & 2.0*u(i1-is1,i2,i3,s12c)*rx(i1,i2,i3,0,1)*rx(i1,i2,i3,0,0) +u(
     & i1-is1,i2,i3,s22c)*rx(i1,i2,i3,0,0)**2
                   u(i1-is1,i2,i3,s11c)=deti*(s11tilde*rx(i1,i2,i3,0,0)
     & *(rx(i1,i2,i3,0,0)**2+2.0*rx(i1,i2,i3,0,1)**2) -s12tilde*rx(i1,
     & i2,i3,0,1)*rx(i1,i2,i3,0,0)**2+stautau*rx(i1,i2,i3,0,1)**2)
                   u(i1-is1,i2,i3,s12c)=deti*(s11tilde*rx(i1,i2,i3,0,1)
     & **3+s12tilde*rx(i1,i2,i3,0,0)**3 -stautau*rx(i1,i2,i3,0,0)*rx(
     & i1,i2,i3,0,1))
                   u(i1-is1,i2,i3,s21c)=u(i1-is1,i2,i3,s12c)
                   u(i1-is1,i2,i3,s22c)=deti*(s12tilde*rx(i1,i2,i3,0,1)
     & *(rx(i1,i2,i3,0,1)**2+2.0*rx(i1,i2,i3,0,0)**2) -s11tilde*rx(i1,
     & i2,i3,0,0)*rx(i1,i2,i3,0,1)**2+stautau*rx(i1,i2,i3,0,0)**2)
                  end if
                  end do
                  end do
                else ! axis .eq. 1
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   if (materialFormat.eq.constantMaterialProperties) 
     & then
                     accel1=rho*bcf(side,axis,i1,i2,i3,s11c)   ! rho times given acceleration
                     accel2=rho*bcf(side,axis,i1,i2,i3,s12c)
                   elseif (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                     accel1=rhopc(i1,i2)*bcf(side,axis,i1,i2,i3,s11c)   ! rho times given acceleration
                     accel2=rhopc(i1,i2)*bcf(side,axis,i1,i2,i3,s12c)
                   elseif (
     & materialFormat.eq.variableMaterialProperties) then
                     accel1=rhov(i1,i2)*bcf(side,axis,i1,i2,i3,s11c)   ! rho times given acceleration
                     accel2=rhov(i1,i2)*bcf(side,axis,i1,i2,i3,s12c)
                   else
                     write(6,*)'Error (bcOptSmFOS) : materialFormat 
     & not supported'
                     stop 4323
                   end if
                   deti=1.0/(rx(i1,i2,i3,1,0)**2+rx(i1,i2,i3,1,1)**2)**
     & 2
                   s11tilde=rx(i1,i2,i3,1,0)*u(i1,i2+is2,i3,s11c)+rx(
     & i1,i2,i3,1,1)*u(i1,i2+is2,i3,s21c) -2.*dr(1)*is*(accel1-(rx(i1,
     & i2,i3,0,0)*(u(i1+1,i2,i3,s11c)-u(i1-1,i2,i3,s11c)) +rx(i1,i2,
     & i3,0,1)*(u(i1+1,i2,i3,s21c)-u(i1-1,i2,i3,s21c)))/(2.*dr(0)))
                   s12tilde=rx(i1,i2,i3,1,0)*u(i1,i2+is2,i3,s12c)+rx(
     & i1,i2,i3,1,1)*u(i1,i2+is2,i3,s22c) -2.*dr(1)*is*(accel2-(rx(i1,
     & i2,i3,0,0)*(u(i1+1,i2,i3,s12c)-u(i1-1,i2,i3,s12c)) +rx(i1,i2,
     & i3,0,1)*(u(i1+1,i2,i3,s22c)-u(i1-1,i2,i3,s22c)))/(2.*dr(0)))
                   stautau=u(i1,i2-is2,i3,s11c)*rx(i1,i2,i3,1,1)**2-
     & 2.0*u(i1,i2-is2,i3,s12c)*rx(i1,i2,i3,1,1)*rx(i1,i2,i3,1,0) +u(
     & i1,i2-is2,i3,s22c)*rx(i1,i2,i3,1,0)**2
                   u(i1,i2-is2,i3,s11c)=deti*(s11tilde*rx(i1,i2,i3,1,0)
     & *(rx(i1,i2,i3,1,0)**2+2.0*rx(i1,i2,i3,1,1)**2) -s12tilde*rx(i1,
     & i2,i3,1,1)*rx(i1,i2,i3,1,0)**2+stautau*rx(i1,i2,i3,1,1)**2)
                   u(i1,i2-is2,i3,s12c)=deti*(s11tilde*rx(i1,i2,i3,1,1)
     & **3+s12tilde*rx(i1,i2,i3,1,0)**3 -stautau*rx(i1,i2,i3,1,0)*rx(
     & i1,i2,i3,1,1))
                   u(i1,i2-is2,i3,s21c)=u(i1,i2-is2,i3,s12c)
                   u(i1,i2-is2,i3,s22c)=deti*(s12tilde*rx(i1,i2,i3,1,1)
     & *(rx(i1,i2,i3,1,1)**2+2.0*rx(i1,i2,i3,1,0)**2) -s11tilde*rx(i1,
     & i2,i3,1,0)*rx(i1,i2,i3,1,1)**2+stautau*rx(i1,i2,i3,1,0)**2)
                  end if
                  end do
                  end do
                end if ! axis
              else   ! SVK case
                ! old for linear case, okay for SVK
                if( axis.eq.0 )then
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   accel1=rho*bcf(side,axis,i1,i2,i3,s11c)   ! rho times given acceleration
                   accel2=rho*bcf(side,axis,i1,i2,i3,s12c)
                   deti=1.0/(rx(i1,i2,i3,0,0)**2+rx(i1,i2,i3,0,1)**2)
                   s11tilde=rx(i1,i2,i3,0,0)*u(i1+is1,i2,i3,s11c)+rx(
     & i1,i2,i3,0,1)*u(i1+is1,i2,i3,s21c) -2.*dr(0)*is*(accel1-(rx(i1,
     & i2,i3,1,0)*(u(i1,i2+1,i3,s11c)-u(i1,i2-1,i3,s11c)) +rx(i1,i2,
     & i3,1,1)*(u(i1,i2+1,i3,s21c)-u(i1,i2-1,i3,s21c)))/(2.*dr(1)))
                   s21tilde=-rx(i1,i2,i3,0,1)*u(i1-is1,i2,i3,s11c)+rx(
     & i1,i2,i3,0,0)*u(i1-is1,i2,i3,s21c)
                   u(i1-is1,i2,i3,s11c)=deti*(rx(i1,i2,i3,0,0)*
     & s11tilde-rx(i1,i2,i3,0,1)*s21tilde)
                   u(i1-is1,i2,i3,s21c)=deti*(rx(i1,i2,i3,0,0)*
     & s21tilde+rx(i1,i2,i3,0,1)*s11tilde)
                   s12tilde=rx(i1,i2,i3,0,0)*u(i1+is1,i2,i3,s12c)+rx(
     & i1,i2,i3,0,1)*u(i1+is1,i2,i3,s22c) -2.*dr(0)*is*(accel2-(rx(i1,
     & i2,i3,1,0)*(u(i1,i2+1,i3,s12c)-u(i1,i2-1,i3,s12c)) +rx(i1,i2,
     & i3,1,1)*(u(i1,i2+1,i3,s22c)-u(i1,i2-1,i3,s22c)))/(2.*dr(1)))
                   s22tilde=-rx(i1,i2,i3,0,1)*u(i1-is1,i2,i3,s12c)+rx(
     & i1,i2,i3,0,0)*u(i1-is1,i2,i3,s22c)
                   u(i1-is1,i2,i3,s12c)=deti*(rx(i1,i2,i3,0,0)*
     & s12tilde-rx(i1,i2,i3,0,1)*s22tilde)
                   u(i1-is1,i2,i3,s22c)=deti*(rx(i1,i2,i3,0,0)*
     & s22tilde+rx(i1,i2,i3,0,1)*s12tilde)
                  end if
                  end do
                  end do
                else ! axis .eq. 1
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                   accel1=rho*bcf(side,axis,i1,i2,i3,s11c)   ! rho times given acceleration
                   accel2=rho*bcf(side,axis,i1,i2,i3,s12c)
                   deti=1.0/(rx(i1,i2,i3,1,0)**2+rx(i1,i2,i3,1,1)**2)
                   s11tilde=rx(i1,i2,i3,1,0)*u(i1,i2+is2,i3,s11c)+rx(
     & i1,i2,i3,1,1)*u(i1,i2+is2,i3,s21c) -2.*dr(1)*is*(accel1-(rx(i1,
     & i2,i3,0,0)*(u(i1+1,i2,i3,s11c)-u(i1-1,i2,i3,s11c)) +rx(i1,i2,
     & i3,0,1)*(u(i1+1,i2,i3,s21c)-u(i1-1,i2,i3,s21c)))/(2.*dr(0)))
                   s21tilde=-rx(i1,i2,i3,1,1)*u(i1,i2-is2,i3,s11c)+rx(
     & i1,i2,i3,1,0)*u(i1,i2-is2,i3,s21c)
                   u(i1,i2-is2,i3,s11c)=deti*(rx(i1,i2,i3,1,0)*
     & s11tilde-rx(i1,i2,i3,1,1)*s21tilde)
                   u(i1,i2-is2,i3,s21c)=deti*(rx(i1,i2,i3,1,0)*
     & s21tilde+rx(i1,i2,i3,1,1)*s11tilde)
                   s12tilde=rx(i1,i2,i3,1,0)*u(i1,i2+is2,i3,s12c)+rx(
     & i1,i2,i3,1,1)*u(i1,i2+is2,i3,s22c) -2.*dr(1)*is*(accel2-(rx(i1,
     & i2,i3,0,0)*(u(i1+1,i2,i3,s12c)-u(i1-1,i2,i3,s12c)) +rx(i1,i2,
     & i3,0,1)*(u(i1+1,i2,i3,s22c)-u(i1-1,i2,i3,s22c)))/(2.*dr(0)))
                   s22tilde=-rx(i1,i2,i3,1,1)*u(i1,i2-is2,i3,s12c)+rx(
     & i1,i2,i3,1,0)*u(i1,i2-is2,i3,s22c)
                   u(i1,i2-is2,i3,s12c)=deti*(rx(i1,i2,i3,1,0)*
     & s12tilde-rx(i1,i2,i3,1,1)*s22tilde)
                   u(i1,i2-is2,i3,s22c)=deti*(rx(i1,i2,i3,1,0)*
     & s22tilde+rx(i1,i2,i3,1,1)*s12tilde)
                  end if
                  end do
                  end do
                end if ! axis
              end if ! bctype
              end if ! choice
             end if ! end gridType
            else if( boundaryCondition(side,axis).eq.tractionBC )then
              ! **************** TRACTION : Neumann type conditions ******************
             if( applyInterfaceBoundaryConditions.eq.0 .and. 
     & interfaceType(side,axis,grid).eq.tractionInterface )then
              write(*,'("SMBC: skip traction BC2 on an interface, (
     & side,axis,grid)=(",3i3,")")') side,axis,grid
             else
             if( gridType.eq.rectangular )then
              ! ********* TRACTION : Cartesian Grid **********
              ! Assign displacements on the ghost points from given tractions on the boundary
              !   s11 = lambda ( u.x + v.y ) + 2 mu u.x
              !   s12 = s21 = mu ( u.y + v.x )
              !   s22 = lambda ( u.x + v.y ) + 2 mu v.y
              ! an1*s11 + an2*s12 = f1
              ! an1*s21 + an2*s22 = f2
              ! Assign velocities on the ghost points from given time derivatives of the tractions on the boundary
               if (bctype.eq.linearBoundaryCondition) then             
     &                   ! linear case
                 if( axis.eq.0 )then
                   i3=n3a
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                    f1=bcf(side,axis,i1,i2,i3,uc)               ! given traction forces
                    f2=bcf(side,axis,i1,i2,i3,vc)
                    fdot1=bcf(side,axis,i1,i2,i3,v1c)           ! rate of change of traction forces
                    fdot2=bcf(side,axis,i1,i2,i3,v2c)
                    if (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                      mu=mupc(i1,i2)
                      lambda=lambdapc(i1,i2)
                    elseif (
     & materialFormat.eq.variableMaterialProperties) then
                      mu=muv(i1,i2)
                      lambda=lambdav(i1,i2)
                    end if
                    u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)+2.*dx(0)*(f1+
     & is*lambda*(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.*dx(1)))/(
     & lambda+2.*mu)
                    u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)+2.*dx(0)*(f2+
     & is*mu*(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.*dx(1)))/mu
                    u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)+2.*dx(0)*(
     & fdot1+is*lambda*(u(i1,i2+1,i3,v2c)-u(i1,i2-1,i3,v2c))/(2.*dx(1)
     & ))/(lambda+2.*mu)
                    u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)+2.*dx(0)*(
     & fdot2+is*mu*(u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,v1c))/(2.*dx(1)))
     & /mu
                   end if
                   end do
                   end do
                 else
                   i3=n3a
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                    f1=bcf(side,axis,i1,i2,i3,uc)              ! given traction forces
                    f2=bcf(side,axis,i1,i2,i3,vc)
                    fdot1=bcf(side,axis,i1,i2,i3,v1c)           ! rate of change of traction forces
                    fdot2=bcf(side,axis,i1,i2,i3,v2c)
                    if (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                      mu=mupc(i1,i2)
                      lambda=lambdapc(i1,i2)
                    elseif (
     & materialFormat.eq.variableMaterialProperties) then
                      mu=muv(i1,i2)
                      lambda=lambdav(i1,i2)
                    end if
                    u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)+2.*dx(1)*(f1+
     & is*mu*(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.*dx(0)))/mu
                    u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)+2.*dx(1)*(f2+
     & is*lambda*(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.*dx(0)))/(
     & lambda+2.*mu)
                    u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)+2.*dx(1)*(
     & fdot1+is*mu*(u(i1+1,i2,i3,v2c)-u(i1-1,i2,i3,v2c))/(2.*dx(0)))
     & /mu
                    u(i1,i2-is2,i3,v2c)=u(i1,i2+is2,i3,v2c)+2.*dx(1)*(
     & fdot2+is*lambda*(u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,v1c))/(2.*dx(0)
     & ))/(lambda+2.*mu)
                   end if
                   end do
                   end do
                 end if
               else                                               ! SVK case
                 if( axis.eq.0 )then
                   i3=n3a
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                     ! initialize
                     ! u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(0))
                     ! u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(0))
                     u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(1)
     & )
                     u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(1)
     & )
                     alpha=sqrt(u1y**2+(1.0+u2y)**2)
                     v1y=(u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,v1c))/(2.*dx(
     & 1))
                     v2y=(u(i1,i2+1,i3,v2c)-u(i1,i2-1,i3,v2c))/(2.*dx(
     & 1))
                     dalpha=(u1y*v1y+(1.0+u2y)*v2y)/alpha
                     f1=-is*bcf(side,axis,i1,i2,i3,uc)*alpha         ! given traction forces (adjust here for sign of normal)
                     f2=-is*bcf(side,axis,i1,i2,i3,vc)*alpha
                     fdot1=-is*(bcf(side,axis,i1,i2,i3,v1c)*alpha+bcf(
     & side,axis,i1,i2,i3,uc)*dalpha)
                     fdot2=-is*(bcf(side,axis,i1,i2,i3,v2c)*alpha+bcf(
     & side,axis,i1,i2,i3,vc)*dalpha)
                     u1x0=is*(u(i1+is1,i2,i3,uc)-u(i1,i2,i3,uc))/dx(0)
                     u2x0=is*(u(i1+is1,i2,i3,vc)-u(i1,i2,i3,vc))/dx(0)
                     u1x=u1x0
                     u2x=u2x0
                     ! Newton iteration for u1x,u2x
                     iter=1
                     istop=0
                     bmax=10.*toler
                     do while (bmax.gt.toler)
                       ! compute stress and the deriv based on current deformation gradient
                       !  ideriv=1
                       !  call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                       du(1,1)=u1x
                       du(1,2)=u1y
                       du(2,1)=u2x
                       du(2,2)=u2y
                       ideriv=1
                       call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                       ! solve the 2x2 system
                       b1=p(1,1)-f1
                       b2=p(1,2)-f2
                       determ=dpdf(1,1)*dpdf(2,3)-dpdf(1,3)*dpdf(2,1)
                       du1x=(b1*dpdf(2,3)-b2*dpdf(1,3))/determ
                       du2x=(b2*dpdf(1,1)-b1*dpdf(2,1))/determ
                       ! compute max residual of the stress condition and update
                       bmax=max(abs(b1),abs(b2))/lambda
                       if (istop.ne.0) then
                         write(6,'(1x,i2,3(1x,1pe15.8))')iter,du1x,
     & du2x,bmax
                       end if
                       u1x=u1x-du1x
                       u2x=u2x-du2x
                       iter=iter+1
                       ! check for convergence
                       if (iter.gt.itmax) then
                         write(6,*)'Error (bcOptSmFOS) : Newton failed 
     & to converge'
                         if (istop.eq.0) then
                           iter=1
                           istop=1
                           u1x=u1x0
                           u2x=u2x0
                         else
                           stop 8883
                         end if
                       end if
                     end do
                     ! set displacement in the ghost point
                     u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is*dx(0)*
     & u1x
                     u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is*dx(0)*
     & u2x
                     ! solve a 2x2 system for (v1x,v2x) and set velocity in the ghost point
                     b1=fdot1-dpdf(1,2)*v1y-dpdf(1,4)*v2y
                     b2=fdot2-dpdf(2,2)*v1y-dpdf(2,4)*v2y
                     v1x=(b1*dpdf(2,3)-b2*dpdf(1,3))/determ
                     v2x=(b2*dpdf(1,1)-b1*dpdf(2,1))/determ
                     u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)-2.*is*dx(
     & 0)*v1x
                     u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)-2.*is*dx(
     & 0)*v2x
                   end if
                   end do
                   end do
                 else
                   i3=n3a
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                     ! initialize
                     u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(0)
     & )
                     u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(0)
     & )
                     !  u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(1))
                     !  u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(1))
                     alpha=sqrt((1.0+u1x)**2+u2x**2)
                     v1x=(u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,v1c))/(2.*dx(
     & 0))
                     v2x=(u(i1+1,i2,i3,v2c)-u(i1-1,i2,i3,v2c))/(2.*dx(
     & 0))
                     dalpha=((1.0+u1x)*v1x+u2x*v2x)/alpha
                     f1=-is*bcf(side,axis,i1,i2,i3,uc)*alpha         ! given traction forces (adjust here for sign of normal)
                     f2=-is*bcf(side,axis,i1,i2,i3,vc)*alpha
                     fdot1=-is*(bcf(side,axis,i1,i2,i3,v1c)*alpha+bcf(
     & side,axis,i1,i2,i3,uc)*dalpha)
                     fdot2=-is*(bcf(side,axis,i1,i2,i3,v2c)*alpha+bcf(
     & side,axis,i1,i2,i3,vc)*dalpha)
                     u1y0=is*(u(i1,i2+is2,i3,uc)-u(i1,i2,i3,uc))/dx(1)
                     u2y0=is*(u(i1,i2+is2,i3,vc)-u(i1,i2,i3,vc))/dx(1)
                     u1y=u1y0
                     u2y=u2y0
                     ! Newton iteration for u1y,u2y
                     iter=1
                     istop=0
                     bmax=10.*toler
                     do while (bmax.gt.toler)
                       ! compute stress and the deriv based on current deformation gradient
                       ! ideriv=1
                       ! call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                       du(1,1)=u1x
                       du(1,2)=u1y
                       du(2,1)=u2x
                       du(2,2)=u2y
                       ideriv=1
                       call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                       ! solve the 2x2 system
                       b1=p(2,1)-f1
                       b2=p(2,2)-f2
                       determ=dpdf(3,2)*dpdf(4,4)-dpdf(3,4)*dpdf(4,2)
                       du1y=(b1*dpdf(4,4)-b2*dpdf(3,4))/determ
                       du2y=(b2*dpdf(3,2)-b1*dpdf(4,2))/determ
                       ! compute max residual of the stress condition and update
                       bmax=max(abs(b1),abs(b2))/lambda
                       if (istop.ne.0) then
                         write(6,'(1x,i2,3(1x,1pe15.8))')iter,du1y,
     & du2y,bmax
                       end if
                       u1y=u1y-du1y
                       u2y=u2y-du2y
                       iter=iter+1
                       ! check for convergence
                       if (iter.gt.itmax) then
                         write(6,*)'Error (bcOptSmFOS) : Newton failed 
     & to converge'
                         if (istop.eq.0) then
                           iter=1
                           istop=1
                           u1y=u1y0
                           u2y=u2y0
                         else
                           stop 8884
                         end if
                       end if
                     end do
                     ! set displacement in the ghost point
                     u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is*dx(1)*
     & u1y
                     u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is*dx(1)*
     & u2y
                     ! solve a 2x2 system for (v1y,v2y) and set velocity in the ghost point
                     b1=fdot1-dpdf(3,1)*v1x-dpdf(3,3)*v2x
                     b2=fdot2-dpdf(4,1)*v1x-dpdf(4,3)*v2x
                     v1y=(b1*dpdf(4,4)-b2*dpdf(3,4))/determ
                     v2y=(b2*dpdf(3,2)-b1*dpdf(4,2))/determ
                     u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)-2.*is*dx(
     & 1)*v1y
                     u(i1,i2-is2,i3,v2c)=u(i1,i2+is2,i3,v2c)-2.*is*dx(
     & 1)*v2y
                   end if
                   end do
                   end do
                 end if
               end if
             else
              ! *********** TRACTION : Curvilinear Grid ****************
               if (bctype.eq.linearBoundaryCondition) then             
     &                   ! linear case
                 if( axis.eq.0 )then
                   i3=n3a
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                    ! given traction forces (adjust here for sign of normal)
                    f1=-is*bcf(side,axis,i1,i2,i3,uc)
                    f2=-is*bcf(side,axis,i1,i2,i3,vc)
                    ! rate of change of traction forces:
                    fdot1=-is*bcf(side,axis,i1,i2,i3,v1c)
                    fdot2=-is*bcf(side,axis,i1,i2,i3,v2c)
                    if (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                      mu=mupc(i1,i2)
                      lambda=lambdapc(i1,i2)
                    elseif (
     & materialFormat.eq.variableMaterialProperties) then
                      mu=muv(i1,i2)
                      lambda=lambdav(i1,i2)
                    end if
                    rad=sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,i2,i3,axis,1)
     & **2)
                    a(0,0,0)=(lambda+2.*mu)*rx(i1,i2,i3,0,0)**2+mu*rx(
     & i1,i2,i3,0,1)**2
                    a(0,1,0)=(lambda+mu)*rx(i1,i2,i3,0,0)*rx(i1,i2,i3,
     & 0,1)
                    a(1,0,0)=a(0,1,0)
                    a(1,1,0)=mu*rx(i1,i2,i3,0,0)**2+(lambda+2.*mu)*rx(
     & i1,i2,i3,0,1)**2
                    adet=a(0,0,0)*a(1,1,0)-a(0,1,0)*a(1,0,0)
                    a(0,0,1)=(lambda+2.*mu)*rx(i1,i2,i3,0,0)*rx(i1,i2,
     & i3,1,0)+mu*rx(i1,i2,i3,0,1)*rx(i1,i2,i3,1,1)
                    a(0,1,1)=lambda*rx(i1,i2,i3,0,0)*rx(i1,i2,i3,1,1)+
     & mu*rx(i1,i2,i3,0,1)*rx(i1,i2,i3,1,0)
                    a(1,0,1)=lambda*rx(i1,i2,i3,0,1)*rx(i1,i2,i3,1,0)+
     & mu*rx(i1,i2,i3,0,0)*rx(i1,i2,i3,1,1)
                    a(1,1,1)=mu*rx(i1,i2,i3,0,0)*rx(i1,i2,i3,1,0)+(
     & lambda+2.*mu)*rx(i1,i2,i3,0,1)*rx(i1,i2,i3,1,1)
                    u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is*dr(0)*(
     & rad*( a(1,1,0)*f1-a(0,1,0)*f2) -( a(1,1,0)*a(0,0,1)-a(0,1,0)*a(
     & 1,0,1))*(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.*dr(1)) -( a(1,
     & 1,0)*a(0,1,1)-a(0,1,0)*a(1,1,1))*(u(i1,i2+1,i3,vc)-u(i1,i2-1,
     & i3,vc))/(2.*dr(1)))/adet
                    u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is*dr(0)*(
     & rad*(-a(1,0,0)*f1+a(0,0,0)*f2) -(-a(1,0,0)*a(0,0,1)+a(0,0,0)*a(
     & 1,0,1))*(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.*dr(1)) -(-a(1,
     & 0,0)*a(0,1,1)+a(0,0,0)*a(1,1,1))*(u(i1,i2+1,i3,vc)-u(i1,i2-1,
     & i3,vc))/(2.*dr(1)))/adet
                    u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)-2.*is*dr(0)
     & *(rad*( a(1,1,0)*fdot1-a(0,1,0)*fdot2) -( a(1,1,0)*a(0,0,1)-a(
     & 0,1,0)*a(1,0,1))*(u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,v1c))/(2.*dr(
     & 1)) -( a(1,1,0)*a(0,1,1)-a(0,1,0)*a(1,1,1))*(u(i1,i2+1,i3,v2c)-
     & u(i1,i2-1,i3,v2c))/(2.*dr(1)))/adet
                    u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)-2.*is*dr(0)
     & *(rad*(-a(1,0,0)*fdot1+a(0,0,0)*fdot2) -(-a(1,0,0)*a(0,0,1)+a(
     & 0,0,0)*a(1,0,1))*(u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,v1c))/(2.*dr(
     & 1)) -(-a(1,0,0)*a(0,1,1)+a(0,0,0)*a(1,1,1))*(u(i1,i2+1,i3,v2c)-
     & u(i1,i2-1,i3,v2c))/(2.*dr(1)))/adet
                   end if
                   end do
                   end do
                 else ! axis .eq. 1
                   i3=n3a
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                    ! given traction forces
                    f1=-is*bcf(side,axis,i1,i2,i3,uc)
                    f2=-is*bcf(side,axis,i1,i2,i3,vc)
                    ! rate of change of traction forces
                    fdot1=-is*bcf(side,axis,i1,i2,i3,v1c)
                    fdot2=-is*bcf(side,axis,i1,i2,i3,v2c)
                    if (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                      mu=mupc(i1,i2)
                      lambda=lambdapc(i1,i2)
                    elseif (
     & materialFormat.eq.variableMaterialProperties) then
                      mu=muv(i1,i2)
                      lambda=lambdav(i1,i2)
                    end if
                    rad=sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,i2,i3,axis,1)
     & **2)
                    a(0,0,0)=(lambda+2.*mu)*rx(i1,i2,i3,1,0)**2+mu*rx(
     & i1,i2,i3,1,1)**2
                    a(0,1,0)=(lambda+mu)*rx(i1,i2,i3,1,0)*rx(i1,i2,i3,
     & 1,1)
                    a(1,0,0)=a(0,1,0)
                    a(1,1,0)=mu*rx(i1,i2,i3,1,0)**2+(lambda+2.*mu)*rx(
     & i1,i2,i3,1,1)**2
                    adet=a(0,0,0)*a(1,1,0)-a(0,1,0)*a(1,0,0)
                    a(0,0,1)=(lambda+2.*mu)*rx(i1,i2,i3,1,0)*rx(i1,i2,
     & i3,0,0)+mu*rx(i1,i2,i3,1,1)*rx(i1,i2,i3,0,1)
                    a(0,1,1)=lambda*rx(i1,i2,i3,1,0)*rx(i1,i2,i3,0,1)+
     & mu*rx(i1,i2,i3,1,1)*rx(i1,i2,i3,0,0)
                    a(1,0,1)=lambda*rx(i1,i2,i3,1,1)*rx(i1,i2,i3,0,0)+
     & mu*rx(i1,i2,i3,1,0)*rx(i1,i2,i3,0,1)
                    a(1,1,1)=mu*rx(i1,i2,i3,1,0)*rx(i1,i2,i3,0,0)+(
     & lambda+2.*mu)*rx(i1,i2,i3,1,1)*rx(i1,i2,i3,0,1)
                    u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is*dr(1)*(
     & rad*( a(1,1,0)*f1-a(0,1,0)*f2) -( a(1,1,0)*a(0,0,1)-a(0,1,0)*a(
     & 1,0,1))*(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.*dr(0)) -( a(1,
     & 1,0)*a(0,1,1)-a(0,1,0)*a(1,1,1))*(u(i1+1,i2,i3,vc)-u(i1-1,i2,
     & i3,vc))/(2.*dr(0)))/adet
                    u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is*dr(1)*(
     & rad*(-a(1,0,0)*f1+a(0,0,0)*f2) -(-a(1,0,0)*a(0,0,1)+a(0,0,0)*a(
     & 1,0,1))*(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.*dr(0)) -(-a(1,
     & 0,0)*a(0,1,1)+a(0,0,0)*a(1,1,1))*(u(i1+1,i2,i3,vc)-u(i1-1,i2,
     & i3,vc))/(2.*dr(0)))/adet
                    u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)-2.*is*dr(1)
     & *(rad*( a(1,1,0)*fdot1-a(0,1,0)*fdot2) -( a(1,1,0)*a(0,0,1)-a(
     & 0,1,0)*a(1,0,1))*(u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,v1c))/(2.*dr(
     & 0)) -( a(1,1,0)*a(0,1,1)-a(0,1,0)*a(1,1,1))*(u(i1+1,i2,i3,v2c)-
     & u(i1-1,i2,i3,v2c))/(2.*dr(0)))/adet
                    u(i1,i2-is2,i3,v2c)=u(i1,i2+is2,i3,v2c)-2.*is*dr(1)
     & *(rad*(-a(1,0,0)*fdot1+a(0,0,0)*fdot2) -(-a(1,0,0)*a(0,0,1)+a(
     & 0,0,0)*a(1,0,1))*(u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,v1c))/(2.*dr(
     & 0)) -(-a(1,0,0)*a(0,1,1)+a(0,0,0)*a(1,1,1))*(u(i1+1,i2,i3,v2c)-
     & u(i1-1,i2,i3,v2c))/(2.*dr(0)))/adet
                   end if
                   end do
                   end do
                 end if ! axis
               else          ! ---- SVK case ----
                 if( axis.eq.0 )then
                   i3=n3a
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                     ! initialize
                     aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2))
                     an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                     an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                     !  u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(0))
                     !  u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(0))
                     u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(1)
     & )
                     u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(1)
     & )
                     coef1=rx(i1,i2,i3,0,1)-u1s/det(i1,i2,i3)
                     coef2=rx(i1,i2,i3,0,0)+u2s/det(i1,i2,i3)
                     alpha=sqrt(coef1**2+coef2**2)*aNormi
                     v1s=(u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,v1c))/(2.0*dr(
     & 1))
                     v2s=(u(i1,i2+1,i3,v2c)-u(i1,i2-1,i3,v2c))/(2.0*dr(
     & 1))
                     dalpha=(-coef1*v1s+coef2*v2s)*aNormi**2/(det(i1,
     & i2,i3)*alpha)
                     f1=bcf(side,axis,i1,i2,i3,uc)*alpha        ! given traction forces
                     f2=bcf(side,axis,i1,i2,i3,vc)*alpha
                     fdot1=bcf(side,axis,i1,i2,i3,v1c)*alpha+bcf(side,
     & axis,i1,i2,i3,uc)*dalpha
                     fdot2=bcf(side,axis,i1,i2,i3,v2c)*alpha+bcf(side,
     & axis,i1,i2,i3,vc)*dalpha
                     u1r0=is*(u(i1+is1,i2,i3,uc)-u(i1,i2,i3,uc))/dr(0)
                     u2r0=is*(u(i1+is1,i2,i3,vc)-u(i1,i2,i3,vc))/dr(0)
                     u1r=u1r0
                     u2r=u2r0
                     ! Newton iteration for u1r,u2r
                     iter=1
                     istop=0
                     bmax=10.*toler
                     do while (bmax.gt.toler)
                       u1x=rx(i1,i2,i3,0,0)*u1r+rx(i1,i2,i3,1,0)*u1s
                       u1y=rx(i1,i2,i3,0,1)*u1r+rx(i1,i2,i3,1,1)*u1s
                       u2x=rx(i1,i2,i3,0,0)*u2r+rx(i1,i2,i3,1,0)*u2s
                       u2y=rx(i1,i2,i3,0,1)*u2r+rx(i1,i2,i3,1,1)*u2s
                       ! compute stress and the deriv based on current deformation gradient
                       !  ideriv=1
                       !  call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                       du(1,1)=u1x
                       du(1,2)=u1y
                       du(2,1)=u2x
                       du(2,2)=u2y
                       ideriv=1
                       call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                       !  construct linear system
                       b1=an1*p(1,1)+an2*p(2,1)-f1
                       b2=an1*p(1,2)+an2*p(2,2)-f2
                       a11= an1*(dpdf(1,1)*rx(i1,i2,i3,0,0)+dpdf(1,2)*
     & rx(i1,i2,i3,0,1)) +an2*(dpdf(3,1)*rx(i1,i2,i3,0,0)+dpdf(3,2)*
     & rx(i1,i2,i3,0,1))
                       a12= an1*(dpdf(1,3)*rx(i1,i2,i3,0,0)+dpdf(1,4)*
     & rx(i1,i2,i3,0,1)) +an2*(dpdf(3,3)*rx(i1,i2,i3,0,0)+dpdf(3,4)*
     & rx(i1,i2,i3,0,1))
                       a21= an1*(dpdf(2,1)*rx(i1,i2,i3,0,0)+dpdf(2,2)*
     & rx(i1,i2,i3,0,1)) +an2*(dpdf(4,1)*rx(i1,i2,i3,0,0)+dpdf(4,2)*
     & rx(i1,i2,i3,0,1))
                       a22= an1*(dpdf(2,3)*rx(i1,i2,i3,0,0)+dpdf(2,4)*
     & rx(i1,i2,i3,0,1)) +an2*(dpdf(4,3)*rx(i1,i2,i3,0,0)+dpdf(4,4)*
     & rx(i1,i2,i3,0,1))
                       ! solve the 2x2 system
                       determ=a11*a22-a12*a21
                       du1r=(b1*a22-b2*a12)/determ
                       du2r=(b2*a11-b1*a21)/determ
                       ! compute max residual of the stress condition and update
                       bmax=max(abs(b1),abs(b2))/lambda
                       if (istop.ne.0) then
                         write(6,'(1x,i2,3(1x,1pe15.8))')iter,du1r,
     & du2r,bmax
                       end if
                       ! debugging stuff
                       ! if (i2.eq.n2a.or.i2.eq.n2b) then
                       !  write(6,*)'i2, iter, bmax =',i2,iter,bmax
                       ! end if
                       u1r=u1r-du1r
                       u2r=u2r-du2r
                       iter=iter+1
                       ! check for convergence
                       if (iter.gt.itmax) then
                         write(6,*)'Error (bcOptSmFOS) : Newton failed 
     & to converge'
                         if (istop.eq.0) then
                           iter=1
                           istop=1
                           u1r=u1r0
                           u2r=u2r0
                         else
                           stop 8885
                         end if
                       end if
                     end do
                     ! set displacement in the ghost point
                     u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is*dr(0)*
     & u1r
                     u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is*dr(0)*
     & u2r
                     ! solve a 2x2 system for (v1r,v2r) and set velocity in the ghost point
                     b1=fdot1-((an1*dpdf(1,1)+an2*dpdf(3,1))*rx(i1,i2,
     & i3,1,0)+(an1*dpdf(1,2)+an2*dpdf(3,2))*rx(i1,i2,i3,1,1))*v1s -((
     & an1*dpdf(1,3)+an2*dpdf(3,3))*rx(i1,i2,i3,1,0)+(an1*dpdf(1,4)+
     & an2*dpdf(3,4))*rx(i1,i2,i3,1,1))*v2s
                     b2=fdot2-((an1*dpdf(2,1)+an2*dpdf(4,1))*rx(i1,i2,
     & i3,1,0)+(an1*dpdf(2,2)+an2*dpdf(4,2))*rx(i1,i2,i3,1,1))*v1s -((
     & an1*dpdf(2,3)+an2*dpdf(4,3))*rx(i1,i2,i3,1,0)+(an1*dpdf(2,4)+
     & an2*dpdf(4,4))*rx(i1,i2,i3,1,1))*v2s
                     v1r=(b1*a22-b2*a12)/determ
                     v2r=(b2*a11-b1*a21)/determ
                     u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)-2.*is*dr(
     & 0)*v1r
                     u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)-2.*is*dr(
     & 0)*v2r
                       !c debugging stuff
                       !c          v1r=(u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,v1c))/(2.0*dr(0))
                       !c          v1s=(u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,v1c))/(2.0*dr(1))
                       !c          v2r=(u(i1+1,i2,i3,v2c)-u(i1-1,i2,i3,v2c))/(2.0*dr(0))
                       !c          v2s=(u(i1,i2+1,i3,v2c)-u(i1,i2-1,i3,v2c))/(2.0*dr(1))
                       !c          v1x=v1r*rx(i1,i2,i3,0,0)+v1s*rx(i1,i2,i3,1,0)
                       !c          v1y=v1r*rx(i1,i2,i3,0,1)+v1s*rx(i1,i2,i3,1,1)
                       !c          v2x=v2r*rx(i1,i2,i3,0,0)+v2s*rx(i1,i2,i3,1,0)
                       !c          v2y=v2r*rx(i1,i2,i3,0,1)+v2s*rx(i1,i2,i3,1,1)
                       !c          a11=dpdf(1,1)*v1x+dpdf(1,2)*v1y+dpdf(1,3)*v2x+dpdf(1,4)*v2y
                       !c          a12=dpdf(2,1)*v1x+dpdf(2,2)*v1y+dpdf(2,3)*v2x+dpdf(2,4)*v2y
                       !c          a21=dpdf(3,1)*v1x+dpdf(3,2)*v1y+dpdf(3,3)*v2x+dpdf(3,4)*v2y
                       !c          a22=dpdf(4,1)*v1x+dpdf(4,2)*v1y+dpdf(4,3)*v2x+dpdf(4,4)*v2y
                       !c          b1=an1*a11+an2*a21-fdot1
                       !c          b2=an1*a12+an2*a22-fdot2
                       !c          write(6,*)i1,b1,b2
                   end if
                   end do
                   end do
                   !c debugging stuff
                   !c            pause
                 else ! axis .eq. 1
                   i3=n3a
                   do i2=n2a,n2b
                   do i1=n1a,n1b
                   if( mask(i1,i2,i3).gt.0 )then
                     ! initialize
                     aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2))
                     an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                     an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                     u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(0)
     & )
                     u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(0)
     & )
                     !  u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(1))
                     !  u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(1))
                     coef1=rx(i1,i2,i3,1,1)+u1r/det(i1,i2,i3)
                     coef2=rx(i1,i2,i3,1,0)-u2r/det(i1,i2,i3)
                     alpha=sqrt(coef1**2+coef2**2)*aNormi
                     v1r=(u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,v1c))/(2.0*dr(
     & 0))
                     v2r=(u(i1+1,i2,i3,v2c)-u(i1-1,i2,i3,v2c))/(2.0*dr(
     & 0))
                     dalpha=(coef1*v1r-coef2*v2r)*aNormi**2/(det(i1,i2,
     & i3)*alpha)
                     f1=bcf(side,axis,i1,i2,i3,uc)*alpha         ! given traction forces
                     f2=bcf(side,axis,i1,i2,i3,vc)*alpha
                     fdot1=bcf(side,axis,i1,i2,i3,v1c)*alpha+bcf(side,
     & axis,i1,i2,i3,uc)*dalpha
                     fdot2=bcf(side,axis,i1,i2,i3,v2c)*alpha+bcf(side,
     & axis,i1,i2,i3,vc)*dalpha
                     u1s0=is*(u(i1,i2+is2,i3,uc)-u(i1,i2,i3,uc))/dr(1)
                     u2s0=is*(u(i1,i2+is2,i3,vc)-u(i1,i2,i3,vc))/dr(1)
                     u1s=u1s0
                     u2s=u2s0
                     ! Newton iteration for u1s,u2s
                     iter=1
                     istop=0
                     bmax=10.*toler
                     do while (bmax.gt.toler)
                       u1x=rx(i1,i2,i3,0,0)*u1r+rx(i1,i2,i3,1,0)*u1s
                       u1y=rx(i1,i2,i3,0,1)*u1r+rx(i1,i2,i3,1,1)*u1s
                       u2x=rx(i1,i2,i3,0,0)*u2r+rx(i1,i2,i3,1,0)*u2s
                       u2y=rx(i1,i2,i3,0,1)*u2r+rx(i1,i2,i3,1,1)*u2s
                       ! compute stress and the deriv based on current deformation gradient
                       !  ideriv=1
                       !  call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                       du(1,1)=u1x
                       du(1,2)=u1y
                       du(2,1)=u2x
                       du(2,2)=u2y
                       ideriv=1
                       call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                       ! construct linear system
                       b1=an1*p(1,1)+an2*p(2,1)-f1
                       b2=an1*p(1,2)+an2*p(2,2)-f2
                       a11= an1*(dpdf(1,1)*rx(i1,i2,i3,1,0)+dpdf(1,2)*
     & rx(i1,i2,i3,1,1)) +an2*(dpdf(3,1)*rx(i1,i2,i3,1,0)+dpdf(3,2)*
     & rx(i1,i2,i3,1,1))
                       a12= an1*(dpdf(1,3)*rx(i1,i2,i3,1,0)+dpdf(1,4)*
     & rx(i1,i2,i3,1,1)) +an2*(dpdf(3,3)*rx(i1,i2,i3,1,0)+dpdf(3,4)*
     & rx(i1,i2,i3,1,1))
                       a21= an1*(dpdf(2,1)*rx(i1,i2,i3,1,0)+dpdf(2,2)*
     & rx(i1,i2,i3,1,1)) +an2*(dpdf(4,1)*rx(i1,i2,i3,1,0)+dpdf(4,2)*
     & rx(i1,i2,i3,1,1))
                       a22= an1*(dpdf(2,3)*rx(i1,i2,i3,1,0)+dpdf(2,4)*
     & rx(i1,i2,i3,1,1)) +an2*(dpdf(4,3)*rx(i1,i2,i3,1,0)+dpdf(4,4)*
     & rx(i1,i2,i3,1,1))
                       ! solve the 2x2 system
                       determ=a11*a22-a12*a21
                       du1s=(b1*a22-b2*a12)/determ
                       du2s=(b2*a11-b1*a21)/determ
                       ! compute max residual of the stress condition and update
                       bmax=max(abs(b1),abs(b2))/lambda
                       if (istop.ne.0) then
                         write(6,'(1x,i2,3(1x,1pe15.8))')iter,du1s,
     & du2s,bmax
                       end if
                       !c debugging stuff
                       !c                if (i1.eq.n1a.or.i1.eq.n1b) then
                       !c                  write(6,*)'i1, iter, bmax =',i1,iter,bmax
                       !c                end if
                       u1s=u1s-du1s
                       u2s=u2s-du2s
                       iter=iter+1
                       if (iter.gt.itmax) then
                         write(6,*)'Error (bcOptSmFOS) : Newton failed 
     & to converge'
                         if (istop.eq.0) then
                           iter=1
                           istop=1
                           u1s=u1s0
                           u2s=u2s0
                         else
                           stop 8886
                         end if
                       end if
                     end do
                     ! set displacement in the ghost point
                     u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is*dr(1)*
     & u1s
                     u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is*dr(1)*
     & u2s
                     ! solve a 2x2 system for (v1s,v2s) and set velocity in the ghost point
                     b1=fdot1-((an1*dpdf(1,1)+an2*dpdf(3,1))*rx(i1,i2,
     & i3,0,0)+(an1*dpdf(1,2)+an2*dpdf(3,2))*rx(i1,i2,i3,0,1))*v1r -((
     & an1*dpdf(1,3)+an2*dpdf(3,3))*rx(i1,i2,i3,0,0)+(an1*dpdf(1,4)+
     & an2*dpdf(3,4))*rx(i1,i2,i3,0,1))*v2r
                     b2=fdot2-((an1*dpdf(2,1)+an2*dpdf(4,1))*rx(i1,i2,
     & i3,0,0)+(an1*dpdf(2,2)+an2*dpdf(4,2))*rx(i1,i2,i3,0,1))*v1r -((
     & an1*dpdf(2,3)+an2*dpdf(4,3))*rx(i1,i2,i3,0,0)+(an1*dpdf(2,4)+
     & an2*dpdf(4,4))*rx(i1,i2,i3,0,1))*v2r
                     !c debugging stuff
                     !c               if (i1.eq.n1a) then
                     !c                 write(6,*)a11,a12,a21,a22
                     !c                 write(6,*)b1,b2
                     !c               end if
                     v1s=(b1*a22-b2*a12)/determ
                     v2s=(b2*a11-b1*a21)/determ
                     u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)-2.*is*dr(
     & 1)*v1s
                     u(i1,i2-is2,i3,v2c)=u(i1,i2+is2,i3,v2c)-2.*is*dr(
     & 1)*v2s
                     !c debugging stuff
                     !c               if (i1.eq.n1a) then
                     !c                 write(6,*)u(i1,i2-is2,i3,v1c),u(i1,i2+is2,i3,v1c),v1s
                     !c                 write(6,*)u(i1,i2-is2,i3,v2c),u(i1,i2+is2,i3,v2c),v2s
                     !c               end if
                     !c debugging stuff
                     !c          v1r=(u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,v1c))/(2.0*dr(0))
                     !c          v1s=(u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,v1c))/(2.0*dr(1))
                     !c          v2r=(u(i1+1,i2,i3,v2c)-u(i1-1,i2,i3,v2c))/(2.0*dr(0))
                     !c          v2s=(u(i1,i2+1,i3,v2c)-u(i1,i2-1,i3,v2c))/(2.0*dr(1))
                     !c          v1x=v1r*rx(i1,i2,i3,0,0)+v1s*rx(i1,i2,i3,1,0)
                     !c          v1y=v1r*rx(i1,i2,i3,0,1)+v1s*rx(i1,i2,i3,1,1)
                     !c          v2x=v2r*rx(i1,i2,i3,0,0)+v2s*rx(i1,i2,i3,1,0)
                     !c          v2y=v2r*rx(i1,i2,i3,0,1)+v2s*rx(i1,i2,i3,1,1)
                     !c          a11=dpdf(1,1)*v1x+dpdf(1,2)*v1y+dpdf(1,3)*v2x+dpdf(1,4)*v2y
                     !c          a12=dpdf(2,1)*v1x+dpdf(2,2)*v1y+dpdf(2,3)*v2x+dpdf(2,4)*v2y
                     !c          a21=dpdf(3,1)*v1x+dpdf(3,2)*v1y+dpdf(3,3)*v2x+dpdf(3,4)*v2y
                     !c          a22=dpdf(4,1)*v1x+dpdf(4,2)*v1y+dpdf(4,3)*v2x+dpdf(4,4)*v2y
                     !c          b1=rx(i1,i2,i3,1,0)*a11+rx(i1,i2,i3,1,1)*a21
                     !c          b2=rx(i1,i2,i3,1,0)*a12+rx(i1,i2,i3,1,1)*a22
                     !c          write(6,*)i1,b1,b2
                   end if
                   end do
                   end do
                 end if ! axis
               end if
             end if ! end gridType
             end if ! interfaceType
            else if( boundaryCondition(side,axis).eq.slipWall )then
              ! **************** SLIPWALL : Neumann type conditions ******************
             if( gridType.eq.rectangular )then
              ! ********* SLIPWALL : Cartesian Grid **********
              if (materialFormat.eq.constantMaterialProperties) then
                if( axis.eq.0 )then
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1-is1,i2,i3,vc )=u(i1+is1,i2,i3,vc )-2.*is*dx(0)
     & *( bcf(side,axis,i1,i2,i3,s21c)/mu-(u(i1,i2+1,i3,uc )-u(i1,i2-
     & 1,i3,uc ))/(2.*dx(1)))
                    u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)-2.*is*dx(0)
     & *( bcf(side,axis,i1,i2,i3,s22c)/mu-(u(i1,i2+1,i3,v1c)-u(i1,i2-
     & 1,i3,v1c))/(2.*dx(1)))
                  end if
                  end do
                  end do
                else
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2-is2,i3,uc )=u(i1,i2+is2,i3,uc )-2.*is*dx(1)
     & *(-bcf(side,axis,i1,i2,i3,s21c)/mu-(u(i1+1,i2,i3,vc )-u(i1-1,
     & i2,i3,vc ))/(2.*dx(0)))
                    u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)-2.*is*dx(1)
     & *(-bcf(side,axis,i1,i2,i3,s22c)/mu-(u(i1+1,i2,i3,v2c)-u(i1-1,
     & i2,i3,v2c))/(2.*dx(0)))
                  end if
                  end do
                  end do
                end if
              elseif (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                if( axis.eq.0 )then
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1-is1,i2,i3,vc )=u(i1+is1,i2,i3,vc )-2.*is*dx(0)
     & *( bcf(side,axis,i1,i2,i3,s21c)/mupc(i1,i2)-(u(i1,i2+1,i3,uc )-
     & u(i1,i2-1,i3,uc ))/(2.*dx(1)))
                    u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)-2.*is*dx(0)
     & *( bcf(side,axis,i1,i2,i3,s22c)/mupc(i1,i2)-(u(i1,i2+1,i3,v1c)-
     & u(i1,i2-1,i3,v1c))/(2.*dx(1)))
                  end if
                  end do
                  end do
                else
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2-is2,i3,uc )=u(i1,i2+is2,i3,uc )-2.*is*dx(1)
     & *(-bcf(side,axis,i1,i2,i3,s21c)/mupc(i1,i2)-(u(i1+1,i2,i3,vc )-
     & u(i1-1,i2,i3,vc ))/(2.*dx(0)))
                    u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)-2.*is*dx(1)
     & *(-bcf(side,axis,i1,i2,i3,s22c)/mupc(i1,i2)-(u(i1+1,i2,i3,v2c)-
     & u(i1-1,i2,i3,v2c))/(2.*dx(0)))
                  end if
                  end do
                  end do
                end if
              elseif (materialFormat.eq.variableMaterialProperties) 
     & then
                if( axis.eq.0 )then
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1-is1,i2,i3,vc )=u(i1+is1,i2,i3,vc )-2.*is*dx(0)
     & *( bcf(side,axis,i1,i2,i3,s21c)/muv(i1,i2)-(u(i1,i2+1,i3,uc )-
     & u(i1,i2-1,i3,uc ))/(2.*dx(1)))
                    u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)-2.*is*dx(0)
     & *( bcf(side,axis,i1,i2,i3,s22c)/muv(i1,i2)-(u(i1,i2+1,i3,v1c)-
     & u(i1,i2-1,i3,v1c))/(2.*dx(1)))
                  end if
                  end do
                  end do
                else
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                    u(i1,i2-is2,i3,uc )=u(i1,i2+is2,i3,uc )-2.*is*dx(1)
     & *(-bcf(side,axis,i1,i2,i3,s21c)/muv(i1,i2)-(u(i1+1,i2,i3,vc )-
     & u(i1-1,i2,i3,vc ))/(2.*dx(0)))
                    u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)-2.*is*dx(1)
     & *(-bcf(side,axis,i1,i2,i3,s22c)/muv(i1,i2)-(u(i1+1,i2,i3,v2c)-
     & u(i1-1,i2,i3,v2c))/(2.*dx(0)))
                  end if
                  end do
                  end do
                end if
              else
                write(6,*)'Error (bcOptSmFOS) : materialFormat not 
     & supported'
                stop 4948
              end if
             else
              ! *********** SLIPWALL : Curvilinear Grid ****************
              ! (an1,an2) = outward normal and (-an2,an1) = unit tangent
              !   aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,i2,i3,axis,1)**2))
              !   an1=-is*rx(i1,i2,i3,axis,0)*aNormi
              !   an2=-is*rx(i1,i2,i3,axis,1)*aNormi
              if( axis.eq.0 )then
                i3=n3a
                do i2=n2a,n2b
                do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  if (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                    mu=mupc(i1,i2)
                  elseif (materialFormat.eq.variableMaterialProperties)
     &  then
                    mu=muv(i1,i2)
                  end if
                  b1=2.*rx(i1,i2,i3,axis,0)*rx(i1,i2,i3,axis,1)
                  b2=rx(i1,i2,i3,axis,0)**2-rx(i1,i2,i3,axis,1)**2
                  b3=1./(rx(i1,i2,i3,axis,0)**2+rx(i1,i2,i3,axis,1)**2)
                  f1=bcf(side,axis,i1,i2,i3,s21c)/mu-b3*((-b1*rx(i1,i2,
     & i3,1,0)+b2*rx(i1,i2,i3,1,1))*(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc)
     & ) +( b1*rx(i1,i2,i3,1,1)+b2*rx(i1,i2,i3,1,0))*(u(i1,i2+1,i3,vc)
     & -u(i1,i2-1,i3,vc)))/(2.*dr(1))
                  f1=-rx(i1,i2,i3,axis,1)*u(i1+is1,i2,i3,uc)+rx(i1,i2,
     & i3,axis,0)*u(i1+is1,i2,i3,vc)-2.*is*dr(0)*f1
                  f2= rx(i1,i2,i3,axis,0)*u(i1-is1,i2,i3,uc)+rx(i1,i2,
     & i3,axis,1)*u(i1-is1,i2,i3,vc)
                  u(i1-is1,i2,i3,uc)=b3*(-rx(i1,i2,i3,axis,1)*f1+rx(i1,
     & i2,i3,axis,0)*f2)
                  u(i1-is1,i2,i3,vc)=b3*( rx(i1,i2,i3,axis,0)*f1+rx(i1,
     & i2,i3,axis,1)*f2)
                  f1=bcf(side,axis,i1,i2,i3,s22c)/mu-b3*((-b1*rx(i1,i2,
     & i3,1,0)+b2*rx(i1,i2,i3,1,1))*(u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,
     & v1c)) +( b1*rx(i1,i2,i3,1,1)+b2*rx(i1,i2,i3,1,0))*(u(i1,i2+1,
     & i3,v2c)-u(i1,i2-1,i3,v2c)))/(2.*dr(1))
                  f1=-rx(i1,i2,i3,axis,1)*u(i1+is1,i2,i3,v1c)+rx(i1,i2,
     & i3,axis,0)*u(i1+is1,i2,i3,v2c)-2.*is*dr(0)*f1
                  f2= rx(i1,i2,i3,axis,0)*u(i1-is1,i2,i3,v1c)+rx(i1,i2,
     & i3,axis,1)*u(i1-is1,i2,i3,v2c)
                  u(i1-is1,i2,i3,v1c)=b3*(-rx(i1,i2,i3,axis,1)*f1+rx(
     & i1,i2,i3,axis,0)*f2)
                  u(i1-is1,i2,i3,v2c)=b3*( rx(i1,i2,i3,axis,0)*f1+rx(
     & i1,i2,i3,axis,1)*f2)
                end if
                end do
                end do
              else ! axis .eq. 1
                i3=n3a
                do i2=n2a,n2b
                do i1=n1a,n1b
                if( mask(i1,i2,i3).gt.0 )then
                  if (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                    mu=mupc(i1,i2)
                  elseif (materialFormat.eq.variableMaterialProperties)
     &  then
                    mu=muv(i1,i2)
                  end if
                  b1=2.*rx(i1,i2,i3,axis,0)*rx(i1,i2,i3,axis,1)
                  b2=rx(i1,i2,i3,axis,0)**2-rx(i1,i2,i3,axis,1)**2
                  b3=1./(rx(i1,i2,i3,axis,0)**2+rx(i1,i2,i3,axis,1)**2)
                  f1=bcf(side,axis,i1,i2,i3,s21c)/mu-b3*((-b1*rx(i1,i2,
     & i3,0,0)+b2*rx(i1,i2,i3,0,1))*(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc)
     & ) +( b1*rx(i1,i2,i3,0,1)+b2*rx(i1,i2,i3,0,0))*(u(i1+1,i2,i3,vc)
     & -u(i1-1,i2,i3,vc)))/(2.*dr(0))
                  f1=-rx(i1,i2,i3,axis,1)*u(i1,i2+is2,i3,uc)+rx(i1,i2,
     & i3,axis,0)*u(i1,i2+is2,i3,vc)-2.*is*dr(1)*f1
                  f2= rx(i1,i2,i3,axis,0)*u(i1,i2-is2,i3,uc)+rx(i1,i2,
     & i3,axis,1)*u(i1,i2-is2,i3,vc)
                  u(i1,i2-is2,i3,uc)=b3*(-rx(i1,i2,i3,axis,1)*f1+rx(i1,
     & i2,i3,axis,0)*f2)
                  u(i1,i2-is2,i3,vc)=b3*( rx(i1,i2,i3,axis,0)*f1+rx(i1,
     & i2,i3,axis,1)*f2)
                  f1=bcf(side,axis,i1,i2,i3,s22c)/mu-b3*((-b1*rx(i1,i2,
     & i3,0,0)+b2*rx(i1,i2,i3,0,1))*(u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,
     & v1c)) +( b1*rx(i1,i2,i3,0,1)+b2*rx(i1,i2,i3,0,0))*(u(i1+1,i2,
     & i3,v2c)-u(i1-1,i2,i3,v2c)))/(2.*dr(0))
                  f1=-rx(i1,i2,i3,axis,1)*u(i1,i2+is2,i3,v1c)+rx(i1,i2,
     & i3,axis,0)*u(i1,i2+is2,i3,v2c)-2.*is*dr(1)*f1
                  f2= rx(i1,i2,i3,axis,0)*u(i1,i2-is2,i3,v1c)+rx(i1,i2,
     & i3,axis,1)*u(i1,i2-is2,i3,v2c)
                  u(i1,i2-is2,i3,v1c)=b3*(-rx(i1,i2,i3,axis,1)*f1+rx(
     & i1,i2,i3,axis,0)*f2)
                  u(i1,i2-is2,i3,v2c)=b3*( rx(i1,i2,i3,axis,0)*f1+rx(
     & i1,i2,i3,axis,1)*f2)
                end if
                end do
                end do
              end if ! axis
             end if ! end gridType
            end if ! bc
            end do ! end side
            end do ! end axis
          if( debug.gt.32 )then
           n1a=gridIndexRange(0,0)
           n1b=gridIndexRange(1,0)
           n2a=gridIndexRange(0,1)
           n2b=gridIndexRange(1,1)
           n3a=gridIndexRange(0,2)
           n3b=gridIndexRange(1,2)
            write(*,'("v1c",1x,"AftersecondaryNeumann")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,v1c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,v1c),i1=n1a-2,n1b+
     & 2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("v2c",1x,"AftersecondaryNeumann")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,v2c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,v2c),i1=n1a-2,n1b+
     & 2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("s11c",1x,"AftersecondaryNeumann")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s11c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s11c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("s12c",1x,"AftersecondaryNeumann")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s12c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s12c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("s22c",1x,"AftersecondaryNeumann")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s22c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s22c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
          end if
         !  n1a=gridIndexRange(0,0)
         !  n1b=gridIndexRange(1,0)
         !  n2a=gridIndexRange(0,1)
         !  n2b=gridIndexRange(1,1)
         !  i3=gridIndexRange(0,2)
         !  do i1=n1a-1,n1b+1
         !  do i2=n2a-1,n2b+1
         !    write(33,"(2(1x,i3),6(1x,1pe9.2))")i1,i2,u(i1,i2,i3,uc),u(i1,i2,i3,vc),u(i1,i2,i3,s11c),u(i1,i2,i3,s12c),u(i1,i2,i3,s21c),u(i1,i2,i3,s22c)
         !  end do
         !  end do
         !  pause
         ! return after applying secondary bcs for debugging
         if (.false.) return
          !*******
          !******* Secondary Dirichlet conditions for the tangential components of stress (tractionBC only) ********
          !*******
         if (assignTangentStress) then
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
                if( applyInterfaceBoundaryConditions.eq.0 .and. 
     & interfaceType(side,axis,grid).eq.tractionInterface )then
                 write(*,'("SMBC: skip traction BC3 on an interface, (
     & side,axis,grid)=(",3i3,")")') side,axis,grid
                else
                 if( gridType.eq.rectangular )then
                   if (bctype.eq.linearBoundaryCondition) then      ! linear case
                     ! new
                     if (materialFormat.eq.constantMaterialProperties) 
     & then
                       if( axis.eq.0 )then
                         i3=n3a
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                           u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(
     & 2.0*dx(0))
                           u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(
     & 2.0*dx(1))
                           u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(
     & 2.0*dx(0))
                           u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(
     & 2.0*dx(1))
                           u(i1,i2,i3,s22c)=lambda*(u1x+u2y)+2.0*mu*u2y
                         end if
                         end do
                         end do
                       else
                         i3=n3a
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                           u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(
     & 2.0*dx(0))
                           u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(
     & 2.0*dx(1))
                           u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(
     & 2.0*dx(0))
                           u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(
     & 2.0*dx(1))
                           u(i1,i2,i3,s11c)=lambda*(u1x+u2y)+2.0*mu*u1x
                         end if
                         end do
                         end do
                       end if
                     elseif (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                       if( axis.eq.0 )then
                         i3=n3a
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                           u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(
     & 2.0*dx(0))
                           u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(
     & 2.0*dx(1))
                           u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(
     & 2.0*dx(0))
                           u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(
     & 2.0*dx(1))
                           u(i1,i2,i3,s22c)=lambdapc(i1,i2)*(u1x+u2y)+
     & 2.0*mupc(i1,i2)*u2y
                         end if
                         end do
                         end do
                       else
                         i3=n3a
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                           u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(
     & 2.0*dx(0))
                           u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(
     & 2.0*dx(1))
                           u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(
     & 2.0*dx(0))
                           u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(
     & 2.0*dx(1))
                           u(i1,i2,i3,s11c)=lambdapc(i1,i2)*(u1x+u2y)+
     & 2.0*mupc(i1,i2)*u1x
                         end if
                         end do
                         end do
                       end if
                     elseif (
     & materialFormat.eq.variableMaterialProperties) then
                       if( axis.eq.0 )then
                         i3=n3a
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                           u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(
     & 2.0*dx(0))
                           u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(
     & 2.0*dx(1))
                           u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(
     & 2.0*dx(0))
                           u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(
     & 2.0*dx(1))
                           u(i1,i2,i3,s22c)=lambdav(i1,i2)*(u1x+u2y)+
     & 2.0*muv(i1,i2)*u2y
                         end if
                         end do
                         end do
                       else
                         i3=n3a
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                           u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(
     & 2.0*dx(0))
                           u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(
     & 2.0*dx(1))
                           u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(
     & 2.0*dx(0))
                           u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(
     & 2.0*dx(1))
                           u(i1,i2,i3,s11c)=lambdav(i1,i2)*(u1x+u2y)+
     & 2.0*muv(i1,i2)*u1x
                         end if
                         end do
                         end do
                       end if
                     else
                       write(6,*)'Error (bcOptSmFOS) : materialFormat 
     & not supported'
                       stop 4949
                     end if
                   else  ! ---- SVK case -------
                     if( axis.eq.0 )then
                       i3=n3a
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                         u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*
     & dx(0))
                         u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*
     & dx(1))
                         u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*
     & dx(0))
                         u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*
     & dx(1))
                         ! ideriv=0
                         ! call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                         du(1,1)=u1x
                         du(1,2)=u1y
                         du(2,1)=u2x
                         du(2,2)=u2y
                         ideriv=0
                         call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                         u(i1,i2,i3,s21c)=p(2,1)
                         u(i1,i2,i3,s22c)=p(2,2)
                       end if
                       end do
                       end do
                     else
                       i3=n3a
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                         u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*
     & dx(0))
                         u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*
     & dx(1))
                         u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*
     & dx(0))
                         u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*
     & dx(1))
                         ! ideriv=0
                         ! call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                         du(1,1)=u1x
                         du(1,2)=u1y
                         du(2,1)=u2x
                         du(2,2)=u2y
                         ideriv=0
                         call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                         u(i1,i2,i3,s11c)=p(1,1)
                         u(i1,i2,i3,s12c)=p(1,2)
                       end if
                       end do
                       end do
                     end if
                   end if
                 else  ! curvilinear
                   if (bctype.eq.linearBoundaryCondition) then         
     &                       ! linear case
                    ! new
                     i3=n3a
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                     if( mask(i1,i2,i3).gt.0 )then
                       if (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                         mu=mupc(i1,i2)
                         lambda=lambdapc(i1,i2)
                       elseif (
     & materialFormat.eq.variableMaterialProperties) then
                         mu=muv(i1,i2)
                         lambda=lambdav(i1,i2)
                       end if
                       u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(
     & 0))
                       u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(
     & 1))
                       u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(
     & 0))
                       u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(
     & 1))
                       u1x=u1r*rx(i1,i2,i3,0,0)+u1s*rx(i1,i2,i3,1,0)
                       u1y=u1r*rx(i1,i2,i3,0,1)+u1s*rx(i1,i2,i3,1,1)
                       u2x=u2r*rx(i1,i2,i3,0,0)+u2s*rx(i1,i2,i3,1,0)
                       u2y=u2r*rx(i1,i2,i3,0,1)+u2s*rx(i1,i2,i3,1,1)
                       s11t=lambda*(u1x+u2y)+2.0*mu*u1x
                       s12t=mu*(u1y+u2x)
                       s21t=s12t
                       s22t=lambda*(u1x+u2y)+2.0*mu*u2y
                       ! (an1,an2) = outward normal and (-an2,an1) = unit tangent
                       aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+
     & rx(i1,i2,i3,axis,1)**2))
                       an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                       an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                       b1=-an2*(-an2*(s11t-u(i1,i2,i3,s11c))+an1*(s12t-
     & u(i1,i2,i3,s12c))) +an1*(-an2*(s21t-u(i1,i2,i3,s21c))+an1*(
     & s22t-u(i1,i2,i3,s22c)))
                       u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+an2*b1*an2
                       u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)-an2*b1*an1
                       u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)-an1*b1*an2
                       u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+an1*b1*an1
                     end if
                     end do
                     end do
                   else ! --- SVK case ---
                     i3=n3a
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                     if( mask(i1,i2,i3).gt.0 )then
                       u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(
     & 0))
                       u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(
     & 1))
                       u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(
     & 0))
                       u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(
     & 1))
                       u1x=u1r*rx(i1,i2,i3,0,0)+u1s*rx(i1,i2,i3,1,0)
                       u1y=u1r*rx(i1,i2,i3,0,1)+u1s*rx(i1,i2,i3,1,1)
                       u2x=u2r*rx(i1,i2,i3,0,0)+u2s*rx(i1,i2,i3,1,0)
                       u2y=u2r*rx(i1,i2,i3,0,1)+u2s*rx(i1,i2,i3,1,1)
                       ! ideriv=0
                       ! call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                       du(1,1)=u1x
                       du(1,2)=u1y
                       du(2,1)=u2x
                       du(2,2)=u2y
                       ideriv=0
                       call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                       ! Should be able to just set all four components of stress here.  The idea is that the deformation gradient
                       ! has been set so that the stress agrees with the specified traction.  Here we just set the other two
                       ! component to be compatible.  All together this agrees with setting all four components.   DWS, 2/28/12
                       u(i1,i2,i3,s11c)=p(1,1)
                       u(i1,i2,i3,s12c)=p(1,2)
                       u(i1,i2,i3,s21c)=p(2,1)
                       u(i1,i2,i3,s22c)=p(2,2)
                     end if
                     end do
                     end do
                   end if
                 end if  ! end gridType
                end if ! interfaceType
               end if ! bc
              end do ! end side
              end do ! end axis
             ! set tangential components of stress on the boundary  (TZ forcing, if necessary)
             if (twilightZone.ne.0) then
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
                     if (bctype.eq.linearBoundaryCondition) then       
     &                         ! linear case
                       if( axis.eq.0 )then
                         i3=n3a
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,uc,u1xe)
                          call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,vc,u2xe)
                          call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,uc,u1ye)
                          call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,vc,u2ye)
                          call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s21c,s21e)
                          call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s22c,s22e)
                          if (
     & materialFormat.ne.constantMaterialProperties) then
                            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,muc,mu)
                            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,lambdac,lambda)
                          end if
                           ! old
                           ! u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+s21e-mu*(u1ye+u2xe)
                          u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+s22e-
     & lambda*(u1xe+u2ye)-2.0*mu*u2ye
                         end if
                         end do
                         end do
                       else
                         i3=n3a
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,uc,u1xe)
                          call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,vc,u2xe)
                          call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,uc,u1ye)
                          call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,vc,u2ye)
                          call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s11c,s11e)
                          call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s12c,s12e)
                          if (
     & materialFormat.ne.constantMaterialProperties) then
                            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,muc,mu)
                            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,lambdac,lambda)
                          end if
                          u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+s11e-
     & lambda*(u1xe+u2ye)-2.0*mu*u1xe
                          ! old
                          ! u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+s12e-mu*(u1ye+u2xe)
                         end if
                         end do
                         end do
                       end if
                     else                                               ! SVK case
                       if( axis.eq.0 )then
                         i3=n3a
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,uc,u1xe)
                          call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,vc,u2xe)
                          call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,uc,u1ye)
                          call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,vc,u2ye)
                          call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s21c,s21e)
                          call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s22c,s22e)
                          !  ideriv=0
                          ! call smbcsdp (u1xe,u1ye,u2xe,u2ye,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1xe
                          du(1,2)=u1ye
                          du(2,1)=u2xe
                          du(2,2)=u2ye
                          ideriv=0
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+s21e-p(2,1)
                          u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+s22e-p(2,2)
                         end if
                         end do
                         end do
                       else
                         i3=n3a
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                         if( mask(i1,i2,i3).gt.0 )then
                          call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,uc,u1xe)
                          call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,vc,u2xe)
                          call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,uc,u1ye)
                          call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,vc,u2ye)
                          call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s11c,s11e)
                          call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s12c,s12e)
                          ! ideriv=0
                          ! call smbcsdp (u1xe,u1ye,u2xe,u2ye,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1xe
                          du(1,2)=u1ye
                          du(2,1)=u2xe
                          du(2,2)=u2ye
                          ideriv=0
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+s11e-p(1,1)
                          u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+s12e-p(1,2)
                         end if
                         end do
                         end do
                       end if
                     end if
                   else  ! curvilinear
                     if (bctype.eq.linearBoundaryCondition) then       
     &                         ! linear case
                       i3=n3a
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,uc,u1xe)
                        call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,vc,u2xe)
                        call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,uc,u1ye)
                        call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,vc,u2ye)
                        call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s11c,s11e)
                        call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s12c,s12e)
                        call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s21c,s21e)
                        call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s22c,s22e)
                        if (
     & materialFormat.ne.constantMaterialProperties) then
                          call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,muc,mu)
                          call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,lambdac,lambda)
                        end if
                        ! (an1,an2) = outward normal => (an2,-an1) = tangent vector
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+
     & rx(i1,i2,i3,axis,1)**2))
                        an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                        an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                        b1=-an2*(-an2*(s11e-lambda*(u1xe+u2ye)-2.0*mu*
     & u1xe)+an1*(s12e-mu*(u1ye+u2xe))) +an1*(-an2*(s21e-mu*(u1ye+
     & u2xe))+an1*(s22e-lambda*(u1xe+u2ye)-2.0*mu*u2ye))
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+an2*b1*an2
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)-an2*b1*an1
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)-an1*b1*an2
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+an1*b1*an1
                        !c old
                        !c                   b1=an2*(s11e-lambda*(u1xe+u2ye)-2.0*mu*u1xe)-an1*(s21e-mu*(u1ye+u2xe))
                        !c                   b2=an2*(s12e-mu*(u1ye+u2xe))-an1*(s22e-lambda*(u1xe+u2ye)-2.0*mu*u2ye)
                        !c
                        !c                   u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+an2*b1
                        !c                   u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+an2*b2
                        !c                   u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)-an1*b1
                        !c                   u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)-an1*b2
                       end if
                       end do
                       end do
                     else                                               ! SVK case
                       i3=n3a
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                       if( mask(i1,i2,i3).gt.0 )then
                        call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,uc,u1xe)
                        call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,vc,u2xe)
                        call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,uc,u1ye)
                        call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,vc,u2ye)
                        call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s11c,s11e)
                        call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s12c,s12e)
                        call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s21c,s21e)
                        call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s22c,s22e)
                        !c                   ideriv=0
                        !c                   call smbcsdp (u1xe,u1ye,u2xe,u2ye,lambda,mu,p,dpdf,ideriv)
                        du(1,1)=u1xe
                        du(1,2)=u1ye
                        du(2,1)=u2xe
                        du(2,2)=u2ye
                        ideriv=0
                        call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                         ! new (DWS, 2/28/12)
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+s11e-p(1,1)
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+s12e-p(1,2)
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+s21e-p(2,1)
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+s22e-p(2,2)
                          !c old (DWS, 2/28/12)
                          !c                   ! (an1,an2) = outward normal => (an2,-an1) = tangent vector
                          !c                   aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,i2,i3,axis,1)**2))
                          !c                   an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                          !c                   an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                          !c                   b1=an2*(s11e-p(1,1))-an1*(s21e-p(2,1))
                          !c                   b2=an2*(s12e-p(1,2))-an1*(s22e-p(2,2))
                          !c
                          !c                   u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+an2*b1
                          !                     !c                   u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+an2*b2
                          !c
                          !c                   u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)-an1*b1
                          !c                   u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)-an1*b2
                       end if
                       end do
                       end do
                          !c             pause
                     end if
                   end if  ! end gridType
                 end if ! bc
                end do ! end side
                end do ! end axis
             end if
             !
             ! fix up TZ stress at traction-traction corners (SVK case only at the moment).  The fix is needed because the loops above
             ! apply the TZ forcing correction twice at traction-traction corners.  (The fix is probably only needed for the 
             ! curvilinear grid case.)
             !
             if (twilightZone.ne.0) then
               axis1=0
               axis2=1
               i3=gridIndexRange(0,2)
               if (bctype.ne.0) then            ! SVK case
                 do side1=0,1
                   i1=gridIndexRange(side1,axis1)
                   do side2=0,1
                     i2=gridIndexRange(side2,axis2)
                     if (mask(i1,i2,i3).ne.0) then
                       if (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC) then
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s11c,s11e)
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s12c,s12e)
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s21c,s21e)
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s22c,s22e)
                         u(i1,i2,i3,s11c)=s11e
                         u(i1,i2,i3,s12c)=s12e
                         u(i1,i2,i3,s21c)=s21e
                         u(i1,i2,i3,s22c)=s22e
                       end if
                     end if
                   end do
                 end do
               end if
             end if
         end if
         ! return after applying bcs for tangential components of stress for debugging
         if (.false.) return
         !*******
         !******* Secondary Dirichlet conditions for stress (slipWall only) ********
         !*******
           ! TZ forcing (if necessary)
           if (twilightZone.ne.0) then
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
               if( boundaryCondition(side,axis).eq.slipWall )then
                 if( gridType.eq.rectangular )then
                   if( axis.eq.0 )then
                      i3=n3a
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if (mask(i1,i2,i3).ne.0) then
                         call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,uc,u1xe)
                         call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,vc,u2ye)
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s11c,s11e)
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s12c,s12e)
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s21c,s21e)
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s22c,s22e)
                         if (
     & materialFormat.ne.constantMaterialProperties) then
                           call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,muc,mu)
                           call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,lambdac,lambda)
                         end if
                         bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,
     & axis)+(dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,
     & side,axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,
     & 2,side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c)))
     & ))=s11e-lambda*(u1xe+u2ye)-2.0*mu*u1xe
                         bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,
     & axis)+(dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,
     & side,axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,
     & 2,side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s21c)))
     & ))=s21e-s12e
                         bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,
     & axis)+(dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,
     & side,axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,
     & 2,side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s22c)))
     & ))=s22e-lambda*(u1xe+u2ye)-2.0*mu*u2ye
                       end if
                      end do
                      end do
                   else
                      i3=n3a
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                       if (mask(i1,i2,i3).ne.0) then
                         call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,uc,u1xe)
                         call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,vc,u2ye)
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s11c,s11e)
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s12c,s12e)
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s21c,s21e)
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s22c,s22e)
                         if (
     & materialFormat.ne.constantMaterialProperties) then
                           call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,muc,mu)
                           call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,lambdac,lambda)
                         end if
                         bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,
     & axis)+(dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,
     & side,axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,
     & 2,side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c)))
     & ))=s11e-lambda*(u1xe+u2ye)-2.0*mu*u1xe
                         bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,
     & axis)+(dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,
     & side,axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,
     & 2,side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c)))
     & ))=s12e-s21e
                         bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,
     & axis)+(dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,
     & side,axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,
     & 2,side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s22c)))
     & ))=s22e-lambda*(u1xe+u2ye)-2.0*mu*u2ye
                       end if
                      end do
                      end do
                   end if
                 else  ! curvilinear
                    i3=n3a
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                     if (mask(i1,i2,i3).ne.0) then
                       call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,uc,u1xe)
                       call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,vc,u2xe)
                       call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,uc,u1ye)
                       call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,vc,u2ye)
                       call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s11c,s11e)
                       call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s12c,s12e)
                       call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s21c,s21e)
                       call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,s22c,s22e)
                       if (
     & materialFormat.ne.constantMaterialProperties) then
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,muc,mu)
                         call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.0,t,lambdac,lambda)
                       end if
                       s11t=lambda*(u1xe+u2ye)+2.0*mu*u1xe
                       s12t=mu*(u1ye+u2xe)
                       s21t=s12t
                       s22t=lambda*(u1xe+u2ye)+2.0*mu*u2ye
                       ! (an1,an2) = outward normal and (-an2,an1) = unit tangent
                       aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+
     & rx(i1,i2,i3,axis,1)**2))
                       an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                       an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                       b1=an1*(-s11t*an2+s12t*an1)+an2*(-s21t*an2+s22t*
     & an1)
                       bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+
     & (dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,
     & axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,
     & side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(vc)))))
     & =b1
                       bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+
     & (dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,
     & axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,
     & side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c)))))
     & =s11e-s11t
                       bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+
     & (dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,
     & axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,
     & side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c)))))
     & =s12e-s12t
                       bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+
     & (dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,
     & axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,
     & side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s21c)))))
     & =s21e-s21t
                       bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+
     & (dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,
     & axis)+(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,
     & side,axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s22c)))))
     & =s22e-s22t
                     end if
                    end do
                    end do
                 end if  ! end gridType
               end if ! bc
              end do ! end side
              end do ! end axis
           else
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
               if( boundaryCondition(side,axis).eq.slipWall )then
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                   if (mask(i1,i2,i3).ne.0) then
                     ! the true bc for stress is moved for convenience applying TZ flow
                     bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(vc)))))=bcf0(
     & bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(dim(1,0,side,axis)-
     & dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)+(dim(1,1,side,
     & axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,axis)+(dim(1,2,
     & side,axis)-dim(0,2,side,axis)+1)*(s11c)))))
                     bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s11c)))))=0.
                     bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s12c)))))=0.
                     bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s21c)))))=0.
                     bcf0(bcOffset(side,axis)+(i1-dim(0,0,side,axis)+(
     & dim(1,0,side,axis)-dim(0,0,side,axis)+1)*(i2-dim(0,1,side,axis)
     & +(dim(1,1,side,axis)-dim(0,1,side,axis)+1)*(i3-dim(0,2,side,
     & axis)+(dim(1,2,side,axis)-dim(0,2,side,axis)+1)*(s22c)))))=0.
                   end if
                  end do
                  end do
               end if ! bc
              end do ! end side
              end do ! end axis
           end if
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
             if( boundaryCondition(side,axis).eq.slipWall )then
               if( gridType.eq.rectangular )then
                 ! new
                 if (materialFormat.eq.constantMaterialProperties) then
                   if( axis.eq.0 )then
                      i3=n3a
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                       u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(
     & 0))
                       u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(
     & 1))
                       u(i1,i2,i3,s11c)=lambda*(u1x+u2y)+2.0*mu*u1x
                       u(i1,i2,i3,s22c)=lambda*(u1x+u2y)+2.0*mu*u2y
                      end if
                      end do
                      end do
                   else
                      i3=n3a
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                       u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(
     & 0))
                       u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(
     & 1))
                       u(i1,i2,i3,s11c)=lambda*(u1x+u2y)+2.0*mu*u1x
                       u(i1,i2,i3,s22c)=lambda*(u1x+u2y)+2.0*mu*u2y
                      end if
                      end do
                      end do
                   end if
                 elseif (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                   if( axis.eq.0 )then
                      i3=n3a
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                       u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(
     & 0))
                       u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(
     & 1))
                       u(i1,i2,i3,s11c)=lambdapc(i1,i2)*(u1x+u2y)+2.0*
     & mupc(i1,i2)*u1x
                       u(i1,i2,i3,s22c)=lambdapc(i1,i2)*(u1x+u2y)+2.0*
     & mupc(i1,i2)*u2y
                      end if
                      end do
                      end do
                   else
                      i3=n3a
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                       u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(
     & 0))
                       u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(
     & 1))
                       u(i1,i2,i3,s11c)=lambdapc(i1,i2)*(u1x+u2y)+2.0*
     & mupc(i1,i2)*u1x
                       u(i1,i2,i3,s22c)=lambdapc(i1,i2)*(u1x+u2y)+2.0*
     & mupc(i1,i2)*u2y
                      end if
                      end do
                      end do
                   end if
                 elseif (materialFormat.eq.variableMaterialProperties) 
     & then
                   if( axis.eq.0 )then
                      i3=n3a
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                       u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(
     & 0))
                       u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(
     & 1))
                       u(i1,i2,i3,s11c)=lambdav(i1,i2)*(u1x+u2y)+2.0*
     & muv(i1,i2)*u1x
                       u(i1,i2,i3,s22c)=lambdav(i1,i2)*(u1x+u2y)+2.0*
     & muv(i1,i2)*u2y
                      end if
                      end do
                      end do
                   else
                      i3=n3a
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                       u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(
     & 0))
                       u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(
     & 1))
                       u(i1,i2,i3,s11c)=lambdav(i1,i2)*(u1x+u2y)+2.0*
     & muv(i1,i2)*u1x
                       u(i1,i2,i3,s22c)=lambdav(i1,i2)*(u1x+u2y)+2.0*
     & muv(i1,i2)*u2y
                      end if
                      end do
                      end do
                   end if
                 else
                   write(6,*)'Error (bcOptSmFOS) : materialFormat not 
     & supported'
                   stop 4956
                 end if
               else  ! curvilinear
                 !  new
                  i3=n3a
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                  if( mask(i1,i2,i3).gt.0 )then
                     u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(0)
     & )
                     u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(1)
     & )
                     u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(0)
     & )
                     u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(1)
     & )
                     u1x=u1r*rx(i1,i2,i3,0,0)+u1s*rx(i1,i2,i3,1,0)
                     u1y=u1r*rx(i1,i2,i3,0,1)+u1s*rx(i1,i2,i3,1,1)
                     u2x=u2r*rx(i1,i2,i3,0,0)+u2s*rx(i1,i2,i3,1,0)
                     u2y=u2r*rx(i1,i2,i3,0,1)+u2s*rx(i1,i2,i3,1,1)
                     if (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                       mu=mupc(i1,i2)
                       lambda=lambdapc(i1,i2)
                     elseif (
     & materialFormat.eq.variableMaterialProperties) then
                       mu=muv(i1,i2)
                       lambda=lambdav(i1,i2)
                     end if
                     s11t=lambda*(u1x+u2y)+2.0*mu*u1x
                     s12t=mu*(u1y+u2x)
                     s21t=s12t
                     s22t=lambda*(u1x+u2y)+2.0*mu*u2y
                     ! (an1,an2) = outward normal and (-an2,an1) = unit tangent
                     aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2))
                     an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                     an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                     b1=-an2*(-an2*(s11t-u(i1,i2,i3,s11c))+an1*(s12t-u(
     & i1,i2,i3,s12c))) +an1*(-an2*(s21t-u(i1,i2,i3,s21c))+an1*(s22t-
     & u(i1,i2,i3,s22c)))
                     b2= an1*( an1*(s11t-u(i1,i2,i3,s11c))+an2*(s12t-u(
     & i1,i2,i3,s12c))) +an2*( an1*(s21t-u(i1,i2,i3,s21c))+an2*(s22t-
     & u(i1,i2,i3,s22c)))
                     u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+an2*b1*an2+an1*
     & b2*an1
                     u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)-an2*b1*an1+an1*
     & b2*an2
                     u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)-an1*b1*an2+an2*
     & b2*an1
                     u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+an1*b1*an1+an2*
     & b2*an2
                  end if
                  end do
                  end do
               end if  ! end gridType
             end if ! bc
            end do ! end side
            end do ! end axis
         !******* Assign symmetry BC on ghost line 1 (note: do this after extrap ghost)
         ! NOTE: we really only need to do the extended boundary and extended ghost??
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
           if( boundaryCondition(side,axis).eq.symmetry )then
            ! even symmetry 
            js1=is1*1
            js2=is2*1
            if( twilightZone.eq.0 )then
              i3=n3a
              do i2=nn2a,nn2b
              do i1=nn1a,nn1b
              if (mask(i1,i2,i3).ne.0) then
               do n=0,numberOfComponents-1
                 u(i1-js1,i2-js2,i3,n)=u(i1+js1,i2+js2,i3,n)
               end do
              end if
              end do
              end do
            else
             ! TZ :
              i3=n3a
              do i2=nn2a,nn2b
              do i1=nn1a,nn1b
              if (mask(i1,i2,i3).ne.0) then
               do n=0,numberOfComponents-1
                 call ogDeriv(ep,0,0,0,0,xy(i1-js1,i2-js2,i3,0),xy(i1-
     & js1,i2-js2,i3,1),0.,t,n,uem)
                 call ogDeriv(ep,0,0,0,0,xy(i1+js1,i2+js2,i3,0),xy(i1+
     & js1,i2+js2,i3,1),0.,t,n,uep)
                 u(i1-js1,i2-js2,i3,n)=u(i1+js1,i2+js2,i3,n) + uem - 
     & uep
               end do
              end if
              end do
              end do
            end if
           end if ! bc
           end do ! end side
           end do ! end axis
          if( debug.gt.32 )then
           n1a=gridIndexRange(0,0)
           n1b=gridIndexRange(1,0)
           n2a=gridIndexRange(0,1)
           n2b=gridIndexRange(1,1)
           n3a=gridIndexRange(0,2)
           n3b=gridIndexRange(1,2)
            write(*,'("v1c",1x,"AftersecondaryDirichlet")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,v1c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,v1c),i1=n1a-2,n1b+
     & 2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("v2c",1x,"AftersecondaryDirichlet")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,v2c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,v2c),i1=n1a-2,n1b+
     & 2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("s11c",1x,"AftersecondaryDirichlet")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s11c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s11c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("s12c",1x,"AftersecondaryDirichlet")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s12c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s12c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("s22c",1x,"AftersecondaryDirichlet")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s22c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s22c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
          end if
         !  debugging
         if (.false.) return
         ! --- Optionally Pin (fix the location of ) corners and edges ---
         ! Do this again to the stress on the boundary is the pinned value
          do n=0,numToPin-1
            ! write(*,'("bcOptSmFOS: pin: grid,side,side2,side3,option=",5i3)') pinbc(0,n),pinbc(1,n),pinbc(2,n),pinbc(3,n),pinbc(4,n)
            if( pinbc(0,n).eq.grid )then
              side1=pinbc(1,n)
              if( side1.eq.0 .or. side1.eq.1 )then
               n1a=gridIndexRange(side1,0)
               n1b=n1a
              else
               n1a=gridIndexRange(0,0)
               n1b=gridIndexRange(1,0)
              end if
              side2=pinbc(2,n)
              if( side2.eq.0 .or. side2.eq.1 )then
               n2a=gridIndexRange(side2,1)
               n2b=n2a
              else
               n2a=gridIndexRange(0,1)
               n2b=gridIndexRange(1,1)
              end if
              side3=pinbc(3,n)
              if( side3.eq.0 .or. side3.eq.1 )then
               n3a=gridIndexRange(side3,2)
               n3b=n3a
              else
               n3a=gridIndexRange(0,2)
               n3b=gridIndexRange(1,2)
              end if
              ! ** FIX ME for parallel **
              ! We set all solution values at the pinned points -- is this correct ??
              if( nd.eq.2 )then
                ! Pin values:
                !    u1,u2,u3, v1,v2,v3, s11, s12, s13, s22, s23, s33
                !     0  1  2   3  4  5    6    7    8    9   10   11
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                 u(i1,i2,i3,uc ) =pinValues( 0,n)
                 u(i1,i2,i3,vc ) =pinValues( 1,n)
                 u(i1,i2,i3,v1c) =pinValues( 3,n)
                 u(i1,i2,i3,v2c) =pinValues( 4,n)
                 u(i1,i2,i3,s11c)=pinValues( 6,n)
                 u(i1,i2,i3,s12c)=pinValues( 7,n)
                 u(i1,i2,i3,s22c)=pinValues( 9,n)
                 ! symmetric values:
                 u(i1,i2,i3,s21c)=u(i1,i2,i3,s12c)
                 ! write(*,'(" pin point (i1,i2,i3)=(",3i4,")")') i1,i2,i3
                 ! write(*,'(" u,v=",2e11.3," v1,v2=",2e11.3," s11,s12,s22=",3e11.3)') u(i1,i2,i3,uc ),u(i1,i2,i3,vc ), u(i1,i2,i3,v1c),u(i1,i2,i3,v2c),u(i1,i2,i3,s11c),u(i1,i2,i3,s12c),u(i1,i2,i3,s22c)
                 end do
                 end do
                 end do
              else
                 do i3=n3a,n3b
                 do i2=n2a,n2b
                 do i1=n1a,n1b
                 u(i1,i2,i3,uc ) =pinValues( 0,n)
                 u(i1,i2,i3,vc ) =pinValues( 1,n)
                 u(i1,i2,i3,wc ) =pinValues( 2,n)
                 u(i1,i2,i3,v1c) =pinValues( 3,n)
                 u(i1,i2,i3,v2c) =pinValues( 4,n)
                 u(i1,i2,i3,v3c) =pinValues( 5,n)
                 u(i1,i2,i3,s11c)=pinValues( 6,n)
                 u(i1,i2,i3,s12c)=pinValues( 7,n)
                 u(i1,i2,i3,s13c)=pinValues( 8,n)
                 u(i1,i2,i3,s22c)=pinValues( 9,n)
                 u(i1,i2,i3,s23c)=pinValues(10,n)
                 u(i1,i2,i3,s33c)=pinValues(11,n)
                 ! symmetric values:
                 u(i1,i2,i3,s21c)=u(i1,i2,i3,s12c)
                 u(i1,i2,i3,s31c)=u(i1,i2,i3,s13c)
                 u(i1,i2,i3,s32c)=u(i1,i2,i3,s23c)
                 end do
                 end do
                 end do
              end if
            end if
          end do
         !*******
         !******* Re-extrapolate the components of stress to first ghost line : Traction or slip wall ********
         !          ( Since we have better values on the boundary now)
         !*******
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
           if( boundaryCondition(side,axis).eq.tractionBC .or. 
     & boundaryCondition(side,axis).eq.slipWall )then
              i3=n3a
              do i2=nn2a,nn2b
              do i1=nn1a,nn1b
               if (mask(i1,i2,i3).ne.0) then
                 ! u(i1-is1,i2-is2,i3,s11c)=extrap3(u,i1,i2,i3,s11c,is1,is2,is3)
                 ! u(i1-is1,i2-is2,i3,s12c)=extrap3(u,i1,i2,i3,s12c,is1,is2,is3)
                 ! u(i1-is1,i2-is2,i3,s21c)=extrap3(u,i1,i2,i3,s21c,is1,is2,is3)
                 ! u(i1-is1,i2-is2,i3,s22c)=extrap3(u,i1,i2,i3,s22c,is1,is2,is3)
                   ! here du2=2nd-order approximation, du3=third order
                   ! Blend the 2nd and 3rd order based on the difference 
                   !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                   du1 = u(i1,i2,i3,s11c)
                   du2 = 2.*u(i1,i2,i3,s11c)-u(i1+is1,i2+is2,i3,s11c)
                   du3 = 3.*u(i1,i2,i3,s11c)-3.*u(i1+is1,i2+is2,i3,
     & s11c)+u(i1+2*is1,i2+2*is2,i3,s11c)
                   !   alpha = cdl*(abs(du3-u(i1+is1,i2+is2,i3,s11c))+abs(du3-du2))/(uEps+abs(u(i1+is1,i2+is2,i3,s11c))+abs(u(i1+2*is1,i2+2*is2,i3,s11c)))
                   ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1+is1,i2+is2,i3,s11c))+abs(u(i1+2*is1,i2+2*is2,i3,s11c)))
                   uNorm= uEps+ abs(du3) + abs(u(i1,i2,i3,s11c))+abs(u(
     & i1+is1,i2+is2,i3,s11c))
                   ! **  du = abs(du3-u(i1+is1,i2+is2,i3,s11c))/uNorm  ! changed 050711
                   ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                   alpha = cdl*( abs(du3-du2)/uNorm )
                   alpha =min(1.,alpha)
                   ! if( mm.eq.1 )then
                 !  if (alpha.gt.0.9) then
                 !    write(6,*)'limiting, s11c,du1,du3=',s11c,du1,du3
                 !    write(6,*)'i1,i2,i3=',i1,i2,i3
                 !    write(6,*)'is1,is2,is3=',is1,is2,is3
                 !  end if
                   !   u(i1,i2,i3,s11c)=(1.-alpha)*du3+alpha*du2
                   u(i1-is1,i2-is2,i3,s11c)=(1.-alpha)*du3+alpha*du1
                   ! here du2=2nd-order approximation, du3=third order
                   ! Blend the 2nd and 3rd order based on the difference 
                   !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                   du1 = u(i1,i2,i3,s12c)
                   du2 = 2.*u(i1,i2,i3,s12c)-u(i1+is1,i2+is2,i3,s12c)
                   du3 = 3.*u(i1,i2,i3,s12c)-3.*u(i1+is1,i2+is2,i3,
     & s12c)+u(i1+2*is1,i2+2*is2,i3,s12c)
                   !   alpha = cdl*(abs(du3-u(i1+is1,i2+is2,i3,s12c))+abs(du3-du2))/(uEps+abs(u(i1+is1,i2+is2,i3,s12c))+abs(u(i1+2*is1,i2+2*is2,i3,s12c)))
                   ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1+is1,i2+is2,i3,s12c))+abs(u(i1+2*is1,i2+2*is2,i3,s12c)))
                   uNorm= uEps+ abs(du3) + abs(u(i1,i2,i3,s12c))+abs(u(
     & i1+is1,i2+is2,i3,s12c))
                   ! **  du = abs(du3-u(i1+is1,i2+is2,i3,s12c))/uNorm  ! changed 050711
                   ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                   alpha = cdl*( abs(du3-du2)/uNorm )
                   alpha =min(1.,alpha)
                   ! if( mm.eq.1 )then
                 !  if (alpha.gt.0.9) then
                 !    write(6,*)'limiting, s12c,du1,du3=',s12c,du1,du3
                 !    write(6,*)'i1,i2,i3=',i1,i2,i3
                 !    write(6,*)'is1,is2,is3=',is1,is2,is3
                 !  end if
                   !   u(i1,i2,i3,s12c)=(1.-alpha)*du3+alpha*du2
                   u(i1-is1,i2-is2,i3,s12c)=(1.-alpha)*du3+alpha*du1
                   ! here du2=2nd-order approximation, du3=third order
                   ! Blend the 2nd and 3rd order based on the difference 
                   !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                   du1 = u(i1,i2,i3,s21c)
                   du2 = 2.*u(i1,i2,i3,s21c)-u(i1+is1,i2+is2,i3,s21c)
                   du3 = 3.*u(i1,i2,i3,s21c)-3.*u(i1+is1,i2+is2,i3,
     & s21c)+u(i1+2*is1,i2+2*is2,i3,s21c)
                   !   alpha = cdl*(abs(du3-u(i1+is1,i2+is2,i3,s21c))+abs(du3-du2))/(uEps+abs(u(i1+is1,i2+is2,i3,s21c))+abs(u(i1+2*is1,i2+2*is2,i3,s21c)))
                   ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1+is1,i2+is2,i3,s21c))+abs(u(i1+2*is1,i2+2*is2,i3,s21c)))
                   uNorm= uEps+ abs(du3) + abs(u(i1,i2,i3,s21c))+abs(u(
     & i1+is1,i2+is2,i3,s21c))
                   ! **  du = abs(du3-u(i1+is1,i2+is2,i3,s21c))/uNorm  ! changed 050711
                   ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                   alpha = cdl*( abs(du3-du2)/uNorm )
                   alpha =min(1.,alpha)
                   ! if( mm.eq.1 )then
                 !  if (alpha.gt.0.9) then
                 !    write(6,*)'limiting, s21c,du1,du3=',s21c,du1,du3
                 !    write(6,*)'i1,i2,i3=',i1,i2,i3
                 !    write(6,*)'is1,is2,is3=',is1,is2,is3
                 !  end if
                   !   u(i1,i2,i3,s21c)=(1.-alpha)*du3+alpha*du2
                   u(i1-is1,i2-is2,i3,s21c)=(1.-alpha)*du3+alpha*du1
                   ! here du2=2nd-order approximation, du3=third order
                   ! Blend the 2nd and 3rd order based on the difference 
                   !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                   du1 = u(i1,i2,i3,s22c)
                   du2 = 2.*u(i1,i2,i3,s22c)-u(i1+is1,i2+is2,i3,s22c)
                   du3 = 3.*u(i1,i2,i3,s22c)-3.*u(i1+is1,i2+is2,i3,
     & s22c)+u(i1+2*is1,i2+2*is2,i3,s22c)
                   !   alpha = cdl*(abs(du3-u(i1+is1,i2+is2,i3,s22c))+abs(du3-du2))/(uEps+abs(u(i1+is1,i2+is2,i3,s22c))+abs(u(i1+2*is1,i2+2*is2,i3,s22c)))
                   ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1+is1,i2+is2,i3,s22c))+abs(u(i1+2*is1,i2+2*is2,i3,s22c)))
                   uNorm= uEps+ abs(du3) + abs(u(i1,i2,i3,s22c))+abs(u(
     & i1+is1,i2+is2,i3,s22c))
                   ! **  du = abs(du3-u(i1+is1,i2+is2,i3,s22c))/uNorm  ! changed 050711
                   ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                   alpha = cdl*( abs(du3-du2)/uNorm )
                   alpha =min(1.,alpha)
                   ! if( mm.eq.1 )then
                 !  if (alpha.gt.0.9) then
                 !    write(6,*)'limiting, s22c,du1,du3=',s22c,du1,du3
                 !    write(6,*)'i1,i2,i3=',i1,i2,i3
                 !    write(6,*)'is1,is2,is3=',is1,is2,is3
                 !  end if
                   !   u(i1,i2,i3,s22c)=(1.-alpha)*du3+alpha*du2
                   u(i1-is1,i2-is2,i3,s22c)=(1.-alpha)*du3+alpha*du1
               end if
              end do
              end do
            end if ! bc
          end do ! end side
          end do ! end axis
         !*******
         !******* re-extrapolate the solution to first ghost line near corners ********
         !         (1) Extrapolate points A and B (below) on displacement sides
         !         (2) Extrapolate corner point C on all physical sides 
         !*******
         !               |
         !               |
         !            A--+---+---+
         !               |
         !            C  B
         i3=gridIndexRange(0,2)
         do side1=0,1
           i1=gridIndexRange(side1,axis1)
           is1=1-2*side1
           do side2=0,1
             i2=gridIndexRange(side2,axis2)
             is2=1-2*side2
             ! extrapolate in the i1 direction
             !*wdh       if (boundaryCondition(side1,axis1).eq.displacementBC) then
             if (boundaryCondition(side1,axis1)
     & .eq.displacementBC.and.boundaryCondition(side2,axis2).gt.0) 
     & then
               if (mask(i1,i2,i3).ne.0) then
                 do n=0,numberOfComponents-1
                   ! u(i1-is1,i2,i3,n)=extrap3(u,i1,i2,i3,n,is1,0,0)
                     ! here du2=2nd-order approximation, du3=third order
                     ! Blend the 2nd and 3rd order based on the difference 
                     !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                     du1 = u(i1,i2,i3,n)
                     du2 = 2.*u(i1,i2,i3,n)-u(i1+is1,i2+0,i3,n)
                     du3 = 3.*u(i1,i2,i3,n)-3.*u(i1+is1,i2+0,i3,n)+u(
     & i1+2*is1,i2+2*0,i3,n)
                     !   alpha = cdl*(abs(du3-u(i1+is1,i2+0,i3,n))+abs(du3-du2))/(uEps+abs(u(i1+is1,i2+0,i3,n))+abs(u(i1+2*is1,i2+2*0,i3,n)))
                     ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1+is1,i2+0,i3,n))+abs(u(i1+2*is1,i2+2*0,i3,n)))
                     uNorm= uEps+ abs(du3) + abs(u(i1,i2,i3,n))+abs(u(
     & i1+is1,i2+0,i3,n))
                     ! **  du = abs(du3-u(i1+is1,i2+0,i3,n))/uNorm  ! changed 050711
                     ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                     alpha = cdl*( abs(du3-du2)/uNorm )
                     alpha =min(1.,alpha)
                     ! if( mm.eq.1 )then
                   !  if (alpha.gt.0.9) then
                   !    write(6,*)'limiting, n,du1,du3=',n,du1,du3
                   !    write(6,*)'i1,i2,i3=',i1,i2,i3
                   !    write(6,*)'is1,0,0=',is1,0,0
                   !  end if
                     !   u(i1,i2,i3,n)=(1.-alpha)*du3+alpha*du2
                     u(i1-is1,i2-0,i3,n)=(1.-alpha)*du3+alpha*du1
                 end do
               end if
             end if
             ! extrapolate in the i2 direction
             !*wdh       if (boundaryCondition(side2,axis2).eq.displacementBC) then
             if (boundaryCondition(side2,axis2)
     & .eq.displacementBC.and.boundaryCondition(side1,axis1).gt.0) 
     & then
               if (mask(i1,i2,i3).ne.0) then
                 do n=0,numberOfComponents-1
                   ! u(i1,i2-is2,i3,n)=extrap3(u,i1,i2,i3,n,0,is2,0)
                     ! here du2=2nd-order approximation, du3=third order
                     ! Blend the 2nd and 3rd order based on the difference 
                     !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                     du1 = u(i1,i2,i3,n)
                     du2 = 2.*u(i1,i2,i3,n)-u(i1+0,i2+is2,i3,n)
                     du3 = 3.*u(i1,i2,i3,n)-3.*u(i1+0,i2+is2,i3,n)+u(
     & i1+2*0,i2+2*is2,i3,n)
                     !   alpha = cdl*(abs(du3-u(i1+0,i2+is2,i3,n))+abs(du3-du2))/(uEps+abs(u(i1+0,i2+is2,i3,n))+abs(u(i1+2*0,i2+2*is2,i3,n)))
                     ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1+0,i2+is2,i3,n))+abs(u(i1+2*0,i2+2*is2,i3,n)))
                     uNorm= uEps+ abs(du3) + abs(u(i1,i2,i3,n))+abs(u(
     & i1+0,i2+is2,i3,n))
                     ! **  du = abs(du3-u(i1+0,i2+is2,i3,n))/uNorm  ! changed 050711
                     ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                     alpha = cdl*( abs(du3-du2)/uNorm )
                     alpha =min(1.,alpha)
                     ! if( mm.eq.1 )then
                   !  if (alpha.gt.0.9) then
                   !    write(6,*)'limiting, n,du1,du3=',n,du1,du3
                   !    write(6,*)'i1,i2,i3=',i1,i2,i3
                   !    write(6,*)'0,is2,0=',0,is2,0
                   !  end if
                     !   u(i1,i2,i3,n)=(1.-alpha)*du3+alpha*du2
                     u(i1-0,i2-is2,i3,n)=(1.-alpha)*du3+alpha*du1
                 end do
               end if
             end if
             ! extrapolate in the diagonal direction
             if (boundaryCondition(side1,axis1)
     & .gt.0.and.boundaryCondition(side2,axis2).gt.0) then
               if (mask(i1,i2,i3).ne.0) then
                 do n=0,numberOfComponents-1
                   ! u(i1-is1,i2-is2,i3,n)=extrap3(u,i1,i2,i3,n,is1,is2,0)
                     ! here du2=2nd-order approximation, du3=third order
                     ! Blend the 2nd and 3rd order based on the difference 
                     !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                     du1 = u(i1,i2,i3,n)
                     du2 = 2.*u(i1,i2,i3,n)-u(i1+is1,i2+is2,i3,n)
                     du3 = 3.*u(i1,i2,i3,n)-3.*u(i1+is1,i2+is2,i3,n)+u(
     & i1+2*is1,i2+2*is2,i3,n)
                     !   alpha = cdl*(abs(du3-u(i1+is1,i2+is2,i3,n))+abs(du3-du2))/(uEps+abs(u(i1+is1,i2+is2,i3,n))+abs(u(i1+2*is1,i2+2*is2,i3,n)))
                     ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1+is1,i2+is2,i3,n))+abs(u(i1+2*is1,i2+2*is2,i3,n)))
                     uNorm= uEps+ abs(du3) + abs(u(i1,i2,i3,n))+abs(u(
     & i1+is1,i2+is2,i3,n))
                     ! **  du = abs(du3-u(i1+is1,i2+is2,i3,n))/uNorm  ! changed 050711
                     ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                     alpha = cdl*( abs(du3-du2)/uNorm )
                     alpha =min(1.,alpha)
                     ! if( mm.eq.1 )then
                   !  if (alpha.gt.0.9) then
                   !    write(6,*)'limiting, n,du1,du3=',n,du1,du3
                   !    write(6,*)'i1,i2,i3=',i1,i2,i3
                   !    write(6,*)'is1,is2,0=',is1,is2,0
                   !  end if
                     !   u(i1,i2,i3,n)=(1.-alpha)*du3+alpha*du2
                     u(i1-is1,i2-is2,i3,n)=(1.-alpha)*du3+alpha*du1
                 end do
               end if
             end if
           end do
         end do
         !******* Assign symmetry BC on ghost line 1 (note: do this after extrap ghost)
         ! NOTE: we really only need to do the extended boundary and extended ghost??
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
           if( boundaryCondition(side,axis).eq.symmetry )then
            ! even symmetry 
            js1=is1*1
            js2=is2*1
            if( twilightZone.eq.0 )then
              i3=n3a
              do i2=nn2a,nn2b
              do i1=nn1a,nn1b
              if (mask(i1,i2,i3).ne.0) then
               do n=0,numberOfComponents-1
                 u(i1-js1,i2-js2,i3,n)=u(i1+js1,i2+js2,i3,n)
               end do
              end if
              end do
              end do
            else
             ! TZ :
              i3=n3a
              do i2=nn2a,nn2b
              do i1=nn1a,nn1b
              if (mask(i1,i2,i3).ne.0) then
               do n=0,numberOfComponents-1
                 call ogDeriv(ep,0,0,0,0,xy(i1-js1,i2-js2,i3,0),xy(i1-
     & js1,i2-js2,i3,1),0.,t,n,uem)
                 call ogDeriv(ep,0,0,0,0,xy(i1+js1,i2+js2,i3,0),xy(i1+
     & js1,i2+js2,i3,1),0.,t,n,uep)
                 u(i1-js1,i2-js2,i3,n)=u(i1+js1,i2+js2,i3,n) + uem - 
     & uep
               end do
              end if
              end do
              end do
            end if
           end if ! bc
           end do ! end side
           end do ! end axis
          if( debug.gt.32 )then
           n1a=gridIndexRange(0,0)
           n1b=gridIndexRange(1,0)
           n2a=gridIndexRange(0,1)
           n2b=gridIndexRange(1,1)
           n3a=gridIndexRange(0,2)
           n3b=gridIndexRange(1,2)
            write(*,'("v1c",1x,"Afterextrapcorners")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,v1c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,v1c),i1=n1a-2,n1b+
     & 2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("v2c",1x,"Afterextrapcorners")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,v2c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,v2c),i1=n1a-2,n1b+
     & 2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("s11c",1x,"Afterextrapcorners")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s11c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s11c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("s12c",1x,"Afterextrapcorners")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s12c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s12c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("s22c",1x,"Afterextrapcorners")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s22c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s22c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
          end if
         !*******
         !******* Extrapolation of stress to the first ghost line (for the tractionBC case only) ********
         !*******
         !******* currently this extrapolation does not work for the case when a tractionBC side ********
         !        includes tractionBC-tractionBC corner and the grid is not Cartesian
         !******* Note: there has been no consideration of this section of code for the case of new linear bcs (which is now the only option) *******
         !              So, the if( .false. ... ) below has to stay
         !******* Note: variable material properties has not been implemented here either.  DWS, 1/6/12
         ! =====================================================================================================
         ! =====================================================================================================
         if( .false. .and. bctype.eq.linearBoundaryCondition ) then ! ----  Only do this for the linear case for now....
             !    write(*,'(" --bcFOS: set stress on the ghost pt --")') 
             ! *WDH* this periodic fix will not work in parallel -- fix me ---
             ! fix up periodic boundaries (if necessary)
             if (boundaryCondition(0,0)
     & .eq.tractionBC.or.boundaryCondition(1,0).eq.tractionBC) then
               if (boundaryCondition(0,1).lt.0) then
                 if (boundaryCondition(1,1).lt.0) then
                   n1a=gridIndexRange(0,0)
                   n1b=gridIndexRange(1,0)
                   n2a=gridIndexRange(0,1)
                   n2b=gridIndexRange(1,1)
                   do i1=n1a-1,n1b+1
                     u(i1,n2a-1,0,uc)=u(i1,n2b-1,0,uc)
                     u(i1,n2a-1,0,vc)=u(i1,n2b-1,0,vc)
                     u(i1,n2b+1,0,uc)=u(i1,n2a+1,0,uc)
                     u(i1,n2b+1,0,vc)=u(i1,n2a+1,0,vc)
                   end do
                 else
                   write(*,'("periodic bcs must be on opposite 
     & sides???")')
                   stop
                 end if
               end if
             elseif (boundaryCondition(0,1)
     & .eq.tractionBC.or.boundaryCondition(1,1).eq.tractionBC) then
               if (boundaryCondition(0,0).lt.0) then
                 if (boundaryCondition(1,0).lt.0) then
                   n1a=gridIndexRange(0,0)
                   n1b=gridIndexRange(1,0)
                   n2a=gridIndexRange(0,1)
                   n2b=gridIndexRange(1,1)
                   do i2=n2a-1,n2b+1
                     u(n1a-1,i2,0,uc)=u(n1b-1,i2,0,uc)
                     u(n1a-1,i2,0,vc)=u(n1b-1,i2,0,vc)
                     u(n1b+1,i2,0,uc)=u(n1a+1,i2,0,uc)
                     u(n1b+1,i2,0,vc)=u(n1a+1,i2,0,vc)
                   end do
                 else
                   write(*,'("periodic bcs must be on opposite 
     & sides???")')
                   stop
                 end if
               end if
             end if
             !c        if( boundaryCondition(0,0).lt.0 )then
             !c         ! perioidic fix -- finish me ---
             !c         n1a=gridIndexRange(0,0)
             !c         n1b=gridIndexRange(1,0)
             !c         n2a=gridIndexRange(0,1)
             !c         n2b=gridIndexRange(1,1)
             !c         do i2=n2a-1,n2b+1
             !c          u(n1a-1,i2,0,uc)=u(n1b-1,i2,0,uc)
             !c          u(n1a-1,i2,0,vc)=u(n1b-1,i2,0,vc)
             !c          u(n1b+1,i2,0,uc)=u(n1a+1,i2,0,uc)
             !c          u(n1b+1,i2,0,vc)=u(n1a+1,i2,0,vc)
             !c         end do
             !c        end if
             ! *** determine the stress on the ghost points from the "normal-derivative" of the stress: :
             !               s_ij = lambda( u.x + v.y ) + 2*mu u.x
             ! 
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
                      i3=n3a
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                         u1xx=(u(i1+1,i2,i3,uc)-2.*u(i1,i2,i3,uc)+u(i1-
     & 1,i2,i3,uc))/dx(0)**2
                         u1xy=(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc)-u(
     & i1+1,i2-1,i3,uc)+u(i1-1,i2-1,i3,uc))/(4.*dx(0)*dx(1))
                         u2xx=(u(i1+1,i2,i3,vc)-2.*u(i1,i2,i3,vc)+u(i1-
     & 1,i2,i3,vc))/dx(0)**2
                         u2xy=(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc)-u(
     & i1+1,i2-1,i3,vc)+u(i1-1,i2-1,i3,vc))/(4.*dx(0)*dx(1))
                         u(i1-is1,i2,i3,s11c)=u(i1+is1,i2,i3,s11c)-2.*
     & is*dx(0)*((lambda+2.*mu)*u1xx+lambda*u2xy)
                         u(i1-is1,i2,i3,s12c)=u(i1+is1,i2,i3,s12c)-2.*
     & is*dx(0)*mu*(u1xy+u2xx)
             !c                    u(i1-is1,i2,i3,s21c)=u(i1-is1,i2,i3,s12c)  
                         u(i1-is1,i2,i3,s21c)=u(i1+is1,i2,i3,s21c)-2.*
     & is*dx(0)*mu*(u1xy+u2xx)
                         u(i1-is1,i2,i3,s22c)=u(i1+is1,i2,i3,s22c)-2.*
     & is*dx(0)*(lambda*u1xx+(lambda+2.*mu)*u2xy)
                      end if
                      end do
                      end do
                   else ! axis .eq. 1
                      i3=n3a
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                      if( mask(i1,i2,i3).gt.0 )then
                         u1xy=(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc)-u(
     & i1+1,i2-1,i3,uc)+u(i1-1,i2-1,i3,uc))/(4.*dx(0)*dx(1))
                         u1yy=(u(i1,i2+1,i3,uc)-2.*u(i1,i2,i3,uc)+u(i1,
     & i2-1,i3,uc))/dx(1)**2
                         u2xy=(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc)-u(
     & i1+1,i2-1,i3,vc)+u(i1-1,i2-1,i3,vc))/(4.*dx(0)*dx(1))
                         u2yy=(u(i1,i2+1,i3,vc)-2.*u(i1,i2,i3,vc)+u(i1,
     & i2-1,i3,vc))/dx(1)**2
                         u(i1,i2-is2,i3,s11c)=u(i1,i2+is2,i3,s11c)-2.*
     & is*dx(1)*((lambda+2.*mu)*u1xy+lambda*u2yy)
                         u(i1,i2-is2,i3,s12c)=u(i1,i2+is2,i3,s12c)-2.*
     & is*dx(1)*mu*(u1yy+u2xy)
             !c                    u(i1,i2-is2,i3,s21c)=u(i1,i2-is2,i3,s12c)
                         u(i1,i2-is2,i3,s21c)=u(i1,i2+is2,i3,s21c)-2.*
     & is*dx(1)*mu*(u1yy+u2xy)
                         u(i1,i2-is2,i3,s22c)=u(i1,i2+is2,i3,s22c)-2.*
     & is*dx(1)*(lambda*u1xy+(lambda+2.*mu)*u2yy)
                      end if
                      end do
                      end do
                   end if ! axis
                 else  ! curvilinear
              if (.false.) then    ! temporary : controls which discretization is used (option 1)
                   if( axis.eq.0 )then
                     if( boundaryCondition(0,1)
     & .ne.tractionBC.and.boundaryCondition(1,1).ne.tractionBC) then
                        i3=n3a
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                           u1xp= (rx(i1+1,i2,i3,0,0)+rx(i1,i2,i3,0,0))*
     & (u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc))/(2.*dr(0)) +(rx(i1+1,i2,i3,1,
     & 0)+rx(i1,i2,i3,1,0))*(u(i1+1,i2+1,i3,uc)+u(i1,i2+1,i3,uc)-u(i1+
     & 1,i2-1,i3,uc)-u(i1,i2-1,i3,uc))/(8.*dr(1))
                           u2xp= (rx(i1+1,i2,i3,0,0)+rx(i1,i2,i3,0,0))*
     & (u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc))/(2.*dr(0)) +(rx(i1+1,i2,i3,1,
     & 0)+rx(i1,i2,i3,1,0))*(u(i1+1,i2+1,i3,vc)+u(i1,i2+1,i3,vc)-u(i1+
     & 1,i2-1,i3,vc)-u(i1,i2-1,i3,vc))/(8.*dr(1))
                           u1yp= (rx(i1+1,i2,i3,0,1)+rx(i1,i2,i3,0,1))*
     & (u(i1+1,i2,i3,uc)-u(i1,i2,i3,uc))/(2.*dr(0)) +(rx(i1+1,i2,i3,1,
     & 1)+rx(i1,i2,i3,1,1))*(u(i1+1,i2+1,i3,uc)+u(i1,i2+1,i3,uc)-u(i1+
     & 1,i2-1,i3,uc)-u(i1,i2-1,i3,uc))/(8.*dr(1))
                           u2yp= (rx(i1+1,i2,i3,0,1)+rx(i1,i2,i3,0,1))*
     & (u(i1+1,i2,i3,vc)-u(i1,i2,i3,vc))/(2.*dr(0)) +(rx(i1+1,i2,i3,1,
     & 1)+rx(i1,i2,i3,1,1))*(u(i1+1,i2+1,i3,vc)+u(i1,i2+1,i3,vc)-u(i1+
     & 1,i2-1,i3,vc)-u(i1,i2-1,i3,vc))/(8.*dr(1))
                           u1xm= (rx(i1,i2,i3,0,0)+rx(i1-1,i2,i3,0,0))*
     & (u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.*dr(0)) +(rx(i1,i2,i3,1,0)
     & +rx(i1-1,i2,i3,1,0))*(u(i1,i2+1,i3,uc)+u(i1-1,i2+1,i3,uc)-u(i1,
     & i2-1,i3,uc)-u(i1-1,i2-1,i3,uc))/(8.*dr(1))
                           u2xm= (rx(i1,i2,i3,0,0)+rx(i1-1,i2,i3,0,0))*
     & (u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.*dr(0)) +(rx(i1,i2,i3,1,0)
     & +rx(i1-1,i2,i3,1,0))*(u(i1,i2+1,i3,vc)+u(i1-1,i2+1,i3,vc)-u(i1,
     & i2-1,i3,vc)-u(i1-1,i2-1,i3,vc))/(8.*dr(1))
                           u1ym= (rx(i1,i2,i3,0,1)+rx(i1-1,i2,i3,0,1))*
     & (u(i1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.*dr(0)) +(rx(i1,i2,i3,1,1)
     & +rx(i1-1,i2,i3,1,1))*(u(i1,i2+1,i3,uc)+u(i1-1,i2+1,i3,uc)-u(i1,
     & i2-1,i3,uc)-u(i1-1,i2-1,i3,uc))/(8.*dr(1))
                           u2ym= (rx(i1,i2,i3,0,1)+rx(i1-1,i2,i3,0,1))*
     & (u(i1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.*dr(0)) +(rx(i1,i2,i3,1,1)
     & +rx(i1-1,i2,i3,1,1))*(u(i1,i2+1,i3,vc)+u(i1-1,i2+1,i3,vc)-u(i1,
     & i2-1,i3,vc)-u(i1-1,i2-1,i3,vc))/(8.*dr(1))
                           u(i1-is1,i2,i3,s11c)=u(i1+is1,i2,i3,s11c)-
     & 2.*is*( (lambda+2.*mu)*(u1xp-u1xm) + lambda*(u2yp-u2ym) )
                           u(i1-is1,i2,i3,s12c)=u(i1+is1,i2,i3,s12c)-
     & 2.*is*( mu*(u1yp-u1ym+u2xp-u2xm) )
                           u(i1-is1,i2,i3,s21c)=u(i1+is1,i2,i3,s21c)-
     & 2.*is*( mu*(u1yp-u1ym+u2xp-u2xm) )
                           u(i1-is1,i2,i3,s22c)=u(i1+is1,i2,i3,s22c)-
     & 2.*is*( lambda*(u1xp-u1xm) + (lambda+2.*mu)*(u2yp-u2ym) )
                        end if
                        end do
                        end do
                     end if
                   else ! axis .eq. 1
                     if( boundaryCondition(0,0)
     & .ne.tractionBC.and.boundaryCondition(1,0).ne.tractionBC) then
                        i3=n3a
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                           u1xp= (rx(i1,i2+1,i3,0,0)+rx(i1,i2,i3,0,0))*
     & (u(i1+1,i2+1,i3,uc)+u(i1+1,i2,i3,uc)-u(i1-1,i2+1,i3,uc)-u(i1-1,
     & i2,i3,uc))/(8.*dr(0)) +(rx(i1,i2+1,i3,1,0)+rx(i1,i2,i3,1,0))*(
     & u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc))/(2.*dr(1))
                           u2xp= (rx(i1,i2+1,i3,0,0)+rx(i1,i2,i3,0,0))*
     & (u(i1+1,i2+1,i3,vc)+u(i1+1,i2,i3,vc)-u(i1-1,i2+1,i3,vc)-u(i1-1,
     & i2,i3,vc))/(8.*dr(0)) +(rx(i1,i2+1,i3,1,0)+rx(i1,i2,i3,1,0))*(
     & u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc))/(2.*dr(1))
                           u1yp= (rx(i1,i2+1,i3,0,1)+rx(i1,i2,i3,0,1))*
     & (u(i1+1,i2+1,i3,uc)+u(i1+1,i2,i3,uc)-u(i1-1,i2+1,i3,uc)-u(i1-1,
     & i2,i3,uc))/(8.*dr(0)) +(rx(i1,i2+1,i3,1,1)+rx(i1,i2,i3,1,1))*(
     & u(i1,i2+1,i3,uc)-u(i1,i2,i3,uc))/(2.*dr(1))
                           u2yp= (rx(i1,i2+1,i3,0,1)+rx(i1,i2,i3,0,1))*
     & (u(i1+1,i2+1,i3,vc)+u(i1+1,i2,i3,vc)-u(i1-1,i2+1,i3,vc)-u(i1-1,
     & i2,i3,vc))/(8.*dr(0)) +(rx(i1,i2+1,i3,1,1)+rx(i1,i2,i3,1,1))*(
     & u(i1,i2+1,i3,vc)-u(i1,i2,i3,vc))/(2.*dr(1))
                           u1xm= (rx(i1,i2,i3,0,0)+rx(i1,i2-1,i3,0,0))*
     & (u(i1+1,i2,i3,uc)+u(i1+1,i2-1,i3,uc)-u(i1-1,i2,i3,uc)-u(i1-1,
     & i2-1,i3,uc))/(8.*dr(0)) +(rx(i1,i2,i3,1,0)+rx(i1,i2-1,i3,1,0))*
     & (u(i1,i2,i3,uc)-u(i1,i2-1,i3,uc))/(2.*dr(1))
                           u2xm= (rx(i1,i2,i3,0,0)+rx(i1,i2-1,i3,0,0))*
     & (u(i1+1,i2,i3,vc)+u(i1+1,i2-1,i3,vc)-u(i1-1,i2,i3,vc)-u(i1-1,
     & i2-1,i3,vc))/(8.*dr(0)) +(rx(i1,i2,i3,1,0)+rx(i1,i2-1,i3,1,0))*
     & (u(i1,i2,i3,vc)-u(i1,i2-1,i3,vc))/(2.*dr(1))
                           u1ym= (rx(i1,i2,i3,0,1)+rx(i1,i2-1,i3,0,1))*
     & (u(i1+1,i2,i3,uc)+u(i1+1,i2-1,i3,uc)-u(i1-1,i2,i3,uc)-u(i1-1,
     & i2-1,i3,uc))/(8.*dr(0)) +(rx(i1,i2,i3,1,1)+rx(i1,i2-1,i3,1,1))*
     & (u(i1,i2,i3,uc)-u(i1,i2-1,i3,uc))/(2.*dr(1))
                           u2ym= (rx(i1,i2,i3,0,1)+rx(i1,i2-1,i3,0,1))*
     & (u(i1+1,i2,i3,vc)+u(i1+1,i2-1,i3,vc)-u(i1-1,i2,i3,vc)-u(i1-1,
     & i2-1,i3,vc))/(8.*dr(0)) +(rx(i1,i2,i3,1,1)+rx(i1,i2-1,i3,1,1))*
     & (u(i1,i2,i3,vc)-u(i1,i2-1,i3,vc))/(2.*dr(1))
                           u(i1,i2-is2,i3,s11c)=u(i1,i2+is2,i3,s11c)-
     & 2.*is*( (lambda+2.*mu)*(u1xp-u1xm) + lambda*(u2yp-u2ym) )
                           u(i1,i2-is2,i3,s12c)=u(i1,i2+is2,i3,s12c)-
     & 2.*is*( mu*(u1yp-u1ym+u2xp-u2xm) )
                           u(i1,i2-is2,i3,s21c)=u(i1,i2+is2,i3,s21c)-
     & 2.*is*( mu*(u1yp-u1ym+u2xp-u2xm) )
                           u(i1,i2-is2,i3,s22c)=u(i1,i2+is2,i3,s22c)-
     & 2.*is*( lambda*(u1xp-u1xm) + (lambda+2.*mu)*(u2yp-u2ym) )
                        end if
                        end do
                        end do
                     end if
                   end if ! axis
                else        ! option 2
                   if( axis.eq.0 )then
                     if( boundaryCondition(0,1)
     & .ne.tractionBC.and.boundaryCondition(1,1).ne.tractionBC) then
                        i3=n3a
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                           u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(
     & 2.0*dr(0))
                           u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(
     & 2.0*dr(1))
                           u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(
     & 2.0*dr(0))
                           u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(
     & 2.0*dr(1))
                           u1rr=(u(i1+1,i2,i3,uc)-2.*u(i1,i2,i3,uc)+u(
     & i1-1,i2,i3,uc))/dr(0)**2
                           u1rs=(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc)-
     & u(i1+1,i2-1,i3,uc)+u(i1-1,i2-1,i3,uc))/(4.*dr(0)*dr(1))
                           u2rr=(u(i1+1,i2,i3,vc)-2.*u(i1,i2,i3,vc)+u(
     & i1-1,i2,i3,vc))/dr(0)**2
                           u2rs=(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc)-
     & u(i1+1,i2-1,i3,vc)+u(i1-1,i2-1,i3,vc))/(4.*dr(0)*dr(1))
                           rxr=(rx(i1+1,i2,i3,0,0)-rx(i1-1,i2,i3,0,0))
     & /(2.*dr(0))
                           ryr=(rx(i1+1,i2,i3,0,1)-rx(i1-1,i2,i3,0,1))
     & /(2.*dr(0))
                           sxr=(rx(i1+1,i2,i3,1,0)-rx(i1-1,i2,i3,1,0))
     & /(2.*dr(0))
                           syr=(rx(i1+1,i2,i3,1,1)-rx(i1-1,i2,i3,1,1))
     & /(2.*dr(0))
                           u(i1-is1,i2,i3,s11c)=u(i1+is1,i2,i3,s11c)-
     & 2.*is*dr(0)*( +(lambda+2.*mu)*(u1rr*rx(i1,i2,i3,0,0)+u1rs*rx(
     & i1,i2,i3,1,0)+u1r*rxr+u1s*sxr) +lambda*(u2rr*rx(i1,i2,i3,0,1)+
     & u2rs*rx(i1,i2,i3,1,1)+u2r*ryr+u2s*syr))
                           u(i1-is1,i2,i3,s12c)=u(i1+is1,i2,i3,s12c)-
     & 2.*is*dr(0)*( +mu*((u1rr*rx(i1,i2,i3,0,1)+u1rs*rx(i1,i2,i3,1,1)
     & +u1r*ryr+u1s*syr) +(u2rr*rx(i1,i2,i3,0,0)+u2rs*rx(i1,i2,i3,1,0)
     & +u2r*rxr+u2s*sxr)))
                            u(i1-is1,i2,i3,s21c)=u(i1-is1,i2,i3,s12c)
                           u(i1-is1,i2,i3,s21c)=u(i1+is1,i2,i3,s21c)-
     & 2.*is*dr(0)*( +mu*((u1rr*rx(i1,i2,i3,0,1)+u1rs*rx(i1,i2,i3,1,1)
     & +u1r*ryr+u1s*syr) +(u2rr*rx(i1,i2,i3,0,0)+u2rs*rx(i1,i2,i3,1,0)
     & +u2r*rxr+u2s*sxr)))
                           u(i1-is1,i2,i3,s22c)=u(i1+is1,i2,i3,s22c)-
     & 2.*is*dr(0)*( +lambda*(u1rr*rx(i1,i2,i3,0,0)+u1rs*rx(i1,i2,i3,
     & 1,0)+u1r*rxr+u1s*sxr) +(lambda+2.*mu)*(u2rr*rx(i1,i2,i3,0,1)+
     & u2rs*rx(i1,i2,i3,1,1)+u2r*ryr+u2s*syr))
                        end if
                        end do
                        end do
                     end if
                   else ! axis .eq. 1
                     if( boundaryCondition(0,0)
     & .ne.tractionBC.and.boundaryCondition(1,0).ne.tractionBC) then
                        i3=n3a
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                        if( mask(i1,i2,i3).gt.0 )then
                           u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(
     & 2.0*dr(0))
                           u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(
     & 2.0*dr(1))
                           u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(
     & 2.0*dr(0))
                           u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(
     & 2.0*dr(1))
                           u1rs=(u(i1+1,i2+1,i3,uc)-u(i1-1,i2+1,i3,uc)-
     & u(i1+1,i2-1,i3,uc)+u(i1-1,i2-1,i3,uc))/(4.*dr(0)*dr(1))
                           u1ss=(u(i1,i2+1,i3,uc)-2.*u(i1,i2,i3,uc)+u(
     & i1,i2-1,i3,uc))/dr(1)**2
                           u2rs=(u(i1+1,i2+1,i3,vc)-u(i1-1,i2+1,i3,vc)-
     & u(i1+1,i2-1,i3,vc)+u(i1-1,i2-1,i3,vc))/(4.*dr(0)*dr(1))
                           u2ss=(u(i1,i2+1,i3,vc)-2.*u(i1,i2,i3,vc)+u(
     & i1,i2-1,i3,vc))/dr(1)**2
                           rxs=(rx(i1,i2+1,i3,0,0)-rx(i1,i2-1,i3,0,0))
     & /(2.*dr(1))
                           rys=(rx(i1,i2+1,i3,0,1)-rx(i1,i2-1,i3,0,1))
     & /(2.*dr(1))
                           sxs=(rx(i1,i2+1,i3,1,0)-rx(i1,i2-1,i3,1,0))
     & /(2.*dr(1))
                           sys=(rx(i1,i2+1,i3,1,1)-rx(i1,i2-1,i3,1,1))
     & /(2.*dr(1))
                           u(i1,i2-is2,i3,s11c)=u(i1,i2+is2,i3,s11c)-
     & 2.*is*dr(1)*( +(lambda+2.*mu)*(u1rs*rx(i1,i2,i3,0,0)+u1ss*rx(
     & i1,i2,i3,1,0)+u1r*rxs+u1s*sxs) +lambda*(u2rs*rx(i1,i2,i3,0,1)+
     & u2ss*rx(i1,i2,i3,1,1)+u2r*rys+u2s*sys))
                           u(i1,i2-is2,i3,s12c)=u(i1,i2+is2,i3,s12c)-
     & 2.*is*dr(1)*( +mu*((u1rs*rx(i1,i2,i3,0,1)+u1ss*rx(i1,i2,i3,1,1)
     & +u1r*rys+u1s*sys) +(u2rs*rx(i1,i2,i3,0,0)+u2ss*rx(i1,i2,i3,1,0)
     & +u2r*rxs+u2s*sxs)))
                            u(i1,i2-is2,i3,s21c)=u(i1,i2-is2,i3,s12c)
                           u(i1,i2-is2,i3,s21c)=u(i1,i2+is2,i3,s21c)-
     & 2.*is*dr(1)*( +mu*((u1rs*rx(i1,i2,i3,0,1)+u1ss*rx(i1,i2,i3,1,1)
     & +u1r*rys+u1s*sys) +(u2rs*rx(i1,i2,i3,0,0)+u2ss*rx(i1,i2,i3,1,0)
     & +u2r*rxs+u2s*sxs)))
                           u(i1,i2-is2,i3,s22c)=u(i1,i2+is2,i3,s22c)-
     & 2.*is*dr(1)*( +lambda*(u1rs*rx(i1,i2,i3,0,0)+u1ss*rx(i1,i2,i3,
     & 1,0)+u1r*rxs+u1s*sxs) +(lambda+2.*mu)*(u2rs*rx(i1,i2,i3,0,1)+
     & u2ss*rx(i1,i2,i3,1,1)+u2r*rys+u2s*sys))
                        end if
                        end do
                        end do
                     end if
                   end if ! axis
                end if    ! end of options (temporary)
                 end if  ! end gridType
               end if ! bc
              end do ! end side
              end do ! end axis
             ! add on TZ forcing (if necessary)
             if (twilightZone.ne.0) then
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
                        i3=n3a
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if (mask(i1,i2,i3).ne.0) then
                           call ogDeriv (ep,0,2,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,uc,u1xxe)
                           call ogDeriv (ep,0,2,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,vc,u2xxe)
                           call ogDeriv (ep,0,1,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,uc,u1xye)
                           call ogDeriv (ep,0,1,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,vc,u2xye)
                           call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s11c,s11xe)
                           call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s12c,s12xe)
                           call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s21c,s21xe)
                           call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s22c,s22xe)
                           u(i1-is1,i2,i3,s11c)=u(i1-is1,i2,i3,s11c)-
     & 2.*is*dx(0)*(s11xe-(lambda+2.*mu)*u1xxe-lambda*u2xye)
                           u(i1-is1,i2,i3,s12c)=u(i1-is1,i2,i3,s12c)-
     & 2.*is*dx(0)*(s12xe-mu*(u1xye+u2xxe))
                           u(i1-is1,i2,i3,s21c)=u(i1-is1,i2,i3,s21c)-
     & 2.*is*dx(0)*(s21xe-mu*(u1xye+u2xxe))
                           u(i1-is1,i2,i3,s22c)=u(i1-is1,i2,i3,s22c)-
     & 2.*is*dx(0)*(s22xe-lambda*u1xxe-(lambda+2.*mu)*u2xye)
                         end if
                        end do
                        end do
                     else ! axis .eq. 1
                        i3=n3a
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                         if (mask(i1,i2,i3).ne.0) then
                           call ogDeriv (ep,0,1,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,uc,u1xye)
                           call ogDeriv (ep,0,1,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,vc,u2xye)
                           call ogDeriv (ep,0,0,2,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,uc,u1yye)
                           call ogDeriv (ep,0,0,2,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,vc,u2yye)
                           call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s11c,s11ye)
                           call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s12c,s12ye)
                           call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s21c,s21ye)
                           call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(
     & i1,i2,i3,1),0.0,t,s22c,s22ye)
                           u(i1,i2-is2,i3,s11c)=u(i1,i2-is2,i3,s11c)-
     & 2.*is*dx(1)*(s11ye-(lambda+2.*mu)*u1xye-lambda*u2yye)
                           u(i1,i2-is2,i3,s12c)=u(i1,i2-is2,i3,s12c)-
     & 2.*is*dx(1)*(s12ye-mu*(u1yye+u2xye))
                           u(i1,i2-is2,i3,s21c)=u(i1,i2-is2,i3,s21c)-
     & 2.*is*dx(1)*(s21ye-mu*(u1yye+u2xye))
                           u(i1,i2-is2,i3,s22c)=u(i1,i2-is2,i3,s22c)-
     & 2.*is*dx(1)*(s22ye-lambda*u1xye-(lambda+2.*mu)*u2yye)
                         end if
                        end do
                        end do
                     end if ! axis
                   else  ! curvilinear
               if (.false.) then     ! temporary : difference ways to apply the TZ forcing
                     if( axis.eq.0 )then
                       if( boundaryCondition(0,1)
     & .ne.tractionBC.and.boundaryCondition(1,1).ne.tractionBC) then
                          i3=n3a
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                           if (mask(i1,i2,i3).ne.0) then
                             call ogDeriv (ep,0,1,0,0,xy(i1+is1,i2,i3,
     & 0),xy(i1+is1,i2,i3,1),0.0,t,uc,u1xpe)
                             call ogDeriv (ep,0,1,0,0,xy(i1+is1,i2,i3,
     & 0),xy(i1+is1,i2,i3,1),0.0,t,vc,u2xpe)
                             call ogDeriv (ep,0,0,1,0,xy(i1+is1,i2,i3,
     & 0),xy(i1+is1,i2,i3,1),0.0,t,uc,u1ype)
                             call ogDeriv (ep,0,0,1,0,xy(i1+is1,i2,i3,
     & 0),xy(i1+is1,i2,i3,1),0.0,t,vc,u2ype)
                             call ogDeriv (ep,0,1,0,0,xy(i1-is1,i2,i3,
     & 0),xy(i1-is1,i2,i3,1),0.0,t,uc,u1xme)
                             call ogDeriv (ep,0,1,0,0,xy(i1-is1,i2,i3,
     & 0),xy(i1-is1,i2,i3,1),0.0,t,vc,u2xme)
                             call ogDeriv (ep,0,0,1,0,xy(i1-is1,i2,i3,
     & 0),xy(i1-is1,i2,i3,1),0.0,t,uc,u1yme)
                             call ogDeriv (ep,0,0,1,0,xy(i1-is1,i2,i3,
     & 0),xy(i1-is1,i2,i3,1),0.0,t,vc,u2yme)
                             call ogDeriv (ep,0,0,0,0,xy(i1+is1,i2,i3,
     & 0),xy(i1+is1,i2,i3,1),0.0,t,s11c,s11pe)
                             call ogDeriv (ep,0,0,0,0,xy(i1+is1,i2,i3,
     & 0),xy(i1+is1,i2,i3,1),0.0,t,s12c,s12pe)
                             call ogDeriv (ep,0,0,0,0,xy(i1+is1,i2,i3,
     & 0),xy(i1+is1,i2,i3,1),0.0,t,s21c,s21pe)
                             call ogDeriv (ep,0,0,0,0,xy(i1+is1,i2,i3,
     & 0),xy(i1+is1,i2,i3,1),0.0,t,s22c,s22pe)
                             call ogDeriv (ep,0,0,0,0,xy(i1-is1,i2,i3,
     & 0),xy(i1-is1,i2,i3,1),0.0,t,s11c,s11me)
                             call ogDeriv (ep,0,0,0,0,xy(i1-is1,i2,i3,
     & 0),xy(i1-is1,i2,i3,1),0.0,t,s12c,s12me)
                             call ogDeriv (ep,0,0,0,0,xy(i1-is1,i2,i3,
     & 0),xy(i1-is1,i2,i3,1),0.0,t,s21c,s21me)
                             call ogDeriv (ep,0,0,0,0,xy(i1-is1,i2,i3,
     & 0),xy(i1-is1,i2,i3,1),0.0,t,s22c,s22me)
                             u(i1-is1,i2,i3,s11c)=u(i1-is1,i2,i3,s11c)+
     & s11me-s11pe+(lambda+2.*mu)*(u1xpe-u1xme)+lambda*(u2ype-u2yme)
                             u(i1-is1,i2,i3,s12c)=u(i1-is1,i2,i3,s12c)+
     & s12me-s12pe+mu*(u1ype-u1yme+u2xpe-u2xme)
                             u(i1-is1,i2,i3,s21c)=u(i1-is1,i2,i3,s21c)+
     & s21me-s21pe+mu*(u1ype-u1yme+u2xpe-u2xme)
                             u(i1-is1,i2,i3,s22c)=u(i1-is1,i2,i3,s22c)+
     & s22me-s22pe+lambda*(u1xpe-u1xme)+(lambda+2.*mu)*(u2ype-u2yme)
                            !c          call ogDeriv (ep,0,0,0,0,xy(i1-is1,i2,i3,0),xy(i1-is1,i2,i3,1),0.0,t,s11c,s11e)
                            !c          call ogDeriv (ep,0,0,0,0,xy(i1-is1,i2,i3,0),xy(i1-is1,i2,i3,1),0.0,t,s12c,s12e)
                            !c          call ogDeriv (ep,0,0,0,0,xy(i1-is1,i2,i3,0),xy(i1-is1,i2,i3,1),0.0,t,s21c,s21e)
                            !c          call ogDeriv (ep,0,0,0,0,xy(i1-is1,i2,i3,0),xy(i1-is1,i2,i3,1),0.0,t,s22c,s22e)
                            !c          s11e=u(i1-is1,i2,i3,s11c)-s11e
                            !c          s12e=u(i1-is1,i2,i3,s12c)-s12e
                            !c          s21e=u(i1-is1,i2,i3,s21c)-s21e
                            !c          s22e=u(i1-is1,i2,i3,s22c)-s22e
                            !c   write(6,"(2(1x,i2),4(1x,1pe10.3))")i1-is1,i2,s11e,s12e,s21e,s22e
                           end if
                          end do
                          end do
                       end if
                     else ! axis .eq. 1
                       if( boundaryCondition(0,0)
     & .ne.tractionBC.and.boundaryCondition(1,0).ne.tractionBC) then
                          i3=n3a
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                           if (mask(i1,i2,i3).ne.0) then
                             call ogDeriv (ep,0,1,0,0,xy(i1,i2+is2,i3,
     & 0),xy(i1,i2+is2,i3,1),0.0,t,uc,u1xpe)
                             call ogDeriv (ep,0,1,0,0,xy(i1,i2+is2,i3,
     & 0),xy(i1,i2+is2,i3,1),0.0,t,vc,u2xpe)
                             call ogDeriv (ep,0,0,1,0,xy(i1,i2+is2,i3,
     & 0),xy(i1,i2+is2,i3,1),0.0,t,uc,u1ype)
                             call ogDeriv (ep,0,0,1,0,xy(i1,i2+is2,i3,
     & 0),xy(i1,i2+is2,i3,1),0.0,t,vc,u2ype)
                             call ogDeriv (ep,0,1,0,0,xy(i1,i2-is2,i3,
     & 0),xy(i1,i2-is2,i3,1),0.0,t,uc,u1xme)
                             call ogDeriv (ep,0,1,0,0,xy(i1,i2-is2,i3,
     & 0),xy(i1,i2-is2,i3,1),0.0,t,vc,u2xme)
                             call ogDeriv (ep,0,0,1,0,xy(i1,i2-is2,i3,
     & 0),xy(i1,i2-is2,i3,1),0.0,t,uc,u1yme)
                             call ogDeriv (ep,0,0,1,0,xy(i1,i2-is2,i3,
     & 0),xy(i1,i2-is2,i3,1),0.0,t,vc,u2yme)
                             call ogDeriv (ep,0,0,0,0,xy(i1,i2+is2,i3,
     & 0),xy(i1,i2+is2,i3,1),0.0,t,s11c,s11pe)
                             call ogDeriv (ep,0,0,0,0,xy(i1,i2+is2,i3,
     & 0),xy(i1,i2+is2,i3,1),0.0,t,s12c,s12pe)
                             call ogDeriv (ep,0,0,0,0,xy(i1,i2+is2,i3,
     & 0),xy(i1,i2+is2,i3,1),0.0,t,s21c,s21pe)
                             call ogDeriv (ep,0,0,0,0,xy(i1,i2+is2,i3,
     & 0),xy(i1,i2+is2,i3,1),0.0,t,s22c,s22pe)
                             call ogDeriv (ep,0,0,0,0,xy(i1,i2-is2,i3,
     & 0),xy(i1,i2-is2,i3,1),0.0,t,s11c,s11me)
                             call ogDeriv (ep,0,0,0,0,xy(i1,i2-is2,i3,
     & 0),xy(i1,i2-is2,i3,1),0.0,t,s12c,s12me)
                             call ogDeriv (ep,0,0,0,0,xy(i1,i2-is2,i3,
     & 0),xy(i1,i2-is2,i3,1),0.0,t,s21c,s21me)
                             call ogDeriv (ep,0,0,0,0,xy(i1,i2-is2,i3,
     & 0),xy(i1,i2-is2,i3,1),0.0,t,s22c,s22me)
                             u(i1,i2-is2,i3,s11c)=u(i1,i2-is2,i3,s11c)+
     & s11me-s11pe+(lambda+2.*mu)*(u1xpe-u1xme)+lambda*(u2ype-u2yme)
                             u(i1,i2-is2,i3,s12c)=u(i1,i2-is2,i3,s12c)+
     & s12me-s12pe+mu*(u1ype-u1yme+u2xpe-u2xme)
                             u(i1,i2-is2,i3,s21c)=u(i1,i2-is2,i3,s21c)+
     & s21me-s21pe+mu*(u1ype-u1yme+u2xpe-u2xme)
                             u(i1,i2-is2,i3,s22c)=u(i1,i2-is2,i3,s22c)+
     & s22me-s22pe+lambda*(u1xpe-u1xme)+(lambda+2.*mu)*(u2ype-u2yme)
                            !c          call ogDeriv (ep,0,0,0,0,xy(i1,i2-is2,i3,0),xy(i1,i2-is2,i3,1),0.0,t,s11c,s11e)
                            !c          call ogDeriv (ep,0,0,0,0,xy(i1,i2-is2,i3,0),xy(i1,i2-is2,i3,1),0.0,t,s12c,s12e)
                            !c          call ogDeriv (ep,0,0,0,0,xy(i1,i2-is2,i3,0),xy(i1,i2-is2,i3,1),0.0,t,s21c,s21e)
                            !c          call ogDeriv (ep,0,0,0,0,xy(i1,i2-is2,i3,0),xy(i1,i2-is2,i3,1),0.0,t,s22c,s22e)
                            !c          s11e=u(i1,i2-is2,i3,s11c)-s11e
                            !c          s12e=u(i1,i2-is2,i3,s12c)-s12e
                            !c          s21e=u(i1,i2-is2,i3,s21c)-s21e
                            !c          s22e=u(i1,i2-is2,i3,s22c)-s22e
                            !c   write(6,"(2(1x,i2),4(1x,1pe10.3))")i1,i2-is2,s11e,s12e,s21e,s22e
                           end if
                          end do
                          end do
                       end if
                     end if ! axis
                  else     ! options
                     if( axis.eq.0 )then
                       if( boundaryCondition(0,1)
     & .ne.tractionBC.and.boundaryCondition(1,1).ne.tractionBC) then
                          i3=n3a
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                           if (mask(i1,i2,i3).ne.0) then
                             call ogDeriv (ep,0,2,0,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,uc,u1xxe)
                             call ogDeriv (ep,0,2,0,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,vc,u2xxe)
                             call ogDeriv (ep,0,1,1,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,uc,u1xye)
                             call ogDeriv (ep,0,1,1,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,vc,u2xye)
                             call ogDeriv (ep,0,0,2,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,uc,u1yye)
                             call ogDeriv (ep,0,0,2,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,vc,u2yye)
                             call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s11c,s11xe)
                             call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s12c,s12xe)
                             call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s21c,s21xe)
                             call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s22c,s22xe)
                             call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s11c,s11ye)
                             call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s12c,s12ye)
                             call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s21c,s21ye)
                             call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s22c,s22ye)
                             u(i1-is1,i2,i3,s11c)=u(i1-is1,i2,i3,s11c)-
     & 2.*is*dr(0)*det(i1,i2,i3)*( +rx(i1,i2,i3,1,1)*(s11xe-(lambda+
     & 2.*mu)*u1xxe-lambda*u2xye) -rx(i1,i2,i3,1,0)*(s11ye-(lambda+2.*
     & mu)*u1xye-lambda*u2yye))
                             u(i1-is1,i2,i3,s12c)=u(i1-is1,i2,i3,s12c)-
     & 2.*is*dr(0)*det(i1,i2,i3)*( +rx(i1,i2,i3,1,1)*(s12xe-mu*(u1xye+
     & u2xxe)) -rx(i1,i2,i3,1,0)*(s12ye-mu*(u1yye+u2xye)))
                             u(i1-is1,i2,i3,s21c)=u(i1-is1,i2,i3,s21c)-
     & 2.*is*dr(0)*det(i1,i2,i3)*( +rx(i1,i2,i3,1,1)*(s21xe-mu*(u1xye+
     & u2xxe)) -rx(i1,i2,i3,1,0)*(s21ye-mu*(u1yye+u2xye)))
                             u(i1-is1,i2,i3,s22c)=u(i1-is1,i2,i3,s22c)-
     & 2.*is*dr(0)*det(i1,i2,i3)*( +rx(i1,i2,i3,1,1)*(s22xe-lambda*
     & u1xxe-(lambda+2.*mu)*u2xye) -rx(i1,i2,i3,1,0)*(s22ye-lambda*
     & u1xye-(lambda+2.*mu)*u2yye))
                           end if
                          end do
                          end do
                       end if
                     else ! axis .eq. 1
                       if( boundaryCondition(0,0)
     & .ne.tractionBC.and.boundaryCondition(1,0).ne.tractionBC) then
                          i3=n3a
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                           if (mask(i1,i2,i3).ne.0) then
                             call ogDeriv (ep,0,2,0,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,uc,u1xxe)
                             call ogDeriv (ep,0,2,0,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,vc,u2xxe)
                             call ogDeriv (ep,0,1,1,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,uc,u1xye)
                             call ogDeriv (ep,0,1,1,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,vc,u2xye)
                             call ogDeriv (ep,0,0,2,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,uc,u1yye)
                             call ogDeriv (ep,0,0,2,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,vc,u2yye)
                             call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s11c,s11xe)
                             call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s12c,s12xe)
                             call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s21c,s21xe)
                             call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s22c,s22xe)
                             call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s11c,s11ye)
                             call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s12c,s12ye)
                             call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s21c,s21ye)
                             call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),
     & xy(i1,i2,i3,1),0.0,t,s22c,s22ye)
                             u(i1,i2-is2,i3,s11c)=u(i1,i2-is2,i3,s11c)-
     & 2.*is*dr(1)*det(i1,i2,i3)*( -rx(i1,i2,i3,0,1)*(s11xe-(lambda+
     & 2.*mu)*u1xxe-lambda*u2xye) +rx(i1,i2,i3,0,0)*(s11ye-(lambda+2.*
     & mu)*u1xye-lambda*u2yye))
                             u(i1,i2-is2,i3,s12c)=u(i1,i2-is2,i3,s12c)-
     & 2.*is*dr(1)*det(i1,i2,i3)*( -rx(i1,i2,i3,0,1)*(s12xe-mu*(u1xye+
     & u2xxe)) +rx(i1,i2,i3,0,0)*(s12ye-mu*(u1yye+u2xye)))
                             u(i1,i2-is2,i3,s21c)=u(i1,i2-is2,i3,s21c)-
     & 2.*is*dr(1)*det(i1,i2,i3)*( -rx(i1,i2,i3,0,1)*(s21xe-mu*(u1xye+
     & u2xxe)) +rx(i1,i2,i3,0,0)*(s21ye-mu*(u1yye+u2xye)))
                             u(i1,i2-is2,i3,s22c)=u(i1,i2-is2,i3,s22c)-
     & 2.*is*dr(1)*det(i1,i2,i3)*( -rx(i1,i2,i3,0,1)*(s22xe-lambda*
     & u1xxe-(lambda+2.*mu)*u2xye) +rx(i1,i2,i3,0,0)*(s22ye-lambda*
     & u1xye-(lambda+2.*mu)*u2yye))
                           end if
                          end do
                          end do
                       end if
                     end if ! axis
                 end if   ! end options
                   end if  ! end gridType
                 end if ! bc
                end do ! end side
                end do ! end axis
             end if
             !c*******
             !c******* An attempt to fix up the tractionBC-tractionBC corner problem ********
             !c******* (helps but is not satisfactory)
             !c..fix up values for stress in the first ghost line near corners
             !c  and extrapolate all components in the corner ghost points.
             i3=gridIndexRange(0,2)
             do side1=0,1
               i1=gridIndexRange(side1,axis1)
               is1=1-2*side1
               do side2=0,1
                 i2=gridIndexRange(side2,axis2)
                 is2=1-2*side2
             !c extrapolate in the i1 direction
                 if (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC) then
                   if (mask(i1,i2,i3).ne.0) then
                     u(i1-is1,i2,i3,s11c)=(3.*u(i1,i2,i3,s11c)-3.*u(i1+
     & is1,i2+0,i3+0,s11c)+u(i1+2*is1,i2+2*0,i3+2*0,s11c))
                     u(i1-is1,i2,i3,s12c)=(3.*u(i1,i2,i3,s12c)-3.*u(i1+
     & is1,i2+0,i3+0,s12c)+u(i1+2*is1,i2+2*0,i3+2*0,s12c))
                     u(i1-is1,i2,i3,s21c)=(3.*u(i1,i2,i3,s21c)-3.*u(i1+
     & is1,i2+0,i3+0,s21c)+u(i1+2*is1,i2+2*0,i3+2*0,s21c))
                     u(i1-is1,i2,i3,s22c)=(3.*u(i1,i2,i3,s22c)-3.*u(i1+
     & is1,i2+0,i3+0,s22c)+u(i1+2*is1,i2+2*0,i3+2*0,s22c))
                   end if
                 end if
             !c extrapolate in the i2 direction
                 if (boundaryCondition(side2,axis2)
     & .eq.tractionBC.and.boundaryCondition(side1,axis1)
     & .eq.tractionBC) then
                   if (mask(i1,i2,i3).ne.0) then
                     u(i1,i2-is2,i3,s11c)=(3.*u(i1,i2,i3,s11c)-3.*u(i1+
     & 0,i2+is2,i3+0,s11c)+u(i1+2*0,i2+2*is2,i3+2*0,s11c))
                     u(i1,i2-is2,i3,s12c)=(3.*u(i1,i2,i3,s12c)-3.*u(i1+
     & 0,i2+is2,i3+0,s12c)+u(i1+2*0,i2+2*is2,i3+2*0,s12c))
                     u(i1,i2-is2,i3,s21c)=(3.*u(i1,i2,i3,s21c)-3.*u(i1+
     & 0,i2+is2,i3+0,s21c)+u(i1+2*0,i2+2*is2,i3+2*0,s21c))
                     u(i1,i2-is2,i3,s22c)=(3.*u(i1,i2,i3,s22c)-3.*u(i1+
     & 0,i2+is2,i3+0,s22c)+u(i1+2*0,i2+2*is2,i3+2*0,s22c))
                   end if
                 end if
             !c extrapolate in the diagonal direction
                 if (boundaryCondition(side1,axis1)
     & .gt.0.and.boundaryCondition(side2,axis2).gt.0) then
                   if (mask(i1,i2,i3).ne.0) then
                     do n=0,numberOfComponents-1
                       u(i1-is1,i2-is2,i3,n)=(3.*u(i1,i2,i3,n)-3.*u(i1+
     & is1,i2+is2,i3+0,n)+u(i1+2*is1,i2+2*is2,i3+2*0,n))
                     end do
                   end if
                 end if
               end do
             end do
         end if ! ---  end "if( .false.  ... ) then"
         ! =====================================================================================================
         ! =====================================================================================================
         if ( .false. ) then
            n1a=gridIndexRange(0,0)
            n1b=gridIndexRange(1,0)
            n2a=gridIndexRange(0,1)
            n2b=gridIndexRange(1,1)
            i1=n1a-1
            i2=n2a
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,uc,u(i1,i2,i3,uc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,vc,u(i1,i2,i3,vc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v1c,u(i1,i2,i3,v1c))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v2c,u(i1,i2,i3,v2c))
            i1=n1a-1
            i2=n2b
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,uc,u(i1,i2,i3,uc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,vc,u(i1,i2,i3,vc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v1c,u(i1,i2,i3,v1c))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v2c,u(i1,i2,i3,v2c))
            i1=n1b+1
            i2=n2a
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,uc,u(i1,i2,i3,uc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,vc,u(i1,i2,i3,vc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v1c,u(i1,i2,i3,v1c))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v2c,u(i1,i2,i3,v2c))
            i1=n1b+1
            i2=n2b
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,uc,u(i1,i2,i3,uc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,vc,u(i1,i2,i3,vc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v1c,u(i1,i2,i3,v1c))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v2c,u(i1,i2,i3,v2c))
            i1=n1a
            i2=n2a-1
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,uc,u(i1,i2,i3,uc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,vc,u(i1,i2,i3,vc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v1c,u(i1,i2,i3,v1c))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v2c,u(i1,i2,i3,v2c))
            i1=n1b
            i2=n2a-1
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,uc,u(i1,i2,i3,uc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,vc,u(i1,i2,i3,vc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v1c,u(i1,i2,i3,v1c))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v2c,u(i1,i2,i3,v2c))
            i1=n1a
            i2=n2b+1
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,uc,u(i1,i2,i3,uc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,vc,u(i1,i2,i3,vc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v1c,u(i1,i2,i3,v1c))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v2c,u(i1,i2,i3,v2c))
            i1=n1b
            i2=n2b+1
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,uc,u(i1,i2,i3,uc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,vc,u(i1,i2,i3,vc))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v1c,u(i1,i2,i3,v1c))
            call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.0,
     & t,v2c,u(i1,i2,i3,v2c))
         end if
  ! TEMP TEMP TEMP TEMP
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
             if( boundaryCondition(side,axis).eq.displacementBC )then
              ! *************** Displacement BC *****************
              ! ..step 0: Dirichlet bcs for displacement and velocity
               i3=n3a
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
               if (mask(i1,i2,i3).ne.0) then
                u(i1,i2,i3,uc) =bcf(side,axis,i1,i2,i3,uc)    ! given displacements
                u(i1,i2,i3,vc) =bcf(side,axis,i1,i2,i3,vc)
                u(i1,i2,i3,v1c)=bcf(side,axis,i1,i2,i3,v1c)   ! given velocities
                u(i1,i2,i3,v2c)=bcf(side,axis,i1,i2,i3,v2c)
                !call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,uc,ue)
                !write(*,'(" i1,i2=",2i3," u,ue=",2e10.2)') i1,i2,u(i1,i2,i3,uc),ue
               end if
               end do
               end do
             else if( boundaryCondition(side,axis).eq.tractionBC )then
              if( applyInterfaceBoundaryConditions.eq.0 .and. 
     & interfaceType(side,axis,grid).eq.tractionInterface )then
               write(*,'("SMBC: skip traction BC on an interface, (
     & side,axis,grid)=(",3i3,")")') side,axis,grid
              else
               ! ********* Traction BC ********
               ! put "dirichlet parts of the traction BC here
              if( debug.gt.3. .and. interfaceType(side,axis,grid)
     & .eq.tractionInterface )then
               write(*,'("SMBC:INFO: assignPrimaryDirichletBC for an 
     & interface, (side,axis,grid)=(",3i3,")")') side,axis,grid
              end if
              if( gridType.eq.rectangular )then
                if (bctype.eq.linearBoundaryCondition) then      ! linear
                  ! new
                  if( axis.eq.0 )then
                    i3=n3a
                    do i2=nn2a,nn2b
                    do i1=nn1a,nn1b
                    if (mask(i1,i2,i3).ne.0) then
                     ! set normal components of the stress, n=(-is,0)
                      f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                      f2=bcf(side,axis,i1,i2,i3,s12c)
                      f1=f1+is*u(i1,i2,i3,s11c)
                      f2=f2+is*u(i1,i2,i3,s12c)
                      u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)-is*f1
                      u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)-is*f2
                      u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)-is*f2
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s11c,tau11)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s21c,tau21)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s12c,tau12)
                      ! if (abs(tau11-u(i1,i2,i3,s11c)).gt.1.e-14) then
                      !   write(6,*)i1,i2,i3,t,s11c,abs(tau11-u(i1,i2,i3,s11c))
                      !   pause
                      ! end if
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s22c,tau22)
                      !  write(6,'(2(1x,i2),4(1x,f8.4),/,6x,4(1x,f8.4))')i1,i2,u(i1,i2,i3,s11c),u(i1,i2,i3,s12c),u(i1,i2,i3,s21c),u(i1,i2,i3,s22c),tau11,tau12,tau21,tau22
                      !  333            format(2(1x,i2),4(1x,f8.4),/,6x,4(1x,f8.4))
                    end if
                    end do
                    end do
                  else
                    i3=n3a
                    do i2=nn2a,nn2b
                    do i1=nn1a,nn1b
                    if (mask(i1,i2,i3).ne.0) then
                     ! set normal components of the stress, n=(0,-is)
                      f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                      f2=bcf(side,axis,i1,i2,i3,s12c)
                      f1=f1+is*u(i1,i2,i3,s21c)
                      f2=f2+is*u(i1,i2,i3,s22c)
                      !   u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)
                      u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)-is*f1
                      u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)-is*f1
                      u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)-is*f2
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s11c,tau11)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s21c,tau21)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s12c,tau12)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s22c,tau22)
                      ! write(6,'(2(1x,i2),4(1x,f8.4),/,6x,4(1x,f8.4))')i1,i2,u(i1,i2,i3,s11c),u(i1,i2,i3,s12c),u(i1,i2,i3,s21c),u(i1,i2,i3,s22c),tau11,tau12,tau21,tau22
                    end if
                    end do
                    end do
                  end if
                else    ! SVK
                  if( axis.eq.0 )then
                    i3=n3a
                    do i2=nn2a,nn2b
                    do i1=nn1a,nn1b
                    if (mask(i1,i2,i3).ne.0) then
                     ! set normal components of the stress, n=(-is,0)
                     u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(1)
     & )
                     u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(1)
     & )
                     alpha=sqrt(u1y**2+(1.0+u2y)**2)
                     u(i1,i2,i3,s11c) =-is*bcf(side,axis,i1,i2,i3,s11c)
     & *alpha
                     u(i1,i2,i3,s12c) =-is*bcf(side,axis,i1,i2,i3,s12c)
     & *alpha
          !!      write(*,'(" primary: set i1,i2,i3 alpha, bc, s11=",3i4,3e16.8)')  i1,i2,i3,alpha,bcf(side,axis,i1,i2,i3,s11c),u(i1,i2,i3,s11c)        
          !!      write(*,'(" primary: u,v=",4e16.8)') u(i1,i2+1,i3,uc),u(i1,i2-1,i3,uc),u(i1,i2+1,i3,vc),u(i1,i2-1,i3,vc)
                    end if
                    end do
                    end do
                  else
                    i3=n3a
                    do i2=nn2a,nn2b
                    do i1=nn1a,nn1b
                    if (mask(i1,i2,i3).ne.0) then
                     ! set normal components of the stress, n=(0,-is)
                     u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(0)
     & )
                     u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(0)
     & )
                     alpha=sqrt((1.0+u1x)**2+u2x**2)
                     u(i1,i2,i3,s21c) =-is*bcf(side,axis,i1,i2,i3,s11c)
     & *alpha
                     u(i1,i2,i3,s22c) =-is*bcf(side,axis,i1,i2,i3,s12c)
     & *alpha
                    end if
                    end do
                    end do
                  end if
                end if
              else  ! curvilinear
                if (bctype.eq.linearBoundaryCondition) then   ! linear
                  ! new
                   i3=n3a
                   do i2=nn2a,nn2b
                   do i1=nn1a,nn1b
                   if (mask(i1,i2,i3).ne.0) then
                    f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                    f2=bcf(side,axis,i1,i2,i3,s12c)
                    ! (an1,an2) = outward normal 
                    aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2))
                    an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                    an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                    f1=f1-(an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,i3,s21c))
                    f2=f2-(an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,i3,s22c))
                    b1=((1.0+an2**2)*f1-an1*an2*f2)/2.0
                    b2=((1.0+an1**2)*f2-an1*an2*f1)/2.0
                    u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+2.0*b1*an1
                    u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+b2*an1+b1*an2
                    u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+b2*an1+b1*an2
                    u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+2.0*b2*an2
                   end if
                   end do
                   end do
                else     ! SVK
                  if (axis.eq.0) then
                     i3=n3a
                     do i2=nn2a,nn2b
                     do i1=nn1a,nn1b
                      if (mask(i1,i2,i3).ne.0) then
                        ! (an1,an2) = outward normal 
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+
     & rx(i1,i2,i3,axis,1)**2))
                        an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                        an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                        u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*
     & dr(1))
                        u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*
     & dr(1))
                        alpha=sqrt((rx(i1,i2,i3,0,1)-u1s/det(i1,i2,i3))
     & **2+(rx(i1,i2,i3,0,0)+u2s/det(i1,i2,i3))**2)*aNormi
                        f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                        f2=bcf(side,axis,i1,i2,i3,s12c)
                        b1=f1*alpha-(an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,
     & i3,s21c))
                        b2=f2*alpha-(an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,
     & i3,s22c))
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+an1*b1
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+an1*b2
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+an2*b1
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+an2*b2
                      end if
                     end do
                     end do
                  else
                     i3=n3a
                     do i2=nn2a,nn2b
                     do i1=nn1a,nn1b
                      if (mask(i1,i2,i3).ne.0) then
                        ! (an1,an2) = outward normal 
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+
     & rx(i1,i2,i3,axis,1)**2))
                        an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                        an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                        u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*
     & dr(0))
                        u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*
     & dr(0))
                        alpha=sqrt((rx(i1,i2,i3,1,1)+u1r/det(i1,i2,i3))
     & **2+(rx(i1,i2,i3,1,0)-u2r/det(i1,i2,i3))**2)*aNormi
                        f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                        f2=bcf(side,axis,i1,i2,i3,s12c)
                        b1=f1*alpha-(an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,
     & i3,s21c))
                        b2=f2*alpha-(an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,
     & i3,s22c))
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+an1*b1
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+an1*b2
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+an2*b1
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+an2*b2
                      end if
                     end do
                     end do
                  end if
                end if
              end if  ! end gridType
              end if ! not interface
             else if( boundaryCondition(side,axis).eq.slipWall )then
               ! ********* SlipWall BC ********
               ! put "dirichlet parts of the slipwall BC here
              if( gridType.eq.rectangular )then
                ! new
                if( axis.eq.0 )then
                  i3=n3a
                  do i2=nn2a,nn2b
                  do i1=nn1a,nn1b
                  if (mask(i1,i2,i3).ne.0) then
                   ! set n.tau.t and the normal component of displacement, n=(-is,0), t=(0,-is)
                   u(i1,i2,i3,s12c) = bcf(side,axis,i1,i2,i3,s11c)
                   u(i1,i2,i3,s21c) = bcf(side,axis,i1,i2,i3,s11c)
                   u(i1,i2,i3,uc) = -is*bcf(side,axis,i1,i2,i3,uc)
                   u(i1,i2,i3,v1c) = -is*bcf(side,axis,i1,i2,i3,v1c)
                  end if
                  end do
                  end do
                else
                  i3=n3a
                  do i2=nn2a,nn2b
                  do i1=nn1a,nn1b
                  if (mask(i1,i2,i3).ne.0) then
                   ! set n.tau.t and the normal component of displacement, n=(0,-is), t=(+is,0)
                   u(i1,i2,i3,s12c) = -bcf(side,axis,i1,i2,i3,s11c)
                   u(i1,i2,i3,s21c) = -bcf(side,axis,i1,i2,i3,s11c)
                   u(i1,i2,i3,vc) = -is*bcf(side,axis,i1,i2,i3,uc)
                   u(i1,i2,i3,v2c) = -is*bcf(side,axis,i1,i2,i3,v1c)
                  end if
                  end do
                  end do
                end if
              else  ! curvilinear
                ! new
                 i3=n3a
                 do i2=nn2a,nn2b
                 do i1=nn1a,nn1b
                 if (mask(i1,i2,i3).ne.0) then
                  f1=bcf(side,axis,i1,i2,i3,s11c)              ! given tangential traction force
                  f2=bcf(side,axis,i1,i2,i3,uc)                ! given normal displacement
                  f3=bcf(side,axis,i1,i2,i3,v1c)               ! given normal velocity
                  ! (an1,an2) = outward normal and (-an2,an1) = unit tangent
                  aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,
     & i2,i3,axis,1)**2))
                  an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                  an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                  b1=f1-an1*(-u(i1,i2,i3,s11c)*an2+u(i1,i2,i3,s12c)*
     & an1)-an2*(-u(i1,i2,i3,s21c)*an2+u(i1,i2,i3,s22c)*an1)
                  b2=f2-an1*u(i1,i2,i3,uc)-an2*u(i1,i2,i3,vc)
                  b3=f3-an1*u(i1,i2,i3,v1c)-an2*u(i1,i2,i3,v2c)
                  u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)-2.0*b1*an1*an2
                  u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+b1*(an1**2-an2**2)
                  u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+b1*(an1**2-an2**2)
                  u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+2.0*b1*an1*an2
                  u(i1,i2,i3,uc)=u(i1,i2,i3,uc)+an1*b2
                  u(i1,i2,i3,vc)=u(i1,i2,i3,vc)+an2*b2
                  u(i1,i2,i3,v1c)=u(i1,i2,i3,v1c)+an1*b3
                  u(i1,i2,i3,v2c)=u(i1,i2,i3,v2c)+an2*b3
                 end if
                 end do
                 end do
              end if  ! end gridType
             end if ! bc
             end do ! end side
             end do ! end axis
            !  Note: it does not appear to be possible to set the components of stress in the corner
            !        if one of the sides is a slipwall bc
            !  Note: new implementation of traction bcs for SVK case leads to Dirichlet bcs for *all*
            !        components of stress on the boundary.  (Two components are set by the physical
            !        bcs and the other two are set by compatibility conditions.)  Thus, no corner
            !        stress fix is needed for the SVK case if any bc is a traction bc.  DWS, 2/28/12
            ! 
            !  Update: the above is not true.  DWS, 3/28/12.  :)
            !
            !  Additional changes:  DWS, 7/8/15
            !    The mixed displacement-traction corner cases for the nonlinear (SVK) cases now
            !    set the tangent components of the stress in the corner and set ghost points
            !    for displacement and velocity.  The basic configuration is this.  If the North
            !    face is traction while the East face is displacement, then ghost points for
            !    displacement and velocity would be set in the first ghost line to the east of
            !    of the corner.  The displacement and velocity in the first ghost line to the
            !    north of the corner are known already because of the displacement bcs.  So,
            !    by setting the east ghost points, centered differences of displacement lead
            !    to compatible stress components in the corner. 
            i3=gridIndexRange(0,2)
            if (gridType.eq.rectangular) then
              ! -----------------------------------------------------------------------
              ! --------------------- CARTESIAN FIXUP CORNER STRESS -------------------
              ! -----------------------------------------------------------------------
              do side1=0,1
                i1=gridIndexRange(side1,axis1)
                do side2=0,1
                  i2=gridIndexRange(side2,axis2)
                  if (mask(i1,i2,i3).ne.0) then
                    if (bctype.eq.linearBoundaryCondition) then    ! linear case only
                      if (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                        mu=mupc(i1,i2)
                        lambda=lambdapc(i1,i2)
                      elseif (
     & materialFormat.eq.variableMaterialProperties) then
                        mu=muv(i1,i2)
                        lambda=lambdav(i1,i2)
                      end if
                    end if
                    if (boundaryCondition(side1,axis1)
     & .eq.displacementBC.and.boundaryCondition(side2,axis2)
     & .eq.displacementBC) then
                      !  Cartesian grid, pure displacement/velocity bcs
                      u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(
     & 0))
                      u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(
     & 0))
                      u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(
     & 1))
                      u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(
     & 1))
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        u(i1,i2,i3,s11c)=lambda*(u1x+u2y)+2.0*mu*u1x
                        u(i1,i2,i3,s12c)=mu*(u1y+u2x)
                        u(i1,i2,i3,s21c)=mu*(u1y+u2x)
                        u(i1,i2,i3,s22c)=lambda*(u1x+u2y)+2.0*mu*u2y
                      else                                             
     &   ! SVK case
                        !   call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,0)
                        du(1,1)=u1x
                        du(1,2)=u1y
                        du(2,1)=u2x
                        du(2,2)=u2y
                        ideriv=0
                        call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                        u(i1,i2,i3,s11c)=p(1,1)
                        u(i1,i2,i3,s12c)=p(1,2)
                        u(i1,i2,i3,s21c)=p(2,1)
                        u(i1,i2,i3,s22c)=p(2,2)
                      end if
                   elseif (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC) then
                   !  Cartesian grid, pure traction bcs
                   !  No fix is needed since the normals for (side1,axis1) and (side2,axis2) are orthogonal (linear case only)
                      if (bctype.ne.linearBoundaryCondition) then    ! SVK case
                        !  initialize
                        is1=1-2*side1
                        is2=1-2*side2
                        u1x0=is1*(u(i1+is1,i2,i3,uc)-u(i1,i2,i3,uc))
     & /dx(0)
                        u2x0=is1*(u(i1+is1,i2,i3,vc)-u(i1,i2,i3,vc))
     & /dx(0)
                        u1y0=is2*(u(i1,i2+is2,i3,uc)-u(i1,i2,i3,uc))
     & /dx(1)
                        u2y0=is2*(u(i1,i2+is2,i3,vc)-u(i1,i2,i3,vc))
     & /dx(1)
                        u1x=u1x0
                        u2x=u2x0
                        u1y=u1y0
                        u2y=u2y0
                        ! u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(0))
                        ! u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(0))
                        ! u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(1))
                        ! u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(1))
                        ! Newton iteration for u1x,u2x,u1y,u2y
                        ier=0
                        iter=1
                        istop=0
                        bmax=10.*toler
                        do while (bmax.gt.toler)
                         !  compute stress and the deriv based on current deformation gradient
                         !   ideriv=1
                         !   call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1x
                          du(1,2)=u1y
                          du(2,1)=u2x
                          du(2,2)=u2y
                          ideriv=1
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          alpha1=sqrt(u1y**2+(1.0+u2y)**2)
                          ! given traction forces (adjust here for sign of normal)
                          f11=-is1*bcf(side1,axis1,i1,i2,i3,uc)*alpha1
                          f21=-is1*bcf(side1,axis1,i1,i2,i3,vc)*alpha1
                          dalpha11=u1y/alpha1
                          dalpha12=(1.0+u2y)/alpha1
                          alpha2=sqrt((1.0+u1x)**2+u2x**2)
                          ! given traction forces (adjust here for sign of normal)
                          f12=-is2*bcf(side2,axis2,i1,i2,i3,uc)*alpha2
                          f22=-is2*bcf(side2,axis2,i1,i2,i3,vc)*alpha2
                          dalpha21=(1.0+u1x)/alpha2
                          dalpha22=u2x/alpha2
                          !  set up the 4x4 system
                          bb(1)=p(1,1)-f11
                          bb(2)=p(1,2)-f21
                          bb(3)=p(2,1)-f12
                          bb(4)=p(2,2)-f22
                          aa(1,1)=dpdf(1,1)
                          aa(1,2)=dpdf(1,2)+is1*bcf(side1,axis1,i1,i2,
     & i3,uc)*dalpha11
                          aa(1,3)=dpdf(1,3)
                          aa(1,4)=dpdf(1,4)+is1*bcf(side1,axis1,i1,i2,
     & i3,uc)*dalpha12
                          aa(2,1)=dpdf(2,1)
                          aa(2,2)=dpdf(2,2)+is1*bcf(side1,axis1,i1,i2,
     & i3,vc)*dalpha11
                          aa(2,3)=dpdf(2,3)
                          aa(2,4)=dpdf(2,4)+is1*bcf(side1,axis1,i1,i2,
     & i3,vc)*dalpha12
                          aa(3,1)=dpdf(3,1)+is2*bcf(side2,axis2,i1,i2,
     & i3,uc)*dalpha21
                          aa(3,2)=dpdf(3,2)
                          aa(3,3)=dpdf(3,3)+is2*bcf(side2,axis2,i1,i2,
     & i3,uc)*dalpha22
                          aa(3,4)=dpdf(3,4)
                          aa(4,1)=dpdf(4,1)+is2*bcf(side2,axis2,i1,i2,
     & i3,vc)*dalpha21
                          aa(4,2)=dpdf(4,2)
                          aa(4,3)=dpdf(4,3)+is2*bcf(side2,axis2,i1,i2,
     & i3,vc)*dalpha22
                          aa(4,4)=dpdf(4,4)
                          !  solve the 4x4 system
                          bmax=max(abs(bb(1)),abs(bb(2)),abs(bb(3)),
     & abs(bb(4)))/lambda
                          call smsolve (aa,bb,ier)
                          if (istop.ne.0) then
                            write(6,'(1x,i2,5(1x,1pe15.8))')iter,bb(1),
     & bb(2),bb(3),bb(4),bmax
                          end if
                          !  update
                          u1x=u1x-bb(1)
                          u1y=u1y-bb(2)
                          u2x=u2x-bb(3)
                          u2y=u2y-bb(4)
                          iter=iter+1
                          !  check for convergence
                          if (iter.gt.itmax.or.ier.ne.0) then
                            write(6,*)'Error (bcOptSmFOS) : Newton 
     & failed to converge'
                            if (istop.eq.0) then
                              ier=0
                              iter=1
                              istop=1
                              u1x=u1x0
                              u2x=u2x0
                              u1y=u1y0
                              u2y=u2y0
                            else
                              stop 8881
                            end if
                          end if
                        end do
                        !  set displacement in the ghost point
                        u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*
     & dx(0)*u1x
                        u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*
     & dx(0)*u2x
                        u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is2*
     & dx(1)*u1y
                        u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is2*
     & dx(1)*u2y
                      end if   ! end bctype
                    elseif (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.displacementBC .and. fixupTractionDisplacementCorners ) 
     & then
                      !  Cartesian grid, mix bcs, case 1
                      u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(
     & 0))
                      u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(
     & 0))
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        u1y=(u(i1,i2,i3,s12c)-mu*u2x)/mu
                        u2y=(u(i1,i2,i3,s11c)-(lambda+2.0*mu)*u1x)
     & /lambda
                        u(i1,i2,i3,s22c)=lambda*(u1x+u2y)+2.0*mu*u2y
                        !  write(6,*)'here (1), side1,side2=',side1,side2
                        !  write(6,*)boundaryCondition(0,0),boundaryCondition(1,0),boundaryCondition(0,1),boundaryCondition(1,1)
                        !  pause
                      else                                             
     &                          ! nonlinear case
                        if (.true.) then   ! true/false switch here is for testing Cartesian grids
                        !  initialize
                        is1=1-2*side1
                        is2=1-2*side2
                        u1y0=is2*(u(i1,i2+is2,i3,uc)-u(i1,i2,i3,uc))
     & /dx(1)
                        u2y0=is2*(u(i1,i2+is2,i3,vc)-u(i1,i2,i3,vc))
     & /dx(1)
                        u1y=u1y0
                        u2y=u2y0
                        ! Newton iteration for u1y,u2y
                        ier=0
                        iter=1
                        istop=0
                        bmax=10.*toler
                        do while (bmax.gt.toler)
                         !  compute stress and the deriv based on current deformation gradient
                         !   ideriv=1
                         !   call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1x
                          du(1,2)=u1y
                          du(2,1)=u2x
                          du(2,2)=u2y
                          ideriv=1
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          alpha1=sqrt(u1y**2+(1.0+u2y)**2)
                          ! given traction forces (adjust here for sign of normal)
                          f11=-is1*bcf(side1,axis1,i1,i2,i3,uc)*alpha1
                          f21=-is1*bcf(side1,axis1,i1,i2,i3,vc)*alpha1
                          dalpha11=u1y/alpha1
                          dalpha12=(1.0+u2y)/alpha1
                          !  set up the 2x2 system
                          bb(1)=p(1,1)-f11
                          bb(2)=p(1,2)-f21
                          aa(1,1)=dpdf(1,2)+is1*bcf(side1,axis1,i1,i2,
     & i3,uc)*dalpha11
                          aa(1,2)=dpdf(1,4)+is1*bcf(side1,axis1,i1,i2,
     & i3,uc)*dalpha12
                          aa(2,1)=dpdf(2,2)+is1*bcf(side1,axis1,i1,i2,
     & i3,vc)*dalpha11
                          aa(2,2)=dpdf(2,4)+is1*bcf(side1,axis1,i1,i2,
     & i3,vc)*dalpha12
                          !  solve the 2x2 system
                          determ=aa(1,1)*aa(2,2)-aa(1,2)*aa(2,1)
                          du1y=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                          du2y=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                          bmax=max(abs(bb(1)),abs(bb(2)))/lambda
                          if (istop.ne.0) then
                            write(6,'(1x,i2,3(1x,1pe15.8))')iter,bb(1),
     & bb(2),bmax
                          end if
                          !  update
                          u1y=u1y-du1y
                          u2y=u2y-du2y
                          iter=iter+1
                          !  check for convergence
                          if (iter.gt.itmax) then
                            write(6,*)'Error (bcOptSmFOS) : Newton 
     & failed to converge'
                            if (istop.eq.0) then
                              ier=0
                              iter=1
                              istop=1
                              u1y=u1y0
                              u2y=u2y0
                            else
                              stop 7881
                            end if
                          end if
                        end do
                        !  set displacement in the ghost point and the tangent components of stress
                        u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is2*
     & dx(1)*u1y
                        u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is2*
     & dx(1)*u2y
                        u(i1,i2,i3,s21c)=p(2,1)
                        u(i1,i2,i3,s22c)=p(2,2)
                        !  compute v1y and v2y
                        v1x=(u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,v1c))/(2.*
     & dx(0))
                        v2x=(u(i1+1,i2,i3,v2c)-u(i1-1,i2,i3,v2c))/(2.*
     & dx(0))
                        bb(1)=-dpdf(1,1)*v1x-dpdf(1,3)*v2x-is1*bcf(
     & side1,axis1,i1,i2,i3,v1c)*alpha1
                        bb(2)=-dpdf(2,1)*v1x-dpdf(2,3)*v2x-is1*bcf(
     & side1,axis1,i1,i2,i3,v2c)*alpha1
                        v1y=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                        v2y=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                        !  set velocity in the ghost point
                        u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)-2.*is2*
     & dx(1)*v1y
                        u(i1,i2-is2,i3,v2c)=u(i1,i2+is2,i3,v2c)-2.*is2*
     & dx(1)*v2y
                        else    ! else true/false testing
                        is2=1-2*side2
                        u1y=0.
                        u2y=0.
                        u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is2*
     & dx(1)*u1y
                        u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is2*
     & dx(1)*u2y
                        u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)
                        u(i1,i2-is2,i3,v2c)=u(i1,i2+is2,i3,v2c)
                        u(i1,i2,i3,s11c)=0.
                        u(i1,i2,i3,s12c)=0.
                        u(i1,i2,i3,s21c)=0.
                        u(i1,i2,i3,s22c)=0.
                        end if   ! end true/false testing
                      end if
                    elseif (boundaryCondition(side1,axis1)
     & .eq.displacementBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC .and. fixupTractionDisplacementCorners ) then
                      ! Cartesian grid, mix bcs, case 2
                      u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(
     & 1))
                      u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(
     & 1))
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        u1x=(u(i1,i2,i3,s22c)-(lambda+2.0*mu)*u2y)
     & /lambda
                        u2x=(u(i1,i2,i3,s21c)-mu*u1y)/mu
                        u(i1,i2,i3,s11c)=lambda*(u1x+u2y)+2.0*mu*u1x
                        !  write(6,*)'here (2), side1,side2=',side1,side2
                        !  pause
                      else                                             
     &                          ! nonlinear case
                        if (.true.) then   ! true/false switch here is for testing Cartesian grids
                        !  initialize
                        is1=1-2*side1
                        is2=1-2*side2
                        u1x0=is1*(u(i1+is1,i2,i3,uc)-u(i1,i2,i3,uc))
     & /dx(0)
                        u2x0=is1*(u(i1+is1,i2,i3,vc)-u(i1,i2,i3,vc))
     & /dx(0)
                        u1x=u1x0
                        u2x=u2x0
                        ! Newton iteration for u1x,u2x
                        ier=0
                        iter=1
                        istop=0
                        bmax=10.*toler
                        do while (bmax.gt.toler)
                         !  compute stress and the deriv based on current deformation gradient
                         !   ideriv=1
                         !   call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1x
                          du(1,2)=u1y
                          du(2,1)=u2x
                          du(2,2)=u2y
                          ideriv=1
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          alpha2=sqrt((1.0+u1x)**2+u2x**2)
                          ! given traction forces (adjust here for sign of normal)
                          f12=-is2*bcf(side2,axis2,i1,i2,i3,uc)*alpha2
                          f22=-is2*bcf(side2,axis2,i1,i2,i3,vc)*alpha2
                          dalpha21=(1.0+u1x)/alpha2
                          dalpha22=u2x/alpha2
                          !  set up the 2x2 system
                          bb(1)=p(2,1)-f12
                          bb(2)=p(2,2)-f22
                          aa(1,1)=dpdf(3,1)+is2*bcf(side2,axis2,i1,i2,
     & i3,uc)*dalpha21
                          aa(1,2)=dpdf(3,3)+is2*bcf(side2,axis2,i1,i2,
     & i3,uc)*dalpha22
                          aa(2,1)=dpdf(4,1)+is2*bcf(side2,axis2,i1,i2,
     & i3,vc)*dalpha21
                          aa(2,2)=dpdf(4,3)+is2*bcf(side2,axis2,i1,i2,
     & i3,vc)*dalpha22
                          !  solve the 2x2 system
                          determ=aa(1,1)*aa(2,2)-aa(1,2)*aa(2,1)
                          du1x=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                          du2x=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                          bmax=max(abs(bb(1)),abs(bb(2)))/lambda
                          if (istop.ne.0) then
                            write(6,'(1x,i2,3(1x,1pe15.8))')iter,bb(1),
     & bb(2),bmax
                          end if
                          !  update
                          u1x=u1x-du1x
                          u2x=u2x-du2x
                          iter=iter+1
                          !  check for convergence
                          if (iter.gt.itmax) then
                            write(6,*)'Error (bcOptSmFOS) : Newton 
     & failed to converge'
                            if (istop.eq.0) then
                              ier=0
                              iter=1
                              istop=1
                              u1x=u1x0
                              u2x=u2x0
                            else
                              stop 7882
                            end if
                          end if
                        end do
                        !  set displacement in the ghost point and the tangent components of stress
                        u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*
     & dx(0)*u1x
                        u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*
     & dx(0)*u2x
                        u(i1,i2,i3,s11c)=p(1,1)
                        u(i1,i2,i3,s12c)=p(1,2)
                        !  compute v1x and v2x
                        v1y=(u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,v1c))/(2.*
     & dx(1))
                        v2y=(u(i1,i2+1,i3,v2c)-u(i1,i2-1,i3,v2c))/(2.*
     & dx(1))
                        bb(1)=-dpdf(3,2)*v1y-dpdf(3,4)*v2y-is2*bcf(
     & side2,axis2,i1,i2,i3,v1c)*alpha2
                        bb(2)=-dpdf(4,2)*v1y-dpdf(4,4)*v2y-is2*bcf(
     & side2,axis2,i1,i2,i3,v2c)*alpha2
                        v1x=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                        v2x=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                        !  set velocity in the ghost point
                        u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)-2.*is1*
     & dx(0)*v1x
                        u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)-2.*is1*
     & dx(0)*v2x
c              u1x=0.
c              u2x=0.
c              u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*dx(0)*u1x
c              u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*dx(0)*u2x
c              u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)
c              u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)
c              u(i1,i2,i3,s11c)=0.
c              u(i1,i2,i3,s12c)=0.
c              u(i1,i2,i3,s21c)=0.
c              u(i1,i2,i3,s22c)=0.
                        else   ! else true/false testing
                        is1=1-2*side1
                        u1x=0.
                        u2x=0.
                        u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*
     & dx(0)*u1x
                        u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*
     & dx(0)*u2x
                        u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)
                        u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)
                        u(i1,i2,i3,s11c)=0.
                        u(i1,i2,i3,s12c)=0.
                        u(i1,i2,i3,s21c)=0.
                        u(i1,i2,i3,s22c)=0.
                        end if   ! end true/false testing
                      end if
                    end if
                  end if
                end do
              end do
            else    ! non-Cartesian cases
              ! -----------------------------------------------------------------------
              ! ------------------- CURVILINEAR FIXUP CORNER STRESS -------------------
              ! -----------------------------------------------------------------------
              do side1=0,1
                i1=gridIndexRange(side1,axis1)
                do side2=0,1
                  i2=gridIndexRange(side2,axis2)
                  if (mask(i1,i2,i3).ne.0) then
                    if (bctype.eq.linearBoundaryCondition) then    ! linear case only
                      if (
     & materialFormat.eq.piecewiseConstantMaterialProperties) then
                        mu=mupc(i1,i2)
                        lambda=lambdapc(i1,i2)
                      elseif (
     & materialFormat.eq.variableMaterialProperties) then
                        mu=muv(i1,i2)
                        lambda=lambdav(i1,i2)
                      end if
                    end if
                    if (boundaryCondition(side1,axis1)
     & .eq.displacementBC.and.boundaryCondition(side2,axis2)
     & .eq.displacementBC) then
                      ! non-Cartesian grid, pure displacement/velocity bcs
                      u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(
     & 0))
                      u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(
     & 0))
                      u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(
     & 1))
                      u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(
     & 1))
                      u1x=u1r*rx(i1,i2,i3,0,0)+u1s*rx(i1,i2,i3,1,0)
                      u2x=u2r*rx(i1,i2,i3,0,0)+u2s*rx(i1,i2,i3,1,0)
                      u1y=u1r*rx(i1,i2,i3,0,1)+u1s*rx(i1,i2,i3,1,1)
                      u2y=u2r*rx(i1,i2,i3,0,1)+u2s*rx(i1,i2,i3,1,1)
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        u(i1,i2,i3,s11c)=lambda*(u1x+u2y)+2.0*mu*u1x
                        u(i1,i2,i3,s12c)=mu*(u1y+u2x)
                        u(i1,i2,i3,s21c)=mu*(u1y+u2x)
                        u(i1,i2,i3,s22c)=lambda*(u1x+u2y)+2.0*mu*u2y
                      else                                             
     &   ! SVK case
                        !  call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,0)
                        du(1,1)=u1x
                        du(1,2)=u1y
                        du(2,1)=u2x
                        du(2,2)=u2y
                        ideriv=0
                        call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                        u(i1,i2,i3,s11c)=p(1,1)
                        u(i1,i2,i3,s12c)=p(1,2)
                        u(i1,i2,i3,s21c)=p(2,1)
                        u(i1,i2,i3,s22c)=p(2,2)
                      end if
                    elseif (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC) then
                      ! non-Cartesian grid, pure traction bcs.  For the newBCs case, there is nothing to be done for traction-traction
                      ! corners.  Here is the situation.  Assuming that the stress tensor is symmetric, there are 3 components to set,
                      ! i.e. s11, s12=s21 and s22.  Two bcs would be used for one traction side and two bcs would be used for the other
                      ! traction side.  This makes 4 bcs at the corner.  Suppose sigma.n=f for one side and sigma.m=g for the other.  The
                      ! compatibility condition is m.f=n.g.  If this condition is satisfied, then it does matter which traction bc on a
                      ! side is applied first.  When the other traction bc is applied, it does not destroy the bcs already applied.  (I have
                      ! checked the algebra on this, DWS 12/4/10)
                      ! non-Cartesian grid, pure traction bcs (needed since grid lines may not be orthogonal)
                      if (bctype.eq.linearBoundaryCondition) then   ! linear
                      else    ! SVK
                       if (.false.) then  ! old stuff
                        is=1-2*side1
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis1,0)**
     & 2+rx(i1,i2,i3,axis1,1)**2))
                        an11=-is*rx(i1,i2,i3,axis1,0)*aNormi          ! normals for axis1,side1
                        an21=-is*rx(i1,i2,i3,axis1,1)*aNormi
                        u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*
     & dr(1))
                        u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*
     & dr(1))
                        alpha=sqrt((rx(i1,i2,i3,0,1)-u1s/det(i1,i2,i3))
     & **2+(rx(i1,i2,i3,0,0)+u2s/det(i1,i2,i3))**2)*aNormi
                        f11=bcf(side1,axis1,i1,i2,i3,s11c)*alpha      ! given traction forces for axis1,side1
                        f21=bcf(side1,axis1,i1,i2,i3,s12c)*alpha
                        is=1-2*side2
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis2,0)**
     & 2+rx(i1,i2,i3,axis2,1)**2))
                        an12=-is*rx(i1,i2,i3,axis2,0)*aNormi          ! normals for axis2,side2
                        an22=-is*rx(i1,i2,i3,axis2,1)*aNormi
                        u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*
     & dr(0))
                        u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*
     & dr(0))
                        alpha=sqrt((rx(i1,i2,i3,1,1)+u1r/det(i1,i2,i3))
     & **2+(rx(i1,i2,i3,1,0)-u2r/det(i1,i2,i3))**2)*aNormi
                        f12=bcf(side2,axis2,i1,i2,i3,s11c)*alpha      ! given traction forces for axis2,side2
                        f22=bcf(side2,axis2,i1,i2,i3,s12c)*alpha
                        b11=f11-(an11*u(i1,i2,i3,s11c)+an21*u(i1,i2,i3,
     & s21c))
                        b21=f21-(an11*u(i1,i2,i3,s12c)+an21*u(i1,i2,i3,
     & s22c))
                        dot1=an11*an12+an21*an22                      ! cosine of the angle between the normals
                        dot2=an21*an12-an11*an22                      ! cosine of the angle between tangent(1) and normal(2)
                        b12=(f12-(an12*u(i1,i2,i3,s11c)+an22*u(i1,i2,
     & i3,s21c))-dot1*b11)/dot2
                        b22=(f22-(an12*u(i1,i2,i3,s12c)+an22*u(i1,i2,
     & i3,s22c))-dot1*b21)/dot2
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+an11*b11+
     & an21*b12
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+an11*b21+
     & an21*b22
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+an21*b11-
     & an11*b12
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+an21*b21-
     & an11*b22
                       else   ! new stuff: iterate on u1r,u1s,u2r,u2s until traction bcs on both sides are satisfied
                        ! initialize
                        is1=1-2*side1
                        aNormi1=1./max(epsx,sqrt(rx(i1,i2,i3,axis1,0)**
     & 2+rx(i1,i2,i3,axis1,1)**2))
                        an11=-is1*rx(i1,i2,i3,axis1,0)*aNormi1          ! normals for axis1,side1
                        an21=-is1*rx(i1,i2,i3,axis1,1)*aNormi1
                        is2=1-2*side2
                        aNormi2=1./max(epsx,sqrt(rx(i1,i2,i3,axis2,0)**
     & 2+rx(i1,i2,i3,axis2,1)**2))
                        an12=-is2*rx(i1,i2,i3,axis2,0)*aNormi2          ! normals for axis2,side2
                        an22=-is2*rx(i1,i2,i3,axis2,1)*aNormi2
                        u1r0=is1*(u(i1+is1,i2,i3,uc)-u(i1,i2,i3,uc))
     & /dr(0)
                        u2r0=is1*(u(i1+is1,i2,i3,vc)-u(i1,i2,i3,vc))
     & /dr(0)
                        u1s0=is2*(u(i1,i2+is2,i3,uc)-u(i1,i2,i3,uc))
     & /dr(1)
                        u2s0=is2*(u(i1,i2+is2,i3,vc)-u(i1,i2,i3,vc))
     & /dr(1)
                        u1r=u1r0
                        u2r=u2r0
                        u1s=u1s0
                        u2s=u2s0
                        ! u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(0))
                        ! u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(0))
                        ! u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(1))
                        ! u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(1))
                        ! Newton iteration for u1r,u2r,u1s,u2s
                        ier=0
                        iter=1
                        istop=0
                        bmax=10.*toler
                        do while (bmax.gt.toler)
                          u1x=rx(i1,i2,i3,0,0)*u1r+rx(i1,i2,i3,1,0)*u1s
                          u1y=rx(i1,i2,i3,0,1)*u1r+rx(i1,i2,i3,1,1)*u1s
                          u2x=rx(i1,i2,i3,0,0)*u2r+rx(i1,i2,i3,1,0)*u2s
                          u2y=rx(i1,i2,i3,0,1)*u2r+rx(i1,i2,i3,1,1)*u2s
                          ! compute stress and the deriv based on current deformation gradient
                          !                      ideriv=1
                          !                      call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1x
                          du(1,2)=u1y
                          du(2,1)=u2x
                          du(2,2)=u2y
                          ideriv=1
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          coef11=rx(i1,i2,i3,0,1)-u1s/det(i1,i2,i3)
                          coef21=rx(i1,i2,i3,0,0)+u2s/det(i1,i2,i3)
                          alpha1=sqrt(coef11**2+coef21**2)*aNormi1
                          f11=bcf(side1,axis1,i1,i2,i3,uc)*alpha1      
     &   ! given traction forces
                          f21=bcf(side1,axis1,i1,i2,i3,vc)*alpha1
                          fact=aNormi1/(det(i1,i2,i3)*sqrt(coef11**2+
     & coef21**2))
                          dalpha11=-coef11*fact
                          dalpha12= coef21*fact
                          coef12=rx(i1,i2,i3,1,1)+u1r/det(i1,i2,i3)
                          coef22=rx(i1,i2,i3,1,0)-u2r/det(i1,i2,i3)
                          alpha2=sqrt(coef12**2+coef22**2)*aNormi2
                          f12=bcf(side2,axis2,i1,i2,i3,uc)*alpha2      
     &    ! given traction forces
                          f22=bcf(side2,axis2,i1,i2,i3,vc)*alpha2
                          fact=aNormi2/(det(i1,i2,i3)*sqrt(coef12**2+
     & coef22**2))
                          dalpha21= coef12*fact
                          dalpha22=-coef22*fact
                          ! construct linear system
                          bb(1)=an11*p(1,1)+an21*p(2,1)-f11
                          bb(2)=an11*p(1,2)+an21*p(2,2)-f21
                          bb(3)=an12*p(1,1)+an22*p(2,1)-f12
                          bb(4)=an12*p(1,2)+an22*p(2,2)-f22
                          aa(1,1)= an11*(dpdf(1,1)*rx(i1,i2,i3,0,0)+
     & dpdf(1,2)*rx(i1,i2,i3,0,1)) +an21*(dpdf(3,1)*rx(i1,i2,i3,0,0)+
     & dpdf(3,2)*rx(i1,i2,i3,0,1))
                          aa(1,2)= an11*(dpdf(1,3)*rx(i1,i2,i3,0,0)+
     & dpdf(1,4)*rx(i1,i2,i3,0,1)) +an21*(dpdf(3,3)*rx(i1,i2,i3,0,0)+
     & dpdf(3,4)*rx(i1,i2,i3,0,1))
                          aa(1,3)= an11*(dpdf(1,1)*rx(i1,i2,i3,1,0)+
     & dpdf(1,2)*rx(i1,i2,i3,1,1)) +an21*(dpdf(3,1)*rx(i1,i2,i3,1,0)+
     & dpdf(3,2)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,uc)*
     & dalpha11
                          aa(1,4)= an11*(dpdf(1,3)*rx(i1,i2,i3,1,0)+
     & dpdf(1,4)*rx(i1,i2,i3,1,1)) +an21*(dpdf(3,3)*rx(i1,i2,i3,1,0)+
     & dpdf(3,4)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,uc)*
     & dalpha12
                          aa(2,1)= an11*(dpdf(2,1)*rx(i1,i2,i3,0,0)+
     & dpdf(2,2)*rx(i1,i2,i3,0,1)) +an21*(dpdf(4,1)*rx(i1,i2,i3,0,0)+
     & dpdf(4,2)*rx(i1,i2,i3,0,1))
                          aa(2,2)= an11*(dpdf(2,3)*rx(i1,i2,i3,0,0)+
     & dpdf(2,4)*rx(i1,i2,i3,0,1)) +an21*(dpdf(4,3)*rx(i1,i2,i3,0,0)+
     & dpdf(4,4)*rx(i1,i2,i3,0,1))
                          aa(2,3)= an11*(dpdf(2,1)*rx(i1,i2,i3,1,0)+
     & dpdf(2,2)*rx(i1,i2,i3,1,1)) +an21*(dpdf(4,1)*rx(i1,i2,i3,1,0)+
     & dpdf(4,2)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,vc)*
     & dalpha11
                          aa(2,4)= an11*(dpdf(2,3)*rx(i1,i2,i3,1,0)+
     & dpdf(2,4)*rx(i1,i2,i3,1,1)) +an21*(dpdf(4,3)*rx(i1,i2,i3,1,0)+
     & dpdf(4,4)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,vc)*
     & dalpha12
                          aa(3,1)= an12*(dpdf(1,1)*rx(i1,i2,i3,0,0)+
     & dpdf(1,2)*rx(i1,i2,i3,0,1)) +an22*(dpdf(3,1)*rx(i1,i2,i3,0,0)+
     & dpdf(3,2)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,uc)*
     & dalpha21
                          aa(3,2)= an12*(dpdf(1,3)*rx(i1,i2,i3,0,0)+
     & dpdf(1,4)*rx(i1,i2,i3,0,1)) +an22*(dpdf(3,3)*rx(i1,i2,i3,0,0)+
     & dpdf(3,4)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,uc)*
     & dalpha22
                          aa(3,3)= an12*(dpdf(1,1)*rx(i1,i2,i3,1,0)+
     & dpdf(1,2)*rx(i1,i2,i3,1,1)) +an22*(dpdf(3,1)*rx(i1,i2,i3,1,0)+
     & dpdf(3,2)*rx(i1,i2,i3,1,1))
                          aa(3,4)= an12*(dpdf(1,3)*rx(i1,i2,i3,1,0)+
     & dpdf(1,4)*rx(i1,i2,i3,1,1)) +an22*(dpdf(3,3)*rx(i1,i2,i3,1,0)+
     & dpdf(3,4)*rx(i1,i2,i3,1,1))
                          aa(4,1)= an12*(dpdf(2,1)*rx(i1,i2,i3,0,0)+
     & dpdf(2,2)*rx(i1,i2,i3,0,1)) +an22*(dpdf(4,1)*rx(i1,i2,i3,0,0)+
     & dpdf(4,2)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,vc)*
     & dalpha21
                          aa(4,2)= an12*(dpdf(2,3)*rx(i1,i2,i3,0,0)+
     & dpdf(2,4)*rx(i1,i2,i3,0,1)) +an22*(dpdf(4,3)*rx(i1,i2,i3,0,0)+
     & dpdf(4,4)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,vc)*
     & dalpha22
                          aa(4,3)= an12*(dpdf(2,1)*rx(i1,i2,i3,1,0)+
     & dpdf(2,2)*rx(i1,i2,i3,1,1)) +an22*(dpdf(4,1)*rx(i1,i2,i3,1,0)+
     & dpdf(4,2)*rx(i1,i2,i3,1,1))
                          aa(4,4)= an12*(dpdf(2,3)*rx(i1,i2,i3,1,0)+
     & dpdf(2,4)*rx(i1,i2,i3,1,1)) +an22*(dpdf(4,3)*rx(i1,i2,i3,1,0)+
     & dpdf(4,4)*rx(i1,i2,i3,1,1))
                          ! solve the 4x4 system
                          bmax=max(abs(bb(1)),abs(bb(2)),abs(bb(3)),
     & abs(bb(4)))/lambda
                          call smsolve (aa,bb,ier)
                          if (istop.ne.0) then
                            write(6,'(1x,i2,5(1x,1pe15.8))')iter,bb(1),
     & bb(2),bb(3),bb(4),bmax
                          end if
                          ! update
                          u1r=u1r-bb(1)
                          u2r=u2r-bb(2)
                          u1s=u1s-bb(3)
                          u2s=u2s-bb(4)
                          iter=iter+1
                          ! check for convergence
                          if (iter.gt.itmax.or.ier.ne.0) then
                            write(6,*)'Error (bcOptSmFOS) : Newton 
     & failed to converge'
                            if (istop.eq.0) then
                              ier=0
                              iter=1
                              istop=1
                              u1r=u1r0
                              u2r=u2r0
                              u1s=u1s0
                              u2s=u2s0
                            else
                              stop 8882
                            end if
                          end if
                        end do
                        ! set displacement in the ghost point
                        u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*
     & dr(0)*u1r
                        u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*
     & dr(0)*u2r
                        u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is2*
     & dr(1)*u1s
                        u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is2*
     & dr(1)*u2s
                       end if   ! end old/new
                      end if
                    elseif (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.displacementBC .and. fixupTractionDisplacementCorners ) 
     & then
                      ! non-Cartesian grid, mix bcs, case 1  (Should be okay for both new and old bcs)
                      is=1-2*side1
                      aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis1,0)**2+
     & rx(i1,i2,i3,axis1,1)**2))
                      an1=-is*rx(i1,i2,i3,axis1,0)*aNormi
                      an2=-is*rx(i1,i2,i3,axis1,1)*aNormi
                      u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dr(
     & 0))
                      u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dr(
     & 0))
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        a11=an1*(lambda+2.0*mu)*rx(i1,i2,i3,axis2,0)+
     & an2*mu*rx(i1,i2,i3,axis2,1)
                        a12=an1*lambda*rx(i1,i2,i3,axis2,1)+an2*mu*rx(
     & i1,i2,i3,axis2,0)
                        b1=an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,i3,s21c)-(
     & an1*(lambda+2.0*mu)*rx(i1,i2,i3,axis1,0)+an2*mu*rx(i1,i2,i3,
     & axis1,1))*u1r-(an1*lambda*rx(i1,i2,i3,axis1,1)+an2*mu*rx(i1,i2,
     & i3,axis1,0))*u2r
                        a21=an1*mu*rx(i1,i2,i3,axis2,1)+an2*lambda*rx(
     & i1,i2,i3,axis2,0)
                        a22=an1*mu*rx(i1,i2,i3,axis2,0)+an2*(lambda+
     & 2.0*mu)*rx(i1,i2,i3,axis2,1)
                        b2=an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,i3,s22c)-(
     & an1*mu*rx(i1,i2,i3,axis1,1)+an2*lambda*rx(i1,i2,i3,axis1,0))*
     & u1r-(an1*mu*rx(i1,i2,i3,axis1,0)+an2*(lambda+2.0*mu)*rx(i1,i2,
     & i3,axis1,1))*u2r
                        deti=1.0/(a11*a22-a21*a12)
                        u1s=( b1*a22-b2*a12)*deti
                        u2s=(-b1*a21+b2*a11)*deti
                        u1x=rx(i1,i2,i3,0,0)*u1r+rx(i1,i2,i3,1,0)*u1s
                        u1y=rx(i1,i2,i3,0,1)*u1r+rx(i1,i2,i3,1,1)*u1s
                        u2x=rx(i1,i2,i3,0,0)*u2r+rx(i1,i2,i3,1,0)*u2s
                        u2y=rx(i1,i2,i3,0,1)*u2r+rx(i1,i2,i3,1,1)*u2s
                        u(i1,i2,i3,s11c)=(lambda+2.0*mu)*u1x+lambda*u2y
                        u(i1,i2,i3,s21c)=mu*(u1y+u2x)
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s21c)
                        u(i1,i2,i3,s22c)=(lambda+2.0*mu)*u2y+lambda*u1x
                      else
                        ! initialize
                        is1=1-2*side1
                        is2=1-2*side2
                        if (.true.) then   ! true/false switch here is for testing Cartesian grids
c              aNormi1=1./max(epsx,sqrt(rx(i1,i2,i3,axis1,0)**2+rx(i1,i2,i3,axis1,1)**2))
c              an11=-is1*rx(i1,i2,i3,axis1,0)*aNormi1          ! normals for axis1,side1
c              an21=-is1*rx(i1,i2,i3,axis1,1)*aNormi1
                        aNormi1=aNormi
                        an11=an1
                        an21=an2
                        u1s0=is2*(u(i1,i2+is2,i3,uc)-u(i1,i2,i3,uc))
     & /dr(1)
                        u2s0=is2*(u(i1,i2+is2,i3,vc)-u(i1,i2,i3,vc))
     & /dr(1)
                        u1s=u1s0
                        u2s=u2s0
                        ! Newton iteration for u1s,u2s
                        ier=0
                        iter=1
                        istop=0
                        bmax=10.*toler
                        do while (bmax.gt.toler)
                          u1x=rx(i1,i2,i3,0,0)*u1r+rx(i1,i2,i3,1,0)*u1s
                          u1y=rx(i1,i2,i3,0,1)*u1r+rx(i1,i2,i3,1,1)*u1s
                          u2x=rx(i1,i2,i3,0,0)*u2r+rx(i1,i2,i3,1,0)*u2s
                          u2y=rx(i1,i2,i3,0,1)*u2r+rx(i1,i2,i3,1,1)*u2s
                          ! compute stress and the deriv based on current deformation gradient
                          !                      ideriv=1
                          !                      call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1x
                          du(1,2)=u1y
                          du(2,1)=u2x
                          du(2,2)=u2y
                          ideriv=1
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          coef11=rx(i1,i2,i3,0,1)-u1s/det(i1,i2,i3)
                          coef21=rx(i1,i2,i3,0,0)+u2s/det(i1,i2,i3)
                          alpha1=sqrt(coef11**2+coef21**2)*aNormi1
                          f11=bcf(side1,axis1,i1,i2,i3,uc)*alpha1      
     &   ! given traction forces
                          f21=bcf(side1,axis1,i1,i2,i3,vc)*alpha1
                          fact=aNormi1/(det(i1,i2,i3)*sqrt(coef11**2+
     & coef21**2))
                          dalpha11=-coef11*fact
                          dalpha12= coef21*fact
                          ! construct linear system
                          bb(1)=an11*p(1,1)+an21*p(2,1)-f11
                          bb(2)=an11*p(1,2)+an21*p(2,2)-f21
                          aa(1,1)= an11*(dpdf(1,1)*rx(i1,i2,i3,1,0)+
     & dpdf(1,2)*rx(i1,i2,i3,1,1)) +an21*(dpdf(3,1)*rx(i1,i2,i3,1,0)+
     & dpdf(3,2)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,uc)*
     & dalpha11
                          aa(1,2)= an11*(dpdf(1,3)*rx(i1,i2,i3,1,0)+
     & dpdf(1,4)*rx(i1,i2,i3,1,1)) +an21*(dpdf(3,3)*rx(i1,i2,i3,1,0)+
     & dpdf(3,4)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,uc)*
     & dalpha12
                          aa(2,1)= an11*(dpdf(2,1)*rx(i1,i2,i3,1,0)+
     & dpdf(2,2)*rx(i1,i2,i3,1,1)) +an21*(dpdf(4,1)*rx(i1,i2,i3,1,0)+
     & dpdf(4,2)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,vc)*
     & dalpha11
                          aa(2,2)= an11*(dpdf(2,3)*rx(i1,i2,i3,1,0)+
     & dpdf(2,4)*rx(i1,i2,i3,1,1)) +an21*(dpdf(4,3)*rx(i1,i2,i3,1,0)+
     & dpdf(4,4)*rx(i1,i2,i3,1,1)) -bcf(side1,axis1,i1,i2,i3,vc)*
     & dalpha12
                          ! solve the 2x2 system
                          determ=aa(1,1)*aa(2,2)-aa(1,2)*aa(2,1)
                          du1s=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                          du2s=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                          bmax=max(abs(bb(1)),abs(bb(2)))/lambda
                          if (istop.ne.0) then
                            write(6,'(1x,i2,3(1x,1pe15.8))')iter,bb(1),
     & bb(2),bmax
                          end if
                          ! update
                          u1s=u1s-du1s
                          u2s=u2s-du2s
                          iter=iter+1
                          ! check for convergence
                          if (iter.gt.itmax) then
                            write(6,*)'Error (bcOptSmFOS) : Newton 
     & failed to converge'
                            if (istop.eq.0) then
                              ier=0
                              iter=1
                              istop=1
                              u1s=u1s0
                              u2s=u2s0
                            else
                              stop 7782
                            end if
                          end if
                        end do
                        ! set displacement in the ghost point and set stress in the corner
                        u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is2*
     & dr(1)*u1s
                        u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is2*
     & dr(1)*u2s
                        u(i1,i2,i3,s11c)=p(1,1)
                        u(i1,i2,i3,s12c)=p(1,2)
                        u(i1,i2,i3,s21c)=p(2,1)
                        u(i1,i2,i3,s22c)=p(2,2)
                        !  compute v1s and v2s
                        v1r=(u(i1+1,i2,i3,v1c)-u(i1-1,i2,i3,v1c))/(2.0*
     & dr(0))
                        v2r=(u(i1+1,i2,i3,v2c)-u(i1-1,i2,i3,v2c))/(2.0*
     & dr(0))
                        aa(1,3)= an11*(dpdf(1,1)*rx(i1,i2,i3,0,0)+dpdf(
     & 1,2)*rx(i1,i2,i3,0,1)) +an21*(dpdf(3,1)*rx(i1,i2,i3,0,0)+dpdf(
     & 3,2)*rx(i1,i2,i3,0,1))
                        aa(1,4)= an11*(dpdf(1,3)*rx(i1,i2,i3,0,0)+dpdf(
     & 1,4)*rx(i1,i2,i3,0,1)) +an21*(dpdf(3,3)*rx(i1,i2,i3,0,0)+dpdf(
     & 3,4)*rx(i1,i2,i3,0,1))
                        bb(1)=bcf(side1,axis1,i1,i2,i3,v1c)*alpha1-aa(
     & 1,3)*v1r-aa(1,4)*v2r
                        aa(2,3)= an11*(dpdf(2,1)*rx(i1,i2,i3,0,0)+dpdf(
     & 2,2)*rx(i1,i2,i3,0,1)) +an21*(dpdf(4,1)*rx(i1,i2,i3,0,0)+dpdf(
     & 4,2)*rx(i1,i2,i3,0,1))
                        aa(2,4)= an11*(dpdf(2,3)*rx(i1,i2,i3,0,0)+dpdf(
     & 2,4)*rx(i1,i2,i3,0,1)) +an21*(dpdf(4,3)*rx(i1,i2,i3,0,0)+dpdf(
     & 4,4)*rx(i1,i2,i3,0,1))
                        bb(2)=bcf(side1,axis1,i1,i2,i3,v2c)*alpha1-aa(
     & 2,3)*v1r-aa(2,4)*v2r
                        v1s=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                        v2s=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                        !  set velocity in the ghost point
                        u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)-2.*is2*
     & dr(1)*v1s
                        u(i1,i2-is2,i3,v2c)=u(i1,i2+is2,i3,v2c)-2.*is2*
     & dr(1)*v2s
                        else   ! else true/false testing
                        u1s=0.
                        u2s=0.
                        u(i1,i2-is2,i3,uc)=u(i1,i2+is2,i3,uc)-2.*is2*
     & dr(1)*u1s
                        u(i1,i2-is2,i3,vc)=u(i1,i2+is2,i3,vc)-2.*is2*
     & dr(1)*u2s
                        u(i1,i2-is2,i3,v1c)=u(i1,i2+is2,i3,v1c)
                        u(i1,i2-is2,i3,v2c)=u(i1,i2+is2,i3,v2c)
                        u(i1,i2,i3,s11c)=0.
                        u(i1,i2,i3,s12c)=0.
                        u(i1,i2,i3,s21c)=0.
                        u(i1,i2,i3,s22c)=0.
                        end if   ! end true/false testing
                      end if
                    elseif (boundaryCondition(side1,axis1)
     & .eq.displacementBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC .and. fixupTractionDisplacementCorners ) then
                      ! non-Cartesian grid, mix bcs, case 2  (Should be okay for both new and old bcs)
                      is=1-2*side2
                      aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis2,0)**2+
     & rx(i1,i2,i3,axis2,1)**2))
                      an1=-is*rx(i1,i2,i3,axis2,0)*aNormi
                      an2=-is*rx(i1,i2,i3,axis2,1)*aNormi
                      u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dr(
     & 1))
                      u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dr(
     & 1))
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        a11=an1*(lambda+2.0*mu)*rx(i1,i2,i3,axis1,0)+
     & an2*mu*rx(i1,i2,i3,axis1,1)
                        a12=an1*lambda*rx(i1,i2,i3,axis1,1)+an2*mu*rx(
     & i1,i2,i3,axis1,0)
                        b1=an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,i3,s21c)-(
     & an1*(lambda+2.0*mu)*rx(i1,i2,i3,axis2,0)+an2*mu*rx(i1,i2,i3,
     & axis2,1))*u1s-(an1*lambda*rx(i1,i2,i3,axis2,1)+an2*mu*rx(i1,i2,
     & i3,axis2,0))*u2s
                        a21=an1*mu*rx(i1,i2,i3,axis1,1)+an2*lambda*rx(
     & i1,i2,i3,axis1,0)
                        a22=an1*mu*rx(i1,i2,i3,axis1,0)+an2*(lambda+
     & 2.0*mu)*rx(i1,i2,i3,axis1,1)
                        b2=an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,i3,s22c)-(
     & an1*mu*rx(i1,i2,i3,axis2,1)+an2*lambda*rx(i1,i2,i3,axis2,0))*
     & u1s-(an1*mu*rx(i1,i2,i3,axis2,0)+an2*(lambda+2.0*mu)*rx(i1,i2,
     & i3,axis2,1))*u2s
                        deti=1.0/(a11*a22-a21*a12)
                        u1r=( b1*a22-b2*a12)*deti
                        u2r=(-b1*a21+b2*a11)*deti
                        u1x=rx(i1,i2,i3,0,0)*u1r+rx(i1,i2,i3,1,0)*u1s
                        u1y=rx(i1,i2,i3,0,1)*u1r+rx(i1,i2,i3,1,1)*u1s
                        u2x=rx(i1,i2,i3,0,0)*u2r+rx(i1,i2,i3,1,0)*u2s
                        u2y=rx(i1,i2,i3,0,1)*u2r+rx(i1,i2,i3,1,1)*u2s
                        u(i1,i2,i3,s11c)=(lambda+2.0*mu)*u1x+lambda*u2y
                        u(i1,i2,i3,s21c)=mu*(u1y+u2x)
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s21c)
                        u(i1,i2,i3,s22c)=(lambda+2.0*mu)*u2y+lambda*u1x
                      else
                        ! initialize
                        is1=1-2*side1
                        is2=1-2*side2
                        if (.true.) then   ! true/false switch here is for testing Cartesian grids
c              aNormi2=1./max(epsx,sqrt(rx(i1,i2,i3,axis2,0)**2+rx(i1,i2,i3,axis2,1)**2))
c              an12=-is2*rx(i1,i2,i3,axis2,0)*aNormi2          ! normals for axis2,side2
c              an22=-is2*rx(i1,i2,i3,axis2,1)*aNormi2
                        aNormi2=aNormi
                        an12=an1
                        an22=an2
                        u1r0=is1*(u(i1+is1,i2,i3,uc)-u(i1,i2,i3,uc))
     & /dr(0)
                        u2r0=is1*(u(i1+is1,i2,i3,vc)-u(i1,i2,i3,vc))
     & /dr(0)
                        u1r=u1r0
                        u2r=u2r0
                        ! Newton iteration for u1r,u2r
                        ier=0
                        iter=1
                        istop=0
                        bmax=10.*toler
                        do while (bmax.gt.toler)
                          u1x=rx(i1,i2,i3,0,0)*u1r+rx(i1,i2,i3,1,0)*u1s
                          u1y=rx(i1,i2,i3,0,1)*u1r+rx(i1,i2,i3,1,1)*u1s
                          u2x=rx(i1,i2,i3,0,0)*u2r+rx(i1,i2,i3,1,0)*u2s
                          u2y=rx(i1,i2,i3,0,1)*u2r+rx(i1,i2,i3,1,1)*u2s
                          ! compute stress and the deriv based on current deformation gradient
                          !                      ideriv=1
                          !                      call smbcsdp (u1x,u1y,u2x,u2y,lambda,mu,p,dpdf,ideriv)
                          du(1,1)=u1x
                          du(1,2)=u1y
                          du(2,1)=u2x
                          du(2,2)=u2y
                          ideriv=1
                          call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                          coef12=rx(i1,i2,i3,1,1)+u1r/det(i1,i2,i3)
                          coef22=rx(i1,i2,i3,1,0)-u2r/det(i1,i2,i3)
                          alpha2=sqrt(coef12**2+coef22**2)*aNormi2
                          f12=bcf(side2,axis2,i1,i2,i3,uc)*alpha2      
     &    ! given traction forces
                          f22=bcf(side2,axis2,i1,i2,i3,vc)*alpha2
                          fact=aNormi2/(det(i1,i2,i3)*sqrt(coef12**2+
     & coef22**2))
                          dalpha21= coef12*fact
                          dalpha22=-coef22*fact
                          ! construct linear system
                          bb(1)=an12*p(1,1)+an22*p(2,1)-f12
                          bb(2)=an12*p(1,2)+an22*p(2,2)-f22
                          aa(1,1)= an12*(dpdf(1,1)*rx(i1,i2,i3,0,0)+
     & dpdf(1,2)*rx(i1,i2,i3,0,1)) +an22*(dpdf(3,1)*rx(i1,i2,i3,0,0)+
     & dpdf(3,2)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,uc)*
     & dalpha21
                          aa(1,2)= an12*(dpdf(1,3)*rx(i1,i2,i3,0,0)+
     & dpdf(1,4)*rx(i1,i2,i3,0,1)) +an22*(dpdf(3,3)*rx(i1,i2,i3,0,0)+
     & dpdf(3,4)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,uc)*
     & dalpha22
                          aa(2,1)= an12*(dpdf(2,1)*rx(i1,i2,i3,0,0)+
     & dpdf(2,2)*rx(i1,i2,i3,0,1)) +an22*(dpdf(4,1)*rx(i1,i2,i3,0,0)+
     & dpdf(4,2)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,vc)*
     & dalpha21
                          aa(2,2)= an12*(dpdf(2,3)*rx(i1,i2,i3,0,0)+
     & dpdf(2,4)*rx(i1,i2,i3,0,1)) +an22*(dpdf(4,3)*rx(i1,i2,i3,0,0)+
     & dpdf(4,4)*rx(i1,i2,i3,0,1)) -bcf(side2,axis2,i1,i2,i3,vc)*
     & dalpha22
                          ! solve the 2x2 system
                          determ=aa(1,1)*aa(2,2)-aa(1,2)*aa(2,1)
                          du1r=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                          du2r=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                          bmax=max(abs(bb(1)),abs(bb(2)))/lambda
                          if (istop.ne.0) then
                            write(6,'(1x,i2,3(1x,1pe15.8))')iter,bb(1),
     & bb(2),bmax
                          end if
                          ! update
                          u1r=u1r-du1r
                          u2r=u2r-du2r
                          iter=iter+1
                          ! check for convergence
                          if (iter.gt.itmax) then
                            write(6,*)'Error (bcOptSmFOS) : Newton 
     & failed to converge'
                            if (istop.eq.0) then
                              ier=0
                              iter=1
                              istop=1
                              u1r=u1r0
                              u2r=u2r0
                            else
                              stop 7783
                            end if
                          end if
                        end do
                        ! set displacement in the ghost point and stress in the corner
                        u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*
     & dr(0)*u1r
                        u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*
     & dr(0)*u2r
                        u(i1,i2,i3,s11c)=p(1,1)
                        u(i1,i2,i3,s12c)=p(1,2)
                        u(i1,i2,i3,s21c)=p(2,1)
                        u(i1,i2,i3,s22c)=p(2,2)
                        !  compute v1r and v2r
                        v1s=(u(i1,i2+1,i3,v1c)-u(i1,i2-1,i3,v1c))/(2.0*
     & dr(1))
                        v2s=(u(i1,i2+1,i3,v2c)-u(i1,i2-1,i3,v2c))/(2.0*
     & dr(1))
                        aa(1,3)= an12*(dpdf(1,1)*rx(i1,i2,i3,1,0)+dpdf(
     & 1,2)*rx(i1,i2,i3,1,1)) +an22*(dpdf(3,1)*rx(i1,i2,i3,1,0)+dpdf(
     & 3,2)*rx(i1,i2,i3,1,1))
                        aa(1,4)= an12*(dpdf(1,3)*rx(i1,i2,i3,1,0)+dpdf(
     & 1,4)*rx(i1,i2,i3,1,1)) +an22*(dpdf(3,3)*rx(i1,i2,i3,1,0)+dpdf(
     & 3,4)*rx(i1,i2,i3,1,1))
                        bb(1)=bcf(side2,axis2,i1,i2,i3,v1c)*alpha2-aa(
     & 1,3)*v1s-aa(1,4)*v2s
                        aa(2,3)= an12*(dpdf(2,1)*rx(i1,i2,i3,1,0)+dpdf(
     & 2,2)*rx(i1,i2,i3,1,1)) +an22*(dpdf(4,1)*rx(i1,i2,i3,1,0)+dpdf(
     & 4,2)*rx(i1,i2,i3,1,1))
                        aa(2,4)= an12*(dpdf(2,3)*rx(i1,i2,i3,1,0)+dpdf(
     & 2,4)*rx(i1,i2,i3,1,1)) +an22*(dpdf(4,3)*rx(i1,i2,i3,1,0)+dpdf(
     & 4,4)*rx(i1,i2,i3,1,1))
                        bb(2)=bcf(side2,axis2,i1,i2,i3,v2c)*alpha2-aa(
     & 2,3)*v1s-aa(2,4)*v2s
                        v1r=(bb(1)*aa(2,2)-bb(2)*aa(1,2))/determ
                        v2r=(aa(1,1)*bb(2)-aa(2,1)*bb(1))/determ
                        !  set velocity in the ghost point
                        u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)-2.*is1*
     & dr(0)*v1r
                        u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)-2.*is1*
     & dr(0)*v2r
                        else   ! else true/false testing
                        u1r=0.
                        u2r=0.
                        u(i1-is1,i2,i3,uc)=u(i1+is1,i2,i3,uc)-2.*is1*
     & dr(0)*u1r
                        u(i1-is1,i2,i3,vc)=u(i1+is1,i2,i3,vc)-2.*is1*
     & dr(0)*u2r
                        u(i1-is1,i2,i3,v1c)=u(i1+is1,i2,i3,v1c)
                        u(i1-is1,i2,i3,v2c)=u(i1+is1,i2,i3,v2c)
                        u(i1,i2,i3,s11c)=0.
                        u(i1,i2,i3,s12c)=0.
                        u(i1,i2,i3,s21c)=0.
                        u(i1,i2,i3,s22c)=0.
                        end if   ! end true/false testing
                      end if
                    end if
                  end if
                end do
              end do
            end if
            ! ..add on TZ flow contribution (if necessary)
            if (twilightZone.ne.0) then
              do side1=0,1
                i1=gridIndexRange(side1,axis1)
                do side2=0,1
                  i2=gridIndexRange(side2,axis2)
                  if (mask(i1,i2,i3).ne.0) then
                    if (bctype.eq.linearBoundaryCondition) then     ! linear case only
                      if (materialFormat.ne.constantMaterialProperties)
     &  then
                        call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.,t,muc,mu)
                        call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,
     & i2,i3,1),0.,t,lambdac,lambda)
                      end if
                    end if
                    if (boundaryCondition(side1,axis1)
     & .eq.displacementBC.and.boundaryCondition(side2,axis2)
     & .eq.displacementBC) then
                      ! pure displacement/velocity bcs
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s11c,s11e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s12c,s12e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s21c,s21e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s22c,s22e)
                      call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,uc,u1xe)
                      call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,vc,u2xe)
                      call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,uc,u1ye)
                      call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,vc,u2ye)
                      if (bctype.eq.linearBoundaryCondition) then      
     &                          ! linear case
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+s11e-(lambda*
     & (u1xe+u2ye)+2.0*mu*u1xe)
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+s12e-mu*(
     & u1ye+u2xe)
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+s21e-mu*(
     & u1ye+u2xe)
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+s22e-(lambda*
     & (u1xe+u2ye)+2.0*mu*u2ye)
                      else                                             
     &   ! SVK case
                        ! call smbcsdp (u1xe,u1ye,u2xe,u2ye,lambda,mu,p,dpdf,0)
                        du(1,1)=u1xe
                        du(1,2)=u1ye
                        du(2,1)=u2xe
                        du(2,2)=u2ye
                        ideriv=0
                        call smgetdp (du,p,dpdf,cpar,ideriv,itype)
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+s11e-p(1,1)
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+s12e-p(1,2)
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+s21e-p(2,1)
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+s22e-p(2,2)
                      end if
                    elseif (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC) then
                      ! pure traction bcs
                      !                  No TZ forcing needed here.  For the Cartesian case, no fix was done, and for the
                      !                  non-Cartesian case, the forcing was already included in the bcf array.
                    elseif (boundaryCondition(side1,axis1)
     & .eq.tractionBC.and.boundaryCondition(side2,axis2)
     & .eq.displacementBC) then
                      ! mix bcs, case 1
                      call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,uc,u1xe)
                      call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,vc,u2xe)
                      call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,uc,u1ye)
                      call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,vc,u2ye)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s11c,s11e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s12c,s12e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s21c,s21e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s22c,s22e)
                      if (gridType.eq.rectangular) then
                      !   Cartesian case
                        if (bctype.eq.linearBoundaryCondition) then    
     &                            ! linear case
                          u1ye=(s12e-mu*u2xe)/mu
                          u2ye=(s11e-(lambda+2.0*mu)*u1xe)/lambda
                          u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+s21e-mu*(
     & u1ye+u2xe)
                          u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+s22e-
     & lambda*(u1xe+u2ye)-2.0*mu*u2ye
                        end if
                      else
                        !   non-Cartesian case
                        is=1-2*side1
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis1,0)**
     & 2+rx(i1,i2,i3,axis1,1)**2))
                        an1=-is*rx(i1,i2,i3,axis1,0)*aNormi
                        an2=-is*rx(i1,i2,i3,axis1,1)*aNormi
                        if (bctype.eq.linearBoundaryCondition) then    
     &                            ! linear case
                          deti=1.0/(rx(i1,i2,i3,axis2,1)*rx(i1,i2,i3,
     & axis1,0)-rx(i1,i2,i3,axis2,0)*rx(i1,i2,i3,axis1,1))
                          u1re=(rx(i1,i2,i3,axis2,1)*u1xe-rx(i1,i2,i3,
     & axis2,0)*u1ye)*deti
                          u2re=(rx(i1,i2,i3,axis2,1)*u2xe-rx(i1,i2,i3,
     & axis2,0)*u2ye)*deti
                          a11=an1*(lambda+2.0*mu)*rx(i1,i2,i3,axis2,0)+
     & an2*mu*rx(i1,i2,i3,axis2,1)
                          a12=an1*lambda*rx(i1,i2,i3,axis2,1)+an2*mu*
     & rx(i1,i2,i3,axis2,0)
                          b1=an1*s11e+an2*s21e-(an1*(lambda+2.0*mu)*rx(
     & i1,i2,i3,axis1,0)+an2*mu*rx(i1,i2,i3,axis1,1))*u1re-(an1*
     & lambda*rx(i1,i2,i3,axis1,1)+an2*mu*rx(i1,i2,i3,axis1,0))*u2re
                          a21=an1*mu*rx(i1,i2,i3,axis2,1)+an2*lambda*
     & rx(i1,i2,i3,axis2,0)
                          a22=an1*mu*rx(i1,i2,i3,axis2,0)+an2*(lambda+
     & 2.0*mu)*rx(i1,i2,i3,axis2,1)
                          b2=an1*s12e+an2*s22e-(an1*mu*rx(i1,i2,i3,
     & axis1,1)+an2*lambda*rx(i1,i2,i3,axis1,0))*u1re-(an1*mu*rx(i1,
     & i2,i3,axis1,0)+an2*(lambda+2.0*mu)*rx(i1,i2,i3,axis1,1))*u2re
                          deti=1.0/(a11*a22-a21*a12)
                          u1se=( b1*a22-b2*a12)*deti
                          u2se=(-b1*a21+b2*a11)*deti
                          u1xe=rx(i1,i2,i3,0,0)*u1re+rx(i1,i2,i3,1,0)*
     & u1se
                          u1ye=rx(i1,i2,i3,0,1)*u1re+rx(i1,i2,i3,1,1)*
     & u1se
                          u2xe=rx(i1,i2,i3,0,0)*u2re+rx(i1,i2,i3,1,0)*
     & u2se
                          u2ye=rx(i1,i2,i3,0,1)*u2re+rx(i1,i2,i3,1,1)*
     & u2se
                          u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+s11e-(
     & lambda+2.0*mu)*u1xe-lambda*u2ye
                          u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+s21e-mu*(
     & u1ye+u2xe)
                          u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+s12e-mu*(
     & u1ye+u2xe)
                          u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+s22e-(
     & lambda+2.0*mu)*u2ye-lambda*u1xe
                        end if
                      end if
                    elseif (boundaryCondition(side1,axis1)
     & .eq.displacementBC.and.boundaryCondition(side2,axis2)
     & .eq.tractionBC) then
                      ! mix bcs, case 2
                      call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,uc,u1xe)
                      call ogDeriv (ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,vc,u2xe)
                      call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,uc,u1ye)
                      call ogDeriv (ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,vc,u2ye)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s11c,s11e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s12c,s12e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s21c,s21e)
                      call ogDeriv (ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,
     & i3,1),0.0,t,s22c,s22e)
                      if (gridType.eq.rectangular) then
                        !  Cartesian case
                        if (bctype.eq.linearBoundaryCondition) then    
     &                            ! linear case
                          u1xe=(s22e-(lambda+2.0*mu)*u2ye)/lambda
                          u2xe=(s21e-mu*u1ye)/mu
                          u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+s11e-
     & lambda*(u1xe+u2ye)-2.0*mu*u1xe
                          u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+s12e-mu*(
     & u1ye+u2xe)
                        end if
                      else
                        ! non-Cartesian case
                        is=1-2*side2
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis2,0)**
     & 2+rx(i1,i2,i3,axis2,1)**2))
                        an1=-is*rx(i1,i2,i3,axis2,0)*aNormi
                        an2=-is*rx(i1,i2,i3,axis2,1)*aNormi
                        if (bctype.eq.linearBoundaryCondition) then    
     &                            ! linear case
                          deti=1.0/(rx(i1,i2,i3,axis1,0)*rx(i1,i2,i3,
     & axis2,1)-rx(i1,i2,i3,axis1,1)*rx(i1,i2,i3,axis2,0))
                          u1se=(rx(i1,i2,i3,axis1,0)*u1ye-rx(i1,i2,i3,
     & axis1,1)*u1xe)*deti
                          u2se=(rx(i1,i2,i3,axis1,0)*u2ye-rx(i1,i2,i3,
     & axis1,1)*u2xe)*deti
                          a11=an1*(lambda+2.0*mu)*rx(i1,i2,i3,axis1,0)+
     & an2*mu*rx(i1,i2,i3,axis1,1)
                          a12=an1*lambda*rx(i1,i2,i3,axis1,1)+an2*mu*
     & rx(i1,i2,i3,axis1,0)
                          b1=an1*s11e+an2*s21e-(an1*(lambda+2.0*mu)*rx(
     & i1,i2,i3,axis2,0)+an2*mu*rx(i1,i2,i3,axis2,1))*u1se-(an1*
     & lambda*rx(i1,i2,i3,axis2,1)+an2*mu*rx(i1,i2,i3,axis2,0))*u2se
                          a21=an1*mu*rx(i1,i2,i3,axis1,1)+an2*lambda*
     & rx(i1,i2,i3,axis1,0)
                          a22=an1*mu*rx(i1,i2,i3,axis1,0)+an2*(lambda+
     & 2.0*mu)*rx(i1,i2,i3,axis1,1)
                          b2=an1*s12e+an2*s22e-(an1*mu*rx(i1,i2,i3,
     & axis2,1)+an2*lambda*rx(i1,i2,i3,axis2,0))*u1se-(an1*mu*rx(i1,
     & i2,i3,axis2,0)+an2*(lambda+2.0*mu)*rx(i1,i2,i3,axis2,1))*u2se
                          deti=1.0/(a11*a22-a21*a12)
                          u1re=( b1*a22-b2*a12)*deti
                          u2re=(-b1*a21+b2*a11)*deti
                          u1xe=rx(i1,i2,i3,0,0)*u1re+rx(i1,i2,i3,1,0)*
     & u1se
                          u1ye=rx(i1,i2,i3,0,1)*u1re+rx(i1,i2,i3,1,1)*
     & u1se
                          u2xe=rx(i1,i2,i3,0,0)*u2re+rx(i1,i2,i3,1,0)*
     & u2se
                          u2ye=rx(i1,i2,i3,0,1)*u2re+rx(i1,i2,i3,1,1)*
     & u2se
                          u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+s11e-(
     & lambda+2.0*mu)*u1xe-lambda*u2ye
                          u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+s21e-mu*(
     & u1ye+u2xe)
                          u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+s12e-mu*(
     & u1ye+u2xe)
                          u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+s22e-(
     & lambda+2.0*mu)*u2ye-lambda*u1xe
                        end if
                      end if
                    end if
                  end if
                end do
              end do
            end if
  ! TEMP TEMP TEMP TEMP
         if( .false. )then ! ********** TESTING *wdh* June 27, 2015
          !*******
          !******* RE-ASSIGN Primary Dirichlet boundary conditions ***********
          !*******
          ! -- Dirichlet values on ghost may not be correct : fix them
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
             if( boundaryCondition(side,axis).eq.displacementBC )then
              ! *************** Displacement BC *****************
              ! ..step 0: Dirichlet bcs for displacement and velocity
               i3=n3a
               do i2=nn2a,nn2b
               do i1=nn1a,nn1b
               if (mask(i1,i2,i3).ne.0) then
                u(i1,i2,i3,uc) =bcf(side,axis,i1,i2,i3,uc)    ! given displacements
                u(i1,i2,i3,vc) =bcf(side,axis,i1,i2,i3,vc)
                u(i1,i2,i3,v1c)=bcf(side,axis,i1,i2,i3,v1c)   ! given velocities
                u(i1,i2,i3,v2c)=bcf(side,axis,i1,i2,i3,v2c)
                !call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,uc,ue)
                !write(*,'(" i1,i2=",2i3," u,ue=",2e10.2)') i1,i2,u(i1,i2,i3,uc),ue
               end if
               end do
               end do
             else if( boundaryCondition(side,axis).eq.tractionBC )then
              if( applyInterfaceBoundaryConditions.eq.0 .and. 
     & interfaceType(side,axis,grid).eq.tractionInterface )then
               write(*,'("SMBC: skip traction BC on an interface, (
     & side,axis,grid)=(",3i3,")")') side,axis,grid
              else
               ! ********* Traction BC ********
               ! put "dirichlet parts of the traction BC here
              if( debug.gt.3. .and. interfaceType(side,axis,grid)
     & .eq.tractionInterface )then
               write(*,'("SMBC:INFO: assignPrimaryDirichletBC for an 
     & interface, (side,axis,grid)=(",3i3,")")') side,axis,grid
              end if
              if( gridType.eq.rectangular )then
                if (bctype.eq.linearBoundaryCondition) then      ! linear
                  ! new
                  if( axis.eq.0 )then
                    i3=n3a
                    do i2=nn2a,nn2b
                    do i1=nn1a,nn1b
                    if (mask(i1,i2,i3).ne.0) then
                     ! set normal components of the stress, n=(-is,0)
                      f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                      f2=bcf(side,axis,i1,i2,i3,s12c)
                      f1=f1+is*u(i1,i2,i3,s11c)
                      f2=f2+is*u(i1,i2,i3,s12c)
                      u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)-is*f1
                      u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)-is*f2
                      u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)-is*f2
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s11c,tau11)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s21c,tau21)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s12c,tau12)
                      ! if (abs(tau11-u(i1,i2,i3,s11c)).gt.1.e-14) then
                      !   write(6,*)i1,i2,i3,t,s11c,abs(tau11-u(i1,i2,i3,s11c))
                      !   pause
                      ! end if
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s22c,tau22)
                      !  write(6,'(2(1x,i2),4(1x,f8.4),/,6x,4(1x,f8.4))')i1,i2,u(i1,i2,i3,s11c),u(i1,i2,i3,s12c),u(i1,i2,i3,s21c),u(i1,i2,i3,s22c),tau11,tau12,tau21,tau22
                      !  333            format(2(1x,i2),4(1x,f8.4),/,6x,4(1x,f8.4))
                    end if
                    end do
                    end do
                  else
                    i3=n3a
                    do i2=nn2a,nn2b
                    do i1=nn1a,nn1b
                    if (mask(i1,i2,i3).ne.0) then
                     ! set normal components of the stress, n=(0,-is)
                      f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                      f2=bcf(side,axis,i1,i2,i3,s12c)
                      f1=f1+is*u(i1,i2,i3,s21c)
                      f2=f2+is*u(i1,i2,i3,s22c)
                      !   u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)
                      u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)-is*f1
                      u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)-is*f1
                      u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)-is*f2
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s11c,tau11)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s21c,tau21)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s12c,tau12)
                      ! call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),0.,t,s22c,tau22)
                      ! write(6,'(2(1x,i2),4(1x,f8.4),/,6x,4(1x,f8.4))')i1,i2,u(i1,i2,i3,s11c),u(i1,i2,i3,s12c),u(i1,i2,i3,s21c),u(i1,i2,i3,s22c),tau11,tau12,tau21,tau22
                    end if
                    end do
                    end do
                  end if
                else    ! SVK
                  if( axis.eq.0 )then
                    i3=n3a
                    do i2=nn2a,nn2b
                    do i1=nn1a,nn1b
                    if (mask(i1,i2,i3).ne.0) then
                     ! set normal components of the stress, n=(-is,0)
                     u1y=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*dx(1)
     & )
                     u2y=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*dx(1)
     & )
                     alpha=sqrt(u1y**2+(1.0+u2y)**2)
                     u(i1,i2,i3,s11c) =-is*bcf(side,axis,i1,i2,i3,s11c)
     & *alpha
                     u(i1,i2,i3,s12c) =-is*bcf(side,axis,i1,i2,i3,s12c)
     & *alpha
          !!      write(*,'(" primary: set i1,i2,i3 alpha, bc, s11=",3i4,3e16.8)')  i1,i2,i3,alpha,bcf(side,axis,i1,i2,i3,s11c),u(i1,i2,i3,s11c)        
          !!      write(*,'(" primary: u,v=",4e16.8)') u(i1,i2+1,i3,uc),u(i1,i2-1,i3,uc),u(i1,i2+1,i3,vc),u(i1,i2-1,i3,vc)
                    end if
                    end do
                    end do
                  else
                    i3=n3a
                    do i2=nn2a,nn2b
                    do i1=nn1a,nn1b
                    if (mask(i1,i2,i3).ne.0) then
                     ! set normal components of the stress, n=(0,-is)
                     u1x=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*dx(0)
     & )
                     u2x=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*dx(0)
     & )
                     alpha=sqrt((1.0+u1x)**2+u2x**2)
                     u(i1,i2,i3,s21c) =-is*bcf(side,axis,i1,i2,i3,s11c)
     & *alpha
                     u(i1,i2,i3,s22c) =-is*bcf(side,axis,i1,i2,i3,s12c)
     & *alpha
                    end if
                    end do
                    end do
                  end if
                end if
              else  ! curvilinear
                if (bctype.eq.linearBoundaryCondition) then   ! linear
                  ! new
                   i3=n3a
                   do i2=nn2a,nn2b
                   do i1=nn1a,nn1b
                   if (mask(i1,i2,i3).ne.0) then
                    f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                    f2=bcf(side,axis,i1,i2,i3,s12c)
                    ! (an1,an2) = outward normal 
                    aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(
     & i1,i2,i3,axis,1)**2))
                    an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                    an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                    f1=f1-(an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,i3,s21c))
                    f2=f2-(an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,i3,s22c))
                    b1=((1.0+an2**2)*f1-an1*an2*f2)/2.0
                    b2=((1.0+an1**2)*f2-an1*an2*f1)/2.0
                    u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+2.0*b1*an1
                    u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+b2*an1+b1*an2
                    u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+b2*an1+b1*an2
                    u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+2.0*b2*an2
                   end if
                   end do
                   end do
                else     ! SVK
                  if (axis.eq.0) then
                     i3=n3a
                     do i2=nn2a,nn2b
                     do i1=nn1a,nn1b
                      if (mask(i1,i2,i3).ne.0) then
                        ! (an1,an2) = outward normal 
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+
     & rx(i1,i2,i3,axis,1)**2))
                        an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                        an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                        u1s=(u(i1,i2+1,i3,uc)-u(i1,i2-1,i3,uc))/(2.0*
     & dr(1))
                        u2s=(u(i1,i2+1,i3,vc)-u(i1,i2-1,i3,vc))/(2.0*
     & dr(1))
                        alpha=sqrt((rx(i1,i2,i3,0,1)-u1s/det(i1,i2,i3))
     & **2+(rx(i1,i2,i3,0,0)+u2s/det(i1,i2,i3))**2)*aNormi
                        f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                        f2=bcf(side,axis,i1,i2,i3,s12c)
                        b1=f1*alpha-(an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,
     & i3,s21c))
                        b2=f2*alpha-(an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,
     & i3,s22c))
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+an1*b1
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+an1*b2
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+an2*b1
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+an2*b2
                      end if
                     end do
                     end do
                  else
                     i3=n3a
                     do i2=nn2a,nn2b
                     do i1=nn1a,nn1b
                      if (mask(i1,i2,i3).ne.0) then
                        ! (an1,an2) = outward normal 
                        aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+
     & rx(i1,i2,i3,axis,1)**2))
                        an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                        an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                        u1r=(u(i1+1,i2,i3,uc)-u(i1-1,i2,i3,uc))/(2.0*
     & dr(0))
                        u2r=(u(i1+1,i2,i3,vc)-u(i1-1,i2,i3,vc))/(2.0*
     & dr(0))
                        alpha=sqrt((rx(i1,i2,i3,1,1)+u1r/det(i1,i2,i3))
     & **2+(rx(i1,i2,i3,1,0)-u2r/det(i1,i2,i3))**2)*aNormi
                        f1=bcf(side,axis,i1,i2,i3,s11c)              ! given traction forces
                        f2=bcf(side,axis,i1,i2,i3,s12c)
                        b1=f1*alpha-(an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,
     & i3,s21c))
                        b2=f2*alpha-(an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,
     & i3,s22c))
                        u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)+an1*b1
                        u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+an1*b2
                        u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+an2*b1
                        u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+an2*b2
                      end if
                     end do
                     end do
                  end if
                end if
              end if  ! end gridType
              end if ! not interface
             else if( boundaryCondition(side,axis).eq.slipWall )then
               ! ********* SlipWall BC ********
               ! put "dirichlet parts of the slipwall BC here
              if( gridType.eq.rectangular )then
                ! new
                if( axis.eq.0 )then
                  i3=n3a
                  do i2=nn2a,nn2b
                  do i1=nn1a,nn1b
                  if (mask(i1,i2,i3).ne.0) then
                   ! set n.tau.t and the normal component of displacement, n=(-is,0), t=(0,-is)
                   u(i1,i2,i3,s12c) = bcf(side,axis,i1,i2,i3,s11c)
                   u(i1,i2,i3,s21c) = bcf(side,axis,i1,i2,i3,s11c)
                   u(i1,i2,i3,uc) = -is*bcf(side,axis,i1,i2,i3,uc)
                   u(i1,i2,i3,v1c) = -is*bcf(side,axis,i1,i2,i3,v1c)
                  end if
                  end do
                  end do
                else
                  i3=n3a
                  do i2=nn2a,nn2b
                  do i1=nn1a,nn1b
                  if (mask(i1,i2,i3).ne.0) then
                   ! set n.tau.t and the normal component of displacement, n=(0,-is), t=(+is,0)
                   u(i1,i2,i3,s12c) = -bcf(side,axis,i1,i2,i3,s11c)
                   u(i1,i2,i3,s21c) = -bcf(side,axis,i1,i2,i3,s11c)
                   u(i1,i2,i3,vc) = -is*bcf(side,axis,i1,i2,i3,uc)
                   u(i1,i2,i3,v2c) = -is*bcf(side,axis,i1,i2,i3,v1c)
                  end if
                  end do
                  end do
                end if
              else  ! curvilinear
                ! new
                 i3=n3a
                 do i2=nn2a,nn2b
                 do i1=nn1a,nn1b
                 if (mask(i1,i2,i3).ne.0) then
                  f1=bcf(side,axis,i1,i2,i3,s11c)              ! given tangential traction force
                  f2=bcf(side,axis,i1,i2,i3,uc)                ! given normal displacement
                  f3=bcf(side,axis,i1,i2,i3,v1c)               ! given normal velocity
                  ! (an1,an2) = outward normal and (-an2,an1) = unit tangent
                  aNormi=1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,
     & i2,i3,axis,1)**2))
                  an1=-is*rx(i1,i2,i3,axis,0)*aNormi
                  an2=-is*rx(i1,i2,i3,axis,1)*aNormi
                  b1=f1-an1*(-u(i1,i2,i3,s11c)*an2+u(i1,i2,i3,s12c)*
     & an1)-an2*(-u(i1,i2,i3,s21c)*an2+u(i1,i2,i3,s22c)*an1)
                  b2=f2-an1*u(i1,i2,i3,uc)-an2*u(i1,i2,i3,vc)
                  b3=f3-an1*u(i1,i2,i3,v1c)-an2*u(i1,i2,i3,v2c)
                  u(i1,i2,i3,s11c)=u(i1,i2,i3,s11c)-2.0*b1*an1*an2
                  u(i1,i2,i3,s12c)=u(i1,i2,i3,s12c)+b1*(an1**2-an2**2)
                  u(i1,i2,i3,s21c)=u(i1,i2,i3,s21c)+b1*(an1**2-an2**2)
                  u(i1,i2,i3,s22c)=u(i1,i2,i3,s22c)+2.0*b1*an1*an2
                  u(i1,i2,i3,uc)=u(i1,i2,i3,uc)+an1*b2
                  u(i1,i2,i3,vc)=u(i1,i2,i3,vc)+an2*b2
                  u(i1,i2,i3,v1c)=u(i1,i2,i3,v1c)+an1*b3
                  u(i1,i2,i3,v2c)=u(i1,i2,i3,v2c)+an2*b3
                 end if
                 end do
                 end do
              end if  ! end gridType
             end if ! bc
             end do ! end side
             end do ! end axis
         end if
         !*******
         !******* Extrapolation to the second ghost line ********
         !*******
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
           if( boundaryCondition(side,axis).gt.0 .and. 
     & boundaryCondition(side,axis).ne.symmetry )then
             i3=n3a
             do i2=nn2a,nn2b
             do i1=nn1a,nn1b
             if (mask(i1,i2,i3).ne.0) then
              do n=0,numberOfComponents-1
          !      u(i1-2*is1,i2-2*is2,i3,n)=extrap3(u,i1-is1,i2-is2,i3,n,is1,is2,is3)
                   ! here du2=2nd-order approximation, du3=third order
                   ! Blend the 2nd and 3rd order based on the difference 
                   !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                   du1 = u(i1-is1,i2-is2,i3,n)
                   du2 = 2.*u(i1-is1,i2-is2,i3,n)-u(i1-is1+is1,i2-is2+
     & is2,i3,n)
                   du3 = 3.*u(i1-is1,i2-is2,i3,n)-3.*u(i1-is1+is1,i2-
     & is2+is2,i3,n)+u(i1-is1+2*is1,i2-is2+2*is2,i3,n)
                   !   alpha = cdl*(abs(du3-u(i1-is1+is1,i2-is2+is2,i3,n))+abs(du3-du2))/(uEps+abs(u(i1-is1+is1,i2-is2+is2,i3,n))+abs(u(i1-is1+2*is1,i2-is2+2*is2,i3,n)))
                   ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1-is1+is1,i2-is2+is2,i3,n))+abs(u(i1-is1+2*is1,i2-is2+2*is2,i3,n)))
                   uNorm= uEps+ abs(du3) + abs(u(i1-is1,i2-is2,i3,n))+
     & abs(u(i1-is1+is1,i2-is2+is2,i3,n))
                   ! **  du = abs(du3-u(i1-is1+is1,i2-is2+is2,i3,n))/uNorm  ! changed 050711
                   ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                   alpha = cdl*( abs(du3-du2)/uNorm )
                   alpha =min(1.,alpha)
                   ! if( mm.eq.1 )then
                 !  if (alpha.gt.0.9) then
                 !    write(6,*)'limiting, n,du1,du3=',n,du1,du3
                 !    write(6,*)'i1-is1,i2-is2,i3=',i1-is1,i2-is2,i3
                 !    write(6,*)'is1,is2,is3=',is1,is2,is3
                 !  end if
                   !   u(i1-is1,i2-is2,i3,n)=(1.-alpha)*du3+alpha*du2
                   u(i1-is1-is1,i2-is2-is2,i3,n)=(1.-alpha)*du3+alpha*
     & du1
              end do
             end if
             end do
             end do
           else if( boundaryCondition(side,axis).eq.symmetry )then  ! *wdh* 101108
            ! even symmetry 
            if( twilightZone.eq.0 )then
              i3=n3a
              do i2=nn2a,nn2b
              do i1=nn1a,nn1b
              if (mask(i1,i2,i3).ne.0) then
               do n=0,numberOfComponents-1
                 u(i1-2*is1,i2-2*is2,i3,n)=u(i1+2*is1,i2+2*is2,i3,n)
               end do
              end if
              end do
              end do
            else
             ! TZ :
              i3=n3a
              do i2=nn2a,nn2b
              do i1=nn1a,nn1b
              if (mask(i1,i2,i3).ne.0) then
               do n=0,numberOfComponents-1
                 call ogDeriv(ep,0,0,0,0,xy(i1-2*is1,i2-2*is2,i3,0),xy(
     & i1-2*is1,i2-2*is2,i3,1),0.,t,n,uem)
                 call ogDeriv(ep,0,0,0,0,xy(i1+2*is1,i2+2*is2,i3,0),xy(
     & i1+2*is1,i2+2*is2,i3,1),0.,t,n,uep)
                 u(i1-2*is1,i2-2*is2,i3,n)=u(i1+2*is1,i2+2*is2,i3,n) + 
     & uem - uep
               end do
              end if
              end do
              end do
            end if
           end if ! bc
           end do ! end side
           end do ! end axis
          !..extrapolate the 2nd ghost line near the corners
          i3=gridIndexRange(0,2)
          do side1=0,1
            i1=gridIndexRange(side1,axis1)
            is1=1-2*side1
            do side2=0,1
              i2=gridIndexRange(side2,axis2)
              is2=1-2*side2
              ! extrapolate in the i1 direction
              if (boundaryCondition(side1,axis1).gt.0) then
                if (mask(i1,i2,i3).ne.0) then
                  do n=0,numberOfComponents-1
                   ! u(i1-2*is1,i2-is2,i3,n)=extrap3(u,i1-is1,i2-is2,i3,n,is1,0,0)
                      ! here du2=2nd-order approximation, du3=third order
                      ! Blend the 2nd and 3rd order based on the difference 
                      !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                      du1 = u(i1-is1,i2-0,i3,n)
                      du2 = 2.*u(i1-is1,i2-0,i3,n)-u(i1-is1+is1,i2-0+0,
     & i3,n)
                      du3 = 3.*u(i1-is1,i2-0,i3,n)-3.*u(i1-is1+is1,i2-
     & 0+0,i3,n)+u(i1-is1+2*is1,i2-0+2*0,i3,n)
                      !   alpha = cdl*(abs(du3-u(i1-is1+is1,i2-0+0,i3,n))+abs(du3-du2))/(uEps+abs(u(i1-is1+is1,i2-0+0,i3,n))+abs(u(i1-is1+2*is1,i2-0+2*0,i3,n)))
                      ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1-is1+is1,i2-0+0,i3,n))+abs(u(i1-is1+2*is1,i2-0+2*0,i3,n)))
                      uNorm= uEps+ abs(du3) + abs(u(i1-is1,i2-0,i3,n))+
     & abs(u(i1-is1+is1,i2-0+0,i3,n))
                      ! **  du = abs(du3-u(i1-is1+is1,i2-0+0,i3,n))/uNorm  ! changed 050711
                      ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                      alpha = cdl*( abs(du3-du2)/uNorm )
                      alpha =min(1.,alpha)
                      ! if( mm.eq.1 )then
                    !  if (alpha.gt.0.9) then
                    !    write(6,*)'limiting, n,du1,du3=',n,du1,du3
                    !    write(6,*)'i1-is1,i2-0,i3=',i1-is1,i2-0,i3
                    !    write(6,*)'is1,0,0=',is1,0,0
                    !  end if
                      !   u(i1-is1,i2-0,i3,n)=(1.-alpha)*du3+alpha*du2
                      u(i1-is1-is1,i2-0-0,i3,n)=(1.-alpha)*du3+alpha*
     & du1
                  end do
                end if
              end if
              !  extrapolate in the i2 direction
              if (boundaryCondition(side2,axis2).gt.0) then
                if (mask(i1,i2,i3).ne.0) then
                  do n=0,numberOfComponents-1
                   ! u(i1-is1,i2-2*is2,i3,n)=extrap3(u,i1-is1,i2-is2,i3,n,0,is2,0)
                      ! here du2=2nd-order approximation, du3=third order
                      ! Blend the 2nd and 3rd order based on the difference 
                      !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                      du1 = u(i1-0,i2-is2,i3,n)
                      du2 = 2.*u(i1-0,i2-is2,i3,n)-u(i1-0+0,i2-is2+is2,
     & i3,n)
                      du3 = 3.*u(i1-0,i2-is2,i3,n)-3.*u(i1-0+0,i2-is2+
     & is2,i3,n)+u(i1-0+2*0,i2-is2+2*is2,i3,n)
                      !   alpha = cdl*(abs(du3-u(i1-0+0,i2-is2+is2,i3,n))+abs(du3-du2))/(uEps+abs(u(i1-0+0,i2-is2+is2,i3,n))+abs(u(i1-0+2*0,i2-is2+2*is2,i3,n)))
                      ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1-0+0,i2-is2+is2,i3,n))+abs(u(i1-0+2*0,i2-is2+2*is2,i3,n)))
                      uNorm= uEps+ abs(du3) + abs(u(i1-0,i2-is2,i3,n))+
     & abs(u(i1-0+0,i2-is2+is2,i3,n))
                      ! **  du = abs(du3-u(i1-0+0,i2-is2+is2,i3,n))/uNorm  ! changed 050711
                      ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                      alpha = cdl*( abs(du3-du2)/uNorm )
                      alpha =min(1.,alpha)
                      ! if( mm.eq.1 )then
                    !  if (alpha.gt.0.9) then
                    !    write(6,*)'limiting, n,du1,du3=',n,du1,du3
                    !    write(6,*)'i1-0,i2-is2,i3=',i1-0,i2-is2,i3
                    !    write(6,*)'0,is2,0=',0,is2,0
                    !  end if
                      !   u(i1-0,i2-is2,i3,n)=(1.-alpha)*du3+alpha*du2
                      u(i1-0-0,i2-is2-is2,i3,n)=(1.-alpha)*du3+alpha*
     & du1
                  end do
                end if
              end if
              !  extrapolate in the diagonal direction
              if (boundaryCondition(side1,axis1)
     & .gt.0.and.boundaryCondition(side2,axis2).gt.0) then
                if (mask(i1,i2,i3).ne.0) then
                  do n=0,numberOfComponents-1
                   ! u(i1-2*is1,i2-2*is2,i3,n)=extrap3(u,i1-is1,i2-is2,i3,n,is1,is2,0)
                      ! here du2=2nd-order approximation, du3=third order
                      ! Blend the 2nd and 3rd order based on the difference 
                      !   (which equals the second difference: uNew(-1)-2*u(0)+u(1))
                      du1 = u(i1-is1,i2-is2,i3,n)
                      du2 = 2.*u(i1-is1,i2-is2,i3,n)-u(i1-is1+is1,i2-
     & is2+is2,i3,n)
                      du3 = 3.*u(i1-is1,i2-is2,i3,n)-3.*u(i1-is1+is1,
     & i2-is2+is2,i3,n)+u(i1-is1+2*is1,i2-is2+2*is2,i3,n)
                      !   alpha = cdl*(abs(du3-u(i1-is1+is1,i2-is2+is2,i3,n))+abs(du3-du2))/(uEps+abs(u(i1-is1+is1,i2-is2+is2,i3,n))+abs(u(i1-is1+2*is1,i2-is2+2*is2,i3,n)))
                      ! alpha = cdl*(abs(du3-du2))/(.1+abs(u(i1-is1+is1,i2-is2+is2,i3,n))+abs(u(i1-is1+2*is1,i2-is2+2*is2,i3,n)))
                      uNorm= uEps+ abs(du3) + abs(u(i1-is1,i2-is2,i3,n)
     & )+abs(u(i1-is1+is1,i2-is2+is2,i3,n))
                      ! **  du = abs(du3-u(i1-is1+is1,i2-is2+is2,i3,n))/uNorm  ! changed 050711
                      ! **  alpha = cdl*( du**2 + abs(du3-du2)/uNorm )
                      alpha = cdl*( abs(du3-du2)/uNorm )
                      alpha =min(1.,alpha)
                      ! if( mm.eq.1 )then
                    !  if (alpha.gt.0.9) then
                    !    write(6,*)'limiting, n,du1,du3=',n,du1,du3
                    !    write(6,*)'i1-is1,i2-is2,i3=',i1-is1,i2-is2,i3
                    !    write(6,*)'is1,is2,0=',is1,is2,0
                    !  end if
                      !   u(i1-is1,i2-is2,i3,n)=(1.-alpha)*du3+alpha*du2
                      u(i1-is1-is1,i2-is2-is2,i3,n)=(1.-alpha)*du3+
     & alpha*du1
                  end do
                end if
              end if
            end do
          end do
         !******* Assign symmetry BC on ghost line 2 
         ! NOTE: we really only need to do the extended boundary and extended ghost??
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
           if( boundaryCondition(side,axis).eq.symmetry )then
            ! even symmetry 
            js1=is1*2
            js2=is2*2
            if( twilightZone.eq.0 )then
              i3=n3a
              do i2=nn2a,nn2b
              do i1=nn1a,nn1b
              if (mask(i1,i2,i3).ne.0) then
               do n=0,numberOfComponents-1
                 u(i1-js1,i2-js2,i3,n)=u(i1+js1,i2+js2,i3,n)
               end do
              end if
              end do
              end do
            else
             ! TZ :
              i3=n3a
              do i2=nn2a,nn2b
              do i1=nn1a,nn1b
              if (mask(i1,i2,i3).ne.0) then
               do n=0,numberOfComponents-1
                 call ogDeriv(ep,0,0,0,0,xy(i1-js1,i2-js2,i3,0),xy(i1-
     & js1,i2-js2,i3,1),0.,t,n,uem)
                 call ogDeriv(ep,0,0,0,0,xy(i1+js1,i2+js2,i3,0),xy(i1+
     & js1,i2+js2,i3,1),0.,t,n,uep)
                 u(i1-js1,i2-js2,i3,n)=u(i1+js1,i2+js2,i3,n) + uem - 
     & uep
               end do
              end if
              end do
              end do
            end if
           end if ! bc
           end do ! end side
           end do ! end axis
          if( debug.gt.32 )then
           n1a=gridIndexRange(0,0)
           n1b=gridIndexRange(1,0)
           n2a=gridIndexRange(0,1)
           n2b=gridIndexRange(1,1)
           n3a=gridIndexRange(0,2)
           n3b=gridIndexRange(1,2)
            write(*,'("v1c",1x,"AtEND")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,v1c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,v1c),i1=n1a-2,n1b+
     & 2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("v2c",1x,"AtEND")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,v2c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,v2c),i1=n1a-2,n1b+
     & 2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("s11c",1x,"AtEND")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s11c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s11c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("s12c",1x,"AtEND")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s12c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s12c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'("s22c",1x,"AtEND")')
           ! write(*,'(10(10e18.10,/))') (((u(i1,i2,i3,s22c),i1=n1a-2,n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
            write(*,'(10(10e14.6,/))') (((u(i1,i2,i3,s22c),i1=n1a-2,
     & n1b+2),i2=n2a-2,n2b+2),i3=n3a,n3b)
          end if
        return
        end
