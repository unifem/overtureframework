// This file automatically generated from assignBoundaryConditions.bC with bpp.
#include "Cgsm.h"
#include "SmParameters.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "UnstructuredMapping.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "RadiationBoundaryCondition.h"
#include "ParallelUtility.h"
#include "Interface.h" 

// --- put this forward declaration here for now ---
// int
// getInterfaceData( real t, int grid, int side, int axis, 
// 		  int interfaceDataOptions,
// 		  RealArray & data,
//                   Parameters & parameters );

#define ForBoundary(side,axis)   for( int axis=0; axis<mg.numberOfDimensions(); axis++ ) for( int side=0; side<=1; side++ )

// =========================================================================================================
/// \brief Get the interfce data need to assign boundary conditions.
// =========================================================================================================
int Cgsm::
getInterfaceBoundaryData( int current )
{
    const Parameters::InterfaceCommunicationModeEnum & interfaceCommunicationMode= 
        parameters.dbase.get<Parameters::InterfaceCommunicationModeEnum>("interfaceCommunicationMode");

    if( interfaceCommunicationMode!=Parameters::requestInterfaceDataWhenNeeded )
        return 0;

    GridFunction & cgf = gf[current];
    CompositeGrid & cg = cgf.cg;
    const real t= cgf.t; 
    const int numberOfDimensions = cg.numberOfDimensions();
    
    assert( parameters.dbase.has_key("interfaceType") );
    
    const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");

  // --- Here is the interface data we need ---
    int interfaceDataOptions = Parameters::tractionInterfaceData;
    int numberOfItems=numberOfDimensions;

    SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
    if( pdeVariation==SmParameters::godunov )
    {
    // The godunov solver also needs the time derivative of the traction: 
        interfaceDataOptions = interfaceDataOptions | Parameters::tractionRateInterfaceData;
        numberOfItems+=numberOfDimensions;
    }	  
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    const int uc = parameters.dbase.get<int >("uc");
    const int v1c = parameters.dbase.get<int >("v1c");
    Range Dc(uc,uc+numberOfDimensions-1);    // displacement components
    Range Vc(v1c,v1c+numberOfDimensions-1);  // velocity components
    Range Rx = numberOfDimensions;
    
    InterfaceData interfaceData;
    Range C=numberOfItems;
    RealArray & ui = interfaceData.u;

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        MappedGrid & mg = cg[grid];
        ForBoundary(side,axis)
        {
            if(  interfaceType(side,axis,grid)==Parameters::tractionInterface )
            {
        // --- This is an FSI interface ----

        // *new* way June 25, 2017 -- explicitly request interface data from other solver (e.g. Cgins)
                if( true )
                    printF("--SM-- getInterfaceBoundaryData: REQUEST interface data (grid,side,axis)=(%i,%i,%i) "
                                  "at t=%9.3e\n",grid,side,axis,t);
                        
                getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
                
                ui.redim(I1,I2,I3,C);
                interfaceData.t=t;
                interfaceData.u=0;

                bool saveTimeHistory=true;  // do this for now
                getInterfaceData( t, grid, side, axis, 
                                                    interfaceDataOptions,
                                                    interfaceData.u,
                                                    parameters,saveTimeHistory );

        // save the RHS values here:
                RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

                bd(I1,I2,I3,Dc)=ui(I1,I2,I3,Rx); // save traction here
                if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
                {
                    bd(I1,I2,I3,Vc)=ui(I1,I2,I3,Rx+numberOfDimensions);
                }
                
        // RealArray & interfaceVelocity =interfaceData.u;
        // bcVelocity(I1,I2,I3,Rx)=interfaceVelocity(I1,I2,I3,Rx);

            }
        }
    }

    return 0;

}


// =========================================================================================================
/// \brief High-level apply boundary conditions routine
///
///  \param option: 
///  \note: Apply BC to "current" solution at time t optionally using "prev" for the solution at t-dt (prev is
///     not currently used. 
///
// =========================================================================================================
void Cgsm::
applyBoundaryConditions( int option, real dt, int current, int prev )
{

    GridFunction & cgf = gf[current];
    const real t= cgf.t; 

  // Get data for any interfaces 
    getInterfaceBoundaryData( current );

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        realMappedGridFunction & fieldCurrent = gf[current].u[grid];
        realMappedGridFunction & fieldPrev    = gf[prev].u[grid];

    // --- Get forcing for BC's  ---
      
    // determine time dependent conditions:
        getTimeDependentBoundaryConditions( cgf.cg[grid],cgf.t,grid ); 

        if( parameters.thereAreTimeDependentUserBoundaryConditions(nullIndex,nullIndex,grid)>0 )
        {
      // there are user defined boundary conditions
      // realMappedGridFunction & gridVelocity = cgf.getGridVelocity(grid);
      // userDefinedBoundaryValues( cgf.t,cgf.u[grid],gridVelocity,grid);
            userDefinedBoundaryValues( cgf.t,cgf,grid); // *new way* 12/02/20
        }

    // Projection of velocity (for INS-SM AMP scheme) **FINISH ME**
        projectInterface( grid, dt, current );

        assignBoundaryConditions( option, grid, t, dt, fieldCurrent, fieldPrev,current );

    }
}


