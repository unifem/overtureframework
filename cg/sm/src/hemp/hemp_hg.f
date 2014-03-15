      subroutine hemp_hg(
     *   nxl,nxu,nyl,nyu,
     *   vx,vy,vx_work,vy_work,vx_temp,vy_temp,phi,
     *   dt,hgFlag,hgVisc )
c
      implicit none
      integer nxl,nxu,nyl,nyu
      real vx(nxl:nxu,nyl:nyu),vy(nxl:nxu,nyl:nyu)
      real vx_work(nxl:nxu,nyl:nyu),vy_work(nxl:nxu,nyl:nyu)
      real vx_temp(nxl:nxu,nyl:nyu),vy_temp(nxl:nxu,nyl:nyu)
      real phi(nxl:nxu,nyl:nyu)
      real dt,alpha,hgVisc
      real L7(8),L8(8),vec1(8),vec2(8)
c
      integer i,j,k,hgFlag
      real dvx,dvy,dmx,dmy,comp1,comp2
c
c      data alpha / 8.e-2 /
cc      data alpha / 4.e-2 /
cc      data alpha / 1.e-2 /
c      data alpha / 2.e-2 /
c      data alpha / 0.e-2 /
      data L7 / -0.5e0,0.5e0,-0.5e0,0.5e0,0.e0,0.e0,0.e0,0.e0 /
      data L8 / 0.e0,0.e0,0.e0,0.e0,-0.5e0,0.5e0,-0.5e0,0.5e0 /
c
      alpha = hgVisc
c
c.. Start in on the hourglass control
      do j = nyl,nyu
        do i = nxl,nxu
          vx_temp(i,j) = vx_work(i,j)
          vy_temp(i,j) = vy_work(i,j)
        end do
      end do
c
      if( hgFlag.eq.1 ) then ! Margolin's filter with small amount of regular 
                             !  diffusion operator (with momentum redistribution)
        do j = nyl+1,nyu-1
          do i = nxl+1,nxu-1
            dvx = -dt*0.1e0*
     *         (4.0*vx_temp(i,j)-
     *         (vx_temp(i+1,j)+vx_temp(i,j+1)+
     *         vx_temp(i-1,j)+vx_temp(i,j-1)))
            dvy = -dt*0.1e0*
     *         (4.0*vy_temp(i,j)-
     *         (vy_temp(i+1,j)+vy_temp(i,j+1)+
     *         vy_temp(i-1,j)+vy_temp(i,j-1)))
c
            dvx = dvx-0.5*alpha*
     *         (2.0*vx_temp(i,j)+
     *         0.5*(vx_temp(i+1,j+1)+vx_temp(i-1,j+1)+
     *         vx_temp(i-1,j-1)+vx_temp(i+1,j-1))-
     *         (vx_temp(i+1,j)+vx_temp(i,j+1)+
     *         vx_temp(i-1,j)+vx_temp(i,j-1)))
            dvy = dvy-0.5*alpha*
     *         (2.0*vy_temp(i,j)+
     *         0.5*(vy_temp(i+1,j+1)+vy_temp(i-1,j+1)+
     *         vy_temp(i-1,j-1)+vy_temp(i+1,j-1))-
     *         (vy_temp(i+1,j)+vy_temp(i,j+1)+
     *         vy_temp(i-1,j)+vy_temp(i,j-1)))
