#include "OB_MappedGridSolver.h"
#include "Parameters.h"
#include "OB_MappedGridSolver.h"
#include "App.h"
#include "FlowSolutions.h"
#include "ParallelUtility.h"

#include "SurfaceEquation.h"
extern SurfaceEquation surfaceEquation;  // This is in the global name space for now.

#define cnsSlipWallBC cnsslipwallbc_
#define cnsSlipWallBC2 cnsslipwallbc2_
#define cnsFarFieldBC cnsfarfieldbc_
#define cnsNoSlipWallBC cnsnoslipwallbc_
extern "C"
{
  void cnsSlipWallBC(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
                     const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,\
		     const int&ipar, const real&rpar, 
                     real&u, const real&u2, const real&gv, const real&gv2, const real & gtt,
                     const int& mask, const real&x, const real&rsxy, 
                     const int&bc, const int&indexRange, const int&exact, 
                     const real& uKnown, const int&ierr );
  void cnsSlipWallBC2(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
                     const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,\
		     const int&ipar, const real&rpar, 
                     real&u, const real&u2, const real&gv, const real&gv2, const real & gtt,
                     const int& mask, const real&x, const real&rsxy, 
                     const int&bc, const int&indexRange, const int&exact, 
                     const real& uKnown, const int&ierr );

  void cnsFarFieldBC(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
                     const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,\
		     const int&ipar, const real&rpar, 
                     real&u, const real&u2, const real&gv, const real&gv2, const real & gtt,
                     const int& mask, const real&x, const real&rsxy, 
                     const int&bc, const int&indexRange, const int&exact, 
                     const real& uKnown, const int&ierr );

  void cnsNoSlipWallBC(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
		       const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,
		       const real*u, const real*x, const real *aj, const real*rsxy,
		       const int*ipar, const real*rpar, const int*indexRange, const int*bc);
}


// extern "C"
// {

// /* Here are functions for TZ flow that can be called from fortran */

// /* return a general derivative */
// void
// ogderiv_(OGFunction *&ep, const int & ntd, const int & nxd, const int & nyd, const int & nzd, 
//          const real &x, const real &y, const real &z, const real & t, const int & n, real & ud )
// {
//   ud=(*ep).gd(ntd,nxd,nyd,nzd,x,y,z,n,t);
// }
 
// /* Return the forcing functions and derivatives for the Taylor slipwall BC */
// /*   ud(0)= RHS for rho equation */
// void
// ogftaylor_(OGFunction *&ep, const real &x, const real &y, const real &z, const real & t, 
//            const int & nd, real *ud )
// {
//   const int rc=0, uc=1, vc=2, qc=3;
//   const real gamma=1.4, gm1=gamma-1.;

//   real r,rt,rx,ry,rxx,rxy,ryy, rtx,rty,rtt;
//   real u,ut,ux,uy,uxx,uxy,uyy, utx,uty,utt;
//   real v,vt,vx,vy,vxx,vxy,vyy, vtx,vty,vtt;
//   real q,qt,qx,qy,qxx,qxy,qyy, qtx,qty,qtt;
//   real p,pt,px,py,pxx,pxy,pyy, ptx,pty,ptt;
//   real rf,uf,vf,qf,pf;
//   real rtf,utf,vtf,qtf,ptf;
//   real rxf,uxf,vxf,qxf,pxf;
//   real ryf,uyf,vyf,qyf,pyf;

  

//   r  =(*ep).gd(0,0,0,0,x,y,z,rc,t);
//   rt =(*ep).gd(1,0,0,0,x,y,z,rc,t);
//   rx =(*ep).gd(0,1,0,0,x,y,z,rc,t);
//   ry =(*ep).gd(0,0,1,0,x,y,z,rc,t);
//   rtt=(*ep).gd(2,0,0,0,x,y,z,rc,t);
//   rtx=(*ep).gd(1,1,0,0,x,y,z,rc,t);
//   rty=(*ep).gd(1,0,1,0,x,y,z,rc,t);
//   rxx=(*ep).gd(0,2,0,0,x,y,z,rc,t);
//   rxy=(*ep).gd(0,1,1,0,x,y,z,rc,t);
//   ryy=(*ep).gd(0,0,2,0,x,y,z,rc,t);

//   u  =(*ep).gd(0,0,0,0,x,y,z,uc,t);
//   ut =(*ep).gd(1,0,0,0,x,y,z,uc,t);
//   ux =(*ep).gd(0,1,0,0,x,y,z,uc,t);
//   uy =(*ep).gd(0,0,1,0,x,y,z,uc,t);
//   utt=(*ep).gd(2,0,0,0,x,y,z,uc,t);
//   utx=(*ep).gd(1,1,0,0,x,y,z,uc,t);
//   uty=(*ep).gd(1,0,1,0,x,y,z,uc,t);
//   uxx=(*ep).gd(0,2,0,0,x,y,z,uc,t);
//   uxy=(*ep).gd(0,1,1,0,x,y,z,uc,t);
//   uyy=(*ep).gd(0,0,2,0,x,y,z,uc,t);

//   v  =(*ep).gd(0,0,0,0,x,y,z,vc,t);
//   vt =(*ep).gd(1,0,0,0,x,y,z,vc,t);
//   vx =(*ep).gd(0,1,0,0,x,y,z,vc,t);
//   vy =(*ep).gd(0,0,1,0,x,y,z,vc,t);
//   vtt=(*ep).gd(2,0,0,0,x,y,z,vc,t);
//   vtx=(*ep).gd(1,1,0,0,x,y,z,vc,t);
//   vty=(*ep).gd(1,0,1,0,x,y,z,vc,t);
//   vxx=(*ep).gd(0,2,0,0,x,y,z,vc,t);
//   vxy=(*ep).gd(0,1,1,0,x,y,z,vc,t);
//   vyy=(*ep).gd(0,0,2,0,x,y,z,vc,t);

//   q  =(*ep).gd(0,0,0,0,x,y,z,qc,t);
//   qt =(*ep).gd(1,0,0,0,x,y,z,qc,t);
//   qx =(*ep).gd(0,1,0,0,x,y,z,qc,t);
//   qy =(*ep).gd(0,0,1,0,x,y,z,qc,t);
//   qtt=(*ep).gd(2,0,0,0,x,y,z,qc,t);
//   qtx=(*ep).gd(1,1,0,0,x,y,z,qc,t);
//   qty=(*ep).gd(1,0,1,0,x,y,z,qc,t);
//   qxx=(*ep).gd(0,2,0,0,x,y,z,qc,t);
//   qxy=(*ep).gd(0,1,1,0,x,y,z,qc,t);
//   qyy=(*ep).gd(0,0,2,0,x,y,z,qc,t);

//   p=r*q;
//   pt=rt*q+r*qt;
//   px=rx*q+r*qx;
//   py=ry*q+r*qy;
//   ptt=rtt*q+2.*rt*qt+r*qtt;
//   pxx=rxx*q+2.*rx*qx+r*qxx;
//   pyy=ryy*q+2.*ry*qy+r*qyy;
  
//   ptx=rtx*q+rt*qx+rx*qt+r*qtx;
//   pty=rty*q+rt*qy+ry*qt+r*qty;
//   pxy=rxy*q+rx*qy+ry*qx+r*qxy;

//   rf = rt + u*rx+v*ry + r*(ux+vy);
//   uf = ut + u*ux+v*uy + px/r;
//   vf = vt + u*vx+v*vy + py/r;
//   qf = qt + u*qx+v*qy + gm1*q*(ux+vy);
//   pf = pt + u*px+v*py + gamma*p*(ux+vy);
  
//   rtf = rtt + ut*rx+vt*ry + rt*(ux+vy) + u*rtx+v*rty + r*(utx+vty);
//   utf = utt + ut*ux+vt*uy + ptx/r + u*utx+v*uty - px*rt/(r*r);
//   vtf = vtt + ut*vx+vt*vy + pty/r + u*vtx+v*vty - py*rt/(r*r);
//   qtf = qtt + ut*qx+vt*qy + gm1*qt*(ux+vy) + u*qtx+v*qty + gm1*q*(utx+vty);
//   ptf = ptt + ut*px+vt*py + gamma*pt*(ux+vy) + u*ptx+v*pty + gamma*p*(utx+vty);
  
//   rxf = rtx + ux*rx+vx*ry + rx*(ux+vy) + u*rxx+v*rxy + r*(uxx+vxy);
//   uxf = utx + ux*ux+vx*uy + pxx/r + u*uxx+v*uxy - px*rx/(r*r);
//   vxf = vtx + ux*vx+vx*vy + pxy/r + u*vxx+v*vxy - py*rx/(r*r);
//   qxf = qtx + ux*qx+vx*qy + gm1*qx*(ux+vy) + u*qxx+v*qxy + gm1*q*(uxx+vxy);
//   pxf = ptx + ux*px+vx*py + gamma*px*(ux+vy) + u*pxx+v*pxy + gamma*p*(uxx+vxy);
  
//   ryf = rty + uy*rx+vy*ry + ry*(ux+vy) + u*rxy+v*ryy + r*(uxy+vyy);
//   uyf = uty + uy*ux+vy*uy + pxy/r + u*uxy+v*uyy - px*ry/(r*r);
//   vyf = vty + uy*vx+vy*vy + pyy/r + u*vxy+v*vyy - py*ry/(r*r);
//   qyf = qty + uy*qx+vy*qy + gm1*qy*(ux+vy) + u*qxy+v*qyy + gm1*q*(uxy+vyy);
//   pyf = pty + uy*px+vy*py + gamma*py*(ux+vy) + u*pxy+v*pyy + gamma*p*(uxy+vyy);
  

//   int n=0;
//   ud[n]=rf;  n++;
//   ud[n]=uf;  n++;
//   ud[n]=vf;  n++;
//   ud[n]=qf;  n++;
//   ud[n]=pf;  n++;   /* ud[4] */
  
//   ud[n]=rtf;  n++;
//   ud[n]=utf;  n++;
//   ud[n]=vtf;  n++;
//   ud[n]=qtf;  n++; 
//   ud[n]=ptf;  n++;  /* ud[9] */
  
//   ud[n]=rxf;  n++;
//   ud[n]=uxf;  n++;
//   ud[n]=vxf;  n++;
//   ud[n]=qxf;  n++;
//   ud[n]=pxf;  n++;  /* ud[14] */ 
  
//   ud[n]=ryf;  n++;
//   ud[n]=uyf;  n++;
//   ud[n]=vyf;  n++;
//   ud[n]=qyf;  n++;
//   ud[n]=pyf;  n++;  /* ud[19] */ 
  

// }
 
// }


// =============================================================================================================
// /Description:
//    Compute a new gridIndexRange, dimension
//             and boundaryCondition array that will be valid for the local grid on a processor.
// 
//    Set the gid to match the ends of the local array.
//    Set the bc(side,axis) to -1 for internal boundaries between processors
//
// NOTES: In parallel we cannot assume the rsxy array is defined on all ghost points -- it will not
// be set on the extra ghost points put at the far ends of the array. -- i.e. internal boundary ghost 
// points will be set but not external
// =============================================================================================================
void
getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
                                     IntegerArray & gidLocal, 
                                     IntegerArray & dimensionLocal, 
                                     IntegerArray & bcLocal );


// {

//   MappedGrid & mg = *a.getMappedGrid();
  
//   const IntegerArray & dimension = mg.dimension();
//   const IntegerArray & gid = mg.gridIndexRange();
//   const IntegerArray & bc = mg.boundaryCondition();
  
//   gidLocal = gid;
//   bcLocal = bc;
//   dimensionLocal=dimension;
  
//   for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
//   {
// //      printf(" axis=%i gidLocal(0,axis)=%i a.getLocalBase(axis)=%i  dimension(0,axis)=%i\n",axis,gidLocal(0,axis),
// //                        a.getLocalBase(axis),dimension(0,axis));
// //      printf(" axis=%i gidLocal(1,axis)=%i a.getLocalBound(axis)=%i dimension(0,axis)=%i\n",axis,gidLocal(1,axis),
// //                        a.getLocalBound(axis),dimension(1,axis));
//     if( a.getLocalBase(axis) == a.getBase(axis) ) 
//     {
//       assert( dimension(0,axis)==a.getLocalBase(axis) );
//       gidLocal(0,axis) = gid(0,axis); 
//       dimensionLocal(0,axis) = dimension(0,axis); 
//     }
//     else
//     {
//       gidLocal(0,axis) = a.getLocalBase(axis)+a.getGhostBoundaryWidth(axis);
//       dimensionLocal(0,axis) = a.getLocalBase(axis); 
//       // for internal ghost mark as periodic since these behave in the same was as periodic
//       // ** we cannot mark as "0" since the mask may be non-zero at these points and assignBC will 
//       // access points out of bounds
//       bcLocal(0,axis) = -1; // bc(0,axis)>=0 ? 0 : -1;
//     }
    
