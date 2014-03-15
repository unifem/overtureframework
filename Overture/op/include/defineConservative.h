c Define macros for conservative approximations.
c These are used by the forward and inverse operators
c   included in files: dsg.bf, dsgc4.bf, dsgc6.bf and opcoeff.bf

#beginMacro DXSDY21(X,Y,SJAC)
do j3=m3a,m3b
  do j2=m2a,m2b
    do j1=m1a,m1b
      sj = SJAC
      a11(j1,j2,j3) = (r X (j1,j2,j3)*r Y (j1,j2,j3))*sj
    end do
  end do
end do
#endMacro

c get coefficients for 1D
#beginMacro GETA21( SJAC, AVE, FACTOR )
if( derivOption.eq.laplace )then
  do j3=m3a,m3b
    do j2=m2a,m2b
      do j1=m1a,m1b
        sj = jac(j1,j2,j3)
        a11(j1,j2,j3) = (rx(j1,j2,j3)**2)*sj
      end do
    end do
  end do
else if( derivOption.eq.divScalarGrad .or. derivOption .eq. divTensorGrad )then
  do j3=m3a,m3b
    do j2=m2a,m2b
      do j1=m1a,m1b
        sj = SJAC
        a11(j1,j2,j3) = (rx(j1,j2,j3)**2)*sj
      end do
    end do
  end do
else if( derivOption.eq.derivativeScalarDerivative )then
  if(      dir1.eq.0 .and. dir2.eq.0 )then
    DXSDY21(x,x,SJAC)
  else
    write(*,*) 'ERROR invalid values: dir1=',dir1,' dir2=',dir2
  end if
end if       
m1a=n1a
do j3=m3a,m3b
  do j2=m2a,m2b
    do j1=m1b,m1a,-1 ! go backwards ** worry about division by zero
AVE     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
      a11(j1,j2,j3) = FACTOR *d22(1)*(a11(j1,j2,j3)+a11(j1-1,j2,j3))
    end do
  end do
end do
m1a=n1a-1
#endMacro

#beginMacro defineA21()
m1a=n1a-1
m1b=n1b+1
m2a=n2a
m2b=n2b
m3a=n3a
m3b=n3b

if( averagingType .eq. arithmeticAverage )then
  factor=.5
  GETA21(s(j1,j2,j3,0)*jac(j1,j2,j3),c,factor)
else    
c       Harmonic average
factor=2.
c       do not average in s:  
GETA21(jac(j1,j2,j3), ,sh)  

end if    
#endMacro


c --------------------------------------------------------------------------------------------

c This macro defines Da(sDb) where a=x,y and b=x,y
#beginMacro DXSDY22(X,Y,SJAC)
do j3=m3a,m3b
  do j2=m2a,m2b
    do j1=m1a,m1b
      sj = SJAC
      a11(j1,j2,j3) = (r X (j1,j2,j3)*r Y (j1,j2,j3))*sj
      a12(j1,j2,j3) = (r X (j1,j2,j3)*s Y (j1,j2,j3))*sj
      a22(j1,j2,j3) = (s X (j1,j2,j3)*s Y (j1,j2,j3))*sj
#If #X == #Y
      a21(j1,j2,j3) = a12(j1,j2,j3)
#Else
      a21(j1,j2,j3) = (s X (j1,j2,j3)*r Y (j1,j2,j3))*sj
#End
    end do
  end do
end do
#endMacro

c =======================================================================
c  Get coefficients for 2D
c =======================================================================
#beginMacro GETA22( SJAC, AVE, FACTOR )
if( derivOption.eq.laplace )then
  do j3=m3a,m3b
    do j2=m2a,m2b
      do j1=m1a,m1b
        sj = jac(j1,j2,j3)
        a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2)*sj
        a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(j1,j2,j3)*sy(j1,j2,j3))*sj 
        a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2)*sj 
        a21(j1,j2,j3) = a12(j1,j2,j3)
      end do
    end do
  end do
else if( derivOption.eq.divScalarGrad )then
  do j3=m3a,m3b
    do j2=m2a,m2b
      do j1=m1a,m1b
        sj = SJAC
        a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2)*sj
        a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(j1,j2,j3)*sy(j1,j2,j3))*sj 
        a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2)*sj 
        a21(j1,j2,j3) = a12(j1,j2,j3)
      end do
    end do
  end do
