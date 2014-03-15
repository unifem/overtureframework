      subroutine geteosb (rho,e,mu,lam,vi,vs,vg,p,dp,iform,ier)
c
c Evaluate the equation of state p=p(rho,rho*e,,rho*mu,rho*lambda) for the
c mixture JWL EOS and a second inert gas used for multi-fluid Ignition and Growth 
c detonation modeling.
c
c Variables: r=density, rho
c            e=internal energy per unit mass, E/rho-|velocity|^2/2
c            mu=species mass fraction
c            lam=progress mass fraction
c            vi=specific volume of inert
c            vs=specific volume of solid explosive
c            vg=specific volume of gas products
c            p=pressure
c            dp=derivative of pressure w/r.t. (rho,e,rho*mu,rho*lam)
c            iform=integer flag (see below)
c            ier=error return flag
c
c
c Note: Within a tolerance the three end cases of all inert (mu=0),
c     unburnt (mu=1, lam=0), and burnt (mu=1,lam=1) yeild an explicit
c     expression for p so no Newton iteration is needed. These will
c     be handled seperately.
c
c
c if iform=0, then e (energy) is given and p (pressure) is returned
c         >0, then e is given and p and its derivatives dp are returned
c         <0, then p is given and e is returned
c
c if ier=0 on input, then failure of Newton iteration results in an error
c          message and a solution dump
c       <0 on input, then no error message is given if Newton fails
c
c In either case, ier.ne.0 on return if Newton fails.
c
      implicit none
      integer iform,ier
      real rho,e,mu,lam,vi,vs,vg,p
      include 'tempSizes.h'
      real dp(dpSize)
c
c local variables
      integer i,itr,ipvt(4),itmax,hittol
      integer info,failSafe,failSafeItr
      real a(3,3),b(3,4),dv(4),agen(4,4),rcond,norm
      real fi,fs,fg,gi,gs,gg
      real fip,fsp,fgp,gip,gsp,ggp
      real t1,t2,t3,phi,psi
      real viold,vsold,vgold,pold,eold
      real MAXJUMP
      real viSafe,vsSafe,vgSafe,pSafe,eSafe
      real rho_e, rho_lam
c
      include 'multijwl.h'
      real tol,near,pmin,wi,ws,wg
      real near2,pnear,rhoMin,vgMax
      parameter( MAXJUMP=1000,itmax=25 )
c
c      real ai(2),ri(2)
      data tol / 1.e-8 /
c      data near, pmin, near2 / 1.e-6, 1.e-10, 1.e-3 /
      data near, pnear, pmin, rhoMin / 1.e-3, 1.e-3, 0.e0, 1.e-4 /
      data vgMax / 15.e0 /
c
c.. first we limit the initial guesses to fall within "realistic" limits
      vi=min(vi,1.)
      vs=min(vs,1.)
      vg=min(vg,5.)
      vi=max(vi,0.5)
      vs=max(vs,0.5)
      vg=max(vg,0.5)
c
      wi=gm1s(1)
      ws=gm1s(2)
      wg=gm1s(3)
c
      viold=vi
      vsold=vs
      vgold=vg
      pold=p
      eold=e
      p=max(0.e0,p)
      e=max(0.e0,e)
      lam=max(0.e0,lam)
      lam=min(1.e0,lam)
      mu=max(0.e0,mu)
      mu=min(1.e0,mu)
      rho=max(rho,rhoMin)
c
c..First check to see if we are close to one of the end cases.
c  Note here that these are the most used case by far.
c
c      if( mu.lt.near ) then
      if( mu.lt.1.e-10 ) then
c..Inert case
        vi=1.e0/rho
        vs=mvs0
        vg=mvg0
        fi=0.e0
        fip=0.e0
        do i=1,2
          t1=dexp(-ri(i)*vi)
          t2=ai(i)*(vi/wi-1.e0/ri(i))
          fi=fi+t1*t2
          fip=fip+t1*(ai(i)/wi-t2*ri(i))
        end do
        fi=fi-fi0
        if( iform.ge.0 ) then
c..return p from e
c          p=max(0.e0,rho*(e+fi-iheat)*wi)
          p=rho*(e+fi-iheat)*wi
          if( iform.gt.0 ) then
c..return derivatives too
            dp(1)=wi*(-fip/rho+fi-iheat)
            dp(2)=wi
            dp(3)=0.e0
            dp(4)=0.e0
          end if
        else
c..return e from p
          e=p/(rho*wi)-fi+iheat
        end if
        ier=0
        return
c      elseif( 1.e0-mu.lt.near.and.lam.lt.near ) then
      elseif( 1.e0-mu.lt.1.e-10.and.lam.lt.1.e-10 ) then
c..Unburnt case
        vi=mvi0
        vs=1.e0/rho
        vg=mvg0
        fs=0.e0
        fsp=0.e0
        do i=1,2
          t1=dexp(-rmjwl(i,1)*vs)
          t2=amjwl(i,1)*(vs/ws-1.e0/rmjwl(i,1))
          fs=fs+t1*t2
          fsp=fsp+t1*(amjwl(i,1)/ws-t2*rmjwl(i,1))
        end do
        fs=fs-fs0
        if( iform.ge.0 ) then
