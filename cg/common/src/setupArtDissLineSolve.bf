c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"


#beginMacro beginLoop(n1a,n1b,n2a,n2b,n3a,n3b)
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b 
#endMacro
#beginMacro beginLoopJ(n1a,n1b,n2a,n2b,n3a,n3b)
do j3=n3a,n3b
do j2=n2a,n2b
do j1=n1a,n1b 
#endMacro
#beginMacro endLoop()
end do
end do
end do
#endMacro


c return the loop indicies for the "boundary" (side,axis) shifted by "shift"
#beginMacro getBoundaryIndex(side,axis,m1a,m1b,m2a,m2b,m3a,m3b,shift)
 m1a=n1a
 m1b=n1b
 m2a=n2a
 m2b=n2b
 m3a=n3a
 m3b=n3b
 if( axis.eq.0 )then
  if( side.eq.0 )then
    m1a=n1a+shift
  else
    m1a=n1b-shift
  end if
  m1b=m1a
 else if( axis.eq.1 )then
  if( side.eq.0 )then
    m2a=n2a+shift
  else
    m2a=n2b-shift
  end if
  m2b=m2a
 else
  if( side.eq.0 )then
    m3a=n3a+shift
  else
    m3a=n3b-shift
  end if
  m3b=m3a
 end if
#endMacro

c DIM: 2 or 3
c GRID_TYPE: rectangular or curvilinear
#beginMacro fillPentaDiagonalMatrix(DIM,GRID_TYPE,ORDER_OF_ACCURACY)
  beginLoop(n1a,n1b,n2a,n2b,n3a,n3b)
  if( mask(i1,i2,i3) .gt. 0 ) then
    #If #DIM == "2"
      #If #GRID_TYPE == "rectangular"
        ad = dt*ad4Coeffr(i1,i2,i3)
!        ad2=ad*.1
!       ad2= dt*aDivCoeffr(i1,i2,i3)
!       ad2= dt*aDivDerivCoeffr(i1,i2,i3)
!       ad2= dt*aLapCoeff(i1,i2,i3)
        ad2=0.
      #Else
        ad = dt*ad4Coeff(i1,i2,i3)
!        ad2=ad*.1
!        ad2= dt*aDivCoeff(i1,i2,i3)
!        ad2= dt*aDivDerivCoeff(i1,i2,i3)
!        ad2= dt*aLapCoeff(i1,i2,i3)
        ad2=0.
      #End
    #Else
      #If #GRID_TYPE == "rectangular"
        ad = dt*ad43Coeffr(i1,i2,i3)
        ad2= 0.
      #Else
        ad = dt*ad43Coeff(i1,i2,i3)
        ad2= 0.
      #End
    #End
    a(i1,i2,i3)=        ad
    b(i1,i2,i3)=    -4.*ad    -ad2
    c(i1,i2,i3)=1. + 6.*ad +2.*ad2
    d(i1,i2,i3)=    -4.*ad    -ad2
    e(i1,i2,i3)=        ad
   else if( mask(i1,i2,i3) .lt. 0 ) then 
    #If #DIM == "2"
      #If #GRID_TYPE == "rectangular"
        ad = dt*ad4Coeffr(i1,i2,i3)
        ad2=.1*ad
        ad2=0.
      #Else
        ad = dt*ad4Coeff(i1,i2,i3)
        ad2=.1*ad
        ad2=0.
      #End
    #Else
      #If #GRID_TYPE == "rectangular"
        ad = dt*ad43Coeffr(i1,i2,i3)
        ad2=.1*ad
        ad2=0.
      #Else
        ad = dt*ad43Coeff(i1,i2,i3)
        ad2=.1*ad
        ad2=0.
      #End
    #End
    a(i1,i2,i3)=        0. 
    b(i1,i2,i3)=       -ad2
    c(i1,i2,i3)=1. + 2.*ad2
    d(i1,i2,i3)=       -ad2
    e(i1,i2,i3)=        0.

