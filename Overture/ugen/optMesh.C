//#define BOUNDS_CHECK
//#define OV_DEBUG

//kkc 081124 #include <iostream.h>
#include <iostream>

#include "mathutil.h"
#include "Overture.h"
#include "UnstructuredMapping.h"
#include "MeshQuality.h"
#include "ArraySimple.h"
#include "Geom.h"

using namespace std;

void optimize(UnstructuredMapping &umap, RealCompositeGridFunction *cf);

//#define NEWTON 
#define STEEPEST

bool useCorners = true;
static bool equilateralIdeal = true;

enum objEnum {
  optShape,
  optSize,
  optShapeSize,
  optShapePlusSize
};

objEnum objFunc = optShape;

static ArraySimpleFixed<real,3,3,1,1> Tident() {
  ArraySimpleFixed<real,3,3,1,1> T;
  T=0;
  T(0,0)=T(1,1)=T(2,2)=1;
  return T; // *wdh* 030915
}

#ifdef KKC_OPTMAIN
int main(int argc, char *argv[])
{
  Overture::start(argc,argv);

  aString fileName;

  if ( argc!=2 )
    {
      cout<<"usage : \n"<<argv[0]<<" mesh.msh"<<endl;
      exit(1);
    }
  else
    {
      fileName = argv[1];
    }

  UnstructuredMapping umap;
  umap.get(fileName);
  MeshQualityMetrics mq(umap);

  GenericGraphicsInterface &gi = *(Overture::getGraphicsInterface());
  GraphicsParameters gp;
  gp.set(GI_PLOT_UNS_EDGES, true);  
  gp.set(GI_PLOT_UNS_FACES, true);


  //gi.plot(umap,gp);
  //mq.plot((GL_GraphicsInterface &)gi);

//   PlotIt::plot(gi,umap);

  int nSteps = 3;
  int dp = 1;

  cout<<"Initial quality :"<<endl;
  mq.outputHistogram();
//   mq.plot((GL_GraphicsInterface &)gi);

//   gi.erase();
  for ( int s=0; s<nSteps; s++ )
    {
      optimize(umap,0);
      
      if ((s%dp)==0) {
	mq.outputHistogram();
	//mq.plot((GL_GraphicsInterface &)gi);
      }
    }

//   gi.erase();
//   PlotIt::plot(gi,umap);
  
  umap.put(fileName+"opt.msh");
  Overture::finish();

  return 0;
}

#endif

#ifdef KKC_OLD
static real 
linesearch( ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
	    realArray &nodes, ArraySimpleFixed<real,3,3,1,1> &T, real d_max, ArraySimpleFixed<real,3,1,1,1> &dir,
	    real (*objective_evaluation)(ArraySimple<int> &, ArraySimple<int> &, 
					 realArray & , ArraySimpleFixed<real,3,3,1,1> & ) )
#endif

static real 
linesearch( ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
	    realArray &nodes, MeshQualityMetrics &mq, real d_max, ArraySimpleFixed<real,3,1,1,1> &dir,
	    real (*objective_evaluation)(ArraySimple<int> &, ArraySimple<int> &, 
					 realArray & , MeshQualityMetrics & ) )
{
  ArraySimpleFixed<real,3,3,1,1> T; 
  T = Tident();

  int rDim = nodes.getLength(1);

  real eps = 1e-3; // change in step size tolerance for ending line search
  real alpha = .5; // fraction of d_max to use as initial step length guess
  int nStepMax = 10; // max number of attempts to find a step size

  bool searchDone = false;
  
  real d_left=0, d_right=alpha*d_max;;
  real obj_left, obj_right;

  ArraySimpleFixed<real,3,1,1,1> original_node;
  original_node[2] = 0;
  for ( int a=0; a<rDim; a++ ) original_node[a] = nodes(elements(0,node_in_element(0)),a);

  obj_left = objective_evaluation(node_in_element, elements, nodes, mq);

  for ( int a=0; a<rDim; a++ ) nodes(elements(0,node_in_element(0)),a) = original_node[a] + d_right*dir[a];
  obj_right = objective_evaluation(node_in_element, elements, nodes, mq);

  int step=0;
  real d_use = 0.; // actual step size to take

  //   cout<<"INITIALLY : "<<"d_left "<<d_left<<" obj_left "<<obj_left<<" d_right "<<d_right<<" obj_right "<<obj_right<<endl;

  while(!searchDone)
    {
      step++;
      ArraySimpleFixed<real,3,1,1,1> mid;
      mid = 0.;
      for ( int a=0; a<rDim; a++ ) mid[a] = original_node[a] + 0.5*(d_left+d_right)*dir[a];

      for ( int a=0; a<rDim; a++ ) nodes(elements(0,node_in_element(0)),a) = mid[a];
      real new_obj = objective_evaluation(node_in_element, elements, nodes, mq);


      // use a quadratic approximation to the objective with left,mid and right if
      //    the function appears positive using an undivided difference 
      //    ( d2o/d2x = obj_left -2*new_obj + obj_right ) and new_obj is feasible.
      //    set the right to the smaller of ether the midpoint or the quadratic approximation
      //       if the step size is greater than 0 and the new objective value is less than the right.
      //    if the new objective value is greater than the right, but less than the left we have
      //       probably passed over the minimum, set the left to the computed value.
      //  if it does not appear positive, then bisect the step size and set the right to the midpoint
      //    find the step size, stop if nstep>nStepMax or (d_right-d_left)/d_max<eps

      if ( obj_right<REAL_MAX && new_obj<REAL_MAX && (obj_left - 2*new_obj + obj_right)>0. )
	{

	  //	  cout<<"yep, its positive "<<(obj_left - 2*new_obj + obj_right)<<endl;
	  real d_mid = 0.5*(d_left+d_right);
	  d_use = -0.5*( (d_mid+d_right)*(obj_left-obj_right) - (d_left+d_right)*(new_obj-obj_right) )/(new_obj-obj_left);

	  for ( int a=0; a<rDim; a++ ) nodes(elements(0,node_in_element(0)),a) = original_node[a] + d_use*dir[a];
	  real obj_q = objective_evaluation(node_in_element, elements, nodes, mq);

	  //	  cout<<"obj_q "<<obj_q<<endl;

	  if ( obj_q<new_obj && obj_q<obj_right && d_use>0. )
	    { // if the objective computed from the quadratic is less than the right,
	      //   reset the right to this new value (only if the step length remains positive)
	      d_right = d_use;
	      obj_right = obj_q;
	    }
	  else if ( new_obj<obj_right && d_use>0. )
	    {
	      // choose the midpoint if it works better than the computed location
	      d_right = d_mid;
	      obj_right = new_obj;
	    }
	  else if ( obj_q>obj_right && obj_q<obj_left && d_use<d_right && d_use>0. )
	    {
	      // however, if the objective at the computed location (with d_use in [d_left,d_right] )is greater
	      //   than the right but still less than the left, we have passed over
	      //   the minimum.  set the left to the computed value and keep the current right.
	      obj_left = obj_q;
	      d_left = d_use;
	    }
	  else
	    {
	      // no idea where to go next, guess something smaller than the right side.
	      d_right *=alpha;
	      for ( int a=0; a<rDim; a++ ) nodes(elements(0,node_in_element(0)),a) = original_node[a] + d_right*dir[a];
	      obj_right = objective_evaluation(node_in_element, elements, nodes, mq);
	    }
	}
      else
	{
	  // move to the midpoint since the objective function was rediculous or not positive
	  d_right = 0.5*(d_left+d_right);
	  obj_right = new_obj;
	}
      
//         cout<<"d_left "<<d_left<<" obj_left "<<obj_left<<" d_use "<<d_use<<
//   	" obj_new "<<new_obj<<" d_right "<<d_right<<" obj_right "<<obj_right<<endl;

      d_use = obj_left<obj_right ? d_left : d_right;

      searchDone = searchDone || (fabs(d_right-d_left)/d_max < eps || step>nStepMax || fabs(obj_left-obj_right)/min(obj_left,obj_right)<(eps));
    }

  //  cout<<"determined step size "<<d_use<<endl;
  //cout<<"search used "<<step<<" steps, objective functions were "<<obj_left<<"  "<<obj_right<<" d_l, d_r were "<<d_left<<"  "<<d_right<<endl;
  return d_use;
}

