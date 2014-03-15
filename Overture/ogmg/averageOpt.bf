       subroutine averageOpt( nd, 
     &   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ! for cFine
     &   md1a,md1b,md2a,md2b,md3a,md3b, ! for cCoarse
     &   j1a,j1b,j2a,j2b,j3a,j3b, i1a,i1b,i1c,i2a,i2b,i2c,i3a,i3b,i3c, 
     &   i1pa,i1pb,i2pa,i2pb,i3pa,i3pb,
     &   ndc, cFine, cCoarse, ndc0, c0, c1, option, orderOfAccuracy, ipar )
c =========================================================================================
c Description:
c    Form a coarse grid operator by averaging from a fine grid operator.
c
c With option==fullWeighting the operator at a point is averaged with the operators of 
c  the nearest neighbours. In 1D point, for example we would average the equations at
c points $i-1$, $i$ and $i+1$
c 
c    a_{i-2} u_{i-2}  b_{i-1} u_{i-1}  c_{i  } u_{i  }
c                     a_{i-1} u_{i-1}  b_{i  } u_{i  }  c_{i+1} u_{i+1}
c                                      a_{i  } u_{i  }  b_{i+1} u_{i+1}  c_{i+2} u_{i+2}
c
c  We average the 3 equations with weights $1/4$, $1/2$ and $1/4$. The values at points 
c  $i-1$ and $i+1$ are distributed to points $i-2$, $i$ and $i+2$ to give a new operator
c only defined on $i-2$, $i$ and $i+2$.
c
c  With option==injection the operator at point $i$ is averaged as if the stencil operator
c  at points $i-1$ and $i+1$ had the same coefficients as point $i$.
c
c /option (input) : injection, fullWeighting
c 
c =========================================================================================

      implicit none
      integer fullWeighting,restrictedFullWeighting,injection
