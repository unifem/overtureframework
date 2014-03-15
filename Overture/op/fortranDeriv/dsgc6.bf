
c
c Define the values of the coefficients at x+ m*h/2  for m=+1,-1,+2,-2,...
c
c  OPTION: DSG = divScalaraGrad
c          LAPLACE = laplace
c          DSGR = divScalarGrad (rectangular)
c
#beginMacro defineMidValueCoefficients(DIMENSION,OPTION,ORDER)

#If #OPTION == "DSG" 

 #If #DIMENSION == "2" 

  a11i  = a11(i1,i2,i3)
  a11m1 = a11(i1-1,i2,i3)
  a11p1 = a11(i1+1,i2,i3)
  a11m2 = a11(i1-2,i2,i3)
  a11p2 = a11(i1+2,i2,i3)
  a11m3 = a11(i1-3,i2,i3)
  a11p3 = a11(i1+3,i2,i3)
  
  a22i  = a22(i1,i2,i3)
  a22m1 = a22(i1,i2-1,i3)
  a22p1 = a22(i1,i2+1,i3)
  a22m2 = a22(i1,i2-2,i3)
  a22p2 = a22(i1,i2+2,i3)
  a22m3 = a22(i1,i2-3,i3)
  a22p3 = a22(i1,i2+3,i3)
  
  #If #ORDER == "8"
   a11m4 = a11(i1-4,i2,i3)
   a11p4 = a11(i1+4,i2,i3)
   a22m4 = a22(i1,i2-4,i3)
   a22p4 = a22(i1,i2+4,i3)
  #End

 #Else

  a11i  = a113d(i1  ,i2,i3)
  a11m1 = a113d(i1-1,i2,i3)
  a11p1 = a113d(i1+1,i2,i3)
  a11m2 = a113d(i1-2,i2,i3)
  a11p2 = a113d(i1+2,i2,i3)
  a11m3 = a113d(i1-3,i2,i3)
  a11p3 = a113d(i1+3,i2,i3)
  
  a22i  = a223d(i1,i2  ,i3)
  a22m1 = a223d(i1,i2-1,i3)
  a22p1 = a223d(i1,i2+1,i3)
  a22m2 = a223d(i1,i2-2,i3)
  a22p2 = a223d(i1,i2+2,i3)
  a22m3 = a223d(i1,i2-3,i3)
  a22p3 = a223d(i1,i2+3,i3)
 
  a33i  = a333d(i1,i2,i3  )
  a33m1 = a333d(i1,i2,i3-1)
  a33p1 = a333d(i1,i2,i3+1)
  a33m2 = a333d(i1,i2,i3-2)
  a33p2 = a333d(i1,i2,i3+2)
  a33m3 = a333d(i1,i2,i3-3)
  a33p3 = a333d(i1,i2,i3+3)

  #If #ORDER == "8"
   a11m4 = a113d(i1-4,i2,i3)
   a11p4 = a113d(i1+4,i2,i3)
   a22m4 = a223d(i1,i2-4,i3)
   a22p4 = a223d(i1,i2+4,i3)
   a33m4 = a333d(i1,i2,i3-4)
   a33p4 = a333d(i1,i2,i3+4)
  #End

 #End

#Elif #OPTION == "DSGR" 

 #If #DIMENSION == "2" 

  a11i  = a11r(i1,i2,i3)
  a11m1 = a11r(i1-1,i2,i3)
  a11p1 = a11r(i1+1,i2,i3)
  a11m2 = a11r(i1-2,i2,i3)
  a11p2 = a11r(i1+2,i2,i3)
  a11m3 = a11r(i1-3,i2,i3)
  a11p3 = a11r(i1+3,i2,i3)
  
  a22i  = a22r(i1,i2,i3)
  a22m1 = a22r(i1,i2-1,i3)
  a22p1 = a22r(i1,i2+1,i3)
  a22m2 = a22r(i1,i2-2,i3)
  a22p2 = a22r(i1,i2+2,i3)
  a22m3 = a22r(i1,i2-3,i3)
  a22p3 = a22r(i1,i2+3,i3)
  
  #If #ORDER == "8"
   a11m4 = a11r(i1-4,i2,i3)
   a11p4 = a11r(i1+4,i2,i3)
   a22m4 = a22r(i1,i2-4,i3)
   a22p4 = a22r(i1,i2+4,i3)
  #End

 #Else

  a11i  = a113dr(i1  ,i2,i3)
  a11m1 = a113dr(i1-1,i2,i3)
  a11p1 = a113dr(i1+1,i2,i3)
  a11m2 = a113dr(i1-2,i2,i3)
  a11p2 = a113dr(i1+2,i2,i3)
  a11m3 = a113dr(i1-3,i2,i3)
  a11p3 = a113dr(i1+3,i2,i3)
  
  a22i  = a223dr(i1,i2  ,i3)
  a22m1 = a223dr(i1,i2-1,i3)
  a22p1 = a223dr(i1,i2+1,i3)
  a22m2 = a223dr(i1,i2-2,i3)
  a22p2 = a223dr(i1,i2+2,i3)
  a22m3 = a223dr(i1,i2-3,i3)
  a22p3 = a223dr(i1,i2+3,i3)
 
  a33i  = a333dr(i1,i2,i3  )
  a33m1 = a333dr(i1,i2,i3-1)
  a33p1 = a333dr(i1,i2,i3+1)
  a33m2 = a333dr(i1,i2,i3-2)
  a33p2 = a333dr(i1,i2,i3+2)
  a33m3 = a333dr(i1,i2,i3-3)
  a33p3 = a333dr(i1,i2,i3+3)

  #If #ORDER == "8"
   a11m4 = a113dr(i1-4,i2,i3)
   a11p4 = a113dr(i1+4,i2,i3)
   a22m4 = a223dr(i1,i2-4,i3)
   a22p4 = a223dr(i1,i2+4,i3)
   a33m4 = a333dr(i1,i2,i3-4)
   a33p4 = a333dr(i1,i2,i3+4)
  #End

 #End

#Else

 a11i  = s(i1  ,i2,i3,0)
 a11m1 = s(i1-1,i2,i3,0)
 a11p1 = s(i1+1,i2,i3,0)
 a11m2 = s(i1-2,i2,i3,0)
 a11p2 = s(i1+2,i2,i3,0)
 a11m3 = s(i1-3,i2,i3,0)
 a11p3 = s(i1+3,i2,i3,0)
 
 a22i  = s(i1,i2,i3  ,0)
 a22m1 = s(i1,i2-1,i3,0)
 a22p1 = s(i1,i2+1,i3,0)
 a22m2 = s(i1,i2-2,i3,0)
 a22p2 = s(i1,i2+2,i3,0)
 a22m3 = s(i1,i2-3,i3,0)
 a22p3 = s(i1,i2+3,i3,0)
 
 #If #ORDER == "8"
  a11m4 = s(i1-4,i2,i3,0)
  a11p4 = s(i1+4,i2,i3,0)
  a22m4 = s(i1,i2-4,i3,0)
  a22p4 = s(i1,i2+4,i3,0)
 #End

 #If #DIMENSION == "3" 
  a33i  = s(i1,i2,i3  ,0)
  a33m1 = s(i1,i2,i3-1,0)
  a33p1 = s(i1,i2,i3+1,0)
  a33m2 = s(i1,i2,i3-2,0)
  a33p2 = s(i1,i2,i3+2,0)
  a33m3 = s(i1,i2,i3-3,0)
  a33p3 = s(i1,i2,i3+3,0)
  #If #ORDER == "8"
   a33m4 = s(i1,i2,i3-4,0)
   a33p4 = s(i1,i2,i3+4,0)
  #End
 #End

#End


 a11ph6 = (150.*(a11p1+a11i)-25.*(a11p2+a11m1)+3.*(a11p3+a11m2))/256.  ! 6th-order approx
 a11mh6 = (150.*(a11i+a11m1)-25.*(a11p1+a11m2)+3.*(a11p2+a11m3))/256.

 a11ph4 = ((a11p1+a11i )*9.-(a11p2+a11m1))/16.                        
 a11mh4 = ((a11i +a11m1)*9.-(a11p1+a11m2))/16.
 a11p3h4= ((a11p2+a11p1)*9.-(a11p3+a11i ))/16.                          ! 4th-order approx
 a11m3h4= ((a11m1+a11m2)*9.-(a11i +a11m3))/16.

 a11ph2 = .5*(a11p1+a11i)  ! 2nd-order approx
 a11mh2 = .5*(a11m1+a11i)
 a11p3h2= .5*(a11p2+a11p1)
 a11m3h2= .5*(a11m1+a11m2)
 a11p5h2= .5*(a11p3+a11p2)
 a11m5h2= .5*(a11m2+a11m3)

 a22ph6 = (150.*(a22p1+a22i)-25.*(a22p2+a22m1)+3.*(a22p3+a22m2))/256.  ! 6th-order approx
 a22mh6 = (150.*(a22i+a22m1)-25.*(a22p1+a22m2)+3.*(a22p2+a22m3))/256.

 a22ph4 = ((a22p1+a22i )*9.-(a22p2+a22m1))/16.                        
 a22mh4 = ((a22i +a22m1)*9.-(a22p1+a22m2))/16.
 a22p3h4= ((a22p2+a22p1)*9.-(a22p3+a22i ))/16.                          ! 4th-order approx
 a22m3h4= ((a22m1+a22m2)*9.-(a22i +a22m3))/16.

 a22ph2 = .5*(a22p1+a22i)  ! 2nd-order approx
 a22mh2 = .5*(a22m1+a22i)
 a22p3h2= .5*(a22p2+a22p1)
 a22m3h2= .5*(a22m1+a22m2)
 a22p5h2= .5*(a22p3+a22p2)
 a22m5h2= .5*(a22m2+a22m3)


 #If #ORDER == "8"

  a11ph8 = (1225.*(a11p1+a11i)-245.*(a11p2+a11m1)+49.*(a11p3+a11m2)-5.*(a11p4+a11m3))/2048. ! 8th-order approx
  a11mh8 = (1225.*(a11m1+a11i)-245.*(a11m2+a11p1)+49.*(a11m3+a11p2)-5.*(a11m4+a11p3))/2048.
c a11ph8 = a11ph6
c a11mh8 = a11mh6

  a11p3h6 = (150.*(a11p2+a11p1)-25.*(a11p3+a11i )+3.*(a11p4+a11m1))/256.  ! 6th-order approx
  a11m3h6 = (150.*(a11m2+a11m1)-25.*(a11m3+a11i )+3.*(a11m4+a11p1))/256.

  a11p5h4= ((a11p3+a11p2)*9.-(a11p4+a11p1))/16.                          ! 4th-order approx
  a11m5h4= ((a11m3+a11m2)*9.-(a11m4+a11m1))/16.

  a11p7h2= .5*(a11p4+a11p3) ! 2nd order approx
  a11m7h2= .5*(a11m4+a11m3)

  a22ph8 = (1225.*(a22p1+a22i)-245.*(a22p2+a22m1)+49.*(a22p3+a22m2)-5.*(a22p4+a22m3))/2048. ! 8th-order approx
  a22mh8 = (1225.*(a22m1+a22i)-245.*(a22m2+a22p1)+49.*(a22m3+a22p2)-5.*(a22m4+a22p3))/2048.
c a22ph8 = a22ph6
c a22mh8 = a22mh6

  a22p3h6 = (150.*(a22p2+a22p1)-25.*(a22p3+a22i )+3.*(a22p4+a22m1))/256.  ! 6th-order approx
  a22m3h6 = (150.*(a22m2+a22m1)-25.*(a22m3+a22i )+3.*(a22m4+a22p1))/256.

  a22p5h4= ((a22p3+a22p2)*9.-(a22p4+a22p1))/16.                          ! 4th-order approx
  a22m5h4= ((a22m3+a22m2)*9.-(a22m4+a22m1))/16.

  a22p7h2= .5*(a22p4+a22p3) ! 2nd order approx
  a22m7h2= .5*(a22m4+a22m3)

 #End

#If #DIMENSION == "3" 

 a33ph6 = (150.*(a33p1+a33i)-25.*(a33p2+a33m1)+3.*(a33p3+a33m2))/256.  ! 6th-order approx
 a33mh6 = (150.*(a33i+a33m1)-25.*(a33p1+a33m2)+3.*(a33p2+a33m3))/256.

 a33p3h4= ((a33p2+a33p1)*9.-(a33p3+a33i ))/16.                          ! 4th-order approx
 a33ph4 = ((a33p1+a33i )*9.-(a33p2+a33m1))/16.                        
 a33mh4 = ((a33i +a33m1)*9.-(a33p1+a33m2))/16.
 a33m3h4= ((a33m1+a33m2)*9.-(a33i +a33m3))/16.

 a33ph2 = .5*(a33p1+a33i)  ! 2nd-order approx
 a33mh2 = .5*(a33m1+a33i)
 a33p3h2= .5*(a33p2+a33p1)
 a33m3h2= .5*(a33m1+a33m2)
 a33p5h2= .5*(a33p3+a33p2)
 a33m5h2= .5*(a33m2+a33m3)

 #If #ORDER == "8"
  a33ph8 = (1225.*(a33p1+a33i)-245.*(a33p2+a33m1)+49.*(a33p3+a33m2)-5.*(a33p4+a33m3))/2048. ! 8th-order approx
  a33mh8 = (1225.*(a33m1+a33i)-245.*(a33m2+a33p1)+49.*(a33m3+a33p2)-5.*(a33m4+a33p3))/2048.

  a33p3h6 = (150.*(a33p2+a33p1)-25.*(a33p3+a33i )+3.*(a33p4+a33m1))/256.  ! 6th-order approx
  a33m3h6 = (150.*(a33m2+a33m1)-25.*(a33m3+a33i )+3.*(a33m4+a33p1))/256.

  a33p5h4= ((a33p3+a33p2)*9.-(a33p4+a33p1))/16.                          ! 4th-order approx
  a33m5h4= ((a33m3+a33m2)*9.-(a33m4+a33m1))/16.

  a33p7h2= .5*(a33p4+a33p3) ! 2nd order approx
  a33m7h2= .5*(a33m4+a33m3)

 #End

#End

#endMacro


#beginMacro divScalarGradConservativeHigherOrder(operatorName,ORDER)
subroutine operatorName( nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b, \
  ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b, n1a,n1b,n2a,n2b,n3a,n3b, ca,cb, \
  dr,dx,rsxy,u,s,deriv,derivOption,gridType,order,averagingType,dir1,dir2 )
c ===============================================================
c    Sixth- or eighth- order Conservative form of the operators:
c           Laplace
c           div( s grad )
c           div( tensor Grad )
c
c  Evaluate:
c     deriv(n1a:n1b,n2a:n2b,n3a:n3b,ca:cb) = Operator()
c
c nd (input) : number of space dimensions
c nd1a,nd1b,nd2a,nd2b,nd3a,nd3b (input) : array dimensions for rsxy
c ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b (input) : array dimensions for u and s
c ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b (input) : array dimensions for deriv
c n1a,n1b,n2a,n2b,n3a,n3b, ca,cb (input): evaluate the operator at these points and components
c dr(1:3),dx(1:3)  (input): dr(axis) = grid spacing on unit square. dx(axis) = grid spacing for Cartesian grids
c rsxy(i1,i2,i3,1:nd,1:nd) (input) : Jacobian derivatives, e.g. rsxy(i1,i2,i3,1,2) = ry (not used if rectangular)
c u(i1,i2,i3,c) (input) : solution to apply the operator to
c s(i1,i2,i3) (input) : scalar coefficient (ignore for Laplace operator)
c deriv(i1,i2,i3,c) (output) : result is returned here
c derivOption : ignored
c gridType: 0=rectangular, 1=non-rectangular
c order : ignored
c averagingType : ignored
c dir1,dir2 : ingnored
c NOTES:
c   o This function assumes that there are enough ghost point values to evaluate the operator at all
c      the requested points. It is assumed that some other functions fill in the ghost point values
c      at the boundaries.
c
c Who to blame:
c    Bill Henshaw
c ===============================================================

