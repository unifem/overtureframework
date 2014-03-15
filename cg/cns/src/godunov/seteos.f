      subroutine seteos (m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *                   nd3a,nd3b,n3a,n3b,u,mask,nrprm,rparam,
     *                   niprm,iparam,ier)
c
c seteos updates EOS params contained in u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,.)
c (seteos is called from ~henshaw/res/OverBlown/updateStateVariables.C)
c
c seteos also limits all reacting species to [0,1] when istage.eq.2
c
      implicit real*8 (a-h,o-z)
      dimension u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,m),
     *          mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b),
     *          rparam(nrprm),iparam(niprm),dp(10)
c
      include 'eosDefine.h'  ! define the EOS names, idealGasEOS, jwlEOS, mieGruneisenEOS, userDefinedEOS
c
c 
c     include 'multijwl.h'
c.. Can't include multijwl.h because this file (seteos.f) is NOT autodoubled!! Must explicitly
c    include common blocks from this file. We should be very careful here!!
      real*8 gm1s,amjwl,rmjwl,ai,ri,fs0,fg0,gs0,gg0,
     *     fi0,gi0,ci,cs,cg,mjwlq,mvi0,mvs0,mvg0,iheat
      integer iterations,newMethod
      common / multijwl / gm1s(3),amjwl(2,2),rmjwl(2,2),
     *     ai(2),ri(2),fs0,fg0,gs0,gg0,fi0,gi0,ci,cs,cg,mjwlq,mvi0,
     *     mvs0,mvg0,iheat,iterations,newMethod
       include 'eosdat.h'
c      common / eosdat / omeg(2),ajwl(2,2),rjwl(2,2),vs0,ts0,
c     *                  fsvs0,zsvs0,vg0,fgvg0,zgvg0,cgcs,heat
c
      ier=0
      ieos=iparam(1)
      imult=iparam(11)
      ides=iparam(16)
      myid=iparam(12)
      if (ieos.lt.0.or.ieos.ge.numberOfEOS) then
        write(6,*)'Error (seteos) : invalid value for ieos'
        stop
      end if
c
c..check if there are any reacting species (if ieos=1, then mr includes vs,vg
c  and these should be subtracted from mr).
      mr=iparam(2)
      if (ieos.eq.jwlEOS) mr=mr-2
      if( imult.eq.1 ) mr=mr-1
      if (ieos.ne.jwlEOS.and.mr.eq.0) return
c
c..if istage=1 and ieos=0 (an ideal gas), then there is nothing to do.
      istage=iparam(3)
      if (istage.eq.1.and.ieos.ne.jwlEOS ) return
c
      nd=iparam(10)
      if (nd.eq.2) then
        if( imult.eq.0 ) then
          ! single component 2-D case
          if( ides.eq.0 ) then
            ie=4
            is=5
            ivs=6
            ivg=7
          else
            ie=4
            is=5
            ip=6
            ivs=7
            ivg=8
          end if
        else
          ! two-component 2-D case
          if( ides.eq.0 ) then
            ie=4
            im=5
            is=6
            ivi=7
            ivs=8
            ivg=9
          else
            ie=4
            im=5
            is=6
            ip=7
            ivi=8
            ivs=9
            ivg=10
          end if
        end if
      else
        ! single component 3-D case ... no two-component 3-D case yet
        ie=5
        is=6
        ivs=7
        ivg=8
      end if
c
c..Only need to this next part for single component case
c  For two component case this is done already in setUserDefinedParameters.C
c      if (ieos.eq.jwlEOS.and.imult.eq.0) then
c        omeg(1)=rparam(1)
c        ajwl(1,1)=rparam(2)
c        ajwl(2,1)=rparam(3)
c        rjwl(1,1)=rparam(4)
c        rjwl(2,1)=rparam(5)
c        omeg(2)=rparam(6)
c        ajwl(1,2)=rparam(7)
c        ajwl(2,2)=rparam(8)
c        rjwl(1,2)=rparam(9)
c        rjwl(2,2)=rparam(10)
c        vs0=rparam(11)
c        ts0=1.d0
c        zsvs0=vs0*(ajwl(1,1)*dexp(-rjwl(1,1)*vs0)
c     *            +ajwl(2,1)*dexp(-rjwl(2,1)*vs0))/omeg(1)
c        fsvs0=zsvs0-ajwl(1,1)*dexp(-rjwl(1,1)*vs0)/rjwl(1,1)
c     *            -ajwl(2,1)*dexp(-rjwl(2,1)*vs0)/rjwl(2,1)
c        vg0=rparam(12)
c        zgvg0=vg0*(ajwl(1,2)*dexp(-rjwl(1,2)*vg0)
c     *            +ajwl(2,2)*dexp(-rjwl(2,2)*vg0))/omeg(2)
c        fgvg0=zgvg0-ajwl(1,2)*dexp(-rjwl(1,2)*vg0)/rjwl(1,2)
c     *             -ajwl(2,2)*dexp(-rjwl(2,2)*vg0)/rjwl(2,2)
c        cgcs=rparam(13)
c        heat=rparam(14)
c      end if
c
      if (istage.eq.1) then
