c
c NURBS evaluation utilities
c
c Reference: Piegl, Les and Tiller, Wayne, "The NURBS Book", second edition
c
c Notes: - The functions have been generalized to work on tensor products
c          so that we don't need different functions for curves, surfaces
c          and volumes. 
c          XXX  I got tired and implemented the derivatives for max rdim=3 .
c            the implementation is still generic in the sense that we don't have
c            separate code for each dimension <=3.
c           
c        - This nurbs implementation has been done in Fortran so that
c          the multidimensional loops used are fast and readable.
c
c        - MAXORDER and MAXDIM are two parameters that appear in multiple
c          subroutines.  Currently,
c                        MAXORDER = 20
c                        MAXDIM   = 4
c          If you want a higher order B-Spline or a higher homogeneous
c          coordinate dimension these need to be changed in 
c          dersbasisfuns
c          - and -
c          nurbseval 
c          - and -
c          nurbsiso
c          - and -
c          nurbsinterp
c          - and -
c          kk_nurbs.hh
c          
c        - Note that Piegl and Tiller don't really discuss higher dimensional
c          NURBS algorithms (higher than surfaces).  Since I had to derive
c          the more general algorithms there still needs to be extensive testing.
c
c        - The C preprocessor variable KK_NRB_BLOCKED switched the index for 
c          dimensioned arrays.  If defined(KK_NRB_BLOCKED) then arrays will look
c          like cpts(ncp, rdim), otherwise they will be cpts(rdim, ncp).
c
c Initial Version: 040308 kkc
c

c
c bico computes the binomial coeffiient n!/(k!(n-k)!) using the log
c      of the gamma function.  This comes from yorick's implementation
c      of bico
      function bico(n,k)

      implicit none

      integer n,k, bico

      real lngam, x

c     statement function for the log of the gamma function, from yorick
c            (if you are copying this, yorick's comment says the following
c             method for log(Gamma(x)) is accurate to 2e-10 only for x>1).
      lngam(x) =  -( x+4.5-(x-0.5)*log(x+4.5) ) 
     .        + log( 2.50662827465* 
     .     (1.0 + 76.18009173/x - 86.50532033/(x+1.) + 
     .      24.01409822/(x+2.) -
     .     1.231739516/(x+3.) + 0.120858003e-2/(x+4.) - 
     .     0.536382e-5/(x+5.) ) )

c     yorick uses floor here but I use the conversion to integer
c            to truncate the floating point number
      bico = 0.5+exp(lngam(n+1.)-lngam(k+1.)-lngam(n-k+1.))

      return
      end

c      
c

c
c findspan determines the knot interval spanning the parameter coordinate u
c
      function findspan(n,p,u,knts)

c     - findspan implements Algorithm A2.1 
c     - this only needs to work on 1D arrays since it will
c       be called once for each direction of a tensor product evaluation

      implicit none

      integer findspan
      integer n,p
      real knts(0:*),u

      integer low, high, mid

      real tmp

      tmp = abs(knts(n+1)-u)
c      if ( u.eq.knts(n+1) ) then
      if ( (1.+tmp).eq.1. ) then
         findspan = n
         return
      else if ( u.lt.knts(0) ) then
         findspan = p
         return
      else if ( u.gt.knts(n+1) ) then
         findspan = n
         return
      endif

      low  = p
      high = n+1
      mid  = (low+high)/2

c      print *,"  ",low,mid,high
      do while ( (u.lt.knts(mid)) .or. (u.ge.knts(mid+1)) )

         if ( u.lt.knts(mid) ) then
            high = mid
         else
            low = mid
         endif

         mid = (low+high)/2

      end do

      findspan = mid

      return 
      end

c
c dersbasisfuns evaluates the nonvanishing basis functions and
c               thier derivatives.  It has been modified to only
c               compute derivatives up to order nder (i.e. one does
c               not need to compute the derivatives at all).
c
      subroutine dersbasisfuns(span, u, p, n, nder, knts, bfuns)
      
      implicit none

      integer MAXORDER
      parameter(MAXORDER=20)

      integer span, p, n, nder
      real u,knts(0:*),bfuns(0:MAXORDER,0:*)

c     temporaries, ndu stores knot differences
      real ndu(0:MAXORDER-1,0:MAXORDER-1)
      real a(0:MAXORDER-1,0:MAXORDER-1)
      real temp2,d,temp

c     index and loop variables
      integer j,j1,j2,k,s1,s2,r,rk,pk