static ArraySimpleFixed<real,3,3,1,1> 
dk2dj(ArraySimpleFixed<real,3,3,1,1> &jac, real norm2, real k, real det, int rDim)
{
  int r,c;
  ArraySimpleFixed<real,3,3,1,1> dfda, ata,aata,jtinv;
  
  for ( r=0; r<rDim; r++ )
    for ( c=0; c<rDim; c++ ) 
      {
	dfda(r,c) = 0.;
	ata(r,c) = 0.;
	aata(r,c) = 0.;
      }
  
  real invnorm2 = k*k/norm2;

  if ( rDim==2 )
    {
      jtinv(0,0) = jac(1,1)/det;
      jtinv(1,1) = jac(0,0)/det;
      jtinv(0,1) = -jac(1,0)/det;
      jtinv(1,0) = -jac(0,1)/det;  
    }
  else
    {
      jtinv(0,0) = ( jac(1,1)*jac(2,2)-jac(2,1)*jac(1,2) )/det;
      jtinv(0,1) =-( jac(1,0)*jac(2,2)-jac(2,0)*jac(1,2) )/det;
      jtinv(0,2) = ( jac(1,0)*jac(2,1)-jac(2,0)*jac(1,1) )/det;
      
      jtinv(1,0) =-( jac(0,1)*jac(2,2)-jac(2,1)*jac(0,2) )/det;
      jtinv(1,1) = ( jac(0,0)*jac(2,2)-jac(2,0)*jac(0,2) )/det;
      jtinv(1,2) =-( jac(0,0)*jac(2,1)-jac(2,0)*jac(0,1) )/det;
      
      jtinv(2,0) = ( jac(0,1)*jac(1,2)-jac(1,1)*jac(0,2) )/det;
      jtinv(2,1) =-( jac(0,0)*jac(1,2)-jac(1,0)*jac(0,2) )/det;
      jtinv(2,2) = ( jac(0,0)*jac(1,1)-jac(0,1)*jac(1,0) )/det;
    }

  for ( r=0; r<rDim; r++ )
    for ( c=0; c<rDim; c++ ) 
      for ( int cc=0; cc<rDim; cc++ )
	ata(r,c) += jac(cc,r)*jac(cc,c);
	  
  for ( r=0; r<rDim; r++ )
    for ( c=0; c<rDim; c++ ) 
      for ( int cc=0; cc<rDim; cc++ )
	aata(r,c) += jac(r,cc)*ata(cc,c);

  //  cout<<invnorm2<<" "<<norm2<<" "<<det<<endl;
  for ( r=0; r<rDim; r++ )
    for ( c=0; c<rDim; c++ ) 
      {
	dfda(r,c) = 2*jac(r,c)*( invnorm2 + norm2*norm2/det/det ) -
	  2*norm2*aata(r,c)/det/det - 2*norm2*invnorm2*jtinv(r,c);
      }
  
  return dfda;
}

static ArraySimpleFixed<real,3,3,1,1>
dd2dj(ArraySimpleFixed<real,3,3,1,1> &jac, real norm2, real k, real det, int rDim)
{
  int r,c;
  ArraySimpleFixed<real,3,3,1,1> dfda, jtinv;
  
  for ( r=0; r<rDim; r++ )
    for ( c=0; c<rDim; c++ ) 
      {
	dfda(r,c) = 0.;
      }
  
  real invnorm2 = k*k/norm2;

  if ( rDim==2 )
    {
      jtinv(0,0) = jac(1,1)/det;
      jtinv(1,1) = jac(0,0)/det;
      jtinv(0,1) = -jac(1,0)/det;
      jtinv(1,0) = -jac(0,1)/det;  
    }
  else
    {
      jtinv(0,0) = ( jac(1,1)*jac(2,2)-jac(2,1)*jac(1,2) )/det;
      jtinv(0,1) =-( jac(1,0)*jac(2,2)-jac(2,0)*jac(1,2) )/det;
      jtinv(0,2) = ( jac(1,0)*jac(2,1)-jac(2,0)*jac(1,1) )/det;
      
      jtinv(1,0) =-( jac(0,1)*jac(2,2)-jac(2,1)*jac(0,2) )/det;
      jtinv(1,1) = ( jac(0,0)*jac(2,2)-jac(2,0)*jac(0,2) )/det;
      jtinv(1,2) =-( jac(0,0)*jac(2,1)-jac(2,0)*jac(0,1) )/det;
      
      jtinv(2,0) = ( jac(0,1)*jac(1,2)-jac(1,1)*jac(0,2) )/det;
      jtinv(2,1) =-( jac(0,0)*jac(1,2)-jac(1,0)*jac(0,2) )/det;
      jtinv(2,2) = ( jac(0,0)*jac(1,1)-jac(0,1)*jac(1,0) )/det;
    }

  real fac = useCorners ? (rDim==2 ? 4 : 8) : 1;
  //  real fac=1;
  if ( det>1/det )
    {
      for ( r=0; r<rDim; r++ )
	for ( c=0; c<rDim; c++ ) 
	  dfda(r,c) = fac*2*det*jtinv(r,c);
    }
  else
    {
      for ( r=0; r<rDim; r++ )
	for ( c=0; c<rDim; c++ ) 
	  dfda(r,c) = -2*jtinv(r,c)/det/fac;
    }

  return dfda;
}

#ifdef OLD
static ArraySimpleFixed<real,3,1,1,1> 
grad_analytic_k2(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
		 realArray & nodes, ArraySimpleFixed<real,3,3,1,1> & T, real & obj )
#endif

