#include "BeamModel.h"
#include "display.h"
#include "TravelingWaveFsi.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"

// standing wave: 
#define W(x,t) (amplitude*sin(k*(x))*cos(w*(t+tsw)))
#define Wt(x,t) (-w*amplitude*sin(k*(x))*sin(w*(t+tsw)))
#define Wtt(x,t) (-w*w*amplitude*sin(k*(x))*cos(w*(t+tsw)))

#define Wx(x,t) (k*amplitude*cos(k*(x))*cos(w*(t+tsw)))
#define Wtx(x,t) (-w*k*amplitude*cos(k*(x))*sin(w*(t+tsw)))
#define Wttx(x,t) (-w*w*k*amplitude*cos(k*(x))*cos(w*(t+tsw)))

// Longfei 20160118:
// Now, evaluate solutions only, no more x derivatives in base class.
// x derivatives are evaluated in subclasses as needed

// =====================================================================================
/// \brief Evaluate the exact solution (if any)
/// \param t (input) : assign values at this time.
/// \param u,v (output) : displacement and velocity
// =====================================================================================
int BeamModel::
getExactSolution( real t, RealArray & u, RealArray & v, RealArray & a ) const
{

  // Longfei 20160118: new way to redim solution array
  redimSolutionArray(u,v,a);  // u(I1,I2,I3,c) represents displacement in c-axis. c could be 0,1,2

  //Longfei 20160121: new way of handling parameters
  const real & beamLength = dbase.get<real>("length");
  const aString & exactSolutionOption = dbase.get<aString>("exactSolutionOption");
  // old way
  //real beamLength=L;
  //real breadth=1.;

  if( exactSolutionOption=="standingWave" )
    {
      return getStandingWave( t,u,v,a );
    }
  else if( exactSolutionOption=="eigenmode" )
    {
      getBeamEigenmode( t,u,v,a  );
    }
  else if( exactSolutionOption=="beamPiston" )
    {
      getBeamPiston( t,u,v,a  );
    }
  else if( exactSolutionOption=="beamUnderPressure" )
    {
      getBeamUnderPressure( t,u,v,a  );
    }
  else if( exactSolutionOption=="travelingWaveFSI" )
    {
      getTravelingWaveFSI( t,u,v,a  );
    }
  // Longfei 20160120: removed
  // else if( exactSolutionOption=="oldTravelingWaveFsi" )
  // { // old way
  //   setExactSolution( t,u,v,a);
  // }  
  else if( exactSolutionOption=="twilightZone" ) 
    {
      // Longfei 20160118: new way to compute twilightzone exact solution
      getTwilightZone(t,u,v,a);
    
    }
  else
    {
      u=0.; v=0; a=0; 
    }

  return 0;
}


// =====================================================================================
/// \brief Evaluate the standing wave solution
/// \param t (input) : assign values at this time.
/// \param u,v (output) : displacement and velocity
// =====================================================================================
int BeamModel::
getStandingWave( real t, RealArray & u, RealArray & v, RealArray & a ) const
{
  
  // Longfei 20160118:
  // dimension should be updated  already!
  assert(u.getLength(0)!=0 && v.getLength(0)!=0 && a.getLength(0)!=0);
  assert(dbase.get<int>("numberOfMotionDirections")==1); //StandingWave exact solution works for beam moving  in 1 direction
  
  // Longfei 20160121: new way of handling parameters
  const real & beamLength = dbase.get<real>("length");
  const real & density = dbase.get<real>("density");
  const real & thickness = dbase.get<real>("thickness");
  const real & breadth = dbase.get<real>("breadth");
  const real & elasticModulus = dbase.get<real>("elasticModulus");
  const real & areaMomentOfInertia = dbase.get<real>("areaMomentOfInertia");

  
  const real & amplitude=dbase.get<real>("amplitude");
  const real & waveNumber=dbase.get<real>("waveNumber");

  const real & tension=dbase.get<real>("tension");

  //real beamLength=L;
  //real k=2.*Pi*waveNumber/L;
  real k=2.*Pi*waveNumber/beamLength;
  
  real w = sqrt( (elasticModulus*areaMomentOfInertia*pow(k,4) +tension*k*k)/( density*thickness*breadth ) );

  // time offset for the standing wave: 
  real tsw = (2.*Pi/w)*dbase.get<real>("standingWaveTimeOffset");


  
  const bool xd = dbase.get<bool>("isCubicHermiteFEM");
  const int & numElem = dbase.get<int>("numElem");
  const int & numGhost = dbase.get<int>("numberOfGhostPoints");
  Index I1 = Range(-numGhost,numElem+numGhost); // index of all nodes (include ghost nodes if any)
  //Longfei 20160118: loop over all nodes (include ghost if any)
  for (int i = I1.getBase(); i <= I1.getBound(); ++i)
    {

      real xl = ( (real)i /numElem) *  beamLength;

      if(xd)
	{
	  // each nodes has 2 solutions, u and ux
	  u(2*i,0,0,0)   = W(xl,t);     // w 
	  v(2*i,0,0,0)   = Wt(xl,t);    // w_t 
	  a(2*i,0,0,0)   = Wtt(xl,t);    // w_tt 
	  u(2*i+1,0,0,0) = Wx(xl,t);    // w_x
	  v(2*i+1,0,0,0) = Wtx(xl,t);   // w_tx 
	  a(2*i+1,0,0,0) = Wttx(xl,t);   // w_ttx
	}
      else
	{
	  // each nodes has 1 solution: u
	  u(i,0,0,0)   = W(xl,t);     // w 
	  v(i,0,0,0)   = Wt(xl,t);    // w_t 
	  a(i,0,0,0)   = Wtt(xl,t);    // w_tt 
	}
    

    }
  return 0;
}

