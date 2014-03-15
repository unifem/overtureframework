      subroutine geteosc (rho,e,mu,lam,vi,vs,vg,p,dp,iform,ier)
c
c Evaluate the equation of state p=p(rho,rho*e,rho*mu,rho*lambda) for the
c mixture JWL EOS and a second inert gas. This algorithm derives from the
c assumption p=mu*p_r+(1-mu)*p_i, where p_r and p_i are computed independently.
c We then assume v=v_i=lambda*v_g+(1-lambda)*v_s.
c See my notes around Feb 7, 2006.
c
c Variables: r=density, rho
c            e=internal energy per unit mass, E/rho-|velocity|^2/2
c            mu=species mass fraction
c            lam=progress mass fraction
c            vs=specific volume of solid explosive
c            vg=specific volume of gas products
c            p=pressure
c            dp=derivative of pressure w/r.t. (rho,e,rho*mu,rho*lam)
c            iform=integer flag (see below)
c            ier=error return flag
c
c if iform=0, then e (energy) is given and p (pressure) is returned
c         >0, then e is given and p and its derivatives dp are returned
c         <0, then p is given and e is returned
c
c ier passed on to geteos as input
c ier as output:
c   ier=0 (no problems)
c   ier<1 (problems)
c
      implicit none
      integer iform,ier
      real rho,e,mu,lam,vi,vs,vg,p
      include 'tempSizes.h'
      real dp(dpSize)
c
c local variables
      real dpr(dpSize), dpi(dpSize)
      real near
      real pr,pi
      real wi,ws,wg
      real fi,fs,fg,gs,gg
      real fip,fsp,fgp,gsp,ggp
      real t1,t2
      real rho_e, rho_lam
c
c stuff for Newton iteration
      real f,fp,dv,v,tol,emin,pmin
      real f1,f2,f3,f4
      real f1p,f2p,f3p,f4p
      integer i,j,ier2,iform2,itr,itmax
      parameter( itmax=15 )
      include 'multijwl.h'
c
      data near,tol,emin,pmin / 1.0d-3,1.0d-10,1.0d-3,0.0d0 /
c
      wi=gm1s(1)
      ws=gm1s(2)
      wg=gm1s(3)
      e=max(e,emin)
      p=max(p,pmin)
c
      if( mu.lt.1d-10 ) then
c      if( mu.lt.near ) then
c      if( 0.eq.1.and.mu.lt.near ) then
cc      if( mu.le.0.d0 ) then
c..Inert case
        vi=1.d0/rho
        vs=mvs0
        vg=mvg0
        fi=0.d0
        fip=0.d0
        do i=1,2
          t1=dexp(-ri(i)*vi)
          t2=ai(i)*(vi/wi-1.d0/ri(i))
          fi=fi+t1*t2
          fip=fip+t1*(ai(i)/wi-t2*ri(i))
        end do
        fi=fi-fi0
        if( iform.ge.0 ) then
c..return p from e
          p=max(0.d0,rho*(e+fi-iheat)*wi)
          if( iform.gt.0 ) then
c..return derivatives too
            dp(1)=wi*(-fip/rho+fi-iheat)
            dp(2)=wi
            dp(3)=0.d0
            dp(4)=0.d0
          end if
        else
c..return e from p
          e=p/(rho*wi)-fi+iheat
        end if
        ier=0
        return
      elseif( 1.d0-mu.lt.1.d-10 ) then
c      elseif( 1.d0-mu.lt.near ) then
c      elseif( 0.eq.1.and.1.d0-mu.le.0.d0 ) then
c      elseif( 1.d0-mu.le.0.d0 ) then
c..pure reactive material
        vi=1.d0/rho
        ier2=ier
        iform2=iform
c        call geteosb( rho,e,1.d0,lam,vi,vs,vg,p,dp,iform2,ier2 )
c        e=max(e,emin)
c        p=max(p,pmin)
        if( 1.d0-lam.lt.near ) vs=0.d0
        rho_e=rho*e
        rho_lam=rho*lam
        call geteos( rho,rho_e,rho_lam,vs,vg,p,dp,iform2,ier2 )
        e=max(emin,rho_e/rho)
        p=max(p,pmin)
        if( iform.gt.0 ) then
          dp(4)=dp(3)
          dp(3)=0.d0
        end if
        if( 0.eq.1.and.ier2.ne.0 ) then
          write(6,*)'Error1 (geteosc): in calling geteos'
          write(6,*)rho,mu,lam,e,p,iform
        end if
        ier=0
        return
      end if
