#include "MovingGrids.h"
#include "MatrixTransform.h"
#include "GridFunction.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"

// Define new user-defined options here:
static enum UserDefinedMotionOption
{
  linearMotion=0,
  sinusoidalMotion,
  rampMotion
} userDefinedMotionOption=linearMotion;

static real rpar[20];  // holds parameters


// Define a ramp function with VELOCITY that increases monotonically from 0 to 1. 

#define ramp1t(t)  (t)*(t)*( -(t)/3.+.5 )*6.

// ramp3t(0)=0  ramp3t(1)=1 -- three derivatives zero at 0 and 1

#define ramp3(t)    ( -14.*pow(t,6.)+7.*pow(t,5.)-2.5*pow(t,8.)+10.*pow(t,7.) )
#define ramp3t(t)    ( -84*pow(t,5.)+35*pow(t,4.)-20*pow(t,7.)+70*pow(t,6.) )
#define ramp3tt(t)   ( -84*5.*pow(t,4.)+35.*4.*pow(t,3.)-20.*7.*pow(t,6.)+70.*6.*pow(t,5.) )
#define ramp3ttt(t)   ( -84*5.*4.*pow(t,3.)+35.*4.*3.*pow(t,2.)-20.*7.*6.*pow(t,5.)+70.*6.*5.*pow(t,4.) )


//\begin{>>MovingGridsSolverInclude.tex}{\subsection{userDefinedMotion}} 
int MovingGrids::
userDefinedMotion(const real & t1, 
		  const real & t2, 
		  const real & t3,
		  const real & dt0,
		  GridFunction & cgf1,  
		  GridFunction & cgf2,
		  GridFunction & cgf3 )
// =========================================================================================
// /Description:
//    User defined moving grids. Change this routine to define a new general type of motion.
//  Specified motions can normally be treated using the userDefinedTransformMotion below, rather
//  than this more general case.
//\end{MovingGridsSolverInclude.tex}  
//=========================================================================================
{

  return 0;
}



//\begin{>>MovingGridsSolverInclude.tex}{\subsection{userDefinedTransformMotion}} 
int MovingGrids::
userDefinedTransformMotion(const real & t1, 
			   const real & t2, 
			   const real & t3,
			   const real & dt0,
			   GridFunction & cgf1,  
			   GridFunction & cgf2,
			   GridFunction & cgf3,
			   const int grid )
// =========================================================================================
// /Description:
//    User defined moving grids that use the matrix transform to rotate scale and shift the grids.
//\end{MovingGridsSolverInclude.tex}  
//=========================================================================================
{
  
  assert( cgf3.transform[grid]!=NULL );
  MatrixTransform & transform = *cgf3.transform[grid];  // --- move grids in cgf3 ----
  assert( moveOption(grid)==userDefinedMovingGrid );

  const real t0=t3;

  if( userDefinedMotionOption==linearMotion )   
  {
    const real dv[3]={rpar[2],rpar[3],rpar[4]};

    // compute the shift from time t=0 to avoid accumulation of round-off.
    real a=rpar[0], p=rpar[1];
    const real deltaX=a*pow(t0,p); 

    if( true && parameters.dbase.get<FILE* >("moveFile")!=NULL )
    {
      // save info to the "moveFile"
      fprintf(parameters.dbase.get<FILE* >("moveFile"),"userDefinedMotion:linearMotion t=%10.3e x=%10.3e\n",t0,dv[0]*deltaX);
    }

    transform.reset();
    transform.shift(dv[0]*deltaX,dv[1]*deltaX,dv[2]*deltaX);

  }
  else if( userDefinedMotionOption==sinusoidalMotion )
  {
    // Sinusoidal motion:
    // x(t) = x0(t) + dv*pow( .5-.5*cos(2*Pi*omega*(t-ta)), beta ) 

    const real xv[3]={rpar[0],rpar[1],rpar[2]};
    const real dv[3]={rpar[3],rpar[4],rpar[5]};
    const real ta=rpar[6], omega=rpar[7], beta=rpar[8];
    
    real amp = pow( .5-.5*cos(twoPi*omega*(t0-ta)) , beta );
    
    // printf("sinusoidal motion: t=%7.5f dv[0]=%6.4f, omega=%6.4f, dv[0]*pow(g,beta)=%10.4e\n",t0,dv[0],omega,dv[0]*amp);

    // compute the shift from time t=0 to avoid accumulation of round-off.
    transform.reset();
    transform.shift(xv[0]+amp*dv[0],xv[1]+amp*dv[1],xv[2]+amp*dv[2]);
  }
  else if( userDefinedMotionOption==rampMotion )   
  {
    // x(t) = a*ramp(t/rampInterval)*dv[]
    const real a=rpar[0], rampInterval=rpar[1];
    const real dv[3]={rpar[2],rpar[3],rpar[4]};
    const real ts = t0/rampInterval;  // ramp(t)=ramp(1) for 

    real deltaX;
    if( ts<=1. )
    {
      deltaX=a*ramp3(ts)*rampInterval; 
    }
    else
    {
      deltaX = a*( ramp3(1.)*rampInterval + ramp3t(1.)*(t0-rampInterval) ); 
    }
    // printf("rampMotion: rampInterval=%10.2e, t=%12.4e x=%12.4e\n",rampInterval,t0,deltaX*dv[0]);
    
    // compute the shift from time t=0 to avoid accumulation of round-off.
    transform.reset();
    transform.shift(dv[0]*deltaX,dv[1]*deltaX,dv[2]*deltaX);

  }
  else
  {
    printF("MovingGrids::userDefinedMotion:ERROR unknown userDefinedMotionOption=%i\n",userDefinedMotionOption);
    OV_ABORT("ERROR");
  }

  return 0;
}

