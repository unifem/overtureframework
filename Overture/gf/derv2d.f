      subroutine uhatxy( uhat, uxy,ixy )
c===================================================================
c       Given a the Transform uhat(kx,ky) of a function u
c       Compute the x or y Derivative in Transform Space
c PURPOSE
c  Take an x or y derivative in Fourier space. This amounts
c  to the appropriate multiplication of the cosine transform uhat
c INPUT
c  uhat(k1,k2)
c  ixy  -  >=  0  x derivative
c          <   0  y derivative
c OUTPUT
c  uxy(i,j)  - derivative in Fourier space
c===================================================================
c
      include 'nsblk.h'
      real uhat(nd,1),uxy(nd,1)
c
      if( ixy.ge.0. )then
c       x derivative
        do 200 ky=1,ny
          uxy(1,ky)=0.
          do 100 kx=2,nx-1,2
            p=kx*0.5
            uxy(kx,ky)=-uhat(kx+1,ky)*p
            uxy(kx+1,ky)=uhat(kx,ky)*p
 100      continue
          uxy(nx,ky)=0.
 200    continue
      else
c       y derivative
        do 400 kx=1,nx
          uxy(kx,1)=0.
          do 300 ky=2,ny-1,2
            p=ky*0.5
            uxy(kx,ky)=-uhat(kx,ky+1)*p
            uxy(kx,ky+1)=uhat(kx,ky)*p
 300      continue
          uxy(kx,ny)=0.
 400    continue
      end if
c
      return
      end

      subroutine delhat( what,wxxyy )
c===================================================================
c      Compute the Laplacian in Fourier Space
c INPUT
c  what
c OUTPUT
c  wxxyy   - the laplacian of what in Fourier space
c===================================================================
c
      include 'nsblk.h'
      real what(nd,1),wxxyy(nd,1)
c
      do 200 kx=1,nx
        ipx=(kx/2)**2
        do 100 ky=1,ny
          wxxyy(kx,ky)= -what(kx,ky)*(ipx+(ky/2)**2)*anu
 100    continue
 200  continue
c
      return
      end

      subroutine strhat( what,psihat )
c===================================================================
c     Compute psihat from what in Fourier Space
c PURPOSE
c  The operation of inverting
c             psi.xx + psi.yy = - w
c  for psi takes the form of a division in Fourier space
c
c INPUT
c  what - vorticity in Fourier space
c OUTPUT
c  psihat - streamfunction in Fourier space
c===================================================================
c
      include 'nsblk.h'
      real what(nd,1),psihat(nd,1)
c
c        arbitrary constant set to zero
      psihat(1,1)=0.
      kx=1
      do 100 ky=2,ny
        psihat(kx,ky)= what(kx,ky)/((ky/2)**2)
 100  continue
c
      do 300 kx=2,nx
        ipx=(kx/2)**2
        do 200 ky=1,ny
          psihat(kx,ky)= what(kx,ky)/(ipx+(ky/2)**2)
 200    continue
 300  continue
c
      return
      end
c@process dc(wtblk)
c      subroutine smooth( w )
cc===================================================================
cc      "Smooth" the vorticity
cc   Smooth higher frequencies
cc                            May 1986.
cc===================================================================
c      include 'nsblk h'
c      real w(nd,1)
c      include 'wtblk h'
c
cc       smooth top 4 modes : x-direction
c      nx1=nx-3
cc       smooth top 4 modes : y-direction
c      ny1=ny-3
c      if( nx1.le. nx .or. ny1.le.ny )then
cc           w -> what
c        call fft2df( w,what )
cc        write(6,*) 'SMOOTH: Before smoothing'
cc        write(6,9000) ((w(i,j),i=1,nx),j=1,ny)
cc        write(6,9100) ((what(i,j),i=1,nx),j=1,ny)
c        do 100 j=1,ny
c          p=1./nx
c          do 100 i=nx1,nx
c            what(i,j)=what(i,j)*(nx-i)*p
c          continue
c 100    continue
c        do 200 i=1,nx
c          p=1./ny
c          do 200 j=ny1,ny
c            what(i,j)=what(i,j)*(ny-j)*p
c          continue
c 200    continue
cc           what -> w
c        call fft2db( what,w )
cc        write(6,*) 'SMOOTH: After smoothing'
cc        write(6,9100) ((what(i,j),i=1,nx),j=1,ny)
cc        write(6,9000) ((w(i,j),i=1,nx),j=1,ny)
c      end if
cc 9000 format(5x,'w(i,j) =',/,<ny>(<nx>(1x,e9.3),/) )
cc 9100 format(5x,'what(i,j) =',/,<ny>(<nx>(1x,e9.3),/) )
c      return
c      end
