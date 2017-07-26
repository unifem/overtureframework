!
!  Evaluate the bessel function J_nu(z) with complex argument z=(zr,ri)
!  Return J = (jr,ji)
!
      subroutine cBesselJ( nu, zr,zi, jr,ji )

      double precision nu, zr,zi,jr,ji

      double precision cjr(10), cji(10)
      integer np,nz,ierr,kode

      kode=1 ! do not scale result by exp(-abs(zi))
      np=1 ! we just want J_nu 
      call zbesj( zr,zi,nu,kode,np,cjr,cji,nz,ierr)
      if( nz.ne.0 .or. ierr.ne.0 )then
        write(*,'("WARNING: zbesj: nz,ierr=",2i4)') nz,ierr
      end if

      jr=cjr(1)
      ji=cji(1)

      return 
      end 

!
!  Evaluate the bessel function Y_nu(z) with complex argument z=(zr,ri)
!  Return Y = (yr,yi)
!
      subroutine cBesselY( nu, zr,zi, yr,yi )

      double precision nu, zr,zi,yr,yi

      double precision cyr(10), cyi(10),cwkr(10),cwrki(10)
      integer np,nz,ierr,kode

      kode=1 ! do not scale result by exp(-abs(zi))
      np=1 ! we just want Y_nu 
      call zbesy( zr,zi,nu,kode,np,cyr,cyi,nz,
     &   cwkr,cwrki,ierr)
      if( nz.ne.0 .or. ierr.ne.0 )then
        write(*,'("WARNING: zbesy: nz,ierr=",2i4)') nz,ierr
      end if
      !write(*,'(" zbesy: nz,ierr=",2i4)') nz,ierr
      !write(*,'(" zbesy: Y=",2e12.4)') cyr(1),cyi(1)

      yr=cyr(1)
      yi=cyi(1)

      return 
      end 
