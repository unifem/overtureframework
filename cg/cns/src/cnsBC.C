// This file automatically generated from cnsBC.bC with bpp.
#include "Cgcns.h"
#include "CnsParameters.h"
#include "App.h"
#include "FlowSolutions.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"

#include "SurfaceEquation.h"

#define ForBoundary(side,axis)   for( int axis=0; axis<mg.numberOfDimensions(); axis++ ) for( int side=0; side<=1; side++ )

// This next include file defines the setTemperatureBC macro
// ----------------------------------------------------------------------------
// Macro: Apply BC's on the Temperature 
// 
//   There are 3 cases: 
//      (1) apply a dirichlet BC                       (OPTION=dirichlet)
//      (2) extrapolate ghost pts on dirichlet BC's     (OPTION=extrapolateGhost)
//      (3) apply a mixed BC                           (OPTION=mixed)
// 
// Macro args:
// 
// tc : component to assign
// NAME : name of of the calling function (for comments)
// BCNAME : noSlipWall, inflowWithVelocityGiven etc. 
// OPTION: dirichlet, mixed, extrapolateGhost
// ----------------------------------------------------------------------------

#define cnsSlipWallBC EXTERN_C_NAME(cnsslipwallbc)
#define cnsSlipWallBC2 EXTERN_C_NAME(cnsslipwallbc2)
#define cnsFarFieldBC EXTERN_C_NAME(cnsfarfieldbc)
#define cnsNoSlipWallBC EXTERN_C_NAME(cnsnoslipwallbc)
#define cnsNoSlipBC EXTERN_C_NAME(cnsnoslipbc)
#define INOUTFLOWEXP EXTERN_C_NAME(inoutflowexp)
extern "C"
{
    void cnsSlipWallBC(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
                                          const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,		     const int&ipar, const real&rpar, 
                                          real&u, const real&u2, const real&gv, const real&gv2, const real & gtt,
                                          const int& mask, const real&x, const real&rsxy, 
                                          const int&bc, const int&indexRange, const int&exact, 
                                          const real& uKnown, const DataBase *pdb, const int&ierr );
    void cnsSlipWallBC2(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
                                          const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,		     const int&ipar, const real&rpar, 
                                          real&u, const real&u2, const real&gv, const real&gv2, const real & gtt,
                                          const int& mask, const real&x, const real&rsxy, 
                                          const int&bc, const int&indexRange, const int&interfaceType, const int&exact, 
                                          const real& uKnown, const DataBase *pdb, const int&ierr );

    void cnsFarFieldBC(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
                                          const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,		     const int&ipar, const real&rpar, 
                                          real&u, const real&u2, const real&gv, const real&gv2, const real & gtt,
                                          const int& mask, const real&x, const real&rsxy, 
                                          const int&bc, const int&indexRange, const int&exact, 
                                          const real& uKnown, const int&ierr );

  // Kyle's version:
    void cnsNoSlipWallBC(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
                   		       const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,
                   		       const real*u, const real*x, const real *aj, const real*rsxy,
                   		       const int*ipar, const real*rpar, const int*indexRange, const int*bc);


    void cnsNoSlipBC(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
               		   const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,		   const int&ipar, const real&rpar, 
               		   real&u, const real&gv, const real & gtt,
               		   const int& mask, const real&x, const real&rsxy, 
               		   const int&bc, const int&indexRange, const int&exact, 
               		   const real& uKnown, const int&ierr );

    void INOUTFLOWEXP(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
                		    const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,
                		    const real*ul, const real*x, const real *aj, const real*rsxy,
                		    const int*ipar, const real*rpar, const int*indexRange, const int*bc, const real*bd, const int*bt, int&nbd);

}


#define ogfTaylor EXTERN_C_NAME(ogftaylor)

extern "C"
{

/* Here are functions for TZ flow that can be called from fortran */

/* return a general derivative */
// void
// ogderiv_(OGFunction *&ep, const int & ntd, const int & nxd, const int & nyd, const int & nzd, 
//          const real &x, const real &y, const real &z, const real & t, const int & n, real & ud )
// {
//   ud=(*ep).gd(ntd,nxd,nyd,nzd,x,y,z,n,t);
// }
  
/* Return the forcing functions and derivatives for the Taylor slipwall BC */
/*   ud(0)= RHS for rho equation */
void
ogfTaylor(OGFunction *&ep, const real &x, const real &y, const real &z, const real & t, 
                      const int & nd, real *ud )
{
    const int rc=0, uc=1, vc=2, qc=3;
    const real gamma=1.4, gm1=gamma-1.;

    real r,rt,rx,ry,rxx,rxy,ryy, rtx,rty,rtt;
    real u,ut,ux,uy,uxx,uxy,uyy, utx,uty,utt;
    real v,vt,vx,vy,vxx,vxy,vyy, vtx,vty,vtt;
    real q,qt,qx,qy,qxx,qxy,qyy, qtx,qty,qtt;
    real p,pt,px,py,pxx,pxy,pyy, ptx,pty,ptt;
    real rf,uf,vf,qf,pf;
    real rtf,utf,vtf,qtf,ptf;
    real rxf,uxf,vxf,qxf,pxf;
    real ryf,uyf,vyf,qyf,pyf;

    

    r  =(*ep).gd(0,0,0,0,x,y,z,rc,t);
    rt =(*ep).gd(1,0,0,0,x,y,z,rc,t);
    rx =(*ep).gd(0,1,0,0,x,y,z,rc,t);
    ry =(*ep).gd(0,0,1,0,x,y,z,rc,t);
    rtt=(*ep).gd(2,0,0,0,x,y,z,rc,t);
    rtx=(*ep).gd(1,1,0,0,x,y,z,rc,t);
    rty=(*ep).gd(1,0,1,0,x,y,z,rc,t);
    rxx=(*ep).gd(0,2,0,0,x,y,z,rc,t);
    rxy=(*ep).gd(0,1,1,0,x,y,z,rc,t);
    ryy=(*ep).gd(0,0,2,0,x,y,z,rc,t);

    u  =(*ep).gd(0,0,0,0,x,y,z,uc,t);
    ut =(*ep).gd(1,0,0,0,x,y,z,uc,t);
    ux =(*ep).gd(0,1,0,0,x,y,z,uc,t);
    uy =(*ep).gd(0,0,1,0,x,y,z,uc,t);
    utt=(*ep).gd(2,0,0,0,x,y,z,uc,t);
    utx=(*ep).gd(1,1,0,0,x,y,z,uc,t);
    uty=(*ep).gd(1,0,1,0,x,y,z,uc,t);
    uxx=(*ep).gd(0,2,0,0,x,y,z,uc,t);
    uxy=(*ep).gd(0,1,1,0,x,y,z,uc,t);
    uyy=(*ep).gd(0,0,2,0,x,y,z,uc,t);

    v  =(*ep).gd(0,0,0,0,x,y,z,vc,t);
    vt =(*ep).gd(1,0,0,0,x,y,z,vc,t);
    vx =(*ep).gd(0,1,0,0,x,y,z,vc,t);
    vy =(*ep).gd(0,0,1,0,x,y,z,vc,t);
    vtt=(*ep).gd(2,0,0,0,x,y,z,vc,t);
    vtx=(*ep).gd(1,1,0,0,x,y,z,vc,t);
    vty=(*ep).gd(1,0,1,0,x,y,z,vc,t);
    vxx=(*ep).gd(0,2,0,0,x,y,z,vc,t);
    vxy=(*ep).gd(0,1,1,0,x,y,z,vc,t);
    vyy=(*ep).gd(0,0,2,0,x,y,z,vc,t);

    q  =(*ep).gd(0,0,0,0,x,y,z,qc,t);
    qt =(*ep).gd(1,0,0,0,x,y,z,qc,t);
    qx =(*ep).gd(0,1,0,0,x,y,z,qc,t);
    qy =(*ep).gd(0,0,1,0,x,y,z,qc,t);
    qtt=(*ep).gd(2,0,0,0,x,y,z,qc,t);
    qtx=(*ep).gd(1,1,0,0,x,y,z,qc,t);
    qty=(*ep).gd(1,0,1,0,x,y,z,qc,t);
    qxx=(*ep).gd(0,2,0,0,x,y,z,qc,t);
    qxy=(*ep).gd(0,1,1,0,x,y,z,qc,t);
    qyy=(*ep).gd(0,0,2,0,x,y,z,qc,t);

    p=r*q;
    pt=rt*q+r*qt;
    px=rx*q+r*qx;
    py=ry*q+r*qy;
    ptt=rtt*q+2.*rt*qt+r*qtt;
    pxx=rxx*q+2.*rx*qx+r*qxx;
    pyy=ryy*q+2.*ry*qy+r*qyy;
    
    ptx=rtx*q+rt*qx+rx*qt+r*qtx;
    pty=rty*q+rt*qy+ry*qt+r*qty;
    pxy=rxy*q+rx*qy+ry*qx+r*qxy;

    rf = rt + u*rx+v*ry + r*(ux+vy);
    uf = ut + u*ux+v*uy + px/r;
    vf = vt + u*vx+v*vy + py/r;
    qf = qt + u*qx+v*qy + gm1*q*(ux+vy);
    pf = pt + u*px+v*py + gamma*p*(ux+vy);
    
    rtf = rtt + ut*rx+vt*ry + rt*(ux+vy) + u*rtx+v*rty + r*(utx+vty);
    utf = utt + ut*ux+vt*uy + ptx/r + u*utx+v*uty - px*rt/(r*r);
    vtf = vtt + ut*vx+vt*vy + pty/r + u*vtx+v*vty - py*rt/(r*r);
    qtf = qtt + ut*qx+vt*qy + gm1*qt*(ux+vy) + u*qtx+v*qty + gm1*q*(utx+vty);
    ptf = ptt + ut*px+vt*py + gamma*pt*(ux+vy) + u*ptx+v*pty + gamma*p*(utx+vty);
    
    rxf = rtx + ux*rx+vx*ry + rx*(ux+vy) + u*rxx+v*rxy + r*(uxx+vxy);
    uxf = utx + ux*ux+vx*uy + pxx/r + u*uxx+v*uxy - px*rx/(r*r);
    vxf = vtx + ux*vx+vx*vy + pxy/r + u*vxx+v*vxy - py*rx/(r*r);
    qxf = qtx + ux*qx+vx*qy + gm1*qx*(ux+vy) + u*qxx+v*qxy + gm1*q*(uxx+vxy);
    pxf = ptx + ux*px+vx*py + gamma*px*(ux+vy) + u*pxx+v*pxy + gamma*p*(uxx+vxy);
    
    ryf = rty + uy*rx+vy*ry + ry*(ux+vy) + u*rxy+v*ryy + r*(uxy+vyy);
    uyf = uty + uy*ux+vy*uy + pxy/r + u*uxy+v*uyy - px*ry/(r*r);
    vyf = vty + uy*vx+vy*vy + pyy/r + u*vxy+v*vyy - py*ry/(r*r);
    qyf = qty + uy*qx+vy*qy + gm1*qy*(ux+vy) + u*qxy+v*qyy + gm1*q*(uxy+vyy);
    pyf = pty + uy*px+vy*py + gamma*py*(ux+vy) + u*pxy+v*pyy + gamma*p*(uxy+vyy);
    

    int n=0;
    ud[n]=rf;  n++;
    ud[n]=uf;  n++;
    ud[n]=vf;  n++;
    ud[n]=qf;  n++;
    ud[n]=pf;  n++;   /* ud[4] */
    
    ud[n]=rtf;  n++;
    ud[n]=utf;  n++;
    ud[n]=vtf;  n++;
    ud[n]=qtf;  n++; 
    ud[n]=ptf;  n++;  /* ud[9] */
    
    ud[n]=rxf;  n++;
    ud[n]=uxf;  n++;
    ud[n]=vxf;  n++;
    ud[n]=qxf;  n++;
    ud[n]=pxf;  n++;  /* ud[14] */ 
    
    ud[n]=ryf;  n++;
    ud[n]=uyf;  n++;
    ud[n]=vyf;  n++;
    ud[n]=qyf;  n++;
    ud[n]=pyf;  n++;  /* ud[19] */ 
    

}
  
}