c
c Stage 1: u has been advanced by up and has had interpolated point updated.
c          It is in conservative variable form (rho,rho*u,E,rho*lambda,vs,vg)
c
c       write(55,*)'** seteos : stage 1 **'
c
c..update vs,vg in the interior for the mixture eos
        if( ieos.ne.jwlEOS )then
          write(*,'("seteos:ERROR unexpected ieos=",i4)') ieos
          stop 8865
        end if
        do j3=n3a,n3b
        do j2=n2a,n2b
        do j1=n1a,n1b
          if (mask(j1,j2,j3).ne.0) then
            r=u(j1,j2,j3,1)
            q2=0.d0
            do i=1,nd
              q2=q2+(u(j1,j2,j3,i+1)/r)**2
            end do
            e=u(j1,j2,j3,ie)-r*q2/2.d0
            y=u(j1,j2,j3,is)
            if (y.lt.0.d0) then
              y=0.d0
              u(j1,j2,j3,is)=0.d0
            elseif (y.gt.r) then
              y=r
              u(j1,j2,j3,is)=r
            end if
            vs=u(j1,j2,j3,ivs)
            vg=u(j1,j2,j3,ivg)
            if( imult.eq.1 ) then
c              p=.1
              e=e/r
              rlam=y/r
              rmu=u(j1,j2,j3,im)/r
              if( rmu.lt.0.d0 ) then
                rmu=0.d0
c                u(j1,j2,j3,is+1)=0.d0
                u(j1,j2,j3,im)=0.d0
              else if( rmu.gt.1.d0 ) then
                rmu=1.d0
                u(j1,j2,j3,im)=r
              end if
              vi=u(j1,j2,j3,ivi)
              if( newMethod.eq.0 ) then
                call geteosb( r,e,rmu,rlam,vi,vs,vg,p,dp,0,ier )
              else
                call geteosc( r,e,rmu,rlam,vi,vs,vg,p,dp,0,ier )
              end if
            else
              call geteos (r,e,y,vs,vg,p,dp,0,ier)
            end if
            if (ier.ne.0) then
              write(17,*)'Error (seteos) : stage 1, j1,j2,j3 =',
     *                    j1,j2,j3
              write(17,*)'u =',(u(j1,j2,j3,i),i=1,m)
              stop
            else
              u(j1,j2,j3,ivs)=vs
              u(j1,j2,j3,ivg)=vg
              if( imult.eq.1 ) u(j1,j2,j3,ivi)=vi
            end if
          end if
        end do
        end do
        end do
c
      else
c
c Stage 2: boundary conditions have been applied to u at physical boundaries.
c          It is in primitive variable form (rho,u,p/rho,lambda,vs,vg)
c
c       write(55,*)'** seteos : stage 2 **'
c
c..limit lambda
        ilow=is
        if( imult.eq.1 ) ilow=im
        do i=ilow,ilow+mr-1
          do j3=nd3a,nd3b
          do j2=nd2a,nd2b
          do j1=nd1a,nd1b
            alam=u(j1,j2,j3,i)
            if (alam.lt.0.d0) then
              u(j1,j2,j3,i)=0.d0
            elseif (alam.gt.1.d0) then
              u(j1,j2,j3,i)=1.d0
            end if
          end do
          end do
          end do
        end do
c
        if (ieos.ne.jwlEOS) return

        if( ieos.ne.jwlEOS )then
          write(*,'("seteos:ERROR unexpected ieos")') 
          stop 8865
        end if
