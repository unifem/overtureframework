#ifndef __MESH_QUALITY__
#define __MESH_QUALITY__

#include "GenericDataBase.h"
#include "OvertureTypes.h"
#include "UnstructuredMapping.h"
#include "CompositeGrid.h"
#include "CompositeGridFunction.h"
#include "GL_GraphicsInterface.h"
#include "ArraySimple.h"
#include "InterpolatePoints.h"

class MetricEvaluator {
public:
  virtual ArraySimpleFixed<real,3,3,1,1> computeMetric(const ArraySimpleFixed<real,3,1,1,1> &x)=0;
  virtual ArraySimpleFixed<real,3,3,1,1> computeMetric(int id)=0; 
  virtual ArraySimpleFixed<real,3,3,1,1> computeMetric(void *entity)=0; 
};

class IdentityMetricEvaluator : public MetricEvaluator {

public:
  IdentityMetricEvaluator() { }
  virtual ~IdentityMetricEvaluator() { }
  
  virtual ArraySimpleFixed<real,3,3,1,1> computeMetric(const ArraySimpleFixed<real,3,1,1,1> &x);
  virtual ArraySimpleFixed<real,3,3,1,1> computeMetric(int id); 
  virtual ArraySimpleFixed<real,3,3,1,1> computeMetric(void *entity); 

};

class MetricCGFunctionEvaluator : public MetricEvaluator {

public:
  MetricCGFunctionEvaluator(RealCompositeGridFunction *cgf_=0) : cgf(cgf_){ }
  virtual ~MetricCGFunctionEvaluator() { }
  
  virtual ArraySimpleFixed<real,3,3,1,1> computeMetric(const ArraySimpleFixed<real,3,1,1,1> &x);
  virtual ArraySimpleFixed<real,3,3,1,1> computeMetric(int id); 
  virtual ArraySimpleFixed<real,3,3,1,1> computeMetric(void *entity); 

private:
  RealCompositeGridFunction *cgf;

};


enum MeshQualityMetric {
  volumeMetric=0,
  shapeMetric,
  volumeShapeMetric,
  numberOfQualityMetrics
};

class MeshQualityMetrics
{

public:

  enum JacobianProperties {
    normSquared=0,
    determinant,
    conditionNum,
    numberOfProperties
  };

  MeshQualityMetrics();
  MeshQualityMetrics(UnstructuredMapping &umap_);

  const realArray &computeMetric(MeshQualityMetric metric);

  void setReferenceTransformation(MetricEvaluator *rt);

  void plot(GL_GraphicsInterface &gi);

  void outputHistogram(aString fileName="meshQuality.dat");

  const realArray & getJacobianProperties() { return jacobianProperties; }
  const realArray & getJacobians() { return jacobians; }

  // 2d triangle
  inline ArraySimpleFixed<real,2,2,1,1> computeJacobian(const ArraySimpleFixed<real, 2,1,1,1> &x0, 
							const ArraySimpleFixed<real,2,1,1,1> &xm,
							const ArraySimpleFixed<real,2,1,1,1> &xmp1,
							const ArraySimpleFixed<real,2,2,1,1> &T);

  // 2d quadrilateral
  inline ArraySimpleFixed<real,2,2,1,1> computeJacobian(const ArraySimpleFixed<real,2,1,1,1> &x0, 
							const ArraySimpleFixed<real,2,1,1,1> &x1,
							const ArraySimpleFixed<real,2,1,1,1> &x2,
							const ArraySimpleFixed<real,2,1,1,1> &x3,
							const ArraySimpleFixed<real,2,2,1,1> &T);

  // 3d tetrahedron
  inline ArraySimpleFixed<real,3,3,1,1> computeJacobian(const ArraySimpleFixed<real,3,1,1,1> &x0, 
							const ArraySimpleFixed<real,3,1,1,1> &x1,
							const ArraySimpleFixed<real,3,1,1,1> &x2,
							const ArraySimpleFixed<real,3,1,1,1> &x3,
							const ArraySimpleFixed<real,3,3,1,1> &T);