//     if( a.getLocalBound(axis) == a.getBound(axis) ) 
//     {
//       assert( dimension(1,axis) == a.getLocalBound(axis) );
      
//       gidLocal(1,axis) = gid(1,axis); 
//       dimensionLocal(1,axis) = dimension(1,axis); 
//     }
//     else
//     {
//       gidLocal(1,axis) = a.getLocalBound(axis)-a.getGhostBoundaryWidth(axis);
//       dimensionLocal(1,axis) = a.getLocalBound(axis);
//       // for internal ghost mark as periodic since these behave in the same was as periodic
//       bcLocal(1,axis) = -1; // bc(1,axis)>=0 ? 0 : -1;
//     }
    
//   }
// }


int
checkSolution(realMappedGridFunction & u, const aString & title, bool printResults, int grid,
              real & maxVal, bool printResultsOnFailure=false );

void
checkNormalBC( int grid, MappedGrid & mg, realMappedGridFunction & u, Parameters & parameters );


// {
//   const int numberOfDimensions=mg.numberOfDimensions();
  
//   int side,axis;
//   real maxErr=0., maxErr2=0., uDotN, maxNormalError=0.;
//   Index Ib1,Ib2,Ib3;
//   for( side=0; side<=1; side++ )
//   {
//     for( axis=0; axis<mg.numberOfDimensions(); axis++ )
//     {
//       if( mg.boundaryCondition(side,axis)==Parameters::slipWall )
//       {
// 	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,-1);
//         #ifndef USE_PPP
//     	  const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
//         #else
//     	  // const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);  // parallel version
//           const realArray & normal = mg.vertexBoundaryNormal(side,axis);
// 	  Overture::abort("error: fix this for parallel -- set uDotN below");
//         #endif
// 	if( mg.numberOfDimensions()==2 )
// 	{
// 	  uDotN=max(fabs(
//              normal(Ib1,Ib2,Ib3,0)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("uc"))
// 	    +normal(Ib1,Ib2,Ib3,1)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("vc"))));
// 	}
// 	else
// 	{
// 	  uDotN=max(
//             normal(Ib1,Ib2,Ib3,0)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("uc"))
// 	    +normal(Ib1,Ib2,Ib3,1)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("vc"))  
// 	    +normal(Ib1,Ib2,Ib3,2)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("wc")));
// 	}
// 	maxErr=max(maxErr,uDotN);

//         realArray & rx = mg.inverseVertexDerivative();
//         int n1,n2,n3;
//         n1=axis;
//         n2=n1+mg.numberOfDimensions(); n3=n1+2*mg.numberOfDimensions();
// 	if( mg.numberOfDimensions()==2 )
// 	{
// 	  realArray norm;
// 	  norm=(2.*side-1)/SQRT( SQR(rx(Ib1,Ib2,Ib3,n1))+SQR(rx(Ib1,Ib2,Ib3,n2)) );

//           // printf(" max(norm)=%e, minNorm=%e\n",max(fabs(norm)),min(fabs(norm)));
	  
// 	  uDotN=max(fabs(
//               (rx(Ib1,Ib2,Ib3,n1)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("uc"))
// 	      +rx(Ib1,Ib2,Ib3,n2)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("vc")))*norm));


// 	  real normalError;
// 	  normalError=max(fabs(normal(Ib1,Ib2,Ib3,0)-rx(Ib1,Ib2,Ib3,n1)*norm)+
//   	                  fabs(normal(Ib1,Ib2,Ib3,1)-rx(Ib1,Ib2,Ib3,n2)*norm));
	  
//           maxNormalError=max(maxNormalError,normalError);

//           if( uDotN>.1e-5 )
// 	  {
// // 	    display(normal(Ib1,Ib2,Ib3,0),"normal(Ib1,Ib2,Ib3,0)");
// // 	    display(rx(Ib1,Ib2,Ib3,n1)*norm,"rx(Ib1,Ib2,Ib3,n1)*norm");

 
// // 	    display(normal(Ib1,Ib2,Ib3,1),"normal(Ib1,Ib2,Ib3,1)");
// // 	    display(rx(Ib1,Ib2,Ib3,n2)*norm,"rx(Ib1,Ib2,Ib3,n2)*norm");
// 	  }
// 	}
// 	else
// 	{
// 	  uDotN=max(
//              rx(Ib1,Ib2,Ib3,n1)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("uc"))
// 	    +rx(Ib1,Ib2,Ib3,n2)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("vc"))  
// 	    +rx(Ib1,Ib2,Ib3,n3)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("wc")));
// 	}
// 	maxErr2=max(maxErr2,uDotN);


//       }
      
//     }
//   }
//   printf(">>>>>>>>>checkNormalBC: grid %i: max(n.u) = %8.2e, max(grad(r).u)=%8.2e normalError=%8.2e\n",
//        grid,maxErr,maxErr2,maxNormalError);
// }

void
symmetryBC( int grid, MappedGrid & mg, realMappedGridFunction & u, Parameters & parameters );

// {
  
//   Range C=parameters.dbase.get<int >("numberOfComponents");
//   int side,axis;
//   real maxErr=0., maxErr2=0., uDotN, maxNormalError=0.;
//   Index Ib1,Ib2,Ib3, Ig1,Ig2,Ig3;
//   for( side=0; side<=1; side++ )
//   {
//     for( axis=0; axis<mg.numberOfDimensions(); axis++ )
//     {
//       if( mg.boundaryCondition(side,axis)==Parameters::slipWall )
//       {
// 	getGhostIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,-2); // second line in.
// 	getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3, 2); // second ghost line

//         u(Ig1,Ig2,Ig3,C)=u(Ib1,Ib2,Ib3,C);
//       }
//     }
//   }
// }



int OB_MappedGridSolver::
applyBoundaryConditionsCNS(const real & t, realMappedGridFunction & u,
			   realMappedGridFunction & gridVelocity,
                           const int & grid,
			   const int & option /* =-1 */,
			   realMappedGridFunction *puOld /* =NULL */, 
                           realMappedGridFunction *pGridVelocityOld /* =NULL */,
			   const real & dt /* =-1. */ )
