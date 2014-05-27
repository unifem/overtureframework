! You should preprocess this file with the bpp preprocessor before compiling.


! loops: use mask(i1,i2,i3)
#beginMacro loops(expression)
if( useWhereMask.ne.0 )then
  do c0=ca,cb
    c=uC(c0)  ! component of u
    f=fC(c0) ! component of forcing
    do i3=n3a,n3b
      do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1,i2,i3).ne.0 )then
            expression
          end if
        end do
      end do
    end do
  end do
else
  do c0=ca,cb
    c=uC(c0)
    f=fC(c0) ! component of forcing
    do i3=n3a,n3b
      do i2=n2a,n2b
        do i1=n1a,n1b
          expression
        end do
      end do
    end do
  end do
end if
#endMacro

! loopsm: use mask(i1+im1,i2+im2,i3+im3)
#beginMacro loopsm(expression)
if( useWhereMask.ne.0 )then
  do c0=ca,cb
    c=uC(c0)  ! component of u
    f=fC(c0) ! component of forcing
    do i3=n3a,n3b
      do i2=n2a,n2b
        do i1=n1a,n1b
          if( mask(i1+im1,i2+im2,i3+im3).ne.0 )then
            expression
          end if
        end do
      end do
    end do
  end do
else
  do c0=ca,cb
    c=uC(c0)
    f=fC(c0) ! component of forcing
    do i3=n3a,n3b
      do i2=n2a,n2b
        do i1=n1a,n1b
          expression
        end do
      end do
    end do
  end do
end if
#endMacro

! Assign ghost point from a Neuman or Mixed BC
! For variable coefficients we set ghost value from
!   v(i1,i2,i3,0)*u + v(i1,i2,i3,1)*coeff*u = rhs 
#beginMacro neumannLoops2D(rhs)
if( varCoeff.eq.1 )then
 ! var coeff: 
loopsm(u(i1+im1,i2+im2,i3+im3,c)=( \
  (rhs - v(i1,i2,i3,0)*u(i1,i2,i3,c))/v(i1,i2,i3,1) - ( \
  coeff(m21,i1,i2,i3)*u(i1  ,i2-1,i3,c) \
 +coeff(m12,i1,i2,i3)*u(i1-1,i2  ,i3,c) \
 +coeff(m22,i1,i2,i3)*u(i1  ,i2  ,i3,c) \
 +coeff(m32,i1,i2,i3)*u(i1+1,i2  ,i3,c) \
 +coeff(m23,i1,i2,i3)*u(i1  ,i2+1,i3,c) \
 ))/coeff(mGhost,i1,i2,i3) )
else ! const coeff
loopsm(u(i1+im1,i2+im2,i3+im3,c)=( \
 rhs - ( \
  coeff(m21,i1,i2,i3)*u(i1  ,i2-1,i3,c) \
 +coeff(m12,i1,i2,i3)*u(i1-1,i2  ,i3,c) \
 +coeff(m22,i1,i2,i3)*u(i1  ,i2  ,i3,c) \
 +coeff(m32,i1,i2,i3)*u(i1+1,i2  ,i3,c) \
 +coeff(m23,i1,i2,i3)*u(i1  ,i2+1,i3,c) \
 ))/coeff(mGhost,i1,i2,i3) )
end if
#endMacro


#beginMacro neumannLoops3D(rhs)
if( varCoeff.eq.1 )then
 ! var coeff:
loopsm(u(i1+im1,i2+im2,i3+im3,c)=( \
 (rhs - v(i1,i2,i3,0)*u(i1,i2,i3,c))/v(i1,i2,i3,1) - ( \
  coeff(m221,i1,i2,i3)*u(i1  ,i2  ,i3-1,c) \
 +coeff(m212,i1,i2,i3)*u(i1  ,i2-1,i3  ,c) \
 +coeff(m122,i1,i2,i3)*u(i1-1,i2  ,i3  ,c) \
 +coeff(m222,i1,i2,i3)*u(i1  ,i2  ,i3  ,c) \
 +coeff(m322,i1,i2,i3)*u(i1+1,i2  ,i3  ,c) \
 +coeff(m232,i1,i2,i3)*u(i1  ,i2+1,i3  ,c) \
 +coeff(m223,i1,i2,i3)*u(i1  ,i2  ,i3+1,c) \
 ))/coeff(mGhost,i1,i2,i3) )
else ! const coeff
loopsm(u(i1+im1,i2+im2,i3+im3,c)=( \
 rhs - ( \
  coeff(m221,i1,i2,i3)*u(i1  ,i2  ,i3-1,c) \
 +coeff(m212,i1,i2,i3)*u(i1  ,i2-1,i3  ,c) \
 +coeff(m122,i1,i2,i3)*u(i1-1,i2  ,i3  ,c) \
 +coeff(m222,i1,i2,i3)*u(i1  ,i2  ,i3  ,c) \
 +coeff(m322,i1,i2,i3)*u(i1+1,i2  ,i3  ,c) \
 +coeff(m232,i1,i2,i3)*u(i1  ,i2+1,i3  ,c) \
 +coeff(m223,i1,i2,i3)*u(i1  ,i2  ,i3+1,c) \
 ))/coeff(mGhost,i1,i2,i3) )
end if
#endMacro

! from pmb 
#beginMacro aDotGradULoops2D(rhs)
loopsm(u(i1+im1,i2+im2,i3+im3,c)=( \
 rhs - ( \
  coeff(m21,i1,i2,i3)*u(i1  ,i2-1,i3,c) \
 +coeff(m12,i1,i2,i3)*u(i1-1,i2  ,i3,c) \
 +coeff(m22,i1,i2,i3)*u(i1  ,i2  ,i3,c) \
 +coeff(m32,i1,i2,i3)*u(i1+1,i2  ,i3,c) \
 +coeff(m23,i1,i2,i3)*u(i1  ,i2+1,i3,c) \
 ))/coeff(mGhost,i1,i2,i3) )
#endMacro


#beginMacro aDotGradULoops3D(rhs)
loopsm(u(i1+im1,i2+im2,i3+im3,c)=( \
 rhs - ( \
  coeff(m221,i1,i2,i3)*u(i1  ,i2  ,i3-1,c) \
 +coeff(m212,i1,i2,i3)*u(i1  ,i2-1,i3  ,c) \
 +coeff(m122,i1,i2,i3)*u(i1-1,i2  ,i3  ,c) \
 +coeff(m222,i1,i2,i3)*u(i1  ,i2  ,i3  ,c) \
 +coeff(m322,i1,i2,i3)*u(i1+1,i2  ,i3  ,c) \
 +coeff(m232,i1,i2,i3)*u(i1  ,i2+1,i3  ,c) \
 +coeff(m223,i1,i2,i3)*u(i1  ,i2  ,i3+1,c) \
 ))/coeff(mGhost,i1,i2,i3) )
#endMacro

#beginMacro loopsd(expression)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).ne.0 )then
      expression
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    expression
  end do
  end do
  end do
end if
#endMacro
#beginMacro loopsd4(e1,e2,e3,e4)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).ne.0 )then
      e1
      e2
      e3
      e4
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
      e1
      e2
      e3
      e4
  end do
  end do
  end do
end if
#endMacro

! The generalized divergence BC should only be applied where the mask>0 061015
#beginMacro loopsgt(expression)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
      expression
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    expression
  end do
  end do
  end do
end if
#endMacro
#beginMacro loopsgt4(e1,e2,e3,e4)
if( useWhereMask.ne.0 )then
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
    if( mask(i1,i2,i3).gt.0 )then
      e1
      e2
      e3
      e4
    end if
  end do
  end do
  end do
else
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
      e1
      e2
      e3
      e4
  end do
  end do
  end do
end if
#endMacro


#beginMacro generalizedDivergenceRectangular(rhs)
if( nd.eq.1 )then
  loopsgt(u(i1+im1,i2,i3,n1)=u(i1+ip1,i2,i3,n1) +((2*side-1)*twoDeltaX/b1)*( rhs ))
else if( nd.eq.2 ) then
  if( axis.eq.0 ) then
    loopsgt(u(i1+im1,i2+im2,i3,n1)=u(i1+ip1,i2+ip2,i3,n1) +((2*side-1)*twoDeltaX/b1)*\
      ( rhs - (u(i1,i2+1,i3,n2)-u(i1,i2-1,i3,n2))*(b2/twoDeltaY) ) )
  else
    loopsgt( u(i1+im1,i2+im2,i3,n2)=u(i1+ip1,i2+ip2,i3,n2) +((2*side-1)*twoDeltaY/b2)*\
     ( rhs-(u(i1+1,i2,i3,n1)-u(i1-1,i2,i3,n1))*(b1/twoDeltaX) ) )
  end if
