c*******
c******* Fill in forcing arrays if they are not provided ***********
c*******
      beginLoopOverSides( numGhost,numGhost )
        if( boundaryCondition(side,axis).eq.displacementBC )then
          if( addBoundaryForcing(side,axis).eq.0 ) then
            beginGhostLoopsMask3d()  ! *wdh* we can assign dirichlet like conditions at ghost/imter points too
                ! given displacements
                bcfa(side,axis,i1,i2,i3,uc) = 0.0
                bcfa(side,axis,i1,i2,i3,vc) = 0.0
                bcfa(side,axis,i1,i2,i3,wc) = 0.0
                ! given velocities
                bcfa(side,axis,i1,i2,i3,v1c) = 0.0
                bcfa(side,axis,i1,i2,i3,v2c) = 0.0
                bcfa(side,axis,i1,i2,i3,v3c) = 0.0
                ! given acceleration
                bcfa(side,axis,i1,i2,i3,s11c) = 0.0
                bcfa(side,axis,i1,i2,i3,s12c) = 0.0
                bcfa(side,axis,i1,i2,i3,s13c) = 0.0
            endLoopsMask3d()
          else if( twilightZone.ne.0 ) then
            beginGhostLoopsMask3d()
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,ue )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,ve )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,we )
                  
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v1c,v1e )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v2c,v2e )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v3c,v3e )
                  
                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s11c,tau11x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s21c,tau21y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s31c,tau31z )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s12c,tau12x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s22c,tau22y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s32c,tau32z )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s13c,tau13x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s23c,tau23y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s33c,tau33z )

                bcfa(side,axis,i1,i2,i3,uc) = ue
                bcfa(side,axis,i1,i2,i3,vc) = ve
                bcfa(side,axis,i1,i2,i3,wc) = we

                bcfa(side,axis,i1,i2,i3,v1c) = v1e
                bcfa(side,axis,i1,i2,i3,v2c) = v2e
                bcfa(side,axis,i1,i2,i3,v3c) = v3e
                  
                bcfa(side,axis,i1,i2,i3,s11c) = tau11x+tau21y+tau31z
                bcfa(side,axis,i1,i2,i3,s12c) = tau12x+tau22y+tau32z
                bcfa(side,axis,i1,i2,i3,s13c) = tau13x+tau23y+tau33z
            endLoopsMask3d()
          end if
        else if( boundaryCondition(side,axis).eq.tractionBC ) then
          if( addBoundaryForcing(side,axis).eq.0 )then
            beginGhostLoopsMask3d()
                ! given traction (for the traction BC)
                bcfa(side,axis,i1,i2,i3,s11c) = 0.0  
                bcfa(side,axis,i1,i2,i3,s12c) = 0.0
                bcfa(side,axis,i1,i2,i3,s13c) = 0.0

                ! given traction (for determining displacements). Normally this is equal to the above
                ! traction values except when using twilight-zone
                bcfa(side,axis,i1,i2,i3,uc) = 0.0   
                bcfa(side,axis,i1,i2,i3,vc) = 0.0
                bcfa(side,axis,i1,i2,i3,wc) = 0.0

                ! given rate of change of traction (for determining the velocity)
                bcfa(side,axis,i1,i2,i3,v1c) = 0.0
                bcfa(side,axis,i1,i2,i3,v2c) = 0.0
                bcfa(side,axis,i1,i2,i3,v3c) = 0.0
            endLoopsMask3d()
          else if( twilightZone.ne.0 )then
            beginGhostLoopsMask3d()
                ! (an1,an2,an3) = outward normal
                if( gridType.eq.rectangular )then
                  if( axis.eq.0 )then
                    an1 = -is
                    an2 = 0.0
                    an3 = 0.0
                  else if( axis.eq.1 ) then
                    an1 = 0.0
                    an2 = -is
                    an3 = 0.0
                  else 
                    an1 = 0.0
                    an2 = 0.0
                    an3 = -is
                  end if
                else
                  aNormi = 1.0/max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                  an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                  an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                  an3 = -is*rx(i1,i2,i3,axis,2)*aNormi
                end if
                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,u1x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,u1y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,u1z )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,u2x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,u2y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,u2z )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,u3x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,u3y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,u3z )
                
                bcfa(side,axis,i1,i2,i3,uc) = an1*( kappa*u1x+lambda*(u2y+u3z) ) + an2*( mu*(u2x+u1y) )               + an3*( mu*(u3x+u1z) )
                bcfa(side,axis,i1,i2,i3,vc) = an1*( mu*(u2x+u1y) )               + an2*( kappa*u2y+lambda*(u1x+u3z) ) + an3*( mu*(u3y+u2z) )
                bcfa(side,axis,i1,i2,i3,wc) = an1*( mu*(u3x+u1z) )               + an2*( mu*(u3y+u2z) )               + an3*( kappa*u3z+lambda*(u1x+u2y) )
                
                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v1c,v1x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v1c,v1y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v1c,v1z )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v2c,v2x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v2c,v2y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v2c,v2z )

                call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v3c,v3x )
                call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v3c,v3y )
                call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v3c,v3z )
                
                bcfa(side,axis,i1,i2,i3,v1c) = an1*( kappa*v1x+lambda*(v2y+v3z) ) + an2*( mu*(v2x+v1y) )                + an3*( mu*(v3x+v1z) )
                bcfa(side,axis,i1,i2,i3,v2c) = an1*( mu*(v2x+v1y) )               + an2*( kappa*v2y+lambda*(v1x+v3z) )  + an3*( mu*(v3y+v2z) )
                bcfa(side,axis,i1,i2,i3,v3c) = an1*( mu*(v3x+v1z) )               + an2*( mu*(v3y+v2z) )                + an3*( kappa*v3z+lambda*(v1x+v2y) )

                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s11c,tau11 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s21c,tau21 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s31c,tau31 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s12c,tau12 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s22c,tau22 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s32c,tau32 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s13c,tau13 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s23c,tau23 )
                call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s33c,tau33 )
                
                ! note : n_j sigma_ji  : sum over first index 
                bcfa(side,axis,i1,i2,i3,s11c) = an1*tau11+an2*tau21+an3*tau31 
                bcfa(side,axis,i1,i2,i3,s12c) = an1*tau12+an2*tau22+an3*tau32 
                bcfa(side,axis,i1,i2,i3,s13c) = an1*tau13+an2*tau23+an3*tau33 

            endLoopsMask3d()

          else
            ! fill in the traction BC into the stress components  
            ! (this is needed since for TZ flow these values are different)
            beginGhostLoopsMask3d()
                bcfa(side,axis,i1,i2,i3,s11c) = bcfa(side,axis,i1,i2,i3,uc)
                bcfa(side,axis,i1,i2,i3,s12c) = bcfa(side,axis,i1,i2,i3,vc)
                bcfa(side,axis,i1,i2,i3,s13c) = bcfa(side,axis,i1,i2,i3,wc)
            endLoopsMask3d()
          end if

        else if( boundaryCondition(side,axis).eq.slipWall ) then
          if( addBoundaryForcing(side,axis).eq.0 ) then
            beginGhostLoopsMask3d()
                !! check these components with Bill ... FIX ME!! ...
                ! given tangential stresses (often zero)
                bcfa(side,axis,i1,i2,i3,s11c) = 0.0
                bcfa(side,axis,i1,i2,i3,s12c) = 0.0

                ! time rate of change of tangential stresses
                bcfa(side,axis,i1,i2,i3,s21c) = 0.0
                bcfa(side,axis,i1,i2,i3,s22c) = 0.0

                ! given normal displacement
                bcfa(side,axis,i1,i2,i3,uc) = 0.0

                ! time rate of change of normal displacement
                bcfa(side,axis,i1,i2,i3,v1c) = 0.0
            endLoopsMask3d()
          else if( twilightZone.ne.0 ) then
            beginGhostLoopsMask3d()
                ! (an1,an2,an3) = outward normal
                if( gridType.eq.rectangular ) then
                  an1 = 0.0
                  an2 = 0.0
                  an3 = 0.0

                  sn1 = 0.0
                  sn2 = 0.0
                  sn3 = 0.0

                  tn1 = 0.0
                  tn2 = 0.0
                  tn3 = 0.0
                  if( axis.eq.0 ) then
                    an1 = -is
                    sn2 = -is
                    tn3 = -is
                  else if( axis.eq.1 ) then
                    an2 = -is
                    sn1 = -is
                    tn3 = -is
                  else 
                    an3 = -is
                    sn1 = -is
                    tn2 = -is
                  end if
                else
                  if( axis.eq.0 ) then
                    tan1c = 1
                    tan2c = 2
                  else if( axis.eq.1 ) then
                    tan1c = 0
                    tan2c = 2
                  else
                    tan1c = 0
                    tan2c = 1
                  end if
                  aNormi = 1.0/max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                  an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                  an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                  an3 = -is*rx(i1,i2,i3,axis,2)*aNormi

                  sn1 = rx(i1,i2,i3,tan1c,0)
                  sn2 = rx(i1,i2,i3,tan1c,1)
                  sn3 = rx(i1,i2,i3,tan1c,2)

                  tn1 = rx(i1,i2,i3,tan2c,0)
                  tn2 = rx(i1,i2,i3,tan2c,1)
                  tn3 = rx(i1,i2,i3,tan2c,2)

                  ! set sn to be part of sn which is orthogonal to an
                  alpha = an1*sn1+an2*sn2+an3*sn3
                  sn1 = sn1-alpha*an1
                  sn2 = sn2-alpha*an2
                  sn3 = sn3-alpha*an3
                  ! normalize sn
                  aNormi = 1.0/max(epsx,sqrt(sn1**2+sn2**2+sn3**2))
                  sn1 = sn1*aNormi
                  sn2 = sn2*aNormi
                  sn3 = sn3*aNormi

                  ! set tn to be part of tn which is orthogonal to an and sn
                  alpha = an1*tn1+an2*tn2+an3*tn3
                  tn1 = tn1-alpha*an1
                  tn2 = tn2-alpha*an2
                  tn3 = tn3-alpha*an3
                  alpha = sn1*tn1+sn2*tn2+sn3*tn3
                  tn1 = tn1-alpha*sn1
                  tn2 = tn2-alpha*sn2
                  tn3 = tn3-alpha*sn3
                  ! normalize tn
                  aNormi = 1.0/max(epsx,sqrt(tn1**2+tn2**2+tn3**2))
                  tn1 = tn1*aNormi
                  tn2 = tn2*aNormi
                  tn3 = tn3*aNormi
                end if

                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,ue)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,ve)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,we)

                bcfa(side,axis,i1,i2,i3,uc) = an1*ue+an2*ve+an3*we

                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v1c,ue)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v2c,ve)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v3c,we)

                bcfa(side,axis,i1,i2,i3,v1c) = an1*ue+an2*ve+an3*we

                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s11c,tau11)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s21c,tau21)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s31c,tau31)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s12c,tau12)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s22c,tau22)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s32c,tau32)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s13c,tau13)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s23c,tau23)
                call ogDeriv(ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s33c,tau33)

                ! check indicies ... FIX ME!! ...
                bcfa(side,axis,i1,i2,i3,s11c) = an1*(sn1*tau11+sn2*tau12+sn3*tau13)+ \
                                                an2*(sn1*tau21+sn2*tau22+sn3*tau23)+ \
                                                an3*(sn1*tau31+sn2*tau32+sn3*tau33)
                bcfa(side,axis,i1,i2,i3,s12c) = an1*(tn1*tau11+tn2*tau12+tn3*tau13)+ \
                                                an2*(tn1*tau21+tn2*tau22+tn3*tau23)+ \
                                                an3*(tn1*tau31+tn2*tau32+tn3*tau33)

                call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,u1x)
                call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,u1y)
                call ogDeriv(ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,u1z)
                call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,u2x)
                call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,u2y)
                call ogDeriv(ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,u2z)
                call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,u3x)
                call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,u3y)
                call ogDeriv(ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,u3z)
                
                tau11 = kappa*u1x+lambda*(u2y+u3z)
                tau21 = mu*(u2x+u1y)
                tau31 = mu*(u3x+u1z)
                tau12 = tau21
                tau22 = kappa*u2y+lambda*(u1x+u3z)
                tau32 = mu*(u3y+u2z)
                tau13 = tau31
                tau23 = tau32
                tau33 = kappa*u3z+lambda*(u1x+u2y)
