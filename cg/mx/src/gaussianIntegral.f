CCCCCCCCC1CCCCCCCCC2CCCCCCCCC3CCCCCCCCC4CCCCCCCCC5CCCCCCCCC6CCCCCCCCC7CC 
      subroutine wpulse(w,nsources,xs,ys,tau,var,amp,
     1                 a,x,y,time)
C
C     computes an exact solution to the two-dimensional wave equation
C     on the domain  -\infty < x < \infty, 0 < y < a, 0 < t,
C     periodic in y.
C
C     The solution is constructed as a superposition of 
c     derivatives of smooth pulses,
C     all of which have been turned on and off before time t = 0.
C     The values of nsources,xs,ys,tau,var,amp must be input
c     x,y,time are the coordinates at
c     which the solution is to be evaluated. It is assumed that the
c     support of the initial data is a subset of (-2,2)XT
C
c
c
C     OUTPUT: w 
C-----------------------------------------------------------------------
C
      implicit double precision (a-h,o-z)
      dimension xs(*),ys(*),tau(*),var(*),amp(*)
c
      w=wex(nsources,xs,ys,tau,var,amp,a,x,y,time,0)
c
      return
      end
CCCCCCCCC1CCCCCCCCC2CCCCCCCCC3CCCCCCCCC4CCCCCCCCC5CCCCCCCCC6CCCCCCCCC7CC 
      subroutine wdpulse(w,wt,wx,nsources,xs,ys,tau,var,amp,
     1                 a,x,y,time)
C
C     computes an exact solution to the two-dimensional wave equation
C     on the domain  -\infty < x < \infty, 0 < y < a, 0 < t,
C     periodic in y.
C
C     The solution is constructed as a superposition of 
c     derivatives of smooth pulses,
C     all of which have been turned on and off before time t = 0.
C     The values of nsources,xs,ys,tau,var,amp must be input
c     x,y,time are the coordinates at
c     which the solution is to be evaluated. It is assumed that the
c     support of the initial data is a subset of (-2,2)XT
C
c
c
C     OUTPUT: w 
C-----------------------------------------------------------------------
C
      implicit double precision (a-h,o-z)
      dimension xs(*),ys(*),tau(*),var(*),amp(*)
c
      w =wex(nsources,xs,ys,tau,var,amp,a,x,y,time,0)
      wx=wex(nsources,xs,ys,tau,var,amp,a,x,y,time,1)
      wt=wex(nsources,xs,ys,tau,var,amp,a,x,y,time,3)
c
      return
      end

CCCCCCCCC1CCCCCCCCC2CCCCCCCCC3CCCCCCCCC4CCCCCCCCC5CCCCCCCCC6CCCCCCCCC7CC 
      subroutine exmax(wt,wx,wy,nsources,xs,ys,tau,var,amp,
     1                 a,x,y,time)
C
C     computes an exact solution to the two-dimensional
C     TE Maxwell equations
C     on the domain  -\infty < x < \infty, 0 < y < a, 0 < t,
C     periodic in y.
C
C     The solution is constructed as a superposition of 
c     derivatives of smooth pulses,
C     all of which have been turned on and off before time t = 0.
C     The values of nsources,xs,ys,tau,var,amp must be input
c     x,y,time are the coordinates at
c     which the solution is to be evaluated. It is assumed that the
c     support of the initial data is a subset of (-2,2)XT
C
c
c
C     OUTPUT: Ez, Bx, By
C-----------------------------------------------------------------------
C
      implicit double precision (a-h,o-z)
      dimension xs(*),ys(*),tau(*),var(*),amp(*)
c
      wx=wex(nsources,xs,ys,tau,var,amp,a,x,y,time,1)
      wy=wex(nsources,xs,ys,tau,var,amp,a,x,y,time,2)
      wt=wex(nsources,xs,ys,tau,var,amp,a,x,y,time,3)
c
      return
      end

CCCCCCCCC1CCCCCCCCC2CCCCCCCCC3CCCCCCCCC4CCCCCCCCC5CCCCCCCCC6CCCCCCCCC7CC 
      double precision function wex(nsources,xs,ys,tau,var,amp,
     1                 a,x,y,time,ider)
C
C     computes an exact solution to the two-dimensional
C     wave equation 
C
C                    p_{tt} = \Delta p
C
C     on the domain  -\infty < x < \infty, 0 < y < a, 0 < t,
C     periodic in y.
C
C     The solution is constructed as a superposition of smooth pulses,
C     all of which have been turned on and off before time t = 0.
C
C     For the ith source,
C
C     wex(x,y,t) = 
C        \sum_{k=-\infty}^\infty 
C           \int_{-\infty}^t  H((t-s)^2 - r_{ik}^2)/
C                   [\sqrt{(t-s)^2 - r_{ik}^2}] sigma_i(s) ds
C                        
C                   = 
C        \sum_{k=-\infty}^\infty 
C           \int_{-\infty}^{t-r_{ik}} 1/
C                   [\sqrt{(t-s)^2 - r_{ik}^2}] sigma_i(s) ds
C                        
C     where r_{ik} = distance from kth image of ith source 
C     (xs(i),ys(i)+ ka) to target (x,y), H(x) denotes the 
C     Heaviside function, and sigma_i(s) = amp(i)*exp( - var(i)*(s-\tau_i)^2).
C     
C     In order to impose nonreflecting B.C. on specific lines
C     x = c, x = d, c and d have to be chosen so that the 
C     initial data is supported in [c,d] x [0,a].
C
C-----------------------------------------------------------------------
C     REMARKS: 
C     
C     1) For any precision epsilon, the ith pulse
C        has a lifetime [ tau(i) - \sqrt{-log(epsilon/amp(i)))/var(i),
C                         tau(i) + \sqrt{-log(epsilon/amp(i)))/var(i)].
C
C     Setting gamma(i)=-log(epsilon/amp(i)),
C     there are three cases to consider.
C     If (t - r_{ik}) < tau(i) - \sqrt{gamma(i)/var(i)}
C        there is no contribution.
C     else if (t - r_{ik}) < tau(i) + \sqrt{gamma(i)/var(i)}
C        there is a contribution and the integrand is not smooth
C     else 
C        there is a contribution and the integrand is smooth
C
C     In the nonsmooth case, the code calls wavgrcor, an end-point
C     corrected trapezoidal method. In the smooth case, the code
C     uses Gauss-Legendre quadrature.
C
C     2) I have fixed the order of Gauss-Legendre quadrature to 64.
C        My experiments indicate that it should yield 14 digits, no matter how
C        sharp the pulse, but I haven't checked carefully.
C
C     3) I have fixed the trapezoidal quadrature to use 200 points
C        with 8th order corrections. My experiments indicate that this
C        should yield 10 digits.
C
C-----------------------------------------------------------------------
C     INPUT:
C     
C     nsources = number of sources
C     xs(i),ys(i) = coordinates of ith source point.
C     tau(i) = mean time of pulse at ith source point.
C     var(i) = variance of pulse at ith source point 
C                (see formula above)
C     amp(i) = amplitude of pulse at ith source point
C     a = periodic length in y
C     x,y    = coordinates of the target point.
C     time   = time of evaluation.
C     ider = specification of derivative to be evaluated
C            ider=0 - evaluate p
C            ider=1 - evaluate dp/dx
C            ider=2 - evaluate dp/dy
C            ider=3 - evaluate dp/dt
C     glnodes = nodes for Gauss-Legendre quadrature on [-1,1].
C     glweights = weights for Gauss-Legendre quadrature on [-1,1].
C     nglnodes = number of Gauss-Legendre nodes
C
C     OUTPUT: wex = solution. (see ider)
C-----------------------------------------------------------------------
C
      implicit double precision (a-h,o-z)
      dimension xs(*),ys(*),tau(*),var(*),amp(*)
      dimension ts(64),ws(64),densj(64)
      dimension gln1(24),gln2(20),gln3(20)
      dimension glw1(24),glw2(20),glw3(20)
      data gln1/ -0.99930504173577, -0.99634011677196,
     2   -0.99101337147674, -0.98333625388463,
     3 -0.97332682778991, -0.96100879965205,
     4 -0.94641137485840, -0.92956917213194,
     5 -0.91052213707850, -0.88931544599511,
     6 -0.86599939815409, -0.84062929625258,
     7 -0.81326531512280, -0.78397235894334,
     8 -0.75281990726053, -0.71988185017161,
     9 -0.68523631305423, -0.64896547125466,
     * -0.61115535517239, -0.57189564620263,
     * -0.53127946401989, -0.48940314570705,
     * -0.44636601725346, -0.40227015796399/
      data gln2/-0.35722015833767, -0.31132287199021,
     2 -0.26468716220877, -0.21742364374001,
     3 -0.16964442042399, -0.12146281929612,
     4 -7.2993121787799D-02, -2.4350292663425D-02,
     5 2.4350292663424D-02, 7.2993121787799D-02,
     6 0.12146281929612, 0.16964442042399,
     7 0.21742364374001, 0.26468716220877,
     8 0.31132287199021, 0.35722015833767,
     9 0.40227015796399, 0.44636601725346,
     * 0.48940314570705, 0.53127946401989/
      data gln3/0.57189564620263, 0.61115535517239,
     2 0.64896547125466, 0.68523631305423,
     3 0.71988185017161, 0.75281990726053,
     4 0.78397235894334, 0.81326531512280,
     5 0.84062929625258, 0.86599939815409,
     6 0.88931544599511, 0.91052213707850,
     7 0.92956917213194, 0.94641137485840,
     8 0.96100879965205, 0.97332682778991,
     9 0.98333625388463, 0.99101337147674,
     * 0.99634011677195, 0.99930504173577/
      data glw1/1.7832807216959D-03, 4.1470332605626D-03,
     2    6.5044579689788D-03, 8.8467598263635D-03,
     3    1.1168139460132D-02, 1.3463047896719D-02,
     4    1.5726030476023D-02, 1.7951715775662D-02,
     5    2.0134823153566D-02, 2.2270173808384D-02,
     6    2.4352702568696D-02, 2.6377469715070D-02,
     7    2.8339672614260D-02, 3.0234657072402D-02,
     8    3.2057928354852D-02, 3.3805161837141D-02,
     9    3.5472213256885D-02, 3.7055128540240D-02,
     *    3.8550153178616D-02, 3.9953741132721D-02,
     *    4.1262563242623D-02, 4.2473515123651D-02,
     *    4.3583724529323D-02, 4.4590558163756D-02/
      data glw2/4.5491627927419D-02, 4.6284796581315D-02,
     2    4.6968182816210D-02, 4.7540165714831D-02,
     3    4.7999388596458D-02, 4.8344762234802D-02,
     4    4.8575467441503D-02, 4.8690957009140D-02,
     5    4.8690957009139D-02, 4.8575467441503D-02,
     6    4.8344762234803D-02, 4.7999388596458D-02,
     7    4.7540165714830D-02, 4.6968182816210D-02,
     8    4.6284796581315D-02, 4.5491627927418D-02,
     9    4.4590558163758D-02, 4.3583724529322D-02,
     *    4.2473515123654D-02, 4.1262563242624D-02/
      data glw3/3.9953741132721D-02, 3.8550153178617D-02,
     2    3.7055128540241D-02, 3.5472213256882D-02,
     3    3.3805161837142D-02, 3.2057928354852D-02,
     4    3.0234657072402D-02, 2.8339672614260D-02,
     5    2.6377469715054D-02, 2.4352702568711D-02,
     6    2.2270173808383D-02, 2.0134823153531D-02,
     7    1.7951715775697D-02, 1.5726030476025D-02,
     8    1.3463047896719D-02, 1.1168139460130D-02,
     9    8.8467598263639D-03, 6.5044579689784D-03,
     *    4.1470332605626D-03, 1.7832807216966D-03/
c
c
      wex = 0.0d0
      nglnodes = 64
c
c     loop over sources 
c
      tol = 1.0d-11
      do 4000 i = 1,nsources
c	 write (6,*) ' nsource = ',i
         if (abs(amp(i)).le.tol) go to 4000 
	 halflife = dsqrt(-dlog(tol/abs(amp(i)))/var(i))
	 tmin = tau(i) - halflife
	 tmaxd = tau(i) + halflife
c         write (6,*)' tmin - tmax ',tmin,tmaxd
c
c        compute scaled Gauss weights/nodes
c
	 do 200 j = 1,24
	    ts(j) = halflife*gln1(j) + tau(i)
	    ws(j) = halflife*glw1(j)
200      continue
	 do 210 j = 1,20
	    ts(24+j) = halflife*gln2(j) + tau(i)
	    ws(24+j) = halflife*glw2(j)
210      continue
	 do 220 j = 1,20
	    ts(44+j) = halflife*gln3(j) + tau(i)
	    ws(44+j) = halflife*glw3(j)
220      continue
c
c       compute density at corresponding nodes
c
	 do 230 j = 1,nglnodes
            if (ider.eq.0) then
               densj(j) = dens(ts(j),tau(i),var(i),amp(i),0)
            else if (ider.eq.3) then
               densj(j) = dens(ts(j),tau(i),var(i),amp(i),1)
            else 
               df=1.d0/(time-ts(j))
               densj(j) = df*(dens(ts(j),tau(i),var(i),amp(i),1)
     &                     +dens(ts(j),tau(i),var(i),amp(i),0)*df)
            end if
230      continue
	 rx = xs(i) - x 
	 ry = ys(i) - y 
	 r2 = rx*rx + ry*ry
	 rdis = dsqrt(r2)
         if ( (time-rdis).lt.tmin) then
	    goto 1000
         else if ( (time-rdis).lt.tmaxd) then
	    call singquad(time,tmin,x,y,rdis,rx,ry,tau(i),var(i),
     &                    amp(i),ider,pinc)
	    wex = wex + pinc
         else 
c
c        apply gauss-legendre quadrature
c
            do 300 j = 1,nglnodes
               if (ider.eq.1) then    
                 sinc = rx*densj(j)/(dsqrt((time-ts(j))**2-r2))
               else if (ider.eq.2) then
                 sinc = ry*densj(j)/(dsqrt((time-ts(j))**2-r2))
               else
                 sinc = densj(j)/(dsqrt((time-ts(j))**2-r2))
               end if
	       wex = wex + sinc*ws(j)
300 	    continue
         endif
1000     continue
c
c        process (+) images
c
	 do 2000 k = 1,10000
            ry = ys(i) + k*a - y
	    r2 = rx*rx + ry*ry
	    rdis = dsqrt(r2)
            if ( (time-rdis).lt.tmin) then
	       goto 2001
            else if ( (time-rdis).lt.tmaxd) then
	      call singquad(time,tmin,x,y,rdis,rx,ry,tau(i),var(i),
     &                    amp(i),ider,pinc)
	      wex = wex + pinc
            else 
c
c        apply gauss-legendre quadrature
c
               do 400 j = 1,nglnodes
                 if (ider.eq.1) then    
                   sinc = rx*densj(j)/(dsqrt((time-ts(j))**2-r2))
                 else if (ider.eq.2) then
                   sinc = ry*densj(j)/(dsqrt((time-ts(j))**2-r2))
                 else
                   sinc = densj(j)/(dsqrt((time-ts(j))**2-r2))
                 end if
	         wex = wex + sinc*ws(j)
400 	       continue
            endif
2000     continue
2001     continue
c
c        process (-) images
c
	 do 3000 k = 1,10000
            ry = ys(i) - k*a - y
	    r2 = rx*rx + ry*ry
	    rdis = dsqrt(r2)
            if ( (time-rdis).lt.tmin) then
	       goto 4000
            else if ( (time-rdis).lt.tmaxd) then
	      call singquad(time,tmin,x,y,rdis,rx,ry,tau(i),var(i),
     &                    amp(i),ider,pinc)
  	      wex = wex + pinc
            else 
c
c        apply gauss-legendre quadrature
c
               do 500 j = 1,nglnodes
                 if (ider.eq.1) then    
                   sinc = rx*densj(j)/(dsqrt((time-ts(j))**2-r2))
                 else if (ider.eq.2) then
                   sinc = ry*densj(j)/(dsqrt((time-ts(j))**2-r2))
                 else
                   sinc = densj(j)/(dsqrt((time-ts(j))**2-r2))
                 end if
	         wex = wex + sinc*ws(j)
500 	       continue
            endif
3000     continue
4000  continue
      return
      end
c
c
      subroutine singquad(time,tmin,x,y,rdis,rx,ry,tau,var,amp,
     &                    ider,pinc)
      implicit double precision (a-h,o-z)
      dimension densj(200)
C
C     The subroutine wavgrcor uses a change of variables applied
C     to the formula above:  v = s - t + r, so that
C     the range of integration is [tmin - t + r,0], rather than
C     [tmin,t-r]. See documention for wavgrcor for more details.
C
C
      nnodes = 200
      ss = tmin - time + rdis
ccc      print *, ' ss = ',ss
      h = - ss/(nnodes-1)
ccc      print *, ' h = ',h
      do 100 i = 1,nnodes
	 ww = ss + (i-1)*h
	 tss = ww + time - rdis
         if (ider.eq.0) then
            densj(i) = dens(tss,tau,var,amp,0)
         else if (ider.eq.3) then
            densj(i) = dens(tss,tau,var,amp,1)
         else if (ider.eq.1) then 
            df=1.d0/(time-tss)
            densj(i) = rx*df*(dens(tss,tau,var,amp,1)
     &                     +dens(tss,tau,var,amp,0)*df)
         else if (ider.eq.2) then
            df=1.d0/(time-tss)
            densj(i) = ry*df*(dens(tss,tau,var,amp,1)
     &                     +dens(tss,tau,var,amp,0)*df)
         end if
100   continue
      ml = 8
      mr = 8
      call wavgrcor(ier,h,rdis,densj,nnodes,pinc,ml,mr)
      if (ier.ne.0) then
	 print *,' error in singquad from wavgrcor'
	 print *,' ier =  ',ier
	 print *,' x = ',x
	 print *,' y = ',y
      endif
      return
      end
c
C************************************************
      double precision function dens(t,tau,var,amp,ider)
      implicit double precision (a-h,o-z)
      double precision t,tau,var,amp
c
      if (ider.eq.0) then
        dens = amp*dexp( - var*(t-tau)*(t-tau))
      else if (ider.eq.1) then
        dens = -2.d0*amp*var*(t-tau)*dexp( - var*(t-tau)*(t-tau))
      end if 
      return
      end




c=================================================================================
c====================== file quadepc.f ===========================================
c=================================================================================

ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c       this is the end of the debugging code and the beginning
c       of the actual quadrature routines
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c
c
c
        subroutine wavgrcor(ier,h,u,sigma,n,
     1    trap,ml,mr)
        implicit real *8 (a-h,o-z)
        dimension sigma(1),whtsl(12),whtsr(12)
        data ml7/-2431034/,mr7/-2431034/,tt7/-2341034.0d0/,
     1      zermach/1.0d-18/
c
c       this subroutine uses the end-point corrected 
c       trapezoidal rule to evaluate the integral 
c
c       \int_{-a^0} sigma(t) /sqrt((t-u)^2-u^2),        (1)
c
c       with a in (1) defined by the formula
c
c       a=(n-1)*h,
c
c       and h,u, and sigma supplied by the user.
c
c                input parameters:
c
c  h - the sampling interval at which the function sigma
c       to be integrated has been discretized
c  u - the parameter in  the formula (1)
c  sigma - the table of values of the function to be integrated.
c       it is assumed to be tabulated at the points 
c
c        -a, -a+h, -a+2*h, . . . , -h, 0.
c
c  n - the number of elements in the array sigma
c  ml - the order of the lef-tend correction to be used. 
c       permitted values: 2,4,6,8,10,12.
c  mr - the order of the right-end correction.
c       permitted values: 2,3,4,5,6,7,8.
c 
c                 output parameters:
c
c  ier - error return code;
c        ier=0 means sucessful completion
c        ier=4 means that the order ml of the left-end correction
c              is not a permitted one (see above)
c        ier=8 means that the order mr of the right-end correction
c              is not a permitted one (see above)
c        ier=16 means that the ratio h/u is outside the 
c              interval [0,50000].
c              any one of the above errors is fatal.
c  trap - the value of the integral.
c
c        . . . obtain the correction weights for the 
c              left (non-singular) end
c
        if (ml .ne. ml7) call alptrap0(ier,ml,whtsl)
          if(ier. eq. 4) return
          ml7=ml
cccc         call prin2('after alptrap0, whtsl=*',whtsl,ml7)        
c
c        obtain the correction weights for the 
c        right (singular) end
c
         tt=h/u
cccc          call prin2('in wavgrcor, h/u=*',tt,1)
cccc          call prin2('in wavgrcor, h=*',h,1)
cccc          call prin2('in wavgrcor, u=*',u,1)
c
        if( (mr .eq. mr7) .and. ( dabs(tt-tt7) .lt. zermach) )
     1      goto 1200
         call corwhtsg(ier,tt,mr,whtsr)     
         if(ier .ne. 0) return
 1100 continue
c
          mr7=mr
          tt7=tt
 1200 continue
c         call prin2('after corwhtsg, whtsr=*',whtsr,mr7)        
c
c        construct the trapezoidal approximation to the
c        integral to be evaluated
c
        a=-(n-1)*h
        trap=0
        do 1400 i=1,n-1
        tt=a+(i-1)*h
        trap=trap+sigma(i)/dsqrt((u-tt)**2-u**2)
 1400 continue
        trap=trap-sigma(1)/dsqrt((u-a)**2-u**2)/2
c
c        apply the left-end corrections
c
        corrl=0
        do 1600 i=1,ml-1
        tt=a+(i-1)*h
        corrl=corrl+whtsl(i)*sigma(i)/dsqrt((u-tt)**2-u**2)
 1600 continue
cccc         call prin2('corrl=*',corrl*h,1)
c
c     
        trap=(trap+corrl)*h
cccc        call prin2('trap before right-end corrections*',
cccc     1      trap,1)
c
c        apply the right-hand corrections
c
        corrr=0
        do 1800 i=1,mr
        corrr=corrr+whtsr(i)*sigma(n-i+1)
 1800 continue
cccc         call prin2('corrr=*',corrr,1)
        trap=trap+corrr
        return
        end
c
c
c
c
c
        subroutine corwhtsg(ier,t,m,whts)
        implicit real *8 (a-h,o-z)
        dimension whts(*)
        dimension p101(23),p201(23),p202(23),
     1      p301(23),p302(23),p303(23),
     2      p401(23),p402(23),p403(23),p404(23),
     3      p501(23),p502(23),p503(23),p504(23),p505(23),
     4      p601(23),p602(23),p603(23),p604(23),p605(23),
     5      p606(23),
     6      p701(23),p702(23),p703(23),p704(23),p705(23),
     7      p706(23),p707(23),
     6      p801(23),p802(23),p803(23),p804(23),p805(23),
     7      p806(23),p807(23),p808(23)
c
        dimension q101(30),q201(30),q202(30),
     1      q301(30),q302(30),q303(30),
     2      q401(30),q402(30),q403(30),q404(30),
     3      q501(30),q502(30),q503(30),q504(30),q505(30),
     4      q601(30),q602(30),q603(30),q604(30),q605(30),
     5      q606(30),
     6      q701(30),q702(30),q703(30),q704(30),q705(30),
     7      q706(30),q707(30),
     6      q801(30),q802(30),q803(30),q804(30),q805(30),
     7      q806(30),q807(30),q808(30)
c
c
        data p101/
     1  0.50528256125956521D+00,  0.49984531584054604D+00,
     2  -.64522798766779468D-02,  -.98011442659071395D-03,
     3  0.39300703305137581D-04,  0.44226348802363087D-05,
     4  0.67258627391068816D-08,  -.41492199713248596D-07,
     5  -.59103711584782068D-08,  0.51606555654990238D-09,
     6  0.17702795579349418D-09,  0.12969963078617962D-11,
     7  -.53226599917469729D-11,  -.48090435804635874D-12,
     8  0.16661864910536008D-12,  0.30117328301547917D-13,
     9  -.52048504538626966D-14,  -.17045845252088460D-14,
     *  0.19289646409639044D-15,  0.78126794946595571D-16,
     *  0.16991748178893818D-17,  -.85898314642767047D-17,
     *  0.14288716250960691D-17/
c
        data p201/
     1  0.43330074727338927D+00,  0.42863764761426144D+00,
     2  -.55102251310106078D-02,  -.80738231028906033D-03,
     3  0.43818007695321867D-04,  0.38894316724961839D-05,
     4  -.22920545009434519D-06,  -.46048965288331647D-07,
     5  -.12919793461051276D-08,  0.10843166826491352D-08,
     6  0.96738642484545900D-10,  -.26607749693003065D-10,
     7  -.45403578297577631D-11,  0.68726818282176050D-12,
     8  0.21862292463944206D-12,  -.19569578481254003D-13,
     9  -.10902766740038764D-13,  0.61683907789900925D-15,
     *  0.54899150755069461D-15,  -.25945192228679084D-17,
     *  -.39712772918025239D-16,  0.33280546921544252D-17,
     *  0.12855779864244668D-17/
        data p202/
     1  0.71981813986175937D-01,  0.71207668226284598D-01,
     2  -.94205474566733901D-03,  -.17273211630165362D-03,
     3  -.45173043901842857D-05,  0.53320320774012475D-06,
     4  0.23593131283345207D-06,  0.45567655750830515D-08,
     5  -.46183918123730792D-08,  -.56825112609923283D-09,
     6  0.80289313308948278D-10,  0.27904746000864861D-10,
     7  -.78230216198920986D-12,  -.11681725408681192D-11,
     8  -.52004275534081978D-13,  0.49686906782801919D-13,
     9  0.56979162861760670D-14,  -.23214236031078553D-14,
     *  -.35609504345430420D-15,  0.80721314169463491D-16,
     *  0.41411947735914615D-16,  -.11917886156431128D-16,
     *  0.14329363867160180D-18/
c
        data p301/
     1  0.40200863096247487D+00,  0.39782218891211499D+00,
     2  -.49383711038219853D-02,  -.71295824325411473D-03,
     3  0.42538128080256277D-04,  0.32931503759021181D-05,
     4  -.31165167850830799D-06,  -.36993780098389729D-07,
     5  0.14473828997375425D-08,  0.99359955783686049D-09,
     6  0.85369390110737665D-11,  -.30197963536667976D-10,
     7  -.12127671316465991D-11,  0.10321068794918520D-11,
     8  0.83295680937748396D-13,  -.42471076177184154D-13,
     9  -.49537903991904656D-14,  0.21134094124956069D-14,
     *  0.23149101471745579D-15,  -.84862562370085021D-16,
     *  -.29707338450983633D-16,  0.12853653480881091D-16,
     *  -.11536690153924342D-17/
        data p302/
     1  0.13456604660800474D+00,  0.13283858563057751D+00,
     2  -.20857628000445841D-02,  -.36158025037154482D-03,
     3  -.19575451600531067D-05,  0.17257658009282563D-05,
     4  0.40082376966137768D-06,  -.13553604804800786D-07,
     5  -.10097116304058419D-07,  -.38681687647468337D-09,
     6  0.25669272025589254D-09,  0.35085173688194684D-10,
     7  -.74374835582115379D-11,  -.18578499342083022D-11,
     8  0.21865021186930535D-12,  0.95489902174662222D-13,
     9  -.62000363955205289D-14,  -.53145642723010507D-14,
     *  0.27890594221217350D-15,  0.24525740046389770D-15,
     *  0.21401078801831403D-16,  -.30969083733884457D-16,
     *  0.50217876423054036D-17/
        data p303/
     1  -.31292116310914402D-01,  -.30815458702146458D-01,
     2  0.57185402718862252D-03,  0.94424067034945599D-04,
     3  -.12798796150655895D-05,  -.59628129659406576D-06,
     4  -.82446228413962801D-07,  0.90551851899419187D-08,
     5  0.27393622458426701D-08,  -.90717124812274730D-10,
     6  -.88201703473472134D-10,  -.35902138436649115D-11,
     7  0.33275906981111640D-11,  0.34483869667009147D-12,
     8  -.13532724370169367D-12,  -.22901497695930150D-13,
     9  0.59489763408482972D-14,  0.14965703345965982D-14,
     *  -.31750049283323912D-15,  -.82268043147216958D-16,
     *  0.10005434467041536D-16,  0.95255987887266910D-17,
     *  -.24392470018169074D-17/
c
        data p401/
     1  0.38316907896988200D+00,  0.37930649527009290D+00,
     2  -.45525259561749256D-02,  -.65264381395680663D-03,
     3  0.40475143961491376D-04,  0.28695319064518219D-05,
     4  -.33774812204435659D-06,  -.28683869129303836D-07,
     5  0.26992681913789196D-08,  0.79251104336897406D-09,
     6  -.37773815319622207D-10,  -.25683813163605155D-10,
     7  0.85500161224808019D-12,  0.92095032610367394D-12,
     8  -.17629980020550849D-13,  -.39702776558954617D-13,
     9  0.44791132548874533D-15,  0.20189174334016327D-14,
     *  -.61036035042332778D-16,  -.94121337137455023D-16,
     *  -.44336324755514916D-17,  0.10381640954278709D-16,
     *  -.18913662417434463D-17/
        data p402/
     1  0.19108470258578335D+00,  0.18838566655664377D+00,
     2  -.32432982429857632D-02,  -.54252353826346911D-03,
     3  0.42314071962415952D-05,  0.29966212092791451D-05,
     4  0.47911310026952345D-06,  -.38483337712058464D-07,
     5  -.13852772178982551D-07,  0.21644866692897589D-09,
     6  0.39562498324798047D-09,  0.21542722569006219D-10,
     7  -.13640789789895573D-10,  -.15243802740437696D-11,
     8  0.52142719474420411D-12,  0.87185003319972968D-13,
     9  -.22405141569557784D-13,  -.50310883350193318D-14,
     *  0.11564870914916382D-14,  0.27303372476596578D-15,
     *  -.54420039124450291D-16,  -.23553046154081202D-16,
     *  0.72348793213590460D-17/
        data p403/
     1  -.87810772288693007D-01,  -.86362539628212714D-01,
     2  0.17293894701298017D-02,  0.27536735492686989D-03,
     3  -.74688319713602914D-05,  -.18671367049449546D-05,
     4  -.16073555902210858D-06,  0.33984918097199597D-07,
     5  0.64950181207668013D-08,  -.69398266821593401D-09,
     6  -.22713396646556005D-09,  0.99522372755235473D-11,
     7  0.95308969297952042D-11,  0.11369036505555738D-13,
     8  -.43810422657659030D-12,  -.14596598841242232D-13,
     9  0.22154081514886341D-13,  0.12130943973144479D-14,
     *  -.11950816421124889D-14,  -.11004436744937988D-15,
     *  0.85826552393358711D-16,  0.21095612089131337D-17,
     *  -.46523386808686348D-17/
        data p404/
     1  0.18839551992592868D-01,  0.18515693642022085D-01,
     2  -.38584514764705973D-03,  -.60314429297308096D-04,
     3  0.20629841187649006D-05,  0.42361846945029628D-06,
     4  0.26096443536048593D-07,  -.83099109690858928D-08,
     5  -.12518852916413771D-08,  0.20108851446788642D-09,
     6  0.46310754330695973D-10,  -.45141503730628207D-11,
     7  -.20677687438946793D-11,  0.11115655338817805D-12,
     8  0.10092566095829922D-12,  -.27682996182295159D-14,
     9  -.54017017246792279D-14,  0.94491979093986633D-16,
     *  0.29252704975978028D-15,  0.92587747673748692D-17,
     *  -.25273705975434542D-16,  0.24720125266032935D-17,
     *  0.73769722635078298D-18/