static ArraySimpleFixed<real,3,1,1,1> 
grad_analytic_k2(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
		 realArray & nodes, MeshQualityMetrics &mq, real & obj )
{
  ArraySimpleFixed<real,3,3,1,1> T; 
  T = Tident();

  int nE = elements.size(0);
  int rDim = nodes.getLength(1);
  //  int dDim = elements.size(1) <= 4 ? 2 : 3;

  ArraySimpleFixed<real,3,1,1,1> grad;
  ArraySimpleFixed<real,3,3,1,1> dfda;

  grad = 0.;
  obj = 0;

  real norm2,k,det;
  UnstructuredMapping::ElementType etype;
  
  ArraySimpleFixed<real,3,3,1,1> dodj,jac;
  dodj = 0;
  
  if ( rDim==2 )
    {
      for ( int e=0; e<nE; e++ )
	{
	  ArraySimpleFixed<real,2,1,1,1> point[4],cent;
	  ArraySimpleFixed<real,2,2,1,1> jac2,jacs,T2,I;
	  ArraySimpleFixed<real,2,2,1,1> W;

	  I = 0;
	  I(1,1) = I(0,0)  = 1;

	  for ( int i=0; i<rDim; i++ )
	    for ( int j=0; j<rDim; j++ )
	      T2(i,j) = T(i,j);

	  ArraySimpleFixed<real,2,1,1,1> xc;
	  xc[0] = nodes(elements(0,node_in_element(0)),0);
	  xc[1] = nodes(elements(0,node_in_element(0)),1);

	  int n=0; 
	      
	  cent=0;
	  while( elements(e,n)!=-1 && n<4 )
	    {

	      for ( int a=0; a<rDim; a++ ) 
		{
		  point[n][a] = nodes(elements(e,n),a);
		  cent[a] += point[n][a];
		}
	      //	      for ( int a=0; a<rDim; a++ ) point[n][a] = nodes(elements(e,n),a);
	      n++;
	    }
	  
	  for ( int a=0; a<rDim; a++ )
	    cent[a]/=real(n);

	  if ( n==3 )
	    {
		  
	      etype = UnstructuredMapping::triangle;
	      ArraySimpleFixed<real,2,1,1,1> dum;

	      if ( equilateralIdeal ) 
		W = mq.computeWeight(cent, etype);
	      else
		W = mq.computeWeight(cent, UnstructuredMapping::quadrilateral);
		  
	      jac2 = mq.computeJacobian(point[0],point[1],point[2],I);
	      jacs = mq.computeJacobian(point[0],point[1],point[2],W);
	    }
	  else
	    {
	      etype = UnstructuredMapping::quadrilateral;
	      T2 = mq.computeWeight(cent,etype);
	      W = T2;

	      if ( useCorners )
		{
		  ArraySimpleFixed<real,3,3,1,1> cent;
		  cent[0] = ( point[0][0] + point[1][0] + point[2][0] + point[3][0] )/4.;
		  cent[1] = ( point[0][1] + point[1][1] + point[2][1] + point[3][1] )/4.;
		  
		  int ne=node_in_element(e);
		  int np1 = (ne+1)%n;
		  int nm1 = (ne+n-1)%n;
		  for ( int a=0; a<rDim; a++ )
		    {
		      point[0][a]=nodes(elements(e,ne),a);
		      point[1][a]=(nodes(elements(e,np1),a)+point[0][a])/2.;
		      point[2][a]=cent[a];
		      point[3][a]=(nodes(elements(e,nm1),a)+point[0][a])/2.;
		    }
		}
	      
	      //	      jac2 = mq.computeJacobian(point[0],point[1],point[2],point[3],T2);
	      jac2 = mq.computeJacobian(point[0],point[1],point[2],point[3],I);
	      jacs = mq.computeJacobian(point[0],point[1],point[2],point[3],W);
	    }
	  
	  for ( int i=0; i<rDim; i++ )
	    for ( int j=0; j<rDim; j++ )
	      T(i,j)=T2(i,j);

	  mq.computeJacobianProperties(norm2,det,k,jacs);
	  
	  for ( int i=0; i<rDim; i++ )
	    for ( int j=0; j<rDim; j++ )
	      jac(i,j) = jacs(i,j);
	
	  // compute the derivative of the condition number squared by the jacobian
	  //	  dodj = dk2dj(jac,norm2,k,det,rDim);
	  if (objFunc==optShape)
	    dodj = dk2dj(jac,norm2,k,det,rDim);
	  else if ( objFunc==optSize )
	    dodj = dd2dj(jac,norm2,k,det,rDim);
	  else if ( objFunc==optShapeSize )
	    {
	      ArraySimpleFixed<real,3,3,1,1> d1,d2;
	      d1 = dk2dj(jac,norm2,k,det,rDim);
	      d2 = dd2dj(jac,norm2,k,det,rDim);
	      for ( int r=0; r<rDim; r++ )
		for ( int c=0; c<rDim; c++ )
		  dodj(r,c) = k*k*d2(r,c) + max(det*det,1/(det*det))*d1(r,c);
	    }
	  else if ( objFunc==optShapePlusSize )
	    {
	      ArraySimpleFixed<real,3,3,1,1> d1,d2;
	      d1 = dk2dj(jac,norm2,k,det,rDim);
	      d2 = dd2dj(jac,norm2,k,det,rDim);
	      for ( int r=0; r<rDim; r++ )
		for ( int c=0; c<rDim; c++ )
		  dodj(r,c) = d1(r,c) + d2(r,c);
	    }

	      
	  if ( k!=REAL_MAX )
	    obj+=k*k;
	  else
	    obj=REAL_MAX;

#if 0
	  if ( obj<.1*REAL_MAX )
	    if ( useCorners && (etype==UnstructuredMapping::quadrilateral) )
	      for ( int r=0; r<rDim; r++ )
		for ( int c=0; c<rDim; c++ )
		  grad[r] += dodj(r,c)*mq.jacobianNodeDerivative(etype,0,c);
	    else
	      for ( int r=0; r<rDim; r++ )
		for ( int c=0; c<rDim; c++ )
		  grad[r] += dodj(r,c)*mq.jacobianNodeDerivative(etype,node_in_element(e),c);
	  else
	    grad[0] = grad[1] = grad[2] = 0;
	}	
#else
	  ArraySimpleFixed<real,3,3,1,1> dm;
	  dm = 0;
	  
	  int nei = ( useCorners && (etype==UnstructuredMapping::quadrilateral) ) ? 0 : node_in_element(e);
	  
	  if ( obj<.1*REAL_MAX )
	    for ( int r=0; r<rDim; r++ )
	      {
		dm = 0;	    
		for ( int c=0; c<rDim; c++ )
		  for ( int cc=0; cc<rDim; cc++ )
		    dm(r,c) += W(cc,c)*mq.jacobianNodeDerivative(etype,nei,cc);

		
		//  	      cout<<"here is dm"<<dm;
		//  	      cout<<"here is T"<<T<<endl;
		//  	      cout<<"---"<<endl;
		for ( int c=0; c<rDim; c++ )
		  for ( int cc=0; cc<rDim; cc++ )
		    grad[r]+=dodj(c,cc)*dm(c,cc);
	      }
	  else
	    {
	      grad[0] = grad[1] = grad[2] = 0;
	      //	      cout<<"setting grad to zero since obj was bad "<<k<<" "<<"  "<<det<<"  "<<obj<<endl;
	    }
	}
#endif
    
    }
  else if ( rDim==3 )
    {
      for ( int e=0; e<nE; e++ )
	{
	  
	  int n=0;
	  ArraySimpleFixed<real,3,1,1,1> point[8],cent;
	  cent=0;

	  while( elements(e,n)!=-1 && n<8 )
	    {
	      for ( int a=0; a<rDim; a++ ) 
		{
		  point[n][a] = nodes(elements(e,n),a);
		  cent[a] += point[n][a];
		}
	      n++;
	    }

	  for ( int a=0; a<rDim; a++ )
	    cent[a] /= real(n);

	  if ( n==4 )
	    {
	      ArraySimpleFixed<real,3,1,1,1> dum;
	      etype = UnstructuredMapping::tetrahedron;
	      ArraySimpleFixed<real,3,3,1,1> W = mq.computeWeight(cent, etype);
	      jac = mq.computeJacobian(point[0],point[1],point[2],point[3],W);
	    }
	  else if ( n==5 )
	    {
	      ArraySimpleFixed<real,3,1,1,1> dum;
	      etype = UnstructuredMapping::pyramid;
	      ArraySimpleFixed<real,3,3,1,1> W = mq.computeWeight(cent, etype);
	      jac = mq.computeJacobian(point[0],point[1],point[2],point[3],point[4],W);
	    }
	  else if ( n==8 )
	    {
	      ArraySimpleFixed<real,3,1,1,1> dum;
	      etype = UnstructuredMapping::hexahedron;
	      ArraySimpleFixed<real,3,3,1,1> W = mq.computeWeight(cent, etype);



	      if ( useCorners )
		{
		  int offStart =  node_in_element(e)<4 ? 0 : 4;
		  int offset = node_in_element(e);

		  // permute the hex vertices so that the target vertex is in the "0" position
		  //		  cout<<"offset is "<<offset<<endl;
		  if ( offStart==0 )
		    {
		      for ( int n=0; n<4; n++ )
			{
			  for ( int a=0; a<3; a++ )
			    {
			      point[n][a] = nodes(elements(e, (offset+n)%4 ),a);
			      point[n+4][a] = nodes(elements(e,4+(offset+n)%4),a);
			    }
			  
			  //			  cout<<(offset+n)%4<<"   "<<4+(offset+n)%4<<endl;
			}
		    }
		  else
		    { // top is now bottom, iterate backwards
		      for ( int n=0; n<4; n++ )
			{
			  for ( int a=0; a<3; a++ )
			    {
			      point[(4-n)%4][a] = nodes(elements(e, 4+(offset-4+n)%4 ),a);
			      point[4+(4-n)%4][a] = nodes(elements(e,(offset-4+n)%4),a);
			    }
			  //			  cout<<4-n%4<<": "<<4+(offset-4+n)%4<<", "<<7-n<<": "<<(offset-4+n)%4<<endl;
			}
		    }

		  // now compute the three face vertices and three edge vertices
		  ArraySimpleFixed<real,3,1,1,1> fc1,fc2,fc3,ec1,ec2,ec3;
		  fc1=fc2=fc3=ec1=ec2=ec3=0;

		  for ( int a=0; a<3; a++ )
		    {
		      fc1[a] = (point[0][a] + point[1][a] + point[4][a] + point[5][a])/4.;
		      fc2[a] = (point[0][a] + point[1][a] + point[2][a] + point[3][a])/4.;
		      fc3[a] = (point[0][a] + point[4][a] + point[7][a] + point[3][a])/4.;
		      ec1[a] = (point[0][a] + point[1][a])/2.;
		      ec2[a] = (point[0][a] + point[4][a])/2.;
		      ec3[a] = (point[0][a] + point[3][a])/2.;
		    }

		  jac = mq.computeJacobian(point[0], ec1, fc2, ec3, ec1, fc1, cent, fc3, W);
		}
	      else
		jac = mq.computeJacobian(point[0],point[1],point[2],point[3],point[4],
					 point[5],point[6],point[7],W);
	    }
	  else
	    abort();

	  mq.computeJacobianProperties(norm2,det,k,jac);

	  // compute the derivative of the condition number squared by the jacobian
	  //	  dodj = dk2dj(jac,norm2,k,det,rDim);
	  if (objFunc==optShape)
	    dodj = dk2dj(jac,norm2,k,det,rDim);
	  else if ( objFunc==optSize )
	    dodj = dd2dj(jac,norm2,k,det,rDim);
	  else if ( objFunc==optShapeSize )
	    {
	      ArraySimpleFixed<real,3,3,1,1> d1,d2;
	      d1 = dk2dj(jac,norm2,k,det,rDim);
	      d2 = dd2dj(jac,norm2,k,det,rDim);
	      for ( int r=0; r<rDim; r++ )
		for ( int c=0; c<rDim; c++ )
		  dodj(r,c) = k*k*d2(r,c) + max(det*det,1/(det*det))*d1(r,c);
	    }
	  else if ( objFunc==optShapePlusSize )
	    {
	      ArraySimpleFixed<real,3,3,1,1> d1,d2;
	      d1 = dk2dj(jac,norm2,k,det,rDim);
	      d2 = dd2dj(jac,norm2,k,det,rDim);
	      for ( int r=0; r<rDim; r++ )
		for ( int c=0; c<rDim; c++ )
		  dodj(r,c) = d1(r,c) + d2(r,c);
	    }
	  // XXX check this math again!
	  //       ArraySimpleFixed<real,3,3,1,1> djdxi;
	  //       djdxi = 0.;
	  
	  //       for ( int r=0; r<rDim; r++ )
	  // 	for ( int c=0; c<rDim; c++ )
	  // 	  for ( int cc=0; cc<rDim; cc++ )
	  // 	    djdxi(r,c) += T(r,cc)*mq.jacobianNodeDerivative(etype,node_in_element(e),c);
	  
	  if ( k!=REAL_MAX )
	    obj+=k*k;
	  else
	    obj=REAL_MAX;

#if 1
	  //	  for ( int r=0; r<rDim; r++ )
	  //	    for ( int c=0; c<rDim; c++ )
	  //	      grad[r] += dodj(r,c)*mq.jacobianNodeDerivative(etype,node_in_element(e),c);

	  if ( obj<.1*REAL_MAX )
	    if ( useCorners && (etype==UnstructuredMapping::hexahedron) )
	      for ( int r=0; r<rDim; r++ )
		for ( int c=0; c<rDim; c++ )
		  grad[r] += dodj(r,c)*mq.jacobianNodeDerivative(etype,0,c);
	    else
	      for ( int r=0; r<rDim; r++ )
		for ( int c=0; c<rDim; c++ )
		  grad[r] += dodj(r,c)*mq.jacobianNodeDerivative(etype,node_in_element(e),c);
	  else
	    grad[0] = grad[1] = grad[2] = 0;
#else
	  ArraySimpleFixed<real,3,3,1,1> dm;
	  dm = 0;

	  int nei = ( useCorners && (etype==UnstructuredMapping::hexahedron) ) ? 0 : node_in_element(e);

	  if ( obj<.1*REAL_MAX )
	    for ( int r=0; r<rDim; r++ )
	      {
		//		dm = 0;
		//		for ( int c=0; c<rDim; c++ )
		//		  {
		//		    dm(r,c) = T(r,c)*mq.jacobianNodeDerivative(etype,nei,r);
		//		  }
		
		dm = 0;
		for ( int c=0; c<rDim; c++ )
		  for ( int cc=0; cc<rDim; cc++ )
		    {
		      dm(r,c) += T(cc,c)*mq.jacobianNodeDerivative(etype,nei,cc);
		    }
		for ( int c=0; c<rDim; c++ )
		  for ( int cc=0; cc<rDim; cc++ )
		    grad[r]+=dodj(c,cc)*dm(c,cc);
	      }
	  else
	    grad[0] = grad[1] = grad[2] = 0;
#endif
		  
	}
    }

  return grad;
}

