! This file automatically generated from insdtKE.bf with bpp.
        subroutine insdtKE3dOrder2(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy,rsxy,radiusInverse, 
     &  u,uu, ut,uti,gv,dw,  bc, ipar, rpar, ierr )
       !======================================================================
       !       EMPTY VERSION for Linking without this Capability
       !
       !   Compute du/dt for the incompressible NS on rectangular grids
       !     OPTIMIZED version for rectangular grids.
       ! nd : number of space dimensions
       !
       ! gv : gridVelocity for moving grids
       ! uu : for moving grids uu is a workspace to hold u-gv, otherwise uu==u
       ! dw : distance to the wall for some turbulence models
       !======================================================================
        implicit none
        integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,nd4a,nd4b
        real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real uu(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real ut(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real uti(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
        real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
        real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
        real radiusInverse(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        integer bc(0:1,0:2),ierr
        integer ipar(0:*)
        real rpar(0:*)
        write(*,'("ERROR: NULL version of subroutine insdtKE3dOrder2 
     & called")')
        write(*,'(" You may have to turn on an option in the 
     & Makefile.")')
        stop 1080
        return
        end
