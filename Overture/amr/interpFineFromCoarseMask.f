! This file automatically generated from interpFineFromCoarse.bf with bpp.
! INTERP(WithMask)
!       #If "WithMask" == ""
!       #Else
       subroutine interpFineFromCoarseWithMask ( ndfra,ndfrb,ndfsa,
     & ndfsb,ndfta,ndftb,uf,ndcra,ndcrb,ndcsa,ndcsb,ndcta,ndctb,uc,nd,
     & nra,nrb,nsa,nsb,nta,ntb, width,ratios, ndca,ca,cb, ishift, 
     & centerings, update, mask )
c ==================================================================================
c Interpolate fine grid values from a coarse grid values
c
c  uf : fine grid patch
c  uc : coarse grid patch
c  uf(nra:nrb, nsa:nsb, nta:ntb, ca:ca) - interpolate these values.
c  width : interpolation width 
c ratio(1:3) : refinement ratios in each direction
c ishift(1:3) : if 0 prefer a stencil to the right, if 1 prefer a stencil to the left (when there is a choice)
c              When the width is odd there is a choice of stencil for some points
c centerings(1:3) : 0=vertex centred, 1=cell centred
c update : 0=set uf=interpolant(uc) 1=set uf=uf+interpolant(uc)
c ==================================================================================

       real uf(ndfra:ndfrb,ndfsa:ndfsb,ndfta:ndftb,ndca:*)
       real uc(ndcra:ndcrb,ndcsa:ndcsb,ndcta:ndctb,ndca:*)
       integer ca,cb,width,centerings(*)
       integer ratios(*),ishift(*),update
!       #If "WithMask" ne ""
       integer mask(ndfra:ndfrb,ndfsa:ndfsb,ndfta:ndftb)

       integer i1,i2,i3,j1,j2,j3,c,ratio,ratio1,ratio2,ratio3
       integer centering,centering1,centering2,centering3

c these coefficients are set up for a ratio of 4
       real c2(0:1,0:3)
       real c3(0:2,-2:2)
       real lagrange20,lagrange21
       real lagrange30,lagrange31,lagrange32
       real lagrange40,lagrange41,lagrange42,lagrange43
       real lagrange50,lagrange51,lagrange52,lagrange53,lagrange54
       real interp21,interp31,interp41,interp51,interp52
       save c2,c3
       parameter( c38=3./8., c18=-1./8.,c5b32=5./32., c1516=15./16., 
     & c3b32=-3./32. )
       logical ratioEqualsTwoOrFour
       integer vertex,cell
       parameter( vertex=0, cell=1 )
       parameter( maxWidth=10 )  ! maximum interpolation width -- just increase this value to do larger widths
       real cl1(0:maxWidth-1),cl2(0:maxWidth-1),cl3(0:maxWidth-1)

c.. begin statement functions
       lagrange20(r)=1.-r
       lagrange21(r)=r

       interp21(j1,j2,j3)=c210*uc(j1  ,j2,j3,c)+c211*uc(j1+1,j2,j3,c)

       lagrange30(r)=       r*(r-1.)/2.
       lagrange31(r)=(r+1.)  *(r-1.)/(-1.)
       lagrange32(r)=(r+1.)*r       /2.

       interp31(j1,j2,j3)=c310*uc(j1  ,j2,j3,c)+c311*uc(j1+1,j2,j3,c)+
     & c312*uc(j1+2,j2,j3,c)

       lagrange40(r)=       r*(r-1.)*(r-2.)/(-6.)
       lagrange41(r)=(r+1.)  *(r-1.)*(r-2.)/2.
       lagrange42(r)=(r+1.)*r       *(r-2.)/(-2.)
       lagrange43(r)=(r+1.)*r*(r-1.)       /(6.)

       interp41(j1,j2,j3)=c410*uc(j1  ,j2,j3,c)+c411*uc(j1+1,j2,j3,c)+
     & c412*uc(j1+2,j2,j3,c)+ c413*uc(j1+3,j2,j3,c)
c  lagrange polynomials, order 5 on [-2,-1,0,1,2] 
       lagrange50(r)=       (r+1.)*r*(r-1.)*(r-2.)/24.
       lagrange51(r)=(r+2.)       *r*(r-1.)*(r-2.)/(-6.)
       lagrange52(r)=(r+2.)*(r+1.)  *(r-1.)*(r-2.)/4.
       lagrange53(r)=(r+2.)*(r+1.)*r       *(r-2.)/(-6.)
       lagrange54(r)=(r+2.)*(r+1.)*r*(r-1.)       /24.

       interp51(j1,j2,j3)=c510*uc(j1  ,j2,j3,c)+c511*uc(j1+1,j2,j3,c)+
     & c512*uc(j1+2,j2,j3,c)+ c513*uc(j1+3,j2,j3,c)+c514*uc(j1+4,j2,
     & j3,c)
       interp52(j1,j2,j3)=c520*uc(j1,j2  ,j3,c)+c521*uc(j1,j2+1,j3,c)+
     & c522*uc(j1,j2+2,j3,c)+ c523*uc(j1,j2+3,j3,c)+c524*uc(j1,j2+4,
     & j3,c)

c.. end statement functions


       data c2/1.,0., .75,.25, .5,.5, .25,.75/
       data c3/c38,.75,c18, c5b32,c1516,c3b32, 0.,1.,0., c3b32,c1516,
     & c5b32, c18,.75,c38/


       if( width.gt.maxWidth )then
         write(*,*) 'interpFineFromCoarse:ERROR:width=',width,' too 
     & large'
        end if

       ratio1=ratios(1)
       ratio2=ratios(2)
       ratio3=ratios(3)

       centering1=centerings(1)
       centering2=centerings(2)
       centering3=centerings(3)
       centering=centering1

       ir1=4/ratio1
       ir2=4/ratio2
       ir3=4/ratio3

       if( nd.eq.1 )then
         ratio2=1
         ratio3=1
         ir2=1
         ir3=1
         ratioEqualsTwoOrFour=ratio1.eq.2 .or. ratio1.eq.4

       else if( nd.eq.2 )then
         ratio3=1
         ir3=1
         ratioEqualsTwoOrFour=(ratio1.eq.2 .or. ratio1.eq.4).and.(
     & ratio2.eq.2 .or. ratio2.eq.4)
       else
         ratioEqualsTwoOrFour=(ratio1.eq.2 .or. ratio1.eq.4).and.(
     & ratio2.eq.2 .or. ratio2.eq.4).and.(ratio3.eq.2 .or. 
     & ratio3.eq.4)
       end if

       ratio=max(ratio1,ratio2,ratio3)

       ir=4/ratio


       i1Shift=ishift(1) ! if equal to 1 to prefer a left shifted stencil when there is a choice
       i2Shift=ishift(2)
       i3Shift=ishift(3)

       i1offset =centering  ! offset to shift i1 to be positive so division by ratio always shifts to the left
       if( nra.lt.0 ) i1offset=i1offset+1-nra/ratio
       i2offset =centering  ! offset to shift i2
       if( nsa.lt.0 ) i2offset=i2offset+1-nsa/ratio
       i3offset =centering  ! offset to shift i3
       if( nta.lt.0 ) i3offset=i3offset+1-nta/ratio

       i3=ndfta ! defaults for 1D and 2D
       j3=ndcta
       i2=ndfsa
       j2=ndcsa

       if( update.eq.0 )then
c    Here we set uf(i1,i2,i3,c)=interpolant(uc)
         if( nd.eq.2 )then
! INTERP_LOOPS(2,uf(i1,i2,i3,c)=)

          if( width.eq.2 .and. centering.eq.vertex .and. 
     & ratioEqualsTwoOrFour )then

!           #If "2" == "1"
!           #Elif "2" == "2"
! interp2dWidth2RatioTwoOrFour(2,uf(i1,i2,i3,c)=,uf(i1,i2,i3,c)= c2(0,m2)*( c2(0,m1)*uc(j1,j2  ,j3,c)+c2(1,m1)*uc(j1+1,j2  ,j3,c) )+c2(1,m2)*( c2(0,m1)*uc(j1,j2+1,j3,c)+c2(1,m1)*uc(j1+1,j2+1,j3,c) )

            if( nd.eq.2 .and. ratio1.eq.2 .and. ratio2.eq.2 )then
!             #If "2" eq "2"
c .... special case for 2d, ratio 2
c  4 cases: 
c      uf(i1,i2,i3,c)=uc(j1,j2,j3,c) if mod(i1,2).eq.0 and mod(i2,2).eq.0 
c      uf(i1,i2,i3,c)=.5*(uc(j1,j2,j3,c)+uc(j1+1,j2,j3,c) 
c      uf(i1,i2,i3,c)=.5*(uc(j1,j2,j3,c)+uc(j1,j2+1,j3,c) 
c      uf(i1,i2,i3,c)=.25*(uc(j1,j2,j3,c)+uc(j1+1,j2,j3,c)+uc(j1,j2+1,j3,c)+uc(j1+1,j2+1,j3,c))
            if( .true. )then
            msa=nsa+mod(nsa+32,2)  ! msa is even (add 32 to be make nsa+32 positive)
            msb=nsb-mod(nsb+32,2)  ! msb is even
            mra=nra+mod(nra+32,2)  ! mra is even
            mrb=nrb-mod(nrb+32,2)  ! mrb is even
            do c=ca,cb
            do i2=msa,msb,2
             j2=i2/2
             do i1=mra,mrb,2
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
               j1=i1/2  ! could add 1 instead??
               uf(i1,i2,i3,c)= uc(j1,j2,j3,c)  ! i1,i2 even
! endCheckForMask()
              endif
             end do
            end do
            end do
            mra=nra+mod(nra+31,2)  ! mra is odd
            mrb=nrb-mod(nrb+31,2)  ! mrb is odd
            do c=ca,cb
            do i2=msa,msb,2
             j2=i2/2
             do i1=mra,mrb,2
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
               j1=i1/2
               uf(i1,i2,i3,c)= .5*(uc(j1,j2,j3,c)+uc(j1+1,j2,j3,c))  ! i1 odd, i2 even
! endCheckForMask()
              endif
             end do
            end do
            end do
            msa=nsa+mod(nsa+31,2)
            msb=nsb-mod(nsb+31,2)
            mra=nra+mod(nra+32,2)
            mrb=nrb-mod(nrb+32,2)
            do c=ca,cb
            do i2=msa,msb,2
             j2=i2/2
             do i1=mra,mrb,2
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
               j1=i1/2
               uf(i1,i2,i3,c)= .5*(uc(j1,j2,j3,c)+uc(j1,j2+1,j3,c))   ! i1 even i2 odd
! endCheckForMask()
              endif
             end do
            end do
            end do
            mra=nra+mod(nra+31,2)
            mrb=nrb-mod(nrb+31,2)
            do c=ca,cb
            do i2=msa,msb,2
             j2=i2/2
             do i1=mra,mrb,2
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
               j1=i1/2
               uf(i1,i2,i3,c)= .25*(uc(j1,j2,j3,c)+uc(j1+1,j2,j3,c)+uc(
     & j1,j2+1,j3,c)+uc(j1+1,j2+1,j3,c))
! endCheckForMask()
              endif
             end do
            end do
            end do

            else
            do c=ca,cb
            do i2=nsa,nsb
             j2=(i2+20)/2-10
             m2=(i2-j2*2)*ir2
             do i1=nra,nrb
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+20)/2-10
               m1=(i1-j1*2)*ir1 ! 0 <= m1 <r
! innerLoop(uf(i1,i2,i3,c)=c2(0,m2)*(c2(0,m1)*uc(j1,j2,j3,c)+c2(1,m1)*uc(j1+1,j2,j3,c))+c2(1,m2)*(c2(0,m1)*uc(j1,j2+1,j3,c)+c2(1,m1)*uc(j1+1,j2+1,j3,c)))
               ! do c=ca,cb
                 uf(i1,i2,i3,c)=c2(0,m2)*(c2(0,m1)*uc(j1,j2,j3,c)+c2(1,
     & m1)*uc(j1+1,j2,j3,c))+c2(1,m2)*(c2(0,m1)*uc(j1,j2+1,j3,c)+c2(1,
     & m1)*uc(j1+1,j2+1,j3,c))
               ! end do
! endCheckForMask()
              endif
             end do
            end do
            end do ! do c
            end if
            else
            do c=ca,cb
!             #If "2" != "1"
            do i2=nsa,nsb
             j2=(i2+ratio2*i2offset)/ratio2-i2offset
             m2=(i2-j2*ratio2)*ir2
             do i1=nra,nrb
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1*i1offset)/ratio1-i1offset
               m1=(i1-j1*ratio1)*ir1 ! 0 <= m1 <r
! innerLoop(uf(i1,i2,i3,c)=c2(0,m2)*(c2(0,m1)*uc(j1,j2,j3,c)+c2(1,m1)*uc(j1+1,j2,j3,c))+c2(1,m2)*(c2(0,m1)*uc(j1,j2+1,j3,c)+c2(1,m1)*uc(j1+1,j2+1,j3,c)))
               ! do c=ca,cb
                 uf(i1,i2,i3,c)=c2(0,m2)*(c2(0,m1)*uc(j1,j2,j3,c)+c2(1,
     & m1)*uc(j1+1,j2,j3,c))+c2(1,m2)*(c2(0,m1)*uc(j1,j2+1,j3,c)+c2(1,
     & m1)*uc(j1+1,j2+1,j3,c))
               ! end do
! endCheckForMask()
              endif
             end do
!             #If "2" != "1"
            end do
            end do ! do c
            end if


          else if( width.eq.3 .and. centering.eq.vertex .and. 
     & ratioEqualsTwoOrFour )then
            ! this verion is about 50% faster than the more general case below using lagrange30 etc.
!           #If "2" == "1"
!           #Elif "2" == "2"
            if( nrb-nra.ge.nsb-nsa )then
! interp2dWidth3RatioTwoOrFour(2,12,1,1,uf(i1,i2,i3,c)= c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3,c)+c3(1,m1)*uc(j1+1,j2  ,j3,c)+c3(2,m1)*uc(j1+2,j2  ,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))
             do c=ca,cb
!              #If "2" == 1
!              #Else
!              #If "12" == "12"
! beginLoopOdd2(1,*ir2)
             do i2=nsa,nsb
              j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -1-i2offset
              m2=(i2-(j2+1)*ratio2) *ir2
! beginLoopOdd1WithMask(1,*ir1)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
               m1=(i1-(j1+1)*ratio1) *ir1
! innerLoop(uf(i1,i2,i3,c)=c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3,c)+c3(
     & 1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))+c3(1,m2)*(c3(
     & 0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(
     & j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*
     & uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c))
                ! end do
! endCheckForMask()
               endif
              end do
             end do
             end do ! do c

            else
! interp2dWidth3RatioTwoOrFour(2,21,1,1,uf(i1,i2,i3,c)= c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3,c)+c3(1,m1)*uc(j1+1,j2  ,j3,c)+c3(2,m1)*uc(j1+2,j2  ,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))
             do c=ca,cb
!              #If "2" == 1
!              #Else
!              #If "21" == "12"
!              #Else
! beginLoopOdd1(1,*ir1)
             do i1=nra,nrb
              j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
              m1=(i1-(j1+1)*ratio1) *ir1
! beginLoopOdd2WithMask(1,*ir2)
              do i2=nsa,nsb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -1-i2offset
               m2=(i2-(j2+1)*ratio2) *ir2
