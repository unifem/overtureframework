#include "Cgad.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "AdamsPCData.h"
#include "AdvanceOptions.h"
#include "OGPulseFunction.h"
#include "OGTrigFunction.h"
#include "Oges.h"
#include "PlotStuff.h"
#include "Ogshow.h"

static real S = 6.92*pow(10.,-6); // *note* also defined in AdParameters.C
static real G = 0.0;
#define PI 3.14159

// static real cDecouple=1.;
static real cDecouple=0.;

// static Ogshow *pshow=NULL;
// if( pshow==NULL ){ pshow = new Ogshow("my name"); }
  

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


#define FOR_3(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


// ==========================================================================================
// Evaluate the thin film equations
// ==========================================================================================
int evaluateThinFilmFunction( realCompositeGridFunction & rhs,
                              GridFunction *gf, realCompositeGridFunction & newtCur,
                              int mNew, int mCur, real t0, real dt0, Parameters & parameters )
{
  OGFunction & exact = *(parameters.dbase.get<OGFunction* >("exactSolution"));

  Index I1,I2,I3;
  Index Ib1,Ib2,Ib3;
  Index Ig1,Ig2,Ig3;
  int side,axis;
  

  rhs = 0.;
        
  // ------------ Recalculate F ---------------------
  for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
  {

    realMappedGridFunction & uCur = gf[mCur].u[grid];
    realMappedGridFunction & gridVelocity = gf[mNew].getGridVelocity(grid);
    MappedGrid & mg = gf[mNew].cg[grid];

    const int numberOfDimensions = mg.numberOfDimensions();
         
    getIndex(mg.indexRange(),I1,I2,I3);
            
    OV_GET_SERIAL_ARRAY_CONST(real,newtCur[grid],uLocal);       
    Range N=2;
    RealArray ux(I1,I2,I3,N), uy(I1,I2,I3,N), uLap(I1,I2,I3,N); 
    MappedGridOperators & op = *(newtCur[grid].getOperators());  // opertaors at *new* time
    op.derivative(MappedGridOperators::xDerivative,uLocal,ux  ,I1,I2,I3,N);
    op.derivative(MappedGridOperators::yDerivative,uLocal,uy  ,I1,I2,I3,N);
    op.derivative(MappedGridOperators::laplacianOperator,uLocal,uLap  ,I1,I2,I3,N);

    OV_GET_SERIAL_ARRAY_CONST(real,mg.center(),xLocal);
    RealArray ue(I1,I2,I3,N), uet(I1,I2,I3,N), uex(I1,I2,I3,N), uey(I1,I2,I3,N); 
    const bool isRectangular = false; // do this for now
    const real tNew = t0+dt0;
    exact.gd( ue  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,N,tNew);
    exact.gd( uet ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,N,tNew);
    exact.gd( uex ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,N,tNew);
    exact.gd( uey ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,N,tNew);

    //We first build the right hand side where F(uNew)for backward Euler
    if(parameters.gridIsMoving(grid)){
                
      //where(mg.mask()(I1,I2,I3)>0){
                
      if( true )
      { // *new* 

	rhs[grid](I1,I2,I3,0) = newtCur[grid](I1,I2,I3,0)-uCur(I1,I2,I3,0) - dt0*( newtCur[grid].x()(I1,I2,I3,0)*gridVelocity(I1,I2,I3,0) + newtCur[grid].y()(I1,I2,I3,0)*gridVelocity(I1,I2,I3,1) 
              + cDecouple*( (1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid].x()(I1,I2,I3,0)*newtCur[grid].x()(I1,I2,I3,1) +(1./12)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid].laplacian()(I1,I2,I3,1)+(1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid].y()(I1,I2,I3,0)*(newtCur[grid].y()(I1,I2,I3,1) +G) 
		)
           );
	
	rhs[grid](I1,I2,I3,0) +=
	  - dt0*(exact.t(mg,I1,I2,I3,0,t0+dt0) - 
                 cDecouple*( (1./4)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact.x(mg,I1,I2,I3,0,t0+dt0)*exact.x(mg,I1,I2,I3,1,t0+dt0) - (1./12)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact.laplacian(mg,I1,I2,I3,1,t0+dt0) - (1./4)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact.y(mg,I1,I2,I3,0,t0+dt0)*(exact.y(mg,I1,I2,I3,1,t0+dt0)+G) 
		   )
                );

	rhs[grid](I1,I2,I3,1) =newtCur[grid](I1,I2,I3,1)+S*newtCur[grid].laplacian()(I1,I2,I3,0);

	rhs[grid](I1,I2,I3,1) -= ue(I1,I2,I3,1) + S*exact.laplacian(mg,I1,I2,I3,0,t0+dt0);
	 
        
      }
      else
      { // *old*
	rhs[grid](I1,I2,I3,0) = newtCur[grid](I1,I2,I3,0)-uCur(I1,I2,I3,0) - dt0*( newtCur[grid].x()(I1,I2,I3,0)*gridVelocity(I1,I2,I3,0) + newtCur[grid].y()(I1,I2,I3,0)*gridVelocity(I1,I2,I3,1) +(1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid].x()(I1,I2,I3,0)*newtCur[grid].x()(I1,I2,I3,1) +(1./12)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid].laplacian()(I1,I2,I3,1)+(1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid].y()(I1,I2,I3,0)*(newtCur[grid].y()(I1,I2,I3,1) +G) ) - dt0*(exact.t(mg,I1,I2,I3,0,t0+dt0)- exact.x(mg,I1,I2,I3,0,t0+dt0)*gridVelocity(I1,I2,I3,0)- exact.y(mg,I1,I2,I3,0,t0+dt0)*gridVelocity(I1,I2,I3,1) - (1./4)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact.x(mg,I1,I2,I3,0,t0+dt0)*exact.x(mg,I1,I2,I3,1,t0+dt0) - (1./12)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact.laplacian(mg,I1,I2,I3,1,t0+dt0) - (1./4)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact.y(mg,I1,I2,I3,0,t0+dt0)*(exact.y(mg,I1,I2,I3,1,t0+dt0)+G) );
	rhs[grid](I1,I2,I3,1) =newtCur[grid](I1,I2,I3,1)+S*newtCur[grid].laplacian()(I1,I2,I3,0);
      }
      
      //}//end of where
                
      ForBoundary(side,axis){
	if(mg.boundaryCondition()(side,axis)>0){
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,0);
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
	  rhs[grid](Ib1,Ib2,Ib3,0) = newtCur[grid](Ib1,Ib2,Ib3,0) - exact(mg,Ib1,Ib2,Ib3,0,t0+dt0);
	  rhs[grid](Ib1,Ib2,Ib3,1) = newtCur[grid](Ib1,Ib2,Ib3,1) - exact(mg,Ib1,Ib2,Ib3,1,t0+dt0);
	  // rhs[grid](Ig1,Ig2,Ig3,0) = newtCur[grid](Ig1,Ig2,Ig3,0) - exact(mg,Ig1,Ig2,Ig3,0,t0+dt0);
	  // rhs[grid](Ig1,Ig2,Ig3,1) = newtCur[grid](Ig1,Ig2,Ig3,1) - exact(mg,Ig1,Ig2,Ig3,1,t0+dt0);
	}
      }//end of ForBoundary
                
    }else
    {
      //where(mg.mask()(I1,I2,I3)>0){
                
      if( true )
      {
	// *new* way
	rhs[grid](I1,I2,I3,0) = newtCur[grid](I1,I2,I3,0)-uCur(I1,I2,I3,0) -  cDecouple*dt0*( (1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*ux(I1,I2,I3,0)*ux(I1,I2,I3,1) +(1./12)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*uLap(I1,I2,I3,1)+(1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*uy(I1,I2,I3,0)*(uy(I1,I2,I3,1)+G) );

	// if( parameters.dbase.get<bool >("twilightZoneFlow") )
	  rhs[grid](I1,I2,I3,0) += - dt0*( uet(I1,I2,I3,0) - 
					   cDecouple*( (1./4)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*uex(I1,I2,I3,0)*uex(I1,I2,I3,1) - (1./12)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact.laplacian(mg,I1,I2,I3,1,t0+dt0) - (1./4)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact.y(mg,I1,I2,I3,0,t0+dt0)*(exact.y(mg,I1,I2,I3,1,t0+dt0) + G ) 
					     )
                                          );

	rhs[grid](I1,I2,I3,1) =newtCur[grid](I1,I2,I3,1)+S*uLap(I1,I2,I3,0);

        rhs[grid](I1,I2,I3,1) -= ue(I1,I2,I3,1) + S*exact.laplacian(mg,I1,I2,I3,0,t0+dt0);
      }
      else
      {
        // *old way*	
      
	rhs[grid](I1,I2,I3,0) = newtCur[grid](I1,I2,I3,0)-uCur(I1,I2,I3,0) - dt0*( (1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid].x()(I1,I2,I3,0)*newtCur[grid].x()(I1,I2,I3,1) +(1./12)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid].laplacian()(I1,I2,I3,1)+(1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid].y()(I1,I2,I3,0)*(newtCur[grid].y()(I1,I2,I3,1)+G) ) - dt0*(exact.t(mg,I1,I2,I3,0,t0+dt0) - (1./4)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact.x(mg,I1,I2,I3,0,t0+dt0)*exact.x(mg,I1,I2,I3,1,t0+dt0) - (1./12)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact.laplacian(mg,I1,I2,I3,1,t0+dt0) - (1./4)*exact(mg,I1,I2,I3,0,t0+dt0)*exact(mg,I1,I2,I3,0,t0+dt0)*exact.y(mg,I1,I2,I3,0,t0+dt0)*(exact.y(mg,I1,I2,I3,1,t0+dt0) + G ) );
	rhs[grid](I1,I2,I3,1) =newtCur[grid](I1,I2,I3,1)+S*newtCur[grid].laplacian()(I1,I2,I3,0);
      }
      
      //}//end of where
                
      ForBoundary(side,axis){
	if(mg.boundaryCondition()(side,axis)>0){
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,0);
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
	  rhs[grid](Ib1,Ib2,Ib3,0) = newtCur[grid](Ib1,Ib2,Ib3,0) - exact(mg,Ib1,Ib2,Ib3,0,t0+dt0);
	  rhs[grid](Ib1,Ib2,Ib3,1) = newtCur[grid](Ib1,Ib2,Ib3,1) - exact(mg,Ib1,Ib2,Ib3,1,t0+dt0);
	  // rhs[grid](Ig1,Ig2,Ig3,0) = newtCur[grid](Ig1,Ig2,Ig3,0) - exact(mg,Ig1,Ig2,Ig3,0,t0+dt0);
	  // rhs[grid](Ig1,Ig2,Ig3,1) = newtCur[grid](Ig1,Ig2,Ig3,1) - exact(mg,Ig1,Ig2,Ig3,1,t0+dt0);
          
	}
      }//end of ForBoundary
                
    }


  }//end of for grid
        
  // rhs.interpolate();  //I didn't build an interpolant but this works?!
  // rhs.finishBoundaryConditions(bcParams);

  return 0;
  

}


