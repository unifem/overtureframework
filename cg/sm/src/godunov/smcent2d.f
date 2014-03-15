      subroutine smcent2d( m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,
     *   dt,t,xy,u,up,nrparam,rparam,
     *   niparam,iparam,ier )
c
c compute du/dt for 2d linear elasticity (smcent => solid mechanics, centered)
c
c variables: u(j1,j2,1:2)=velocity
c            u(j1,j2,3:6)=stress (S11,S12,S21,S22)
c            u(j1,j2,7:8)=displacement
c
      implicit none
c
      integer m,nd1a,nd1b,n1a,n1b,nd2a,nd2b,n2a,n2b,nrparam
      integer niparam,ier
      integer iparam(niparam)
      real dt,t
      real xy(nd1a:nd1b,nd2a:nd2b,2)
      real u(nd1a:nd1b,nd2a:nd2b,m)
      real up(nd1a:nd1b,nd2a:nd2b,m)
      real rparam(nrparam)
c
      integer i1,i2,k
      real ux(m)
      real uy(m)
      real almax(2)
      real mu,lam,rho,dx,dy,c1,c2,sp,area
      real upp(m),upm(m),ump(m),umm(m),ul(m),ur(m)
      real xl,yl,xr,yr,a1,a2
c
c..set error flag
c      write(6,*)'**BEGIN smcent**'
      ier=0
c
c..parameters
      mu  = rparam(3)
      lam = rparam(4)
      rho = rparam(5)
c      write(6,*)mu,lam,rho
c
      almax(1) = 0.0
      almax(2) = 0.0
c
      do i1 = n1a,n1b
        do i2 = n2a,n2b
          area = 0.5*(
     *       (xy(i1+1,i2,1)-xy(i1-1,i2,1))*
     *         (xy(i1,i2+1,2)-xy(i1,i2-1,2))-
     *       (xy(i1,i2+1,1)-xy(i1,i2-1,1))*
     *         (xy(i1+1,i2,2)-xy(i1-1,i2,2)))
c
          do k = 1,6
            ul(k) = (u(i1,i2,k)    +u(i1+1,i2,k)+u(i1,i2+1,k))/3.0
            ur(k) = (u(i1+1,i2+1,k)+u(i1+1,i2,k)+u(i1,i2+1,k))/3.0
          end do
          a1 = xy(i1,i2+1,2)-xy(i1+1,i2,2)
          a2 = xy(i1+1,i2,1)-xy(i1,i2+1,1)
          call centRiem2d( ul,ur,a1,a2,area,upp,mu,lam,rho )
c
          do k = 1,6
            ul(k) = (u(i1-1,i2+1,k)+u(i1,i2+1,k)+u(i1-1,i2,k))/3.0
            ur(k) = (u(i1,i2,k)    +u(i1,i2+1,k)+u(i1-1,i2,k))/3.0
          end do
          a1 = -(xy(i1-1,i2,2)-xy(i1,i2+1,2))
          a2 = -(xy(i1,i2+1,1)-xy(i1-1,i2,1))
          call centRiem2d( ul,ur,a1,a2,area,ump,mu,lam,rho )
c
          do k = 1,6
            ul(k) = (u(i1-1,i2-1,k)+u(i1-1,i2,k)+u(i1,i2-1,k))/3.0
            ur(k) = (u(i1,i2,k)    +u(i1-1,i2,k)+u(i1,i2-1,k))/3.0
          end do
          a1 = -(xy(i1,i2-1,2)-xy(i1-1,i2,2))
          a2 = -(xy(i1-1,i2,1)-xy(i1,i2-1,1))
          call centRiem2d( ul,ur,a1,a2,area,umm,mu,lam,rho )
