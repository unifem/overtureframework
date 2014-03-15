c ***********************************************************************
c
c        Utility Routines for the all-speed flow solver
c
c ***********************************************************************

#Include "defineDiffOrder2f.h"


#beginMacro beginLoops()
 n1a=nr(0,0)
 n1b=nr(1,0)
 n2a=nr(0,1)
 n2b=nr(1,1)
 n3a=nr(0,2)
 n3b=nr(1,2)
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
#endMacro

#beginMacro endLoops()
  end if
 end do
 end do
 end do
#endMacro

#beginMacro beginLoopsNoMask()
 n1a=nr(0,0)
 n1b=nr(1,0)
 n2a=nr(0,1)
 n2b=nr(1,1)
 n3a=nr(0,2)
 n3b=nr(1,2)
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
#endMacro

#beginMacro endLoopsNoMask()
 end do
 end do
 end do
#endMacro

#beginMacro loopse1(e1)
if( useWhereMask.ne.0 )then
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
   e1
  end if
 end do
 end do
 end do
else
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  e1
 end do
 end do
 end do
end if
#endMacro
#beginMacro loopse2(e1,e2)
if( useWhereMask.ne.0 )then
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).gt.0 )then
   e1
   e2
  end if
 end do
 end do
 end do
else
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  e1
  e2
 end do
 end do
 end do
end if
#endMacro

#beginMacro loopse3(e1,e2,e3)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
      e1
      e2
      e3
  end do
  end do
  end do
end if
#endMacro

#beginMacro loopse4(e1,e2,e3,e4)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
      e4
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
      e1
      e2
      e3
      e4
  end do
  end do
  end do
end if
#endMacro

      subroutine asfAddGradP(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
          ipar,rpar, u,rho, mask, rsxy, pdb, ierr )         