else if( derivOption.eq.divTensorGrad )then
  do j3=m3a,m3b
    do j2=m2a,m2b
      do j1=m1a,m1b
        sj =jac(j1,j2,j3)
        s11=s(j1,j2,j3,0)
        s21=s(j1,j2,j3,1)
        s12=s(j1,j2,j3,2)
        s22=s(j1,j2,j3,3)
        a11(j1,j2,j3) = (s11*rx(j1,j2,j3)**2+(s12+s21)*rx(j1,j2,j3)*ry(j1,j2,j3)+s22*ry(j1,j2,j3)**2)*sj
        a12(j1,j2,j3) = (s11*rx(j1,j2,j3)*sx(j1,j2,j3)+s12*rx(j1,j2,j3)*sy(j1,j2,j3)+s21*ry(j1,j2,j3)*sx(j1,j2,j3)+\
                         s22*ry(j1,j2,j3)*sy(j1,j2,j3))*sj 
        a22(j1,j2,j3) = (s11*sx(j1,j2,j3)**2+(s12+s21)*sx(j1,j2,j3)*sy(j1,j2,j3)+s22*sy(j1,j2,j3)**2)*sj 
        a21(j1,j2,j3) = (s11*sx(j1,j2,j3)*rx(j1,j2,j3)+s12*sx(j1,j2,j3)*ry(j1,j2,j3)+s21*sy(j1,j2,j3)*rx(j1,j2,j3)+\
                         s22*sy(j1,j2,j3)*ry(j1,j2,j3))*sj
      end do
    end do
  end do
else if( derivOption.eq.derivativeScalarDerivative )then
  if(      dir1.eq.0 .and. dir2.eq.0 )then
    DXSDY22(x,x,SJAC)
  else if( dir1.eq.0 .and. dir2.eq.1 )then
    DXSDY22(x,y,SJAC)
  else if( dir1.eq.1 .and. dir2.eq.0 )then
    DXSDY22(y,x,SJAC)
  else if( dir1.eq.1 .and. dir2.eq.1 )then
    DXSDY22(y,y,SJAC)
  else
    write(*,*) 'ERROR invalid values: dir1=',dir1,' dir2=',dir2
  end if
else
write(*,*) 'ERROR: unknown value for derivOption=',derivOption
end if       
if( derivType.eq.symmetric )then
! symmetric case -- do not average a12 and a21 -- could do better
 m1a=n1a
 do j3=m3a,m3b
  do j2=m2a,m2b
    do j1=m1b,m1a,-1 ! go backwards   worry about division by zero
AVE     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
      a11(j1,j2,j3) = FACTOR *d22(1)*(a11(j1,j2,j3)+a11(j1-1,j2,j3))
    end do
  end do
 end do
 m1a=n1a-1
! ***** for now do not average in this case
 do j3=m3a,m3b
  do j2=m2a,m2b
    do j1=m1b,m1a,-1 
#If #AVE eq "c"
      a12(j1,j2,j3) =         (d12(1)*d12(2))*a12(j1,j2,j3)
#Else
      a12(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(2))*a12(j1,j2,j3) 
#End
    end do
  end do
 end do
 m2a=n2a
 do j3=m3a,m3b
  do j2=m2b,m2a,-1
    do j1=m1a,m1b
AVE     sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)) 
      a22(j1,j2,j3) = FACTOR *d22(2)*(a22(j1,j2,j3)+a22(j1,j2-1,j3))
    end do
  end do
 end do
 m2a=n2a-1
 do j3=m3a,m3b
  do j2=m2b,m2a,-1
    do j1=m1a,m1b
#If #AVE eq "c"
      a21(j1,j2,j3) =         (d12(1)*d12(2))*a21(j1,j2,j3)
#Else
      a21(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(2))*a21(j1,j2,j3)
#End
    end do
  end do
 end do
else
 m1a=n1a
 do j3=m3a,m3b
  do j2=m2a,m2b
    do j1=m1b,m1a,-1 ! go backwards   worry about division by zero
