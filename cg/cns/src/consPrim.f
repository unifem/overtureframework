! This file automatically generated from consPrim.bf with bpp.
c=====================================================================================
c  Convert between conservative and primitive variables 
c
c
c  nd,ns : number of dimensions, number of species
c  q(i1,i2,i3,0:c) : field values to convert
c  rc,uc,vc,tc : component numbers for rho, u,v,w and T
c  val(0:*) : default values for each component where mask==0 (if fixup==1)
c option : 0= primitive to conservative. 1=cons to prim
c fixup : 1= fixup unused points
c
c *****************************************************************************************************
c IMPORTANT: 
c    Generally the conservative <--> primitive transformation must be done at ALL points
c (not just mask.ne.0) since we also must convert the interpolation neighbours and exposed points !!
c  wdh 040803
c *****************************************************************************************************
c=====================================================================================

c#defineMacro multigam(a) (((gam1-1.)*(gam2-1.))/((a)*(gam1-1.)+(1.-(a))*(gam2-1.))+1.)
c
c#defineMacro multipie(a) (((pi1*cv1*(a))+(pi2*cv2*(1.0-(a))))/((cv1*(a))+(cv2*(1.0-(a)))))

c===============================================================================
c This macro checks the density and adjusts it if it is negative or too small
c===============================================================================



c *********************************************************
c *************** NEW VERSION *****************************
c *********************************************************
c
c FIXUP : set to "fixup" to set values where mask==0





