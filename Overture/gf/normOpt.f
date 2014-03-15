! This file automatically generated from normOpt.bf with bpp.

! ===================================================================================
! ===================================================================================

! ===================================================================================
! ===================================================================================


! ===================================================================================
! ===================================================================================

! ===================================================================================
! ===================================================================================

! ===================================================================================
! Macro to define the subroutine that computesd the area weighted Lp norm
! Args:
!  NAME : subroutine name
! ===================================================================================



       subroutine getAreaWeightedLpNorm( nd, nd1a,nd1b,nd2a,nd2b,nd3a,
     & nd3b,n1a,n1b,n2a,n2b,n3a,n3b, u, mask, rsxy,  ipar, rpar )
c ===================================================================================
c  Compute quantities needed for a discrete area-weighted Lp norm
c        lpNorm = ( up / vol )^(1/p)
c  up : sum |u_i|^p *dv_i    at all points with mask.ne.0 , dv_i is the local volume element
c  vol : sum 1 *dv_i
c  count : number of valid points
c  maskOption(input) : maskOption=0 : check points where mask(i1,i2,i3).ne.0
c                      maskOption>0 : check points where mask(i1,i2,i3)>0
c  p : the (integer) power in the norm
c
c
c  ipar(0) = maskOption
c  ipar(1) = p         
c  ipar(2) = gridType : 0=rectangular, 1=curvilinear
c
c  rpar(0:2) = dx(0:2) : grid-spacing for a rectangular grid
c  rpar(3:5) = dr(0:2) : parameter space grid spacing
c
c OUTPUT:
c  ipar(10) = count
c  rpar(10) = up 
c  rpar(11) = vol 
c
c NOTES:
c   We do NOT include points that are hidden by refinement. 
c ===================================================================================
        implicit none
        integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,n1a,n1b,n2a,n2b,n3a,
     & n3b,maskOption,p
        integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
        real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
        integer ipar(0:*)
        real rpar(0:*)
        integer rectangular,curvilinear
        parameter( rectangular=0,curvilinear=1 )
        integer hiddenByRefinementBit !  bit    26
        parameter( hiddenByRefinementBit=26 )
        real up,dv, dx(0:2),dr(0:2), vol, xvol,rvol, volTotal, xEps
        integer i1,i2,i3, n, count, gridType
        real rx,ry,rz,sx,sy,sz,tx,ty,tz
c     --- begin statement functions
        rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
        ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
        rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
        sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
        sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
        sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
        tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
        ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
        tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)
