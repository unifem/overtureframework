
      subroutine lagrange( order, n, r, value )
! evaluate the n'th lagrange polynomial of order "order" at value r
! The nodes are at (0,1,2,...,order-1)
      integer order
      real r,value

      value=1.
      do i=0,order-1
       if( i.ne.n )then
         value=value*(r-i)/(n-i)
       end if
      end do

      return
      end   

c This macro can be used to turn on the checking of the mask
#beginMacro beginCheckForMask()
#endMacro
#beginMacro endCheckForMask()
#endMacro


c Here is the inner loop 
#beginMacro innerLoop(formula)
! do c=ca,cb
  formula
! end do
#endMacro

#beginMacro beginLoopOdd1(iw,m1m)
do i1=nra,nrb
 j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*ratio1/2)/ratio1 -iw-i1offset  
 m1=(i1-(j1+iw)*ratio1) m1m 
#endMacro
#beginMacro beginLoopOdd1WithMask(iw,m1m)
do i1=nra,nrb
 beginCheckForMask()
 j1=(i1+ratio1/2+ratio1*i1offset-i1Shift-centering*ratio1/2)/ratio1 -iw-i1offset  
 m1=(i1-(j1+iw)*ratio1) m1m 
#endMacro

#beginMacro beginLoopOdd2(iw,m2m)
do i2=nsa,nsb
 j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*ratio2/2)/ratio2 -iw-i2offset 
 m2=(i2-(j2+iw)*ratio2) m2m
#endMacro
#beginMacro beginLoopOdd2WithMask(iw,m2m)
do i2=nsa,nsb
 beginCheckForMask()
 j2=(i2+ratio2/2+ratio2*i2offset-i2Shift-centering*ratio2/2)/ratio2 -iw-i2offset 
 m2=(i2-(j2+iw)*ratio2) m2m
#endMacro

#beginMacro beginLoopOdd3(iw,m3m)
do i3=nta,ntb
 j3=(i3+ratio3/2+ratio3*i3offset-i3Shift-centering*ratio3/2)/ratio3 -iw-i3offset 
 m3=(i3-(j3+iw)*ratio3) m3m
#endMacro

#beginMacro beginLoopEven1(iw,iw2,m1m)
do i1=nra,nrb
  j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-i1offset-iw
  m1=(i1-(j1+iw)*ratio1) m1m
  r=(m1+centering*.5*(1-ratio1))/ratio1 +iw2
#endMacro
#beginMacro beginLoopEven1WithMask(iw,iw2,m1m)
do i1=nra,nrb
 beginCheckForMask()
  j1=(i1+ratio1*i1offset-centering*ratio1/2)/ratio1-i1offset-iw
  m1=(i1-(j1+iw)*ratio1) m1m
  r=(m1+centering*.5*(1-ratio1))/ratio1 +iw2
#endMacro

#beginMacro beginLoopEven2(iw,iw2,m2m)
do i2=nsa,nsb
  j2=(i2+ratio2*i2offset-centering*ratio2/2)/ratio2-i2offset-iw
  m2=(i2-(j2+iw)*ratio2) m2m
  r=(m2+centering*.5*(1-ratio2))/ratio2 +iw2
#endMacro

#beginMacro beginLoopEven3(iw,iw2,m3m)
do i3=nta,ntb
  j3=(i3+ratio3*i3offset-centering*ratio3/2)/ratio3-i3offset-iw
  m3=(i3-(j3+iw)*ratio3) m3m
  r=(m3+centering*.5*(1-ratio3))/ratio3 +iw2
#endMacro


#beginMacro interp2dWidth2RatioTwoOrFour(ND,UPDATE, formula)

if( nd.eq.2 .and. ratio1.eq.2 .and. ratio2.eq.2 )then
#If #ND eq "2"
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
 j2=(i2+i2offset*2)/2-i2offset  ! j2=i2/2
 do i1=mra,mrb,2
  beginCheckForMask()
   j1=(i1+i1offset*2)/2-i1offset ! j1=i1/2 
   UPDATE uc(j1,j2,j3,c)  ! i1,i2 even
  endCheckForMask()
 end do
end do
end do
mra=nra+mod(nra+31,2)  ! mra is odd
mrb=nrb-mod(nrb+31,2)  ! mrb is odd
do c=ca,cb
do i2=msa,msb,2
 j2=(i2+i2offset*2)/2-i2offset  ! j2=i2/2
 do i1=mra,mrb,2
  beginCheckForMask() 
   j1=(i1+i1offset*2)/2-i1offset  ! j1=i1/2 
   UPDATE .5*(uc(j1,j2,j3,c)+uc(j1+1,j2,j3,c))  ! i1 odd, i2 even
  endCheckForMask()
 end do
