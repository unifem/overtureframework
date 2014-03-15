      subroutine geteosburnt (r,e,y,vs,vg,p,dp,iform,ier)
c
c   Routine called during multicoponent computation when only 2 burnt materials are present.
c   Same file as geteos.f except the common block definition is for materials where lam=1
c   and the subroutine names have been changed to avoid conflict.
c
      implicit real*8 (a-h,o-z)
      include 'tempSizes.h'
      dimension dp(dpSize),a(3,5)
c
      include 'eosDefine.h'  ! define the EOS names, idealGasEOS, jwlEOS, mieGruneisenEOS
c
      include 'eosburntdat.h'
c
      common / faildat / ferr, ifail
c
      data alam0, tol, eps / .5d0, 1.d-8, 1.d-12 /
c     data tiny, emin, pmin / 1.d-3, 1.d-2, 1.d-2 /
      data tiny, emin, pmin, rmin / 1.d-3, -1.d10, 0.d0, 1.d-3 /
c
c     include 'LX17puck.h'
c     include 'LX17caseII.h'
c     include 'LX17caseI.h'
c     include 'PBX9502.h'
c
      ferr=0.d0
      ifail=0

c wdh: ieos is not known here -- should this be fixed?
c      if( ieos.ne.jwlEOS )then
c        write(*,'("geteos:ERROR unexpected ieos=",i4)') ieos
c        stop 8865
c      end if

c
c set ilow=0 if a low-density guess for vs and vg is to be used for
c the case when the Newton iteration fails using the input values for
c vs and vg.  (If the low-density guess fails too, then subroutines
c fail1 or fail2 are called.)
c
c The low-density guess is the solution of 1=x*vs+y*vg and the
c temperature equilibrium equation with fs=fg=zs=zg=0, i.e.
c t1=cgcs*vs/omeg(1)-vg/omeg(2)=0.
      ilow=0
c
      vmin=1.d-1
      itmax=15
      dtemp=0.d0
c
c limit the density
      r1=max(r,rmin)
c
c limit y=rho*lambda
      y1=min(max(y,0.d0),r1)
c
c compute x=(1-lambda)*rho and lambda
      x1=r1-y1
      alam=y1/r1
c
c if iform<0, then geteos is being called from seteos and is used
c to update vs and vg for the ghost points at physical boundaries.
c Input values of vs and vg are extrapolated and these values may
c be way off, vg in particular.  If lambda is close to zero, fix
c up vs and vg.
      if (iform.lt.0.and.alam.lt.5.d-3) then
        vs=1.d0/r1
        vg=vg0
      end if
c
      vshold=vs
      vghold=vg
    1 if (ier.gt.0) then
        write(6,*)'ier =',ier
        vs=vshold
        vg=vghold
        write(6,*)'Error (geteosburnt) : Newton did not converge'
        write(6,*)'  (see file eos.out for information)'
        open (17,file='eos.out',status='UNKNOWN')
        if (iform.ge.0) then
          write(17,101)r,e,y,vs,vg
  101     format('** geteosburnt **',/,'  r,e,y,vs,vg =',5(1pe15.8))
        else
          write(17,111)r,p,y,vs,vg
  111     format('** geteosburnt **',/,'  r,p,y,vs,vg =',5(1pe15.8))
        end if
      end if
c
      if (iform.ge.0) then
c
        e1=max(e,emin)
c
c..rho, rho*e, rho*lambda given.  Update vs and vg and return p and possibly dp
c
        if (alam.gt.1.d0-tiny.or.vs.lt.tiny) then
c       if (alam.gt.1.d0-tiny.and..false.) then
          ier=0
          vs=0.d0
          vg=1.d0/r1
          a12=ajwl(1,2)*dexp(-rjwl(1,2)*vg)
          a22=ajwl(2,2)*dexp(-rjwl(2,2)*vg)
          b12=a12/omeg(2)
          b22=a22/omeg(2)
          zg=(b12+b22)*vg
          fg=zg-a12/rjwl(1,2)-a22/rjwl(2,2)
          fg=fg-fgvg0
          p=max(omeg(2)*(e1+r*fg),pmin)
          if (iform.gt.0) then
            dzg=b12+b22-(rjwl(1,2)*b12+rjwl(2,2)*b22)*vg
            dfg=dzg+a12+a22
            dp(1)=max(omeg(2)*(fg-dfg/r1),0.d0)
            dp(2)=omeg(2)
            dp(3)=0.d0
          end if
          return
        end if
