      subroutine geteosm (rho,e,alam,p,dp,ideriv,ier)

c geteosm computes either p(rho,rho*e,rho*alam) and derivatives or e(rho,p,alam), where
c p=pressure, e is the internal energy per unit mass, alam=mass fraction of material "r"
c and rho=density.
 
c I/O: if ideriv<0 compute se given rho,alam,p
c         ideriv=0 compute p given rho,alam,se (no derivatives of p computed)
c         ideriv>0 compute p and derivatives dp given rho,alam,se

c      if istiff=1 use temperature equilibrium set without stiffening pressures (MO version)
c         istiff=2 use temperature equilibrium set without stiffening pressures (DWS version)
c         istiff=3 use temperature equilibirum set with stiffening pressures (DWS version)

      implicit real*8 (a-h,o-z)
      dimension dp(3),ppartials(4),da(3),db(3),dc(3)

      include 'multiDat.h'
c      common / muldat / gami, gamr, cvi, cvr, pii, pir, istiff

      ier=0

c      write(6,*)'geteosm'
c      write(6,*)rho,e,alam,p,dp,ideriv,ier
c      write(6,*)' '
c      write(6,*)gami, gamr, cvi, cvr, pii, pir, istiff
c      pause

      if (istiff.eq.1) then

        method=1
        if (ideriv.lt.0) then
          idir=1
          ppartials(1)=p
          call getmixeos_2 (rho,e,ppartials,alam,idir,method,ier)
        else
          if (ideriv.eq.0) then
            idir=2
            call getmixeos_2 (rho,e,ppartials,alam,idir,method,ier)
            p=ppartials(1)
          else
            idir=3
            call getmixeos_2 (rho,e,ppartials,alam,idir,method,ier)
            p=ppartials(1)
            dp(1)=ppartials(2)            ! dp/d(rho)
            dp(2)=ppartials(3)            ! dp/d(rho*e)
            dp(3)=ppartials(4)            ! dp/d(rho*alam)
          end if
        end if

      elseif (istiff.eq.2) then

        si=gami*pii
        sr=gamr*pir
        wi=cvi*(gami-1.d0)
        wr=cvr*(gamr-1.d0)
        amu=1.d0-alam
        if (ideriv.lt.0) then 
          e=(amu*cvi*(p+si)+alam*cvr*(p+sr))
     *      /(rho*(amu*wi+alam*wr))
        else
          z=rho*e
          gi=z*wi-cvi*si
          gr=z*wr-cvr*sr
          fact=1.d0/(amu*cvi+alam*cvr)
          p=fact*(amu*gi+alam*gr)
          if (ideriv.gt.0) then
            temp=cvr*gi-cvi*gr
            dp(1)=alam*temp*fact**2/rho            ! dp/d(rho)
            dp(2)=fact*(amu*wi+alam*wr)            ! dp/d(rho*e)
            dp(3)=    -temp*fact**2/rho            ! dp/d(rho*alam)
          end if
          p=ppartials(1)
        end if

      elseif (istiff.eq.3) then

        if (ideriv.lt.0) then
          pfact1=(p+gami*pii)/(gami-1.d0)
          pfact2=(p+gamr*pir)/(gamr-1.d0)
          afact1=alam/cvi
          afact2=(1.d0-alam)/cvr
          e=pfact1*pfact2*(afact1+afact2)
     *      /(rho*(afact1*pfact1+afact2*pfact2))
        else
          x=rho
          y=rho*alam
          z=rho*e
          gfact1=1.d0/(gami-1.d0)
          gfact2=1.d0/(gamr-1.d0)
          cfact1=y/cvi
          cfact2=(x-y)/cvr
          pfact1=pii*gami
          pfact2=pir*gamr
          efact1=pfact1*gfact1
          efact2=pfact2*gfact2
          a=gfact1*gfact2*(cfact1+cfact2)
          b=a*(pfact1+pfact2)-z*(gfact1*cfact1+gfact2*cfact2)
          c=a*pfact1*pfact2-z*(efact1*cfact1+efact2*cfact2)
          test=b*b-4.d0*a*c
          if (test.lt.0.d0) then
            write(6,*)'Error (geteosm) : no real solution for p'
            write(6,*)'rho,energy,alam =',rho,e,alam
            stop
          end if
          if (b.le.0.d0) then
            p=(-b+dsqrt(test))/(2.d0*a)
          else
            p=-2.d0*c/(b+dsqrt(test))
          end if
          if (ideriv.gt.0) then
            test=2.d0*a*p+b
            if (test.le.0.d0) then
              write(6,*)'Error (geteosm) : cannot compute deriv.s'
              stop
            end if
            da(1)=gfact1*gfact2/cvr
            da(2)=0.d0
            da(3)=gfact1*gfact2*(1.d0/cvi-1.d0/cvr)
            db(1)=da(1)*(pfact1+pfact2)-z*gfact2/cvr
            db(2)=-(gfact1*cfact1+gfact2*cfact2)
            db(3)=da(3)*(pfact1+pfact2)-z*(gfact1/cvi-gfact2/cvr)
            dc(1)=da(1)*pfact1*pfact2-z*efact2/cvr
            dc(2)=-(efact1*cfact1+efact2*cfact2)
            dc(3)=da(3)*pfact1*pfact2-z*(efact1/cvi-efact2/cvr)
            do i=1,3
              dp(i)=-(dc(i)+p*(db(i)+p*da(i)))/test
            end do
          end if
        end if

      else

        write(6,*)'Error (geteosm) : istiff value not supported'
        write(6,*)'istiff =',istiff
        pause

      end if

      return
      end