c
      if( iform.ge.0 ) then
c
c..getting p and possibly derivatives from e
c
c..get pi
        vi=1.d0/rho
        fi=0.d0
        fip=0.d0
        do i=1,2
          t1=dexp(-ri(i)*vi)
          t2=ai(i)*(vi/wi-1.d0/ri(i))
          fi=fi+t1*t2
          fip=fip+t1*(ai(i)/wi-t2*ri(i))
        end do
        fi=fi-fi0
        pi=max(0.d0,rho*(e+fi-iheat)*wi)
        if( iform.gt.0 ) then
c..return derivatives too
          dpi(1)=wi*(-fip/rho+fi-iheat)
          dpi(2)=wi
          dpi(3)=0.d0
          dpi(4)=0.d0
        end if
c
c..get pr
c        if( lam.lt.near ) then
c        if( lam.lt.1d-10 ) then
        if( lam.le.0.d0 ) then
c..unburnt
          vs=1.d0/rho
          vg=mvg0
          fs=0.d0
          fsp=0.d0
          do i=1,2
            t1=dexp(-rmjwl(i,1)*vs)
            t2=amjwl(i,1)*(vs/ws-1.d0/rmjwl(i,1))
            fs=fs+t1*t2
            fsp=fsp+t1*(amjwl(i,1)/ws-t2*rmjwl(i,1))
          end do
          fs=fs-fs0
c..return p from e
          pr=max(0.d0,rho*(e+fs-mjwlq)*ws)
          if( iform.gt.0 ) then
c..return derivatives too
            dpr(1)=ws*(-fsp/rho+fs-mjwlq)
            dpr(2)=ws
            dpr(3)=0.d0
            dpr(4)=0.d0
          end if
        elseif( 1.d0-lam.lt.near ) then
c..burnt
c          vs=mvs0
          vs=0.d0
          vg=1.d0/rho
          fg=0.d0
          fgp=0.d0
          do i=1,2
            t1=dexp(-rmjwl(i,2)*vg)
            t2=amjwl(i,2)*(vg/wg-1.d0/rmjwl(i,2))
            fg=fg+t1*t2
            fgp=fgp+t1*(amjwl(i,2)/wg-t2*rmjwl(i,2))
          end do
          fg=fg-fg0
          pr=max(0.d0,rho*(e+fg)*wg)
          if( iform.gt.0 ) then
c..return derivatives too
            dpr(1)=wg*(-fgp/rho+fg)
            dpr(2)=wg
            dpr(3)=0.d0
            dpr(4)=0.d0
          end if
        else
          ier2=0
          iform2=iform
          rho_e=rho*e
          rho_lam=rho*lam
c          if( 1.d0-lam.lt.near ) vs=0.d0
          call geteos( rho,rho_e,rho_lam,vs,vg,pr,dpr,iform2,ier2 )
c          pr=max(pmin,pr)
          dpr(4)=dpr(3)
          dpr(3)=0.d0
c          call geteosb( rho,e,1.0d0,lam,vi,vs,vg,pr,dpr,iform2,ier2 )
          if( ier2.ne.0 ) then
            write(6,*)'Error2 (geteosc): in calling geteos'
          end if
        end if
c
c..calculate p
c        p=mu*pr+(1.d0-mu)*pi
        p=max(pmin,mu*pr+(1.d0-mu)*pi)
        if( iform.gt.0 ) then
          dp(1)=-mu/rho*(pr-pi)+mu*dpr(1)+(1.d0-mu)*dpi(1)
          dp(2)=mu*dpr(2)+(1.d0-mu)*dpi(2)
          dp(3)=1.d0/rho*(pr-pi)
          dp(4)=mu*dpr(4)
        end if
      else
c
c..getting e from p
c
        v=1.d0/rho
        vi=v
        fi=0.d0
        do i=1,2
          t1=dexp(-ri(i)*vi)
          t2=ai(i)*(vi/wi-1.d0/ri(i))
          fi=fi+t1*t2
        end do
        fi=fi-fi0-iheat
c..special cases for small and large lambda
c        if( lam.lt.near ) then
c        if( lam.lt.1.0d-10 ) then
        if( lam.le.0.d0 ) then
          vs=1.d0/rho
          vg=mvg0
        elseif( 1.d0-lam.lt.near ) then
          vg=1.d0/rho
