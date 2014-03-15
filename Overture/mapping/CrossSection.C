      subroutine cgcscs(id,rd,ip,k,r,x,xr)
c================================================================
c Composite Grid Curve routine:
c
c       CGCSCS: define 3d Surfaces by Cross-Sections
c       --------------------------------------------
c
c  Purpose: This routine defines a number of surfaces that can
c   be used by CMPGRD. The surfaces all are topologically spheres
c   or parts of spheres and include a sphere, ellipse, banana,
c   Joukowsky wing and an M6 wing. The surfaces are all created by
c   defining cross-sections (routines CS1,CS2,...) and using the
c   routine CGCS to put patches on the surface.
c
c  To define any of the surfaces here one must use the EXTERNAL
c  option of the CURVE menu in CMPGRD.
c
c Input -
c
c       integer   id(*),ip(*),k
c       real      rd(*),r,x(*),xr(*)
c
c       id, rd:   The DSK array passed to CMPGRD
c       ip:       Parameter array used to pass and return arguments
c       k:        Curve number
c       r:        Coordinate in the unit interval
c       x,xr:     Curve function and its first derivative
c
c Who to blame
c  Bill Henshaw
c=======================================================================
      integer id(*),ip(*)
      real rd(*),r(*),x(*),xr(3,*)
c.......local
      parameter(kmax=10,magic=900827,info=2)
      integer nd(kmax),bc(kmax),nrs(2,kmax),per(2,kmax),share(kmax),
     & iop(4,kmax),kcv(kmax)
c       rc(.,k) = real parameters for use with curve k
      real rop(10,kmax),rc(4,kmax)
      parameter( nmenu1=5,nmenu2=3,nmenu3=4 )
      character line*80,answer*80,menu1(nmenu1)*9,menu2(nmenu2)*6,
     & menu3(nmenu3)*10
      external cs1,cs2,cs3,cs4,cs5,cs6,cs7
      include 'gchain.h'

      save

      data menu1/'SPHERE','BANANA','WING-J',
     & 'WING-M6','WING-M6S'/
      data menu2/'BOTTOM','MIDDLE','TOP'/
      data menu3/'EXIT','DIMENSIONS','CENTRE','PARAMETERS'/
c
      if(ip(3).eq.1)then
        if( kcv(k).le.3 )then
c............ellipsoid (spheroid)
          ipatch=kcv(k)-2
          call cgcs( ipatch,cs2,r,x,xr,iop(1,k),rop(1,k),ierr )
        elseif( kcv(k).le.6 )then
c.............banana
          ipatch=kcv(k)-5
          call cgcs( ipatch,cs3,r,x,xr,iop(1,k),rop(1,k),ierr )
        elseif( kcv(k).le.9  )then
c.............Joukowsky
          ipatch=kcv(k)-8
          call cgcs( ipatch,cs5,r,x,xr,iop(1,k),rop(1,k),ierr )
        elseif( kcv(k).le.12 )then
c.............M6:
          ipatch=kcv(k)-11
          call cgcs( ipatch,cs6,r,x,xr,iop(1,k),rop(1,k),ierr )
        elseif( kcv(k).le.15 )then
c..............smoothed M6, distance dn away
c            rc(1,k) = dn : normal displacement distance
          ipatch=kcv(k)-14
          call cgcvn( ipatch,r,x,xr,rc(1,k),iop(1,k),rop(1,k),cs7 )
        else
          stop 'CGCSCS: inavlid value for k'
        end if
      elseif(ip(3).eq.0)then
c*        q0=dskfnd(id,ip(2),'q0')
        ip(4)=0
        ip(5)=magic
        ncv=0
      elseif(ip(3).eq.-1)then
        ip(4)=nd(k)
      elseif(ip(3).eq.-2)then
        ip(4)=nrs(1,k)
        ip(5)=nrs(2,k)
      elseif(ip(3).eq.-3)then
        ip(4)=bc(k)
      elseif(ip(3).eq.-4)then
        ip(4)=per(1,k)
        ip(5)=per(2,k)
      elseif(ip(3).eq.-5)then
        ip(4)=share(k)
      elseif(ip(3).eq.-6)then
c.........Define a new curve
        if(k.le.kmax)then
c           assign default values:
          nd(k)=3
          bc(k)=1
          nrs(1,k)=20
          nrs(2,k)=20
          per(1,k)=0
          per(2,k)=0
          share(k)=0
          line='CGCSCS: Create a surface from cross-sections'
          call giout( id,q0,info,line )
          line=' SPHERE: surface of a 3D sphere or ellipse'
          call giout( id,q0,info,line )
          line=' BANANA   : surface of a banana '
          call giout( id,q0,info,line )
          line=' WING-J   : wing with Joukowsky airfoil'
          call giout( id,q0,info,line )
          line=' WING-M6  : M6 wing'
          call giout( id,q0,info,line )
          line=' WING-M6S : smoothed M6 wing '
          call giout( id,q0,info,line )
          call giin( id,q0,menu1,nmenu1,'CGCSCS>',answer)
          do 100 i=1,nmenu1
            if( answer.eq.menu1(i) )then
              icv=3*(i-1)
              goto 150
            end if
 100      continue
          stop 'CGCSCS: invalid choice'
 150      continue
          line=' BOTTOM : choose bottom patch'
          call giout( id,q0,info,line )
          line=' MIDDLE : choose middle patch'
          call giout( id,q0,info,line )
          line=' TOP    : choose top patch'
          call giout( id,q0,info,line )
          call giin( id,q0,menu2,nmenu2,'CGCSCS>',answer)
          do 200 i=1,nmenu2
            if( answer.eq.menu2(i) )then
              icv=icv+i
              goto 250
            end if
 200      continue
          stop 'CGCSCS: invalid choice'
 250      continue
          if( icv.gt.0 .and. icv.le.18 )then
            if( k.lt.1 .or. k.gt.kmax )then
              stop 'CGCSCS: Invalid value for k'
            end if
            kcv(k)=icv