ccccccccccccccccccccccccccccccccccccccccccccccccccc

                bcfa(side,axis,i1,i2,i3,s21c) = an1*(-an2*tau11+an1*tau12)+an2*(-an2*tau21+an1*tau22)

                call ogDeriv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s11c,tau11)
                call ogDeriv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s21c,tau21)
                call ogDeriv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s12c,tau12)
                call ogDeriv(ep,1,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s22c,tau22)
                
                bcfa(side,axis,i1,i2,i3,s12c) = an1*(-an2*tau11+an1*tau12)+an2*(-an2*tau21+an1*tau22)
                
                call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v1c,v1ex)
                call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v1c,v1ey)
                call ogDeriv(ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v2c,v2ex)
                call ogDeriv(ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v2c,v2ey)
                
                tau11=(lambda+2.*mu)*v1ex+lambda*v2ey
                tau12=mu*(v1ey+v2ex)
                tau21=tau12
                tau22=lambda*v1ex+(lambda+2.*mu)*v2ey
                bcfa(side,axis,i1,i2,i3,s22c) = an1*(-an2*tau11+an1*tau12)+an2*(-an2*tau21+an1*tau22)

            endLoopsMask3d()
          else
            ! fill in the traction BC into the stress components  *wdh* 081109
            ! (this is needed since for TZ flow these values are different)
            beginGhostLoopsMask3d()
                bcfa(side,axis,i1,i2,i3,s11c) = bcfa(side,axis,i1,i2,i3,uc)
                bcfa(side,axis,i1,i2,i3,s12c) = bcfa(side,axis,i1,i2,i3,vc)
            endLoopsMask3d()
          end if

        else if( boundaryCondition(side,axis).gt.0 .and. boundaryCondition(side,axis).ne.dirichletBoundaryCondition ) then
	  write(*,'("smg3d:BC: unknown BC: side,axis,grid, boundaryCondition=",i2,i2,i4,i8)') side,axis,grid,boundaryCondition(side,axis)
        end if
      endLoopOverSides()

c*******
c******* Primary Dirichlet boundary conditions ***********
c*******
      beginLoopOverSides( numGhost,numGhost )
        if( boundaryCondition(side,axis).eq.displacementBC )then
          ! ..step 0: Dirichlet bcs for displacement and velocity
          beginGhostLoopsMask3d()
              ! given displacements
              u(i1,i2,i3,uc)  = bcf(side,axis,i1,i2,i3,uc) 
              u(i1,i2,i3,vc)  = bcf(side,axis,i1,i2,i3,vc)
              u(i1,i2,i3,wc)  = bcf(side,axis,i1,i2,i3,wc)

              ! given velocities
              u(i1,i2,i3,v1c) = bcf(side,axis,i1,i2,i3,v1c) 
              u(i1,i2,i3,v2c) = bcf(side,axis,i1,i2,i3,v2c)
              u(i1,i2,i3,v3c) = bcf(side,axis,i1,i2,i3,v3c)
          endLoopsMask3d()
        else if( boundaryCondition(side,axis).eq.tractionBC )then
          ! dirichlet portion of traction BC
          if( gridType.eq.rectangular ) then
            if( axis.eq.0 )then
              beginGhostLoopsMask3d()
                  ! set normal components of the stress, n=(-is,0,0)
                  u(i1,i2,i3,s11c) = -is*bcf(side,axis,i1,i2,i3,s11c) 
                  u(i1,i2,i3,s12c) = -is*bcf(side,axis,i1,i2,i3,s12c)
                  u(i1,i2,i3,s13c) = -is*bcf(side,axis,i1,i2,i3,s13c)
              endLoopsMask3d()
            else if( axis.eq.1 ) then
              beginGhostLoopsMask3d()
                  ! set normal components of the stress, n=(0,-is,0)
                  u(i1,i2,i3,s21c) = -is*bcf(side,axis,i1,i2,i3,s11c) 
                  u(i1,i2,i3,s22c) = -is*bcf(side,axis,i1,i2,i3,s12c)
                  u(i1,i2,i3,s23c) = -is*bcf(side,axis,i1,i2,i3,s13c)
              endLoopsMask3d()
            else 
              beginGhostLoopsMask3d()
                  ! set normal components of the stress, n=(0,0,-is)
                  u(i1,i2,i3,s31c) = -is*bcf(side,axis,i1,i2,i3,s11c) 
                  u(i1,i2,i3,s32c) = -is*bcf(side,axis,i1,i2,i3,s12c)
                  u(i1,i2,i3,s33c) = -is*bcf(side,axis,i1,i2,i3,s13c)
              endLoopsMask3d()
            end if

          else ! curvilinear 
            beginGhostLoopsMask3d()
                f1 = bcf(side,axis,i1,i2,i3,s11c)
                f2 = bcf(side,axis,i1,i2,i3,s12c)
                f3 = bcf(side,axis,i1,i2,i3,s13c)

                ! (an1,an2,an3) = outward normal 
                aNormi = 1.0/max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                an3 = -is*rx(i1,i2,i3,axis,2)*aNormi
                
                b1 = f1-(an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,i3,s21c)+an3*u(i1,i2,i3,s31c))
                b2 = f2-(an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,i3,s22c)+an3*u(i1,i2,i3,s32c))
                b3 = f3-(an1*u(i1,i2,i3,s13c)+an2*u(i1,i2,i3,s23c)+an3*u(i1,i2,i3,s33c))

                u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)+an1*b1
                u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)+an1*b2
                u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)+an1*b3

                u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)+an2*b1
                u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)+an2*b2
                u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)+an2*b3

                u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)+an3*b1
                u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)+an3*b2
                u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)+an3*b3
            endLoopsMask3d()
          end if ! grid type

        else if( boundaryCondition(side,axis).eq.slipWall ) then
           ! ********* SlipWall BC ********
           ! set "dirichlet" parts of the slipwall BC
          if( gridType.eq.rectangular ) then
            if( axis.eq.0 ) then
              beginGhostLoopsMask3d()
                  ! set n.tau.t and the normal component of displacement, n=(-is,0,0)
                  u(i1,i2,i3,s12c) = bcf(side,axis,i1,i2,i3,s11c)
                  u(i1,i2,i3,s13c) = bcf(side,axis,i1,i2,i3,s12c)
                  u(i1,i2,i3,uc)   = -is*bcf(side,axis,i1,i2,i3,uc)
                  u(i1,i2,i3,v1c)  = -is*bcf(side,axis,i1,i2,i3,v1c)
              endLoopsMask3d()
            else if( axis.eq.1 ) then
              beginGhostLoopsMask3d()
                  ! set n.tau.t and the normal component of displacement, n=(-is,0,0)
                  u(i1,i2,i3,s21c) = bcf(side,axis,i1,i2,i3,s11c)
                  u(i1,i2,i3,s23c) = bcf(side,axis,i1,i2,i3,s12c)
                  u(i1,i2,i3,vc)   = -is*bcf(side,axis,i1,i2,i3,uc)
                  u(i1,i2,i3,v2c)  = -is*bcf(side,axis,i1,i2,i3,v1c)
              endLoopsMask3d()
            else
              beginGhostLoopsMask3d()
                  ! set n.tau.t and the normal component of displacement, n=(-is,0,0)
                  u(i1,i2,i3,s31c) = bcf(side,axis,i1,i2,i3,s11c)
                  u(i1,i2,i3,s32c) = bcf(side,axis,i1,i2,i3,s12c)
                  u(i1,i2,i3,wc)   = -is*bcf(side,axis,i1,i2,i3,uc)
                  u(i1,i2,i3,v3c)  = -is*bcf(side,axis,i1,i2,i3,v1c)
              endLoopsMask3d()
            end if

          else  ! curvilinear 
            beginGhostLoopsMask3d()
                ! given tangential traction forces
                f1 = bcf(side,axis,i1,i2,i3,s11c) 
                f2 = bcf(side,axis,i1,i2,i3,s12c) 

                ! (an1,an2,an3) = outward normal 
                if( axis.eq.0 ) then
                  tan1c = 1
                  tan2c = 2
                else if( axis.eq.1 ) then
                  tan1c = 0
                  tan2c = 2
                else
                  tan1c = 0
                  tan2c = 1
                end if
                aNormi = 1.0/max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                an3 = -is*rx(i1,i2,i3,axis,2)*aNormi

                sn1 = rx(i1,i2,i3,tan1c,0)
                sn2 = rx(i1,i2,i3,tan1c,1)
                sn3 = rx(i1,i2,i3,tan1c,2)

                tn1 = rx(i1,i2,i3,tan2c,0)
                tn2 = rx(i1,i2,i3,tan2c,1)
                tn3 = rx(i1,i2,i3,tan2c,2)

                ! set sn to be part of sn which is orthogonal to an
                alpha = an1*sn1+an2*sn2+an3*sn3
                sn1 = sn1-alpha*an1
                sn2 = sn2-alpha*an2
                sn3 = sn3-alpha*an3
                ! normalize sn
                aNormi = 1.0/max(epsx,sqrt(sn1**2+sn2**2+sn3**2))
                sn1 = sn1*aNormi
                sn2 = sn2*aNormi
                sn3 = sn3*aNormi

                ! set tn to be part of tn which is orthogonal to an and sn
                alpha = an1*tn1+an2*tn2+an3*tn3
                tn1 = tn1-alpha*an1
                tn2 = tn2-alpha*an2
                tn3 = tn3-alpha*an3
                alpha = sn1*tn1+sn2*tn2+sn3*tn3
                tn1 = tn1-alpha*sn1
                tn2 = tn2-alpha*sn2
                tn3 = tn3-alpha*sn3
                ! normalize tn
                aNormi = 1.0/max(epsx,sqrt(tn1**2+tn2**2+tn3**2))
                tn1 = tn1*aNormi
                tn2 = tn2*aNormi
                tn3 = tn3*aNormi

                b1 = f1-an1*(u(i1,i2,i3,s11c)*sn1+u(i1,i2,i3,s12c)*sn2+u(i1,i2,i3,s13c)*sn3)- \
                        an2*(u(i1,i2,i3,s21c)*sn1+u(i1,i2,i3,s22c)*sn2+u(i1,i2,i3,s23c)*sn3)- \
                        an3*(u(i1,i2,i3,s31c)*sn1+u(i1,i2,i3,s32c)*sn2+u(i1,i2,i3,s33c)*sn3)
                b2 = f2-an1*(u(i1,i2,i3,s11c)*tn1+u(i1,i2,i3,s12c)*tn2+u(i1,i2,i3,s13c)*tn3)- \
                        an2*(u(i1,i2,i3,s21c)*tn1+u(i1,i2,i3,s22c)*tn2+u(i1,i2,i3,s23c)*tn3)- \
                        an3*(u(i1,i2,i3,s31c)*tn1+u(i1,i2,i3,s32c)*tn2+u(i1,i2,i3,s33c)*tn3)


                u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)+an1*b1*sn1+an1*b2*tn1
                u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)+an1*b1*sn2+an1*b2*tn2
                u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)+an1*b1*sn3+an1*b2*tn3

                u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)+an2*b1*sn1+an2*b2*tn1
                u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)+an2*b1*sn2+an2*b2*tn2
                u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)+an2*b1*sn3+an2*b2*tn3

                u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)+an3*b1*sn1+an3*b2*tn1
                u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)+an3*b1*sn2+an3*b2*tn2
                u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)+an3*b1*sn3+an3*b2*tn3


                ! given normal displacement
                f1 = bcf(side,axis,i1,i2,i3,uc) 

                ! given normal velocity
                f2 = bcf(side,axis,i1,i2,i3,v1c) 

                b1 = f1-an1*u(i1,i2,i3,uc)-an2*u(i1,i2,i3,vc)-an3*u(i1,i2,i3,wc)
                b2 = f2-an1*u(i1,i2,i3,v1c)-an2*u(i1,i2,i3,v2c)-an3*u(i1,i2,i3,v3c)

                u(i1,i2,i3,uc) = u(i1,i2,i3,uc)+an1*b1
                u(i1,i2,i3,vc) = u(i1,i2,i3,vc)+an2*b1
                u(i1,i2,i3,wc) = u(i1,i2,i3,wc)+an3*b1

                u(i1,i2,i3,v1c) = u(i1,i2,i3,v1c)+an1*b2
                u(i1,i2,i3,v2c) = u(i1,i2,i3,v2c)+an2*b2
                u(i1,i2,i3,v3c) = u(i1,i2,i3,v3c)+an3*b2
            endLoopsMask3d()

          end if  ! end gridType

        else if( boundaryCondition(side,axis).gt.0 .and. boundaryCondition(side,axis).ne.dirichletBoundaryCondition ) then
	  write(*,'("smg3d:BC: unknown BC: side,axis,grid, boundaryCondition=",i2,i2,i4,i8)') side,axis,grid,boundaryCondition(side,axis)

        end if ! bc type
      endLoopOverSides()

