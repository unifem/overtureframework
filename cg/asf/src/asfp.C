// This file automatically generated from asfp.bC with bpp.
#include "Cgasf.h"
#include "MappedGridOperators.h"
#include "interpPoints.h"
#include "Reactions.h"
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "AdamsPCData.h"
#include "ParallelUtility.h"
#include "App.h"
#include "AsfParameters.h"

//    Mixed-derivative BC for component i: 
//          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)


// Macros for extracting local arrays



#define asfAssignPressureRhs EXTERN_C_NAME(asfassignpressurerhs)
extern "C"
{
    void asfAssignPressureRhs(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
                      			    const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,			    const int&ipar, const real&rpar, 
                      			    real&f, real&u, real&p, real&gam, const int& mask, const real&rsxy, const int&ierr );
}


// This macro is used to loop over the boundaries
#define ForBoundary(side,axis)   for( axis=0; axis<c.numberOfDimensions(); axis++ ) for( side=0; side<=1; side++ )

static int
smoothNearBoundary( realCompositeGridFunction & u, Range & R)
{
    CompositeGrid & cg = *u.getCompositeGrid();
    Index I1b,I2b,I3b;
    const int numberOfIterations =2;
    for( int it=0; it<numberOfIterations; it++ )
    {
    // smooth points on lines near the boundary
        for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
        {
            MappedGrid & mg = cg[grid];
        
            realArray & v = u[grid];
            const real omega=1./8.;
            for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
            {
      	for( int side=Start; side<=End; side++ )
      	{
        	  if( mg.boundaryCondition(side,axis)>0 )
        	  {
          	    getGhostIndex(mg.gridIndexRange(),side,axis,I1b,I2b,I3b,-1,-2); // first line in
          	    v(I1b,I2b,I3b,R)+=omega*( v(I1b-1,I2b,I3b,R)+v(I1b+1,I2b,I3b,R)+v(I1b,I2b+1,I3b,R)+v(I1b,I2b-1,I3b,R)
                              				      -4.*v(I1b,I2b,I3b,R) );
          	    getGhostIndex(mg.gridIndexRange(),side,axis,I1b,I2b,I3b,-2,-2); 
          	    v(I1b,I2b,I3b,R)+=omega*( v(I1b-1,I2b,I3b,R)+v(I1b+1,I2b,I3b,R)+v(I1b,I2b+1,I3b,R)+v(I1b,I2b-1,I3b,R)
                              				      -4.*v(I1b,I2b,I3b,R) );
        	  }
      	}
            }
        }
        u.periodicUpdate();
    }
    return 0;
}

// ================================================================================================
//  Output debugging info the the pressure RHS
// ================================================================================================

// ==================================================================================
//   Output more debugging info the the pressure RHS
// ==================================================================================

// ==================================================================================
// Testing: smooth the rhs to the pressure equation
// ==================================================================================


//     normal derivative of p (outward normal)
#define PN1(I1,I2,I3)  ( (2*side-1)*(mu*uxx(I1,I2,I3,uc) +acRho*uu(I1,I2,I3,uc)*ux(I1,I2,I3,uc) ) )


#define PXB2(I1,I2,I3) ( mu*((4./3.)*uxx(I1,I2,I3,uc)+uyy(I1,I2,I3,uc)+(1./3.)*uxy(I1,I2,I3,vc)) +acRho*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,uc) + uu(I1,I2,I3,vc)*uy(I1,I2,I3,uc)) )

#define PYB2(I1,I2,I3) ( mu*(uxx(I1,I2,I3,vc)+(4./3.)*uyy(I1,I2,I3,vc)+(1./3.)*uxy(I1,I2,I3,uc)) +acRho*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,vc) + uu(I1,I2,I3,vc)*uy(I1,I2,I3,vc)) )

//     normal derivative of p (outward normal)
#define PN2(I1,I2,I3)  ( normal(I1,I2,I3,0)*PXB2(I1,I2,I3)  +normal(I1,I2,I3,1)*PYB2(I1,I2,I3) )
//  ...momentum eqn's in 3d without grad p term

#define PXB3(I1,I2,I3) ( mu*((4./3.)*uxx(I1,I2,I3,uc)+uyy(I1,I2,I3,uc)+uzz(I1,I2,I3,uc)+(1./3.)*(uxy(I1,I2,I3,vc)+uxz(I1,I2,I3,wc)) ) +acRho*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,uc) + uu(I1,I2,I3,vc)*uy(I1,I2,I3,uc)+ uu(I1,I2,I3,wc)*uz(I1,I2,I3,uc)) )

#define PXB3(I1,I2,I3) ( mu*((4./3.)*uxx(I1,I2,I3,uc)+uyy(I1,I2,I3,uc)+uzz(I1,I2,I3,uc)+(1./3.)*(uxy(I1,I2,I3,vc)+uxz(I1,I2,I3,wc)) ) +acRho*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,uc) + uu(I1,I2,I3,vc)*uy(I1,I2,I3,uc)+ uu(I1,I2,I3,wc)*uz(I1,I2,I3,uc)) )

#define PYB3(I1,I2,I3) ( mu*(uxx(I1,I2,I3,vc)+(4./3.)*uyy(I1,I2,I3,vc)+uzz(I1,I2,I3,vc)+(1./3.)*(uxy(I1,I2,I3,uc)+uyz(I1,I2,I3,wc)) ) +acRho*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,vc) + uu(I1,I2,I3,vc)*uy(I1,I2,I3,vc)+ uu(I1,I2,I3,wc)*uz(I1,I2,I3,vc)) )

#define PZB3(I1,I2,I3) ( mu*(uxx(I1,I2,I3,wc)+uyy(I1,I2,I3,wc)+(4./3.)*uzz(I1,I2,I3,wc)+(1./3.)*(uxz(I1,I2,I3,uc)+uyz(I1,I2,I3,vc)) ) +acRho*( uu(I1,I2,I3,uc)*ux(I1,I2,I3,wc) + uu(I1,I2,I3,vc)*uy(I1,I2,I3,wc)+ uu(I1,I2,I3,wc)*uz(I1,I2,I3,wc)) )


//    ...normal derivative of p in 3d (outward normal)
#define PN3(I1,I2,I3) ( normal(I1,I2,I3,0)*PXB3(I1,I2,I3)  +normal(I1,I2,I3,1)*PYB3(I1,I2,I3)  +normal(I1,I2,I3,2)*PZB3(I1,I2,I3) )

#define POW2(x) pow((x),2)
//   ...weight divergence term : 1/dx^2 + 1/dy^2 + 1/dz^2  ******************* compute and save this *******
#define DAI1(cd,I1,I2,I3)  ( cd/ ( POW2(xy(I1+1,I2  ,I3  ,0)-xy(I1-1,I2  ,I3  ,0))  +POW2(xy(I1+1,I2  ,I3  ,1)-xy(I1-1,I2  ,I3  ,1)) ) )

#define DAI2(cd,I1,I2,I3)  ( cd/ ( POW2(xy(I1+1,I2  ,I3  ,0)-xy(I1-1,I2  ,I3  ,0))  +POW2(xy(I1+1,I2  ,I3  ,1)-xy(I1-1,I2  ,I3  ,1)) )  + cd/ ( POW2(xy(I1  ,I2+1,I3  ,0)-xy(I1  ,I2-1,I3  ,0))  +POW2(xy(I1  ,I2+1,I3  ,1)-xy(I1  ,I2-1,I3  ,1)) ) )
#define DAI3(cd,I1,I2,I3)  ( cd/ ( POW2(xy(I1+1,I2  ,I3  ,0)-xy(I1-1,I2  ,I3  ,0))  +POW2(xy(I1+1,I2  ,I3  ,1)-xy(I1-1,I2  ,I3  ,1))  +POW2(xy(I1+1,I2  ,I3  ,2)-xy(I1-1,I2  ,I3  ,2)) )  + cd/ ( POW2(xy(I1  ,I2+1,I3  ,0)-xy(I1  ,I2-1,I3  ,0))  +POW2(xy(I1  ,I2+1,I3  ,1)-xy(I1  ,I2-1,I3  ,1))  +POW2(xy(I1  ,I2+1,I3  ,2)-xy(I1  ,I2-1,I3  ,2)) )  + cd/ ( POW2(xy(I1  ,I2  ,I3+1,0)-xy(I1  ,I2  ,I3-1,0))  +POW2(xy(I1  ,I2  ,I3+1,1)-xy(I1  ,I2  ,I3-1,1))  +POW2(xy(I1  ,I2  ,I3+1,2)-xy(I1  ,I2  ,I3-1,2)) ) )


// extern int n0,n1,n2, m0,m1;

