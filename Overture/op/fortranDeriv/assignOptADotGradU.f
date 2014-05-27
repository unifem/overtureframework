! This file automatically generated from assignBoundaryConditions.bf with bpp.
! assignBoundaryConditionMacro(assignOptADotGradU,aDotGradU)
         subroutine assignOptADotGradU( nd,  n1a,n1b,n2a,n2b,n3a,n3b, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ndv1a,ndv1b,
     & ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b, ndc1a,ndc1b,ndc2a,ndc2b,
     & ndc3a,ndc3b,ndc4a,ndc4b, ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,
     & ndg4a,ndg4b, ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b, 
     & ndw1a,ndw1b, ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b, nd1a,nd1b,
     & nd2a,nd2b,nd3a,nd3b,rsxy, u,v,coeff, mask, scalarData,gfData,
     & fData,vData,  dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC, 
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask, 
     & lineForForcing )
       !======================================================================
       !  Optimised Boundary Conditions
       !         
       ! nd : number of space dimensions
       ! uC(uCBase:*) : defines components to assign
       ! ca,cb : assign components c=uC(ca),..,uC(cb)
       ! fC(fCBase:*) : defines components of forcing to use.
       ! gridType: 0=rectangular, 1=non-rectangular
       ! order : 2 or 4 -- only 2 implemented
       ! par, ipar : real and integer parameters
       ! useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
       ! lineForForcing : if 0 evaluate the forcing gfData on the boundary, if 1 evaluate forcing
       !    on the first ghost line, etc.
       !
       ! v : for generalizedDiveregence v holds ux(i1,i2,i3,n1) uy(i1,i2,i3,n2) uz(i1,i2,i3,n3)
       !======================================================================
        implicit none
        integer nd, n1a,n1b,n2a,n2b,n3a,n3b, ndu1a,ndu1b,ndu2a,ndu2b,
     & ndu3a,ndu3b,ndu4a,ndu4b, ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,
     & ndv4a,ndv4b, ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b, 
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b, ndf1a,ndf1b,
     & ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b, ndw1a,ndw1b, ndm1a,ndm1b,
     & ndm2a,ndm2b,ndm3a,ndm3b,uCBase,fCBase
        integer side,axis,grid, bcType,bcOption,gridType,order,
     & useWhereMask,lineForForcing
        real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
        real v(ndv1a:ndv1b,ndv2a:ndv2b,ndv3a:ndv3b,ndv4a:ndv4b)
        real coeff(ndc1a:ndc1b,ndc2a:ndc2b,ndc3a:ndc3b,ndc4a:ndc4b)
        real gfData(ndg1a:ndg1b,ndg2a:ndg2b,ndg3a:ndg3b,ndg4a:ndg4b)
        real fData(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b,ndf4a:ndf4b)
        real vData(ndw1a:ndw1b)
        integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)
        integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
        real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
        integer ipar(0:*), ca,cb,uC(uCBase:*),fC(fCBase:*)
        real scalarData,par(0:*),dx(3),dr(3)
        real vv(0:2)
        integer axisp1,axisp2
       !     --- boundary conditions (from BCTypes.h) ---
        integer dirichlet, neumann, extrapolate, normalComponent, 
     & mixed, generalMixedDerivative, normalDerOfNormalComponent, 
     & normalDerivativeOfADotU, aDotU, aDotGradU, normalDotScalarGrad,
     &  evenSymmetry, oddSymmetry, generalizedDivergence, 
     & vectorSymmetry, tangentialComponent0, tangentialComponent1, 
     & normalDerOfTangentialComponent0, 
     & normalDerOfTangentialComponent1, extrapolateInterpNeighbours, 
     & tangentialComponent,                       
     & extrapolateNormalComponent, extrapolateTangentialComponent0, 
     & extrapolateTangentialComponent1
        parameter(dirichlet=0,neumann=dirichlet+1,extrapolate=neumann+
     & 1,normalComponent=extrapolate+1,mixed=normalComponent+1,
     & generalMixedDerivative=mixed+1,
     & normalDerOfNormalComponent=generalMixedDerivative+1,
     & normalDerivativeOfADotU=normalDerOfNormalComponent+1,
     & aDotU=normalDerivativeOfADotU+1,aDotGradU=aDotU+1,
     & normalDotScalarGrad=aDotGradU+1,
     & evenSymmetry=normalDotScalarGrad+1,oddSymmetry=evenSymmetry+1,
     & generalizedDivergence=oddSymmetry+1,
     & vectorSymmetry=generalizedDivergence+1,
     & tangentialComponent0=vectorSymmetry+1,
     & tangentialComponent1=tangentialComponent0+1,
     & normalDerOfTangentialComponent0=tangentialComponent1+1,
     & normalDerOfTangentialComponent1 
     & =normalDerOfTangentialComponent0+1,extrapolateInterpNeighbours 
     & =normalDerOfTangentialComponent1+1,tangentialComponent 
     & =extrapolateInterpNeighbours+1,
     & extrapolateNormalComponent=tangentialComponent+1,
     & extrapolateTangentialComponent0 =extrapolateNormalComponent+1,
     & extrapolateTangentialComponent1 
     & =extrapolateTangentialComponent0+1 )
       !     --- grid types
        integer rectangular,curvilinear
        parameter( rectangular=0,curvilinear=1 )
       !     --- forcing types ---
        integer  scalarForcing,gfForcing,arrayForcing,vectorForcing
        parameter( scalarForcing=0,gfForcing=1,arrayForcing=2,
     & vectorForcing=3 )
       !     --- local variables ----
        real b0,b1,b2,b3,twoDeltaX,twoDeltaY,twoDeltaZ,temp,g1,g2,g3
        real nsign,an1,an2,an3,nDotU,anorm,aNormi
        integer c,c0,f,i1,i2,i3,im1,im2,im3,ip1,ip2,ip3,if1,if2,if3,cn,
     & cm
        integer n1,n2,n3,m1,m2,m3,v0,v1,v2,ct1,ct2
        real epsX
        integer mGhost
        integer m21,m12,m22,m32,m23
        integer m221,m212,m122,m222,m322,m232,m223
        integer varCoeff
        epsX=1.e-100  ! prevent division by zero when normalizing the normal vector  *FIX ME* -- this should be passed in
        if( side.lt.0 .or. side.gt.1 )then
          write(*,*) 'applyBoundaryConditions:ERROR: side=',side
          stop 1
        end if
        if( axis.lt.0 .or. axis.ge.nd )then
          write(*,*) 'applyBoundaryConditions:ERROR: axise=',axis
          stop 2
        end if
        if( useWheremask.ne.0 )then
          if( n1a.lt.ndm1a .or. n1b.gt.ndm1b .or. n2a.lt.ndm2a .or. 
     & n2b.gt.ndm2b .or. n3a.lt.ndm3a .or. n3b.gt.ndm3b )then
            write(*,'("ERROR:assignBoundaryConditions:mask bounds are 
     & wrong for useWhereMask")')
            write(*,'(" n1a,n1b,n2a,n2b,n3a,n3b=",6i5)') n1a,n1b,n2a,
     & n2b,n3a,n3b
            write(*,'(" ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b=",6i5)') 
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b
            ! '
            stop 8855
          end if
        end if
        im1=0
        im2=0
        im3=0
        if( axis.eq.0 )then
          im1=2*side-1
        else if( axis.eq.1 )then
          im2=2*side-1
        else
          im3=2*side-1
        end if
        ip1=-im1
        ip2=-im2
        ip3=-im3
        if1=im1*lineForForcing
        if2=im2*lineForForcing
        if3=im3*lineForForcing
! #If "aDotGradU" eq "neumann"
! #Elif "aDotGradU" eq "aDotGradU"
         !  from pmb 080421
         if( bcType.ne.aDotGradU)then
           write(*,'("ERROR")')
           stop 1145
         end if
       !        **********************
       !        ***** aDotGradU  *****
       !        **********************
        b0=par(0)
        b1=par(1)
        b2=par(2)
        twoDeltaX=par(3)
        twoDeltaY=par(4)
        twoDeltaZ=par(5)
        if( gridType.eq.rectangular )then
       !          *************************
       !          *** rectangular grid  ***
       !          *************************
            if( bcOption.eq.scalarForcing )then
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(scalarData-b0*u(i1,i2,i3,c))*(twoDeltaX/b1));
              if( useWhereMask.ne.0 )then
                do c0=ca,cb
                  c=uC(c0)  ! component of u
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                          u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+
     & ip3,c)+(scalarData-b0*u(i1,i2,i3,c))*(twoDeltaX/b1)
                        end if
                      end do
                    end do
                  end do
                end do
              else
                do c0=ca,cb
                  c=uC(c0)
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+
     & ip3,c)+(scalarData-b0*u(i1,i2,i3,c))*(twoDeltaX/b1)
                      end do
                    end do
                  end do
                end do
              end if
            else if( bcOption.eq.gfForcing )then
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(gfData(i1+if1,i2+if2,i3+if3,f)-b0*u(i1,i2,i3,c))*(twoDeltaX/b1))
              if( useWhereMask.ne.0 )then
                do c0=ca,cb
                  c=uC(c0)  ! component of u
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                          u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+
     & ip3,c)+(gfData(i1+if1,i2+if2,i3+if3,f)-b0*u(i1,i2,i3,c))*(
     & twoDeltaX/b1)
                        end if
                      end do
                    end do
                  end do
                end do
              else
                do c0=ca,cb
                  c=uC(c0)
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+
     & ip3,c)+(gfData(i1+if1,i2+if2,i3+if3,f)-b0*u(i1,i2,i3,c))*(
     & twoDeltaX/b1)
                      end do
                    end do
                  end do
                end do
              end if
            else if( bcOption.eq.arrayForcing )then
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(fData(f,side,axis,grid)-b0*u(i1,i2,i3,c))*(twoDeltaX/b1))
              if( useWhereMask.ne.0 )then
                do c0=ca,cb
                  c=uC(c0)  ! component of u
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                          u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+
     & ip3,c)+(fData(f,side,axis,grid)-b0*u(i1,i2,i3,c))*(
     & twoDeltaX/b1)
                        end if
                      end do
                    end do
                  end do
                end do
              else
                do c0=ca,cb
                  c=uC(c0)
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+
     & ip3,c)+(fData(f,side,axis,grid)-b0*u(i1,i2,i3,c))*(
     & twoDeltaX/b1)
                      end do
                    end do
                  end do
                end do
              end if
            else if( bcOption.eq.vectorForcing )then
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(vData(f)-b0*u(i1,i2,i3,c))*(twoDeltaX/b1))
              if( useWhereMask.ne.0 )then
                do c0=ca,cb
                  c=uC(c0)  ! component of u
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                          u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+
     & ip3,c)+(vData(f)-b0*u(i1,i2,i3,c))*(twoDeltaX/b1)
                        end if
                      end do
                    end do
                  end do
                end do
              else
                do c0=ca,cb
                  c=uC(c0)
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+
     & ip3,c)+(vData(f)-b0*u(i1,i2,i3,c))*(twoDeltaX/b1)
                      end do
                    end do
                  end do
                end do
              end if
            else
              write(*,*) 'assignBC:ERROR unknown bcOption=',bcOption
              stop 2
            end if
        else
       !          *************************
       !          **** Curvilinear case ***
       !          *************************
          if( nd.eq.2 )then
            m21=1
            m12=3
            m22=4
            m32=5
            m23=7
            if( axis.eq.0 )then
              mGhost=m22+2*side-1
            else
              mGhost=m22+3*(2*side-1)
            end if
            ! first zero out ghost value
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=0.)
            if( useWhereMask.ne.0 )then
              do c0=ca,cb
                c=uC(c0)  ! component of u
                f=fC(c0) ! component of forcing
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                        u(i1+im1,i2+im2,i3+im3,c)=0.
                      end if
                    end do
                  end do
                end do
              end do
            else
              do c0=ca,cb
                c=uC(c0)
                f=fC(c0) ! component of forcing
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      u(i1+im1,i2+im2,i3+im3,c)=0.
                    end do
                  end do
                end do
              end do
            end if
            if( bcOption.eq.scalarForcing )then
