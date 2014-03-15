c
c $Header: /usr/gapps/overture/OvertureRepoCVS/overture/Overture/mapping/strtch.f,v 1.7 2004/02/15 16:58:32 henshaw Exp $
c
cTeX begin \listing strtch.ftex
c====================================================================
c           Stretching Routines
c           -------------------
c This file contains:
c  STINIT : Initialization routine
c  STTR    : t --> r
c  STRT    : r --> t
c
c Description:
c  These routines can be used to evaluate the function r=R(t),
c  its first derivative and possibly the inverse function
c  t=T(r)=R**(-1)(r) and its first derivative. The stretching
c  function R(t) is defined as a sum of tanh's and integrals
c  of tanh's (log(cosh)):
c
c                     nu
c   r = R(t) = [ t + SUM ( U(t,i)-U(0,i) )
c                    i=1    nv
c                        + SUM ( V(t,j)-V(0,j) ) ]*r1 + r0
c                          j=1
c where
c
c   U(t,i) = (a(i)/2) * tanh b(i)*(t-c(i))
c
c                            cosh e(j)*(t-f(j))
c   V(t,j) = (d(j)-1) * log( ------------------- ) /(e(j)*2)
c                            cosh e(j)*(t-f(j+1))
c with
c   b(j) > 0 j=1,..,nu   0 =< c(j) =< 1  j=1,..,nu
c   e(j) > 0 j=1,..,nv   f(1) <= 1 f(nv) >= 0  0 =< f(j) =< 1 j=2,..,nv-1
c                        f(1) < f(2) < f(3) ... < f(nv)
c
c  Parameters:
c
c   r0 and r1 are used to normalize the function to the desired
c   range; The user may either specify r0 and r1 or the user may specify
c   the boundary conditions on R, ra and rb, so that:
c                 R(t=0)=ra   R(t=1)=rb
c
c   The function U(t,i) is a hyperbolic tangent which is centred at
c   t=c(i) and asymptotes to -a(i)/2 or a(i)/2. As b(i) tends to infinity
c   the function U tends toward a step function.
c                              +------------- a(i)/2
c         .....................|........................... t
c                              |
c        -a(i)/2  -------------+
c                             c(i)
c
c   The function V(t,j) is a smoothed out ramp function with
c   transitions at f(j) and f(j+1). The slope of the ramp is d(j)-1.
c   Thus d(j) indicates the relative slope of the ramp compared to
c   the linear term "t" which appears in R(t). That is if d(j)=2
c   then the slope of R(t) between f(j) and f(j+1) will be approximately
c   twice the slope of the region where the linear term is dominant.
c   A sloped region can be made to extend past t=0 or t=1 (so that t=0
c   or t=1 is in the middle of the sloped region) by choosing
c   f(1)<0 or f(nv+1)>1. A reasonable value might be f(1)=-.5 or
c   f(nv+1)=1.5
c                            /--------------
c                           /
c       .................../.............................. t
c                         /
c          --------------/
c                      f(j)  f(j+1)
c Periodic Case
c  When R(t) is specified to be "periodic" (in the call to STINIT)
c  instead of the U(t,i) and V(t,j) given above the program
c  uses Up(t,i) and Vp(t,j) given by
c              +inf                             +inf
c   Up(t,i) =  SUM   U(t+k,i)        Vp(t,i) =  SUM   V(t+k,i)
c             k=-inf                           k=-inf
c
c  These functions are not really periodic, but their derivative
c  with respect to t is periodic with period 1.
c
c Invertible Case
c  If one requires the transformation to be invertible (if for example
c   the transformation is used as a change of variables) then the
c   parameters a(j) and d(j) should satisfy
c              a(j) > 0  and  d(j) > 0
c
c Change of Variables:
c  Here are some remarks which might be useful when one is using the
c  stretching functions for a change of variables. Typically the
c  variable r refers to a uniform grid while t refers to a grid which
c  has been stretched so that points are clustered in certain locations.
c
c              x  x  x  x  x  x  x  x  x   ---> r
c              x   x  x xxx x  x   x   x   ---> t
c
c  The clustering of points can be done in two ways. Using the U(t,i)
c  functions (tanh's) the point spacing  can be made to decrease
c  exponentially to a minimum spacing located at c(i). The value of b(i)
c  determines how small the spacing can get. Roughly speaking a value
c  of b(i)=10. means the spacing will be about 10 times smaller at the
c  center of the layer. The relative number of points in this
c  stretched region is proportional to a(i).
c  The linear term t appearing in the definition of R(t) has a weight
c  of 1 so that if there is only one term U(t,i), the relative
c  number of points in the layer is essentially
c                  a(1) / (1+a(1)).
c  Thus if nu=1 (nv=0) and a(1)=1., then half the points will be in the
c  stretched layer. For two layers the relative number of points in
c  layer i (i=1 or i=2) would be
c                  a(i) / (1+a(1)+a(2)).
c    The functions V(t,i) (log(cosh/cosh)) allow one to have intervals
c  where the grid point spacing is relatively smaller or larger than the
c  grid spacing in the region where the linear term t is dominant.
c  In each interval the grid spacing is nearly constant, except near
c  the transition points f(i) and f(i+1).
c  The parameter d(i) denotes the relative grid spacing in
c  each interval.  For example to have the grid spacing twice as fine
c  for t between .25 and .5 one would specify f(1)=.25, f(2)=.5 and
c  d(1)=2.  As another example, to have the spacing 5 times smaller
c  for t between 0. and .5 one could say f(1)=-.5, f(2)=.5 and
c  d(1)=5. Assigning the first transition point a value less than zero,
c  f(1)=-.5, means that t=0 will be in the middle of the interval where
c  the spacing will be 5 times smaller.  (If instead f(1)=0. then
c  near t=0. the spacing would be in transition to the default
c  relative grid spacing of 1). The parameters e(i) denote how
c  rapid the transition is from one spacing to another. A reasonable
c  value for e(i) might be 10. or 20.
c
c====================================================================
cTeX end
c*wdh debug subchk
c*wdh end debug
cTeX begin \listing stinit.ftex
      subroutine stinit( ndi,iw,ndr,rw,iopt,ndwk,wk,ierr )
c=====================================================================
c       Initialize the Stretching Routines
c       ----------------------------------
c PURPOSE -
c   Initialize the stretching routines. Input the coefficients which
c   define the stretching function.
c
c INPUT -
c   ndi : dimension of the array iw.
c         ndi >= 12 if not periodic (iw(3)=0)
c         ndi >= 12+(nu+nv)*2 if periodic (iw(3)<>0)
c   iw : integer array for passing parameters and work space
c    iw(1) = nu = number of different U(t,i) (tanh's)
c    iw(2) = nv = number of different V(t,i) (log(cosh/cosh))
c    iw(3) = 0 : transformation not periodic
c          = 1 : transformation periodic
c    iw(4) = nsp: number of points on the spline (see iopt and remarks).
c            if a spline is to be computed and iw(4) = 0 then the
c            routine chooses the number of points by choosing as many
c            points as possible with the given work space, rw and wk.
c            if nsp > 0 is specified then it must be > 5.
c   ndr : dimension of the array rw.
c        ndr >= 3*(nu+nv+1)+4+nv+nsp*4 if not periodic (iw(3)=0)
c           nsp=number of spline points, nsp=0 if no spline is created
c        ndr >=3*(nu+nv+1)+4+nv+nsp*4+extra if periodic (iw(3)<>0)
c           extra space is needed which depends on values of b(i),e(i).
c           Usually nu+nv <= extra <= 2*(nu+nv)
c   rw : real array for passing parameters and work space
c        This array contains the parameters for U(t,i) and V(t,i) in the
c        order :
c              a(1),b(1),c(1),a(2),b(2),c(2),...,a(nu),b(nu),c(nu),
c              d(1),e(1),f(1),d(2),e(2),f(2),...,d(nv),e(nv),f(nv),
c              rar0,rbr1,f(nv+1)
c        Or in other words:
c
c     rw(3*i-2),rw(3*i-1),rw(3*i) = a(i),b(i),c(i) i=1,2,...,nu
c     rw(3*i-2),rw(3*i-1),rw(3*i) = d(j),e(j),f(j) j=1,...,nv  i=j+nu
c     rw(3*(nu+nv)+1) = rar0 = ra or r0 : this variable either specifies
c                       the value of R(t=0) or it specifies the
c                       normalization shift in the definition of R(t);
c                       the meaning of rar0 is determined by iopt
c     rw(3*(nu+nv)+2) = rbr1 = rb or r1 = value of R(t=1) or the
c                       normalization factor in the definition of R(t);
c                       the meaning of rbr1 is determined by iopt
c     rw(3*(nu+nv)+3) = f(nv+1)
c
c      NOTE (1) The parameters a(j) and d(j) can take on any value,
c               although the transformation will probably not
c               be invertible unless a(j) > 0 and d(j) > 0.
c
c      NOTE (2) The parameters b(j) and e(j) must be greater than zero
c                    b(j) > 0   and  e(j) > 0
c
c      NOTE (3) The c(j) should satisfy  0. <= c(j) <= 1.  However,
c               while 0 <= f(j) <= 1 for j=2,..,nv-1 it is true that
c               f(1) can be less than zero and f(nv+1) can be greater
c               than one if the transformation is not periodic. The f(j)
c               must always be in strictly increasing order:
c                       f(1) < f(2) < ... < f(nv)
c
c  iopt : Supply options - This parameter can be used to specify one
c         or more options.
c   iopt = 0 : use default values for all parameters -
c              (1) compute spline for the inversion routine strt
c                  (see remarks below)
c              (2) Specify ra and rb, i.e. rar0=R(t=0) and rbr1=R(t=1)
c                  However if iopt=0 and rar0=rbr1=0. then STINIT assumes
c                  that the user really wants rar0=0. and rbr1=1.
c
c   (iopt/1) mod 2 = 1 : do not compute spline for inversion routine strt
c   (iopt/2) mod 2 = 1 : Specify r0 and r1, i.e. rar0=r0 and rbr1=r1
c                        where r0 and r1 are the scaling parameters
c                        appearing in the definition of R(t)
c   (iopt/4) mod 2 = 1 : debugging info
c      Examples: iopt=1 : do not compute a spline
c                iopt=2 : specify r0,r1
c                iopt=3 : (=1+2) do not compute a spline; specify r0,r1
c
c   ndwk: Dimension of the work array wk, only used if a spline is
c         created (see iopt). If iw(4)=nsp > 0 is specified then
c         ndwk >= nsp. Otherwise if the program chooses nsp it will
c         choose as large a value as possible for the given ndr
c         and ndwk; with nsp <= ndwk
c   wk : real array for used for work space when a spline is created,
c        (see iopt).
c
c OUTPUT -
c   ierr = 0   Success
c        = 1   ndi too small - on output iw(1) is set to the proper size
c        = 2   ndr too small - on output iw(1) is set to what could be
c              the correct size. (This value may have to be increased
c              even more depending on where the error occured)
c        = 3   Error : some b(j) or e(j) is less than or equal to zero
c        = 4   Error : some c(j) is out of range (0 =< c(i) <= 1)
c        = 5   Error : the f(j) are not in increasing order, or some
c                      f(j) is out of range
c        = 6   Error : No spline can be generated since the
c                      transformation is not invertible
c        = 7   Warning : no spline created since there was not enough
c                work space, either ndr or ndwk is too small
c        = 8   ERROR : Unable to satisfy the conditions R(0)=rar0 and
c                      R(1)=rbr1 since R(0)=R(1)
c        = 9   ERROR : Algorithm to prevent underflows/overflows has
c                      apparently failed -- this should not occur so
c                      blame Bill.
c  iw,rw = The values of the input parameters may be changed after
c          calling STINIT. These arrays are needed by the subroutines
c          STTR and STRT
c
c REMARKS -
c  (1) Spline for Inverse: If the function r=R(t) is invertible and
c      the inverse t=T(r) is to be evaluated by calling STRT then it is
c      preferable to let this routine fit a spline to the inverse.
c      This spline is used as an initial guess for a Newton iteration
c      in subroutine STRT. Usually only 1 or 2 Newton iterations are
c      required when a spline is used making the inversion of R(t)
c      very fast.
c
c EXTERNAL ROUTINES
c  CSGEN - Bill Henshaw's cubic spline routine found in CS.FOR (?)
c
c=====================================================================
cTeX end
      real rw(ndr),wk(ndwk)
      integer iw(ndi)
      include "OVFortranDefine.h"
c local
      parameter( ipnsp=5,ipsp=ipnsp+1,ippr0=ipsp+1,ippar1=ippr0+1,
     & ippar2=ippar1+1,ippar3=ippar2+1,ipntu=ippar3+1,ipntv=ipntu+1,
     & ipend=ipntv )
      integer pr0,par1,psp
      real*4 r1mach
      real*8 d1mach
      logical per
      logical dbg
      data dbg/.true./
c........statement functions
c     a(i)=rw(3*i-2)
      b(i)=rw(3*i-1)
      c(i)=rw(3*i  )
      d(i)=rw(3*(i+nu)-2)
      e(i)=rw(3*(i+nu)-1)
      f(i)=rw(3*(i+nu)  )
c........end statement functions
c.....First executable statement
      ierr=0
      nu=iw(1)
      nv=iw(2)
      per=iw(3).ne.0

      if( mod(iopt/4,2).eq.1 )then
        write(51,9000) nu,nv,iw(3),ndi,ndr,iw(4),ndwk,
     &  (j,rw(3*j-2),rw(3*j-1),rw(3*j),j=1,nu+nv+1)
      end if
 9000 format(2x,'STINIT:    nu =',i3,'    nv =',i3,' iw(3)=',i3,/,
     &       2x,'ndi=',i3,' ndr=',i3,' iw(4) =',i3,' ndwk =',i3,/,
     &       2x,'j      a(d)    b(e)   c(f)',/,(1x,i2,3f10.4))

      if( per )then
        ndimn=ipend+nu+nv
      else
        ndimn=ipend
      end if
      if( ndi.lt.ndimn )then
        if(dbg)write(6,*) 'ERROR STINIT: dimension ndi too small'
        if(dbg)write(6,*) 'ndi =',ndi,' < ',ndimn
        iw(1)=ndimn
        ierr=1
        return
      end if
      if( ndr.lt.3*(nu+nv+1)+4 +nv) then
        if(dbg)write(6,*) 'ERROR STINIT: dimension ndr too small'
        if(dbg)write(6,*) 'ndr =',ndr,' < ',3*(nu+nv+1)+4+nv
        iw(1)=3*(nu+nv+1)+4+nv
        ierr=2
        return
      end if
c     .... get eps=r1mach(3) = smallest relative spacing
c     .... get epslon=r1mach(4) = largest relative spacing
c     .....small=r1mach(1) = smallest positive magnitude
c      IBM Single Prec.: epslon=.95367E-06   eps=.596046E-07
c               small = .53976E-78
      eps=r1mach(3)
      epslon=r1mach(4)
      small=r1mach(1)
c** The following fudge tries to detect whether the code has been
c   compiled with an automatic double precision option in which
c   case we want to call D1MACH
c      IBM Double Prec.: epslon=.222E-15   eps=.1388-16

c*wdh      oneps=1.+eps/100.
c*wdh      if( oneps.ne.1. )then
      if( OVUseDouble.eq.1 )then
        eps=d1mach(3)
        epslon=d1mach(4)
        small=d1mach(1)
      end if
c     ..... biga = - .5 * ln( eps/4 )
c     ..... abs( tanh(x)-1 ) < eps/2 for abs(x) > biga
c     ......sml = alog(small)
      biga=-.5*alog(eps/4.)
      sml=alog(small)
c**   write(6,*) 'STINIT: eps,epslon,sml  =',eps,epslon,sml
c**   write(6,*) 'STINIT: biga =',biga
      pr0=(nu+nv)*3+1
      iw(ippr0)=pr0
c Save:  rw(pr0+3) = biga
c        rw(pr0+4) = epslon*factor
c        rw(pr0+5) = R(0)   assigned later
c        rw(pr0+6) = R(1)   assigned later
c
      rw(pr0+3)=biga
      rw(pr0+4)=epslon*20.  ! (this is changed later)
c.......pointer to first free space in rw and iw
      irwe=pr0+7
      iiwe=ipend+1
c...................U(t,i)...........................................
      bmax=0.
      do 100 i=1,nu
c       .... a(i) <- a(i)/2
        rw(3*i-2)=rw(3*i-2)/2.
        bmax=max(bmax,b(i))
        if( b(i).le.0. )then
          if(dbg)write(6,*) 'ERROR STINIT b(i) =< 0, i,b(i)=',i,b(i)
          ierr=3
          return
        end if
        if( c(i).lt.0. .or. c(i).gt.1. )then
          if(dbg)write(6,*) 'ERROR STINIT invalid c(i)  i,c(i)=',i,c(i)
          ierr=4
          return
        end if
 100  continue
      if( per )then
c       Periodic case
        intu=iiwe-1
        iw(ipntu)=intu
        iw(ippar2)=irwe
        kb=iw(ippar2)-1
        if( kb+nu+nv .gt. ndr )then
          if(dbg)write(6,*) 'ERROR STINIT: dimension ndr too '//
     &                      'small, ndr=',ndr
          if(dbg)write(6,*) '      ndr=',ndr,' < ',kb+nu+nv
          iw(1)=kb+nu+nv
          ierr=2
          return
        end if
        do 250 i=1,nu+nv
c         ....number of terms needed in periodic correction
          nterm=1.+log(4./epslon)/(2.*b(i))
c         ....check to see if exp(-4*b(i)) will underflow
c         ....we assume here that the smallest possible number, sml,
c             is much less that eps so that when an underflow can occur
c             nterm must equal 1
          if( -4.*b(i).le. sml )then
            if( nterm.ne.1 )then
              ierr=9
              if(dbg)write(6,*) 'ERROR STINIT: Underflow but nterm<>1'
              if(dbg)write(6,*) '      nterm,sml,eps=',nterm,sml,eps
              return
            end if
            nterm=0
          end if
          iw(intu+i)=nterm
c         ....save sech(2*b(i)*k)  k=1,...,nterm
          if( kb+nterm .gt. ndr )then
            if(dbg)write(6,*) 'ERROR STINIT: dimension ndr too '//
     &                      'small, ndr=',ndr
            if(dbg)write(6,*) '      ndr=',ndr,' < ',kb+nterm+(nu+nv-i)
            iw(1)=kb+nterm+(nu+nv-i)
            ierr=2
            return
          end if
          do 200 k=1,nterm
            kb=kb+1
            rw(kb)=2.*exp(-2.*b(i)*k)/(1.+exp(-4.*b(i)*k))
 200      continue
 250    continue
        irwe=kb+1
        iiwe=intu+nu+nv+1
      end if
c..................V(t,i)............................................
      do 300 i=nu+1,nu+nv
c       .... d(i) <- (d(i)-1.)/2
        rw(3*i-2)=(rw(3*i-2)-1.)/2.
 300  continue
c........Check the parameters e(i) and f(i)
      emax=0.
      do 400 i=1,nv
        emax=max(emax,e(i))
        if( e(i).le.0. )then
          if(dbg)write(6,*) 'ERROR STINIT e(i) =< 0, i,e(i)=',i,e(i)
          ierr=3
          return
        end if
        if( f(i+1).le.f(i) )then
          if(dbg)write(6,*) 'ERROR STINIT: f(i) not increasing'
          if(dbg)write(6,*) '  i,f(i),f(i+1) =',i,f(i),f(i+1)
          ierr=5
          return
        end if
        if( (f(i).lt.0. .and. i.gt.1) .or.
     &      (f(i).gt.1. .and. i.le.nv) )then
          if(dbg)write(6,*) 'ERROR STINIT: some f(i) out of range'
          if(dbg)write(6,*) '  i,f(i) =',i,f(i)
          ierr=5
          return
        end if
 400  continue
c.......compute the parameters d(i)*sinh( e(i)*(f(i+1)-f(i)) )
c       for V(t,i). Only used if e(i)*(f(i+1)-f(i))/2 < biga
      par1=irwe-1
      iw(ippar1)=par1
      if( ndr.lt.par1+nv ) then
        if(dbg)write(6,*) 'ERROR STINIT: ndr =',ndr,' < ',par1+nv
        iw(1)=par1+nv
        ierr=2
        return
      end if
      do 500 i=1,nv
        arg=e(i)*(f(i+1)-f(i))
        if( arg.lt.4.*biga )then
          rw(par1+i)=d(i)*sinh(arg)
        else
          rw(par1+i)=0.
        end if
 500  continue
      irwe=par1+nv+1
c..........compute normalization constants
c     ...tolerance for newton is scaled by the maximum slope
c     wdh940620:
      rw(pr0+4)=epslon*max(20.,bmax,emax)

      rar0=rw(pr0)
      rbr1=rw(pr0+1)
c     .... evaluate R(0) when the paramters are set to 0. and 1. :
      rw(pr0)=0.
      rw(pr0+1)=1.
      call sttr( 0.,rzero,rt, iw,rw,ierr )
      if( mod(iopt/2,2).eq.0 )then
c       ....BC's given for R: rar0=ra and rbr1=rb
        if( rar0.eq.0. .and. rbr1.eq.0. )then
          rar0=0.
          rbr1=1.
        end if
c       ....normalize to (ra,rb) solve: R(0) = rzero*z1+z0
c           and  R(1) =  rone*z1+z0  and save z0,z1
        call sttr( 1.,rone,rt, iw,rw,ierr )
        if( rone-rzero .eq.0. )then
          if(dbg)write(6,*) 'ERROR STINIT: R(0)=R(1) unable to apply'
          if(dbg)write(6,*) ' normalization conditions'
          ierr=8
          return
        end if
        rw(pr0+1)=(rbr1-rar0)/(rone-rzero)
        rw(pr0)=rbr1-rone*rw(pr0+1)
        rw(pr0+5)=rar0
        rw(pr0+6)=rbr1
      else
c       ....r0 and r1 are supplied: rar0=r0 and rbr1=r1
        rw(pr0)=rar0-rzero*rbr1
        rw(pr0+1)=rbr1
c       ....now save R(0) and R(1)
        call sttr( 0.,rar0,tmp, iw,rw,ierr )
        call sttr( 1.,rbr1,tmp, iw,rw,ierr )
        rw(pr0+5)=rar0
        rw(pr0+6)=rbr1
      end if
      psp=irwe-1
      iw(ipnsp)=0
      if( mod(iopt,2).eq.0 )then
c...........create a spline for the inverse transformation
        nsp=iw(4)
        if( nsp.le.0 )then
          nsp=min(ndwk,(ndr-irwe)/4)
        end if
        if( nsp.ge.5 )then
          irwe=irwe+4*nsp
          if( irwe-1.gt.ndr )then
            if(dbg)write(6,*) 'ERROR STINIT: not enough work'//
     &                     'space for spline'
            if(dbg)write(6,*) '        ndr=',ndr,' < ',irwe-1
            iw(1)=irwe-1
            ierr=2
            return
          end if
c         .... fit a spline to T(r) equally spaced in r  ra<= r <= rb:
          do 600 i=1,nsp
            wk(i)=rar0+(rbr1-rar0)*(i-1)/float(nsp-1)
            call strt( wk(i),rw(psp+i),tr, iw,rw,ierr )
            if( tr.le.0. )then
c             ....transformation apparently not invertible
              if(dbg)write(6,9200) wk(i),rw(psp+i),tr
              ierr=6
              return
            end if
 600      continue
          ioptsp=iw(3)
          call csgen( nsp,wk,rw(psp+1),rw(psp+nsp+1),ioptsp )
          iw(ipnsp)=nsp
          iw(ipsp)=psp
 700      continue
        else
          if( dbg) then
            write(6,*) 'WARNING STINIT: Not enough space for a spline'
            write(6,*) '  of at least 5 points, or 1 < iw(4) < 4    '
            write(6,*) 'One of ndr,ndwk,iw(4)=',ndr,ndwk,iw(4),
     &               ' is too small'
            write(6,*) ' No spline created...'
          end if
          ierr=7
        end if
      end if
 900  continue
      return
 9200 format(1x,'ERROR STINIT: Apparently the transformation is not',/,
     & 1x,'invertible since dt/dr .le. 0. at some point',/,
     & 1x,'(t,r,dt/dr)=(',f12.6,',',f12.6,',',e12.5,')',/,
     & 1x,'...No spline has been generated')
      end

cTeX begin \listing sttr.ftex
      subroutine sttr( t,r,rt, iw,rw,ierr )
c=====================================================================
c       Stretching Subroutine  t --> r
c Evaluate the stretching function r=R(t)
c
c INPUT -
c  t     = evaluate R at this value of t
c  iw,rw = arrays initialized by the routine STINIT
c
c OUTPUT -
c  r,rt   = function value r, and derivative rt = dR/dt
c  ierr  = 0 Normal return
c  ierr  = 1   Error : This error should not occur if the algorithm
c                      for computing R(t) is correct. If it does
c                      occur - blame Bill.
c
c METHOD -
c  We try to evaluate the stretching function, R(t) in a manner which is
c relatively efficient and which guards against underflows or overflows
c when the parameters b(i) or e(i) become large. The periodic corrections
c are especially difficult to evaluate for large parameter values.
c
c=====================================================================
cTeX end
      real rw(*)
      integer iw(*),pr0,par1
      logical per
      logical dbg
c*wdh logical debug
      parameter( ipnsp=5,ipsp=ipnsp+1,ippr0=ipsp+1,ippar1=ippr0+1,
     & ippar2=ippar1+1,ippar3=ippar2+1,ipntu=ippar3+1,ipntv=ipntu+1,
     & ipend=ipntv )
      data dbg/.true./
c*wdh data debug/.false./
      a(i)=rw(3*i-2)
      b(i)=rw(3*i-1)
      c(i)=rw(3*i)
      wb(i)=biga/b(i)
      absinh(i)=rw(par1+i)
c......first executable statement
      ierr=0
      per=iw(3).ne.0
      r=t
      rt=1.
      nu=iw(1)
      nv=iw(2)
      pr0=iw(ippr0)
      biga=rw(pr0+3)
      par1=iw(ippar1) - nu
c..........First U(t,i) i=1,nu....................................
      if( .not.per )then
c       ....non-periodic case
        do 100 i=1,nu
          arg=b(i)*(t-c(i))
          if( abs(arg).lt.biga )then
            th=tanh(arg)
            r=r+a(i)*th
            rt=rt+a(i)*b(i)*(1.-th**2)
          else
            th=sign(1.,t-c(i))
            r=r+a(i)*th
          end if
 100    continue
      else
c       ....periodic case
        intu=iw(ipntu)
        kb=iw(ippar2)-1
        do 300 i=1,nu
          arg=b(i)*(t-c(i))
          if( abs(arg).lt.biga )then
            th=tanh(arg)
            r=r+a(i)*th
            rt=rt+a(i)*b(i)*(1.-th**2)
          else
            th=sign(1.,t-c(i))
            r=r+a(i)*th
          end if
c         ....corrections for periodicity
          nterm=iw(intu+i)
          if( nterm.gt.0 )then
c           ....add correction from infinite series
            sech2a=2.*exp(-2.*arg)/(1.+exp(-4.*arg))
            tth2a=4.*th/(1.+th**2)
            rp=0.
            rp2=0.
            do 250 k=1,nterm
              kb=kb+1
              sech2b=rw(kb)
              tmp=sech2b/(sech2a+sech2b)
              rp=rp+tmp
              rp2=rp2+tmp**2
 250         continue
            r=r+a(i)*tth2a*rp
            rt=rt+a(i)*b(i)*(4.*rp-tth2a**2*rp2)
          elseif( abs(t-c(i)).gt.0.5 )then
c           ....infinite series correction reduces to two terms
c               these terms correspond to c(i)+1 and  c(i)-1
c               However due to possible underflows we evaluate these
c               terms in the same way as the original term
            pm1=sign(1.,t-c(i))
            ci=c(i)+pm1
            arg=b(i)*(t-ci)
            if( abs(arg).lt.biga )then
              th=tanh(arg)
              r=r+a(i)*(th+pm1)
              rt=rt+a(i)*b(i)*(1.-th**2)
            else
              th=sign(1.,t-ci)
              r=r+a(i)*(th+pm1)
            end if
          end if
 300    continue
      end if
c................V(t,i)............................................
c*wdh if( debug ) write(6,9000) nv,i,t
 9000 format(1x,'STTR%%%%%%%nv=',i3,' i=',i3,'t=',f11.5,'%%%%%%%%' )
      do 800 i=nu+1,nu+nv
        if( a(i).ne.0. )then
          tc=t-(c(i+1)+c(i))/2.
          ta=abs(tc)
          w=wb(i)
          cd=(c(i+1)-c(i))/2.
c*wdh     if( debug ) write(6,9100) i,a(i),c(i),c(i+1),b(i),tc,ta,w,cd
 9100 format(1x,'STTR i,a(i),c(i),c(i+1),b(i),tc,ta,w,cd =',/,
     & 1x,i2,3f8.4,f8.2,4f8.4)
          if( ta-cd .gt. w ) then
c           ....flat sections on ends
            dr=2.*a(i)*cd
            drt=0.
c*wdh       if( debug ) write(6,*) '1 ta-cd>w     dr,drt=',dr,drt
          elseif( ta-cd .gt. -w )then
c           ....transition regions
            if( ta+cd .lt. w )then
              cosh1=cosh(b(i)*(ta+cd))
              cosh2=cosh(b(i)*(ta-cd))
              dr =a(i)*alog(cosh1/cosh2)/b(i)
              drt=absinh(i)/(cosh1*cosh2)
c*wdh         if(debug) write(6,*) '2 ta-cd>-w ta+cd<w dr,drt=',dr,drt
            else
              dr=a(i)*(ta+cd-alog(2.*cosh(b(i)*(ta-cd)))/b(i))
              drt=a(i)*(1.-tanh(b(i)*(ta-cd)))
c*wdh         if(debug) write(6,*) '3 ta-cd>-w ta+cd>w dr,drt=',dr,drt
            end if
          else
c           ....center sloped section
            dr=2.*a(i)*ta
            drt=2.*a(i)
c*wdh         if(debug) write(6,*) '4 center slope dr,drt=',dr,drt
          end if
          if( ta.ne.tc ) dr=-dr
          r=r+dr
          rt=rt+drt
c*wdh     if(debug) write(6,*) 'STTR i,r,rt =',i,r,rt
          if( per )then
c           ....corrections for periodicity
            nterm=iw(intu+i)
            if( nterm.gt.0. )then
              arg=2.*b(i)*abs(t-c(i))
              sech2u=2.*exp(-arg)/(1.+exp(-2.*arg))
              tanh2u=sign(tanh(arg),t-c(i))
              arg=2.*b(i)*abs(t-c(i+1))
              sech2w=2.*exp(-arg)/(1.+exp(-2.*arg))
              tanh2w=sign(tanh(arg),t-c(i+1))
c             ....trouble here if sech2u=0.
              if( sech2u.eq.0. .or. sech2w.eq.0. )then
                if( dbg ) then
                  write(6,*)
     &             'ERROR STTR: It seems a periodic V(t,i)'
                  write(6,*)
     &             'V(t,j) has been specified with a large b(j)'
                  write(6,*)
     &             'so sech(2.*b(j)*(t-c(j))) == 0 (computer)'
                  write(6,*)
     &             'This case should not happen -- '
                end if
                ierr=1
                return
              end if
              sratio=sech2w/sech2u
              rp=1.
              rp2=0.
              rp3=0.
              do 600 k=1,nterm
                kb=kb+1
                sech2b=rw(kb)
                tu=sech2u+sech2b
                tw=sech2w+sech2b
                rp=rp*(tu/tw)*sratio
                rp2=rp2+sech2b/tu
                rp3=rp3+sech2b/tw
 600          continue
              if( rp.le.0. )then
                if( dbg )then
                  write(6,*)'ERROR STTR: Error evaluating V(t,i)'
                  write(6,*) '    t,r,i,b(i) =',t,r,i,b(i)
                  write(6,*) '    c(i),c(i+1)=',c(i),c(i+1)
                  write(6,*) '    sech2u,sech2w,rp=',sech2u,sech2w,rp
                  write(6,*) '  This error should not happen --'
                end if
                ierr=1
                return
              end if
              r=r+a(i)*alog(rp)/b(i)
              rt=rt+a(i)*2.*(tanh2u*rp2-tanh2w*rp3)
            elseif( t-c(i+1).lt.-0.5 .or. t-c(i).gt.0.5 )then
c             ....infinite series correction reduces to two terms
c                 these terms correspond to c(i)+1 and  c(i)-1
c                 However due to possible underflows we evaluate these
c                 terms in the same way as the original term
              do 700 mp1=-1,1,2
                ci  =c(i  )+mp1
                cip1=c(i+1)+mp1
                tc=t-(cip1+ci)/2.
                ta=abs(tc)
                w=wb(i)
                cd=(cip1-ci)/2.
                if( ta-cd .gt. w ) then
c                 ....flat sections on ends
                  dr=2.*a(i)*cd
                  drt=0.
                elseif( ta-cd .gt. -w )then
c                 ....transition regions
                  if( ta+cd .lt.w )then
                    cosh1=cosh(b(i)*(ta+cd))
                    cosh2=cosh(b(i)*(ta-cd))
                    dr =a(i)*alog(cosh1/cosh2)/b(i)
                    drt=absinh(i)/(cosh1*cosh2)
                  else
                    dr=a(i)*(ta+cd-alog(2.*cosh(b(i)*(ta-cd)))/b(i))
                    drt=a(i)*(1.-tanh(b(i)*(ta-cd)))
                  end if
                else
c                 ....center sloped section
                  dr=2.*a(i)*ta
                  drt=2.*a(i)
                end if
                if( ta.ne.tc ) dr=-dr
                r=r+dr
                rt=rt+drt
 700          continue
            end if
          end if
        end if
 800  continue
      r=r*rw(pr0+1)+rw(pr0)
      rt=rt*rw(pr0+1)

      return
      end
cTeX begin \listing strt.ftex
      subroutine strt( r,t,tr, iw,rw,ierr )
c=====================================================================
c       Stretching Subroutine  r --> t
c Evaluate the inverse of the stretching function t = T(r) = R**(-1) (r)
c and the first derivative, dT/dr
c
c INPUT -
c  r     = evaluate T at this value of r
c  iw,rw = arrays initialized by the routine STINIT
c
c OUTPUT -
c  t,tr  = function value t, and derivative tr = dT/dr
c  ierr  = 0 normal return. NOTE: If the input value for r is way outside 
c          the range of the function then bogus values are returned.
c        = -1 Warning:  Newton required more than 5 iterations.
c            If a spline is being used for an initial guess one might
c            want to increase the number of points on the spline.
c        = 1 Error: Fatal error in attempting to invert the function
c                   Apparently the Jacobian is nearly singular
c
c METHOD -
c  Invert the stretching function using  Newton's method. Obtain the
c  initial guess with a spline (if created by stinit) or else using
c  bisection.
c
c=====================================================================
cTeX end
      real rw(*)
      integer iw(*)
      parameter(mxbis1=4,mxbis2=8,mxnewt=12)
      integer pt,pbcd,pr0
      parameter( ipnsp=5,ipsp=ipnsp+1,ippr0=ipsp+1,ippar1=ippr0+1,
     & ippar2=ippar1+1,ippar3=ippar2+1,ipntu=ippar3+1,ipntv=ipntu+1,
     & ipend=ipntv )
      logical dbg
      data dbg/.true./

      ierr=0
c      if( abs(r-ra) .gt. 9.*(rb-ra) )then
c        t=r
c        tr=1.
c        ierr=0
c        return
c      end if

      pr0=iw(ippr0)
      ra=rw(pr0+5)
      rb=rw(pr0+6)

      if( iw(ipnsp).ne.0 )then
c........use a spline to get first guess for Newton
        nsp=iw(ipnsp)
        pt=iw(ipsp)
        rd=(rb-ra)/(nsp-1)
        ii=1+(r-ra)/rd
        ii=max(1,min(nsp,ii))
        dr=r-(ra+rd*(ii-1))
        pbcd=pt+nsp+3*ii-2
        t=rw(pt+ii)+dr*(rw(pbcd)+dr*(rw(pbcd+1)+dr*rw(pbcd+2)))
c**     write(6,*) 'STRT: nsp,ra,rb =',nsp,ra,rb
c       write(6,*) 'STRT: r,t(spline),tr(spline)= ',r,t,
c**  &               rw(pbcd)+dr*(2.*rw(pbcd+1)+3.*dr*rw(pbcd+2))

      else
c.......No spline available : use bisection
c       Do at least mxbis2 iterations of bisection.
        t=.5
c 960906    dt=.51
        dt=1.01
        do 20 i=1,mxbis1
          do 10 j=1,mxbis2
            call sttr( t,rr,rt, iw,rw,ierr )
            dr=r-rr
            dt=sign(.5*dt,dr)
            t=t+dt
10         continue
c         Check if we need more bisection iterations before doing Newton.
          if(abs(dr).lt.abs(rt*2.5*dt))goto30
20       continue

c       Newton couldn't be used.
        if(rt.eq.0.)goto60
        goto50
      end if
30    continue
      epslon=rw(iw(ippr0)+4)
c         Now do up to mxnewt iterations of Newton.
          do 40 it=1,mxnewt
            call sttr( t,rr,rt, iw,rw,ierr )
            dr=r-rr
c           Check for nearly singular Jacobian.
            if(rt.le.abs(dr))goto60
            dt=dr/rt
            t=t+dt
c           Check for convergence.
            if( it.gt.mxnewt-2 )then
              if( dbg )then
                write(6,*) 'WARNING:STRT: NEWTON converging slowly'
                write(6,*) '  STRT:  it,r,t,dt=',it,r,t,dt
                if( nsp.gt.0 )then
                  write(6,*) '  STRT: You may want to increase the '//
     &             'number of points on the spline, nsp=',nsp
                end if
              end if
              ierr=-1
            end if
            if(abs(dt).lt.epslon)goto50
40         continue

          if(dbg) write(6,9000) r,dt,epslon
9000      format(1x,'Warning in STRT:  Newton didn''t converge at r='
     +      ,e9.2,' dt =',e9.2,' epslon=',e9.2)

50        continue
c          write(*,*) 'Number of Newton Iterations=',it
c          write(6,'(" rr=",e10.2," t=",e12.4," dt=",e10.2," '//
c     &      'epslon=",e10.2)') rr,t,dt,epslon
          tr=1./rt
        return

60      continue
        dbg=.false.
        if(dbg .and. abs(r-ra) .lt. 90.*(rb-ra) ) 
     &    write(6,*) 'Fatal Error in STRT dr/dt=',rt,' dr=',dr,
     &   ' r=',r,'t=',t
        if( rt.ne.0. )then
          tr=1/rt
        else
          tr=1.e10
        end if
        if(abs(r-ra) .lt. 90.*(rb-ra)) then
          ierr=1
        else
          ierr=-1
        end if
        return
       end
