      subroutine solidMechanicsHemp( 
     *             nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *             mask,f0,u,up,xy,
     *             boundaryCondition,dim,bcf0,bcOffset,
     *             iparam,rparam,niwrk,iwrk,
     *             nrwrk,rwrk,ierr )
c===================================================================================
c  Cgsm interface to Jeff Banks' Hemp code
c
c  f0(i1,i2,i3,1:3) input : holds mass, density and internal energy from t=0. 
c  u(i1,i2,i3,n) (input) : solution at time t
c  up(i1,i2,i3,n) (output) : "uPrime" = du/dt
c  ipar(0:*) : integer parameters
c  rpar(0:*) : real parameters 
c  iwk(1:niwk) : integer work space
c  rwk(nrwk) : real work space
c
c 2D: solution components n=1,2,3,...,9 :
c        x,y, v1,v2, s11,s12,s22, p, q 
c
c The major goal of this subroutine for now is to unpack the solution vector
c into a form the Hemp code knows what to do with.
c      
c===================================================================================
c
c..declarations of incomming variables 
c   (note this file is single precision so you probably want to auto double with -r8 -i4)
      implicit none
      integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real f0(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)
      real up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)
 
      integer boundaryCondition(1:2,1:3)
      integer dim(1:2,1:3,1:2,1:3)
      real bcf0(1:*)
      integer*8 bcOffset(1:2,1:3)
      integer*8 tzptr ! check this size with Bill!!
      integer itz


      integer niwrk,nrwrk,ierr
      real rparam(0:*),rwrk(nrwrk)
      integer iparam(0:*),iwrk(niwrk)
c
c..declarations of local variables
      integer n1a,n1b,n2a,n2b,n3a,n3b,nc
      integer idebug,i1,i2,i3,k,grid,ilinear,addForcing
c
      real t,dt,R,Y0,lamMax,vismax,apr,bpr,cpr,dpr,p0,c0,cl,hgVisc
      real lemu,lelambda
      integer lx,ly,lvx,lvy,lsx,ltxy,lsy,lp,lmass,lrho0,le0,ier
      integer lphi,lxold,lyold,lvx_temp,lvy_temp,lvx_old,lvy_old
      integer lsigma_x,lsigma_y,lArea,lq,nx,ny,hgFlag
      integer n1atemp,n1btemp,n2atemp,n2btemp
      real xLoc,yLoc,rhs(11),currRho,currArea

      ! boundary conditions are defined here: 
      include 'bcDefineFortranInclude.h'

      real bcf,xoffsetlr,yoffsetlr,xoffsettb,yoffsettb,xavg,yavg
      integer kd,ks,n
c     --- start statement functions ----

      ! Here is the the generic boundary condition forcing array. It uses the bcOffset(side,axis) values as an
      ! an offset from the bcf0 array to access the bcf10, bcf01, bcf11, ... arrays
      bcf(ks,kd,i1,i2,i3,k) = bcf0(bcOffset(ks,kd)+1+ 
     & (i1-dim(1,1,ks,kd)+(dim(2,1,ks,kd)-dim(1,1,ks,kd)+1)* 
     & (i2-dim(1,2,ks,kd)+(dim(2,2,ks,kd)-dim(1,2,ks,kd)+1)*
     & (i3-dim(1,3,ks,kd)+(dim(2,3,ks,kd)-dim(1,3,ks,kd)+1)*
     &   (k-1)))))
c    --- end statement functions ----
c
c.. make sure we are doing 2D
      ierr = 0
      if( nd.ne.2 ) then
        ierr = -1
        return
      end if

      n1a = iparam(0)
      n1b = iparam(1)
      n2a = iparam(2)
      n2b = iparam(3)
      n3a = iparam(4)
      n3b = iparam(5)
      nc  = min(iparam(6),9)
      grid   = iparam(7) ! wdh: this is the component grid number 
      idebug = iparam(8)
      hgFlag = iparam(9)
c      itz    = iparam(10)
      itz = 0 ! for now just turn twilight zone off ... FIX THIS

      addForcing=iparam(17) ! =1 -> add forcing *wdh* 090825 

      ilinear = 1 ! turn on linear elasticity

      t  = rparam(0)
      dt = rparam(1)
      R  = rparam(2)
      Y0 = rparam(3)
      apr = rparam(4)
      bpr = rparam(5)
      cpr = rparam(6)
      dpr = rparam(7)

      p0  = rparam(8) ! background baseline pressure for BCs, below are some old values
c
      c0 = rparam(9)
      cl = rparam(10)
      hgVisc = rparam(11)
c
      lemu = rparam(12)
      lelambda = rparam(13)

c      ep  = rparam(13) ! pointer for exact solution ! I don't think this will work in general ... FIX THIS
      tzptr = 0      

c      write(6,*)n1a,n1b,n2a,n2b
c      write(6,*)'HEMP called at t=',t,dt

c      write(6,*)dim
c      write(6,*)bcoffset
c      do i1=1,2
c        do i2=1,3
c          write(6,*)bcOffset(i1,i2)
c        end do
c      end do
c.. left
c      if( .true. ) then
      if( .false. ) then
        do i2=n2a,n2b
          write(6,*)n1a,i2,n3a
          write(6,*)bcf(1,1,n1a,i2,n3a,1),bcf(1,1,n1a,i2,n3a,2)
          write(6,*)bcf(1,1,n1a,i2,n3a,3),bcf(1,1,n1a,i2,n3a,4)
          write(6,*)
        end do
      end if
c..right
c      if( .true. ) then
      if( .false. ) then
        do i2=n2a,n2b
          write(6,*)bcf(2,1,n1a,i2,n3a,1),bcf(2,1,n1a,i2,n3a,2)
          write(6,*)bcf(2,1,n1a,i2,n3a,3),bcf(2,1,n1a,i2,n3a,4)
          write(6,*)
        end do
      end if