  // 3d pyramid
  inline ArraySimpleFixed<real,3,3,1,1> computeJacobian(const ArraySimpleFixed<real,3,1,1,1> &x0, 
							const ArraySimpleFixed<real,3,1,1,1> &x1,
							const ArraySimpleFixed<real,3,1,1,1> &x2,
							const ArraySimpleFixed<real,3,1,1,1> &x3,
							const ArraySimpleFixed<real,3,1,1,1> &x4,
							const ArraySimpleFixed<real,3,3,1,1> &T);

  // 3d hexahedron
  inline ArraySimpleFixed<real,3,3,1,1> computeJacobian(const ArraySimpleFixed<real,3,1,1,1> &x0, 
							const ArraySimpleFixed<real,3,1,1,1> &x1,
							const ArraySimpleFixed<real,3,1,1,1> &x2,
							const ArraySimpleFixed<real,3,1,1,1> &x3,
							const ArraySimpleFixed<real,3,1,1,1> &x4,
							const ArraySimpleFixed<real,3,1,1,1> &x5,
							const ArraySimpleFixed<real,3,1,1,1> &x6,
							const ArraySimpleFixed<real,3,1,1,1> &x7,
							const ArraySimpleFixed<real,3,3,1,1> &T);

  inline ArraySimpleFixed<real,2,2,1,1> computeWeight(const ArraySimpleFixed<real,2,1,1,1> &xc,
						      UnstructuredMapping::ElementType et);

  inline ArraySimpleFixed<real,3,3,1,1> computeWeight(const ArraySimpleFixed<real,3,1,1,1> &xc,
						      UnstructuredMapping::ElementType et);

  inline void computeJacobianProperties(real &N2, 
					real &det, 
					real &K, const ArraySimpleFixed<real,2,2,1,1> &J) const;

  inline void computeJacobianProperties(real &N2, 
					real &det, 
					real &K, const ArraySimpleFixed<real,3,3,1,1> &J) const;

  inline real jacobianNodeDerivative(UnstructuredMapping::ElementType, int n, int c) const;


protected:
  void computeJacobianProperties();
  
private:
  UnstructuredMapping *umap;
  MetricEvaluator *referenceTransformation;

  realArray jacobianProperties;
  realArray jacobians;
  realArray metrics[int(numberOfQualityMetrics)];

};