! aDotGradULoops2D(scalarData)
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=( scalarData - ( coeff(m21,i1,i2,i3)*u(i1  ,i2-1,i3,c) +coeff(m12,i1,i2,i3)*u(i1-1,i2  ,i3,c) +coeff(m22,i1,i2,i3)*u(i1  ,i2  ,i3,c) +coeff(m32,i1,i2,i3)*u(i1+1,i2  ,i3,c) +coeff(m23,i1,i2,i3)*u(i1  ,i2+1,i3,c) ))/coeff(mGhost,i1,i2,i3) )
              if( useWhereMask.ne.0 )then
                do c0=ca,cb
                  c=uC(c0)  ! component of u
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                          u(i1+im1,i2+im2,i3+im3,c)=(scalarData-(coeff(
     & m21,i1,i2,i3)*u(i1,i2-1,i3,c)+coeff(m12,i1,i2,i3)*u(i1-1,i2,i3,
     & c)+coeff(m22,i1,i2,i3)*u(i1,i2,i3,c)+coeff(m32,i1,i2,i3)*u(i1+
     & 1,i2,i3,c)+coeff(m23,i1,i2,i3)*u(i1,i2+1,i3,c)))/coeff(mGhost,
     & i1,i2,i3)
                        end if
                      end do
                    end do
                  end do
                end do
              else
                do c0=ca,cb
                  c=uC(c0)
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        u(i1+im1,i2+im2,i3+im3,c)=(scalarData-(coeff(
     & m21,i1,i2,i3)*u(i1,i2-1,i3,c)+coeff(m12,i1,i2,i3)*u(i1-1,i2,i3,
     & c)+coeff(m22,i1,i2,i3)*u(i1,i2,i3,c)+coeff(m32,i1,i2,i3)*u(i1+
     & 1,i2,i3,c)+coeff(m23,i1,i2,i3)*u(i1,i2+1,i3,c)))/coeff(mGhost,
     & i1,i2,i3)
                      end do
                    end do
                  end do
                end do
              end if
            else if( bcOption.eq.gfForcing )then
