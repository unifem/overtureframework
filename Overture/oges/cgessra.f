      subroutine cgessra( i0,ne,ie,c, neq,ia,ja,a, ia0,ja0,a0,epsz )
c===================================================================
c Sparse Row addition
c        a0(i0,j) <- SUM c(n)*a(ie(n),j)    n=1,...,ne
c
c Row i:   (i,ja(j),a(j))  j=ia(i),...,ia(i+1)-1
c
c Input
c  ia,ja,a : sparse matrix, columns sorted into increasing order
c  ia0,ja0,a0 : output sparse matrix, ia0(i0) should be assigned
c               to be the starting point for the new row, there
c               must be space to hold the new entries
c  epsz : throw away elements samller in absolute value than epsz
c Output
c  ia0(i0+1), ja0(),a0()
c==================================================================
      integer i0,ne,ie(ne),ia(*),ja(*),ia0(*),ja0(*)
      real c(ne),a(*),a0(*),epsz
c........local
      integer ke(100),je(100)

      if( ne.gt.100 )then
        stop 'CGES:CGESSRA: dimension error, ne > 100'
      end if

      k0=ia0(i0)-1
      do n=1,ne
        ke(n)=ia(ie(n))   ! current element in row ie(n)
        je(n)=ja(ke(n))    ! current column in row ie(n)
      end do

      do i=1,neq
c       ---add in the entries with the lowest column index
c       ... jemin=min(je(1),...,je(ne))
        jemin=je(1)
        do n=2,ne
          jemin=min(jemin,je(n))
        end do
        if( jemin.eq.neq+1 ) goto 100  ! finished
        k0=k0+1
        ja0(k0)=jemin
        a0(k0)=0.
c       ...add up all contributions to this column
        do n=1,ne
          if( je(n).eq.jemin )then
            a0(k0)=a0(k0)+c(n)*a(ke(n))
            ke(n)=ke(n)+1     ! increment current element in row ie(n)
            if( ke(n).lt.ia(ie(n)+1) )then
              je(n)=ja(ke(n))
            else
              je(n)=neq+1   ! mark this row as finished
            end if
          end if
        end do
        if( abs(a0(k0)).lt.epsz )then
          a0(k0)=0.
          k0=k0-1
        end if
      end do
 100  continue
      ia0(i0+1)=k0+1  ! start of next row

      end
