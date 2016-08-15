#include "Maxwell.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"


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


//==============================================================================================
/// \brief Evaluate the user defined forcing.
/// \details This function is called to actually evaluate the user defined forcing
///   The function setupUserDefinedForcing is first 
///   called to assign the option and parameters. Rewrite or add new options to 
///   this function and to setupUserDefinedForcing to supply your own forcing option.
///
/// \param f (input/output) : add to this forcing function
/// \param iparam[] (input) : holds some integer parameters
/// \param rparam[] (input) : holds some real parameters
/// 
// NOTES:
//   (1) The forcing function is added to the right-hand side of Maxwell's equations
//        in second order form:
//               E_tt = c^2 ( E_xx + E_yy + E_zz ) + F(x,y,z,t) 
//==============================================================================================

int Maxwell:: 
userDefinedForcing( realArray & f, int iparam[], real rparam[] )
{
  // Look for the userDefinedForcing sub-directory in the data-base
  if( !dbase.has_key("userDefinedForcingData") )
  {
    // if the directory is not there then assume that there is no user defined forcing
    return 0;
  }
  DataBase & db = dbase.get<DataBase>("userDefinedForcingData");

  aString & option= db.get<aString>("option");

  if( option=="none" ) // No user defined forcing has been specified
   return 0;

  const real t =rparam[0];       // Add the forcing at this time.
  const real dt =rparam[1];      // Current time step.
  const int & grid = iparam[0];  // Here is the grid we are on 
  const int & current = iparam[1];
  
  if( t<= 2.*dt  )
  {
    printF("userDefinedForcing: t=%9.3e option=[%s]\n",t,(const char*)option);
  }

  // Here is the CompositeGrid: 
  assert( cgp!=NULL );
  CompositeGrid & cg = *cgp;
  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  const int numberOfDimensions = cg.numberOfDimensions();

  assert( grid>=0 && grid<numberOfComponentGrids );
  MappedGrid & mg = cg[grid];

  // Here is the current solution: 
  realCompositeGridFunction & ucg = cgfields[current];
  realMappedGridFunction & u = ucg[grid];
  
  // Access the local arrays on this processor:
  OV_GET_SERIAL_ARRAY(real,u,uLocal);
  OV_GET_SERIAL_ARRAY(real,f,fLocal);

  // -- we optimize for Cartesian grids (we can avoid creating the vertex array)
  const bool isRectangular=mg.isRectangular();
  if( !isRectangular )
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
  OV_GET_SERIAL_ARRAY(real,mg.center(),xLocal);

  real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
  int iv0[3]={0,0,0}; //
  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];  // NOTE: iv[0]==i1, iv[1]==i2, iv[2]==i3
  real xv[3]={0.,0.,0.};
  if( isRectangular )
  {
    mg.getRectangularGridParameters( dvx, xab );
    for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
    {
      iv0[dir]=mg.gridIndexRange(0,dir);
      if( mg.isAllCellCentered() )
	xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
    }
  }
  // This macro defines the grid points for rectangular grids:
