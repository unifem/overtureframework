c You should preprocess this file with the bpp preprocessor before compiling.


c loops: use mask(i1,i2,i3)
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

c loopsm: use mask(i1+im1,i2+im2,i3+im3)
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

#beginMacro neumannLoops2D(rhs)
loopsm(u(i1+im1,i2+im2,i3+im3,c)=( \
 rhs - ( \
  coeff(m21,i1,i2,i3)*u(i1  ,i2-1,i3,c) \
 +coeff(m12,i1,i2,i3)*u(i1-1,i2  ,i3,c) \
 +coeff(m22,i1,i2,i3)*u(i1  ,i2  ,i3,c) \
 +coeff(m32,i1,i2,i3)*u(i1+1,i2  ,i3,c) \
 +coeff(m23,i1,i2,i3)*u(i1  ,i2+1,i3,c) \
 ))/coeff(mGhost,i1,i2,i3) )
#endMacro


#beginMacro neumannLoops3D(rhs)
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

#beginMacro generalizedDivergenceRectangular(rhs)
if( nd.eq.1 )then
  loopsd(u(i1+im1,i2,i3,n1)=u(i1+ip1,i2,i3,n1) +((2*side-1)*twoDeltaX/b1)*( rhs ))
else if( nd.eq.2 ) then
  if( axis.eq.0 ) then
    loopsd(u(i1+im1,i2+im2,i3,n1)=u(i1+ip1,i2+ip2,i3,n1) +((2*side-1)*twoDeltaX/b1)*\
      ( rhs - (u(i1,i2+1,i3,n2)-u(i1,i2-1,i3,n2))*(b2/twoDeltaY) ) )
  else
    loopsd( u(i1+im1,i2+im2,i3,n2)=u(i1+ip1,i2+ip2,i3,n2) +((2*side-1)*twoDeltaY/b2)*\
     ( rhs-(u(i1+1,i2,i3,n1)-u(i1-1,i2,i3,n1))*(b1/twoDeltaX) ) )
  end if
else
  if( axis.eq.0 ) then
    loopsd( u(i1+im1,i2+im2,i3+im3,n1)=u(i1+ip1,i2+ip2,i3+ip3,n1)+((2*side-1)*twoDeltaX/b1)*\
      ( rhs - (u(i1  ,i2+1,i3  ,n2)-u(i1  ,i2-1,i3  ,n2))*(b2/twoDeltaY) \
                      - (u(i1  ,i2  ,i3+1,n3)-u(i1  ,i2  ,i3-1,n3))*(b3/twoDeltaZ) ) )
  else if( axis.eq.1 )then
    loopsd( u(i1+im1,i2+im2,i3+im3,n2)=u(i1+ip1,i2+ip2,i3+ip3,n2) +((2*side-1)*twoDeltaY/b2)* \
      ( rhs - (u(i1+1,i2  ,i3  ,n1)-u(i1-1,i2  ,i3  ,n1))*(b1/twoDeltaX)\
                      - (u(i1  ,i2  ,i3+1,n3)-u(i1  ,i2  ,i3-1,n3))*(b3/twoDeltaZ) ) )
  else
    loopsd( u(i1+im1,i2+im2,i3+im3,n3)=u(i1+ip1,i2+ip2,i3+ip3,n3) +((2*side-1)*twoDeltaZ/b3)*\
      ( rhs - (u(i1+1,i2  ,i3  ,n1)-u(i1-1,i2  ,i3  ,n1))*(b1/twoDeltaX)\
                      - (u(i1  ,i2+1,i3  ,n2)-u(i1  ,i2-1,i3  ,n2))*(b2/twoDeltaY) ) )
  end if
end if
#endMacro

#beginMacro generalizedDivergenceCurvilinearLoops1D(rhs)
  loopsd(u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+b1*coeff(i1,i2,i3,0)*(rhs))
#endMacro
#beginMacro generalizedDivergenceCurvilinearLoops2D(rhs)
  loopsd4(temp=rhs,\
          u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+b1*coeff(i1,i2,i3,0)*(temp), \
          u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+b2*coeff(i1,i2,i3,1)*(temp), )
#endMacro
#beginMacro generalizedDivergenceCurvilinearLoops3D(rhs)
  loopsd4(temp=rhs,\
          u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+b1*coeff(i1,i2,i3,0)*(temp), \
          u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+b2*coeff(i1,i2,i3,1)*(temp), \
          u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+b3*coeff(i1,i2,i3,2)*(temp))
#endMacro


c   v(i1,i2,i3,n1) = ux(i1,i2,i3,n1) 
c   v(i1,i2,i3,n2) = uy(i1,i2,i3,n2) 
c   v(i1,i2,i3,n3) = uz(i1,i2,i3,n3) 
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

#beginMacro normalDerivativeRectangular(rhs1,rhs2,rhs3)
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



      subroutine assignBoundaryConditions( nd, 
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,v,coeff, mask,
     & scalarData,gfData,fData,vData, 
     & dx,dr,ipar,par, ca,cb,uC,fC,
     & side,axis,grid, bcType,bcOption,gridType,order,useWhereMask,
     & lineForForcing )    
c======================================================================
c  Optimised Boundary Conditions
c         
c nd : number of space dimensions
c uC(0:*) : defines components to assign
c ca,cb : assign components c=uC(ca),..,uC(cb)
c fC(0;*) : defines components of forcing to use.
c gridType: 0=rectangular, 1=non-rectangular
c order : 2 or 4 -- only 2 implemented
c par, ipar : real and integer parameters
c useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
c lineForForcing : if 0 evaluate the forcing gfData on the boundary, if 1 evaluate forcing
c    on the first ghost line, etc.
c
c v : for generalizedDiveregence v holds ux(i1,i2,i3,n1) uy(i1,i2,i3,n2) uz(i1,i2,i3,n3)
c======================================================================
      implicit none
      integer nd, n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndc1a,ndc1b,ndc2a,ndc2b,ndc3a,ndc3b,ndc4a,ndc4b,
     & ndg1a,ndg1b,ndg2a,ndg2b,ndg3a,ndg3b,ndg4a,ndg4b,
     & ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b,ndf4a,ndf4b,
     & ndw1a,ndw1b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b

      integer side,axis,grid, bcType,bcOption,gridType,order,
     &  useWhereMask,lineForForcing

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real v(ndv1a:ndv1b,ndv2a:ndv2b,ndv3a:ndv3b,ndv4a:ndv4b)
      real coeff(ndc1a:ndc1b,ndc2a:ndc2b,ndc3a:ndc3b,ndc4a:ndc4b)
      real gfData(ndg1a:ndg1b,ndg2a:ndg2b,ndg3a:ndg3b,ndg4a:ndg4b)
      real fData(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b,ndf4a:ndf4b)
      real vData(ndw1a:ndw1b)
      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)

      integer ipar(*), ca,cb,uC(0:*),fC(0:*)
      real scalarData,par(*),dx(3),dr(3)
      
c      real h21(3),d22(3),d12(3),h22(3)
c      real d24(3),d14(3),h42(3),h41(3)


c     --- boundary conditions (from BCTypes.h) ---
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
      parameter(dirichlet=0,neumann=1,extrapolate=2,normalComponent=3,
     & mixed=4,generalMixedDerivative=5,
     & normalDerOfNormalComponent=6,
     & generalizedDivergence=12,
     & normalDerOfTangentialComponent0=16,
     & normalDerOfTangentialComponent1=17,
     & extrapolateNormalComponent=20,extrapolateTangentialComponent0=21,
     & extrapolateTangentialComponent1=22 )

c     --- grid types
      integer rectangular,curvilinear
      parameter( rectangular=0,curvilinear=1 )
    
c     --- forcing types ---
      integer  scalarForcing,gfForcing,arrayForcing,vectorForcing
      parameter( scalarForcing=0,gfForcing=1,arrayForcing=2,
     &           vectorForcing=3 )

c     --- local variables ----
      real b0,b1,b2,b3,twoDeltaX,twoDeltaY,twoDeltaZ,temp
      integer c,c0,f,i1,i2,i3,im1,im2,im3,ip1,ip2,ip3,if1,if2,if3
      integer n1,n2,n3,m1,m2,m3,v0,v1,v2
    
      integer mGhost
      integer m21,m12,m22,m32,m23
      integer m221,m212,m122,m222,m322,m232,m223


      if( side.lt.0 .or. side.gt.1 )then
        write(*,*) 'applyBoundaryConditions:ERROR: side=',side
        stop 1
      end if
      if( axis.lt.0 .or. axis.ge.nd )then
        write(*,*) 'applyBoundaryConditions:ERROR: axise=',axis
        stop 2
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


      if( bcType.eq.neumann .or. bcType.eq.mixed )then
c        *******************
c        ***** Neumann *****
c        *******************
        b0=par(1)
        b1=par(2)
        twoDeltaX=par(3)



        if( gridType.eq.rectangular )then   
c          *************************
c          *** rectangular grid  ***
c          *************************
          if( bcType.eq.neumann .or. b0.eq.0. )then
c            *** neumann ***
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
              stop 2
            end if
          else
c           *** mixed ***

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
          end if

        else 
