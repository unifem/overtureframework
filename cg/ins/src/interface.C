// This file automatically generated from interface.bC with bpp.
//
//  -- 2012/05/31 -- version taken from Cgcns (to support FSI)
//  -- We should really share this code
//
#include "Cgins.h"
#include "InsParameters.h"
#include "Interface.h"  
#include "ParallelUtility.h"
#include "ArrayEvolution.h"       

// include some interface bpp macros
//------------------------------------------------------------------------------------
// This file contains macros used by the interface routines.
//------------------------------------------------------------------------------------



// ===========================================================================
// Get/set the interface RHS for a heat flux interface
// ===========================================================================

// forward declaration
int ovmod (int a, int b);  // *fix me*


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
    // projectInterface : not sure if this is currently used
        const bool projectInterface = parameters.dbase.get<bool>("projectInterface");

    // useAddedMassAlgorithm=true : coupling to an elastic solid
        const bool useAddedMassAlgorithm = parameters.dbase.get<bool>("useAddedMassAlgorithm");

    // Boundary condition on the interface: 
    //   Use this to decide if the boundary is a velocity BC or a traction BC.
        const int bc = gf[0].cg[grid].boundaryCondition(side,axis);
        
        if( debug() & 2 )
            printP("*** Cgins:getInterfaceDataOptions: projectInterface = %i ***\n",projectInterface);

        if( !projectInterface && !useAddedMassAlgorithm )
        {
      // -- no added-mass algorithm or interface projection-----

            if( bc==InsParameters::noSlipWall ||
        	  bc==InsParameters::slipWall )
            {
	// For a velocity boundary we need the position of the interface from the
        //    opposite domain (traditional scheme)
      	interfaceDataOptions=Parameters::positionInterfaceData     |
                       			     Parameters::velocityInterfaceData     |
                       			     Parameters::accelerationInterfaceData;
      	numberOfItems+= 3*numberOfDimensions;
            }
            else if( bc==InsParameters::freeSurfaceBoundaryCondition || 
                              bc==InsParameters::tractionFree )
            {
        // For a traction boundary (or "freeSurface") we need the traction from the
        //    opposite domain (traditional scheme)

      	if( TRUE )
        	  printP("*** Cgins:getInterfaceDataOptions: (grid,side,axis)=(%i,%i,%i) bc=%i (traction or freeSurface)\n",grid,side,axis,bc);

      	interfaceDataOptions=Parameters::tractionInterfaceData;
      	numberOfItems+=numberOfDimensions;
            }
            else
            {
      	printP(" --Cgins-- getInterfaceDataOptions: ERROR: unexpected BC=%i for a traction interface\n",bc);
            }
            
        }
        else if( useAddedMassAlgorithm )
        {
      // *wdh* Nov. 20, 2016
      // For the addedMass algorithm (coupling INS to an elastic solid) we need the following interface data: 
      //  - position  ?
      //  - velocity
      //  - traction 
      //  - acceleration 
      //  - traction-rate  : d(traction)/dt  ?

            if( true )
      	printP("*** Cgins:getInterfaceDataOptions: (grid,side,axis)=(%i,%i,%i) bc=%i useAddedMassAlgorithm=%i\n"
                              "       ---> request position,velocity,acceleration, traction from the solid\n"
             	       ,grid,side,axis,bc,(int)useAddedMassAlgorithm);
            if( true )
            {
	// do this for now:
      	interfaceDataOptions=Parameters::positionInterfaceData     |
                       			     Parameters::velocityInterfaceData     |
                       			     Parameters::tractionInterfaceData     |
                       			     Parameters::accelerationInterfaceData;
      	numberOfItems+=4*numberOfDimensions;
            }
            else if( true )
            {
	// do this for now:
      	interfaceDataOptions=Parameters::positionInterfaceData     |
                       			     Parameters::velocityInterfaceData;
      	numberOfItems+=2*numberOfDimensions;
            }
            else
            {
      	interfaceDataOptions = ( Parameters::positionInterfaceData     |
                         				 Parameters::velocityInterfaceData     |
                         				 Parameters::tractionInterfaceData     |
                         				 Parameters::accelerationInterfaceData
        	  );

      	numberOfItems+=4*numberOfDimensions;
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

// =======================================================================================================
// Macro: compute the traction by extrapolation of nearby values in time (extrapolation or interpolation)
// =======================================================================================================

// ==================================================================================================
// Macro: compute the traction-rate by differencing traction values in time
// ==================================================================================================


// =============================================================================================
// Macro: Compute the traction or traction rate for TZ flow
//   NOTE : THIS ROUTINE RETURNS MINUS the fluid traction --> this is the force on the opposite side
//  traction  (output) 
//  ntd (input) : number of time derivatives, use 0 for traction, 1 for traction rate
// =============================================================================================


// =============================================================================================
// Macro: Compute the traction-rate using the interace data history *old way*
// =============================================================================================

// =================================================================================================
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
/// \param saveTimeHistory (input) : if true, save a time-history of the requested data. This is the
///    new way to save a time-history when interfaceCommunicationMode==requestInterfaceDataWhenNeeded
// ===================================================================================================
int
Cgins::
interfaceRightHandSide( InterfaceOptionsEnum option, 
                                                int interfaceDataOptions,
                                                GridFaceDescriptor & info, 
                                                GridFaceDescriptor & gfd, 
                  			int gfIndex, real t,
                                                bool saveTimeHistory /* = false */ )
{
  // return DomainSolver::interfaceRightHandSide(option,interfaceDataOptions,info,gfIndex,t);
  // *wdh* 081212 CompositeGrid & cg = gf[0].cg;
  // CompositeGrid & cg = gf[0].cg;
    FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

    if( gfIndex==-1 )
    {
    // Find the solution that matches time=t
        const int & currentGF = parameters.dbase.get<int>("currentGF");
        const int & nextGF    = parameters.dbase.get<int>("nextGF");

        assert( current>=0 );
        if( gf[current].t == t )
        {
            gfIndex=current;
        }
        else if( currentGF<0 )   // do this for now
        {
            gfIndex=current;
            printF("Cgsm: interfaceRightHandSide:WARNING cannot find gfIndex to match t=%9.3e, using current...\n",t);
        }
        else
        {
        
            if( !(currentGF>=0 && nextGF>=0) )
            {
      	printF("Cgsm: interfaceRightHandSide:ERROR: t=%9.2e, current=%i gf[current].t=%9.2e, currentGF=%i, nextGF=%i\n",
             	       t,current,gf[current].t,currentGF,nextGF);
      	OV_ABORT("FIX ME");
            }

            if( gf[currentGF].t == t )
            {
      	gfIndex=currentGF;
            }
            else if( gf[nextGF].t == t )
            {
      	gfIndex=nextGF;
            }
            else 
            {
	// ************** FIX ME ************
      	printF("Cgsm: interfaceRightHandSide:WARNING cannot find gfIndex to match t=%9.3e\n"
             	       "      currentGF=%i, gf[currentGF].t=%9.3e, nextGF=%i, gf[nextGF].t=%9.3e\n",
             	       t,currentGF,gf[currentGF].t,nextGF,gf[nextGF].t);
      	if( fabs(gf[currentGF].t-t) <  fabs(gf[nextGF].t-t) )
        	  gfIndex=currentGF; 
      	else
        	  gfIndex=nextGF; 
	// OV_ABORT("fix me");
            }
        }
        
    }

    const Parameters::InterfaceCommunicationModeEnum & interfaceCommunicationMode= 
        parameters.dbase.get<Parameters::InterfaceCommunicationModeEnum>("interfaceCommunicationMode");

    CompositeGrid & cg = gf[gfIndex].cg;
    const int numberOfDimensions = cg.numberOfDimensions();
    const bool & twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
    const Parameters::KnownSolutionsEnum & knownSolution = 
        parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution"); 


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

      // this did not work:
      // if( FALSE && interfaceDataOptions & Parameters::tractionInterfaceData )
      // {
      // 	// make sure there is space for the traction
      // 	if( bd.getLength(3)< 2*numberOfDimensions+1 )
      // 	{
      // 	  int numComponents=2*numberOfDimensions+1;
      // 	  bd.redim(bd.dimension(0),bd.dimension(1),bd.dimension(2),Range(0,numComponents-1));
      // 	  // bd.redim(bd.dimension(0),bd.dimension(1),bd.dimension(2),numberOfDimensions+1);

      // 	}
      	
      // }
            

            bd=0.;  // boundary data is filled into this array


            if( interfaceDataOptions & Parameters::positionInterfaceData )
            {
        // --- interface position is given ---
      	C=Range(numSaved,numSaved+numberOfDimensions-1);
      	if( debug() & 2 )
        	  printP("Cgins::interfaceRHS: Set interface positions at t=%9.3e in bd components V=[%i,%i] from C=[%i,%i]\n",
             		 gf[gfIndex].t,V.getBase(),V.getBound(),C.getBase(),C.getBound());

                bool useExactInterfacePosition=FALSE;
                if( useExactInterfacePosition && knownSolution==Parameters::userDefinedKnownSolution ) // ****** TESTING 
                {
                    int body=0;
          // Range Rx=numberOfDimensions;
          // RealArray state(I1,I2,I3,Rx);
                    parameters.getUserDefinedDeformingBodyKnownSolution( body,Parameters::boundaryPosition,
                                                                                                                              t, grid, mg, I1,I2,I3,V,bd );
                    
          // bd(I1,I2,I3,V)=state(I1,I2,I3,Rx);
            
                    printF(" ********** TEST: SET INTERFACE POSITION TO EXACT : t=%9.3e *TEMP* *************\n",t);
                    ::display(bd(I1,I2,I3,V),sPrintF("interface exact t=%9.3e, dt=%9.3e",t,dt),"%9.3e ");
                    ::display(f(I1,I2,I3,C),sPrintF("position computed (from cgsm) t=%9.3e",t),"%9.3e ");

                }
                else
                {
        	  bd(I1,I2,I3,V)=f(I1,I2,I3,C);  // set positions of interface -- fill into velocity components for now ***
                }
                
      	numSaved+=numberOfDimensions;
            }
            
            if( interfaceDataOptions & Parameters::velocityInterfaceData )
            {
        // --- interface velocity is given ---
      	C=Range(numSaved,numSaved+numberOfDimensions-1);

                if( true || debug() & 4 )
      	{
        	  printP(">>> --INS-- interfaceRHS: interface velocity provided t=%9.3e  in C=[%i,%i]<<<\n",
                                  t,C.getBase(),C.getBound());
      	}
	// -- for now save the velocity data in the dbase *wdh* June 12, 2017
                aString velocityDataName;
      	sPrintF(velocityDataName,"velocityG%iS%iA%i",grid,side,axis);
      	if( !parameters.dbase.has_key(velocityDataName) )
      	{
        	  InterfaceData & interfaceData = parameters.dbase.put<InterfaceData>(velocityDataName);
        	  interfaceData.u.redim(bd.dimension(0),bd.dimension(1),bd.dimension(2),numberOfDimensions);
        	  interfaceData.u=0;
      	}
      	InterfaceData & interfaceData = parameters.dbase.get<InterfaceData>(velocityDataName);
      	interfaceData.t=t;
      	Range Rx=numberOfDimensions;
      	interfaceData.u(I1,I2,I3,Rx)=f(I1,I2,I3,C);  

                numSaved+=numberOfDimensions;
            }

            if( interfaceDataOptions & Parameters::accelerationInterfaceData )
            {
        // --- interface acceleration is given ---
      	C=Range(numSaved,numSaved+numberOfDimensions-1);

                if( true || debug() & 4 )
      	{
        	  printP(">>> --INS-- interfaceRHS: interface acceleration provided t=%9.3e  in C=[%i,%i]<<<\n",
                                  t,C.getBase(),C.getBound());
      	}
	// -- for now save the acceleration data in the dbase *wdh* June 12, 2017
                aString accelerationDataName;
      	sPrintF(accelerationDataName,"accelerationG%iS%iA%i",grid,side,axis);
      	if( !parameters.dbase.has_key(accelerationDataName) )
      	{
        	  InterfaceData & interfaceData = parameters.dbase.put<InterfaceData>(accelerationDataName);
        	  interfaceData.u.redim(bd.dimension(0),bd.dimension(1),bd.dimension(2),numberOfDimensions);
        	  interfaceData.u=0;
      	}
      	InterfaceData & interfaceData = parameters.dbase.get<InterfaceData>(accelerationDataName);
      	interfaceData.t=t;
      	Range Rx=numberOfDimensions;
      	interfaceData.u(I1,I2,I3,Rx)=f(I1,I2,I3,C);  

        // the interface acceleration is currently not used.
	// bd(I1,I2,I3,Dc)=f(I1,I2,I3,C);    // where should we put this?

                numSaved+=numberOfDimensions;
            }

            if( interfaceDataOptions & Parameters::tractionInterfaceData )
            {
        // --- interface traction is given ---
      	C=Range(numSaved,numSaved+numberOfDimensions-1);
                if( true || debug() & 4 )
      	{
        	  printP(">>> --INS-- interfaceRHS: interface traction provided t=%9.3e  in C=[%i,%i]<<<\n",
                                  t,C.getBase(),C.getBound());
                    printF(" #### bd array component dimensions: [base3,bound3]=[%i,%i] ####\n",bd.getBase(3),bd.getBound(3));
      	}
      	
	// -- for now save the traction data in the dbase
                aString tractionDataName;
      	sPrintF(tractionDataName,"tractionG%iS%iA%i",grid,side,axis);
      	if( !parameters.dbase.has_key(tractionDataName) )
      	{
        	  InterfaceData & interfaceData = parameters.dbase.put<InterfaceData>(tractionDataName);
        	  interfaceData.u.redim(bd.dimension(0),bd.dimension(1),bd.dimension(2),numberOfDimensions);
        	  interfaceData.u=0;
      	}
      	InterfaceData & interfaceData = parameters.dbase.get<InterfaceData>(tractionDataName);
      	interfaceData.t=t;
      	Range Rx=numberOfDimensions;
      	interfaceData.u(I1,I2,I3,Rx)=f(I1,I2,I3,C);  

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

      // -------------------For TESTING return exact values (if known) --------------------
            bool useExactInterfaceValues=FALSE;  

            if( useExactInterfaceValues )
            {
                printF("--INS--IRHS: USING EXACT INTERFACE VALUES t=%9.3e ***TEMP***\n",t);
            }

      // We could optimize this for rectangular grids 
            mg.update(MappedGrid::THEvertexBoundaryNormal);
            OV_GET_VERTEX_BOUNDARY_NORMAL(mg,side,axis,normal);

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

                if( knownSolution==Parameters::userDefinedKnownSolution )
                {
                    int body=0;
          // RealArray state(I1,I2,I3,Rx);
                    parameters.getUserDefinedDeformingBodyKnownSolution( body,Parameters::boundaryVelocity,
                                                                                                                              t, grid, mg, I1,I2,I3,C,f );
                }
                else
                {
                    f(I1,I2,I3,C) = uLocal(I1,I2,I3,V);
                }
                
      	numSaved+=numberOfDimensions;
            }
            if( interfaceDataOptions & Parameters::accelerationInterfaceData )
            {
	// -- return the ACCELERATION of the boundary --
      	if( debug() & 4 )
        	  printP("Cgins:interfaceRightHandSide: Save the interface acceleration.\n");

      	Range Rx(0,numberOfDimensions-1);                // vertex components 
      	C=Range(numSaved,numSaved+numberOfDimensions-1); // save displacement in these components of f

      	printP("Cgins:interfaceRightHandSide: save acceleration - FINISH ME!\n");
                OV_ABORT("error");
      	
      	numSaved+=numberOfDimensions;
            }

      // -- Now save the traction and traction rate --

            Index Ib1,Ib2,Ib3;
      // getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
            Ib1=I1, Ib2=I2, Ib3=I3;
            realSerialArray traction(Ib1,Ib2,Ib3,numberOfDimensions);  // this should be a serial array

      // gf[gfIndex].conservativeToPrimitive();

      // Here is the time we have actually computed the traction at. It may be less than t on the predictor step
            const real t0 = gf[gfIndex].t;  

            int ipar[] = {grid,side,axis,gf[gfIndex].form}; // 
            real rpar[] = { gf[gfIndex].t }; // 

      //  getNormalForce: This is the force on the adjacent body 
      //      This is MINUS the fluid traction (using the outward fluid normal)
            parameters.getNormalForce( gf[gfIndex].u,traction,ipar,rpar );

            Range D=numberOfDimensions;
            Ct=Range(numSaved,numSaved+numberOfDimensions-1);  // save interface traction in these components
            
            if( debug() & 4 )
            {
                printP("Cgins::interfaceRHS: Get normal force (traction) at t=%9.3e\n",gf[gfIndex].t);
      	::display(traction(I1,I2,I3,D),sPrintF("--INS--IRHS traction from getNormalForce t=%8.2e",gf[gfIndex].t),"%5.2f ");
      	
            }

            if( twilightZoneFlow )
            {
        // subtract off TZ traction
                Range Rx=numberOfDimensions;
                RealArray tractionTZ(I1,I2,I3,Rx);
                int ntd=0;  // number of time derivatives
                {
                    OV_GET_SERIAL_ARRAY(real,mg.vertex(),xLocal);
                    OV_GET_VERTEX_BOUNDARY_NORMAL(mg,side,axis,normal);
                    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                    const int pc=parameters.dbase.get<int >("pc");
                    const int uc=parameters.dbase.get<int >("uc");
                    const int vc=parameters.dbase.get<int >("vc");
                    const int wc=parameters.dbase.get<int >("wc");
                    const real & nu = parameters.dbase.get<real >("nu");
                    const real & fluidDensity = parameters.dbase.get<real>("fluidDensity");
                    assert( fluidDensity>0. );
                    const real mu = nu*fluidDensity; 
                    Range V(uc,uc+numberOfDimensions-1); // velocity components
                    realSerialArray pe(I1,I2,I3),uxe(I1,I2,I3,V),uye(I1,I2,I3,V),uze;
                    bool isRectangular=false;
                    e.gd( pe ,xLocal,numberOfDimensions,isRectangular,ntd,0,0,0,I1,I2,I3,pc,t0);  // p exact solution 
                    e.gd( uxe,xLocal,numberOfDimensions,isRectangular,ntd,1,0,0,I1,I2,I3,V,t0);  // v.x
                    e.gd( uye,xLocal,numberOfDimensions,isRectangular,ntd,0,1,0,I1,I2,I3,V,t0);  // v.y
                    if( numberOfDimensions==3 )
                    {
                        uze.redim(I1,I2,I3,V);
                        e.gd( uze,xLocal,numberOfDimensions,isRectangular,ntd,0,0,1,I1,I2,I3,V,t0);  // v.z
                    }
          // The sign is correct here I think: normalForce = sigma.normal = (-pI + tauv)*normal 
                    if( numberOfDimensions==2 )
                    {
                        int axis=0;
                        tractionTZ(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,axis)
                                                                                              -mu*( (uxe(I1,I2,I3,uc)+uxe(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                          (uye(I1,I2,I3,uc)+uxe(I1,I2,I3,vc))*normal(I1,I2,I3,1)) );
                        axis=1;
                        tractionTZ(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,axis)
                                                                                              -mu*( (uxe(I1,I2,I3,vc)+uye(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                          (uye(I1,I2,I3,vc)+uye(I1,I2,I3,vc))*normal(I1,I2,I3,1)) );
                    }
                    else
                    {
                        int axis=0;
                        tractionTZ(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,0)
                                                                                              -(mu*((uxe(I1,I2,I3,uc)+uxe(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                          (uye(I1,I2,I3,uc)+uxe(I1,I2,I3,vc))*normal(I1,I2,I3,1)+ 
                                                                                                          (uze(I1,I2,I3,uc)+uxe(I1,I2,I3,wc))*normal(I1,I2,I3,2)) ) );
                        axis=1;
                        tractionTZ(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,1)
                                                                                              -(mu*((uxe(I1,I2,I3,vc)+uye(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                          (uye(I1,I2,I3,vc)+uye(I1,I2,I3,vc))*normal(I1,I2,I3,1)+ 
                                                                                                          (uze(I1,I2,I3,vc)+uye(I1,I2,I3,wc))*normal(I1,I2,I3,2)) ) );
                        axis=2;
                        tractionTZ(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,2)
                                                                                              -(mu*((uxe(I1,I2,I3,wc)+uze(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                          (uye(I1,I2,I3,wc)+uze(I1,I2,I3,vc))*normal(I1,I2,I3,1)+ 
                                                                                                          (uze(I1,I2,I3,wc)+uze(I1,I2,I3,wc))*normal(I1,I2,I3,2)) ) );
                    }
                }
                if( true )
                {
                    printF("--INS--IRHS: subtract off TZ traction from computed traction at t0=%8.2e\n",t0);
                    ::display(tractionTZ,sPrintF("--INS-- Traction from TZ (minus normal force) at t0=%8.2e",t0),"%8.2e ");
                }
                
                traction(I1,I2,I3,D) -= tractionTZ(I1,I2,I3,Rx);
            }
            
            if( interfaceDataOptions & Parameters::tractionInterfaceData )
            {
      	if( debug() & 2 )
        	  printP("Cgins:interfaceRightHandSide: Eval the interface traction at t=%8.2e in components Ct=[%i,%i].\n",t,Ct.getBase(),Ct.getBound());


                bool useExactTraction=useExactInterfaceValues;
                if( useExactTraction && knownSolution==Parameters::userDefinedKnownSolution ) // ********* TESTING 
                {
                    int body=0;
          // RealArray state(I1,I2,I3,Rx);
                    parameters.getUserDefinedDeformingBodyKnownSolution( body,Parameters::boundaryTraction,
                                                                                                                              t, grid, mg, I1,I2,I3,Ct,f );
                    traction(I1,I2,I3,D)=f(I1,I2,I3,Ct);
            
                    printF(" ********** TEST: SET traction to EXACT : t=%9.3e *TEMP* *************\n",t);
                    ::display(f(I1,I2,I3,Ct),sPrintF("traction EXACT t=%9.3e, dt=%9.3e",t,dt),"%9.3e ");
                    ::display(traction(I1,I2,I3,D),sPrintF("traction Computed t=%9.3e, dt=%9.3e",t,dt),"%9.3e ");

                }
                else
                {
                    f(I1,I2,I3,Ct)=traction(I1,I2,I3,D);
                }
                    
      	if( debug() & 4  )
      	{
        	  printF("--INS--IRHS: interface data at t=%8.2e, traction from gfIndex=%i, gf[gfIndex].t=%9.3e--\n",
                                    t,gfIndex, gf[gfIndex].t);
	  // ::display(gf[gfIndex].u[grid](I1,I2,I3,V),"--INS--IRHS:  velocity on the boundary","%9.3e ");
        	  ::display(f(I1,I2,I3,Ct),sPrintF("--INS--IRHS:  traction f, Ct=[%i,%i]",Ct.getBase(),Ct.getBound()),"%9.3e ");

        	  fprintf(pDebugFile,"-- interfaceRHS: interface data at t=%8.2e --\n",t);
        	  ::display(f(I1,I2,I3,Ct),"--INS-- traction f",pDebugFile,"%9.3e ");
      	}
      	
        // *new* way June 26, 2017 
                if( saveTimeHistory )
                {
                    if( interfaceDataOptions & Parameters::tractionInterfaceData )
                    {
            // -- save a time history of the traction
                        if( !gfd.dbase.has_key("tractionHistory") )
                        {
                            gfd.dbase.put<ArrayEvolution>("tractionHistory");
                        }
                        ArrayEvolution & tractionHistory = gfd.dbase.get<ArrayEvolution>("tractionHistory");

            // NOTE: array data may hold more than just the traction! **FIX ME**
                        printF("--IRHS-- Save traction time history at t=%9.3e\n",t);
                        tractionHistory.add( t, traction);  
                    }
        
                }

                numSaved+=numberOfDimensions;
            }
            

      // -- check if we need the traction at a different time than the current solution --
      // (this could be a predictor-step for e.g.)

            bool tractionTimeDiffers = fabs(t-t0) > 100.*REAL_EPSILON;
      // -- this next section was re-worked *wdh* June 11, 2017
            if( tractionTimeDiffers )
            {
        // --------------------------------------------------------------------------------------
        //  Compute the traction at a different time than the current time
        //     t = time required
        //     t0 = time requested
        // --------------------------------------------------------------------------------------
        // A time history of interface values is saved here in the master list gfd: 
                InterfaceDataHistory & idh = gfd.interfaceDataHistory;
                InterfaceDataHistory & idi = gfd.interfaceDataIterates; // iterates of interface values from the predictor corrector 
                if( idh.current>=0 && idh.current<idh.interfaceDataList.size() )
      	{
                    const int numberOfInterfaceHistoryValuesToSave=idh.interfaceDataList.size();
        	  
	  // find a previous time value we can use
                    int prev = idh.current;  // by default use this as the old solution
        	  real tp = idh.interfaceDataList[prev].t;
                    if( fabs(tp-t0)< REAL_EPSILON*100.*(1 + fabs(t)) )
                    {
            // New traction computed at t0, but tPrev=t0 -- find an even previouser 
                        int prev2 = ovmod(prev -1,numberOfInterfaceHistoryValuesToSave);
                        real tPrev2 = idh.interfaceDataList[prev2].t;
                        if(  tPrev2 >  tp ) // if tPrev2 > tp then we have no previous values
                        {
                            prev=-1;  // this means there is no previous value 
                        }
                        else
                        {
                            prev=prev2;
                            tp=tPrev2;
                        }
                        
                    }

                    dt = t - tp;
                    assert( dt> REAL_EPSILON*100.*(1 + fabs(t)) );
          // Get the traction by extrapolation from  traction(I1,I2,I3,D) and values at prev         

            // --- We need the traction at a time that differs from the current solution time  ---
            // We can use the time history values and extrap/interp in time
            // If there are no previous time history values we fill some appropriate values in.
            //   -- for TZ : evaluate the exact solution
            //   -- for real : just assume constant ? 
                        if( !(twilightZoneFlow && dt==0.) ) 
                        {
                            const real dtp = t0-tp;
                            printF("\n ---INS-- >>>>>>>>>>> getTractionFromNearbyValues t0=%9.3e tp=%9.3e<<<<<<<<<<< \n",t0,tp);
                            if( dtp==0. && interfaceDataOptions & Parameters::tractionRateInterfaceData )
                            {
                // We have traction and traction rate at a previous time 
                                int prev = idh.current;
                                real tp = idh.interfaceDataList[prev].t;
                                real dtp= t-tp;
                                assert( dtp>0. );
                                RealArray & f0 = idh.interfaceDataList[prev].f;  // here is the RHS a time tp
                                Ctr=Ct+numberOfDimensions;
                                f(I1,I2,I3,Ct)= traction(I1,I2,I3,D) + dtp*f0(I1,I2,I3,Ctr);
                                if( debug() & 2 )
                                {
                                    printF("   >>> PREDICT traction in time at t=%8.2e using traction and traction-rate from tp=%9.3e (prev=%i)\n",
                                                  t,tp,prev);
                                    ::display(f(I1,I2,I3,Ct),sPrintF(" predicted traction for t=%8.e2",t),"%8.2e ");
                                }
                            }
                            else if( prev>=0 && dtp!=0. )
                            {
                                RealArray & f0 = idh.interfaceDataList[prev].f;  // here is the RHS a time tp
                // f(t) = (t-tp)/(t0-tp) *f(t0) + (t0-t)/(t0-tp) *f(tp)
                                assert( dtp>0. );
                                const real cex1 = (t-tp)/dtp;
                                const real cex2 = (t0-t)/dtp;
                                if( true || debug() & 2 )
                                {
                                    printF("--INS--IRHS- EXTRAPOLATE traction in time: t=%9.3e t0=%9.3e, tp=%9.3e (prev=%i), "
                                                  "cex1=%9.3e cex2=%9.3e\n",t,t0,tp,prev,cex1,cex2);
                                    fprintf(pDebugFile,"Cgins::interfaceRHS: Extrapolate traction in time: t=%9.3e t0=%9.3e, tp=%9.3e, "
                                                    "cex1=%9.3e cex2=%9.3e\n",t,t0,tp,cex1,cex2);
                                }
                                if( TRUE )
                                {
                                    printF("--INS--IRHS- EXTRAPOLATE TRACTION TO SECOND ORDER \n");
                                    f(I1,I2,I3,Ct)= cex1*traction(I1,I2,I3,D)+cex2*f0(I1,I2,I3,Ct);
                                }
                                else
                                {
                                    printF("--INS--IRHS- EXTRAPOLATE TRACTION TO FIRST ORDER  *** TEMP**** \n");
                                    f(I1,I2,I3,Ct)= traction(I1,I2,I3,D);
                                }
                            }
                            else
                            {
                                printF("--INS--IRHS- EXTRAPOLATE TRACTION TO FIRST ORDER t=%9.3e, t0=%9.3e**** CHECK ME**** \n",t,t0);
                                f(I1,I2,I3,Ct)= traction(I1,I2,I3,D);
                            }
                        }
                        else  // twilightzone and dt==0 : 
                        {
                            assert( twilightZoneFlow );
                            assert( t==0. );
              // this does not work exactly anyway since the grid is not at the new time!
              // special case: TZ and no previous values:
              // do this for now: set to exact 
                            printP("--INS--IRHS **** Setting traction at t=%9.3e to exact for TZ ****\n",t);
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
                                printP("--INS-- interface:ERROR: nu>0 but TZ traction forcing does not include viscous terms. Finish me!\n");
                                OV_ABORT("error");
                            }
              // The sign is correct here: normalForce = p*normal 
                            for( int axis=0; axis<numberOfDimensions; axis++ )
                                f(I1,I2,I3,Ct.getBase()+axis) = pe(I1,I2,I3)*normal(I1,I2,I3,axis);           
                    }

                }
                

            }
            
      // --------------------------------------------------------------------------------------
      //  Compute the traction rate from the time history
      //
      // --------------------------------------------------------------------------------------
      // -- this next section was re-worked *wdh* June 11, 2017
            if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
            {
	// -- save the interface traction rate --
      	if( t<= 2.*dt || debug() & 4 )
        	  printP("--INS--interfaceRightHandSide: EVAL the interface traction-rate, t=%9.3e.\n",t);

                C=Range(numSaved,numSaved+numberOfDimensions-1);
                Ctr=C;


                if( interfaceCommunicationMode==Parameters::requestInterfaceDataWhenNeeded )
                {
          // --- evaluate the traction-rate from the traction time-history ---
          // *new* way June 26, 2017
                    Range Rx=numberOfDimensions;
                    RealArray tractionRate(I1,I2,I3,Rx);

                    if( gfd.dbase.has_key("tractionHistory") )
                    {
                        ArrayEvolution & tractionHistory = gfd.dbase.get<ArrayEvolution>("tractionHistory");
                        if( tractionHistory.getNumberOfTimeLevels() >1 )
                        {
                            const int numberOfTimeDerivatives=1; // eval 1 time derivative of the traction
                            const int orderOfAccuracyForTractionRate=2; // 
                            tractionHistory.eval(t,tractionRate,numberOfTimeDerivatives,orderOfAccuracyForTractionRate);
                        }
                        else
                        {
              // If there are not enough time levels this could be first step
                            if( twilightZoneFlow )
                            {
                                Range Rx=numberOfDimensions;
                                RealArray tractionRate(I1,I2,I3,Rx);
                // For TZ "exact traction-rate" = tractionRateTZ - tractionRateTZ = 0 
                                tractionRate=0.;
                                printF(" ********** SET traction-rate to EXACT: t=%9.3e (For TZ)*************\n",t);              
                            }
                            else if(  knownSolution==Parameters::userDefinedKnownSolution && (t<=0. || dt==0.) )
                            {

                                int body=0;
                // RealArray state(I1,I2,I3,Rx);
                                parameters.getUserDefinedDeformingBodyKnownSolution( body,Parameters::boundaryTractionRate,
                                                                                                                                          t, grid, mg, I1,I2,I3,Rx,tractionRate );
            
                                printF(" ********** SET traction-rate to EXACT: t=%9.3e (No time history)*************\n",t);
                                ::display(tractionRate,sPrintF("traction-rate EXACT t=%9.3e, dt=%9.3e",t,dt),"%9.3e ");
            		
                            }
                            else
                            {
                                printF("\n--INS-IRHS: t=%9.3e **WARNING** Not enough time-history levels"
                                              "of the traction to compute the traction rate. Setting traction-rate=0\n\n",
                                              t);
                                tractionRate=0.;
                            }
          	    
                        }
                    }
                    else
                    {
                        OV_ABORT("--INS---IRHS--Error: there is no tractionHistory to compute traction-rate");
                    }

                    f(I1,I2,I3,Ctr)=tractionRate;

                }
                else
                {
          // old way 
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
                            dt = t -tp;
                            real dt0=t0-tp;  
                            if( fabs(dt0)<= REAL_EPSILON*1000. )
                            {
                // if tp is the same as t0, look for an earlier time: 
                                if( numberOfInterfaceHistoryValuesToSave>1 )
                                {
                                    prev = ( prev -1 + numberOfInterfaceHistoryValuesToSave ) %  numberOfInterfaceHistoryValuesToSave;
                                    tp = idh.interfaceDataList[prev].t;
                                    dt0=t0-tp;
                                }
                                else
                                {
                                    printP("Cgins::interfaceRHS:WARNING: there are no previous history values to compute tractionRate.\n"
                                                  "  The only history value is at time tp=%9.3e, but gf[gfIndex=%i].t=%9.3e\n",tp,gfIndex,t0);
                                    dt0=0.; 
                                }
                            }
              // ---------------------------------------------------
              // --- return the time derivative of the traction: ---
              // ---------------------------------------------------
                                if( true || t <= 2.*dt || dt==0. )
                                {
                                    printF("\n--INS-- interfaceRHS: EVAL traction-rate at t=%9.3e t0=%9.2e, dt=%9.3e, cur=%i prev=%i t[prev]=%9.3e\n,",
                                                  t,gf[gfIndex].t,dt,idh.current,prev,idh.interfaceDataList[prev].t);
                                }
                                const int orderOfAccuracyTractionRate=2; // *****************************************************
                // for 2nd-order accuracy we need two previous levels;
                                int im2=-1;
                                if( orderOfAccuracyTractionRate==2 )
                                {
                                    im2 = ( prev -1 + numberOfInterfaceHistoryValuesToSave ) %  numberOfInterfaceHistoryValuesToSave;
                                    if( idh.interfaceDataList[im2 ].t >= tp )
                                        im2=-1;  // there is no past value before prev
                                }
                                RealArray tractionRate;
                                const bool evalExactTractionRate=true;
                                const bool useExactTractionRate=useExactInterfaceValues && !twilightZoneFlow;
                                if( evalExactTractionRate )  // ********* TESTING
                                {
                                    tractionRate.redim(I1,I2,I3,Rx);
                                    if( knownSolution==Parameters::userDefinedKnownSolution )
                                    {
                                        int body=0;
                    // RealArray state(I1,I2,I3,Rx);
                                        parameters.getUserDefinedDeformingBodyKnownSolution( body,Parameters::boundaryTractionRate,
                                                                                                                                                  t, grid, mg, I1,I2,I3,Rx,tractionRate );
                    // parameters.getUserDefinedDeformingBodyKnownSolution( body,Parameters::boundaryTractionRate,
                    //                                                      t, grid, mg, I1,I2,I3,Ctr,f );
                                    }
                                    else if( twilightZoneFlow )
                                    {
                                        if( true )
                                        {
                      // for TZ flow the "exact" tractionRate = tractionRateTZ - tractionRateTZ = 0 
                                            tractionRate=0.;
                                        }
                                        else
                                        {
                                            int ntd=1;// number of time derivatives
                                            {
                                                OV_GET_SERIAL_ARRAY(real,mg.vertex(),xLocal);
                                                OV_GET_VERTEX_BOUNDARY_NORMAL(mg,side,axis,normal);
                                                OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                                                const int pc=parameters.dbase.get<int >("pc");
                                                const int uc=parameters.dbase.get<int >("uc");
                                                const int vc=parameters.dbase.get<int >("vc");
                                                const int wc=parameters.dbase.get<int >("wc");
                                                const real & nu = parameters.dbase.get<real >("nu");
                                                const real & fluidDensity = parameters.dbase.get<real>("fluidDensity");
                                                assert( fluidDensity>0. );
                                                const real mu = nu*fluidDensity; 
                                                Range V(uc,uc+numberOfDimensions-1); // velocity components
                                                realSerialArray pe(I1,I2,I3),uxe(I1,I2,I3,V),uye(I1,I2,I3,V),uze;
                                                bool isRectangular=false;
                                                e.gd( pe ,xLocal,numberOfDimensions,isRectangular,ntd,0,0,0,I1,I2,I3,pc,t);  // p exact solution 
                                                e.gd( uxe,xLocal,numberOfDimensions,isRectangular,ntd,1,0,0,I1,I2,I3,V,t);  // v.x
                                                e.gd( uye,xLocal,numberOfDimensions,isRectangular,ntd,0,1,0,I1,I2,I3,V,t);  // v.y
                                                if( numberOfDimensions==3 )
                                                {
                                                    uze.redim(I1,I2,I3,V);
                                                    e.gd( uze,xLocal,numberOfDimensions,isRectangular,ntd,0,0,1,I1,I2,I3,V,t);  // v.z
                                                }
                        // The sign is correct here I think: normalForce = sigma.normal = (-pI + tauv)*normal 
                                                if( numberOfDimensions==2 )
                                                {
                                                    int axis=0;
                                                    tractionRate(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,axis)
                                                                                                                          -mu*( (uxe(I1,I2,I3,uc)+uxe(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                                                      (uye(I1,I2,I3,uc)+uxe(I1,I2,I3,vc))*normal(I1,I2,I3,1)) );
                                                    axis=1;
                                                    tractionRate(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,axis)
                                                                                                                          -mu*( (uxe(I1,I2,I3,vc)+uye(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                                                      (uye(I1,I2,I3,vc)+uye(I1,I2,I3,vc))*normal(I1,I2,I3,1)) );
                                                }
                                                else
                                                {
                                                    int axis=0;
                                                    tractionRate(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,0)
                                                                                                                          -(mu*((uxe(I1,I2,I3,uc)+uxe(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                                                      (uye(I1,I2,I3,uc)+uxe(I1,I2,I3,vc))*normal(I1,I2,I3,1)+ 
                                                                                                                                      (uze(I1,I2,I3,uc)+uxe(I1,I2,I3,wc))*normal(I1,I2,I3,2)) ) );
                                                    axis=1;
                                                    tractionRate(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,1)
                                                                                                                          -(mu*((uxe(I1,I2,I3,vc)+uye(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                                                      (uye(I1,I2,I3,vc)+uye(I1,I2,I3,vc))*normal(I1,I2,I3,1)+ 
                                                                                                                                      (uze(I1,I2,I3,vc)+uye(I1,I2,I3,wc))*normal(I1,I2,I3,2)) ) );
                                                    axis=2;
                                                    tractionRate(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,2)
                                                                                                                          -(mu*((uxe(I1,I2,I3,wc)+uze(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                                                      (uye(I1,I2,I3,wc)+uze(I1,I2,I3,vc))*normal(I1,I2,I3,1)+ 
                                                                                                                                      (uze(I1,I2,I3,wc)+uze(I1,I2,I3,wc))*normal(I1,I2,I3,2)) ) );
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        printF(" ********** TEST: SET EXACT traction-rate to ZERO : t=%9.3e *ERROR* *************\n",t);
                                        tractionRate= 0.;
                                    }
                                }
                                if( useExactTractionRate )
                                {
                                    f(I1,I2,I3,Ctr)=tractionRate(I1,I2,I3,Rx);
                                    printF(" ********** TEST: SET traction-rate to EXACT : t=%9.3e *TEMP* *************\n",t);
                  // ::display(f(I1,I2,I3,Ctr),sPrintF("traction-rate EXACT t=%9.3e, dt=%9.3e",t,dt),"%9.3e ");
                                }
                                else if( dt==0. )
                                {
                                    printF("\n --INS-- interfaceRHS-- WARNING dt=0 for traction-rate t=%9.3e SET TO ZERO !! FIX ME *********\n\n",
                                     	   gf[gfIndex].t);
                                    f(I1,I2,I3,Ctr)=0.;
                                }
                                else if( orderOfAccuracyTractionRate==1 || im2==-1 )
                                {
                                    const int prev = idh.current;  // by default use this as the old solution
                                    const real tp = idh.interfaceDataList[prev].t;
                                    RealArray & f1 = idh.interfaceDataList[prev].f;         // here is the RHS a time tp
                                    real dtp=t0-tp; // we have just computed the traction at time t0 
                                    if( fabs(dtp) < REAL_EPSILON*100. )
                                    {
                    // there is no previous traction -- this must be step 1 and the predictor step:
                                        if( debug() & 2 )
                                            printP("--INS-- IRHS: set traction-rate at =%8.2e to rate at previous time t=%9.3e\n",t,tp);
                                        f(I1,I2,I3,Ctr)=f1(I1,I2,I3,Ctr);
                                        if( true || debug() & 8 )
                                        {
                                            ::display(tractionRate,sPrintF("traction-rate - EXACT,       t=%9.3e, dt=%9.3e",t,dt),"%9.3e ");
                                            ::display(f(I1,I2,I3,Ctr),sPrintF("traction-rate - from previous step tp=%9.3e",tp),"%9.3e ");
                                        }    
                                    }
                                    else
                                    {
                                        assert( dtp>0. );
                                        if( debug() & 2 )
                                            printP("--INS-- IRHS: get d(traction)/dt (1st order): t=%9.3e, "
                                                          "gfIndex=%i gf[gfIndex].t=%9.3e, prev=%i, tp=%9.3e\n",t,gfIndex,gf[gfIndex].t,prev,tp);
                                        f(I1,I2,I3,Ctr)= (traction(I1,I2,I3,D) - f1(I1,I2,I3,Ct))/dtp;
                                        if( true || debug() & 8 )
                                        {
                                            ::display(tractionRate,sPrintF("traction-rate - EXACT,       t=%9.3e, dt=%9.3e",t,dt),"%9.3e ");
                                            ::display(f(I1,I2,I3,Ctr),sPrintF("traction-rate - first-order, t=%9.3e, dt=%9.3e",t,dt),"%9.3e ");
                                            ::display(traction(I1,I2,I3,D),sPrintF("traction computed t0=%9.3e",t0),"%9.3e ");
                                            ::display(f1(I1,I2,I3,Ct),sPrintF("traction prev=%i t=%9.3e",prev,idh.interfaceDataList[prev].t),"%9.3e ");
                                            ::display(f(I1,I2,I3,Ctr),sPrintF("traction-rate - first-order, t=%9.3e",t),pDebugFile,"%9.3e ");
                                        }
                                    }
                                    if( evalExactTractionRate ) //#######################################
                                    {
                                        real err = max(fabs(f(I1,I2,I3,Ctr)-tractionRate(I1,I2,I3,Rx)))/(max(fabs(tractionRate(I1,I2,I3,Rx)))+1.);
                                        if( err > .01 )
                                            printF(" >>>>>> TRACTION-RATE t=%9.3e t0=%9.3e relative err=%9.2e  **TROUBLE** \n",t,t0,err);
                                        else
                                            printF(" >>>>>> TRACTION-RATE t=%9.3e t0=%9.3e relative err=%9.2e\n",t,t0,err);
                                        if( FALSE )
                                        {
                                            printF(" ********** TEST: SET traction-rate to EXACT : t=%9.3e *TEMP* *************\n",t);
                                            if( !twilightZoneFlow )
                                                f(I1,I2,I3,Ctr)=tractionRate(I1,I2,I3,Rx);
                                            else
                                                f(I1,I2,I3,Ctr)=0.;  // for TZ flow this is TZ - TZ = 0 
                                        }
                                    }
                                }
                                else 
                                {
                  // Compute the traction-rate to second order accuracy 
                                    assert( im2>=0 );
                                    const int cur = idh.current;
                                    RealArray & f0 = idh.interfaceDataList[cur].f;    // f(t0)
                                    RealArray & f1 = idh.interfaceDataList[prev].f;   // f(t1)
                                    RealArray & f2 = idh.interfaceDataList[im2 ].f;   // f(t2)
                                    const real t1 = idh.interfaceDataList[prev].t;
                                    const real t2 = idh.interfaceDataList[im2 ].t;
                                    real dt0= t0-t1;
                                    real dt1= t1-t2;
                                    if( dt0<=0. || dt1<=0. )
                                    {
                                        printP("Cgins::interfaceRHS:ERROR: computing traction-rate: t0=%9.3e, prev=%i, tp=%9.3e, "
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
                  // f(I1,I2,I3,Ctr)= l0t*f0(I1,I2,I3,Ct) +l1t*f1(I1,I2,I3,Ct) + l2t*f2(I1,I2,I3,Ct);
                                    f(I1,I2,I3,Ctr)= l0t*traction(I1,I2,I3,D) +l1t*f1(I1,I2,I3,Ct) + l2t*f2(I1,I2,I3,Ct);
                                    if( TRUE )
                                    {
                                        if( true || t0 < 3.*dt0 )
                                  	printF(" ********** EVAL traction-rate to 2nd-order from traction at times : t0=%8.2e, t1=%8.2e, t2=%8.2e, dt0=%9.3e, dt1=%8.2e *************\n",t0,t1,t2,dt0,dt1);
                                            ::display(tractionRate,sPrintF("traction-rate - EXACT,       t=%9.3e, dt=%9.3e",t,dt),"%9.3e ");
                                            ::display(f(I1,I2,I3,Ctr),"traction-rate (2nd-order) <- use this","%9.3e ");
                                            ::display(traction,sPrintF("traction - computed t0=%8.2e",t0),"%9.3e ");
                                            ::display(f1(I1,I2,I3,Ctr),sPrintF("f1: saved traction-rate (t1=%9.3e) prev=%i",t1,prev),"%9.3e ");
                                            ::display(f2(I1,I2,I3,Ctr),sPrintF("f2: saved traction-rate (t2=%9.3e) im2 =%i",t2,im2 ),"%9.3e ");
                                    }
                                    else if( TRUE )  // ********* TEMP 
                                    {
                                        if( true || t0 < 3.*dt0 )
                                  	printF(" ********** EVAL traction-rate to first-order : t=%8.2e, t0=%8.2e, t1=%8.2e, dt0=%9.3e *TEMP* *************\n",t,t0,t1,dt0);
                                        f(I1,I2,I3,Ctr)= (traction(I1,I2,I3,D) -f1(I1,I2,I3,Ct))/dt0;  // ****************** TEMP *************
                                        if( true )
                                        {
                                            ::display(tractionRate,sPrintF("traction-rate - EXACT,       t=%9.3e, dt=%9.3e",t,dt),"%9.3e ");
                                            ::display(f(I1,I2,I3,Ctr),"traction-rate (first-order) <- use this","%9.3e ");
                                            ::display(traction,sPrintF("traction - computed t0=%8.2e",t0),"%9.3e ");
                                            ::display(f1(I1,I2,I3,Ctr),sPrintF("f1: saved traction-rate (t1=%9.3e) prev=%i",t1,prev),"%9.3e ");
                                        }
                                    }
                                    if( evalExactTractionRate ) //#######################################
                                    {
                                        real err = max(fabs(f(I1,I2,I3,Ctr)-tractionRate(I1,I2,I3,Rx)))/(max(fabs(tractionRate(I1,I2,I3,Rx)))+1.);
                                        if( err > .01 )
                                            printF(" >>>>>> TRACTION-RATE t=%9.3e t0=%9.3e relative err=%9.2e  **TROUBLE** \n",t,t0,err);
                                        else
                                            printF(" >>>>>> TRACTION-RATE t=%9.3e t0=%9.3e relative err=%9.2e\n",t,t0,err);
                    // f(I1,I2,I3,Ctr)=tractionRate(I1,I2,I3,Rx);
                    // printF(" ********** TEST: SET traction-rate to EXACT : t=%9.3e *TEMP* *************\n",t);
                                    }
                                    if( FALSE )  // ********* TEMP 
                                    {
                                        if( t0 < 3.*dt0 )
                                  	printF(" ********** EXTRAPOLATE traction-rate to first-order : *TEMP* *************\n");
                    // real l1 = (t-t2)/(t1-t2);
                    // real l2 = (t-t1)/(t2-t1);
                    // f(I1,I2,I3,Ctr)= l1*f1(I1,I2,I3,Ctr) + l2*f2(I1,I2,I3,Ctr); // ******************** TEMP *************
                                        f(I1,I2,I3,Ctr)= f1(I1,I2,I3,Ctr);
                                        if( debug() & 4 )
                                        {
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
                                }
                                if( debug() & 8 )
                                {
                  // ::display(f1(I1,I2,I3,Ct),sPrintF("Cgins::interfaceRHS: old traction at tp=%9.3e",tp),"%8.2e ");
                                    ::display(f(I1,I2,I3,Ct),sPrintF("Cgins::interfaceRHS: new traction at t=%9.3e ",t),"%8.2e ");
                                    ::display(f(I1,I2,I3,Ctr),sPrintF("Cgins::interfaceRHS: time derivative of the traction, t=%9.3e",t),"%8.2e ");
                                }
                        }
                        else
                        {
              // -- there is no time history available to compute the traction rate --
                            if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
                            {
                                if( twilightZoneFlow )
                                {
                                    Range Rx=numberOfDimensions;
                                    RealArray tractionRate(I1,I2,I3,Rx);
                                    if( true )
                                    { // For TZ "exact traction-rate" = tractionRateTZ - tractionRateTZ = 0 
                                        tractionRate=0.;
                                    }
                                    else
                                    {
                                        int ntd=1;  // number of time derivatives
                                        {
                                            OV_GET_SERIAL_ARRAY(real,mg.vertex(),xLocal);
                                            OV_GET_VERTEX_BOUNDARY_NORMAL(mg,side,axis,normal);
                                            OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
                                            const int pc=parameters.dbase.get<int >("pc");
                                            const int uc=parameters.dbase.get<int >("uc");
                                            const int vc=parameters.dbase.get<int >("vc");
                                            const int wc=parameters.dbase.get<int >("wc");
                                            const real & nu = parameters.dbase.get<real >("nu");
                                            const real & fluidDensity = parameters.dbase.get<real>("fluidDensity");
                                            assert( fluidDensity>0. );
                                            const real mu = nu*fluidDensity; 
                                            Range V(uc,uc+numberOfDimensions-1); // velocity components
                                            realSerialArray pe(I1,I2,I3),uxe(I1,I2,I3,V),uye(I1,I2,I3,V),uze;
                                            bool isRectangular=false;
                                            e.gd( pe ,xLocal,numberOfDimensions,isRectangular,ntd,0,0,0,I1,I2,I3,pc,t);  // p exact solution 
                                            e.gd( uxe,xLocal,numberOfDimensions,isRectangular,ntd,1,0,0,I1,I2,I3,V,t);  // v.x
                                            e.gd( uye,xLocal,numberOfDimensions,isRectangular,ntd,0,1,0,I1,I2,I3,V,t);  // v.y
                                            if( numberOfDimensions==3 )
                                            {
                                                uze.redim(I1,I2,I3,V);
                                                e.gd( uze,xLocal,numberOfDimensions,isRectangular,ntd,0,0,1,I1,I2,I3,V,t);  // v.z
                                            }
                      // The sign is correct here I think: normalForce = sigma.normal = (-pI + tauv)*normal 
                                            if( numberOfDimensions==2 )
                                            {
                                                int axis=0;
                                                tractionRate(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,axis)
                                                                                                                      -mu*( (uxe(I1,I2,I3,uc)+uxe(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                                                  (uye(I1,I2,I3,uc)+uxe(I1,I2,I3,vc))*normal(I1,I2,I3,1)) );
                                                axis=1;
                                                tractionRate(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,axis)
                                                                                                                      -mu*( (uxe(I1,I2,I3,vc)+uye(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                                                  (uye(I1,I2,I3,vc)+uye(I1,I2,I3,vc))*normal(I1,I2,I3,1)) );
                                            }
                                            else
                                            {
                                                int axis=0;
                                                tractionRate(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,0)
                                                                                                                      -(mu*((uxe(I1,I2,I3,uc)+uxe(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                                                  (uye(I1,I2,I3,uc)+uxe(I1,I2,I3,vc))*normal(I1,I2,I3,1)+ 
                                                                                                                                  (uze(I1,I2,I3,uc)+uxe(I1,I2,I3,wc))*normal(I1,I2,I3,2)) ) );
                                                axis=1;
                                                tractionRate(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,1)
                                                                                                                      -(mu*((uxe(I1,I2,I3,vc)+uye(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                                                  (uye(I1,I2,I3,vc)+uye(I1,I2,I3,vc))*normal(I1,I2,I3,1)+ 
                                                                                                                                  (uze(I1,I2,I3,vc)+uye(I1,I2,I3,wc))*normal(I1,I2,I3,2)) ) );
                                                axis=2;
                                                tractionRate(I1,I2,I3,axis) = (  fluidDensity*pe(I1,I2,I3)*normal(I1,I2,I3,2)
                                                                                                                      -(mu*((uxe(I1,I2,I3,wc)+uze(I1,I2,I3,uc))*normal(I1,I2,I3,0)+
                                                                                                                                  (uye(I1,I2,I3,wc)+uze(I1,I2,I3,vc))*normal(I1,I2,I3,1)+ 
                                                                                                                                  (uze(I1,I2,I3,wc)+uze(I1,I2,I3,wc))*normal(I1,I2,I3,2)) ) );
                                            }
                                        }
                                    }
                                    f(I1,I2,I3,Ctr)=tractionRate;
                                    if( true )
                                    {
                                        printF(" ********** SET traction-rate to EXACT TZ: t=%9.3e *************\n",t);
                                        ::display(f(I1,I2,I3,Ctr),sPrintF("traction-rate EXACT TZ t=%9.3e, dt=%9.3e",t,dt),"%9.3e ");
                                    }
                                }
                                else if(  knownSolution==Parameters::userDefinedKnownSolution && (t<=0. || dt==0.) )
                                {
                                    int body=0;
                  // RealArray state(I1,I2,I3,Rx);
                                    parameters.getUserDefinedDeformingBodyKnownSolution( body,Parameters::boundaryTractionRate,
                                                                                                                                              t, grid, mg, I1,I2,I3,Ctr,f );
                  // f(I1,I2,I3,Ctr)=state(I1,I2,I3,Rx);
                                    printF(" ********** SET traction-rate to EXACT: t=%9.3e (No time history)*************\n",t);
                                    ::display(f(I1,I2,I3,Ctr),sPrintF("traction-rate EXACT t=%9.3e, dt=%9.3e",t,dt),"%9.3e ");
                                }
                                else
                                {
                                    printF("\nCgins::interfaceRightHandSide:traction-rate: t=%9.3e **WARNING** There is NO time history. "
                                                  "idh.current=%i, size=%i dt=%8.2e. Setting traction-rate=0\n\n",
                                                  t,idh.current,idh.interfaceDataList.size(),dt);
                                    f(I1,I2,I3,Ctr)=0.;
                                }
                            }
                        }
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


    // // *****************************************************************************
    // // ******************** Traction Twilight Zone Forcing *************************
    // // *****************************************************************************

    // if( twilightZoneFlow )
    // {
    //   // ---add forcing for twlight-zone flow---

    //   if( option==setInterfaceRightHandSide )
    //   { // set 
    //     //   add on TZ flow:
    //     //   bd <- bd + (true boundary position)

    //     // printP("interface:ERROR: we need to include the true boundary position for TZ. Finish me!\n");
      	
    //   }
    //   else if( option==getInterfaceRightHandSide )
    //   { // get 
    //     //   subtract off TZ flow:
    //     //   f <- f - ( pe*normal )  ** should also include viscous terms **
    //     Range Rx=numberOfDimensions;
    //     RealArray traction(I1,I2,I3,Rx);
    //     if( false )
    //     {
    //       // This is done above now
    //       int ntd=0;  // number of time derivatives
    //       getTractionTZ(traction,I1,I2,I3,t,ntd);
    //       ::display(traction,"--INS-- Traction from TZ (minus normal force)");

    //       f(I1,I2,I3,Rx+Ct.getBase()) -= traction(I1,I2,I3,Rx);
    //     }
                
                
    //     // ::display(f(I1,I2,I3,Ct),"Cgins::interface fluid traction after TZ fix");

    //     if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
    //     {
    //       // -- get the traction rate for TZ ---

    //       if( false )// This is done above now
    //       {
    //         int ntd=1;// number of time derivatives
    //         getTractionTZ(traction,I1,I2,I3,t,ntd);
    //         ::display(traction,"--INS-- Traction-rate from TZ");

    //         f(I1,I2,I3,Rx+Ctr.getBase()) -= traction(I1,I2,I3,Rx);
    //       }
                    

    //       // // time derivative of the traction (store in pe)
    //       // e.gd( pe ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,pc,t);  // p.t

    //       // for( int axis=0; axis<numberOfDimensions; axis++ )
    //       //   f(I1,I2,I3,Ctr.getBase()+axis) -= pe(I1,I2,I3)*normal(I1,I2,I3,axis); 

    //     }

    //   }
    //   else
    //   {
    //     OV_ABORT("error");
    //   }
        
    // } // end if TZ 



    }
    else
    {
        printF("Cgins::interfaceRightHandSide:unexpected interfaceType=%i\n",interfaceType(side,axis,grid));
        OV_ABORT("error");
    }
    

    return 0;
}
