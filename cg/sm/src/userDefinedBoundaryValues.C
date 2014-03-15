#include "Cgsm.h"
#include "SmParameters.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"
#include "NurbsMapping.h"
#include "MappingInformation.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

namespace
{
enum UserDefinedBoundaryConditions
{
  ellipseDeform,
  testDeform,
  tractionForcing,
  pressureForce,    
  piston,
  GaussianForcing,
  tractionFromDataPoints,
  pressureFromDataPoints,
  superseismicShock
};
 
}

// =========================================================================================
/// \brief Interactively define user specific values for boundary conditions. 
/// \details This function will be called when interactively choosing boundary conditions and the
/// option userDefinedBoundaryData is used.
///
/// You may add a time dependence to an existing boundary condition or you may define a 
/// new boundary condition. In this function you should prompt for the boundary condition
/// to be used as well as any parameters that will be needed. Parameters can be saved using
/// the setUserBoundaryConditionParameters function. 
///
/// \note:  The actual boundary values are NOT assigned in this routine. This is done in the
///    userDefinedBoundaryValues function.
/// 
/// \param side,axis,grid (input): assign boundary conditions for this face.
///
// =========================================================================================
int SmParameters::
chooseUserDefinedBoundaryValues(int side, int axis, int grid, CompositeGrid & cg)
{
  Parameters & parameters = *this;
  assert( parameters.dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");


  printF("Choose boundary values for (side,axis,grid)=(%i,%i,%i) gridName=%s\n",side,axis,grid,
	 (const char*)cg[grid].getName());

  aString menu[]=
  {
    "!user boundary values",
    "ellipse deform",
    "test deform",
    "traction forcing",
    "pressure force",
    "time dependent inflow",
    "piston",
    "Gaussian forcing",
    "traction from data points",
    "pressure from data points",
    "superseismic shock",
    "done",
    ""
  };
  aString answer,answer2;
  Index Ib1,Ib2,Ib3;
//   const int pc=parameters.dbase.get<int >("pc");
//   const int uc=parameters.dbase.get<int >("uc");
//   const int vc=parameters.dbase.get<int >("vc");
//   const int wc=parameters.dbase.get<int >("wc");
  
  for(;;)
  {
    gi.getMenuItem(menu,answer,"choose a menu item");
    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( answer=="ellipse deform" )
    {
      parameters.setUserBcType(side,axis,grid,ellipseDeform);    // set the bcType to be a unique value.
      parameters.setBcIsTimeDependent(side,axis,grid,true);  // this condition IS time dependent


      RealArray values(2);
      real deformationAmplitude=.2;
      real deformationFrequency=1.;
      gi.inputString(answer2,"Enter the amplitude and frequency");
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e",&deformationAmplitude,&deformationFrequency);
      }
      printF("***userDefinedBoundaryValues: amplitude=%9.3e frequency=%9.3e for ellipse deform\n",
	     deformationAmplitude,deformationFrequency);

      values(0)=deformationAmplitude;
      values(1)=deformationFrequency;

      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

    }
    else if( answer=="test deform" )
    {
      parameters.setUserBcType(side,axis,grid,testDeform);    // set the bcType to be a unique value.
      parameters.setBcIsTimeDependent(side,axis,grid,true);  // this condition IS time dependent

    }
    else if( answer=="traction forcing" )
    {
      parameters.setUserBcType(side,axis,grid,tractionForcing);    // set the bcType to be a unique value.
      parameters.setBcIsTimeDependent(side,axis,grid,false);  // this condition is NOT time dependent

      RealArray values(3);
      values=0.;
      gi.inputString(answer2,"Enter the traction force");
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e %e",&values(0),&values(1),&values(2));
      }
      printF("***traction force= (%9.3e,%9.3e,%9.3e)\n",values(0),values(1),values(2));

      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);
    }
    else if( answer=="pressure force" )
    {
      parameters.setUserBcType(side,axis,grid,pressureForce);    // set the bcType to be a unique value.
      parameters.setBcIsTimeDependent(side,axis,grid,false);  // this condition is NOT time dependent

      RealArray values(1);
      values=0.;
      gi.inputString(answer2,"Enter the pressure force");
      if( answer2!="" )
      {
	sScanF(answer2,"%e",&values(0));
      }
      printF("***pressure force= %9.3e for grid=%i (side,axis)=(%i,%i)\n",values(0),grid,side,axis);

      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);
    }
    else if( answer=="piston" )
    {
      parameters.setUserBcType(side,axis,grid,piston);    // set the bcType to be a unique value
      parameters.setBcIsTimeDependent(side,axis,grid,true);  // this condition IS time dependent

      RealArray values(2);
      real velx = 0.1;
      real vely = 0.0;
      gi.inputString(answer2,"Enter the x and y velocities");
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e",&velx,&vely);
      }
      printF("***userDefinedBoundaryValues: velx=%9.3e vely=%9.3e for piston\n",velx,vely);

      values(0)=velx;
      values(1)=vely;

      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);
    }
    else if( answer=="Gaussian forcing" )
    {
      printF("The Gaussian forcing is \n"
             "   f(x,y,z,t) = amp*ft(t)*fx(x,y,z)*normal \n"
             "   fx(x,y,z) = exp( -alpha*( (x-x0)^2 + (y-y0)^2 + (z-z0)^2 ) )\n"
             "   ft(t) = scale*(t/t0)^p*(1-t/t0)^p*(.5-t/t0)  for 0 < t < t0 and ft=0 otherwise,\n"
             "     (scale is chosen so the integral of ft(t) from [0,.5*t0] is 1. \n"
             "   Note that the mean in time of ft(t)=0)\n");

      parameters.setUserBcType(side,axis,grid,GaussianForcing);    // set the bcType to be a unique value.
      parameters.setBcIsTimeDependent(side,axis,grid,true);  // this condition IS time dependent

      RealArray values(7);
      values=0.;
      gi.inputString(answer2,"Enter amp,alpha,x0,y0,z0,t0,p");
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e %e %e %e %e %e",&values(0),&values(1),&values(2),&values(3),&values(4),&values(5),
               &values(6));
      }
      printF("***Using amp=%g ,alpha=%g ,x0=%g ,y0=%g ,z0=%g ,t0=%g, p=%g \n",
                 values(0),values(1),values(2),values(3),values(4),values(5),values(6));

      if( values(6)<1. )
      {
	printF("ERROR: p=%g should be greater than or equal to 1.\n",values(6));
	OV_ABORT("error");
      }
      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);
    }

    else if( answer=="traction from data points" )
    {
      parameters.setUserBcType(side,axis,grid,tractionFromDataPoints);    // set the bcType to be a unique value.
      parameters.setBcIsTimeDependent(side,axis,grid,false);              // this condition is NOT time dependent

      printF("Define the traction on a boundary from data points.\n");
      printF("Note: the data points do not need to match the boundary points.\n");
      if( cg.numberOfDimensions()==2 )
      {
	gi.inputString(answer,"Enter n, the number of data points");
	int n=0;
        sScanF(answer,"%i",&n);
        RealArray x(n,2), traction(n,2);
	for( int i=0; i<n; i++ )
	{
	  gi.inputString(answer,sPrintF("Enter x,y, x-traction, y-traction for point %i",i));
	  sScanF(answer,"%e %e %e %e",&x(i,0),&x(i,1),&traction(i,0),&traction(i,1));
	}
	
	NurbsMapping & boundary = *new NurbsMapping(1,2); boundary.incrementReferenceCount();
        RealArray parameterization;
	boundary.interpolate(x,1,parameterization);  // form a nurbs and return the parameterization

        NurbsMapping & tractionCurve = *new NurbsMapping; tractionCurve.incrementReferenceCount();
        tractionCurve.interpolate(traction,0,parameterization);  // form a curve with the given parameterization
	

	// Save the Mapping's in the data base for later use
	parameters.dbase.put<Mapping*>("userTractionForcingBoundaryCurve",&boundary);
        parameters.dbase.put<Mapping*>("userTractionForcingCurve",&tractionCurve);


	if( false )
	{
          // plot the results ...
          MappingInformation mapInfo;
	  mapInfo.graphXInterface=&gi;
          printF("edit the boundary curve...\n");
	  boundary.update( mapInfo ) ;
          printF("edit the traction curve...\n");
	  tractionCurve.update( mapInfo ) ;
	  

	}
	
      }
      else
      {
	OV_ABORT("traction from data points: 3D : finish me!");
      }
      


    }
    else if( answer=="pressure from data points" )
    {
      parameters.setUserBcType(side,axis,grid,pressureFromDataPoints);    // set the bcType to be a unique value.
      parameters.setBcIsTimeDependent(side,axis,grid,false);              // this condition is NOT time dependent

      printF("Define the pressure on a boundary from data points.\n");
      printF("Note: the data points do not need to match the boundary points.\n");
      if( cg.numberOfDimensions()==2 )
      {
	gi.inputString(answer,"Enter n, the number of data points");
	int n=0;
        sScanF(answer,"%i",&n);
        RealArray x(n,2), p(n);
	for( int i=0; i<n; i++ )
	{
	  gi.inputString(answer,sPrintF("Enter x,y,p for point %i",i));
	  sScanF(answer,"%e %e %e",&x(i,0),&x(i,1),&p(i));
	}
	
	// const real pOffset = .995; // *wdh* guess the offset ************************* FIX ME ***************
	// p-=pOffset;

	NurbsMapping & boundary = *new NurbsMapping(1,2); boundary.incrementReferenceCount();
        RealArray parameterization;
	boundary.interpolate(x,1,parameterization);  // form a nurbs and return the parameterization

        NurbsMapping & pressureCurve = *new NurbsMapping; pressureCurve.incrementReferenceCount();
        pressureCurve.interpolate(p,0,parameterization);  // form a curve with the given parameterization
	

	// Save the Mapping's in the data base for later use
	parameters.dbase.put<Mapping*>("userPressureForcingBoundaryCurve",&boundary);
        parameters.dbase.put<Mapping*>("userPressureForcingCurve",&pressureCurve);


	if( false )
	{
          // plot the results ...
          MappingInformation mapInfo;
	  mapInfo.graphXInterface=&gi;
          printF("edit the boundary curve...\n");
	  boundary.update( mapInfo ) ;
          printF("edit the pressure curve...\n");
	  pressureCurve.update( mapInfo ) ;
	  

	}
	
      }
      else
      {
	OV_ABORT("pressure from data points: 3D : finish me!");
      }
      


    }
    else if( answer=="superseismic shock" )
    {
      parameters.setUserBcType(side,axis,grid,superseismicShock);    // set the bcType to be a unique value
      parameters.setBcIsTimeDependent(side,axis,grid,true);  // this condition IS time dependent

      RealArray values(4);
      values(0) = 1.;  // velocity of the 'shock" on the surface
      values(1) = -1.;  // initial x position of the shock
      values(2) = 9.082105e-2 - 1.045151e-02;  // post shock pressure 
      values(3) = 8.888718; // theta
      

      gi.inputString(answer2,"Enter vs, xs, ps, nx,ny,nz  ('shock' velocity, initial position, post-shock pressure and angle(degrees))");
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e %e %e",&values(0),&values(1),&values(2),&values(3));
      }
      printF("***userDefinedBoundaryValues: superseismic shock: velocity=%10.4e, position=%9.3e, post-shock pressure=%10.4e, theta=%10.4e.\n",
	     values(0),values(1), values(2),values(3));

      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

    }
    
    else
    {
      printF("Unknown answer =[%s]\n",(const char*)answer2);
      gi.stopReadingCommandFile();
    }
  }

  return 0;
}




