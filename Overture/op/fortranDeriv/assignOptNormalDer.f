! This file automatically generated from assignBoundaryConditions.bf with bpp.
! assignBoundaryConditionMacro(assignOptNormalDer,normalDerivative)
         subroutine assignOptNormalDer( nd,  n1a,n1b,n2a,n2b,n3a,n3b, 
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
! #If "normalDerivative" eq "neumann"
! #Elif "normalDerivative" eq "aDotGradU"
! #Elif "normalDerivative" eq "generalizedDivergence"
! #Elif "normalDerivative" eq "normalDerivative"
          if( .not.(bcType.eq.normalDerOfNormalComponent 
     & .or.bcType.eq.normalDerOfTangentialComponent0 
     & .or.bcType.eq.normalDerOfTangentialComponent1) )then
           write(*,'("ERROR")')
           stop 1147
          end if
       !**      else if( bcType.eq.normalDerOfNormalComponent .or.
       !**     &         bcType.eq.normalDerOfTangentialComponent0 .or.
       !**     &         bcType.eq.normalDerOfTangentialComponent1 )then
       !       **************************************************
       !       **** normal derivative of a vector dot u *********
       !       **************************************************
         twoDeltaX=par(0)
         n1=ipar(0)
         n2=ipar(1)
         n3=ipar(2)
         m1=ipar(3)
         m2=ipar(4)
         m3=ipar(5)
         v0=ipar(6)
         v1=ipar(7)
         v2=ipar(8)
         if( gridType.eq.rectangular ) then
           vv(0)=0.
           vv(1)=0.
           vv(2)=0.
           if( bcType.eq.normalDerOfNormalComponent )then
             vv(axis)=2*side-1  ! outward normal
           else if( bcType.eq.normalDerOfTangentialComponent0 ) then
             ! The tangent is just xr, xs or xt
             axisp1=mod(axis+1,nd)
             vv(axisp1)=1.
           else if( bcType.eq.normalDerOfTangentialComponent1 ) then
             axisp2=mod(axis+2,nd)
             vv(axisp2)=1.
           else
             stop 1253
           end if
           if( bcOption.eq.scalarForcing ) then
             if( nd.eq.2 ) then
! loopsd4(temp=scalarData*twoDeltaX+(u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1))*vv(0) +(u(i1+ip1,i2+ip2,i3,n2)-u(i1+im1,i2+im2,i3,n2))*vv(1), u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*vv(0), u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*vv(1), )
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   temp=scalarData*twoDeltaX+(u(i1+ip1,i2+ip2,i3,n1)-u(
     & i1+im1,i2+im2,i3,n1))*vv(0)+(u(i1+ip1,i2+ip2,i3,n2)-u(i1+im1,
     & i2+im2,i3,n2))*vv(1)
                   u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & vv(0)
                   u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & vv(1)

                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   temp=scalarData*twoDeltaX+(u(i1+ip1,i2+ip2,i3,n1)-u(
     & i1+im1,i2+im2,i3,n1))*vv(0)+(u(i1+ip1,i2+ip2,i3,n2)-u(i1+im1,
     & i2+im2,i3,n2))*vv(1)
                   u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & vv(0)
                   u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & vv(1)

               end do
               end do
               end do
             end if
             else if( nd.eq.3 ) then
! loopsd4(temp=scalarData*twoDeltaX+(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1))*vv(0) +(u(i1+ip1,i2+ip2,i3+ip3,n2)-u(i1+im1,i2+im2,i3+im3,n2))*vv(1) +(u(i1+ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+im2,i3+im3,n3))*vv(2),u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*vv(0), u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*vv(1), u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*vv(2))
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   temp=scalarData*twoDeltaX+(u(i1+ip1,i2+ip2,i3+ip3,
     & n1)-u(i1+im1,i2+im2,i3+im3,n1))*vv(0)+(u(i1+ip1,i2+ip2,i3+ip3,
     & n2)-u(i1+im1,i2+im2,i3+im3,n2))*vv(1)+(u(i1+ip1,i2+ip2,i3+ip3,
     & n3)-u(i1+im1,i2+im2,i3+im3,n3))*vv(2)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*vv(1)
                   u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*vv(2)
                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   temp=scalarData*twoDeltaX+(u(i1+ip1,i2+ip2,i3+ip3,
     & n1)-u(i1+im1,i2+im2,i3+im3,n1))*vv(0)+(u(i1+ip1,i2+ip2,i3+ip3,
     & n2)-u(i1+im1,i2+im2,i3+im3,n2))*vv(1)+(u(i1+ip1,i2+ip2,i3+ip3,
     & n3)-u(i1+im1,i2+im2,i3+im3,n3))*vv(2)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*vv(1)
                   u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*vv(2)
               end do
               end do
               end do
             end if
             else
! loopsd4(temp=scalarData*twoDeltaX+(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1))*vv(0), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*vv(0), , )
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   temp=scalarData*twoDeltaX+(u(i1+ip1,i2+ip2,i3+ip3,
     & n1)-u(i1+im1,i2+im2,i3+im3,n1))*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)


                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   temp=scalarData*twoDeltaX+(u(i1+ip1,i2+ip2,i3+ip3,
     & n1)-u(i1+im1,i2+im2,i3+im3,n1))*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)


               end do
               end do
               end do
             end if
             end if
           else if( bcOption.eq.gfForcing ) then
! normalDerivativeRectangular(gfData(i1,i2,i3,m1),gfData(i1,i2,i3,m2),gfData(i1,i2,i3,m3))
             if( nd.eq.2 ) then
! loopsd4(temp=(u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1)+gfData(i1,i2,i3,m1)*twoDeltaX)*vv(0) +(u(i1+ip1,i2+ip2,i3,n2)-u(i1+im1,i2+im2,i3,n2)+gfData(i1,i2,i3,m2)*twoDeltaX)*vv(1), u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*vv(0), u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*vv(1), )
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   temp=(u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1)+
     & gfData(i1,i2,i3,m1)*twoDeltaX)*vv(0)+(u(i1+ip1,i2+ip2,i3,n2)-u(
     & i1+im1,i2+im2,i3,n2)+gfData(i1,i2,i3,m2)*twoDeltaX)*vv(1)
                   u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & vv(0)
                   u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & vv(1)

                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   temp=(u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1)+
     & gfData(i1,i2,i3,m1)*twoDeltaX)*vv(0)+(u(i1+ip1,i2+ip2,i3,n2)-u(
     & i1+im1,i2+im2,i3,n2)+gfData(i1,i2,i3,m2)*twoDeltaX)*vv(1)
                   u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & vv(0)
                   u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & vv(1)

               end do
               end do
               end do
             end if
             else if( nd.eq.3 ) then