c          *************************
c          **** Curvilinear case ***
c          *************************
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


      else if( bcType.eq.generalizedDivergence )then
        ! ***********************************
        ! ****** Generalized divergence *****
        ! ***********************************  
        !  to set the component along a to g:
        !       u <- u + (g-(a.u)) a/<a,a>
        !       g-(a.u) = b - ( discrete div of u )

        b1=par(1)
        b2=par(2)
        b3=par(3)
        n1=ipar(1)
        n2=ipar(2)
        n3=ipar(3)
        m1=ipar(4)
        m2=ipar(5)
        m3=ipar(6)
        if( gridType.eq.rectangular ) then 

          twoDeltaX = par(4)  ! 2.*dx[axis1]
          twoDeltaY = par(5)
          twoDeltaZ = par(6)

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

c         **** Curvilinear generalized divergence ****

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

      else if( bcType.eq.normalDerOfNormalComponent .or.
     &         bcType.eq.normalDerOfTangentialComponent0 .or.
     &         bcType.eq.normalDerOfTangentialComponent1 )then

c       **************************************************
c       **** normal derivative of a vector dot u *********
c       **************************************************

        twoDeltaX=par(1)

        n1=ipar(1)
        n2=ipar(2)
        n3=ipar(3)
        m1=ipar(4)
        m2=ipar(5)
        m3=ipar(6)
        v0=ipar(7)
        v1=ipar(8)
        v2=ipar(9)

        if( gridType.eq.rectangular ) then 

          if( bcOption.eq.scalarForcing ) then
            if( nd.eq.2 ) then
            loopsd4(temp=scalarData*twoDeltaX+\
               (u(i1+ip1,i2+ip2,i3,n1)-u(i1+im1,i2+im2,i3,n1))*v(i1,i2,i3,v0) \
              +(u(i1+ip1,i2+ip2,i3,n2)-u(i1+im1,i2+im2,i3,n2))*v(i1,i2,i3,v1), \
                 u(i1+im1,i2+im2,i3,n1)=u(i1+im1,i2+im2,i3,n1)+temp*v(i1,i2,i3,v0), \
                 u(i1+im1,i2+im2,i3,n2)=u(i1+im1,i2+im2,i3,n2)+temp*v(i1,i2,i3,v1), )
            else if( nd.eq.3 ) then
            loopsd4(temp=scalarData*twoDeltaX+\
               (u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1))*v(i1,i2,i3,v0) \
              +(u(i1+ip1,i2+ip2,i3+ip3,n2)-u(i1+im1,i2+im2,i3+im3,n2))*v(i1,i2,i3,v1) \
              +(u(i1+ip1,i2+ip2,i3+ip3,n3)-u(i1+im1,i2+im2,i3+im3,n3))*v(i1,i2,i3,v2),\
                 u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), \
                 u(i1+im1,i2+im2,i3+im3,n2)=u(i1+im1,i2+im2,i3+im3,n2)+temp*v(i1,i2,i3,v1), \
                 u(i1+im1,i2+im2,i3+im3,n3)=u(i1+im1,i2+im2,i3+im3,n3)+temp*v(i1,i2,i3,v2)) 
            else
            loopsd4(temp=scalarData*twoDeltaX+\
               (u(i1+ip1,i2+ip2,i3+ip3,n1)-u(i1+im1,i2+im2,i3+im3,n1))*v(i1,i2,i3,v0), \
                 u(i1+im1,i2+im2,i3+im3,n1)=u(i1+im1,i2+im2,i3+im3,n1)+temp*v(i1,i2,i3,v0), , )
            end if
          else if( bcOption.eq.gfForcing ) then
            normalDerivativeRectangular(gfData(i1,i2,i3,m1),gfData(i1,i2,i3,m2),gfData(i1,i2,i3,m3))
          else if( bcOption.eq.arrayForcing ) then
            normalDerivativeRectangular(fData(m1,side,axis,grid),fData(m2,side,axis,grid),fData(m3,side,axis,grid))
          else if( bcOption.eq.vectorForcing ) then
            normalDerivativeRectangular(vData(m1),vData(m2),vData(m3))
          end if

        else
c            ******* curvilinear normal derivative ******


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
              stop 123456
            end if
          else if( bcOption.eq.arrayForcing ) then
            if( nd.eq.2 ) then
              normalDerivativeCurvilinear(fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+\
                      fData(m2,side,axis,grid)*v(i1,i2,i3,v1))
           else if( nd.eq.3 ) then
              normalDerivativeCurvilinear(fData(m1,side,axis,grid)*v(i1,i2,i3,v0)+\
                      fData(m2,side,axis,grid)*v(i1,i2,i3,v1)+fData(m3,side,axis,grid)*v(i1,i2,i3,v2))
            else
              stop 123456
           end if
          else if( bcOption.eq.vectorForcing ) then
            if( nd.eq.2 ) then
              normalDerivativeCurvilinear(vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,i3,v1))
            else if( nd.eq.3 ) then
              normalDerivativeCurvilinear(vData(m1)*v(i1,i2,i3,v0)+vData(m2)*v(i1,i2,i3,v1)+vData(m3)*v(i1,i2,i3,v2))
            else
              stop 123456
            end if
          end if


        end if

      else
        write(*,*) 'assignBoundaryConditions:ERROR unknown bcType=',
     &    bcType
        stop 3
      end if

      return 
      end

#beginMacro beginLoopsWithMask()
  do c=ca,cb
    do i3=n3a,n3b,n3c
      do i2=n2a,n2b,n2c
        do i1=n1a,n1b,n1c
          if( mask(i1,i2,i3).ne.0 )then
#endMacro
#beginMacro endLoopsWithMask()
          end if
        end do
      end do
    end do
  end do
#endMacro

#beginMacro beginLoops()
  do c=ca,cb
    do i3=n3a,n3b,n3c
      do i2=n2a,n2b,n2c
        do i1=n1a,n1b,n1c
#endMacro
#beginMacro endLoops()
        end do
      end do
    end do
  end do
#endMacro

#beginMacro loops(expression)
if( useWhereMask.ne.0 )then
  do c=ca,cb
    do i3=n3a,n3b,n3c
      do i2=n2a,n2b,n2c
        do i1=n1a,n1b,n1c
          if( mask(i1,i2,i3).ne.0 )then
            expression
          end if
        end do
      end do
    end do
  end do
else
  do c=ca,cb
    do i3=n3a,n3b,n3c
      do i2=n2a,n2b,n2c
        do i1=n1a,n1b,n1c
          expression
        end do
      end do
    end do
  end do
end if
#endMacro

#beginMacro loops(expression)
if( useWhereMask.ne.0 )then
  do c=ca,cb
    do i3=n3a,n3b,n3c
      do i2=n2a,n2b,n2c
        do i1=n1a,n1b,n1c
          if( mask(i1,i2,i3).ne.0 )then
            expression
          end if
        end do
      end do
    end do
  end do
else
  do c=ca,cb
    do i3=n3a,n3b,n3c
      do i2=n2a,n2b,n2c
        do i1=n1a,n1b,n1c
          expression
        end do
      end do
    end do
  end do
end if
#endMacro

#beginMacro loopse8(e1,e2,e3,e4,e5,e6,e7,e8)
if( useWhereMask.ne.0 )then
  do c=ca,cb
    do i3=n3a,n3b,n3c
      do i2=n2a,n2b,n2c
        do i1=n1a,n1b,n1c
          if( mask(i1,i2,i3).ne.0 )then
            e1
            e2
            e3
            e4
            e5
            e6
            e7
            e8
          end if
        end do
      end do
    end do
  end do
else
  do c=ca,cb
    do i3=n3a,n3b,n3c
      do i2=n2a,n2b,n2c
        do i1=n1a,n1b,n1c
            e1
            e2
            e3
            e4
            e5
            e6
            e7
            e8
        end do
      end do
    end do
  end do
end if
#endMacro

#beginMacro evenSymmetryBCMacro()
 do m3=ng3a,ng3b
 do m2=ng2a,ng2b
 do m1=ng1a,ng1b
   u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=u(i1+m1*is1,i2+m2*is2,i3+m3*is3,c)
 end do
 end do
 end do
#endMacro 

#beginMacro oddSymmetryBCMacro()
 do m3=ng3a,ng3b
 do m2=ng2a,ng2b
 do m1=ng1a,ng1b
   u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=2.*u(i1,i2,i3,c)-u(i1+m1*is1,i2+m2*is2,i3+m3*is3,c)
 end do
 end do
 end do
#endMacro

c ================================================================================================
c  /Description:
c     Apply an extrapolation or symmetry boundary condition.
c  /i1,i2,i3,n: Index;'s of points to assign.
c ===============================================================================================
#beginMacro assignCorners(side1,side2,side3)

if( cornerBC(side1,side2,side3).eq.extrapolateCorner )then

  if( is1.eq.0 )then
   n1a=indexRange(0,axis1)
   n1b=indexRange(1,axis1)
   n1c=1
  else
   ! loop from inside to outside
   n1a=indexRange(side1,axis1)-is1
   n1b= dimension(side1,axis1)
   n1c=-is1
  end if

  if( is2.eq.0 )then
   n2a=indexRange(0,axis2)
   n2b=indexRange(1,axis2)
   n2c=1
  else
   ! loop from inside to outside
   n2a=indexRange(side2,axis2)-is2
   n2b= dimension(side2,axis2)
   n2c=-is2
  end if

  if( is3.eq.0 )then
   n3a=indexRange(0,axis3)
   n3b=indexRange(1,axis3)
   n3c=1
  else
   ! loop from inside to outside
   n3a=indexRange(side3,axis3)-is3
   n3b= dimension(side3,axis3)
   n3c=-is3
  end if

  js1=is1
  js2=is2
  js3=is3
  if( cornerExtrapolationOption.ne.0 )then
    ! Use this option to avoid extrapolating corners along a diagonal
    if( cornerExtrapolationOption.eq.1 )then
      js1=0  ! do not extrap along axis1
    else if( cornerExtrapolationOption.eq.2 )then
      js2=0  ! do not extrap along axis2
    else if( cornerExtrapolationOption.eq.3 )then
      js3=0  ! do not extrap along axis3
    end if
  end if
  if( js1.eq.0 .and. js2.eq.0 .and. js3.eq.0  )then
    write(*,'(''ERROR: extrapolating corners js1.eq.0 .and. js2.eq.0 .and. js3.eq.0'')')
    stop 55
  end if