void Cgasf::
solveForAllSpeedPressure( real t, real deltaT, const real & dtRatio )
// ==================================================================================================
// /Purpose:
//   Compute p and px at time t for the implicit time-stepping method
//
// /t (input) : This is the time at which the pressure lives.
// /p (implied input/output): a guess for the pressure on input, and the pressure on output
// /px (implied output): gradient of the pressure
//
//  Solve:
//    (I - (a0*deltaT)^2 gamma P grad( 1/rho grad) ) p(n+1) = stuff
//
//  We form the matrix (multiplied out, non-conservative form)
//
//        I - alpha ( p/r \Delta  - p/r^2 grad(r).\grad )   
//
//     where alpha = [a0*deltaT]^2 gamma
//
// ==================================================================================================
{ 
    real time=getCPU();

  // Get some time-steping state information from the data base
    DataBase & modelData = parameters.dbase.get<DataBase >("modelData");
    if( !modelData.has_key("asfImplicitData") )
        modelData.put<AdamsPCData>("asfImplicitData");
    AdamsPCData & adamsData = modelData.get<AdamsPCData>("asfImplicitData");
    int &n0=adamsData.nab0, &n1=adamsData.nab1, &n2=adamsData.nab2, &m0=adamsData.mab0, &m1=adamsData.mab1; 


  // const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    const int & rc = parameters.dbase.get<int >("rc");
    const int & uc = parameters.dbase.get<int >("uc");
    const int & vc = parameters.dbase.get<int >("vc");
    const int & wc = parameters.dbase.get<int >("wc");
    const int & tc = parameters.dbase.get<int >("tc");
    const int & pc = parameters.dbase.get<int >("pc");
    const real & advectionCoefficient = parameters.dbase.get<real >("advectionCoefficient");
    const int & computeReactions = parameters.dbase.get<bool >("computeReactions");

    const real & mu = parameters.dbase.get<real >("mu");
    const real & gamma = parameters.dbase.get<real >("gamma");
  // const real & kThermal = parameters.dbase.get<real >("kThermal");
  // const real & Rg = parameters.dbase.get<real >("Rg");
  // const real & avr = parameters.dbase.get<real >("avr");
    const real & pressureLevel = parameters.dbase.get<real >("pressureLevel");
  // const real & anu = parameters.dbase.get<real >("anu");
    const real & a0 = parameters.dbase.get<real >("a0");
  // const real & a1 = parameters.dbase.get<real >("a1");
  // const real & a2 = parameters.dbase.get<real >("a2");
  // const real & b0 = parameters.dbase.get<real >("b0");
  // const real & b1 = parameters.dbase.get<real >("b1");
  // const real & b2 = parameters.dbase.get<real >("b2");

    const int numberOfComponents= parameters.dbase.get<int >("numberOfComponents");
    
    const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
    
    RealArray & timing = parameters.dbase.get<RealArray>("timing");
    bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
    const int & linearizeImplicitMethod = parameters.dbase.get<int >("linearizeImplicitMethod");
    const Parameters::ImplicitMethod & implicitMethod = parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");
    FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");
    FILE *pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");
    
    realCompositeGridFunction & u0 = gf[m0].u;
    realCompositeGridFunction & u1 = gf[m1].u;
    realMappedGridFunction **gridVelocity =  gf[m1].gridVelocity;

    CompositeGrid & cg = gf[m1].cg; // ****************** fix this ****
// *  CompositeGridOperators & operators = *(u0.getOperators());
    CompositeGridOperators & operators = *(u1.getOperators());   // ****


    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

    numberOfImplicitSolves++;

    Index Iv[3], &I1 = Iv[0], &I2=Iv[1], &I3=Iv[2];
    Index Ivg[4], &I1g= Ivg[0], &I2g=Ivg[1], &I3g=Ivg[2];
    Index I1b,I2b,I3b;

    int stencilSize=int( pow(3,cg.numberOfDimensions()) ); 
    Index M(0,stencilSize);  
    int grid,side,axis;

    real alpha=-gamma*pow(double(a0*deltaT),double(2));

  // pM : use this pressure in the matrix 
    realCompositeGridFunction & pM = linearizeImplicitMethod ? pL() : p();

    if( movingGridProblem() || refactorImplicitMatrix || !linearizeImplicitMethod 
            || ((numberOfImplicitSolves-1) % parameters.dbase.get<int >("refactorFrequency") == 0) )
    {
        formAllSpeedPressureEquation( gf[m1],t,deltaT );
    }
    else
    {
        if( debug() & 64 )
            cout << "#####################solveForPressure: matrix was not refactored! ###################\n";
        
        implicitSolver[0].setRefactor(FALSE);
    }
    
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // %%%%%%%%%%%%% Assign the RHS for the pressure equation %%%%%%%%%%%%%%%
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  // ---- RHS = p(t) -  dt*a0*gamma*p* div( u1 ) 
  //      where u1 = u(1) + dt*( u.grad(u) + ... )

    realCompositeGridFunction & f0 = pressureRightHandSide;
    f0=0.;   // ********************************************************** to prevent overflows? ******
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        MappedGrid & c = cg[grid];
        MappedGrid & mg =cg[grid];
        realArray & f = f0[grid];

        getIndex(mg.extendedIndexRange(),I1,I2,I3);

        bool isRectangular=mg.isRectangular();

        real dx[3]={1.,1.,1.};
        if( isRectangular )
            mg.getDeltaX(dx);

        bool useOpt=true;
        if( useOpt )
        {
    
            #ifdef USE_PPP
                realSerialArray u1Local; getLocalArrayWithGhostBoundaries(u1[grid],u1Local);
            #else
                realSerialArray & u1Local=u1[grid];
            #endif

            int includeGhost=1;
            bool ok = ParallelUtility::getLocalArrayBounds(u1[grid],u1Local,I1,I2,I3,includeGhost);
            if( !ok ) continue;

            #ifdef USE_PPP
                realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
            #else
                realSerialArray & fLocal=f;
            #endif
            #ifdef USE_PPP
                realSerialArray pMLocal; getLocalArrayWithGhostBoundaries(pM[grid],pMLocal);
            #else
                realSerialArray & pMLocal=pM[grid];
            #endif
            #ifdef USE_PPP
                intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
            #else
                intSerialArray & maskLocal=mg.mask();
            #endif

            real *pu=u1Local.getDataPointer();
            int *pmask=maskLocal.getDataPointer();

      // p is either the linearized version or the current value
            real *pp= pMLocal.getDataPointer();
            real *pf = fLocal.getDataPointer();
            real *pGam = computeReactions ? gam()[grid].getLocalArray().getDataPointer() : pu;
            real *prsxy = isRectangular ? pu : cg[grid].inverseVertexDerivative().getLocalArray().getDataPointer();

            int gridType = isRectangular? 0 : 1;
            int orderOfAccuracy=parameters.dbase.get<int >("orderOfAccuracy");
            const int gridIsMoving = parameters.gridIsMoving(grid);
            int useWhereMask=true;
            int variableGamma = computeReactions;
            int ipar[]={rc,uc,vc,wc,tc,pc,grid,gridType,orderOfAccuracy,gridIsMoving,useWhereMask,
              		  I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),variableGamma};  //
                      
            real alpha = !computeReactions ? -deltaT*a0*gamma : -deltaT*a0;
            
            real rpar[]={dx[0],dx[1],dx[2],mg.gridSpacing(0),mg.gridSpacing(1),mg.gridSpacing(2),t,dt,alpha,pressureLevel}; 

            int ierr=0;

      // Add the (grad(p)/rho term to the velocity equations:
      //       u(i1,i2,i3,uc..) += alpha*grad(p)/rho
      // where rho is either the current density or the linearized density
            asfAssignPressureRhs(cg[grid].numberOfDimensions(),
                     			   u1Local.getBase(0),u1Local.getBound(0),
                     			   u1Local.getBase(1),u1Local.getBound(1),
                     			   u1Local.getBase(2),u1Local.getBound(2),
                     			   u1Local.getBase(3),u1Local.getBound(3),
                     			   ipar[0],rpar[0],*pf,*pu,*pp,*pGam,*pmask,*prsxy,ierr);

        }
        else
        {
      // old way 
            getIndex(mg.extendedIndexRange(),I1g,I2g,I3g,1);

            px()[grid](I1g,I2g,I3g)=u1[grid](I1g,I2g,I3g,uc);   // ghost values needed (** this is not px **) **** not***
        
            if( twilightZoneFlow && debug() & 32 )
            {
                f(I1g,I2g,I3g)=(u1[grid](I1g,I2g,I3g,uc)-e(c,I1g,I2g,I3g,uc,t)
                          		  -(deltaT*a0)*e.x(c,I1g,I2g,I3g,pc,t)/ e(c,I1g,I2g,I3g,rc,t))/deltaT;
                display( f,"solveForPressure: error in [f1-u(t+dt)-(a0*dt)*p.x/r(t+dt)]/dt (zero?)",debugFile,"%10.2e ");
                if( cg.numberOfDimensions() >1  )
                {
          // save in f so the results are displayed with the correct base/bound
                    f(I1g,I2g,I3g)=(u1[grid](I1g,I2g,I3g,vc)-e(c,I1g,I2g,I3g,vc,t)
                            		    -(deltaT*a0)*e.y(c,I1g,I2g,I3g,pc,t)/e(c,I1g,I2g,I3g,rc,t))/deltaT;
                    display(f,"solveForPressure: error in [f2-v(t+dt)-(a0*dt)*p.y/r(t+dt)]/dt (zero?)",debugFile,"%10.2e ");
                }
                if( cg.numberOfDimensions() >2 )
                {
                    f(I1g,I2g,I3g)=(u1[grid](I1g,I2g,I3g,wc)-e(c,I1g,I2g,I3g,wc,t)
                            		    -(deltaT*a0)*e.z(c,I1g,I2g,I3g,pc,t)/e(c,I1g,I2g,I3g,rc,t))/deltaT;
                    display(f,"solveForPressure: error in [f3-w(t+dt)-(a0*dt)*p.z/r(t+dt)]/dt (zero?)",debugFile,"%10.2e ");
                }
                if( cg.numberOfDimensions() ==2 )
                {
          // **Note** the errors here can appear large for low Mach number
                    f(I1g,I2g,I3g)=(u1[grid](I1g,I2g,I3g,pc)-e(c,I1g,I2g,I3g,pc,t)
                            		    +(-deltaT*gamma*a0)*(pM[grid](I1g,I2g,I3g)+pressureLevel)*
                            		    (e.x(c,I1g,I2g,I3g,uc,t)+e.y(c,I1g,I2g,I3g,vc,t)) )/deltaT;
                    display(f,"solveForPressure: error in [p.rhs - p(t+dt) - gamma*(pM+P0)*(a0*dt)*div(u)]/dt",
                      	    debugFile,"%10.2e ");
                }
                else if( cg.numberOfDimensions() ==3 )
                {
                    f(I1g,I2g,I3g)=(u1[grid](I1g,I2g,I3g,pc)-e(c,I1g,I2g,I3g,pc,t)
                            		    +(-deltaT*gamma*a0)*(pM[grid](I1g,I2g,I3g)+pressureLevel)*
                            		    (e.x(c,I1g,I2g,I3g,uc,t)+e.y(c,I1g,I2g,I3g,vc,t)+e.z(c,I1g,I2g,I3g,wc,t)));
                    display(f,"solveForPressure: error in p.rhs - p(t+dt) - gamma*(pM+P0)*(a0*dt)*div(u)",
                      	    debugFile,"%10.2e ");
                }
                else if ( cg.numberOfDimensions() ==1 )
                {
                    f(I1g,I2g,I3g)=(u1[grid](I1g,I2g,I3g,pc)-e(c,I1g,I2g,I3g,pc,t)
                            		    +(-deltaT*gamma*a0)*(e(c,I1g,I2g,I3g,pc,t)+pressureLevel)*
                            		    (e.x(c,I1g,I2g,I3g,uc,t)));
                    display(f,"solveForPressure: error in p.rhs - p(t+dt) - gamma*p*(a0*dt)*div(u)",
                      	    debugFile,"%10.2e ");
                }
            }
        
      // boundary values not needed:
            const realArray & pTotal = evaluate(pM[grid](I1,I2,I3)+pressureLevel);  // total pressure

            if( !computeReactions )
            {
      	f(I1,I2,I3)=u1[grid](I1,I2,I3,pc)
        	  +(-deltaT*gamma*a0)*pTotal*px()[grid].x(I1,I2,I3)(I1,I2,I3);              // *** /u1[grid](I1,I2,I3,rc);
            }
            else
            { // use variable gamma
      	f(I1,I2,I3)=u1[grid](I1,I2,I3,pc)
        	  +(-deltaT*gam()[grid](I1,I2,I3)*a0)*pTotal*px()[grid].x(I1,I2,I3)(I1,I2,I3);            
            }
        
            if( mg.numberOfDimensions()>1 ) // *** do this all at once in 2D ***
            {
      	px()[grid](I1g,I2g,I3g)=u1[grid](I1g,I2g,I3g,vc);   // this is *NOT* px,  ghost values needed ** why this step****
      	f(I1,I2,I3)+=(-deltaT*gamma*a0)*pTotal*px()[grid].y(I1,I2,I3)(I1,I2,I3);    // *** /u1[grid](I1,I2,I3,rc);
            }
            if( mg.numberOfDimensions()>2 ) // *** do this all at once in 2D ***
            {
      	px()[grid](I1g,I2g,I3g)=u1[grid](I1g,I2g,I3g,wc);   // this is *NOT* px,  ghost values needed
      	f(I1,I2,I3)+=(-deltaT*gamma*a0)*pTotal*px()[grid].z(I1,I2,I3)(I1,I2,I3); //   **** /u1[grid](I1,I2,I3,rc);
            }
        
            if( false )
            { // add divergence damping for low Mach numbers
      	const realArray & xy = mg.vertex();
      	real cdvnu=parameters.dbase.get<real >("cdv")*max(mu,hMin[grid])*4;
      	printf("add damping term to p equation... cdvnu=%e\n",cdvnu);

      	f(I1,I2,I3)+=(-gamma*SQR(deltaT*a0))*pTotal*(  DAI2(cdvnu,I1,I2,I3)*fn[n1][grid](I1,I2,I3,rc) );
            }
        }
        

    // --- debug checks ----
        if( twilightZoneFlow && c.numberOfDimensions()<=2 && debug() & 32 )
        { 
            px()[grid](I1g,I2g,I3g)=u1[grid](I1g,I2g,I3g,rc)-e(c,I1g,I2g,I3g,rc,t);
            display(px()[grid](I1g,I2g,I3g),"Error in rho",debugFile,"%10.2e ");
            px()[grid](I1g,I2g,I3g)=p()[grid](I1g,I2g,I3g)-e(c,I1g,I2g,I3g,pc,t);
            display(px()[grid](I1g,I2g,I3g),"Error in guess for p",debugFile,"%10.2e ");
            if( c.numberOfDimensions()==2 )
            {
                px()[grid](I1g,I2g,I3g)=(f(I1g,I2g,I3g)-e(c,I1g,I2g,I3g,pc,t)
                               			     -alpha*((e(c,I1g,I2g,I3g,pc,t)+pressureLevel)/e(c,I1g,I2g,I3g,rc,t)
                                     				     *(e.xx(c,I1g,I2g,I3g,pc,t)+e.yy(c,I1g,I2g,I3g,pc,t)
                                       				       -(e.x(c,I1g,I2g,I3g,pc,t)*e.x(c,I1g,I2g,I3g,rc,t)
                                       					 +e.y(c,I1g,I2g,I3g,pc,t)*e.y(c,I1g,I2g,I3g,rc,t))/e(c,I1g,I2g,I3g,rc,t) )));        
                display(px()[grid](I1g,I2g,I3g),
                  	    "Error in p.rhs - p(t+dt)-gamma*(a0*dt0)^2*(p/r)*(Delta(p)-(r.x*p.x+r.y*p.y)/r) ",
                  	    debugFile,"%10.2e ");
            }
            else if( c.numberOfDimensions()==1 )
            {
                display( evaluate( (f(I1g,I2g,I3g)-e(c,I1g,I2g,I3g,pc,t)
                          			-alpha*((e(c,I1g,I2g,I3g,pc,t)+pressureLevel)/e(c,I1g,I2g,I3g,rc,t)
                                				*(e.xx(c,I1g,I2g,I3g,pc,t)
                                  				  -(e.x(c,I1g,I2g,I3g,pc,t)*e.x(c,I1g,I2g,I3g,rc,t))/e(c,I1g,I2g,I3g,rc,t) )))),
                   	     "Error in p.rhs - p(t+dt)-gamma*(a0*dt0)^2*(p/r)*(Delta(p)-(r.x*p.x)/r) ",debugFile,"%10.2e ");
            }
            display(f(I1g,I2g,I3g),"rhs for p equation, including ghost points",debugFile);
        }
        if( c.numberOfDimensions()==3 && debug() & 16 )
        {
            display( evaluate( 
                   	     (f(I1g,I2g,I3g)-e(c,I1g,I2g,I3g,pc,t)
                    	      -alpha*((e(c,I1g,I2g,I3g,pc,t)+pressureLevel)/e(c,I1g,I2g,I3g,rc,t)
                          		      *(e.xx(c,I1g,I2g,I3g,pc,t)+e.yy(c,I1g,I2g,I3g,pc,t)+e.zz(c,I1g,I2g,I3g,pc,t)
                          			-(e.x(c,I1g,I2g,I3g,pc,t)*e.x(c,I1g,I2g,I3g,rc,t)
                            			  +e.y(c,I1g,I2g,I3g,pc,t)*e.y(c,I1g,I2g,I3g,rc,t)
                            			  +e.z(c,I1g,I2g,I3g,pc,t)*e.z(c,I1g,I2g,I3g,rc,t))/e(c,I1g,I2g,I3g,rc,t) )))),
                 	   "Error in p.rhs - p(t+dt)-gamma*(a0*dt0)^2*(p/r)*(Delta(p)-(r.x*p.x+r.y*p.y+r.z*p.z)/r) ",
                 	   debugFile,"%10.2e ");
        }

        if( FALSE )
        {
            cout << "***setting rhs for p equation to exact...\n";
            if( mg.numberOfDimensions()==2 )
            {
      	f(I1g,I2g,I3g)= e(c,I1g,I2g,I3g,pc,t)
        	  +alpha*(
          	    (e(c,I1g,I2g,I3g,pc,t)+pressureLevel)/e(c,I1g,I2g,I3g,rc,t)
          	    *(e.xx(c,I1g,I2g,I3g,pc,t)+e.yy(c,I1g,I2g,I3g,pc,t)
            	      -(e.x(c,I1g,I2g,I3g,pc,t)*e.x(c,I1g,I2g,I3g,rc,t)
            		+e.y(c,I1g,I2g,I3g,pc,t)*e.y(c,I1g,I2g,I3g,rc,t))/e(c,I1g,I2g,I3g,rc,t) ));
            }
            else if( mg.numberOfDimensions()==3 )
            {
      	f(I1g,I2g,I3g)= e(c,I1g,I2g,I3g,pc,t)
        	  +alpha*(
          	    (e(c,I1g,I2g,I3g,pc,t)+pressureLevel)/e(c,I1g,I2g,I3g,rc,t)
          	    *(e.xx(c,I1g,I2g,I3g,pc,t)+e.yy(c,I1g,I2g,I3g,pc,t)+e.zz(c,I1g,I2g,I3g,pc,t)
            	      -(e.x(c,I1g,I2g,I3g,pc,t)*e.x(c,I1g,I2g,I3g,rc,t)
            		+e.y(c,I1g,I2g,I3g,pc,t)*e.y(c,I1g,I2g,I3g,rc,t)
            		+e.z(c,I1g,I2g,I3g,pc,t)*e.z(c,I1g,I2g,I3g,rc,t))/e(c,I1g,I2g,I3g,rc,t) ));
            }
      	
        }

