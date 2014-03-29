#include "DeformingBodyMotion.h"
#include "SplineMapping.h"
#include "NurbsMapping.h"
#include "DomainSolver.h"
#include "AdParameters.h"
#include "ParallelUtility.h"

#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base,I2Base,I3Base;\
  int I1Bound,I2Bound,I3Bound;\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

// ==================================================================================================
/// \brief This function can be used by users to define the motion of a deforming surface which is
///    defined by a set of data points.
///
// ==================================================================================================
// ========================================================================================================
/// \brief This function can be filled in to define the motion of a deforming surface which is
///    defined by a set of data points. In general this function will be called by a predictor-corrector
///    time stepping scheme. The parameter "option" indicates whether the function is being called by the 
///    predictor or corrector. 
/// 
/// \details This function is intended to be changed by a user to define a new deforming body motion.
///       The default implementation simply advects the interface with the current fluid velocity.
///
/// \t1, cgf1, t2, cgf2, t3,cgf3 : Advance the solution from (t1,cgf1) to (t2,cgf2) using the 
///    solution at (t2,cgf2) 
/// \param option : option=0 : predictor-step, option=1 corrector step. The shape of the interface
///  should always be advanced during the predictor step. The corrector step can be used to make
///  small corrections to the shape that was computed from predictor step. This is sometimes needed
///  to make the time-stepping stable. 
///
/// 
// ========================================================================================================
int AdParameters::
userDefinedDeformingSurface( DeformingBodyMotion & deformingBody,
                            real t1, real t2, real t3, 
			    GridFunction & cgf1,
			    GridFunction & cgf2,
			    GridFunction & cgf3,
			    int option )
{
  int ierr=0;

  // -----  First look up some variables -----
  const int numberOfDimensions = cgf1.cg.numberOfDimensions();

  DataBase & deformingBodyDataBase = deformingBody.deformingBodyDataBase;
 
  const aString & userDefinedDeformingSurfaceOption= 
                   deformingBodyDataBase.get<aString>("userDefinedDeformingSurfaceOption");
 

  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");

  CompositeGrid & cg = cgf3.cg;

  const real dt = t3-t1;  
  
  // globalStepNumber : counts the time steps 
  const int & globalStepNumber = dbase.get<int >("globalStepNumber");

  if( true || debug & 2 )
    printF("--DeformingBodyMotion::userDefinedDeformingSurface called for t1=%g, t2=%g, t3=%g, dt=%9.3e"
           " globalStepNumber=%i, option=%i.\n",
	   t1,t2,t3,dt,globalStepNumber,option);

  // Loop over the "faces" (i.e. surface grid patches) that make up this deforming body 
  for( int face=0; face<numberOfFaces; face++ )
  {
    int sideToMove=boundaryFaces(0,face);
    int axisToMove=boundaryFaces(1,face);
    int gridToMove=boundaryFaces(2,face); 

    realArray & u1 = cgf1.u[gridToMove];   // solution at cgf1.t 
    realArray & u2 = cgf2.u[gridToMove];   // solution at cgf2.t 
	
    int i1,i2,i3;
    Index Ib1,Ib2,Ib3;
    const int numGhost=1;  // include ghost points (this must match the value in DeformingBodyMotion::initialize)
    getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3,numGhost);


    vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
    assert( face<surfaceArray.size() );
    RealArray *px = surfaceArray[face];
    RealArray &x0 = px[0];                // initial surface 

    const int mx1 = (globalStepNumber+1)%2; // mx1 alteranates between 0 and 1
    const int mx2 = (mx1+1)%2;              // mx2 alteranates between 1 and 0
    RealArray &x1 = px[mx1+1];              // array holding points on the surface - past time
    RealArray &x2 = px[mx2+1];              // array holding points on the surface - new time

    // NOTE: x0,x1,x2 have a ghost point and these can be used and or set 

    if( userDefinedDeformingSurfaceOption=="sinusoidal" )
    {
      // Define a sinusoidal motion of the interface
      //  x_m(t) = x_m(0) + amp[m]*sin(freqx*s0) * sin(freqt*t), m=0,1,2

      RealArray & par = deformingBodyDataBase.get<RealArray>("userDefinedDeformingSurfaceParams");
      real amp[3]={par(0),par(1),par(2)};     // amplitudes
      real freqx=par(3);   // frequency in space
      real freqt=par(4);   // frequency in time

      if( true )
	printF("userDefinedDeformingSurface:sinusoidal: t=%9.3e, amp=[%g,%g,%g] freqx=%g, freqt=%g\n",t3,
	       amp[0],amp[1],amp[2],par(3),par(4));

      FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
      {
        // arclgength for an initially flat surface: 
        real s;
        if( numberOfDimensions==2)
           s = sqrt( SQR(x0(i1,i2,i3,0)) + SQR(x0(i1,i2,i3,1)) ); 
        else
           s = sqrt( SQR(x0(i1,i2,i3,0)) + SQR(x0(i1,i2,i3,1)) + SQR(x0(i1,i2,i3,2)) ); 
	for( int axis=0; axis<numberOfDimensions; axis++ )
	{
	  x2(i1,i2,i3,axis) = x0(i1,i2,i3,axis) + amp[axis]*sin(freqx*s)*sin(freqt*t3);
	}
      }

    }
    else if( userDefinedDeformingSurfaceOption=="concentration motion" )
    {
      // The surface height h(s,t) satisfies 
      // 
      //    d(h)/dt = alpha*( u - ue )  
      // 
      // where u(x,y,z,t) is the concentration (solution to the advection-diffusion equation)
      // and ue is a given "equilibrium" concentration. 

      RealArray & par = deformingBodyDataBase.get<RealArray>("userDefinedDeformingSurfaceParams");
      real alpha=par(0), ue=par(1);

      if( true )
	printF("userDefinedDeformingSurface:concentration motion: (%s) t1=%9.3e, t2=%9.3e, alpha=%g, ue=%g\n",
               (option==0 ? "predict" : "correct"),t1,t2,alpha,ue);

      const int dir=1, dirp1=0; // vertical motion *fix me*
      if( option==0 )
      {
        // predict stage:
        //   u2 = solution at current time t
        //   x2 should predict solution at t+dt 
	FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	{
          // predict with forward-Euler 
	  x2(i1,i2,i3,dir) = x1(i1,i2,i3,dir) + (dt*alpha)*( u2(i1,i2,i3,0) - ue );
	  x2(i1,i2,i3,dirp1) = x0(i1,i2,i3,dirp1);

	  if( false &&  i1==1 )
            printf("predict: (i1,i2)=(%i,%i)  x1=%10.4e, u2=%10.4e, x2=%10.4e delta=%9.3e\n",
		   i1,i2,x1(i1,i2,i3,dir),u2(i1,i2,i3,0),x2(i1,i2,i3,dir),(dt*alpha)*( u2(i1,i2,i3,0) - ue ));
	}
      }
      else
      {
	// -- corrector stage --
        //   u1 = solution at time t 
        //   u2 = solution at new time t+dt 
        //   x2 should correct at t+dt 

	FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	{
          // correct with the trapezodial rule: 
          x2(i1,i2,i3,dir) = x1(i1,i2,i3,dir) + (dt*alpha)*( .5*u2(i1,i2,i3,0)+.5*u1(i1,i2,i3,0) - ue );            

	  if( false &&  i1==1 )
	  {
	    real delta=(dt*alpha)*( .5*u2(i1,i2,i3,0)+.5*u1(i1,i2,i3,0) - ue );
            printf("correct: (i1,i2)=(%i,%i)  x1=%10.4e, x2=%10.4e u1=%10.4e, u2=%10.4e, dt=%8.2e delta=%8.2e\n",
		   i1,i2,x1(i1,i2,i3,dir),x2(i1,i2,i3,dir),u1(i1,i2,i3,0),u2(i1,i2,i3,0),dt,delta);
	  }

	}

        // save current solution in x1 
	// FOR_3(i1,i2,i3,Ib1,Ib2,Ib3)
	// {
	//   for( int axis=0; axis<numberOfDimensions; axis++ )
	//     x1(i1,i2,i3,axis) = x2(i1,i2,i3,axis);
	// }

      }
      
      
    }


    else if( userDefinedDeformingSurfaceOption=="advection" )
    {
      // ** THIS OPTION WILL NOT WORK ***
      OV_ABORT("finish me");
      // ****FIX ME to use the variable advection velocity ***

      // advect the body from time t1 to t3 using velocity from time t2 

      const int uc = dbase.get<int >("uc");
      const int vc = dbase.get<int >("vc");
      const int wc = dbase.get<int >("wc");

      if( numberOfDimensions==2 )
      {
	assert( uc>=0 && vc>=0 );
	FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	{
	  real x = x2(i1,i2,i3,0), y=x2(i1,i2,i3,1);
	  real u0=u2(i1,i2,i3,uc), v0=u2(i1,i2,i3,vc);
	      
	  // u0=1.;  v0=0.; // do this for testing 
	  // u0=0.;  v0=1; // do this for testing 
	     
	  if( false )
	  {
	    x2(i1,i2,i3,0) = x + dt*u0;
	    x2(i1,i2,i3,1) = y + dt*v0;
	  }
	  else
	  {
    	    real amp=.05;
	    x2(i1,i2,i3,0) = x; 
	    x2(i1,i2,i3,1) = x0(i1,i2,i3,1) + amp*sin(twoPi*x)*sin(twoPi*t3); 
	  }
	  
	  // printF("userDefinedDeformingSurface: advectBody: x=(%5.2f,%5.2f) u=(%5.2f,%5.2f) x2=(%5.2f,%5.2f)\n",
          //           x,y,u0,v0,x2(i1,i2,i3,0),x2(i1,i2,i3,1));
	    
	}

      }
      else if( numberOfDimensions==3)
      {
	assert( uc>=0 && vc>=0 && wc>= 0 );

	FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	{
	  real x = x2(i1,i2,i3,0), y=x2(i1,i2,i3,1), z=x2(i1,i2,i3,2);
	  real u0=u2(i1,i2,i3,uc), v0=u2(i1,i2,i3,vc), w0=u2(i1,i2,i3,wc);
	      
	  x2(i1,i2,0) = x + dt*u0;
	  x2(i1,i2,1) = y + dt*v0;
	  x2(i1,i2,2) = z + dt*w0;

	}
      }

    }
    else
    {
      OV_ABORT("ERROR: unknown user defined deforming surface option");
    }

    // ----- Now update the Mapping that defines the starting curve for the hyperbolic mapping ----

    // The "surface" Mapping holds the start curve for the hyperbolic mapping

    vector<Mapping*> & surface = deformingBodyDataBase.get<vector<Mapping*> >("surface");
    assert( face<surface.size() );
    NurbsMapping & startCurve = *((NurbsMapping*)surface[face]);

    // We need to reshape the x2 array so that it looks like x2(I,0:1) in 2d and x2(I,J,0:2) in 3d 
    int axisp1, axisp2;  // tangential directions (directions that form the surface)
    if( axisToMove==0 )
    {
      axisp1=1; axisp2=2;
    }
    else if( axisToMove==1 )
    {
      axisp1=0; axisp2=2;
    }
    else
    {
      axisp1=0; axisp2=1;
    }
    Range Rx=numberOfDimensions;
    if( numberOfDimensions==2 )
      x2.reshape(x2.dimension(axisp1),Rx);
    else
      x2.reshape(x2.dimension(axisp1),x2.dimension(axisp2),Rx);

    int interpOption=0, degree=3;
    // Note: Choose parameterizeByChordLength instead of parameterizeByIndex to redistribute the
    // points on the surface evenly in arc-length
    #ifdef USE_PPP
     Overture::abort("fix me");
    #else
      startCurve.interpolate(x2,interpOption,Overture::nullRealDistributedArray(),degree,
					      NurbsMapping::parameterizeByIndex,numGhost);
    #endif
    x2.reshape(Ib1,Ib2,Ib3,Rx);

  } // end for face
  
  return ierr;
}