c..bottom
c      if( .true. ) then
      if( .false. ) then
        do i1=n1a,n1b
          write(6,*)bcf(1,2,i1,n2a,n3a,1),bcf(1,2,i1,n2a,n3a,2)
          write(6,*)bcf(1,2,i1,n2a,n3a,3),bcf(1,2,i1,n2a,n3a,4)
          write(6,*)
        end do
      end if
c..top
c      if( .true. ) then
      if( .false. ) then
        do i1=n1a,n1b
          write(6,*)bcf(2,2,i1,n2b,n3a,1),bcf(2,2,i1,n2b,n3a,2)
          write(6,*)bcf(2,2,i1,n2b,n3a,3),bcf(2,2,i1,n2b,n3a,4)
          write(6,*)
        end do
      end if
c
c      if( .true. )then
      if( .false. )then
        ! just return until the rest is finished
        write(*,'(" solidMechanicsHemp called at t=",e9.3)') t
        write(*,'(" solidMechanicsHemp : R=",e8.2)') R
        write(*,'(" solidMechanicsHemp : Y0=",e8.2)') Y0
        write(*,'(" solidMechanicsHemp : n1a,n1b=",2i4)') n1a,n1b
        write(*,'(" solidMechanicsHemp : n2a,n2b=",2i4)') n2a,n2b
        write(*,'(" solidMechanicsHemp : n3a,n3b=",2i4)') n3a,n3b
        write(*,'(" solidMechanicsHemp : nd1a,nd1b=",2i4)') nd1a,nd1b
        write(*,'(" solidMechanicsHemp : nd2a,nd2b=",2i4)') nd2a,nd2b
        write(*,'(" solidMechanicsHemp : nd3a,nd3b=",2i4)') nd3a,nd3b
        write(*,'(" solidMechanicsHemp : apr =",e9.3)') apr
        write(*,'(" solidMechanicsHemp : bpr =",e9.3)') bpr
        write(*,'(" solidMechanicsHemp : cpr =",e9.3)') cpr
        write(*,'(" solidMechanicsHemp : dpr =",e9.3)') dpr
        write(*,'(" solidMechanicsHemp : hourGlass =",i4)') hgFlag
        write(*,'(" solidMechanicsHemp : BC left   =",i4)') 
     *     boundaryCondition(1,1)
        write(*,'(" solidMechanicsHemp : BC right  =",i4)') 
     *     boundaryCondition(2,1)
        write(*,'(" solidMechanicsHemp : BC bottom =",i4)') 
     *     boundaryCondition(1,2)
        write(*,'(" solidMechanicsHemp : BC top    =",i4)') 
     *     boundaryCondition(2,2)
        do i3 = n3a,n3b
        do i2 = n2a,n2b
        do i1 = n1a,n1b
          do k = 1,nc
            up(i1,i2,i3,k) = 0.0
          end do
        end do
        end do
        end do

        rparam(20) = 0.
        rparam(21) = 10.

        return 
      end if
c
      n1atemp = n1a
      n1btemp = n1b
      n2atemp = n2a
      n2btemp = n2b
c
c.. increase the grid size if we are doing periodic
      if( boundaryCondition(2,1).lt.0 ) then
        if( boundaryCondition(1,1).ge.0 ) then
          write(6,*)'(solidMechanicsHemp.f): BC mismatch'
          stop
        end if
      end if
      if( boundaryCondition(1,1).lt.0 ) then
        if( boundaryCondition(2,1).ge.0 ) then
          write(6,*)'(solidMechanicsHemp.f): BC mismatch'
          stop
        end if
        n1atemp = n1a-1
        n1btemp = n1b+1
        i3 = n3a
        xoffsetlr = u(n1b,n2a,i3,1)-u(n1a,n2a,i3,1)
        yoffsetlr = u(n1b,n2a,i3,2)-u(n1a,n2a,i3,2)
        ! set positions and velocities
        do i2 = n2a-1,n2b+1
          ! left
          u(n1a-1,i2,i3,1) = u(n1b-1,i2,i3,1)-xoffsetlr
          u(n1a-1,i2,i3,2) = u(n1b-1,i2,i3,2)-yoffsetlr
          u(n1a-1,i2,i3,3) = u(n1b-1,i2,i3,3)                  ! x-velocity
          u(n1a-1,i2,i3,4) = u(n1b-1,i2,i3,4)                  ! y-velocity
          ! right
          u(n1b+1,i2,i3,1) = u(n1a+1,i2,i3,1)+xoffsetlr
          u(n1b+1,i2,i3,2) = u(n1a+1,i2,i3,2)+yoffsetlr
          u(n1b+1,i2,i3,3) = u(n1a+1,i2,i3,3)                  ! x-velocity
          u(n1b+1,i2,i3,4) = u(n1a+1,i2,i3,4)                  ! y-velocity
        end do
        ! set stresses
        do i2 = n2a-1,n2b
          ! left
          u(n1a-1,i2,i3,5) = u(n1b-1,i2,i3,5)   ! x-stress
          u(n1a-1,i2,i3,6) = u(n1b-1,i2,i3,6)   ! cross stress
          u(n1a-1,i2,i3,7) = u(n1b-1,i2,i3,7)   ! y-stress
          u(n1a-1,i2,i3,8) = u(n1b-1,i2,i3,8)   ! pressure
          u(n1a-1,i2,i3,9) = u(n1b-1,i2,i3,9)   ! q
          f0(n1a-1,i2,i3,1) = f0(n1b-1,i2,i3,1) ! mass
          f0(n1a-1,i2,i3,2) = f0(n1b-1,i2,i3,2) ! rho0
          f0(n1a-1,i2,i3,3) = f0(n1b-1,i2,i3,3) ! e0
          ! right
          u(n1b,i2,i3,5) = u(n1a,i2,i3,5)   ! x-stress
          u(n1b,i2,i3,6) = u(n1a,i2,i3,6)   ! cross stress
          u(n1b,i2,i3,7) = u(n1a,i2,i3,7)   ! y-stress
          u(n1b,i2,i3,8) = u(n1a,i2,i3,8)   ! pressure
          u(n1b,i2,i3,9) = u(n1a,i2,i3,9)   ! q
          f0(n1b,i2,i3,1) = f0(n1a,i2,i3,1) ! mass
          f0(n1b,i2,i3,2) = f0(n1a,i2,i3,2) ! rho0
          f0(n1b,i2,i3,3) = f0(n1a,i2,i3,3) ! e0
        end do
      end if