c
          do k = 1,6
            ul(k) = (u(i1,i2,k)    +u(i1,i2-1,k)+u(i1+1,i2,k))/3.0
            ur(k) = (u(i1+1,i2-1,k)+u(i1,i2-1,k)+u(i1+1,i2,k))/3.0
          end do
          a1 = xy(i1+1,i2,2)-xy(i1,i2-1,2)
          a2 = xy(i1,i2-1,1)-xy(i1+1,i2,1)
          call centRiem2d( ul,ur,a1,a2,area,upm,mu,lam,rho )
c
c          do k=1,6
cc            upp(k) = 0.25*(u(i1+1,i2,k)+u(i1+1,i2+1,k)+u(i1,i2+1,k))
cc            ump(k) = 0.25*(u(i1,i2+1,k)+u(i1-1,i2+1,k)+u(i1-1,i2,k))
cc            umm(k) = 0.25*(u(i1-1,i2,k)+u(i1-1,i2-1,k)+u(i1,i2-1,k))
cc            upm(k) = 0.25*(u(i1,i2-1,k)+u(i1+1,i2-1,k)+u(i1+1,i2,k))
cc
c            upp(k) = 1.0/6.0*(2.0*(u(i1+1,i2,k)+u(i1,i2+1,k))+
c     *                            (u(i1+1,i2+1,k)+u(i1,i2,k)))
c            ump(k) = 1.0/6.0*(2.0*(u(i1,i2+1,k)+u(i1-1,i2,k))+
c     *                            (u(i1-1,i2+1,k)+u(i1,i2,k)))
c            umm(k) = 1.0/6.0*(2.0*(u(i1-1,i2,k)+u(i1,i2-1,k))+
c     *                            (u(i1-1,i2-1,k)+u(i1,i2,k)))
c            upm(k) = 1.0/6.0*(2.0*(u(i1,i2-1,k)+u(i1+1,i2,k))+
c     *                            (u(i1+1,i2-1,k)+u(i1,i2,k)))
c          end do
          do k=1,6
c
            ux(k) = (upp(k)*(xy(i1,i2+1,2)-xy(i1+1,i2,2))+
     *               ump(k)*(xy(i1-1,i2,2)-xy(i1,i2+1,2))+
     *               umm(k)*(xy(i1,i2-1,2)-xy(i1-1,i2,2))+
     *               upm(k)*(xy(i1+1,i2,2)-xy(i1,i2-1,2)))/(area)
            uy(k) = (upp(k)*(xy(i1+1,i2,1)-xy(i1,i2+1,1))+
     *               ump(k)*(xy(i1,i2+1,1)-xy(i1-1,i2,1))+
     *               umm(k)*(xy(i1-1,i2,1)-xy(i1,i2-1,1))+
     *               upm(k)*(xy(i1,i2-1,1)-xy(i1+1,i2,1)))/(area)
          end do
c
          up(i1,i2,1) = 1.0/rho*(ux(3)+uy(5))
          up(i1,i2,2) = 1.0/rho*(ux(4)+uy(6))
          up(i1,i2,3) = (lam+2.0*mu)*ux(1)+lam*uy(2)
          up(i1,i2,4) = mu*(ux(2)+uy(1))
          up(i1,i2,5) = mu*(ux(2)+uy(1))
          up(i1,i2,6) = lam*ux(1)+(lam+2.0*mu)*uy(2)
          up(i1,i2,7) = u(i1,i2,1)
          up(i1,i2,8) = u(i1,i2,2)

          dx = 0.5*abs(area/(max(
     *       abs(xy(i1+1,i2+1,2)-xy(i1-1,i2-1,2)),
     *       abs(xy(i1-1,i2+1,2)-xy(i1+1,i2-1,2)))))
          dy = 0.5*abs(area/(max(
     *       abs(xy(i1+1,i2+1,1)-xy(i1-1,i2-1,1)),
     *       abs(xy(i1-1,i2+1,1)-xy(i1+1,i2-1,1)))))
          c1 = sqrt((lam+2.0*mu)/rho)
          c2 = sqrt(mu/rho)
          sp = max(c1,c2)
          almax(2) = max(almax(2),sp/dx+sp/dy)
        end do
      end do
