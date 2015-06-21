! This file automatically generated from advOpt.bf with bpp.
        subroutine advMx2dOrder8c(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,mask,rsxy,  um,u,un,f,fa, v,vvt2,
     & ut3,vvt4,ut5,ut6,ut7, bc, dis, varDis, ipar, rpar, ierr )
c======================================================================
c   Advance a time step for Maxwells eqution
c     OPTIMIZED version for rectangular grids.
c nd : number of space dimensions
c
c ipar(0)  = option : option=0 - Maxwell+Artificial diffusion
c                           =1 - AD only
c
c  dis(i1,i2,i3) : temp space to hold artificial dissipation
c  varDis(i1,i2,i3) : coefficient of the variable artificial dissipation
c======================================================================
         return
         end