c             set ip(4)=0, new curve is defined.
            ip(4)=0
            rop( 1,k)=1.5
            rop( 2,k)=1.5
            rop( 3,k)=.5
            rop( 4,k)=.9
            rop( 5,k)=1.
            rop( 6,k)=1.
            rop( 7,k)=1.
            rop( 8,k)=0.
            rop( 9,k)=0.
            rop( 10,k)=0.
            ntry=100
            do 500 itry=1,ntry
              line=' DIMENSIONS: length of axes a,b,c'
              call giout( id,q0,info,line )
              line=' CENTRE : centre of ellipsoid xc,yc,zc'
              call giout( id,q0,info,line )
              line=' PARAMETERS: Patch parameters'
              call giout( id,q0,info,line )
              call giin( id,q0,menu3,nmenu3,'CGCSCS>',answer)
c                 (3,4)=(sa,sb), (5,6,7)=(a,b,c) (8,9,10)=centre
              if( answer.eq.'EXIT' )then
                goto 550
              elseif( answer.eq.'DIMENSIONS' )then
c                  axes lengths (a,b,c)
                write(line,9000) (rop(i,k),i=5,7)
                call giin( id,q0,' ',0,line,answer)
                call gigtre( id,q0,answer,rop(5,k),3,ierr)
                write(line,9050) (rop(i,k),i=5,7)
              elseif( answer.eq.'CENTRE' )then
c                  centre
                write(line,9100) (rop(i,k),i=8,10)
                call giin( id,q0,' ',0,line,answer)
                call gigtre( id,q0,answer,rop(8,k),3,ierr)
                write(line,9150) (rop(i,k),i=8,10)
              elseif( answer.eq.'PARAMETERS' )then
c                   patch parameters
                if( icv.eq. 2 .or. icv.eq. 5 .or. icv.eq. 8 .or.
     &              icv.eq.11 .or. icv.eq.14      )then
c                    centre patch
                  line=' (za,zb)=(.5,.90) : 90% of entire sphere'
                  call giout( id,q0,info,line )
                  line=' (za,zb)=(.0,.45) : 90% of top half'
                  call giout( id,q0,info,line )
                  write(line,9200) (rop(i,k),i=3,4)
                  call giin( id,q0,' ',0,line,answer)
                  call gigtre( id,q0,answer,rop(3,k),2,ierr)
                  write(line,9250) (rop(i,k),i=3,4)
                  call giout( id,q0,info,line )
c                   central patch is periodic:
                  per(1,k)=2
                else
c.................patch on ends
                  line=' (sa,sb) : extent of patch on the pole'
                  call giout( id,q0,info,line )
                  line=' (1.5,1.5) : medium patch'
                  call giout( id,q0,info,line )
                  line=' (4.0,4.0) : patch covers half the sphere'
                  call giout( id,q0,info,line )
                  write(line,9300) (rop(i,k),i=1,2)
                  call giin( id,q0,' ',0,line,answer)
                  call gigtre( id,q0,answer,rop(1,k),2,ierr)
                  write(line,9350) (rop(i,k),i=1,2)
                  call giout( id,q0,info,line )
                end if
              end if
              share(k)=-(1+(icv-1)/3)
              if( icv.eq.13 .or. icv.eq.14 .or. icv.eq.15 )then
                rc(1,k)=.5
                write(line,9400) rc(1,k)
                call giin( id,q0,' ',0,line,answer)
                call gigtre( id,q0,answer,rc(1,k),1,ierr)
                write(line,9450) rc(1,k)
                call giout( id,q0,info,line )
              end if
 500        continue
            line=' CGCSCS: Too many tries, exiting...'
            call giout( id,q0,info,line )
 550        continue
          else
            line='CGCSCS: Invalid value for icv'
            call giout( id,q0,info,line )
          end if
 9000 format('CGCSCS: Enter a,b,c (current values =',3f5.2,')')
 9050 format('CGCSCS: New values a,b,c  =',3f7.2)
 9100 format('CGCSCS: Enter x,y,z (current values =',3f5.2,')')
 9150 format('CGCSCS: New values x,y,z  =',3f7.2)
 9200 format('CGCSCS: Enter za,zb (current values =',2f5.2,')')
 9250 format('CGCSCS: New values za,zb  =',2f7.2)
 9300 format('CGCSCS: Enter sa,sb (current values =',2f5.2,')')
 9350 format('CGCSCS: New values sa,sb  =',2f7.2)
 9400 format('CGCSCS: Enter normal distance, dn, (default=',f5.2,')')
 9450 format('CGCSCS: New normal distance, dn=',f7.2)

        endif
      elseif(ip(3).eq.-7)then
c       Delete curve k.
      else
        write(6,*) 'ERROR:CGCSCS: ip(3) =',ip(3)
        stop       '     :Unknown value for ip(3)'
      endif
      return
      end

      subroutine cgcs( ipatch,cs,r,x,xr,ip,rp,ierr )
