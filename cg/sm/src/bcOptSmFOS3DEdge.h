c*******
c******* Fix up components of stress along the edges
c*******
      if( .false. )then ! *wdh* 090910 -- turn this off for now  --------------------
       
        write(*,'(" bcOptSmFOS3DEdge: do NOT apply edge fixup, grid,gridType=",i4,i2)')  grid,gridType

      else

       ! write(*,'(" bcOptSmFOS3DEdge: DO apply edge fixup, grid,gridType=",i4,i2)')  grid,gridType 

      beginEdgeMacro()
        if( bc1.eq.displacementBC .and. bc2.eq.displacementBC ) then
          if( gridType.eq.rectangular ) then
            beginLoopsMask3d()
c              if( mask(i1,i2,i3).gt.0 ) then
c                write(6,*)i1,i2,i3
                u1x = diffr1(uc,dx(0))
                u1y = diffr2(uc,dx(1))
                u1z = diffr3(uc,dx(2))

                u2x = diffr1(vc,dx(0))
                u2y = diffr2(vc,dx(1))
                u2z = diffr3(vc,dx(2))

                u3x = diffr1(wc,dx(0))
                u3y = diffr2(wc,dx(1))
                u3z = diffr3(wc,dx(2))

                u(i1,i2,i3,s11c) = kappa*u1x+lambda*(u2y+u3z)
                u(i1,i2,i3,s21c) = mu*(u2x+u1y)
                u(i1,i2,i3,s31c) = mu*(u3x+u1z)
                u(i1,i2,i3,s12c) = mu*(u2x+u1y)
                u(i1,i2,i3,s22c) = kappa*u2y+lambda*(u1x+u3z)
                u(i1,i2,i3,s32c) = mu*(u3y+u2z)
                u(i1,i2,i3,s13c) = mu*(u3x+u1z)
                u(i1,i2,i3,s23c) = mu*(u3y+u2z)
                u(i1,i2,i3,s33c) = kappa*u3z+lambda*(u1x+u2y)
c              end if
            endLoopsMask3d()
          else
            beginLoopsMask3d()
c              if( mask(i1,i2,i3).gt.0 ) then
                u1r = diffr1(uc,dr(0))
                u1s = diffr2(uc,dr(1))
                u1t = diffr3(uc,dr(2))

                u2r = diffr1(vc,dr(0))
                u2s = diffr2(vc,dr(1))
                u2t = diffr3(vc,dr(2))

                u3r = diffr1(wc,dr(0))
                u3s = diffr2(wc,dr(1))
                u3t = diffr3(wc,dr(2))

                u1x = u1r*rx(i1,i2,i3,0,0)+u1s*rx(i1,i2,i3,1,0)+u1t*rx(i1,i2,i3,2,0)
                u1y = u1r*rx(i1,i2,i3,0,1)+u1s*rx(i1,i2,i3,1,1)+u1t*rx(i1,i2,i3,2,1)
                u1z = u1r*rx(i1,i2,i3,0,2)+u1s*rx(i1,i2,i3,1,2)+u1t*rx(i1,i2,i3,2,2)

                u2x = u2r*rx(i1,i2,i3,0,0)+u2s*rx(i1,i2,i3,1,0)+u2t*rx(i1,i2,i3,2,0)
                u2y = u2r*rx(i1,i2,i3,0,1)+u2s*rx(i1,i2,i3,1,1)+u2t*rx(i1,i2,i3,2,1)
                u2z = u2r*rx(i1,i2,i3,0,2)+u2s*rx(i1,i2,i3,1,2)+u2t*rx(i1,i2,i3,2,2)

                u3x = u3r*rx(i1,i2,i3,0,0)+u3s*rx(i1,i2,i3,1,0)+u3t*rx(i1,i2,i3,2,0)
                u3y = u3r*rx(i1,i2,i3,0,1)+u3s*rx(i1,i2,i3,1,1)+u3t*rx(i1,i2,i3,2,1)
                u3z = u3r*rx(i1,i2,i3,0,2)+u3s*rx(i1,i2,i3,1,2)+u3t*rx(i1,i2,i3,2,2)

                u(i1,i2,i3,s11c) = kappa*u1x+lambda*(u2y+u3z)
                u(i1,i2,i3,s21c) = mu*(u2x+u1y)
                u(i1,i2,i3,s31c) = mu*(u3x+u1z)
                u(i1,i2,i3,s12c) = mu*(u2x+u1y)
                u(i1,i2,i3,s22c) = kappa*u2y+lambda*(u1x+u3z)
                u(i1,i2,i3,s32c) = mu*(u3y+u2z)
                u(i1,i2,i3,s13c) = mu*(u3x+u1z)
                u(i1,i2,i3,s23c) = mu*(u3y+u2z)
                u(i1,i2,i3,s33c) = kappa*u3z+lambda*(u1x+u2y)