c$$$  else if( mask(i1+is1,i2+is2,i3+is3) .lt. 0 ) then  ! ***************** should we do this ?? or extrapInterpNeigh
c$$$                          ! extrap (i1,i2,i3) in the direction (is1,is2,is3)
c$$$    a(i1,i2,i3)=0.
c$$$    b(i1,i2,i3)=0.
c$$$    c(i1,i2,i3)=1.
c$$$    d(i1,i2,i3)=-2.
c$$$    e(i1,i2,i3)=1.
c$$$  else if( mask(i1-is1,i2-is2,i3-is3) .lt. 0 ) then
c$$$                          ! extrap (i1,i2,i3) in the direction (-is1,-is2,-is3)
c$$$    a(i1,i2,i3)=1.
c$$$    b(i1,i2,i3)=-2.
c$$$    c(i1,i2,i3)=1.
c$$$    d(i1,i2,i3)=0.
c$$$    e(i1,i2,i3)=0.
  else
    a(i1,i2,i3)=0.
    b(i1,i2,i3)=0.
    c(i1,i2,i3)=1.
    d(i1,i2,i3)=0.
    e(i1,i2,i3)=0.
  end if
  endLoop()
#endMacro

      subroutine setupArtDissLineSolve( nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,
     &    nda1a,nda1b,nda2a,nda2b,nda3a,nda3b,
     &    a, b, c, d, e, u, mask, rsxy, ipar, rpar )
c ===================================================================================
c Setup the pentadiagonal system for the fourth-order artificial dissipation
c
c  a,b,c,d,e : for pentadiagonal
c ===================================================================================

      implicit none
      integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b
      integer nda1a,nda1b,nda2a,nda2b,nda3a,nda3b

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer ipar(0:*)

      real a(nda1a:nda1b,nda2a:nda2b,nda3a:nda3b)
      real b(nda1a:nda1b,nda2a:nda2b,nda3a:nda3b)
      real c(nda1a:nda1b,nda2a:nda2b,nda3a:nda3b)
      real d(nda1a:nda1b,nda2a:nda2b,nda3a:nda3b)
      real e(nda1a:nda1b,nda2a:nda2b,nda3a:nda3b)

      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real rpar(0:*)

c....local variables
      integer n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c
      integer orderOfAccuracy
      integer uc,vc,wc
      integer i1,i2,i3,m1a,m1b,m1c,m2a,m2b,m2c,m3a,m3b,m3c,j1,j2,j3,is1,is2,is3,kd
      integer l1a,l1b,l2a,l2b,l3a,l3b
      integer grid

      integer direction,gridType
      integer side,axis,axisp1,axisp2
      integer bc(0:1,0:2)
      real dx(0:2),dr(0:2)

      real cexa,cexb,cexc,cexd,cexe

      integer dirichlet,neumann,mixed,equation,extrapolation,combination 
      parameter( 
     &     dirichlet=1,
     &     neumann=2,
     &     mixed=3,
     &     equation=4,
     &     extrapolation=5,
     &     combination=6 )

      integer rectangular,curvilinear
      parameter(
     &     rectangular=0,
     &     curvilinear=1)

      real ad41,ad42,cd42,dt,ad,ad2
      real ad4Coeff,ad4Coeffr,ad43Coeff,ad43Coeffr,aDivCoeff,aDivCoeffr,aLapCoeff,aDivDerivCoeff,aDivDerivCoeffr

      real rx,ry,rz,sx,sy,sz,tx,ty,tz
c      include 'declareDiffOrder2f.h'
c      include 'declareDiffOrder4f.h'
 
      declareDifferenceOrder2(u,RX)
      declareDifferenceOrder4(u,RX)

c....start statement functions 

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
      defineDifferenceOrder4Components1(u,RX)