//\begin{>>MovingGridsSolverInclude.tex}{\subsection{getUserDefinedGridVelocity}} 
int MovingGrids::
getUserDefinedGridVelocity( GridFunction & gf0, const real & t0, const int grid )
//=================================================================================
// /Description:
//    Determine the gridVelocity for this grid function
//
//\end{MovingGridsSolverInclude.tex}  
//=================================================================================
{
  assert( moveOption(grid)==userDefinedMovingGrid );

  MappedGrid & mg = gf0.cg[grid];
  realArray & gridVelocity = gf0.getGridVelocity(grid);
  Index I1,I2,I3;
  
  if( userDefinedMotionOption==linearMotion )   
  {

    // compute the velocity:
    real a=rpar[0], p=rpar[1];
    const real dv[3]={rpar[2],rpar[3],rpar[4]};

    const real velocity=a*p*pow(t0,p-1.); 

    getIndex( mg.dimension(),I1,I2,I3 );
    for( int axis=axis1; axis<mg.numberOfDimensions(); axis++ )
      gridVelocity(I1,I2,I3,axis)=velocity*dv[axis]; 

  }
  else if( userDefinedMotionOption==sinusoidalMotion )
  {
    const real xv[3]={rpar[0],rpar[1],rpar[2]};
    const real dv[3]={rpar[3],rpar[4],rpar[5]};
    const real ta=rpar[6], omega=rpar[7], beta=rpar[8];
    
    // Sinusoidal motion:
    // x(t) = x0(t) + dv*pow( .5-.5*cos(2*Pi*omega*(t-ta)), beta ) 
    // v(t) = dv*[ beta*pow(g,beta-1) *.5*2*Pi*omega*sin(2*Pi*omega*(t-ta))
    real g=.5-.5*cos(twoPi*omega*(t0-ta));
    real hPrime=beta*pow(g,beta-1.);
    real velocity = hPrime* .5*twoPi*omega*sin(twoPi*omega*(t0-ta));
    
    // printf("sinusoidal motion: t=%7.5f velocity/twoPi=%10.4e\n",t0,velocity/twoPi);
    

    getIndex( mg.dimension(),I1,I2,I3 );
    for( int axis=axis1; axis<mg.numberOfDimensions(); axis++ )
      gridVelocity(I1,I2,I3,axis)=velocity*dv[axis]; 

  }
  else if( userDefinedMotionOption==rampMotion )   
  {
    // x(t) = a*ramp(t/rampInterval)*dv[]
    const real a=rpar[0], rampInterval=rpar[1];
    const real dv[3]={rpar[2],rpar[3],rpar[4]};
    const real ts=t0/rampInterval;

    // compute the velocity:
    real velocity;
    if( ts<=1. )
      velocity = a*ramp3t(ts); 
    else
      velocity = a*ramp3t(1.);
    
    // printf("rampMotion: rampInterval=%10.2e, t=%12.4e v=%12.4e\n",rampInterval,t0,velocity*dv[0]);

    getIndex( mg.dimension(),I1,I2,I3 );
    for( int axis=axis1; axis<mg.numberOfDimensions(); axis++ )
      gridVelocity(I1,I2,I3,axis)=velocity*dv[axis]; 

  }
  else
  {
    printF("MovingGrids::getUserDefinedGridVelocity:ERROR unknown userDefinedMotionOption=%i\n",
           userDefinedMotionOption);
    OV_ABORT("ERROR");
  }
  return 0;
}


