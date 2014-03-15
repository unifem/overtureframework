! This file automatically generated from assignBoundaryConditions.bf with bpp.
         subroutine assignOptNormalComponent( nd,  n1a,n1b,n2a,n2b,n3a,
     & n3b, ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ndv1a,
     & ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b, ndc1a,ndc1b,ndc2a,
     & ndc2b,ndc3a,ndc3b,ndc4a,ndc4b, ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,
     & ndg3b,ndg4a,ndg4b, ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,
     & ndf4b, ndw1a,ndw1b, ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b, nd1a,
     & nd1b,nd2a,nd2b,nd3a,nd3b,rsxy, u,v,coeff, mask, scalarData,
     & gfData,fData,vData,  dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,
     & fC, side,axis,grid, bcType,bcOption,gridType,order,
     & useWhereMask, lineForForcing )
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
       !**      else if( bcType.eq.normalComponent )then
          if( bcType.ne.normalComponent )then
           write(*,'("ERROR")')
           stop 1148
          end if
       !        ****************************
       !        ***** Normal Component *****
       !        ****************************
        ! **** check these ***
        n1=ipar(0)  ! components are (n1,n2,n3)
        n2=ipar(1)
        n3=ipar(2)
        m1=ipar(3)  ! use RHS values (m1,m2,m3)
        m2=ipar(4)
        m3=ipar(5)
        nsign=2*side-1  ! sign to convert RHS to outward normal component
        if( gridType.eq.rectangular )then
       !          *************************
       !          *** rectangular grid  ***
       !          *************************
          cn=n1+axis ! This is the normal component for a rectangular grid
          cm=m1+axis ! normal component for forcing arrays
          if( bcOption.eq.scalarForcing )then
            if( scalarData.eq.0. )then
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    u(i1,i2,i3,cn)=0.
                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  u(i1,i2,i3,cn)=0.
                end do
                end do
                end do
              end if
            else
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    u(i1,i2,i3,cn)=scalarData*nsign
                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  u(i1,i2,i3,cn)=scalarData*nsign
                end do
                end do
                end do
              end if
            end if
          else if( bcOption.eq.gfForcing )then
            ! NOTE: There is no *nsign here since we set nsign*u = nsign*gfData *wdh* 040317
            if( useWhereMask.ne.0 )then
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  u(i1,i2,i3,cn)=gfData(i1,i2,i3,cm)
                end if
              end do
              end do
              end do
            else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                u(i1,i2,i3,cn)=gfData(i1,i2,i3,cm)
              end do
              end do
              end do
            end if
          else if( bcOption.eq.arrayForcing )then
            if( useWhereMask.ne.0 )then
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  u(i1,i2,i3,cn)=fData(cm,side,axis,grid)
                end if
              end do
              end do
              end do
            else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                u(i1,i2,i3,cn)=fData(cm,side,axis,grid)
              end do
              end do
              end do
            end if
          else if( bcOption.eq.vectorForcing )then
            if( useWhereMask.ne.0 )then
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  u(i1,i2,i3,cn)=vData(cm)
                end if
              end do
              end do
              end do
            else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                u(i1,i2,i3,cn)=vData(cm)
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
          if( bcOption.eq.scalarForcing )then
           if( scalarData.eq.0. )then
            if( useWhereMask.ne.0 )then
             if( nd.eq.2 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                an1=rsxy(i1,i2,i3,axis,0)
                an2=rsxy(i1,i2,i3,axis,1)
                nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)
                aNorm=an1**2+an2**2
                  nDotU=nDotU/aNorm
                u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
                u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
                  end if
                end do
                end do
                end do
             else if( nd.eq.3 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                an1=rsxy(i1,i2,i3,axis,0)
                an2=rsxy(i1,i2,i3,axis,1)
                an3=rsxy(i1,i2,i3,axis,2)
                nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)+an3*u(i1,
     & i2,i3,n3)
                aNorm=an1**2+an2**2+an3**2
                  nDotU=nDotU/aNorm
                u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
                u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
                u(i1,i2,i3,n3)=u(i1,i2,i3,n3)-nDotU*an3
                  end if
                end do
                end do
                end do
             else
              stop 92743
             end if
            else
             if( nd.eq.2 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                an1=rsxy(i1,i2,i3,axis,0)
                an2=rsxy(i1,i2,i3,axis,1)
                nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)
                aNorm=an1**2+an2**2
                  nDotU=nDotU/aNorm
                u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
                u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
                end do
                end do
                end do
             else if( nd.eq.3 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                an1=rsxy(i1,i2,i3,axis,0)
                an2=rsxy(i1,i2,i3,axis,1)
                an3=rsxy(i1,i2,i3,axis,2)
                nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)+an3*u(i1,
     & i2,i3,n3)
                aNorm=an1**2+an2**2+an3**2
                  nDotU=nDotU/aNorm
                u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
                u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
                u(i1,i2,i3,n3)=u(i1,i2,i3,n3)-nDotU*an3
                end do
                end do
                end do
             else
              stop 92743
             end if
            end if
           else
            if( useWhereMask.ne.0 )then
             if( nd.eq.2 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                an1=rsxy(i1,i2,i3,axis,0)
                an2=rsxy(i1,i2,i3,axis,1)
                nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)
                aNorm=an1**2+an2**2
                  nDotU=(nDotU - scalarData*nsign*sqrt(anorm) )/aNorm
                u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
                u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
                  end if
                end do
                end do
                end do
             else if( nd.eq.3 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                an1=rsxy(i1,i2,i3,axis,0)
                an2=rsxy(i1,i2,i3,axis,1)
                an3=rsxy(i1,i2,i3,axis,2)
                nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)+an3*u(i1,
     & i2,i3,n3)
                aNorm=an1**2+an2**2+an3**2
                  nDotU=(nDotU - scalarData*nsign*sqrt(anorm) )/aNorm
                u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
                u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
                u(i1,i2,i3,n3)=u(i1,i2,i3,n3)-nDotU*an3
                  end if
                end do
                end do
                end do
             else
              stop 92743
             end if
            else
             if( nd.eq.2 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                an1=rsxy(i1,i2,i3,axis,0)
                an2=rsxy(i1,i2,i3,axis,1)
                nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)
                aNorm=an1**2+an2**2
                  nDotU=(nDotU - scalarData*nsign*sqrt(anorm) )/aNorm
                u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
                u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
                end do
                end do
                end do
             else if( nd.eq.3 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                an1=rsxy(i1,i2,i3,axis,0)
                an2=rsxy(i1,i2,i3,axis,1)
                an3=rsxy(i1,i2,i3,axis,2)
                nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)+an3*u(i1,
     & i2,i3,n3)
                aNorm=an1**2+an2**2+an3**2
                  nDotU=(nDotU - scalarData*nsign*sqrt(anorm) )/aNorm
                u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
                u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
                u(i1,i2,i3,n3)=u(i1,i2,i3,n3)-nDotU*an3
                end do
                end do
                end do
             else
              stop 92743
             end if
            end if
           end if
          else if( bcOption.eq.gfForcing )then
           if( useWhereMask.ne.0 )then
            if( nd.eq.2 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)
               aNorm=an1**2+an2**2
                 nDotU=(nDotU - (an1*gfData(i1,i2,i3,m1)+an2*gfData(i1,
     & i2,i3,m2)) )/aNorm
               u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
               u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
                 end if
               end do
               end do
               end do
            else if( nd.eq.3 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               an3=rsxy(i1,i2,i3,axis,2)
               nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)+an3*u(i1,i2,
     & i3,n3)
               aNorm=an1**2+an2**2+an3**2
                 nDotU=(nDotU - (an1*gfData(i1,i2,i3,m1)+an2*gfData(i1,
     & i2,i3,m2)+an3*gfData(i1,i2,i3,m3)) )/aNorm
               u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
               u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
               u(i1,i2,i3,n3)=u(i1,i2,i3,n3)-nDotU*an3
                 end if
               end do
               end do
               end do
            else
             stop 92743
            end if
           else
            if( nd.eq.2 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)
               aNorm=an1**2+an2**2
                 nDotU=(nDotU - (an1*gfData(i1,i2,i3,m1)+an2*gfData(i1,
     & i2,i3,m2)) )/aNorm
               u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
               u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
               end do
               end do
               end do
            else if( nd.eq.3 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               an3=rsxy(i1,i2,i3,axis,2)
               nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)+an3*u(i1,i2,
     & i3,n3)
               aNorm=an1**2+an2**2+an3**2
                 nDotU=(nDotU - (an1*gfData(i1,i2,i3,m1)+an2*gfData(i1,
     & i2,i3,m2)+an3*gfData(i1,i2,i3,m3)) )/aNorm
               u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
               u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
               u(i1,i2,i3,n3)=u(i1,i2,i3,n3)-nDotU*an3
               end do
               end do
               end do
            else
             stop 92743
            end if
           end if
          else if( bcOption.eq.arrayForcing )then
           if( useWhereMask.ne.0 )then
            if( nd.eq.2 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)
               aNorm=an1**2+an2**2
                 nDotU=(nDotU - (an1*fData(m1,side,axis,grid)+an2*
     & fData(m2,side,axis,grid)) )/aNorm
               u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
               u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
                 end if
               end do
               end do
               end do
            else if( nd.eq.3 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               an3=rsxy(i1,i2,i3,axis,2)
               nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)+an3*u(i1,i2,
     & i3,n3)
               aNorm=an1**2+an2**2+an3**2
                 nDotU=(nDotU - (an1*fData(m1,side,axis,grid)+an2*
     & fData(m2,side,axis,grid)+an3*fData(m3,side,axis,grid)) )/aNorm
               u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
               u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
               u(i1,i2,i3,n3)=u(i1,i2,i3,n3)-nDotU*an3
                 end if
               end do
               end do
               end do
            else
             stop 92743
            end if
           else
            if( nd.eq.2 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)
               aNorm=an1**2+an2**2
                 nDotU=(nDotU - (an1*fData(m1,side,axis,grid)+an2*
     & fData(m2,side,axis,grid)) )/aNorm
               u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
               u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
               end do
               end do
               end do
            else if( nd.eq.3 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               an3=rsxy(i1,i2,i3,axis,2)
               nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)+an3*u(i1,i2,
     & i3,n3)
               aNorm=an1**2+an2**2+an3**2
                 nDotU=(nDotU - (an1*fData(m1,side,axis,grid)+an2*
     & fData(m2,side,axis,grid)+an3*fData(m3,side,axis,grid)) )/aNorm
               u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
               u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
               u(i1,i2,i3,n3)=u(i1,i2,i3,n3)-nDotU*an3
               end do
               end do
               end do
            else
             stop 92743
            end if
           end if
          else if( bcOption.eq.vectorForcing )then
           if( useWhereMask.ne.0 )then
            if( nd.eq.2 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)
               aNorm=an1**2+an2**2
                 nDotU=(nDotU - (an1*vData(m1)+an2*vData(m2)) )/aNorm
               u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
               u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
                 end if
               end do
               end do
               end do
            else if( nd.eq.3 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               an3=rsxy(i1,i2,i3,axis,2)
               nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)+an3*u(i1,i2,
     & i3,n3)
               aNorm=an1**2+an2**2+an3**2
                 nDotU=(nDotU - (an1*vData(m1)+an2*vData(m2)+an3*vData(
     & m3)) )/aNorm
               u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
               u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
               u(i1,i2,i3,n3)=u(i1,i2,i3,n3)-nDotU*an3
                 end if
               end do
               end do
               end do
            else
             stop 92743
            end if
           else
            if( nd.eq.2 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)
               aNorm=an1**2+an2**2
                 nDotU=(nDotU - (an1*vData(m1)+an2*vData(m2)) )/aNorm
               u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
               u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
               end do
               end do
               end do
            else if( nd.eq.3 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
               an1=rsxy(i1,i2,i3,axis,0)
               an2=rsxy(i1,i2,i3,axis,1)
               an3=rsxy(i1,i2,i3,axis,2)
               nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)+an3*u(i1,i2,
     & i3,n3)
               aNorm=an1**2+an2**2+an3**2
                 nDotU=(nDotU - (an1*vData(m1)+an2*vData(m2)+an3*vData(
     & m3)) )/aNorm
               u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
               u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
               u(i1,i2,i3,n3)=u(i1,i2,i3,n3)-nDotU*an3
               end do
               end do
               end do
            else
             stop 92743
            end if
           end if
          end if
        end if
        return
        end