// ===================================================================================================================
/// \brief Advance the THIN FILM EQUATIONS one time step for the BDF scheme.
///
///   **THIS VERSION IS A PLACEHOLDER FOR THE REAL EQUATIONS ***
/// 
/// \details This routine is called by the implicitTimeStep routine which is in turn called by
/// the BDF takeTimeStep routine which handles details of moving and adaptive grids.
/// 
/// \param t0 (input) : current time
/// \param dt0 (input) : current time step
///
// ===================================================================================================================
int Cgad::
thinFilmSolver(  real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions )
{
  
    if( t0<5*dt0 )
        printF("--AD-- thinFilmSolver WARNING : FINISH ME, t=%9.3e\n",t0);

    
    /* It appears as though the parameter orderOfBDF tells cgad how many time levels the solver needs. */
    const int & orderOfBDF = parameters.dbase.get<int>("orderOfBDF");

    
    /* It appears as though the structure adamsData tells thinFilmSolver.C where the current time step is located in the array. */
    assert( parameters.dbase.get<int >("orderOfPredictorCorrector")==2 );  // for now we just have 2nd-order in time

    assert( parameters.dbase.get<DataBase >("modelData").has_key("AdamsPCData") );
    AdamsPCData & adamsData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData");
  
    real & dtb=adamsData.dtb;
    int &mab0 =adamsData.mab0, &mab1=adamsData.mab1, &mab2=adamsData.mab2;
    int &nab0 =adamsData.nab0, &nab1=adamsData.nab1, &nab2=adamsData.nab2, &nab3=adamsData.nab3;
    int &ndt0=adamsData.ndt0;
    real *dtp = adamsData.dtp;
    

    /* This piece finds the index of the solution at older times */
    const int numberOfGridFunctions =  orderOfBDF+1; // movingGridProblem() ? 3 : 2;
    int & mCur = mab0;
    const int mNew   = (mCur + 1 + numberOfGridFunctions ) % numberOfGridFunctions; // holds u(t+dt)
    const int mOld   = (mCur - 1 + numberOfGridFunctions ) % numberOfGridFunctions; // holds u(t-dt)
    const int mOlder = (mCur - 2 + numberOfGridFunctions ) % numberOfGridFunctions; // holds u(t-2*dt)
    const int mMinus3= (mCur - 3 + numberOfGridFunctions*2) % numberOfGridFunctions; // holds u(t-3*dt)

    
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3,D1,D2,D3;
    Range all;
    int side,axis;
    Range e0(0,0), e1(1,1);  // e0 = first equation, e1 = second equation
    Range c0(0,0), c1(1,1);  // c0 = first component, c1 = second component
    
    gf[mNew].cg.update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEvertexBoundaryNormal );
    
    //  -----------------------------------------------
    //  --- Build exact solution ----------------------
    //  ----------Can I pass the exact solution?-------
    //  -----------------------------------------------
    

    DataBase & db = parameters.dbase.get<DataBase >("modelData");
    
    const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")-parameters.dbase.get<int >("numberOfExtraVariables");

    assert( numberOfComponents==2 );
    