c     **these should agree with the values in Ogmg.h **** 
      parameter( fullWeighting=0,restrictedFullWeighting=1,injection=2 )

      integer nd, nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &     md1a,md1b,md2a,md2b,md3a,md3b,
     &     ndc, ndc0, option(0:2), orderOfAccuracy, ipar(0:*)

      real cFine(0:ndc-1,nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real cCoarse(0:ndc-1,md1a:md1b,md2a:md2b,md3a:md3b)

      integer j1a,j1b,j2a,j2b,j3a,j3b
      integer i1a,i1b,i1c,i2a,i2b,i2c,i3a,i3b,i3c
      integer i1pa,i1pb,i2pa,i2pb,i3pa,i3pb
      integer side,axis

c     realArray c0(27,J1,I2p,I3p);
      real c0(0:ndc0-1,j1a:j1b,i2pa:i2pb,i3pa:i3pb)
c     realArray c1(27,J1,J2,I3p);
      real c1(0:ndc0-1,j1a:j1b,j2a:j2b,i3pa:i3pb)

      real fw0_1d_0,fw0_1d_1,fw0_1d_2
      real fw1_1d_0,fw1_1d_1,fw1_1d_2
      real fw2_1d_0,fw2_1d_1,fw2_1d_2
      real inj_1d_0 ,inj_1d_1 ,inj_1d_2
      real inj_1d_0a,inj_1d_1a,inj_1d_2a
      real inj_1d_0b,inj_1d_1b,inj_1d_2b

      real fw0_1d5_0,fw0_1d5_1,fw0_1d5_2,fw0_1d5_3,fw0_1d5_4
      real fw1_1d5_0,fw1_1d5_1,fw1_1d5_2,fw1_1d5_3,fw1_1d5_4
      real fw2_1d5_0,fw2_1d5_1,fw2_1d5_2,fw2_1d5_3,fw2_1d5_4
      real inj_1d5_0,inj_1d5_1,inj_1d5_2,inj_1d5_3,inj_1d5_4
      real inj_1d5_0a,inj_1d5_1a,inj_1d5_2a,inj_1d5_3a,inj_1d5_4a
      real inj_1d5_0b,inj_1d5_1b,inj_1d5_2b,inj_1d5_3b,inj_1d5_4b


      integer i1,i2,i3,i2p,i3p,j1,j2,j3,m,m0,m1,m2,m3,m4,width
      integer mm
      real tsum
c     statement functions      

c ***************  2nd-order accurate 3 point operator *************************
c       1D full weighting along axis1==0 for the 3 coefficients
      fw0_1d_0(m0,m1,m2,i1,i2,i3)=.25*(cFine(m0,i1-1,i2,i3)+
     &            .5*cFine(m1,i1-1,i2,i3)+cFine(m0,i1  ,i2,i3))
      fw0_1d_1(m0,m1,m2,i1,i2,i3)=.5*cFine(m1,i1  ,i2,i3)+
     & .125*(cFine(m1,i1-1,i2,i3)+cFine(m1,i1+1,i2,i3))+ 
     &      .25*(cFine(m2,i1-1,i2,i3)+cFine(m2,i1  ,i2,i3)+ 
     &      cFine(m0,i1  ,i2,i3)+cFine(m0,i1+1,i2,i3)) 
      fw0_1d_2(m0,m1,m2,i1,i2,i3)=.25*(cFine(m2,i1  ,i2,i3)+
     &     .5*cFine(m1,i1+1,i2,i3)+cFine(m2,i1+1,i2,i3))

c        1d full weighting along axis2==1 for the 3 coefficients
      fw1_1d_0(m0,m1,m2,i1,i2,i3)=.25*(c0(m0,i1,i2-1,i3)+
     & .5*c0(m1,i1,i2-1,i3)+c0(m0,i1,i2  ,i3))
      fw1_1d_1(m0,m1,m2,i1,i2,i3)=.5*c0(m1,i1  ,i2,i3)+
     &    .125*(c0(m1,i1,i2-1,i3)+c0(m1,i1,i2+1,i3))+ 
     &              .25*(c0(m2,i1,i2-1,i3)+c0(m2,i1  ,i2,i3)+ 
     &                  c0(m0,i1  ,i2,i3)+c0(m0,i1,i2+1,i3)) 
      fw1_1d_2(m0,m1,m2,i1,i2,i3)=.25*(c0(m2,i1  ,i2,i3)+
     &  .5*c0(m1,i1,i2+1,i3)+c0(m2,i1,i2+1,i3)) 

c          1d full weighting along axis2==2 for the 3 coefficients
      fw2_1d_0(m0,m1,m2,i1,i2,i3)=.25*(c1(m0,i1,i2,i3-1)+
     &  .5*c1(m1,i1,i2,i3-1)+c1(m0,i1,i2  ,i3)) 
      fw2_1d_1(m0,m1,m2,i1,i2,i3)=.5*c1(m1,i1  ,i2,i3)+
     &  .125*(c1(m1,i1,i2,i3-1)+c1(m1,i1,i2,i3+1))+
     &   .25*(c1(m2,i1,i2,i3-1)+c1(m2,i1  ,i2,i3)+
     &    c1(m0,i1  ,i2,i3)+c1(m0,i1,i2,i3+1))
      fw2_1d_2(m0,m1,m2,i1,i2,i3)=.25*(c1(m2,i1  ,i2,i3)+
     &  .5*c1(m1,i1,i2,i3+1)+c1(m2,i1,i2,i3+1)) 

c ****** The "injection" macros are like FW but they only average points on i1,i2,i3 ****
      inj_1d_0(m0,m1,m2,i1,i2,i3)=.25*(cFine(m0,i1,i2,i3)+
     &  .5*cFine(m1,i1,i2,i3)+cFine(m0,i1,i2,i3)) 
      inj_1d_1(m0,m1,m2,i1,i2,i3)=.5*cFine(m1,i1,i2,i3)+
     & .125*(cFine(m1,i1,i2,i3)+cFine(m1,i1,i2,i3))+ 
     &                .25*(cFine(m2,i1,i2,i3)+cFine(m2,i1,i2,i3)+ 
     &                     cFine(m0,i1,i2,i3)+cFine(m0,i1,i2,i3)) 
      inj_1d_2(m0,m1,m2,i1,i2,i3)=.25*(cFine(m2,i1,i2,i3)+
     & .5*cFine(m1,i1,i2,i3)+cFine(m2,i1,i2,i3)) 

      inj_1d_0a(m0,m1,m2,i1,i2,i3)=.25*(c0(m0,i1,i2,i3)+
     &  .5*c0(m1,i1,i2,i3)+c0(m0,i1,i2,i3)) 
      inj_1d_1a(m0,m1,m2,i1,i2,i3)=.5*c0(m1,i1,i2,i3)+
     & .125*(c0(m1,i1,i2,i3)+c0(m1,i1,i2,i3))+ 
     &                .25*(c0(m2,i1,i2,i3)+c0(m2,i1,i2,i3)+ 
     &                     c0(m0,i1,i2,i3)+c0(m0,i1,i2,i3)) 
      inj_1d_2a(m0,m1,m2,i1,i2,i3)=.25*(c0(m2,i1,i2,i3)+
     & .5*c0(m1,i1,i2,i3)+c0(m2,i1,i2,i3)) 

      inj_1d_0b(m0,m1,m2,i1,i2,i3)=.25*(c1(m0,i1,i2,i3)+
     &  .5*c1(m1,i1,i2,i3)+c1(m0,i1,i2,i3)) 
      inj_1d_1b(m0,m1,m2,i1,i2,i3)=.5*c1(m1,i1,i2,i3)+
     & .125*(c1(m1,i1,i2,i3)+c1(m1,i1,i2,i3))+ 
     &                .25*(c1(m2,i1,i2,i3)+c1(m2,i1,i2,i3)+ 
     &                     c1(m0,i1,i2,i3)+c1(m0,i1,i2,i3)) 
      inj_1d_2b(m0,m1,m2,i1,i2,i3)=.25*(c1(m2,i1,i2,i3)+
     & .5*c1(m1,i1,i2,i3)+c1(m2,i1,i2,i3)) 

c ***************  4th-order accurate 5 point operator *************************
c            average with a full weighting of 3 equations and linear interpolation
#beginMacro fw5(fw0,fw1,fw2,fw3,fw4,i1m,i1p,i2m,i2p,i3m,i3p,u)
fw0(m0,m1,m2,m3,m4,i1,i2,i3)=.125*u(m0,i1m,i2m,i3m)
fw1(m0,m1,m2,m3,m4,i1,i2,i3)=.125*u(m0,i1m,i2m,i3m)+.25*u(m1,i1m,i2m,i3m)+.125*u(m2,i1m,i2m,i3m)+\
       .5*u(m0,i1,i2,i3)+.25*u(m1,i1,i2,i3)+.125*u(m0,i1p,i2p,i3p)

!fw2(m0,m1,m2,m3,m4,i1,i2,i3)=.125*u(m2,i1m,i2m,i3m)+.25*u(m3,i1m,i2m,i3m)+.125*u(m4,i1m,i2m,i3m)+
!           .25*u(m1,i1p,i2p,i3p)+.5*u(m2,i1,i2,i3)+.25*u(m3,i1m,i2m,i3m)+
!                             .125*u(m0,i1p,i2p,i3p)+.25*u(m1,i1p,i2p,i3p)+.125*u(m2,i1p,i2p,i3p)
! *wdh* 100118 fix: 
fw2(m0,m1,m2,m3,m4,i1,i2,i3)=.125*u(m0,i1p,i2p,i3p)+.25*u(m1,i1,i2,i3)+.125*u(m2,i1m,i2m,i3m)+\
           .25*u(m1,i1p,i2p,i3p)+.5*u(m2,i1,i2,i3)+.25*u(m3,i1m,i2m,i3m)+\
                             .125*u(m4,i1m,i2m,i3m)+.25*u(m3,i1,i2,i3)+.125*u(m2,i1p,i2p,i3p)

fw3(m0,m1,m2,m3,m4,i1,i2,i3)=.125*u(m4,i1p,i2p,i3p)+.25*u(m3,i1p,i2p,i3p)+.125*u(m2,i1p,i2p,i3p)+\
        .5*u(m4,i1,i2,i3)+.25*u(m3,i1,i2,i3)+.125*u(m4,i1m,i2m,i3m)
fw4(m0,m1,m2,m3,m4,i1,i2,i3)=.125*u(m4,i1p,i2p,i3p)
#endMacro

c       1D full weighting along axis1==0 for the 3 coefficients
      fw5(fw0_1d5_0,fw0_1d5_1,fw0_1d5_2,fw0_1d5_3,fw0_1d5_4,i1-1,i1+1,i2,i2,i3,i3,cFine)
c        1d full weighting along axis2==1 for the 3 coefficients
      fw5(fw1_1d5_0,fw1_1d5_1,fw1_1d5_2,fw1_1d5_3,fw1_1d5_4,i1,i1,i2-1,i2+1,i3,i3,c0)
c          1d full weighting along axis2==2 for the 3 coefficients
      fw5(fw2_1d5_0,fw2_1d5_1,fw2_1d5_2,fw2_1d5_3,fw2_1d5_4,i1,i1,i2,i2,i3-1,i3+1,c1)

c       1D full weighting along axis1==0 for the 3 coefficients
      fw5(inj_1d5_0,inj_1d5_1,inj_1d5_2,inj_1d5_3,inj_1d5_4,i1,i1,i2,i2,i3,i3,cFine)
c        1d full weighting along axis2==1 for the 3 coefficients
      fw5(inj_1d5_0a,inj_1d5_1a,inj_1d5_2a,inj_1d5_3a,inj_1d5_4a,i1,i1,i2,i2,i3,i3,c0)
c          1d full weighting along axis2==2 for the 3 coefficients
      fw5(inj_1d5_0b,inj_1d5_1b,inj_1d5_2b,inj_1d5_3b,inj_1d5_4b,i1,i1,i2,i2,i3,i3,c1)

c      inj_1d5_0(m0,m1,m2,m3,m4,i1,i2,i3)=.25*(cFine(m0,i1,i2,i3)+
c     &  .5*cFine(m1,i1,i2,i3)+cFine(m0,i1,i2,i3)) 
c      inj_1d5_1(m0,m1,m2,m3,m4,i1,i2,i3)=.5*cFine(m1,i1,i2,i3)+
c     & .125*(cFine(m1,i1,i2,i3)+cFine(m1,i1,i2,i3))+ 
c     &                .25*(cFine(m2,i1,i2,i3)+cFine(m2,i1,i2,i3)+ 
c     &                     cFine(m0,i1,i2,i3)+cFine(m0,i1,i2,i3)) 
c      inj_1d5_2(m0,m1,m2,m3,m4,i1,i2,i3)=.25*(cFine(m2,i1,i2,i3)+
c     & .5*cFine(m1,i1,i2,i3)+cFine(m2,i1,i2,i3)) 
c
c      inj_1d5_0a(m0,m1,m2,m3,m4,i1,i2,i3)=.25*(c0(m0,i1,i2,i3)+
c     &  .5*c0(m1,i1,i2,i3)+c0(m0,i1,i2,i3)) 
c
c      inj_1d5_1a(m0,m1,m2,m3,m4,i1,i2,i3)=.5*c0(m1,i1,i2,i3)+
c     & .125*(c0(m1,i1,i2,i3)+c0(m1,i1,i2,i3))+ 
c     &                .25*(c0(m2,i1,i2,i3)+c0(m2,i1,i2,i3)+ 
c     &                     c0(m0,i1,i2,i3)+c0(m0,i1,i2,i3)) 
c      inj_1d5_2a(m0,m1,m2,m3,m4,i1,i2,i3)=.25*(c0(m2,i1,i2,i3)+
c     & .5*c0(m1,i1,i2,i3)+c0(m2,i1,i2,i3)) 
c
c      inj_1d5_0b(m0,m1,m2,m3,m4,i1,i2,i3)=.25*(c1(m0,i1,i2,i3)+
c     &  .5*c1(m1,i1,i2,i3)+c1(m0,i1,i2,i3)) 
c      inj_1d5_1b(m0,m1,m2,m3,m4,i1,i2,i3)=.5*c1(m1,i1,i2,i3)+
c     & .125*(c1(m1,i1,i2,i3)+c1(m1,i1,i2,i3))+ 
c     &                .25*(c1(m2,i1,i2,i3)+c1(m2,i1,i2,i3)+ 
c     &                     c1(m0,i1,i2,i3)+c1(m0,i1,i2,i3)) 
c      inj_1d5_2b(m0,m1,m2,m3,m4,i1,i2,i3)=.25*(c1(m2,i1,i2,i3)+
c     & .5*c1(m1,i1,i2,i3)+c1(m2,i1,i2,i3)) 


      width=orderOfAccuracy+1
      if( orderOfAccuracy.ne.2 .and. orderOfAccuracy.ne.4 )then
        write(*,*) 'averageOpt:ERROR:orderOfAccuracy=',orderOfAccuracy
         stop 1
      end if

c    Average interior points using the full weighting operator

#beginMacro AVERAGE(ORDER)
if( nd.eq.1 )then

  if( option(0).eq.fullWeighting )then
    i3=i3a
    do j3=j3a,j3b
      i2=i2a
      do j2=j2a,j2b
        i1=i1a
        do j1=j1a,j1b
#If #ORDER == "2"
          cCoarse(0,j1,j2,j3)=fw0_1d_0(0,1,2,i1,i2,i3)
          cCoarse(1,j1,j2,j3)=fw0_1d_1(0,1,2,i1,i2,i3)
          cCoarse(2,j1,j2,j3)=fw0_1d_2(0,1,2,i1,i2,i3)
#Else
          cCoarse(0,j1,j2,j3)=fw0_1d5_0(0,1,2,3,4,i1,i2,i3)
          cCoarse(1,j1,j2,j3)=fw0_1d5_1(0,1,2,3,4,i1,i2,i3)
          cCoarse(2,j1,j2,j3)=fw0_1d5_2(0,1,2,3,4,i1,i2,i3)
          cCoarse(3,j1,j2,j3)=fw0_1d5_3(0,1,2,3,4,i1,i2,i3)
          cCoarse(4,j1,j2,j3)=fw0_1d5_4(0,1,2,3,4,i1,i2,i3)
#End
          i1=i1+i1c
        end do
        i2=i2+i2c
      end do
      i3=i3+i3c
    end do
  else
    i3=i3a
    do j3=j3a,j3b
      i2=i2a
      do j2=j2a,j2b
        i1=i1a
        do j1=j1a,j1b
#If #ORDER == "2"
          cCoarse(0,j1,j2,j3)=inj_1d_0(0,1,2,i1,i2,i3)
          cCoarse(1,j1,j2,j3)=inj_1d_1(0,1,2,i1,i2,i3)
          cCoarse(2,j1,j2,j3)=inj_1d_2(0,1,2,i1,i2,i3)
#Else
          cCoarse(0,j1,j2,j3)=inj_1d5_0(0,1,2,3,4,i1,i2,i3)
          cCoarse(1,j1,j2,j3)=inj_1d5_1(0,1,2,3,4,i1,i2,i3)
          cCoarse(2,j1,j2,j3)=inj_1d5_2(0,1,2,3,4,i1,i2,i3)
          cCoarse(3,j1,j2,j3)=inj_1d5_3(0,1,2,3,4,i1,i2,i3)
          cCoarse(4,j1,j2,j3)=inj_1d5_4(0,1,2,3,4,i1,i2,i3)
#End
          i1=i1+i1c
        end do
        i2=i2+i2c
      end do
      i3=i3+i3c
    end do
  end if

else if( nd.eq.2 )then

c     // const int width=3
c     // numbering of coefficients:
c     //       6  7  8
c     //       3  4  5
c     //       0  1  2

c     // first average in direction 0

  do m=0,width-1  ! ***** remove this loop to improve performance ****

    m0=width*m
    m1=m0+1
    m2=m0+2
    m3=m0+3
    m4=m0+4
    if( option(0).eq.fullWeighting )then
c             average coefficients m0,m1,m2 along axis1:
!  write(*,'(" avOpt[0]: m=",i3," average m0,m1,m2,m3,m4=",5i3)') m,m0,m1,m2,m3,m4
      do i3p=i3pa,i3pb
        do i2p=i2pa,i2pb
          i1=i1a
          do j1=j1a,j1b
#If #ORDER == "2"
            c0(m0,j1,i2p,i3p)=fw0_1d_0(m0,m1,m2,i1,i2p,i3p) 
            c0(m1,j1,i2p,i3p)=fw0_1d_1(m0,m1,m2,i1,i2p,i3p)
            c0(m2,j1,i2p,i3p)=fw0_1d_2(m0,m1,m2,i1,i2p,i3p)
#Else
            c0(m0,j1,i2p,i3p)=fw0_1d5_0(m0,m1,m2,m3,m4,i1,i2p,i3p) 
            c0(m1,j1,i2p,i3p)=fw0_1d5_1(m0,m1,m2,m3,m4,i1,i2p,i3p)
            c0(m2,j1,i2p,i3p)=fw0_1d5_2(m0,m1,m2,m3,m4,i1,i2p,i3p)
            c0(m3,j1,i2p,i3p)=fw0_1d5_3(m0,m1,m2,m3,m4,i1,i2p,i3p) 
            c0(m4,j1,i2p,i3p)=fw0_1d5_4(m0,m1,m2,m3,m4,i1,i2p,i3p)

!             tsum =0.
!             do mm=0,width*width-1
!               tsum=tsum+c0(mm,j1,i2p,i3p)
!             end do
!    write(*,'(" avOpt[0]: i=",2i3," c=",5e10.2," sum,tsum=",2e12.4)') i1,i2p,c0(m0,j1,i2p,i3p),c0(m1,j1,i2p,i3p),c0(m2,j1,i2p,i3p),c0(m3,j1,i2p,i3p),c0(m4,j1,i2p,i3p),c0(m0,j1,i2p,i3p)+c0(m1,j1,i2p,i3p)+c0(m2,j1,i2p,i3p)+c0(m3,j1,i2p,i3p)+c0(m4,j1,i2p,i3p),tsum

#End
            i1=i1+i1c
          end do
        end do
      end do
    else if( option(0).eq.restrictedFullWeighting )then
      do i3p=i3pa,i3pb
        do i2p=i2pa,i2pb
          i1=i1a
          do j1=j1a,j1b
#If #ORDER == "2"
            c0(m0,j1,i2p,i3p)=inj_1d_0(m0,m1,m2,i1,i2p,i3p)  
            c0(m1,j1,i2p,i3p)=inj_1d_1(m0,m1,m2,i1,i2p,i3p)
            c0(m2,j1,i2p,i3p)=inj_1d_2(m0,m1,m2,i1,i2p,i3p)
#Else
          c0(m0,j1,i2p,i3p)=inj_1d5_0(m0,m1,m2,m3,m4,i1,i2p,i3p) 
          c0(m1,j1,i2p,i3p)=inj_1d5_1(m0,m1,m2,m3,m4,i1,i2p,i3p)
          c0(m2,j1,i2p,i3p)=inj_1d5_2(m0,m1,m2,m3,m4,i1,i2p,i3p)
          c0(m3,j1,i2p,i3p)=inj_1d5_3(m0,m1,m2,m3,m4,i1,i2p,i3p) 
          c0(m4,j1,i2p,i3p)=inj_1d5_4(m0,m1,m2,m3,m4,i1,i2p,i3p)
#End
            i1=i1+i1c
          end do
        end do
      end do
    else if( option(0).eq.injection )then
      do i3p=i3pa,i3pb
        do i2p=i2pa,i2pb
          i1=i1a
          do j1=j1a,j1b
#If #ORDER == "2"
            c0(m0,j1,i2p,i3p)=cFine(m0,i1,i2p,i3p)  
            c0(m1,j1,i2p,i3p)=cFine(m1,i1,i2p,i3p)
            c0(m2,j1,i2p,i3p)=cFine(m2,i1,i2p,i3p)
#Else
            c0(m0,j1,i2p,i3p)=cFine(m0,i1,i2p,i3p)  
            c0(m1,j1,i2p,i3p)=cFine(m1,i1,i2p,i3p)
            c0(m2,j1,i2p,i3p)=cFine(m2,i1,i2p,i3p)
            c0(m3,j1,i2p,i3p)=cFine(m3,i1,i2p,i3p)  
            c0(m4,j1,i2p,i3p)=cFine(m4,i1,i2p,i3p)
#End
            i1=i1+i1c
          end do
        end do
      end do
    else
      stop 18
    end if
  end do

c     now average in direction 1
  do m=0,width-1
    m0=m
    m1=m0+width
    m2=m0+width*2
    m3=m0+width*3
    m4=m0+width*4
    if( option(1).eq.fullWeighting )then
      do i3p=i3pa,i3pb
        i2=i2a
        do j2=j2a,j2b
          do j1=j1a,j1b
#If #ORDER == "2"
            cCoarse(m0,j1,j2,i3p)=fw1_1d_0(m0,m1,m2,j1,i2,i3p) 
            cCoarse(m1,j1,j2,i3p)=fw1_1d_1(m0,m1,m2,j1,i2,i3p) 
            cCoarse(m2,j1,j2,i3p)=fw1_1d_2(m0,m1,m2,j1,i2,i3p) 
#Else
            cCoarse(m0,j1,j2,i3p)=fw1_1d5_0(m0,m1,m2,m3,m4,j1,i2,i3p) 
            cCoarse(m1,j1,j2,i3p)=fw1_1d5_1(m0,m1,m2,m3,m4,j1,i2,i3p) 
            cCoarse(m2,j1,j2,i3p)=fw1_1d5_2(m0,m1,m2,m3,m4,j1,i2,i3p) 
            cCoarse(m3,j1,j2,i3p)=fw1_1d5_3(m0,m1,m2,m3,m4,j1,i2,i3p) 
            cCoarse(m4,j1,j2,i3p)=fw1_1d5_4(m0,m1,m2,m3,m4,j1,i2,i3p) 
#End
          end do
          i2=i2+i2c
        end do
      end do
    else if( option(1).eq.restrictedFullWeighting )then
      do i3p=i3pa,i3pb
        i2=i2a
        do j2=j2a,j2b
          do j1=j1a,j1b
#If #ORDER == "2"
            cCoarse(m0,j1,j2,i3p)=inj_1d_0a(m0,m1,m2,j1,i2,i3p) 
            cCoarse(m1,j1,j2,i3p)=inj_1d_1a(m0,m1,m2,j1,i2,i3p) 
            cCoarse(m2,j1,j2,i3p)=inj_1d_2a(m0,m1,m2,j1,i2,i3p) 
#Else
          cCoarse(m0,j1,j2,i3p)=inj_1d5_0a(m0,m1,m2,m3,m4,j1,i2,i3p) 
          cCoarse(m1,j1,j2,i3p)=inj_1d5_1a(m0,m1,m2,m3,m4,j1,i2,i3p) 
          cCoarse(m2,j1,j2,i3p)=inj_1d5_2a(m0,m1,m2,m3,m4,j1,i2,i3p) 
          cCoarse(m3,j1,j2,i3p)=inj_1d5_3a(m0,m1,m2,m3,m4,j1,i2,i3p) 
          cCoarse(m4,j1,j2,i3p)=inj_1d5_4a(m0,m1,m2,m3,m4,j1,i2,i3p) 
#End
          end do
          i2=i2+i2c
        end do
      end do
    else if( option(1).eq.injection )then
      do i3p=i3pa,i3pb
        i2=i2a
        do j2=j2a,j2b
          do j1=j1a,j1b
#If #ORDER == "2"
            cCoarse(m0,j1,j2,i3p)=c0(m0,j1,i2,i3p)
            cCoarse(m1,j1,j2,i3p)=c0(m1,j1,i2,i3p)
            cCoarse(m2,j1,j2,i3p)=c0(m2,j1,i2,i3p)
#Else
            cCoarse(m0,j1,j2,i3p)=c0(m0,j1,i2,i3p)
            cCoarse(m1,j1,j2,i3p)=c0(m1,j1,i2,i3p)
            cCoarse(m2,j1,j2,i3p)=c0(m2,j1,i2,i3p)
            cCoarse(m3,j1,j2,i3p)=c0(m3,j1,i2,i3p)
            cCoarse(m4,j1,j2,i3p)=c0(m4,j1,i2,i3p)
#End
          end do
          i2=i2+i2c
        end do
      end do
    else
      stop 14
    end if
  end do


else if( nd.eq.3 )then

c    // first average in direction 0
c    // 0 1 2
c    // 3 4 5
c    // 6 7 8
c    // 9 10 11
c    // ...

c  i1=2
c  i2=2
c  i3=2
c  write(*,*) ' averageOpt: orderOfAccuracy,width=',orderOfAccuracy,width
c  write(*,''' cFine(.,2,2,2) = '',(25(f7.1,1x))') (cFine(m,i1,i2,i3),m=0,width*width*width-1)

  do m=0,width*width-1

    m0=width*m
    m1=m0+1
    m2=m0+2
    m3=m0+3
    m4=m0+4
    if( option(0).eq.fullWeighting )then
      do i3p=i3pa,i3pb
        do i2p=i2pa,i2pb
          i1=i1a
          do j1=j1a,j1b
#If #ORDER == "2"
            c0(m0,j1,i2p,i3p)=fw0_1d_0(m0,m1,m2,i1,i2p,i3p) ! average coefficients m0,m1,m2 along axis1
            c0(m1,j1,i2p,i3p)=fw0_1d_1(m0,m1,m2,i1,i2p,i3p)
            c0(m2,j1,i2p,i3p)=fw0_1d_2(m0,m1,m2,i1,i2p,i3p)
#Else
            c0(m0,j1,i2p,i3p)=fw0_1d5_0(m0,m1,m2,m3,m4,i1,i2p,i3p) 
            c0(m1,j1,i2p,i3p)=fw0_1d5_1(m0,m1,m2,m3,m4,i1,i2p,i3p)
            c0(m2,j1,i2p,i3p)=fw0_1d5_2(m0,m1,m2,m3,m4,i1,i2p,i3p)
            c0(m3,j1,i2p,i3p)=fw0_1d5_3(m0,m1,m2,m3,m4,i1,i2p,i3p) 
            c0(m4,j1,i2p,i3p)=fw0_1d5_4(m0,m1,m2,m3,m4,i1,i2p,i3p)
#End
            i1=i1+i1c
          end do
        end do
      end do
    else if( option(0).eq.restrictedFullWeighting )then
      do i3p=i3pa,i3pb
        do i2p=i2pa,i2pb
          i1=i1a
          do j1=j1a,j1b
#If #ORDER == "2"
            c0(m0,j1,i2p,i3p)=inj_1d_0(m0,m1,m2,i1,i2p,i3p)  
            c0(m1,j1,i2p,i3p)=inj_1d_1(m0,m1,m2,i1,i2p,i3p)
            c0(m2,j1,i2p,i3p)=inj_1d_2(m0,m1,m2,i1,i2p,i3p)
#Else
            c0(m0,j1,i2p,i3p)=inj_1d5_0(m0,m1,m2,m3,m4,i1,i2p,i3p) 
            c0(m1,j1,i2p,i3p)=inj_1d5_1(m0,m1,m2,m3,m4,i1,i2p,i3p)
            c0(m2,j1,i2p,i3p)=inj_1d5_2(m0,m1,m2,m3,m4,i1,i2p,i3p)
            c0(m3,j1,i2p,i3p)=inj_1d5_3(m0,m1,m2,m3,m4,i1,i2p,i3p) 
            c0(m4,j1,i2p,i3p)=inj_1d5_4(m0,m1,m2,m3,m4,i1,i2p,i3p)
#End
            i1=i1+i1c
          end do
        end do
      end do
    end if
  end do

c j1=1
c i2=2
c i3=2
c write(*,''' c0(.,1,2,2) = '',(25(f7.1,1x))') (c0(m,j1,i2,i3),m=0,width*width*width-1)

c     // now average in direction 1
c     // 0 3 6
c     // 1 4 7
c     // 2 5 8
c     // 9 12 15
c     // 10 13 16
c     // 11 14 17
c     // 18
c     // 19
c     // 20 23 26 

  do m=0,width*width-1

    m0=m+(m/width)*width*(width-1)
    m1=m0+width
    m2=m0+width*2
    m3=m0+width*3
    m4=m0+width*4
    if( option(1).eq.fullWeighting )then 
      do i3p=i3pa,i3pb
        i2=i2a
        do j2=j2a,j2b
          do j1=j1a,j1b
#If #ORDER == "2"
            c1(m0,j1,j2,i3p)=fw1_1d_0(m0,m1,m2,j1,i2,i3p) 
            c1(m1,j1,j2,i3p)=fw1_1d_1(m0,m1,m2,j1,i2,i3p) 
            c1(m2,j1,j2,i3p)=fw1_1d_2(m0,m1,m2,j1,i2,i3p) 
#Else
            c1(m0,j1,j2,i3p)=fw1_1d5_0(m0,m1,m2,m3,m4,j1,i2,i3p) 
            c1(m1,j1,j2,i3p)=fw1_1d5_1(m0,m1,m2,m3,m4,j1,i2,i3p) 
            c1(m2,j1,j2,i3p)=fw1_1d5_2(m0,m1,m2,m3,m4,j1,i2,i3p) 
            c1(m3,j1,j2,i3p)=fw1_1d5_3(m0,m1,m2,m3,m4,j1,i2,i3p) 
            c1(m4,j1,j2,i3p)=fw1_1d5_4(m0,m1,m2,m3,m4,j1,i2,i3p) 
#End
          end do
          i2=i2+i2c
        end do
      end do
    else if( option(1).eq.restrictedFullWeighting )then
      do i3p=i3pa,i3pb
        i2=i2a
        do j2=j2a,j2b
          do j1=j1a,j1b
#If #ORDER == "2"
            c1(m0,j1,j2,i3p)=inj_1d_0a(m0,m1,m2,j1,i2,i3p) 
            c1(m1,j1,j2,i3p)=inj_1d_1a(m0,m1,m2,j1,i2,i3p) 
            c1(m2,j1,j2,i3p)=inj_1d_2a(m0,m1,m2,j1,i2,i3p) 
#Else
            c1(m0,j1,j2,i3p)=inj_1d5_0a(m0,m1,m2,m3,m4,j1,i2,i3p) 
            c1(m1,j1,j2,i3p)=inj_1d5_1a(m0,m1,m2,m3,m4,j1,i2,i3p) 
            c1(m2,j1,j2,i3p)=inj_1d5_2a(m0,m1,m2,m3,m4,j1,i2,i3p) 
            c1(m3,j1,j2,i3p)=inj_1d5_3a(m0,m1,m2,m3,m4,j1,i2,i3p) 
            c1(m4,j1,j2,i3p)=inj_1d5_4a(m0,m1,m2,m3,m4,j1,i2,i3p) 
#End
          end do
          i2=i2+i2c
        end do
      end do
    end if
  end do
        
c j1=1
c j2=1
c i3=2
c write(*,''' c1(.,1,1,2) = '',(25(f7.1,1x))') (c1(m,j1,j2,i3),m=0,width*width*width-1)

c     // now average in direction 2
c     // 0  9 18
c     // 1 10 19
c     // 2 11 20
c     // 3 12 21
  do m=0,width*width-1
    m0=m
    m1=m0+width*width
    m2=m0+width*width*2
    m3=m0+width*width*3
    m4=m0+width*width*4
    if( option(2).eq.fullWeighting )then
      i3=i3a
      do j3=j3a,j3b
        do j2=j2a,j2b
          do j1=j1a,j1b
#If #ORDER == "2"
            cCoarse(m0,j1,j2,j3)=fw2_1d_0(m0,m1,m2,j1,j2,i3) 
            cCoarse(m1,j1,j2,j3)=fw2_1d_1(m0,m1,m2,j1,j2,i3) 
            cCoarse(m2,j1,j2,j3)=fw2_1d_2(m0,m1,m2,j1,j2,i3) 
#Else
            cCoarse(m0,j1,j2,j3)=fw2_1d5_0(m0,m1,m2,m3,m4,j1,j2,i3) 
            cCoarse(m1,j1,j2,j3)=fw2_1d5_1(m0,m1,m2,m3,m4,j1,j2,i3) 
            cCoarse(m2,j1,j2,j3)=fw2_1d5_2(m0,m1,m2,m3,m4,j1,j2,i3) 
            cCoarse(m3,j1,j2,j3)=fw2_1d5_3(m0,m1,m2,m3,m4,j1,j2,i3) 
            cCoarse(m4,j1,j2,j3)=fw2_1d5_4(m0,m1,m2,m3,m4,j1,j2,i3) 
#End
          end do
        end do
        i3=i3+i3c
      end do
    else if( option(2).eq.restrictedFullWeighting )then
      i3=i3a
      do j3=j3a,j3b
        do j2=j2a,j2b
          do j1=j1a,j1b
#If #ORDER == "2"
            cCoarse(m0,j1,j2,j3)=inj_1d_0b(m0,m1,m2,j1,j2,i3) 
            cCoarse(m1,j1,j2,j3)=inj_1d_1b(m0,m1,m2,j1,j2,i3) 
            cCoarse(m2,j1,j2,j3)=inj_1d_2b(m0,m1,m2,j1,j2,i3) 
#Else
            cCoarse(m0,j1,j2,j3)=inj_1d5_0b(m0,m1,m2,m3,m4,j1,j2,i3) 
            cCoarse(m1,j1,j2,j3)=inj_1d5_1b(m0,m1,m2,m3,m4,j1,j2,i3) 
            cCoarse(m2,j1,j2,j3)=inj_1d5_2b(m0,m1,m2,m3,m4,j1,j2,i3) 
            cCoarse(m3,j1,j2,j3)=inj_1d5_3b(m0,m1,m2,m3,m4,j1,j2,i3) 
            cCoarse(m4,j1,j2,j3)=inj_1d5_4b(m0,m1,m2,m3,m4,j1,j2,i3) 
#End
          end do
        end do
        i3=i3+i3c
      end do
    end if
  end do

c j1=1
c j2=1
c j3=1
c write(*,''' cCoarse(.,1,1,1) = '',(25(f7.1,1x))') (cCoarse(m,j1,j2,j3),m=0,width*width*width-1)

else

  write(*,*) 'ERROR: invalid numberOfDimensions=',nd
  stop
end if
#endMacro

      if( orderOfAccuracy.eq.2 )then
        AVERAGE(2)
      else
        ! write(*,*) '@@@averageOpt: orderOfAccuracy=',orderOfAccuracy
        AVERAGE(4)
      end if


#beginMacro setSym(m1,m2)
 cCoarse(m1,j1,j2,j3)=cCoarse(m1,j1,j2,j3)+cCoarse(m2,j1,j2,j3)
 cCoarse(m2,j1,j2,j3)=0.
#endMacro

#beginMacro setSymValues(m0a,m0b,m1a,m1b,m2a,m2b,m3a,m3b,m4a,m4b,m5a,m5b,m6a,m6b,m7a,m7b,m8a,m8b,m9a,m9b)
 do j3=j3a,j3b
 do j2=j2a,j2b
 do j1=j1a,j1b
  setSym(m0a,m0b)
  setSym(m1a,m1b)
  setSym(m2a,m2b)
  setSym(m3a,m3b)
  setSym(m4a,m4b)
  setSym(m5a,m5b)
  setSym(m6a,m6b)
  setSym(m7a,m7b)
  setSym(m8a,m8b)
  setSym(m9a,m9b)
 end do
 end do
 end do
#endMacro

#beginMacro assignSym(n0a,n0b,n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,n5a,n5b,n6a,n6b,n7a,n7b,n8a,n8b,n9a,n9b)
 if( side.eq.0 )then
   setSymValues(n0a,n0b,n1a,n1b,n2a,n2b,n3a,n3b,n4a,n4b,n5a,n5b,n6a,n6b,n7a,n7b,n8a,n8b,n9a,n9b)
 else
   setSymValues(n0b,n0a,n1b,n1a,n2b,n2a,n3b,n3a,n4b,n4a,n5b,n5a,n6b,n6a,n7b,n7a,n8b,n8a,n9b,n9a)
 end if
#endMacro
      if( ipar(0).eq.1 )then


        side=ipar(1)
        axis=ipar(2)

        write(*,'(" >>>> averageOpt: apply even symmetry to coeff, side,axis=",2i2," <<<")') side,axis

        if( side.lt.0 .or. side.gt.1 .or. axis.lt.0 .or. axis.gt.nd )then
          stop 67
        end if

        if( axis.eq.0 ) then
          assignSym(4,0,3,1, 9,5,8,6, 14,10,13,11, 19,15,18,16, 24,20,23,21 )
        else if( axis.eq.1 ) then 
          assignSym(20,0,15,5, 21,1,16,6, 22,2,17,7, 23,3,18,8, 24,4,19,9 )
        else
          stop 66
        end if
    

      end if

      return 
      end

#beginMacro beginJLoops()
 do j3=m3a,m3b
 do j2=m2a,m2b
 do j1=m1a,m1b
#endMacro
#beginMacro endJLoops()
 end do
 end do
 end do
#endMacro

#beginMacro beginIJLoops(nn1a,nn2a,nn3a, nn1b,nn2b,nn3b,   mm1a,mm2a,mm3a, mm1b,mm2b,mm3b)
 i3=nn3a
 do j3=mm3a,mm3b
   i2=nn2a
   do j2=mm2a,mm2b
     i1=nn1a
     do j1=mm1a,mm1b
#endMacro
#beginMacro endIJLoops()
       i1=i1+rf1
     end do
     i2=i2+rf2
   end do
   i3=i3+rf3
 end do
#endMacro


#beginMacro assignEndPoint(i1,i2,i3, j1,j2,j3)
  if( maskFine(i1,i2,i3).ne.0 )then
    fCoarse(j1,j2,j3)=fFine(i1,i2,i3)
  else
    fCoarse(j1,j2,j3)=0.
  end if
#endMacro
                
c
c Assign coarse grid points along a line in a given DIRection. Use injection on the end points
c
#beginMacro assignPoints1d(DIR, nn1a,nn2a,nn3a, nn1b,nn2b,nn3b,   mm1a,mm2a,mm3a, mm1b,mm2b,mm3b )
 beginIJLoops(nn1a,nn2a,nn3a, nn1b,nn2b,nn3b,   mm1a,mm2a,mm3a, mm1b,mm2b,mm3b)
   if( maskFine(i1,i2,i3).gt.0 )then
     #If #DIR eq "R" 
       fCoarse(j1,j2,j3)=averageR(i1,i2,i3)
     #Elif #DIR eq "S"
       fCoarse(j1,j2,j3)=averageS(i1,i2,i3)
     #Elif #DIR eq "T"
       fCoarse(j1,j2,j3)=averageT(i1,i2,i3)
     #Else
       stop 88
     #End
   else if( maskFine(i1,i2,i3).lt.0 )then
     fCoarse(j1,j2,j3)=fFine(i1,i2,i3)
   else
     fCoarse(j1,j2,j3)=0.
   end if
 endIJLoops()

 ! end points
 #If DIR eq "R" 
   assignEndPoint(nn1a-rf1,nn2a,nn3a, mm1a-1,mm2a,mm3a )
   assignEndPoint(nn1b+rf1,nn2b,nn3b, mm1b+1,mm2b,mm3b )
 #Elif #DIR eq "S"
   assignEndPoint(nn1a,nn2a-rf2,nn3a, mm1a,mm2a-1,mm3a )
   assignEndPoint(nn1b,nn2b+rf2,nn3b, mm1b,mm2b+1,mm3b )
 #Else
   assignEndPoint(nn1a,nn2a,nn3a-rf3, mm1a,mm2a,mm3a-1 )
   assignEndPoint(nn1b,nn2b,nn3b+rf3, mm1b,mm2b,mm3b+1 )
 #End

#endMacro

c
c Assign coarse grid points on a plane normal to a given DIRection
c
#beginMacro assignPoints2d(DIR, nn1a,nn2a,nn3a, nn1b,nn2b,nn3b,   mm1a,mm2a,mm3a, mm1b,mm2b,mm3b )

 beginIJLoops(nn1a,nn2a,nn3a, nn1b,nn2b,nn3b,   mm1a,mm2a,mm3a, mm1b,mm2b,mm3b)
   if( maskFine(i1,i2,i3).gt.0 )then
     #If #DIR eq "R" 
       fCoarse(j1,j2,j3)=averageST(i1,i2,i3)
     #Elif #DIR eq "S"
       fCoarse(j1,j2,j3)=averageRT(i1,i2,i3)
     #Elif #DIR eq "T"
       fCoarse(j1,j2,j3)=averageRS(i1,i2,i3)
     #Else
       stop 88
     #End
   else if( maskFine(i1,i2,i3).lt.0 )then
     fCoarse(j1,j2,j3)=fFine(i1,i2,i3)
   else
     fCoarse(j1,j2,j3)=0.
   end if
 endIJLoops()

#endMacro

#beginMacro getBoundaryIndex(side,axis,nnr,nn1a,nn1b,nn2a,nn2b,nn3a,nn3b, extra1,extra2,extra3)
  nn1a=nnr(0,0)-extra1
  nn1b=nnr(1,0)+extra1
  nn2a=nnr(0,1)-extra2
  nn2b=nnr(1,1)+extra2
  if( nd.gt.2 )then
    nn3a=nnr(0,2)-extra3
    nn3b=nnr(1,2)+extra3
  else
    nn3a=nnr(0,2)
    nn3b=nnr(1,2)
  end if
  if( axis.eq.0 )then
    nn1a=nnr(side,axis)
    nn1b=nnr(side,axis)
  else if( axis.eq.1 )then
    nn2a=nnr(side,axis)
    nn2b=nnr(side,axis)
  else
    nn3a=nnr(side,axis)
    nn3b=nnr(side,axis)
  end if
#endMacro

      subroutine fineToCoarseBoundaryConditions( 
     &   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b, ! for fFine
     &   md1a,md1b,md2a,md2b,md3a,md3b, ! for fCoarse
     &   maskFine, fFine, fCoarse, bc, bcd, ipar )
c =========================================================================================
c Description:
c    apply boundary conditions to fCoarse at the end of the fine to coarse transfer
c
c  bc(side,axis) : boundary "type"
c  bcd(side,axis) : boundary condition (dirichlet, neumann) if provided
c =========================================================================================

      implicit none

      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     &        md1a,md1b,md2a,md2b,md3a,md3b,
     &        ipar(0:*),bc(0:1,0:2),bcd(0:1,0:2)

      integer maskFine(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real fFine(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real fCoarse(md1a:md1b,md2a:md2b,md3a:md3b)

c........local variables

      integer nd,side,axis,i1,i2,i3,j1,j2,j3,rf1,rf2,rf3,extra,is1,is2,is3
      integer nr(0:1,0:2),mr(0:1,0:2)
      integer n1a,n2a,n3a, n1b,n2b,n3b, m1a,m2a,m3a, m1b,m2b,m3b
      integer transferForcing,bcSupplied

      integer dirichlet,neumann,mixed,equation,extrapolation,
     &        combination,equationToSecondOrder,mixedToSecondOrder,
     &        evenSymmetry,oddSymmetry,extrapolateTwoGhostLines
      parameter( 
     &     dirichlet=1,
     &     neumann=2,
     &     mixed=3,
     &     equation=4,
     &     extrapolation=5,
     &     combination=6,
     &     equationToSecondOrder=7,
     &     mixedToSecondOrder=8,
     &     evenSymmetry=9,
     &     oddSymmetry=10,
     &     extrapolateTwoGhostLines=11 )

      integer equationToSolve
      integer userDefined,laplaceEquation,divScalarGradOperator,
     &  heatEquationOperator,variableHeatEquationOperator,
     &   divScalarGradHeatOperator,secondOrderConstantCoefficients,
     & axisymmetricLaplaceEquation
      parameter(
     & userDefined=0,
     & laplaceEquation=1,
     & divScalarGradOperator=2,              ! div[ s[x] grad ]
     & heatEquationOperator=3,               ! I + c0*Delta
     & variableHeatEquationOperator=4,       ! I + s[x]*Delta
     & divScalarGradHeatOperator=5,  ! I + div[ s[x] grad ]
     & secondOrderConstantCoefficients=6,
     & axisymmetricLaplaceEquation=7 ) 

c...... start statement functions
      real averageR,averageS,averageT,averageRS,averageRT,averageST

      averageR(i1,i2,i3)=.25*(fFine(i1-1,i2,i3)+fFine(i1+1,i2,i3))+.5*fFine(i1,i2,i3)
      averageS(i1,i2,i3)=.25*(fFine(i1,i2-1,i3)+fFine(i1,i2+1,i3))+.5*fFine(i1,i2,i3)
      averageT(i1,i2,i3)=.25*(fFine(i1,i2,i3-1)+fFine(i1,i2,i3+1))+.5*fFine(i1,i2,i3)

      averageRS(i1,i2,i3)=.25*(averageS(i1-1,i2,i3)+averageS(i1+1,i2,i3))+.5*averageS(i1,i2,i3)
      averageRT(i1,i2,i3)=.25*(averageT(i1-1,i2,i3)+averageT(i1+1,i2,i3))+.5*averageT(i1,i2,i3)
      averageST(i1,i2,i3)=.25*(averageT(i1,i2-1,i3)+averageT(i1,i2+1,i3))+.5*averageT(i1,i2,i3)
c........end statement functions


      nd =ipar(0)

      nr(0,0)=ipar(1)
      nr(1,0)=ipar(2)
      nr(0,1)=ipar(3)
      nr(1,1)=ipar(4)
      nr(0,2)=ipar(5)
      nr(1,2)=ipar(6)
      
      mr(0,0)=ipar(7)
      mr(1,0)=ipar(8)
      mr(0,1)=ipar(9)
      mr(1,1)=ipar(10)
      mr(0,2)=ipar(11)
      mr(1,2)=ipar(12)

      equationToSolve=ipar(13)

      rf1    =ipar(14) ! refinement factor in direction 1
      rf2    =ipar(15)
      rf3    =ipar(16)
      transferForcing=ipar(17) ! equals 1 if we are transferring the forcing as opposed to the defect
      bcSupplied = ipar(18)

      if( rf1.ne.2 .or. rf2.ne.2 .or. (nd.eq.3 .and. rf3.ne.2) )then
        write(*,*) "ERROR: refinement factor is NOT 2"
        stop 71
      end if

      do axis=0,nd-1
      do side=0,1

        if( bc(side,axis).eq.extrapolation )then

          ! this is a dirichlet boundary

          is1=0
          is2=0
          is3=0
          if( axis.eq.0 )then
            is1=1-2*side
          else if( axis.eq.1 )then
            is2=1-2*side
          else
            is3=1-2*side
          end if
          if( bcSupplied.eq.1 .and. bcd(side,axis).ne.dirichlet )then
           write(*,*) 'fineToCoarseBoundaryConditions: This case not expected!'
           stop 1789
          end if
          
          if( (equationToSolve.ne.userDefined .or. (bcSupplied.eq.1 .and. bcd(side,axis).eq.dirichlet)) \
              .and. transferForcing.eq.0 )then

            ! this is a Dirichlet BC -- the defect is zero
            !  (Note that if transferForcing.eq.1 then we are transferring the forcing so fFine will NOT be zero on the boundary)

            extra=0
            getBoundaryIndex(side,axis,mr,m1a,m1b,m2a,m2b,m3a,m3b, extra,extra,extra)

c     write(*,'("fcBC: side,axis=",2i2," m1a,m1b,m2a,m2b,m3a,m3b=",6i5)') side,axis,m1a,m1b,m2a,m2b,m3a,m3b

            if( nd.eq.2 )then
              beginJLoops()
                fCoarse(j1,j2,j3)=0.
                ! not needed? fCoarse(j1-is1,j2-is2,j3)=0.
              endJLoops()
            else
              beginJLoops()
                fCoarse(j1,j2,j3)=0.
                ! not needed? fCoarse(j1-is1,j2-is2,j3-is3)=0.
              endJLoops()
            end if
          else

            extra=-1 ! leave off ends
            getBoundaryIndex(side,axis,nr,n1a,n1b,n2a,n2b,n3a,n3b, extra*rf1,extra*rf2,extra*rf3)
            getBoundaryIndex(side,axis,mr,m1a,m1b,m2a,m2b,m3a,m3b, extra,extra,extra)

            if( nd.eq.2 )then

              if( axis.eq.0 )then
                assignPoints1d(S, n1a,n2a,n3a, n1b,n2b,n3b,   m1a,m2a,m3a, m1b,m2b,m3b )
              else
                assignPoints1d(R, n1a,n2a,n3a, n1b,n2b,n3b,   m1a,m2a,m3a, m1b,m2b,m3b )
              end if

            else ! 3D

              ! write(*,'("fineToCoarseBC: transferForcing=",i2)') transferForcing

              if( axis.eq.0 )then
                ! assign interior points on this face
               
                assignPoints2d(R, n1a,n2a,n3a, n1b,n2b,n3b,   m1a,m2a,m3a, m1b,m2b,m3b )

                ! edges 
                n2a=n2a-rf2   ! shift to boundary
                n2b=n2b+rf2
                m2a=m2a-1
                m2b=m2b+1
                assignPoints1d(T, n1a,n2a,n3a, n1b,n2a,n3b,   m1a,m2a,m3a, m1b,m2a,m3b )
                assignPoints1d(T, n1a,n2b,n3a, n1b,n2b,n3b,   m1a,m2b,m3a, m1b,m2b,m3b )

                n2a=n2a+rf2   ! reset
                n2b=n2b-rf2
                m2a=m2a+1
                m2b=m2b-1

                n3a=n3a-rf3   ! shift to boundary
                n3b=n3b+rf3
                m3a=m3a-1
                m3b=m3b+1

                assignPoints1d(S, n1a,n2a,n3a, n1b,n2b,n3a,   m1a,m2a,m3a, m1b,m2b,m3a )
                assignPoints1d(S, n1a,n2a,n3b, n1b,n2b,n3b,   m1a,m2a,m3b, m1b,m2b,m3b )

              else if( axis.eq.1 )then
                ! assign interior points on this face
                assignPoints2d(S, n1a,n2a,n3a, n1b,n2b,n3b,   m1a,m2a,m3a, m1b,m2b,m3b )

                ! edges 
                n3a=n3a-rf3   ! shift to boundary
                n3b=n3b+rf3
                m3a=m3a-1
                m3b=m3b+1
                assignPoints1d(R, n1a,n2a,n3a, n1b,n2b,n3a,   m1a,m2a,m3a, m1b,m2b,m3a )
                assignPoints1d(R, n1a,n2a,n3b, n1b,n2b,n3b,   m1a,m2a,m3b, m1b,m2b,m3b )

                n3a=n3a+rf3   ! reset
                n3b=n3b-rf3
                m3a=m3a+1
                m3b=m3b-1

                n1a=n1a-rf1   ! shift to boundary
                n1b=n1b+rf1
                m1a=m1a-1
                m1b=m1b+1
                assignPoints1d(T, n1a,n2a,n3a, n1a,n2b,n3b,   m1a,m2a,m3a, m1a,m2b,m3b )
                assignPoints1d(T, n1b,n2a,n3a, n1b,n2b,n3b,   m1b,m2a,m3a, m1b,m2b,m3b )

              else
                ! assign interior points on this face
                assignPoints2d(T, n1a,n2a,n3a, n1b,n2b,n3b,   m1a,m2a,m3a, m1b,m2b,m3b )

                ! edges 
                n2a=n2a-rf2   ! shift to boundary
                n2b=n2b+rf2
                m2a=m2a-1
                m2b=m2b+1
                assignPoints1d(R, n1a,n2a,n3a, n1b,n2a,n3b,   m1a,m2a,m3a, m1b,m2a,m3b )
                assignPoints1d(R, n1a,n2b,n3a, n1b,n2b,n3b,   m1a,m2b,m3a, m1b,m2b,m3b )

                n2a=n2a+rf2   ! reset
                n2b=n2b-rf2
                m2a=m2a+1
                m2b=m2b-1

                n1a=n1a-rf1   ! shift to boundary
                n1b=n1b+rf1
                m1a=m1a-1
                m1b=m1b+1
                assignPoints1d(S, n1a,n2a,n3a, n1a,n2b,n3b,   m1a,m2a,m3a, m1a,m2b,m3b )
                assignPoints1d(S, n1b,n2a,n3a, n1b,n2b,n3b,   m1b,m2a,m3a, m1b,m2b,m3b )

              end if

            end if

          end if  ! end userDefined
        end if

      end do      
      end do      


      return
      end