c*******
c******* Extrapolate to the first ghost cells (only for physical sides) ********
c*******
      ! *wdh* For now assign 2 ghost lines and points outside edges and corners
      extra = 2 
      numGhostExtrap=2
      beginLoopOverSides( extra,numGhostExtrap )
        if( boundaryCondition(side,axis).gt.0.and.boundaryCondition(side,axis).ne.dirichletBoundaryCondition )then

       if( .false. )then
         write(*,'(" bcOpt: Extrap ghost: grid,side,axis=",3i3,", \
           loop bounds: nn1a,nn1b,nn2a,nn2b,nn3a,nn3b=",6i3)') grid,side,axis,\
           nn1a,nn1b,nn2a,nn2b,nn3a,nn3b

       end if

          beginGhostLoops3d()
          if( mask(i1,i2,i3).ne.0 ) then
              do n=0,numberOfComponents-1
                u(i1-is1,i2-is2,i3-is3,n)=extrap3(u,i1,i2,i3,n,is1,is2,is3)
                u(i1-2*is1,i2-2*is2,i3-2*is3,n)=extrap3(u,i1-is1,i2-is2,i3-is3,n,is1,is2,is3)
              end do
            end if
          endLoops3d()
        end if
      endLoopOverSides()

c*******
c******* Fix up components of stress along the edges
c*******
      #Include 'bcOptSmFOS3DEdge.h'

cccccccccccccccccccccccccccccccccccccccccccccccccc
c .. set exact solution for corners for now
      if( setCornersWithTZ .and. twilightZone.ne.0 ) then ! *wdh* 090909
       write(*,'(" bcOptSmFOS3D: INFO set exact values on corners")')
      beginLoopOverCorners3d(0)
        if( mask(i1,i2,i3).gt.0 ) then
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,ue )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,ve )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,we )
              
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v1c,v1e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v2c,v2e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v3c,v3e )
              
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s11c,tau11e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s21c,tau21e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s31c,tau31e )
              
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s12c,tau12e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s22c,tau22e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s32c,tau32e )
              
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s13c,tau13e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s23c,tau23e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s33c,tau33e )

          u(i1,i2,i3,uc) = ue
          u(i1,i2,i3,vc) = ve
          u(i1,i2,i3,wc) = we

          u(i1,i2,i3,v1c) = v1e
          u(i1,i2,i3,v2c) = v2e
          u(i1,i2,i3,v3c) = v3e

          u(i1,i2,i3,s11c) = tau11e
          u(i1,i2,i3,s21c) = tau21e
          u(i1,i2,i3,s31c) = tau31e

          u(i1,i2,i3,s12c) = tau12e
          u(i1,i2,i3,s22c) = tau22e
          u(i1,i2,i3,s32c) = tau32e

          u(i1,i2,i3,s13c) = tau13e
          u(i1,i2,i3,s23c) = tau23e
          u(i1,i2,i3,s33c) = tau33e
        end if
      endLoopOverCorners3d()
      end if
cccccccccccccccccccccccccccccccccccccccccccccccccc
      
c*******
c******* Fix up components of stress in the corners 
c*******
c.. for now we are going to ignore this too

c*******
c******* Secondary Neumann boundary conditions (compatibility conditions) ********
c*******
      beginLoopOverSides(numGhost,numGhost)

        if( boundaryCondition(side,axis).eq.displacementBC ) then

          if( gridType.eq.rectangular ) then

            ! ********* DISPLACEMENT : Cartesian Grid **********

            ! Use momentum equations ... 
            !   s11_x + s21_y + s31_z = rho * u_tt  
            !   s12_x + s22_y + s32_z = rho * v_tt  
            !   s13_x + s23_y + s33_z = rho * w_tt  
            ! *wdh* 090909 -- only assign pts where mask > 0 since we assume values at adjacent points.
            beginLoopsMask3d()
                accel(1) = rho*bcf(side,axis,i1,i2,i3,s11c)
                accel(2) = rho*bcf(side,axis,i1,i2,i3,s12c)
                accel(3) = rho*bcf(side,axis,i1,i2,i3,s13c)

