#include "Overture.h"
#include "wdhdefs.h"           // some useful defines and constants
#include "mathutil.h"          // define max, min,  etc

#define NORMAL   \
        normal[side+2*(axis+numberOfDimensions*(grid))]
#define XR(i1,i2,i3,m,n) c.vertexDerivative(i1,i2,i3,m+numberOfDimensions*(n))

#ifdef OV_USE_DOUBLE
  typedef ListOfDoubleArray ListOfRealArray;
#else
  typedef  ListOfFloatArray ListOfRealArray;
#endif

void createNormals( CompositeGrid & og, ListOfRealArray & normal )
{
  //==========================================================================
  // Create arrays that hold the INWARD normal vectors on the boundaries
  //
  // The normals on side=0,1 and axis=0,1,numberOfDimensions-1 and grid
  // ...
  // Input -
  //   
  //=========================================================================


  // cout << "createNormals: compute Normals... " << endl;

  int numberOfDimensions=og.numberOfDimensions;
  
  const int Start=0;
  const int End=1;
  const int axis1=0;
  const int axis2=1;
  const int axis3=2;
  

  RealArray l2Norm;
  Range R[3];
  real aj,signOfJacobian;
  int grid,axis,side,dir,i1,i2,i3,ap1,ap2;
  Index I1,I2,I3;

  for( grid=0; grid<og.numberOfComponentGrids; grid++ )
  {
    MappedGrid & c = og[grid];
    if( c.vertexDerivative.getLength(0) < 1 )
    {
      cout << "createNormals::ERROR vertexDerivative not found! " << endl;
      exit(1);
    }
    // Define normals on all sides (faces) of the grid
    for( axis=axis1; axis<numberOfDimensions; axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
        normal.addElement( *(new RealArray) );
         //  Define ranges for normal on this side
        for( dir=axis1; dir<3; dir++ )
        {
  	  R[dir]= dir != axis ?
	    Range(c.dimension(Start,dir),c.dimension(End ,dir)) :
	    Range(c.gridIndexRange(side,dir),c.gridIndexRange(side,dir));
        }
        // NORMAL(i1,i2,i3,dim)
        NORMAL.redim(R[0],R[1],R[2],Range(0,numberOfDimensions-1));
        l2Norm.redim(R[0],R[1],R[2]);

        // Get the sign of the Jacobian
        i1=NORMAL.getBase(axis1); 
        i2=NORMAL.getBase(axis2);
        i3=NORMAL.getBase(axis3);
        if( numberOfDimensions == 2  )
          aj=XR(i1,i2,i3,0,0)*XR(i1,i2,i3,1,1)-XR(i1,i2,i3,0,1)*XR(i1,i2,i3,1,0);
        else
          aj =XR(i1,i2,i3,0,0)*
                (XR(i1,i2,i3,1,1)*XR(i1,i2,i3,2,2)-XR(i1,i2,i3,1,2)*XR(i1,i2,i3,2,1))
             +XR(i1,i2,i3,0,1)*
                (XR(i1,i2,i3,1,2)*XR(i1,i2,i3,2,0)-XR(i1,i2,i3,1,0)*XR(i1,i2,i3,2,2))
	     +XR(i1,i2,i3,0,2)*
                (XR(i1,i2,i3,1,0)*XR(i1,i2,i3,2,1)-XR(i1,i2,i3,1,1)*XR(i1,i2,i3,2,0));
        signOfJacobian= aj>0. ? 1. : -1.;  // sign of the jacobian
        if( signOfJacobian < 0. )
          cout << "createNormals::WARNING: Jacobian is negative on grid =" << grid << endl;
	 
// c       ...get normal by a cross product
//        kdp1=mod(kd  ,nd)+1
//        kdp2=mod(kdp1,nd)+1
//        kdp3=mod(kdp2,nd)+1
//        an(1)=xr(2,kdp1)*xr(3,kdp2)-xr(3,kdp1)*xr(2,kdp2)
//        an(2)=xr(3,kdp1)*xr(1,kdp2)-xr(1,kdp1)*xr(3,kdp2)
//        an(3)=xr(1,kdp1)*xr(2,kdp2)-xr(2,kdp1)*xr(1,kdp2)
//        anli=(-1)**ks*ajs/sqrt(an(1)**2+an(2)**2+an(3)**2)
//        an(1)=an(1)*anli
//        an(2)=an(2)*anli
//        an(3)=an(3)*anli

        ap1=(axis+1) % numberOfDimensions;
        ap2=(axis+2) % numberOfDimensions;
        I1=R[0]; I2=R[1]; I3=R[2];
	if( numberOfDimensions==2)
	{
          NORMAL(I1,I2,I3,axis1)=+XR(I1,I2,I3,axis2,ap1); 
          NORMAL(I1,I2,I3,axis2)=-XR(I1,I2,I3,axis1,ap1);
          l2Norm(I1,I2,I3)=(1-2*side)*(1-2*axis)*signOfJacobian/
             SQRT(SQR(NORMAL(I1,I2,I3,axis1))+
                  SQR(NORMAL(I1,I2,I3,axis2)));
	}
	else if( numberOfDimensions==3 )
	{
          NORMAL(I1,I2,I3,axis1)=
            XR(I1,I2,I3,1,ap1)*XR(I1,I2,I3,2,ap2)-XR(I1,I2,I3,2,ap1)*XR(I1,I2,I3,1,ap2);
          NORMAL(I1,I2,I3,axis2)=
            XR(I1,I2,I3,2,ap1)*XR(I1,I2,I3,0,ap2)-XR(I1,I2,I3,0,ap1)*XR(I1,I2,I3,2,ap2);
          NORMAL(I1,I2,I3,axis3)=
	    XR(I1,I2,I3,0,ap1)*XR(I1,I2,I3,1,ap2)-XR(I1,I2,I3,1,ap1)*XR(I1,I2,I3,0,ap2);
	  
          l2Norm(I1,I2,I3)=(1-2*side)*signOfJacobian/
             SQRT(SQR(NORMAL(I1,I2,I3,axis1))+
                  SQR(NORMAL(I1,I2,I3,axis2))+
                  SQR(NORMAL(I1,I2,I3,axis3)));
	  
	}
        // normalize the normal to have length 1
        for( dir=axis1; dir<numberOfDimensions; dir++ )
	  NORMAL(I1,I2,I3,dir)=NORMAL(I1,I2,I3,dir)*l2Norm(I1,I2,I3);
	  
        //cout << "***createNormals: axis = " << axis << ", side = " << side << endl;
	//NORMAL.display("Here are the normals" );

      }
    }
  }
}