! loopsd4(temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1)+gfData(i1,i2,i3,m1)*twoDeltaX)*vv(0) +(u(i1+ip1,i2+ip2,i3+ip3,n2)-u(i1+im1,i2+im2,i3+im3,n2)+gfData(i1,i2,i3,m2)*twoDeltaX)*vv(1) +(u(i1+ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+im2,i3+im3,n3)+gfData(i1,i2,i3,m3)*twoDeltaX)*vv(2),u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*vv(0), u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*vv(1), u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*vv(2))
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+
     & im3,n1)+gfData(i1,i2,i3,m1)*twoDeltaX)*vv(0)+(u(i1+ip1,i2+ip2,
     & i3+ip3,n2)-u(i1+im1,i2+im2,i3+im3,n2)+gfData(i1,i2,i3,m2)*
     & twoDeltaX)*vv(1)+(u(i1+ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+im2,
     & i3+im3,n3)+gfData(i1,i2,i3,m3)*twoDeltaX)*vv(2)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*vv(1)
                   u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*vv(2)
                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+
     & im3,n1)+gfData(i1,i2,i3,m1)*twoDeltaX)*vv(0)+(u(i1+ip1,i2+ip2,
     & i3+ip3,n2)-u(i1+im1,i2+im2,i3+im3,n2)+gfData(i1,i2,i3,m2)*
     & twoDeltaX)*vv(1)+(u(i1+ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+im2,
     & i3+im3,n3)+gfData(i1,i2,i3,m3)*twoDeltaX)*vv(2)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*vv(1)
                   u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*vv(2)
               end do
               end do
               end do
             end if
             else
! loopsd4(temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1)+gfData(i1,i2,i3,m1)*twoDeltaX)*vv(0), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*vv(0), , )
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+
     & im3,n1)+gfData(i1,i2,i3,m1)*twoDeltaX)*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)


                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+
     & im3,n1)+gfData(i1,i2,i3,m1)*twoDeltaX)*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)


               end do
               end do
               end do
             end if
             end if
           else if( bcOption.eq.arrayForcing ) then
! normalDerivativeRectangular(fData(m1,side,axis,grid),fData(m2,side,axis,grid),fData(m3,side,axis,grid))
             if( nd.eq.2 ) then
! loopsd4(temp=(u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1)+fData(m1,side,axis,grid)*twoDeltaX)*vv(0) +(u(i1+ip1,i2+ip2,i3,n2)-u(i1+im1,i2+im2,i3,n2)+fData(m2,side,axis,grid)*twoDeltaX)*vv(1), u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*vv(0), u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*vv(1), )
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   temp=(u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1)+
     & fData(m1,side,axis,grid)*twoDeltaX)*vv(0)+(u(i1+ip1,i2+ip2,i3,
     & n2)-u(i1+im1,i2+im2,i3,n2)+fData(m2,side,axis,grid)*twoDeltaX)*
     & vv(1)
                   u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & vv(0)
                   u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & vv(1)

                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   temp=(u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1)+
     & fData(m1,side,axis,grid)*twoDeltaX)*vv(0)+(u(i1+ip1,i2+ip2,i3,
     & n2)-u(i1+im1,i2+im2,i3,n2)+fData(m2,side,axis,grid)*twoDeltaX)*
     & vv(1)
                   u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & vv(0)
                   u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & vv(1)

               end do
               end do
               end do
             end if
             else if( nd.eq.3 ) then
! loopsd4(temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1)+fData(m1,side,axis,grid)*twoDeltaX)*vv(0) +(u(i1+ip1,i2+ip2,i3+ip3,n2)-u(i1+im1,i2+im2,i3+im3,n2)+fData(m2,side,axis,grid)*twoDeltaX)*vv(1) +(u(i1+ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+im2,i3+im3,n3)+fData(m3,side,axis,grid)*twoDeltaX)*vv(2),u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*vv(0), u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*vv(1), u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*vv(2))
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+
     & im3,n1)+fData(m1,side,axis,grid)*twoDeltaX)*vv(0)+(u(i1+ip1,i2+
     & ip2,i3+ip3,n2)-u(i1+im1,i2+im2,i3+im3,n2)+fData(m2,side,axis,
     & grid)*twoDeltaX)*vv(1)+(u(i1+ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+
     & im2,i3+im3,n3)+fData(m3,side,axis,grid)*twoDeltaX)*vv(2)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*vv(1)
                   u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*vv(2)
                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+
     & im3,n1)+fData(m1,side,axis,grid)*twoDeltaX)*vv(0)+(u(i1+ip1,i2+
     & ip2,i3+ip3,n2)-u(i1+im1,i2+im2,i3+im3,n2)+fData(m2,side,axis,
     & grid)*twoDeltaX)*vv(1)+(u(i1+ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+
     & im2,i3+im3,n3)+fData(m3,side,axis,grid)*twoDeltaX)*vv(2)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*vv(1)
                   u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*vv(2)
               end do
               end do
               end do
             end if
             else
! loopsd4(temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1)+fData(m1,side,axis,grid)*twoDeltaX)*vv(0), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*vv(0), , )
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+
     & im3,n1)+fData(m1,side,axis,grid)*twoDeltaX)*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)


                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+
     & im3,n1)+fData(m1,side,axis,grid)*twoDeltaX)*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)


               end do
               end do
               end do
             end if
             end if
           else if( bcOption.eq.vectorForcing ) then
! normalDerivativeRectangular(vData(m1),vData(m2),vData(m3))
             if( nd.eq.2 ) then
! loopsd4(temp=(u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1)+vData(m1)*twoDeltaX)*vv(0) +(u(i1+ip1,i2+ip2,i3,n2)-u(i1+im1,i2+im2,i3,n2)+vData(m2)*twoDeltaX)*vv(1), u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*vv(0), u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*vv(1), )
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   temp=(u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1)+
     & vData(m1)*twoDeltaX)*vv(0)+(u(i1+ip1,i2+ip2,i3,n2)-u(i1+im1,i2+
     & im2,i3,n2)+vData(m2)*twoDeltaX)*vv(1)
                   u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & vv(0)
                   u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & vv(1)

                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   temp=(u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1)+
     & vData(m1)*twoDeltaX)*vv(0)+(u(i1+ip1,i2+ip2,i3,n2)-u(i1+im1,i2+
     & im2,i3,n2)+vData(m2)*twoDeltaX)*vv(1)
                   u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & vv(0)
                   u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & vv(1)

               end do
               end do
               end do
             end if
             else if( nd.eq.3 ) then
