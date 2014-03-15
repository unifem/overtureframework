! This file automatically generated from advOptSm.bf with bpp.
c
c Advance the equations of solid mechanics
c
c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 2 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX



c Use this next macro to declare the statement functions that are defined below
c To include derivatives of rx use OPTION=RX


c Define statement functions for difference approximations of order 4 
c To include derivatives of rx use OPTION=RX
c To include derivatives of rx use OPTION=RX

! ogf2d, ogf3d, ogDeriv2, etc. are foundin forcing.bC


! ntd,nxd,nyd,nzd : number of derivatives to evaluate in t,x,y,z








c This macro is used for variable dissipation in 2D

c This macro is used for variable dissipation in 3D

c This macro is used for variable dissipation in 3D



c Optionally add the forcing terms

c Optionally add the forcing terms
c Optionally solve for E or H or both


c Optionally add the dissipation and or forcing terms


c Optionally add add the dissipation and or forcing terms
c Optionally solve for E or H or both

c The next macro is used for curvilinear girds where the Laplacian term is precomputed.










c ** evaluate the laplacian on the 9 points centred at (i1,i2,i3)


c ** evaluate the square of the Laplacian for a component ****

c ** evaluate the square of the Laplacian for [ex,ey,hz] ****

c **********************************************************************************
c NAME: name of the subroutine
c DIM : 2 or 3
c ORDER : 2 ,4, 6 or 8
c GRIDTYPE : rectangular, curvilinear
c **********************************************************************************




c**
c**      buildFile(advSm22Order6r,2,6,rectangular)
c**      buildFile(advSm23Order6r,3,6,rectangular)
c**
c**       ! build these for testing symmetric operators -- BC's not implemented yet
c**      buildFile(advSm22Order6c,2,6,curvilinear)
c**      buildFile(advSm23Order6c,3,6,curvilinear)
c**
c**      buildFile(advSm22Order8r,2,8,rectangular)
c**      buildFile(advSm23Order8r,3,8,rectangular)
c**
c**       ! build these for testing symmetric operators -- BC's not implemented yet
c**      buildFile(advSm22Order8c,2,8,curvilinear)
c**      buildFile(advSm23Order8c,3,8,curvilinear)






      subroutine advSM(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,
     & nd3a,nd3b,nd4a,nd4b,mask,rx,xy,  um,u,un,f, ndMatProp,matIndex,
     & matValpc,matVal,bc, dis, varDis, ipar, rpar, ierr )