c
      if( boundaryCondition(2,2).lt.0 ) then
        if( boundaryCondition(1,2).ge.0 ) then
          write(6,*)'(solidMechanicsHemp.f): BC mismatch'
          stop
        end if
      end if
      if( boundaryCondition(1,2).lt.0 ) then
        if( boundaryCondition(2,2).ge.0 ) then
          write(6,*)'(solidMechanicsHemp.f): BC mismatch'
          stop
        end if
        n2atemp = n2a-1
        n2btemp = n2b+1
        i3 = n3a
        xoffsettb = u(n1a,n2b,i3,1)-u(n1a,n2a,i3,1)
        yoffsettb = u(n1a,n2b,i3,2)-u(n1a,n2a,i3,2)
        ! set positions and velocities
        do i1 = n1a-1,n1b+1
          ! bottom
          u(i1,n2a-1,i3,1) = u(i1,n2b-1,i3,1)-xoffsettb
          u(i1,n2a-1,i3,2) = u(i1,n2b-1,i3,2)-yoffsettb
          u(i1,n2a-1,i3,3) = u(i1,n2b-1,i3,3)                  ! x-velocity
          u(i1,n2a-1,i3,4) = u(i1,n2b-1,i3,4)                  ! y-velocity

          ! top
          u(i1,n2b+1,i3,1) = u(i1,n2a+1,i3,1)+xoffsettb
          u(i1,n2b+1,i3,2) = u(i1,n2a+1,i3,2)+yoffsettb
          u(i1,n2b+1,i3,3) = u(i1,n2a+1,i3,3)                  ! x-velocity
          u(i1,n2b+1,i3,4) = u(i1,n2a+1,i3,4)                  ! y-velocity
        end do
        ! set stresses
        do i1 = n1a-1,n1b
          ! bottom
          u(i1,n2a-1,i3,5) = u(i1,n2b-1,i3,5)   ! x-stress
          u(i1,n2a-1,i3,6) = u(i1,n2b-1,i3,6)   ! cross stress
          u(i1,n2a-1,i3,7) = u(i1,n2b-1,i3,7)   ! y-stress
          u(i1,n2a-1,i3,8) = u(i1,n2b-1,i3,8)   ! pressure
          u(i1,n2a-1,i3,9) = u(i1,n2b-1,i3,9)   ! q
          f0(i1,n2a-1,i3,1) = f0(i1,n2b-1,i3,1) ! mass
          f0(i1,n2a-1,i3,2) = f0(i1,n2b-1,i3,2) ! rho0
          f0(i1,n2a-1,i3,3) = f0(i1,n2b-1,i3,3) ! e0

          ! top
          u(i1,n2b,i3,5) = u(i1,n2a,i3,5)   ! x-stress
          u(i1,n2b,i3,6) = u(i1,n2a,i3,6)   ! cross stress
          u(i1,n2b,i3,7) = u(i1,n2a,i3,7)   ! y-stress
          u(i1,n2b,i3,8) = u(i1,n2a,i3,8)   ! pressure
          u(i1,n2b,i3,9) = u(i1,n2a,i3,9)   ! q
          f0(i1,n2b,i3,1) = f0(i1,n2a,i3,1) ! mass
          f0(i1,n2b,i3,2) = f0(i1,n2a,i3,2) ! rho0
          f0(i1,n2b,i3,3) = f0(i1,n2a,i3,3) ! e0

        end do
      end if

      n1a = n1atemp
      n1b = n1btemp
      n2a = n2atemp
      n2b = n2btemp
c
c..Assign portions of the workspace for scatter
      nx = n1b-n1a+1
      ny = n2b-n2a+1
      lx       = 1
      ly       = lx+nx*ny
      lvx      = ly+nx*ny
      lvy      = lvx+nx*ny
      lsx      = lvy+nx*ny
      lsy      = lsx+(nx-1)*(ny-1)
      ltxy     = lsy+(nx-1)*(ny-1)
      lp       = ltxy+(nx-1)*(ny-1)
      lq       = lp+(nx-1)*(ny-1)
      lmass    = lq+(nx-1)*(ny-1)
      lrho0    = lmass+(nx-1)*(ny-1)
      le0      = lrho0+(nx-1)*(ny-1)

c
c.. now unpack the solution vector so we don't have to rewrite the kernel
c      that rewrite is on the "to do" list
      call solid_scatter( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                    n1a,n1b,n2a,n2b,n3a,n3b,u,f0,
     *                    rwrk(lx),rwrk(ly),rwrk(lvx),rwrk(lvy),
     *                    rwrk(lsx),rwrk(lsy),rwrk(ltxy),rwrk(lp),
     *                    rwrk(lq),rwrk(lmass),rwrk(lrho0),
     *                    rwrk(le0),ier )
      if( ier.eq.-1 ) then
        ierr = -1
        return
      end if
c
c..Assign rest of the workspace for Hemp kernel
      lphi     = le0+(nx-1)*(ny-1)
      lxold    = lphi+nx*ny
      lyold    = lxold+nx*ny
      lvx_temp = lyold+nx*ny
      lvy_temp = lvx_temp+nx*ny
      lvx_old  = lvy_temp+nx*ny
      lvy_old  = lvx_old+nx*ny
      lsigma_x = lvy_old+nx*ny
      lsigma_y = lsigma_x+(nx-1)*(ny-1)
      lArea    = lsigma_y+(nx-1)*(ny-1)