! innerLoop(uf(i1,i2,i3,c)=c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3,c)+c3(
     & 1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))+c3(1,m2)*(c3(
     & 0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(
     & j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*
     & uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c))
                ! end do
! endCheckForMask()
               endif
              end do
             end do
             end do ! do c
            end if

          else if( width.eq.5 )then
!           #If "2" == "1"
!           #Elif "2" == "2"
            if( nrb-nra.ge.nsb-nsa )then
! interp2dWidth5RatioTwoOrFour(2,1,2,2,2,uf(i1,i2,i3,c)= c520*interp51(j1,j2  ,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))
              do c=ca,cb
!               #If "2" != "1"
! beginLoopOdd2(2,)
              do i2=nsa,nsb
               j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -2-i2offset
               m2=(i2-(j2+2)*ratio2)
               r=(m2 +centering*.5*(1-ratio2))/ratio2
               c52 0=lagrange50(r)
               c52 1=lagrange51(r)
               c52 2=lagrange52(r)
               c52 3=lagrange53(r)
               c52 4=lagrange54(r)
!               #If "1" == "1"
! beginLoopOdd1WithMask(2,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -2-i1offset
                m1=(i1-(j1+2)*ratio1)
                 r=(m1 +centering*.5*(1-ratio1 ))/ratio1
                 c51 0=lagrange50(r)
                 c51 1=lagrange51(r)
                 c51 2=lagrange52(r)
                 c51 3=lagrange53(r)
                 c51 4=lagrange54(r)

! innerLoop(uf(i1,i2,i3,c)=c520*interp51(j1,j2,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=c520*interp51(j1,j2,j3)+c521*
     & interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(
     & j1,j2+3,j3)+c524*interp51(j1,j2+4,j3)
                 ! end do
! endCheckForMask()
                endif
               end do
!               #If "2" != "1"
              end do
              end do ! do c
            else
! interp2dWidth5RatioTwoOrFour(2,2,1,2,2,uf(i1,i2,i3,c)= c520*interp51(j1,j2  ,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))
              do c=ca,cb
!               #If "2" != "1"
! beginLoopOdd1(2,)
              do i1=nra,nrb
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -2-i1offset
               m1=(i1-(j1+2)*ratio1)
               r=(m1 +centering*.5*(1-ratio1))/ratio1
               c51 0=lagrange50(r)
               c51 1=lagrange51(r)
               c51 2=lagrange52(r)
               c51 3=lagrange53(r)
               c51 4=lagrange54(r)
!               #If "2" == "1"
!               #Else
! beginLoopOdd2WithMask(2,)
               do i2=nsa,nsb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -2-i2offset
                m2=(i2-(j2+2)*ratio2)
                 r=(m2 +centering*.5*(1-ratio2 ))/ratio2
                 c52 0=lagrange50(r)
                 c52 1=lagrange51(r)
                 c52 2=lagrange52(r)
                 c52 3=lagrange53(r)
                 c52 4=lagrange54(r)

! innerLoop(uf(i1,i2,i3,c)=c520*interp51(j1,j2,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=c520*interp51(j1,j2,j3)+c521*
     & interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(
     & j1,j2+3,j3)+c524*interp51(j1,j2+4,j3)
                 ! end do
! endCheckForMask()
                endif
               end do
!               #If "2" != "1"
              end do
              end do ! do c
            end if

          else if( width.eq.2 )then
c   width=2 but ratio is not 2 or 4 or centering.eq.cell
!           #If "2" == "1"
!           #Elif "2" == "2"
! interp2dWidth2(2,0,uf(i1,i2,i3,c)= c220*interp21(j1,j2  ,j3)+c221*interp21(j1,j2+1,j3))
            do c=ca,cb
!             #If "2" != "1"
! beginLoopEven2(0,0,)
            do i2=nsa,nsb
              j2=(i2+ratio2*i2offset-centering*ratio2/2)/ratio2-
     & i2offset-0
              m2=(i2-(j2+0)*ratio2)
              r=(m2+centering*.5*(1-ratio2))/ratio2 +0
              c220=lagrange20(r)
              c221=lagrange21(r)
! beginLoopEven1WithMask(0,0,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-0
                m1=(i1-(j1+0)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1 +0
                c210=lagrange20(r)
                c211=lagrange21(r)
! innerLoop(uf(i1,i2,i3,c)=c220*interp21(j1,j2,j3)+c221*interp21(j1,j2+1,j3))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=c220*interp21(j1,j2,j3)+c221*interp21(
     & j1,j2+1,j3)
                ! end do

! endCheckForMask()
              endif
              end do
!             #If "2" != "1"
            end do
            end do ! do c
          else if( width.eq.3 )then
c   width=3 but ratio is not 2 or 4
!           #If "2" == "1"
!           #Elif "2" == "2"
! interp2dWidth3(2,uf(i1,i2,i3,c)= c320*interp31(j1,j2,j3)+c321*interp31(j1,j2+1,j3)+c322*interp31(j1,j2+2,j3))
            do c=ca,cb
!             #If "2" != "1"
! beginLoopOdd2(1,)
            do i2=nsa,nsb
             j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -1-i2offset
             m2=(i2-(j2+1)*ratio2)
             r=(m2+centering*.5*(1-ratio2))/ratio2
             c320=lagrange30(r)
             c321=lagrange31(r)
             c322=lagrange32(r)
! beginLoopOdd1WithMask(1,)
             do i1=nra,nrb
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
              j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
              m1=(i1-(j1+1)*ratio1)
               r=(m1+centering*.5*(1-ratio1))/ratio1
               c310=lagrange30(r)
               c311=lagrange31(r)
               c312=lagrange32(r)

! innerLoop(uf(i1,i2,i3,c)=c320*interp31(j1,j2,j3)+c321*interp31(j1,j2+1,j3)+c322*interp31(j1,j2+2,j3))
               ! do c=ca,cb
                 uf(i1,i2,i3,c)=c320*interp31(j1,j2,j3)+c321*interp31(
     & j1,j2+1,j3)+c322*interp31(j1,j2+2,j3)
               ! end do

! endCheckForMask()
              endif
             end do
!             #If "2" != "1"
            end do
            end do ! do c
          else if( width.eq.1 )then
c   width=1 but ratio is not 2 or 4
          do c=ca,cb
!           #If "2" == "1"
!           #Elif "2" == "2"
          do i2=nsa,nsb
           j2=.5+ (i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -i2offset
c write(*,*) ' width=1: i2,j2,ratio2=',i2,j2,ratio2
          do i1=nra,nrb
           j1=.5+ (i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -i1offset
! beginCheckForMask()
             if( mask(i1,i2,i3).gt.0 )then
! innerLoop(uf(i1,i2,i3,c)= uc(j1,j2,j3,c))
             ! do c=ca,cb
               uf(i1,i2,i3,c)=uc(j1,j2,j3,c)
             ! end do
! endCheckForMask()
             endif
            end do
            end do
          end do ! do c
          else if( width.eq.4 )then

!           #If "2" == "1"
!           #Elif "2" == "2"
! interp2dWidth4(2,1,uf(i1,i2,i3,c)= c420*interp41(j1,j2  ,j3)+c421*interp41(j1,j2+1,j3)+c422*interp41(j1,j2+2,j3)+c423*interp41(j1,j2+3,j3))
            do c=ca,cb
!             #If "2" != "1"
! beginLoopEven2(1,0,)
            do i2=nsa,nsb
              j2=(i2+ratio2*i2offset-centering*ratio2/2)/ratio2-
     & i2offset-1
              m2=(i2-(j2+1)*ratio2)
              r=(m2+centering*.5*(1-ratio2))/ratio2 +0
c      write(*,*)' i2,j2,m2,r=',i2,j2,m2,r
              c420=lagrange40(r)
              c421=lagrange41(r)
              c422=lagrange42(r)
              c423=lagrange43(r)
! beginLoopEven1WithMask(1,0,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-1
                m1=(i1-(j1+1)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1 +0
                c410=lagrange40(r)
                c411=lagrange41(r)
                c412=lagrange42(r)
                c413=lagrange43(r)
! innerLoop(uf(i1,i2,i3,c)=c420*interp41(j1,j2,j3)+c421*interp41(j1,j2+1,j3)+c422*interp41(j1,j2+2,j3)+c423*interp41(j1,j2+3,j3))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=c420*interp41(j1,j2,j3)+c421*interp41(
     & j1,j2+1,j3)+c422*interp41(j1,j2+2,j3)+c423*interp41(j1,j2+3,j3)
                ! end do
! endCheckForMask()
              endif
              end do
!             #If "2" != "1"
            end do
            end do ! do c

          else if( mod(width,2).eq.0 )then
c    general case, even width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width

            iw=(width-1)/2
!           #If "2" == "1"
!           #Elif "2" == "2"
! interp2dWidthEven(2,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl2(j)*cl1(i)*uc(j1+i,j2+j,j3,c),uf(i1,i2,i3,c)=)
!             #If "2" == "3"
!             #If "2" == "2" || "2" == "3"
! beginLoopEven2(iw,iw,)
            do i2=nsa,nsb
              j2=(i2+ratio2*i2offset-centering*ratio2/2)/ratio2-
     & i2offset-iw
              m2=(i2-(j2+iw)*ratio2)
              r=(m2+centering*.5*(1-ratio2))/ratio2 +iw
              if( abs(r-iw-.5).gt.0.5)then
                write(*,*) ' ERROR r=',r
              end if
              do i=0,width-1
                call lagrange(width,i,r,cl2(i))
              end do
! beginLoopEven1WithMask(iw,iw,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-iw
                m1=(i1-(j1+iw)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1 +iw
                if( abs(r-iw-.5).gt.0.5)then
                  write(*,*) ' ERROR r=',r
                end if
                do i=0,width-1
                  call lagrange(width,i,r,cl1(i))
                end do

                do c=ca,cb
!                   #If "uf(i1,i2,i3,c)=" eq "uf(i1,i2,i3,c)="
                  uf(i1,i2,i3,c)=0.
!                   #If "2" == "3"
!                   #If "2" == "2" || "2" == "3"
                  do j=0,width-1
                  do i=0,width-1
                    uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl2(j)*cl1(i)*uc(j1+
     & i,j2+j,j3,c)
                  end do
!                   #If "2" == "2" || "2" == "3"
                  end do
!                   #If "2" == "3"
                end do
! endCheckForMask()
                endif
              end do
!             #If "2" == "2" || "2" == "3"
            end do
!             #If "2" == "3"

          else if( mod(width,2).eq.1 )then
c    general case, odd width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width
          iw=(width-1)/2
!           #If "2" == "1"
!           #Elif "2" == "2"
! interp2dWidthOdd(2,1,2,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl2(j)*cl1(i)*uc(j1+i,j2+j,j3,c),uf(i1,i2,i3,c)=)
!             #If "2" == "3"
!             #If "2" == "2" || "2" == "3"
! beginLoopOdd2(iw,)
            do i2=nsa,nsb
             j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -iw-i2offset
             m2=(i2-(j2+iw)*ratio2)
             r=(m2+centering*.5*(1-ratio2))/ratio2 +iw
c write(*,*)' i2,j2,m2,r=',i2,j2,m2,r
             if( abs(r-iw).gt.0.5)then
               write(*,*) ' ERROR r=',r
             end if
             do i=0,width-1
               call lagrange(width,i,r,cl2(i))
             end do
! beginLoopOdd1WithMask(iw,)
             do i1=nra,nrb
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
              j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -iw-i1offset
              m1=(i1-(j1+iw)*ratio1)
               r=(m1+centering*.5*(1-ratio1))/ratio1 +iw
               if( abs(r-iw).gt.0.5)then
                 write(*,*) ' ERROR r=',r
               end if
               do i=0,width-1
                 call lagrange(width,i,r,cl1(i))
               end do
               do c=ca,cb
!                  #If "uf(i1,i2,i3,c)=" eq "uf(i1,i2,i3,c)="
                   uf(i1,i2,i3,c)=0.
!                  #If "2" == "3"
!                  #If "2" == "2" || "2" == "3"
                 do j=0,width-1
                 do i=0,width-1
                   uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl2(j)*cl1(i)*uc(j1+i,
     & j2+j,j3,c)
                 end do
!                  #If "2" == "2" || "2" == "3"
                 end do
!                  #If "2" == "3"
               end do
! endCheckForMask()
               endif
             end do
!             #If "2" == "2" || "2" == "3"
            end do
!             #If "2" == "3"

          else
           write(*,*) 'interpFineFromCoarse:ERROR: interp width=',
     & width,' not implemeted'
          end if
         else if( nd.eq.3 )then
! INTERP_LOOPS(3,uf(i1,i2,i3,c)=)

           if( width.eq.2 .and. centering.eq.vertex .and. 
     & ratioEqualsTwoOrFour )then

!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Elif "3" == "3"
! beginLoopEven3(0,0,*ir3)
            do i3=nta,ntb
              j3=(i3+ratio3*i3offset-centering*ratio3/2)/ratio3-
     & i3offset-0
              m3=(i3-(j3+0)*ratio3) *ir3
              r=(m3+centering*.5*(1-ratio3))/ratio3 +0
! interp2dWidth2RatioTwoOrFour(3,uf(i1,i2,i3,c)=,uf(i1,i2,i3,c)= c2(0,m3)*(c2(0,m2)*( c2(0,m1)*uc(j1,j2  ,j3  ,c)+c2(1,m1)*uc(j1+1,j2  ,j3  ,c) )+c2(1,m2)*( c2(0,m1)*uc(j1,j2+1,j3  ,c)+c2(1,m1)*uc(j1+1,j2+1,j3  ,c) ))+c2(1,m3)*(c2(0,m2)*( c2(0,m1)*uc(j1,j2  ,j3+1,c)+c2(1,m1)*uc(j1+1,j2  ,j3+1,c) ) +c2(1,m2)*( c2(0,m1)*uc(j1,j2+1,j3+1,c)+c2(1,m1)*uc(j1+1,j2+1,j3+1,c) )) )

             if( nd.eq.2 .and. ratio1.eq.2 .and. ratio2.eq.2 )then
!              #If "3" eq "2"
             else
             do c=ca,cb
!              #If "3" != "1"
             do i2=nsa,nsb
              j2=(i2+ratio2*i2offset)/ratio2-i2offset
              m2=(i2-j2*ratio2)*ir2
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1*i1offset)/ratio1-i1offset
                m1=(i1-j1*ratio1)*ir1 ! 0 <= m1 <r
! innerLoop(uf(i1,i2,i3,c)=c2(0,m3)*(c2(0,m2)*(c2(0,m1)*uc(j1,j2,j3,c)+c2(1,m1)*uc(j1+1,j2,j3,c))+c2(1,m2)*(c2(0,m1)*uc(j1,j2+1,j3,c)+c2(1,m1)*uc(j1+1,j2+1,j3,c)))+c2(1,m3)*(c2(0,m2)*(c2(0,m1)*uc(j1,j2,j3+1,c)+c2(1,m1)*uc(j1+1,j2,j3+1,c))+c2(1,m2)*(c2(0,m1)*uc(j1,j2+1,j3+1,c)+c2(1,m1)*uc(j1+1,j2+1,j3+1,c))))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=c2(0,m3)*(c2(0,m2)*(c2(0,m1)*uc(j1,j2,
     & j3,c)+c2(1,m1)*uc(j1+1,j2,j3,c))+c2(1,m2)*(c2(0,m1)*uc(j1,j2+1,
     & j3,c)+c2(1,m1)*uc(j1+1,j2+1,j3,c)))+c2(1,m3)*(c2(0,m2)*(c2(0,
     & m1)*uc(j1,j2,j3+1,c)+c2(1,m1)*uc(j1+1,j2,j3+1,c))+c2(1,m2)*(c2(
     & 0,m1)*uc(j1,j2+1,j3+1,c)+c2(1,m1)*uc(j1+1,j2+1,j3+1,c)))
                ! end do
! endCheckForMask()
               endif
              end do
!              #If "3" != "1"
             end do
             end do ! do c
             end if
            end do


           else if( width.eq.3 .and. centering.eq.vertex .and. 
     & ratioEqualsTwoOrFour )then
             ! this verion is about 50% faster than the more general case below using lagrange30 etc.
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
             if( nrb-nra.ge.nsb-nsa )then
! beginLoopOdd3(1,*ir3)
              do i3=nta,ntb
               j3=(i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -1-i3offset
               m3=(i3-(j3+1)*ratio3) *ir3
! interp2dWidth3RatioTwoOrFour(3,12,1,1,uf(i1,i2,i3,c)= c3(0,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3  ,c)+c3(1,m1)*uc(j1+1,j2  ,j3  ,c)+c3(2,m1)*uc(j1+2,j2  ,j3  ,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3  ,c)+c3(1,m1)*uc(j1+1,j2+1,j3  ,c)+c3(2,m1)*uc(j1+2,j2+1,j3  ,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3  ,c)+c3(1,m1)*uc(j1+1,j2+2,j3  ,c)+c3(2,m1)*uc(j1+2,j2+2,j3  ,c)))+ c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3+1,c)+c3(1,m1)*uc(j1+1,j2  ,j3+1,c)+c3(2,m1)*uc(j1+2,j2  ,j3+1,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(2,m1)*uc(j1+2,j2+1,j3+1,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,c)+c3(1,m1)*uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c)))+ c3(2,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3+2,c)+c3(1,m1)*uc(j1+1,j2  ,j3+2,c)+c3(2,m1)*uc(j1+2,j2  ,j3+2,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(j1+2,j2+1,j3+2,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,m1)*uc(j1+1,j2+2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c))) )
              do c=ca,cb
!               #If "3" == 1
!               #Else
!               #If "12" == "12"
! beginLoopOdd2(1,*ir2)
              do i2=nsa,nsb
               j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -1-i2offset
               m2=(i2-(j2+1)*ratio2) *ir2
! beginLoopOdd1WithMask(1,*ir1)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
                m1=(i1-(j1+1)*ratio1) *ir1
! innerLoop(uf(i1,i2,i3,c)=c3(0,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))+c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+1,c)+c3(1,m1)*uc(j1+1,j2,j3+1,c)+c3(2,m1)*uc(j1+2,j2,j3+1,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(2,m1)*uc(j1+2,j2+1,j3+1,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,c)+c3(1,m1)*uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c)))+c3(2,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+2,c)+c3(1,m1)*uc(j1+1,j2,j3+2,c)+c3(2,m1)*uc(j1+2,j2,j3+2,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(j1+2,j2+1,j3+2,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,m1)*uc(j1+1,j2+2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c))))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=c3(0,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,
     & j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))+
     & c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)
     & +c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,
     & c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))+
     & c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+1,c)+c3(1,m1)*uc(j1+1,
     & j2,j3+1,c)+c3(2,m1)*uc(j1+2,j2,j3+1,c))+c3(1,m2)*(c3(0,m1)*uc(
     & j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(2,m1)*uc(j1+2,
     & j2+1,j3+1,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,c)+c3(1,m1)*
     & uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c)))+c3(2,m3)*(
     & c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+2,c)+c3(1,m1)*uc(j1+1,j2,j3+2,c)
     & +c3(2,m1)*uc(j1+2,j2,j3+2,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+
     & 2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(j1+2,j2+1,j3+2,
     & c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,m1)*uc(j1+1,j2+
     & 2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c)))
                 ! end do