#if 0
// stupid sun compiler can't understand function templates with non-type parameters
template<int DIM>
void
interpolateFromControlFunction( const ArraySimpleFixed<real,DIM,1,1,1> &midPt, 
				ArraySimpleFixed<real,DIM,DIM,1,1> &T, 
				RealCompositeGridFunction &controlFunction )
{
 
  for ( int a1=0; a1<DIM; a1++ )
    for ( int a2=0; a2<DIM; a2++ )
      T(a1,a2) = 0.;

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  CompositeGrid &controlGrid = *controlFunction.getCompositeGrid();
  getIndex(controlGrid[0].gridIndexRange(),I1,I2,I3);

  RealArray controlBounds(2,3); 
  realArray & vertices = controlGrid[0].vertex();

  real dx[3];
  int ii[3];
  ii[2] = I3.getBase();
  real dxa[3];
  dx[2] = dxa[2] = 0.0;
  int axis;

  // compute indices into the control grid
  for ( axis=0; axis<DIM; axis++ )
    {
      controlBounds(0,axis) = vertices(I1.getBase(), I2.getBase(), I3.getBase(),axis);
      controlBounds(1,axis) = vertices(I1.getBound(), I2.getBound(), I3.getBound(), axis);
      dx[axis] = (controlBounds(1,axis) - controlBounds(0,axis))/real(Iv[axis].getLength()-1);
      ii[axis] = int( (midPt(axis)-controlBounds(0,axis))/dx[axis] );
    }

  // compute linear interpolation coefficients
  for ( axis=0; axis<DIM; axis++ )
    dxa[axis] = (midPt(axis) - vertices(ii[0],ii[1],ii[2],axis))/dx[axis];

  // compute the interpolation
  if ( DIM==2 )
    {
      for ( int ti=0; ti<DIM; ti++ )
	for ( int tj=0; tj<DIM; tj++ )
	  T(ti,tj) = (( 1.0 -dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1],ii[2],ti,tj) + 
					  (     dxa[0] ) * controlFunction[0](ii[0]+1,ii[1],ii[2],ti,tj) ) +
		      (      dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1]+1,ii[2],ti,tj) +
					  (     dxa[0] ) * controlFunction[0](ii[0]+1, ii[1]+1,ii[2],ti,tj) ) );
    }
  else
    {
      for ( int ti=0; ti<DIM; ti++ )
	for ( int tj=0; tj<DIM; tj++ )
	  T(ti,tj) = ( (1.0-dxa[2])*( (( 1.0 -dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1],ii[2],ti,tj) + 
							 (     dxa[0] ) * controlFunction[0](ii[0]+1,ii[1],ii[2],ti,tj) ) +
				     (      dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1]+1,ii[2],ti,tj) +
							 (     dxa[0] ) * controlFunction[0](ii[0]+1, ii[1]+1,ii[2],ti,tj) ) ) ) +
		       (    dxa[2])*( (( 1.0 -dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1],ii[2]+1,ti,tj) + 
							   (     dxa[0] ) * controlFunction[0](ii[0]+1,ii[1],ii[2]+1,ti,tj) ) +
				       (      dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1]+1,ii[2]+1,ti,tj) +
							   (     dxa[0] ) * controlFunction[0](ii[0]+1, ii[1]+1,ii[2]+1,ti,tj) ) ) ));

    }

}
#else
inline void
interpolateFromControlFunction( const ArraySimpleFixed<real,2,1,1,1> &midPt, 
				ArraySimpleFixed<real,2,2,1,1> &T, 
				RealCompositeGridFunction &controlFunction )
{
 
  int DIM=2;
  for ( int a1=0; a1<DIM; a1++ )
    for ( int a2=0; a2<DIM; a2++ )
      T(a1,a2) = 0.;

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  CompositeGrid &controlGrid = *controlFunction.getCompositeGrid();
  getIndex(controlGrid[0].gridIndexRange(),I1,I2,I3);

  RealArray controlBounds(2,3); 
  realArray & vertices = controlGrid[0].vertex();

  real dx[3];
  int ii[3];
  ii[2] = I3.getBase();
  real dxa[3];
  dx[2] = dxa[2] = 0.0;
  int axis;

  // compute indices into the control grid
  for ( axis=0; axis<DIM; axis++ )
    {
      controlBounds(0,axis) = vertices(I1.getBase(), I2.getBase(), I3.getBase(),axis);
      controlBounds(1,axis) = vertices(I1.getBound(), I2.getBound(), I3.getBound(), axis);
      dx[axis] = (controlBounds(1,axis) - controlBounds(0,axis))/real(Iv[axis].getLength()-1);
      ii[axis] = min(Iv[axis].getBound(),max(Iv[axis].getBase(),int( (midPt(axis)-controlBounds(0,axis))/dx[axis] ) ));
    }

  // compute linear interpolation coefficients
  for ( axis=0; axis<DIM; axis++ )
    dxa[axis] = (midPt(axis) - vertices(ii[0],ii[1],ii[2],axis))/dx[axis];

  for ( int ti=0; ti<DIM; ti++ )
    for ( int tj=0; tj<DIM; tj++ )
      T(ti,tj) = (( 1.0 -dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1],ii[2],ti,tj) + 
				      (     dxa[0] ) * controlFunction[0](ii[0]+1,ii[1],ii[2],ti,tj) ) +
		  (      dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1]+1,ii[2],ti,tj) +
				      (     dxa[0] ) * controlFunction[0](ii[0]+1, ii[1]+1,ii[2],ti,tj) ) );
}

