
c These next include files will define the macros that will define the difference approximations
c The actual macro is called below
#Include "defineDiffOrder2f.h"
#Include "defineDiffOrder4f.h"

#Include "commonMacros.h"

#beginMacro beginLoops()
 do i3=n3a,n3b
 do i2=n2a,n2b
 do i1=n1a,n1b
  if( mask(i1,i2,i3).ne.0 )then
#endMacro

#beginMacro endLoops()
  end if
 end do
 end do
 end do
#endMacro

c **************************************************************
c   Macro to compute Baldwin-Lomax Turbulent viscosity (from "lineSolveBL.h" )
c **************************************************************
#beginMacro computeBLNuT(u,nc)

 do i3=n3a,n3b  
 do i2=n2a,n2b
 do i1=n1a,n1b
  u(i1,i2,i3,nc)=nu !  give a default value to all points 
 end do
 end do
 end do

 maxvt=0  ! holds the maxium value for nuT 
 indexRange(0,0)=n1a
 indexRange(1,0)=n1b
 indexRange(0,1)=n2a
 indexRange(1,1)=n2b
 indexRange(0,2)=n3a
 indexRange(1,2)=n3b
 ! assign loop variables to correspond to the boundary

 do axis=0,nd-1
 do side=0,1
  ! write(*,*) "BL side, axis, bc ",side,axis,boundaryCondition(side,axis)
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
    ! ibe=indexRange(1,axis)-1   ! wdh: Why is there a -1 here ??
    ibe=indexRange(1,axis)  ! *wdh* 
    !  write(*,*) ibb,ibe

    ! loop over points on the boundary 
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
        !   ctrans=1
        ! write(*,*) i1,i2,i3,ctrans

       ! compute the normal to the boundary 
       norm(1) = 0
       norm(2) = 0
       norm(3) = 0
       if ( gridType.eq.rectangular ) then
         norm(axis+1)=2*side-1
       else
        norm(1) = rxi(axis,0)
        norm(2) = rxi(axis,1)
        if ( nd.eq.3 )norm(3) = rxi(axis,2)
       end if
       nmag=sqrt(norm(1)**2+norm(2)**2+norm(3)**2)

       norm(1) = norm(1)/nmag
       norm(2) = norm(2)/nmag
       norm(3) = norm(3)/nmag

       ! first compute ftan = normal.( D_i u_j + D_j u_i )
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

       ! Now compute tangential part by subtracting off the normal component
       fdotn = ftan(1)*norm(1)+ftan(2)*norm(2)+ftan(3)*norm(3)
       ftan(1) = ftan(1) - norm(1)*fdotn
       ftan(2) = ftan(2) - norm(2)*fdotn
       ftan(3) = ftan(3) - norm(3)*fdotn

       ! Here is the wall shear stress: 
       tauw=nu*sqrt(ftan(1)**2+ftan(2)**2+ftan(3)**2)
        
       ! yplus = y*yscale
       yscale = sqrt(tauw)/nu ! assuming density=1 here...

       ymax=0
       lmixmax=0
       lmix2max=0

       ! maxumag = max_y ( |u| ) 
       ! *wdh* maxumag=0
       magumax=0.
       ulmax=0.

       ! Only assign points that are closer to this wall then any other wall 
       ibe = indexRange(1,axis)
       i = ibb 
       do while( i.le.ibe ) 
       
        i1 = ii1 + io(1)*i
        i2 = ii2 + io(2)*i
        i3 = ii3 + io(3)*i
        i1p= i1 + io(1)
        i2p= i2 + io(2)
        i3p= i3 + io(3)
        if( dw(i1p,i2p,i3p).le.dw(i1,i2,i3) )then
          ibe=min(i+1,ibe)  ! choose ibe=i+1 to make sure there is a bit of overlap 
          i=ibe+1
        end if
        i=i+1
       end do 
       ! write(*,'("BL:  i=",3i3," ibb,ibe=",2i3)') ii1,ii2,ii3,ibb,ibe

       do i=ibb,ibe

        i1 = ii1 + io(1)*i
        i2 = ii2 + io(2)*i
        i3 = ii3 + io(3)*i

        ! compute the norm of the vorticity:
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

        ! lmix = kappa y ( 1 - exp( y+/A+ ) )
        ! nuT(inner) = lmixw = lmix^2 * w 
        ! wdh lmixw = vort* kbl*kbl*dw(i1,i2,i3)*dw(i1,i2,i3)*(1.-exp(-yplus/a0p))**2
        kappaF = kbl*dw(i1,i2,i3)*(1.-exp(-yplus/a0p)) ! wdh 
        lmixw = vort*kappaF**2 
        
        !  write(*,*) "yplus, vort ",yplus, vort
        !  write(*,*) "dw, yscale, yplus, lmixw  is ",dw(i1,i2,i3),"  ",yscale," ",yplus," " ,lmixw
        magu = u(i1,i2,i3,uc)**2 + u(i1,i2,i3,vc)**2 

        if ( nd.eq.3 ) magu = magu + u(i1,i2,i3,wc)**2
        
        ! magumax = max(magu,maxumag)  *wdh* there was a bug here I think : there was both magumax and maxumag
        magumax = max(magu,magumax)

        ! F(y) =  y w ( 1 - exp( y+/A+ ) )
        ! lmixmax = max_y kappa*F(y) : occurs at y=ymax, ulmax=|u|(ymax) 
        ! wdh if ( (vort*kbl*dw(i1,i2,i3)*(1.-exp(-yplus/a0p))).gt.lmixmax ) then
        if( (vort*kappaF).gt.lmixmax ) then
          ymax = dw(i1,i2,i3)
          ulmax = magu
          ! wdh lmixmax = vort*kbl*dw(i1,i2,i3)*(1.-exp(-yplus/a0p))
          lmixmax = vort*kappaF
          lmix2max = lmixw
          !   write(*,*) "--",i,ymax,lmixmax,lmix2max
        end if
           
        ! save nuT = nuT(inner)
        u(i1,i2,i3,nc) = lmixw

       end do ! i=ibb,ibe

       ! now that we know lmixmax, ulmax and maxumag we can compute the eddy viscosity

       magumax = sqrt(magumax)
       ulmax = sqrt(ulmax)
       ! NOTE: Wilcox says to take ulamx=0 for boundary layer flows ??  ************* check this **********
       ulmax=0.  ! *wdh* 

       !  write(*,*) "ymax is ",ymax," lmix2max ",lmix2max
       iswitch=0
       do i=ibb,ibe
        
        i1 = ii1 + io(1)*i
        i2 = ii2 + io(2)*i
        i3 = ii3 + io(3)*i

        ! vto = nuT(outer) = alpha*Ccp*Fwake*Fkleb(y,ymax/Ckleb)
        !  Fwake = min( ymax*Fmax, Cwk ymax Udif**2/Fmax )
        !  FKleb(y,d) = [ 1 + 5.5 (y/d)^6 ]^{-1}

        ! ulamx = |u| at y=ymax 
        ! maxumag = max |u| 
        ! vto = alpha*ccp*min(ymax*lmixmax/kbl, cwk*ymax*(maxumag-ulmax)**2*kbl/lmixmax) / (1+5.5*(dw(i1,i2,i3)*ckleb/ymax)**6)


        vto = alpha*ccp*min(ymax*lmixmax/kbl, cwk*ymax*(magumax-ulmax)**2*kbl/lmixmax) / (1+5.5*(dw(i1,i2,i3)*ckleb/ymax)**6)

        !  vto = alpha*ccp*ymax*lmixmax/kbl/(1+5.5*(dw(i1,i2,i3)*ckleb/ymax)**6)
        !  write(*,*) (1+5.5*(dw(i1,i2,i3)*ckleb/ymax)**6)
        ! write(*,'("i,j,k, yplus, vti,vto,ymax,dw=",3i4,5(e9.2,1x))') i1,i2,i3,dw(i1,i2,i3)*yscale,u(i1,i2,i3,nc), vto,ymax,dw(i1,i2,i3)

         !  write(*,*) yscale*dw(i1,i2,i3),u(i1,i2,i3,nc),vto,iswitch
        if( (iswitch.eq.0 .and. vto.lt.u(i1,i2,i3,nc)).or. iswitch.gt.0 ) then
          ! switch to nuT(outer) when nuT(outer) = nuT(inner) 
           !  write(*,*) "switched at ",i, u(i1,i2,i3,nc), vto
           u(i1,i2,i3,nc) = vto 
           if ( iswitch.eq.0 ) iswitch = i
        endif

        ! scale by ctrans -- this turns on nuT after the trip point 
        u(i1,i2,i3,nc) = nu + ctrans*u(i1,i2,i3,nc)  ! *wdh* include nu 
        maxvt = max(maxvt,u(i1,i2,i3,nc))

       end do ! i=ibb,ibe


       ! smooth the eddy viscosity a bit near the switch from inner to outter solutions
       do i=max(ibb+1,iswitch-5),min(iswitch+5,ibe-2)

        i1 = ii1 + io(1)*i
        i2 = ii2 + io(2)*i
        i3 = ii3 + io(3)*i

         !  yes, the relaxation coeff. is 1.  I'm just setting it equal to the neighbors now
         !  yes, the i+1 node uses the updated version of the i node's value             
        u(i1,i2,i3,nc) = .5*(u(i1+io(1),i2+io(2),i3+io(3),nc)+u(i1-io(1),i2-io(2),i3-io(3),nc))

         !  also, it seems the region for this smoothing should increase as the boundary
         !  layer increases in order to improve convergence.  +- 5 was chosen through trial and
         !  error but could be made a function of iswitch or ymax for instance.
       enddo

      else
       ! point is before the trip point 
       ! do i=ibb,ibe
       !    i1 = ii1 + io(1)*i
       !    i2 = ii2 + io(2)*i
       !    i3 = ii3 + io(3)*i
       !    u(i1,i2,i3,nc) = nu  ! *wdh* =0. 
       ! end do
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
       
  end if  ! end if( boundaryCondition(side,axis).eq.noSlipWall )
    
 end do                    ! do side
 end do                    ! do axis
 
 write(*,*) "BL : max(nuT) is ",maxvt