// =========================================================================================================
/// \brief Here is apply BC function from the DomainSolver (used in adaptGrids)
///
///  \param option: 
///
// =========================================================================================================
int Cgsm::
applyBoundaryConditions(GridFunction & cgf,
                                                const int & option /* =-1 */ ,
                                                int grid_ /* = -1 */ ,
                                                GridFunction *puOld /* =NULL */ , 
                                                const real & dt /* =-1. */ )
{
    
  // *wdh* 080929 assert( &(gf[current]) == &cgf );
    assert( grid_ == -1 );
    
    assert( numberOfTimeLevels> 0 );
    int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;

  // *wdh* 090829 : while we assign initial conditions gf[prev] may not be updated yet
  //  (after updating gf[current] we then apply BC's to gf[current])
    if( gf[prev].u.numberOfComponentGrids() != gf[current].u.numberOfComponentGrids() )
        prev=current;  

    applyBoundaryConditions( option, dt, current, prev );

    return 0;
}




// =========================================================================================================
/// \brief Apply boundary conditions.
///
///  \param option: 
///
// Note: uOld = u[current]
///
// =========================================================================================================
void Cgsm::
assignBoundaryConditions( int option, int grid, real t, real dt, realMappedGridFunction & u, 
                    			  realMappedGridFunction & uOld, int current )
{
    real time0=getCPU();

    if( ((SmParameters&)parameters).isSecondOrderSystem() )
    {
    // apply BCs for a second-order-system formulation
        assignBoundaryConditionsSOS( option,grid,t,dt,u,uOld,current );
    }
    else
    {
    // apply BCs for a first-order-system formulation
        assignBoundaryConditionsFOS( option,grid,t,dt,u,uOld,current );
    }

    parameters.dbase.get<RealArray >("timing")(parameters.dbase.get<int>("timeForBoundaryConditions"))+=getCPU()-time0;
}



