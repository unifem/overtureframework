// ====================================================================================
///  \file collide.C
///  \brief test program to check the collision between moving bodies.
// ===================================================================================

#include "Overture.h"
#include "SquareMapping.h"
#include "PlotIt.h"
#include "PlotStuff.h"
#include "MatrixTransform.h"
#include "AnnulusMapping.h"
#include "RigidBodyMotion.h"
#include "BodyDefinition.h"
#include "DetectCollisions.h"

int
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
  Mapping::debug=0;

  char buff[180];

  PlotStuff ps;
  GraphicsParameters psp;
    
  const int numberOfBodies=2;

  // centre of mass: position, velocity
  RealArray xCM(3,numberOfBodies), vCM(3,numberOfBodies);
  xCM=0.; vCM=0.; 
  xCM(0,0)=-.3;  xCM(1,0)=0.4;  vCM(0,0)= 1.;
  xCM(0,1)=+.3;  xCM(1,1)=0.0;  vCM(0,1)= -1.;

  BodyDefinition bodyDefinition;
  int numberOfFaces=1;
  IntegerArray boundary(3,numberOfFaces);
  int side=0, axis=axis2, grid;
  boundary(0,0)=side;
  boundary(1,0)=axis;
  int b;
  for( b=0; b<numberOfBodies; b++ )
  {
    int surfaceID=b;    // this number identifies the surface
    boundary(2,0)=b; // grid;
    bodyDefinition.defineSurface( surfaceID,numberOfFaces,boundary ); // define the surface
  }
  
  Mapping *map[numberOfBodies];
  // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
  // can rotate/scale and shift any Mapping
  MatrixTransform *transform[numberOfBodies];

  for( b=0; b<numberOfBodies; b++ )
  {
    map[b] = new AnnulusMapping(.2,.4, xCM(0,b),xCM(1,b) );
    transform[b] = new MatrixTransform(*map[b]);
  }

  int numberOfDimensions=2;
  GridCollection gc(numberOfDimensions,numberOfBodies);
  for( b=0; b<numberOfBodies; b++ )
  {
    gc[b].reference(*transform[b]);
  }
  gc.updateReferences();  


  RealArray xBound(2,3);
  xBound(0,Range(0,2))=-1., xBound(1,Range(0,2))=1.;
  
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  psp.set(GI_USE_PLOT_BOUNDS,TRUE);  // use the region defined by the plot bounds
  psp.set(GI_PLOT_BOUNDS,xBound); // set plot bounds

  psp.set(GI_TOP_LABEL,"Initial grid");  // set title
  PlotIt::plot(ps,gc,psp);
  
  int numberOfSteps=101;
  real t=0., dt=.01;
  
  int debug=0;

  RealArray mass(numberOfBodies);           // total mass
  mass(0)=1.;
  mass(1)=1.;

  RealArray mI(3,numberOfBodies);        // 3 moment of inertia

  mI(0,0)=1.; mI(1,0)=1.; mI(2,0)=1.;
  mI(0,1)=1.; mI(1,1)=1.; mI(2,1)=1.;

  Range all;

  RealArray e(3,3), e0(3,3), r(3,3);      // 3 axes of inertia
  RealArray xNew(3);
  

  RealArray f(3), g(3);  // force and torque

  RealArray omega(3); omega=.0;
  RealArray w(3); w=.0;   // angular velocities about axes of inertia
  RealArray dx(3), vDot(3), dw(3), wDot(3),dTheta(3), eDot(3,3);

  RigidBodyMotion *body[numberOfBodies];
  for( b=0; b<numberOfBodies; b++ )
  {
    body[b] = new RigidBodyMotion(numberOfDimensions);
    body[b]->setProperties(mass(b),mI(all,b),numberOfDimensions);
  }
  
  
  aString menu[]=
  {
    "move",
    "exit",
    ""
  };
  aString answer;

  for(;;)
  {
    
    ps.getMenuItem(menu,answer,"Choose");
    if( answer=="exit" )
      break;
  
    t=0.;
    e(0,0)=1.; e(1,0)=0.; e(2,0)=0.;
    e(0,1)=0.; e(1,1)=1.; e(2,1)=0.;
    e(0,2)=0.; e(1,2)=0.; e(2,2)=1.;
    e0=e;
    w=0.;

    int component=0;
    
    RealArray vCM0(3,numberOfBodies);
    vCM0=vCM;
    
    for( b=0; b<numberOfBodies; b++ )
      body[b]->setInitialConditions(t,xCM(all,b),vCM(all,b),w,e0);

    for (int i=0; i<=numberOfSteps; i++) 
    {
      // getForce( t,component,f,g );
      f=0.;
      g=0.;
      
      for( b=0; b<numberOfBodies; b++ )
      {
        RigidBodyMotion & bod = *body[b];
	
	bod.integrate( t,f,g, t+dt,xNew,r  );

	// getForce( t+dt,component,f,g );
	bod.correct( t+dt,f,g, xNew,r  );

	// bod.getAngularVelocities( t+dt,w );
	// bod.getVelocity( t+dt,vCM );
      
	// body.getPosition( t+dt,xNew,r );
      
	transform[b]->shift(-xCM(0,b),-xCM(1,b),-xCM(2,b));
	transform[b]->rotate( r );
	transform[b]->shift(xNew(0),xNew(1),xNew(2));

	xCM(all,b)=xNew;
	
      }
      t+=dt;
      gc.geometryHasChanged();
      
      psp.set(GI_TOP_LABEL,sPrintF(buff,"Solution at step %i",i+1));


      int collision=detectCollisions( t,gc,numberOfBodies,body,bodyDefinition);
      if( collision )
        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);

      ps.erase();
      PlotIt::plot(ps,gc,psp);
      ps.redraw(true);   // force a redraw
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);    

    } // end for (i )

  } // for(;;)
  
  Overture::finish();          
  return 0;
}