#ifdef OLD
static real
objective_analytic_k2(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
		      realArray & nodes, ArraySimpleFixed<real,3,3,1,1> & T )
#else
static real
objective_analytic_k2(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
		      realArray & nodes, MeshQualityMetrics &mq )
#endif
{
  real obj = 0;

  int nE = elements.size(0);
  int rDim = nodes.getLength(1);
  //int dDim = elements.size(1) <= 4 ? 2 : 3;
  ArraySimpleFixed<real,3,3,1,1> T; 
  T = Tident();

  ArraySimpleFixed<real,2,2,1,1> T2;

  //  MeshQualityMetrics mq;

  real norm2,det,k;
  UnstructuredMapping::ElementType etype;

  if ( rDim==2 )
    {  
      for ( int i=0; i<rDim; i++ )
	for ( int j=0; j<rDim; j++ )
	  T2(i,j) = T(i,j);
      ArraySimpleFixed<real,2,1,1,1> xc;
      xc[0] = nodes(elements(0,node_in_element(0)),0);
      xc[1] = nodes(elements(0,node_in_element(0)),1);

      for ( int e=0; e<nE; e++ )
	{
	  ArraySimpleFixed<real,2,1,1,1> point[4],cent;
	  ArraySimpleFixed<real,2,2,1,1> jac2;
	  int n=0; 
	  
	  cent=0;
	  while( elements(e,n)!=-1 && n<4 )
	    {
	      //	      for ( int a=0; a<rDim; a++ ) point[n][a] = nodes(elements(e,n),a);
	      for ( int a=0; a<rDim; a++ ) 
		{
		  point[n][a] = nodes(elements(e,n),a);
		  cent[a] += point[n][a];
		}
	      n++;
	    }

	  for ( int a=0; a<rDim; a++ )
	    cent[a] /= real(n);

	  if ( n==3 )
	    {
	      
	      ArraySimpleFixed<real,2,1,1,1> dum;
	      //	      ArraySimpleFixed<real,2,2,1,1> W = mq.computeWeight(dum, UnstructuredMapping::triangle);
	      ArraySimpleFixed<real,2,2,1,1> W;
	      
	      if ( equilateralIdeal ) 
		W = mq.computeWeight(cent, UnstructuredMapping::triangle);
	      else
		W = mq.computeWeight(cent, UnstructuredMapping::quadrilateral);
	      jac2 = mq.computeJacobian(point[0],point[1],point[2],W);
	      T2=W;
	    }
	  else
	    {
	      etype = UnstructuredMapping::quadrilateral;
		      
	      if ( useCorners )
		{
		  ArraySimpleFixed<real,3,3,1,1> cent;
		  cent[0] = ( point[0][0] + point[1][0] + point[2][0] + point[3][0] )/4.;
		  cent[1] = ( point[0][1] + point[1][1] + point[2][1] + point[3][1] )/4.;
		  
		  int ne=node_in_element(e);
		  int np1 = (ne+1)%n;
		  int nm1 = (ne+n-1)%n;
		  for ( int a=0; a<rDim; a++ )
		    {
		      point[0][a]=nodes(elements(e,ne),a);
		      point[1][a]=(nodes(elements(e,np1),a)+point[0][a])/2.;
		      point[2][a]=cent[a];
		      point[3][a]=(nodes(elements(e,nm1),a)+point[0][a])/2.;
		    }
		}
	      T2 = mq.computeWeight(cent,etype);
	      jac2 = mq.computeJacobian(point[0],point[1],point[2],point[3],T2);
	    }
	  
	  mq.computeJacobianProperties(norm2,det,k,jac2);

// 	  if ( k!=REAL_MAX )
// 	    obj+=k*k;
// 	  else
// 	    obj=REAL_MAX;	  
	  //	  if ( useCorners ) det *=4; 
	  if ( k<.1*sqrt(REAL_MAX) && obj<.1*REAL_MAX )
	    {
	      //	      cout<<"determining objective "<<endl;
	      if ( objFunc==optShape )
		obj+=k*k;
	      else if ( objFunc==optSize )
		{
		  //		  cout<<"using shape "<<det<<endl;
		  obj+=max(det*det,1/(det*det));
		}
	      else if ( objFunc==optShapeSize )
		obj+=k*k*max(det*det,1/(det*det));
	      else if ( objFunc==optShapePlusSize )
		obj += k*k + max(det*det,1/(det*det));
	      else
		abort();
	    }
	  else
	    obj=REAL_MAX;

	  //	  cout<<"obj is "<<obj<<"  "<<k<<" "<<det<<endl;
	}
      
      //	  if ( k<.1*sqrt(REAL_MAX) && obj<.1*REAL_MAX )
      //	    obj+=k*k;
      //	  else
      //	    obj=REAL_MAX;
      //	}
      
    }
  else if ( rDim==3 )
    {
      ArraySimpleFixed<real,3,3,1,1> jac;
      for ( int e=0; e<nE; e++ )
	{
	  
	  int n=0;
	  ArraySimpleFixed<real,3,1,1,1> point[8],cent;

	  cent=0;
	  while( elements(e,n)!=-1 && n<8 )
	    {
	      for ( int a=0; a<rDim; a++ ) 
		{
		  point[n][a] = nodes(elements(e,n),a);
		  cent[a] += point[n][a];
		}
	      n++;
	    }

	  for ( int a=0; a<rDim; a++ )
	    cent[a] /= real(n);

	  if ( n==4 )
	    {
	      ArraySimpleFixed<real,3,1,1,1> dum;
	      etype = UnstructuredMapping::tetrahedron;
	      ArraySimpleFixed<real,3,3,1,1> W = mq.computeWeight(cent, etype);
	      jac = mq.computeJacobian(point[0],point[1],point[2],point[3],W);
	    }
	  else if ( n==5 )
	    {
	      ArraySimpleFixed<real,3,1,1,1> dum;
	      etype = UnstructuredMapping::pyramid;
	      ArraySimpleFixed<real,3,3,1,1> W = mq.computeWeight(cent, etype);
	      jac = mq.computeJacobian(point[0],point[1],point[2],point[3],point[4],W);
	    }
	  else if ( n==8 )
	    {
	      ArraySimpleFixed<real,3,1,1,1> dum;
	      etype = UnstructuredMapping::hexahedron;
	      ArraySimpleFixed<real,3,3,1,1> W = mq.computeWeight(cent, etype);
	      
	      if ( useCorners )
		{
		  int offStart =  node_in_element(e)<4 ? 0 : 4;
		  int offset = node_in_element(e);

		  // permute the hex vertices so that the target vertex is in the "0" position
		  if ( offStart==0 )
		    {
		      for ( int n=0; n<4; n++ )
			for ( int a=0; a<3; a++ )
			  {
			    point[n][a] = nodes(elements(e, (offset+n)%4 ),a);
			    point[n+4][a] = nodes(elements(e,4+(offset+n)%4),a);
			  }
		    }
		  else
		    { // top is now bottom, iterate backwards
		      for ( int n=0; n<4; n++ )
			for ( int a=0; a<3; a++ )
			  {
			      point[(4-n)%4][a] = nodes(elements(e, 4+(offset-4+n)%4 ),a);
			      point[4+(4-n)%4][a] = nodes(elements(e,(offset-4+n)%4),a);
			  }
		    }

		  // now compute the three face vertices and three edge vertices
		  ArraySimpleFixed<real,3,1,1,1> fc1,fc2,fc3,ec1,ec2,ec3;
		  fc1=fc2=fc3=ec1=ec2=ec3=0;

		  for ( int a=0; a<3; a++ )
		    {
		      fc1[a] = (point[0][a] + point[1][a] + point[4][a] + point[5][a])/4.;
		      fc2[a] = (point[0][a] + point[1][a] + point[2][a] + point[3][a])/4.;
		      fc3[a] = (point[0][a] + point[4][a] + point[7][a] + point[3][a])/4.;
		      ec1[a] = (point[0][a] + point[1][a])/2.;
		      ec2[a] = (point[0][a] + point[4][a])/2.;
		      ec3[a] = (point[0][a] + point[3][a])/2.;
		    }

		  jac = mq.computeJacobian(point[0], ec1, fc2, ec3, ec1, fc1, cent, fc3, W);

		}
	      else
		jac = mq.computeJacobian(point[0],point[1],point[2],point[3],point[4],
					 point[5],point[6],point[7],W);
	    }
	  else
	    abort();

	  mq.computeJacobianProperties(norm2,det,k,jac);
	  //	  if ( useCorners && n==8) det *=8; 

	  if ( k<.1*sqrt(REAL_MAX) && obj<.1*REAL_MAX )
	    if ( objFunc==optShape )
	      obj+=k*k;
	    else if ( objFunc==optSize )
	      {
		//		  cout<<"using shape "<<det<<endl;
		obj+=max(det*det,1/(det*det));
	      }
	    else if ( objFunc==optShapeSize )
	      obj+=k*k*max(det*det,1/(det*det));
	    else if ( objFunc==optShapePlusSize )
	      obj += k*k + max(det*det,1/(det*det));
	    else
	      abort();
	  else
	    obj=REAL_MAX;
	  

	  
	}
    }

  return obj;
}