c                write(6,*)is1,is2,is3,is,axis
c                write(6,*)accel(1),accel(2),accel(3)

                do isc = 1,3
                  u(i1-is1,i2-is2,i3-is3,sc(axis+1,isc)) = u(i1+is1,i2+is2,i3+is3,sc(axis+1,isc))-2.0*is*dx(axis)*(accel(isc)- \
                        (1.0-delta(axis+1,1))*(u(i1+1,i2,i3,sc(1,isc))-u(i1-1,i2,i3,sc(1,isc)))/(2.0*dx(0))- \
                        (1.0-delta(axis+1,2))*(u(i1,i2+1,i3,sc(2,isc))-u(i1,i2-1,i3,sc(2,isc)))/(2.0*dx(1))- \
                        (1.0-delta(axis+1,3))*(u(i1,i2,i3+1,sc(3,isc))-u(i1,i2,i3-1,sc(3,isc)))/(2.0*dx(2)))
                end do
            endLoopsMask3d()
          else

            if( .false. ) then ! non-free stream preserving method
            ! *********** DISPLACEMENT : Curvilinear Grid (not free stream preserving) ****************
 
            ! Use momentum equations to get J*(rx,ry,rz).(s11,s21,s31)(-1) = s11tilde
            !    (1)   D_r1[ J*(rx,ry,rz).(s11,s21,s31)] + D_r2[J*(sx,sy,sz).(s11,s21,s31)] + D_r3[J*(tx,ty,tz).(s11,s21,s31)] = J * rho * u_tt  
            !                        s11tilde                         s21tilde                          s31tilde
            !    (2)   Use extrapolated values to get  J*(sx,sy,sz).(s11,s21,s31)(-1) = s21tilde
            !    (3)   Use extrapolated values to get  J*(tx,ty,tz).(s11,s21,s31)(-1) = s31tilde
            ! To give 3 equations for (s11,s21,s31) on the ghost point:
            !   (J rx) s11(-1) + (J ry) s21(-1) + (J rz) s31(-1) = f1 = s11tilde  (from momentum eqn)
            !   (J sx) s11(-1) + (J sy) s21(-1) + (J sz) s31(-1) = f2 = s21tilde  (from extrapolated values)
            !   (J tx) s11(-1) + (J ty) s21(-1) + (J tz) s31(-1) = f3 = s31tilde  (from extrapolated values)
            ! Solve: (note the Jacobian cancels when the matrix inversion is determined)
            !     s11(-1) = (sy*tz-sz*ty)*f1 + (rz*ty-ry*tz)*f2 + (ry*sz-rz*sy)*f3
            !     s21(-1) = (tx*sz-sx*tz)*f1 + (rx*tz-rz*tx)*f2 + (rz*sx-rx*sz)*f3
            !     s31(-1) = (sx*ty-sy*tx)*f1 + (ry*tx-rx*ty)*f2 + (rx*sy-ry*sx)*f3
            !
            ! (A similar expression holds for other stresses) 
            beginLoopsMask3d()
                accel(1) = rho*bcf(side,axis,i1,i2,i3,s11c)
                accel(2) = rho*bcf(side,axis,i1,i2,i3,s12c)
                accel(3) = rho*bcf(side,axis,i1,i2,i3,s13c)

                met(0,0) = rx(i1,i2,i3,0,0)
                met(0,1) = rx(i1,i2,i3,0,1)
                met(0,2) = rx(i1,i2,i3,0,2)
                met(1,0) = rx(i1,i2,i3,1,0)
                met(1,1) = rx(i1,i2,i3,1,1)
                met(1,2) = rx(i1,i2,i3,1,2)
                met(2,0) = rx(i1,i2,i3,2,0)
                met(2,1) = rx(i1,i2,i3,2,1)
                met(2,2) = rx(i1,i2,i3,2,2)

                ! loop over stress components
                do isc = 1,3
                  do idot = 1,3
                    ! these are extrapolated components
                    stilde(idot) = det(i1-is1,i2-is2,i3-is3)*(rx(i1-is1,i2-is2,i3-is3,idot-1,0)*u(i1-is1,i2-is2,i3-is3,sc(1,isc))+ \
                                                              rx(i1-is1,i2-is2,i3-is3,idot-1,1)*u(i1-is1,i2-is2,i3-is3,sc(2,isc))+ \
                                                              rx(i1-is1,i2-is2,i3-is3,idot-1,2)*u(i1-is1,i2-is2,i3-is3,sc(3,isc)))
                  end do
                  ! now override in the direction we are currently looking
                  stilde(axis+1) = det(i1+is1,i2+is2,i3+is3)*(rx(i1+is1,i2+is2,i3+is3,axis,0)*u(i1+is1,i2+is2,i3+is3,sc(1,isc))+ \
                                                              rx(i1+is1,i2+is2,i3+is3,axis,1)*u(i1+is1,i2+is2,i3+is3,sc(2,isc))+ \
                                                              rx(i1+is1,i2+is2,i3+is3,axis,2)*u(i1+is1,i2+is2,i3+is3,sc(3,isc)))- \
                       2.0*dr(axis)*is*(det(i1,i2,i3)*accel(isc)- \
                             (1.0-delta(axis+1,1))* \
                             (det(i1+1,i2,i3)*(rx(i1+1,i2,i3,0,0)*u(i1+1,i2,i3,sc(1,isc))+ \
                                               rx(i1+1,i2,i3,0,1)*u(i1+1,i2,i3,sc(2,isc))+ \
                                               rx(i1+1,i2,i3,0,2)*u(i1+1,i2,i3,sc(3,isc)))- \
                              det(i1-1,i2,i3)*(rx(i1-1,i2,i3,0,0)*u(i1-1,i2,i3,sc(1,isc))+ \
                                               rx(i1-1,i2,i3,0,1)*u(i1-1,i2,i3,sc(2,isc))+ \
                                               rx(i1-1,i2,i3,0,2)*u(i1-1,i2,i3,sc(3,isc))))/(2.0*dr(0))- \
                             (1.0-delta(axis+1,2))* \
                             (det(i1,i2+1,i3)*(rx(i1,i2+1,i3,1,0)*u(i1,i2+1,i3,sc(1,isc))+ \
                                               rx(i1,i2+1,i3,1,1)*u(i1,i2+1,i3,sc(2,isc))+ \
                                               rx(i1,i2+1,i3,1,2)*u(i1,i2+1,i3,sc(3,isc)))- \
                              det(i1,i2-1,i3)*(rx(i1,i2-1,i3,1,0)*u(i1,i2-1,i3,sc(1,isc))+ \
                                               rx(i1,i2-1,i3,1,1)*u(i1,i2-1,i3,sc(2,isc))+ \
                                               rx(i1,i2-1,i3,1,2)*u(i1,i2-1,i3,sc(3,isc))))/(2.0*dr(1))- \
                             (1.0-delta(axis+1,3))* \
                             (det(i1,i2,i3+1)*(rx(i1,i2,i3+1,2,0)*u(i1,i2,i3+1,sc(1,isc))+ \
                                               rx(i1,i2,i3+1,2,1)*u(i1,i2,i3+1,sc(2,isc))+ \
                                               rx(i1,i2,i3+1,2,2)*u(i1,i2,i3+1,sc(3,isc)))- \
                              det(i1,i2,i3-1)*(rx(i1,i2,i3-1,2,0)*u(i1,i2,i3-1,sc(1,isc))+ \
                                               rx(i1,i2,i3-1,2,1)*u(i1,i2,i3-1,sc(2,isc))+ \
                                               rx(i1,i2,i3-1,2,2)*u(i1,i2,i3-1,sc(3,isc))))/(2.0*dr(2)))

                  u(i1-is1,i2-is2,i3-is3,sc(1,isc)) = (met(1,1)*met(2,2)-met(1,2)*met(2,1))*stilde(1)+ \
                                                      (met(0,2)*met(2,1)-met(0,1)*met(2,2))*stilde(2)+ \
                                                      (met(0,1)*met(1,2)-met(0,2)*met(1,1))*stilde(3)
                  u(i1-is1,i2-is2,i3-is3,sc(2,isc)) = (met(1,2)*met(2,0)-met(1,0)*met(2,2))*stilde(1)+ \
                                                      (met(0,0)*met(2,2)-met(0,2)*met(2,0))*stilde(2)+ \
                                                      (met(0,2)*met(1,0)-met(0,0)*met(1,2))*stilde(3)
                  u(i1-is1,i2-is2,i3-is3,sc(3,isc)) = (met(1,0)*met(2,1)-met(1,1)*met(2,0))*stilde(1)+ \
                                                      (met(0,1)*met(2,0)-met(0,0)*met(2,1))*stilde(2)+ \
                                                      (met(0,0)*met(1,1)-met(0,1)*met(1,0))*stilde(3)
                end do
            endLoopsMask3d()
            else ! free stream preserving method
cccccccccccc
            ! *********** DISPLACEMENT : Curvilinear Grid (free stream preserving) ****************
 
            ! Use momentum equations to get (rx,ry,rz)(0).(s11,s21,s31)(-1) = s11tilde
            !    (1)   (rx,ry,rz).D_r1[(s11,s21,s31)] + (sx,sy,sz).D_r2[(s11,s21,s31)] + (tx,ty,tz).D_r3[(s11,s21,s31)] = rho * u_tt  
            !                      s11tilde                         s21tilde                         s31tilde
            !    (2)   Use extrapolated values to get  (sx,sy,sz)(0).(s11,s21,s31)(-1) = s21tilde
            !    (3)   Use extrapolated values to get  (tx,ty,tz)(0).(s11,s21,s31)(-1) = s31tilde
            ! To give 3 equations for (s11,s21,s31) on the ghost point:
            !   (rx)(0) s11(-1) + (ry)(0) s21(-1) + (rz)(0) s31(-1) = f1 = s11tilde  (from momentum eqn)
            !   (sx)(0) s11(-1) + (sy)(0) s21(-1) + (sz)(0) s31(-1) = f2 = s21tilde  (from extrapolated values)
            !   (tx)(0) s11(-1) + (ty)(0) s21(-1) + (tz)(0) s31(-1) = f3 = s31tilde  (from extrapolated values)
            ! Solve: (note that det is the inverse of the determinant det[rx,ry,rz; sx,sy,sz; tx,ty,tz])
            !     s11(-1) = ((sy*tz-sz*ty)*f1 + (rz*ty-ry*tz)*f2 + (ry*sz-rz*sy)*f3)*det
            !     s21(-1) = ((tx*sz-sx*tz)*f1 + (rx*tz-rz*tx)*f2 + (rz*sx-rx*sz)*f3)*det
            !     s31(-1) = ((sx*ty-sy*tx)*f1 + (ry*tx-rx*ty)*f2 + (rx*sy-ry*sx)*f3)*det
            !
            ! (A similar expression holds for other stresses) 
            beginLoopsMask3d()
                accel(1) = rho*bcf(side,axis,i1,i2,i3,s11c)
                accel(2) = rho*bcf(side,axis,i1,i2,i3,s12c)
                accel(3) = rho*bcf(side,axis,i1,i2,i3,s13c)

                met(0,0) = rx(i1,i2,i3,0,0)
                met(0,1) = rx(i1,i2,i3,0,1)
                met(0,2) = rx(i1,i2,i3,0,2)
                met(1,0) = rx(i1,i2,i3,1,0)
                met(1,1) = rx(i1,i2,i3,1,1)
                met(1,2) = rx(i1,i2,i3,1,2)
                met(2,0) = rx(i1,i2,i3,2,0)
                met(2,1) = rx(i1,i2,i3,2,1)
                met(2,2) = rx(i1,i2,i3,2,2)

                ! loop over stress components
                do isc = 1,3
                  do idot = 1,3
                    ! these are extrapolated components
                    stilde(idot) = rx(i1,i2,i3,idot-1,0)*u(i1-is1,i2-is2,i3-is3,sc(1,isc))+ \
                                   rx(i1,i2,i3,idot-1,1)*u(i1-is1,i2-is2,i3-is3,sc(2,isc))+ \
                                   rx(i1,i2,i3,idot-1,2)*u(i1-is1,i2-is2,i3-is3,sc(3,isc))
                  end do
                  ! now override in the direction we are currently looking
                  stilde(axis+1) = (rx(i1,i2,i3,axis,0)*u(i1+is1,i2+is2,i3+is3,sc(1,isc))+ \
                                    rx(i1,i2,i3,axis,1)*u(i1+is1,i2+is2,i3+is3,sc(2,isc))+ \
                                    rx(i1,i2,i3,axis,2)*u(i1+is1,i2+is2,i3+is3,sc(3,isc)))- \
                       2.0*dr(axis)*is*(accel(isc)- \
                             (1.0-delta(axis+1,1))* \
                             (rx(i1,i2,i3,0,0)*(u(i1+1,i2,i3,sc(1,isc))-u(i1-1,i2,i3,sc(1,isc)))+ \
                              rx(i1,i2,i3,0,1)*(u(i1+1,i2,i3,sc(2,isc))-u(i1-1,i2,i3,sc(2,isc)))+ \
                              rx(i1,i2,i3,0,2)*(u(i1+1,i2,i3,sc(3,isc))-u(i1-1,i2,i3,sc(3,isc))))/(2.0*dr(0))- \
                             (1.0-delta(axis+1,2))* \
                             (rx(i1,i2,i3,1,0)*(u(i1,i2+1,i3,sc(1,isc))-u(i1,i2-1,i3,sc(1,isc)))+ \
                              rx(i1,i2,i3,1,1)*(u(i1,i2+1,i3,sc(2,isc))-u(i1,i2-1,i3,sc(2,isc)))+ \
                              rx(i1,i2,i3,1,2)*(u(i1,i2+1,i3,sc(3,isc))-u(i1,i2-1,i3,sc(3,isc))))/(2.0*dr(1))- \
                             (1.0-delta(axis+1,3))* \
                             (rx(i1,i2,i3,2,0)*(u(i1,i2,i3+1,sc(1,isc))-u(i1,i2,i3-1,sc(1,isc)))+ \
                              rx(i1,i2,i3,2,1)*(u(i1,i2,i3+1,sc(2,isc))-u(i1,i2,i3-1,sc(2,isc)))+ \
                              rx(i1,i2,i3,2,2)*(u(i1,i2,i3+1,sc(3,isc))-u(i1,i2,i3-1,sc(3,isc))))/(2.0*dr(2)))

                  u(i1-is1,i2-is2,i3-is3,sc(1,isc)) = ((met(1,1)*met(2,2)-met(1,2)*met(2,1))*stilde(1)+ \
                                                       (met(0,2)*met(2,1)-met(0,1)*met(2,2))*stilde(2)+ \
                                                       (met(0,1)*met(1,2)-met(0,2)*met(1,1))*stilde(3))*det(i1,i2,i3)
                  u(i1-is1,i2-is2,i3-is3,sc(2,isc)) = ((met(1,2)*met(2,0)-met(1,0)*met(2,2))*stilde(1)+ \
                                                       (met(0,0)*met(2,2)-met(0,2)*met(2,0))*stilde(2)+ \
                                                       (met(0,2)*met(1,0)-met(0,0)*met(1,2))*stilde(3))*det(i1,i2,i3)
                  u(i1-is1,i2-is2,i3-is3,sc(3,isc)) = ((met(1,0)*met(2,1)-met(1,1)*met(2,0))*stilde(1)+ \
                                                       (met(0,1)*met(2,0)-met(0,0)*met(2,1))*stilde(2)+ \
                                                       (met(0,0)*met(1,1)-met(0,1)*met(1,0))*stilde(3))*det(i1,i2,i3)
                end do
            endLoopsMask3d()
