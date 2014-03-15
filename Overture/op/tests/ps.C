#include "FourierOperators.h"
//=========================================================================
// Test out the FourierOperators Class
//=========================================================================

// define a function and derivatives
#define U(x,y)   sin(px*x)*cos(py*y) 

#define UX(x,y)  px*cos(px*x)*cos(py*y)
#define UY(x,y) -py*sin(px*x)*sin(py*y)
#define U_LAPLACIAN(x,y) -(px*px+py*py)*sin(px*x)*cos(py*y)
#define U_INVERSE_LAPLACIAN(x,y) sin(px*x)*cos(py*y)*(-1./(px*px+py*py))

#define X(i)  xPeriod*i/nx
#define Y(j)  yPeriod*j/ny

int
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  int nd=8,nx=8,ny=8;
  realArray u(nd,nd),uHat(nd,nd),u2(nd,nd),uHatX(nd,nd),ux(nd,nd);
  realArray x(nd,nd),y(nd,nd);
  Range R1(0,nx-1),R2(0,ny-1);
  
  // x is periodic with period xPeriod, y is periodic with period yPeriod
  real xPeriod=1., yPeriod=2., px=twoPi/xPeriod, py=twoPi/yPeriod;   
  // assign values to x,y, and u
  int i,j;
  for( j=0; j<ny; j++ )
    for( i=0; i<nx; i++ )
    {
      x(i,j)=X(i);
      y(i,j)=Y(j);
    }
  u(R1,R2)=U(x(R1,R2),y(R1,R2));

  int numberOfDimensions=2; 
  FourierOperators fourier(numberOfDimensions,nx,ny);
  fourier.setPeriod(xPeriod,yPeriod);
  
  u.display("Here is u");
  fourier.realToFourier( u,uHat );
  uHat.display("Here is uHat");
  fourier.fourierToReal( uHat,u2 );

  real maxError=max(fabs(u2-U(x,y)));
  cout << "Maximum error in F^-1(Fu) = " << maxError << endl;
  // u2.display("Here is F^-1(Fu back again ");
  
  fourier.fourierDerivative(uHat,uHatX,1);   // x derivative
  // wHatX.display("Here is wHatX");
  fourier.fourierToReal( uHatX,ux );
  maxError=max(fabs(ux-UX(x,y)));
  cout << "Maximum error in u.x = " << maxError << endl;

  fourier.fourierDerivative(uHat,uHatX,0,1);   // y derivative
  fourier.fourierToReal( uHatX,ux );
  maxError=max(fabs(ux-UY(x,y)));
  cout << "Maximum error in u.y = " << maxError << endl;

  fourier.fourierLaplacian(uHat,uHatX,1);   // xx+yy derivative
  fourier.fourierToReal( uHatX,ux );
  maxError=max(fabs(ux-U_LAPLACIAN(x,y)));
  cout << "Maximum error in u.xx+u.yy = " << maxError << endl;

  fourier.fourierLaplacian(uHat,uHatX,-1);   // (xx+yy)^-1 operator
  fourier.fourierToReal( uHatX,ux );
  maxError=max(fabs(ux-U_INVERSE_LAPLACIAN(x,y)));
  cout << "Maximum error in inverse laplacian = " << maxError << endl;

  Overture::finish();          
  return 0;
}