#if 0
static real 
objective_fv_k2(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
	   realArray & nodes, ArraySimpleFixed<real,3,3,1,1> & T)
#endif
static real 
objective_fv_k2(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
	   realArray & nodes, MeshQualityMetrics &mq)
{
  real obj;
  real det, norm2, k;
  int nE = elements.size(0);
  int rDim = nodes.getLength(1);
  //  int dDim = elements.size(1) <= 4 ? 2 : 3;

  //  MeshQualityMetrics mq;
  ArraySimpleFixed<real,3,3,1,1> T; 
  T = Tident();

  ArraySimpleFixed<real,3,1,1,1> grad;
  ArraySimpleFixed<real,3,3,1,1> dfda;

  grad = 0.;
  obj = 0;

  ArraySimpleFixed<real,3,1,1,1> surf;

  real vol = 0;
  if ( rDim==2 )
    {  
      ArraySimpleFixed<real,2,2,1,1> T2;

      for ( int i=0; i<rDim; i++ )
	for ( int j=0; j<rDim; j++ )
	  T2(i,j) = T(i,j);

      for ( int e=0; e<nE; e++ )
	{
	  ArraySimpleFixed<real,2,1,1,1> point[4];
	  ArraySimpleFixed<real,2,2,1,1> jac2;
	  int n=0; 
	  
	  while( elements(e,n)!=-1 && n<4 )
	    {
	      for ( int a=0; a<rDim; a++ ) point[n][a] = nodes(elements(e,n),a);
	      n++;
	    }
	  
	  if ( n==3 )
	    {
	      
	      ArraySimpleFixed<real,2,1,1,1> dum;
	      ArraySimpleFixed<real,2,2,1,1> W = mq.computeWeight(dum, UnstructuredMapping::triangle);
	      
	      // XXX currently ignores T!
	      jac2 = mq.computeJacobian(point[0],point[1],point[2],W);
	    }
	  else
	    jac2 = mq.computeJacobian(point[0],point[1],point[2],point[3],T2);
	  
	  mq.computeJacobianProperties(norm2,det,k,jac2);

	  obj += k*k*det;
	  vol += det;
	}
    }
  return sqrt(obj/vol);
}

#if 0
static ArraySimpleFixed<real,3,1,1,1> 
grad_fv_k2(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
	   realArray & nodes, ArraySimpleFixed<real,3,3,1,1> & T, real obj )