c
        data p501/
     1  0.37009382861169867D+00,  0.36646910479988065D+00,
     2  -.42703162501783672D-02,  -.60999864513988079D-03,
     3  0.38480051846286758D-04,  0.25685944859667446D-05,
     4  -.34271461858039352D-06,  -.22456318105212614D-07,
     5  0.32440887566778566D-08,  0.61249519667162698D-09,
     6  -.58764584235172455D-10,  -.20237423461379761D-10,
     7  0.18455277089267579D-11,  0.71541629395775082D-12,
     8  -.68268849598518754D-13,  -.30131951767476527D-13,
     9  0.32252683400002717D-14,  0.14560898752058798D-14,
     *  -.19708449794871251D-15,  -.72592941031732687D-16,
     *  0.11391610076774729D-16,  0.51639308111063088D-17,
     *  -.15397121825739558D-17/
        data p502/
     1  0.24338570401851668D+00,  0.23973522843749278D+00,
     2  -.43721370669719969D-02,  -.71310421353117247D-03,
     3  0.12211775657060070D-04,  0.42003708912194540D-05,
     4  0.49897908641367119D-06,  -.63393541808423343D-07,
     5  -.16032054440178305D-07,  0.93651205371836948D-09,
     6  0.47958805891017734D-09,  -.24283623989225277D-12,
     7  -.17602894176612541D-10,  -.70224414545850734D-12,
     8  0.72398267305503524D-12,  0.48901704154712942D-13,
     9  -.33514569627987123D-13,  -.27797781020279958D-14,
     *  0.17006809430142139D-14,  0.18692014038809688D-15,
     *  -.11772100935040912D-15,  -.26822055766227299D-17,
     *  0.58282630838119908D-17/
        data p503/
     1  -.16626227443779300D+00,  -.16338688244948623D+00,
     2  0.34226477061091522D-02,  0.53123836782842493D-03,
     3  -.19439384662588004D-04,  -.36727612278554180D-05,
     4  -.19053453823833018D-06,  0.71350224241746909D-07,
     5  0.97639415125604384D-08,  -.17740777484000285D-08,
     6  -.35307857995885214D-09,  0.42630575488868786D-10,
     7  0.15474053509872475D-10,  -.12218351563736230D-11,
     8  -.74193744404197235D-12,  0.42828349906097840D-13,
     9  0.38818223602857911D-13,  -.21638709523529133D-14,
     *  -.20113724193061804D-14,  0.19126009077576290D-16,
     *  0.18077800774716789D-15,  -.29196699661561792D-16,
     *  -.25424143237638603D-17/
        data p504/
     1  0.71140553425326200D-01,  0.69865255522871098D-01,
     2  -.15146839716332934D-02,  -.23089510456501146D-03,
     3  0.10043352579583376D-04,  0.16273681513906052D-05,
     4  0.45962429680196321D-07,  -.33220115065450765D-07,
     5  -.34311675528371377D-08,  0.92115190125728475D-09,
     6  0.13027382999288911D-09,  -.26299709181958440D-10,
     7  -.60298731306137401D-11,  0.93329268197491237D-12,
     8  0.30348113926814445D-12,  -.41051598782865159D-13,
     9  -.16511129783478901D-13,  0.23458022122884216D-14,
     *  0.83672090118119335D-15,  -.76854809565945949D-16,
     *  -.88574676217965336D-16,  0.23342853108825500D-16,
     *  -.66891901206519121D-18/
        data p505/
     1  -.13075250358183333D-01,  -.12837390470212253D-01,
     2  0.28220970599655842D-03,  0.42645168816925840D-04,
     3  -.19950921152046187D-05,  -.30093742048507723D-06,
     4  -.49664965360369326D-08,  0.62275510240912185D-08,
     5  0.54482056529893970D-09,  -.18001584669734922D-09,
     6  -.20990768915548566D-10,  0.54463897022241182D-11,
     7  0.99052609667960994D-12,  -.20553403214657538D-12,
     8  -.50638869577533124D-13,  0.95708247912039832D-14,
     9  0.27773570146734278D-14,  -.56282755828421539D-15,
     *  -.13604846286245327D-15,  0.21528396086424329D-16,
     *  0.15825242559495001D-16,  -.52177101452330925D-17,
     *  0.35165405954646610D-18/
c
        data p601/
     1  0.36026189831727993D+00,  0.35682151129965697D+00,
     2  -.40522092269424743D-02,  -.57778313715337711D-03,
     3  0.36704155364822447D-04,  0.23466764282024286D-05,
     4  -.33924124124753531D-06,  -.17896565762139828D-07,
     5  0.34589031502765911D-08,  0.47118497381990998D-09,
     6  -.67063477022561001D-10,  -.15602365413131373D-10,
     7  0.22427507199609393D-11,  0.52849191316189723D-12,
     8  -.88516750676765319D-13,  -.20939711433525934D-13,
     9  0.42627815832827932D-14,  0.91755271389162214D-15,
     *  -.23510311731680511D-15,  -.47030603898825992D-16,
     *  0.17430226971070333D-16,  0.12085554311374334D-17,
     *  -.97246431706866265D-18/
        data p602/
     1  0.29254535549061033D+00,  0.28797319593861118D+00,
     2  -.54626721831514611D-02,  -.87418175346369085D-03,
     3  0.21091258064381618D-04,  0.53099611800410394D-05,
     4  0.48161219974937536D-06,  -.86192303523783195D-07,
     5  -.17106126408175377D-07,  0.16430631679797051D-08,
     6  0.52108252284496418D-09,  -.23418126479502088D-10,
     7  -.19589009232972592D-10,  0.23237775935087423D-12,
     8  0.82522217789419828D-12,  0.29405028322178733D-14,
     9  -.38702136049044922D-13,  -.87092183891702935D-16,
     *  0.18907739845835885D-14,  0.59108478947710371D-16,
     *  -.14791410279764733D-15,  0.17094673895734355D-16,
     *  0.29920232877235122D-17/
        data p603/
     1  -.26458157738198032D+00,  -.25986281745172303D+00,
     2  0.56037179384680806D-02,  0.85339344769346168D-03,
     3  -.37198349477231092D-04,  -.58919418054985949D-05,
     4  -.15580076490973300D-06,  0.11694774767246189D-06,
     5  0.11912085448558517D-07,  -.31871799769258856D-08,
     6  -.43606750782592701D-09,  0.88981155966195295D-10,
     7  0.19446283623973022D-10,  -.30910789669568183D-11,
     8  -.94441645307838455D-12,  0.13475075214699878D-12,
     9  0.49193356683291785D-13,  -.75492429186418280D-14,
     *  -.23915584379892903D-14,  0.27474930369140799D-15,
     *  0.24116420512107679D-15,  -.68750461611109009D-16,
     *  0.31300658159034521D-17/
        data p604/
     1  0.16945985636951351D+00,  0.16634119052510789D+00,
     2  -.36957542039922218D-02,  -.55305018443004820D-03,
     3  0.27802317394226458D-04,  0.38465487290337870D-05,
     4  0.11228656351594859D-07,  -.78817638496162086D-07,
     5  -.55793114888382598D-08,  0.23342541297855962D-08,
     6  0.21326275785804722D-09,  -.72650289657839255D-10,
     7  -.10002103245763630D-10,  0.28025364932878412D-11,
     8  0.50596014782107579D-12,  -.13297400072077277D-12,
     9  -.26886263041836712D-13,  0.77311742752455627D-14,
     *  0.12169068721270196D-14,  -.33247808332047918D-15,
     *  -.14896088129927675D-15,  0.62896617261661699D-16,
     *  -.63413995519913150D-17/
        data p605/
     1  -.62234901830276989D-01,  -.61075357971330651D-01,
     2  0.13727448221760226D-02,  0.20372270874944422D-03,
     3  -.10874574522526163D-04,  -.14105277093066651D-05,
     4  0.12400390128261134D-07,  0.29026312739449159D-07,
     5  0.16188925332976026D-08,  -.88656696095997081D-09,
     6  -.62485232849328622D-10,  0.28621679941072627D-10,
     7  0.29766411535937342D-11,  -.11401559373423083D-11,
     8  -.15187837416003496D-12,  0.55532025952428576D-13,
     9  0.79649235306736064D-14,  -.32555135281292727D-14,
     *  -.32614147883807135D-15,  0.14934004631939501D-15,
     *  0.46018340155968152D-16,  -.24994590805828494D-16,
     *  0.31878940718628344D-17/
        data p606/
     1  0.98319302944187313D-02,  0.96475935002236796D-02,
     2  -.21810702323589283D-03,  -.32215507986503675D-04,
     3  0.17758964814643083D-05,  0.22191805776431820D-06,
     4  -.34733773328601586D-08,  -.45597523430711214D-08,
     5  -.21481439360012131D-09,  0.14131022285283862D-09,
     6  0.82988927865098628D-11,  -.46350580475835077D-11,
     7  -.39722301151837332D-12,  0.18692438113369132D-12,
     8  0.20247900853674834D-13,  -.91922401927588863D-14,
     9  -.10375133264515801D-14,  0.53853720663526987D-15,
     *  0.38018596924661779D-16,  -.25562327300340154D-16,
     *  -.60386205361538368D-17,  0.39553764233520265D-17,
     *  -.56724805546335572D-18/
c
        data p701/
     1  0.35247817820683391D+00,  0.34918624146513627D+00,
     2  -.38769020757911182D-02,  -.55230632312534049D-03,
     3  0.35150566903808214D-04,  0.21768154268447994D-05,
     4  -.33235584173455589D-06,  -.14518659777759600D-07,
     5  0.35152344101140872D-08,  0.36322208075761718D-09,
     6  -.69195754022074977D-10,  -.11973492593978056D-10,
     7  0.23434617619927368D-11,  0.38167416480465596D-12,
     8  -.93325175292000960D-13,  -.13752533133410646D-13,
     9  0.44167456796058789D-14,  0.51391475407940881D-15,
     *  -.22796464630521982D-15,  -.26174083249249870D-16,
     *  0.17989049560661017D-16,  -.10785266802654498D-17,
     *  -.50771902445478869D-18/
        data p702/
     1  0.33924767615328649D+00,  0.33378481494573535D+00,
     2  -.65145150900595978D-02,  -.10270426376319107D-02,
     3  0.30412788830467100D-04,  0.63291271881867366D-05,
     4  0.44029980267156772D-06,  -.10645973943012391D-06,
     5  -.17444113967150591D-07,  0.22908405263129271D-08,
     6  0.53387618487404293D-09,  -.45191363418819914D-10,
     7  -.20193275467255936D-10,  0.11132842369015162D-11,
     8  0.85407273402112383D-12,  -.40182572311768522D-13,
     9  -.39625917456880465D-13,  0.23347338357087857D-14,
     *  0.18479440253687965D-14,  -.66031027017768781D-16,
     *  -.15126689602409390D-15,  0.30817125576673638D-16,
     *  0.20355903588807294D-18/
        data p703/
     1  -.38133737903867071D+00,  -.37439186496953346D+00,
     2  0.82333252057384195D-02,  0.12355456581140139D-02,
     3  -.60502176392447278D-04,  -.84398568258606022D-05,
     4  -.52519772217179432D-07,  0.16761633743999702D-06,
     5  0.12757054344594160D-07,  -.48066233716245738D-08,
     6  -.46805166378744473D-09,  0.14341424898715459D-09,
     7  0.20956948719734579D-10,  -.52933448189198254D-11,
     8  -.10165430707169080D-11,  0.24255858295215088D-12,
     9  0.51502725986686035D-13,  -.13603762068522731D-13,
     *  -.22845062732341660D-14,  0.58760802965539190D-15,
     *  0.24954249820610671D-15,  -.10305553349181248D-15,
     *  0.10101033917290650D-16/
        data p704/
     1  0.32513425857843403D+00,  0.31904658721552181D+00,
     2  -.72018972270193551D-02,  -.10625864649907705D-02,
     3  0.58874086614501754D-04,  0.72437687561948076D-05,
     4  -.12647933391541235D-06,  -.14637575817741076D-06,
     5  -.67059366908852725D-08,  0.44935119953179171D-08,
     6  0.25590829448826150D-09,  -.14522774349685114D-09,
     7  -.12016325940249025D-10,  0.57388927547481731D-11,
     8  0.60212777921215436D-12,  -.27671702512132601D-12,
     9  -.29965864188230867D-13,  0.15804107521503693D-13,
     *  0.10740512140877355D-14,  -.74957069588656469D-15,
     *  -.16015134061790751D-15,  0.10864227444944548D-15,
     *  -.15637036732214482D-16/
        data p705/
     1  -.17899070348696739D+00,  -.17560440548914107D+00,
     2  0.40023520894463541D-02,  0.58587491917000341D-03,
     3  -.34178401437748776D-04,  -.39584427296628797D-05,
     4  0.11568138281572151D-06,  0.79694902511347132D-07,
     5  0.24638614256977648D-08,  -.25060103527172931D-08,
     6  -.94469391116099891D-10,  0.83054774707114198D-10,
     7  0.44873049779584770D-11,  -.33424209018143972D-11,
     8  -.22400558200186118D-12,  0.16334022798724341D-12,
     9  0.10274074069820577D-13,  -.93099134198845016D-14,
     *  -.21914839448477220D-15,  0.46222466506504368D-15,
     *  0.54387039578337173D-16,  -.59296913213342474D-16,
     *  0.10158361419327377D-16/
        data p706/
     1  0.56534250957094889D-01,  0.55459212507347850D-01,
     2  -.12699499301440283D-02,  -.18507639215472461D-03,
     3  0.11097427247550824D-04,  0.12410840659090837D-05,
     4  -.44785774409848276D-07,  -.24827188250114087D-07,
     5  -.55280195198993366D-09,  0.78908758071223001D-09,
     6  0.21092555187111101D-10,  -.26408295268274120D-10,
     7  -.10014890407085482D-11,  0.10678307154527383D-11,
     8  0.49098552277615276D-13,  -.52315375304566736D-13,
     9  -.19612593799428070D-14,  0.29603439449300072D-14,
     *  -.48018065572197613D-17,  -.15070602275220233D-15,
     *  -.93898609579335588D-17,  0.17677382938607994D-16,
     *  -.33556312036136099D-17/
        data p707/
     1  -.77837201104460262D-02,  -.76352698345206953D-02,
     2  0.17530715115135607D-03,  0.25476814028036679D-04,
     3  -.15535884610142866D-05,  -.16986100135758060D-06,
     4  0.68853995129366854D-08,  0.33779059844168349D-08,
     5  0.56331259806992587D-10,  -.10796289303761339D-09,
     6  -.21322770188560036D-11,  0.36288728337952147D-11,
     7  0.10071103136433175D-12,  -.14681774091089448D-12,
     8  -.48084295672353406D-14,  0.71871814148536297D-14,
     9  0.15396226081938662D-15,  -.40363695919778461D-15,
     *  0.71379753084953742D-17,  0.20856737897420155D-16,
     *  0.55874209652184727D-18,  -.22870590429516399D-17,
     *  0.46474109133108029D-18/
c
        data p801/
     1  0.34609100481750591D+00,  0.34292208867100209D+00,
     2  -.37318314296140241D-02,  -.53147584026066619D-03,
     3  0.33791123650068993D-04,  0.20425925271775796D-05,
     4  -.32421136522748381D-06,  -.11967163133679078D-07,
     5  0.34937970243714131D-08,  0.28061829827655149D-09,
     6  -.68366995494285438D-10,  -.91923685597988143D-11,
     7  0.23028832887544356D-11,  0.27110379414342729D-12,
     8  -.90644143379760780D-13,  -.86626853822221206D-14,
     9  0.42715750760175652D-14,  0.18551719737782837D-15,
     *  -.17871215594978621D-15,  -.22259909051924421D-16,
     *  0.20276528656282653D-16,  -.33572765889356793D-17,
     *  0.20102137103231888D-19/
        data p802/
     1  0.38395788987858284D+00,  0.37763388450467387D+00,
     2  -.75300096132985019D-02,  -.11728560176853438D-02,
     3  0.39928891607300055D-04,  0.72686874852637166D-05,
     4  0.38328846764401395D-06,  -.12432021638585482D-06,
     5  -.17294051894277890D-07,  0.28690667021099639D-08,
     6  0.52807511157272647D-09,  -.64659410641030046D-10,
     7  -.19909095730464139D-10,  0.18871857726986636D-11,
     8  0.83536607821875765D-12,  -.75849609622441096D-13,
     9  -.38587265335771171D-13,  0.46212719016625720D-14,
     *  0.15092436265716961D-14,  -.96089597308233339D-16,
     *  -.16629378522619455D-15,  0.46485911934729562D-16,
     *  -.34397380683560857D-17/
        data p803/
     1  -.51546802021455997D+00,  -.50593907364634860D+00,
     2  0.11279808775454743D-01,  0.16729857982746810D-02,
     3  -.89050484723285592D-04,  -.11258537716785521D-04,
     4  0.11851423259637265D-06,  0.22119776853774840D-06,
     5  0.12306867933820058D-07,  -.65413017435165512D-08,
     6  -.45064856577941797D-09,  0.20181848294941538D-09,
     7  0.20104342251544055D-10,  -.76150024669581244D-11,
     8  -.96045433928881730D-12,  0.34957934607891111D-12,
     9  0.48375186866910125D-13,  -.20457060753825128D-13,
     *  -.12715343569511915D-14,  0.67915542810914937D-15,
     *  0.29411485258349565D-15,  -.14991619158211513D-15,
     *  0.21004384880644027D-16/
        data p804/
     1  0.54868532720491546D+00,  0.53829193501021505D+00,
     2  -.12279369843214522D-01,  -.17916533652573258D-02,
     3  0.10645460049810283D-03,  0.11941570242087814D-04,
     4  -.41153600949990808D-06,  -.23567814257342285D-06,
     5  -.59556266453474692D-08,  0.73846431322356806D-08,
     6  0.22690272584758060D-09,  -.24256782660517708D-09,
     7  -.10595538718072817D-10,  0.96084782320524517D-11,
     8  0.50854276695467002D-12,  -.45501967498056677D-12,
     9  -.24791775742424605D-13,  0.27247248618754132D-13,
     *  -.62462769347407206D-15,  -.89759485658548020D-15,
     *  -.23612631441281426D-15,  0.18722708019547662D-15,
     *  -.33897389048153233D-16/
        data p805/
     1  -.40254177211344629D+00,  -.39484975328383930D+00,
     2  0.90798247056463420D-02,  0.13149418194320021D-02,
     3  -.81758915317142304D-04,  -.86562442193491703D-05,
     4  0.40073806173601772D-06,  0.16899728404935904D-06,
     5  0.17135537621789908D-08,  -.53971434172956625D-08,
     6  -.65462311338489741D-10,  0.18039371359856979D-09,
     7  0.30673515960237272D-11,  -.72125885831501049D-11,
     8  -.13003329323986878D-12,  0.34139922715709575D-12,
     9  0.52436016278531830D-14,  -.20831363428918545D-13,
     *  0.15183328649424933D-14,  0.59323981217014003D-15,
     *  0.13666526818282947D-15,  -.13968849646903142D-15,
     *  0.28747836208082941D-16/
        data p806/
     1  0.19066489213298250D+00,  0.18700642118416626D+00,
     2  -.43164334998635114D-02,  -.62251653231240557D-03,
     3  0.39645735575631885D-04,  0.40597649593196278D-05,
     4  -.21581978140908767D-06,  -.78408617475402234D-07,
     5  -.10261710168607160D-09,  0.25237672152946473D-08,
     6  0.36884674330179665D-11,  -.84811779888109861D-10,
     7  -.14942858928505769D-12,  0.33898695604227298D-11,
     8  -.72437194598023235D-14,  -.15917664366293875D-12,
     9  0.10722779022850576D-14,  0.98648935373412393D-14,
     *  -.10431663515749988D-14,  -.23112354102891561D-15,
     *  -.58086423416130627D-16,  0.65720121567269821D-16,
     *  -.14474292659335228D-16/
        data p807/
     1  -.52493933835742510D-01,  -.51484339393458951D-01,
     2  0.11908016743900095D-02,  0.17129019408170675D-03,
     3  -.11069691238066014D-04,  -.11094212982373027D-05,
     4  0.63896734366995756D-07,  0.21238383088818241D-07,
     5  -.93730937000148924D-10,  -.68618896851916337D-09,
     6  0.36687176255899571D-11,  0.23096979627680215D-10,
     7  -.18351212789622485D-12,  -.92068895126981515D-12,
     8  0.13878049457213364D-13,  0.42866915528793664D-13,
     9  -.89217537943127362D-15,  -.26860926008562951D-14,
     *  0.34381513147638579D-15,  0.51802355781488001D-16,
     *  0.15256851359912957D-16,  -.17861588495825527D-16,
     *  0.40908655397070871D-17/
        data p808/
     1  0.63871733893281162D-02,  0.62641527941339434D-02,
     2  -.14507064617686054D-03,  -.20830482864895017D-04,
     3  0.13594432539430194D-05,  0.13422289948349535D-06,
     4  -.81444763455139650D-08,  -.25514967824896542D-08,
     5  0.21437501093004940D-10,  0.82603689139916820D-10,
     6  -.82868536104816980D-12,  -.27811794309556037D-11,
     7  0.40618840076144202D-13,  0.11054218794577992D-12,
     8  -.26622864983418799D-14,  -.51016403283925694D-14,
     9  0.15212104542113486D-15,  0.32460797920756839D-15,
     *  -.47374863212003803D-16,  -.47371818316253473D-17,
     *  -.19825037091098511D-17,  0.21913358442293859D-17,
     *  -.51189872228465644D-18/
c
c
        data q101/
     1  0.22075960681663666D+01,  0.10860289711285256D+01,
     2  -.11416482212605705D+00,  0.10390799863197490D-01,
     3  0.17384721298677499D-03,  -.41296714982944868D-03,
     4  0.12220793636944143D-03,  -.20180011302231004D-04,
     5  0.22633828160826354D-06,  0.11695476199575691D-05,
     6  -.46665719537431450D-06,  0.10306330901393883D-06,
     7  -.72697529262121086D-08,  -.51319440362609173D-08,
     8  0.28789035172189644D-08,  -.86576395894108030D-09,
     9  0.14747733069555882D-09,  0.83888993210337589D-11,
     *  -.17565231454002767D-10,  0.77678929900404739D-11,
     *  -.21676499967625240D-11,  0.33609917889958228D-12,
     *  0.41605742045517789D-13,  -.54907725495493480D-13,
     *  0.24666751364757895D-13,  -.76597506514497855D-14,
     *  0.17211538190846807D-14,  -.23274398889201165D-15,
     *  -.59085645969635596D-17,  0.10674602715470736D-16/
c
        data q201/
     1  0.19195040386946797D+01,  0.96521123340885231D+00,
     2  -.91242273334822828D-01,  0.77888958208324493D-02,
     3  0.49017520468167370D-04,  -.25958294440697107D-03,
     4  0.81120106610952590D-04,  -.15920933695299930D-04,
     5  0.15710356527074733D-05,  0.31666139971896434D-06,
     6  -.22528568307976170D-06,  0.72326564071377389D-07,
     7  -.14971039253189135D-07,  0.11704967649338156D-08,
     8  0.65576134136033668D-09,  -.40567972519368879D-09,
     9  0.13911746849306896D-09,  -.32537082933791400D-10,
     *  0.36789994109900145D-11,  0.11413457998317050D-11,
     *  -.93520501568700915D-12,  0.37865492913166963D-12,
     *  -.10890702407091499D-12,  0.20808447690355994D-13,
     *  -.39760042371748203D-15,  -.18377696169327231D-14,
     *  0.10207362614886814D-14,  -.36275502603631022D-15,
     *  0.92883422460004400D-16,  -.15369619388323358D-16/
        data q202/
     1  0.28809202947168688D+00,  0.12081773771967334D+00,
     2  -.22922548791234221D-01,  0.26019040423650403D-02,
     3  0.12482969251860762D-03,  -.15338420542247761D-03,
     4  0.41087829758488844D-04,  -.42590776069310735D-05,
     5  -.13446973710992097D-05,  0.85288622023860471D-06,
     6  -.24137151229455280D-06,  0.30736744942561440D-07,
     7  0.77012863269770265D-08,  -.63024408011947329D-08,
     8  0.22231421758586277D-08,  -.46008423374739151D-09,
     9  0.83598622024898672D-11,  0.40925982254825159D-10,
     *  -.21244230864992782D-10,  0.66265471902087689D-11,
     *  -.12324449810755149D-11,  -.42555750232087348D-13,
     *  0.15051276611643278D-12,  -.75716173185849474D-13,
     *  0.25064351788475377D-13,  -.58219810345170625D-14,
     *  0.70041755759599934D-15,  0.13001103714429855D-15,
     *  -.98791987056967951D-16,  0.26044222103794090D-16/
c
        data q301/
     1  0.17987111357789038D+01,  0.91650694888514835D+00,
     2  -.81803609523881617D-01,  0.65451996222308844D-02,
     3  0.76602895443768800D-04,  -.21450927923700556D-03,
     4  0.64720009354091368D-04,  -.12702897148427252D-04,
     5  0.14166384084887214D-05,  0.14934512794088299D-06,
     6  -.14261174778005682D-06,  0.49373113314606793D-07,
     7  -.11478403473775698D-07,  0.15425455005504605D-08,
     8  0.15979607761108043D-09,  -.19560629599420890D-09,
     9  0.80663513860291231D-10,  -.23022840582769447D-10,
     *  0.45070080086744964D-11,  -.24581161657695434D-12,
     *  -.28172212735938123D-12,  0.16994833404291074D-12,
     *  -.63525254608793635D-13,  0.17870861362879935D-13,
     *  -.35967175191017145D-14,  0.26874348919310155D-15,
     *  0.17465504234047022D-15,  -.10649590912246126D-15,
     *  0.34688125504822735D-16,  -.68766328133793473D-17/
        data q302/
     1  0.52967783530323871D+00,  0.21822630676708127D+00,
     2  -.41799876413116641D-01,  0.50892964395681701D-02,
     3  0.69658942567404755D-04,  -.24353153576240862D-03,
     4  0.73888024272211289D-04,  -.10695150700676431D-04,
     5  -.10359028826617060D-05,  0.11875187637947674D-05,
     6  -.40671938289396256D-06,  0.76643646456102633D-07,
     7  0.71601476815015214D-09,  -.70465382724280228D-08,
     8  0.32150727033571402D-08,  -.88023109214635129D-09,
     9  0.12526777146804532D-09,  0.21897497552781254D-10,
     *  -.22900248060361746D-10,  0.94008620230260875D-11,
     *  -.25394107577307707D-11,  0.37485743994543044D-12,
     *  0.59749227192190071D-13,  -.69841000530897356D-13,
     *  0.31462585979243842D-13,  -.10035007246768712D-13,
     *  0.23925799958924216D-14,  -.38250719668339931D-15,
     *  0.17598606853395334D-16,  0.90582489539060916D-17/
        data q303/
     1  -.12079290291577591D+00,  -.48704284523703966D-01,
     2  0.94386638109412103D-02,  -.12436961986015649D-02,
     3  0.27585374975601430D-04,  0.45073665169965503D-04,
     4  -.16400097256861222D-04,  0.32180365468726788D-05,
     5  -.15439724421875187D-06,  -.16731627177808135D-06,
     6  0.82673935299704881D-07,  -.22953450756770597D-07,
     7  0.34926357794134372D-08,  0.37204873561664495D-09,
     8  -.49596526374925625D-09,  0.21007342919947989D-09,
     9  -.58453954632777727D-10,  0.95142423510219527D-11,
     *  0.82800859768448187D-12,  -.13871574164086593D-11,
     *  0.65348288832762792D-12,  -.20870659508875890D-12,
     *  0.45381769462121353D-13,  -.29375863274760591D-14,
     *  -.31991170953842324D-14,  0.21065131061258246D-14,
     *  -.84608121914821114D-15,  0.25625911691384893D-15,
     *  -.58195296955181641D-16,  0.84929865749439985D-17/