else
  if( axis.eq.0 ) then
    loopsgt( u(i1+im1,i2+im2,i3+im3,n1)=u(i1+ip1,i2+ip2,i3+ip3,n1)+((2*side-1)*twoDeltaX/b1)*\
      ( rhs - (u(i1  ,i2+1,i3  ,n2)-u(i1  ,i2-1,i3  ,n2))*(b2/twoDeltaY) \
                      - (u(i1  ,i2  ,i3+1,n3)-u(i1  ,i2  ,i3-1,n3))*(b3/twoDeltaZ) ) )
  else if( axis.eq.1 )then
    loopsgt( u(i1+im1,i2+im2,i3+im3,n2)=u(i1+ip1,i2+ip2,i3+ip3,n2) +((2*side-1)*twoDeltaY/b2)* \
      ( rhs - (u(i1+1,i2  ,i3  ,n1)-u(i1-1,i2  ,i3  ,n1))*(b1/twoDeltaX)\
                      - (u(i1  ,i2  ,i3+1,n3)-u(i1  ,i2  ,i3-1,n3))*(b3/twoDeltaZ) ) )
  else
    loopsgt( u(i1+im1,i2+im2,i3+im3,n3)=u(i1+ip1,i2+ip2,i3+ip3,n3) +((2*side-1)*twoDeltaZ/b3)*\
      ( rhs - (u(i1+1,i2  ,i3  ,n1)-u(i1-1,i2  ,i3  ,n1))*(b1/twoDeltaX)\
                      - (u(i1  ,i2+1,i3  ,n2)-u(i1  ,i2-1,i3  ,n2))*(b2/twoDeltaY) ) )
  end if
end if
#endMacro

#beginMacro generalizedDivergenceCurvilinearLoops1D(rhs)
  loopsgt(u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+b1*coeff(i1,i2,i3,0)*(rhs))
#endMacro
#beginMacro generalizedDivergenceCurvilinearLoops2D(rhs)
  loopsgt4(temp=rhs,\
          u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+b1*coeff(i1,i2,i3,0)*(temp), \
          u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+b2*coeff(i1,i2,i3,1)*(temp), )
#endMacro
#beginMacro generalizedDivergenceCurvilinearLoops3D(rhs)
  loopsgt4(temp=rhs,\
          u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+b1*coeff(i1,i2,i3,0)*(temp), \
          u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+b2*coeff(i1,i2,i3,1)*(temp), \
          u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+b3*coeff(i1,i2,i3,2)*(temp))
#endMacro


!   v(i1,i2,i3,n1) = ux(i1,i2,i3,n1) 
!   v(i1,i2,i3,n2) = uy(i1,i2,i3,n2) 
!   v(i1,i2,i3,n3) = uz(i1,i2,i3,n3) 
#beginMacro generalizedDivergenceCurvilinear1D(rhs)
generalizedDivergenceCurvilinearLoops1D(((rhs)- \
    (b1*v(i1,i2,i3,n1)))/  \
    ((b1*coeff(i1,i2,i3,0))**2))
#endMacro
#beginMacro generalizedDivergenceCurvilinear2D(rhs)
generalizedDivergenceCurvilinearLoops2D(((rhs)- \
    (b1*v(i1,i2,i3,n1)+b2*v(i1,i2,i3,n2)))/  \
    ((b1*coeff(i1,i2,i3,0))**2+(b2*coeff(i1,i2,i3,1))**2))
#endMacro
#beginMacro generalizedDivergenceCurvilinear3D(rhs)
generalizedDivergenceCurvilinearLoops3D(((rhs)- \
    (b1*v(i1,i2,i3,n1)+b2*v(i1,i2,i3,n2)+b3*v(i1,i2,i3,n3)))/  \
    ((b1*coeff(i1,i2,i3,0))**2+(b2*coeff(i1,i2,i3,1))**2+(b3*coeff(i1,i2,i3,2))**2))
#endMacro

#beginMacro normalDerivativeRectangularOld(rhs1,rhs2,rhs3)
if( nd.eq.2 ) then
loopsd4(temp=\
   (u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1)+rhs1*twoDeltaX)*v(i1,i2,i3,v0) \
  +(u(i1+ip1,i2+ip2,i3,n2)-u(i1+im1,i2+im2,i3,n2)+rhs2*twoDeltaX)*v(i1,i2,i3,v1), \
     u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*v(i1,i2,i3,v0), \
     u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*v(i1,i2,i3,v1), )
else if( nd.eq.3 ) then
loopsd4(temp=\
   (u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1)+rhs1*twoDeltaX)*v(i1,i2,i3,v0) \
  +(u(i1+ip1,i2+ip2,i3+ip3,n2)-u(i1+im1,i2+im2,i3+im3,n2)+rhs2*twoDeltaX)*v(i1,i2,i3,v1) \
  +(u(i1+ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+im2,i3+im3,n3)+rhs3*twoDeltaX)*v(i1,i2,i3,v2),\
     u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), \
     u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*v(i1,i2,i3,v1), \
     u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*v(i1,i2,i3,v2)) 
else
loopsd4(temp=\
   (u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1)+rhs1*twoDeltaX)*v(i1,i2,i3,v0), \
    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), , )
end if
#endMacro

#beginMacro normalDerivativeRectangular(rhs1,rhs2,rhs3)
if( nd.eq.2 ) then
loopsd4(temp=\
   (u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1)+rhs1*twoDeltaX)*vv(0) \
  +(u(i1+ip1,i2+ip2,i3,n2)-u(i1+im1,i2+im2,i3,n2)+rhs2*twoDeltaX)*vv(1), \
     u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*vv(0), \
     u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*vv(1), )
else if( nd.eq.3 ) then
loopsd4(temp=\
   (u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1)+rhs1*twoDeltaX)*vv(0) \
  +(u(i1+ip1,i2+ip2,i3+ip3,n2)-u(i1+im1,i2+im2,i3+im3,n2)+rhs2*twoDeltaX)*vv(1) \
  +(u(i1+ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+im2,i3+im3,n3)+rhs3*twoDeltaX)*vv(2),\
     u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*vv(0), \
     u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*vv(1), \
     u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*vv(2)) 
else
loopsd4(temp=\
   (u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1)+rhs1*twoDeltaX)*vv(0), \
    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*vv(0), , )
end if
#endMacro


#beginMacro normalDerivativeCurvilinear(rhs)
if( nd.eq.2 ) then
loopsd4(temp=(\
   (rhs) \
 -(coeff(m21,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3,n1) \
                       +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3,n2)) \
  +coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1) \
                       +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3,n2)) \
  +coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1) \
                       +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3,n2)) \
  +coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1) \
                       +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3,n2)) \
  +coeff(m23,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3,n1) \
                       +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3,n2)) ) )/coeff(mGhost,i1,i2,i3), \
     u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*v(i1,i2,i3,v0), \
     u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*v(i1,i2,i3,v1), )
else if( nd.eq.3 ) then
loopsd4(temp=( \
    (rhs) \
   -(coeff(m221,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3-1,n1) \
		       	  +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3-1,n2) \
		          +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3-1,n3)) \
    +coeff(m212,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2-1,i3  ,n1) \
		          +v(i1,i2,i3,v1)*u(i1  ,i2-1,i3  ,n2) \
		          +v(i1,i2,i3,v2)*u(i1  ,i2-1,i3  ,n3)) \
    +coeff(m122,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3  ,n1) \
		          +v(i1,i2,i3,v1)*u(i1-1,i2  ,i3  ,n2) \
		          +v(i1,i2,i3,v2)*u(i1-1,i2  ,i3  ,n3)) \
    +coeff(m222,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3  ,n1) \
		          +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3  ,n2) \
		          +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3  ,n3)) \
    +coeff(m322,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3  ,n1) \
		          +v(i1,i2,i3,v1)*u(i1+1,i2  ,i3  ,n2) \
		          +v(i1,i2,i3,v2)*u(i1+1,i2  ,i3  ,n3)) \
    +coeff(m232,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2+1,i3  ,n1) \
		          +v(i1,i2,i3,v1)*u(i1  ,i2+1,i3  ,n2) \
		          +v(i1,i2,i3,v2)*u(i1  ,i2+1,i3  ,n3)) \
    +coeff(m223,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3+1,n1) \
			  +v(i1,i2,i3,v1)*u(i1  ,i2  ,i3+1,n2) \
			  +v(i1,i2,i3,v2)*u(i1  ,i2  ,i3+1,n3)) ) )/coeff(mGhost,i1,i2,i3), \
     u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), \
     u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*v(i1,i2,i3,v1), \
     u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*v(i1,i2,i3,v2)) 
