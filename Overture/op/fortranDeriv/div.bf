c This file contains:
c
c   divergenceFDeriv  : compute the divergence, conservative and non-conservative, orders 2,4,6,8
c

c These next include files will define the macros that will define the difference approximations
#Include "../src/derivMacroDefinitions.h"

#Include "../src/defineParametricDerivMacros.h"

! defineParametricDerivativeMacros(u,DIM,ORDER,COMPONENTS,MAXDERIV)

defineParametricDerivativeMacros(u,dr,dx,2,2,1,1)
defineParametricDerivativeMacros(u,dr,dx,2,4,1,1)
defineParametricDerivativeMacros(u,dr,dx,2,6,1,1)
defineParametricDerivativeMacros(u,dr,dx,2,8,1,1)

defineParametricDerivativeMacros(u1,dr,dx,2,2,0,1)
defineParametricDerivativeMacros(u1,dr,dx,2,4,0,1)
defineParametricDerivativeMacros(u1,dr,dx,2,6,0,1)
defineParametricDerivativeMacros(u1,dr,dx,2,8,0,1)

defineParametricDerivativeMacros(u2,dr,dx,2,2,0,1)
defineParametricDerivativeMacros(u2,dr,dx,2,4,0,1)
defineParametricDerivativeMacros(u2,dr,dx,2,6,0,1)
defineParametricDerivativeMacros(u2,dr,dx,2,8,0,1)

c  3D

defineParametricDerivativeMacros(u,dr,dx,3,2,1,1)
defineParametricDerivativeMacros(u,dr,dx,3,4,1,1)
defineParametricDerivativeMacros(u,dr,dx,3,6,1,1)
defineParametricDerivativeMacros(u,dr,dx,3,8,1,1)

defineParametricDerivativeMacros(u1,dr,dx,3,2,0,1)
defineParametricDerivativeMacros(u1,dr,dx,3,4,0,1)
defineParametricDerivativeMacros(u1,dr,dx,3,6,0,1)
defineParametricDerivativeMacros(u1,dr,dx,3,8,0,1)

defineParametricDerivativeMacros(u2,dr,dx,3,2,0,1)
defineParametricDerivativeMacros(u2,dr,dx,3,4,0,1)
defineParametricDerivativeMacros(u2,dr,dx,3,6,0,1)
defineParametricDerivativeMacros(u2,dr,dx,3,8,0,1)

defineParametricDerivativeMacros(u3,dr,dx,3,2,0,1)
defineParametricDerivativeMacros(u3,dr,dx,3,4,0,1)
defineParametricDerivativeMacros(u3,dr,dx,3,6,0,1)
defineParametricDerivativeMacros(u3,dr,dx,3,8,0,1)




#beginMacro beginLoops()
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
#endMacro

#beginMacro endLoops()
 end do
 end do
 end do
#endMacro


c =====================================================================
c Main Macro to evaluate the divergence
c
c  DIM : 2,3
c ORDER: 2,4,6,8
c =====================================================================
#beginMacro getDivergence(DIM,ORDER)

c  *** Evaluate the divergence ***

 if( gridType.eq.rectangular )then
  ! Cartesian, dim=DIM, order=ORDER 
  beginLoops()
   #If #DIM == "2" 
     deriv(i1,i2,i3,n)=ux ## ORDER(i1,i2,i3,c1)+uy ## ORDER(i1,i2,i3,c2)
   #Else
     deriv(i1,i2,i3,n)=ux ## ORDER(i1,i2,i3,c1)+uy ## ORDER(i1,i2,i3,c2)+uz ## ORDER(i1,i2,i3,c3)
   #End
  endLoops()

 else if( derivType.eq.nonConservative )then

  ! Curvilinear, non-conservative, dim=DIM, order=ORDER 
  beginLoops()
   ! order 4:
   !                                        DIM,ORDER,MAXDERIV
   evalJacobianDerivatives(rsxy,i1,i2,i3,rx,DIM,ORDER,0)

   !                                                      DIM,ORDER,MAXDERIV
   evalParametricDerivativesComponents1(u,i1,i2,i3,c1, uu,DIM,ORDER,1)
   getDuDx ## DIM(uu,rx,ux)

   evalParametricDerivativesComponents1(u,i1,i2,i3,c2, uu,DIM,ORDER,1)
   getDuDy ## DIM(uu,rx,vy)

   #If #DIM == "2" 
     deriv(i1,i2,i3,n)=ux+vy
   #Else

     evalParametricDerivativesComponents1(u,i1,i2,i3,c3, uu,DIM,ORDER,1)
     getDuDz ## DIM(uu,rx,wz)
     deriv(i1,i2,i3,n)=ux+vy+wz

   #End

  endLoops()

 else
   ! conservative, curvilinear, dim=DIM, order=ORDER 

   #If #DIM == "2"     