AVE     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
      a11(j1,j2,j3) = FACTOR *d22(1)*(a11(j1,j2,j3)+a11(j1-1,j2,j3))
      a12(j1,j2,j3) = FACTOR *(d12(1)*d12(2))*(a12(j1,j2,j3)+a12(j1-1,j2,j3))
    end do
  end do
 end do
 m1a=n1a-1
 m2a=n2a
 do j3=m3a,m3b
  do j2=m2b,m2a,-1
    do j1=m1a,m1b
AVE     sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)) 
      a21(j1,j2,j3) = FACTOR *(d12(1)*d12(2))*(a21(j1,j2,j3)+a21(j1,j2-1,j3))
      a22(j1,j2,j3) = FACTOR *d22(2)*(a22(j1,j2,j3)+a22(j1,j2-1,j3))
    end do
  end do
 end do
 m2a=n2a-1
end if


#endMacro

c 
#beginMacro defineA22()
m1a=n1a-1
m1b=n1b+1
m2a=n2a-1
m2b=n2b+1
m3a=n3a
m3b=n3b

if( averagingType .eq. arithmeticAverage .or. derivOption.eq.laplace )then
  factor=.5
  GETA22(s(j1,j2,j3,0)*jac(j1,j2,j3),c,factor)
else    
c Harmonic average
c  factor=2.
c do not average in s:  
   GETA22(jac(j1,j2,j3), ,sh)  
end if    
#endMacro


c --------------------------------------------------------------------------------------------

#beginMacro DXSDY23(X,Y,SJAC)
do j3=m3a,m3b
  do j2=m2a,m2b
    do j1=m1a,m1b
      sj = SJAC
      a11(j1,j2,j3) = (r X (j1,j2,j3)*r Y (j1,j2,j3))*sj
      a12(j1,j2,j3) = (r X (j1,j2,j3)*s Y (j1,j2,j3))*sj
      a13(j1,j2,j3) = (r X (j1,j2,j3)*t Y (j1,j2,j3))*sj
      a22(j1,j2,j3) = (s X (j1,j2,j3)*s Y (j1,j2,j3))*sj
      a23(j1,j2,j3) = (s X (j1,j2,j3)*t Y (j1,j2,j3))*sj
      a33(j1,j2,j3) = (t X (j1,j2,j3)*t Y (j1,j2,j3))*sj
#If #X == #Y
      a21(j1,j2,j3) = a12(j1,j2,j3)
      a31(j1,j2,j3) = a13(j1,j2,j3)
      a32(j1,j2,j3) = a23(j1,j2,j3)
#Else
      a21(j1,j2,j3) = (s X (j1,j2,j3)*r Y (j1,j2,j3))*sj
      a31(j1,j2,j3) = (t X (j1,j2,j3)*r Y (j1,j2,j3))*sj
      a32(j1,j2,j3) = (t X (j1,j2,j3)*s Y (j1,j2,j3))*sj
#End
    end do
  end do
end do
#endMacro

c 
c =======================================================================
c  Get coefficients for 3D
c =======================================================================
#beginMacro GETA23( SJAC, AVE, FACTOR )
if( derivOption.eq.laplace )then
  do j3=m3a,m3b
    do j2=m2a,m2b
      do j1=m1a,m1b
        sj = jac(j1,j2,j3)
        a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2+rz(j1,j2,j3)**2)*sj
        a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(j1,j2,j3)*sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3))*sj 
        a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+ry(j1,j2,j3)*ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3))*sj 
        a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2+sz(j1,j2,j3)**2)*sj 
        a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+sy(j1,j2,j3)*ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3))*sj 
        a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**2+tz(j1,j2,j3)**2)*sj 
        a21(j1,j2,j3) = a12(j1,j2,j3)
        a31(j1,j2,j3) = a13(j1,j2,j3)
        a32(j1,j2,j3) = a23(j1,j2,j3)
      end do
    end do
  end do
