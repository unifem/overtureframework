#Include "ins_factors.bh"

      subroutine get_os_mop_coeffs(hw,nder,side,order,type,P,D)
!!    Return the one sided merged compact operators in P and D.
      implicit none

      integer nder,side,order,type,hw
      double precision P(0:*),D(0:*)
      
      DEFINE_PARAMETERS()
      integer i

      do i = 0,2*hw
         P(i) = 0d0
         D(i) = 0d0
      enddo

      if ( type.eq.compact ) then
         if (  order.eq. 2 ) then
            if ( .true. ) then
               !! these are second order accurate compact approximations
               if (side.eq.0) then
                  P(0) = 1d0
                  P(1) = 1d0
                  P(2) = 1d0
                  if ( nder.eq.1 ) then
                     D(0) = -3d0/2d0
                     D(1) =  0d0
                     D(2) =  3d0/2d0
                  else if ( nder.eq.2 ) then
                     D( 0) = 3d0
                     D( 1) =-6d0
                     D( 2) = 3d0
                  endif
               else
                  P(2*hw-2) = 1d0
                  P(2*hw-1) = 1d0
                  P(2*hw)   = 1d0
                  if ( nder.eq.1 ) then
                     D(2*hw)   =  3d0/2d0
                     D(2*hw-2) = -3d0/2d0
                  else
                     D(2*hw)   =  3d0
                     D(2*hw-1) = -6d0
                     D(2*hw-2) =  3d0                  
                  endif
               endif
            else 
               !! these are first order accurate explicit approximations
               if ( side.eq.0 ) then
                  P(0) = 1d0
                  if ( nder.eq.1 ) then
                     D(0) = -1d0
                     D(1) =  1d0
                  else
                     D(0) = 1d0
                     D(1) =-2d0
                     D(2) = 1d0
                  endif
               else
                  P(2*hw) = 1d0
                  if ( nder.eq.1 ) then
                     D(2*hw-1) = -1d0
                     D(2*hw) =  1d0
                  else
                     D(2*hw-2) = 1d0
                     D(2*hw-1) =-2d0
                     D(2*hw) = 1d0
                  endif
               endif
            endif
         else if ( order.eq.3 ) then
            if (side.eq.0) then
               P(0) = 1d0
               P(1) = 8d0
               P(2) = 3d0
               if ( nder.eq.1 ) then
                  D(0) = -4d0
                  D(1) = -4d0
                  D(2) = 8d0
               else
                  D(0) = 10d0
                  D(1) = -18d0
                  D(2) = 6d0
                  D(3) = 2d0
               endif
            else
               P(2*hw-2) = 3d0
               P(2*hw-1) = 8d0
               P(2*hw) = 1d0
               if ( nder.eq.1 ) then
                  D(2*hw-2) = -8d0
                  D(2*hw-1) = 4d0
                  D(2*hw) = 4d0
               else
                  D(2*hw-3) = 2d0
                  D(2*hw-2) = 6d0
                  D(2*hw-1) = -18d0
                  D(2*hw) = 10d0
               endif
            endif
         else if ( order.eq.4 ) then
            if ( side.eq.0 ) then
               P(0) = 1d0
               P(1) = 11d0
               P(2) = 11d0
               P(3) = 1d0
               P(4) = 0d0
               if ( nder.eq.1 ) then
                  D(0) = -4d0
                  D(1) = -12d0
                  D(2) =  12d0
                  D(3) =  4d0
               else
                  D(0) =  12d0
                  D(1) = -12d0
                  D(2) = -12d0
                  D(3) =  12d0
               endif
            else
               P(2*hw)   = 1d0
               P(2*hw-1) = 11d0
               P(2*hw-2) = 11d0
               P(2*hw-3) = 1d0
               P(2*hw-4) = 0d0

               if ( nder.eq.1 ) then
                  D(2*hw)   = 4d0
                  D(2*hw-1) = 12d0
                  D(2*hw-2) =-12d0
                  D(2*hw-3) =-4d0
               else
                  D(2*hw)   = 12d0
                  D(2*hw-1) =-12d0
                  D(2*hw-2) =-12d0
                  D(2*hw-3) = 12d0
               endif
            endif
         elseif ( order.eq.6 ) then 
!!! 5th order accurate for order=6 near the boundary !!!
            if ( side.eq.0 ) then
               P(0) = 1d0
               P(1) = 15d0
               P(2) = 24d0
               P(3) = 5d0
               P(4) = 0d0
               if ( nder.eq.1 ) then
                  D(0) = -0.17D2 / 0.4D1
                  D(1) = -0.22D2
                  D(2) = 0.12D2
                  D(3) = 0.14D2
                  D(4) = 0.1D1 / 0.4D1
               else
                  D(0) = 0.57D2 / 0.4D1
                  D(1) = 0.0D0
                  D(2) = -0.81D2 / 0.2D1
                  D(3) = 0.24D2
                  D(4) = 0.9D1 / 0.4D1
               endif
            else
               P(2*hw-4) = 0d0
               P(2*hw-3) = 5d0
               P(2*hw-2) = 24d0
               P(2*hw-1) = 15d0
               P(2*hw) = 1d0
               if ( nder.eq.1 ) then
                  D(2*hw-4) = -0.1D1 / 0.4D1
                  D(2*hw-3) = -0.14D2
                  D(2*hw-2) = -0.12D2
                  D(2*hw-1) = 0.22D2
                  D(2*hw) = 0.17D2 / 0.4D1
               else
                  D(2*hw-4) = 0.9D1 / 0.4D1
                  D(2*hw-3) = 0.24D2
                  D(2*hw-2) = -0.81D2 / 0.2D1
                  D(2*hw-1) = 0.0D0
                  D(2*hw) = 0.57D2 / 0.4D1
               endif
            endif
         else !! order of accuracy
            stop 99502
         endif
      else !! type of difference operator
         stop 99600
      endif

      return 
      end


      subroutine get_bos_mop_coeffs(hw,nder,side,order,type,P,D)