c..return p from e
c          p=max(0.e0,rho*(e+fs-mjwlq)*ws)
          p=rho*(e+fs-mjwlq)*ws
          if( iform.gt.0 ) then
c..return derivatives too
            dp(1)=ws*(-fsp/rho+fs-mjwlq)
            dp(2)=ws
            dp(3)=0.e0
            dp(4)=0.e0
          end if
        else
c..return e from p
          e=p/(rho*ws)-fs+mjwlq
        end if
        ier=0
        return
      elseif( 1.e0-mu.lt.near.and.1.e0-lam.lt.near ) then
c..Burnt case
        vi=mvi0
        vs=mvs0
        vg=1.e0/rho
        fg=0.e0
        fgp=0.e0
        do i=1,2
          t1=dexp(-rmjwl(i,2)*vg)
          t2=amjwl(i,2)*(vg/wg-1.e0/rmjwl(i,2))
          fg=fg+t1*t2
          fgp=fgp+t1*(amjwl(i,2)/wg-t2*rmjwl(i,2))
        end do
        fg=fg-fg0
        if( iform.ge.0 ) then
c          p=max(0.e0,rho*(e+fg)*wg)
          p=rho*(e+fg)*wg
c..return p from e
          if( iform.gt.0 ) then
c..return derivatives too
            dp(1)=wg*(-fgp/rho+fg)
            dp(2)=wg
            dp(3)=0.e0
            dp(4)=0.e0
          end if
        else
c..return e from p
          e=p/(rho*wg)-fg
        end if
        ier=0
        return
c      elseif( 1.e0-lam.lt.1.e-10 )  then
      elseif( 1.e0-lam.lt.near )  then
c.. use Don's code for burnt JWL gases!! Mixture between inert and gas
c     Be sure to have common block set up properly
        rho_e=rho*e
        rho_lam=rho*mu
        call geteosburnt( rho,rho_e,rho_lam,vi,vg,p,dp,iform,ier )
        vs=mvs0
        if( iform.gt.0 ) dp(4)=0.e0
        if( iform.lt.0 ) e=rho_e/rho
        ier=0
        return
c      elseif( 1.e0-mu.lt.near )  then
      elseif( 1.e0-mu.lt.1.e-10 )  then
c.. use Don's code for reactive JWL gases!! Mixture between solid and gas
c     Be sure to have common block set up properly
        rho_e=rho*e
        rho_lam=rho*lam
        if( lam.gt.near.and.iform.lt.0.and.p.lt.pnear ) then
          p=pnear
        end if
        call geteos( rho,rho_e,rho_lam,vs,vg,p,dp,iform,ier )
        vi=mvi0
        if( iform.gt.0 ) then
          dp(4)=dp(3)
          dp(3)=0.e0
        end if
        if( iform.lt.0 ) e=rho_e/rho
        ier=0
        return
      else
c
c..If we get here we are not in an end case and must do
c  a Newton iteration
        failSafe=0     ! initially don't do fail safe iteration
        failSafeItr=0
        itr=0
        hittol=0       ! initiate the tolerance aquired flag
c
        do while (itr.lt.itmax )
c
c..evaluate f, g, fprime, and gprime
          fi=0.e0
          fs=0.e0
          fg=0.e0
c
          fsp=0.e0
          fgp=0.e0
          fip=0.e0
c
          gs=0.e0
          gg=0.e0
          gi=0.e0
c
          gsp=0.e0
          ggp=0.e0
          gip=0.e0
          do i=1,2
            t1=dexp(-ri(i)*vi)
            t2=ai(i)*(vi/wi-1.e0/ri(i))
            fi=fi+t1*t2
            fip=fip+t1*(ai(i)/wi-t2*ri(i))
            gi=gi+ai(i)/ri(i)*t1
            gip=gip-ai(i)*t1
c
            t1=dexp(-rmjwl(i,1)*vs)
            t2=amjwl(i,1)*(vs/ws-1.e0/rmjwl(i,1))
            fs=fs+t1*t2
            fsp=fsp+t1*(amjwl(i,1)/ws-t2*rmjwl(i,1))
            gs=gs+amjwl(i,1)/rmjwl(i,1)*t1
            gsp=gsp-amjwl(i,1)*t1
c
            t1=dexp(-rmjwl(i,2)*vg)
            t2=amjwl(i,2)*(vg/wg-1.e0/rmjwl(i,2))
            fg=fg+t1*t2
            fgp=fgp+t1*(amjwl(i,2)/wg-t2*rmjwl(i,2))
            gg=gg+amjwl(i,2)/rmjwl(i,2)*t1
            ggp=ggp-amjwl(i,2)*t1
          end do
          fs=fs-fs0
          fg=fg-fg0
          fi=fi-fi0
c
          gs=gs-gs0
          gg=gg-gg0
          gi=gi-gi0