// =====================================================================================
/// \brief Evaluate the eigemode solution.
/// \param t (input) : assign values at this time.
/// \param u,v (output) : displacement and velocity
// =====================================================================================
int BeamModel::
getBeamEigenmode( real t, RealArray & u, RealArray & v, RealArray & a ) const
{
  
  // Longfei 20160118:
  // dimension should be updated  already!
  assert(u.getLength(0)!=0 && v.getLength(0)!=0 && a.getLength(0)!=0);
  assert(dbase.get<int>("numberOfMotionDirections")==1); //BeamEigenmode exact solution works for beam moving  in 1 direction


  const int & eigenMode = dbase.get<int>("eigenMode");

  const real & amplitude=dbase.get<real>("amplitude");
  const real & waveNumber=dbase.get<real>("waveNumber");

  const real & tension=dbase.get<real>("tension");


  // Longfei 20160121: new way of handling parameters
  //real beamLength=L;  // old way
  const  real &  beamLength= dbase.get<real>("length");
  const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  const BoundaryCondition & bcLeft =  boundaryConditions[0];
  const BoundaryCondition & bcRight =  boundaryConditions[1];

  // --- eigenvalues computed with cgDoc/moving/codes/beam/beamModes.maple ---

  // --- here are the cases we know so far --
  //
  real lambda;
  real c1,c2;
  if( bcLeft==clamped && bcRight==clamped )
    {
      // BC: clamped + clamped:
      //     cosh(lambda*L)*cos(lambda*L)=1 
      if( eigenMode==1 )
	lambda = 4.7300407448627040;
      else if( eigenMode==2 )
	lambda=7.8532046240958376;
      else if( eigenMode==3 )
	lambda = 1.0995607838001671e+01;
      else
	{
	  OV_ABORT("finish me");
	}
  
      c1 = -sinh(lambda)+sin(lambda);
      c2 =  cosh(lambda)-cos(lambda);
    }
  else if( bcLeft==clamped && bcRight==freeBC )
    {
      // BC: clamped + free:
      //     cosh(lambda*L)*cos(lambda*L)=-1 

      if( eigenMode==1 )
	lambda = 1.8751040687119612;
      else if( eigenMode==2 )
	lambda=4.6940911329741746;
      else if( eigenMode==3 )
	lambda = 7.8547574382376126;
      else
	{
	  OV_ABORT("finish me");
	}
  
      c1 = -sinh(lambda)-sin(lambda);
      c2 =  cosh(lambda)+cos(lambda);
    }
  else if( bcLeft==clamped && bcRight==slideBC )
    {
      // BC: clamped + slide
      //     sinh(lambda*L)*cos(lambda*L) + cosh()*sin() = 0

      if( eigenMode==1 )
	lambda = 2.3650203724313520;
      else if( eigenMode==2 )
	lambda=5.4978039190008355;
      else if( eigenMode==3 )
	lambda =8.6393798286997407;
      else
	{
	  OV_ABORT("finish me");
	}
  
      c1 = -cosh(lambda)+cos(lambda);
      c2 =  sinh(lambda)+sin(lambda);
    }
  else
    {
      OV_ABORT("finish me");
    }

  real cNorm = sqrt(c1*c1+c2*c2);
  const real amp=.2;  // approximate amplitude
  c1 *= amp/cNorm;
  c2 *= amp/cNorm;

  lambda = lambda/beamLength;  // scale by the beam length


#define WE(x) ( c1*( cosh(lambda*x) - cos(lambda*x) ) + c2*( sinh(lambda*x) -sin(lambda*x) ) )
#define WEx(x) ( lambda*( c1*( sinh(lambda*x) +sin(lambda*x) ) + c2*( cosh(lambda*x) -cos(lambda*x) ) ) )

  // Longfei 20160121: new way of handling parameters  
  //const real EI = elasticModulus*areaMomentOfInertia;
  const real & EI = dbase.get<real>("EI");
  const real & density = dbase.get<real>("density");
  const real & thickness = dbase.get<real>("thickness");
  const real & breadth = dbase.get<real>("breadth");
  const real & rhosAs= dbase.get<real>("massPerUnitLength");
  //const real rhosAs= density*thickness*breadth;

  const real & T = dbase.get<real>("tension");
  const real & K0 = dbase.get<real>("K0");
  const real & Kt = dbase.get<real>("Kt");
  const real & Kxxt = dbase.get<real>("Kxxt");

  assert( EI>0. && T==0. && K0==0. && Kt==0. && Kxxt==0. );
  
  real w =  SQR(lambda)*sqrt( EI/rhosAs );

  real cost = cos(w*t), sint=sin(w*t);

  const bool xd = dbase.get<bool>("isCubicHermiteFEM");
  const int & numElem = dbase.get<int>("numElem");
  const int & numGhost = dbase.get<int>("numberOfGhostPoints");
  Index I1 = Range(-numGhost,numElem+numGhost); // index of all nodes (include ghost nodes if any)
  //Longfei 20160118: loop over all nodes (include ghost if any)
  for (int i = I1.getBase(); i <= I1.getBound(); ++i)
    {
      real xl = ( (real)i /numElem) *  beamLength;
      real we = WE(xl);
      real wex = WEx(xl);
      
      if(xd)
	{
	  // each node has 2 solutions: u and ux
	  u(2*i,0,0,0)   = we*cost;     // w 
	  v(2*i,0,0,0)   = we*(-w*sint);    // w_t 
	  a(2*i,0,0,0)   = we*(-w*w*cost);    // w_tt
	  u(2*i+1,0,0,0) = wex*cost;    // w_x
	  v(2*i+1,0,0,0) = wex*(-w*sint);   // w_tx 
	  a(2*i+1,0,0,0) = wex*(-w*w*cost);   // w_ttx	
	}
      else
	{
	  // each node has 1 solution: u 
	  u(i,0,0,0)   = we*cost;     // w 
	  v(i,0,0,0)   = we*(-w*sint);    // w_t 
	  a(i,0,0,0)   = we*(-w*w*cost);    // w_tt
	}
    }
  return 0;
}



