c ============================================================================================
c Compute the scattered field solution of electromagnetic scattering from a cylinder
c          a : radius of the cylinder
c          k : wavelength of the incident light
c          m : m=c1/c2 - ratio of speed of sounds
c
c  This solution is taken from 
c    Bowman, Senior and Uslemghi, "Electromagnetic And Acoustic Scattering by Simple Shapes"
c
c  The solution is for an incident wave traveling in the positive x-direction (The opposite direction
c  to the above ref):   
c                     Hz =     exp(i(k*x-w*t))
c                     Ey = -Z* exp(i(k*x-w*t))
c
c
c  nd : =2 number of spcae dimensions
c  n1a:n1b, n2a:n2b, n3a:n3b : evaluate solution at these points, i1=n1a..n1b, 
c  nd1a:nd1b,... : dimensions of u, xy
c  xy : grid points 
c
c  ipar(0) = exr : store the Re part of Ex here
c  ipar(1) = eyr : store the Re part of Ey here
c  ipar(2) = hzr : store the Re part of Hz here
c  ipar(3) = exi : store the Im part of Ex here
c  ipar(4) = eyi : store the Im part of Ey here
c  ipar(5) = hzi : store the Im part of Hz here
c 
c  ipar(6) : option : 0=no dielectric, 1=dielectric
c  ipar(7) : inOut : 0=exterior, 1=interior (for dielectric)
c  ipar(9)= staggeredGrid : 0 = return node centered data, 1= return E and H on a "Yee" type staggered grid.
c  
c =============================================================================================
      subroutine scatcyl(nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                         xy,u,ipar,rpar )

      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)

      integer ipar(0:*),option
      real rpar(0:*)

c.............local variables
      integer ntermMax
      parameter( ntermMax=50 )
      integer i1,i2,i3,nterm,ncalc,nb,exr,eyr,hzr,exi,eyi,hzi,staggeredGrid,debug
      real k,a,ka,r,theta,kr,x,y,alpha,twoPi
      real jnka(0:ntermMax),ynka(0:ntermMax),jnpka(0:ntermMax),ynpka(0:ntermMax)
      real jn(0:ntermMax),yn(0:ntermMax),jnp(0:ntermMax),ynp(0:ntermMax),an(0:ntermMax)

      real s,sr,si,srr,srt,sir,sit,rx,ry,tx,ty,pm,cnt,cnp1t,sMax,eps,dir
      real sr0,srr0,srt0,si0,sir0,sit0
      integer n,np1
      integer numEdges,edge