c
c.. compute e or p
          if( iform.ge.0 ) then
            ! find p
            p=(e+mu*lam*fg+mu*(1.e0-lam)*(fs-mjwlq)+
     *         (1.e0-mu)*(fi-iheat))/
     *         (mu*lam*vg/wg+mu*(1.e0-lam)*vs/ws+
     *         (1.e0-mu)*vi/wi)
          else
            e=p*(mu*lam*vg/wg+mu*(1.e0-lam)*vs/ws+
     *         (1.e0-mu)*vi/wi)-(mu*lam*fg+mu*(1.e0-lam)*
     *         (fs-mjwlq)+(1.e0-mu)*(fi-iheat))
c            e=max(0.e0,e)
          end if
c
c.. First deal with case when pressure is realistically zero
c    Only allow this iteration to provide final result if iform < 0 (finding e)
          if( p.lt.pnear.and.(itr.lt.8.or.iform.lt.0) ) then
            if( 0.eq.1.and.1.e0-mu.lt.near ) then
c.. this case where mu~1 can likely be removed as it is handeled above
              dv(1)=mu*lam*vg+mu*(1.e0-lam)*vs-1.e0/rho
              dv(2)=cg*(fs+gs)-cs*(fg+gg)
c
              a(1,1)=mu*(1.e0-lam)
              a(1,2)=mu*lam
              a(2,1)=cg*(fsp+gsp)
              a(2,2)=-cs*(fgp+ggp)
              call dgesv( 2,1,a,3,ipvt,dv,2,info )
              if( info.ne.0 ) then
                write(6,*)'TROUBLE2!!'
              end if
c
c              norm=((1.e0-lam)*dv(1))**2+(lam*dv(2))**2
              norm=(dv(1))**2+(dv(2))**2
c
              vi=mvi0
              vs=vs-dv(1)
              vg=vg-dv(2)
              vs=max(vs,-1.e0)
              vg=max(vg,-1.e0)
              vs=min(vs,15.e0)
              vg=min(vg,15.e0)
c
            elseif( lam.lt.near ) then
              dv(1)=mu*vs+(1.e0-mu)*vi-1.e0/rho
              dv(2)=cs*(fi+gi)-ci*(fs+gs)
c
              a(1,1)=(1.e0-mu)
              a(1,2)=mu
              a(2,1)=cs*(fip+gip)
              a(2,2)=-ci*(fsp+gsp)
              call dgesv( 2,1,a,3,ipvt,dv,2,info )
              if( info.ne.0 ) then
                write(6,*)'TROUBLE3!!'
              end if
c
              norm=(dv(1))**2+(dv(2))**2
c              norm=0.e0
c              do i=1,2
c                norm=norm+dv(i)**2
c              end do
c
              vi=vi-dv(1)
              vs=vs-dv(2)
              vg=mvg0
              vi=max(vi,-1.e0)
              vs=max(vs,-1.e0)
              vi=min(vi,15.e0)
              vs=min(vs,15.e0)
            else
              dv(1)=mu*lam*vg+mu*(1.e0-lam)*vs+(1.e0-mu)*vi-1.e0/rho
              dv(2)=cs*(fi+gi)-ci*(fs+gs)
              dv(3)=cg*(fi+gi)-ci*(fg+gg)
c     
              a(1,1)=1.e0-mu
              a(1,2)=mu*(1.e0-lam)
              a(1,3)=mu*lam
c     
              a(2,1)=cs*(fip+gip)
              a(2,2)=-ci*(fsp+gsp)
              a(2,3)=0.e0
c     
              a(3,1)=cg*(fip+gip)
              a(3,2)=0.e0
              a(3,3)=-ci*(fgp+ggp)
c     
              call dgesv( 3,1,a,3,ipvt,dv,3,info )
              if( info.ne.0 ) then
                write(6,*)'TROUBLE1!!'
              end if
c
              norm=0.e0
              do i=1,3
                norm=norm+dv(i)**2
              end do
c
              vi=vi-dv(1)
              vs=vs-dv(2)
              vg=vg-dv(3)
              vi=max(vi,-1.e0)
              vs=max(vs,-1.e0)
              vg=max(vg,-1.e0)
              vi=min(vi,15.e0)
              vs=min(vs,15.e0)
              vg=min(vg,15.e0)
            end if
c
c
            if( failSafe.eq.1 ) then
              failSafeItr=failSafeItr+1
            else
              itr=itr+1
            end if
c
c.. now in the p>0 case            
          elseif( failSafe.eq.0 ) then
c.. non failSafe calculation
            ! set up the right hand side vector
            dv(1)=mu*lam*vg+mu*(1.e0-lam)*vs+(1.e0-mu)*vi-1.e0/rho
            if( iform.ge.0 ) then
              ! finding pressure
              phi=e+mu*lam*fg+mu*(1.e0-lam)*(fs-mjwlq)+
     *             (1.e0-mu)*(fi-iheat)
              psi=mu*lam*vg/wg+mu*(1.e0-lam)*vs/ws+(1.e0-mu)*vi/wi
              dv(2)=phi*(cs*vi/wi-ci*vs/ws)-
     *              psi*(cs*(fi+gi)-ci*(fs+gs))
              dv(3)=phi*(cg*vi/wi-ci*vg/wg)-
     *              psi*(cg*(fi+gi)-ci*(fg+gg))
            else
              ! finding e
              dv(2)=cs*(fi+gi)-ci*(fs+gs)-p*(vi*cs/wi-vs*ci/ws)
              dv(3)=cg*(fi+gi)-ci*(fg+gg)-p*(vi*cg/wi-vg*ci/wg)
            end if