c
c..The workspace will need to be big enough to hold the following
cccccccc
c
c x        -- NX x NY
c y        -- NX x NY
c vx       -- NX x NY
c vy       -- NX x NY
c sx       -- NX-1 x NY-1
c sy       -- NX-1 x NY-1
c txy      -- NX-1 x NY-1
c p        -- NX-1 x NY-1
c q        -- NX-1 x NY-1
c mass     -- NX-1 x NY-1
c rho0     -- NX-1 x NY-1
c e0       -- NX-1 x NY-1
c phi      -- NX x NY
c xold     -- NX x NY
c yold     -- NX x NY
c vx_temp  -- NX x NY
c vy_temp  -- NX x NY
c
c sigma_x  -- NX-1 x NY-1
c sigma_y  -- NX-1 x NY-1
c Area     -- NX-1 x NY-1
c-----------------------------
c total     = 9*NX*NY+11*(NX-1)*(NY-1)
c
ccccccc
c
c..Call actual time stepper
      if( .false. ) then
c      if( .true. ) then
      call solid_step( n1a,n1b,n2a,n2b,n3a,n3b,dt,R,Y0,p0,c0,cl,
     *                 hgVisc,lemu,lelambda,
     *                 hgFlag,rwrk(lmass),rwrk(lx),rwrk(ly),
     *                 rwrk(lvx),rwrk(lvy),rwrk(lsx),
     *                 rwrk(lsy),rwrk(ltxy),rwrk(lp),rwrk(lq),
     *                 rwrk(lrho0),rwrk(le0),
     *                 rwrk(lphi),rwrk(lxold),rwrk(lyold),
     *                 rwrk(lvx_temp),rwrk(lvy_temp),
     *                 rwrk(lvx_old),rwrk(lvy_old),
     *                 rwrk(lsigma_x),rwrk(lsigma_y),
     *                 rwrk(lArea),apr,bpr,cpr,dpr,lamMax,vismax,
     *                 ilinear,boundaryCondition(1,1),dim(1,1,1,1),
     *                 bcf0(1),bcOffset(1,1) )
      else
      call solid_step_ol( n1a,n1b,n2a,n2b,n3a,n3b,dt,R,Y0,p0,c0,cl,
     *                 hgVisc,lemu,lelambda,
     *                 hgFlag,rwrk(lmass),rwrk(lx),rwrk(ly),
     *                 rwrk(lvx),rwrk(lvy),rwrk(lsx),
     *                 rwrk(lsy),rwrk(ltxy),rwrk(lp),rwrk(lq),
     *                 rwrk(lrho0),rwrk(le0),
     *                 rwrk(lphi),rwrk(lxold),rwrk(lyold),
     *                 rwrk(lvx_temp),rwrk(lvy_temp),
     *                 rwrk(lvx_old),rwrk(lvy_old),
     *                 rwrk(lsigma_x),rwrk(lsigma_y),
     *                 rwrk(lArea),apr,bpr,cpr,dpr,lamMax,vismax,
     *                 ilinear,boundaryCondition(1,1),dim(1,1,1,1),
     *                 bcf0(1),bcOffset(1,1) )
      end if
c
c..Now repack the data
      call solid_gather( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                   n1a,n1b,n2a,n2b,n3a,n3b,up,
     *                   rwrk(lx),rwrk(ly),rwrk(lvx),rwrk(lvy),
     *                   rwrk(lsx),rwrk(lsy),rwrk(ltxy),
     *                   rwrk(lp),rwrk(lq),ier )
      if( ier.eq.-1 ) then
        ierr = -1
        return
      end if
c
c..fixup positions
      n1a = iparam(0)
      n1b = iparam(1)
      n2a = iparam(2)
      n2b = iparam(3)
      n3a = iparam(4)
      n3b = iparam(5)
      if( boundaryCondition(2,1).lt.0 ) then
        if( boundaryCondition(1,1).ge.0 ) then
          write(6,*)'(solidMechanicsHemp.f): BC mismatch'
          stop
        end if
      end if
      if( boundaryCondition(1,1).lt.0 ) then
        if( boundaryCondition(2,1).ge.0 ) then
          write(6,*)'(solidMechanicsHemp.f): BC mismatch'
          stop
        end if
        i3 = n3a
        ! set positions and velocities
        do i2 = n2a-1,n2b+1
          xavg = 0.5*(up(n1a,i2,i3,1)+up(n1b,i2,i3,1))
          up(n1a,i2,i3,1) = xavg-0.5*xoffsetlr
          up(n1b,i2,i3,1) = xavg+0.5*xoffsetlr
          yavg = 0.5*(up(n1a,i2,i3,2)+up(n1b,i2,i3,2))
          up(n1a,i2,i3,2) = yavg-0.5*yoffsetlr
          up(n1b,i2,i3,2) = yavg+0.5*yoffsetlr
c
          up(n1a,i2,i3,3) = 0.5*(up(n1b,i2,i3,3)+up(n1a,i2,i3,3)) ! x-vel
          up(n1a,i2,i3,4) = 0.5*(up(n1b,i2,i3,4)+up(n1a,i2,i3,4)) ! y-vel
          up(n1b,i2,i3,3) = up(n1a,i2,i3,3) ! x-vel
          up(n1b,i2,i3,4) = up(n1a,i2,i3,4) ! y-vel       
        end do
      end if
c
      if( boundaryCondition(2,2).lt.0 ) then
        if( boundaryCondition(1,2).ge.0 ) then
          write(6,*)'(solidMechanicsHemp.f): BC mismatch'
          stop
        end if
      end if
      if( boundaryCondition(1,2).lt.0 ) then
        if( boundaryCondition(2,2).ge.0 ) then
          write(6,*)'(solidMechanicsHemp.f): BC mismatch'
          stop
        end if
        i3 = n3a
        ! set positions and velocities
        do i1 = n1a-1,n1b+1
          xavg = 0.5*(up(i1,n2a,i3,1)+up(i1,n2b,i3,1))
          up(i1,n2a,i3,1) = xavg-0.5*xoffsettb
          up(i1,n2b,i3,1) = xavg+0.5*xoffsettb
          yavg = 0.5*(up(i1,n2a,i3,2)+up(i1,n2b,i3,2))
          up(i1,n2a,i3,2) = yavg-0.5*yoffsettb
          up(i1,n2b,i3,2) = yavg+0.5*yoffsettb