c     statement functions used to avoid additional arrays
      real left, right
      integer l
      left(l)  = u - knts(span+1-l)
      right(l) = knts(span+l) - u
      
c     compute the basis functions and knot differences
      ndu(0,0) = 1.

      do j=1,p

         temp2 = 0.

         do r=0,j-1
            ndu(j,r) = right(r+1)+left(j-r)
            temp = ndu(r,j-1)/ndu(j,r)

            ndu(r,j) = temp2+right(r+1)*temp
            temp2 = left(j-r)*temp
         end do
         ndu(j,j) = temp2

      end do

c     actually set the basis functions here
      do j=0,p
         bfuns(0,j) = ndu(j,p)
c         print *,"ndu ",ndu(j,p)
      end do

      if ( nder.eq.0 ) return

c     now compute the dervivatives (Eq. 2.9)
      do r=0,p

         s1=0
         s2=1
         a(0,0) = 1.0
         
         do  k=1,nder
            
            d = 0.
            rk = r-k
            pk = p-k

            if ( r.ge.k ) then
               a(s2,0) = a(s1,0)/ndu(pk+1,rk)
               d = a(s2,0)*ndu(rk,pk)
            end if

            if ( rk.ge.-1 ) then
               j1 = 1
            else
               j1 = -rk
            endif

            if ( r-1.le.pk ) then
               j2 = k-1
            else
               j2 = p-r
            endif

            do j=j1,j2
               a(s2,j) = (a(s1,j)-a(s1,j-1))/ndu(pk+1,rk+j)
               d = d + a(s2,j)*ndu(rk+j,pk)
            end do

            if ( r.le.pk ) then
               a(s2,k) = -a(s1,k-1)/ndu(pk+1,r)
               d = d + a(s2,k)*ndu(r,pk)
            end if

            bfuns(k,r) = d
            j  = s1
            s1 = s2
            s2 = j
            
         end do

      end do

      r = p
      do k=1, nder
         do j=0,p
            bfuns(k,j) = bfuns(k,j)*r
         end do
         r = r*(p-k)
      end do

      return
      end 