c              end if
            endLoopsMask3d()
          end if ! gridType

	  ! *wdh* we need to adjust for TZ here (not in a separate edgeMacro loop for then
	  ! corner points are corrected twice)
          if( twilightZone.ne.0 ) then 
           beginLoopsMask3d()
c            if( mask(i1,i2,i3).gt.0 ) then
              call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,u1x )
              call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,u1y )
              call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,u1z )
              
              call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,u2x )
              call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,u2y )
              call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,u2z )
              
              call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,u3x )
              call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,u3y )
              call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,u3z )
              
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s11c,s11e )
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s12c,s12e )
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s13c,s13e )
              
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s21c,s21e )
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s22c,s22e )
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s23c,s23e )
              
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s31c,s31e )
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s32c,s32e )
              call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s33c,s33e )
              
              tau11 = kappa*u1x+lambda*(u2y+u3z)
              tau21 = mu*(u2x+u1y)
              tau31 = mu*(u3x+u1z)
              tau12 = tau21
              tau22 = kappa*u2y+lambda*(u1x+u3z)
              tau32 = mu*(u3y+u2z)
              tau13 = tau31
              tau23 = tau32
              tau33 = kappa*u3z+lambda*(u1x+u2y)

              u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)-tau11+s11e
              u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)-tau21+s21e
              u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)-tau31+s31e
              					       					      
              u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)-tau12+s12e
              u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)-tau22+s22e
              u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)-tau32+s32e
              					       					      
              u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)-tau13+s13e
              u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)-tau23+s23e
              u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)-tau33+s33e