!!    Return the "biased" one sided merged compact operators in P and D.
!!    The biased operators are ones where the stencil for P is shifted with
!!    respect to the stencil for D. For example, on the left side for 4th order
!!    we have coefficients that look like:
!!    0PPPP
!!    DDDD      
!!
      implicit none

      integer nder,side,order,type,hw
      double precision P(0:*),D(0:*)
      
      DEFINE_PARAMETERS()
      integer i

      do i = 0,2*hw
         P(i) = 0d0
         D(i) = 0d0
      enddo

      if ( type.eq.compact ) then
         if ( order.eq. 2 ) then
            if (side.eq.0) then
               P(1) = 1d0
               if ( nder.eq.1 ) then
                  D( 0) = -0.5d0
                  D( 2) =  0.5d0
               else if ( nder.eq.2 ) then
                  D( 0) = 1d0
                  D( 1) =-2d0
                  D( 2) = 1d0
               endif
            else
               P(2*hw-1) = 1d0
               if ( nder.eq.1 ) then
                  D(2*hw)   =  0.5d0
                  D(2*hw-2) = -0.5d0
               else
                  D(2*hw)   =  1d0
                  D(2*hw-1) = -2d0
                  D(2*hw-2) =  1d0                  
               endif
            endif
         else if ( .true. .or. order.eq.4 ) then
            if ( side.eq.0 ) then
               P(0) = 0d0
               P(1) = 1d0
               P(2) = 1d0/3d0
               P(3) = 1d0/3d0
               P(4) = -1d0/15d0
               if ( nder.eq.1 ) then
                  D(0) = -4d0/15d0
                  D(1) = -4d0/5d0
                  D(2) =  4d0/5d0
                  D(3) =  4d0/15d0
               else
                  D(0) =  4d0/5d0
                  D(1) = -4d0/5d0
                  D(2) = -4d0/5d0
                  D(3) =  4d0/5d0
               endif
            else
               P(2*hw)   = 0d0
               P(2*hw-1) = 1d0
               P(2*hw-2) = 1d0/3d0
               P(2*hw-3) = 1d0/3d0
               P(2*hw-4) = -1d0/15d0

               if ( nder.eq.1 ) then
                  D(2*hw)   = 4d0/15d0
                  D(2*hw-1) = 4d0/5d0
                  D(2*hw-2) =-4d0/5d0
                  D(2*hw-3) =-4d0/15d0
               else
                  D(2*hw)   = 4d0/5d0
                  D(2*hw-1) =-4d0/5d0
                  D(2*hw-2) =-4d0/5d0
                  D(2*hw-3) = 4d0/5d0
               endif
            endif
         else if ( order.eq.4 .or. order.eq.6 ) then 
!!! 5th order accurate for order=6 near the boundary !!!
            if ( side.eq.0 ) then
               P(0) = 0d0
               P(1) = 1d0
               P(2) = 24d0 / 5d0
               P(3) = 3d0
               P(4) = 1d0 / 5d0
               if ( nder.eq.1 ) then
                  D(0) = -0.1D1 / 0.20D2
                  D(1) = -0.14D2 / 0.5D1
                  D(2) = -0.12D2 / 0.5D1
                  D(3) = 0.22D2 / 0.5D1
                  D(4) = 0.17D2 / 0.20D2
               else
                  D(0) = 0.9D1 / 0.20D2
                  D(1) = 0.24D2 / 0.5D1
                  D(2) = -0.81D2 / 0.10D2
                  D(3) = 0.0D0
                  D(4) = 0.57D2 / 0.20D2
               endif
            else
               P(2*hw-4) = 0.1D1 / 0.5D1
               P(2*hw-3) = 0.3D1
               P(2*hw-2) = 0.24D2 / 0.5D1
               P(2*hw-1) = 0.1D1
               P(2*hw)   = 0.0D0
               if ( nder.eq.1 ) then
                  D(2*hw-4) = -0.17D2 / 0.20D2
                  D(2*hw-3) = -0.22D2 / 0.5D1
                  D(2*hw-2) = 0.12D2 / 0.5D1
                  D(2*hw-1) = 0.14D2 / 0.5D1
                  D(2*hw) = 0.1D1 / 0.20D2
               else
                  D(2*hw-4) = 0.57D2 / 0.20D2
                  D(2*hw-3) = 0.0D0
                  D(2*hw-2) = -0.81D2 / 0.10D2
                  D(2*hw-1) = 0.24D2 / 0.5D1
                  D(2*hw) = 0.9D1 / 0.20D2
               endif
            endif ! side
         else !! order of accuracy
            stop 99502
         endif
      else !! type of difference operator
         if ( order.eq. 2 ) then
            if (side.eq.0) then
               P(0) = 1d0
               if ( nder.eq.1 ) then
                  D( 0) =  -3d0/2d0
                  D( 1) =   2d0
                  D( 2) =  -0.5d0
               else if ( nder.eq.2 ) then
                  stop 99503
               endif
            else
               P(2*hw) = 1d0
               if ( nder.eq.1 ) then
                  D(2*hw)   =  3d0/2d0
                  D(2*hw-1) = -2d0
                  D(2*hw-2) =  0.5d0
               else
                  stop 99504
               endif
            endif
         else if ( order.eq.4 .or. order.eq.6 ) then 