c *********************************************************
c *************** OLD VERSION *****************************
c *********************************************************



      subroutine consPrim(nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &     n1a,n1b,n2a,n2b,n3a,n3b, nd,ns, rc,uc,vc,wc,tc,sc, q,
     &     mask, val,ipar,rpar, option, fixup, epsRho )

      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer n1a,n1b,n2a,n2b,n3a,n3b,nd,ns
      real q(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer ipar(0:*),rc,uc,vc,wc,tc,sc,option,fixup
      real val(0:*), rpar(0:*)

c      -- -local variables ---
      integer conservativeGodunov
      parameter( conservativeGodunov=2 )

      integer fortranVersion,multiComponentVersion,multiFluidVersion
      parameter( fortranVersion=0, ! this is dudr
     &           multiComponentVersion=3,  ! Jeff Banks
     &           multiFluidVersion=4       ! new multifluid Godunov
     &         )
c
      include 'eosDefine.h'  ! define the EOS names, idealGasEOS, jwlEOS, mieGruneisenEOS, userDefinedEOS
c
      integer DonsVersion
      parameter( DonsVersion=1 )

      integer noReactions,oneStep,branching,ignitionAndGrowth,
     & oneStepPress,
     & oneEquationMixtureFraction,
     & twoEquationMixtureFraction,
     & chemkinReaction,
     & igDesensitization,
     & ignitionPressureReactionRate
      parameter( noReactions=0,oneStep=1,branching=2,oneStepPress=7,
     &           ignitionAndGrowth=3,
     & oneEquationMixtureFraction=4,
     & twoEquationMixtureFraction=5,
     & chemkinReaction=6,
     & igDesensitization=8,
     & ignitionPressureReactionRate=9 )

      integer compressibleMultiphase
      parameter (compressibleMultiphase=5)

      real Rg,gamma,Rgg,fact,heatRelease,absorbedEnergy,gammaRg,epsRho
      integer i1,i2,i3,is,pdeVariation,reactionType,equationOfState,
     & imask,n
      integer conservativeGodunovMethod,useNewVersion,multiVersion

      integer pde

c     these are floor values used for the mixture JWL eos
      real rmin,pmin

      real rhoi
c
c     these are for multifluid, ignition-pressure reaction rate option
      real alam,amu1,amu2,amu3
c
c     these are variables used for geteos (for mixture JWL eos)
      real r,y,e,vs,vg,p,dp(10)
      integer ier,i
c
c     these are needed for desensitization
      real phi
      integer ivi,ivs,ivg
c
c     these are values needed for multicomponent case
      real gam1,gam2,cv1,cv2,pi1,pi2,v1,v2,lambda,gam,pie
      real gami, gamr, mu, omegai, omegar, cvi, cvr, gm1Inv
c
c     new values for multicomponent case
      real pii,pir,rho
      integer istiff
      include 'multiDat.h'
c
c     these are mixture JWL eos values in common with geteos
      real omeg,ajwl,rjwl,vs0,ts0,fsvs0,zsvs0,vg0,fgvg0,zgvg0,cgcs,
     & heat,eospar
      real mgp1,mgp2,v0,vn,mgkappa ! variables for Mie-Gruneisen EOS
      include 'eosdat.h'
c      common / eosdat / omeg(2),ajwl(2,2),rjwl(2,2),vs0,ts0,
c     *                  fsvs0,zsvs0,vg0,fgvg0,zgvg0,cgcs,heat
c     these are values are set through the cmd file
      include 'multijwl.h'

      ! user defined EOS class pointer
      include 'eosUserDefined.h'

c multiphase parameters
      real asmin,compac,alps,alpg,ralps,ralpg,rsi,rgi
      real gamc,gm1,gp1,em,ep,ps0,bgas
      real astiny,pgtiny
      common / gasdat / gamc(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas

c four component parameters
      integer fourComp
      real fgam1,fgam2,fgam3,fgam4,fcv1,fcv2,fcv3,fcv4,c1h,c2h,c3h,c4h,
     & flam,fmu

      integer asc,rsc,usc,vsc,tsc,rgc,ugc,vgc,tgc
      integer mu1c,mu2c,lamc,mfsolid
c
c  subroutine geteosb (rho,e,mu,lam,vi,vs,vg,p,dp,iform,ier)
c  parameters for call to multicomponent JWL EOS code
      real mrho,mmu,mlam,mvi,mvs,mvg
c
      parameter (asmin=1.e-5)
c
      ! variables for user defined EOS:
      integer iparEOS(5),eosOption,eosDerivOption,nq
      real rparEOS(5)
      real qv(0:20) ! holds state
c
      real pStiff,gammaStiff

      Rg=rpar(0)
      gamma=rpar(1)
      heatRelease=rpar(2)
      absorbedEnergy=rpar(3)

      pdeVariation=ipar(0)
      reactionType=ipar(1)
      equationOfState=ipar(2)
      conservativeGodunovMethod=ipar(3)
      multiVersion=ipar(17)
      fourComp = ipar(20)
      istiff=ipar(18)
      if( istiff.ne.0 .and. istiff.ne.1 )then
        write(*,'(" consPrim:ERROR: istiff not set.")')
        stop 9983
      end if
      pde=ipar(4)

      ! nq : number of state values to pass to the user defined EOS
      nq = 2 + nd + ns  ! rho+ E+ [velocity] + [species] *fix me*
      if( nq.gt.20 )then
        write(*,'(" consPrim:ERROR: nq is too big for qv array -- fix 
     & me --")')
      end if


      if (pde.eq.compressibleMultiphase) then
        rsc=0
        usc=1
        vsc=2
        tsc=3
        rgc=4
        ugc=5
        vgc=6
        tgc=7
        asc=8    ! assumed components

c multifluid solid
        mu1c=9
        mu2c=10
        lamc=11
        if( conservativeGodunovMethod.eq.multiFluidVersion )then
          mfsolid=1
        else
          mfsolid=0
        end if

      end if

      if (conservativeGodunovMethod.eq.multiComponentVersion) then
        if( multiVersion.eq.DonsVersion ) then
!           -- using Don's version
          if (istiff.ne.0) then
            pii=rpar(44)
            pir=rpar(45)
          end if
          if( fourComp.eq.1 ) then
            fgam1 = rpar(50)
            fcv1  = rpar(51)
            fgam2 = rpar(52)
            fcv2  = rpar(53)
            fgam3 = rpar(54)
            fcv3  = rpar(55)
            fgam4 = rpar(56)
            fcv4  = rpar(57)
c            write(6,*)fgam1,fcv1
c            write(6,*)fgam2,fcv2
c            write(6,*)fgam3,fcv3
c            write(6,*)fgam4,fcv4
          else
            ! two component
            gami=rpar(40)
            gamr=rpar(41)
            cvi=rpar(42)
            cvr=rpar(43)
            if( reactionType.eq.igDesensitization ) then
              ivi=sc+3
              ivs=sc+4
              ivg=sc+5
            else if( reactionType.eq.ignitionAndGrowth ) then
              ivi=sc+2
              ivs=sc+3
              ivg=sc+4
            end if
          end if
        else
c         using Jeffs version
          gam1=rpar(4)
          gam2=rpar(5)
          cv1=rpar(6)
          cv2=rpar(7)
          pi1=rpar(8)
          pi2=rpar(9)
        endif
      else
        if( reactionType.eq.igDesensitization ) then
          ivs=sc+2
          ivg=sc+3
        else if( reactionType.eq.ignitionAndGrowth ) then
          ivs=sc+1
          ivg=sc+2
        end if
      end if

c      if (pdeVariation.eq.conservativeGodunov .and. equationOfState.eq.jwlEOS) then
c        omeg(1)=rpar(4)
c        ajwl(1,1)=rpar(5)
c        ajwl(2,1)=rpar(6)
c        rjwl(1,1)=rpar(7)
c        rjwl(2,1)=rpar(8)
c        omeg(2)=rpar(9)
c        ajwl(1,2)=rpar(10)
c        ajwl(2,2)=rpar(11)
c        rjwl(1,2)=rpar(12)
c        rjwl(2,2)=rpar(13)
c        vs0=rpar(14)
c        ts0=1.0
c        zsvs0=vs0*(ajwl(1,1)*exp(-rjwl(1,1)*vs0)
c     *            +ajwl(2,1)*exp(-rjwl(2,1)*vs0))/omeg(1)
c        fsvs0=zsvs0-ajwl(1,1)*exp(-rjwl(1,1)*vs0)/rjwl(1,1)
c     *            -ajwl(2,1)*exp(-rjwl(2,1)*vs0)/rjwl(2,1)
c        vg0=rpar(15)
c        zgvg0=vg0*(ajwl(1,2)*exp(-rjwl(1,2)*vg0)
c     *            +ajwl(2,2)*exp(-rjwl(2,2)*vg0))/omeg(2)
c        fgvg0=zgvg0-ajwl(1,2)*exp(-rjwl(1,2)*vg0)/rjwl(1,2)
c     *             -ajwl(2,2)*exp(-rjwl(2,2)*vg0)/rjwl(2,2)
c        cgcs=rpar(16)
c        heat=rpar(17)
c      end if
      if( equationOfState.eq.mieGruneisenEOS )then
        mgp1=eospar(1)  ! alpha
        mgp2=eospar(2)  ! beta
        v0  =eospar(3)
        if( .false. ) write(*,'(" consPrim: mieGruneisen: alpha,beta,
     & v0=",3f6.2)') mgp1,mgp2,v0
        ! '
      end if

      Rgg=Rg/(gamma-1.)
      gammaRg=(gamma-1.)/Rg

      useNewVersion=1 ! set to 0 to use old version

c       write(6,*)'consPrim(in), option,pde=',option,pde
c       write(6,'(9(1x,i1,1x,f15.8,/))')(i,q(0,0,0,i),i=0,8)
c       write(6,*)gamc,gm1
c       pause


      if( fixup.eq.0 )then

        if( useNewVersion.eq.1 )then
          if( option.eq.0 )then
            ! ***************************************
            ! ****** Primitive to Conservative ******
            ! ***************************************
            if( pde.eq.compressibleMultiphase ) then
c    write(6,*)'here i am (1)',mfsolid
c    do i=0,11
c      write(6,*)i,q(n1a,n2a,n3a,i)
c    end do
c    pause
c here is the new multiphase option
c first step: thermodynamics => convert Tk=pk/rk to ek, k=s or g
              astiny=1.e-3
              pgtiny=1.e-3
              if (mfsolid.eq.0) then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    q(i1,i2,i3,tsc)=(q(i1,i2,i3,tsc)+ps0*gamc(1)/q(i1,
     & i2,i3,rsc))/gm1(1)+compac(q(i1,i2,i3,asc),0)
                    q(i1,i2,i3,tgc)=q(i1,i2,i3,tgc)/(gm1(2)*(1.0+bgas*
     & q(i1,i2,i3,rgc)))
                    if (q(i1,i2,i3,asc).lt.astiny) then
                      if (q(i1,i2,i3,tgc).lt.pgtiny/q(i1,i2,i3,rgc)) 
     & then
                        q(i1,i2,i3,tgc)=(pgtiny/q(i1,i2,i3,rgc))/(gm1(
     & 2)*(1.0+bgas*q(i1,i2,i3,rgc)))
                      end if
                    end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    q(i1,i2,i3,tsc)=q(i1,i2,i3,tsc)*q(i1,i2,i3,mu1c)+q(
     & i1,i2,i3,mu2c)/q(i1,i2,i3,rsc)+compac(q(i1,i2,i3,asc),0)
                    q(i1,i2,i3,tgc)=q(i1,i2,i3,tgc)/(gm1(2)*(1.0+bgas*
     & q(i1,i2,i3,rgc)))
                    if (q(i1,i2,i3,asc).lt.astiny) then
                      if (q(i1,i2,i3,tgc).lt.pgtiny/q(i1,i2,i3,rgc)) 
     & then
                        q(i1,i2,i3,tgc)=(pgtiny/q(i1,i2,i3,rgc))/(gm1(
     & 2)*(1.0+bgas*q(i1,i2,i3,rgc)))
                      end if
                    end if
                end do
                end do
                end do
              end if
c    do i=0,11
c      write(6,*)i,q(n1a,n2a,n3a,i)
c    end do
c    pause
c    write(6,*)(q(0,0,0,i),i=0,8)
c    pause
c second step : kinematics => convert (ek,uk) to (rk*Ek,rk*uk), k=s or g  (2d is assumed)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  q(i1,i2,i3,tsc)=q(i1,i2,i3,rsc)*(q(i1,i2,i3,tsc)+.5*(
     & q(i1,i2,i3,usc)**2+q(i1,i2,i3,vsc)**2))
                  q(i1,i2,i3,usc)=q(i1,i2,i3,usc)*q(i1,i2,i3,rsc)
                  q(i1,i2,i3,vsc)=q(i1,i2,i3,vsc)*q(i1,i2,i3,rsc)
                  q(i1,i2,i3,tgc)=q(i1,i2,i3,rgc)*(q(i1,i2,i3,tgc)+.5*(
     & q(i1,i2,i3,ugc)**2+q(i1,i2,i3,vgc)**2))
                  q(i1,i2,i3,ugc)=q(i1,i2,i3,ugc)*q(i1,i2,i3,rgc)
                  q(i1,i2,i3,vgc)=q(i1,i2,i3,vgc)*q(i1,i2,i3,rgc)
              end do
              end do
              end do
c    write(6,*)(q(0,0,0,i),i=0,8)
c    pause
c third step : multiply by volume fraction
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  alps=q(i1,i2,i3,asc)
                  alpg=1.0-alps
                  q(i1,i2,i3,rsc)=alps*q(i1,i2,i3,rsc)
                  q(i1,i2,i3,usc)=alps*q(i1,i2,i3,usc)
                  q(i1,i2,i3,vsc)=alps*q(i1,i2,i3,vsc)
                  q(i1,i2,i3,tsc)=alps*q(i1,i2,i3,tsc)
                  q(i1,i2,i3,rgc)=alpg*q(i1,i2,i3,rgc)
                  q(i1,i2,i3,ugc)=alpg*q(i1,i2,i3,ugc)
                  q(i1,i2,i3,vgc)=alpg*q(i1,i2,i3,vgc)
                  q(i1,i2,i3,tgc)=alpg*q(i1,i2,i3,tgc)
              end do
              end do
              end do
c    write(6,*)(q(0,0,0,i),i=0,8)
c    pause
c    do i=0,11
c      write(6,*)i,q(n1a,n2a,n3a,i)
c    end do
c    pause
            else
c first step: thermodynamics => convert (rho,T,lambda,mu) to (rho,e,rho*lambda,mu)
c
c             where rho=density
c                   T=temperature (perhaps just p/rho)
c                   e=internal energy (per unit volume)
c                   lambda=species fractions
c                   mu=any "steady state" variables (such as vs,vg for mixture JWL eos)
              if( pdeVariation.eq.conservativeGodunov )then
               if( conservativeGodunovMethod.eq.multiFluidVersion )then
c handles cmfdu
                    ! --------- Multi-fluid Godunov ---------
                    if( equationOfState.eq.idealGasEOS )then
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                         ! rho*e = mu1*p = mu1*rho*T
                         q(i1,i2,i3,tc)=q(i1,i2,i3,sc)*q(i1,i2,i3,rc)*
     & q(i1,i2,i3,tc)
                     end do
                     end do
                     end do
                    else if( equationOfState.eq.stiffenedGasEOS )then
                     if( reactionType.eq.noReactions )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                          ! rho*e = mu1*p + mu2 = mu1*rho*T+mu2
                          q(i1,i2,i3,tc)=q(i1,i2,i3,sc)*q(i1,i2,i3,rc)*
     & q(i1,i2,i3,tc) + q(i1,i2,i3,sc+1)
          ! might want to set q(i1,i2,i3,tc)=q(i1,i2,i3,sc)*q(i1,i2,i3,rc)*max(q(i1,i2,i3,tc),0.) + q(i1,i2,i3,sc+1)
                      end do
                      end do
                      end do
                     elseif( 
     & reactionType.eq.ignitionPressureReactionRate )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                          ! rho*e = mu1*p + mu2 + rho*mu3 = rho*(mu1*T+mu3)+mu2
                          alam=q(i1,i2,i3,sc)
                          amu1=(1.0-alam)*q(i1,i2,i3,sc+1)+alam*q(i1,
     & i2,i3,sc+2)
                          amu2=(1.0-alam)*q(i1,i2,i3,sc+3)+alam*q(i1,
     & i2,i3,sc+4)
                          amu3=                            alam*q(i1,
     & i2,i3,sc+5)
                          q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*(amu1*q(i1,i2,
     & i3,tc)+amu3)+amu2
          ! might want to set q(i1,i2,i3,tc)=q(i1,i2,i3,sc)*q(i1,i2,i3,rc)*max(q(i1,i2,i3,tc),0.) + q(i1,i2,i3,sc+1)
                      end do
                      end do
                      end do
                     else
                       write(*,*) ' consPrim:ERROR: multifluid unknown 
     & reaction rate'
                       stop 9015
                     end if
                    else
                     write(*,*) 'consPrim:ERROR: multifluid unknown 
     & EOS'
                     stop 9016
                    end if
               else
c handles all of dudr2d and dudr3d cases
c mixture JWL eos (also handles heat release contribution to the energy)
                if( equationOfState.eq.jwlEOS )then
                  ! write(55,*)'consPrim(p->c)'
                  ! DWS 4/28/04
                  rmin=1.e-2
                  pmin=1.e-3
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                    ! compute specific internal energy en given (rho,p,lambda), possibly update vs and vg
                      !   write(55,*)i1,i2,(q(i1,i2,i3,i),i=0,6)
                      ier=0
                      r=q(i1,i2,i3,rc)
                      ! if (r.lt.1.e-5) then
                      !   write(6,*)'q =',(q(i1,i2,i3,i),i=0,6)
                      !   write(6,*)'mask =',mask(i1,i2,i3)
                      !   write(6,*)'(1) fixup =',fixup
                      !   stop
                      ! end if
                      ! DWS 4/28/04
                      if (r.lt.rmin) then
                        q(i1,i2,i3,rc)=rmin
                        q(i1,i2,i3,sc)=1.
                        q(i1,i2,i3,tc)=pmin/(Rg*rmin)
                        ! write(6,*)'Warning (consPrim) : low density fix, r=',r
                      end if
                      if( 
     & conservativeGodunovMethod.eq.multiComponentVersion )then
                        mrho=r
                        p=Rg*r*q(i1,i2,i3,tc)
                        mmu=q(i1,i2,i3,sc)
                        mlam=q(i1,i2,i3,sc+1)
c              mvi=q(i1,i2,i3,sc+2)
c              mvs=q(i1,i2,i3,sc+3)
c              mvg=q(i1,i2,i3,sc+4)
                        mvi=q(i1,i2,i3,ivi)
                        mvs=q(i1,i2,i3,ivs)
                        mvg=q(i1,i2,i3,ivg)
                        if( newMethod.eq.0 ) then
                          call geteosb( mrho,e,mmu,mlam,mvi,mvs,mvg,p,
     & dp,-1,ier )
                        else
                          call geteosc( mrho,e,mmu,mlam,mvi,mvs,mvg,p,
     & dp,-1,ier )
                        end if
                        if (ier.ne.0) then
                          write(6,*)'Error (consPrim) : call to 
     & geteosb failed'
                          stop
                        end if
                        q(i1,i2,i3,tc)=e*mrho
                        q(i1,i2,i3,sc)=mrho*mmu
                        q(i1,i2,i3,sc+1)=mrho*mlam
                        if( reactionType.eq.igDesensitization ) then
                          q(i1,i2,i3,sc+2)=mrho*q(i1,i2,i3,sc+2)
                        end if
c              q(i1,i2,i3,sc+2)=mvi
c              q(i1,i2,i3,sc+3)=mvs
c              q(i1,i2,i3,sc+4)=mvg
                        q(i1,i2,i3,ivi)=mvi
                        q(i1,i2,i3,ivs)=mvs
                        q(i1,i2,i3,ivg)=mvg
                      else
                        y=r*q(i1,i2,i3,sc)
c              vs=q(i1,i2,i3,sc+1)
c              vg=q(i1,i2,i3,sc+2)
                        vs=q(i1,i2,i3,ivs)
                        vg=q(i1,i2,i3,ivg)
                        p=Rg*r*q(i1,i2,i3,tc)
                        call geteos (r,e,y,vs,vg,p,dp,-1,ier)
                        if (ier.ne.0) then
                          write(6,*)'Error (consPrim) : call to geteos 
     & failed (p->c)'
                                          ! '
                          stop
                        end if
                        q(i1,i2,i3,tc)=e
                        q(i1,i2,i3,sc)=y
                        if( reactionType.eq.igDesensitization) then
                          q(i1,i2,i3,sc+1)=r*q(i1,i2,i3,sc+1)
                        end if
c              q(i1,i2,i3,sc+1)=vs
c              q(i1,i2,i3,sc+2)=vg
                        q(i1,i2,i3,ivs)=vs
                        q(i1,i2,i3,ivg)=vg
                      end if
                  end do
                  end do
                  end do
c now do ideal and Mie-Gruneisen eos cases
                else if( equationOfState.eq.idealGasEOS .or. 
     & equationOfState.eq.mieGruneisenEOS .or. 
     & equationOfState.eq.userDefinedEOS .or. 
     & equationOfState.eq.stiffenedGasEOS 
     & .or.equationOfState.eq.taitEOS )then
c first just Euler
                  if( 
     & conservativeGodunovMethod.eq.multiComponentVersion )then
                    ! multicomponent
                    if( multiVersion.eq.DonsVersion ) then
                      ! Use multi-component version in dudr
                      if (istiff.eq.0) then
                        ! non-stiff multi-component
                        if( fourComp.eq.1 ) then
                          ! four component
                          do i3=n3a,n3b
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                             fmu = q(i1,i2,i3,sc)
                             flam = q(i1,i2,i3,sc+1)
                             c1h = flam*fcv1*fgam1+(1.e0-flam)*fcv2*
     & fgam2
                             c2h = flam*fcv3*fgam3+(1.e0-flam)*fcv4*
     & fgam4
                             c3h = flam*fcv1+(1.e0-flam)*fcv2
                             c4h = flam*fcv3+(1.e0-flam)*fcv4
                             gm1Inv = 1.e0/((fmu*c1h+(1.e0-fmu)*c2h)/(
     & fmu*c3h+(1.e0-fmu)*c4h)-1.e0)
                             q(i1,i2,i3,tc)=(q(i1,i2,i3,rc)*Rg*q(i1,i2,
     & i3,tc))*gm1Inv
                          end do
                          end do
                          end do
                        else
                          ! 2 component multi-component
                          do i3=n3a,n3b
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                              ! e = P*(mu/omegar+(1-mu)/omegai)
                              mu = q(i1,i2,i3,sc)
                              omegai=gami-1.d0
                              omegar=gamr-1.d0
                              if( cvi.lt.0.d0 ) then
                                gm1Inv=(mu/omegar+(1.d0-mu)/omegai)
                              else
                                gm1Inv=(mu*cvr+(1.d0-mu)*cvi)/(mu*cvr*
     & omegar+(1.d0-mu)*cvi*omegai)
                              endif
                              q(i1,i2,i3,tc)=(q(i1,i2,i3,rc)*Rg*q(i1,
     & i2,i3,tc))*gm1Inv
                           end do
                           end do
                           end do
                        end if
                      else
                       ! stiffened multi-component
                        ! DWS 5/5/09
                        rmin=1.e-2
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                            if (q(i1,i2,i3,rc).lt.rmin) then
                              q(i1,i2,i3,rc)=rmin
                            end if
                            if (q(i1,i2,i3,tc).lt.0.0) then
                              q(i1,i2,i3,tc)=0.0
                            end if
                            if (q(i1,i2,i3,sc).lt.0.0) then
                              q(i1,i2,i3,sc)=0.0
                            else
                              if (q(i1,i2,i3,sc).gt.1.0) then
                                q(i1,i2,i3,sc)=1.0
                              end if
                            end if
                            rho=q(i1,i2,i3,rc)
                            p=Rg*q(i1,i2,i3,rc)*q(i1,i2,i3,tc)
                            mu=q(i1,i2,i3,sc)
                            call geteosm (rho,e,mu,p,dp,-1,ier)
                            q(i1,i2,i3,tc)=rho*e
                        end do
                        end do
                        end do
                      end if  ! end stiffened multi-component
                    else !  multiVersion.ne.DonsVersion
                      ! Jeff's multi-component version
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                          ! e = (p+gamma*pi)/(gamma-1)
                          lambda = q(i1,i2,i3,sc)
                          gam = (((gam1*cv1*(lambda))+(gam2*cv2*(1.0-(
     & lambda))))/((cv1*(lambda))+(cv2*(1.0-(lambda)))))
                          pie = (pi1*(lambda)+pi2*(1.0-(lambda)))
                          q(i1,i2,i3,tc)=(Rg*q(i1,i2,i3,rc)*q(i1,i2,i3,
     & tc)+gam*pie)/((gam-1.0))
                      end do
                      end do
                      end do
                    end if
                  else if( equationOfState.eq.idealGasEOS )then
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                        ! rho*e = p/(gamma-1) = rho*Rg*T/(gamma-1)
                        q(i1,i2,i3,tc)=Rgg*q(i1,i2,i3,rc)*q(i1,i2,i3,
     & tc)
                    end do
                    end do
                    end do
                  else if( equationOfState.eq.mieGruneisenEOS )then
                    ! Mie Gruneisen EOS
                    mgkappa=eospar(4)  !  Cp = Cv + kappa*R
                    ! write(*,'(" consprim: eosPar(4)=kappa=",e10.3)') eospar(4)
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                        ! rho*e = rho*kappa*R*T/(gamma-1) + E_c
                        !       = rho*Cv*T + E_c/rho  with Cv = kappa* Rg/(gamma-1)
                        vn=1./(q(i1,i2,i3,rc)*v0)
                        ! *wdh* 050108 -- changed sign of Ec
                        q(i1,i2,i3,tc)=Rgg*mgkappa*q(i1,i2,i3,rc)*q(i1,
     & i2,i3,tc) - ((vn-1.)**2)*( .5*mgp1 + (mgp2/3.)*(vn-1.) )/vn
                    end do
                    end do
                    end do
                  else if( equationOfState.eq.stiffenedGasEOS )then
                    ! Stiffened EOS *ve* 071030 
                    gammaStiff=eosPar(1)
                    pStiff=eosPar(2)
                    ! write(*,'(" consprim: stiffened gamma,p0=",2e10.3)') gammaStiff,pStiff
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                        ! *ve* rho*e = (rho*T-gammaStiff*pStiff)/(gammaStiff-1)
                        ! *ve* rho*e = (rho*T + gammaStiff*pStiff)/(gammaStiff-1)  *wdh* 
                         q(i1,i2,i3,tc)=(q(i1,i2,i3,rc)*q(i1,i2,i3,tc)+
     & (gammaStiff*pStiff))/(gammaStiff-1.)
                    end do
                    end do
                    end do
                  else if( equationOfState.eq.taitEOS )then
                    stop 2745
                  else if( equationOfState.eq.userDefinedEOS )then
                    eosOption=0      ! get e=e(r,e)
                    eosDerivOption=0 ! no derivatives needed
                    iparEOS(1)=nd
                    ier = 0
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                        ! first get e from rho and p
                        r =q(i1,i2,i3,rc)
                        p = r*q(i1,i2,i3,tc)
                        do n=0,nq-1
                          qv(n)=q(i1,i2,i3,n)
                        end do
                        call getUserDefinedEOS( r,e,p,dp, eosOption, 
     & eosDerivOption, qv,iparEOS,rparEOS,userEOSDataPointer, ier )
                        q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*e
                    end do
                    end do
                    end do
                 else
                   write(*,'("consPrim: ERROR - Unknown EOS, 
     & equationOfState=",i6)') equationOfState
                    ! '
                   stop 1432
                 end if
c add psi (placed here but commented out because it is not currently used)
c             ! q(all,all,all,tc)+=rho*psi(rho)
c             fact=1.
c             if( nd.eq.2 )then
c               call addpsi(nd1a,nd1b,nd2a,nd2b,fact,
c    *                      q(nd1a,nd2a,nd3a,rc),
c    *                      q(nd1a,nd2a,nd3a,tc))
c
c             end if
c now do reacting cases
                  ! Species
                  do is=0,ns-1
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                      q(i1,i2,i3,sc+is)=q(i1,i2,i3,sc+is)*q(i1,i2,i3,
     & rc)
                  end do
                  end do
                  end do
                  end do
                  if( reactionType.eq.noReactions.or.fourComp.eq.1 )
     & then
                    ! do nothing
                  else if( 
     & reactionType.eq.oneStep.or.reactionType.eq.oneStepPress )then
                    !  ***** one step *****
                    ! e = e - Q*(rho*product)
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                        if( 
     & conservativeGodunovMethod.eq.multiComponentVersion ) then
                          q(i1,i2,i3,tc)=q(i1,i2,i3,tc) -heatRelease*q(
     & i1,i2,i3,sc+1)*q(i1,i2,i3,sc)/q(i1,i2,i3,rc)
                        else
                          q(i1,i2,i3,tc)=q(i1,i2,i3,tc) -heatRelease*q(
     & i1,i2,i3,sc)
                        endif
                    end do
                    end do
                    end do
                  else if( reactionType.eq.branching )then
                    ! **** chain branching *****
                    ! e = e - [Q*(rho*product) - R*(rho*radical)]
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                        q(i1,i2,i3,tc)=q(i1,i2,i3,tc)-heatRelease*q(i1,
     & i2,i3,sc)-absorbedEnergy*q(i1,i2,i3,sc+1)
                    end do
                    end do
                    end do
                  else
                    write(6,*)'Error (consPrim) : reaction type not 
     & supported'
                    write(*,*)'reactionType=',reactionType
                    stop
                  end if
                else
                  write(6,*)'Error (consPrim) : EOS type not supported'
                  stop
                end if
               end if
              else
c     handles non dudr2d and dudr3d cases (such as Jameson ???)
c    assume just Euler
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    ! e = p/(gamma-1)
                    q(i1,i2,i3,tc)=Rgg*q(i1,i2,i3,rc)*q(i1,i2,i3,tc)
                end do
                end do
                end do
              end if
c second step: kinetics => convert (e,u) to (E,rho*u)
c
c              where u=velocity
c                    E=total energy (per unit volume)
              if( nd.eq.1 )then
                ! *** 1D ***
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  ! E = e + .5*rho*u*u
                    q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+.5*q(i1,i2,i3,rc)*q(
     & i1,i2,i3,uc)**2
                    q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                end do
                end do
                end do
              else if( nd.eq.2 )then
                ! *** 2D ***
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    !  E = e + .5*rho*(u*u+v*v)
                    q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+.5*q(i1,i2,i3,rc)*(q(
     & i1,i2,i3,uc)**2+q(i1,i2,i3,vc)**2)
                    q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                    q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*q(i1,i2,i3,rc)
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    !  E = e + .5*rho*(u*u+v*v+w*w)
                    q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+.5*q(i1,i2,i3,rc)*(q(
     & i1,i2,i3,uc)**2+q(i1,i2,i3,vc)**2+q(i1,i2,i3,wc)**2)
                    q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                    q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*q(i1,i2,i3,rc)
                    q(i1,i2,i3,wc)=q(i1,i2,i3,wc)*q(i1,i2,i3,rc)
                end do
                end do
                end do
              end if
            end if  ! end pde choice
          else
           ! ***************************************
           ! ****** Conservative to Primitive ******
           ! ***************************************
            if( pde.eq.compressibleMultiphase ) then
c here is the new multiphase option
c first step : divide by volume fraction
c    write(6,*)'here i am (2)',mfsolid
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  ralps=1.0/max(q(i1,i2,i3,asc),asmin)
                  ralpg=ralps/(ralps-1.0)
                  q(i1,i2,i3,rsc)=ralps*q(i1,i2,i3,rsc)
                  q(i1,i2,i3,usc)=ralps*q(i1,i2,i3,usc)
                  q(i1,i2,i3,vsc)=ralps*q(i1,i2,i3,vsc)
                  q(i1,i2,i3,tsc)=ralps*q(i1,i2,i3,tsc)
                  q(i1,i2,i3,rgc)=ralpg*q(i1,i2,i3,rgc)
                  q(i1,i2,i3,ugc)=ralpg*q(i1,i2,i3,ugc)
                  q(i1,i2,i3,vgc)=ralpg*q(i1,i2,i3,vgc)
                  q(i1,i2,i3,tgc)=ralpg*q(i1,i2,i3,tgc)
              end do
              end do
              end do
c second step : kinematics => convert (Ek,rk*uk) to (ek,uk), k=s or g  (2d is assumed)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  rsi=1.0/q(i1,i2,i3,rsc)
                  q(i1,i2,i3,usc)=q(i1,i2,i3,usc)*rsi
                  q(i1,i2,i3,vsc)=q(i1,i2,i3,vsc)*rsi
                  q(i1,i2,i3,tsc)=q(i1,i2,i3,tsc)*rsi-.5*(q(i1,i2,i3,
     & usc)**2+q(i1,i2,i3,vsc)**2)
                  rgi=1.0/q(i1,i2,i3,rgc)
                  q(i1,i2,i3,ugc)=q(i1,i2,i3,ugc)*rgi
                  q(i1,i2,i3,vgc)=q(i1,i2,i3,vgc)*rgi
                  q(i1,i2,i3,tgc)=q(i1,i2,i3,tgc)*rgi-.5*(q(i1,i2,i3,
     & ugc)**2+q(i1,i2,i3,vgc)**2)
              end do
              end do
              end do
c third step: thermodynamics => convert ek to Tk=pk/rk, k=s or g
              astiny=1.e-3
              pgtiny=1.e-3
              if (mfsolid.eq.0) then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    rsi=1.0/q(i1,i2,i3,rsc)
                    q(i1,i2,i3,tsc)=gm1(1)*(q(i1,i2,i3,tsc)-compac(q(
     & i1,i2,i3,asc),0))-ps0*gamc(1)*rsi
                    q(i1,i2,i3,tgc)=q(i1,i2,i3,tgc)*gm1(2)*(1.0+bgas*q(
     & i1,i2,i3,rgc))
                    if (q(i1,i2,i3,asc).lt.astiny) then
                      if (q(i1,i2,i3,tgc).lt.pgtiny/q(i1,i2,i3,rgc)) 
     & then
                        q(i1,i2,i3,tgc)=pgtiny/q(i1,i2,i3,rgc)
                      end if
                    end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    rsi=1.0/q(i1,i2,i3,rsc)
                    q(i1,i2,i3,tsc)=(q(i1,i2,i3,tsc)-q(i1,i2,i3,mu2c)*
     & rsi-compac(q(i1,i2,i3,asc),0))/q(i1,i2,i3,mu1c)
                    q(i1,i2,i3,tgc)=q(i1,i2,i3,tgc)*gm1(2)*(1.0+bgas*q(
     & i1,i2,i3,rgc))
                    if (q(i1,i2,i3,asc).lt.astiny) then
                      if (q(i1,i2,i3,tgc).lt.pgtiny/q(i1,i2,i3,rgc)) 
     & then
                        q(i1,i2,i3,tgc)=pgtiny/q(i1,i2,i3,rgc)
                      end if
                    end if
                end do
                end do
                end do
              end if
            else
c first step: kinetics => convert (E,rho*u) to (e,u)
cc   check the density
c    do i3=n3a,n3b
c    do i2=n2a,n2b
c    do i1=n1a,n1b
c      if( q(i1,i2,i3,rc).lt.epsRho )then
c        ! imask=mask(i1,i2,i3)
c        ! if( imask.lt.0 )then
c        !   imask=-1
c        ! else if( imask.gt.0 )then
c        !   imask=1
c        ! end if
c        ! write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
c        if( mask(i1,i2,i3).eq.0 )then
c          q(i1,i2,i3,rc)=1.
c        else
c          q(i1,i2,i3,rc)=epsRho
c        end if
c      end if
c    end do
c    end do
c    end do
              if( nd.eq.1 )then
                ! *** 1D ***
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                     if( q(i1,i2,i3,rc).lt.epsRho )then
                       imask=mask(i1,i2,i3)
                       if( imask.lt.0 )then
                         imask=-1
                       else if( imask.gt.0 )then
                         imask=1
                       end if
c    write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
                       if( mask(i1,i2,i3).eq.0 )then
                         q(i1,i2,i3,rc)=1.
                       else
                         q(i1,i2,i3,rc)=epsRho
                       end if
                     end if
                    rhoi=1./q(i1,i2,i3,rc)
                    ! e = E - .5*rho*u*u
                    q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*rhoi
                    q(i1,i2,i3,tc)=q(i1,i2,i3,tc) -.5*q(i1,i2,i3,rc)*q(
     & i1,i2,i3,uc)**2
                end do
                end do
                end do
              else if( nd.eq.2 )then
                ! *** 2D ***
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    !  e = E - .5*rho*(u*u+v*v)
                     if( q(i1,i2,i3,rc).lt.epsRho )then
                       imask=mask(i1,i2,i3)
                       if( imask.lt.0 )then
                         imask=-1
                       else if( imask.gt.0 )then
                         imask=1
                       end if
c    write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
                       if( mask(i1,i2,i3).eq.0 )then
                         q(i1,i2,i3,rc)=1.
                       else
                         q(i1,i2,i3,rc)=epsRho
                       end if
                     end if
                    rhoi=1./q(i1,i2,i3,rc)
                    q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*rhoi
                    q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*rhoi
                    q(i1,i2,i3,tc)=q(i1,i2,i3,tc)-.5*q(i1,i2,i3,rc)*(q(
     & i1,i2,i3,uc)**2 +q(i1,i2,i3,vc)**2)
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    !  e = E - .5*rho*(u*u+v*v+w*w)
                     if( q(i1,i2,i3,rc).lt.epsRho )then
                       imask=mask(i1,i2,i3)
                       if( imask.lt.0 )then
                         imask=-1
                       else if( imask.gt.0 )then
                         imask=1
                       end if
c    write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
                       if( mask(i1,i2,i3).eq.0 )then
                         q(i1,i2,i3,rc)=1.
                       else
                         q(i1,i2,i3,rc)=epsRho
                       end if
                     end if
                    rhoi=1./q(i1,i2,i3,rc)
                    q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*rhoi
                    q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*rhoi
                    q(i1,i2,i3,wc)=q(i1,i2,i3,wc)*rhoi
                    q(i1,i2,i3,tc)=q(i1,i2,i3,tc)-.5*q(i1,i2,i3,rc)*(q(
     & i1,i2,i3,uc)**2+q(i1,i2,i3,vc)**2+q(i1,i2,i3,wc)**2)
                end do
                end do
                end do
              end if
c second step: thermodynamics => convert (rho,e,rho*lambda,mu) to (rho,T,lambda,mu)
              if( pdeVariation.eq.conservativeGodunov )then
               if( conservativeGodunovMethod.eq.multiFluidVersion )then
c handles cmfdu
                    ! --------- Multi-fluid Godunov ---------
                    if( equationOfState.eq.idealGasEOS )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                          !  T = (rho*e)/(rho*mu1) 
                          q(i1,i2,i3,tc)=q(i1,i2,i3,tc)/(q(i1,i2,i3,rc)
     & *q(i1,i2,i3,sc))
                      end do
                      end do
                      end do
                    else if( equationOfState.eq.stiffenedGasEOS )then
                     if( reactionType.eq.noReactions )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                           !  T = (rho*e - mu2)/(rho*mu1) 
                           q(i1,i2,i3,tc)=(q(i1,i2,i3,tc)-q(i1,i2,i3,
     & sc+1))/(q(i1,i2,i3,rc)*q(i1,i2,i3,sc))
          ! might want to set q(i1,i2,i3,tc) = max(q(i1,i2,i3,tc),0.)
                       end do
                       end do
                       end do
                     elseif( 
     & reactionType.eq.ignitionPressureReactionRate )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                           !  T = (rho*e - mu2 - rho*mu3)/(rho*mu1) 
                           alam=q(i1,i2,i3,sc)
                           amu1=(1.0-alam)*q(i1,i2,i3,sc+1)+alam*q(i1,
     & i2,i3,sc+2)
                           amu2=(1.0-alam)*q(i1,i2,i3,sc+3)+alam*q(i1,
     & i2,i3,sc+4)
                           amu3=                            alam*q(i1,
     & i2,i3,sc+5)
                           q(i1,i2,i3,tc)=(q(i1,i2,i3,tc)-amu2-q(i1,i2,
     & i3,rc)*amu3)/(q(i1,i2,i3,rc)*amu1)
          ! might want to set q(i1,i2,i3,tc) = max(q(i1,i2,i3,tc),0.)
                       end do
                       end do
                       end do
                     else
                       write(*,*)'consPrim:ERROR: multifluid unknown 
     & reaction rate'
                       stop 9015
                     end if
                    else
                     write(*,*) 'consPrim:ERROR: multifluid unknown 
     & EOS'
                     stop 9016
                    end if
               else
c handles all of dudr2d and dudr3d cases
c mixture JWL eos (also handles heat release contribution to the energy)
                if( equationOfState.eq.jwlEOS )then
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                    ! compute pressure p given (rho,en,lambda), possibly update vs and vg
                !     write(55,*)i1,i2,(q(i1,i2,i3,i),i=0,6)
                      ier=0
                      if( 
     & conservativeGodunovMethod.eq.multiComponentVersion )then
                        mrho=q(i1,i2,i3,rc)
                        e=q(i1,i2,i3,tc)/mrho
                        mmu=q(i1,i2,i3,sc)/mrho
                        mlam=q(i1,i2,i3,sc+1)/mrho
c              mvi=q(i1,i2,i3,sc+2)
c              mvs=q(i1,i2,i3,sc+3)
c              mvg=q(i1,i2,i3,sc+4)
                        mvi=q(i1,i2,i3,ivi)
                        mvs=q(i1,i2,i3,ivs)
                        mvg=q(i1,i2,i3,ivg)
                        if( newMethod.eq.0 ) then
                          call geteosb( mrho,e,mmu,mlam,mvi,mvs,mvg,p,
     & dp,0,ier )
                        else
                          call geteosc( mrho,e,mmu,mlam,mvi,mvs,mvg,p,
     & dp,0,ier )
                        end if
                        if (ier.ne.0) then
                          write(6,*)'Error (consPrim) : call to 
     & geteosb failed'
                          stop
                        end if
                        q(i1,i2,i3,tc)=p/(mrho*Rg)
                        q(i1,i2,i3,sc)=mmu
                        q(i1,i2,i3,sc+1)=mlam
                        if( reactionType.eq.igDesensitization ) then
                          q(i1,i2,i3,sc+2)=q(i1,i2,i3,sc+2)/mrho
                        end if
c              q(i1,i2,i3,sc+2)=mvi
c              q(i1,i2,i3,sc+3)=mvs
c              q(i1,i2,i3,sc+4)=mvg
                        q(i1,i2,i3,ivi)=mvi
                        q(i1,i2,i3,ivs)=mvs
                        q(i1,i2,i3,ivg)=mvg
                      else
                        r=q(i1,i2,i3,rc)
                        e=q(i1,i2,i3,tc)
                        y=q(i1,i2,i3,sc)
c              vs=q(i1,i2,i3,sc+1)
c              vg=q(i1,i2,i3,sc+2)
                        vs=q(i1,i2,i3,ivs)
                        vg=q(i1,i2,i3,ivg)
                        call geteos (r,e,y,vs,vg,p,dp,0,ier)
          !     write(55,'(2(1x,i2),9(1x,f11.8))')i1,i2,(q(i1,i2,i3,i),i=0,6),vs,vg
                        if (ier.ne.0) then
                          write(6,*)'Error (consPrim) : call to geteos 
     & failed (c->p)'
                                          ! '
                          stop
                        end if
                        q(i1,i2,i3,tc)=p/(r*Rg)
                        q(i1,i2,i3,sc)=y/r
                        if( reactionType.eq.igDesensitization ) then
                          q(i1,i2,i3,sc+1)=q(i1,i2,i3,sc+1)/r
                        end if
c              q(i1,i2,i3,sc+1)=vs
c              q(i1,i2,i3,sc+2)=vg
                        q(i1,i2,i3,ivs)=vs
                        q(i1,i2,i3,ivg)=vg
                      end if
                  end do
                  end do
                  end do
c now do ideal eos cases
                else if( equationOfState.eq.idealGasEOS .or. 
     & equationOfState.eq.mieGruneisenEOS .or. 
     & equationOfState.eq.userDefinedEOS .or. 
     & equationOfState.eq.stiffenedGasEOS 
     & .or.equationOfState.eq.taitEOS )then
                  if( reactionType.eq.noReactions.or.fourComp.eq.1 )
     & then
                    ! do nothing
                  else if( 
     & reactionType.eq.oneStep.or.reactionType.eq.oneStepPress )then
                    ! **** reacting cases: one step *****
                    ! e = e + Q*(rho*product)
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                        if( 
     & conservativeGodunovMethod.eq.multiComponentVersion ) then
                          q(i1,i2,i3,tc)=q(i1,i2,i3,tc) +heatRelease*q(
     & i1,i2,i3,sc+1)*q(i1,i2,i3,sc)/q(i1,i2,i3,rc)
                        else
                          q(i1,i2,i3,tc)=q(i1,i2,i3,tc) +heatRelease*q(
     & i1,i2,i3,sc)
                        endif
                    end do
                    end do
                    end do
c chain branching
                  else if( reactionType.eq.branching )then
                    ! e = e + [Q*(rho*product) - R*(rho*radical)]
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                        q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+heatRelease*q(i1,
     & i2,i3,sc)+absorbedEnergy*q(i1,i2,i3,sc+1)
                    end do
                    end do
                    end do
                  else
                    write(6,*)'Error (consPrim) : reaction type not 
     & supported'
                    write(*,*)'reactionType=',reactionType
                    stop
                  end if
                  ! Species
                  do is=0,ns-1
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                        q(i1,i2,i3,sc+is)=q(i1,i2,i3,sc+is)/q(i1,i2,i3,
     & rc)
                    end do
                    end do
                    end do
                  end do
c add psi (placed here but commented out because it is not currently used)
c             ! q(all,all,all,tc)+=rho*psi(rho)
c             fact=-1.
c             if( nd.eq.2 )then
c               call addpsi(nd1a,nd1b,nd2a,nd2b,fact,
c    *                      q(nd1a,nd2a,nd3a,rc),
c    *                      q(nd1a,nd2a,nd3a,tc))
c
c             end if
c now just Euler part
                  if( 
     & conservativeGodunovMethod.eq.multiComponentVersion )then
                    ! multicomponent
                    if( multiVersion.eq.DonsVersion ) then
                      if (istiff.eq.0) then
                       ! non-stiff multi-component
                       if( fourComp.eq.1 ) then
                         ! four component case
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                            fmu = q(i1,i2,i3,sc)
                            flam = q(i1,i2,i3,sc+1)
                            c1h = flam*fcv1*fgam1+(1.e0-flam)*fcv2*
     & fgam2
                            c2h = flam*fcv3*fgam3+(1.e0-flam)*fcv4*
     & fgam4
                            c3h = flam*fcv1+(1.e0-flam)*fcv2
                            c4h = flam*fcv3+(1.e0-flam)*fcv4
                            gm1Inv = 1.e0/((fmu*c1h+(1.e0-fmu)*c2h)/(
     & fmu*c3h+(1.e0-fmu)*c4h)-1.e0)
                            q(i1,i2,i3,tc)=q(i1,i2,i3,tc)/(q(i1,i2,i3,
     & rc)*Rg*gm1Inv)
                         end do
                         end do
                         end do
                       else
                         ! two component case
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                            ! e = P*(mu/omegar+(1-mu)/omegai)
                            mu = q(i1,i2,i3,sc)
                            omegai=gami-1.d0
                            omegar=gamr-1.d0
                            if( cvi.lt.0.d0 ) then
                              gm1Inv=(mu/omegar+(1.d0-mu)/omegai)
                            else
                              gm1Inv=(mu*cvr+(1.d0-mu)*cvi)/(mu*cvr*
     & omegar+(1.d0-mu)*cvi*omegai)
                            endif
                            q(i1,i2,i3,tc)=q(i1,i2,i3,tc)/(q(i1,i2,i3,
     & rc)*Rg*gm1Inv)
                         end do
                         end do
                         end do
                       end if
                      else ! stiff multi-component version
                        ! DWS 5/5/09
                        rmin=1.e-2
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                            if (q(i1,i2,i3,rc).lt.rmin) then
                              q(i1,i2,i3,rc)=rmin
                            end if
                            if (q(i1,i2,i3,sc).lt.0.0) then
                              q(i1,i2,i3,sc)=0.0
                            else
                              if (q(i1,i2,i3,sc).gt.1.0) then
                                q(i1,i2,i3,sc)=1.0
                              end if
                            end if
                            rho=q(i1,i2,i3,rc)
                            e=q(i1,i2,i3,tc)/rho
                            mu=q(i1,i2,i3,sc)
                            call geteosm (rho,e,mu,p,dp,0,ier)
                            if (p.lt.0.0) then
                              p=0.0
                            end if
                            q(i1,i2,i3,tc)=p/(rho*Rg)
                        end do
                        end do
                        end do
                      end if
                    else ! not don's version
                      ! Jeff's multicomponent
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                          !  e = (p+gamma*pi)/(gamma-1)
                          lambda = q(i1,i2,i3,sc)
                          gam = (((gam1*cv1*(lambda))+(gam2*cv2*(1.0-(
     & lambda))))/((cv1*(lambda))+(cv2*(1.0-(lambda)))))
                          pie = (pi1*(lambda)+pi2*(1.0-(lambda)))
                          q(i1,i2,i3,tc)=(q(i1,i2,i3,tc)*(gam-1.0)-gam*
     & pie)/(q(i1,i2,i3,rc)*Rg)
                      end do
                      end do
                      end do
                    end if
                  else if( equationOfState.eq.idealGasEOS )then
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                        !  rho*e = p/(gamma-1)  -> T = (gamma-1)/R * (rho*e)/(rho)
                        q(i1,i2,i3,tc)=gammaRg*q(i1,i2,i3,tc)/q(i1,i2,
     & i3,rc)
                    end do
                    end do
                    end do
                  else if( equationOfState.eq.mieGruneisenEOS )then
                    ! Mie-Gruneisen EOS
                    mgkappa=eospar(4)
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                        !  rho kappa Cv T = rho*e - E_c --> compute T = (gamma-1)/(kappa*Rg) * (1/rho) *( rho*e - E_c )
                        vn=1./(q(i1,i2,i3,rc)*v0)
                        ! *wdh* 050108 -- changed sign of Ec
                        q(i1,i2,i3,tc)=gammaRg/(mgkappa*q(i1,i2,i3,rc))
     & *( q(i1,i2,i3,tc) + (vn-1.)**2/vn*( .5*mgp1 + (mgp2/3.)*(vn-1.)
     &  ) )
                    end do
                    end do
                    end do
                  else if( equationOfState.eq.stiffenedGasEOS )then
                    gammaStiff=eospar(1)
                  pStiff=eospar(2)
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                        !  compute T = p/rho = ((gammaStiff-1)*rho*e - gammaStiff*pStiff)/rho
                        q(i1,i2,i3,tc)=((gammaStiff-1)*q(i1,i2,i3,tc)-
     & gammaStiff*pStiff)/q(i1,i2,i3,rc)
                    end do
                    end do
                    end do
                  else if( equationOfState.eq.taitEOS )then
                    stop 2745
                  else if( equationOfState.eq.userDefinedEOS )then
                    ! Get T 
                    eosOption=1    ! get p=p(r,e)
                    eosDerivOption=0 ! no derivatives needed
                    iparEOS(1)=nd
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                        !   T = (gamma-1)/R * (rho*e)/(rho)
                        r =q(i1,i2,i3,rc)
                        e =q(i1,i2,i3,tc)/r
                        do n=0,nq-1
                          qv(n)=q(i1,i2,i3,n)
                        end do
                        call getUserDefinedEOS( r,e,p,dp, eosOption, 
     & eosDerivOption, qv,iparEOS,rparEOS,userEOSDataPointer, ier )
                        q(i1,i2,i3,tc)=p/r ! T := p/rho
                    end do
                    end do
                    end do
                  end if
                else
                  write(6,*)'Error (consPrim) : EOS type not supported'
                  stop
                end if
               end if
c handles non dudr2d and dudr3d cases (such as Jameson ???)
              else
c assume just Euler
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    ! e = p/(gamma-1)
                    q(i1,i2,i3,tc)=gammaRg*q(i1,i2,i3,tc)/q(i1,i2,i3,
     & rc)
                end do
                end do
                end do
              end if
            end if ! end pde choice
          end if ! end conservative to primitive
        else
          if( option.eq.0 )then
            ! ****** Primitive to Conservative ******
          if( pdeVariation.eq.conservativeGodunov .and. 
     & equationOfState.eq.jwlEOS )then
          ! write(55,*)'consPrim(p->c)'
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
            ! compute specific internal energy en given (rho,p,lambda), possibly update vs and vg
            if (mask(i1,i2,i3).ne.0) then
          !   write(55,*)i1,i2,(q(i1,i2,i3,i),i=0,6)
              ier=0
              r=q(i1,i2,i3,rc)
              if (r.lt.1.e-5) then
              write(6,*)'q =',(q(i1,i2,i3,i),i=0,6)
              write(6,*)'mask =',mask(i1,i2,i3)
              stop
              end if
              y=r*q(i1,i2,i3,sc)
              vs=q(i1,i2,i3,sc+1)
              vg=q(i1,i2,i3,sc+2)
              p=Rg*r*q(i1,i2,i3,tc)
              call geteos (r,e,y,vs,vg,p,dp,-1,ier)
              if (ier.ne.0) then
                write(6,*)'Error (consPrim) : call to geteos failed (p-
     & >c)'
                stop
              end if
              q(i1,i2,i3,tc)=e/r
              q(i1,i2,i3,sc)=y
              q(i1,i2,i3,sc+1)=vs
              q(i1,i2,i3,sc+2)=vg
c*wdh  else
c*wdh    q(i1,i2,i3,tc)=val(tc)
c*wdh    q(i1,i2,i3,sc)=val(sc)
c*wdh    q(i1,i2,i3,sc+1)=vs0
c*wdh    q(i1,i2,i3,sc+2)=vg0
            end if
          end do
          end do
          end do
          if( nd.eq.1 )then
            ! *** 1D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              ! E = rho*( e + .5*u*u )
              q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*( q(i1,i2,i3,tc)+.5*q(i1,
     & i2,i3,uc)**2 )
              q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
            end do
            end do
            end do
          else if( nd.eq.2 )then
            ! *** 2D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              !  E = rho*( e + .5*(u*u+v*v) )
                q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*( q(i1,i2,i3,tc)+.5*(q(
     & i1,i2,i3,uc)**2 + q(i1,i2,i3,vc)**2))
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*q(i1,i2,i3,rc)
            end do
            end do
            end do
          else
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              !  E = rho*( e + .5*(u*u+v*v+w*w) )
                q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*( q(i1,i2,i3,tc)+.5*(q(
     & i1,i2,i3,uc)**2+q(i1,i2,i3,vc)**2+q(i1,i2,i3,wc)**2))
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*q(i1,i2,i3,rc)
                q(i1,i2,i3,wc)=q(i1,i2,i3,wc)*q(i1,i2,i3,rc)
            end do
            end do
            end do
          end if
          else
          if( nd.eq.1 )then
            ! *** 1D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              ! E = ( p/(gamma-1) + .5*rho*u*u )
              q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*( Rgg*q(i1,i2,i3,tc)+.5*q(
     & i1,i2,i3,uc)**2 )
              q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
            end do
            end do
            end do
          else if( nd.eq.2 )then
            ! *** 2D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              !  E = ( p/(gamma-1) + .5*rho*u*u )
                q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*( Rgg*q(i1,i2,i3,tc)+.5*(
     & q(i1,i2,i3,uc)**2 + q(i1,i2,i3,vc)**2))
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*q(i1,i2,i3,rc)
            end do
            end do
            end do
          else
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              !  E = ( p/(gamma-1) + .5*rho*u*u )
                q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*( Rgg*q(i1,i2,i3,tc)+.5*(
     & q(i1,i2,i3,uc)**2+q(i1,i2,i3,vc)**2+q(i1,i2,i3,wc)**2))
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*q(i1,i2,i3,rc)
                q(i1,i2,i3,wc)=q(i1,i2,i3,wc)*q(i1,i2,i3,rc)
            end do
            end do
            end do
          end if
          ! Species
          do is=0,ns-1
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
                q(i1,i2,i3,sc+is)=q(i1,i2,i3,sc+is)*q(i1,i2,i3,rc)
            end do
            end do
            end do
          end do
          if( pdeVariation.eq.conservativeGodunov )then
            ! here is where psi comes in for general eos, e.g.
            ! q(all,all,all,tc)+=rho*psi(rho)
            fact=1.
            if( nd.eq.2 )then
              call addpsi(nd1a,nd1b,nd2a,nd2b,fact,q(nd1a,nd2a,nd3a,rc)
     & ,q(nd1a,nd2a,nd3a,tc))
            end if
            if( 
     & reactionType.eq.oneStep.or.reactionType.eq.oneStepPress )then
              ! E = E - Q*(rho*product)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)-heatRelease*q(i1,i2,i3,
     & sc)
              end do
              end do
              end do
            else if( reactionType.eq.branching )then
              ! E = E - [Q*(rho*product) - R*(rho*radical)]
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)-heatRelease*q(i1,i2,i3,
     & sc)-absorbedEnergy*q(i1,i2,i3,sc+1)
              end do
              end do
              end do
            else if( reactionType.eq.ignitionAndGrowth )then
              ! E = E - Q*(rho*product)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)-heatRelease*q(i1,i2,i3,
     & sc)
                q(i1,i2,i3,sc+1)=q(i1,i2,i3,sc+1)/q(i1,i2,i3,rc)
                q(i1,i2,i3,sc+2)=q(i1,i2,i3,sc+2)/q(i1,i2,i3,rc)
              end do
              end do
              end do
            end if
          end if
          end if
          else
            ! ****** Conservative to Primitive ******
          if( pdeVariation.eq.conservativeGodunov .and. 
     & equationOfState.eq.jwlEOS )then
          ! write(55,*)'consPrim(c->p)'
          if( nd.eq.1 )then
            ! *** 1D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              ! e = ( E/rho - .5*u*u )
              q(i1,i2,i3,uc)=q(i1,i2,i3,uc)/q(i1,i2,i3,rc)
              q(i1,i2,i3,tc)=q(i1,i2,i3,tc)/q(i1,i2,i3,rc)-.5*q(i1,i2,
     & i3,uc)**2
            end do
            end do
            end do
          else if( nd.eq.2 )then
            ! *** 2D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              !  e = ( E/rho - .5*(u*u+v*v) )
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)/q(i1,i2,i3,rc)
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)/q(i1,i2,i3,rc)
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)/q(i1,i2,i3,rc)-.5*(q(i1,
     & i2,i3,uc)**2 + q(i1,i2,i3,vc)**2)
            end do
            end do
            end do
          else
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              !  e = ( E/rho - .5*(u*u+v*v+w*w) )
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)/q(i1,i2,i3,rc)
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)/q(i1,i2,i3,rc)
                q(i1,i2,i3,wc)=q(i1,i2,i3,wc)/q(i1,i2,i3,rc)
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)/q(i1,i2,i3,rc)-.5*(q(i1,
     & i2,i3,uc)**2+q(i1,i2,i3,vc)**2+q(i1,i2,i3,wc)**2)
            end do
            end do
            end do
          end if
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
            ! compute pressure p given (rho,en,lambda), possibly update vs and vg
            if (mask(i1,i2,i3).ne.0) then
          !   write(55,*)i1,i2,(q(i1,i2,i3,i),i=0,6)
              ier=0
              r=q(i1,i2,i3,rc)
              e=r*q(i1,i2,i3,tc)
              y=q(i1,i2,i3,sc)
              vs=q(i1,i2,i3,sc+1)
              vg=q(i1,i2,i3,sc+2)
              call geteos (r,e,y,vs,vg,p,dp,0,ier)
          !   write(55,'(2(1x,i2),9(1x,f11.8))')i1,i2,(q(i1,i2,i3,i),i=0,6),vs,vg
              if (ier.ne.0) then
                write(6,*)'Error (consPrim) : call to geteos failed (c-
     & >p)'
                stop
              end if
              q(i1,i2,i3,tc)=p/(r*Rg)
              q(i1,i2,i3,sc)=y/r
              q(i1,i2,i3,sc+1)=vs
              q(i1,i2,i3,sc+2)=vg