c              u(i1,i2,i3,s11c) = s11e
c              u(i1,i2,i3,s21c) = s21e
c              u(i1,i2,i3,s31c) = s31e
c              			 				      
c              u(i1,i2,i3,s12c) = s12e
c              u(i1,i2,i3,s22c) = s22e
c              u(i1,i2,i3,s32c) = s32e
c              			 				      
c              u(i1,i2,i3,s13c) = s13e
c              u(i1,i2,i3,s23c) = s23e
c              u(i1,i2,i3,s33c) = s33e
c            end if ! end if mask
           endLoopsMask3d()
	  end if ! end if TZ


        else if( bc1.eq.tractionBC .and. bc2.eq.tractionBC ) then
          if( gridType.eq.rectangular ) then
            ! do nothing because normals are perpendicular and so no part of the force is counted twice
          else
            ! do stuff because normals are not perpendicular and some part of the stress might be counted twice
            beginLoopsMask3d()
              if( edgeDirection.eq.0 ) then
                norm1(0) = -is2*rx(i1,i2,i3,1,0)
                norm1(1) = -is2*rx(i1,i2,i3,1,1)
                norm1(2) = -is2*rx(i1,i2,i3,1,2)
                f11 = bcf(side2,1,i1,i2,i3,s11c)
                f21 = bcf(side2,1,i1,i2,i3,s12c)
                f31 = bcf(side2,1,i1,i2,i3,s13c)

                norm2(0) = -is3*rx(i1,i2,i3,2,0)
                norm2(1) = -is3*rx(i1,i2,i3,2,1)
                norm2(2) = -is3*rx(i1,i2,i3,2,2)
                f12 = bcf(side3,2,i1,i2,i3,s11c)
                f22 = bcf(side3,2,i1,i2,i3,s12c)
                f32 = bcf(side3,2,i1,i2,i3,s13c)
              else if( edgeDirection.eq.1 ) then
                norm1(0) = -is3*rx(i1,i2,i3,2,0)
                norm1(1) = -is3*rx(i1,i2,i3,2,1)
                norm1(2) = -is3*rx(i1,i2,i3,2,2)
                f11 = bcf(side3,2,i1,i2,i3,s11c)
                f21 = bcf(side3,2,i1,i2,i3,s12c)
                f31 = bcf(side3,2,i1,i2,i3,s13c)

                norm2(0) = -is1*rx(i1,i2,i3,0,0)
                norm2(1) = -is1*rx(i1,i2,i3,0,1)
                norm2(2) = -is1*rx(i1,i2,i3,0,2)
                f12 = bcf(side1,0,i1,i2,i3,s11c)
                f22 = bcf(side1,0,i1,i2,i3,s12c)
                f32 = bcf(side1,0,i1,i2,i3,s13c)
              else
                norm1(0) = -is1*rx(i1,i2,i3,0,0)
                norm1(1) = -is1*rx(i1,i2,i3,0,1)
                norm1(2) = -is1*rx(i1,i2,i3,0,2)
                f11 = bcf(side1,0,i1,i2,i3,s11c)
                f21 = bcf(side1,0,i1,i2,i3,s12c)
                f31 = bcf(side1,0,i1,i2,i3,s13c)
                
                norm2(0) = -is2*rx(i1,i2,i3,1,0)
                norm2(1) = -is2*rx(i1,i2,i3,1,1)
                norm2(2) = -is2*rx(i1,i2,i3,1,2)
                f12 = bcf(side2,1,i1,i2,i3,s11c)
                f22 = bcf(side2,1,i1,i2,i3,s12c)
                f32 = bcf(side2,1,i1,i2,i3,s13c)
              end if

              aNormi = 1.0/max(epsx,sqrt(norm1(0)**2+norm1(1)**2+norm1(2)**2))
              norm1(0) = norm1(0)*aNormi
              norm1(1) = norm1(1)*aNormi
              norm1(2) = norm1(2)*aNormi
              
              aNormi = 1.0/max(epsx,sqrt(norm2(0)**2+norm2(1)**2+norm2(2)**2))
              norm2(0) = norm2(0)*aNormi
              norm2(1) = norm2(1)*aNormi
              norm2(2) = norm2(2)*aNormi

              b11 = f11-(norm1(0)*u(i1,i2,i3,s11c)+norm1(1)*u(i1,i2,i3,s21c)+norm1(2)*u(i1,i2,i3,s31c))
              b21 = f21-(norm1(0)*u(i1,i2,i3,s12c)+norm1(1)*u(i1,i2,i3,s22c)+norm1(2)*u(i1,i2,i3,s32c))
              b31 = f31-(norm1(0)*u(i1,i2,i3,s13c)+norm1(1)*u(i1,i2,i3,s23c)+norm1(2)*u(i1,i2,i3,s33c))

              dot1 = norm1(0)*norm2(0)+norm1(1)*norm2(1)+norm1(2)*norm2(2)
              dot2 = -sin(acos(dot1))

              b12 = (f12-(norm2(0)*u(i1,i2,i3,s11c)+norm2(1)*u(i1,i2,i3,s21c)+norm2(2)*u(i1,i2,i3,s31c))-dot1*b11)/dot2
              b22 = (f22-(norm2(0)*u(i1,i2,i3,s12c)+norm2(1)*u(i1,i2,i3,s22c)+norm2(2)*u(i1,i2,i3,s32c))-dot1*b21)/dot2
              b32 = (f32-(norm2(0)*u(i1,i2,i3,s13c)+norm2(1)*u(i1,i2,i3,s23c)+norm2(2)*u(i1,i2,i3,s33c))-dot1*b31)/dot2

              u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)+norm1(0)*b11+norm2(0)*b12
              u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)+norm1(0)*b21+norm2(0)*b22
              u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)+norm1(0)*b31+norm2(0)*b32

              u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)+norm1(1)*b11+norm2(1)*b12
              u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)+norm1(1)*b21+norm2(1)*b22
              u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)+norm1(1)*b31+norm2(1)*b32

              u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)+norm1(2)*b11+norm2(2)*b12
              u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)+norm1(2)*b21+norm2(2)*b22
              u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)+norm1(2)*b31+norm2(2)*b32
            endLoopsMask3d()
          end if ! gridType

        end if ! bcType
      endEdgeMacro()

      end if 
