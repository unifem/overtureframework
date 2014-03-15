c This function is called by the primer example "callingFortran.C"
c to illustrate how to call fortran from Overture

      subroutine mySolver( t,dt,a,b,nu,nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &   n1a,n1b,n2a,n2b,n3a,n3b,x,u,dudt )

      implicit none
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer n1a,n1b,n2a,n2b,n3a,n3b,nd
      real t,dt,a,b,nu
      real x(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real dudt(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      integer i1,i2,i3


      do i3=n3a,n3b
        do i2=n2a,n2b
          do i1=n1a,n1b

c          compute du/dt = -( x**2 + y**2 ) u
           dudt(i1,i2,i3)=-(x(i1,i2,i3,1)**2 + x(i1,i2,i3,2)**2)*
     &                  u(i1,i2,i3)

          end do
        end do
      end do

      return
      end
