! This file automatically generated from assignBoundaryConditions.bf with bpp.
! You should preprocess this file with the bpp preprocessor before compiling.


! loops: use mask(i1,i2,i3)

! loopsm: use mask(i1+im1,i2+im2,i3+im3)

! Assign ghost point from a Neuman or Mixed BC
! For variable coefficients we set ghost value from
!   v(i1,i2,i3,0)*u + v(i1,i2,i3,1)*coeff*u = rhs 



! from pmb 




! The generalized divergence BC should only be applied where the mask>0 061015





!   v(i1,i2,i3,n1) = ux(i1,i2,i3,n1) 
!   v(i1,i2,i3,n2) = uy(i1,i2,i3,n2) 
!   v(i1,i2,i3,n3) = uz(i1,i2,i3,n3) 







! ================================================================================================
!   Set the normal component for a curvilinear grid
!
!  DIM : dimension, 2 or 3
!  FORCING: type of forcing, NO_FORCING, SCALAR_FORCING, GF_FORCING, ARRAY_FORCING, VECTOR_FORCING
!================================================================================================

! ================================================================================================
!   Assign the normal component for a curvilinear grid
!
!  FORCING: type of forcing, NO_FORCING, SCALAR_FORCING, GF_FORCING, ARRAY_FORCING, VECTOR_FORCING
!================================================================================================

! ================================================================================================
!   Set the tangential component for a curvilinear grid
!
!  DIM : dimension, 2 or 3
!  FORCING: type of forcing, NO_FORCING, SCALAR_FORCING, GF_FORCING, ARRAY_FORCING, VECTOR_FORCING
!================================================================================================