c
          up(i1,n2a,i3,3) = 0.5*(up(i1,n2b,i3,3)+up(i1,n2a,i3,3)) ! x-vel
          up(i1,n2a,i3,4) = 0.5*(up(i1,n2b,i3,4)+up(i1,n2a,i3,4)) ! y-vel
          up(i1,n2b,i3,3) = up(i1,n2a,i3,3) ! x-vel
          up(i1,n2b,i3,4) = up(i1,n2a,i3,4) ! y-vel       
        end do
      end if
c
c..and return the time derivative
      do i3 = n3a,n3b
        do i2 = n2a,n2b
          do i1 = n1a,n1b
c            do k = 1,nc
            do k = 3,9
              up(i1,i2,i3,k) = (up(i1,i2,i3,k)-u(i1,i2,i3,k))/dt
            end do
            ! update the residual for the displacements in 10,11
c            up(i1,i2,i3,10) = up(i1,i2,i3,1)
c            up(i1,i2,i3,11) = up(i1,i2,i3,2)
            up(i1,i2,i3,10) = u(i1,i2,i3,3)
            up(i1,i2,i3,11) = u(i1,i2,i3,4)
c
            up(i1,i2,i3,1) = u(i1,i2,i3,3)
            up(i1,i2,i3,2) = u(i1,i2,i3,4)
          end do
        end do
c
        i2 = n2b
        do i1=n1a,n1b
          do k=5,nc
            up(i1,i2,i3,k) = 0.0
          end do
        end do
       
        i1 = n1b
        do i2 = n2a,n2b
          do k = 5,nc
            up(i1,i2,i3,k) = 0.0
          end do
        end do
      end do
c
c      if( .false. ) then
      if( .true. ) then
        call hempArtVis( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                 n1a,n1b,n2a,n2b,n3a,n3b,u,up,
     *                 boundaryCondition )
      end if
c
      if( ilinear.eq.1 ) then
        ! do not actually move the mesh for linear elasticity
        do k = 1,2
          do i3 = n3a,n3b
            do i2 = n2a,n2b
              do i1 = n1a,n1b
                up(i1,i2,i3,k) = 0.0
              end do
            end do
          end do
        end do
      end if
c
      if( itz.eq.1 ) then
        ! find twilight source
        ! first we have to extrapolate positions for one line of ghost cells
        ! note that one line of ghost cells for the mass should be filled in by the IC
c        do i1 = n2a,n2b
c          u(i1,n2a-1,n3a,1) = 2.0*u(i1,n2a,n3a,1)-u(i1,n2a+1,n3a,1)
c          u(i1,n2a-1,n3a,2) = 2.0*u(i1,n2a,n3a,2)-u(i1,n2a+1,n3a,2)
c          u(i1,n2b+1,n3a,1) = 2.0*u(i1,n2b,n3a,1)-u(i1,n2b-1,n3a,1)
c          u(i1,n2b+1,n3a,2) = 2.0*u(i1,n2b,n3a,2)-u(i1,n2b-1,n3a,2)
c        end do
c        do i2 = n2a,n2b
c          u(n1a-1,i2,n3a,1) = 2.0*u(n1a,i2,n3a,1)-u(n1a+1,i2,n3a,1)
c          u(n1a-1,i2,n3a,2) = 2.0*u(n1a,i2,n3a,2)-u(n1a+1,i2,n3a,2)
c          u(n1b+1,i2,n3a,1) = 2.0*u(n1b,i2,n3a,1)-u(n1b-1,i2,n3a,1)
c          u(n1b+1,i2,n3a,2) = 2.0*u(n1b,i2,n3a,2)-u(n1b-1,i2,n3a,2)
c        end do
c        u(n1a-1,n2a-1,n3a,1) = 2.0*u(n1a,n2a,n3a,1)
c     *     -u(n1a+1,n2a+1,n3a,1)
c        u(n1a-1,n2a-1,n3a,2) = 2.0*u(n1a,n2a,n3a,2)
c     *     -u(n1a+1,n2a+1,n3a,2)
cc
c        u(n1b+1,n2a-1,n3a,1) = 2.0*u(n1b,n2a,n3a,1)
c     *     -u(n1b-1,n2a+1,n3a,1)
c        u(n1b+1,n2a-1,n3a,2) = 2.0*u(n1b,n2a,n3a,2)
c     *     -u(n1b-1,n2a+1,n3a,2)
cc
c        u(n1b+1,n2b+1,n3a,1) = 2.0*u(n1b,n2b,n3a,1)
c     *     -u(n1b-1,n2b-1,n3a,1)
c        u(n1b+1,n2b+1,n3a,2) = 2.0*u(n1b,n2b,n3a,2)
c     *     -u(n1b-1,n2b-1,n3a,2)
cc
c        u(n1a-1,n2b+1,n3a,1) = 2.0*u(n1a,n2b,n3a,1)
c     *     -u(n1a+1,n2b-1,n3a,1)
c        u(n1a-1,n2b+1,n3a,2) = 2.0*u(n1a,n2b,n3a,2)
c     *     -u(n1a+1,n2b-1,n3a,2)
c
        ! next we get the source contribution at the nodes
        do i3 = n3a,n3b
          do i2 = n2a,n2b
            do i1 = n1a,n1b
              xLoc = u(i1,i2,i3,1)
              yLoc = u(i1,i2,i3,2)
