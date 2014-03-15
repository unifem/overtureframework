      subroutine setQ( nxl,nxu,nyl,nyu,x,y,vx,vy,Area,
     *                 mass,rho0,e0,p,q,sx,sy,
     *                 c0,cl,R,
     *                 apr,bpr,cpr,dpr,lemu,lelambda,ilinear,
     *                 vismax )
c
c..declaration of incomming variables
c 
      implicit none
      integer nxl,nxu,nyl,nyu,ilinear
      real x,y,vx,vy,rho0,e0,p,q,mass,Area,c0,cl
      real sx,sy
      real apr,bpr,cpr,dpr,lemu,lelambda,R,vismax
      dimension x(nxl:nxu,nyl:nyu),y(nxl:nxu,nyl:nyu),
     *          vx(nxl:nxu,nyl:nyu),vy(nxl:nxu,nyl:nyu),
     *          Area(nxl:nxu-1,nyl:nyu-1),mass(nxl:nxu-1,nyl:nyu-1),
     *          rho0(nxl:nxu-1,nyl:nyu-1),e0(nxl:nxu-1,nyl:nyu-1),
     *          sx(nxl:nxu-1,nyl:nyu-1),sy(nxl:nxu-1,nyl:nyu-1),
     *          p(nxl:nxu-1,nyl:nyu-1),q(nxl:nxu-1,nyl:nyu-1)
c
c..declration of local variables
      integer i,j
      real d1,d2,rad,dvel,q1,q2,q3,q4,rhoij,a,eng
      real pmin,p_r,p_re,e_r,e_p,al1,al2,alx,aly
      real kap,temperature,a2,mu
      real lam1,lam2,lam3,lam4
c      data c0,cl / 1.e0,2.e0 /
      data pmin / 1.e-12 /
c
      vismax = 0.0
      do j = nyl,nyu-1
        do i = nxl,nxu-1
          ! calculate artificial viscosity "q"
          rhoij = mass(i,j)/Area(i,j)
          if( ilinear.eq.1 ) then
            mu = lemu
            kap = lelambda+mu*2.0/3.0
            al1 = sqrt((3.0*kap+4.0*mu)/(3.0*rhoij))
c            al2 = sqrt((2.0*mu+sy(i,j)-sx(i,j))/(2.0*rhoij))
            al2 = sqrt((2.0*mu)/(2.0*rhoij))
            alx = max( al1,al2 )

            al1 = sqrt((3.0*kap+4.0*mu)/(3.0*rhoij))
c            al2 = sqrt((2.0*mu+sx(i,j)-sy(i,j))/(2.0*rhoij))
            al2 = sqrt((2.0*mu)/(2.0*rhoij))
            aly = max( al1,al2 )
            a = max( alx,aly )
          else
            call getEng( rhoij/rho0(i,j),p(i,j),eng,apr,bpr,cpr,dpr )
            temperature = ((eng-e0(i,j))/rho0(i,j))/(3.0*R)
            call getMu( p(i,j),rho0(i,j)/rhoij,temperature,mu )
            call getEOSDerivs( rhoij,rho0(i,j),p(i,j),eng,
     *                       apr,bpr,cpr,dpr,
     *                       p_r,p_re,e_r,e_p )
            a2 = p_r+p_re*(eng+p(i,j)/rhoij)
            al1 = sqrt( (2.0*mu-sx(i,j)+sy(i,j))/(2.0*rhoij) )
            al2 = sqrt( (4.0*mu/3.0-p_re*sx(i,j))/rhoij+a2 )
            alx = max( al1,al2 )

            al1 = sqrt( (2.0*mu-sy(i,j)+sx(i,j))/(2.0*rhoij) )
            al2 = sqrt( (4.0*mu/3.0-p_re*sy(i,j))/rhoij+a2 )
            aly = max( al1,al2 )
            a = max( alx,aly )
c            a = sqrt(max(pmin,p(i,j))/rhoij)
          end if

          d1 = x(i+1,j)-x(i,j)
          d2 = y(i+1,j)-y(i,j)
          rad = sqrt(d1**2+d2**2)
          d1 = d1/rad
          d2 = d2/rad
          dvel = d1*(vx(i+1,j)-vx(i,j))+
     *           d2*(vy(i+1,j)-vy(i,j))
          if( dvel.gt.0.e0 ) then
            q1 = 0.e0
            lam1 = 0.0
          else
            q1 = c0**2*rhoij*dvel**2+cl*a*rhoij*abs(dvel)
            lam1 = (2.0*abs(dvel)*c0**2+cl*a)/rad
          end if
          
          d1 = x(i+1,j+1)-x(i+1,j)
          d2 = y(i+1,j+1)-y(i+1,j)
          rad = sqrt(d1**2+d2**2)
          d1 = d1/rad
          d2 = d2/rad
          dvel = d1*(vx(i+1,j+1)-vx(i+1,j))+
     *           d2*(vy(i+1,j+1)-vy(i+1,j))
          if( dvel.gt.0.e0 ) then
            q2 = 0.e0
            lam2 = 0.0
          else
            q2 = c0**2*rhoij*dvel**2+cl*a*rhoij*abs(dvel)
            lam2 = (2.0*abs(dvel)*c0**2+cl*a)/rad
          end if
    
          d1 = x(i,j+1)-x(i+1,j+1)
          d2 = y(i,j+1)-y(i+1,j+1)
          rad = sqrt(d1**2+d2**2)
          d1 = d1/rad
          d2 = d2/rad
          dvel = d1*(vx(i,j+1)-vx(i+1,j+1))+
     *           d2*(vy(i,j+1)-vy(i+1,j+1))
          if( dvel.gt.0.e0 ) then
            q3 = 0.e0
            lam3 = 0.0
          else
            q3 = c0**2*rhoij*dvel**2+cl*a*rhoij*abs(dvel)
            lam3 = (2.0*abs(dvel)*c0**2+cl*a)/rad
          end if
    
          d1 = x(i,j)-x(i,j+1)
          d2 = y(i,j)-y(i,j+1)
          rad = sqrt(d1**2+d2**2)
          d1 = d1/rad
          d2 = d2/rad
          dvel = d1*(vx(i,j)-vx(i,j+1))+
     *           d2*(vy(i,j)-vy(i,j+1))
          if( dvel.gt.0.e0 ) then
            q4 = 0.e0
            lam4 = 0.0
          else
            q4 = c0**2*rhoij*dvel**2+cl*a*rhoij*abs(dvel)
            lam4 = (2.0*abs(dvel)*c0**2+cl*a)/rad
          end if
    
          !q(i,j) = 0.5*(q1+q2+q3+q4)
          q(i,j) = max(q1,max(q2,max(q3,q4)))
          vismax = max(vismax,max(lam1,max(lam2,max(lam3,lam4))))
c          q(i,j) = 0.0
        end do
      end do
      
      return
      end