/* ----
      if( FALSE )
      {
      cout << "***setting rhs for p equation to new div(f_1)...\n";
      f(I1,I2,I3)=u1[grid](I1,I2,I3,pc)
      +(-deltaT*gamma*a0)*pTotal*( fn[n1][grid](I1,I2,I3,rc) );
      }

      if( FALSE )
      {
   // Here we check the hypothesis that extrapolting u1 is bad --- seems true
      if( c.numberOfDimensions()==2 )
      {
      printf("Setting rhs(p) = exact on boundary\n");
      ForBoundary(side,axis)
      {
      Index I1g,I2g,I3g;
      getBoundaryIndex(c.gridIndexRange(),side,axis,I1g,I2g,I3g,0);  // boundary

      f(I1g,I2g,I3g)=e(c,I1g,I2g,I3g,pc,t)
      -alpha*((e(c,I1g,I2g,I3g,pc,t)+pressureLevel)/e(c,I1g,I2g,I3g,rc,t)
      *(e.xx(c,I1g,I2g,I3g,pc,t)+e.yy(c,I1g,I2g,I3g,pc,t)
      -(e.x(c,I1g,I2g,I3g,pc,t)*e.x(c,I1g,I2g,I3g,rc,t)
      +e.y(c,I1g,I2g,I3g,pc,t)*e.y(c,I1g,I2g,I3g,rc,t))/e(c,I1g,I2g,I3g,rc,t) ) );
      }
      }
      }
      ----- */  

        if( false )
        {
            if( parameters.dbase.get<real >("ad21")>0. && !twilightZoneFlow )
            {
        // smooth the rhs for the pressure to remove large oscillations in the divergence
        // ++ do this before BC's since we do not want the BC values in f when we smooth. ++
                printf("...smooth rhs for pressure\n");
                const int numberOfIterations=2;
        // interpolate(f0);
                f0.interpolate();  // do this -- add timing
                for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
                {
          // getIndex(cg[grid].extendedIndexRange(),I1,I2,I3,-1);  // interior points
          // *** need to smooth f on the boundary too ! ***********
          // smooth interior points and periodic edges
                    MappedGrid & mg = cg[grid];
                    getIndex(mg.extendedIndexRange(),I1,I2,I3); 
                    int axis;
                    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
                    {
                        if( mg.boundaryCondition(Start,axis)>0 )
                  	Iv[axis]=Range(Iv[axis].getBase()+1,Iv[axis].getBound());
                        if( mg.boundaryCondition(End  ,axis)>0 )
                  	Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);
                    }
                    realArray & v = f0[grid];
      // **      const real omega=1./8.;
                    const real omega=1./32;
                    for( int it=0; it<numberOfIterations; it++ )
                    {
                        f0[grid].periodicUpdate(); // ** fix this **
                        v(I1,I2,I3)+=omega*( v(I1+1,I2,I3)+v(I1-1,I2,I3)+v(I1,I2+1,I3)+v(I1,I2-1,I3)-4.*v(I1,I2,I3) );
            // smooth the boundary too
                        for( axis=0; axis<cg.numberOfDimensions(); axis++ )
                        {
                  	for( int side=Start; side<=End; side++ )
                  	{
                    	  if( mg.boundaryCondition(side,axis)>0 )
                    	  {
                      	    getBoundaryIndex(mg.gridIndexRange(),side,axis,I1b,I2b,I3b);
                      	    f0[grid].periodicUpdate(); // ** fix this **
                      	    if( axis==axis1 )
                        	      v(I1b,I2b,I3b)+=.25*( v(I1b,I2b+1,I3b)+v(I1b,I2b-1,I3b)-2.*v(I1b,I2b,I3b) );
                      	    else if( axis==axis2 )
                        	      v(I1b,I2b,I3b)+=0.25*( v(I1b+1,I2b,I3b)+v(I1b-1,I2b,I3b)-2.*v(I1b,I2b,I3b) );
                    	  }
                  	}
                        }
                    }
            /* ----
                  const real omega=1./16;
                  for( int it=0; it<2; it++ )
                  v(I1,I2,I3)+=omega*( 4.*(v(I1+1,I2,I3)+v(I1-1,I2,I3)) -6.*v(I1,I2,I3) -(v(I1+2,I2,I3)+v(I1-2,I2,I3)) );
                  ---- */
                }
            }
        }
        


    // ----------------------------
    // --- Boundary Conditions ----
    // ----------------------------

        realMappedGridFunction & ub = u0[grid];  // $$$$$$$$$$$$$$$$$$$$$$$$$$ use u0, fix $$$$$$$$$
        realArray & u = ub;
        realArray & uf = u1[grid];

        #ifdef USE_PPP
            realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
        #else
            realSerialArray & fLocal=f;
        #endif
        #ifdef USE_PPP
            realSerialArray ufLocal; getLocalArrayWithGhostBoundaries(uf,ufLocal);
        #else
            realSerialArray & ufLocal=uf;
        #endif
        #ifdef USE_PPP
            realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
        #else
            realSerialArray & uLocal=u;
        #endif
        realArray & rL0 = linearizeImplicitMethod ? rL()[grid] : f;
        #ifdef USE_PPP
            realSerialArray rLLocal; getLocalArrayWithGhostBoundaries(rL0,rLLocal);
        #else
            realSerialArray & rLLocal=rL0;
        #endif
        #ifdef USE_PPP
            realSerialArray pLocal; getLocalArrayWithGhostBoundaries(p()[grid],pLocal);
        #else
            realSerialArray & pLocal=p()[grid];
        #endif

        realArray & gv = parameters.gridIsMoving(grid) ? *gridVelocity[grid] : u;
        #ifdef USE_PPP
            realSerialArray gridVelocityLocal; getLocalArrayWithGhostBoundaries(gv,gridVelocityLocal);
        #else
            realSerialArray & gridVelocityLocal=gv;
        #endif

        realArray & x= mg.center();
        #ifdef USE_PPP
            realSerialArray xLocal; 
            if( !isRectangular || twilightZoneFlow ) 
                getLocalArrayWithGhostBoundaries(x,xLocal);
        #else
            const realSerialArray & xLocal = x;
        #endif

        isRectangular=false; // do this for now so the vertexBoundaryNormal is created --- fix this ---

        if( !isRectangular )
            mg.update(MappedGrid::THEvertexBoundaryNormal); // *wdh* 060925
        
        ForBoundary(side,axis)
        {
            getBoundaryIndex(mg.gridIndexRange(),side,axis,I1b,I2b,I3b);
            getGhostIndex(mg.gridIndexRange(),side,axis,I1g,I2g,I3g);

            int includeGhost=1;
            bool ok = ParallelUtility::getLocalArrayBounds(f,fLocal,I1b,I2b,I3b,includeGhost);
            ok = ok && ParallelUtility::getLocalArrayBounds(f,fLocal,I1g,I2g,I3g,includeGhost);
            if( !ok ) continue;

            #ifdef USE_PPP
                const realSerialArray & normal = !isRectangular ? mg.vertexBoundaryNormalArray(side,axis) : fLocal; 
            #else
                const realArray & normal  = !isRectangular ? mg.vertexBoundaryNormal(side,axis) : fLocal;
            #endif

            switch (c.boundaryCondition(side,axis))
            {

            case Parameters::slipWall:  // Neumann
            case Parameters::noSlipWall:  
            case AsfParameters::subSonicInflow:
      	if( false && twilightZoneFlow )
      	{ // for testing set p.n = exact value 
                    realSerialArray p0x(I1b,I2b,I3b);
                    e.gd(p0x,xLocal,mg.numberOfDimensions(),isRectangular,0,1,0,0,I1b,I2b,I3b,pc,t);
        	  if( c.numberOfDimensions()==1 )
        	  {
          	    fLocal(I1g,I2g,I3g)=p0x(I1b,I2b,I3b)*(2*side-1); // outward normal in 1d
        	  }
        	  else
        	  {
          	    realSerialArray p0y(I1b,I2b,I3b);
          	    e.gd(p0y,xLocal,mg.numberOfDimensions(),isRectangular,0,0,1,0,I1b,I2b,I3b,pc,t);
          	    fLocal(I1g,I2g,I3g)=(normal(I1b,I2b,I3b,0)*p0x(I1b,I2b,I3b)+
                         				 normal(I1b,I2b,I3b,1)*p0y(I1b,I2b,I3b));
          	    if( c.numberOfDimensions()==3 )
          	    {
            	      realSerialArray p0z(I1b,I2b,I3b);
            	      e.gd(p0z,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,1,I1b,I2b,I3b,pc,t);
            	      fLocal(I1g,I2g,I3g)+=normal(I1b,I2b,I3b,2)*p0z(I1b,I2b,I3b);
          	    }
        	  
        	  }
      	}
      	else
      	{
                    if( c.boundaryCondition(side,axis)==Parameters::slipWall || 
                            c.boundaryCondition(side,axis)==Parameters::noSlipWall )
        	  {
            // ++++ is this true:
            // BC for pressure comes from  n.{  u^{n+1} + a0*dt*(1/r)*grad(p) = f_1 } // ***************************
            //    On a moving grid: u^{n+1} = gridVelocity
            // or just p.n = - n. ( rho u.t + u.grad(u) - div(tau) )

                        const RealArray & rho0 = linearizeImplicitMethod ? rLLocal(I1b,I2b,I3b) : ufLocal(I1b,I2b,I3b,rc);

                        if( c.numberOfDimensions()==1 )
          	    {
            	      fLocal(I1g,I2g,I3g)=(1./(a0*deltaT))*rho0*((2*side-1)*uLocal(I1b,I2b,I3b,uc));
          	    }
          	    else if( c.numberOfDimensions()==2 )
          	    {
            	      if( twilightZoneFlow )
            	      {
            		realSerialArray u0(I1b,I2b,I3b),v0(I1b,I2b,I3b);
            		e.gd(u0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1b,I2b,I3b,uc,t);
            		e.gd(v0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1b,I2b,I3b,vc,t);

            		fLocal(I1g,I2g,I3g)=(1./(a0*deltaT))*rho0*
              		  (normal(I1b,I2b,I3b,0)*(ufLocal(I1b,I2b,I3b,uc)-u0)+
               		   normal(I1b,I2b,I3b,1)*(ufLocal(I1b,I2b,I3b,vc)-v0) );
                                        
            		if( debug() & 4 )
            		{
                                    RealArray temp(I1b,I2b,I3b);
                                    RealArray p0x(I1b,I2b,I3b),p0y(I1b,I2b,I3b);
                                    e.gd(p0x,xLocal,mg.numberOfDimensions(),isRectangular,0,1,0,0,I1b,I2b,I3b,pc,t);
              		  temp=(1./(a0*deltaT))*rho0*(ufLocal(I1b,I2b,I3b,uc)-u0)-p0x;

              		  display(temp,"asfp:p-BC:Error in ..uf-u(t+dt) - p.x",pDebugFile,"%10.2e ");
                                    e.gd(p0y,xLocal,mg.numberOfDimensions(),isRectangular,0,0,1,0,I1b,I2b,I3b,pc,t);
              		  temp=(1./(a0*deltaT))*rho0*(ufLocal(I1b,I2b,I3b,vc)-v0)-p0y;
              		  display(temp,"asfp:p-BC:Error in ..vf-v(t+dt) - p.y",pDebugFile,"%10.2e ");
            		}
	        // if( false )
		// {
		//   printf("asfp:p-BC: assign exact BC for pressure t=%9.3e\n",t);
		//   fLocal(I1g,I2g,I3g)=(normal(I1b,I2b,I3b,0)*e.x(c,I1b,I2b,I3b, pc,t)+
		// 		  normal(I1b,I2b,I3b,1)*e.y(c,I1b,I2b,I3b, pc,t) );
		// }
            	      }

            	      else 
            	      { 
            		if( !parameters.gridIsMoving(grid) )
            		{
		  // this assumes u=0 on the boundary
              		  fLocal(I1g,I2g,I3g)=(1./(a0*deltaT))*rho0*
                		    (normal(I1b,I2b,I3b,0)*ufLocal(I1b,I2b,I3b,uc)+
                 		     normal(I1b,I2b,I3b,1)*ufLocal(I1b,I2b,I3b,vc));
            		}
            		else
            		{
              		  fLocal(I1g,I2g,I3g)=(1./(a0*deltaT))*rho0*
                		    (normal(I1b,I2b,I3b,0)*(ufLocal(I1b,I2b,I3b,uc)-gridVelocityLocal(I1b,I2b,I3b,0))+
                 		     normal(I1b,I2b,I3b,1)*(ufLocal(I1b,I2b,I3b,vc)-gridVelocityLocal(I1b,I2b,I3b,1)));
            		}
            	      }
/* ------
            	      else
            	      {
            		if( parameters.gridIsMoving(grid) )
            		{
              		  uu(I1b,I2b,I3b,uc)=gridVelocity[grid](I1b,I2b,I3b,0)-advectionCoefficient*u(I1b,I2b,I3b,uc);
              		  uu(I1b,I2b,I3b,vc)=gridVelocity[grid](I1b,I2b,I3b,1)-advectionCoefficient*u(I1b,I2b,I3b,vc);
            		}
            		else
            		{
              		  uu(I1b,I2b,I3b,uc)=(-advectionCoefficient)*u(I1b,I2b,I3b,uc);
              		  uu(I1b,I2b,I3b,vc)=(-advectionCoefficient)*u(I1b,I2b,I3b,vc);
            		}

            		Index N(0,numberOfComponents);
            		ub.getDerivatives(I1b,I2b,I3b,N);                   
            		f(I1g,I2g,I3g)=PN2(I1b,I2b,I3b);
                      	      }
------- */	      
          	    }
          	    else
          	    {
              /// ***** three dimensions ****
            	      if( twilightZoneFlow )
            	      {
            		realSerialArray u0(I1b,I2b,I3b),v0(I1b,I2b,I3b),w0(I1b,I2b,I3b);
            		e.gd(u0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1b,I2b,I3b,uc,t);
            		e.gd(v0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1b,I2b,I3b,vc,t);
            		e.gd(w0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1b,I2b,I3b,wc,t);

            		fLocal(I1g,I2g,I3g)=(1./(a0*deltaT))*rho0*
              		  (normal(I1b,I2b,I3b,0)*(ufLocal(I1b,I2b,I3b,uc)-u0)+
               		   normal(I1b,I2b,I3b,1)*(ufLocal(I1b,I2b,I3b,vc)-v0)+
               		   normal(I1b,I2b,I3b,2)*(ufLocal(I1b,I2b,I3b,wc)-w0) );

		// printf("setting pressure BC exactly..\n");
		// fLocal(I1g,I2g,I3g)=(normal(I1b,I2b,I3b,0)*e.x(c,I1b,I2b,I3b, pc,t)+
		//		normal(I1b,I2b,I3b,1)*e.y(c,I1b,I2b,I3b, pc,t)+  
		//		normal(I1b,I2b,I3b,2)*e.z(c,I1b,I2b,I3b, pc,t) );
            	      }
            	      else
            	      {
            		if( !parameters.gridIsMoving(grid) )
            		{

              		  fLocal(I1g,I2g,I3g)=(1./(a0*deltaT))*rho0*
                		    (normal(I1b,I2b,I3b,0)*ufLocal(I1b,I2b,I3b,uc)+
                 		     normal(I1b,I2b,I3b,1)*ufLocal(I1b,I2b,I3b,vc)+
                 		     normal(I1b,I2b,I3b,2)*ufLocal(I1b,I2b,I3b,wc));
            		}
            		else
            		{
              		  fLocal(I1g,I2g,I3g)=(1./(a0*deltaT))*rho0*
                 		     (normal(I1b,I2b,I3b,0)*(ufLocal(I1b,I2b,I3b,uc)-gridVelocityLocal(I1b,I2b,I3b,0))
                 		     +normal(I1b,I2b,I3b,1)*(ufLocal(I1b,I2b,I3b,vc)-gridVelocityLocal(I1b,I2b,I3b,1))
                 		     +normal(I1b,I2b,I3b,2)*(ufLocal(I1b,I2b,I3b,wc)-gridVelocityLocal(I1b,I2b,I3b,2)));
            		}
            	      }
          	    }

/* ------- 
            // DO NOT do this as u.t is already included!
          	    if( parameters.gridIsMoving(grid) ) // u.t = grid.tt ***************** 
          	    {  // subtract off n.(u.t) = n.(grid.tt)
            	      mappedGridSolver[grid]->gridAccelerationBC(grid, t, c, ub, f0[grid], gridVelocity[grid],
                                           							 c.vertexBoundaryNormal(side,axis),I1b,I2b,I3b,I1g,I2g,I3g  );
          	    }
------ */
        	  }
        	  else
        	  {
                  	    fLocal(I1g,I2g,I3g)=0.;
        	  }
      	}
      	break;
	//kkc 070201      case Parameters::subSonicInflow2:
            case Parameters::dirichletBoundaryCondition:
      	if( twilightZoneFlow )
      	{
        	  realSerialArray p0(I1b,I2b,I3b);
                    e.gd(p0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1b,I2b,I3b,pc,t);
        	  fLocal(I1b,I2b,I3b)=p0; // -pressureLevel;  
      	}
      	else
        	  fLocal(I1b,I2b,I3b)=mixedRHS(pc,side,axis,grid)*uLocal(I1b,I2b,I3b,rc)-pressureLevel;  // **** is this right ??
      	break;
            case AsfParameters::subSonicOutflow:  // mixed alpha*p + beta* p.n
            case AsfParameters::convectiveOutflow:  // mixed alpha*p + beta* p.n
            case AsfParameters::tractionFree:  // mixed alpha*p + beta* p.n
      	if( twilightZoneFlow )
      	{
                    realSerialArray p0x(I1b,I2b,I3b);
                    e.gd(p0x,xLocal,mg.numberOfDimensions(),isRectangular,0,1,0,0,I1b,I2b,I3b,pc,t);
        	  if( c.numberOfDimensions()==1 )
        	  {
          	    fLocal(I1g,I2g,I3g)=p0x(I1b,I2b,I3b)*(2*side-1); // outward normal in 1d
        	  }
        	  else
        	  {
          	    realSerialArray p0y(I1b,I2b,I3b);
          	    e.gd(p0y,xLocal,mg.numberOfDimensions(),isRectangular,0,0,1,0,I1b,I2b,I3b,pc,t);
          	    fLocal(I1g,I2g,I3g)=(normal(I1b,I2b,I3b,0)*p0x(I1b,I2b,I3b)+
                         				 normal(I1b,I2b,I3b,1)*p0y(I1b,I2b,I3b));
          	    if( c.numberOfDimensions()==3 )
          	    {
            	      realSerialArray p0z(I1b,I2b,I3b);
            	      e.gd(p0z,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,1,I1b,I2b,I3b,pc,t);
            	      fLocal(I1g,I2g,I3g)+=normal(I1b,I2b,I3b,2)*p0z(I1b,I2b,I3b);
          	    }
        	  
        	  }

                    realSerialArray & p0 = p0x; // reuse 
                    e.gd(p0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1b,I2b,I3b,pc,t);

          // mixed alpha*p + beta* p.n
        	  fLocal(I1g,I2g,I3g)=mixedCoeff(pc,side,axis,grid)*p0+
                            	                      mixedNormalCoeff(pc,side,axis,grid)*fLocal(I1g,I2g,I3g);
      	}
      	else
      	{ // set alpha*p + beta*p.n = gamma
        	  if( debug() & 2 )
                        printF("++++ solveForAllSpeedPressure: rhs for subSonicOutflow=%e \n",mixedRHS(pc,side,axis,grid));
        	  fLocal(I1g,I2g,I3g)=mixedRHS(pc,side,axis,grid);
      	}
      	break;
            default:
      	if( c.boundaryCondition()(side,axis)>0 )
      	{
        	  cout << "solveForAllSpeedPressure: Unknown BC = " << c.boundaryCondition()(side,axis) << endl;
        	  printf("cg[grid=%i].boundaryCondition()(side=%i,axis=%i)=%i (%s)\n",grid,side,axis,
             		 cg[grid].boundaryCondition()(side,axis), 
             		 parameters.bcNames[cg[grid].boundaryCondition()(side,axis)]);
                	  throw "error";
      	}
            }
        }
        if( FALSE && twilightZoneFlow )
        { // This is not needed ****
      // add forcing to rhs for twilight-zone flow
            addForcingToPressureEquation( grid,c,f0[grid],*gridVelocity[grid],t ); 
        }
    }
    
    