c     
c     nurbseval evaluates the NURBS curve and its derivatives (if requested)
c               at positions in parameter space given by u(ddim,*).  This 
c               subroutine generalizes the single point curve/surface/volume
c               coordinate and derivative evaluation algorithms found in The NURBS Book.
c               Since NURBS surfaces and volumes (and higher...) are
c               tensor products we can write one routine (more complex) to do
c               them all. (CAVEAT - the last part is not completely general and
c               will have to be modified to handle ddim>3 NURBS things (hypervolumes?).
c
c               To accomodate computing the first index bounds on cptsw, cpbnds
c               should always be MAXDIM long with 0 in dimensions that are not used
c
      subroutine nurbseval(nder,ddim,rdim,nn,p,knts,kntbnds,
     .                     cptswt,cpbnds,
     .                     ut,nu,
     .                     derst)
 
      implicit none

      integer MAXORDER
      parameter(MAXORDER=20)
      integer MAXDIM
      parameter(MAXDIM=4)

c     intput parameters
      integer nder, ddim, rdim, nn(0:*), p(0:*), nu
      integer kntbnds(0:1,0:*), cpbnds(0:*)
      real knts(0:*)
#ifndef KK_NRB_BLOCKED
      real ut(0:ddim-1,0:*), cptswt(0:rdim,0:*)
#else
      real ut(0:nu-1,0:*)
      real cptswt(
     .          0:(cpbnds(0)+1)*
     .          (cpbnds(1)+1)*(cpbnds(2)+1)*(cpbnds(3)+1)-1,0:*)
#endif
      
c     output 
c     derivatives will be stored in an array of rank ddim+1
c                 - the first index will be the rdim coordinate position
c                 - the remaining indices will be of length nder
c                 - in 2D, for example, S_{uu} = ders(:,2,0,iu), S_{uv} = ders(:,1,1,iu), etc.
c                 - the coordinate is stored in ders(:,0,0,iu)
#ifndef KK_NRB_BLOCKED
      real derst(0:rdim-1,0:((nder+1)**ddim-1),0:*) 
#else
      real derst(0:nu-1,0:((nder+1)**ddim-1),0:*) 
#endif

c     these functions are defined at the top of the file
      integer findspan, bico
      external findspan
      external bico

c     temporaries
c                      tmp hold coordinates and derivatives of the homogeneous-space
c                          non-rational spline
      real tmp(0:MAXDIM-1,0:((MAXORDER+1)**MAXDIM-1))
      real ud, bfunfac, w, bimn, bilj, biki
      real bfuns(0:MAXORDER, 0:MAXORDER, 0:MAXDIM-1)

      integer i,j,k,l,m,n,iu,ider,d,span(0:MAXDIM-1),idxcnt(0:MAXDIM-1)
      integer idercnt(0:MAXDIM-1)
      integer tp(0:MAXDIM-1),tn(0:MAXDIM-1),idert,r

c     statement functions to compute index offsets
      integer cidt,cid
      cidt(k,l,m) = k+(MAXORDER+1)*((l+m*(MAXORDER+1)))
      cid(k,l,m)  = k+(nder+1)*(l+m*(nder+1))

c     macros to abstract out the blocked/interleaved orderings
#ifdef KK_NRB_BLOCKED
#define cptsw(i,j)  cptswt(j,i)
#define u(i,j)      ut(j,i)
#define ders(i,j,k) derst(k,j,i)
#else
#define cptsw(i,j)  cptswt(i,j)
#define u(i,j)      ut(i,j)
#define ders(i,j,k) derst(i,j,k)
#endif

      do d=0, MAXDIM-1
         span(d) = 0
         tp(d) = 0
         tn(d) = 0
         if ( d.lt.ddim ) then
            tp(d) = p(d)
            tn(d) = nn(d)
         end if
           
      end do

c     for each parameter value in u, compute the coordinate and derivatives
      do iu = 0, nu-1

         do d=0,ddim-1
            ud = u(d, iu)
c        find out where we are in the knot index space
c            print *,d,nn(d),p(d),ud,ut(iu, d)
            span(d) = findspan(nn(d), p(d), ud, knts(kntbnds(0,d)))
c            print *,"span = ",span(d)
c        determine the basis functions and thier derivatives in each direction
            call dersbasisfuns(span(d),ud,p(d),nn(d),nder,
     .                         knts(kntbnds(0,d)),bfuns(0,0,d))
         end do

         do d=0,MAXDIM-1

            do n=0, MAXDIM-1
               idercnt(n) = 0
            end do
            
            do while (idercnt(0).le.nder)
c               ider = idercnt(0) + 
c     .             (MAXORDER+1)*((idercnt(1) + (MAXORDER+1)*idercnt(2)))
               ider = cidt(idercnt(0),idercnt(1),idercnt(2))

c              this is all this loop is doing!
               tmp(d,ider) = 0.

               idercnt(ddim-1) = idercnt(ddim-1) + 1
               do n=2, ddim
                  if ( idercnt(ddim-n+1).gt.nder) then !p(ddim-n+1) ) then
                     idercnt(ddim-n+1)=0
                     idercnt(ddim-n) = idercnt(ddim-n) + 1
                  end if
               end do
               
            end do

         end do

         do d=0,MAXDIM-1
            idercnt(d) = 0
         end do

c        this loops over all the derivatives requested (including no derivative)
c        This section only computes the coordinate and the HOMOGENEOUS spline derivatives.
c        The rational spline derivatives (what you actually what) are computed in the next section.
         do while (idercnt(0).le.nder)
c           compute this derivative's index in tmp
c                   XXX We have assumed MAXDIM=4 here !!!
c            ider = idercnt(0) + 
c     .             (MAXORDER+1)*((idercnt(1) + (MAXORDER+1)*idercnt(2)))
            ider = cidt(idercnt(0),idercnt(1),idercnt(2))
     
c            print *,"ider = ",ider

c           initialize the "loop" counters
            do d=0,MAXDIM-1
               idxcnt(d) = 0
            end do

c           here is the actual tensor product
            do while (idxcnt(0).le.p(0))

c              do this index computation since the rank of cptsw is
c                 determined by the domain dimension (could always assume MAXDIM?)
c              XXX add more terms if MAXDIM increases !
               i =  span(0)-tp(0)+idxcnt(0) + 
     .              (cpbnds(0)+1)*((span(1)-tp(1)+idxcnt(1) + 
     .              (cpbnds(1)+1)*(span(2)-tp(2)+idxcnt(2))))


c               print *,idxcnt(0),idxcnt(1),idxcnt(2)
c               print *,idercnt(0),idercnt(1),idercnt(2)
c               print *," i = ",i