!  write(*,'(''side1,side2,side3, n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c='',12i4)') side1,side2,side3,n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c
  if( orderOfExtrapolation.eq.1 )then
    loops(u(i1,i2,i3,c)=u(i1+  (js1),i2+  (js2),i3+  (js3),c))
  else if( orderOfExtrapolation.eq.2 )then
    loops(u(i1,i2,i3,c)=2.*u(i1+  (js1),i2+  (js2),i3+  (js3),c) \
                      -    u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c))
  else if( orderOfExtrapolation.eq.3 )then
    loops(u(i1,i2,i3,c)= 3.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                       - 3.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                       +    u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c))
  else if( orderOfExtrapolation.eq.4 )then
    loops(u(i1,i2,i3,c)=4.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      - 6.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      + 4.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      -    u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c))
  else if( orderOfExtrapolation.eq.5 )then
    loops(u(i1,i2,i3,c)=5.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -10.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +10.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      - 5.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                      +    u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c))
  else if( orderOfExtrapolation.eq.6 )then
    loops(u(i1,i2,i3,c)=6.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -15.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +20.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      -15.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                      + 6.*u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c)  \
                      -    u(i1+6*(js1),i2+6*(js2),i3+6*(js3),c))
  else if( orderOfExtrapolation.eq.7 )then
    loops(u(i1,i2,i3,c)=7.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -21.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +35.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      -35.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                      +21.*u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c)  \
                      - 7.*u(i1+6*(js1),i2+6*(js2),i3+6*(js3),c)  \
                      +    u(i1+7*(js1),i2+7*(js2),i3+7*(js3),c))
  else if( orderOfExtrapolation.eq.8 )then
    loops(u(i1,i2,i3,c)=8.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -28.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +56.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                      -70.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                      +56.*u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c)  \
                      -28.*u(i1+6*(js1),i2+6*(js2),i3+6*(js3),c)  \
                      + 8.*u(i1+7*(js1),i2+7*(js2),i3+7*(js3),c)  \
                      -    u(i1+8*(js1),i2+8*(js2),i3+8*(js3),c))
  else if( orderOfExtrapolation.eq.9 )then
    loops(u(i1,i2,i3,c)=9.*u(i1+  (js1),i2+  (js2),i3+  (js3),c)  \
                      -36.*u(i1+2*(js1),i2+2*(js2),i3+2*(js3),c)  \
                      +84.*u(i1+3*(js1),i2+3*(js2),i3+3*(js3),c)  \
                     -126.*u(i1+4*(js1),i2+4*(js2),i3+4*(js3),c)  \
                     +126.*u(i1+5*(js1),i2+5*(js2),i3+5*(js3),c)  \
                      -84.*u(i1+6*(js1),i2+6*(js2),i3+6*(js3),c)  \
                      +36.*u(i1+7*(js1),i2+7*(js2),i3+7*(js3),c)  \
                      - 9.*u(i1+8*(js1),i2+8*(js2),i3+8*(js3),c)  \
                      +    u(i1+9*(js1),i2+9*(js2),i3+9*(js3),c))
  else 
    write(*,*) 'fixBoundaryCorners:Error: '
    write(*,*) 'unable to extrapolate '
    write(*,*) ' to orderOfExtrapolation',orderOfExtrapolation
    write(*,*) ' can only do orders 1 to 9.'
    stop 1
  end if