! loopsd4(temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1)+vData(m1)*twoDeltaX)*vv(0) +(u(i1+ip1,i2+ip2,i3+ip3,n2)-u(i1+im1,i2+im2,i3+im3,n2)+vData(m2)*twoDeltaX)*vv(1) +(u(i1+ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+im2,i3+im3,n3)+vData(m3)*twoDeltaX)*vv(2),u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*vv(0), u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*vv(1), u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*vv(2))
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+
     & im3,n1)+vData(m1)*twoDeltaX)*vv(0)+(u(i1+ip1,i2+ip2,i3+ip3,n2)-
     & u(i1+im1,i2+im2,i3+im3,n2)+vData(m2)*twoDeltaX)*vv(1)+(u(i1+
     & ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+im2,i3+im3,n3)+vData(m3)*
     & twoDeltaX)*vv(2)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*vv(1)
                   u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*vv(2)
                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+
     & im3,n1)+vData(m1)*twoDeltaX)*vv(0)+(u(i1+ip1,i2+ip2,i3+ip3,n2)-
     & u(i1+im1,i2+im2,i3+im3,n2)+vData(m2)*twoDeltaX)*vv(1)+(u(i1+
     & ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+im2,i3+im3,n3)+vData(m3)*
     & twoDeltaX)*vv(2)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*vv(1)
                   u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*vv(2)
               end do
               end do
               end do
             end if
             else
! loopsd4(temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1)+vData(m1)*twoDeltaX)*vv(0), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*vv(0), , )
             if( useWhereMask.ne.0 )then
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                 if( mask(i1,i2,i3).ne.0 )then
                   temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+
     & im3,n1)+vData(m1)*twoDeltaX)*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)


                 end if
               end do
               end do
               end do
             else
               do i3=n3a,n3b
               do i2=n2a,n2b
               do i1=n1a,n1b
                   temp=(u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+
     & im3,n1)+vData(m1)*twoDeltaX)*vv(0)
                   u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*vv(0)


               end do
               end do
               end do
             end if
             end if
           end if
         else
       !            ******* curvilinear normal derivative ******
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
          else if( nd.eq.3 )then
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
          else
            m12=0
            m22=1
            m32=2
            mGhost=m22+2*side-1
          end if
          if( bcOption.eq.scalarForcing ) then
! normalDerivativeCurvilinear(scalarData)
            if( nd.eq.2 ) then
! loopsd4(temp=((scalarData) -(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3,n2)) +coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3,n2)) +coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3,n2)) +coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3,n2)) +coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3,n2)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*v(i1,i2,i3,v1), )
            if( useWhereMask.ne.0 )then
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  temp=((scalarData)-(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2-1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2))+coeff(
     & m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)+v(i1,i2,i3,v1)*
     & u(i1-1,i2,i3,n2))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,
     & i3,n1)+v(i1,i2,i3,v1)*u(i1,i2,i3,n2))+coeff(m32,i1,i2,i3)*(v(
     & i1,i2,i3,v0)*u(i1+1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2))+
     & coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)+v(i1,i2,
     & i3,v1)*u(i1,i2+1,i3,n2))))/coeff(mGhost,i1,i2,i3)
                  u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*v(
     & i1,i2,i3,v0)
                  u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*v(
     & i1,i2,i3,v1)

                end if
              end do
              end do
              end do
            else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  temp=((scalarData)-(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2-1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2))+coeff(
     & m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)+v(i1,i2,i3,v1)*
     & u(i1-1,i2,i3,n2))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,
     & i3,n1)+v(i1,i2,i3,v1)*u(i1,i2,i3,n2))+coeff(m32,i1,i2,i3)*(v(
     & i1,i2,i3,v0)*u(i1+1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2))+
     & coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)+v(i1,i2,
     & i3,v1)*u(i1,i2+1,i3,n2))))/coeff(mGhost,i1,i2,i3)
                  u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*v(
     & i1,i2,i3,v0)
                  u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*v(
     & i1,i2,i3,v1)

              end do
              end do
              end do
            end if
            else if( nd.eq.3 ) then
! loopsd4(temp=( (scalarData) -(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3-1,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3-1,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3-1,n3)) +coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2-1,i3  ,n3)) +coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1-1,i2  ,i3  ,n3)) +coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3  ,n3)) +coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1+1,i2  ,i3  ,n3)) +coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2+1,i3  ,n3)) +coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3+1,n1)      +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3+1,n2)      +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3+1,n3)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*v(i1,i2,i3,v1), u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*v(i1,i2,i3,v2))
            if( useWhereMask.ne.0 )then
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  temp=((scalarData)-(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2,i3-1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3-1,n2)  +v(i1,
     & i2,i3,v2)*u(i1,i2,i3-1,n3))+coeff(m212,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2-1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1,i2-1,i3,n3))+coeff(m122,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1-1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1-1,i2,i3,n3))+coeff(m222,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2,i3,n2)  +v(i1,i2,
     & i3,v2)*u(i1,i2,i3,n3))+coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1+1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2)  +v(i1,i2,i3,
     & v2)*u(i1+1,i2,i3,n3))+coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2+1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2)  +v(i1,i2,i3,
     & v2)*u(i1,i2+1,i3,n3))+coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2,i3+1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3+1,n2)   +v(i1,i2,
     & i3,v2)*u(i1,i2,i3+1,n3))))/coeff(mGhost,i1,i2,i3)
                  u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)
     & +temp*v(i1,i2,i3,v0)
                  u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)
     & +temp*v(i1,i2,i3,v1)
                  u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)
     & +temp*v(i1,i2,i3,v2)
                end if
              end do
              end do
              end do
            else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  temp=((scalarData)-(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2,i3-1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3-1,n2)  +v(i1,
     & i2,i3,v2)*u(i1,i2,i3-1,n3))+coeff(m212,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2-1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1,i2-1,i3,n3))+coeff(m122,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1-1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1-1,i2,i3,n3))+coeff(m222,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2,i3,n2)  +v(i1,i2,
     & i3,v2)*u(i1,i2,i3,n3))+coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1+1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2)  +v(i1,i2,i3,
     & v2)*u(i1+1,i2,i3,n3))+coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2+1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2)  +v(i1,i2,i3,
     & v2)*u(i1,i2+1,i3,n3))+coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2,i3+1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3+1,n2)   +v(i1,i2,
     & i3,v2)*u(i1,i2,i3+1,n3))))/coeff(mGhost,i1,i2,i3)
                  u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)
     & +temp*v(i1,i2,i3,v0)
                  u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)
     & +temp*v(i1,i2,i3,v1)
                  u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)
     & +temp*v(i1,i2,i3,v2)
              end do
              end do
              end do
            end if
            else
! loopsd4(temp=( (scalarData) - (coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), , )
            if( useWhereMask.ne.0 )then
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                if( mask(i1,i2,i3).ne.0 )then
                  temp=((scalarData)-(coeff(m12,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1-1,i2,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,
     & i2,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1)
     & )))/coeff(mGhost,i1,i2,i3)
                  u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)
     & +temp*v(i1,i2,i3,v0)


                end if
              end do
              end do
              end do
            else
              do i3=n3a,n3b
              do i2=n2a,n2b
              do i1=n1a,n1b
                  temp=((scalarData)-(coeff(m12,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1-1,i2,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,
     & i2,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1)
     & )))/coeff(mGhost,i1,i2,i3)
                  u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)
     & +temp*v(i1,i2,i3,v0)


              end do
              end do
              end do
            end if
            end if
          else if( bcOption.eq.gfForcing ) then
            if( nd.eq.2 ) then