c...............end local variables      

      option = ipar(6)
      if( option.eq.1 )then
        ! dielectric case:
        call scatDielectricCyl(nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                               xy,u,ipar,rpar )
        return
      end if

      k = rpar(0)  !
      a = rpar(1)  ! radius

      exr=ipar(0)
      eyr=ipar(1)
      hzr=ipar(2)
      exi=ipar(3)
      eyi=ipar(4)
      hzi=ipar(5)

      staggeredGrid=ipar(9)
      debug= ipar(10)

      ka=k*a
      write(*,'(" scatcyl: k,a=",2f10.6," exr,eyr,hzr=",3i3," staggeredGrid=",i2)') k,a,exr,eyr,hzr,staggeredGrid
      ! ' 
      if( ka.le.0 )then
        stop 11233
      end if

      ! I estimate that the number of terms, N, should satisfy
      !        ??? N * eps**(1/N) > e*k*a/2 * 1/(2*pi)**(1/N)
      ! where eps=size of the final term (series is alternating)
      ! Take N = max( e*k*a, log(1/eps) )

      eps = 1.e-16
      nterm = max( abs(7.*ka), 16. )
      nterm=min(nterm,ntermMax-2)
      nterm = nterm - mod(nterm,2) + 1   ! nterm should be odd
      ! nterm = 25  ! nterm should be odd
      twoPi=atan2(1.,1.)*8.

      ! First evaluate Jn(ka), Yn(ka) n=0,1,...,nterm
      alpha=0. ! fractional part of Bessel order

      nb = nterm+1  ! eval J0, J1, ... J(nb)  -- compute one extra 
      call rjbesl(ka, alpha, nb, jnka, ncalc)
      call rybesl(ka, alpha, nb, ynka, ncalc)

      ! compute the derivatives 
      
      jnpka(0) = -jnka(1)
      ynpka(0) = -ynka(1)
      do n=1,nterm-1
        jnpka(n) = .5*( jnka(n-1)-jnka(n+1) )
        ynpka(n) = .5*( ynka(n-1)-ynka(n+1) )
      end do

      ! precompute some coefficients
      do n=0,nterm-1
        an(n) = jnpka(n)/( jnpka(n)**2 + ynpka(n)**2 )
      end do

      sMax=0. ! keep track of the size of the last term for monitoring convergence

      dir=-1.


  
      numEdges=1
      if( staggeredGrid.eq.1 )then
       numEdges=3
       n1b=min(n1b,nd1b-1)
       n2b=min(n2b,nd2b-1)
      end if

      do edge=1,numEdges  ! for a staggered grid we need to evaluate along edges and the cell-centers 

       i3=n3a
       do i2=n2a,n2b
       do i1=n1a,n1b
         
         x=dir*xy(i1,i2,i3,0)   ! rotate by Pi so incident wave travels in the positive x-direction
         y=dir*xy(i1,i2,i3,1)
         if( staggeredGrid.eq.1 )then
           if( edge.eq.1 )then
             ! Ex lives on this edge
             x=dir*.5*(xy(i1,i2,i3,0)+xy(i1+1,i2,i3,0) )
           else if( edge.eq.2 )then
             ! Ey lives on this edge
             y=dir*.5*(xy(i1,i2,i3,1)+xy(i1,i2+1,i3,1) )
           else
             ! Hz lives at the cell center 
             x=dir*.5*(xy(i1,i2,i3,0)+xy(i1+1,i2,i3,0) )
             y=dir*.5*(xy(i1,i2,i3,1)+xy(i1,i2+1,i3,1) )
           end if
         end if
 
         r=sqrt(x*x+y*y)
         ! r=max(r,.75*a)    !  don't allow r to get too small -- not valid and convergence is poor
         if( r.lt. .75*a )then
           if( r.lt.eps )then
             x=eps  ! avoid atan(0,0)
             y=eps
           end if
           r=.75*a  !  don't allow r to get too small -- not valid and convergence is poor
         end if
 
         theta=atan2(y,x)
         ! if( theta.lt.0. ) then
         !   theta=theta+twoPi
         ! end if
         ! write(*,'(" i=",2i4," x,y,r,theta=",4f10.5)') i1,i2,x,y,r,theta
 
         kr=k*r
         call rjbesl(kr, alpha, nb, jn, ncalc)
         call rybesl(kr, alpha, nb, yn, ncalc)
         ! derivatives:
         jnp(0) = -jn(1)
         ynp(0) = -yn(1)
         do n=1,nterm-1
           jnp(n) = .5*( jn(n-1)-jn(n+1) )
           ynp(n) = .5*( yn(n-1)-yn(n+1) )
         end do
         
         ! compute the hz field: s and it derivatives sr, sTheta
         !   (sr,si) : holds the Re an Im parts of Hz
         !   srr = d(sr)/dr  srt=d(sr)/d(theta)
         sr=0.
         pm=1.  ! +1 or -1
 
         ! n=0: 
         n=0
         sr = .5*(  jn(n)*jnpka(n) +  yn(n)*ynpka(n))*an(n) 
         srr= .5*( jnp(n)*jnpka(n) + ynp(n)*ynpka(n))*an(n)
         srt= 0.
         si = .5*( -jn(n)*ynpka(n)+   yn(n)*jnpka(n))*an(n)
         sir= .5*(-jnp(n)*ynpka(n)+  ynp(n)*jnpka(n))*an(n)
         sit= 0.
         do n=1,nterm-2,2   ! nterm should be odd
 
           cnt=cos(n*theta)
           sr = sr + pm*( -jn(n)*ynpka(n)+  yn(n)*jnpka(n))*an(n)*cnt
           srr= srr+ pm*(-jnp(n)*ynpka(n)+ ynp(n)*jnpka(n))*an(n)*cnt
           srt= srt+ pm*( -jn(n)*ynpka(n)+  yn(n)*jnpka(n))*an(n)*(-n*sin(n*theta))
 
           si = si - pm*(  jn(n)*jnpka(n) +  yn(n)*ynpka(n) )*an(n)*cnt
           sir= sir- pm*( jnp(n)*jnpka(n) + ynp(n)*ynpka(n) )*an(n)*cnt
           sit= sit- pm*(  jn(n)*jnpka(n) +  yn(n)*ynpka(n) )*an(n)*(-n*sin(n*theta))
           
           np1=n+1
           cnp1t=cos(np1*theta)
           sr = sr - pm*(  jn(np1)*jnpka(np1) +  yn(np1)*ynpka(np1) )*an(np1)*cnp1t
           srr= srr- pm*( jnp(np1)*jnpka(np1) + ynp(np1)*ynpka(np1) )*an(np1)*cnp1t
           srt= srt- pm*(  jn(np1)*jnpka(np1) +  yn(np1)*ynpka(np1) )*an(np1)*(-np1*sin(np1*theta))
 
           si = si - pm*( -jn(np1)*ynpka(np1)+  yn(np1)*jnpka(np1))*an(np1)*cnp1t
           sir= sir- pm*(-jnp(np1)*ynpka(np1)+ ynp(np1)*jnpka(np1))*an(np1)*cnp1t
           sit= sit- pm*( -jn(np1)*ynpka(np1)+  yn(np1)*jnpka(np1))*an(np1)*(-np1*sin(np1*theta))
           pm=-pm
 
           if( n.eq.(nterm-4) )then
             sr0=sr
             srr0=srr
             srt0=srt
 
             si0=si
             sir0=sir
             sit0=sit
           end if
         end do
         ! check the size of the last terms
         sMax = max(sMax,max(abs(sr-sr0),max(abs(srr-srr0),abs(srt-srt0))))
         sMax = max(sMax,max(abs(si-si0),max(abs(sir-sir0),abs(sit-sit0))))
 
 
         sr=-2.*sr
         srr=-2.*srr*k   ! note factor k from Dr( Jn(k*r) )
         srt=-2.*srt
 
         si=-2.*si
         sir=-2.*sir*k   ! note factor k from Dr( Jn(k*r) )
         sit=-2.*sit
 
         rx = x/r  ! r.x 
         ry = y/r
         tx=-sin(theta)/r  ! d(theta)/dx
         ty= cos(theta)/r  ! d(theta)/dy
 
         ! Ex.t=(Hz).y =>  -i*k*Ex = (Hz).y -> Ex = i (Hz).y/k  -> Re(Ex) = -Im(Hz.y)/k  Im(Ex) = Re(Hz.y)
         ! k*Ey =-i*(Hz).x
 
         if( staggeredGrid.eq.0 )then
           ! node centered values 
           u(i1,i2,i3,hzr)=sr
           u(i1,i2,i3,exr)=-dir*(ry*sir+ty*sit)/k  
           u(i1,i2,i3,eyr)= dir*(rx*sir+tx*sit)/k  
  
           u(i1,i2,i3,hzi)=si
           u(i1,i2,i3,exi)= dir*(ry*srr+ty*srt)/k  
           u(i1,i2,i3,eyi)=-dir*(rx*srr+tx*srt)/k  
         else 
           if( edge.eq.1 )then
             ! Ex lives on this edge
            u(i1,i2,i3,exr)=-dir*(ry*sir+ty*sit)/k
            u(i1,i2,i3,exi)= dir*(ry*srr+ty*srt)/k  
           else if( edge.eq.2 )then
             ! Ey lives on this edge
            u(i1,i2,i3,eyr)= dir*(rx*sir+tx*sit)/k  
            u(i1,i2,i3,eyi)=-dir*(rx*srr+tx*srt)/k  
           else
             ! Hz lives at the cell center 
             u(i1,i2,i3,hzr)=sr
             u(i1,i2,i3,hzi)=si
           end if
         end if

         ! rotate to the specfied direction of the incident wave (kx,ky) 
 
         !  u(i1,i2,i3,exr)= fexr
         !  u(i1,i2,i3,eyr)= feyr
         !  u(i1,i2,i3,exi)= fexi
         !  u(i1,i2,i3,eyi)= feyi
 
         
       end do  ! end i1
       end do  ! end i2 
 
       write(*,'(" >>>scatcyl: nterm=",i3," largest final term sMax=",e10.2)') nterm,sMax
       ! '
  
      end do ! edge 

      return
      end