c
        if (alam.lt.tiny.and..false.) then
          ier=0
          vs=1.d0/r1
          vg=vg0
          a11=ajwl(1,1)*dexp(-rjwl(1,1)*vs)
          a21=ajwl(2,1)*dexp(-rjwl(2,1)*vs)
          b11=a11/omeg(1)
          b21=a21/omeg(1)
          zs=(b11+b21)*vs
          fs=zs-a11/rjwl(1,1)-a21/rjwl(2,1)
          fs=fs-fsvs0-heat
          p=max(omeg(1)*(e1+r*fs),pmin)
          if (iform.gt.0) then
            dzs=b11+b21-(rjwl(1,1)*b11+rjwl(2,1)*b21)*vs
            dfs=dzs+a11+a21
            dp(1)=max(omeg(1)*(fs-dfs/r1),0.d0)
            dp(2)=omeg(1)
            dp(3)=0.d0
          end if
          return
        end if
c
        if (alam.lt.alam0) then
c
          fact=-y1/x1
c         vs=(1.d0-y1*vg)/x1
c
          vs=max(vs,vmin)
          vg=max(vg,vmin)
c
          it=0
    2     it=it+1
c
            a11=ajwl(1,1)*dexp(-rjwl(1,1)*vs)
            a21=ajwl(2,1)*dexp(-rjwl(2,1)*vs)
            a12=ajwl(1,2)*dexp(-rjwl(1,2)*vg)
            a22=ajwl(2,2)*dexp(-rjwl(2,2)*vg)
            b11=a11/omeg(1)
            b21=a21/omeg(1)
            b12=a12/omeg(2)
            b22=a22/omeg(2)
            zs=(b11+b21)*vs
            zg=(b12+b22)*vg
            fs=zs-a11/rjwl(1,1)-a21/rjwl(2,1)
            fg=zg-a12/rjwl(1,2)-a22/rjwl(2,2)
            fs=fs-fsvs0-heat
            fg=fg-fgvg0
            zs=zs-zsvs0
            zg=zg-zgvg0
            t1=cgcs*vs/omeg(1)-vg/omeg(2)
            t2=e1+x1*fs+y1*fg
            t3=x1*vs/omeg(1)+y1*vg/omeg(2)
            t4=cgcs*zs-zg+dtemp
c
            dzs=b11+b21-(rjwl(1,1)*b11+rjwl(2,1)*b21)*vs
            dzg=b12+b22-(rjwl(1,2)*b12+rjwl(2,2)*b22)*vg
            dfs=dzs+a11+a21
            dfg=dzg+a12+a22
            dt1=fact*cgcs/omeg(1)-1.d0/omeg(2)
            dt2=y1*(-dfs+dfg)
            dt3=-y1/omeg(1)+y1/omeg(2)
            dt4=fact*cgcs*dzs-dzg
c
            g=t1*t2-t3*t4
            dg=dt1*t2+t1*dt2-dt3*t4-t3*dt4
            if (dabs(dg).gt.eps) then
              del=g/dg
            else
              del=1.d10
            end if
            relax=1.d0
            if (dabs(del).gt.0.1d0) relax=.5d0
            vg=vg-relax*del
            vs=(1.d0-y1*vg)/x1
c
          if (dabs(del).gt.tol) then
            if (it.lt.itmax.and.vs.gt.vmin.and.vg.gt.vmin) then
c             write(55,202)it,vs,vg,g/dg
c 202         format(1x,i2,2(1x,f15.8),1x,1pe10.3)
              if (ier.gt.0) then
                write(17,102)it,vg,del
  102           format('it=',i2,', vg=',1pe15.8,', dvg=',1pe15.8)
              end if
              goto 2
            else
              if (ilow.eq.0) then
                it=0
                ilow=1
                vg=1.d0/(x1*omeg(1)/(cgcs*omeg(2))+y1)
                vs=(1.d0-y1*vg)/x1
                goto 2
              end if
              if (ier.gt.0) then
                write(17,103)it,vg,del
  103           format('it=',i2,', vg=',1pe15.8,', dvg=',1pe15.8,
     *                 '  itmax exceeded')
                return
              else
                if (ier.lt.0) then
c                 write(6,*)'Warning (geteos) : Newton did not converge'
c                 write(6,*)'iform, ier, lambda =',iform,ier,alam
                  vs=vshold
                  vg=vghold
                  return
                else
                  if (itmax.gt.1) then
c                   if (y.gt.1.d-6.or.r.gt.1.d0) then
c                   write(6,*)'geteos (fail1) : r,e,y,vshold,vghold=',
c    *                        r,e,y,vshold,vghold
c                   pause
c                   end if
                    call fail1burnt (r1,e1,y1,vs,vg,vshold,vghold,
     *                          dtemp,err,vmin)
                    if (err.gt.tol) then
                      ifail=1
                      ferr=err
                    end if
                    itmax=1
                    goto 2
                  else
c Note: if the code gets here, then the Newton failed on its
c       first attempt.  The iteration continued in fail mode
c       perhaps getting dtemp to zero.  After fail mode dtemp
c       should be set so that only one iteration of Newton is
c       needed to converge.  If the code ends up here, then
c       more than one iteration was needed...
                    write(6,*)'Bug (geteosburnt) : location 1'
                    write(6,101)r1,e1,y1,vshold,vghold
                    stop
                  end if
                end if
              end if
            end if
          else
            if (dabs(t3).lt.eps) then
              write(6,*)'Error (geteosburnt) : bad error, t3=0'
              stop
            end if
            p=t2/t3
          end if