#defineMacro u1(i1,i2,i3) (jac(i1,i2,i3)*(rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,c1)+rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,c2)))
#defineMacro u2(i1,i2,i3) (jac(i1,i2,i3)*(rsxy(i1,i2,i3,1,0)*u(i1,i2,i3,c1)+rsxy(i1,i2,i3,1,1)*u(i1,i2,i3,c2)))

     beginLoops()
       deriv(i1,i2,i3,n)=(u1r ## ORDER(i1,i2,i3)+u2s ## ORDER(i1,i2,i3))/jac(i1,i2,i3)
     endLoops()

   #Else

#defineMacro u1(i1,i2,i3) (jac(i1,i2,i3)*(rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,c1)+\
                                          rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,c2)+\
                                          rsxy(i1,i2,i3,0,2)*u(i1,i2,i3,c3)))
#defineMacro u2(i1,i2,i3) (jac(i1,i2,i3)*(rsxy(i1,i2,i3,1,0)*u(i1,i2,i3,c1)+\
                                          rsxy(i1,i2,i3,1,1)*u(i1,i2,i3,c2)+\
                                          rsxy(i1,i2,i3,1,2)*u(i1,i2,i3,c3)))
#defineMacro u3(i1,i2,i3) (jac(i1,i2,i3)*(rsxy(i1,i2,i3,2,0)*u(i1,i2,i3,c1)+\
                                          rsxy(i1,i2,i3,2,1)*u(i1,i2,i3,c2)+\
                                          rsxy(i1,i2,i3,2,2)*u(i1,i2,i3,c3)))


     beginLoops()
       deriv(i1,i2,i3,n)=(u1r ## ORDER(i1,i2,i3)+u2s ## ORDER(i1,i2,i3)+u3t ## ORDER(i1,i2,i3))/jac(i1,i2,i3)
     endLoops()

   #End

 end if


#endMacro

      subroutine divergenceFDeriv( nd, 
     &    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,         
     &    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, ! dimensions for u
     &    ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, ! dimensions for deriv
     &    n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, 
     &    dx, dr,
     &    rsxy, jac, u,s, deriv, 
     &    ndw,w,  ! work space
     &    derivOption, derivType, gridType, order, averagingType, 
     &    dir1, dir2  )
c======================================================================
c  Discretizations for
c           div
c  
c ca,cb : assign components c=ca,..,cb (base 0)
c derivOption : 4=divergence
c derivType : 0=nonconservative, 1=conservative, 2=conservative+symmetric
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4
c averagingType : arithmeticAverage=0, harmonicAverage=1
c dir1,dir2 : for derivOption=derivativeScalarDerivative
c rsxy : not used if rectangular
c dr : 
c 
c======================================================================
      implicit none
      integer nd, 
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,
     &  n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,
     &  derivOption, derivType, gridType, order, averagingType,ndw,
     & dir1,dir2

      real dx(0:2),dr(0:2)
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
      real jac(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:*)
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)
      real w(0:*)
      