cccccccccccc
            end if
          end if ! end gridType

        else if( boundaryCondition(side,axis).eq.tractionBC ) then
           ! **************** TRACTION : Neumann type conditions ******************
          if( gridType.eq.rectangular )then

            ! ********* TRACTION : Cartesian Grid **********

            ! Assign displacements on the ghost points from given tractions on the boundary
            !   s11 = kappa*u.x + lambda*( v.y + w.z )
            !   s22 = kappa*v.y + lambda*( u.x + w.z )
            !   s33 = kappa*w.z + lambda*( u.x + v.y )
            !   s12 = s21 = mu*( u.y + v.x )
            !   s13 = s31 = mu*( w.x + u.z )
            !
            !   an1*s11 + an2*s21 + an3*s31 = f1
            !   an1*s12 + an2*s22 + an3*s32 = f2
            !   an1*s13 + an2*s23 + an3*s33 = f3
           
            ! Assign velocities on the ghost points from given time derivatives of the tractions on the boundary
            beginLoopsMask3d()
                f1 = bcf(side,axis,i1,i2,i3,uc)
                f2 = bcf(side,axis,i1,i2,i3,vc)
                f3 = bcf(side,axis,i1,i2,i3,wc)

                if( axis.eq.0 )then
                  an1 = -is
                  an2 = 0.0
                  an3 = 0.0
                else if( axis.eq.1 ) then
                  an1 = 0.0
                  an2 = -is
                  an3 = 0.0
                else 
                  an1 = 0.0
                  an2 = 0.0
                  an3 = -is
                end if
                
                dux = diffr1(uc,dx(0))*(1.0-delta(axis+1,1))
                duy = diffr2(uc,dx(1))*(1.0-delta(axis+1,2))
                duz = diffr3(uc,dx(2))*(1.0-delta(axis+1,3))

                dvx = diffr1(vc,dx(0))*(1.0-delta(axis+1,1))
                dvy = diffr2(vc,dx(1))*(1.0-delta(axis+1,2))
                dvz = diffr3(vc,dx(2))*(1.0-delta(axis+1,3))

                dwx = diffr1(wc,dx(0))*(1.0-delta(axis+1,1))
                dwy = diffr2(wc,dx(1))*(1.0-delta(axis+1,2))
                dwz = diffr3(wc,dx(2))*(1.0-delta(axis+1,3))

                f1 = f1-an1*(kappa*dux+lambda*(dvy+dwz))- \
                        an2*(mu*(dvx+duy))- \
                        an3*(mu*(dwx+duz))
                f2 = f2-an1*(mu*(dvx+duy))- \
                        an2*(kappa*dvy+lambda*(dux+dwz))- \
                        an3*(mu*(dwy+dvz))
                f3 = f3-an1*(mu*(dwx+duz))- \
                        an2*(mu*(dwy+dvz))- \
                        an3*(kappa*dwz+lambda*(dux+dvy))

                ! in the Cartesian case all that survives in the Matrix are the diagonal terms
                f1 = f1/(an1*kappa +an2*mu    +an3*mu)
                f2 = f2/(an1*mu    +an2*kappa +an3*mu)
                f3 = f3/(an1*mu    +an2*mu    +an3*kappa)

                u(i1-is1,i2-is2,i3-is3,uc) = -is*2.0*dx(axis)*f1+u(i1+is1,i2+is2,i3+is3,uc)
                u(i1-is1,i2-is2,i3-is3,vc) = -is*2.0*dx(axis)*f2+u(i1+is1,i2+is2,i3+is3,vc)
                u(i1-is1,i2-is2,i3-is3,wc) = -is*2.0*dx(axis)*f3+u(i1+is1,i2+is2,i3+is3,wc)

                !!! now do velocities
                fdot1 = bcf(side,axis,i1,i2,i3,v1c)
                fdot2 = bcf(side,axis,i1,i2,i3,v2c)
                fdot3 = bcf(side,axis,i1,i2,i3,v3c)
                
                dux = diffr1(v1c,dx(0))*(1.0-delta(axis+1,1))
                duy = diffr2(v1c,dx(1))*(1.0-delta(axis+1,2))
                duz = diffr3(v1c,dx(2))*(1.0-delta(axis+1,3))

                dvx = diffr1(v2c,dx(0))*(1.0-delta(axis+1,1))
                dvy = diffr2(v2c,dx(1))*(1.0-delta(axis+1,2))
                dvz = diffr3(v2c,dx(2))*(1.0-delta(axis+1,3))

                dwx = diffr1(v3c,dx(0))*(1.0-delta(axis+1,1))
                dwy = diffr2(v3c,dx(1))*(1.0-delta(axis+1,2))
                dwz = diffr3(v3c,dx(2))*(1.0-delta(axis+1,3))

                fdot1 = fdot1-an1*(kappa*dux+lambda*(dvy+dwz))- \
                              an2*(mu*(dvx+duy))- \
                              an3*(mu*(dwx+duz))
                fdot2 = fdot2-an1*(mu*(dvx+duy))- \
                              an2*(kappa*dvy+lambda*(dux+dwz))- \
                              an3*(mu*(dwy+dvz))
                fdot3 = fdot3-an1*(mu*(dwx+duz))- \
                              an2*(mu*(dwy+dvz))- \
                              an3*(kappa*dwz+lambda*(dux+dvy))

                ! in the Cartesian case all that survives in the Matrix are the diagonal terms
                fdot1 = fdot1/(an1*kappa +an2*mu    +an3*mu)
                fdot2 = fdot2/(an1*mu    +an2*kappa +an3*mu)
                fdot3 = fdot3/(an1*mu    +an2*mu    +an3*kappa)

                u(i1-is1,i2-is2,i3-is3,v1c) = -is*2.0*dx(axis)*fdot1+u(i1+is1,i2+is2,i3+is3,v1c)
                u(i1-is1,i2-is2,i3-is3,v2c) = -is*2.0*dx(axis)*fdot2+u(i1+is1,i2+is2,i3+is3,v2c)
                u(i1-is1,i2-is2,i3-is3,v3c) = -is*2.0*dx(axis)*fdot3+u(i1+is1,i2+is2,i3+is3,v3c)

            endLoopsMask3d()
          else
            ! *********** TRACTION : Curvilinear Grid ****************
            beginLoopsMask3d()
                f1 = bcf(side,axis,i1,i2,i3,uc)
                f2 = bcf(side,axis,i1,i2,i3,vc)
                f3 = bcf(side,axis,i1,i2,i3,wc)

                fdot1 = bcf(side,axis,i1,i2,i3,v1c)
                fdot2 = bcf(side,axis,i1,i2,i3,v2c)
                fdot3 = bcf(side,axis,i1,i2,i3,v3c)

                met(0,0) = rx(i1,i2,i3,0,0)
                met(0,1) = rx(i1,i2,i3,0,1)
                met(0,2) = rx(i1,i2,i3,0,2)
                met(1,0) = rx(i1,i2,i3,1,0)
                met(1,1) = rx(i1,i2,i3,1,1)
                met(1,2) = rx(i1,i2,i3,1,2)
                met(2,0) = rx(i1,i2,i3,2,0)
                met(2,1) = rx(i1,i2,i3,2,1)
                met(2,2) = rx(i1,i2,i3,2,2)

                ! (an1,an2,an3) = outward normal 
                aNormi = 1.0/max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                an3 = -is*rx(i1,i2,i3,axis,2)*aNormi

                dur(0) = diffr1(uc,dr(0))*(1.0-delta(axis+1,1))
                dur(1) = diffr2(uc,dr(1))*(1.0-delta(axis+1,2))
                dur(2) = diffr3(uc,dr(2))*(1.0-delta(axis+1,3))

                dvr(0) = diffr1(vc,dr(0))*(1.0-delta(axis+1,1))
                dvr(1) = diffr2(vc,dr(1))*(1.0-delta(axis+1,2))
                dvr(2) = diffr3(vc,dr(2))*(1.0-delta(axis+1,3))

                dwr(0) = diffr1(wc,dr(0))*(1.0-delta(axis+1,1))
                dwr(1) = diffr2(wc,dr(1))*(1.0-delta(axis+1,2))
                dwr(2) = diffr3(wc,dr(2))*(1.0-delta(axis+1,3))

                dv1r(0) = diffr1(v1c,dr(0))*(1.0-delta(axis+1,1))
                dv1r(1) = diffr2(v1c,dr(1))*(1.0-delta(axis+1,2))
                dv1r(2) = diffr3(v1c,dr(2))*(1.0-delta(axis+1,3))

                dv2r(0) = diffr1(v2c,dr(0))*(1.0-delta(axis+1,1))
                dv2r(1) = diffr2(v2c,dr(1))*(1.0-delta(axis+1,2))
                dv2r(2) = diffr3(v2c,dr(2))*(1.0-delta(axis+1,3))

                dv3r(0) = diffr1(v3c,dr(0))*(1.0-delta(axis+1,1))
                dv3r(1) = diffr2(v3c,dr(1))*(1.0-delta(axis+1,2))
                dv3r(2) = diffr3(v3c,dr(2))*(1.0-delta(axis+1,3))

                do isc = 0,2
                  mat(0,0) = an1*kappa*met(isc,0) +an2*mu*met(isc,1)    +an3*mu*met(isc,2)
                  mat(0,1) = an1*lambda*met(isc,1)+an2*mu*met(isc,0)
                  mat(0,2) = an1*lambda*met(isc,2)                      +an3*mu*met(isc,0)
                  mat(1,0) = an1*mu*met(isc,1)    +an2*lambda*met(isc,0)
                  mat(1,1) = an1*mu*met(isc,0)    +an2*kappa*met(isc,1) +an3*mu*met(isc,2)
                  mat(1,2) =                       an2*lambda*met(isc,2)+an3*mu*met(isc,1)
                  mat(2,0) = an1*mu*met(isc,2)                          +an3*lambda*met(isc,0)
                  mat(2,1) =                       an2*mu*met(isc,2)    +an3*lambda*met(isc,1)
                  mat(2,2) = an1*mu*met(isc,0)    +an2*mu*met(isc,1)    +an3*kappa*met(isc,2)

                  f1 = f1-(mat(0,0)*dur(isc)+mat(0,1)*dvr(isc)+mat(0,2)*dwr(isc))
                  f2 = f2-(mat(1,0)*dur(isc)+mat(1,1)*dvr(isc)+mat(1,2)*dwr(isc))
                  f3 = f3-(mat(2,0)*dur(isc)+mat(2,1)*dvr(isc)+mat(2,2)*dwr(isc))

                  fdot1 = fdot1-(mat(0,0)*dv1r(isc)+mat(0,1)*dv2r(isc)+mat(0,2)*dv3r(isc))
                  fdot2 = fdot2-(mat(1,0)*dv1r(isc)+mat(1,1)*dv2r(isc)+mat(1,2)*dv3r(isc))
                  fdot3 = fdot3-(mat(2,0)*dv1r(isc)+mat(2,1)*dv2r(isc)+mat(2,2)*dv3r(isc))

                  if( axis.eq.isc ) then
                    lhs(0,0) = mat(0,0)
                    lhs(0,1) = mat(0,1)
                    lhs(0,2) = mat(0,2)
                    lhs(1,0) = mat(1,0)
                    lhs(1,1) = mat(1,1)
                    lhs(1,2) = mat(1,2)
                    lhs(2,0) = mat(2,0)
                    lhs(2,1) = mat(2,1)
                    lhs(2,2) = mat(2,2)
                  end if                  
                end do

                
                !! solve linear systems to get the solution (grid derivatives)
                rhs(0,0) = f1
                rhs(1,0) = f2
                rhs(2,0) = f3
                rhs(0,1) = fdot1
                rhs(1,1) = fdot2
                rhs(2,1) = fdot3
                
                call dgesv( 3,2,lhs,3,ipiv,rhs,3,info )
                if( info.ne.0 ) then
                  write(6,*)'Error (compat3D) : error in  linear system'
                  stop
                end if

                u(i1-is1,i2-is2,i3-is3,uc)  = -is*2.0*rhs(0,0)*dr(axis)+u(i1+is1,i2+is2,i3+is3,uc)
                u(i1-is1,i2-is2,i3-is3,vc)  = -is*2.0*rhs(1,0)*dr(axis)+u(i1+is1,i2+is2,i3+is3,vc)
                u(i1-is1,i2-is2,i3-is3,wc)  = -is*2.0*rhs(2,0)*dr(axis)+u(i1+is1,i2+is2,i3+is3,wc)

                u(i1-is1,i2-is2,i3-is3,v1c) = -is*2.0*rhs(0,1)*dr(axis)+u(i1+is1,i2+is2,i3+is3,v1c)
                u(i1-is1,i2-is2,i3-is3,v2c) = -is*2.0*rhs(1,1)*dr(axis)+u(i1+is1,i2+is2,i3+is3,v2c)
                u(i1-is1,i2-is2,i3-is3,v3c) = -is*2.0*rhs(2,1)*dr(axis)+u(i1+is1,i2+is2,i3+is3,v3c)

            endLoopsMask3d()
          end if ! gridType
        else if( boundaryCondition(side,axis).eq.slipWall ) then
          ! **************** SLIPWALL : Neumann type conditions ******************
          if( gridType.eq.rectangular ) then
            ! ********* SLIPWALL : Cartesian Grid **********
          else
            ! ********* SLIPWALL : Curvilinear Grid **********
          end if ! gridType

        else if( boundaryCondition(side,axis).gt.0 .and. boundaryCondition(side,axis).ne.dirichletBoundaryCondition ) then
	  write(*,'("smg3d:BC: unknown BC: side,axis,grid, boundaryCondition=",i2,i2,i4,i8)') side,axis,grid,boundaryCondition(side,axis)

        end if ! bc
      endLoopOverSides()