c
        else
c
          fact=-x1/y1
c         vg=(1.d0-x1*vs)/y1
c
c This next line was commented out, 2/20/06.  DWS
c (Not sure why it was here in the first place.)
c         vs=vg
c
c..testing...
c         vs=1.d0/r1
c         vg=vs
c
          vs=max(vs,vmin)
          vg=max(vg,vmin)
c
          it=0
    3     it=it+1
c
            a11=ajwl(1,1)*dexp(-rjwl(1,1)*vs)
            a21=ajwl(2,1)*dexp(-rjwl(2,1)*vs)
            a12=ajwl(1,2)*dexp(-rjwl(1,2)*vg)
            a22=ajwl(2,2)*dexp(-rjwl(2,2)*vg)
            b11=a11/omeg(1)
            b21=a21/omeg(1)
            b12=a12/omeg(2)
            b22=a22/omeg(2)
            zs=(b11+b21)*vs
            zg=(b12+b22)*vg
            fs=zs-a11/rjwl(1,1)-a21/rjwl(2,1)
            fg=zg-a12/rjwl(1,2)-a22/rjwl(2,2)
            fs=fs-fsvs0-heat
            fg=fg-fgvg0
            zs=zs-zsvs0
            zg=zg-zgvg0
            t1=cgcs*vs/omeg(1)-vg/omeg(2)
            t2=e1+x1*fs+y1*fg
            t3=x1*vs/omeg(1)+y1*vg/omeg(2)
            t4=cgcs*zs-zg+dtemp
c
            dzs=b11+b21-(rjwl(1,1)*b11+rjwl(2,1)*b21)*vs
            dzg=b12+b22-(rjwl(1,2)*b12+rjwl(2,2)*b22)*vg
            dfs=dzs+a11+a21
            dfg=dzg+a12+a22
            dt1=cgcs/omeg(1)-fact/omeg(2)
            dt2=x1*(dfs-dfg)
            dt3=x1/omeg(1)-x1/omeg(2)
            dt4=cgcs*dzs-fact*dzg
c
            g=t1*t2-t3*t4
            dg=dt1*t2+t1*dt2-dt3*t4-t3*dt4
            if (dabs(dg).gt.eps) then
              del=g/dg
            else
              del=1.d10
            end if
            vs=vs-del
            vg=(1.d0-x1*vs)/y1
c
          if (dabs(del).gt.tol) then
            if (it.lt.itmax.and.vs.gt.vmin.and.vg.gt.vmin) then
              if (ier.gt.0) then
                write(17,105)it,vs,del
  105           format('it=',i2,', vs=',1pe15.8,', dvs=',1pe15.8)
              end if
              goto 3
            else
              if (ilow.eq.0) then
                it=0
                ilow=1
                vs=1.d0/(x1+y1*cgcs*omeg(2)/omeg(1))
                vg=(1.d0-x1*vs)/y1
                goto 3
              end if
              if (ier.gt.0) then
                write(17,106)it,vs,del
  106           format('it=',i2,', vs=',1pe15.8,', dvs=',1pe15.8,
     *                 '  itmax exceeded')
                return
              else
                if (ier.lt.0) then
c                 write(6,*)'Warning (geteos) : Newton did not converge'
c                 write(6,*)'iform, ier, lambda =',iform,ier,alam
                  vs=vshold
                  vg=vghold
                  return
                else
                  if (itmax.gt.1) then
c                   if (y.gt.1.d-6.or.r.gt.1.d0) then
c                   write(6,*)'geteos (fail1) : r,e,y,vshold,vghold=',
c    *                        r,e,y,vshold,vghold
c                   pause
c                   end if
                    call fail1burnt (r1,e1,y1,vs,vg,vshold,vghold,
     *                          dtemp,err,vmin)
                    if (err.gt.tol) then
                      ifail=1
                      ferr=err
                    end if
                    itmax=1
                    goto 3
                  else
c Note: if the code gets here, then the Newton failed on its
c       first attempt.  The iteration continued in fail mode
c       perhaps getting dtemp to zero.  After fail mode dtemp
c       should be set so that only one iteration of Newton is
c       needed to converge.  If the code ends up here, then
c       more than one iteration was needed...
                    write(6,*)'Bug (geteosburnt) : location 2'
                    write(6,101)r,e,y,vshold,vghold
                    stop
                  end if
                end if
              end if
            end if
          else
            if (dabs(t3).lt.eps) then
              write(6,*)'Error (geteosburnt) : bad error, t3=0'
              stop
            end if
            p=t2/t3
          end if
c
        end if
c
        ier=0
        if (iform.gt.0) then
