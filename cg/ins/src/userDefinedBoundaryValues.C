#include "Cgins.h"
#include "InsParameters.h"

#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"
#include "ExternalBoundaryData.h"
#include "Integrate.h"
#include "App.h"
#include "Controller.h"
#include "BoundaryLayerProfile.h"
#include "TimeFunction.h"

#include <vector>

// *OLD WAY*  2015/03/27 *WDH*
namespace
{
// enum UserDefinedBoundaryConditions
// {
//   variableInflow,
//   timeDependentInflow,
//   perturbedShearFlow,
//   wallWithScalarFlux,
//   axisymmetricRotation,
//   linearRampInX,
//   linearRampInY,
//   ablProfile,
//   boundaryDataFromAFile, // reads probe bounding box data for e.g.
//   variableTemperature,
//   variableBoundaryValues,
//   inflowWithControl,
//   normalComponentOfVelocity,    // specify the normal component of the velocity at the boundary
//   cylindricalVelocity,          // specify cylindrical components of velocity (cr,vTheta,vPhi)
//   pressureProfile,              // for cgins, outflow pressure profile
//   knownSolutionValues,          // use boundary values from the known solution
//   flatPlateBoundaryLayerProfile,          // use a flat plate boundary layer profile
//   pressurePulse,                 // for flow in a flexible channel
//   timeFunctionOption             // use time variations from TimeFunction class
// };

real Tcontrol=0., regionVolume=0.;  // **FIX ME**

}

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
int InsParameters::
chooseUserDefinedBoundaryValues(int side, int axis, int grid, CompositeGrid & cg)
{
  Parameters & parameters = *this;
  assert( parameters.dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  const int numberOfComponents = dbase.get<int>("numberOfComponents");

  // *new* way *wdh* 2015/03/2
  const aString userDefinedBoundaryValueName=sPrintF("userBV_G%i_S%i_A%i",grid,side,axis); // unique name for this (grid,side,axis)
  if( !parameters.dbase.has_key(userDefinedBoundaryValueName) )
    parameters.dbase.put<aString>(userDefinedBoundaryValueName)="none";

  aString & userDefinedBoundaryValue = parameters.dbase.get<aString>(userDefinedBoundaryValueName);

  printF("Choose boundary values for (side,axis,grid)=(%i,%i,%i) gridName=%s (userDefinedBoundaryValueName=%s)\n",side,axis,grid,
	 (const char*)cg[grid].getName(),(const char*)userDefinedBoundaryValueName);

  aString menu[]=
  {
    "!user boundary values",
    "variable inflow",
    "time dependent inflow",
    "perturbed shear flow",
    "wall with scalar flux",
    "axisymmetric rotation",
    "linear ramp in x",
    "linear ramp in y",
    "abl profile",
    "boundary data from a file",
    "variable temperature",
    "inflow with control",
    "normal component of velocity",
    "cylindrical velocity",
    "pressure profile",
    "known solution",
    "flat plate boundary layer profile",
    "pressure pulse",
    "time function option",
    "polynomial inflow profile",
    "external temperature values", // **Added QC**
    "external flux values", // ** Added QC**
    "external robin coeffs", // ** Added QC **
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
    /////////////////////////////////////////////////////////////////
    // **Added QC**
    /////////////////////////////////////////////////////////////////
    else if (answer == "external temperature values" || answer == "external flux values")
    {
      // boundary tag when applying iterator
      userDefinedBoundaryValue = "externalHeatValues";

      // time dep
      parameters.setBcIsTimeDependent(side,axis,grid,true);

      Index Ib1, Ib2, Ib3;

      MappedGrid &mg = cg[grid];

      getBoundaryIndex(mg.gridIndexRange(), side, axis, Ib1, Ib2, Ib3);

      const int sz = Ib1.length()*Ib2.length()*Ib3.length();

      RealArray values(sz);
      values = 0.0;
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

      printF("**External heat resource setup!");
    }
    else if (answer=="external robin coeffs")
    {
      // boundary tag when applying iterator
      userDefinedBoundaryValue = "externalRobinCoeffs";

      // time dep
      parameters.setBcIsTimeDependent(side,axis,grid,true);

      Index Ib1, Ib2, Ib3;

      MappedGrid &mg = cg[grid];

      getBoundaryIndex(mg.gridIndexRange(), side, axis, Ib1, Ib2, Ib3);

      const int sz = Ib1.length()*Ib2.length()*Ib3.length();
      RealArray values(2*sz); // first sz for ambient temp, rest for h
      values = 0.0;
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

      printF("**External heat robin coefficients setup!");
    }
    /////////////////////////////////////////////////////////////////
    // **Finished QC**
    /////////////////////////////////////////////////////////////////
    else if( answer=="variable inflow" )
    {
      // parameters.setUserBcType(side,axis,grid,variableInflow);    // set the bcType to be a unique value.
      userDefinedBoundaryValue = "variableInflow";

      parameters.setBcIsTimeDependent(side,axis,grid,false);  // this condition is NOT time dependent

      gi.inputString(answer2,"Enter u,v,w");
      real u0=1., v0=0., w0=0.;
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e %e",&u0,&v0,&w0);
      }
      printF("***userDefinedBoundaryValues: assign variable inflow on (side,axis,grid)=(%i,%i,%i)\n",
	     side,axis,grid);

      RealArray values(3);
      values(0)=u0;
      values(1)=v0;
      values(2)=w0;
      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

    }
    else if( answer=="time dependent inflow" )
    {
      // parameters.setUserBcType(side,axis,grid,timeDependentInflow);  // set the bcType to be a unique value.
      userDefinedBoundaryValue = "timeDependentInflow";

      parameters.setBcIsTimeDependent(side,axis,grid,true);      // this condition is time dependent

      gi.inputString(answer2,"Enter u,v,w");
      real u0=1., v0=0., w0=0.;
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e %e",&u0,&v0,&w0);
      }
      printF("***userDefinedBoundaryValues: assign time dependent inflow on (side,axis,grid)=(%i,%i,%i)\n",
	     side,axis,grid);

      RealArray values(3);
      values(0)=u0;
      values(1)=v0;
      values(2)=w0;
      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

    }
    else if( answer=="perturbed shear flow" )
    {
      // parameters.setUserBcType(side,axis,grid,perturbedShearFlow);  // set the bcType to be a unique value.
      userDefinedBoundaryValue = "perturbedShearFlow";

      gi.inputString(answer2,"Enter u1,u2,beta,y0,f0,amp");
      real u1=2., u2=1.,beta=50., y0=.5, f0=1., amp=0.;
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e %e %e %e %e",&u1,&u2,&beta,&y0,&f0,&amp);
      }
      printF("***userDefinedBoundaryValues: assign perturbed shear flow on (side,axis,grid)=(%i,%i,%i)\n",
	     side,axis,grid);

      RealArray values(6);
      values(0)=u1;
      values(1)=u2;
      values(2)=beta;
      values(3)=y0;
      values(4)=f0;
      values(5)=amp;
      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

      parameters.setBcIsTimeDependent(side,axis,grid,amp!=0.);      // is this condition is time dependent?

    }
    else if( answer=="wall with scalar flux" )
    {
      // parameters.setUserBcType(side,axis,grid,wallWithScalarFlux);  // set the bcType to be a unique value.
      userDefinedBoundaryValue = "wallWithScalarFlux";

      gi.inputString(answer2,"Enter amplitude,radius and position: amp,radius,x0,y0,z0");
      real amp=1.,radius=1.,x0=0.,y0=0.,z0=0;
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e %e %e %e %e",&amp,&radius,&x0,&y0,&z0);
      }
      printF("***userDefinedBoundaryValues: wall with scalar flux on (side,axis,grid)=(%i,%i,%i)\n"
             "                              amp=%8.2e, radius=%8.2e, x=(%8.2e,%8.2e,%8.2e)\n",
	     side,axis,grid,amp,radius,x0,y0,z0);

      RealArray values(5);
      values(0)=amp;
      values(1)=radius;
      values(2)=x0;
      values(3)=y0;
      values(4)=z0;
      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

      parameters.setBcIsTimeDependent(side,axis,grid,false);    // is this condition is time dependent?

    }
    else if( answer=="axisymmetric rotation" )
    {
      // parameters.setUserBcType(side,axis,grid,axisymmetricRotation);    // set the bcType to be a unique value.
      userDefinedBoundaryValue = "axisymmetricRotation";
      parameters.setBcIsTimeDependent(side,axis,grid,true);  // this condition is time dependent

      gi.inputString(answer2,"Enter d(omega)/dt, u, v, t, ramp time");
      real odot=0., u0=1., v0=0., te0=1., trmp=-1;
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e %e %e %e",&odot,&u0,&v0,&te0, &trmp);
      }
      printF("***userDefinedBoundaryValues: assign axisymmetric rotation on (side,axis,grid)=(%i,%i,%i)\n",
	     side,axis,grid);

      RealArray values(5);
      values(0)=odot;
      values(1)=u0;
      values(2)=v0;
      values(3)=te0;
      values(4)=trmp;
      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);
      if ( te0<=0. && false)
	{
	  parameters.dbase.get<RealArray>("bcData")(numberOfComponents+1,side,axis,grid) = 1;
	  cout<<"adiabatic wall on "<<side<<"  "<<axis<<endl;
	}

    }
    else if( answer=="linear ramp in x" )
    {
      // parameters.setUserBcType(side,axis,grid,linearRampInX);    // set the bcType to be a unique value.
      userDefinedBoundaryValue = "linearRampInX";

      parameters.setBcIsTimeDependent(side,axis,grid,true);  // this condition is time dependent

      RealArray values(numberOfComponents*3);
      values = -1;
      for( ;; )
	{
	  gi.inputString(answer2,"Enter component, f0 (constant), slope, ramp time");
	  int comp=-1;
	  real f0=0, slope=0, trmp=0;
	  if ( answer2=="done")
	    break;
	  else if( answer2!="" )
	    {
	      sScanF(answer2,"%i %e %e %e",&comp, &f0,&slope, &trmp);
	    }

	  if ( comp>-1 )
	    {
	      values(3*comp+0)=f0;
	      values(3*comp+1)=slope;
	      values(3*comp+2)=trmp;
	    }
	  else
	    break;
	}
      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

      printF("***userDefinedBoundaryValues: assign axisymmetric rotation on (side,axis,grid)=(%i,%i,%i)\n",
	     side,axis,grid);
    }
    else if( answer=="linear ramp in y" )
    {
      // parameters.setUserBcType(side,axis,grid,linearRampInY);    // set the bcType to be a unique value.
      userDefinedBoundaryValue = "linearRampInY";

      parameters.setBcIsTimeDependent(side,axis,grid,true);  // this condition is time dependent

      RealArray values(numberOfComponents*3);
      values = -1;
      for( ;; )
	{
	  gi.inputString(answer2,"Enter component, f0 (constant), slope, ramp time");
	  int comp=-1;
	  real f0=0, slope=0, trmp=0;
	  if ( answer2=="done")
	    break;
	  else if( answer2!="" )
	    {
	      sScanF(answer2,"%i %e %e %e",&comp, &f0,&slope, &trmp);
	    }

	  if ( comp>-1 )
	    {
	      values(3*comp+0)=f0;
	      values(3*comp+1)=slope;
	      values(3*comp+2)=trmp;
	    }
	  else
	    break;
	}
      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

      printF("***userDefinedBoundaryValues: assign axisymmetric rotation on (side,axis,grid)=(%i,%i,%i)\n",
	     side,axis,grid);
    }
    else if( answer=="abl profile" )
      {
	// parameters.setUserBcType(side,axis,grid,ablProfile);
        userDefinedBoundaryValue = "ablProfile";

	parameters.setBcIsTimeDependent(side,axis,grid,false);

	gi.inputString(answer2,"Enter u_ref, z_ref, alpha, d");
	real u_ref=1., z_ref=1., alpha=1., d=0;
	if( answer2!="" )
	  {
	    sScanF(answer2,"%e %e %e %e",&u_ref,&z_ref,&alpha,&d);
	  }
	printF("***userDefinedBoundaryValues: assign abl profile on (side,axis,grid)=(%i,%i,%i)\n",
	       side,axis,grid);

	RealArray values(4);
	values(0) = u_ref;
	values(1) = z_ref;
	values(2) = alpha;
	values(3) = d;

	// save parameters to be used when evaluating the time dependent BC's:
	parameters.setUserBoundaryConditionParameters(side,axis,grid,values);
      }
    else if( answer=="boundary data from a file" )
    {
      // parameters.setUserBcType(side,axis,grid,boundaryDataFromAFile);    // set the bcType to be a unique value.
      userDefinedBoundaryValue = "boundaryDataFromAFile";

      parameters.setBcIsTimeDependent(side,axis,grid,true);  // this condition is time dependent

      // The ExternalBoundaryData class knows how to deal with external data formats
      // that hold time depedent boundary data.
      if( !parameters.dbase.has_key("externalBoundaryData") )
      {
        parameters.dbase.put<ExternalBoundaryData*>("externalBoundaryData");
        parameters.dbase.get<ExternalBoundaryData*>("externalBoundaryData")=new ExternalBoundaryData;

      }
      parameters.dbase.get<ExternalBoundaryData*>("externalBoundaryData")->update(gi);


//       aString probeFileName;
//       gi.inputString(probeFileName,"Enter the name of the probe bounding box data file");
//       printF("probeFileName=[%s]\n",(const char*)probeFileName);

//       assert( !parameters.dbase.has_key("boundaryDataFile") );


//       if( !parameters.dbase.has_key("boundaryDataFile") )
//       {
//         parameters.dbase.put<GenericDataBase*>("boundaryDataFile");
//         parameters.dbase.get<GenericDataBase*>("boundaryDataFile")=NULL;
//       }
//       GenericDataBase *& pdb = parameters.dbase.get<GenericDataBase*>("boundaryDataFile");

//       if( pdb==NULL )
//       {
// 	pdb = new HDF_DataBase;  // ************ who will delete ?? ***********************

//         printF("userDefinedBoundaryValues: opening the data base file %s for the bounding box probe info.\n",
// 	       (const char*)probeFileName);

//         HDF_DataBase & db = ( HDF_DataBase &)(*pdb);
//         db.setMode(GenericDataBase::noStreamMode);
// 	db.mount(probeFileName,"R");    // open the data base, R=read-only
//       }
//       assert( pdb!=NULL );
//       HDF_DataBase & db = ( HDF_DataBase &)(*pdb);

//       int numberOfTimes=-1;
//       db.get(numberOfTimes,"numberOfTimes");
//       RealArray times;
//       db.get(times,"times");

//       printF(" Found probe data: numberOfTimes=%i\n",numberOfTimes);
//       ::display(times,"The solution was saved at these times","%6.3f ");

//       OV_ABORT("finish me");


    }
    else if( answer=="variable temperature" )
    {
      // parameters.setUserBcType(side,axis,grid,variableTemperature);    // set the bcType to be a unique value.
      userDefinedBoundaryValue = "variableTemperature";

      parameters.setBcIsTimeDependent(side,axis,grid,false);  // this condition is NOT time dependent

      gi.inputString(answer2,"Enter T1, T2, y0");
      real q1=-1., q2=1., x0=0., y0=0.;
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e %e",&q1,&q2,&y0);
      }
      printF("***userDefinedBoundaryValues: assign variable Temperature on (side,axis,grid)=(%i,%i,%i)\n",
	     side,axis,grid);

      RealArray values(3);
      values(0)=q1;
      values(1)=q2;
      values(2)=y0;
      // save the parameters to be used when evaluating the BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

    }
    else if( answer=="inflow with control" )
    {
      printF("Inflow with control: vary the temperature on inflow.\n");

      if( !parameters.dbase.has_key("Controller") )
      {
	printF("Inflow with control:ERROR: no Controller exists!\n");
	printF("You should first create a controller (in the forcing options menu).\n");
	OV_ABORT("ERROR");
      }

      // parameters.setUserBcType(side,axis,grid,inflowWithControl);  // set the bcType to be a unique value.
      userDefinedBoundaryValue = "inflowWithControl";

      parameters.setBcIsTimeDependent(side,axis,grid,true);      // this condition is time dependent

      gi.inputString(answer2,"Enter u,v,w (inflow values)");
      real u0=1., v0=0., w0=0.;
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e %e",&u0,&v0,&w0);
      }
      printF("***userDefinedBoundaryValues: assign inflow with control on (side,axis,grid)=(%i,%i,%i)"
             " (u,v,w)=(%f,%f,%f)\n",side,axis,grid,u0,v0,w0);

      RealArray values(3);
      values(0)=u0;
      values(1)=v0;
      values(2)=w0;

      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);