c*******
c******* Secondary Dirichlet conditions for the tangential components of stress (tractionBC only) ********
c*******
c      if( .false. ) then
        beginLoopOverSides(numGhost,numGhost)
          if( boundaryCondition(side,axis).eq.tractionBC )then
            if( gridType.eq.rectangular )then
              if( axis.eq.0 )then
                beginLoopsMask3d()
                    u1x = diffr1(uc,dx(0))
                    u1y = diffr2(uc,dx(1))
                    u1z = diffr3(uc,dx(2))

                    u2x = diffr1(vc,dx(0))
                    u2y = diffr2(vc,dx(1))
                    u2z = diffr3(vc,dx(2))
                  
                    u3x = diffr1(wc,dx(0))
                    u3y = diffr2(wc,dx(1))
                    u3z = diffr3(wc,dx(2))

                    u(i1,i2,i3,s21c) = mu*(u1y+u2x)
                    u(i1,i2,i3,s22c) = kappa*u2y+lambda*(u1x+u3z)
                    u(i1,i2,i3,s23c) = mu*(u3y+u2z)

                    u(i1,i2,i3,s31c) = mu*(u3x+u1z)
                    u(i1,i2,i3,s32c) = mu*(u3y+u2z)
                    u(i1,i2,i3,s33c) = kappa*u3z+lambda*(u1x+u2y)
                endLoopsMask3d()
              else if( axis.eq.1 ) then
                beginLoopsMask3d()
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
                    u(i1,i2,i3,s12c) = mu*(u2x+u1y)
                    u(i1,i2,i3,s13c) = mu*(u3x+u1z)

                    u(i1,i2,i3,s31c) = mu*(u3x+u1z)
                    u(i1,i2,i3,s32c) = mu*(u3y+u2z)
                    u(i1,i2,i3,s33c) = kappa*u3z+lambda*(u1x+u2y)
                endLoopsMask3d()
              else 
                beginLoopsMask3d()
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
                    u(i1,i2,i3,s12c) = mu*(u2x+u1y)
                    u(i1,i2,i3,s13c) = mu*(u3x+u1z)

                    u(i1,i2,i3,s21c) = mu*(u1y+u2x)
                    u(i1,i2,i3,s22c) = kappa*u2y+lambda*(u1x+u3z)
                    u(i1,i2,i3,s23c) = mu*(u3y+u2z)
                endLoopsMask3d()
              end if

            else  ! curvilinear 
              if( axis.eq.0 ) then
                tan1c = 1
                tan2c = 2
              else if( axis.eq.1 ) then
                tan1c = 2
                tan2c = 0
              else
                tan1c = 0
                tan2c = 1
              end if
              beginLoopsMask3d()
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

                  s11t = kappa*u1x+lambda*(u2y+u3z)
                  s12t = mu*(u2x+u1y)
                  s13t = mu*(u3x+u1z)

                  s21t = mu*(u2x+u1y)
                  s22t = kappa*u2y+lambda*(u1x+u3z)
                  s23t = mu*(u3y+u2z)

                  s31t = mu*(u3x+u1z)
                  s32t = mu*(u3y+u2z)
                  s33t = kappa*u3z+lambda*(u1x+u2y)

                  ! (an1,an2,3) = outward unit normal 
                  aNormi = 1.0/max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                  an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                  an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                  an3 = -is*rx(i1,i2,i3,axis,2)*aNormi

                  !! do the signs of sn and tn matter?? ... I do not think so
                  sn1 = rx(i1,i2,i3,tan1c,0)
                  sn2 = rx(i1,i2,i3,tan1c,1)
                  sn3 = rx(i1,i2,i3,tan1c,2)

                  tn1 = rx(i1,i2,i3,tan2c,0)
                  tn2 = rx(i1,i2,i3,tan2c,1)
                  tn3 = rx(i1,i2,i3,tan2c,2)

                  ! set sn to be part of sn which is orthogonal to an
                  alpha = an1*sn1+an2*sn2+an3*sn3
                  sn1 = sn1-alpha*an1
                  sn2 = sn2-alpha*an2
                  sn3 = sn3-alpha*an3
                  ! normalize sn
                  aNormi = 1.0/max(epsx,sqrt(sn1**2+sn2**2+sn3**2))
                  sn1 = sn1*aNormi
                  sn2 = sn2*aNormi
                  sn3 = sn3*aNormi

                  ! set tn to be part of tn which is orthogonal to an and sn
                  alpha = an1*tn1+an2*tn2+an3*tn3
                  tn1 = tn1-alpha*an1
                  tn2 = tn2-alpha*an2
                  tn3 = tn3-alpha*an3
                  alpha = sn1*tn1+sn2*tn2+sn3*tn3
                  tn1 = tn1-alpha*sn1
                  tn2 = tn2-alpha*sn2
                  tn3 = tn3-alpha*sn3
                  ! normalize tn
                  aNormi = 1.0/max(epsx,sqrt(tn1**2+tn2**2+tn3**2))
                  tn1 = tn1*aNormi
                  tn2 = tn2*aNormi
                  tn3 = tn3*aNormi

                  ! compute components of stress in normal direction (primary condition)
                  ns1 = an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,i3,s21c)+an3*u(i1,i2,i3,s31c)
                  ns2 = an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,i3,s22c)+an3*u(i1,i2,i3,s32c)
                  ns3 = an1*u(i1,i2,i3,s13c)+an2*u(i1,i2,i3,s23c)+an3*u(i1,i2,i3,s33c)
                  
                  ! compute componenets of stress in 1st tangential direction (secondary condition)
                  ss1 = sn1*s11t+sn2*s21t+sn3*s31t
                  ss2 = sn1*s12t+sn2*s22t+sn3*s32t
                  ss3 = sn1*s13t+sn2*s23t+sn3*s33t

                  ! compute componenets of stress in 2nd tangential direction (secondary condition)
                  ts1 = tn1*s11t+tn2*s21t+tn3*s31t
                  ts2 = tn1*s12t+tn2*s22t+tn3*s32t
                  ts3 = tn1*s13t+tn2*s23t+tn3*s33t

                  u(i1,i2,i3,s11c) = an1*ns1+sn1*ss1+tn1*ts1
                  u(i1,i2,i3,s12c) = an1*ns2+sn1*ss2+tn1*ts2
                  u(i1,i2,i3,s13c) = an1*ns3+sn1*ss3+tn1*ts3

                  u(i1,i2,i3,s21c) = an2*ns1+sn2*ss1+tn2*ts1
                  u(i1,i2,i3,s22c) = an2*ns2+sn2*ss2+tn2*ts2
                  u(i1,i2,i3,s23c) = an2*ns3+sn2*ss3+tn2*ts3

                  u(i1,i2,i3,s31c) = an3*ns1+sn3*ss1+tn3*ts1
                  u(i1,i2,i3,s32c) = an3*ns2+sn3*ss2+tn3*ts2
                  u(i1,i2,i3,s33c) = an3*ns3+sn3*ss3+tn3*ts3
              endLoopsMask3d()

            end if  ! end gridType

          end if ! bc 
        endLoopOverSides()

c set tangential components of stress on the boundary  (TZ forcing, if necessary)
        if( twilightZone.ne.0 ) then