inline void
interpolateFromControlFunction( const ArraySimpleFixed<real,3,1,1,1> &midPt, 
				ArraySimpleFixed<real,3,3,1,1> &T, 
				RealCompositeGridFunction &controlFunction )
{
 
  int DIM=3;
  for ( int a1=0; a1<DIM; a1++ )
    for ( int a2=0; a2<DIM; a2++ )
      T(a1,a2) = 0.;
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  CompositeGrid &controlGrid = *controlFunction.getCompositeGrid();
  getIndex(controlGrid[0].gridIndexRange(),I1,I2,I3);
  
  RealArray controlBounds(2,3); 
  realArray & vertices = controlGrid[0].vertex();
  
  real dx[3];
  int ii[3];
  ii[2] = I3.getBase();
  real dxa[3];
  dx[2] = dxa[2] = 0.0;
  int axis;

  // compute indices into the control grid
  for ( axis=0; axis<DIM; axis++ )
    {
      controlBounds(0,axis) = vertices(I1.getBase(), I2.getBase(), I3.getBase(),axis);
      controlBounds(1,axis) = vertices(I1.getBound(), I2.getBound(), I3.getBound(), axis);
      dx[axis] = (controlBounds(1,axis) - controlBounds(0,axis))/real(Iv[axis].getLength()-1);
      //      ii[axis] = int( (midPt(axis)-controlBounds(0,axis))/dx[axis] );
      ii[axis] = min(Iv[axis].getBound(),max(Iv[axis].getBase(),int( (midPt(axis)-controlBounds(0,axis))/dx[axis] ) ));
    }
  
  // compute linear interpolation coefficients
  for ( axis=0; axis<DIM; axis++ )
    dxa[axis] = (midPt(axis) - vertices(ii[0],ii[1],ii[2],axis))/dx[axis];
  
  
  for ( int ti=0; ti<DIM; ti++ )
    for ( int tj=0; tj<DIM; tj++ )
      T(ti,tj) = ( (1.0-dxa[2])*( (( 1.0 -dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1],ii[2],ti,tj) + 
						       (     dxa[0] ) * controlFunction[0](ii[0]+1,ii[1],ii[2],ti,tj) ) +
				   (      dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1]+1,ii[2],ti,tj) +
						       (     dxa[0] ) * controlFunction[0](ii[0]+1, ii[1]+1,ii[2],ti,tj) ) ) ) +
		   (    dxa[2])*( (( 1.0 -dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1],ii[2]+1,ti,tj) + 
						       (     dxa[0] ) * controlFunction[0](ii[0]+1,ii[1],ii[2]+1,ti,tj) ) +
				   (      dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1]+1,ii[2]+1,ti,tj) +
						       (     dxa[0] ) * controlFunction[0](ii[0]+1, ii[1]+1,ii[2]+1,ti,tj) ) ) ));
  
}
#endif


inline ArraySimpleFixed<real,2,2,1,1>
MeshQualityMetrics::
computeJacobian(const ArraySimpleFixed<real, 2,1,1,1> &x0, 
		const ArraySimpleFixed<real,2,1,1,1> &xm,
		const ArraySimpleFixed<real,2,1,1,1> &xmp1,
		const ArraySimpleFixed<real,2,2,1,1> &T)
{

  ArraySimpleFixed<real,2,2,1,1> J,Jt;

  Jt(0,0) = xm[0]-x0[0];
  Jt(0,1) = xmp1[0]-x0[0];

  Jt(1,0) = xm[1]-x0[1];
  Jt(1,1) = xmp1[1]-x0[1];

  int r,c,cc;
  for ( r=0; r<2; r++ )
    for ( c=0; c<2; c++ )
      J(r,c) = 0;

  for ( r=0; r<2; r++ )
    for ( c=0; c<2; c++ )
      for ( cc=0; cc<2; cc++ )
	J(r,c) += Jt(r,cc)*T(cc,c);

  return J;
}

inline ArraySimpleFixed<real,2,2,1,1>
MeshQualityMetrics::
computeJacobian(const ArraySimpleFixed<real,2,1,1,1> &x0, 
		const ArraySimpleFixed<real,2,1,1,1> &x1,
		const ArraySimpleFixed<real,2,1,1,1> &x2,
		const ArraySimpleFixed<real,2,1,1,1> &x3,
		const ArraySimpleFixed<real,2,2,1,1> &T)
{

  ArraySimpleFixed<real,2,2,1,1> J,Jt;

  Jt(0,0) = 0.5*(x1[0]+x2[0]-x3[0]-x0[0]);
  Jt(0,1) = 0.5*(x3[0]+x2[0]-x1[0]-x0[0]);
  Jt(1,0) = 0.5*(x1[1]+x2[1]-x3[1]-x0[1]);
  Jt(1,1) = 0.5*(x3[1]+x2[1]-x1[1]-x0[1]);

  int r,c,cc;
  for ( r=0; r<2; r++ )
    for ( c=0; c<2; c++ )
      J(r,c) = 0;

  for ( r=0; r<2; r++ )
    for ( c=0; c<2; c++ )
      for ( cc=0; cc<2; cc++ )
	J(r,c) += Jt(r,cc)*T(cc,c);

  return J;
}