! endCheckForMask()
                endif
               end do
              end do
              end do ! do c
              end do
             else
! beginLoopOdd3(1,*ir3)
              do i3=nta,ntb
               j3=(i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -1-i3offset
               m3=(i3-(j3+1)*ratio3) *ir3
! interp2dWidth3RatioTwoOrFour(3,21,1,1,uf(i1,i2,i3,c)= c3(0,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3  ,c)+c3(1,m1)*uc(j1+1,j2  ,j3  ,c)+c3(2,m1)*uc(j1+2,j2  ,j3  ,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3  ,c)+c3(1,m1)*uc(j1+1,j2+1,j3  ,c)+c3(2,m1)*uc(j1+2,j2+1,j3  ,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3  ,c)+c3(1,m1)*uc(j1+1,j2+2,j3  ,c)+c3(2,m1)*uc(j1+2,j2+2,j3  ,c)))+ c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3+1,c)+c3(1,m1)*uc(j1+1,j2  ,j3+1,c)+c3(2,m1)*uc(j1+2,j2  ,j3+1,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(2,m1)*uc(j1+2,j2+1,j3+1,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,c)+c3(1,m1)*uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c)))+ c3(2,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3+2,c)+c3(1,m1)*uc(j1+1,j2  ,j3+2,c)+c3(2,m1)*uc(j1+2,j2  ,j3+2,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(j1+2,j2+1,j3+2,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,m1)*uc(j1+1,j2+2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c))) )
              do c=ca,cb
!               #If "3" == 1
!               #Else
!               #If "21" == "12"
!               #Else
! beginLoopOdd1(1,*ir1)
              do i1=nra,nrb
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
               m1=(i1-(j1+1)*ratio1) *ir1
! beginLoopOdd2WithMask(1,*ir2)
               do i2=nsa,nsb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -1-i2offset
                m2=(i2-(j2+1)*ratio2) *ir2
! innerLoop(uf(i1,i2,i3,c)=c3(0,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))+c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+1,c)+c3(1,m1)*uc(j1+1,j2,j3+1,c)+c3(2,m1)*uc(j1+2,j2,j3+1,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(2,m1)*uc(j1+2,j2+1,j3+1,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,c)+c3(1,m1)*uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c)))+c3(2,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+2,c)+c3(1,m1)*uc(j1+1,j2,j3+2,c)+c3(2,m1)*uc(j1+2,j2,j3+2,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(j1+2,j2+1,j3+2,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,m1)*uc(j1+1,j2+2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c))))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=c3(0,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,
     & j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))+
     & c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)
     & +c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,
     & c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))+
     & c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+1,c)+c3(1,m1)*uc(j1+1,
     & j2,j3+1,c)+c3(2,m1)*uc(j1+2,j2,j3+1,c))+c3(1,m2)*(c3(0,m1)*uc(
     & j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(2,m1)*uc(j1+2,
     & j2+1,j3+1,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,c)+c3(1,m1)*
     & uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c)))+c3(2,m3)*(
     & c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+2,c)+c3(1,m1)*uc(j1+1,j2,j3+2,c)
     & +c3(2,m1)*uc(j1+2,j2,j3+2,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+
     & 2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(j1+2,j2+1,j3+2,
     & c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,m1)*uc(j1+1,j2+
     & 2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c)))
                 ! end do
! endCheckForMask()
                endif
               end do
              end do
              end do ! do c
              end do
             end if


           else if( width.eq.5 )then
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
             if( nrb-nra.ge.nsb-nsa )then