! normalDerivativeCurvilinear(gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(i1,i2,i3,m2)*v(i1,i2,i3,v1))
              if( nd.eq.2 ) then
! loopsd4(temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(i1,i2,i3,m2)*v(i1,i2,i3,v1)) -(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3,n2)) +coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3,n2)) +coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3,n2)) +coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3,n2)) +coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3,n2)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*v(i1,i2,i3,v1), )
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(
     & i1,i2,i3,m2)*v(i1,i2,i3,v1))-(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2-1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2))+coeff(
     & m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)+v(i1,i2,i3,v1)*
     & u(i1-1,i2,i3,n2))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,
     & i3,n1)+v(i1,i2,i3,v1)*u(i1,i2,i3,n2))+coeff(m32,i1,i2,i3)*(v(
     & i1,i2,i3,v0)*u(i1+1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2))+
     & coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)+v(i1,i2,
     & i3,v1)*u(i1,i2+1,i3,n2))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & v(i1,i2,i3,v1)

                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(
     & i1,i2,i3,m2)*v(i1,i2,i3,v1))-(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2-1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2))+coeff(
     & m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)+v(i1,i2,i3,v1)*
     & u(i1-1,i2,i3,n2))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,
     & i3,n1)+v(i1,i2,i3,v1)*u(i1,i2,i3,n2))+coeff(m32,i1,i2,i3)*(v(
     & i1,i2,i3,v0)*u(i1+1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2))+
     & coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)+v(i1,i2,
     & i3,v1)*u(i1,i2+1,i3,n2))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & v(i1,i2,i3,v1)

                end do
                end do
                end do
              end if
              else if( nd.eq.3 ) then
! loopsd4(temp=( (gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(i1,i2,i3,m2)*v(i1,i2,i3,v1)) -(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3-1,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3-1,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3-1,n3)) +coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2-1,i3  ,n3)) +coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1-1,i2  ,i3  ,n3)) +coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3  ,n3)) +coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1+1,i2  ,i3  ,n3)) +coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2+1,i3  ,n3)) +coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3+1,n1)      +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3+1,n2)      +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3+1,n3)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*v(i1,i2,i3,v1), u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*v(i1,i2,i3,v2))
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(
     & i1,i2,i3,m2)*v(i1,i2,i3,v1))-(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2,i3-1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3-1,n2)  +v(i1,
     & i2,i3,v2)*u(i1,i2,i3-1,n3))+coeff(m212,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2-1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1,i2-1,i3,n3))+coeff(m122,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1-1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1-1,i2,i3,n3))+coeff(m222,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2,i3,n2)  +v(i1,i2,
     & i3,v2)*u(i1,i2,i3,n3))+coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1+1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2)  +v(i1,i2,i3,
     & v2)*u(i1+1,i2,i3,n3))+coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2+1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2)  +v(i1,i2,i3,
     & v2)*u(i1,i2+1,i3,n3))+coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2,i3+1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3+1,n2)   +v(i1,i2,
     & i3,v2)*u(i1,i2,i3+1,n3))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*v(i1,i2,i3,v1)
                    u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*v(i1,i2,i3,v2)
                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(
     & i1,i2,i3,m2)*v(i1,i2,i3,v1))-(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2,i3-1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3-1,n2)  +v(i1,
     & i2,i3,v2)*u(i1,i2,i3-1,n3))+coeff(m212,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2-1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1,i2-1,i3,n3))+coeff(m122,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1-1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1-1,i2,i3,n3))+coeff(m222,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2,i3,n2)  +v(i1,i2,
     & i3,v2)*u(i1,i2,i3,n3))+coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1+1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2)  +v(i1,i2,i3,
     & v2)*u(i1+1,i2,i3,n3))+coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2+1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2)  +v(i1,i2,i3,
     & v2)*u(i1,i2+1,i3,n3))+coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2,i3+1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3+1,n2)   +v(i1,i2,
     & i3,v2)*u(i1,i2,i3+1,n3))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*v(i1,i2,i3,v1)
                    u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*v(i1,i2,i3,v2)
                end do
                end do
                end do
              end if
              else
! loopsd4(temp=( (gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(i1,i2,i3,m2)*v(i1,i2,i3,v1)) - (coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), , )
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(
     & i1,i2,i3,m2)*v(i1,i2,i3,v1))-(coeff(m12,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1-1,i2,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,
     & i2,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1)
     & )))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)


                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(
     & i1,i2,i3,m2)*v(i1,i2,i3,v1))-(coeff(m12,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1-1,i2,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,
     & i2,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1)
     & )))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)


                end do
                end do
                end do
              end if
              end if
            else if( nd.eq.3 ) then
! normalDerivativeCurvilinear(gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(i1,i2,i3,m2)*v(i1,i2,i3,v1)+gfData(i1,i2,i3,m3)*v(i1,i2,i3,v2))
              if( nd.eq.2 ) then