c =============================================================================
c Macro to Evaluate the fields scattered by a dielectrix cylinder
c  CASE (input): exterior ot interior
c =============================================================================

#beginMacro dielectricScatteringMacro(CASE)
 ! precompute some coefficients
 do n=0,nterm-1
   ! H = H^(2) = J - i Y
   hnc = cmplx(jnka(n),-ynka(n))
   hnpc= cmplx(jnpka(n),-ynpka(n))
   ! Since H becomes large as n gets large, form the ratio Hn/Hn'  (to avoid cancellation)
   hr = hnc/hnpc
   detc = am*jnmka(n)-hr*jnpmka(n)
   detic = 1./(am*jnmka(n)-hr*jnpmka(n))
   #If #CASE eq "exterior"
     an(n)= (jnka(n)*jnpmka(n)-jnpka(n)*jnmka(n)*am)*detic/hnpc
   #Else
     an(n)= ( -2.*ai*am/(pi*ka) )*detic/hnpc
   #End
 !   write(*,'(" n=",i2," h=(",2e10.2,") hp=(",2e10.2,")  hr=(",2e10.2,") detc=",2e10.2," detic=",2e10.2," an=",2e10.2)') n,real(hnc),aimag(hnc),real(hnpc),aimag(hnpc),real(hr),aimag(hr),real(detc),aimag(detc),real(detic),aimag(detic),real(an(n)),aimag(an(n))

   aa = 2.*am/( am**n*(am+1./am))  ! asymptotic formula
    ! write(*,'(" n=",i2," h=(",2e10.2,") an=",2e10.2," an/asymp=",2e10.2)') n,real(hnc),aimag(hnc)\
     ,real(an(n)),aimag(an(n)),real(an(n)/aa),aimag(an(n)/aa)

  #If #CASE eq "interior"
   !  if( aa.gt.1e8 )then
   !    an(n)=cmplx(aa,0.)
   !  end if
  #End

 end do
 ! write(*,'(" an=",50(2e10.2,2x)') (real(an(n)),aimag(an(n)),n=1,nterm-1)


 numEdges=1
 if( staggeredGrid.eq.1 )then
  numEdges=3
  n1b=min(n1b,nd1b-1)
  n2b=min(n2b,nd2b-1)
 end if

 do edge=1,numEdges  ! for a staggered grid we need to evaluate along edges and the cell-centers 


 sMax=0. ! keep track of the size of the last term for monitoring convergence

 i3=n3a
 do i2=n2a,n2b
 do i1=n1a,n1b
   
   x=xy(i1,i2,i3,0)  
   y=xy(i1,i2,i3,1)
   if( staggeredGrid.eq.1 )then
     if( edge.eq.1 )then
       ! Ex lives on this edge
       x=.5*(xy(i1,i2,i3,0)+xy(i1+1,i2,i3,0) )
     else if( edge.eq.2 )then
       ! Ey lives on this edge
       y=.5*(xy(i1,i2,i3,1)+xy(i1,i2+1,i3,1) )
     else
       ! Hz lives at the cell center 
       x=.5*(xy(i1,i2,i3,0)+xy(i1+1,i2,i3,0) )
       y=.5*(xy(i1,i2,i3,1)+xy(i1,i2+1,i3,1) )
     end if
   end if

   r=sqrt(x*x+y*y)
   !  don't allow r to get too small -- not valid and convergence is poor
   if( r.lt. rMin )then
     if( r.lt.eps )then
       x=eps  ! avoid atan(0,0)
       y=eps
     end if
     r=rMin  !  don't allow r to get too small -- not valid and convergence is poor
   end if

   theta=atan2(y,x)
   ! if( theta.lt.0. ) then
   !   theta=theta+twoPi
   ! end if
   ! write(*,'(" i=",2i4," x,y,r,theta=",4f10.5)') i1,i2,x,y,r,theta

   ! eval Jn(kk*r)
   kr=kk*r
   call rjbesl(kr, alpha, nb, jn, ncalc)
   call rybesl(kr, alpha, nb, yn, ncalc)
   ! derivatives:
   jnp(0) = -jn(1)
   ynp(0) = -yn(1)
   do n=1,nterm
     jnp(n) = .5*( jn(n-1)-jn(n+1) )
     ynp(n) = .5*( yn(n-1)-yn(n+1) )
   end do
   
   ! compute the hz field: s and it derivatives sr, sTheta
   !   (sr,si) : holds the Re an Im parts of Hz
   !   srr = d(sr)/dr  srt=d(sr)/d(theta)
   sr=0.

   ! n=0: 
   n=0
   aimn=1.  ! ai**(0)

   expc=cmplx(cos(n*theta),sin(n*theta))

   ! ---- exterior
   #If #CASE eq "exterior"
     hnc=cmplx(jn(n),-yn(n))
     hnpc=cmplx(jnp(n),-ynp(n))
     sc = (aimn*inc*jn(n) + an(n)*hnc)*expc
     scr= (aimn*inc*jnp(n)+ an(n)*hnpc)*expc
   #Else
    ! --- interior
    sc = an(n)*jn(n)*expc
    scr= an(n)*jnp(n)*expc
   #End

   sr=real(sc)
   srr=real(scr)
   srt=0.
   
   si=aimag(sc)
   sir=aimag(scr)
   sit=0.

   aimn=1. ! aimn = (i)^{-n} = (-i)^n
   ain=1.  ! ain  = (i)^{n} 
   do n=1,nterm-1
     if( mod(n,4).eq.0 )then
       aimn=1.
       ain=1.
     else
       aimn=-aimn*ai
       ain=ain*ai
     end if

     cnt=cos(n*theta)
     snt=sin(n*theta)

     expc=cmplx(cnt,snt)
     exptc=n*cmplx(-snt,cnt)

     expmc=cmplx(cnt,-snt)
     expmtc=n*cmplx(-snt,-cnt)

     #If #CASE eq "exterior"
       hnc=cmplx(jn(n),-yn(n))
       hnpc=cmplx(jnp(n),-ynp(n))
       sc = (inc*jn(n) + an(n)*hnc )*aimn*expc + (inc*jn(n) + an(n)*hnc )*aimn*expmc
       scr= (inc*jnp(n)+ an(n)*hnpc)*aimn*expc + (inc*jnp(n)+ an(n)*hnpc)*aimn*expmc
       sct= (inc*jn(n) + an(n)*hnc )*aimn*exptc+ (inc*jn(n) + an(n)*hnc )*aimn*expmtc
     #Else
       sc = an(n)*jn(n) *(aimn*expc +aimn*expmc)
       scr= an(n)*jnp(n)*(aimn*expc +aimn*expmc)
       sct= an(n)*jn(n) *(aimn*exptc+aimn*expmtc)
     #End
     ! write(*,'("i1,i2=",2i3," n=",i2," sc=",2f6.3)') i1,i2,n,real(sc),aimag(sc)

     sr=sr  +real(sc)
     srr=srr+real(scr)
     srt=srt+real(sct)
     
     si=si  +aimag(sc)
     sir=sir+aimag(scr)
     sit=sit+aimag(sct)

     if( n.eq.(nterm-2) )then
       sr0=sr
       srr0=srr
       srt0=srt

       si0=si
       sir0=sir
       sit0=sit
     end if
   end do
   ! check the size of the last terms
   sMax = max(sMax,max(abs(sr-sr0),max(abs(srr-srr0),abs(srt-srt0))))
   sMax = max(sMax,max(abs(si-si0),max(abs(sir-sir0),abs(sit-sit0))))

   sr = sr 
   srr= srr*kk   ! note factor kk from Dr( Jn(kk*r) )
   srt= srt
        
   si = si 
   sir= sir*kk   ! note factor kk from Dr( Jn(kk*r) )
   sit= sit

   rx = x/r  ! r.x 
   ry = y/r
   tx=-sin(theta)/r  ! d(theta)/dx
   ty= cos(theta)/r  ! d(theta)/dy

   ! Ex.t=(Hz).y =>  -i*k*Ex = (Hz).y -> Ex = i (Hz).y/k  -> Re(Ex) = -Im(Hz.y)/k  Im(Ex) = Re(Hz.y)
   ! k*Ey =-i*(Hz).x

   ! ***** fix this for exterior/interior ******

   #If #CASE eq "exterior"
     kkm=kk
   #Else
     kkm=kk*am
   #End 

   if( staggeredGrid.eq.0 )then
     ! node centered values 
     ! *wdh* 090515 - H should be -H (didn't matter for SOS but does for FOS)
    u(i1,i2,i3,hzr)=-dir*sr                        
    u(i1,i2,i3,hzi)=-si                        

    u(i1,i2,i3,exr)=-dir*(ry*sir+ty*sit)/kkm  
    u(i1,i2,i3,exi)= (ry*srr+ty*srt)/kkm

    u(i1,i2,i3,eyr)= dir*(rx*sir+tx*sit)/kkm
    u(i1,i2,i3,eyi)=-(rx*srr+tx*srt)/kkm
 
    #If #CASE eq "exterior"
     u(i1,i2,i3,hzr)= u(i1,i2,i3,hzr) - dir*(1.-inc)*cos(k*x)
     u(i1,i2,i3,hzi)= u(i1,i2,i3,hzi) +     (1.-inc)*sin(k*x)

     u(i1,i2,i3,eyr)= u(i1,i2,i3,eyr) - dir*(1.-inc)*cos(k*x)
     u(i1,i2,i3,eyi)= u(i1,i2,i3,eyi) +     (1.-inc)*sin(k*x)
    #End

   else 
     if( edge.eq.1 )then
       ! Ex lives on this edge
      u(i1,i2,i3,exr)=-dir*(ry*sir+ty*sit)/kkm  
      u(i1,i2,i3,exi)= (ry*srr+ty*srt)/kkm
     else if( edge.eq.2 )then
       ! Ey lives on this edge
      u(i1,i2,i3,eyr)= dir*(rx*sir+tx*sit)/kkm
      u(i1,i2,i3,eyi)=-(rx*srr+tx*srt)/kkm
      #If #CASE eq "exterior"
       u(i1,i2,i3,eyr)= u(i1,i2,i3,eyr) - dir*(1.-inc)*cos(k*x)
       u(i1,i2,i3,eyi)= u(i1,i2,i3,eyi) + (1.-inc)*sin(k*x)
      #End
     else
       ! Hz lives at the cell center 
      u(i1,i2,i3,hzr)=-dir*sr                        
      u(i1,i2,i3,hzi)=-si                        
      #If #CASE eq "exterior"
       u(i1,i2,i3,hzr)= u(i1,i2,i3,hzr) - dir*(1.-inc)*cos(k*x)
       u(i1,i2,i3,hzi)= u(i1,i2,i3,hzi) +     (1.-inc)*sin(k*x)
      #End

     end if
   end if


 end do
 end do

 write(*,'(" >>>scatDielectricCyl: nterm=",i3," largest final term sMax=",e10.2)') nterm,sMax
 ! ' 
 end do ! edge

#endMacro

c ============================================================================================
c Compute the field of electromagnetic scattering from a *dielectric* cylinder
c Both the field exterior and the field interior to the cylinder can be computed
c          a : radius of the cylinder
c          k : wavelength of the incident light
c
c  This solution is taken from 
c    Balanis, "Advanced Engineering Eletromagnetics", p666 (problem 11.27)
c
c  The solution is for an incident wave traveling in the positive x-direction 
c                     Hz =     exp(i(k*x-w*t))
c                     Ey = -Z* exp(i(k*x-w*t))
c
c
c  nd : =2 number of space dimensions
c  n1a:n1b, n2a:n2b, n3a:n3b : evaluate solution at these points, i1=n1a..n1b, 
c  nd1a:nd1b,... : dimensions of u, xy
c  xy : grid points 
c
c  rpar(0) : k
c  rpar(1) : a 
c  rpar(2) : m 
c
c  ipar(0) = exr : store the Re part of Ex here
c  ipar(1) = eyr : store the Re part of Ey here
c  ipar(2) = hzr : store the Re part of Hz here
c  ipar(3) = exi : store the Im part of Ex here
c  ipar(4) = eyi : store the Im part of Ey here
c  ipar(5) = hzi : store the Im part of Hz here
c
c  ipar(6) : option : 0=no dielectric, 1=dielectric
c  ipar(7) : inOut : 0=exterior, 1=interior
c =============================================================================================
      subroutine scatDielectricCyl(nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
                         xy,u,ipar,rpar )

      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)

      integer ipar(0:*)
      real rpar(0:*)