c
            ! set up the jacobian matrix
            a(1,1)=1.e0-mu
            a(1,2)=mu*(1.e0-lam)
            a(1,3)=mu*lam
            if( iform.ge.0 ) then
              ! finding pressure
              a(2,1)=phi*(cs/wi)+(1.e0-mu)*fip*(cs*vi/wi-ci*vs/ws)-
     *             psi*cs*(fip+gip)-
     *             (1.e0-mu)/wi*(cs*(fi+gi)-ci*(fs+gs))
              a(2,2)=-phi*ci/ws+mu*(1.e0-lam)*fsp*(cs*vi/wi-ci*vs/ws)+
     *             psi*ci*(fsp+gsp)-
     *             mu*(1.e0-lam)/ws*(cs*(fi+gi)-ci*(fs+gs))
              a(2,3)=mu*lam*fgp*(cs*vi/wi-ci*vs/ws)-
     *             mu*lam/wg*(cs*(fi+gi)-ci*(fs+gs))
              a(3,1)=phi*cg/wi+(1.e0-mu)*fip*(cg*vi/wi-ci*vg/wg)-
     *             psi*cg*(fip+gip)-
     *             (1.e0-mu)/wi*(cg*(fi+gi)-ci*(fg+gg))
              a(3,2)=mu*(1.e0-lam)*fsp*(cg*vi/wi-ci*vg/wg)-
     *             mu*(1.e0-lam)/ws*(cg*(fi+gi)-ci*(fg+gg))
              a(3,3)=-phi*ci/wg+mu*lam*fgp*(cg*vi/wi-ci*vg/wg)+
     *             psi*ci*(fgp+ggp)-
     *             mu*lam/wg*(cg*(fi+gi)-ci*(fg+gg))
            else
              ! finding e
              a(2,1)=cs*(fip+gip)-p*cs/wi
              a(2,2)=-ci*(fsp+gsp)+ci*p/ws
              a(2,3)=0.e0
              a(3,1)=cg*(fip+gip)-p*cg/wi
              a(3,2)=0.e0
              a(3,3)=-ci*(fgp+ggp)+ci*p/wg
            end if
c
c..now we use LAPACK to solve this system
            call dgesv( 3,1,a,3,ipvt,dv,3,info )
            if( info.ne.0 ) then
              if( failSafeItr.gt.0.and.failSafeItr.lt.itmax ) then
c.. try again using failSafe iteration
                do i=1,3
                  dv(i)=0.e0
                end do
                vi=viSafe
                vs=vsSafe
                vg=vgSafe
                if( iform.gt.0 ) p=pSafe
                if( iform.lt.0 ) e=eSafe
              else
c.. iteration genuinely failed ... print message and stop
                write(6,*)
     *  'Error1 (geteosb.f) : error in  linear system for Newton itr'
                write(6,*)info,iform,lam,mu
                write(6,*)rho,p,e
                write(6,*)vi,vs,vg
                write(6,*)viold,vsold,vgold
                stop
              end if
            end if
c
c..add update
            vi=vi-dv(1)
            vs=vs-dv(2)
            vg=vg-dv(3)
            vg=max(vg,-1.e0)
c            vg=min(vg,10.e0)
            vi=min(vi,15.e0)
c-- jwb 27022006
            vi=max(vi,-1.e0)
c--
            norm=0.e0
            do i=1,3
              norm=norm+dv(i)**2
            end do
c            vi=max(-1.e0,vi-dv(1))
c            vs=max(-1.e0,vs-dv(2))
c            vg=max(-1.e0,vg-dv(3))
            itr=itr+1
          else
c..we are in what I call the fail safe mode
            if( 1.e0-mu.lt.near ) then
              ! all reactive fail Safe (can probably remove as this is handeled earlier)
c
              ! set up right hand side
              dv(1)=mu*lam*vg+mu*(1.e0-lam)*vs-1.e0/rho
              dv(2)=mu*lam*(p*vg/wg-fg)+
     *             mu*(1.e0-lam)*(cs/cg*(p*vg/wg-fg-gg)+gs+mjwlq)-e
              dv(3)=mu*lam*(p*vg/wg-fg)+
     *           mu*(1.e0-lam)*(p*vs/ws-fs+mjwlq)-e
c
              ! set up Jacobian matrix d/d(vs,vg,(e or p))
              a(1,1)=mu*(1.e0-lam)
              a(1,2)=mu*lam
              a(1,3)=0.e0