c              call getArea( u(i1-1,i2-1,i3,1),u(i1+1,i2-1,i3,1),
c     *                      u(i1+1,i2+1,i3,1),u(i1-1,i2+1,i3,1),
c     *                      u(i1-1,i2-1,i3,2),u(i1+1,i2-1,i3,2),
c     *                      u(i1+1,i2+1,i3,2),u(i1-1,i2+1,i3,2),
c     *                      currArea )
c              currRho = 0.25*(f0(i1-1,i2-1,i3,1)+f0(i1,i2-1,i3,1)+
c     *                        f0(i1,i2,i3,1)+f0(i1-1,i2,i3,1))/
c     *                        currArea
c
              call getTZSource( tzptr,xLoc,yLoc,t,rhs,
     *           lemu,lelambda,ilinear )
c.. add the source ... note that getTZSource deals with the 
c    differences between linear and non-linear elasticity and 
c    so we don't worry about it here
              do k = 1,4
                up(i1,i2,i3,k) = u(i1,i2,i3,k)+rhs(k)
              end do
              do k = 10,11
                up(i1,i2,i3,k) = up(i1,i2,i3,k)+rhs(k)
              end do
c
            end do
          end do
        end do
c
        ! now get the source contribution to the centers
        do i3 = n3a,n3b
          do i2 = n2a,n2b-1
            do i1 = n1a,n1b-1
              xLoc = 0.5*(u(i1,i2,i3,1)+u(i1+1,i2,i3,1)+
     *                    u(i1,i2+1,i3,1)+u(i1+1,i2+1,i3,1))
              yLoc = 0.5*(u(i1,i2,i3,2)+u(i1+1,i2,i3,2)+
     *                    u(i1,i2+1,i3,2)+u(i1+1,i2+1,i3,2))
c              call getArea( u(i1,i2,i3,1),u(i1+1,i2,i3,1),
c     *                      u(i1+1,i2+1,i3,1),u(i1,i2+1,i3,1),
c     *                      u(i1,i2,i3,2),u(i1+1,i2,i3,2),
c     *                      u(i1+1,i2+1,i3,2),u(i1,i2+1,i3,2),
c     *                      currArea )
c              currRho = f0(i1,i2,i3,1)/currArea
c
              call getTZSource( tzptr,xLoc,yLoc,t,rhs,
     *           lemu,lelambda,ilinear )
c.. add the source ... note that getTZSource deals with the 
c    differences between linear and non-linear elasticity and 
c    so we don't worry about it here
              do k = 5,8
                up(i1,i2,i3,k) = u(i1,i2,i3,k)+rhs(k)
              end do
c
            end do
          end do
        end do
      end if
c
c      write(6,*)vismax,lamMax
      ! return these: 
      rparam(20) = 0.e0
c      rparam(20) = 4.0*vismax
c      rparam(21) = 1.e0/dxbalmin
c      rparam(21) = lamMax
      rparam(21) = 2.0*lamMax  ! put in a safety factor of 2
c
      return
      end 
c
c+++++++++++++++
c
      subroutine solid_scatter( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                          n1a,n1b,n2a,n2b,n3a,n3b,u,f0,
     *                          x,y,vx,vy,
     *                          sx,sy,txy,p,q,
     *                          mass,rho0,e0,ierr )
      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer n1a,n1b,n2a,n2b,n3a,n3b,ierr
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)
      real f0(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)
      real x(n1a:n1b,n2a:n2b),y(n1a:n1b,n2a:n2b)
      real vx(n1a:n1b,n2a:n2b),vy(n1a:n1b,n2a:n2b)
      real sx(n1a:n1b-1,n2a:n2b-1),sy(n1a:n1b-1,n2a:n2b-1)
      real txy(n1a:n1b-1,n2a:n2b-1),p(n1a:n1b-1,n2a:n2b-1)
      real q(n1a:n1b-1,n2a:n2b-1)
      real mass(n1a:n1b-1,n2a:n2b-1),rho0(n1a:n1b-1,n2a:n2b-1)
      real e0(n1a:n1b-1,n2a:n2b-1)

      integer i1,i2,i3

      ierr = 0
      if( nd3a.ne.nd3b ) then
        ierr = -1
        return
      end if
      i3 = nd3a


c..Distribute informaion to nodes
      do i2=n2a,n2b
        do i1=n1a,n1b
          x(i1,i2) = u(i1,i2,i3,1)
          y(i1,i2) = u(i1,i2,i3,2)
          vx(i1,i2) = u(i1,i2,i3,3)
          vy(i1,i2) = u(i1,i2,i3,4)
        end do
      end do
c
c..Distribute information to cell centers
      do i2=n2a,n2b-1
        do i1=n1a,n1b-1
          sx(i1,i2) = u(i1,i2,i3,5)
          txy(i1,i2) = u(i1,i2,i3,6)
          sy(i1,i2) = u(i1,i2,i3,7)
          p(i1,i2) = u(i1,i2,i3,8)
          q(i1,i2) = u(i1,i2,i3,9)
          mass(i1,i2) = f0(i1,i2,i3,1)
          rho0(i1,i2) = f0(i1,i2,i3,2)
          e0(i1,i2) = f0(i1,i2,i3,3)
        end do
      end do

      return
      end
c
c+++++++++++++++
c
      subroutine solid_gather( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                         n1a,n1b,n2a,n2b,n3a,n3b,u,
     *                         x,y,vx,vy,
     *                         sx,sy,txy,p,q,ierr )
      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer n1a,n1b,n2a,n2b,n3a,n3b,ierr
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)
      real x(n1a:n1b,n2a:n2b),y(n1a:n1b,n2a:n2b)
      real vx(n1a:n1b,n2a:n2b),vy(n1a:n1b,n2a:n2b)
      real sx(n1a:n1b-1,n2a:n2b-1),sy(n1a:n1b-1,n2a:n2b-1)
      real txy(n1a:n1b-1,n2a:n2b-1),p(n1a:n1b-1,n2a:n2b-1)
      real q(n1a:n1b-1,n2a:n2b-1),tol

      integer i1,i2,i3

      tol = 1.e-30

      ierr = 0
      if( nd3a.ne.nd3b ) then
        ierr = -1
        return
      end if
      i3 = nd3a