//==============================================================================================
/// \brief Choose and set parameters for a user defined deforming surface.
//==============================================================================================
int AdParameters::
userDefinedDeformingSurfaceSetup( DeformingBodyMotion & deformingBody )
{
  GenericGraphicsInterface & gi = *dbase.get<GenericGraphicsInterface* >("ps");

  const int & numberOfComponents=dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions=dbase.get<int >("numberOfDimensions");
  // const int & rc = dbase.get<int >("rc");   //  density = u(all,all,all,rc)  (if appropriate for this PDE)

  // Here is where parameters can be put to be saved in the show file:
  ListOfShowFileParameters & showFileParams = dbase.get<ListOfShowFileParameters>("showFileParams");

  // here is a menu of possible initial conditions
  aString menu[]=  
  {
    "sinusoidal",
    // "advection", // this does not work yet
    "concentration motion",
    "exit",
    ""
  };
  aString answer,answer2;
  char buff[100];
  gi.appendToTheDefaultPrompt(">UserDefinedDeformingSurface");

  DataBase & deformingBodyDataBase = deformingBody.deformingBodyDataBase;
  // first time through allocate variables 
  if( !deformingBodyDataBase.has_key("userDefinedDeformingSurfaceOption") )
  {
    deformingBodyDataBase.put<aString>("userDefinedDeformingSurfaceOption");
    deformingBodyDataBase.get<aString>("userDefinedDeformingSurfaceOption")="advection";
  }

  aString & userDefinedDeformingSurfaceOption= deformingBodyDataBase.get<aString>("userDefinedDeformingSurfaceOption");
  
 
  for( ;; )
  {
    gi.getMenuItem(menu,answer,"enter an option");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="advection" )
    {
      // Advect the interface with the current solution velocity
      userDefinedDeformingSurfaceOption="advection";

    }
    else if( answer=="sinusoidal" )
    {
      // Define a sinusoidal motion of the interface
      //  x_m(t) = x_m(0) + amp[m] * sin(freqx*x_m(0)) * sin(freqt*t), m=0,1,2

      userDefinedDeformingSurfaceOption="sinusoidal";

      if( !deformingBodyDataBase.has_key("userDefinedDeformingSurfaceParams") )
      {
         deformingBodyDataBase.put<RealArray>("userDefinedDeformingSurfaceParams");
      }
      RealArray & par = deformingBodyDataBase.get<RealArray>("userDefinedDeformingSurfaceParams");
      par.redim(5);   par=0.;
      printF("The sinusoidal motion is defined as\n"
             "   x_m(t) = x_m(0) + amp[m] * sin(freqx*s0) * sin(freqt*t), m=0,1,2\n"
             " where s0 is the arclength parameter of the initial curve\n");
             
      gi.inputString(answer,"Enter amp[0],amp[1],amp[2], freqx, freqt");
      sScanF(answer,"%e %e %e %e %e",&par(0),&par(1),&par(2),&par(3),&par(4));
      printf("Setting amp=[%g,%g,%g] freqx=%g, freqt=%g\n",par(0),par(1),par(2),par(3),par(4));

      
    }
    else if( answer=="concentration motion" )
    {
      // 
      // The surface height h(s,t) satisfies 
      // 
      //    d(h)/dt = alpha*( u - ue )  
      // 
      // where u(x,y,z,t) is the concentration (solution to the advection-diffusion equation)
      // and ue is a given "equilibrium" concentration. 

      userDefinedDeformingSurfaceOption="concentration motion";

      if( !deformingBodyDataBase.has_key("userDefinedDeformingSurfaceParams") )
      {
         deformingBodyDataBase.put<RealArray>("userDefinedDeformingSurfaceParams");
      }
      RealArray & par = deformingBodyDataBase.get<RealArray>("userDefinedDeformingSurfaceParams");
      par.redim(5);   par=0.;
      printF("The surface height h satisfies \n"
             "     d(h)/dt = alpha*( u - ue ) )  \n"
             " where u is the current solution and ue is the equilibrium concentration\n");

      gi.inputString(answer,"Enter alpha,ue");
      sScanF(answer,"%e %e",&par(0),&par(1));
      printf("Setting alpha=%e, ue=%e\n",par(0),par(1));

      
    }
    else 
    {
      printF("Parameters::userDefinedDeformingSurfaceSetup: Unknown option =[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
  }
  
  gi.unAppendTheDefaultPrompt();

  return 0;
}

// ================================================================================================
/// \brief This routine is called when DomainSolver is finished and can 
///  be used to clean up memory.
// ================================================================================================
void AdParameters::
userDefinedDeformingSurfaceCleanup( DeformingBodyMotion & deformingBody )
{
  printF("***userDefinedDeformingSurfaceCleanup\n");

}