/* ----
    if( (debug() & 32) && twilightZoneFlow )
    {
        realMappedGridFunction result;
        result=multiply(p[0],coeff[0]);   // ****
    // coeff[0].display("Here is coeff");
        p[0].display("Here is p");
        f[0].display("f: here is the rhs for p equation");
        result.display("******* Here is Lp ******* ");
        (result-f[0]).display("******* Lp-f *******");
    }
----- */

    if( debug() & 32 )
    {
        for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            display(f0[grid],"f: here is the rhs for p equation",debugFile,"%10.2e ");
    }
    






    if( implicitSolver[0].getCompatibilityConstraint() )
    { // set the rhs for the compatibility equation for the pressure

    // First get the indices of the (unused) point on the grid where the compat. eqn is put
        int ne,i1e,i2e,i3e,gride;
        implicitSolver[0].equationToIndex( implicitSolver[0].extraEquationNumber(0),ne,i1e,i2e,i3e,gride);
        f0[gride](i1e,i2e,i3e)=0.;
        if( twilightZoneFlow ) 
        {
            for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            {
	//     ...add in the constraint equation     
	//        (This is the equation that sets the mean value of p)
      	MappedGrid & c = cg[grid];
      	getIndex(c.dimension(),I1,I2,I3);
      	f0[gride](i1e,i2e,i3e)+=sum(implicitSolver[0].rightNullVector[grid](I1,I2,I3)*e(c,I1,I2,I3,pc,t));
            }
        }
    }

    timing(parameters.dbase.get<int>("timeForAssignPressureRHS"))+=getCPU()-time;


    if( false )
    {
        assign(p(),0.);  // fix this 
        for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            f0[grid].updateGhostBoundaries();

        assign(f0,0.1);
        
    }


  // ***************************************************************
  // ************** solve the system for p *************************
  // ***************************************************************
    time=getCPU();
    implicitSolver[0].solve(p(),f0);
    timing(parameters.dbase.get<int>("timeForPressureSolve"))+=getCPU()-time;

    if( debug() & 16 )
    {
        for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
        {
            MappedGrid & mg = cg[grid];
            getIndex(mg.extendedIndexRange(),I1,I2,I3);

/* ----
            real error;
            where( cg[grid].mask()(I1,I2,I3)!=0 )
                error = max(fabs(p()[grid](I1,I2,I3)+pressureLevel-P(t)));
            fprintf(debugFile,"solveForPressure: t=%f, grid=%i, maximum error in p = %e\n",t,grid,error);
---- */

            getIndex(mg.extendedIndexRange(),I1g,I2g,I3g,1);
            if( twilightZoneFlow )
            {
      	fprintf(debugFile,"solveForPressure: t=%f, error in p\n",t);

                bool isRectangular=false;
    
                realArray & x= mg.center();
                #ifdef USE_PPP
                    realSerialArray xLocal; 
                    if( !isRectangular || twilightZoneFlow ) 
                        getLocalArrayWithGhostBoundaries(x,xLocal);
                #else
                    const realSerialArray & xLocal = x;
                #endif

                #ifdef USE_PPP
                    realSerialArray pLocal; getLocalArrayWithGhostBoundaries(p()[grid],pLocal);
                #else
                    realSerialArray & pLocal=p()[grid];
                #endif
                #ifdef USE_PPP
                    realSerialArray pxLocal; getLocalArrayWithGhostBoundaries(px()[grid],pxLocal);
                #else
                    realSerialArray & pxLocal=px()[grid];
                #endif

                int includeGhost=1;
                bool ok = ParallelUtility::getLocalArrayBounds(p()[grid],pLocal,I1g,I2g,I3g,includeGhost);
                if( !ok ) continue;

      	realSerialArray p0(I1g,I2g,I3g);
      	e.gd(p0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1g,I2g,I3g,pc,t);
                pxLocal(I1g,I2g,I3g)=fabs(pLocal(I1g,I2g,I3g)-p0(I1g,I2g,I3g));

        // px()[grid](I1g,I2g,I3g)=fabs(p()[grid](I1g,I2g,I3g)-e(c,I1g,I2g,I3g,pc,t));
	// display(px()[grid](I1g,I2g,I3g),"Error in p including ghost line",debugFile);

                getIndex(mg.extendedIndexRange(),I1g,I2g,I3g,1);
                display(px()[grid],"Error in p including ghost line",debugFile,"%8.2e ",Ivg);
      	
            }
            else
            {
      	display(p()[grid](I1g,I2g,I3g),"after pressure solve, p including ghost line",debugFile);
            }
      //cout << "pressureLevel= " << pressureLevel << endl;
      //p()[grid](I1g,I2g,I3g).display("solveForPressure:Here is p after implicit solve");
        }
    }

  // tm(4)+=getCPU()-time;
}