c
c      if( .false. ) then
      if( .true. ) then
        call centArtVis2d( nd1a,nd1b,nd2a,nd2b,
     *                   n1a,n1b,n2a,n2b,u,up )
      end if
c
      rparam(1) = almax(1)
      rparam(2) = almax(2)
c      write(6,*)rparam(1),rparam(2)
c      write(6,*)'**END smcent**'
c
      return
      end
c
c+++++++++++++++
c
      subroutine centArtVis2d( nd1a,nd1b,nd2a,nd2b,
     *                       n1a,n1b,n2a,n2b,u,up )
c
c.. An artificial viscosity
      implicit none
c
      integer nd1a,nd1b,nd2a,nd2b
      integer n1a,n1b,n2a,n2b
      real u(nd1a:nd1b,nd2a:nd2b,1:*)
      real up(nd1a:nd1b,nd2a:nd2b,1:*)
c
      integer i1,i2,k
      real nu,dr,ds
      
      nu = 2.0e-1
      dr = 1.0/(n1b-n1a)
      ds = 1.0/(n2b-n2a)
c
c.. compute 4th order diffusion 
      do k=1,6
        do i2 = n2a,n2b
          do i1 = n1a,n1b
            up(i1,i2,k) = up(i1,i2,k)-nu*(
     *         u(i1-2,i2,k)-4.0*u(i1-1,i2,k)+
     *         6.0*u(i1,i2,k)-4.0*u(i1+1,i2,k)+
     *         u(i1+2,i2,k) )/dr-nu*(
     *         u(i1,i2-2,k)-4.0*u(i1,i2-1,k)+
     *         6.0*u(i1,i2,k)-4.0*u(i1,i2+1,k)+
     *         u(i1,i2+2,k) )/ds
          end do
        end do
      end do
c      
      return
      end
c
c+++++++++++++++++++++++
c
      subroutine centRiem2d( ul,ur,a1,a2,area,u0,mu,lam,rho )
c
c.. A Riemann solver
c
      implicit none
c
      real ul(*),ur(*),u0(*)
      real a1,a2,area,mu,lam,rho
c
      integer i,j
      real wl(6),wr(6),w0(6)
      real el(6,6),er(6,6),al(6)
c
c.. centered flux ...
      if( .true. ) then
c      if( .false. ) then
        do i=1,6
          u0(i) = 0.5*(ul(i)+ur(i))
        end do
        return
      end if
c
c.. determine eigenstructure
      call centEig2d( a1,a2,al,el,er,mu,lam,rho )
c
c.. convert to characteristic variables and pick center state
      do i = 1,6
        wl(i) = 0.0
        wr(i) = 0.0
        do j = 1,6
          wl(i) = wl(i)+el(i,j)*ul(j)
          wr(i) = wr(i)+el(i,j)*ur(j)
        end do
        if( area*al(i).gt.0.0 ) then
          w0(i) = wl(i)
        elseif( area*al(i).lt.0.0 ) then
          w0(i) = wr(i)
        else
          w0(i) = 0.5*(wl(i)+wr(i))
        end if
      end do
c
c.. convert back from characteristic variables
      do i = 1,6
        u0(i) = 0.0
        do j = 1,6
          u0(i) = u0(i)+er(i,j)*w0(j)
        end do
      end do
c
      return
      end
c
c++++++++++++++++++++
c
      subroutine centEig2d( a1,a2,al,el,er,mu,lam,rho )
c
c.. Eigenvalues and eigenvectors
      implicit none
c
      real a1,a2,mu,lam,rho
      real al(6),el(6,6),er(6,6)
c
      real rad,an1,an2,an11,an12,an22
      real c1,c2
c
c..directions
      rad  = sqrt(a1**2+a2**2)
      an1  = a1/rad
      an2  = a2/rad
      an11 = an1*an1
      an12 = an1*an2
      an22 = an2*an2
c
c..wave speeds
      c1 = sqrt((lam+2.0*mu)/rho)
      c2 = sqrt(mu/rho)
