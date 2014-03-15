! This file automatically generated from insLineSolveNew.bf with bpp.
        subroutine lineSolveNewINS(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,md1a,md1b,md2a,md2b,md3a,
     & md3b, mask,rsxy,  u,gv,dt,f,dw,dir,am,bm,cm,dm,em,  bc, 
     & boundaryCondition, ndbcd1a,ndbcd1b,ndbcd2a,ndbcd2b,ndbcd3a,
     & ndbcd3b,ndbcd4a,ndbcd4b,bcData, ipar, rpar, ierr )
c======================================================================
c
c        ****** NULL version **********
c 
c Used if we don't want to compile the real file for a given case
c======================================================================
        write(*,'("ERROR: NULL version of subroutine lineSolveNewINS 
     & called")')
        write(*,'(" You may have to turn on an option in the 
     & Makefile.")')
        ! ' 
        stop 1050
        return
        end