// void
// getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
//                                      IntegerArray & gidLocal, 
//                                      IntegerArray & dimensionLocal, 
//                                      IntegerArray & bcLocal );

int
checkSolution(realMappedGridFunction & u, const aString & title, bool printResults, int grid,
                            real & maxVal, bool printResultsOnFailure=false );

void
checkNormalBC( int grid, MappedGrid & mg, realMappedGridFunction & u, Parameters & parameters )
{
    const int numberOfDimensions=mg.numberOfDimensions();
    
    int side,axis;
    real maxErr=0., maxErr2=0., uDotN, maxNormalError=0.;
    Index Ib1,Ib2,Ib3;
    for( side=0; side<=1; side++ )
    {
        for( axis=0; axis<mg.numberOfDimensions(); axis++ )
        {
            if( mg.boundaryCondition(side,axis)==Parameters::slipWall )
            {
      	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,-1);
                #ifndef USE_PPP
                	  const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                #else
    	  // const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);  // parallel version
                    const realArray & normal = mg.vertexBoundaryNormal(side,axis);
        	  Overture::abort("error: fix this for parallel -- set uDotN below");
                #endif
      	if( mg.numberOfDimensions()==2 )
      	{
        	  uDotN=max(fabs(
                          normal(Ib1,Ib2,Ib3,0)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("uc"))
          	    +normal(Ib1,Ib2,Ib3,1)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("vc"))));
      	}
      	else
      	{
        	  uDotN=max(
                        normal(Ib1,Ib2,Ib3,0)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("uc"))
          	    +normal(Ib1,Ib2,Ib3,1)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("vc"))  
          	    +normal(Ib1,Ib2,Ib3,2)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("wc")));
      	}
      	maxErr=max(maxErr,uDotN);

                realArray & rx = mg.inverseVertexDerivative();
                int n1,n2,n3;
                n1=axis;
                n2=n1+mg.numberOfDimensions(); n3=n1+2*mg.numberOfDimensions();
      	if( mg.numberOfDimensions()==2 )
      	{
        	  realArray norm;
        	  norm=(2.*side-1)/SQRT( SQR(rx(Ib1,Ib2,Ib3,n1))+SQR(rx(Ib1,Ib2,Ib3,n2)) );

          // printf(" max(norm)=%e, minNorm=%e\n",max(fabs(norm)),min(fabs(norm)));
        	  
        	  uDotN=max(fabs(
                            (rx(Ib1,Ib2,Ib3,n1)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("uc"))
            	      +rx(Ib1,Ib2,Ib3,n2)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("vc")))*norm));


        	  real normalError;
        	  normalError=max(fabs(normal(Ib1,Ib2,Ib3,0)-rx(Ib1,Ib2,Ib3,n1)*norm)+
                            	                  fabs(normal(Ib1,Ib2,Ib3,1)-rx(Ib1,Ib2,Ib3,n2)*norm));
        	  
                    maxNormalError=max(maxNormalError,normalError);

                    if( uDotN>.1e-5 )
        	  {
// 	    display(normal(Ib1,Ib2,Ib3,0),"normal(Ib1,Ib2,Ib3,0)");
// 	    display(rx(Ib1,Ib2,Ib3,n1)*norm,"rx(Ib1,Ib2,Ib3,n1)*norm");

  
// 	    display(normal(Ib1,Ib2,Ib3,1),"normal(Ib1,Ib2,Ib3,1)");
// 	    display(rx(Ib1,Ib2,Ib3,n2)*norm,"rx(Ib1,Ib2,Ib3,n2)*norm");
        	  }
      	}
      	else
      	{
        	  uDotN=max(
                          rx(Ib1,Ib2,Ib3,n1)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("uc"))
          	    +rx(Ib1,Ib2,Ib3,n2)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("vc"))  
          	    +rx(Ib1,Ib2,Ib3,n3)*u(Ib1,Ib2,Ib3,parameters.dbase.get<int >("wc")));
      	}
      	maxErr2=max(maxErr2,uDotN);


            }
            
        }
    }
    printf(">>>>>>>>>checkNormalBC: grid %i: max(n.u) = %8.2e, max(grad(r).u)=%8.2e normalError=%8.2e\n",
              grid,maxErr,maxErr2,maxNormalError);
}

// -- for testing: 
// void
// symmetryBC( int grid, MappedGrid & mg, realMappedGridFunction & u, Parameters & parameters )
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

//    Mixed-derivative BC for component i: 
//          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)



// ===================================================================================================
/// \brief Apply boundary conditions.
/// \details Most the methods implemented in Cgcns use this routine to assign boundary conditions.
///
/// \param t (input):
/// \param u (input/output) : apply to this grid function.
/// \param gridVelocity (input) : the grid velocity if gridIsMoving==true.
/// \param grid (input) : the grid number if this MappedGridFunction is part of a CompositeGridFunction.
/// \param option (input): not used here.
/// \param puOld (input): pointer to the solution at an old time (only used for some BC's).
/// \param pGridVelocityOld (input): pointer to the grid velocity at an old time (only used for some BC's).
/// \param dt (input): time step.
///
///
/// \note Note on the bcData array: 
///  Boundary condition parameter values are stored in the array bcData. Let nc=numberOfComponents,
///  then the values \n
///          bcData(i,side,axis,grid)  : i=0,1,...,nc-1 \n
///  would normally represent the RHS values for dirichlet BC's on component i, such as \n
///            u(i1,i2,i3,i) = bcData(i,side,axis,grid) \n
///  For a Mixed-derivative boundary condition, the parameters (a0,a1,a2) in the mixed BC: \n
///               a1*u(i1,i2,i3,i) + a2*u(i1,i2,i3,i)_n = a0 \n
///  are stored in\n
///          a_j = bcData(i+nc*(j),side,axis,grid),  j=0,1,2\n
///  Thus bcData(i,side,axis,grid) still holds the RHS value for the mixed-derivative condition
// ===================================================================================================
int Cgcns::
applyBoundaryConditions(const real & t, 
                                                realMappedGridFunction & u,
                  			realMappedGridFunction & gridVelocity,
                  			const int & grid,
                  			const int & option /* =-1 */,
                  			realMappedGridFunction *puOld /* =NULL */, 
                  			realMappedGridFunction *pGridVelocityOld /* =NULL */,
                  			const real & dt /* =-1. */ )
{
    if( parameters.dbase.get<int>("simulateGridMotion")>0 ) return 0;

    real time0=getCPU();

    if( debug() & 16 )
        printF("******* Cgcns::cnsBC: START grid=%i ***********\n",grid);

  // int numSent=getSent();
    checkArrayIDs(" cnsBC: start"); 
    
    MappedGrid & mg = *u.getMappedGrid();
    const IntegerArray & boundaryCondition = mg.boundaryCondition();
    bool isRectangular = mg.isRectangular();
    const int numberOfDimensions=mg.numberOfDimensions();
    const int nc =parameters.dbase.get<int >("numberOfComponents");
    const int numberOfComponents=nc;

    const bool gridIsMoving = parameters.gridIsMoving(grid);
    bool newMovingGridBoundaryConditions=false && gridIsMoving;

    const CnsParameters::PDE & pde = parameters.dbase.get<CnsParameters::PDE >("pde");

  // Here is the array that defines the domain interfaces, interfaceType(side,axis,grid) 
    const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");
    const int applyInterfaceBoundaryConditions = parameters.dbase.get<int>("applyInterfaceBoundaryConditions");
    

  // ** if( parameters.dbase.get<int >("slipWallBoundaryConditionOption")==4 || (gridIsMoving && isRectangular) )
    if( parameters.dbase.get<bool >("alwaysUseCurvilinearBoundaryConditions") )
    { // * for testing use the curvilinear version of the BC's even for rectangular grids  * 
        isRectangular=false;
        mg.update(MappedGrid::THEinverseVertexDerivative | MappedGrid::THEvertex | MappedGrid::THEcenter );
    }
    

    const int & numberOfSpecies  = parameters.dbase.get<int >("numberOfSpecies");
    
  // printF("******* Cgcns::cnsBC: numberOfSpecies=%i\n",numberOfSpecies);
    

//  const bool useOptSlipWall=!gridIsMoving && mg.numberOfDimensions()==2 && numberOfSpecies==0; 
    const bool useOptSlipWall=mg.numberOfDimensions()==2 && numberOfSpecies==0 &&
                                                (
                   			 (parameters.dbase.get<int >("slipWallBoundaryConditionOption")==0  // use opt here too: *wdh* 060220
                                                                                                                  && !gridIsMoving)   ||
                                                  parameters.dbase.get<int >("slipWallBoundaryConditionOption")==1 ||  // use opt here too: *wdh* 100810
                                                  parameters.dbase.get<int >("slipWallBoundaryConditionOption")==2 ||  // use opt here too: *wdh* 100810
                                                  parameters.dbase.get<int >("slipWallBoundaryConditionOption")==3 || 
                                                  parameters.dbase.get<int >("slipWallBoundaryConditionOption")==4 ) &&
                                                  pde!=CnsParameters::compressibleMultiphase; 

    const int & rc = parameters.dbase.get<int >("rc");
    const int & uc = parameters.dbase.get<int >("uc");
    const int & vc = parameters.dbase.get<int >("vc");
    const int & wc = parameters.dbase.get<int >("wc");
  // const int & pc = parameters.dbase.get<int >("pc");
    const int & tc = parameters.dbase.get<int >("tc");
    const int & sc = parameters.dbase.get<int >("sc");
    Range all;

    const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
    


  //  typedef Parameters::BoundaryCondition BoundaryCondition;
    typedef int BoundaryCondition;

    const BoundaryCondition & noSlipWall                = Parameters::noSlipWall;
    const BoundaryCondition & slipWall                  = Parameters::slipWall;
    const BoundaryCondition & superSonicOutflow         = CnsParameters::superSonicOutflow;
    const BoundaryCondition & superSonicInflow          = CnsParameters::superSonicInflow;
    const BoundaryCondition & subSonicOutflow           = CnsParameters::subSonicOutflow;
    const BoundaryCondition & subSonicInflow            = CnsParameters::subSonicInflow;
    const BoundaryCondition & symmetry                  = Parameters::symmetry;
    const BoundaryCondition & inflowWithVelocityGiven   = CnsParameters::inflowWithVelocityGiven;
    const BoundaryCondition & outflow                   = CnsParameters::outflow;
  //  const BoundaryCondition & inflowWithPressureAndTangentialVelocityGiven 
  //                                                      = Parameters::inflowWithPressureAndTangentialVelocityGiven;
    const BoundaryCondition & dirichletBoundaryCondition= Parameters::dirichletBoundaryCondition;
    const BoundaryCondition & neumannBoundaryCondition  = Parameters::neumannBoundaryCondition;
    const BoundaryCondition & axisymmetric              = Parameters::axisymmetric;
    const BoundaryCondition & farField                  = CnsParameters::farField;
    
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
    

  // Set this next variable to true if a grid face has a slipWall traction interface
    bool gridHasSlipWallTractionInterfaces=false; 

    ForBoundary(side,axis)
    {
        int bc=boundaryCondition(side,axis);
        switch (bc)
        {
        case 0 : break;
        case -1: break;
        case Parameters::slipWall:           assignSlipWall=true; break;
        case Parameters::noSlipWall :        assignNoSlipWall=true; break;
        case CnsParameters::superSonicInflow:   assignSuperSonicInflow=true; break;
        case CnsParameters::superSonicOutflow:  assignSuperSonicOutflow=true; break;
        case CnsParameters::subSonicInflow:     assignSubSonicInflow=true; break;
        case CnsParameters::subSonicOutflow:    assignSubSonicOutflow=true; break;
        case Parameters::symmetry :          assignSymmetry=true; break;
        case Parameters::axisymmetric:       assignAxisymmetric=true; break;
        case Parameters::dirichletBoundaryCondition:  assignDirichletBoundaryCondition=true; break;
        case Parameters::neumannBoundaryCondition:  assignNeumannBoundaryCondition=true; break;
        case CnsParameters::outflow:             assignOutflow=true; break;
        case CnsParameters::farField:            assignFarField=true; break;
        default: 
            printf("cnsBC:ERROR: unknown boundary condition =%i on grid %i, side=%i, axis=%i\n",bc,grid,side,axis);
            OV_ABORT("ERROR");
            break;
        }


        gridHasSlipWallTractionInterfaces = gridHasSlipWallTractionInterfaces ||
            ( bc ==slipWall && interfaceType(side,axis,grid)==Parameters::tractionInterface );
        
    }
    if( gridHasSlipWallTractionInterfaces )
    {
        if( debug() & 4 )
            printF("++cnsBC: grid=%i applyInterfaceBoundaryConditions=%i gridHasSlipWallTractionInterfaces=%i, \n",
           	     grid,(int)applyInterfaceBoundaryConditions, (int)gridHasSlipWallTractionInterfaces);
    }
    
          	
    if( !isRectangular )
    {
        if( assignNoSlipWall || 
                ( assignSlipWall && !useOptSlipWall) || 
                assignSuperSonicOutflow )
        {
            mg.update(MappedGrid::THEvertexBoundaryNormal );  // *wdh* 060213
        } 
    }
    

  // ----------------------------------------------------------------------------------------------------
  // For testing -- evaluate the known solution ----

    const Parameters::KnownSolutionsEnum & knownSolution = 
                        parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution");
    
    realArray *uKnownPointer=NULL;
    if( knownSolution!=CnsParameters::noKnownSolution )
    {
        int extra=2;
        Index I1,I2,I3;
        getIndex(mg.gridIndexRange(),I1,I2,I3,extra);  // **************** fix this -- only evaluate near boundaries --

    // NOTE: This next call will not recompute the supersonic expanding flow if it has already been computed
        uKnownPointer = &parameters.getKnownSolution( t,grid,I1,I2,I3 );
    }
    realArray & uKnown = uKnownPointer!=NULL ? *uKnownPointer : u;

  // ----------------------------------------------------------------------------------------------------

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

    IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
    ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( u,gidLocal,dimLocal,bcLocal ); 

    const int gridType = isRectangular ? 0 : 1;
    real dx[3]={1.,1.,1.};
    if( isRectangular )
        mg.getDeltaX(dx);


    const bool projectInterface = parameters.dbase.get<bool>("projectInterface");
    if( debug() & 4 )
    {
        printP("*** cnsBC: projectInterface = %i ***\n",projectInterface);
    }


    const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
    BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid);
    BoundaryData & bd = parameters.dbase.get<std::vector<BoundaryData> >("boundaryData")[grid];

    const bool assignTemperature = true;
