#include "DomainSolver.h"
#include "Interface.h"
#include "ParallelUtility.h"

// ****** NOTE: this base class implementation assumes that interface BC is for the Temperature only *****

// ===========================================================================
/// \brief Setup an interface boundary condition.
/// \details This function is used when solving the interface equations 
///           by iteration. It will setup the interface conditions that should be
///           used. For example, on a heatFlux interface the interface BC may 
///           be Dirichlet, Neumann or mixed. This choice is determined by cgmp.
/// \param info (input) : contains the info on which interface to set. 
// ===========================================================================
int
DomainSolver::
setInterfaceBoundaryCondition( GridFaceDescriptor & info )
{

  CompositeGrid & cg = gf[0].cg;
  
  const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");

  // *** As a start, here is how we save the interface BC info ***
  //   interfaceCondition(side,axis,grid) = [dirichletInterface/neumannInterface]
  //
  if (!parameters.dbase.has_key("interfaceCondition")) 
  {
    parameters.dbase.put<IntegerArray>("interfaceCondition");
    IntegerArray & interfaceCondition = parameters.dbase.get<IntegerArray>("interfaceCondition");

    interfaceCondition.redim(2,3,cg.numberOfComponentGrids());
    interfaceCondition=Parameters::dirichletInterface;
  }
  
  IntegerArray & interfaceCondition = parameters.dbase.get<IntegerArray>("interfaceCondition");

  const int grid=info.grid, side=info.side, axis=info.axis;

  if( grid<0 || grid>=cg.numberOfComponentGrids() ||
      side<0 || side>1 || axis<0 || axis>=cg.numberOfDimensions() )
  {
    printF("DomainSolver::setInterfaceBoundaryCondition:ERROR: invalid values: (side,axis,grid)=(%i,%i,%i)\n",
	   side,axis,grid);
    Overture::abort("DomainSolver::setInterfaceBoundaryCondition:ERROR");
  }

  if( interfaceType(side,axis,grid)==Parameters::heatFluxInterface )
  {
    // ****************************************
    // ********* Heat flux interface **********
    // ****************************************

    printP("DomainSolver::setInterfaceBC: set heat-flux interface BC=%i for (side,axis,grid)=(%i,%i,%i)"
           " a0=%6.2f a1=%6.2f\n",
	   info.interfaceBC,side,axis,grid,info.a[0],info.a[1]);

    interfaceCondition(side,axis,grid)=info.interfaceBC;

    // from insp.C
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)

    RealArray & bcData = parameters.dbase.get<RealArray>("bcData");
    const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    if( bcData.getLength(0)<3*numberOfComponents || bcData.getLength(1)!=2 )
    {
      bcData.display("error");
      Overture::abort("error");
    }

    const int tc = parameters.dbase.get<int >("tc");   
    assert( tc>=0 );  

    mixedCoeff(tc,side,axis,grid)=info.a[0];
    mixedNormalCoeff(tc,side,axis,grid)=info.a[1];
    
  }
  else if( interfaceType(side,axis,grid)==Parameters::tractionInterface )
  {
    // *******************************************
    // ********** Traction Interface *************
    // *******************************************

    printP("DomainSolver::setInterfaceBC: do nothing for interfaceType(%i,%i,%i)=%i \n",side,axis,grid,
	   interfaceCondition(side,axis,grid));
  }
  else
  {
    printP("DomainSolver::setInterfaceBC: do nothing for interfaceType(%i,%i,%i)=%i \n",side,axis,grid,
	   interfaceCondition(side,axis,grid));
  }

}