#endif
static ArraySimpleFixed<real,3,1,1,1> 
grad_fv_k2(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
	   realArray & nodes, MeshQualityMetrics &mq, real obj )
{
  real det, norm2, k;
  int nE = elements.size(0);
  int rDim = nodes.getLength(1);
  //  int dDim = elements.size(1) <= 4 ? 2 : 3;
  ArraySimpleFixed<real,3,3,1,1> T; 
  T = Tident();

  //  MeshQualityMetrics mq;

  ArraySimpleFixed<real,3,1,1,1> grad;
  ArraySimpleFixed<real,3,3,1,1> dfda;

  grad = 0.;
  obj = 0;

  ArraySimpleFixed<real,3,1,1,1> surf;

  real vol = 0;
  real sum = 0;
  if ( rDim==2 )
    {  
      ArraySimpleFixed<real,2,2,1,1> T2;

      for ( int i=0; i<rDim; i++ )
	for ( int j=0; j<rDim; j++ )
	  T2(i,j) = T(i,j);

      for ( int e=0; e<nE; e++ )
	{
	  ArraySimpleFixed<real,2,1,1,1> point[4];
	  ArraySimpleFixed<real,2,2,1,1> jac2;
	  int n=0; 
	  
	  while( elements(e,n)!=-1 && n<4 )
	    {
	      for ( int a=0; a<rDim; a++ ) point[n][a] = nodes(elements(e,n),a);
	      n++;
	    }
	  
	  if ( n==3 )
	    {
	      
	      ArraySimpleFixed<real,2,1,1,1> dum;
	      ArraySimpleFixed<real,2,2,1,1> W = mq.computeWeight(dum, UnstructuredMapping::triangle);
	      
	      // XXX currently ignores T!
	      jac2 = mq.computeJacobian(point[0],point[1],point[2],W);
	    }
	  else
	    jac2 = mq.computeJacobian(point[0],point[1],point[2],point[3],T2);
	  
	  mq.computeJacobianProperties(norm2,det,k,jac2);

	  int nn = node_in_element(e);

	  ArraySimpleFixed<real,2,1,1,1> xc1,xc2;

	  for ( int a=0; a<rDim; a++ )
	    {
	      xc1[a] = 0.5*(point[nn][a] + point[(nn+1)%n][a]);
	      xc2[a] = 0.5*(point[nn][a] + point[(nn-1+n)%n][a]);
	    }

	  vol+=triangleArea2D(point[nn],xc1,xc2);

	  surf[0] = xc2[1]-xc1[1];
	  surf[1] = xc1[0]-xc2[0];
	  
	  if ( k!=REAL_MAX && obj<REAL_MAX )
	    {
	      obj += det*k*k;
	      sum += det;
	      for ( int a=0; a<rDim; a++ ) grad[a] += surf[a]*k*k;
	    }
	  else
	    {
	      obj=REAL_MAX;
	      for ( int a=0; a<rDim; a++ ) grad[a] = REAL_MAX;
	    }

	}
      
    }

  if ( obj!=REAL_MAX ) obj = sqrt(obj/sum);

  for ( int a=0; a<rDim; a++ ) grad[a] = grad[a]/vol;
  return grad;
}

#if 0
static ArraySimpleFixed<real,3,3,1,1> 
hess_fv_k2(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
	   realArray & nodes, ArraySimpleFixed<real,3,3,1,1> & T )
#endif
static ArraySimpleFixed<real,3,3,1,1> 
hess_fv_k2(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
	   realArray & nodes, MeshQualityMetrics &mq )
{
  ArraySimpleFixed<real,3,3,1,1> T; 
  T = Tident();
  real det, norm2, k;
  int nE = elements.size(0);
  int rDim = nodes.getLength(1);
  //  int dDim = elements.size(1) <= 4 ? 2 : 3;

  //  MeshQualityMetrics mq;

  ArraySimpleFixed<real,3,1,1,1> grad;
  ArraySimpleFixed<real,3,3,1,1> jac,dfda,hess;

  jac = 0;
  dfda = 0.;
  hess = 0;

  ArraySimpleFixed<real,3,1,1,1> surf;

  real vol = 0;
  if ( rDim==2 )
    {  
      ArraySimpleFixed<real,2,2,1,1> T2;

      for ( int i=0; i<rDim; i++ )
	for ( int j=0; j<rDim; j++ )
	  T2(i,j) = T(i,j);

      for ( int e=0; e<nE; e++ )
	{
	  ArraySimpleFixed<real,2,1,1,1> point[4];
	  ArraySimpleFixed<real,2,2,1,1> jac2;
	  int n=0; 
	  
	  while( elements(e,n)!=-1 && n<4 )
	    {
	      for ( int a=0; a<rDim; a++ ) point[n][a] = nodes(elements(e,n),a);
	      n++;
	    }
	  
	  UnstructuredMapping::ElementType etype;
	  if ( n==3 )
	    {
	      
	      etype = UnstructuredMapping::triangle;
	      ArraySimpleFixed<real,2,1,1,1> dum;
	      ArraySimpleFixed<real,2,2,1,1> W = mq.computeWeight(dum, UnstructuredMapping::triangle);
	      
	      // XXX currently ignores T!
	      jac2 = mq.computeJacobian(point[0],point[1],point[2],T2);
	    }
	  else
	    {
	      etype = UnstructuredMapping::quadrilateral;
	      jac2 = mq.computeJacobian(point[0],point[1],point[2],point[3],T2);
	    }
	  
// 	  cout<<"jac2 "<<jac2<<endl;
// 	  cout<<norm2<<" "<<det<<" "<<k<<endl;
	  mq.computeJacobianProperties(norm2,det,k,jac2);

	  for ( int i=0; i<rDim; i++ )
	    for ( int j=0; j<rDim; j++ )
	      jac(i,j) = jac2(i,j);

	  dfda = dk2dj(jac,norm2,k,det,rDim);

	  int nn = node_in_element(e);
	  ArraySimpleFixed<real,2,1,1,1> xc1,xc2;

	  for ( int a=0; a<rDim; a++ )
	    {
	      xc1[a] = 0.5*(point[nn][a] + point[(nn+1)%n][a]);
	      xc2[a] = 0.5*(point[nn][a] + point[(nn-1+n)%n][a]);
	    }

	  vol+=triangleArea2D(point[nn],xc1,xc2);

	  surf[0] = xc2[1]-xc1[1];
	  surf[1] = xc1[0]-xc2[0];
// 	  cout<<"surf for element "<<e<<" "<<surf<<endl;
// 	  cout<<"jac "<<jac<<endl;
// 	  cout<<"dfda "<<dfda<<endl;
	  if ( k<REAL_MAX )
	    {
	      //	      cout<<"gradient for element "<<e<<endl;
	      for ( int a=0; a<rDim; a++ )
		{
		  real gr = 0.;
		  for ( int aaa=0; aaa<rDim; aaa++ )
		    gr += dfda(a,aaa)*mq.jacobianNodeDerivative(etype,nn,aaa);

		  //		  cout<<gr<<endl;
		  for ( int aa=0; aa<rDim; aa++ )
		    hess(a,aa) += surf[aa]*gr;
		}
	    }
	  else
	    {
	      hess = 0;
	      break;
	    }

	}
      
    }

  for ( int a=0; a<hess.size(); a++ ) hess[a] = hess[a]/vol;
  return hess;
}

#if 0
ArraySimpleFixed<real,3,1,1,1> 
optimize_one_node_newton_fv(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
			    realArray & nodes, ArraySimpleFixed<real,3,3,1,1> & T )
#endif
ArraySimpleFixed<real,3,1,1,1> 
optimize_one_node_newton_fv(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
			    realArray & nodes, MeshQualityMetrics &mq )