// =========================================================================================
/// \brief Assign user specific values for boundary conditions. 
/// \details The user may fill in
/// the boundaryData array with right-hand-side values for boundary conditions.
/// The user is also required to provide the time derivative of the boundary values.
/// 
/// \note  This function shows examples of applying a user defined boundary condition as
/// well as adding a time dependence to some pre-defined boundary conditions.
///
/// \param t (input) : current time.
/// \param u (input) : the current solution.
/// \param grid (input): the component grid we are assigning.
/// \param forcingType (input) : if forcingType==computeForcing then return the rhs for the 
///  boundary condition; if forcingType==computeTimeDerivativeOfForcing then return the 
///   first time derivative of the forcing.
///
// NOTE: calling sequence changed in base class, fixed this version 2012/02/20 *wdh*
// =========================================================================================
int Cgsm::
userDefinedBoundaryValues(const real & t, 
                          GridFunction & gf0,
			  const int & grid,
			  int side0 /* = -1 */,
			  int axis0 /* = -1 */,
			  ForcingTypeEnum forcingType /* =computeForcing */)
// OLD WAY: 
// int Cgsm::
// userDefinedBoundaryValues(const real & t, 
// 			  realMappedGridFunction & u, 
// 			  realMappedGridFunction & gridVelocity,
// 			  const int & grid,
// 			  int side0 /* = -1 */,
// 			  int axis0 /* = -1 */,
// 			  ForcingTypeEnum forcingType /* =computeForcing */)
{
  // printF("***userDefinedBoundaryValues\n");

  realMappedGridFunction & u = gf0.u[grid];
  MappedGrid & mg = *u.getMappedGrid();

  assert( side0>=-1 && side0<2 );
  assert( axis0>=-1 && axis0<parameters.dbase.get<int >("numberOfDimensions") );
  
  const int axisStart= axis0==-1 ? 0 : axis0;
  const int axisEnd  = axis0==-1 ? parameters.dbase.get<int >("numberOfDimensions")-1 : axis0;
  const int sideStart= side0==-1 ? 0 : side0;
  const int sideEnd  = side0==-1 ? 1 : side0;

  int numberOfSidesAssigned=0;

  Range C(0,parameters.dbase.get<int >("numberOfComponents")-1);
  const int pc=parameters.dbase.get<int >("pc");
  const int uc=parameters.dbase.get<int >("uc");
  const int vc=parameters.dbase.get<int >("vc");
  const int wc=parameters.dbase.get<int >("wc");
  const int tc=parameters.dbase.get<int >("tc");
  
  const int v1c=parameters.dbase.get<int >("v1c");
  const int v2c=parameters.dbase.get<int >("v2c");
  const int v3c=parameters.dbase.get<int >("v3c");

  const int s11c=parameters.dbase.get<int >("s11c");
  const int s12c=parameters.dbase.get<int >("s12c");
  const int s13c=parameters.dbase.get<int >("s13c");
  const int s21c=parameters.dbase.get<int >("s21c");
  const int s22c=parameters.dbase.get<int >("s22c");
  const int s23c=parameters.dbase.get<int >("s23c");
  const int s31c=parameters.dbase.get<int >("s31c");
  const int s32c=parameters.dbase.get<int >("s32c");
  const int s33c=parameters.dbase.get<int >("s33c");

  const bool gridIsMoving = parameters.gridIsMoving(grid); // true if the grid is moving


  mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEvertexBoundaryNormal);
  const realArray & vertex = mg.vertex();  // here is the array of grid points
  
 int includeGhost=1;
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    realSerialArray xLocal; getLocalArrayWithGhostBoundaries(vertex,xLocal);
  #else
    const realSerialArray & uLocal = u;
    const realSerialArray & xLocal = vertex;
  #endif

  const int numberOfDimensions = mg.numberOfDimensions();
  
  int axis;
  Index Ib1,Ib2,Ib3;
  
  for( axis=axisStart; axis<=axisEnd; axis++ )
  {
    for( int side=sideStart; side<=sideEnd; side++ )
    {
       #ifdef USE_PPP
	 realSerialArray & normal = *(mg.rcData->pVertexBoundaryNormal[axis][side]); 
       #else
        realArray & normal = mg.vertexBoundaryNormal(side,axis);
       #endif


      if( parameters.userBcType(side,axis,grid)==ellipseDeform )
      {
        RealArray values(2);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);
        // printF("***userDefinedBoundaryValues: assign (side,axis,grid)=(%i,%i,%i) variableInflow force=%i, "
        //        "values=%f,%f,%f\n",side,axis,grid,(int)forcingType,values(0),values(1),values(2));

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	
	numberOfSidesAssigned++;

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
	real deformationAmplitude=values(0);
	real deformationFrequency=values(1);

        real omega=pow(sin(deformationFrequency*Pi*t),2.);  // omega varies in the interval [0,1]

	real rad=1.;  // initial radius 
	real a =  deformationAmplitude*sin(omega);
	real b = -deformationAmplitude*sin(omega);
      
	printF("***userDefinedBoundaryValues:ellipseDeform: t=%9.3e, da=%9.3e\n",t,a);

        // Here are the displacements from a circle to form an ellipse
	bd(Ib1,Ib2,Ib3,uc)=a*xLocal(Ib1,Ib2,Ib3,0);  //  x=r*cos(theta) 
	bd(Ib1,Ib2,Ib3,vc)=b*xLocal(Ib1,Ib2,Ib3,1);  //  y=r*sin(theta)
	if( numberOfDimensions>2 )
	  bd(Ib1,Ib2,Ib3,wc)=0.;

	// ::display(bd(Ib1,Ib2,Ib3,Range(uc,vc)),"bd: boundary data");
	
        // NOTE: For the first order system we need to also specify the velocity and acceleration
        if( ((SmParameters&)parameters).isFirstOrderSystem() )
 	{
          real omegat=2.*deformationFrequency*Pi*sin(deformationFrequency*Pi*t)*cos(deformationFrequency*Pi*t);
          real at = deformationAmplitude*cos(omega)*omegat;	  
          real bt = -at;
 	  bd(Ib1,Ib2,Ib3,v1c)=at*xLocal(Ib1,Ib2,Ib3,0);  // velocity
 	  bd(Ib1,Ib2,Ib3,v2c)=bt*xLocal(Ib1,Ib2,Ib3,1);
        
          real omegatt = 2.*SQR(deformationFrequency*Pi)*(SQR(cos(deformationFrequency*Pi*t)) - 
                                                          SQR(sin(deformationFrequency*Pi*t)) );
          real att = -deformationAmplitude*cos(omega)*omegat*omegat + deformationAmplitude*cos(omega)*omegatt;
          real btt = -att;
 	  bd(Ib1,Ib2,Ib3,s11c)=att*xLocal(Ib1,Ib2,Ib3,0); //  acceleration  
 	  bd(Ib1,Ib2,Ib3,s12c)=btt*xLocal(Ib1,Ib2,Ib3,1);
 	}
	
      }
      else if( parameters.userBcType(side,axis,grid)==testDeform )
      {
        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	
	numberOfSidesAssigned++;

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
      
	printF("***userDefinedBoundaryValues:testDeform: t=%9.3e\n",t);

	bd(Ib1,Ib2,Ib3,uc)=xLocal(Ib1,Ib2,Ib3,0)*(1.+t);  
	bd(Ib1,Ib2,Ib3,vc)=xLocal(Ib1,Ib2,Ib3,1)*(1.+t);  
	if( numberOfDimensions>2 )
	  bd(Ib1,Ib2,Ib3,wc)=xLocal(Ib1,Ib2,Ib3,2)*(1.+t); 

	// ::display(bd(Ib1,Ib2,Ib3,Range(uc,vc)),"bd: boundary data");
	
      }
      else if( parameters.userBcType(side,axis,grid)==tractionForcing )
      {
        RealArray values(3);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	
	numberOfSidesAssigned++;

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
      
	printF("***userDefinedBoundaryValues: set tractionForce=(%8.2e,%8.2e,%8.2e) at t=%9.3e for"
               "(side,axis,grid)=(%i,%i,%i)\n",values(0),values(1),values(2),t,side,axis,grid);

	bd(Ib1,Ib2,Ib3,uc)=values(0);
	if( true )
  	  bd(Ib1,Ib2,Ib3,vc)=values(1)*cos(Pi*xLocal(Ib1,Ib2,Ib3,0));  // spatially variable traction
        else
	  bd(Ib1,Ib2,Ib3,vc)=values(1);

	if( numberOfDimensions>2 )
	  bd(Ib1,Ib2,Ib3,wc)=values(2);

        // NOTE: For the first order system we need to also specify: 
        if( ((SmParameters&)parameters).isFirstOrderSystem() )
 	{
          // time derivative of the traction: 
   	  bd(Ib1,Ib2,Ib3,v1c)=0.;  
   	  bd(Ib1,Ib2,Ib3,v2c)=0.;
        
          // The traction also appears here: *wdh* this is no longer needed
   	  // bd(Ib1,Ib2,Ib3,s11c)=bd(Ib1,Ib2,Ib3,uc);  
   	  // bd(Ib1,Ib2,Ib3,s12c)=bd(Ib1,Ib2,Ib3,vc);
	  // if( numberOfDimensions>2 )
	  //  bd(Ib1,Ib2,Ib3,s13c)=bd(Ib1,Ib2,Ib3,wc);
 	}

      }
      else if( parameters.userBcType(side,axis,grid)==pressureForce )
      {
        RealArray values(1);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);
	const real p0=values(0);

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	
	numberOfSidesAssigned++;

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
      
	printF("***userDefinedBoundaryValues: set pressure force=%8.2e at t=%9.3e for"
               "(side,axis,grid)=(%i,%i,%i)\n",p0,t,side,axis,grid);

	bd(Ib1,Ib2,Ib3,uc)=-p0*normal(Ib1,Ib2,Ib3,0);
	bd(Ib1,Ib2,Ib3,vc)=-p0*normal(Ib1,Ib2,Ib3,1);
	if( numberOfDimensions>2 )
	  bd(Ib1,Ib2,Ib3,wc)=-p0*normal(Ib1,Ib2,Ib3,2);

        // NOTE: For the first order system we need to also specify: 
        if( ((SmParameters&)parameters).isFirstOrderSystem() )
 	{
          // time derivative of the traction: 
   	  bd(Ib1,Ib2,Ib3,v1c)=0.;  
   	  bd(Ib1,Ib2,Ib3,v2c)=0.;
        
          // The traction also appears here:  *wdh* this is no longer needed
   	  // bd(Ib1,Ib2,Ib3,s11c)=bd(Ib1,Ib2,Ib3,uc);  
   	  // bd(Ib1,Ib2,Ib3,s12c)=bd(Ib1,Ib2,Ib3,vc);
	  // if( numberOfDimensions>2 )
	  //   bd(Ib1,Ib2,Ib3,s13c)=bd(Ib1,Ib2,Ib3,wc);
 	}

      }
      else if( parameters.userBcType(side,axis,grid)==piston )
      {
        RealArray values(2);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	
	numberOfSidesAssigned++;

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
	real velx=values(0);
	real vely=values(1);

// 	bd(Ib1,Ib2,Ib3,uc)=bd(Ib1,Ib2,Ib3,uc)+velx*t;
// 	bd(Ib1,Ib2,Ib3,vc)=bd(Ib1,Ib2,Ib3,vc)+vely*t;

	bd(Ib1,Ib2,Ib3,uc)=velx*t;
	bd(Ib1,Ib2,Ib3,vc)=vely*t;

	//printF( "vx=%e, vy=%e %i %i %i %i\n",velx,vely,uc,vc,v1c,v2c );
	
        // NOTE: For the first order system we need to also specify the velocity and acceleration
        if( ((SmParameters&)parameters).isFirstOrderSystem() )
 	{
 	  bd(Ib1,Ib2,Ib3,v1c)=velx; // velocity
 	  bd(Ib1,Ib2,Ib3,v2c)=vely;
        
	  // 	  bd(Ib1,Ib2,Ib3,s11c)=0.0; //  acceleration  
	  // 	  bd(Ib1,Ib2,Ib3,s12c)=0.0;
 	}
	
        // bd(Ib1,Ib2,Ib3,uc)=p0*normal(Ib1,Ib2,Ib3,0);
        // bd(Ib1,Ib2,Ib3,vc)=p0*normal(Ib1,Ib2,Ib3,1);

      }
      else if( parameters.userBcType(side,axis,grid)==GaussianForcing )
      {

        RealArray values(7);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

        real amp=values(0), alpha=values(1), x0=values(2), y0=values(3), z0=values(4), t0=values(5), p=values(6);

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	
	numberOfSidesAssigned++;

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
      
	if( t<0. || t>t0 )
	{
          // forcing is zero: 
	  bd(Ib1,Ib2,Ib3,uc)=0.;
	  bd(Ib1,Ib2,Ib3,vc)=0.;
	  if( numberOfDimensions>2 )
	    bd(Ib1,Ib2,Ib3,wc)=0.;
	  if( ((SmParameters&)parameters).isFirstOrderSystem() )
	  {
	    // time derivative of the traction: 
	    bd(Ib1,Ib2,Ib3,v1c)=0.;
	    bd(Ib1,Ib2,Ib3,v2c)=0.;
	    if( numberOfDimensions>2 )
	      bd(Ib1,Ib2,Ib3,v3c)=0.;
	  }
	}
	else
	{

	  RealArray fx(Ib1,Ib2,Ib3);
	  if( numberOfDimensions==2 )
	    fx= exp( -alpha*( SQR(xLocal(Ib1,Ib2,Ib3,0)-x0)+SQR(xLocal(Ib1,Ib2,Ib3,1)-y0) ) );
	  else
	    fx= exp( -alpha*( SQR(xLocal(Ib1,Ib2,Ib3,0)-x0)
                             +SQR(xLocal(Ib1,Ib2,Ib3,1)-y0)
                             +SQR(xLocal(Ib1,Ib2,Ib3,2)-z0)) );
          // Here is the time dependence, ff(t)
          // fft = (d/dt)ft
	  // * const real ff=amp*16.*SQR(t/t0)*SQR(1.-t/t0);
	  // * const real fft=amp*(32./t0)*( (t/t0)*SQR(1.-t/t0) - SQR(t/t0)*(1.-t/t0) );

          // This ft(t) has a mean zero in time 
//           const real tm = .5*(1.-sqrt(1./5.));  // max/min of f(t) occurs at this value
//           const real scale =1./( SQR(tm)*SQR(1.-tm)*(.5-tm) );
// 	  const real ff =amp*scale*SQR(t/t0)*SQR(1.-t/t0)*(.5-t/t0);
// 	  const real fft=amp*scale*( (2./t0)*(t/t0)*SQR(1.-t/t0)      *(.5-t/t0) 
// 					+ SQR(t/t0)*(-2./t0)*(1.-t/t0)*(.5-t/t0)
// 					+ SQR(t/t0)*SQR(1.-t/t0)      *(-1./t0) );

          // Here we scale f(t) so that the integral of f(t) from [0,.5*t0] is 1. (see smDoc/force.maple)
          // For p=3, the max value of f is -3.808295
          const real scale =(p+1.)*pow(2.,2.*p+3.);
          real tz = t/t0;
	  const real ff =amp*scale*         pow(tz,p  )*pow(1.-tz,p   )*(.5-tz);
	  const real fft=amp*scale*( (p/t0)*pow(tz,p-1)*pow(1.-tz,p   )*(.5-tz) 
				  + (-p/t0)*pow(tz,p  )*pow(1.-tz,p-1.)*(.5-tz)
				  +(-1./t0)*pow(tz,p  )*pow(1.-tz,p   )         );

	  
        
	  printF("***userDefinedBoundaryValues: set GaussianForcing at t=%9.3e for"
		 "(side,axis,grid)=(%i,%i,%i)\n",t,side,axis,grid);
	  printF("***Using amp=%g ,alpha=%g ,x0=%g ,y0=%g ,z0=%g ,t0=%g, p=%g, ... scale=%9.3e ff=%9.3e, fft=%9.3e \n",
		 amp,alpha,x0,y0,z0,t0,p,scale,ff,fft);
        
	  if( true )
	  {
	    real teps=1.e-7;
	    tz+=teps;
	    real ffp = amp*scale*         pow(tz,p  )*pow(1.-tz,p   )*(.5-tz);
	    printF(" ff=%9.3e, fft=%9.3e, (diff-approx = %9.3e)\n",ff,fft,(ffp-ff)/teps);
	  }

	  bd(Ib1,Ib2,Ib3,uc)=ff*fx*normal(Ib1,Ib2,Ib3,0);
	  bd(Ib1,Ib2,Ib3,vc)=ff*fx*normal(Ib1,Ib2,Ib3,1);
	  if( numberOfDimensions>2 )
	    bd(Ib1,Ib2,Ib3,wc)=ff*fx*normal(Ib1,Ib2,Ib3,2);

	  // NOTE: For the first order system we need to also specify: 
	  if( ((SmParameters&)parameters).isFirstOrderSystem() )
	  {
	    // time derivative of the traction: 
	    bd(Ib1,Ib2,Ib3,v1c)=fft*fx*normal(Ib1,Ib2,Ib3,0);  
	    bd(Ib1,Ib2,Ib3,v2c)=fft*fx*normal(Ib1,Ib2,Ib3,1);
	    if( numberOfDimensions>2 )
	      bd(Ib1,Ib2,Ib3,v3c)=fft*fx*normal(Ib1,Ib2,Ib3,2);
	  }
	  
	}
	
      }

      else if( parameters.userBcType(side,axis,grid)==tractionFromDataPoints )
      {
        // --- traction on the boundary is defined by a set of  data points ---
        //  A curve has been fit to the data points and we evaluate this curve at
        // the grid points on the boundary. 

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	
	numberOfSidesAssigned++;

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
      
	printF("***userDefinedBoundaryValues:tractionFromDataPoints set traction force at t=%9.3e for"
               "(side,axis,grid)=(%i,%i,%i)\n",t,side,axis,grid);

        
        Mapping *pBoundary = parameters.dbase.get<Mapping*>("userTractionForcingBoundaryCurve");
        assert( pBoundary!=NULL );
	Mapping & boundary = *pBoundary;
	Mapping *pTractionCurve = parameters.dbase.get<Mapping*>("userTractionForcingCurve");
	assert( pTractionCurve!=NULL );
        Mapping & tractionCurve = *pTractionCurve;

        mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
        OV_GET_SERIAL_ARRAY(real,mg.vertex(),vertex);  // vertex = local array for mg.vertex()

	realSerialArray x(Ib1,Ib2,Ib3,numberOfDimensions);
	x=vertex(Ib1,Ib2,Ib3,Range(numberOfDimensions));
        int numPoints=Ib1.getLength()*Ib2.getLength()*Ib3.getLength();
	x.reshape(numPoints,numberOfDimensions);

        // Project the grid points on the grid face onto the user defined curve "boundary" 
        //   Find r such that 
        //             boundary(r) = x ,    where x= grid points on the grid boundary 
	realSerialArray r(numPoints,numberOfDimensions);
	r=-1;
	boundary.inverseMapS(x,r); // find closest points on the "boundary curve" 
	::display(r,"r locations of boundary points","%5.2f ");

        // Evaluate the user defined traction at the points "r" 
        realSerialArray traction(numPoints,numberOfDimensions);
	tractionCurve.mapS(r,traction);
	traction.reshape(Ib1,Ib2,Ib3,numberOfDimensions);
	if( false )
	  ::display(traction,"traction values of boundary points","%5.2f ");

	bd(Ib1,Ib2,Ib3,uc)=traction(Ib1,Ib2,Ib3,0);
	bd(Ib1,Ib2,Ib3,vc)=traction(Ib1,Ib2,Ib3,1);
	if( numberOfDimensions>2 )
	  bd(Ib1,Ib2,Ib3,wc)=traction(Ib1,Ib2,Ib3,2);

        // NOTE: For the first order system we need to also specify: 
        if( ((SmParameters&)parameters).isFirstOrderSystem() )
 	{
          // time derivative of the traction: 
   	  bd(Ib1,Ib2,Ib3,v1c)=0.;  
   	  bd(Ib1,Ib2,Ib3,v2c)=0.;
          if( numberOfDimensions>2 )
            bd(Ib1,Ib2,Ib3,v3c)=0.;
          
 	}

      }

      else if( parameters.userBcType(side,axis,grid)==pressureFromDataPoints )
      {

        // --- pressure on the boundary is defined by a set of  data points ---

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	
	numberOfSidesAssigned++;

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
      
	printF("***userDefinedBoundaryValues:pressureFromDataPoints set pressure force at t=%9.3e for"
               "(side,axis,grid)=(%i,%i,%i)\n",t,side,axis,grid);

        
        Mapping *pBoundary = parameters.dbase.get<Mapping*>("userPressureForcingBoundaryCurve");
        assert( pBoundary!=NULL );
	Mapping & boundary = *pBoundary;
	Mapping *pPressureCurve = parameters.dbase.get<Mapping*>("userPressureForcingCurve");
	assert( pPressureCurve!=NULL );
        Mapping & pressureCurve = *pPressureCurve;

        mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
        OV_GET_SERIAL_ARRAY(real,mg.vertex(),vertex);  // vertex = local array for mg.vertex()

	realSerialArray x(Ib1,Ib2,Ib3,numberOfDimensions);
	x=vertex(Ib1,Ib2,Ib3,Range(numberOfDimensions));
        int numPoints=Ib1.getLength()*Ib2.getLength()*Ib3.getLength();
	x.reshape(numPoints,numberOfDimensions);

        // Project the grid points on the grid face onto the user defined curve "boundary" 
        //   Find r such that 
        //             boundary(r) = x ,    where x= grid points on the grid boundary 
	realSerialArray r(numPoints,numberOfDimensions);
	r=-1;
	boundary.inverseMapS(x,r); // find closest points on the "boundary curve" 
	if( false )
	  ::display(r,"r locations of boundary points","%5.2f ");

        // Evaluate the user defined pressure at the points "r" 
        realSerialArray p0(numPoints);
	pressureCurve.mapS(r,p0);
	p0.reshape(Ib1,Ib2,Ib3);
	::display(p0,"pressure values of boundary points","%5.2f ");

	bd(Ib1,Ib2,Ib3,uc)=-p0*normal(Ib1,Ib2,Ib3,0);
	bd(Ib1,Ib2,Ib3,vc)=-p0*normal(Ib1,Ib2,Ib3,1);
	if( numberOfDimensions>2 )
	  bd(Ib1,Ib2,Ib3,wc)=-p0*normal(Ib1,Ib2,Ib3,2);

        // NOTE: For the first order system we need to also specify: 
        if( ((SmParameters&)parameters).isFirstOrderSystem() )
 	{
          // time derivative of the traction: 
   	  bd(Ib1,Ib2,Ib3,v1c)=0.;  
   	  bd(Ib1,Ib2,Ib3,v2c)=0.;
        
          // The traction also appears here:  *wdh* this is no longer needed
   	  // bd(Ib1,Ib2,Ib3,s11c)=bd(Ib1,Ib2,Ib3,uc);  
   	  // bd(Ib1,Ib2,Ib3,s12c)=bd(Ib1,Ib2,Ib3,vc);
	  // if( numberOfDimensions>2 )
	  //   bd(Ib1,Ib2,Ib3,s13c)=bd(Ib1,Ib2,Ib3,wc);
 	}

      }

      else if( parameters.userBcType(side,axis,grid)==superseismicShock )
      {

        RealArray values(4);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

        real vs=values(0), xs=values(1), ps=values(2), theta=values(3)*Pi/180.;
	

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	
	numberOfSidesAssigned++;

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	if( t<=0. )
	  printF("***userDefinedBoundaryValues:superseismicShock: t=%9.3e, vs=%9.3e, xs=%9.3e, ps=%9.3e, "
		 "theta=%9.3e\n",t,vs,xs,ps,theta);

	const real nx=-sin(theta), ny=cos(theta), nz=0.;  // fluid normal 

        const real xi = xs + vs*t;  // current position of the shock on the top surface 
	
	bool setTimeDerivative=((SmParameters&)parameters).isFirstOrderSystem();
	
	assert( numberOfDimensions==2 );

        bool useSmoothedForce=false;
	if( useSmoothedForce )
	{
	  // smooth surface force: 

	  const real beta=50.; // 30.;
	
	  int i1,i2,i3;
	  FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	  {
	    real x = xLocal(i1,i2,i3,0);
	  
	    real tanhb = tanh(beta*(x-xi));
	    real alpha = .5*(1.-tanhb);     // alpha=0 for x >> xi and alpha=1 for x << xi 
	  
	    bd(i1,i2,i3,uc)=-ps*nx*alpha;
	    bd(i1,i2,i3,vc)=-ps*ny*alpha;
	  
	    if( setTimeDerivative )
	    {
	      real alphat = .5*beta*(1.-tanhb*tanhb)*vs;  // d(alpha)/dt 
	      bd(i1,i2,i3,v1c)=-ps*nx*alphat;
	      bd(i1,i2,i3,v2c)=-ps*ny*alphat;
	    }
	  

	  }
	}
	else
	{
	  // force on surface changes discontinuously
	  int i1,i2,i3;
	  FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	  {
	    real x = xLocal(i1,i2,i3,0);
	  
	    if( x<xi )
	    {
	      bd(i1,i2,i3,uc)=-ps*nx;
	      bd(i1,i2,i3,vc)=-ps*ny;
	  
	      if( setTimeDerivative )
	      {
		bd(i1,i2,i3,v1c)=0.;  // The time derivative is really -ps*nx/dt at x=xi 
		bd(i1,i2,i3,v2c)=0.;  // The time derivative is really -ps*ny/dt at x=xi 
	      }
	    
	    }
	    
	  }
	}
	

	
//         where( xLocal(Ib1,Ib2,Ib3,0)<xi )
// 	{
// 	  bd(Ib1,Ib2,Ib3,uc)=-ps*nx;
// 	  bd(Ib1,Ib2,Ib3,vc)=-ps*ny;
// 	  if( numberOfDimensions>2 )
// 	    bd(Ib1,Ib2,Ib3,wc)=-ps*nz;
	  
// 	}
// 	otherwise()
// 	{
// 	  bd(Ib1,Ib2,Ib3,uc)=0.;
// 	  bd(Ib1,Ib2,Ib3,vc)=0.;
// 	  if( numberOfDimensions>2 )
// 	    bd(Ib1,Ib2,Ib3,wc)=0.;
// 	}
	
//         // NOTE: For the first order system we need to also specify: 
//         if( ((SmParameters&)parameters).isFirstOrderSystem() )
//  	{
//           // time derivative of the traction: 
//    	  bd(Ib1,Ib2,Ib3,v1c)=0.;  
//    	  bd(Ib1,Ib2,Ib3,v2c)=0.;
        
//  	}

      }
      else
      {
	// printF(" perturbed shear flow: mg.boundaryCondition(side,axis)=%i \n",mg.boundaryCondition(side,axis));
	
      }
    }
    
  }
  
  return numberOfSidesAssigned;
}