c
c..update vs,vg for mixture eos
        if (iparam(4).gt.0) then   ! bc(side=0,axis=0)>0 ?
          do j3=n3a,n3b
          do j2=n2a,n2b
            if (mask(n1a,j2,j3).ne.0) then
              do j1=n1a-1,n1a
                r=u(j1,j2,j3,1)
                p=r*u(j1,j2,j3,ie)
                y=r*u(j1,j2,j3,is)
                vs=u(j1,j2,j3,ivs)
                vg=u(j1,j2,j3,ivg)
                if( imult.eq.1 ) then
c                  e=.1
                  rlam=y/r
                  rmu=u(j1,j2,j3,im)
                  vi=u(j1,j2,j3,ivi)
                  if( newMethod.eq.0 ) then
                    call geteosb( r,e,rmu,rlam,vi,vs,vg,p,dp,-1,ier )
                  else
                    call geteosc( r,e,rmu,rlam,vi,vs,vg,p,dp,-1,ier )
                  end if
                else
                  call geteos (r,e,y,vs,vg,p,dp,-1,ier)
                end if
                if (ier.ne.0) then
                  write(17,*)'Error (seteos) : stage 2a, j1,j2,j3 =',
     *                       j1,j2,j3
                  write(17,*)'u =',(u(j1,j2,j3,i),i=1,m)
                  stop
                end if
                u(j1,j2,j3,ivs)=vs
                u(j1,j2,j3,ivg)=vg
                if( imult.eq.1 ) u(j1,j2,j3,ivi)=vi
              end do
            end if
          end do
          end do
        end if
c
        if (iparam(5).gt.0) then   ! bc(side=1,axis=0)>0 ?
          do j3=n3a,n3b
          do j2=n2a,n2b
            if (mask(n1b,j2,j3).ne.0) then
              do j1=n1b,n1b+1
                r=u(j1,j2,j3,1)
                p=r*u(j1,j2,j3,ie)
                y=r*u(j1,j2,j3,is)
                vs=u(j1,j2,j3,ivs)
                vg=u(j1,j2,j3,ivg)
                if( imult.eq.1 ) then
c                  e=.1
                  rlam=y/r
                  rmu=u(j1,j2,j3,im)
                  vi=u(j1,j2,j3,ivi)
                  if( newMethod.eq.0 ) then
                    call geteosb( r,e,rmu,rlam,vi,vs,vg,p,dp,-1,ier )
                  else
                    call geteosc( r,e,rmu,rlam,vi,vs,vg,p,dp,-1,ier )
                  end if
                else
c                  call geteos (r,p,y,vs,vg,p,dp,-1,ier)
                  call geteos (r,e,y,vs,vg,p,dp,-1,ier)
                end if
                if (ier.ne.0) then
                  write(17,*)'Error (seteos) : stage 2b, j1,j2,j3 =',
     *                       j1,j2,j3
                  write(17,*)'u =',(u(j1,j2,j3,i),i=1,m)
                  stop
                end if
                u(j1,j2,j3,ivs)=vs
                u(j1,j2,j3,ivg)=vg
                if( imult.eq.1 ) u(j1,j2,j3,ivi)=vi
              end do
            end if
          end do
          end do
        end if
c
        if (iparam(6).gt.0) then   ! bc(side=0,axis=1)>0 ?
          do j3=n3a,n3b
          do j1=n1a,n1b
            if (mask(j1,n2a,j3).ne.0) then
              do j2=n2a-1,n2a
                r=u(j1,j2,j3,1)
                p=r*u(j1,j2,j3,ie)
                y=r*u(j1,j2,j3,is)
                vs=u(j1,j2,j3,ivs)
                vg=u(j1,j2,j3,ivg)
                if( imult.eq.1 ) then
c                  e=.1
                  rlam=y/r
                  rmu=u(j1,j2,j3,im)
                  vi=u(j1,j2,j3,ivi)
                  if( newMethod.eq.0 ) then
                    call geteosb( r,e,rmu,rlam,vi,vs,vg,p,dp,-1,ier )
                  else
                    call geteosc( r,e,rmu,rlam,vi,vs,vg,p,dp,-1,ier )
                  end if
                else
                  call geteos (r,e,y,vs,vg,p,dp,-1,ier)
                end if
                if (ier.ne.0) then
                  write(17,*)'Error (seteos) : stage 2c, j1,j2,j3 =',
     *                       j1,j2,j3
                  write(17,*)'u =',(u(j1,j2,j3,i),i=1,m)
                  stop