c dws
c  else
c    q(i1,i2,i3,tc)=val(tc)
c    q(i1,i2,i3,sc)=val(sc)
c    q(i1,i2,i3,sc+1)=vs0
c    q(i1,i2,i3,sc+2)=vg0
            end if
          end do
          end do
          end do
          else
          if( nd.eq.1 )then
            ! *** 1D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              ! E = ( p/(gamma-1) + .5*rho*u*u )
                ! rhoi=1./max(epsRho,q(i1,i2,i3,rc))
                 if( q(i1,i2,i3,rc).lt.epsRho )then
                   imask=mask(i1,i2,i3)
                   if( imask.lt.0 )then
                     imask=-1
                   else if( imask.gt.0 )then
                     imask=1
                   end if
c    write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
                   if( mask(i1,i2,i3).eq.0 )then
                     q(i1,i2,i3,rc)=1.
                   else
                     q(i1,i2,i3,rc)=epsRho
                   end if
                 end if
                rhoi=1./q(i1,i2,i3,rc)
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*rhoi
                q(i1,i2,i3,tc)=gammaRg*( q(i1,i2,i3,tc)*rhoi - .5*(q(
     & i1,i2,i3,uc)**2) )
            end do
            end do
            end do
          else if( nd.eq.2 )then
            ! *** 2D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              !  E = ( p/(gamma-1) + .5*rho*u*u )
                ! rhoi=1./max(epsRho,q(i1,i2,i3,rc))
                 if( q(i1,i2,i3,rc).lt.epsRho )then
                   imask=mask(i1,i2,i3)
                   if( imask.lt.0 )then
                     imask=-1
                   else if( imask.gt.0 )then
                     imask=1
                   end if