c
c..compute derivatives
c    dp(1)=dp/dr
c    dp(2)=dp/de
c    dp(3)=dp/dy
c
c (vs,vg) solve the equations
c         t1*t2-t3*t4=0
c         x*vs+y*vg-1=0
c
c where  x=r-y
c        t1=cgcs*vs/omeg(1)-vg/omeg(2)
c        t2=e+x*fs+y*fg
c        t3=x*vs/omeg(1)+y*vg/omeg(2)
c        t4=cgcs*zs-zg
c
c set up a system where a(1:2,3:5) become the derivatives of (vs,vg)
c with respect to (r,e,y)
          a(1,1)=cgcs*(t2/omeg(1)-t3*dzs)+x1*(t1*dfs-t4/omeg(1))
          a(1,2)=     -t2/omeg(2)+t3*dzg +y1*(t1*dfg-t4/omeg(2))
          a(1,3)=-t1*fs+t4*vs/omeg(1)
          a(1,4)=-t1
          a(1,5)=-t1*fg+t4*vg/omeg(2)-a(1,3)
          a(2,1)=x1
          a(2,2)=y1
          a(2,3)=-vs
          a(2,4)=0.d0
          a(2,5)=vs-vg
c
          if (alam.le.alam0) then
            ivs=2
            ivg=1
            fact=a(1,1)/x1
            a(1,2)=a(1,2)-fact*a(2,2)
            a(1,3)=a(1,3)-fact*a(2,3)
            a(1,5)=a(1,5)-fact*a(2,5)
            do j=3,5
              a(1,j)=a(1,j)/a(1,2)
              a(2,j)=(a(2,j)-y1*a(1,j))/x1
            end do
          else
            ivs=1
            ivg=2
            fact=a(1,2)/y1
            a(1,1)=a(1,1)-fact*a(2,1)
            a(1,3)=a(1,3)-fact*a(2,3)
            a(1,5)=a(1,5)-fact*a(2,5)
            do j=3,5
              a(1,j)=a(1,j)/a(1,1)
              a(2,j)=(a(2,j)-x1*a(1,j))/y1
            end do
          end if
c
c the pressure p solves p*t3=t2.  Differentiate to get dp(1:3)
          dp(1)=(fs+x1*dfs*a(ivs,3)+y1*dfg*a(ivg,3)
     *          -p*((vs+x1*a(ivs,3))/omeg(1)+y1*a(ivg,3)/omeg(2)))/t3
          dp(2)=(1.d0+x1*dfs*a(ivs,4)+y1*dfg*a(ivg,4)
     *          -p*(x1*a(ivs,4)/omeg(1)+y1*a(ivg,4)/omeg(2)))/t3
          dp(3)=(fg-fs+x1*dfs*a(ivs,5)+y1*dfg*a(ivg,5)
     *          -p*((vg+y1*a(ivg,5))/omeg(2)
     *             -(vs-x1*a(ivs,5))/omeg(1)))/t3
c
        end if
c
      else
c
c..rho, p, rho*lambda given.  Update vs and vg and return rho*e
c
        if (alam.gt.1.d0-tiny.or.vs.lt.tiny) then
c       if (alam.gt.1.d0-tiny.and..false.) then
          ier=0
          vs=0.d0
          vg=1.d0/r1
          a12=ajwl(1,2)*dexp(-rjwl(1,2)*vg)
          a22=ajwl(2,2)*dexp(-rjwl(2,2)*vg)
          b12=a12/omeg(2)
          b22=a22/omeg(2)
          zg=(b12+b22)*vg
          fg=zg-a12/rjwl(1,2)-a22/rjwl(2,2)
          fg=fg-fgvg0
          e=max(p/omeg(2)-r1*fg,emin)
          return
        end if
c
        if (alam.lt.tiny.and..false.) then
          ier=0
          vs=1.d0/r1
          vg=vg0
          a11=ajwl(1,1)*dexp(-rjwl(1,1)*vs)
          a21=ajwl(2,1)*dexp(-rjwl(2,1)*vs)
          b11=a11/omeg(1)
          b21=a21/omeg(1)
          zs=(b11+b21)*vs
          fs=zs-a11/rjwl(1,1)-a21/rjwl(2,1)
          fs=fs-fsvs0-heat
          e=max(p/omeg(1)-r1*fs,emin)
          return
        end if
c
        p1=max(p,pmin)
c
        if (alam.lt.alam0) then
c
          if (vg.lt.vmin) then
            vs=1.d0/r1
            vg=vg0
          end if
c
          fact=-y1/x1
          vs=(1.d0-y1*vg)/x1
c
          vs=max(vs,vmin)
          vg=max(vg,vmin)
c
          it=0
    4     it=it+1
c
            b11=ajwl(1,1)*dexp(-rjwl(1,1)*vs)/omeg(1)
            b21=ajwl(2,1)*dexp(-rjwl(2,1)*vs)/omeg(1)
            b12=ajwl(1,2)*dexp(-rjwl(1,2)*vg)/omeg(2)
            b22=ajwl(2,2)*dexp(-rjwl(2,2)*vg)/omeg(2)
            zs=(b11+b21)*vs
            zg=(b12+b22)*vg
            zs=zs-zsvs0
            zg=zg-zgvg0
            t1=cgcs*vs/omeg(1)-vg/omeg(2)
            t4=cgcs*zs-zg+dtemp