else
  if( is1.eq.0 )then
    n1a=indexRange(0,axis1)
    n1b=indexRange(1,axis1)
    n1c=1
    ng1a=0
    ng1b=0
  else
    n1a=indexRange(side1,axis1)
    n1b=n1a
    n1c=1
    ng1a=1
    ng1b=abs(indexRange(side1,axis1)-dimension(side1,axis1))
  end if
  if( is2.eq.0 )then
    n2a=indexRange(0,axis2)
    n2b=indexRange(1,axis2)
    n2c=1
    ng2a=0
    ng2b=0
  else
    n2a=indexRange(side2,axis2)
    n2b=n2a
    n2c=1
    ng2a=1
    ng2b=abs(indexRange(side2,axis2)-dimension(side2,axis2))
  end if
  if( is3.eq.0 )then
    n3a=indexRange(0,axis3)
    n3b=indexRange(1,axis3)
    n3c=1
    ng3a=0
    ng3b=0
  else
    n3a=indexRange(side3,axis3)
    n3b=n3a
    n3c=1
    ng3a=1
    ng3b=abs(indexRange(side3,axis3)-dimension(side3,axis3))
  end if



 if( cornerBC(side1,side2,side3).eq.evenSymmetryCorner .or. \
          cornerBC(side1,side2,side3).eq.symmetryCorner )then
   !  even symmetry boundary condition 

    loops($evenSymmetryBCMacro())

 else if( cornerBC(side1,side2,side3).eq.oddSymmetryCorner )then
   !  odd symmetry boundary condition 

   loops($oddSymmetryBCMacro())
 
 else if( cornerBC(side1,side2,side3).eq.taylor2ndOrderEvenCorner .or. \
          cornerBC(side1,side2,side3).eq.taylor2ndOrder )then
   !  Use a 2nd-order taylor approximation that preserves even symmetry if present

   write(*,'(" ****taylor2ndOrderEvenCorner...")') 

   if( nd.eq.2 )then
     if( ncg.eq.1 )then
       ! assign 1 line of corner ghost points
       loops(u(i1-is1,i2-is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,is1,is2,c))
     else if( ncg.eq.2 )then
       ! assign 2 lines of corner ghost points
       loopse8(\
         u(i1-  is1,i2-  is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,  is1,  is2,c),\
         u(i1-2*is1,i2-  is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,2*is1,  is2,c),\
         u(i1-  is1,i2-2*is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,  is1,2*is2,c),\
         u(i1-2*is1,i2-2*is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,2*is1,2*is2,c),,,,)
     else if( ncg.gt.2 )then
       ! general case : do all
       if( useWhereMask.ne.0 )then
        beginLoopsWithMask()
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           u(i1-m1*is1,i2-m2*is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,m1*is1,m2*is2,c)
         end do
         end do
        endLoopsWithMask()
       else
        beginLoops()
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           u(i1-m1*is1,i2-m2*is2,i3,c)=taylor2ndOrderEven2d(i1,i2,i3,m1*is1,m2*is2,c)
         end do
         end do
        endLoops()
       end if
     end if

   else if( nd.eq.3 )then

     if( ncg.eq.1 )then
       ! assign 1 line of corner ghost points
       loops(u(i1-is1,i2-is2,i3-is3,c)=taylor2ndOrderEven3d(i1,i2,i3,is1,is2,is3,c))
     else if( ncg.eq.2 )then
       ! assign 2 lines of corner ghost points
       if( is1.ne.0 .and. is2.ne.0 .and. is3.ne.0 )then
         loopse8(\
           u(i1-  is1,i2-  is2,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1-2*is1,i2-  is2,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,  is2,  is3,c),\
           u(i1-  is1,i2-2*is2,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,2*is2,  is3,c),\
           u(i1-2*is1,i2-2*is2,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,2*is2,  is3,c),\
           u(i1-  is1,i2-  is2,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,2*is3,c),\
           u(i1-2*is1,i2-  is2,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,  is2,2*is3,c),\
           u(i1-  is1,i2-2*is2,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,2*is2,2*is3,c),\
           u(i1-2*is1,i2-2*is2,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,2*is2,2*is3,c))
       else if( is1.ne.0 .and. is2.ne.0 )then
         loopse8(\
           u(i1-  is1,i2-  is2,i3      ,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1-2*is1,i2-  is2,i3      ,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,  is2,  is3,c),\
           u(i1-  is1,i2-2*is2,i3      ,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,2*is2,  is3,c),\
           u(i1-2*is1,i2-2*is2,i3      ,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,2*is2,  is3,c),,,,)
       else if( is1.ne.0 .and. is3.ne.0 )then
         loopse8(\
           u(i1-  is1,i2      ,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1-2*is1,i2      ,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,  is2,  is3,c),\
           u(i1-  is1,i2      ,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,2*is3,c),\
           u(i1-2*is1,i2      ,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,2*is1,  is2,2*is3,c),,,,)
       else if( is2.ne.0 .and. is3.ne.0 )then
         loopse8(\
           u(i1      ,i2-  is2,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1      ,i2-2*is2,i3-  is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,2*is2,  is3,c),\
           u(i1      ,i2-  is2,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,  is2,2*is3,c),\
           u(i1      ,i2-2*is2,i3-2*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,  is1,2*is2,2*is3,c),,,,)
       else
         write(*,*) 'ERROR: is1=is2=is3=0!'
         stop 21
       end if
     else if( ncg.gt.2 )then
       ! general case : do all
       if( useWhereMask.ne.0 )then
        beginLoopsWithMask()
         do m3=ng3a,ng3b
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,m1*is1,m2*is2,m3*is3,c)
         end do
         end do
         end do
        endLoopsWithMask()
       else
        beginLoops()
         do m3=ng3a,ng3b
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=taylor2ndOrderEven3d(i1,i2,i3,m1*is1,m2*is2,m3*is3,c)
         end do
         end do
         end do
        endLoops()
      end if
     end if
   end if

 else if( cornerBC(side1,side2,side3).eq.taylor4thOrderEvenCorner )then
   !  Use a 4th-order taylor approximation that preserves even symmetry if present

   write(*,'(" ****taylor4thOrderEvenCorner, is1,is2=",2i3,"...")') is1,is2

   if( nd.eq.2 )then
     if( ncg.eq.1 )then
       ! assign 1 line of corner ghost points
       loops(u(i1-is1,i2-is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,is1,is2,c))
     else if( ncg.eq.2 )then
       ! assign 2 lines of corner ghost points
       loopse8(\
         u(i1-  is1,i2-  is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,  is1,  is2,c),\
         u(i1-2*is1,i2-  is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,2*is1,  is2,c),\
         u(i1-  is1,i2-2*is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,  is1,2*is2,c),\
         u(i1-2*is1,i2-2*is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,2*is1,2*is2,c),,,,)
     else if( ncg.gt.2 )then
       ! general case : do all
       if( useWhereMask.ne.0 )then
        beginLoopsWithMask()
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           u(i1-m1*is1,i2-m2*is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,m1*is1,m2*is2,c)
         end do
         end do
        endLoopsWithMask()
       else
        beginLoops()
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           u(i1-m1*is1,i2-m2*is2,i3,c)=taylor4thOrderEven2d(i1,i2,i3,m1*is1,m2*is2,c)
         end do
         end do
        endLoops()
       end if
     end if

   else if( nd.eq.3 )then

     mmm(-is1,-is2,-is3)=0 ! for zeroing out the proper term in taylor4thOrderEven3dVertex

     if( ncg.eq.1 )then
       ! assign 1 line of corner ghost points
       loops(u(i1-is1,i2-is2,i3-is3,c)=taylor4thOrderEven3dVertex(i1,i2,i3,is1,is2,is3,c))
     else if( ncg.eq.2 )then
       ! assign 2 lines of corner ghost points
       if( is1.ne.0 .and. is2.ne.0 .and. is3.ne.0 )then
         loopse8(\
           u(i1-  is1,i2-  is2,i3-  is3,c)=taylor4thOrderEven3dVertex(i1,i2,i3,is1,is2,is3,c),\
           u(i1-2*is1,i2-  is2,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,  is2,  is3,c),\
           u(i1-  is1,i2-2*is2,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,2*is2,  is3,c),\
           u(i1-2*is1,i2-2*is2,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,2*is2,  is3,c),\
           u(i1-  is1,i2-  is2,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,  is2,2*is3,c),\
           u(i1-2*is1,i2-  is2,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,  is2,2*is3,c),\
           u(i1-  is1,i2-2*is2,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,2*is2,2*is3,c),\
           u(i1-2*is1,i2-2*is2,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,2*is2,2*is3,c))
       else if( is1.ne.0 .and. is2.ne.0 )then
         loopse8(\
           u(i1-  is1,i2-  is2,i3      ,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1-2*is1,i2-  is2,i3      ,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,  is2,  is3,c),\
           u(i1-  is1,i2-2*is2,i3      ,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,2*is2,  is3,c),\
           u(i1-2*is1,i2-2*is2,i3      ,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,2*is2,  is3,c),,,,)
       else if( is1.ne.0 .and. is3.ne.0 )then
         loopse8(\
           u(i1-  is1,i2      ,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1-2*is1,i2      ,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,  is2,  is3,c),\
           u(i1-  is1,i2      ,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,  is2,2*is3,c),\
           u(i1-2*is1,i2      ,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,2*is1,  is2,2*is3,c),,,,)
       else if( is2.ne.0 .and. is3.ne.0 )then
         loopse8(\
           u(i1      ,i2-  is2,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,  is2,  is3,c),\
           u(i1      ,i2-2*is2,i3-  is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,2*is2,  is3,c),\
           u(i1      ,i2-  is2,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,  is2,2*is3,c),\
           u(i1      ,i2-2*is2,i3-2*is3,c)=taylor4thOrderEven3d(i1,i2,i3,  is1,2*is2,2*is3,c),,,,)
       else
         write(*,*) 'ERROR: is1=is2=is3=0!'
         stop 21
       end if
     else if( ncg.gt.2 )then
       ! general case : do all
       if( useWhereMask.ne.0 )then
        beginLoopsWithMask()
         do m3=ng3a,ng3b
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           if( m1.eq.1 .and. m2.eq.1 .and. m3.eq.1 )then
             u(i1-  is1,i2-  is2,i3-  is3,c)=taylor4thOrderEven3dVertex(i1,i2,i3,is1,is2,is3,c)
           else
             u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=taylor4thOrderEven3d(i1,i2,i3,m1*is1,m2*is2,m3*is3,c)
           end if
         end do
         end do
         end do
        endLoopsWithMask()
       else
        beginLoops()
         do m3=ng3a,ng3b
         do m2=ng2a,ng2b
         do m1=ng1a,ng1b
           if( m1.eq.1 .and. m2.eq.1 .and. m3.eq.1 )then
             u(i1-  is1,i2-  is2,i3-  is3,c)=taylor4thOrderEven3dVertex(i1,i2,i3,is1,is2,is3,c)
           else
             u(i1-m1*is1,i2-m2*is2,i3-m3*is3,c)=taylor4thOrderEven3d(i1,i2,i3,m1*is1,m2*is2,m3*is3,c)
           end if
         end do
         end do
         end do
        endLoops()
       end if
     end if
     mmm(-is1,-is2,-is3)=1 ! reset

   end if
 else if( cornerBC(side1,side2,side3).ne.doNothingCorner )then
   write(*,*)'fixBoundaryCorners:Error:'
   write(*,*)' unknown cornerBC=',cornerBC(side1,side2,side3)
 end if
end if
#endMacro



      subroutine fixBoundaryCornersOpt( nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask, ca,cb, useWhereMask, indexRange, dimension, 
     & isPeriodic, bc, cornerBC, orderOfExtrapolation,ncg,
     & cornerExtrapolationOption )    
c======================================================================
c  Optimised Boundary Conditions
c         
c nd : number of space dimensions
c ca,cb : assign components c=uC(ca),..,uC(cb)
c useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
c
c ncg: number of corner ghost points to assign
c cornerExtrapolationOption : used to extrapolate corners along certain directions (or really
c  to not extrapolate in certain directions).
c======================================================================
      implicit none
      integer nd, orderOfExtrapolation,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,ncg,cornerExtrapolationOption

      integer useWhereMask,bc(0:1,0:2),isPeriodic(0:2)
      integer indexRange(0:1,0:2),dimension(0:1,0:2)
      integer cornerBC(0:2,0:2,0:2)

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)

      integer c,ca,cb,n

      integer doNothingCorner,extrapolateCorner,symmetryCorner,taylor2ndOrder
      integer evenSymmetryCorner,oddSymmetryCorner,taylor2ndOrderEvenCorner,taylor4thOrderEvenCorner

      parameter(doNothingCorner=-1,extrapolateCorner=0,symmetryCorner=1,taylor2ndOrder=2,
     &  evenSymmetryCorner=3,oddSymmetryCorner=4,taylor2ndOrderEvenCorner=5,taylor4thOrderEvenCorner=6 )

c     --- local variables 
      integer side1,side2,side3,is1,is2,is3,i1,i2,i3  
      integer n1a,n1b,n1c, n2a,n2b,n2c, n3a,n3b,n3c
      integer ng1a,ng1b,ng2a,ng2b,ng3a,ng3b
      integer js1,js2,js3

      integer mmm(-1:1,-1:1,-1:1)

      integer axis1,axis2,axis3
      parameter( axis1=0,axis2=1,axis3=2 )
c........begin statement functions
      integer m1,m2,m3
      real taylor2ndOrderEven2d,taylor2ndOrderEven3d
      real taylor4thOrderEven2d,taylor4thOrderEven3d
      real taylor4thOrderEven3dVertex

c     These approximations come from ogmg/bc.maple
      taylor2ndOrderEven2d(i1,i2,i3,m1,m2,c)=-m1*u(i1+1,i2,i3,c)+m1*u(i1-1,i2,i3,c)\
                     -m2*u(i1,i2+1,i3,c)+m2*u(i1,i2-1,i3,c)+u(i1+m1,i2+m2,i3,c)
      taylor2ndOrderEven3d(i1,i2,i3,m1,m2,m3,c)= -m1*u(i1+1,i2,i3,c)+m1*u(i1-1,i2,i3,c)-m2*u(i1,i2+1,i3,c)\
                          +m2*u(i1,i2-1,i3,c)-m3*u(i1,i2,i3+1,c)+m3*u(i1,i2,i3-1,c)+u(i1+m1,i2+m2,i3+m3,c)
      