! aDotGradULoops2D(gfData(i1+if1,i2+if2,i3+if3,f))
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=( gfData(i1+if1,i2+if2,i3+if3,f) - ( coeff(m21,i1,i2,i3)*u(i1  ,i2-1,i3,c) +coeff(m12,i1,i2,i3)*u(i1-1,i2  ,i3,c) +coeff(m22,i1,i2,i3)*u(i1  ,i2  ,i3,c) +coeff(m32,i1,i2,i3)*u(i1+1,i2  ,i3,c) +coeff(m23,i1,i2,i3)*u(i1  ,i2+1,i3,c) ))/coeff(mGhost,i1,i2,i3) )
              if( useWhereMask.ne.0 )then
                do c0=ca,cb
                  c=uC(c0)  ! component of u
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                          u(i1+im1,i2+im2,i3+im3,c)=(gfData(i1+if1,i2+
     & if2,i3+if3,f)-(coeff(m21,i1,i2,i3)*u(i1,i2-1,i3,c)+coeff(m12,
     & i1,i2,i3)*u(i1-1,i2,i3,c)+coeff(m22,i1,i2,i3)*u(i1,i2,i3,c)+
     & coeff(m32,i1,i2,i3)*u(i1+1,i2,i3,c)+coeff(m23,i1,i2,i3)*u(i1,
     & i2+1,i3,c)))/coeff(mGhost,i1,i2,i3)
                        end if
                      end do
                    end do
                  end do
                end do
              else
                do c0=ca,cb
                  c=uC(c0)
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        u(i1+im1,i2+im2,i3+im3,c)=(gfData(i1+if1,i2+
     & if2,i3+if3,f)-(coeff(m21,i1,i2,i3)*u(i1,i2-1,i3,c)+coeff(m12,
     & i1,i2,i3)*u(i1-1,i2,i3,c)+coeff(m22,i1,i2,i3)*u(i1,i2,i3,c)+
     & coeff(m32,i1,i2,i3)*u(i1+1,i2,i3,c)+coeff(m23,i1,i2,i3)*u(i1,
     & i2+1,i3,c)))/coeff(mGhost,i1,i2,i3)
                      end do
                    end do
                  end do
                end do
              end if
            else if( bcOption.eq.arrayForcing )then
