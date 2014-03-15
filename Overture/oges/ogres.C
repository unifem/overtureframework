#include "Oges.h"


void Oges::
ogres(realCompositeGridFunction & u, 
      realCompositeGridFunction & f, 
      realCompositeGridFunction & v, real & resmx )
{
  //
  //      subroutine cgres( id,rd,cgdir,pu,pf,pv,ip,rp,resmx,ierr )
  //c================================================================
  //c   Compute the residual
  //c
  //c           v <- f - A u
  //c
  //c  Input
  //c   pu,pf : pointers to current solution and right hand side
  //c   ip() : array of integer parameters (same as call to cgesr)
  //c       ip(1)=inl
  //c       ip(2)=idopt
  //c       ip(3)=ioptr
  //c       ip(4)=wdir
  //c       ip(5)=nv
  //c   rp() : array of real parameters : currently not not used
  //c Output
  //c   pv    : pointer to the residual
  //c   resmx : maximum residual
  //c
  //c================================================================
  //      implicit integer (a-z)
  //      integer id(*),pu(*),pf(*),pv(*),cgdir,ip(*)
  //      real rd(*),rp(*),resmx
  //c.......local
  //      integer cgdir0
  //      data cgdir0/0/,nd,ng,pndrs3/3*0/
  //      save
  //c........statement functions
  //      ndrs3(kd,ks,k)=id(pndrs3+kd-1+3*(ks-1+2*(k-1)))
  //c.........end statement functions
  //
  //      inl  =ip(1)
  //      idopt=ip(2)
  //      ioptr=ip(3)
  //      wdir =ip(4)
  //      nv   =ip(5)
  //      if( cgdir0.ne.cgdir )then
  //        cgdir0=cgdir
  //        nd     =id(dskfnd(id,cgdir,'nd'))
  //        ng     =id(dskfnd(id,cgdir,'ng'))
  //        pndrs3 =dskloc(id,cgdir,'ndrs3')
  //        if( pndrs3.eq.0 )then
  //c         ...make the ndrs3 array
  //          call cgndrs3( id,cgdir,ierr )
  //          pndrs3=dskfnd(id,cgdir,'ndrs3')
  //        end if
  //      end if
  //c...........Assign the right hand side
  //      resmx=0.
  //      do k=1,ng
  //        call cgres2( id,rd,nd,ng,nv,k,ndrs3(1,1,k),ndrs3(1,2,k),
  //     &   ndrs3(2,1,k),ndrs3(2,2,k),ndrs3(3,1,k),ndrs3(3,2,k),rd(pu(k)),
  //     &   rd(pf(k)),rd(pv(k)),cgdir,idopt,pu,id(pndrs3),wdir,resmx )
  //      end do
  //
  //      end
  //
  //      subroutine cgres2( id,rd,nd,ng,nv,k,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
  //     &  u,f,v,cgdir,idopt,pu,ndrs3,wdir,resmx )
  //c================================================================
  //c   Compute the residual on a component grid
  //c Input -
  //c   u : solution
  //c   f : right hand side for CGES
  //c Output -
  //c   v : residual
  //c================================================================
  //      integer id(*),cgdir,pu(ng),ndrs3(3,2,ng),wdir
  //      real rd(*),
  //     &     u(ndra:ndrb,ndsa:ndsb,ndta:ndtb,*),
  //     &     f(ndra:ndrb,ndsa:ndsb,ndta:ndtb,*),
  //     &     v(ndra:ndrb,ndsa:ndsb,ndta:ndtb,*),resmx
  //c.........local
  //      parameter( nvd=10,nde=100,nip=30 )
  //      real ce(nde,nvd)
  //      integer iv(4),ip(nip),ie(5,nde,nvd),ne(nvd),ppuc,puc
  //c.......start statement functions
  //      ndr(kd,k)=ndrs3(kd,2,k)-ndrs3(kd,1,k)+1
  //      puc(k)=id(ppuc+k-1)
  //      uc(i1,i2,i3,n,k)=rd(puc(k)+i1-ndrs3(1,1,k)+ndr(1,k)*(
  //     &                           i2-ndrs3(2,1,k)+ndr(2,k)*(
  //     &                           i3-ndrs3(3,1,k)+ndr(3,k)*(n-1))) )
  //      uk(i1,i2,i3,n,k)=rd( pu(k)+i1-ndrs3(1,1,k)+ndr(1,k)*(
  //     &                           i2-ndrs3(2,1,k)+ndr(2,k)*(
  //     &                           i3-ndrs3(3,1,k)+ndr(3,k)*(n-1))) )
  //c........end statement functions
  //
  //      if( nv.gt.nvd )then
  //        stop 'CGRES2: dimension error, nv > nvd'
  //      end if

  IntegerArray ip(40); ip.setBase(1);
  ip=0;
  
  IntegerArray iv(4); iv.setBase(1);

  const int nde=100;  // *****
  RealArray ce(nde,numberOfComponents);  ce.setBase(1);
  IntegerArray ie(5,nde,numberOfComponents); ie.setBase(1);
  IntegerArray ne(numberOfComponents);       ne.setBase(1);


  int i1,i2,i3,n,i;
  
  RealArray res(numberOfComponents); res.setBase(1);

  resmx=0.;
  for( int grid=0; grid< numberOfGrids; grid++ )
  {
    iv(4)=grid+1;
    
    for( i3=arrayDims(grid,Start,axis3); i3<=arrayDims(grid,End,axis3); i3++ )
    {
      iv(3)=i3;
      for( i2=arrayDims(grid,Start,axis2); i2<=arrayDims(grid,End,axis2); i2++ )
      {
        iv(2)=i2;
        for( i1=arrayDims(grid,Start,axis1); i1<=arrayDims(grid,End,axis1); i1++ )
        {
          iv(1)=i1;
          // get equations in discrete form
          // (ignore constant term if any)

// ****          ogde( ip,iv, ce,ie,ne,ierr );

          for( n=1; n<=numberOfComponents; n++ )
	  {
	    res(n)=-f[grid](i1,i2,i3,n-1);
	    for( i=1; i<=ne(n); i++ )
	    {
	      res(n)=res(n)+ce(i,n)*
                u[ie(5,i,n)-1](ie(2,i,n),ie(3,i,n),ie(4,i,n),ie(1,i,n)-1);
	    }
            // ..add in constant term
            res(n)=res(n)+ce(ne(n)+1,n);
	    
	  }
	  
          // Add in "dense" extra equations such as those equations that define
          // an "integral" type constraint (e.g. setting the mean pressure to zero)
          // *** is this what we want: ? ****
          if( numberOfExtraEquations > 0 
              && coefficientsOfDenseExtraEquations != NULL )
          {
     
            cout << "ogres: adding denseExtraEquations..." << endl;
            int gridc;	      
            for( gridc=0; gridc < (*coefficientsOfDenseExtraEquations).numberOfComponentGrids(); gridc++ )
            {
              RealDistributedArray & c = (*coefficientsOfDenseExtraEquations)[gridc];
              // **** should the nc loop go outside or inside
              for( int nc=c.getBase(axis3+1); nc<=c.getBound(axis3+1); nc++ )
	      {
    	        for( int i3c=c.getBase(axis3); i3c<=c.getBound(axis3); i3c++ )
	        {
		  for( int i2c=c.getBase(axis2); i2c<=c.getBound(axis2); i2c++ )
	  	  {
		    for( int i1c=c.getBase(axis1); i1c<=c.getBound(axis1); i1c++ )
		    {
                      res(nc+1)=res(nc+1)+c(i1c,i2c,i3c,nc)*u[gridc](i1c,i2c,i3c,nc);
		    }
		  }
		}
	      }
	    }
	    coefficientsOfDenseExtraEquations = NULL;
	  } // end if numberOfExtra

       
          for( n=1; n<=numberOfComponents; n++ )
	  {
            v[grid](i1,i2,i3,n-1)=-res(n);
            resmx=max(resmx,fabs(res(n)));
	  }
	}
      }
    }
  }
}