#undef  NORMAL
#define NORMAL   \
        normal[side+2*(axis)]
void createNormals( MappedGrid & c, ListOfRealArray & normal )
{
  //==========================================================================
  // Create arrays that hold the INWARD normal vectors on the boundaries
  //
  // The normals on side=0,1 and axis=0,1,numberOfDimensions-1 and grid
  // ...
  // Input -
  //   
  //=========================================================================


  // cout << "createNormals: compute Normals... " << endl;

  int numberOfDimensions=c.numberOfDimensions;
  
  const int Start=0;
  const int End=1;
  const int axis1=0;
  const int axis2=1;
  const int axis3=2;
  

  RealArray l2Norm;
  Range R[3];
  real aj,signOfJacobian;
  int axis,side,dir,i1,i2,i3,ap1,ap2;
  Index I1,I2,I3;

  if( c.vertexDerivative.dimension(0) < 1 )
  {
    cout << "createNormals::ERROR vertexDerivative not found! " << endl;
    exit(1);
  }
  // Define normals on all sides (faces) of the grid
  for( axis=axis1; axis<numberOfDimensions; axis++ )
  {
    for( side=Start; side<=End; side++ )
    {
      normal.addElement( *(new RealArray) );
      //  Define ranges for normal on this side
      for( dir=axis1; dir<3; dir++ )
      {
	R[dir]= dir != axis ?
	  Range(c.dimension(Start,dir),c.dimension(End ,dir)) :
	  Range(c.gridIndexRange(side,dir),c.gridIndexRange(side,dir));
      }
      // NORMAL(i1,i2,i3,dim)
      NORMAL.redim(R[0],R[1],R[2],Range(0,numberOfDimensions-1));
      l2Norm.redim(R[0],R[1],R[2]);

      // Get the sign of the Jacobian
      i1=NORMAL.getBase(axis1); 
      i2=NORMAL.getBase(axis2);
      i3=NORMAL.getBase(axis3);
      if( numberOfDimensions == 2  )
	aj=XR(i1,i2,i3,0,0)*XR(i1,i2,i3,1,1)-XR(i1,i2,i3,0,1)*XR(i1,i2,i3,1,0);
      else
	aj =XR(i1,i2,i3,0,0)*
	  (XR(i1,i2,i3,1,1)*XR(i1,i2,i3,2,2)-XR(i1,i2,i3,1,2)*XR(i1,i2,i3,2,1))
	    +XR(i1,i2,i3,0,1)*
	      (XR(i1,i2,i3,1,2)*XR(i1,i2,i3,2,0)-XR(i1,i2,i3,1,0)*XR(i1,i2,i3,2,2))
		+XR(i1,i2,i3,0,2)*
		  (XR(i1,i2,i3,1,0)*XR(i1,i2,i3,2,1)-XR(i1,i2,i3,1,1)*XR(i1,i2,i3,2,0));
      signOfJacobian= aj>0. ? 1. : -1.;  // sign of the jacobian
      if( signOfJacobian < 0. )
	cout << "createNormals::WARNING: Jacobian is negative on grid \n";
      
      // c       ...get normal by a cross product
      //        kdp1=mod(kd  ,nd)+1
      //        kdp2=mod(kdp1,nd)+1
      //        kdp3=mod(kdp2,nd)+1
      //        an(1)=xr(2,kdp1)*xr(3,kdp2)-xr(3,kdp1)*xr(2,kdp2)
      //        an(2)=xr(3,kdp1)*xr(1,kdp2)-xr(1,kdp1)*xr(3,kdp2)
      //        an(3)=xr(1,kdp1)*xr(2,kdp2)-xr(2,kdp1)*xr(1,kdp2)
      //        anli=(-1)**ks*ajs/sqrt(an(1)**2+an(2)**2+an(3)**2)
      //        an(1)=an(1)*anli
      //        an(2)=an(2)*anli
      //        an(3)=an(3)*anli

      ap1=(axis+1) % numberOfDimensions;
      ap2=(axis+2) % numberOfDimensions;
      I1=R[0]; I2=R[1]; I3=R[2];
      if( numberOfDimensions==2)
      {
	NORMAL(I1,I2,I3,axis1)=+XR(I1,I2,I3,axis2,ap1); 
	NORMAL(I1,I2,I3,axis2)=-XR(I1,I2,I3,axis1,ap1);
	l2Norm(I1,I2,I3)=(1-2*side)*(1-2*axis)*signOfJacobian/
	  SQRT(SQR(NORMAL(I1,I2,I3,axis1))+
	       SQR(NORMAL(I1,I2,I3,axis2)));
      }
      else if( numberOfDimensions==3 )
      {
	NORMAL(I1,I2,I3,axis1)=
	  XR(I1,I2,I3,1,ap1)*XR(I1,I2,I3,2,ap2)-XR(I1,I2,I3,2,ap1)*XR(I1,I2,I3,1,ap2);
	NORMAL(I1,I2,I3,axis2)=
	  XR(I1,I2,I3,2,ap1)*XR(I1,I2,I3,0,ap2)-XR(I1,I2,I3,0,ap1)*XR(I1,I2,I3,2,ap2);
	NORMAL(I1,I2,I3,axis3)=
	  XR(I1,I2,I3,0,ap1)*XR(I1,I2,I3,1,ap2)-XR(I1,I2,I3,1,ap1)*XR(I1,I2,I3,0,ap2);
	
	l2Norm(I1,I2,I3)=(1-2*side)*signOfJacobian/
	  SQRT(SQR(NORMAL(I1,I2,I3,axis1))+
	       SQR(NORMAL(I1,I2,I3,axis2))+
	       SQR(NORMAL(I1,I2,I3,axis3)));
	
      }
      // normalize the normal to have length 1
      for( dir=axis1; dir<numberOfDimensions; dir++ )
	NORMAL(I1,I2,I3,dir)=NORMAL(I1,I2,I3,dir)*l2Norm(I1,I2,I3);
      
      //cout << "***createNormals: axis = " << axis << ", side = " << side << endl;
      //NORMAL.display("Here are the normals" );

    }
  }
}


#undef  NORMAL