else if( derivOption.eq.divScalarGrad )then
  do j3=m3a,m3b
    do j2=m2a,m2b
      do j1=m1a,m1b
        sj = SJAC
        a11(j1,j2,j3) = (rx(j1,j2,j3)**2+ry(j1,j2,j3)**2+rz(j1,j2,j3)**2)*sj
        a12(j1,j2,j3) = (rx(j1,j2,j3)*sx(j1,j2,j3)+ry(j1,j2,j3)*sy(j1,j2,j3)+rz(j1,j2,j3)*sz(j1,j2,j3))*sj 
        a13(j1,j2,j3) = (rx(j1,j2,j3)*tx(j1,j2,j3)+ry(j1,j2,j3)*ty(j1,j2,j3)+rz(j1,j2,j3)*tz(j1,j2,j3))*sj 
        a22(j1,j2,j3) = (sx(j1,j2,j3)**2+sy(j1,j2,j3)**2+sz(j1,j2,j3)**2)*sj 
        a23(j1,j2,j3) = (sx(j1,j2,j3)*tx(j1,j2,j3)+sy(j1,j2,j3)*ty(j1,j2,j3)+sz(j1,j2,j3)*tz(j1,j2,j3))*sj 
        a33(j1,j2,j3) = (tx(j1,j2,j3)**2+ty(j1,j2,j3)**2+tz(j1,j2,j3)**2)*sj 
        a21(j1,j2,j3) = a12(j1,j2,j3)
        a31(j1,j2,j3) = a13(j1,j2,j3)
        a32(j1,j2,j3) = a23(j1,j2,j3)
      end do
    end do
  end do
else if( derivOption.eq.divTensorGrad )then
  do j3=m3a,m3b
    do j2=m2a,m2b
      do j1=m1a,m1b
        sj =jac(j1,j2,j3)
        s11=s(j1,j2,j3,0)
        s21=s(j1,j2,j3,1)
        s31=s(j1,j2,j3,2)
        s12=s(j1,j2,j3,3)
        s22=s(j1,j2,j3,4)
        s32=s(j1,j2,j3,5)
        s13=s(j1,j2,j3,6)
        s23=s(j1,j2,j3,7)
        s33=s(j1,j2,j3,8)
        rxj=rx(j1,j2,j3)
        ryj=ry(j1,j2,j3)
        rzj=rz(j1,j2,j3)
        sxj=sx(j1,j2,j3)
        syj=sy(j1,j2,j3)
        szj=sz(j1,j2,j3)
        txj=tx(j1,j2,j3)
        tyj=ty(j1,j2,j3)
        tzj=tz(j1,j2,j3)
        a11(j1,j2,j3) = (s11*rxj**2+s22*ryj**2+s33*rzj**2+(s12+s21)*rxj*ryj+(s13+s31)*rxj*rzj+(s23+s32)*ryj*rzj)*sj
        a22(j1,j2,j3) = (s11*sxj**2+s22*syj**2+s33*szj**2+(s12+s21)*sxj*syj+(s13+s31)*sxj*szj+(s23+s32)*syj*szj)*sj
        a33(j1,j2,j3) = (s11*txj**2+s22*tyj**2+s33*tzj**2+(s12+s21)*txj*tyj+(s13+s31)*txj*tzj+(s23+s32)*tyj*tzj)*sj
        a12(j1,j2,j3) = (s11*rxj*sxj+s22*ryj*syj+s33*rzj*szj+s12*rxj*syj+s13*rxj*szj+s21*ryj*sxj+s23*ryj*szj+\
                         s31*rzj*sxj+s32*rzj*syj)*sj
        a13(j1,j2,j3) = (s11*rxj*txj+s22*ryj*tyj+s33*rzj*tzj+s12*rxj*tyj+s13*rxj*tzj+s21*ryj*txj+s23*ryj*tzj+\
                         s31*rzj*txj+s32*rzj*tyj)*sj 
        a23(j1,j2,j3) = (s11*sxj*txj+s22*syj*tyj+s33*szj*tzj+s12*sxj*tyj+s13*sxj*tzj+s21*syj*txj+s23*syj*tzj+\
                         s31*szj*txj+s32*szj*tyj)*sj  
        a21(j1,j2,j3) = (s11*sxj*rxj+s22*syj*ryj+s33*szj*rzj+s12*sxj*ryj+s13*sxj*rzj+s21*syj*rxj+s23*syj*rzj+\
                         s31*szj*rxj+s32*szj*ryj)*sj 
        a31(j1,j2,j3) = (s11*txj*rxj+s22*tyj*ryj+s33*tzj*rzj+s12*txj*ryj+s13*txj*rzj+s21*tyj*rxj+s23*tyj*rzj+\
                         s31*tzj*rxj+s32*tzj*ryj)*sj 
        a32(j1,j2,j3) = (s11*txj*sxj+s22*tyj*syj+s33*tzj*szj+s12*txj*syj+s13*txj*szj+s21*tyj*sxj+s23*tyj*szj+\
                         s31*tzj*sxj+s32*tzj*syj)*sj 
      end do
    end do
  end do