void Cgsm::initializeInterfaces()
// =====================================================================================================
//   /Description:
//      Find the interfaces and initialize the work-space. 
// =====================================================================================================
{
    int & debug = parameters.dbase.get<int >("debug");
    if( true || debug & 4 )
        printF("initializeInterfaces....\n");
    

    const int numberOfDimensions = cg.numberOfDimensions();
    const int numberOfComponentGrids = cg.numberOfComponentGrids();
    const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
    const int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

    int grid1;
    for( grid1=0; grid1<cg.numberOfComponentGrids(); grid1++ )
    {

        MappedGrid & mg1 = cg[grid1];
        const IntegerArray & bc1 = mg1.boundaryCondition();
        const IntegerArray & share1 = mg1.sharedBoundaryFlag();

    // check for interface boundary conditions
        for( int dir1=0; dir1<mg1.numberOfDimensions(); dir1++ )
        {
            for( int side1=0; side1<=1; side1++ )
            {
      	if( bc1(side1,dir1)==SmParameters::interfaceBoundaryCondition )
      	{
        	  
                    for( int grid2=grid1+1; grid2<cg.numberOfComponentGrids(); grid2++ ) // only check higher numbered grids
        	  {
          	    MappedGrid & mg2 = cg[grid2];
          	    const IntegerArray & bc2 = mg2.boundaryCondition();
                        const IntegerArray & share2 = mg2.sharedBoundaryFlag();

          	    for( int dir2=0; dir2<mg2.numberOfDimensions(); dir2++ )
          	    {
            	      for( int side2=0; side2<=1; side2++ )
            	      {
            		if( bc2(side2,dir2)==SmParameters::interfaceBoundaryCondition &&
                                        share1(side1,dir1)==share2(side2,dir2) )   // **** do this for now
            		{
		  // **** interface found *************

                  // add an interface to the list: 
                                    interfaceInfo.push_back(InterfaceInfo(grid1,side1,dir1, grid2,side2,dir2));   
              		  
                                    InterfaceInfo & interface = interfaceInfo[interfaceInfo.size()-1];

                                    const int extra=0; // orderOfAccuracyInSpace/2;
                                    getBoundaryIndex(mg1.dimension(),side1,dir1,I1,I2,I3,extra);
                                    getBoundaryIndex(mg2.dimension(),side2,dir2,J1,J2,J3,extra);

                                    const int ndf1=I1.length()*I2.length()*I3.length();  // number of points on interface face 1
              		  const int ndf2=J1.length()*J2.length()*J3.length();  // number of points on interface face 2

                                    interface.ndf1=ndf1; // number of points on face1 (is at most this value)
                                    interface.ndf2=ndf2;

                                    const int ndf=max(ndf1,ndf2);

                  // allocate work-space
                                    int nrwk=0, niwk=0;
                                    if( orderOfAccuracyInSpace==2 )
              		  {
                                        nrwk= 4*4*2*ndf;        // a4, matrix at each point is (4,4), we need 2 copies
                		    niwk=  4*ndf;            // ipvt
              		  }
              		  else if( orderOfAccuracyInSpace==4 )
              		  {
                                        
                                        nrwk= 8*8*2*ndf +       // a8, matrix at each point is (8,8), we need 2 copies
                      		          4*4*2*ndf;        // a4 (for Hz)
                		    niwk=  8*ndf + 4*ndf;    // ipvt8, ipvt4
                		    
              		  }
              		  else
              		  {
              		  }
              		  if( nrwk>0 )
              		  {
                                        printF("**** allocate rwk[%i] ****\n",nrwk);
                		    
                  		    interface.rwk = new real [nrwk];
              		  }
                                    if( niwk>0 )
              		  {
                                        printF("**** allocate iwk[%i] ****\n",niwk);

                		    interface.iwk = new int [niwk];
              		  }
              		  
            		}
            	      }
          	    }
        	  }
      	}
            }
        }
    }
}