// ===================================================================================
/// \brief Return the interface data required for a given type of interface.
/// \param info (input) : the descriptor for the interface.
/// \param interfaceDataOptions (output) : a list of items from Parameters::InterfaceDataEnum that define
///                    which data to get (or which data were set).  Multiple items are
///                     chosen by bit-wise or of the different options
/// \note: this function should be over-loaded.
// ===================================================================================
int DomainSolver::
getInterfaceDataOptions( GridFaceDescriptor & info, int & interfaceDataOptions ) const
{

  const int grid=info.grid, side=info.side, axis=info.axis;

  IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");
  if( grid<0 || grid>interfaceType.getBound(2) ||
      side<0 || side>1 || axis<0 || axis>interfaceType.getBound(1) )
  {
    printP("DomainSolver::getInterfaceDataOptions:ERROR: invalid values: (grid,side,axis)=(%i,%i,%i)\n",
	   grid,side,axis);
    OV_ABORT("DomainSolver::getInterfaceDataOptions:ERROR");
  }
  if( interfaceType(side,axis,grid)==Parameters::heatFluxInterface )
  {
    interfaceDataOptions=Parameters::heatFluxInterfaceData;
  }
  else if( interfaceType(side,axis,grid)==Parameters::tractionInterface ) 
  {
    // *** do this for now ** this function should be over-loaded by Cgad, Cgins, Cgcns, and Cgsm
    if( getClassName()=="Cgins" || getClassName()=="Cgcns" )
    {
      interfaceDataOptions=Parameters::positionInterfaceData;
    }
    else if( getClassName()=="Cgsm" )
    {
      interfaceDataOptions=Parameters::tractionInterfaceData;
    }
    else
    {
      printP("DomainSolver::getInterfaceDataOptions:ERROR: unknown class using tractionInterface\n");
      OV_ABORT("DomainSolver::getInterfaceDataOptions:ERROR");
    }
  }
  else
  {
    printP("DomainSolver::getInterfaceDataOptions:ERROR: interfaceType(grid=%i,side=%i,axis=%i)=%i\n",
	   grid,side,axis,interfaceType(side,axis,grid));
    OV_ABORT("DomainSolver::getInterfaceDataOptions:ERROR");
  }
  
  

  return 0;
}