c.............local variables
      integer ntermMax
      parameter( ntermMax=50 )
      integer i1,i2,i3,nterm,ncalc,nb,exr,eyr,hzr,exi,eyi,hzi,staggeredGrid,debug
      real k,a,ka,r,theta,kr,x,y,alpha,twoPi,pi
      real jnka(0:ntermMax),ynka(0:ntermMax),jnpka(0:ntermMax),ynpka(0:ntermMax)
      real jn(0:ntermMax),yn(0:ntermMax),jnp(0:ntermMax),ynp(0:ntermMax)

      real s,sr,si,srr,srt,sir,sit,rx,ry,tx,ty,pm,cnt,cnp1t,sMax,eps,dir
      real sr0,srr0,srt0,si0,sir0,sit0
      integer n,np1

      integer numEdges,edge
      integer outside,inside
      parameter( outside=0, inside=1)
      integer option,inOut
      real jnmka(0:ntermMax),jnpmka(0:ntermMax),ynmka(0:ntermMax),ynpmka(0:ntermMax)
      real am,mka,snt,kk,rMin,aa,kkm,inc
      complex*16 ai,hnc,hnpc,detc,detic,aimn,expc,exptc,sc,scr,sct,ain,expmc,expmtc,hr
      complex*16 an(0:ntermMax)