c                   write(6,*)'Error (seteos) : stage 2c'
c                   ier=0
c                   return
                end if
                u(j1,j2,j3,ivs)=vs
                u(j1,j2,j3,ivg)=vg
                if( imult.eq.1 ) u(j1,j2,j3,ivi)=vi
              end do
            end if
          end do
          end do
        end if
c
        if (iparam(7).gt.0) then   ! bc(side=1,axis=1)>0 ?
          do j3=n3a,n3b
          do j1=n1a,n1b
            if (mask(j1,n2b,j3).ne.0) then
              do j2=n2b,n2b+1
                r=u(j1,j2,j3,1)
                p=r*u(j1,j2,j3,ie)
                y=r*u(j1,j2,j3,is)
                vs=u(j1,j2,j3,ivs)
                vg=u(j1,j2,j3,ivg)
                if( imult.eq.1 ) then
c                  e=.1
                  rlam=y/r
                  rmu=u(j1,j2,j3,im)
                  vi=u(j1,j2,j3,ivi)
                  if( newMethod.eq.0 ) then
                    call geteosb( r,e,rmu,rlam,vi,vs,vg,p,dp,-1,ier )
                  else
                    call geteosc( r,e,rmu,rlam,vi,vs,vg,p,dp,-1,ier )
                  end if
                else
c                  call geteos (r,p,y,vs,vg,p,dp,-1,ier)
                  call geteos (r,e,y,vs,vg,p,dp,-1,ier)
                end if
                if (ier.ne.0) then
                  write(17,*)'Error (seteos) : stage 2d, j1,j2,j3 =',
     *                       j1,j2,j3
                  write(17,*)'u =',(u(j1,j2,j3,i),i=1,m)
                  stop
                end if
                u(j1,j2,j3,ivs)=vs
                u(j1,j2,j3,ivg)=vg
                if( imult.eq.1 ) u(j1,j2,j3,ivi)=vi
              end do
            end if
          end do
          end do
        end if
c
        if (nd.eq.2) return
c
c..only done if 3d
        if (iparam(8).gt.0) then   ! bc(side=0,axis=2)>0 ?
          do j2=n2a,n2b
          do j1=n1a,n1b
            if (mask(j1,j2,n3a).ne.0) then
              do j3=n3a-1,n3a
                r=u(j1,j2,j3,1)
                p=r*u(j1,j2,j3,ie)
                y=r*u(j1,j2,j3,is)
                vs=u(j1,j2,j3,ivs)
                vg=u(j1,j2,j3,ivg)
                if( imult.eq.1 ) then
                  write(6,*)'Error (seteos): no 2-comp in 3-D yet'
                  stop
                end if
                call geteos (r,e,y,vs,vg,p,dp,-1,ier)
                if (ier.ne.0) then
                  write(17,*)'Error (seteos) : stage 2e, j1,j2,j3 =',
     *                       j1,j2,j3
                  write(17,*)'u =',(u(j1,j2,j3,i),i=1,m)
                  stop
                end if
                u(j1,j2,j3,ivs)=vs
                u(j1,j2,j3,ivg)=vg
              end do
            end if
          end do
          end do
        end if
c
        if (iparam(9).gt.0) then   ! bc(side=1,axis=2)>0 ?
          do j2=n2a,n2b
          do j1=n1a,n1b
            if (mask(j1,j2,n3b).ne.0) then
              do j3=n3b,n3b+1
                r=u(j1,j2,j3,1)
                p=r*u(j1,j2,j3,ie)
                y=r*u(j1,j2,j3,is)
                vs=u(j1,j2,j3,ivs)
                vg=u(j1,j2,j3,ivg)
                if( imult.eq.1 ) then
                  write(6,*)'Error (seteos): no 2-comp in 3-D yet'
                  stop
                end if
                call geteos (r,e,y,vs,vg,p,dp,-1,ier)
                if (ier.ne.0) then
                  write(17,*)'Error (seteos) : stage 2f, j1,j2,j3 =',
     *                       j1,j2,j3
                  write(17,*)'u =',(u(j1,j2,j3,i),i=1,m)
                  stop
                end if
                u(j1,j2,j3,ivs)=vs
                u(j1,j2,j3,ivg)=vg
              end do
            end if
          end do
          end do
        end if
c
      end if
c
      return
      end