inline ArraySimpleFixed<real,3,3,1,1>
MeshQualityMetrics::
computeJacobian(const ArraySimpleFixed<real,3,1,1,1> &x0, 
		const ArraySimpleFixed<real,3,1,1,1> &x1,
		const ArraySimpleFixed<real,3,1,1,1> &x2,
		const ArraySimpleFixed<real,3,1,1,1> &x3,
		const ArraySimpleFixed<real,3,3,1,1> &T)
{

  ArraySimpleFixed<real,3,3,1,1> J,Jt;

  Jt(0,0) = x1[0]-x0[0];
  Jt(0,1) = x2[0]-x0[0];
  Jt(0,2) = x3[0]-x0[0];

  Jt(1,0) = x1[1]-x0[1];
  Jt(1,1) = x2[1]-x0[1];
  Jt(1,2) = x3[1]-x0[1];

  Jt(2,0) = x1[2]-x0[2];
  Jt(2,1) = x2[2]-x0[2];
  Jt(2,2) = x3[2]-x0[2];

  int r,c,cc;
  for ( r=0; r<3; r++ )
    for ( c=0; c<3; c++ )
      J(r,c) = 0;

  for ( r=0; r<3; r++ )
    for ( c=0; c<3; c++ )
      for ( cc=0; cc<3; cc++ )
	J(r,c) += Jt(r,cc)*T(cc,c);

  return J;
}

inline ArraySimpleFixed<real,3,3,1,1>
MeshQualityMetrics::
computeJacobian(const ArraySimpleFixed<real,3,1,1,1> &x0, 
		const ArraySimpleFixed<real,3,1,1,1> &x1,
		const ArraySimpleFixed<real,3,1,1,1> &x2,
		const ArraySimpleFixed<real,3,1,1,1> &x3,
		const ArraySimpleFixed<real,3,1,1,1> &x4,
		const ArraySimpleFixed<real,3,3,1,1> &T)
{
  ArraySimpleFixed<real,3,3,1,1> J,Jt;

  Jt(0,0) = -( x0(0)+x3(0)+x4(0)-
	       (x1(0)+x2(0)+x4(0)))/3.;
  Jt(1,0) = -( x0(1)+x3(1)+x4(1)-
	       (x1(1)+x2(1)+x4(1)))/3.;
  Jt(2,0) = -( x0(2)+x3(2)+x4(2)-
	       (x1(2)+x2(2)+x4(2)))/3.;
  
  Jt(0,1) = -( x0(0)+x1(0)+x4(0)-
	       (x2(0)+x3(0)+x4(0)))/3.;
  Jt(1,1) = -( x0(1)+x1(1)+x4(1)-
	       (x2(1)+x3(1)+x4(1)))/3.;
  Jt(2,1) = -( x0(2)+x1(2)+x4(2)-
	       (x2(2)+x3(2)+x4(2)))/3.;
  
  Jt(0,2) = -2*( (x0(0)+x1(0)+
		  x2(0)+x3(0))/4.-
		 (x4(0)));
  Jt(1,2) = -2*( (x0(1)+x1(1)+x2(1)+
		  x3(1))/4.-
		 (x4(1)));
  Jt(2,2) = -2*( (x0(2)+x1(2)+x2(2)+
		  x3(2))/4.-
		 (x4(2)));

  int r,c,cc;
  for ( r=0; r<3; r++ )
    for ( c=0; c<3; c++ )
      J(r,c) = 0;

  for ( r=0; r<3; r++ )
    for ( c=0; c<3; c++ )
      for ( cc=0; cc<3; cc++ )
	J(r,c) += Jt(r,cc)*T(cc,c);

  return J;
}