/* ---
    DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("CgadUserDefinedInitialConditionData");
    RealArray & pulseParameters = db.get<RealArray>("pulseParameters");
    
    // Pulse parameters:
    real xPulse=pulseParameters(0);
    real yPulse=pulseParameters(1);
    real zPulse=pulseParameters(2);
    real amp   =pulseParameters(3); // amplitude
    real alpha =pulseParameters(4); // 50.; // 200.;
    
    // Trig parameters:  how do I pass this information?
    RealArray fx,fy,fz,ft,a,c;
    fx.redim(numberOfComponents); fx = 2.;  //would be nice to pass this information
    fy.redim(numberOfComponents); fy = 2.;  //would be nice to pass this information
    fz.redim(numberOfComponents); fz = 0.;  //would be nice to pass this information
    ft.redim(numberOfComponents); ft = 2.;  //would be nice to pass this information
    a.redim(numberOfComponents); a = 1.; a(1) = 2*S*pow(2.*PI,2.);  //would be nice to pass this information
    c.redim(numberOfComponents); c = 2.; c(1) = 0.; //would be nice to pass this information
    

    OGFunction *exactPointer=NULL;
    //OGFunction *exactPointer = new OGPulseFunction(2,1,amp,alpha,xPulse,yPulse,zPulse,1.,0.,0.,1);


    exactPointer = new OGTrigFunction(fx,fy,fz,ft);
    ((OGTrigFunction*)exactPointer)->setAmplitudes( a );
    ((OGTrigFunction*)exactPointer)->setConstants( c );
    
    OGFunction & exact = *exactPointer;
  -- */
    
    OGFunction & exact = *(parameters.dbase.get<OGFunction* >("exactSolution"));

    //  ----------------------------------------------------------------------------------
    //  --- Build a differential operator on the new grid --------------------------------
    //  ----- Do I have to do this?  Since I can take derivatives of uNew and uCur, ------
    //  ----- is there a differential operator create elsewhere?  How do I access it? ----
    //  ----------------------------------------------------------------------------------
    