//    Mixed-derivative BC for component i: 
//          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
// #define mixedRHS(component)         bcData(component+nc*(0),side,axis,grid)
// #define mixedCoeff(component)       bcData(component+nc*(1),side,axis,grid)
// #define mixedNormalCoeff(component) bcData(component+nc*(2),side,axis,grid)

    
  // **************************************************************************
  // ORDERING OF BOUNDARY CONDITIONS:
  //  (1) Apply Dirichlet like BC's first, these do not depend on the other BC's
  //  (2) Apply Neumann and extrap BC's second
  //  (3) Apply symmetry-like conditions
  // **************************************************************************

    int numVelocityComponents=parameters.dbase.get<int >("numberOfDimensions");
    if( parameters.dbase.get<bool >("axisymmetricWithSwirl") ) numVelocityComponents=3;

    Range C(0,nc-1);  // ***** is this correct ******
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
    if( pde==CnsParameters::compressibleMultiphase )
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

    if( false && pde==CnsParameters::compressibleMultiphase )
    {

        u.applyBoundaryCondition(C,neumann,BCTypes::allBoundaries,0.,t);

        printf(">>> applyBoundaryConditionsCNS:compressibleMultiphase: Apply neumann BC on all boundaries <<<<\n");

        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForBoundaryConditions"))+=getCPU()-time0;
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


  // the dirichletBoundaryCondition is for testing TZ flow or known solutions.
    if( assignDirichletBoundaryCondition )
    {
    // *wdh* 090412 if( knownSolution==CnsParameters::supersonicFlowInAnExpandingChannel )
        if( knownSolution!=CnsParameters::noKnownSolution ) // apply any known solution at dirichlet BC's
        {
            bcParams.extraInTangentialDirections=2;
            bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::arrayForcing); // *wdh* 050528 -- new way

            for( int line=0; line<=numberOfGhostPointsNeeded; line++ )
            {
      	bcParams.lineToAssign=line;
                u.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,uKnownLocal,t,bcParams);
            }
            
            bcParams.lineToAssign=0;  // reset
            bcParams.extraInTangentialDirections=0;
            bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::unSpecifiedForcing);

            if( debug() & 64 )
            {
      	::display(uKnown,"cnsBC: known solution: uKnown",parameters.dbase.get<FILE* >("pDebugFile"),"%4.2f ");
      	::display(u,"cnsBC: after assign dirichlet BC (from known solution)",parameters.dbase.get<FILE* >("pDebugFile"),"%4.2f ");
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
    // *wdh* 090412 if( knownSolution==CnsParameters::supersonicFlowInAnExpandingChannel )
        if( knownSolution!=CnsParameters::noKnownSolution ) // apply any known solution at Neumann BC's
        {

            bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::arrayForcing); // *wdh* 050528 -- new way
            u.applyBoundaryCondition(C,neumann,neumannBoundaryCondition,uKnownLocal,t,bcParams);
            bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::unSpecifiedForcing);

            if( debug() & 64 )
            {
      	::display(uKnown,"cnsBC: known solution: uKnown",parameters.dbase.get<FILE* >("pDebugFile"),"%4.2f ");
      	::display(u,"cnsBC: after assign neumann BC (from known solution)",parameters.dbase.get<FILE* >("pDebugFile"),"%4.2f ");
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

        if( pde!=CnsParameters::compressibleMultiphase )
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
            if( pde!=CnsParameters::compressibleMultiphase )
            {
        // deformingSticks is broken : try turning off this:
        // bool projectInterface=false;  // ************************ TEMP 2013/02/08 ************************ TEST **********

                if( (bool)applyInterfaceBoundaryConditions && !( projectInterface && gridHasSlipWallTractionInterfaces) )
      	{
        	  u.applyBoundaryCondition(V,normalComponent,slipWall,gridVelocity,t);
      	}
                else
      	{
          // Here we do NOT apply the slipWall n.u=g BC to traction interfaces when 
          // we project the interface since it has already been done.
        	  ForBoundary(side,axis)
        	  {
          	    if( boundaryCondition(side,axis)==slipWall )
          	    {
            	      if( !projectInterface || interfaceType(side,axis,grid)!=Parameters::tractionInterface )
            	      {
            		u.applyBoundaryCondition(V,normalComponent,BCTypes::boundary(side,axis),gridVelocity,t);
            	      }
            	      else
            	      {
                                if( debug() & 2 )
              		  printF("cnsBC:slipWall: do NOT set n.v=g on interface (side,axis,grid)=(%i,%i,%i)" 
                   			 "- this has already been done.\n",side,axis,grid);
            	      }
          	    }
        	  }
      	}
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
            if( pde!=CnsParameters::compressibleMultiphase )
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
    
        if( debug() & 64 )
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
        if( pde!=CnsParameters::compressibleMultiphase )
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
  // old const  int nc=parameters.dbase.get<int >("numberOfComponents");

    if( assignNoSlipWall )
    {
    // noSlipWall
    // noSlipWall:
    // (1) set rho.n=, (u,v,w)= T=
    // (2) extrapolate (u,v,w,T)

    // ::display(bcData(all,all,all,grid),"bcData(all,all,all,grid)");
        

        for( int side=0; side<=1; side++ )
        {
            for( int axis=0; axis<numberOfDimensions; axis++ )
            {
      	if( mg.boundaryCondition(side,axis)==Parameters::noSlipWall && bcData(tc+1,side,axis,grid)!=0. )
      	{
        	  adiabaticNoSlipWall=true;
                    if( debug() & 4 )
          	    printF("+++noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : Mixed BC for T: %3.2f*T+%3.2f*T.n=%g\n",
               		   grid,side,axis, mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), 
                                      mixedRHS(tc,side,axis,grid));
      	}
            }
        }
            
    // Here is an example of how to apply a BC to a particular side:
    // u.applyBoundaryCondition(V,neumann,BCTypes::boundary(side,axis),0.,t,bcParams);

        if( pde!=CnsParameters::compressibleMultiphase )
        {
      // *wdh* 051128 u.applyBoundaryCondition(tc,dirichlet, noSlipWall,bcData,t,
      //              Overture::defaultBoundaryConditionParameters(),grid);

      // *wdh* 080517 -- new way below 
//       if( !adiabaticNoSlipWall )
//       {
// 	u.applyBoundaryCondition(tc,dirichlet, noSlipWall,bcData,pBoundaryData,t,
// 				 Overture::defaultBoundaryConditionParameters(),grid);
//       }
//       else
//       {
// 	// Some noSlipWall are adiabatic: A Neumann or Mixed BC on T
// 	for( int side=0; side<=1; side++ )
// 	{
// 	  for( int axis=0; axis<numberOfDimensions; axis++ )
// 	  {
// 	    if( mg.boundaryCondition(side,axis)==noSlipWall )
// 	    {
// 	      if( mixedNormalCoeff(tc)==0. ) // coeff of T.n 
// 	      {
// 		// Dirichlet
// 		u.applyBoundaryCondition(tc,dirichlet,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
// 					 Overture::defaultBoundaryConditionParameters(),grid);
// 	      }
// 	      else
// 	      {
// 		// Mixed or Neumann -- this case is done below
            		
// 	      }
          	    
// 	    }
// 	  }
// 	}
//       }

      // assign dirichlet BC's on T here *wdh* 080517 
              if( assignTemperature )
              {
                  FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
                  FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
                  ForBoundary(side,axis)
                  {
                      if( mg.boundaryCondition(side,axis)==noSlipWall )
                      {
                          if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                          { // This is an interface between domains
               // for now we only know about interfaces at no-slip walls: 
                              assert( mg.boundaryCondition(side,axis)==noSlipWall );
      	 // what about BC's applied at t=0 before the boundary data is set ??
      	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
      	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
      	 // to use the boundary data instead.
                              #ifdef USE_PPP
                                  realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                              #else
                                  const realSerialArray & uLocal = u;
                              #endif
                   	 Index Ib1,Ib2,Ib3;
                   	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                   	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
                   	 if( debug() & 4 )
                   	 {
                     	   printP("cnsBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                          		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                     	   fprintf(pDebugFile,"cnsBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                     			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                     	   if( pBoundaryData[side][axis]==NULL )
                     	   {
                       	     if( !ok )
                                          fprintf(pDebugFile," cnsBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                      else
                                          fprintf(pDebugFile," cnsBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                     	   }
                     	   else
                     	   {
      	     // RealArray & bd = *pBoundaryData[side][axis];
      	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                     	   }
                   	 }
                              assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
                   	 if( ok && pBoundaryData[side][axis]==NULL )
                   	 {
                 // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
                 // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
                 // based on the current solution
                                  RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                                  bd=0.;
                                  #ifdef USE_PPP
                       	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                                  #else
                       	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                                  #endif
                     	   real a0=mixedCoeff(tc,side,axis,grid);
                     	   real a1=mixedNormalCoeff(tc,side,axis,grid);
                 // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                                  MappedGridOperators & op = *(u.getOperators());
                                  Range N(tc,tc);
                                  RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                     	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                     	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                                  if( mg.numberOfDimensions()==2 )
                     	   {
                                      bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                        				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                     	   }
                     	   else
                     	   {
                                      RealArray uz(Ib1,Ib2,Ib3,N);
                       	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                       	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                        				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
                                        				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                     	   }
                                  const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                     	   if( false && twilightZoneFlow ) //  *******************************************************
                     	   {
                                      fprintf(pDebugFile," cnsBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                       	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                       	     realArray & x= mg.center();
            #ifdef USE_PPP
                       	     realSerialArray xLocal; 
                       	     if( !rectangular || twilightZoneFlow ) 
                         	       getLocalArrayWithGhostBoundaries(x,xLocal);
            #else
                       	     const realSerialArray & xLocal = x;
            #endif
                       	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                       	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                       	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                       	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                       	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                       	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
                       	     if( mg.numberOfDimensions()==2 )
                       	     {
                         	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                       	     }
                       	     else
                       	     {
                         	       realSerialArray uez(Ib1,Ib2,Ib3);
                         	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
                         	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                       	     }
                     	   } // *******************************************
                   	 }
                   	 if( pBoundaryData[side][axis]!=NULL )
                     	   u.getOperators()->setTwilightZoneFlow( false );
                   	 else
                   	 {
                                  if( t>0. || debug() & 4 )
                     	   {
                                      if( ok )
                                 	       printP("$$$$ cnsBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                     	   }
                   	 }
                          }
                          if( debug() & 4 )
                   	 printF("++++cnsBC: noSlipWall: (grid,side,axis)=(%i,%i,%i) : "
                        		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                        		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                        		grid,side,axis, 
                        		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
                        		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                        		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                     	   );
      //        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
      //        {
      // 	 mixedBoundaryConditionOnTemperature=true;
      // 	 if( debug() & 4 )
      // 	   printF("++++insBC: noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
      // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
      //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
      // 		  grid,side,axis, 
      //                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
      //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
      //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
      // 	     );
      //        }
                          if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
                          {
      	 // Dirichlet
                   	 u.applyBoundaryCondition(tc,dirichlet,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				  bcParams,grid);
                          }
                          else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                          {
               // -- Variable Coefficient Temperature (const coeff.) BC --- 
                   	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                        		grid,side,axis);
               // BC is : a0(x)*T + an(x)*T.n = g 
               //  a0 = varCoeff(i1,i2,i3,0)
               //  an = varCoeff(i1,i2,i3,1)
                   	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                            BoundaryData::variableCoefficientTemperatureBC,side,axis );
                              bcParams.setVariableCoefficientsArray(&varCoeff);
                   	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				  bcParams,grid);
                              bcParams.setVariableCoefficientsArray(NULL);  // reset 
                          } 
                          else
                          {
      	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
                          }
                          if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                          { // reset TZ
                   	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                          }
                      } // end if bc = noSlipWall
                  } // end for boundary
         // ************ try this ********* 080909
         // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
              } // end if assignTemperature

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
            for( int m=0; m<numberOfPhases; m++ )
            {
	// *wdh* 051128 u.applyBoundaryCondition(mpT[m],dirichlet, noSlipWall,bcData,t,
        // *wdh* 051128                          Overture::defaultBoundaryConditionParameters(),grid);

//         u.applyBoundaryCondition(mpT[m],dirichlet, noSlipWall,bcData,pBoundaryData,t,
//  			         Overture::defaultBoundaryConditionParameters(),grid);

	// assign dirichlet BC's on T here *wdh* 080517 
              if( assignTemperature )
              {
                  FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
                  FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
                  ForBoundary(side,axis)
                  {
                      if( mg.boundaryCondition(side,axis)==noSlipWall )
                      {
                          if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                          { // This is an interface between domains
               // for now we only know about interfaces at no-slip walls: 
                              assert( mg.boundaryCondition(side,axis)==noSlipWall );
      	 // what about BC's applied at t=0 before the boundary data is set ??
      	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
      	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
      	 // to use the boundary data instead.
                              #ifdef USE_PPP
                                  realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                              #else
                                  const realSerialArray & uLocal = u;
                              #endif
                   	 Index Ib1,Ib2,Ib3;
                   	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                   	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
                   	 if( debug() & 4 )
                   	 {
                     	   printP("cnsBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                          		  side,axis,grid,mixedCoeff(mpT[m],side,axis,grid),mixedNormalCoeff(mpT[m],side,axis,grid));
                     	   fprintf(pDebugFile,"cnsBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                     			       side,axis,grid,mixedCoeff(mpT[m],side,axis,grid),mixedNormalCoeff(mpT[m],side,axis,grid));
                     	   if( pBoundaryData[side][axis]==NULL )
                     	   {
                       	     if( !ok )
                                          fprintf(pDebugFile," cnsBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                      else
                                          fprintf(pDebugFile," cnsBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                     	   }
                     	   else
                     	   {
      	     // RealArray & bd = *pBoundaryData[side][axis];
      	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                     	   }
                   	 }
                              assert( mixedCoeff(mpT[m],side,axis,grid)!=0. || mixedNormalCoeff(mpT[m],side,axis,grid)!=0. );
                   	 if( ok && pBoundaryData[side][axis]==NULL )
                   	 {
                 // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
                 // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
                 // based on the current solution
                                  RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                                  bd=0.;
                                  #ifdef USE_PPP
                       	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                                  #else
                       	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                                  #endif
                     	   real a0=mixedCoeff(mpT[m],side,axis,grid);
                     	   real a1=mixedNormalCoeff(mpT[m],side,axis,grid);
                 // bd(Ib1,Ib2,Ib3,mpT[m])=a0*uLocal(Ib1,Ib2,Ib3,mpT[m]);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                                  MappedGridOperators & op = *(u.getOperators());
                                  Range N(mpT[m],mpT[m]);
                                  RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                     	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                     	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                                  if( mg.numberOfDimensions()==2 )
                     	   {
                                      bd(Ib1,Ib2,Ib3,mpT[m])=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,mpT[m])+
                                        				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,mpT[m])) + a0*uLocal(Ib1,Ib2,Ib3,mpT[m]);
                     	   }
                     	   else
                     	   {
                                      RealArray uz(Ib1,Ib2,Ib3,N);
                       	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                       	     bd(Ib1,Ib2,Ib3,mpT[m])=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,mpT[m])+
                                        				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,mpT[m])+
                                        				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,mpT[m])) + a0*uLocal(Ib1,Ib2,Ib3,mpT[m]);
                     	   }
                                  const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                     	   if( false && twilightZoneFlow ) //  *******************************************************
                     	   {
                                      fprintf(pDebugFile," cnsBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                       	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                       	     realArray & x= mg.center();
            #ifdef USE_PPP
                       	     realSerialArray xLocal; 
                       	     if( !rectangular || twilightZoneFlow ) 
                         	       getLocalArrayWithGhostBoundaries(x,xLocal);
            #else
                       	     const realSerialArray & xLocal = x;
            #endif
                       	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                       	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                       	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,mpT[m],t);
                       	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,mpT[m],t);
                       	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,mpT[m],t);
                       	     real a0=mixedCoeff(mpT[m],side,axis,grid), a1=mixedNormalCoeff(mpT[m],side,axis,grid);
                       	     if( mg.numberOfDimensions()==2 )
                       	     {
                         	       bd(Ib1,Ib2,Ib3,mpT[m])=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                       	     }
                       	     else
                       	     {
                         	       realSerialArray uez(Ib1,Ib2,Ib3);
                         	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,mpT[m],t);
                         	       bd(Ib1,Ib2,Ib3,mpT[m])=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                       	     }
                     	   } // *******************************************
                   	 }
                   	 if( pBoundaryData[side][axis]!=NULL )
                     	   u.getOperators()->setTwilightZoneFlow( false );
                   	 else
                   	 {
                                  if( t>0. || debug() & 4 )
                     	   {
                                      if( ok )
                                 	       printP("$$$$ cnsBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                     	   }
                   	 }
                          }
                          if( debug() & 4 )
                   	 printF("++++cnsBC: noSlipWall: (grid,side,axis)=(%i,%i,%i) : "
                        		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                        		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                        		grid,side,axis, 
                        		mixedCoeff(mpT[m],side,axis,grid), mixedNormalCoeff(mpT[m],side,axis,grid), mixedRHS(mpT[m],side,axis,grid), 
                        		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                        		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                     	   );
      //        if( mixedNormalCoeff(mpT[m],side,axis,grid)!=0. ) // coeff of T.n is non-zero
      //        {
      // 	 mixedBoundaryConditionOnTemperature=true;
      // 	 if( debug() & 4 )
      // 	   printF("++++insBC: noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
      // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
      //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
      // 		  grid,side,axis, 
      //                   mixedCoeff(mpT[m],side,axis,grid), mixedNormalCoeff(mpT[m],side,axis,grid), mixedRHS(mpT[m],side,axis,grid), 
      //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
      //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
      // 	     );
      //        }
                          if( mixedNormalCoeff(mpT[m],side,axis,grid)==0. ) // coeff of T.n 
                          {
      	 // Dirichlet
                   	 u.applyBoundaryCondition(mpT[m],dirichlet,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				  bcParams,grid);
                          }
                          else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                          {
               // -- Variable Coefficient Temperature (const coeff.) BC --- 
                   	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                        		grid,side,axis);
               // BC is : a0(x)*T + an(x)*T.n = g 
               //  a0 = varCoeff(i1,i2,i3,0)
               //  an = varCoeff(i1,i2,i3,1)
                   	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                            BoundaryData::variableCoefficientTemperatureBC,side,axis );
                              bcParams.setVariableCoefficientsArray(&varCoeff);
                   	 u.applyBoundaryCondition(mpT[m],mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				  bcParams,grid);
                              bcParams.setVariableCoefficientsArray(NULL);  // reset 
                          } 
                          else
                          {
      	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
                          }
                          if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                          { // reset TZ
                   	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                          }
                      } // end if bc = noSlipWall
                  } // end for boundary
         // ************ try this ********* 080909
         // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
              } // end if assignTemperature

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
        if( parameters.dbase.get<SurfaceEquation* >("pSurfaceEquation")!=NULL )
        {
            SurfaceEquation & surfaceEquation = *(parameters.dbase.get<SurfaceEquation* >("pSurfaceEquation"));
            const int numberOfSurfaceEquationFaces= surfaceEquation.faceList.size();
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
        }
        

    // do rest later

        checkArrayIDs(" cnsBC: after assignNoSlipWall"); 

    }


    if( assignSubSonicInflow && 
      !(parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit  ||
      	parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateNewton))
    {
    // subSonicInflow:
    // ****** fix this ***********
        if( pde!=CnsParameters::compressibleMultiphase )
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
    // -- re-assign the exact solution on ghost lines since these were changed by the extrapolation --

    // *wdh* 090412 if( knownSolution==CnsParameters::supersonicFlowInAnExpandingChannel )
        if( knownSolution!=CnsParameters::noKnownSolution ) // apply any known solution at dirichlet BC's
        {
            bcParams.extraInTangentialDirections=2;
            bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::arrayForcing); // *wdh* 050528 -- new way

            for( int line=1; line<=numberOfGhostPointsNeeded; line++ )
            {
      	bcParams.lineToAssign=line;
                u.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,uKnownLocal,t,bcParams);
            }
            
            bcParams.lineToAssign=0;  // reset
            bcParams.extraInTangentialDirections=0;
            bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::unSpecifiedForcing);

            if( debug() & 64 )
            {
      	::display(uKnown,"cnsBC: known solution: uKnown",parameters.dbase.get<FILE* >("pDebugFile"),"%4.2f ");
      	::display(u,"cnsBC: after assign dirichlet BC (from known solution)",parameters.dbase.get<FILE* >("pDebugFile"),"%4.2f ");
            }

        }
        else
        {
      // u.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,0.,t);
            
            bcParams.extraInTangentialDirections=2;  // *wdh* 050611 -- assign extended boundary
            for( int line=1; line<=2; line++ )
            {
                bcParams.lineToAssign=line;
                u.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,0.,t,bcParams);
            }
            bcParams.lineToAssign=0; // reset
            bcParams.extraInTangentialDirections=0;
        }