c            
              a(2,1)=mu*(1.e0-lam)*gsp
              a(2,2)=mu*lam*(p/wg-fgp)+
     *             mu*(1.e0-lam)*cs/cg*(p/wg-fgp-ggp)
c
              a(3,1)=mu*(1.e0-lam)*(p/ws-fsp)
              a(3,2)=mu*lam*(p/wg-fgp)

              if( iform.ge.0 ) then
                ! finding pressure
                a(2,3)=mu*lam*vg/wg+mu*(1.e0-lam)*cs*vg/(cg*wg)
                a(3,3)=mu*lam*vg/wg+mu*(1.e0-lam)*vs/ws+
     *             (1.e0-mu)*vi/wi
              else
                ! finding e
                a(2,3)=-1.e0
                a(3,3)=-1.e0
              end if
c
c..use LAPACK to solve system
              call dgesv( 3,1,a,3,ipvt,dv,3,info )
              if( info.ne.0 ) then
                write(6,*)
     * 'Error2 (geteosb.f) : error in  linear system for Newton itr'
                hittol=0
                failSafeItr=itmax
                itr=itmax
c                stop
              end if
c
c..add update
              vi=mvi0
              vs=vs-dv(1)
              vg=vg-dv(2)
              vg=max(vg,-1.e0)
              norm=0.e0
              do i=1,3
                norm=norm+dv(i)**2
              end do
c              vs=max(-1.e0,vs-dv(1))
c              vg=max(-1.e0,vg-dv(2))
c              vg=min(vg,10.e0)
              if( iform.ge.0 ) then
c                p=max(-1.e0,p-dv(3))
c                p=max(0.e0,p-dv(3))
                p=p-dv(3)
              else
                e=e-dv(3)
c                e=max(0.e0,e-dv(3))
              end if
            else
              ! general case fail Safe mode
              dv(1)=mu*lam*vg+mu*(1.e0-lam)*vs+
     *           (1.e0-mu)*vi-1.e0/rho
              dv(2)=mu*lam*(p*vg/wg-fg)+
     *             mu*(1.e0-lam)*(cs/cg*(p*vg/wg-fg-gg)+gs+mjwlq)+
     *             (1.e0-mu)*(ci/cg*(p*vg/wg-fg-gg)+gi+iheat)-e
              dv(3)=mu*lam*(cg/cs*(p*vs/ws-fs-gs-mjwlq)+gg)+
     *             mu*(1.e0-lam)*(p*vs/ws-fs)+
     *             (1.e0-mu)*(ci/cs*(p*vs/ws-fs-gs-mjwlq)+gi+iheat)-e
              dv(4)=mu*lam*(p*vg/wg-fg)+
     *           mu*(1.e0-lam)*(p*vs/ws-fs+mjwlq)+
     *           (1.e0-mu)*(p*vi/wi-fi+iheat)-e
c
c..set up Jacobian matrix
              agen(1,1)=1.e0-mu
              agen(1,2)=mu*(1.e0-lam)
              agen(1,3)=mu*lam
              agen(1,4)=0.e0
            
              agen(2,1)=(1.e0-mu)*gip
              agen(2,2)=mu*(1.e0-lam)*gsp
              agen(2,3)=mu*lam*(p/wg-fgp)+
     *             mu*(1.e0-lam)*cs/cg*(p/wg-fgp-ggp)+
     *             (1.-mu)*ci/cg*(p/wg-fgp-ggp)
c
              agen(3,1)=(1.e0-mu)*gip
              agen(3,2)=mu*lam*cg/cs*(p/ws-fsp-gsp)+
     *             mu*(1.e0-lam)*(p/ws-fsp)+
     *             (1.e0-mu)*ci/cs*(p/ws-fsp-gsp)
              agen(3,3)=mu*lam*ggp
c
              agen(4,1)=(1.e0-mu)*(p/wi-fip)
              agen(4,2)=mu*(1.e0-lam)*(p/ws-fsp)
              agen(4,3)=mu*lam*(p/wg-fgp)

              if( iform.ge.0 ) then
                ! finding pressure
                agen(2,4)=mu*lam*vg/wg+
     *               mu*(1.e0-lam)*cs*vg/(cg*wg)+
     *               (1.e0-mu)*ci*vg/(cg*wg)
                agen(3,4)=mu*lam*cg*vs/(cs*ws)+
     *               mu*(1.e0-lam)*vs/ws+
     *               (1.e0-mu)*ci*vs/(cs*ws)
                agen(4,4)=mu*lam*vg/wg+
     *               mu*(1.e0-lam)*vs/ws+
     *               (1.e0-mu)*vi/wi
              else
                ! finding e
                agen(2,4)=-1.e0
                agen(3,4)=-1.e0
                agen(4,4)=-1.e0
              end if
c
c..use LAPACK to solve system
              call dgesv( 4,1,agen,4,ipvt,dv,4,info )
              if( info.ne.0 ) then
                write(6,*)
     * 'Error3 (geteosb.f) : error in  linear system for Newton itr'
                write(6,*)'info=',info,mu,lam,p
                write(6,*)rho,e
                write(6,*)vi,vs,vg
                stop
              end if
