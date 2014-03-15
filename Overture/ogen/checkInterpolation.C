#include "Overture.h"
#include "OGFunction.h"
#include "ParallelOverlappingGridInterpolator.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


int 
checkInterpolation( realCompositeGridFunction & u,  
                    real & maxError, 
                    OGFunction *exact=NULL, 
                    ParallelOverlappingGridInterpolator *pogi=NULL,
                    const aString & label = blankString )
// =======================================================================================
//  /Description:
//    Utility routine to check the interpolation of a grid function
// =======================================================================================
{
  const int myid = Communication_Manager::My_Process_Number;
  const int np=max(1,Communication_Manager::Number_Of_Processors);
  
  const int debug=0;

  CompositeGrid & cg = *u.getCompositeGrid();
  cg.update(MappedGrid::THEcenter);
  
  assert( exact!=NULL );
  OGFunction & e = *exact;

  const int nc = u.getComponentDimension(0); 
  const int dw = max(cg[0].discretizationWidth());
  const int orderOfAccuracyInSpace=dw-1;

  real t=0.;
  Index I1,I2,I3;
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    realMappedGridFunction & ug = u[grid];
    const intArray & mask = mg.mask();

    const realSerialArray & uLocal  =  ug.getLocalArrayWithGhostBoundaries();
    const realSerialArray & xLocal  =  mg.center().getLocalArrayWithGhostBoundaries();
    const intSerialArray & maskLocal = mask.getLocalArrayWithGhostBoundaries();

    real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
    const int uDim0=uLocal.getRawDataSize(0);
    const int uDim1=uLocal.getRawDataSize(1);
    const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

    real *xp = xLocal.Array_Descriptor.Array_View_Pointer3;
    const int xDim0=xLocal.getRawDataSize(0);
    const int xDim1=xLocal.getRawDataSize(1);
    const int xDim2=xLocal.getRawDataSize(2);
#undef X
#define X(i0,i1,i2,i3) xp[i0+xDim0*(i1+xDim1*(i2+xDim2*(i3)))]

    const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=maskLocal.getRawDataSize(0);
    const int maskDim1=maskLocal.getRawDataSize(1);
    const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

    // getIndex(mg.gridIndexRange(),I1,I2,I3);
    getIndex(mg.dimension(),I1,I2,I3);
    
    Index J1 = Range(max(I1.getBase(),uLocal.getBase(0)),min(I1.getBound(),uLocal.getBound(0)));
    Index J2 = Range(max(I2.getBase(),uLocal.getBase(1)),min(I2.getBound(),uLocal.getBound(1)));
    Index J3 = Range(max(I3.getBase(),uLocal.getBase(2)),min(I3.getBound(),uLocal.getBound(2)));
    int i1,i2,i3;
    if( mg.numberOfDimensions()==2 )
    {
      FOR_3D(i1,i2,i3,J1,J2,J3)
      {
	if( MASK(i1,i2,i3)>0. )
	{
	  real x0 = X(i1,i2,i3,0);
	  real y0 = X(i1,i2,i3,1);
	  for( int c=0; c<nc; c++ )
	    U(i1,i2,i3,c) =e(x0,y0,0.,c,t);
	}
	else
	{
	  for( int c=0; c<nc; c++ )
	    U(i1,i2,i3,c) =0.;
	}
	
      }
    }
    else
    {
      FOR_3D(i1,i2,i3,J1,J2,J3)
      {
	if( MASK(i1,i2,i3)>0. )
	{
	  real x0 = X(i1,i2,i3,0);
	  real y0 = X(i1,i2,i3,1);
	  real z0 = X(i1,i2,i3,2);
	  for( int c=0; c<nc; c++ )
	    U(i1,i2,i3,c) =e(x0,y0,z0,c,t);
	}
	else
	{
	  for( int c=0; c<nc; c++ )
	    U(i1,i2,i3,c) =0.;
	}
      }
    }
      
  } // end for grid
  

    // u.display("************u before interpolate");

  if( pogi!=NULL )
  {
    pogi->interpolate(u);

    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      u[grid].periodicUpdate(); // this IS needed  *wdh* 060306
    }
  }
  else
  {
    u.interpolate();
  }
    
  // u.display("************* u after interpolate");

  real err=0.;

  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    realMappedGridFunction & ug = u[grid];
    const intArray & mask = mg.mask();

    const realSerialArray & uLocal  =  ug.getLocalArrayWithGhostBoundaries();
    const realSerialArray & xLocal  =  mg.center().getLocalArrayWithGhostBoundaries();
    const intSerialArray & maskLocal = mask.getLocalArrayWithGhostBoundaries();

    real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
    const int uDim0=uLocal.getRawDataSize(0);
    const int uDim1=uLocal.getRawDataSize(1);
    const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

    real *xp = xLocal.Array_Descriptor.Array_View_Pointer3;
    const int xDim0=xLocal.getRawDataSize(0);
    const int xDim1=xLocal.getRawDataSize(1);
    const int xDim2=xLocal.getRawDataSize(2);