c========================================================================
c
c    ASF: add  alpha*grad(p)/rho to the momentum equations :
c
c      u(i1,i2,i3,uc..) += alpha*grad(p)/rho
c
c  u : solution to add to
c  rho : density to use (this may be the true density or a linearized version.
c 
c wdh: 0701
c========================================================================
      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real rho(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real rpar(0:*)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer ipar(0:*),ierr

      double precision pdb  ! pointer to data base

c.......local

      integer kd,kd3,i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b
      integer rc,tc,uc,vc,wc,pc,sc
      integer grid,orderOfAccuracy,gridType,gridIsMoving,useWhereMask

      real dr(0:2),dx(0:2)

      real t,dt,alpha

      integer debug

      integer ok,getInt,getReal,pdeModel
c..................
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )


      ! declare variables for difference approximations of u and RX
      declareDifferenceOrder2(u,RX)

c .............. begin statement functions
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real ogf,diss2,ad2,disst2,tanDiss2

c.......statement functions for jacobian
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

c     The next macro call will define the difference approximation statement functions
      defineDifferenceOrder2Components1(u,RX)
          
c     --- end statement functions

c .............. end statement functions


      ierr=0
      ! write(*,*) 'Inside asfSlipWallBC'

c$$$#beginMacro getIntPar(name,value)
c$$$ ok = getInt(pdb,name,value)  
c$$$ if( ok.eq.1 )then
c$$$   write(*,'("*** asfAddGradP: 'name'=",i4)') value
c$$$ else
c$$$   write(*,'("*** asfAddGradP: 'name' NOT FOUND")') 
c$$$ end if
c$$$#endMacro
c$$$
c$$$      getIntPar('rc',rc)  
c$$$      getIntPar('uc',uc)  
c$$$      getIntPar('vc',vc)  

c$$$      ok = getInt(pdb,'uc',uc)  
c$$$      if( ok.eq.1 )then
c$$$        write(*,'("*** asfAddGradP: uc=",i4)') uc
c$$$      else
c$$$        write(*,'("*** asfAddGradP: rc NOT FOUND")') 
c$$$      end if
c$$$      ok = getInt(pdb,'pdeModel',pdeModel)  
c$$$      if( ok.eq.1 )then
c$$$        write(*,'("*** asfAddGradP: pdeModel=",i4)') pdeModel
c$$$      else
c$$$        write(*,'("*** asfAddGradP: pdeModel NOT FOUND")') 
c$$$      end if

      rc                =ipar(0)
      uc                =ipar(1)
      vc                =ipar(2)
      wc                =ipar(3)
      tc                =ipar(4)
      pc                =ipar(5)
      grid              =ipar(6)
      gridType          =ipar(7)
      orderOfAccuracy   =ipar(8)
      gridIsMoving      =ipar(9) 
      useWhereMask      =ipar(10)
      n1a               =ipar(11)
      n1b               =ipar(12)
      n2a               =ipar(13)
      n2b               =ipar(14)
      n3a               =ipar(15)
      n3b               =ipar(16)


      dx(0)             =rpar(0)
      dx(1)             =rpar(1)
      dx(2)             =rpar(2)
      dr(0)             =rpar(3)
      dr(1)             =rpar(4)
      dr(2)             =rpar(5)
      t                 =rpar(6)
      dt                =rpar(7)
      alpha             =rpar(8)

c      write(*,'(" **** asfAddGradP: rc,pc,n1a,n1b,n2a,n2b=",2i2,4i5," t,dt,alpha=",3f7.2)') \
c          rc,pc,n1a,n1b,n2a,n2b,t,dt,alpha
c      ! ' 

      if( nd.eq.2 .and. gridType.eq.rectangular )then

        ! 2D rectangular

        loopse2(u(i1,i2,i3,uc)=u(i1,i2,i3,uc)+alpha*ux22r(i1,i2,i3,pc)/rho(i1,i2,i3),\
                u(i1,i2,i3,vc)=u(i1,i2,i3,vc)+alpha*uy22r(i1,i2,i3,pc)/rho(i1,i2,i3))

      else if( nd.eq.2 .and. gridType.eq.curvilinear )then
     
        ! 2D curvilinear

        loopse2(u(i1,i2,i3,uc)=u(i1,i2,i3,uc)+alpha*ux22(i1,i2,i3,pc)/rho(i1,i2,i3),\
                u(i1,i2,i3,vc)=u(i1,i2,i3,vc)+alpha*uy22(i1,i2,i3,pc)/rho(i1,i2,i3))

      else if( nd.eq.3 .and. gridType.eq.rectangular )then

        ! 3D rectangular

        loopse3(u(i1,i2,i3,uc)=u(i1,i2,i3,uc)+alpha*ux23r(i1,i2,i3,pc)/rho(i1,i2,i3),\
                u(i1,i2,i3,vc)=u(i1,i2,i3,vc)+alpha*uy23r(i1,i2,i3,pc)/rho(i1,i2,i3),\
                u(i1,i2,i3,wc)=u(i1,i2,i3,wc)+alpha*uz23r(i1,i2,i3,pc)/rho(i1,i2,i3))

      else if( nd.eq.3 .and. gridType.eq.curvilinear )then
     
        ! 3D curvilinear

        loopse3(u(i1,i2,i3,uc)=u(i1,i2,i3,uc)+alpha*ux23(i1,i2,i3,pc)/rho(i1,i2,i3),\
                u(i1,i2,i3,vc)=u(i1,i2,i3,vc)+alpha*uy23(i1,i2,i3,pc)/rho(i1,i2,i3),\
                u(i1,i2,i3,wc)=u(i1,i2,i3,wc)+alpha*uz23(i1,i2,i3,pc)/rho(i1,i2,i3))

      else

        write(*,'("asfAddGradP:ERROR: unexpected nd or gridType")')
        stop 679

      end if


      return
      end



      subroutine asfAssignPressureRhs(nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
          ipar,rpar, f, u,p, gam, mask, rsxy, ierr )         
c========================================================================
c
c
c  %%%%%%%% Assign the RHS for the pressure equation %%%%%%%%
c
c    -> RHS = p(t) +  alpha*p* div( u )  for constant gamma
c OR -> RHS = p(t) +  alpha*p*gam*div( u )  for variable gamma
c
c          where normally u = u(t) + dt*( u.grad(u) + ... )
c
c  f : rhs to assign 
c  u : solution to take the divergence of 
c  p : pressure to use as the coefficient (this may be the true pressure or a linearized version.)
c  gam : for variable gas constant gamma
c
c 
c wdh: 0701
c========================================================================
      implicit none
      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real p(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real gam(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real rpar(0:*)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer ipar(0:*),ierr

c.......local

      integer kd,kd3,i1,i2,i3,n1a,n1b,n2a,n2b,n3a,n3b
      integer rc,tc,uc,vc,wc,pc,sc
      integer grid,orderOfAccuracy,gridType,gridIsMoving,useWhereMask,variableGamma

      real dr(0:2),dx(0:2)

      real t,dt,alpha,pressureLevel,divu

      integer debug


c..................
      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )


      ! declare variables for difference approximations of u and RX
      declareDifferenceOrder2(u,RX)

c .............. begin statement functions
      real rx,ry,rz,sx,sy,sz,tx,ty,tz
      real ogf,diss2,ad2,disst2,tanDiss2

c.......statement functions for jacobian
      rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
      ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
      rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
      sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
      sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
      sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
      tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
      ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
      tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

c     The next macro call will define the difference approximation statement functions
      defineDifferenceOrder2Components1(u,RX)
          
c     --- end statement functions

c .............. end statement functions


      ierr=0
      ! write(*,*) 'Inside asfSlipWallBC'

      rc                =ipar(0)
      uc                =ipar(1)
      vc                =ipar(2)
      wc                =ipar(3)
      tc                =ipar(4)
      pc                =ipar(5)
      grid              =ipar(6)
      gridType          =ipar(7)
      orderOfAccuracy   =ipar(8)
      gridIsMoving      =ipar(9) 
      useWhereMask      =ipar(10)
      n1a               =ipar(11)
      n1b               =ipar(12)
      n2a               =ipar(13)
      n2b               =ipar(14)
      n3a               =ipar(15)
      n3b               =ipar(16)
      variableGamma     =ipar(17)

      dx(0)             =rpar(0)
      dx(1)             =rpar(1)
      dx(2)             =rpar(2)
      dr(0)             =rpar(3)
      dr(1)             =rpar(4)
      dr(2)             =rpar(5)
      t                 =rpar(6)
      dt                =rpar(7)
      alpha             =rpar(8)
      pressureLevel     =rpar(9)

c      write(*,'(" **** asfAssignPressureRhs: rc,pc,variableGamma,n1a,n1b,n2a,n2b,",3i2,4i5," t,dt,alpha,pressureLevel=",4f7.3)') \
c          rc,pc,variableGamma,n1a,n1b,n2a,n2b,t,dt,alpha,pressureLevel
c      ! ' 

      if( nd.eq.2 .and. gridType.eq.rectangular )then

        ! 2D rectangular
       if( variableGamma.eq.0 )then
        loopse2(divu=ux22r(i1,i2,i3,uc)+uy22r(i1,i2,i3,vc),\
                f(i1,i2,i3)=u(i1,i2,i3,pc)+alpha*(p(i1,i2,i3)+pressureLevel)*divu)
       else
        loopse2(divu=ux22r(i1,i2,i3,uc)+uy22r(i1,i2,i3,vc),\
                f(i1,i2,i3)=u(i1,i2,i3,pc)+alpha*(p(i1,i2,i3)+pressureLevel)*gam(i1,i2,i3)*divu)
       end if

      else if( nd.eq.2 .and. gridType.eq.curvilinear )then
     
        ! 2D curvilinear

       if( variableGamma.eq.0 )then
        loopse2(divu=ux22(i1,i2,i3,uc)+uy22(i1,i2,i3,vc),\
                f(i1,i2,i3)=u(i1,i2,i3,pc)+alpha*(p(i1,i2,i3)+pressureLevel)*divu)
       else
        loopse2(divu=ux22(i1,i2,i3,uc)+uy22(i1,i2,i3,vc),\
                f(i1,i2,i3)=u(i1,i2,i3,pc)+alpha*(p(i1,i2,i3)+pressureLevel)*gam(i1,i2,i3)*divu)
       end if

      else if( nd.eq.3 .and. gridType.eq.rectangular )then

        ! 3D rectangular

       if( variableGamma.eq.0 )then
        loopse2(divu=ux23r(i1,i2,i3,uc)+uy23r(i1,i2,i3,vc)+uz23r(i1,i2,i3,wc),\
                f(i1,i2,i3)=u(i1,i2,i3,pc)+alpha*(p(i1,i2,i3)+pressureLevel)*divu)
       else
        loopse2(divu=ux23r(i1,i2,i3,uc)+uy23r(i1,i2,i3,vc)+uz23r(i1,i2,i3,wc),\
                f(i1,i2,i3)=u(i1,i2,i3,pc)+alpha*(p(i1,i2,i3)+pressureLevel)*gam(i1,i2,i3)*divu)
       end if

      else if( nd.eq.3 .and. gridType.eq.curvilinear )then
     
        ! 3D curvilinear

       if( variableGamma.eq.0 )then
        loopse2(divu=ux23(i1,i2,i3,uc)+uy23(i1,i2,i3,vc)+uz23(i1,i2,i3,wc),\
                f(i1,i2,i3)=u(i1,i2,i3,pc)+alpha*(p(i1,i2,i3)+pressureLevel)*divu)
       else
        loopse2(divu=ux23(i1,i2,i3,uc)+uy23(i1,i2,i3,vc)+uz23(i1,i2,i3,wc),\
                f(i1,i2,i3)=u(i1,i2,i3,pc)+alpha*(p(i1,i2,i3)+pressureLevel)*gam(i1,i2,i3)*divu)
       end if

      else

        write(*,'("asfAssignPressureRhs:ERROR: unexpected nd or gridType")')
        stop 679

      end if


      return
      end

