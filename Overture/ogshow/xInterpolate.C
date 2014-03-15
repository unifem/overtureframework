#include "Overture.h"

#define CGNRST EXTERN_C_NAME(cgnrst)
#define CGNRSC EXTERN_C_NAME(cgnrsc)

extern "C"
{
  void CGNRST(const int & ndra, const int & ndrb, const int & ndsa, const int & ndsb, int & mrsab, const real & xy,
              real & x, real & y, int & ip, int & jp, real & distmn );
  void CGNRSC(const int & ndra, const int & ndrb, const int & ndsa, const int & ndsb, int & mrsab, const real & xy,
              real & x, real & y, int & ip, int & jp, real & distmn );
}


#define NRM(axis,grid)  ( gc[grid].indexRange()(End,axis)-gc[grid].indexRange()(Start,axis) )
#define MODR(i,axis,grid)  ( \
  ( (i-gc[grid].indexRange()(Start,axis)+NRM(axis,grid)) % NRM(axis,grid)) \
      +gc[grid].indexRange()(Start,axis) \
                           )

#define INVERSE_CENTER_DERIVATIVE(i1,i2,i3,m,n) inverseCenterDerivative(i1,i2,i3,(m)+numberOfDimensions*(n))
#define CENTER_DERIVATIVE(i1,i2,i3,m,n) centerDerivative(i1,i2,i3,(m)+numberOfDimensions*(n))