c
        data q401/
     1  0.17267074357040163D+01,  0.88769181313319153D+00,
     2  -.76271780923479291D-01,  0.57976566848058288D-02,
     3  0.10649982827777176D-03,  -.19293708571488014D-03,
     4  0.56027692069331225D-04,  -.10776454663975149D-04,
     5  0.12165428637632875D-05,  0.10073570459923612D-06,
     6  -.10775398638302349D-06,  0.37795617091627717D-07,
     7  -.90121865325910690D-08,  0.13539824303746316D-08,
     8  0.37380408385710988D-10,  -.11860225264373863D-09,
     9  0.53181029669439137D-10,  -.16164295629423715D-10,
     *  0.35608424737594679D-11,  -.41127379822202691D-12,
     *  -.10056107280565129D-12,  0.88701720052395561D-13,
     *  -.37473411786162682D-13,  0.11779190414722638D-13,
     *  -.28656247296671774D-14,  0.47072005254376761D-15,
     *  -.31242430794857439D-17,  -.33087063957584708D-16,
     *  0.13952539558118546D-16,  -.31192488955464695D-17/
        data q402/
     1  0.74568893552790124D+00,  0.30467171402295171D+00,
     2  -.58395362214323620D-01,  0.73319252518433368D-02,
     3  -.20031855934604129D-04,  -.30824811632878488D-03,
     4  0.99964976126491718D-04,  -.16474478154032738D-04,
     5  -.43561624848540429D-06,  0.13333470338197080D-05,
     6  -.51129266708506255D-06,  0.11137613512503986D-06,
     7  -.66826360554037345D-08,  -.64808490619005360D-08,
     8  0.35823197110332485D-08,  -.11112432221977621D-08,
     9  0.20771522404060160D-09,  0.13218626927440575D-11,
     *  -.20061751455616660D-10,  0.98972485679613052D-11,
     *  -.30828939213919605D-11,  0.61859728191697597D-12,
     *  -.18406301275702789D-13,  -.51565987686425466D-13,
     *  0.29269307610940230D-13,  -.10640936936820710D-13,
     *  0.29259178521522894D-14,  -.60273373217802889D-15,
     *  0.79805364693507857D-16,  -.22139027995925196D-17/
        data q403/
     1  -.33680400314043845D+00,  -.13514969177957441D+00,
     2  0.26034149612148189D-01,  -.34863250108767316D-02,
     3  0.11727617347761031D-03,  0.10979024573634177D-03,
     4  -.42477049111141652D-04,  0.89973640002289860D-05,
     5  -.75468387839505358D-06,  -.31314454180302197D-06,
     6  0.18724721949080486D-06,  -.57685939425707824D-07,
     7  0.10891286602967324D-07,  -.19364047491084185D-09,
     8  -.86321227142536457D-09,  0.44108555925089069D-09,
     9  -.14090140720533401D-09,  0.30089877211059149D-10,
     *  -.20104880070606037D-11,  -.18835439613438770D-11,
     *  0.11969660519888177D-11,  -.45244643706030442D-12,
     *  0.12353729793001421D-12,  -.21212599171947949D-13,
     *  -.10058387270806210D-14,  0.27124427961778227D-14,
     *  -.13794190754080789D-14,  0.47648565240847852D-15,
     *  -.12040205479529417D-15,  0.19765138328442612D-16/
        data q404/
     1  0.72003700074887512D-01,  0.28815135751956813D-01,
     2  -.55318286004023262D-02,  0.74754293742505556D-03,
     3  -.29896932834002961D-04,  -.21572193522125423D-04,
     4  0.86923172847601431D-05,  -.19264424844521024D-05,
     5  0.20009554472543390D-06,  0.48609423341646874D-07,
     6  -.34857761397033328D-07,  0.11577496222979076D-07,
     7  -.24662169411846289D-08,  0.18856307017582893D-09,
     8  0.12241566922536944D-09,  -.77004043350470267D-10,
     9  0.27482484190852093D-10,  -.68585449533457320D-11,
     *  0.94616553491502853D-12,  0.16546218164507257D-12,
     *  -.18116105455372994D-12,  0.81246613990515175D-13,
     *  -.26051842822630953D-13,  0.60916709481572966D-14,
     *  -.73109278943453713D-15,  -.20197656335066602D-15,
     *  0.17777928541995593D-15,  -.73408845164876528D-16,
     *  0.20735585946704175D-16,  -.37573839178328704D-17/
c
        data q501/
     1  0.16768849023417378D+01,  0.86775626851406852D+00,
     2  -.72490898422140984D-01,  0.52855956548252545D-02,
     3  0.13041306838981421D-03,  -.17987506910133269D-03,
     4  0.50528711883548975D-04,  -.95033231887606441D-05,
     5  0.10574844114623434D-05,  0.82732600183474592D-07,
     6  -.89340845901707331D-07,  0.31122837057714521D-07,
     7  -.74262122690197497D-08,  0.11530866872322505D-08,
     8  0.14250545836485928D-11,  -.83895518290627831D-10,
     9  0.39078503825186888D-10,  -.12142921662984090D-10,
     *  0.27855071279477128D-11,  -.38237663844593165D-12,
     *  -.42099911212371519D-13,  0.54554339426319465D-13,
     *  -.24660722724843333D-13,  0.81218409838795049D-14,
     *  -.21083378077309606D-14,  0.40556394851619403D-15,
     *  -.36148110618120579D-16,  -.11613662037992706D-16,
     *  0.67786327359017758D-17,  -.16686582785776777D-17/
        data q502/
     1  0.94497906897701511D+00,  0.38441389249944376D+00,
     2  -.73518892219676846D-01,  0.93801693717656342D-02,
     3  -.11568481638277393D-03,  -.36049618278297467D-03,
     4  0.12196089686962072D-03,  -.21567004054890759D-04,
     5  0.20061756071837229D-06,  0.14053594514827541D-05,
     6  -.58494522901032717D-06,  0.13806725526069264D-06,
     7  -.13026533109689012D-07,  -.56772660893310115D-08,
     8  0.37261411262414981D-08,  -.12500701596102053D-08,
     9  0.26412532741761060D-09,  -.14763633173014444D-10,
     *  -.16960410072369640D-10,  0.97816599288569242D-11,
     *  -.33167385677650796D-11,  0.75518680442128035D-12,
     *  -.69657057520980184D-13,  -.36936589963052933D-13,
     *  0.26240159923195364D-13,  -.10380312520710416D-13,
     *  0.30580133223068293D-14,  -.68862733985639727D-15,
     *  0.10850099198237515D-15,  -.80162652674677758D-17/
        data q503/
     1  -.63573920331410925D+00,  -.25476295949431248D+00,
     2  0.48719444620178028D-01,  -.65586911907601778D-02,
     3  0.26075561414986502D-03,  0.18816234541762646D-03,
     4  -.75470930225835149D-04,  0.16636152851516016D-04,
     5  -.17090345922007185D-05,  -.42116316829759112D-06,
     6  0.29772606237870181D-06,  -.97722619629186999D-07,
     7  0.20407132184395240D-07,  -.13990149337651286D-08,
     8  -.10789443942377389D-08,  0.64932596536955549D-09,
     9  -.22551656227084750D-09,  0.54218121009696902D-10,
     *  -.66625000819311342D-11,  -.17101610026873055D-11,
     *  0.15477330215484963D-11,  -.65733072081676099D-12,
     *  0.20041343229793031D-12,  -.43156695757006748D-13,
     *  0.35378828045366781D-14,  0.23215061720123822D-14,
     *  -.15775622806398887D-14,  0.60532606392603100D-15,
     *  -.16344549572859507D-15,  0.28468682030255484D-16/
        data q504/
     1  0.27129383352400138D+00,  0.10855731422844886D+00,
     2  -.20655358605755553D-01,  0.27957870573473530D-02,
     3  -.12554989328217277D-03,  -.73820259976315213D-04,
     4  0.30688238027889141D-04,  -.70189683853101227D-05,
     5  0.83632935392921048D-06,  0.12062184100469297D-06,
     6  -.10851032332229796D-06,  0.38268616358631859D-07,
     7  -.88101139954699063D-08,  0.99214604274535344D-09,
     8  0.26623708443361902D-09,  -.21583098076291346D-09,
     9  0.83892587567861089D-10,  -.22944040819104234D-10,
     *  0.40475069181620489D-11,  0.49873542540691525D-13,
     *  -.41500570092684901D-12,  0.21783613649481955D-12,
     *  -.77302599067908347D-13,  0.20721068671529829D-13,
     *  -.37602404771794025D-14,  0.58647852759627191D-16,
     *  0.30987475557449598D-15,  -.15930245284324497D-15,
     *  0.49431213235571493D-16,  -.95597463857081367D-17/
        data q505/
     1  -.49822533362278467D-01,  -.19935544619123012D-01,
     2  0.37808825013383066D-02,  -.51206102998057436D-03,
     3  0.23913240112042451D-04,  0.13062016613547448D-04,
     4  -.54989801857822496D-05,  0.12731314752145051D-05,
     5  -.15905845230094415D-06,  -.18003104415761525D-07,
     6  0.18413140481316157D-07,  -.66727800339131959D-08,
     7  0.15859742635713193D-08,  -.20089574314238113D-09,
     8  -.35955353802062395D-10,  0.34706734353110799D-10,
     9  -.14102525844252249D-10,  0.40213739664396254D-11,
     *  -.77533534581175509D-12,  0.28897159776095259D-13,
     *  0.58461161593279768D-13,  -.34147380626076096D-13,
     *  0.12812689061319350D-13,  -.36573494308431337D-14,
     *  0.75728692193621683D-15,  -.65156104027573599D-16,
     *  -.33023867538634843D-16,  0.21473401919592022D-16,
     *  -.71739068222167911D-17,  0.14505906169688027D-17/
c
        data q601/
     1  0.16394385388238233D+01,  0.85274075035493443D+00,
     2  -.69675715187862858D-01,  0.49060373884732864D-02,
     3  0.14921006095262404D-03,  -.17086634381652968D-03,
     4  0.46663319880656145D-04,  -.85920157941314899D-05,
     5  0.93440858786052771D-06,  0.75121518409362105D-07,
     6  -.78060453422263262D-07,  0.26819797648240084D-07,
     7  -.63495345987198140D-08,  0.99245598885690344D-09,
     8  -.93487970254534238D-11,  -.65394373469726073D-10,
     9  0.30887492810863595D-10,  -.96488259446850141D-11,
     *  0.22437505053084303D-11,  -.32857110545785533D-12,
     *  -.20230450243356709D-13,  0.37862205968335151D-13,
     *  -.17755152238375509D-13,  0.59673161688771103D-14,
     *  -.15901576405536940D-14,  0.32428354435019387D-15,
     *  -.38206405232140832D-16,  -.44780202690080041D-17,
     *  0.38947687802009993D-17,  -.10312604688460376D-17/
        data q602/
     1  0.11322108865665877D+01,  0.45949148329511419D+00,
     2  -.87594808391067481D-01,  0.11277960703525475D-01,
     3  -.20966977919682307D-03,  -.40553980920698975D-03,
     4  0.14128785688408487D-03,  -.26123541028036529D-04,
     5  0.81599667872745057D-06,  0.14434148603533166D-05,
     6  -.64134719140754752D-06,  0.15958245230806483D-06,
     7  -.18409921461188691D-07,  -.48741125974542763D-08,
     8  0.37800103842870082D-08,  -.13425758837147141D-08,
     9  0.30508038248922706D-09,  -.27234111764509824D-10,
     *  -.14251626959173227D-10,  0.95126322639165426D-11,
     *  -.34260858726101536D-11,  0.83864747171120192D-12,
     *  -.10418490995331930D-12,  -.26163965888040962D-13,
     *  0.23649259087309032D-13,  -.99739104998804162D-14,
     *  0.30683047953769310D-14,  -.72430554870132098D-15,
     *  0.12292031176087917D-15,  -.11203254316126050D-16/
        data q603/
     1  -.10102028384932545D+01,  -.40491814108565335D+00,
     2  0.76871276962959297D-01,  -.10354273854279859D-01,
     3  0.44872553977796329D-03,  0.27824959826565661D-03,
     4  -.11412485025476345D-03,  0.25749226797807558D-04,
     5  -.29397928282188750D-05,  -.49727398603871599D-06,
     6  0.41052998717314249D-06,  -.14075301372393137D-06,
     7  0.31173908887394597D-07,  -.30053219175185990D-08,
     8  -.11866829103287591D-08,  0.83433741357857307D-09,
     9  -.30742667241408043D-09,  0.79159078192687660D-10,
     *  -.12080066308323960D-10,  -.11721056728065422D-11,
     *  0.17664276312386444D-11,  -.82425205539660413D-12,
     *  0.26946913716260854D-12,  -.64701943907030692D-13,
     *  0.87196844763093432D-14,  0.15087021303523812D-14,
     *  -.15981452267800915D-14,  0.67668248161587822D-15,
     *  -.19228413528560300D-15,  0.34842660127572003D-16/
        data q604/
     1  0.64575746870314660D+00,  0.25871249581978973D+00,
     2  -.48807190948536821D-01,  0.65913697208670340D-02,
     3  -.31351981891027104D-03,  -.16390751282434536D-03,
     4  0.69342158056817441D-04,  -.16132042331601665D-04,
     5  0.20670875899473671D-05,  0.19673265874581784D-06,
     6  -.22131424811673864D-06,  0.81299010453376230D-07,
     7  -.19576890698469264D-07,  0.25984530264988238D-08,
     8  0.37397560052463919D-09,  -.40084242897193104D-09,
     9  0.16580269771109402D-09,  -.47884998002094993D-10,
     *  0.94650731445548742D-11,  -.48818178734007169D-12,
     *  -.63370031061699710D-12,  0.38475747107466270D-12,
     *  -.14635830393258659D-12,  0.42266316821553773D-13,
     *  -.89420421489520685D-14,  0.87145189441962880D-15,
     *  0.33045770171469853D-15,  -.23065887053309199D-15,
     *  0.78269852792579341D-16,  -.15933724483024609D-16/
        data q605/
     1  -.23705435095185108D+00,  -.95013135414793447D-01,
     2  0.17856798672728941D-01,  -.24098523617404149D-02,
     3  0.11789820292609159D-03,  0.58105643037562523D-04,
     4  -.24825940200246399D-04,  0.58296684483602760D-05,
     5  -.77443757031002243D-06,  -.56058513286323960D-07,
     6  0.74815102878536501D-07,  -.28187977081285381D-07,
     7  0.69693626150709980D-08,  -.10040492350191163D-08,
     8  -.89824611847572478D-10,  0.12721245845761959D-09,
     9  -.55057580915868716D-10,  0.16491852557935005D-10,
     *  -.34841184590081677D-11,  0.29792482471647687D-12,
     *  0.16780846643835381D-12,  -.11760804791599767D-12,
     *  0.47340541493658468D-13,  -.14429973505855106D-13,
     *  0.33481877578225495D-14,  -.47155812485757415D-15,
     *  -.43315340608736273D-16,  0.57151610764515633D-16,
     *  -.21593226600720764D-16,  0.46375796656270760D-17/
        data q606/
     1  0.37446363517914522D-01,  0.15015518159134087D-01,
     2  -.28151832342781269D-02,  0.37955826635196810D-03,
     3  -.18796992562809827D-04,  -.90087252848030151D-05,
     4  0.38653920028928300D-05,  -.91130739462915418D-06,
     5  0.12307582360181566D-06,  0.76110817741124870D-08,
     6  -.11280392479444069D-07,  0.43030394094744370D-08,
     7  -.10766776702999357D-08,  0.16063069837534704D-09,
     8  0.10773851609102017D-10,  -.18501144820901758D-10,
     9  0.81910110143232934D-11,  -.24940957182990759D-11,
     *  0.54175662263928252D-12,  -.53805532988076317D-13,
     *  -.21869460969014812D-13,  0.16692133457984317D-13,
     *  -.69055704864678259D-14,  0.21545248150023958D-14,
     *  -.51818016717726750D-15,  0.81280404166000764D-16,
     *  0.20582946140199127D-17,  -.71356417689845123D-17,
     *  0.28838639557006966D-17,  -.63739780973159979D-18/
c
        data q701/
     1  0.16097749392487823D+01,  0.84081153465963748D+00,
     2  -.67462266172917597D-01,  0.46096204922930330D-02,
     3  0.16422181770052357D-03,  -.16413411950617870D-03,
     4  0.43755222749401972D-04,  -.79014174873407568D-05,
     5  0.83738938134840044D-06,  0.71703654436089656D-07,
     6  -.70428913130266497D-07,  0.23812277570192535D-07,
     7  -.55758645477125994D-08,  0.86765757183290543D-09,
     8  -.11589453697321111D-10,  -.54258964111443717D-10,
     9  0.25643996790681090D-10,  -.79906932391004167D-11,
     *  0.18617943880944415D-11,  -.27965205681581438D-12,
     *  -.11189072177241826D-13,  0.28599541962739375D-13,
     *  -.13649132509702277D-13,  0.46204137676841077D-14,
     *  -.12430904912046317D-14,  0.25960263218714163D-15,
     *  -.33779731310206403D-16,  -.18689035369751976D-17,
     *  0.25553934073039240D-17,  -.71060267028370420D-18/
        data q702/
     1  0.13101924840168340D+01,  0.53106677746689592D+00,
     2  -.10087550248073904D+00,  0.13056462080606995D-01,
     3  -.29974031968422026D-03,  -.44593315506909562D-03,
     4  0.15873643967160991D-03,  -.30267130868780928D-04,
     5  0.13981119178002142D-05,  0.14639220441929513D-05,
     6  -.68713643315952811D-06,  0.17762757277635012D-06,
     7  -.23051941767231978D-07,  -.41253220953102882D-08,
     8  0.37934543243182143D-08,  -.14093883398644082D-08,
     9  0.33654135861032209D-09,  -.37182907998017406D-10,
     *  -.11959890255889296D-10,  0.92191179720642978D-11,
     *  -.34803341410068436D-11,  0.89422345574477709D-12,
     *  -.12882102832535907D-12,  -.18082551480882686D-13,
     *  0.21566856191214487D-13,  -.95858250269019991D-14,
     *  0.30417447518452656D-14,  -.73996024909348886D-15,
     *  0.13095656399825129D-15,  -.13127201107497564D-16/
        data q703/
     1  -.14551568321188701D+01,  -.58385637651510766D+00,
     2  0.11007301218713821D+00,  -.14800527296983659D-01,
     3  0.67390189099645626D-03,  0.37923296292092128D-03,
     4  -.15774630722357605D-03,  0.36108201399668555D-04,
     5  -.43950809259007840D-05,  -.54854194563780273D-06,
     6  0.52500309155309397D-06,  -.18586581489464460D-06,
     7  0.42778959652502815D-07,  -.48772981728785692D-08,
     8  -.12202927604067744D-08,  0.10013685539528084D-08,
     9  -.38607911271681801D-09,  0.10403106877645662D-09,
     *  -.17809408066533789D-10,  -.43831994317592996D-12,
     *  0.19020483022303692D-11,  -.96319201548054191D-12,
     *  0.33105943309270785D-12,  -.84905479924926307D-13,
     *  0.13925691716545657D-13,  0.53848844790636843D-15,
     *  -.15317451179509450D-14,  0.71581923259630618D-15,
     *  -.21237476587903591D-15,  0.39652527106001170D-16/
        data q704/
     1  0.12390294602039674D+01,  0.49729680972572882D+00,
     2  -.93076171247442034D-01,  0.12519707644472101D-01,
     3  -.61375495386826167D-03,  -.29855199903136492D-03,
     4  0.12750410068190091D-03,  -.29944008467416326D-04,
     5  0.40074717201899124D-05,  0.26508993821126683D-06,
     6  -.37394505395667395D-06,  0.14144941201432720D-06,
     7  -.35050291718613555D-07,  0.50944213669787841D-08,
     8  0.41878873396199293D-09,  -.62355061613757816D-09,
     9  0.27067261811474412D-09,  -.81047652113786936D-10,
     *  0.17104195488834646D-10,  -.14665627601808880D-11,
     *  -.81452787193929679D-12,  0.57001075118657974D-12,
     *  -.22847869850605233D-12,  0.69204364845414595D-13,
     *  -.15883385135933821D-13,  0.21650701376809791D-14,
     *  0.24192422327583638D-15,  -.28284120517366262D-15,
     *  0.10505736025049037D-15,  -.22346880454263910D-16/
        data q705/
     1  -.68200834457746669D+00,  -.27395137084424776D+00,
     2  0.51058533896907850D-01,  -.68561058044442151D-02,
     3  0.34307455414458456D-03,  0.15908900769282719D-03,
     4  -.68447397169059004D-04,  0.16188643050221272D-04,
     5  -.22297256679919314D-05,  -.10732647288541070D-06,
     6  0.18928820725848798D-06,  -.73300778251998611D-07,
     7  0.18574413380179216D-07,  -.28760254903790865D-08,
     8  -.12343446192558778D-09,  0.29424359883185493D-09,
     9  -.13371002121860629D-09,  0.41363843141703962D-10,
     *  -.92134602172179967D-11,  0.10317105543470891D-11,
     *  0.30342913743007861D-12,  -.25654800799993547D-12,
     *  0.10893083742375779D-12,  -.34633509523750732D-13,
     *  0.85541949980588699D-14,  -.14417718073035913D-14,
     *  0.23084768220412649D-16,  0.96288361744942491D-16,
     *  -.41683857194153308D-16,  0.94474466440561763D-17/
        data q706/
     1  0.21542796096816077D+00,  0.86590812330915814D-01,
     2  -.16095877323949691D-01,  0.21580596434334882D-02,
     3  -.10886753305020702D-03,  -.49402071146908883D-04,
     4  0.21313974790417872D-04,  -.50548972353735527D-05,
     5  0.70519106267457926D-06,  0.28118265613747184D-07,
     6  -.57069634231424660D-07,  0.22348159877759729D-07,
     7  -.57186979763432230D-08,  0.90942120051933512D-09,
     8  0.24217791640308139D-10,  -.85313600970595893D-10,
     9  0.39651987135418322D-10,  -.12442891951806659D-10,
     *  0.28334933259232143D-11,  -.34731982484032132D-12,
     *  -.76117729365704644D-13,  0.72268117491559372D-13,
     *  -.31541688858507508D-13,  0.10235939222160613D-13,
     *  -.26005830632717740D-14,  0.46936587714439409D-15,
     *  -.24501748917632344D-16,  -.22790342161158773D-16,
     *  0.10920116193075273D-16,  -.25613446011037487D-17/
        data q707/
     1  -.29663599575041041D-01,  -.11929215695296954D-01,
     2  0.22134490149452606D-02,  -.29641689618025335D-03,
     3  0.15011756747899531D-04,  0.67322243103509779D-05,
     4  -.29080971312541736D-05,  0.69059830679073309D-06,
     5  -.97019206512127266D-07,  -.34178639732724495D-08,
     6  0.76315402919967653D-08,  -.30075200780475487D-08,
     7  0.77367005100721455D-09,  -.12479841702399802D-09,
     8  -.22406566718676867D-11,  0.11135409358282356D-10,
     9  -.52434960201825045D-11,  0.16581327055845970D-11,
     *  -.38195611721398845D-12,  0.48919048642040697D-13,
     *  0.90413780661150782D-14,  -.92626640055959230D-14,
     *  0.41060197286733384D-14,  -.13469024011930765D-14,
     *  0.34706714934911074D-15,  -.64680912163081699D-16,
     *  0.44266739219507080D-17,  0.26091167320249487D-17,
     *  -.13393753728940950D-17,  0.32065779856161179D-18/
c
        data q801/
     1  0.15854065884546936D+01,  0.83098138222514957D+00,
     2  -.65655187126295468D-01,  0.43694250510040900D-02,
     3  0.17645145715341633D-03,  -.15882694832714225D-03,
     4  0.41462372883387687D-04,  -.73558971197191006D-05,
     5  0.75907912206966268D-06,  0.70176081664073219D-07,
     6  -.64894692658044572D-07,  0.21584223590800945D-07,
     7  -.49935404789597311D-08,  0.76956012246634816D-09,
     8  -.10715403606410193D-10,  -.46936285673648799D-10,
     9  0.22034025072334644D-10,  -.68220521100073674D-11,
     *  0.15835648801986779D-11,  -.23959210233047404D-12,
     *  -.72442400187821334D-14,  0.22928086664916816D-13,
     *  -.11002955752091190D-13,  0.37249729203527864D-14,
     *  -.10034827850599434D-14,  0.21125427358779821D-15,
     *  -.28603730730561660D-16,  -.87207365991233810D-18,
     *  0.18539393538831270D-17,  -.53009059953476751D-18/
        data q802/
     1  0.14807709395754545D+01,  0.59987784450831130D+00,
     2  -.11352505580709395D+00,  0.14737830169629596D-01,
     3  -.38534779585446960D-03,  -.48308335332235078D-03,
     4  0.17478638873370990D-03,  -.34085773442132521D-04,
     5  0.19462837327513785D-05,  0.14746150535970663D-05,
     6  -.72587597646508159D-06,  0.19322395063209125D-06,
     7  -.27128210248502056D-07,  -.34386399497443873D-08,
     8  0.37873359736818379D-08,  -.14606470889289727D-08,
     9  0.36181116063874723D-09,  -.45363395901668769D-10,
     *  -.10012283700618937D-10,  0.89386982906669046D-11,
     *  -.35079479661160531D-11,  0.93392364282952871D-12,
     *  -.14734426562863211D-12,  -.11814465549566588D-13,
     *  0.19889602248203724D-13,  -.92473865167078419D-14,
     *  0.30055127477884357D-14,  -.74693805823325107D-15,
     *  0.13586674237230015D-15,  -.14390785602773299D-16/
        data q803/
     1  -.19668921987947317D+01,  -.79028957763935381D+00,
     2  0.14802167216620292D+00,  -.19844631564051461D-01,
     3  0.93072431950720428D-03,  0.49068355768068678D-03,
     4  -.20589615440987603D-03,  0.47564129119723333D-04,
     5  -.60395963707542770D-05,  -.58062097385014790D-06,
     6  0.64122172146975441D-06,  -.23265494846186799D-06,
     7  0.55007765096313050D-07,  -.69373446095762719D-08,
     8  -.12019377084976452D-08,  0.11551448011465017D-08,
     9  -.46188851880209341D-09,  0.12857253248741069D-09,
     *  -.23652227732344857D-10,  0.40293910101624301D-12,
     *  0.19848897775580027D-11,  -.10822925767348005D-11,
     *  0.38662914500252973D-12,  -.10370973771887651D-12,
     *  0.18957453545579206D-13,  -.47682708267687164D-15,
     *  -.14230491057800252D-14,  0.73675266001538300D-15,
     *  -.22710530100110565D-15,  0.43443280591803258D-16/
        data q804/
     1  0.20919217379970702D+01,  0.84135214493280573D+00,
     2  -.15632393787921655D+00,  0.20926548089585105D-01,
     3  -.10417923347195084D-02,  -.48430299029764076D-03,
     4  0.20775384599240087D-03,  -.49037221334174291D-04,
     5  0.67483307949457341D-05,  0.31855498523184212D-06,
     6  -.56764277048444134D-06,  0.21943130129303285D-06,
     7  -.55431634124963945D-07,  0.85278320948082886D-08,
     8  0.38819698078011093D-09,  -.87984436146040041D-09,
     9  0.39702162825686980D-09,  -.12195009163204374D-09,
     *  0.26842228265186432D-10,  -.28686611671678472D-11,
     *  -.95259699748534937D-12,  0.76851168661034167D-12,
     *  -.32109488502242035D-12,  0.10054479450199702D-12,
     *  -.24269654850988909D-13,  0.38572626886525389D-14,
     *  0.60764202991257349D-16,  -.31773025087226664D-15,
     *  0.12960825212066281D-15,  -.28664802930619054D-16/
        data q805/
     1  -.15349006223705694D+01,  -.61800670605132468D+00,
     2  0.11430630052868237D+00,  -.15262946249557219D-01,
     3  0.77111193499583125D-03,  0.34483999895910303D-03,
     4  -.14869714247955896D-03,  0.35281855916979237D-04,
     5  -.49705847427477531D-05,  -.16079151990598599D-06,
     6  0.38298592378625537D-06,  -.15128266753070426D-06,
     7  0.38955755786529607D-07,  -.63094362182085910D-08,
     8  -.92842708743705775D-10,  0.55053734415467717D-09,
     9  -.26005903136073196D-09,  0.82266282659960757D-10,
     *  -.18951492993569779D-10,  0.24338089613340450D-11,
     *  0.44149826297613366D-12,  -.45504894342369926D-12,
     *  0.20154702394012718D-12,  -.65973939180334109D-13,
     *  0.16940464713114596D-13,  -.31339643582755430D-14,
     *  0.20424478850521510D-15,  0.13117740744343439D-15,
     *  -.66234749064279658D-16,  0.15765369120396568D-16/
        data q806/
     1  0.72716332764402240D+00,  0.29302401345516196D+00,
     2  -.54044537303014403D-01,  0.72021639105012906D-02,
     3  -.36568996156095503D-03,  -.16085266590667439D-03,
     4  0.69463821976717848D-04,  -.16510824955428332D-04,
     5  0.23497065075280723D-05,  0.60197293826092356D-07,
     6  -.17328826414808509D-06,  0.69137293444983119D-07,
     7  -.17947503420153457D-07,  0.29694676372170378D-08,
     8  0.58627397311789392D-11,  -.23908984816428924D-09,
     9  0.11546139322069373D-09,  -.36984355662760739D-10,
     *  0.86763129917342860D-11,  -.11885788690324969D-11,
     *  -.15895920469333609D-12,  0.19136867874581645D-12,
     *  -.87111400768328258D-13,  0.29040197016110025D-13,
     *  -.76323448923047958D-14,  0.14846814077273091D-14,
     *  -.13319776108836678D-15,  -.43723769580328424D-16,
     *  0.25650651315183691D-16,  -.63520980869179832D-17/
        data q807/
     1  -.20024205513366159D+00,  -.80740282736712337D-01,
     2  0.14863002341300165D-01,  -.19777849852028542D-02,
     3  0.10061923291814887D-03,  0.43882422563606146D-04,
     4  -.18958046193354166D-04,  0.45092408801423260D-05,
     5  -.64519102146329160D-06,  -.14110873377387507D-07,
     6  0.46371083597550243D-07,  -.18603897933788679D-07,
     7  0.48499385322772927D-08,  -.81148056258989891D-09,
     8  0.38776939645087167D-11,  0.62394158422846802D-10,
     9  -.30513298048607638D-10,  0.98386206092359549D-11,
     *  -.23295626724843440D-11,  0.32933873003943115D-12,
     *  0.36655203175326663D-13,  -.48962851090349118D-13,
     *  0.22629257031947537D-13,  -.76149883325099766D-14,
     *  0.20243210923604082D-14,  -.40311942235756727D-15,
     *  0.40658677978966003D-16,  0.95869258716952130D-17,
     *  -.62495537469057442D-17,  0.15842422938261081D-17/
        data q808/
     1  0.24368350794088649D-01,  0.98301524344879118D-02,
     2  -.18070790466221292D-02,  0.24019544128894297D-03,
     3  -.12229639452892763D-04,  -.53071711790364525D-05,
     4  0.22928498660142846D-05,  -.54552036762165614D-06,
     5  0.78310259278737762D-07,  0.15275727720164368D-08,
     6  -.55342204722219254D-08,  0.22280539793915900D-08,
     7  -.58232406875286830D-09,  0.98097449366557268D-10,
     8  -.87405009091091212D-12,  -.73226784377949233D-11,
     9  0.36099717183464497D-11,  -.11686411290930528D-11,
     *  0.27822950789576641D-12,  -.40059954485342567D-13,
     *  -.39448321584579600D-14,  0.56714552978212497D-14,
     *  -.26461767576101332D-14,  0.89544084733065842D-15,
     *  -.23960770614425072D-15,  0.48348358599074259D-16,
     *  -.51760005794951659D-17,  -.99682987713575358D-18,
     *  0.70145405344988742D-18,  -.18051207075676033D-18/