c              compute the product of the basis functions currently being evaluated
               bfunfac = bfuns(idercnt(0),idxcnt(0),0)
               do d=2, ddim
                  bfunfac = bfunfac*bfuns(idercnt(d-1),idxcnt(d-1),d-1)
               end do
c               print *,"bf = ",bfunfac
               do r=0, rdim
                  tmp(r,ider) = tmp(r,ider) + bfunfac*cptsw(r, i)
               end do

c              increment the knot indicies
               idxcnt(ddim-1) = idxcnt(ddim-1) + 1
               do d=2, ddim
                  if ( idxcnt(ddim-d+1).gt.p(ddim-d+1) ) then
c                     print *, "incrementing ",ddim-d,", ",
c     .                    idxcnt(ddim-d+1), d, idxcnt(ddim-d)
                     idxcnt(ddim-d+1)=0
                     idxcnt(ddim-d) = idxcnt(ddim-d) + 1
                  end if
               end do

            end do ! do while idxcnt

c     increment derivative indices
            idercnt(ddim-1) = idercnt(ddim-1) + 1
            do d=2, ddim
               if ( idercnt(ddim-d+1).gt.nder ) then !p(ddim-d+1) ) then
                  idercnt(ddim-d+1)=0
                  idercnt(ddim-d) = idercnt(ddim-d) + 1
               end if
            end do

         end do                 ! do while idercnt

c     XXX
c     yeah, so, I got lazy and wrote the
c         ddim=3 version to work for lower ddim
c         NURBS.  NO, it's not a general tensor product,
c         you want to make something of it or what?

         w = 1./tmp(rdim,0)

c        This section computes the coordinates and derivatives of the
c             rational NURBS from the non-rational polynomial data in
c             the homogeneous space.  The coordinates are computed when
c             k=l=m=0.
         do k=0,min(tn(0),nder)
            do l=0,min(tn(1),nder)
               do m=0,min(tn(2),nder)
cccccccccccccccccccccccccccccccc
ccc FOR EACH DERIVATIVE


                  idert = cidt(k,l,m)
                  ider  = cid(k,l,m)
c                  print *,"k,l,m,ider,idert ",k,l,m,ider,idert
c                  do r=0,rdim-1
c                     print *,"  ders(ider),tmp(idert) ",ders(r,ider,iu),
c     .                    tmp(r,idert)
c                  end do

c        first set the derivatives to Pw_klm (we later subtract off the rest)
                  do r=0,rdim-1
                     ders(r,ider,iu) = w*tmp(r,idert)
                  end do

c                  do r=0,rdim-1
c                    print *,"  /ders(ider),tmp(idert) ",ders(r,ider,iu),
c     .                    tmp(r,idert)
c                  end do

c        subtract off the remaining sums (see notes)
                  do i=1,min(tn(0),k)
                     biki = bico(k,i)

c                     print *, "  i ",i
c                    SUM 1 (from notes)
                     do r=0, rdim-1
                     ders(r,ider,iu) = ders(r,ider,iu) - 
     .                          biki*w*
     .                  tmp(rdim,cidt(i,l,m))*ders(r,cid(k-i,l,m),iu)
                     end do

                     do j=1,min(tn(1),l)
                        bilj = bico(l,j)

c                        print *, "    i,j ",i,j
c                    SUM 2 (from notes)
                        do r=0, rdim-1
                        ders(r,ider,iu) = ders(r,ider,iu) -
     .                        biki*bilj*w*
     .                tmp(rdim,cidt(i,j,m))*ders(r,cid(k-i,l-j,m),iu)
                        end do

                        do n=1,min(tn(2),m)
                           bimn = bico(m,n)
                           
c                    SUM 3 (from notes)
                           do r=0, rdim-1
                              ders(r,ider,iu) = ders(r,ider,iu) -
     .                             biki*bilj*bimn*w*
     .                             tmp(rdim,cidt(i,j,n))*
     .                            ders(r,cid(k-i,l-j,m-n),iu)
                           end do
                           
                        end do  ! n
                     end do ! j 

                     do n=1,min(tn(2),m)
                        bimn = bico(m,n)
c                    SUM 4 (from notes)
                        do r=0,rdim-1
                        ders(r,ider,iu) = ders(r,ider,iu) -
     .                        biki*bimn*w*
     .                 tmp(rdim,cidt(i,l,n))*ders(r,cid(k-i,l,m-n),iu)
                        end do

                     end do

                  end do ! i

                  do j=1,min(tn(1),l)
                     bilj = bico(l,j)

