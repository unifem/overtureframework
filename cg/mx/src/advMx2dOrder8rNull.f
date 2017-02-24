! This file automatically generated from advOpt.bf with bpp.
        subroutine advMx2dOrder8r(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
       !======================================================================
       !   Advance a time step for Maxwells eqution
       !     OPTIMIZED version for rectangular grids.
       ! nd : number of space dimensions
       !
       ! ipar(0)  = option : option=0 - Maxwell+Artificial diffusion
       !                           =1 - AD only
       !
       !  dis(i1,i2,i3) : temp space to hold artificial dissipation
       !  varDis(i1,i2,i3) : coefficient of the variable artificial dissipation
       !======================================================================
         write(*,'("ERROR: null version of advMx2dOrder8r called")')
         stop 9922
         return
         end