end do
end do
msa=nsa+mod(nsa+31,2) 
msb=nsb-mod(nsb+31,2)
mra=nra+mod(nra+32,2)
mrb=nrb-mod(nrb+32,2)
do c=ca,cb
do i2=msa,msb,2
 j2=(i2+i2offset*2)/2-i2offset ! j2=i2/2
 do i1=mra,mrb,2
  beginCheckForMask()
   j1=(i1+i1offset*2)/2-i1offset ! j1=i1/2 
   UPDATE .5*(uc(j1,j2,j3,c)+uc(j1,j2+1,j3,c))   ! i1 even i2 odd
  endCheckForMask()
 end do
end do
end do
mra=nra+mod(nra+31,2)
mrb=nrb-mod(nrb+31,2)
do c=ca,cb
do i2=msa,msb,2
 j2=(i2+i2offset*2)/2-i2offset  ! j2=i2/2
 do i1=mra,mrb,2
  beginCheckForMask()
   j1=(i1+i1offset*2)/2-i1offset ! j1=i1/2 
   UPDATE .25*(uc(j1,j2,j3,c)+uc(j1+1,j2,j3,c)+uc(j1,j2+1,j3,c)+uc(j1+1,j2+1,j3,c))
  endCheckForMask()
 end do
end do
end do

else
do c=ca,cb
do i2=nsa,nsb
 j2=(i2+i2offset*2)/2-i2offset   ! j2=(i2+20)/2-10     
 m2=(i2-j2*2)*ir2
 do i1=nra,nrb
  beginCheckForMask()
   j1=(i1+i1offset*2)/2-i1offset  ! j1=(i1+20)/2-10
   m1=(i1-j1*2)*ir1 ! 0 <= m1 <r
   innerLoop(formula)
  endCheckForMask()
 end do
end do
end do ! do c
end if
#End
else
do c=ca,cb
#If #ND != "1"
do i2=nsa,nsb
 j2=(i2+ratio2*i2offset)/ratio2-i2offset     
 m2=(i2-j2*ratio2)*ir2
#End
 do i1=nra,nrb
  beginCheckForMask()
   j1=(i1+ratio1*i1offset)/ratio1-i1offset    
   m1=(i1-j1*ratio1)*ir1 ! 0 <= m1 <r
   innerLoop(formula)
  endCheckForMask()
 end do
#If #ND != "1"
end do
#End
end do ! do c
end if
#endMacro


#beginMacro interp2dWidth3RatioTwoOrFour(ND,loopOrder,shift1,shift2,formula)
do c=ca,cb
#If #ND == 1
beginLoopOdd1WithMask(shift1,*ir1)
innerLoop(formula)
endCheckForMask()
end do
#Else
#If #loopOrder == "12"
beginLoopOdd2(shift2,*ir2)
 beginLoopOdd1WithMask(shift1,*ir1)
#Else
beginLoopOdd1(shift1,*ir1)
 beginLoopOdd2WithMask(shift2,*ir2)
#End
   innerLoop(formula)
  endCheckForMask()
 end do
end do
#End
end do ! do c
#endMacro