c                     print *, "  j ",j

c                    SUM 5
                     do r=0,rdim-1
                     ders(r,ider,iu) = ders(r,ider,iu) -
     .                    bilj*w*
     .                  tmp(rdim,cidt(k,j,m))*ders(r,cid(k,l-j,m),iu)
                     end do

                     do n=1,min(tn(2),m)
                        bimn = bico(m,n)
c                    SUM 6
                        do r=0,rdim-1
                        ders(r,ider,iu) = ders(r,ider,iu) -
     .                      bilj*bimn*w*
     .                 tmp(rdim,cidt(k,j,n))*ders(r,cid(k,l-j,m-n),iu)
                        end do

                     end do !n

                  end do ! j

                  do n=1,min(tn(2),m)
                     bimn = bico(m,n)
c                    SUM 7
                     do r=0, rdim-1
                     ders(r,ider,iu) = ders(r,ider,iu) -
     .                    bimn*w*
     .                  tmp(rdim,cidt(k,l,n))*ders(r,cid(k,l,m-n),iu)
                     end do

                  end do !n

ccc END FOR EACH DERIVATIVE
ccccccccccccccccccccccccccccccccc                  

               end do ! m
            end do ! l
         end do ! k

c         print *,"for iu=",iu," : ",c1,c2
c         print *,"---"
      end do                    ! do iu=0,nu-1

#undef cptsw
#undef u
#undef ders

      return 
      end

#ifndef KK_NRB_BLOCKED
c
c     nurbsiso extracts the homogeneous control points for an iso-curve or surface
c
      subroutine nurbsiso(ddim,
     .                    rdim,
     .                    dir,
     .                    isou,
     .                    p,
     .                    knts,
     .                    kbnds,
     .                    cptsw,
     .                    cpbnds,
     .                    cpwi)

      implicit none

      integer MAXORDER
      parameter(MAXORDER=20)
      integer MAXDIM
      parameter(MAXDIM=4)

      integer ddim,rdim,dir,p(0:*),kbnds(0:1,0:*),cpbnds(0:*)
      real isou, knts(0:*)
      real cptsw(0:rdim,0:*),cpwi(0:rdim,0:*)

      integer findspan
      external findspan

c     temporaries
      integer idx(0:MAXDIM-1)
      real bfuns(0:MAXORDER,0:MAXORDER)

      integer r,i,ii,j,k,span,d1,d2
      integer ci,cii
      cii(i,j) = i + j*(cpbnds(d1)+1)
      ci(i,j,k) = i + (cpbnds(0)+1)*(j+(cpbnds(1)+1)*k)
      
      d1 = mod( dir+1, ddim )
      d2 = mod( dir+2, ddim )
      if ( ddim.eq.2 ) d2=3 !d2 is set to 3 to get it out of the way 

      do k=0, MAXDIM-1
         idx(k) = 0
      end do

      do while(idx(d1).le.cpbnds(d1))

         ii = cii( idx(d1), idx(d2) )

c        this is all this while loop is doing!
         do r=0,rdim
            cpwi(r,ii) = 0.
         end do

         if ( ddim.eq.3 ) then
            
            idx(d2) = idx(d2)+1
            if ( idx(d2).gt.cpbnds(d2) ) then
               idx(d1)=idx(d1)+1
               idx(d2)=0
            end if

         else

            idx(d1)=idx(d1)+1

         end if
            
      end do

      do k=0, MAXDIM-1
         idx(k) = 0
      end do

      span = findspan(cpbnds(dir),p(dir),isou,knts(kbnds(0,dir)))

      call dersbasisfuns(span,isou,p(dir),cpbnds(dir),
     .     0,knts(kbnds(0,dir)),bfuns(0,0))


      do while(idx(d1).le.cpbnds(d1))

         ii = cii( idx(d1), idx(d2) )
         
c         print *, "ii : ",idx(d1),idx(d2),ii

         do k=0,p(dir)
            
            idx(dir) = span-p(dir)+k
            i  = ci(idx(0),idx(1),idx(2))

c            print *,"i : ",idx(0),idx(1),idx(2),i

            do r=0,rdim
               cpwi(r,ii) = cpwi(r,ii) + cptsw(r,i)*bfuns(0,k)
            end do

         end do
