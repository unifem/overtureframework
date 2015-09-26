// This file automatically generated from interface.bC with bpp.
//
//  -- 2012/05/31 -- version taken from Cgcns (to support FSI)
//  -- We should really share this code
//
#include "Cgins.h"
#include "InsParameters.h"
#include "Interface.h"  
#include "ParallelUtility.h"

// include some interface bpp macros
//------------------------------------------------------------------------------------
// This file contains macros used by the interface routines.
//------------------------------------------------------------------------------------



// ===========================================================================
// Get/set the interface RHS for a heat flux interface
// ===========================================================================

// ===================================================================================
/// \brief Return the interface data required for a given type of interface.
/// \param info (input) : the descriptor for the interface.
/// \param interfaceDataOptions (output) : a list of items from Parameters::InterfaceDataEnum that define
///                    which data to get (or which data were set).  Multiple items are
///                     chosen by bit-wise or of the different options
/// \note: this function should be over-loaded.
// ===================================================================================
int Cgins::
getInterfaceDataOptions( GridFaceDescriptor & info, int & interfaceDataOptions ) const
{
    int numberOfItems=0;
    const int numberOfDimensions = parameters.dbase.get<int>("numberOfDimensions");

  // Here is the grid face on the interface: 
    const int grid=info.grid, side=info.side, axis=info.axis;

    IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");
    if( grid<0 || grid>interfaceType.getBound(2) ||
            side<0 || side>1 || axis<0 || axis>interfaceType.getBound(1) )
    {
        printP("Cgins::getInterfaceDataOptions:ERROR: invalid values: (grid,side,axis)=(%i,%i,%i)\n",
         	   grid,side,axis);
        OV_ABORT("Cgins::getInterfaceDataOptions:ERROR");
    }
    if( interfaceType(side,axis,grid)==Parameters::heatFluxInterface )
    {
        interfaceDataOptions=Parameters::heatFluxInterfaceData;
        numberOfItems+=1;
    }
    else if( interfaceType(side,axis,grid)==Parameters::tractionInterface ) 
    {
        const bool projectInterface = parameters.dbase.get<bool>("projectInterface");

    // Boundary condition on the interrface: 
    //   Use this to decide if the boundary is a velocity BC or a traction BC.
        const int bc = gf[0].cg[grid].boundaryCondition(side,axis);
        
        if( debug() & 2 )
            printP("*** Cgins:getInterfaceDataOptions: projectInterface = %i ***\n",projectInterface);

        if( !projectInterface )
        {
      // No interface projection:

            if( bc==InsParameters::noSlipWall ||
        	  bc==InsParameters::slipWall )
            {
	// For a velocity boundary we need the position of the interface from the
        //    opposite domain (traditional scheme)
      	interfaceDataOptions=Parameters::positionInterfaceData;
      	numberOfItems+=numberOfDimensions;
            }
            else if( bc==InsParameters::freeSurfaceBoundaryCondition || 
                              bc==InsParameters::tractionFree )
            {
        // For a traction boundary (or "freeSurface") we need the traction from the
        //    opposite domain (traditional scheme)

      	if( TRUE )
        	  printP("*** Cgins:getInterfaceDataOptions: (grid,side,axis)=(%i,%i,%i) bc=%i (traction or freeSurface)\n",grid,side,axis,bc);

      	interfaceDataOptions=Parameters::Parameters::tractionInterfaceData;
      	numberOfItems+=numberOfDimensions;
            }
            else
            {
      	printP(" --Cgins-- getInterfaceDataOptions: ERROR: unexpected BC=%i for a traction interface\n",bc);
            }
            
        }
        else
        {
      // When we project the interface values we need the following interface: 
      //  - position
      //  - velocity
      //  - traction 
      //  - acceleration 
      //  - traction-rate  : d(traction)/dt 
      // Parameters::accelerationInterfaceData 
      // Parameters::tractionRateInterfaceData 

            interfaceDataOptions = ( Parameters::positionInterfaceData     |
                         			       Parameters::velocityInterfaceData     |
                         			       Parameters::tractionInterfaceData    
                                                          );

            numberOfItems+=3*numberOfDimensions;
        }

    }
    else
    {
        printP("Cgins::getInterfaceDataOptions:ERROR: interfaceType(grid=%i,side=%i,axis=%i)=%i\n",
         	   grid,side,axis,interfaceType(side,axis,grid));
        OV_ABORT("Cgins::getInterfaceDataOptions:ERROR");
    }
    
    return numberOfItems;
}

// ---------- TEMP : from UserDefinedKnownSolution ---
namespace
{
enum UserDefinedKnownSolutionEnum
{
    unknownSolution=0,
    specifiedPiston=1,
    forcedPiston=2,
    obliqueShockFlow=3,
    superSonicExpandingFlow=4,
    exactSolutionFromAFile=5
};
}