#undef XC
#define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

  
  Index I1,I2,I3;
  getIndex( mg.dimension(),I1,I2,I3 );          // all points including ghost points.
  // getIndex( mg.gridIndexRange(),I1,I2,I3 );  // boundary plus interior points.
  // restrict bounds to local processor, include parallel ghost pts:
  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,1);   
  if( !ok ) return 0;  // no points on this processor (NOTE: no communication should be done after this point)

  // fLocal=0.;

  if( option=="gaussianSources" )
  {
  
    int & numberOfGaussianSources = db.get<int>("numberOfGaussianSources");
    RealArray & gaussianParameters = db.get<RealArray>("gaussianParameters");

    // Add the Gaussian source terms to fLocal
    for( int m=0; m<numberOfGaussianSources; m++ )
    {
      real a    = gaussianParameters(0,m);
      real beta = gaussianParameters(1,m); 
      real omega= gaussianParameters(2,m); 
      real p    = gaussianParameters(3,m);
      real x0   = gaussianParameters(4,m); 
      real y0   = gaussianParameters(5,m); 
      real z0   = gaussianParameters(6,m);
      real t0   = gaussianParameters(7,m);

      if( false && t0 < 5.*dt )
	printF("Gaussian source %i: setting a=%8.2e, beta=%8.2e, omega=%8.2e, p=%8.2e, x0=%8.2e, y0=%8.2e, "
               "z0=%8.2e, t0=%8.2e\n", m,a,beta,omega,p,x0,y0,z0,t0);

      const real cost=cos(2.*Pi*omega*(t-t0));
      const real sint=sin(2.*Pi*omega*(t-t0));

      if( mg.numberOfDimensions()==2 )
      {
      
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
          real x= xLocal(i1,i2,i3,0), y=xLocal(i1,i2,i3,1);
          real rSq = SQR(x-x0)+SQR(y-y0);
	  // real g = a*cost*exp( -beta*pow( rSq, p ) );
	  real aExp = a*exp( -beta*pow( rSq, p ) );
	  real g = aExp*cost;
          real rPow = p==1. ? 1 :  pow(rSq,p-1.);

          // *wdh* 2015/04/19 -- fixed to be divergence free for p!=1
          //  Fx = - const * g_y
          //  Fy =   const * g_x
          // => (Fx)_x + (Fy)_y = 0 
	  fLocal(i1,i2,i3,ex)+= -(y-y0)*rPow*g;
	  fLocal(i1,i2,i3,ey)+=  (x-x0)*rPow*g;
	  fLocal(i1,i2,i3,hz)+=              g;

          // ---- this next section is currently not needed -- maybe in future ---
	  // if( method==sosup )
	  // {
          //   // supply time derivative of the forcing 
          //   real gt = -(2.*Pi*omega)*aExp*sint;
	  //   fLocal(i1,i2,i3,ext)+= -(y-y0)*rPow*gt;
	  //   fLocal(i1,i2,i3,eyt)+=  (x-x0)*rPow*gt;
	  //   fLocal(i1,i2,i3,hzt)+=              gt;
	  // }
	  
	}
      }
      else
      {
	// -- 3D ---
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
          real x= xLocal(i1,i2,i3,0), y=xLocal(i1,i2,i3,1), z=xLocal(i1,i2,i3,2);
          real rSq =  SQR(x-x0)+SQR(y-y0)+SQR(z-z0);
	  // real g = a*cost*exp( -beta*pow( rSq, p ) );
	  real aExp = a*exp( -beta*pow( rSq, p ) );
	  real g = aExp*cost;
	  real rPow = pow(rSq,p-1.);
	  
          // *wdh* 2015/04/19 --FIX ME to be DIV FREE
          //   Fx = -const* g_y + const* g_z 
          //   Fy =  const* g_x - const* g_z
          //   Fz =  const* g_y - const* g_x 
          // => (Fx)_x + (Fy)_y + (Fz)_z = 0 
	  fLocal(i1,i2,i3,ex)+= ((z-z0)-(y-y0))*rPow*g;
	  fLocal(i1,i2,i3,ey)+= ((x-x0)-(z-z0))*rPow*g;
	  fLocal(i1,i2,i3,ez)+= ((y-y0)-(x-x0))*rPow*g;

          // ---- this next section is currently not needed -- maybe in future ---
	  // if( method==sosup )
	  // {
          //   // supply time derivative of the forcing 
          //   real gt = -(2.*Pi*omega)*aExp*sint;
	  //   fLocal(i1,i2,i3,ex)+= ((z-z0)-(y-y0))*rPow*gt;
	  //   fLocal(i1,i2,i3,ey)+= ((x-x0)-(z-z0))*rPow*gt;
	  //   fLocal(i1,i2,i3,ez)+= ((y-y0)-(x-x0))*rPow*gt;
	  // }

	}
      }
    }
  }
  else if( option=="my source" )
  {
    int & numberOfGaussianSources = db.get<int>("numberOfGaussianSources");
    RealArray & gaussianParameters = db.get<RealArray>("gaussianParameters");

    // Add the Gaussian source terms to fLocal
    for( int m=0; m<numberOfGaussianSources; m++ )
    {
      real a    = gaussianParameters(0,m);
      real beta = gaussianParameters(1,m); 
      real omega= gaussianParameters(2,m); 
      real p    = gaussianParameters(3,m);
      real x0   = gaussianParameters(4,m); 
      real y0   = gaussianParameters(5,m); 
      real z0   = gaussianParameters(6,m);
      real t0   = gaussianParameters(7,m);

      if( false )
	printF("Gaussian source %i: setting a=%8.2e, beta=%8.2e, omega=%8.2e, p=%8.2e, x0=%8.2e, y0=%8.2e, "
               "z0=%8.2e, t0=%8.2e\n", m,a,beta,omega,p,x0,y0,z0,t0);

      const real cost=cos(2.*Pi*omega*(t-t0));
      const real sint=sin(2.*Pi*omega*(t-t0));

      if( mg.numberOfDimensions()==2 )
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
          real x= xLocal(i1,i2,i3,0), y=xLocal(i1,i2,i3,1);
	  // real g = a*cost*exp( -beta*pow( SQR(x-x0)+SQR(y-y0), p ) );
	  real aExp = a*exp( -beta*pow( SQR(x-x0)+SQR(y-y0), p ) );
	  real g = aExp*cost;

	  fLocal(i1,i2,i3,ex)+= -(y-y0)*g;
	  fLocal(i1,i2,i3,ey)+=  (x-x0)*g;
	  fLocal(i1,i2,i3,hz)+=         g;
	  // fLocal(i1,i2,i3,ey)=0.; // *****************

          // ---- this next section is currently not needed -- maybe in future ---
	  // if( method==sosup )
	  // {
          //   // supply time derivative of the forcing 
          //   real gt = -(2.*Pi*omega)*aExp*sint;
	  //   fLocal(i1,i2,i3,ex)+= -(y-y0)*gt;
	  //   fLocal(i1,i2,i3,ey)+=  (x-x0)*gt;
	  //   fLocal(i1,i2,i3,hz)+=         gt;
	  // }	  
	}
      }
      else
      {
	// -- 3D ---
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
          real x= xLocal(i1,i2,i3,0), y=xLocal(i1,i2,i3,1), z=xLocal(i1,i2,i3,2);
	  // real g = a*cost*exp( -beta*pow( SQR(x-x0)+SQR(y-y0)+SQR(z-z0), p ) );
	  real aExp = a*exp( -beta*pow( SQR(x-x0)+SQR(y-y0)+SQR(z-z0), p ) );
	  real g = aExp*cost;

	  fLocal(i1,i2,i3,ex)+= ((z-z0)-(y-y0))*g;
	  fLocal(i1,i2,i3,ey)+= ((x-x0)-(z-z0))*g;
	  fLocal(i1,i2,i3,ez) =0.;  // *****************
	  // fLocal(i1,i2,i3,ez)+= ((y-y0)-(x-x0))*g;

          // ---- this next section is currently not needed -- maybe in future ---
	  // if( method==sosup )
	  // {
          //   // supply time derivative of the forcing 
          //   real gt = -(2.*Pi*omega)*aExp*sint;
	  //   fLocal(i1,i2,i3,ex)+= ((z-z0)-(y-y0))*gt;
	  //   fLocal(i1,i2,i3,ey)+= ((x-x0)-(z-z0))*gt;
	  //   fLocal(i1,i2,i3,ez) =0.;  // *****************
	  // }	  


	}
      }
    }
  }
  else if( option=="manufacturedPulse" )
  {
    // Manufactured pulse:
    //   A pulse like solution that requires a forcing function to make it a solution
    //   Used to test the forcing terms in the equations.

    if( method!=nfdtd && method!=sosup )
    {
      printF("userDefinedForcing:ERROR: method!=nfdtd -- not implemented for option=[%s] yet.\n",(const char*)option);
      OV_ABORT("ERROR");
    }

    if( true && t<5.*dt  )
      printF("--MX-- getUserDefinedForcing: -- eval RHS for manufacturedPulse at t=%9.3e\n",t);

    // --- NOTE: We use the same parameters as for the userDefinedKnownSolution -----
    if( ! dbase.has_key("userDefinedKnownSolutionData") )
    {
      printF("--MX-- getUserDefinedForcing:ERROR: sub-directory `userDefinedKnownSolutionData' not found!\n");
      OV_ABORT("error");
    }
    DataBase & db =  dbase.get<DataBase>("userDefinedKnownSolutionData");

    const aString & userKnownSolution = db.get<aString>("userKnownSolution");
    assert( userKnownSolution=="manufacturedPulse" );

    real *rpar = db.get<real[20]>("rpar");
    int *ipar = db.get<int[20]>("ipar");

    const real amp = rpar[0];
    const real beta= rpar[1];
    const real x0  = rpar[2];
    const real y0  = rpar[3];
    const real z0  = rpar[4];
    const real cx  = rpar[5];
    const real cy  = rpar[6];
    const real cz  = rpar[7];

    const real cSq=c*c;

    real x,y,z;
    if( numberOfDimensions==2 )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	if( !isRectangular )
	{
	  x= xLocal(i1,i2,i3,0);
	  y= xLocal(i1,i2,i3,1);
	}
	else
	{
	  x=XC(iv,0);
	  y=XC(iv,1);
	}

	// if( method==nfdtd )
	// {
        // The forcing function is determined in mx/codes/manufacturedPulse.maple 
        #include "../codes/manufacturedPulseForcing2d.h"
	fLocal(i1,i2,i3,ex) += FEX;
	fLocal(i1,i2,i3,ey) += FEY;
	fLocal(i1,i2,i3,hz) += FHZ;

        // ---- this next section is currently not needed -- maybe in future ---
	// }
        // else if( method=sosup )
	// {
	//   // The forcing function is determined in mx/codes/manufacturedPulse.maple 
        //   #include "../codes/manufacturedPulseForcingSosup2d.h"
	//   fLocal(i1,i2,i3,ex ) += FEX;
	//   fLocal(i1,i2,i3,ey ) += FEY;
	//   fLocal(i1,i2,i3,hz ) += FHZ;
	//   fLocal(i1,i2,i3,ext) += FEXT;
	//   fLocal(i1,i2,i3,eyt) += FEYT;
	//   fLocal(i1,i2,i3,hzt) += FHZT;
	// }
      }
    }
    else
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	if( !isRectangular )
	{
	  x= xLocal(i1,i2,i3,0);
	  y= xLocal(i1,i2,i3,1);
	  z= xLocal(i1,i2,i3,2);
	}
	else
	{
	  x=XC(iv,0);
	  y=XC(iv,1);
	  z=XC(iv,2);
	}
	//if( method==nfdtd )
	// {
          // The forcing function is determined in mx/codes/manufacturedPulse.maple 
          #include "../codes/manufacturedPulseForcing3d.h"
	  fLocal(i1,i2,i3,ex) += FEX;
  	  fLocal(i1,i2,i3,ey) += FEY;
	  fLocal(i1,i2,i3,ez) += FEZ;

        // ---- this next section is currently not needed -- maybe in future --- 
	// }
	// else if( method=sosup )
	// {
	//   // The forcing function is determined in mx/codes/manufacturedPulse.maple 
        //   #include "../codes/manufacturedPulseForcingSosup3d.h"
	//   fLocal(i1,i2,i3,ex ) += FEX;
  	//   fLocal(i1,i2,i3,ey ) += FEY;
	//   fLocal(i1,i2,i3,hz ) += FEZ;
	//   fLocal(i1,i2,i3,ext) += FEXT;
  	//   fLocal(i1,i2,i3,eyt) += FEYT;
	//   fLocal(i1,i2,i3,hzt) += FEZT;
	// }
      }
    }
    
  }
  else
  {
    printF("Maxwell::userDefinedForcing:ERROR: unknown option =[%s]\n",(const char*)option);
    OV_ABORT("error");
  }

  return 0;
}