c    write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
                   if( mask(i1,i2,i3).eq.0 )then
                     q(i1,i2,i3,rc)=1.
                   else
                     q(i1,i2,i3,rc)=epsRho
                   end if
                 end if
                rhoi=1./q(i1,i2,i3,rc)
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*rhoi
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*rhoi
                q(i1,i2,i3,tc)=gammaRg*( q(i1,i2,i3,tc)*rhoi - .5*(q(
     & i1,i2,i3,uc)**2 + q(i1,i2,i3,vc)**2 ) )
            end do
            end do
            end do
          else
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              !  E = ( p/(gamma-1) + .5*rho*u*u )
                ! rhoi=1./max(epsRho,q(i1,i2,i3,rc))
                 if( q(i1,i2,i3,rc).lt.epsRho )then
                   imask=mask(i1,i2,i3)
                   if( imask.lt.0 )then
                     imask=-1
                   else if( imask.gt.0 )then
                     imask=1
                   end if
c    write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
                   if( mask(i1,i2,i3).eq.0 )then
                     q(i1,i2,i3,rc)=1.
                   else
                     q(i1,i2,i3,rc)=epsRho
                   end if
                 end if
                rhoi=1./q(i1,i2,i3,rc)
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*rhoi
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*rhoi
                q(i1,i2,i3,wc)=q(i1,i2,i3,wc)*rhoi
                q(i1,i2,i3,tc)=gammaRg*( q(i1,i2,i3,tc)*rhoi - .5*(q(
     & i1,i2,i3,uc)**2 + q(i1,i2,i3,vc)**2 + q(i1,i2,i3,wc)**2 ) )
                 if( q(i1,i2,i3,tc).lt.epsRho )then
                   imask=mask(i1,i2,i3)
                   if( imask.lt.0 )then
                     imask=-1
                   else if( imask.gt.0 )then
                     imask=1
                   end if