// =====================================================================================
/// \brief Evaluate the FSi traveling wave solution
/// \param t (input) : assign values at this time.
/// \param u,v (output) : displacement and velocity
// =====================================================================================
int BeamModel::
getTravelingWaveFSI( real t, RealArray & u, RealArray & v, RealArray & a ) const
{

  // Longfei 20160118:
  // dimension should be updated  already!
  assert(u.getLength(0)!=0 && v.getLength(0)!=0 && a.getLength(0)!=0);
  assert(dbase.get<int>("numberOfMotionDirections")==1); //BeamEigenmode exact solution works for beam moving  in 1 direction

 

  assert( dbase.get<TravelingWaveFsi*>("travelingWaveFsi")!=NULL );
  TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

  //Longfei 20160121: new way of handling parameters
  const real & dx=dbase.get<real>("elementLength");
  //real beamLength=L;
  //const real dx=beamLength/numElem;


  // int numGhost=1;
  // Index I1,I2,I3;
  // I1=Range(-numGhost,numElem+numGhost); I2=0; I3=0;

  const bool xd = dbase.get<bool>("isCubicHermiteFEM");
  const int & numElem = dbase.get<int>("numElem");
  const int & numGhost = dbase.get<int>("numberOfGhostPoints");
  Index I1 = Range(-numGhost,numElem+numGhost); // index of all nodes (include ghost nodes if any)
  Index I2=0,I3=0;
  
  int ng = xd? 1:0; // get one more solution on each side; so we can use centere FD to compute x derivatives FOR NOW
  Index II1 = Range( I1.getBase()-ng,I1.getBound()+ng);
  RealArray x(II1,I2,I3,2), ue(II1,I2,I3,2), ve(II1,I2,I3,2), ae(II1,I2,I3,2);


  for( int i1 =II1.getBase(); i1<=II1.getBound(); i1++ ) 
    {
      x(i1,0,0,0) = i1*dx; 
      x(i1,0,0,1) = 0.;    // should this be y0 ?
    }
  travelingWaveFsi.getExactShellSolution( x,ue,ve,ae, t, II1,I2,I3 );

  //Longfei 20160118: loop over all nodes (include ghost if any)
  for (int i = I1.getBase(); i <= I1.getBound(); ++i)
    {

      if(xd)
	{
	  // each nodes has 2 solutions: u and ux
	  u(2*i,0,0,0)   = ue(i,0,0,1);     // w 
	  v(2*i,0,0,0)   = ve(i,0,0,1);     // w_t 
	  a(2*i,0,0,0)   = ae(i,0,0,1);     // w_tt 
	  u(2*i+1,0,0,0) = (ue(i+1,0,0,1)-ue(i-1,0,0,1))/(2.*dx);  // w_x   *** DO THIS FOR NOW **
	  v(2*i+1,0,0,0) = (ve(i+1,0,0,1)-ve(i-1,0,0,1))/(2.*dx);      // w_xt *** DO THIS FOR NOW **
	  a(2*i+1,0,0,0) = (ae(i+1,0,0,1)-ae(i-1,0,0,1))/(2.*dx);      // w_xtt *** DO THIS FOR NOW **
	}
      else
	{
	  // each nodes has 1 solutions: u and ux
	  u(i,0,0,0)   = ue(i,0,0,1);     // w 
	  v(i,0,0,0)   = ve(i,0,0,1);     // w_t 
	  a(i,0,0,0)   = ae(i,0,0,1);     // w_tt 
	}

    }

  return 0;
}