c     --- end statement functions
        maskOption = ipar(0)
        p          = ipar(1)
        gridType   = ipar(2)
        dx(0)      = rpar(0)
        dx(1)      = rpar(1)
        dx(2)      = rpar(2)
        dr(0)      = rpar(3)
        dr(1)      = rpar(4)
        dr(2)      = rpar(5)
        xEps       = rpar(6)
        xvol = 1.  ! volume element for a rectangular grid
        rvol = 1.  ! rvol = dr(0)*dr(1)*dr(2)
        do n=0,nd-1
         xvol=xvol*dx(n)
         rvol=rvol*dr(n)
        end do
        count=0
        up=0.
        volTotal=0.
         if( nd.eq.2 )then
           if( gridType.eq.rectangular )then
             if( maskOption.eq.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 ! do NOT include points that are hidden by refinement 
                  if( mask(i1,i2,i3).ne.0  .and. .not.btest(mask(i1,i2,
     & i3),hiddenByRefinementBit) )then
                      up=up+abs(u(i1,i2,i3))**p
                    count=count+1
                  end if
                end do
                end do
                end do
             else if( maskOption.eq.1 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 ! do NOT include points that are hidden by refinement 
                  if( mask(i1,i2,i3).gt.0  .and. .not.btest(mask(i1,i2,
     & i3),hiddenByRefinementBit) )then
                      up=up+abs(u(i1,i2,i3))**p
                    count=count+1
                  end if
                end do
                end do
                end do
             else
               stop 90302
             end if
           else if( gridType.eq.curvilinear )then
             if( maskOption.eq.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 ! do NOT include points that are hidden by refinement 
                  if( mask(i1,i2,i3).ne.0  .and. .not.btest(mask(i1,i2,
     & i3),hiddenByRefinementBit) )then
                        vol = (rvol/max(1.e-30,abs(rx(i1,i2,i3)*sy(i1,
     & i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3))))
                      volTotal=volTotal+vol
                      up=up+vol*abs(u(i1,i2,i3))**p
                    count=count+1
                  end if
                end do
                end do
                end do
             else if( maskOption.eq.1 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 ! do NOT include points that are hidden by refinement 
                  if( mask(i1,i2,i3).gt.0  .and. .not.btest(mask(i1,i2,
     & i3),hiddenByRefinementBit) )then
                        vol = (rvol/max(1.e-30,abs(rx(i1,i2,i3)*sy(i1,
     & i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3))))
                      volTotal=volTotal+vol
                      up=up+vol*abs(u(i1,i2,i3))**p
                    count=count+1
                  end if
                end do
                end do
                end do
             else
               stop 90302
             end if
           else
            stop 90202
           end if
         else if( nd.eq.3 )then
           if( gridType.eq.rectangular )then
             if( maskOption.eq.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 ! do NOT include points that are hidden by refinement 
                  if( mask(i1,i2,i3).ne.0  .and. .not.btest(mask(i1,i2,
     & i3),hiddenByRefinementBit) )then
                      up=up+abs(u(i1,i2,i3))**p
                    count=count+1
                  end if
                end do
                end do
                end do
             else if( maskOption.eq.1 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 ! do NOT include points that are hidden by refinement 
                  if( mask(i1,i2,i3).gt.0  .and. .not.btest(mask(i1,i2,
     & i3),hiddenByRefinementBit) )then
                      up=up+abs(u(i1,i2,i3))**p
                    count=count+1
                  end if
                end do
                end do
                end do
             else
               stop 90302
             end if
           else if( gridType.eq.curvilinear )then
             if( maskOption.eq.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 ! do NOT include points that are hidden by refinement 
                  if( mask(i1,i2,i3).ne.0  .and. .not.btest(mask(i1,i2,
     & i3),hiddenByRefinementBit) )then
                        vol = (rvol/max(1.e-30,abs((rx(i1,i2,i3)*sy(i1,
     & i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3))*tz(i1,i2,i3)+(ry(i1,i2,i3)*
     & sz(i1,i2,i3)-rz(i1,i2,i3)*sy(i1,i2,i3))*tx(i1,i2,i3)+(rz(i1,i2,
     & i3)*sx(i1,i2,i3)-rx(i1,i2,i3)*sz(i1,i2,i3))*ty(i1,i2,i3))))
                      volTotal=volTotal+vol
                      up=up+vol*abs(u(i1,i2,i3))**p
                    count=count+1
                  end if
                end do
                end do
                end do
             else if( maskOption.eq.1 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                 ! do NOT include points that are hidden by refinement 
                  if( mask(i1,i2,i3).gt.0  .and. .not.btest(mask(i1,i2,
     & i3),hiddenByRefinementBit) )then
                        vol = (rvol/max(1.e-30,abs((rx(i1,i2,i3)*sy(i1,
     & i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3))*tz(i1,i2,i3)+(ry(i1,i2,i3)*
     & sz(i1,i2,i3)-rz(i1,i2,i3)*sy(i1,i2,i3))*tx(i1,i2,i3)+(rz(i1,i2,
     & i3)*sx(i1,i2,i3)-rx(i1,i2,i3)*sz(i1,i2,i3))*ty(i1,i2,i3))))
                      volTotal=volTotal+vol
                      up=up+vol*abs(u(i1,i2,i3))**p
                    count=count+1
                  end if
                end do
                end do
                end do
             else
               stop 90302
             end if
           else
            stop 90202
           end if
         else
           stop 90102
         end if
        if( gridType.eq.rectangular )then
          up=up*xvol
          volTotal=xvol*count
        end if
        ipar(10)=count
        rpar(10)=up
        rpar(11)=volTotal
        return
        end






      subroutine getLpNormOpt( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, u, mask, up, count, maskOption, p )
c ===================================================================================
c  Compute quantities needed for a discrete Lp norm
c        lpNorm = ( up/count )^(1/p)
c  up : sum |u|^p at all points with mask.ne.0
c  count : number of valid points
c  maskOption(input) : maskOption=0 : check points where mask(i1,i2,i3).ne.0
c                      maskOption>0 : check points where mask(i1,i2,i3)>0
c  p : the (integer) power in the norm
c ===================================================================================

      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b,maskOption,p

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real up
      integer i1,i2,i3, count

      count=0
      up=0.
      if( maskOption.eq.0 )then
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).ne.0  )then
                up=up+abs(u(i1,i2,i3))**p
                count=count+1
              end if
            end do
          end do
        end do
      else
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).gt.0  )then
                up=up+abs(u(i1,i2,i3))**p
                count=count+1
              end if
            end do
          end do
        end do

      end if
      return
      end




      subroutine getL2normOpt( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, u, mask, uSquared, count, maskOption )
c ===================================================================================
c  Compute quantities needed for a discrete L2 norm
c        l2norm = sqrt( uSquared/count )
c  uSquared : sum u*u at all points with mask.ne.0
c  count : number of valid points
c  maskOption(input) : maskOption=0 : check points where mask(i1,i2,i3).ne.0
c                      maskOption>0 : check points where mask(i1,i2,i3)>0
c ===================================================================================

      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b,maskOption

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real uSquared
      integer i1,i2,i3, count

      count=0
      uSquared=0.
      if( maskOption.eq.0 )then
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).ne.0  )then
                uSquared=uSquared+u(i1,i2,i3)**2
                count=count+1
              end if
            end do
          end do
        end do
      else
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).gt.0  )then
                uSquared=uSquared+u(i1,i2,i3)**2
                count=count+1
              end if
            end do
          end do
        end do

      end if
      return
      end


      subroutine getL2ErrorOpt( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, u, v, mask, uSquared, count,
     & maskOption )