else
loopsd4(temp=( \
    (rhs) \
      - (coeff(m12,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1-1,i2  ,i3,n1))\
        +coeff(m22,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1  ,i2  ,i3,n1))\
        +coeff(m32,i1,i2,i3)*(v(i1,i2,i3,v0)*u(i1+1,i2  ,i3,n1)) ) )/coeff(mGhost,i1,i2,i3), \
    u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), , )
end if
#endMacro

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
#beginMacro beginLoopsNoMask()
  do i3=n3a,n3b
  do i2=n2a,n2b
  do i1=n1a,n1b
#endMacro
#beginMacro endLoopsNoMask()
  end do
  end do
  end do
#endMacro


! ================================================================================================
!   Set the normal component for a curvilinear grid
!
!  DIM : dimension, 2 or 3
!  FORCING: type of forcing, NO_FORCING, SCALAR_FORCING, GF_FORCING, ARRAY_FORCING, VECTOR_FORCING
!================================================================================================
#beginMacro setNormalComponent(DIM,FORCING)
#If #DIM == "2"
 an1=rsxy(i1,i2,i3,axis,0)
 an2=rsxy(i1,i2,i3,axis,1)
 nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)

 aNorm=an1**2+an2**2
 #If #FORCING == "NO_FORCING"
   nDotU=nDotU/aNorm
 #Elif #FORCING == "SCALAR_FORCING"
   nDotU=(nDotU - scalarData*nsign*sqrt(anorm) )/aNorm 
 #Elif #FORCING == "GF_FORCING"
   nDotU=(nDotU - (an1*gfData(i1,i2,i3,m1)+an2*gfData(i1,i2,i3,m2)) )/aNorm
 #Elif #FORCING == "ARRAY_FORCING"
   nDotU=(nDotU - (an1*fData(m1,side,axis,grid)+an2*fData(m2,side,axis,grid)) )/aNorm
 #Elif #FORCING == "VECTOR_FORCING"
   nDotU=(nDotU - (an1*vData(m1)+an2*vData(m2)) )/aNorm
 #Else
   stop 55277
 #End

 u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
 u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2

#Elif #DIM == "3"
 an1=rsxy(i1,i2,i3,axis,0)
 an2=rsxy(i1,i2,i3,axis,1)
 an3=rsxy(i1,i2,i3,axis,2)
 nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)+an3*u(i1,i2,i3,n3)

 aNorm=an1**2+an2**2+an3**2
 #If #FORCING == "NO_FORCING"
   nDotU=nDotU/aNorm
 #Elif #FORCING == "SCALAR_FORCING"
   nDotU=(nDotU - scalarData*nsign*sqrt(anorm) )/aNorm 
 #Elif #FORCING == "GF_FORCING"
   nDotU=(nDotU - (an1*gfData(i1,i2,i3,m1)+an2*gfData(i1,i2,i3,m2)+an3*gfData(i1,i2,i3,m3)) )/aNorm
 #Elif #FORCING == "ARRAY_FORCING"
   nDotU=(nDotU - (an1*fData(m1,side,axis,grid)+an2*fData(m2,side,axis,grid)+an3*fData(m3,side,axis,grid)) )/aNorm
 #Elif #FORCING == "VECTOR_FORCING"
   nDotU=(nDotU - (an1*vData(m1)+an2*vData(m2)+an3*vData(m3)) )/aNorm
 #Else
   stop 55277
 #End

 u(i1,i2,i3,n1)=u(i1,i2,i3,n1)-nDotU*an1
 u(i1,i2,i3,n2)=u(i1,i2,i3,n2)-nDotU*an2
 u(i1,i2,i3,n3)=u(i1,i2,i3,n3)-nDotU*an3
#End
#endMacro

! ================================================================================================
!   Assign the normal component for a curvilinear grid
!
!  FORCING: type of forcing, NO_FORCING, SCALAR_FORCING, GF_FORCING, ARRAY_FORCING, VECTOR_FORCING
!================================================================================================
#beginMacro assignNormalComponent(FORCING)
if( useWhereMask.ne.0 )then
 if( nd.eq.2 )then
  beginLoops()
   setNormalComponent(2,FORCING)
  endLoops()
 else if( nd.eq.3 )then
  beginLoops()
   setNormalComponent(3,FORCING)
  endLoops()
 else
  stop 92743
 end if
else
 if( nd.eq.2 )then
  beginLoopsNoMask()
   setNormalComponent(2,FORCING)
  endLoopsNoMask()
 else if( nd.eq.3 )then
  beginLoopsNoMask()
   setNormalComponent(3,FORCING)
  endLoopsNoMask()
 else
  stop 92743
 end if
end if
#endMacro

! ================================================================================================
!   Set the tangential component for a curvilinear grid
!
!  DIM : dimension, 2 or 3
!  FORCING: type of forcing, NO_FORCING, SCALAR_FORCING, GF_FORCING, ARRAY_FORCING, VECTOR_FORCING
!================================================================================================
#beginMacro setTangentialComponent(DIM,FORCING)

#If #DIM == "2"

 ! NOTE: this normal is NOT always the outward normal but this doesn't matter here
 an1=rsxy(i1,i2,i3,axis,0)
 an2=rsxy(i1,i2,i3,axis,1)
 aNormi=1./max(epsX,sqrt(an1**2+an2**2))
 an1=an1*aNormi
 an2=an2*aNormi
 nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)

 #If #FORCING == "NO_FORCING"
   g1=0.
   g2=0.
 #Elif #FORCING == "SCALAR_FORCING"
   stop 55277
 #Elif #FORCING == "GF_FORCING"
   stop 55277
 #Elif #FORCING == "ARRAY_FORCING"

 #Elif #FORCING == "VECTOR_FORCING"
   stop 55277
 #Else
   stop 55277
 #End

 u(i1,i2,i3,n1)=nDotU*an1 +g1
 u(i1,i2,i3,n2)=nDotU*an2 +g2

#Elif #DIM == "3"

 ! NOTE: this normal is NOT always the outward normal but this doesn't matter here
 an1=rsxy(i1,i2,i3,axis,0)
 an2=rsxy(i1,i2,i3,axis,1)
 an3=rsxy(i1,i2,i3,axis,2)
 aNormi=1./max(epsX,sqrt(an1**2+an2**2+an3**2))
 an1=an1*aNormi
 an2=an2*aNormi
 an3=an3*aNormi

 nDotU=an1*u(i1,i2,i3,n1)+an2*u(i1,i2,i3,n2)+an3*u(i1,i2,i3,n3)

 #If #FORCING == "NO_FORCING"
   g1=0.
   g2=0.
   g3=0.
 #Elif #FORCING == "SCALAR_FORCING"
   stop 55277
 #Elif #FORCING == "GF_FORCING"
   stop 55277
 #Elif #FORCING == "ARRAY_FORCING"
   stop 55277
 #Elif #FORCING == "VECTOR_FORCING"

 #Else
   stop 55277
 #End

 u(i1,i2,i3,n1)=nDotU*an1 +g1
 u(i1,i2,i3,n2)=nDotU*an2 +g2
 u(i1,i2,i3,n3)=nDotU*an3 +g3 
#End
#endMacro

! ================================================================================================
!   Assign the tangential component for a curvilinear grid
!
!  FORCING: type of forcing, NO_FORCING, SCALAR_FORCING, GF_FORCING, ARRAY_FORCING, VECTOR_FORCING
!================================================================================================
#beginMacro assignTangentialComponent(FORCING)
if( useWhereMask.ne.0 )then
 if( nd.eq.2 )then
  beginLoops()
   setTangentialComponent(2,FORCING)
  endLoops()
 else if( nd.eq.3 )then
  beginLoops()
   setTangentialComponent(3,FORCING)
  endLoops()
 else
  stop 92743
 end if
else
 if( nd.eq.2 )then
  beginLoopsNoMask()
   setTangentialComponent(2,FORCING)
  endLoopsNoMask()
 else if( nd.eq.3 )then
  beginLoopsNoMask()
   setTangentialComponent(3,FORCING)
  endLoopsNoMask()
 else
  stop 92743
 end if