void Cgasf::
solveForTimeIndependentVariables( GridFunction & cgf, bool updateSolutionDependentEquations  )
// ===================================================================================
// This routine is used to compute the initial pressure, given the initial velocity and density
// ===================================================================================
{
    if( parameters.dbase.get<int >("explicitMethod") ) return;

    printf(" @@@@@@@@@@@@@ solveForTimeIndependentVariablesASF @@@@@@@@@@@@@@ \n");

    const int & numberOfComponents = parameters.dbase.get<int>("numberOfComponents");
    const int & rc = parameters.dbase.get<int >("rc");
    const int & uc = parameters.dbase.get<int >("uc");
    const int & vc = parameters.dbase.get<int >("vc");
    const int & wc = parameters.dbase.get<int >("wc");
    const int & pc = parameters.dbase.get<int >("pc");
    const real & mu = parameters.dbase.get<real >("mu");
    const real & pressureLevel = parameters.dbase.get<real >("pressureLevel");
    const real & advectionCoefficient = parameters.dbase.get<real >("advectionCoefficient");

    FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");
    
    RealArray & timing = parameters.dbase.get<RealArray>("timing");
    bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");

    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
    RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
    
    bool formSteadyEquation=TRUE;
    real t=0., deltaT=1.;
    formAllSpeedPressureEquation(cgf,t,deltaT,formSteadyEquation);
    
    real cpu1=getCPU();

    realCompositeGridFunction & u0 = cgf.u;
  // realCompositeGridFunction & gridVelocity = cgf.gridVelocity;
    realMappedGridFunction **gridVelocity =  cgf.gridVelocity;
    CompositeGrid & cg = cgf.cg;
    realCompositeGridFunction & f0 = pressureRightHandSide;

    Index I1,I2,I3;
    Range N(uc,uc+cg.numberOfDimensions()-1);
    Range NN(0,numberOfComponents-1);
    
    bool useOpt=true;
    
    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        MappedGrid & c = cg[grid];
        MappedGrid & mg = cg[grid];
        realArray & u = u0[grid];
        realArray & f = f0[grid];
        
        MappedGridOperators & op = *(u0[grid].getOperators());
        bool isRectangular = mg.isRectangular();
        
        if( debug() & 64 )
            display(u0[grid],"u",debugFile,"%5.2f ");

        getIndex(mg.extendedIndexRange(),I1,I2,I3);

        if( useOpt )
        {
            #ifdef USE_PPP
                realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
            #else
                realSerialArray & uLocal=u;
            #endif

            int includeGhost=1;
            bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
            if( !ok ) continue;

            #ifdef USE_PPP
                realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
            #else
                realSerialArray & fLocal=f;
            #endif
            #ifdef USE_PPP
                intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
            #else
                intSerialArray & maskLocal=mg.mask();
            #endif

            if( mg.numberOfDimensions()==1 )
            {
      	RealArray ux(I1,I2,I3,N);
      	op.derivative(MappedGridOperators::xDerivative ,uLocal,ux ,I1,I2,I3,N); 

      	fLocal(I1,I2,I3)=-SQR(ux(I1,I2,I3,uc));
            }
            else if( mg.numberOfDimensions()==2 )  // ****** fix these, add second derivative terms ***
            {
      	RealArray ux(I1,I2,I3,N), uy(I1,I2,I3,N);
      	op.derivative(MappedGridOperators::xDerivative ,uLocal,ux ,I1,I2,I3,N); 
      	op.derivative(MappedGridOperators::yDerivative ,uLocal,uy ,I1,I2,I3,N); 

      	fLocal(I1,I2,I3)=-( SQR(ux(I1,I2,I3,uc))+2.*uy(I1,I2,I3,uc)*ux(I1,I2,I3,vc)+SQR(uy(I1,I2,I3,vc)) );
            }
            else if( mg.numberOfDimensions()==3 )
            {
      	RealArray ux(I1,I2,I3,N), uy(I1,I2,I3,N), uz(I1,I2,I3,N);
      	op.derivative(MappedGridOperators::xDerivative ,uLocal,ux ,I1,I2,I3,N); 
      	op.derivative(MappedGridOperators::yDerivative ,uLocal,uy ,I1,I2,I3,N); 
      	op.derivative(MappedGridOperators::zDerivative ,uLocal,uz ,I1,I2,I3,N); 

      	fLocal(I1,I2,I3)=-( SQR(ux(I1,I2,I3,uc)) + 2.*uy(I1,I2,I3,uc)*ux(I1,I2,I3,vc) + 
                                                        2.*uz(I1,I2,I3,uc)*ux(I1,I2,I3,wc)
                      			    +SQR(uy(I1,I2,I3,vc))+ 2.*uz(vc)*uy(I1,I2,I3,wc)+SQR(uz(I1,I2,I3,wc)) );
            
            }
        
            isRectangular=false;
    
            if( !isRectangular )
                mg.update(MappedGrid::THEvertexBoundaryNormal); // *wdh* 060901
      
            realArray & x= mg.center();
            #ifdef USE_PPP
                realSerialArray xLocal; 
                if( !isRectangular || twilightZoneFlow ) 
                    getLocalArrayWithGhostBoundaries(x,xLocal);
            #else
                const realSerialArray & xLocal = x;
            #endif

            realArray & gv = parameters.gridIsMoving(grid) ? *gridVelocity[grid] : u;
            #ifdef USE_PPP
                realSerialArray gridVelocityLocal; getLocalArrayWithGhostBoundaries(gv,gridVelocityLocal);
            #else
                realSerialArray & gridVelocityLocal=gv;
            #endif
            #ifdef USE_PPP
                realSerialArray pLocal; getLocalArrayWithGhostBoundaries(p()[grid],pLocal);
            #else
                realSerialArray & pLocal=p()[grid];
            #endif

      // --- Boundary Conditions ----
            Range V(uc,uc+mg.numberOfDimensions()-1);
            int side,axis;
            ForBoundary(side,axis)
            {
      	Index I1b,I2b,I3b;
      	Index I1g,I2g,I3g;
      	getBoundaryIndex(mg.gridIndexRange(),side,axis,I1b,I2b,I3b);
      	getGhostIndex(mg.gridIndexRange(),side,axis,I1g,I2g,I3g);

                int includeGhost=1;
                bool ok = ParallelUtility::getLocalArrayBounds(f,fLocal,I1b,I2b,I3b,includeGhost);
                ok = ok && ParallelUtility::getLocalArrayBounds(f,fLocal,I1g,I2g,I3g,includeGhost);
                if( !ok ) continue;

                #ifdef USE_PPP
                    const realSerialArray & normal = !isRectangular ? mg.vertexBoundaryNormalArray(side,axis) : fLocal; 
                #else
                    const realArray & normal  = !isRectangular ? mg.vertexBoundaryNormal(side,axis) : fLocal;
                #endif

      	RealArray uu(I1b,I2b,I3b,N);
            
      	switch (mg.boundaryCondition()(side,axis))
      	{
      	case Parameters::slipWall:  // Neumann
      	case Parameters::noSlipWall:  
      	case AsfParameters::subSonicInflow:
        	  if( twilightZoneFlow )
        	  {
          	    realSerialArray p0x(I1b,I2b,I3b);
          	    e.gd(p0x,xLocal,mg.numberOfDimensions(),isRectangular,0,1,0,0,I1b,I2b,I3b,pc,t);

          	    if( mg.numberOfDimensions()==1 )
            	      fLocal(I1g,I2g,I3g)=p0x*(2*side-1); // outward normal in 1d
          	    else
          	    {
            	      realSerialArray p0y(I1b,I2b,I3b);
            	      e.gd(p0y,xLocal,mg.numberOfDimensions(),isRectangular,0,0,1,0,I1b,I2b,I3b,pc,t);
            	      fLocal(I1g,I2g,I3g)=normal(I1b,I2b,I3b,0)*p0x+normal(I1b,I2b,I3b,1)*p0y;
            	      if( mg.numberOfDimensions()==3 )
            	      {
            		realSerialArray & p0z = p0x; // reuse
                  	        e.gd(p0z,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,1,I1b,I2b,I3b,pc,t);
            		fLocal(I1g,I2g,I3g)+=normal(I1b,I2b,I3b,2)*p0z;
            	      }
          	    
          	    }
        	  }
        	  else
        	  {
          	    RealArray acRho(I1b,I2b,I3b);  acRho =advectionCoefficient*uLocal(I1b,I2b,I3b,rc);
          	    if( mg.numberOfDimensions()==1 )
          	    {
            	      if( parameters.gridIsMoving(grid) )
            		uu(I1b,I2b,I3b,uc)=gridVelocityLocal(I1b,I2b,I3b,0)-advectionCoefficient*uLocal(I1b,I2b,I3b,uc);
            	      else
            		uu(I1b,I2b,I3b,uc)=(-advectionCoefficient)*uLocal(I1b,I2b,I3b,uc);

	      // *wdh* 060901u0[grid].getDerivatives(I1b,I2b,I3b,NN);
            	      RealArray ux(I1b,I2b,I3b,NN), uxx(I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::xDerivative ,uLocal,ux ,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::xxDerivative,uLocal,uxx,I1b,I2b,I3b,NN);

            	      fLocal(I1g,I2g,I3g)=PN1(I1b,I2b,I3b);

          	    }
          	    else if( mg.numberOfDimensions()==2 )
          	    {
          	    
            	      if( parameters.gridIsMoving(grid) )
            	      {
            		uu(I1b,I2b,I3b,uc)=gridVelocityLocal(I1b,I2b,I3b,0)-advectionCoefficient*uLocal(I1b,I2b,I3b,uc);
            		uu(I1b,I2b,I3b,vc)=gridVelocityLocal(I1b,I2b,I3b,1)-advectionCoefficient*uLocal(I1b,I2b,I3b,vc);
            	      }
            	      else
            	      {
            		uu(I1b,I2b,I3b,uc)=(-advectionCoefficient)*uLocal(I1b,I2b,I3b,uc);
            		uu(I1b,I2b,I3b,vc)=(-advectionCoefficient)*uLocal(I1b,I2b,I3b,vc);
            	      }
      	
	      // *wdh* 060901    u0[grid].getDerivatives(I1b,I2b,I3b,NN);

            	      RealArray ux(I1b,I2b,I3b,NN), uy(I1b,I2b,I3b,NN);
            	      RealArray uxx(I1b,I2b,I3b,NN), uxy(I1b,I2b,I3b,NN), uyy(I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::xDerivative ,uLocal,ux ,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::yDerivative ,uLocal,uy ,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::xxDerivative,uLocal,uxx,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::xyDerivative,uLocal,uxy,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::yyDerivative,uLocal,uyy,I1b,I2b,I3b,NN);


//             fprintf(stdout,"++side=%i, axis=%i, bc=%i \n",side,axis,mg.boundaryCondition()(side,axis));
//             display(uu(I1b,I2b,I3b,V),"uu",stdout,"%4.1f ");
//             display(ux(I1b,I2b,I3b,V),"ux",stdout,"%4.1f ");
//             display(uy(I1b,I2b,I3b,V),"ux",stdout,"%4.1f ");
//             display(uxx(I1b,I2b,I3b,V),"uxx",stdout,"%4.1f ");
//             display(uyy(I1b,I2b,I3b,V),"uyy",stdout,"%4.1f ");

            	      fLocal(I1g,I2g,I3g)=PN2(I1b,I2b,I3b);
	      // display(f(I1g,I2g,I3g),"PN2",debugFile,"%4.1f ");
          	    }
          	    else
          	    {
              // --- 3D -----------
            	      if( parameters.gridIsMoving(grid) )
            	      {
            		uu(I1b,I2b,I3b,uc)=gridVelocityLocal(I1b,I2b,I3b,0)-advectionCoefficient*uLocal(I1b,I2b,I3b,uc);
            		uu(I1b,I2b,I3b,vc)=gridVelocityLocal(I1b,I2b,I3b,1)-advectionCoefficient*uLocal(I1b,I2b,I3b,vc);
            		uu(I1b,I2b,I3b,wc)=gridVelocityLocal(I1b,I2b,I3b,2)-advectionCoefficient*uLocal(I1b,I2b,I3b,wc);
            	      }
            	      else
            	      {
            		uu(I1b,I2b,I3b,uc)=(-advectionCoefficient)*uLocal(I1b,I2b,I3b,uc);
            		uu(I1b,I2b,I3b,vc)=(-advectionCoefficient)*uLocal(I1b,I2b,I3b,vc);
            		uu(I1b,I2b,I3b,wc)=(-advectionCoefficient)*uLocal(I1b,I2b,I3b,wc);
            	      }
      	
	      // *wdh* 060901    u0[grid].getDerivatives(I1b,I2b,I3b,NN);

            	      RealArray ux(I1b,I2b,I3b,NN), uy(I1b,I2b,I3b,NN), uz(I1b,I2b,I3b,NN);
            	      RealArray uxx(I1b,I2b,I3b,NN), uxy(I1b,I2b,I3b,NN), uyy(I1b,I2b,I3b,NN);
            	      RealArray uxz(I1b,I2b,I3b,NN), uyz(I1b,I2b,I3b,NN), uzz(I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::xDerivative ,uLocal,ux ,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::yDerivative ,uLocal,uy ,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::zDerivative ,uLocal,uz ,I1b,I2b,I3b,NN);

            	      op.derivative(MappedGridOperators::xxDerivative,uLocal,uxx,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::xyDerivative,uLocal,uxy,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::yyDerivative,uLocal,uyy,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::xzDerivative,uLocal,uxz,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::yzDerivative,uLocal,uyz,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::zzDerivative,uLocal,uzz,I1b,I2b,I3b,NN);


            	      fLocal(I1g,I2g,I3g)=PN3(I1b,I2b,I3b);


          	    }
          	    if( parameters.gridIsMoving(grid) ) // u.t = grid.tt ***************** 
          	    {  // subtract off n.(u.t) = n.(grid.tt)
            	      gridAccelerationBC(grid, t, cgf, f0, side,axis  );
          	    }
        	  }
        	  break;
	  //kkc 070201	case Parameters::subSonicInflow2:
      	case Parameters::dirichletBoundaryCondition:
        	  if( twilightZoneFlow )
        	  {
          	    realSerialArray p0(I1b,I2b,I3b);
          	    e.gd(p0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1b,I2b,I3b,pc,t);

          	    fLocal(I1b,I2b,I3b)=p0; // -pressureLevel;  
        	  }
        	  else
          	    fLocal(I1b,I2b,I3b)=mixedRHS(pc,side,axis,grid)*uLocal(I1b,I2b,I3b,rc); // -pressureLevel; 
        	  break;
      	case AsfParameters::subSonicOutflow:  // mixed alpha*p + beta* p.n
      	case AsfParameters::convectiveOutflow:  // mixed alpha*p + beta* p.n
      	case AsfParameters::tractionFree:  // mixed alpha*p + beta* p.n
        	  if( twilightZoneFlow )
        	  {
          	    realSerialArray p0x(I1b,I2b,I3b);
          	    e.gd(p0x,xLocal,mg.numberOfDimensions(),isRectangular,0,1,0,0,I1b,I2b,I3b,pc,t);

          	    if( mg.numberOfDimensions()==1 )
            	      fLocal(I1g,I2g,I3g)=p0x*(2*side-1); // outward normal in 1d
          	    else
          	    {
            	      realSerialArray p0y(I1b,I2b,I3b);
            	      e.gd(p0y,xLocal,mg.numberOfDimensions(),isRectangular,0,0,1,0,I1b,I2b,I3b,pc,t);
            	      fLocal(I1g,I2g,I3g)=normal(I1b,I2b,I3b,0)*p0x+normal(I1b,I2b,I3b,1)*p0y;
            	      if( mg.numberOfDimensions()==3 )
            	      {
            		realSerialArray & p0z = p0x; // reuse
                  	        e.gd(p0z,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,1,I1b,I2b,I3b,pc,t);
            		fLocal(I1g,I2g,I3g)+=normal(I1b,I2b,I3b,2)*p0z;
            	      }
          	    
          	    }
          	    realSerialArray p0(I1b,I2b,I3b);
          	    e.gd(p0,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1b,I2b,I3b,pc,t);
          	    fLocal(I1g,I2g,I3g)=(mixedCoeff(pc,side,axis,grid)*p0+
                         				 mixedNormalCoeff(pc,side,axis,grid)*fLocal(I1g,I2g,I3g));
        	  }
        	  else
        	  {  // set alpha*p + beta*p.n = gamma
          	    fLocal(I1g,I2g,I3g)=mixedRHS(pc,side,axis,grid);
        	  }
        	  break;
      	default:
        	  if( mg.boundaryCondition()(side,axis)>0 )
        	  {
          	    cout << "solveForTimeIndependentVariablesASF: Unknown BC = " << mg.boundaryCondition()(side,axis) << endl;
          	    printf("cg[grid=%i].boundaryCondition()(side=%i,axis=%i)=%i (%s)\n",grid,side,axis,
               		   cg[grid].boundaryCondition()(side,axis), 
               		   parameters.bcNames[cg[grid].boundaryCondition()(side,axis)]);
          	    throw "error";
        	  }
      	}
            }


        }
        else
        {  // --- old way  ----
#ifdef USE_PPP
            Overture::abort("error");
#else
            if( c.numberOfDimensions()==1 )
            {
      	realArray ux(I1,I2,I3,N);
      	op.derivative(MappedGridOperators::xDerivative ,u,ux ,I1,I2,I3,N); 

      	f(I1,I2,I3)=-SQR(ux(I1,I2,I3,uc));
            }
            else if( c.numberOfDimensions()==2 )  // ****** fix these, add second derivative terms ***
            {
      	realArray ux(I1,I2,I3,N), uy(I1,I2,I3,N);
      	op.derivative(MappedGridOperators::xDerivative ,u,ux ,I1,I2,I3,N); 
      	op.derivative(MappedGridOperators::yDerivative ,u,uy ,I1,I2,I3,N); 

      	f(I1,I2,I3)=-( SQR(ux(I1,I2,I3,uc))+2.*uy(I1,I2,I3,uc)*ux(I1,I2,I3,vc)+SQR(uy(I1,I2,I3,vc)) );
            }
            else if( c.numberOfDimensions()==3 )
            {
      	realArray ux(I1,I2,I3,N), uy(I1,I2,I3,N), uz(I1,I2,I3,N);
      	op.derivative(MappedGridOperators::xDerivative ,u,ux ,I1,I2,I3,N); 
      	op.derivative(MappedGridOperators::yDerivative ,u,uy ,I1,I2,I3,N); 
      	op.derivative(MappedGridOperators::zDerivative ,u,uz ,I1,I2,I3,N); 

      	f(I1,I2,I3)=-( SQR(ux(I1,I2,I3,uc)) + 2.*uy(I1,I2,I3,uc)*ux(I1,I2,I3,vc) + 2.*uz(I1,I2,I3,uc)*ux(I1,I2,I3,wc)
                   		       +SQR(uy(I1,I2,I3,vc))+ 2.*uz(vc)*uy(I1,I2,I3,wc)+SQR(uz(I1,I2,I3,wc)) );
            
            }
        
            c.update(MappedGrid::THEvertexBoundaryNormal); // *wdh* 060901
      
      // --- Boundary Conditions ----
            Range V(uc,uc+c.numberOfDimensions()-1);
            int side,axis;
            ForBoundary(side,axis)
            {
      	Index I1b,I2b,I3b;
      	Index I1g,I2g,I3g;
      	getBoundaryIndex(c.gridIndexRange(),side,axis,I1b,I2b,I3b);
      	getGhostIndex(c.gridIndexRange(),side,axis,I1g,I2g,I3g);
      	realArray & normal = c.vertexBoundaryNormal(side,axis);

      	realArray uu(I1b,I2b,I3b,N);
            
      	switch (c.boundaryCondition()(side,axis))
      	{
      	case Parameters::slipWall:  // Neumann
      	case Parameters::noSlipWall:  
      	case AsfParameters::subSonicInflow:
        	  if( twilightZoneFlow )
        	  {
          	    if( c.numberOfDimensions()==1 )
            	      f(I1g,I2g,I3g)=e.x(c,I1b,I2b,I3b, pc,t)*(2*side-1); // outward normal in 1d
          	    else
          	    {
            	      f(I1g,I2g,I3g)=normal(I1b,I2b,I3b,0)*e.x(c,I1b,I2b,I3b, pc,t) 
            		+normal(I1b,I2b,I3b,1)*e.y(c,I1b,I2b,I3b, pc,t);
            	      if( c.numberOfDimensions()==3 )
            		f(I1g,I2g,I3g)+=normal(I1b,I2b,I3b,2)*e.z(c,I1b,I2b,I3b, pc,t);
          	    }
        	  }
        	  else
        	  {
          	    RealArray acRho(I1b,I2b,I3b);  acRho =advectionCoefficient*u(I1b,I2b,I3b,rc);
          	    if( c.numberOfDimensions()==1 )
          	    {
            	      if( parameters.gridIsMoving(grid) )
            		uu(I1b,I2b,I3b,uc)=(*gridVelocity[grid])(I1b,I2b,I3b,0)-advectionCoefficient*u(I1b,I2b,I3b,uc);
            	      else
            		uu(I1b,I2b,I3b,uc)=(-advectionCoefficient)*u(I1b,I2b,I3b,uc);

	      // *wdh* 060901u0[grid].getDerivatives(I1b,I2b,I3b,NN);
            	      realArray ux(I1b,I2b,I3b,NN), uxx(I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::xDerivative ,u0[grid],ux ,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::xxDerivative,u0[grid],uxx,I1b,I2b,I3b,NN);

            	      f(I1g,I2g,I3g)=PN1(I1b,I2b,I3b);

          	    }
          	    else if( c.numberOfDimensions()==2 )
          	    {
          	    
            	      if( parameters.gridIsMoving(grid) )
            	      {
            		uu(I1b,I2b,I3b,uc)=(*gridVelocity[grid])(I1b,I2b,I3b,0)-advectionCoefficient*u(I1b,I2b,I3b,uc);
            		uu(I1b,I2b,I3b,vc)=(*gridVelocity[grid])(I1b,I2b,I3b,1)-advectionCoefficient*u(I1b,I2b,I3b,vc);
            	      }
            	      else
            	      {
            		uu(I1b,I2b,I3b,uc)=(-advectionCoefficient)*u(I1b,I2b,I3b,uc);
            		uu(I1b,I2b,I3b,vc)=(-advectionCoefficient)*u(I1b,I2b,I3b,vc);
            	      }
      	
	      // *wdh* 060901    u0[grid].getDerivatives(I1b,I2b,I3b,NN);

            	      realArray ux(I1b,I2b,I3b,NN), uy(I1b,I2b,I3b,NN);
            	      realArray uxx(I1b,I2b,I3b,NN), uxy(I1b,I2b,I3b,NN), uyy(I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::xDerivative ,u0[grid],ux ,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::yDerivative ,u0[grid],uy ,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::xxDerivative,u0[grid],uxx,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::xyDerivative,u0[grid],uxy,I1b,I2b,I3b,NN);
            	      op.derivative(MappedGridOperators::yyDerivative,u0[grid],uyy,I1b,I2b,I3b,NN);


//             fprintf(stdout,"++side=%i, axis=%i, bc=%i \n",side,axis,c.boundaryCondition()(side,axis));
//             display(uu(I1b,I2b,I3b,V),"uu",stdout,"%4.1f ");
//             display(ux(I1b,I2b,I3b,V),"ux",stdout,"%4.1f ");
//             display(uy(I1b,I2b,I3b,V),"ux",stdout,"%4.1f ");
//             display(uxx(I1b,I2b,I3b,V),"uxx",stdout,"%4.1f ");
//             display(uyy(I1b,I2b,I3b,V),"uyy",stdout,"%4.1f ");

            	      f(I1g,I2g,I3g)=PN2(I1b,I2b,I3b);
	      // display(f(I1g,I2g,I3g),"PN2",debugFile,"%4.1f ");
          	    }
          	    else
          	    {
            	      throw "error";
          	    }
          	    if( parameters.gridIsMoving(grid) ) // u.t = grid.tt ***************** 
          	    {  // subtract off n.(u.t) = n.(grid.tt)
//               #ifdef USE_PPP
//                realSerialArray & normal = c.vertexBoundaryNormalArray(side,axis);
//               #else
//                realSerialArray & normal = c.vertexBoundaryNormal(side,axis);
//               #endif               
// 	      gridAccelerationBC(grid, t, c, u0[grid], 
// 				 f0[grid], *gridVelocity[grid],
// 				 normal,
// 				 I1b,I2b,I3b, I1g,I2g,I3g, side,axis  );

                            gridAccelerationBC(grid, t, cgf, f0, side,axis  );
          	    }
        	  }
        	  break;
	  //kkc 070201	case Parameters::subSonicInflow2:
      	case Parameters::dirichletBoundaryCondition:
        	  if( twilightZoneFlow )
          	    f(I1b,I2b,I3b)=e(c,I1b,I2b,I3b, pc,t); // -pressureLevel;  
        	  else
          	    f(I1b,I2b,I3b)=mixedRHS(pc,side,axis,grid)*u(I1b,I2b,I3b,rc)-pressureLevel;  // **** is this right ??
        	  break;
      	case AsfParameters::subSonicOutflow:  // mixed alpha*p + beta* p.n
      	case AsfParameters::convectiveOutflow:  // mixed alpha*p + beta* p.n
      	case AsfParameters::tractionFree:  // mixed alpha*p + beta* p.n
        	  if( twilightZoneFlow )
        	  {
          	    if( c.numberOfDimensions()==1 )
            	      f(I1g,I2g,I3g)=e.x(c,I1b,I2b,I3b, pc,t)*(2*side-1); // outward normal in 1d
          	    else
          	    {
            	      f(I1g,I2g,I3g)=normal(I1b,I2b,I3b,0)*e.x(c,I1b,I2b,I3b, pc,t) 
            		+normal(I1b,I2b,I3b,1)*e.y(c,I1b,I2b,I3b, pc,t);
            	      if( c.numberOfDimensions()==3 )
            		f(I1g,I2g,I3g)+=normal(I1b,I2b,I3b,2)*e.z(c,I1b,I2b,I3b, pc,t);
          	    }
          	    f(I1g,I2g,I3g)=mixedCoeff(pc,side,axis,grid)*e(c,I1b,I2b,I3b, pc,t)+
            	      mixedNormalCoeff(pc,side,axis,grid)*f(I1g,I2g,I3g);
        	  }
        	  else
        	  {  // set alpha*p + beta*p.n = gamma
          	    f(I1g,I2g,I3g)=mixedRHS(pc,side,axis,grid);
        	  }
        	  break;
      	default:
        	  if( c.boundaryCondition()(side,axis)>0 )
        	  {
          	    cout << "solveForTimeIndependentVariablesASF: Unknown BC = " << c.boundaryCondition()(side,axis) << endl;
          	    printf("cg[grid=%i].boundaryCondition()(side=%i,axis=%i)=%i (%s)\n",grid,side,axis,
               		   cg[grid].boundaryCondition()(side,axis), 
               		   parameters.bcNames[cg[grid].boundaryCondition()(side,axis)]);
          	    throw "error";
        	  }
      	}
            }
#endif
        } // end if !useOpt 
    } 
    
    if( implicitSolver[0].getCompatibilityConstraint() )
    { // set the rhs for the compatibility equation for the pressure

    // First get the indices of the (unused) point on the grid where the compat. eqn is put
        int ne,i1e,i2e,i3e,gride;
        implicitSolver[0].equationToIndex( implicitSolver[0].extraEquationNumber(0),ne,i1e,i2e,i3e,gride);
        f0[gride](i1e,i2e,i3e)=0.;
    // **** does this need to be fixed for TZ flow?
    }

    if( debug() & 2 )
    {
        for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            display(f0[grid],"f: here is the rhs for p equation",debugFile,"%8.1e ");
    }
    timing(parameters.dbase.get<int>("timeForAssignPressureRHS"))+=getCPU()-cpu1;

    cpu1=getCPU();
  // solve the system for p
    implicitSolver[0].solve(p(),f0);

    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        MappedGrid & c = cg[grid];
        getIndex(c.dimension(),I1,I2,I3);

        #ifdef USE_PPP
            realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u0[grid],uLocal);
        #else
            realSerialArray & uLocal=u0[grid];
        #endif

        int includeGhost=1;
        bool ok = ParallelUtility::getLocalArrayBounds(u0[grid],uLocal,I1,I2,I3,includeGhost);
        if( !ok ) continue;

        #ifdef USE_PPP
            realSerialArray pLocal; getLocalArrayWithGhostBoundaries(p()[grid],pLocal);
        #else
            realSerialArray & pLocal=p()[grid];
        #endif


        uLocal(I1,I2,I3,pc)=pLocal(I1,I2,I3);
    }
    
    timing(parameters.dbase.get<int>("timeForPressureSolve"))+=getCPU()-cpu1;
    
}


