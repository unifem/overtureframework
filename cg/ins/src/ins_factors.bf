#Include "ins_factors.bh"
#beginMacro ONE_SIDED_GHOST(CC)
      do side=0,1

      call get_os_op_coeffs(half_width,1,side,order,disc_approx,px_coeff,dx_coeff)

      do i3l=bc_range(side,0,2),bc_range(side,1,2)
      do i2l=bc_range(side,0,1),bc_range(side,1,1)
      do i1l=bc_range(side,0,0),bc_range(side,1,0)
         SET_STENCIL_IDX()

         if ( side.eq.0 ) then
            d(i1,i2,i3) = px_coeff(-half_width+0)
            du1(i1,i2,i3) = px_coeff(-half_width+1)
            if ( .not. is_penta ) then
               dl1(i1,i2,i3) = px_coeff(-half_width+2)
            else
               du2(i1,i2,i3) = px_coeff(-half_width+2)
               dl2(i1,i2,i3) = px_coeff(-half_width+3)
               dl1(i1,i2,i3) = px_coeff(-half_width+4)
            endif
         else
            d(i1,i2,i3) = px_coeff(-half_width+2*half_width)
            dl1(i1,i2,i3) = px_coeff(-half_width+2*half_width-1)
            if ( .not. is_penta ) then
               du1(i1,i2,i3) = px_coeff(-half_width+2*half_width-2)
            else
               dl2(i1,i2,i3) = px_coeff(-half_width+2*half_width-2)
               du2(i1,i2,i3) = px_coeff(-half_width+2*half_width-3)
               du1(i1,i2,i3) = px_coeff(-half_width+0)
            endif
         endif
         rhs(i1,i2,i3) = 0d0
         do io=0,2*half_width
            GET_SHIFTED_IDX(i1o,i2o,i3o, (1-2*side)*io*is1, (1-2*side)*io*is2, (1-2*side)*io*is3)
            rhs(i1,i2,i3) = rhs(i1,i2,i3) + u(i1o,i2o,i3o,CC)*dx_coeff(-half_width + side*2*half_width + (1-2*side)*io)*drid
         enddo
      enddo
      enddo
      enddo
      enddo
#endMacro

      subroutine get_os_op_coeffs(hw,nder,side,order,type,P,D)
      implicit none

      integer nder,side,order,type,hw
      double precision P(0:*),D(0:*)
      
      double precision alpha,beta,a,b ! correspond to Lele's notation
      DEFINE_PARAMETERS()
      integer i

      do i = 0,2*hw
         P(i) = 0d0
         D(i) = 0d0
      enddo

      if ( nder.eq.1 ) then
         if ( type.eq.compact ) then
            if ( order.eq. 2 ) then
               if (side.eq.0) then
                  P(0) = 1d0
                  P(1) = 1d0
                  D(0) = -2d0
                  D(1) =  2d0
               else
                  P(2*hw)   = 1d0
                  P(2*hw-1) = 1d0
                  D(2*hw)   = 2d0
                  D(2*hw-1) = -2d0
               endif
            else if ( order.eq.4 .or. order.eq.6) then 
