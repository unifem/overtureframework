// ==================================================================================
// Macro to evaluate the traveling (shock) wave solution
// 
// evalSolution : true=eval solution, false=eval error
// ==================================================================================
#beginMacro getTravelingWaveSolution(evalSolution,U,ERR,X,t,I1,I2,I3)
//	printF("INFO: The traveling wave solutions are combinations of p and s solutions:\n"
//               "  [u1,u2] = ap* [ k1,k2] * G( k1*(x-xa)+k2*(y-ya) - cp*t )  (p-wave)\n"
//               "  [u1,u2] = as* [-k2,k1] * G( k1*(x-xa)+k2*(y-ya) - cs*t )  (s-wave\n",
//               "    where  G(xi)=0 for xi>0 and G(xi)=-1 for xi<0 \n");
{
  const int v1c = parameters.dbase.get<int >("v1c");
  const int v2c = parameters.dbase.get<int >("v2c");
  const int v3c = parameters.dbase.get<int >("v3c");

  bool assignVelocities= v1c>=0 ;
  const int s11c = parameters.dbase.get<int >("s11c");
  const int s12c = parameters.dbase.get<int >("s12c");
  const int s13c = parameters.dbase.get<int >("s13c");
  const int s21c = parameters.dbase.get<int >("s21c");
  const int s22c = parameters.dbase.get<int >("s22c");
  const int s23c = parameters.dbase.get<int >("s23c");
  const int s31c = parameters.dbase.get<int >("s31c");
  const int s32c = parameters.dbase.get<int >("s32c");
  const int s33c = parameters.dbase.get<int >("s33c");

  const int pc = parameters.dbase.get<int >("pc");
    
  bool assignStress = s11c >=0 ;

  // hard code some numbers for now: 
  // real k1=1., k2=0., k3=0.;
  // real ap=1., xa=.5, ya=0.;
  real cp = sqrt( (lambda+2.*mu)/rho );
  real cs = sqrt( mu/rho );

  std::vector<real> & twd = parameters.dbase.get<std::vector<real> >("travelingWaveData");
  const int np = int(twd[0]);  // number of p wave solutions
  const int ns = int(twd[1]);  // number of s wave solutions

  if( pdeVariation == SmParameters::hemp )
  {
    printF("\n\n **************** FIX ME: travelingWave: finish me for HEMP **********\n\n");
    // OV_ABORT("error");
  }
  

  // printF("**** travelingWave: cp=%8.2e, t=%8.2e v0=%8.2e *********\n",cp,t,v0);
  int i1,i2,i3;
  if( mg.numberOfDimensions()==2 )
  {
    real z0=0.;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      real x0 = X(i1,i2,i3,0);
      real y0 = X(i1,i2,i3,1);

      real u1=0., u2=0.;  // back-ground field
      real v1=0., v2=0.;
      real s11=0., s12=0., s22=0.;
      real div=0.;
      // real a1=0., b1=-1., a2=0., b2=0.;
      // u1 = a1*x0 + b1*t;  // more general back ground field
      // u2 = a2*x0 + b2*t;

      int m=2;
      for (int n=0; n<np; n++ ) // p-wave solutions
      {
	real ap = twd[m++], k1=twd[m++], k2=twd[m++], k3=twd[m++], xa=twd[m++], ya=twd[m++], za=twd[m++];

	real xi = k1*(x0-xa) + k2*(y0-ya) - cp*t;

	if( xi <= 0. )
	{
	  u1 += -ap*k1*xi;
	  u2 += -ap*k2*xi;
          v1 += ap*cp*k1;
          v2 += ap*cp*k2;
          s11+= -ap*( lambda+2.*mu*k1*k1 );
	  s12+= -ap*( 2.*mu*k1*k2 );
	  s22+= -ap*( lambda+2.*mu*k2*k2 );
          div+= -ap;
	}
      }
      for (int n=0; n<ns; n++ ) // s-wave solutions
      {
	real as = twd[m++], k1=twd[m++], k2=twd[m++], k3=twd[m++], xa=twd[m++], ya=twd[m++], za=twd[m++];

	real xi = k1*(x0-xa) + k2*(y0-ya) - cs*t;

	if( xi <= 0. )
	{  // (-k2,k1)
	  u1 += +as*k2*xi;
	  u2 += -as*k1*xi;
          v1 += -as*cs*k2;
          v2 += +as*cs*k1;
          s11+= -as*(-2.*mu*k1*k2 );
	  s12+= -as*( mu*(k1*k1-k2*k2) );
	  s22+= -as*( 2.*mu*k1*k2 );
	}
      }
      
      // printF(" (i1,i2)=(%i,%i) (x0,y0)=(%8.2e,%8.2e) xi=%8.2e (u1,u2)=(%8.2e,%8.2e)\n",i1,i2,x0,y0,xi,u1,u2);

      if( evalSolution )
      {
	if( pdeVariation == SmParameters::hemp )
	{
	  U(i1,i2,i3,u1c) =u1;
	  U(i1,i2,i3,u2c) =u2;
	  U(i1,i2,i3,uc) =x0;
	  U(i1,i2,i3,vc) =y0;

	  state0Local(i1,i2,i3,1) = 1.0; // density
	  /*********/
	  state0Local(i1,i2,i3,0) = state0Local(i1,i2,i3,1)*det(i1,i2,i3)*mg.gridSpacing(axis1)*mg.gridSpacing(axis2); // mass
	  if( pdeVariation == SmParameters::hemp && 
	      i1 > I1Base && i1 < I1Bound &&
	      i2 > I2Base && i2 < I2Bound )
	  {
	    real area = 0.25*(det(i1,i2,i3)+det(i1+1,i2,i3)+det(i1+1,i2+1,i3)+det(i1,i2+1,i3));
	    state0Local(i1,i2,i3,0) = area*mg.gridSpacing(axis1)*mg.gridSpacing(axis2); // mass
	  }

	}
	else
	{
	U(i1,i2,i3,uc) =u1;
	U(i1,i2,i3,vc) =u2;
	}
	
        if( assignVelocities )
	{
	  U(i1,i2,i3,v1c) = v1;
	  U(i1,i2,i3,v2c) = v2;
	}
	if( assignStress )
	{
	  U(i1,i2,i3,s11c) =s11;
	  U(i1,i2,i3,s12c) =s12;
	  U(i1,i2,i3,s21c) =s12;
	  U(i1,i2,i3,s22c) =s22;
	  if( pdeVariation == SmParameters::hemp )
	  {
            real press = -(lambda+2.0*mu/3.0)*div;
            U(i1,i2,i3,pc)   = press;
            U(i1,i2,i3,s11c) += press;
            U(i1,i2,i3,s22c) += press;
	  }
          
	}
      }
      else
      {
	if( pdeVariation == SmParameters::hemp )
	{
	  ERR(i1,i2,i3,u1c) = U(i1,i2,i3,u1c) - u1;
	  ERR(i1,i2,i3,u2c) = U(i1,i2,i3,u2c) - u2;
	  ERR(i1,i2,i3,uc) = U(i1,i2,i3,uc) - x0;
	  ERR(i1,i2,i3,vc) = U(i1,i2,i3,vc) - y0;
	}
	else
	{
	  ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
	  ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
	  }
	  
        if( assignVelocities )
	{
	  ERR(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
	  ERR(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
	}
	if( assignStress )
	{
	  ERR(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
	  ERR(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
	  ERR(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s12;
	  ERR(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
	  if( pdeVariation == SmParameters::hemp )
	  {
            real press = -(lambda+2.0*mu/3.0)*div;
            ERR(i1,i2,i3,s11c) -= press;
	    ERR(i1,i2,i3,s22c) -= press;
	    ERR(i1,i2,i3,pc)   = U(i1,i2,i3,pc)   - press;
	  }
	  
	}
	
      }
    } // end FOR_3D

  }
  else
  {
    OV_ABORT("Error: finish me");
  }
}

#endMacro

// ==================================================================================
// Macro to evaluate the traveling (sine) wave solution
// 
// evalSolution : true=eval solution, false=eval error
// ==================================================================================
#beginMacro getPlaneTravelingWaveSolution(evalSolution,U,ERR,X,t,I1,I2,I3)
//	printF("INFO: The traveling wave solutions are combinations of p and s solutions:\n"
//               "  [u1,u2] = ap* [ k1,k2] * G( k1*(x-xa)+k2*(y-ya) - cp*t )  (p-wave)\n"
//               "  [u1,u2] = as* [-k2,k1] * G( k1*(x-xa)+k2*(y-ya) - cs*t )  (s-wave\n",
//               "    where  G(xi)=sin(freq,xi) \n");
{
  const int v1c = parameters.dbase.get<int >("v1c");
  const int v2c = parameters.dbase.get<int >("v2c");
  const int v3c = parameters.dbase.get<int >("v3c");

  bool assignVelocities= v1c>=0 ;
  const int s11c = parameters.dbase.get<int >("s11c");
  const int s12c = parameters.dbase.get<int >("s12c");
  const int s13c = parameters.dbase.get<int >("s13c");
  const int s21c = parameters.dbase.get<int >("s21c");
  const int s22c = parameters.dbase.get<int >("s22c");
  const int s23c = parameters.dbase.get<int >("s23c");
  const int s31c = parameters.dbase.get<int >("s31c");
  const int s32c = parameters.dbase.get<int >("s32c");
  const int s33c = parameters.dbase.get<int >("s33c");

  const int pc = parameters.dbase.get<int >("pc");
    
  bool assignStress = s11c >=0 ;

  // hard code some numbers for now: 
  // real k1=1., k2=0., k3=0.;
  // real ap=1., xa=.5, ya=0.;
  real cp = sqrt( (lambda+2.*mu)/rho );
  real cs = sqrt( mu/rho );

  std::vector<real> & twd = parameters.dbase.get<std::vector<real> >("travelingWaveData");
  const int np = int(twd[0]);  // number of p wave solutions
  const int ns = int(twd[1]);  // number of s wave solutions

  if( pdeVariation == SmParameters::hemp )
  {
    printF("\n\n **************** FIX ME: travelingWave: finish me for HEMP **********\n\n");
    // OV_ABORT("error");
  }
  

  // printF("**** planeTravelingWave: cp=%8.2e, t=%8.2e  *********\n",cp,t);

  int i1,i2,i3;
  if( mg.numberOfDimensions()==2 )
  {
    real z0=0.;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      real x0 = X(i1,i2,i3,0);
      real y0 = X(i1,i2,i3,1);

      real u1=0., u2=0.;  // back-ground field
      real v1=0., v2=0.;
      real s11=0., s12=0., s22=0.;
      real div=0.;
      // real a1=0., b1=-1., a2=0., b2=0.;
      // u1 = a1*x0 + b1*t;  // more general back ground field
      // u2 = a2*x0 + b2*t;

      int m=2;
      for (int n=0; n<np; n++ ) // p-wave solutions
      {
	real ap = twd[m++], k1=twd[m++], k2=twd[m++], k3=twd[m++], xa=twd[m++], ya=twd[m++], za=twd[m++];
	real freq=twd[m++];
	
	real xi = k1*(x0-xa) + k2*(y0-ya) - cp*t;
        real sinf = sin(freq*xi), cosf=cos(freq*xi);

	u1 += -ap*k1*sinf;
	u2 += -ap*k2*sinf;
	v1 += ap*cp*k1*freq*cosf;
	v2 += ap*cp*k2*freq*cosf;
	s11+= -ap*( lambda+2.*mu*k1*k1 )*freq*cosf;
	s12+= -ap*( 2.*mu*k1*k2        )*freq*cosf;
	s22+= -ap*( lambda+2.*mu*k2*k2 )*freq*cosf;
	div+= -ap;
      }
      for (int n=0; n<ns; n++ ) // s-wave solutions
      {
	real as = twd[m++], k1=twd[m++], k2=twd[m++], k3=twd[m++], xa=twd[m++], ya=twd[m++], za=twd[m++];

	real xi = k1*(x0-xa) + k2*(y0-ya) - cs*t;
	real freq=twd[m++];
	real sinf = sin(freq*xi), cosf=cos(freq*xi);
	
	// (-k2,k1)
	u1 += +as*k2*sinf;
	u2 += -as*k1*sinf;
	v1 += -as*cs*k2*freq*cosf;
	v2 += +as*cs*k1*freq*cosf;
	s11+= -as*(-2.*mu*k1*k2      )*freq*cosf;
	s12+= -as*( mu*(k1*k1-k2*k2) )*freq*cosf;
	s22+= -as*( 2.*mu*k1*k2      )*freq*cosf;

      }
      
      // printF(" (i1,i2)=(%i,%i) (x0,y0)=(%8.2e,%8.2e) xi=%8.2e (u1,u2)=(%8.2e,%8.2e)\n",i1,i2,x0,y0,xi,u1,u2);

      if( evalSolution )
      {
	if( pdeVariation == SmParameters::hemp )
	{
	  U(i1,i2,i3,u1c) =u1;
	  U(i1,i2,i3,u2c) =u2;
	  U(i1,i2,i3,uc) =x0;
	  U(i1,i2,i3,vc) =y0;

	  state0Local(i1,i2,i3,1) = 1.0; // density
	  /*********/
	  state0Local(i1,i2,i3,0) = state0Local(i1,i2,i3,1)*det(i1,i2,i3)*mg.gridSpacing(axis1)*mg.gridSpacing(axis2); // mass
	  if( pdeVariation == SmParameters::hemp && 
	      i1 > I1Base && i1 < I1Bound &&
	      i2 > I2Base && i2 < I2Bound )
	  {
	    real area = 0.25*(det(i1,i2,i3)+det(i1+1,i2,i3)+det(i1+1,i2+1,i3)+det(i1,i2+1,i3));
	    state0Local(i1,i2,i3,0) = area*mg.gridSpacing(axis1)*mg.gridSpacing(axis2); // mass
	  }

	}
	else
	{
	U(i1,i2,i3,uc) =u1;
	U(i1,i2,i3,vc) =u2;
	}
	
        if( assignVelocities )
	{
	  U(i1,i2,i3,v1c) = v1;
	  U(i1,i2,i3,v2c) = v2;
	}
	if( assignStress )
	{
	  U(i1,i2,i3,s11c) =s11;
	  U(i1,i2,i3,s12c) =s12;
	  U(i1,i2,i3,s21c) =s12;
	  U(i1,i2,i3,s22c) =s22;
	  if( pdeVariation == SmParameters::hemp )
	  {
            real press = -(lambda+2.0*mu/3.0)*div;
            U(i1,i2,i3,pc)   = press;
            U(i1,i2,i3,s11c) += press;
            U(i1,i2,i3,s22c) += press;
	  }
          
	}
      }
      else
      {
	if( pdeVariation == SmParameters::hemp )
	{
	  ERR(i1,i2,i3,u1c) = U(i1,i2,i3,u1c) - u1;
	  ERR(i1,i2,i3,u2c) = U(i1,i2,i3,u2c) - u2;
	  ERR(i1,i2,i3,uc) = U(i1,i2,i3,uc) - x0;
	  ERR(i1,i2,i3,vc) = U(i1,i2,i3,vc) - y0;
	}
	else
	{
	  ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
	  ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
	  }
	  
        if( assignVelocities )
	{
	  ERR(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
	  ERR(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
	}
	if( assignStress )
	{
	  ERR(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
	  ERR(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
	  ERR(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s12;
	  ERR(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
	  if( pdeVariation == SmParameters::hemp )
	  {
            real press = -(lambda+2.0*mu/3.0)*div;
            ERR(i1,i2,i3,s11c) -= press;
	    ERR(i1,i2,i3,s22c) -= press;
	    ERR(i1,i2,i3,pc)   = U(i1,i2,i3,pc)   - press;
	  }
	  
	}
	
      }
    } // end FOR_3D

  }
  else
  {
    OV_ABORT("Error: finish me");
  }
}

#endMacro


// ==========================================================================
//  Define a Rayleigh surface wave: (see cgDoc/sm/notes.pdf)
//     u1 = SUM_n a_n [ exp(-b1(k_n)*y + ...  ] cos( 2*pi*k_n (x-c*t) )
//     u2 = SUM_n a_n [                       ] sin( 2*pi*k_n (x-c*t) )
//
//  Here we assume that the solid occupies the space y<= ySurf
// where ySurf is given by the user. 
// ==========================================================================
#beginMacro getRayleighWaveSolution(evalSolution,U,ERR,X,t,I1,I2,I3)
{
  const int v1c = parameters.dbase.get<int >("v1c");
  const int v2c = parameters.dbase.get<int >("v2c");
  const int v3c = parameters.dbase.get<int >("v3c");

  bool assignVelocities= v1c>=0 ;
  const int s11c = parameters.dbase.get<int >("s11c");
  const int s12c = parameters.dbase.get<int >("s12c");
  const int s13c = parameters.dbase.get<int >("s13c");
  const int s21c = parameters.dbase.get<int >("s21c");
  const int s22c = parameters.dbase.get<int >("s22c");
  const int s23c = parameters.dbase.get<int >("s23c");
  const int s31c = parameters.dbase.get<int >("s31c");
  const int s32c = parameters.dbase.get<int >("s32c");
  const int s33c = parameters.dbase.get<int >("s33c");

  const int pc = parameters.dbase.get<int >("pc");
    
  bool assignStress = s11c >=0 ;

  real cp = sqrt( (lambda+2.*mu)/rho );
  real cs = sqrt( mu/rho );

  std::vector<real> & data = parameters.dbase.get<std::vector<real> >("RayleighWaveData");
  const int nk = int(data[0]);  // number of modes
  const real cr    = data[1];   // Rayleigh wave speed
  const real ySurf = data[2];   // y value on surface
  const real period= data[3];   // Rayleigh wave speed
  const real xShift= data[4];   // shift in x for wave 
  const int mStart=5;  // Fourier coeff's start at this index in the data 

  if( pdeVariation == SmParameters::hemp )
  {
    printF("\n\n **************** FIX ME: RayelighWave: finish me for HEMP **********\n\n");
    OV_ABORT("error");
  }
  
  real cb1 = sqrt(1.-SQR(cr/cp)); // b1/k : for computing b1 
  real cb2 = sqrt(1.-SQR(cr/cs)); // b2/k : for computing b2 
  
  real c1 = .5*SQR(cr/cs)-1.; // x/2-1 ,   x=cr^2/cs^2

  if( t==0. )
  {
    printF("**** RayleighWave: ySurf=%8.2e, cr=%8.2e, period=%8.2e, t=%8.2e *********\n",ySurf,cr,period,t);
    int m=mStart;
    for( int n=0; n<nk; n++ ) 
    {
      real k = data[m++];  // k=wave-number
      real a=data[m++];    // an : amplitude
      real b=data[m++]; 
      printF(" k%i = %e, a%i=%e, b%i=%e\n",n,k,n,a,n,b);
    }
    
  }
  
  real scale = -1./( cb1+(c1/cb2) ); // make coeff of cos() in u2 = a at y=0
  

  int i1,i2,i3;
  if( mg.numberOfDimensions()==2 )
  {
    real z0=0.;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      real x0 = X(i1,i2,i3,0)-xShift;
      real y0 = X(i1,i2,i3,1)-ySurf;

      real u1=0., u2=0.;  
      real v1=0., v2=0.;
      real s11=0., s12=0., s22=0.;

      int m=mStart;
      // --- loop over different values of k and add contributions ---
      for( int n=0; n<nk; n++ ) 
      {
	real k = twoPi*data[m++]/period;  // 2*pi*k/period , k=wave-number
        // -- note definition of a and b so that they define the Fourier coefficients
        //    of u2 on the surface
        real b=data[m++]*scale;          
        real a=data[m++]*scale;          

        real b1= k*cb1, b2=k*cb2;
	
        real eb1 = exp(b1*(y0)), eb2 = exp(b2*(y0));
	
        real ct = cos(k*(x0-cr*t));
	real st = sin(k*(x0-cr*t));

        u1 +=  ( eb1 + c1*eb2           )*( a*ct+b*st);
	u2 +=  ( cb1*eb1 + (c1/cb2)*eb2 )*( a*st-b*ct);
	if( assignVelocities )
	{
	  v1 += ( eb1 + c1*eb2           )*( k*cr*( a*st-b*ct) );
	  v2 +=-( cb1*eb1 + (c1/cb2)*eb2 )*( k*cr*( a*ct+b*st) );
	}
	if( assignStress )
	{   
          real u1x = ( eb1 + c1*eb2       )*( k*(-a*st+b*ct) );
          real u1y = ( b1*eb1 + b2*c1*eb2 )*(   ( a*ct+b*st) );

          real u2x = ( cb1*eb1       + (c1/cb2)*eb2 )*( k*(a*ct+b*st) );
          real u2y = ( b1*cb1*eb1 + b2*(c1/cb2)*eb2 )*(   (a*st-b*ct) );
          real div=u1x+u2y;
	  
	  s11 += lambda*div+2.*mu*u1x;
	  s12 += mu*( u1y+u2x );
	  s22 += lambda*div+2.*mu*u2y;
	}

      }
      
      // printF(" (i1,i2)=(%i,%i) (x0,y0)=(%8.2e,%8.2e) xi=%8.2e (u1,u2)=(%8.2e,%8.2e)\n",i1,i2,x0,y0,xi,u1,u2);

      if( evalSolution )
      {

	U(i1,i2,i3,uc) =u1;
	U(i1,i2,i3,vc) =u2;
	
        if( assignVelocities )
	{
	  U(i1,i2,i3,v1c) = v1;
	  U(i1,i2,i3,v2c) = v2;
	}
	if( assignStress )
	{
	  U(i1,i2,i3,s11c) =s11;
	  U(i1,i2,i3,s12c) =s12;
	  U(i1,i2,i3,s21c) =s12;
	  U(i1,i2,i3,s22c) =s22;
	}
      }
      else
      {
	if( pdeVariation == SmParameters::hemp )
	{
	  ERR(i1,i2,i3,u1c) = U(i1,i2,i3,u1c) - u1;
	  ERR(i1,i2,i3,u2c) = U(i1,i2,i3,u2c) - u2;
	  ERR(i1,i2,i3,uc) = U(i1,i2,i3,uc) - x0;
	  ERR(i1,i2,i3,vc) = U(i1,i2,i3,vc) - y0;
	}
	else
	{
	  ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
	  ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
	  }
	  
        if( assignVelocities )
	{
	  ERR(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
	  ERR(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
	}
	if( assignStress )
	{
	  ERR(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
	  ERR(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
	  ERR(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s12;
	  ERR(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
	  
	}
	
      }
    } // end FOR_3D

  }
  else
  {
    OV_ABORT("RayleighWave:ERROR: finish me for 3D");
  }
}

#endMacro

// =======================================================================================
//  The function "fg" is basically the integral appearing in the D'Alambert solution
// ======================================================================================
#defineMacro fg(x,z) ( .5*( cg1*(x)*( 1.+ (z)*( 7./p + (z)*( 21./(2.*p-1.) + (z)*( 35./(3.*p-2.) + (z)*( 35./(4.*p-3.) \
			                + (z)*( 21./(5.*p-4.) + (z)*( 7./(6.*p-5.) + z/(7.*p-6.) )))))))  ) )


// ===========================================================
// Evaluate the D'Alambert function "f" and it's derivative
//  Here we assume that u(x,0)=0 and v(x,0)!=0  
// ===========================================================
#beginMacro getF( x, f, fPrime )
{
  real xx = -x/cp;
  real xp1 = pow(xx,p-1);
  real z=cg2*xp1;
  f = cp*fg(xx,z)  - .5*(a/p)*xp1*xx;
  fPrime = .5*( -cg1*pow( 1. + z , 7.) + a*xp1/cp );
}
#endMacro

// ===========================================================
// Evaluate the D'Alambert function "g"
//  Here we assume that u(x,0)=0 and v(x,0)!=0  
// ===========================================================
#beginMacro getG( x, g, gPrime )
{
  if( x<=0. )
  {
    real xx = -x/cp;
    real xp1 = pow(xx,p-1);
    real z=cg2*xp1;
    g = cp*fg(xx,z)  + .5*(a/p)*xp1*xx ;
    gPrime = +.5*( -cg1*pow( 1. + z , 7.) -a*xp1/cp );
  }
  else
  {
     //   g(x) = F(x/cp) - f(-x),
    real xx=x/cp;
    real xp1 = pow(xx,p-1);
    real z=cg2*xp1;
    g = -(a/p)*xp1*xx - cp*fg(xx,z) + .5*(a/p)*xp1*xx;
    gPrime = -a*xp1/cp + .5*( -cg1*pow( 1. + z , 7.) + a*xp1/cp );
  }
}
#endMacro



// ==========================================================================
//  Define the pistonMotion solution (see cgDoc/mp/fluidStructure/fsm.tex)
// ==========================================================================
#beginMacro getPistonMotionSolution(evalSolution,U,ERR,X,t,I1,I2,I3)
{
  const int v1c = parameters.dbase.get<int >("v1c");
  const int v2c = parameters.dbase.get<int >("v2c");
  const int v3c = parameters.dbase.get<int >("v3c");

  bool assignVelocities= v1c>=0 ;
  const int s11c = parameters.dbase.get<int >("s11c");
  const int s12c = parameters.dbase.get<int >("s12c");
  const int s13c = parameters.dbase.get<int >("s13c");
  const int s21c = parameters.dbase.get<int >("s21c");
  const int s22c = parameters.dbase.get<int >("s22c");
  const int s23c = parameters.dbase.get<int >("s23c");
  const int s31c = parameters.dbase.get<int >("s31c");
  const int s32c = parameters.dbase.get<int >("s32c");
  const int s33c = parameters.dbase.get<int >("s33c");

  const int pc = parameters.dbase.get<int >("pc");
    
  bool assignStress = s11c >=0 ;

  const real cp = sqrt( (lambda+2.*mu)/rho );
  const real cs = sqrt( mu/rho );

  std::vector<real> & data = parameters.dbase.get<std::vector<real> >("pistonMotionData");

  int m=0;
  const real a    =data[m++];
  const real p    =data[m++];
  const real rhog =data[m++];
  const real pg   =data[m++];
  const real gamma=data[m++];
  real angle      =data[m++];  // angle (in degrees) for a rotated piston

  const real a0 = sqrt( gamma*pg/rhog);  // speed of sound in the gas

  if( pdeVariation == SmParameters::hemp )
  {
    printF("\n\n **************** FIX ME: getPistonMotionSolution: finish me for HEMP **********\n\n");
    OV_ABORT("error");
  }
  
 


  if( t==0. )
  {
    printP("**** getPistonMotion: a=%8.2e, p=%8.2e, Gas: rho=%8.2e, p=%8.2e gamma=%8.2e angle=%5.2f(degrees)*******\n",
           a,p,rhog,pg,gamma,angle);
  }
  
  angle = angle*Pi/180.;
  const real cosa = cos(angle), sina=sin(angle);

  const real cg1 = pg/(rho*cp*cp);
  const real cg2 = (-a)*(gamma-1.)/(2.*a0);
  // we assume gamma=1.4
  assert( fabs(gamma-1.4) < REAL_EPSILON*100. );

  int i1,i2,i3;
  if( mg.numberOfDimensions()==2 )
  {
    real z0=0.;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      real x0 = X(i1,i2,i3,0);
      real y0 = X(i1,i2,i3,1);

      real u1=0., u2=0.;  
      real v1=0., v2=0.;
      real s11=0., s12=0., s22=0.;

      // Boundary motion:
      //    F(t) = -(a/p)*t^p 
      //    F'(t) = -a*t^{p-1}
      // Solution: 
      //    u1 = f(x-cp*t) + g(x+cp*t)
      // where  
      //   f(x) = - 1/(2*cp)*( int_0^x v0(s) ds ) = -(1/2)* int_0^x G(-s/cp) ds  ,   for   x<0
      //                                          =  (cp/2)* int_0^{-x/cp} G(u) du 
      //   g(x) = + 1/(2*cp)*( int_0^x v0(s) ds ) =  (1/2)* int_0^x G(-s/cp) ds  ,   for   x<0
      //                                          = -(cp/2)* int_0^{-x/cp} G(u) du 
      //   g(x) = F(x/cp) - f(-x),                   for   x>0 
      //
      //   v0(s) = cp*G(-t/cp)   : velocity at t=0 for s<0
      //   G(t) = (pg/(rho*cp^2)) * [ 1 + (gamma-1)/(2*a0)* F'(t) ]^7 + F'(t)/cp
      //        = cg1* [ 1 + cg2*t^{p-1} ]^7 + F'(t)/cp
      //     cg1=p0/(rho*cp^2), cg2=(-a)*(gamma-1)/(2*a0)
      // 
      // where we have assumed that gamma=1.4=7/5 so that 2*gamma/(gamma-1)= 7 
      // 
      //  Int_0^t G(s) ds = cg1*[ t + (7/p)*cg2*t^{p-1}*t + (21/(2p-1))*(cg2*t^{p-1})^2*t + ) + F(t)/cp
      //                  = cg1*t*[ 1 + Z*(7)/(p) + Z^2*(21)/(2p-1) + Z^3*(35)/(3p-2) + Z^4*(35)/(4p-3) + ...
      //                              + Z^5*(21)/(5p-4) + Z^6*(7)/(6p-5) + Z^7*(1)/(7p-6) ] 
      //   Z=cg2*t^{p-1}

      // xa = distance of point (x0,y0) from the plane  (cosa,sina).(x,y)=0 
      real xa = x0*cosa + y0*sina;

      real xp = xa + cp*t;
      real xm = xa - cp*t;
      

      real fm, fmPrime, gp, gpPrime;

      getF( xm, fm, fmPrime );
      getG( xp, gp, gpPrime );

      real ua = fm + gp;
      real va = cp*( - fmPrime + gpPrime );
      real uap=fmPrime + gpPrime;

      // Piston at an angle:
      //   n = [ cos(angle) , sin(angle) ] = [ c, s ]    -- normal to the face
      //   u1 = c*ua,  u2=s*ua
      //   u1.x = c*ua.x,  u1.y = c*ua.y,   u2.x = s*ua.x, u2.y = s*ua.y
      //   
      // (1)  ua.n = c*ua.x + s*ua.y = uap
      // (2)  ua.t =-s*ua.x + c*ua.y = 0    "tangential derivative of motion"
      //
      //  (1) and (2) -->   ua.x = c*uap, ua.y=s*uap
      // Thus: u1.x = c*ua.x = c*c*uap,  u1.y = c*ua.y = c*s*uap

      u1 = ua*cosa;
      u2 = ua*sina;
      v1 = va*cosa;
      v2 = va*sina;
      
      real u1x = uap*cosa*cosa;
      real u1y = uap*cosa*sina;

      real u2x = uap*sina*cosa;
      real u2y = uap*sina*sina;
      

      s11 = (lambda+2.*mu)*u1x + lambda*u2y;   
      s12 = mu*( u1y+u2x );
      s22 = lambda*u1x + (lambda+2.*mu)*u2y;

      // printF("piston (i1,i2)=(%i,%i) (u1,u2)=(%8.2e,%8.2e)\n",i1,i2,u1,u2);

      if( evalSolution )
      {
	U(i1,i2,i3,uc) =u1;
	U(i1,i2,i3,vc) =u2;
	
        if( assignVelocities )
	{
	  U(i1,i2,i3,v1c) = v1;
	  U(i1,i2,i3,v2c) = v2;
	}
	if( assignStress )
	{
	  U(i1,i2,i3,s11c) =s11;
	  U(i1,i2,i3,s12c) =s12;
	  U(i1,i2,i3,s21c) =s12;
	  U(i1,i2,i3,s22c) =s22;
	}
      }
      else
      {
	ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
	ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
	  
        if( assignVelocities )
	{
	  ERR(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
	  ERR(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
	}
	if( assignStress )
	{
	  ERR(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
	  ERR(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
	  ERR(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s12;
	  ERR(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
	  
	}
	
      }
    } // end FOR_3D

  }
  else
  {
    OV_ABORT("getPistonMotion:ERROR: finish me for 3D");
  }
}

#endMacro








//   (Ex).t = (1/eps)*[  (Hz).y ]
//   (Ey).t = (1/eps)*[ -(Hz).x ]
//   (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]

#define exTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(-ky/(eps*cc))
#define eyTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*( kx/(eps*cc))
#define hzTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))

#define exLaplacianTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(+ky*(twoPi*twoPi*(kx*kx+ky*ky))/(eps*cc))
#define eyLaplacianTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(-kx*(twoPi*twoPi*(kx*kx+ky*ky))/(eps*cc))
#define hzLaplacianTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*( -(twoPi*twoPi*(kx*kx+ky*ky) ) )

// define eyTrue(x,y,t) exp( -beta*SQR((x-x0)-c*(t)) )

// Here is a plane wave with the shape of a Gaussian
// xi = kx*(x)+ky*(y)-cc*(t)
// cc=  c*sqrt( kx*kx+ky*ky );
#define hzGaussianPulse(xi)  exp(-betaGaussianPlaneWave*((xi)*(xi)))
#define exGaussianPulse(xi)  hzGaussianPulse(xi)*(-ky/(eps*cc))
#define eyGaussianPulse(xi)  hzGaussianPulse(xi)*( kx/(eps*cc))

#define hzLaplacianGaussianPulse(xi)  ((4.*betaGaussianPlaneWave*betaGaussianPlaneWave*(kx*kx+ky*ky))*xi*xi-\
                                        (2.*betaGaussianPlaneWave*(kx*kx+ky*ky)))*exp(-betaGaussianPlaneWave*((xi)*(xi)))
#define exLaplacianGaussianPulse(xi)  hzLaplacianGaussianPulse(xi,t)*(-ky/(eps*cc))
#define eyLaplacianGaussianPulse(xi)  hzLaplacianGaussianPulse(xi,t)*( kx/(eps*cc))

// 3D
// E:
//   u.tt = (1/eps)*[ ((1/mu)*u.x).x + ((1/mu)*u.y).y + ((1/mu)*u.z).z ]
//   div(u)=0
// H
//   v.tt = (1/mu)*[ ((1/eps)*v.x).x + ((1/eps)*v.y).y + ((1/eps)*v.z).z ]
// Define macros for forcing functions


//
//   (Ex).t = (1/eps)*[ (Hz).y - (Hy).z ]
//   (Ey).t = (1/eps)*[ (Hx).z - (Hz).x ]
//   (Ez).t = (1/eps)*[ (Hy).x - (Hx).y ]
//   (Hx).t = (1/mu) *[ (Ey).z - (Ez).y ]
//   (Hy).t = (1/mu) *[ (Ez).x - (Ex).z ]
//   (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]

// ****************** finish this -> should `rotate' the 2d solution ****************

#define exTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-ky/(eps*cc))
#define eyTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*( kx/(eps*cc))
#define ezTrue3d(x,y,z,t) 0

#define hxTrue3d(x,y,z,t) 0
#define hyTrue3d(x,y,z,t) 0
#define hzTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))

#define exLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(+ky*(twoPi*twoPi*(kx*kx+ky*ky))/(eps*cc))
#define eyLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(-kx*(twoPi*twoPi*(kx*kx+ky*ky))/(eps*cc))
#define ezLaplacianTrue3d(x,y,z,t) 0

#define hxLaplacianTrue3d(x,y,z,t) 0
#define hyLaplacianTrue3d(x,y,z,t) 0
#define hzLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*( -(twoPi*twoPi*(kx*kx+ky*ky) ) )


// ------------ macros for the plane material interface -------------------------
#defineMacro PMIex(x,y,t) (aa1*cos(twoPi*(kx*(x)+ky*(y)-w*(t))) + r*bb1*cos(twoPi*(-kx*(x)+ky*(y)-w*(t))))
#defineMacro PMIey(x,y,t) (aa2*cos(twoPi*(kx*(x)+ky*(y)-w*(t))) + r*bb2*cos(twoPi*(-kx*(x)+ky*(y)-w*(t))))
#defineMacro PMIhz(x,y,t) ((-twoPi/w)*( (a1*ky-a2*kx)*sin(twoPi*(kx*(x)+ky*(y)-w*(t))) \
                                    + (b1*ky+b2*kx)*r*sin(twoPi*(-kx*(x)+ky*(y)-w*(t)))) )

#defineMacro PMITex(x,y,t) (tau*dd1*cos(twoPi*(kappax*(x)+kappay*(y)-w*(t))))
#defineMacro PMITey(x,y,t) (tau*dd2*cos(twoPi*(kappax*(x)+kappay*(y)-w*(t))))
#defineMacro PMIThz(x,y,t) ((-twoPi*tau/w)*(d1*kappay-d2*kappax)*sin(twoPi*(kappax*(x)+kappay*(y)-w*(t))))

// OPTION: initialCondition, error, boundaryCondition
#beginMacro setPlaneMaterialInterfaceMacro(OPTION,J1,J2,J3)
if( numberOfDimensions==2 )
{
  int i1,i2,i3;

  // we assume there are just two grids with an interface in-between
  const real c1=cGrid(0);
  const real c2=cGrid(1);

  const real kNorm = sqrt(kx*kx+ky*ky);
  const real a1=-ky/kNorm, a2= kx/kNorm;
  const real b1=-ky/kNorm, b2=-kx/kNorm;
  const real w = c1*kNorm;

  real kappax,kappay,kappaNorm;
	
  const real cr=c2/c1;
  kappay=ky;
  // discrimant is assumed positive (otherwise internal reflection)
  kappax=sqrt( (kx*kx+ky*ky)/(cr*cr) - kappay*kappay );
	
  kappaNorm=sqrt(kappax*kappax+kappay*kappay);
  const real d1=-kappay/kappaNorm;
  const real d2= kappax/kappaNorm;
	
  const real cosTheta1=kx/kNorm;
  const real cosTheta2=kappax/kappaNorm;

   // reflection and transmission coefficients
  const real r = (c1*cosTheta1-c2*cosTheta2)/(c1*cosTheta1+c2*cosTheta2);
  const real tau = (2.*c2*cosTheta1)/(c1*cosTheta1+c2*cosTheta2);

  real tm=t-dt,x,y;
  const real x0=x0PlaneMaterialInterface[0], y0=x0PlaneMaterialInterface[1];
  // The solution is rotated according to the normal of the plane of the material interface
  real an0=normalPlaneMaterialInterface[0], an1=normalPlaneMaterialInterface[1];
  real aNorm = sqrt(an0*an0+an1*an1);
  an0/= aNorm; an1/=aNorm;
  real aa1=an0*a1-an1*a2, aa2= an1*a1+an0*a2;
  real bb1=an0*b1-an1*b2, bb2= an1*b1+an0*b2;
  real dd1=an0*d1-an1*d2, dd2= an1*d1+an0*d2;

  if( grid==0 )
  { // incident plus reflected wave.
    FOR_3D(i1,i2,i3,J1,J2,J3)
    {
      real xx1 = XEP(i1,i2,i3,0) -x0;
      real yy1 = XEP(i1,i2,i3,1) -y0;
      // rotate the coordinate system
      x = an0*xx1+an1*yy1;
      y =-an1*xx1+an0*yy1;

      real u1 = PMIex(x,y,t);
      real u2 = PMIey(x,y,t);
      real u3 = PMIhz(x,y,t);

      #If #OPTION eq "initialCondition"
       UEX(i1,i2,i3)= u1;
       UEY(i1,i2,i3)= u2;
       UHZ(i1,i2,i3)= u3;
 
       UMEX(i1,i2,i3)= PMIex(x,y,tm);
       UMEY(i1,i2,i3)= PMIey(x,y,tm);
       UMHZ(i1,i2,i3)= PMIhz(x,y,tm);
      #Elif #OPTION eq "error"
       ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
       ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
       ERRHZ(i1,i2,i3)=UHZ(i1,i2,i3)-u3;
      #Elif #OPTION eq "boundaryCondition"
       U(i1,i2,i3,ex)= u1;
       U(i1,i2,i3,ey)= u2;
       U(i1,i2,i3,hz)= u3;
      #End
    }
  }
  else
  {
    // transmitted wave
    FOR_3D(i1,i2,i3,J1,J2,J3)
    {
      real xx1 = XEP(i1,i2,i3,0) -x0;
      real yy1 = XEP(i1,i2,i3,1) -y0;
      // rotate the coordinate system
      x = an0*xx1+an1*yy1;
      y =-an1*xx1+an0*yy1;

      real u1 = PMITex(x,y,t);
      real u2 = PMITey(x,y,t);
      real u3 = PMIThz(x,y,t);

      #If #OPTION eq "initialCondition"
       UEX(i1,i2,i3)= u1;
       UEY(i1,i2,i3)= u2;
       UHZ(i1,i2,i3)= u3;
 
       UMEX(i1,i2,i3)= PMITex(x,y,tm);
       UMEY(i1,i2,i3)= PMITey(x,y,tm);
       UMHZ(i1,i2,i3)= PMIThz(x,y,tm);
      #Elif #OPTION eq "error"
       ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
       ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
       ERRHZ(i1,i2,i3)=UHZ(i1,i2,i3)-u3;
      #Elif #OPTION eq "boundaryCondition"
       U(i1,i2,i3,ex)= u1;
       U(i1,i2,i3,ey)= u2;
       U(i1,i2,i3,hz)= u3;
      #End
    }
  }
 }
#endMacro

//==================================================================================================
// Evaluate Tom Hagstom's exact solution defined as an integral of Guassian sources
// 
// OPTION: OPTION=solution or OPTION=error OPTION=bounary to compute the solution or the error or
//     the boundary condition
//
//==================================================================================================
#beginMacro getGaussianIntegralSolution(OPTION,VEX,VEY,VHZ,t)

if( initialConditionOption==gaussianIntegralInitialCondition )
{
  
  double wt,wx,wy;
  const int nsources=1;
  double xs[nsources], ys[nsources], tau[nsources], var[nsources], amp[nsources];
  xs[0]=0.;
  ys[0]=1.e-8*1./3.;  // should not be on a grid point
  tau[0]=-.95;
  var[0]=30.;
  amp[0]=1.;
   
  double period= 1.;  // period in y
  double time=t;
   
  int i1,i2,i3;

  FOR_3D(i1,i2,i3,J1,J2,J3)
  {
    double x=X(i1,i2,i3,0); 
    double y=X(i1,i2,i3,1);

    exmax(wt,wx,wy,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,time);

    #If #OPTION eq "solution"
      VEX(i1,i2,i3) = wy;
      VEY(i1,i2,i3) =-wx;
      VHZ(i1,i2,i3)= wt;
    #Elif #OPTION eq "error" 
      ERREX(i1,i2,i3) = VEX(i1,i2,i3) - wy;
      ERREY(i1,i2,i3) = VEY(i1,i2,i3) + wx;
      ERRHZ(i1,i2,i3) = VHZ(i1,i2,i3) - wt;

    #Else
      U(i1,i2,i3,ex) = wy;
      U(i1,i2,i3,ey) =-wx;
      U(i1,i2,i3,hz) = wt;
    #End
	
  }
}

#endMacro