c ===================================================================================
c  Compute quantities needed for a discrete L2 norm
c        l2norm = sqrt( uSquared/count )
c  uSquared : sum (u-v)*(u-v) at all valid points (mask.ne.0)
c  count : number of valid points
c  maskOption(input) : maskOption=0 : check points where mask(i1,i2,i3).ne.0
c                      maskOption>0 : check points where mask(i1,i2,i3)>0
c ===================================================================================

      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b, maskOption

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real uSquared
      integer i1,i2,i3, count

      count=0
      uSquared=0.
      if( maskOption.eq.0 )then
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).ne.0  )then
                uSquared=uSquared+(u(i1,i2,i3)-v(i1,i2,i3))**2
                count=count+1
              end if
            end do
          end do
        end do
      else
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).gt.0  )then
                uSquared=uSquared+(u(i1,i2,i3)-v(i1,i2,i3))**2
                count=count+1
              end if
            end do
          end do
        end do
      end if
      return
      end

      subroutine getMaxNormOpt( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, u, mask, uMax, maskOption )
c ===================================================================================
c  Compute the max norm, returned as uMax
c              uMax= max(abs(u)) where mask.ne.0
c  maskOption(input) : maskOption=0 : check points where mask(i1,i2,i3).ne.0
c                      maskOption>0 : check points where mask(i1,i2,i3)>0
c ===================================================================================

      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b, maskOption

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real uMax

      integer i1,i2,i3

      uMax=0.
      if( maskOption.eq.0 )then
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).ne.0  )then
                uMax=max(uMax,abs(u(i1,i2,i3)))
              end if
            end do
          end do
        end do
      else
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).gt.0  )then
                uMax=max(uMax,abs(u(i1,i2,i3)))
              end if
            end do
          end do
        end do
      end if

      return
      end

      subroutine getL2AndMaxNormOpt( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, u, mask, uSquared, count, uMax,
     &  maskOption )