c
            dzs=b11+b21-(rjwl(1,1)*b11+rjwl(2,1)*b21)*vs
            dzg=b12+b22-(rjwl(1,2)*b12+rjwl(2,2)*b22)*vg
            dt1=fact*cgcs/omeg(1)-1.d0/omeg(2)
            dt4=fact*cgcs*dzs-dzg
c
            g=t1*p1-t4
            dg=dt1*p1-dt4
            if (dabs(dg).gt.eps) then
              del=g/dg
            else
              del=1.d10
            end if
            relax=1.d0
            if (dabs(del).gt.0.1d0) relax=.5d0
            vg=vg-relax*del
            vs=(1.d0-y1*vg)/x1
c
          if (dabs(del).gt.tol) then
            if (it.lt.itmax.and.vs.gt.vmin.and.vg.gt.vmin) then
c             write(55,202)it,vs,vg,g/dg
c 202         format(1x,i2,2(1x,f15.8),1x,1pe10.3)
              if (ier.gt.0) then
                write(17,102)it,vg,del
              end if
              goto 4
            else
              if (ilow.eq.0) then
                it=0
                ilow=1
                vg=1.d0/(x1*omeg(1)/(cgcs*omeg(2))+y1)
                vs=(1.d0-y1*vg)/x1
                goto 4
              end if
              if (ier.gt.0) then
                write(17,103)it,vs,del
                return
              else
c               if (ier.lt.0) return
                if (itmax.gt.1) then
c                 if (y.gt.1.d-6.or.r.gt.1.d0) then
c                 write(6,*)'geteos (fail2) : r,p,y,vshold,vghold=',
c    *                      r,p,y,vshold,vghold
c                 pause
c                 end if
                  call fail2burnt (r1,p1,y1,vs,vg,vshold,vghold,
     *                        dtemp,err,vmin)
                  if (err.gt.tol) then
                    ifail=1
                    ferr=err
                  end if
                  itmax=1
                  goto 4
                else
c Note: if the code gets here, then the Newton failed on its
c       first attempt.  The iteration continued in fail mode
c       perhaps getting dtemp to zero.  After fail mode dtemp
c       should be set so that only one iteration of Newton is
c       needed to converge.  If the code ends up here, then
c       more than one iteration was needed...
                  write(6,*)'Bug (geteosburnt) : location 3'
                  write(6,111)r,p,y,vshold,vghold
                  stop
                end if
              end if
            end if
          end if
c
        else
          fact=-x1/y1
c         vg=(1.d0-x1*vs)/y1
ccc       vs=vg
c
          vs=max(vs,vmin)
          vg=max(vg,vmin)
c
          it=0
    5     it=it+1
c
            b11=ajwl(1,1)*dexp(-rjwl(1,1)*vs)/omeg(1)
            b21=ajwl(2,1)*dexp(-rjwl(2,1)*vs)/omeg(1)
            b12=ajwl(1,2)*dexp(-rjwl(1,2)*vg)/omeg(2)
            b22=ajwl(2,2)*dexp(-rjwl(2,2)*vg)/omeg(2)
            zs=(b11+b21)*vs
            zg=(b12+b22)*vg
            zs=zs-zsvs0
            zg=zg-zgvg0
            t1=cgcs*vs/omeg(1)-vg/omeg(2)
            t4=cgcs*zs-zg+dtemp
c
            dzs=b11+b21-(rjwl(1,1)*b11+rjwl(2,1)*b21)*vs
            dzg=b12+b22-(rjwl(1,2)*b12+rjwl(2,2)*b22)*vg
            dt1=cgcs/omeg(1)-fact/omeg(2)
            dt4=cgcs*dzs-fact*dzg
c
            g=t1*p1-t4
            dg=dt1*p1-dt4
            if (dabs(dg).gt.eps) then
              del=g/dg
            else
              del=1.d10
            end if
            vs=vs-del
            vg=(1.d0-x1*vs)/y1
c
          if (dabs(del).gt.tol) then
            if (it.lt.itmax.and.vs.gt.vmin.and.vg.gt.vmin) then
              if (ier.gt.0) then
                write(17,105)it,vs,del
              end if
              goto 5
            else
              if (ilow.eq.0) then
                it=0
                ilow=1
                vs=1.d0/(x1+y1*cgcs*omeg(2)/omeg(1))
                vg=(1.d0-x1*vs)/y1
                goto 5
              end if
              if (ier.gt.0) then
                write(17,106)it,vs,del
                return
              else
c               if (ier.lt.0) return
                if (itmax.gt.1) then
