      subroutine amcof(amc,ord)
c
c  this routine returns the coefficients of the
c  Adams-Moulton methods of order ord
c
      implicit none
      integer ord,i,j,k,l,qord
      double precision amc(-1:ord-2)
      double precision cof(0:11),fc
c
      if (ord.gt.12) then
        print *,' order higher than allowed - 12th order returned '
        qord=12
      else
        qord=ord
      end if
c
c  now Adams-Moulton
c
      do k=2-qord,1
        do i=0,11
          cof(i)=0.d0
        end do
        i=1
        cof(0)=1.d0
        do j=2-qord,1
          if (j.ne.k) then
            fc=1.d0/float(k-j)
            do l=i,0,-1
              if (l.ne.0) then
                cof(l)=-fc*float(j)*cof(l)+fc*cof(l-1)
              else
                cof(l)=-fc*float(j)*cof(l)
              end if
            end do
            i=i+1
          end if
        end do
        amc(-k)=cof(0)
        do l=1,qord-1 
          amc(-k)=amc(-k)+cof(l)/float(l+1)
        end do
      end do
c
      return
      end