#undef PN1
#undef PXB2
#undef PYB2
#undef PN2
#undef DELTAU
#undef P3B
#undef PN3

#define U(n)      e(c,I1,I2,I3,n,t)
#define UT(n)   e.t(c,I1,I2,I3,n,t)
#define UX(n)   e.x(c,I1,I2,I3,n,t)
#define UY(n)   e.y(c,I1,I2,I3,n,t)
#define UZ(n)   e.z(c,I1,I2,I3,n,t)
#define UXX(n) e.xx(c,I1,I2,I3,n,t)
#define UYY(n) e.yy(c,I1,I2,I3,n,t)
#define UZZ(n) e.zz(c,I1,I2,I3,n,t)
#define UXY(n) e.xy(c,I1,I2,I3,n,t)
#define UXZ(n) e.xz(c,I1,I2,I3,n,t)
#define UYZ(n) e.yz(c,I1,I2,I3,n,t)

#define P0(c,I1,I2,I3,t)  e(c,I1,I2,I3,pc,t)
#define P02N(c,I1,I2,I3,t) ( normal(I1,I2,I3,0)*e.x(c,I1,I2,I3,pc,t) +normal(I1,I2,I3,1)*e.y(c,I1,I2,I3,pc,t) )

#define P03N(c,I1,I2,I3,t) ( normal(I1,I2,I3,0)*e.x(c,I1,I2,I3,pc,t) +normal(I1,I2,I3,1)*e.y(c,I1,I2,I3,pc,t) +normal(I1,I2,I3,2)*e.z(c,I1,I2,I3,pc,t) )

