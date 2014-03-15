c-------------------------------------------------------------------------
c  These are null subroutines to go with CGES
c-------------------------------------------------------------------------
      subroutine dsris( flag,initc,neq,a,ja,ia,rhs,sol,iparm,rparm,
     &   wk1,naux1,wk2,naux2 )
      character*(*) flag
      write(*,'('' ERROR: you are using the null version of DSRIS'')')
      write(*,'('' If you have ESSL then remove the dummy DSRIS'')')
      write(*,'('' found in CGESNULL.f '')')
      stop 'DSRIS'
      end