// =====================================================================================
/// \brief Evaluate the "beam piston" exact solution (beam adjacent to one or two fluid domains)
/// \param t (input) : assign values at this time.
/// \param u,v (output) : displacement and velocity
// =====================================================================================
int BeamModel::
getBeamPiston( real t, RealArray & u, RealArray & v, RealArray & a ) const
{
  
  // Longfei 20160118:
  // dimension should be updated  already!
  assert(u.getLength(0)!=0 && v.getLength(0)!=0 && a.getLength(0)!=0);
  assert(dbase.get<int>("numberOfMotionDirections")==1); //BeamEigenmode exact solution works for beam moving  in 1 direction


  // ** NOTE this solution appears in userDefinedKnownSolution.C and beamInitialConditions.C ***
  const real *rpar = dbase.get<real[10]>("beamPistonPar");
  const real & ya          = rpar[0];    
  const real & yb          = rpar[1];    
  const real & pa          = rpar[2];   
  const real & pb          = rpar[3];   
  const real & rhos        = rpar[4];    
  const real & hs          = rpar[5];    
  const real & fluidHeight = rpar[6];    
  const real & K0          = rpar[7];    
  const real & Kt          = rpar[8];    

  assert( K0!=0. && Kt==0. );
  
  const real dp = pa-pb;
  // -- hard code these for now: **FIX ME**
  const real rho=1;

  const real omega=sqrt(K0/(rhos*hs+rho*fluidHeight));
  const real w    = (dp/K0)*(1. - cos(omega*t) );   // beam position y=w(t) 
  const real wt   = (dp/K0)*omega*sin(omega*t);
  const real wtt  = (dp/K0)*omega*omega*cos(omega*t);
  const real dpdy =-rho*wtt;  // dp/dy = -rho*w_tt

  const real & dt = dbase.get<real>("dt");
  if( t<=0 || t<=3.*dt )
    printF("--BM-- getBeamPiston solution at t=%9.3e, rhos*hs=%8.2e, rho*H=%8.2e, K0=%8.2e, pa=%g pb=%g dpdy=%g"
           " w=%g, wt=%g, wtt=%g\n",
	   t,rhos*hs,rho*fluidHeight,K0,pa,pb,dpdy,w,wt,wtt);

  const bool xd = dbase.get<bool>("isCubicHermiteFEM");
  const int & numElem = dbase.get<int>("numElem");
  const int & numGhost = dbase.get<int>("numberOfGhostPoints");
  Index I1 = Range(-numGhost,numElem+numGhost); // index of all nodes (include ghost if any)
  //Longfei 20160118: loop over all nodes (include ghost if any)
  for (int i = I1.getBase(); i <= I1.getBound(); ++i)
    {

      if(xd)
	{
	  // each node has 2 solutions: u and ux
	  u(2*i,0,0,0)   = w;      // w 
	  v(2*i,0,0,0)   = wt;     // w_t 
	  a(2*i,0,0,0)   = wtt;    // w_tt 
	  u(2*i+1,0,0,0) = 0.;     // w_x
	  v(2*i+1,0,0,0) = 0.;     // w_tx 
	  a(2*i+1,0,0,0) = 0.;     // w_ttx	  
	}
      else
	{
	  // each node has 1 solution: u
	  u(i,0,0,0)   = w;      // w 
	  v(i,0,0,0)   = wt;     // w_t 
	  a(i,0,0,0)   = wtt;    // w_tt 
	}
      

    }


  return 0;
}

// =====================================================================================
/// \brief Evaluate the "beam under pressure" exact solution 
/// \param t (input) : assign values at this time.
/// \param u,v (output) : displacement and velocity
// =====================================================================================
int BeamModel::
getBeamUnderPressure( real t, RealArray & u, RealArray & v, RealArray & a ) const
{
  // if( u.getLength(0)==0 )
  //   u.redim(2*numElem+2);
  // if( v.getLength(0)==0 )
  //   v.redim(2*numElem+2);
  // if( a.getLength(0)==0 )
  //   a.redim(2*numElem+2);
  
  // Longfei 20160118:
  // dimension should be updated  already!
  assert(u.getLength(0)!=0 && v.getLength(0)!=0 && a.getLength(0)!=0);
  assert(dbase.get<int>("numberOfMotionDirections")==1); //BeamEigenmode exact solution works for beam moving  in 1 direction

 

  //Longfei 20160120: new way of handling parameters
  const real & elasticModulus = dbase.get<real>("elasticModulus");
  const real & areaMomentOfInertia = dbase.get<real>("areaMomentOfInertia");
  const real & beamLength= dbase.get<real>("length");
  //real beamLength=L;
  
  const real *rpar = dbase.get<real[10]>("beamUnderPressurePar");
  const real & dp          = rpar[0];    

  const real & tension = dbase.get<real>("tension");
  
  const real & dt = dbase.get<real>("dt");
  if( t<=0 || t<=3.*dt )
    printF("--BM-- getBeamUnderPressure solution at t=%9.3e, dp=%8.2e\n",t,dp);

  real factor;
  if( tension!=0. && elasticModulus==0. )
    factor = dp/(2.*tension);
  else if( tension==0. && elasticModulus!=0. )
    factor = dp/(24.*elasticModulus*areaMomentOfInertia);
  else
    {
      OV_ABORT("BeamModel::getBeamUnderPressure:ERROR -- finish me--");
    }

  real xa=0, xb=beamLength;

  // TODO: Workout the solution for E!=0 AND T!=0 ******
  
  const bool xd = dbase.get<bool>("isCubicHermiteFEM");
  const int & numElem = dbase.get<int>("numElem");
  const int & numGhost = dbase.get<int>("numberOfGhostPoints");
  Index I1 = Range(-numGhost,numElem+numGhost); // index of all nodes (include ghost if any)
  //Longfei 20160118: loop over all nodes (include ghost if any)
  for (int i = I1.getBase(); i <= I1.getBound(); ++i)
    {  
      real x = ( (real)i /numElem) *  beamLength;
      if( tension!=0. && elasticModulus==0. )
	{ // solution is a quadratic
	  if (xd)
	    {
	      // each node has 2 solutions: u and ux
	      u(2*i,0,0,0)   = factor*(x-xa)*(xb-x);           // w 
	      u(2*i+1,0,0,0) = factor*( (xb-x) - (x-xa) );     // w_x
	    }
	  else
	    {
	      // each node has 1 solution: u
	      u(i,0,0,0)   = factor*(x-xa)*(xb-x);           // w 
	    }
	}
      else if( tension==0. && elasticModulus!=0. )
	{ // solution is a quartic
	  if(xd)
	    {
	      u(2*i,0,0,0)   = factor*SQR(x-xa)*SQR(x-xb);     // w
	      u(2*i+1,0,0,0) = factor*( 2.*(x-xa)*SQR(x-xb) + 2.*(x-xb)*SQR(x-xa) );     // w_x
	    }
	  else
	    {
	      u(i,0,0,0)   = factor*SQR(x-xa)*SQR(x-xb);     // w 
	    }
	}
    
      if(xd)
	{
	  v(2*i,0,0,0)   = 0.;     // w_t 
	  a(2*i,0,0,0)   = 0.;    // w_t
	  v(2*i+1,0,0,0) = 0.;     // w_tx 
	  a(2*i+1,0,0,0) = 0.;     // w_ttx
	}
      else
	{
	  v(i,0,0,0)   = 0.;     // w_t 
	  a(i,0,0,0)   = 0.;    // w_tt
	}
    }


  return 0;
}