!!! lower order accurate for 6th!!!
               if ( side.eq.0 ) then
                  D(0) = -0.17e2 / 0.6e1
                  D(1) = 0.3e1 / 0.2e1
                  D(2) = 0.3e1 / 0.2e1
                  D(3) = -0.1e1 / 0.6e1
                  D(4) = 0.0e0
                  P(0) = 1d0
                  P(1) = 3d0
               else
                  D(2*hw) =  0.17e2 / 0.6e1
                  D(2*hw-1) = -0.3e1 / 0.2e1
                  D(2*hw-2) = -0.3e1 / 0.2e1
                  D(2*hw-3)   = 0.1e1 / 0.6e1
                  P(2*hw)   = 1d0
                  P(2*hw-1) = 3d0
               endif
            else ! order .gt. 6
               stop 99502
            endif
         else ! type .ne. compact
            stop 99501
         endif
      else !nder .ne.1
         stop 99500
      endif
      
      return
      end 

      subroutine get_op_coeffs(hw,nder,order,type,P,D)
      implicit none

      integer nder,order,type,hw
      double precision P(-hw:*),D(-hw:*)
      
      double precision alpha,beta,a,b ! correspond to Lele's notation
      DEFINE_PARAMETERS()

      integer i
      
      alpha = 0d0
      beta  = 0d0
      a = 0d0
      b = 0d0
      do i=-hw,hw
         P(i) = 0d0
         D(i) = 0d0
      end do

      if ( type.eq.finite_difference ) then

         P(0) = 1d0
         if ( order.eq.2 ) then
            if ( nder.eq.1 ) then
               D(-1) = -0.5d0
               D( 0) =  0.0d0
               D( 1) =  0.5d0
            else if ( nder.eq.2 ) then
               D(-1) =  1d0
               D( 0) = -2d0
               D( 1) =  1d0
            else
               stop 99000
            endif ! nder
         else if (order.ge.4) then
            if ( nder.eq.1 ) then
               D(-2) =  1d0/12d0
               D(-1) = -2d0/3d0
               D( 0) =  0d0
               D( 1) =  2d0/3d0
               D( 2) = -1d0/12d0
            else if ( nder.eq.2 ) then
               D(-2) = -1d0 /12d0
               D(-1) =  4d0 /3d0
               D( 0) = -15d0/6d0
               D( 1) =  4d0 /3d0
               D( 2) = -1d0 /12d0
            else
               stop 99001
            endif ! nder
         else
            stop 99002
         endif ! order

      else if (type.eq.compact) then

         if ( order.eq.2 ) then
            ! b = alpha = beta = 0
            if ( nder.eq.1 .or. nder.eq.2 ) then
               a = 1d0
            else
               stop 99003
            endif ! nder
         else if (order.eq.4) then
            if ( nder.eq.1 ) then
               alpha = 0.25d0
               a = 2d0*(alpha + 2d0)/3d0
            else if ( nder.eq.2 ) then
               alpha = 0.1d0
               a = 4d0*(1d0-alpha)/3d0
            else
               stop 99004
            endif ! nder
         else if (order.eq.6) then
            if ( nder.eq.1 ) then
               alpha = 1d0/3d0
               a = 14d0/9d0
               b = 1d0/9d0
            else if ( nder.eq.2 ) then
               alpha = 2d0/11d0
               a = 12d0/11d0
               b = 3d0/11d0
            else
               stop 99005
            endif               ! nder
         else if (order.eq.8) then
            if ( nder.eq.1 ) then
               alpha = 4d0/9d0
               beta = 1d0/36d0
               a = 40d0/27d0
               b = 25d0/54d0
            else if ( nder.eq.2 ) then
               alpha = 344d0/1179d0
               beta  = (38d0*alpha-9d0)/214d0
               a = (696d0 - 1191d0*alpha)/428d0
               b = (2454d0*alpha - 294d0)/535d0
            else
               stop 99006
            endif               ! nder
         else
            stop 99007
         endif ! order
         
         P(-2) = beta
         P(-1) = alpha
         P( 0) = 1d0
         P( 1) = alpha
         P( 2) = beta

         if ( nder.eq.1 ) then
            D(-2) = -b/4d0
            D(-1) = -a/2d0
            D( 0) =  0d0
            D( 1) =  a/2d0
            D( 2) =  b/4d0
         else
            D(-2) =  b/4d0
            D(-1) =  a
            D( 0) =  -2d0*( b/4d0 + a )
            D( 1) =  a
            D( 2) =  b/4d0
         endif
         
      else ! unknown difference type
         stop 99008
      endif ! type

      return
      end

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! subroutine ins_rfactor
INS_FACTOR_SUBROUTINE(ins_rfactor)
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      double precision a_ik(-half_width:half_width) !! this holds the variable pde coefficients at adjacent grid points
      integer io !! loop variable for looping over adjacent grid points in the stencil (i.e. from -half_width to half_width)

      INS_FACTOR_EXTRACT_PARAM()

      PRINT_DEBUG_INFO(ins_rfactor)