c
c        check that m is in the permitted range
c
        ier=8
        if(m .eq. 1) ier=0
        if(m .eq. 2) ier=0
        if(m .eq. 3) ier=0
        if(m .eq. 4) ier=0
        if(m .eq. 5) ier=0
        if(m .eq. 6) ier=0
        if(m .eq. 7) ier=0
        if(m .eq. 8) ier=0
c
        if(ier .ne. 0) return
c
c       check if t is in the permitted range
c
ccccc          if(2 .ne. 3) goto 1400
        if( (t .ge. 0) .and. (t .le. 50000) ) goto 1400
c
        ier=16
        return
 1400 continue
c
c       construct the correction weights in the case when 
c       t \in [20,1000]
c 
        if(t .lt. 20) goto 1600
c
        if(t .lt. 1000) goto 1500
        call corwhtsg4(t,m,whts)
        return
c
 1500 continue
c
        call corwhtsg3(t,m,whts)
        return
 1600 continue
c
c        construct the correction weights in the case when 
c        t \in [0,1]
c
        if(t .gt. 1) goto 3000
c
c       linearly transform the user-supplied t to put
c       it on the interval [-1,1]
c
        n=23
        u=2
        v=-1
        tt=dsqrt(t)
        x=u*tt+v
c
c        one after another, calculate the values of
c        the correction weights at the user-specified  t
c
c        . . . if m=1
c
        if(m .ne. 1) goto 2100
        call chexev(p101,x,whts(1),n-1)
        return
 2100 continue
c
        if(m .ne. 2) goto 2200
        call chexev(p201,x,whts(1),n-1)
        call chexev(p202,x,whts(2),n-1)
        return
 2200 continue
c
        if(m .ne. 3) goto 2300
        call chexev(p301,x,whts(1),n-1)
        call chexev(p302,x,whts(2),n-1)
        call chexev(p303,x,whts(3),n-1)
        return
 2300 continue
c
        if(m .ne. 4) goto 2400
        call chexev(p401,x,whts(1),n-1)
        call chexev(p402,x,whts(2),n-1)
        call chexev(p403,x,whts(3),n-1)
        call chexev(p404,x,whts(4),n-1)
        return
 2400 continue
c
        if(m .ne. 5) goto 2500
        call chexev(p501,x,whts(1),n-1)
        call chexev(p502,x,whts(2),n-1)
        call chexev(p503,x,whts(3),n-1)
        call chexev(p504,x,whts(4),n-1)
        call chexev(p505,x,whts(5),n-1)
        return
 2500 continue
c
        if(m .ne. 6) goto 2600
        call chexev(p601,x,whts(1),n-1)
        call chexev(p602,x,whts(2),n-1)
        call chexev(p603,x,whts(3),n-1)
        call chexev(p604,x,whts(4),n-1)
        call chexev(p605,x,whts(5),n-1)
        call chexev(p606,x,whts(6),n-1)
        return
 2600 continue
c
        if(m .ne. 7) goto 2700
        call chexev(p701,x,whts(1),n-1)
        call chexev(p702,x,whts(2),n-1)
        call chexev(p703,x,whts(3),n-1)
        call chexev(p704,x,whts(4),n-1)
        call chexev(p705,x,whts(5),n-1)
        call chexev(p706,x,whts(6),n-1)
        call chexev(p707,x,whts(7),n-1)
        return
 2700 continue
c
        if(m .ne. 8) goto 2800
        call chexev(p801,x,whts(1),n-1)
        call chexev(p802,x,whts(2),n-1)
        call chexev(p803,x,whts(3),n-1)
        call chexev(p804,x,whts(4),n-1)
        call chexev(p805,x,whts(5),n-1)
        call chexev(p806,x,whts(6),n-1)
        call chexev(p807,x,whts(7),n-1)
        call chexev(p808,x,whts(8),n-1)
        return
 2800 continue
        return
 3000 continue
cc
c        construct the correction weights in the case when 
c        t \in [1,20]
c
        if(t .gt. 20) goto 4000
c
c       linearly transform the user-supplied t to put
c       it on the interval [-1,1]
c
        n=30
c
        rt20=20
        rt20=dsqrt(rt20)
        u=2/(rt20-1)
        v=-1-u
        tt=dsqrt(t)
        x=u*tt+v
c           call prin2('x=*',x,1)
c
c        one after another, calculate the values of
c        the correction weights at the user-specified  t
c
c        . . . if m=1
c
        if(m .ne. 1) goto 3100
        call chexev(q101,x,whts(1),n-1)
        return
 3100 continue
c
        if(m .ne. 2) goto 3200
        call chexev(q201,x,whts(1),n-1)
        call chexev(q202,x,whts(2),n-1)
        return
 3200 continue
c
        if(m .ne. 3) goto 3300
        call chexev(q301,x,whts(1),n-1)
        call chexev(q302,x,whts(2),n-1)
        call chexev(q303,x,whts(3),n-1)
        return
 3300 continue
c
        if(m .ne. 4) goto 3400
        call chexev(q401,x,whts(1),n-1)
        call chexev(q402,x,whts(2),n-1)
        call chexev(q403,x,whts(3),n-1)
        call chexev(q404,x,whts(4),n-1)
        return
 3400 continue
c
        if(m .ne. 5) goto 3500
        call chexev(q501,x,whts(1),n-1)
        call chexev(q502,x,whts(2),n-1)
        call chexev(q503,x,whts(3),n-1)
        call chexev(q504,x,whts(4),n-1)
        call chexev(q505,x,whts(5),n-1)
        return
 3500 continue
c
        if(m .ne. 6) goto 3600
        call chexev(q601,x,whts(1),n-1)
        call chexev(q602,x,whts(2),n-1)
        call chexev(q603,x,whts(3),n-1)
        call chexev(q604,x,whts(4),n-1)
        call chexev(q605,x,whts(5),n-1)
        call chexev(q606,x,whts(6),n-1)
        return
 3600 continue
c
        if(m .ne. 7) goto 3700
        call chexev(q701,x,whts(1),n-1)
        call chexev(q702,x,whts(2),n-1)
        call chexev(q703,x,whts(3),n-1)
        call chexev(q704,x,whts(4),n-1)
        call chexev(q705,x,whts(5),n-1)
        call chexev(q706,x,whts(6),n-1)
        call chexev(q707,x,whts(7),n-1)
        return
 3700 continue
c
        if(m .ne. 8) goto 3800
        call chexev(q801,x,whts(1),n-1)
        call chexev(q802,x,whts(2),n-1)
        call chexev(q803,x,whts(3),n-1)
        call chexev(q804,x,whts(4),n-1)
        call chexev(q805,x,whts(5),n-1)
        call chexev(q806,x,whts(6),n-1)
        call chexev(q807,x,whts(7),n-1)
        call chexev(q808,x,whts(8),n-1)
        return
 3800 continue
        return
 4000 continue

        return

        end
c
c
c
c
c
      SUBROUTINE CHexev(texp,X,VAL,N)
C
C     Subroutine computes the value and the derivative
c     of a chebychev expansion with coefficients TEXP
C     at point X in interval [-1,1]
C
c                input parameters:
c
C     TEXP = expansion coefficients
C     X = evaluation point
C     N  = order of expansion (0 ,..., N-1)
c
c                output parameters:
c
C     VAL = computed value
C
      IMPLICIT REAL *8 (A-H,O-Z)
      REAL *8 TEXP(*)
C
        done=1
        tjm2=1
        tjm1=x
c
        val=texp(1)*tjm2+texp(2)*tjm1      
c
      DO 600 J = 2,N
c
        tj=2*x*tjm1-tjm2
        val=val+texp(j+1)*tj
c
        tjm2=tjm1
        tjm1=tj
 600   CONTINUE
c
      RETURN
      END
c
c
c
c
c
        subroutine alptrap0(ier,m,weights)
        implicit real *8 (a-h,o-z)
        dimension weights(1)
        integer *4 c4(4),c6(6),c8(8),c10(10),c12(12),cc12(12)
        data c4/24,-3,4,-1/,c6/1440,-245,462,-336,146,-27/,
     1      c8/120960,-23681,55688,-66109,57024,
     2      -31523,9976,-1375/,
     3      c10/7257600,-1546047,4274870,-6996434,9005886,
     4      -8277760,5232322,-2161710,526154,-57281/
c
            data c12/9580032,-2162543,6795432,-14129473,
     6       24158814,-31035790,29399424,-20232241,
     7       9845153,-3214558,632535,-56752/
c
            data cc12/00,-35,84,-89,
     6       96,-86,00,-14,
     7       04,-11,16,-65/
c
c       this subroutine returns to the user the end-point 
c       corrections for the trapezoidal rule, resulting 
c       in a rule of order m
c   
        ier=4
c
c       . . . if m=2
c
        if(m .ne. 2) goto 1400
c
        ier=0
        weights(1)=0
        return
 1400 continue
c
c       . . . if m=4
c
        if(m .ne. 4) goto 2400
c
        ier=0
        d=c4(1)
        do 2200 i=1,m-1
        weights(i)=c4(i+1)/d
 2200 continue
        return
 2400 continue
c
c
c       . . . if m=6
c
        if(m .ne. 6) goto 3400
c
        ier=0
        d=c6(1)
        do 3200 i=1,m-1
        weights(i)=c6(i+1)/d
 3200 continue
        return
 3400 continue
c
c
c       . . . if m=8
c
        if(m .ne. 8) goto 4400
c
        ier=0
        d=c8(1)
        do 4200 i=1,m-1
        weights(i)=c8(i+1)/d
 4200 continue
        return
 4400 continue
c
c
c       . . . if m=10
c
        if(m .ne. 10) goto 5400
c
        ier=0
        d=c10(1)
        do 5200 i=1,m-1
        weights(i)=c10(i+1)/d
 5200 continue
        return
 5400 continue
c
c
c       . . . if m=12
c
        if(m .ne. 12) goto 6400
c
        ier=0
        d=c12(1)*100+cc12(1)
        do 6200 i=1,m-1
        weights(i)=c12(i+1)/d*100+cc12(i+1)/d
 6200 continue
        return
 6400 continue
c
        return
        end
c
c
c
c
c
        subroutine corwhtsg3(t,m,whts)
        implicit real *8 (a-h,o-z)
        dimension whts(*)
c
        dimension pp101(20),pp201(20),pp202(20),
     1    pp301(20),pp302(20),pp303(20),
     2    pp401(20),pp402(20),pp403(20),pp404(20),
     3    pp501(20),pp502(20),pp503(20),pp504(20),pp505(20),
     4    pp601(20),pp602(20),pp603(20),pp604(20),pp605(20),
     5    pp606(20),
     6    pp701(20),pp702(20),pp703(20),pp704(20),pp705(20),
     7    pp706(20),pp707(20),
     8    pp801(20),pp802(20),pp803(20),pp804(20),pp805(20),
     9    pp806(20),pp807(20),pp808(20)
c
        dimension buf101(2),buf201(2),buf202(2),
     1      buf301(2),buf302(2),buf303(2),
     2      buf401(2),buf402(2),buf403(2),buf404(2),
     3      buf501(2),buf502(2),buf503(2),buf504(2),buf505(2),
     4      buf601(2),buf602(2),buf603(2),buf604(2),buf605(2),
     5      buf606(2),
     6      buf701(2),buf702(2),buf703(2),buf704(2),buf705(2),
     7      buf706(2),buf707(2),
     8      buf801(2),buf802(2),buf803(2),buf804(2),buf805(2),
     9      buf806(2),buf807(2),buf808(2)
c
        equivalence (pp101(20),buf101(1)),(pq101(1),buf101(2)),
c
     1      (pp201(20),buf201(1)),(pq201(1),buf201(2)),
     2      (pp202(20),buf202(1)),(pq202(1),buf202(2)),
c 
     3      (pp301(20),buf301(1)),(pq301(1),buf301(2)),
     4      (pp302(20),buf302(1)),(pq302(1),buf302(2)),
     5      (pp303(20),buf303(1)),(pq303(1),buf303(2)),
c 
     6      (pp401(20),buf401(1)),(pq401(1),buf401(2)),
     7      (pp402(20),buf402(1)),(pq402(1),buf402(2)),
     8      (pp403(20),buf403(1)),(pq403(1),buf403(2)),
     9      (pp404(20),buf404(1)),(pq404(1),buf404(2)),
c 
     a      (pp501(20),buf501(1)),(pq501(1),buf501(2)),
     b      (pp502(20),buf502(1)),(pq502(1),buf502(2)),
     c      (pp503(20),buf503(1)),(pq503(1),buf503(2)),
     d      (pp504(20),buf504(1)),(pq504(1),buf504(2)),
     e      (pp505(20),buf505(1)),(pq505(1),buf505(2))
c 
        equivalence
     1      (pp601(20),buf601(1)),(pq601(1),buf601(2)),
     2      (pp602(20),buf602(1)),(pq602(1),buf602(2)),
     3      (pp603(20),buf603(1)),(pq603(1),buf603(2)),
     4      (pp604(20),buf604(1)),(pq604(1),buf604(2)),
     5      (pp605(20),buf605(1)),(pq605(1),buf605(2)),
     6      (pp606(20),buf606(1)),(pq606(1),buf606(2))
        equivalence
     1      (pp701(20),buf701(1)),(pq701(1),buf701(2)),
     2      (pp702(20),buf702(1)),(pq702(1),buf702(2)),
     3      (pp703(20),buf703(1)),(pq703(1),buf703(2)),
     4      (pp704(20),buf704(1)),(pq704(1),buf704(2)),
     5      (pp705(20),buf705(1)),(pq705(1),buf705(2)),
     6      (pp706(20),buf706(1)),(pq706(1),buf706(2)),
     7      (pp707(20),buf707(1)),(pq707(1),buf707(2))
c
        equivalence
     1      (pp801(20),buf801(1)),(pq801(1),buf801(2)),
     2      (pp802(20),buf802(1)),(pq802(1),buf802(2)),
     3      (pp803(20),buf803(1)),(pq803(1),buf803(2)),
     4      (pp804(20),buf804(1)),(pq804(1),buf804(2)),
     5      (pp805(20),buf805(1)),(pq805(1),buf805(2)),
     6      (pp806(20),buf806(1)),(pq806(1),buf806(2)),
     7      (pp807(20),buf807(1)),(pq807(1),buf807(2)),
     8      (pp808(20),buf808(1)),(pq808(1),buf808(2))
c
        dimension pq101(28),pq201(28),pq202(28),
     1    pq301(28),pq302(28),pq303(28),
     2    pq401(28),pq402(28),pq403(28),pq404(28),
     3    pq501(28),pq502(28),pq503(28),pq504(28),pq505(28),
     4    pq601(28),pq602(28),pq603(28),pq604(28),pq605(28),
     5    pq606(28),
     6    pq701(28),pq702(28),pq703(28),pq704(28),pq705(28),
     7    pq706(28),pq707(28),
     8    pq801(28),pq802(28),pq803(28),pq804(28),pq805(28),
     9    pq806(28),pq807(28),pq808(28)
c
        data pp101/
     1  0.46404515022356831D+01,  0.17753190273882518D+01,
     2  0.41795051202651360D+00,  0.12447157537547518D+00,
     3  0.42256071775771703D-01,  0.15331802421201186D-01,
     4  0.57935063768246097D-02,  0.22516382426039052D-02,
     5  0.89332842936920885D-03,  0.36005079171270597D-03,
     6  0.14693032523912585D-03,  0.60565303899450135D-04,
     7  0.25173319299853677D-04,  0.10536188287167031D-04,
     8  0.44361271406153554D-05,  0.18773528496165038D-05,
     9  0.79803571406736214D-06,  0.34056383975921299D-06,
     *  0.14584115495214333D-06,  0.62647557589094936D-07/
        data pq101/
     1  0.26985650678467706D-07,  0.11653294985557213D-07,
     2  0.50437145421855949D-08,  0.21875122245000804D-08,
     3  0.95054400670308790D-09,  0.41376009781301431D-09,
     4  0.18039330340115305D-09,  0.78765337856575757D-10,
     5  0.34438642036822818D-10,  0.15076870208617445D-10,
     6  0.66083507383459976D-11,  0.28997315138867687D-11,
     7  0.12737219974591889D-11,  0.56003584433535839D-12,
     8  0.24646543421160035D-12,  0.10856053844094986D-12,
     9  0.47856720982773434D-13,  0.21112927609064042D-13,
     *  0.93212115400624706D-14,  0.41180762521258861D-14,
     *  0.18205542873827810D-14,  0.80533099617508013D-15,
     *  0.35646374053717243D-15,  0.15781909706713635D-15,
     *  0.69880767953369207D-16,  0.30768587730047711D-16,
     *  0.13234813256289963D-16,  0.48853204373727372D-17/
c
        data pp201/
     1  0.41963059676547129D+01,  0.17215978767902550D+01,
     2  0.42065303506340822D+00,  0.12547876618024107D+00,
     3  0.42408383695165401D-01,  0.15355792822534690D-01,
     4  0.57985402158102823D-02,  0.22529139822422532D-02,
     5  0.89368383650129760D-03,  0.36015651550557107D-03,
     6  0.14696343502520850D-03,  0.60576101302681183D-04,
     7  0.25176956574006684D-04,  0.10537446630654143D-04,
     8  0.44365722747659438D-05,  0.18775133161437948D-05,
     9  0.79809450669253641D-06,  0.34058568604409035D-06,
     *  0.14584937344527465D-06,  0.62650683252466794D-07/
        data pq201/
     1  0.26986851023640459D-07,  0.11653759976099154D-07,
     2  0.50438960860489654D-08,  0.21875836078711985D-08,
     3  0.95057225631898704D-09,  0.41377134350334944D-09,
     4  0.18039780436545390D-09,  0.78767148299847466D-10,
     5  0.34439373607499606D-10,  0.15077167087649260D-10,
     6  0.66084716893822485D-11,  0.28997809744282368D-11,
     7  0.12737422918910435D-11,  0.56004419964889069D-12,
     8  0.24646888222332824D-12,  0.10856196779692146D-12,
     9  0.47857311177980192D-13,  0.21113175595266227D-13,
     *  0.93213131824614110D-14,  0.41181207317102053D-14,
     *  0.18205725207958076D-14,  0.80534004428245736D-15,
     *  0.35646552494804256D-15,  0.15782176435245411D-15,
     *  0.69878219779482351D-16,  0.30770204026250334D-16,
     *  0.13233401748336721D-16,  0.48870059557462790D-17/
        data pp202/
     1  0.44414553458097027D+00,  0.53721150597996830D-01,
     2  -.27025230368946160D-02,  -.10071908047658897D-02,
     3  -.15231191939369757D-03,  -.23990401333503281D-04,
     4  -.50338389856725839D-05,  -.12757396383480070D-05,
     5  -.35540713208874558D-06,  -.10572379286509790D-06,
     6  -.33109786082648110D-07,  -.10797403231047899D-07,
     7  -.36372741530067106D-08,  -.12583434871116187D-08,
     8  -.44513415058841420D-09,  -.16046652729106947D-09,
     9  -.58792625174271728D-10,  -.21846284877358094D-10,
     *  -.82184931313174967D-11,  -.31256633718577977D-11/
        data pq202/
     1  -.12003451727520818D-11,  -.46499054194084861D-12,
     2  -.18154386337057184D-12,  -.71383371118074123D-13,
     3  -.28249615899141898D-13,  -.11245690335130891D-13,
     4  -.45009643008555470D-14,  -.18104432717087867D-14,
     5  -.73157067678852857D-15,  -.29687903181485948D-15,
     6  -.12095103625089538D-15,  -.49460541468109118D-16,
     7  -.20294431854588446D-16,  -.83553135323005526D-17,
     8  -.34480117278858688D-17,  -.14293559716021197D-17,
     9  -.59019520675828444D-18,  -.24798620218437733D-18,
     *  -.10164239894033834D-18,  -.44479584319211043D-19,
     *  -.18233413026589049D-19,  -.90481073772252042D-20,
     *  -.17844108701333909D-20,  -.26672853177612422D-20,
     *  0.25481738868557496D-20,  -.16162962026227084D-20,
     *  0.14115079532422039D-20,  -.16855183735416581D-20/
c
        data pp301/
     1  0.40119625625046961D+01,  0.16990106823119418D+01,
     2  0.42151628553784594D+00,  0.12589121127099549D+00,
     3  0.42481279105726901D-01,  0.15368401850856010D-01,
     4  0.58011693291720693D-02,  0.22535642292247582D-02,
     5  0.89386317070868816D-03,  0.36020968619024330D-03,
     6  0.14698005849739696D-03,  0.60581516419964359D-04,
     7  0.25178779416901558D-04,  0.10538076945013376D-04,
     8  0.44367951673265066D-05,  0.18775936458222714D-05,
     9  0.79812393252461043D-06,  0.34059661848413866D-06,
     *  0.14585348569296049D-06,  0.62652247073860007D-07/
        data pq301/
     1  0.26987451528841238D-07,  0.11653992585046017D-07,
     2  0.50439868974191221D-08,  0.21876193134253791D-08,
     3  0.95058638606387981D-09,  0.41377696812547890D-09,
     4  0.18040005549386948D-09,  0.78768053758540818D-10,
     5  0.34439739480829548D-10,  0.15077315559821557D-10,
     6  0.66085321773083357D-11,  0.28998057092471434D-11,
     7  0.12737524408654092D-11,  0.56004837787368726D-12,
     8  0.24647060659623543D-12,  0.10856268247626926D-12,
     9  0.47857606547370261D-13,  0.21113299500818760D-13,
     *  0.93213641215138489D-14,  0.41181428527702180D-14,
     *  0.18205816221149311D-14,  0.80534444383277884D-15,
     *  0.35646653650635954D-15,  0.15782298482291759D-15,
     *  0.69877210417305865D-16,  0.30770894423130805D-16,
     *  0.13232824459894866D-16,  0.48877089380958622D-17/
        data pp302/
     1  0.81283234488100382D+00,  0.98895539554623310D-01,
     2  -.44290239857700597D-02,  -.18320809862747393D-02,
     3  -.29810274051669795D-03,  -.49208457976143782D-04,
     4  -.10292065709246516D-04,  -.25762336033580606D-05,
     5  -.71407554686987550D-06,  -.21206516220954901D-06,
     6  -.66356730459563692D-07,  -.21627637797399522D-07,
     7  -.72829599427548134D-08,  -.25189722055780386D-08,
     8  -.89091927171400490D-09,  -.32112588424413242D-09,
     9  -.11764428932230438D-09,  -.43711164973986115D-10,
     *  -.16442988502992426D-10,  -.62533061582846865D-11/
        data pq302/
     1  -.24013555743114806D-11,  -.93020843566662674D-12,
     2  -.36316660368393306D-12,  -.14279447947920465D-12,
     3  -.56509105684687737D-13,  -.22494934594036900D-13,
     4  -.90032211320203132D-14,  -.36213606584135740D-14,
     5  -.14633173366713614D-14,  -.59382337640908686D-15,
     6  -.24192688842534103D-15,  -.98930179281178784D-16,
     7  -.40592380586053327D-16,  -.16711763125444829D-16,
     8  -.68967575422708601D-17,  -.28587146672013325D-17,
     9  -.11809339868966230D-17,  -.49579730725011510D-18,
     *  -.20352050381624412D-18,  -.88721704344441164D-19,
     *  -.36436051273681488D-19,  -.17847208020188726D-19,
     *  -.38075275040912580D-20,  -.51082262447195014D-20,
     *  0.45668982398268629D-20,  -.29970899635652539D-20,
     *  0.25660848369523941D-20,  -.30914830727080279D-20/
        data pp303/
     1  -.18434340515001677D+00,  -.22587194478313240D-01,
     2  0.86325047443772183D-03,  0.41244509075442480D-03,
     3  0.72895410561500192D-04,  0.12609028321320251D-04,
     4  0.26291133617869658D-05,  0.65024698250502679D-06,
     5  0.17933420739056496D-06,  0.53170684672225557D-07,
     6  0.16623472188457791D-07,  0.54151172831758113D-08,
     7  0.18228428948740514D-08,  0.63031435923320993D-09,
     8  0.22289256056279535D-09,  0.80329678476531474D-10,
     9  0.29425832074016328D-10,  0.10932440048314011D-10,
     *  0.41122476858374644D-11,  0.15638213932134444D-11/
        data pq303/
     1  0.60050520077969939D-12,  0.23260894686288906D-12,
     2  0.90811370156680610D-13,  0.35705554180565265D-13,
     3  0.14129744892772919D-13,  0.56246221294530042D-14,
     4  0.22511284155823831D-14,  0.90545869335239362D-15,
     5  0.36587332994141641D-15,  0.14847217229711369D-15,
     6  0.60487926087222827D-16,  0.24734818906534833D-16,
     7  0.10148974365732440D-16,  0.41782247965721382D-17,
     8  0.17243729071924956D-17,  0.71467934779960633D-18,
     9  0.29536939006916933D-18,  0.12390555253286896D-18,
     *  0.50939052437952970D-19,  0.22121060012615128D-19,
     *  0.91013191235460916D-20,  0.43995503214817100D-20,
     *  0.10115583169789512D-20,  0.12204704634791642D-20,
     *  -.10093621764855858D-20,  0.69039688047115557D-21,
     *  -.57728844185519854D-21,  0.70298234958309903D-21/
c
        data pp401/
     1  0.39018677786550249D+01,  0.16851341098103432D+01,
     2  0.42195500941767458D+00,  0.12614225036207673D+00,
     3  0.42528827607411628D-01,  0.15376932021770721D-01,
     4  0.58029521574909747D-02,  0.22540019688134564D-02,
     5  0.89398334654638006D-03,  0.36024524429963029D-03,
     6  0.14699116435159234D-03,  0.60585132061196974D-04,
     7  0.25179996064907705D-04,  0.10538497539016678D-04,
     8  0.44369438716281621D-05,  0.18776472312568226D-05,
     9  0.79814355962941714D-06,  0.34060390988796189D-06,
     *  0.14585622819481881D-06,  0.62653289952202062D-07/
        data pq401/
     1  0.26987851976756042D-07,  0.11654147695641008D-07,
     2  0.50440474515004644D-08,  0.21876431217577710D-08,
     3  0.95059580754158195D-09,  0.41378071846656756D-09,
     4  0.18040155646179258D-09,  0.78768657476760923D-10,
     5  0.34439983425740262D-10,  0.15077414552161808D-10,
     6  0.66085725067327554D-11,  0.28998222006423676D-11,
     7  0.12737592074356362D-11,  0.56005116354520674D-12,
     8  0.24647175630212340D-12,  0.10856315892765265D-12,
     9  0.47857803553233579D-13,  0.21113382073307800D-13,
     *  0.93213981218110352D-14,  0.41181575593285782D-14,
     *  0.18205876854482999D-14,  0.80534733407363785D-15,
     *  0.35646725242313674D-15,  0.15782375849236940D-15,
     *  0.69876628410577284D-16,  0.30771313114889692D-16,
     *  0.13232484310330145D-16,  0.48881295742822432D-17/
        data pp402/
     1  0.11431166964300176D+01,  0.14052525705941900D+00,
     2  -.57451956252559919D-02,  -.25851982595184475D-02,
     3  -.44074824557087715D-03,  -.74798970720276733D-04,
     4  -.15640550665962868D-04,  -.38894523694527611D-05,
     5  -.10746030599455770D-05,  -.31873949037054026D-06,
     6  -.99674293045694141D-07,  -.32474561495245553D-07,
     7  -.10932903961194347D-07,  -.37807542154839687D-08,
     8  -.13370321766805737D-08,  -.48188218789786451D-09,
     9  -.17652560374243095D-09,  -.65585376443676725D-10,
     *  -.24670494077966351D-10,  -.93819411844473442D-11/
        data pq402/
     1  -.36026993187234571D-11,  -.13955402206397811D-11,
     2  -.54482884771064759D-12,  -.21421947665486962D-12,
     3  -.84773538791092424D-13,  -.33745957860022205D-13,
     4  -.13506124901315760D-13,  -.54325153187272390D-14,
     5  -.21951520688142431D-14,  -.89080039716296246D-15,
     6  -.36291516168432166D-15,  -.14840436495398032D-15,
     7  -.60892091266811243D-16,  -.25068777683896441D-16,
     8  -.10345875206163760D-16,  -.42880688173653566D-17,
     9  -.17719515768506559D-17,  -.74351477437257553D-18,
     *  -.30552139537504588D-18,  -.13284137942520662D-18,
     *  -.54626051380056147D-19,  -.26517930597223990D-19,
     *  -.59552778356987718D-20,  -.74292346001316379D-20,
     *  0.63129184255732690D-20,  -.42531652402258260D-20,
     *  0.35865335311152342D-20,  -.43533916318516688D-20/
        data pp403/
     1  -.51462775669903050D+00,  -.64216911983108928D-01,
     2  0.21794221139236540D-02,  0.11655623639981330D-02,
     3  0.21554091561567939D-03,  0.38199541065453202D-04,
     4  0.79775983185033185D-05,  0.19634657485997273D-05,
     5  0.53986172046626648D-06,  0.15984501283321680D-06,
     6  0.49941034774588240D-07,  0.16262040981021843D-07,
     7  0.54727869133135850D-08,  0.18920963691391400D-08,
     8  0.66900546552936414D-09,  0.24108598213026356D-09,
     9  0.88307146494142898D-10,  0.32806651518004620D-10,
     *  0.12339753260811390D-10,  0.46924564193761021D-11/
        data pq403/
     1  0.18018489451916758D-11,  0.69794073183604345D-12,
     2  0.27247361418339513D-12,  0.10713055135623023D-12,
     3  0.42394177999177606D-13,  0.16875645395438310D-13,
     4  0.67540321848778299D-14,  0.27166133536660586D-14,
     5  0.10977080620842981D-14,  0.44544919305098929D-15,
     6  0.18147619934620346D-15,  0.74209004579336370D-16,
     7  0.30448685046490356D-16,  0.12535239355023750D-16,
     8  0.51734905710853951D-17,  0.21440334979636298D-17,
     9  0.88638698002320225D-18,  0.37162301965532966D-18,
     *  0.15293994399675506D-18,  0.66240735093380600D-19,
     *  0.27291319229920683D-19,  0.13070272898516899D-19,
     *  0.31593086485868397D-20,  0.35414788188912957D-20,
     *  -.27553823622321562D-20,  0.19464721571316455D-20,
     *  -.15977371360178927D-20,  0.19648909087269712D-20/
        data pp404/
     1  0.11009478384967124D+00,  0.13876572501598563D-01,
     2  -.43872387982864407D-03,  -.25103909108123606D-03,
     3  -.47548501684726399D-04,  -.85301709147109837D-05,
     4  -.17828283189054509D-05,  -.43773958869823350D-06,
     5  -.12017583769190051D-06,  -.35558109386997082D-07,
     6  -.11105854195376817D-07,  -.36156412326153439D-08,
     7  -.12166480061465112D-08,  -.42059400330197670D-09,
     8  -.14870430165552293D-09,  -.53585434551244028D-10,
     9  -.19627104806708857D-10,  -.72914038232302031D-11,
     *  -.27425018583246417D-11,  -.10428783420542192D-11/
        data pq404/
     1  -.40044791480399215D-12,  -.15511059499105146D-12,
     2  -.60554081342238175D-13,  -.23808332391888323D-13,
     3  -.94214777021348955D-14,  -.37503410886617684D-14,
     4  -.15009679230984823D-14,  -.60371822010455499D-15,
     5  -.24394491071429390D-15,  -.98992340251291868D-16,
     6  -.40329424419660210D-16,  -.16491395224267179D-16,
     7  -.67665702269193055D-17,  -.27856715194838706D-17,
     8  -.11497058879642998D-17,  -.47645138338800774D-18,
     9  -.19700586331801116D-18,  -.82572489040820432D-19,
     *  -.34000297186267644D-19,  -.14706558360255335D-19,
     *  -.60633333687917171D-20,  -.28902408590117456D-20,
     *  -.71591677720267869D-21,  -.77366945180396239D-21,
     *  0.58200672858228701D-21,  -.41869175888671713D-21,
     *  0.34014956472096828D-21,  -.42063618638122185D-21/