c
c..add update
              norm=0.e0
              do i=1,4
                norm=norm+dv(i)**2
              end do
              vi=vi-dv(1)
              vs=vs-dv(2)
              vg=vg-dv(3)
              vi=max(vi,-1.e0)
              vs=max(vs,-1.e0)
              vg=max(vg,-1.e0)
              vi=min(vi,15.e0)
              vs=min(vs,15.e0)
              vg=min(vg,15.e0)
              if( iform.ge.0 ) then
                p=p-dv(4)
              else
c                e=e-dv(3)
                e=e-dv(4)
              end if
            end if
            failSafeItr=failSafeItr+1
          end if
c
c..find size of correction
          if( norm.lt.tol ) then
            if( failSafe.eq.1 ) then
              ! do regular iteration now
              viSafe=vi
              vsSafe=vs
              vgSafe=vg
              if( iform.ge.0 ) pSafe=p
              if( iform.lt.0 ) eSafe=e
              failSafe=0
c              hittol=1
c              itr=itmax
            else
              ! within tolerance and not in failSafe mode so we quit
              hittol=1
              itr=itmax
            end if
          else
            if( failSafe.eq.1 ) then
              if( failSafeItr.ge.itmax ) then
c.. done using failSafe ... move on to regular iteration
                failSafe=0
              endif
            else
              if( norm.gt.MAXJUMP.and.failSafeItr.eq.0 ) then
                ! try again with failSafe iteration if we make a HUGE jump
                itr=0
                failSafe=1
                if( iform.ge.0 ) then
                  p=pold
                else
                  e=eold
                end if
                vi=viold
                vs=vsold
                vg=vgold
              end if
              if( itr.ge.itmax.and.failSafeItr.eq.0 ) then
                ! try again with failSafe iteration
                itr=0
                failSafe=1
                if( iform.ge.0 ) then
                  p=pold
                else
                  e=eold
                end if
                vi=viold
                vs=vsold
                vg=vgold
              end if
            end if
          end if
c.. done with while loop
        end do
c
c..if we missed the tolerance then display warning if applicable
        if( hittol.ne.1 ) then
          if( failSafeItr.gt.0.and.failsafeItr.lt.itmax ) then
c.. believe the failSafe result instead of the other
            vi=viSafe
            vs=vsSafe
            vg=vgSafe
c            if( iform.gt.0 ) p=pSafe
c            if( iform.lt.0 ) e=eSafe
c          elseif( iform.ge.0.and.p.lt.pnear ) then
c            ! believe this result for now and continue
cc            p=0.e0
c            ier=0
c          elseif( rho.lt.1.e0/mvg0 ) then
          elseif( 0.eq.1 ) then
c          elseif( rho.le.rhoMin ) then
            write(6,*)'Special 1'
            vs=1.e0/rho
            vg=vs
            vi=vg
c            if( iform.lt.0 ) then
c              e=mu*lam*(p*vg/wg)
c     *           +mu*(1.e0-lam)*(p*vs/ws+mjwlq)
c     *           +(1.e0-mu)*(p*vi/wi+iheat)
c            else
c              t1=e-(1.e0-mu)*iheat-mu*(1.e0-lam)*mjwlq
c              t2=mu*vg/wg+mu*(1.e0-lam)*vs/ws+(1.e0-mu)*vi/wi
c              p=t1/t2
c            end if
            ier=0
c          elseif( (1.e0-lam.lt.1e-10.and.1.e0-mu.lt.0.1e0).or.
c     *            vg.gt.vgMax.or.vg.lt.0.e0 ) then
          elseif( 0.eq.1 ) then
            write(6,*)'Special 2'
            vi=1.e0/rho
            vs=1.e0/rho
            vg=1.e0/rho
            ier=0
c            write(6,*)'Here', rho,e,p,mu,lam,iform
c            write(6,*)'  --',norm,dv(1),dv(2),dv(3),dv(4),
c     *         failSafe,failSafeItr
c            return
          elseif( 1.e0-mu.lt.near ) then
c.. use Don's code for reactive JWL gases!! Mixture between solid and gas
c     Be sure to have common block set up properly
            rho_e=rho*e
            rho_lam=rho*lam
            if( lam.gt.near.and.iform.lt.0.and.p.lt.pnear ) then
              p=pnear
            end if
            call geteos( rho,rho_e,rho_lam,vs,vg,p,dp,iform,ier )
            vi=mvi0
            if( iform.gt.0 ) then
              dp(4)=dp(3)
              dp(3)=0.e0
            end if
            if( iform.lt.0 ) e=rho_e/rho
            ier=0
            return
          elseif( ier.eq.0 ) then
            write(6,*)'Did not get to tolerance ... be careful!!'
            write(6,*)'v=',vi,vs,vg
            write(6,*)'v0=',viold,vsold,vgold
            if( iform.ge.0 ) then
              write(6,*)'re',rho,e,p
              write(6,*)'ml=',mu,lam
            else
              write(6,*)'rp',rho,p,e
              write(6,*)'ml=',mu,lam
            end if
            write(6,*)'  norm(dv)',norm,itr,failSafeItr
            write(6,*)'failSafe=',failSafe
            ier=0
          end if
        else
          ier=0
        endif
      end if