// ========================================================================================================
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
/// \param saveTimeHistory (input) : if true, save a time-history of the requested data. This is the
///    new way to save a time-history when interfaceCommunicationMode==requestInterfaceDataWhenNeeded
// ==========================================================================================================
int
DomainSolver::
interfaceRightHandSide( InterfaceOptionsEnum option, 
                        int interfaceDataOptions,
                        GridFaceDescriptor & info, 
                        GridFaceDescriptor & gfd,
			int gfIndex, real t,
                        bool saveTimeHistory /* = false */ )
{
  // *wdh* 081212 CompositeGrid & cg = gf[0].cg;
  CompositeGrid & cg = gf[gfIndex].cg;
  const int numberOfDimensions = cg.numberOfDimensions();
  
  const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");

  const int grid=info.grid, side=info.side, axis=info.axis;

  if( grid<0 || grid>=cg.numberOfComponentGrids() ||
      side<0 || side>1 || axis<0 || axis>=cg.numberOfDimensions() )
  {
    printP("DomainSolver::interfaceRightHandSide:ERROR: invalid values: (grid,side,axis)=(%i,%i,%i)\n",
	   grid,side,axis);
    OV_ABORT("DomainSolver::interfaceRightHandSide:ERROR");
  }

  MappedGrid & mg = cg[grid];
  RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

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
      printP("DomainSolver::interfaceRHS:heatFlux %s RHS for (side,axis,grid)=(%i,%i,%i) a=[%5.2f,%5.2f]"
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


//     else
//     {
//       printf("DomainSolver::getIterativeInterfaceRightHandSide:ERROR: unknown interface BC=%i for (grid,side,axis)=(%i,%i,%i)\n",
// 	     ibc,grid,side,axis);
//       Overture::abort("DomainSolver::getIterativeInterfaceRightHandSide:ERROR");
//     }
      
    }
    else
    {
      printF("DomainSolver::interfaceRightHandSide:ERROR: unknown option=%i\n",option);
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
      printP("DomainSolver::iterativeInterfaceRHS:traction: %s RHS for (grid,side,axis)=(%i,%i,%i) "
	     " t=%9.3e gfIndex=%i (current=%i)\n",
	     (option==0 ? "get" : "set"),grid,side,axis,t,gfIndex,current);


    const int uc = parameters.dbase.get<int >("uc");
    Range V(uc,uc+numberOfDimensions-1);

    if( option==setInterfaceRightHandSide )
    {
      // **** set the RHS *****
      //   (TZ is done below) <- todo 

      bd=0.;
      bd(I1,I2,I3,V)=f(I1,I2,I3,V);

      if( debug() & 8 )
      {
        bd(I1,I2,I3,V).display("setInterfaceRightHandSide: Here is the RHS");
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


      // **** we need to call a virtual function to evaluate the traction or the position of the interface 


      // ************ do this for now as a test ***************
      if( getClassName()=="Cgins" || getClassName()=="Cgcns" )
      {
	// We could optimize this for rectangular grids 
	mg.update(MappedGrid::THEvertexBoundaryNormal);
#ifdef USE_PPP
	const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
#else
	const realSerialArray & normal = mg.vertexBoundaryNormal(side,axis);
#endif

        // *new* way : Use this: 

        Index Ib1,Ib2,Ib3;
	// getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
        Ib1=I1, Ib2=I2, Ib3=I3;
        realSerialArray traction(Ib1,Ib2,Ib3,numberOfDimensions);  // this should be a serial array

        gf[gfIndex].conservativeToPrimitive();

 	int ipar[] = {grid,side,axis,gf[gfIndex].form}; // 
 	real rpar[] = { gf[gfIndex].t }; // 

 	parameters.getNormalForce( gf[gfIndex].u,traction,ipar,rpar );

        Range D=numberOfDimensions;

        #ifndef USE_PPP
          f(I1,I2,I3,V)=traction(I1,I2,I3,D);
        #else
	  Overture::abort("ERROR: finish me for parallel");
        #endif

        // f(I1,I2,I3,V)=-traction(I1,I2,I3,D); // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&



        if( debug() & 8 )
	{
	  f(I1,I2,I3,V).display("getInterfaceRightHandSide: Here is the RHS (traction=normalForce)");
	}

	if( interfaceDataOptions & Parameters::tractionRateInterfaceData )
	{
	
	  OV_ABORT("DomainSolver::getInterfaceRightHandSide: ERROR: unable to compute the time derivative of the traction!");
	}

// 	const int pc = parameters.dbase.get<int >("pc");
// 	realSerialArray p(I1,I2,I3);

//         p=1.;  // do this for now ******************************

        // ******** into what components should we fill the force? +++++++++++++++++++++++++++++++++++++=

// 	f(I1,I2,I3,uc  )=normal(I1,I2,I3,0)*p(I1,I2,I3);
// 	f(I1,I2,I3,uc+1)=normal(I1,I2,I3,1)*p(I1,I2,I3);
//         if( numberOfDimensions>2 )
// 	  f(I1,I2,I3,uc+2)=normal(I1,I2,I3,2)*p(I1,I2,I3);
        
//  	f(I1,I2,I3,V)=0.;
//  	f(I1,I2,I3,uc)=1.;

// 	Range R4=f.dimension(3);
// 	f(I1,I2,I3,R4)=0.;
// 	f(I1,I2,I3,0)=1.;
	
      }
      else if( getClassName()=="Cgsm" )
      {
	// We could optimize this for rectangular grids 
	mg.update(MappedGrid::THEvertex);
#ifdef USE_PPP
	realSerialArray vertex; getLocalArrayWithGhostBoundaries(mg.vertex(),vertex);
#else
	const realSerialArray & vertex = mg.vertex();
#endif
        // return the position of the boundary 
        Range D(0,numberOfDimensions-1);

        // ******** into what components should we fill the positions? +++++++++++++++++++++++++++++++++++++=
        bool methodComputesDisplacements=true;
        if( parameters.dbase.has_key("methodComputesDisplacements") )
	{
	  methodComputesDisplacements=parameters.dbase.get<bool>("methodComputesDisplacements");
	}
        // Some solid-mechanics solvers compute the displacements, others the full deformation.	
	if( methodComputesDisplacements )
          f(I1,I2,I3,V)=vertex(I1,I2,I3,D)+uLocal(I1,I2,I3,V);
        else
	{  // for now this is for Jeff's hemp code -- fix me --
	   if( true )
	   {
             // for linear elasticity, the displacement is saved in components u1c,u2c,u3c
             if( !parameters.dbase.has_key("u1c") )
	     {
	       printP("interfaceRightHandSide:ERROR: unable to find variable u1c in the data-base\n");
	       Overture::abort("error");
	     }
	     const int u1c = parameters.dbase.get<int >("u1c");
	     assert( u1c>=0 );
	     Range V1(u1c,u1c+numberOfDimensions-1);
             f(I1,I2,I3,V)=vertex(I1,I2,I3,D)+uLocal(I1,I2,I3,V1);
	   }
	   else
	   {
	     f(I1,I2,I3,V)=uLocal(I1,I2,I3,V);
	   }
	   
	}
	
        if( debug() & 8 )
	{
	  f(I1,I2,I3,V).display("getInterfaceRightHandSide: Here is the RHS (vertex+displacement)");
	}
      }
      else
      {
        printP("ERROR: unknown className=[%s]\n",(const char*)getClassName());
	Overture::abort("error");
      }
      
	
//       f(I1,I2,I3,V)=0.;
//       f(I1,I2,I3,uc)=1.;
      
    }
    



  }
  else
  {
    printF("DomainSolver::iterativeInterfaceRightHandSide:unexpected interfaceType=%i\n",
	   interfaceType(side,axis,grid));
    Overture::abort("error");
  }
  

  return 0;
}