c        if( .false. ) then

          beginLoopOverSides(numGhost,numGhost)
            if( boundaryCondition(side,axis).eq.tractionBC )then

              if( gridType.eq.rectangular )then
                if( axis.eq.0 )then
                  beginLoopsMask3d()
                      call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,u1x )
                      call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,u1y )
                      call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,u1z )
                  
                      call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,u2x )
                      call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,u2y )
                      call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,u2z )
                  
                      call ogDeriv( ep,0,1,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,u3x )
                      call ogDeriv( ep,0,0,1,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,u3y )
                      call ogDeriv( ep,0,0,0,1,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,u3z )

                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s21c,s21e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s22c,s22e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s23c,s23e )

                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s31c,s31e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s32c,s32e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s33c,s33e )

                      u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)+s21e-mu*(u1y+u2x)
                      u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)+s22e-(kappa*u2y+lambda*(u1x+u3z))
                      u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)+s23e-mu*(u3y+u2z)

                      u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)+s31e-mu*(u3x+u1z)
                      u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)+s32e-mu*(u3y+u2z)
                      u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)+s33e-(kappa*u3z+lambda*(u1x+u2y))
                  endLoopsMask3d()
                else if( axis.eq.1 ) then
                  beginLoopsMask3d()
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

                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s31c,s31e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s32c,s32e )
                      call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s33c,s33e )

                      u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)+s11e-(kappa*u1x+lambda*(u2y+u3z))
                      u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)+s12e-mu*(u2x+u1y)
                      u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)+s13e-mu*(u3x+u1z)

                      u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)+s31e-mu*(u3x+u1z)
                      u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)+s32e-mu*(u3y+u2z)
                      u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)+s33e-(kappa*u3z+lambda*(u1x+u2y))
                  endLoopsMask3d()
                else
                  beginLoopsMask3d()
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

                      u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)+s11e-(kappa*u1x+lambda*(u2y+u3z))
                      u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)+s12e-mu*(u2x+u1y)
                      u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)+s13e-mu*(u3x+u1z)

                      u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)+s21e-mu*(u1y+u2x)
                      u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)+s22e-(kappa*u2y+lambda*(u1x+u3z))
                      u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)+s23e-mu*(u3y+u2z)
                  endLoopsMask3d()
                end if

              else ! curvilinear 
                if( axis.eq.0 ) then
                  tan1c = 1
                  tan2c = 2
                else if( axis.eq.1 ) then
                  tan1c = 2
                  tan2c = 0
                else
                  tan1c = 0
                  tan2c = 1
                end if
                beginLoopsMask3d()
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

                    s11t = kappa*u1x+lambda*(u2y+u3z)
                    s12t = mu*(u2x+u1y)
                    s13t = mu*(u3x+u1z)

                    s21t = mu*(u2x+u1y)
                    s22t = kappa*u2y+lambda*(u1x+u3z)
                    s23t = mu*(u3y+u2z)

                    s31t = mu*(u3x+u1z)
                    s32t = mu*(u3y+u2z)
                    s33t = kappa*u3z+lambda*(u1x+u2y)

                    ! (an1,an2,3) = outward unit normal 
                    aNormi = 1./max(epsx,sqrt(rx(i1,i2,i3,axis,0)**2+rx(i1,i2,i3,axis,1)**2+rx(i1,i2,i3,axis,2)**2))
                    an1 = -is*rx(i1,i2,i3,axis,0)*aNormi
                    an2 = -is*rx(i1,i2,i3,axis,1)*aNormi
                    an3 = -is*rx(i1,i2,i3,axis,2)*aNormi

                    sn1 = rx(i1,i2,i3,tan1c,0)
                    sn2 = rx(i1,i2,i3,tan1c,1)
                    sn3 = rx(i1,i2,i3,tan1c,2)

                    tn1 = rx(i1,i2,i3,tan2c,0)
                    tn2 = rx(i1,i2,i3,tan2c,1)
                    tn3 = rx(i1,i2,i3,tan2c,2)

                    ! set sn to be part of sn which is orthogonal to an
                    alpha = an1*sn1+an2*sn2+an3*sn3
                    sn1 = sn1-alpha*an1
                    sn2 = sn2-alpha*an2
                    sn3 = sn3-alpha*an3
                    ! normalize sn
                    aNormi = 1./max(epsx,sqrt(sn1**2+sn2**2+sn3**2))
                    sn1 = sn1*aNormi
                    sn2 = sn2*aNormi
                    sn3 = sn3*aNormi

                    ! set tn to be part of tn which is orthogonal to an and sn
                    alpha = an1*tn1+an2*tn2+an3*tn3
                    tn1 = tn1-alpha*an1
                    tn2 = tn2-alpha*an2
                    tn3 = tn3-alpha*an3
                    alpha = sn1*tn1+sn2*tn2+sn3*tn3
                    tn1 = tn1-alpha*sn1
                    tn2 = tn2-alpha*sn2
                    tn3 = tn3-alpha*sn3
                    ! normalize tn
                    aNormi = 1./max(epsx,sqrt(tn1**2+tn2**2+tn3**2))
                    tn1 = tn1*aNormi
                    tn2 = tn2*aNormi
                    tn3 = tn3*aNormi

                    ! compute components of stress in normal direction (leave these alone)
                    ns1 = an1*u(i1,i2,i3,s11c)+an2*u(i1,i2,i3,s21c)+an3*u(i1,i2,i3,s31c)
                    ns2 = an1*u(i1,i2,i3,s12c)+an2*u(i1,i2,i3,s22c)+an3*u(i1,i2,i3,s32c)
                    ns3 = an1*u(i1,i2,i3,s13c)+an2*u(i1,i2,i3,s23c)+an3*u(i1,i2,i3,s33c)

                    ! compute componenets of stress in tangential directions (add forcing to these)
                    ss1 = sn1*u(i1,i2,i3,s11c)+sn2*u(i1,i2,i3,s21c)+sn3*u(i1,i2,i3,s31c)
                    ss2 = sn1*u(i1,i2,i3,s12c)+sn2*u(i1,i2,i3,s22c)+sn3*u(i1,i2,i3,s32c)
                    ss3 = sn1*u(i1,i2,i3,s13c)+sn2*u(i1,i2,i3,s23c)+sn3*u(i1,i2,i3,s33c)
                    ts1 = tn1*u(i1,i2,i3,s11c)+tn2*u(i1,i2,i3,s21c)+tn3*u(i1,i2,i3,s31c)
                    ts2 = tn1*u(i1,i2,i3,s12c)+tn2*u(i1,i2,i3,s22c)+tn3*u(i1,i2,i3,s32c)
                    ts3 = tn1*u(i1,i2,i3,s13c)+tn2*u(i1,i2,i3,s23c)+tn3*u(i1,i2,i3,s33c)

                    ! compute componenets of derived stress in tangential directions
                    ss1d = sn1*s11t+sn2*s21t+sn3*s31t
                    ss2d = sn1*s12t+sn2*s22t+sn3*s32t
                    ss3d = sn1*s13t+sn2*s23t+sn3*s33t
                    ts1d = tn1*s11t+tn2*s21t+tn3*s31t
                    ts2d = tn1*s12t+tn2*s22t+tn3*s32t
                    ts3d = tn1*s13t+tn2*s23t+tn3*s33t

                    ! compute componenets of exact stress in tangential directions
                    ss1e = sn1*s11e+sn2*s21e+sn3*s31e
                    ss2e = sn1*s12e+sn2*s22e+sn3*s32e
                    ss3e = sn1*s13e+sn2*s23e+sn3*s33e
                    ts1e = tn1*s11e+tn2*s21e+tn3*s31e
                    ts2e = tn1*s12e+tn2*s22e+tn3*s32e
                    ts3e = tn1*s13e+tn2*s23e+tn3*s33e

                    ss1 = ss1+ss1e-ss1d
                    ss2 = ss2+ss2e-ss2d
                    ss3 = ss3+ss3e-ss3d
                    ts1 = ts1+ts1e-ts1d
                    ts2 = ts2+ts2e-ts2d
                    ts3 = ts3+ts3e-ts3d

                    u(i1,i2,i3,s11c) = an1*ns1+sn1*ss1+tn1*ts1
                    u(i1,i2,i3,s12c) = an1*ns2+sn1*ss2+tn1*ts2
                    u(i1,i2,i3,s13c) = an1*ns3+sn1*ss3+tn1*ts3

                    u(i1,i2,i3,s21c) = an2*ns1+sn2*ss1+tn2*ts1
                    u(i1,i2,i3,s22c) = an2*ns2+sn2*ss2+tn2*ts2
                    u(i1,i2,i3,s23c) = an2*ns3+sn2*ss3+tn2*ts3

                    u(i1,i2,i3,s31c) = an3*ns1+sn3*ss1+tn3*ts1
                    u(i1,i2,i3,s32c) = an3*ns2+sn3*ss2+tn3*ts2
                    u(i1,i2,i3,s33c) = an3*ns3+sn3*ss3+tn3*ts3

c                    u(i1,i2,i3,s11c) = s11e
c                    u(i1,i2,i3,s12c) = s12e
c                    u(i1,i2,i3,s13c) = s13e
c                                                                        
c                    u(i1,i2,i3,s21c) = s21e
c                    u(i1,i2,i3,s22c) = s22e
c                    u(i1,i2,i3,s23c) = s23e
c                                                                        
c                    u(i1,i2,i3,s31c) = s31e
c                    u(i1,i2,i3,s32c) = s32e
c                    u(i1,i2,i3,s33c) = s33e
                endLoopsMask3d()

              end if  ! end gridType

            end if ! bc 
          endLoopOverSides()
c
c.. substract off components of TZ force that were added twice
          beginEdgeMacro()
            if( bc1.eq.tractionBC .and. bc2.eq.tractionBC ) then
              if( gridType.eq.rectangular ) then
                beginLoopsMask3d()
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

                  if( edgeDirection.eq.0 ) then
                    u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)-(s11e-(kappa*u1x+lambda*(u2y+u3z)))
                    u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)-(s12e-mu*(u2x+u1y))
                    u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)-(s13e-mu*(u3x+u1z))
                  else if( edgeDirection.eq.1 ) then
                    u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)-(s21e-mu*(u1y+u2x))
                    u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)-(s22e-(kappa*u2y+lambda*(u1x+u3z)))
                    u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)-(s23e-mu*(u3y+u2z))
                  else
                    u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)-(s31e-mu*(u3x+u1z))
                    u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)-(s32e-mu*(u3y+u2z))
                    u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)-(s33e-(kappa*u3z+lambda*(u1x+u2y)))
                  end if
                endLoopsMask3d()
              else
                beginLoopsMask3d()
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

                  tn1 = rx(i1,i2,i3,edgeDirection,0)
                  tn2 = rx(i1,i2,i3,edgeDirection,1)
                  tn3 = rx(i1,i2,i3,edgeDirection,2)
                  aNormi = 1.0/max(epsx,sqrt(tn1**2+tn2**2+tn3**2))
                  tn1 = tn1*aNormi
                  tn2 = tn2*aNormi
                  tn3 = tn3*aNormi

                  f1 = tn1*(s11e-(kappa*u1x+lambda*(u2y+u3z)))+tn2*(s21e-mu*(u1y+u2x))                +tn3*(s31e-mu*(u3x+u1z))                
                  f2 = tn1*(s12e-mu*(u2x+u1y))                +tn2*(s22e-(kappa*u2y+lambda*(u1x+u3z)))+tn3*(s32e-mu*(u3y+u2z))                
                  f3 = tn1*(s13e-mu*(u3x+u1z))                +tn2*(s23e-mu*(u3y+u2z))                +tn3*(s33e-(kappa*u3z+lambda*(u1x+u2y)))