c     ---fourth-order artificial diffusion in 2D
      ad4Coeff(i1,i2,i3)=(ad41 + cd42*    
     &    ( abs(ux22(i1,i2,i3,uc))+abs(uy22(i1,i2,i3,uc))    
     &     +abs(ux22(i1,i2,i3,vc))+abs(uy22(i1,i2,i3,vc)) ) )
      ad4Coeffr(i1,i2,i3)=(ad41 + cd42*    
     &    ( abs(ux22r(i1,i2,i3,uc))+abs(uy22r(i1,i2,i3,uc))    
     &     +abs(ux22r(i1,i2,i3,vc))+abs(uy22r(i1,i2,i3,vc)) ) )
c     ---fourth-order artificial diffusion in 3D
      ad43Coeff(i1,i2,i3)=
     &   (ad41 + cd42*    
     &    ( abs(ux23(i1,i2,i3,uc))+abs(uy23(i1,i2,i3,uc))+abs(uz23(i1,i2,i3,uc))    
     &     +abs(ux23(i1,i2,i3,vc))+abs(uy23(i1,i2,i3,vc))+abs(uz23(i1,i2,i3,vc))    
     &     +abs(ux23(i1,i2,i3,wc))+abs(uy23(i1,i2,i3,wc))+abs(uz23(i1,i2,i3,wc)) ) )
      ad43Coeffr(i1,i2,i3)=
     &   (ad41 + cd42*    
     &    ( abs(ux23r(i1,i2,i3,uc))+abs(uy23r(i1,i2,i3,uc))+abs(uz23r(i1,i2,i3,uc))    
     &     +abs(ux23r(i1,i2,i3,vc))+abs(uy23r(i1,i2,i3,vc))+abs(uz23r(i1,i2,i3,vc))    
     &     +abs(ux23r(i1,i2,i3,wc))+abs(uy23r(i1,i2,i3,wc))+abs(uz23r(i1,i2,i3,wc)) ) )

c....... for 2nd-order dissipation based on the divergence
      aDivCoeff(i1,i2,i3) =ad42*abs(ux22 (i1,i2,i3,uc)+uy22 (i1,i2,i3,vc)) 
      aDivCoeffr(i1,i2,i3)=ad42*abs(ux22r(i1,i2,i3,uc)+uy22r(i1,i2,i3,vc)) 

c     ..based on the derivatives of the divergence
      aDivDerivCoeff(i1,i2,i3) =ad42*(abs(uxx22 (i1,i2,i3,uc)+uxy22 (i1,i2,i3,vc)) +
     &                                abs(uxy22 (i1,i2,i3,uc)+uyy22 (i1,i2,i3,vc)) )
      aDivDerivCoeffr(i1,i2,i3)=ad42*(abs(uxx22r(i1,i2,i3,uc)+uxy22r(i1,i2,i3,vc)) +
     &                                abs(uxy22r(i1,i2,i3,uc)+uyy22r(i1,i2,i3,vc)) )
c....... for 2nd-order dissipation based on the laplacian
      aLapCoeff(i1,i2,i3)= .5*ad42*( 
     &   abs(u(i1-1,i2,i3,uc)+u(i1+1,i2,i3,uc)+u(i1,i2-1,i3,uc)+u(i1,i2+1,i3,uc)-4.*u(i1,i2,i3,uc)) +
     &   abs(u(i1-1,i2,i3,vc)+u(i1+1,i2,i3,vc)+u(i1,i2-1,i3,vc)+u(i1,i2+1,i3,vc)-4.*u(i1,i2,i3,vc)) )