c================================================================
c Composite Grid routine:
c
c       CGCS: Define 3D surfaces by Cross-Sections
c       ------------------------------------------
c
c
c       integer   id(*),ip(*)
c       real      rd(*),r,x(*),xr(*),rp(*)
c       external  cs
c
c
c       ipatch:   patch to use, -1, 0, +1
c       cs:       subroutine defining cross sections
c       ip:       integer parameter array
c       rp:       real parameter array
c       r:        Coordinate in the unit interval
c       x,xr:     Curve function and its first derivative
c       id, rd:   The DSK array passed to CMPGRD
c
c rp:   rp(1) = sa, rp(2) = sb, rp(3) = za, rp(4) = zb
c       rp(5),... : passed on to the subroutine cs
c             s1   = sa*( r(1) - .5 )
c             s2   = sb*( r(1) - .5 )
c             zeta = 2.*zb*( r(2) - za )
c=======================================================================
      real rp(*),r(2),x(3),xr(3,2)
      integer ipatch,ip(*)
c........local
      parameter( info=2 )
      real f(3),fr(3,2)
      logical first
      external cs
      save
      data first/.true./

      if( first )then
        first=.false.
        tpi=8.*atan(1.)
        eps=r1mach(4)
      end if

      if( ipatch.eq.0 )then
c..............central patch

        theta=tpi*r(1)
c...........zeta = 2.*zb*( r(2) - za )
        zf=2.*rp(4)
        zeta=zf*(r(2)-rp(3))
        ct=cos(theta)
        st=sin(theta)
        ro=sqrt(max(eps,1.-zeta**2))
        call cs( ro,zeta,ct,st, f,fr,rp(5) )
        do 300 kd=1,3
          x(kd)=f(kd)
          xr(kd,1)=ro*fr(kd,1)*tpi
          xr(kd,2)=fr(kd,2)/ro*zf
 300    continue
      elseif( abs(ipatch).eq.1 )then
c............Patches at the poles of the sphere
c       ipatch=+1 : top
c       ipatch=-1 : bottom
        sgn=ipatch
        sp1= rp(1)
        sp2= rp(2)*sgn
        s1=sp1*(r(1)-.5)
        s2=sp2*(r(2)-.5)
        ss=s1**2+s2**2
        s=sqrt(ss)
        ro=4.*s/(ss+4.)
        robs=4./(ss+4.)
        zeta=sgn*(4.-ss)/(4.+ss)
        if( s.gt.0. )then
          ct=s1/s
          st=s2/s
        else
          ct=1.
          st=0.
        end if
c..........evaluate cross-section
        call cs( ro,zeta,ct,st, f,fr,rp(5) )

        robsp1=robs*sp1
        robsp2=robs*sp2
        do 100 kd=1,3
          x(kd)=f(kd)
          xr(kd,1)=(-sgn*ct*fr(kd,2)-st*fr(kd,1) )*robsp1
          xr(kd,2)=(-sgn*st*fr(kd,2)+ct*fr(kd,1) )*robsp2
 100    continue
      else
        stop 'CGCS: invalid value for ipatch'
      end if
      return
      end

      subroutine cs1( ro,zeta,ct,st, f,fr,rp )
c===================================================================
c Define Cross Sections: SPHERE
c
c Input -
c  ro,zeta :
c  ct,st   : cos(theta),sin(theta)
c Output -
c   f(3)   : (x,y,z) = (f(1),f(2),f(3))
c   fr(3,2)   :  fr(i,1) = (1/ro) f.theta
c             :  fr(i,2) = ro f1.zeta
c
c===================================================================
      real ro,zeta,ct,st, f(3),fr(3,2),rp(*)

      a=1.
      b=1.
      c=1.

      f(1)=a* ro*ct
      f(2)=b* ro*st
      f(3)=c*zeta
      fr(1,1)=a*(-st)
      fr(2,1)=b* ct
      fr(3,1)=0.
      fr(1,2)=a* ct *(-zeta)
      fr(2,2)=b* st *(-zeta)
      fr(3,2)=c*ro

      return
      end

      subroutine cs2( ro,zeta,ct,st, f,fr,rp )
c===================================================================
c Define Cross Sections: ELLIPSE
c
c Input -
c  ro,zeta :
c  ct,st   : cos(theta),sin(theta)
c  rp()    : (rp(1),rp(2),rp(3)) = (a,b,c) lengths of semi-axes
c            (rp(4),rp(5),rp(6)) = (x,y,z) centre of ellipsoid
c Output -
c   f(3)   : (x,y,z) = (f(1),f(2),f(3))
c   fr(3,2)   :  fr(i,1) = (1/ro) f.theta
c             :  fr(i,2) = ro f1.zeta
c
c===================================================================
      parameter( pi=3.141592653 )
      real ro,zeta,ct,st, f(3),fr(3,2),rp(*)

      a=rp(1)
      b=rp(2)
      c=rp(3)

      f(1)=a* ro*ct  +rp(4)
      f(2)=b* ro*st  +rp(5)
      f(3)=c*zeta    +rp(6)
      fr(1,1)=a*(-st)
      fr(2,1)=b* ct
      fr(3,1)=0.
      fr(1,2)=a* ct *(-zeta)
      fr(2,2)=b* st *(-zeta)
      fr(3,2)=c*ro

      return
      end

      subroutine cs3( ro,zeta,ct,st, f,fr,rp )