c
c..get nodal data
      do i2=n2a,n2b
        do i1=n1a,n1b
          u(i1,i2,i3,1) = x(i1,i2)
          u(i1,i2,i3,2) = y(i1,i2)

          if( abs(vx(i1,i2)).lt.tol )vx(i1,i2) = 0.0
          if( abs(vy(i1,i2)).lt.tol )vy(i1,i2) = 0.0
          u(i1,i2,i3,3) = vx(i1,i2)
          u(i1,i2,i3,4) = vy(i1,i2)
        end do
      end do
c
c..get cell center data
      do i2=n2a,n2b-1
        do i1=n1a,n1b-1
          if( abs(sx(i1,i2)).lt.tol )sx(i1,i2) = 0.0
          if( abs(txy(i1,i2)).lt.tol )txy(i1,i2) = 0.0
          if( abs(sy(i1,i2)).lt.tol )sy(i1,i2) = 0.0
          if( abs(p(i1,i2)).lt.tol )p(i1,i2) = 0.0
          if( abs(q(i1,i2)).lt.tol )q(i1,i2) = 0.0

          u(i1,i2,i3,5) = sx(i1,i2)
          u(i1,i2,i3,6) = txy(i1,i2)
          u(i1,i2,i3,7) = sy(i1,i2)
          u(i1,i2,i3,8) = p(i1,i2)
          u(i1,i2,i3,9) = q(i1,i2)
        end do
      end do

      return
      end
c
c+++++++++++++++
c
      subroutine getTZSource( tzptr,x,y,t,rhs,
     *   lemu,lelambda,ilinear )
c
      implicit none
      integer*8 tzptr
      integer ilinear
      real x,y,t,rhs(11),lemu,lelambda
c
      real u0(11),ux(11),uy(11),ut(11),mu,lam,rho
      integer k
c
      ! call getTZSource( tzptr,xLoc,yLoc,t,rhs,ilinear )
      rho = 1.0 ! This needs to be sorted out for the general case ... FIX ME!!!
c
      do k = 1,11
        call ogDeriv( tzptr,1,0,0,0,x,y,0.0,t,k-1,ut(k) )
        call ogDeriv( tzptr,0,1,0,0,x,y,0.0,t,k-1,ux(k) )
        call ogDeriv( tzptr,0,0,1,0,x,y,0.0,t,k-1,uy(k) )
        call ogDeriv( tzptr,0,0,0,0,x,y,0.0,t,k-1,u0(k) )
      end do
c
      if( ilinear.eq.1 ) then
        mu = lemu
        lam = lelambda
      else
        ! compute mu ... FIX ME!
      end if
c
      rhs(3) = ut(3)-1.0/rho*(-ux(8)+ux(5)+uy(6))
      rhs(4) = ut(4)-1.0/rho*( ux(6)-uy(8)+uy(7))
      rhs(5) = ut(5)-4.0/3.0*mu*ux(3)+2.0/3.0*mu*uy(4)
      rhs(6) = ut(6)-mu*(ux(4)+uy(3))
      rhs(7) = ut(7)+2.0/3.0*mu*ux(3)-4.0/4.0*mu*uy(4)
      rhs(9) = 0.0
      rhs(10) = ut(10)-u0(3)
      rhs(11) = ut(11)-u0(4)

      if( ilinear.eq.1 ) then
        rhs(1) = 0.0
        rhs(2) = 0.0
        rhs(8) = ut(8)+(lam+2.0/3.0*mu)*(ux(3)+uy(4))
      else
        rhs(1) = ut(10)-u0(3)
        rhs(2) = ut(11)-u0(4)
c.. correct for rotations
        rhs(5) = rhs(5)-u0(6)*(ux(4)+uy(3))
        rhs(6) = rhs(6)+0.5*(u0(5)-u0(7))*(ux(4)-uy(3))
        rhs(7) = rhs(7)+u0(6)*(ux(4)+uy(3))
        rhs(8) = 0.0
c        rhs(8) = ! still have to figure this out for nonliner ... FIX ME!!
      end if
c
      return
      end
c
c+++++++++++++++
c
      subroutine hempArtVis( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *                       n1a,n1b,n2a,n2b,n3a,n3b,u,up,
     *                       boundaryCondition )
      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer n1a,n1b,n2a,n2b,n3a,n3b,ierr
      integer boundaryCondition(1:2,1:3)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)
      real up(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:*)
c
      include 'bcDefineFortranInclude.h'
      integer i1,i2,i3,k,perr,pers
      real nu,dr,ds
      
      nu = 1.0e-1
c      nu = 0.0
      dr = 1.0/(n1b-n1a)
      ds = 1.0/(n2b-n2a)
c      dr = 1.0
c      ds = 1.0
      perr = 0
      pers = 0

c.. extrapolate to low order for ghost cells     
      if( .false. ) then
      do k=2,4
        do i3=n3a,n3b
          do i1 = n1a,n1b
            u(i1,n2a-1,i3,k) = u(i1,n2a,i3,k)
            u(i1,n2a-2,i3,k) = u(i1,n2a+1,i3,k)
            u(i1,n2b+1,i3,k) = u(i1,n2b,i3,k)
            u(i1,n2b+2,i3,k) = u(i1,n2b-1,i3,k)
          end do
          do i2 = n2a-2,n2b+2
            u(n1a-1,i2,i3,k) = u(n1a,i2,i3,k)
            u(n1a-2,i2,i3,k) = u(n1a+1,i2,i3,k)
            u(n1b+1,i2,i3,k) = u(n1b,i2,i3,k)
            u(n1b+2,i2,i3,k) = u(n1b-1,i2,i3,k)
          end do
        end do
      end do