#undef X
#define X(i0,i1,i2,i3) xp[i0+xDim0*(i1+xDim1*(i2+xDim2*(i3)))]

    const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=maskLocal.getRawDataSize(0);
    const int maskDim1=maskLocal.getRawDataSize(1);
    const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

    getIndex(mg.gridIndexRange(),I1,I2,I3,1);

    const int ng=orderOfAccuracyInSpace/2;
    const int ng3 = mg.numberOfDimensions()==2 ? 0 : ng;
      
    Index J1 = Range(max(I1.getBase(),uLocal.getBase(0)+ng ),min(I1.getBound(),uLocal.getBound(0)-ng ));
    Index J2 = Range(max(I2.getBase(),uLocal.getBase(1)+ng ),min(I2.getBound(),uLocal.getBound(1)-ng ));
    Index J3 = Range(max(I3.getBase(),uLocal.getBase(2)+ng3),min(I3.getBound(),uLocal.getBound(2)-ng3));

    real gridErr=0.;
    int i1,i2,i3;
    if( mg.numberOfDimensions()==2 )
    {
      FOR_3D(i1,i2,i3,J1,J2,J3)
      {
	if( MASK(i1,i2,i3)<0. )
	{
	  real x0 = X(i1,i2,i3,0);
	  real y0 = X(i1,i2,i3,1);
	  for( int c=0; c<nc; c++ )
	  {
	    if( debug>0  )
	    {
	      printf(" grid=%i i=(%i,%i) u=%9.2e exact=%9.2e err=%8.2e\n",grid,i1,i2,U(i1,i2,i3,c),e(x0,y0,0.,c,t),
		     fabs(U(i1,i2,i3,c)-e(x0,y0,0.,c,t)));
	    }
	    gridErr=max(gridErr,fabs(U(i1,i2,i3,c)-e(x0,y0,0.,c,t)));
	  }
	}
	
      }
    }
    else
    {
      FOR_3D(i1,i2,i3,J1,J2,J3)
      {
	if( MASK(i1,i2,i3)<0. )
	{
	  real x0 = X(i1,i2,i3,0);
	  real y0 = X(i1,i2,i3,1);
	  real z0 = X(i1,i2,i3,2);
	  for( int c=0; c<nc; c++ )
	    gridErr=max(gridErr,fabs(U(i1,i2,i3,c)-e(x0,y0,z0,c,t)));
	}
      }
    }
    gridErr= ParallelUtility::getMaxValue(gridErr);     
    err=max(err,gridErr);
    aString gridName = cg[grid].getName();
    if( myid==0 )
    {
//      printf(" ---- grid=%i : maximum error = %8.2e (np=%i, %s) ---------\n",grid,gridErr,np,(const char*)label);
       printf(" ---- grid=%3i : maximum error = %8.2e (np=%i, grid=%s, %s) ---------\n",grid,gridErr,np,
              (const char*)gridName,(const char*)label);
    }
    
  } // end for grid
  
  err=ParallelUtility::getMaxValue(err);

  if( myid==0 ) printf(" ============    maximum error = %8.2e (np=%i, %i components, %i grids, dw=%i, %s)===============\n",
		      err,np,nc,cg.numberOfComponentGrids(),dw,(const char*)label);

  maxError=err;
  return 0;
}