//     normal derivative of p (outward normal)
#define PN1(I1,I2,I3)  ( (2*side-1)*(rho*UT(uc) +acRho*uu(I1,I2,I3,uc)*UX(uc) - mu*UXX(uc) ) )


#define PXB2(I1,I2,I3) ( rho*UT(uc) + acRho*( uu(I1,I2,I3,uc)*UX(uc) + uu(I1,I2,I3,vc)*UY(uc)) + e.x(c,I1,I2,I3,pc,t) - mu*((4./3.)*UXX(uc)+UYY(uc)+(1./3.)*UXY(vc)) )

#define PYB2(I1,I2,I3) ( rho*UT(vc) + acRho*( uu(I1,I2,I3,uc)*UX(vc) + uu(I1,I2,I3,vc)*UY(vc)) + e.y(c,I1,I2,I3,pc,t) -mu*(UXX(vc)+(4./3.)*UYY(vc)+(1./3.)*UXY(uc)) )

//     normal derivative of p (outward normal)
#define PN2(I1,I2,I3)  ( normal(I1,I2,I3,0)*PXB2(I1,I2,I3)  +normal(I1,I2,I3,1)*PYB2(I1,I2,I3) )
//  ...momentum eqn's in 3d without grad p term
#define DELTAU(I1,I2,I3,dir) (ctxx[dir]*UXX(dir)+ctyy[dir]*UYY(dir)+ctzz[dir]*UZZ(dir))

