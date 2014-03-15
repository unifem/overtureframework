#include "MappedGridOperators.h"
#include "FourierOperators.h"


void MappedGridOperators::
spectralDerivatives(const int & numberOfDerivatives,
		    const IntegerArray & derivativesToEvaluate,  
		    RealDistributedArray *derivative[],
		    const realMappedGridFunction & u, 
		    const Range & R1,
		    const Range & R2, 
		    const Range & R3, 
		    const Range & R4)
//
// protected routine: compute derivatives using the pseudo spectral method
//
{
  if( fourierOperators==NULL )
  {
    int nx = mappedGrid.gridIndexRange(End,axis1)-mappedGrid.gridIndexRange(Start,axis1);
    int ny = mappedGrid.gridIndexRange(End,axis2)-mappedGrid.gridIndexRange(Start,axis2);
    int nz = mappedGrid.gridIndexRange(End,axis3)-mappedGrid.gridIndexRange(Start,axis3);
    real xPeriod =nx*dx[0]; // .5*nx/h21(axis1); 
    real yPeriod =numberOfDimensions>1 ? ny*dx[1] : 1.; 
    real zPeriod =numberOfDimensions>2 ? nz*dx[2] : 1.; 
    
    nx=max(nx,1);
    ny=max(ny,1);
    nz=max(nz,1);
    
    printf("spectralDerivatives: nx=%i, ny=%i, nz=%i, (periods=%e,%e,%e) \n",nx,ny,nz,xPeriod,yPeriod,zPeriod);
    // check for a power of two ******
    
    fourierOperators= new FourierOperators(mappedGrid.numberOfDimensions(),nx,ny,nz);
    fourierOperators->setPeriod(xPeriod,yPeriod,zPeriod);  
    Range F1(mappedGrid.gridIndexRange(Start,axis1),mappedGrid.gridIndexRange(End,axis1)-1);
    Range F2(mappedGrid.gridIndexRange(Start,axis2),mappedGrid.gridIndexRange(End,axis2)-1);
    Range F3(mappedGrid.gridIndexRange(Start,axis3),mappedGrid.gridIndexRange(End,axis3)-1);
    
    fourierOperators->setDefaultRanges(F1,F2,F3);
  }
    
  assert( fourierOperators!=NULL );


  FourierOperators & f = *fourierOperators;

//realDistributedArray & v = u;
//v.reshape();
  
  RealDistributedArray uHat,uHatX;
  uHat.redim(u);       //
  uHatX.redim(uHat);
  realMappedGridFunction ux;
  ux.updateToMatchGridFunction(u);

  f.realToFourier( u,uHat );

  for( int i=0; i<numberOfDerivatives; i++ )
  {
    switch (derivativesToEvaluate(i))
    {
    case xDerivative:
      f.fourierDerivative(uHat,uHatX,1);   // x derivative
      break;
    case yDerivative:
      f.fourierDerivative(uHat,uHatX,0,1);  
      break;
    case zDerivative:
      f.fourierDerivative(uHat,uHatX,0,0,1);  
      break;
    case xxDerivative:
      f.fourierDerivative(uHat,uHatX,2);  
      break;
    case xyDerivative:
      f.fourierDerivative(uHat,uHatX,1,1);  
      break;
    case xzDerivative:
      f.fourierDerivative(uHat,uHatX,1,0,1);  
      break;
    case yxDerivative:
      f.fourierDerivative(uHat,uHatX,1,1,0);  
      break;
    case yyDerivative:
      f.fourierDerivative(uHat,uHatX,0,2);  
      break;
    case yzDerivative:
      f.fourierDerivative(uHat,uHatX,0,1,1);  
      break;
    case zxDerivative:
      f.fourierDerivative(uHat,uHatX,1,0,1);  
      break;
    case zyDerivative:
      f.fourierDerivative(uHat,uHatX,0,1,1);  
      break;
    case zzDerivative:
      f.fourierDerivative(uHat,uHatX,0,0,2);  
      break;
    case laplacianOperator:
      f.fourierLaplacian(uHat,uHatX,1);  
      break;
    case r1Derivative:
      f.fourierDerivative(uHat,uHatX,1,0,0);  
      break;
    case r2Derivative:
      f.fourierDerivative(uHat,uHatX,0,1,0);  
      break;
    case r3Derivative:
      f.fourierDerivative(uHat,uHatX,0,0,1);  
      break;
    case r1r1Derivative:
      f.fourierDerivative(uHat,uHatX,1,0,0);  
      break;
    case r1r2Derivative:
      f.fourierDerivative(uHat,uHatX,1,1,0);  
      break;
    case r1r3Derivative:
      f.fourierDerivative(uHat,uHatX,1,0,1);  
      break;
    case r2r2Derivative:
      f.fourierDerivative(uHat,uHatX,0,2,0);  
      break;
    case r2r3Derivative:
      f.fourierDerivative(uHat,uHatX,0,1,1);  
      break;
    case r3r3Derivative:
      f.fourierDerivative(uHat,uHatX,0,0,2);  
      break;
    case gradient:
      break;
    case divergence:
      break;
    case divergenceScalarGradient:
      break;
    case identityOperator:
      break;
    default:
      break;
    }
    // uHat.display("uHat");
    // uHatX.display("uHatX");
    
    f.fourierToReal( uHatX,ux );
    ux.periodicUpdate();
    (*derivative[i])(R1,R2,R3,R4)=ux(R1,R2,R3,R4); 
  }
  
}