c
        data pp501/
     1  0.38254087718204091D+01,  0.16752525985096849D+01,
     2  0.42222373296084423D+00,  0.12631993070378939D+00,
     3  0.42563963063907327D-01,  0.15383376560621708D-01,
     4  0.58043029053521072D-02,  0.22543323861501587D-02,
     5  0.89407380002186098D-03,  0.36027197014702931D-03,
     6  0.14699950572045915D-03,  0.60587846601739315D-04,
     7  0.25180909265327946D-04,  0.10538813177650589D-04,
     8  0.44370554546811917D-05,  0.18776874365357210D-05,
     9  0.79815828491079636D-06,  0.34060937999890431D-06,
     *  0.14585828557367704D-06,  0.62654072276544533D-07/
        data pq501/
     1  0.26988152368310373D-07,  0.11654264047587169D-07,
     2  0.50440928736508840D-08,  0.21876609803235577D-08,
     3  0.95060287447427868D-09,  0.41378353151905716D-09,
     4  0.18040268229561317D-09,  0.78769110304990646D-10,
     5  0.34440166399106716D-10,  0.15077488801865687D-10,
     6  0.66086027558713167D-11,  0.28998345699477838D-11,
     7  0.12737642826572758D-11,  0.56005325289382496D-12,
     8  0.24647261864283385D-12,  0.10856351626602986D-12,
     9  0.47857951352442600D-13,  0.21113443987681991D-13,
     *  0.93214236417649165D-14,  0.41181685696995129D-14,
     *  0.18205922310917802D-14,  0.80534948124233304D-15,
     *  0.35646780935213530D-15,  0.15782431942795072D-15,
     *  0.69876235467726129D-16,  0.30771607048921684D-16,
     *  0.13232250709861906D-16,  0.48884220344285304D-17/
        data pp502/
     1  0.14489527237684807D+01,  0.18005130226205207D+00,
     2  -.68200897979345753D-02,  -.32959196263690867D-02,
     3  -.58129007155367612D-03,  -.10057712612422728D-03,
     4  -.21043542110492737D-04,  -.52111217162618795D-05,
     5  -.14364169618692496D-05,  -.42564287996659513D-06,
     6  -.13303976851294130D-06,  -.43332723664607034D-07,
     7  -.14585705642159407D-07,  -.50433087511301256D-08,
     8  -.17833643887987707D-08,  -.64270330349132343D-09,
     9  -.23542672925933298D-09,  -.87465820213356481D-10,
     *  -.32900009510900375D-10,  -.12511238554333512D-10/
        data pq502/
     1  -.48042655360483395D-11,  -.18609480052832837D-11,
     2  -.72651744938904691D-12,  -.28565373980163720D-12,
     3  -.11304126957799890D-12,  -.44998167818451598D-13,
     4  -.18009460183676729D-13,  -.72438282376209543D-14,
     5  -.29270455346314198D-14,  -.11877992126764363D-14,
     6  -.48391171592943572D-15,  -.19788158661871213D-15,
     7  -.81192977825505329D-16,  -.33426172156760240D-16,
     8  -.13795238047996190D-16,  -.57174223262046415D-17,
     9  -.23631484129355590D-17,  -.99117227113651097D-18,
     *  -.40760121090027551D-18,  -.17688286316399744D-18,
     *  -.72808625301087468D-19,  -.35106605377975781D-19,
     *  -.81829938299422833D-20,  -.96729769254185614D-20,
     *  0.78846898301940367D-20,  -.54289013682020549D-20,
     *  0.45209354040702145D-20,  -.55232322170043773D-20/
        data pp503/
     1  -.97338179770672523D+00,  -.12350597978705854D+00,
     2  0.37917633729415292D-02,  0.22316444142740918D-02,
     3  0.42635365458987785D-03,  0.76866774171379025D-04,
     4  0.16082085485298122D-04,  0.39459697688134048D-05,
     5  0.10825825733517753D-05,  0.32020009722729911D-06,
     6  0.99989247975458972D-07,  0.32549284235064064D-07,
     7  0.10951989434761174D-07,  0.37859281726083754D-08,
     8  0.13385037837066596D-08,  0.48231765552045194D-09,
     9  0.17665883476949594D-09,  0.65627317172524255D-10,
     *  0.24684026410212427D-10,  0.93864024742053540D-11/
        data pq503/
     1  0.36041982711789995D-11,  0.13960524088012973D-11,
     2  0.54500651670099411D-12,  0.21428194607638160D-12,
     3  0.84795774179537319D-13,  0.33753960333082400D-13,
     4  0.13509035108419284D-13,  0.54335827320066317D-14,
     5  0.21955482608100632D-14,  0.89094741632120002D-15,
     6  0.36297103071387454D-15,  0.14842483707643409D-15,
     7  0.60900014884531482D-16,  0.25071331064319448D-16,
     8  0.10347534833834035D-16,  0.42880637612225592D-17,
     9  0.17731822341505570D-17,  0.74310926480123689D-18,
     *  0.30605966728460063D-18,  0.13230296070156242D-18,
     *  0.54565180111465099D-19,  0.25953285069644199D-19,
     *  0.65008826399496288D-20,  0.69070923068203071D-20,
     *  -.51130394691616754D-20,  0.37100763490944864D-20,
     *  -.29993399454493602D-20,  0.37196517864512203D-20/
        data pp504/
     1  0.41593081118813439D+00,  0.53402617704231635D-01,
     2  -.15136180525072275D-02,  -.96176045793187528D-03,
     3  -.18809032766752538D-03,  -.34308326318661532D-04,
     4  -.71858197634353200D-05,  -.17594089355073519D-05,
     5  -.48198973961557306D-06,  -.14246149898305196D-06,
     6  -.44471329662623971D-07,  -.14473803401976825D-07,
     7  -.48694496871115708D-08,  -.16831485389481336D-08,
     8  -.59503651377371990D-09,  -.21440655014470295D-09,
     9  -.78528230323610888D-10,  -.29171847592909959D-10,
     *  -.10972017291258666D-10,  -.41721757119403872D-11/
        data pq504/
     1  -.16020141321288746D-11,  -.62051837963455401D-12,
     2  -.24224268302063749D-12,  -.95242595538655902D-13,
     3  -.37689208489041371D-13,  -.15002551047091162D-13,
     4  -.60043032054594518D-14,  -.24150311389982704D-14,
     5  -.97583837653147066D-15,  -.39599115576476569D-15,
     6  -.16132597866477427D-15,  -.65968616888998993D-16,
     7  -.27067456785613390D-16,  -.11143065992347670D-16,
     8  -.45990687297967291D-17,  -.19058048922272947D-17,
     9  -.78820269940291129D-18,  -.33022998580475440D-18,
     *  -.13608011271149593D-18,  -.58748042099043152D-19,
     *  -.24245907289820645D-19,  -.11478915639764799D-19,
     *  -.29436327714443698D-20,  -.30174117770902813D-20,
     *  0.21537781332038608D-20,  -.15944278868651365D-20,
     *  0.12745514376764545D-20,  -.15904767715323181D-20/
        data pp505/
     1  -.76459006834615787D-01,  -.98815113006582681D-02,
     2  0.26872354316964587D-03,  0.17768034171265981D-03,
     3  0.35135456495699744D-04,  0.64445388509876372D-05,
     4  0.13507478611324673D-05,  0.33041733670227959D-06,
     5  0.90453475480918138D-07,  0.26725847399013718D-07,
     6  0.83413688668117886D-08,  0.27145405423403702D-08,
     7  0.91320042024126490D-09,  0.31563863391153923D-09,
     8  0.11158305302954924D-09,  0.40205278898364730D-10,
     9  0.14725281379225508D-10,  0.54701109424199391D-11,
     *  0.20573788582335062D-11,  0.78232434247154199D-12/
        data pq505/
     1  0.30039155433122061D-12,  0.11635194616087564D-12,
     2  0.45422150419599829D-13,  0.17858565786691895D-13,
     3  0.70669326967266187D-14,  0.28130524896073484D-14,
     4  0.11258338205902424D-14,  0.45282822972342885D-15,
     5  0.18297336645429419D-15,  0.74249703878368456D-16,
     6  0.30249138561278513D-16,  0.12369305416182954D-16,
     7  0.50752216396735201D-17,  0.20893486182159492D-17,
     8  0.86234071045810534D-18,  0.35733837720982213D-18,
     9  0.14779920902122665D-18,  0.61914374190987371D-19,
     *  0.25519953881308447D-19,  0.11010370934695212D-19,
     *  0.45456434802570281D-20,  0.21471686951862105D-20,
     *  0.55692899855965471D-21,  0.56093558132067677D-21,
     *  -.39294285115429561D-21,  0.29393403199293823D-21,
     *  -.23360046823790050D-21,  0.29246014628609975D-21/
c
        data pp601/
     1  0.37677231855660316D+01,  0.16676378100854034D+01,
     2  0.42240577947922629D+00,  0.12645619945292931D+00,
     3  0.42591754996480955D-01,  0.15388555165220779D-01,
     4  0.58053911680781930D-02,  0.22545979786835959D-02,
     5  0.89414635944418956D-03,  0.36029338584576949D-03,
     6  0.14700618608898762D-03,  0.60590019931903143D-04,
     7  0.25181640256276939D-04,  0.10539065804811463D-04,
     8  0.44371447540992696D-05,  0.18777196104988946D-05,
     9  0.79817006811202282D-06,  0.34061375702364844D-06,
     *  0.14585993177854934D-06,  0.62654698235455453D-07/
        data pq601/
     1  0.26988392714949287D-07,  0.11654357140551285D-07,
     2  0.50441292153270565D-08,  0.21876752685667604D-08,
     3  0.95060852851530182D-09,  0.41378578213912180D-09,
     4  0.18040358302741852D-09,  0.78769472591321050D-10,
     5  0.34440312786611397D-10,  0.15077548204899283D-10,
     6  0.66086269564241247D-11,  0.28998444658481421D-11,
     7  0.12737683430110910D-11,  0.56005492443025647D-12,
     8  0.24647330855167895D-12,  0.10856380213710412D-12,
     9  0.47858069617568805D-13,  0.21113493510667618D-13,
     *  0.93214440690337201D-14,  0.41181773668530433D-14,
     *  0.18205958665908011D-14,  0.80535118727507909D-15,
     *  0.35646826632065906D-15,  0.15782475711577425D-15,
     *  0.69875945967050110D-16,  0.30771830699487047D-16,
     *  0.13232076123341760D-16,  0.48886428683043585D-17/
        data pp602/
     1  0.17373806550403682D+01,  0.21812524438345976D+00,
     2  -.77303223898448642D-02,  -.39772633720686991D-02,
     3  -.72024973442181649D-03,  -.12647014911958117D-03,
     4  -.26484855740921951D-04,  -.65390843834478480D-05,
     5  -.17992140735121587D-05,  -.53272137366753187D-06,
     6  -.16644161115531024D-06,  -.54199374483750152D-07,
     7  -.18240660387126639D-07,  -.63064445554998818D-08,
     8  -.22298614791885197D-08,  -.80357311935953377D-09,
     9  -.29434273539163004D-09,  -.10935094393398587D-09,
     *  -.41131033872389026D-10,  -.15641033108931982D-10/
        data pq602/
     1  -.60059987306183133D-11,  -.23264128258637046D-11,
     2  -.90822583025169891D-12,  -.35709495581514234D-12,
     3  -.14131147469373717D-12,  -.56251268141615294D-13,
     4  -.22513119210431268D-13,  -.90552598896389416D-14,
     5  -.36589830580372667D-14,  -.14848143806590300D-14,
     6  -.60491447996975903D-15,  -.24736108841001508D-15,
     7  -.10149474690147745D-15,  -.41783854314301988D-16,
     8  -.17244782273485977D-16,  -.71467776974811632D-17,
     9  -.29544740439604653D-17,  -.12387871992698423D-17,
     *  -.50973755491846119D-18,  -.22086863081592007D-18,
     *  -.90986120405879259D-19,  -.43636769108156302D-19,
     *  -.10467836448694078D-19,  -.11861416042961614D-19,
     *  0.93321932103117791D-20,  -.65471541950321869D-20,
     *  0.53938680048353641D-20,  -.66274015962023165D-20/
        data pp603/
     1  -.15502376602505001D+01,  -.19965386402987392D+00,
     2  0.56122285567621069D-02,  0.35943319056733167D-02,
     3  0.70427298032615859D-03,  0.12865282016208681D-03,
     4  0.26964712746156548D-04,  0.66018951031853419D-05,
     5  0.18081767966375937D-05,  0.53435708462917259D-06,
     6  0.16679293326019687D-06,  0.54282585873350299D-07,
     7  0.18261898924695640D-07,  0.63121997813478879D-08,
     8  0.22314979644861577D-08,  0.80405728725687262D-09,
     9  0.29449084703409005D-09,  0.10939756461378304D-09,
     *  0.41146075133189728D-10,  0.15645991583402294D-10/
        data pq603/
     1  0.60076646603189472D-11,  0.23269820499621391D-11,
     2  0.90842327842629812D-12,  0.35716437810339187D-12,
     3  0.14133618441101386D-12,  0.56260160979409791D-13,
     4  0.22516353161928361D-13,  0.90564460360426062D-14,
     5  0.36594233076217569D-14,  0.14849777522863874D-14,
     6  0.60497655879452118D-15,  0.24738384065904000D-15,
     7  0.10150355303647568D-15,  0.41786695379402990D-16,
     8  0.17246623284813600D-16,  0.71467745037755820D-17,
     9  0.29558334962003676D-17,  0.12383391210678873D-17,
     *  0.51033235532095648D-18,  0.22027449600542793D-18,
     *  0.90920170321027483D-19,  0.43013612530038849D-19,
     *  0.11070567877463668D-19,  0.11283970541941591D-19,
     *  -.80080462293933084D-20,  0.59465820027440374D-20,
     *  -.47452051469770866D-20,  0.59279905448058109D-20/
        data pp604/
     1  0.99278667373190931D+00,  0.12955050194704702D+00,
     2  -.33340832363278052D-02,  -.23244479493311002D-02,
     3  -.46600965340380611D-03,  -.86094372309369314D-04,
     4  -.18068447024293746D-04,  -.44153342698792889D-05,
     5  -.12075839629013914D-05,  -.35661848638492543D-06,
     6  -.11127501494736187D-06,  -.36207105040263060D-07,
     7  -.12179359177046036D-07,  -.42094201476876461D-08,
     8  -.14880306945532180D-08,  -.53614618188112363D-09,
     9  -.19636024258820500D-09,  -.72942095034168746D-10,
     *  -.27434066014235968D-10,  -.10431764821137328D-10/
        data pq604/
     1  -.40054805212688222D-11,  -.15514480207953959D-11,
     2  -.60565944474594150D-12,  -.23812502756566617D-12,
     3  -.94229618720517909D-13,  -.37508751693418554D-13,
     4  -.15011621258968529D-13,  -.60378944430342449D-14,
     5  -.24397134233431643D-14,  -.99002149172995305D-15,
     6  -.40333150674542088D-15,  -.16492762047160491D-15,
     7  -.67670994937557625D-16,  -.27858430307431185D-16,
     8  -.11498157180776302D-16,  -.47645156347803273D-17,
     9  -.19708539614527343D-17,  -.82545984207140024D-18,
     *  -.34035280074786410D-18,  -.14671957740289573D-18,
     *  -.60600897499395424D-19,  -.28539243100128306D-19,
     *  -.75133180089549870D-20,  -.73942900121905448D-20,
     *  0.50487848934436982D-20,  -.38309335404994660D-20,
     *  0.30204166392072765D-20,  -.37988155299059748D-20/
        data pp605/
     1  -.36488693810650324D+00,  -.47955453422065958D-01,
     2  0.11789561350799347D-02,  0.85902408741227226D-03,
     3  0.17409511936384011D-03,  0.32337561846341528D-04,
     4  0.67920614915616804D-05,  0.16583800038882481D-05,
     5  0.45325058712382731D-06,  0.13380434109995046D-06,
     6  0.41743211509180737D-07,  0.13581191361483488D-07,
     7  0.45681551652084976D-08,  0.15787744382812955D-08,
     8  0.55808014341929831D-09,  0.20107509476657507D-09,
     9  0.73641287511522562D-10,  0.27355234663049332D-10,
     *  0.10288403219722157D-10,  0.39121188970700122D-11/
        data pq605/
     1  0.15021247489011944D-11,  0.58181676674129656D-12,
     2  0.22713053128225183D-12,  0.89299781800197029D-13,
     3  0.35337137812464888D-13,  0.14066152812771044D-13,
     4  0.56294928473447810D-14,  0.22642598817414161D-14,
     5  0.91491088986014100D-15,  0.37126487186096211D-15,
     6  0.15125190260160184D-15,  0.61848807207485901D-16,
     7  0.25376990715645612D-16,  0.10447030775757728D-16,
     8  0.43118849359478824D-17,  0.17866937484863257D-17,
     9  0.73912484004612779D-18,  0.30952930232431161D-18,
     *  0.12765629789949305D-18,  0.54996138586636285D-19,
     *  0.22723138585041065D-19,  0.10677332425392561D-19,
     *  0.28417716173206413D-20,  0.27493746988768674D-20,
     *  -.18404462312721863D-20,  0.14121868588224736D-20,
     *  -.11065330690044813D-20,  0.13966295254560770D-20/
        data pp606/
     1  0.57685586254377491D-01,  0.76147884242815381D-02,
     2  -.18204651838205777D-03,  -.13626874913992249D-03,
     3  -.27791932573628073D-04,  -.51786045990707781D-05,
     4  -.10882627260858426D-05,  -.26559253343719371D-06,
     5  -.72559422328581835D-07,  -.21415698740187347D-07,
     6  -.66803685284737897D-08,  -.21733301638286235D-08,
     7  -.73099094899344654D-09,  -.25262716087395124D-09,
     8  -.89299418077949814D-10,  -.32173963173642068D-10,
     9  -.11783201226459411D-10,  -.43770247441258786D-11,
     *  -.16462048722977301D-11,  -.62595891091969404D-12/
        data pq606/
     1  -.24034663891399476D-12,  -.93092964116084185D-13,
     2  -.36341676172530401D-13,  -.14288243202701027D-13,
     3  -.56540410231476538D-14,  -.22506200646327392D-14,
     4  -.90073180535090770D-15,  -.36228633040359746D-15,
     5  -.14638750468116938D-15,  -.59403033596518753D-16,
     6  -.24200552808064649D-16,  -.98959003582606104D-17,
     7  -.40603538151944428D-17,  -.16715364315083422D-17,
     8  -.68990884509796655D-18,  -.28587107425531246D-18,
     9  -.11826512620499012D-18,  -.49522985626653680D-19,
     *  -.20427268803633317D-19,  -.87971535303754366D-20,
     *  -.36354990209594036D-20,  -.17060327460239494D-20,
     *  -.45696852374858425D-21,  -.43768782350813277D-21,
     *  0.28950067602695918D-21,  -.22365056534778576D-21,
     *  0.17458652015549726D-21,  -.22083387584216921D-21/
c
        data pp701/
     1  0.37218619591127502D+01,  0.16614737654480956D+01,
     2  0.42253711433527078D+00,  0.12656606107200410D+00,
     3  0.42614703710216246D-01,  0.15392883665027977D-01,
     4  0.58063029228191454D-02,  0.22548201388998910D-02,
     5  0.89420695827922242D-03,  0.36031125592335187D-03,
     6  0.14701175795550149D-03,  0.60591832178372485D-04,
     7  0.25182249703484246D-04,  0.10539276405138520D-04,
     8  0.44372191923049661D-05,  0.18777464286374385D-05,
     9  0.79817988943270106D-06,  0.34061740516883688D-06,
     *  0.14586130381728739D-06,  0.62655219934215317D-07/
        data pq701/
     1  0.26988593026090964D-07,  0.11654434725629798D-07,
     2  0.50441595026955210D-08,  0.21876871763634842D-08,
     3  0.95061324054617193D-09,  0.41378765777459226D-09,
     4  0.18040433368043318D-09,  0.78769774512431422D-10,
     5  0.34440434782073583D-10,  0.15077597709608324D-10,
     6  0.66086471243792859D-11,  0.28998527127361414D-11,
     7  0.12737717267570072D-11,  0.56005631741596270D-12,
     8  0.24647388349958639D-12,  0.10856404036357191D-12,
     9  0.47858168188352838D-13,  0.21113534774477794D-13,
     *  0.93214610989624082D-14,  0.41181846907555805D-14,
     *  0.18205988955262040D-14,  0.80535260153830970D-15,
     *  0.35646865440363136D-15,  0.15782511483870822D-15,
     *  0.69875720503281706D-16,  0.30772009745251384D-16,
     *  0.13231938464189466D-16,  0.48888185599091165D-17/
        data pp702/
     1  0.20125480137600561D+01,  0.25510951220730643D+00,
     2  -.85183315261118409D-02,  -.46364330865174361D-02,
     3  -.85794201683355939D-03,  -.15244114796276727D-03,
     4  -.31955384186636431D-04,  -.78720456812186042D-05,
     5  -.21628070837092827D-05,  -.63994183916178228D-06,
     6  -.19987281023849389D-06,  -.65072853299800140D-07,
     7  -.21897343630967429D-07,  -.75700465178370626D-08,
     8  -.26764907133673024D-08,  -.96448195062271660D-09,
     9  -.35327065946105420D-09,  -.13123981506465030D-09,
     *  -.49363266300692467D-10,  -.18771225668113478D-10/
        data pq702/
     1  -.72078655806773881D-11,  -.27919232969435515D-11,
     2  -.10899500410388118D-11,  -.42854173615805201D-12,
     3  -.16958365991440420D-12,  -.67505080964369452D-13,
     4  -.27017037298374719D-13,  -.10866786551871341D-13,
     5  -.43909558311536316D-14,  -.17818426349029549D-14,
     6  -.72592221093656511D-15,  -.29684241640583614D-15,
     7  -.12179722239844714D-15,  -.50141768551696209D-16,
     8  -.20694469718085475D-16,  -.85761365042219738D-17,
     9  -.35458987481589530D-17,  -.14863700603229588D-17,
     *  -.61191712704567136D-18,  -.26481204603917726D-18,
     *  -.10915973282251419D-18,  -.52122348491348304D-19,
     *  -.12796334283645636D-19,  -.14007753647012892D-19,
     *  0.10684975822383949D-19,  -.76214287814692870D-20,
     *  0.62198229183945003D-20,  -.76815512240983839D-20/
        data pp703/
     1  -.22381560570497201D+01,  -.29211453358949060D+00,
     2  0.75822513974295486D-02,  0.52422561917951591D-02,
     3  0.10485036863555158D-02,  0.19358031727005204D-03,
     4  0.40641033860442751D-04,  0.99342983476122324D-05,
     5  0.27171593221304036D-05,  0.80240824836479862D-06,
     6  0.25037093096815598D-06,  0.81466282913475269D-07,
     7  0.27403607034297615D-07,  0.94712046871908398D-08,
     8  0.33480710499331144D-08,  0.12063293654148297D-08,
     9  0.44181065720765045D-09,  0.16411974244044410D-09,
     *  0.61726656203948330D-10,  0.23471472981356032D-10/
        data pq703/
     1  0.90123317854666341D-11,  0.34907582276617563D-11,
     2  0.13627338053940804D-11,  0.53578132896066604D-12,
     3  0.21201664746268145D-12,  0.84394693036295185D-13,
     4  0.33776148381786987D-13,  0.13585262691623604D-13,
     5  0.54893552404126694D-14,  0.22275483878961995D-14,
     6  0.90749588621153694D-15,  0.37108716064859282D-15,
     7  0.15225974177890078D-15,  0.62681480972888382D-16,
     8  0.25870841896311839D-16,  0.10720171520627354D-16,
     9  0.44343952566969706D-17,  0.18572962737005895D-17,
     *  0.76578128563961780D-18,  0.33013303406365010D-18,
     *  0.13635420136285367D-18,  0.64227560988129315D-19,
     *  0.16891812464347503D-19,  0.16649814551867641D-19,
     *  -.11390002758735547D-19,  0.86322684687763227D-20,
     *  -.68100924311384770D-20,  0.85633646146845472D-20/
        data pp704/
     1  0.19100112027975359D+01,  0.25283139469320259D+00,
     2  -.59607803572177275D-02,  -.45216803308268900D-02,
     3  -.92498392810961576D-03,  -.17266436845332297D-03,
     4  -.36303541843342016D-04,  -.88585385957818096D-05,
     5  -.24195606635584713D-05,  -.71402003803242680D-06,
     6  -.22271234522464068D-06,  -.72452034427096353D-07,
     7  -.24368303323182003D-07,  -.84214266888115820D-08,
     8  -.29767948084824936D-08,  -.10725089527583998D-08,
     9  -.39278665615295219D-09,  -.14590499880305015D-09,
     *  -.54874840775247437D-10,  -.20865740018408978D-10/
        data pq704/
     1  -.80117033547990715D-11,  -.31031495910615521D-11,
     2  -.12114068140363179D-11,  -.47628096204203173D-12,
     3  -.18847023612274136D-12,  -.75021461102599079D-13,
     4  -.30024681552113364D-13,  -.12076316650475575D-13,
     5  -.48796226670643810D-14,  -.19801156725430360D-14,
     6  -.80669060996810827D-15,  -.32986538045767527D-15,
     7  -.13534591326079055D-15,  -.55718144432078351D-16,
     8  -.22997115329440947D-16,  -.95290449905829462D-17,
     9  -.39422696421147198D-17,  -.16507360455817693D-17,
     *  -.68095137450561683D-18,  -.29319762814729117D-18,
     *  -.12117960555500328D-18,  -.56824507710936565D-19,
     *  -.15274977458354596D-19,  -.14548748692300768D-19,
     *  0.95580602663319597D-20,  -.74118488286008934D-20,
     *  0.57735996845929099D-20,  -.73126476230383165D-20/
        data pp705/
     1  -.10528053349057232D+01,  -.14041612298168264D+00,
     2  0.31489789757473765D-02,  0.25069483735341146D-02,
     3  0.51832582539319735D-03,  0.97265058954306766D-04,
     4  0.20468382605847883D-04,  0.49907832483151386D-05,
     5  0.13622331126166373D-05,  0.40185550483557649D-06,
     6  0.12532120921713985D-06,  0.40764888401608457D-07,
     7  0.13709863274810473D-07,  0.47377793441242474D-08,
     8  0.16746532288662550D-08,  0.60334717292453216D-09,
     9  0.22096109768508296D-09,  0.82077412489710387D-10,
     *  0.30868984290480759D-10,  0.11737600295023750D-10/
        data pq705/
     1  0.45067918740488814D-11,  0.17455929444409137D-11,
     2  0.68144105825003412D-12,  0.26791673265747120D-12,
     3  0.10601760086413248D-12,  0.42200684869656438D-13,
     4  0.16889288067203407D-13,  0.67930765373224137D-14,
     5  0.27448428226510536D-14,  0.11138355074707743D-14,
     6  0.45377123001861764D-15,  0.18555212719703876D-15,
     7  0.76133179458070717D-16,  0.31341816369243156D-16,
     8  0.12936103547446201D-16,  0.53600907653380478D-17,
     9  0.22176866005426642D-17,  0.92848645495692808D-18,
     *  0.38310522821809187D-18,  0.16485467664490296D-18,
     *  0.68157169626910446D-19,  0.31891280883436029D-19,
     *  0.86630162042873308D-20,  0.81152187088942315D-20,
     *  -.52224027605759087D-20,  0.40978733249030586D-20,
     *  -.31714203532080449D-20,  0.40320035952428739D-20/
        data pp706/
     1  0.33285294497406548D+00,  0.44599056248128210D-01,
     2  -.97005565464903447D-03,  -.79543846358865944D-03,
     3  -.16548421498537097D-03,  -.31149603442256873D-04,
     4  -.65587911718003235D-05,  -.15985538312079499D-05,
     5  -.43615243252570581D-06,  -.12863616423443776D-06,
     6  -.40111567611657433D-07,  -.13046808979878611D-07,
     7  -.43876741928342365D-08,  -.15162291232111320D-08,
     8  -.53592865225673249D-09,  -.19308279443682491D-09,
     9  -.70711125295883570D-10,  -.26265895874790300D-10,
     *  -.98784373006011709D-11,  -.37561514701011893D-11/
        data pq706/
     1  -.14422134889730695D-11,  -.55860343519593104D-12,
     2  -.21806588695964331D-12,  -.85735023545610696D-13,
     3  -.33926226243814690D-13,  -.13504432887386897D-13,
     4  -.54046498932943583D-14,  -.21738129926359965D-14,
     5  -.87836027779753434D-15,  -.35643128784044359D-15,
     6  -.14520828377487082D-15,  -.59377228354081688D-16,
     7  -.24362829312164202D-16,  -.10029450668902401D-16,
     8  -.41395962896974191D-17,  -.17152298809962431D-17,
     9  -.70968983040361832D-18,  -.29710584667988272D-18,
     *  -.12260684093087111D-18,  -.52740568753758343D-19,
     *  -.21809111437587662D-19,  -.10191612129335016D-19,
     *  -.27854663585229066D-20,  -.25840254275587541D-20,
     *  0.16422832879941024D-20,  -.12979251518010636D-20,
     *  0.10005414337462633D-20,  -.12749835038304435D-20/
        data pp707/
     1  -.45861226453281331D-01,  -.61640446373077787D-02,
     2  0.13133485604449612D-03,  0.10986161907478949D-03,
     3  0.22948713735290482D-04,  0.43284998071976826D-05,
     4  0.91175474095241347D-06,  0.22216021629512603D-06,
     5  0.60598835032853996D-07,  0.17870077582375069D-07,
     6  0.55718665138639406D-08,  0.18122464693416647D-08,
     7  0.60944720730679833D-09,  0.21060032705619680D-09,
     8  0.74438205696463779D-10,  0.26818138543863806D-10,
     9  0.98213206782373598D-11,  0.36481451884440703D-11,
     *  0.13720387380505735D-11,  0.52169875986358254D-12/
        data pq707/
     1  0.20031114167651246D-12,  0.77585078513307810D-13,
     2  0.30287368464518819D-13,  0.11907796723818278D-13,
     3  0.47120308701111726D-14,  0.18756354704590260D-14,
     4  0.75065301465724167D-15,  0.30192111037206655D-15,
     5  0.12199546218606087D-15,  0.49504709040654229D-16,
     6  0.20167955161134464D-16,  0.82468879993035549D-17,
     7  0.33837459161617971D-17,  0.13929857062324858D-17,
     8  0.57494790743313687D-18,  0.23822646778990630D-18,
     9  0.98570784033042348D-19,  0.41263810175344679D-19,
     *  0.17029928687987917D-19,  0.73239025372024243D-20,
     *  0.30289354028785025D-20,  0.14142632304579067D-20,
     *  0.38808297253125005D-21,  0.35772293399942016D-21,
     *  -.22546376843250029D-21,  0.17904576441241177D-21,
     *  -.13765915231572930D-21,  0.17569160455463093D-21/
