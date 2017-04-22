#include "Maxwell.h"
#include "DispersiveMaterialParameters.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

// ==========================================================================================
/// \brief  Evaluate a user defined known solution.
///
/// \param numberOfTimeDerivatives (input) : evaluate this many time-derivatives of the solution.
///     Normally  numberOfTimeDerivatives=0, but it can be 1 when the known solution is used
//      to define boundary conditions 
// ==========================================================================================
int Maxwell::
getUserDefinedKnownSolution(real t, CompositeGrid & cg, int grid, realArray & ua, 
			    const Index & I1a, const Index &I2a, const Index &I3a, 
                            int numberOfTimeDerivatives /* = 0 */ )
{
  if( false )
    printF("--MX--getUserDefinedKnownSolution at t=%9.3e\n",t);

  MappedGrid & mg = cg[grid];
  const int numberOfDimensions = cg.numberOfDimensions();
  const real & dt= deltaT;
  
  if( ! dbase.has_key("userDefinedKnownSolutionData") )
  {
    printF("--MX-- getUserDefinedKnownSolution:ERROR: sub-directory `userDefinedKnownSolutionData' not found!\n");
    OV_ABORT("error");
  }
  DataBase & db =  dbase.get<DataBase>("userDefinedKnownSolutionData");

  const aString & userKnownSolution = db.get<aString>("userKnownSolution");

  real *rpar = db.get<real[20]>("rpar");
  int *ipar = db.get<int[20]>("ipar");
  
  OV_GET_SERIAL_ARRAY(real,ua,uLocal);

  Index I1=I1a, I2=I2a, I3=I3a;
  bool ok = ParallelUtility::getLocalArrayBounds(ua,uLocal,I1,I2,I3,1);   
  if( !ok ) return 0;  // no points on this processor (NOTE: no communication should be done after this point)

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

  
  if( userKnownSolution=="manufacturedPulse" )
  {
    // Manufactured pulse:
    //   A pulse like solution that requires a forcing function to make it a solution
    //   Used to test the forcing terms in the equations.
    const real amp = rpar[0];
    const real beta= rpar[1];
    const real x0  = rpar[2];
    const real y0  = rpar[3];
    const real z0  = rpar[4];
    const real cx  = rpar[5];
    const real cy  = rpar[6];
    const real cz  = rpar[7];

    real x,y,z;
    if( numberOfTimeDerivatives==0 )
    {
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
	  
	  real psi = amp*exp(-beta*( SQR(x-x0-cx*t) + SQR(y-y0-cy*t) ));
	  uLocal(i1,i2,i3,ex) = -(y-y0-cy*t)*psi;    // Ex =  psi_y * const 
	  uLocal(i1,i2,i3,ey) =  (x-x0-cx*t)*psi;    // Ey = -psi_x * const
	  uLocal(i1,i2,i3,hz) =  psi;
          if( method==sosup )
	  {
	    // supply time-derivatives for sosup scheme

            // **check me**
	    real psit = (2.*beta)*( cx*(x-x0-cx*t) + cy*(y-y0-cy*t) )*psi;
	    uLocal(i1,i2,i3,ext) =  cy*psi  -(y-y0-cy*t)*psit;    
	    uLocal(i1,i2,i3,eyt) = -cx*psi  +(x-x0-cx*t)*psit;    
	    uLocal(i1,i2,i3,hzt) =  psit;
	  }
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

	  real psi = amp*exp(-beta*( SQR(x-x0-cx*t) + SQR(y-y0-cy*t) + SQR(z-z0-cz*t) ));
	  uLocal(i1,i2,i3,ex) = ((z-z0-cz*t)-(y-y0-cy*t))*psi;    // Ex = ( psi_z - psi_y ) * const
	  uLocal(i1,i2,i3,ey) = ((x-x0-cx*t)-(z-z0-cz*t))*psi;    // Ey = ( psi_x - psi_z ) * const
	  uLocal(i1,i2,i3,ez) = ((y-y0-cy*t)-(x-x0-cx*t))*psi;    // Ez = ( psi_y - psi_x ) * const
          if( method==sosup )
	  {
	    // supply time-derivatives for sosup scheme

            // **check me**
	    real psit = (2.*beta)*( cx*(x-x0-cx*t) + cy*(y-y0-cy*t) +cz*(z-z0-cz*t))*psi;
	    uLocal(i1,i2,i3,ext) = (-cz+cy)*psi + ((z-z0-cz*t)-(y-y0-cy*t))*psit;    
	    uLocal(i1,i2,i3,eyt) = (-cx+cz)*psi + ((x-x0-cx*t)-(z-z0-cz*t))*psit;    
            uLocal(i1,i2,i3,ezt) = (-cy+cx)*psi + ((y-y0-cy*t)-(x-x0-cx*t))*psit;
	  }	  
	  

	}
      }
    }
    else
    {
      OV_ABORT("finish me: numberOfTimeDerivatives1=0");
      
    }
    
  }
  else if( userKnownSolution=="chirpedPlaneWave" )
  {
    // -------------------------------------------
    // ---------- Chirped plane wave  -------------
    // -------------------------------------------

    // Chirped plane-wave parameters
    const ChirpedArrayType & cpw = dbase.get<ChirpedArrayType >("chirpedParameters");
    const real cpwTa   =cpw(0); // ta 
    const real cpwTb   =cpw(1); // tb 
    const real cpwAlpha=cpw(2); // alpha
    const real cpwBeta =cpw(3); // beta
    const real cpwAmp  =cpw(4); // amp
    const real cpwX0   =cpw(5); // x0
    const real cpwY0   =cpw(6); // y0
    const real cpwZ0   =cpw(7); // z0

    const real xi0 = .5*(cpwTa+cpwTb);
    const real cpwTau= cpwTb-cpwTa;    // tau=tb-ta
    
    printF("--UDKS-- eval chirped plane wave at t=%10.3e, [ta,tb]=[%g,%g]\n",t,cpwTa,cpwTb);

    c = cGrid(grid);
    const real cc= c*sqrt( kx*kx+ky*ky+kz*kz);

    // For checking the scattering from a planar PEC boundary we change the
    // sign of the solution to be that for for scattered field.
    const bool & solveForScatteredField = dbase.get<bool>("solveForScatteredField");
    const real signForField = solveForScatteredField ? -1. : 1.;

    real x,y,z;
    if( numberOfTimeDerivatives==0 )
    {
      if( numberOfDimensions==2 )
      {
        // ----------- 2D --------------
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
 	  
          real xi = t - (kx*(x-cpwX0)+ky*(y-cpwY0))/cc - xi0;

	  real tanha = tanh(cpwBeta*(xi+.5*cpwTau));
	  real tanhb = tanh(cpwBeta*(xi-.5*cpwTau));
	  real amp = cpwAmp*.5*( tanha - tanhb );

	  real phi = cc*xi + cpwAlpha*xi*xi;
          real sinPhi = sin(twoPi*phi);

	  real  chirp = signForField*amp*sinPhi;
	  
	  uLocal(i1,i2,i3,ex) = chirp*pwc[0];
	  uLocal(i1,i2,i3,ey) = chirp*pwc[1];
	  uLocal(i1,i2,i3,hz) = chirp*pwc[5];
          if( method==sosup )
	  {
	    // supply time-derivatives for sosup scheme
            
            // tanh' = 1 - tanh^2
            real damp= cpwAmp*.5*cpwBeta*( -tanha*tanha + tanhb*tanhb );
            real dphi = cc + 2.*cpwAlpha*xi;

            chirp = signForField*( damp*sinPhi + amp*twoPi*dphi*cos(twoPi*phi) );

	    uLocal(i1,i2,i3,ext) = chirp*pwc[0];
	    uLocal(i1,i2,i3,eyt) = chirp*pwc[1];
	    uLocal(i1,i2,i3,hzt) = chirp*pwc[5];
	  }
	}
      }
      else
      {
        // ----------- 3D --------------
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

          real xi = t - (kx*(x-cpwX0)+ky*(y-cpwY0))/cc - xi0;

          // these next formulae are the same as in 2D
	  real tanha = tanh(cpwBeta*(xi+.5*cpwTau));
	  real tanhb = tanh(cpwBeta*(xi-.5*cpwTau));
	  real amp = cpwAmp*.5*( tanha - tanhb );

	  real phi = cc*xi + cpwAlpha*xi*xi;
          real sinPhi = sin(twoPi*phi);

	  real  chirp = signForField*amp*sinPhi;
	  
	  uLocal(i1,i2,i3,ex) =  chirp*pwc[0];
	  uLocal(i1,i2,i3,ey) =  chirp*pwc[1];
	  uLocal(i1,i2,i3,ez) =  chirp*pwc[2];
          if( method==sosup )
	  {
	    // supply time-derivatives for sosup scheme
            // tanh' = 1 - tanh^2
            real damp= cpwAmp*.5*cpwBeta*( -tanha*tanha + tanhb*tanhb );
            real dphi = cc + 2.*cpwAlpha*xi;

            chirp = signForField*( damp*sinPhi + amp*twoPi*dphi*cos(twoPi*phi) );

	    uLocal(i1,i2,i3,ext) = chirp*pwc[0];
	    uLocal(i1,i2,i3,eyt) = chirp*pwc[1];
	    uLocal(i1,i2,i3,ezt) = chirp*pwc[2];

	  }	  
	  

	}
      }
    }
    else
    {
      OV_ABORT("finish me: numberOfTimeDerivatives1=0");
      
    }
    
  }
  
  else if( userKnownSolution=="dispersivePlaneWave" )
  {
    // -----------------------------------------------
    // ---------- Dispersive plane wave  -------------
    // -----------------------------------------------

    assert( dispersionModel!=noDispersion );

    DispersiveMaterialParameters & dmp = getDispersiveMaterialParameters(grid);

    // evaluate the dispersion relation,  exp(i(k*x-omega*t))
    //    omega is complex 
    const real kk = twoPi*sqrt( kx*kx+ky*ky+kz*kz); // true wave-number (note factor of twoPi)
    real omegaDpwRe, omegaDpwIm;
    dmp.computeDispersivePlaneWaveParameters( c,eps,mu,kk, omegaDpwRe, omegaDpwIm );

    if( t<3.*dt )
      printF("--UDKS:DPW-- t=%10.3e, omegar=%g, omegai=%g\n",t,omegaDpwRe, omegaDpwIm );

    const real dpwExp =exp(omegaDpwIm*t);
    
    const real c = cGrid(grid);
    const real cc= c*sqrt( kx*kx+ky*ky+kz*kz);

    const real eps = epsGrid(grid);
    const real mu  = muGrid(grid);
    const real ck2 = SQR(c*kk);

    // compute coefficients of P :   s = sr+I*si = -I*omega = -I*( omegar + I omegai) = omegai - I*omegar
    real sr = omegaDpwIm, si=-omegaDpwRe;
    real sNorm2=sr*sr+si*si, sNorm4=sNorm2*sNorm2;
    real pc = -eps*( -2.*sr*si*ck2/sNorm4 );
    real ps = -eps*( 1. + ck2*(sr*sr-si*si)/sNorm4 );

    //  mu * (Hz)_t = (Ex)_y - (Ey)_x
    //  Hz = [hc*cos(xi) + hs*sin(xi) ]*exp(omegai*t)
    // *check me*
    real factor = twoPi*( kx*pwc[1] - ky*pwc[0] )/mu;  // (kx*Ey - ky*Ex )/mu
    real omegaNorm2=SQR(omegaDpwRe)+SQR(omegaDpwIm);
    real hs =  factor*omegaDpwRe/omegaNorm2;
    real hc = -factor*omegaDpwIm/omegaNorm2;
    

    if( t<3.*dt )
      printF("--UDKS:DPW-- ck2=%10.3e, pc=%g, ps=%g, sr=%g, si=%g, hc=%g hs=%g\n",ck2,pc,ps,sr,si,hc,hs);

    real x,y,z;
    if( numberOfTimeDerivatives==0 )
    {
      if( numberOfDimensions==2 )
      {
        // ----------- 2D --------------
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
 	  real xi=twoPi*(kx*x+ky*y) -omegaDpwRe*t;
	  real sinxi=sin(xi), cosxi=cos(xi);

	  uLocal(i1,i2,i3,ex) = sinxi*pwc[0]*dpwExp;
	  uLocal(i1,i2,i3,ey) = sinxi*pwc[1]*dpwExp;
	  uLocal(i1,i2,i3,hz) = (hc*cosxi+hs*sinxi)*dpwExp;

          // -- polarization vector --
          uLocal(i1,i2,i3,pxc) = (pc*cosxi+ps*sinxi)*pwc[0]*dpwExp;
          uLocal(i1,i2,i3,pyc) = (pc*cosxi+ps*sinxi)*pwc[1]*dpwExp;

          if( method==sosup )
	  {
	    // supply time-derivatives for sosup scheme
            OV_ABORT("finish me");
	  }
	}
      }
      else
      {
        // ----------- 3D --------------
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

	  OV_ABORT("finish me");
      
	}
      }
    }
    else
    {
      OV_ABORT("finish me: numberOfTimeDerivatives1=0");

    }
    
  }
  
  else
  {
    printF("getUserDefinedKnownSolution:ERROR: unknown value for userDefinedKnownSolution=%s\n",
	   (const char*)userKnownSolution);
    OV_ABORT("ERROR");
  }
  
  return 0;
}