implicit none
integer nd,nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndu1a,ndu1b,ndu2a,ndu2b,ndu3a,ndu3b,ndu4a,ndu4b,\
  ndd1a,ndd1b,ndd2a,ndd2b,ndd3a,ndd3b,ndd4a,ndd4b,n1a,n1b,n2a,n2b,n3a,n3b, ca,cb,\
  derivOption, gridType, order, averagingType, dir1, dir2

integer laplace,divScalarGrad,derivativeScalarDerivative,divTensorGrad
parameter(laplace=0,divScalarGrad=1,derivativeScalarDerivative=2,divTensorGrad=3)

real rsxy(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,1:nd,1:nd)
real u(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,ndu4a:ndu4b)
real s(ndu1a:ndu1b,ndu2a:ndu2b,ndu3a:ndu3b,0:*)
real deriv(ndd1a:ndd1b,ndd2a:ndd2b,ndd3a:ndd3b,ndd4a:ndd4b)
real dr(0:*),dx(0:*)

c.....local variables
integer i1,i2,i3,kd,c
real drsqi(0:2),jac,dxsqi(0:2)

integer rectangular,curvilinear
parameter( rectangular=0,curvilinear=1 )
c.......statement functions 
real rx,ry,rz,sx,sy,sz,tx,ty,tz
real dp1u,dp2u,dz1u,dz2u,dp1Cubedu,dp2Cubedu,dzdpdm1u,dzdpdm2u
real a11,a12,a22,a21, a11r,a12r,a22r,a21r
real a11i,a11m1,a11p1,a11m2,a11p2,a11mh,a11ph,a11p3h,a11m3h,a11p3,a11m3,a11ph2,a11mh2
real a11ph6,a11mh6,a11p3h4,a11ph4,a11mh4,a11m3h4,a11p3h2,a11m3h2,a11p5h2,a11m5h2
real a22i,a22m1,a22p1,a22m2,a22p2,a22mh,a22ph,a22p3h,a22m3h,a22p3,a22m3,a22ph2,a22mh2
real a22ph6,a22mh6,a22p3h4,a22ph4,a22mh4,a22m3h4,a22p3h2,a22m3h2,a22p5h2,a22m5h2
real a33i,a33m1,a33p1,a33m2,a33p2,a33mh,a33ph,a33p3h,a33m3h,a33p3,a33m3,a33ph2,a33mh2
real a33ph6,a33mh6,a33p3h4,a33ph4,a33mh4,a33m3h4,a33p3h2,a33m3h2,a33p5h2,a33m5h2
real a113dr,a223dr,a333dr,a123dr,a133dr,a233dr,a213dr,a313dr,a323dr
#If #ORDER == "8"
real a11m4,a11p4,a11ph8,a11mh8,a11p3h6,a11m3h6,a11p5h4,a11m5h4,a11p7h2,a11m7h2
real a22m4,a22p4,a22ph8,a22mh8,a22p3h6,a22m3h6,a22p5h4,a22m5h4,a22p7h2,a22m7h2
real a33m4,a33p4,a33ph8,a33mh8,a33p3h6,a33m3h6,a33p5h4,a33m5h4,a33p7h2,a33m7h2
#End
real ajac

real ajac3d,a113d,a223d,a333d,a123d,a133d,a233d,a213d,a313d,a323d
real dp1r,dp1s,dp1t,dzr,dzs,dzt,dp3r,dp3s,dp3t,dp5r,dp5s,dp5t,dzpmr,dzpms,dzpmt,dp7r,dp7s,dp7t

c.......statement functions for jacobian
rx(i1,i2,i3)=rsxy(i1,i2,i3,1,1)
ry(i1,i2,i3)=rsxy(i1,i2,i3,1,2)
rz(i1,i2,i3)=rsxy(i1,i2,i3,1,3)
sx(i1,i2,i3)=rsxy(i1,i2,i3,2,1)
sy(i1,i2,i3)=rsxy(i1,i2,i3,2,2)
sz(i1,i2,i3)=rsxy(i1,i2,i3,2,3)
tx(i1,i2,i3)=rsxy(i1,i2,i3,3,1)
ty(i1,i2,i3)=rsxy(i1,i2,i3,3,2)
tz(i1,i2,i3)=rsxy(i1,i2,i3,3,3)

c D+
dp1r(i1,i2,i3)= u(i1+1,i2,i3,c)-u(i1,i2,i3,c) 
dp1s(i1,i2,i3)= u(i1,i2+1,i3,c)-u(i1,i2,i3,c) 
dp1t(i1,i2,i3)= u(i1,i2,i3+1,c)-u(i1,i2,i3,c) 

c D0
dzr(i1,i2,i3) = u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c) 
dzs(i1,i2,i3) = u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c) 
dzt(i1,i2,i3) = u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c) 

c (D+)^3
dp3r(i1,i2,i3)= u(i1+3,i2,i3,c)-3.*(u(i1+2,i2,i3,c)-u(i1+1,i2,i3,c))-u(i1,i2,i3,c)
dp3s(i1,i2,i3)= u(i1,i2+3,i3,c)-3.*(u(i1,i2+2,i3,c)-u(i1,i2+1,i3,c))-u(i1,i2,i3,c)
dp3t(i1,i2,i3)= u(i1,i2,i3+3,c)-3.*(u(i1,i2,i3+2,c)-u(i1,i2,i3+1,c))-u(i1,i2,i3,c)

c (D+)^5
dp5r(i1,i2,i3)= u(i1+5,i2,i3,c)-5.*(u(i1+4,i2,i3,c)-u(i1+1,i2,i3,c))+10.*(u(i1+3,i2,i3,c)-u(i1+2,i2,i3,c))-u(i1,i2,i3,c)
dp5s(i1,i2,i3)= u(i1,i2+5,i3,c)-5.*(u(i1,i2+4,i3,c)-u(i1,i2+1,i3,c))+10.*(u(i1,i2+3,i3,c)-u(i1,i2+2,i3,c))-u(i1,i2,i3,c)
dp5t(i1,i2,i3)= u(i1,i2,i3+5,c)-5.*(u(i1,i2,i3+4,c)-u(i1,i2,i3+1,c))+10.*(u(i1,i2,i3+3,c)-u(i1,i2,i3+2,c))-u(i1,i2,i3,c)

c (D+)^7
dp7r(i1,i2,i3)= u(i1+7,i2,i3,c)-7.*(u(i1+6,i2,i3,c)-u(i1+1,i2,i3,c))+21.*(u(i1+5,i2,i3,c)-u(i1+2,i2,i3,c))\
                              -35.*(u(i1+4,i2,i3,c)-u(i1+3,i2,i3,c))-u(i1,i2,i3,c)
dp7s(i1,i2,i3)= u(i1,i2+7,i3,c)-7.*(u(i1,i2+6,i3,c)-u(i1,i2+1,i3,c))+21.*(u(i1,i2+5,i3,c)-u(i1,i2+2,i3,c))\
                              -35.*(u(i1,i2+4,i3,c)-u(i1,i2+3,i3,c))-u(i1,i2,i3,c)
dp7t(i1,i2,i3)= u(i1,i2,i3+7,c)-7.*(u(i1,i2,i3+6,c)-u(i1,i2,i3+1,c))+21.*(u(i1,i2,i3+5,c)-u(i1,i2,i3+2,c))\
                              -35.*(u(i1,i2,i3+4,c)-u(i1,i2,i3+3,c))-u(i1,i2,i3,c)

c D0 D+ D-
dzpmr(i1,i2,i3)= u(i1+2,i2,i3,c)-2.*(u(i1+1,i2,i3,c)-u(i1-1,i2,i3,c))-u(i1-2,i2,i3,c)
dzpms(i1,i2,i3)= u(i1,i2+2,i3,c)-2.*(u(i1,i2+1,i3,c)-u(i1,i2-1,i3,c))-u(i1,i2-2,i3,c)
dzpmt(i1,i2,i3)= u(i1,i2,i3+2,c)-2.*(u(i1,i2,i3+1,c)-u(i1,i2,i3-1,c))-u(i1,i2,i3-2,c)

ajac(i1,i2,i3)=rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3)

#If (#operatorName == "laplace6Cons") || (#operatorName == "laplace8Cons")
! "
a11(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)/ajac(i1,i2,i3)
a22(i1,i2,i3)=(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)/ajac(i1,i2,i3)
a12(i1,i2,i3)=(rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3))/ajac(i1,i2,i3)

#Elif (#operatorName == "divScalarGrad6Cons") || (#operatorName == "divScalarGrad8Cons")
!"
a11(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac(i1,i2,i3)
a22(i1,i2,i3)=(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac(i1,i2,i3)
a12(i1,i2,i3)=(rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3))*s(i1,i2,i3,0)/ajac(i1,i2,i3)

#Else
! divTensorGrad
a11(i1,i2,i3) = (s(i1,i2,i3,0)*rx(i1,i2,i3)**2+\
                (s(i1,i2,i3,1)+\
                 s(i1,i2,i3,2))*rx(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,3)*ry(i1,i2,i3)**2)/ajac(i1,i2,i3)
a12(i1,i2,i3) = (s(i1,i2,i3,0)*rx(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ry(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,2)*rx(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,3)*ry(i1,i2,i3)*sy(i1,i2,i3))/ajac(i1,i2,i3) 
a22(i1,i2,i3) = (s(i1,i2,i3,0)*sx(i1,i2,i3)**2+\
                (s(i1,i2,i3,1)+\
                 s(i1,i2,i3,2))*sx(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,3)*sy(i1,i2,i3)**2)/ajac(i1,i2,i3) 
a21(i1,i2,i3) = (s(i1,i2,i3,0)*sx(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,1)*sy(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,2)*sx(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,3)*sy(i1,i2,i3)*ry(i1,i2,i3))/ajac(i1,i2,i3)

! rectangular grid versions
a11r(i1,i2,i3) = s(i1,i2,i3,0)
a21r(i1,i2,i3) = s(i1,i2,i3,1)
a12r(i1,i2,i3) = s(i1,i2,i3,2)
a22r(i1,i2,i3) = s(i1,i2,i3,3)
#End

ajac3d(i1,i2,i3)=(rx(i1,i2,i3)*sy(i1,i2,i3)-ry(i1,i2,i3)*sx(i1,i2,i3))*tz(i1,i2,i3)+\
                 (ry(i1,i2,i3)*sz(i1,i2,i3)-rz(i1,i2,i3)*sy(i1,i2,i3))*tx(i1,i2,i3)+\
                 (rz(i1,i2,i3)*sx(i1,i2,i3)-rx(i1,i2,i3)*sz(i1,i2,i3))*ty(i1,i2,i3)

#If (#operatorName == "laplace6Cons") || (#operatorName == "laplace8Cons")
! "

a113d(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)/ajac3d(i1,i2,i3)
a223d(i1,i2,i3)=(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)/ajac3d(i1,i2,i3)
a333d(i1,i2,i3)=(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)/ajac3d(i1,i2,i3)
a123d(i1,i2,i3)=(rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))/ajac3d(i1,i2,i3)
a133d(i1,i2,i3)=(rx(i1,i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))/ajac3d(i1,i2,i3)
a233d(i1,i2,i3)=(sx(i1,i2,i3)*tx(i1,i2,i3)+sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))/ajac3d(i1,i2,i3)