! loopsd4(temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(i1,i2,i3,m2)*v(i1,i2,i3,v1)+gfData(i1,i2,i3,m3)*v(i1,i2,i3,v2)) -(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3,n2)) +coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3,n2)) +coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3,n2)) +coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3,n2)) +coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3,n2)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*v(i1,i2,i3,v1), )
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(
     & i1,i2,i3,m2)*v(i1,i2,i3,v1)+gfData(i1,i2,i3,m3)*v(i1,i2,i3,v2))
     & -(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2-1,i3,n1)+v(i1,i2,
     & i3,v1)*u(i1,i2-1,i3,n2))+coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1-1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2))+coeff(m22,i1,
     & i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2,
     & i3,n2))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1)+v(
     & i1,i2,i3,v1)*u(i1+1,i2,i3,n2))+coeff(m23,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2+1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2))))/coeff(
     & mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & v(i1,i2,i3,v1)

                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(
     & i1,i2,i3,m2)*v(i1,i2,i3,v1)+gfData(i1,i2,i3,m3)*v(i1,i2,i3,v2))
     & -(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2-1,i3,n1)+v(i1,i2,
     & i3,v1)*u(i1,i2-1,i3,n2))+coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1-1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2))+coeff(m22,i1,
     & i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2,
     & i3,n2))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1)+v(
     & i1,i2,i3,v1)*u(i1+1,i2,i3,n2))+coeff(m23,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2+1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2))))/coeff(
     & mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & v(i1,i2,i3,v1)

                end do
                end do
                end do
              end if
              else if( nd.eq.3 ) then
! loopsd4(temp=( (gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(i1,i2,i3,m2)*v(i1,i2,i3,v1)+gfData(i1,i2,i3,m3)*v(i1,i2,i3,v2)) -(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3-1,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3-1,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3-1,n3)) +coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2-1,i3  ,n3)) +coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1-1,i2  ,i3  ,n3)) +coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3  ,n3)) +coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1+1,i2  ,i3  ,n3)) +coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2+1,i3  ,n3)) +coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3+1,n1)      +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3+1,n2)      +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3+1,n3)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*v(i1,i2,i3,v1), u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*v(i1,i2,i3,v2))
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(
     & i1,i2,i3,m2)*v(i1,i2,i3,v1)+gfData(i1,i2,i3,m3)*v(i1,i2,i3,v2))
     & -(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3-1,n1)   +v(
     & i1,i2,i3,v1)*u(i1,i2,i3-1,n2)  +v(i1,i2,i3,v2)*u(i1,i2,i3-1,n3)
     & )+coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2-1,i3,n1)  +v(i1,
     & i2,i3,v1)*u(i1,i2-1,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2-1,i3,n3))+
     & coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)  +v(i1,
     & i2,i3,v1)*u(i1-1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1-1,i2,i3,n3))+
     & coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1)  +v(i1,i2,
     & i3,v1)*u(i1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2,i3,n3))+coeff(
     & m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1)  +v(i1,i2,i3,
     & v1)*u(i1+1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1+1,i2,i3,n3))+coeff(
     & m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)  +v(i1,i2,i3,
     & v1)*u(i1,i2+1,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2+1,i3,n3))+coeff(
     & m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3+1,n1)   +v(i1,i2,i3,
     & v1)*u(i1,i2,i3+1,n2)   +v(i1,i2,i3,v2)*u(i1,i2,i3+1,n3))))
     & /coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*v(i1,i2,i3,v1)
                    u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*v(i1,i2,i3,v2)
                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(
     & i1,i2,i3,m2)*v(i1,i2,i3,v1)+gfData(i1,i2,i3,m3)*v(i1,i2,i3,v2))
     & -(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3-1,n1)   +v(
     & i1,i2,i3,v1)*u(i1,i2,i3-1,n2)  +v(i1,i2,i3,v2)*u(i1,i2,i3-1,n3)
     & )+coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2-1,i3,n1)  +v(i1,
     & i2,i3,v1)*u(i1,i2-1,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2-1,i3,n3))+
     & coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)  +v(i1,
     & i2,i3,v1)*u(i1-1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1-1,i2,i3,n3))+
     & coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1)  +v(i1,i2,
     & i3,v1)*u(i1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2,i3,n3))+coeff(
     & m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1)  +v(i1,i2,i3,
     & v1)*u(i1+1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1+1,i2,i3,n3))+coeff(
     & m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)  +v(i1,i2,i3,
     & v1)*u(i1,i2+1,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2+1,i3,n3))+coeff(
     & m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3+1,n1)   +v(i1,i2,i3,
     & v1)*u(i1,i2,i3+1,n2)   +v(i1,i2,i3,v2)*u(i1,i2,i3+1,n3))))
     & /coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*v(i1,i2,i3,v1)
                    u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*v(i1,i2,i3,v2)
                end do
                end do
                end do
              end if
              else
! loopsd4(temp=( (gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(i1,i2,i3,m2)*v(i1,i2,i3,v1)+gfData(i1,i2,i3,m3)*v(i1,i2,i3,v2)) - (coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), , )
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(
     & i1,i2,i3,m2)*v(i1,i2,i3,v1)+gfData(i1,i2,i3,m3)*v(i1,i2,i3,v2))
     & -(coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1))+coeff(
     & m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1))+coeff(m32,i1,i2,
     & i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)


                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(
     & i1,i2,i3,m2)*v(i1,i2,i3,v1)+gfData(i1,i2,i3,m3)*v(i1,i2,i3,v2))
     & -(coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1))+coeff(
     & m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1))+coeff(m32,i1,i2,
     & i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)


                end do
                end do
                end do
              end if
              end if
            else
              stop 12345
            end if
          else if( bcOption.eq.arrayForcing ) then
            if( nd.eq.2 ) then
! normalDerivativeCurvilinear(fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+fData(m2,side,axis,grid)*v(i1,i2,i3,v1))
              if( nd.eq.2 ) then
! loopsd4(temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+fData(m2,side,axis,grid)*v(i1,i2,i3,v1)) -(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3,n2)) +coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3,n2)) +coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3,n2)) +coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3,n2)) +coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3,n2)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*v(i1,i2,i3,v1), )
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+
     & fData(m2,side,axis,grid)*v(i1,i2,i3,v1))-(coeff(m21,i1,i2,i3)*(
     & v(i1,i2,i3,v0)*u(i1,i2-1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2)
     & )+coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)+v(i1,i2,
     & i3,v1)*u(i1-1,i2,i3,n2))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2,i3,n2))+coeff(m32,i1,i2,i3)
     & *(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1+1,i2,i3,
     & n2))+coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)+v(i1,
     & i2,i3,v1)*u(i1,i2+1,i3,n2))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & v(i1,i2,i3,v1)

                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+
     & fData(m2,side,axis,grid)*v(i1,i2,i3,v1))-(coeff(m21,i1,i2,i3)*(
     & v(i1,i2,i3,v0)*u(i1,i2-1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2)
     & )+coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)+v(i1,i2,
     & i3,v1)*u(i1-1,i2,i3,n2))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2,i3,n2))+coeff(m32,i1,i2,i3)
     & *(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1+1,i2,i3,
     & n2))+coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)+v(i1,
     & i2,i3,v1)*u(i1,i2+1,i3,n2))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & v(i1,i2,i3,v1)

                end do
                end do
                end do
              end if
              else if( nd.eq.3 ) then