//\begin{>>MovingGridsSolverInclude.tex}{\subsection{userDefinedGridAccelerationBC}}
int MovingGrids::
userDefinedGridAccelerationBC(const int & grid,
			      const real & t0,
			      MappedGrid & mg,
			      realMappedGridFunction & u ,
			      realMappedGridFunction & f ,
			      realMappedGridFunction & gridVelocity ,
			      realSerialArray & normal,
			      const Index & J1,
			      const Index & J2,
			      const Index & J3,
			      const Index & J1g,
			      const Index & J2g,
			      const Index & J3g )
//=================================================================================
// /Description:
//    Provide the acceleration of the boundary 
//
//\end{MovingGridsSolverInclude.tex}  
//=================================================================================
{
  assert( moveOption(grid)==userDefinedMovingGrid );

  #ifdef USE_PPP
    realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
    Index I1=J1,   I2=J2,   I3=J3;
    Index I1g=J1g, I2g=J2g, I3g=J3g;
    int includeGhost=1;
    bool ok = ParallelUtility::getLocalArrayBounds(f,fLocal,I1,I2,I3,includeGhost);
    ok = ParallelUtility::getLocalArrayBounds(f,fLocal,I1g,I2g,I3g,includeGhost);
    if( !ok ) return 0;   // there must be no communication after this 
  #else
    const Index &I1=J1,   &I2=J2,   &I3=J3;
    const Index &I1g=J1g, &I2g=J2g, &I3g=J3g;
    realArray & fLocal = f;
  #endif

  if( userDefinedMotionOption==linearMotion )
  {
    const real dv[3]={rpar[2],rpar[3],rpar[4]};

    // compute the acceleration:
    real a=rpar[0], p=rpar[1];
    real accel;
    if( p==0. || p==1. )
      accel=0.;
    else
    {
      accel=a*p*(p-1.)*pow(t0,p-2.); 
    }

    if( mg.numberOfDimensions()==2 )
    {
      fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(dv[0]*accel)+
		            normal(I1,I2,I3,1)*(dv[1]*accel) );
    }
    else
    {
      fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(dv[0]*accel)+
		            normal(I1,I2,I3,1)*(dv[1]*accel)+
		            normal(I1,I2,I3,2)*(dv[2]*accel) );
    }
  }
  else if( userDefinedMotionOption==sinusoidalMotion )
  {
    const real xv[3]={rpar[0],rpar[1],rpar[2]};
    const real dv[3]={rpar[3],rpar[4],rpar[5]};
    const real ta=rpar[6], omega=rpar[7], beta=rpar[8];
    
    // Sinusoidal motion:
    // x(t) = x0(t) + dv*pow( .5-.5*cos(2*Pi*omega*(t-ta)), beta ) 
    // v(t) = dv*[ beta*pow(g,beta-1)*.5*2*Pi*omega*sin(2*Pi*omega*(t-ta))
    // a(t) = dv*[ beta*(beta-1)*pow(g,beta-2)*SQR( .5*2*Pi*omega*sin(2*Pi*omega*(t-ta)) )
    //             +beta*pow(g,beta-1)*.5* SQR(2*Pi*omega)*cos(2*Pi*omega*(t-ta)) ] 

    real accel;
    if( beta==1 )
    {
      accel=.5*SQR(twoPi*omega)*cos(twoPi*omega*(t0-ta));
    }
    else
    {
      real g=.5-.5*cos(twoPi*omega*(t0-ta));
      real gDot=.5*twoPi*omega*sin(twoPi*omega*(t0-ta));

      accel = beta*(beta-1.)*pow( g,beta-2.)*gDot*gDot + 
              beta*pow( g , beta-1. )* .5*SQR(twoPi*omega)*cos(twoPi*omega*(t0-ta));

      printF("MovingGrids::userDefinedGridAccelerationBC: sinusoidal motion: t=%7.5f dv[0]*accel=%10.4e\n",
             t0,dv[0]*accel);

    }
    
    if( mg.numberOfDimensions()==2 )
    {
      fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(dv[0]*accel)+
		            normal(I1,I2,I3,1)*(dv[1]*accel) );
    }
    else
    {
      fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(dv[0]*accel)+
		            normal(I1,I2,I3,1)*(dv[1]*accel)+
		            normal(I1,I2,I3,2)*(dv[2]*accel) );
    }
  }
  else if( userDefinedMotionOption==rampMotion )
  {
    // x(t) = a*ramp(t/rampInterval)*dv[]
    const real a=rpar[0], rampInterval=rpar[1];
    const real dv[3]={rpar[2],rpar[3],rpar[4]};
    const real ts=t0/rampInterval;

    // compute the acceleration: 
    const real accel = ts<=1. ? a*ramp3tt(ts)/rampInterval : 0.; 
     
    if( mg.numberOfDimensions()==2 )
    {
      fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(dv[0]*accel)+
			    normal(I1,I2,I3,1)*(dv[1]*accel) );
    }
    else
    {
      fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(dv[0]*accel)+
			    normal(I1,I2,I3,1)*(dv[1]*accel)+
			    normal(I1,I2,I3,2)*(dv[2]*accel) );
    }
  }
  else
  {
    printF("MovingGrids::userDefinedGridAccelerationBC:ERROR unknown userDefinedMotionOption=%i\n",
           userDefinedMotionOption);
    OV_ABORT("ERROR");
  }
  return 0;
  
}