!!! 4th order accurate for 4th and 6th !!!
            if ( side.eq.0 ) then
               P(0) = 1d0
               if ( nder.eq.1 ) then
                  D(0) = 19d0/6d0
                  D(1) = -7d0/4d0
                  D(2) =  1d0/2d0
                  D(3) = -1d0/24d0
               else
                  stop 99505
               endif
            else
               P(2*hw)   = 1d0
               if ( nder.eq.1 ) then
                  D(2*hw)   =-19d0/6d0
                  D(2*hw-1) = 7d0/4d0
                  D(2*hw-2) =-1d0/2d0
                  D(2*hw-3) = 1d0/24d0
               else
                  stop 99506
               endif
            endif
         else
            stop 99507
         endif
      endif
      
      return
      end 

      subroutine get_mop_coeffs(hw,nder,order,type,P,D)
      implicit none

      integer nder,order,type,hw
      double precision P(-hw:*),D(-hw:*)
      
      DEFINE_PARAMETERS()

      integer i

      do i = -hw,hw
         P(i) = 0d0
         D(i) = 0d0
      enddo

      if (type.eq.compact) then

         if ( order.eq.2 ) then

            P(0) = 1d0
            if ( nder.eq.1 ) then
               D(-1) = -0.5d0
               D( 1) =  0.5d0
            else if ( nder.eq.2 ) then
               D(-1) = 1d0
               D( 0) =-2d0
               D( 1) = 1d0
            else
               stop 99003
            endif ! nder
         else if (order.eq.4) then
            P(-2) = 0d0
            P(-1) = (1d0 / 4d0)
            P( 0) = 1d0
            P( 1) = (1d0 / 4d0)
            P( 2) = 0d0

            if ( nder.eq.1 ) then
               D(-2) = 0
               D(-1) = -(3d0 / 4d0)
               D( 0) = 0
               D( 1) = (3d0 / 4d0)
               D( 2) = 0
            else if ( nder.eq.2 ) then
               D(-2) = 1d0 / 8d0
               D(-1) = 1d0
               D( 0) = -9d0 / 4d0
               D( 1) = 1d0
               D( 2) = 1d0 / 8d0
            else
               stop 99004
            endif ! nder
         else if (order.eq.6) then
            P(-2) = 0.1e1 / 0.48e2
            P(-1) = 0.5e1 / 0.12e2
            P( 0) = 0.1e1
            P( 1) = 0.5e1 / 0.12e2
            P( 2) = 0.1e1 / 0.48e2
            
            if ( nder.eq.1 ) then
               D(-2) = -0.3e1 / 0.32e2
               D(-1) = -0.3e1 / 0.4e1
               D( 0) = 0.0e0
               D( 1) = 0.3e1 / 0.4e1
               D( 2) = 0.3e1 / 0.32e2
            else if (nder.eq.2) then
               D(-2) = 0.11e2 / 0.32e2
               D(-1) = 0.1e1 / 0.2e1
               D( 0) = -0.27e2 / 0.16e2
               D( 1) = 0.1e1 / 0.2e1
               D( 2) = 0.11e2 / 0.32e2
            else
               stop 99005
            endif

         else

            stop 99007
         endif ! order

      else if (type.eq.finite_difference) then
         call get_op_coeffs(hw,nder,order,type,P,D)
      else ! unknown difference typ
         stop 99008
      endif ! type

      return
      end

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! subroutine ins_rfactor
INS_FACTOR_SUBROUTINE(ins_mfactor)
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      double precision b_ik(-half_width:half_width) !! this holds the variable pde coefficients at adjacent grid points
      double precision a_ik(-half_width:half_width) !! this holds the variable pde coefficients at adjacent grid points
      integer io !! loop variable for looping over adjacent grid points in the stencil (i.e. from -half_width to half_width)
      integer idir !! index counter in the direction of the implicit solve
      integer width
      logical interp_bc,near_interp
      double precision pxc,dxc,dxxc
      pxc(io) = px_coeff(io)
      dxc(io) = dx_coeff(io)
      dxxc(io)= dxx_coeff(io)

      INS_FACTOR_EXTRACT_PARAM()

      is_penta = (order.gt.2 .and. disc_approx.eq.compact)
      have_ghost_points = .false.
      width = 2*half_width
      
      PRINT_DEBUG_INFO(ins_mfactor)

      INIT_STENCIL_SHIFT()

      idir = index_range(0,dir)+ilo(dir)
      do i3l=index_range(0,2)+ilo3,index_range(1,2)-ilo3
      do i2l=index_range(0,1)+ilo2,index_range(1,1)-ilo2
      do i1l=index_range(0,0)+ilo1,index_range(1,0)-ilo1

      SET_STENCIL_IDX()

      interp_bc = .false. .and. ((bc(0,dir).eq.0 .and. idir.le.(index_range(0,dir)+1)).or.(bc(1,dir).eq.0 .and. idir.ge.(index_range(1,dir)-1)))
      if ( mask(i1,i2,i3).gt.0 ) then

         if ( order.eq.2 ) then
            near_interp = mask(i1m,i2m,i3m).lt.0 .or. mask(i1p,i2p,i3p).lt.0 
         else
            near_interp = mask(i1m,i2m,i3m).lt.0 .or. mask(i1mm,i2mm,i3mm).lt.0 .or. mask(i1p,i2p,i3p).lt.0 .or. mask(i1pp,i2pp,i3pp).lt.0  
         endif

         if ( near_interp ) then
            call get_op_coeffs(half_width,1,min(order,4),finite_difference,px_coeff,dx_coeff)
            call get_op_coeffs(half_width,2,min(order,4),finite_difference,pxx_coeff,dxx_coeff)
            off = 0
         elseif ( interp_bc .or. is_periodic .or. (idir.gt.(index_range(0,dir)+1) .and. idir.lt.(index_range(1,dir)-1))) then
            call get_mop_coeffs(half_width,1,order,disc_approx,px_coeff,dx_coeff)
            call get_mop_coeffs(half_width,2,order,disc_approx,pxx_coeff,dxx_coeff)
            off =  0
         elseif ( .false. .and. idir.eq.index_range(0,dir) ) then
            call get_os_mop_coeffs(half_width,1,0,order,disc_approx,px_coeff,dx_coeff)
            call get_os_mop_coeffs(half_width,2,0,order,disc_approx,pxx_coeff,dxx_coeff)
            off = -2
         elseif ( .false. .and. idir.eq.index_range(1,dir) ) then
            call get_os_mop_coeffs(half_width,1,1,order,disc_approx,px_coeff,dx_coeff)
            call get_os_mop_coeffs(half_width,2,1,order,disc_approx,pxx_coeff,dxx_coeff)
            off =  2
         elseif ( idir.le.(index_range(0,dir)+1) ) then
            call get_bos_mop_coeffs(half_width,1,0,order,disc_approx,px_coeff,dx_coeff)
            call get_bos_mop_coeffs(half_width,2,0,order,disc_approx,pxx_coeff,dxx_coeff)
            off = -1
         else if ( idir.ge.(index_range(1,dir)-1) ) then
            call get_bos_mop_coeffs(half_width,1,1,order,disc_approx,px_coeff,dx_coeff)
            call get_bos_mop_coeffs(half_width,2,1,order,disc_approx,pxx_coeff,dxx_coeff)
            off =  1
         endif

         EVALUATE_JACOBIAN_DERIVATIVES(half_width)

         !!! here we assume there is always space for the first upper and lower diagonals
         !!! also, we assume that this is a "merged" coefficient routine so that px_coeff == pxx_coeff 
         d(i1,i2,i3)   = pxc(off+0)

         if ( off.eq.0 .or. off.eq.-1 .or. off.eq.1) then
            dl1(i1,i2,i3) = pxc(off-1)
            du1(i1,i2,i3) = pxc(off+1)
            if ( is_penta ) then
               if (off.ne.-1) then
                  dl2(i1,i2,i3) = pxc(off-2)
               else
                  dl2(i1,i2,i3) = pxc(off+3)
               endif
               
               if (off.ne.1) then
                  du2(i1,i2,i3) = pxc(off+2)
               else
                  du2(i1,i2,i3) = pxc(off-3)
               endif
            endif
         elseif (off.eq.-2 ) then
