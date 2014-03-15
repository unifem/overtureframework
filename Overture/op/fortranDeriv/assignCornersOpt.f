! This file automatically generated from assignCornersOpt.bf with bpp.
c You should preprocess this file with the bpp preprocessor before compiling.

c *** macros for extrapolation ***








! *************** Vector Symmetry BC Macros ********************







c ================================================================================================
c  /Description:
c     Apply an extrapolation, symmetry or Taylor-series approximation boundary condition.
c  /i1,i2,i3,n: Indexs of points to assign.
c ===============================================================================================



      subroutine fixBoundaryCornersOpt( nd,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension,
     & isPeriodic, bc, cornerBC, ipar, rpar )
c======================================================================
c  Optimised Boundary Conditions
c         
c nd : number of space dimensions
c ca,cb : assign components c=uC(ca),..,uC(cb)
c cv : start of vector for vectorSymmetryCorner BC
c useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
c
c ncg: number of corner ghost points to assign
c cornerExtrapolationOption : used to extrapolate corners along certain directions (or really
c  to not extrapolate in certain directions).
c======================================================================
      implicit none
      integer nd, orderOfExtrapolation,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,ncg,
     & cornerExtrapolationOption

      integer useWhereMask,bc(0:1,0:2),isPeriodic(0:2)
      integer indexRange(0:1,0:2),dimension(0:1,0:2)
      integer cornerBC(0:2,0:2,0:2)
      integer ipar(0:*)
      real rpar(0:*),normEps

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real rsxy(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b,0:nd-1,0:nd-1)
      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)

      integer c,ca,cb,n

c........end statement functions

c        ---extrapolate or otherwise assign values outside edges---

      call fixBCOptEdge3( nd,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension,
     & isPeriodic, bc, cornerBC, ipar, rpar )

      if( nd.le.2 )then
        return
      end if

      call fixBCOptEdge2( nd,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension,
     & isPeriodic, bc, cornerBC, ipar, rpar )

      call fixBCOptEdge1( nd,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension,
     & isPeriodic, bc, cornerBC, ipar, rpar )

      call fixBCOptVerticies( nd,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask,rsxy, indexRange, dimension,
     & isPeriodic, bc, cornerBC, ipar, rpar )


      return
      end