c
      vi=max(1.e-1,vi)
      vs=max(1.e-1,vs)
      vg=max(1.e-1,vg)
c
c.. calculate just e or p
      if( iform.le.0 ) then

        fi=0.e0
        fs=0.e0
        fg=0.e0
c     
        do i=1,2
          t1=dexp(-ri(i)*vi)
          t2=ai(i)*(vi/wi-1.e0/ri(i))
          fi=fi+t1*t2
c     
          t1=dexp(-rmjwl(i,1)*vs)
          t2=amjwl(i,1)*(vs/ws-1.e0/rmjwl(i,1))
          fs=fs+t1*t2
c     
          t1=dexp(-rmjwl(i,2)*vg)
          t2=amjwl(i,2)*(vg/wg-1.e0/rmjwl(i,2))
          fg=fg+t1*t2
        end do
        fs=fs-fs0
        fg=fg-fg0
        fi=fi-fi0
c
c.. compute e or p
        if( iform.ge.0 ) then
          ! find p
          p=(e+mu*lam*fg+mu*(1.e0-lam)*(fs-mjwlq)+
     *       (1.e0-mu)*(fi-iheat))/
     *       (mu*lam*vg/wg+mu*(1.e0-lam)*vs/ws+
     *       (1.e0-mu)*vi/wi)
        else
          e=p*(mu*lam*vg/wg+mu*(1.e0-lam)*vs/ws+
     *       (1.e0-mu)*vi/wi)-(mu*lam*fg+mu*(1.e0-lam)*
     *       (fs-mjwlq)+(1.e0-mu)*(fi-iheat))
c          e=max(0.e0,e)
        end if
      else
c
c..calculate p and derivatives
        fi=0.e0
        fs=0.e0
        fg=0.e0
c     
        fsp=0.e0
        fgp=0.e0
        fip=0.e0
c     
        gs=0.e0
        gg=0.e0
        gi=0.e0
c     
        gsp=0.e0
        ggp=0.e0
        gip=0.e0
        do i=1,2
          t1=dexp(-ri(i)*vi)
          t2=ai(i)*(vi/wi-1.e0/ri(i))
          fi=fi+t1*t2
          fip=fip+t1*(ai(i)/wi-t2*ri(i))
          gi=gi+ai(i)/ri(i)*t1
          gip=gip-ai(i)*t1
c     
          t1=dexp(-rmjwl(i,1)*vs)
          t2=amjwl(i,1)*(vs/ws-1.e0/rmjwl(i,1))
          fs=fs+t1*t2
          fsp=fsp+t1*(amjwl(i,1)/ws-t2*rmjwl(i,1))
          gs=gs+amjwl(i,1)/rmjwl(i,1)*t1
          gsp=gsp-amjwl(i,1)*t1
c     
          t1=dexp(-rmjwl(i,2)*vg)
          t2=amjwl(i,2)*(vg/wg-1.e0/rmjwl(i,2))
          fg=fg+t1*t2
          fgp=fgp+t1*(amjwl(i,2)/wg-t2*rmjwl(i,2))
          gg=gg+amjwl(i,2)/rmjwl(i,2)*t1
          ggp=ggp-amjwl(i,2)*t1
        end do
        fs=fs-fs0
        fg=fg-fg0
        fi=fi-fi0
c     
        gs=gs-gs0
        gg=gg-gg0
        gi=gi-gi0
c
        ! find p
        p=(e+mu*lam*fg+mu*(1.e0-lam)*(fs-mjwlq)+
     *     (1.e0-mu)*(fi-iheat))/
     *     (mu*lam*vg/wg+mu*(1.e0-lam)*vs/ws+
     *     (1.e0-mu)*vi/wi)
c
c..set up matrix and rhs to find derivaties of specific volumes w/r.t. rho,rho*e,rho*mu,rho*lam
        t1=rho**2
        phi=(e+mu*lam*fg+mu*(1.e0-lam)*(fs-mjwlq)+
     *     (1.e0-mu)*(fi-iheat))*t1
        psi=(mu*lam*vg/wg+mu*(1.e0-lam)*vs/ws+(1.e0-mu)*vi/wi)*t1
        a(1,1)=t1*(1.e0-mu)
        a(1,2)=t1*mu*(1.e0-lam)
        a(1,3)=t1*mu*lam
        b(1,1)=-rho*mu*vs-rho*(2.e0-mu)*vi+1.e0
        b(1,2)=0.e0
        b(1,3)=-rho*(lam*vg+(1.e0-lam)*vs-vi)
        b(1,4)=rho*mu*(-vg+vs)