c         print *,"---"
         if ( ddim.eq.3 ) then
            
            idx(d2) = idx(d2)+1
            if ( idx(d2).gt.cpbnds(d2) ) then
               idx(d1)=idx(d1)+1
               idx(d2)=0
            end if

         else

            idx(d1)=idx(d1)+1

         end if
            
      end do

      return 
      end

c
c     nurbsintrp computes a nurbs curve of specified degree to the
c                given data; basically P&T A9.1.  
c
c                work array sizes:
c                real wrk(npts,npts+1) 
c                integer iwrk(npts)
c
      subroutine nurbsintrp(pts,npts,u,knts,nknts,p,rdim,cpts,
     .                      wrk,iwrk)

      implicit none

      integer MAXORDER
      parameter(MAXORDER=20)
      integer MAXDIM
      parameter(MAXDIM=4)

c     inputs
      integer npts, nknts,p,rdim
      real pts(0:rdim-1, 0:*), u(0:*), knts(0:*)

c     output
      real cpts(0:rdim,0:*)

c     work arrays
      real wrk(0:(npts-1),0:*)
      integer iwrk(0:*)

c     temporaries
      integer n,i,j,span,info
      real bfuns(0:MAXORDER,0:MAXORDER)

c     functions
      integer findspan
      external findspan

      n = npts-1

      do i=0,n
         do j=0,n
            wrk(i,j)=0
         end do
      end do

      do i=0,n
         span=findspan(n,p,u(i),knts)
         call dersbasisfuns(span,u(i),p,n,0,knts,bfuns)
         do j=0,p
            wrk(i,span-p+j) = bfuns(0,j)
         end do
      end do

c     note that dgefa and dgesl do not take advantage
c          of the banded structure of the matrix nor the
c          property that no pivots are needed 

c      call dgefa(wrk,npts,npts,iwrk,info)
      call dgetrf(npts,npts,wrk,npts,iwrk,info)

      do i=0,rdim-1
         do j=0,n
            wrk(j,npts) = pts(i,j)
         end do
c         call dgesl(wrk,npts,npts,iwrk,wrk(0,npts),0)
         call dgetrs('N',npts,1,wrk,npts,iwrk,wrk(0,npts),npts,info)
         do j=0,n
            cpts(i,j) = wrk(j,npts)
         end do
      end do

      do j=0,n
         cpts(rdim,j) = 1
      end do

      return
      end 

#endif

c      program testbico

c      implicit none
      
c      integer bico
c      external bico

c      integer n,k

c      do n=0,15
c         do k=0,n

c            write(*,'(i5$)') bico(n,k)
c         end do
c         write (*,*) ""
c      end do

c      stop
c      end


c      program testneval

c      implicit none

c      integer p,n,nder,ddim,rdim,kntbnds(2),cpbnds(3),nu
c      real cptsw(0:2,0:3), knts(8), ders(2,0:3,4), u(4)

c      real s1,s2

c      integer i,j

c      nu = 4
c      p=3
c      n=3
c      nder=3
c      ddim=1
c      rdim = 2
c      kntbnds(1) = 0
c      kntbnds(2) = 7
c      cpbnds(1) = 3
c      cpbnds(2) = 0
c      cpbnds(3) = 0

c      cptsw(0,0)=0.
c      cptsw(1,0)=0.
c      cptsw(2,0)=1.
c      cptsw(0,1)=.25
c      cptsw(1,1)=1.
c      cptsw(2,1)=2.
c      cptsw(0,2)=.75
c      cptsw(1,2)=1.
c      cptsw(2,2)=2.
c      cptsw(0,3)=1.
c      cptsw(1,3)=0. 
c      cptsw(2,3)=1.

c      do j=0,3
c         do i=0,1
c            cptsw(i,j) = cptsw(i,j)*cptsw(2,j)
c         end do
c      end do

c      do i=1,4
c         knts(i) = 0.
c         knts(i+4) = 1.
c      end do

c      u(1) = 0.
c      u(2) = 1./3.
c      u(3) = 2./3.
c      u(4) = 1.

c      call second(s1)
c      call nurbseval(nder,ddim,rdim,n,p,knts,kntbnds,cptsw,cpbnds,
c     .               u, nu, ders)
c      call second(s2)

c      do j=0,nder
c         do i=1,4
c            write(*,*) ders(1,j,i), ders(2,j,i)
c         end do
c         print *,"==="
c      end do

c      print *,"time = ",s2-s1," s ",s1,s2

c      stop

c      end



