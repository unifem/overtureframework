#include "Overture.h"


#define XR(i,j,k,m,n) xr(i,j,k,m+numberOfDimensions*n)
#define RX(i,j,k,m,n) rx(i,j,k,m+numberOfDimensions*n)

void createInverseVertexDerivative( CompositeGrid & cog )
{
  //------------------------------------------------------------------
  // Create inverseVertexDerivative and remove vertexDerivative
  //
  // Notes:
  //   At points where the determinant of the Jacobian matrix is zero
  //   the determinant is changed from zero to one before computing
  //   the inverse.
  //-----------------------------------------------------------------

  cog.update(MappedGrid::THEinverseVertexDerivative);  // this will create the arrays

  for( int grid=0; grid<cog.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cog[grid];

    RealArray & xr = c.vertexDerivative;
    RealArray & rx = c.inverseVertexDerivative;

    Index I=Range(xr.getBase(0),xr.getBound(0));
    Index J=Range(xr.getBase(1),xr.getBound(1));
    Index K=Range(xr.getBase(2),xr.getBound(2));
    
    int numberOfDimensions = cog.numberOfDimensions();

    if( numberOfDimensions==2 )
    {
      RealArray det =  XR(I,J,K,0,0)*XR(I,J,K,1,1)
                      -XR(I,J,K,1,0)*XR(I,J,K,0,1);

      where( det==0. )  // ** check for det=0 ***
        det=1.;
      
      det = 1./det; 
      RX(I,J,K,0,0)= XR(I,J,K,1,1)*det;
      RX(I,J,K,1,0)=-XR(I,J,K,1,0)*det;
      RX(I,J,K,0,1)=-XR(I,J,K,0,1)*det;
      RX(I,J,K,1,1)= XR(I,J,K,0,0)*det;
    }
    else if( numberOfDimensions==3 )
    {
      RealArray det=
          (XR(I,J,K,0,1)*XR(I,J,K,1,2)-XR(I,J,K,1,1)*XR(I,J,K,0,2))*XR(I,J,K,2,0)
         +(XR(I,J,K,0,2)*XR(I,J,K,1,0)-XR(I,J,K,1,2)*XR(I,J,K,0,0))*XR(I,J,K,2,1)
	 +(XR(I,J,K,0,0)*XR(I,J,K,1,1)-XR(I,J,K,1,0)*XR(I,J,K,0,1))*XR(I,J,K,2,2);
      where( det==0. )
        det=1.;
      det=1./det;
      RX(I,J,K,0,0)=(XR(I,J,K,1,1)*XR(I,J,K,2,2)-XR(I,J,K,1,2)*XR(I,J,K,2,1))*det;
      RX(I,J,K,1,0)=(XR(I,J,K,1,2)*XR(I,J,K,2,0)-XR(I,J,K,1,0)*XR(I,J,K,2,2))*det;
      RX(I,J,K,2,0)=(XR(I,J,K,1,0)*XR(I,J,K,2,1)-XR(I,J,K,1,1)*XR(I,J,K,2,0))*det;
      RX(I,J,K,0,1)=(XR(I,J,K,2,1)*XR(I,J,K,0,2)-XR(I,J,K,2,2)*XR(I,J,K,0,1))*det;
      RX(I,J,K,1,1)=(XR(I,J,K,2,2)*XR(I,J,K,0,0)-XR(I,J,K,2,0)*XR(I,J,K,0,2))*det;
      RX(I,J,K,2,1)=(XR(I,J,K,2,0)*XR(I,J,K,0,1)-XR(I,J,K,2,1)*XR(I,J,K,0,0))*det;
      RX(I,J,K,0,2)=(XR(I,J,K,0,1)*XR(I,J,K,1,2)-XR(I,J,K,0,2)*XR(I,J,K,1,1))*det;
      RX(I,J,K,1,2)=(XR(I,J,K,0,2)*XR(I,J,K,1,0)-XR(I,J,K,0,0)*XR(I,J,K,1,2))*det;
      RX(I,J,K,2,2)=(XR(I,J,K,0,0)*XR(I,J,K,1,1)-XR(I,J,K,0,1)*XR(I,J,K,1,0))*det;
    }
    else
    {
      cerr << "InvertJacobian: ERROR : numberOfDimensions = " 
	<< numberOfDimensions << endl;
    }
    
    // "remove" the vertexDerivative array:   ************** do we want to do this ? *****
    // ***    c.vertexDerivative.redim(0);
    
  }
    
}