c      real h21(3),d22(3),d12(3),h22(3)
c      real d24(3),d14(3),h42(3),h41(3)
      integer i1,i2,i3
      real ux,vy,wz

      integer n,nda,ndwMin,c1,c2,c3
      integer laplace,divScalarGrad,derivativeScalarDerivative,divTensorGrad,divergence
      parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,divTensorGrad=3,divergence=4)
    
      integer rectangular,curvilinear
      parameter( rectangular=0,curvilinear=1 )

      integer nonConservative,conservative,conservativeAndSymmetric
      parameter( nonConservative=0, conservative=1, conservativeAndSymmetric=2)

c     --- start statement function ----
      integer kd,m
      real u1,u2

      declareParametricDerivativeVariables(rx,3)
      declareParametricDerivativeVariables(uu,3)

      declareJacobianDerivativeVariables(rx,3)


      if( derivOption.ne.divergence )then
        write(*,'("divergenceFDeriv:ERROR:derivOption=",i6)') derivOption
        ! "
        stop 9273
      end if

      n=ndd4a

      c1=ca     ! ****
      c2=ca+1
      c3=ca+2

c       write(*,'(" i=",2i3," u=",2e11.2," div=",e11.2)') 
c     & i1,i2,u(i1,i2,i3,c1),u(i1,i2,i3,c2),deriv(i1,i2,i3,n)

c     Evaluate the derivative

      if( nd.eq.2 )then
        ! ********************************
        ! ************* 2D ***************
        ! ********************************

        if( order.eq.2 )then
          getDivergence(2,2)
        else if( order.eq.4 )then
          getDivergence(2,4)
        else if( order.eq.6 )then
          getDivergence(2,6)
        else if( order.eq.8 )then
          getDivergence(2,8)
        else
          stop 6134
        end if

      else if( nd.eq.3 )then
        ! ********************************
        ! ************* 3D ***************
        ! ********************************

        if( order.eq.2 )then
          getDivergence(3,2)
        else if( order.eq.4 )then
          getDivergence(3,4)
        else if( order.eq.6 )then
          getDivergence(3,6)
        else if( order.eq.8 )then
          getDivergence(3,8)
        else
          stop 6134
        end if
      else
        stop 11
      end if

c*      ! Cartesian:
c*      beginLoops()
c*        ! getDuDx2(u,rsxy,ux)
c*
c*        deriv(i1,i2,i3,0)=ux4(i1,i2,i3,c1)+uy4(i1,i2,i3,c2)
c*      endLoops()
c*
c*      ! Curvilinear, non-conservative
c*      beginLoops()
c*        ! order 4:
c*        !                                        DIM,ORDER,MAXDERIV
c*        evalJacobianDerivatives(rsxy,i1,i2,i3,rx,2,4,0)
c*
c*
c*        !                                                      DIM,ORDER,MAXDERIV
c*        evalParametricDerivativesComponents1(u,i1,i2,i3,c1, uu,2,4,1)
c*        getDuDx2(uu,rx,ux)
c*
c*        evalParametricDerivativesComponents1(u,i1,i2,i3,c2, uu,2,4,1)
c*        getDuDx2(uu,rx,vy)
c*
c*        deriv(i1,i2,i3,0)=ux+vy
c*      endLoops()
c*
c*     
c*#defineMacro u1(i1,i2,i3) (jac(i1,i2,i3)*(rsxy(i1,i2,i3,0,0)*u(i1,i2,i3,c1)+rsxy(i1,i2,i3,0,1)*u(i1,i2,i3,c2)))
c*#defineMacro u2(i1,i2,i3) (jac(i1,i2,i3)*(rsxy(i1,i2,i3,1,0)*u(i1,i2,i3,c1)+rsxy(i1,i2,i3,1,1)*u(i1,i2,i3,c2)))
c*
c*      ! conservative, curvilinear
c*      beginLoops()
c*        deriv(i1,i2,i3,0)=(u1r4(i1,i2,i3)+u2s4(i1,i2,i3))/jac(i1,i2,i3)
c*      endLoops()
c*
      

      return 
      end