//\begin{>>MovingGridsSolverInclude.tex}{\subsection{getUserDefinedBoundaryAcceleration}}
int MovingGrids::
getUserDefinedBoundaryAcceleration( MappedGrid & mg, realSerialArray & gtt, int grid, real t0, 
                                    int option, const int side, const int axis )
//=================================================================================
// /Description:
//    Provide the acceleration (or other time derivative) of the boundary 
// 
// /gtt (output) : the acceleration of the boundary, g'', (if option==0). 
// /grid (input) : which grid
// /t0 (input) : time to evaluate the acceleration. 
// /option (input): if option==2^2 then return g'' the second time derivative of the boundary motion. If 
//   option==2^3 then return g''', the third time derivative of the boundary motion. If option=2^2+2^3 then
//          return both g'' and g''' ( consecutively in the array gtt).
//
//\end{MovingGridsSolverInclude.tex}  
//=================================================================================
{
  assert( moveOption(grid)==userDefinedMovingGrid );

  const bool computeTwoTimeDerivatives  = (option/4) % 2;
  const bool computeThreeTimeDerivatives= (option/8) % 2;
  Range R2 = mg.numberOfDimensions();  // put gtt in these components
  Range R3 = mg.numberOfDimensions();  // put gttt in these components
  if( computeTwoTimeDerivatives )
    R3=R2+mg.numberOfDimensions();

  #ifdef USE_PPP
    realSerialArray vertex; getLocalArrayWithGhostBoundaries(mg.vertex(),vertex);
  #else
    const realSerialArray & vertex = mg.vertex();
  #endif

  Index I1,I2,I3;
  getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);

  if( userDefinedMotionOption==linearMotion )
  {
    const real dv[3]={rpar[2],rpar[3],rpar[4]};

    real a=rpar[0], p=rpar[1];

    if( computeTwoTimeDerivatives )
    {
      // compute the acceleration:
      real accel;
        if( p==0. || p==1. ) 
	accel=0.;
      else
      {
	accel=a*p*(p-1.)*pow(t0,p-2.); 
      }
      for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
      {
	gtt(I1,I2,I3,dir)=accel*dv[dir];
      }
    }
    if( computeThreeTimeDerivatives )
    {
      // compute the third time derivative
      real factor; 
      if( p==0. || p==1. || p==2. ) 
      {
	factor=0.;
      }
      else
      {
	factor=a*p*(p-1.)*(p-2.)*pow(t0,p-3.); 
      }

      int base=R3.getBase();
      for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
      {
	gtt(I1,I2,I3,dir+base)=factor*dv[dir];
      }
    }
    
  }
  else if( userDefinedMotionOption==sinusoidalMotion )
  {
    const real xv[3]={rpar[0],rpar[1],rpar[2]};
    const real dv[3]={rpar[3],rpar[4],rpar[5]};
    const real ta=rpar[6], omega=rpar[7], beta=rpar[8];
    
    // Sinusoidal motion:
    // x(t) = x0(t) + dv*pow( .5-.5*cos(2*Pi*omega*(t-ta)), beta ) 
    // v(t) = dv*[ beta*pow(g,beta-1)*.5*2*Pi*omega*sin(2*Pi*omega*(t-ta))
    // a(t) = dv*[ beta*(beta-1)*pow(g,beta-2)*SQR( .5*2*Pi*omega*sin(2*Pi*omega*(t-ta)) )
    //             +beta*pow(g,beta-1)*.5* SQR(2*Pi*omega)*cos(2*Pi*omega*(t-ta)) ] 

    if( computeTwoTimeDerivatives )
    {
      real accel;
      if( beta==1 )
      {
	accel=.5*SQR(twoPi*omega)*cos(twoPi*omega*(t0-ta));
      }
      else
      {
	real g=.5-.5*cos(twoPi*omega*(t0-ta));
	real gDot=.5*twoPi*omega*sin(twoPi*omega*(t0-ta));

	accel = beta*(beta-1.)*pow( g,beta-2.)*gDot*gDot + 
	  beta*pow( g , beta-1. )* .5*SQR(twoPi*omega)*cos(twoPi*omega*(t0-ta));

	// printf("sinusoidal motion: t=%7.5f dv[0]*accel=%10.4e\n",t0,dv[0]*accel);

      }

      for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
      {
	gtt(I1,I2,I3,dir)=accel*dv[dir];
      }
    }
    if( computeThreeTimeDerivatives )
    {
      // I don't think this option should occur 
      Overture::abort("Error:sinusoidalMotion: not implemented");
    }
    
  }
  else if( userDefinedMotionOption==rampMotion )
  {
    // x(t) = a*ramp(t/rampInterval)*dv[]
    const real a=rpar[0], rampInterval=rpar[1];
    const real dv[3]={rpar[2],rpar[3],rpar[4]};
    const real ts=t0/rampInterval;

    if( computeTwoTimeDerivatives )
    {
      // compute the acceleration:
      const real accel = ts<=1. ? a*ramp3tt(ts)/rampInterval : 0.; 

      for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
      {
	gtt(I1,I2,I3,dir)=accel*dv[dir];
      }
    }
    if( computeThreeTimeDerivatives )
    {
      // I don't think this option should occur 
      Overture::abort("Error:rampMotion: not implemented");
    }
    
  }
  else
  {
    printf("MovingGrids::getUserDefinedBoundaryAcceleration:ERROR unknown userDefinedMotionOption=%i\n",
           userDefinedMotionOption);
    Overture::abort("ERROR");
  }
  return 0;
}