end if
#endMacro

      subroutine assignBoundaryConditions( nd, 
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData, 
     & dx,dr,ipar,par, ca,cb, uCBase, uC, fCBase, fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )    
!======================================================================
!  Optimised Boundary Conditions
!         
! nd : number of space dimensions
! uC(uCBase:*) : defines components to assign
! ca,cb : assign components c=uC(ca),..,uC(cb)
! fC(fCBase:*) : defines components of forcing to use.
! gridType: 0=rectangular, 1=non-rectangular
! order : 2 or 4 -- only 2 implemented
! par, ipar : real and integer parameters
! useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
! lineForForcing : if 0 evaluate the forcing gfData on the boundary, if 1 evaluate forcing
!    on the first ghost line, etc.
!
! v : = variable coefficients 
!   : for generalizedDivergence v holds ux(i1,i2,i3,n1) uy(i1,i2,i3,n2) uz(i1,i2,i3,n3)
!======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,uCBase,fCBase

      integer side,axis,grid, bcType,bcOption,gridType,order,
     &  useWhereMask,lineForForcing

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real v(ndv1a:ndv1b,ndv2a:ndv2b,ndv3a:ndv3b,ndv4a:ndv4b)
      real coeff(ndc1a:ndc1b,ndc2a:ndc2b,ndc3a:ndc3b,ndc4a:ndc4b)
      real gfData(ndg1a:ndg1b,ndg2a:ndg2b,ndg3a:ndg3b,ndg4a:ndg4b)
      real fData(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b,ndf4a:ndf4b)
      real vData(ndw1a:ndw1b)
      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)

      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

      integer ipar(0:*), ca,cb,uC(uCBase:*),fC(fCBase:*)
      real scalarData,par(0:*),dx(3),dr(3)
      
      real vv(0:2)
      integer axisp1,axisp2
!      real h21(3),d22(3),d12(3),h22(3)
!      real d24(3),d14(3),h42(3),h41(3)


!     --- boundary conditions (from BCTypes.h) ---
      integer dirichlet,
     &        neumann,
     &        extrapolate,
     &        normalComponent,
     &        mixed,
     &        generalMixedDerivative,
     &        normalDerOfNormalComponent,
     &        normalDerivativeOfADotU,
     &        aDotU,
     &        aDotGradU,
     &        normalDotScalarGrad,
     &        evenSymmetry,
     &        oddSymmetry,
     &        generalizedDivergence,
     &        vectorSymmetry,
     &        tangentialComponent0,
     &        tangentialComponent1,
     &        normalDerOfTangentialComponent0,
     &        normalDerOfTangentialComponent1,
     &        extrapolateInterpNeighbours,
     &        tangentialComponent,                      
     &        extrapolateNormalComponent,
     &        extrapolateTangentialComponent0,
     &        extrapolateTangentialComponent1
      parameter(dirichlet=0,\
                neumann=dirichlet+1,\
                extrapolate=neumann+1,\
                normalComponent=extrapolate+1,\
                mixed=normalComponent+1,\
                generalMixedDerivative=mixed+1,\
                normalDerOfNormalComponent=generalMixedDerivative+1,\
                normalDerivativeOfADotU=normalDerOfNormalComponent+1,\
                aDotU=normalDerivativeOfADotU+1,\
                aDotGradU=aDotU+1,\
                normalDotScalarGrad=aDotGradU+1,\
                evenSymmetry=normalDotScalarGrad+1,\
                oddSymmetry=evenSymmetry+1,\
                generalizedDivergence=oddSymmetry+1,\
                vectorSymmetry=generalizedDivergence+1,\
                tangentialComponent0=vectorSymmetry+1,\
                tangentialComponent1=tangentialComponent0+1,\
                normalDerOfTangentialComponent0=tangentialComponent1+1,\
                normalDerOfTangentialComponent1 =normalDerOfTangentialComponent0+1,\
                extrapolateInterpNeighbours =normalDerOfTangentialComponent1+1,\
                tangentialComponent =extrapolateInterpNeighbours+1,\
                extrapolateNormalComponent=tangentialComponent+1,\
                extrapolateTangentialComponent0 =extrapolateNormalComponent+1,\
                extrapolateTangentialComponent1 =extrapolateTangentialComponent0+1 )

!     --- grid types
      integer rectangular,curvilinear
      parameter( rectangular=0,curvilinear=1 )
    
!     --- forcing types ---
      integer  scalarForcing,gfForcing,arrayForcing,vectorForcing
      parameter( scalarForcing=0,gfForcing=1,arrayForcing=2,
     &           vectorForcing=3 )

!     --- local variables ----
      real b0,b1,b2,b3,twoDeltaX,twoDeltaY,twoDeltaZ,temp
      real nsign,an1,an2,an3,nDotU,anorm
      integer c,c0,f,i1,i2,i3,im1,im2,im3,ip1,ip2,ip3,if1,if2,if3,cn,cm
      integer n1,n2,n3,m1,m2,m3,v0,v1,v2
    
      integer mGhost
      integer m21,m12,m22,m32,m23
      integer m221,m212,m122,m222,m322,m232,m223



      if( bcType.eq.neumann .or. bcType.eq.mixed )then

        call assignOptNeumann( nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData, 
     & dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )   


      else if( bcType.eq.generalizedDivergence )then
        ! ***********************************
        ! ****** Generalized divergence *****
        ! ***********************************  
        !  to set the component along a to g:
        !       u <- u + (g-(a.u)) a/<a,a>
        !       g-(a.u) = b - ( discrete div of u )

        call assignOptGenDiv( nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData, 
     & dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )   

      else if( bcType.eq.aDotGradU )then
        ! from pmb 080421
        ! ***********************************
        ! ******       aDotGradU        *****
        ! ***********************************  
        !  to set the derivative along a to g:
	!  (a.grad) u = g

        call assignOptADotGradU( nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData, 
     & dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )


      else if( bcType.eq.normalDerOfNormalComponent .or.
     &         bcType.eq.normalDerOfTangentialComponent0 .or.
     &         bcType.eq.normalDerOfTangentialComponent1 )then

!       **************************************************
!       **** normal derivative of a vector dot u *********
!       **************************************************

        call assignOptNormalDer( nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData, 
     & dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )   
        twoDeltaX=par(0)


      else if( bcType.eq.normalComponent )then
!        ****************************
!        ***** Normal Component *****
!        ****************************

        call assignOptNormalComponent( nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData, 
     & dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )   

      else if( bcType.eq.tangentialComponent .or.  
     &         bcType.eq.tangentialComponent0 .or. 
     &         bcType.eq.tangentialComponent1 )then
!        ********************************
!        ***** Tangential Component *****
!        ********************************

        call assignOptTangentialComponent( nd,
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData, 
     & dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )   



      else
        write(*,*) 'assignBoundaryConditions:ERROR unknown bcType=',
     &    bcType
        stop 33
      end if

      return 
      end



! ======================================================================
! 
!  Generate opt BC routines for different BCs
!
! BCOPT: dirichlet, neuman
! ======================================================================
#beginMacro assignBoundaryConditionMacro(subroutineName,BCOPT)
  subroutine subroutineName( nd,  \
    n1a,n1b,n2a,n2b,n3a,n3b, \
    ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, \
    ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b, \
    ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b, \
    ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b, \
    ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b, \
    ndw1a,ndw1b, \
    ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b, \
    nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,rsxy, \
    u,v,coeff, mask, \
    scalarData,gfData,fData,vData,  \
    dx,dr,ipar,par, ca,cb, uCBase,uC, fCBase,fC, \
    side,axis,grid, bcType,bcOption,gridType,order,useWhereMask, \
    lineForForcing )    