// =====================================================================================
/// \brief Evaluate the twilightzone exact solution 
/// \param t (input) : assign values at this time.
/// \param u,v, a (output) : displacement,  velocity and acceleration
// =====================================================================================
int BeamModel::
getTwilightZone( real t, RealArray & u, RealArray & v, RealArray & a ) const
{

  // Longfei 20160118:
  // dimension should be updated  already!
  assert(u.getLength(0)!=0 && v.getLength(0)!=0 && a.getLength(0)!=0);
  
 

  // Longfei 20160121: new way of handling parameters
  const real & dx = dbase.get<real>("elementLength");
  // old way:
  //real beamLength=L;
  //real breadth=1.;
  //const real dx=beamLength/numElem;

  const int & domainDimension = dbase.get<int>("domainDimension");
  const bool & twilightZone = dbase.get<bool>("twilightZone");
  assert( twilightZone );
  assert( dbase.get<OGFunction*>("exactPointer")!=NULL );
  OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");


  const bool xd = dbase.get<bool>("isCubicHermiteFEM");
  const int & numElem = dbase.get<int>("numElem");
  const int & numGhost = dbase.get<int>("numberOfGhostPoints");
  Index I1 = Range(-numGhost,numElem+numGhost); // index of all nodes (include ghost if any)
  Index I2=0,I3=0;
  
  RealArray x(I1,I2,I3,2);  // beam axis (undeformed)
  for( int i1 = I1.getBase(); i1<=I1.getBound(); i1++ )
    {
      x(i1,0,0,0) = i1*dx; 
      x(i1,0,0,1) = 0.;    // should this be y0 ?
    }

  RealArray ue(I1,I2,I3,1), ve(I1,I2,I3,1), ae(I1,I2,I3,1), uxe(I1,I2,I3,1), vxe(I1,I2,I3,1), axe(I1,I2,I3,1); 
  int isRectangular=0;
  const int wc=0;
  exact.gd( ue ,x,domainDimension,isRectangular,0,0,0,0,I1,I2,I3,wc,t );
  exact.gd( ve ,x,domainDimension,isRectangular,1,0,0,0,I1,I2,I3,wc,t );
  exact.gd( ae ,x,domainDimension,isRectangular,2,0,0,0,I1,I2,I3,wc,t );
  if(xd)
    {
      // get x derivatives
      exact.gd( uxe,x,domainDimension,isRectangular,0,1,0,0,I1,I2,I3,wc,t );
      exact.gd( vxe,x,domainDimension,isRectangular,1,1,0,0,I1,I2,I3,wc,t );
      exact.gd( axe,x,domainDimension,isRectangular,2,1,0,0,I1,I2,I3,wc,t );
    }
  
  // loop over all nodes (include ghost if any)
  for( int i = I1.getBase(); i<=I1.getBound(); i++ )
    {
  
      if(xd)
	{
	  u(2*i,0,0,0) = ue(i,0,0,0);
	  v(2*i,0,0,0) = ve(i,0,0,0);
	  a(2*i,0,0,0) = ae(i,0,0,0);
	  u(2*i+1,0,0,0) = uxe(i,0,0,0);
	  v(2*i+1,0,0,0) = vxe(i,0,0,0);
	  a(2*i+1,0,0,0) = axe(i,0,0,0);
	}
      else
	{
	  u(i,0,0,0) = ue(i,0,0,0) ;
	  v(i,0,0,0) = ve(i,0,0,0) ;
	  a(i,0,0,0) = ae(i,0,0,0) ;
	}
    }
	
  if( false )
    {
      RealArray uxe(I1,I2,I3,1);
      exact.gd( uxe,x,domainDimension,isRectangular,0,2,0,0,I1,I2,I3,wc,t );
      ::display(uxe,"uxx exact","%6.2f ");

      exact.gd( uxe,x,domainDimension,isRectangular,0,3,0,0,I1,I2,I3,wc,t );
      ::display(uxe,"uxxx exact","%6.2f ");
    
    }

}