c
c..eigenvalues
      al(1) = -rad*c1
      al(2) = -rad*c2
      al(3) =  0.0
      al(4) =  0.0
      al(5) = -al(2)
      al(6) = -al(1)
c
c..left eigenvector
      el(1,1) =  0.5*an1/c1
      el(1,2) =  0.5*an2/c1
      el(1,3) =  0.5*an11/(lam+2.0*mu)
      el(1,4) =  0.5*an12/(lam+2.0*mu)
      el(1,5) =  el(1,4)
      el(1,6) =  0.5*an22/(lam+2.0*mu)
      el(2,1) = -0.5*an2/c2
      el(2,2) =  0.5*an1/c2
      el(2,3) = -0.5*an12/mu
      el(2,4) =  0.5*an11/mu
      el(2,5) = -0.5*an22/mu
      el(2,6) = -el(2,3)
      el(3,1) =  0.0
      el(3,2) =  0.0
      el(3,3) = -an2*(((an22-an11)*lam+2.0*an22*mu)/(lam+2.0*mu))
      el(3,4) = -an1*(((an11-an22)*lam+2.0*an11*mu)/(lam+2.0*mu))
      el(3,5) =  an1*(1.0+2.0*an22*(lam+mu)/(lam+2.0*mu))
      el(3,6) = -an2*(((an11-an22)*lam+2.0*an11*mu)/(lam+2.0*mu))
      el(4,1) =  0.0
      el(4,2) =  0.0
      el(4,3) =  an1*(((an22-an11)*lam+2.0*an22*mu)/(lam+2.0*mu))
      el(4,4) = -an2*(1.0+2.0*an11*(lam+mu)/(lam+2.0*mu))
      el(4,5) =  an2*(((an22-an11)*lam+2.0*an22*mu)/(lam+2.0*mu))
      el(4,6) =  an1*(((an11-an22)*lam+2.0*an11*mu)/(lam+2.0*mu))
      el(5,1) = -el(2,1)
      el(5,2) = -el(2,2)
      el(5,3) =  el(2,3)
      el(5,4) =  el(2,4)
      el(5,5) =  el(2,5)
      el(5,6) =  el(2,6)
      el(6,1) = -el(1,1)
      el(6,2) = -el(1,2)
      el(6,3) =  el(1,3)
      el(6,4) =  el(1,4)
      el(6,5) =  el(1,5)
      el(6,6) =  el(1,6)
c
c..right eigenvector
      er(1,1) =  an1*c1
      er(2,1) =  an2*c1
      er(3,1) =  lam+2.0*an11*mu
      er(4,1) =  2.0*an12*mu
      er(5,1) =  er(4,1)
      er(6,1) =  lam+2.0*an22*mu
      er(1,2) = -an2*c2
      er(2,2) =  an1*c2
      er(3,2) = -2.0*an12*mu
      er(4,2) =  (an11-an22)*mu
      er(5,2) =  er(4,2)
      er(6,2) = -er(3,2)
      er(1,3) =  0.0
      er(2,3) =  0.0
      er(3,3) = -an2
      er(4,3) =  0.0
      er(5,3) =  an1
      er(6,3) =  0.0
      er(1,4) =  0.0
      er(2,4) =  0.0
      er(3,4) =  0.0
      er(4,4) = -an2
      er(5,4) =  0.0
      er(6,4) =  an1
      er(1,5) = -er(1,2)
      er(2,5) = -er(2,2)
      er(3,5) =  er(3,2)
      er(4,5) =  er(4,2)
      er(5,5) =  er(5,2)
      er(6,5) =  er(6,2)
      er(1,6) = -er(1,1)
      er(2,6) = -er(2,1)
      er(3,6) =  er(3,1)
      er(4,6) =  er(4,1)
      er(5,6) =  er(5,1)
      er(6,6) =  er(6,1)
c
      return
      end
c
c+++++++++++++++++++++++
c