c      taylor4thOrderEven3dVertex(i1,i2,i3)=(-8*u(i1+1,i2,i3,c)+8*u(i1-1,i2,i3,c)+8*u(i1+1,i2+1,i3,c)-4*u(i1-1,i2+1,i3,c)-4*u(i1+1,i2-1,i3,c)-8*u(i1,i2+1,i3,c)+8*u(i1,i2-1,i3,c)-8*u(i1,i2,i3+1,c)+8*u(i1,i2,i3-1,c)+3*u(i1+1,i2+1,i3+1,c)+u(i1+1,i2+1,i3-1,c)+u(i1+1,i2-1,i3+1,c)-u(i1+1,i2-1,i3-1,c)+u(i1-1,i2+1,i3+1,c)-u(i1-1,i2+1,i3-1,c)-u(i1-1,i2-1,i3+1,c)+2*u(i1-1,i2+2,i3,c)-2*u(i1,i2+2,i3+1,c)+8*u(i1,i2+1,i3+1,c)+2*u(i1,i2+2,i3-1,c)-4*u(i1,i2+1,i3-1,c)-2*u(i1+1,i2,i3+2,c)+2*u(i1-1,i2,i3+2,c)-4*u(i1-1,i2,i3+1,c)-2*u(i1,i2+1,i3+2,c)+2*u(i1,i2-1,i3+2,c)-4*u(i1,i2-1,i3+1,c)-2*u(i1+2,i2,i3+1,c)+8*u(i1+1,i2,i3+1,c)+2*u(i1+2,i2,i3-1,c)-4*u(i1+1,i2,i3-1,c)-2*u(i1+1,i2+2,i3,c)+2*u(i1+2,i2-1,i3,c)-2*u(i1+2,i2+1,i3,c))/3.0

c      taylor4thOrderEven2d(i1,i2,i3,m1,m2,c)=(-3*m1*m2**2*u(i1+1,i2+2*m2,i3,c)+6*m1*m2**2*u(i1+1,i2+m2,i3,c)-3*m1*m2**2*u(i1+1,i2,i3,c)+3*m1*m2**2*u(i1-1,i2+2*m2,i3,c)-6*m1*m2**2*u(i1-1,i2+m2,i3,c)+3*m1*m2**2*u(i1-1,i2,i3,c)-3*m1**2*m2*u(i1+2*m1,i2+1,i3,c)+6*m1**2*m2*u(i1+m1,i2+1,i3,c)-3*m1**2*m2*u(i1,i2+1,i3,c)+3*m1**2*m2*u(i1+2*m1,i2-1,i3,c)-6*m1**2*m2*u(i1+m1,i2-1,i3,c)+3*m1**2*m2*u(i1,i2-1,i3,c)-8*m1*u(i1+1,i2,i3,c)+8*m1*u(i1-1,i2,i3,c)-8*m2*u(i1,i2+1,i3,c)+8*m2*u(i1,i2-1,i3,c)+2*m1**3*u(i1+1,i2,i3,c)-2*m1**3*u(i1-1,i2,i3,c)+m1**3*u(i1-2,i2,i3,c)-m2**3*u(i1,i2+2,i3,c)+2*m2**3*u(i1,i2+1,i3,c)-m2*u(i1,i2-2,i3,c)+m1*u(i1+2,i2,i3,c)+m2*u(i1,i2+2,i3,c)-m1*u(i1-2,i2,i3,c)-2*m2**3*u(i1,i2-1,i3,c)+m2**3*u(i1,i2-2,i3,c)-m1**3*u(i1+2,i2,i3,c)+6*u(i1+m1,i2+m2,i3,c))/6.0

      taylor4thOrderEven2d(i1,i2,i3,m1,m2,n)=(-3*(m1)**2*(m2)*u(i1,i2+1,i3,n)+3*(m1)**2*(m2)*u(i1+2*is1,i2-1,i3,n)-6*(m1)**2*(m2)*u(i1+is1,i2-1,i3,n)+3*(m1)**2*(m2)*u(i1,i2-1,i3,n)-3*(m1)*(m2)**2*u(i1+1,i2+2*is2,i3,n)+6*(m1)*(m2)**2*u(i1+1,i2+is2,i3,n)-3*(m1)*(m2)**2*u(i1+1,i2,i3,n)+3*(m1)*(m2)**2*u(i1-1,i2+2*is2,i3,n)-6*(m1)*(m2)**2*u(i1-1,i2+is2,i3,n)+3*(m1)*(m2)**2*u(i1-1,i2,i3,n)+6*(m1)**2*(m2)*u(i1+is1,i2+1,i3,n)-3*(m1)**2*(m2)*u(i1+2*is1,i2+1,i3,n)-8*(m2)*u(i1,i2+1,i3,n)+8*(m2)*u(i1,i2-1,i3,n)+6*u(i1+(m1),i2+(m2),i3,n)-(m2)*u(i1,i2-2,i3,n)-(m1)**3*u(i1+2,i2,i3,n)+2*(m1)**3*u(i1+1,i2,i3,n)-2*(m1)**3*u(i1-1,i2,i3,n)+(m1)**3*u(i1-2,i2,i3,n)-(m2)**3*u(i1,i2+2,i3,n)+2*(m2)**3*u(i1,i2+1,i3,n)-2*(m2)**3*u(i1,i2-1,i3,n)+(m2)**3*u(i1,i2-2,i3,n)-6*(m1)*u(i1+1,i2,i3,n)+6*(m1)*u(i1-1,i2,i3,n)+(m1)*u(i1+2,i2,i3,n)-2*(m1)*u(i1+1,i2,i3,n)+2*(m1)*u(i1-1,i2,i3,n)-(m1)*u(i1-2,i2,i3,n)+(m2)*u(i1,i2+2,i3,n))/6.0


      taylor4thOrderEven3d(i1,i2,i3,m1,m2,m3,n)=(6*m2*m3**2*u(i1,i2-1,i3,n)+6*m2*m3**2*u(i1,i2-1,i3+2*is3,n)+6*m2**2*m3*u(i1,i2,i3-1,n)-12*m2**2*m3*u(i1,i2+is2,i3-1,n)+6*m1*m3**2*u(i1-1,i2,i3,n)-6*m2*m3**2*u(i1,i2+1,i3,n)+12*m2**2*m3*u(i1,i2+is2,i3+1,n)-6*m1**2*m3*u(i1+2*is1,i2,i3+1,n)+12*m1**2*m3*u(i1+is1,i2,i3+1,n)-6*m1**2*m3*u(i1,i2,i3+1,n)+6*m1**2*m3*u(i1+2*is1,i2,i3-1,n)-12*m1**2*m3*u(i1+is1,i2,i3-1,n)+6*m1**2*m3*u(i1,i2,i3-1,n)-6*m1**2*m2*u(i1+2*is1,i2+1,i3,n)+12*m1**2*m2*u(i1+is1,i2+1,i3,n)-6*m1**2*m2*u(i1,i2+1,i3,n)+6*m1**2*m2*u(i1+2*is1,i2-1,i3,n)-12*m1**2*m2*u(i1+is1,i2-1,i3,n)+6*m1**2*m2*u(i1,i2-1,i3,n)-6*m1*m2**2*u(i1+1,i2+2*is2,i3,n)+12*m1*m2**2*u(i1+1,i2+is2,i3,n)-6*m1*m2**2*u(i1+1,i2,i3,n)+6*m1*m2**2*u(i1-1,i2+2*is2,i3,n)-12*m1*m2**2*u(i1-1,i2+is2,i3,n)+6*m1*m2**2*u(i1-1,i2,i3,n)+12*m2*m3**2*u(i1,i2+1,i3+is3,n)+12*m1*m3**2*u(i1+1,i2,i3+is3,n)-12*m1*m3**2*u(i1-1,i2,i3+is3,n)-6*m1*m3**2*u(i1+1,i2,i3,n)-12*m2*m3**2*u(i1,i2-1,i3+is3,n)-6*m2**2*m3*u(i1,i2,i3+1,n)-6*m1*m3**2*u(i1+1,i2,i3+2*is3,n)+6*m1*m3**2*u(i1-1,i2,i3+2*is3,n)-6*m2**2*m3*u(i1,i2+2*is2,i3+1,n)-6*m2*m3**2*u(i1,i2+1,i3+2*is3,n)+6*m2**2*m3*u(i1,i2+2*is2,i3-1,n)+12*u(i1+m1,i2+m2,i3+m3,n)-16*m2*u(i1,i2+1,i3,n)+16*m2*u(i1,i2-1,i3,n)-16*m3*u(i1,i2,i3+1,n)+16*m3*u(i1,i2,i3-1,n)-3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2+is2,i3+is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2+is2,i3-is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2-is2,i3+is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1+is1,i2-is2,i3-is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2+is2,i3+is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2+is2,i3-is3,n)-3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2-is2,i3+is3,n)+3*m1*m2*m3*is1*is2*is3*u(i1-is1,i2-is2,i3-is3,n)-12*m1*u(i1+1,i2,i3,n)+12*m1*u(i1-1,i2,i3,n)+2*m1*u(i1+2,i2,i3,n)-4*m1*u(i1+1,i2,i3,n)+4*m1*u(i1-1,i2,i3,n)-2*m1*u(i1-2,i2,i3,n)+2*m2*u(i1,i2+2,i3,n)-2*m2*u(i1,i2-2,i3,n)+2*m3*u(i1,i2,i3+2,n)-2*m3*u(i1,i2,i3-2,n)-2*m1**3*u(i1+2,i2,i3,n)+4*m1**3*u(i1+1,i2,i3,n)-4*m1**3*u(i1-1,i2,i3,n)+2*m1**3*u(i1-2,i2,i3,n)-2*m2**3*u(i1,i2+2,i3,n)+4*m2**3*u(i1,i2+1,i3,n)-4*m2**3*u(i1,i2-1,i3,n)+2*m2**3*u(i1,i2-2,i3,n)-2*m3**3*u(i1,i2,i3+2,n)+4*m3**3*u(i1,i2,i3+1,n)-4*m3**3*u(i1,i2,i3-1,n)+2*m3**3*u(i1,i2,i3-2,n))/12.0

      taylor4thOrderEven3dVertex(i1,i2,i3,m1,m2,m3,n)=(u(i1+is1,i2+is2,i3-is3,n)+8*is3*u(i1,i2,i3-1,n)-8*is3*u(i1,i2,i3+1,n)+8*is2*u(i1,i2-1,i3,n)-8*is2*u(i1,i2+1,i3,n)-4*is1*u(i1+1,i2,i3,n)+u(i1+is1,i2-is2,i3+is3,n)-u(i1+is1,i2-is2,i3-is3,n)+u(i1-is1,i2+is2,i3+is3,n)-u(i1-is1,i2+is2,i3-is3,n)-u(i1-is1,i2-is2,i3+is3,n)+3*u(i1+is1,i2+is2,i3+is3,n)-2*is2*u(i1+2*is1,i2+1,i3,n)+2*is2*u(i1+2*is1,i2-1,i3,n)-4*is2*u(i1+is1,i2-1,i3,n)+4*is2*u(i1+is1,i2+1,i3,n)-4*is3*u(i1+is1,i2,i3-1,n)+2*is3*u(i1+2*is1,i2,i3-1,n)+4*is3*u(i1+is1,i2,i3+1,n)-2*is3*u(i1+2*is1,i2,i3+1,n)+2*is2*u(i1,i2-1,i3+2*is3,n)+4*is2*u(i1,i2+1,i3+is3,n)+2*is1*u(i1-1,i2,i3+2*is3,n)+2*is1*u(i1-1,i2+2*is2,i3,n)-2*is1*u(i1+1,i2,i3+2*is3,n)+4*is1*u(i1+1,i2,i3+is3,n)-4*is1*u(i1-1,i2+is2,i3,n)+4*is1*u(i1+1,i2+is2,i3,n)-2*is1*u(i1+1,i2+2*is2,i3,n)-4*is1*u(i1-1,i2,i3+is3,n)-2*is2*u(i1,i2+1,i3+2*is3,n)-4*is2*u(i1,i2-1,i3+is3,n)+4*is1*u(i1-1,i2,i3,n)-4*is1*u(i1+1,i2,i3,n)+4*is1*u(i1-1,i2,i3,n)-2*is3*u(i1,i2+2*is2,i3+1,n)+4*is3*u(i1,i2+is2,i3+1,n)-4*is3*u(i1,i2+is2,i3-1,n)+2*is3*u(i1,i2+2*is2,i3-1,n))/3.0