c
            vx(i,j) = vx(i,j)+dvx
            vy(i,j) = vy(i,j)+dvy
            dmx = dvx*phi(i,j)
            dmy = dvy*phi(i,j)
            vx(i-1,j) = vx(i-1,j)-0.25*dmx/phi(i-1,j)
            vx(i+1,j) = vx(i+1,j)-0.25*dmx/phi(i+1,j)
            vx(i,j-1) = vx(i,j-1)-0.25*dmx/phi(i,j-1)
            vx(i,j+1) = vx(i,j+1)-0.25*dmx/phi(i,j+1)
            vy(i-1,j) = vy(i-1,j)-0.25*dmy/phi(i-1,j)
            vy(i+1,j) = vy(i+1,j)-0.25*dmy/phi(i+1,j)
            vy(i,j-1) = vy(i,j-1)-0.25*dmy/phi(i,j-1)
            vy(i,j+1) = vy(i,j+1)-0.25*dmy/phi(i,j+1)
          end do
        end do
      elseif( hgFlag.eq.2.or.hgFlag.eq.4 ) then ! Regular diffusion operator (possibly with momentum redistribution)
        do j = nyl+1,nyu-1
          do i = nxl+1,nxu-1
            dvx = -0.5*alpha*
     *         (4.0*vx_temp(i,j)-
     *         (vx_temp(i+1,j)+vx_temp(i,j+1)+
     *         vx_temp(i-1,j)+vx_temp(i,j-1)))
            dvy = -0.5*alpha*
     *         (4.0*vy_temp(i,j)-
     *         (vy_temp(i+1,j)+vy_temp(i,j+1)+
     *         vy_temp(i-1,j)+vy_temp(i,j-1)))
            vx(i,j) = vx(i,j)+dvx
            vy(i,j) = vy(i,j)+dvy
c
            if( hgFlag.eq.4 ) then
              dmx = dvx*phi(i,j)
              dmy = dvy*phi(i,j)
              vx(i-1,j) = vx(i-1,j)-0.25*dmx/phi(i-1,j)
              vx(i+1,j) = vx(i+1,j)-0.25*dmx/phi(i+1,j)
              vx(i,j-1) = vx(i,j-1)-0.25*dmx/phi(i,j-1)
              vx(i,j+1) = vx(i,j+1)-0.25*dmx/phi(i,j+1)
              vy(i-1,j) = vy(i-1,j)-0.25*dmy/phi(i-1,j)
              vy(i+1,j) = vy(i+1,j)-0.25*dmy/phi(i+1,j)
              vy(i,j-1) = vy(i,j-1)-0.25*dmy/phi(i,j-1)
              vy(i,j+1) = vy(i,j+1)-0.25*dmy/phi(i,j+1)
            end if
          end do
        end do
        ! now add diffusion on the boundaries
        ! left
        i = nxl
        do j = nyl+1,nyu-1
          dvx = -0.5*alpha*
     *       (2.0*vx_temp(i,j)-
     *       (vx_temp(i,j+1)+vx_temp(i,j-1)))
          dvy = -0.5*alpha*
     *       (2.0*vy_temp(i,j)-
     *       (vy_temp(i,j+1)+vy_temp(i,j-1)))
          vx(i,j) = vx(i,j)+dvx
          vy(i,j) = vy(i,j)+dvy
c
          if( hgFlag.eq.4 ) then
            dmx = dvx*phi(i,j)
            dmy = dvy*phi(i,j)
            vx(i,j-1) = vx(i,j-1)-0.5*dmx/phi(i,j-1)
            vx(i,j+1) = vx(i,j+1)-0.5*dmx/phi(i,j+1)
            vy(i,j-1) = vy(i,j-1)-0.5*dmy/phi(i,j-1)
            vy(i,j+1) = vy(i,j+1)-0.5*dmy/phi(i,j+1)
          end if
        end do
        ! right
        i = nxu
        do j = nyl+1,nyu-1
          dvx = -0.5*alpha*
     *       (2.0*vx_temp(i,j)-
     *       (vx_temp(i,j+1)+vx_temp(i,j-1)))
          dvy = -0.5*alpha*
     *       (2.0*vy_temp(i,j)-
     *       (vy_temp(i,j+1)+vy_temp(i,j-1)))
          vx(i,j) = vx(i,j)+dvx
          vy(i,j) = vy(i,j)+dvy