! aDotGradULoops2D(fData(f,side,axis,grid))
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=( fData(f,side,axis,grid) - ( coeff(m21,i1,i2,i3)*u(i1  ,i2-1,i3,c) +coeff(m12,i1,i2,i3)*u(i1-1,i2  ,i3,c) +coeff(m22,i1,i2,i3)*u(i1  ,i2  ,i3,c) +coeff(m32,i1,i2,i3)*u(i1+1,i2  ,i3,c) +coeff(m23,i1,i2,i3)*u(i1  ,i2+1,i3,c) ))/coeff(mGhost,i1,i2,i3) )
              if( useWhereMask.ne.0 )then
                do c0=ca,cb
                  c=uC(c0)  ! component of u
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                          u(i1+im1,i2+im2,i3+im3,c)=(fData(f,side,axis,
     & grid)-(coeff(m21,i1,i2,i3)*u(i1,i2-1,i3,c)+coeff(m12,i1,i2,i3)*
     & u(i1-1,i2,i3,c)+coeff(m22,i1,i2,i3)*u(i1,i2,i3,c)+coeff(m32,i1,
     & i2,i3)*u(i1+1,i2,i3,c)+coeff(m23,i1,i2,i3)*u(i1,i2+1,i3,c)))
     & /coeff(mGhost,i1,i2,i3)
                        end if
                      end do
                    end do
                  end do
                end do
              else
                do c0=ca,cb
                  c=uC(c0)
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        u(i1+im1,i2+im2,i3+im3,c)=(fData(f,side,axis,
     & grid)-(coeff(m21,i1,i2,i3)*u(i1,i2-1,i3,c)+coeff(m12,i1,i2,i3)*
     & u(i1-1,i2,i3,c)+coeff(m22,i1,i2,i3)*u(i1,i2,i3,c)+coeff(m32,i1,
     & i2,i3)*u(i1+1,i2,i3,c)+coeff(m23,i1,i2,i3)*u(i1,i2+1,i3,c)))
     & /coeff(mGhost,i1,i2,i3)
                      end do
                    end do
                  end do
                end do
              end if
            else if( bcOption.eq.vectorForcing )then