c...............end local variables      

      k = rpar(0)  !
      a = rpar(1)  ! radius
      am= rpar(2)  ! "m" ratio of dielectrics

      exr=ipar(0)
      eyr=ipar(1)
      hzr=ipar(2)
      exi=ipar(3)
      eyi=ipar(4)
      hzi=ipar(5)
      option=ipar(6) ! should be 1
      inOut=ipar(7)  ! 0 = outside, 1=inside

      staggeredGrid=ipar(9)
      debug= ipar(10)

      ka=k*a
      mka=am*k*a
      ai=cmplx(0.,1.)  ! i 

      inc=0.  ! do not compute incident wave by a series

      write(*,'(" scatcyl: k,a,m=",3f10.6," exr,eyr,hzr=",3i3)') k,a,am,exr,eyr,hzr
      if( ka.le.0 )then
        stop 12233
      end if

      ! I estimate that the number of terms, N, should satisfy
      !        ??? N * eps**(1/N) > e*k*a/2 * 1/(2*pi)**(1/N)
      ! where eps=size of the final term (series is alternating)
      ! Take N = max( e*k*a, log(1/eps) )

      eps = 1.e-16  ! ********** fix this  -- should be REAL_EPSILON
      nterm = max( abs(7.*ka), 16. )

      nterm=min(nterm,30)  ! do this for now ****

      nterm=min(nterm,ntermMax-2)
      ! ** nterm = nterm - mod(nterm,2) + 1   ! nterm should be odd
      ! nterm = 25  ! nterm should be odd
      twoPi=atan2(1.,1.)*8.
      pi=twoPi*.5

      ! First evaluate Jn(ka), Yn(ka) n=0,1,...,nterm
      alpha=0. ! fractional part of Bessel order

      nb = nterm+1  ! eval J0, J1, ... J(nb)  -- compute one extra 
      call rjbesl(ka, alpha, nb, jnka, ncalc)
      call rybesl(ka, alpha, nb, ynka, ncalc)
      ! also eval Jn(mka), Yn(mka) 
      call rjbesl(mka, alpha, nb, jnmka, ncalc)
      call rybesl(mka, alpha, nb, ynmka, ncalc)

      ! compute the derivatives 
      
      jnpka(0) = -jnka(1)
      ynpka(0) = -ynka(1)
      jnpmka(0) = -jnmka(1)
      ynpmka(0) = -ynmka(1)
      do n=1,nterm
        jnpka(n) = .5*( jnka(n-1)-jnka(n+1) )
        ynpka(n) = .5*( ynka(n-1)-ynka(n+1) )

        jnpmka(n) = .5*( jnmka(n-1)-jnmka(n+1) )
        ynpmka(n) = .5*( ynmka(n-1)-ynmka(n+1) )
      end do

      dir=-1.
      if( inOut.eq.outside )then
        kk=k
        rMin=.75*a
        dielectricScatteringMacro(exterior)
      else
        kk=am*k
        rMin=eps*sqrt(2.)  ! need sqrt(2.) for correct evaluation at origin *wdh* 061008
        dielectricScatteringMacro(interior)
      end if

      write(*,'(" >>>inOut=",i3," rMin=",e10.2)') inOut,rMin

      return
      end