{
  // elements(nElem,nMax) the nodes of each element in the canonical ordering
  // node_in_element gives the index into each element of the node to be optimized
  // nodes are the nodes in this subset of the mesh
  // T is the scaling for the transformation

  int nE = elements.size(0);
  int rDim = nodes.getLength(1);
  int nemax = rDim==2 ? 4 : 8;
  //  int dDim = elements.size(1) <= 4 ? 2 : 3;
  ArraySimpleFixed<real,3,3,1,1> T; 
  T = Tident();
 
   ArraySimpleFixed<real,3,1,1,1> original_node;
   for ( int a=0; a<rDim; a++ ) original_node[a] = nodes(elements(0,node_in_element(0)),a);
//   ArraySimpleFixed<real,3,1,1,1> original_node;
//   for ( int a=0; a<rDim; a++ ) original_node[a] = nodes(elements(0,node_in_element(0)),a);

  // compute the objective and its gradient 
  real obj = -REAL_MAX;

  ArraySimpleFixed<real,3,1,1,1> grad = grad_fv_k2(node_in_element, elements, nodes, mq, obj);

  real mag_grad = sqrt(ASmag2(grad));
  // now estimate the hessian
  ArraySimpleFixed<real,3,3,1,1> hess = hess_fv_k2(node_in_element, elements, nodes, mq);

  // compute a normalized search direction and initial step size
  ArraySimpleFixed<real,3,1,1,1> dir;
  dir=0;
  ArraySimpleFixed<real,3,3,1,1> hess_inv;
  real hess_det = hess(0,0)*hess(1,1)-hess(1,0)*hess(0,1);

  real d_use = 0.;
  if ( fabs(hess_det) > 100*REAL_MIN && mag_grad>100*REAL_MIN )
    {
      hess_inv = 0;
      hess_inv(0,0) = hess(1,1)/hess_det;
      hess_inv(1,1) = hess(0,0)/hess_det;
      hess_inv(0,1) = -hess(0,1)/hess_det;
      hess_inv(1,0) = -hess(1,0)/hess_det;

      dir[0] = -(hess_inv(0,0)*grad[0]+hess_inv(0,1)*grad[1]);
      dir[1] = -(hess_inv(1,0)*grad[0]+hess_inv(1,1)*grad[1]);

      real dlmax = sqrt(ASmag2(dir));
      for ( int a=0; a<rDim; a++ ) dir[a] = dir[a]/dlmax;

//    cout<<"dir is "<<dir<<endl;
//    cout<<"grad is "<<grad<<endl;
//   ArraySimpleFixed<real,3,1,1,1> analytic_grad = grad_analytic_k2(node_in_element, elements, nodes, T, obj);
//   real agmag = sqrt(ASmag2(analytic_grad));
//   //  for ( int a=0; a<rDim; a++ ) analytic_grad[a] = -analytic_grad[a]/agmag;
//   cout<<"dir from analytic grad is "<<analytic_grad<<endl;
//   cout<<"H is "<<hess<<endl;
//   cout<<"Hinv is "<<hess_inv<<endl;
//   real a = 1;
//   real b = -(hess(0,0)+hess(1,1));
//   real c = hess_det;
//   cout<<"b*b-4*a*c "<<b*b-4.*a*c<<endl;
//   cout<<"e1 "<<(-b+sqrt(b*b-4.*a*c))/2./a<<endl;
//   cout<<"e2 "<<(-b-sqrt(b*b-4.*a*c))/2./a<<endl;
//   cout<<"hess_det "<<hess_det<<endl;

      d_use = linesearch(node_in_element, elements, nodes, mq, dlmax, dir, objective_fv_k2);

    }
  else if ( mag_grad>100*REAL_MIN )
    {
      // resort to steepest descent
      // compute a normalized search direction
      real mag_grad = sqrt(ASmag2(grad));
      for ( int a=0; a<rDim; a++ ) dir[a] = -grad[a]/mag_grad;

      real dlmax = -REAL_MAX;
      for ( int e=0; e<nE; e++ )
	{
	  ArraySimpleFixed<real,3,1,1,1> dx;
	  dx=0;
	  int ne=0;
	  while ( ne<nemax && elements(e,ne)!=-1 )
	    {
	      for ( int a=0; a<rDim; a++ )
		dx[a] = nodes(elements(e,ne),a) - original_node[a];
	      
	      dlmax = max(dlmax,ASmag2(dx));

	      ne++;
	    }
	  
	}

      dlmax = sqrt(dlmax);
      
      d_use = linesearch(node_in_element, elements, nodes, mq, dlmax, dir, objective_analytic_k2);
    }

  ArraySimpleFixed<real,3,1,1,1> new_node;
  for ( int a=0; a<rDim; a++ ) new_node[a] = original_node[a] + d_use*dir[a];

  return new_node;
}


ArraySimpleFixed<real,3,1,1,1>
optimize_one_node_move_to_center(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
				   realArray & nodes, ArraySimpleFixed<real,3,3,1,1> & T )
{
  // elements(nElem,nMax) the nodes of each element in the canonical ordering
  // node_in_element gives the index into each element of the node to be optimized
  // nodes are the nodes in this subset of the mesh
  // T is the scaling for the transformation

  int nE = elements.size(0);
  int rDim = nodes.getLength(1);
  int nemax = rDim==2 ? 4 : 8;

  //  int dDim = elements.size(1) <= 4 ? 2 : 3;

  ArraySimpleFixed<real,3,1,1,1> original_node;
  for ( int a=0; a<rDim; a++ ) original_node[a] = nodes(elements(0,node_in_element(0)),a);

  ArraySimpleFixed<real,3,1,1,1> avgVert;
  avgVert = 0;

  int nForAvg=0;
  for ( int e=0; e<nE; e++ )
    {
      int ne=0;
      while ( ne<nemax && elements(e,ne)!=-1 )
	{
	  if ( ne!=node_in_element(e) )
	    {
	      for ( int a=0; a<rDim; a++ )
		avgVert[a] += nodes(elements(e,ne),a);
	      nForAvg++;
	    }
	  ne++;
	}
    }


  ArraySimpleFixed<real,3,1,1,1> new_node;
  for ( int a=0; a<rDim; a++ )
    new_node[a] = avgVert[a]/real(nForAvg);
      
  return new_node;

}

#if 0
ArraySimpleFixed<real,3,1,1,1>
optimize_one_node_steepest_descent(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
				   realArray & nodes, ArraySimpleFixed<real,3,3,1,1> & T )
#endif
ArraySimpleFixed<real,3,1,1,1>
optimize_one_node_steepest_descent(ArraySimple<int> &node_in_element, ArraySimple<int> & elements, 
				   realArray & nodes, MeshQualityMetrics &mq )
{
  // elements(nElem,nMax) the nodes of each element in the canonical ordering
  // node_in_element gives the index into each element of the node to be optimized
  // nodes are the nodes in this subset of the mesh
  // T is the scaling for the transformation

  int nE = elements.size(0);
  int rDim = nodes.getLength(1);
  int nemax = rDim==2 ? 4 : 8;

  //  int dDim = elements.size(1) <= 4 ? 2 : 3;
  ArraySimpleFixed<real,3,3,1,1> T; 
  T = Tident();

  ArraySimpleFixed<real,3,1,1,1> original_node;
  for ( int a=0; a<rDim; a++ ) original_node[a] = nodes(elements(0,node_in_element(0)),a);

  // compute the objective and its gradient 
  real obj = -REAL_MAX;
  ArraySimpleFixed<real,3,1,1,1> grad = grad_analytic_k2(node_in_element, elements, nodes, mq, obj);

  // compute a normalized search direction
  real mag_grad = sqrt(ASmag2(grad));
  ArraySimpleFixed<real,3,1,1,1> dir;
  dir[2] = 0;
  if ( fabs(mag_grad)>100*REAL_MIN )
    for ( int a=0; a<rDim; a++ ) dir[a] = -grad[a]/mag_grad;

  //       cout<<"HERE IS THE GRADIENT "<<grad<<endl;
  //       cout<<"HERE IS THE DIRECTION "<<dir<<endl;

  real dlmax = -REAL_MAX;

  int nForAvg = 0;
  ArraySimpleFixed<real,3,1,1,1> avgVert;
  avgVert = 0;

  for ( int e=0; e<nE; e++ )
    {
      ArraySimpleFixed<real,3,1,1,1> dx;
      dx=0;
      int ne=0;
      while ( ne<nemax && elements(e,ne)!=-1 )
	{
	  for ( int a=0; a<rDim; a++ )
	    dx[a] = nodes(elements(e,ne),a) - original_node[a];

	  dlmax = max(dlmax,ASmag2(dx));

	  if ( ne!=node_in_element(e) )
	    {
	      for ( int a=0; a<rDim; a++ )
		avgVert[a] += nodes(elements(e,ne),a);
	      nForAvg++;
	    }
	  ne++;
	}
    }


  dlmax = sqrt(dlmax);
  
  real d_use=0.;
  
  
  ArraySimpleFixed<real,3,1,1,1> new_node;
  if ( fabs(obj)>.1*REAL_MAX || mag_grad<REAL_EPSILON )
    {
      // the gradient was absurd due to an inverted element or something, move towards the average of the surrounding verts
      //      cout<<"WARNING : resorting to a move to the average vertex because "<< (mag_grad<REAL_EPSILON ? "the gradient was zero" : "one or more elements were invalid")<<endl;
//             for ( int a=0; a<rDim; a++ )
//        	dir[a] = avgVert[a]/real(nForAvg) - original_node[a];
       for ( int a=0; a<rDim; a++ )
 	new_node[a] = avgVert[a]/real(nForAvg);
      
    }
  else
    {
  
      //   if ( fabs(obj)>.1REAL_MAX )
      //     {
      d_use = linesearch(node_in_element, elements, nodes, mq, dlmax, dir, objective_analytic_k2);
      //     }
      //   else
      //      cout<<"d_use is "<<d_use<<endl;

      real uCoeff = .9;
      for ( int a=0; a<rDim; a++ ) new_node[a] = original_node[a] + uCoeff*d_use*dir[a];
    }

  return new_node;

}

