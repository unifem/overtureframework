c
c  vout = vector0 + (CSR format matrix) X vector1  (used in DSI update)
c  
c  kkc 031223
c
c
      subroutine dsimv(dt,ne,nd,v0,v1,mc,mo,mi,vout)
      
      implicit none

c     dt - the timestep
c     ne - number of entries in the CSR offset array
c     nd - number of dimensions in the problem (not really needed, right?)
c     v0 - the vector to be added
c     v1 - the vector to be multiplied
c     mc - CSR coefficient array
c     mo - CSR offset array
c     mi - CSR index array
c     vout - the output vector
c
c
c     CSR means that the nonzeros in row r are in columns mi( mo(r) ) ... mi( mo(r+1)-1 )
c       
c

      integer ne,nd,mo(*),mi(*)
      real*8 dt,v0(*),v1(*),mc(*),vout(*)
      
      integer r,c

c     .false. means try using sparsekit for a change
      if ( .true. ) then

!$OMP PARALLEL DO PRIVATE(r,c)
         do r = 1,ne
            
            vout(r) = v0(r)
            
            do c = mo(r), mo(r+1)-1
               
               vout(r) = vout(r) + dt*mc(c)*v1(mi(c))
               
c               print *, "MV r, dt*mc*v1 ",r,dt,mc(c),v1(mi(c))
            end do

c            print *,"MV ediff ",vout(r)-v0(r)
c            print *,"MV enew ",r,vout(r)
         end do
!$END PARALLEL DO

      else
         
c     call the corresponding sparesekit routine
         
        write(*,'("ERROR: sparsekit not linked anymore")')
        if ( .true. ) stop 1567
c*wdh* 060529         call amux(ne,v1,vout,mc,mi,mo)
         
         do r=1,ne
            vout(r) = dt*vout(r) + v0(r)
         end do
         
      end if

      return
      end

      subroutine dsimv2(tr,dt,ne,nd,v1,mc,mo,mi,vout)
      
      implicit none

c     tr - if tr>0 then compute the transpose times the vector with tr the column dim of the matrix
c     dt - the timestep
c     ne - number of entries in the CSR offset array
c     nd - number of dimensions in the problem (not really needed, right?)
c     v1 - the vector to be multiplied
c     mc - CSR coefficient array
c     mo - CSR offset array
c     mi - CSR index array
c     vout - the output vector
c     
c
c
c     CSR means that the nonzeros in row r are in columns mi( mo(r) ) ... mi( mo(r+1)-1 )
c       
c
      integer tr,ne,nd,mo(*),mi(*)
      real*8 dt,v1(*),mc(*),vout(*)
      
      integer r,c
      logical transpose

      if ( tr.gt.0 ) then
         transpose = .true.
      else
         transpose = .false.
      end if

c     .false. means try using sparsekit for a change
      if ( .true. ) then


         if ( .not.transpose ) then
!$OMP PARALLEL DO PRIVATE(r,c)
            do r = 1,ne
               
               vout(r) = 0
               
               do c = mo(r), mo(r+1)-1
                  
                  vout(r) = vout(r) + dt*mc(c)*v1(mi(c))
                  
c     print *, "MV r, dt*mc*v1 ",r,dt,mc(c),v1(mi(c))
               end do
               
c            print *,"MV ediff ",vout(r)-v0(r)
c            print *,"MV enew ",r,vout(r)
            end do
!$END PARALLEL DO

         else

            do r=1,tr
               vout(r) = 0
            end do

!$OMP PARALLEL DO PRIVATE(r,c)
            do r = 1,ne
               
               do c = mo(r), mo(r+1)-1
                  
                  vout(mi(c)) = vout(mi(c)) + dt*mc(c)*v1(r)
                  
               end do
               
            end do
!$END PARALLEL DO

         end if
         
      else
         
c     call the corresponding sparesekit routine
         
        write(*,'("ERROR: sparsekit not linked anymore")')
        if ( .true. ) stop 1568
c*wdh* 060529          call amux(ne,v1,vout,mc,mi,mo)
         
         do r=1,ne
            vout(r) = dt*vout(r)
         end do
         
      end if

      return
      end

      subroutine f90_fopen(iunt, fname)
      implicit none
      integer iunt
      character*(*) fname

      open(iunt,FILE=fname,FORM='FORMATTED',STATUS= 'UNKNOWN')

      return
      end

      subroutine f90_fclose(iunt)
      implicit  none
      integer iunt

      close(iunt)

      return 
      end