c$$$      taylor4thOrderEven3d(i1,i2,i3,m1,m2,m3)=(6*m1*m3**2*u(i1-1,i2,i3+2*m3,c)+12*u(i1+m1,i2+m2,i3+m3,c)-16*m1*u(i1+1,i2,i3,c)+16*m1*u(i1-1,i2,i3,c)-16*m2*u(i1,i2+1,i3,c)+16*m2*u(i1,i2-1,i3,c)-16*m3*u(i1,i2,i3+1,c)+16*m3*u(i1,i2,i3-1,c)+6*m1*m3**2*u(i1-1,i2,i3,c)-4*m2**3*u(i1,i2-1,i3,c)+2*m2**3*u(i1,i2-2,i3,c)-2*m3**3*u(i1,i2,i3+2,c)-12*m2**2*m3*u(i1,i2+m2,i3-1,c)+6*m2**2*m3*u(i1,i2,i3-1,c)-6*m1**2*m3*u(i1+2*m1,i2,i3+1,c)+12*m1**2*m3*u(i1+m1,i2,i3+1,c)-6*m1**2*m3*u(i1,i2,i3+1,c)+6*m1**2*m3*u(i1+2*m1,i2,i3-1,c)-12*m1**2*m3*u(i1+m1,i2,i3-1,c)+6*m1**2*m3*u(i1,i2,i3-1,c)-6*m1**2*m2*u(i1+2*m1,i2+1,i3,c)-6*m1**2*m2*u(i1,i2+1,i3,c)+6*m1**2*m2*u(i1+2*m1,i2-1,i3,c)-12*m1**2*m2*u(i1+m1,i2-1,i3,c)+6*m1**2*m2*u(i1,i2-1,i3,c)-6*m1*m2**2*u(i1+1,i2+2*m2,i3,c)+12*m1*m2**2*u(i1+1,i2+m2,i3,c)-6*m1*m2**2*u(i1+1,i2,i3,c)+6*m1*m2**2*u(i1-1,i2+2*m2,i3,c)-12*m1*m2**2*u(i1-1,i2+m2,i3,c)+6*m1*m2**2*u(i1-1,i2,i3,c)-6*m2*m3**2*u(i1,i2+1,i3+2*m3,c)+12*m2*m3**2*u(i1,i2+1,i3+m3,c)-6*m2*m3**2*u(i1,i2+1,i3,c)+6*m2*m3**2*u(i1,i2-1,i3+2*m3,c)-12*m2*m3**2*u(i1,i2-1,i3+m3,c)+6*m2*m3**2*u(i1,i2-1,i3,c)+6*m2**2*m3*u(i1,i2+2*m2,i3-1,c)-6*m1*m3**2*u(i1+1,i2,i3,c)-6*m2**2*m3*u(i1,i2+2*m2,i3+1,c)+12*m2**2*m3*u(i1,i2+m2,i3+1,c)-6*m2**2*m3*u(i1,i2,i3+1,c)-12*m1*m3**2*u(i1-1,i2,i3+m3,c)+12*m1**2*m2*u(i1+m1,i2+1,i3,c)-6*m1*m3**2*u(i1+1,i2,i3+2*m3,c)+12*m1*m3**2*u(i1+1,i2,i3+m3,c)+4*m3**3*u(i1,i2,i3+1,c)-4*m3**3*u(i1,i2,i3-1,c)+2*m3**3*u(i1,i2,i3-2,c)+2*m1*u(i1+2,i2,i3,c)-2*m1*u(i1-2,i2,i3,c)+2*m2*u(i1,i2+2,i3,c)-2*m2*u(i1,i2-2,i3,c)+2*m3*u(i1,i2,i3+2,c)-2*m3*u(i1,i2,i3-2,c)-2*m1**3*u(i1+2,i2,i3,c)+4*m1**3*u(i1+1,i2,i3,c)-4*m1**3*u(i1-1,i2,i3,c)+2*m1**3*u(i1-2,i2,i3,c)-2*m2**3*u(i1,i2+2,i3,c)+4*m2**3*u(i1,i2+1,i3,c)-3*m1*m2*m3*u(i1+1,i2+1,i3+1,c)+3*m1*m2*m3*u(i1+1,i2+1,i3-1,c)+3*m1*m2*m3*u(i1+1,i2-1,i3+1,c)-3*m1*m2*m3*u(i1+1,i2-1,i3-1,c)+3*m1*m2*m3*u(i1-1,i2+1,i3+1,c)-3*m1*m2*m3*u(i1-1,i2+1,i3-1,c)-3*m1*m2*m3*u(i1-1,i2-1,i3+1,c)+3*m1*m2*m3*u(i1-1,i2-1,i3-1,c))/12.0
c$$$
c$$$c The next statement function is a copy of the above except that we zero out one value corresponding to a
c$$$c point (-1,-1,-1), (1,-1,-1), (1,1,1), (-1,-1,1) etc. which is brought to the left hand side (coeff of 1/4)
c$$$c and thus changes the /12.0 to  4/3*(1/12) = 1/9
c$$$      taylor4thOrderEven3dVertex(i1,i2,i3,m1,m2,m3)=(6*m1*m3**2*u(i1-1,i2,i3+2*m3,c)+12*u(i1+m1,i2+m2,i3+m3,c)-16*m1*u(i1+1,i2,i3,c)+16*m1*u(i1-1,i2,i3,c)-16*m2*u(i1,i2+1,i3,c)+16*m2*u(i1,i2-1,i3,c)-16*m3*u(i1,i2,i3+1,c)+16*m3*u(i1,i2,i3-1,c)+6*m1*m3**2*u(i1-1,i2,i3,c)-4*m2**3*u(i1,i2-1,i3,c)+2*m2**3*u(i1,i2-2,i3,c)-2*m3**3*u(i1,i2,i3+2,c)-12*m2**2*m3*u(i1,i2+m2,i3-1,c)+6*m2**2*m3*u(i1,i2,i3-1,c)-6*m1**2*m3*u(i1+2*m1,i2,i3+1,c)+12*m1**2*m3*u(i1+m1,i2,i3+1,c)-6*m1**2*m3*u(i1,i2,i3+1,c)+6*m1**2*m3*u(i1+2*m1,i2,i3-1,c)-12*m1**2*m3*u(i1+m1,i2,i3-1,c)+6*m1**2*m3*u(i1,i2,i3-1,c)-6*m1**2*m2*u(i1+2*m1,i2+1,i3,c)-6*m1**2*m2*u(i1,i2+1,i3,c)+6*m1**2*m2*u(i1+2*m1,i2-1,i3,c)-12*m1**2*m2*u(i1+m1,i2-1,i3,c)+6*m1**2*m2*u(i1,i2-1,i3,c)-6*m1*m2**2*u(i1+1,i2+2*m2,i3,c)+12*m1*m2**2*u(i1+1,i2+m2,i3,c)-6*m1*m2**2*u(i1+1,i2,i3,c)+6*m1*m2**2*u(i1-1,i2+2*m2,i3,c)-12*m1*m2**2*u(i1-1,i2+m2,i3,c)+6*m1*m2**2*u(i1-1,i2,i3,c)-6*m2*m3**2*u(i1,i2+1,i3+2*m3,c)+12*m2*m3**2*u(i1,i2+1,i3+m3,c)-6*m2*m3**2*u(i1,i2+1,i3,c)+6*m2*m3**2*u(i1,i2-1,i3+2*m3,c)-12*m2*m3**2*u(i1,i2-1,i3+m3,c)+6*m2*m3**2*u(i1,i2-1,i3,c)+6*m2**2*m3*u(i1,i2+2*m2,i3-1,c)-6*m1*m3**2*u(i1+1,i2,i3,c)-6*m2**2*m3*u(i1,i2+2*m2,i3+1,c)+12*m2**2*m3*u(i1,i2+m2,i3+1,c)-6*m2**2*m3*u(i1,i2,i3+1,c)-12*m1*m3**2*u(i1-1,i2,i3+m3,c)+12*m1**2*m2*u(i1+m1,i2+1,i3,c)-6*m1*m3**2*u(i1+1,i2,i3+2*m3,c)+12*m1*m3**2*u(i1+1,i2,i3+m3,c)+4*m3**3*u(i1,i2,i3+1,c)-4*m3**3*u(i1,i2,i3-1,c)+2*m3**3*u(i1,i2,i3-2,c)+2*m1*u(i1+2,i2,i3,c)-2*m1*u(i1-2,i2,i3,c)+2*m2*u(i1,i2+2,i3,c)-2*m2*u(i1,i2-2,i3,c)+2*m3*u(i1,i2,i3+2,c)-2*m3*u(i1,i2,i3-2,c)-2*m1**3*u(i1+2,i2,i3,c)+4*m1**3*u(i1+1,i2,i3,c)-4*m1**3*u(i1-1,i2,i3,c)+2*m1**3*u(i1-2,i2,i3,c)-2*m2**3*u(i1,i2+2,i3,c)+4*m2**3*u(i1,i2+1,i3,c)-3*m1*m2*m3*mmm(1,1,1)*u(i1+1,i2+1,i3+1,c)+3*m1*m2*m3*mmm(1,1,-1)*u(i1+1,i2+1,i3-1,c)+3*m1*m2*m3*mmm(1,-1,1)*u(i1+1,i2-1,i3+1,c)-3*m1*m2*m3*mmm(1,-1,-1)*u(i1+1,i2-1,i3-1,c)+3*m1*m2*m3*mmm(-1,1,1)*u(i1-1,i2+1,i3+1,c)-3*m1*m2*m3*mmm(-1,1,-1)*u(i1-1,i2+1,i3-1,c)-3*m1*m2*m3*mmm(-1,-1,1)*u(i1-1,i2-1,i3+1,c)+3*m1*m2*m3*mmm(-1,-1,-1)*u(i1-1,i2-1,i3-1,c))/9.

      
      data mmm/1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1/

