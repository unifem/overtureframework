
//       subroutine cgmg1( mxl,l,nv,u,pu,ppu,f,pf,ppf,nu,res,resmx,epsc,
//      & itmax,wkt,pdf,ppdf,eta,delta,lrnv,rplr, mgdir,id,rd,ierr )
// c===================================================================
// c        Mulitgrid on Composite Meshes
// c Input
// c  mxl,l             : maximum number of levels and current level
// c  pu,pf,...   : pointers into the DISKs
// c  u,f               : u and f should be given values
// c  epsc              : level 1 - attempt to get max. residual below epsc
// c  delta             : level >1 - attempt to reduce max. res. by delta
// c  eta               : for smoothers
// c  itmax             : maximum number of iterations
// c  lrnv,rplr         : for left/right null vectors, used in singular case
// c Output
// c  The new solution in u
// c  resmx   : the maximum residual
// c  wkt  : total number of work units used
// c                                  Bill Henshaw June 1985 - 1992
// c===================================================================
void Ogmg::
cycle(const int & level)  // cycle at level l
{
      integer nu(*),pu(ng,*),ppu(*),pf(ng,*),ppf(*),lrnv(2,*),
     & id(*),mgdir,pdf(ng,*),ppdf(*)
      real rd(*),res(ng,*),resmx2,rplr(2,*)
      character u*(*),f*(*)
c.......local
c***
      character*180 str

c.......pass the error message to cgeerr by common
      character*80 errmes
      common/cgmgeb/ errmes
      common/cgmgbl/ info,iopt
      include 'cgmg2.h'
      include 'cgmg.h'

      call second( t0 )
      do k=1,ng
        res(k,l)=0.
      end do

      if( lrnv(1,l).ne.0 )then
c       ---singular problem: project rhs with left null vector
c             f <- f- alpha w
c             alpha = w*f/(w*w)
        call cgmgrf( lrnv(1,l),rplr(1,l),l,pf,id,rd,ierr )
        if(mod(info/8,2).eq.1)then
          write(str,'('' Level'',i1,'' f after project '')') l
          call cgmgpr( l,ng,pf,str,'(1x,80f6.3)',10,id,rd )
        end if
      end if

      minsm=2
      maxsm=5

      if( l.eq.1 .and. l.ne.mxl )then
c       Perform some initial smooths, at least minsm
c       ****
        minsm0=2
        maxsm0=5
        conv=0.
        call cgmgsm( 1,l,ng,pu,pf,minsm0,maxsm0,eta,conv,resmx,wks,
     &   mgdir,id,rd,ierr )
        wkt=wkt+wks
        if( l.eq.1 .and. resmx.lt.epsc ) then
          if( mod(info,2).eq.1 )then
            write(*,'('' CGMG'',i1,'': Convergence reached after '//
     &       'initial smooths'')') l
          end if
          goto 600
        end if
      else
c       set u to zero as initial guess
        call cgmgz( l,ng,pu,id,rd )
      end if

      resmx0=resmx

      wkt=0.
      it0=0
      do it=1,itmax
        wkto=wkt
        resmxo=resmx
        if( l.lt.mxl )then
          if( l.gt.1 .and. it.eq.1 )then
            conv=0.
            call cgmgsm( 1,l,ng,pu,pf,minsm,maxsm,eta,conv,resmx,wks,
     &       mgdir,id,rd,ierr )
            wkt=wkt+wks
          end if

c         ...f2 <- Restriction of df <- defect f - Au
          call cgmgdf( l,nv,ng,pu,pf,pdf,ppdf,res,resmx,mgdir,
     &     id,rd,ierr )
          wkt=wkt+1.

c         === Now solve defect equation on the next level ===
          resmx2=resmx
          wkt2=0.
          call cgmg2( mxl,l+1,nv,u,pu,ppu,f,pf,ppf,nu,res,resmx2,epsc,
     &     itmax,wkt2,pdf,ppdf,eta,delta,lrnv,rplr, mgdir,id,rd,ierr )
          wkt=wkt+wkt2/2**nd

*           if( l+1.eq.mxl )then
*             call cgmgds( l+1,ng,u,pu,ppu,f,pf,ppf,resmx2,wkt2,
*      &       mgdir,id,rd,ierr )
*             wkt=wkt+wkt2/2**nd
*           else
*             write(*,*) 'CGMG?: ERROR Not enough levels implemented!!'
*             if( .true. ) stop 'CGMG?'
*           end if

c         ...Coarse to fine correction
          call cgmgcf( l,nv,u,pu,ppu,id,rd,mgdir )
          wkt=wkt+.5

          if( mod(info/8,2).eq.1)then
            call cgmgres( l,nd,ng,pu,pf,pdf,res,resmx,id,rd,ierr )
            write(str,'('' Level'',i1,'' Defect after Correct '')') l
            call cgmgpr( l,ng,pdf,str,'(1x,80f6.3)',10,id,rd )
          end if

c         ...smooth
          conv=0.
          call cgmgsm( 1,l,ng,pu,pf,minsm,maxsm,eta,conv,resmx,wks,
     &     mgdir,id,rd,ierr )
          wkt=wkt+wks

        else
c         ===Direct solve at the lowest level===
          call cgmgds( l,ng,u,pu,ppu,f,pf,ppf,resmx,wkt,
     &     mgdir,id,rd,ierr )
        end if

        if(mod(info/8,2).eq.1)then
c         ...get residual
          call cgmgres( l,nd,ng,pu,pf,pdf,res,resmx,id,rd,ierr )
          if( l.ne.mxl )then
            write(str,'('' Level'',i1,'' Defect after Smooth2 '')') l
          else
            write(str,'('' Level'',i1,'' Defect after Solve '')') l
          end if
          call cgmgpr( l,ng,pdf,str,'(1x,80f6.3)',10,id,rd )
        end if


        if( (l.eq.1 .and. mod(info,2).eq.1) .or.
     &      (l.gt.1 .and. mod(info/2,2).eq.1) )then
          write(str,9000) l,1
          write(*,str) l,it,resmx,(res(k,l),k=1,1),wkt,
     &     resmx/resmxo,(resmx/resmxo)**(1./(wkt-wkto))
        end if
        it0=it
        if( l.eq.1.and.(resmx.lt.epsc .or. l.eq.mxl) ) goto 600
        if( l.gt.1.and.(resmx/resmx0.lt.delta.or.l.eq.mxl) )goto 600
        if( mod(info/2,2).eq.1 .and. l.gt.1 )then
          write(*,'('' l='',i1,'' resmx/resmx0 ='',e10.2,'//
     &     ''' <? delta ='',f6.3)') l,resmx/resmx0,delta
        end if
      end do
      if( l.eq.1 )then
        ierr=-1
        write(errmes,9100) itmax,resmx
 9100   format('Warning: No convergence after ',i3,' iterations,',
     &         ' resmx =',e10.4)
      end if

 600  continue
c     ---keep track of total number of iterations for statistics
      ittotal=ittotal+it0

      call second(t1)
      if( l.eq.1 )then
        tm(ittl)=tm(ittl)+t1-t0
      end if
      tm(itcgmg+l-1)=tm(itcgmg+l-1)+t1-t0

 9000 format('(',i1,'x,''level'',i1,'' it='',i3,'' resmx='',e8.2,',
     & '''res(k)='',',i2,'(e7.2,1x),''WU='',f5.1,'' CR='',f5.2,',
     & ''' ECR='',f5.3)')
      if(l.eq.1 .and. mod(info/8,2).eq.1 )then
        write(str,'('' Level'',i1,'' u after finished '')') l
        call cgmgpr( l,ng,pu,str,'(1x,40f6.3)',10,id,rd )
c       ---compute and print final residual
        call cgmgres( l,nd,ng,pu,pf,pdf,res,resmx,id,rd,ierr )
        write(str,'('' Level'',i1,'' ***Defect after Done '')') l
        call cgmgpr( l,ng,pdf,str,'(1x,80f6.3)',10,id,rd )
      end if

      end
}