c===================================================================
c Define Cross Sections: BANANA
c
c Input -
c  ro,zeta :
c  ct,st   : cos(theta),sin(theta)
c Output -
c   f(3)   : (x,y,z) = (f(1),f(2),f(3))
c   fr(3,2)   :  fr(i,1) = (1/ro) f.theta
c             :  fr(i,2) = ro f1.zeta
c
c===================================================================
      parameter( pi=3.141592653 )
      real ro,zeta,ct,st, f(3),fr(3,2),rp(*)

      a=1.
      b=1.
      c=4.

      f(1)=a* ro*ct
      f(2)=b* ro*st
      f(3)=c*zeta
      fr(1,1)=a*(-st)
      fr(2,1)=b* ct
      fr(3,1)=0.
      fr(1,2)=a* ct *(-zeta)
      fr(2,2)=b* st *(-zeta)
      fr(3,2)=c*ro

c........rotate about x-axis as a function of zeta
c          bw: determines the rate of rotation
c          rotate from -ang to +ang
      bw=1.
      ang=pi*.125
      th=tanh(bw*zeta)
      w =th*ang
      cw=cos(w)
      sw=sin(w)
      cwz=-ang*bw*(1.-th**2)*sw
      swz= ang*bw*(1.-th**2)*cw
      f2=cw*f(2)-sw*f(3)
      f3=sw*f(2)+cw*f(3)
      fr21=cw*fr(2,1)-sw*fr(3,1)
      fr31=sw*fr(2,1)+cw*fr(3,1)
      fr22=cw*fr(2,2)+cwz*f(2)*ro-sw*fr(3,2)-swz*f(3)*ro
      fr32=sw*fr(2,2)+swz*f(2)*ro+cw*fr(3,2)+cwz*f(3)*ro
      fr(2,1)=fr21
      fr(3,1)=fr31
      fr(2,2)=fr22
      fr(3,2)=fr32
      f(2)=f2
      f(3)=f3
      return
      end

      subroutine cs4( ro,zeta,ct,st, f,fr,rp )
c===================================================================
c Define Cross Sections: Joukowsky
c
c Input -
c  ro,zeta :
c  ct,st   : cos(theta),sin(theta)
c Output -
c   f(3)   : (x,y,z) = (f(1),f(2),f(3))
c   fr(3,2)   :  fr(i,1) = (1/ro) f.theta
c             :  fr(i,2) = ro f1.zeta
c
c===================================================================
      real ro,zeta,ct,st, f(3),fr(3,2),rp(*)
c.........local
      complex dz,z,zpz,am,z1,z2,i,cexpt
      parameter(i=(0.,1.))
      logical first
      save
c.........start statement functions
      fz(xx)=(xx**2-1.)**2
      fzp(xx)=4.*xx*(xx**2-1.)
c.........end statement functions
      data first/.true./

      if(first)then
        first=.false.
        pi=4.*atan(1.)
        d=.15
        a=.8
        delta=15.
        wl=5.
        beta=1.
        delta=delta*2.*pi/360.
        am=i*d*cexp(i*delta)
      end if
      cexpt=ct+i*st
      zpz=a*cexpt+am*fz(zeta)
      z=zpz+1./zpz
      dz=1.-1./(zpz*zpz)
      z1=a*i*cexpt*dz
      z2=fzp(zeta)*am*dz
      th=tanh(beta*ro)
      thp=beta*(1.-th**2)
c        thro=tanh(beta*ro)/ro
      if( ro.ne.0. )then
        thro=th/ro
      else
        thro=beta
      end if
      f(1)=wl*zeta
      f(2)= real(z)*th
      f(3)=aimag(z)*th
      fr(1,1)=0.
      fr(2,1)= real(z1)  *thro
      fr(3,1)=aimag(z1)  *thro
      fr(1,2)=wl         *ro
      fr(2,2)= real(z2)*th*ro +  real(z)*thp*(-zeta)
      fr(3,2)=aimag(z2)*th*ro + aimag(z)*thp*(-zeta)

      return
      end

      subroutine cs5( ro,zeta,ct,st, f,fr,rp )
c===================================================================
c Define Cross Sections
c
c Input -
c  ro,zeta :
c  ct,st   : cos(theta),sin(theta)
c Output -
c   f(3)   : (x,y,z) = (f(1),f(2),f(3))
c   fr(3,2)   :  fr(i,1) = (1/ro) f.theta
c             :  fr(i,2) = ro f1.zeta
c
c===================================================================
      real ro,zeta,ct,st, f(3),fr(3,2),rp(*)
c.........local
      real f0(3)
      complex dz,z,zpz,am,z1,z2,i,cexpt
      parameter(i=(0.,1.))
      logical first
      save
      data first/.true./

      if(first)then
        first=.false.
        pi=4.*atan(1.)

        d=.15
c1      a=.8
        a=.85
        delta=15.
        wl=10.

c2      d=.15
c2      a=.9
c2      delta=15.
c2      wl=15.

        beta=2.
        th=tanh(beta*1.)
        thbp=beta*(1.-th**2)
        delta=delta*2.*pi/360.
        am=i*d*cexp(i*delta)
      end if


c*wdh cexpt=ct+i*st
      cexpt=ct-i*st

      th=tanh(beta*ro)
      thp=beta*(1.-th**2)
c        thro=tanh(beta*ro)/ro
      if( ro.ne.0. )then
        thro=th/ro
      else
        thro=beta
      end if