c........end statement functions

c  Index I1=Range(indexRange[Start][0],indexRange[End][0])
c  Index I2=Range(indexRange[Start][1],indexRange[End][1])
c  Index I3=Range(indexRange[Start][2],indexRange[End][2])
c  Index N =C0!=nullRange ? C0 : Range(u.getComponentBase(0),u.getComponentBound(0))  

      
c        ---extrapolate or otherwise assign values outside edges---

      if( isPeriodic(0).eq.0 .and. isPeriodic(1).eq.0 )then
c     ...Do the four edges parallel to i3
        side3=2 ! this means we are on an edge
        is3=0
        do side1=0,1
          is1=1-2*side1
          do side2=0,1
   	    is2=1-2*side2
	    if( bc(side1,0).gt.0 .or. bc(side2,1).gt.0 )then
	      assignCorners(side1,side2,side3)
            end if
          end do
        end do
 
      end if
      if( nd.le.2 )then
        return
      end if

      if( isPeriodic(0).eq.0 .and. isPeriodic(2).eq.0 )then
c     ...Do the four edges parallel to i2
        side2=2 ! this means we are on an edge
        is2=0
        do side1=0,1
          is1=1-2*side1
          do side3=0,1
            is3=1-2*side3
            if( bc(side1,0).gt.0 .or. bc(side3,2).gt.0 )then
	      assignCorners(side1,side2,side3)
            end if
          end do
        end do
      end if

      if( isPeriodic(1).eq.0 .and. isPeriodic(2).eq.0 )then
c          ...Do the four edges parallel to i1
        side1=2 ! this means we are on an edge
        is1=0
        do side2=0,1
          is2=1-2*side2
          do side3=0,1
            is3=1-2*side3
            if( bc(side2,1).gt.0 .or. bc(side3,2).gt.0 )then
c             We have to loop over i3 from inside to outside since later points depend on previous ones.
              assignCorners(side1,side2,side3)
            end if
          end do
        end do
      end if
  
      if( isPeriodic(0).eq.0 .and. isPeriodic(1).eq.0 .and. 
     &    isPeriodic(2).eq.0 )then
c           ...Do the points outside vertices in 3D
        do side1=0,1
          is1=1-2*side1
          do side2=0,1 
            is2=1-2*side2
            do side3=0,1
              is3=1-2*side3
              if( bc(side1,0).gt.0 .or.
     &            bc(side2,1).gt.0 .or.
     &            bc(side3,2).gt.0 )then
c     write(*,'(''n1a,n1b,n2a,n2b,n3a,n3b,n3c='',6i4,'' cornerBC='',i4)') n1a,n1b,n2a,n2b,n3a,n3b,n3c,cornerBC(side1,side2,side3)                
c     write(*,'(''orderOfExtrapolation='',i4)') orderOfExtrapolation
                assignCorners(side1,side2,side3)
              end if
            end do
          end do
        end do
      end if


      return
      end

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
c======================================================================
c  Optimised Boundary Conditions
c         
c nd : number of space dimensions
c ca,cb : assign components c=uC(ca),..,uC(cb)
c useWhereMask : if not equal to zero, only apply the BC where mask(i1,i2,i3).ne.0
c======================================================================
      implicit none
      integer nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b

      integer isPeriodic(0:2),indexRange(0:1,0:2)
      integer gridIndexRange(0:1,0:2),dimension(0:1,0:2)

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      integer ca,cb

c     --- local variables 
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
c         length of the period:
          diff=gridIndexRange(1,axis)-gridIndexRange(0,axis)
c         assign all ghost points on "left"
c         I[i]=Range(dimension(Start,axis),indexRange(Start,axis)-1);
c         u(I[0],I[1],I[2],I[3])=u(I[0]+diff[0],I[1]+diff[1],I[2]+diff[2],I[3]+diff[3]);
c         // assign all ghost points on "right"
c         I[i]=Range(indexRange(End,axis)+1,dimension(End,axis));
c         u(I[0],I[1],I[2],I[3])=u(I[0]-diff[0],I[1]-diff[1],I[2]-diff[2],I[3]-diff[3]);
          
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
      
#beginMacro loops(expression)
do c=ca,cb
do i=n1a,n1b
  expression
end do
end do
#endMacro


      subroutine extrapInterpNeighboursOpt(nd, 
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & nda1a,nda1b,ndd1a,ndd1b,
     & ia,id, u,ca,cb, orderOfExtrapolation )
c======================================================================
c  Optimised Boundary Conditions ** extrapolate interpolation neighbours ***
c         
c nd : number of space dimensions
c ca,cb : assign components c=uC(ca),..,uC(cb)
c ia : extrapolateInterpolationNeighbourPoints
c id : extrapolateInterpolationNeighboursDirection
c======================================================================
      implicit none
      integer nd, nda1a,nda1b,ndd1a,ndd1b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b

      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      integer ia(nda1a:nda1b,0:*), id(ndd1a:ndd1b,0:*)
      integer ca,cb,orderOfExtrapolation