c    write(*,'("consPrim:WARNING: i=",3i4," T=",e8.2," epsT=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,tc),epsRho,imask
                   if( mask(i1,i2,i3).eq.0 )then
                     q(i1,i2,i3,tc)=1.
                   else
                     q(i1,i2,i3,tc)=epsRho
                   end if
                 end if
            end do
            end do
            end do
          end if
          ! Species
          do is=0,ns-1
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
                q(i1,i2,i3,sc+is)=q(i1,i2,i3,sc+is)/max(epsRho,q(i1,i2,
     & i3,rc))
            end do
            end do
            end do
          end do
          if( pdeVariation.eq.conservativeGodunov )then
              ! here is where psi comes in for general eos, e.g.
              !  q(all,all,all,tc)-=((gamma-1.)/Rg)*psi(rho);
            fact=-(gamma-1.)/Rg
            if( nd.eq.2 )then
              call addpsi(nd1a,nd1b,nd2a,nd2b,fact,q(nd1a,nd2a,nd3a,rc)
     & ,q(nd1a,nd2a,nd3a,tc))
            end if
            if(  
     & reactionType.eq.oneStep.or.reactionType.eq.oneStepPress )then
              ! E = E - Q*(rho*product)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+gammaRg*heatRelease*q(i1,
     & i2,i3,sc)
              end do
              end do
              end do
            else if(  reactionType.eq.branching )then
              ! E = E - [Q*(rho*product) - R*(rho*radical)]
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+gammaRg*heatRelease*q(i1,
     & i2,i3,sc)-gammaRg*absorbedEnergy*q(i1,i2,i3,sc+1)
              end do
              end do
              end do
            else if(  reactionType.eq.ignitionAndGrowth )then
              ! E = E - Q*(rho*product)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+gammaRg*heatRelease*q(i1,
     & i2,i3,sc)
                q(i1,i2,i3,sc+1)=q(i1,i2,i3,sc+1)*q(i1,i2,i3,rc)
                q(i1,i2,i3,sc+2)=q(i1,i2,i3,sc+2)*q(i1,i2,i3,rc)
              end do
              end do
              end do
            end if
          end if
          end if
          end if
        endif

      else



c        ! **** this version does a fixup *****
c        write(*,'(" ******** PRIMCONS FIXUP *******")')
c        stop 1234

        if( useNewVersion.eq.1 )then
          if( option.eq.0 )then
            ! ***************************************
            ! ****** Primitive to Conservative ******
            ! ***************************************
            if( pde.eq.compressibleMultiphase ) then
c    write(6,*)'here i am (1)',mfsolid
c    do i=0,11
c      write(6,*)i,q(n1a,n2a,n3a,i)
c    end do
c    pause
c here is the new multiphase option
c first step: thermodynamics => convert Tk=pk/rk to ek, k=s or g
              astiny=1.e-3
              pgtiny=1.e-3
              if (mfsolid.eq.0) then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    q(i1,i2,i3,tsc)=(q(i1,i2,i3,tsc)+ps0*gamc(1)/q(i1,
     & i2,i3,rsc))/gm1(1)+compac(q(i1,i2,i3,asc),0)
                    q(i1,i2,i3,tgc)=q(i1,i2,i3,tgc)/(gm1(2)*(1.0+bgas*
     & q(i1,i2,i3,rgc)))
                    if (q(i1,i2,i3,asc).lt.astiny) then
                      if (q(i1,i2,i3,tgc).lt.pgtiny/q(i1,i2,i3,rgc)) 
     & then
                        q(i1,i2,i3,tgc)=(pgtiny/q(i1,i2,i3,rgc))/(gm1(
     & 2)*(1.0+bgas*q(i1,i2,i3,rgc)))
                      end if
                    end if
                  else
                    q(i1,i2,i3,tsc)=val(tsc)
                    q(i1,i2,i3,tgc)=val(tgc)
                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    q(i1,i2,i3,tsc)=q(i1,i2,i3,tsc)*q(i1,i2,i3,mu1c)+q(
     & i1,i2,i3,mu2c)/q(i1,i2,i3,rsc)+compac(q(i1,i2,i3,asc),0)
                    q(i1,i2,i3,tgc)=q(i1,i2,i3,tgc)/(gm1(2)*(1.0+bgas*
     & q(i1,i2,i3,rgc)))
                    if (q(i1,i2,i3,asc).lt.astiny) then
                      if (q(i1,i2,i3,tgc).lt.pgtiny/q(i1,i2,i3,rgc)) 
     & then
                        q(i1,i2,i3,tgc)=(pgtiny/q(i1,i2,i3,rgc))/(gm1(
     & 2)*(1.0+bgas*q(i1,i2,i3,rgc)))
                      end if
                    end if
                  else
                    q(i1,i2,i3,tsc)=val(tsc)
                    q(i1,i2,i3,tgc)=val(tgc)
                  end if
                end do
                end do
                end do
              end if
c    do i=0,11
c      write(6,*)i,q(n1a,n2a,n3a,i)
c    end do
c    pause
c    write(6,*)(q(0,0,0,i),i=0,8)
c    pause
c second step : kinematics => convert (ek,uk) to (rk*Ek,rk*uk), k=s or g  (2d is assumed)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  q(i1,i2,i3,tsc)=q(i1,i2,i3,rsc)*(q(i1,i2,i3,tsc)+.5*(
     & q(i1,i2,i3,usc)**2+q(i1,i2,i3,vsc)**2))
                  q(i1,i2,i3,usc)=q(i1,i2,i3,usc)*q(i1,i2,i3,rsc)
                  q(i1,i2,i3,vsc)=q(i1,i2,i3,vsc)*q(i1,i2,i3,rsc)
                  q(i1,i2,i3,tgc)=q(i1,i2,i3,rgc)*(q(i1,i2,i3,tgc)+.5*(
     & q(i1,i2,i3,ugc)**2+q(i1,i2,i3,vgc)**2))
                  q(i1,i2,i3,ugc)=q(i1,i2,i3,ugc)*q(i1,i2,i3,rgc)
                  q(i1,i2,i3,vgc)=q(i1,i2,i3,vgc)*q(i1,i2,i3,rgc)
                else
                  q(i1,i2,i3,tsc)=val(tsc)
                  q(i1,i2,i3,usc)=val(usc)
                  q(i1,i2,i3,vsc)=val(vsc)
                  q(i1,i2,i3,tgc)=val(tgc)
                  q(i1,i2,i3,ugc)=val(ugc)
                  q(i1,i2,i3,vgc)=val(vgc)
                end if
              end do
              end do
              end do
c    write(6,*)(q(0,0,0,i),i=0,8)
c    pause
c third step : multiply by volume fraction
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  alps=q(i1,i2,i3,asc)
                  alpg=1.0-alps
                  q(i1,i2,i3,rsc)=alps*q(i1,i2,i3,rsc)
                  q(i1,i2,i3,usc)=alps*q(i1,i2,i3,usc)
                  q(i1,i2,i3,vsc)=alps*q(i1,i2,i3,vsc)
                  q(i1,i2,i3,tsc)=alps*q(i1,i2,i3,tsc)
                  q(i1,i2,i3,rgc)=alpg*q(i1,i2,i3,rgc)
                  q(i1,i2,i3,ugc)=alpg*q(i1,i2,i3,ugc)
                  q(i1,i2,i3,vgc)=alpg*q(i1,i2,i3,vgc)
                  q(i1,i2,i3,tgc)=alpg*q(i1,i2,i3,tgc)
                else
                  q(i1,i2,i3,asc)=val(asc)
                  q(i1,i2,i3,rsc)=val(rsc)
                  q(i1,i2,i3,usc)=val(usc)
                  q(i1,i2,i3,vsc)=val(vsc)
                  q(i1,i2,i3,tsc)=val(tsc)
                  q(i1,i2,i3,rgc)=val(rgc)
                  q(i1,i2,i3,ugc)=val(ugc)
                  q(i1,i2,i3,vgc)=val(vgc)
                  q(i1,i2,i3,tgc)=val(tgc)
                end if
              end do
              end do
              end do
c    write(6,*)(q(0,0,0,i),i=0,8)
c    pause
c    do i=0,11
c      write(6,*)i,q(n1a,n2a,n3a,i)
c    end do
c    pause
            else
c first step: thermodynamics => convert (rho,T,lambda,mu) to (rho,e,rho*lambda,mu)
c
c             where rho=density
c                   T=temperature (perhaps just p/rho)
c                   e=internal energy (per unit volume)
c                   lambda=species fractions
c                   mu=any "steady state" variables (such as vs,vg for mixture JWL eos)
              if( pdeVariation.eq.conservativeGodunov )then
               if( conservativeGodunovMethod.eq.multiFluidVersion )then
c handles cmfdu
                    ! --------- Multi-fluid Godunov ---------
                    if( equationOfState.eq.idealGasEOS )then
                     do i3=n3a,n3b
                     do i2=n2a,n2b
                     do i1=n1a,n1b
                       if( mask(i1,i2,i3).ne.0 )then
                         ! rho*e = mu1*p = mu1*rho*T
                         q(i1,i2,i3,tc)=q(i1,i2,i3,sc)*q(i1,i2,i3,rc)*
     & q(i1,i2,i3,tc)
                       else
                         ! fixup unused points
                         q(i1,i2,i3,rc)=val(rc)
                         q(i1,i2,i3,tc)=val(tc)
                       end if
                     end do
                     end do
                     end do
                    else if( equationOfState.eq.stiffenedGasEOS )then
                     if( reactionType.eq.noReactions )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).ne.0 )then
                          ! rho*e = mu1*p + mu2 = mu1*rho*T+mu2
                          q(i1,i2,i3,tc)=q(i1,i2,i3,sc)*q(i1,i2,i3,rc)*
     & q(i1,i2,i3,tc) + q(i1,i2,i3,sc+1)
          ! might want to set q(i1,i2,i3,tc)=q(i1,i2,i3,sc)*q(i1,i2,i3,rc)*max(q(i1,i2,i3,tc),0.) + q(i1,i2,i3,sc+1)
                        else
                          ! fixup unused points
                          q(i1,i2,i3,rc)=val(rc)
                          q(i1,i2,i3,tc)=val(tc)
                        end if
                      end do
                      end do
                      end do
                     elseif( 
     & reactionType.eq.ignitionPressureReactionRate )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).ne.0 )then
                          ! rho*e = mu1*p + mu2 + rho*mu3 = rho*(mu1*T+mu3)+mu2
                          alam=q(i1,i2,i3,sc)
                          amu1=(1.0-alam)*q(i1,i2,i3,sc+1)+alam*q(i1,
     & i2,i3,sc+2)
                          amu2=(1.0-alam)*q(i1,i2,i3,sc+3)+alam*q(i1,
     & i2,i3,sc+4)
                          amu3=                            alam*q(i1,
     & i2,i3,sc+5)
                          q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*(amu1*q(i1,i2,
     & i3,tc)+amu3)+amu2
          ! might want to set q(i1,i2,i3,tc)=q(i1,i2,i3,sc)*q(i1,i2,i3,rc)*max(q(i1,i2,i3,tc),0.) + q(i1,i2,i3,sc+1)
                        else
                          ! fixup unused points
                          q(i1,i2,i3,rc)=val(rc)
                          q(i1,i2,i3,tc)=val(tc)
                        end if
                      end do
                      end do
                      end do
                     else
                       write(*,*) ' consPrim:ERROR: multifluid unknown 
     & reaction rate'
                       stop 9015
                     end if
                    else
                     write(*,*) 'consPrim:ERROR: multifluid unknown 
     & EOS'
                     stop 9016
                    end if
               else
c handles all of dudr2d and dudr3d cases
c mixture JWL eos (also handles heat release contribution to the energy)
                if( equationOfState.eq.jwlEOS )then
                  ! write(55,*)'consPrim(p->c)'
                  ! DWS 4/28/04
                  rmin=1.e-2
                  pmin=1.e-3
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                    ! compute specific internal energy en given (rho,p,lambda), possibly update vs and vg
                    if( mask(i1,i2,i3).ne.0 )then
                      !   write(55,*)i1,i2,(q(i1,i2,i3,i),i=0,6)
                      ier=0
                      r=q(i1,i2,i3,rc)
                      ! if (r.lt.1.e-5) then
                      !   write(6,*)'q =',(q(i1,i2,i3,i),i=0,6)
                      !   write(6,*)'mask =',mask(i1,i2,i3)
                      !   write(6,*)'(1) fixup =',fixup
                      !   stop
                      ! end if
                      ! DWS 4/28/04
                      if (r.lt.rmin) then
                        q(i1,i2,i3,rc)=rmin
                        q(i1,i2,i3,sc)=1.
                        q(i1,i2,i3,tc)=pmin/(Rg*rmin)
                        ! write(6,*)'Warning (consPrim) : low density fix, r=',r
                      end if
                      if( 
     & conservativeGodunovMethod.eq.multiComponentVersion )then
                        mrho=r
                        p=Rg*r*q(i1,i2,i3,tc)
                        mmu=q(i1,i2,i3,sc)
                        mlam=q(i1,i2,i3,sc+1)
c              mvi=q(i1,i2,i3,sc+2)
c              mvs=q(i1,i2,i3,sc+3)
c              mvg=q(i1,i2,i3,sc+4)
                        mvi=q(i1,i2,i3,ivi)
                        mvs=q(i1,i2,i3,ivs)
                        mvg=q(i1,i2,i3,ivg)
                        if( newMethod.eq.0 ) then
                          call geteosb( mrho,e,mmu,mlam,mvi,mvs,mvg,p,
     & dp,-1,ier )
                        else
                          call geteosc( mrho,e,mmu,mlam,mvi,mvs,mvg,p,
     & dp,-1,ier )
                        end if
                        if (ier.ne.0) then
                          write(6,*)'Error (consPrim) : call to 
     & geteosb failed'
                          stop
                        end if
                        q(i1,i2,i3,tc)=e*mrho
                        q(i1,i2,i3,sc)=mrho*mmu
                        q(i1,i2,i3,sc+1)=mrho*mlam
                        if( reactionType.eq.igDesensitization ) then
                          q(i1,i2,i3,sc+2)=mrho*q(i1,i2,i3,sc+2)
                        end if
c              q(i1,i2,i3,sc+2)=mvi
c              q(i1,i2,i3,sc+3)=mvs
c              q(i1,i2,i3,sc+4)=mvg
                        q(i1,i2,i3,ivi)=mvi
                        q(i1,i2,i3,ivs)=mvs
                        q(i1,i2,i3,ivg)=mvg
                      else
                        y=r*q(i1,i2,i3,sc)
c              vs=q(i1,i2,i3,sc+1)
c              vg=q(i1,i2,i3,sc+2)
                        vs=q(i1,i2,i3,ivs)
                        vg=q(i1,i2,i3,ivg)
                        p=Rg*r*q(i1,i2,i3,tc)
                        call geteos (r,e,y,vs,vg,p,dp,-1,ier)
                        if (ier.ne.0) then
                          write(6,*)'Error (consPrim) : call to geteos 
     & failed (p->c)'
                                          ! '
                          stop
                        end if
                        q(i1,i2,i3,tc)=e
                        q(i1,i2,i3,sc)=y
                        if( reactionType.eq.igDesensitization) then
                          q(i1,i2,i3,sc+1)=r*q(i1,i2,i3,sc+1)
                        end if
c              q(i1,i2,i3,sc+1)=vs
c              q(i1,i2,i3,sc+2)=vg
                        q(i1,i2,i3,ivs)=vs
                        q(i1,i2,i3,ivg)=vg
                      end if
                    else
                      ! fixup unused points
                      q(i1,i2,i3,rc)=val(rc)
                      q(i1,i2,i3,tc)=val(tc)
                      q(i1,i2,i3,sc)=val(sc)
                      q(i1,i2,i3,sc+1)=val(sc+1)
                      q(i1,i2,i3,sc+2)=val(sc+2)
                      if( 
     & conservativeGodunovMethod.eq.multiComponentVersion )then
                        q(i1,i2,i3,sc+3)=val(sc+3)
                        q(i1,i2,i3,sc+3)=val(sc+4)
                      end if
                    end if
                  end do
                  end do
                  end do
c now do ideal and Mie-Gruneisen eos cases
                else if( equationOfState.eq.idealGasEOS .or. 
     & equationOfState.eq.mieGruneisenEOS .or. 
     & equationOfState.eq.userDefinedEOS .or. 
     & equationOfState.eq.stiffenedGasEOS 
     & .or.equationOfState.eq.taitEOS )then