#Elif (#operatorName == "divScalarGrad6Cons") || (#operatorName == "divScalarGrad8Cons")
!"
a113d(i1,i2,i3)=(rx(i1,i2,i3)**2+ry(i1,i2,i3)**2+rz(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac3d(i1,i2,i3)
a223d(i1,i2,i3)=(sx(i1,i2,i3)**2+sy(i1,i2,i3)**2+sz(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac3d(i1,i2,i3)
a333d(i1,i2,i3)=(tx(i1,i2,i3)**2+ty(i1,i2,i3)**2+tz(i1,i2,i3)**2)*s(i1,i2,i3,0)/ajac3d(i1,i2,i3)
a123d(i1,i2,i3)=(rx(i1,i2,i3)*sx(i1,i2,i3)+ry(i1,i2,i3)*sy(i1,i2,i3)+rz(i1,i2,i3)*sz(i1,i2,i3))\
                                                                 *s(i1,i2,i3,0)/ajac3d(i1,i2,i3)
a133d(i1,i2,i3)=(rx(i1,i2,i3)*tx(i1,i2,i3)+ry(i1,i2,i3)*ty(i1,i2,i3)+rz(i1,i2,i3)*tz(i1,i2,i3))\
                                                                 *s(i1,i2,i3,0)/ajac3d(i1,i2,i3)
a233d(i1,i2,i3)=(sx(i1,i2,i3)*tx(i1,i2,i3)+sy(i1,i2,i3)*ty(i1,i2,i3)+sz(i1,i2,i3)*tz(i1,i2,i3))\
                                                                 *s(i1,i2,i3,0)/ajac3d(i1,i2,i3)
#Else

! divTensorGrad
a113d(i1,i2,i3)=(s(i1,i2,i3,0)*rx(i1,i2,i3)**2+\
                 s(i1,i2,i3,4)*ry(i1,i2,i3)**2+\
                 s(i1,i2,i3,8)*rz(i1,i2,i3)**2+\
                 (s(i1,i2,i3,3)+s(i1,i2,i3,1))*rx(i1,i2,i3)*ry(i1,i2,i3)+\
                 (s(i1,i2,i3,6)+s(i1,i2,i3,2))*rx(i1,i2,i3)*rz(i1,i2,i3)+\
                 (s(i1,i2,i3,7)+s(i1,i2,i3,5))*ry(i1,i2,i3)*rz(i1,i2,i3))/ajac3d(i1,i2,i3)
a223d(i1,i2,i3)=(s(i1,i2,i3,0)*sx(i1,i2,i3)**2+\
                 s(i1,i2,i3,4)*sy(i1,i2,i3)**2+\
                 s(i1,i2,i3,8)*sz(i1,i2,i3)**2+\
                 (s(i1,i2,i3,3)+s(i1,i2,i3,1))*sx(i1,i2,i3)*sy(i1,i2,i3)+\
                 (s(i1,i2,i3,6)+s(i1,i2,i3,2))*sx(i1,i2,i3)*sz(i1,i2,i3)+\
                 (s(i1,i2,i3,7)+s(i1,i2,i3,5))*sy(i1,i2,i3)*sz(i1,i2,i3))/ajac3d(i1,i2,i3)
a333d(i1,i2,i3)=(s(i1,i2,i3,0)*tx(i1,i2,i3)**2+\
                 s(i1,i2,i3,4)*ty(i1,i2,i3)**2+\
                 s(i1,i2,i3,8)*tz(i1,i2,i3)**2+\
                 (s(i1,i2,i3,3)+s(i1,i2,i3,1))*tx(i1,i2,i3)*ty(i1,i2,i3)+\
                 (s(i1,i2,i3,6)+s(i1,i2,i3,2))*tx(i1,i2,i3)*tz(i1,i2,i3)+\
                 (s(i1,i2,i3,7)+s(i1,i2,i3,5))*ty(i1,i2,i3)*tz(i1,i2,i3))/ajac3d(i1,i2,i3)
a123d(i1,i2,i3)=(s(i1,i2,i3,0)*rx(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*ry(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,8)*rz(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*rx(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,6)*rx(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ry(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*ry(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*rz(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*rz(i1,i2,i3)*sy(i1,i2,i3))/ajac3d(i1,i2,i3)
a133d(i1,i2,i3)=(s(i1,i2,i3,0)*rx(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*ry(i1,i2,i3)*ty(i1,i2,i3)+\
                 s(i1,i2,i3,8)*rz(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*rx(i1,i2,i3)*ty(i1,i2,i3)+\
                 s(i1,i2,i3,6)*rx(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ry(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*ry(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*rz(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*rz(i1,i2,i3)*ty(i1,i2,i3))/ajac3d(i1,i2,i3) 
a233d(i1,i2,i3)=(s(i1,i2,i3,0)*sx(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*sy(i1,i2,i3)*ty(i1,i2,i3)+\
                 s(i1,i2,i3,8)*sz(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*sx(i1,i2,i3)*ty(i1,i2,i3)+\
                 s(i1,i2,i3,6)*sx(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*sy(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*sy(i1,i2,i3)*tz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*sz(i1,i2,i3)*tx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*sz(i1,i2,i3)*ty(i1,i2,i3))/ajac3d(i1,i2,i3)  
a213d(i1,i2,i3)=(s(i1,i2,i3,0)*sx(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*sy(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,8)*sz(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*sx(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,6)*sx(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*sy(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*sy(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*sz(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*sz(i1,i2,i3)*ry(i1,i2,i3))/ajac3d(i1,i2,i3) 
a313d(i1,i2,i3)=(s(i1,i2,i3,0)*tx(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*ty(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,8)*tz(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*tx(i1,i2,i3)*ry(i1,i2,i3)+\
                 s(i1,i2,i3,6)*tx(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ty(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*ty(i1,i2,i3)*rz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*tz(i1,i2,i3)*rx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*tz(i1,i2,i3)*ry(i1,i2,i3))/ajac3d(i1,i2,i3) 
a323d(i1,i2,i3)=(s(i1,i2,i3,0)*tx(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,4)*ty(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,8)*tz(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,3)*tx(i1,i2,i3)*sy(i1,i2,i3)+\
                 s(i1,i2,i3,6)*tx(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,1)*ty(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,7)*ty(i1,i2,i3)*sz(i1,i2,i3)+\
                 s(i1,i2,i3,2)*tz(i1,i2,i3)*sx(i1,i2,i3)+\
                 s(i1,i2,i3,5)*tz(i1,i2,i3)*sy(i1,i2,i3))/ajac3d(i1,i2,i3)


! rectangular grid versions
a113dr(i1,i2,i3) = s(i1,i2,i3,0)
a213dr(i1,i2,i3) = s(i1,i2,i3,1)
a313dr(i1,i2,i3) = s(i1,i2,i3,2)
a123dr(i1,i2,i3) = s(i1,i2,i3,3)
a223dr(i1,i2,i3) = s(i1,i2,i3,4)
a323dr(i1,i2,i3) = s(i1,i2,i3,5)
a133dr(i1,i2,i3) = s(i1,i2,i3,6)
a233dr(i1,i2,i3) = s(i1,i2,i3,7)
a333dr(i1,i2,i3) = s(i1,i2,i3,8)

#End
c........end statement functions


if( gridType.eq.curvilinear )then

  ! ******************************************************************
  ! *************Curvilinear Grid ************************************
  ! ******************************************************************

drsqi(0)=1./dr(0)**2
drsqi(1)=1./dr(1)**2
drsqi(2)=1./dr(2)**2


if( nd.eq.2 )then

#If #ORDER == "6"
c write(*,*) '****INSIDE dsgc6 2d'
#Else
c write(*,*) '****INSIDE dsgc8 2d'
#End

do c=ca,cb
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b

 defineMidValueCoefficients(2,DSG,ORDER)

 jac= ajac(i1,i2,i3)

#If #ORDER == "6"
 deriv(i1,i2,i3,c)=\
   (\
(-270*a11ph4*dp3r(i1-1,i2,i3)+270*a11mh4*dp3r(i1-2,i2,i3)+10*a11p3h4*dp3r(i1,i2,i3)-10*a11m3h4*dp3r(i1-3,i2,i3)+27*a11ph2*dp5r(i1-2,i2,i3)-27*a11mh2*dp5r(i1-3,i2,i3)+(720*a11ph4+5760*a11ph6+270*a11ph2)*dp1r(i1,i2,i3)+(-5760*a11mh6-720*a11mh4-270*a11mh2)*dp1r(i1-1,i2,i3)+(-240*a11p3h4-135*a11p3h2)*dp1r(i1+1,i2,i3)+(240*a11m3h4+135*a11m3h2)*dp1r(i1-2,i2,i3)-27*a11m5h2*dp1r(i1-3,i2,i3)+27*a11p5h2*dp1r(i1+2,i2,i3))/(5760.0*dr(0)**2)\
+(-270*a22ph4*dp3s(i1,i2-1,i3)+270*a22mh4*dp3s(i1,i2-2,i3)+10*a22p3h4*dp3s(i1,i2,i3)-10*a22m3h4*dp3s(i1,i2-3,i3)+27*a22ph2*dp5s(i1,i2-2,i3)-27*a22mh2*dp5s(i1,i2-3,i3)+(720*a22ph4+5760*a22ph6+270*a22ph2)*dp1s(i1,i2,i3)+(-5760*a22mh6-720*a22mh4-270*a22mh2)*dp1s(i1,i2-1,i3)+(-240*a22p3h4-135*a22p3h2)*dp1s(i1,i2+1,i3)+(240*a22m3h4+135*a22m3h2)*dp1s(i1,i2-2,i3)-27*a22m5h2*dp1s(i1,i2-3,i3)+27*a22p5h2*dp1s(i1,i2+2,i3))/(5760.0*dr(1)**2)\
 +((54*dzs(i1-2,i2,i3)-5*dzpms(i1-2,i2,i3))*a12(i1-2,i2,i3)+(-54*dzr(i1,i2+2,i3)+5*dzpmr(i1,i2+2,i3))*a12(i1,i2+2,i3)+(54*dzr(i1,i2-2,i3)-5*dzpmr(i1,i2-2,i3))*a12(i1,i2-2,i3)+(-54*dzs(i1+2,i2,i3)+5*dzpms(i1+2,i2,i3))*a12(i1+2,i2,i3)-6*a12(i1,i2-3,i3)*dzr(i1,i2-3,i3)+6*a12(i1,i2+3,i3)*dzr(i1,i2+3,i3)+6*a12(i1+3,i2,i3)*dzs(i1+3,i2,i3)-6*a12(i1-3,i2,i3)*dzs(i1-3,i2,i3)+(270*dzs(i1+1,i2,i3)+6*dzpms(i1+1,i2+1,i3)-52*dzpms(i1+1,i2,i3)+6*dzpms(i1+1,i2-1,i3))*a12(i1+1,i2,i3)+(-6*dzpms(i1-1,i2+1,i3)-270*dzs(i1-1,i2,i3)-6*dzpms(i1-1,i2-1,i3)+52*dzpms(i1-1,i2,i3))*a12(i1-1,i2,i3)+(-52*dzpmr(i1,i2+1,i3)+6*dzpmr(i1+1,i2+1,i3)+270*dzr(i1,i2+1,i3)+6*dzpmr(i1-1,i2+1,i3))*a12(i1,i2+1,i3)+(-6*dzpmr(i1+1,i2-1,i3)-6*dzpmr(i1-1,i2-1,i3)+52*dzpmr(i1,i2-1,i3)-270*dzr(i1,i2-1,i3))*a12(i1,i2-1,i3))/(720.0*dr(0)*dr(1))\
   )*jac
#Else
 deriv(i1,i2,i3,c)=\
   (\
((-630*a11ph2-15120*a11ph6)*dp3r(i1-1,i2,i3)+(15120*a11mh6+630*a11mh2)*dp3r(i1-2,i2,i3)+(315*a11p3h2+560*a11p3h6)*dp3r(i1,i2,i3)+(-560*a11m3h6-315*a11m3h2)*dp3r(i1-3,i2,i3)+63*a11m5h2*dp3r(i1-4,i2,i3)-63*a11p5h2*dp3r(i1+1,i2,i3)+(-7875*a11mh2-15120*a11mh4-40320*a11mh6-322560*a11mh8)*dp1r(i1-1,i2,i3)+(15120*a11ph4+40320*a11ph6+7875*a11ph2+322560*a11ph8)*dp1r(i1,i2,i3)+(-4725*a11p3h2-7560*a11p3h4-13440*a11p3h6)*dp1r(i1+1,i2,i3)+(4725*a11m3h2+13440*a11m3h6+7560*a11m3h4)*dp1r(i1-2,i2,i3)+(1575*a11p5h2+1512*a11p5h4)*dp1r(i1+2,i2,i3)+(-1512*a11m5h4-1575*a11m5h2)*dp1r(i1-3,i2,i3)-225*a11p7h2*dp1r(i1+3,i2,i3)+225*a11m7h2*dp1r(i1-4,i2,i3)+(1512*a11ph4+189*a11ph2)*dp5r(i1-2,i2,i3)+(-189*a11mh2-1512*a11mh4)*dp5r(i1-3,i2,i3)-63*a11p3h2*dp5r(i1-1,i2,i3)+63*a11m3h2*dp5r(i1-4,i2,i3)+225*a11mh2*dp7r(i1-4,i2,i3)-225*a11ph2*dp7r(i1-3,i2,i3))/(322560.0*dr(0)**2)\
  + ((-15120*a22ph6-630*a22ph2)*dp3s(i1,i2-1,i3)+(15120*a22mh6+630*a22mh2)*dp3s(i1,i2-2,i3)+(560*a22p3h6+315*a22p3h2)*dp3s(i1,i2,i3)+(-560*a22m3h6-315*a22m3h2)*dp3s(i1,i2-3,i3)+63*a22m5h2*dp3s(i1,i2-4,i3)-63*a22p5h2*dp3s(i1,i2+1,i3)+(-40320*a22mh6-15120*a22mh4-7875*a22mh2-322560*a22mh8)*dp1s(i1,i2-1,i3)+(40320*a22ph6+322560*a22ph8+7875*a22ph2+15120*a22ph4)*dp1s(i1,i2,i3)+(-7560*a22p3h4-4725*a22p3h2-13440*a22p3h6)*dp1s(i1,i2+1,i3)+(13440*a22m3h6+4725*a22m3h2+7560*a22m3h4)*dp1s(i1,i2-2,i3)+(1575*a22p5h2+1512*a22p5h4)*dp1s(i1,i2+2,i3)+(-1575*a22m5h2-1512*a22m5h4)*dp1s(i1,i2-3,i3)-225*a22p7h2*dp1s(i1,i2+3,i3)+225*a22m7h2*dp1s(i1,i2-4,i3)+(1512*a22ph4+189*a22ph2)*dp5s(i1,i2-2,i3)+(-189*a22mh2-1512*a22mh4)*dp5s(i1,i2-3,i3)-63*a22p3h2*dp5s(i1,i2-1,i3)+63*a22m3h2*dp5s(i1,i2-4,i3)+225*a22mh2*dp7s(i1,i2-4,i3)-225*a22ph2*dp7s(i1,i2-3,i3))/(322560.0*dr(1)**2)\
  + ((7*dzpms(i1-2,i2+1,i3)-77*dzpms(i1-2,i2,i3)+7*dzpms(i1-2,i2-1,i3)+504*dzs(i1-2,i2,i3))*a12(i1-2,i2,i3)+(-7*dzpmr(i1+1,i2+2,i3)+77*dzpmr(i1,i2+2,i3)-7*dzpmr(i1-1,i2+2,i3)-504*dzr(i1,i2+2,i3))*a12(i1,i2+2,i3)+(7*dzpmr(i1-1,i2-2,i3)+7*dzpmr(i1+1,i2-2,i3)-77*dzpmr(i1,i2-2,i3)+504*dzr(i1,i2-2,i3))*a12(i1,i2-2,i3)+(77*dzpms(i1+2,i2,i3)-7*dzpms(i1+2,i2-1,i3)-7*dzpms(i1+2,i2+1,i3)-504*dzs(i1+2,i2,i3))*a12(i1+2,i2,i3)+9*a12(i1,i2-4,i3)*dzr(i1,i2-4,i3)-9*a12(i1,i2+4,i3)*dzr(i1,i2+4,i3)+(-96*dzr(i1,i2-3,i3)+7*dzpmr(i1,i2-3,i3))*a12(i1,i2-3,i3)+(-7*dzpmr(i1,i2+3,i3)+96*dzr(i1,i2+3,i3))*a12(i1,i2+3,i3)+(-7*dzpms(i1+3,i2,i3)+96*dzs(i1+3,i2,i3))*a12(i1+3,i2,i3)+(-96*dzs(i1-3,i2,i3)+7*dzpms(i1-3,i2,i3))*a12(i1-3,i2,i3)+9*a12(i1-4,i2,i3)*dzs(i1-4,i2,i3)-9*a12(i1+4,i2,i3)*dzs(i1+4,i2,i3)+(92*dzpms(i1+1,i2-1,i3)+92*dzpms(i1+1,i2+1,i3)+2016*dzs(i1+1,i2,i3)-481*dzpms(i1+1,i2,i3)-9*dzpms(i1+1,i2+2,i3)-9*dzpms(i1+1,i2-2,i3))*a12(i1+1,i2,i3)+(9*dzpms(i1-1,i2+2,i3)+9*dzpms(i1-1,i2-2,i3)+481*dzpms(i1-1,i2,i3)-92*dzpms(i1-1,i2+1,i3)-92*dzpms(i1-1,i2-1,i3)-2016*dzs(i1-1,i2,i3))*a12(i1-1,i2,i3)+(-9*dzpmr(i1-2,i2+1,i3)-481*dzpmr(i1,i2+1,i3)-9*dzpmr(i1+2,i2+1,i3)+92*dzpmr(i1+1,i2+1,i3)+92*dzpmr(i1-1,i2+1,i3)+2016*dzr(i1,i2+1,i3))*a12(i1,i2+1,i3)+(-92*dzpmr(i1-1,i2-1,i3)-92*dzpmr(i1+1,i2-1,i3)+9*dzpmr(i1+2,i2-1,i3)-2016*dzr(i1,i2-1,i3)+481*dzpmr(i1,i2-1,i3)+9*dzpmr(i1-2,i2-1,i3))*a12(i1,i2-1,i3))/(5040.0*dr(0)*dr(1))\
   )*jac
#End

end do
end do
end do
end do

else if( nd.eq.3 )then

#If #ORDER == "6"
c write(*,*) '****INSIDE dsgc6 3d'
#Else
c write(*,*) '****INSIDE dsgc8 3d'
#End

do c=ca,cb
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b

 defineMidValueCoefficients(3,DSG,ORDER)
 
 jac= ajac3d(i1,i2,i3)
#If #ORDER == "6"

 deriv(i1,i2,i3,c)=\
   (\
(27*a11ph2*dp5r(i1-2,i2,i3)-27*a11mh2*dp5r(i1-3,i2,i3)+10*a11p3h4*dp3r(i1,i2,i3)-10*a11m3h4*dp3r(i1-3,i2,i3)-270*a11ph4*dp3r(i1-1,i2,i3)+270*a11mh4*dp3r(i1-2,i2,i3)+27*a11p5h2*dp1r(i1+2,i2,i3)+(5760*a11ph6+720*a11ph4+270*a11ph2)*dp1r(i1,i2,i3)+(-270*a11mh2-5760*a11mh6-720*a11mh4)*dp1r(i1-1,i2,i3)-27*a11m5h2*dp1r(i1-3,i2,i3)+(135*a11m3h2+240*a11m3h4)*dp1r(i1-2,i2,i3)+(-240*a11p3h4-135*a11p3h2)*dp1r(i1+1,i2,i3))/(5760.0*dr(0)**2)\
  + (27*a22ph2*dp5s(i1,i2-2,i3)-27*a22mh2*dp5s(i1,i2-3,i3)+10*a22p3h4*dp3s(i1,i2,i3)-10*a22m3h4*dp3s(i1,i2-3,i3)-270*a22ph4*dp3s(i1,i2-1,i3)+270*a22mh4*dp3s(i1,i2-2,i3)+27*a22p5h2*dp1s(i1,i2+2,i3)+(5760*a22ph6+720*a22ph4+270*a22ph2)*dp1s(i1,i2,i3)+(-270*a22mh2-5760*a22mh6-720*a22mh4)*dp1s(i1,i2-1,i3)-27*a22m5h2*dp1s(i1,i2-3,i3)+(135*a22m3h2+240*a22m3h4)*dp1s(i1,i2-2,i3)+(-240*a22p3h4-135*a22p3h2)*dp1s(i1,i2+1,i3))/(5760.0*dr(1)**2)\
  + (27*a33ph2*dp5t(i1,i2,i3-2)-27*a33mh2*dp5t(i1,i2,i3-3)+10*a33p3h4*dp3t(i1,i2,i3)-10*a33m3h4*dp3t(i1,i2,i3-3)-270*a33ph4*dp3t(i1,i2,i3-1)+270*a33mh4*dp3t(i1,i2,i3-2)+27*a33p5h2*dp1t(i1,i2,i3+2)+(5760*a33ph6+720*a33ph4+270*a33ph2)*dp1t(i1,i2,i3)+(-270*a33mh2-5760*a33mh6-720*a33mh4)*dp1t(i1,i2,i3-1)-27*a33m5h2*dp1t(i1,i2,i3-3)+(135*a33m3h2+240*a33m3h4)*dp1t(i1,i2,i3-2)+(-240*a33p3h4-135*a33p3h2)*dp1t(i1,i2,i3+1))/(5760.0*dr(2)**2)\
  + ((5*dzpmr(i1,i2+2,i3)-54*dzr(i1,i2+2,i3))*a123d(i1,i2+2,i3)+(-5*dzpmr(i1,i2-2,i3)+54*dzr(i1,i2-2,i3))*a123d(i1,i2-2,i3)+(270*dzs(i1+1,i2,i3)+6*dzpms(i1+1,i2-1,i3)-52*dzpms(i1+1,i2,i3)+6*dzpms(i1+1,i2+1,i3))*a123d(i1+1,i2,i3)+(-6*dzpms(i1-1,i2+1,i3)-270*dzs(i1-1,i2,i3)-6*dzpms(i1-1,i2-1,i3)+52*dzpms(i1-1,i2,i3))*a123d(i1-1,i2,i3)+(270*dzr(i1,i2+1,i3)-52*dzpmr(i1,i2+1,i3)+6*dzpmr(i1+1,i2+1,i3)+6*dzpmr(i1-1,i2+1,i3))*a123d(i1,i2+1,i3)+(52*dzpmr(i1,i2-1,i3)-6*dzpmr(i1-1,i2-1,i3)-6*dzpmr(i1+1,i2-1,i3)-270*dzr(i1,i2-1,i3))*a123d(i1,i2-1,i3)-6*a123d(i1,i2-3,i3)*dzr(i1,i2-3,i3)+6*a123d(i1,i2+3,i3)*dzr(i1,i2+3,i3)+(5*dzpms(i1+2,i2,i3)-54*dzs(i1+2,i2,i3))*a123d(i1+2,i2,i3)+(-5*dzpms(i1-2,i2,i3)+54*dzs(i1-2,i2,i3))*a123d(i1-2,i2,i3)-6*a123d(i1-3,i2,i3)*dzs(i1-3,i2,i3)+6*a123d(i1+3,i2,i3)*dzs(i1+3,i2,i3))/(720.0*dr(0)*dr(1))\
  + ((270*dzt(i1+1,i2,i3)-52*dzpmt(i1+1,i2,i3)+6*dzpmt(i1+1,i2,i3+1)+6*dzpmt(i1+1,i2,i3-1))*a133d(i1+1,i2,i3)+(-6*dzpmt(i1-1,i2,i3+1)+52*dzpmt(i1-1,i2,i3)-270*dzt(i1-1,i2,i3)-6*dzpmt(i1-1,i2,i3-1))*a133d(i1-1,i2,i3)+(6*dzpmr(i1+1,i2,i3+1)-52*dzpmr(i1,i2,i3+1)+6*dzpmr(i1-1,i2,i3+1)+270*dzr(i1,i2,i3+1))*a133d(i1,i2,i3+1)+(-270*dzr(i1,i2,i3-1)-6*dzpmr(i1+1,i2,i3-1)+52*dzpmr(i1,i2,i3-1)-6*dzpmr(i1-1,i2,i3-1))*a133d(i1,i2,i3-1)+(5*dzpmt(i1+2,i2,i3)-54*dzt(i1+2,i2,i3))*a133d(i1+2,i2,i3)+(-5*dzpmt(i1-2,i2,i3)+54*dzt(i1-2,i2,i3))*a133d(i1-2,i2,i3)+6*a133d(i1+3,i2,i3)*dzt(i1+3,i2,i3)-6*a133d(i1-3,i2,i3)*dzt(i1-3,i2,i3)+(5*dzpmr(i1,i2,i3+2)-54*dzr(i1,i2,i3+2))*a133d(i1,i2,i3+2)+(54*dzr(i1,i2,i3-2)-5*dzpmr(i1,i2,i3-2))*a133d(i1,i2,i3-2)+6*a133d(i1,i2,i3+3)*dzr(i1,i2,i3+3)-6*a133d(i1,i2,i3-3)*dzr(i1,i2,i3-3))/(720.0*dr(0)*dr(2))\
  + ((5*dzpmt(i1,i2+2,i3)-54*dzt(i1,i2+2,i3))*a233d(i1,i2+2,i3)+(54*dzt(i1,i2-2,i3)-5*dzpmt(i1,i2-2,i3))*a233d(i1,i2-2,i3)+6*a233d(i1,i2+3,i3)*dzt(i1,i2+3,i3)-6*a233d(i1,i2-3,i3)*dzt(i1,i2-3,i3)+(270*dzt(i1,i2+1,i3)-52*dzpmt(i1,i2+1,i3)+6*dzpmt(i1,i2+1,i3-1)+6*dzpmt(i1,i2+1,i3+1))*a233d(i1,i2+1,i3)+(-6*dzpmt(i1,i2-1,i3+1)+52*dzpmt(i1,i2-1,i3)-6*dzpmt(i1,i2-1,i3-1)-270*dzt(i1,i2-1,i3))*a233d(i1,i2-1,i3)+6*a233d(i1,i2,i3+3)*dzs(i1,i2,i3+3)-6*a233d(i1,i2,i3-3)*dzs(i1,i2,i3-3)+(-52*dzpms(i1,i2,i3+1)+270*dzs(i1,i2,i3+1)+6*dzpms(i1,i2-1,i3+1)+6*dzpms(i1,i2+1,i3+1))*a233d(i1,i2,i3+1)+(52*dzpms(i1,i2,i3-1)-270*dzs(i1,i2,i3-1)-6*dzpms(i1,i2-1,i3-1)-6*dzpms(i1,i2+1,i3-1))*a233d(i1,i2,i3-1)+(5*dzpms(i1,i2,i3+2)-54*dzs(i1,i2,i3+2))*a233d(i1,i2,i3+2)+(54*dzs(i1,i2,i3-2)-5*dzpms(i1,i2,i3-2))*a233d(i1,i2,i3-2))/(720.0*dr(1)*dr(2))\
   )*jac

#Else
 deriv(i1,i2,i3,c)=\
   (\
((-630*a11ph2-15120*a11ph6)*dp3r(i1-1,i2,i3)+(15120*a11mh6+630*a11mh2)*dp3r(i1-2,i2,i3)+(315*a11p3h2+560*a11p3h6)*dp3r(i1,i2,i3)+(-560*a11m3h6-315*a11m3h2)*dp3r(i1-3,i2,i3)+63*a11m5h2*dp3r(i1-4,i2,i3)-63*a11p5h2*dp3r(i1+1,i2,i3)+(-7875*a11mh2-15120*a11mh4-40320*a11mh6-322560*a11mh8)*dp1r(i1-1,i2,i3)+(15120*a11ph4+40320*a11ph6+7875*a11ph2+322560*a11ph8)*dp1r(i1,i2,i3)+(-4725*a11p3h2-7560*a11p3h4-13440*a11p3h6)*dp1r(i1+1,i2,i3)+(4725*a11m3h2+13440*a11m3h6+7560*a11m3h4)*dp1r(i1-2,i2,i3)+(1575*a11p5h2+1512*a11p5h4)*dp1r(i1+2,i2,i3)+(-1512*a11m5h4-1575*a11m5h2)*dp1r(i1-3,i2,i3)-225*a11p7h2*dp1r(i1+3,i2,i3)+225*a11m7h2*dp1r(i1-4,i2,i3)+(1512*a11ph4+189*a11ph2)*dp5r(i1-2,i2,i3)+(-189*a11mh2-1512*a11mh4)*dp5r(i1-3,i2,i3)-63*a11p3h2*dp5r(i1-1,i2,i3)+63*a11m3h2*dp5r(i1-4,i2,i3)+225*a11mh2*dp7r(i1-4,i2,i3)-225*a11ph2*dp7r(i1-3,i2,i3))/(322560.0*dr(0)**2)\
  + ((-15120*a22ph6-630*a22ph2)*dp3s(i1,i2-1,i3)+(15120*a22mh6+630*a22mh2)*dp3s(i1,i2-2,i3)+(560*a22p3h6+315*a22p3h2)*dp3s(i1,i2,i3)+(-560*a22m3h6-315*a22m3h2)*dp3s(i1,i2-3,i3)+63*a22m5h2*dp3s(i1,i2-4,i3)-63*a22p5h2*dp3s(i1,i2+1,i3)+(-40320*a22mh6-15120*a22mh4-7875*a22mh2-322560*a22mh8)*dp1s(i1,i2-1,i3)+(40320*a22ph6+322560*a22ph8+7875*a22ph2+15120*a22ph4)*dp1s(i1,i2,i3)+(-7560*a22p3h4-4725*a22p3h2-13440*a22p3h6)*dp1s(i1,i2+1,i3)+(13440*a22m3h6+4725*a22m3h2+7560*a22m3h4)*dp1s(i1,i2-2,i3)+(1575*a22p5h2+1512*a22p5h4)*dp1s(i1,i2+2,i3)+(-1575*a22m5h2-1512*a22m5h4)*dp1s(i1,i2-3,i3)-225*a22p7h2*dp1s(i1,i2+3,i3)+225*a22m7h2*dp1s(i1,i2-4,i3)+(1512*a22ph4+189*a22ph2)*dp5s(i1,i2-2,i3)+(-189*a22mh2-1512*a22mh4)*dp5s(i1,i2-3,i3)-63*a22p3h2*dp5s(i1,i2-1,i3)+63*a22m3h2*dp5s(i1,i2-4,i3)+225*a22mh2*dp7s(i1,i2-4,i3)-225*a22ph2*dp7s(i1,i2-3,i3))/(322560.0*dr(1)**2)\
  + ((-15120*a33ph6-630*a33ph2)*dp3t(i1,i2,i3-1)+(15120*a33mh6+630*a33mh2)*dp3t(i1,i2,i3-2)+(560*a33p3h6+315*a33p3h2)*dp3t(i1,i2,i3)+(-560*a33m3h6-315*a33m3h2)*dp3t(i1,i2,i3-3)+63*a33m5h2*dp3t(i1,i2,i3-4)-63*a33p5h2*dp3t(i1,i2,i3+1)+(-40320*a33mh6-15120*a33mh4-7875*a33mh2-322560*a33mh8)*dp1t(i1,i2,i3-1)+(40320*a33ph6+322560*a33ph8+7875*a33ph2+15120*a33ph4)*dp1t(i1,i2,i3)+(-7560*a33p3h4-4725*a33p3h2-13440*a33p3h6)*dp1t(i1,i2,i3+1)+(13440*a33m3h6+4725*a33m3h2+7560*a33m3h4)*dp1t(i1,i2,i3-2)+(1575*a33p5h2+1512*a33p5h4)*dp1t(i1,i2,i3+2)+(-1575*a33m5h2-1512*a33m5h4)*dp1t(i1,i2,i3-3)-225*a33p7h2*dp1t(i1,i2,i3+3)+225*a33m7h2*dp1t(i1,i2,i3-4)+(1512*a33ph4+189*a33ph2)*dp5t(i1,i2,i3-2)+(-189*a33mh2-1512*a33mh4)*dp5t(i1,i2,i3-3)-63*a33p3h2*dp5t(i1,i2,i3-1)+63*a33m3h2*dp5t(i1,i2,i3-4)+225*a33mh2*dp7t(i1,i2,i3-4)-225*a33ph2*dp7t(i1,i2,i3-3))/(322560.0*dr(2)**2)\
  + ((2016*dzs(i1+1,i2,i3)-481*dzpms(i1+1,i2,i3)+92*dzpms(i1+1,i2+1,i3)-9*dzpms(i1+1,i2+2,i3)-9*dzpms(i1+1,i2-2,i3)+92*dzpms(i1+1,i2-1,i3))*a123d(i1+1,i2,i3)+(9*dzpms(i1-1,i2-2,i3)-92*dzpms(i1-1,i2+1,i3)+481*dzpms(i1-1,i2,i3)-92*dzpms(i1-1,i2-1,i3)-2016*dzs(i1-1,i2,i3)+9*dzpms(i1-1,i2+2,i3))*a123d(i1-1,i2,i3)+(92*dzpmr(i1+1,i2+1,i3)-481*dzpmr(i1,i2+1,i3)-9*dzpmr(i1+2,i2+1,i3)-9*dzpmr(i1-2,i2+1,i3)+92*dzpmr(i1-1,i2+1,i3)+2016*dzr(i1,i2+1,i3))*a123d(i1,i2+1,i3)+(9*dzpmr(i1+2,i2-1,i3)+9*dzpmr(i1-2,i2-1,i3)+481*dzpmr(i1,i2-1,i3)-92*dzpmr(i1-1,i2-1,i3)-92*dzpmr(i1+1,i2-1,i3)-2016*dzr(i1,i2-1,i3))*a123d(i1,i2-1,i3)+9*a123d(i1-4,i2,i3)*dzs(i1-4,i2,i3)-9*a123d(i1+4,i2,i3)*dzs(i1+4,i2,i3)+(-7*dzpms(i1+2,i2-1,i3)-504*dzs(i1+2,i2,i3)+77*dzpms(i1+2,i2,i3)-7*dzpms(i1+2,i2+1,i3))*a123d(i1+2,i2,i3)+(7*dzpms(i1-2,i2+1,i3)+7*dzpms(i1-2,i2-1,i3)-77*dzpms(i1-2,i2,i3)+504*dzs(i1-2,i2,i3))*a123d(i1-2,i2,i3)+(-7*dzpms(i1+3,i2,i3)+96*dzs(i1+3,i2,i3))*a123d(i1+3,i2,i3)+(-96*dzs(i1-3,i2,i3)+7*dzpms(i1-3,i2,i3))*a123d(i1-3,i2,i3)+(-7*dzpmr(i1+1,i2+2,i3)-7*dzpmr(i1-1,i2+2,i3)+77*dzpmr(i1,i2+2,i3)-504*dzr(i1,i2+2,i3))*a123d(i1,i2+2,i3)+(-77*dzpmr(i1,i2-2,i3)+504*dzr(i1,i2-2,i3)+7*dzpmr(i1+1,i2-2,i3)+7*dzpmr(i1-1,i2-2,i3))*a123d(i1,i2-2,i3)+(-7*dzpmr(i1,i2+3,i3)+96*dzr(i1,i2+3,i3))*a123d(i1,i2+3,i3)+(-96*dzr(i1,i2-3,i3)+7*dzpmr(i1,i2-3,i3))*a123d(i1,i2-3,i3)-9*a123d(i1,i2+4,i3)*dzr(i1,i2+4,i3)+9*a123d(i1,i2-4,i3)*dzr(i1,i2-4,i3))/(5040.0*dr(0)*dr(1)))
 deriv(i1,i2,i3,c)=(deriv(i1,i2,i3,c)\
  + ((2016*dzt(i1+1,i2,i3)-481*dzpmt(i1+1,i2,i3)+92*dzpmt(i1+1,i2,i3+1)-9*dzpmt(i1+1,i2,i3+2)-9*dzpmt(i1+1,i2,i3-2)+92*dzpmt(i1+1,i2,i3-1))*a133d(i1+1,i2,i3)+(9*dzpmt(i1-1,i2,i3-2)-92*dzpmt(i1-1,i2,i3+1)+481*dzpmt(i1-1,i2,i3)-92*dzpmt(i1-1,i2,i3-1)-2016*dzt(i1-1,i2,i3)+9*dzpmt(i1-1,i2,i3+2))*a133d(i1-1,i2,i3)+(92*dzpmr(i1+1,i2,i3+1)-481*dzpmr(i1,i2,i3+1)-9*dzpmr(i1+2,i2,i3+1)-9*dzpmr(i1-2,i2,i3+1)+92*dzpmr(i1-1,i2,i3+1)+2016*dzr(i1,i2,i3+1))*a133d(i1,i2,i3+1)+(9*dzpmr(i1+2,i2,i3-1)+9*dzpmr(i1-2,i2,i3-1)+481*dzpmr(i1,i2,i3-1)-92*dzpmr(i1-1,i2,i3-1)-92*dzpmr(i1+1,i2,i3-1)-2016*dzr(i1,i2,i3-1))*a133d(i1,i2,i3-1)+9*a133d(i1-4,i2,i3)*dzt(i1-4,i2,i3)-9*a133d(i1+4,i2,i3)*dzt(i1+4,i2,i3)+(-7*dzpmt(i1+2,i2,i3-1)-504*dzt(i1+2,i2,i3)+77*dzpmt(i1+2,i2,i3)-7*dzpmt(i1+2,i2,i3+1))*a133d(i1+2,i2,i3)+(7*dzpmt(i1-2,i2,i3+1)+7*dzpmt(i1-2,i2,i3-1)-77*dzpmt(i1-2,i2,i3)+504*dzt(i1-2,i2,i3))*a133d(i1-2,i2,i3)+(-7*dzpmt(i1+3,i2,i3)+96*dzt(i1+3,i2,i3))*a133d(i1+3,i2,i3)+(-96*dzt(i1-3,i2,i3)+7*dzpmt(i1-3,i2,i3))*a133d(i1-3,i2,i3)+(-7*dzpmr(i1+1,i2,i3+2)-7*dzpmr(i1-1,i2,i3+2)+77*dzpmr(i1,i2,i3+2)-504*dzr(i1,i2,i3+2))*a133d(i1,i2,i3+2)+(-77*dzpmr(i1,i2,i3-2)+504*dzr(i1,i2,i3-2)+7*dzpmr(i1+1,i2,i3-2)+7*dzpmr(i1-1,i2,i3-2))*a133d(i1,i2,i3-2)+(-7*dzpmr(i1,i2,i3+3)+96*dzr(i1,i2,i3+3))*a133d(i1,i2,i3+3)+(-96*dzr(i1,i2,i3-3)+7*dzpmr(i1,i2,i3-3))*a133d(i1,i2,i3-3)-9*a133d(i1,i2,i3+4)*dzr(i1,i2,i3+4)+9*a133d(i1,i2,i3-4)*dzr(i1,i2,i3-4))/(5040.0*dr(0)*dr(2))\
  + ((2016*dzt(i1,i2+1,i3)-481*dzpmt(i1,i2+1,i3)+92*dzpmt(i1,i2+1,i3+1)-9*dzpmt(i1,i2+1,i3+2)-9*dzpmt(i1,i2+1,i3-2)+92*dzpmt(i1,i2+1,i3-1))*a233d(i1,i2+1,i3)+(9*dzpmt(i1,i2-1,i3-2)-92*dzpmt(i1,i2-1,i3+1)+481*dzpmt(i1,i2-1,i3)-92*dzpmt(i1,i2-1,i3-1)-2016*dzt(i1,i2-1,i3)+9*dzpmt(i1,i2-1,i3+2))*a233d(i1,i2-1,i3)+(92*dzpms(i1,i2+1,i3+1)-481*dzpms(i1,i2,i3+1)-9*dzpms(i1,i2+2,i3+1)-9*dzpms(i1,i2-2,i3+1)+92*dzpms(i1,i2-1,i3+1)+2016*dzs(i1,i2,i3+1))*a233d(i1,i2,i3+1)+(9*dzpms(i1,i2+2,i3-1)+9*dzpms(i1,i2-2,i3-1)+481*dzpms(i1,i2,i3-1)-92*dzpms(i1,i2-1,i3-1)-92*dzpms(i1,i2+1,i3-1)-2016*dzs(i1,i2,i3-1))*a233d(i1,i2,i3-1)+9*a233d(i1,i2-4,i3)*dzt(i1,i2-4,i3)-9*a233d(i1,i2+4,i3)*dzt(i1,i2+4,i3)+(-7*dzpmt(i1,i2+2,i3-1)-504*dzt(i1,i2+2,i3)+77*dzpmt(i1,i2+2,i3)-7*dzpmt(i1,i2+2,i3+1))*a233d(i1,i2+2,i3)+(7*dzpmt(i1,i2-2,i3+1)+7*dzpmt(i1,i2-2,i3-1)-77*dzpmt(i1,i2-2,i3)+504*dzt(i1,i2-2,i3))*a233d(i1,i2-2,i3)+(-7*dzpmt(i1,i2+3,i3)+96*dzt(i1,i2+3,i3))*a233d(i1,i2+3,i3)+(-96*dzt(i1,i2-3,i3)+7*dzpmt(i1,i2-3,i3))*a233d(i1,i2-3,i3)+(-7*dzpms(i1,i2+1,i3+2)-7*dzpms(i1,i2-1,i3+2)+77*dzpms(i1,i2,i3+2)-504*dzs(i1,i2,i3+2))*a233d(i1,i2,i3+2)+(-77*dzpms(i1,i2,i3-2)+504*dzs(i1,i2,i3-2)+7*dzpms(i1,i2+1,i3-2)+7*dzpms(i1,i2-1,i3-2))*a233d(i1,i2,i3-2)+(-7*dzpms(i1,i2,i3+3)+96*dzs(i1,i2,i3+3))*a233d(i1,i2,i3+3)+(-96*dzs(i1,i2,i3-3)+7*dzpms(i1,i2,i3-3))*a233d(i1,i2,i3-3)-9*a233d(i1,i2,i3+4)*dzs(i1,i2,i3+4)+9*a233d(i1,i2,i3-4)*dzs(i1,i2,i3-4))/(5040.0*dr(1)*dr(2))\
   )*jac
#End

 ! write(*,*) 'i1,i2,i3,jac,deriv=',i1,i2,i3,jac,deriv(i1,i2,i3,c)

end do
end do
end do
end do

else
  write(*,*) 'dsgc4: ERROR invalid nd=',nd
  stop 11
end if

else 

  ! ******************************************************************
  ! *************Rectangular Grid ************************************
  ! ******************************************************************

! This case should only be called for divScalarGrad -- not laplace
#If (#operatorName == "laplace6Cons") || (#operatorName == "laplace8Cons")
! "
write(*,*) 'ERROR: laplace[6/8]Cons called for rectangular grids'
stop 20
#End

dxsqi(0)=1./dx(0)**2
dxsqi(1)=1./dx(1)**2
dxsqi(2)=1./dx(2)**2

#If (#operatorName == "divScalarGrad6Cons") || (#operatorName == "divScalarGrad8Cons")
! "
!  ************************************************
!  ************divScalarGrad RECTANGULAR **********
!  ************************************************

if( nd.eq.2 )then

c write(*,*) '****INSIDE dsgc6 2d rectangular'

do c=ca,cb
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b

 defineMidValueCoefficients(2,LAPLACE,ORDER)

#If #ORDER == "6"
 deriv(i1,i2,i3,c)=\
   (\
(-270*a11ph4*dp3r(i1-1,i2,i3)+270*a11mh4*dp3r(i1-2,i2,i3)+10*a11p3h4*dp3r(i1,i2,i3)-10*a11m3h4*dp3r(i1-3,i2,i3)+27*a11ph2*dp5r(i1-2,i2,i3)-27*a11mh2*dp5r(i1-3,i2,i3)+(720*a11ph4+5760*a11ph6+270*a11ph2)*dp1r(i1,i2,i3)+(-5760*a11mh6-720*a11mh4-270*a11mh2)*dp1r(i1-1,i2,i3)+(-240*a11p3h4-135*a11p3h2)*dp1r(i1+1,i2,i3)+(240*a11m3h4+135*a11m3h2)*dp1r(i1-2,i2,i3)-27*a11m5h2*dp1r(i1-3,i2,i3)+27*a11p5h2*dp1r(i1+2,i2,i3))/(5760.0*dx(0)**2)\
+(-270*a22ph4*dp3s(i1,i2-1,i3)+270*a22mh4*dp3s(i1,i2-2,i3)+10*a22p3h4*dp3s(i1,i2,i3)-10*a22m3h4*dp3s(i1,i2-3,i3)+27*a22ph2*dp5s(i1,i2-2,i3)-27*a22mh2*dp5s(i1,i2-3,i3)+(720*a22ph4+5760*a22ph6+270*a22ph2)*dp1s(i1,i2,i3)+(-5760*a22mh6-720*a22mh4-270*a22mh2)*dp1s(i1,i2-1,i3)+(-240*a22p3h4-135*a22p3h2)*dp1s(i1,i2+1,i3)+(240*a22m3h4+135*a22m3h2)*dp1s(i1,i2-2,i3)-27*a22m5h2*dp1s(i1,i2-3,i3)+27*a22p5h2*dp1s(i1,i2+2,i3))/(5760.0*dx(1)**2)\
   )

#Else
 deriv(i1,i2,i3,c)=\
   (\
((-630*a11ph2-15120*a11ph6)*dp3r(i1-1,i2,i3)+(15120*a11mh6+630*a11mh2)*dp3r(i1-2,i2,i3)+(315*a11p3h2+560*a11p3h6)*dp3r(i1,i2,i3)+(-560*a11m3h6-315*a11m3h2)*dp3r(i1-3,i2,i3)+63*a11m5h2*dp3r(i1-4,i2,i3)-63*a11p5h2*dp3r(i1+1,i2,i3)+(-7875*a11mh2-15120*a11mh4-40320*a11mh6-322560*a11mh8)*dp1r(i1-1,i2,i3)+(15120*a11ph4+40320*a11ph6+7875*a11ph2+322560*a11ph8)*dp1r(i1,i2,i3)+(-4725*a11p3h2-7560*a11p3h4-13440*a11p3h6)*dp1r(i1+1,i2,i3)+(4725*a11m3h2+13440*a11m3h6+7560*a11m3h4)*dp1r(i1-2,i2,i3)+(1575*a11p5h2+1512*a11p5h4)*dp1r(i1+2,i2,i3)+(-1512*a11m5h4-1575*a11m5h2)*dp1r(i1-3,i2,i3)-225*a11p7h2*dp1r(i1+3,i2,i3)+225*a11m7h2*dp1r(i1-4,i2,i3)+(1512*a11ph4+189*a11ph2)*dp5r(i1-2,i2,i3)+(-189*a11mh2-1512*a11mh4)*dp5r(i1-3,i2,i3)-63*a11p3h2*dp5r(i1-1,i2,i3)+63*a11m3h2*dp5r(i1-4,i2,i3)+225*a11mh2*dp7r(i1-4,i2,i3)-225*a11ph2*dp7r(i1-3,i2,i3))/(322560.0*dx(0)**2)\
  + ((-15120*a22ph6-630*a22ph2)*dp3s(i1,i2-1,i3)+(15120*a22mh6+630*a22mh2)*dp3s(i1,i2-2,i3)+(560*a22p3h6+315*a22p3h2)*dp3s(i1,i2,i3)+(-560*a22m3h6-315*a22m3h2)*dp3s(i1,i2-3,i3)+63*a22m5h2*dp3s(i1,i2-4,i3)-63*a22p5h2*dp3s(i1,i2+1,i3)+(-40320*a22mh6-15120*a22mh4-7875*a22mh2-322560*a22mh8)*dp1s(i1,i2-1,i3)+(40320*a22ph6+322560*a22ph8+7875*a22ph2+15120*a22ph4)*dp1s(i1,i2,i3)+(-7560*a22p3h4-4725*a22p3h2-13440*a22p3h6)*dp1s(i1,i2+1,i3)+(13440*a22m3h6+4725*a22m3h2+7560*a22m3h4)*dp1s(i1,i2-2,i3)+(1575*a22p5h2+1512*a22p5h4)*dp1s(i1,i2+2,i3)+(-1575*a22m5h2-1512*a22m5h4)*dp1s(i1,i2-3,i3)-225*a22p7h2*dp1s(i1,i2+3,i3)+225*a22m7h2*dp1s(i1,i2-4,i3)+(1512*a22ph4+189*a22ph2)*dp5s(i1,i2-2,i3)+(-189*a22mh2-1512*a22mh4)*dp5s(i1,i2-3,i3)-63*a22p3h2*dp5s(i1,i2-1,i3)+63*a22m3h2*dp5s(i1,i2-4,i3)+225*a22mh2*dp7s(i1,i2-4,i3)-225*a22ph2*dp7s(i1,i2-3,i3))/(322560.0*dx(1)**2)\
   )
#End

end do
end do
end do
end do

else if( nd.eq.3 )then

! write(*,*) '****INSIDE dsgc6 3d rectangular'

do c=ca,cb
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b

 defineMidValueCoefficients(3,LAPLACE,ORDER)

#If #ORDER == "6"
 deriv(i1,i2,i3,c)=\
   (\
(-270*a11ph4*dp3r(i1-1,i2,i3)+270*a11mh4*dp3r(i1-2,i2,i3)+10*a11p3h4*dp3r(i1,i2,i3)-10*a11m3h4*dp3r(i1-3,i2,i3)+(-270*a11mh2-720*a11mh4-5760*a11mh6)*dp1r(i1-1,i2,i3)+(720*a11ph4+5760*a11ph6+270*a11ph2)*dp1r(i1,i2,i3)+(-240*a11p3h4-135*a11p3h2)*dp1r(i1+1,i2,i3)+(135*a11m3h2+240*a11m3h4)*dp1r(i1-2,i2,i3)-27*a11mh2*dp5r(i1-3,i2,i3)+27*a11ph2*dp5r(i1-2,i2,i3)-27*a11m5h2*dp1r(i1-3,i2,i3)+27*a11p5h2*dp1r(i1+2,i2,i3))/(5760.0*dx(0)**2)\
+(-270*a22ph4*dp3s(i1,i2-1,i3)+270*a22mh4*dp3s(i1,i2-2,i3)+10*a22p3h4*dp3s(i1,i2,i3)-10*a22m3h4*dp3s(i1,i2-3,i3)+(-270*a22mh2-720*a22mh4-5760*a22mh6)*dp1s(i1,i2-1,i3)+(720*a22ph4+5760*a22ph6+270*a22ph2)*dp1s(i1,i2,i3)+(-240*a22p3h4-135*a22p3h2)*dp1s(i1,i2+1,i3)+(135*a22m3h2+240*a22m3h4)*dp1s(i1,i2-2,i3)-27*a22mh2*dp5s(i1,i2-3,i3)+27*a22ph2*dp5s(i1,i2-2,i3)-27*a22m5h2*dp1s(i1,i2-3,i3)+27*a22p5h2*dp1s(i1,i2+2,i3))/(5760.0*dx(1)**2)\
+(-270*a33ph4*dp3t(i1,i2,i3-1)+270*a33mh4*dp3t(i1,i2,i3-2)+10*a33p3h4*dp3t(i1,i2,i3)-10*a33m3h4*dp3t(i1,i2,i3-3)+(-270*a33mh2-720*a33mh4-5760*a33mh6)*dp1t(i1,i2,i3-1)+(720*a33ph4+5760*a33ph6+270*a33ph2)*dp1t(i1,i2,i3)+(-240*a33p3h4-135*a33p3h2)*dp1t(i1,i2,i3+1)+(135*a33m3h2+240*a33m3h4)*dp1t(i1,i2,i3-2)-27*a33mh2*dp5t(i1,i2,i3-3)+27*a33ph2*dp5t(i1,i2,i3-2)-27*a33m5h2*dp1t(i1,i2,i3-3)+27*a33p5h2*dp1t(i1,i2,i3+2))/(5760.0*dx(2)**2)\
   )
#Else
 deriv(i1,i2,i3,c)=\
   (\
((-630*a11ph2-15120*a11ph6)*dp3r(i1-1,i2,i3)+(15120*a11mh6+630*a11mh2)*dp3r(i1-2,i2,i3)+(315*a11p3h2+560*a11p3h6)*dp3r(i1,i2,i3)+(-560*a11m3h6-315*a11m3h2)*dp3r(i1-3,i2,i3)+63*a11m5h2*dp3r(i1-4,i2,i3)-63*a11p5h2*dp3r(i1+1,i2,i3)+(-7875*a11mh2-15120*a11mh4-40320*a11mh6-322560*a11mh8)*dp1r(i1-1,i2,i3)+(15120*a11ph4+40320*a11ph6+7875*a11ph2+322560*a11ph8)*dp1r(i1,i2,i3)+(-4725*a11p3h2-7560*a11p3h4-13440*a11p3h6)*dp1r(i1+1,i2,i3)+(4725*a11m3h2+13440*a11m3h6+7560*a11m3h4)*dp1r(i1-2,i2,i3)+(1575*a11p5h2+1512*a11p5h4)*dp1r(i1+2,i2,i3)+(-1512*a11m5h4-1575*a11m5h2)*dp1r(i1-3,i2,i3)-225*a11p7h2*dp1r(i1+3,i2,i3)+225*a11m7h2*dp1r(i1-4,i2,i3)+(1512*a11ph4+189*a11ph2)*dp5r(i1-2,i2,i3)+(-189*a11mh2-1512*a11mh4)*dp5r(i1-3,i2,i3)-63*a11p3h2*dp5r(i1-1,i2,i3)+63*a11m3h2*dp5r(i1-4,i2,i3)+225*a11mh2*dp7r(i1-4,i2,i3)-225*a11ph2*dp7r(i1-3,i2,i3))/(322560.0*dx(0)**2)\
  + ((-15120*a22ph6-630*a22ph2)*dp3s(i1,i2-1,i3)+(15120*a22mh6+630*a22mh2)*dp3s(i1,i2-2,i3)+(560*a22p3h6+315*a22p3h2)*dp3s(i1,i2,i3)+(-560*a22m3h6-315*a22m3h2)*dp3s(i1,i2-3,i3)+63*a22m5h2*dp3s(i1,i2-4,i3)-63*a22p5h2*dp3s(i1,i2+1,i3)+(-40320*a22mh6-15120*a22mh4-7875*a22mh2-322560*a22mh8)*dp1s(i1,i2-1,i3)+(40320*a22ph6+322560*a22ph8+7875*a22ph2+15120*a22ph4)*dp1s(i1,i2,i3)+(-7560*a22p3h4-4725*a22p3h2-13440*a22p3h6)*dp1s(i1,i2+1,i3)+(13440*a22m3h6+4725*a22m3h2+7560*a22m3h4)*dp1s(i1,i2-2,i3)+(1575*a22p5h2+1512*a22p5h4)*dp1s(i1,i2+2,i3)+(-1575*a22m5h2-1512*a22m5h4)*dp1s(i1,i2-3,i3)-225*a22p7h2*dp1s(i1,i2+3,i3)+225*a22m7h2*dp1s(i1,i2-4,i3)+(1512*a22ph4+189*a22ph2)*dp5s(i1,i2-2,i3)+(-189*a22mh2-1512*a22mh4)*dp5s(i1,i2-3,i3)-63*a22p3h2*dp5s(i1,i2-1,i3)+63*a22m3h2*dp5s(i1,i2-4,i3)+225*a22mh2*dp7s(i1,i2-4,i3)-225*a22ph2*dp7s(i1,i2-3,i3))/(322560.0*dx(1)**2)\
  + ((-15120*a33ph6-630*a33ph2)*dp3t(i1,i2,i3-1)+(15120*a33mh6+630*a33mh2)*dp3t(i1,i2,i3-2)+(560*a33p3h6+315*a33p3h2)*dp3t(i1,i2,i3)+(-560*a33m3h6-315*a33m3h2)*dp3t(i1,i2,i3-3)+63*a33m5h2*dp3t(i1,i2,i3-4)-63*a33p5h2*dp3t(i1,i2,i3+1)+(-40320*a33mh6-15120*a33mh4-7875*a33mh2-322560*a33mh8)*dp1t(i1,i2,i3-1)+(40320*a33ph6+322560*a33ph8+7875*a33ph2+15120*a33ph4)*dp1t(i1,i2,i3)+(-7560*a33p3h4-4725*a33p3h2-13440*a33p3h6)*dp1t(i1,i2,i3+1)+(13440*a33m3h6+4725*a33m3h2+7560*a33m3h4)*dp1t(i1,i2,i3-2)+(1575*a33p5h2+1512*a33p5h4)*dp1t(i1,i2,i3+2)+(-1575*a33m5h2-1512*a33m5h4)*dp1t(i1,i2,i3-3)-225*a33p7h2*dp1t(i1,i2,i3+3)+225*a33m7h2*dp1t(i1,i2,i3-4)+(1512*a33ph4+189*a33ph2)*dp5t(i1,i2,i3-2)+(-189*a33mh2-1512*a33mh4)*dp5t(i1,i2,i3-3)-63*a33p3h2*dp5t(i1,i2,i3-1)+63*a33m3h2*dp5t(i1,i2,i3-4)+225*a33mh2*dp7t(i1,i2,i3-4)-225*a33ph2*dp7t(i1,i2,i3-3))/(322560.0*dx(2)**2)\
   )
#End


end do
end do
end do
end do

else
  write(*,*) 'dsgc6: ERROR invalid nd=',nd
  stop 11
end if

#Elif (#operatorName == "divTensorGrad6Cons") || (#operatorName == "divTensorGrad8Cons")
! "
!  ************************************************
!  ************divTensorGrad RECTANGULAR **********
!  ************************************************

if( nd.eq.2 )then

#If #ORDER == "6"
c write(*,*) '****INSIDE dsgc6 2d'
#Else
c write(*,*) '****INSIDE dsgc8 2d'
#End

do c=ca,cb
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b

! ---  *** new *** ---

 defineMidValueCoefficients(2,DSGR,ORDER)


#If #ORDER == "6"
 deriv(i1,i2,i3,c)=\
   (\
(-270*a11ph4*dp3r(i1-1,i2,i3)+270*a11mh4*dp3r(i1-2,i2,i3)+10*a11p3h4*dp3r(i1,i2,i3)-10*a11m3h4*dp3r(i1-3,i2,i3)+27*a11ph2*dp5r(i1-2,i2,i3)-27*a11mh2*dp5r(i1-3,i2,i3)+(720*a11ph4+5760*a11ph6+270*a11ph2)*dp1r(i1,i2,i3)+(-5760*a11mh6-720*a11mh4-270*a11mh2)*dp1r(i1-1,i2,i3)+(-240*a11p3h4-135*a11p3h2)*dp1r(i1+1,i2,i3)+(240*a11m3h4+135*a11m3h2)*dp1r(i1-2,i2,i3)-27*a11m5h2*dp1r(i1-3,i2,i3)+27*a11p5h2*dp1r(i1+2,i2,i3))/(5760.0*dx(0)**2)\
+(-270*a22ph4*dp3s(i1,i2-1,i3)+270*a22mh4*dp3s(i1,i2-2,i3)+10*a22p3h4*dp3s(i1,i2,i3)-10*a22m3h4*dp3s(i1,i2-3,i3)+27*a22ph2*dp5s(i1,i2-2,i3)-27*a22mh2*dp5s(i1,i2-3,i3)+(720*a22ph4+5760*a22ph6+270*a22ph2)*dp1s(i1,i2,i3)+(-5760*a22mh6-720*a22mh4-270*a22mh2)*dp1s(i1,i2-1,i3)+(-240*a22p3h4-135*a22p3h2)*dp1s(i1,i2+1,i3)+(240*a22m3h4+135*a22m3h2)*dp1s(i1,i2-2,i3)-27*a22m5h2*dp1s(i1,i2-3,i3)+27*a22p5h2*dp1s(i1,i2+2,i3))/(5760.0*dx(1)**2)\
 +((54*dzs(i1-2,i2,i3)-5*dzpms(i1-2,i2,i3))*a12r(i1-2,i2,i3)+(-54*dzr(i1,i2+2,i3)+5*dzpmr(i1,i2+2,i3))*a12r(i1,i2+2,i3)+(54*dzr(i1,i2-2,i3)-5*dzpmr(i1,i2-2,i3))*a12r(i1,i2-2,i3)+(-54*dzs(i1+2,i2,i3)+5*dzpms(i1+2,i2,i3))*a12r(i1+2,i2,i3)-6*a12r(i1,i2-3,i3)*dzr(i1,i2-3,i3)+6*a12r(i1,i2+3,i3)*dzr(i1,i2+3,i3)+6*a12r(i1+3,i2,i3)*dzs(i1+3,i2,i3)-6*a12r(i1-3,i2,i3)*dzs(i1-3,i2,i3)+(270*dzs(i1+1,i2,i3)+6*dzpms(i1+1,i2+1,i3)-52*dzpms(i1+1,i2,i3)+6*dzpms(i1+1,i2-1,i3))*a12r(i1+1,i2,i3)+(-6*dzpms(i1-1,i2+1,i3)-270*dzs(i1-1,i2,i3)-6*dzpms(i1-1,i2-1,i3)+52*dzpms(i1-1,i2,i3))*a12r(i1-1,i2,i3)+(-52*dzpmr(i1,i2+1,i3)+6*dzpmr(i1+1,i2+1,i3)+270*dzr(i1,i2+1,i3)+6*dzpmr(i1-1,i2+1,i3))*a12r(i1,i2+1,i3)+(-6*dzpmr(i1+1,i2-1,i3)-6*dzpmr(i1-1,i2-1,i3)+52*dzpmr(i1,i2-1,i3)-270*dzr(i1,i2-1,i3))*a12r(i1,i2-1,i3))/(720.0*dx(0)*dx(1))\
   )
#Else
 deriv(i1,i2,i3,c)=\
   (\
((-630*a11ph2-15120*a11ph6)*dp3r(i1-1,i2,i3)+(15120*a11mh6+630*a11mh2)*dp3r(i1-2,i2,i3)+(315*a11p3h2+560*a11p3h6)*dp3r(i1,i2,i3)+(-560*a11m3h6-315*a11m3h2)*dp3r(i1-3,i2,i3)+63*a11m5h2*dp3r(i1-4,i2,i3)-63*a11p5h2*dp3r(i1+1,i2,i3)+(-7875*a11mh2-15120*a11mh4-40320*a11mh6-322560*a11mh8)*dp1r(i1-1,i2,i3)+(15120*a11ph4+40320*a11ph6+7875*a11ph2+322560*a11ph8)*dp1r(i1,i2,i3)+(-4725*a11p3h2-7560*a11p3h4-13440*a11p3h6)*dp1r(i1+1,i2,i3)+(4725*a11m3h2+13440*a11m3h6+7560*a11m3h4)*dp1r(i1-2,i2,i3)+(1575*a11p5h2+1512*a11p5h4)*dp1r(i1+2,i2,i3)+(-1512*a11m5h4-1575*a11m5h2)*dp1r(i1-3,i2,i3)-225*a11p7h2*dp1r(i1+3,i2,i3)+225*a11m7h2*dp1r(i1-4,i2,i3)+(1512*a11ph4+189*a11ph2)*dp5r(i1-2,i2,i3)+(-189*a11mh2-1512*a11mh4)*dp5r(i1-3,i2,i3)-63*a11p3h2*dp5r(i1-1,i2,i3)+63*a11m3h2*dp5r(i1-4,i2,i3)+225*a11mh2*dp7r(i1-4,i2,i3)-225*a11ph2*dp7r(i1-3,i2,i3))/(322560.0*dx(0)**2)\
  + ((-15120*a22ph6-630*a22ph2)*dp3s(i1,i2-1,i3)+(15120*a22mh6+630*a22mh2)*dp3s(i1,i2-2,i3)+(560*a22p3h6+315*a22p3h2)*dp3s(i1,i2,i3)+(-560*a22m3h6-315*a22m3h2)*dp3s(i1,i2-3,i3)+63*a22m5h2*dp3s(i1,i2-4,i3)-63*a22p5h2*dp3s(i1,i2+1,i3)+(-40320*a22mh6-15120*a22mh4-7875*a22mh2-322560*a22mh8)*dp1s(i1,i2-1,i3)+(40320*a22ph6+322560*a22ph8+7875*a22ph2+15120*a22ph4)*dp1s(i1,i2,i3)+(-7560*a22p3h4-4725*a22p3h2-13440*a22p3h6)*dp1s(i1,i2+1,i3)+(13440*a22m3h6+4725*a22m3h2+7560*a22m3h4)*dp1s(i1,i2-2,i3)+(1575*a22p5h2+1512*a22p5h4)*dp1s(i1,i2+2,i3)+(-1575*a22m5h2-1512*a22m5h4)*dp1s(i1,i2-3,i3)-225*a22p7h2*dp1s(i1,i2+3,i3)+225*a22m7h2*dp1s(i1,i2-4,i3)+(1512*a22ph4+189*a22ph2)*dp5s(i1,i2-2,i3)+(-189*a22mh2-1512*a22mh4)*dp5s(i1,i2-3,i3)-63*a22p3h2*dp5s(i1,i2-1,i3)+63*a22m3h2*dp5s(i1,i2-4,i3)+225*a22mh2*dp7s(i1,i2-4,i3)-225*a22ph2*dp7s(i1,i2-3,i3))/(322560.0*dx(1)**2)\
  + ((7*dzpms(i1-2,i2+1,i3)-77*dzpms(i1-2,i2,i3)+7*dzpms(i1-2,i2-1,i3)+504*dzs(i1-2,i2,i3))*a12r(i1-2,i2,i3)+(-7*dzpmr(i1+1,i2+2,i3)+77*dzpmr(i1,i2+2,i3)-7*dzpmr(i1-1,i2+2,i3)-504*dzr(i1,i2+2,i3))*a12r(i1,i2+2,i3)+(7*dzpmr(i1-1,i2-2,i3)+7*dzpmr(i1+1,i2-2,i3)-77*dzpmr(i1,i2-2,i3)+504*dzr(i1,i2-2,i3))*a12r(i1,i2-2,i3)+(77*dzpms(i1+2,i2,i3)-7*dzpms(i1+2,i2-1,i3)-7*dzpms(i1+2,i2+1,i3)-504*dzs(i1+2,i2,i3))*a12r(i1+2,i2,i3)+9*a12r(i1,i2-4,i3)*dzr(i1,i2-4,i3)-9*a12r(i1,i2+4,i3)*dzr(i1,i2+4,i3)+(-96*dzr(i1,i2-3,i3)+7*dzpmr(i1,i2-3,i3))*a12r(i1,i2-3,i3)+(-7*dzpmr(i1,i2+3,i3)+96*dzr(i1,i2+3,i3))*a12r(i1,i2+3,i3)+(-7*dzpms(i1+3,i2,i3)+96*dzs(i1+3,i2,i3))*a12r(i1+3,i2,i3)+(-96*dzs(i1-3,i2,i3)+7*dzpms(i1-3,i2,i3))*a12r(i1-3,i2,i3)+9*a12r(i1-4,i2,i3)*dzs(i1-4,i2,i3)-9*a12r(i1+4,i2,i3)*dzs(i1+4,i2,i3)+(92*dzpms(i1+1,i2-1,i3)+92*dzpms(i1+1,i2+1,i3)+2016*dzs(i1+1,i2,i3)-481*dzpms(i1+1,i2,i3)-9*dzpms(i1+1,i2+2,i3)-9*dzpms(i1+1,i2-2,i3))*a12r(i1+1,i2,i3)+(9*dzpms(i1-1,i2+2,i3)+9*dzpms(i1-1,i2-2,i3)+481*dzpms(i1-1,i2,i3)-92*dzpms(i1-1,i2+1,i3)-92*dzpms(i1-1,i2-1,i3)-2016*dzs(i1-1,i2,i3))*a12r(i1-1,i2,i3)+(-9*dzpmr(i1-2,i2+1,i3)-481*dzpmr(i1,i2+1,i3)-9*dzpmr(i1+2,i2+1,i3)+92*dzpmr(i1+1,i2+1,i3)+92*dzpmr(i1-1,i2+1,i3)+2016*dzr(i1,i2+1,i3))*a12r(i1,i2+1,i3)+(-92*dzpmr(i1-1,i2-1,i3)-92*dzpmr(i1+1,i2-1,i3)+9*dzpmr(i1+2,i2-1,i3)-2016*dzr(i1,i2-1,i3)+481*dzpmr(i1,i2-1,i3)+9*dzpmr(i1-2,i2-1,i3))*a12r(i1,i2-1,i3))/(5040.0*dx(0)*dx(1))\
   )
#End

end do
end do
end do
end do

else if( nd.eq.3 )then

#If #ORDER == "6"
c write(*,*) '****INSIDE dsgc6 3d'
#Else
c write(*,*) '****INSIDE dsgc8 3d'
#End

do c=ca,cb
do i3=n3a,n3b
do i2=n2a,n2b
do i1=n1a,n1b

 defineMidValueCoefficients(3,DSGR,ORDER)
 
#If #ORDER == "6"

 deriv(i1,i2,i3,c)=\
   (\
(27*a11ph2*dp5r(i1-2,i2,i3)-27*a11mh2*dp5r(i1-3,i2,i3)+10*a11p3h4*dp3r(i1,i2,i3)-10*a11m3h4*dp3r(i1-3,i2,i3)-270*a11ph4*dp3r(i1-1,i2,i3)+270*a11mh4*dp3r(i1-2,i2,i3)+27*a11p5h2*dp1r(i1+2,i2,i3)+(5760*a11ph6+720*a11ph4+270*a11ph2)*dp1r(i1,i2,i3)+(-270*a11mh2-5760*a11mh6-720*a11mh4)*dp1r(i1-1,i2,i3)-27*a11m5h2*dp1r(i1-3,i2,i3)+(135*a11m3h2+240*a11m3h4)*dp1r(i1-2,i2,i3)+(-240*a11p3h4-135*a11p3h2)*dp1r(i1+1,i2,i3))/(5760.0*dx(0)**2)\
  + (27*a22ph2*dp5s(i1,i2-2,i3)-27*a22mh2*dp5s(i1,i2-3,i3)+10*a22p3h4*dp3s(i1,i2,i3)-10*a22m3h4*dp3s(i1,i2-3,i3)-270*a22ph4*dp3s(i1,i2-1,i3)+270*a22mh4*dp3s(i1,i2-2,i3)+27*a22p5h2*dp1s(i1,i2+2,i3)+(5760*a22ph6+720*a22ph4+270*a22ph2)*dp1s(i1,i2,i3)+(-270*a22mh2-5760*a22mh6-720*a22mh4)*dp1s(i1,i2-1,i3)-27*a22m5h2*dp1s(i1,i2-3,i3)+(135*a22m3h2+240*a22m3h4)*dp1s(i1,i2-2,i3)+(-240*a22p3h4-135*a22p3h2)*dp1s(i1,i2+1,i3))/(5760.0*dx(1)**2)\
  + (27*a33ph2*dp5t(i1,i2,i3-2)-27*a33mh2*dp5t(i1,i2,i3-3)+10*a33p3h4*dp3t(i1,i2,i3)-10*a33m3h4*dp3t(i1,i2,i3-3)-270*a33ph4*dp3t(i1,i2,i3-1)+270*a33mh4*dp3t(i1,i2,i3-2)+27*a33p5h2*dp1t(i1,i2,i3+2)+(5760*a33ph6+720*a33ph4+270*a33ph2)*dp1t(i1,i2,i3)+(-270*a33mh2-5760*a33mh6-720*a33mh4)*dp1t(i1,i2,i3-1)-27*a33m5h2*dp1t(i1,i2,i3-3)+(135*a33m3h2+240*a33m3h4)*dp1t(i1,i2,i3-2)+(-240*a33p3h4-135*a33p3h2)*dp1t(i1,i2,i3+1))/(5760.0*dx(2)**2)\
  + ((5*dzpmr(i1,i2+2,i3)-54*dzr(i1,i2+2,i3))*a123dr(i1,i2+2,i3)+(-5*dzpmr(i1,i2-2,i3)+54*dzr(i1,i2-2,i3))*a123dr(i1,i2-2,i3)+(270*dzs(i1+1,i2,i3)+6*dzpms(i1+1,i2-1,i3)-52*dzpms(i1+1,i2,i3)+6*dzpms(i1+1,i2+1,i3))*a123dr(i1+1,i2,i3)+(-6*dzpms(i1-1,i2+1,i3)-270*dzs(i1-1,i2,i3)-6*dzpms(i1-1,i2-1,i3)+52*dzpms(i1-1,i2,i3))*a123dr(i1-1,i2,i3)+(270*dzr(i1,i2+1,i3)-52*dzpmr(i1,i2+1,i3)+6*dzpmr(i1+1,i2+1,i3)+6*dzpmr(i1-1,i2+1,i3))*a123dr(i1,i2+1,i3)+(52*dzpmr(i1,i2-1,i3)-6*dzpmr(i1-1,i2-1,i3)-6*dzpmr(i1+1,i2-1,i3)-270*dzr(i1,i2-1,i3))*a123dr(i1,i2-1,i3)-6*a123dr(i1,i2-3,i3)*dzr(i1,i2-3,i3)+6*a123dr(i1,i2+3,i3)*dzr(i1,i2+3,i3)+(5*dzpms(i1+2,i2,i3)-54*dzs(i1+2,i2,i3))*a123dr(i1+2,i2,i3)+(-5*dzpms(i1-2,i2,i3)+54*dzs(i1-2,i2,i3))*a123dr(i1-2,i2,i3)-6*a123dr(i1-3,i2,i3)*dzs(i1-3,i2,i3)+6*a123dr(i1+3,i2,i3)*dzs(i1+3,i2,i3))/(720.0*dx(0)*dx(1))\
  + ((270*dzt(i1+1,i2,i3)-52*dzpmt(i1+1,i2,i3)+6*dzpmt(i1+1,i2,i3+1)+6*dzpmt(i1+1,i2,i3-1))*a133dr(i1+1,i2,i3)+(-6*dzpmt(i1-1,i2,i3+1)+52*dzpmt(i1-1,i2,i3)-270*dzt(i1-1,i2,i3)-6*dzpmt(i1-1,i2,i3-1))*a133dr(i1-1,i2,i3)+(6*dzpmr(i1+1,i2,i3+1)-52*dzpmr(i1,i2,i3+1)+6*dzpmr(i1-1,i2,i3+1)+270*dzr(i1,i2,i3+1))*a133dr(i1,i2,i3+1)+(-270*dzr(i1,i2,i3-1)-6*dzpmr(i1+1,i2,i3-1)+52*dzpmr(i1,i2,i3-1)-6*dzpmr(i1-1,i2,i3-1))*a133dr(i1,i2,i3-1)+(5*dzpmt(i1+2,i2,i3)-54*dzt(i1+2,i2,i3))*a133dr(i1+2,i2,i3)+(-5*dzpmt(i1-2,i2,i3)+54*dzt(i1-2,i2,i3))*a133dr(i1-2,i2,i3)+6*a133dr(i1+3,i2,i3)*dzt(i1+3,i2,i3)-6*a133dr(i1-3,i2,i3)*dzt(i1-3,i2,i3)+(5*dzpmr(i1,i2,i3+2)-54*dzr(i1,i2,i3+2))*a133dr(i1,i2,i3+2)+(54*dzr(i1,i2,i3-2)-5*dzpmr(i1,i2,i3-2))*a133dr(i1,i2,i3-2)+6*a133dr(i1,i2,i3+3)*dzr(i1,i2,i3+3)-6*a133dr(i1,i2,i3-3)*dzr(i1,i2,i3-3))/(720.0*dx(0)*dx(2))\
  + ((5*dzpmt(i1,i2+2,i3)-54*dzt(i1,i2+2,i3))*a233dr(i1,i2+2,i3)+(54*dzt(i1,i2-2,i3)-5*dzpmt(i1,i2-2,i3))*a233dr(i1,i2-2,i3)+6*a233dr(i1,i2+3,i3)*dzt(i1,i2+3,i3)-6*a233dr(i1,i2-3,i3)*dzt(i1,i2-3,i3)+(270*dzt(i1,i2+1,i3)-52*dzpmt(i1,i2+1,i3)+6*dzpmt(i1,i2+1,i3-1)+6*dzpmt(i1,i2+1,i3+1))*a233dr(i1,i2+1,i3)+(-6*dzpmt(i1,i2-1,i3+1)+52*dzpmt(i1,i2-1,i3)-6*dzpmt(i1,i2-1,i3-1)-270*dzt(i1,i2-1,i3))*a233dr(i1,i2-1,i3)+6*a233dr(i1,i2,i3+3)*dzs(i1,i2,i3+3)-6*a233dr(i1,i2,i3-3)*dzs(i1,i2,i3-3)+(-52*dzpms(i1,i2,i3+1)+270*dzs(i1,i2,i3+1)+6*dzpms(i1,i2-1,i3+1)+6*dzpms(i1,i2+1,i3+1))*a233dr(i1,i2,i3+1)+(52*dzpms(i1,i2,i3-1)-270*dzs(i1,i2,i3-1)-6*dzpms(i1,i2-1,i3-1)-6*dzpms(i1,i2+1,i3-1))*a233dr(i1,i2,i3-1)+(5*dzpms(i1,i2,i3+2)-54*dzs(i1,i2,i3+2))*a233dr(i1,i2,i3+2)+(54*dzs(i1,i2,i3-2)-5*dzpms(i1,i2,i3-2))*a233dr(i1,i2,i3-2))/(720.0*dx(1)*dx(2))\
   )

#Else
 deriv(i1,i2,i3,c)=\
   (\
((-630*a11ph2-15120*a11ph6)*dp3r(i1-1,i2,i3)+(15120*a11mh6+630*a11mh2)*dp3r(i1-2,i2,i3)+(315*a11p3h2+560*a11p3h6)*dp3r(i1,i2,i3)+(-560*a11m3h6-315*a11m3h2)*dp3r(i1-3,i2,i3)+63*a11m5h2*dp3r(i1-4,i2,i3)-63*a11p5h2*dp3r(i1+1,i2,i3)+(-7875*a11mh2-15120*a11mh4-40320*a11mh6-322560*a11mh8)*dp1r(i1-1,i2,i3)+(15120*a11ph4+40320*a11ph6+7875*a11ph2+322560*a11ph8)*dp1r(i1,i2,i3)+(-4725*a11p3h2-7560*a11p3h4-13440*a11p3h6)*dp1r(i1+1,i2,i3)+(4725*a11m3h2+13440*a11m3h6+7560*a11m3h4)*dp1r(i1-2,i2,i3)+(1575*a11p5h2+1512*a11p5h4)*dp1r(i1+2,i2,i3)+(-1512*a11m5h4-1575*a11m5h2)*dp1r(i1-3,i2,i3)-225*a11p7h2*dp1r(i1+3,i2,i3)+225*a11m7h2*dp1r(i1-4,i2,i3)+(1512*a11ph4+189*a11ph2)*dp5r(i1-2,i2,i3)+(-189*a11mh2-1512*a11mh4)*dp5r(i1-3,i2,i3)-63*a11p3h2*dp5r(i1-1,i2,i3)+63*a11m3h2*dp5r(i1-4,i2,i3)+225*a11mh2*dp7r(i1-4,i2,i3)-225*a11ph2*dp7r(i1-3,i2,i3))/(322560.0*dx(0)**2)\
  + ((-15120*a22ph6-630*a22ph2)*dp3s(i1,i2-1,i3)+(15120*a22mh6+630*a22mh2)*dp3s(i1,i2-2,i3)+(560*a22p3h6+315*a22p3h2)*dp3s(i1,i2,i3)+(-560*a22m3h6-315*a22m3h2)*dp3s(i1,i2-3,i3)+63*a22m5h2*dp3s(i1,i2-4,i3)-63*a22p5h2*dp3s(i1,i2+1,i3)+(-40320*a22mh6-15120*a22mh4-7875*a22mh2-322560*a22mh8)*dp1s(i1,i2-1,i3)+(40320*a22ph6+322560*a22ph8+7875*a22ph2+15120*a22ph4)*dp1s(i1,i2,i3)+(-7560*a22p3h4-4725*a22p3h2-13440*a22p3h6)*dp1s(i1,i2+1,i3)+(13440*a22m3h6+4725*a22m3h2+7560*a22m3h4)*dp1s(i1,i2-2,i3)+(1575*a22p5h2+1512*a22p5h4)*dp1s(i1,i2+2,i3)+(-1575*a22m5h2-1512*a22m5h4)*dp1s(i1,i2-3,i3)-225*a22p7h2*dp1s(i1,i2+3,i3)+225*a22m7h2*dp1s(i1,i2-4,i3)+(1512*a22ph4+189*a22ph2)*dp5s(i1,i2-2,i3)+(-189*a22mh2-1512*a22mh4)*dp5s(i1,i2-3,i3)-63*a22p3h2*dp5s(i1,i2-1,i3)+63*a22m3h2*dp5s(i1,i2-4,i3)+225*a22mh2*dp7s(i1,i2-4,i3)-225*a22ph2*dp7s(i1,i2-3,i3))/(322560.0*dx(1)**2)\
  + ((-15120*a33ph6-630*a33ph2)*dp3t(i1,i2,i3-1)+(15120*a33mh6+630*a33mh2)*dp3t(i1,i2,i3-2)+(560*a33p3h6+315*a33p3h2)*dp3t(i1,i2,i3)+(-560*a33m3h6-315*a33m3h2)*dp3t(i1,i2,i3-3)+63*a33m5h2*dp3t(i1,i2,i3-4)-63*a33p5h2*dp3t(i1,i2,i3+1)+(-40320*a33mh6-15120*a33mh4-7875*a33mh2-322560*a33mh8)*dp1t(i1,i2,i3-1)+(40320*a33ph6+322560*a33ph8+7875*a33ph2+15120*a33ph4)*dp1t(i1,i2,i3)+(-7560*a33p3h4-4725*a33p3h2-13440*a33p3h6)*dp1t(i1,i2,i3+1)+(13440*a33m3h6+4725*a33m3h2+7560*a33m3h4)*dp1t(i1,i2,i3-2)+(1575*a33p5h2+1512*a33p5h4)*dp1t(i1,i2,i3+2)+(-1575*a33m5h2-1512*a33m5h4)*dp1t(i1,i2,i3-3)-225*a33p7h2*dp1t(i1,i2,i3+3)+225*a33m7h2*dp1t(i1,i2,i3-4)+(1512*a33ph4+189*a33ph2)*dp5t(i1,i2,i3-2)+(-189*a33mh2-1512*a33mh4)*dp5t(i1,i2,i3-3)-63*a33p3h2*dp5t(i1,i2,i3-1)+63*a33m3h2*dp5t(i1,i2,i3-4)+225*a33mh2*dp7t(i1,i2,i3-4)-225*a33ph2*dp7t(i1,i2,i3-3))/(322560.0*dx(2)**2)\
  + ((2016*dzs(i1+1,i2,i3)-481*dzpms(i1+1,i2,i3)+92*dzpms(i1+1,i2+1,i3)-9*dzpms(i1+1,i2+2,i3)-9*dzpms(i1+1,i2-2,i3)+92*dzpms(i1+1,i2-1,i3))*a123dr(i1+1,i2,i3)+(9*dzpms(i1-1,i2-2,i3)-92*dzpms(i1-1,i2+1,i3)+481*dzpms(i1-1,i2,i3)-92*dzpms(i1-1,i2-1,i3)-2016*dzs(i1-1,i2,i3)+9*dzpms(i1-1,i2+2,i3))*a123dr(i1-1,i2,i3)+(92*dzpmr(i1+1,i2+1,i3)-481*dzpmr(i1,i2+1,i3)-9*dzpmr(i1+2,i2+1,i3)-9*dzpmr(i1-2,i2+1,i3)+92*dzpmr(i1-1,i2+1,i3)+2016*dzr(i1,i2+1,i3))*a123dr(i1,i2+1,i3)+(9*dzpmr(i1+2,i2-1,i3)+9*dzpmr(i1-2,i2-1,i3)+481*dzpmr(i1,i2-1,i3)-92*dzpmr(i1-1,i2-1,i3)-92*dzpmr(i1+1,i2-1,i3)-2016*dzr(i1,i2-1,i3))*a123dr(i1,i2-1,i3)+9*a123dr(i1-4,i2,i3)*dzs(i1-4,i2,i3)-9*a123dr(i1+4,i2,i3)*dzs(i1+4,i2,i3)+(-7*dzpms(i1+2,i2-1,i3)-504*dzs(i1+2,i2,i3)+77*dzpms(i1+2,i2,i3)-7*dzpms(i1+2,i2+1,i3))*a123dr(i1+2,i2,i3)+(7*dzpms(i1-2,i2+1,i3)+7*dzpms(i1-2,i2-1,i3)-77*dzpms(i1-2,i2,i3)+504*dzs(i1-2,i2,i3))*a123dr(i1-2,i2,i3)+(-7*dzpms(i1+3,i2,i3)+96*dzs(i1+3,i2,i3))*a123dr(i1+3,i2,i3)+(-96*dzs(i1-3,i2,i3)+7*dzpms(i1-3,i2,i3))*a123dr(i1-3,i2,i3)+(-7*dzpmr(i1+1,i2+2,i3)-7*dzpmr(i1-1,i2+2,i3)+77*dzpmr(i1,i2+2,i3)-504*dzr(i1,i2+2,i3))*a123dr(i1,i2+2,i3)+(-77*dzpmr(i1,i2-2,i3)+504*dzr(i1,i2-2,i3)+7*dzpmr(i1+1,i2-2,i3)+7*dzpmr(i1-1,i2-2,i3))*a123dr(i1,i2-2,i3)+(-7*dzpmr(i1,i2+3,i3)+96*dzr(i1,i2+3,i3))*a123dr(i1,i2+3,i3)+(-96*dzr(i1,i2-3,i3)+7*dzpmr(i1,i2-3,i3))*a123dr(i1,i2-3,i3)-9*a123dr(i1,i2+4,i3)*dzr(i1,i2+4,i3)+9*a123dr(i1,i2-4,i3)*dzr(i1,i2-4,i3))/(5040.0*dx(0)*dx(1)))
 deriv(i1,i2,i3,c)=(deriv(i1,i2,i3,c)\
  + ((2016*dzt(i1+1,i2,i3)-481*dzpmt(i1+1,i2,i3)+92*dzpmt(i1+1,i2,i3+1)-9*dzpmt(i1+1,i2,i3+2)-9*dzpmt(i1+1,i2,i3-2)+92*dzpmt(i1+1,i2,i3-1))*a133dr(i1+1,i2,i3)+(9*dzpmt(i1-1,i2,i3-2)-92*dzpmt(i1-1,i2,i3+1)+481*dzpmt(i1-1,i2,i3)-92*dzpmt(i1-1,i2,i3-1)-2016*dzt(i1-1,i2,i3)+9*dzpmt(i1-1,i2,i3+2))*a133dr(i1-1,i2,i3)+(92*dzpmr(i1+1,i2,i3+1)-481*dzpmr(i1,i2,i3+1)-9*dzpmr(i1+2,i2,i3+1)-9*dzpmr(i1-2,i2,i3+1)+92*dzpmr(i1-1,i2,i3+1)+2016*dzr(i1,i2,i3+1))*a133dr(i1,i2,i3+1)+(9*dzpmr(i1+2,i2,i3-1)+9*dzpmr(i1-2,i2,i3-1)+481*dzpmr(i1,i2,i3-1)-92*dzpmr(i1-1,i2,i3-1)-92*dzpmr(i1+1,i2,i3-1)-2016*dzr(i1,i2,i3-1))*a133dr(i1,i2,i3-1)+9*a133dr(i1-4,i2,i3)*dzt(i1-4,i2,i3)-9*a133dr(i1+4,i2,i3)*dzt(i1+4,i2,i3)+(-7*dzpmt(i1+2,i2,i3-1)-504*dzt(i1+2,i2,i3)+77*dzpmt(i1+2,i2,i3)-7*dzpmt(i1+2,i2,i3+1))*a133dr(i1+2,i2,i3)+(7*dzpmt(i1-2,i2,i3+1)+7*dzpmt(i1-2,i2,i3-1)-77*dzpmt(i1-2,i2,i3)+504*dzt(i1-2,i2,i3))*a133dr(i1-2,i2,i3)+(-7*dzpmt(i1+3,i2,i3)+96*dzt(i1+3,i2,i3))*a133dr(i1+3,i2,i3)+(-96*dzt(i1-3,i2,i3)+7*dzpmt(i1-3,i2,i3))*a133dr(i1-3,i2,i3)+(-7*dzpmr(i1+1,i2,i3+2)-7*dzpmr(i1-1,i2,i3+2)+77*dzpmr(i1,i2,i3+2)-504*dzr(i1,i2,i3+2))*a133dr(i1,i2,i3+2)+(-77*dzpmr(i1,i2,i3-2)+504*dzr(i1,i2,i3-2)+7*dzpmr(i1+1,i2,i3-2)+7*dzpmr(i1-1,i2,i3-2))*a133dr(i1,i2,i3-2)+(-7*dzpmr(i1,i2,i3+3)+96*dzr(i1,i2,i3+3))*a133dr(i1,i2,i3+3)+(-96*dzr(i1,i2,i3-3)+7*dzpmr(i1,i2,i3-3))*a133dr(i1,i2,i3-3)-9*a133dr(i1,i2,i3+4)*dzr(i1,i2,i3+4)+9*a133dr(i1,i2,i3-4)*dzr(i1,i2,i3-4))/(5040.0*dx(0)*dx(2))\
  + ((2016*dzt(i1,i2+1,i3)-481*dzpmt(i1,i2+1,i3)+92*dzpmt(i1,i2+1,i3+1)-9*dzpmt(i1,i2+1,i3+2)-9*dzpmt(i1,i2+1,i3-2)+92*dzpmt(i1,i2+1,i3-1))*a233dr(i1,i2+1,i3)+(9*dzpmt(i1,i2-1,i3-2)-92*dzpmt(i1,i2-1,i3+1)+481*dzpmt(i1,i2-1,i3)-92*dzpmt(i1,i2-1,i3-1)-2016*dzt(i1,i2-1,i3)+9*dzpmt(i1,i2-1,i3+2))*a233dr(i1,i2-1,i3)+(92*dzpms(i1,i2+1,i3+1)-481*dzpms(i1,i2,i3+1)-9*dzpms(i1,i2+2,i3+1)-9*dzpms(i1,i2-2,i3+1)+92*dzpms(i1,i2-1,i3+1)+2016*dzs(i1,i2,i3+1))*a233dr(i1,i2,i3+1)+(9*dzpms(i1,i2+2,i3-1)+9*dzpms(i1,i2-2,i3-1)+481*dzpms(i1,i2,i3-1)-92*dzpms(i1,i2-1,i3-1)-92*dzpms(i1,i2+1,i3-1)-2016*dzs(i1,i2,i3-1))*a233dr(i1,i2,i3-1)+9*a233dr(i1,i2-4,i3)*dzt(i1,i2-4,i3)-9*a233dr(i1,i2+4,i3)*dzt(i1,i2+4,i3)+(-7*dzpmt(i1,i2+2,i3-1)-504*dzt(i1,i2+2,i3)+77*dzpmt(i1,i2+2,i3)-7*dzpmt(i1,i2+2,i3+1))*a233dr(i1,i2+2,i3)+(7*dzpmt(i1,i2-2,i3+1)+7*dzpmt(i1,i2-2,i3-1)-77*dzpmt(i1,i2-2,i3)+504*dzt(i1,i2-2,i3))*a233dr(i1,i2-2,i3)+(-7*dzpmt(i1,i2+3,i3)+96*dzt(i1,i2+3,i3))*a233dr(i1,i2+3,i3)+(-96*dzt(i1,i2-3,i3)+7*dzpmt(i1,i2-3,i3))*a233dr(i1,i2-3,i3)+(-7*dzpms(i1,i2+1,i3+2)-7*dzpms(i1,i2-1,i3+2)+77*dzpms(i1,i2,i3+2)-504*dzs(i1,i2,i3+2))*a233dr(i1,i2,i3+2)+(-77*dzpms(i1,i2,i3-2)+504*dzs(i1,i2,i3-2)+7*dzpms(i1,i2+1,i3-2)+7*dzpms(i1,i2-1,i3-2))*a233dr(i1,i2,i3-2)+(-7*dzpms(i1,i2,i3+3)+96*dzs(i1,i2,i3+3))*a233dr(i1,i2,i3+3)+(-96*dzs(i1,i2,i3-3)+7*dzpms(i1,i2,i3-3))*a233dr(i1,i2,i3-3)-9*a233dr(i1,i2,i3+4)*dzs(i1,i2,i3+4)+9*a233dr(i1,i2,i3-4)*dzs(i1,i2,i3-4))/(5040.0*dx(1)*dx(2))\
   )
#End

 ! write(*,*) 'i1,i2,i3,jac,deriv=',i1,i2,i3,jac,deriv(i1,i2,i3,c)

end do
end do
end do
end do

else
  write(*,*) 'dsgc6: ERROR invalid nd=',nd
  stop 11
end if


#Else
   ! unexpected operatorName
   stop 2893
#End


end if


return
end


#endMacro




      divScalarGradConservativeHigherOrder(laplace6Cons,6)
      divScalarGradConservativeHigherOrder(divScalarGrad6Cons,6)
      divScalarGradConservativeHigherOrder(divTensorGrad6Cons,6)

      divScalarGradConservativeHigherOrder(laplace8Cons,8)
      divScalarGradConservativeHigherOrder(divScalarGrad8Cons,8)
      divScalarGradConservativeHigherOrder(divTensorGrad8Cons,8)