! aDotGradULoops2D(vData(f))
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=( vData(f) - ( coeff(m21,i1,i2,i3)*u(i1  ,i2-1,i3,c) +coeff(m12,i1,i2,i3)*u(i1-1,i2  ,i3,c) +coeff(m22,i1,i2,i3)*u(i1  ,i2  ,i3,c) +coeff(m32,i1,i2,i3)*u(i1+1,i2  ,i3,c) +coeff(m23,i1,i2,i3)*u(i1  ,i2+1,i3,c) ))/coeff(mGhost,i1,i2,i3) )
              if( useWhereMask.ne.0 )then
                do c0=ca,cb
                  c=uC(c0)  ! component of u
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                          u(i1+im1,i2+im2,i3+im3,c)=(vData(f)-(coeff(
     & m21,i1,i2,i3)*u(i1,i2-1,i3,c)+coeff(m12,i1,i2,i3)*u(i1-1,i2,i3,
     & c)+coeff(m22,i1,i2,i3)*u(i1,i2,i3,c)+coeff(m32,i1,i2,i3)*u(i1+
     & 1,i2,i3,c)+coeff(m23,i1,i2,i3)*u(i1,i2+1,i3,c)))/coeff(mGhost,
     & i1,i2,i3)
                        end if
                      end do
                    end do
                  end do
                end do
              else
                do c0=ca,cb
                  c=uC(c0)
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        u(i1+im1,i2+im2,i3+im3,c)=(vData(f)-(coeff(m21,
     & i1,i2,i3)*u(i1,i2-1,i3,c)+coeff(m12,i1,i2,i3)*u(i1-1,i2,i3,c)+
     & coeff(m22,i1,i2,i3)*u(i1,i2,i3,c)+coeff(m32,i1,i2,i3)*u(i1+1,
     & i2,i3,c)+coeff(m23,i1,i2,i3)*u(i1,i2+1,i3,c)))/coeff(mGhost,i1,
     & i2,i3)
                      end do
                    end do
                  end do
                end do
              end if
            end if
          else
            m221=4
            m212=10
            m122=12
            m222=13
            m322=14
            m232=16
            m223=22
            if( axis.eq.0 )then
              mGhost=m222+2*side-1
            else if( axis.eq.1 )then
              mGhost=m222+3*(2*side-1)
            else
              mGhost=m222+9*(2*side-1)
            end if
            ! first zero out ghost value
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=0.)
            if( useWhereMask.ne.0 )then
              do c0=ca,cb
                c=uC(c0)  ! component of u
                f=fC(c0) ! component of forcing
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                        u(i1+im1,i2+im2,i3+im3,c)=0.
                      end if
                    end do
                  end do
                end do
              end do
            else
              do c0=ca,cb
                c=uC(c0)
                f=fC(c0) ! component of forcing
                do i3=n3a,n3b
                  do i2=n2a,n2b
                    do i1=n1a,n1b
                      u(i1+im1,i2+im2,i3+im3,c)=0.
                    end do
                  end do
                end do
              end do
            end if
            if( bcOption.eq.scalarForcing )then