// =========================================================================================
// =========================================================================================
{

  // int numSent=getSent();
  // if( parameters.dbase.get<int >("myid")==0 ) printf("******* cnsBC: START: new messages sent=%i\n",numSent);
  checkArrayIDs(" cnsBC: start"); 
  
  

  MappedGrid & mg = *u.getMappedGrid();
  const IntegerArray & boundaryCondition = mg.boundaryCondition();
  bool isRectangular = mg.isRectangular();
  const int numberOfDimensions=mg.numberOfDimensions();
  
  const bool gridIsMoving = parameters.gridIsMoving(grid);
  bool newMovingGridBoundaryConditions=false && gridIsMoving;

  // ** if( parameters.dbase.get<int >("slipWallBoundaryConditionOption")==4 || (gridIsMoving && isRectangular) )
  if( parameters.dbase.get<bool >("alwaysUseCurvilinearBoundaryConditions") )
  { // * for testing use the curvilinear version of the BC's even for rectangular grids  * 
    isRectangular=false;
    mg.update(MappedGrid::THEinverseVertexDerivative | MappedGrid::THEvertex | MappedGrid::THEcenter );
  }
  

  const int & numberOfSpecies  = parameters.dbase.get<int >("numberOfSpecies");
  
//  const bool useOptSlipWall=!gridIsMoving && mg.numberOfDimensions()==2 && numberOfSpecies==0; 
  const bool useOptSlipWall=mg.numberOfDimensions()==2 && numberOfSpecies==0 &&
                        (
			 (parameters.dbase.get<int >("slipWallBoundaryConditionOption")==0  // use opt here too: *wdh* 060220
                                                         && !gridIsMoving)   ||
                         parameters.dbase.get<int >("slipWallBoundaryConditionOption")==3 || 
                         parameters.dbase.get<int >("slipWallBoundaryConditionOption")==4 ) &&
                         parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase; 

  const int & rc = parameters.dbase.get<int >("rc");
  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");
  // const int & pc = parameters.dbase.get<int >("pc");
  const int & tc = parameters.dbase.get<int >("tc");
  const int & sc = parameters.dbase.get<int >("sc");
  Range all;


  typedef Parameters::BoundaryCondition BoundaryCondition;

  const BoundaryCondition & noSlipWall                = Parameters::noSlipWall;
  const BoundaryCondition & slipWall                  = Parameters::slipWall;
  const BoundaryCondition & superSonicOutflow         = Parameters::superSonicOutflow;
  const BoundaryCondition & superSonicInflow          = Parameters::superSonicInflow;
  const BoundaryCondition & subSonicOutflow           = Parameters::subSonicOutflow;
  const BoundaryCondition & subSonicInflow            = Parameters::subSonicInflow;
  const BoundaryCondition & symmetry                  = Parameters::symmetry;
  const BoundaryCondition & inflowWithVelocityGiven   = Parameters::inflowWithVelocityGiven;
  const BoundaryCondition & outflow                   = Parameters::outflow;
  const BoundaryCondition & inflowWithPressureAndTangentialVelocityGiven 
                                                      = Parameters::inflowWithPressureAndTangentialVelocityGiven;
  const BoundaryCondition & dirichletBoundaryCondition= Parameters::dirichletBoundaryCondition;
  const BoundaryCondition & neumannBoundaryCondition  = Parameters::neumannBoundaryCondition;
  const BoundaryCondition & axisymmetric              = Parameters::axisymmetric;
  const BoundaryCondition & farField                  = Parameters::farField;
  
  // make some shorter names for readability
  BCTypes::BCNames 
    dirichlet             = BCTypes::dirichlet,
    neumann               = BCTypes::neumann,
    mixed                 = BCTypes::mixed,
    vectorSymmetry        = BCTypes::vectorSymmetry,
    extrapolate           = BCTypes::extrapolate,
    normalComponent       = BCTypes::normalComponent,
    evenSymmetry          = BCTypes::evenSymmetry;
  

  //                  aDotU                 = BCTypes::aDotU,
  //                  generalizedDivergence = BCTypes::generalizedDivergence,
  //                  tangentialComponent   = BCTypes::tangentialComponent,
  //                  allBoundaries         = BCTypes::allBoundaries; 

  // First determine which boundary conditions we have
  bool assignAxisymmetric=false;
  bool assignDirichletBoundaryCondition=false;
  bool assignNeumannBoundaryCondition=false;
  bool assignSuperSonicInflow=false;
  bool assignSuperSonicOutflow=false;
  bool assignSubSonicInflow=false;
  bool assignSubSonicOutflow=false;
  bool assignSlipWall=false;
  bool assignNoSlipWall=false;
  bool assignSymmetry=false;
  bool assignOutflow=false;
  bool assignFarField=false;
  
  int side,axis;
  for( axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
    for( side=0; side<=1; side++ )
    {
      int bc=boundaryCondition(side,axis);
      switch (bc)
      {
      case 0 : break;
      case -1: break;
      case Parameters::slipWall:           assignSlipWall=true; break;
      case Parameters::noSlipWall :        assignNoSlipWall=true; break;
      case Parameters::superSonicInflow:   assignSuperSonicInflow=true; break;
      case Parameters::superSonicOutflow:  assignSuperSonicOutflow=true; break;
      case Parameters::subSonicInflow:     assignSubSonicInflow=true; break;
      case Parameters::subSonicOutflow:    assignSubSonicOutflow=true; break;
      case Parameters::symmetry :          assignSymmetry=true; break;
      case Parameters::axisymmetric:       assignAxisymmetric=true; break;
      case Parameters::dirichletBoundaryCondition:  assignDirichletBoundaryCondition=true; break;
      case Parameters::neumannBoundaryCondition:  assignNeumannBoundaryCondition=true; break;
      case Parameters::outflow:             assignOutflow=true; break;
      case Parameters::farField:            assignFarField=true; break;
      default: 
        printf("cnsBC:ERROR: unknown boundary condition =%i on grid %i, side=%i, axis=%i\n",bc,grid,side,axis);
        throw "error";
      break;
      }
    }
  }
  	
  if( !isRectangular )
  {
    if( assignNoSlipWall || ( assignSlipWall && !useOptSlipWall) || assignSuperSonicOutflow )
    {
      mg.update(MappedGrid::THEvertexBoundaryNormal );  // *wdh* 060213
    } 
  }
  
  // ----------------------------------------------------------------------------------------------------
  // For testing -- evaluate the known solution ----

  realArray *uKnownPointer=NULL;
  if( parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution")!=Parameters::noKnownSolution )
  {
    int extra=2;
    Index I1,I2,I3;
    getIndex(mg.gridIndexRange(),I1,I2,I3,extra);  // **************** fix this -- only evaluate near boundaries --

    // NOTE: This next call will not recompute the supersonic expanding flow if it has already been computed
    uKnownPointer = &parameters.getKnownSolution( t,grid,I1,I2,I3 );
  }
  realArray & uKnown = uKnownPointer!=NULL ? *uKnownPointer : u;

  // ----------------------------------------------------------------------------------------------------

  const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");

  // **************************************************************************
  // ORDERING OF BOUNDARY CONDITIONS:
  //  (1) Apply Dirichlet like BC's first, these do not depend on the other BC's
  //  (2) Apply Neumann and extrap BC's second
  //  (3) Apply symmetry-like conditions
  // **************************************************************************

  int numVelocityComponents=parameters.dbase.get<int >("numberOfDimensions");
  if( parameters.dbase.get<bool >("axisymmetricWithSwirl") ) numVelocityComponents=3;

  Range C(0,parameters.dbase.get<int >("numberOfComponents")-1);  // ***** is this correct ******
  Range V = Range(uc,uc+numVelocityComponents-1);
  Range S(sc,sc+numberOfSpecies-1);
  BoundaryConditionParameters extrapParams;
  BoundaryConditionParameters bcParams;

  // extrapolationOption== polynomialExtrapolation or limitedExtrapolation
  extrapParams.extrapolationOption=bcParams.extrapolationOption=parameters.dbase.get<BoundaryConditionParameters::ExtrapolationOptionEnum >("extrapolationOption");

  // Multiphase flow:  We have two sets of velocities to deal with 
//   int rs=0, us=1, vs=2, ws=3, ts=us+numberOfDimensions;
//   int rg=ts+1, ug=rg+1, vg=ug+1, wg=vg+1, tg=ug+numberOfDimensions;

  int numberOfPhases=1;
  if( parameters.dbase.get<Parameters::PDE >("pde")==Parameters::compressibleMultiphase )
  {
    numberOfPhases=2;           // assume 2 phases for now
  }
  
  const int maxNumberOfPhases=2;  // assume a max of 2 phases for now
  int mpRho[maxNumberOfPhases];  // index's for desnities
  int mpT[maxNumberOfPhases];    // index's for Temperature
  Range mpV[maxNumberOfPhases];  // velocity components for multiphase flow
  for( int m=0; m<numberOfPhases; m++ )
  {
    mpRho[m]=0+m*(numberOfDimensions+2);
    mpV[m]  =Range(mpRho[m]+1,mpRho[m]+1+numberOfDimensions-1);
    mpT[m]  =mpRho[m] + 1+numberOfDimensions;
  }
  Range mpA(mpT[numberOfPhases-1]+1,mpT[numberOfPhases-1]+numberOfPhases-1);  // fractions of phases

  if( false && parameters.dbase.get<Parameters::PDE >("pde")==Parameters::compressibleMultiphase )
  {

    u.applyBoundaryCondition(C,neumann,BCTypes::allBoundaries,0.,t);

    printf(">>> applyBoundaryConditionsCNS:compressibleMultiphase: Apply neumann BC on all boundaries <<<<\n");

    return 0;
  }

  if( debug() & 64 )
  {
    fprintf(parameters.dbase.get<FILE* >("pDebugFile")," ======Entering cnsBC t=%9.3e ========\n",t);
      
  }
 
  if( debug() & 4 || debug() & 64 )
  {
    ::display(u,sPrintF("cnsBC: u at start, t=%8.2e",t),parameters.dbase.get<FILE* >("pDebugFile"),"%9.2e ");
  }


  const int numberOfGhostPointsNeeded = parameters.numberOfGhostPointsNeeded();
  
  // *******************************************************************
  // ****************(1) DIRICHLET LIKE CONDITIONS *********************
  // *******************************************************************


  // the dirichletBoundaryCondition is for testing TZ flow.
  if( assignDirichletBoundaryCondition )
  {
    if( parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution")==Parameters::supersonicFlowInAnExpandingChannel )
    {
      bcParams.extraInTangentialDirections=2;
      bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::arrayForcing); // *wdh* 050528 -- new way

      for( int line=0; line<=numberOfGhostPointsNeeded; line++ )
      {
	bcParams.lineToAssign=line;
        u.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,uKnown,t,bcParams);
      }
      
      bcParams.lineToAssign=0;  // reset
      bcParams.extraInTangentialDirections=0;
      bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::unSpecifiedForcing);

      if( debug() & 64 )
      {
	::display(uKnown,"cnsBC: supersonicFlowInAnExpandingChannel: uKnown",parameters.dbase.get<FILE* >("pDebugFile"),"%4.2f ");
	::display(u,"cnsBC: after assign dirichlet BC (supersonicFlowInAnExpandingChannel)",parameters.dbase.get<FILE* >("pDebugFile"),"%4.2f ");
      }

    }
    else
    {
      // u.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,0.,t);
      
      bcParams.extraInTangentialDirections=2;  // *wdh* 050611 -- assign extended boundary
      for( int line=0; line<=2; line++ )
      {
        bcParams.lineToAssign=line;
        u.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,0.,t,bcParams);
      }
      bcParams.lineToAssign=0; // reset
      bcParams.extraInTangentialDirections=0;
    }

    // do rest later 
  }
  
  // the neumannBoundaryCondition is for testing TZ flow.
  if( assignNeumannBoundaryCondition )
  {
    if( parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution")==Parameters::supersonicFlowInAnExpandingChannel )
    {

      bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::arrayForcing); // *wdh* 050528 -- new way
      u.applyBoundaryCondition(C,neumann,neumannBoundaryCondition,uKnown,t,bcParams);
      bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::unSpecifiedForcing);

      if( debug() & 64 )
      {
	::display(uKnown,"cnsBC: supersonicFlowInAnExpandingChannel: uKnown",parameters.dbase.get<FILE* >("pDebugFile"),"%4.2f ");
	::display(u,"cnsBC: after assign neumann BC (supersonicFlowInAnExpandingChannel)",parameters.dbase.get<FILE* >("pDebugFile"),"%4.2f ");
      }

    }
    else
    {
      // u.applyBoundaryCondition(C,neumann,neumannBoundaryCondition,0.,t);
      
      u.applyBoundaryCondition(C,neumann,neumannBoundaryCondition,0.,t,bcParams);

    }

    if( numberOfGhostPointsNeeded>=2 )
    {
      extrapParams.ghostLineToAssign=2;
      extrapParams.orderOfExtrapolation=3; 
      u.applyBoundaryCondition(C,extrapolate,neumannBoundaryCondition,0.,t,extrapParams);
    }
    
    // do rest later 
  }
  

 
  if( assignSuperSonicInflow ) // *wdh* 030518 : moved first
  { 
    // superSonicInflow:
    // (1) set rho=, u=, v=, [w=], T=
    // (2) extrapolate all components 
    if( debug() & 64 )
    {
      ::display(u,"cnsBC: before supersonic inflow: uu",parameters.dbase.get<FILE* >("pDebugFile"),"%6.2f ");
    }

    if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
    {
      u.applyBoundaryCondition(V,dirichlet,superSonicInflow,bcData,t,
			       Overture::defaultBoundaryConditionParameters(),grid);
      u.applyBoundaryCondition(rc,dirichlet,superSonicInflow,bcData,t,
			       Overture::defaultBoundaryConditionParameters(),grid);
      u.applyBoundaryCondition(tc,dirichlet,superSonicInflow,bcData,t,
			       Overture::defaultBoundaryConditionParameters(),grid);
      
    }
    else
    {
      u.applyBoundaryCondition(C,dirichlet,superSonicInflow,bcData,t,
			       Overture::defaultBoundaryConditionParameters(),grid);
    }
    
    if( numberOfSpecies>0 )
      u.applyBoundaryCondition(S,dirichlet,superSonicInflow,bcData,pBoundaryData,t,
 			     Overture::defaultBoundaryConditionParameters(),grid);

    if( debug() & 64 )
    {
      ::display(u,"cnsBC: after supersonic inflow: uu",parameters.dbase.get<FILE* >("pDebugFile"),"%6.2f ");
    }

    // do rest later
  }

  checkArrayIDs(" cnsBC: before slipWall"); 


  if( assignSlipWall )
  {
    // slipWall:
    // (1) set n.(u,v[,w])=
    // (2) apply a symmetry condition
    if( debug() & 64 )
    {   
      printf(" cnsBC:before slipWall: u dimensions: [%i:%i][%i:%i]\n",
	     u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1));
    }

    if( gridIsMoving )
    {
      if( (debug() & 64) )
      {
	::display(gridVelocity,sPrintF("cnsBC: grid Velocity for slip wall t=%8.2e",t),parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");
	::display(u,sPrintF("cnsBC: u BEFORE slip-wall normalComponent"),parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");
	
      }
      if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
      {
	u.applyBoundaryCondition(V,normalComponent,slipWall,gridVelocity,t);
      }
      else
      {
	for( int m=0; m<numberOfPhases; m++ )
	{
	  u.applyBoundaryCondition(mpV[m],normalComponent,slipWall,gridVelocity,t);
	}
      }
      
      if( (debug() & 64) )
      {
	::display(u,sPrintF("cnsBC: u AFTER slip-wall normalComponent"),parameters.dbase.get<FILE* >("debugFile"),"%5.2f ");
	
      }
    }
    else
    {
      checkArrayIDs(" cnsBC: slipWall before normalComponent"); 
      if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
      {
        u.applyBoundaryCondition(V,normalComponent,slipWall,0.,t);
      }
      else
      {
	for( int m=0; m<numberOfPhases; m++ )
	{
	  u.applyBoundaryCondition(mpV[m],normalComponent,slipWall,0.,t);
	}
      }

      checkArrayIDs(" cnsBC: slipWall after normalComponent"); 
    }
    
    // do rest of slip wall later
  
    if( debug() & 4 || debug() & 64 )
    {
      printf(" cnsBC:after slipWall: u dimensions: [%i:%i][%i:%i]\n",
	     u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1));

      // ::display(u,"cnsBC: u after slipWall, normalComponent",parameters.dbase.get<FILE* >("pDebugFile"),"%6.2f ");
      ::display(u,sPrintF("cnsBC: u after slipWall, normalComponent, t=%8.2e",t),parameters.dbase.get<FILE* >("pDebugFile"),"%9.2e ");
      fflush(parameters.dbase.get<FILE* >("pDebugFile"));
      
    }


  }

  checkArrayIDs(" cnsBC: after slipWall (I)"); 

  

  // numSent=getSent();
  // if( parameters.dbase.get<int >("myid")==0 ) printf("******* cnsBC: After slipWall (I): new messages sent=%i\n",numSent);


  if( assignAxisymmetric )
  {

    // Assigm the dirichlet part of the axisymmetric BC here -- do the rest later
    if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
    {
      // radial axis=0:  x=0 is axis of symmetry, u=radial, v=axial, w=theta
      // radial axis=1:  y=0 is axis of symmetry, v=radial, u=axial, w=theta
      const int ur = uc+parameters.dbase.get<int >("radialAxis"); // radial component of the velocity

      u.applyBoundaryCondition(ur,dirichlet,axisymmetric,0.,t);
      if( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
        u.applyBoundaryCondition(wc,dirichlet,axisymmetric,0.,t);
    }
    else
    {
      for( int m=0; m<numberOfPhases; m++ )
      {
        const int ur =mpV[m].getBase()+parameters.dbase.get<int >("radialAxis"); // radial component of the velocity
	u.applyBoundaryCondition(ur,dirichlet,axisymmetric,0.,t); // apply to each "v" component
	if( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
	  u.applyBoundaryCondition(mpV[m].getBase()+2,dirichlet,axisymmetric,0.,t);  // apply to "w"
      }
    }
    
  }

     u.applyBoundaryCondition(S,dirichlet,superSonicInflow,bcData,pBoundaryData,t,
 			     Overture::defaultBoundaryConditionParameters(),grid);

  bool adiabaticNoSlipWall=false;
  const  int nc=parameters.dbase.get<int >("numberOfComponents");

  if( assignNoSlipWall )
  {
    // noSlipWall
    // noSlipWall:
    // (1) set rho.n=, (u,v,w)= T=
    // (2) extrapolate (u,v,w,T)

    // ::display(parameters.bcData(all,all,all,grid),"bcData(all,all,all,grid)");
    

    for( int side=0; side<=1; side++ )
    {
      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
	if( parameters.bcData(nc+1,side,axis,grid)!=0. )
	{
	  adiabaticNoSlipWall=true;
// 	  printf("++++noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : Mixed BC for T: %3.2f*T+%3.2f*T.n=\n",
// 		 grid,side,axis,parameters.bcData(nc,side,axis,grid),
// 		 parameters.bcData(nc+1,side,axis,grid));
	}
      }
    }
      
    // Here is an example of how to apply a BC to a particular side:
    // u.applyBoundaryCondition(V,neumann,BCTypes::boundary(side,axis),0.,t,bcParams);

    if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
    {
      // *wdh* 051128 u.applyBoundaryCondition(tc,dirichlet, noSlipWall,bcData,t,
      //              Overture::defaultBoundaryConditionParameters(),grid);

      if( !adiabaticNoSlipWall )
      {
	u.applyBoundaryCondition(tc,dirichlet, noSlipWall,bcData,pBoundaryData,t,
				 Overture::defaultBoundaryConditionParameters(),grid);
      }
      else
      {
        // Some noSlipWall are adiabatic: A Neumann or Mixed BC on T
	for( int side=0; side<=1; side++ )
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	  {
            if( mg.boundaryCondition(side,axis)==noSlipWall )
	    {
	      if( parameters.bcData(nc+1,side,axis,grid)==0. ) // coeff of T.n 
	      {
		// Dirichlet
		u.applyBoundaryCondition(tc,dirichlet,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
					 Overture::defaultBoundaryConditionParameters(),grid);
	      }
	      else
	      {
                // Mixed or Neumann -- this case is done below
		
	      }
	    
	    }
	  }
	}
      }
      

      if( gridIsMoving )
	u.applyBoundaryCondition(V,dirichlet,noSlipWall,gridVelocity,t);
      else
      {

	// *wdh* 051128 u.applyBoundaryCondition(V,dirichlet,noSlipWall,bcData,t,
        //     Overture::defaultBoundaryConditionParameters(),grid);
        u.applyBoundaryCondition(V,dirichlet,noSlipWall,bcData,pBoundaryData,t,
 			     Overture::defaultBoundaryConditionParameters(),grid);
      }
      
    }
    else
    {
      assert( !adiabaticNoSlipWall );  // finish this case

      for( int m=0; m<numberOfPhases; m++ )
      {
	// *wdh* 051128 u.applyBoundaryCondition(mpT[m],dirichlet, noSlipWall,bcData,t,
        // *wdh* 051128                          Overture::defaultBoundaryConditionParameters(),grid);
        u.applyBoundaryCondition(mpT[m],dirichlet, noSlipWall,bcData,pBoundaryData,t,
 			         Overture::defaultBoundaryConditionParameters(),grid);

	if( gridIsMoving )
	  u.applyBoundaryCondition(mpV[m],dirichlet,noSlipWall,gridVelocity,t);
	else
	{
	  // *wdh* 051128 u.applyBoundaryCondition(mpV[m],dirichlet,noSlipWall,bcData,t,
          // *wdh* 051128                          Overture::defaultBoundaryConditionParameters(),grid);
          u.applyBoundaryCondition(mpV[m],dirichlet,noSlipWall,bcData,pBoundaryData,t,
 			     Overture::defaultBoundaryConditionParameters(),grid);
	}
	
      }
    }
    
    if( debug() & 64 )
    {
      char buff[80];
      ::display(u,sPrintF(buff,"cnsBC:After assignNoSlipWall (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%7.4f ");
    }

    // Set the Temperature on boundaries where a surface equation is applied -- 
    //    over-writing the above dirichlet condition
    const int numberOfSurfaceEquationFaces=surfaceEquation.faceList.size();
    for( int face=0; face<numberOfSurfaceEquationFaces; face++ )
    {
      const SurfaceEquationFace & surfaceEquationFace = surfaceEquation.faceList[face];
      if( surfaceEquationFace.grid==grid )
      {
	// Solve a surface equation on this face:
	const int side=surfaceEquationFace.side;
	const int axis=surfaceEquationFace.axis;

	const int sec = parameters.dbase.get<int >("sec"); 
	assert( sec>=0 );
	const real kThermal=surfaceEquation.kThermal;

	Index Ib1,Ib2,Ib3;
	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

        if( debug() & 4 )
	  printf("cnsBC: Setting T to equal Tb on noSlipWall: (grid,side,axis)=(%i,%i,%i) t=%8.2e\n",
		 grid,side,axis,t);
	
        u(Ib1,Ib2,Ib3,tc)=u(Ib1,Ib2,Ib3,sec);


      }
    }
  

    // do rest later

    checkArrayIDs(" cnsBC: after assignNoSlipWall"); 

  }

  if( assignSubSonicInflow )
  {
    // subSonicInflow:
    // ****** fix this ***********
    if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
    {
      u.applyBoundaryCondition(V,dirichlet,subSonicInflow,bcData,pBoundaryData,t,
			       Overture::defaultBoundaryConditionParameters(),grid);
      
    }
    else
    {
      for( int m=0; m<numberOfPhases; m++ )
      {
	u.applyBoundaryCondition(mpV[m],dirichlet,subSonicInflow,bcData,pBoundaryData,t,
				 Overture::defaultBoundaryConditionParameters(),grid);
      }
      u.applyBoundaryCondition(mpA,dirichlet,subSonicInflow,bcData,pBoundaryData,t,
 			       Overture::defaultBoundaryConditionParameters(),grid);   
    }   
    if( numberOfSpecies>0 )
    {
      u.applyBoundaryCondition(S,dirichlet,subSonicInflow,bcData,pBoundaryData,t,
 			       Overture::defaultBoundaryConditionParameters(),grid);
    }
    // do rest later
    checkArrayIDs(" cnsBC: after assignSubSonicInflow"); 

  }

  // *******************************************************************8
  // ****************(2) Neumann and Extrapolation *************************8
  // *******************************************************************8


  // ** get some decent values at all ghost points // ***** can we avoid this ??????
  if( debug() & 64 )
  {   
    printf(" cnsBC:before extrap: u dimensions: [%i:%i][%i:%i]\n",
	   u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1));
    char buff[80];
    ::display(u,sPrintF(buff,"cnsBC:Before extrap (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%7.4f ");
  }

  u.applyBoundaryCondition(C,extrapolate,BCTypes::allBoundaries,0.,t);

  checkArrayIDs(" cnsBC: after extrapolate"); 

  if( debug() & 64 )
  {   
    printf(" cnsBC:after extrap: u dimensions: [%i:%i][%i:%i]\n",
	   u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1));
    char buff[80];
    ::display(u,sPrintF(buff,"cnsBC:After extrap (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%7.4f ");
  }

  if( assignDirichletBoundaryCondition )
  {
    if( parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution")==Parameters::supersonicFlowInAnExpandingChannel )//||
    {
      bcParams.lineToAssign=1;
      bcParams.extraInTangentialDirections=2;
      u.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,uKnown,t,bcParams);
      bcParams.lineToAssign=0;
      bcParams.extraInTangentialDirections=0;
//        extrapParams.ghostLineToAssign=1;
//        extrapParams.orderOfExtrapolation=2; 
//        u.applyBoundaryCondition(C,extrapolate,dirichletBoundaryCondition,0.,t,extrapParams);
    }
    else
    {
      bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::arrayForcing); // *wdh* 050528 -- new way

      bcParams.extraInTangentialDirections=2;
      for( int line=1; line<=2; line++ )
      {
	bcParams.lineToAssign=line;
	u.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,uKnown,t,bcParams);
      }
      bcParams.lineToAssign=0; // reset
      bcParams.extraInTangentialDirections=0;
      bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::unSpecifiedForcing);

    }

  }

  if( assignSuperSonicOutflow )
  {
    if( debug() & 64 )
    {
      char buff[80];
      ::display(u,sPrintF(buff,"cnsBC:Before supersonic outflow - neumann (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%7.4f ");
    }
    // superSonicOutflow:
    // (1) set rho.n=, u.n=, v.n=, [w.n=], T.n=
    u.applyBoundaryCondition(C,neumann,superSonicOutflow,0.,t);
    //     extrapParams.orderOfExtrapolation=2;
    //     u.applyBoundaryCondition(C,extrapolate,superSonicOutflow,0.,t,extrapParams);
    if( debug() & 64 )
    {
      char buff[80];
      ::display(u,sPrintF(buff,"cnsBC:After supersonic outflow - neumann (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%7.4f ");
    }
  }

  if( assignSuperSonicInflow ) 
  {
    // finish superSonicInflow
    extrapParams.ghostLineToAssign=1;
    extrapParams.orderOfExtrapolation=1; 
    u.applyBoundaryCondition(C,extrapolate,superSonicInflow,0.,t,extrapParams);

    if( debug() & 64 )
    {
      char buff[80];
      ::display(u,sPrintF(buff,"cnsBC:After extrap for supersonic inflow (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%7.4f ");
    }

  }

  if( assignNoSlipWall )
  { // finish noSlipWall
    if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
    {
      //u.applyBoundaryCondition(rc,extrapolate,   noSlipWall,0.,t,extrapParams);
      extrapParams.orderOfExtrapolation=2;
      //      u.applyBoundaryCondition(rc,neumann,   noSlipWall,0.,t);
      //      u.applyBoundaryCondition(tc,neumann,noSlipWall,0.,t,extrapParams);
      //      u.applyBoundaryCondition(V,extrapolate,noSlipWall,0.,t,extrapParams);

      if (  !parameters.dbase.get<bool >("twilightZoneFlow") && (parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit||parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateNewton) )
	{
	  int ipar[20];
	  real rpar[20];
	  ipar[0]=parameters.dbase.get<int >("rc");
	  ipar[1]=parameters.dbase.get<int >("uc");
	  ipar[2]=parameters.dbase.get<int >("vc");
	  ipar[3]=parameters.dbase.get<int >("wc");
	  ipar[4]=parameters.dbase.get<int >("tc");

	  ipar[7]=grid;

	  ipar[8]=isRectangular ? 0 : 1;
	  
	  ipar[12]=parameters.isAxisymmetric();
	  
	  ipar[15]=parameters.dbase.get<int >("debug");
	  ipar[18]=parameters.dbase.get<int >("radialAxis");  // =axis1 if x=0 is axis of cylindrical symmetry, =axis2 if y=0 is..

	  ipar[19]=parameters.dbase.get<bool >("axisymmetricWithSwirl");

	  /*	  rpar[0]=dx[0];
	  rpar[1]=dx[1];
	  rpar[2]=dx[2]; 
	  rpar[3]=mg.gridSpacing(0); 
	  rpar[4]=mg.gridSpacing(1); 
	  rpar[5]=mg.gridSpacing(2); 
	  rpar[6]=t;  
	  rpar[7]=dt; 
	  rpar[8]=REAL_MIN*100.; 
	  rpar[9]=parameters.dbase.get<real >("gamma"); */
	  mg.update(MappedGrid::THEinverseVertexDerivative | MappedGrid::THEcenter | 
		    MappedGrid::THEcenterJacobian);	  
	  real *px, *prsxy, *prdet,*pu,*pdet;

#ifdef USE_PPP
	  realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
  	  px = mg.center().getLocalArray().getDataPointer();
	  prsxy = mg.inverseVertexDerivative().getLocalArray().getDataPointer();
	  pdet = mg.centerJacobian().getLocalArray().getDataPointer();
	  pu = u.getLocalArray().getDataPointer();
#else
	  realSerialArray &uLocal = u;
  	  px = mg.center().getDataPointer();
	  prsxy = mg.inverseVertexDerivative().getDataPointer();
	  pdet = mg.centerJacobian().getDataPointer();
	  pu = u.getDataPointer();
#endif
	  IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
	  getLocalBoundsAndBoundaryConditions( u,gidLocal,dimLocal,bcLocal ); 
	  cnsNoSlipWallBC(mg.numberOfDimensions(),
			  uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
			  uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
			  pu,px,pdet,prsxy,ipar,rpar,gidLocal.getDataPointer(),bcLocal.getDataPointer());

	} else
	{
	  u.applyBoundaryCondition(rc,neumann,   noSlipWall,0.,t);
	  u.applyBoundaryCondition(V,extrapolate,noSlipWall,0.,t);
	  u.applyBoundaryCondition(tc,extrapolate,noSlipWall,0.,t);
	}
// =======
//       u.applyBoundaryCondition(rc,neumann,   noSlipWall,0.,t);
//       u.applyBoundaryCondition(V,extrapolate,noSlipWall,0.,t);

//       if( !adiabaticNoSlipWall )
//       {
//         u.applyBoundaryCondition(tc,extrapolate,noSlipWall,0.,t);
//       }
//       else
//       {

//         // Some noSlipWall are adiabatic: A Neumann or Mixed BC on T
// 	bcParams.a.redim(3);
// 	bcParams.a=0.;
// 	for( int side=0; side<=1; side++ )
// 	{
// 	  for( int axis=0; axis<numberOfDimensions; axis++ )
// 	  {
//             if( mg.boundaryCondition(side,axis)==noSlipWall )
// 	    {
// 	      if( parameters.bcData(nc+1,side,axis,grid)==0. ) // coeff of T.n 
// 	      {
// 		// Dirichlet BC on T -- extrap the ghost points:
//                  u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
// 	      }
// 	      else
// 	      {
//                 // Mixed BC or Neumann
//                 real a0=parameters.bcData(nc  ,side,axis,grid);
// 		real a1=parameters.bcData(nc+1,side,axis,grid);
// 		if( a0==0. && a1==1. )
// 		{
// 		  // printf("++++noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : apply neumannBC\n",
// 	  	  // 	 grid,side,axis);

// 		  u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),0.,t);
// // 		  u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
// // 					   Overture::defaultBoundaryConditionParameters(),grid);
// 		}
// 		else
// 		{
// 		  // printf("++++noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
//                   //        "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f\n",
// 		  // 	 grid,side,axis,a0,a1,parameters.bcData(nc+2,side,axis,grid));

// 		  bcParams.a(0)=a0;
// 		  bcParams.a(1)=a1;
// 		  bcParams.a(2)=parameters.bcData(nc+2,side,axis,grid);
// 		  u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
// 					   bcParams,grid);
// 		}
		
// 	      }
	    
// 	    }
// 	  }
// 	}

//       }
      
// >>>>>>> 1.37
    }
    else
    {
      assert( !adiabaticNoSlipWall );  // finish this case
      
      for( int m=0; m<numberOfPhases; m++ )
      {
	u.applyBoundaryCondition(mpRho[m],neumann,   noSlipWall,0.,t);
	u.applyBoundaryCondition(mpV[m],extrapolate,noSlipWall,0.,t);
	u.applyBoundaryCondition(mpT[m],extrapolate,noSlipWall,0.,t);
      }
      u.applyBoundaryCondition(mpA,neumann,noSlipWall,0.,t);  // phase fractions
    }
    
    if( numberOfSpecies>0 )
      u.applyBoundaryCondition(S,neumann,noSlipWall,0.,t); 
    
    checkArrayIDs(" cnsBC: after assignNoSlipWall"); 
    
    if( debug() & 64 )
      {
	char buff[80];
	::display(u,sPrintF(buff,"cnsBC:After finish no slip wall (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%7.4f ");
      }
  }
  
  if( assignSubSonicInflow )
  {
    // finish subSonicInflow:
    bcParams.a.redim(3);
    bcParams.a=0.;
    bcParams.a(0)=1.;  
    bcParams.a(1)=1.;  // .1; // 10.; // 1. *wdh* 050627 -- try this
    bcParams.a(2)=1.;

    if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
    {
      // u.applyBoundaryCondition(rc,extrapolate,subSonicInflow,0.,t);
      u.applyBoundaryCondition(rc,mixed,subSonicInflow,bcData,t,bcParams,grid);
      u.applyBoundaryCondition(V,extrapolate, subSonicInflow,0.,t);
      // u.applyBoundaryCondition(tc,neumann,    subSonicInflow,0.,t);
      u.applyBoundaryCondition(tc,mixed,subSonicInflow,bcData,t,bcParams,grid);
    }
    else
    {
      for( int m=0; m<numberOfPhases; m++ )
      {
	u.applyBoundaryCondition(mpRho[m],mixed,subSonicInflow,bcData,t,bcParams,grid);
	u.applyBoundaryCondition(mpV[m],extrapolate, subSonicInflow,0.,t);
	u.applyBoundaryCondition(mpT[m],mixed,subSonicInflow,bcData,t,bcParams,grid);
      }
      u.applyBoundaryCondition(mpA,neumann,subSonicInflow,0.,t); // phase fractions
    }
    
    if( numberOfSpecies>0 )
      u.applyBoundaryCondition(S,neumann,subSonicInflow,0.,t); 

    checkArrayIDs(" cnsBC: after assignSubSonicInflow"); 

  }

  if( assignSubSonicOutflow )
  {
    // subSonicOutflow:
    // ****** fix this ***********

    // (1) extrapolate (r,u,v,[w,]), p
    // (2) set alpha T + beta T.n =  
  
// *wdh* 040929    u.applyBoundaryCondition(rc,extrapolate,subSonicOutflow,0.,t);
// *wdh* 040929     u.applyBoundaryCondition(V,extrapolate,subSonicOutflow,0.,t);

    if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
    {
      // *wdh* 040929  use neumann BC's -- these are more stable
      u.applyBoundaryCondition(rc,neumann,subSonicOutflow,0.,t);
      u.applyBoundaryCondition(V,neumann,subSonicOutflow,0.,t);

      // fill in the coefficients for the mixed derivative BC:
      bcParams.a.redim(3,2,bcData.getLength(2),grid+1); bcParams.a=0.;
      for( int i=0; i<=1; i++ )
	bcParams.a(i,all,all,grid)=bcData(tc+parameters.dbase.get<int >("numberOfComponents")*(i+1),all,all,grid);
      u.applyBoundaryCondition(tc,mixed,subSonicOutflow,bcData,t,bcParams,grid);
    }
    else
    {
      for( int m=0; m<numberOfPhases; m++ )
      {
	u.applyBoundaryCondition(mpRho[m],neumann,subSonicOutflow,0.,t);
	u.applyBoundaryCondition(mpV[m],neumann,subSonicOutflow,0.,t);

	// fill in the coefficients for the mixed derivative BC:
	bcParams.a.redim(3,2,bcData.getLength(2),grid+1); bcParams.a=0.;
	for( int i=0; i<=1; i++ )
	  bcParams.a(i,all,all,grid)=bcData(mpT[m]+parameters.dbase.get<int >("numberOfComponents")*(i+1),all,all,grid);
	u.applyBoundaryCondition(mpT[m],mixed,subSonicOutflow,bcData,t,bcParams,grid);
      }
      u.applyBoundaryCondition(mpA,neumann,subSonicOutflow,0.,t);
    }
    
    if( numberOfSpecies>0 )
      u.applyBoundaryCondition(S,neumann,subSonicOutflow,0.,t); 

    checkArrayIDs(" cnsBC: after assignSubSonicOutflow"); 

  }
  
  if( outflow )
  {
    // extrapolate all variables
    extrapParams.orderOfExtrapolation=2;
    u.applyBoundaryCondition(C,extrapolate,outflow,0.,t,extrapParams);
//     u.applyBoundaryCondition(rc,extrapolate,outflow,0.,t,extrapParams);
//     u.applyBoundaryCondition(tc,extrapolate,outflow,0.,t,extrapParams);

//     u.applyBoundaryCondition(V,extrapolate,outflow,0.,t,extrapParams);
//     u.applyBoundaryCondition(S,extrapolate,outflow,0.,t,extrapParams); 

    // u.applyBoundaryCondition(V,neumann,outflow,0.,t);
    // u.applyBoundaryCondition(S,neumann,outflow,0.,t); 
  }
  
  checkArrayIDs(" cnsBC: after outflow"); 


//   if( (parameters.dbase.get<Parameters::PDEVariation >("pdeVariation")==Parameters::conservativeWithArtificialDissipation &&
//       parameters.dbase.get<real >("av4")!=0.) || parameters.dbase.get<Parameters::PDEVariation >("pdeVariation")==Parameters::conservativeGodunov  )
  if( numberOfGhostPointsNeeded>=2 )
  {
    // ***********************************************
    // ***** assign values on the 2nd ghost line *****
    // ***********************************************
    extrapParams.ghostLineToAssign=2;

    if( debug() & 64 )
    {
      char buff[80];
      ::display(u,sPrintF(buff,"cnsBC:Before assign 2nd-ghost line (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%5.2f ");
    }

    if( true || parameters.dbase.get<Parameters::PDEVariation >("pdeVariation")==Parameters::conservativeGodunov )
    {
      // we need to assign values at the second ghostline for all boundaries

      // for Godunov only extrap to lower order to avoid negative densities
      extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfExtrapolationForSecondGhostLine");  // 2; // 1;

      if( !useOptSlipWall && assignSlipWall  )
      {
        // No:  moving grids are done here for now -- useOptSlipWall is only valid for non-moving, 2D

        // on a slip wall we assign the 2nd ghost line by symmetry
	if( !newMovingGridBoundaryConditions )
	{
	  if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
	  {
	    u.applyBoundaryCondition(V,vectorSymmetry, slipWall,0.,t,extrapParams); // ****** 010629
	    u.applyBoundaryCondition(rc,evenSymmetry,slipWall,0.,t,extrapParams);
	    u.applyBoundaryCondition(tc,evenSymmetry,slipWall,0.,t,extrapParams);
	  }
	  else
	  {
	    for( int m=0; m<numberOfPhases; m++ )
	    {
	      u.applyBoundaryCondition(mpV[m],vectorSymmetry, slipWall,0.,t,extrapParams); 
              // *wdh* 050520 -- do this for testing:
	      // u.applyBoundaryCondition(mpV[m],extrapolate, slipWall,0.,t,extrapParams);     
	      u.applyBoundaryCondition(mpRho[m],evenSymmetry,slipWall,0.,t,extrapParams);
	      u.applyBoundaryCondition(mpT[m],evenSymmetry,slipWall,0.,t,extrapParams);
	    }
            u.applyBoundaryCondition(mpA,evenSymmetry,slipWall,0.,t,extrapParams);
	  }
	  if( numberOfSpecies>0 )
	    u.applyBoundaryCondition(S,evenSymmetry,slipWall,0.,t,extrapParams);
	}
	else
	{
	  extrapParams.orderOfExtrapolation=3;
	  if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
	  {
	    // u.applyBoundaryCondition(V,vectorSymmetry, slipWall,0.,t,extrapParams); // ****** 010629
	    u.applyBoundaryCondition(V,extrapolate, slipWall,0.,t,extrapParams); // ****** 010629
	    u.applyBoundaryCondition(rc,extrapolate,slipWall,0.,t,extrapParams);
	    u.applyBoundaryCondition(tc,extrapolate,slipWall,0.,t,extrapParams);
	  }
	  else
	  {
	    for( int m=0; m<numberOfPhases; m++ )
	    {
	      // u.applyBoundaryCondition(mpV[m],vectorSymmetry, slipWall,0.,t,extrapParams); // ****** 010629
	      u.applyBoundaryCondition(mpV[m],extrapolate, slipWall,0.,t,extrapParams); // ****** 010629
	      u.applyBoundaryCondition(mpRho[m],extrapolate,slipWall,0.,t,extrapParams);
	      u.applyBoundaryCondition(mpT[m],extrapolate,slipWall,0.,t,extrapParams);
	    }
   	    u.applyBoundaryCondition(mpA,extrapolate,slipWall,0.,t,extrapParams);
	  }
	  if( numberOfSpecies>0 )
	    u.applyBoundaryCondition(S,extrapolate,slipWall,0.,t,extrapParams);
	}
	
      }

      if( assignSuperSonicInflow )
        u.applyBoundaryCondition(C,extrapolate,superSonicInflow,0.,t,extrapParams);
      if( assignSuperSonicOutflow )
        u.applyBoundaryCondition(C,extrapolate,superSonicOutflow,0.,t,extrapParams);
      if( assignSubSonicInflow )
        u.applyBoundaryCondition(C,extrapolate,subSonicInflow,0.,t,extrapParams);
      if( assignSubSonicOutflow )
        u.applyBoundaryCondition(C,extrapolate,subSonicOutflow,0.,t,extrapParams);
      if( assignNoSlipWall )
        u.applyBoundaryCondition(C,extrapolate,noSlipWall,0.,t,extrapParams);

      if( false && assignDirichletBoundaryCondition )
        u.applyBoundaryCondition(C,extrapolate,dirichletBoundaryCondition,0.,t,extrapParams);

      if( assignSymmetry ) // **** this is not correct if symmetry walls haven't been assigned yet!
      {
	// printf(" *** cnsBC: extrapolate 2nd ghost for symmetry\n");
        u.applyBoundaryCondition(C,extrapolate,symmetry,0.,t,extrapParams);
      }
	
      if( assignOutflow )
        u.applyBoundaryCondition(C,extrapolate,outflow,0.,t,extrapParams);

    }


    if( debug() & 4 || debug() & 64 )
    {
      char buff[80];
      ::display(u,sPrintF(buff,"cnsBC:After assign 2nd-ghost line (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%9.2e ");
    }
    real maxVal=0.;
    if( Parameters::checkForFloatingPointErrors )
      checkSolution(u,"cnsBC:before extrapInterpN",false,grid,maxVal,true);
    
    // extrapolate neighbours of interpolation points for 4th order artificial viscosity
    //kkc 060710    assert( parameters.dbase.get<int >("extrapolateInterpolationNeighbours") );  // consistency check
    if ( parameters.dbase.get<int >("extrapolateInterpolationNeighbours")) 
      {
	extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours"); // 3;

//     printf("***cnsBC orderOfExtrapolationForInterpolationNeighbours=%i\n",
//                 parameters.dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours"));
    
	u.applyBoundaryCondition(C,BCTypes::extrapolateInterpolationNeighbours,BCTypes::allBoundaries,0.,t,
			     extrapParams,grid);
      }

    if( Parameters::checkForFloatingPointErrors )
      checkSolution(u,"cnsBC:after extrapInterpN",false,grid,maxVal,true);
    
  }
  else
  {
    assert( !parameters.dbase.get<int >("extrapolateInterpolationNeighbours") ); // consistency check
  }
  
  
  // *******************************************************************
  // ****************(3) SYMMETRY CONDITIONS  **************************
  // *******************************************************************

  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    const realSerialArray & uKnownLocal = uKnown.getLocalArray();
    const intSerialArray & maskLocal = mg.mask().getLocalArray();
    const realSerialArray & gridVelocityLocal = gridVelocity.getLocalArray(); 
  #else
    const realSerialArray & uLocal = u; 
    const realSerialArray & uKnownLocal = uKnown;
    const intSerialArray & maskLocal = mg.mask();
    const realSerialArray & gridVelocityLocal = gridVelocity; 
  #endif  


  checkArrayIDs(" cnsBC: before symmetry"); 
  if( debug() & 64 )
  {
    char buff[80];
    ::display(u,sPrintF(buff,"cnsBC:Before assign slipWallOpt: u (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%13.10f ");
  }

  if( assignSlipWall || assignAxisymmetric || assignFarField )
  { 
    // finish slip wall  or assign assignAxisymmetric

    if( useOptSlipWall || assignAxisymmetric || assignFarField )
    {
      int ierr=0;
      int exact; // do this for now
      real *px, *prsxy, *pu2, *pu;

      pu=uLocal.getDataPointer();
      if( isRectangular  )
      {
	px = pu;  // these are not used in this case
	prsxy=pu;
      }
      else
      {
        mg.update(MappedGrid::THEinverseVertexDerivative | MappedGrid::THEcenter);
        #ifdef USE_PPP
  	  px = mg.center().getLocalArray().getDataPointer();
	  prsxy = mg.inverseVertexDerivative().getLocalArray().getDataPointer();
        #else
  	  px = mg.center().getDataPointer();
	  prsxy = mg.inverseVertexDerivative().getDataPointer();
        #endif
      }
      #ifdef USE_PPP
        pu2 = puOld==NULL ? pu : puOld->getLocalArray().getDataPointer();
      #else
        pu2 = puOld==NULL ? pu : puOld->getDataPointer();
      #endif
      real *pgv = gridVelocityLocal.getDataPointer();
      
      real *pgv2 = pgv;  // we need gridVelocity at t-dt ******************************************
      if( pGridVelocityOld!=NULL )
      {
	pgv2 = pGridVelocityOld->getLocalArray().getDataPointer();
      }

      real dx[3]={1.,1.,1.};
      if( isRectangular )
	mg.getDeltaX(dx);

      const int ndpar=50; //kkc 051208 20 was too small for debugging variables
      int ipar[ndpar];
      real rpar[ndpar];

      // parameter( slipWallSymmetry=0, slipWallPressureEntropySymmetry=1 )

      IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
      getLocalBoundsAndBoundaryConditions( u,gidLocal,dimLocal,bcLocal ); 


      realArray *gtt = NULL; 
      real *pgtt=NULL; 
      if( gridIsMoving && assignSlipWall )
      {
        // get acceleration terms on the boundary for the slip wall BC

        const int nc = mg.numberOfDimensions()*2;  // room for gtt and gttt
        gtt = new realArray(u.dimension(0),u.dimension(1),u.dimension(2),nc); 
	
        int option=4; // return g.tt 
        if( false && parameters.dbase.get<int >("slipWallBoundaryConditionOption")==4 ) // *** may need g'''
          option=option+8;  // return g.ttt as well
	 
        // gtt(i1,i2,i3,0:d-1)  : g.tt
        // gtt(i1,i2,i3,d:2d-1) : g.ttt
	parameters.dbase.get<MovingGrids >("movingGrids").getBoundaryAcceleration( mg, *gtt, grid,t, option );
	pgtt=gtt->getLocalArray().getDataPointer(); // is this ok? getLocalArrayWithGhostBoundaries ?
      }
      else
      {
	pgtt=pgv;  // not used in this case
      }
      


      int bcOption=parameters.dbase.get<int >("slipWallBoundaryConditionOption");

      int gridType = isRectangular ? 0 : 1;
      int useWhereMask=false;
      
      for( int m=0; m<numberOfPhases; m++ )
      {

	if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
	{
	  ipar[0]=parameters.dbase.get<int >("rc");
	  ipar[1]=parameters.dbase.get<int >("uc");
	  ipar[2]=parameters.dbase.get<int >("vc");
	  ipar[3]=parameters.dbase.get<int >("wc");
	  ipar[4]=parameters.dbase.get<int >("tc");
	}
	else
	{
	  ipar[0]=mpRho[m];
	  ipar[1]=mpV[m].getBase(); 
	  ipar[2]=ipar[1]+1;
	  ipar[3]=mpV[m].getBound(); 
	  ipar[4]=mpT[m]; 
	}
	
	ipar[5]=parameters.dbase.get<int >("sc");
	ipar[6]=parameters.dbase.get<int >("numberOfSpecies");
	ipar[7]=grid;
	ipar[8]=gridType;
	ipar[9]=parameters.dbase.get<int >("orderOfAccuracy");
	ipar[10]=gridIsMoving;
	ipar[11]=useWhereMask;
	ipar[12]=parameters.isAxisymmetric();
	ipar[13]=parameters.dbase.get<bool >("twilightZoneFlow");
	ipar[14]=bcOption;
	ipar[15]=parameters.dbase.get<int >("debug");
	ipar[16]=parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution"); 
        ipar[17]=parameters.dbase.get<int >("numberOfComponents");

        ipar[18]=parameters.dbase.get<int >("radialAxis");  // =axis1 if x=0 is axis of cylindrical symmetry, =axis2 if y=0 is..
        ipar[19]=parameters.dbase.get<bool >("axisymmetricWithSwirl");

	rpar[0]=dx[0];
	rpar[1]=dx[1];
	rpar[2]=dx[2];
	rpar[3]=mg.gridSpacing(0);
	rpar[4]=mg.gridSpacing(1);
	rpar[5]=mg.gridSpacing(2);
	rpar[6]=t; 
	rpar[7]=dt;
	rpar[8]=REAL_MIN*100.;
	rpar[9]=parameters.dbase.get<real >("gamma");
	rpar[10]=parameters.dbase.get<OGFunction* >("exactSolution")!=NULL ? (real &)parameters.dbase.get<OGFunction* >("exactSolution") : 0.;  // twilight zone pointer

        assert( 20+parameters.dbase.get<int >("numberOfComponents") < ndpar );
        for( int m=0; m<parameters.dbase.get<int >("numberOfComponents"); m++ )
	{
	  rpar[20+m]=parameters.artificialDiffusion(m); 
	}
	
        if( (assignSlipWall || assignAxisymmetric) && parameters.dbase.get<int >("slipWallBoundaryConditionOption")!=4 )
	{
	  cnsSlipWallBC(mg.numberOfDimensions(),
			uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
			uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
			ipar[0],rpar[0], *pu, 
			*pu2, *pgv, *pgv2, *pgtt, *maskLocal.getDataPointer(), *px, *prsxy, 
			bcLocal(0,0), gidLocal(0,0), exact, 
			*uKnownLocal.getDataPointer(), ierr );
	  
	}
	else if( (assignSlipWall || assignAxisymmetric) )
	{
	  // slip-wall derivative conditions
	  cnsSlipWallBC2(mg.numberOfDimensions(),
			 uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
			 uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
			 ipar[0],rpar[0], *pu, 
			 *pu2, *pgv, *pgv2, *pgtt, *maskLocal.getDataPointer(), *px, *prsxy, 
			 bcLocal(0,0), gidLocal(0,0), exact, 
			 *uKnownLocal.getDataPointer(), ierr );
	}

        if( assignFarField )
	{
	  cnsFarFieldBC(mg.numberOfDimensions(),
			uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
			uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
			ipar[0],rpar[0], *pu, 
			*pu2, *pgv, *pgv2, *pgtt, *maskLocal.getDataPointer(), *px, *prsxy, 
			bcLocal(0,0), gidLocal(0,0), exact, 
			*uKnownLocal.getDataPointer(), ierr );
	}
	

	
      }
      
      delete gtt;
      
//       if( debug() & 4 )
//       {
//         fprintf(parameters.dbase.get<FILE* >("pDebugFile"),">>>cnsBC: Errors after cnsSlipWallBC, t=%9.3e\n",t);
//         determineErrors(u,t);

//       }

      if( debug() & 4 )
      {
	char buff[80];
	::display(u,sPrintF(buff,"cnsBC:After cnsSlipWallBC (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%7.5f ");
	// ::display(u,sPrintF(buff,"cnsBC:After cnsSlipWallBC (t=%8.2e)",t),stdout,"%7.5f ");
      }

      // if( true ) return 0;  // ******************************************************************
      
    }
    else if( true )
    {
      // *** new way -- symmetry conditions  // ****** 010629
      if( debug() & 64 )
      {
	char buff[80];
	::display(u,sPrintF(buff,"cnsBC:Before slipWall symmetry (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%7.4f ");
      }

      const bool useExtrap=false;  // set to true to use old extrap BC's for the slipWall
      if( !useExtrap && !newMovingGridBoundaryConditions )
      {
	if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
	{
	  u.applyBoundaryCondition(V,vectorSymmetry,slipWall,0.,t); 
	  u.applyBoundaryCondition(rc,evenSymmetry ,slipWall,0.,t);
	  u.applyBoundaryCondition(tc,evenSymmetry ,slipWall,0.,t);
	}
	else
	{
	  for( int m=0; m<numberOfPhases; m++ )
	  {
	    u.applyBoundaryCondition(mpV[m],vectorSymmetry,slipWall,0.,t); 
	    u.applyBoundaryCondition(mpRho[m],evenSymmetry ,slipWall,0.,t);
	    u.applyBoundaryCondition(mpT[m],evenSymmetry ,slipWall,0.,t);
	  }
  	  u.applyBoundaryCondition(mpA,evenSymmetry,slipWall,0.,t);
	}	
	if( numberOfSpecies>0 )
	  u.applyBoundaryCondition(S,evenSymmetry,slipWall,0.,t);
      }
      else
      {
	extrapParams.ghostLineToAssign=1;
	extrapParams.orderOfExtrapolation=3;
	if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
	{
          // ** u.applyBoundaryCondition(V,vectorSymmetry, slipWall,0.,t,extrapParams); // ****** 010629
	  u.applyBoundaryCondition(V,extrapolate, slipWall,0.,t,extrapParams); // ****** 010629
	  u.applyBoundaryCondition(rc,extrapolate,slipWall,0.,t,extrapParams);
	  u.applyBoundaryCondition(tc,extrapolate,slipWall,0.,t,extrapParams);
	}
	else
	{
	  for( int m=0; m<numberOfPhases; m++ )
	  {
	    u.applyBoundaryCondition(mpV[m],extrapolate, slipWall,0.,t,extrapParams); // ****** 010629
	    u.applyBoundaryCondition(mpRho[m],extrapolate,slipWall,0.,t,extrapParams);
	    u.applyBoundaryCondition(mpT[m],extrapolate,slipWall,0.,t,extrapParams);
	  }
          u.applyBoundaryCondition(mpA,extrapolate,slipWall,0.,t,extrapParams);
	}
	
	if( numberOfSpecies>0 )
	  u.applyBoundaryCondition(S,extrapolate,slipWall,0.,t,extrapParams);
      }


      if( debug() & 64 )
      {
	char buff[80];
	::display(u,sPrintF(buff,"cnsBC:After slip wall even-symmetry: u (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%13.10f ");
      }
    }
    else
    {
      assert( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase );
      
      // **** old way
      if( true &&  // 010629: try turning this off **************************************************
	  parameters.dbase.get<Parameters::PDEVariation >("pdeVariation")==Parameters::conservativeGodunov )
      {
	extrapParams.ghostLineToAssign=1;
	extrapParams.orderOfExtrapolation=2;
	u.applyBoundaryCondition(C,extrapolate,    slipWall,0.,t,extrapParams);
      }
      else
      {
	u.applyBoundaryCondition(C,extrapolate,    slipWall,0.,t);
      }
  
      u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent0, slipWall,0.,t);

      if( parameters.dbase.get<int >("numberOfDimensions")==3 )
	u.applyBoundaryCondition(V,BCTypes::normalDerivativeOfTangentialComponent1, slipWall,0.,t);
      //  u.applyBoundaryCondition(V,BCTypes::extrapolate,    slipWall,0.,t);
      //  u.applyBoundaryCondition(V,BCTypes::vectorSymmetry, slipWall,0.,t);

      u.applyBoundaryCondition(rc,neumann,    slipWall,0.,t);
      u.applyBoundaryCondition(tc,neumann,    slipWall,0.,t);

    }
  }
  if( debug() & 64 )
  {
    char buff[80];
    ::display(u,sPrintF(buff,"cnsBC:Before assignSymmetry: u (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%13.10f ");
  }

  // symmetry
  if( assignSymmetry )
  {
    if( parameters.dbase.get<Parameters::PDE >("pde")!=Parameters::compressibleMultiphase )
    {
      u.applyBoundaryCondition(V,vectorSymmetry, symmetry,0.,t);
      u.applyBoundaryCondition(rc,neumann,       symmetry,0.,t);
      u.applyBoundaryCondition(tc,neumann,       symmetry,0.,t);
    }
    else
    {
      for( int m=0; m<numberOfPhases; m++ )
      {
	u.applyBoundaryCondition(mpV[m],vectorSymmetry, symmetry,0.,t);
	u.applyBoundaryCondition(mpRho[m],neumann,       symmetry,0.,t);
	u.applyBoundaryCondition(mpT[m],neumann,       symmetry,0.,t);
      }
      u.applyBoundaryCondition(mpA,neumann,       symmetry,0.,t);
    }
    if( numberOfSpecies>0 )
      u.applyBoundaryCondition(S,neumann,symmetry,0.,t);

    // 050602 -- do this for now --- fix this 
    if( true || parameters.dbase.get<Parameters::PDEVariation >("pdeVariation")==Parameters::conservativeGodunov )
    {
      extrapParams.ghostLineToAssign=2;
      extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfExtrapolationForSecondGhostLine"); 
      u.applyBoundaryCondition(C,extrapolate,symmetry,0.,t,extrapParams);
    }

  }
  
  if( debug() & 64 )
  {
    char buff[80];
    ::display(u,sPrintF(buff,"cnsBC:Before finishBoundaryConditions: u (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%13.10f ");
  }

  // numSent=getSent();
  // if( parameters.dbase.get<int >("myid")==0 ) printf("******* cnsBC: After final symmetry: new messages sent=%i\n",numSent);

  extrapParams.ghostLineToAssign=1;
  extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfExtrapolationForSecondGhostLine"); // 1;



  // cornerExtrapolationOption: for extrapolating corners along given directions instead of the diagonal
  //    1= do not extrapolate in axis1 direction
  //    2= do not extrapolate in axis2 direction
  //    3= do not extrapolate in axis3 direction
  extrapParams.cornerExtrapolationOption=parameters.dbase.get<int >("cornerExtrapolationOption"); 


  if( parameters.dbase.get<int >("slipWallBoundaryConditionOption")== 4 )  // ********************************** *wdh* 050613 : for now
  {
    extrapParams.orderOfExtrapolation=3;
  }

  // *** new way for symmetry BC's ***
  bool useNewSymmetryBC=false;
  if( useNewSymmetryBC && !parameters.dbase.get<bool >("twilightZoneFlow") )
  {
    if( !isRectangular ) 
      mg.update(MappedGrid::THEinverseVertexDerivative);   // this is used for the normal for vectorSymmetry

    extrapParams.setVectorSymmetryCornerComponent(uc);
    
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )for( int side=0; side<=1; side++ )
    {
      if( mg.boundaryCondition(side,axis)==slipWall ||
	  mg.boundaryCondition(side,axis)==symmetry  )
      {
	BoundaryConditionParameters::CornerBoundaryConditionEnum edgeBC
	  =BoundaryConditionParameters::CornerBoundaryConditionEnum(
             BoundaryConditionParameters::vectorSymmetryAxis1Corner+axis);
      
	if( axis==0 )
	{ // set all edges and corners next to this face (side,axis) -- side2==-1 -> edge parallel to axis2
	  for( int side2=-1; side2<=1; side2++ )for( int side3=-1; side3<=1; side3++ )
            if( side2!=-1 || side3!=-1 )
  	      extrapParams.setCornerBoundaryCondition(edgeBC,side,side2,side3);
	}
	else if( axis==1 )
	{
	  for( int side1=-1; side1<=1; side1++ )for( int side3=-1; side3<=1; side3++ )
            if( side1!=-1 || side3!=-1 )
	      extrapParams.setCornerBoundaryCondition(edgeBC,side1,side,side3);
	}
	else
	{
	  for( int side1=-1; side1<=1; side1++ )for( int side2=-1; side2<=1; side2++ )
            if( side1!=-1 || side2!=-1 )
	      extrapParams.setCornerBoundaryCondition(edgeBC,side1,side2,side);
	}
      }
    }
  }

  extrapParams.orderOfExtrapolation=2;  // reset from above *wdh* 060511 
  u.finishBoundaryConditions(extrapParams);  // This does a periodic update *******

  if( useNewSymmetryBC )
  {
    if( debug() & 4 || debug() & 16 )
    {
      char buff[80];
      ::display(u,sPrintF(buff,"cnsBC:After new symmetry BC: u (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%6.3f ");
    }
  }
  
  // numSent=getSent();
  // if( parameters.dbase.get<int >("myid")==0 ) printf("******* cnsBC: After finishBoundaryConditions: new messages sent=%i\n",numSent);

  if( !useNewSymmetryBC )  // ====================================
  {
    

  if( debug() & 4 || debug() & 16 )
  {
    char buff[80];
    ::display(u,sPrintF(buff,"cnsBC:After finishBoundaryConditions (before corner symmetry): u (t=%9.2e)",t),
              parameters.dbase.get<FILE* >("pDebugFile"),"%9.2e ");
  }
  
  // ** now assign symmetry type boundary conditions at corner points.
  if( !isRectangular ) mg.update(MappedGrid::THEvertexBoundaryNormal ); 
  real na[3],nb[3];
#ifdef USE_PPP
  for( axis=0; axis<3; axis++ )
  {
    na[axis] = uLocal.getBase(axis) +u.getGhostBoundaryWidth(axis);
    nb[axis] = uLocal.getBound(axis)-u.getGhostBoundaryWidth(axis);
  }
#else
  for( axis=0; axis<3; axis++ )
  {
    na[axis] = uLocal.getBase(axis);
    nb[axis] = uLocal.getBound(axis);
  }
#endif

  const real normEps = REAL_MIN*100.;
  for( int m=0; m<numberOfPhases; m++ )
  {
    int rc=parameters.dbase.get<int >("rc"), uc=parameters.dbase.get<int >("uc"), vc=parameters.dbase.get<int >("vc"), wc=parameters.dbase.get<int >("wc"), tc=parameters.dbase.get<int >("tc");
     
    if( parameters.dbase.get<Parameters::PDE >("pde")==Parameters::compressibleMultiphase )
    {
      rc=mpRho[m];
      uc=mpV[m].getBase();
      vc=uc+1;
      wc=mpV[m].getBound();
      tc=mpT[m];
    }
    
    if( mg.numberOfDimensions()==2 && !parameters.dbase.get<bool >("twilightZoneFlow") )
    {
      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
      int kv[3], &k1=kv[0], &k2=kv[1], &k3=kv[2];
      int is[3]={0,0,0};
      real nv[3]; // holds normal 
      i3=mg.gridIndexRange(0,axis3);
      j3=i3; k3=i3;

      for( axis=0; axis<mg.numberOfDimensions(); axis++ )
      {
	int axisp1 = (axis+1) % mg.numberOfDimensions();
	for( int side=0; side<=1; side++ )
	{
	  if( mg.boundaryCondition(side,axis)==slipWall || 
	      /*(axis!=parameters.dbase.get<int >("radialAxis") && mg.boundaryCondition(side,axis)==noSlipWall) ||*/ // kkc 051130 temporary hack 
	      mg.boundaryCondition(side,axis)==symmetry ) // *wdh* 050510 
	  {
	    //     axis=side
	    //        |
	    //        |slip wall
	    //        |         
	    //        |         
	    //        |(0,0)    
	    //  ------X--------  axisp1==side2
	    //    3 1 | 1' 3'   <-- ghost2=1
	    //    4 2 | 2' 4'   
	    //        |         
	    //      ^
	    //      |
	    //      ghost=1
	    //   Apply symmetry between ghost points 1 and 1', 2 and 2' etc..
	    //

	    // ***** fix this for PPP ************

#ifdef USE_PPP
	    // define normal(i1,i2,i3,dir) rx(i1,i2,i3,axis+numberOfDimensions*(dir))
            const RealArray & normal = !isRectangular ? mg.vertexBoundaryNormalArray(side,axis) :
                                                        uLocal;  // *wdh* 060703
#else
	    const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
#endif

	    is[axis]=1-2*side;

	    for( int ghost=1; ghost<=2; ghost++ )
	    {
	      iv[axis]=mg.gridIndexRange(side,axis)-is[axis]*ghost;  // ghost line
	      jv[axis]=mg.gridIndexRange(side,axis)+is[axis]*ghost;  // line inside
	      kv[axis]=mg.gridIndexRange(side,axis);                 // boundary point

	      if( kv[axis]<na[axis] || kv[axis]>nb[axis] ) continue;
	    
            
	      for( int side2=0; side2<=1; side2++ )  // do bottom or top
	      {
		if(  mg.boundaryCondition(side,axis)==slipWall &&
		     ( mg.boundaryCondition(side2,axisp1)==symmetry ||
		       mg.boundaryCondition(side2,axisp1)<0) ) // *wdh* 050510 
		{
		  // skip this side -- apply symmetry condition instead on adjacent side
		  continue;
		}
		if( gridIsMoving &&
		    mg.boundaryCondition(side,axis)==slipWall && 
		    mg.boundaryCondition(side2,axisp1)>0  )
		{
		  printf("cnsBC:ERROR:Symmetry corner condition -- fix this case for moving grids\n");
		  printf(" grid=%i, mg.boundaryCondition(side,axis)=%i, bc(side2,axisp1)=%i\n",
			 grid,mg.boundaryCondition(side,axis),mg.boundaryCondition(side2,axisp1));
		  Overture::abort("ERROR");
		}

		kv[axisp1]=mg.gridIndexRange(side2,axisp1);  // kv holds the corner point now
		if( kv[axisp1]<na[axisp1] || kv[axisp1]>nb[axisp1] ) continue;

		if( isRectangular )
		{
		  nv[0]=0.; nv[1]=0.; 
		  nv[axis]=2*side-1.;
		}
		else
		{
		  nv[0]=normal(k1,k2,k3,0);
		  nv[1]=normal(k1,k2,k3,1);
		}
	      
		// Assign all values out to dimension -- this will fix points when (side2,axisp1) is an interp bndry,
		// Is this really needed?
		int numGhost = abs( mg.gridIndexRange(side2,axisp1)-mg.dimension(side2,axisp1) );
		for( int ghost2=1; ghost2<=numGhost; ghost2++ ) // *wdh* 050514
		  // for( int ghost2=1; ghost2<=2; ghost2++ )
		{
		  iv[axisp1]=mg.gridIndexRange(side2,axisp1)-(1-2*side2)*ghost2;  
		  jv[axisp1]=iv[axisp1];
		  //		   printf(" assign symmetry iv=(%i,%i) jv=(%i,%i) gir=[%i,%i]x[%i,%i]\n",i1,i2,j1,j2,
		  //		      mg.gridIndexRange(0,0),mg.gridIndexRange(1,0),mg.gridIndexRange(0,1),mg.gridIndexRange(1,1));
	    
		  uLocal(i1,i2,i3,rc)=uLocal(j1,j2,j3,rc);
		  uLocal(i1,i2,i3,tc)=uLocal(j1,j2,j3,tc);
		  if( numberOfSpecies>0 )
		    uLocal(i1,i2,i3,S)=uLocal(j1,j2,j3,S);
                  if( parameters.dbase.get<Parameters::PDE >("pde")==Parameters::compressibleMultiphase )
                    uLocal(i1,i2,i3,mpA)=uLocal(j1,j2,j3,mpA);
                
		  uLocal(i1,i2,i3,uc)=uLocal(j1,j2,j3,uc);
		  uLocal(i1,i2,i3,vc)=uLocal(j1,j2,j3,vc);
		  if( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
		  {
		    uLocal(i1,i2,i3,wc)=uLocal(j1,j2,j3,wc);
		  }
		  
		  // velocity: n.u is odd
		  //  n.u(-1) = - n.u(+1)
		  // u(-1) <- u(-1) - (n.u)(-1)*n -(n.u)(+1)*n
		  real ndum=nv[0]*uLocal(i1,i2,i3,uc)+nv[1]*uLocal(i1,i2,i3,vc);
		  real ndup=nv[0]*uLocal(j1,j2,j3,uc)+nv[1]*uLocal(j1,j2,j3,vc);
		  uLocal(i1,i2,i3,uc)-=(ndup+ndum)*nv[0];
		  uLocal(i1,i2,i3,vc)-=(ndup+ndum)*nv[1];
		

		}
	      }
	    }
	  
	    is[axis]=0;  // reset
	  }
	}
      }
    }
    else if( mg.numberOfDimensions()==3 && !parameters.dbase.get<bool >("twilightZoneFlow") )
    {
//      printf("***WARNING*** applyBoundaryConditionsCNS: symmetry correction at corners at not yet been "
//             "implemented in 3D\n");
      Index iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      Index jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
      Index kv[3], &k1=kv[0], &k2=kv[1], &k3=kv[2];
      int is[3]={0,0,0};
      for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      {
	for( int side=0; side<=1; side++ )
	{
	  if( mg.boundaryCondition(side,axis)==slipWall ||
	      mg.boundaryCondition(side,axis)==symmetry ) // *wdh* 050514 
	  {
	    //     axis=side
	    //        |
	    //        |slip wall
	    //        |         
	    //        |         
	    //        |(0,0)    
	    //  ------X--------  axisp1==side2
	    //    3 1 | 1' 3'   <-- ghost2=1
	    //    4 2 | 2' 4'   
	    //        |         
	    //      ^
	    //      |
	    //      ghost=1
	    //   Apply symmetry between ghost points 1 and 1', 2 and 2' etc..
	    //

	    if( mg.boundaryCondition(side,axis)==symmetry )
	    {
	      Overture::abort("cnsBC:ERROR: finish this for symmetry");
	    }
	  

	    const int extra=2; // include 2 ghost points
	    // const realArray & normal = mg.vertexBoundaryNormal(side,axis);
#ifdef USE_PPP
            // define normal(i1,i2,i3,dir) rx(i1,i2,i3,axis+numberOfDimensions*(dir))
            const RealArray & normal = !isRectangular ? mg.vertexBoundaryNormalArray(side,axis) :
                                                        uLocal;  // *wdh* 060703
#else
	    const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
#endif
	    is[axis]=1-2*side;

	    for( int dir=0; dir<=1; dir++ ) // two tangential directions
	    {
	      int axisp = (axis+1+dir) % mg.numberOfDimensions();

	      getBoundaryIndex(mg.gridIndexRange(),side,axis,i1,i2,i3,extra);

	      // Here is the third direction besides axis and axisp:
	      const int axisn = (axis+1+1-dir) % mg.numberOfDimensions();  // (axis,axisp,axisn)
	      assert( axisn!=axis && axisn!=axisp );
	      // limit points to fit on the local array (*wdh* 050321)
	      const int n1a=max(iv[axisn].getBase(), uLocal.getBase(axisn));
	      const int n1b=min(iv[axisn].getBound(),uLocal.getBound(axisn));
	    
	      if( n1a>n1b ) continue;   // no points on this processor
	      iv[axisn]=Range(n1a,n1b);
	    

	      j1=i1, j2=i2, j3=i3;
	      k1=i1, k2=i2, k3=i3;


	      for( int ghost=1; ghost<=2; ghost++ )
	      {
		iv[axis]=mg.gridIndexRange(side,axis)-is[axis]*ghost;  // ghost line
		jv[axis]=mg.gridIndexRange(side,axis)+is[axis]*ghost;  // line inside
		kv[axis]=mg.gridIndexRange(side,axis);                 // boundary point
            
		// skip assignment of points that are not on this processor
		if( mg.gridIndexRange(side,axis)<na[axis] || mg.gridIndexRange(side,axis)>nb[axis] ) continue;

		for( int side2=0; side2<=1; side2++ )  // do bottom or top of direction axisp
		{
		  kv[axisp]=mg.gridIndexRange(side2,axisp);  // kv holds the corner point now *** could do both at once
		  if( mg.gridIndexRange(side2,axisp)<na[axisp] || mg.gridIndexRange(side2,axisp)>nb[axisp] ) continue;

		  for( int ghost2=1; ghost2<=2; ghost2++ )
		  {
		    iv[axisp]=mg.gridIndexRange(side2,axisp)-(1-2*side2)*ghost2;  
		    jv[axisp]=iv[axisp];
// 		    printf(" assign 3D symmetry at edges iv=(%i,%i)(%i,%i)(%i,%i) jv=(%i,%i)(%i,%i)(%i,%i) "
//                             "gir=[%i,%i]x[%i,%i]x[%i,%i]\n",
//                         i1.getBase(),i1.getBound(),i2.getBase(),i2.getBound(),i3.getBase(),i3.getBound(),
//                         j1.getBase(),j1.getBound(),j2.getBase(),j2.getBound(),j3.getBase(),j3.getBound(),
// 		        mg.gridIndexRange(0,0),mg.gridIndexRange(1,0),mg.gridIndexRange(0,1),mg.gridIndexRange(1,1),
//                         mg.gridIndexRange(0,2),mg.gridIndexRange(1,2));
	    
		    uLocal(i1,i2,i3,rc)=uLocal(j1,j2,j3,rc);
		    uLocal(i1,i2,i3,tc)=uLocal(j1,j2,j3,tc);
		    if( numberOfSpecies>0 )
		      uLocal(i1,i2,i3,S)=uLocal(j1,j2,j3,S);
		    if( parameters.dbase.get<Parameters::PDE >("pde")==Parameters::compressibleMultiphase )
		      uLocal(i1,i2,i3,mpA)=uLocal(j1,j2,j3,mpA);
                
		    if( isRectangular )
		    {
		      // velocity: n.u is odd, tangential components are even
		      if( axis==0 )
			uLocal(i1,i2,i3,uc)=-uLocal(j1,j2,j3,uc);
		      else
			uLocal(i1,i2,i3,uc)= uLocal(j1,j2,j3,uc);
		      if( axis==1 )
			uLocal(i1,i2,i3,vc)=-uLocal(j1,j2,j3,vc);
		      else
			uLocal(i1,i2,i3,vc)= uLocal(j1,j2,j3,vc);
		      if( axis==2 )
			uLocal(i1,i2,i3,wc)=-uLocal(j1,j2,j3,wc);
		      else
			uLocal(i1,i2,i3,wc)= uLocal(j1,j2,j3,wc);
		    }
		    else
		    {
		      uLocal(i1,i2,i3,uc)=uLocal(j1,j2,j3,uc);
		      uLocal(i1,i2,i3,vc)=uLocal(j1,j2,j3,vc);
		      uLocal(i1,i2,i3,wc)=uLocal(j1,j2,j3,wc);
                
		      // velocity: n.u is odd
		      //  n.u(-1) = - n.u(+1)
		      // u(-1) <- u(-1) - (n.u)(-1)*n -(n.u)(+1)*n
		      RealArray ndum,ndup,norm;
		      ndum=(normal(k1,k2,k3,0)*uLocal(i1,i2,i3,uc)+
			    normal(k1,k2,k3,1)*uLocal(i1,i2,i3,vc)+
			    normal(k1,k2,k3,2)*uLocal(i1,i2,i3,wc));
		      ndup=(normal(k1,k2,k3,0)*uLocal(j1,j2,j3,uc)+
			    normal(k1,k2,k3,1)*uLocal(j1,j2,j3,vc)+
			    normal(k1,k2,k3,2)*uLocal(j1,j2,j3,wc));
		      uLocal(i1,i2,i3,uc)-=(ndup+ndum)*normal(k1,k2,k3,0);
		      uLocal(i1,i2,i3,vc)-=(ndup+ndum)*normal(k1,k2,k3,1);
		      uLocal(i1,i2,i3,wc)-=(ndup+ndum)*normal(k1,k2,k3,2);

		    }

		  }
		}
	      }
	    } // end for dir
	  
	    is[axis]=0;  // reset
	  }
	}
      } // for int axis
	
	
    } // end else 3D
    
  }  // end for m (multiphase)
  
  if( debug() & 4 || debug() & 16 )
  {
    char buff[80];
    ::display(u,sPrintF(buff,"cnsBC:After symmetry BC (end of cnsBC): u (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%9.2e ");
  }
  
  }  // end if( !useNewSymmetryBC )
  

  // **** check that n.u = 0 for slip walls
  // checkNormalBC( grid, mg, u, parameters );
  
//   int side,axis;
//   real maxErr=0.,uDotN;
//   Index Ib1,Ib2,Ib3;
//   for( side=0; side<=1; side++ )
//   {
//     for( axis=0; axis<mg.numberOfDimensions(); axis++ )
//     {
//       getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
//       realArray & normal = mg.vertexBoundaryNormal(side,axis);

//       if( mg.numberOfDimensions()==2 )
// 	uDotN=max(
//             normal(Ib1,Ib2,Ib3,0)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("uc"))
// 	   +normal(Ib1,Ib2,Ib3,1)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("vc")));
//       else
// 	uDotN=max(
//             normal(Ib1,Ib2,Ib3,0)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("uc"))
// 	   +normal(Ib1,Ib2,Ib3,1)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("vc"))  
// 	   +normal(Ib1,Ib2,Ib3,2)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("wc")));

//       maxErr=max(maxErr,uDotN);
//     }
//   }
//   printf(">>>>>>>>>Grid %i: max(n.u) = %e\n",grid,maxErr);
  


/* ----
  bool trailingEdgeSingularity=TRUE;
  if( grid==1 && trailingEdgeSingularity )
  {
    // at a 
    MappedGrid & mg = *u.getMappedGrid();
    assert( mg.boundaryCondition(Start,axis1)<0 );

    for( int side=0; side<=1; side++ )
    {
      const int i1 = mg.gridIndexRange(side,0);
      const int i2 = mg.gridIndexRange(0,1);
      const int i3 = mg.gridIndexRange(0,2);
    
      Range I1(i1-2,i1+2), I2(i2-2,i2+2);
      u(I1,I2,i3,rc)=1.;
      u(I1,I2,i3,uc)=.1;
      u(I1,I2,i3,vc)=0.;
      u(I1,I2,i3,tc)=1.;
    }
    
  }
----- */

  checkArrayIDs(" cnsBC: done"); 

  return 0;
}