else if( derivOption.eq.derivativeScalarDerivative )then
  if(      dir1.eq.0 .and. dir2.eq.0 )then
    DXSDY23(x,x,SJAC)
  else if( dir1.eq.0 .and. dir2.eq.1 )then
    DXSDY23(x,y,SJAC)
  else if( dir1.eq.0 .and. dir2.eq.2 )then
    DXSDY23(x,z,SJAC)
  else if( dir1.eq.1 .and. dir2.eq.0 )then
    DXSDY23(y,x,SJAC)
  else if( dir1.eq.1 .and. dir2.eq.1 )then
    DXSDY23(y,y,SJAC)
  else if( dir1.eq.1 .and. dir2.eq.2 )then
    DXSDY23(y,z,SJAC)
  else if( dir1.eq.2 .and. dir2.eq.0 )then
    DXSDY23(z,x,SJAC)
  else if( dir1.eq.2 .and. dir2.eq.1 )then
    DXSDY23(z,y,SJAC)
  else if( dir1.eq.2 .and. dir2.eq.2 )then
    DXSDY23(z,z,SJAC)
  else
    write(*,*) 'ERROR invalid values: dir1=',dir1,' dir2=',dir2
  end if
end if       
if( derivType.eq.symmetric )then
! symmetric case -- do not average a12, a21, a13, z31, etc.  -- could do better
 m1a=n1a
 do j3=m3a,m3b
  do j2=m2a,m2b
    do j1=m1b,m1a,-1 ! go backwards   worry about division by zero
AVE     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0))
      a11(j1,j2,j3) = FACTOR *d22(1)*(a11(j1,j2,j3)+a11(j1-1,j2,j3))
    end do
  end do
 end do
 m1a=n1a-1

 do j3=m3a,m3b
  do j2=m2a,m2b
    do j1=m1b,m1a,-1 
#If #AVE eq "c"
      a12(j1,j2,j3) =         (d12(1)*d12(2))*a12(j1,j2,j3)
      a13(j1,j2,j3) =         (d12(1)*d12(3))*a13(j1,j2,j3)
#Else
      a12(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(2))*a12(j1,j2,j3) 
      a13(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(3))*a13(j1,j2,j3)
#End
    end do
  end do
 end do

 m2a=n2a
 do j3=m3a,m3b
  do j2=m2b,m2a,-1
    do j1=m1a,m1b
AVE     sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)) 
      a22(j1,j2,j3) = FACTOR *d22(2)*(a22(j1,j2,j3)+a22(j1,j2-1,j3))
    end do
  end do
 end do
 m2a=n2a-1

 do j3=m3a,m3b
  do j2=m2b,m2a,-1
    do j1=m1a,m1b
#If #AVE eq "c"
      a21(j1,j2,j3) =         (d12(1)*d12(2))*a21(j1,j2,j3)
      a23(j1,j2,j3) =         (d12(2)*d12(3))*a23(j1,j2,j3)
#Else
      a21(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(2))*a21(j1,j2,j3)
      a23(j1,j2,j3) = s(j1,j2,j3,0)*(d12(2)*d12(3))*a23(j1,j2,j3)
#End
    end do
  end do
 end do

 m3a=n3a
 do j3=m3b,m3a,-1
  do j2=m2a,m2b
    do j1=m1a,m1b