!======================================================================
!  Optimised Boundary Conditions
!         
! nd : number of space dimensions
! uC(uCBase:*) : defines components to assign
! ca,cb : assign components c=uC(ca),..,uC(cb)
! fC(fCBase:*) : defines components of forcing to use.
! gridType: 0=rectangular, 1=non-rectangular
! order : 2 or 4 -- only 2 implemented
! par, ipar : real and integer parameters
! useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
! lineForForcing : if 0 evaluate the forcing gfData on the boundary, if 1 evaluate forcing
!    on the first ghost line, etc.
!
! v : for generalizedDiveregence v holds ux(i1,i2,i3,n1) uy(i1,i2,i3,n2) uz(i1,i2,i3,n3)
!======================================================================
 implicit none
 integer nd, n1a,n1b,n2a,n2b,n3a,n3b, \
 ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, \
 ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b, \
 ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b, \
 ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b, \
 ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b, \
 ndw1a,ndw1b, \
 ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,uCBase,fCBase

 integer side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,lineForForcing

 real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
 real v(ndv1a:ndv1b,ndv2a:ndv2b,ndv3a:ndv3b,ndv4a:ndv4b)
 real coeff(ndc1a:ndc1b,ndc2a:ndc2b,ndc3a:ndc3b,ndc4a:ndc4b)
 real gfData(ndg1a:ndg1b,ndg2a:ndg2b,ndg3a:ndg3b,ndg4a:ndg4b)
 real fData(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b,ndf4a:ndf4b)
 real vData(ndw1a:ndw1b)
 integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)

 integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
 real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:nd-1,0:nd-1)

 integer ipar(0:*), ca,cb,uC(uCBase:*),fC(fCBase:*)
 real scalarData,par(0:*),dx(3),dr(3)
 
 real vv(0:2)
 integer axisp1,axisp2

!     --- boundary conditions (from BCTypes.h) ---
 integer dirichlet, \
     neumann, \
     extrapolate, \
     normalComponent, \
     mixed, \
     generalMixedDerivative, \
     normalDerOfNormalComponent, \
     normalDerivativeOfADotU, \
     aDotU, \
     aDotGradU, \
     normalDotScalarGrad, \
     evenSymmetry, \
     oddSymmetry, \
     generalizedDivergence, \
     vectorSymmetry, \
     tangentialComponent0, \
     tangentialComponent1, \
     normalDerOfTangentialComponent0, \
     normalDerOfTangentialComponent1, \
     extrapolateInterpNeighbours, \
     tangentialComponent,                       \
     extrapolateNormalComponent, \
     extrapolateTangentialComponent0, \
     extrapolateTangentialComponent1
 parameter(dirichlet=0,\
      neumann=dirichlet+1,\
      extrapolate=neumann+1,\
      normalComponent=extrapolate+1,\
      mixed=normalComponent+1,\
      generalMixedDerivative=mixed+1,\
      normalDerOfNormalComponent=generalMixedDerivative+1,\
      normalDerivativeOfADotU=normalDerOfNormalComponent+1,\
      aDotU=normalDerivativeOfADotU+1,\
      aDotGradU=aDotU+1,\
      normalDotScalarGrad=aDotGradU+1,\
      evenSymmetry=normalDotScalarGrad+1,\
      oddSymmetry=evenSymmetry+1,\
      generalizedDivergence=oddSymmetry+1,\
      vectorSymmetry=generalizedDivergence+1,\
      tangentialComponent0=vectorSymmetry+1,\
      tangentialComponent1=tangentialComponent0+1,\
      normalDerOfTangentialComponent0=tangentialComponent1+1,\
      normalDerOfTangentialComponent1 =normalDerOfTangentialComponent0+1,\
      extrapolateInterpNeighbours =normalDerOfTangentialComponent1+1,\
      tangentialComponent =extrapolateInterpNeighbours+1,\
      extrapolateNormalComponent=tangentialComponent+1,\
      extrapolateTangentialComponent0 =extrapolateNormalComponent+1,\
      extrapolateTangentialComponent1 =extrapolateTangentialComponent0+1 )

!     --- grid types
 integer rectangular,curvilinear
 parameter( rectangular=0,curvilinear=1 )
    
!     --- forcing types ---
 integer  scalarForcing,gfForcing,arrayForcing,vectorForcing
 parameter( scalarForcing=0,gfForcing=1,arrayForcing=2,vectorForcing=3 )

!     --- local variables ----
 real b0,b1,b2,b3,twoDeltaX,twoDeltaY,twoDeltaZ,temp,g1,g2,g3
 real nsign,an1,an2,an3,nDotU,anorm,aNormi
 integer c,c0,f,i1,i2,i3,im1,im2,im3,ip1,ip2,ip3,if1,if2,if3,cn,cm
 integer n1,n2,n3,m1,m2,m3,v0,v1,v2,ct1,ct2
 real epsX

 integer mGhost
 integer m21,m12,m22,m32,m23
 integer m221,m212,m122,m222,m322,m232,m223
 integer varCoeff

 epsX=1.e-100  ! prevent division by zero when normalizing the normal vector  *FIX ME* -- this should be passed in 


 if( side.lt.0 .or. side.gt.1 )then
   write(*,*) 'applyBoundaryConditions:ERROR: side=',side
   stop 1
 end if
 if( axis.lt.0 .or. axis.ge.nd )then
   write(*,*) 'applyBoundaryConditions:ERROR: axise=',axis
   stop 2
 end if

 if( useWheremask.ne.0 )then
   if( n1a.lt.ndm1a .or. n1b.gt.ndm1b .or. n2a.lt.ndm2a .or. n2b.gt.ndm2b .or. \
       n3a.lt.ndm3a .or. n3b.gt.ndm3b )then
     write(*,'("ERROR:assignBoundaryConditions:mask bounds are wrong for useWhereMask")')
     write(*,'(" n1a,n1b,n2a,n2b,n3a,n3b=",6i5)') n1a,n1b,n2a,n2b,n3a,n3b
     write(*,'(" ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b=",6i5)') ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b
     ! '
     stop 8855
   end if
 end if

 im1=0
 im2=0
 im3=0
 if( axis.eq.0 )then
   im1=2*side-1
 else if( axis.eq.1 )then
   im2=2*side-1
 else
   im3=2*side-1
 end if
 ip1=-im1
 ip2=-im2
 ip3=-im3

 if1=im1*lineForForcing
 if2=im2*lineForForcing
 if3=im3*lineForForcing


#If #BCOPT eq "neumann"
  if( bcType.ne.neumann .and. bcType.ne.mixed )then
    write(*,'("ERROR")') 
    stop 1145
  end if
!**      if( bcType.eq.neumann .or. bcType.eq.mixed )then

!        *******************
!        ***** Neumann *****
!        *******************
 b0=par(0)
 b1=par(1)
 twoDeltaX=par(2)
 varCoeff=ipar(1)
 ! write(*,'(" BC:Neumann:varCoeff=",i4)') varCoeff

 if( gridType.eq.rectangular )then   
!          *************************
!          *** rectangular grid  ***
!          *************************
   if( varCoeff.eq.1 )then
     ! variable coefficients:
     !   v(i1,i2,i3,0)*u + v(i1,i2,i3,1)*u.n = g 
     if( bcOption.eq.scalarForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(scalarData-v(i1,i2,i3,0)*u(i1,i2,i3,c))*(twoDeltaX/v(i1,i2,i3,1)));
     else if( bcOption.eq.gfForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(gfData(i1+if1,i2+if2,i3+if3,f)-v(i1,i2,i3,0)*u(i1,i2,i3,c))*(twoDeltaX/v(i1,i2,i3,1)))
     else if( bcOption.eq.arrayForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(fData(f,side,axis,grid)-v(i1,i2,i3,0)*u(i1,i2,i3,c))*(twoDeltaX/v(i1,i2,i3,1)))
     else if( bcOption.eq.vectorForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(vData(f)-v(i1,i2,i3,0)*u(i1,i2,i3,c))*(twoDeltaX/v(i1,i2,i3,1)))
     else
       write(*,*) 'assignBC:ERROR unknown bcOption=',bcOption
       stop 21
     end if

   else if( bcType.eq.neumann .or. b0.eq.0. )then
!            *** neumann ***
     if( bcOption.eq.scalarForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+scalarData*(twoDeltaX/b1))
     else if( bcOption.eq.gfForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+gfData(i1+if1,i2+if2,i3+if3,f)*(twoDeltaX/b1))
     else if( bcOption.eq.arrayForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+fData(f,side,axis,grid)*(twoDeltaX/b1))
     else if( bcOption.eq.vectorForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+vData(f)*(twoDeltaX/b1))
     else
       write(*,*) 'assignBC:ERROR unknown bcOption=',bcOption
       stop 22
     end if

   else