// ===========================================================================
/// \brief Set or get the right-hand-side for an interface boundary condition.
/// \details This function is used when solving the interface equations 
///           by iteration.
/// \param option (input) : option=getInterfaceRightHandSide : get the RHS, 
///                         option=setInterfaceRightHandSide : set the RHS
/// \param interfaceDataOptions (input) : a list of items from Parameters::InterfaceDataEnum that define
////                    which data to get (or which data were set).  Multiple items are
///                     chosen by bit-wise or of the different options   
/// \param info (input) : contains the GridFaceDescriptor info used to set the right-hand-side.
/// \param gfd (input) : the master GridFaceDescriptor. 
/// \param gfIndex (input) : use the solution from gf[gfIndex]
/// \param t (input) : time at which the RHS values are required.
// ===========================================================================
int
Cgins::
interfaceRightHandSide( InterfaceOptionsEnum option, 
                                                int interfaceDataOptions,
                                                GridFaceDescriptor & info, 
                                                GridFaceDescriptor & gfd, 
                  			int gfIndex, real t )
{
  // return DomainSolver::interfaceRightHandSide(option,interfaceDataOptions,info,gfIndex,t);
  // *wdh* 081212 CompositeGrid & cg = gf[0].cg;
  // CompositeGrid & cg = gf[0].cg;
    FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

    CompositeGrid & cg = gf[gfIndex].cg;
    const int numberOfDimensions = cg.numberOfDimensions();
    
    const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");

    const int grid=info.grid, side=info.side, axis=info.axis;

    if( grid<0 || grid>=cg.numberOfComponentGrids() ||
            side<0 || side>1 || axis<0 || axis>=cg.numberOfDimensions() )
    {
        printP("Cgins::interfaceRightHandSide:ERROR: invalid values: (grid,side,axis)=(%i,%i,%i)\n",
         	   grid,side,axis);
        OV_ABORT("Cgins::interfaceRightHandSide:ERROR");
    }

    MappedGrid & mg = cg[grid];
    RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
    real dt=0.;  // *wdh* 101106 - do not change DomainSolver::dt 

    const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")-
        parameters.dbase.get<int >("numberOfExtraVariables");

    assert( info.u != NULL );
    RealArray & f = *info.u;
    Index I1=f.dimension(0),I2=f.dimension(1),I3=f.dimension(2);

    if( interfaceType(side,axis,grid)==Parameters::heatFluxInterface )
    {
    // ****************************************
    // ********* Heat flux interface **********
    // ****************************************

        real *a = info.a;
        if( debug() & 4 )
        {
            printP("Cgins::interfaceRHS:heatFlux %s RHS for (side,axis,grid)=(%i,%i,%i) a=[%5.2f,%5.2f]"
               	 " t=%9.3e gfIndex=%i (current=%i)\n",
               	 (option==0 ? "get" : "set"),side,axis,grid,a[0],a[1],t,gfIndex,current);
        }
        const int tc = parameters.dbase.get<int >("tc");   
        assert( tc>=0 );
        Range N(tc,tc);
    // We could optimize this for rectangular grids 
        mg.update(MappedGrid::THEvertexBoundaryNormal);
        #ifdef USE_PPP
        const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
        #else
        const realSerialArray & normal = mg.vertexBoundaryNormal(side,axis);
        #endif
        if( option==setInterfaceRightHandSide )
        {
      // **** set the RHS *****
      //   (TZ is done below)
            bd(I1,I2,I3,tc)=f(I1,I2,I3,tc);
            if( false )
            {
                ::display(bd(I1,I2,I3,tc)," RHS values","%4.2f ");
            }
        }
        else if( option==getInterfaceRightHandSide )
        {
      // **** get the RHS ****
            realMappedGridFunction & u = gf[gfIndex].u[grid];
        #ifdef USE_PPP
            realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
        #else
            realSerialArray & uLocal = gf[gfIndex].u[grid];
        #endif
            f(I1,I2,I3,tc) = a[0]*uLocal(I1,I2,I3,tc);
            if( a[1]!=0. )
            {
        // add on a[1]*( nu*u.n ) on the boundary 
        // **be careful** -- the normal changes sign on the two sides of the interface ---
                MappedGridOperators & op = *(u.getOperators());
                realSerialArray ux(I1,I2,I3,N), uy(I1,I2,I3,N);
                op.derivative(MappedGridOperators::xDerivative,uLocal,ux,I1,I2,I3,N);
                op.derivative(MappedGridOperators::yDerivative,uLocal,uy,I1,I2,I3,N);
                if( cg.numberOfDimensions()==2 )
                {
                    f(I1,I2,I3,tc) += a[1]*( normal(I1,I2,I3,0)*ux + normal(I1,I2,I3,1)*uy );
                }
                else
                {
                    realSerialArray uz(I1,I2,I3);
                    op.derivative(MappedGridOperators::zDerivative,uLocal,uz,I1,I2,I3,N);
                    f(I1,I2,I3,tc) += a[1]*( normal(I1,I2,I3,0)*ux + normal(I1,I2,I3,1)*uy + normal(I1,I2,I3,2)*uz );
                }
            }
            if( debug() & 4 )
            {
                ::display(f(I1,I2,I3,tc) ,sPrintF("getRHS:  %f*u + %f*( u.n ) ",a[0],a[1]));
            }
        }
        else
        {
            printF("Cgins::interfaceRightHandSide:ERROR: unknown option=%i\n",option);
            Overture::abort("error");
        }
        if( // false &&  // turn this off for testing the case where the same TZ holds across all domains
            parameters.dbase.get<bool >("twilightZoneFlow") )
        {
      // ---add forcing for twlight-zone flow---
            OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
            const bool isRectangular = false; // ** do this for now ** mg.isRectangular();
            if( !isRectangular )
                mg.update(MappedGrid::THEcenter);
            realArray & x= mg.center();
        #ifdef USE_PPP
            realSerialArray xLocal; 
            if( !isRectangular ) 
                getLocalArrayWithGhostBoundaries(x,xLocal);
        #else
            const realSerialArray & xLocal = x;
        #endif
            realSerialArray ue(I1,I2,I3,N);
            if( a[0]!=0. )
            {
                e.gd( ue ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,N,t);  // exact solution 
                ue(I1,I2,I3,N) = a[0]*ue(I1,I2,I3,N);
            }
            else
            {
                ue(I1,I2,I3,N) =0.;
            }
            if( a[1]!=0. )
            {
                realSerialArray uex(I1,I2,I3,N), uey(I1,I2,I3,N);
                e.gd( uex ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,N,t);
                e.gd( uey ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,N,t);
                if( numberOfDimensions==2 )
                {
                    ue(I1,I2,I3,N) += a[1]*( normal(I1,I2,I3,0)*uex + normal(I1,I2,I3,1)*uey );
                }
                else
                {
                    realSerialArray uez(I1,I2,I3,N);
                    e.gd( uez ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,N,t);
                    ue(I1,I2,I3,N) += a[1]*( normal(I1,I2,I3,0)*uex + normal(I1,I2,I3,1)*uey + normal(I1,I2,I3,2)*uez ); 
                }
            }
            if( option==getInterfaceRightHandSide )
            { // get 
        //   subtract off TZ flow:
        //   f <- f - ( a[0]*ue + a[1]*( nu*ue.n ) )
                if( false )
                {
                    ::display(f(I1,I2,I3,tc) ," a[0]*u + a[1]*( k u.n )");
                    ::display(ue(I1,I2,I3,tc)," a[0]*ue + a[1]*( k ue.n )");
                }
                f(I1,I2,I3,tc) -= ue(I1,I2,I3,N);
                if( false )
                {
                    ::display(f(I1,I2,I3,tc) ," a[0]*u + a[1]*( k u.n ) - [a[0]*ue + a[1]*( k ue.n )]");
                }
            }
            else if( option==setInterfaceRightHandSide )
            { // set 
        //   add on TZ flow:
        //   bd <- bd + a[0]*ue + a[1]*( nu*ue.n )
                bd(I1,I2,I3,tc) += ue(I1,I2,I3,N);
                if( false )
                {
                    bd(I1,I2,I3,tc) = ue(I1,I2,I3,N);
                }
            }
            else
            {
                Overture::abort("error");
            }
        } // end if TZ 

    }
    else if( interfaceType(side,axis,grid)==Parameters::tractionInterface )
    {
    // *******************************************
    // ********** Traction Interface *************
    // *******************************************

        if( debug() & 2 )
            printP("Cgins::interfaceRHS: TRACTION: %s RHS for (grid,side,axis)=(%i,%i,%i) "
           	     " t=%9.3e gfIndex=%i (current=%i) gf[gfIndex].t=%9.3e\n",
           	     (option==0 ? "get" : "set"),grid,side,axis,t,gfIndex,current,gf[gfIndex].t);


        const int uc = parameters.dbase.get<int >("uc");
        Range V(uc,uc+numberOfDimensions-1);
        Range C;    // component range for indexing f.
        Range Ct;   // save traction components here 
        Range Ctr;  // save tractionRate here 
        
        if( option==setInterfaceRightHandSide )
        {
      // -----------------------------------------------------
      // ------------ Interface data has been provided -------
      // -----------------------------------------------------
      // **** set the RHS *****
      //   (TZ is done below) <- todo 

            int numSaved=0; // keeps track of how many things we have saved in f 

            bd=0.;  // boundary data is filled into this array


            if( interfaceDataOptions & Parameters::positionInterfaceData )
            {
        // --- interface position is given ---
      	C=Range(numSaved,numSaved+numberOfDimensions-1);
      	if( debug() & 2 )
        	  printP("Cgins::interfaceRHS: Set interface positions at t=%9.3e in bd components V=[%i,%i] from C=[%i,%i]\n",
             		 gf[gfIndex].t,V.getBase(),V.getBound(),C.getBase(),C.getBound());

      	bd(I1,I2,I3,V)=f(I1,I2,I3,C);  // set positions of interface -- fill into velocity components for now ***

      	numSaved+=numberOfDimensions;
            }
            
            if( false ) // **************** TEMP ************************
            {
                DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedKnownSolutionData");
      	UserDefinedKnownSolutionEnum & userKnownSolution = db.get<UserDefinedKnownSolutionEnum>("userKnownSolution");
      	
      	if( userKnownSolution==specifiedPiston )
      	{
        	  real ap=1., pp=4;
        	  real ff = -(ap/pp)*pow(t,pp);
        	  printP(" elasticPiston: t=%9.3e: F(t)=[%9.3e,%9.3e] (computed) true=%9.3e\n",t,max(bd(I1,I2,I3,uc)),min(bd(I1,I2,I3,uc)),ff);

          // printP(" elasticPiston: ************* SET INTERFACE POSITION TO EXACT *************\n");
          // bd(I1,I2,I3,uc)=ff; // ******************
      	}
      	
            }
            
            if( interfaceDataOptions & Parameters::velocityInterfaceData )
            {
        // --- interface velocity is given ---
      	C=Range(numSaved,numSaved+numberOfDimensions-1);
                if( debug() & 4 )
        	  printP("Cgins:interfaceRightHandSide: interface velocity provided t=%9.3e in C=[%i,%i]\n",t,C.getBase(),C.getBound());


        // the interface velocity is currently not used.
	// bd(I1,I2,I3,Dc)=f(I1,I2,I3,C);     // where should we put this?          

                numSaved+=numberOfDimensions;
            }

            if( interfaceDataOptions & Parameters::accelerationInterfaceData )
            {
        // --- interface acceleration is given ---
      	C=Range(numSaved,numSaved+numberOfDimensions-1);
                if( debug() & 4 )
        	  printP("Cgins:interfaceRightHandSide: interface acceleration provided t=%9.3e in C=[%i,%i]\n",t,C.getBase(),C.getBound());


        // the interface acceleration is currently not used.
	// bd(I1,I2,I3,Dc)=f(I1,I2,I3,C);    // where should we put this?

                numSaved+=numberOfDimensions;
            }

            if( interfaceDataOptions & Parameters::tractionInterfaceData )
            {
        // --- interface traction is given ---
                if( debug() & 4 )
        	  printP("Cgins:interfaceRightHandSide: interface traction provided t=%9.3e\n",t);

      	C=Range(numSaved,numSaved+numberOfDimensions-1);

        // the interface acceleration is currently not used.
	// bd(I1,I2,I3,Dc)=f(I1,I2,I3,C);    // where should we put this?

                numSaved+=numberOfDimensions;
            }

            if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
            {
	// save the time derivative of the traction:
                if( debug() & 4 )
        	  printP("Cgins:interfaceRightHandSide: the tractionRate is provided at t=%9.3e\n",t);

                C=Range(numSaved,numSaved+numberOfDimensions-1);

        // the interface traction rate is currently not used.
	// bd(I1,I2,I3,Dc)=f(I1,I2,I3,C);    // where should we put this?

      	numSaved+=numberOfDimensions;
            }


            if( debug() & 8 )
            {
                bd(I1,I2,I3,V).display("setInterfaceRightHandSide: Here is the RHS");
            }
        
        }
        else if( option==getInterfaceRightHandSide )
        {

      // -----------------------------------
      // ---- Return the interface data ----
      // -----------------------------------


            realMappedGridFunction & u = gf[gfIndex].u[grid];
            OV_GET_SERIAL_ARRAY(real,u,uLocal);


      // We could optimize this for rectangular grids 
            mg.update(MappedGrid::THEvertexBoundaryNormal);
            #ifdef USE_PPP
                const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
            #else
                const realSerialArray & normal = mg.vertexBoundaryNormal(side,axis);
            #endif

            Range Rx(0,numberOfDimensions-1);                // vertex components 

            int numSaved=0; // keeps track of how many things we have saved in f 

            if( interfaceDataOptions & Parameters::positionInterfaceData )
            {
	// -- return the position of the boundary --
      	if( debug() & 4 )
        	  printP("Cgins:interfaceRightHandSide: Save the interface position.\n");

      	Range Rx(0,numberOfDimensions-1);                // vertex components 
      	C=Range(numSaved,numSaved+numberOfDimensions-1); // save displacement in these components of f

        // We could optimize this for rectangular grids 
                mg.update(MappedGrid::THEvertex);
                OV_GET_SERIAL_ARRAY(real,mg.vertex(),vertex);

      	f(I1,I2,I3,C) = vertex(I1,I2,I3,Rx);
      	
      	numSaved+=numberOfDimensions;
            }
            if( interfaceDataOptions & Parameters::velocityInterfaceData )
            {
	// -- return the interface velocity --
      	if( debug() & 4 )
        	  printP("Cgins:interfaceRightHandSide: Save the interface velocity.\n");

      	C=Range(numSaved,numSaved+numberOfDimensions-1); // save displacement in these components of f

      	f(I1,I2,I3,C) = uLocal(I1,I2,I3,V);
      	
      	numSaved+=numberOfDimensions;
            }
            if( interfaceDataOptions & Parameters::accelerationInterfaceData )
            {
	// -- return the position of the boundary --
      	if( debug() & 4 )
        	  printP("Cgins:interfaceRightHandSide: Save the interface acceleration.\n");

      	Range Rx(0,numberOfDimensions-1);                // vertex components 
      	C=Range(numSaved,numSaved+numberOfDimensions-1); // save displacement in these components of f

      	printP("Cgins:interfaceRightHandSide: save acceleration - FINISH ME!\n");
      	
      	numSaved+=numberOfDimensions;
            }

      // -- Now save the traction and traction rate --

            Index Ib1,Ib2,Ib3;
      // getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
            Ib1=I1, Ib2=I2, Ib3=I3;
            realSerialArray traction(Ib1,Ib2,Ib3,numberOfDimensions);  // this should be a serial array

      // gf[gfIndex].conservativeToPrimitive();

            int ipar[] = {grid,side,axis,gf[gfIndex].form}; // 
            real rpar[] = { gf[gfIndex].t }; // 

            parameters.getNormalForce( gf[gfIndex].u,traction,ipar,rpar );

            Range D=numberOfDimensions;
            Ct=Range(numSaved,numSaved+numberOfDimensions-1);  // save interface traction in these components
            
            if( debug() & 2 )
            {
                printP("Cgins::interfaceRHS: Get normal force (traction) at t=%9.3e\n",gf[gfIndex].t);
	// ::display(traction(I1,I2,I3,D),"Cgins::interfaceRHS:normalForce","%5.2f ");
      	
            }
            
            if( interfaceDataOptions & Parameters::tractionInterfaceData )
            {
      	if( debug() & 2 )
        	  printP("Cgins:interfaceRightHandSide: Save the interface traction at t=%8.2e in components Ct=[%i,%i].\n",t,Ct.getBase(),Ct.getBound());

	// old: f(I1,I2,I3,V)=traction(I1,I2,I3,D);
      	f(I1,I2,I3,Ct)=traction(I1,I2,I3,D);

      	if( true )
      	{
        	  fprintf(pDebugFile,"-- interfaceRHS: interface data at t=%8.2e --\n",t);
        	  ::display(f(I1,I2,I3,Ct),"--INS-- traction f",pDebugFile,"%9.3e ");
      	}
      	
                numSaved+=numberOfDimensions;
            }
            
      // Here is the time we have actually computed the traction at. It may be less than t on the predictor step
            const real t0 = gf[gfIndex].t;  

      // -- check if we need the traction at a different time than the current solution --
      // (this could be a predictor-step for e.g.)

            bool tractionTimeDiffers = fabs(t-t0) > 100.*REAL_EPSILON;
            
      // Range Vt=V+numberOfDimensions;  // *********** save tractionRate here for now *** fix me ***
            if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
            {
	// -- save the interface traction rate --
      	if( debug() & 4 )
        	  printP("Cgins:interfaceRightHandSide: Save the interface traction rate.\n");

                C=Range(numSaved,numSaved+numberOfDimensions-1);
                Ctr=C;
            }
            

      // --------------------------------------------------------------------------------------
      //  (1) Compute the traction at a different time than the current time
      // AND/OR
      //  (2) Compute the traction rate from the time history
      //
      // --------------------------------------------------------------------------------------
            if( tractionTimeDiffers || interfaceDataOptions & Parameters::tractionRateInterfaceData )
            {

        // A time history of interface values is saved here in the master list gfd: 
                InterfaceDataHistory & idh = gfd.interfaceDataHistory;
                InterfaceDataHistory & idi = gfd.interfaceDataIterates; // iterates of interface values from the predictor corrector 
                if( idh.current>=0 && idh.current<idh.interfaceDataList.size() )
      	{

                    const int numberOfInterfaceHistoryValuesToSave=idh.interfaceDataList.size();
        	  
	  // find a previous time value we can use
                    int prev = idh.current;  // by default use this as the old solution
        	  real tp = idh.interfaceDataList[prev].t;
                    dt=0.; 
        	  if( fabs(t0-tp)<= REAL_EPSILON*1000. )
        	  {
            // if tp is the same as t0, look for an earlier time: 
          	    if( numberOfInterfaceHistoryValuesToSave>1 )
          	    {
            	      prev = ( prev -1 + numberOfInterfaceHistoryValuesToSave ) %  numberOfInterfaceHistoryValuesToSave;
            	      tp = idh.interfaceDataList[prev].t;
                            dt=t0-tp;
          	    }
          	    else
          	    {
            	      printP("Cgins::interfaceRHS:WARNING: there are no previous history values to compute tractionRate.\n"
                 		     "  The only history value is at time tp=%9.3e, but gf[gfIndex=%i].t=%9.3e\n",tp,gfIndex,t0);
                            dt=0.; 
          	    }
        	  }

        	  if( tractionTimeDiffers )
        	  {
	    // --- We need the traction at a time that differs from the current solution time  ---
	    // We can use the time history values and extrap/interp in time

	    // If there are no previous time history values we fill some appropriate values in.
	    //   -- for TZ : evaluate the exact solution
	    //   -- for real : just assume constant ? 

          	    
                        RealArray & f0 = idh.interfaceDataList[prev].f;  // here is the RHS a time tp
            // f(t) = (t-tp)/(t0-tp) *f(t0) + (t0-t)/(t0-tp) *f(tp)
                        real cex1 = dt==0. ? 1. : (t-tp)/dt;
          	    real cex2 = dt==0. ? 0. : (t0-t)/dt;
          	    if( true || debug() & 2 )
            	      fprintf(pDebugFile,"Cgins::interfaceRHS: Extrapolate traction in time: t=%9.3e t0=%9.3e, tp=%9.3e, cex1=%9.3e cex2=%9.3e\n",
                  		      t,t0,tp,cex1,cex2);
          	    
          	    if( !(parameters.dbase.get<bool >("twilightZoneFlow") && dt==0.) ) 
          	    {
            	      if( true ) 
            		f(I1,I2,I3,Ct)= cex1*traction(I1,I2,I3,D)+cex2*f0(I1,I2,I3,Ct);
            	      else
            	      {
            		printF(" EXTRAP traction-rate to first-order : *TEMP*\n");
            		f(I1,I2,I3,Ct)= traction(I1,I2,I3,D);
            	      }
            	      
          	    }
          	    else
          	    {
	      // this does not work exactly anyway since the grid is not at the new time!

              // special case: TZ and no previous values:
              // do this for now: set to exact 

                            printP("Cgins::interfaceRHS: **** Setting traction at t=%9.3e to exact for TZ ****\n",t);

                            OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                            const bool isRectangular = false; // ** do this for now ** mg.isRectangular();

            	      mg.update(MappedGrid::THEcenter);

            	      realArray & x= mg.center();
                            #ifdef USE_PPP
              	        realSerialArray xLocal; 
              	        if( !isRectangular ) 
                	  	  getLocalArrayWithGhostBoundaries(x,xLocal);
                            #else
              	        const realSerialArray & xLocal = x;
                            #endif

            	      const int pc=parameters.dbase.get<int >("pc");
                            const real & nu = parameters.dbase.get<real >("nu");
          	    
            	      realSerialArray pe(I1,I2,I3);
            	      e.gd( pe ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,pc,t);  // p exact solution 

            	      if( nu>0. )
            	      {
            		printP("interface:ERROR: nu>0 but TZ traction forcing does not include viscous terms. Finish me!\n");
            	      }
	      // The sign is correct here: normalForce = p*normal 
            	      for( int axis=0; axis<numberOfDimensions; axis++ )
            		f(I1,I2,I3,Ct.getBase()+axis) = pe(I1,I2,I3)*normal(I1,I2,I3,axis);           

          	    }
        	  }
        	  

        	  if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
        	  {
            // ---------------------------------------------------
            // --- return the time derivative of the traction: ---
            // ---------------------------------------------------
          	    if( dt==0. )
          	    {
                            dt=1.;   // do this for now -- time derivative is zero
          	    }
                        const int orderOfAccuracyTractionRate=2; // *****************************************************

          	    RealArray & f1 = idh.interfaceDataList[prev].f;         // here is the RHS a time tp
          	    
            // for 2nd-order accuracy we need two previous levels;
                        int im2=-1;
                        if( orderOfAccuracyTractionRate==2 )
          	    {
                            im2 = ( prev -1 + numberOfInterfaceHistoryValuesToSave ) %  numberOfInterfaceHistoryValuesToSave;
            	      if( idh.interfaceDataList[im2 ].t >= tp )
                                im2=-1;  // there is no past value before prev
          	    }

          	    if( orderOfAccuracyTractionRate==1 || im2==-1 )
          	    {

            	      if( debug() & 2 )
            		printP("Cgins::interfaceRHS: get d(traction)/dt (1st order): t=%9.3e, "
                   		       "gfIndex=%i gf[gfIndex].t=%9.3e, prev=%i, tp=%9.3e\n",t,gfIndex,gf[gfIndex].t,prev,tp);

            	      f(I1,I2,I3,Ctr)= (f(I1,I2,I3,Ct) - f1(I1,I2,I3,Ct))/dt;

            	      if( true )
            		::display(f(I1,I2,I3,Ctr),"traction-rate",pDebugFile,"%9.3e ");
            	      

          	    }
          	    else 
          	    {
              // Compute the traction-rate to second order accuracy 
                            assert( im2>=0 );

                            RealArray & f0 = f;                               // f(t0)
                            RealArray & f1 = idh.interfaceDataList[prev].f;   // f(t1)
                            RealArray & f2 = idh.interfaceDataList[im2 ].f;   // f(t2)

            	      const real t1 = idh.interfaceDataList[prev].t;
                            const real t2 = idh.interfaceDataList[im2 ].t;
            	      real dt0= t0-t1;
            	      real dt1= t1-t2;
            	      if( dt0<=0. || dt1<=0. )
            	      {
            		printP("Cgins::interfaceRHS:ERROR: computing traction rate: t0=%9.3e, prev=%i, tp=%9.3e, "
                                              "im2=%i, t2=%9.3e\n",t0,prev,tp,im2,t2);
            		OV_ABORT("error");
            	      }
            	      assert( dt0>0. && dt1>0. );

	      // Compute the time derivative of the Lagrange polynomial: 
	      //    f(t) = l0(t)*f0 + l1(t)*f1 + l2(t)*f2 
	      // where 
	      //   l0 = (t-t1)*(t-t2)/( (t0-t1)*(t0-t2) );
	      //   l1 = (t-t2)*(t-t0)/( (t1-t2)*(t1-t0) );
	      //   l2 = (t-t0)*(t-t1)/( (t2-t0)*(t2-t1) );
            
            	      real l0t = (2.*t-(t1+t2))/( (t0-t1)*(t0-t2) );
            	      real l1t = (2.*t-(t2+t0))/( (t1-t2)*(t1-t0) );
            	      real l2t = (2.*t-(t0+t1))/( (t2-t0)*(t2-t1) );
            
                            f(I1,I2,I3,Ctr)= l0t*f0(I1,I2,I3,Ct) +l1t*f1(I1,I2,I3,Ct) + l2t*f2(I1,I2,I3,Ct);

            	      if( true || debug() & 2 )
            	      {
            		printF(" EXTRAP traction-rate to first-order : *TEMP*\n");
                                real l1 = (t-t2)/(t1-t2);
            		real l2 = (t-t1)/(t2-t1);
                // f(I1,I2,I3,Ctr)= l1*f1(I1,I2,I3,Ctr) + l2*f2(I1,I2,I3,Ctr); // ******************** TEMP *************
                                f(I1,I2,I3,Ctr)= f1(I1,I2,I3,Ctr);

            		fprintf(pDebugFile,"Cgins::interfaceRHS: get d(traction)/dt (2nd order): t=%9.3e, t0=%9.3e, dt0=%9.3e "
                  			"gfIndex=%i gf[gfIndex].t=%9.3e, prev=%i, t1=%9.3e, im2=%i t2=%9.3e, "
                  			" l0t*dt0=%4.2f l1t*dt0=%4.2f l2t*dt0=%4.2f\n",
                  			t,t0,dt0,gfIndex,gf[gfIndex].t,prev,t1,im2,t2,l0t*dt0,l1t*dt0,l2t*dt0);   
                    		::display(f(I1,I2,I3,Ctr),"traction-rate",pDebugFile,"%9.3e ");
            		::display(f0(I1,I2,I3,Ct),"f0 traction (t0)",pDebugFile,"%9.3e ");
            		::display(f1(I1,I2,I3,Ct),"f1 traction (tp)",pDebugFile,"%9.3e ");
            		::display(f2(I1,I2,I3,Ct),"f2 traction (t2)",pDebugFile,"%9.3e ");

            		::display(f1(I1,I2,I3,Ctr),sPrintF("f1 traction-rate (t1=%9.3e)",t1),pDebugFile,"%9.3e ");
            		::display(f2(I1,I2,I3,Ctr),sPrintF("f2 traction-rate (t2=%9.3e)",t2),pDebugFile,"%9.3e ");

            	      }
          	    }
          	    
          	    
          	    if( debug() & 8 )
          	    {
            	      ::display(f1(I1,I2,I3,Ct),sPrintF("Cgins::interfaceRHS: old traction at tp=%9.3e",tp),"%8.2e ");
            	      ::display(f(I1,I2,I3,Ct),sPrintF("Cgins::interfaceRHS: new traction at t=%9.3e ",t),"%8.2e ");
            	      ::display(f(I1,I2,I3,Ctr),sPrintF("Cgins::interfaceRHS: time derivative of the traction, t=%9.3e",t),"%8.2e ");
          	    }
        	  }
        	  
      	}
      	else
      	{
        	  printP("Cgins::interfaceRightHandSide:traction: t=%9.3e There is NO time history. "
                                  "idh.current=%i, size=%i\n",t,idh.current,idh.interfaceDataList.size());

        	  if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
        	  {
          	    f(I1,I2,I3,Ct+numberOfDimensions)=0.;
        	  }
        	  
      	}
      	
            }
            


// #ifndef USE_PPP
//       f(I1,I2,I3,V)=traction(I1,I2,I3,D);
// #else
//       OV_ABORT("ERROR: finish me for parallel");
// #endif

            if( debug() & 8 )
            {
      	f(I1,I2,I3,Ct).display("Cgins::interfaceRightHandSide: Here is the RHS (traction=normalForce)");
            }
            
        }


    // *****************************************************************************
    // ******************** Traction Twilight Zone Forcing *************************
    // *****************************************************************************

        if( parameters.dbase.get<bool >("twilightZoneFlow") )
        {
      // ---add forcing for twlight-zone flow---

            OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

            const bool isRectangular = false; // ** do this for now ** mg.isRectangular();

            if( !isRectangular )
      	mg.update(MappedGrid::THEcenter);

            realArray & x= mg.center();
#ifdef USE_PPP
            realSerialArray xLocal; 
            if( !isRectangular ) 
      	getLocalArrayWithGhostBoundaries(x,xLocal);
#else
            const realSerialArray & xLocal = x;
#endif

            const int pc=parameters.dbase.get<int >("pc");
            const real & nu = parameters.dbase.get<real >("nu");

            if( option==setInterfaceRightHandSide )
            { // set 
	//   add on TZ flow:
	//   bd <- bd + (true boundary position)

        // printP("interface:ERROR: we need to include the true boundary position for TZ. Finish me!\n");
      	
            }
            else if( option==getInterfaceRightHandSide )
            { // get 
	//   subtract off TZ flow:
        //   f <- f - ( pe*normal )  ** should also include viscous terms **

                #ifdef USE_PPP
         	 const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                #else
         	 const realSerialArray & normal = mg.vertexBoundaryNormal(side,axis);
                #endif

          	    
      	realSerialArray pe(I1,I2,I3);
      	e.gd( pe ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,pc,t);  // p exact solution 

      	if( nu>0. )
      	{
        	  printP("interface:ERROR: nu>0 but TZ traction forcing does not include viscous terms. Finish me!\n");
      	}

        // ::display(f(I1,I2,I3,Ct),"Cgins::interface fluid traction before TZ fix");
      	

        // The sign is correct here: normalForce = p*normal 
                for( int axis=0; axis<numberOfDimensions; axis++ )
            	  f(I1,I2,I3,Ct.getBase()+axis) -= pe(I1,I2,I3)*normal(I1,I2,I3,axis);

        // ::display(f(I1,I2,I3,Ct),"Cgins::interface fluid traction after TZ fix");

      	if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
      	{
          // time derivative of the traction (store in pe)
        	  e.gd( pe ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,pc,t);  // p.t

        	  for( int axis=0; axis<numberOfDimensions; axis++ )
          	    f(I1,I2,I3,Ctr.getBase()+axis) -= pe(I1,I2,I3)*normal(I1,I2,I3,axis); 

      	}

            }
            else
            {
      	OV_ABORT("error");
            }
        
        } // end if TZ 



    }
    else
    {
        printF("Cgins::interfaceRightHandSide:unexpected interfaceType=%i\n",interfaceType(side,axis,grid));
        OV_ABORT("error");
    }
    

    return 0;
}