c........fz(ro) = ( g(ro) - ro * g'(1) )**2
c        fzp = fz.zeta = fz.ro (-zeta/ro)
c         g(ro) = tanh( beta*ro )
      fz=(th-thbp*ro)**2
      fzp=2.*(thp-thbp)*(thro-thbp)*(-zeta)

      zpz=a*cexpt+am*fz
      z=zpz+1./zpz
      dz=1.-1./(zpz*zpz)
c*wdh z1=a*i*cexpt*dz
      z1=-a*i*cexpt*dz
      z2=fzp*am*dz
      f(1)=wl*zeta
      f(2)= real(z)*th
      f(3)=aimag(z)*th
      fr(1,1)=0.
      fr(2,1)= real(z1)  *thro
      fr(3,1)=aimag(z1)  *thro
      fr(1,2)=wl         *ro
      fr(2,2)= real(z2)*th*ro +  real(z)*thp*(-zeta)
      fr(3,2)=aimag(z2)*th*ro + aimag(z)*thp*(-zeta)


      if( .true. ) return

c........define axis to rotate about:
      ir1=1
      ir2=3
      f0(1)=.8*wl*sign(1.,zeta)
      f0(2)=0.
      f0(3)=.5*wl

      bw=3.
      ang=pi*.25*sign(1.,zeta)
      th=tanh(bw*ro)
      w =ang*(1.-th)
      cw=cos(w)
      sw=sin(w)
c          cwzro = ro * cos(w).zeta
      cwzro=-ang*bw*(1.-th**2)*sw*zeta
      swzro= ang*bw*(1.-th**2)*cw*zeta
      f10=f(ir1)-f0(ir1)
      f20=f(ir2)-f0(ir2)

      f2=cw*f10-sw*f20  + f0(ir1)
      f3=sw*f10+cw*f20  + f0(ir2)
      fr21=cw*fr(ir1,1)-sw*fr(ir2,1)
      fr31=sw*fr(ir1,1)+cw*fr(ir2,1)
      fr22=cw*fr(ir1,2)+cwzro*f10-sw*fr(ir2,2)-swzro*f20
      fr32=sw*fr(ir1,2)+swzro*f10+cw*fr(ir2,2)+cwzro*f20
      fr(ir1,1)=fr21
      fr(ir2,1)=fr31
      fr(ir1,2)=fr22
      fr(ir2,2)=fr32
      f(ir1)=f2
      f(ir2)=f3


      return
      end

      subroutine cs6( ro,zeta,ct,st, f,fr,rp )
c==============================================================
c    Define a surface for CMPGRD by cross sections
c    ---------------------------------------------
c
c  Define an M6 wing by data points.
c
c Input -
c   ro,zeta,ct,st: ro,zeta,ct=cos(theta),st=sin(theta)
c Output -
c   f(i)      :  as defined above
c   fr(3,2)   :  fr(i,1) = (1/ro) f.theta
c             :  fr(i,2) = ro f1.zeta
c
c ncs = number of cross-sections
c ns  = number of points per cross-section
c cross-sections are parameterized by z(j) j=1,...,ncs
c==============================================================
      real zeta,ro,ct,st,f(3),fr(3,2),rp(*)
c.......local
      parameter( ndra=1,ndrb=51,ndsa=1,ndsb=21 )
      real xc(ndsa:ndsb),sc(ndsa:ndsb),xcs(ndsa:ndsb),
     &  xy(ndra:ndrb,ndsa:ndsb,3),
     &  sp(ndra:ndrb),bcd(3,ndra:ndrb,ndsa:ndsb,2:3),
     &  sx(3,0:3),sxr(3,0:3),xy0(3),ab(3),ft(3)

      logical first
      save
      data first/.true./

      if( first )then
        first=.false.
        tpi=8.*atan(1.)
        call cgopen(10,'cggcs.dat','old','formatted',ierr)
        read(10,*) nsb,nrb
        if( nrb.gt.ndrb .or. nsb.gt.ndsb ) then
           stop 'CGGCS: Dimension error, nrb or nsb'
        end if
        nra=1
        nsa=1
        read(10,*) (xc(j),(xy(i,j,2),i=nra,nrb),(xy(i,j,3),i=nra,nrb),
     &        j=nsa,nsb)
        close(10)

c.........determine normalized parameterization for different
c         cross-sections
        xmn=xc(1)
        xmx=xc(nsb)
        do 100 j=nsa,nsb-1
          if( xc(j).ge.xc(j+1) )then
            write(6,*) 'DS3: ERROR xc(j) not in ascending order'
            stop 'DS3'
          end if
          sc(j)=(xc(j)-xmn)/(xmx-xmn)
 100    continue
        sc(nsb)=1.
c.........extrapolate the last cross-section

        write(6,*) '***Before extrapolation***'
        write(*,9000) (i,(xy(i,j,2),xy(i,j,3),j=nsb-2,nsb),i=nra,nrb)
 9000   format(1x,' i    xy(i,nsb-2,2:3)     xy(i,nsb-1,2:3)   ',
     &   '      xy(i,nsb,2:3)',/,(1x,i3,3(2f10.4,3x)) )

        az=(xc(nsb)-xc(nsb-1))/(xc(nsb-1)-xc(nsb-2))
        do 200 kd=2,3
          do 200 i=nra,nrb
            xy(i,nsb,kd)=xy(i,nsb-1,kd)+
     &                  (xy(i,nsb-1,kd)-xy(i,nsb-2,kd))*az
          continue
 200    continue

        write(*,*) '***After extrapolation***'
        write(*,9000) (i,(xy(i,j,2),xy(i,j,3),j=nsb-2,nsb),i=nra,nrb)
c..........Fit an ellipse to the last cross-section
        xy0(1)=0.
        do 350 kd=2,3
          xy0(kd)=0.
          do 300 i=nra,nrb-1
            xy0(kd)=xy0(kd)+xy(i,nsb,kd)
 300      continue
          xy0(kd)=xy0(kd)/(nrb-nra)
          do 350 i=nra,nrb-1
            ab(kd)=max(abs(xy(i,nsb,kd)-xy0(kd)),ab(kd))
          continue
 350    continue
c        write(6,*) ' old a,b =',ab(2),ab(3)
cc..........change the apsect ration of the last ellipse
c        ab(2)=min(ab(2),ab(3)*2.)
c        write(6,*) ' new a,b =',ab(2),ab(3)
c        do 400 i=nra,nrb
c          ss=(i-nra)/real(nrb-nra)
c          xy(i,nsb,2)=xy0(2)+ab(2)*cos(tpi*ss)
c          xy(i,nsb,3)=xy0(3)-ab(3)*sin(tpi*ss)
c 400    continue
c        write(*,*) '***After ellipsizing***'
c        write(*,9000) (i,(xy(i,j,2),xy(i,j,3),j=nsb-2,nsb),i=nra,nrb)

c...........fit a spline to each cross section
        do 500 i=nra,nrb
          sp(i)=(i-nra)/real(nrb-nra)
 500    continue
        n=nrb-nra+1
        iopt=0
        do 550 j=nsa,nsb
          do 550 kd=2,3
            call  csgen ( n,sp,xy(1,j,kd),bcd(1,1,j,kd),iopt  )
          continue
 550    continue

        do 650 i=nra,nrb
          do 600 j=nsa+1,nsb-1
            xcs(j)=(xc(j+1)-xc(j-1))/(sc(j+1)-sc(j-1))
 600      continue
          xcs(nsa)=(xc(nsa+1)-xc(nsa  ))/(sc(nsa+1)-sc(nsa  ))
          xcs(nsb)=(xc(nsb  )-xc(nsb-1))/(sc(nsb  )-sc(nsb-1))
 650    continue


      end if

c**   t=atan2(st,ct)/tpi+.5
c1    t=atan2(-st,-ct)/tpi+.5
      t=atan2(-st,-ct)/tpi+.5
      t=1.-t
      tp=-1.

c*wdh beta=2.  ! worked
c*wdh beta=4.  ! ""
c*wdh beta=10. ! yes
      beta=8.
      th=tanh(beta*ro)
      thp=beta*(1.-th**2)
c        thro=tanh(beta*ro)/ro
      if( ro.ne.0. )then
        thro=th/ro
      else
        thro=beta
      end if

c........find appropriate cross-section to use
c*******do better here
      do 700 j=nsa+1,nsb
        if( zeta.le.sc(j) )then
          jj=j-1
          goto 750
        end if
 700  continue
      jj=nsb-1
 750  continue
      b=(zeta-sc(jj))/(sc(jj+1)-sc(jj))

c.......evaluate splines at cross-sections jj-1,jj,jj+1,jj+2
      do 800 j=max(nsa,jj-1),min(nsb,jj+2)
        do 800 kd=2,3
          call cseval(n, sp,xy(1,j,kd),bcd(1,1,j,kd),t,sx(kd,j-jj+1),
     &             sxr(kd,j-jj+1) )
        continue
 800  continue

      f(1)=xc(jj)*(1.-b)+xc(jj+1)*b
      ft(2)=sx(2,1)*(1.-b)+sx(2,2)*b-xy0(2)
      ft(3)=sx(3,1)*(1.-b)+sx(3,2)*b-xy0(3)
      f(2)=ft(2)*th
      f(3)=ft(3)*th

      fr(1,1)=0.
      fr(2,1)=(sxr(2,1)*(1.-b)+sxr(2,2)*b)*thro/tpi*tp
      fr(3,1)=(sxr(3,1)*(1.-b)+sxr(3,2)*b)*thro/tpi*tp

      fr(1,2)= (xcs(jj)*(1.-b)+xcs(jj+1)*b)*ro
      do 900 kd=2,3
        if( jj.gt.nsa .and. jj.lt.nsb-1 )then
          fr(kd,2)=((sx(kd,2)-sx(kd,0))/(sc(jj+1)-sc(jj-1))*(1.-b)+
     &              (sx(kd,3)-sx(kd,1))/(sc(jj+2)-sc(jj  ))*b)*th*ro
     &             + ft(kd)*thp*(-zeta)
        elseif( jj.gt.nsa )then
          fr(kd,2)=((sx(kd,2)-sx(kd,0))/(sc(jj+1)-sc(jj-1))*(1.-b)+
     &              (sx(kd,2)-sx(kd,1))/(sc(jj+1)-sc(jj  ))*b)*th*ro
     &             + ft(kd)*thp*(-zeta)
        else
          fr(kd,2)=((sx(kd,2)-sx(kd,1))/(sc(jj+1)-sc(jj  ))*(1.-b)+
     &              (sx(kd,3)-sx(kd,1))/(sc(jj+2)-sc(jj  ))*b)*th*ro
     &             + ft(kd)*thp*(-zeta)
        end if
 900  continue
      return
      end

      subroutine cs7( ro,zeta,ct,st, f,fr,rp )
c==============================================================
c    Define a surface for CMPGRD by cross sections
c              SMOOTHED VERSION
c
c Purpose-
c  This surface uses smoothed versions of the cross-sections which are
c  extended in the normal direction to define an outer surface for
c  the airfoil.
c
c Representation: x=f(theta,zeta)
c          f_1 = (x0e(zeta)+ae(zeta)*cos(theta))*tanh(beta*ro)
c          f_2 = (y0e(zeta)+be(zeta)*sin(theta))*tanh(beta*ro)
c          f_3 = ze(zeta)
c The functions x0e(r), y0e(r), ae(r), be(r) and ze(r) are defined
c by splines. (x0e,y0e) passes through the centres of the ellipses
c and (ae,be) are the lengths of the major and minor axes.
c t(s) is a stretched version of s which puts more
c points at the narrow ends of the ellipse
c
c Input -
c   ro,zeta,ct,st: ro,zeta,ct=cos(theta),st=sin(theta)
c Output -
c   f(i)      :  as defined above
c   fr(3,2)   :  fr(i,1) = (1/ro) f.theta
c             :  fr(i,2) = ro f1.zeta
c
c Internal variables
c
c ncs = number of cross-sections
c ns  = number of points per cross-section
c cross-sections are parameterized by zc(j) j=1,...,ncs
c==============================================================
      real f(3),fr(3,2),rp(*)
c.......local
      parameter( ncsd=21,nsd=51 )
      real xc(nsd,ncsd), yc(nsd,ncsd),zc(ncsd),sc(ncsd),
     &  x0e(ncsd),y0e(ncsd),ae(ncsd),be(ncsd),ze(ncsd),
     &  bcdx0(3,ncsd),bcdy0(3,ncsd),bcda(3,ncsd),
     &  bcdb(3,ncsd), bcdz(3,ncsd)
c........for plots
*       parameter( ndp=200 )
*       real xp(ndp),yp(ndp,5)

      logical first
      save
      data first/.true./

      if( first )then
        first=.false.
c..........read in cross-section data
        call cgopen(10,'cggcs.dat','old','formatted',ierr)
        read(10,*) ncs,ns
        if( ncs.gt.ncsd .or. ns.gt.nsd ) then
           stop 'CS7: Dimension error, ncs or ns'
        end if
        read(10,*) (zc(j),(xc(i,j),i=1,ns),(yc(i,j),i=1,ns),j=1,ncs)
        close(10)

c.........determine sc(j): normalized parameterization for different
c         cross-sections
        zmn=zc(1)
        zmx=zc(ncs)
        do 100 j=1,ncs-1
          if( zc(j).ge.zc(j+1) )then
            write(6,*) 'CS7: ERROR zc(j) not in ascending order'
            stop 'CS7'
          end if
          sc(j)=(zc(j)-zmn)/zmx
 100    continue
        sc(ncs)=1.

c..........Fit an ellipse to each cross section
        do 300 j=1,ncs
          x0e(j)=0.
          y0e(j)=0.
          do 200 i=1,ns
            x0e(j)=x0e(j)+xc(i,j)
            y0e(j)=y0e(j)+yc(i,j)
 200      continue
          x0e(j)=x0e(j)/ns
          y0e(j)=y0e(j)/ns
          ae(j)=abs(xc(1,j)-x0e(j))
          be(j)=abs(yc(1,j)-y0e(j))
          do 250 i=1,ns
            ae(j)=max(abs(xc(i,j)-x0e(j)),ae(j))
            be(j)=max(abs(yc(i,j)-y0e(j)),be(j))
 250      continue

          ze(j)=zc(j)
 300    continue

      write(*,9200) (j,x0e(j),y0e(j),zc(j),ae(j),be(j),j=1,ncs)
 9200 format(1x,' j        x0e        y0e         zc      ',
     &       '    ae          be',/,(1x,i3,1x,5(1x,f12.6)) )
c..........boundary conditions
c         extrapolate last ellipse -- fudge --
        j=ncs
        if( .true. )then
          az=(zc(j)-zc(j-1))/(zc(j-1)-zc(j-2))
          ae(j)=ae(j-1)+(ae(j-1)-ae(j-2))*az
          be(j)=be(j-1)+(be(j-1)-be(j-2))*az
          x0e(j)=x0e(j-1)+(x0e(j-1)-x0e(j-2))*az
          y0e(j)=y0e(j-1)+(y0e(j-1)-y0e(j-2))*az
        else
          az=0.
          ae(j)=ae(j-1)+(ae(j-1)-ae(j-2))*az
          be(j)=be(j-1)+(be(j-1)-be(j-2))*az
          x0e(j)=x0e(j-1)+(x0e(j-1)-x0e(j-2))*az
          y0e(j)=y0e(j-1)+(y0e(j-1)-y0e(j-2))*az
        end if
      write(*,*) '***After changing last ellipse:'
      write(*,9200) (j,x0e(j),y0e(j),zc(j),ae(j),be(j),j=1,ncs)
c........create splines
        n=ncs
        iopt=0
        call  csgen (n, sc,  ae, bcda , iopt  )
        call  csgen (n, sc,  be, bcdb , iopt  )
        call  csgen (n, sc, x0e, bcdx0, iopt  )
        call  csgen (n, sc, y0e, bcdy0, iopt  )
        call  csgen (n, sc,  ze, bcdz , iopt  )

        x0=x0e(ncs)
        y0=y0e(ncs)

* c...........plots:
*         r2a=.9
*         r2b=1.1
*         do 400 i=1,ndp
*           r2=r2a+(r2b-r2a)*real(i-1)/(ndp-1)
*           xp(i)=r2
*           call cseval(n, sc, ae , bcda , r2, yp(i,1), asp   )
*           call cseval(n, sc, be , bcdb , r2, yp(i,2), bsp   )
*           call cseval(n, sc, x0e, bcdx0, r2, yp(i,3), x0sp  )
*           call cseval(n, sc, y0e, bcdy0, r2, yp(i,4), y0sp  )
*           call cseval(n, sc, ze , bcdz , r2, yp(i,5), zsp   )
*  400    continue
*         call ncarb
*         call set( 0.,1.,0.,1., 0.,1.,0.,1.,1 )
* c          use alphabetic dashes
*         call agseti('DASH/SELECTOR.',-1)
* c..........use background grid
* c**     call agseti( 'GRID/SELECTOR.',1 )
*         mp=5
*         call ezmxy( xp,yp,ndp,mp,ndp,'$' )
*         line='"PRL"ae be x0e y0e ze'
*         call pwritx( 512,900,line,nblank(line),21,0,0 )
*         call frame
*         call ncarnd

      end if

      beta=2.
      th=tanh(beta*ro)
      thp=beta*(1.-th**2)

c        thro=tanh(beta*ro)/ro
      if( ro.ne.0. )then
        thro=th/ro
      else
        thro=beta
      end if

c.........evaluate splines and their derivatives
      r2=zeta
      call cseval(n, sc, ae , bcda , r2, as , asp   )
      call cseval(n, sc, be , bcdb , r2, bs , bsp   )
      call cseval(n, sc, x0e, bcdx0, r2, x0s, x0sp  )
      call cseval(n, sc, y0e, bcdy0, r2, y0s, y0sp  )
      call cseval(n, sc, ze , bcdz , r2, zs , zsp   )

c        efact= increase minor axis by this factor
c*wdh efact=rp(2)
      efact=2.
      bs =bs *efact
      bsp=bsp*efact

      f(1)=zs
      f(2)=(x0s+as*ct   -x0)*th
      f(3)=(y0s+bs*st   -y0)*th

      fr(1,1)=0.
      fr(2,1)=-as*st*thro
      fr(3,1)= bs*ct*thro
      fr(1,2)=zsp          *ro
      fr(2,2)=(x0sp+asp*ct)*th*ro + (x0s+as*ct   -x0)*thp*(-zeta)
      fr(3,2)=(y0sp+bsp*st)*th*ro + (y0s+bs*st   -y0)*thp*(-zeta)

      return
      end

      subroutine cgcvn( ipatch,t,x,xt,dn,iop,rop,cs )
c=================================================================
c        Create a new curve by extending normals
c          ...for curves generated by CGCS
c
c Input -
c  dn : normal distance to move
c  cs,iop,rop : cross-section routine and parameters for cgcs
c
c  NOTE: currently the routine projects the normals near zeta=t(2)=0
c        onto the y-z plane.
c=================================================================
      real t(*),x(*),xt(3,*),dn,rop(*)
      integer ipatch,iop(*)
      external cs
c.......local
      real r(3),tt(3),xtt(3,2),xx(3,4)
      save


c*wdh      h=(20.*d1mach(4))**(1./5.)
      h=(20.*r1mach(4))**(1./5.)
      r(1)=t(1)
      r(2)=t(2)
      t3= 1.
      call cgcs( ipatch,cs,r,x,xtt,iop,rop,ierr )
      xn1=xtt(2,1)*xtt(3,2)-xtt(3,1)*xtt(2,2)
      xn2=xtt(3,1)*xtt(1,2)-xtt(1,1)*xtt(3,2)
      xn3=xtt(1,1)*xtt(2,2)-xtt(2,1)*xtt(1,2)
c      project the normals
      call cggrdp( t(2),xn1,xn2,xn3,dn,rpar )
      x(1)=x(1)+xn1
      x(2)=x(2)+xn2
      x(3)=x(3)+xn3
      tt(1)=r(1)
      tt(2)=r(2)
      do 300 kdd=1,2
        do 100 i=1,4
          tt(kdd)=r(kdd)+.5*(2*i-5)*h
          call cgcs( ipatch,cs,tt,xx(1,i),xtt,iop,rop,ierr )
          xn1=xtt(2,1)*xtt(3,2)-xtt(3,1)*xtt(2,2)
          xn2=xtt(3,1)*xtt(1,2)-xtt(1,1)*xtt(3,2)
          xn3=xtt(1,1)*xtt(2,2)-xtt(2,1)*xtt(1,2)
c            project normals onto y-z plane for points near zeta=0
          call cggrdp( tt(2),xn1,xn2,xn3,dn,rpar )
          xx(1,i)=xx(1,i)+xn1
          xx(2,i)=xx(2,i)+xn2
          xx(3,i)=xx(3,i)+xn3
 100    continue
        do 200 kd=1,3
          xt(kd,kdd)=
     &     (27.*(xx(kd,3)-xx(kd,2))-(xx(kd,4)-xx(kd,1)))/(24.*h)
 200    continue
        tt(kdd)=r(kdd)
 300  continue

      return
      end
      subroutine cggrdp( r,xn1,xn2,xn3,dn,rpar )
c==============================================================
c Project the normals
c   n =
c==============================================================
      real rpar(*)
      save

      an1=1.
      an2=0.
      an3=0.
c.......projected normal
      adx=an1*xn1+an2*xn2+an3*xn3
      yn1=xn1-adx*an1
      yn2=xn2-adx*an2
      yn3=xn3-adx*an3

c.......blend normal
      beta=6.
      th=tanh(beta*r)
      xn1=(1.-th)*yn1+th*xn1
      xn2=(1.-th)*yn2+th*xn2
      xn3=(1.-th)*yn3+th*xn3

      xn=dn/sqrt(xn1**2+xn2**2+xn3**2)
      xn1=xn1*xn
      xn2=xn2*xn
      xn3=xn3*xn
      return
      end