! beginLoopOdd3(2,)
               do i3=nta,ntb
                j3=(i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -2-i3offset
                m3=(i3-(j3+2)*ratio3)
                r=(m3 +centering*.5*(1-ratio3))/ratio3
                c530=lagrange50(r)
                c531=lagrange51(r)
                c532=lagrange52(r)
                c533=lagrange53(r)
                c534=lagrange54(r)

! interp2dWidth5RatioTwoOrFour(3,1,2,2,2,uf(i1,i2,i3,c)= c530*(c520*interp51(j1,j2  ,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+c531*(c520*interp51(j1,j2  ,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*interp51(j1,j2+2,j3+1)+c523*interp51(j1,j2+3,j3+1)+c524*interp51(j1,j2+4,j3+1))+c532*(c520*interp51(j1,j2  ,j3+2)+c521*interp51(j1,j2+1,j3+2)+c522*interp51(j1,j2+2,j3+2)+c523*interp51(j1,j2+3,j3+2)+c524*interp51(j1,j2+4,j3+2))+c533*(c520*interp51(j1,j2  ,j3+3)+c521*interp51(j1,j2+1,j3+3)+c522*interp51(j1,j2+2,j3+3)+c523*interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+4,j3+3))+c534*(c520*interp51(j1,j2  ,j3+4)+c521*interp51(j1,j2+1,j3+4)+c522*interp51(j1,j2+2,j3+4)+c523*interp51(j1,j2+3,j3+4)+c524*interp51(j1,j2+4,j3+4)) )
               do c=ca,cb
!                #If "3" != "1"
! beginLoopOdd2(2,)
               do i2=nsa,nsb
                j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -2-i2offset
                m2=(i2-(j2+2)*ratio2)
                r=(m2 +centering*.5*(1-ratio2))/ratio2
                c52 0=lagrange50(r)
                c52 1=lagrange51(r)
                c52 2=lagrange52(r)
                c52 3=lagrange53(r)
                c52 4=lagrange54(r)
!                #If "1" == "1"
! beginLoopOdd1WithMask(2,)
                do i1=nra,nrb
! beginCheckForMask()
                 if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -2-i1offset
                 m1=(i1-(j1+2)*ratio1)
                  r=(m1 +centering*.5*(1-ratio1 ))/ratio1
                  c51 0=lagrange50(r)
                  c51 1=lagrange51(r)
                  c51 2=lagrange52(r)
                  c51 3=lagrange53(r)
                  c51 4=lagrange54(r)

! innerLoop(uf(i1,i2,i3,c)=c530*(c520*interp51(j1,j2,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+c531*(c520*interp51(j1,j2,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*interp51(j1,j2+2,j3+1)+c523*interp51(j1,j2+3,j3+1)+c524*interp51(j1,j2+4,j3+1))+c532*(c520*interp51(j1,j2,j3+2)+c521*interp51(j1,j2+1,j3+2)+c522*interp51(j1,j2+2,j3+2)+c523*interp51(j1,j2+3,j3+2)+c524*interp51(j1,j2+4,j3+2))+c533*(c520*interp51(j1,j2,j3+3)+c521*interp51(j1,j2+1,j3+3)+c522*interp51(j1,j2+2,j3+3)+c523*interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+4,j3+3))+c534*(c520*interp51(j1,j2,j3+4)+c521*interp51(j1,j2+1,j3+4)+c522*interp51(j1,j2+2,j3+4)+c523*interp51(j1,j2+3,j3+4)+c524*interp51(j1,j2+4,j3+4)))
                  ! do c=ca,cb
                    uf(i1,i2,i3,c)=c530*(c520*interp51(j1,j2,j3)+c521*
     & interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(
     & j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+c531*(c520*interp51(j1,
     & j2,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*interp51(j1,j2+2,j3+
     & 1)+c523*interp51(j1,j2+3,j3+1)+c524*interp51(j1,j2+4,j3+1))+
     & c532*(c520*interp51(j1,j2,j3+2)+c521*interp51(j1,j2+1,j3+2)+
     & c522*interp51(j1,j2+2,j3+2)+c523*interp51(j1,j2+3,j3+2)+c524*
     & interp51(j1,j2+4,j3+2))+c533*(c520*interp51(j1,j2,j3+3)+c521*
     & interp51(j1,j2+1,j3+3)+c522*interp51(j1,j2+2,j3+3)+c523*
     & interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+4,j3+3))+c534*(c520*
     & interp51(j1,j2,j3+4)+c521*interp51(j1,j2+1,j3+4)+c522*interp51(
     & j1,j2+2,j3+4)+c523*interp51(j1,j2+3,j3+4)+c524*interp51(j1,j2+
     & 4,j3+4))
                  ! end do
! endCheckForMask()
                 endif
                end do
!                #If "3" != "1"
               end do
               end do ! do c
               end do
             else
! beginLoopOdd3(2,)
               do i3=nta,ntb
                j3=(i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -2-i3offset
                m3=(i3-(j3+2)*ratio3)
                r=(m3 +centering*.5*(1-ratio3))/ratio3
                c530=lagrange50(r)
                c531=lagrange51(r)
                c532=lagrange52(r)
                c533=lagrange53(r)
                c534=lagrange54(r)

! interp2dWidth5RatioTwoOrFour(3,2,1,2,2,uf(i1,i2,i3,c)= c530*(c520*interp51(j1,j2  ,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+c531*(c520*interp51(j1,j2  ,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*interp51(j1,j2+2,j3+1)+c523*interp51(j1,j2+3,j3+1)+c524*interp51(j1,j2+4,j3+1))+c532*(c520*interp51(j1,j2  ,j3+2)+c521*interp51(j1,j2+1,j3+2)+c522*interp51(j1,j2+2,j3+2)+c523*interp51(j1,j2+3,j3+2)+c524*interp51(j1,j2+4,j3+2))+c533*(c520*interp51(j1,j2  ,j3+3)+c521*interp51(j1,j2+1,j3+3)+c522*interp51(j1,j2+2,j3+3)+c523*interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+4,j3+3))+c534*(c520*interp51(j1,j2  ,j3+4)+c521*interp51(j1,j2+1,j3+4)+c522*interp51(j1,j2+2,j3+4)+c523*interp51(j1,j2+3,j3+4)+c524*interp51(j1,j2+4,j3+4)) )
               do c=ca,cb
!                #If "3" != "1"
! beginLoopOdd1(2,)
               do i1=nra,nrb
                j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -2-i1offset
                m1=(i1-(j1+2)*ratio1)
                r=(m1 +centering*.5*(1-ratio1))/ratio1
                c51 0=lagrange50(r)
                c51 1=lagrange51(r)
                c51 2=lagrange52(r)
                c51 3=lagrange53(r)
                c51 4=lagrange54(r)
!                #If "2" == "1"
!                #Else
! beginLoopOdd2WithMask(2,)
                do i2=nsa,nsb
! beginCheckForMask()
                 if( mask(i1,i2,i3).gt.0 )then
                 j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -2-i2offset
                 m2=(i2-(j2+2)*ratio2)
                  r=(m2 +centering*.5*(1-ratio2 ))/ratio2
                  c52 0=lagrange50(r)
                  c52 1=lagrange51(r)
                  c52 2=lagrange52(r)
                  c52 3=lagrange53(r)
                  c52 4=lagrange54(r)

! innerLoop(uf(i1,i2,i3,c)=c530*(c520*interp51(j1,j2,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+c531*(c520*interp51(j1,j2,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*interp51(j1,j2+2,j3+1)+c523*interp51(j1,j2+3,j3+1)+c524*interp51(j1,j2+4,j3+1))+c532*(c520*interp51(j1,j2,j3+2)+c521*interp51(j1,j2+1,j3+2)+c522*interp51(j1,j2+2,j3+2)+c523*interp51(j1,j2+3,j3+2)+c524*interp51(j1,j2+4,j3+2))+c533*(c520*interp51(j1,j2,j3+3)+c521*interp51(j1,j2+1,j3+3)+c522*interp51(j1,j2+2,j3+3)+c523*interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+4,j3+3))+c534*(c520*interp51(j1,j2,j3+4)+c521*interp51(j1,j2+1,j3+4)+c522*interp51(j1,j2+2,j3+4)+c523*interp51(j1,j2+3,j3+4)+c524*interp51(j1,j2+4,j3+4)))
                  ! do c=ca,cb
                    uf(i1,i2,i3,c)=c530*(c520*interp51(j1,j2,j3)+c521*
     & interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(
     & j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+c531*(c520*interp51(j1,
     & j2,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*interp51(j1,j2+2,j3+
     & 1)+c523*interp51(j1,j2+3,j3+1)+c524*interp51(j1,j2+4,j3+1))+
     & c532*(c520*interp51(j1,j2,j3+2)+c521*interp51(j1,j2+1,j3+2)+
     & c522*interp51(j1,j2+2,j3+2)+c523*interp51(j1,j2+3,j3+2)+c524*
     & interp51(j1,j2+4,j3+2))+c533*(c520*interp51(j1,j2,j3+3)+c521*
     & interp51(j1,j2+1,j3+3)+c522*interp51(j1,j2+2,j3+3)+c523*
     & interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+4,j3+3))+c534*(c520*
     & interp51(j1,j2,j3+4)+c521*interp51(j1,j2+1,j3+4)+c522*interp51(
     & j1,j2+2,j3+4)+c523*interp51(j1,j2+3,j3+4)+c524*interp51(j1,j2+
     & 4,j3+4))
                  ! end do
! endCheckForMask()
                 endif
                end do
!                #If "3" != "1"
               end do
               end do ! do c
               end do
             end if

           else if( width.eq.2 )then
c   width=2 but ratio is not 2 or 4 or centering.eq.cell
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Elif "3" == "3"
! beginLoopEven3(0,0,)
             do i3=nta,ntb
               j3=(i3+ratio3*i3offset-centering*ratio3/2)/ratio3-
     & i3offset-0
               m3=(i3-(j3+0)*ratio3)
               r=(m3+centering*.5*(1-ratio3))/ratio3 +0
             c230=lagrange20(r)
             c231=lagrange21(r)
! interp2dWidth2(3,0,uf(i1,i2,i3,c)=  c230*(c220*interp21(j1,j2  ,j3  )+c221*interp21(j1,j2+1,j3))+ c231*(c220*interp21(j1,j2  ,j3+1)+c221*interp21(j1,j2+1,j3+1)) )
             do c=ca,cb
!              #If "3" != "1"
! beginLoopEven2(0,0,)
             do i2=nsa,nsb
               j2=(i2+ratio2*i2offset-centering*ratio2/2)/ratio2-
     & i2offset-0
               m2=(i2-(j2+0)*ratio2)
               r=(m2+centering*.5*(1-ratio2))/ratio2 +0
               c220=lagrange20(r)
               c221=lagrange21(r)
! beginLoopEven1WithMask(0,0,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-0
                 m1=(i1-(j1+0)*ratio1)
                 r=(m1+centering*.5*(1-ratio1))/ratio1 +0
                 c210=lagrange20(r)
                 c211=lagrange21(r)
! innerLoop(uf(i1,i2,i3,c)=c230*(c220*interp21(j1,j2,j3)+c221*interp21(j1,j2+1,j3))+c231*(c220*interp21(j1,j2,j3+1)+c221*interp21(j1,j2+1,j3+1)))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=c230*(c220*interp21(j1,j2,j3)+c221*
     & interp21(j1,j2+1,j3))+c231*(c220*interp21(j1,j2,j3+1)+c221*
     & interp21(j1,j2+1,j3+1))
                 ! end do

! endCheckForMask()
               endif
               end do
!              #If "3" != "1"
             end do
             end do ! do c
             end do
           else if( width.eq.3 )then
c   width=3 but ratio is not 2 or 4
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
! beginLoopOdd3(1,)
            do i3=nta,ntb
             j3=(i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -1-i3offset
             m3=(i3-(j3+1)*ratio3)
             r=(m3+centering*.5*(1-ratio3))/ratio3
             c330=lagrange30(r)
             c331=lagrange31(r)
             c332=lagrange32(r)
! interp2dWidth3(3,uf(i1,i2,i3,c)= c330*(c320*interp31(j1,j2,j3  )+c321*interp31(j1,j2+1,j3  )+c322*interp31(j1,j2+2,j3  ))+c331*(c320*interp31(j1,j2,j3+1)+c321*interp31(j1,j2+1,j3+1)+c322*interp31(j1,j2+2,j3+1))+c332*(c320*interp31(j1,j2,j3+2)+c321*interp31(j1,j2+1,j3+2)+c322*interp31(j1,j2+2,j3+2)) )
             do c=ca,cb
!              #If "3" != "1"
! beginLoopOdd2(1,)
             do i2=nsa,nsb
              j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -1-i2offset
              m2=(i2-(j2+1)*ratio2)
              r=(m2+centering*.5*(1-ratio2))/ratio2
              c320=lagrange30(r)
              c321=lagrange31(r)
              c322=lagrange32(r)
! beginLoopOdd1WithMask(1,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
               m1=(i1-(j1+1)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1
                c310=lagrange30(r)
                c311=lagrange31(r)
                c312=lagrange32(r)

! innerLoop(uf(i1,i2,i3,c)=c330*(c320*interp31(j1,j2,j3)+c321*interp31(j1,j2+1,j3)+c322*interp31(j1,j2+2,j3))+c331*(c320*interp31(j1,j2,j3+1)+c321*interp31(j1,j2+1,j3+1)+c322*interp31(j1,j2+2,j3+1))+c332*(c320*interp31(j1,j2,j3+2)+c321*interp31(j1,j2+1,j3+2)+c322*interp31(j1,j2+2,j3+2)))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=c330*(c320*interp31(j1,j2,j3)+c321*
     & interp31(j1,j2+1,j3)+c322*interp31(j1,j2+2,j3))+c331*(c320*
     & interp31(j1,j2,j3+1)+c321*interp31(j1,j2+1,j3+1)+c322*interp31(
     & j1,j2+2,j3+1))+c332*(c320*interp31(j1,j2,j3+2)+c321*interp31(
     & j1,j2+1,j3+2)+c322*interp31(j1,j2+2,j3+2))
                ! end do

! endCheckForMask()
               endif
              end do
!              #If "3" != "1"
             end do
             end do ! do c
            end do
           else if( width.eq.1 )then
c   width=1 but ratio is not 2 or 4
           do c=ca,cb
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
           do i3=nta,ntb
            j3=.5+ (i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -i3offset
           do i2=nsa,nsb
            j2=.5+ (i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -i2offset
           do i1=nra,nrb
            j1=.5+ (i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -i1offset
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
! innerLoop(uf(i1,i2,i3,c)= uc(j1,j2,j3,c))
               ! do c=ca,cb
                 uf(i1,i2,i3,c)=uc(j1,j2,j3,c)
               ! end do
! endCheckForMask()
              endif
             end do
             end do
             end do
           end do ! do c
           else if( width.eq.4 )then

!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
! beginLoopEven3(1,0,)
           do i3=nta,ntb
             j3=(i3+ratio3*i3offset-centering*ratio3/2)/ratio3-
     & i3offset-1
             m3=(i3-(j3+1)*ratio3)
             r=(m3+centering*.5*(1-ratio3))/ratio3 +0
             c430=lagrange40(r)
             c431=lagrange41(r)
             c432=lagrange42(r)
             c433=lagrange43(r)
! interp2dWidth4(3,1, uf(i1,i2,i3,c)= c430*(c420*interp41(j1,j2,j3  )+c421*interp41(j1,j2+1,j3  )+c422*interp41(j1,j2+2,j3  )+c423*interp41(j1,j2+3,j3  ))+c431*(c420*interp41(j1,j2,j3+1)+c421*interp41(j1,j2+1,j3+1)+c422*interp41(j1,j2+2,j3+1)+c423*interp41(j1,j2+3,j3+1))+c432*(c420*interp41(j1,j2,j3+2)+c421*interp41(j1,j2+1,j3+2)+c422*interp41(j1,j2+2,j3+2)+c423*interp41(j1,j2+3,j3+2))+c433*(c420*interp41(j1,j2,j3+3)+c421*interp41(j1,j2+1,j3+3)+c422*interp41(j1,j2+2,j3+3)+c423*interp41(j1,j2+3,j3+3)) )
             do c=ca,cb
!              #If "3" != "1"
! beginLoopEven2(1,0,)
             do i2=nsa,nsb
               j2=(i2+ratio2*i2offset-centering*ratio2/2)/ratio2-
     & i2offset-1
               m2=(i2-(j2+1)*ratio2)
               r=(m2+centering*.5*(1-ratio2))/ratio2 +0
c      write(*,*)' i2,j2,m2,r=',i2,j2,m2,r
               c420=lagrange40(r)
               c421=lagrange41(r)
               c422=lagrange42(r)
               c423=lagrange43(r)
! beginLoopEven1WithMask(1,0,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-1
                 m1=(i1-(j1+1)*ratio1)
                 r=(m1+centering*.5*(1-ratio1))/ratio1 +0
                 c410=lagrange40(r)
                 c411=lagrange41(r)
                 c412=lagrange42(r)
                 c413=lagrange43(r)
! innerLoop(uf(i1,i2,i3,c)=c430*(c420*interp41(j1,j2,j3)+c421*interp41(j1,j2+1,j3)+c422*interp41(j1,j2+2,j3)+c423*interp41(j1,j2+3,j3))+c431*(c420*interp41(j1,j2,j3+1)+c421*interp41(j1,j2+1,j3+1)+c422*interp41(j1,j2+2,j3+1)+c423*interp41(j1,j2+3,j3+1))+c432*(c420*interp41(j1,j2,j3+2)+c421*interp41(j1,j2+1,j3+2)+c422*interp41(j1,j2+2,j3+2)+c423*interp41(j1,j2+3,j3+2))+c433*(c420*interp41(j1,j2,j3+3)+c421*interp41(j1,j2+1,j3+3)+c422*interp41(j1,j2+2,j3+3)+c423*interp41(j1,j2+3,j3+3)))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=c430*(c420*interp41(j1,j2,j3)+c421*
     & interp41(j1,j2+1,j3)+c422*interp41(j1,j2+2,j3)+c423*interp41(
     & j1,j2+3,j3))+c431*(c420*interp41(j1,j2,j3+1)+c421*interp41(j1,
     & j2+1,j3+1)+c422*interp41(j1,j2+2,j3+1)+c423*interp41(j1,j2+3,
     & j3+1))+c432*(c420*interp41(j1,j2,j3+2)+c421*interp41(j1,j2+1,
     & j3+2)+c422*interp41(j1,j2+2,j3+2)+c423*interp41(j1,j2+3,j3+2))+
     & c433*(c420*interp41(j1,j2,j3+3)+c421*interp41(j1,j2+1,j3+3)+
     & c422*interp41(j1,j2+2,j3+3)+c423*interp41(j1,j2+3,j3+3))
                 ! end do
! endCheckForMask()
               endif
               end do
!              #If "3" != "1"
             end do
             end do ! do c
             end do

           else if( mod(width,2).eq.0 )then
c    general case, even width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width

             iw=(width-1)/2
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
! interp2dWidthEven(3,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl3(k)*cl2(j)*cl1(i)*uc(j1+i,j2+j,j3+k,c),uf(i1,i2,i3,c)=)
!              #If "3" == "3"
! beginLoopEven3(iw,iw,)
             do i3=nta,ntb
               j3=(i3+ratio3*i3offset-centering*ratio3/2)/ratio3-
     & i3offset-iw
               m3=(i3-(j3+iw)*ratio3)
               r=(m3+centering*.5*(1-ratio3))/ratio3 +iw
               if( abs(r-iw-.5).gt.0.5)then
                 write(*,*) ' ERROR r=',r
               end if
               do i=0,width-1
                 call lagrange(width,i,r,cl3(i))
               end do
!              #If "3" == "2" || "3" == "3"
! beginLoopEven2(iw,iw,)
             do i2=nsa,nsb
               j2=(i2+ratio2*i2offset-centering*ratio2/2)/ratio2-
     & i2offset-iw
               m2=(i2-(j2+iw)*ratio2)
               r=(m2+centering*.5*(1-ratio2))/ratio2 +iw
               if( abs(r-iw-.5).gt.0.5)then
                 write(*,*) ' ERROR r=',r
               end if
               do i=0,width-1
                 call lagrange(width,i,r,cl2(i))
               end do
! beginLoopEven1WithMask(iw,iw,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-iw
                 m1=(i1-(j1+iw)*ratio1)
                 r=(m1+centering*.5*(1-ratio1))/ratio1 +iw
                 if( abs(r-iw-.5).gt.0.5)then
                   write(*,*) ' ERROR r=',r
                 end if
                 do i=0,width-1
                   call lagrange(width,i,r,cl1(i))
                 end do

                 do c=ca,cb
!                    #If "uf(i1,i2,i3,c)=" eq "uf(i1,i2,i3,c)="
                   uf(i1,i2,i3,c)=0.
!                    #If "3" == "3"
                   do k=0,width-1
!                    #If "3" == "2" || "3" == "3"
                   do j=0,width-1
                   do i=0,width-1
                     uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl3(k)*cl2(j)*cl1(i)
     & *uc(j1+i,j2+j,j3+k,c)
                   end do
!                    #If "3" == "2" || "3" == "3"
                   end do
!                    #If "3" == "3"
                   end do
                 end do
! endCheckForMask()
                 endif
               end do
!              #If "3" == "2" || "3" == "3"
             end do
!              #If "3" == "3"
             end do

           else if( mod(width,2).eq.1 )then
c    general case, odd width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width
           iw=(width-1)/2
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
! interp2dWidthOdd(3,1,2,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl3(k)*cl2(j)*cl1(i)*uc(j1+i,j2+j,j3+k,c),uf(i1,i2,i3,c)=)
!              #If "3" == "3"
! beginLoopOdd3(iw,)
             do i3=nta,ntb
              j3=(i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -iw-i3offset
              m3=(i3-(j3+iw)*ratio3)
              r=(m3+centering*.5*(1-ratio3))/ratio3 +iw
              if( abs(r-iw).gt.0.5)then
                write(*,*) ' ERROR r=',r
              end if
              do i=0,width-1
                call lagrange(width,i,r,cl3(i))
              end do
!              #If "3" == "2" || "3" == "3"
! beginLoopOdd2(iw,)
             do i2=nsa,nsb
              j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -iw-i2offset
              m2=(i2-(j2+iw)*ratio2)
              r=(m2+centering*.5*(1-ratio2))/ratio2 +iw
c write(*,*)' i2,j2,m2,r=',i2,j2,m2,r
              if( abs(r-iw).gt.0.5)then
                write(*,*) ' ERROR r=',r
              end if
              do i=0,width-1
                call lagrange(width,i,r,cl2(i))
              end do
! beginLoopOdd1WithMask(iw,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -iw-i1offset
               m1=(i1-(j1+iw)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1 +iw
                if( abs(r-iw).gt.0.5)then
                  write(*,*) ' ERROR r=',r
                end if
                do i=0,width-1
                  call lagrange(width,i,r,cl1(i))
                end do
                do c=ca,cb
!                   #If "uf(i1,i2,i3,c)=" eq "uf(i1,i2,i3,c)="
                    uf(i1,i2,i3,c)=0.
!                   #If "3" == "3"
                  do k=0,width-1
!                   #If "3" == "2" || "3" == "3"
                  do j=0,width-1
                  do i=0,width-1
                    uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl3(k)*cl2(j)*cl1(i)*
     & uc(j1+i,j2+j,j3+k,c)
                  end do
!                   #If "3" == "2" || "3" == "3"
                  end do
!                   #If "3" == "3"
                  end do
                end do
! endCheckForMask()
                endif
              end do
!              #If "3" == "2" || "3" == "3"
             end do
!              #If "3" == "3"
             end do

           else
            write(*,*) 'interpFineFromCoarse:ERROR: interp width=',
     & width,' not implemeted'
           end if
         else if( nd.eq.1 )then
! INTERP_LOOPS(1,uf(i1,i2,i3,c)=)

           if( width.eq.2 .and. centering.eq.vertex .and. 
     & ratioEqualsTwoOrFour )then

!            #If "1" == "1"
! interp2dWidth2RatioTwoOrFour(1,uf(i1,i2,i3,c)=,uf(i1,i2,i3,c)= c2(0,m1)*uc(j1,j2  ,j3,c)+c2(1,m1)*uc(j1+1,j2  ,j3,c) )

             if( nd.eq.2 .and. ratio1.eq.2 .and. ratio2.eq.2 )then
!              #If "1" eq "2"
             else
             do c=ca,cb
!              #If "1" != "1"
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1*i1offset)/ratio1-i1offset
                m1=(i1-j1*ratio1)*ir1 ! 0 <= m1 <r
! innerLoop(uf(i1,i2,i3,c)=c2(0,m1)*uc(j1,j2,j3,c)+c2(1,m1)*uc(j1+1,j2,j3,c))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=c2(0,m1)*uc(j1,j2,j3,c)+c2(1,m1)*uc(
     & j1+1,j2,j3,c)
                ! end do
! endCheckForMask()
               endif
              end do
!              #If "1" != "1"
             end do ! do c
             end if


           else if( width.eq.3 .and. centering.eq.vertex .and. 
     & ratioEqualsTwoOrFour )then
             ! this verion is about 50% faster than the more general case below using lagrange30 etc.
!            #If "1" == "1"
! interp2dWidth3RatioTwoOrFour(1,12,1,0,uf(i1,i2,i3,c)= c3(0,m1)*uc(j1,j2  ,j3,c)+c3(1,m1)*uc(j1+1,j2  ,j3,c)+c3(2,m1)*uc(j1+2,j2  ,j3,c))
              do c=ca,cb
!               #If "1" == 1
! beginLoopOdd1WithMask(1,*ir1)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
               m1=(i1-(j1+1)*ratio1) *ir1
! innerLoop(uf(i1,i2,i3,c)=c3(0,m1)*uc(j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))
              ! do c=ca,cb
                uf(i1,i2,i3,c)=c3(0,m1)*uc(j1,j2,j3,c)+c3(1,m1)*uc(j1+
     & 1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c)
              ! end do
! endCheckForMask()
              endif
              end do
              end do ! do c

           else if( width.eq.5 )then
!            #If "1" == "1"
! interp2dWidth5RatioTwoOrFour(1,1,2,2,0,uf(i1,i2,i3,c)= interp51(j1,j2,j3))
             do c=ca,cb
!              #If "1" != "1"
!              #If "1" == "1"
! beginLoopOdd1WithMask(2,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -2-i1offset
               m1=(i1-(j1+2)*ratio1)
                r=(m1 +centering*.5*(1-ratio1 ))/ratio1
                c51 0=lagrange50(r)
                c51 1=lagrange51(r)
                c51 2=lagrange52(r)
                c51 3=lagrange53(r)
                c51 4=lagrange54(r)

! innerLoop(uf(i1,i2,i3,c)=interp51(j1,j2,j3))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=interp51(j1,j2,j3)
                ! end do
! endCheckForMask()
               endif
              end do
!              #If "1" != "1"
             end do ! do c

           else if( width.eq.2 )then
c   width=2 but ratio is not 2 or 4 or centering.eq.cell
!            #If "1" == "1"
! interp2dWidth2(1,0,uf(i1,i2,i3,c)= interp21(j1,j2  ,j3))
             do c=ca,cb
!              #If "1" != "1"
! beginLoopEven1WithMask(0,0,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-0
                 m1=(i1-(j1+0)*ratio1)
                 r=(m1+centering*.5*(1-ratio1))/ratio1 +0
                 c210=lagrange20(r)
                 c211=lagrange21(r)
! innerLoop(uf(i1,i2,i3,c)=interp21(j1,j2,j3))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=interp21(j1,j2,j3)
                 ! end do

! endCheckForMask()
               endif
               end do
!              #If "1" != "1"
             end do ! do c
           else if( width.eq.3 )then
c   width=3 but ratio is not 2 or 4
!            #If "1" == "1"
! interp2dWidth3(1,uf(i1,i2,i3,c)= interp31(j1,j2,j3))
             do c=ca,cb
!              #If "1" != "1"
! beginLoopOdd1WithMask(1,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
               m1=(i1-(j1+1)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1
                c310=lagrange30(r)
                c311=lagrange31(r)
                c312=lagrange32(r)

! innerLoop(uf(i1,i2,i3,c)=interp31(j1,j2,j3))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=interp31(j1,j2,j3)
                ! end do

! endCheckForMask()
               endif
              end do
!              #If "1" != "1"
             end do ! do c
           else if( width.eq.1 )then
c   width=1 but ratio is not 2 or 4
           do c=ca,cb
!            #If "1" == "1"
           do i1=nra,nrb
            j1=.5+ (i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -i1offset
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
! innerLoop(uf(i1,i2,i3,c)= uc(j1,j2,j3,c))
              ! do c=ca,cb
                uf(i1,i2,i3,c)=uc(j1,j2,j3,c)
              ! end do
! endCheckForMask()
             endif
             end do
           end do ! do c
           else if( width.eq.4 )then

!            #If "1" == "1"
! interp2dWidth4(1,1,uf(i1,i2,i3,c)= interp41(j1,j2,j3))
             do c=ca,cb
!              #If "1" != "1"
! beginLoopEven1WithMask(1,0,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-1
                 m1=(i1-(j1+1)*ratio1)
                 r=(m1+centering*.5*(1-ratio1))/ratio1 +0
                 c410=lagrange40(r)
                 c411=lagrange41(r)
                 c412=lagrange42(r)
                 c413=lagrange43(r)
! innerLoop(uf(i1,i2,i3,c)=interp41(j1,j2,j3))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=interp41(j1,j2,j3)
                 ! end do
! endCheckForMask()
               endif
               end do
!              #If "1" != "1"
             end do ! do c

           else if( mod(width,2).eq.0 )then
c    general case, even width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width

             iw=(width-1)/2
!            #If "1" == "1"
! interp2dWidthEven(1,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl1(i)*uc(j1+i,j2,j3,c),uf(i1,i2,i3,c)=)
!              #If "1" == "3"
!              #If "1" == "2" || "1" == "3"
! beginLoopEven1WithMask(iw,iw,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-iw
                 m1=(i1-(j1+iw)*ratio1)
                 r=(m1+centering*.5*(1-ratio1))/ratio1 +iw
                 if( abs(r-iw-.5).gt.0.5)then
                   write(*,*) ' ERROR r=',r
                 end if
                 do i=0,width-1
                   call lagrange(width,i,r,cl1(i))
                 end do

                 do c=ca,cb
!                    #If "uf(i1,i2,i3,c)=" eq "uf(i1,i2,i3,c)="
                   uf(i1,i2,i3,c)=0.
!                    #If "1" == "3"
!                    #If "1" == "2" || "1" == "3"
                   do i=0,width-1
                     uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl1(i)*uc(j1+i,j2,
     & j3,c)
                   end do
!                    #If "1" == "2" || "1" == "3"
!                    #If "1" == "3"
                 end do
! endCheckForMask()
                 endif
               end do
!              #If "1" == "2" || "1" == "3"
!              #If "1" == "3"

           else if( mod(width,2).eq.1 )then
c    general case, odd width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width
           iw=(width-1)/2
!            #If "1" == "1"
! interp2dWidthOdd(1,1,2,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl1(i)*uc(j1+i,j2+j,j3,c),uf(i1,i2,i3,c)=)
!              #If "1" == "3"
!              #If "1" == "2" || "1" == "3"
! beginLoopOdd1WithMask(iw,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -iw-i1offset
               m1=(i1-(j1+iw)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1 +iw
                if( abs(r-iw).gt.0.5)then
                  write(*,*) ' ERROR r=',r
                end if
                do i=0,width-1
                  call lagrange(width,i,r,cl1(i))
                end do
                do c=ca,cb
!                   #If "uf(i1,i2,i3,c)=" eq "uf(i1,i2,i3,c)="
                    uf(i1,i2,i3,c)=0.
!                   #If "1" == "3"
!                   #If "1" == "2" || "1" == "3"
                  do i=0,width-1
                    uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl1(i)*uc(j1+i,j2+j,
     & j3,c)
                  end do
!                   #If "1" == "2" || "1" == "3"
!                   #If "1" == "3"
                end do
! endCheckForMask()
                endif
              end do
!              #If "1" == "2" || "1" == "3"
!              #If "1" == "3"

           else
            write(*,*) 'interpFineFromCoarse:ERROR: interp width=',
     & width,' not implemeted'
           end if
         else
           write(*,*) 'interpFineFromCoarse:ERROR:nd=',nd
           stop 1
         end if
       else
c   Here we set uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+interpolant(uc)
         if( nd.eq.2 )then
! INTERP_LOOPS(2,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+)

          if( width.eq.2 .and. centering.eq.vertex .and. 
     & ratioEqualsTwoOrFour )then

!           #If "2" == "1"
!           #Elif "2" == "2"
! interp2dWidth2RatioTwoOrFour(2,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c2(0,m2)*( c2(0,m1)*uc(j1,j2  ,j3,c)+c2(1,m1)*uc(j1+1,j2  ,j3,c) )+c2(1,m2)*( c2(0,m1)*uc(j1,j2+1,j3,c)+c2(1,m1)*uc(j1+1,j2+1,j3,c) )

            if( nd.eq.2 .and. ratio1.eq.2 .and. ratio2.eq.2 )then
!             #If "2" eq "2"
c .... special case for 2d, ratio 2
c  4 cases: 
c      uf(i1,i2,i3,c)=uc(j1,j2,j3,c) if mod(i1,2).eq.0 and mod(i2,2).eq.0 
c      uf(i1,i2,i3,c)=.5*(uc(j1,j2,j3,c)+uc(j1+1,j2,j3,c) 
c      uf(i1,i2,i3,c)=.5*(uc(j1,j2,j3,c)+uc(j1,j2+1,j3,c) 
c      uf(i1,i2,i3,c)=.25*(uc(j1,j2,j3,c)+uc(j1+1,j2,j3,c)+uc(j1,j2+1,j3,c)+uc(j1+1,j2+1,j3,c))
            if( .true. )then
            msa=nsa+mod(nsa+32,2)  ! msa is even (add 32 to be make nsa+32 positive)
            msb=nsb-mod(nsb+32,2)  ! msb is even
            mra=nra+mod(nra+32,2)  ! mra is even
            mrb=nrb-mod(nrb+32,2)  ! mrb is even
            do c=ca,cb
            do i2=msa,msb,2
             j2=i2/2
             do i1=mra,mrb,2
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
               j1=i1/2  ! could add 1 instead??
               uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ uc(j1,j2,j3,c)  ! i1,i2 even
! endCheckForMask()
              endif
             end do
            end do
            end do
            mra=nra+mod(nra+31,2)  ! mra is odd
            mrb=nrb-mod(nrb+31,2)  ! mrb is odd
            do c=ca,cb
            do i2=msa,msb,2
             j2=i2/2
             do i1=mra,mrb,2
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
               j1=i1/2
               uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ .5*(uc(j1,j2,j3,c)+uc(j1+
     & 1,j2,j3,c))  ! i1 odd, i2 even
! endCheckForMask()
              endif
             end do
            end do
            end do
            msa=nsa+mod(nsa+31,2)
            msb=nsb-mod(nsb+31,2)
            mra=nra+mod(nra+32,2)
            mrb=nrb-mod(nrb+32,2)
            do c=ca,cb
            do i2=msa,msb,2
             j2=i2/2
             do i1=mra,mrb,2
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
               j1=i1/2
               uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ .5*(uc(j1,j2,j3,c)+uc(j1,
     & j2+1,j3,c))   ! i1 even i2 odd
! endCheckForMask()
              endif
             end do
            end do
            end do
            mra=nra+mod(nra+31,2)
            mrb=nrb-mod(nrb+31,2)
            do c=ca,cb
            do i2=msa,msb,2
             j2=i2/2
             do i1=mra,mrb,2
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
               j1=i1/2
               uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ .25*(uc(j1,j2,j3,c)+uc(
     & j1+1,j2,j3,c)+uc(j1,j2+1,j3,c)+uc(j1+1,j2+1,j3,c))
! endCheckForMask()
              endif
             end do
            end do
            end do

            else
            do c=ca,cb
            do i2=nsa,nsb
             j2=(i2+20)/2-10
             m2=(i2-j2*2)*ir2
             do i1=nra,nrb
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+20)/2-10
               m1=(i1-j1*2)*ir1 ! 0 <= m1 <r
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c2(0,m2)*(c2(0,m1)*uc(j1,j2,j3,c)+c2(1,m1)*uc(j1+1,j2,j3,c))+c2(1,m2)*(c2(0,m1)*uc(j1,j2+1,j3,c)+c2(1,m1)*uc(j1+1,j2+1,j3,c)))
               ! do c=ca,cb
                 uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c2(0,m2)*(c2(0,m1)*uc(
     & j1,j2,j3,c)+c2(1,m1)*uc(j1+1,j2,j3,c))+c2(1,m2)*(c2(0,m1)*uc(
     & j1,j2+1,j3,c)+c2(1,m1)*uc(j1+1,j2+1,j3,c))
               ! end do
! endCheckForMask()
              endif
             end do
            end do
            end do ! do c
            end if
            else
            do c=ca,cb
!             #If "2" != "1"
            do i2=nsa,nsb
             j2=(i2+ratio2*i2offset)/ratio2-i2offset
             m2=(i2-j2*ratio2)*ir2
             do i1=nra,nrb
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1*i1offset)/ratio1-i1offset
               m1=(i1-j1*ratio1)*ir1 ! 0 <= m1 <r
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c2(0,m2)*(c2(0,m1)*uc(j1,j2,j3,c)+c2(1,m1)*uc(j1+1,j2,j3,c))+c2(1,m2)*(c2(0,m1)*uc(j1,j2+1,j3,c)+c2(1,m1)*uc(j1+1,j2+1,j3,c)))
               ! do c=ca,cb
                 uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c2(0,m2)*(c2(0,m1)*uc(
     & j1,j2,j3,c)+c2(1,m1)*uc(j1+1,j2,j3,c))+c2(1,m2)*(c2(0,m1)*uc(
     & j1,j2+1,j3,c)+c2(1,m1)*uc(j1+1,j2+1,j3,c))
               ! end do
! endCheckForMask()
              endif
             end do
!             #If "2" != "1"
            end do
            end do ! do c
            end if


          else if( width.eq.3 .and. centering.eq.vertex .and. 
     & ratioEqualsTwoOrFour )then
            ! this verion is about 50% faster than the more general case below using lagrange30 etc.
!           #If "2" == "1"
!           #Elif "2" == "2"
            if( nrb-nra.ge.nsb-nsa )then
! interp2dWidth3RatioTwoOrFour(2,12,1,1,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3,c)+c3(1,m1)*uc(j1+1,j2  ,j3,c)+c3(2,m1)*uc(j1+2,j2  ,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))
             do c=ca,cb
!              #If "2" == 1
!              #Else
!              #If "12" == "12"
! beginLoopOdd2(1,*ir2)
             do i2=nsa,nsb
              j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -1-i2offset
              m2=(i2-(j2+1)*ratio2) *ir2
! beginLoopOdd1WithMask(1,*ir1)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
               m1=(i1-(j1+1)*ratio1) *ir1
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c3(0,m2)*(c3(0,m1)*uc(
     & j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c)
     & )+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,
     & c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,
     & j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c))
                ! end do
! endCheckForMask()
               endif
              end do
             end do
             end do ! do c

            else
! interp2dWidth3RatioTwoOrFour(2,21,1,1,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3,c)+c3(1,m1)*uc(j1+1,j2  ,j3,c)+c3(2,m1)*uc(j1+2,j2  ,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))
             do c=ca,cb
!              #If "2" == 1
!              #Else
!              #If "21" == "12"
!              #Else
! beginLoopOdd1(1,*ir1)
             do i1=nra,nrb
              j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
              m1=(i1-(j1+1)*ratio1) *ir1
! beginLoopOdd2WithMask(1,*ir2)
              do i2=nsa,nsb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -1-i2offset
               m2=(i2-(j2+1)*ratio2) *ir2
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c3(0,m2)*(c3(0,m1)*uc(
     & j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c)
     & )+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,
     & c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,
     & j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c))
                ! end do
! endCheckForMask()
               endif
              end do
             end do
             end do ! do c
            end if

          else if( width.eq.5 )then
!           #If "2" == "1"
!           #Elif "2" == "2"
            if( nrb-nra.ge.nsb-nsa )then
! interp2dWidth5RatioTwoOrFour(2,1,2,2,2,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c520*interp51(j1,j2  ,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))
              do c=ca,cb
!               #If "2" != "1"
! beginLoopOdd2(2,)
              do i2=nsa,nsb
               j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -2-i2offset
               m2=(i2-(j2+2)*ratio2)
               r=(m2 +centering*.5*(1-ratio2))/ratio2
               c52 0=lagrange50(r)
               c52 1=lagrange51(r)
               c52 2=lagrange52(r)
               c52 3=lagrange53(r)
               c52 4=lagrange54(r)
!               #If "1" == "1"
! beginLoopOdd1WithMask(2,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -2-i1offset
                m1=(i1-(j1+2)*ratio1)
                 r=(m1 +centering*.5*(1-ratio1 ))/ratio1
                 c51 0=lagrange50(r)
                 c51 1=lagrange51(r)
                 c51 2=lagrange52(r)
                 c51 3=lagrange53(r)
                 c51 4=lagrange54(r)

! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c520*interp51(j1,j2,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c520*interp51(j1,j2,
     & j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*
     & interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3)
                 ! end do
! endCheckForMask()
                endif
               end do
!               #If "2" != "1"
              end do
              end do ! do c
            else
! interp2dWidth5RatioTwoOrFour(2,2,1,2,2,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c520*interp51(j1,j2  ,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))
              do c=ca,cb
!               #If "2" != "1"
! beginLoopOdd1(2,)
              do i1=nra,nrb
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -2-i1offset
               m1=(i1-(j1+2)*ratio1)
               r=(m1 +centering*.5*(1-ratio1))/ratio1
               c51 0=lagrange50(r)
               c51 1=lagrange51(r)
               c51 2=lagrange52(r)
               c51 3=lagrange53(r)
               c51 4=lagrange54(r)
!               #If "2" == "1"
!               #Else
! beginLoopOdd2WithMask(2,)
               do i2=nsa,nsb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -2-i2offset
                m2=(i2-(j2+2)*ratio2)
                 r=(m2 +centering*.5*(1-ratio2 ))/ratio2
                 c52 0=lagrange50(r)
                 c52 1=lagrange51(r)
                 c52 2=lagrange52(r)
                 c52 3=lagrange53(r)
                 c52 4=lagrange54(r)

! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c520*interp51(j1,j2,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c520*interp51(j1,j2,
     & j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*
     & interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3)
                 ! end do
! endCheckForMask()
                endif
               end do
!               #If "2" != "1"
              end do
              end do ! do c
            end if

          else if( width.eq.2 )then
c   width=2 but ratio is not 2 or 4 or centering.eq.cell
!           #If "2" == "1"
!           #Elif "2" == "2"
! interp2dWidth2(2,0,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c220*interp21(j1,j2  ,j3)+c221*interp21(j1,j2+1,j3))
            do c=ca,cb
!             #If "2" != "1"
! beginLoopEven2(0,0,)
            do i2=nsa,nsb
              j2=(i2+ratio2*i2offset-centering*ratio2/2)/ratio2-
     & i2offset-0
              m2=(i2-(j2+0)*ratio2)
              r=(m2+centering*.5*(1-ratio2))/ratio2 +0
              c220=lagrange20(r)
              c221=lagrange21(r)
! beginLoopEven1WithMask(0,0,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-0
                m1=(i1-(j1+0)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1 +0
                c210=lagrange20(r)
                c211=lagrange21(r)
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c220*interp21(j1,j2,j3)+c221*interp21(j1,j2+1,j3))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c220*interp21(j1,j2,j3)
     & +c221*interp21(j1,j2+1,j3)
                ! end do

! endCheckForMask()
              endif
              end do
!             #If "2" != "1"
            end do
            end do ! do c
          else if( width.eq.3 )then
c   width=3 but ratio is not 2 or 4
!           #If "2" == "1"
!           #Elif "2" == "2"
! interp2dWidth3(2,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c320*interp31(j1,j2,j3)+c321*interp31(j1,j2+1,j3)+c322*interp31(j1,j2+2,j3))
            do c=ca,cb
!             #If "2" != "1"
! beginLoopOdd2(1,)
            do i2=nsa,nsb
             j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -1-i2offset
             m2=(i2-(j2+1)*ratio2)
             r=(m2+centering*.5*(1-ratio2))/ratio2
             c320=lagrange30(r)
             c321=lagrange31(r)
             c322=lagrange32(r)
! beginLoopOdd1WithMask(1,)
             do i1=nra,nrb
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
              j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
              m1=(i1-(j1+1)*ratio1)
               r=(m1+centering*.5*(1-ratio1))/ratio1
               c310=lagrange30(r)
               c311=lagrange31(r)
               c312=lagrange32(r)

! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c320*interp31(j1,j2,j3)+c321*interp31(j1,j2+1,j3)+c322*interp31(j1,j2+2,j3))
               ! do c=ca,cb
                 uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c320*interp31(j1,j2,j3)+
     & c321*interp31(j1,j2+1,j3)+c322*interp31(j1,j2+2,j3)
               ! end do

! endCheckForMask()
              endif
             end do
!             #If "2" != "1"
            end do
            end do ! do c
          else if( width.eq.1 )then
c   width=1 but ratio is not 2 or 4
          do c=ca,cb
!           #If "2" == "1"
!           #Elif "2" == "2"
          do i2=nsa,nsb
           j2=.5+ (i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -i2offset
c write(*,*) ' width=1: i2,j2,ratio2=',i2,j2,ratio2
          do i1=nra,nrb
           j1=.5+ (i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -i1offset
! beginCheckForMask()
             if( mask(i1,i2,i3).gt.0 )then
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ uc(j1,j2,j3,c))
             ! do c=ca,cb
               uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+uc(j1,j2,j3,c)
             ! end do
! endCheckForMask()
             endif
            end do
            end do
          end do ! do c
          else if( width.eq.4 )then

!           #If "2" == "1"
!           #Elif "2" == "2"
! interp2dWidth4(2,1,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c420*interp41(j1,j2  ,j3)+c421*interp41(j1,j2+1,j3)+c422*interp41(j1,j2+2,j3)+c423*interp41(j1,j2+3,j3))
            do c=ca,cb
!             #If "2" != "1"
! beginLoopEven2(1,0,)
            do i2=nsa,nsb
              j2=(i2+ratio2*i2offset-centering*ratio2/2)/ratio2-
     & i2offset-1
              m2=(i2-(j2+1)*ratio2)
              r=(m2+centering*.5*(1-ratio2))/ratio2 +0
c      write(*,*)' i2,j2,m2,r=',i2,j2,m2,r
              c420=lagrange40(r)
              c421=lagrange41(r)
              c422=lagrange42(r)
              c423=lagrange43(r)
! beginLoopEven1WithMask(1,0,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-1
                m1=(i1-(j1+1)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1 +0
                c410=lagrange40(r)
                c411=lagrange41(r)
                c412=lagrange42(r)
                c413=lagrange43(r)
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c420*interp41(j1,j2,j3)+c421*interp41(j1,j2+1,j3)+c422*interp41(j1,j2+2,j3)+c423*interp41(j1,j2+3,j3))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c420*interp41(j1,j2,j3)
     & +c421*interp41(j1,j2+1,j3)+c422*interp41(j1,j2+2,j3)+c423*
     & interp41(j1,j2+3,j3)
                ! end do
! endCheckForMask()
              endif
              end do
!             #If "2" != "1"
            end do
            end do ! do c

          else if( mod(width,2).eq.0 )then
c    general case, even width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width

            iw=(width-1)/2
!           #If "2" == "1"
!           #Elif "2" == "2"
! interp2dWidthEven(2,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl2(j)*cl1(i)*uc(j1+i,j2+j,j3,c),uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+)
!             #If "2" == "3"
!             #If "2" == "2" || "2" == "3"
! beginLoopEven2(iw,iw,)
            do i2=nsa,nsb
              j2=(i2+ratio2*i2offset-centering*ratio2/2)/ratio2-
     & i2offset-iw
              m2=(i2-(j2+iw)*ratio2)
              r=(m2+centering*.5*(1-ratio2))/ratio2 +iw
              if( abs(r-iw-.5).gt.0.5)then
                write(*,*) ' ERROR r=',r
              end if
              do i=0,width-1
                call lagrange(width,i,r,cl2(i))
              end do
! beginLoopEven1WithMask(iw,iw,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-iw
                m1=(i1-(j1+iw)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1 +iw
                if( abs(r-iw-.5).gt.0.5)then
                  write(*,*) ' ERROR r=',r
                end if
                do i=0,width-1
                  call lagrange(width,i,r,cl1(i))
                end do

                do c=ca,cb
!                   #If "uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+" eq "uf(i1,i2,i3,c)="
!                   #If "2" == "3"
!                   #If "2" == "2" || "2" == "3"
                  do j=0,width-1
                  do i=0,width-1
                    uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl2(j)*cl1(i)*uc(j1+
     & i,j2+j,j3,c)
                  end do
!                   #If "2" == "2" || "2" == "3"
                  end do
!                   #If "2" == "3"
                end do
! endCheckForMask()
                endif
              end do
!             #If "2" == "2" || "2" == "3"
            end do
!             #If "2" == "3"

          else if( mod(width,2).eq.1 )then
c    general case, odd width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width
          iw=(width-1)/2
!           #If "2" == "1"
!           #Elif "2" == "2"
! interp2dWidthOdd(2,1,2,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl2(j)*cl1(i)*uc(j1+i,j2+j,j3,c),uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+)
!             #If "2" == "3"
!             #If "2" == "2" || "2" == "3"
! beginLoopOdd2(iw,)
            do i2=nsa,nsb
             j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -iw-i2offset
             m2=(i2-(j2+iw)*ratio2)
             r=(m2+centering*.5*(1-ratio2))/ratio2 +iw
c write(*,*)' i2,j2,m2,r=',i2,j2,m2,r
             if( abs(r-iw).gt.0.5)then
               write(*,*) ' ERROR r=',r
             end if
             do i=0,width-1
               call lagrange(width,i,r,cl2(i))
             end do
! beginLoopOdd1WithMask(iw,)
             do i1=nra,nrb
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
              j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -iw-i1offset
              m1=(i1-(j1+iw)*ratio1)
               r=(m1+centering*.5*(1-ratio1))/ratio1 +iw
               if( abs(r-iw).gt.0.5)then
                 write(*,*) ' ERROR r=',r
               end if
               do i=0,width-1
                 call lagrange(width,i,r,cl1(i))
               end do
               do c=ca,cb
!                  #If "uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+" eq "uf(i1,i2,i3,c)="
!                  #If "2" == "3"
!                  #If "2" == "2" || "2" == "3"
                 do j=0,width-1
                 do i=0,width-1
                   uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl2(j)*cl1(i)*uc(j1+i,
     & j2+j,j3,c)
                 end do
!                  #If "2" == "2" || "2" == "3"
                 end do
!                  #If "2" == "3"
               end do
! endCheckForMask()
               endif
             end do
!             #If "2" == "2" || "2" == "3"
            end do
!             #If "2" == "3"

          else
           write(*,*) 'interpFineFromCoarse:ERROR: interp width=',
     & width,' not implemeted'
          end if
         else if( nd.eq.3 )then
! INTERP_LOOPS(3,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+)

           if( width.eq.2 .and. centering.eq.vertex .and. 
     & ratioEqualsTwoOrFour )then

!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Elif "3" == "3"
! beginLoopEven3(0,0,*ir3)
            do i3=nta,ntb
              j3=(i3+ratio3*i3offset-centering*ratio3/2)/ratio3-
     & i3offset-0
              m3=(i3-(j3+0)*ratio3) *ir3
              r=(m3+centering*.5*(1-ratio3))/ratio3 +0
! interp2dWidth2RatioTwoOrFour(3,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c2(0,m3)*(c2(0,m2)*( c2(0,m1)*uc(j1,j2  ,j3  ,c)+c2(1,m1)*uc(j1+1,j2  ,j3  ,c) )+c2(1,m2)*( c2(0,m1)*uc(j1,j2+1,j3  ,c)+c2(1,m1)*uc(j1+1,j2+1,j3  ,c) ))+c2(1,m3)*(c2(0,m2)*( c2(0,m1)*uc(j1,j2  ,j3+1,c)+c2(1,m1)*uc(j1+1,j2  ,j3+1,c) ) +c2(1,m2)*( c2(0,m1)*uc(j1,j2+1,j3+1,c)+c2(1,m1)*uc(j1+1,j2+1,j3+1,c) )) )

             if( nd.eq.2 .and. ratio1.eq.2 .and. ratio2.eq.2 )then
!              #If "3" eq "2"
             else
             do c=ca,cb
!              #If "3" != "1"
             do i2=nsa,nsb
              j2=(i2+ratio2*i2offset)/ratio2-i2offset
              m2=(i2-j2*ratio2)*ir2
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1*i1offset)/ratio1-i1offset
                m1=(i1-j1*ratio1)*ir1 ! 0 <= m1 <r
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c2(0,m3)*(c2(0,m2)*(c2(0,m1)*uc(j1,j2,j3,c)+c2(1,m1)*uc(j1+1,j2,j3,c))+c2(1,m2)*(c2(0,m1)*uc(j1,j2+1,j3,c)+c2(1,m1)*uc(j1+1,j2+1,j3,c)))+c2(1,m3)*(c2(0,m2)*(c2(0,m1)*uc(j1,j2,j3+1,c)+c2(1,m1)*uc(j1+1,j2,j3+1,c))+c2(1,m2)*(c2(0,m1)*uc(j1,j2+1,j3+1,c)+c2(1,m1)*uc(j1+1,j2+1,j3+1,c))))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c2(0,m3)*(c2(0,m2)*(c2(
     & 0,m1)*uc(j1,j2,j3,c)+c2(1,m1)*uc(j1+1,j2,j3,c))+c2(1,m2)*(c2(0,
     & m1)*uc(j1,j2+1,j3,c)+c2(1,m1)*uc(j1+1,j2+1,j3,c)))+c2(1,m3)*(
     & c2(0,m2)*(c2(0,m1)*uc(j1,j2,j3+1,c)+c2(1,m1)*uc(j1+1,j2,j3+1,c)
     & )+c2(1,m2)*(c2(0,m1)*uc(j1,j2+1,j3+1,c)+c2(1,m1)*uc(j1+1,j2+1,
     & j3+1,c)))
                ! end do
! endCheckForMask()
               endif
              end do
!              #If "3" != "1"
             end do
             end do ! do c
             end if
            end do


           else if( width.eq.3 .and. centering.eq.vertex .and. 
     & ratioEqualsTwoOrFour )then
             ! this verion is about 50% faster than the more general case below using lagrange30 etc.
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
             if( nrb-nra.ge.nsb-nsa )then
! beginLoopOdd3(1,*ir3)
              do i3=nta,ntb
               j3=(i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -1-i3offset
               m3=(i3-(j3+1)*ratio3) *ir3
! interp2dWidth3RatioTwoOrFour(3,12,1,1,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c3(0,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3  ,c)+c3(1,m1)*uc(j1+1,j2  ,j3  ,c)+c3(2,m1)*uc(j1+2,j2  ,j3  ,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3  ,c)+c3(1,m1)*uc(j1+1,j2+1,j3  ,c)+c3(2,m1)*uc(j1+2,j2+1,j3  ,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3  ,c)+c3(1,m1)*uc(j1+1,j2+2,j3  ,c)+c3(2,m1)*uc(j1+2,j2+2,j3  ,c)))+ c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3+1,c)+c3(1,m1)*uc(j1+1,j2  ,j3+1,c)+c3(2,m1)*uc(j1+2,j2  ,j3+1,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(2,m1)*uc(j1+2,j2+1,j3+1,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,c)+c3(1,m1)*uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c)))+ c3(2,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3+2,c)+c3(1,m1)*uc(j1+1,j2  ,j3+2,c)+c3(2,m1)*uc(j1+2,j2  ,j3+2,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(j1+2,j2+1,j3+2,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,m1)*uc(j1+1,j2+2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c))) )
              do c=ca,cb
!               #If "3" == 1
!               #Else
!               #If "12" == "12"
! beginLoopOdd2(1,*ir2)
              do i2=nsa,nsb
               j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -1-i2offset
               m2=(i2-(j2+1)*ratio2) *ir2
! beginLoopOdd1WithMask(1,*ir1)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
                m1=(i1-(j1+1)*ratio1) *ir1
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c3(0,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))+c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+1,c)+c3(1,m1)*uc(j1+1,j2,j3+1,c)+c3(2,m1)*uc(j1+2,j2,j3+1,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(2,m1)*uc(j1+2,j2+1,j3+1,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,c)+c3(1,m1)*uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c)))+c3(2,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+2,c)+c3(1,m1)*uc(j1+1,j2,j3+2,c)+c3(2,m1)*uc(j1+2,j2,j3+2,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(j1+2,j2+1,j3+2,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,m1)*uc(j1+1,j2+2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c))))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c3(0,m3)*(c3(0,m2)*(
     & c3(0,m1)*uc(j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(
     & j1+2,j2,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(
     & j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)
     & *uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,
     & j2+2,j3,c)))+c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+1,c)+c3(
     & 1,m1)*uc(j1+1,j2,j3+1,c)+c3(2,m1)*uc(j1+2,j2,j3+1,c))+c3(1,m2)*
     & (c3(0,m1)*uc(j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(
     & 2,m1)*uc(j1+2,j2+1,j3+1,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,
     & c)+c3(1,m1)*uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c))
     & )+c3(2,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+2,c)+c3(1,m1)*uc(j1+
     & 1,j2,j3+2,c)+c3(2,m1)*uc(j1+2,j2,j3+2,c))+c3(1,m2)*(c3(0,m1)*
     & uc(j1,j2+1,j3+2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(
     & j1+2,j2+1,j3+2,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,
     & m1)*uc(j1+1,j2+2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c)))
                 ! end do
! endCheckForMask()
                endif
               end do
              end do
              end do ! do c
              end do
             else
! beginLoopOdd3(1,*ir3)
              do i3=nta,ntb
               j3=(i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -1-i3offset
               m3=(i3-(j3+1)*ratio3) *ir3
! interp2dWidth3RatioTwoOrFour(3,21,1,1,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c3(0,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3  ,c)+c3(1,m1)*uc(j1+1,j2  ,j3  ,c)+c3(2,m1)*uc(j1+2,j2  ,j3  ,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3  ,c)+c3(1,m1)*uc(j1+1,j2+1,j3  ,c)+c3(2,m1)*uc(j1+2,j2+1,j3  ,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3  ,c)+c3(1,m1)*uc(j1+1,j2+2,j3  ,c)+c3(2,m1)*uc(j1+2,j2+2,j3  ,c)))+ c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3+1,c)+c3(1,m1)*uc(j1+1,j2  ,j3+1,c)+c3(2,m1)*uc(j1+2,j2  ,j3+1,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(2,m1)*uc(j1+2,j2+1,j3+1,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,c)+c3(1,m1)*uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c)))+ c3(2,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3+2,c)+c3(1,m1)*uc(j1+1,j2  ,j3+2,c)+c3(2,m1)*uc(j1+2,j2  ,j3+2,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(j1+2,j2+1,j3+2,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,m1)*uc(j1+1,j2+2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c))) )
              do c=ca,cb
!               #If "3" == 1
!               #Else
!               #If "21" == "12"
!               #Else
! beginLoopOdd1(1,*ir1)
              do i1=nra,nrb
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
               m1=(i1-(j1+1)*ratio1) *ir1
! beginLoopOdd2WithMask(1,*ir2)
               do i2=nsa,nsb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -1-i2offset
                m2=(i2-(j2+1)*ratio2) *ir2
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c3(0,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))+c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+1,c)+c3(1,m1)*uc(j1+1,j2,j3+1,c)+c3(2,m1)*uc(j1+2,j2,j3+1,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(2,m1)*uc(j1+2,j2+1,j3+1,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,c)+c3(1,m1)*uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c)))+c3(2,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+2,c)+c3(1,m1)*uc(j1+1,j2,j3+2,c)+c3(2,m1)*uc(j1+2,j2,j3+2,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(j1+2,j2+1,j3+2,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,m1)*uc(j1+1,j2+2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c))))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c3(0,m3)*(c3(0,m2)*(
     & c3(0,m1)*uc(j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(
     & j1+2,j2,j3,c))+c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(
     & j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))+c3(2,m2)*(c3(0,m1)
     & *uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,
     & j2+2,j3,c)))+c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+1,c)+c3(
     & 1,m1)*uc(j1+1,j2,j3+1,c)+c3(2,m1)*uc(j1+2,j2,j3+1,c))+c3(1,m2)*
     & (c3(0,m1)*uc(j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(
     & 2,m1)*uc(j1+2,j2+1,j3+1,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,
     & c)+c3(1,m1)*uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c))
     & )+c3(2,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2,j3+2,c)+c3(1,m1)*uc(j1+
     & 1,j2,j3+2,c)+c3(2,m1)*uc(j1+2,j2,j3+2,c))+c3(1,m2)*(c3(0,m1)*
     & uc(j1,j2+1,j3+2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(
     & j1+2,j2+1,j3+2,c))+c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,
     & m1)*uc(j1+1,j2+2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c)))
                 ! end do