! loopsd4(temp=( (fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+fData(m2,side,axis,grid)*v(i1,i2,i3,v1)) -(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3-1,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3-1,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3-1,n3)) +coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2-1,i3  ,n3)) +coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1-1,i2  ,i3  ,n3)) +coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3  ,n3)) +coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1+1,i2  ,i3  ,n3)) +coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2+1,i3  ,n3)) +coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3+1,n1)      +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3+1,n2)      +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3+1,n3)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*v(i1,i2,i3,v1), u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*v(i1,i2,i3,v2))
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+
     & fData(m2,side,axis,grid)*v(i1,i2,i3,v1))-(coeff(m221,i1,i2,i3)*
     & (v(i1,i2,i3,v0)*u(i1,i2,i3-1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3-
     & 1,n2)  +v(i1,i2,i3,v2)*u(i1,i2,i3-1,n3))+coeff(m212,i1,i2,i3)*(
     & v(i1,i2,i3,v0)*u(i1,i2-1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2-1,i3,
     & n2)  +v(i1,i2,i3,v2)*u(i1,i2-1,i3,n3))+coeff(m122,i1,i2,i3)*(v(
     & i1,i2,i3,v0)*u(i1-1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2)
     &   +v(i1,i2,i3,v2)*u(i1-1,i2,i3,n3))+coeff(m222,i1,i2,i3)*(v(i1,
     & i2,i3,v0)*u(i1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2,i3,n2)  +v(
     & i1,i2,i3,v2)*u(i1,i2,i3,n3))+coeff(m322,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1+1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1+1,i2,i3,n3))+coeff(m232,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2+1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1,i2+1,i3,n3))+coeff(m223,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2,i3+1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3+1,n2)   +v(
     & i1,i2,i3,v2)*u(i1,i2,i3+1,n3))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*v(i1,i2,i3,v1)
                    u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*v(i1,i2,i3,v2)
                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+
     & fData(m2,side,axis,grid)*v(i1,i2,i3,v1))-(coeff(m221,i1,i2,i3)*
     & (v(i1,i2,i3,v0)*u(i1,i2,i3-1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3-
     & 1,n2)  +v(i1,i2,i3,v2)*u(i1,i2,i3-1,n3))+coeff(m212,i1,i2,i3)*(
     & v(i1,i2,i3,v0)*u(i1,i2-1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2-1,i3,
     & n2)  +v(i1,i2,i3,v2)*u(i1,i2-1,i3,n3))+coeff(m122,i1,i2,i3)*(v(
     & i1,i2,i3,v0)*u(i1-1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2)
     &   +v(i1,i2,i3,v2)*u(i1-1,i2,i3,n3))+coeff(m222,i1,i2,i3)*(v(i1,
     & i2,i3,v0)*u(i1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2,i3,n2)  +v(
     & i1,i2,i3,v2)*u(i1,i2,i3,n3))+coeff(m322,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1+1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1+1,i2,i3,n3))+coeff(m232,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2+1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1,i2+1,i3,n3))+coeff(m223,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1,i2,i3+1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3+1,n2)   +v(
     & i1,i2,i3,v2)*u(i1,i2,i3+1,n3))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*v(i1,i2,i3,v1)
                    u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*v(i1,i2,i3,v2)
                end do
                end do
                end do
              end if
              else
! loopsd4(temp=( (fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+fData(m2,side,axis,grid)*v(i1,i2,i3,v1)) - (coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), , )
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+
     & fData(m2,side,axis,grid)*v(i1,i2,i3,v1))-(coeff(m12,i1,i2,i3)*(
     & v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,
     & i3,v0)*u(i1,i2,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1+1,i2,i3,n1))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)


                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+
     & fData(m2,side,axis,grid)*v(i1,i2,i3,v1))-(coeff(m12,i1,i2,i3)*(
     & v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,
     & i3,v0)*u(i1,i2,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1+1,i2,i3,n1))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)


                end do
                end do
                end do
              end if
              end if
           else if( nd.eq.3 ) then
! normalDerivativeCurvilinear(fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+fData(m2,side,axis,grid)*v(i1,i2,i3,v1)+fData(m3,side,axis,grid)*v(i1,i2,i3,v2))
              if( nd.eq.2 ) then
! loopsd4(temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+fData(m2,side,axis,grid)*v(i1,i2,i3,v1)+fData(m3,side,axis,grid)*v(i1,i2,i3,v2)) -(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3,n2)) +coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3,n2)) +coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3,n2)) +coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3,n2)) +coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3,n2)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*v(i1,i2,i3,v1), )
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+
     & fData(m2,side,axis,grid)*v(i1,i2,i3,v1)+fData(m3,side,axis,
     & grid)*v(i1,i2,i3,v2))-(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2-1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2))+coeff(m12,i1,
     & i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1-1,
     & i2,i3,n2))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1)+
     & v(i1,i2,i3,v1)*u(i1,i2,i3,n2))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1+1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2))+coeff(
     & m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)+v(i1,i2,i3,v1)*
     & u(i1,i2+1,i3,n2))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & v(i1,i2,i3,v1)

                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+
     & fData(m2,side,axis,grid)*v(i1,i2,i3,v1)+fData(m3,side,axis,
     & grid)*v(i1,i2,i3,v2))-(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2-1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2))+coeff(m12,i1,
     & i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1-1,
     & i2,i3,n2))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1)+
     & v(i1,i2,i3,v1)*u(i1,i2,i3,n2))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,
     & v0)*u(i1+1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2))+coeff(
     & m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)+v(i1,i2,i3,v1)*
     & u(i1,i2+1,i3,n2))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & v(i1,i2,i3,v1)

                end do
                end do
                end do
              end if
              else if( nd.eq.3 ) then
! loopsd4(temp=( (fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+fData(m2,side,axis,grid)*v(i1,i2,i3,v1)+fData(m3,side,axis,grid)*v(i1,i2,i3,v2)) -(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3-1,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3-1,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3-1,n3)) +coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2-1,i3  ,n3)) +coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1-1,i2  ,i3  ,n3)) +coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3  ,n3)) +coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1+1,i2  ,i3  ,n3)) +coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2+1,i3  ,n3)) +coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3+1,n1)      +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3+1,n2)      +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3+1,n3)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*v(i1,i2,i3,v1), u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*v(i1,i2,i3,v2))
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+
     & fData(m2,side,axis,grid)*v(i1,i2,i3,v1)+fData(m3,side,axis,
     & grid)*v(i1,i2,i3,v2))-(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2,i3-1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3-1,n2)  +v(i1,i2,i3,
     & v2)*u(i1,i2,i3-1,n3))+coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2-1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2)  +v(i1,i2,i3,
     & v2)*u(i1,i2-1,i3,n3))+coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1-1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2)  +v(i1,i2,i3,
     & v2)*u(i1-1,i2,i3,n3))+coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2,i3,n2)  +v(i1,i2,i3,v2)*
     & u(i1,i2,i3,n3))+coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,
     & i3,n1)  +v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1+
     & 1,i2,i3,n3))+coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,
     & n1)  +v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2+
     & 1,i3,n3))+coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3+1,n1)
     &    +v(i1,i2,i3,v1)*u(i1,i2,i3+1,n2)   +v(i1,i2,i3,v2)*u(i1,i2,
     & i3+1,n3))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*v(i1,i2,i3,v1)
                    u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*v(i1,i2,i3,v2)
                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+
     & fData(m2,side,axis,grid)*v(i1,i2,i3,v1)+fData(m3,side,axis,
     & grid)*v(i1,i2,i3,v2))-(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2,i3-1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3-1,n2)  +v(i1,i2,i3,
     & v2)*u(i1,i2,i3-1,n3))+coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2-1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2)  +v(i1,i2,i3,
     & v2)*u(i1,i2-1,i3,n3))+coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1-1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2)  +v(i1,i2,i3,
     & v2)*u(i1-1,i2,i3,n3))+coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2,i3,n2)  +v(i1,i2,i3,v2)*
     & u(i1,i2,i3,n3))+coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,
     & i3,n1)  +v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1+
     & 1,i2,i3,n3))+coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,
     & n1)  +v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2+
     & 1,i3,n3))+coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3+1,n1)
     &    +v(i1,i2,i3,v1)*u(i1,i2,i3+1,n2)   +v(i1,i2,i3,v2)*u(i1,i2,
     & i3+1,n3))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*v(i1,i2,i3,v1)
                    u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*v(i1,i2,i3,v2)
                end do
                end do
                end do
              end if
              else