c                 if (y.gt.1.d-6.or.r.gt.1.d0) then
c                 write(6,*)'geteos (fail2) : r,p,y,vshold,vghold=',
c    *                      r,p,y,vshold,vghold
c                 pause
c                 end if
                  call fail2burnt (r1,p1,y1,vs,vg,vshold,vghold,
     *                        dtemp,err,vmin)
                  if (err.gt.tol) then
                    ifail=1
                    ferr=err
                  end if
                  itmax=1
                  goto 5
                else
c Note: if the code gets here, then the Newton failed on its
c       first attempt.  The iteration continued in fail mode
c       perhaps getting dtemp to zero.  After fail mode dtemp
c       should be set so that only one iteration of Newton is
c       needed to converge.  If the code ends up here, then
c       more than one iteration was needed...
                  write(6,*)'Bug (geteosburnt) : location 4'
                  write(6,111)r,p,y,vshold,vghold
                  stop
                end if
              end if
            end if
          end if
c
        end if
c
        ier=0
        a11=ajwl(1,1)*dexp(-rjwl(1,1)*vs)
        a21=ajwl(2,1)*dexp(-rjwl(2,1)*vs)
        a12=ajwl(1,2)*dexp(-rjwl(1,2)*vg)
        a22=ajwl(2,2)*dexp(-rjwl(2,2)*vg)
        b11=a11/omeg(1)
        b21=a21/omeg(1)
        b12=a12/omeg(2)
        b22=a22/omeg(2)
        zs=(b11+b21)*vs
        zg=(b12+b22)*vg
        fs=zs-a11/rjwl(1,1)-a21/rjwl(2,1)
        fg=zg-a12/rjwl(1,2)-a22/rjwl(2,2)
        es=p1*vs/omeg(1)-fs+fsvs0+heat
        eg=p1*vg/omeg(2)-fg+fgvg0
        e=x1*es+y1*eg
      end if
c
      return
      end
c
c+++++++++++++++++++
c
      subroutine fail1burnt (r,e,y,vs,vg,vshold,vghold,dtemp,err,vmin)
c
      implicit real*8 (a-h,o-z)
      include 'eosburntdat.h'
      data alam0,tol,eps,itmax / .5d0,1.d-8,1.d-12,15 /
c
      x=r-y
      alam=y/r
c
c     write(6,*)'fail1: r,e,alam,vshold,vghold=',r,e,alam,vshold,vghold
c     read(5,*)vs,vg
c
      vs=max(vshold,vmin)
      vg=max(vghold,vmin)
c      vs=1.d0/r
c      vg=vs
c      vs=.6d0
c      vg=.9d0
c      write(6,*)'fail...'
c      pause
c
      a11=ajwl(1,1)*dexp(-rjwl(1,1)*vs)
      a21=ajwl(2,1)*dexp(-rjwl(2,1)*vs)
      a12=ajwl(1,2)*dexp(-rjwl(1,2)*vg)
      a22=ajwl(2,2)*dexp(-rjwl(2,2)*vg)
      b11=a11/omeg(1)
      b21=a21/omeg(1)
      b12=a12/omeg(2)
      b22=a22/omeg(2)
      zs=(b11+b21)*vs
      zg=(b12+b22)*vg
      fs=zs-a11/rjwl(1,1)-a21/rjwl(2,1)
      fg=zg-a12/rjwl(1,2)-a22/rjwl(2,2)
      fs=fs-fsvs0-heat
      fg=fg-fgvg0
      zs=zs-zsvs0
      zg=zg-zgvg0
      t1=cgcs*vs/omeg(1)-vg/omeg(2)
      t2=e+x*fs+y*fg
      t3=x*vs/omeg(1)+y*vg/omeg(2)
      t4=cgcs*zs-zg
c
      p=t2/t3
      delta=p*t1-t4
c
      nsteps=5
      do k=1,nsteps
        dtemp=delta*(nsteps-k)/nsteps
c
        vsk=vs
        vgk=vg
c
        if (alam.lt.alam0) then
c
          fact=-y/x
c
          it=0
    2     it=it+1
c
            a11=ajwl(1,1)*dexp(-rjwl(1,1)*vs)
            a21=ajwl(2,1)*dexp(-rjwl(2,1)*vs)
            a12=ajwl(1,2)*dexp(-rjwl(1,2)*vg)
            a22=ajwl(2,2)*dexp(-rjwl(2,2)*vg)
            b11=a11/omeg(1)
            b21=a21/omeg(1)
            b12=a12/omeg(2)
            b22=a22/omeg(2)
            zs=(b11+b21)*vs
            zg=(b12+b22)*vg
            fs=zs-a11/rjwl(1,1)-a21/rjwl(2,1)
            fg=zg-a12/rjwl(1,2)-a22/rjwl(2,2)
            fs=fs-fsvs0-heat
            fg=fg-fgvg0
            zs=zs-zsvs0
            zg=zg-zgvg0
            t1=cgcs*vs/omeg(1)-vg/omeg(2)
            t2=e+x*fs+y*fg
            t3=x*vs/omeg(1)+y*vg/omeg(2)
            t4=cgcs*zs-zg+dtemp