!!actually, do the pressure gradient in the explicit case      if ( mode.eq.get_explicit ) return ! there are no explicit contributions from this factor

      call get_op_coeffs(half_width,1,order,disc_approx,px_coeff,dx_coeff)

      INIT_STENCIL_SHIFT()

!      do i3l=i3s,i3e
!      do i2l=i2s,i2e
!      do i1l=i1s,i1e
      do i3l=index_range(0,2),index_range(1,2)
      do i2l=index_range(0,1),index_range(1,1)
      do i1l=index_range(0,0),index_range(1,0)

      SET_STENCIL_IDX()
      if ( mask(i1,i2,i3).gt.0 ) then

         EVALUATE_JACOBIAN_DERIVATIVES(half_width)

         !!!x here we assume there is always space for the first upper and lower diagonals
         dl1(i1,i2,i3) = px_coeff(-1)
         d(i1,i2,i3)   = px_coeff( 0)
         du1(i1,i2,i3) = px_coeff( 1)
         if ( is_penta ) then
            dl2(i1,i2,i3) = px_coeff(-2)
            du2(i1,i2,i3) = px_coeff( 2)
         endif
            
         !!! fill in the variable coefficient at each point to be used in later loops over discretization coefficients
         if ( mode.ne.get_explicit ) then
            do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
               a_ik(io) = 0d0
               GET_SHIFTED_IDX(i1o,i2o,i3o, io*is1, io*is2, io*is3)
               do id=0,nd-1
                  a_ik(io) = a_ik(io) + u(i1o,i2o,i3o,uc+id)*rx(io,dir,id) - nu_eq*rxx(io,dir,id)
!!linear                  a_ik(io) = a_ik(io) + rx(io,dir,id) - nu_eq*rxx(io,dir,id)
               enddo

               a_ik(io) = a_ik(io)*advection_coefficient
!               if ( i1.eq.0 .and. i2.eq.i1 ) print *,i1o,i2o,i3o,u(i1o,i2o,i3o,uc),u(i1o,i2o,i3o,vc),rx(io,dir,0),rx(io,dir,1),a_ik(io)
!               if ( i1.eq.2 .and. i2.eq.i1 .and. cc.eq.2 ) print *,i1o,i2o,ul(i1o,i2o,i3o,uc),ul(i1o,i2o,i3o,vc),rx(io,dir,0),rx(io,dir,1),a_ik(io)
            enddo
         endif !mode.ne.get_explicit

         if ( mode.eq.solve_rhs ) then
            rhs(i1,i2,i3) = 0d0
            do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
               !!!                                 (P  - (dt/2) D_r a_ik) U^* 
               GET_SHIFTED_IDX(i1o,i2o,i3o, io*is1, io*is2, io*is3)
               rhs(i1,i2,i3) = rhs(i1,i2,i3) + (px_coeff(io) - dto2*drid*dx_coeff(io)*a_ik(io))*ul(i1o,i2o,i3o,cc)
            enddo

         else if ( mode.eq.solve_lhs ) then
            rhs(i1,i2,i3) = 0d0
            do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
               !!!                                 P U^{*}
               GET_SHIFTED_IDX(i1o,i2o,i3o, io*is1, io*is2, io*is3)
               rhs(i1,i2,i3) = rhs(i1,i2,i3) + px_coeff(io)*ul(i1o,i2o,i3o,cc)
            enddo
               !!!                                 (P + (dt/2) D_r a_ik) U^{**}_i
            io = -1
            dl1(i1,i2,i3)    = dl1(i1,i2,i3) + dto2*drid*dx_coeff(io)*a_ik(io)
            io = 0
            d(i1,i2,i3)      = d(i1,i2,i3)   + dto2*drid*dx_coeff(io)*a_ik(io)
            io = 1
            du1(i1,i2,i3)    = du1(i1,i2,i3) + dto2*drid*dx_coeff(io)*a_ik(io)
            if ( is_penta ) then
               io = -2
               dl2(i1,i2,i3) = dl2(i1,i2,i3) + dto2*drid*dx_coeff(io)*a_ik(io)
               io =  2
               du2(i1,i2,i3) = du2(i1,i2,i3) + dto2*drid*dx_coeff(io)*a_ik(io)
            endif
         else ! mode .eq. get_explicit
            ! this is a basic compact scheme for the pressure, the contribution will be scaled and added to the appropriate equation
            !   outside this subroutine
            !! dp/dx_i = sum_{j=0}^{ndim-1} dr_j/dx_i dp/dr_j
            !! dr_j/dx_i dp/dr_j = dr_j/dx_i P^{-1} D p
            ! note we filled in the diagonals above and the bcs are taken care of below...
            rhs(i1,i2,i3) = 0d0
            do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
               GET_SHIFTED_IDX(i1o,i2o,i3o, io*is1, io*is2, io*is3)
               rhs(i1,i2,i3) = rhs(i1,i2,i3) + dx_coeff(io)*u(i1o,i2o,i3o,pc)*drid
            enddo

         endif ! if mode .eq. solve_rhs or solve_lhs

      endif ! if mask gt 0

      enddo ! i1l
      enddo ! i2l
      enddo ! i3l

      if (mode.eq.get_explicit ) then
         if ( disc_approx.eq.compact ) then
            ONE_SIDED_GHOST(pc)
