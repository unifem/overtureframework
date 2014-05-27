// This file automatically generated from interface.bC with bpp.
#include "Cgsm.h"
#include "SmParameters.h"
#include "ParallelUtility.h"
#include "Interface.h"  

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
/// \return : the number of interface data items required. (use this to dimension arrays).
/// \note: this function should be over-loaded.
// ===================================================================================
int Cgsm::
getInterfaceDataOptions( GridFaceDescriptor & info, int & interfaceDataOptions ) const
{
    int numberOfItems=0;
    const int numberOfDimensions = parameters.dbase.get<int>("numberOfDimensions");

    SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
    
    const int grid=info.grid, side=info.side, axis=info.axis;

    IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");
    if( grid<0 || grid>interfaceType.getBound(2) ||
            side<0 || side>1 || axis<0 || axis>interfaceType.getBound(1) )
    {
        printP("Cgsm::getInterfaceDataOptions:ERROR: invalid values: (grid,side,axis)=(%i,%i,%i)\n",
         	   grid,side,axis);
        OV_ABORT("Cgsm::getInterfaceDataOptions:ERROR");
    }
    if( interfaceType(side,axis,grid)==Parameters::heatFluxInterface )
    {
        interfaceDataOptions=Parameters::heatFluxInterfaceData;
        numberOfItems=1;
        
        printP("Cgsm::getInterfaceDataOptions:ERROR: not expecting a heatFluxInterface!\n");
        OV_ABORT("Cgsm::getInterfaceDataOptions:ERROR");
    }
    else if( interfaceType(side,axis,grid)==Parameters::tractionInterface ) 
    {
        const bool projectInterface = parameters.dbase.get<bool>("projectInterface");
        if( debug() & 2 )
            printP("*** Cgsm:getInterfaceDataOptions: projectInterface = %i ***\n",projectInterface);

        if( !projectInterface )
        {
      // -- the standard interface approximation requires the traction from the fluid

      // We need the traction (boundary force) at a tractionInterface:
            interfaceDataOptions=Parameters::tractionInterfaceData;
            numberOfItems+=numberOfDimensions;
        }
        else
        {
      // if we project the interface values then we need the following from the fluid:

            interfaceDataOptions = ( Parameters::positionInterfaceData     |
                         			       Parameters::velocityInterfaceData     |
                         			       Parameters::tractionInterfaceData    
                                                          );

            numberOfItems+=3*numberOfDimensions;
            
      // Parameters::accelerationInterfaceData 
      // Parameters::tractionRateInterfaceData 

        }
        
        if( pdeVariation==SmParameters::godunov )
        {
      // The godunov solver also needs the time derivative of the traction: 
            interfaceDataOptions = interfaceDataOptions | Parameters::tractionRateInterfaceData;
            numberOfItems+=numberOfDimensions;
        }


    }
    else
    {
        printP("Cgsm::getInterfaceDataOptions:ERROR: interfaceType(grid=%i,side=%i,axis=%i)=%i\n",
         	   grid,side,axis,interfaceType(side,axis,grid));
        OV_ABORT("Cgsm::getInterfaceDataOptions:ERROR");
    }

    return numberOfItems;
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
/// \param t (input) : current time.
// ===========================================================================
int
Cgsm::
interfaceRightHandSide( InterfaceOptionsEnum option, 
                                                int interfaceDataOptions,
                                                GridFaceDescriptor & info, 
                                                GridFaceDescriptor & gfd, 
                  			int gfIndex, real t )
{
  // return DomainSolver::interfaceRightHandSide(option,interfaceDataOptions,info,gfIndex,t);

  // *wdh* 081212 CompositeGrid & cg = gf[0].cg;
    CompositeGrid & cg = gf[gfIndex].cg;
    const int numberOfDimensions = cg.numberOfDimensions();
    
    const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");

    const int grid=info.grid, side=info.side, axis=info.axis;

    if( grid<0 || grid>=cg.numberOfComponentGrids() ||
            side<0 || side>1 || axis<0 || axis>=cg.numberOfDimensions() )
    {
        printP("Cgsm::interfaceRightHandSide:ERROR: invalid values: (grid,side,axis)=(%i,%i,%i)\n",
         	   grid,side,axis);
        OV_ABORT("Cgsm::interfaceRightHandSide:ERROR");
    }

    MappedGrid & mg = cg[grid];
    RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

    const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")- parameters.dbase.get<int >("numberOfExtraVariables");
    SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");

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
            printP("Cgsm::interfaceRHS:heatFlux %s RHS for (side,axis,grid)=(%i,%i,%i) a=[%5.2f,%5.2f]"
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
            printF("Cgsm::interfaceRightHandSide:ERROR: unknown option=%i\n",option);
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
            printP("Cgsm::interfaceRightHandSide:traction: %s RHS for (grid,side,axis)=(%i,%i,%i) "
           	     " t=%9.3e gfIndex=%i (current=%i)\n",
           	     (option==0 ? "get" : "set"),grid,side,axis,t,gfIndex,current);


        const int uc = parameters.dbase.get<int >("uc");
        const int v1c = parameters.dbase.get<int >("v1c");
        Range Dc(uc,uc+numberOfDimensions-1);  // displacement components
        Range C, Cd;
        if( option==setInterfaceRightHandSide )
        {
      // -----------------------------------------------------
      // ------------ Interface data has been provided -------
      // ------------ Fill in the bd array             -------
      // -----------------------------------------------------
      // **** set the RHS *****
      //   (TZ is done below) <- todo 

            int numSaved=0; // keeps track of how many things we have saved in f 

            bd=0.;

            if( interfaceDataOptions & Parameters::positionInterfaceData )
            {
        // --- interface position is given ---
                if( debug() & 4 )
        	  printP("Cgsm:interfaceRightHandSide: interface position provided t=%9.3e\n",t);

      	C=Range(numSaved,numSaved+numberOfDimensions-1);

        // the interface position is currently not used.
	// bd(I1,I2,I3,Dc)=f(I1,I2,I3,C);        // where should we put this?        

                numSaved+=numberOfDimensions;
            }
            
            if( interfaceDataOptions & Parameters::velocityInterfaceData )
            {
        // --- interface velocity is given ---
                if( debug() & 4 )
        	  printP("Cgsm:interfaceRightHandSide: interface velocity provided t=%9.3e\n",t);

      	C=Range(numSaved,numSaved+numberOfDimensions-1);

        // the interface velocity is currently not used.
	// bd(I1,I2,I3,Dc)=f(I1,I2,I3,C);     // where should we put this?          

                numSaved+=numberOfDimensions;
            }

            if( interfaceDataOptions & Parameters::accelerationInterfaceData )
            {
        // --- interface acceleration is given ---
                if( debug() & 4 )
        	  printP("Cgsm:interfaceRightHandSide: interface acceleration provided t=%9.3e\n",t);

      	C=Range(numSaved,numSaved+numberOfDimensions-1);

        // the interface acceleration is currently not used.
	// bd(I1,I2,I3,Dc)=f(I1,I2,I3,C);    // where should we put this?

                numSaved+=numberOfDimensions;
            }

            if( interfaceDataOptions & Parameters::tractionInterfaceData )
            {
        // --- interface traction is given ---
                if( debug() & 4 )
        	  printP("Cgsm:interfaceRightHandSide: interface traction provided t=%9.3e\n",t);

      	C=Range(numSaved,numSaved+numberOfDimensions-1);

	// bd(I1,I2,I3,Dc)=f(I1,I2,I3,Dc);   // old
      	bd(I1,I2,I3,Dc)=f(I1,I2,I3,C);    // ***** save traction data here for now -- FIX ME 

	// ::display(bd(I1,I2,I3,Dc),"Cgsm::interface traction from fluid");
      	

                numSaved+=numberOfDimensions;
            }

            if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
            {
	// save the time derivative of the traction:
                if( debug() & 4 )
        	  printP("Cgsm:interfaceRightHandSide: the tractionRate is provided at t=%9.3e\n",t);

                C=Range(numSaved,numSaved+numberOfDimensions-1);

        // traction rate is filled into the velocity components (v1c,...)
                Range Vc(v1c,v1c+numberOfDimensions-1);
        // bd(I1,I2,I3,Vc)=f(I1,I2,I3,Dc+numberOfDimensions);      // ************** fix me bases
                bd(I1,I2,I3,Vc)=f(I1,I2,I3,C);

      	if( debug() & 8 )
      	{
        	  ::display(bd(I1,I2,I3,Vc),sPrintF("Cgsm::interfaceRightHandSide: time derivative of the traction, t=%9.3e",t),"%8.2e ");
      	}

      	numSaved+=numberOfDimensions;
            }
            

            if( debug() & 8 )
            {
                bd(I1,I2,I3,Dc).display("Cgsm::interfaceRightHandSide:set: Here is the RHS (traction)");
            }
        
        }
        else if( option==getInterfaceRightHandSide )
        {

      // -----------------------------------
      // ---- Return the interface data ----
      // -----------------------------------

            if( !(interfaceDataOptions & Parameters::positionInterfaceData) )
            {
      	printP("interfaceRightHandSide:get:ERROR: interfaceDataOptions does not include positionInterfaceData??\n");
      	OV_ABORT("error");
            }
            
            realMappedGridFunction & u = gf[gfIndex].u[grid];
            OV_GET_SERIAL_ARRAY(real,u,uLocal);

      // We could optimize this for rectangular grids 
            mg.update(MappedGrid::THEvertex);
            OV_GET_SERIAL_ARRAY(real,mg.vertex(),vertex);

            int numSaved=0; // keeps track of how many things we have saved in f 

            if( interfaceDataOptions & Parameters::positionInterfaceData )
            {
	// -- return the position of the boundary --

      	Range Rx(0,numberOfDimensions-1);                // vertex components 
      	C=Range(numSaved,numSaved+numberOfDimensions-1); // save displacement in these components of f
                Cd=C;  // Save me for TZ below

      	if( !parameters.dbase.has_key("u1c") )
      	{
        	  printP("interfaceRightHandSide:ERROR: unable to find displacement component u1c in the data-base\n");
        	  OV_ABORT("error");
      	}
      	const int u1c = parameters.dbase.get<int >("u1c");
      	assert( u1c>=0 );
      	Range Dc(u1c,u1c+numberOfDimensions-1);  // displacement components

	// Some solid-mechanics solvers compute the displacements, others the full deformation.	
      	bool methodComputesDisplacements=true;
      	if( parameters.dbase.has_key("methodComputesDisplacements") )
      	{
        	  methodComputesDisplacements=parameters.dbase.get<bool>("methodComputesDisplacements");
      	}
      	if( methodComputesDisplacements )
      	{
        	  if( debug() & 4 )
          	    printP("interfaceRightHandSide:get interface position and save in components [%i,%i] \n",C.getBase(),C.getBound());

        	  f(I1,I2,I3,C)=vertex(I1,I2,I3,Rx)+uLocal(I1,I2,I3,Dc);            // ***** fix me: f should take base 0 when we change
	  // f(I1,I2,I3,Dc)=vertex(I1,I2,I3,Rx)+uLocal(I1,I2,I3,Dc);              // ***** fix me: f should take base 0 when we change

      	}
      	else
      	{  // for now this is for Jeff's hemp code -- fix me --
        	  if( true )
        	  {
	    // for linear elasticity, the displacement is saved in components u1c,u2c,u3c
          	    f(I1,I2,I3,C)=vertex(I1,I2,I3,Rx)+uLocal(I1,I2,I3,Dc);
        	  }
        	  else
        	  {
          	    f(I1,I2,I3,C)=uLocal(I1,I2,I3,Dc);
        	  }
         	   
      	}
      	
      	if( debug() & 8 )
      	{
        	  f(I1,I2,I3,C).display("interfaceRightHandSide:get: Here is the RHS (vertex+displacement)");
      	}

      	numSaved+=numberOfDimensions;
            }
            
            
            if( interfaceDataOptions & Parameters::velocityInterfaceData )
            {
	// -- save the interface velocity --
      	if( debug() & 2 )
        	  printP("Cgsm:interfaceRightHandSide: Save the interface velocity.\n");
      	
      	const int v1c = parameters.dbase.get<int >("v1c");
      	assert( v1c>=0 );
      	Range V(v1c,v1c+numberOfDimensions-1);           
      	C=Range(numSaved,numSaved+numberOfDimensions-1);   // save velocity in these components of f
      	f(I1,I2,I3,C)=uLocal(I1,I2,I3,V);

      	numSaved+=numberOfDimensions;
            }
        
            if( interfaceDataOptions & Parameters::accelerationInterfaceData )
            {
	// -- save the interface acceleration --
      	if( debug() & 2 )
        	  printP("Cgsm:interfaceRightHandSide: Save the interface acceleration. FINISH ME \n");

      	C=Range(numSaved,numSaved+numberOfDimensions-1);   // save acceleration in these components of f

	// f(I1,I2,I3,C)=uLocal(I1,I2,I3,V);

      	numSaved+=numberOfDimensions;
            }
        
            if( interfaceDataOptions & Parameters::tractionInterfaceData )
            {
	// -- save the interface traction --
      	if( debug() & 2 )
        	  printP("Cgsm:interfaceRightHandSide: Save the interface traction.  FINISH ME \n");

      	C=Range(numSaved,numSaved+numberOfDimensions-1);   // save traction in these components of f

	// f(I1,I2,I3,C)=uLocal(I1,I2,I3,V);

      	numSaved+=numberOfDimensions;
            }
        
            if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
            {
	// -- save the interface traction rate --
      	if( debug() & 2 )
        	  printP("Cgsm:interfaceRightHandSide: Save the interface traction rate. FINISH ME \n");

      	C=Range(numSaved,numSaved+numberOfDimensions-1);   // save traction rate in these components of f

	// f(I1,I2,I3,C)=uLocal(I1,I2,I3,V);

      	numSaved+=numberOfDimensions;
            }
        


        } // end option==getInterfaceRightHandSide
        


    // *****************************************************************************
    // ******************** Traction Twilight Zone Forcing *************************
    // *****************************************************************************

        if( parameters.dbase.get<bool >("twilightZoneFlow") )
        {
      // ---add forcing for twilight-zone flow---

            OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

            const bool isRectangular = false; // ** do this for now ** mg.isRectangular();

            if( !isRectangular )
      	mg.update(MappedGrid::THEcenter | MappedGrid::THEvertexBoundaryNormal );

            realArray & x= mg.center();
#ifdef USE_PPP
            realSerialArray xLocal; 
            if( !isRectangular ) 
      	getLocalArrayWithGhostBoundaries(x,xLocal);
#else
            const realSerialArray & xLocal = x;
#endif

            const int rc=parameters.dbase.get<int >("rc");
            const int uc=parameters.dbase.get<int >("uc");
            const int vc=parameters.dbase.get<int >("vc");
            const int wc=parameters.dbase.get<int >("wc");


            const int s11c=parameters.dbase.get<int >("s11c");
            const int s12c=parameters.dbase.get<int >("s12c");
            const int s13c=parameters.dbase.get<int >("s13c");
            const int s21c=parameters.dbase.get<int >("s21c");
            const int s22c=parameters.dbase.get<int >("s22c");
            const int s23c=parameters.dbase.get<int >("s23c");
            const int s31c=parameters.dbase.get<int >("s31c");
            const int s32c=parameters.dbase.get<int >("s32c");
            const int s33c=parameters.dbase.get<int >("s33c");

            if( option==setInterfaceRightHandSide )
            { // set 
	//   add on TZ flow:
	//   bd <- bd + (true boundary traction)

                
                #ifdef USE_PPP
         	 const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
                #else
         	 const realSerialArray & normal = mg.vertexBoundaryNormal(side,axis);
                #endif

      	Range Dc(uc,uc+numberOfDimensions-1); // displacements

      	RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
      	RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");
      	real lambda = lambdaGrid(grid);
      	real mu = muGrid(grid);
      	real alpha = lambda+2.*mu;
    
      	if( pdeVariation==SmParameters::nonConservative || pdeVariation==SmParameters::conservative )
      	{
          // TZ forcing for traction for SOS codes: compute traction from displacements 

        	  realSerialArray uex(I1,I2,I3,Dc),uey(I1,I2,I3,Dc);
        	  e.gd( uex,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,Dc,t);  // u.x : exact solution 
        	  e.gd( uey,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,Dc,t);  // u.y : exact solution 

        	  bd(I1,I2,I3,0) += (normal(I1,I2,I3,0)*( alpha*uex(I1,I2,I3,uc)+lambda*uey(I1,I2,I3,vc)) +
                       			     normal(I1,I2,I3,1)*( mu*( uey(I1,I2,I3,uc)+uex(I1,I2,I3,vc) ) ) );
      	
        	  bd(I1,I2,I3,1) += (normal(I1,I2,I3,0)*( mu*( uey(I1,I2,I3,uc)+uex(I1,I2,I3,vc) )) +
                       			     normal(I1,I2,I3,1)*( alpha*uey(I1,I2,I3,vc)+lambda*uex(I1,I2,I3,uc) ) );

        	  if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
        	  {
	    // tractionRate: time derivative of the traction
	    // compute time derivatives of velocity spatial derivatives
             // traction rate is filled into the velocity components (v1c,...)
          	    e.gd( uex,xLocal,numberOfDimensions,isRectangular,1,1,0,0,I1,I2,I3,Dc,t);  // v.x : exact solution 
          	    e.gd( uey,xLocal,numberOfDimensions,isRectangular,1,0,1,0,I1,I2,I3,Dc,t);  // v.y : exact solution 


	    // *note* uex is really vex
          	    bd(I1,I2,I3,v1c+0) += (normal(I1,I2,I3,0)*( alpha*uex(I1,I2,I3,uc)+lambda*uey(I1,I2,I3,vc)) +
                           				   normal(I1,I2,I3,1)*( mu*( uey(I1,I2,I3,uc)+uex(I1,I2,I3,vc) ) ) );
      	
          	    bd(I1,I2,I3,v1c+1) += (normal(I1,I2,I3,0)*( mu*( uey(I1,I2,I3,uc)+uex(I1,I2,I3,vc) )) +
                           				   normal(I1,I2,I3,1)*( alpha*uey(I1,I2,I3,vc)+lambda*uex(I1,I2,I3,uc) ) );
        	  }
        	  

      	}
      	else if( pdeVariation==SmParameters::godunov )
      	{
          // TZ forcing for traction for FOS codes: compute traction from stress	  
        	  
        	  Range Sc(s11c,s22c);
        	  realSerialArray ue(I1,I2,I3,Sc);
        	  e.gd( ue,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,Sc,t);   // exact values for the stress


        	  bd(I1,I2,I3,uc) += normal(I1,I2,I3,0)*ue(I1,I2,I3,s11c) + normal(I1,I2,I3,1)*ue(I1,I2,I3,s21c);
        	  bd(I1,I2,I3,vc) += normal(I1,I2,I3,0)*ue(I1,I2,I3,s12c) + normal(I1,I2,I3,1)*ue(I1,I2,I3,s22c);
      	
   	  // ::display(ue(I1,I2,I3,s11c),"TZ ue(I1,I2,I3,s11c)");
	  // ::display(bd(I1,I2,I3,uc),"TZ traction on solid boundary");
        	  if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
        	  {
	    // tractionRate: time derivative of the traction
	    // compute time derivatives of velocity spatial derivatives
             // traction rate is filled into the velocity components (v1c,...)
          	    
          	    e.gd( ue,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,Sc,t);   // exact values for the d(stress)/dt 
          	    bd(I1,I2,I3,v1c+0) += normal(I1,I2,I3,0)*ue(I1,I2,I3,s11c) + normal(I1,I2,I3,1)*ue(I1,I2,I3,s21c);
          	    bd(I1,I2,I3,v1c+1) += normal(I1,I2,I3,0)*ue(I1,I2,I3,s12c) + normal(I1,I2,I3,1)*ue(I1,I2,I3,s22c);

        	  }
        	  
      	}
      	else
      	{
        	  printF("interface:ERROR: pdeVariation=%i not implemented yet. \n",(int)pdeVariation);
      	}

      	if( numberOfDimensions==3 )
      	{
        	  printP("interface:ERROR: add traction forcing in 3D for TZ. Finish me!\n");
        	  OV_ABORT("error");
      	}
      	
        // bd(I1,I2,I3,V) += xe(I1,I2,I3,V);
      	
            }
            else if( option==getInterfaceRightHandSide )
            { // get 
	//   subtract off TZ flow:
        //   f <- f - ( sm-boundary-position )  +  ( TZ-interface-position)

      	Range D=numberOfDimensions;
      	Range Dc(uc,uc+numberOfDimensions-1); // displacements
      	realSerialArray ue(I1,I2,I3,Dc);
      	e.gd( ue,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,Dc,t);  // u : exact solution        


      	bool methodComputesDisplacements=true;
      	if( parameters.dbase.has_key("methodComputesDisplacements") )
        	  methodComputesDisplacements=parameters.dbase.get<bool>("methodComputesDisplacements");

	// Some solid-mechanics solvers compute the displacements, others the full deformation.	
      	if( methodComputesDisplacements )
      	{
        	  f(I1,I2,I3,Cd) -= xLocal(I1,I2,I3,D) + ue(I1,I2,I3,Dc); 

                    const real *v0 = parameters.dbase.get<real [3]>("tzInterfaceVelocity");  // this should match vg0 in the Deforming body motion 
                    const real *a0 = parameters.dbase.get<real [3]>("tzInterfaceAcceleration");         // this should match ag0 in the Deforming body motion 
        	  if( t<3.*dt )
        	  {
          	    printP("interface:INFO: set TZ interface at t=%9.3e for (grid,side,axis)=(%i,%i,%i) to be x0 + v0*t + a0*t^2\n"
                                      " v0=(%g,%g,%g), a0=(%g,%g,%g) (NOTE: these should match the values for the DeformingBodyMotion)\n",t,grid,side,axis,
               		   v0[0],v0[1],v0[2], a0[0],a0[1],a0[2]);
        	  }
        	  
        	  for( int axis=0; axis<numberOfDimensions; axis++ )
        	  {
          	    f(I1,I2,I3,Cd.getBase()+axis) += xLocal(I1,I2,I3,axis) + t*( v0[axis] + t*( a0[axis] ) );
        	  }
        	  

      	}
      	else
      	{
        	  printP("interface:ERROR: subtract boundary position for TZ with !methodComputesDisplacements. Finish me!\n");
        	  OV_ABORT("error");
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
        printF("Cgsm::interfaceRightHandSide:unexpected interfaceType=%i\n",interfaceType(side,axis,grid));
        OV_ABORT("error");
    }
    

    return 0;
}
