! This file ins included by insLineSolveNew.bf 

c Define the turbulent eddy viscosity and its derivatives for the BL model 
#beginMacro defineBLDerivatives(dim,gt)
  nuT = u(i1,i2,i3,nc)
  #If #gt == "rectangular"
    nuTx(0)=ux2(nc)
    nuTx(1)=uy2(nc)
    #If #dim == "3" 
      nuTx(2)=uz2(nc)
    #End
  #Else
    #If #dim == "2" 
      nuTx(0)=ux2c(nc)
      nuTx(1)=uy2c(nc)
    #Else
      nuTx(0)=ux3c(nc)
      nuTx(1)=uy3c(nc)
      nuTx(2)=uz3c(nc)
    #End
  #End
#endMacro


c Define the turbulent eddy viscosity and its derivatives  
#beginMacro defineValuesBL(dim,gt)
  ! chi3=0.
  defineBLDerivatives(dim,gt)
#endMacro


c **************************************************************
c   Macro to compute Baldwin-Lomax Turbulent viscosity
c **************************************************************
#beginMacro computeBLNuT()

      maxvt=0
      indexRange(0,0)=n1a
      indexRange(1,0)=n1b
      indexRange(0,1)=n2a
      indexRange(1,1)=n2b
      indexRange(0,2)=n3a
      indexRange(1,2)=n3b
      ! assign loop variables to correspond to the boundary

      do axis=0,nd-1
      do side=0,1
c         write(*,*) "SIDE, AXIS, BC ",side,axis,boundaryCondition(side,axis)
         if( boundaryCondition(side,axis).eq.noSlipWall )then
            is1=0
            is2=0
            is3=0
            if( axis.eq.0 )then
               is1=1-2*side
               n1a=indexRange(side,axis) !-is1 ! boundary is 1 pt outside
               n1b=n1a
            else if( axis.eq.1 )then
               is2=1-2*side
               n2a=indexRange(side,axis) !-is2
               n2b=n2a
            else
               is3=1-2*side
               n3a=indexRange(side,axis) !-is3
               n3b=n3a
            end if

            io(1)=0
            io(2)=0
            io(3)=0
            io(axis+1)=1-2*side

            ibb=indexRange(0,axis)
            ibe=indexRange(1,axis)-1
c            write(*,*) ibb,ibe

            do ii3=n3a,n3b
            do ii2=n2a,n2b
            do ii1=n1a,n1b

            if ( ii3.ge.ktrip .and. ii2.ge.jtrip .and. ii1.ge.itrip ) then
             i1 = ii1
             i2 = ii2
             i3 = ii3

             if ( nd.eq.2 ) then
                if ( axis.eq.0 ) then
                   ditrip = ii2-jtrip
                else 
                   ditrip = ii1-itrip
                endif
             else
                if ( axis.eq.0 ) then
                   ditrip = min((ii3-ktrip),(ii2-jtrip))
                else if ( axis.eq.1 ) then
                   ditrip = min((ii1-itrip),(ii3-ktrip))
                else
                   ditrip = min((ii1-itrip),(ii2-jtrip))
                endif
             endif

             ctrans = (1-exp(-ditrip/3.))**2