//       // Create a Controller object if it does not already exist.
//       if( !parameters.dbase.has_key("Controller") )
//       {
// 	Controller controller(parameters);
// 	parameters.dbase.put<Controller>("Controller",controller);
//       }
//       Controller & controller = parameters.dbase.get<Controller>("Controller");

//       // Make changes to the controller:
//       controller.update(cg,gi);

//       // NOTE: We should share this Integrate object with the one in MovingGrids! *******************

//       if( !parameters.dbase.has_key("integrate"))
//       {
// 	printF("Create an integrate object...\n");
//         parameters.dbase.put<Integrate*>("integrate");
//         parameters.dbase.get<Integrate*>("integrate")=NULL;
//       }

//       Integrate *& pIntegrate = parameters.dbase.get<Integrate*>("integrate");
//       // cout << "pIntegrate=" << pIntegrate << endl;
//       if( pIntegrate==NULL )
//       {
// 	printF("inflow with control: Build an Integration object...\n");
//         pIntegrate = new Integrate(cg);  // ************************************ who deletes this??
//       }

//       // -- compute the volume (needed to compute the average T)---
//       Integrate & integrate = *pIntegrate;
//       real cpu = getCPU();
//       regionVolume = integrate.volume();
//       printF("inflow with control: regionVolume = %e, cpu=%8.2e(s).\n",regionVolume,getCPU()-cpu);


    }

    else if( answer=="normal component of velocity" )
    {
      // parameters.setUserBcType(side,axis,grid,normalComponentOfVelocity);    // set the bcType to be a unique value.
      userDefinedBoundaryValue = "normalComponentOfVelocity";

      parameters.setBcIsTimeDependent(side,axis,grid,false);      // this condition is NOT time dependent

      gi.inputString(answer2,"Enter un, T : the normal component of the velocity and the temperature (for Boussinesq).");
      real un=0., inflowTemperature=0.;
      sScanF(answer2,"%e %e",&un,&inflowTemperature);

      printF("***userDefinedBoundaryValues: setting the normal component of velocity on "
             "(side,axis,grid)=(%i,%i,%i) to un=%9.3e, and T=%9.3e.\n",
	     side,axis,grid,un,inflowTemperature);

      RealArray values(3);
      values(0)=un;
      values(1)=inflowTemperature;
      values(2)=0.;  // save for future use
      // save the parameters to be used when evaluating the BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);
    }

    else if( answer=="cylindrical velocity" )
    {
      // parameters.setUserBcType(side,axis,grid,cylindricalVelocity);    // set the bcType to be a unique value.
      userDefinedBoundaryValue = "cylindricalVelocity";

      parameters.setBcIsTimeDependent(side,axis,grid,false);      // this condition is NOT time dependent

      printF("Define the cylindrical components of velocity : (vr,vTheta,vPhi)=(radial,angular,axial)\n"
             " The line through the center of the cylinder is (x0,x1,x2)+ s*(d0,d1,d2),   -infty < s < infty\n");

      gi.inputString(answer2,"Enter vr, vTheta, vPhi, temperature");
      real vr=0., vTheta=0., vPhi=0., tb=0.;
      sScanF(answer2,"%e %e %e %e",&vr,&vTheta,&vPhi,&tb);

      gi.inputString(answer2,"Enter x0,y0,z0, d0,d1,d2");
      real x0=0., x1=0., x2=0., d0=0., d1=0., d2=1.;
      sScanF(answer2,"%e %e %e %e %e %e",&x0,&x1,&x2, &d0,&d1,&d2 );

      printF("***userDefinedBoundaryValues: setting the cylindrical components of velocity on "
             "(side,axis,grid)=(%i,%i,%i) to vr=%9.3e, vTheta=%9.3e, vPhi=%9.3e, T=%9.3e.\n"
             "  The cylinder axis is x(s) = (%g,%g,%g)+s*(%g,%g,%g)\n",
	     side,axis,grid,vr,vTheta,vPhi,tb,x0,x1,x2,d0,d1,d2);

      RealArray values(10);
      values(0)=vr;
      values(1)=vTheta;
      values(2)=vPhi;
      values(3)=tb;
      values(4)=x0;
      values(5)=x1;
      values(6)=x2;
      values(7)=d0;
      values(8)=d1;
      values(9)=d2;
      // save the parameters to be used when evaluating the BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);
    }
    else if( answer=="pressure profile" )
    {
      // parameters.setUserBcType(side,axis,grid,pressureProfile);    // set the bcType to be a unique value.
      userDefinedBoundaryValue = "pressureProfile";

      parameters.setBcIsTimeDependent(side,axis,grid,false);      // this condition is NOT time dependent

      printF("***userDefinedBoundaryValues:Define the pressure profile:\n"
             "   p = p0*(y-y1)/(y0-y1) + p1*(y-y0)/(y1-y0). (linear function: p(y0)=p0, p(y1)=p1).\n");

      gi.inputString(answer2,"Enter p0,p1, y0,y1");
      real p0=0., p1=1., y0=0., y1=1.;
      sScanF(answer2,"%e %e %e %e",&p0,&p1,&y0,&y1);

      printF("***userDefinedBoundaryValues: setting p0=%g, p1=%g, y0=%g, y1=%g\n",p0,p1,y0,y1);

      RealArray values(10);
      values(0)=p0;
      values(1)=p1;
      values(2)=y0;
      values(3)=y1;
      // save the parameters to be used when evaluating the BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);
    }
    else if( answer=="known solution" )
    {
      // parameters.setUserBcType(side,axis,grid,knownSolutionValues);    // set the bcType to be a unique value.
      userDefinedBoundaryValue = "knownSolutionValues";

      parameters.setBcIsTimeDependent(side,axis,grid,true);            // *FIX* ME

      printF("***userDefinedBoundaryValues:set values according to the known solution\n");

    }
    else if( answer=="flat plate boundary layer profile" )
    {
      // parameters.setUserBcType(side,axis,grid,flatPlateBoundaryLayerProfile);  // set the bcType to be a unique value.
      userDefinedBoundaryValue = "flatPlateBoundaryLayerProfile";

      parameters.setBcIsTimeDependent(side,axis,grid,false);      // this condition is NOT time dependent

      real U=1., xOffset=1., nuBL=parameters.dbase.get<real>("nu");
      printF("The flat plate boundary layer solution (Blasius) is a similiarity solution with\n"
             "the flat plate starting at x=0, y=0. The free stream velocity is U.\n"
             "To have a smooth inflow profile, enter an offset in x so the similiarity solution starts at this value\n"
             "Note: the vertical velocity v only makes sense if sqrt(nu*U/x) is small.\n"
             "NOTE: nu for the BL profile can be different than the actual nu\n");
      gi.inputString(answer,sPrintF("Enter U, xOffset and nu (defaults U=%8.2e, xOffset=%8.2e, nu=%8.2e)",U,xOffset,nuBL));
      sScanF(answer,"%e %e %e",&U,&xOffset,&nuBL);
      printF("Setting U=%9.3e, xOffset=%9.3e, nuBL=%8.2e\n",U,xOffset,nuBL);

      RealArray values(3);
      values(0)=U;
      values(1)=xOffset;
      values(2)=nuBL;
      // save the parameters to be used when evaluating the BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

    }
    else if( answer=="pressure pulse" )
    {
      // parameters.setUserBcType(side,axis,grid,pressurePulse);  // set the bcType to be a unique value.
      userDefinedBoundaryValue = "pressurePulse";

      parameters.setBcIsTimeDependent(side,axis,grid,true);      // this condition is time dependent

      printF("The pressure pulse is p = pMax*sin(pi*t/tMax),  for 0 <=t<=tMax, p=0 other-wise\n");
      gi.inputString(answer2,"Enter pMax, tMax");
      real pMax=1., tMax=1.;
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e",&pMax,&tMax);
      }
      printF("***userDefinedBoundaryValues: pressure pulse: setting pMax=%8.2e, tMax=%8.2e for"
             " (side,axis,grid)=(%i,%i,%i)\n",pMax,tMax,side,axis,grid);

      RealArray values(2);
      values(0)=pMax;
      values(1)=tMax;
      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);

    }
    else if( answer=="cardiac cycle" )
    {
      userDefinedBoundaryValue = "cardiacCycle";

      parameters.setBcIsTimeDependent(side,axis,grid,true);      // this condition is time dependent

      printF("The pressure is p = pMax*sin(pi*t/tP1),  for 0<=t<=tP1, p = pMin*sin(pi*(t-tP1)/tP2) for tP1<=t<=tP1+tP2/2,"
              "p=-pMin other-wise\n"); //note there is only one half period for the second period
      gi.inputString(answer2,"Enter pMax, tP1, pMin, tP2");
      real pMax=1., tP1=1., pMin=-1, tP2=1.;
      if( answer2!="" )
      {
	sScanF(answer2,"%e %e %e %e",&pMax,&tP1,&pMin,&tP2);
      }
      printF("***userDefinedBoundaryValues: cardiac cycle: setting pMax=%8.2e, tP1=%8.2e, pMin=%8.2e, tP2=%8.2e"
             "for (side,axis,grid)=(%i,%i,%i)\n",pMax,tP1,pMin,tP2,side,axis,grid);

      RealArray values(4);
      values(0)=pMax;
      values(1)=tP1;
      values(2)=pMin;
      values(3)=tP2;
      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);
    }
    else if( answer=="polynomial inflow profile" )
    {
      // parameters.setUserBcType(side,axis,grid,pressurePulse);  // set the bcType to be a unique value.
      userDefinedBoundaryValue = "polynomialInflowProfile";

      parameters.setBcIsTimeDependent(side,axis,grid,false);      // this condition is NOT time dependent

      // printF("The pressure pulse is p = .5*pMax*[ 1 - cos(2*pi*t/tMax) ],  for 0 <=t<=tMax, p=0 other-wise\n");
      // gi.inputString(answer2,"Enter pMax, tMax");
      // real pMax=1., tMax=1.;
      // if( answer2!="" )
      // {
      // 	sScanF(answer2,"%e %e",&pMax,&tMax);
      // }
      // printF("***userDefinedBoundaryValues: pressure pulse: setting pMax=%8.2e, tMax=%8.2e for"
      //        " (side,axis,grid)=(%i,%i,%i)\n",pMax,tMax,side,axis,grid);

      real uMax=1.;

      RealArray values(2);
      values(0)=uMax;
      // save the parameters to be used when evaluating the time dependent BC's:
      parameters.setUserBoundaryConditionParameters(side,axis,grid,values);


    }
    else if( answer=="time function option" )
    {
      // parameters.setUserBcType(side,axis,grid,timeFunctionOption);  // set the bcType to be a unique value.
      userDefinedBoundaryValue = "timeFunctionOption";

      parameters.setBcIsTimeDependent(side,axis,grid,true);      // this condition is time dependent

      printF("--UBV-- Info: choose a time variation based from the TimeFunction class\n");

      TimeFunction & timeFunction = dbase.put<TimeFunction>(sPrintF("timeFunctionG%iS%1iA%1i",grid,side,axis));

      // printF(" ---UBV-- bcType =%i\n",(int)parameters.bcType(side,axis,grid));

      timeFunction.update(gi);

      // -- For uniform inflow grab the inflow values from the bcData array --
      RealArray values(numberOfComponents);
      RealArray & bcData = parameters.dbase.get<RealArray>("bcData");
      for( int n=0; n<numberOfComponents; n++ )
      {
	values(n)=bcData(n,side,axis,grid);
	printF("--UBV-- TimeFunction will multiply : n=%i value=%9.3e\n",n,values(n));
      }
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
/// \param gf0 (input) : the current solution.
/// \param grid (input): the component grid we are assigning.
/// \param forcingType (input) : if forcingType==computeForcing then return the rhs for the
///  boundary condition; if forcingType==computeTimeDerivativeOfForcing then return the
///   first time derivative of the forcing.
///
// =========================================================================================
int Cgins::
userDefinedBoundaryValues(const real & t,
                          GridFunction & gf0,
			  const int & grid,
			  int side0 /* = -1 */,
			  int axis0 /* = -1 */,
			  ForcingTypeEnum forcingType /* =computeForcing */)
{
  // printF("***userDefinedBoundaryValues\n");

  realMappedGridFunction & u = gf0.u[grid];
  // realMappedGridFunction & gridVelocity = gf0.getGridVelocity(grid);

  CompositeGrid & cg = gf0.cg;
  MappedGrid & mg = *u.getMappedGrid();

  const int numberOfDimensions = mg.numberOfDimensions();
  const int numberOfComponents = parameters.dbase.get<int>("numberOfComponents");

  assert( side0>=-1 && side0<2 );
  assert( axis0>=-1 && axis0<parameters.dbase.get<int >("numberOfDimensions") );

  const int axisStart= axis0==-1 ? 0 : axis0;
  const int axisEnd  = axis0==-1 ? parameters.dbase.get<int >("numberOfDimensions")-1 : axis0;
  const int sideStart= side0==-1 ? 0 : side0;
  const int sideEnd  = side0==-1 ? 1 : side0;

  int numberOfSidesAssigned=0;

  Range C(0,numberOfComponents-1);
  const int pc=parameters.dbase.get<int >("pc");
  const int uc=parameters.dbase.get<int >("uc");
  const int vc=parameters.dbase.get<int >("vc");
  const int wc=parameters.dbase.get<int >("wc");
  const int tc=parameters.dbase.get<int >("tc");

  const bool gridIsMoving = parameters.gridIsMoving(grid); // true if the grid is moving

  // uncomment the next two lines if you want the grid points.
  // c.update(MappedGrid::THEvertex);
  // const realArray & vertex = c.vertex();  // here is the array of grid points

  OV_GET_SERIAL_ARRAY(real,u,uLocal);
  int includeGhost=1;


  // -- Retrieve the known solution ----
  const Parameters::KnownSolutionsEnum & knownSolution =
            parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution");

  realArray *uKnownPointer=NULL;
  if( knownSolution!=Parameters::noKnownSolution )
  {
    int extra=2;
    Index I1,I2,I3;
    getIndex(mg.gridIndexRange(),I1,I2,I3,extra);

    uKnownPointer = &parameters.getKnownSolution( t,grid,I1,I2,I3 );
  }
  realArray & uKnown = uKnownPointer!=NULL ? *uKnownPointer : u;

  OV_GET_SERIAL_ARRAY(real,uKnown,uKnownLocal);


  int axis;
  Index Ib1,Ib2,Ib3;

  for( axis=axisStart; axis<=axisEnd; axis++ )
  {
    for( int side=sideStart; side<=sideEnd; side++ )
    {
      const aString userDefinedBoundaryValueName=sPrintF("userBV_G%i_S%i_A%i",grid,side,axis); // unique name for this (grid,side,axis)
      if( !parameters.dbase.has_key(userDefinedBoundaryValueName) )
	continue; // no user defined option

      const aString & userDefinedBoundaryValue = parameters.dbase.get<aString>(userDefinedBoundaryValueName);

      /////////////////////////////////////////////////////////////////
      // **Added QC**
      /////////////////////////////////////////////////////////////////
      if (userDefinedBoundaryValue=="externalHeatValues")
      {
        numberOfSidesAssigned++;
        // get the index FIXME ghost line needed?
        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
        bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
      	if( !ok ) continue;
        const int sz = Ib1.length()*Ib2.length()*Ib3.length();
        RealArray values(sz);

        parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

        // get the boundary data
        RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

        // assume no slip walls with no mesh motion
        Range C(uc, uc+numberOfDimensions-1);
        bd(Ib1, Ib2, Ib3, C) = 0.0;

        int i1, i2, i3, count = 0;
        // NOTE that we expect the external data loop through x, then y
        // finally z, this aligns with the macro FOR_3D
        // tc is temperature component number
        FOR_3D(i1, i2, i3, Ib1,Ib2,Ib3)
        {
          bd(i1, i2, i3, tc) = values(count);
          ++count;
        }
      }
      else if (userDefinedBoundaryValue=="externalRobinCoeffs")
      {
        numberOfSidesAssigned++;
        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
        bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
      	if( !ok ) continue;
        const int sz = Ib1.length()*Ib2.length()*Ib3.length();
        RealArray values(2*sz); // agian half for temp, half for h
        parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

        // The formula reads
        //      kappa * dT/dn + h * (T - T_inf) = 0
        //  =>  kappa * dT/dn + h * T = h * T_inf
        // where h is the heat transfer coeff from other side, T_inf is
        // the so-called ambient tempereature.
        // For Overture, this is called mixed BC with the general formulation
        // of a1 * dT/dn + a2 * T = g
        // So in our case,
        //    a1 = kappa
        //    a2 = h
        //    g = h * T_inf

        const real kappa = parameters.dbase.get<real>("thermalConductivity");

        // get the boundary data
        RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
        // assume no slip walls with no mesh motion
        Range C(uc, uc+numberOfDimensions-1);
        bd(Ib1, Ib2, Ib3, C) = 0.0;

        int i1, i2, i3, count_temp = 0, count_h = sz;
        // NOTE that we expect the external data loop through x, then y
        // finally z, this aligns with the macro FOR_3D
        // tc is temperature component number
        FOR_3D(i1, i2, i3, Ib1,Ib2,Ib3)
        {
          bd(i1, i2, i3, tc) = values(count_temp)*values(count_h);
          ++count_temp;
          ++count_h;
        }

        // NOTE that bd stores the rhs g

        BoundaryData &BD = parameters.dbase.get<std::vector<BoundaryData> >("boundaryData")[grid];

        RealArray &varCoeff = BD.getVariableCoefficientBoundaryConditionArray(
          BoundaryData::variableCoefficientTemperatureBC, side, axis);

        count_h = sz;

        FOR_3(i1,i2,i3,Ib1,Ib2,Ib3)
        {
          varCoeff(i1,i2,i3,0) = kappa;
          varCoeff(i1,i2,i3,1) = values(count_h);
          ++count_h;
        }
      }
      /////////////////////////////////////////////////////////////////
      // **Finished QC**
      /////////////////////////////////////////////////////////////////
      else if( userDefinedBoundaryValue=="variableInflow" )
      {
        RealArray values(3);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);
        // printF("***userDefinedBoundaryValues: assign (side,axis,grid)=(%i,%i,%i) variableInflow force=%i, "
        //        "values=%f,%f,%f\n",side,axis,grid,(int)forcingType,values(0),values(1),values(2));

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	// kkc 070130 BILL : this section of code does nothing if mg.bC != inflowWithVelocityGiven, so right now
	//                   I assume this is only called in that instance obviating the need for this check
	//        if( mg.boundaryCondition(side,axis)==Parameters::inflowWithVelocityGiven )
	//	{
	// assign values for inflow velocity: The first 3 components are the values of (u,v,w) on the boundary.
	numberOfSidesAssigned++;

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
	real u0=values(0);
	real v0=values(1);
	real w0=values(2);
	// just set to constant values for now.
	bd(Ib1,Ib2,Ib3,uc)=u0;
	bd(Ib1,Ib2,Ib3,vc)=v0;
	if( numberOfDimensions>2 )
	  bd(Ib1,Ib2,Ib3,wc)=w0;
      }
      else if( userDefinedBoundaryValue=="timeDependentInflow" )
      {

        RealArray values(3);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

        // printF("***userDefinedBoundaryValues: assign (side,axis,grid)=(%i,%i,%i) timeDependentInflow force=%i, "
        //        "values=%f,%f,%f\n",side,axis,grid,(int)forcingType,values(0),values(1),values(2));

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	// kkc 070130 BILL : this section of code does nothing if mg.bC != inflowWithVelocityGiven, so right now
	//                   I assume this is only called in that instance obviating the need for this check

	//        if( mg.boundaryCondition(side,axis)==Parameters::inflowWithVelocityGiven )
	//	{
	// printF("***userDefinedBoundaryValues: assign (side,axis,grid)=(%i,%i,%i) timeDependentInflow force=%i,"
	//	 " values=%f,%f,%f\n",side,axis,grid,(int)forcingType,values(0),values(1),values(2));

	// assign values for inflow velocity: The first 3 components are the values of (u,v,w) on the boundary.
	numberOfSidesAssigned++;

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	real u0=values(0);
	real v0=values(1);
	real w0=values(2);

	if( parameters.bcType(side,axis,grid)==Parameters::parabolicInflowUserDefinedTimeDependence )
	{
	  // give time dependence to to a parabolic inflow profile.
	  real factor;
	  if( forcingType==computeForcing )
	    factor=min(1.,t*2.);  // ramp values for u in time.
	  else
	    factor=t<.5 ? 2.*u0 : 0.;  // time derivative of the forcing

	  // the parabolic profile is saved in the ghost line of the "bd" array.
	  Index Ig1,Ig2,Ig3;
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
  	  ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);

	  Range C(uc,uc+numberOfDimensions-1);
	  bd(Ib1,Ib2,Ib3,C)= factor*bd(Ig1,Ig2,Ig3,C);

	}
	else
	{
	  if( forcingType==computeForcing )
	    bd(Ib1,Ib2,Ib3,uc)=u0*min(1.,t*2.);  // ramp values for u in time.
	  else
	    bd(Ib1,Ib2,Ib3,uc)=t<.5 ? 2.*u0 : 0.;  // time derivative of the forcing

	  bd(Ib1,Ib2,Ib3,vc)=v0;
	  if( numberOfDimensions>2 )
	    bd(Ib1,Ib2,Ib3,wc)=w0;
	}

	//	}
      }
      else if( userDefinedBoundaryValue=="perturbedShearFlow" )
      {

	// kkc 070130 BILL : this section of code does nothing if mg.bC != inflowWithVelocityGiven, so right now
	//                   I assume this is only called in that instance obviating the need for this check
	//        if( mg.boundaryCondition(side,axis)==Parameters::inflowWithVelocityGiven )
	//	{

	// printF("***userDefinedBoundaryValues: assign (side,axis,grid)=(%i,%i,%i) timeDependentInflow force=%i, "
	//        "values=%f,%f,%f\n",side,axis,grid,(int)forcingType,values(0),values(1),values(2));

	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	// printF("***userDefinedBoundaryValues: assign (side,axis,grid)=(%i,%i,%i) timeDependentInflow force=%i,"
	//	 " values=%f,%f,%f\n",side,axis,grid,(int)forcingType,values(0),values(1),values(2));

	numberOfSidesAssigned++;
	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	RealArray values(6);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);
	real u1  =values(0);
	real u2  =values(1);
	real beta=values(2);
	real y0  =values(3);
	real f0  =values(4);
	real amp =values(5);

	if( t==0. )
	  printF(" ASSIGN perturbed shear flow: u1=%8.2e u2=%8.2e beta=%8.2e y0=%8.2e f0=%8.2e amp=%8.2e\n",
		 u1,u2,beta,y0,f0,amp);


	Range C(uc,uc+numberOfDimensions-1);
	bd(Ib1,Ib2,Ib3,C)=0.;
#define STIME(t) ( 1.+ amp*( .5*sin(2.*Pi*f0*(t))+(1./3.)*sin(3.*Pi*f0*(t))+.25*sin(4.*Pi*f0*(t))+\
                     .2*sin(5.*Pi*f0*(t))) )