c....end statement function

      n1a             =ipar(0)
      n1b             =ipar(1)
      n1c             =ipar(2)
      n2a             =ipar(3)
      n2b             =ipar(4)
      n2c             =ipar(5)
      n3a             =ipar(6)
      n3b             =ipar( 7)
      n3c             =ipar( 8)
      bc(0,0)         =ipar( 9)
      bc(1,0)         =ipar(10)
      bc(0,1)         =ipar(11)
      bc(1,1)         =ipar(12)
      bc(0,2)         =ipar(13)
      bc(1,2)         =ipar(14)
      direction       =ipar(15)
      orderOfAccuracy =ipar(16)
      gridType        =ipar(17)
      uc              =ipar(18)
      vc              =ipar(19)
      wc              =ipar(20)

      dx(0)           =rpar(0)
      dx(1)           =rpar(1)
      dx(2)           =rpar(2)
      dr(0)           =rpar(3) 
      dr(1)           =rpar(4)
      dr(2)           =rpar(5)

      ad41            =rpar(6)
      ad42            =rpar(7)
      dt              =rpar(8)

      cd42=ad42/(nd**2)

      if( uc.lt.nd4a .or. uc.gt.nd4b .or. vc.lt.nd4a .or. vc.gt.nd4b .or. \
          (nd.eq.3 .and. (wc.lt.nd4a .or. wc.gt.nd4b)) )then
        write(*,'("setupArtDissLineSolve:ERROR: invalid values for uc,vc or wc=",3i5)') uc,vc,wc
        stop 21
      end if
      if( dx(0).lt.0. .or. dx(1).lt.0. .or. dx(2).lt.0 )then
        write(*,'("setupArtDissLineSolve:ERROR: invalid values for dx=",3e10.2)') dx(0),dx(1),dx(2)
        stop 22
      end if
      if( dr(0).lt.0. .or. dr(1).lt.0. .or. dr(2).lt.0 )then
        write(*,'("setupArtDissLineSolve:ERROR: invalid values for dr=",3e10.2)') dr(0),dr(1),dr(2)
        stop 22
      end if
      if( ad41.lt.0. .or. ad42.lt.0. .or. dt.lt.0. )then
        write(*,'("setupArtDissLineSolve:ERROR: invalid values one of ad41,ad42,dt",3e10.2)') ad41,ad42,dt
        stop 23
      end if


      axis=direction 
      axisp1=mod(axis+1,nd)
      axisp2=mod(axis+2,nd)

      is1=0
      is2=0
      is3=0
      if( direction.eq.0 )then
        is1=1
      else if( direction.eq.1 )then
        is2=1
      else
        is3=1
      end if


      if( orderOfAccuracy.eq.2 )then

        if( gridType.eq.rectangular .and. nd.eq.2 )then
          fillPentaDiagonalMatrix(2,rectangular,2)
        else if( gridType.eq.rectangular .and. nd.eq.3 )then
          fillPentaDiagonalMatrix(3,rectangular,2)
        else if( gridType.eq.curvilinear .and. nd.eq.2 )then
          fillPentaDiagonalMatrix(2,curvilinear,2)
        else if( gridType.eq.curvilinear .and. nd.eq.3 )then
          fillPentaDiagonalMatrix(3,curvilinear,2)
        else
          stop 88
        end if

      else if( orderOfAccuracy.eq.4 )then
        stop 66
      else
        stop 77
      end if

      ! fix up boundary conditions 
      axis=direction
      is1=0
      is2=0
      is3=0
      do side=0,1
        if( axis.eq.0 )then
          is1=1-2*side
        else if( axis.eq.1 )then
          is2=1-2*side
        else
          is3=1-2*side
        end if

c$$$        if( bc(side,direction).eq.noSlipWall )then
c$$$          bcType=dirichlet
c$$$        else if( bc(side,direction).lt.0 )then
c$$$          bcType=periodic
c$$$        else if( bc(side,direction).outflow )then
c$$$          bcType=extrapolation
c$$$        else
c$$$          bcType=extrapolation
c$$$        end if

        if( bc(side,direction).ge.0 )then

          ! For now we always fix the boundary value and extrap the first ghost

          getBoundaryIndex(side,axis,l1a,l1b,l2a,l2b,l3a,l3b,0)  ! first ghost line
c      write(*,'(''+++++lineSmoothOpt: bcOptionD,l1='',i2,2x,6i3)') bcOptionD,l1a,l1b,l2a,l2b,l3a,l3b

          if( side.eq.0 )then
            ! 1st ghost line on left:
            !       [  c  d  e  a  b ]
            !      i= -1 -0  1  2  3
            cexc= 1.
            cexd=-3.
            cexe= 3.
            cexa=-1.
            cexb= 0.
          else
            ! 1st ghost line on right:
            !       [  d  e  a  b  c ]
            cexc= 1.
            cexb=-3.
            cexa= 3.
            cexe=-1.
            cexd= 0.
          end if