! aDotGradULoops3D(scalarData)
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=( scalarData - ( coeff(m221,i1,i2,i3)*u(i1  ,i2  ,i3-1,c) +coeff(m212,i1,i2,i3)*u(i1  ,i2-1,i3  ,c) +coeff(m122,i1,i2,i3)*u(i1-1,i2  ,i3  ,c) +coeff(m222,i1,i2,i3)*u(i1  ,i2  ,i3  ,c) +coeff(m322,i1,i2,i3)*u(i1+1,i2  ,i3  ,c) +coeff(m232,i1,i2,i3)*u(i1  ,i2+1,i3  ,c) +coeff(m223,i1,i2,i3)*u(i1  ,i2  ,i3+1,c) ))/coeff(mGhost,i1,i2,i3) )
              if( useWhereMask.ne.0 )then
                do c0=ca,cb
                  c=uC(c0)  ! component of u
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                          u(i1+im1,i2+im2,i3+im3,c)=(scalarData-(coeff(
     & m221,i1,i2,i3)*u(i1,i2,i3-1,c)+coeff(m212,i1,i2,i3)*u(i1,i2-1,
     & i3,c)+coeff(m122,i1,i2,i3)*u(i1-1,i2,i3,c)+coeff(m222,i1,i2,i3)
     & *u(i1,i2,i3,c)+coeff(m322,i1,i2,i3)*u(i1+1,i2,i3,c)+coeff(m232,
     & i1,i2,i3)*u(i1,i2+1,i3,c)+coeff(m223,i1,i2,i3)*u(i1,i2,i3+1,c))
     & )/coeff(mGhost,i1,i2,i3)
                        end if
                      end do
                    end do
                  end do
                end do
              else
                do c0=ca,cb
                  c=uC(c0)
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        u(i1+im1,i2+im2,i3+im3,c)=(scalarData-(coeff(
     & m221,i1,i2,i3)*u(i1,i2,i3-1,c)+coeff(m212,i1,i2,i3)*u(i1,i2-1,
     & i3,c)+coeff(m122,i1,i2,i3)*u(i1-1,i2,i3,c)+coeff(m222,i1,i2,i3)
     & *u(i1,i2,i3,c)+coeff(m322,i1,i2,i3)*u(i1+1,i2,i3,c)+coeff(m232,
     & i1,i2,i3)*u(i1,i2+1,i3,c)+coeff(m223,i1,i2,i3)*u(i1,i2,i3+1,c))
     & )/coeff(mGhost,i1,i2,i3)
                      end do
                    end do
                  end do
                end do
              end if
            else if( bcOption.eq.gfForcing )then