AVE     sh=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)/(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)) 
      a33(j1,j2,j3) = FACTOR *d22(3)*(a33(j1,j2,j3)+a33(j1,j2,j3-1))
    end do
  end do
 end do
 m3a=n3a-1

 do j3=m3b,m3a,-1
  do j2=m2a,m2b
    do j1=m1a,m1b
#If #AVE eq "c"
      a31(j1,j2,j3) =         (d12(1)*d12(3))*a31(j1,j2,j3)
      a32(j1,j2,j3) =         (d12(2)*d12(3))*a32(j1,j2,j3)
#Else
      a31(j1,j2,j3) = s(j1,j2,j3,0)*(d12(1)*d12(3))*a31(j1,j2,j3)
      a32(j1,j2,j3) = s(j1,j2,j3,0)*(d12(2)*d12(3))*a32(j1,j2,j3)
#End
    end do
  end do
 end do


   else
 ! ** old way -- average all coefficients

m1a=n1a
do j3=m3a,m3b
  do j2=m2a,m2b
    do j1=m1b,m1a,-1 ! go backwards  worry about division by zero
AVE     sh=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)) 
      a11(j1,j2,j3) = FACTOR *d22(1)*(a11(j1,j2,j3)+a11(j1-1,j2,j3))
      a12(j1,j2,j3) = FACTOR *(d12(1)*d12(2))*(a12(j1,j2,j3)+a12(j1-1,j2,j3))
      a13(j1,j2,j3) = FACTOR *(d12(1)*d12(3))*(a13(j1,j2,j3)+a13(j1-1,j2,j3))
    end do
  end do
end do
m1a=n1a-1
m2a=n2a
do j3=m3a,m3b
  do j2=m2b,m2a,-1
    do j1=m1a,m1b
AVE     sh=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)) 
      a21(j1,j2,j3) = FACTOR *(d12(1)*d12(2))*(a21(j1,j2,j3)+a21(j1,j2-1,j3))
      a22(j1,j2,j3) = FACTOR *d22(2)*(a22(j1,j2,j3)+a22(j1,j2-1,j3))
      a23(j1,j2,j3) = FACTOR *(d12(2)*d12(3))*(a23(j1,j2,j3)+a23(j1,j2-1,j3))
    end do
  end do
end do
m2a=n2a-1
m3a=n3a
do j3=m3b,m3a,-1
  do j2=m2a,m2b
    do j1=m1a,m1b
AVE     sh=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)/(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)) 
      a31(j1,j2,j3) = FACTOR *(d12(1)*d12(3))*(a31(j1,j2,j3)+a31(j1,j2,j3-1))
      a32(j1,j2,j3) = FACTOR *(d12(2)*d12(3))*(a32(j1,j2,j3)+a32(j1,j2,j3-1))
      a33(j1,j2,j3) = FACTOR *d22(3)*(a33(j1,j2,j3)+a33(j1,j2,j3-1))
    end do
  end do
end do
m3a=n3a-1

end if

#endMacro

#beginMacro defineA23()
m1a=n1a-1
m1b=n1b+1
m2a=n2a-1
m2b=n2b+1
m3a=n3a-1
m3b=n3b+1

if( averagingType .eq. arithmeticAverage )then
  factor=.5
  GETA23(s(j1,j2,j3,0)*jac(j1,j2,j3),c,factor)
else    
c  Harmonic average
  factor=2.
c  do not average in s:  
  GETA23(jac(j1,j2,j3), ,sh)  

end if    
#endMacro


c --------------------------------------------------------------------------------------------

c  define a macro
#beginMacro loopsDSG(arg)
do c=ca,cb
  do i3=n3a,n3b
    do i2=n2a,n2b
      do i1=n1a,n1b
        arg
      end do
    end do
  end do
end do
#endMacro

#beginMacro loopsDSG1(arg)
m1a=n1a
do j3=m3a,m3b
  do j2=m2a,m2b
    do j1=m1a,m1b 
      arg
    end do
  end do
end do
m1a=n1a-1
#endMacro

#beginMacro loopsDSG2(arg)
m2a=n2a
do j3=m3a,m3b
  do j2=m2a,m2b
    do j1=m1a,m1b
      arg
    end do
  end do