#beginMacro interp2dWidth5RatioTwoOrFour(ND,dir1,dir2,shift1,shift2,formula)
do c=ca,cb
#If #ND != "1"
beginLoopOdd ## dir2(shift2,)
 r=(m ## dir2 +centering*.5*(1-ratio ## dir2))/ratio ## dir2
 c5 ## dir2 0=lagrange50(r)
 c5 ## dir2 1=lagrange51(r)
 c5 ## dir2 2=lagrange52(r)
 c5 ## dir2 3=lagrange53(r)
 c5 ## dir2 4=lagrange54(r)
#End
#If #dir1 == "1"
 beginLoopOdd1WithMask(shift1,)
#Else
 beginLoopOdd2WithMask(shift1,)
#End
   r=(m ## dir1 +centering*.5*(1-ratio ## dir1 ))/ratio ## dir1 
   c5 ## dir1 0=lagrange50(r)
   c5 ## dir1 1=lagrange51(r)
   c5 ## dir1 2=lagrange52(r)
   c5 ## dir1 3=lagrange53(r)
   c5 ## dir1 4=lagrange54(r)

   innerLoop(formula)
  endCheckForMask()
 end do
#If #ND != "1"
end do
#End
end do ! do c
#endMacro

#beginMacro interp2dWidth3(ND,formula)
do c=ca,cb
#If #ND != "1"
beginLoopOdd2(1,)
 r=(m2+centering*.5*(1-ratio2))/ratio2
 c320=lagrange30(r)
 c321=lagrange31(r)
 c322=lagrange32(r)
#End
 beginLoopOdd1WithMask(1,)
   r=(m1+centering*.5*(1-ratio1))/ratio1
   c310=lagrange30(r)
   c311=lagrange31(r)
   c312=lagrange32(r)

   innerLoop(formula)

  endCheckForMask()
 end do
#If #ND != "1"
end do
#End
end do ! do c
#endMacro

c general odd width
#beginMacro interp2dWidthOdd(ND,dir1,dir2,iw,formula,UPDATE)
#If #ND == "3"
beginLoopOdd3(iw,)
 r=(m3+centering*.5*(1-ratio3))/ratio3 +iw  
 if( abs(r-iw).gt.0.5)then
   write(*,*) ' ERROR r=',r
 end if
 do i=0,width-1
   call lagrange(width,i,r,cl3(i))
 end do
#End
#If #ND == "2" || #ND == "3"
beginLoopOdd ## dir2(iw,)
 r=(m ## dir2+centering*.5*(1-ratio ## dir2))/ratio ## dir2 +iw  
c write(*,*)' i2,j2,m2,r=',i2,j2,m2,r
 if( abs(r-iw).gt.0.5)then
   write(*,*) ' ERROR r=',r
 end if
 do i=0,width-1
   call lagrange(width,i,r,cl ## dir2(i))
 end do
#End
 beginLoopOdd1WithMask(iw,)
   r=(m ## dir1+centering*.5*(1-ratio ## dir1))/ratio ## dir1 +iw  
   if( abs(r-iw).gt.0.5)then
     write(*,*) ' ERROR r=',r
   end if
   do i=0,width-1
     call lagrange(width,i,r,cl ## dir1(i))
   end do
   do c=ca,cb
     #If #UPDATE eq "uf(i1,i2,i3,c)="
       uf(i1,i2,i3,c)=0.
     #End
     #If #ND == "3"
     do k=0,width-1
     #End
     #If #ND == "2" || #ND == "3"
     do j=0,width-1
     #End
     do i=0,width-1
       formula
     end do
     #If #ND == "2" || #ND == "3"
     end do
     #End
     #If #ND == "3"
     end do
     #End     
   end do
   endCheckForMask()
 end do
#If #ND == "2" || #ND == "3"
end do
#End
#If #ND == "3"
end do
#End
#endMacro

#beginMacro interp2dWidth2(ND,iw,formula)
do c=ca,cb
#If #ND != "1"
beginLoopEven2(iw,0,)
  c220=lagrange20(r)
  c221=lagrange21(r)
#End
  beginLoopEven1WithMask(iw,0,)
    c210=lagrange20(r)
    c211=lagrange21(r)
    innerLoop(formula)

  endCheckForMask()
  end do
#If #ND != "1"
end do
#End
end do ! do c
#endMacro

#beginMacro interp2dWidth4(ND,iw,formula)
do c=ca,cb
#If #ND != "1"
beginLoopEven2(iw,0,)
c      write(*,*)' i2,j2,m2,r=',i2,j2,m2,r
  c420=lagrange40(r)
  c421=lagrange41(r)
  c422=lagrange42(r)
  c423=lagrange43(r)
#End
  beginLoopEven1WithMask(iw,0,)
    c410=lagrange40(r)
    c411=lagrange41(r)
    c412=lagrange42(r)
    c413=lagrange43(r)
    innerLoop(formula)
  endCheckForMask()
  end do
#If #ND != "1"
end do
#End
end do ! do c
#endMacro

c general case even width
#beginMacro interp2dWidthEven(ND,iw,formula,UPDATE)
#If #ND == "3"
beginLoopEven3(iw,iw,)
  if( abs(r-iw-.5).gt.0.5)then
    write(*,*) ' ERROR r=',r
  end if
  do i=0,width-1
    call lagrange(width,i,r,cl3(i))
  end do
#End
#If #ND == "2" || #ND == "3"
beginLoopEven2(iw,iw,)
  if( abs(r-iw-.5).gt.0.5)then
    write(*,*) ' ERROR r=',r
  end if
  do i=0,width-1
    call lagrange(width,i,r,cl2(i))
  end do
#End
  beginLoopEven1WithMask(iw,iw,)
    if( abs(r-iw-.5).gt.0.5)then
      write(*,*) ' ERROR r=',r
    end if
    do i=0,width-1
      call lagrange(width,i,r,cl1(i))
    end do

    do c=ca,cb
      #If #UPDATE eq "uf(i1,i2,i3,c)="
      uf(i1,i2,i3,c)=0.
      #End
      #If #ND == "3"
      do k=0,width-1
      #End
      #If #ND == "2" || #ND == "3"
      do j=0,width-1
      #End
      do i=0,width-1
        formula
      end do
      #If #ND == "2" || #ND == "3"
      end do
      #End
      #If #ND == "3"
      end do
      #End
    end do
    endCheckForMask()
  end do 
#If #ND == "2" || #ND == "3"
end do
#End
#If #ND == "3"
end do
#End
#endMacro


c choose UPDATE to be 
c        uf(i1,i2,i3,c)=
c  or
c        uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+
#beginMacro INTERP_LOOPS(ND,UPDATE)

if( width.eq.2 .and. centering.eq.vertex .and. ratioEqualsTwoOrFour )then

#If #ND == "1"
  interp2dWidth2RatioTwoOrFour(ND,UPDATE,UPDATE c2(0,m1)*uc(j1,j2  ,j3,c)+c2(1,m1)*uc(j1+1,j2  ,j3,c) )
#Elif #ND == "2" 
  interp2dWidth2RatioTwoOrFour(ND,UPDATE,UPDATE c2(0,m2)*( c2(0,m1)*uc(j1,j2  ,j3,c)+c2(1,m1)*uc(j1+1,j2  ,j3,c) )\
                                             +c2(1,m2)*( c2(0,m1)*uc(j1,j2+1,j3,c)+c2(1,m1)*uc(j1+1,j2+1,j3,c) )
#Elif #ND == "3"
 beginLoopEven3(0,0,*ir3)
  interp2dWidth2RatioTwoOrFour(ND,UPDATE,\
       UPDATE c2(0,m3)*(c2(0,m2)*( c2(0,m1)*uc(j1,j2  ,j3  ,c)+c2(1,m1)*uc(j1+1,j2  ,j3  ,c) )\
                               +c2(1,m2)*( c2(0,m1)*uc(j1,j2+1,j3  ,c)+c2(1,m1)*uc(j1+1,j2+1,j3  ,c) ))+\
                      c2(1,m3)*(c2(0,m2)*( c2(0,m1)*uc(j1,j2  ,j3+1,c)+c2(1,m1)*uc(j1+1,j2  ,j3+1,c) ) \
                               +c2(1,m2)*( c2(0,m1)*uc(j1,j2+1,j3+1,c)+c2(1,m1)*uc(j1+1,j2+1,j3+1,c) )) )
 end do
#End  


else if( width.eq.3 .and. centering.eq.vertex .and. ratioEqualsTwoOrFour )then
  ! this verion is about 50% faster than the more general case below using lagrange30 etc.
#If #ND == "1"
   interp2dWidth3RatioTwoOrFour(ND,12,1,0,\
    UPDATE c3(0,m1)*uc(j1,j2  ,j3,c)+c3(1,m1)*uc(j1+1,j2  ,j3,c)+c3(2,m1)*uc(j1+2,j2  ,j3,c))
#Elif #ND == "2" 
  if( nrb-nra.ge.nsb-nsa )then
   interp2dWidth3RatioTwoOrFour(ND,12,1,1,\
    UPDATE c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3,c)+c3(1,m1)*uc(j1+1,j2  ,j3,c)+c3(2,m1)*uc(j1+2,j2  ,j3,c))\
                  +c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))\
                  +c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))

  else
   interp2dWidth3RatioTwoOrFour(ND,21,1,1,\
    UPDATE c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3,c)+c3(1,m1)*uc(j1+1,j2  ,j3,c)+c3(2,m1)*uc(j1+2,j2  ,j3,c))\
                  +c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3,c)+c3(1,m1)*uc(j1+1,j2+1,j3,c)+c3(2,m1)*uc(j1+2,j2+1,j3,c))\
                  +c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3,c)+c3(1,m1)*uc(j1+1,j2+2,j3,c)+c3(2,m1)*uc(j1+2,j2+2,j3,c)))
  end if