! aDotGradULoops3D(gfData(i1+if1,i2+if2,i3+if3,f))
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=( gfData(i1+if1,i2+if2,i3+if3,f) - ( coeff(m221,i1,i2,i3)*u(i1  ,i2  ,i3-1,c) +coeff(m212,i1,i2,i3)*u(i1  ,i2-1,i3  ,c) +coeff(m122,i1,i2,i3)*u(i1-1,i2  ,i3  ,c) +coeff(m222,i1,i2,i3)*u(i1  ,i2  ,i3  ,c) +coeff(m322,i1,i2,i3)*u(i1+1,i2  ,i3  ,c) +coeff(m232,i1,i2,i3)*u(i1  ,i2+1,i3  ,c) +coeff(m223,i1,i2,i3)*u(i1  ,i2  ,i3+1,c) ))/coeff(mGhost,i1,i2,i3) )
              if( useWhereMask.ne.0 )then
                do c0=ca,cb
                  c=uC(c0)  ! component of u
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                          u(i1+im1,i2+im2,i3+im3,c)=(gfData(i1+if1,i2+
     & if2,i3+if3,f)-(coeff(m221,i1,i2,i3)*u(i1,i2,i3-1,c)+coeff(m212,
     & i1,i2,i3)*u(i1,i2-1,i3,c)+coeff(m122,i1,i2,i3)*u(i1-1,i2,i3,c)+
     & coeff(m222,i1,i2,i3)*u(i1,i2,i3,c)+coeff(m322,i1,i2,i3)*u(i1+1,
     & i2,i3,c)+coeff(m232,i1,i2,i3)*u(i1,i2+1,i3,c)+coeff(m223,i1,i2,
     & i3)*u(i1,i2,i3+1,c)))/coeff(mGhost,i1,i2,i3)
                        end if
                      end do
                    end do
                  end do
                end do
              else
                do c0=ca,cb
                  c=uC(c0)
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        u(i1+im1,i2+im2,i3+im3,c)=(gfData(i1+if1,i2+
     & if2,i3+if3,f)-(coeff(m221,i1,i2,i3)*u(i1,i2,i3-1,c)+coeff(m212,
     & i1,i2,i3)*u(i1,i2-1,i3,c)+coeff(m122,i1,i2,i3)*u(i1-1,i2,i3,c)+
     & coeff(m222,i1,i2,i3)*u(i1,i2,i3,c)+coeff(m322,i1,i2,i3)*u(i1+1,
     & i2,i3,c)+coeff(m232,i1,i2,i3)*u(i1,i2+1,i3,c)+coeff(m223,i1,i2,
     & i3)*u(i1,i2,i3+1,c)))/coeff(mGhost,i1,i2,i3)
                      end do
                    end do
                  end do
                end do
              end if
            else if( bcOption.eq.arrayForcing )then
! aDotGradULoops3D(fData(f,side,axis,grid))
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=( fData(f,side,axis,grid) - ( coeff(m221,i1,i2,i3)*u(i1  ,i2  ,i3-1,c) +coeff(m212,i1,i2,i3)*u(i1  ,i2-1,i3  ,c) +coeff(m122,i1,i2,i3)*u(i1-1,i2  ,i3  ,c) +coeff(m222,i1,i2,i3)*u(i1  ,i2  ,i3  ,c) +coeff(m322,i1,i2,i3)*u(i1+1,i2  ,i3  ,c) +coeff(m232,i1,i2,i3)*u(i1  ,i2+1,i3  ,c) +coeff(m223,i1,i2,i3)*u(i1  ,i2  ,i3+1,c) ))/coeff(mGhost,i1,i2,i3) )
              if( useWhereMask.ne.0 )then
                do c0=ca,cb
                  c=uC(c0)  ! component of u
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                          u(i1+im1,i2+im2,i3+im3,c)=(fData(f,side,axis,
     & grid)-(coeff(m221,i1,i2,i3)*u(i1,i2,i3-1,c)+coeff(m212,i1,i2,
     & i3)*u(i1,i2-1,i3,c)+coeff(m122,i1,i2,i3)*u(i1-1,i2,i3,c)+coeff(
     & m222,i1,i2,i3)*u(i1,i2,i3,c)+coeff(m322,i1,i2,i3)*u(i1+1,i2,i3,
     & c)+coeff(m232,i1,i2,i3)*u(i1,i2+1,i3,c)+coeff(m223,i1,i2,i3)*u(
     & i1,i2,i3+1,c)))/coeff(mGhost,i1,i2,i3)
                        end if
                      end do
                    end do
                  end do
                end do
              else
                do c0=ca,cb
                  c=uC(c0)
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        u(i1+im1,i2+im2,i3+im3,c)=(fData(f,side,axis,
     & grid)-(coeff(m221,i1,i2,i3)*u(i1,i2,i3-1,c)+coeff(m212,i1,i2,
     & i3)*u(i1,i2-1,i3,c)+coeff(m122,i1,i2,i3)*u(i1-1,i2,i3,c)+coeff(
     & m222,i1,i2,i3)*u(i1,i2,i3,c)+coeff(m322,i1,i2,i3)*u(i1+1,i2,i3,
     & c)+coeff(m232,i1,i2,i3)*u(i1,i2+1,i3,c)+coeff(m223,i1,i2,i3)*u(
     & i1,i2,i3+1,c)))/coeff(mGhost,i1,i2,i3)
                      end do
                    end do
                  end do
                end do
              end if
            else if( bcOption.eq.vectorForcing )then