c            EXTRAPOLATE_GHOST()
         endif
      else
c         if ( advection_coefficient .gt. 1e-10 ) then
            APPLY_BOUNDARY_CONDITIONS()
c         else
c            IDENTITY_GHOST()
c         endif
      endif

      end ! subroutine ins_rfactor
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! subroutine ins_rrfactor
INS_FACTOR_SUBROUTINE(ins_rrfactor)
      double precision b_ik(-half_width:half_width) !! this holds the variable pde coefficients at adjacent grid points
      integer io !! loop variable for looping over adjacent grid points in the stencil (i.e. from -half_width to half_width)

      INS_FACTOR_EXTRACT_PARAM()

      PRINT_DEBUG_INFO(ins_rrfactor)

      call get_op_coeffs(half_width,2,order,disc_approx,
     &                   pxx_coeff,dxx_coeff)

      INIT_STENCIL_SHIFT()

!      do i3l=i3s,i3e
!      do i2l=i2s,i2e
!      do i1l=i1s,i1e
      do i3l=index_range(0,2),index_range(1,2)
      do i2l=index_range(0,1),index_range(1,1)
      do i1l=index_range(0,0),index_range(1,0)

      SET_STENCIL_IDX()
      if ( mask(i1,i2,i3).gt.0 ) then

         EVALUATE_JACOBIAN_DERIVATIVES(half_width)
         !!! fill in the variable coefficient at each point to be used in later loops over discretization coefficients
         do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
            b_ik(io) = 0d0
            do id=0,nd-1
               b_ik(io) = b_ik(io) + rx(io,dir,id)*rx(io,dir,id)
            enddo
            b_ik(io) = 1d0/(b_ik(io)) ! take the reciprocal here instead of dividing everywhere in the loops below
         enddo

         if ( mode.eq.solve_rhs .or. mode.eq.solve_lhs ) then
         !!!x here we assume there is always space for the first upper and lower diagonals
         dl1(i1,i2,i3) = pxx_coeff(-1)*b_ik(-1)
         d(i1,i2,i3)   = pxx_coeff( 0)*b_ik( 0)
         du1(i1,i2,i3) = pxx_coeff( 1)*b_ik( 1)
         if ( is_penta ) then
            dl2(i1,i2,i3) = pxx_coeff(-2)*b_ik(-2)
            du2(i1,i2,i3) = pxx_coeff( 2)*b_ik( 2)
         endif
         endif ! if one of the matrix fill-in modes

         if ( mode.eq.solve_rhs ) then
            rhs(i1,i2,i3) = 0d0
            do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
               !!!                                 (P b_ik^{-1}  - (-nu)*(dt/2) D_rr) ( U^* )
               GET_SHIFTED_IDX(i1o,i2o,i3o, io*is1, io*is2, io*is3)
               rhs(i1,i2,i3) = rhs(i1,i2,i3) + (pxx_coeff(io)*b_ik(io) + nu_eq*dto2*drid*drid*dxx_coeff(io))*ul(i1o,i2o,i3o,cc)
            enddo
         else if ( mode.eq.solve_lhs ) then
            rhs(i1,i2,i3) = 0d0
            do io=-half_width,half_width !!!x this loop may need to be changed if the grid does not have ghost points with sufficient width
               !!!                                 P b_ik^{-1} U^{*}
               GET_SHIFTED_IDX(i1o,i2o,i3o, io*is1, io*is2, io*is3)
               rhs(i1,i2,i3) = rhs(i1,i2,i3) + pxx_coeff(io)*ul(i1o,i2o,i3o,cc)*b_ik(io)
            enddo
               !!!                                 (P b_ik^{-1} + (-nu)*(dt/2) D_rr) U_i 
            io = -1
            dl1(i1,i2,i3)    = dl1(i1,i2,i3) - nu_eq*dto2*drid*drid*dxx_coeff(io)
            io = 0
            d(i1,i2,i3)      = d(i1,i2,i3)   - nu_eq*dto2*drid*drid*dxx_coeff(io)
            io = 1
            du1(i1,i2,i3)    = du1(i1,i2,i3) - nu_eq*dto2*drid*drid*dxx_coeff(io)
            if ( is_penta ) then
               io = -2
               dl2(i1,i2,i3) = dl2(i1,i2,i3) - nu_eq*dto2*drid*drid*dxx_coeff(io)
               io =  2
               du2(i1,i2,i3) = du2(i1,i2,i3) - nu_eq*dto2*drid*drid*dxx_coeff(io)
            endif

         else if ( mode.eq.get_explicit ) then
            rhs(i1,i2,i3) = 0d0
         endif ! if mode = solve_rhs or solve_lhs or get_explicit

      endif ! if mask gt 0

      enddo ! i1l
      enddo ! i2l
      enddo ! i3l

      if (mode.ne.get_explicit) then