!           *** mixed ***

     if( bcOption.eq.scalarForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(scalarData-b0*u(i1,i2,i3,c))*(twoDeltaX/b1));
     else if( bcOption.eq.gfForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(gfData(i1+if1,i2+if2,i3+if3,f)-b0*u(i1,i2,i3,c))*(twoDeltaX/b1))
     else if( bcOption.eq.arrayForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(fData(f,side,axis,grid)-b0*u(i1,i2,i3,c))*(twoDeltaX/b1))
     else if( bcOption.eq.vectorForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(vData(f)-b0*u(i1,i2,i3,c))*(twoDeltaX/b1))
     else
       write(*,*) 'assignBC:ERROR unknown bcOption=',bcOption
       stop 23
     end if
   end if

 else 
!          *************************
!          **** Curvilinear case ***
!          *************************
   if( nd.eq.2 )then
     m21=1
     m12=3
     m22=4
     m32=5
     m23=7
     if( axis.eq.0 )then
       mGhost=m22+2*side-1
     else
       mGhost=m22+3*(2*side-1)
     end if
     ! first zero out ghost value
     loopsm(u(i1+im1,i2+im2,i3+im3,c)=0.) 
     
     if( bcOption.eq.scalarForcing )then
       neumannLoops2D(scalarData)
     else if( bcOption.eq.gfForcing )then
       neumannLoops2D(gfData(i1+if1,i2+if2,i3+if3,f))
     else if( bcOption.eq.arrayForcing )then
       neumannLoops2D(fData(f,side,axis,grid))
     else if( bcOption.eq.vectorForcing )then
       neumannLoops2D(vData(f))
     end if

   else
     m221=4
     m212=10
     m122=12
     m222=13
     m322=14
     m232=16
     m223=22
     if( axis.eq.0 )then
       mGhost=m222+2*side-1
     else if( axis.eq.1 )then
       mGhost=m222+3*(2*side-1)
     else
       mGhost=m222+9*(2*side-1)
     end if
     ! first zero out ghost value
     loopsm(u(i1+im1,i2+im2,i3+im3,c)=0.) 
     if( bcOption.eq.scalarForcing )then
       neumannLoops3D(scalarData)
     else if( bcOption.eq.gfForcing )then
       neumannLoops3D(gfData(i1+if1,i2+if2,i3+if3,f))
     else if( bcOption.eq.arrayForcing )then
       neumannLoops3D(fData(f,side,axis,grid))
     else if( bcOption.eq.vectorForcing )then
       neumannLoops3D(vData(f))
     end if

   end if

 end if


#Elif #BCOPT eq "aDotGradU"
  !  from pmb 080421
  if( bcType.ne.aDotGradU)then
    write(*,'("ERROR")') 
    stop 1145
  end if

!        **********************
!        ***** aDotGradU  *****
!        **********************
 b0=par(0)
 b1=par(1)
 b2=par(2)
 twoDeltaX=par(3)
 twoDeltaY=par(4)
 twoDeltaZ=par(5)


 if( gridType.eq.rectangular )then   
!          *************************
!          *** rectangular grid  ***
!          *************************

     if( bcOption.eq.scalarForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(scalarData-b0*u(i1,i2,i3,c))*(twoDeltaX/b1));
     else if( bcOption.eq.gfForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(gfData(i1+if1,i2+if2,i3+if3,f)-b0*u(i1,i2,i3,c))*(twoDeltaX/b1))
     else if( bcOption.eq.arrayForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(fData(f,side,axis,grid)-b0*u(i1,i2,i3,c))*(twoDeltaX/b1))
     else if( bcOption.eq.vectorForcing )then
       loopsm(u(i1+im1,i2+im2,i3+im3,c)=u(i1+ip1,i2+ip2,i3+ip3,c)+(vData(f)-b0*u(i1,i2,i3,c))*(twoDeltaX/b1))
     else
       write(*,*) 'assignBC:ERROR unknown bcOption=',bcOption
       stop 2
     end if

 else 
!          *************************
!          **** Curvilinear case ***
!          *************************
   if( nd.eq.2 )then
     m21=1
     m12=3
     m22=4
     m32=5
     m23=7
     if( axis.eq.0 )then
       mGhost=m22+2*side-1
     else
       mGhost=m22+3*(2*side-1)
     end if
     ! first zero out ghost value
     loopsm(u(i1+im1,i2+im2,i3+im3,c)=0.) 
     
     if( bcOption.eq.scalarForcing )then
       aDotGradULoops2D(scalarData)
     else if( bcOption.eq.gfForcing )then
       aDotGradULoops2D(gfData(i1+if1,i2+if2,i3+if3,f))
     else if( bcOption.eq.arrayForcing )then
       aDotGradULoops2D(fData(f,side,axis,grid))
     else if( bcOption.eq.vectorForcing )then
       aDotGradULoops2D(vData(f))
     end if

   else
     m221=4
     m212=10
     m122=12
     m222=13
     m322=14
     m232=16
     m223=22
     if( axis.eq.0 )then
       mGhost=m222+2*side-1
     else if( axis.eq.1 )then
       mGhost=m222+3*(2*side-1)
     else
       mGhost=m222+9*(2*side-1)
     end if
     ! first zero out ghost value
     loopsm(u(i1+im1,i2+im2,i3+im3,c)=0.) 
     if( bcOption.eq.scalarForcing )then
       aDotGradULoops3D(scalarData)
     else if( bcOption.eq.gfForcing )then
       aDotGradULoops3D(gfData(i1+if1,i2+if2,i3+if3,f))
     else if( bcOption.eq.arrayForcing )then
       aDotGradULoops3D(fData(f,side,axis,grid))
     else if( bcOption.eq.vectorForcing )then
       aDotGradULoops3D(vData(f))
     end if
   end if
 end if




#Elif #BCOPT eq "generalizedDivergence"
!**      else if( bcType.eq.generalizedDivergence )then
  if( bcType.ne.generalizedDivergence ) then
    write(*,'("ERROR")') 
    stop 1146
  end if

 ! ***********************************
 ! ****** Generalized divergence *****
 ! ***********************************  
 !  to set the component along a to g:
 !       u <- u + (g-(a.u)) a/<a,a>
 !       g-(a.u) = b - ( discrete div of u )

 b1=par(0)
 b2=par(1)
 b3=par(2)

 n1=ipar(0)
 n2=ipar(1)
 n3=ipar(2)
 m1=ipar(3)
 m2=ipar(4)
 m3=ipar(5)
 if( gridType.eq.rectangular ) then 

   twoDeltaX = par(3)  ! 2.*dx[axis1]
   twoDeltaY = par(4)
   twoDeltaZ = par(5)

   if( bcOption.eq.scalarForcing )then
     generalizedDivergenceRectangular(scalarData)
   else if( bcOption.eq.gfForcing )then
     if( nd.eq.2 )then
       generalizedDivergenceRectangular(b1*gfData(i1,i2,i3,m1)+b2*gfData(i1,i2,i3,m2))
     else if( nd.eq.3 )then
       generalizedDivergenceRectangular(b1*gfData(i1,i2,i3,m1)+b2*gfData(i1,i2,i3,m2)+b3*gfData(i1,i2,i3,m3))
     else
       generalizedDivergenceRectangular(b1*gfData(i1,i2,i3,m1))
     end if
   else if( bcOption.eq.arrayForcing )then
     if( nd.eq.2 )then
       generalizedDivergenceRectangular(b1*fData(m1,side,axis,grid)+b2*fData(m2,side,axis,grid))
     else if( nd.eq.3 )then
       generalizedDivergenceRectangular(b1*fData(m1,side,axis,grid)+b2*fData(m2,side,axis,grid)+\
                                        b3*fData(m3,side,axis,grid))
     else
       generalizedDivergenceRectangular(b1*fData(m1,side,axis,grid))
     end if
   else if( bcOption.eq.vectorForcing )then
     if( nd.eq.2 )then
       generalizedDivergenceRectangular(b1*vData(m1)+b2*vData(m2))
     else if( nd.eq.3 )then
       generalizedDivergenceRectangular(b1*vData(m1)+b2*vData(m2)+b3*vData(m3))
     else
       generalizedDivergenceRectangular(b1*vData(m1))
     end if
   end if

 else