inline ArraySimpleFixed<real,3,3,1,1>
MeshQualityMetrics::
computeJacobian(const ArraySimpleFixed<real,3,1,1,1> &x0, 
		const ArraySimpleFixed<real,3,1,1,1> &x1,
		const ArraySimpleFixed<real,3,1,1,1> &x2,
		const ArraySimpleFixed<real,3,1,1,1> &x3,
		const ArraySimpleFixed<real,3,1,1,1> &x4,
		const ArraySimpleFixed<real,3,1,1,1> &x5,
		const ArraySimpleFixed<real,3,1,1,1> &x6,
		const ArraySimpleFixed<real,3,1,1,1> &x7,
		const ArraySimpleFixed<real,3,3,1,1> &T)
{
  ArraySimpleFixed<real,3,3,1,1> J,Jt;

  Jt(0,0) = -( x0(0)+x3(0)+x4(0)+x7(0)-
	       (x1(0)+x2(0)+x5(0)+x6(0)))/4.;
  Jt(1,0) = -( x0(1)+x3(1)+x4(1)+x7(1)-
	       (x1(1)+x2(1)+x5(1)+x6(1)))/4.;
  Jt(2,0) = -( x0(2)+x3(2)+x4(2)+x7(2)-
	       (x1(2)+x2(2)+x5(2)+x6(2)))/4.;
  
  Jt(0,1) = -( x0(0)+x1(0)+x4(0)+x5(0)-
	       (x2(0)+x3(0)+x6(0)+x7(0)))/4.;
  Jt(1,1) = -( x0(1)+x1(1)+x4(1)+x5(1)-
	       (x2(1)+x3(1)+x6(1)+x7(1)))/4.;
  Jt(2,1) = -( x0(2)+x1(2)+x4(2)+x5(2)-
	       (x2(2)+x3(2)+x6(2)+x7(2)))/4.;
  
  Jt(0,2) = -( x0(0)+x1(0)+x2(0)+x3(0)-
	       (x4(0)+x5(0)+x6(0)+x7(0)))/4.;
  Jt(1,2) = -( x0(1)+x1(1)+x2(1)+x3(1)-
	       (x4(1)+x5(1)+x6(1)+x7(1)))/4.;
  Jt(2,2) = -( x0(2)+x1(2)+x2(2)+x3(2)-
	       (x4(2)+x5(2)+x6(2)+x7(2)))/4.;

  int r,c,cc;
  for ( r=0; r<3; r++ )
    for ( c=0; c<3; c++ )
      J(r,c) = 0;
  
  for ( r=0; r<3; r++ )
    for ( c=0; c<3; c++ )
      for ( cc=0; cc<3; cc++ )
	J(r,c) += Jt(r,cc)*T(cc,c);
  
  return J;
}


inline void 
MeshQualityMetrics::
computeJacobianProperties(real &N2, 
			  real &det, 
			  real &K, const ArraySimpleFixed<real,2,2,1,1> &J) const
{
  int r,c;
  N2 = det = K = 0.;

  for ( r=0; r<2; r++ )
    for ( c=0; c<2; c++ )
      N2 += J(r,c)*J(r,c);

  det = J(0,0)*J(1,1) - J(1,0)*J(0,1);

  if ( det > 0.0 )
    {
      real normOfInverse = 0.;
      
      normOfInverse += J(1,1)*J(1,1);
      normOfInverse += J(1,0)*J(1,0);
      normOfInverse += J(0,1)*J(0,1);
      normOfInverse += J(0,0)*J(0,0);

      normOfInverse = sqrt(normOfInverse)/det;
	      
      K = sqrt(N2)*normOfInverse;
    
    }
  else
    K = REAL_MAX;

}