void createInverseVertexDerivative( MappedGrid & c )
{
  //------------------------------------------------------------------
  // Create inverseVertexDerivative and remove vertexDerivative
  //
  // Notes:
  //   At points where the determinant of the Jacobian matrix is zero
  //   the determinant is changed from zero to one before computing
  //   the inverse.
  //-----------------------------------------------------------------
  c.update(MappedGrid::THEinverseVertexDerivative);  // this will create the arrays

  RealArray & xr = c.vertexDerivative;
  RealArray & rx = c.inverseVertexDerivative;

  Index I=Range(xr.getBase(0),xr.getBound(0));
  Index J=Range(xr.getBase(1),xr.getBound(1));
  Index K=Range(xr.getBase(2),xr.getBound(2));
  

  int numberOfDimensions = c.numberOfDimensions();

  if( numberOfDimensions==2 )
  {
    RealArray det =  XR(I,J,K,0,0)*XR(I,J,K,1,1)
      -XR(I,J,K,1,0)*XR(I,J,K,0,1);

    where( det==0. )  // ** check for det=0 ***
      det=1.;
    
    det = 1./det; 
    RX(I,J,K,0,0)= XR(I,J,K,1,1)*det;
    RX(I,J,K,1,0)=-XR(I,J,K,1,0)*det;
    RX(I,J,K,0,1)=-XR(I,J,K,0,1)*det;
    RX(I,J,K,1,1)= XR(I,J,K,0,0)*det;
  }
  else if( numberOfDimensions==3 )
  {
    RealArray det=
      (XR(I,J,K,0,1)*XR(I,J,K,1,2)-XR(I,J,K,1,1)*XR(I,J,K,0,2))*XR(I,J,K,2,0)
     +(XR(I,J,K,0,2)*XR(I,J,K,1,0)-XR(I,J,K,1,2)*XR(I,J,K,0,0))*XR(I,J,K,2,1)
     +(XR(I,J,K,0,0)*XR(I,J,K,1,1)-XR(I,J,K,1,0)*XR(I,J,K,0,1))*XR(I,J,K,2,2);
    where( det==0. )
      det=1.;
    det=1./det;
    RX(I,J,K,0,0)=(XR(I,J,K,1,1)*XR(I,J,K,2,2)-XR(I,J,K,1,2)*XR(I,J,K,2,1))*det;
    RX(I,J,K,1,0)=(XR(I,J,K,1,2)*XR(I,J,K,2,0)-XR(I,J,K,1,0)*XR(I,J,K,2,2))*det;
    RX(I,J,K,2,0)=(XR(I,J,K,1,0)*XR(I,J,K,2,1)-XR(I,J,K,1,1)*XR(I,J,K,2,0))*det;
    RX(I,J,K,0,1)=(XR(I,J,K,2,1)*XR(I,J,K,0,2)-XR(I,J,K,2,2)*XR(I,J,K,0,1))*det;
    RX(I,J,K,1,1)=(XR(I,J,K,2,2)*XR(I,J,K,0,0)-XR(I,J,K,2,0)*XR(I,J,K,0,2))*det;
    RX(I,J,K,2,1)=(XR(I,J,K,2,0)*XR(I,J,K,0,1)-XR(I,J,K,2,1)*XR(I,J,K,0,0))*det;
    RX(I,J,K,0,2)=(XR(I,J,K,0,1)*XR(I,J,K,1,2)-XR(I,J,K,0,2)*XR(I,J,K,1,1))*det;
    RX(I,J,K,1,2)=(XR(I,J,K,0,2)*XR(I,J,K,1,0)-XR(I,J,K,0,0)*XR(I,J,K,1,2))*det;
    RX(I,J,K,2,2)=(XR(I,J,K,0,0)*XR(I,J,K,1,1)-XR(I,J,K,0,1)*XR(I,J,K,1,0))*det;
  }
  else
  {
    cerr << "InvertJacobian: ERROR : numberOfDimensions = " 
      << numberOfDimensions << endl;
  }
  
  // "remove" the vertexDerivative array:   ************** do we want to do this ? *****
  // *** c.vertexDerivative.redim(0);
  
}