#endMacro


c ================================================================================
c Define the Coefficient of Viscosity for the Baldwin-Lomax Model
c
c=================================================================================
#beginMacro GET_BL_VISCOSITY()
 subroutine getBaldwinLomaxViscosity(nd,n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b,\
       mask,rsxy,xy,  u, v, dw, bc, boundaryCondition, ipar, rpar, pdb, ierr )
c======================================================================
c
c nd : number of space dimensions
c
c n1a,n1b,n2a,n2b,n3a,n3b : 
c u : current solution
c v : save results in v(i1,i2,i3,nc). v and u may be the same
c
c dw: distance to wall
c======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,nd4a,nd4b

 real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,nd4a:nd4b)
 real dw(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)
 real xy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1)

 integer mask(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
 integer bc(0:1,0:*),boundaryCondition(0:1,0:*), ierr

 integer ipar(0:*)
 real rpar(0:*)
 double precision pdb  ! pointer to data base
 
 !     ---- local variables -----
 integer m,n,c,i1,i2,i3,orderOfAccuracy,useWhereMask,i1p,i2p,i3p
 integer pc,uc,vc,wc,tc,nc,vsc,grid,side,gridType
 integer twilightZoneFlow
 integer indexRange(0:1,0:2),is1,is2,is3
 real nu,dx(0:2),dr(0:2)


 integer turbulenceModel,noTurbulenceModel
 integer baldwinLomax,spalartAllmaras,kEpsilon,kOmega
 parameter (noTurbulenceModel=0,baldwinLomax=1,kEpsilon=2,kOmega=3,spalartAllmaras=4 )

 integer axis,kd
 real kbl,alpha,a0p,ccp,ckleb,cwk !baldwin-lomax constants
 real magu,magumax,ymax,ulmax,lmixw,lmixmax,lmix2max,vto,vort,fdotn,tawu ! baldwin-lomax tmp variables
 real yscale,yplus,nmag,ftan(3),norm(3),tauw,maxvt,ctrans,ditrip,kappaF ! more baldwin-lomax tmp variables
 integer iswitch, ibb, ibe, i, ii1,ii2,ii3,io(3) ! baldwin-lomax loop variables
 integer itrip,jtrip,ktrip !baldwin-lomax trip location

 character *50 name
 integer ok,getInt,getReal

 integer \
     noSlipWall,\
     outflow,\
     convectiveOutflow,\
     tractionFree,\
     inflowWithPandTV,\
     dirichletBoundaryCondition,\
     symmetry,\
     axisymmetric
 parameter( noSlipWall=1,outflow=5,convectiveOutflow=14,tractionFree=15,\
  inflowWithPandTV=3,dirichletBoundaryCondition=12,symmetry=11,axisymmetric=13 )

 integer rectangular,curvilinear
 parameter( rectangular=0, curvilinear=1 )

 integer interpolate,dirichlet,neumann,extrapolate
 parameter( interpolate=0, dirichlet=1, neumann=2, extrapolate=3 )

 integer pdeModel,standardModel,BoussinesqModel,viscoPlasticModel
 parameter( standardModel=0,BoussinesqModel=1,viscoPlasticModel=2 )

!     --- begin statement functions

 real rxi
 real uu, ux2,uy2,uz2,ux2c,uy2c,ux3c,uy3c,uz3c
 real rx,ry,rz,sx,sy,sz,tx,ty,tz

 declareDifferenceOrder2(u,RX)

 !.......statement functions for jacobian
 rx(i1,i2,i3)=rsxy(i1,i2,i3,0,0)
 ry(i1,i2,i3)=rsxy(i1,i2,i3,0,1)
 rz(i1,i2,i3)=rsxy(i1,i2,i3,0,2)
 sx(i1,i2,i3)=rsxy(i1,i2,i3,1,0)
 sy(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
 sz(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
 tx(i1,i2,i3)=rsxy(i1,i2,i3,2,0)
 ty(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
 tz(i1,i2,i3)=rsxy(i1,i2,i3,2,2)

 !     The next macro call will define the difference approximation statement functions
 defineDifferenceOrder2Components1(u,RX)

 rxi(m,n) = rsxy(i1,i2,i3,m,n)
 uu(c)    = u(i1,i2,i3,c)

 ux2(c)   = ux22r(i1,i2,i3,c)
 uy2(c)   = uy22r(i1,i2,i3,c)
 uz2(c)   = uz23r(i1,i2,i3,c)

 ux2c(m) = ux22(i1,i2,i3,m)
 uy2c(m) = uy22(i1,i2,i3,m)

 ux3c(m) = ux23(i1,i2,i3,m)
 uy3c(m) = uy23(i1,i2,i3,m)
 uz3c(m) = uz23(i1,i2,i3,m)


 ierr=0
 ! write(*,*) 'Inside getBaldwinLomaxViscosity'

 nc                =ipar(0)
 grid              =ipar(1)
 gridType          =ipar(2)
 orderOfAccuracy   =ipar(3)
 useWhereMask      =ipar(4)
 turbulenceModel   =ipar(5)
 twilightZoneFlow  =ipar(6)
 pdeModel          =ipar(7)

 itrip = ipar(50)
 jtrip = ipar(51)
 ktrip = ipar(52)

 ! write(*,*) "BL itrip,jtrip,ktrip=",itrip,jtrip,ktrip

 dx(0)             =rpar(0)
 dx(1)             =rpar(1)
 dx(2)             =rpar(2)
 dr(0)             =rpar(3)
 dr(1)             =rpar(4)
 dr(2)             =rpar(5)

 ok = getInt(pdb,'uc',uc)  
 if( ok.eq.0 )then
   write(*,'("*** getBaldwinLomaxViscosity: ERROR: uc NOT FOUND")') 
 end if
 ok = getInt(pdb,'vc',vc)  
 if( ok.eq.0 )then
   write(*,'("*** getBaldwinLomaxViscosity: ERROR: vc NOT FOUND")') 
 end if
 ok = getInt(pdb,'wc',wc)  
 if( ok.eq.0 )then
   write(*,'("*** getBaldwinLomaxViscosity: ERROR: wc NOT FOUND")') 
 end if

 ok = getReal(pdb,'nu',nu)  
 if( ok.eq.0 )then
   write(*,'("*** getBaldwinLomaxViscosity: ERROR: nu NOT FOUND")') 
 end if


 ! assign constants for baldwin-lomax  ***** get these from the data base *****
 kbl=.4      ! kappa : Von Karman constant 
 alpha=.0168
 a0p=26.
c   ccp=1.6 : wilcox 
 ccp=2.6619
 ckleb=0.3
c   cwk=1  : wilcox
 cwk=.25

 if ( turbulenceModel.ne.baldwinLomax ) then
   stop 9002
 end if

 if( .false. )then
  ! for testing -- set viscosity equal to nu 
  beginLoops()
   ! v(i1,i2,i3,nc)=nu
   ! v(i1,i2,i3,nc)=nu + .1*( xy(i1,i2,i3,0) + xy(i1,i2,i3,1) )
   ! v(i1,i2,i3,nc)=nu + .1*( xy(i1,i2,i3,0)**2 )
   ! v(i1,i2,i3,nc)=nu + .1*( xy(i1,i2,i3,1)**2 )
   v(i1,i2,i3,nc)=nu + .1*( xy(i1,i2,i3,0)**2 + xy(i1,i2,i3,1)**2 )
  endLoops()
 else
  computeBLNuT(v,nc)
 end if

 return
 end
#endMacro


      GET_BL_VISCOSITY()