! d u1 l1          tri
! d u1 u2 l1 l2    penta
            du1(i1,i2,i3) = pxc(off+1)
            if ( is_penta ) then
               du2(i1,i2,i3) = pxc(off+2)
               dl1(i1,i2,i3) = pxc(off+3)
               dl2(i1,i2,i3) = pxc(off+4)
            else
               dl1(i1,i2,i3) = pxc(off+2)
            endif
         elseif (off.eq.2) then
!  u1 l1 d         tri 
!  u1 u2 l2 l1 d   penta
            dl1(i1,i2,i3) = pxc(off-1)
            if ( is_penta ) then
               dl2(i1,i2,i3) = pxc(off-2)
               du1(i1,i2,i3) = pxc(off-4)
               du2(i1,i2,i3) = pxc(off-3)
            else
               du1(i1,i2,i3) = pxc(off-2)
            endif
         endif

C          dl1(i1,i2,i3) = pxc(off-1)
C          d(i1,i2,i3)   = pxc(off+0)
C          du1(i1,i2,i3) = pxc(off+1)
C          if ( is_penta ) then
C             if (off.ne.-1) then
C                dl2(i1,i2,i3) = pxc(off-2)
C             else
C                dl2(i1,i2,i3) = pxc(off+3)
C             endif

C             if (off.ne.1) then
C                du2(i1,i2,i3) = pxc(off+2)
C             else
C                du2(i1,i2,i3) = pxc(off-3)
C             endif
C          endif
            
         !!! fill in the variable coefficient at each point to be used in later loops over discretization coefficients
         if ( mode.ne.get_explicit ) then
            do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
               a_ik(io) = 0d0
               b_ik(io) = 0d0
               GET_SHIFTED_IDX(i1o,i2o,i3o, (io-off)*is1, (io-off)*is2, (io-off)*is3)
               do id=0,nd-1
                  b_ik(io) = b_ik(io) + rx(io,dir,id)*rx(io,dir,id)
                  a_ik(io) = a_ik(io) + u(i1o,i2o,i3o,uc+id)*rx(io,dir,id)*advection_coefficient - nu_eq*rxx(io,dir,id) + nu_eq*4d0*rxr(io,dir,id)*rx(io,dir,id)
               enddo
               b_ik(io) = -b_ik(io)*nu_eq
            enddo
         endif !mode.ne.get_explicit

         if ( mode.eq.solve_rhs ) then
            rhs(i1,i2,i3) = 0d0
            do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
               !!!                                 (P  - (dt/2) D_r a_ik - (dt/2) D_rr b_ik) U^* 
               GET_SHIFTED_IDX(i1o,i2o,i3o, (io-off)*is1, (io-off)*is2, (io-off)*is3)
               rhs(i1,i2,i3) = rhs(i1,i2,i3) + (pxc(io) - dto2*drid*dxc(io)*a_ik(io) - dto2*drid*drid*dxxc(io)*b_ik(io))*ul(i1o,i2o,i3o,cc)
            enddo
         else if ( mode.eq.solve_lhs ) then
            rhs(i1,i2,i3) = 0d0
            do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
               !!!                                 P U^{*}
               GET_SHIFTED_IDX(i1o,i2o,i3o, (io-off)*is1, (io-off)*is2, (io-off)*is3)
               rhs(i1,i2,i3) = rhs(i1,i2,i3) + pxc(io)*ul(i1o,i2o,i3o,cc)
            enddo
               !!!                                 (P + (dt/2) D_r a_ik + (dt/2) D_rr b_ik) U^{**}_i
            io = 0+off
            d(i1,i2,i3)      = d(i1,i2,i3)   + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
            if ( off.eq.0 .or. off.eq.-1 .or. off.eq.1 ) then
               io = -1+off
               dl1(i1,i2,i3)    = dl1(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
               io = 1+off
               du1(i1,i2,i3)    = du1(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
               if ( is_penta ) then
                  io = -2+off
                  if (off.ne.-1) then
                     dl2(i1,i2,i3) = dl2(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
                  else
                     io = 2
                     dl2(i1,i2,i3) = dl2(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
                  endif
                  io =  2+off
                  if (off.ne.1) then
                     du2(i1,i2,i3) = du2(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
                  else
                     io = -2
                     du2(i1,i2,i3) = du2(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
                  endif
               endif
            elseif ( off.eq.-2) then
! d u1 l1          tri
! d u1 u2 l1 l2    penta
               io = 1+off
               du1(i1,i2,i3)    = du1(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
               if ( is_penta ) then
                  io = off+2
                  du2(i2,i3,i3) = du2(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
                  io = off+3
                  dl1(i1,i2,i3) = dl1(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
                  io = off+4
                  dl2(i1,i2,i3) = dl2(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
               else
                  io = off+2
                  dl1(i1,i2,i3) = dl1(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
               endif
            elseif ( off.eq. 2) then
!  u1 l1 d         tri 
!  u1 u2 l2 l1 d   penta
               io = -1+off
               dl1(i1,i2,i3)    = dl1(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
               if ( is_penta ) then
                  io = off-2
                  dl2(i1,i2,i3) = dl2(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
                  io = off-3
                  du2(i1,i2,i3) = du2(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
                  io = off-4
                  du1(i1,i2,i3) = du1(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
               else
                  io=off-2
                  du1(i1,i2,i3) = du1(i1,i2,i3) + dto2*drid*dxc(io)*a_ik(io) + dto2*drid*drid*dxxc(io)*b_ik(io)
               endif
            endif

         else ! mode .eq. get_explicit
            ! this is a compact scheme for the pressure, the contribution will be scaled and added to the appropriate equation
            !   outside this subroutine
            !! dp/dx_i = sum_{j=0}^{ndim-1} dr_j/dx_i dp/dr_j
            !! dr_j/dx_i dp/dr_j = dr_j/dx_i P^{-1} D p
            ! note we filled in the diagonals above and the bcs are taken care of below...
            rhs(i1,i2,i3) = 0d0
            do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
               GET_SHIFTED_IDX(i1o,i2o,i3o, (io-off)*is1, (io-off)*is2, (io-off)*is3)
               rhs(i1,i2,i3) = rhs(i1,i2,i3) +dxc(io)*u(i1o,i2o,i3o,pc)*drid
            enddo

         endif ! if mode .eq. solve_rhs or solve_lhs

      elseif ( mask(i1,i2,i3).ne.0 .and. mode.eq.get_explicit) then
         ! fill in interpolation points with a standard (4th order only) finite difference operator
         call get_op_coeffs(half_width,1,min(order,4),finite_difference,px_coeff,dx_coeff)
         dl1(i1,i2,i3) = 0d0
         d(i1,i2,i3)   = 1d0
         du1(i1,i2,i3) = 0d0
         if ( is_penta ) then
            dl2(i1,i2,i3) = 0d0
            du2(i1,i2,i3) = 0d0
         endif
         rhs(i1,i2,i3) = 0d0
         do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
            GET_SHIFTED_IDX(i1o,i2o,i3o, (io-off)*is1, (io-off)*is2, (io-off)*is3)
            rhs(i1,i2,i3) = rhs(i1,i2,i3) +dxc(io)*u(i1o,i2o,i3o,pc)*drid
         enddo
      else

      !  fill in unused points with the identity
         dl1(i1,i2,i3) = 0d0
         d(i1,i2,i3)   = 1d0
         du1(i1,i2,i3) = 0d0
         if ( is_penta ) then
            dl2(i1,i2,i3) = 0d0
            du2(i1,i2,i3) = 0d0
         endif
         if ( mode.eq.solve_rhs .or. mode.eq.solve_lhs ) then
            rhs(i1,i2,i3) = ul(i1,i2,i3,cc)
         else
            rhs(i1,i2,i3) = 0d0
         endif

         if ( mask(i1,i2,i3).eq.0) rhs(i1,i2,i3) = 0d0

      endif ! if mask gt 0

      if ( dir.eq.0) then
      if ( idir.eq.(index_range(1,dir)-ilo(dir)) ) then
         idir = index_range(0,dir)+ilo(dir)
      else
         idir = idir + 1
      endif 
      endif

      enddo ! i1l

      if ( dir.eq.1) then
      if ( idir.eq.(index_range(1,dir)-ilo(dir)) ) then
         idir = index_range(0,dir)+ilo(dir)
      else
         idir = idir + 1
      endif 
      endif

      enddo ! i2l

      if ( dir.eq.2) then
      if ( idir.eq.(index_range(1,dir)-ilo(dir)) ) then
         idir = index_range(0,dir)+ilo(dir)
      else
         idir = idir + 1
      endif 
      endif

      enddo ! i3l

      if (mode.eq.get_explicit ) then
         if ( disc_approx.eq.compact .and. .not.is_periodic) then
            ONE_SIDED_BDY(pc)
c            EXTRAPOLATE_GHOST()
         endif
      else
c         if ( advection_coefficient .gt. 1e-10 ) then
         APPLY_BOUNDARY_CONDITIONS()
c         else
c            IDENTITY_GHOST()
c         endif
      endif

      end ! subroutine ins_mfactor
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! subroutine ins_rfactor
INS_FACTOR_SUBROUTINE(ins_fscoeff)
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      double precision b_ik(-half_width:half_width) !! this holds the variable pde coefficients at adjacent grid points
      double precision a_ik(-half_width:half_width) !! this holds the variable pde coefficients at adjacent grid points
      integer io !! loop variable for looping over adjacent grid points in the stencil (i.e. from -half_width to half_width)
      integer idir !! index counter in the direction of the implicit solve
      integer width
      logical interp_bc
      double precision pxc,dxc,dxxc
      pxc(io) = px_coeff(io)
      dxc(io) = dx_coeff(io)
      dxxc(io)= dxx_coeff(io)

      INS_FACTOR_EXTRACT_PARAM()

      is_penta = (order.gt.2 .and. disc_approx.eq.compact)
      have_ghost_points = .false.
      width = 2*half_width
      
      PRINT_DEBUG_INFO(ins_fscoeff)

      INIT_STENCIL_SHIFT()

!!kkc 120508      idir = index_range(0,dir)+ilo(dir)
!!      do i3l=index_range(0,2)+ilo3,index_range(1,2)-ilo3
!!      do i2l=index_range(0,1)+ilo2,index_range(1,1)-ilo2
!!      do i1l=index_range(0,0)+ilo1,index_range(1,0)-ilo1
       idir = gir(0,dir)
       do i3l=gir(0,2)+ilo3,gir(1,2)-ilo3
       do i2l=gir(0,1)+ilo2,gir(1,1)-ilo2
       do i1l=gir(0,0)+ilo1,gir(1,0)-ilo1

      SET_STENCIL_IDX()
      if ( mask(i1,i2,i3).ne.0 ) then

         if ( mask(i1,i2,i3).gt.0 ) then
            if ( mask(i1m,i2m,i3m).lt.0 .or. mask(i1mm,i2mm,i3mm).lt.0 .or. mask(i1p,i2p,i3p).lt.0 .or. mask(i1pp,i2pp,i3pp).lt.0 ) then
               call get_op_coeffs(half_width,1,min(order,4),finite_difference,px_coeff,dx_coeff)
               call get_op_coeffs(half_width,2,min(order,4),finite_difference,pxx_coeff,dxx_coeff)
!!            else if ( is_periodic .or. (idir.gt.(index_range(0,dir)+2) .and. idir.lt.(index_range(1,dir)-2))) then
            else if ( is_periodic .or. (idir.gt.(gir(0,dir)+2) .and. idir.lt.(gir(1,dir)-2))) then
               call get_mop_coeffs(half_width,1,order,disc_approx,px_coeff,dx_coeff)
               call get_mop_coeffs(half_width,2,order,disc_approx,pxx_coeff,dxx_coeff)
               off =  0
            else if ( .not. is_periodic ) then
               call get_op_coeffs(half_width,1,order,finite_difference,px_coeff,dx_coeff)
               call get_op_coeffs(half_width,2,order,finite_difference,pxx_coeff,dxx_coeff)
               off =  0
            else if ( (idir.eq.(gir(0,dir)+2) .or. idir.eq.(gir(1,dir)-2))) then
               call get_op_coeffs(half_width,1,order,disc_approx,px_coeff,dx_coeff)
               call get_op_coeffs(half_width,2,order,disc_approx,pxx_coeff,dxx_coeff)
               off =  0
            else if (idir.eq.(gir(0,dir)+1)) then
               call get_bos_mop_coeffs(half_width,1,0,order,disc_approx,px_coeff,dx_coeff)
               call get_bos_mop_coeffs(half_width,2,0,order,disc_approx,pxx_coeff,dxx_coeff)
               off = -1
            else if ( idir.eq.(gir(1,dir)-1) ) then
               call get_bos_mop_coeffs(half_width,1,1,order,disc_approx,px_coeff,dx_coeff)
               call get_bos_mop_coeffs(half_width,2,1,order,disc_approx,pxx_coeff,dxx_coeff)
               off =  1
            elseif ( idir.le.(gir(0,dir)+1) ) then
               call get_bos_mop_coeffs(half_width,1,0,order,disc_approx,px_coeff,dx_coeff)
               call get_bos_mop_coeffs(half_width,2,0,order,disc_approx,pxx_coeff,dxx_coeff)
               off = -1
            else if ( idir.ge.(gir(1,dir)-1) ) then
               call get_bos_mop_coeffs(half_width,1,1,order,disc_approx,px_coeff,dx_coeff)
               call get_bos_mop_coeffs(half_width,2,1,order,disc_approx,pxx_coeff,dxx_coeff)
               off =  1
            endif
         else 
            call get_op_coeffs(half_width,1,min(order,4),finite_difference,px_coeff,dx_coeff)
            call get_op_coeffs(half_width,2,min(order,4),finite_difference,pxx_coeff,dxx_coeff)
            off = 0
         endif

         use_os_rxr = .true.
         EVALUATE_JACOBIAN_DERIVATIVES(half_width)

         !!! here we assume there is always space for the first upper and lower diagonals
         !!! also, we assume that this is a "merged" coefficient routine so that px_coeff == pxx_coeff 
         dl1(i1,i2,i3) = pxc(off-1)
         d(i1,i2,i3)   = pxc(off+0)
         du1(i1,i2,i3) = pxc(off+1)
         if ( is_penta ) then
            if (off.ne.-1) then
               dl2(i1,i2,i3) = pxc(off-2)
            else
               dl2(i1,i2,i3) = pxc(off+3)
            endif

            if (off.ne.1) then
               du2(i1,i2,i3) = pxc(off+2)
            else
               du2(i1,i2,i3) = pxc(off-3)
            endif
         endif
            
         !!! fill in the variable coefficient at each point to be used in later loops over discretization coefficients
         do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
            a_ik(io) = 0d0
            b_ik(io) = 0d0
            GET_SHIFTED_IDX(i1o,i2o,i3o, (io-off)*is1, (io-off)*is2, (io-off)*is3)
            do id=0,nd-1
               b_ik(io) = b_ik(io) + rx(io,dir,id)*rx(io,dir,id)
               a_ik(io) = a_ik(io) + u(i1o,i2o,i3o,uc+id)*rx(io,dir,id)*advection_coefficient - nu_eq*rxx(io,dir,id)
            enddo
            b_ik(io) = -nu_eq*b_ik(io)

            if (is_moving) then
               do id=0,nd-1
                  a_ik(io) = a_ik(io) - gv(i1o,i2o,i3o,id)*rx(io,dir,id)
               enddo
            endif
         enddo

         rhs(i1,i2,i3) = 0d0
         do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
                                !!!                                 D_r a_ik - nu_eq D_rr b_ik
c            GET_SHIFTED_IDX(i1o,i2o,i3o, (io-off)*is1, (io-off)*is2, (io-off)*is3)
c         !!!! The - sign before the second term is due to the way we have implemented the curvilinear grid correction
            rhs(i1,i2,i3) = rhs(i1,i2,i3) + drid*dxc(io)*a_ik(io) - drid*drid*dxxc(io)*b_ik(io) 
         enddo
      else

      !  fill in unused points with the identity
         dl1(i1,i2,i3) = 0d0
         d(i1,i2,i3)   = 1d0
         du1(i1,i2,i3) = 0d0
         if ( is_penta ) then
            dl2(i1,i2,i3) = 0d0
            du2(i1,i2,i3) = 0d0
         endif
         rhs(i1,i2,i3) = 0d0 

      endif ! if mask ne 0

      if ( dir.eq.0) then
      if ( idir.eq.(gir(1,dir)-ilo(dir)) ) then
         idir = gir(0,dir)+ilo(dir)
      else
         idir = idir + 1
      endif 
      endif

      enddo ! i1l

      if ( dir.eq.1) then
      if ( idir.eq.(gir(1,dir)-ilo(dir)) ) then
         idir = gir(0,dir)+ilo(dir)
      else
         idir = idir + 1
      endif 
      endif

      enddo ! i2l

      if ( dir.eq.2) then
      if ( idir.eq.(gir(1,dir)-ilo(dir)) ) then
         idir = gir(0,dir)+ilo(dir)
      else
         idir = idir + 1
      endif 
      endif

      enddo ! i3l

      ! this will actually extrapolate the boundary point but is only 4th order accurate for penta-diagonal solves
      if ( .not. is_periodic ) then
         bc_range(0,0,dir) = index_range(0,dir)
         bc_range(0,1,dir) = gir(0,dir)-1
         bc_range(1,0,dir) = gir(1,dir)+1
         bc_range(1,1,dir) = index_range(1,dir)
         EXTRAPOLATE_GHOST() 
      endif

      end ! subroutine ins_fscoeff
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! subroutine ins_evalux
INS_FACTOR_SUBROUTINE(ins_evalux)
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      integer io !! loop variable for looping over adjacent grid points in the stencil (i.e. from -half_width to half_width)
      integer idir !! index counter in the direction of the implicit solve
      integer width
      logical interp_bc
      double precision pxc,dxc,dxxc
      pxc(io) = px_coeff(io)
      dxc(io) = dx_coeff(io)
      dxxc(io)= dxx_coeff(io)

      INS_FACTOR_EXTRACT_PARAM()

      is_penta = (order.gt.2 .and. disc_approx.eq.compact)
      have_ghost_points = .false.
      width = 2*half_width
      
      PRINT_DEBUG_INFO(ins_evalux)

      INIT_STENCIL_SHIFT()

      idir = index_range(0,dir)+ilo(dir)
      do i3l=index_range(0,2)+ilo3,index_range(1,2)-ilo3
      do i2l=index_range(0,1)+ilo2,index_range(1,1)-ilo2
      do i1l=index_range(0,0)+ilo1,index_range(1,0)-ilo1
!!!      idir = gir(0,dir)
!!!      do i3l=gir(0,2),gir(1,2)
!!!      do i2l=gir(0,1),gir(1,1)
!!!      do i1l=gir(0,0),gir(1,0)

      SET_STENCIL_IDX()
      
      if ( mask(i1,i2,i3).ne.0 ) then

         if ( mask(i1,i2,i3).gt.0 ) then
            if ( is_periodic .or. (idir.ge.(gir(0,dir)+2) .and. idir.le.(gir(1,dir)-2))) then
               call get_mop_coeffs(half_width,1,order,disc_approx,px_coeff,dx_coeff)
               call get_mop_coeffs(half_width,2,order,disc_approx,pxx_coeff,dxx_coeff)
               off =  0
            elseif ( idir.le.(gir(0,dir)+1) ) then
               call get_bos_mop_coeffs(half_width,1,0,order,disc_approx,px_coeff,dx_coeff)
               call get_bos_mop_coeffs(half_width,2,0,order,disc_approx,pxx_coeff,dxx_coeff)
               off = -1
            else if ( idir.ge.(gir(1,dir)-1) ) then
               call get_bos_mop_coeffs(half_width,1,1,order,disc_approx,px_coeff,dx_coeff)
               call get_bos_mop_coeffs(half_width,2,1,order,disc_approx,pxx_coeff,dxx_coeff)
               off =  1
            elseif ( .not. is_periodic ) then
               call get_op_coeffs(half_width,1,order,finite_difference,px_coeff,dx_coeff)
               call get_op_coeffs(half_width,2,order,finite_difference,pxx_coeff,dxx_coeff)
               off =  0
            endif
         else
            call get_op_coeffs(half_width,1,min(order,4),finite_difference,px_coeff,dx_coeff)
            call get_op_coeffs(half_width,2,min(order,4),finite_difference,pxx_coeff,dxx_coeff)
            off = 0
         endif

!!! NOT NEEDED FOR PARAMETRIC DERIVATIVE 100929         EVALUATE_JACOBIAN_DERIVATIVES(half_width)

         !!! here we assume there is always space for the first upper and lower diagonals
         !!! also, we assume that this is a "merged" coefficient routine so that px_coeff == pxx_coeff 
         dl1(i1,i2,i3) = pxc(off-1)
         d(i1,i2,i3)   = pxc(off+0)
         du1(i1,i2,i3) = pxc(off+1)
         if ( is_penta ) then
            if (off.ne.-1) then
               dl2(i1,i2,i3) = pxc(off-2)
            else
               dl2(i1,i2,i3) = pxc(off+3)
            endif

            if (off.ne.1) then
               du2(i1,i2,i3) = pxc(off+2)
            else
               du2(i1,i2,i3) = pxc(off-3)
            endif
         endif
            
         rhs(i1,i2,i3) = 0d0
         do io=-half_width,half_width 
            GET_SHIFTED_IDX(i1o,i2o,i3o, (io-off)*is1, (io-off)*is2, (io-off)*is3)
            rhs(i1,i2,i3) = rhs(i1,i2,i3) + drid*dxc(io)*u(i1o,i2o,i3o,cc)
         enddo

      else
      !  fill in unused points with the identity
         dl1(i1,i2,i3) = 0d0
         d(i1,i2,i3)   = 1d0
         du1(i1,i2,i3) = 0d0
         if ( is_penta ) then
            dl2(i1,i2,i3) = 0d0
            du2(i1,i2,i3) = 0d0
         endif
         rhs(i1,i2,i3) = 0d0 


      endif ! if mask gt 0

      if ( dir.eq.0) then
      if ( idir.eq.(index_range(1,dir)-ilo(dir)) ) then
         idir = index_range(0,dir)+ilo(dir)
      else
         idir = idir + 1
      endif 
      endif

      enddo ! i1l

      if ( dir.eq.1) then
      if ( idir.eq.(index_range(1,dir)-ilo(dir)) ) then
         idir = index_range(0,dir)+ilo(dir)
      else
         idir = idir + 1
      endif 
      endif

      enddo ! i2l

      if ( dir.eq.2) then
      if ( idir.eq.(index_range(1,dir)-ilo(dir)) ) then
         idir = index_range(0,dir)+ilo(dir)
      else
         idir = idir + 1
      endif 
      endif

      enddo ! i3l

      if ( .not. is_periodic ) then

         if ( disc_approx.eq.compact .and. .not.is_periodic) then
         bc_range(0,0,dir) = index_range(0,dir)
         bc_range(0,1,dir) = gir(0,dir)-1
         bc_range(1,0,dir) = gir(1,dir)+1
         bc_range(1,1,dir) = index_range(1,dir)

!!XX            bc_range(0,0,dir) = index_range(0,dir)
!!XX            bc_range(0,1,dir) = index_range(0,dir)!!index_range(0,dir)-1
!!XX            bc_range(1,0,dir) = index_range(1,dir)!!index_range(1,dir)+1
!!XX            bc_range(1,1,dir) = index_range(1,dir)
            ONE_SIDED_BDY(pc)
         endif
      !! ONE_SIDED_BDY(cc)
      endif

      end ! subroutine ins_evalux
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



