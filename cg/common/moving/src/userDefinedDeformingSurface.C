#include "DeformingBodyMotion.h"
#include "SplineMapping.h"
#include "NurbsMapping.h"
#include "DomainSolver.h"
#include "Parameters.h"
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
int DeformingBodyMotion::
userDefinedDeformingSurface(real t1, real t2, real t3, 
			    GridFunction & cgf1,
			    GridFunction & cgf2,
			    GridFunction & cgf3,
			    int option )
{
  int ierr=0;

  // -----  First look up some variables -----
  const int numberOfDimensions = cgf1.cg.numberOfDimensions();
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");

  CompositeGrid & cg = cgf3.cg;

  const real dt = t3-t2;  
  
  // globalStepNumber : counts the time steps 
  const int & globalStepNumber = parameters.dbase.get<int >("globalStepNumber");

  if( true || debug & 2 )
    printF("--DeformingBodyMotion::userDefinedDeformingSurface called for t1=%g, t2=%g, t3=%g, dt=%9.3e"
           " globalStepNumber=%i\n",
	   t1,t2,t3,dt,globalStepNumber);

  // Loop over the "faces" (i.e. surface grid patches) that make up this deforming body 
  for( int face=0; face<numberOfFaces; face++ )
  {
    int sideToMove=boundaryFaces(0,face);
    int axisToMove=boundaryFaces(1,face);
    int gridToMove=boundaryFaces(2,face); 


    if( true )
    {
      // advect the body from time t1 to t3 using velocity from time t2 

      const int uc = parameters.dbase.get<int >("uc");
      const int vc = parameters.dbase.get<int >("vc");
      const int wc = parameters.dbase.get<int >("wc");

      vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
      assert( face<surfaceArray.size() );
      RealArray *px = surfaceArray[face];
      RealArray &x0 = px[0];                // initial surface 
      RealArray &x1 = px[1];                // array holding points on the surface - past time
      RealArray &x2 = px[2];                // array holding points on the surface - new tim


      realArray & u = cgf2.u[gridToMove];
	
      int i1,i2,i3;
      Index Ib1,Ib2,Ib3;
      const int numGhost=1;  // include ghost points (this must match the value in DeformingBodyMotion::initialize)
      getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3,numGhost);

      if( numberOfDimensions==2 )
      {
	assert( uc>=0 && vc>=0 );
	FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	{
	  real x = x2(i1,i2,i3,0), y=x2(i1,i2,i3,1);
	  real u0=u(i1,i2,i3,uc), v0=u(i1,i2,i3,vc);
	      
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
	  real u0=u(i1,i2,i3,uc), v0=u(i1,i2,i3,vc), w0=u(i1,i2,i3,wc);
	      
	  x2(i1,i2,0) = x + dt*u0;
	  x2(i1,i2,1) = y + dt*v0;
	  x2(i1,i2,2) = z + dt*w0;

	}
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


      //  startCurve.interpolate(x2);

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

    }
    
  } // end for face
  
  return ierr;
}