#define STIME_T(t) ( amp*Pi*f0*( cos(2.*Pi*f0*(t))+cos(3.*Pi*f0*(t))+cos(4.*Pi*f0*(t))+cos(5.*Pi*f0*(t))) )

#define SHEAR(y,t) ((u1+u2)*.5+(u1-u2)*.5*tanh(beta*((y)-y0)))*STIME(t)
#define SHEAR_T(y,t) ((u1+u2)*.5+(u1-u2)*.5*tanh(beta*((y)-y0)))*STIME_T(t)
	if( mg.isRectangular() )
	{
	  real dx[3],xab[2][3];
	  mg.getRectangularGridParameters( dx, xab );
	  const int i0a=mg.gridIndexRange(0,0);
	  const int i1a=mg.gridIndexRange(0,1);
	  const int i2a=mg.gridIndexRange(0,2);

	  const real xa=xab[0][0], dx0=dx[0];
	  const real ya=xab[0][1], dy0=dx[1];
	  const real za=xab[0][2], dz0=dx[2];

#define X0(i0,i1,i2) (xa+dx0*(i0-i0a))
#define X1(i0,i1,i2) (ya+dy0*(i1-i1a))
#define X2(i0,i1,i2) (za+dz0*(i2-i2a))

	  RealArray y(1,Ib2);
	  y(0,Ib2).seqAdd(X1(i0a,Ib2.getBase(),i2a),dy0);
	  if( forcingType==computeForcing )
	    bd(Ib1,Ib2,Ib3,uc)=SHEAR(y,t);
	  else
	    bd(Ib1,Ib2,Ib3,uc)=SHEAR_T(y,t);
	}
	else
	{
#ifndef USE_PPP
	  mg.update(MappedGrid::THEvertex);
#else
	  OV_ABORT("mg.update may contain parallel call; may need fix");
#endif
	  const RealArray & vertex = mg.vertex().getLocalArray(); // no need to use if rectangular ************

	  if( forcingType==computeForcing )
	    bd(Ib1,Ib2,Ib3,uc)=SHEAR(vertex(Ib1,Ib2,Ib3,axis2),t);
	  else
	    bd(Ib1,Ib2,Ib3,uc)=SHEAR_T(vertex(Ib1,Ib2,Ib3,axis2),t);

	}
	//	}
      }
      else if( userDefinedBoundaryValue=="wallWithScalarFlux" )
      {
        if( mg.boundaryCondition(side,axis)==Parameters::noSlipWall ||
            mg.boundaryCondition(side,axis)==Parameters::slipWall )
	{

	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	  // printF("***userDefinedBoundaryValues: assign (side,axis,grid)=(%i,%i,%i) timeDependentInflow force=%i,"
	  //	 " values=%f,%f,%f\n",side,axis,grid,(int)forcingType,values(0),values(1),values(2));

	  numberOfSidesAssigned++;

	  bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	  if( !ok ) continue;  // no points on this processor

          RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	  RealArray values(5);
	  parameters.getUserBoundaryConditionParameters(side,axis,grid,values);
	  real amp=    values(0);
	  real radius= values(1);
	  real x0=     values(2);
	  real y0=     values(3);
	  real z0=     values(4);

          if( true )
	    printF(" ASSIGN wallWithScalarFlux: amp=%8.2e, radius=%8.2e, x=(%8.2e,%8.2e,%8.2e)\n",
		   amp,radius,x0,y0,z0);

	  Range V(uc,uc+numberOfDimensions-1);
	  bd(Ib1,Ib2,Ib3,V)=0.;  // set values for the velocity components on the wall

          // Now set the RHS for the scalar BC:
          //     s.n = rhs
	  const int sc=parameters.dbase.get<int >("sc");
          assert( sc>=0 );

	  const RealArray & vertex = mg.vertex().getLocalArray();
          RealArray rad(Ib1,Ib2,Ib3);
          if( mg.numberOfDimensions()==2 )
  	    rad = SQR(vertex(Ib1,Ib2,Ib3,0)-x0)+SQR(vertex(Ib1,Ib2,Ib3,1)-y0);
          else
  	    rad = SQR(vertex(Ib1,Ib2,Ib3,0)-x0)+SQR(vertex(Ib1,Ib2,Ib3,1)-y0)+SQR(vertex(Ib1,Ib2,Ib3,2)-z0);

          // note: rhs for neumann BC is filled into the bd array for points on the boundary
          bd(Ib1,Ib2,Ib3,sc)=0.;
          where( rad<SQR(radius) )
	  {
	    bd(Ib1,Ib2,Ib3,sc)=amp;
	  }

	}

      }
      else if( userDefinedBoundaryValue=="axisymmetricRotation" )
      {
        RealArray values(5);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);
	printF("***userDefinedBoundaryValues: assign (side,axis,grid)=(%i,%i,%i) axisymmetric rotation =%i, "
	       "values=%f,%f,%f,%f,%f\n",side,axis,grid,(int)forcingType,values(0),values(1),values(2),values(3),values(4));

        const int extra=1;  // include ghost
        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extra);
	//        getBoundaryIndex(mg.extendedIndexRange(),side,axis,Ib1,Ib2,Ib3,extra);

	if ( mg.isPeriodic((axis+1)%2) )
	{
	  if ( axis==1 )
	    Ib1 = Index(Ib1.getBase(), Ib1.getLength()-1);
	  else
	    Ib2 = Index(Ib2.getBase(), Ib2.getLength()-1);
	}
	// assign values for inflow velocity: The first 3 components are the values of (u,v,w) on the boundary.
	numberOfSidesAssigned++;

	mg.update(MappedGrid::THEvertex);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

        const RealArray & vertex = mg.vertex().getLocalArray();

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	real odot=values(0);
	real u0=values(1);
	real v0=values(2);
	real te0=values(3);
	real trmp=values(4);

	// just set to constant values for now.
	real f = trmp>REAL_MIN ? min(1.,t/trmp) : 1;
	f = f*f*f;
	bd(Ib1,Ib2,Ib3,uc)=f*u0;
	bd(Ib1,Ib2,Ib3,vc)=f*v0;
	if( wc>=0 )
	{
	  bd(Ib1,Ib2,Ib3,wc)= f*odot*vertex(Ib1,Ib2,Ib3,1); // XXX assumed radial coordinate is 1!!!
	}

	if( tc>=0 )
	{
          // printF(" ********** axisymmetricRotation BC: set T = %8.2e\n",te0);
	  if ( te0>0. )
	  {
	    bd(Ib1,Ib2,Ib3,tc)=f*te0 + (1-f)*uLocal(Ib1,Ib2,Ib3,tc);
	  }
	  else
	  {
	    bd(Ib1,Ib2,Ib3,tc)= uLocal(Ib1,Ib2,Ib3,tc);
	  }
	}
      }
      else if( userDefinedBoundaryValue=="linearRampInX" )
      {
        RealArray values(3*numberOfComponents);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);
        // printF("***userDefinedBoundaryValues: assign (side,axis,grid)=(%i,%i,%i) variableInflow force=%i, "
        //        "values=%f,%f,%f\n",side,axis,grid,(int)forcingType,values(0),values(1),values(2));

        const int extra=1;  // include ghost
        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extra);
	if ( mg.isPeriodic((axis+1)%2) )
	{
	  if ( axis==1 )
	    Ib1 = Index(Ib1.getBase(), Ib1.getLength()-1);
	  else
	    Ib2 = Index(Ib2.getBase(), Ib2.getLength()-1);
	}
	// assign values for inflow velocity: The first 3 components are the values of (u,v,w) on the boundary.
	numberOfSidesAssigned++;

	mg.update(MappedGrid::THEvertex);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

        const RealArray & vertex = mg.vertex().getLocalArray();

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
	for ( int c=0; c<numberOfComponents; c++ )
	{
	  real f0 = values(3*c+0);
	  real slope=values(3*c+1);
	  real trmp = values(3*c+2);

	  // just set to constant values for now.
	  if ( trmp>=0. )
	  {
	    real f = trmp>REAL_MIN ? min(1.,t/trmp) : 1;
	    f = f*f*f;

	    bd(Ib1,Ib2,Ib3,c) = f*(vertex(Ib1,Ib2,Ib3,0)*slope + f0) + (1-f)*uLocal(Ib1,Ib2,Ib3,c);
	  }
	  else
	    bd(Ib1,Ib2,Ib3,c) = uLocal(Ib1,Ib2,Ib3,c);

	}
      }
      else if( userDefinedBoundaryValue=="linearRampInY" )
      {
        RealArray values(3*numberOfComponents);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);
        // printF("***userDefinedBoundaryValues: assign (side,axis,grid)=(%i,%i,%i) variableInflow force=%i, "
        //        "values=%f,%f,%f\n",side,axis,grid,(int)forcingType,values(0),values(1),values(2));

        const int extra=1;  // include ghost
        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extra);
	if ( mg.isPeriodic((axis+1)%2) )
	{
	  if ( axis==1 )
	    Ib1 = Index(Ib1.getBase(), Ib1.getLength()-1);
	  else
	    Ib2 = Index(Ib2.getBase(), Ib2.getLength()-1);
	}
	// assign values for inflow velocity: The first 3 components are the values of (u,v,w) on the boundary.
	numberOfSidesAssigned++;

	mg.update(MappedGrid::THEvertex);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor


        const RealArray & vertex = mg.vertex().getLocalArray();

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
	for ( int c=0; c<numberOfComponents; c++ )
	{
	  real f0 = values(3*c+0);
	  real slope=values(3*c+1);
	  real trmp = values(3*c+2);

	  // just set to constant values for now.
	  if ( trmp>=0. )
	  {
	    real f = trmp>REAL_MIN ? min(1.,t/trmp) : 1;
	    f = f*f*f;

	    bd(Ib1,Ib2,Ib3,c) = f*(vertex(Ib1,Ib2,Ib3,1)*slope + f0) + (1-f)*uLocal(Ib1,Ib2,Ib3,c);
	  }
	  else
	    bd(Ib1,Ib2,Ib3,c) = uLocal(Ib1,Ib2,Ib3,c);

	}
      }
      else if(userDefinedBoundaryValue=="ablProfile" )
      {

	RealArray values(4);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

	real u_ref, z_ref, alpha,d;
	u_ref = values(0);
	z_ref = values(1);
	alpha = values(2);
	d     = values(3);
	printF("***userDefinedBoundaryValues: assign (side,axis,grid)=(%i,%i,%i) abl profile = %f %f %f %e\n",
	       side,axis,grid,u_ref,z_ref,alpha,d);

	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	numberOfSidesAssigned++;

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	Range C(uc,uc+numberOfDimensions-1);
	bd(Ib1,Ib2,Ib3,C) = 0.;

#define X0(i0,i1,i2) (xa+dx0*(i0-i0a))
#define X1(i0,i1,i2) (ya+dy0*(i1-i1a))
#define X2(i0,i1,i2) (za+dz0*(i2-i2a))

	  //	  real d=1e-3;
	if( mg.isRectangular() )
	{
	  real dx[3], xab[2][3];
	  mg.getRectangularGridParameters(dx,xab);
	  const int i0a = mg.gridIndexRange(0,0);
	  const int i1a = mg.gridIndexRange(0,1);
	  const int i2a = mg.gridIndexRange(0,2);

	  const real xa = xab[0][0], dx0 = dx[0];
	  const real ya = xab[0][1], dy0 = dx[1];
	  const real za = xab[0][2], dz0 = dx[2];

	  for ( int ib3=Ib3.getBase(); ib3<=Ib3.getBound(); ib3++ )
	    for ( int ib2=Ib2.getBase(); ib2<=Ib2.getBound(); ib2++ )
	      for ( int ib1=Ib1.getBase(); ib1<=Ib1.getBound(); ib1++ )
	      {
		real scale = X1(ib1,ib2,ib3)>d ? 1 : X1(ib1,ib2,ib3)*(2*d-X1(ib1,ib2,ib3))/(d*d);
		bd(ib1,ib2,ib3,uc) = scale*u_ref * pow(X1(ib1,ib2,ib3)/z_ref,alpha);
	      }
	}
	else
	{
#ifndef USE_PPP
	  mg.update(MappedGrid::THEvertex);
#else
	  OV_ABORT("mg.update may contain parallel call; may need fix");
#endif
	  const RealArray & vertex = mg.vertex().getLocalArray();
	  //	      RealArray scale(Ib1,Ib2,Ib3);

	  for ( int ib3=Ib3.getBase(); ib3<=Ib3.getBound(); ib3++ )
	    for ( int ib2=Ib2.getBase(); ib2<=Ib2.getBound(); ib2++ )
	      for ( int ib1=Ib1.getBase(); ib1<=Ib1.getBound(); ib1++ )
	      {
		real scale = vertex(ib1,ib2,ib3,1)>d ? 1 : vertex(ib1,ib2,ib3,1)*(2*d-vertex(ib1,ib2,ib3,1))/(d*d);
		bd(ib1,ib2,ib3,uc) = scale*u_ref * pow(abs(vertex(ib1,ib2,ib3,1))/z_ref,alpha);
	      }
	  //	      bd(Ib1,Ib2,Ib3,uc) = u_ref * pow(abs(vertex(Ib1,Ib2,Ib3,1))/z_ref,alpha);
	}


      }
      else if( userDefinedBoundaryValue=="variableTemperature" )
      {
	// -- assign a variable T on a wall --

	numberOfSidesAssigned++;

	RealArray values(3);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

	real q1, q2, y0;
	q1 = values(0);
	q2 = values(1);
	y0 = values(2);
	printF("***userDefinedBoundaryValues: assign (side,axis,grid)=(%i,%i,%i) variable T: T1=%f, T2=%f, y0=%f\n",
	       side,axis,grid,q1,q2,y0);

	int extra=0; // **MODIFIED by QC for testing ghost line
	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extra);

	mg.update(MappedGrid::THEvertex);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	Range C(uc,uc+numberOfDimensions-1);
	bd(Ib1,Ib2,Ib3,C) = 0.;

	assert( tc>=0 );

	// *** fix me for rectangular ***
	OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),vertex);

	int i1,i2,i3;
	FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	{
	  if( vertex(i1,i2,i3,1)>y0 )
	  {
	    bd(i1,i2,i3,tc)=q1;
	  }
	  else
	  {
	    bd(i1,i2,i3,tc)=q2;
	  }

	}
      }
      else if( userDefinedBoundaryValue=="boundaryDataFromAFile" )
      {
	ExternalBoundaryData & ebd = *parameters.dbase.get<ExternalBoundaryData*>("externalBoundaryData");
	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
	ebd.getBoundaryData( t, cg, side, axis, grid, bd );

      }
      else if( userDefinedBoundaryValue=="inflowWithControl" )
      {

        RealArray values(3);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

	real u0=values(0);
	real v0=values(1);
	real w0=values(2);

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	// assign values for inflow velocity: The first 3 components are the values of (u,v,w) on the boundary.
	numberOfSidesAssigned++;

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);


	// Check that the Controller exists:
	if( !parameters.dbase.has_key("Controller") )
	{
	  printF("userDefinedBoundaryValues:ERROR: The Controller does not exist!\n");
	  OV_ABORT("ERROR");
	}

	Controller & controller = parameters.dbase.get<Controller>("Controller");

        // Evaluate the control function:
        real uControl=0., uControlDot=0.;
        // const real dt = parameters.dbase.get<real>("dt");
        // controller.getControl( gf0.u,t,dt,uControl,uControlDot );
        controller.getControl( t,uControl,uControlDot );

        printF("***userDefinedBoundaryValues: inflowWithControl: t=%9.3e, (side,axis,grid)=(%i,%i,%i),"
	       " force=%i, (u,v,w)=(%g,%g,%g) uControl=%g, uControlDot=%g.\n",t,side,axis,grid,(int)forcingType,
               u0,v0,w0,uControl,uControlDot);


	real factor=1.;
	if( forcingType==computeForcing )
	  factor=1.;  // return boundary values
	else
	  factor=0.;  // return time derivative of the boundary values
        Range C(uc,uc+numberOfDimensions-1);