// =====================================================================================
/// \brief Assign initial conditions
/// \param t (input) : assign values at this time.
/// \param u,v,a (output) : displacement, velocity and acceleration
// =====================================================================================
int BeamModel::
assignInitialConditions( real t, RealArray & u, RealArray & v, RealArray & a )
{
  const aString & initialConditionOption = dbase.get<aString>("initialConditionOption");

  if( debug & 1 )
    printF("BeamModel::assignInitialConditions for %s, t=%9.3e\n",(const char*)initialConditionOption,t);

  //real beamLength=L; //Longfei 20160121: this is not needed in this function
  
  // if( u.getLength(0)==0 )
  //   u.redim(2*numElem+2);
  // if( v.getLength(0)==0 )
  //   v.redim(2*numElem+2);
  // if( a.getLength(0)==0 )
  //   a.redim(2*numElem+2);

  // Longfei 20160118: new way to redim solution array
  redimSolutionArray(u,v,a);  // u(I1,I2,I3,c) represents displacement in c-axis. c could be 0,1,2
  

  if( initialConditionOption=="zero" || initialConditionOption=="none" )
    {
      u=0.;
      v=0.;
      a=0.;
    }
  else if( initialConditionOption=="exact" )
    {
      getExactSolution( t, u, v, a  ); 
    }
  // else if( initialConditionOption=="oldTravelingWaveFsi" )
  // {
  //   setExactSolution( t,u,v,a);
  // }
  // else if( initialConditionOption=="twilightZone" )
  // {
  //   bool & twilightZone = dbase.get<bool>("twilightZone");
  //   assert( twilightZone );
  //   assert( dbase.get<OGFunction*>("exactPointer")!=NULL );
  //   OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");

  //   Index I1,I2,I3;
  //   I1=Range(0,numElem); I2=0; I3=0;

  //   RealArray x(I1,I2,I3,2);  // beam axis (undeformed)
  //   const real dx=beamLength/numElem;
  //   for( int i1 = I1.getBase(); i1<=I1.getBound(); i1++ )
  //   {
  //     x(i1,0,0,0) = i1*dx; 
  //     x(i1,0,0,1) = 0.;    // should this be y0 ?
  //   }

  //   RealArray ue(I1,I2,I3,1), ve(I1,I2,I3,1), ae(I1,I2,I3,1), uxe(I1,I2,I3,1), vxe(I1,I2,I3,1), axe(I1,I2,I3,1);
  //   int isRectangular=0;
  //   const int wc=0;
  //   exact.gd( ue ,x,domainDimension,isRectangular,0,0,0,0,I1,I2,I3,wc,t );
  //   exact.gd( ve ,x,domainDimension,isRectangular,1,0,0,0,I1,I2,I3,wc,t );
  //   exact.gd( ae ,x,domainDimension,isRectangular,2,0,0,0,I1,I2,I3,wc,t );
  //   exact.gd( uxe,x,domainDimension,isRectangular,0,1,0,0,I1,I2,I3,wc,t );
  //   exact.gd( vxe,x,domainDimension,isRectangular,1,1,0,0,I1,I2,I3,wc,t );
  //   exact.gd( axe,x,domainDimension,isRectangular,2,1,0,0,I1,I2,I3,wc,t );
  //   for (int i = 0; i <= numElem; ++i)
  //   {
  //     u(i*2)   = ue(i,0,0,0);     // w 
  //     u(i*2+1) = uxe(i,0,0,0);    // w_x
    
  //     v(i*2)   = ve(i,0,0,0);     // w_t 
  //     v(i*2+1) = vxe(i,0,0,0);    // w_xt

  //     a(i*2)   = ae(i,0,0,0);     // w_tt 
  //     a(i*2+1) = axe(i,0,0,0);    // w_xtt

  //   }
  // }
  else
    {
      printF("BeamModel::assignInitialConditions:ERROR:unknown initialConditionOption=[%s]\n",
	     (const char*)initialConditionOption);
      OV_ABORT("error");
    }
  

  // assignBoundaryConditions(t,u,v,a );

  // if( t==0. )
  // {
  //   myPosition=u;
  //   myVelocity=v;
  //   myAcceleration=a;
  // }
  
  return 0;
}