c     --- local variables 
      integer i,i1,i2,i3,c,n1a,n1b,width

      n1a=nda1a
      n1b=nda1b

      i2=ndu2a
      i3=ndu3a

      ! write(*,'("extrapInterpNeighboursOpt: orderOfExtrapolation=",i4)') orderOfExtrapolation

      if( .true. .and. nd.eq.3 ) then
        ! check the extrapolation formula
        width=orderOfExtrapolation
        if( width.le.0 )then
          width=3 ! default
        end if
        do i=n1a,n1b
          i1=ia(i,0)
          i2=ia(i,1)
          i3=ia(i,2)
          if( i1.lt.ndu1a .or. i1.gt.ndu1b .or. i1+width*id(i,0).lt.ndu1a .or. i1+width*id(i,0).gt.ndu1b .or.\
              i2.lt.ndu2a .or. i2.gt.ndu2b .or. i2+width*id(i,1).lt.ndu2a .or. i2+width*id(i,1).gt.ndu2b .or.\
              i3.lt.ndu3a .or. i3.gt.ndu3b .or. i3+width*id(i,2).lt.ndu3a .or. i3+width*id(i,2).gt.ndu3b )then
            write(*,'("extrapInterpNeighboursOpt:ERROR: i=",3i4," id=",3i4," ndu1a,...=",6i4)') \
              i1,i2,i3,id(i,0),id(i,1),id(i,2),ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b
              
          end if
        end do
      end if

      if( orderOfExtrapolation.eq.3 .or. orderOfExtrapolation.le.0 ) then
	if( nd.eq.2 ) then 
          loops(u(ia(i,0),ia(i,1),i3,c)=(3.*u(ia(i,0)+  id(i,0),ia(i,1)+  id(i,1),i3,c)- \
				         3.*u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),i3,c)+ \
				            u(ia(i,0)+3*id(i,0),ia(i,1)+3*id(i,1),i3,c)) )
	else if( nd.eq.3 ) then
          loops(u(ia(i,0),ia(i,1),ia(i,2),c)=(3.*u(ia(i,0)+  id(i,0),ia(i,1)+  id(i,1),ia(i,2)+  id(i,2),c)-\
					      3.*u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),ia(i,2)+2*id(i,2),c)+\
					         u(ia(i,0)+3*id(i,0),ia(i,1)+3*id(i,1),ia(i,2)+3*id(i,2),c)))
	else
	  loops(u(ia(i,0),i2,i3,c)=(3.*u(ia(i,0)+  id(i,0),i2,i3,c)-\
				    3.*u(ia(i,0)+2*id(i,0),i2,i3,c)+\
				       u(ia(i,0)+3*id(i,0),i2,i3,c)))
        end if
      else if( orderOfExtrapolation.eq.2 ) then

	if( nd.eq.2 ) then 
	  loops(u(ia(i,0),ia(i,1),i3,c)=(2.*u(ia(i,0)+  id(i,0),ia(i,1)+  id(i,1),i3,c)- \
				            u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),i3,c)) )
	else if( nd.eq.3 ) then

	  loops(u(ia(i,0),ia(i,1),ia(i,2),c)=(2.*u(ia(i,0)+id(i,0),ia(i,1)+id(i,1),ia(i,2)+id(i,2),c)- \
	    u(ia(i,0)+2*id(i,0),ia(i,1)+2*id(i,1),ia(i,2)+2*id(i,2),c)))
	else
          loops(u(ia(i,0),i2,i3,c)=(2.*u(ia(i,0)+  id(i,0),i2,i3,c)- \
			               u(ia(i,0)+2*id(i,0),i2,i3,c)))
        end if
      else if( orderOfExtrapolation.eq.1 ) then

	if( nd.eq.2 ) then 
          loops(u(ia(i,0),ia(i,1),i3,c)=u(ia(i,0)+id(i,0),ia(i,1)+id(i,1),i3,c))
	else if( nd.eq.3 ) then 
	  loops(u(ia(i,0),ia(i,1),ia(i,2),c)=u(ia(i,0)+id(i,0),ia(i,1)+id(i,1),ia(i,2)+id(i,2),c))
	else
	  loops(u(ia(i,0),i2,i3,c)=u(ia(i,0)+id(i,0),i2,i3,c))
        end if
      else
	write(*,*) 'extrapInterpNeighboursOpt:ERROR: '
        write(*,*) ' order of extrapolation=',orderOfExtrapolation
        stop 1
      end if

      return 
      end


#beginMacro loopse(expression)
if( useWhereMask.ne.0 )then
  do c0=ca,cb
   c=uC(c0)
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

      subroutine extrapolateOpt(nd, 
     & n1a,n1b,n2a,n2b,n3a,n3b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & u,mask, v, ipar, uC )
c======================================================================
c  Optimised Boundary Conditions ** extrapolate ***
c         
c nd : number of space dimensions
c ca,cb : assign components c=ca...cb
c======================================================================
      implicit none
      integer nd, nda1a,nda1b,ndd1a,ndd1b,
     & ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,
     & ndv1a,ndv1b,ndv2a,ndv2b,ndv3a,ndv3b,ndv4a,ndv4b,
     & ndm1a,ndm1b,ndm2a,ndm2b,ndm3a,ndm3b,
     & n1a,n1b,n2a,n2b,n3a,n3b
      real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
      real v(ndv1a:ndv1b,ndv2a:ndv2b,ndv3a:ndv3b,ndv4a:ndv4b)
      integer mask(ndm1a:ndm1b,ndm2a:ndm2b,ndm3a:ndm3b)
      integer ipar(*),uC(0:*)

      integer extrapolate,extrapolateNormalComponent,
     & extrapolateTangentialComponent0,
     & extrapolateTangentialComponent1
      parameter( extrapolate=2,
     & extrapolateNormalComponent=20,
     & extrapolateTangentialComponent0=21,
     & extrapolateTangentialComponent1=22 )
c     --- local variables 
      integer i1,i2,i3,c0,c,bcType,ca,cb,orderOfExtrapolation,
     & is1,is2,is3,useWhereMask


      bcType=ipar(1)
      useWhereMask=ipar(2)
      orderOfExtrapolation=ipar(3)
      ca=ipar(4)
      cb=ipar(5)
      is1=ipar(6)
      is2=ipar(7)
      is3=ipar(8)

      if( bcType.eq.extrapolate )then

        if( orderOfExtrapolation.eq.1 )then
          loopse(u(i1,i2,i3,c)=u(i1+is1,i2+is2,i3+is3,c))
        else if( orderOfExtrapolation.eq.2 )then
          loopse(u(i1,i2,i3,c)=2.*u(i1+  (is1),i2+  (is2),i3+  (is3),c) \
          -    u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c))
        else if( orderOfExtrapolation.eq.3 )then
          loopse(u(i1,i2,i3,c)= 3.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
          - 3.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
          +    u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c))
        else if( orderOfExtrapolation.eq.4 )then
          loopse(u(i1,i2,i3,c)=4.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
          - 6.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
          + 4.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  \
          -    u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c))
        else if( orderOfExtrapolation.eq.5 )then
          loopse(u(i1,i2,i3,c)=5.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
          -10.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
          +10.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  \
          - 5.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)  \
          +    u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c))
        else if( orderOfExtrapolation.eq.6 )then
          loopse(u(i1,i2,i3,c)=6.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
          -15.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
          +20.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  \
          -15.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)  \
          + 6.*u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c)  \
          -    u(i1+6*(is1),i2+6*(is2),i3+6*(is3),c))
        else 
          write(*,*) 'extrapolateOpt:Error: '
          write(*,*) 'unable to extrapolate '
          write(*,*) ' to orderOfExtrapolation',orderOfExtrapolation
          write(*,*) ' can only do orders 1 to 6.'
          stop 1
        end if

c      else if( bcType.eq.extrapolateNormalComponent .or.
c               bcType.eq.extrapolateTangentialComponent0 .or.
c               bcType.eq.extrapolateTangentialComponent1 )then
c
c
c#beginMacro assignExtrap(rhs)
c        if( nd.eq.1 )
c        loops(temp=rhs,\
c              u(i1,i2,i3,n1)=u(i1,i2,i3,n1)+temp*v(i1,i2,i3,v0),,)
c        else if( nd.eq.2 )then
c        loops(temp=rhs,\
c              u(i1,i2,i3,n1)=u(i1,i2,i3,n1)+temp*v(i1,i2,i3,v0),\
c              u(i1,i2,i3,n2)=u(i1,i2,i3,n2)+temp*v(i1,i2,i3,v1),)
c        else if( nd.eq.3 )then
c        loops(temp=rhs,\
c              u(i1,i2,i3,n1)=u(i1,i2,i3,n1)+temp*v(i1,i2,i3,v0),\
c              u(i1,i2,i3,n2)=u(i1,i2,i3,n2)+temp*v(i1,i2,i3,v1),\
c              u(i1,i2,i3,n3)=u(i1,i2,i3,n3)+temp*v(i1,i2,i3,v2))
c        end if
c#endMacro
c
c       
c        if( orderOfExtrapolation.eq.1 )then
c          assignExtrap(-(u(i1,i2,i3,n1)*v(i1,i2,i3,v0)+u(i1,i2,i3,n2)*v(i1,i2,i3,v1)+u(i1,i2,i3,n3)*v(i1,i2,i3,v2))+\
c        else if( orderOfExtrapolation.eq.2 )then
c          loopse(u(i1,i2,i3,c)=2.*u(i1+  (is1),i2+  (is2),i3+  (is3),c) \
c          -    u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c))
c        else if( orderOfExtrapolation.eq.3 )then
c          loopse(u(i1,i2,i3,c)= 3.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
c          - 3.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
c          +    u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c))
c        else if( orderOfExtrapolation.eq.4 )then
c          loopse(u(i1,i2,i3,c)=4.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
c          - 6.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
c          + 4.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  \
c          -    u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c))
c        else if( orderOfExtrapolation.eq.5 )then
c          loopse(u(i1,i2,i3,c)=5.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
c          -10.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
c          +10.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  \
c          - 5.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)  \
c          +    u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c))
c        else if( orderOfExtrapolation.eq.6 )then
c          loopse(u(i1,i2,i3,c)=6.*u(i1+  (is1),i2+  (is2),i3+  (is3),c)  \
c          -15.*u(i1+2*(is1),i2+2*(is2),i3+2*(is3),c)  \
c          +20.*u(i1+3*(is1),i2+3*(is2),i3+3*(is3),c)  \
c          -15.*u(i1+4*(is1),i2+4*(is2),i3+4*(is3),c)  \
c          + 6.*u(i1+5*(is1),i2+5*(is2),i3+5*(is3),c)  \
c          -    u(i1+6*(is1),i2+6*(is2),i3+6*(is3),c))
c        else 
c          write(*,*) 'extrapolateOpt:Error: '
c          write(*,*) 'unable to extrapolate '
c          write(*,*) ' to orderOfExtrapolation',orderOfExtrapolation
c          write(*,*) ' can only do orders 1 to 6.'
c          stop 1
c        end if


      else
	write(*,*) 'extrapolateOpt:ERROR: '
        write(*,*) ' unknown bcType=',bcType
        stop 1
      end if

      return 
      end