c           Don't change the boundary value or ghost line value for now
          beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
            a(i1,i2,i3)=0.
            b(i1,i2,i3)=0.
            c(i1,i2,i3)=1.
            d(i1,i2,i3)=0.
            e(i1,i2,i3)=0.
            j1=i1+is1           ! (j1,j2,j3) is the boundary point
            j2=i2+is2
            j3=i3+is3
            a(j1,j2,j3)=0.
            b(j1,j2,j3)=0.
            c(j1,j2,j3)=1. 
            d(j1,j2,j3)=0.
            e(j1,j2,j3)=0.
          endLoop()
c$$$          beginLoop(l1a,l1b,l2a,l2b,l3a,l3b)
c$$$            a(i1,i2,i3)=cexa
c$$$            b(i1,i2,i3)=cexb
c$$$            c(i1,i2,i3)=cexc
c$$$            d(i1,i2,i3)=cexd
c$$$            e(i1,i2,i3)=cexe
c$$$            j1=i1+is1           ! (j1,j2,j3) is the boundary point
c$$$            j2=i2+is2
c$$$            j3=i3+is3
c$$$            if( mask(j1,j2,j3).gt.0 ) then 
c$$$              a(j1,j2,j3)=0.
c$$$              b(j1,j2,j3)=0.
c$$$              c(j1,j2,j3)=1. 
c$$$              d(j1,j2,j3)=0.
c$$$              e(j1,j2,j3)=0.
c$$$            end if
c$$$          endLoop()

        end if

      end do ! do side

      return
      end      

      subroutine artDissAssignRHS( nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &    r, u, mask, ipar, rpar )
c ===================================================================================
c  Assign the RHS for the line smooth verion of the artificial dissipation
c
c  r : rhs to be filled in
c  
c ===================================================================================

      implicit none
      integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndc,ndbcd

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer ipar(0:*)

      real r(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rpar(0:*)

c....local variables
      integer n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c,sparseStencil,orderOfAccuracy,bcOptionD,bcOptionN
      integer i1,i2,i3,m1a,m1b,m1c,m2a,m2b,m2c,m3a,m3b,m3c,j1,j2,j3,is1,is2,is3
      integer l1a,l1b,l2a,l2b,l3a,l3b,kd,shift
      integer nn1a,nn1b,nn2a,nn2b,nn3a,nn3b

      integer direction
      integer bc(0:1,0:2),gridType
      real dx(0:2),dr(0:2)

      integer dirichlet,neumann,mixed,equation,extrapolation,combination 
      parameter( 
     &     dirichlet=1,
     &     neumann=2,
     &     mixed=3,
     &     equation=4,
     &     extrapolation=5,
     &     combination=6 )

      integer rectangular,curvilinear
      parameter(
     &     rectangular=0,
     &     curvilinear=1)

c....start statement functions 

      nd              =ipar(0)
      direction       =ipar(1)
      orderOfAccuracy =ipar(2)
      n1a             =ipar(3)
      n1b             =ipar(4)
      n1c             =ipar(5)
      n2a             =ipar(6)
      n2b             =ipar(7)
      n2c             =ipar(8)
      n3a             =ipar( 9)
      n3b             =ipar(10)
      n3c             =ipar(11)
      bc(0,0)         =ipar(12)
      bc(1,0)         =ipar(13)
      bc(0,1)         =ipar(14)
      bc(1,1)         =ipar(15)
      bc(0,2)         =ipar(16)
      bc(1,2)         =ipar(17)

      beginLoop(n1a,n1b,n2a,n2b,n3a,n3b)
        if( mask(i1,i2,i3).ne.0 )then
          r(i1,i2,i3)=u(i1,i2,i3)
        else
          r(i1,i2,i3)=0.
        end if 
      endLoop()
      
      return 
      end