!         **** Curvilinear generalized divergence ****

   if( bcOption.eq.scalarForcing )then
     if( nd.eq.2 )then
       generalizedDivergenceCurvilinear2D(scalarData)
     else if( nd.eq.3 )then
       generalizedDivergenceCurvilinear3D(scalarData)
     else
       generalizedDivergenceCurvilinear1D(scalarData)
     end if
   else if( bcOption.eq.gfForcing )then
     if( nd.eq.2 )then
       generalizedDivergenceCurvilinear2D(b1*gfData(i1,i2,i3,m1)+b2*gfData(i1,i2,i3,m2))
     else if( nd.eq.3 )then
       generalizedDivergenceCurvilinear3D(b1*gfData(i1,i2,i3,m1)+b2*gfData(i1,i2,i3,m2)+b3*gfData(i1,i2,i3,m3))
     else
       generalizedDivergenceCurvilinear1D(b1*gfData(i1,i2,i3,m1))
     end if
   else if( bcOption.eq.arrayForcing )then
     if( nd.eq.2 )then
       generalizedDivergenceCurvilinear2D(b1*fData(m1,side,axis,grid)+b2*fData(m2,side,axis,grid))
     else if( nd.eq.3 )then
       generalizedDivergenceCurvilinear3D(b1*fData(m1,side,axis,grid)+b2*fData(m2,side,axis,grid)+\
                                          b3*fData(m3,side,axis,grid))
     else
       generalizedDivergenceCurvilinear1D(b1*fData(m1,side,axis,grid))
     end if
   else if( bcOption.eq.vectorForcing )then
     if( nd.eq.2 )then
       generalizedDivergenceCurvilinear2D(b1*vData(m1)+b2*vData(m2))
     else if( nd.eq.3 )then
       generalizedDivergenceCurvilinear3D(b1*vData(m1)+b2*vData(m2)+b3*vData(m3))
     else
       generalizedDivergenceCurvilinear1D(b1*vData(m1))
     end if
   end if

 end if

#Elif #BCOPT eq "normalDerivative"
   if( .not.(bcType.eq.normalDerOfNormalComponent .or.\
             bcType.eq.normalDerOfTangentialComponent0 .or.\
             bcType.eq.normalDerOfTangentialComponent1) )then
    write(*,'("ERROR")') 
    stop 1147
   end if

!**      else if( bcType.eq.normalDerOfNormalComponent .or.
!**     &         bcType.eq.normalDerOfTangentialComponent0 .or.
!**     &         bcType.eq.normalDerOfTangentialComponent1 )then

!       **************************************************
!       **** normal derivative of a vector dot u *********
!       **************************************************

  twoDeltaX=par(0)

  n1=ipar(0)
  n2=ipar(1)
  n3=ipar(2)
  m1=ipar(3)
  m2=ipar(4)
  m3=ipar(5)
  v0=ipar(6)
  v1=ipar(7)
  v2=ipar(8)

  if( gridType.eq.rectangular ) then 

    vv(0)=0. 
    vv(1)=0. 
    vv(2)=0.
    if( bcType.eq.normalDerOfNormalComponent )then
      vv(axis)=2*side-1  ! outward normal
    else if( bcType.eq.normalDerOfTangentialComponent0 ) then
      ! The tangent is just xr, xs or xt
      axisp1=mod(axis+1,nd)
      vv(axisp1)=1. 
    else if( bcType.eq.normalDerOfTangentialComponent1 ) then
      axisp2=mod(axis+2,nd)
      vv(axisp2)=1. 
    else
      stop 1253
    end if

    if( bcOption.eq.scalarForcing ) then
      if( nd.eq.2 ) then
      loopsd4(temp=scalarData*twoDeltaX+\
         (u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1))*vv(0) \
        +(u(i1+ip1,i2+ip2,i3,n2)-u(i1+im1,i2+im2,i3,n2))*vv(1), \
           u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*vv(0), \
           u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*vv(1), )
      else if( nd.eq.3 ) then
      loopsd4(temp=scalarData*twoDeltaX+\
         (u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1))*vv(0) \
        +(u(i1+ip1,i2+ip2,i3+ip3,n2)-u(i1+im1,i2+im2,i3+im3,n2))*vv(1) \
        +(u(i1+ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+im2,i3+im3,n3))*vv(2),\
           u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*vv(0), \
           u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*vv(1), \
           u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*vv(2)) 
      else
      loopsd4(temp=scalarData*twoDeltaX+\
         (u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1))*vv(0), \
           u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*vv(0), , )
      end if
    else if( bcOption.eq.gfForcing ) then
      normalDerivativeRectangular(gfData(i1,i2,i3,m1),gfData(i1,i2,i3,m2),gfData(i1,i2,i3,m3))
    else if( bcOption.eq.arrayForcing ) then
      normalDerivativeRectangular(fData(m1,side,axis,grid),fData(m2,side,axis,grid),fData(m3,side,axis,grid))
    else if( bcOption.eq.vectorForcing ) then
      normalDerivativeRectangular(vData(m1),vData(m2),vData(m3))
    end if

  else
!            ******* curvilinear normal derivative ******


   if( nd.eq.2 )then
     m21=1
     m12=3
     m22=4
     m32=5
     m23=7
     if( axis.eq.0 )then
       mGhost=m22+2*side-1
     else
       mGhost=m22+3*(2*side-1)
     end if
   else if( nd.eq.3 )then
     m221=4
     m212=10
     m122=12
     m222=13
     m322=14
     m232=16
     m223=22
     if( axis.eq.0 )then
       mGhost=m222+2*side-1
     else if( axis.eq.1 )then
       mGhost=m222+3*(2*side-1)
     else
       mGhost=m222+9*(2*side-1)
     end if
   else
     m12=0
     m22=1
     m32=2
     mGhost=m22+2*side-1
   end if

   if( bcOption.eq.scalarForcing ) then
     normalDerivativeCurvilinear(scalarData)
   else if( bcOption.eq.gfForcing ) then
     if( nd.eq.2 ) then
       normalDerivativeCurvilinear(gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(i1,i2,i3,m2)*v(i1,i2,i3,v1))
     else if( nd.eq.3 ) then
       normalDerivativeCurvilinear(gfData(i1,i2,i3,m1)*v(i1,i2,i3,v0)+gfData(i1,i2,i3,m2)*v(i1,i2,i3,v1)+\
                                   gfData(i1,i2,i3,m3)*v(i1,i2,i3,v2))
     else
       stop 12345
     end if
   else if( bcOption.eq.arrayForcing ) then
     if( nd.eq.2 ) then
       normalDerivativeCurvilinear(fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+\
               fData(m2,side,axis,grid)*v(i1,i2,i3,v1))
    else if( nd.eq.3 ) then
       normalDerivativeCurvilinear(fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+\
               fData(m2,side,axis,grid)*v(i1,i2,i3,v1)+fData(m3,side,axis,grid)*v(i1,i2,i3,v2))
     else
       stop 12345
    end if
   else if( bcOption.eq.vectorForcing ) then
     if( nd.eq.2 ) then
       normalDerivativeCurvilinear(vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,i3,v1))
     else if( nd.eq.3 ) then
       normalDerivativeCurvilinear(vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,i3,v1)+vData(m3)*v(i1,i2,i3,v2))
     else
       stop 12345
     end if
   end if


 end if

#Elif #BCOPT eq "normalComponent"

!**      else if( bcType.eq.normalComponent )then
   if( bcType.ne.normalComponent )then
    write(*,'("ERROR")') 
    stop 1148
   end if

!        ****************************
!        ***** Normal Component *****
!        ****************************

 ! **** check these ***
 n1=ipar(0)  ! components are (n1,n2,n3)
 n2=ipar(1)
 n3=ipar(2)
 m1=ipar(3)  ! use RHS values (m1,m2,m3)
 m2=ipar(4)
 m3=ipar(5)

 nsign=2*side-1  ! sign to convert RHS to outward normal component
 
 if( gridType.eq.rectangular )then   
!          *************************
!          *** rectangular grid  ***
!          *************************
   cn=n1+axis ! This is the normal component for a rectangular grid
   cm=m1+axis ! normal component for forcing arrays

   if( bcOption.eq.scalarForcing )then
     if( scalarData.eq.0. )then
       loopsd(u(i1,i2,i3,cn)=0.)
     else
       loopsd(u(i1,i2,i3,cn)=scalarData*nsign)
     end if
   else if( bcOption.eq.gfForcing )then
     ! NOTE: There is no *nsign here since we set nsign*u = nsign*gfData *wdh* 040317
     loopsd(u(i1,i2,i3,cn)=gfData(i1,i2,i3,cm))
   else if( bcOption.eq.arrayForcing )then
     loopsd(u(i1,i2,i3,cn)=fData(cm,side,axis,grid))
   else if( bcOption.eq.vectorForcing )then
     loopsd(u(i1,i2,i3,cn)=vData(cm))
   else
     write(*,*) 'assignBC:ERROR unknown bcOption=',bcOption
     stop 2
   end if

 else 