// =====================================================================================
/// \brief Choose the exact solution.
// =====================================================================================
int BeamModel::
chooseExactSolution(CompositeGrid & cg, GenericGraphicsInterface & gi )
{
  aString & exactSolutionOption = dbase.get<aString>("exactSolutionOption");
  real & amplitude=dbase.get<real>("amplitude");
  real & waveNumber=dbase.get<real>("waveNumber");

  GUIState gui;
  gui.setWindowTitle("Beam exact solution");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;


  const int maxCommands=40;
  aString cmd[maxCommands];

  aString exactSolutionOptions[] = {"no exact solution",
				    "twilight zone",
				    "standing wave",
				    "traveling wave FSI-INS",
				    //"old traveling wave FSI-INS",
				    "beam piston",
                                    "beam under pressure",
                                    "eigenmode",
				    ""};
  GUIState::addPrefix(exactSolutionOptions,"Exact solution:",cmd,maxCommands);
  int option  =(exactSolutionOption=="none"                ? 0 : 
		exactSolutionOption=="twilightZone"        ? 1 : 
                exactSolutionOption=="standingWave"        ? 2 :
                exactSolutionOption=="travelingWaveFSI"    ? 3 :
                //exactSolutionOption=="oldTravelingWaveFsi" ? 4 :
                exactSolutionOption=="beamPiston"          ? 4 : 
                exactSolutionOption=="beamUnderPressure"   ? 5 : 
                exactSolutionOption=="eigenmode"           ? 6 : 
                0 );
  dialog.addOptionMenu("Exact solution:",cmd,exactSolutionOptions,option );

  const int numberOfTextStrings=10;
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
    
  textLabels[nt] = "amplitude:"; sPrintF(textStrings[nt], "%g",amplitude);  nt++; 
  textLabels[nt] = "wave number:"; sPrintF(textStrings[nt], "%g",waveNumber);  nt++; 
  textLabels[nt] = "standing wave t0:"; sPrintF(textStrings[nt], "%g",dbase.get<real>("standingWaveTimeOffset"));  nt++; 


  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  // addPrefix(textLabels,prefix,cmd,maxCommands);
  // dialog.setTextBoxes(cmd, textLabels, textStrings);
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("beamExact>");

  aString prefix = ""; // prefix for commands to make them unique.

  aString answer,buff;

  int len=0;
  for( ;; ) 
    {
	    
      gi.getAnswer(answer,"");
  
      // printF(answer,"answer=[answer]\n",(const char *)answer);

      if( answer(0,prefix.length()-1)==prefix )
	answer=answer(prefix.length(),answer.length()-1);


      if( answer=="done" || answer=="exit" )
	{
	  break;
	}
      else if( (len=answer.matches("Exact solution:")) )
	{
	  aString option = answer(len,answer.length()-1);
	  if( option=="no exact solution" )
	    {
	      exactSolutionOption="none";
	      printF("Setting exactSolutionOption=[%s]\n",(const char*)exactSolutionOption);
	    }
	  else if( option=="twilight zone" )
	    {
	      exactSolutionOption="twilightZone";
	      dbase.get<bool>("twilightZone")=true;
	      printF("Setting exactSolutionOption=[%s]\n",(const char*)exactSolutionOption);
	    }
	  else if( option=="standing wave" )
	    {
	      exactSolutionOption="standingWave"; 
	      printF("Setting exactSolutionOption=[%s]\n",(const char*)exactSolutionOption);
	    }
	  else if( option=="eigenmode" )
	    {
	      exactSolutionOption="eigenmode";

	      if( !dbase.has_key("eigenMode") )
		dbase.put<int>("eigenMode")=1;

	      int & eigenMode = dbase.get<int>("eigenMode");
	      gi.inputString(answer,sPrintF("Enter the eigemode: 1,2,3,..."));
	      sScanF(answer,"%i",&eigenMode);
	      printF("Setting eigenMode=%i\n",eigenMode);

	    }
	  // Longfei 20160120: removed
	  // else if( option=="old traveling wave FSI-INS" )
	  // {
	  // 	// *old way*
	  // 	exactSolutionOption="oldTravelingWaveFsi";
	  // }
	  else if( option=="traveling wave FSI-INS" )
	    {
	      exactSolutionOption="travelingWaveFSI"; 
	      printF("Setting exactSolutionOption=[%s]\n",(const char*)exactSolutionOption);

	      printF("INFO:The FSI traveling wave solution is an exact solution for a solid (shell or bulk)\n"
		     "coupled to a linearized incompressible fluid\n"
		     "See: `An analysis of a new stable partitioned algorithm for FSI problems.\n"
		     "      Part I: Incompressible flow and elastic solids'. \n"
		     "      J.W. Banks, W.D. Henshaw and D.W. Schwendeman, JCP 2014.\n");
    
      
	      // *************** WE should share this object with the fluid domain ********************   **FIX ME*

	      if( !dbase.has_key("travelingWaveFsi") )
		dbase.put<TravelingWaveFsi*>("travelingWaveFsi")=NULL;

	      if( dbase.get<TravelingWaveFsi*>("travelingWaveFsi")==NULL )
		dbase.get<TravelingWaveFsi*>("travelingWaveFsi")= new TravelingWaveFsi; // who will delete ???
	      TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

	      travelingWaveFsi.update(gi );

	      // we also pass the grid for the solid:
	      CompositeGrid & cgSolid = cg; // do this for now  -- only used for number of grid points

	      int numberOfFluidGridPoints=21, numberOfSolidGridPoints=21;  // I don't think these matter
	      travelingWaveFsi.setup( numberOfFluidGridPoints, numberOfSolidGridPoints );


	    }
	  else if( option=="beam piston" )
	    {
	      exactSolutionOption="beamPiston"; 
	      printF("--BM-- The beam piston solution is an FSI solution for a horizontal beam adjacent to one or two fluid domains.\n"
		     " The vertical motion of the beam is of the form:\n"
		     "      y(t) = (dp/K0)*( 1 - cos(omega*t) )   (for an undamped beam, Kt=0)\n");

	      if( !dbase.has_key("beamPistonPar") )
		dbase.put<real[10]>("beamPistonPar");
	      real *rpar = dbase.get<real[10]>("beamPistonPar");

	      real & ya          = rpar[0];    
	      real & yb          = rpar[1];    
	      real & pa          = rpar[2];   
	      real & pb          = rpar[3];   
	      real & rhos        = rpar[4];    
	      real & hs          = rpar[5];    
	      real & fluidHeight = rpar[6];    
	      real & K0          = rpar[7];    
	      real & Kt          = rpar[8];    

	      gi.inputString(answer,sPrintF("Enter ya,yb,pa,pb,rhos,hs,fluidHeight,K0,Kt"));
	      sScanF(answer,"%e %e %e %e %e  %e %e %e %e %e %e %e %e",&ya,&yb,&pa,&pb,&rhos,&hs,&fluidHeight,&K0,&Kt);
	      printF("Setting ya=%g, yb=%g, pa=%g, pb=%g, rhos=%g, hs=%g, fluidHeight=%g, K0=%g, Kt=%g \n",
		     ya,yb,pa,pb,rhos,hs,fluidHeight,K0,Kt);   


	    }
	  else if( option=="beam under pressure" )
	    {
	      exactSolutionOption="beamUnderPressure"; 
	      printF("--BM-- The `beam under pressure' solution is a steady FSI solution for a horizontal beam \n"
		     " pinned (or clamped) on both ends and with constant pressure force.\n"
		     " The vertical displacement of the beam is of the form :\n"
		     "      y(t) = ( dp/(2T) )*(x-a)(b-x) )          (for T!=0, E=0)\n"
		     "      y(t) = ( dp/(24 EI) )*(x-a)^2*(b-x)^2 )  (for T=0, E!=0)\n"
		     "      dp = pressure force\n"
		     );

	      if( !dbase.has_key("beamUnderPressurePar") )
		dbase.put<real[10]>("beamUnderPressurePar");
	      real *rpar = dbase.get<real[10]>("beamUnderPressurePar");
	      real & dp          = rpar[0];    
	      gi.inputString(answer,sPrintF("Enter dp"));
	      sScanF(answer,"%e",&dp);
	      printF("Setting dp=%g\n",dp);

	    }
	  else
	    {
	      printF("BeamModel::chooseExactSolution:ERROR:unknown response=[%s]\n",(const char*)answer);
	      gi.stopReadingCommandFile();
	    }
      
	}
    
      else if( dialog.getTextValue(answer,"amplitude:","%g",amplitude) ){} //
      else if( dialog.getTextValue(answer,"wave number:","%g",waveNumber) ){} //
      else if( dialog.getTextValue(answer,"standing wave t0:","%g",dbase.get<real>("standingWaveTimeOffset")) ){} //
      else
	{
	  printF("BeamModel::chooseExactSolution:ERROR:unknown response=[%s]\n",(const char*)answer);
	  gi.stopReadingCommandFile();
	}



    }
    
  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

  return 0;
}