int Maxwell::
updateUserDefinedKnownSolution(GenericGraphicsInterface & gi, CompositeGrid & cg)
// ==========================================================================================
/// \brief This function is called to set the user defined know solution.
/// 
// ==========================================================================================
{
  // Make  dbase.get<real >("a") sub-directory in the data-base to store variables used here
  if( ! dbase.has_key("userDefinedKnownSolutionData") )
     dbase.put<DataBase>("userDefinedKnownSolutionData");
  DataBase & db =  dbase.get<DataBase>("userDefinedKnownSolutionData");

  if( !db.has_key("userKnownSolution") )
  {
    db.put<aString>("userKnownSolution");
    db.get<aString>("userKnownSolution")="unknownSolution";
    
    db.put<real[20]>("rpar");
    db.put<int[20]>("ipar");
  }
  aString & userKnownSolution = db.get<aString>("userKnownSolution");
  real *rpar = db.get<real[20]>("rpar");
  int *ipar = db.get<int[20]>("ipar");


  const aString menu[]=
    {
      "no known solution",
      "manufactured pulse",
      "chirped plane wave",
      "dispersive plane wave",
      "done",
      ""
    }; 

  gi.appendToTheDefaultPrompt("userDefinedKnownSolution>");
  aString answer;
  for( ;; ) 
  {

    int response=gi.getMenuItem(menu,answer,"Choose a known solution");
    
    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( answer=="no known solution" )
    {
      userKnownSolution="unknownSolution";
    }
    else if( answer=="manufactured pulse" ) 
    {
      userKnownSolution="manufacturedPulse";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution depends on time
      

      printF("The manufactured pulse is based on \n"
             "    psi = amp*( -beta*( (x-x0-cx*t)^2 + (y-y0-cy*t)^2 + (z-z0-cz*t)^2 ) )\n"
             " ---2D ---\n"
	     "   Ex = -(y-y0-cy*t)*psi   ( Ex =  psi_y * const )\n"
             "   Ey =  (x-x0-cx*t)*psi   ( Ey = -psi_x * const )\n"
	     "   Hz =   psi;\n"
             " --- 3D ---\n"
	     "  Ex = ((z-z0-cz*t)-(y-y0-cy*t))*psi    ( Ex = ( psi_z - psi_y ) * const)\n"
	     "  Ey = ((x-x0-cx*t)-(z-z0-cz*t))*psi    ( Ey = ( psi_x - psi_z ) * const)\n"
	     "  Ez = ((y-y0-cy*t)-(x-x0-cx*t))*psi    ( Ez = ( psi_y - psi_x ) * const)\n"
                 );
      gi.inputString(answer,"Enter amp,beta,x0,y0,z0, cx,cy,cz");
      sScanF(answer,"%e %e %e %e %e %e %e %e",&rpar[0],&rpar[1],&rpar[2],&rpar[3],&rpar[4],&rpar[5],&rpar[6],&rpar[7]);
      printF(" Setting amp=%g, beta=%g, [x0,y0,z0]=[%g,%g,%g] [cx,cy,cz]=[%g,%g,%g]\n",
	     rpar[0],rpar[1],rpar[2],rpar[3],rpar[4],rpar[5],rpar[6],rpar[7]);
      
    }
    else if( answer=="chirped plane wave" ) 
    {
      userKnownSolution="chirpedPlaneWave";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution depends on time
      

      printF("The chirped plane wave is defined by ...\n");
      
      
    }
    
    else if( answer=="dispersive plane wave" ) 
    {
      userKnownSolution="dispersivePlaneWave";
      dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution depends on time
      

      printF("The dispersive plane wave is defined by: \n"
             "    E = a*sin( k*x - omegar*t)*exp(omegai*t)\n"
             "    P = [b*cos( k*x - omegar*t) + c*sin( k*x - omegar*t) ]*exp(omegai*t)\n"
            );
      
      
    }
    
    else
    {
      printF("unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
  }

  gi.unAppendTheDefaultPrompt();
  bool knownSolutionChosen = userKnownSolution!="unknownSolution";
  return knownSolutionChosen;
}