#Else
  if( nrb-nra.ge.nsb-nsa )then
   beginLoopOdd3(1,*ir3)
   interp2dWidth3RatioTwoOrFour(ND,12,1,1,\
    UPDATE \
      c3(0,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3  ,c)+c3(1,m1)*uc(j1+1,j2  ,j3  ,c)+c3(2,m1)*uc(j1+2,j2  ,j3  ,c))\
               +c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3  ,c)+c3(1,m1)*uc(j1+1,j2+1,j3  ,c)+c3(2,m1)*uc(j1+2,j2+1,j3  ,c))\
               +c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3  ,c)+c3(1,m1)*uc(j1+1,j2+2,j3  ,c)+c3(2,m1)*uc(j1+2,j2+2,j3  ,c)))+ \
      c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3+1,c)+c3(1,m1)*uc(j1+1,j2  ,j3+1,c)+c3(2,m1)*uc(j1+2,j2  ,j3+1,c))\
               +c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(2,m1)*uc(j1+2,j2+1,j3+1,c))\
               +c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,c)+c3(1,m1)*uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c)))+ \
      c3(2,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3+2,c)+c3(1,m1)*uc(j1+1,j2  ,j3+2,c)+c3(2,m1)*uc(j1+2,j2  ,j3+2,c))\
               +c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(j1+2,j2+1,j3+2,c))\
               +c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,m1)*uc(j1+1,j2+2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c))) )
   end do
  else
   beginLoopOdd3(1,*ir3)
   interp2dWidth3RatioTwoOrFour(ND,21,1,1,\
    UPDATE \
      c3(0,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3  ,c)+c3(1,m1)*uc(j1+1,j2  ,j3  ,c)+c3(2,m1)*uc(j1+2,j2  ,j3  ,c))\
               +c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3  ,c)+c3(1,m1)*uc(j1+1,j2+1,j3  ,c)+c3(2,m1)*uc(j1+2,j2+1,j3  ,c))\
               +c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3  ,c)+c3(1,m1)*uc(j1+1,j2+2,j3  ,c)+c3(2,m1)*uc(j1+2,j2+2,j3  ,c)))+ \
      c3(1,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3+1,c)+c3(1,m1)*uc(j1+1,j2  ,j3+1,c)+c3(2,m1)*uc(j1+2,j2  ,j3+1,c))\
               +c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+1,c)+c3(1,m1)*uc(j1+1,j2+1,j3+1,c)+c3(2,m1)*uc(j1+2,j2+1,j3+1,c))\
               +c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+1,c)+c3(1,m1)*uc(j1+1,j2+2,j3+1,c)+c3(2,m1)*uc(j1+2,j2+2,j3+1,c)))+ \
      c3(2,m3)*(c3(0,m2)*(c3(0,m1)*uc(j1,j2  ,j3+2,c)+c3(1,m1)*uc(j1+1,j2  ,j3+2,c)+c3(2,m1)*uc(j1+2,j2  ,j3+2,c))\
               +c3(1,m2)*(c3(0,m1)*uc(j1,j2+1,j3+2,c)+c3(1,m1)*uc(j1+1,j2+1,j3+2,c)+c3(2,m1)*uc(j1+2,j2+1,j3+2,c))\
               +c3(2,m2)*(c3(0,m1)*uc(j1,j2+2,j3+2,c)+c3(1,m1)*uc(j1+1,j2+2,j3+2,c)+c3(2,m1)*uc(j1+2,j2+2,j3+2,c))) )
   end do
  end if