c          vs=0.d0
          vs=mvs0
        else
c..perform Newton iteration to find vs,vg
          itr=0
          dv=1.d0
c         write(6,*)'--',rho,mu,lam,p,vs,vg
c
          if( lam.lt.0.5d0 ) then
c
c..iterate to find vg
            vs=(v-lam*vg)/(1.d0-lam)
c     
            do while( itr.lt.itmax.and.dabs(dv).gt.tol )
c
c..set f's, and g's
              fs=0.d0
              fg=0.d0
c     
              fsp=0.d0
              fgp=0.d0
c     
              gs=0.d0
              gg=0.d0
c     
              gsp=0.d0
              ggp=0.d0
              do i=1,2
                t1=dexp(-rmjwl(i,1)*vs)
                t2=amjwl(i,1)*(vs/ws-1.d0/rmjwl(i,1))
                fs=fs+t1*t2
                fsp=fsp+t1*(amjwl(i,1)/ws-t2*rmjwl(i,1))
                gs=gs+amjwl(i,1)/rmjwl(i,1)*t1
                gsp=gsp-amjwl(i,1)*t1
c     
                t1=dexp(-rmjwl(i,2)*vg)
                t2=amjwl(i,2)*(vg/wg-1.d0/rmjwl(i,2))
                fg=fg+t1*t2
                fgp=fgp+t1*(amjwl(i,2)/wg-t2*rmjwl(i,2))
                gg=gg+amjwl(i,2)/rmjwl(i,2)*t1
                ggp=ggp-amjwl(i,2)*t1
              end do
              fs=fs-fs0-mjwlq
              fg=fg-fg0
              gs=gs-gs0+mjwlq
              gg=gg-gg0
c     
c..set each portion of f
              f1 = p+(1.d0-mu)*rho*wi*(lam*fg+(1.d0-lam)*fs-fi)
              f1p= (1.d0-mu)*rho*wi*lam*(fgp-fsp)
              f2 = mu+(1.d0-mu)*rho*wi*(lam*vg/wg+(v-lam*vg)/ws)
              f2p= (1.d0-mu)*rho*wi*lam*(1.d0/wg-1.d0/ws)
              f3 = fs+gs-cs/cg*(fg+gg)
              f3p= -lam/(1.d0-lam)*(fsp+gsp)-cs/cg*(fgp+ggp)
              f4 = (v-lam*vg)/(ws*(1.d0-lam))-cs*vg/(cg*wg)
              f4p= -lam/(ws*(1.d0-lam))-cs/(cg*wg)
c     
              f=f1/f2-f3/f4
              fp=(f1p*f2-f1*f2p)/(f2**2)-(f3p*f4-f3*f4p)/(f4**2)
c     
c..update vg
              dv=f/fp
c            write(6,*)f1,f2,f3,f4
c            write(6,*)f1p,f2p,f3p,f4p
c            write(6,*)f,fp,dv
c            stop
cc            write(6,*)'  ',itr,dv,vs,vg
              vg=vg-dv
              vs=(v-lam*vg)/(1.d0-lam)
c              write(6,*)'--',dv
c     
              itr=itr+1
            end do
c            write(6,*)
          else                  ! lam.ge.0.5d0
c
c..iterate to find vs
            vg=(v-(1.d0-lam)*vs)/lam
c
            do while( itr.lt.itmax.and.dabs(dv).gt.tol )
c
c..set f's, and g's
              fs=0.d0
              fg=0.d0
c     
              fsp=0.d0
              fgp=0.d0
c     
              gs=0.d0
              gg=0.d0
c     
              gsp=0.d0
              ggp=0.d0
              do i=1,2
                t1=dexp(-rmjwl(i,1)*vs)
                t2=amjwl(i,1)*(vs/ws-1.d0/rmjwl(i,1))
                fs=fs+t1*t2
                fsp=fsp+t1*(amjwl(i,1)/ws-t2*rmjwl(i,1))
                gs=gs+amjwl(i,1)/rmjwl(i,1)*t1
                gsp=gsp-amjwl(i,1)*t1