/* -----
      subroutine cgcor( id,rd,cgdir,pu,pv,ip,rp,vmax,ierr )
c================================================================
c   Compute the correction
c          u <- u + v
c Input -
c   pu,pf : pointers to current solution and right hand side
c   ip() : array of integer parameters (same as call to cgesr)
c       ip(1)=inl
c       ip(2)=idopt
c       ip(3)=ioptr
c       ip(4)=wdir
c       ip(5)=nv
c   rp() : array of real parameters : currently not not used
c Output
c  vmax : maximum correction
c================================================================
      implicit integer (a-z)
      integer id(*),pu(*),pv(*),ip(*)
      real rd(*),vmax,rp(*)
c.......local
      integer cgdir0,pndrs3
      data cgdir0/0/,nd,ng,pndrs3/3*0/
      save
c........statement functions
      ndrs3(kd,ks,k)=id(pndrs3+kd-1+3*(ks-1+2*(k-1)))
c.........end statement functions

      inl  =ip(1)
      idopt=ip(2)
      ioptr=ip(3)
      wdir =ip(4)
      nv   =ip(5)
      if( cgdir0.ne.cgdir )then
        cgdir0=cgdir
        nd     =id(dskfnd(id,cgdir,'nd'))
        ng     =id(dskfnd(id,cgdir,'ng'))
        pndrs3 =dskloc(id,cgdir,'ndrs3')
        if( pndrs3.eq.0 )then
c         ...make the ndrs3 array
          call cgndrs3( id,cgdir,ierr )
          pndrs3=dskfnd(id,cgdir,'ndrs3')
        end if
      end if
c...........Assign the right hand side
      vmax=0.
      do 200 k=1,ng
        call cgcor2( k,nd,nv,ndrs3(1,1,k),ndrs3(1,2,k),ndrs3(2,1,k),
     &   ndrs3(2,2,k),ndrs3(3,1,k),ndrs3(3,2,k),rd(pu(k)),rd(pv(k)),
     &   vmax,id,rd )
 200  continue
      return
      end

      subroutine cgcor2( k,nd,nv,ndra,ndrb,ndsa,ndsb,ndta,ndtb,
     &  u,v,vmax,id,rd )
c================================================================
c   Compute the correction on a component grid
c Input -
c  u : old solution
c  v : correction
c Output -
c  u : new solution
c  vmax : maximum correction
c================================================================
      integer id(*)
      real rd(*),
     &     u(ndra:ndrb,ndsa:ndsb,ndta:ndtb,*),
     &     v(ndra:ndrb,ndsa:ndsb,ndta:ndtb,*),vmax

      do n=1,nv
        do i3=ndta,ndtb
          do i2=ndsa,ndsb
            do i1=ndra,ndrb
              u(i1,i2,i3,n)=u(i1,i2,i3,n)+v(i1,i2,i3,n)
              vmax=max(vmax,abs(v(i1,i2,i3,n)))
            end do
          end do
        end do
      end do

      return
      end

      subroutine cgesprt( id,rd,cgdir,nv,uvn )
c======================================================================
c  Print the solution
c
c Input
c   id,rd,cgdir,nv,uvn
c Output
c
c======================================================================
      implicit integer (a-z)
      integer id(*)
      real rd(*)
      character*(*) uvn(*)
c.......local
      integer cgdir0
      data cgdir0/0/
      save

      if( cgdir0.ne.cgdir )then
        cgdir0=cgdir
        nd     =id(dskfnd(id,cgdir,'nd'))
        ng     =id(dskfnd(id,cgdir,'ng'))
        pndrs3 =dskloc(id,cgdir,'ndrs3')
        if( pndrs3.eq.0 )then
c         ...make the ndrs3 array
          call cgndrs3( id,cgdir,ierr )
          pndrs3=dskfnd(id,cgdir,'ndrs3')
        end if
      end if
      qu=dskfnd(id,cgdir,uvn)

      call cgesprt2( id,rd,nd,ng,nv,id(pndrs3),id(qu) )

      end


      subroutine cgesprt2( id,rd,nd,ng,nv,ndrs,pu )
c======================================================================
c  Output the solution
c
c Input
c   u
c Output
c
c======================================================================
      real rd(*)
      integer ndrs(3,2,*),pu(*),id(*)
c.......local
c...........start statement functions
      ndr(kd,k)=ndrs(kd,2,k)-ndrs(kd,1,k)+1
      iu(i1,i2,i3,kd,k)=
     &     pu(k)+i1-ndrs(1,1,k)+ndr(1,k)*(
     &           i2-ndrs(2,1,k)+ndr(2,k)*(
     &           i3-ndrs(3,1,k)+ndr(3,k)*(kd-1)))
      u(i1,i2,i3,kd,k)=rd(iu(i1,i2,i3,kd,k))
c...........end statement functions

      iout=1
      write(iout,*) '********* Solution ndra,ndrb ... ********'
      do kd=1,nv
        write(iout,'(''****u'',i1,''(i1,i2,i3)**** '')') kd
        do k=1,ng
          if( ng.ne.1 )then
            write(iout,*) '------------k=',k
          end if
          do i3=ndrs(3,1,k),ndrs(3,2,k)
            if(nd.eq.3) write(iout,*) '+++i3=',i3,'+++'
            do i2=ndrs(2,1,k),ndrs(2,2,k)
              write(iout,9000) (u(i1,i2,i3,kd,k),
     &                    i1=ndrs(1,1,k),ndrs(1,2,k))
            end do
          end do
        end do
      end do
 9000 format((1x,40f6.2))

      end

------- */