c
        data pp801/
     1  0.36840620754029158E+01,  0.16563134726035632E+01,
     2  0.42263600891791207E+00,  0.12665770632437332E+00,
     3  0.42634223111999494E-01,  0.15396601869500660E-01,
     4  0.58070877732947384E-02,  0.22550111530179675E-02,
     5  0.89425899566564357E-03,  0.36032659020703848E-03,
     6  0.14701653736103232E-03,  0.60593386348952378E-04,
     7  0.25182772293165479E-04,  0.10539456975309780E-04,
     8  0.44372830122358308E-05,  0.18777694202633061E-05,
     9  0.79818830912765565E-06,  0.34062053259687671E-06,
     *  0.14586247999439712E-06,  0.62655667151986918E-07/
        data pq801/
     1  0.26988764737273656E-07,  0.11654501232562325E-07,
     2  0.50441854651822044E-08,  0.21876973837090048E-08,
     3  0.95061727966556276E-09,  0.41378926554697960E-09,
     4  0.18040497712814919E-09,  0.78770033313267843E-10,
     5  0.34440539353809635E-10,  0.15077640143774193E-10,
     6  0.66086644117890345E-11,  0.28998597817150769E-11,
     7  0.12737746271946138E-11,  0.56005751143141038E-12,
     8  0.24647437632888691E-12,  0.10856424455799136E-12,
     9  0.47858252688758159E-13,  0.21113570139680068E-13,
     *  0.93214757011539447E-14,  0.41181909635547831E-14,
     *  0.18206014916570820E-14,  0.80535380874371604E-15,
     *  0.35646899178793141E-15,  0.15782541642469812E-15,
     *  0.69875537864541793E-16,  0.30772158450031487E-16,
     *  0.13231825857638115E-16,  0.48889634950777713E-17/
        data pp802/
     1  0.22771471997288973E+01,  0.29123156211903325E+00,
     2  -.92105936046008722E-02,  -.52779498531020190E-02,
     3  -.99457782931629746E-03,  -.17846857927154692E-03,
     4  -.37449337515786785E-04,  -.92091445077537333E-05,
     5  -.25270687886573579E-05,  -.74728182496803530E-06,
     6  -.23332864895434128E-06,  -.75952047359049209E-07,
     7  -.25555471399594575E-07,  -.88340377166580504E-08,
     8  -.31232302294203906E-08,  -.11254233316960393E-08,
     9  -.41220852414317654E-09,  -.15313181134346598E-09,
     *  -.57596506068796396E-10,  -.21901750069322310E-10/
        data pq802/
     1  -.84098438595188223E-11,  -.32574718246258500E-11,
     2  -.12716874478226389E-11,  -.49999315480202837E-12,
     3  -.19785749565015744E-12,  -.78759487675743309E-13,
     4  -.31521171310430637E-13,  -.12678392406817553E-13,
     5  -.51229579835167205E-14,  -.20788817959833388E-14,
     6  -.84693407917695445E-15,  -.34632526895462184E-15,
     7  -.14210028564482765E-15,  -.58499876685453000E-16,
     8  -.24144274821736822E-16,  -.10005497440422126E-16,
     9  -.41374015854117692E-17,  -.17339264762487820E-17,
     *  -.71413246780175997E-18,  -.30872164047025025E-18,
     *  -.12733264896512798E-18,  -.60572786341192450E-19,
     *  -.15158024378265574E-19,  -.16118855574638621E-19,
     *  0.11963446993190003E-19,  -.86623622431992612E-20,
     *  0.70080687823264447E-20,  -.86960973959824436E-20/
        data pp803/
     1  -.30319536149562436E+01,  -.40048068332467105E+00,
     2  0.96590376328966425E-02,  0.71668064915489078E-02,
     3  0.14584111238037300E-02,  0.27166261119639099E-03,
     4  0.57122893847893811E-04,  0.13945594827217620E-04,
     5  0.38099444369746291E-05,  0.11244282057835577E-05,
     6  0.35073844711569816E-06,  0.11410386509122248E-06,
     7  0.38377990340179052E-07,  0.13263178283653803E-07,
     8  0.46882895980923791E-08,  0.16891535086347979E-08,
     9  0.61862425125401747E-09,  0.22979573127689114E-09,
     *  0.86426375508260116E-10,  0.32863046184982529E-10/
        data pq803/
     1  0.12618266621990937E-10,  0.48874038107086520E-11,
     2  0.19079460257455615E-11,  0.75013558489259514E-12,
     3  0.29683815466994115E-12,  0.11815791317041676E-12,
     4  0.47288550417954745E-13,  0.19020080256462233E-13,
     5  0.76853616975019305E-14,  0.31186658711373515E-14,
     6  0.12705314909326960E-14,  0.51953571829493702E-15,
     7  0.21316893151804787E-15,  0.87755805374159291E-16,
     8  0.36220257207265463E-16,  0.15008254329226109E-16,
     9  0.62089037684523523E-17,  0.25999655214762797E-17,
     *  0.10724273079070844E-17,  0.46186181735059143E-18,
     *  0.19087294979296926E-18,  0.89578874534810037E-19,
     *  0.23976882748812967E-19,  0.22983120335678097E-19,
     *  -.15225416272480907E-19,  0.11755068854610439E-19,
     *  -.91748300202585293E-20,  0.11607003131681503E-19/
        data pp804/
     1  0.32330071326417418E+01,  0.43344164425183668E+00,
     2  -.94220907496628840E-02,  -.77292641637498045E-02,
     3  -.16081629905233061E-02,  -.30280152499722121E-03,
     4  -.63773308489093783E-04,  -.15544032728457455E-04,
     5  -.42408691882988472E-05,  -.12507199670636919E-05,
     6  -.38999153880387764E-06,  -.12684800472334170E-06,
     7  -.42658942166317731E-07,  -.14741382682916521E-07,
     8  -.52104923887479347E-08,  -.18772158581250135E-08,
     9  -.68747597956356390E-09,  -.25536498019712857E-09,
     *  -.96041039615767081E-10,  -.36518362024453138E-10/
        data pq804/
     1  -.14021594749006242E-10,  -.54308922294730449E-11,
     2  -.21200938479554531E-11,  -.83353805526191356E-12,
     3  -.32983941480150754E-12,  -.13129349465946837E-12,
     4  -.52545351612392960E-13,  -.21134345925206629E-13,
     5  -.85396334288798170E-14,  -.34653114779449546E-14,
     6  -.14117499511700444E-14,  -.57727964320159022E-15,
     7  -.23686122949269994E-15,  -.97508685100862070E-16,
     8  -.40246140847696828E-16,  -.16675849671579629E-16,
     9  -.68997838283752472E-17,  -.28885181252095789E-17,
     *  -.11920280782853892E-17,  -.51274560029516553E-18,
     *  -.21204418627000585E-18,  -.99076696956978268E-19,
     *  -.27083427933566358E-19,  -.25104258331149317E-19,
     *  0.15950416122777936E-19,  -.12616516137015641E-19,
     *  0.97148290016950091E-20,  -.12385378485480953E-19/
        data pp805/
     1  -.23758012647499290E+01,  -.32102637254031673E+00,
     2  0.66102893681925329E-02,  0.57145322064570291E-02,
     3  0.12015048878068877E-02,  0.22740221549820501E-03,
     4  0.47938149251599650E-04,  0.11676277380990784E-04,
     5  0.31835416373570132E-05,  0.93855543386684155E-06,
     6  0.29260040279637681E-06,  0.95160858697853802E-07,
     7  0.32000502117946201E-07,  0.11057735338229186E-07,
     8  0.39083508091316961E-08,  0.14080540782911459E-08,
     9  0.51565042109569466E-09,  0.19153739388378880E-09,
     *  0.72035183131000403E-10,  0.27390222301067910E-10/
        data pq805/
     1  0.10516683268256052E-10,  0.40733355828524066E-11,
     2  0.15901280921691693E-11,  0.62517382587735303E-12,
     3  0.24738677954289865E-12,  0.98472718426525729E-13,
     4  0.39409958127483003E-13,  0.15851105812053463E-13,
     5  0.64048535844664900E-14,  0.25990313128726947E-14,
     6  0.10588305712205504E-14,  0.43296638994094714E-15,
     7  0.17764849568998067E-15,  0.73132357038028083E-16,
     8  0.30185129065701603E-16,  0.12506895446337029E-16,
     9  0.51752007868029554E-17,  0.21662685345827662E-17,
     *  0.89418193199743274E-18,  0.38440264879037863E-18,
     *  0.15902175034326055E-18,  0.74143470128506041E-19,
     *  0.20471466678023287E-19,  0.18670728348324578E-19,
     *  -.11614758616514156E-19,  0.93025406352228838E-20,
     *  -.71126496683858719E-20,  0.91047344566204648E-20/
        data pp806/
     1  0.11266505028805890E+01,  0.15296520598330866E+00,
     2  -.30468418901161284E-02,  -.27199887633424081E-02,
     3  -.57539165243358518E-03,  -.10923189736859582E-03,
     4  -.23040651159251384E-04,  -.56098503108133371E-05,
     5  -.15289375473699314E-05,  -.45065612165319680E-06,
     6  -.14047908375919961E-06,  -.45684391157625818E-07,
     7  -.15362057498715674E-07,  -.53082027196740953E-08,
     8  -.18761472004159971E-08,  -.67590693765679313E-09,
     9  -.24752471934225059E-09,  -.91941884711237349E-10,
     *  -.34578156604912957E-10,  -.13147724673727685E-10/
        data pq806/
     1  -.50481483254973721E-11,  -.19552490182428268E-11,
     2  -.76327810731112443E-12,  -.30008927947753979E-12,
     3  -.11874773345107440E-12,  -.47267653021508470E-13,
     4  -.18917051929462117E-13,  -.76086305574746300E-14,
     5  -.30743667348867942E-14,  -.12475487710815942E-14,
     6  -.50824388849603293E-15,  -.20782578600043161E-15,
     7  -.85272019051309995E-16,  -.35103775070171755E-16,
     8  -.14489011600650886E-16,  -.60033126895932292E-17,
     9  -.24841983421595889E-17,  -.10397750944575417E-17,
     *  -.42925286319884731E-18,  -.18446935204214729E-18,
     *  -.76327859866231693E-19,  -.35542925676998528E-19,
     *  -.98705366443651159E-20,  -.89173312106984063E-20,
     *  0.54776968020741452E-20,  -.44207255357075212E-20,
     *  0.33652790245655359E-20,  -.43186220216592228E-20/
        data pp807/
     1  -.31046041242212250E+00,  -.42286094549034596E-01,
     2  0.82359693453352742E-03,  0.75137838565937239E-03,
     3  0.15958452621802855E-03,  0.30355931115977331E-04,
     4  0.64057080701027670E-05,  0.15592590428302551E-05,
     5  0.42486053998092918E-06,  0.12521006338862808E-06,
     6  0.39027705229711333E-07,  0.12691440528590734E-07,
     7  0.42675749759339440E-08,  0.14745915258771846E-08,
     8  0.52117772174955199E-09,  0.18775951961718655E-09,
     9  0.68759185360359700E-10,  0.25540141467259753E-10,
     *  0.96052785061545023E-11,  0.36522231610724146E-11/
        data pq807/
     1  0.14022894205179466E-11,  0.54313360619560638E-12,
     2  0.21202477524834586E-12,  0.83359215367794641E-13,
     3  0.32985866605864406E-13,  0.13130042181832886E-13,
     4  0.52547870267131603E-14,  0.21135269653182741E-14,
     5  0.85399761454915008E-15,  0.34654387012103935E-15,
     6  0.14117982340151926E-15,  0.57729740548082364E-16,
     7  0.23686809162543583E-16,  0.97510938399907954E-17,
     8  0.40247530110838694E-17,  0.16675874039911048E-17,
     9  0.69007362128596307E-18,  0.28882022609880519E-18,
     *  0.11924526944363783E-18,  0.51233496965679854E-19,
     *  0.21201851546459251E-19,  0.98647010790711435E-20,
     *  0.27497730666859758E-20,  0.24688248620580179E-20,
     *  -.15039349394219053E-20,  0.12199792283222202E-20,
     *  -.92590501421352020E-21,  0.11902377767922934E-20/
        data pp808/
     1  0.37799883709834453E-01,  0.51602928445324025E-02,
     2  -.98894582641290185E-04,  -.91645252369226128E-04,
     3  -.19519401783248296E-04,  -.37182044726828069E-05,
     4  -.78485047559290764E-06,  -.19101411807644701E-06,
     5  -.52037386421153597E-07,  -.15334283686607573E-07,
     6  -.47794055308353418E-08,  -.15541705798927241E-08,
     7  -.52258968123244938E-09,  -.18057017126014111E-09,
     8  -.63819930864726888E-10,  -.22991625867617534E-10,
     9  -.84196949545889058E-11,  -.31274280398308118E-11,
     *  -.11761771097291327E-11,  -.44721777160126172E-12/
        data pq808/
     1  -.17171118269163345E-12,  -.66506932526042651E-13,
     2  -.25962486683403862E-13,  -.10207345520568055E-13,
     3  -.40391193908218916E-14,  -.16077723873391208E-14,
     4  -.64344771600798926E-15,  -.25880083642089017E-15,
     5  -.10457173605186667E-15,  -.42434165868624860E-16,
     6  -.17287409748629252E-16,  -.70689789355440306E-17,
     7  -.29004376066259590E-17,  -.11940154476773511E-17,
     8  -.49282930052196494E-18,  -.20419441945573480E-18,
     9  -.84500405320314116E-19,  -.35365202277655649E-19,
     *  -.14602191536800433E-19,  -.62727992049178960E-20,
     *  -.25961308774375984E-20,  -.12072054078478153E-20,
     *  -.33738429948718082E-21,  -.30158598953200754E-21,
     *  0.18263873923685597E-21,  -.14870477762457122E-21,
     *  0.11260655339728702E-21,  -.14493516709847147E-21/
c
c        determine the parameter in the chebychev approximation
c
        done=1
        ac=20
        bc=1000
        ac=done/dsqrt(ac)
        bc=done/dsqrt(bc)
c
        u=2/(bc-ac)
        v=done-u*bc
c
        tt=done/dsqrt(t)
        x=u*tt+v
c
c        one after another, calculate the values of
c        the correction weights at the user-specified  t
c
        n=47
c
c        . . . if m=1
c
        if(m .ne. 1) goto 2100
        call chexev(pp101,x,whts(1),n-1)
        return
 2100 continue
c
        if(m .ne. 2) goto 2200
        call chexev(pp201,x,whts(1),n-1)
        call chexev(pp202,x,whts(2),n-1)
        return
 2200 continue
c
        if(m .ne. 3) goto 2300
        call chexev(pp301,x,whts(1),n-1)
        call chexev(pp302,x,whts(2),n-1)
        call chexev(pp303,x,whts(3),n-1)
        return
 2300 continue
c
        if(m .ne. 4) goto 2400
        call chexev(pp401,x,whts(1),n-1)
        call chexev(pp402,x,whts(2),n-1)
        call chexev(pp403,x,whts(3),n-1)
        call chexev(pp404,x,whts(4),n-1)
        return
 2400 continue
c
        if(m .ne. 5) goto 2500
        call chexev(pp501,x,whts(1),n-1)
        call chexev(pp502,x,whts(2),n-1)
        call chexev(pp503,x,whts(3),n-1)
        call chexev(pp504,x,whts(4),n-1)
        call chexev(pp505,x,whts(5),n-1)
        return
 2500 continue
c
        if(m .ne. 6) goto 2600
        call chexev(pp601,x,whts(1),n-1)
        call chexev(pp602,x,whts(2),n-1)
        call chexev(pp603,x,whts(3),n-1)
        call chexev(pp604,x,whts(4),n-1)
        call chexev(pp605,x,whts(5),n-1)
        call chexev(pp606,x,whts(6),n-1)
        return
 2600 continue
c
        if(m .ne. 7) goto 2700
        call chexev(pp701,x,whts(1),n-1)
        call chexev(pp702,x,whts(2),n-1)
        call chexev(pp703,x,whts(3),n-1)
        call chexev(pp704,x,whts(4),n-1)
        call chexev(pp705,x,whts(5),n-1)
        call chexev(pp706,x,whts(6),n-1)
        call chexev(pp707,x,whts(7),n-1)
        return
 2700 continue
c
        if(m .ne. 8) goto 2800
        call chexev(pp801,x,whts(1),n-1)
        call chexev(pp802,x,whts(2),n-1)
        call chexev(pp803,x,whts(3),n-1)
        call chexev(pp804,x,whts(4),n-1)
        call chexev(pp805,x,whts(5),n-1)
        call chexev(pp806,x,whts(6),n-1)
        call chexev(pp807,x,whts(7),n-1)
        call chexev(pp808,x,whts(8),n-1)
        return
 2800 continue
        return
 3000 continue
cc
        return
        end         
c
c
c
c
c
        subroutine corwhtsg4(t,m,whts)
        implicit real *8 (a-h,o-z)
        dimension whts(*)
c
        dimension pp101(20),pp201(20),pp202(20),
     1    pp301(20),pp302(20),pp303(20),
     2    pp401(20),pp402(20),pp403(20),pp404(20),
     3    pp501(20),pp502(20),pp503(20),pp504(20),pp505(20),
     4    pp601(20),pp602(20),pp603(20),pp604(20),pp605(20),
     5    pp606(20),
     6    pp701(20),pp702(20),pp703(20),pp704(20),pp705(20),
     7    pp706(20),pp707(20),
     8    pp801(20),pp802(20),pp803(20),pp804(20),pp805(20),
     9    pp806(20),pp807(20),pp808(20)
c
        dimension buf101(2),buf201(2),buf202(2),
     1      buf301(2),buf302(2),buf303(2),
     2      buf401(2),buf402(2),buf403(2),buf404(2),
     3      buf501(2),buf502(2),buf503(2),buf504(2),buf505(2),
     4      buf601(2),buf602(2),buf603(2),buf604(2),buf605(2),
     5      buf606(2),
     6      buf701(2),buf702(2),buf703(2),buf704(2),buf705(2),
     7      buf706(2),buf707(2),
     8      buf801(2),buf802(2),buf803(2),buf804(2),buf805(2),
     9      buf806(2),buf807(2),buf808(2)
c
        equivalence (pp101(20),buf101(1)),(pq101(1),buf101(2)),
c
     1      (pp201(20),buf201(1)),(pq201(1),buf201(2)),
     2      (pp202(20),buf202(1)),(pq202(1),buf202(2)),
c 
     3      (pp301(20),buf301(1)),(pq301(1),buf301(2)),
     4      (pp302(20),buf302(1)),(pq302(1),buf302(2)),
     5      (pp303(20),buf303(1)),(pq303(1),buf303(2)),
c 
     6      (pp401(20),buf401(1)),(pq401(1),buf401(2)),
     7      (pp402(20),buf402(1)),(pq402(1),buf402(2)),
     8      (pp403(20),buf403(1)),(pq403(1),buf403(2)),
     9      (pp404(20),buf404(1)),(pq404(1),buf404(2)),
c 
     a      (pp501(20),buf501(1)),(pq501(1),buf501(2)),
     b      (pp502(20),buf502(1)),(pq502(1),buf502(2)),
     c      (pp503(20),buf503(1)),(pq503(1),buf503(2)),
     d      (pp504(20),buf504(1)),(pq504(1),buf504(2)),
     e      (pp505(20),buf505(1)),(pq505(1),buf505(2))
c 
        equivalence
     1      (pp601(20),buf601(1)),(pq601(1),buf601(2)),
     2      (pp602(20),buf602(1)),(pq602(1),buf602(2)),
     3      (pp603(20),buf603(1)),(pq603(1),buf603(2)),
     4      (pp604(20),buf604(1)),(pq604(1),buf604(2)),
     5      (pp605(20),buf605(1)),(pq605(1),buf605(2)),
     6      (pp606(20),buf606(1)),(pq606(1),buf606(2))
        equivalence
     1      (pp701(20),buf701(1)),(pq701(1),buf701(2)),
     2      (pp702(20),buf702(1)),(pq702(1),buf702(2)),
     3      (pp703(20),buf703(1)),(pq703(1),buf703(2)),
     4      (pp704(20),buf704(1)),(pq704(1),buf704(2)),
     5      (pp705(20),buf705(1)),(pq705(1),buf705(2)),
     6      (pp706(20),buf706(1)),(pq706(1),buf706(2)),
     7      (pp707(20),buf707(1)),(pq707(1),buf707(2))
c
        equivalence
     1      (pp801(20),buf801(1)),(pq801(1),buf801(2)),
     2      (pp802(20),buf802(1)),(pq802(1),buf802(2)),
     3      (pp803(20),buf803(1)),(pq803(1),buf803(2)),
     4      (pp804(20),buf804(1)),(pq804(1),buf804(2)),
     5      (pp805(20),buf805(1)),(pq805(1),buf805(2)),
     6      (pp806(20),buf806(1)),(pq806(1),buf806(2)),
     7      (pp807(20),buf807(1)),(pq807(1),buf807(2)),
     8      (pp808(20),buf808(1)),(pq808(1),buf808(2))
c
        dimension pq101(28),pq201(28),pq202(28),
     1    pq301(28),pq302(28),pq303(28),
     2    pq401(28),pq402(28),pq403(28),pq404(28),
     3    pq501(28),pq502(28),pq503(28),pq504(28),pq505(28),
     4    pq601(28),pq602(28),pq603(28),pq604(28),pq605(28),
     5    pq606(28),
     6    pq701(28),pq702(28),pq703(28),pq704(28),pq705(28),
     7    pq706(28),pq707(28),
     8    pq801(28),pq802(28),pq803(28),pq804(28),pq805(28),
     9    pq806(28),pq807(28),pq808(28)
c

        data pp101/
     1  0.85200640511258197E+01,  0.18128934781949340E+01,
     2  0.41133907867850104E+00,  0.12429522790898998E+00,
     3  0.42268856759057145E-01,  0.15332596665105244E-01,
     4  0.57934813140718179E-02,  0.22516348703312825E-02,
     5  0.89332844691481705E-03,  0.36005080499795489E-03,
     6  0.14693032547012520E-03,  0.60565303851907838E-04,
     7  0.25173319297848159E-04,  0.10536188287314588E-04,
     8  0.44361271406269251E-05,  0.18773528496161486E-05,
     9  0.79803571406730643E-06,  0.34056383975920282E-06,
     *  0.14584115495215416E-06,  0.62647557589084577E-07/
        data pq101/
     1  0.26985650678489181E-07,  0.11653294985546664E-07,
     2  0.50437145421964478E-08,  0.21875122244891916E-08,
     3  0.95054400670297576E-09,  0.41376009780188892E-09,
     4  0.18039330341232478E-09,  0.78765337845345990E-10,
     5  0.34438642059782894E-10,  0.15076870197171772E-10,
     6  0.66083507500424432E-11,  0.28997315020668166E-11,
     7  0.12737219973629290E-11,  0.56003583223399982E-12,
     8  0.24646544652815669E-12,  0.10856052616479635E-12,
     9  0.47856746410052190E-13,  0.21112915002930871E-13,
     *  0.93212246289251246E-14,  0.41180631410441435E-14,
     *  0.18205542694864050E-14,  0.80531747302717925E-15,
     *  0.35647774147777537E-15,  0.15780525083634541E-15,
     *  0.69909780681962549E-16,  0.30754183826644350E-16,
     *  0.13249815344267284E-16,  0.48700996697540175E-17/
c
        data pp201/
     1  0.80227813880270347E+01,  0.18099542733083373E+01,
     2  0.41173139064594468E+00,  0.12432022499709302E+00,
     3  0.42271531944041486E-01,  0.15333058512266331E-01,
     4  0.57935825966737269E-02,  0.22516604564318327E-02,
     5  0.89333555459364293E-03,  0.36005291920046017E-03,
     6  0.14693098766133494E-03,  0.60565519800938603E-04,
     7  0.25173392043371303E-04,  0.10536213454181350E-04,
     8  0.44361360433097041E-05,  0.18773560589467027E-05,
     9  0.79803688991981104E-06,  0.34056427668490169E-06,
     *  0.14584131932201546E-06,  0.62647620102353348E-07/
        data pq201/
     1  0.26985674685389938E-07,  0.11653304285358856E-07,
     2  0.50437181730723482E-08,  0.21875136521579950E-08,
     3  0.95054457169530003E-09,  0.41376032271710156E-09,
     4  0.18039339343019408E-09,  0.78765374055639584E-10,
     5  0.34438656688298119E-10,  0.15076876136211242E-10,
     6  0.66083531675849981E-11,  0.28997324927760811E-11,
     7  0.12737224032584110E-11,  0.56003600087612856E-12,
     8  0.24646551393535163E-12,  0.10856055632303944E-12,
     9  0.47856755014026422E-13,  0.21112921579204460E-13,
     *  0.93212250177159923E-14,  0.41180657035379964E-14,
     *  0.18205546418288560E-14,  0.80531938198969667E-15,
     *  0.35647602285508611E-15,  0.15780708615406650E-15,
     *  0.69906085226342249E-16,  0.30756065130037088E-16,
     *  0.13247898504345955E-16,  0.48720636780806237E-17/
        data pp202/
     1  0.49728266309878499E+00,  0.29392048865967399E-02,
     2  -.39231196744364021E-03,  -.24997088103033367E-04,
     3  -.26751849843416468E-05,  -.46184716108667325E-06,
     4  -.10128260190896124E-06,  -.25586100550194213E-07,
     5  -.71076788258754048E-08,  -.21142025052712850E-08,
     6  -.66219120973485706E-09,  -.21594903076521702E-09,
     7  -.72745523143983790E-10,  -.25166866762059362E-10,
     8  -.89026827789760152E-11,  -.32093305540832570E-11,
     9  -.11758525046070328E-11,  -.43692569886516021E-12,
     *  -.16436986129430914E-12,  -.62513268770818983E-13/
        data pq202/
     1  -.24006900757095762E-13,  -.92998121923700542E-14,
     2  -.36308759004476796E-14,  -.14276688034304965E-14,
     3  -.56499232426898096E-15,  -.22491521264502140E-15,
     4  -.90017869292555564E-16,  -.36210293593931122E-16,
     5  -.14628515224702161E-16,  -.59390394700230255E-17,
     6  -.24175425549161529E-17,  -.99070926449185623E-18,
     7  -.40589548202455656E-18,  -.16864212874365108E-18,
     8  -.67407194945131279E-19,  -.30158243093887359E-19,
     9  -.86039742321677494E-20,  -.65762735895865378E-20,
     *  -.38879086769307012E-21,  -.25624938529147205E-20,
     *  -.37234245106298976E-21,  -.19089625174170485E-20,
     *  0.17186226892630747E-20,  -.18353177210878285E-20,
     *  0.36954556203001056E-20,  -.18813033927385265E-20,
     *  0.19168399213286199E-20,  -.19640083266062026E-20/


        data pp301/
     1  0.78157036272228566E+01,  0.18086051350837438E+01,
     2  0.41190533219654291E+00,  0.12433256287565279E+00,
     3  0.42272878730204465E-01,  0.15333289934412082E-01,
     4  0.57936332728740556E-02,  0.22516732536157675E-02,
     5  0.89333910909323370E-03,  0.36005397642814269E-03,
     6  0.14693131878441372E-03,  0.60565627782010252E-04,
     7  0.25173428417814803E-04,  0.10536226038071808E-04,
     8  0.44361404947812901E-05,  0.18773576636505455E-05,
     9  0.79803747785787112E-06,  0.34056449515147005E-06,
     *  0.14584140150814667E-06,  0.62647651359383503E-07/
        data pq301/
     1  0.26985686688973592E-07,  0.11653308935310308E-07,
     2  0.50437199885261902E-08,  0.21875143659978266E-08,
     3  0.95054485419343654E-09,  0.41376043517529899E-09,
     4  0.18039343843950789E-09,  0.78765392160759513E-10,
     5  0.34438664002837627E-10,  0.15076879105619736E-10,
     6  0.66083543764871282E-11,  0.28997329880049797E-11,
     7  0.12737226062064174E-11,  0.56003608506668019E-12,
     8  0.24646554777136275E-12,  0.10856057126828917E-12,
     9  0.47856759588494775E-13,  0.21112924729565155E-13,
     *  0.93212253521758251E-14,  0.41180668423336427E-14,
     *  0.18205548275029415E-14,  0.80532018932498301E-15,
     *  0.35647531300025424E-15,  0.15780785192921564E-15,
     *  0.69904547817515924E-16,  0.30756848189458657E-16,
     *  0.13247100980322188E-16,  0.48728813122369372E-17/
        data pp302/
     1  0.91143818470714107E+00,  0.56374813357836382E-02,
     2  -.74019506864009573E-03,  -.49672845222587112E-04,
     3  -.53687573102999884E-05,  -.92469145259028010E-06,
     4  -.20263500256649334E-06,  -.51180468419744766E-07,
     5  -.14216678007415820E-07,  -.42286578703284949E-08,
     6  -.13244373673049916E-08,  -.43191117406314716E-09,
     7  -.14549441014331391E-09,  -.50334647677357973E-10,
     8  -.17805625951029553E-10,  -.64187382396674331E-11,
     9  -.23517286247804454E-11,  -.87385883559313417E-12,
     *  -.32874212371093999E-12,  -.12502732908017495E-12/
        data pq302/
     1  -.48014068065006360E-13,  -.18599715094789598E-13,
     2  -.72617835845096648E-14,  -.28553484666335295E-14,
     3  -.11299885972878622E-14,  -.44983160749795906E-15,
     4  -.18003649691129279E-15,  -.72420533452602971E-16,
     5  -.29257594240874023E-16,  -.11877856459365287E-16,
     6  -.48353468150365615E-17,  -.19811670618267147E-17,
     7  -.81179149472083445E-18,  -.33702323200003476E-18,
     8  -.13507921717071829E-18,  -.60048742558848981E-19,
     9  -.17752910937900969E-19,  -.12876994978590192E-19,
     *  -.10577105333817832E-20,  -.48400851455478007E-20,
     *  -.74369062193390640E-21,  -.35236330900883745E-20,
     *  0.31383323529942605E-20,  -.33668680193679842E-20,
     *  0.67702732729499645E-20,  -.34474222358758439E-20,
     *  0.35118879688650719E-20,  -.35992766392333616E-20/
        data pp303/
     1  -.20707776080417804E+00,  -.13491382245934491E-02,
     2  0.17394155059822776E-03,  0.12337878559776873E-04,
     3  0.13467861629791708E-05,  0.23142214575180342E-06,
     4  0.50676200328766052E-07,  0.12797183934775276E-07,
     5  0.35544995907702077E-08,  0.10572276825286050E-08,
     6  0.33112307878506729E-09,  0.10798107164896507E-09,
     7  0.36374443499665061E-10,  0.12583890457649305E-10,
     8  0.44514715860267687E-11,  0.16047038427920881E-11,
     9  0.58793806008670627E-12,  0.21846656836398698E-12,
     *  0.82186131208315424E-13,  0.31257030154677983E-13/
        data pq303/
     1  0.12003583653955299E-13,  0.46499514512097719E-14,
     2  0.18154538420309926E-14,  0.71383983160151654E-15,
     3  0.28249813650944060E-15,  0.11245819742646883E-15,
     4  0.45009313809368612E-16,  0.18105119929335925E-16,
     5  0.73145395080859317E-17,  0.29694084946711302E-17,
     6  0.12089021300602040E-17,  0.49522889866742918E-18,
     7  0.20294800634813889E-18,  0.84190551628191871E-19,
     8  0.33836011112793271E-19,  0.14945249732480799E-19,
     9  0.45744683528669201E-20,  0.31503606945020266E-20,
     *  0.33445983284429784E-21,  0.11387956463166781E-20,
     *  0.18567408543533720E-21,  0.80733528633569045E-21,
     *  -.70985483186548702E-21,  0.76577514914002822E-21,
     *  -.15374088263250936E-20,  0.78305942156860474E-21,
     *  -.79752402376854759E-21,  0.81763415631366313E-21/