! ================================================================================================
!   Assign the tangential component for a curvilinear grid
!
!  FORCING: type of forcing, NO_FORCING, SCALAR_FORCING, GF_FORCING, ARRAY_FORCING, VECTOR_FORCING
!================================================================================================

      subroutine assignBoundaryConditions( nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData,
     & dx,dr,ipar,par, ca,cb, uCBase, uC, fCBase, fC,
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
! v : = variable coefficients 
!   : for generalizedDivergence v holds ux(i1,i2,i3,n1) uy(i1,i2,i3,n2) uz(i1,i2,i3,n3)
!======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,uCBase,fCBase

      integer side,axis,grid, bcType,bcOption,gridType,order,
     &  useWhereMask,lineForForcing

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
!      real h21(3),d22(3),d12(3),h22(3)
!      real d24(3),d14(3),h42(3),h41(3)


!     --- boundary conditions (from BCTypes.h) ---
      integer dirichlet,
     &        neumann,
     &        extrapolate,
     &        normalComponent,
     &        mixed,
     &        generalMixedDerivative,
     &        normalDerOfNormalComponent,
     &        normalDerivativeOfADotU,
     &        aDotU,
     &        aDotGradU,
     &        normalDotScalarGrad,
     &        evenSymmetry,
     &        oddSymmetry,
     &        generalizedDivergence,
     &        vectorSymmetry,
     &        tangentialComponent0,
     &        tangentialComponent1,
     &        normalDerOfTangentialComponent0,
     &        normalDerOfTangentialComponent1,
     &        extrapolateInterpNeighbours,
     &        tangentialComponent,
     &        extrapolateNormalComponent,
     &        extrapolateTangentialComponent0,
     &        extrapolateTangentialComponent1
      parameter(dirichlet=0,neumann=dirichlet+1,extrapolate=neumann+1,
     & normalComponent=extrapolate+1,mixed=normalComponent+1,
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
     &           vectorForcing=3 )

!     --- local variables ----
      real b0,b1,b2,b3,twoDeltaX,twoDeltaY,twoDeltaZ,temp
      real nsign,an1,an2,an3,nDotU,anorm
      integer c,c0,f,i1,i2,i3,im1,im2,im3,ip1,ip2,ip3,if1,if2,if3,cn,cm
      integer n1,n2,n3,m1,m2,m3,v0,v1,v2

      integer mGhost
      integer m21,m12,m22,m32,m23
      integer m221,m212,m122,m222,m322,m232,m223



      if( bcType.eq.neumann .or. bcType.eq.mixed )then

        call assignOptNeumann( nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData,
     & dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )


      else if( bcType.eq.generalizedDivergence )then
        ! ***********************************
        ! ****** Generalized divergence *****
        ! ***********************************  
        !  to set the component along a to g:
        !       u <- u + (g-(a.u)) a/<a,a>
        !       g-(a.u) = b - ( discrete div of u )

        call assignOptGenDiv( nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData,
     & dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )

      else if( bcType.eq.aDotGradU )then
        ! from pmb 080421
        ! ***********************************
        ! ******       aDotGradU        *****
        ! ***********************************  
        !  to set the derivative along a to g:
	!  (a.grad) u = g

        call assignOptADotGradU( nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData,
     & dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )


      else if( bcType.eq.normalDerOfNormalComponent .or.
     &         bcType.eq.normalDerOfTangentialComponent0 .or.
     &         bcType.eq.normalDerOfTangentialComponent1 )then

!       **************************************************
!       **** normal derivative of a vector dot u *********
!       **************************************************

        call assignOptNormalDer( nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData,
     & dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )
        twoDeltaX=par(0)


      else if( bcType.eq.normalComponent )then
!        ****************************
!        ***** Normal Component *****
!        ****************************

        call assignOptNormalComponent( nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData,
     & dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )

      else if( bcType.eq.tangentialComponent .or.
     &         bcType.eq.tangentialComponent0 .or.
     &         bcType.eq.tangentialComponent1 )then
!        ********************************
!        ***** Tangential Component *****
!        ********************************

        call assignOptTangentialComponent( nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData,
     & dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )



      else
        write(*,*) 'assignBoundaryConditions:ERROR unknown bcType=',
     &    bcType
        stop 33
      end if

      return
      end



! ======================================================================
! 
!  Generate opt BC routines for different BCs
!
! BCOPT: dirichlet, neuman
! ======================================================================



! buildFile(assignOptNeumann,neumann)
! buildFile(assignOptGenDiv,generalizedDivergence)
! buildFile(assignOptNormalDer,normalDerivative)
! buildFile(assignOptNormalComponent,normalComponent)
! buildFile(assignOptADotGradU,aDotGradU)
! buildFile(assignOptTangentialComponent,tangentialComponent)




      subroutine periodicUpdateOpt(nd,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & u,ca,cb, indexRange, gridIndexRange, dimension,
     & isPeriodic )
!======================================================================
!  Optimised Boundary Conditions
!         
! nd : number of space dimensions
! ca,cb : assign these components
! useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
!======================================================================
      implicit none
      integer nd,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b

      integer isPeriodic(0:2),indexRange(0:1,0:2)
      integer gridIndexRange(0:1,0:2),dimension(0:1,0:2)

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      integer ca,cb

!     --- local variables 
      integer c,i1,i2,i3,axis,diff
      integer n1a,n1b, n2a,n2b, n3a,n3b


      n1a=dimension(0,0)
      n1b=dimension(1,0)
      n2a=dimension(0,1)
      n2b=dimension(1,1)
      n3a=dimension(0,2)
      n3b=dimension(1,2)

      do axis=0,nd-1
        if( isPeriodic(axis).ne.0 )then
!         length of the period:
          diff=gridIndexRange(1,axis)-gridIndexRange(0,axis)
!         assign all ghost points on "left"
!         I[i]=Range(dimension(Start,axis),indexRange(Start,axis)-1);
!         u(I[0],I[1],I[2],I[3])=u(I[0]+diff[0],I[1]+diff[1],I[2]+diff[2],I[3]+diff[3]);
!         // assign all ghost points on "right"
!         I[i]=Range(indexRange(End,axis)+1,dimension(End,axis));
!         u(I[0],I[1],I[2],I[3])=u(I[0]-diff[0],I[1]-diff[1],I[2]-diff[2],I[3]-diff[3]);

          if( axis.eq.0 )then
            n1a=dimension(0,0)
            n1b=indexRange(0,0)-1
! loops1(u(i1,i2,i3,c)=u(i1+diff,i2,i3,c))
            do c=ca,cb
            do i1=n1a,n1b
            do i3=n3a,n3b
            do i2=n2a,n2b
              u(i1,i2,i3,c)=u(i1+diff,i2,i3,c)
            end do
            end do
            end do
            end do
            n1a=indexRange(1,0)+1
            n1b=dimension(1,0)
! loops1(u(i1,i2,i3,c)=u(i1-diff,i2,i3,c))
            do c=ca,cb
            do i1=n1a,n1b
            do i3=n3a,n3b
            do i2=n2a,n2b
              u(i1,i2,i3,c)=u(i1-diff,i2,i3,c)
            end do
            end do
            end do
            end do
            n1a=dimension(0,0)
            n1b=dimension(1,0)
          else if( axis.eq.1 )then
            n2a=dimension(0,1)
            n2b=indexRange(0,1)-1
! loops2(u(i1,i2,i3,c)=u(i1,i2+diff,i3,c))
            do c=ca,cb
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              u(i1,i2,i3,c)=u(i1,i2+diff,i3,c)
            end do
            end do
            end do
            end do
            n2a=indexRange(1,1)+1
            n2b=dimension(1,1)
! loops2(u(i1,i2,i3,c)=u(i1,i2-diff,i3,c))
            do c=ca,cb
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              u(i1,i2,i3,c)=u(i1,i2-diff,i3,c)
            end do
            end do
            end do
            end do
            n2a=dimension(0,1)
            n2b=dimension(1,1)
          else
            n3a=dimension(0,2)
            n3b=indexRange(0,2)-1
! loops2(u(i1,i2,i3,c)=u(i1,i2,i3+diff,c))
            do c=ca,cb
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              u(i1,i2,i3,c)=u(i1,i2,i3+diff,c)
            end do
            end do
            end do
            end do
            n3a=indexRange(1,2)+1
            n3b=dimension(1,2)
! loops2(u(i1,i2,i3,c)=u(i1,i2,i3-diff,c))
            do c=ca,cb
            do i3=n3a,n3b
            do i2=n2a,n2b
            do i1=n1a,n1b
              u(i1,i2,i3,c)=u(i1,i2,i3-diff,c)
            end do
            end do
            end do
            end do
            n3a=dimension(0,2)
            n3b=dimension(1,2)
          end if

        end if
      end do
      return
      end