//         Integrate *pIntegrate = parameters.dbase.get<Integrate*>("integrate");
//         assert( pIntegrate!=NULL );
// 	Integrate & integrate = *pIntegrate;

// 	// compute the average temperature on the domain
// //         // **FIX ME ** we should be able to compute the volume integrate of a component *********
// //         realCompositeGridFunction ucg(gf0.cg); // holds tc
// //         for( int grid=0; grid<gf0.cg.numberOfComponentGrids(); grid++ )
// // 	{
// // 	  Range all;
// //           assign( ucg[grid], gf0.u[grid](all,all,all,tc) );
// // 	}
// // 	real TintegralOld = integrate.volumeIntegral( ucg );
// 	real Tintegral = integrate.volumeIntegral( gf0.u,tc );
//         real Tbar = Tintegral/regionVolume;
// 	printF(" Tintegral=%9.3e, regionVolume=%8.2e, Tbar = %9.3e\n",
//                Tintegral,regionVolume,Tbar);



// 	real Tdot = -K*( Tbar - Tset );

//         if( t > 0. )
//           Tcontrol = Tcontrol + Tdot*dt;

// 	printF(" -> t=%9.3e, dt=%8.2e: Tcontrol=%f, Tbar=%f, Tset=%f, Tdot=%f\n",t,dt,Tcontrol,Tbar,Tset,Tdot);

	if( parameters.bcType(side,axis,grid)==Parameters::parabolicInflowUserDefinedTimeDependence )
	{
	  // give time dependence to a parabolic inflow profile.

	  // the parabolic profile is saved in the ghost line of the "bd" array.
	  Index Ig1,Ig2,Ig3;
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
  	  ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);

	  bd(Ib1,Ib2,Ib3,C)= factor*bd(Ig1,Ig2,Ig3,C);
	  if( tc>=0 )
	  {
            if( forcingType==computeForcing )
	      bd(Ib1,Ib2,Ib3,tc)= uControl*bd(Ig1,Ig2,Ig3,tc);
	    else
	      bd(Ib1,Ib2,Ib3,tc)= uControlDot*bd(Ig1,Ig2,Ig3,tc);
	  }
	}
	else
	{
	  bd(Ib1,Ib2,Ib3,uc)=u0*factor;
	  bd(Ib1,Ib2,Ib3,vc)=v0*factor;
	  if( numberOfDimensions>2 )
	    bd(Ib1,Ib2,Ib3,wc)=w0*factor;

	  if( tc>=0 )
	  {
            if( forcingType==computeForcing )
	      bd(Ib1,Ib2,Ib3,tc)= uControl;
	    else
	      bd(Ib1,Ib2,Ib3,tc)= uControlDot;
	  }
	}


      }
      else if( userDefinedBoundaryValue=="normalComponentOfVelocity" )
      {
	// -- assign the normal component of the velocity (and Temperature for Boussinesq) --

	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	// assign values for inflow velocity: The first 3 components are the values of (u,v,w) on the boundary.
	numberOfSidesAssigned++;

	RealArray values(3);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

	real un = values(0);     // normal component of the velocity
	real inflowTemperature=values(1);  // Temperature at inflow

	// Here is the (outward) normal -- We could optimize this for rectangular grids --
        #ifdef USE_PPP
	  OV_ABORT("mg.update may contain parallel call; may need fix");
	  const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
        #else
	  mg.update(MappedGrid::THEvertexBoundaryNormal);
	  const realSerialArray & normal = mg.vertexBoundaryNormal(side,axis);
        #endif

	//
        Range V(uc,uc+numberOfDimensions-1), Rx=numberOfDimensions;
        RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	// NOTE: normal is outward so switch the sign of un:
	bd(Ib1,Ib2,Ib3,V)= (-un)*normal(Ib1,Ib2,Ib3,Rx);

	if( tc>=0 )
	  bd(Ib1,Ib2,Ib3,tc)=inflowTemperature;

      }
      else if( userDefinedBoundaryValue=="cylindricalVelocity" )
      {
	// -- assign the cylindrical components of velocity --

        // -- we could avoid building the vertex array on Cartesian grids ---
	mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
        OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);

	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	// assign values for inflow velocity: The first 3 components are the values of (u,v,w) on the boundary.
	numberOfSidesAssigned++;

	RealArray values(10);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

	const real vr    =values(0);
	const real vTheta=values(1);
	const real vPhi  =values(2);
	const real tb    =values(3);
	const real x0    =values(4);
	const real x1    =values(5);
	const real x2    =values(6);
	const real d0    =values(7);
	const real d1    =values(8);
	const real d2    =values(9);

        RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

        // vTheta = vHat.v = (-sin,cos)*v
        // vr     = rHat.v = ( cos,sin)*v
        // vPhi   = phiHat.v
        // v = vr*rHat + vTheta*thetaHat + vPhi*phiHat
	RealArray r(Ib1,Ib2,Ib3), ct(Ib1,Ib2,Ib3), st(Ib1,Ib2,Ib3);

        // We have only implemented some cases:
        int axialAxis=-1;
	if( d0==1. && d1==0. && d2==0. )
          axialAxis=0;
	else if( d0==0. && d1==1. && d2==0. )
          axialAxis=1;
	else if( d0==0. && d1==0. && d2==1. )
          axialAxis=2;
	else
	{
          OV_ABORT("finish me");
	}

        assert( x0==0. && x1==0. && x2==0. );
        // assert( d0==0. && d1==0. && d2==1. );
        assert( vr==0. && vPhi==0. );


	const int axisp1 = (axialAxis+1) % 3;
	const int axisp2 = (axialAxis+2) % 3;

	r = sqrt( SQR(x(Ib1,Ib2,Ib3,axisp1)) + SQR(x(Ib1,Ib2,Ib3,axisp2)) );
        ct = x(Ib1,Ib2,Ib3,axisp1)/r; // cos(theta)
        st = x(Ib1,Ib2,Ib3,axisp2)/r; // sin(theta)

	if( forcingType==computeForcing )
	{
	  bd(Ib1,Ib2,Ib3,uc+axisp1)=(-vTheta)*st;
	  bd(Ib1,Ib2,Ib3,uc+axisp2)=( vTheta)*ct;
	  if( numberOfDimensions==3 )
	    bd(Ib1,Ib2,Ib3,uc+axialAxis)=vPhi;

	  if( tc>=0 )
	    bd(Ib1,Ib2,Ib3,tc)=tb;
	}
	else
	{
	  // time derivative of the forcing
	  Range V(uc,uc+numberOfDimensions-1);
	  bd(Ib1,Ib2,Ib3,V)=0.;
	  if( tc>=0 )
	    bd(Ib1,Ib2,Ib3,tc)=0.;
	}

      }
      else if( userDefinedBoundaryValue=="pressureProfile" )
      {
	// -- define a pressure profile (e.g. for an outflow BC ----

        // -- we could avoid building the vertex array on Cartesian grids ---
	mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
        OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);


	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	numberOfSidesAssigned++;

	RealArray values(10);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

	const real p0    =values(0);
	const real p1    =values(1);
	const real y0    =values(2);
	const real y1    =values(3);

	printF("userDefinedBoundaryValues: pressureProfile: p0=%g, p1=%g, y0=%g, y1=%g (t=%9.3e)\n",p0,p1,y0,y1,t);
	// ::display(x(Ib1,Ib2,Ib3,1),"Here is x on the side");

        RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	if( forcingType==computeForcing )
	{
	  bd(Ib1,Ib2,Ib3,pc)=(p0/(y0-y1))*(x(Ib1,Ib2,Ib3,1)-y1)+ (p1/(y1-y0))*(x(Ib1,Ib2,Ib3,1)-y0);
	}
	else
	{
	  // time derivative of the forcing
	  bd(Ib1,Ib2,Ib3,pc)=0.;
	}

      }
      else if( userDefinedBoundaryValue=="knownSolutionValues" )
      {
	// -- assign boundary values from the known solution ----

	if( debug() & 4 || t<2*dt )
	  printF("--UBV-- userDefinedBoundaryValues:knownSolutionValues: eval solution: t=%8.2e (side,axis,grid)=(%i,%i,%i) forcingType=%i\n",t,
		 side,axis,grid,(int)forcingType);

        // -- we could avoid building the vertex array on Cartesian grids ---
	mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
        OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);


	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	numberOfSidesAssigned++;


	// printF("userDefinedBoundaryValues: assign known solution values (t=%9.3e)\n",t);
	// ::display(x(Ib1,Ib2,Ib3,1),"Here is x on the side");

        RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);


	if( forcingType==computeForcing )
	{
	  bd(Ib1,Ib2,Ib3,pc)=uKnownLocal(Ib1,Ib2,Ib3,pc);
	  bd(Ib1,Ib2,Ib3,uc)=uKnownLocal(Ib1,Ib2,Ib3,uc);
	  bd(Ib1,Ib2,Ib3,vc)=uKnownLocal(Ib1,Ib2,Ib3,vc);
	  if( numberOfDimensions==3 )
	    bd(Ib1,Ib2,Ib3,wc)=uKnownLocal(Ib1,Ib2,Ib3,wc);

	}
	else
	{
	  // time derivative of the forcing  *fix me for time-dependent known solutions*


	  int numberOfTimeDerivatives=1; //
          const Parameters::KnownSolutionsEnum & knownSolution = parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution");
	  if( knownSolution==Parameters::userDefinedKnownSolution )
	  {
#ifndef USE_PPP
	    parameters.getUserDefinedKnownSolution( t,cg, grid, bd, Ib1,Ib2,Ib3, numberOfTimeDerivatives );
#else
	    OV_ABORT("FIX ME FOR PARALLEL");
#endif
	  }
	  else
	  {
	    // finish me
	    bd(Ib1,Ib2,Ib3,pc)=0.;
	    bd(Ib1,Ib2,Ib3,uc)=0.;
	    bd(Ib1,Ib2,Ib3,vc)=0.;
	    if( numberOfDimensions==3 )
	      bd(Ib1,Ib2,Ib3,wc)=0.;
	  }

	}

      }
      else if( userDefinedBoundaryValue=="flatPlateBoundaryLayerProfile" )
      {
	// --- flat plat boundary layer profile ---

        // -- we could avoid building the vertex array on Cartesian grids ---
	mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
        OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);


	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	numberOfSidesAssigned++;

	RealArray values(3);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

	const real U       =values(0);
	const real xOffset =values(1);
	const real nuBL    =values(2);

        DataBase & db = parameters.dbase;
	if( !db.has_key("BoundaryLayerProfile") )
	{
	  // Create the BoundaryLayerProfile object
	  db.put<BoundaryLayerProfile*>("BoundaryLayerProfile");
	  db.get<BoundaryLayerProfile*>("BoundaryLayerProfile") = new BoundaryLayerProfile();  // who will delete ???

	  BoundaryLayerProfile & profile = *db.get<BoundaryLayerProfile*>("BoundaryLayerProfile");
	  // const real nu = parameters.dbase.get<real>("nu");
	  profile.setParameters( nuBL,U );  //

	}
	BoundaryLayerProfile & profile = *db.get<BoundaryLayerProfile*>("BoundaryLayerProfile");

        // -- fill in the bd array:
        RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	printF("\n &&&& userDefinedBoundaryValues: INFO: assign flat plate boundary layer profile at t=%8.2e\n\n",t);

	if( forcingType==computeForcing )
	{
	  int i1,i2,i3;
	  FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	  { // evaluate the boundary layer profile
	    profile.eval( x(i1,i2,i3,0)+xOffset,x(i1,i2,i3,1), bd(i1,i2,i3,uc), bd(i1,i2,i3,vc) );
	  }

	  bd(Ib1,Ib2,Ib3,pc)=uKnownLocal(Ib1,Ib2,Ib3,pc);
	  if( numberOfDimensions==3 )
	    bd(Ib1,Ib2,Ib3,wc)=uKnownLocal(Ib1,Ib2,Ib3,wc);

	}
	else
	{
	  // time derivative of the forcing
	  bd(Ib1,Ib2,Ib3,pc)=0.;
	  bd(Ib1,Ib2,Ib3,uc)=0.;
	  bd(Ib1,Ib2,Ib3,vc)=0.;
	  if( numberOfDimensions==3 )
	    bd(Ib1,Ib2,Ib3,wc)=0.;
	}

      }

      else if( userDefinedBoundaryValue=="pressurePulse" )
      {

        RealArray values(2);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);
	real pMax=values(0);
	real tMax=values(1);

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

        // -- we could avoid building the vertex array on Cartesian grids ---
	mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

        OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);


	numberOfSidesAssigned++;

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
        real factor=0.;
	if( forcingType==computeForcing )
	{
          if( t>=0. && t<=tMax )
	  {
	    // factor=.5*pMax*( 1.-cos(twoPi*t/tMax) );
	    factor=pMax*sin(Pi*t/tMax);
	  }

	}
	else
	{
          // supply the time derivative -- p.t is probably not needed
          if( t>=0. && t <= tMax )
	  {
	    // factor=.5*pMax*twoPi/tMax*sin(twoPi*t/tMax);
	    factor=pMax*Pi/tMax*cos(Pi*t/tMax);
	  }

	}

	if( true && ( (debug() & 2 && t <= tMax) || t <= dt ) )
	  printF("--UBV-- pressure pulse: t=%8.2e, assign (side,axis,grid)=(%i,%i,%i)  forceType=%i, "
		 " pMax=%f, tMax=%f, p=%9.3e\n",t,side,axis,grid,(int)forcingType,pMax,tMax,factor);


        bd(Ib1,Ib2,Ib3,pc)= factor;  // pressure
        // bd(Ib1,Ib2,Ib3,pc)= factor*sin(2.*Pi*x(Ib1,Ib2,Ib3,0));  // pressure
        // bd(Ib1,Ib2,Ib3,pc)= pMax*sin(twoPi*t/tMax)*exp(-10.*SQR(x(Ib1,Ib2,Ib3,0)-.5));

	bd(Ib1,Ib2,Ib3,uc)=0.;
	bd(Ib1,Ib2,Ib3,vc)=0.;
	if( numberOfDimensions>2 )
	  bd(Ib1,Ib2,Ib3,wc)=0.;
      }

      else if( userDefinedBoundaryValue=="cardiacCycle" )
      {

        RealArray values(4);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);
	real pMax=values(0);
	real tP1 =values(1);
	real pMin=values(2);
	real tP2 =values(3);

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

        // -- we could avoid building the vertex array on Cartesian grids ---
	mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

        OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);

	numberOfSidesAssigned++;

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
        real factor;
	if( forcingType==computeForcing )
	{
          if( t>=0. && t<=tP1 )
	    factor=pMax*sin(Pi*t/tP1);
          else if (t>tP1 && t<=tP1+tP2/2.)
	    factor=pMin*sin(Pi*(t-tP1)/tP2);
          else if (t>tP1+tP2/2.)
            factor=pMin;
          else
            factor=0.;
	}
	else
	{
	    OV_ABORT("***userDefinedBoundaryValues: cardiac cycle: Why pdot is needed?");
	}

	if( true && ( (debug() & 2 && t <= tP1+tP2/2.) || t <= dt ) )
	  printF("--UBV-- cardiac cycle: t=%8.2e, assign (side,axis,grid)=(%i,%i,%i)  forceType=%i, "
		 " pMax=%f, tP1=%f, pMin=%f, tP2=%f, p=%9.3e\n",
                 t,side,axis,grid,(int)forcingType,pMax,tP1,pMin,tP2,factor);

        bd(Ib1,Ib2,Ib3,pc)= factor;  // pressure
	bd(Ib1,Ib2,Ib3,uc)=0.;
	bd(Ib1,Ib2,Ib3,vc)=0.;
	if( numberOfDimensions>2 )
	  bd(Ib1,Ib2,Ib3,wc)=0.;

      }
      else if( userDefinedBoundaryValue=="polynomialInflowProfile" )
      {
	// -- define an inflow profile --

        // -- we could avoid building the vertex array on Cartesian grids ---
	mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
        OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);


	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	numberOfSidesAssigned++;

	RealArray values(2);
	parameters.getUserBoundaryConditionParameters(side,axis,grid,values);

	const real uMax  =values(0);


	printF("userDefinedBoundaryValues: polyInflowProfile: uMax=%g (t=%9.3e)\n",uMax,t);
	// ::display(x(Ib1,Ib2,Ib3,1),"Here is x on the side");

        RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	if( forcingType==computeForcing )
	{
          // u = (16*uMax) * [y(1-y)]^2
	  // bd(Ib1,Ib2,Ib3,uc)=(16.*uMax)*x(Ib1,Ib2,Ib3,1)*(1.-x(Ib1,Ib2,Ib3,1))*x(Ib1,Ib2,Ib3,1)*(1.-x(Ib1,Ib2,Ib3,1));
	  // bd(Ib1,Ib2,Ib3,uc)=(64.*uMax)*x(Ib1,Ib2,Ib3,1)*(1.-x(Ib1,Ib2,Ib3,1))*x(Ib1,Ib2,Ib3,1)*(1.-x(Ib1,Ib2,Ib3,1))*x(Ib1,Ib2,Ib3,1)*(1.-x(Ib1,Ib2,Ib3,1));

	  bd(Ib1,Ib2,Ib3,uc)=pow( (4.*uMax)*x(Ib1,Ib2,Ib3,1)*(1.-x(Ib1,Ib2,Ib3,1)), .5 );
	}
	else
	{
	  // time derivative of the forcing
	  bd(Ib1,Ib2,Ib3,uc)=0.;
	}
	bd(Ib1,Ib2,Ib3,vc)=0.;
	if( numberOfDimensions==3 )
	  bd(Ib1,Ib2,Ib3,wc)=0.;

      }


      else if( userDefinedBoundaryValue=="timeFunctionOption" )
      {
	// Here is the time function associated with this grid (*fix me for AMR - use base grid)
        TimeFunction & timeFunction = parameters.dbase.get<TimeFunction>(sPrintF("timeFunctionG%iS%1iA%1i",grid,side,axis));

        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

	bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	numberOfSidesAssigned++;

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

        // evaluate the time function
	real f,ft;
	timeFunction.eval(t,f,ft);
        real factor = forcingType==computeForcing ? f : ft;

	if( false && t <= 5.*dt )
	  printF("--UBV-- timeFunctionOption: t=%8.2e  assign (side,axis,grid)=(%i,%i,%i) f=%9.3e ft=%9.3e\n"
		 ,t,side,axis,grid,f,ft);

        // printF(" ---UBV-- bcType =%i\n",(int)parameters.bcType(side,axis,grid));

        Range C=numberOfComponents;

	// The profile is saved in the ghost line of the "bd" array. (at least for parabolic -- *check me*)
	if( parameters.bcType(side,axis,grid)==Parameters::parabolicInflowUserDefinedTimeDependence )
	{
	  Index Ig1,Ig2,Ig3;
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	  ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);

	  // ::display(bd(Ig1,Ig2,Ig3,uc)," parabolic profile","%8.2e ");

	  // Give time dependence to the BC values
	  if( true )
	    bd(Ib1,Ib2,Ib3,C)= factor*bd(Ig1,Ig2,Ig3,C); // multiply fixed values stored in ghost points and save in boundary values
	}
	else
	{ // *check me*
	  RealArray values(numberOfComponents);
	  parameters.getUserBoundaryConditionParameters(side,axis,grid,values);
	  if( false )
	    printF("--UBV-- t=%9.3e timeFunction factor=%9.3e applied to values [%e,%e,%e]\n",
		   t,factor,values(0),values(1),values(2));
	  for( int c=C.getBase(); c<=C.getBound(); c++ )
	    bd(Ib1,Ib2,Ib3,c)= factor*values(c);
	}

      }


      else
      {
	// printF(" userDefinedBoundaryValues: mg.boundaryCondition(side,axis)=%i \n",mg.boundaryCondition(side,axis));

      }
    }

  }

  return numberOfSidesAssigned;
}