! loopsd4(temp=( (fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+fData(m2,side,axis,grid)*v(i1,i2,i3,v1)+fData(m3,side,axis,grid)*v(i1,i2,i3,v2)) - (coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), , )
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+
     & fData(m2,side,axis,grid)*v(i1,i2,i3,v1)+fData(m3,side,axis,
     & grid)*v(i1,i2,i3,v2))-(coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1-1,i2,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,
     & n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1))))
     & /coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)


                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+
     & fData(m2,side,axis,grid)*v(i1,i2,i3,v1)+fData(m3,side,axis,
     & grid)*v(i1,i2,i3,v2))-(coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1-1,i2,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,
     & n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1))))
     & /coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)


                end do
                end do
                end do
              end if
              end if
            else
              stop 12345
           end if
          else if( bcOption.eq.vectorForcing ) then
            if( nd.eq.2 ) then
! normalDerivativeCurvilinear(vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,i3,v1))
              if( nd.eq.2 ) then
! loopsd4(temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,i3,v1)) -(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3,n2)) +coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3,n2)) +coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3,n2)) +coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3,n2)) +coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3,n2)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*v(i1,i2,i3,v1), )
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,
     & i3,v1))-(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2-1,i3,n1)+
     & v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2))+coeff(m12,i1,i2,i3)*(v(i1,i2,
     & i3,v0)*u(i1-1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2))+coeff(
     & m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1)+v(i1,i2,i3,v1)*u(
     & i1,i2,i3,n2))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,
     & n1)+v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2))+coeff(m23,i1,i2,i3)*(v(i1,
     & i2,i3,v0)*u(i1,i2+1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2))))
     & /coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & v(i1,i2,i3,v1)

                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,
     & i3,v1))-(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2-1,i3,n1)+
     & v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2))+coeff(m12,i1,i2,i3)*(v(i1,i2,
     & i3,v0)*u(i1-1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2))+coeff(
     & m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1)+v(i1,i2,i3,v1)*u(
     & i1,i2,i3,n2))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,
     & n1)+v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2))+coeff(m23,i1,i2,i3)*(v(i1,
     & i2,i3,v0)*u(i1,i2+1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2))))
     & /coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & v(i1,i2,i3,v1)

                end do
                end do
                end do
              end if
              else if( nd.eq.3 ) then
! loopsd4(temp=( (vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,i3,v1)) -(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3-1,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3-1,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3-1,n3)) +coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2-1,i3  ,n3)) +coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1-1,i2  ,i3  ,n3)) +coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3  ,n3)) +coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1+1,i2  ,i3  ,n3)) +coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2+1,i3  ,n3)) +coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3+1,n1)      +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3+1,n2)      +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3+1,n3)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*v(i1,i2,i3,v1), u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*v(i1,i2,i3,v2))
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,
     & i3,v1))-(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3-1,n1) 
     &   +v(i1,i2,i3,v1)*u(i1,i2,i3-1,n2)  +v(i1,i2,i3,v2)*u(i1,i2,i3-
     & 1,n3))+coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2-1,i3,n1)  +
     & v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2-1,i3,
     & n3))+coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)  +v(
     & i1,i2,i3,v1)*u(i1-1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1-1,i2,i3,n3)
     & )+coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1)  +v(i1,
     & i2,i3,v1)*u(i1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2,i3,n3))+
     & coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1)  +v(i1,
     & i2,i3,v1)*u(i1+1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1+1,i2,i3,n3))+
     & coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)  +v(i1,
     & i2,i3,v1)*u(i1,i2+1,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2+1,i3,n3))+
     & coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3+1,n1)   +v(i1,
     & i2,i3,v1)*u(i1,i2,i3+1,n2)   +v(i1,i2,i3,v2)*u(i1,i2,i3+1,n3)))
     & )/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*v(i1,i2,i3,v1)
                    u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*v(i1,i2,i3,v2)
                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,
     & i3,v1))-(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3-1,n1) 
     &   +v(i1,i2,i3,v1)*u(i1,i2,i3-1,n2)  +v(i1,i2,i3,v2)*u(i1,i2,i3-
     & 1,n3))+coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2-1,i3,n1)  +
     & v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2-1,i3,
     & n3))+coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)  +v(
     & i1,i2,i3,v1)*u(i1-1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1-1,i2,i3,n3)
     & )+coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1)  +v(i1,
     & i2,i3,v1)*u(i1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2,i3,n3))+
     & coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1)  +v(i1,
     & i2,i3,v1)*u(i1+1,i2,i3,n2)  +v(i1,i2,i3,v2)*u(i1+1,i2,i3,n3))+
     & coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)  +v(i1,
     & i2,i3,v1)*u(i1,i2+1,i3,n2)  +v(i1,i2,i3,v2)*u(i1,i2+1,i3,n3))+
     & coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3+1,n1)   +v(i1,
     & i2,i3,v1)*u(i1,i2,i3+1,n2)   +v(i1,i2,i3,v2)*u(i1,i2,i3+1,n3)))
     & )/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*v(i1,i2,i3,v1)
                    u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*v(i1,i2,i3,v2)
                end do
                end do
                end do
              end if
              else
! loopsd4(temp=( (vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,i3,v1)) - (coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), , )
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,
     & i3,v1))-(coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1))+
     & coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1))+coeff(m32,
     & i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1))))/coeff(mGhost,i1,
     & i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)


                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,
     & i3,v1))-(coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1))+
     & coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,i3,n1))+coeff(m32,
     & i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,n1))))/coeff(mGhost,i1,
     & i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)


                end do
                end do
                end do
              end if
              end if
            else if( nd.eq.3 ) then
! normalDerivativeCurvilinear(vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,i3,v1)+vData(m3)*v(i1,i2,i3,v2))
              if( nd.eq.2 ) then