inline void 
MeshQualityMetrics::
computeJacobianProperties(real &N2, 
			  real &det, 
			  real &K, const ArraySimpleFixed<real,3,3,1,1> &J) const
{
  int r,c;
  N2 = det = K = 0.;

  for ( r=0; r<3; r++ )
    for ( c=0; c<3; c++ )
      N2 += J(r,c)*J(r,c);
  
  det = 
    J(0,0)*(J(1,1)*J(2,2)-J(1,2)*J(2,1)) - 
    J(0,1)*(J(1,0)*J(2,2)-J(1,2)*J(2,0)) +
    J(0,2)*(J(1,0)*J(2,1)-J(1,1)*J(2,0));

  if ( det>0.0 )
    {
      real normOfInverse = 0.;
      
      normOfInverse += (J(1,1)*J(2,2)-J(1,2)*J(2,1))*
	(J(1,1)*J(2,2)-J(1,2)*J(2,1));
      normOfInverse += (J(1,0)*J(2,2)-J(1,2)*J(2,0))*
	(J(1,0)*J(2,2)-J(1,2)*J(2,0));
      normOfInverse += (J(1,0)*J(2,1)-J(1,1)*J(2,0))*
	(J(1,0)*J(2,1)-J(1,1)*J(2,0));
      normOfInverse += (J(0,1)*J(2,2)-J(0,2)*J(2,1))*
	(J(0,1)*J(2,2)-J(0,2)*J(2,1));
      normOfInverse += (J(0,0)*J(2,2)-J(0,2)*J(2,0))*
	(J(0,0)*J(2,2)-J(0,2)*J(2,0));
      normOfInverse += (J(0,0)*J(2,1)-J(0,1)*J(2,0))*
	(J(0,0)*J(2,1)-J(0,1)*J(2,0));
      normOfInverse += (J(0,1)*J(1,2)-J(0,2)*J(1,1))*
	(J(0,1)*J(1,2)-J(0,2)*J(1,1));
      normOfInverse += (J(0,0)*J(1,2)-J(0,2)*J(1,0))*
	(J(0,0)*J(1,2)-J(0,2)*J(1,0));
      normOfInverse += (J(0,0)*J(1,1)-J(0,1)*J(1,0))*
	(J(0,0)*J(1,1)-J(0,1)*J(1,0));
      
      normOfInverse = sqrt(normOfInverse)/det;
      
      K = sqrt(N2)*normOfInverse;
    }
  else
    K = REAL_MAX;

}

inline ArraySimpleFixed<real,2,2,1,1> 
MeshQualityMetrics::
computeWeight(const ArraySimpleFixed<real, 2,1,1,1> &xc_, 
	      UnstructuredMapping::ElementType et)
{
  //realArray xc(1,2), T(2,2);
  ArraySimpleFixed<real,2,2,1,1> T;
  
  T(0,0) = T(1,1) = 1.;
  T(0,1) = T(1,0) = 0.;

  if ( referenceTransformation!=NULL )
    {
      ArraySimpleFixed<real,3,1,1,1> x3d;
      ArraySimpleFixed<real,3,3,1,1> T3d;
      x3d=0;
      T3d=0;
      x3d[0] = xc_[0];
      x3d[1] = xc_[1];
      T3d = referenceTransformation->computeMetric(x3d);
      T(0,0) = T3d(0,0);
      T(1,0) = T3d(1,0);
      T(1,1) = T3d(1,1);
      T(0,1) = T3d(0,1);
    }

  //interpolateFromControlFunction(xc_, T, *referenceTransformation);

  ArraySimpleFixed<real,2,2,1,1> Winv;
  if ( et==UnstructuredMapping::triangle )
    {
      Winv(0,0) = T(0,0) - T(1,0)/sqrt(3.);
      Winv(0,1) = T(0,1) - T(1,1)/sqrt(3.);
      Winv(1,0) = 2*T(1,0)/sqrt(3.);
      Winv(1,1) = 2*T(1,1)/sqrt(3.);
    }
  else
    for ( int r=0; r<2; r++ )
      for ( int c=0; c<2; c++ )
	Winv(r,c) = T(r,c);

  return Winv;
}