c     
                t1=dexp(-rmjwl(i,2)*vg)
                t2=amjwl(i,2)*(vg/wg-1.d0/rmjwl(i,2))
                fg=fg+t1*t2
                fgp=fgp+t1*(amjwl(i,2)/wg-t2*rmjwl(i,2))
                gg=gg+amjwl(i,2)/rmjwl(i,2)*t1
                ggp=ggp-amjwl(i,2)*t1
              end do
              fs=fs-fs0-mjwlq
              fg=fg-fg0
              gs=gs-gs0+mjwlq
              gg=gg-gg0
c
c..set each portion of f
              f1 = p+(1.d0-mu)*rho*wi*(lam*fg+(1.d0-lam)*fs-fi)
              f1p= -(1.d0-mu)*rho*wi*(1.d0-lam)*(fgp-fsp)
              f2 = mu+(1.d0-mu)*rho*wi*((v-(1.d0-lam)*vs)/wg
     *             +(1.d0-lam)*vs/ws)
              f2p= -(1.d0-mu)*rho*wi*(1.d0-lam)*(1.d0/wg-1.d0/ws)
              f3 = fs+gs-cs/cg*(fg+gg)
              f3p= fsp+gsp+cs*(1.d0-lam)/(cg*lam)*(fgp+ggp)
              f4 = vs/ws-cs*(v-(1.d0-lam)*vs)/(cg*wg*lam)
              f4p= 1.d0/ws+cs*(1.d0-lam)/(cg*wg*lam)
c     
              f=f1/f2-f3/f4
              fp=(f1p*f2-f1*f2p)/(f2**2)-(f3p*f4-f3*f4p)/(f4**2)
c..update vs
              dv=f/fp
c            write(6,*)'=================='
c            write(6,*)f1,f2,f3,f4
c            write(6,*)f1p,f2p,f3p,f4p
c            write(6,*)f,fp,dv
c            stop
cc            write(6,*)'  ',itr,dv,vs,vg
              vs=vs-dv
              vg=(v-(1.d0-lam)*vs)/lam
c              write(6,*)'++',dv
c
              itr=itr+1
            end do
c            write(6,*)
          end if                ! lam.lt.0.5d0
c        write(6,*)'*****',dv,itr,vs,vg,lam
        end if                  ! lam.lt.near
c
c..get pr from vs,vg
        vs=max(0.5d0,vs)
        vg=max(0.5d0,vg)
c        vs=min(mvs0,vs)
c        vg=min(mvg0,vg)
        vs=min(15.d0,vs)
        vg=min(15.d0,vg)
c
        fs=0.d0
        fg=0.d0
        fi=0.d0
c     
        do i=1,2
          t1=dexp(-ri(i)*vi)
          t2=ai(i)*(vi/wi-1.d0/ri(i))
          fi=fi+t1*t2
c     
          t1=dexp(-rmjwl(i,1)*vs)
          t2=amjwl(i,1)*(vs/ws-1.d0/rmjwl(i,1))
          fs=fs+t1*t2
c     
          t1=dexp(-rmjwl(i,2)*vg)
          t2=amjwl(i,2)*(vg/wg-1.d0/rmjwl(i,2))
          fg=fg+t1*t2
        end do
        fi=fi-fi0-iheat
        fs=fs-fs0-mjwlq
        fg=fg-fg0
c
        t1=p+(1.d0-mu)*rho*wi*(lam*fg+(1.d0-lam)*fs-fi)
        t2=(1.d0-mu)*rho*wi*(lam*vg/wg+(1.d0-lam)*vs/ws)+mu
        pr=t1/t2
c
        if( mu.lt.0.5d0 ) then
c
c..get pi from pr
          pi=(p-mu*pr)/(1.d0-mu)
c
c..get e from fi and pi
          e=pi/(rho*wi)-fi
        else 
          ! mu>=0.5
c
c..get e
          e=lam*(pr*vg/wg-fg)+(1.d0-lam)*(pr*vs/ws-fs)
        end if                  ! mu.lt.0.5d0
        if( 0.eq.1.and.e.gt.1.d0 ) then
          write(6,*)'&&',e,pi,pr
          write(6,*)rho,mu,lam
          write(6,*)vs,vg,p
          stop
        end if
        if( 0.eq.1.and.e.lt.emin ) then
          write(6,*)'^^',e,pi,pr
          write(6,*)rho,mu,lam
          write(6,*)vs,vg,p
c          stop
        end if
        e=max(emin,e)
      end if                    ! iform.ge.0
c
      ier=0
      return
      end