//void optimize(UnstructuredMapping &umap, RealCompositeGridFunction *cf)
void optimize(UnstructuredMapping &umap, MetricEvaluator &cf)
{
 
  //  int nElems = umap.getNumberOfElements();
  //  int nNodes = umap.getNumberOfNodes();
  int nElems = umap.size(UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension()));
  int nNodes = umap.size(UnstructuredMapping::Vertex);
  int rangeDim = umap.getRangeDimension();

  //  const intArray  & elems = umap.getElements();
  //  const intArray & faces = umap.getFaces();
  const intArray  & elems = umap.getEntities(UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension()));
  const intArray & faces = umap.getEntities(UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension()-1));

  realArray & xyz = (realArray &)umap.getNodes();
  intArray nodeElements;

  //  umap.createNodeElementList(nodeElements);

  int numberMoved=0;

  intArray xyz_onbdy(nNodes);
  xyz_onbdy = 0;

  MeshQualityMetrics mq(umap);

  mq.setReferenceTransformation(&cf);

  int e,n,r;

  // determine the nodes on the boundary, these will be ignored
  int bf;
#if 0
  for ( bf=0; bf<umap.getNumberOfBoundaryFaces(); bf++ )
    {
      int f = umap.getBoundaryFace(bf);
      for ( int nf=0; nf<umap.getNumberOfNodesThisFace(f); nf++ )
	{
	  int n = faces(f,nf);
	  xyz_onbdy(n) = 1;
	}
    }
#endif

  int ec=0;

  ArraySimpleFixed<real,3,3,1,1> T;
  T = 0;
  T(0,0) = T(1,1) = T(2,2) = 1;

  //  for ( n=0; n<nNodes; n++ )
  //    {  
  UnstructuredMappingIterator vert;
  
  //  for ( n=0; n<nNodes; n++ )
  UnstructuredMapping::EntityTypeEnum cellType = umap.getDomainDimension()==2 ? 
  UnstructuredMapping::Face : UnstructuredMapping::Region;
  
  for ( vert=umap.begin(UnstructuredMapping::Vertex); vert!=umap.end(UnstructuredMapping::Vertex); vert++)
  {
    int n = *vert;
    //    if ( xyz_onbdy(n)==0 )
    if ( !umap.hasTag(UnstructuredMapping::Vertex,n,std::string("boundary ")+UnstructuredMapping::EntityTypeStrings[UnstructuredMapping::Vertex].c_str()) )
      {
	int ec0=ec;
	
	int nEl=0;
	//	while ( nodeElements(ec++,0)!=-1 ) nEl++;
	
	UnstructuredMappingAdjacencyIterator a_iter;
	
	a_iter = umap.adjacency_begin(vert,cellType);
	while ( a_iter != umap.adjacency_end(vert,cellType) ) { a_iter++; nEl++; }
	
	if ( nEl )
	  {
	    ArraySimpleFixed<real,3,1,1,1> new_node;
	    ArraySimple<int> local_elements(nEl,umap.getMaxNumberOfNodesPerElement()+1);
	    ArraySimple<int> node_in_element(nEl);
	    
	    local_elements = -1;
	    node_in_element = -1;
	    
#if 0
	    for ( int e=0; e<nEl; e++ )
	      for ( int n=0; n<umap.getMaxNumberOfNodesPerElement(); n++ )
		{
		  local_elements(e,n) = elems(nodeElements(ec0+e,0),n);
		  node_in_element(e) = nodeElements(ec0+e,1);
		}
#endif
	    int e=0;
	    for ( a_iter=umap.adjacency_begin(vert,cellType); 
		  a_iter!=umap.adjacency_end(vert,cellType); 
		  a_iter++ )
	      {
		UnstructuredMappingAdjacencyIterator av_iter;
		int nie = -1;
		int n=0;
		for ( av_iter = umap.adjacency_begin(a_iter, UnstructuredMapping::Vertex);
		      av_iter != umap.adjacency_end(a_iter, UnstructuredMapping::Vertex);
		      av_iter++ )
		  {
		    local_elements(e,n) = *av_iter;
		    if ( *av_iter == *vert ) nie = n;
		    n++;
		  }
		node_in_element(e) = nie;
		
		e++;
	      }
#ifdef STEEPEST
	    new_node = optimize_one_node_steepest_descent(node_in_element, local_elements, xyz, mq );

	    for ( int a=0; a<rangeDim; a++ )
	      xyz(n,a) = new_node[a];
#endif
#ifdef NEWTON
	    
	    if ( rangeDim==2 )
	      {
		new_node = optimize_one_node_newton_fv(node_in_element, local_elements, xyz, T );
		for ( int a=0; a<rangeDim; a++ )
		  xyz(n,a) = new_node[a];
	      }
#endif
	    
	    //optimize_one_node_move_to_center(node_in_element, local_elements, xyz, T );
	  }
      }
    else
      {
	//	while( nodeElements(ec,0)!=-1 ) ec++;
		ec++;
      }
  }
  
}

void optimize_one(UnstructuredMapping &umap, UnstructuredMappingIterator &vert, MetricEvaluator &cf)
{
 
  //  int nElems = umap.getNumberOfElements();
  //  int nNodes = umap.getNumberOfNodes();
  int nElems = umap.size(UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension()));
  int nNodes = umap.size(UnstructuredMapping::Vertex);
  int rangeDim = umap.getRangeDimension();

  //  const intArray  & elems = umap.getElements();
  //  const intArray & faces = umap.getFaces();
  const intArray  & elems = umap.getEntities(UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension()));
  const intArray & faces = umap.getEntities(UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension()-1));

  realArray & xyz = (realArray &)umap.getNodes();

  int numberMoved=0;

  MeshQualityMetrics mq(umap);

  mq.setReferenceTransformation(&cf);

  int e,n,r;

  // determine the nodes on the boundary, these will be ignored
  int bf;

  int ec=0;

  ArraySimpleFixed<real,3,3,1,1> T;
  T = 0;
  T(0,0) = T(1,1) = T(2,2) = 1;

  //  for ( n=0; n<nNodes; n++ )
  //    {  

  //  for ( n=0; n<nNodes; n++ )
  UnstructuredMapping::EntityTypeEnum cellType = umap.getDomainDimension()==2 ? 
  UnstructuredMapping::Face : UnstructuredMapping::Region;
  
  n = *vert;
  //    if ( xyz_onbdy(n)==0 )
  if ( !umap.hasTag(UnstructuredMapping::Vertex,n,std::string("boundary ")+UnstructuredMapping::EntityTypeStrings[UnstructuredMapping::Vertex].c_str()) )
    {
      int ec0=ec;
      
      int nEl=0;
      //	while ( nodeElements(ec++,0)!=-1 ) nEl++;
      
      UnstructuredMappingAdjacencyIterator a_iter;
      
      a_iter = umap.adjacency_begin(vert,cellType);
      while ( a_iter != umap.adjacency_end(vert,cellType) ) { a_iter++; nEl++; }
      
      if ( nEl )
	{
	  ArraySimpleFixed<real,3,1,1,1> new_node;
	  ArraySimple<int> local_elements(nEl,umap.getMaxNumberOfNodesPerElement()+1);
	  ArraySimple<int> node_in_element(nEl);
	  
	  local_elements = -1;
	  node_in_element = -1;
	  
	  int e=0;
	  for ( a_iter=umap.adjacency_begin(vert,cellType); 
		a_iter!=umap.adjacency_end(vert,cellType); 
		a_iter++ )
	    {
	      UnstructuredMappingAdjacencyIterator av_iter;
	      int nie = -1;
	      int n=0;
	      for ( av_iter = umap.adjacency_begin(a_iter, UnstructuredMapping::Vertex);
		    av_iter != umap.adjacency_end(a_iter, UnstructuredMapping::Vertex);
		    av_iter++ )
		{
		  local_elements(e,n) = *av_iter;
		  if ( *av_iter == *vert ) nie = n;
		  n++;
		}
	      node_in_element(e) = nie;
	      
	      e++;
	    }
	  new_node = optimize_one_node_steepest_descent(node_in_element, local_elements, xyz, mq );

	  for ( int a=0; a<rangeDim; a++ )
	    xyz(n,a) = new_node[a];

	  }
      }
}