!          *************************
!          **** Curvilinear case ***
!          *************************


   if( bcOption.eq.scalarForcing )then
    if( scalarData.eq.0. )then
     assignNormalComponent(NO_FORCING)
    else
     assignNormalComponent(SCALAR_FORCING)
    end if
   else if( bcOption.eq.gfForcing )then
    assignNormalComponent(GF_FORCING)
   else if( bcOption.eq.arrayForcing )then
    assignNormalComponent(ARRAY_FORCING)
   else if( bcOption.eq.vectorForcing )then
    assignNormalComponent(VECTOR_FORCING)
   end if

 end if

#Elif #BCOPT eq "tangentialComponent"

 if( bcType.ne.tangentialComponent .and. bcType.ne.tangentialComponent0 .and. bcType.ne.tangentialComponent1 )then
   write(*,'("assignBC: tangentialComponent: ERROR")') 
   stop 1149
 end if

 if( bcType.eq.tangentialComponent0 .or. bcType.eq.tangentialComponent1 )then
   write(*,'("assignBC: tangentialComponent: Finish me")')
   stop 1150 
 end if

 !  ********************************
 !  ***** Tangential Component *****
 !  ********************************

 n1=ipar(0)  ! components are (n1,n2,n3)
 n2=ipar(1)
 n3=ipar(2)
 m1=ipar(3)  ! use RHS values (m1,m2,m3)
 m2=ipar(4)
 m3=ipar(5)

 nsign=2*side-1  ! sign to convert RHS to outward tangential component
 
 ! we assume n2=n1+1 and n3=n2+1

 if( gridType.eq.rectangular )then   
!          *************************
!          *** rectangular grid  ***
!          *************************
   cn=n1+axis ! This is the normal component for a rectangular grid
   cm=m1+axis ! normal component for forcing arrays

   ct1=n1 + mod( axis+1,nd)   ! This is the first tangential component for a rectangular grid
   ct2=n1 + mod( axis+2,nd)   ! This is the second tangential component for a rectangular grid

   ! write(*,'("assignBC: tangentialComponent: cn,ct1,ct2=",3i3)') cn,ct1,ct2

   if( bcOption.eq.scalarForcing )then
     if( scalarData.eq.0. )then
       if( nd.eq.2 )then
         loopsd(u(i1,i2,i3,ct1)=0.)
       else if( nd.eq.3 )then
         loopsd4(u(i1,i2,i3,ct1)=0.,u(i1,i2,i3,ct2)=0.,,)
       end if
     else
       ! What should we do for scalar data?? Just copy what was done in tangentialComponent.C: 
       if( nd.eq.2 )then
         loopsd(u(i1,i2,i3,ct1)=scalarData)
       else if( nd.eq.3 )then
         loopsd4(u(i1,i2,i3,ct1)=scalarData,u(i1,i2,i3,ct2)=scalarData,,)
       end if
     end if
   else if( bcOption.eq.gfForcing )then

    write(*,'("assignBC: tangentialComponent: bcOption==gfForcing not implemented yet. Finish me")')
    stop 1151

   else if( bcOption.eq.vectorForcing )then

    write(*,'("assignBC: tangentialComponent: bcOption==vectorForcing implemented yet. Finish me")')
    stop 1152

   else
     write(*,*) 'assignBC:ERROR unknown bcOption=',bcOption
     stop 2
   end if

 else 
!          *************************
!          **** Curvilinear case ***
!          *************************

   if( bcOption.eq.scalarForcing )then
    if( scalarData.eq.0. )then
     assignTangentialComponent(NO_FORCING)
    else
     ! assignNormalComponent(SCALAR_FORCING)
      write(*,'("assignBC: tangentialComponent: bcOption==scalarForcing not implemented yet. Finish me")')
      stop 1154
    end if
   else if( bcOption.eq.gfForcing )then
    ! assignNormalComponent(GF_FORCING)
    write(*,'("assignBC: tangentialComponent: bcOption==gfForcing not implemented yet. Finish me")')
    stop 1155
   else if( bcOption.eq.arrayForcing )then
    ! assignNormalComponent(ARRAY_FORCING)
    write(*,'("assignBC: tangentialComponent: bcOption==arrayForcing not implemented yet. Finish me")')
    stop 11565
   else if( bcOption.eq.vectorForcing )then
    ! assignNormalComponent(VECTOR_FORCING)
    write(*,'("assignBC: tangentialComponent: bcOption==vectorForcing not implemented yet. Finish me")')
    stop 1157
   end if

 end if

#Else
  write(*,'("ERROR:assignBoundaryConditions.bf")')
#End

 return 
 end
#endMacro

#beginMacro buildFile(x,y)
#beginFile x ## .f
 assignBoundaryConditionMacro(x,y)
#endFile
#endMacro


      buildFile(assignOptNeumann,neumann)
      buildFile(assignOptGenDiv,generalizedDivergence)
      buildFile(assignOptNormalDer,normalDerivative)
      buildFile(assignOptNormalComponent,normalComponent)
      buildFile(assignOptADotGradU,aDotGradU)
      buildFile(assignOptTangentialComponent,tangentialComponent)

#beginMacro loops1(expression)
do c=ca,cb
do i1=n1a,n1b
do i3=n3a,n3b
do i2=n2a,n2b
  expression
end do
end do
end do
end do
#endMacro

#beginMacro loops2(expression)
do c=ca,cb
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b
  expression
end do
end do
end do
end do
#endMacro


      subroutine periodicUpdateOpt(nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & u,ca,cb, indexRange, gridIndexRange, dimension, 
     & isPeriodic )
!======================================================================
!  Optimised Boundary Conditions
!         
! nd : number of space dimensions
! ca,cb : assign these components
! useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
!======================================================================
      implicit none
      integer nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b

      integer isPeriodic(0:2),indexRange(0:1,0:2)
      integer gridIndexRange(0:1,0:2),dimension(0:1,0:2)

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      integer ca,cb

!     --- local variables 
      integer c,i1,i2,i3,axis,diff
      integer n1a,n1b, n2a,n2b, n3a,n3b


      n1a=dimension(0,0)
      n1b=dimension(1,0)
      n2a=dimension(0,1)
      n2b=dimension(1,1)
      n3a=dimension(0,2)
      n3b=dimension(1,2)

      do axis=0,nd-1
        if( isPeriodic(axis).ne.0 )then
!         length of the period:
          diff=gridIndexRange(1,axis)-gridIndexRange(0,axis)
!         assign all ghost points on "left"
!         I[i]=Range(dimension(Start,axis),indexRange(Start,axis)-1);
!         u(I[0],I[1],I[2],I[3])=u(I[0]+diff[0],I[1]+diff[1],I[2]+diff[2],I[3]+diff[3]);
!         // assign all ghost points on "right"
!         I[i]=Range(indexRange(End,axis)+1,dimension(End,axis));
!         u(I[0],I[1],I[2],I[3])=u(I[0]-diff[0],I[1]-diff[1],I[2]-diff[2],I[3]-diff[3]);
          
          if( axis.eq.0 )then
            n1a=dimension(0,0)
            n1b=indexRange(0,0)-1
            loops1(u(i1,i2,i3,c)=u(i1+diff,i2,i3,c))
            n1a=indexRange(1,0)+1
            n1b=dimension(1,0)
            loops1(u(i1,i2,i3,c)=u(i1-diff,i2,i3,c))
            n1a=dimension(0,0)
            n1b=dimension(1,0)
          else if( axis.eq.1 )then
            n2a=dimension(0,1)
            n2b=indexRange(0,1)-1
            loops2(u(i1,i2,i3,c)=u(i1,i2+diff,i3,c))
            n2a=indexRange(1,1)+1
            n2b=dimension(1,1)
            loops2(u(i1,i2,i3,c)=u(i1,i2-diff,i3,c))
            n2a=dimension(0,1)
            n2b=dimension(1,1)
          else
            n3a=dimension(0,2)
            n3b=indexRange(0,2)-1
            loops2(u(i1,i2,i3,c)=u(i1,i2,i3+diff,c))
            n3a=indexRange(1,2)+1
            n3b=dimension(1,2)
            loops2(u(i1,i2,i3,c)=u(i1,i2,i3-diff,c))
            n3a=dimension(0,2)
            n3b=dimension(1,2)
          end if

        end if
      end do
      return
      end
      
