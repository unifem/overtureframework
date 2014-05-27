#include "Cgad.h"
#include "AdParameters.h"
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
  polynomialTimeVariation,
  specifiedNeumannValues
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
int AdParameters::
chooseUserDefinedBoundaryValues(int side, int axis, int grid, CompositeGrid & cg)
{
  Parameters & parameters = *this;
  assert( parameters.dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  

  printF("Choose boundary values for (side,axis,grid)=(%i,%i,%i) gridName=%s\n",side,axis,grid,
	 (const char*)cg[grid].getName());

  aString menu[]=
  {
    "!user boundary values",
    "polynomial time variation",
    "specified Neumann values",
    "done",
    ""
  };
  aString answer,answer2;
  Index Ib1,Ib2,Ib3;
  
  for(;;)
  {
    gi.getMenuItem(menu,answer,"choose a menu item");
    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( answer=="specified Neumann values" )
    {
      parameters.setUserBcType(side,axis,grid,specifiedNeumannValues);    // set the bcType to be a unique value.
      parameters.setBcIsTimeDependent(side,axis,grid,true);               // this condition IS time dependent by default

      // Query for parameters : for now just input an "option" number
      RealArray values(2);  // store parameters here 
      values=0.;
      gi.inputString(answer2,"Enter option (the option value  u.n=option)");
      real option=0.;
      sScanF(answer2,"%e",&option);
      printF("Setting option=%g\n",option);
      values(0)=option;
      
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);  // save values

    }
    else if( answer=="polynomial time variation" )
    {
      parameters.setUserBcType(side,axis,grid,polynomialTimeVariation);    // set the bcType to be a unique value.
      parameters.setBcIsTimeDependent(side,axis,grid,true);                // this condition IS time dependent by default


      RealArray values(5,numberOfComponents);
      values=0.;
      printF("The polynomial variation is of the form: T = a0 + a1*t + a2*t^2 + a3*t^3 + a3*t^4\n");

      for( int n=0; n<numberOfComponents; n++ )
      {
	gi.inputString(answer2,sPrintF("Enter a0,a1,a2,a3,a4, for component %i",n));
	if( answer2!="" )
	{
	  sScanF(answer2,"%e %e %e %e %e",&values(0,n),&values(1,n),&values(2,n),&values(3,n),&values(4,n));
	}
	printF("***userDefinedBoundaryValues: component %i: a0=%9.3e, a1=%9.3e, a2=%9.3e, a3=%9.3e, a4=%9.3e .\n",
	       n,values(0,n),values(1,n),values(2,n),values(3,n),values(4,n));
      }
      
      if( max(fabs(values(Range(1,values.getBound(0)),values.dimension(1))))==0. )
      {
        parameters.setBcIsTimeDependent(side,axis,grid,false);                // this condition is NOT time dependent
	printF("INFO: this BC is NOT dependent on time.\n");
      }
      
      // save the parameters to be used when evaluating the time dependent BC's:
      values.reshape(values.getLength(0)*numberOfComponents);
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

    }
    else
    {
      printF("AdParameters::chooseUserDefinedBoundaryValues: Unknown answer =[%s]\n",(const char*)answer2);
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
int Cgad::
userDefinedBoundaryValues(const real & t, 
                          GridFunction & gf0,
			  const int & grid,
			  int side0 /* = -1 */,
			  int axis0 /* = -1 */,
			  ForcingTypeEnum forcingType /* =computeForcing */)
{
  printF("Cgad::userDefinedBoundaryValues: start...\n");

  realMappedGridFunction & u = gf0.u[grid];
  MappedGrid & mg = *u.getMappedGrid();

  assert( side0>=-1 && side0<2 );
  assert( axis0>=-1 && axis0<parameters.dbase.get<int >("numberOfDimensions") );
  
  const int axisStart= axis0==-1 ? 0 : axis0;
  const int axisEnd  = axis0==-1 ? parameters.dbase.get<int >("numberOfDimensions")-1 : axis0;
  const int sideStart= side0==-1 ? 0 : side0;
  const int sideEnd  = side0==-1 ? 1 : side0;

  int numberOfSidesAssigned=0;

  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  Range C(0,numberOfComponents-1);
  const int tc=parameters.dbase.get<int >("tc");
  
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


      if( parameters.userBcType(side,axis,grid)==specifiedNeumannValues )
      {
        RealArray values(2);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);
	real option=values(0); // convert back to an integer
        printF("INFO: Neumann BC values: option=%g for (side,axis,grid)=(%i,%i,%i) at t=%8.2e\n",option,
        side,axis,grid,t);
	
        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	
	numberOfSidesAssigned++;

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	for( int n=0; n<numberOfComponents; n++ )
	{
          // Assign the RHS for the Neumann BC
          bd(Ib1,Ib2,Ib3,n)=option;
	}


      }
      else if( parameters.userBcType(side,axis,grid)==polynomialTimeVariation )
      {
        RealArray values(5*numberOfComponents);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);
        values.reshape(5,numberOfComponents);

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	
	numberOfSidesAssigned++;

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);


	for( int n=0; n<numberOfComponents; n++ )
	{
	  real a0=values(0,n), a1=values(1,n), a2=values(2,n), a3=values(3,n), a4=values(4,n);
	  bd(Ib1,Ib2,Ib3,n)=a0+t*(a1+t*(a2+t*(a3+t*(a4))));
	}
	
	
      }
      else
      {
	// printF(" perturbed shear flow: mg.boundaryCondition(side,axis)=%i \n",mg.boundaryCondition(side,axis));
	
      }
    }
    
  }
  
  return numberOfSidesAssigned;
}