! endCheckForMask()
                endif
               end do
              end do
              end do ! do c
              end do
             end if


           else if( width.eq.5 )then
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
             if( nrb-nra.ge.nsb-nsa )then
! beginLoopOdd3(2,)
               do i3=nta,ntb
                j3=(i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -2-i3offset
                m3=(i3-(j3+2)*ratio3)
                r=(m3 +centering*.5*(1-ratio3))/ratio3
                c530=lagrange50(r)
                c531=lagrange51(r)
                c532=lagrange52(r)
                c533=lagrange53(r)
                c534=lagrange54(r)

! interp2dWidth5RatioTwoOrFour(3,1,2,2,2,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c530*(c520*interp51(j1,j2  ,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+c531*(c520*interp51(j1,j2  ,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*interp51(j1,j2+2,j3+1)+c523*interp51(j1,j2+3,j3+1)+c524*interp51(j1,j2+4,j3+1))+c532*(c520*interp51(j1,j2  ,j3+2)+c521*interp51(j1,j2+1,j3+2)+c522*interp51(j1,j2+2,j3+2)+c523*interp51(j1,j2+3,j3+2)+c524*interp51(j1,j2+4,j3+2))+c533*(c520*interp51(j1,j2  ,j3+3)+c521*interp51(j1,j2+1,j3+3)+c522*interp51(j1,j2+2,j3+3)+c523*interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+4,j3+3))+c534*(c520*interp51(j1,j2  ,j3+4)+c521*interp51(j1,j2+1,j3+4)+c522*interp51(j1,j2+2,j3+4)+c523*interp51(j1,j2+3,j3+4)+c524*interp51(j1,j2+4,j3+4)) )
               do c=ca,cb
!                #If "3" != "1"
! beginLoopOdd2(2,)
               do i2=nsa,nsb
                j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -2-i2offset
                m2=(i2-(j2+2)*ratio2)
                r=(m2 +centering*.5*(1-ratio2))/ratio2
                c52 0=lagrange50(r)
                c52 1=lagrange51(r)
                c52 2=lagrange52(r)
                c52 3=lagrange53(r)
                c52 4=lagrange54(r)
!                #If "1" == "1"
! beginLoopOdd1WithMask(2,)
                do i1=nra,nrb
! beginCheckForMask()
                 if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -2-i1offset
                 m1=(i1-(j1+2)*ratio1)
                  r=(m1 +centering*.5*(1-ratio1 ))/ratio1
                  c51 0=lagrange50(r)
                  c51 1=lagrange51(r)
                  c51 2=lagrange52(r)
                  c51 3=lagrange53(r)
                  c51 4=lagrange54(r)

! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c530*(c520*interp51(j1,j2,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+c531*(c520*interp51(j1,j2,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*interp51(j1,j2+2,j3+1)+c523*interp51(j1,j2+3,j3+1)+c524*interp51(j1,j2+4,j3+1))+c532*(c520*interp51(j1,j2,j3+2)+c521*interp51(j1,j2+1,j3+2)+c522*interp51(j1,j2+2,j3+2)+c523*interp51(j1,j2+3,j3+2)+c524*interp51(j1,j2+4,j3+2))+c533*(c520*interp51(j1,j2,j3+3)+c521*interp51(j1,j2+1,j3+3)+c522*interp51(j1,j2+2,j3+3)+c523*interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+4,j3+3))+c534*(c520*interp51(j1,j2,j3+4)+c521*interp51(j1,j2+1,j3+4)+c522*interp51(j1,j2+2,j3+4)+c523*interp51(j1,j2+3,j3+4)+c524*interp51(j1,j2+4,j3+4)))
                  ! do c=ca,cb
                    uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c530*(c520*interp51(
     & j1,j2,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+
     & c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+c531*(
     & c520*interp51(j1,j2,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*
     & interp51(j1,j2+2,j3+1)+c523*interp51(j1,j2+3,j3+1)+c524*
     & interp51(j1,j2+4,j3+1))+c532*(c520*interp51(j1,j2,j3+2)+c521*
     & interp51(j1,j2+1,j3+2)+c522*interp51(j1,j2+2,j3+2)+c523*
     & interp51(j1,j2+3,j3+2)+c524*interp51(j1,j2+4,j3+2))+c533*(c520*
     & interp51(j1,j2,j3+3)+c521*interp51(j1,j2+1,j3+3)+c522*interp51(
     & j1,j2+2,j3+3)+c523*interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+
     & 4,j3+3))+c534*(c520*interp51(j1,j2,j3+4)+c521*interp51(j1,j2+1,
     & j3+4)+c522*interp51(j1,j2+2,j3+4)+c523*interp51(j1,j2+3,j3+4)+
     & c524*interp51(j1,j2+4,j3+4))
                  ! end do
! endCheckForMask()
                 endif
                end do
!                #If "3" != "1"
               end do
               end do ! do c
               end do
             else
! beginLoopOdd3(2,)
               do i3=nta,ntb
                j3=(i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -2-i3offset
                m3=(i3-(j3+2)*ratio3)
                r=(m3 +centering*.5*(1-ratio3))/ratio3
                c530=lagrange50(r)
                c531=lagrange51(r)
                c532=lagrange52(r)
                c533=lagrange53(r)
                c534=lagrange54(r)

! interp2dWidth5RatioTwoOrFour(3,2,1,2,2,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c530*(c520*interp51(j1,j2  ,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+c531*(c520*interp51(j1,j2  ,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*interp51(j1,j2+2,j3+1)+c523*interp51(j1,j2+3,j3+1)+c524*interp51(j1,j2+4,j3+1))+c532*(c520*interp51(j1,j2  ,j3+2)+c521*interp51(j1,j2+1,j3+2)+c522*interp51(j1,j2+2,j3+2)+c523*interp51(j1,j2+3,j3+2)+c524*interp51(j1,j2+4,j3+2))+c533*(c520*interp51(j1,j2  ,j3+3)+c521*interp51(j1,j2+1,j3+3)+c522*interp51(j1,j2+2,j3+3)+c523*interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+4,j3+3))+c534*(c520*interp51(j1,j2  ,j3+4)+c521*interp51(j1,j2+1,j3+4)+c522*interp51(j1,j2+2,j3+4)+c523*interp51(j1,j2+3,j3+4)+c524*interp51(j1,j2+4,j3+4)) )
               do c=ca,cb
!                #If "3" != "1"
! beginLoopOdd1(2,)
               do i1=nra,nrb
                j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -2-i1offset
                m1=(i1-(j1+2)*ratio1)
                r=(m1 +centering*.5*(1-ratio1))/ratio1
                c51 0=lagrange50(r)
                c51 1=lagrange51(r)
                c51 2=lagrange52(r)
                c51 3=lagrange53(r)
                c51 4=lagrange54(r)
!                #If "2" == "1"
!                #Else
! beginLoopOdd2WithMask(2,)
                do i2=nsa,nsb
! beginCheckForMask()
                 if( mask(i1,i2,i3).gt.0 )then
                 j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -2-i2offset
                 m2=(i2-(j2+2)*ratio2)
                  r=(m2 +centering*.5*(1-ratio2 ))/ratio2
                  c52 0=lagrange50(r)
                  c52 1=lagrange51(r)
                  c52 2=lagrange52(r)
                  c52 3=lagrange53(r)
                  c52 4=lagrange54(r)

! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c530*(c520*interp51(j1,j2,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+c531*(c520*interp51(j1,j2,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*interp51(j1,j2+2,j3+1)+c523*interp51(j1,j2+3,j3+1)+c524*interp51(j1,j2+4,j3+1))+c532*(c520*interp51(j1,j2,j3+2)+c521*interp51(j1,j2+1,j3+2)+c522*interp51(j1,j2+2,j3+2)+c523*interp51(j1,j2+3,j3+2)+c524*interp51(j1,j2+4,j3+2))+c533*(c520*interp51(j1,j2,j3+3)+c521*interp51(j1,j2+1,j3+3)+c522*interp51(j1,j2+2,j3+3)+c523*interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+4,j3+3))+c534*(c520*interp51(j1,j2,j3+4)+c521*interp51(j1,j2+1,j3+4)+c522*interp51(j1,j2+2,j3+4)+c523*interp51(j1,j2+3,j3+4)+c524*interp51(j1,j2+4,j3+4)))
                  ! do c=ca,cb
                    uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c530*(c520*interp51(
     & j1,j2,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+
     & c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+c531*(
     & c520*interp51(j1,j2,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*
     & interp51(j1,j2+2,j3+1)+c523*interp51(j1,j2+3,j3+1)+c524*
     & interp51(j1,j2+4,j3+1))+c532*(c520*interp51(j1,j2,j3+2)+c521*
     & interp51(j1,j2+1,j3+2)+c522*interp51(j1,j2+2,j3+2)+c523*
     & interp51(j1,j2+3,j3+2)+c524*interp51(j1,j2+4,j3+2))+c533*(c520*
     & interp51(j1,j2,j3+3)+c521*interp51(j1,j2+1,j3+3)+c522*interp51(
     & j1,j2+2,j3+3)+c523*interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+
     & 4,j3+3))+c534*(c520*interp51(j1,j2,j3+4)+c521*interp51(j1,j2+1,
     & j3+4)+c522*interp51(j1,j2+2,j3+4)+c523*interp51(j1,j2+3,j3+4)+
     & c524*interp51(j1,j2+4,j3+4))
                  ! end do
! endCheckForMask()
                 endif
                end do
!                #If "3" != "1"
               end do
               end do ! do c
               end do
             end if

           else if( width.eq.2 )then
c   width=2 but ratio is not 2 or 4 or centering.eq.cell
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Elif "3" == "3"
! beginLoopEven3(0,0,)
             do i3=nta,ntb
               j3=(i3+ratio3*i3offset-centering*ratio3/2)/ratio3-
     & i3offset-0
               m3=(i3-(j3+0)*ratio3)
               r=(m3+centering*.5*(1-ratio3))/ratio3 +0
             c230=lagrange20(r)
             c231=lagrange21(r)
! interp2dWidth2(3,0,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+  c230*(c220*interp21(j1,j2  ,j3  )+c221*interp21(j1,j2+1,j3))+ c231*(c220*interp21(j1,j2  ,j3+1)+c221*interp21(j1,j2+1,j3+1)) )
             do c=ca,cb
!              #If "3" != "1"
! beginLoopEven2(0,0,)
             do i2=nsa,nsb
               j2=(i2+ratio2*i2offset-centering*ratio2/2)/ratio2-
     & i2offset-0
               m2=(i2-(j2+0)*ratio2)
               r=(m2+centering*.5*(1-ratio2))/ratio2 +0
               c220=lagrange20(r)
               c221=lagrange21(r)
! beginLoopEven1WithMask(0,0,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-0
                 m1=(i1-(j1+0)*ratio1)
                 r=(m1+centering*.5*(1-ratio1))/ratio1 +0
                 c210=lagrange20(r)
                 c211=lagrange21(r)
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c230*(c220*interp21(j1,j2,j3)+c221*interp21(j1,j2+1,j3))+c231*(c220*interp21(j1,j2,j3+1)+c221*interp21(j1,j2+1,j3+1)))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c230*(c220*interp21(
     & j1,j2,j3)+c221*interp21(j1,j2+1,j3))+c231*(c220*interp21(j1,j2,
     & j3+1)+c221*interp21(j1,j2+1,j3+1))
                 ! end do

! endCheckForMask()
               endif
               end do
!              #If "3" != "1"
             end do
             end do ! do c
             end do
           else if( width.eq.3 )then
c   width=3 but ratio is not 2 or 4
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
! beginLoopOdd3(1,)
            do i3=nta,ntb
             j3=(i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -1-i3offset
             m3=(i3-(j3+1)*ratio3)
             r=(m3+centering*.5*(1-ratio3))/ratio3
             c330=lagrange30(r)
             c331=lagrange31(r)
             c332=lagrange32(r)
! interp2dWidth3(3,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c330*(c320*interp31(j1,j2,j3  )+c321*interp31(j1,j2+1,j3  )+c322*interp31(j1,j2+2,j3  ))+c331*(c320*interp31(j1,j2,j3+1)+c321*interp31(j1,j2+1,j3+1)+c322*interp31(j1,j2+2,j3+1))+c332*(c320*interp31(j1,j2,j3+2)+c321*interp31(j1,j2+1,j3+2)+c322*interp31(j1,j2+2,j3+2)) )
             do c=ca,cb
!              #If "3" != "1"
! beginLoopOdd2(1,)
             do i2=nsa,nsb
              j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -1-i2offset
              m2=(i2-(j2+1)*ratio2)
              r=(m2+centering*.5*(1-ratio2))/ratio2
              c320=lagrange30(r)
              c321=lagrange31(r)
              c322=lagrange32(r)
! beginLoopOdd1WithMask(1,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
               m1=(i1-(j1+1)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1
                c310=lagrange30(r)
                c311=lagrange31(r)
                c312=lagrange32(r)

! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c330*(c320*interp31(j1,j2,j3)+c321*interp31(j1,j2+1,j3)+c322*interp31(j1,j2+2,j3))+c331*(c320*interp31(j1,j2,j3+1)+c321*interp31(j1,j2+1,j3+1)+c322*interp31(j1,j2+2,j3+1))+c332*(c320*interp31(j1,j2,j3+2)+c321*interp31(j1,j2+1,j3+2)+c322*interp31(j1,j2+2,j3+2)))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c330*(c320*interp31(j1,
     & j2,j3)+c321*interp31(j1,j2+1,j3)+c322*interp31(j1,j2+2,j3))+
     & c331*(c320*interp31(j1,j2,j3+1)+c321*interp31(j1,j2+1,j3+1)+
     & c322*interp31(j1,j2+2,j3+1))+c332*(c320*interp31(j1,j2,j3+2)+
     & c321*interp31(j1,j2+1,j3+2)+c322*interp31(j1,j2+2,j3+2))
                ! end do

! endCheckForMask()
               endif
              end do
!              #If "3" != "1"
             end do
             end do ! do c
            end do
           else if( width.eq.1 )then
c   width=1 but ratio is not 2 or 4
           do c=ca,cb
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
           do i3=nta,ntb
            j3=.5+ (i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -i3offset
           do i2=nsa,nsb
            j2=.5+ (i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -i2offset
           do i1=nra,nrb
            j1=.5+ (i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -i1offset
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ uc(j1,j2,j3,c))
               ! do c=ca,cb
                 uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+uc(j1,j2,j3,c)
               ! end do
! endCheckForMask()
              endif
             end do
             end do
             end do
           end do ! do c
           else if( width.eq.4 )then

!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
! beginLoopEven3(1,0,)
           do i3=nta,ntb
             j3=(i3+ratio3*i3offset-centering*ratio3/2)/ratio3-
     & i3offset-1
             m3=(i3-(j3+1)*ratio3)
             r=(m3+centering*.5*(1-ratio3))/ratio3 +0
             c430=lagrange40(r)
             c431=lagrange41(r)
             c432=lagrange42(r)
             c433=lagrange43(r)
! interp2dWidth4(3,1, uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c430*(c420*interp41(j1,j2,j3  )+c421*interp41(j1,j2+1,j3  )+c422*interp41(j1,j2+2,j3  )+c423*interp41(j1,j2+3,j3  ))+c431*(c420*interp41(j1,j2,j3+1)+c421*interp41(j1,j2+1,j3+1)+c422*interp41(j1,j2+2,j3+1)+c423*interp41(j1,j2+3,j3+1))+c432*(c420*interp41(j1,j2,j3+2)+c421*interp41(j1,j2+1,j3+2)+c422*interp41(j1,j2+2,j3+2)+c423*interp41(j1,j2+3,j3+2))+c433*(c420*interp41(j1,j2,j3+3)+c421*interp41(j1,j2+1,j3+3)+c422*interp41(j1,j2+2,j3+3)+c423*interp41(j1,j2+3,j3+3)) )
             do c=ca,cb
!              #If "3" != "1"
! beginLoopEven2(1,0,)
             do i2=nsa,nsb
               j2=(i2+ratio2*i2offset-centering*ratio2/2)/ratio2-
     & i2offset-1
               m2=(i2-(j2+1)*ratio2)
               r=(m2+centering*.5*(1-ratio2))/ratio2 +0
c      write(*,*)' i2,j2,m2,r=',i2,j2,m2,r
               c420=lagrange40(r)
               c421=lagrange41(r)
               c422=lagrange42(r)
               c423=lagrange43(r)
! beginLoopEven1WithMask(1,0,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-1
                 m1=(i1-(j1+1)*ratio1)
                 r=(m1+centering*.5*(1-ratio1))/ratio1 +0
                 c410=lagrange40(r)
                 c411=lagrange41(r)
                 c412=lagrange42(r)
                 c413=lagrange43(r)
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c430*(c420*interp41(j1,j2,j3)+c421*interp41(j1,j2+1,j3)+c422*interp41(j1,j2+2,j3)+c423*interp41(j1,j2+3,j3))+c431*(c420*interp41(j1,j2,j3+1)+c421*interp41(j1,j2+1,j3+1)+c422*interp41(j1,j2+2,j3+1)+c423*interp41(j1,j2+3,j3+1))+c432*(c420*interp41(j1,j2,j3+2)+c421*interp41(j1,j2+1,j3+2)+c422*interp41(j1,j2+2,j3+2)+c423*interp41(j1,j2+3,j3+2))+c433*(c420*interp41(j1,j2,j3+3)+c421*interp41(j1,j2+1,j3+3)+c422*interp41(j1,j2+2,j3+3)+c423*interp41(j1,j2+3,j3+3)))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c430*(c420*interp41(
     & j1,j2,j3)+c421*interp41(j1,j2+1,j3)+c422*interp41(j1,j2+2,j3)+
     & c423*interp41(j1,j2+3,j3))+c431*(c420*interp41(j1,j2,j3+1)+
     & c421*interp41(j1,j2+1,j3+1)+c422*interp41(j1,j2+2,j3+1)+c423*
     & interp41(j1,j2+3,j3+1))+c432*(c420*interp41(j1,j2,j3+2)+c421*
     & interp41(j1,j2+1,j3+2)+c422*interp41(j1,j2+2,j3+2)+c423*
     & interp41(j1,j2+3,j3+2))+c433*(c420*interp41(j1,j2,j3+3)+c421*
     & interp41(j1,j2+1,j3+3)+c422*interp41(j1,j2+2,j3+3)+c423*
     & interp41(j1,j2+3,j3+3))
                 ! end do
! endCheckForMask()
               endif
               end do
!              #If "3" != "1"
             end do
             end do ! do c
             end do

           else if( mod(width,2).eq.0 )then
c    general case, even width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width

             iw=(width-1)/2
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
! interp2dWidthEven(3,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl3(k)*cl2(j)*cl1(i)*uc(j1+i,j2+j,j3+k,c),uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+)
!              #If "3" == "3"
! beginLoopEven3(iw,iw,)
             do i3=nta,ntb
               j3=(i3+ratio3*i3offset-centering*ratio3/2)/ratio3-
     & i3offset-iw
               m3=(i3-(j3+iw)*ratio3)
               r=(m3+centering*.5*(1-ratio3))/ratio3 +iw
               if( abs(r-iw-.5).gt.0.5)then
                 write(*,*) ' ERROR r=',r
               end if
               do i=0,width-1
                 call lagrange(width,i,r,cl3(i))
               end do
!              #If "3" == "2" || "3" == "3"
! beginLoopEven2(iw,iw,)
             do i2=nsa,nsb
               j2=(i2+ratio2*i2offset-centering*ratio2/2)/ratio2-
     & i2offset-iw
               m2=(i2-(j2+iw)*ratio2)
               r=(m2+centering*.5*(1-ratio2))/ratio2 +iw
               if( abs(r-iw-.5).gt.0.5)then
                 write(*,*) ' ERROR r=',r
               end if
               do i=0,width-1
                 call lagrange(width,i,r,cl2(i))
               end do
! beginLoopEven1WithMask(iw,iw,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-iw
                 m1=(i1-(j1+iw)*ratio1)
                 r=(m1+centering*.5*(1-ratio1))/ratio1 +iw
                 if( abs(r-iw-.5).gt.0.5)then
                   write(*,*) ' ERROR r=',r
                 end if
                 do i=0,width-1
                   call lagrange(width,i,r,cl1(i))
                 end do

                 do c=ca,cb
!                    #If "uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+" eq "uf(i1,i2,i3,c)="
!                    #If "3" == "3"
                   do k=0,width-1
!                    #If "3" == "2" || "3" == "3"
                   do j=0,width-1
                   do i=0,width-1
                     uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl3(k)*cl2(j)*cl1(i)
     & *uc(j1+i,j2+j,j3+k,c)
                   end do
!                    #If "3" == "2" || "3" == "3"
                   end do
!                    #If "3" == "3"
                   end do
                 end do
! endCheckForMask()
                 endif
               end do
!              #If "3" == "2" || "3" == "3"
             end do
!              #If "3" == "3"
             end do

           else if( mod(width,2).eq.1 )then
c    general case, odd width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width
           iw=(width-1)/2
!            #If "3" == "1"
!            #Elif "3" == "2"
!            #Else
! interp2dWidthOdd(3,1,2,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl3(k)*cl2(j)*cl1(i)*uc(j1+i,j2+j,j3+k,c),uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+)
!              #If "3" == "3"
! beginLoopOdd3(iw,)
             do i3=nta,ntb
              j3=(i3+ratio3/2+ratio3*i3offset-i3Shift-centering*
     & ratio3/2)/ratio3 -iw-i3offset
              m3=(i3-(j3+iw)*ratio3)
              r=(m3+centering*.5*(1-ratio3))/ratio3 +iw
              if( abs(r-iw).gt.0.5)then
                write(*,*) ' ERROR r=',r
              end if
              do i=0,width-1
                call lagrange(width,i,r,cl3(i))
              end do
!              #If "3" == "2" || "3" == "3"
! beginLoopOdd2(iw,)
             do i2=nsa,nsb
              j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*
     & ratio2/2)/ratio2 -iw-i2offset
              m2=(i2-(j2+iw)*ratio2)
              r=(m2+centering*.5*(1-ratio2))/ratio2 +iw
c write(*,*)' i2,j2,m2,r=',i2,j2,m2,r
              if( abs(r-iw).gt.0.5)then
                write(*,*) ' ERROR r=',r
              end if
              do i=0,width-1
                call lagrange(width,i,r,cl2(i))
              end do
! beginLoopOdd1WithMask(iw,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -iw-i1offset
               m1=(i1-(j1+iw)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1 +iw
                if( abs(r-iw).gt.0.5)then
                  write(*,*) ' ERROR r=',r
                end if
                do i=0,width-1
                  call lagrange(width,i,r,cl1(i))
                end do
                do c=ca,cb
!                   #If "uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+" eq "uf(i1,i2,i3,c)="
!                   #If "3" == "3"
                  do k=0,width-1
!                   #If "3" == "2" || "3" == "3"
                  do j=0,width-1
                  do i=0,width-1
                    uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl3(k)*cl2(j)*cl1(i)*
     & uc(j1+i,j2+j,j3+k,c)
                  end do
!                   #If "3" == "2" || "3" == "3"
                  end do
!                   #If "3" == "3"
                  end do
                end do
! endCheckForMask()
                endif
              end do
!              #If "3" == "2" || "3" == "3"
             end do
!              #If "3" == "3"
             end do

           else
            write(*,*) 'interpFineFromCoarse:ERROR: interp width=',
     & width,' not implemeted'
           end if
         else if( nd.eq.1 )then
! INTERP_LOOPS(1,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+)

           if( width.eq.2 .and. centering.eq.vertex .and. 
     & ratioEqualsTwoOrFour )then

!            #If "1" == "1"
! interp2dWidth2RatioTwoOrFour(1,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c2(0,m1)*uc(j1,j2  ,j3,c)+c2(1,m1)*uc(j1+1,j2  ,j3,c) )

             if( nd.eq.2 .and. ratio1.eq.2 .and. ratio2.eq.2 )then
!              #If "1" eq "2"
             else
             do c=ca,cb
!              #If "1" != "1"
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
                j1=(i1+ratio1*i1offset)/ratio1-i1offset
                m1=(i1-j1*ratio1)*ir1 ! 0 <= m1 <r
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c2(0,m1)*uc(j1,j2,j3,c)+c2(1,m1)*uc(j1+1,j2,j3,c))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c2(0,m1)*uc(j1,j2,j3,c)
     & +c2(1,m1)*uc(j1+1,j2,j3,c)
                ! end do
! endCheckForMask()
               endif
              end do
!              #If "1" != "1"
             end do ! do c
             end if


           else if( width.eq.3 .and. centering.eq.vertex .and. 
     & ratioEqualsTwoOrFour )then
             ! this verion is about 50% faster than the more general case below using lagrange30 etc.
!            #If "1" == "1"
! interp2dWidth3RatioTwoOrFour(1,12,1,0,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ c3(0,m1)*uc(j1,j2  ,j3,c)+c3(1,m1)*uc(j1+1,j2  ,j3,c)+c3(2,m1)*uc(j1+2,j2  ,j3,c))
              do c=ca,cb
!               #If "1" == 1
! beginLoopOdd1WithMask(1,*ir1)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
               m1=(i1-(j1+1)*ratio1) *ir1
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c3(0,m1)*uc(j1,j2,j3,c)+c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c))
              ! do c=ca,cb
                uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+c3(0,m1)*uc(j1,j2,j3,c)+
     & c3(1,m1)*uc(j1+1,j2,j3,c)+c3(2,m1)*uc(j1+2,j2,j3,c)
              ! end do
! endCheckForMask()
              endif
              end do
              end do ! do c

           else if( width.eq.5 )then
!            #If "1" == "1"
! interp2dWidth5RatioTwoOrFour(1,1,2,2,0,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ interp51(j1,j2,j3))
             do c=ca,cb
!              #If "1" != "1"
!              #If "1" == "1"
! beginLoopOdd1WithMask(2,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -2-i1offset
               m1=(i1-(j1+2)*ratio1)
                r=(m1 +centering*.5*(1-ratio1 ))/ratio1
                c51 0=lagrange50(r)
                c51 1=lagrange51(r)
                c51 2=lagrange52(r)
                c51 3=lagrange53(r)
                c51 4=lagrange54(r)

! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+interp51(j1,j2,j3))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+interp51(j1,j2,j3)
                ! end do
! endCheckForMask()
               endif
              end do
!              #If "1" != "1"
             end do ! do c

           else if( width.eq.2 )then
c   width=2 but ratio is not 2 or 4 or centering.eq.cell
!            #If "1" == "1"
! interp2dWidth2(1,0,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ interp21(j1,j2  ,j3))
             do c=ca,cb
!              #If "1" != "1"
! beginLoopEven1WithMask(0,0,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-0
                 m1=(i1-(j1+0)*ratio1)
                 r=(m1+centering*.5*(1-ratio1))/ratio1 +0
                 c210=lagrange20(r)
                 c211=lagrange21(r)
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+interp21(j1,j2,j3))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+interp21(j1,j2,j3)
                 ! end do

! endCheckForMask()
               endif
               end do
!              #If "1" != "1"
             end do ! do c
           else if( width.eq.3 )then
c   width=3 but ratio is not 2 or 4
!            #If "1" == "1"
! interp2dWidth3(1,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ interp31(j1,j2,j3))
             do c=ca,cb
!              #If "1" != "1"
! beginLoopOdd1WithMask(1,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -1-i1offset
               m1=(i1-(j1+1)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1
                c310=lagrange30(r)
                c311=lagrange31(r)
                c312=lagrange32(r)

! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+interp31(j1,j2,j3))
                ! do c=ca,cb
                  uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+interp31(j1,j2,j3)
                ! end do

! endCheckForMask()
               endif
              end do
!              #If "1" != "1"
             end do ! do c
           else if( width.eq.1 )then
c   width=1 but ratio is not 2 or 4
           do c=ca,cb
!            #If "1" == "1"
           do i1=nra,nrb
            j1=.5+ (i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -i1offset
! beginCheckForMask()
              if( mask(i1,i2,i3).gt.0 )then
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ uc(j1,j2,j3,c))
              ! do c=ca,cb
                uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+uc(j1,j2,j3,c)
              ! end do
! endCheckForMask()
             endif
             end do
           end do ! do c
           else if( width.eq.4 )then

!            #If "1" == "1"
! interp2dWidth4(1,1,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+ interp41(j1,j2,j3))
             do c=ca,cb
!              #If "1" != "1"
! beginLoopEven1WithMask(1,0,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-1
                 m1=(i1-(j1+1)*ratio1)
                 r=(m1+centering*.5*(1-ratio1))/ratio1 +0
                 c410=lagrange40(r)
                 c411=lagrange41(r)
                 c412=lagrange42(r)
                 c413=lagrange43(r)
! innerLoop(uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+interp41(j1,j2,j3))
                 ! do c=ca,cb
                   uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+interp41(j1,j2,j3)
                 ! end do
! endCheckForMask()
               endif
               end do
!              #If "1" != "1"
             end do ! do c

           else if( mod(width,2).eq.0 )then
c    general case, even width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width

             iw=(width-1)/2
!            #If "1" == "1"
! interp2dWidthEven(1,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl1(i)*uc(j1+i,j2,j3,c),uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+)
!              #If "1" == "3"
!              #If "1" == "2" || "1" == "3"
! beginLoopEven1WithMask(iw,iw,)
               do i1=nra,nrb
! beginCheckForMask()
                if( mask(i1,i2,i3).gt.0 )then
                 j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-
     & i1offset-iw
                 m1=(i1-(j1+iw)*ratio1)
                 r=(m1+centering*.5*(1-ratio1))/ratio1 +iw
                 if( abs(r-iw-.5).gt.0.5)then
                   write(*,*) ' ERROR r=',r
                 end if
                 do i=0,width-1
                   call lagrange(width,i,r,cl1(i))
                 end do

                 do c=ca,cb
!                    #If "uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+" eq "uf(i1,i2,i3,c)="
!                    #If "1" == "3"
!                    #If "1" == "2" || "1" == "3"
                   do i=0,width-1
                     uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl1(i)*uc(j1+i,j2,
     & j3,c)
                   end do
!                    #If "1" == "2" || "1" == "3"
!                    #If "1" == "3"
                 end do
! endCheckForMask()
                 endif
               end do
!              #If "1" == "2" || "1" == "3"
!              #If "1" == "3"

           else if( mod(width,2).eq.1 )then
c    general case, odd width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width
           iw=(width-1)/2
!            #If "1" == "1"
! interp2dWidthOdd(1,1,2,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl1(i)*uc(j1+i,j2+j,j3,c),uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+)
!              #If "1" == "3"
!              #If "1" == "2" || "1" == "3"
! beginLoopOdd1WithMask(iw,)
              do i1=nra,nrb
! beginCheckForMask()
               if( mask(i1,i2,i3).gt.0 )then
               j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*
     & ratio1/2)/ratio1 -iw-i1offset
               m1=(i1-(j1+iw)*ratio1)
                r=(m1+centering*.5*(1-ratio1))/ratio1 +iw
                if( abs(r-iw).gt.0.5)then
                  write(*,*) ' ERROR r=',r
                end if
                do i=0,width-1
                  call lagrange(width,i,r,cl1(i))
                end do
                do c=ca,cb
!                   #If "uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+" eq "uf(i1,i2,i3,c)="
!                   #If "1" == "3"
!                   #If "1" == "2" || "1" == "3"
                  do i=0,width-1
                    uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl1(i)*uc(j1+i,j2+j,
     & j3,c)
                  end do
!                   #If "1" == "2" || "1" == "3"
!                   #If "1" == "3"
                end do
! endCheckForMask()
                endif
              end do
!              #If "1" == "2" || "1" == "3"
!              #If "1" == "3"

           else
            write(*,*) 'interpFineFromCoarse:ERROR: interp width=',
     & width,' not implemeted'
           end if
         else
           write(*,*) 'interpFineFromCoarse:ERROR:nd=',nd
           stop 1
         end if
       end if

      return
      end