c======================================================================
c   Advance a time step for the equations of Solid Mechanics (linear elasticity for now)
c
c nd : number of space dimensions
c
c ipar(0)  = option : option=0 - SM+Artificial diffusion
c                           =1 - AD only
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

      real um(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real un(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real f(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
      real dis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real varDis(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rx(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer bc(0:1,0:2),ierr

      integer ipar(0:*)
      real rpar(0:*)

      ! -- Declare arrays for variable material properties --
      include 'declareVarMatProp.h'

c     ---- local variables -----
      real dt,dtOld
      integer c,i1,i2,i3,n,gridType,orderOfAccuracy,orderInTime,
     & useConservative
      integer addForcing,orderOfDissipation,option
      integer useWhereMask,solveForE,solveForH,grid

      integer rectangular,curvilinear
      parameter( rectangular=0, curvilinear=1 )
c...........end   statement functions


      ! write(*,*) 'Inside advSM...'
      dt    =rpar(0)
      dtOld =rpar(15) ! dt used on the previous time step

      gridType           =ipar(1)
      orderOfAccuracy    =ipar(2)
      useConservative    =ipar(12)

      ! write(*,'(" advOpt: gridType=",i2," useConservative=",i2)') gridType,useConservative
      if( abs(dt-dtOld).gt.dt*.001 .and. orderOfAccuracy.ne.2 )then
       write(*,'(" advSM:ERROR: variable dt not implemented yet for 
     & this case")')
       write(*,'("            : dt,dtOld,diff=",3e9.3)') dt,dtOld,dt-
     & dtOld
       write(*,'("              orderOfAccuracy=",i4," 
     & useConservative=",i4)') orderOfAccuracy,useConservative
       ! '
       stop 9027
      end if


      if( orderOfAccuracy.eq.2 )then
       if( useConservative.eq.1 )then
        ! Conservative (self-adjoint) approximations from Daniel
        if( nd.eq.2 .and. gridType.eq.rectangular ) then
          call advSmCons2dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx,xy, um,u,un,f, ndMatProp,
     & matIndex,matValpc,matVal, bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.2 .and. gridType.eq.curvilinear ) then
          call advSmCons2dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx,xy, um,u,un,f, ndMatProp,
     & matIndex,matValpc,matVal,bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.3 .and. gridType.eq.rectangular ) then
          call advSmCons3dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx,xy, um,u,un,f, ndMatProp,
     & matIndex,matValpc,matVal,bc, dis,varDis, ipar, rpar, ierr )
        else if( nd.eq.3 .and. gridType.eq.curvilinear ) then
          call advSmCons3dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx,xy, um,u,un,f, ndMatProp,
     & matIndex,matValpc,matVal,bc, dis,varDis, ipar, rpar, ierr )
        else
          stop 2271
        end if
       else
        ! non-conservative approximations 
        if( nd.eq.2 .and. gridType.eq.rectangular ) then
          call advSm2dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx,xy, um,u,un,f, bc, dis,
     & varDis, ipar, rpar, ierr )
        else if( nd.eq.2 .and. gridType.eq.curvilinear ) then
          call advSm2dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx,xy, um,u,un,f, bc, dis,
     & varDis, ipar, rpar, ierr )
        else if( nd.eq.3 .and. gridType.eq.rectangular ) then
          call advSm3dOrder2r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx,xy, um,u,un,f, bc, dis,
     & varDis, ipar, rpar, ierr )
        else if( nd.eq.3 .and. gridType.eq.curvilinear ) then
          call advSm3dOrder2c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx,xy, um,u,un,f, bc, dis,
     & varDis, ipar, rpar, ierr )
        else
          stop 2271
        end if
       end if

      else if( orderOfAccuracy.eq.4 ) then
        if( nd.eq.2 .and. gridType.eq.rectangular )then
          call advSm2dOrder4r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx,xy, um,u,un,f, bc, dis,
     & varDis, ipar, rpar, ierr )
        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
          call advSm2dOrder4c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx,xy, um,u,un,f, bc, dis,
     & varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
          call advSm3dOrder4r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx,xy, um,u,un,f, bc, dis,
     & varDis, ipar, rpar, ierr )
        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
          call advSm3dOrder4c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rx,xy, um,u,un,f, bc, dis,
     & varDis, ipar, rpar, ierr )
       else
         stop 8843
       end if

c**c
c**      else if( orderOfAccuracy.eq.6 ) then
c**        if( nd.eq.2 .and. gridType.eq.rectangular )then
c**          call advSm2dOrder6r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
c**          call advSm2dOrder6c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
c**          call advSm3dOrder6r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
c**          call advSm3dOrder6c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**       else
c**         stop 8843
c**       end if
c**
c**      else if( orderOfAccuracy.eq.8 ) then
c**
c**        if( nd.eq.2 .and. gridType.eq.rectangular )then
c**          call advSm2dOrder8r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**        else if(nd.eq.2 .and. gridType.eq.curvilinear )then
c**          call advSm2dOrder8c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**        else if(  nd.eq.3 .and. gridType.eq.rectangular )then
c**          call advSm3dOrder8r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**        else if(  nd.eq.3 .and. gridType.eq.curvilinear )then
c**          call advSm3dOrder8c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,c**                              mask,rx,xy, um,u,un,f, bc, dis,varDis, ipar, rpar, ierr )
c**       else
c**         stop 8843
c**       end if

      else
        write(*,'(" advSM:ERROR: un-implemented order of accuracy =",
     & i6)') orderOfAccuracy
          ! '
        stop 11222
      end if

      return
      end