#define P3B(I1,I2,I3,dir) ( rho*UT(dir) - mu*DELTAU(I1,I2,I3,dir) 			    +acRho*( uu(I1,I2,I3,uc)*UX(dir) 				     + uu(I1,I2,I3,vc)*UY(dir) 				     + uu(I1,I2,I3,wc)*UZ(dir)) )

//    ...normal derivative of p in 3d (outward normal)
#define PN3(I1,I2,I3) ( normal(I1,I2,I3,0)*(P3B(I1,I2,I3,uc)+e.x(c,I1,I2,I3,pc,t))  +normal(I1,I2,I3,1)*(P3B(I1,I2,I3,vc)+e.y(c,I1,I2,I3,pc,t))  +normal(I1,I2,I3,2)*(P3B(I1,I2,I3,wc)+e.z(c,I1,I2,I3,pc,t)) )

void Cgasf::
addForcingToPressureEquation( const int & grid,
                        			      MappedGrid & c, 
                        			      realMappedGridFunction & f,  
                        			      realMappedGridFunction & gridVelocity, 
                        			      const real & t )
//======================================================================
//   Add the forcing to the pressure equation for
//        Twilightzone flow for all speed flow
//
//======================================================================
{
#ifdef USE_PPP
        Overture::abort("Error- fix this Bill");
#else

    bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
    if( !twilightZoneFlow )
        return;

//realArray & uu = get(WorkSpace::uu);
    const int numberOfComponents= parameters.dbase.get<int >("numberOfComponents");
    const int & rc = parameters.dbase.get<int >("rc");
    const int & uc = parameters.dbase.get<int >("uc");
    const int & vc = parameters.dbase.get<int >("vc");
    const int & wc = parameters.dbase.get<int >("wc");
    const int & pc = parameters.dbase.get<int >("pc");
  // const int & tc = parameters.dbase.get<int >("tc");

    const real & mu  = parameters.dbase.get<real >("mu");
  // const real & cdv = parameters.dbase.get<real >("cdv");
    const real & advectionCoefficient = parameters.dbase.get<real >("advectionCoefficient");
    const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
    
    FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");

    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

    if( debug() & 32 )
        cout << " ***Entering addForcingToPressureEquation *** \n";


    Index I1,I2,I3,I1g,I2g,I3g;

    
  //     ----apply the boundary condition----
    int side,axis;
    ForBoundary(side,axis)
    {
        if( c.boundaryCondition()(side,axis) > 0 )
        {
            getGhostIndex( c.extendedIndexRange(),side,axis,I1g,I2g,I3g,1);  // first ghost line
            getGhostIndex( c.extendedIndexRange(),side,axis,I1 ,I2 ,I3 ,0);     // boundary line
            const RealArray & normal = c.vertexBoundaryNormal(side,axis);

            const realArray & rho = U(rc);
            const realArray & acRho = evaluate( advectionCoefficient*rho );
            const realArray & u0=e(c,I1,I2,I3,uc,t);
            const realArray & v0=e(c,I1,I2,I3,vc,t);
            
            realArray uu(I1,I2,I3,parameters.dbase.get<Range >("Ru"));
            
            if( parameters.gridIsMoving(grid) )
            {
      	uu(I1,I2,I3,uc)=e(c,I1,I2,I3,uc,t)-gridVelocity(I1,I2,I3,0);
      	uu(I1,I2,I3,vc)=e(c,I1,I2,I3,vc,t)-gridVelocity(I1,I2,I3,1);
      	if( c.numberOfDimensions()==3 )
        	  uu(I1,I2,I3,wc)=e(c,I1,I2,I3,wc,t)-gridVelocity(I1,I2,I3,2);
            }
            else
            {
      	uu(I1,I2,I3,uc)=e(c,I1,I2,I3,uc,t);
      	uu(I1,I2,I3,vc)=e(c,I1,I2,I3,vc,t);
      	if( c.numberOfDimensions()==3 )
        	  uu(I1,I2,I3,wc)=e(c,I1,I2,I3,wc,t);
            }

            
            switch (c.boundaryCondition(side,axis))
            {
            case AsfParameters::subSonicOutflow:
            case AsfParameters::convectiveOutflow:
            case AsfParameters::tractionFree:  // mixed alpha*p + beta* p.n
      	if( c.numberOfDimensions()==2 )
            	  f(I1g,I2g,I3g)+=mixedCoeff(pc,side,axis,grid)*P0(c,I1,I2,I3,t)+
                                                    mixedNormalCoeff(pc,side,axis,grid)*P02N(c,I1,I2,I3,t);
                else
            	  f(I1g,I2g,I3g)+=mixedCoeff(pc,side,axis,grid)*P0(c,I1,I2,I3,t)+
                                                    mixedNormalCoeff(pc,side,axis,grid)*P03N(c,I1,I2,I3,t);
                break;
	//kkc 070201      case Parameters::inflowWithPressureAndTangentialVelocityGiven:
	//kkc 070201 f(I1,I2,I3)+=P0(c,I1,I2,I3,t);
        //kkc 070201 break;
            default:
      	if( c.numberOfDimensions()==2 )
      	{
        	  f(I1g,I2g,I3g)+=PN2(I1,I2,I3);  // give normal component of momentum equations
	  // f(I1g,I2g,I3g)=P02N(c,I1,I2,I3,t);  // give normal component pressure
	  // ***  f(I1,I2,I3)=P0(c,I1,I2,I3,t);  // Dirichlet for testing
      	}
      	else
      	{
                    throw "error";
        	  
	  // f(I1g,I2g,I3g)+=PN3(I1,I2,I3);  // give normal component of momentum equations
	  // f(I1g,I2g,I3g)=P03N(c,I1,I2,I3,t);  // give normal component of momentum equations
      	}
            }
        }
    }
#endif
}			