// =====================================================================================
/// \brief Choose initial conditions for beam models
// =====================================================================================
int BeamModel::
chooseInitialConditions(CompositeGrid & cg, GenericGraphicsInterface & gi )
{

  // real & amplitude=dbase.get<real>("amplitude");
  // real & waveNumber=dbase.get<real>("waveNumber");

  aString & initialConditionOption = dbase.get<aString>("initialConditionOption");
  
  GUIState gui;
  gui.setWindowTitle("Beam inital conditions");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;

  const int maxCommands=40;
  aString cmd[maxCommands];

  aString initialConditionOptions[] = { "zero",
					"exact solution",
                                        "none", 
					"" };

  GUIState::addPrefix(initialConditionOptions,"Initial conditions:",cmd,maxCommands);
  dialog.addOptionMenu("Initial conditions:",cmd,initialConditionOptions,(initialConditionOption=="zero" ? 0 : 
									  initialConditionOption=="exact" ? 1 : 2) );



  // aString pbLabels[] = {"zero initial conditions",
  //                       "exact solution initial conditions",
  // 			""};

  // int numRows=4;
  // dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

  // const int numberOfTextStrings=10;
  // aString textLabels[numberOfTextStrings];
  // aString textStrings[numberOfTextStrings];

  // int nt=0;
    
  // textLabels[nt] = "amplitude:"; sPrintF(textStrings[nt], "%g",amplitude);  nt++; 
  // textLabels[nt] = "wave number:"; sPrintF(textStrings[nt], "%g",waveNumber);  nt++; 
  // textLabels[nt] = "standing wave t0:"; sPrintF(textStrings[nt], "%g",dbase.get<real>("standingWaveTimeOffset"));  nt++; 


  // // null strings terminal list
  // textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  // // addPrefix(textLabels,prefix,cmd,maxCommands);
  // // dialog.setTextBoxes(cmd, textLabels, textStrings);
  // dialog.setTextBoxes(textLabels, textLabels, textStrings);


  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("beamIC>");

  aString prefix = ""; // prefix for commands to make them unique.

  aString answer,buff;

  int len=0;
  for( ;; ) 
    {
	    
      gi.getAnswer(answer,"");
  
      // printF(answer,"answer=[answer]\n",(const char *)answer);

      if( answer(0,prefix.length()-1)==prefix )
	answer=answer(prefix.length(),answer.length()-1);


      if( answer=="done" || answer=="exit" )
	{
	  break;
	}
      else if( (len=answer.matches("Initial conditions:")) )
	{
	  aString option = answer(len,answer.length()-1);
	  if( option=="zero" )
	    {
	      initialConditionOption="zero";
	      printF("Setting initialConditionOption=[%s]\n",(const char*)initialConditionOption);
	    }
	  else if( option=="exact solution" )
	    {
	      initialConditionOption="exact";
	      printF("Setting initialConditionOption=[%s]\n",(const char*)initialConditionOption);
	    }
	  else if( option=="none" )
	    {
	      initialConditionOption="none";
	      printF("Setting initialConditionOption=[%s]\n",(const char*)initialConditionOption);
	    }
	  else 
	    {
	      printF("BeamModel::chooseInitialConditions:ERROR:unknown response=[%s]\n",(const char*)answer);
	      gi.stopReadingCommandFile();
	    }
	}
    
      else
	{
	  printF("BeamModel::chooseInitialConditions:ERROR:unknown response=[%s]\n",(const char*)answer);
	  gi.stopReadingCommandFile();
	}

    }
    
  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

  return 0;
}