c
        data pp401/
     1  0.76915120770614232E+01,  0.18077391146141059E+01,
     2  0.41201514412769146E+00,  0.12434074001670171E+00,
     3  0.42273779165900624E-01,  0.15333444376864426E-01,
     4  0.57936670688104196E-02,  0.22516817864584837E-02,
     5  0.89334147898015471E-03,  0.36005468128878274E-03,
     6  0.14693153954229610E-03,  0.60565699771577575E-04,
     7  0.25173452668004588E-04,  0.10536234427484500E-04,
     8  0.44361434624724191E-05,  0.18773587334659641E-05,
     9  0.79803786982051412E-06,  0.34056464079708870E-06,
     *  0.14584145629930102E-06,  0.62647672197535535E-07/
        data pq401/
     1  0.26985694691407122E-07,  0.11653312035293061E-07,
     2  0.50437211988340491E-08,  0.21875148418928577E-08,
     3  0.95054504252618571E-09,  0.41376051014762761E-09,
     4  0.18039346844584352E-09,  0.78765404230830460E-10,
     5  0.34438668879291323E-10,  0.15076881085188268E-10,
     6  0.66083551824655409E-11,  0.28997333181156358E-11,
     7  0.12737227415052016E-11,  0.56003614115017746E-12,
     8  0.24646557037288546E-12,  0.10856058118710938E-12,
     9  0.47856762729031570E-13,  0.21112926783823285E-13,
     *  0.93212256218836140E-14,  0.41180675540114721E-14,
     *  0.18205549511478792E-14,  0.80532067846278583E-15,
     *  0.35647488963238778E-15,  0.15780831175487792E-15,
     *  0.69903626391027259E-16,  0.30757317633207719E-16,
     *  0.13246622982921129E-16,  0.48733715707601815E-17/
        data pp402/
     1  0.12840128351914415E+01,  0.82355427446974565E-02,
     2  -.10696308620857518E-02,  -.74204268369323178E-04,
     3  -.80700643987746720E-05,  -.13880188096222976E-05,
     4  -.30402281165830352E-06,  -.76778996568269968E-07,
     5  -.21326338770449083E-07,  -.63432397904560175E-08,
     6  -.19867110144458520E-08,  -.64787987603137884E-09,
     7  -.21824497949751594E-09,  -.75502885754044230E-10,
     8  -.26708699337963907E-10,  -.96281844952584095E-11,
     9  -.35276165537836189E-11,  -.13107956915225369E-11,
     *  -.49311558676150859E-12,  -.18754178517891747E-12/
        data pq402/
     1  -.72021368654314161E-13,  -.27899663353813493E-13,
     2  -.10892707161016660E-13,  -.42830335597962769E-14,
     3  -.16949868448013636E-14,  -.67474859338098797E-15,
     4  -.27005550381906479E-15,  -.10863074629370287E-15,
     5  -.43886955327670216E-16,  -.17816562054265766E-16,
     6  -.72532820531524157E-17,  -.29714990300867709E-17,
     7  -.12176878475042034E-17,  -.50527372381409338E-18,
     8  -.20288378530065112E-18,  -.89805203177403364E-19,
     9  -.27174521324939250E-19,  -.19039769369500018E-19,
     *  -.18668339001948965E-20,  -.69751186337298100E-20,
     *  -.11146254352905220E-20,  -.49910464985628717E-20,
     *  0.44084359523755973E-20,  -.47463450062187777E-20,
     *  0.95345527389457865E-20,  -.48557534830606205E-20,
     *  0.49458801720416178E-20,  -.50700522089654104E-20/
        data pp403/
     1  -.57965241128847844E+00,  -.39471996335072674E-02,
     2  0.50337734404388379E-03,  0.36869301706512939E-04,
     3  0.40480932514538544E-05,  0.69474950278382093E-06,
     4  0.15206400942057623E-06,  0.38395712083300479E-07,
     5  0.10664160353803471E-07,  0.31718096026561275E-08,
     6  0.99339672592592764E-09,  0.32394977361719675E-09,
     7  0.10912501285386709E-09,  0.37752128534335563E-10,
     8  0.13354544972961123E-10,  0.48141500983830644E-11,
     9  0.17638259890898798E-11,  0.65540342429338972E-12,
     *  0.24655959425888402E-12,  0.93771486253420499E-13/
        data pq403/
     1  0.36010884243263101E-13,  0.13949899710233666E-13,
     2  0.54463774185379873E-14,  0.21415249247642639E-14,
     3  0.84749638402294207E-15,  0.33737518330949774E-15,
     4  0.13502832071714061E-15,  0.54315332770435821E-16,
     5  0.21943900594882124E-16,  0.89081140895716093E-17,
     6  0.36268373681760588E-17,  0.14855608669274852E-17,
     7  0.60884435913150741E-18,  0.25244104344225074E-18,
     8  0.10164057924272598E-18,  0.44701710351035119E-19,
     9  0.13996078739904139E-19,  0.93131350854109377E-20,
     *  0.11435831996564693E-20,  0.32738291344982330E-20,
     *  0.55660889879208633E-21,  0.22747486948099131E-20,
     *  -.19799584312468894E-20,  0.21452521359910559E-20,
     *  -.43016882923199749E-20,  0.21913906687534671E-20,
     *  -.22315162269453417E-20,  0.22884097260448306E-20/
        data pp404/
     1  0.12419155016143347E+00,  0.86602046963793942E-03,
     2  -.10981193114855201E-03,  -.81771410489120220E-05,
     3  -.90043569615822788E-06,  -.15444245234400583E-06,
     4  -.33795936363936725E-07,  -.85328427161750674E-08,
     5  -.23698869210110877E-08,  -.70486064004250752E-09,
     6  -.22075788238028678E-09,  -.71989567322743895E-10,
     7  -.24250189784734010E-10,  -.83894126922287524E-11,
     8  -.29676911289781181E-11,  -.10698154185303255E-11,
     9  -.39196264300105785E-12,  -.14564561864313425E-12,
     *  -.54791154350189532E-13,  -.20838152032914172E-13/
        data pq404/
     1  -.80024335297692672E-14,  -.30999827530079648E-14,
     2  -.12103078588356649E-14,  -.47589503105424911E-15,
     3  -.18833274917116715E-15,  -.74972328627676304E-16,
     4  -.30006335635924001E-16,  -.12070070947033298E-16,
     5  -.48764536955987310E-17,  -.19795685316334928E-17,
     6  -.80597841270528520E-18,  -.33011065608668529E-18,
     7  -.13529878426112285E-18,  -.56083497271353199E-19,
     8  -.22601522709977270E-19,  -.99188202061850801E-20,
     9  -.31405367956792193E-20,  -.20542581303030480E-20,
     *  -.26970778893754384E-21,  -.71167782939398145E-21,
     *  -.12364493778556844E-21,  -.48913780282450038E-21,
     *  0.42336786646078413E-21,  -.45982566228305432E-21,
     *  0.92142648866529334E-21,  -.46944374906107090E-21,
     *  0.47799740105930200E-21,  -.49025852324372839E-21/
c
        data pp501/
     1  0.76049527806479168E+01,  0.18071046119899313E+01,
     2  0.41209474346635321E+00,  0.12434684994714743E+00,
     3  0.42274455681234771E-01,  0.15333560288075803E-01,
     4  0.57936924216977989E-02,  0.22516881867859994E-02,
     5  0.89334325650572852E-03,  0.36005520995536585E-03,
     6  0.14693170511529079E-03,  0.60565753764846379E-04,
     7  0.25173470855927354E-04,  0.10536240719620220E-04,
     8  0.44361456882624698E-05,  0.18773595358339566E-05,
     9  0.79803816379446460E-06,  0.34056475003192257E-06,
     *  0.14584149739286689E-06,  0.62647687826215528E-07/
        data pq501/
     1  0.26985700693254481E-07,  0.11653314360287686E-07,
     2  0.50437221065675910E-08,  0.21875151988150369E-08,
     3  0.95054518377607668E-09,  0.41376056637697359E-09,
     4  0.18039349095065746E-09,  0.78765413283380177E-10,
     5  0.34438672536676555E-10,  0.15076882569847132E-10,
     6  0.66083557869701349E-11,  0.28997335656787165E-11,
     7  0.12737228429793528E-11,  0.56003618319211827E-12,
     8  0.24646558734502045E-12,  0.10856058860499498E-12,
     9  0.47856765127615731E-13,  0.21112928302668124E-13,
     *  0.93212258463693155E-14,  0.41180680651938141E-14,
     *  0.18205550438196046E-14,  0.80532102199574390E-15,
     *  0.35647459580032095E-15,  0.15780863253788355E-15,
     *  0.69902984498871476E-16,  0.30757644724705317E-16,
     *  0.13246289992217778E-16,  0.48737132148218357E-17/
        data pp502/
     1  0.16302500208454669E+01,  0.10773553241395733E-01,
     2  -.13880282167327440E-02,  -.98643990152226568E-04,
     3  -.10776125735365083E-04,  -.18516636551277994E-05,
     4  -.40543436117558952E-06,  -.10238030663111631E-06,
     5  -.28436441065672703E-07,  -.84579061228986918E-08,
     6  -.26490029932082295E-08,  -.86385295124945126E-09,
     7  -.29099667056419444E-09,  -.10067142863327032E-09,
     8  -.35611859540932378E-10,  -.12837656465610309E-10,
     9  -.47035123556889264E-11,  -.17477350270133545E-11,
     *  -.65748985025517421E-12,  -.25005650514777502E-12/
        data pq502/
     1  -.96028758092194354E-13,  -.37199641854973928E-13,
     2  -.14523641328794569E-13,  -.57107222768937607E-14,
     3  -.22599864086841905E-14,  -.89966597726825068E-15,
     4  -.36007475958089988E-15,  -.14484094516101763E-15,
     5  -.58516496259020643E-16,  -.23755197509839769E-16,
     6  -.96713004293087005E-17,  -.39617513526702006E-17,
     7  -.16235844520585029E-17,  -.67344148705143369E-18,
     8  -.27077232528470470E-18,  -.11947674559058654E-18,
     9  -.36768857968704955E-19,  -.25115148727089096E-19,
     *  -.27647767060428176E-20,  -.90198480017771204E-20,
     *  -.14853123367276528E-20,  -.63651783308519247E-20,
     *  0.55837642196966720E-20,  -.60294770287220724E-20,
     *  0.12102121362081549E-19,  -.61641194734552185E-20,
     *  0.62778429854393831E-20,  -.64366284555832073E-20/
        data pp503/
     1  -.10990081897695165E+01,  -.77542153785546829E-02,
     2  0.98097337601437217E-03,  0.73528884380868023E-04,
     3  0.81071852563394715E-05,  0.13902167710420736E-05,
     4  0.30418133369650524E-06,  0.76797677177569990E-07,
     5  0.21329313796638900E-07,  0.63438091013201390E-08,
     6  0.19868346940694938E-08,  0.64790938644430538E-09,
     7  0.21825254945388484E-09,  0.75504942853174698E-10,
     8  0.26709285277413829E-10,  0.96283580539109132E-11,
     9  0.35276696919478410E-11,  0.13108124275296160E-11,
     *  0.49312098949938246E-12,  0.18754356620670683E-12/
        data pq503/
     1  0.72021968400083390E-13,  0.27899867461974320E-13,
     2  0.10892778670204851E-13,  0.42830580004104896E-14,
     3  0.16949957298471824E-14,  0.67475125914039180E-15,
     4  0.27005720435989324E-15,  0.10863063107140795E-15,
     5  0.43888211991907759E-16,  0.17816067272932612E-16,
     6  0.72538649324104838E-17,  0.29709393508026347E-17,
     7  0.12176892659629613E-17,  0.50469268829826141E-18,
     8  0.20347338921880765E-18,  0.89209023970808566E-19,
     9  0.28387583705552957E-19,  0.18426204121797721E-19,
     *  0.24904974084266191E-20,  0.63409231865722786E-20,
     *  0.11126392509495196E-20,  0.43359464432484638E-20,
     *  -.37429508322299550E-20,  0.40699501697442864E-20,
     *  -.81530412270235570E-20,  0.41539396543480389E-20,
     *  -.42294604470459719E-20,  0.43382740959723840E-20/
        data pp504/
     1  0.47042873581545887E+00,  0.34040309663362164E-02,
     2  -.42820928579554426E-03,  -.32616862831815412E-04,
     3  -.36064970327486393E-05,  -.61808729784950762E-06,
     4  -.13520748588122273E-06,  -.34134152779021408E-07,
     5  -.94799892162347073E-08,  -.28195269724851819E-08,
     6  -.88304986114266425E-09,  -.28796264254081631E-09,
     7  -.97001880851412510E-10,  -.33557955571454843E-10,
     8  -.11870851331946589E-10,  -.42792873888822246E-11,
     9  -.15678584449063653E-11,  -.58258495413395179E-12,
     *  -.21916541784385516E-12,  -.83352872001771725E-13/
        data pq504/
     1  -.32009822967649460E-13,  -.12399961254168401E-13,
     2  -.48412420266135739E-14,  -.19035837481517329E-14,
     3  -.75333231305399404E-15,  -.29988971251493901E-15,
     4  -.12002559139775908E-15,  -.48280269814348051E-16,
     5  -.19505994626949152E-16,  -.79182039872074935E-17,
     6  -.32239967888615693E-17,  -.13203629786701213E-17,
     7  -.54119538881542396E-18,  -.22425126050869376E-18,
     8  -.90490062694032126E-19,  -.39590362619369502E-19,
     9  -.12734873439445455E-19,  -.81296374878942491E-20,
     *  -.11676505947868503E-20,  -.27564071974432740E-20,
     *  -.49433183922429357E-21,  -.18632696351181048E-20,
     *  0.15986961337813326E-20,  -.17429576847866632E-20,
     *  0.34889951118016826E-20,  -.17778097394570690E-20,
     *  0.18099602144589089E-20,  -.18568347698628332E-20/
        data pp505/
     1  -.86559296413506350E-01,  -.63450262417456925E-03,
     2  0.79599338661748063E-04,  0.61099304457258474E-05,
     3  0.67651533414760285E-06,  0.11591121137637545E-06,
     4  0.25352887379321501E-07,  0.64003275157115852E-08,
     5  0.17775255738059049E-08,  0.52866658311066858E-09,
     6  0.16557299469059437E-09,  0.53993268804518104E-10,
     7  0.18187922766669625E-10,  0.62921357198065225E-11,
     8  0.22257900507421176E-11,  0.80236799258797479E-12,
     9  0.29397395047632687E-12,  0.10923483387270439E-12,
     *  0.41093565873416406E-13,  0.15628679992214388E-13/
        data pq505/
     1  0.60018473594700482E-14,  0.23249946252901089E-14,
     2  0.90773354194447726E-15,  0.35692217927437096E-15,
     3  0.14124989097070672E-15,  0.56229345971815678E-16,
     4  0.22504813940458770E-16,  0.90525497168286874E-17,
     5  0.36573852328376052E-17,  0.14846588638935002E-17,
     6  0.60450459403907040E-18,  0.24756308064585925E-18,
     7  0.10147415113857605E-18,  0.42041940809335084E-19,
     8  0.16972134996014588E-19,  0.74178856032944276E-20,
     9  0.23985841609415576E-20,  0.15188448393992156E-20,
     *  0.22448570146075155E-21,  0.51118234201307851E-21,
     *  0.92671725359689793E-22,  0.34353295807504530E-21,
     *  -.29383206683095517E-21,  0.32078300562485780E-21,
     *  -.64189215578368788E-21,  0.32709149760022212E-21,
     *  -.33299070335080610E-21,  0.34164406165429146E-21/
c
        data pp601/
     1  0.75394481784968316E+01,  0.18066053751736127E+01,
     2  0.41215688695245979E+00,  0.12435172454214753E+00,
     3  0.42274997570652704E-01,  0.15333653064237994E-01,
     4  0.57937127075808097E-02,  0.22516933074661179E-02,
     5  0.89334467859246555E-03,  0.36005563290129843E-03,
     6  0.14693183757643681E-03,  0.60565796960117491E-04,
     7  0.25173485406433839E-04,  0.10536245753374519E-04,
     8  0.44361474689075335E-05,  0.18773601777322081E-05,
     9  0.79803839897480595E-06,  0.34056483742016161E-06,
     *  0.14584153026783966E-06,  0.62647700329199103E-07/
        data pq601/
     1  0.26985705494745695E-07,  0.11653316220287923E-07,
     2  0.50437228327560127E-08,  0.21875154843533245E-08,
     3  0.95054529677618692E-09,  0.41376061136051066E-09,
     4  0.18039350895454534E-09,  0.78765420525418462E-10,
     5  0.34438675462610485E-10,  0.15076883757564324E-10,
     6  0.66083562705856512E-11,  0.28997337637178717E-11,
     7  0.12737229241587138E-11,  0.56003621681391583E-12,
     8  0.24646560093466168E-12,  0.10856059452723439E-12,
     9  0.47856767071031014E-13,  0.21112929505323113E-13,
     *  0.93212260385801843E-14,  0.41180684613050367E-14,
     *  0.18205551179228888E-14,  0.80532128356516509E-15,
     *  0.35647437420517427E-15,  0.15780887547020995E-15,
     *  0.69902498940590474E-16,  0.30757892189903083E-16,
     *  0.13246038099780246E-16,  0.48739717212992469E-17/
        data pp602/
     1  0.19577730316008929E+01,  0.13269737322988687E-01,
     2  -.16987456472656416E-02,  -.12301696515271179E-03,
     3  -.13485572825030858E-04,  -.23155444660843501E-05,
     4  -.50686377622975850E-06,  -.12798370722362044E-06,
     5  -.35546874750817513E-07,  -.10572635785836658E-07,
     6  -.33113087233132441E-08,  -.10798293068068142E-08,
     7  -.36374920298655214E-09,  -.12584020012854836E-09,
     8  -.44515084858983103E-10,  -.16047147722745499E-10,
     9  -.58794140624453423E-11,  -.21846762222230749E-11,
     *  -.82186471409044928E-12,  -.31257142302544605E-12/
        data pq602/
     1  -.12003621416173174E-12,  -.46499643040905654E-13,
     2  -.18154583437204326E-13,  -.71384137149081186E-14,
     3  -.28249869598655137E-14,  -.11245836626447161E-14,
     4  -.45009419897695817E-15,  -.18105113658708254E-15,
     5  -.73146165907764517E-16,  -.29693783468439503E-16,
     6  -.12089378010775356E-16,  -.49519471287227751E-17,
     7  -.20294812573690843E-17,  -.84155047485338219E-18,
     8  -.33872053142917215E-18,  -.14908794260247349E-18,
     9  -.46485934380706771E-19,  -.31128423670623689E-19,
     *  -.37258310501491507E-20,  -.11000404114591773E-19,
     *  -.18558287579249868E-20,  -.76730254368841641E-20,
     *  0.66917399532448721E-20,  -.72441386607558116E-20,
     *  0.14529912767083446E-19,  -.74014454623124620E-20,
     *  0.75373051730457735E-20,  -.77291608425674506E-20/
        data pp603/
     1  -.17540542112803687E+01,  -.12746583541740590E-01,
     2  0.16024082370801674E-02,  0.12227483438183846E-03,
     3  0.13526079435671020E-04,  0.23179783929551750E-05,
     4  0.50704016380484319E-06,  0.12800447836257826E-06,
     5  0.35550181166928521E-07,  0.10573268427196072E-07,
     6  0.33114461542795232E-08,  0.10798620975590313E-08,
     7  0.36375761429860023E-09,  0.12584248584373078E-09,
     8  0.44515735913515280E-10,  0.16047340568181293E-10,
     9  0.58794731054606730E-11,  0.21846948179490569E-11,
     *  0.82187071716993259E-12,  0.31257340196204888E-12/
        data pq603/
     1  0.12003688053915816E-12,  0.46499869833837771E-13,
     2  0.18154662887024365E-13,  0.71384408764392055E-14,
     3  0.28249968322098287E-14,  0.11245866298933227E-14,
     4  0.45009608315200979E-15,  0.18105101392353775E-15,
     5  0.73147551289395533E-16,  0.29693239190132110E-16,
     6  0.12090020095343777E-16,  0.49513309029077251E-17,
     7  0.20294828765841494E-17,  0.84091066390213971E-18,
     8  0.33936980150776640E-18,  0.14843141799458187E-18,
     9  0.47821736529540382E-19,  0.30452754008880836E-19,
     *  0.44126060966328470E-20,  0.10302035412257480E-19,
     *  0.18536720933301674E-20,  0.69516406552942567E-20,
     *  -.59589022992522054E-20,  0.64992734337858293E-20,
     *  -.13008624037019012E-19,  0.66285916320538368E-20,
     *  -.67483848222677883E-20,  0.69233388700002639E-20/
        data pp604/
     1  0.11254747573263110E+01,  0.83963991295221237E-02,
     2  -.10496441468613395E-02,  -.81362812832785846E-04,
     3  -.90253912120801876E-05,  -.15458489197626090E-05,
     4  -.33806631598956068E-06,  -.85340953964029674E-07,
     5  -.23700856586524328E-07,  -.70489862983611147E-08,
     6  -.22076613213526936E-08,  -.71991535365554221E-09,
     7  -.24250694569612790E-09,  -.83895498562010920E-10,
     8  -.29677301968048040E-10,  -.10698269903152604E-10,
     9  -.39196618584191973E-11,  -.14564673445533926E-11,
     *  -.54791514551440529E-12,  -.20838270775711378E-12/
        data pq604/
     1  -.80024735106724229E-13,  -.30999963626031852E-13,
     2  -.12103126243433088E-13,  -.47589666241804488E-14,
     3  -.18833334154166404E-14,  -.74972508326786989E-15,
     4  -.30006447018987565E-15,  -.12070065266647787E-15,
     5  -.48765333924436936E-16,  -.19795375904407013E-16,
     6  -.80601519517948527E-17,  -.33007545307752108E-17,
     7  -.13529889994365963E-17,  -.56046923611257381E-18,
     8  -.22638647498297411E-18,  -.98812756643137522E-19,
     9  -.32169026263449524E-19,  -.20156187374987404E-19,
     *  -.30897592830102611E-20,  -.67175194230920184E-20,
     *  -.12353646815936770E-20,  -.44789638471749522E-20,
     *  0.38146476008074872E-20,  -.41722809488432723E-20,
     *  0.83445779218023687E-20,  -.42524617171735518E-20,
     *  0.43288845896606633E-20,  -.44418995438696879E-20/
        data pp605/
     1  -.41408230716893242E+00,  -.31306867057675229E-02,
     2  0.39031676919464567E-03,  0.30482905446211064E-04,
     3  0.33859624238133770E-05,  0.57979202233292615E-06,
     4  0.12678230243349048E-06,  0.32003728108215718E-07,
     5  0.88879592589507153E-08,  0.26433962460486350E-08,
     6  0.82787872479560904E-09,  0.26996962436188106E-09,
     7  0.90940455189027320E-10,  0.31460907215084561E-10,
     8  0.11129015368792843E-10,  0.40118592497231647E-11,
     9  0.14698756572327428E-11,  0.54617602908242481E-12,
     *  0.20546842970869147E-12,  0.78143597869885415E-13/
        data pq605/
     1  0.30009303429007433E-13,  0.11624995811221835E-13,
     2  0.45386756503542343E-14,  0.17846136172887289E-14,
     3  0.70625044215202988E-15,  0.28114703134828110E-15,
     4  0.11252425333651704E-15,  0.45262741142893572E-16,
     5  0.18287054881581490E-16,  0.74232448224932519E-17,
     6  0.30225821755056992E-17,  0.12377588566983952E-17,
     7  0.50737095644917274E-18,  0.21015092861126257E-18,
     8  0.84920341140499406E-19,  0.37029082615177981E-19,
     9  0.12115660572921736E-19,  0.75321197829542122E-20,
     *  0.11855400455661354E-20,  0.24917384548657094E-20,
     *  0.46318814654595149E-21,  0.16513800641025324E-20,
     *  -.14018078003230953E-20,  0.15354446376414718E-20,
     *  -.30696835607734399E-20,  0.15644174864559606E-20,
     *  -.15924528909635391E-20,  0.16341764486868586E-20/
        data pp606/
     1  0.65504602151085214E-01,  0.49923681631859073E-03,
     2  -.62143486106579522E-04,  -.48745950000970434E-05,
     3  -.54188941793315484E-06,  -.92776162191310141E-07,
     4  -.20285883010833795E-07,  -.51206801185008265E-08,
     5  -.14220867370289621E-08,  -.42294593258759328E-09,
     6  -.13246114602100293E-09,  -.43195271111472590E-10,
     7  -.14550506484471539E-10,  -.50337542990556077E-11,
     8  -.17806450636101451E-11,  -.64189825142703798E-12,
     9  -.23518034135128319E-12,  -.87388239041944084E-13,
     *  -.32874972767055013E-13,  -.12502983575534205E-13/
        data pq606/
     1  -.48014912139074769E-14,  -.18600002371863452E-14,
     2  -.72618842168195140E-15,  -.28553828760287159E-15,
     3  -.11300011023626463E-15,  -.44983537075293094E-16,
     4  -.18003887879211666E-16,  -.72420382852129965E-17,
     5  -.29259339297487916E-17,  -.11877171917199680E-17,
     6  -.48361551629333848E-18,  -.19803915521050219E-18,
     7  -.81179361062111373E-19,  -.33621797560392491E-19,
     8  -.13589641228883931E-19,  -.59222394023795243E-20,
     9  -.19434152824174742E-20,  -.12026549887061686E-20,
     *  -.19221086882752233E-21,  -.39611122255135876E-21,
     *  -.74103284229299791E-22,  -.26156942119988068E-21,
     *  0.22159514670716296E-21,  -.24293232641168342E-21,
     *  0.48555828100592802E-21,  -.24746519776957988E-21,
     *  0.25189243750624016E-21,  -.25850647738661784E-21/