c             ctrans=1
c             write(*,*) i1,i2,i3,ctrans
            norm(1) = 0
            norm(2) = 0
            norm(3) = 0

            norm(1) = rxi(axis,0)
            norm(2) = rxi(axis,1)
            if ( nd.eq.3 )norm(3) = rxi(axis,2)

            nmag=sqrt(norm(1)*norm(1)+norm(2)*norm(2)+norm(3)*norm(3))

            norm(1) = norm(1)/nmag
            norm(2) = norm(2)/nmag
            norm(3) = norm(3)/nmag

            ftan(1) = 0
            ftan(2) = 0
            ftan(3) = 0

            if ( nd.eq.2 ) then

               if ( gridType.eq.rectangular ) then

                ftan(1) = 2*norm(1)*ux2(uc) + norm(2)*(ux2(vc)+uy2(uc))
                ftan(2) = norm(1)*(uy2(uc)+ux2(vc)) + 2*norm(2)*uy2(vc)

               else

             ftan(1) = 2*norm(1)*ux2c(uc) + norm(2)*(ux2c(vc)+uy2c(uc))
             ftan(2) = norm(1)*(uy2c(uc)+ux2c(vc)) + 2*norm(2)*uy2c(vc)

               end if
               
            else
               
               if ( gridType.eq.rectangular ) then

                 ftan(1)=2*norm(1)*ux2(uc)+norm(2)*(ux2(vc)+uy2(uc)) + norm(3)*(ux2(wc)+uz2(uc))

                 ftan(2)=norm(1)*(ux2(vc)+uy2(uc)) + 2*norm(2)*uy2(vc) + norm(3)*(uy2(wc)+uz2(vc))

                 ftan(3)=norm(1)*(ux2(wc)+uz2(uc)) + norm(2)*(uy2(wc)+uz2(vc)) + 2*norm(3)*uz2(wc)

               else

                  ftan(1)=2*norm(1)*ux3c(uc)+ norm(2)*(ux3c(vc)+uy3c(uc)) + norm(3)*(ux3c(wc)+uz3c(uc))
                  
                  ftan(2)=norm(1)*(ux3c(vc)+uy3c(uc)) + 2*norm(2)*uy3c(vc) +  norm(3)*(uy3c(wc)+uz3c(vc))
                  
                  ftan(3)=norm(1)*(ux3c(wc)+uz3c(uc)) + norm(2)*(uy3c(wc)+uz3c(vc)) + 2*norm(3)*uz3c(wc)
                  
               end if

            end if

            fdotn = ftan(1)*norm(1)+ftan(2)*norm(2)+ftan(3)*norm(3)

            
            ftan(1) = ftan(1) - norm(1)*fdotn
            ftan(2) = ftan(2) - norm(2)*fdotn
            ftan(3) = ftan(3) - norm(3)*fdotn

          tauw=nu*sqrt(ftan(1)*ftan(1)+ftan(2)*ftan(2)+ftan(3)*ftan(3))
             
c         yplus = y*yscale
          yscale = sqrt(tauw)/nu ! assuming density=1 here...

          ymax=0
          lmixmax=0
          lmix2max=0

          maxumag=0
          ulmax=0

          do i=ibb,ibe

             i1 = ii1 + io(1)*i
             i2 = ii2 + io(2)*i
             i3 = ii3 + io(3)*i
             u(i1,i2,i3,nc) = 0

             if (gridType.eq.rectangular) then
                if (nd.eq.2) then
                   vort = abs(ux2(vc)-uy2(uc))
                else
                   vort = sqrt( (uy2(wc)-uz2(vc))*(uy2(wc)-uz2(vc)) - (ux2(wc)-uz2(uc))*(ux2(wc)-uz2(uc)) + (ux2(vc)-uy2(uc))*(ux2(vc)-uy2(uc)) )
                end if
             else
                if (nd.eq.2) then
                   vort = abs(ux2c(vc)-uy2c(uc))
                else
                   vort = sqrt( (uy3c(wc)-uz3c(vc))*(uy3c(wc)-uz3c(vc))- (ux3c(wc)-uz3c(uc))*(ux3c(wc)-uz3c(uc))+ (ux3c(vc)-uy3c(uc))*(ux3c(vc)-uy3c(uc)))
                end if
             end if                

             yplus = dw(i1,i2,i3)*yscale
             lmixw = vort* kbl*kbl*dw(i1,i2,i3)*dw(i1,i2,i3)*(1.-exp(-yplus/a0p))**2
             
c             write(*,*) "yplus, vort ",yplus, vort
c             write(*,*) "dw, yscale, yplus, lmixw  is ",dw(i1,i2,i3),"  ",yscale," ",yplus," " ,lmixw
             magu = u(i1,i2,i3,uc)*u(i2,i2,i3,uc) + u(i1,i2,i3,vc)*u(i1,i2,i3,vc) 

             if ( nd.eq.3 ) magu = magu + u(i1,i2,i3,wc)*u(i1,i2,i3,wc)
             
             magumax = max(magu,maxumag)

             if ( (vort*kbl*dw(i1,i2,i3)*(1.-exp(-yplus/a0p))).gt.lmixmax ) then
                ymax = dw(i1,i2,i3)
                ulmax = magu
                lmixmax = vort*kbl*dw(i1,i2,i3)*(1.-exp(-yplus/a0p))
                lmix2max = lmixw
