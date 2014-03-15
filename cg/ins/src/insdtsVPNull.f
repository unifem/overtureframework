! This file automatically generated from insdts.bf with bpp.
        subroutine insdtsVP(nd, n1a,n1b,n2a,n2b,n3a,n3b, nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,xy, rsxy,  u,uu, gv,dw, 
     & divDamping, dtVar, ndMatProp,matIndex,matValpc,matVal, bc, 
     & ipar, rpar, pdb, ierr )
       !======================================================================
       !       EMPTY VERSION for Linking without this Capability
       !
       !    Determine the time step for the INS equations.
       !    ---------------------------------------------
       !
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
        real gv(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
        real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real divDamping(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real dtVar(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)
        real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
        integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        integer bc(0:1,0:2),ierr
        integer ipar(0:*)
        real rpar(0:*)
        double precision pdb  ! pointer to data base
        ! -- arrays for variable material properties --
        integer materialFormat,ndMatProp
        integer matIndex(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real matValpc(0:ndMatProp-1,0:*)
        real matVal(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
        write(*,'("ERROR: NULL version of subroutine insdtsVP called")
     & ')
        write(*,'(" You may have to turn on an option in the 
     & Makefile.")')
        ! ' 
        stop 1060
        return
        end