c     
        t2=cs*vi/wi-ci*vs/ws
        t3=cs*(fi+gi)-ci*(fs+gs)
        a(2,1)=cs*phi/wi+t2*t1*(1.e0-mu)*fip-
     *     cs*psi*(fip+gip)-t3*t1*(1.e0-mu)/wi
        a(2,2)=-ci*phi/ws+t2*t1*mu*(1.e0-lam)*fsp+
     *     ci*psi*(fsp+gsp)-t3*t1*mu*(1.e0-lam)/ws
        a(2,3)=t2*t1*mu*lam*fgp-t3*t1*mu*lam/wg
        b(2,1)=-t2*rho*(e+mu*(fs-mjwlq)+(2.e0-mu)*(fi-iheat))+
     *     t3*rho*(mu*vs/ws+(2.e0-mu)*vi/wi)
        b(2,2)=-t2*rho
        b(2,3)=-t2*rho*(lam*fg+(1.e0-lam)*(fs-mjwlq)-(fi-iheat))+
     *     t3*rho*(lam*vg/wg+(1.e0-lam)*vs/ws-vi/wi)
        b(2,4)=-t2*rho*(mu*fg-mu*(fs-mjwlq))+
     *     t3*rho*(mu*vg/wg-mu*vs/ws)
c     
        t2=cg*vi/wi-ci*vg/wg
        t3=cg*(fi+gi)-ci*(fg+gg)
        a(3,1)=cg*phi/wi+t2*t1*(1.e0-mu)*fip-
     *     cg*psi*(fip+gip)-t3*t1*(1.e0-mu)/wi
        a(3,2)=t2*t1*mu*(1.e0-lam)*fsp-t3*t1*mu*(1.e0-lam)/ws
        a(3,3)=-ci*phi/wg+t2*t1*mu*lam*fgp+ci*psi*(fgp+ggp)-
     *     t3*t1*mu*lam/wg
        b(3,1)=-t2*rho*(e+mu*(fs-mjwlq)+(2.e0-mu)*(fi-iheat))+
     *     t3*rho*(mu*vs/ws+(2.e0-mu)*vi/wi)
        b(3,2)=-t2*rho
        b(3,3)=-t2*rho*(lam*fg+(1.e0-lam)*(fs-mjwlq)-(fi-iheat))+
     *     t3*rho*(lam*vg/wg+(1.e0-lam)*vs/ws-vi/wi)
        b(3,4)=-t2*rho*(mu*fg-mu*(fs-mjwlq))+
     *     t3*rho*(mu*vg/wg-mu*vs/ws)
c     
c..solve system with LAPACK
        call dgesv( 3,4,a,3,ipvt,b,3,info )
        if( info.ne.0 ) then
          write(6,*)'Error (geteosb) : problem finding derivatives'
          write(6,*)rho,e,p,mu,lam,vi,vs,vg
          stop
        end if
c
c..we can now get dp's. First w/r.t. rho
        t2=rho*(e+mu*(fs-mjwlq)+(2.e0-mu)*(fi-iheat))+
     *     t1*(mu*lam*fgp*b(3,1)+mu*(1.e0-lam)*fsp*b(2,1)+
     *     (1.e0-mu)*fip*b(1,1))
        t3=rho*(mu*vs/ws+(2.e0-mu)*vi/wi)+
     *     t1*(mu*lam*b(3,1)/wg+mu*(1.e0-lam)*b(2,1)/ws+
     *     (1.e0-mu)*b(1,1)/wi)
        dp(1)=(t2*psi-phi*t3)/(psi**2)
c
c..w/r.t. rho*e
        t2=rho+t1*(mu*lam*fgp*b(3,2)+mu*(1.e0-lam)*fsp*b(2,2)+
     *     (1.e0-mu)*fip*b(1,2))
        t3=t1*(mu*lam*b(3,2)/wg+mu*(1.e0-lam)*b(2,2)/ws+
     *     (1.e0-mu)*b(1,2)/wi)
        dp(2)=(t2*psi-phi*t3)/(psi**2)
c
c..w/r.t. rho*mu
        t2=rho*(lam*fg+(1.e0-lam)*(fs-mjwlq)-(fi-iheat))+
     *     t1*(mu*lam*fgp*b(3,3)+mu*(1.e0-lam)*fsp*b(2,3)+
     *     (1.e0-mu)*fip*b(1,3))
        t3=rho*(lam*vg/wg+(1.e0-lam)*vs/ws-vi/wi)+
     *     t1*(mu*lam*b(3,3)/wg+mu*(1.e0-lam)*b(2,3)/ws+
     *     (1.e0-mu)*b(1,3)/wi)
        dp(3)=(t2*psi-phi*t3)/(psi**2)
c
c..w/r.t. rho*lam
        t2=rho*(mu*fg-mu*(fs-mjwlq))+
     *     t1*(mu*lam*fgp*b(3,4)+mu*(1.e0-lam)*fsp*b(2,4)+
     *     (1.e0-mu)*fip*b(1,4))
        t3=rho*(mu*vg/wg-mu*vs/ws)+
     *     t1*(mu*lam*b(3,4)/wg+mu*(1.e0-lam)*b(2,4)/ws+
     *     (1.e0-mu)*b(1,4)/wi)
        dp(4)=(t2*psi-phi*t3)/(psi**2)
      end if
c
      return
      end