c                write(*,*) "--",i,ymax,lmixmax,lmix2max
             end if
                
             u(i1,i2,i3,nc) = lmixw

          end do ! i=ibb,ibe
   
c         now that we know lmixmax, ulmax and maxumag we can compute the eddy viscosity

          magumax = sqrt(magumax)
          ulmax = sqrt(ulmax)

c          write(*,*) "ymax is ",ymax," lmix2max ",lmix2max
          iswitch=0
          do i=ibb,ibe
             
             i1 = ii1 + io(1)*i
             i2 = ii2 + io(2)*i
             i3 = ii3 + io(3)*i

             vto = alpha*ccp*min(ymax*lmixmax/kbl, cwk*ymax*(maxumag-ulmax)*(maxumag-ulmax)*kbl/lmixmax) / (1+5.5*(dw(i1,i2,i3)*ckleb/ymax)**6)
c             vto = alpha*ccp*ymax*lmixmax/kbl/(1+5.5*(dw(i1,i2,i3)*ckleb/ymax)**6)
c             write(*,*) ymax,dw(i1,i2,i3)
c             write(*,*) (1+5.5*(dw(i1,i2,i3)*ckleb/ymax)**6)
c             write(*,*) "i,j,k, yplus, vti, vto ",i1,i2,i3,dw(i1,i2,i3)*yscale,u(i1,i2,i3,nc), vto

c             write(*,*) yscale*dw(i1,i2,i3),u(i1,i2,i3,nc),vto,iswitch
             if ( (iswitch.eq.0 .and. vto.lt.u(i1,i2,i3,nc)).or. iswitch.gt.0 ) then
c                write(*,*) "switched at ",i, u(i1,i2,i3,nc), vto
                u(i1,i2,i3,nc) = vto 

                if ( iswitch.eq.0 ) iswitch = i
             endif

             u(i1,i2,i3,nc) = ctrans*u(i1,i2,i3,nc)
             maxvt = max(maxvt,u(i1,i2,i3,nc))

          end do ! i=ibb,ibe

          ! smooth the eddy viscosity a bit near the switch from inner to outter solutions
          do i=max(ibb+1,iswitch-5),min(iswitch+5,ibe-2)

             i1 = ii1 + io(1)*i
             i2 = ii2 + io(2)*i
             i3 = ii3 + io(3)*i

c            yes, the relaxation coeff. is 1.  I'm just setting it equal to the neighbors now
c            yes, the i+1 node uses the updated version of the i node's value             
             u(i1,i2,i3,nc) = .5*(u(i1+io(1),i2+io(2),i3+io(3),nc)+u(i1-io(1),i2-io(2),i3-io(3),nc))

c            also, it seems the region for this smoothing should increase as the boundary
c            layer increases in order to improve convergence.  +- 5 was chosen through trial and
c            error but could be made a function of iswitch or ymax for instance.
          enddo

          else
             do i=ibb,ibe
                i1 = ii1 + io(1)*i
                i2 = ii2 + io(2)*i
                i3 = ii3 + io(3)*i

                u(i1,i2,i3,nc) = 0
             end do
          end if

          end do ! i3=i3a,i3b
          end do ! i2=i2a,2b
          end do ! i1=i1a,i1b

            ! reset values
            if( axis.eq.0 )then
               n1a=indexRange(0,axis)
               n1b=indexRange(1,axis)
            else if( axis.eq.1 )then
               n2a=indexRange(0,axis)
               n2b=indexRange(1,axis)
            else
               n3a=indexRange(0,axis)
               n3b=indexRange(1,axis)
            end if
            
         end if                 !bc
         
      end do                    ! do side
      end do                    ! do axis
      
c      write(*,*) "maxvt is ",maxvt
#endMacro