// >> start new: 

    if( !db.has_key("coeff") )
    {
       db.put<realCompositeGridFunction>("coeff");
    }

    realCompositeGridFunction & coeff = db.get<realCompositeGridFunction>("coeff");

    int orderOfAccuracy=2;  //how do I pass this information?
    const int width = orderOfAccuracy+1;
    int stencilSize=int(pow(width,gf[mNew].cg.numberOfDimensions())+1);  // add 1 for interpolation equations
    int stencilDimension = stencilSize*SQR(numberOfComponents);

    coeff.updateToMatchGrid(gf[mNew].cg,stencilDimension,all,all,all);

    const int numberOfGhostLines=(orderOfAccuracy)/2;
    int includeGhost=numberOfGhostLines;
    coeff.setIsACoefficientMatrix(TRUE,stencilSize,numberOfGhostLines,numberOfComponents);
    coeff=0.;
// << end new 

/* --- old 
    int orderOfAccuracy=2;  //how do I pass this information?
    const int width = orderOfAccuracy+1;
    int stencilSize=int(pow(width,gf[mNew].cg.numberOfDimensions())+1);  // add 1 for interpolation equations
    int stencilDimension = stencilSize*SQR(numberOfComponents);
    realCompositeGridFunction coeff(gf[mNew].cg,stencilDimension,all,all,all);
    const int numberOfGhostLines=(orderOfAccuracy)/2;
    int includeGhost=numberOfGhostLines;
    coeff.setIsACoefficientMatrix(TRUE,stencilSize,numberOfGhostLines,numberOfComponents);
    coeff=0.;
 --- */
    
    
    // *new: 
    CompositeGridOperators & op =  *gf[mNew].u.getOperators();
    // *test*
    op.updateToMatchGrid(gf[mNew].cg);

