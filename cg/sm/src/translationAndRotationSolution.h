// ==================================================================================
// Macro to evaluate the translation and rotation solution
// 
// evalSolution : true=eval solution, false=eval error
// ==================================================================================
#beginMacro getTranslationAndRotationSolution(evalSolution,U,ERR,X,t,I1,I2,I3)
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

  if( pdeVariation == SmParameters::hemp )
  {
    printF("\n\n **************** FIX ME: getTranslationAndRotationSolution: finish me for HEMP **********\n\n");
    // OV_ABORT("error");
  }

  // Here is the solution for large translation and rotation

  // hard code some numbers for now: 
  std::vector<real> & trd = parameters.dbase.get<std::vector<real> >("translationAndRotationSolutionData");

  real omega   = trd[0];   // rotation rate
  real xcenter = trd[1];      // center of rotation in the reference frame
  real ycenter = trd[2];      // center of rotation in the reference frame
  real zcenter = trd[3];      // center of rotation in the reference frame
  real vcenter[3]={trd[4],trd[5],trd[6]};   // velocity of center
  real rx[3]={cos(omega*t)-1.,-sin(omega*t),    0.};
  real ry[3]={sin(omega*t)   , cos(omega*t)-1., 0.};
  real rxt[3]={-omega*sin(omega*t),-omega*cos(omega*t), 0.};
  real ryt[3]={ omega*cos(omega*t),-omega*sin(omega*t), 0.};

#define U0(x,y,z,n,t)  (vcenter[n-uc]*(t) +  rx[n-uc]*((x)-xcenter) +  ry[n-uc]*((y)-ycenter))
#define U0T(x,y,z,n,t) (vcenter[n-uc]     + rxt[n-uc]*((x)-xcenter) + ryt[n-uc]*((y)-ycenter))
#define U0X(x,y,z,n,t) (                     rx[n-uc]                                        )
#define U0Y(x,y,z,n,t) (                                               ry[n-uc]              )

  if( t==0. )
    printF("**** translationAndRotationSolution, t=%8.2e omega=%8.2e (x0,x1,x2)=(%8.2e,%8.2e,%8.2e) "
	   " (v0,v1,v2)=(%8.2e,%8.2e,%8.2e) *********\n",t,omega,xcenter,ycenter,zcenter,
	   vcenter[0],vcenter[1],vcenter[2]);


  int i1,i2,i3;
  if( mg.numberOfDimensions()==2 )
  {
    real z0=0.;
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      real x0 = X(i1,i2,i3,0);
      real y0 = X(i1,i2,i3,1);
      real u1 = U0(x0,y0,z0,uc,t);
      real u2 = U0(x0,y0,z0,vc,t);

      if( evalSolution )
      {
	U(i1,i2,i3,uc) =u1;
	U(i1,i2,i3,vc) =u2;
      }
      else
      {
	ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
	ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
      }

    }

    if( assignVelocities )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
      {
	real x0 = X(i1,i2,i3,0);
	real y0 = X(i1,i2,i3,1);
	real v1 = U0T(x0,y0,z0,uc,t);
	real v2 = U0T(x0,y0,z0,vc,t);

	// printF(" *** assignSpecial: v1=%e v2=%e\n",v1,v2);

	if( evalSolution )
	{
	  U(i1,i2,i3,v1c) = v1;
	  U(i1,i2,i3,v2c) = v2;
	}
	else
	{
	  ERR(i1,i2,i3,v1c) = U(i1,i2,i3,v1c) - v1;
	  ERR(i1,i2,i3,v2c) = U(i1,i2,i3,v2c) - v2;
	}

      }
    }
    if( assignStress )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
      {
	real x0 = X(i1,i2,i3,0);
	real y0 = X(i1,i2,i3,1);
	real f11 = 1. + U0X(x0,y0,z0,uc,t);
	real f12 =      U0Y(x0,y0,z0,uc,t);
	real f21 =      U0X(x0,y0,z0,vc,t);
	real f22 = 1. + U0Y(x0,y0,z0,vc,t);
	real e11 = .5*(f11*f11+f21*f21-1.);     // this is E(i,j), symmetric
	real e12 = .5*(f11*f12+f21*f22   );
	real e22 = .5*(f12*f12+f22*f22-1.);
	real trace = e11 + e22;
	real s11 = lambda*trace + 2*mu*e11;     // this is S(i,j), symmetric
	real s12 =                2*mu*e12;
	real s21 = s12;
	real s22 = lambda*trace + 2*mu*e22;
	real p11 = s11*f11 + s12*f12;           // this P(i,j)
	real p12 = s11*f21 + s12*f22;
	real p21 = s21*f11 + s22*f12;
	real p22 = s21*f21 + s22*f22;
	if( evalSolution )
	{
	  U(i1,i2,i3,s11c) = p11;
	  U(i1,i2,i3,s12c) = p12;
	  U(i1,i2,i3,s21c) = p21;
	  U(i1,i2,i3,s22c) = p22;
	}
	else
	{
	  ERR(i1,i2,i3,s11c) = U(i1,i2,i3,s11c) - p11;
	  ERR(i1,i2,i3,s12c) = U(i1,i2,i3,s12c) - p12;
	  ERR(i1,i2,i3,s21c) = U(i1,i2,i3,s21c) - p21;
	  ERR(i1,i2,i3,s22c) = U(i1,i2,i3,s22c) - p22;
	}
      }
    }
  }
  else
  { // ***** 3D  ****
    OV_ABORT("translationAndRotationSolution: finish me for 3d");
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      real x0 = X(i1,i2,i3,0);
      real y0 = X(i1,i2,i3,1);
      real z0 = X(i1,i2,i3,2);
      U(i1,i2,i3,uc) =U0(x0,y0,z0,uc,t);
      U(i1,i2,i3,vc) =U0(x0,y0,z0,vc,t);
      U(i1,i2,i3,wc) =U0(x0,y0,z0,wc,t);
    }
  }

#undef U0
#undef U0T
#undef U0X
#undef U0Y

  
}
#endMacro