! aDotGradULoops3D(vData(f))
! loopsm(u(i1+im1,i2+im2,i3+im3,c)=( vData(f) - ( coeff(m221,i1,i2,i3)*u(i1  ,i2  ,i3-1,c) +coeff(m212,i1,i2,i3)*u(i1  ,i2-1,i3  ,c) +coeff(m122,i1,i2,i3)*u(i1-1,i2  ,i3  ,c) +coeff(m222,i1,i2,i3)*u(i1  ,i2  ,i3  ,c) +coeff(m322,i1,i2,i3)*u(i1+1,i2  ,i3  ,c) +coeff(m232,i1,i2,i3)*u(i1  ,i2+1,i3  ,c) +coeff(m223,i1,i2,i3)*u(i1  ,i2  ,i3+1,c) ))/coeff(mGhost,i1,i2,i3) )
              if( useWhereMask.ne.0 )then
                do c0=ca,cb
                  c=uC(c0)  ! component of u
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
                          u(i1+im1,i2+im2,i3+im3,c)=(vData(f)-(coeff(
     & m221,i1,i2,i3)*u(i1,i2,i3-1,c)+coeff(m212,i1,i2,i3)*u(i1,i2-1,
     & i3,c)+coeff(m122,i1,i2,i3)*u(i1-1,i2,i3,c)+coeff(m222,i1,i2,i3)
     & *u(i1,i2,i3,c)+coeff(m322,i1,i2,i3)*u(i1+1,i2,i3,c)+coeff(m232,
     & i1,i2,i3)*u(i1,i2+1,i3,c)+coeff(m223,i1,i2,i3)*u(i1,i2,i3+1,c))
     & )/coeff(mGhost,i1,i2,i3)
                        end if
                      end do
                    end do
                  end do
                end do
              else
                do c0=ca,cb
                  c=uC(c0)
                  f=fC(c0) ! component of forcing
                  do i3=n3a,n3b
                    do i2=n2a,n2b
                      do i1=n1a,n1b
                        u(i1+im1,i2+im2,i3+im3,c)=(vData(f)-(coeff(
     & m221,i1,i2,i3)*u(i1,i2,i3-1,c)+coeff(m212,i1,i2,i3)*u(i1,i2-1,
     & i3,c)+coeff(m122,i1,i2,i3)*u(i1-1,i2,i3,c)+coeff(m222,i1,i2,i3)
     & *u(i1,i2,i3,c)+coeff(m322,i1,i2,i3)*u(i1+1,i2,i3,c)+coeff(m232,
     & i1,i2,i3)*u(i1,i2+1,i3,c)+coeff(m223,i1,i2,i3)*u(i1,i2,i3+1,c))
     & )/coeff(mGhost,i1,i2,i3)
                      end do
                    end do
                  end do
                end do
              end if
            end if
          end if
        end if
        return
        end