c ===================================================================================
c  Compute quantities needed for a discrete L2 norm
c        l2norm = sqrt( uSquared/count )
c  uSquared : sum u*u at all valid points
c  count : number of valid points
c  uMax= max(abs(u)) where mask.ne.0
c  maskOption(input) : maskOption=0 : check points where mask(i1,i2,i3).ne.0
c                      maskOption>0 : check points where mask(i1,i2,i3)>0
c ===================================================================================

      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b, maskOption

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real uMax

      real uSquared
      integer i1,i2,i3, count

      count=0
      uSquared=0.
      uMax=0.
      if( maskOption.eq.0 )then
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).ne.0  )then
                uSquared=uSquared+u(i1,i2,i3)**2
                count=count+1
                uMax=max(uMax,abs(u(i1,i2,i3)))
              end if
            end do
          end do
        end do
      else
        do i3=n3a,n3b
          do i2=n2a,n2b
            do i1=n1a,n1b
              if( mask(i1,i2,i3).gt.0  )then
                uSquared=uSquared+u(i1,i2,i3)**2
                count=count+1
                uMax=max(uMax,abs(u(i1,i2,i3)))
              end if
            end do
          end do
        end do
      end if

      return
      end





c ***************** older versions without the mask option ************************


      subroutine l2normOpt( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, u, mask, uSquared, count )
c ===================================================================================
c  Compute quantities needed for a discrete L2 norm
c        l2norm = sqrt( uSquared/count )
c  uSquared : sum u*u at all points with mask.ne.0
c  count : number of valid points
c ===================================================================================

      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real uSquared
      integer i1,i2,i3, count

      count=0
      uSquared=0.
      do i3=n3a,n3b
        do i2=n2a,n2b
          do i1=n1a,n1b
            if( mask(i1,i2,i3).ne.0  )then
              uSquared=uSquared+u(i1,i2,i3)**2
              count=count+1
            end if
          end do
        end do
      end do

      return
      end


      subroutine l2ErrorOpt( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, u, v, mask, uSquared, count )
c ===================================================================================
c  Compute quantities needed for a discrete L2 norm
c        l2norm = sqrt( uSquared/count )
c  uSquared : sum (u-v)*(u-v) at all valid points (mask.ne.0)
c  count : number of valid points
c ===================================================================================

      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      real uSquared
      integer i1,i2,i3, count

      count=0
      uSquared=0.
      do i3=n3a,n3b
        do i2=n2a,n2b
          do i1=n1a,n1b
            if( mask(i1,i2,i3).ne.0  )then
              uSquared=uSquared+(u(i1,i2,i3)-v(i1,i2,i3))**2
              count=count+1
            end if
          end do
        end do
      end do

      return
      end

      subroutine maxNormOpt( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, u, mask, uMax )
c ===================================================================================
c  Compute the max norm, returned as uMax
c              uMax= max(abs(u)) where mask.ne.0
c ===================================================================================

      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real uMax

      integer i1,i2,i3

      uMax=0.
      do i3=n3a,n3b
        do i2=n2a,n2b
          do i1=n1a,n1b
            if( mask(i1,i2,i3).ne.0  )then
              uMax=max(uMax,abs(u(i1,i2,i3)))
            end if
          end do
        end do
      end do

      return
      end

      subroutine l2AndMaxNormOpt( nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, u, mask, uSquared, count, uMax )
c ===================================================================================
c  Compute quantities needed for a discrete L2 norm
c        l2norm = sqrt( uSquared/count )
c  uSquared : sum u*u at all valid points
c  count : number of valid points
c  uMax= max(abs(u)) where mask.ne.0
c ===================================================================================

      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        n1a,n1b,n2a,n2b,n3a,n3b

      integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real uMax

      real uSquared
      integer i1,i2,i3, count

      count=0
      uSquared=0.
      uMax=0.
      do i3=n3a,n3b
        do i2=n2a,n2b
          do i1=n1a,n1b
            if( mask(i1,i2,i3).ne.0  )then
              uSquared=uSquared+u(i1,i2,i3)**2
              count=count+1
              uMax=max(uMax,abs(u(i1,i2,i3)))
            end if
          end do
        end do
      end do

      return
      end