c
      do k=5,9
        do i3=n3a,n3b
          do i1 = n1a,n1b-1
            u(i1,n2a-1,i3,k) = u(i1,n2a,i3,k)
            u(i1,n2a-2,i3,k) = u(i1,n2a+1,i3,k)
            u(i1,n2b,i3,k)   = u(i1,n2b-1,i3,k)
            u(i1,n2b+1,i3,k) = u(i1,n2b-2,i3,k)
          end do
          do i2 = n2a-2,n2b+1
            u(n1a-1,i2,i3,k) = u(n1a,i2,i3,k)
            u(n1a-2,i2,i3,k) = u(n1a+1,i2,i3,k)
            u(n1b,i2,i3,k)   = u(n1b-1,i2,i3,k)
            u(n1b+1,i2,i3,k) = u(n1b-2,i2,i3,k)
          end do
        end do
      end do
      end if
c
c    here we check BC's to treat periodic conditions appropriately
      if( .true. ) then
      if( boundaryCondition(2,1).lt.0 ) then
        if( boundaryCondition(1,1).ge.0 ) then
          write(6,*)'(artVis.f): BC mismatch'
          stop
        end if
      end if
      if( boundaryCondition(1,1).lt.0 ) then
        if( boundaryCondition(2,1).ge.0 ) then
          write(6,*)'(artVis.f): BC mismatch'
          stop
        end if
        perr = 2
        do k=2,4
          do i3=n3a,n3b !*wdh* 090427
          do i2 = n2a-2,n2b+2
            u(n1a-1,i2,i3,k) = u(n1b-1,i2,i3,k)
            u(n1a-2,i2,i3,k) = u(n1b-2,i2,i3,k)
            u(n1b+1,i2,i3,k) = u(n1a+1,i2,i3,k)
            u(n1b+2,i2,i3,k) = u(n1a+2,i2,i3,k)
          end do
          end do
        end do
        do k=5,9
          do i3=n3a,n3b !*wdh* 090427
          do i2 = n2a-2,n2b+1
            u(n1a-1,i2,i3,k) = u(n1b-1,i2,i3,k)
            u(n1a-2,i2,i3,k) = u(n1b-2,i2,i3,k)
            u(n1b,i2,i3,k)   = u(n1a,i2,i3,k)
            u(n1b+1,i2,i3,k) = u(n1a+1,i2,i3,k)
          end do
          end do
        end do
      end if
      if( boundaryCondition(2,2).lt.0 ) then
        if( boundaryCondition(1,2).ge.0 ) then
          write(6,*)'(artVis.f): BC mismatch'
          stop
        end if
      end if
      if( boundaryCondition(1,2).lt.0 ) then
        if( boundaryCondition(2,2).ge.0 ) then
          write(6,*)'(artVis.f): BC mismatch'
          stop
        end if
        pers = 2
        do k=2,4
          do i3=n3a,n3b
            do i1 = n1a,n1b
              u(i1,n2a-1,i3,k) = u(i1,n2b-1,i3,k)
              u(i1,n2a-2,i3,k) = u(i1,n2b-2,i3,k)
              u(i1,n2b+1,i3,k) = u(i1,n2a+1,i3,k)
              u(i1,n2b+2,i3,k) = u(i1,n2a+2,i3,k)
            end do
          end do
        end do
        do k=5,9
          do i3=n3a,n3b
            do i1 = n1a,n1b-1
              u(i1,n2a-1,i3,k) = u(i1,n2b-1,i3,k)
              u(i1,n2a-2,i3,k) = u(i1,n2b-2,i3,k)
              u(i1,n2b,i3,k)   = u(i1,n2a,i3,k)
              u(i1,n2b+1,i3,k) = u(i1,n2a+1,i3,k)
            end do
          end do
        end do
      end if
      end if
c
c.. compute 4th order diffusion for velocities (do not worry about positions for now)
      do k=2,4
        do i3=n3a,n3b
c          do i2 = n2a+2,n2b-2
c            do i1 = n1a+2,n1b-2
c          do i2 = n2a,n2b
c            do i1 = n1a,n1b
          do i2 = n2a+2-pers,n2b-2+pers
            do i1 = n1a+2-perr,n1b-2+perr
              up(i1,i2,i3,k) = up(i1,i2,i3,k)-nu*(
     *           u(i1-2,i2,i3,k)-4.0*u(i1-1,i2,i3,k)+
     *           6.0*u(i1,i2,i3,k)-4.0*u(i1+1,i2,i3,k)+
     *           u(i1+2,i2,i3,k) )/dr-nu*(
     *           u(i1,i2-2,i3,k)-4.0*u(i1,i2-1,i3,k)+
     *           6.0*u(i1,i2,i3,k)-4.0*u(i1,i2+1,i3,k)+
     *           u(i1,i2+2,i3,k) )/ds
            end do
          end do
        end do
      end do

c.. compute 4th order diffusion for stresses (do not worry about displacements for now)
      do k=5,9
        do i3=n3a,n3b
c          do i2 = n2a+2,n2b-3
c            do i1 = n1a+2,n1b-3
c          do i2 = n2a,n2b-1
c            do i1 = n1a,n1b-1
          do i2 = n2a+2-pers,n2b-3+pers
            do i1 = n1a+2-perr,n1b-3+perr
              up(i1,i2,i3,k) = up(i1,i2,i3,k)-nu*(
     *           u(i1-2,i2,i3,k)-4.0*u(i1-1,i2,i3,k)+
     *           6.0*u(i1,i2,i3,k)-4.0*u(i1+1,i2,i3,k)+
     *           u(i1+2,i2,i3,k) )/dr-nu*(
     *           u(i1,i2-2,i3,k)-4.0*u(i1,i2-1,i3,k)+
     *           6.0*u(i1,i2,i3,k)-4.0*u(i1,i2+1,i3,k)+
     *           u(i1,i2+2,i3,k) )/ds
            end do
          end do
        end do
      end do
c      
      return
      end
c
c+++++++++++++++
c