c
          if( hgFlag.eq.4 ) then
            dmx = dvx*phi(i,j)
            dmy = dvy*phi(i,j)
            vx(i,j-1) = vx(i,j-1)-0.5*dmx/phi(i,j-1)
            vx(i,j+1) = vx(i,j+1)-0.5*dmx/phi(i,j+1)
            vy(i,j-1) = vy(i,j-1)-0.5*dmy/phi(i,j-1)
            vy(i,j+1) = vy(i,j+1)-0.5*dmy/phi(i,j+1)
          end if
        end do
        ! bottom
        j = nyl
        do i = nxl+1,nxu-1
          dvx = -0.5*alpha*
     *       (2.0*vx_temp(i,j)-
     *       (vx_temp(i+1,j)+vx_temp(i-1,j)))
          dvy = -0.5*alpha*
     *       (2.0*vy_temp(i,j)-
     *       (vy_temp(i+1,j)+vy_temp(i-1,j)))
          vx(i,j) = vx(i,j)+dvx
          vy(i,j) = vy(i,j)+dvy
c
          if( hgFlag.eq.4 ) then
            dmx = dvx*phi(i,j)
            dmy = dvy*phi(i,j)
            vx(i-1,j) = vx(i-1,j)-0.5*dmx/phi(i-1,j)
            vx(i+1,j) = vx(i+1,j)-0.5*dmx/phi(i+1,j)
            vy(i-1,j) = vy(i-1,j)-0.5*dmy/phi(i-1,j)
            vy(i+1,j) = vy(i+1,j)-0.5*dmy/phi(i+1,j)
          end if
        end do
        ! top
        j = nyu
        do i = nxl+1,nxu-1
          dvx = -0.5*alpha*
     *       (2.0*vx_temp(i,j)-
     *       (vx_temp(i+1,j)+vx_temp(i-1,j)))
          dvy = -0.5*alpha*
     *       (2.0*vy_temp(i,j)-
     *       (vy_temp(i+1,j)+vy_temp(i-1,j)))
          vx(i,j) = vx(i,j)+dvx
          vy(i,j) = vy(i,j)+dvy
c
          if( hgFlag.eq.4 ) then
            dmx = dvx*phi(i,j)
            dmy = dvy*phi(i,j)
            vx(i-1,j) = vx(i-1,j)-0.5*dmx/phi(i-1,j)
            vx(i+1,j) = vx(i+1,j)-0.5*dmx/phi(i+1,j)
            vy(i-1,j) = vy(i-1,j)-0.5*dmy/phi(i-1,j)
            vy(i+1,j) = vy(i+1,j)-0.5*dmy/phi(i+1,j)
          end if
        end do
      elseif( hgFlag.eq.3 ) then ! Just Margolin's filter with classical implimentation
        do j = nyl+1,nyu
          do i = nxl+1,nxu
            ! load the vector
            vec1(1) = vx_temp(i,j-1)
            vec1(2) = vx_temp(i,j)
            vec1(3) = vx_temp(i-1,j)
            vec1(4) = vx_temp(i-1,j-1)
            vec1(5) = vy_temp(i,j-1)
            vec1(6) = vy_temp(i,j)
            vec1(7) = vy_temp(i-1,j)
            vec1(8) = vy_temp(i-1,j-1)

            ! apply the filter
            comp1 = 0.e0
            comp2 = 0.e0
            do k = 1,8
              comp1 = comp1+vec1(k)*L7(k)
              comp2 = comp2+vec1(k)*L8(k)
            end do
            do k = 1,8
              vec2(k) = alpha*(comp1*L7(k)+comp2*L8(k))
            end do
          
            ! unload the vector
            vx(i,j-1)   = vx(i,j-1)  -vec2(1)
            vx(i,j)     = vx(i,j)    -vec2(2)
            vx(i-1,j)   = vx(i-1,j)  -vec2(3)
            vx(i-1,j-1) = vx(i-1,j-1)-vec2(4)
            vy(i,j-1)   = vy(i,j-1)  -vec2(5)
            vy(i,j)     = vy(i,j)    -vec2(6)
            vy(i-1,j)   = vy(i-1,j)  -vec2(7)
            vy(i-1,j-1) = vy(i-1,j-1)-vec2(8)
          end do
        end do
      end if
c
      return
      end