//==============================================================================================
/// \brief This function is used to choose a user defined forcing and input parameters etc.
/// \details This function is used to setup and define the forcing to use.
/// The function userDefinedForcing (above) is called to actually assign the forcing.
///  Rewrite or add new options to  this routine to supply your own forcing.
/// Choose the "user defined forcing" option to have this routine called.
///
//==============================================================================================
int Maxwell::
setupUserDefinedForcing()
{
  GenericGraphicsInterface & gi = *gip;

  // here is a menu of possible forcing options
  aString menu[]=  
  {
    "no forcing",
    "gaussian sources",
    "my source",
    "manufactured pulse",
    "exit",
    ""
  };
  aString answer,answer2;
  char buff[100];
  gi.appendToTheDefaultPrompt(">user defined forcing");

  // Make a sub-directory in the data-base to store variables used in userDefinedForcing
  if( !dbase.has_key("userDefinedForcingData") )
    dbase.put<DataBase>("userDefinedForcingData");

  DataBase & db = dbase.get<DataBase>("userDefinedForcingData");

  // option = the name of the user defined forcing.
  if( !db.has_key("option") )
  { // create option variable in the data base.
    db.put<aString>("option");
    db.get<aString>("option")="none"; // default option
  }
  aString & option = db.get<aString>("option");

  // Here is the CompositeGrid: 
  assert( cgp!=NULL );
  CompositeGrid & cg = *cgp;
  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  const int numberOfDimensions = cg.numberOfDimensions();

  for( ;; )
  {
    gi.getMenuItem(menu,answer,"enter an option");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="no forcing" )
    {
      option="none";
    }
    else if( answer=="my source" )
    {

      option="my source";

      // Query user for parameters

    }
    else if( answer=="gaussian sources" )

    {
      // define a Gaussian forcing
      option="gaussianSources";

      // We save parameters in the data base 

      if( !db.has_key("numberOfGaussianSources") ) db.put<int>("numberOfGaussianSources");
      int & numberOfGaussianSources = db.get<int>("numberOfGaussianSources");

      gi.inputString(answer2,sPrintF("Enter the number of Gaussian sources (default = 1)"));
      sScanF(answer2,"%i",&numberOfGaussianSources);

      if( numberOfDimensions==2 )
      {
	printF("The Gaussian source in 2D is of the form:\n"
               " g(x,y,t) = a*sin(2*pi*omega*(t-t0) )*exp( -beta*[ (x-x0)^2 + (y-y0)^2 ]^p )\n"
               "     R = (x-x0)^2 + (y-y0)^2\n"
	       " F(Ex) = -(y-y0)*R^(p-1)*g(x,y,t) \n"
	       " F(Ey) =  (x-x0)*R^(p-1)**g(x,y,t) \n"
	       " F(Hz) =         g(x,y,t) \n"
	  );
      }
      else
      {
	printF("The Gaussian source in 3D is of the form:\n"
               " g(x,y,z,t) = a*sin(2*pi*omega*(t-t0) )*exp( -beta*[ (x-x0)^2 + (y-y0)^2 + (z-z0)^2 ]^p )\n"
               "     R = (x-x0)^2 + (y-y0)^2\n"
	       " F(Ex) = [(z-z0)-(y-y0)]*R^(p-1)**g(x,y,z,t) \n"
	       " F(Ey) = [(x-x0)-(z-z0)]*R^(p-1)**g(x,y,z,t) \n"
	       " F(Ez) = [(y-xy)-(x-x0)]*R^(p-1)**g(x,y,z,t) \n"
	  );
      }
      
      if( !db.has_key("gaussianParameters") )
	db.put<RealArray>("gaussianParameters");

      RealArray & gaussianParameters = db.get<RealArray>("gaussianParameters");
      gaussianParameters.redim(8,numberOfGaussianSources);
      gaussianParameters=0.;
      
      for( int m=0; m<numberOfGaussianSources; m++ )
      {
        real a=1., beta=10., omega=1., p=1., x0=0., y0=0., z0=0., t0=0.;
	gi.inputString(answer2,sPrintF("Source %i: Enter a,beta,omega,p,x0,y0,z0,t0",m));
        sScanF(answer2,"%e %e %e %e %e %e %e %e",&a,&beta,&omega,&p,&x0,&y0,&z0,&t0);

        printF("Gaussian source %i: setting a=%8.2e, beta=%8.2e, omega=%8.2e, p=%8.2e, x0=%8.2e, y0=%8.2e, "
               "z0=%8.2e, t0=%8.2e\n", m,a,beta,omega,p,x0,y0,z0,t0);

        gaussianParameters(0,m)=a; 
        gaussianParameters(1,m)=beta; 
        gaussianParameters(2,m)=omega; 
        gaussianParameters(3,m)=p;
        gaussianParameters(4,m)=x0; 
        gaussianParameters(5,m)=y0; 
        gaussianParameters(6,m)=z0;
        gaussianParameters(7,m)=t0;
      }

    }
    else if( answer=="manufactured pulse" )
    {
      option="manufacturedPulse";
      printF("--MX-- Setting user defined forcing = manufacturedPulse\n");
      printF("--MX-- You should choose the user defined know solution to also be 'manufactured pulse'\n");
    }
    else 
    {
      printF("Maxwell::setupUserDefinedForcing:ERROR: unknown option =[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
  }
  
  gi.unAppendTheDefaultPrompt();
  return 0;
}



//! This routine is called when cgmx is finished and can be used to clean up memory.
void Maxwell::
userDefinedForcingCleanup()
{
  printF("***userDefinedForcingCleanup: delete arrays\n");

  if( dbase.has_key("userDefinedForcingData") )
  {
    DataBase & db = dbase.get<DataBase>("userDefinedForcingData");

    // For now we do thing here 

  }
  
}