c
        data pp701/
     1  0.74872346898897966E+01,  0.18061946170367455E+01,
     2  0.41220770595644170E+00,  0.12435577799333225E+00,
     3  0.42275449579953764E-01,  0.15333730408961027E-01,
     4  0.57937296148709854E-02,  0.22516975749786902E-02,
     5  0.89334586370895591E-03,  0.36005598536468846E-03,
     6  0.14693194796255894E-03,  0.60565832956614169E-04,
     7  0.25173497531968097E-04,  0.10536249948200252E-04,
     8  0.44361489527871021E-05,  0.18773607126499892E-05,
     9  0.79803859495921107E-06,  0.34056491024394211E-06,
     *  0.14584155766373034E-06,  0.62647710748378471E-07/
        data pq701/
     1  0.26985709495997257E-07,  0.11653317770291146E-07,
     2  0.50437234379140892E-08,  0.21875157223022607E-08,
     3  0.95054539094307709E-09,  0.41376064884683211E-09,
     4  0.18039352395780936E-09,  0.78765426560449740E-10,
     5  0.34438677900905198E-10,  0.15076884747322396E-10,
     6  0.66083566736061155E-11,  0.28997339287433350E-11,
     7  0.12737229918082128E-11,  0.56003624482461215E-12,
     8  0.24646561226694596E-12,  0.10856059945478535E-12,
     9  0.47856768706085766E-13,  0.21112930499667819E-13,
     *  0.93212262067644583E-14,  0.41180687832898774E-14,
     *  0.18205551796457072E-14,  0.80532149313539924E-15,
     *  0.35647419805645173E-15,  0.15780906924547782E-15,
     *  0.69902112021681061E-16,  0.30758089411097802E-16,
     *  0.13245837403317458E-16,  0.48741777395885949E-17/
        data pp702/
     1  0.22710539632431029E+01,  0.15734286144192398E-01,
     2  -.20036596711570732E-02,  -.14733767226100414E-03,
     3  -.16197628631391345E-04,  -.27796128042808731E-05,
     4  -.60830751728364680E-06,  -.15358878265740442E-06,
     5  -.42657573692999317E-07,  -.12687416125974283E-07,
     6  -.39736254560706813E-08,  -.12958082868769265E-08,
     7  -.43650240853626283E-09,  -.15100915452515871E-09,
     8  -.53418362270933622E-10,  -.19256654409735246E-10,
     9  -.70553204931872676E-11,  -.26216189052369341E-11,
     *  -.98624005820433319E-12,  -.37508649923418421E-12/
        data pq702/
     1  -.14404372353316674E-12,  -.55799662376839054E-13,
     2  -.21785531896085634E-13,  -.85661073317431139E-14,
     3  -.33899883008759383E-14,  -.13495015913316616E-14,
     4  -.54011378309815429E-15,  -.21726132425692826E-15,
     5  -.87775934184692214E-16,  -.35632331903137059E-16,
     6  -.14507500796461217E-16,  -.59420999083973195E-17,
     7  -.24353782509768189E-17,  -.10096146527697568E-17,
     8  -.40671423711805598E-18,  -.17865324836174135E-18,
     9  -.56296262898618191E-19,  -.37094491906657275E-19,
     *  -.47349366928084155E-20,  -.12932313161154195E-19,
     *  -.22261656697922039E-20,  -.89304468428176932E-20,
     *  0.77486322884536896E-20,  -.84067902673092609E-20,
     *  0.16851426222769730E-19,  -.85847726306815857E-20,
     *  0.87414839499031695E-20,  -.89652705784236852E-20/
        data pp703/
     1  -.25372565403858936E+01,  -.18907955594749867E-01,
     2  0.23646932968087462E-02,  0.18307660215256935E-03,
     3  0.20306218951572238E-04,  0.34781492384464827E-05,
     4  0.76064951643956394E-06,  0.19201716694703819E-06,
     5  0.53326928522383031E-07,  0.15860219277540135E-07,
     6  0.49672379861731161E-08,  0.16198095477343119E-08,
     7  0.54564062817287696E-09,  0.18876487183525664E-09,
     8  0.66773929443391578E-10,  0.24071107285655661E-10,
     9  0.88192391823154860E-11,  0.32770515254837049E-11,
     *  0.12328090774546424E-11,  0.46886109248389430E-12/
        data pq703/
     1  0.18005565396774567E-12,  0.69749918173671272E-13,
     2  0.27232034034227637E-13,  0.10707674918526693E-13,
     3  0.42375001847358899E-14,  0.16868814516106861E-14,
     4  0.67514504345500039E-15,  0.27157648309815175E-15,
     5  0.10972197198171514E-15,  0.44539610276876188E-16,
     6  0.18135327059558549E-16,  0.74267128520943507E-17,
     7  0.30442253606037541E-17,  0.12610711086931514E-17,
     8  0.50935406572965342E-18,  0.22234468239297322E-18,
     9  0.72347557823938830E-19,  0.45367924599322436E-19,
     *  0.69353702038256378E-20,  0.15131808027687512E-19,
     *  0.27795143724245915E-20,  0.10095194169482133E-19,
     *  -.86011331374362350E-20,  0.94059024504377895E-20,
     *  -.18812407676808637E-19,  0.95869095529528571E-20,
     *  -.97588317644029391E-20,  0.10013613209693660E-19/
        data pp704/
     1  0.21697445294670109E+01,  0.16611561866867826E-01,
     2  -.20660242264994446E-02,  -.16243183652709370E-03,
     3  -.18065577233281812E-04,  -.30927433804176859E-05,
     4  -.67621211950252169E-06,  -.17069120540997625E-06,
     5  -.47403186393797008E-07,  -.14098254098819865E-07,
     6  -.44153837638774842E-08,  -.14398452872225831E-08,
     7  -.48501763086183021E-09,  -.16779201321737875E-09,
     8  -.59354893341216436E-10,  -.21396625526451762E-10,
     9  -.78393499608922814E-11,  -.29129429545995900E-11,
     *  -.10958329592273517E-11,  -.41676629511957434E-12/
        data pq704/
     1  -.16004976634484091E-12,  -.62000028079143186E-13,
     2  -.24206287773037451E-13,  -.95179453469637662E-14,
     3  -.37666712187847221E-14,  -.14994515122243546E-14,
     4  -.60012975059386292E-15,  -.24140127823263010E-15,
     5  -.97531228180862929E-16,  -.39590537353399028E-16,
     6  -.16120561237414746E-16,  -.66012637963574428E-17,
     7  -.27059789781292434E-17,  -.11206831625004805E-17,
     8  -.45303216061230686E-18,  -.19736377584078805E-18,
     9  -.64870121322869150E-19,  -.40043081495118084E-19,
     *  -.64534447588062850E-20,  -.13157216244077763E-19,
     *  -.24698210540775695E-20,  -.86703685329581507E-20,
     *  0.73376220517843908E-20,  -.80477863041476378E-20,
     *  0.16082956107833266E-19,  -.81968856118400127E-20,
     *  0.83428138457928747E-20,  -.85622653302309857E-20/
        data pp705/
     1  -.11972846362744574E+01,  -.92920587587767995E-02,
     2  0.11526018289232245E-02,  0.91284673216941956E-04,
     3  0.10166101939714596E-04,  0.17399628678242338E-05,
     4  0.38039165506821123E-06,  0.96016416692675652E-07,
     5  0.26664706614405226E-07,  0.79303470963926981E-08,
     6  0.24836705566892020E-08,  0.80991707453716170E-09,
     7  0.27282346906330405E-09,  0.94383293206610430E-10,
     8  0.33387208898669141E-10,  0.12035625967197533E-10,
     9  0.44096417340875559E-11,  0.16385327366170728E-11,
     *  0.61640678999340126E-12,  0.23443128839173083E-12/
        data pq705/
     1  0.90028076857594942E-13,  0.34875044151055335E-13,
     2  0.13616046797557506E-13,  0.53538476593762166E-14,
     3  0.21187537946780911E-14,  0.84344185306564461E-15,
     4  0.33757321363950771E-15,  0.13578821031750764E-15,
     5  0.54861475573901178E-16,  0.22269615909237293E-16,
     6  0.90678891397206358E-17,  0.37131408058851493E-17,
     7  0.15221134404687588E-17,  0.63031137340230692E-18,
     8  0.25490460536221845E-18,  0.11094234701378265E-18,
     9  0.36641481867348046E-19,  0.22447290373293239E-19,
     *  0.37083041528227919E-20,  0.73215110702300998E-20,
     *  0.13890304256955060E-20,  0.47949335782384590E-20,
     *  -.40440386387039688E-20,  0.44420736542365630E-20,
     *  -.88734672006374721E-20,  0.45227354073585068E-20,
     *  -.46028998331556137E-20,  0.47244507883745294E-20/
        data pp706/
     1  0.37878553379329519E+00,  0.29637856375223014E-02,
     2  -.36705750999801106E-03,  -.29195302108389400E-04,
     3  -.32539452242936423E-05,  -.55684450038783320E-06,
     4  -.12172962406472210E-06,  -.30725755552284800E-07,
     5  -.85327856792107662E-08,  -.25377262727252185E-08,
     6  -.79477787877844012E-09,  -.25917425118158485E-09,
     7  -.87303712034182231E-10,  -.30202708695665955E-10,
     8  -.10683922475560664E-10,  -.38514049384167852E-11,
     9  -.14110867720932084E-11,  -.52433092205580329E-12,
     *  -.19725031688093893E-12,  -.75018059784272372E-13/
        data pq706/
     1  -.28809000585342481E-13,  -.11160019573119745E-13,
     2  -.43571368805632603E-14,  -.17132319044378670E-14,
     3  -.67800145124668919E-15,  -.26990146576223849E-15,
     4  -.10802347200040781E-15,  -.43452225955058729E-16,
     5  -.17555702206676546E-16,  -.71262656264175859E-17,
     6  -.29017383019793157E-17,  -.11881919348852560E-17,
     7  -.48707635466987409E-18,  -.20168597547661179E-18,
     8  -.81583346917787874E-19,  -.35487545161473873E-19,
     9  -.11753743800318942E-19,  -.71687232246234434E-20,
     *  -.12013165113563641E-20,  -.23280202689067077E-20,
     *  -.44444019606109773E-21,  -.15189908268563099E-20,
     *  0.12784874819805322E-20,  -.14055839329313327E-20,
     *  0.28070717367322138E-20,  -.14307923662663709E-20,
     *  0.14560712142834568E-20,  -.14946162134118581E-20/
        data pp707/
     1  -.52213488607034996E-01,  -.41075813686728511E-03,
     2  0.50819003981905257E-04,  0.40534511847153927E-05,
     3  0.45200930106008124E-06,  0.77344723032753842E-07,
     4  0.16907290175648050E-07,  0.42675125722973289E-08,
     5  0.11851164903636340E-08,  0.35246339002293754E-09,
     6  0.11038612212623953E-09,  0.35996496678352043E-10,
     7  0.12125534258285115E-10,  0.41948257327683913E-11,
     8  0.14838795686584198E-11,  0.53491778116495786E-12,
     9  0.19598440512365420E-12,  0.72823780502309868E-13,
     *  0.27395890685647319E-13,  0.10419179368123028E-13/
        data pq707/
     1  0.40012515619058340E-14,  0.15500032226555667E-14,
     2  0.60515807648021799E-15,  0.23794893613916573E-15,
     3  0.94166890168404083E-16,  0.37486321447824307E-16,
     4  0.15003264020199412E-16,  0.60350312783075280E-17,
     5  0.24382947128213942E-17,  0.98975807244955537E-18,
     6  0.40302046428109614E-18,  0.16502546327908916E-18,
     7  0.67649498934685028E-19,  0.28010696319625708E-19,
     8  0.11332284281283009E-19,  0.49275509602235982E-20,
     9  0.16350547528428642E-20,  0.99434470614624830E-21,
     *  0.16818427411453555E-21,  0.32198484090611745E-21,
     *  0.61722818521800434E-22,  0.20957023436634557E-21,
     *  -.17614872269718687E-21,  0.19377526781031969E-21,
     *  -.38691890946197732E-21,  0.19722119462232962E-21,
     *  -.20069646290595369E-21,  0.20601828919789611E-21/
c
        data pp801/
     1  0.74441010995282651E+01,  0.18058461393093587E+01,
     2  0.41225060507544502E+00,  0.12435924625931108E+00,
     3  0.42275837318092936E-01,  0.15333796726647007E-01,
     4  0.57937441085424977E-02,  0.22517012330462227E-02,
     5  0.89334687955468258E-03,  0.36005628748219984E-03,
     6  0.14693204258054504E-03,  0.60565863811066644E-04,
     7  0.25173507925363314E-04,  0.10536253543786941E-04,
     8  0.44361502246900771E-05,  0.18773611711527814E-05,
     9  0.79803876294640643E-06,  0.34056497266450252E-06,
     *  0.14584158114597954E-06,  0.62647719679122494E-07/
        data pq801/
     1  0.26985712925647798E-07,  0.11653319098867498E-07,
     2  0.50437239566217679E-08,  0.21875159262587513E-08,
     3  0.95054547165764850E-09,  0.41376068097799375E-09,
     4  0.18039353681776725E-09,  0.78765431733333345E-10,
     5  0.34438679990883452E-10,  0.15076885595682376E-10,
     6  0.66083570190574301E-11,  0.28997340701887456E-11,
     7  0.12737230497932768E-11,  0.56003626882866112E-12,
     8  0.24646562198532542E-12,  0.10856060367308565E-12,
     9  0.47856770118227600E-13,  0.21112931346580642E-13,
     *  0.93212263563488533E-14,  0.41180690534508093E-14,
     *  0.18205552327217707E-14,  0.80532166701647982E-15,
     *  0.35647405299463429E-15,  0.15780922921946171E-15,
     *  0.69901792620838892E-16,  0.30758252172672592E-16,
     *  0.13245671351815621E-16,  0.48743479342775883E-17/
        data pp802/
     1  0.25729890957738233E+01,  0.18173630235899922E-01,
     2  -.23039535041803726E-02,  -.17161553411282928E-03,
     3  -.18911795605591411E-04,  -.32438366061394016E-05,
     4  -.70976321787012718E-06,  -.17919525538534567E-06,
     5  -.49768493779730452E-07,  -.14802238705670703E-07,
     6  -.46359513587585577E-08,  -.15117894542019599E-08,
     7  -.50925617505398475E-09,  -.17617826134520882E-09,
     8  -.62321683095688781E-10,  -.22466173955223208E-10,
     9  -.82312308606545299E-11,  -.30585628281055251E-11,
     *  -.11506158025533835E-11,  -.43760170738999874E-12/
        data pq802/
     1  -.16805127731948012E-12,  -.65099696839781362E-13,
     2  -.25416485647038562E-13,  -.99938027660018078E-14,
     3  -.39549903007374410E-14,  -.15744197227801709E-14,
     4  -.63013348835137413E-15,  -.25347150949110290E-15,
     5  -.10240578195979992E-15,  -.41570851763676694E-16,
     6  -.16925659998479989E-16,  -.69322177828769968E-17,
     7  -.28412736991729469E-17,  -.11776429955051206E-17,
     8  -.47474289329028264E-18,  -.20818135048639228E-18,
     9  -.66181255745242322E-19,  -.43022881655240234E-19,
     *  -.57820274612158427E-20,  -.14823439660104389E-19,
     *  -.25976981024835168E-20,  -.10147614396628335E-19,
     *  0.87640650212460130E-20,  -.95266081645499605E-20,
     *  0.19087232119895621E-19,  -.97241036313083104E-20,
     *  0.99038444745417435E-20,  -.10156633399694585E-19/
        data pp803/
     1  -.34430619379780548E+01,  -.26225987869872439E-01,
     2  0.32655747958786445E-02,  0.25591018770804476E-03,
     3  0.28448719874172437E-04,  0.48708206440220679E-05,
     4  0.10650166181990051E-05,  0.26883658513086195E-06,
     5  0.74659688782576436E-07,  0.22204687016629394E-07,
     6  0.69542156942367453E-08,  0.22677530497094122E-08,
     7  0.76390192772604274E-09,  0.26427219229540697E-09,
     8  0.93483891917657054E-10,  0.33699665922119548E-10,
     9  0.12346970284717273E-10,  0.45878832940894781E-11,
     *  0.17259363105017932E-11,  0.65640671695133788E-12/
        data pq803/
     1  0.25207831532668581E-12,  0.97650021562498197E-13,
     2  0.38124895287086408E-13,  0.14990761221302775E-13,
     3  0.59325061843204010E-14,  0.23616358459562089E-14,
     4  0.94520415921466963E-15,  0.38020703880067982E-15,
     5  0.15361151530704503E-15,  0.62355169858490407E-16,
     6  0.25389804665623270E-16,  0.10397066475526334E-16,
     7  0.42619117051894782E-17,  0.17651561369027584E-17,
     8  0.71344003424732155E-18,  0.31092898877262819E-18,
     9  0.10200253635953589E-18,  0.63153093847391848E-19,
     *  0.10076642507292395E-19,  0.20805187536026554E-19,
     *  0.38941116735105612E-20,  0.13746696837864116E-19,
     *  -.11647431325481271E-19,  0.12765356137471090E-19,
     *  -.25519825365995627E-19,  0.13004902566430867E-19,
     *  -.13245913333009342E-19,  0.13587701675253133E-19/
        data pp804/
     1  0.36794201921206129E+01,  0.28808282325405447E-01,
     2  -.35674933916159418E-02,  -.28382114578621939E-03,
     3  -.31636412104282143E-04,  -.54138623897103280E-05,
     4  -.11834906224349236E-05,  -.29872356904968251E-06,
     5  -.82957786827452683E-07,  -.24672366997301965E-07,
     6  -.77270132773168662E-08,  -.25197511238477501E-08,
     7  -.84878646345043983E-09,  -.29363754731762929E-09,
     8  -.10387149746499223E-09,  -.37444223253891573E-10,
     9  -.13718901798228593E-10,  -.50976625689425453E-11,
     *  -.19177116809726030E-11,  -.72934233589864696E-12/
        data pq804/
     1  -.28008753527640782E-12,  -.10850020039385473E-12,
     2  -.42361056527802074E-13,  -.16656422518257237E-13,
     3  -.65916812180922395E-14,  -.26240421694668941E-14,
     4  -.10502282768599732E-14,  -.42245220440350793E-15,
     5  -.17068046705640763E-15,  -.69283136656091362E-16,
     6  -.28211357247515940E-16,  -.11551853168746180E-16,
     7  -.47354562191075701E-17,  -.19608248761825385E-17,
     8  -.79317544147596767E-18,  -.34500428646971773E-18,
     9  -.11429508555024612E-18,  -.69685030240924200E-19,
     *  -.11688898599797713E-19,  -.22612848751972757E-19,
     *  -.43274832224595102E-20,  -.14756206309151672E-19,
     *  0.12414785706865707E-19,  -.13646875784853305E-19,
     *  0.27261985591276183E-19,  -.13893540627118320E-19,
     *  0.14154616463655493E-19,  -.14519079437603873E-19/
        data pp805/
     1  -.27069602989280593E+01,  -.21488779217314421E-01,
     2  0.26540709940397217E-02,  0.21267398247606765E-03,
     3  0.23736936810714927E-04,  0.40610818771168759E-05,
     4  0.88767015800061315E-06,  0.22404878033238191E-06,
     5  0.62219307048060900E-07,  0.18504459994874797E-07,
     6  0.57953000701285840E-08,  0.18898229111623288E-08,
     7  0.63659230165191368E-09,  0.22022882730686098E-09,
     8  0.77903813022444935E-10,  0.28083223694637344E-10,
     9  0.10289193571423868E-10,  0.38232523509600282E-11,
     *  0.14382855117386526E-11,  0.54700732917080346E-12/
        data pq805/
     1  0.21006584578916185E-12,  0.81375216465766877E-13,
     2  0.31770815552322125E-13,  0.12492324830669687E-13,
     3  0.49437637939856078E-14,  0.19680325103081829E-14,
     4  0.78767173990562215E-15,  0.31683913648838614E-15,
     5  0.12801071444945117E-15,  0.51962215211928349E-16,
     6  0.21158685149827014E-16,  0.86637301782716856E-17,
     7  0.35515906814444577E-17,  0.14704530870847598E-17,
     8  0.59504788622370951E-18,  0.25858285764449223E-18,
     9  0.86066446094251231E-19,  0.52089239119518443E-19,
     *  0.89437579926193342E-20,  0.16777143582646179E-19,
     *  0.32466925950626939E-20,  0.10880771358404041E-19,
     *  -.91212022860841624E-20,  0.10041163133385249E-19,
     *  -.20052496683237663E-19,  0.10219390428239544E-19,
     *  -.10414702448503214E-19,  0.10681264897314369E-19/
        data pp806/
     1  0.12845909313854564E+01,  0.10281817912644874E-01,
     2  -.12679390090679094E-02,  -.10202888766386482E-03,
     3  -.11396446146893841E-04,  -.19495159059634185E-05,
     4  -.42609672582416325E-06,  -.10754517373610856E-06,
     5  -.29865545939404171E-07,  -.88821940118144780E-08,
     6  -.27817555868420693E-08,  -.90711775315668508E-09,
     7  -.30556501158734801E-09,  -.10571002915581628E-09,
     8  -.37393884949826141E-10,  -.13479963574880672E-10,
     9  -.49388178744949955E-11,  -.18351626906615765E-11,
     *  -.69037754992808971E-12,  -.26256368425171595E-12/
        data pq806/
     1  -.10083166194428263E-12,  -.39060122961946669E-13,
     2  -.15249998133422034E-13,  -.59963182072139490E-14,
     3  -.23730074508312011E-14,  -.94465586010776211E-15,
     4  -.37808258776007359E-15,  -.15208278165758602E-15,
     5  -.61445245532002144E-16,  -.24941825208032661E-16,
     6  -.10156215908038727E-16,  -.41585455583174786E-17,
     7  -.17047626992573023E-17,  -.70577100368660000E-18,
     8  -.28566931543688901E-18,  -.12407185153795215E-18,
     9  -.41408722336187053E-19,  -.24953892471746779E-19,
     *  -.43425888164389835E-20,  -.80013997740304787E-20,
     *  -.15590374970498486E-20,  -.51704934926456628E-20,
     *  0.43247856764015334E-20,  -.47650376207883798E-20,
     *  0.95144894264575704E-20,  -.48487853751061347E-20,
     *  0.49431527853202391E-20,  -.50687046776455769E-20/
        data pp807/
     1  -.35414862113775539E+00,  -.28501022285748093E-02,
     2  0.35111283700520469E-03,  0.28331313036540532E-04,
     3  0.31661762752601474E-05,  0.54156852489128227E-06,
     4  0.11836299076212843E-06,  0.29873985300238581E-07,
     5  0.82960365770947689E-08,  0.24672859697193574E-08,
     6  0.77271202481411593E-09,  0.25197766400338545E-09,
     7  0.84879300776007040E-10,  0.29363932552818500E-10,
     8  0.10387200393413579E-10,  0.37444373266529202E-11,
     9  0.13718947725909166E-11,  0.50976770337090094E-12,
     *  0.19177163503469758E-12,  0.72934387523937553E-13/
        data pq807/
     1  0.28008805348219215E-13,  0.10850037685597876E-13,
     2  0.42361118274331407E-14,  0.16656443703978601E-14,
     3  0.65916889002990505E-15,  0.26240445289633117E-15,
     4  0.10502296927342397E-15,  0.42245216512481969E-16,
     5  0.17068142487934146E-16,  0.69282779329877004E-17,
     6  0.28211796663043450E-17,  0.11551433377560449E-17,
     7  0.47354494712845174E-18,  0.19603903905550012E-18,
     8  0.79360940451947311E-19,  0.34455653087049549E-19,
     9  0.11520047598470296E-19,  0.69227344558640994E-20,
     *  0.12152750417747930E-20,  0.22131113441694800E-20,
     *  0.43325525356815786E-21,  0.14267377910790939E-20,
     *  -.11915814492115777E-20,  0.13135931640002021E-20,
     *  -.26227248064420443E-20,  0.13365522003030293E-20,
     *  -.13630569854862856E-20,  0.13973811110864796E-20/
        data pp808/
     1  0.43133590361531486E-01,  0.34847772738678917E-03,
     2  -.42899119003328490E-04,  -.34682659788321627E-05,
     3  -.38773813917143802E-06,  -.66317685979789776E-07,
     4  -.14493671512354341E-07,  -.36580675325630360E-08,
     5  -.10158457266758764E-08,  -.30211751138520283E-09,
     6  -.94617986098268057E-10,  -.30854452475004773E-10,
     7  -.10393395216817418E-10,  -.35955866885785870E-11,
     8  -.12719029749650227E-11,  -.45850279221256604E-12,
     9  -.16798719535246605E-12,  -.62420560409798724E-13,
     *  -.23482249192721467E-13,  -.89307440222592173E-14/
        data pq808/
     1  -.34296505409019128E-14,  -.13285763518489001E-14,
     2  -.51870767870756187E-15,  -.20395649060838458E-15,
     3  -.80714571408789544E-16,  -.32131161635502047E-16,
     4  -.12859957893315577E-16,  -.51728836048845115E-17,
     5  -.20899782535835464E-17,  -.84835998007677991E-18,
     6  -.34545131457196617E-18,  -.14144541063992407E-18,
     7  -.57985064029630949E-19,  -.24004048963190932E-19,
     8  -.97183794552895739E-20,  -.42183003024413159E-20,
     9  -.14121418349995163E-20,  -.84691282039867232E-21,
     *  -.14958439529149528E-21,  -.27016092833805678E-21,
     *  -.53076060527478730E-22,  -.17388107906750888E-21,
     *  0.14506182163133443E-21,  -.15997398416685888E-21,
     *  0.31940084133685566E-21,  -.16276157069151809E-21,
     *  0.16605150346436376E-21,  -.17019468937591960E-21/
c
c        determine the parameter in the chebychev approximation
c
        done=1
        ac=1000
        bc=50000
        ac=done/dsqrt(ac)
        bc=done/dsqrt(bc)
c
        u=2/(bc-ac)
        v=done-u*bc
c
        tt=done/dsqrt(t)
        x=u*tt+v
c
c        one after another, calculate the values of
c        the correction weights at the user-specified  t
c
        n=47
c
c        . . . if m=1
c
        if(m .ne. 1) goto 2100
        call chexev(pp101,x,whts(1),n-1)
        return
 2100 continue
c
        if(m .ne. 2) goto 2200
        call chexev(pp201,x,whts(1),n-1)
        call chexev(pp202,x,whts(2),n-1)
        return
 2200 continue
c
        if(m .ne. 3) goto 2300
        call chexev(pp301,x,whts(1),n-1)
        call chexev(pp302,x,whts(2),n-1)
        call chexev(pp303,x,whts(3),n-1)
        return
 2300 continue
c
        if(m .ne. 4) goto 2400
        call chexev(pp401,x,whts(1),n-1)
        call chexev(pp402,x,whts(2),n-1)
        call chexev(pp403,x,whts(3),n-1)
        call chexev(pp404,x,whts(4),n-1)
        return
 2400 continue
c
        if(m .ne. 5) goto 2500
        call chexev(pp501,x,whts(1),n-1)
        call chexev(pp502,x,whts(2),n-1)
        call chexev(pp503,x,whts(3),n-1)
        call chexev(pp504,x,whts(4),n-1)
        call chexev(pp505,x,whts(5),n-1)
        return
 2500 continue
c
        if(m .ne. 6) goto 2600
        call chexev(pp601,x,whts(1),n-1)
        call chexev(pp602,x,whts(2),n-1)
        call chexev(pp603,x,whts(3),n-1)
        call chexev(pp604,x,whts(4),n-1)
        call chexev(pp605,x,whts(5),n-1)
        call chexev(pp606,x,whts(6),n-1)
        return
 2600 continue
c
        if(m .ne. 7) goto 2700
        call chexev(pp701,x,whts(1),n-1)
        call chexev(pp702,x,whts(2),n-1)
        call chexev(pp703,x,whts(3),n-1)
        call chexev(pp704,x,whts(4),n-1)
        call chexev(pp705,x,whts(5),n-1)
        call chexev(pp706,x,whts(6),n-1)
        call chexev(pp707,x,whts(7),n-1)
        return
 2700 continue
c
        if(m .ne. 8) goto 2800
        call chexev(pp801,x,whts(1),n-1)
        call chexev(pp802,x,whts(2),n-1)
        call chexev(pp803,x,whts(3),n-1)
        call chexev(pp804,x,whts(4),n-1)
        call chexev(pp805,x,whts(5),n-1)
        call chexev(pp806,x,whts(6),n-1)
        call chexev(pp807,x,whts(7),n-1)
        call chexev(pp808,x,whts(8),n-1)
        return
 2800 continue
        return
 3000 continue
cc
        return
        end         
        
        



c=================================================================================
c====================== file prini.f ===========================================
c=================================================================================


	SUBROUTINE PRINI(IP1,IQ1)
	CHARACTER *1 MES(1), AA(1)
	REAL *4 A(1)
	REAL *8 A2(1)
	INTEGER *4 IA(1)
	INTEGER *2 IA2(1)
	IP=IP1
	IQ=IQ1
	RETURN

C
C
C
C
	ENTRY PRIN(MES,A,N)
	CALL  MESSPR(MES,IP,IQ)
	IF(IP.NE.0 .AND. N.NE.0) WRITE(IP,1200)(A(J),J=1,N)
	IF(IQ.NE.0 .AND. N.NE.0) WRITE(IQ,1200)(A(J),J=1,N)
 1200 FORMAT(6(2X,E11.5))
	 RETURN
C
C
C
C
	ENTRY PRIN2(MES,A2,N)
	CALL MESSPR(MES,IP,IQ)
	IF(IP.NE.0 .AND. N.NE.0) WRITE(IP,1400)(A2(J),J=1,N)
	IF(IQ.NE.0 .AND. N.NE.0) WRITE(IQ,1400)(A2(J),J=1,N)
 1400 FORMAT(6(2X,E11.5))
	RETURN
C
C
C
C
	ENTRY PRINF(MES,IA,N)
	CALL MESSPR(MES,IP,IQ)
	IF(IP.NE.0 .AND. N.NE.0) WRITE(IP,1600)(IA(J),J=1,N)
	IF(IQ.NE.0 .AND. N.NE.0) WRITE(IQ,1600)(IA(J),J=1,N)
 1600 FORMAT(10(1X,I7))
	RETURN
C
C
C
C
	ENTRY PRINF2(MES,IA2,N)
	CALL MESSPR(MES,IP,IQ)
	IF(IP.NE.0 .AND. N.NE.0) WRITE(IP,1600)(IA2(J),J=1,N)
	IF(IQ.NE.0 .AND. N.NE.0) WRITE(IQ,1600)(IA2(J),J=1,N)
	RETURN
C
C
C
C
	ENTRY PRINA(MES,AA,N)
	CALL MESSPR(MES,IP,IQ)
 2000 FORMAT(1X,80A1)
	IF(IP.NE.0 .AND. N.NE.0) WRITE(IP,2000)(AA(J),J=1,N)
	IF(IQ.NE.0 .AND. N.NE.0) WRITE(IQ,2000)(AA(J),J=1,N)
	RETURN
	END
c
c
c
c
c
	SUBROUTINE MESSPR(MES,IP,IQ)
	CHARACTER *1 MES(1),AST
	DATA AST/'*'/
C
C         DETERMINE THE LENGTH OF THE MESSAGE
C
	I=0
	DO 1400 I=1,10000
	IF(MES(I).EQ.AST) GOTO 1600
	I1=I
 1400 CONTINUE
 1600 CONTINUE
	 IF ( (I1.NE.0) .AND. (IP.NE.0) )
     1     WRITE(IP,1800) (MES(I),I=1,I1)
	 IF ( (I1.NE.0) .AND. (IQ.NE.0) )
     1     WRITE(IQ,1800) (MES(I),I=1,I1)
 1800 FORMAT(1X,80A1)
	 RETURN
	 END
c