c
            dzs=b11+b21-(rjwl(1,1)*b11+rjwl(2,1)*b21)*vs
            dzg=b12+b22-(rjwl(1,2)*b12+rjwl(2,2)*b22)*vg
            dfs=dzs+a11+a21
            dfg=dzg+a12+a22
            dt1=fact*cgcs/omeg(1)-1.d0/omeg(2)
            dt2=y*(-dfs+dfg)
            dt3=-y/omeg(1)+y/omeg(2)
            dt4=fact*cgcs*dzs-dzg
c
            g=t1*t2-t3*t4
            dg=dt1*t2+t1*dt2-dt3*t4-t3*dt4
            if (dabs(dg).gt.eps) then
              del=g/dg
            else
              del=1.d10
            end if
            relax=1.d0
            if (dabs(del).gt.0.1d0) relax=.5d0
            vg=vg-relax*del
            vs=(1.d0-y*vg)/x
c
          if (dabs(del).gt.tol) then
            if (it.lt.itmax.and.vs.gt.vmin.and.vg.gt.vmin) then
              goto 2
            else
c              write(6,*)'Warning (fail1) : unable to satisfy ',
c     *                  'temperature equilibrium, del_temp=',dtemp
              dtemp=delta*(nsteps-k+1.d0)/nsteps
c              err=dabs(dtemp)/max(dabs(t4),eps)
              err=dabs(dtemp)/(dabs(dtemp)+max(dabs(t4),eps))
              vs=vsk
              vg=vgk
              return
            end if
          end if
c
        else
c
          fact=-x/y
c
          it=0
    3     it=it+1
c
            a11=ajwl(1,1)*dexp(-rjwl(1,1)*vs)
            a21=ajwl(2,1)*dexp(-rjwl(2,1)*vs)
            a12=ajwl(1,2)*dexp(-rjwl(1,2)*vg)
            a22=ajwl(2,2)*dexp(-rjwl(2,2)*vg)
            b11=a11/omeg(1)
            b21=a21/omeg(1)
            b12=a12/omeg(2)
            b22=a22/omeg(2)
            zs=(b11+b21)*vs
            zg=(b12+b22)*vg
            fs=zs-a11/rjwl(1,1)-a21/rjwl(2,1)
            fg=zg-a12/rjwl(1,2)-a22/rjwl(2,2)
            fs=fs-fsvs0-heat
            fg=fg-fgvg0
            zs=zs-zsvs0
            zg=zg-zgvg0
            t1=cgcs*vs/omeg(1)-vg/omeg(2)
            t2=e+x*fs+y*fg
            t3=x*vs/omeg(1)+y*vg/omeg(2)
            t4=cgcs*zs-zg+dtemp
c
            dzs=b11+b21-(rjwl(1,1)*b11+rjwl(2,1)*b21)*vs
            dzg=b12+b22-(rjwl(1,2)*b12+rjwl(2,2)*b22)*vg
            dfs=dzs+a11+a21
            dfg=dzg+a12+a22
            dt1=cgcs/omeg(1)-fact/omeg(2)
            dt2=x*(dfs-dfg)
            dt3=x/omeg(1)-x/omeg(2)
            dt4=cgcs*dzs-fact*dzg
c
            g=t1*t2-t3*t4
            dg=dt1*t2+t1*dt2-dt3*t4-t3*dt4
            if (dabs(dg).gt.eps) then
              del=g/dg
            else
              del=1.d10
            end if
            vs=vs-del
            vg=(1.d0-x*vs)/y
c
          if (dabs(del).gt.tol) then
            if (it.lt.itmax.and.vs.gt.vmin.and.vg.gt.vmin) then
              goto 3
            else
c              write(6,*)'Warning (fail1) : unable to satisfy ',
c     *                  'temperature equilibrium, del_temp=',dtemp
              dtemp=delta*(nsteps-k+1.d0)/nsteps
c              err=dabs(dtemp)/max(dabs(t4),eps)
              err=dabs(dtemp)/(dabs(dtemp)+max(dabs(t4),eps))
              vs=vsk
              vg=vgk
              return
            end if
          end if
c
        end if
c
      end do
c
c     write(6,*)'fail: vs,vg=',vs,vg
c
      return
      end
c
c+++++++++++++++++++
c
      subroutine fail2burnt (r,p,y,vs,vg,vshold,vghold,dtemp,err,vmin)
c
      implicit real*8 (a-h,o-z)
      include 'eosburntdat.h'
      data alam0,tol,eps,itmax / .5d0,1.d-8,1.d-12,15 /
c
      x=r-y
      alam=y/r
c
      vs=max(vshold,vmin)
      vg=max(vghold,vmin)