c first just Euler
                  if( 
     & conservativeGodunovMethod.eq.multiComponentVersion )then
                    ! multicomponent
                    if( multiVersion.eq.DonsVersion ) then
                      ! Use multi-component version in dudr
                      if (istiff.eq.0) then
                        ! non-stiff multi-component
                        if( fourComp.eq.1 ) then
                          ! four component
                          do i3=n3a,n3b
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                            if( mask(i1,i2,i3).ne.0 )then
                             fmu = q(i1,i2,i3,sc)
                             flam = q(i1,i2,i3,sc+1)
                             c1h = flam*fcv1*fgam1+(1.e0-flam)*fcv2*
     & fgam2
                             c2h = flam*fcv3*fgam3+(1.e0-flam)*fcv4*
     & fgam4
                             c3h = flam*fcv1+(1.e0-flam)*fcv2
                             c4h = flam*fcv3+(1.e0-flam)*fcv4
                             gm1Inv = 1.e0/((fmu*c1h+(1.e0-fmu)*c2h)/(
     & fmu*c3h+(1.e0-fmu)*c4h)-1.e0)
                             q(i1,i2,i3,tc)=(q(i1,i2,i3,rc)*Rg*q(i1,i2,
     & i3,tc))*gm1Inv
                            else
                             ! fixup unused points
                             q(i1,i2,i3,rc)=val(rc)
                             q(i1,i2,i3,tc)=val(tc)
                            end if
                          end do
                          end do
                          end do
                        else
                          ! 2 component multi-component
                          do i3=n3a,n3b
                          do i2=n2a,n2b
                          do i1=n1a,n1b
                            if( mask(i1,i2,i3).ne.0 )then
                              ! e = P*(mu/omegar+(1-mu)/omegai)
                              mu = q(i1,i2,i3,sc)
                              omegai=gami-1.d0
                              omegar=gamr-1.d0
                              if( cvi.lt.0.d0 ) then
                                gm1Inv=(mu/omegar+(1.d0-mu)/omegai)
                              else
                                gm1Inv=(mu*cvr+(1.d0-mu)*cvi)/(mu*cvr*
     & omegar+(1.d0-mu)*cvi*omegai)
                              endif
                              q(i1,i2,i3,tc)=(q(i1,i2,i3,rc)*Rg*q(i1,
     & i2,i3,tc))*gm1Inv
                             else
                              ! fixup unused points
                              q(i1,i2,i3,rc)=val(rc)
                              q(i1,i2,i3,tc)=val(tc)
                             end if
                           end do
                           end do
                           end do
                        end if
                      else
                       ! stiffened multi-component
                        ! DWS 5/5/09
                        rmin=1.e-2
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                          if( mask(i1,i2,i3).ne.0 )then
                            if (q(i1,i2,i3,rc).lt.rmin) then
                              q(i1,i2,i3,rc)=rmin
                            end if
                            if (q(i1,i2,i3,tc).lt.0.0) then
                              q(i1,i2,i3,tc)=0.0
                            end if
                            if (q(i1,i2,i3,sc).lt.0.0) then
                              q(i1,i2,i3,sc)=0.0
                            else
                              if (q(i1,i2,i3,sc).gt.1.0) then
                                q(i1,i2,i3,sc)=1.0
                              end if
                            end if
                            rho=q(i1,i2,i3,rc)
                            p=Rg*q(i1,i2,i3,rc)*q(i1,i2,i3,tc)
                            mu=q(i1,i2,i3,sc)
                            call geteosm (rho,e,mu,p,dp,-1,ier)
                            q(i1,i2,i3,tc)=rho*e
                          else
                            ! fixup unused points
                            q(i1,i2,i3,rc)=val(rc)
                            q(i1,i2,i3,tc)=val(tc)
                          end if
                        end do
                        end do
                        end do
                      end if  ! end stiffened multi-component
                    else !  multiVersion.ne.DonsVersion
                      ! Jeff's multi-component version
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).ne.0 )then
                          ! e = (p+gamma*pi)/(gamma-1)
                          lambda = q(i1,i2,i3,sc)
                          gam = (((gam1*cv1*(lambda))+(gam2*cv2*(1.0-(
     & lambda))))/((cv1*(lambda))+(cv2*(1.0-(lambda)))))
                          pie = (pi1*(lambda)+pi2*(1.0-(lambda)))
                          q(i1,i2,i3,tc)=(Rg*q(i1,i2,i3,rc)*q(i1,i2,i3,
     & tc)+gam*pie)/((gam-1.0))
                        else
                          ! fixup unused points
                          q(i1,i2,i3,rc)=val(rc)
                          q(i1,i2,i3,tc)=val(tc)
                        end if
                      end do
                      end do
                      end do
                    end if
                  else if( equationOfState.eq.idealGasEOS )then
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1,i2,i3).ne.0 )then
                        ! rho*e = p/(gamma-1) = rho*Rg*T/(gamma-1)
                        q(i1,i2,i3,tc)=Rgg*q(i1,i2,i3,rc)*q(i1,i2,i3,
     & tc)
                      else
                        ! fixup unused points
                        q(i1,i2,i3,rc)=val(rc)
                        q(i1,i2,i3,tc)=val(tc)
                      end if
                    end do
                    end do
                    end do
                  else if( equationOfState.eq.mieGruneisenEOS )then
                    ! Mie Gruneisen EOS
                    mgkappa=eospar(4)  !  Cp = Cv + kappa*R
                    ! write(*,'(" consprim: eosPar(4)=kappa=",e10.3)') eospar(4)
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1,i2,i3).ne.0 )then
                        ! rho*e = rho*kappa*R*T/(gamma-1) + E_c
                        !       = rho*Cv*T + E_c/rho  with Cv = kappa* Rg/(gamma-1)
                        vn=1./(q(i1,i2,i3,rc)*v0)
                        ! *wdh* 050108 -- changed sign of Ec
                        q(i1,i2,i3,tc)=Rgg*mgkappa*q(i1,i2,i3,rc)*q(i1,
     & i2,i3,tc) - ((vn-1.)**2)*( .5*mgp1 + (mgp2/3.)*(vn-1.) )/vn
                      else
                        ! fixup unused points
                        q(i1,i2,i3,rc)=val(rc)
                        q(i1,i2,i3,tc)=val(tc)
                      end if
                    end do
                    end do
                    end do
                  else if( equationOfState.eq.stiffenedGasEOS )then
                    ! Stiffened EOS *ve* 071030 
                    gammaStiff=eosPar(1)
                    pStiff=eosPar(2)
                    ! write(*,'(" consprim: stiffened gamma,p0=",2e10.3)') gammaStiff,pStiff
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1,i2,i3).ne.0 )then
                        ! *ve* rho*e = (rho*T-gammaStiff*pStiff)/(gammaStiff-1)
                        ! *ve* rho*e = (rho*T + gammaStiff*pStiff)/(gammaStiff-1)  *wdh* 
                         q(i1,i2,i3,tc)=(q(i1,i2,i3,rc)*q(i1,i2,i3,tc)+
     & (gammaStiff*pStiff))/(gammaStiff-1.)
                      else
                        ! fixup unused points
                        q(i1,i2,i3,rc)=val(rc)
                        q(i1,i2,i3,tc)=val(tc)
                      end if
                    end do
                    end do
                    end do
                  else if( equationOfState.eq.taitEOS )then
                    stop 2745
                  else if( equationOfState.eq.userDefinedEOS )then
                    eosOption=0      ! get e=e(r,e)
                    eosDerivOption=0 ! no derivatives needed
                    iparEOS(1)=nd
                    ier = 0
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1,i2,i3).ne.0 )then
                        ! first get e from rho and p
                        r =q(i1,i2,i3,rc)
                        p = r*q(i1,i2,i3,tc)
                        do n=0,nq-1
                          qv(n)=q(i1,i2,i3,n)
                        end do
                        call getUserDefinedEOS( r,e,p,dp, eosOption, 
     & eosDerivOption, qv,iparEOS,rparEOS,userEOSDataPointer, ier )
                        q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*e
                      else
                        ! fixup unused points
                        q(i1,i2,i3,rc)=val(rc)
                        q(i1,i2,i3,tc)=val(tc)
                      end if
                    end do
                    end do
                    end do
                 else
                   write(*,'("consPrim: ERROR - Unknown EOS, 
     & equationOfState=",i6)') equationOfState
                    ! '
                   stop 1432
                 end if
c add psi (placed here but commented out because it is not currently used)
c             ! q(all,all,all,tc)+=rho*psi(rho)
c             fact=1.
c             if( nd.eq.2 )then
c               call addpsi(nd1a,nd1b,nd2a,nd2b,fact,
c    *                      q(nd1a,nd2a,nd3a,rc),
c    *                      q(nd1a,nd2a,nd3a,tc))
c
c             end if
c now do reacting cases
                  ! Species
                  do is=0,ns-1
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                    if( mask(i1,i2,i3).ne.0 )then
                      q(i1,i2,i3,sc+is)=q(i1,i2,i3,sc+is)*q(i1,i2,i3,
     & rc)
                    else
                      ! fixup unused points
                      q(i1,i2,i3,sc+is)=val(sc+is)
                    end if
                  end do
                  end do
                  end do
                  end do
                  if( reactionType.eq.noReactions.or.fourComp.eq.1 )
     & then
                    ! do nothing
                  else if( 
     & reactionType.eq.oneStep.or.reactionType.eq.oneStepPress )then
                    !  ***** one step *****
                    ! e = e - Q*(rho*product)
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1,i2,i3).ne.0 )then
                        if( 
     & conservativeGodunovMethod.eq.multiComponentVersion ) then
                          q(i1,i2,i3,tc)=q(i1,i2,i3,tc) -heatRelease*q(
     & i1,i2,i3,sc+1)*q(i1,i2,i3,sc)/q(i1,i2,i3,rc)
                        else
                          q(i1,i2,i3,tc)=q(i1,i2,i3,tc) -heatRelease*q(
     & i1,i2,i3,sc)
                        endif
                      else
                        ! fixup unused points
                        q(i1,i2,i3,tc)=val(tc)
                      end if
                    end do
                    end do
                    end do
                  else if( reactionType.eq.branching )then
                    ! **** chain branching *****
                    ! e = e - [Q*(rho*product) - R*(rho*radical)]
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1,i2,i3).ne.0 )then
                        q(i1,i2,i3,tc)=q(i1,i2,i3,tc)-heatRelease*q(i1,
     & i2,i3,sc)-absorbedEnergy*q(i1,i2,i3,sc+1)
                      else
                        ! fixup unused points
                        q(i1,i2,i3,tc)=val(tc)
                      end if
                    end do
                    end do
                    end do
                  else
                    write(6,*)'Error (consPrim) : reaction type not 
     & supported'
                    write(*,*)'reactionType=',reactionType
                    stop
                  end if
                else
                  write(6,*)'Error (consPrim) : EOS type not supported'
                  stop
                end if
               end if
              else
c     handles non dudr2d and dudr3d cases (such as Jameson ???)
c    assume just Euler
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    ! e = p/(gamma-1)
                    q(i1,i2,i3,tc)=Rgg*q(i1,i2,i3,rc)*q(i1,i2,i3,tc)
                  else
                    ! fixup unused points
                    q(i1,i2,i3,tc)=val(tc)
                  end if
                end do
                end do
                end do
              end if
c second step: kinetics => convert (e,u) to (E,rho*u)
c
c              where u=velocity
c                    E=total energy (per unit volume)
              if( nd.eq.1 )then
                ! *** 1D ***
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  ! E = e + .5*rho*u*u
                  if( mask(i1,i2,i3).ne.0 )then
                    q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+.5*q(i1,i2,i3,rc)*q(
     & i1,i2,i3,uc)**2
                    q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                  else
                    ! fixup unused points
                    q(i1,i2,i3,tc)=val(tc)
                    q(i1,i2,i3,uc)=val(uc)
                  end if
                end do
                end do
                end do
              else if( nd.eq.2 )then
                ! *** 2D ***
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    !  E = e + .5*rho*(u*u+v*v)
                    q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+.5*q(i1,i2,i3,rc)*(q(
     & i1,i2,i3,uc)**2+q(i1,i2,i3,vc)**2)
                    q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                    q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*q(i1,i2,i3,rc)
                  else
                    ! fixup unused points
                    q(i1,i2,i3,tc)=val(tc)
                    q(i1,i2,i3,uc)=val(uc)
                    q(i1,i2,i3,vc)=val(vc)
                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    !  E = e + .5*rho*(u*u+v*v+w*w)
                    q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+.5*q(i1,i2,i3,rc)*(q(
     & i1,i2,i3,uc)**2+q(i1,i2,i3,vc)**2+q(i1,i2,i3,wc)**2)
                    q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                    q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*q(i1,i2,i3,rc)
                    q(i1,i2,i3,wc)=q(i1,i2,i3,wc)*q(i1,i2,i3,rc)
                  else
                    ! fixup unused points
                    q(i1,i2,i3,tc)=val(tc)
                    q(i1,i2,i3,uc)=val(uc)
                    q(i1,i2,i3,vc)=val(vc)
                    q(i1,i2,i3,wc)=val(wc)
                  end if
                end do
                end do
                end do
              end if
            end if  ! end pde choice
          else
           ! ***************************************
           ! ****** Conservative to Primitive ******
           ! ***************************************
            if( pde.eq.compressibleMultiphase ) then
c here is the new multiphase option
c first step : divide by volume fraction
c    write(6,*)'here i am (2)',mfsolid
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  ralps=1.0/max(q(i1,i2,i3,asc),asmin)
                  ralpg=ralps/(ralps-1.0)
                  q(i1,i2,i3,rsc)=ralps*q(i1,i2,i3,rsc)
                  q(i1,i2,i3,usc)=ralps*q(i1,i2,i3,usc)
                  q(i1,i2,i3,vsc)=ralps*q(i1,i2,i3,vsc)
                  q(i1,i2,i3,tsc)=ralps*q(i1,i2,i3,tsc)
                  q(i1,i2,i3,rgc)=ralpg*q(i1,i2,i3,rgc)
                  q(i1,i2,i3,ugc)=ralpg*q(i1,i2,i3,ugc)
                  q(i1,i2,i3,vgc)=ralpg*q(i1,i2,i3,vgc)
                  q(i1,i2,i3,tgc)=ralpg*q(i1,i2,i3,tgc)
                else
                  q(i1,i2,i3,asc)=val(asc)
                  q(i1,i2,i3,rsc)=val(rsc)
                  q(i1,i2,i3,usc)=val(usc)
                  q(i1,i2,i3,vsc)=val(vsc)
                  q(i1,i2,i3,tsc)=val(tsc)
                  q(i1,i2,i3,rgc)=val(rgc)
                  q(i1,i2,i3,ugc)=val(ugc)
                  q(i1,i2,i3,vgc)=val(vgc)
                  q(i1,i2,i3,tgc)=val(tgc)
                end if
              end do
              end do
              end do
c second step : kinematics => convert (Ek,rk*uk) to (ek,uk), k=s or g  (2d is assumed)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  rsi=1.0/q(i1,i2,i3,rsc)
                  q(i1,i2,i3,usc)=q(i1,i2,i3,usc)*rsi
                  q(i1,i2,i3,vsc)=q(i1,i2,i3,vsc)*rsi
                  q(i1,i2,i3,tsc)=q(i1,i2,i3,tsc)*rsi-.5*(q(i1,i2,i3,
     & usc)**2+q(i1,i2,i3,vsc)**2)
                  rgi=1.0/q(i1,i2,i3,rgc)
                  q(i1,i2,i3,ugc)=q(i1,i2,i3,ugc)*rgi
                  q(i1,i2,i3,vgc)=q(i1,i2,i3,vgc)*rgi
                  q(i1,i2,i3,tgc)=q(i1,i2,i3,tgc)*rgi-.5*(q(i1,i2,i3,
     & ugc)**2+q(i1,i2,i3,vgc)**2)
                else
                  q(i1,i2,i3,tsc)=val(tsc)
                  q(i1,i2,i3,usc)=val(usc)
                  q(i1,i2,i3,vsc)=val(vsc)
                  q(i1,i2,i3,tgc)=val(tgc)
                  q(i1,i2,i3,ugc)=val(ugc)
                  q(i1,i2,i3,vgc)=val(vgc)
                end if
              end do
              end do
              end do
c third step: thermodynamics => convert ek to Tk=pk/rk, k=s or g
              astiny=1.e-3
              pgtiny=1.e-3
              if (mfsolid.eq.0) then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    rsi=1.0/q(i1,i2,i3,rsc)
                    q(i1,i2,i3,tsc)=gm1(1)*(q(i1,i2,i3,tsc)-compac(q(
     & i1,i2,i3,asc),0))-ps0*gamc(1)*rsi
                    q(i1,i2,i3,tgc)=q(i1,i2,i3,tgc)*gm1(2)*(1.0+bgas*q(
     & i1,i2,i3,rgc))
                    if (q(i1,i2,i3,asc).lt.astiny) then
                      if (q(i1,i2,i3,tgc).lt.pgtiny/q(i1,i2,i3,rgc)) 
     & then
                        q(i1,i2,i3,tgc)=pgtiny/q(i1,i2,i3,rgc)
                      end if
                    end if
                  else
                    q(i1,i2,i3,tsc)=val(tsc)
                    q(i1,i2,i3,tgc)=val(tgc)
                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    rsi=1.0/q(i1,i2,i3,rsc)
                    q(i1,i2,i3,tsc)=(q(i1,i2,i3,tsc)-q(i1,i2,i3,mu2c)*
     & rsi-compac(q(i1,i2,i3,asc),0))/q(i1,i2,i3,mu1c)
                    q(i1,i2,i3,tgc)=q(i1,i2,i3,tgc)*gm1(2)*(1.0+bgas*q(
     & i1,i2,i3,rgc))
                    if (q(i1,i2,i3,asc).lt.astiny) then
                      if (q(i1,i2,i3,tgc).lt.pgtiny/q(i1,i2,i3,rgc)) 
     & then
                        q(i1,i2,i3,tgc)=pgtiny/q(i1,i2,i3,rgc)
                      end if
                    end if
                  else
                    q(i1,i2,i3,tsc)=val(tsc)
                    q(i1,i2,i3,tgc)=val(tgc)
                  end if
                end do
                end do
                end do
              end if
            else
c first step: kinetics => convert (E,rho*u) to (e,u)
cc   check the density
c    do i3=n3a,n3b
c    do i2=n2a,n2b
c    do i1=n1a,n1b
c      if( q(i1,i2,i3,rc).lt.epsRho )then
c        ! imask=mask(i1,i2,i3)
c        ! if( imask.lt.0 )then
c        !   imask=-1
c        ! else if( imask.gt.0 )then
c        !   imask=1
c        ! end if
c        ! write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
c        if( mask(i1,i2,i3).eq.0 )then
c          q(i1,i2,i3,rc)=1.
c        else
c          q(i1,i2,i3,rc)=epsRho
c        end if
c      end if
c    end do
c    end do
c    end do
              if( nd.eq.1 )then
                ! *** 1D ***
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                     if( q(i1,i2,i3,rc).lt.epsRho )then
                       imask=mask(i1,i2,i3)
                       if( imask.lt.0 )then
                         imask=-1
                       else if( imask.gt.0 )then
                         imask=1
                       end if
c    write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
                       if( mask(i1,i2,i3).eq.0 )then
                         q(i1,i2,i3,rc)=1.
                       else
                         q(i1,i2,i3,rc)=epsRho
                       end if
                     end if
                    rhoi=1./q(i1,i2,i3,rc)
                    ! e = E - .5*rho*u*u
                    q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*rhoi
                    q(i1,i2,i3,tc)=q(i1,i2,i3,tc) -.5*q(i1,i2,i3,rc)*q(
     & i1,i2,i3,uc)**2
                  else
                    ! fixup unused points
                    q(i1,i2,i3,tc)=val(tc)
                    q(i1,i2,i3,uc)=val(uc)
                  end if
                end do
                end do
                end do
              else if( nd.eq.2 )then
                ! *** 2D ***
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    !  e = E - .5*rho*(u*u+v*v)
                     if( q(i1,i2,i3,rc).lt.epsRho )then
                       imask=mask(i1,i2,i3)
                       if( imask.lt.0 )then
                         imask=-1
                       else if( imask.gt.0 )then
                         imask=1
                       end if
c    write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
                       if( mask(i1,i2,i3).eq.0 )then
                         q(i1,i2,i3,rc)=1.
                       else
                         q(i1,i2,i3,rc)=epsRho
                       end if
                     end if
                    rhoi=1./q(i1,i2,i3,rc)
                    q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*rhoi
                    q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*rhoi
                    q(i1,i2,i3,tc)=q(i1,i2,i3,tc)-.5*q(i1,i2,i3,rc)*(q(
     & i1,i2,i3,uc)**2 +q(i1,i2,i3,vc)**2)
                  else
                    ! fixup unused points
                    q(i1,i2,i3,tc)=val(tc)
                    q(i1,i2,i3,uc)=val(uc)
                    q(i1,i2,i3,vc)=val(vc)
                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    !  e = E - .5*rho*(u*u+v*v+w*w)
                     if( q(i1,i2,i3,rc).lt.epsRho )then
                       imask=mask(i1,i2,i3)
                       if( imask.lt.0 )then
                         imask=-1
                       else if( imask.gt.0 )then
                         imask=1
                       end if
c    write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
                       if( mask(i1,i2,i3).eq.0 )then
                         q(i1,i2,i3,rc)=1.
                       else
                         q(i1,i2,i3,rc)=epsRho
                       end if
                     end if
                    rhoi=1./q(i1,i2,i3,rc)
                    q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*rhoi
                    q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*rhoi
                    q(i1,i2,i3,wc)=q(i1,i2,i3,wc)*rhoi
                    q(i1,i2,i3,tc)=q(i1,i2,i3,tc)-.5*q(i1,i2,i3,rc)*(q(
     & i1,i2,i3,uc)**2+q(i1,i2,i3,vc)**2+q(i1,i2,i3,wc)**2)
                  else
                    ! fixup unused points
                    q(i1,i2,i3,tc)=val(tc)
                    q(i1,i2,i3,uc)=val(uc)
                    q(i1,i2,i3,vc)=val(vc)
                    q(i1,i2,i3,wc)=val(wc)
                  end if
                end do
                end do
                end do
              end if
c second step: thermodynamics => convert (rho,e,rho*lambda,mu) to (rho,T,lambda,mu)
              if( pdeVariation.eq.conservativeGodunov )then
               if( conservativeGodunovMethod.eq.multiFluidVersion )then