! loopsd4(temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,i3,v1)+vData(m3)*v(i1,i2,i3,v2)) -(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3,n2)) +coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3,n2)) +coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3,n2)) +coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1) +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3,n2)) +coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3,n1) +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3,n2)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*v(i1,i2,i3,v1), )
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,
     & i3,v1)+vData(m3)*v(i1,i2,i3,v2))-(coeff(m21,i1,i2,i3)*(v(i1,i2,
     & i3,v0)*u(i1,i2-1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2))+coeff(
     & m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)+v(i1,i2,i3,v1)*
     & u(i1-1,i2,i3,n2))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,
     & i3,n1)+v(i1,i2,i3,v1)*u(i1,i2,i3,n2))+coeff(m32,i1,i2,i3)*(v(
     & i1,i2,i3,v0)*u(i1+1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2))+
     & coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)+v(i1,i2,
     & i3,v1)*u(i1,i2+1,i3,n2))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & v(i1,i2,i3,v1)

                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,
     & i3,v1)+vData(m3)*v(i1,i2,i3,v2))-(coeff(m21,i1,i2,i3)*(v(i1,i2,
     & i3,v0)*u(i1,i2-1,i3,n1)+v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2))+coeff(
     & m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2,i3,n1)+v(i1,i2,i3,v1)*
     & u(i1-1,i2,i3,n2))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2,
     & i3,n1)+v(i1,i2,i3,v1)*u(i1,i2,i3,n2))+coeff(m32,i1,i2,i3)*(v(
     & i1,i2,i3,v0)*u(i1+1,i2,i3,n1)+v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2))+
     & coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1,i2+1,i3,n1)+v(i1,i2,
     & i3,v1)*u(i1,i2+1,i3,n2))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*
     & v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*
     & v(i1,i2,i3,v1)

                end do
                end do
                end do
              end if
              else if( nd.eq.3 ) then
! loopsd4(temp=( (vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,i3,v1)+vData(m3)*v(i1,i2,i3,v2)) -(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3-1,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3-1,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3-1,n3)) +coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2-1,i3  ,n3)) +coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1-1,i2  ,i3  ,n3)) +coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3  ,n3)) +coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1+1,i2  ,i3  ,n3)) +coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3  ,n1)             +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3  ,n2)             +v(i1,i2,i3,v2)*u(i1  ,i2+1,i3  ,n3)) +coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3+1,n1)      +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3+1,n2)      +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3+1,n3)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*v(i1,i2,i3,v1), u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*v(i1,i2,i3,v2))
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,
     & i3,v1)+vData(m3)*v(i1,i2,i3,v2))-(coeff(m221,i1,i2,i3)*(v(i1,
     & i2,i3,v0)*u(i1,i2,i3-1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3-1,n2)  
     & +v(i1,i2,i3,v2)*u(i1,i2,i3-1,n3))+coeff(m212,i1,i2,i3)*(v(i1,
     & i2,i3,v0)*u(i1,i2-1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2)  +
     & v(i1,i2,i3,v2)*u(i1,i2-1,i3,n3))+coeff(m122,i1,i2,i3)*(v(i1,i2,
     & i3,v0)*u(i1-1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2)  +v(
     & i1,i2,i3,v2)*u(i1-1,i2,i3,n3))+coeff(m222,i1,i2,i3)*(v(i1,i2,
     & i3,v0)*u(i1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1,i2,i3,n3))+coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*
     & u(i1+1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2)  +v(i1,i2,
     & i3,v2)*u(i1+1,i2,i3,n3))+coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*
     & u(i1,i2+1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2)  +v(i1,i2,
     & i3,v2)*u(i1,i2+1,i3,n3))+coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*
     & u(i1,i2,i3+1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3+1,n2)   +v(i1,i2,
     & i3,v2)*u(i1,i2,i3+1,n3))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*v(i1,i2,i3,v1)
                    u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*v(i1,i2,i3,v2)
                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,
     & i3,v1)+vData(m3)*v(i1,i2,i3,v2))-(coeff(m221,i1,i2,i3)*(v(i1,
     & i2,i3,v0)*u(i1,i2,i3-1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3-1,n2)  
     & +v(i1,i2,i3,v2)*u(i1,i2,i3-1,n3))+coeff(m212,i1,i2,i3)*(v(i1,
     & i2,i3,v0)*u(i1,i2-1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2-1,i3,n2)  +
     & v(i1,i2,i3,v2)*u(i1,i2-1,i3,n3))+coeff(m122,i1,i2,i3)*(v(i1,i2,
     & i3,v0)*u(i1-1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1-1,i2,i3,n2)  +v(
     & i1,i2,i3,v2)*u(i1-1,i2,i3,n3))+coeff(m222,i1,i2,i3)*(v(i1,i2,
     & i3,v0)*u(i1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2,i3,n2)  +v(i1,
     & i2,i3,v2)*u(i1,i2,i3,n3))+coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*
     & u(i1+1,i2,i3,n1)  +v(i1,i2,i3,v1)*u(i1+1,i2,i3,n2)  +v(i1,i2,
     & i3,v2)*u(i1+1,i2,i3,n3))+coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*
     & u(i1,i2+1,i3,n1)  +v(i1,i2,i3,v1)*u(i1,i2+1,i3,n2)  +v(i1,i2,
     & i3,v2)*u(i1,i2+1,i3,n3))+coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*
     & u(i1,i2,i3+1,n1)   +v(i1,i2,i3,v1)*u(i1,i2,i3+1,n2)   +v(i1,i2,
     & i3,v2)*u(i1,i2,i3+1,n3))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)
                    u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,
     & n2)+temp*v(i1,i2,i3,v1)
                    u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,
     & n3)+temp*v(i1,i2,i3,v2)
                end do
                end do
                end do
              end if
              else
! loopsd4(temp=( (vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,i3,v1)+vData(m3)*v(i1,i2,i3,v2)) - (coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1)) ) )/coeff(mGhost,i1,i2,i3), u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), , )
              if( useWhereMask.ne.0 )then
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                  if( mask(i1,i2,i3).ne.0 )then
                    temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,
     & i3,v1)+vData(m3)*v(i1,i2,i3,v2))-(coeff(m12,i1,i2,i3)*(v(i1,i2,
     & i3,v0)*u(i1-1,i2,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,
     & n1))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)


                  end if
                end do
                end do
                end do
              else
                do i3=n3a,n3b
                do i2=n2a,n2b
                do i1=n1a,n1b
                    temp=((vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,
     & i3,v1)+vData(m3)*v(i1,i2,i3,v2))-(coeff(m12,i1,i2,i3)*(v(i1,i2,
     & i3,v0)*u(i1-1,i2,i3,n1))+coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(
     & i1,i2,i3,n1))+coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2,i3,
     & n1))))/coeff(mGhost,i1,i2,i3)
                    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,
     & n1)+temp*v(i1,i2,i3,v0)


                end do
                end do
                end do
              end if
              end if
            else
              stop 12345
            end if
          end if
        end if
        return
        end