c                  u(i1,i2,i3,s11c) = u(i1,i2,i3,s11c)-tn1*f1
c                  u(i1,i2,i3,s12c) = u(i1,i2,i3,s12c)-tn1*f2
c                  u(i1,i2,i3,s13c) = u(i1,i2,i3,s13c)-tn1*f3
c
c                  u(i1,i2,i3,s21c) = u(i1,i2,i3,s21c)-tn2*f1
c                  u(i1,i2,i3,s22c) = u(i1,i2,i3,s22c)-tn2*f2
c                  u(i1,i2,i3,s23c) = u(i1,i2,i3,s23c)-tn2*f3
c
c                  u(i1,i2,i3,s31c) = u(i1,i2,i3,s31c)-tn3*f1
c                  u(i1,i2,i3,s32c) = u(i1,i2,i3,s32c)-tn3*f2
c                  u(i1,i2,i3,s33c) = u(i1,i2,i3,s33c)-tn3*f3
ccc
                  u(i1,i2,i3,s11c) = s11e
                  u(i1,i2,i3,s12c) = s12e
                  u(i1,i2,i3,s13c) = s13e
c
                  u(i1,i2,i3,s21c) = s21e
                  u(i1,i2,i3,s22c) = s22e
                  u(i1,i2,i3,s23c) = s23e
c
                  u(i1,i2,i3,s31c) = s31e
                  u(i1,i2,i3,s32c) = s32e
                  u(i1,i2,i3,s33c) = s33e
                endLoopsMask3d()
              end if ! gridType
            end if ! bcTypes
          endEdgeMacro()
        end if
c
c.. re-compute stress at traction-traction edges
        beginEdgeMacro()
          if( gridType.eq.rectangular ) then
            ! do nothing ...
          else
            if( bc1.eq.tractionBC .and. bc2.eq.tractionBC ) then
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
            end if ! bcTypes
          end if ! gridType
        endEdgeMacro()
c        end if

c.. set exact corner conditions for now
      if( setCornersWithTZ .and.twilightZone.ne.0 ) then ! *wdh* 090909
       write(*,'(" bcOptSmFOS3D: INFO set exact values on corners")')
      beginLoopOverCorners3d(0)
        ! *wdh* Need to check the boundaryConditions on the adjacent faces before applying these values: 
        if( mask(i1,i2,i3).gt.0 ) then
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,ue )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,ve )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,we )
              
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v1c,v1e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v2c,v2e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v3c,v3e )
              
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s11c,tau11e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s21c,tau21e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s31c,tau31e )
              
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s12c,tau12e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s22c,tau22e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s32c,tau32e )
              
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s13c,tau13e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s23c,tau23e )
          call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s33c,tau33e )

          u(i1,i2,i3,uc) = ue
          u(i1,i2,i3,vc) = ve
          u(i1,i2,i3,wc) = we

          u(i1,i2,i3,v1c) = v1e
          u(i1,i2,i3,v2c) = v2e
          u(i1,i2,i3,v3c) = v3e

          u(i1,i2,i3,s11c) = tau11e
          u(i1,i2,i3,s21c) = tau21e
          u(i1,i2,i3,s31c) = tau31e

          u(i1,i2,i3,s12c) = tau12e
          u(i1,i2,i3,s22c) = tau22e
          u(i1,i2,i3,s32c) = tau32e

          u(i1,i2,i3,s13c) = tau13e
          u(i1,i2,i3,s23c) = tau23e
          u(i1,i2,i3,s33c) = tau33e
        end if
      endLoopOverCorners3d()
      end if
c*******
c******* re-extrapolation components of stress to first ghost line ********
c*******

        beginLoopOverSides(numGhost,numGhost)
          if( boundaryCondition(side,axis).eq.tractionBC.or.boundaryCondition(side,axis).eq.slipWall ) then
            beginLoops3d()
            if( mask(i1,i2,i3).ne.0 ) then
                u(i1-is1,i2-is2,i3-is3,s11c) = extrap3(u,i1,i2,i3,s11c,is1,is2,is3)
                u(i1-is1,i2-is2,i3-is3,s12c) = extrap3(u,i1,i2,i3,s12c,is1,is2,is3)
                u(i1-is1,i2-is2,i3-is3,s13c) = extrap3(u,i1,i2,i3,s13c,is1,is2,is3)

                u(i1-is1,i2-is2,i3-is3,s21c) = extrap3(u,i1,i2,i3,s21c,is1,is2,is3)
                u(i1-is1,i2-is2,i3-is3,s22c) = extrap3(u,i1,i2,i3,s22c,is1,is2,is3)
                u(i1-is1,i2-is2,i3-is3,s23c) = extrap3(u,i1,i2,i3,s23c,is1,is2,is3)

                u(i1-is1,i2-is2,i3-is3,s31c) = extrap3(u,i1,i2,i3,s31c,is1,is2,is3)
                u(i1-is1,i2-is2,i3-is3,s32c) = extrap3(u,i1,i2,i3,s32c,is1,is2,is3)
                u(i1-is1,i2-is2,i3-is3,s33c) = extrap3(u,i1,i2,i3,s33c,is1,is2,is3)
              end if
            endLoops3d()

          end if ! bc 
        endLoopOverSides()

c.. set the corners to the exact twilight zone function for testing ... CHANGE ME!! ...
        if( .false. ) then
        beginLoopOverEdges3d(1)
          if( mask(i1,i2,i3).gt.0 ) then
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,ue )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,ve )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,we )
              
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v1c,v1e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v2c,v2e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v3c,v3e )
              
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s11c,tau11e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s21c,tau21e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s31c,tau31e )
              
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s12c,tau12e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s22c,tau22e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s32c,tau32e )
              
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s13c,tau13e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s23c,tau23e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s33c,tau33e )

            u(i1,i2,i3,uc) = ue
            u(i1,i2,i3,vc) = ve
            u(i1,i2,i3,wc) = we

            u(i1,i2,i3,v1c) = v1e
            u(i1,i2,i3,v2c) = v2e
            u(i1,i2,i3,v3c) = v3e

            u(i1,i2,i3,s11c) = tau11e
            u(i1,i2,i3,s21c) = tau21e
            u(i1,i2,i3,s31c) = tau31e

            u(i1,i2,i3,s12c) = tau12e
            u(i1,i2,i3,s22c) = tau22e
            u(i1,i2,i3,s32c) = tau32e

            u(i1,i2,i3,s13c) = tau13e
            u(i1,i2,i3,s23c) = tau23e
            u(i1,i2,i3,s33c) = tau33e
          end if
        endLoopOverEdges3d()
      end if

c*******
c******* Extrapolation to the second ghost line ********
c*******

        beginLoopOverSides(numGhost,numGhost)
         if( boundaryCondition(side,axis).gt.0 ) then
           beginLoops3d()
             if( mask(i1,i2,i3).ne.0 ) then
               do n=0,numberOfComponents-1
                 u(i1-2*is1,i2-2*is2,i3-2*is3,n)=extrap3(u,i1-is1,i2-is2,i3-is3,n,is1,is2,is3)
               end do
             end if
           endLoops3d()
         end if ! bc 
        endLoopOverSides()

c..extrapolate the 2nd ghost line near the corners.
        do side1=0,1
          i1 = gridIndexRange(side1,axis1)
          is1 = 1-2*side1
          do side2=0,1
            i2 = gridIndexRange(side2,axis2)
            is2 = 1-2*side2
            do side3=0,1
              i3 = gridIndexRange(side3,axis3)
              is3 = 1-2*side3

c extrapolate in the i1 direction
              if( boundaryCondition(side1,axis1).gt.0 ) then
                if( mask(i1,i2,i3).ne.0 ) then
                  do n=0,numberOfComponents-1
                    u(i1-2*is1,i2-is2,i3-is3,n)=extrap3(u,i1-is1,i2-is2,i3-is3,n,is1,0,0)
                  end do
                end if
              end if

c extrapolate in the i2 direction
              if( boundaryCondition(side2,axis2).gt.0 ) then
                if( mask(i1,i2,i3).ne.0 ) then
                  do n=0,numberOfComponents-1
                    u(i1-is1,i2-2*is2,i3-is3,n)=extrap3(u,i1-is1,i2-is2,i3-is3,n,0,is2,0)
                  end do
                end if
              end if

c extrapolate in the i3 direction
              if( boundaryCondition(side3,axis3).gt.0 ) then
                if( mask(i1,i2,i3).ne.0 ) then
                  do n=0,numberOfComponents-1
                    u(i1-is1,i2-is2,i3-2*is3,n)=extrap3(u,i1-is1,i2-is2,i3-is3,n,0,0,is3)
                  end do
                end if
              end if

c extrapolate in the diagonal direction
              if( boundaryCondition(side1,axis1).gt.0.and.boundaryCondition(side2,axis2).gt.0.and.boundaryCondition(side3,axis3).gt.0) then
                if( mask(i1,i2,i3).ne.0 ) then
                  do n=0,numberOfComponents-1
                    u(i1-2*is1,i2-2*is2,i3-2*is3,n)=extrap3(u,i1-is1,i2-is2,i3-is3,n,is1,is2,is3)
                  end do
                end if
              end if
            end do
          end do
        end do

        if( .false. ) then
c.. set the corners to the exact twilight zone function for testing ... CHANGE ME!! ...
        beginLoopOverEdges3d(1)
          if( mask(i1,i2,i3).gt.0 ) then
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,uc,ue )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,vc,ve )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,wc,we )
              
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v1c,v1e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v2c,v2e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,v3c,v3e )
              
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s11c,tau11e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s21c,tau21e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s31c,tau31e )
              
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s12c,tau12e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s22c,tau22e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s32c,tau32e )
              
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s13c,tau13e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s23c,tau23e )
            call ogDeriv( ep,0,0,0,0,xy(i1,i2,i3,0),xy(i1,i2,i3,1),xy(i1,i2,i3,2),t,s33c,tau33e )

            u(i1,i2,i3,uc) = ue
            u(i1,i2,i3,vc) = ve
            u(i1,i2,i3,wc) = we

            u(i1,i2,i3,v1c) = v1e
            u(i1,i2,i3,v2c) = v2e
            u(i1,i2,i3,v3c) = v3e

            u(i1,i2,i3,s11c) = tau11e
            u(i1,i2,i3,s21c) = tau21e
            u(i1,i2,i3,s31c) = tau31e

            u(i1,i2,i3,s12c) = tau12e
            u(i1,i2,i3,s22c) = tau22e
            u(i1,i2,i3,s32c) = tau32e

            u(i1,i2,i3,s13c) = tau13e
            u(i1,i2,i3,s23c) = tau23e
            u(i1,i2,i3,s33c) = tau33e
          end if
        endLoopOverEdges3d()
      end if