#End

else if( width.eq.5 )then
#If #ND == "1"
  interp2dWidth5RatioTwoOrFour(ND,1,2,2,0,UPDATE interp51(j1,j2,j3))
#Elif #ND == "2" 
  if( nrb-nra.ge.nsb-nsa )then
    interp2dWidth5RatioTwoOrFour(ND,1,2,2,2,\
      UPDATE c520*interp51(j1,j2  ,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+\
                       c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))
  else
    interp2dWidth5RatioTwoOrFour(ND,2,1,2,2,\
      UPDATE c520*interp51(j1,j2  ,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+\
                       c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))
  end if
#Else
  if( nrb-nra.ge.nsb-nsa )then
    beginLoopOdd3(2,)
     r=(m3 +centering*.5*(1-ratio3))/ratio3
     c530=lagrange50(r)
     c531=lagrange51(r)
     c532=lagrange52(r)
     c533=lagrange53(r)
     c534=lagrange54(r)

    interp2dWidth5RatioTwoOrFour(ND,1,2,2,2,\
      UPDATE c530*(c520*interp51(j1,j2  ,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+\
                           c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+\
                     c531*(c520*interp51(j1,j2  ,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*interp51(j1,j2+2,j3+1)+\
                           c523*interp51(j1,j2+3,j3+1)+c524*interp51(j1,j2+4,j3+1))+\
                     c532*(c520*interp51(j1,j2  ,j3+2)+c521*interp51(j1,j2+1,j3+2)+c522*interp51(j1,j2+2,j3+2)+\
                           c523*interp51(j1,j2+3,j3+2)+c524*interp51(j1,j2+4,j3+2))+\
                     c533*(c520*interp51(j1,j2  ,j3+3)+c521*interp51(j1,j2+1,j3+3)+c522*interp51(j1,j2+2,j3+3)+\
                           c523*interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+4,j3+3))+\
                     c534*(c520*interp51(j1,j2  ,j3+4)+c521*interp51(j1,j2+1,j3+4)+c522*interp51(j1,j2+2,j3+4)+\
                           c523*interp51(j1,j2+3,j3+4)+c524*interp51(j1,j2+4,j3+4)) )
    end do
  else
    beginLoopOdd3(2,)
     r=(m3 +centering*.5*(1-ratio3))/ratio3
     c530=lagrange50(r)
     c531=lagrange51(r)
     c532=lagrange52(r)
     c533=lagrange53(r)
     c534=lagrange54(r)

    interp2dWidth5RatioTwoOrFour(ND,2,1,2,2,\
      UPDATE c530*(c520*interp51(j1,j2  ,j3)+c521*interp51(j1,j2+1,j3)+c522*interp51(j1,j2+2,j3)+\
                           c523*interp51(j1,j2+3,j3)+c524*interp51(j1,j2+4,j3))+\
                     c531*(c520*interp51(j1,j2  ,j3+1)+c521*interp51(j1,j2+1,j3+1)+c522*interp51(j1,j2+2,j3+1)+\
                           c523*interp51(j1,j2+3,j3+1)+c524*interp51(j1,j2+4,j3+1))+\
                     c532*(c520*interp51(j1,j2  ,j3+2)+c521*interp51(j1,j2+1,j3+2)+c522*interp51(j1,j2+2,j3+2)+\
                           c523*interp51(j1,j2+3,j3+2)+c524*interp51(j1,j2+4,j3+2))+\
                     c533*(c520*interp51(j1,j2  ,j3+3)+c521*interp51(j1,j2+1,j3+3)+c522*interp51(j1,j2+2,j3+3)+\
                           c523*interp51(j1,j2+3,j3+3)+c524*interp51(j1,j2+4,j3+3))+\
                     c534*(c520*interp51(j1,j2  ,j3+4)+c521*interp51(j1,j2+1,j3+4)+c522*interp51(j1,j2+2,j3+4)+\
                           c523*interp51(j1,j2+3,j3+4)+c524*interp51(j1,j2+4,j3+4)) )
    end do
  end if
#End

else if( width.eq.2 )then
c   width=2 but ratio is not 2 or 4 or centering.eq.cell
#If #ND == "1"
  interp2dWidth2(ND,0,UPDATE interp21(j1,j2  ,j3))
#Elif #ND == "2" 
  interp2dWidth2(ND,0,UPDATE c220*interp21(j1,j2  ,j3)+c221*interp21(j1,j2+1,j3))
#Elif #ND == "3" 
  beginLoopEven3(0,0,)
  c230=lagrange20(r)
  c231=lagrange21(r)
  interp2dWidth2(ND,0,UPDATE  \
           c230*(c220*interp21(j1,j2  ,j3  )+c221*interp21(j1,j2+1,j3))+ \
           c231*(c220*interp21(j1,j2  ,j3+1)+c221*interp21(j1,j2+1,j3+1)) )
  end do
#End
else if( width.eq.3 )then
c   width=3 but ratio is not 2 or 4
#If #ND == "1"
  interp2dWidth3(ND,UPDATE interp31(j1,j2,j3))
#Elif #ND == "2" 
  interp2dWidth3(ND,UPDATE c320*interp31(j1,j2,j3)+c321*interp31(j1,j2+1,j3)+c322*interp31(j1,j2+2,j3))
#Else
 beginLoopOdd3(1,)
  r=(m3+centering*.5*(1-ratio3))/ratio3
  c330=lagrange30(r)
  c331=lagrange31(r)
  c332=lagrange32(r)
  interp2dWidth3(ND,UPDATE \
               c330*(c320*interp31(j1,j2,j3  )+c321*interp31(j1,j2+1,j3  )+c322*interp31(j1,j2+2,j3  ))+\
               c331*(c320*interp31(j1,j2,j3+1)+c321*interp31(j1,j2+1,j3+1)+c322*interp31(j1,j2+2,j3+1))+\
               c332*(c320*interp31(j1,j2,j3+2)+c321*interp31(j1,j2+1,j3+2)+c322*interp31(j1,j2+2,j3+2)) )
 end do
#End
else if( width.eq.1 )then
c   width=1 but ratio is not 2 or 4
do c=ca,cb
#If #ND == "1"
do i1=nra,nrb
 j1=.5+ (i1+ratio1/2+ratio1*i1offset-i1Shift-centering*ratio1/2)/ratio1 -i1offset
   beginCheckForMask()
   innerLoop(UPDATE uc(j1,j2,j3,c))
  endCheckForMask()
  end do
#Elif #ND == "2"
do i2=nsa,nsb
 j2=.5+ (i2+ratio2/2+ratio2*i2offset-i2Shift-centering*ratio2/2)/ratio2 -i2offset 
c write(*,*) ' width=1: i2,j2,ratio2=',i2,j2,ratio2
do i1=nra,nrb
 j1=.5+ (i1+ratio1/2+ratio1*i1offset-i1Shift-centering*ratio1/2)/ratio1 -i1offset  
   beginCheckForMask()
   innerLoop(UPDATE uc(j1,j2,j3,c))
   endCheckForMask()
  end do
  end do
#Else
do i3=nta,ntb
 j3=.5+ (i3+ratio3/2+ratio3*i3offset-i3Shift-centering*ratio3/2)/ratio3 -i3offset 
do i2=nsa,nsb
 j2=.5+ (i2+ratio2/2+ratio2*i2offset-i2Shift-centering*ratio2/2)/ratio2 -i2offset 
do i1=nra,nrb
 j1=.5+ (i1+ratio1/2+ratio1*i1offset-i1Shift-centering*ratio1/2)/ratio1 -i1offset  
   beginCheckForMask()
    innerLoop(UPDATE uc(j1,j2,j3,c))
   endCheckForMask()
  end do
  end do
  end do
#End
end do ! do c
else if( width.eq.4 )then

#If #ND == "1"
  interp2dWidth4(ND,1,UPDATE interp41(j1,j2,j3))
#Elif #ND == "2"
  interp2dWidth4(ND,1,\
      UPDATE c420*interp41(j1,j2  ,j3)+c421*interp41(j1,j2+1,j3)+c422*interp41(j1,j2+2,j3)+\
                     c423*interp41(j1,j2+3,j3))
#Else
beginLoopEven3(1,0,)
  c430=lagrange40(r)
  c431=lagrange41(r)
  c432=lagrange42(r)
  c433=lagrange43(r)
  interp2dWidth4(ND,1, UPDATE \
    c430*(c420*interp41(j1,j2,j3  )+c421*interp41(j1,j2+1,j3  )+c422*interp41(j1,j2+2,j3  )+c423*interp41(j1,j2+3,j3  ))+\
    c431*(c420*interp41(j1,j2,j3+1)+c421*interp41(j1,j2+1,j3+1)+c422*interp41(j1,j2+2,j3+1)+c423*interp41(j1,j2+3,j3+1))+\
    c432*(c420*interp41(j1,j2,j3+2)+c421*interp41(j1,j2+1,j3+2)+c422*interp41(j1,j2+2,j3+2)+c423*interp41(j1,j2+3,j3+2))+\
    c433*(c420*interp41(j1,j2,j3+3)+c421*interp41(j1,j2+1,j3+3)+c422*interp41(j1,j2+2,j3+3)+c423*interp41(j1,j2+3,j3+3)) )
  end do
#End

else if( mod(width,2).eq.0 )then
c    general case, even width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width

  iw=(width-1)/2
#If #ND == "1"
  interp2dWidthEven(ND,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl1(i)*uc(j1+i,j2,j3,c),UPDATE)
#Elif #ND == "2"
  interp2dWidthEven(ND,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl2(j)*cl1(i)*uc(j1+i,j2+j,j3,c),UPDATE)
#Else
  interp2dWidthEven(ND,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl3(k)*cl2(j)*cl1(i)*uc(j1+i,j2+j,j3+k,c),UPDATE)
#End

else if( mod(width,2).eq.1 )then
c    general case, odd width
c    write(*,*) 'interpFineFromCoarse general formula: width=',width
iw=(width-1)/2
#If #ND == "1"
  interp2dWidthOdd(ND,1,2,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl1(i)*uc(j1+i,j2+j,j3,c),UPDATE)
#Elif #ND == "2"
  interp2dWidthOdd(ND,1,2,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl2(j)*cl1(i)*uc(j1+i,j2+j,j3,c),UPDATE)
#Else
  interp2dWidthOdd(ND,1,2,iw,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+cl3(k)*cl2(j)*cl1(i)*uc(j1+i,j2+j,j3+k,c),UPDATE)
#End

else
 write(*,*) 'interpFineFromCoarse:ERROR: interp width=',width,' not implemeted'
  ! '
end if
#endMacro



      subroutine interpFineFromCoarse( ndfra,ndfrb,ndfsa,ndfsb,ndfta,ndftb,uf,\
                                 ndcra,ndcrb,ndcsa,ndcsb,ndcta,ndctb,uc,\
                                 nd,nra,nrb,nsa,nsb,nta,ntb, \
                                 width,ratios, ndca,ca,cb, ishift, centerings, update, mask, ipar )
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
      ! implicit none
      integer ndfra,ndfrb,ndfsa,ndfsb,ndfta,ndftb,ndca
      integer ndcra,ndcrb,ndcsa,ndcsb,ndcta,ndctb
      real uf(ndfra:ndfrb,ndfsa:ndfsb,ndfta:ndftb,ndca:*)
      real uc(ndcra:ndcrb,ndcsa:ndcsb,ndcta:ndctb,ndca:*)
      integer ca,cb,width,centerings(*)
      integer ratios(*),ishift(*),update
      integer mask(ndfra:ndfrb,ndfsa:ndfsb,ndfta:ndftb)
      integer ipar(0:*)

      integer i1,i2,i3,j1,j2,j3,c,ratio,ratio1,ratio2,ratio3
      integer centering,centering1,centering2,centering3
      integer maxWidth

c these coefficients are set up for a ratio of 4
      real r
      real c2(0:1,0:3)
      real c3(0:2,-2:2)
      real lagrange20,lagrange21
      real lagrange30,lagrange31,lagrange32
      real lagrange40,lagrange41,lagrange42,lagrange43
      real lagrange50,lagrange51,lagrange52,lagrange53,lagrange54
      real interp21,interp31,interp41,interp51,interp52
      save c2,c3

      real c210,c211,c310,c311,c312
      real c410,c411,c412,c413
      real c510,c511,c512,c513,c514,c520,c521,c522,c523,c524
      integer ir1,ir2,ir3,nd,ir,i1shift,i2shift,i3shift,i1offset
      real c38,c18,c5b32,c1516,c3b32
      parameter( c38=3./8., c18=-1./8.,c5b32=5./32., c1516=15./16., c3b32=-3./32. )
      logical ratioEqualsTwoOrFour
      integer vertex,cell
      parameter( vertex=0, cell=1 )
      parameter( maxWidth=10 )  ! maximum interpolation width -- just increase this value to do larger widths
      real cl1(0:maxWidth-1),cl2(0:maxWidth-1),cl3(0:maxWidth-1)

      integer maskOption
      integer doNotUseMask, maskGreaterThanZero,maskEqualZero
      parameter( doNotUseMask=0, maskGreaterThanZero=1, maskEqualZero=2 )

c.. begin statement functions
      lagrange20(r)=1.-r
      lagrange21(r)=r      

      interp21(j1,j2,j3)=c210*uc(j1  ,j2,j3,c)+c211*uc(j1+1,j2,j3,c)

      lagrange30(r)=       r*(r-1.)/2.
      lagrange31(r)=(r+1.)  *(r-1.)/(-1.)
      lagrange32(r)=(r+1.)*r       /2.

      interp31(j1,j2,j3)=c310*uc(j1  ,j2,j3,c)+c311*uc(j1+1,j2,j3,c)+c312*uc(j1+2,j2,j3,c)

      lagrange40(r)=       r*(r-1.)*(r-2.)/(-6.)
      lagrange41(r)=(r+1.)  *(r-1.)*(r-2.)/2.
      lagrange42(r)=(r+1.)*r       *(r-2.)/(-2.)
      lagrange43(r)=(r+1.)*r*(r-1.)       /(6.)

      interp41(j1,j2,j3)=c410*uc(j1  ,j2,j3,c)+c411*uc(j1+1,j2,j3,c)+c412*uc(j1+2,j2,j3,c)+ \
                         c413*uc(j1+3,j2,j3,c)
c  lagrange polynomials, order 5 on [-2,-1,0,1,2] 
      lagrange50(r)=       (r+1.)*r*(r-1.)*(r-2.)/24.
      lagrange51(r)=(r+2.)       *r*(r-1.)*(r-2.)/(-6.)
      lagrange52(r)=(r+2.)*(r+1.)  *(r-1.)*(r-2.)/4.
      lagrange53(r)=(r+2.)*(r+1.)*r       *(r-2.)/(-6.)
      lagrange54(r)=(r+2.)*(r+1.)*r*(r-1.)       /24.

      interp51(j1,j2,j3)=c510*uc(j1  ,j2,j3,c)+c511*uc(j1+1,j2,j3,c)+c512*uc(j1+2,j2,j3,c)+ \
                         c513*uc(j1+3,j2,j3,c)+c514*uc(j1+4,j2,j3,c)
      interp52(j1,j2,j3)=c520*uc(j1,j2  ,j3,c)+c521*uc(j1,j2+1,j3,c)+c522*uc(j1,j2+2,j3,c)+ \
                         c523*uc(j1,j2+3,j3,c)+c524*uc(j1,j2+4,j3,c)

c.. end statement functions


      data c2/1.,0., .75,.25, .5,.5, .25,.75/
      data c3/c38,.75,c18, c5b32,c1516,c3b32, 0.,1.,0., c3b32,c1516,c5b32, c18,.75,c38/


      if( width.gt.maxWidth )then
        write(*,*) 'interpFineFromCoarse:ERROR:width=',width,' too large'
         ! '
      end if

      maskOption=ipar(0)

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
        ratioEqualsTwoOrFour=(ratio1.eq.2 .or. ratio1.eq.4).and.(ratio2.eq.2 .or. ratio2.eq.4)
      else
        ratioEqualsTwoOrFour=(ratio1.eq.2 .or. ratio1.eq.4).and.(ratio2.eq.2 .or. ratio2.eq.4)\
                        .and.(ratio3.eq.2 .or. ratio3.eq.4)
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

#beginMacro InterpolateLoopsMacro()
 if( update.eq.0 )then
 !    Here we set uf(i1,i2,i3,c)=interpolant(uc)
   if( nd.eq.2 )then
    INTERP_LOOPS(2,uf(i1,i2,i3,c)=)
   else if( nd.eq.3 )then
     INTERP_LOOPS(3,uf(i1,i2,i3,c)=)
   else if( nd.eq.1 )then
     INTERP_LOOPS(1,uf(i1,i2,i3,c)=)
   else
     write(*,*) 'interpFineFromCoarse:ERROR:nd=',nd
     stop 1
   end if
 else
 !  Here we set uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+interpolant(uc)
   if( nd.eq.2 )then
    INTERP_LOOPS(2,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+)
   else if( nd.eq.3 )then
     INTERP_LOOPS(3,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+)
   else if( nd.eq.1 )then
     INTERP_LOOPS(1,uf(i1,i2,i3,c)=uf(i1,i2,i3,c)+)
   else
     write(*,*) 'interpFineFromCoarse:ERROR:nd=',nd
     stop 1
   end if
 end if
#endMacro

      if( maskOption.eq.doNotUseMask )then

        InterpolateLoopsMacro()

      else if( maskOption.eq.maskGreaterThanZero )then

        ! **** redefine the macro to set the mask ****
#beginMacro beginCheckForMask()
if( mask(i1,i2,i3).gt.0 )then
#endMacro
#beginMacro endCheckForMask()
endif
#endMacro

        InterpolateLoopsMacro()

      else if( maskOption.eq.maskEqualZero )then

        ! **** redefine the macro to set the mask ****
#beginMacro beginCheckForMask()
if( mask(i1,i2,i3).eq.0 )then
#endMacro
#beginMacro endCheckForMask()
endif
#endMacro

        InterpolateLoopsMacro()

      else
         stop 6241
      end if

      return
      end

c   Here is where we call the main macro

c#beginFile interpFineFromCoarseNoMask.f
c          INTERP()
c#endFile

c #beginMacro beginCheckForMask()
c if( mask(i1,i2,i3).gt.0 )then
c #endMacro
c #beginMacro endCheckForMask()
c endif
c #endMacro
c 
c c     Here is the version that checks the mask    
c #beginFile interpFineFromCoarseMask.f
c       INTERP(WithMask)
c #endFile