inline ArraySimpleFixed<real,3,3,1,1> 
MeshQualityMetrics::
computeWeight(const ArraySimpleFixed<real, 3,1,1,1> &xc_, 
	      UnstructuredMapping::ElementType et)
{
  ArraySimpleFixed<real,3,3,1,1> T;
  
  T(0,0) = T(1,1) = T(2,2) = 1.;
  T(0,1) = T(1,0) = T(0,2) = T(1,2) = T(2,1) = T(2,0) = 0.;
  
  if ( referenceTransformation!=NULL )
    T = referenceTransformation->computeMetric(xc_);
  //    interpolateFromControlFunction(xc_, T, *referenceTransformation);

  ArraySimpleFixed<real,3,3,1,1> Winv;
  if ( et==UnstructuredMapping::tetrahedron )
    {
      Winv(0,0) = T(0,0) - T(1,0)/sqrt(3.) - T(2,0)/sqrt(3.)/sqrt(2.);
      Winv(0,1) = T(0,1) - T(1,1)/sqrt(3.) - T(2,1)/sqrt(3.)/sqrt(2.);
      Winv(0,2) = T(0,2) - T(1,2)/sqrt(3.) - T(2,2)/sqrt(3.)/sqrt(2.);
      
      Winv(1,0) = 2*T(1,0)/sqrt(3.) - T(2,0)/sqrt(3.)/sqrt(2.);
      Winv(1,1) = 2*T(1,1)/sqrt(3.) - T(2,1)/sqrt(3.)/sqrt(2.);
      Winv(1,2) = 2*T(1,2)/sqrt(3.) - T(2,2)/sqrt(3.)/sqrt(2.);
      
      Winv(2,0) = sqrt(3.)*T(2,0)/sqrt(2.);
      Winv(2,1) = sqrt(3.)*T(2,1)/sqrt(2.);
      Winv(2,2) = sqrt(3.)*T(2,2)/sqrt(2.);
    }
  else if ( et==UnstructuredMapping::pyramid )
    {
      Winv(0,0) = T(0,0)*3./2;
      Winv(0,1) = T(0,1)*3./2;
      Winv(0,2) = T(0,2);

      Winv(1,0) = T(1,0)*3./2;
      Winv(1,1) = T(1,1)*3./2;
      Winv(1,2) = T(1,2)/2.;
      
      Winv(2,0) = T(2,0)*3./2;
      Winv(2,1) = T(2,1)*3./2;
      Winv(2,2) = T(2,2);
    }
  else
    for ( int r=0; r<3; r++ )
      for ( int c=0; c<3; c++ )
	Winv(r,c) = T(r,c);

  return Winv;
}

inline 
real 
MeshQualityMetrics::
jacobianNodeDerivative(UnstructuredMapping::ElementType e, int n, int c) const
{
  real res = -1;

  switch(e) {
    
  case UnstructuredMapping::triangle : case UnstructuredMapping::tetrahedron :
    if ( n==0 )
      res = -1;
    else if ( n==(c+1) )
      res = 1;
    else
      res = 0;
    break;

  case UnstructuredMapping::quadrilateral :
    switch(c) {
    case(0):
      if ( n==0 || n==3 )
	res=-.5;
      else
	res= .5;
      break;
    case(1):
      if ( n==0 || n==1 )
	res=-.5;
      else
	res= .5;
      break;
    }
    break;

  case UnstructuredMapping::hexahedron :
    switch(c) {
    case(0):
      if ( n==0 || n==3 || n==4 || n==7 )
	res = -.25;
      else
	res = .25;
      break;
    case(1):
      if ( n==0 || n==1 || n==4 || n==5 )
	res = -.25;
      else
	res = .25;
      break;
    case(2):
      if ( n==0 || n==1 || n==2 || n==3 )
	res = -.25;
      else
	res = .25;
      break;
    default:
      break;
    }
    break;
  case UnstructuredMapping::pyramid :
    switch(c) {
    case(0):
      if ( n==0 || n==3 || n==4 )
	res = -1./3;
      else
	res = 1./3.;
      break;
    case(1):
      if ( n==0 || n==1 || n==4 )
	res = -1./3;
      else
	res = 1./3.;
      break;
    case(2):
      if ( n==4 )
	res = 2.;
      else
	res = -.5;
      break;
    }
    break;
    
  default:
    res = -1;
    break;
  }

  return res;
}

#endif