c handles cmfdu
                    ! --------- Multi-fluid Godunov ---------
                    if( equationOfState.eq.idealGasEOS )then
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).ne.0 )then
                          !  T = (rho*e)/(rho*mu1) 
                          q(i1,i2,i3,tc)=q(i1,i2,i3,tc)/(q(i1,i2,i3,rc)
     & *q(i1,i2,i3,sc))
                          else
                            ! fixup unused points
                            q(i1,i2,i3,tc)=val(tc)
                          end if
                      end do
                      end do
                      end do
                    else if( equationOfState.eq.stiffenedGasEOS )then
                     if( reactionType.eq.noReactions )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).ne.0 )then
                           !  T = (rho*e - mu2)/(rho*mu1) 
                           q(i1,i2,i3,tc)=(q(i1,i2,i3,tc)-q(i1,i2,i3,
     & sc+1))/(q(i1,i2,i3,rc)*q(i1,i2,i3,sc))
          ! might want to set q(i1,i2,i3,tc) = max(q(i1,i2,i3,tc),0.)
                           else
                             ! fixup unused points
                             q(i1,i2,i3,tc)=val(tc)
                           end if
                       end do
                       end do
                       end do
                     elseif( 
     & reactionType.eq.ignitionPressureReactionRate )then
                       do i3=n3a,n3b
                       do i2=n2a,n2b
                       do i1=n1a,n1b
                         if( mask(i1,i2,i3).ne.0 )then
                           !  T = (rho*e - mu2 - rho*mu3)/(rho*mu1) 
                           alam=q(i1,i2,i3,sc)
                           amu1=(1.0-alam)*q(i1,i2,i3,sc+1)+alam*q(i1,
     & i2,i3,sc+2)
                           amu2=(1.0-alam)*q(i1,i2,i3,sc+3)+alam*q(i1,
     & i2,i3,sc+4)
                           amu3=                            alam*q(i1,
     & i2,i3,sc+5)
                           q(i1,i2,i3,tc)=(q(i1,i2,i3,tc)-amu2-q(i1,i2,
     & i3,rc)*amu3)/(q(i1,i2,i3,rc)*amu1)
          ! might want to set q(i1,i2,i3,tc) = max(q(i1,i2,i3,tc),0.)
                           else
                             ! fixup unused points
                             q(i1,i2,i3,tc)=val(tc)
                           end if
                       end do
                       end do
                       end do
                     else
                       write(*,*)'consPrim:ERROR: multifluid unknown 
     & reaction rate'
                       stop 9015
                     end if
                    else
                     write(*,*) 'consPrim:ERROR: multifluid unknown 
     & EOS'
                     stop 9016
                    end if
               else
c handles all of dudr2d and dudr3d cases
c mixture JWL eos (also handles heat release contribution to the energy)
                if( equationOfState.eq.jwlEOS )then
                  do i3=n3a,n3b
                  do i2=n2a,n2b
                  do i1=n1a,n1b
                    ! compute pressure p given (rho,en,lambda), possibly update vs and vg
                    if( mask(i1,i2,i3).ne.0 )then
                !     write(55,*)i1,i2,(q(i1,i2,i3,i),i=0,6)
                      ier=0
                      if( 
     & conservativeGodunovMethod.eq.multiComponentVersion )then
                        mrho=q(i1,i2,i3,rc)
                        e=q(i1,i2,i3,tc)/mrho
                        mmu=q(i1,i2,i3,sc)/mrho
                        mlam=q(i1,i2,i3,sc+1)/mrho
c              mvi=q(i1,i2,i3,sc+2)
c              mvs=q(i1,i2,i3,sc+3)
c              mvg=q(i1,i2,i3,sc+4)
                        mvi=q(i1,i2,i3,ivi)
                        mvs=q(i1,i2,i3,ivs)
                        mvg=q(i1,i2,i3,ivg)
                        if( newMethod.eq.0 ) then
                          call geteosb( mrho,e,mmu,mlam,mvi,mvs,mvg,p,
     & dp,0,ier )
                        else
                          call geteosc( mrho,e,mmu,mlam,mvi,mvs,mvg,p,
     & dp,0,ier )
                        end if
                        if (ier.ne.0) then
                          write(6,*)'Error (consPrim) : call to 
     & geteosb failed'
                          stop
                        end if
                        q(i1,i2,i3,tc)=p/(mrho*Rg)
                        q(i1,i2,i3,sc)=mmu
                        q(i1,i2,i3,sc+1)=mlam
                        if( reactionType.eq.igDesensitization ) then
                          q(i1,i2,i3,sc+2)=q(i1,i2,i3,sc+2)/mrho
                        end if
c              q(i1,i2,i3,sc+2)=mvi
c              q(i1,i2,i3,sc+3)=mvs
c              q(i1,i2,i3,sc+4)=mvg
                        q(i1,i2,i3,ivi)=mvi
                        q(i1,i2,i3,ivs)=mvs
                        q(i1,i2,i3,ivg)=mvg
                      else
                        r=q(i1,i2,i3,rc)
                        e=q(i1,i2,i3,tc)
                        y=q(i1,i2,i3,sc)
c              vs=q(i1,i2,i3,sc+1)
c              vg=q(i1,i2,i3,sc+2)
                        vs=q(i1,i2,i3,ivs)
                        vg=q(i1,i2,i3,ivg)
                        call geteos (r,e,y,vs,vg,p,dp,0,ier)
          !     write(55,'(2(1x,i2),9(1x,f11.8))')i1,i2,(q(i1,i2,i3,i),i=0,6),vs,vg
                        if (ier.ne.0) then
                          write(6,*)'Error (consPrim) : call to geteos 
     & failed (c->p)'
                                          ! '
                          stop
                        end if
                        q(i1,i2,i3,tc)=p/(r*Rg)
                        q(i1,i2,i3,sc)=y/r
                        if( reactionType.eq.igDesensitization ) then
                          q(i1,i2,i3,sc+1)=q(i1,i2,i3,sc+1)/r
                        end if
c              q(i1,i2,i3,sc+1)=vs
c              q(i1,i2,i3,sc+2)=vg
                        q(i1,i2,i3,ivs)=vs
                        q(i1,i2,i3,ivg)=vg
                      end if
                    else
                      ! fixup unused points
                      q(i1,i2,i3,tc)=val(tc)
                      q(i1,i2,i3,sc)=val(sc)
                      q(i1,i2,i3,sc+1)=val(sc+1)
                      q(i1,i2,i3,sc+2)=val(sc+2)
                      if( 
     & conservativeGodunovMethod.eq.multiComponentVersion )then
                        q(i1,i2,i3,sc+3)=val(sc+3)
                        q(i1,i2,i3,sc+3)=val(sc+4)
                      end if
                    end if
                  end do
                  end do
                  end do
c now do ideal eos cases
                else if( equationOfState.eq.idealGasEOS .or. 
     & equationOfState.eq.mieGruneisenEOS .or. 
     & equationOfState.eq.userDefinedEOS .or. 
     & equationOfState.eq.stiffenedGasEOS 
     & .or.equationOfState.eq.taitEOS )then
                  if( reactionType.eq.noReactions.or.fourComp.eq.1 )
     & then
                    ! do nothing
                  else if( 
     & reactionType.eq.oneStep.or.reactionType.eq.oneStepPress )then
                    ! **** reacting cases: one step *****
                    ! e = e + Q*(rho*product)
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1,i2,i3).ne.0 )then
                        if( 
     & conservativeGodunovMethod.eq.multiComponentVersion ) then
                          q(i1,i2,i3,tc)=q(i1,i2,i3,tc) +heatRelease*q(
     & i1,i2,i3,sc+1)*q(i1,i2,i3,sc)/q(i1,i2,i3,rc)
                        else
                          q(i1,i2,i3,tc)=q(i1,i2,i3,tc) +heatRelease*q(
     & i1,i2,i3,sc)
                        endif
                      else
                        ! fixup unused points
                        q(i1,i2,i3,tc)=val(tc)
                      end if
                    end do
                    end do
                    end do
c chain branching
                  else if( reactionType.eq.branching )then
                    ! e = e + [Q*(rho*product) - R*(rho*radical)]
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1,i2,i3).ne.0 )then
                        q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+heatRelease*q(i1,
     & i2,i3,sc)+absorbedEnergy*q(i1,i2,i3,sc+1)
                      else
                        ! fixup unused points
                        q(i1,i2,i3,tc)=val(tc)
                      end if
                    end do
                    end do
                    end do
                  else
                    write(6,*)'Error (consPrim) : reaction type not 
     & supported'
                    write(*,*)'reactionType=',reactionType
                    stop
                  end if
                  ! Species
                  do is=0,ns-1
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1,i2,i3).ne.0 )then
                        q(i1,i2,i3,sc+is)=q(i1,i2,i3,sc+is)/q(i1,i2,i3,
     & rc)
                      else
                        ! fixup unused points
                        q(i1,i2,i3,sc+is)=val(sc+is)
                      end if
                    end do
                    end do
                    end do
                  end do
c add psi (placed here but commented out because it is not currently used)
c             ! q(all,all,all,tc)+=rho*psi(rho)
c             fact=-1.
c             if( nd.eq.2 )then
c               call addpsi(nd1a,nd1b,nd2a,nd2b,fact,
c    *                      q(nd1a,nd2a,nd3a,rc),
c    *                      q(nd1a,nd2a,nd3a,tc))
c
c             end if
c now just Euler part
                  if( 
     & conservativeGodunovMethod.eq.multiComponentVersion )then
                    ! multicomponent
                    if( multiVersion.eq.DonsVersion ) then
                      if (istiff.eq.0) then
                       ! non-stiff multi-component
                       if( fourComp.eq.1 ) then
                         ! four component case
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                          if( mask(i1,i2,i3).ne.0 )then
                            fmu = q(i1,i2,i3,sc)
                            flam = q(i1,i2,i3,sc+1)
                            c1h = flam*fcv1*fgam1+(1.e0-flam)*fcv2*
     & fgam2
                            c2h = flam*fcv3*fgam3+(1.e0-flam)*fcv4*
     & fgam4
                            c3h = flam*fcv1+(1.e0-flam)*fcv2
                            c4h = flam*fcv3+(1.e0-flam)*fcv4
                            gm1Inv = 1.e0/((fmu*c1h+(1.e0-fmu)*c2h)/(
     & fmu*c3h+(1.e0-fmu)*c4h)-1.e0)
                            q(i1,i2,i3,tc)=q(i1,i2,i3,tc)/(q(i1,i2,i3,
     & rc)*Rg*gm1Inv)
                            else
                             ! fixup unused points
                             q(i1,i2,i3,tc)=val(tc)
                            end if
                         end do
                         end do
                         end do
                       else
                         ! two component case
                         do i3=n3a,n3b
                         do i2=n2a,n2b
                         do i1=n1a,n1b
                          if( mask(i1,i2,i3).ne.0 )then
                            ! e = P*(mu/omegar+(1-mu)/omegai)
                            mu = q(i1,i2,i3,sc)
                            omegai=gami-1.d0
                            omegar=gamr-1.d0
                            if( cvi.lt.0.d0 ) then
                              gm1Inv=(mu/omegar+(1.d0-mu)/omegai)
                            else
                              gm1Inv=(mu*cvr+(1.d0-mu)*cvi)/(mu*cvr*
     & omegar+(1.d0-mu)*cvi*omegai)
                            endif
                            q(i1,i2,i3,tc)=q(i1,i2,i3,tc)/(q(i1,i2,i3,
     & rc)*Rg*gm1Inv)
                            else
                             ! fixup unused points
                              q(i1,i2,i3,tc)=val(tc)
                            end if
                         end do
                         end do
                         end do
                       end if
                      else ! stiff multi-component version
                        ! DWS 5/5/09
                        rmin=1.e-2
                        do i3=n3a,n3b
                        do i2=n2a,n2b
                        do i1=n1a,n1b
                          if( mask(i1,i2,i3).ne.0 )then
                            if (q(i1,i2,i3,rc).lt.rmin) then
                              q(i1,i2,i3,rc)=rmin
                            end if
                            if (q(i1,i2,i3,sc).lt.0.0) then
                              q(i1,i2,i3,sc)=0.0
                            else
                              if (q(i1,i2,i3,sc).gt.1.0) then
                                q(i1,i2,i3,sc)=1.0
                              end if
                            end if
                            rho=q(i1,i2,i3,rc)
                            e=q(i1,i2,i3,tc)/rho
                            mu=q(i1,i2,i3,sc)
                            call geteosm (rho,e,mu,p,dp,0,ier)
                            if (p.lt.0.0) then
                              p=0.0
                            end if
                            q(i1,i2,i3,tc)=p/(rho*Rg)
                          else
                            ! fixup unused points
                            q(i1,i2,i3,tc)=val(tc)
                          end if
                        end do
                        end do
                        end do
                      end if
                    else ! not don's version
                      ! Jeff's multicomponent
                      do i3=n3a,n3b
                      do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1,i2,i3).ne.0 )then
                          !  e = (p+gamma*pi)/(gamma-1)
                          lambda = q(i1,i2,i3,sc)
                          gam = (((gam1*cv1*(lambda))+(gam2*cv2*(1.0-(
     & lambda))))/((cv1*(lambda))+(cv2*(1.0-(lambda)))))
                          pie = (pi1*(lambda)+pi2*(1.0-(lambda)))
                          q(i1,i2,i3,tc)=(q(i1,i2,i3,tc)*(gam-1.0)-gam*
     & pie)/(q(i1,i2,i3,rc)*Rg)
                          else
                            ! fixup unused points
                            q(i1,i2,i3,tc)=val(tc)
                          end if
                      end do
                      end do
                      end do
                    end if
                  else if( equationOfState.eq.idealGasEOS )then
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1,i2,i3).ne.0 )then
                        !  rho*e = p/(gamma-1)  -> T = (gamma-1)/R * (rho*e)/(rho)
                        q(i1,i2,i3,tc)=gammaRg*q(i1,i2,i3,tc)/q(i1,i2,
     & i3,rc)
                      else
                        ! fixup unused points
                        q(i1,i2,i3,tc)=val(tc)
                      end if
                    end do
                    end do
                    end do
                  else if( equationOfState.eq.mieGruneisenEOS )then
                    ! Mie-Gruneisen EOS
                    mgkappa=eospar(4)
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1,i2,i3).ne.0 )then
                        !  rho kappa Cv T = rho*e - E_c --> compute T = (gamma-1)/(kappa*Rg) * (1/rho) *( rho*e - E_c )
                        vn=1./(q(i1,i2,i3,rc)*v0)
                        ! *wdh* 050108 -- changed sign of Ec
                        q(i1,i2,i3,tc)=gammaRg/(mgkappa*q(i1,i2,i3,rc))
     & *( q(i1,i2,i3,tc) + (vn-1.)**2/vn*( .5*mgp1 + (mgp2/3.)*(vn-1.)
     &  ) )
                      else
                        ! fixup unused points
                        q(i1,i2,i3,tc)=val(tc)
                      end if
                    end do
                    end do
                    end do
                  else if( equationOfState.eq.stiffenedGasEOS )then
                    gammaStiff=eospar(1)
                  pStiff=eospar(2)
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1,i2,i3).ne.0 )then
                        !  compute T = p/rho = ((gammaStiff-1)*rho*e - gammaStiff*pStiff)/rho
                        q(i1,i2,i3,tc)=((gammaStiff-1)*q(i1,i2,i3,tc)-
     & gammaStiff*pStiff)/q(i1,i2,i3,rc)
                      else
                        ! fixup unused points
                        q(i1,i2,i3,tc)=val(tc)
                      end if
                    end do
                    end do
                    end do
                  else if( equationOfState.eq.taitEOS )then
                    stop 2745
                  else if( equationOfState.eq.userDefinedEOS )then
                    ! Get T 
                    eosOption=1    ! get p=p(r,e)
                    eosDerivOption=0 ! no derivatives needed
                    iparEOS(1)=nd
                    do i3=n3a,n3b
                    do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1,i2,i3).ne.0 )then
                        !   T = (gamma-1)/R * (rho*e)/(rho)
                        r =q(i1,i2,i3,rc)
                        e =q(i1,i2,i3,tc)/r
                        do n=0,nq-1
                          qv(n)=q(i1,i2,i3,n)
                        end do
                        call getUserDefinedEOS( r,e,p,dp, eosOption, 
     & eosDerivOption, qv,iparEOS,rparEOS,userEOSDataPointer, ier )
                        q(i1,i2,i3,tc)=p/r ! T := p/rho
                      else
                        ! fixup unused points
                        q(i1,i2,i3,tc)=val(tc)
                      end if
                    end do
                    end do
                    end do
                  end if
                else
                  write(6,*)'Error (consPrim) : EOS type not supported'
                  stop
                end if
               end if
c handles non dudr2d and dudr3d cases (such as Jameson ???)
              else