//\begin{>>MovingGridsSolverInclude.tex}{\subsection{updateUserDefinedMotion}} 
int MovingGrids::
updateUserDefinedMotion(CompositeGrid & cg, GenericGraphicsInterface & gi)
// ==========================================================================================
// /Description: 
//   This function is called by MovingGrids::update and can be used to define a user defined
// grid motion.
// 
//\end{MovingGridsSolverInclude.tex}  
// ==========================================================================================
{
  userDefinedMotionOption=linearMotion; // default
  rpar[0]=1.; rpar[1]=2.; 
  
  const aString menu[]=
    {
      "linear motion",
      "sinusoidal motion",
      "ramp motion",
      "done",
      ""
    }; 
  gi.appendToTheDefaultPrompt("userDefinedMotion>");
  aString answer;
  for( ;; ) 
  {

    int response=gi.getMenuItem(menu,answer,"Choose the movement");
    
    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( answer=="linear motion" )
    {
      userDefinedMotionOption=linearMotion;

      rpar[2]=1.; rpar[3]=0.;  rpar[4]=0.;  // here is the direction of the acceleration

      printf("linear motion: x(t) = a*t^p\n");
      gi.inputString(answer,"Enter a,p for x(t) = a*t^p");
      sScanF(answer,"%e %e",&rpar[0],&rpar[1]);
      printf("The linear motion parameters are a=%e, p=%e\n",rpar[0],rpar[1]);
    }
    else if( answer=="sinusoidal motion" )
    {
      userDefinedMotionOption=sinusoidalMotion;

      printf("sinusoidal motion: (x,y,z)(t) = (x0,x1,x2) + (d0,d1,d2){ [ 1-cos( (t-ta)*(omega *2*pi) ) ]^beta }\n");

      real ta=0., omega=1., beta=1.;
      rpar[0]=0.; rpar[1]=0.;  rpar[2]=0.;  // here is x0
      rpar[3]=1.; rpar[4]=0.;  rpar[5]=0.;  // here is the direction of the motion
      rpar[6]=ta;
      rpar[7]=omega;
      rpar[8]=beta;
      

      gi.inputString(answer,"Enter x0,x1,x2, d0,d1,d2, ta, omega, beta");
      sScanF(answer,"%e %e %e %e %e %e %e %e %e",&rpar[0],&rpar[1],&rpar[2],
                  &rpar[3],&rpar[4],&rpar[5], &rpar[6],&rpar[7],&rpar[8]);
      printf("The sinusoidal motion parameters are: \n"
             "  (x0,x1,x2)=(%9.3e,%9.3e,%9.3e) (d0,d1,d2)=(%9.3e,%9.3e,%9.3e)\n"
             "  ta=%9.3e, omega=%9.3e, beta=%9.3e\n"
                ,rpar[0],rpar[1],rpar[2], rpar[3],rpar[4],rpar[5], rpar[6],rpar[7],rpar[8]);
    }
    else if( answer=="ramp motion" )
    {
      userDefinedMotionOption=rampMotion;

      real a=1., rampInterval=1.;
      rpar[0]=a; rpar[1]=rampInterval;
      rpar[2]=1.; rpar[3]=0.;  rpar[4]=0.;  // here is the direction of the acceleration

      printf("ramp motion: The velocity: (v0,v1,v2)(t) = a*ramp(t/rampInterval)*(d0,d1,d2)\n");
      printf(" Where ramp(t) increases monotonically from 0 to 1 as t increases from 0 to 1\n");
      printf("  and where ramp(t)=1 for t>1\n");
      
      gi.inputString(answer,"Enter a, rampInterval, d0,d1,d2");
      sScanF(answer,"%e %e %e %e %e",&rpar[0],&rpar[1],&rpar[2],&rpar[3],&rpar[4]);
      printf("The linear motion parameters are a=%e, rampInterval=%e, (d0,d1,d2)=(%9.3e,%9.3e,%9.3e)\n",
                 rpar[0],rpar[1],rpar[2], rpar[3],rpar[4]);
    }
    else
    {
      cout << "unknown response=[" << answer << "]\n";
      gi.stopReadingCommandFile();
    }
    
  }
  gi.unAppendTheDefaultPrompt();
  return 0;
}