// *     // *wdh* 090412 if( knownSolution==CnsParameters::supersonicFlowInAnExpandingChannel )
// *     if( knownSolution!=CnsParameters::noKnownSolution ) // apply any known solution at dirichlet BC's
// *     {
// *       bcParams.lineToAssign=1;
// *       bcParams.extraInTangentialDirections=2;
// *       u.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,uKnownLocal,t,bcParams);
// *       bcParams.lineToAssign=0;
// *       bcParams.extraInTangentialDirections=0;
// * //        extrapParams.dbase.get< >("ghostLineToAssign")=1;
// * //        extrapParams.dbase.get< >("orderOfExtrapolation")=2; 
// * //        u.applyBoundaryCondition(C,extrapolate,dirichletBoundaryCondition,0.,t,extrapParams);
// *     }
// *     else
// *     {
// *       bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::arrayForcing); // *wdh* 050528 -- new way
// * 
// *       bcParams.extraInTangentialDirections=2;
// *       for( int line=1; line<=2; line++ )
// *       {
// * 	bcParams.lineToAssign=line;
// * 	u.applyBoundaryCondition(C,dirichlet,dirichletBoundaryCondition,uKnownLocal,t,bcParams);
// *       }
// *       bcParams.lineToAssign=0; // reset
// *       bcParams.extraInTangentialDirections=0;
// *       bcParams.setBoundaryConditionForcingOption(BoundaryConditionParameters::unSpecifiedForcing);
// * 
// *     }

    }

    if( assignSuperSonicOutflow )
    {
        if( debug() & 64 )
        {
            char buff[80];
            ::display(u,sPrintF(buff,"cnsBC:Before supersonic outflow (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%7.4f ");
        }
        const int orderOfExtrapolationForOutflow = parameters.dbase.get<int >("orderOfExtrapolationForOutflow");
        
    // superSonicOutflow:
        if( orderOfExtrapolationForOutflow<=0 )
        { // By default just use Neumann conditions at outflow. This works best if the flow is nearly normal to the outflow boundary
      // (1) set rho.n=, u.n=, v.n=, [w.n=], T.n=
            u.applyBoundaryCondition(C,neumann,superSonicOutflow,0.,t);
        }
        else
        {
            extrapParams.orderOfExtrapolation=orderOfExtrapolationForOutflow;
            u.applyBoundaryCondition(C,extrapolate,superSonicOutflow,0.,t,extrapParams);
        }
        
        if( debug() & 64 )
        {
            char buff[80];
            ::display(u,sPrintF(buff,"cnsBC:After supersonic outflow (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%7.4f ");
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
        if( pde!=CnsParameters::compressibleMultiphase )
        {
      //u.applyBoundaryCondition(rc,extrapolate,   noSlipWall,0.,t,extrapParams);
            extrapParams.orderOfExtrapolation=2;
      //      u.applyBoundaryCondition(rc,neumann,   noSlipWall,0.,t);
      //      u.applyBoundaryCondition(tc,neumann,noSlipWall,0.,t,extrapParams);
      //      u.applyBoundaryCondition(V,extrapolate,noSlipWall,0.,t,extrapParams);

            if (  (!twilightZoneFlow && 
                        (parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit||
                          parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateNewton) ))
            {
	// Not TZ and implicit method or steadyStateNewton

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
      	cnsNoSlipWallBC(mg.numberOfDimensions(),
                  			uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
                  			uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
                  			pu,px,pdet,prsxy,ipar,rpar,gidLocal.getDataPointer(),bcLocal.getDataPointer());

            } 
            else  // -- noSlipWall --
            {

	//  u.applyBoundaryCondition(rc,neumann,   noSlipWall,0.,t);
	//   u.applyBoundaryCondition(V,extrapolate,noSlipWall,0.,t);
	//   u.applyBoundaryCondition(tc,extrapolate,noSlipWall,0.,t);

	// new way -- *wdh* 070106 

      	u.applyBoundaryCondition(rc,neumann,noSlipWall,0.,t);
      	u.applyBoundaryCondition( V,extrapolate,noSlipWall,0.,t);

	// extrap ghost values of T on faces where T=given
              if( assignTemperature )
              {
                  FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
                  FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
                  ForBoundary(side,axis)
                  {
                      if( mg.boundaryCondition(side,axis)==noSlipWall )
                      {
                          if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                          { // This is an interface between domains
               // for now we only know about interfaces at no-slip walls: 
                              assert( mg.boundaryCondition(side,axis)==noSlipWall );
      	 // what about BC's applied at t=0 before the boundary data is set ??
      	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
      	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
      	 // to use the boundary data instead.
                              #ifdef USE_PPP
                                  realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                              #else
                                  const realSerialArray & uLocal = u;
                              #endif
                   	 Index Ib1,Ib2,Ib3;
                   	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                   	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
                   	 if( debug() & 4 )
                   	 {
                     	   printP("cnsBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                          		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                     	   fprintf(pDebugFile,"cnsBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                     			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                     	   if( pBoundaryData[side][axis]==NULL )
                     	   {
                       	     if( !ok )
                                          fprintf(pDebugFile," cnsBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                      else
                                          fprintf(pDebugFile," cnsBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                     	   }
                     	   else
                     	   {
      	     // RealArray & bd = *pBoundaryData[side][axis];
      	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                     	   }
                   	 }
                              assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
                   	 if( ok && pBoundaryData[side][axis]==NULL )
                   	 {
                 // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
                 // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
                 // based on the current solution
                                  RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                                  bd=0.;
                                  #ifdef USE_PPP
                       	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                                  #else
                       	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                                  #endif
                     	   real a0=mixedCoeff(tc,side,axis,grid);
                     	   real a1=mixedNormalCoeff(tc,side,axis,grid);
                 // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                                  MappedGridOperators & op = *(u.getOperators());
                                  Range N(tc,tc);
                                  RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                     	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                     	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                                  if( mg.numberOfDimensions()==2 )
                     	   {
                                      bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                        				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                     	   }
                     	   else
                     	   {
                                      RealArray uz(Ib1,Ib2,Ib3,N);
                       	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                       	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                        				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
                                        				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                     	   }
                                  const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                     	   if( false && twilightZoneFlow ) //  *******************************************************
                     	   {
                                      fprintf(pDebugFile," cnsBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                       	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                       	     realArray & x= mg.center();
            #ifdef USE_PPP
                       	     realSerialArray xLocal; 
                       	     if( !rectangular || twilightZoneFlow ) 
                         	       getLocalArrayWithGhostBoundaries(x,xLocal);
            #else
                       	     const realSerialArray & xLocal = x;
            #endif
                       	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                       	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                       	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                       	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                       	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                       	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
                       	     if( mg.numberOfDimensions()==2 )
                       	     {
                         	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                       	     }
                       	     else
                       	     {
                         	       realSerialArray uez(Ib1,Ib2,Ib3);
                         	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
                         	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                       	     }
                     	   } // *******************************************
                   	 }
                   	 if( pBoundaryData[side][axis]!=NULL )
                     	   u.getOperators()->setTwilightZoneFlow( false );
                   	 else
                   	 {
                                  if( t>0. || debug() & 4 )
                     	   {
                                      if( ok )
                                 	       printP("$$$$ cnsBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                     	   }
                   	 }
                          }
                          if( debug() & 4 )
                   	 printF("++++cnsBC: noSlipWall: (grid,side,axis)=(%i,%i,%i) : "
                        		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                        		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                        		grid,side,axis, 
                        		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
                        		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                        		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                     	   );
      //        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
      //        {
      // 	 mixedBoundaryConditionOnTemperature=true;
      // 	 if( debug() & 4 )
      // 	   printF("++++insBC: noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
      // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
      //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
      // 		  grid,side,axis, 
      //                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
      //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
      //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
      // 	     );
      //        }
                          if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
                          {
      	 // Dirichlet
                   	 u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
      // 	      u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
      // 				       bcParams,grid);
                          }
                          else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                          {
               // -- Variable Coefficient Temperature (const coeff.) BC --- 
                   	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                        		grid,side,axis);
               // BC is : a0(x)*T + an(x)*T.n = g 
               //  a0 = varCoeff(i1,i2,i3,0)
               //  an = varCoeff(i1,i2,i3,1)
                   	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                            BoundaryData::variableCoefficientTemperatureBC,side,axis );
                              bcParams.setVariableCoefficientsArray(&varCoeff);
                   	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				  bcParams,grid);
                              bcParams.setVariableCoefficientsArray(NULL);  // reset 
                          } 
                          else
                          {
      	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
                          }
                          if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                          { // reset TZ
                   	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                          }
                      } // end if bc = noSlipWall
                  } // end for boundary
         // ************ try this ********* 080909
         // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
              } // end if assignTemperature
        // apply a mixed BC on faces where the BC for T is a mixed
              if( assignTemperature )
              {
                  FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
                  FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
                  ForBoundary(side,axis)
                  {
                      if( mg.boundaryCondition(side,axis)==noSlipWall )
                      {
                          if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                          { // This is an interface between domains
               // for now we only know about interfaces at no-slip walls: 
                              assert( mg.boundaryCondition(side,axis)==noSlipWall );
      	 // what about BC's applied at t=0 before the boundary data is set ??
      	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****
      	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
      	 // to use the boundary data instead.
                              #ifdef USE_PPP
                                  realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
                              #else
                                  const realSerialArray & uLocal = u;
                              #endif
                   	 Index Ib1,Ib2,Ib3;
                   	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                   	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);
                   	 if( debug() & 4 )
                   	 {
                     	   printP("cnsBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                          		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                     	   fprintf(pDebugFile,"cnsBC:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
                                     			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
                     	   if( pBoundaryData[side][axis]==NULL )
                     	   {
                       	     if( !ok )
                                          fprintf(pDebugFile," cnsBC:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
                                      else
                                          fprintf(pDebugFile," cnsBC:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
                     	   }
                     	   else
                     	   {
      	     // RealArray & bd = *pBoundaryData[side][axis];
      	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
                     	   }
                   	 }
                              assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );
                   	 if( ok && pBoundaryData[side][axis]==NULL )
                   	 {
                 // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
                 // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
                 // based on the current solution
                                  RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                                  bd=0.;
                                  #ifdef USE_PPP
                       	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                                  #else
                       	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
                                  #endif
                     	   real a0=mixedCoeff(tc,side,axis,grid);
                     	   real a1=mixedNormalCoeff(tc,side,axis,grid);
                 // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --
                                  MappedGridOperators & op = *(u.getOperators());
                                  Range N(tc,tc);
                                  RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
                     	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
                     	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
                                  if( mg.numberOfDimensions()==2 )
                     	   {
                                      bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                        				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                     	   }
                     	   else
                     	   {
                                      RealArray uz(Ib1,Ib2,Ib3,N);
                       	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
                       	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
                                        				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
                                        				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
                     	   }
                                  const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
                     	   if( false && twilightZoneFlow ) //  *******************************************************
                     	   {
                                      fprintf(pDebugFile," cnsBC:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
                       	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
                       	     realArray & x= mg.center();
            #ifdef USE_PPP
                       	     realSerialArray xLocal; 
                       	     if( !rectangular || twilightZoneFlow ) 
                         	       getLocalArrayWithGhostBoundaries(x,xLocal);
            #else
                       	     const realSerialArray & xLocal = x;
            #endif
                       	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                       	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
                       	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
                       	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
                       	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);
                       	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
                       	     if( mg.numberOfDimensions()==2 )
                       	     {
                         	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                       	     }
                       	     else
                       	     {
                         	       realSerialArray uez(Ib1,Ib2,Ib3);
                         	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
                         	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
                                          				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
                       	     }
                     	   } // *******************************************
                   	 }
                   	 if( pBoundaryData[side][axis]!=NULL )
                     	   u.getOperators()->setTwilightZoneFlow( false );
                   	 else
                   	 {
                                  if( t>0. || debug() & 4 )
                     	   {
                                      if( ok )
                                 	       printP("$$$$ cnsBC:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
                     	   }
                   	 }
                          }
                          if( debug() & 4 )
                   	 printF("++++cnsBC: noSlipWall: (grid,side,axis)=(%i,%i,%i) : "
                        		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
                        		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
                        		grid,side,axis, 
                        		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
                        		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
                        		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
                     	   );
      //        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
      //        {
      // 	 mixedBoundaryConditionOnTemperature=true;
      // 	 if( debug() & 4 )
      // 	   printF("++++insBC: noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
      // 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
      //                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
      // 		  grid,side,axis, 
      //                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
      //                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
      //                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
      // 	     );
      //        }
                          if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
                          {
      	 // Dirichlet
                          }
                          else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
                          {
               // -- Variable Coefficient Temperature (const coeff.) BC --- 
                   	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
                        		grid,side,axis);
               // BC is : a0(x)*T + an(x)*T.n = g 
               //  a0 = varCoeff(i1,i2,i3,0)
               //  an = varCoeff(i1,i2,i3,1)
                   	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                                                                            BoundaryData::variableCoefficientTemperatureBC,side,axis );
                              bcParams.setVariableCoefficientsArray(&varCoeff);
                   	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                      				  bcParams,grid);
                              bcParams.setVariableCoefficientsArray(NULL);  // reset 
                          } 
                          else
                          {
      	 // --- Mixed or Neumann Temperature (const coeff.) BC ---
      	 // Mixed BC or Neumann
                   	 real a0=mixedCoeff(tc,side,axis,grid);
                   	 real a1=mixedNormalCoeff(tc,side,axis,grid);
                   	 bcParams.a.redim(3);
                   	 if( a0==0. && a1==1. )
                   	 {
                     	   if( debug() & 4 )
                       	     printF("++++cnsBC: noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : apply neumannBC\n",
                            		    grid,side,axis);
      //                 real b0=bcData(tc+2,side,axis,grid);
      // 		u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),b0,t); // b0 ignored??
                     	   bcParams.a(0)=a0;
                     	   bcParams.a(1)=a1;
                     	   bcParams.a(2)=mixedRHS(tc,side,axis,grid);  // this is not used -- this does not work
                     	   if( false )
                     	   {  // **** TEMP FIX ****
                       	     u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
                     	   }
                     	   else
                     	   {
                       	     u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                          				      bcParams,grid);
                     	   }
                   	 }
                   	 else
                   	 {
                     	   if( debug() & 4 )
                     	   {
                       	     fPrintF(pDebugFile,"++++cnsBC:noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
                            		    "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f (t=%8.2e)\n",
                             		     grid,side,axis,a0,a1,bcData(tc,side,axis,grid),t);
                     	   }
                     	   if( debug() & 4 )
                     	   {
                                      #ifndef USE_PPP
                            	      Index Ib1,Ib2,Ib3;
                        	      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                                        RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
                        	      ::display(bd(Ib1,Ib2,Ib3,tc),"cnsBC:noSlipWall:T: RHS for mixed BC: bd(Ib1,Ib2,Ib3,tc)",pDebugFile,"%5.2f ");
                            	      Index Ig1,Ig2,Ig3;
      	      // getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
                        	      getIndex(mg.gridIndexRange(),Ig1,Ig2,Ig3,1);
                                        ::display(u(Ig1,Ig2,Ig3,tc),"cnsBC:noSlipWall:T: BEFORE mixed BC: u(I1,I2,I3,tc)",pDebugFile,"%5.2f ");
                                      #endif
                     	   }
                     	   bcParams.a(0)=a0;
                     	   bcParams.a(1)=a1;
                     	   bcParams.a(2)=mixedRHS(tc,side,axis,grid); 
                     	   if( false )
                     	   {  // **** TEMP FIX ****
                       	     u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
                     	   }
                     	   else
                     	   {
                       	     u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
                                          				      bcParams,grid);
                     	   }
                     	   if( debug() & 4 )
                     	   {
                                      #ifndef USE_PPP
                            	      Index Ig1,Ig2,Ig3;
      	      // getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
                        	      getIndex(mg.gridIndexRange(),Ig1,Ig2,Ig3,1);
                                        ::display(u(Ig1,Ig2,Ig3,tc),"cnsBC:noSlipWall:T: AFTER mixed BC: u(I1,I2,I3,tc)",pDebugFile,"%5.2f ");
                                      #endif
                     	   }
                   	 }
                          }
                          if( interfaceType(side,axis,grid)!=Parameters::noInterface )
                          { // reset TZ
                   	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
                          }
                      } // end if bc = noSlipWall
                  } // end for boundary
         // ************ try this ********* 080909
         // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
              } // end if assignTemperature

// 	if( !adiabaticNoSlipWall )
// 	{
// 	  u.applyBoundaryCondition(tc,extrapolate,noSlipWall,0.,t);
// 	}
// 	else
// 	{
// 	  // Some noSlipWall are adiabatic: A Neumann or Mixed BC on T
// 	  bcParams.a.redim(3);
// 	  bcParams.a=0.;
// 	  for( int side=0; side<=1; side++ )
// 	  {
// 	    for( int axis=0; axis<numberOfDimensions; axis++ )
// 	    {
// 	      if( mg.boundaryCondition(side,axis)==noSlipWall )
// 	      {
// 		if( mixedNormalCoeff(tc)==0. ) // coeff of T.n 
// 		{
// 		  // Dirichlet BC on T -- extrap the ghost points:
// 		  u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
// 		}
// 		else
// 		{
// 		  // Mixed BC or Neumann
// 		  real a0=mixedCoeff(tc,side,axis,grid);
// 		  real a1=mixedNormalCoeff(tc,side,axis,grid);
// 		  if( a0==0. && a1==1. )
// 		  {
// 		    if( debug() & 8 )
// 		      printF("++++noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : apply neumannBC\n",
// 			     grid,side,axis);

// 		    // u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),0.,t);
// 		    u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
// 					     Overture::defaultBoundaryConditionParameters(),grid);
// 		  }
// 		  else
// 		  {
// 		    if( true || debug() & 8 )
// 		      printF("++++noSlipWall:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
// 			     "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f\n",
// 			     grid,side,axis,a0,a1,bcData(tc,side,axis,grid));

// 		    bcParams.a(0)=a0;
// 		    bcParams.a(1)=a1;
// 		    bcParams.a(2)=mixedRHS(tc);
// 		    u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
// 					     bcParams,grid);
// 		  }
            		
// 		}
          	    
// 	      }
// 	    }
// 	  }
        	  
// 	}
      	

                bool assignRhoFromEOS = true;
                if( assignRhoFromEOS )
      	{
	  // Apply a BC on rho derived from the p=rho*Rg*T and p.n=...

        	  real *px, *prsxy, *pu;
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
        	  real *pgv = gridVelocityLocal.getDataPointer();
                    real *pgtt = pgv;  // do this for now -- not used 

        	  int ipar[30];
        	  real rpar[30];

                    int useWhereMask=0;
                    int bcOption=0;
                    int ierr=0;
                    int exact; // do this for now

        	  ipar[0]=parameters.dbase.get<int >("rc");
        	  ipar[1]=parameters.dbase.get<int >("uc");
        	  ipar[2]=parameters.dbase.get<int >("vc");
        	  ipar[3]=parameters.dbase.get<int >("wc");
        	  ipar[4]=parameters.dbase.get<int >("tc");
        	  ipar[5]=parameters.dbase.get<int >("sc");
        	  ipar[6]=parameters.dbase.get<int >("numberOfSpecies");
        	  ipar[7]=grid;
        	  ipar[8]=gridType;
        	  ipar[9]=parameters.dbase.get<int >("orderOfAccuracy");
        	  ipar[10]=gridIsMoving;
        	  ipar[11]=useWhereMask;
        	  ipar[12]=parameters.isAxisymmetric();
        	  ipar[13]=twilightZoneFlow;
        	  ipar[14]=bcOption;
        	  ipar[15]=parameters.dbase.get<int >("debug");
        	  ipar[16]=knownSolution; 
        	  ipar[17]=nc;
        	  ipar[18]=parameters.dbase.get<int >("radialAxis"); 
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
        	  rpar[10]=parameters.dbase.get<OGFunction* >("exactSolution")!=NULL ? 
                                      (real &)parameters.dbase.get<OGFunction* >("exactSolution") : 0.;  // twilight zone pointer

                    const ArraySimpleFixed<real,3,1,1,1> &gravity=parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");
                    rpar[11]=gravity(0);
        	  rpar[12]=gravity(1);
        	  rpar[13]=gravity(2);
                    rpar[14]=parameters.dbase.get<real>("mu");
                    rpar[15]=parameters.dbase.get<real>("kThermal");
                    rpar[16]=parameters.dbase.get<real>("Rg");
        	  
        	  cnsNoSlipBC(mg.numberOfDimensions(),
                  		      uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
                  		      uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
                  		      ipar[0],rpar[0], *pu, 
                  		      *pgv, *pgtt, *maskLocal.getDataPointer(), *px, *prsxy, 
                  		      bcLocal(0,0), gidLocal(0,0), exact, 
                  		      *uKnownLocal.getDataPointer(), ierr );


      	}

            }
        }
        else
        {
      // compressible multi-phase case 
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

        if( pde!=CnsParameters::compressibleMultiphase &&
      !(parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit  ||
      	parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateNewton))
        {
      // u.applyBoundaryCondition(rc,extrapolate,subSonicInflow,0.,t);
            u.applyBoundaryCondition(rc,mixed,subSonicInflow,bcData,t,bcParams,grid);
            u.applyBoundaryCondition(V,extrapolate, subSonicInflow,0.,t);
      // u.applyBoundaryCondition(tc,neumann,    subSonicInflow,0.,t);
            u.applyBoundaryCondition(tc,mixed,subSonicInflow,bcData,t,bcParams,grid);
        }
        else if ( pde==CnsParameters::compressibleMultiphase  )
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

        if( pde!=CnsParameters::compressibleMultiphase )
        {
      // *wdh* 040929  use neumann BC's -- these are more stable
            if (!(parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit||
          	    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateNewton))
      	{

        	  u.applyBoundaryCondition(rc,neumann,subSonicOutflow,0.,t);
        	  u.applyBoundaryCondition(V,neumann,subSonicOutflow,0.,t);
        	  
	  // fill in the coefficients for the mixed derivative BC:
                	  bcParams.a.redim(3,2,bcData.getLength(2),grid+1); bcParams.a=0.;
                    
                    for( int side=0; side<=1; side++ )for( int axis=0; axis<numberOfDimensions; axis++ )
        	  {
          	    bcParams.a(0,side,axis,grid)=mixedCoeff(tc,side,axis,grid);
          	    bcParams.a(1,side,axis,grid)=mixedNormalCoeff(tc,side,axis,grid);
        	  }
        	  
//	  for( int i=0; i<=1; i++ )
//	    bcParams.a(i,all,all,grid)=bcData(tc+numberOfComponents*(i+1),all,all,grid);
        	  u.applyBoundaryCondition(tc,mixed,subSonicOutflow,bcData,t,bcParams,grid);
      	}
        }
        else
        {
            for( int m=0; m<numberOfPhases; m++ )
            {
      	u.applyBoundaryCondition(mpRho[m],neumann,subSonicOutflow,0.,t);
      	u.applyBoundaryCondition(mpV[m],neumann,subSonicOutflow,0.,t);

	// fill in the coefficients for the mixed derivative BC:
      	bcParams.a.redim(3,2,bcData.getLength(2),grid+1); bcParams.a=0.;

      	for( int side=0; side<=1; side++ )for( int axis=0; axis<numberOfDimensions; axis++ )
      	{
        	  bcParams.a(0,side,axis,grid)=mixedCoeff(mpT[m],side,axis,grid);        // check this ****** 070623
        	  bcParams.a(1,side,axis,grid)=mixedNormalCoeff(mpT[m],side,axis,grid);  // check this ****** 070623
      	}
      	
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

    if ( false &&  parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateNewton)
        {
            int ipar[20];
            real rpar[20];
            ipar[0] = mg.numberOfDimensions();
            ipar[1]=nc;
            ipar[2] = rc;
            ipar[3] = uc;
            ipar[4] = vc;
            ipar[5] = wc;
            ipar[6] = tc;
            ipar[7] = gridIsMoving;
            ipar[8] = parameters.isAxisymmetric();
            ipar[9] = parameters.dbase.get<bool >("axisymmetricWithSwirl");

            ipar[15]= parameters.dbase.get<int >("debug");
            ipar[18]= parameters.dbase.get<int >("radialAxis"); 
            ipar[19]= grid;
            
            const real mu = parameters.dbase.get<real >("mu");
            const real gamma = parameters.dbase.get<real >("gamma");
            const real kThermal = parameters.dbase.get<real >("kThermal");
            const real Rg = parameters.dbase.get<real >("Rg");
            const real reynoldsNumber = parameters.dbase.get<real >("reynoldsNumber");
            const real prandtlNumber = parameters.dbase.get<real >("prandtlNumber");
            const real machNumber = parameters.dbase.get<real >("machNumber");
            const ArraySimpleFixed<real,3,1,1,1> &gravity=parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");
            
            rpar[0]=reynoldsNumber;
            rpar[1]=prandtlNumber;
            rpar[2]=machNumber;
            rpar[3]=gamma;
            rpar[4]=parameters.dbase.get<real >("implicitFactor");
            rpar[5]=mg.gridSpacing(0);  
            rpar[6]=mg.gridSpacing(1);
            rpar[7]=mg.gridSpacing(2);
            rpar[8]=0; // not used for anything
            
            rpar[9]=parameters.dbase.get<real >("dt");
            rpar[12]= parameters.dbase.get<real >("av2");
            rpar[13]= parameters.dbase.get<real >("av4");
            rpar[14]= gravity[0];
            rpar[15]= gravity[1];
            rpar[16]= gravity[2];
            
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
            const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
            BoundaryConditionParameters bcParams;
            const IntegerArray & bc = mg.boundaryCondition();
            IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
            ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( u,gidLocal,dimLocal,bcLocal ); 
      // determine time dependent conditions:
            getTimeDependentBoundaryConditions( mg,t,grid ); 

            RealArray &ubv = parameters.dbase.get<RealArray >("userBoundaryConditionParameters");
            IntegerArray ubt;
            ubt = parameters.dbase.get<IntegerArray >("bcInfo") - Parameters::numberOfPredefinedBoundaryConditionTypes;
            int nbv = ubv.getLength(0);
            const RealArray &ubd = bcData;
            int nbd = ubd.getLength(0);
            INOUTFLOWEXP(  numberOfDimensions, u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1),
                   		       u.getBase(2),u.getBound(2),u.getBase(3),u.getBound(3),
                   		       u.getDataPointer(), px, pdet, prsxy,
                   		       ipar, rpar, gidLocal.getDataPointer(),bcLocal.getDataPointer(), ubd.getDataPointer(), 
                   		       ubt.getDataPointer(),
                   		       nbd);
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

        if( true || parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeGodunov )
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
        	  if( pde!=CnsParameters::compressibleMultiphase )
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
        	  if( pde!=CnsParameters::compressibleMultiphase )
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


        if( debug() & 64 )
        {
            char buff[80];
            ::display(u,sPrintF(buff,"cnsBC:After assign 2nd-ghost line (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%9.2e ");
        }
        real maxVal=0.;
        if( Parameters::checkForFloatingPointErrors )
            checkSolution(u,"cnsBC:before extrapInterpN",false,grid,maxVal,true);
        
    // extrapolate neighbours of interpolation points for 4th order artificial viscosity
    //kkc 060710    assert( parameters.dbase.get<int >("extrapolateInterpolationNeighbours") );  // consistency check
        if( parameters.dbase.get<int >("extrapolateInterpolationNeighbours") ) 
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



    checkArrayIDs(" cnsBC: before symmetry"); 
    if( debug() & 64 )
    {
        char buff[80];
        ::display(u,sPrintF(buff,"cnsBC:Before assign slipWallOpt: u (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%13.10f ");
    }

  // if( true ) printF("cnsBC:assignSlipWall=%i, useOptSlipWall=%i, slipWallBoundaryConditionOption=%i, t=%9.3e\n",(int)assignSlipWall,int(useOptSlipWall),parameters.dbase.get<int >("slipWallBoundaryConditionOption"),t);

    if( assignSlipWall || assignAxisymmetric || assignFarField )
    { 
    // finish slip wall  or assign assignAxisymmetric

        if( useOptSlipWall || assignAxisymmetric || assignFarField )
        {
            int ierr=0;
            int exact; // do this for now
            real *px, *prsxy, *pu2, *pu;

            pu=uLocal.getDataPointer();
            const bool vertexNeeded = !isRectangular || twilightZoneFlow;
            if( !vertexNeeded  )
            {
      	px = pu;  // not used in this case
            }
            else
            {
                mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );
                #ifdef USE_PPP
            	  px = mg.center().getLocalArray().getDataPointer();
                #else
            	  px = mg.center().getDataPointer();
                #endif
            }
            if( isRectangular  )
            {
      	prsxy=pu; // not used in this case
            }
            else
            {
                mg.update(MappedGrid::THEinverseVertexDerivative );
                #ifdef USE_PPP
        	  prsxy = mg.inverseVertexDerivative().getLocalArray().getDataPointer();
                #else
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

            const int ndpar=50; //kkc 051208 20 was too small for debugging variables
            int ipar[ndpar];
            real rpar[ndpar];

      // parameter( slipWallSymmetry=0, slipWallPressureEntropySymmetry=1 )


            realSerialArray *gtt = NULL; 
            real *pgtt=NULL; 
            if( gridIsMoving && assignSlipWall )
            {
        // get acceleration terms on the boundary for the slip wall BC

                const int nc = mg.numberOfDimensions()*2;  // room for gtt and gttt
                gtt = new realSerialArray(u.dimension(0),u.dimension(1),u.dimension(2),nc); 
      	
                int option=4; // return g.tt 
                if( false && parameters.dbase.get<int >("slipWallBoundaryConditionOption")==4 ) // *** may need g'''
                    option=option+8;  // return g.ttt as well
       	 
        // gtt(i1,i2,i3,0:d-1)  : g.tt
        // gtt(i1,i2,i3,d:2d-1) : g.ttt

	// if( true ) printF("cnsBC:getBoundaryAcceleration t=%9.3e\n",t);

      	parameters.dbase.get<MovingGrids >("movingGrids").getBoundaryAcceleration( mg, *gtt, grid,t, option );
      	pgtt=gtt->getDataPointer(); 
            }
            else
            {
      	pgtt=pgv;  // not used in this case
            }
            


            int bcOption=parameters.dbase.get<int >("slipWallBoundaryConditionOption");

            int useWhereMask=false;
            
            for( int m=0; m<numberOfPhases; m++ )
            {

      	if( pde!=CnsParameters::compressibleMultiphase )
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
      	ipar[13]=twilightZoneFlow;
      	ipar[14]=bcOption;
      	ipar[15]=parameters.dbase.get<int >("debug");
      	ipar[16]=knownSolution; 
                ipar[17]=nc;

                ipar[18]=parameters.dbase.get<int >("radialAxis");  // =axis1 if x=0 is axis of cylindrical symmetry, =axis2 if y=0 is..
                ipar[19]=parameters.dbase.get<bool >("axisymmetricWithSwirl");
                ipar[20]=parameters.dbase.get<int>("applyInterfaceBoundaryConditions");

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

                assert( 20+nc < ndpar );
                for( int m=0; m<nc; m++ )
      	{
        	  rpar[20+m]=parameters.dbase.get<RealArray>("artificialDiffusion")(m); 
      	}
      	
                DataBase *pdb = &parameters.dbase;
                if( (assignSlipWall || assignAxisymmetric) && parameters.dbase.get<int >("slipWallBoundaryConditionOption")!=4 )
      	{
        	  cnsSlipWallBC(mg.numberOfDimensions(),
                  			uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
                  			uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
                  			ipar[0],rpar[0], *pu, 
                  			*pu2, *pgv, *pgv2, *pgtt, *maskLocal.getDataPointer(), *px, *prsxy, 
                  			bcLocal(0,0), gidLocal(0,0), exact, 
                  			*uKnownLocal.getDataPointer(), pdb, ierr );
        	  
      	}
      	else if( (assignSlipWall || assignAxisymmetric) )
      	{
	  // slip-wall derivative conditions
        	  cnsSlipWallBC2(mg.numberOfDimensions(),
                   			 uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
                   			 uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
                   			 ipar[0],rpar[0], *pu, 
                   			 *pu2, *pgv, *pgv2, *pgtt, *maskLocal.getDataPointer(), *px, *prsxy, 
                   			 bcLocal(0,0), gidLocal(0,0), interfaceType(0,0,grid), exact, 
                   			 *uKnownLocal.getDataPointer(), pdb, ierr );
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

            if( debug() & 16 )
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
      	if( pde!=CnsParameters::compressibleMultiphase )
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
      	if( pde!=CnsParameters::compressibleMultiphase )
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
            assert( pde!=CnsParameters::compressibleMultiphase );
            
      // **** old way
            if( true &&  // 010629: try turning this off **************************************************
        	  parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeGodunov )
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
        if( pde!=CnsParameters::compressibleMultiphase )
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
        if( true || parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeGodunov )
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
    if( useNewSymmetryBC && !twilightZoneFlow )
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
        if( debug() & 16 )
        {
            char buff[80];
            ::display(u,sPrintF(buff,"cnsBC:After new symmetry BC: u (t=%8.2e)",t),parameters.dbase.get<FILE* >("pDebugFile"),"%6.3f ");
        }
    }
    
  // numSent=getSent();
  // if( parameters.dbase.get<int >("myid")==0 ) printf("******* cnsBC: After finishBoundaryConditions: new messages sent=%i\n",numSent);

    if( !useNewSymmetryBC )  // ====================================
    {
        

    if( debug() & 16 )
    {
        char buff[80];
        ::display(u,sPrintF(buff,"cnsBC:After finishBoundaryConditions (before corner symmetry): u (t=%9.2e)",t),
                            parameters.dbase.get<FILE* >("pDebugFile"),"%9.2e ");
    }
    
  // ** now assign symmetry type boundary conditions at corner points.
    if( !isRectangular ) mg.update(MappedGrid::THEvertexBoundaryNormal ); 
    real na[3],nb[3];
#ifdef USE_PPP
    for( int axis=0; axis<3; axis++ )
    {
        na[axis] = uLocal.getBase(axis) +u.getGhostBoundaryWidth(axis);
        nb[axis] = uLocal.getBound(axis)-u.getGhostBoundaryWidth(axis);
    }
#else
    for( int axis=0; axis<3; axis++ )
    {
        na[axis] = uLocal.getBase(axis);
        nb[axis] = uLocal.getBound(axis);
    }
#endif

    const real normEps = REAL_MIN*100.;
    for( int m=0; m<numberOfPhases; m++ )
    {
        int rc=parameters.dbase.get<int >("rc"), uc=parameters.dbase.get<int >("uc"), vc=parameters.dbase.get<int >("vc"), wc=parameters.dbase.get<int >("wc"), tc=parameters.dbase.get<int >("tc");
          
        if( pde==CnsParameters::compressibleMultiphase )
        {
            rc=mpRho[m];
            uc=mpV[m].getBase();
            vc=uc+1;
            wc=mpV[m].getBound();
            tc=mpT[m];
        }
        
        if( mg.numberOfDimensions()==2 && !twilightZoneFlow )
        {
      // ---------------------------------------------------------------
      // ---------------- Symmetry Fixup for 2D ------------------------
      // ---------------------------------------------------------------

            int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
            int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
            int kv[3], &k1=kv[0], &k2=kv[1], &k3=kv[2];
            int is[3]={0,0,0};
            real nv[3]; // holds normal 
            i3=mg.gridIndexRange(0,axis3);
            j3=i3; k3=i3;

            for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
            {
      	int axisp1 = (axis+1) % mg.numberOfDimensions();
      	for( int side=0; side<=1; side++ )
      	{
        	  if( mg.boundaryCondition(side,axis)==slipWall || 
            	      mg.boundaryCondition(side,axis)==symmetry  ||
                            mg.boundaryCondition(side,axis)==axisymmetric ) // *wdh* 070509
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
            		if(  (mg.boundaryCondition(side,axis)==slipWall ||
                                            mg.boundaryCondition(side,axis)==axisymmetric ) &&
                 		     ( mg.boundaryCondition(side2,axisp1)==symmetry ||
                   		       mg.boundaryCondition(side2,axisp1)<0) ) // *wdh* 050510 
            		{
		  // skip this side -- apply symmetry condition instead on adjacent side
              		  continue;
            		}
		// * if( gridIsMoving &&
		// *     mg.boundaryCondition(side,axis)==slipWall && 
		// *     mg.boundaryCondition(side2,axisp1)>0 )
                // *   // ??    mg.boundaryCondition(side2,axisp1)!=axisymmetric) ) // *wdh* 080329 : 
		// * {
                // *   // *wdh* 080329 -- I think that a slip wall next to an axisymmetric wall is ok 
                // * 
		// *   printf("cnsBC:ERROR:Symmetry corner condition -- fix this case for moving grids\n");
		// *   printf(" grid=%i, mg.boundaryCondition(side,axis)=%i, bc(side2,axisp1)=%i\n",
		// * 	 grid,mg.boundaryCondition(side,axis),mg.boundaryCondition(side2,axisp1));
		// *   // Overture::abort("ERROR");
		// * }

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
                                    if( pde==CnsParameters::compressibleMultiphase )
                                        uLocal(i1,i2,i3,mpA)=uLocal(j1,j2,j3,mpA);
                                
              		  uLocal(i1,i2,i3,uc)=uLocal(j1,j2,j3,uc);
              		  uLocal(i1,i2,i3,vc)=uLocal(j1,j2,j3,vc);
              		  if( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
              		  {
                		    uLocal(i1,i2,i3,wc)=uLocal(j1,j2,j3,wc);
              		  }
              		  
		  // velocity: n.u is odd
		  //  n.u(-1) = - n.u(+1)
		  // u(-1) <- u(-1) - (n.u)(-1)*n + [ 2*(n.u)(0) -(n.u)(+1)]*n 
		  // * real ndum=nv[0]*uLocal(i1,i2,i3,uc)+nv[1]*uLocal(i1,i2,i3,vc);
		  // * real ndup=nv[0]*uLocal(j1,j2,j3,uc)+nv[1]*uLocal(j1,j2,j3,vc);
		  // * uLocal(i1,i2,i3,uc)-=(ndup+ndum)*nv[0];
		  // * uLocal(i1,i2,i3,vc)-=(ndup+ndum)*nv[1];
            		
                  // *fix* for moving grids *wdh* 081106 do not assume velocity on wall is zero
                  // n.u(-1) = 2*n.u(0) - n.u(+1)
		  // u(-1) <- u(-1) - (n.u)(-1)*n -(n.u)(+1)*n
                                    real ndum=nv[0]*uLocal(i1,i2,i3,uc)+nv[1]*uLocal(i1,i2,i3,vc);
              		  real ndup=(nv[0]*(uLocal(j1,j2,j3,uc)-2.*uLocal(k1,k2,k3,uc))+
                                                          nv[1]*(uLocal(j1,j2,j3,vc)-2.*uLocal(k1,k2,k3,vc)));
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
        else if( mg.numberOfDimensions()==3 && !twilightZoneFlow )
        {
      // ---------------------------------------------------------------
      // ---------------- Symmetry Fixup for 3D ------------------------
      // ---------------------------------------------------------------

//      printf("***WARNING*** applyBoundaryConditionsCNS: symmetry correction at corners at not yet been "
//             "implemented in 3D\n");
            Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
            Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
            Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
            int is[3]={0,0,0};
            for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
            {
      	for( int side=0; side<=1; side++ )
      	{
        	  if( mg.boundaryCondition(side,axis)==slipWall ||
            	      mg.boundaryCondition(side,axis)==symmetry ||
                            mg.boundaryCondition(side,axis)==axisymmetric ) // *wdh* 070509 -- but this shouldn't happen in 3d
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
            // define normal(I1,I2,I3,dir) rx(I1,I2,I3,axis+numberOfDimensions*(dir))
                        const RealArray & normal = !isRectangular ? mg.vertexBoundaryNormalArray(side,axis) :
                                                                                                                uLocal;  // *wdh* 060703
#else
          	    const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
#endif
          	    is[axis]=1-2*side;

          	    for( int dir=0; dir<=1; dir++ ) // two tangential directions
          	    {
            	      int axisp = (axis+1+dir) % mg.numberOfDimensions();

            	      getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3,extra);

	      // Here is the third direction besides axis and axisp:
            	      const int axisn = (axis+1+1-dir) % mg.numberOfDimensions();  // (axis,axisp,axisn)
            	      assert( axisn!=axis && axisn!=axisp );
	      // limit points to fit on the local array (*wdh* 050321)
            	      const int n1a=max(Iv[axisn].getBase(), uLocal.getBase(axisn));
            	      const int n1b=min(Iv[axisn].getBound(),uLocal.getBound(axisn));
          	    
            	      if( n1a>n1b ) continue;   // no points on this processor
            	      Iv[axisn]=Range(n1a,n1b);
          	    

            	      J1=I1, J2=I2, J3=I3;
            	      K1=I1, K2=I2, K3=I3;


            	      for( int ghost=1; ghost<=2; ghost++ )
            	      {
            		Iv[axis]=mg.gridIndexRange(side,axis)-is[axis]*ghost;  // ghost line
            		Jv[axis]=mg.gridIndexRange(side,axis)+is[axis]*ghost;  // line inside
            		Kv[axis]=mg.gridIndexRange(side,axis);                 // boundary point
                        
		// skip assignment of points that are not on this processor
            		if( mg.gridIndexRange(side,axis)<na[axis] || mg.gridIndexRange(side,axis)>nb[axis] ) continue;

            		for( int side2=0; side2<=1; side2++ )  // do bottom or top of direction axisp
            		{
              		  Kv[axisp]=mg.gridIndexRange(side2,axisp);  // kv holds the corner point now *** could do both at once
              		  if( mg.gridIndexRange(side2,axisp)<na[axisp] || mg.gridIndexRange(side2,axisp)>nb[axisp] ) continue;

              		  for( int ghost2=1; ghost2<=2; ghost2++ )
              		  {
                		    Iv[axisp]=mg.gridIndexRange(side2,axisp)-(1-2*side2)*ghost2;  
                		    Jv[axisp]=Iv[axisp];
                		    if( false && debug() & 1 )
                		    {
                  		      printf(" assign 3D symmetry at edges iv=(%i,%i)(%i,%i)(%i,%i) jv=(%i,%i)(%i,%i)(%i,%i) "
                                                          "gir=[%i,%i]x[%i,%i]x[%i,%i]\n",
                       			     I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
                       			     J1.getBase(),J1.getBound(),J2.getBase(),J2.getBound(),J3.getBase(),J3.getBound(),
                       			     mg.gridIndexRange(0,0),mg.gridIndexRange(1,0),mg.gridIndexRange(0,1),mg.gridIndexRange(1,1),
                       			     mg.gridIndexRange(0,2),mg.gridIndexRange(1,2));
                		    }
                		    
                		    uLocal(I1,I2,I3,rc)=uLocal(J1,J2,J3,rc);
                		    uLocal(I1,I2,I3,tc)=uLocal(J1,J2,J3,tc);
                		    if( numberOfSpecies>0 )
                  		      uLocal(I1,I2,I3,S)=uLocal(J1,J2,J3,S);
                		    if( pde==CnsParameters::compressibleMultiphase )
                  		      uLocal(I1,I2,I3,mpA)=uLocal(J1,J2,J3,mpA);
                                
                		    if( isRectangular )
                		    {
		      // velocity: n.u is odd, tangential components are even
                  		      if( axis==0 )
                  			uLocal(I1,I2,I3,uc)=2.*uLocal(K1,K2,K3,uc)-uLocal(J1,J2,J3,uc);
                  		      else
                  			uLocal(I1,I2,I3,uc)= uLocal(J1,J2,J3,uc);
                  		      if( axis==1 )
                  			uLocal(I1,I2,I3,vc)=2.*uLocal(K1,K2,K3,vc)-uLocal(J1,J2,J3,vc);
                  		      else
                  			uLocal(I1,I2,I3,vc)= uLocal(J1,J2,J3,vc);
                  		      if( axis==2 )
                  			uLocal(I1,I2,I3,wc)=2.*uLocal(K1,K2,K3,wc)-uLocal(J1,J2,J3,wc);
                  		      else
                  			uLocal(I1,I2,I3,wc)= uLocal(J1,J2,J3,wc);
                		    }
                		    else
                		    {
                  		      uLocal(I1,I2,I3,uc)=uLocal(J1,J2,J3,uc);
                  		      uLocal(I1,I2,I3,vc)=uLocal(J1,J2,J3,vc);
                  		      uLocal(I1,I2,I3,wc)=uLocal(J1,J2,J3,wc);
                                
		      // velocity: n.u is odd
		      //  n.u(-1) = 2*n.u(0) - n.u(+1)
		      // u(-1) <- u(-1) - (n.u)(-1)*n -(n.u)(+1)*n
                      // *wdh* 081106 -- fix for moving --
                  		      RealArray ndum,ndup,norm;
                  		      ndum=(normal(K1,K2,K3,0)*uLocal(I1,I2,I3,uc)+
                      			    normal(K1,K2,K3,1)*uLocal(I1,I2,I3,vc)+
                      			    normal(K1,K2,K3,2)*uLocal(I1,I2,I3,wc));
                  		      ndup=(normal(K1,K2,K3,0)*(uLocal(J1,J2,J3,uc)-2.*uLocal(K1,K2,K3,uc))+
                      			    normal(K1,K2,K3,1)*(uLocal(J1,J2,J3,vc)-2.*uLocal(K1,K2,K3,vc))+
                      			    normal(K1,K2,K3,2)*(uLocal(J1,J2,J3,wc)-2.*uLocal(K1,K2,K3,wc)));
                  		      uLocal(I1,I2,I3,uc)-=(ndup+ndum)*normal(K1,K2,K3,0);
                  		      uLocal(I1,I2,I3,vc)-=(ndup+ndum)*normal(K1,K2,K3,1);
                  		      uLocal(I1,I2,I3,wc)-=(ndup+ndum)*normal(K1,K2,K3,2);

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
    
    if( debug() & 16 )
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

    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForBoundaryConditions"))+=getCPU()-time0;
    return 0;
}