/* ----
    // old: 
    CompositeGridOperators op(gf[mNew].cg);
  ---- */

    op.setStencilSize(stencilSize);
    op.setNumberOfComponentsForCoefficients(numberOfComponents);
    op.setOrderOfAccuracy(orderOfAccuracy);
    // -----

    coeff.setOperators(op);
    
    BoundaryConditionParameters bcParams;
    // bcParams.ghostLineToAssign=2;
    
    // make some shorter names for readability
    BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
    neumann               = BCTypes::neumann,
    extrapolate           = BCTypes::extrapolate,
    allBoundaries         = BCTypes::allBoundaries;
    
    //  ----------------------------------------------------------------------------------
    //  --- Build a solver on the new grid --------------------------------
    //  ----- Do I have to do this?
    //  ---------------------------------------------------------------------------------
    
    Oges solver( gf[mNew].cg );
    int solverType=OgesParameters::yale;
    solver.set(OgesParameters::THEsolverType,solverType);
    
    
    // ----------------------------------------------------------------------------------
    // --- Create composite grid functions need to complete one step of integration -----
    // --------In your ADI code, it looks like rhs is passed?----------------------------
    // ----------------------------------------------------------------------------------
    
    realCompositeGridFunction rhs(gf[mNew].cg,all,all,all,2), newtCur(gf[mNew].cg,all,all,all,2), newtDiff(gf[mNew].cg,all,all,all,2), ones(gf[mNew].cg),CompgridVelocityx(gf[mNew].cg),CompgridVelocityy(gf[mNew].cg), error(gf[mNew].cg,all,all,all,2);
    
    rhs = 0.;
    newtCur = 0.;
    newtDiff = 0.;
    CompgridVelocityx = 0.;
    CompgridVelocityy = 0.;
    ones = 1.;
    error = 0.;
    
    rhs.setOperators(op);
    newtCur.setOperators(op);
    newtDiff.setOperators(op);
    
    rhs.setName("thickness",0);
    rhs.setName("pressure",1);
    
    error.setName("thickness error",0);
    error.setName("pressure error",1);
    
    real normRHS, time0, time, maxr0, maxr1, maxvalh, maxvalp;
    normRHS=0.;
    time0 = 0.;
    time = 0.;
    maxr0 = 0.;
    maxr1 = 0.;
    
    int i1,i2,i3;
    int buffer = 50;
    
    //  create show file to help debug
    Ogshow show("TestMovingEye.show");
    show.setFlushFrequency(1);
    
    //  -----------------------------------
    //  --- Assign the right-hand-side  ---
    //  -----------------------------------

  
    printF(" gf[mCur].t = %9.3e\n",gf[mCur].t);


    for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
    {
        //realMappedGridFunction & uNew = gf[mNew].u[grid];
        realMappedGridFunction & uCur = gf[mCur].u[grid];
        realMappedGridFunction & gridVelocity = gf[mNew].getGridVelocity(grid);
        MappedGrid & mg = gf[mNew].cg[grid];
        MappedGrid & mgCur = gf[mCur].cg[grid];
        realMappedGridFunction & cgridx =CompgridVelocityx[grid];
        realMappedGridFunction & cgridy =CompgridVelocityy[grid];
        
        //******************Remove this when I fix initial conditions
        getIndex(gf[mNew].cg[grid].dimension(),D1,D2,D3);

        getIndex(gf[mNew].cg[grid].gridIndexRange(),I1,I2,I3,-1);

        // *** this can maybe be removed:
        if(t0==0)
        {

   

	  // ::display(fabs(uCur(I1,I2,I3,0)-exact(mgCur,I1,I2,I3,0,t0)),"uCur-exact","%9.2e ");
	  

/* ---
	  where( mgCur.mask()(I1,I2,I3)>0 )
	    {
	      real diffh = max(fabs(uCur(I1,I2,I3,0)-exact(mgCur,I1,I2,I3,0,t0)));
	      real diffp = max(fabs(uCur(I1,I2,I3,1)-exact(mgCur,I1,I2,I3,1,t0)));
              printF(" grid=%i diffh=%9.3e, diffp=%9.3e\n",grid,diffh,diffp);
	    }
   --- */
	  if( true )
	  {
	    //    uCur(D1,D2,D3,0)=exact(mgCur,D1,D2,D3,0,t0); // use current grid 
            //    uCur(D1,D2,D3,1)=exact(mgCur,D1,D2,D3,1,t0);
	  }
	  else
	  {
            uCur(D1,D2,D3,0)=exact(mg,D1,D2,D3,0,t0);
            uCur(D1,D2,D3,1)=exact(mg,D1,D2,D3,1,t0);
	  }
	  
	    
        }
        
        if(parameters.gridIsMoving(grid)){
            cgridx(D1,D2,D3) = gridVelocity(D1,D2,D3,0);
            cgridy(D1,D2,D3) = gridVelocity(D1,D2,D3,1);
        }
        else{
            cgridx(D1,D2,D3) = 0.;
            cgridy(D1,D2,D3) = 0.;
        }
        //*********************************************************
        
        
        getIndex(mg.indexRange(),I1,I2,I3);
      
        // *** COULD USE newtCur = 2*uCur - uOld   (if dt=constant)
        // newtCur[grid] = uCur;  //uNew holds the current Newton iteration approximation of u(t+dt).  To start the iteration, uNew=uCur.

        RealArray & newtCurArray = newtCur[grid];
        newtCurArray=uCur;
        

    }//end of for grid


    // NOTE: We assume that each iterate satisfies the interpolation equations
    //  since in the Newton correction we do not include a non-zero RHS to the interpolation equations
    // Further NOTE: By setting newtCur = uCur -> interpolation equations are not satisfied on a moving
    //   grid.
    newtCur.interpolate();
    

