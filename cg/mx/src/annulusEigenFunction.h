//==================================================================================================
// Evaluate the annulus eigenfunction or it's error
// 
// OPTION: solution, error
//==================================================================================================
#beginMacro annulusEigenFunction(OPTION,J1,J2,J3)

//   printF(" I1.getBase(),uLocal.getBase(0),I1.getBound(),uLocal.getBound(0)=%i %i %i %i \n",
// 	 I1.getBase(),uLocal.getBase(0),I1.getBound(),uLocal.getBound(0));

//  Index J1 = Range(max(I1.getBase(),uLocal.getBase(0)),min(I1.getBound(),uLocal.getBound(0)));
//  Index J2 = Range(max(I2.getBase(),uLocal.getBase(1)),min(I2.getBound(),uLocal.getBound(1)));
//  Index J3 = Range(max(I3.getBase(),uLocal.getBase(2)),min(I3.getBound(),uLocal.getBound(2)));

 if( numberOfDimensions==2 )
 {
#include "besselPrimeZeros.h"

   const int n = int(initialConditionParameters[0]+.5);  // angular number, n=0,1,... --> Jn(omega*r)
   const int m = int(initialConditionParameters[1]+.5);  // radial number m=0,... 
   
   assert( m<mdbpz && n<ndbpz );
   
   real omega = besselPrimeZeros[n][m];  // m'th zero of Jn' (excluding r=0 for J0)
   
// printF("Annulus: Bessel function solution: n=%i, m=%i, omega=%e (c=%8.2e)\n",n,m,omega,c);

   const real eps=sqrt(REAL_EPSILON);
   
   real np1Factorial=1.;
   for( int k=2; k<=n+1; k++ )
     np1Factorial*=k;              //  (n+1)!

   int i1,i2,i3;
   real r,gr,xd,yd,zd,bj,bjp,rx,ry,theta,thetax,thetay;
   real cosTheta,sinTheta,bjThetax,bjThetay,uex,uey,cosn,sinn;


   FOR_3D(i1,i2,i3,J1,J2,J3)
   {
     xd=X(i1,i2,i3,0);
     yd=X(i1,i2,i3,1);
     #If #DIM == "3" 
       zd=X(i1,i2,i3,2)
       sinPhi=sin(Pi*zd)
       cosPhi=cos(Pi*zd)
     #End
     r = sqrt(xd*xd+yd*yd);
     theta=atan2(yd,xd);
     // if( theta<0. ) theta+=2.*Pi;
     cosTheta=cos(theta);
     sinTheta=sin(theta);
     
     cosn=cos(n*theta);
     sinn=sin(n*theta);
     
     gr=omega*r;
     
     rx = cosTheta;  // x/r
     ry = sinTheta;  // y/r
     
     bj=jn(n,gr);  // Bessel function J of order n
     
     
     if( gr>eps )  // need asymptotic expansion for small gr ??
     {
       bjp = -jn(n+1,gr) + n*bj/gr;  // from the recursion relation for Jn'
       thetay= cosTheta/r;
       thetax=-sinTheta/r;
     
       uex =  (1./omega)*(omega*ry*bjp*cosn -n*bj*thetay*sinn);
       uey = -(1./omega)*(omega*rx*bjp*cosn -n*bj*thetax*sinn);
     
     
     }
     else
     {
       // Jn(z) = (.5*z)^n *( 1 - (z*z/4)/(n+1)! + .. 
     
     
       // At r=0 all the Jn'(0) are zero except for n=1
       // bjp = n==1 ? 1./2. : 0.;
       bjp = n==0 ? 0. : pow(.5,double(n))*pow(gr,n-1.)*( 1. - (gr*gr)/(4.*np1Factorial) );
     
       // bj/r = omega*bjp at r=0
       bjThetay= omega*bjp*cosTheta;
       bjThetax=-omega*bjp*sinTheta;
     
       uex =  (1./omega)*(omega*ry*bjp*cosn -n*bjThetay*sinn);  // Ex.t = Hz.y
       uey = -(1./omega)*(omega*rx*bjp*cosn -n*bjThetax*sinn);  // Ey.t = - Hz.x
     
     }

     real sint = sin(omega*t), cost = cos(omega*t);
     
     #If #OPTION == "solution"
       UHZ(i1,i2,i3)  = bj*cosn*cost;
       UEX(i1,i2,i3) = uex*sint;  // Ex.t = Hz.y
       UEY(i1,i2,i3) = uey*sint;  // Ey.t = - Hz.x
     
       if( method==nfdtd )
       {
         UMHZ(i1,i2,i3) = bj*cosn*cos(omega*(t-dt));
         UMEX(i1,i2,i3) = uex*sin(omega*(t-dt)); 
         UMEY(i1,i2,i3) = uey*sin(omega*(t-dt)); 
       }
       else if( method==sosup )
       {
         uLocal(i1,i2,i3,hzt) = -omega*bj*cosn*sint;
         uLocal(i1,i2,i3,ext) = omega*uex*cost;
         uLocal(i1,i2,i3,eyt) = omega*uey*cost;
       }
       
     #Elif #OPTION == "boundaryCondition"
       // *check me*
       uLocal(i1,i2,i3,hz)  = bj*cosn*cost;
       uLocal(i1,i2,i3,ex) = uex*sint;  // Ex.t = Hz.y
       uLocal(i1,i2,i3,ey) = uey*sint;  // Ey.t = - Hz.x
     
       if( method==sosup )
       {
         uLocal(i1,i2,i3,hzt) = -omega*bj*cosn*sint;
         uLocal(i1,i2,i3,ext) = omega*uex*cost;
         uLocal(i1,i2,i3,eyt) = omega*uey*cost;
       }

     #Elif #OPTION == "error"
       ERRHZ(i1,i2,i3) = UHZ(i1,i2,i3) -bj*cosn*cos(omega*t);
       ERREX(i1,i2,i3) = UEX(i1,i2,i3) - uex*sin(omega*t);  // Ex.t = Hz.y
       ERREY(i1,i2,i3) = UEY(i1,i2,i3) - uey*sin(omega*t);  // Ey.t = - Hz.x
       if( method==sosup )
       {
         errLocal(i1,i2,i3,hzt) = uLocal(i1,i2,i3,hzt) + omega*bj*cosn*sint;
         errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext) - omega*uex*cost;
         errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt) - omega*uey*cost;
       }
     #Else
       Overture::abort("error");
     #End
  
  }
 }
 else /* 3D */
 {
#include "besselZeros.h"

   const real cylinderLength=cylinderAxisEnd-cylinderAxisStart;

   const int n = int(initialConditionParameters[0]+.5);  // angular number, n=0,1,... --> Jn(omega*r)
   const int m = int(initialConditionParameters[1]+.5);  // radial number m=0,... 
   const int k = int(initialConditionParameters[2]+.5);  // axial number k=1,2,3,...
   
   assert( m<mdbz && n<ndbz );
   
   real lambda = besselZeros[n][m];  // m'th zero of Jn (excluding r=0 for J0)
   real omega = sqrt( SQR(k*Pi/cylinderLength) + lambda*lambda );
   
   printF("***Cylinder: Bessel function soln: n=%i, m=%i, k=%i, lambda=%e, omega=%e (c=%8.2e) [za,zb]=[%4.2f,%4.2f]\n",
          n,m,k,lambda,omega,c,cylinderAxisStart,cylinderAxisEnd);

   const real eps=sqrt(REAL_EPSILON);
   
   real np1Factorial=1.;
   for( int k=2; k<=n+1; k++ )
     np1Factorial*=k;              //  (n+1)!

   int i1,i2,i3;
   real r,gr,xd,yd,zd,bj,bjp,rx,ry,theta,thetax,thetay;
   real cosTheta,sinTheta,bjThetax,bjThetay,uex,uey,cosn,sinn,sinkz,coskz,cost,sint;
   
   FOR_3D(i1,i2,i3,J1,J2,J3)
   {
     xd=X(i1,i2,i3,0);
     yd=X(i1,i2,i3,1);
     zd=(X(i1,i2,i3,2)-cylinderAxisStart)/cylinderLength; // *wdh* 040626 -- allow for any length
     
     sinkz=sin(Pi*k*zd);   
     coskz=cos(Pi*k*zd); 

     r = sqrt(xd*xd+yd*yd);
     theta=atan2(yd,xd);

     cosTheta=cos(theta);
     sinTheta=sin(theta);
     
     cosn=cos(n*theta);
     sinn=sin(n*theta);

     cost=cos(omega*t);
     
     gr=lambda*r;
     
     rx = cosTheta;  // x/r
     ry = sinTheta;  // y/r
     
     bj=jn(n,gr);  // Bessel function J of order n
     
     
     if( gr>eps )  // need asymptotic expansion for small gr ??
     {
       bjp = -jn(n+1,gr) + n*bj/gr;  // from the recursion relation for Jn'
       thetay= cosTheta/r;
       thetax=-sinTheta/r;
     
       uex = -(k*Pi/(cylinderLength*lambda*lambda))*( lambda*rx*bjp*cosn - n*bj*thetax*sinn );
       uey = -(k*Pi/(cylinderLength*lambda*lambda))*( lambda*ry*bjp*cosn - n*bj*thetay*sinn );
     
     
     }
     else
     {
       // Jn(z) = (.5*z)^n *( 1 - (z*z/4)/(n+1)! + .. 
     
     
       // At r=0 all the Jn'(0) are zero except for n=1
       // bjp = n==1 ? 1./2. : 0.;
       bjp = n==0 ? 0. : pow(.5,double(n))*pow(gr,n-1.)*( 1. - (gr*gr)/(4.*np1Factorial) );
     
       // bj/r = lambda*bjp at r=0
       bjThetay= lambda*bjp*cosTheta;
       bjThetax=-lambda*bjp*sinTheta;
     
       uex = -(k*Pi/(cylinderLength*lambda*lambda))*( lambda*rx*bjp*cosn -n*bjThetax*sinn);  // Ex.t = Hz.y
       uey = -(k*Pi/(cylinderLength*lambda*lambda))*( lambda*ry*bjp*cosn -n*bjThetay*sinn);  // Ey.t = - Hz.x
     
     }

     #If #OPTION == "solution"

       UEX(i1,i2,i3) = uex*sinkz*cost;
       UEY(i1,i2,i3) = uey*sinkz*cost;
       UEZ(i1,i2,i3) = bj*cosn*coskz*cost;
     
       cost=cos(omega*(t-dt)); 
       UMEX(i1,i2,i3) = uex*sinkz*cost;
       UMEY(i1,i2,i3) = uey*sinkz*cost;
       UMEZ(i1,i2,i3) = bj*cosn*coskz*cost;
     
       if( method==sosup )
       {
         sint=sin(omega*t); 
         uLocal(i1,i2,i3,ext) = -omega*uex*sinkz*sint;
         uLocal(i1,i2,i3,eyt) = -omega*uey*sinkz*sint;
         uLocal(i1,i2,i3,ezt) = -omega*bj*cosn*coskz*sint;
       }

     #Elif #OPTION == "boundaryCondition"

       // *check me*
       uLocal(i1,i2,i3,ex) = uex*sinkz*cost;
       uLocal(i1,i2,i3,ey) = uey*sinkz*cost;
       uLocal(i1,i2,i3,ez) = bj*cosn*coskz*cost;
     
       if( method==sosup )
       {
         sint=sin(omega*t); 
         uLocal(i1,i2,i3,ext) = -omega*uex*sinkz*sint;
         uLocal(i1,i2,i3,eyt) = -omega*uey*sinkz*sint;
         uLocal(i1,i2,i3,ezt) = -omega*bj*cosn*coskz*sint;
       }

     #Elif #OPTION == "error"


       ERREX(i1,i2,i3) = UEX(i1,i2,i3) - uex*sinkz*cost;
       ERREY(i1,i2,i3) = UEY(i1,i2,i3) - uey*sinkz*cost;
       ERREZ(i1,i2,i3) = UEZ(i1,i2,i3) - bj*cosn*coskz*cost;

       if( method==sosup )
       {
         sint=sin(omega*t); 
         errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext) + omega*uex*sinkz*sint;
         errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt) + omega*uey*sinkz*sint;
         errLocal(i1,i2,i3,ezt) = uLocal(i1,i2,i3,ezt) + omega*bj*cosn*coskz*sint;
       }
     #Else
       Overture::abort("error");
     #End
  
  }
 }

#endMacro