c
c     write(6,234)r,p,y,vs,vg
c 234 format('** fail2: r,p,y,vs,vg=',5(1x,1pe9.2))
c     read(5,*)vg
c
      b11=ajwl(1,1)*dexp(-rjwl(1,1)*vs)/omeg(1)
      b21=ajwl(2,1)*dexp(-rjwl(2,1)*vs)/omeg(1)
      b12=ajwl(1,2)*dexp(-rjwl(1,2)*vg)/omeg(2)
      b22=ajwl(2,2)*dexp(-rjwl(2,2)*vg)/omeg(2)
      zs=(b11+b21)*vs
      zg=(b12+b22)*vg
      zs=zs-zsvs0
      zg=zg-zgvg0
      t1=cgcs*vs/omeg(1)-vg/omeg(2)
      t4=cgcs*zs-zg
c
      delta=p*t1-t4
c
      nsteps=5
      do k=1,nsteps
        dtemp=delta*(nsteps-k)/nsteps
c
        vsk=vs
        vgk=vg
c
        if (alam.lt.alam0) then
c
          fact=-y/x
c
          it=0
    4     it=it+1
c
            b11=ajwl(1,1)*dexp(-rjwl(1,1)*vs)/omeg(1)
            b21=ajwl(2,1)*dexp(-rjwl(2,1)*vs)/omeg(1)
            b12=ajwl(1,2)*dexp(-rjwl(1,2)*vg)/omeg(2)
            b22=ajwl(2,2)*dexp(-rjwl(2,2)*vg)/omeg(2)
            zs=(b11+b21)*vs
            zg=(b12+b22)*vg
            zs=zs-zsvs0
            zg=zg-zgvg0
            t1=cgcs*vs/omeg(1)-vg/omeg(2)
            t4=cgcs*zs-zg+dtemp
c
            dzs=b11+b21-(rjwl(1,1)*b11+rjwl(2,1)*b21)*vs
            dzg=b12+b22-(rjwl(1,2)*b12+rjwl(2,2)*b22)*vg
            dt1=fact*cgcs/omeg(1)-1.d0/omeg(2)
            dt4=fact*cgcs*dzs-dzg
c
            g=t1*p-t4
            dg=dt1*p-dt4
            if (dabs(dg).gt.eps) then
              del=g/dg
            else
              del=1.d10
            end if
            relax=1.d0
            if (dabs(del).gt.0.1d0) relax=.5d0
            vg=vg-relax*del
            vs=(1.d0-y*vg)/x
c
          if (dabs(del).gt.tol) then
            if (it.lt.itmax.and.vs.gt.vmin.and.vg.gt.vmin) then
              goto 4
            else
c              write(6,*)'Warning (fail2) : unable to satisfy ',
c     *                  'temperature equilibrium, del_temp=',dtemp
              dtemp=delta*(nsteps-k+1.d0)/nsteps
c              err=dabs(dtemp)/max(dabs(t4),eps)
              err=dabs(dtemp)/(dabs(dtemp)+max(dabs(t4),eps))
              vs=vsk
              vg=vgk
              return
            end if
          end if
c
        else
c
          fact=-x/y
c
          it=0
    5     it=it+1
c
            b11=ajwl(1,1)*dexp(-rjwl(1,1)*vs)/omeg(1)
            b21=ajwl(2,1)*dexp(-rjwl(2,1)*vs)/omeg(1)
            b12=ajwl(1,2)*dexp(-rjwl(1,2)*vg)/omeg(2)
            b22=ajwl(2,2)*dexp(-rjwl(2,2)*vg)/omeg(2)
            zs=(b11+b21)*vs
            zg=(b12+b22)*vg
            zs=zs-zsvs0
            zg=zg-zgvg0
            t1=cgcs*vs/omeg(1)-vg/omeg(2)
            t4=cgcs*zs-zg+dtemp
c
            dzs=b11+b21-(rjwl(1,1)*b11+rjwl(2,1)*b21)*vs
            dzg=b12+b22-(rjwl(1,2)*b12+rjwl(2,2)*b22)*vg
            dt1=cgcs/omeg(1)-fact/omeg(2)
            dt4=cgcs*dzs-fact*dzg
c
            g=t1*p-t4
            dg=dt1*p-dt4
            if (dabs(dg).gt.eps) then
              del=g/dg
            else
              del=1.d10
            end if
            vs=vs-del
            vg=(1.d0-x*vs)/y
c
          if (dabs(del).gt.tol) then
            if (it.lt.itmax.and.vs.gt.vmin.and.vg.gt.vmin) then
              goto 5
            else
c              write(6,*)'Warning (fail2) : unable to satisfy ',
c     *                  'temperature equilibrium, del_temp=',dtemp
              dtemp=delta*(nsteps-k+1.d0)/nsteps
c              err=dabs(dtemp)/max(dabs(t4),eps)
              err=dabs(dtemp)/(dabs(dtemp)+max(dabs(t4),eps))
              vs=vsk
              vg=vgk
              return
            end if
          end if
c
        end if
c
      end do
c
      return
      end