c assume just Euler
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    ! e = p/(gamma-1)
                    q(i1,i2,i3,tc)=gammaRg*q(i1,i2,i3,tc)/q(i1,i2,i3,
     & rc)
                  else
                    ! fixup unused points
                    q(i1,i2,i3,tc)=val(tc)
                  end if
                end do
                end do
                end do
              end if
            end if ! end pde choice
          end if ! end conservative to primitive
        else
          if( option.eq.0 )then
            ! ****** Primitive to Conservative ******
          if( pdeVariation.eq.conservativeGodunov .and. 
     & equationOfState.eq.jwlEOS )then
          ! write(55,*)'consPrim(p->c)'
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
            ! compute specific internal energy en given (rho,p,lambda), possibly update vs and vg
            if (mask(i1,i2,i3).ne.0) then
          !   write(55,*)i1,i2,(q(i1,i2,i3,i),i=0,6)
              ier=0
              r=q(i1,i2,i3,rc)
              if (r.lt.1.e-5) then
              write(6,*)'q =',(q(i1,i2,i3,i),i=0,6)
              write(6,*)'mask =',mask(i1,i2,i3)
              stop
              end if
              y=r*q(i1,i2,i3,sc)
              vs=q(i1,i2,i3,sc+1)
              vg=q(i1,i2,i3,sc+2)
              p=Rg*r*q(i1,i2,i3,tc)
              call geteos (r,e,y,vs,vg,p,dp,-1,ier)
              if (ier.ne.0) then
                write(6,*)'Error (consPrim) : call to geteos failed (p-
     & >c)'
                stop
              end if
              q(i1,i2,i3,tc)=e/r
              q(i1,i2,i3,sc)=y
              q(i1,i2,i3,sc+1)=vs
              q(i1,i2,i3,sc+2)=vg
c*wdh  else
c*wdh    q(i1,i2,i3,tc)=val(tc)
c*wdh    q(i1,i2,i3,sc)=val(sc)
c*wdh    q(i1,i2,i3,sc+1)=vs0
c*wdh    q(i1,i2,i3,sc+2)=vg0
            end if
          end do
          end do
          end do
          if( nd.eq.1 )then
            ! *** 1D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              ! E = rho*( e + .5*u*u )
              if( mask(i1,i2,i3).ne.0 )then
              q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*( q(i1,i2,i3,tc)+.5*q(i1,
     & i2,i3,uc)**2 )
              q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
              else
                q(i1,i2,i3,rc)=val(rc)
                q(i1,i2,i3,tc)=val(tc)
                q(i1,i2,i3,uc)=val(uc)
              end if
            end do
            end do
            end do
          else if( nd.eq.2 )then
            ! *** 2D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              !  E = rho*( e + .5*(u*u+v*v) )
              if( mask(i1,i2,i3).ne.0 )then
                q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*( q(i1,i2,i3,tc)+.5*(q(
     & i1,i2,i3,uc)**2 + q(i1,i2,i3,vc)**2))
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*q(i1,i2,i3,rc)
              else
                q(i1,i2,i3,rc)=val(rc)
                q(i1,i2,i3,tc)=val(tc)
                q(i1,i2,i3,uc)=val(uc)
                q(i1,i2,i3,vc)=val(vc)
              end if
            end do
            end do
            end do
          else
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).ne.0 )then
              !  E = rho*( e + .5*(u*u+v*v+w*w) )
                q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*( q(i1,i2,i3,tc)+.5*(q(
     & i1,i2,i3,uc)**2+q(i1,i2,i3,vc)**2+q(i1,i2,i3,wc)**2))
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*q(i1,i2,i3,rc)
                q(i1,i2,i3,wc)=q(i1,i2,i3,wc)*q(i1,i2,i3,rc)
              else
                q(i1,i2,i3,rc)=val(rc)
                q(i1,i2,i3,tc)=val(tc)
                q(i1,i2,i3,uc)=val(uc)
                q(i1,i2,i3,vc)=val(vc)
                q(i1,i2,i3,wc)=val(wc)
              end if
            end do
            end do
            end do
          end if
          else
          if( nd.eq.1 )then
            ! *** 1D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              ! E = ( p/(gamma-1) + .5*rho*u*u )
              if( mask(i1,i2,i3).ne.0 )then
              q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*( Rgg*q(i1,i2,i3,tc)+.5*q(
     & i1,i2,i3,uc)**2 )
              q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
              else
                q(i1,i2,i3,rc)=val(rc)
                q(i1,i2,i3,tc)=val(tc)
                q(i1,i2,i3,uc)=val(uc)
              end if
            end do
            end do
            end do
          else if( nd.eq.2 )then
            ! *** 2D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              !  E = ( p/(gamma-1) + .5*rho*u*u )
              if( mask(i1,i2,i3).ne.0 )then
                q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*( Rgg*q(i1,i2,i3,tc)+.5*(
     & q(i1,i2,i3,uc)**2 + q(i1,i2,i3,vc)**2))
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*q(i1,i2,i3,rc)
              else
                q(i1,i2,i3,rc)=val(rc)
                q(i1,i2,i3,tc)=val(tc)
                q(i1,i2,i3,uc)=val(uc)
                q(i1,i2,i3,vc)=val(vc)
              end if
            end do
            end do
            end do
          else
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).ne.0 )then
              !  E = ( p/(gamma-1) + .5*rho*u*u )
                q(i1,i2,i3,tc)=q(i1,i2,i3,rc)*( Rgg*q(i1,i2,i3,tc)+.5*(
     & q(i1,i2,i3,uc)**2+q(i1,i2,i3,vc)**2+q(i1,i2,i3,wc)**2))
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*q(i1,i2,i3,rc)
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*q(i1,i2,i3,rc)
                q(i1,i2,i3,wc)=q(i1,i2,i3,wc)*q(i1,i2,i3,rc)
              else
                q(i1,i2,i3,rc)=val(rc)
                q(i1,i2,i3,tc)=val(tc)
                q(i1,i2,i3,uc)=val(uc)
                q(i1,i2,i3,vc)=val(vc)
                q(i1,i2,i3,wc)=val(wc)
              end if
            end do
            end do
            end do
          end if
          ! Species
          do is=0,ns-1
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).ne.0 )then
                q(i1,i2,i3,sc+is)=q(i1,i2,i3,sc+is)*q(i1,i2,i3,rc)
              else
                q(i1,i2,i3,sc+is)=val(is+sc)
              end if
            end do
            end do
            end do
          end do
          if( pdeVariation.eq.conservativeGodunov )then
            ! here is where psi comes in for general eos, e.g.
            ! q(all,all,all,tc)+=rho*psi(rho)
            fact=1.
            if( nd.eq.2 )then
              call addpsi(nd1a,nd1b,nd2a,nd2b,fact,q(nd1a,nd2a,nd3a,rc)
     & ,q(nd1a,nd2a,nd3a,tc))
            end if
            if( 
     & reactionType.eq.oneStep.or.reactionType.eq.oneStepPress )then
              ! E = E - Q*(rho*product)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)-heatRelease*q(i1,i2,i3,
     & sc)
              end do
              end do
              end do
            else if( reactionType.eq.branching )then
              ! E = E - [Q*(rho*product) - R*(rho*radical)]
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)-heatRelease*q(i1,i2,i3,
     & sc)-absorbedEnergy*q(i1,i2,i3,sc+1)
              end do
              end do
              end do
            else if( reactionType.eq.ignitionAndGrowth )then
              ! E = E - Q*(rho*product)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)-heatRelease*q(i1,i2,i3,
     & sc)
                q(i1,i2,i3,sc+1)=q(i1,i2,i3,sc+1)/q(i1,i2,i3,rc)
                q(i1,i2,i3,sc+2)=q(i1,i2,i3,sc+2)/q(i1,i2,i3,rc)
              end do
              end do
              end do
            end if
          end if
          end if
          else
            ! ****** Conservative to Primitive ******
          if( pdeVariation.eq.conservativeGodunov .and. 
     & equationOfState.eq.jwlEOS )then
          ! write(55,*)'consPrim(c->p)'
          if( nd.eq.1 )then
            ! *** 1D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              ! e = ( E/rho - .5*u*u )
              if( mask(i1,i2,i3).ne.0 )then
              q(i1,i2,i3,uc)=q(i1,i2,i3,uc)/q(i1,i2,i3,rc)
              q(i1,i2,i3,tc)=q(i1,i2,i3,tc)/q(i1,i2,i3,rc)-.5*q(i1,i2,
     & i3,uc)**2
              else
                q(i1,i2,i3,rc)=val(rc)
                q(i1,i2,i3,tc)=val(tc)
                q(i1,i2,i3,uc)=val(uc)
              end if
            end do
            end do
            end do
          else if( nd.eq.2 )then
            ! *** 2D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              !  e = ( E/rho - .5*(u*u+v*v) )
              if( mask(i1,i2,i3).ne.0 )then
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)/q(i1,i2,i3,rc)
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)/q(i1,i2,i3,rc)
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)/q(i1,i2,i3,rc)-.5*(q(i1,
     & i2,i3,uc)**2 + q(i1,i2,i3,vc)**2)
              else
                q(i1,i2,i3,rc)=val(rc)
                q(i1,i2,i3,tc)=val(tc)
                q(i1,i2,i3,uc)=val(uc)
                q(i1,i2,i3,vc)=val(vc)
              end if
            end do
            end do
            end do
          else
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).ne.0 )then
              !  e = ( E/rho - .5*(u*u+v*v+w*w) )
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)/q(i1,i2,i3,rc)
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)/q(i1,i2,i3,rc)
                q(i1,i2,i3,wc)=q(i1,i2,i3,wc)/q(i1,i2,i3,rc)
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)/q(i1,i2,i3,rc)-.5*(q(i1,
     & i2,i3,uc)**2+q(i1,i2,i3,vc)**2+q(i1,i2,i3,wc)**2)
              else
                q(i1,i2,i3,rc)=val(rc)
                q(i1,i2,i3,tc)=val(tc)
                q(i1,i2,i3,uc)=val(uc)
                q(i1,i2,i3,vc)=val(vc)
                q(i1,i2,i3,wc)=val(wc)
              end if
            end do
            end do
            end do
          end if
          do i3=n3a,n3b
          do i2=n2a,n2b
          do i1=n1a,n1b
            ! compute pressure p given (rho,en,lambda), possibly update vs and vg
            if (mask(i1,i2,i3).ne.0) then
          !   write(55,*)i1,i2,(q(i1,i2,i3,i),i=0,6)
              ier=0
              r=q(i1,i2,i3,rc)
              e=r*q(i1,i2,i3,tc)
              y=q(i1,i2,i3,sc)
              vs=q(i1,i2,i3,sc+1)
              vg=q(i1,i2,i3,sc+2)
              call geteos (r,e,y,vs,vg,p,dp,0,ier)
          !   write(55,'(2(1x,i2),9(1x,f11.8))')i1,i2,(q(i1,i2,i3,i),i=0,6),vs,vg
              if (ier.ne.0) then
                write(6,*)'Error (consPrim) : call to geteos failed (c-
     & >p)'
                stop
              end if
              q(i1,i2,i3,tc)=p/(r*Rg)
              q(i1,i2,i3,sc)=y/r
              q(i1,i2,i3,sc+1)=vs
              q(i1,i2,i3,sc+2)=vg
c dws
c  else
c    q(i1,i2,i3,tc)=val(tc)
c    q(i1,i2,i3,sc)=val(sc)
c    q(i1,i2,i3,sc+1)=vs0
c    q(i1,i2,i3,sc+2)=vg0
            end if
          end do
          end do
          end do
          else
          if( nd.eq.1 )then
            ! *** 1D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              ! E = ( p/(gamma-1) + .5*rho*u*u )
              if( mask(i1,i2,i3).ne.0 )then
                ! rhoi=1./max(epsRho,q(i1,i2,i3,rc))
                 if( q(i1,i2,i3,rc).lt.epsRho )then
                   imask=mask(i1,i2,i3)
                   if( imask.lt.0 )then
                     imask=-1
                   else if( imask.gt.0 )then
                     imask=1
                   end if
c    write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
                   if( mask(i1,i2,i3).eq.0 )then
                     q(i1,i2,i3,rc)=1.
                   else
                     q(i1,i2,i3,rc)=epsRho
                   end if
                 end if
                rhoi=1./q(i1,i2,i3,rc)
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*rhoi
                q(i1,i2,i3,tc)=gammaRg*( q(i1,i2,i3,tc)*rhoi - .5*(q(
     & i1,i2,i3,uc)**2) )
              else
                q(i1,i2,i3,rc)=val(rc)
                q(i1,i2,i3,tc)=val(tc)
                q(i1,i2,i3,uc)=val(uc)
              end if
            end do
            end do
            end do
          else if( nd.eq.2 )then
            ! *** 2D ***
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              !  E = ( p/(gamma-1) + .5*rho*u*u )
              if( mask(i1,i2,i3).ne.0 )then
                ! rhoi=1./max(epsRho,q(i1,i2,i3,rc))
                 if( q(i1,i2,i3,rc).lt.epsRho )then
                   imask=mask(i1,i2,i3)
                   if( imask.lt.0 )then
                     imask=-1
                   else if( imask.gt.0 )then
                     imask=1
                   end if
c    write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
                   if( mask(i1,i2,i3).eq.0 )then
                     q(i1,i2,i3,rc)=1.
                   else
                     q(i1,i2,i3,rc)=epsRho
                   end if
                 end if
                rhoi=1./q(i1,i2,i3,rc)
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*rhoi
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*rhoi
                q(i1,i2,i3,tc)=gammaRg*( q(i1,i2,i3,tc)*rhoi - .5*(q(
     & i1,i2,i3,uc)**2 + q(i1,i2,i3,vc)**2 ) )
              else
                q(i1,i2,i3,rc)=val(rc)
                q(i1,i2,i3,tc)=val(tc)
                q(i1,i2,i3,uc)=val(uc)
                q(i1,i2,i3,vc)=val(vc)
              end if
            end do
            end do
            end do
          else
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              !  E = ( p/(gamma-1) + .5*rho*u*u )
              if( mask(i1,i2,i3).ne.0 )then
                ! rhoi=1./max(epsRho,q(i1,i2,i3,rc))
                 if( q(i1,i2,i3,rc).lt.epsRho )then
                   imask=mask(i1,i2,i3)
                   if( imask.lt.0 )then
                     imask=-1
                   else if( imask.gt.0 )then
                     imask=1
                   end if
c    write(*,'("consPrim:WARNING: i=",3i4," rho=",e8.2," epsRho=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,rc),epsRho,imask
                   if( mask(i1,i2,i3).eq.0 )then
                     q(i1,i2,i3,rc)=1.
                   else
                     q(i1,i2,i3,rc)=epsRho
                   end if
                 end if
                rhoi=1./q(i1,i2,i3,rc)
                q(i1,i2,i3,uc)=q(i1,i2,i3,uc)*rhoi
                q(i1,i2,i3,vc)=q(i1,i2,i3,vc)*rhoi
                q(i1,i2,i3,wc)=q(i1,i2,i3,wc)*rhoi
                q(i1,i2,i3,tc)=gammaRg*( q(i1,i2,i3,tc)*rhoi - .5*(q(
     & i1,i2,i3,uc)**2 + q(i1,i2,i3,vc)**2 + q(i1,i2,i3,wc)**2 ) )
                 if( q(i1,i2,i3,tc).lt.epsRho )then
                   imask=mask(i1,i2,i3)
                   if( imask.lt.0 )then
                     imask=-1
                   else if( imask.gt.0 )then
                     imask=1
                   end if
c    write(*,'("consPrim:WARNING: i=",3i4," T=",e8.2," epsT=",e8.2,", mask=",i2)') i1,i2,i3,q(i1,i2,i3,tc),epsRho,imask
                   if( mask(i1,i2,i3).eq.0 )then
                     q(i1,i2,i3,tc)=1.
                   else
                     q(i1,i2,i3,tc)=epsRho
                   end if
                 end if
              else
                q(i1,i2,i3,rc)=val(rc)
                q(i1,i2,i3,tc)=val(tc)
                q(i1,i2,i3,uc)=val(uc)
                q(i1,i2,i3,vc)=val(vc)
                q(i1,i2,i3,wc)=val(wc)
              end if
            end do
            end do
            end do
          end if
          ! Species
          do is=0,ns-1
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).ne.0 )then
                q(i1,i2,i3,sc+is)=q(i1,i2,i3,sc+is)/max(epsRho,q(i1,i2,
     & i3,rc))
              else
                q(i1,i2,i3,sc+is)=val(is+sc)
              end if
            end do
            end do
            end do
          end do
          if( pdeVariation.eq.conservativeGodunov )then
              ! here is where psi comes in for general eos, e.g.
              !  q(all,all,all,tc)-=((gamma-1.)/Rg)*psi(rho);
            fact=-(gamma-1.)/Rg
            if( nd.eq.2 )then
              call addpsi(nd1a,nd1b,nd2a,nd2b,fact,q(nd1a,nd2a,nd3a,rc)
     & ,q(nd1a,nd2a,nd3a,tc))
            end if
            if(  
     & reactionType.eq.oneStep.or.reactionType.eq.oneStepPress )then
              ! E = E - Q*(rho*product)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+gammaRg*heatRelease*q(i1,
     & i2,i3,sc)
              end do
              end do
              end do
            else if(  reactionType.eq.branching )then
              ! E = E - [Q*(rho*product) - R*(rho*radical)]
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+gammaRg*heatRelease*q(i1,
     & i2,i3,sc)-gammaRg*absorbedEnergy*q(i1,i2,i3,sc+1)
              end do
              end do
              end do
            else if(  reactionType.eq.ignitionAndGrowth )then
              ! E = E - Q*(rho*product)
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                q(i1,i2,i3,tc)=q(i1,i2,i3,tc)+gammaRg*heatRelease*q(i1,
     & i2,i3,sc)
                q(i1,i2,i3,sc+1)=q(i1,i2,i3,sc+1)*q(i1,i2,i3,rc)
                q(i1,i2,i3,sc+2)=q(i1,i2,i3,sc+2)*q(i1,i2,i3,rc)
              end do
              end do
              end do
            end if
          end if
          end if
          end if
        endif

      end if

c       write(6,*)'consPrim(out)'
c       write(6,'(9(1x,i1,1x,f15.8,/))')(i,q(0,0,0,i),i=0,8)
c       pause

      return
      end