void Cgsm::
assignInterfaceBoundaryConditions( int current, real t, real dt )
// =====================================================================================================
//   /Description:
//      Assign the boundary conditions at the interface between two different materials
//   Values are assigned on the "next" grid function.
// =====================================================================================================
{
    return;
    
// * 
// *   if( !gridHasMaterialInterfaces ) return;
// * 
// *   real time0=getCPU();
// * 
// *   checkArrays("assignInterfaceBoundaryConditions:start");
// * 
// *   if( interfaceInfo.size()==0 )
// *   {
// *     initializeInterfaces();
// *   }
// *   
// * 
// * //FILE *debugFile2 = debugFile;
// * 
// *   const int numberOfDimensions = cg.numberOfDimensions();
// *   const int numberOfComponentGrids = cg.numberOfComponentGrids();
// * 
// *   Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
// *   Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
// * 
// *   const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
// *   const int next = (current+1) % numberOfTimeLevels;
// * 
// *   // *** could iterate to solve the interface conditions for 4th order
// * 
// *   // loop over interfaces
// *   for( int inter=0; inter < interfaceInfo.size(); inter++ )
// *   {
// *     InterfaceInfo & interface = interfaceInfo[inter]; 
// * 
// *     const int grid1=interface.grid1, side1=interface.side1, dir1=interface.dir1;
// *     const int grid2=interface.grid2, side2=interface.side2, dir2=interface.dir2;
// *     
// *     MappedGrid & mg1 = cg[grid1];
// *     const IntegerArray & bc1 = mg1.boundaryCondition();
// *     const IntegerArray & share1 = mg1.sharedBoundaryFlag();
// * 
// *     MappedGrid & mg2 = cg[grid2];
// *     const IntegerArray & bc2 = mg2.boundaryCondition();
// *     const IntegerArray & share2 = mg2.sharedBoundaryFlag();
// * 
// *     const int extra=0; // orderOfAccuracyInSpace/2;
// *     getBoundaryIndex(mg1.gridIndexRange(),side1,dir1,I1,I2,I3,extra);
// *     getBoundaryIndex(mg2.gridIndexRange(),side2,dir2,J1,J2,J3,extra);
// *   
// * 
// * //     if( false )
// * //     {
// * //       gf[next].u[grid1].periodicUpdate();  // *** fix this ***
// * //       gf[next].u[grid2].periodicUpdate();
// * //     }
// * 
// * 
// *     realArray & u1 = gf[next].u[grid1];
// *     realArray & u2 = gf[next].u[grid2];
// * 
// *     int n1a=I1.getBase(),n1b=I1.getBound(),
// *       n2a=I2.getBase(),n2b=I2.getBound(),
// *       n3a=I3.getBase(),n3b=I3.getBound();
// * 
// *     int m1a=J1.getBase(),m1b=J1.getBound(),
// *       m2a=J2.getBase(),m2b=J2.getBound(),
// *       m3a=J3.getBase(),m3b=J3.getBound();
// * 
// * 
// *     bool isRectangular1= mg1.isRectangular();
// *     real dx1[3]={0.,0.,0.}; //
// *     if( isRectangular1 )
// *       mg1.getDeltaX(dx1);
// * 
// *     bool isRectangular2= mg2.isRectangular();
// *     real dx2[3]={0.,0.,0.}; //
// *     if( isRectangular2 )
// *       mg2.getDeltaX(dx2);
// * 
// *     if( true )
// *     { // for testing -- make rectangular grids look curvilinear ***************************************
// *       isRectangular1=false;
// *       isRectangular2=false;
// *     }
// *     if( !isRectangular1 )
// *     {
// *       mg1.update(MappedGrid::THEinverseVertexDerivative);
// *       mg2.update(MappedGrid::THEinverseVertexDerivative);
// *     }
// * 		  
// *     assert(isRectangular1==isRectangular2);
// * 
// *     int gridType = isRectangular1 ? 0 : 1;
// *     int orderOfExtrapolation=orderOfAccuracyInSpace+1;  // not used
// *     int useForcing = forcingOption==twilightZoneForcing;
// *     int useWhereMask=true;
// * 
// *     int ierr=0;
// *     int ipar[]={ //
// *       side1, dir1, grid1,
// *       n1a,n1b,n2a,n2b,n3a,n3b,
// *       side2, dir2, grid2,
// *       m1a,m1b,m2a,m2b,m3a,m3b,
// *       gridType,            
// *       orderOfAccuracyInSpace,    
// *       orderOfExtrapolation,
// *       useForcing,          
// *       ex,                  
// *       ey,                  
// *       ez,                  
// *       hx ,                 
// *       hy,                  
// *       hz,                  
// *       (int)solveForElectricField,          
// *       (int)solveForMagneticField,          
// *       useWhereMask,       
// *       debug,
// *       numberOfIterationsForInterfaceBC,
// *       materialInterfaceOption,
// *       (int)interface.initialized
// *     };
// * 		  
// *     real rpar[]={ //
// *       dx1[0],
// *       dx1[1],
// *       dx1[2],
// *       mg1.gridSpacing(0),
// *       mg1.gridSpacing(1),
// *       mg1.gridSpacing(2),
// *       dx2[0],
// *       dx2[1],
// *       dx2[2],
// *       mg2.gridSpacing(0),
// *       mg2.gridSpacing(1),
// *       mg2.gridSpacing(2),
// *       t,    
// *       (real &)tz,  // twilight zone pointer
// *       dt,    
// *       epsGrid(grid1),
// *       muGrid(grid1),   
// *       cGrid(grid1),    
// *       epsGrid(grid2),  
// *       muGrid(grid2),   
// *       cGrid(grid2),
// *       omegaForInterfaceIteration
// *     };
// * 		  
// *     real *u1p=u1.getDataPointer();
// *     real *prsxy1=isRectangular1 ? u1p : mg1.inverseVertexDerivative().getDataPointer();
// *     real *pxy1=u1p;  // not currently used.
// *     int *mask1p=mg1.mask().getDataPointer();
// * 
// *     real *u2p=u2.getDataPointer();
// *     real *prsxy2=isRectangular2 ? u2p : mg2.inverseVertexDerivative().getDataPointer();
// *     real *pxy2=u2p;  // not currently used.
// *     int *mask2p=mg2.mask().getDataPointer();
// * 
// *     if( orderOfAccuracyInSpace<6 )
// *     {
// *       real *rwk=interface.rwk;
// *       int *iwk=interface.iwk;
// *       assert( rwk!=NULL && iwk!=NULL );
// *       
// *       const int ndf = max(interface.ndf1,interface.ndf2); 
// * 
// *       // assign pointers into the work spaces
// *       int pa2=0,pa4=0,pa8=0, pipvt2=0,pipvt4=0,pipvt8=0;
// *       if( orderOfAccuracyInSpace==2 )
// *       {
// *         pa2=0; 
// *         pa4=pa2 + 2*2*2*ndf;
// * 	pa8=0;  // not used
// * 	
// * 	pipvt2=0;
// * 	pipvt4=pipvt2 + 2*ndf; 
// * 	pipvt8=0;
// *       }
// *       else if( orderOfAccuracyInSpace==4 )
// *       {
// *         pa2=0; // not used
// *         pa4=0;
// * 	pa8=pa4+4*4*2*ndf;
// * 	
// * 	pipvt2=0;
// * 	pipvt4=0;
// * 	pipvt8=pipvt4+4*ndf;
// * 	
// *       }
// *       
// * 
// * 
// *       interfaceSolidMechanics( mg1.numberOfDimensions(), 
// * 			u1.getBase(0),u1.getBound(0),
// * 			u1.getBase(1),u1.getBound(1),
// * 			u1.getBase(2),u1.getBound(2),
// * 			mg1.gridIndexRange(0,0), *u1p, *mask1p,*prsxy1, *pxy1, bc1(0,0), 
// * 			u2.getBase(0),u2.getBound(0),
// * 			u2.getBase(1),u2.getBound(1),
// * 			u2.getBase(2),u2.getBound(2),
// * 			mg2.gridIndexRange(0,0), *u2p, *mask2p,*prsxy2, *pxy2, bc2(0,0), 
// * 			ipar[0], rpar[0], 
// *                         rwk[pa2],rwk[pa4],rwk[pa8], iwk[pipvt2],iwk[pipvt4],iwk[pipvt8],
// *                         ierr );
// * 		    
// *     }
// *     else
// *     {
// *       newInterfaceSolidMechanics( mg1.numberOfDimensions(), 
// * 			   u1.getBase(0),u1.getBound(0),
// * 			   u1.getBase(1),u1.getBound(1),
// * 			   u1.getBase(2),u1.getBound(2),
// * 			   mg1.gridIndexRange(0,0), *u1p, *mask1p,*prsxy1, *pxy1, bc1(0,0), 
// * 			   u2.getBase(0),u2.getBound(0),
// * 			   u2.getBase(1),u2.getBound(1),
// * 			   u2.getBase(2),u2.getBound(2),
// * 			   mg2.gridIndexRange(0,0), *u2p, *mask2p,*prsxy2, *pxy2, bc2(0,0), 
// * 			   ipar[0], rpar[0], ierr );
// * 
// *     }
// * 		  
// *     interface.initialized=true;
// *     
// * 
// * //     if( false ) // this is done inside the interface routine
// * //     {
// * //       gf[next].u[grid1].periodicUpdate();  // *** fix this ***
// * //       gf[next].u[grid2].periodicUpdate();
// * //     }
// * 		  
// * 
// *     if( tz!=NULL && pDebugFile!=NULL )
// *     {
// * 		    
// *       OGFunction & e = *tz;
// * 
// *       getGhostIndex(mg1.gridIndexRange(),side1,dir1,I1,I2,I3);
// * 
// *       realArray err(I1,I2,I3);
// * 		  
// *       err=u1(I1,I2,I3,ex)-e(mg1,I1,I2,I3,ex,t+dt);
// *       ::display(err,sPrintF("err in u1 (ex,ghost) after interface, t=%e",t+dt),pDebugFile,"%8.1e ");
// * 
// *       getGhostIndex(mg1.gridIndexRange(),side1,dir1,I1,I2,I3,-1);
// *       err=u1(I1,I2,I3,ex)-e(mg1,I1,I2,I3,ex,t+dt);
// *       ::display(err,sPrintF("err in u1 (ex,line 1) after interface, t=%e",t+dt),pDebugFile,"%8.1e ");
// * 
// * 
// *       getGhostIndex(mg2.gridIndexRange(),side2,dir2,I1,I2,I3);
// *                     
// *       err=u2(I1,I2,I3,ex)-e(mg2,I1,I2,I3,ex,t+dt);
// *       ::display(err,sPrintF("err in u2 (ex,ghost) after interface, t=%e",t+dt),pDebugFile,"%8.1e ");
// * 
// *       getGhostIndex(mg2.gridIndexRange(),side2,dir2,I1,I2,I3,-1);
// *                     
// *       err=u2(I1,I2,I3,ex)-e(mg2,I1,I2,I3,ex,t+dt);
// *       ::display(err,sPrintF("err in u2 (ex,line 1) after interface, t=%e",t+dt),pDebugFile,"%8.1e ");
// *       
// *     }
// *   }
// * 
// *   timing(Smparameters.dbase.get<int>("timeForInterfaceBC"))+=getCPU()-time0;
// *   return;
}