end do
m2a=n2a-1
#endMacro

#beginMacro loopsDSG3(arg)
m3a=n3a
do j3=m3a,m3b
  do j2=m2a,m2b
    do j1=m1a,m1b
      arg
    end do
  end do
end do
m3a=n3a-1
#endMacro


#beginMacro defineA21R()
m1a=n1a-1
m1b=n1b+1
m2a=n2a
m2b=n2b
m3a=n3a
m3b=n3b


c **** both divScalarGrad and derivativeScalarDerivative are the same in 1D *****
if( averagingType .eq. arithmeticAverage )then
  factor=.5
  loopsDSG1(a11(j1,j2,j3) = factor*h22(1)*(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
else    
c  Harmonic average
  factor=2.
  ! should be worry about division by zero?
  loopsDSG1(a11(j1,j2,j3)=s(j1,j2,j3,0)*h22(1)*s(j1-1,j2,j3,0)/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
end if    
#endMacro

c --------------------------------------------------------------------------------------------

c ===========================================================================================
c Define the coefficients for divScalarGrad, divTensorGrad and derivativeScalarDerivative
c    For 2d rectangular
c============================================================================================
#beginMacro defineA22R()
m1a=n1a-1
m1b=n1b+1
m2a=n2a-1
m2b=n2b+1
m3a=n3a
m3b=n3b


if( averagingType .eq. arithmeticAverage )then

  factor=.5
  if( derivOption.eq.divScalarGrad  )then
    loopsDSG1(a11(j1,j2,j3) = factor*h22(1)*(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
    loopsDSG2(a22(j1,j2,j3) = factor*h22(2)*(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
  else if( derivOption.eq.divTensorGrad )then
    ! form the coefficients and average
    !   we need to worry about the end points m1a=n1a-1 *wdh* 060210
    do j3=m3a,m3b
    do j2=m2a,m2b
      j2m1=max(j2-1,m2a)
    do j1=m1a,m1b
      j1m1=max(j1-1,m1a)
      a11(j1,j2,j3) = .5*(s(j1,j2,j3,0)+s(j1m1,j2,j3,0))/dx(1)**2
      a21(j1,j2,j3) = s(j1,j2,j3,1)/(4.*dx(1)*dx(2))
      a12(j1,j2,j3) = s(j1,j2,j3,2)/(4.*dx(1)*dx(2))
      a22(j1,j2,j3) = .5*(s(j1,j2,j3,3)+s(j1,j2m1,j3,3))/dx(2)**2
    end do
    end do
    end do
  else if( derivOption.eq.derivativeScalarDerivative )then
    if( dir1.eq.dir2 )then
      hh=h22(dir1+1)
    else
      hh=h21(dir1+1)*h21(dir2+1)
    end if
    if( dir1.eq.0 )then
      loopsDSG1(a11(j1,j2,j3) = factor*hh*(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
    else 
      loopsDSG2(a11(j1,j2,j3) = factor*hh*(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
    end if
 else 
   stop 1129   
 end if

else    
c  Harmonic average

  factor=2.
  if( derivOption.eq.divScalarGrad  )then
    ! should be worry about division by zero?
    loopsDSG1(a11(j1,j2,j3) =s(j1,j2,j3,0)*s(j1-1,j2,j3,0)*h22(1)*factor/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
    loopsDSG2(a22(j1,j2,j3) =s(j1,j2,j3,0)*s(j1,j2-1,j3,0)*h22(2)*factor/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
  else if( derivOption.eq.divTensorGrad )then
    stop 2219
  else
    if( dir1.eq.dir2 )then
      hh=h22(dir1+1)
    else
      hh=h21(dir1+1)*h21(dir2+1)
    end if
    if( dir1.eq.0 )then
      loopsDSG1(a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)*hh*factor/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
    else 
      loopsDSG2(a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)*hh*factor/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0))) 
    end if
  end if
end if    

#endMacro

c --------------------------------------------------------------------------------------------

#beginMacro defineA23R()
m1a=n1a-1
m1b=n1b+1
m2a=n2a-1
m2b=n2b+1
m3a=n3a-1
m3b=n3b+1


if( averagingType .eq. arithmeticAverage )then

 factor=.5
 if( derivOption.eq.divScalarGrad  )then
   loopsDSG1(a11(j1,j2,j3) = factor*h22(1)*(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
   loopsDSG2(a22(j1,j2,j3) = factor*h22(2)*(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
   loopsDSG3(a33(j1,j2,j3) = factor*h22(3)*(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)))

 else if( derivOption.eq.divTensorGrad )then

   ! form the coefficients and average
   !   we need to worry about the end points m1a=n1a-1 *wdh* 060210
   do j3=m3a,m3b
     j3m1=max(j3-1,m3a)
   do j2=m2a,m2b
     j2m1=max(j2-1,m2a)
   do j1=m1a,m1b
     j1m1=max(j1-1,m1a)
     a11(j1,j2,j3) = .5*(s(j1,j2,j3,0)+s(j1m1,j2,j3,0))/dx(1)**2
     a21(j1,j2,j3) =     s(j1,j2,j3,1)/(4.*dx(2)*dx(1))
     a31(j1,j2,j3) =     s(j1,j2,j3,2)/(4.*dx(3)*dx(1))

     a12(j1,j2,j3) =     s(j1,j2,j3,3)/(4.*dx(1)*dx(2))
     a22(j1,j2,j3) = .5*(s(j1,j2,j3,4)+s(j1,j2m1,j3,4))/dx(2)**2
     a32(j1,j2,j3) =     s(j1,j2,j3,5)/(4.*dx(3)*dx(2))

     a13(j1,j2,j3) =     s(j1,j2,j3,6)/(4.*dx(1)*dx(3))
     a23(j1,j2,j3) =     s(j1,j2,j3,7)/(4.*dx(2)*dx(3))
     a33(j1,j2,j3) = .5*(s(j1,j2,j3,8)+s(j1,j2,j3m1,8))/dx(3)**2
   end do
   end do
   end do

 else if( derivOption.eq.derivativeScalarDerivative )then
   if( dir1.eq.dir2 )then
     hh=h22(dir1+1)
   else
     hh=h21(dir1+1)*h21(dir2+1)
   end if
   if( dir1.eq.0 )then
     loopsDSG1(a11(j1,j2,j3) = factor*hh*(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
   else if( dir1.eq.1 )then
     loopsDSG2(a11(j1,j2,j3) = factor*hh*(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
   else 
     loopsDSG3(a11(j1,j2,j3) = factor*hh*(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)))
   end if
 else 
   stop 3329   
 end if

else    
c  Harmonic average

  factor=2.
  if( derivOption.eq.divScalarGrad  )then
    ! should be worry about division by zero?
    loopsDSG1(a11(j1,j2,j3) =s(j1,j2,j3,0)*s(j1-1,j2,j3,0)*h22(1)*factor/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
    loopsDSG2(a22(j1,j2,j3) =s(j1,j2,j3,0)*s(j1,j2-1,j3,0)*h22(2)*factor/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0)))
    loopsDSG3(a33(j1,j2,j3) =s(j1,j2,j3,0)*s(j1,j2,j3-1,0)*h22(3)*factor/(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)))
  else if( derivOption.eq.divTensorGrad )then
    stop 3388
  else if( derivOption.eq.derivativeScalarDerivative )then
    if( dir1.eq.dir2 )then
      hh=h22(dir1+1)
    else
      hh=h21(dir1+1)*h21(dir2+1)
    end if
    if( dir1.eq.0 )then
      loopsDSG1(a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1-1,j2,j3,0)*hh*factor/(s(j1,j2,j3,0)+s(j1-1,j2,j3,0)))
    else if( dir1.eq.1 )then
      loopsDSG2(a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1,j2-1,j3,0)*hh*factor/(s(j1,j2,j3,0)+s(j1,j2-1,j3,0))) 
    else 
      loopsDSG3(a11(j1,j2,j3)=s(j1,j2,j3,0)*s(j1,j2,j3-1,0)*hh*factor/(s(j1,j2,j3,0)+s(j1,j2,j3-1,0)))
    end if
  else 
    stop 3429   
  end if

end if    
#endMacro

c --------------------------------------------------------------------------------------------