// ====================================================================================================
// 
//           Composite Grid Interpolation Routine
//
//  Given some points in space, determine the values of a grid function uv. If interpolation
//  is not possible then extrapolate from the nearest grid point.
//
//  Input-
//   numberOfPointsToInterpolate -
//   componentsToInterpolate(i0a:i0b)  - these int values define which components values to interpolate
//       Thus we interpolate values of component number zero that are equal to 
//          componentsToInterpolate(i) for i=i0a,...,i0b
//   positionToInterpolate(0:2,0,numberOfPointsToInterpolate-1) : (x,y,z) positions
//   indexGuess(0:3,numberOfPointsToInterpolate-1) : (i1,i2,i3,grid) values for initial guess for searches
//   gc - GridCollection
//   uv - realGridCollectionFunction
//  Output -
//   uInterpolated(0:numberOfPointsToInterpolate-1,i1a,i1b) - 
//
//  Return value:
//   0 = success
//   1 = error, unable to interpolate
//  -1 = could not interpolate, but could extrapolate -- extrapolation was performed
//       from the nearest grid point.
//
// Who to blame: Bill Henshaw
// =====================================================================================================
int
xInterpolate(const int numberOfPointsToInterpolate,
             const IntegerArray & componentsToInterpolate,
             const RealArray & positionToInterpolate,
             IntegerArray & indexGuess,
             RealArray & uInterpolated, 
             const realGridCollectionFunction & u,
             const GridCollection & gc,
             const int intopt)
{

  bool debug=FALSE;
  const real epsi=1.e-3;

  bool extrap;
  real distmn;
  int jac=(intopt/8) % 2;

  const int numberOfDimensions=gc.numberOfDimensions();
  
  IntegerArray mrsab(gc.numberOfDimensions(),2);

  int returnValue=1;  // 0=ok, 1=error, -1=extrapolate

  int grid=min(gc.numberOfComponentGrids()-1,max(0,indexGuess(3,0)));  // here is the first grid we check

  for( int ipt=0; ipt<numberOfPointsToInterpolate; ipt++ )
  {
    int ip=indexGuess(0,ipt);
    int jp=indexGuess(1,ipt);
    real x=positionToInterpolate(0,ipt);
    real y=positionToInterpolate(1,ipt);

    real dist=-1.;
    // Loop through the grids until we find a point we can interpolate from ...
    for( int gridn=0; gridn<gc.numberOfComponentGrids(); gridn++ )
    {
      if( gridn>0 ) 
        grid = (grid+1) % gc.numberOfComponentGrids();  // here is the next grid to try;

      if( gc[grid].getGridType()==GenericGrid::unstructuredGrid )
      {
	continue;
      }
      
	    
      const IntegerArray & dimension = gc[grid].dimension();
      const IntegerArray & indexRange = gc[grid].indexRange();
      const IntegerArray & gridIndexRange = gc[grid].gridIndexRange();
      int i3=dimension(Start,axis3);
      
//       const intArray & mask = gc[grid].mask();
//       const RealDistributedArray & center = gc[grid].center();
//       // these next references *must* be grid functions since we use 5 arguments
//       const realMappedGridFunction & centerDerivative = (bool)gc[grid].isAllVertexCentered() ?
//                                                        gc[grid].vertexDerivative() 
//                                                      : gc[grid].centerDerivative();
//       const realMappedGridFunction & inverseCenterDerivative = (bool)gc[grid].isAllVertexCentered() ?
//                                                        gc[grid].inverseVertexDerivative() 
//                                                      : gc[grid].inverseCenterDerivative();

      const RealArray & center = gc[grid].center().getLocalArray();
      const IntegerArray & mask = gc[grid].mask().getLocalArray();
      const RealArray & centerDerivative = gc[grid].centerDerivative().getLocalArray();
      // the inverseCenterDerivative is normall not needed (usually jac==1)
      const RealArray & inverseCenterDerivative = jac==0 ? gc[grid].inverseCenterDerivative().getLocalArray() :
	Overture::nullRealArray();

      //    ....find the nearest point, (ip,jp), to (x,y) on grid k
      for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
      {
        mrsab(axis,Start)=indexRange(Start,axis);     
        mrsab(axis,End  )=indexRange(End  ,axis);     
      }
//       CGNRST(dimension(Start,0),dimension(End,0),dimension(Start,1),dimension(End,1),
// 	     mrsab(0,0),center(center.getBase(0),center.getBase(1),center.getBase(2),0),
//                         x,y,ip,jp,distmn );
      CGNRST(center.getBase(0),center.getBound(0),center.getBase(1),center.getBound(1),
	     mrsab(0,0),center(center.getBase(0),center.getBase(1),center.getBase(2),0),
                        x,y,ip,jp,distmn );
      if( debug )
      {
	printf("xInterpolate: CGNRST: x=%e, y=%e, ip=%i, jp=%i \n",x,y,ip,jp);
      }

      if( mask(ip,jp)==0 )  
        continue;  //  ....Unable to interpolate, try another grid
      //
      //.............Iterpolate from the 4 points
      //               (ip ,jp1)   (ip1,jp1)
      //               (ip ,jp )   (ip1,jp )
      //
      real dr,ds,dra,dsa,dx,dy;
      if((bool)gc[grid].isAllVertexCentered())
      {
        dx=x-center(ip,jp,i3,axis1);
	dy=y-center(ip,jp,i3,axis2);
        if( jac==0 )
	{ //...use rsxy array
	  dr=INVERSE_CENTER_DERIVATIVE(ip,jp,i3,axis1,axis1)*dx
            +INVERSE_CENTER_DERIVATIVE(ip,jp,i3,axis1,axis2)*dy;
	  ds=INVERSE_CENTER_DERIVATIVE(ip,jp,i3,axis2,axis1)*dx
            +INVERSE_CENTER_DERIVATIVE(ip,jp,i3,axis2,axis2)*dy;
        }	  
        else
	{// ...rsxy array is really xyrs
#define XR(i1,i2,i3,m,n) centerDerivative(i1,i2,i3,m+numberOfDimensions*(n))
          real deti=    XR(ip,jp,i3,axis1,axis1)*
                        XR(ip,jp,i3,axis2,axis2)-
			XR(ip,jp,i3,axis1,axis2)*
                        XR(ip,jp,i3,axis2,axis1);
          if( deti==0. )
	    cout << "xInterpolate:ERROR: det(x.r)==0 ! \n";
          deti=1./deti;
	    
          dr=( XR(ip,jp,i3,axis2,axis2)*dx-
               XR(ip,jp,i3,axis1,axis2)*dy)*deti;
          ds=(-XR(ip,jp,i3,axis2,axis1)*dx+
               XR(ip,jp,i3,axis1,axis1)*dy)*deti;
#undef XR
	}
      }
      else 
      {
	dx=x-center(ip,jp,i3,axis1);
	dy=y-center(ip,jp,i3,axis2);
        if(jac==0)
	{// ...use rsxy array
	  dr=INVERSE_CENTER_DERIVATIVE(ip,jp,i3,axis1,axis1)*dx+
             INVERSE_CENTER_DERIVATIVE(ip,jp,i3,axis1,axis2)*dy;
	  ds=INVERSE_CENTER_DERIVATIVE(ip,jp,i3,axis2,axis1)*dx+
             INVERSE_CENTER_DERIVATIVE(ip,jp,i3,axis2,axis2)*dy;
	}
        else
	{//       ...rsxyc array is really xyrs
          real deti=    CENTER_DERIVATIVE(ip,jp,i3,axis1,axis1)*
                        CENTER_DERIVATIVE(ip,jp,i3,axis2,axis2)-
                        CENTER_DERIVATIVE(ip,jp,i3,axis1,axis2)*
                        CENTER_DERIVATIVE(ip,jp,i3,axis2,axis1);
          if( deti==0. )
	  {
	    cout << "xInterpolate:ERROR: det(x.r)==0 ! \n";
            printf(" centerDerivative=(%e,%e,%e,%e)\n",CENTER_DERIVATIVE(ip,jp,i3,axis1,axis1),
		   CENTER_DERIVATIVE(ip,jp,i3,axis2,axis2),
		   CENTER_DERIVATIVE(ip,jp,i3,axis1,axis2),
		   CENTER_DERIVATIVE(ip,jp,i3,axis2,axis1));
	  }
          deti=1./deti;
	  dr=( CENTER_DERIVATIVE(ip,jp,i3,axis2,axis2)*dx-
               CENTER_DERIVATIVE(ip,jp,i3,axis1,axis2)*dy)*deti;
	  ds=(-CENTER_DERIVATIVE(ip,jp,i3,axis2,axis1)*dx+
               CENTER_DERIVATIVE(ip,jp,i3,axis1,axis1)*dy)*deti;
	}
      }

      dr*=(gridIndexRange(End,axis1)-gridIndexRange(Start,axis1));
      ds*=(gridIndexRange(End,axis2)-gridIndexRange(Start,axis2));
      dra=min(fabs(dr),1.);
      dsa=min(fabs(ds),1.);
      
      //...........only use 4 points if dra bigger than epsilon, this lets us
      //           interpolate near interpolation boundaries
      int ip1=ip;
      if( dra>epsi )
        ip1+= dr>0. ? 1 : -1;
      int jp1=jp;
      if( dsa>epsi )
        jp1+= ds>0. ? 1 : -1;
      // ........periodic wrap
      if( (bool)gc[grid].isPeriodic()(axis1) )
	if( fabs(dr)<1.5 )   // don't periodic wrap if we are a long way away
    	  ip1=MODR(ip1,axis1,grid);
      if( (bool)gc[grid].isPeriodic()(axis2) )
	if( fabs(ds)<1.5 )
          jp1=MODR(jp1,axis2,grid);
      
      //.............Unable to interpolate if outside the current grid, but
      //             extrapolate (to zero order) if this is the closest point
      //             so far
      if(ip1<indexRange(Start,axis1) || ip1>indexRange(End,axis1) ||
	 jp1<indexRange(Start,axis2) || jp1>indexRange(End,axis2) )
      {
        extrap=TRUE;
        if( distmn<dist || dist<0. )
	{
          dist=distmn;
          if(ip1<indexRange(Start,axis1) || ip1>indexRange(End,axis1))
	    ip1=ip;
          if(jp1<indexRange(Start,axis2) || jp1>indexRange(End,axis2))
	    jp1=jp;
	}
        else
         continue;    //  ....Unable to interpolate, try another grid
      }
      else
        extrap=FALSE;

      //  ... (check to see whether all marked interpolation points are valid)...
/* -- wdh 960203
      if( (int)gc[grid].isAllVertexCentered() )
        if(mask(ip ,jp)==0 || mask(ip ,jp1)==0  ||
           mask(ip1,jp)==0 || mask(ip1,jp1)==0 )
           continue ;  //       ....Unable to interpolate, try another grid
      else if( mask(ip ,jp)==0 )  // check this for cell centered
        continue;
---- */
      if(mask(ip ,jp)==0 || mask(ip ,jp1)==0  ||
	 mask(ip1,jp)==0 || mask(ip1,jp1)==0 )
	continue ;  //       ....Unable to interpolate, try another grid

      // ...........Bi-Linear Interpolation:
      for( int n0=componentsToInterpolate.getBase(0); n0<=componentsToInterpolate.getBound(0); n0++ )
      {
        int c0=componentsToInterpolate(n0);
        uInterpolated(ipt,n0)=                  // fix this for funny components *****
            (1.-dsa)*((1.-dra)*u[grid](ip,jp ,i3,c0)+dra*u[grid](ip1,jp ,i3,c0))
             +  dsa *((1.-dra)*u[grid](ip,jp1,i3,c0)+dra*u[grid](ip1,jp1,i3,c0));
      }
      // return the values used:
      indexGuess(0,ipt)=ip;
      indexGuess(1,ipt)=jp;
      indexGuess(3,ipt)=grid;   //  !extrap ? k: -k;
      
      if( extrap )
      {
	returnValue=-1;
      }
      else
      {
	returnValue=0;
        break;   // point has been successfully interpolated, try next point
      }
    }
  }
  return returnValue;
}

#undef NRM
#undef MODR