/* ---
    // --- these two steps are probably not needed
    rhs.interpolate();  //I didn't build an interpolant but this works?!
    rhs.finishBoundaryConditions(bcParams);
    --- */    
    
    evaluateThinFilmFunction(  rhs, gf, newtCur,  mNew, mCur, t0, dt0,  parameters );

    // -- use mask here ( or use gridFunctionNorm function)

    for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
    {
        MappedGrid & mg = gf[mNew].cg[grid];
        getIndex(mg.indexRange(),I1,I2,I3);
        realArray r0 = abs(rhs[grid](I1,I2,I3,0));
        realArray r1 = abs(rhs[grid](I1,I2,I3,1));
        normRHS = max(max(r1),max(max(r0),normRHS));
    }

    printF("Norm of RHS is %e \n",normRHS);
    

    printF("Size of dt is %e\n",dt0);
    
    //  -----------------------------------
    //  --- Newton's Method  ---
    //  -----------------------------------
    
    int newtonCount = 1;
    real tabserr = pow(10.,-8); //would be nice to pass this parameter

    while(normRHS > tabserr){
        coeff = 0.;

	if( cDecouple!=0 )
	{
	  coeff = op.identityCoefficients(e0,c0) - dt0*( multiply(CompgridVelocityx,op.xCoefficients(e0,c0)) )
            -dt0*( multiply(CompgridVelocityy,op.yCoefficients(e0,c0)) )
             
            -dt0*(multiply( (0.5*newtCur(c0)*(newtCur.x()(c0)*newtCur.x()(c1) + newtCur.y()(c0)*(newtCur.y()(c1) + G*ones))) ,op.identityCoefficients(e0,c0)) )
            -dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*newtCur.x()(c1)),op.xCoefficients(e0,c0) ) )
            - dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*(newtCur.y()(c1) + G*ones)),op.yCoefficients(e0,c0) ) )
            - dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*newtCur.laplacian()(c1)),op.identityCoefficients(e0,c0) ) )
            - dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*newtCur.x()(c0)),op.xCoefficients(e0,c1) ) )
            - dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*newtCur.y()(c0)),op.yCoefficients(e0,c1) ) )
            - dt0*( multiply( ((1./12.)*newtCur(c0)*newtCur(c0)*newtCur(c0)),op.laplacianCoefficients(e0,c1) ) )
            + S*op.laplacianCoefficients(e1,c0)
            + op.identityCoefficients(e1,c1);
	}
	else
	{
	  coeff = op.identityCoefficients(e0,c0) - dt0*( multiply(CompgridVelocityx,op.xCoefficients(e0,c0)) )
            -dt0*( multiply(CompgridVelocityy,op.yCoefficients(e0,c0)) )
            + S*op.laplacianCoefficients(e1,c0)
            + op.identityCoefficients(e1,c1);
	}
	
        // Start with dirichlet boundary conditions
        coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,allBoundaries);
        coeff.applyBoundaryConditionCoefficients(1,1,dirichlet,allBoundaries);
        coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries,bcParams);
        coeff.applyBoundaryConditionCoefficients(1,1,extrapolate,allBoundaries,bcParams);
        coeff.finishBoundaryConditions();
        
        solver.setCoefficientArray( coeff );
        rhs = -rhs;
        
        newtDiff = 0.;
        time0 = getCPU();
        solver.solve(newtDiff,rhs);
        time = getCPU() -time0;
        printF("time for solving of the problem = %8.2e, (iterations=%i)\n",time,solver.getNumberOfIterations());
        
        // update current newton iteration
        newtCur = newtCur + newtDiff;

	// **** This may not be needed if the initial guess satisfies the interpolation equations
        newtCur.interpolate();

        rhs = 0.;
        
        // ------------ Recalculate F ---------------------
        evaluateThinFilmFunction(  rhs, gf, newtCur,  mNew, mCur, t0, dt0,  parameters );
	
         
         maxr1=0.;
         maxr0=0.;
         normRHS=0.;
         for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
         {
             MappedGrid & mg = gf[mNew].cg[grid];
             getIndex(mg.indexRange(),I1,I2,I3);
            

             where(mg.mask()(I1,I2,I3)>0){
                 realArray r0 = abs(rhs[grid](I1,I2,I3,0));
                 realArray r1 = abs(rhs[grid](I1,I2,I3,1));
                 normRHS = max(max(r1),max(max(r0),normRHS));
                 maxr0 = max(maxr0,max(r0));
                 maxr1 = max(maxr1,max(r1));
             }
         }
         
         printF("Norm of RHS is %e.  The max of r0 is %e.  The max of r1 is %e. \n",normRHS,maxr0,maxr1);
        
         //rhs.display();

         //normRHS = pow(10.,-6);
         newtonCount = newtonCount + 1;
    }//end of while loop
    
    gf[mNew].u = newtCur;
    
    maxvalh = 0.;
    maxvalp = 0.;
    for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
    {
        int extra=0;  // include ghost points in error

        realMappedGridFunction & err = error[grid];
        realMappedGridFunction & uNew = gf[mNew].u[grid];
        MappedGrid & mg = gf[mNew].cg[grid];

        getIndex(mg.indexRange(),I1,I2,I3,extra);

        err=0.;
        where( mg.mask()(I1,I2,I3)!=0 )
        {
            err(I1,I2,I3,0) = abs(uNew(I1,I2,I3,0)-exact(mg,I1,I2,I3,0,t0+dt0));
            err(I1,I2,I3,1) = abs(uNew(I1,I2,I3,1)-exact(mg,I1,I2,I3,1,t0+dt0));
            //error=max(error,max(abs(err(I1,I2,I3))));
            maxvalh = max(maxvalh,max(abs(exact(cg[grid],I1,I2,I3,0,t0+dt0))));
            maxvalp = max(maxvalp,max(abs(exact(cg[grid],I1,I2,I3,1,t0+dt0))));
            //err(I1,I2,I3) = err(I1,I2,I3)/maxval;
        }

	printF(" grid=%i: err(h)=%9.3e, err(p)=%9.3e\n",grid,max(err(I1,I2,I3,0)),max(err(I1,I2,I3,1)));
	
    }
    
    printF("max value of thickness is %e and max value of pressure is %e \n",maxvalh,maxvalp);


    show.startFrame();
    //show.saveComment(0,sPrintF(buffer,"Here is the error at t=%e.",t0+dt0));
    show.saveSolution( error );
    
  if( correction==0 )
  {
    // printF(" +++ ims: gf[mNew].t=%9.3e --> change to t0+dt0=%9.3e +++\n",gf[mNew].t,t0+dt0);
    gf[mNew].t=t0+dt0;  // gf[mNew] now lives at this time
  }

  //   delete exactPointer;
    
  return 0;
}