c      if ( nu .gt. 1e-10 ) then
         APPLY_BOUNDARY_CONDITIONS()
c      else
c         IDENTITY_GHOST()
c      endif
      endif

      end ! subroutine ins_rrfactor
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! subroutine ins_diagfactor
INS_FACTOR_SUBROUTINE(ins_diagfactor)

      integer io !! loop variable for looping over adjacent grid points in the stencil (i.e. from -half_width to half_width)

      INS_FACTOR_EXTRACT_PARAM()

      is_penta = (order.gt.2 .and. disc_approx.eq.compact) !! FOR COMBINED/MERGED FACTORS ONLY

      PRINT_DEBUG_INFO(ins_diagfactor)
      INIT_STENCIL_SHIFT()

!      do i3l=i3s,i3e
!      do i2l=i2s,i2e
!      do i1l=i1s,i1e
      do i3l=index_range(0,2),index_range(1,2)
      do i2l=index_range(0,1),index_range(1,1)
      do i1l=index_range(0,0),index_range(1,0)

      SET_STENCIL_IDX()
!we can do this on all points      if ( mask(i1,i2,i3).gt.0 ) then


         !!!x here we assume there is always space for the first upper and lower diagonals
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
 !     endif ! if mask gt 0

      enddo ! i1l
      enddo ! i2l
      enddo ! i3l

      if (mode.ne.get_explicit) then
c      if ( nu .gt. 1e-10 ) then
c         APPLY_BOUNDARY_CONDITIONS()
      else
c         IDENTITY_GHOST()
c      endif
      endif

c      IDENTITY_GHOST()

      end ! subroutine ins_diagfactor
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
