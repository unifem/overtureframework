// This file automatically generated from userDefinedKnownSolution.bC with bpp.
#include "Maxwell.h"
#include "DispersiveMaterialParameters.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

typedef ::real LocalReal;
// utility routine (using complex numbers) defined at the bottom of the file.
void
getTransmisionWaveNumber( const LocalReal & kr,  const LocalReal & ki, 
                                                    const LocalReal & kxr, const LocalReal & kxi, 
                                                    const LocalReal & kyr, const LocalReal & kyi, 
                                                    LocalReal & kxpr, LocalReal & kxpi, 
                                                    LocalReal & kypr, LocalReal & kypi );

void
checkPlaneMaterialInterfaceJumps( 
                                                    const LocalReal & c1, const LocalReal & c2,
                                                    const LocalReal & eps1, const LocalReal & eps2,
                                                    const LocalReal & mu1, const LocalReal & mu2,

                                                    const LocalReal & sr, const LocalReal & si,
                                                    const LocalReal & rr, const LocalReal & ri, 
                                                    const LocalReal & taur, const LocalReal & taui, 

                                                    const LocalReal & eps1Hatr, const LocalReal & eps1Hati,
                                                    const LocalReal & eps2Hatr, const LocalReal & eps2Hati,

                                                    const LocalReal & psiSum1r, const LocalReal & psiSum1i,
                                                    const LocalReal & psiSum2r, const LocalReal & psiSum12i,
                                                    const LocalReal & kxr, const LocalReal & kxi,
                                                    const LocalReal & kyr, const LocalReal & kyi,
                                                    const LocalReal & kxpr, const LocalReal & kxpi,
                                                    const LocalReal & kypr, const LocalReal & kypi
    );


// ==========================================================================================
/// \brief  Evaluate a user defined known solution.
///
/// \param numberOfTimeDerivatives (input) : evaluate this many time-derivatives of the solution.
///     Normally  numberOfTimeDerivatives=0, but it can be 1 when the known solution is used
//      to define boundary conditions 
// ==========================================================================================
int Maxwell::
getUserDefinedKnownSolution(int current, real t, CompositeGrid & cg, int grid, 
                                                        realArray & ua, realArray & pv,
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
    
  // else if( userKnownSolution=="dispersivePlaneWave" )
  // {
  //   // -----------------------------------------------
  //   // ---------- Dispersive plane wave  -------------
  //   // -----------------------------------------------

  //   assert( dispersionModel!=noDispersion );

  //   DispersiveMaterialParameters & dmp = getDispersiveMaterialParameters(grid);

  //   // evaluate the dispersion relation,  exp(i(k*x-omega*t))
  //   //    omega is complex 
  //   const real kk = twoPi*sqrt( kx*kx+ky*ky+kz*kz); // true wave-number (note factor of twoPi)
  //   real omegaDpwRe, omegaDpwIm;
  //   dmp.computeDispersivePlaneWaveParameters( c,eps,mu,kk, omegaDpwRe, omegaDpwIm );

  //   if( t<3.*dt )
  //     printF("--UDKS:DPW-- t=%10.3e, omegar=%g, omegai=%g\n",t,omegaDpwRe, omegaDpwIm );

  //   const real dpwExp =exp(omegaDpwIm*t);
        
  //   const real c = cGrid(grid);
  //   const real cc= c*sqrt( kx*kx+ky*ky+kz*kz);

  //   const real eps = epsGrid(grid);
  //   const real mu  = muGrid(grid);
  //   const real ck2 = SQR(c*kk);

  //   // compute coefficients of P :   s = sr+I*si = -I*omega = -I*( omegar + I omegai) = omegai - I*omegar
  //   //     s^2 E = -(c*k)^2 E - (s^2/eps) * P 
  //   // ->  P = -eps*( 1 + (c*k/s)^2 ) E 
  //   real sr = omegaDpwIm, si=-omegaDpwRe;
  //   real sNorm2=sr*sr+si*si, sNorm4=sNorm2*sNorm2;
  //   real pc = -eps*( -2.*sr*si*ck2/sNorm4 );
  //   real ps = -eps*( 1. + ck2*(sr*sr-si*si)/sNorm4 );

  //   //  mu * (Hz)_t = (Ex)_y - (Ey)_x
  //   //  Hz = [hc*cos(xi) + hs*sin(xi) ]*exp(omegai*t)
  //   // *check me*
  //   real factor = twoPi*( kx*pwc[1] - ky*pwc[0] )/mu;  // (kx*Ey - ky*Ex )/mu
  //   real omegaNorm2=SQR(omegaDpwRe)+SQR(omegaDpwIm);
  //   real hs =  factor*omegaDpwRe/omegaNorm2;
  //   real hc = -factor*omegaDpwIm/omegaNorm2;
        

  //   if( t<3.*dt )
  //     printF("--UDKS:DPW-- ck2=%10.3e, pc=%g, ps=%g, sr=%g, si=%g, hc=%g hs=%g\n",ck2,pc,ps,sr,si,hc,hs);

  //   real x,y,z;
  //   if( numberOfTimeDerivatives==0 )
  //   {
  //     if( numberOfDimensions==2 )
  //     {
  //       // ----------- 2D --------------
  //       FOR_3D(i1,i2,i3,I1,I2,I3)
  //       {
  //         if( !isRectangular )
  //         {
  //          x= xLocal(i1,i2,i3,0);
  //          y= xLocal(i1,i2,i3,1);
  //         }
  //         else
  //         {
  //           x=XC(iv,0);
  //           y=XC(iv,1);
  //         }
  //         real xi=twoPi*(kx*x+ky*y) -omegaDpwRe*t;
  //         real sinxi=sin(xi), cosxi=cos(xi);

  //         uLocal(i1,i2,i3,ex) = sinxi*pwc[0]*dpwExp;
  //         uLocal(i1,i2,i3,ey) = sinxi*pwc[1]*dpwExp;
  //         uLocal(i1,i2,i3,hz) = (hc*cosxi+hs*sinxi)*dpwExp;

  //         // -- polarization vector --
  //         uLocal(i1,i2,i3,pxc) = (pc*cosxi+ps*sinxi)*pwc[0]*dpwExp;
  //         uLocal(i1,i2,i3,pyc) = (pc*cosxi+ps*sinxi)*pwc[1]*dpwExp;

  //         if( method==sosup )
  //         {
  //           // supply time-derivatives for sosup scheme
  //           OV_ABORT("finish me");
  //         }
  //       }
  //     }
  //     else
  //     {
  //       // ----------- 3D --------------
  //       FOR_3D(i1,i2,i3,I1,I2,I3)
  //       {
  //         if( !isRectangular )
  //         {
  //           x= xLocal(i1,i2,i3,0);
  //           y= xLocal(i1,i2,i3,1);
  //           z= xLocal(i1,i2,i3,2);
  //         }
  //         else
  //         {
  //           x=XC(iv,0);
  //           y=XC(iv,1);
  //           z=XC(iv,2);
  //         }

  //         OV_ABORT("finish me");
            
  //       }
  //     }
  //   }
  //   else
  //   {
  //     OV_ABORT("finish me: numberOfTimeDerivatives1=0");

  //   }
        
  // }
    
    else if( userKnownSolution=="dispersivePlaneWaveInterface" )
    {
    // ----------------------------------------------------
    // ---- DISPERSIVE PLANE WAVE MATERIAL INTERFACE ------
    // ----------------------------------------------------
    // NOTES:
    //    (1) incident wave number is given --> compute s=sr + I*si 
    //    (2) Given s, compute wave number in right state

        assert( dispersionModel!=noDispersion );
        assert( cg.numberOfDomains()==2 );


        real sr,si;
        real kxr, kxi, kyr, kyi;        // Incident wave number (complex)
        real kxpr, kxpi, kypr, kypi;    // Transmitted wave number
    
        real psi1r[10],psi1i[10];
        real psi2r[10],psi2i[10];

        real psiSum1r=0.,psiSum1i=0;
        real psiSum2r=0.,psiSum2i=0;
  
      
        const int gridLeft = 0;
        const int gridRight=cg.numberOfComponentGrids()-1;

        real c1,c2,eps1,eps2,mu1,mu2;
        if( method==yee )
        {
            eps1=epsv(gridLeft);  mu1=muv(gridLeft);    // incident 
            eps2=epsv(gridRight); mu2=muv(gridRight);   // transmitted
        }
        else
        {
            eps1=epsGrid(gridLeft);  mu1=muGrid(gridLeft); // incident
            eps2=epsGrid(gridRight); mu2=muGrid(gridRight); // transmitted

        }
        c1=1./sqrt(eps1*mu1);  // incident 
        c2=1./sqrt(eps2*mu2);  // transmitted

        int domain=0;
        DispersiveMaterialParameters & dmp1 = getDomainDispersiveMaterialParameters(domain);
        const int & numberOfPolarizationVectors1=dmp1.numberOfPolarizationVectors;
        assert( numberOfPolarizationVectors1<10 );

        kxr=twoPi*kx; kxi=0.; kyr=twoPi*ky; kyi=0.;  // Incident wave number (complex)

        const real kk = twoPi*sqrt( kx*kx+ky*ky+kz*kz );   
        dmp1.evaluateDispersionRelation( c1,kk, sr, si, psi1r,psi1i ); 
    // si = -si; // reverse the direction NO -- changes psi1 !

    // Save the "sum" of the psi's times alphaP
        psiSum1r=0.; psiSum1i=0;
        for( int iv=0; iv<dmp1.numberOfPolarizationVectors; iv++ )
        {
            psiSum1r += psi1r[iv];
            psiSum1i += psi1i[iv];
        }
        psiSum1r *= dmp1.alphaP;
        psiSum1i *= dmp1.alphaP;
                
    // -- -right domain --
        domain=1;
        DispersiveMaterialParameters & dmp2 = getDomainDispersiveMaterialParameters(domain);
        const int & numberOfPolarizationVectors2=dmp2.numberOfPolarizationVectors;
        assert( numberOfPolarizationVectors2<10 );
      
        real kr,ki;
        dmp2.evaluateComplexWaveNumber( c2,sr,si, kr,ki, psi2r,psi2i );
    //  kxp^2 + kyp^2 = (kr+I*ki)^2 = (kr^2-ki^2) + 2*I*kr*ki 
    // kxp = kxpr + I*kpri = sqrt( (kr+I*ki)^2 - kyp^2 )
        getTransmisionWaveNumber( kr,ki, kxr,kxi, kyr,kyi, kxpr,kxpi, kypr,kypi );
    // // do this for now -- assume normal incidence
    // assert( ky==0. );
    // kxpr=kr; kxpi=ki;
    // kypr=0.; kypi=0.;
      
        if( t<3.*dt )
        {
            printF("\n --UDKS:DPWI-- t=%10.4e, grid=%i, s=(%16.10e,%16.10e) kx=(%16.10e,%16.10e) ky=(%16.10e,%16.10e) -> k2=(kr,ki)=(%16.10e,%16.10e) kxp=(%16.10e,%16.10e) kyp=(%16.10e,%16.10e)\n"
                          ,t,grid,sr,si,kxr,kxi,kyr,kyi,kr,ki,kxpr,kxpi,kypr,kypi);
            printF("    psi1=(%16.10e,%16.10e), psi2=(%16.10e,%16.10e) \n",psi1r[0],psi1i[0],psi2r[0],psi2i[0]);
            
        }
        

    // // Save the "sum" of the psi's  times alphaP
        psiSum2r=0.; psiSum2i=0;
        for( int iv=0; iv<dmp2.numberOfPolarizationVectors; iv++ )
        {
            psiSum2r += psi2r[iv];
            psiSum2i += psi2i[iv];
        }
        psiSum2r *= dmp2.alphaP;
        psiSum2i *= dmp2.alphaP;

        const real alphaP = dmp1.alphaP;
        const real a= 1.; // amplitude *FIX ME**


    // domain number for this grid: 
        const int myDomain = cg.domainNumber(grid);

    // --- Get Arrays for the dispersive model ----

    // realMappedGridFunction & pCur = getDispersionModelMappedGridFunction( grid,current );
        RealArray pLocal;
        if( (myDomain==0 && numberOfPolarizationVectors1>0) ||
                (myDomain==1 && numberOfPolarizationVectors2>0)  )
        {
            OV_GET_SERIAL_ARRAY(real, pv,pLoc);
            pLocal.reference(pLoc);
        }

        real x,y,z=0.;
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

          // Here are the statements to eval the solution: 
//           #Include "dispersivePlaneWaveInterface.h"
// File generated by overtureFramework/cg/mx/codes/dispersivePlaneWaveInterface.maple
// File generated by DropBox/DMX/codes/dispersivePlaneWaveInterface.maple
real t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21,t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37,t38,t39,t40,t41,t42,t43,t44,t45,t46,t47,t48,t49,t50,t51,t52,t53,t54,t55,t56,t57,t58,t59,t60,t61,t62,t63,t64,t65,t66,t67,t68,t69,t70,t71,t72,t73,t74,t75,t76,t77,t78,t79,t80,t81,t82,t83,t84,t85,t86,t87,t88,t89,t90,t91,t92,t93,t94,t95,t96,t97,t98,t99,t100,t101,t102,t103,t104,t105,t106,t107,t108,t109,t110,t111,t112,t113,t114,t115,t116,t117,t118,t119,t120,t121,t122,t123,t124,t125,t126,t127,t128,t129,t130,t131,t132,t133,t134,t135,t136,t137,t138,t139,t140,t141,t142,t143,t144,t145,t146,t147,t148,t149,t150,t151,t152,t153,t154,t155,t156,t157,t158,t159,t160,t161,t162,t163,t164,t165,t166,t167,t168,t169,t170,t171,t172,t173,t174,t175,t176,t177,t178,t179,t180,t181,t182,t183,t184,t185,t186,t187,t188,t189,t190,t191,t192,t193,t194,t195,t196,t197,t198,t199,t200,t201,t202,t203,t204,t205,t206,t207,t208,t209,t210,t211,t212,t213,t214,t215,t216,t217,t218,t219,t220,t221,t222,t223,t224,t225,t226,t227,t228,t229,t230,t231,t232,t233,t234,t235,t236,t237,t238,t239,t240,t241,t242,t243,t244,t245,t246,t247,t248,t249,t250,t251,t252,t253,t254,t255,t256,t257,t258,t259,t260,t261,t262,t263,t264,t265,t266,t267,t268,t269,t270,t271,t272,t273,t274,t275,t276,t277,t278,t279,t280,t281,t282,t283,t284,t285,t286,t287,t288,t289,t290,t291,t292,t293,t294,t295,t296,t297,t298,t299,t300,t301,t302,t303,t304,t305,t306,t307,t308,t309,t310,t311,t312,t313,t314,t315,t316,t317,t318,t319,t320,t321,t322,t323,t324,t325,t326,t327,t328,t329,t330,t331,t332,t333,t334,t335,t336,t337,t338,t339,t340,t341,t342,t343,t344,t345,t346,t347,t348,t349,t350,t351,t352,t353,t354,t355,t356,t357,t358,t359,t360,t361,t362,t363,t364,t365,t366,t367,t368,t369,t370,t371,t372,t373,t374,t375,t376,t377,t378,t379,t380,t381,t382,t383,t384,t385,t386,t387,t388,t389,t390,t391,t392,t393,t394,t395,t396,t397,t398,t399,t400,t401,t402,t403,t404,t405,t406,t407,t408,t409,t410,t411,t412,t413,t414,t415,t416,t417,t418,t419,t420,t421,t422,t423,t424,t425,t426,t427,t428,t429,t430,t431,t432,t433,t434,t435,t436,t437,t438,t439,t440,t441,t442,t443,t444,t445,t446,t447,t448,t449,t450,t451,t452,t453,t454,t455,t456,t457,t458,t459,t460,t461,t462,t463,t464,t465,t466,t467,t468,t469,t470,t471,t472,t473,t474,t475,t476,t477,t478,t479,t480,t481,t482,t483,t484,t485,t486,t487,t488,t489,t490,t491,t492,t493,t494,t495,t496,t497,t498,t499,t500,t501,t502,t503,t504,t505,t506,t507,t508,t509,t510,t511,t512,t513,t514,t515,t516,t517,t518,t519,t520,t521,t522,t523,t524,t525,t526,t527,t528,t529,t530,t531,t532,t533,t534,t535,t536,t537,t538,t539,t540,t541,t542,t543,t544,t545,t546,t547,t548,t549,t550,t551,t552,t553,t554,t555,t556,t557,t558,t559,t560,t561,t562,t563,t564,t565,t566,t567,t568,t569,t570,t571,t572,t573,t574,t575,t576,t577,t578,t579,t580,t581,t582,t583,t584,t585,t586,t587,t588,t589,t590,t591,t592,t593,t594,t595,t596,t597,t598,t599,t600;
// -------------------------------------------------------------------------
// Need: 
//    a = amplitude of the wave, e.g. a=1
//    x,y,t
//    sr,si : s= sr + I*si 
//    alphaP
//    kxr,kxi,  kyr,kyi,     : complex wave number on left
//    kxpr,kxpi,  kypr,kypi, : complex wave number on right (plus)
// -------------------------------------------------------------------------
// Evaluated:                                                              
//    Exr,Eyr   : left state                                                
//    Expr,Eypr : right state                                               
// -------------------------------------------------------------------------
real kNorm = sqrt( kxr*kxr + kxi*kxi + kyr*kyr + kyi*kyi);
real khxr = kxr/kNorm, khxi = kxi/kNorm, khyr=kyr/kNorm, khyi=kyi/kNorm; 
real kpNorm = sqrt( kxpr*kxpr + kxpi*kxpi + kypr*kypr + kypi*kypi);
real khxpr = kxpr/kpNorm, khxpi=kxpi/kpNorm, khypr=kypr/kpNorm, khypi=kypi/kpNorm; 
real kappar,kappai, betar, betai, rr,ri, taur,taui;
real Exr,Eyr,Hzr, Expr,Eypr,Hzpr;
real Exi,Eyi,Hzi, Expi,Eypi,Hzpi;
t1 = khxi * khxpi;
t2 = khxr * khxpr;
t4 = pow(khxpi, 0.2e1);
t5 = pow(khxpr, 0.2e1);
t7 = 0.1e1 / (t4 + t5);
kappar = (t1 + t2) * t7;
t8 = khxi * khxpr;
t9 = khxr * khxpi;
kappai = (t8 - t9) * t7;
t11 = 0.1e1 / mu1;
t12 = mu2 * t11;
t13 = kxi * kxpi;
t15 = kxpr * kxr;
t17 = kxi * kxpr;
t19 = kxpi * kxr;
t21 = khxi * khypi;
t22 = kxi * kypi;
t24 = kxr * kypr;
t26 = khxi * khypr;
t27 = kxi * kypr;
t29 = kxr * kypi;
t33 = khxpi * khyi;
t34 = kxpi * kyi;
t36 = kxpr * kyr;
t38 = khxpi * khyr;
t39 = kxpi * kyr;
t41 = kxpr * kyi;
t45 = t1 * t13 + t1 * t15 + t2 * t13 + t2 * t15 - t8 * t17 + t9 * t17 + t8 * t19 - t9 * t19 + t21 * t22 + t21 * t24 - t26 * t27 + t26 * t29 + t33 * t34 + t33 * t36 - t38 * t39 + t38 * t41;
t46 = khxpr * khyi;
t49 = khxpr * khyr;
t52 = khxr * khypi;
t55 = khxr * khypr;
t58 = khyi * khypi;
t59 = kyi * kypi;
t61 = kypr * kyr;
t63 = khyi * khypr;
t64 = kyi * kypr;
t66 = kypi * kyr;
t68 = khypi * khyr;
t71 = khypr * khyr;
t74 = t55 * t22 + t55 * t24 + t52 * t27 - t52 * t29 + t49 * t34 + t49 * t36 + t46 * t39 - t46 * t41 + t58 * t59 + t58 * t61 + t71 * t59 + t71 * t61 - t63 * t64 + t63 * t66 + t68 * t64 - t68 * t66;
t76 = pow(kxpi, 0.2e1);
t78 = pow(kxpr, 0.2e1);
t80 = khxpi * khypi;
t81 = kxpi * kypi;
t84 = kxpr * kypr;
t87 = khxpi * khypr;
t88 = kxpi * kypr;
t91 = kxpr * kypi;
t96 = khxpr * khypi;
t101 = khxpr * khypr;
t106 = pow(khypi, 0.2e1);
t107 = pow(kypi, 0.2e1);
t109 = pow(kypr, 0.2e1);
t111 = pow(khypr, 0.2e1);
t114 = 0.2e1 * t101 * t81 + 0.2e1 * t101 * t84 + t106 * t107 + t106 * t109 + t111 * t107 + t111 * t109 + t4 * t76 + t4 * t78 + t5 * t76 + t5 * t78 + 0.2e1 * t80 * t81 + 0.2e1 * t80 * t84 - 0.2e1 * t87 * t88 + 0.2e1 * t87 * t91 + 0.2e1 * t96 * t88 - 0.2e1 * t96 * t91;
t115 = 0.1e1 / t114;
betar = t12 * (t45 + t74) * t115;
t133 = t1 * t17 - t1 * t19 + t8 * t13 - t9 * t13 + t8 * t15 - t9 * t15 + t2 * t17 - t2 * t19 + t21 * t27 - t21 * t29 + t26 * t22 + t26 * t24 - t33 * t39 + t33 * t41 - t38 * t34 - t38 * t36;
t150 = -t52 * t22 - t52 * t24 + t55 * t27 - t55 * t29 + t46 * t34 + t46 * t36 - t49 * t39 + t49 * t41 + t58 * t64 - t58 * t66 + t63 * t59 - t68 * t59 + t63 * t61 - t68 * t61 + t71 * t64 - t71 * t66;
betai = t12 * (t133 + t150) * t115;
t153 = pow(betai, 0.2e1);
t154 = pow(betar, 0.2e1);
t155 = pow(kappai, 0.2e1);
t156 = pow(kappar, 0.2e1);
t163 = 0.1e1 / (0.2e1 * kappai * betai + 0.2e1 * kappar * betar + t153 + t154 + t155 + t156);
rr = (t153 + t154 - t155 - t156) * t163;
ri = 0.2e1 * (betai * kappar - betar * kappai) * t163;
taur = 0.2e1 * (betar * t155 + betar * t156 + t153 * kappar + t154 * kappar) * t163;
taui = 0.2e1 * (betai * t155 + betai * t156 + kappai * t153 + t154 * kappai) * t163;
t180 = x * kxi;
t181 = y * kyi;
t182 = t * sr;
t184 = exp(t180 - t181 + t182);
t185 = x * kxr;
t186 = y * kyr;
t187 = t * si;
t188 = t185 - t186 - t187;
t189 = sin(t188);
t190 = t184 * t189;
t191 = khyi * rr;
t193 = khyr * ri;
t195 = cos(t188);
t196 = t184 * t195;
t197 = khyi * ri;
t199 = khyr * rr;
t202 = exp(-t180 - t181 + t182);
t203 = t185 + t186 + t187;
t204 = cos(t203);
t205 = t202 * t204;
t207 = sin(t203);
t208 = t202 * t207;
Exr = -a * (-t208 * khyi + t205 * khyr - t190 * t191 - t190 * t193 + t196 * t197 - t196 * t199);
t212 = khxi * rr;
t214 = khxr * ri;
t216 = khxi * ri;
t218 = khxr * rr;
Eyr = a * (-t208 * khxi + t205 * khxr + t190 * t212 + t190 * t214 - t196 * t216 + t196 * t218);
t226 = exp(-x * kxpi - y * kypi + t182);
t229 = x * kxpr + y * kypr + t187;
t230 = cos(t229);
t231 = t226 * t230;
t232 = khypi * taui;
t234 = khypr * taur;
t236 = sin(t229);
t237 = t226 * t236;
t238 = khypi * taur;
t240 = khypr * taui;
Expr = -a * (-t231 * t232 + t231 * t234 - t237 * t238 - t237 * t240);
t244 = khxpi * taui;
t246 = khxpr * taur;
t248 = khxpi * taur;
t250 = khxpr * taui;
Eypr = a * (-t231 * t244 + t231 * t246 - t237 * t248 - t237 * t250);
Exi = -a * (t205 * khyi + t208 * khyr - t190 * t197 + t190 * t199 - t196 * t191 - t196 * t193);
Eyi = a * (t205 * khxi + t208 * khxr + t190 * t216 - t190 * t218 + t196 * t212 + t196 * t214);
Expi = -a * (t231 * t238 + t231 * t240 - t237 * t232 + t237 * t234);
Eypi = a * (t231 * t248 + t231 * t250 - t237 * t244 + t237 * t246);
t279 = t11 * a;
t280 = t190 * khxi;
t281 = kxi * ri;
t282 = t281 * si;
t284 = kxi * rr;
t285 = t284 * sr;
t287 = kxr * ri;
t288 = t287 * sr;
t290 = kxr * rr;
t291 = t290 * si;
t293 = t190 * khxr;
t294 = t281 * sr;
t296 = t284 * si;
t298 = t287 * si;
t300 = t290 * sr;
t302 = t190 * khyi;
t303 = kyi * ri;
t304 = t303 * si;
t306 = kyi * rr;
t307 = t306 * sr;
t309 = kyr * ri;
t310 = t309 * sr;
t312 = kyr * rr;
t313 = t312 * si;
t315 = -t280 * t282 - t280 * t285 - t280 * t288 + t280 * t291 - t293 * t294 + t293 * t296 + t293 * t298 + t293 * t300 - t302 * t304 - t302 * t307 - t302 * t310 + t302 * t313;
t316 = t190 * khyr;
t317 = t303 * sr;
t319 = t306 * si;
t321 = t309 * si;
t323 = t312 * sr;
t325 = t196 * khxi;
t330 = t196 * khxr;
t335 = -t330 * t282 - t330 * t285 - t330 * t288 + t330 * t291 + t325 * t294 - t325 * t296 - t325 * t298 - t325 * t300 - t316 * t317 + t316 * t319 + t316 * t321 + t316 * t323;
t337 = t196 * khyi;
t342 = t196 * khyr;
t347 = khxi * kxi;
t348 = t347 * si;
t350 = khxi * kxr;
t351 = t350 * sr;
t353 = khxr * kxi;
t354 = t353 * sr;
t356 = khxr * kxr;
t357 = t356 * si;
t359 = t205 * t348 + t205 * t351 + t205 * t354 - t205 * t357 - t342 * t304 - t342 * t307 - t342 * t310 + t342 * t313 + t337 * t317 - t337 * t319 - t337 * t321 - t337 * t323;
t360 = khyi * kyi;
t361 = t360 * si;
t363 = khyi * kyr;
t364 = t363 * sr;
t366 = khyr * kyi;
t367 = t366 * sr;
t369 = khyr * kyr;
t370 = t369 * si;
t372 = t347 * sr;
t374 = t350 * si;
t376 = t353 * si;
t378 = t356 * sr;
t380 = t360 * sr;
t382 = t363 * si;
t384 = t366 * si;
t386 = t369 * sr;
t388 = t205 * t361 + t205 * t364 + t205 * t367 - t205 * t370 - t208 * t372 + t208 * t374 + t208 * t376 + t208 * t378 - t208 * t380 + t208 * t382 + t208 * t384 + t208 * t386;
t391 = pow(si, 0.2e1);
t392 = pow(sr, 0.2e1);
t394 = 0.1e1 / (t391 + t392);
Hzr = t279 * (t315 + t335 + t359 + t388) * t394;
t397 = 0.1e1 / mu2 * a;
t398 = t236 * khypr;
t399 = kypi * si;
t400 = t399 * taur;
t402 = kypi * sr;
t403 = t402 * taui;
t405 = kypr * si;
t406 = t405 * taui;
t408 = kypr * sr;
t409 = t408 * taur;
t411 = t230 * khxpi;
t412 = kxpi * si;
t413 = t412 * taur;
t415 = kxpi * sr;
t416 = t415 * taui;
t418 = kxpr * si;
t419 = t418 * taui;
t421 = kxpr * sr;
t422 = t421 * taur;
t424 = t230 * khxpr;
t425 = t412 * taui;
t427 = t415 * taur;
t429 = t418 * taur;
t431 = t421 * taui;
t433 = t230 * khypi;
t438 = t398 * t400 - t398 * t403 + t398 * t406 + t398 * t409 + t433 * t400 - t433 * t403 + t433 * t406 + t433 * t409 + t411 * t413 - t411 * t416 + t411 * t419 + t411 * t422 + t424 * t425 + t424 * t427 - t424 * t429 + t424 * t431;
t439 = t230 * khypr;
t440 = t399 * taui;
t442 = t402 * taur;
t444 = t405 * taur;
t446 = t408 * taui;
t448 = t236 * khxpi;
t453 = t236 * khxpr;
t458 = t236 * khypi;
t463 = t453 * t413 - t453 * t416 + t453 * t419 + t453 * t422 - t448 * t425 - t448 * t427 + t448 * t429 - t448 * t431 + t439 * t440 + t439 * t442 - t439 * t444 + t439 * t446 - t458 * t440 - t458 * t442 + t458 * t444 - t458 * t446;
Hzpr = t397 * t226 * (t438 + t463) * t394;
t479 = -t280 * t294 + t280 * t296 + t280 * t298 + t280 * t300 + t293 * t282 + t293 * t285 + t293 * t288 - t293 * t291 - t302 * t317 + t302 * t319 + t302 * t321 + t302 * t323;
t492 = -t325 * t282 - t325 * t285 - t325 * t288 + t325 * t291 - t330 * t294 + t330 * t296 + t330 * t298 + t330 * t300 + t316 * t304 + t316 * t307 + t316 * t310 - t316 * t313;
t506 = t205 * t372 - t205 * t374 - t205 * t376 - t205 * t378 - t337 * t304 - t337 * t307 - t337 * t310 + t337 * t313 - t342 * t317 + t342 * t319 + t342 * t321 + t342 * t323;
t519 = t205 * t380 - t205 * t382 - t205 * t384 - t205 * t386 + t208 * t348 + t208 * t351 + t208 * t354 - t208 * t357 + t208 * t361 + t208 * t364 + t208 * t367 - t208 * t370;
Hzi = t279 * (t479 + t492 + t506 + t519) * t394;
t539 = t398 * t440 + t398 * t442 - t398 * t444 + t398 * t446 + t458 * t400 - t458 * t403 + t458 * t406 + t458 * t409 + t411 * t425 + t411 * t427 + t448 * t419 + t448 * t422 + t453 * t425 + t453 * t427 - t453 * t429 + t453 * t431;
t556 = -t439 * t400 + t439 * t403 - t439 * t406 - t439 * t409 - t411 * t429 + t411 * t431 - t424 * t413 + t448 * t413 + t424 * t416 - t448 * t416 - t424 * t419 - t424 * t422 + t433 * t440 + t433 * t442 - t433 * t444 + t433 * t446;
Hzpi = t397 * t226 * (t539 + t556) * t394;


                    if( false )
                    {
                        printF("(i1,i2)=(%3i,%3i): psiSum1=(%9.3e,%9.3e) psiSum2=(%9.3e,%9.3e) r=(%8.2e,%8.2e) "
                                      "tau=(%8.2e,%8.2e) khy=(%8.2e,%8.2e) khpy=(%8.2e,%8.2e): ",
                                      i1,i2,psiSum1r,psiSum1i,psiSum2r,psiSum2i,rr,ri,taur,taui,khyr,khyi,khypr,khypi);
                        real eps1Hatr = eps1*(1+psiSum1r), eps1Hati=eps1*(psiSum1i);
                        real eps2Hatr = eps2*(1+psiSum2r), eps2Hati=eps2*(psiSum2i);


                        checkPlaneMaterialInterfaceJumps( 
                            c1,c2,eps1,eps2,mu1,mu2, sr,si, rr,ri, taur,taui, 
                            eps1Hatr,eps1Hati, eps2Hatr,eps2Hati,
                            psiSum1r,psiSum1i,psiSum2r,psiSum2i,
                            kxr,kxi, kyr,kyi, kxpr,kxpi, kypr,kypi );
                        
                        OV_ABORT("stop here for now");
                        
                    }
                    
                    if( false )
                    {
                        printF("(i1,i2)=(%3i,%3i): kNorm=%g, kpNorm=%g, kappa=(%g,%g) beta=(%g,%g)\n",
                                      i1,i2,kNorm,kpNorm,kappar,kappai,betar,betai);
                        printF("    : eps1=%g, eps2=%g, r=(%g,%g) tau=(%g,%g) \n",eps1,eps2,rr,ri,taur,taui);
            // printF("    : psiSum1=(%g,%g) psiSum2=(%g,%g) \n",psiSum1r,psiSum1i,psiSum2r,psiSum2i);
                        printF("    : Exr=%g, Eyr=%g, Exi=%g, Eyi=%g Hzr=%g Hzi=%g\n",Exr,Eyr,Exi,Eyi,Hzr,Hzi);
                        printF("    : Expr=%g, Eypr=%g, Expi=%g, Eypi=%g Hzpr=%g Hzpi=%g\n",Expr,Eypr,Expi,Eypi,Hzpr,Hzpi);

                        OV_ABORT("finish me");
                    }
                    

                    if( myDomain==0 )
                    {
                        uLocal(i1,i2,i3,ex) = Exr;
                        uLocal(i1,i2,i3,ey) = Eyr;
                        uLocal(i1,i2,i3,hz) = Hzr;

                        for( int iv=0; iv<numberOfPolarizationVectors1; iv++ )
                        {
                            const int pc= iv*numberOfDimensions;
                            pLocal(i1,i2,i3,pc  ) = psi1r[iv]*Exr - psi1i[iv]*Exi;
                            pLocal(i1,i2,i3,pc+1) = psi1r[iv]*Eyr - psi1i[iv]*Eyi;
                        }

                    }
                    else
                    {
                        uLocal(i1,i2,i3,ex) = Expr;
                        uLocal(i1,i2,i3,ey) = Eypr;
                        uLocal(i1,i2,i3,hz) = Hzpr;
                        for( int iv=0; iv<numberOfPolarizationVectors2; iv++ )
                        {
                            const int pc= iv*numberOfDimensions;
                            pLocal(i1,i2,i3,pc  ) = psi2r[iv]*Expr - psi2i[iv]*Expi;
                            pLocal(i1,i2,i3,pc+1) = psi2r[iv]*Eypr - psi2i[iv]*Eypi;
                        }
                    }
                    
          // // -- polarization vector --
          // uLocal(i1,i2,i3,pxc) = (pc*cosxi+ps*sinxi)*pwc[0]*dpwExp;
          // uLocal(i1,i2,i3,pyc) = (pc*cosxi+ps*sinxi)*pwc[1]*dpwExp;

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
            "dispersive plane wave interface",
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
        
    // replaced: 
    // else if( answer=="dispersive plane wave" ) 
    // {
    //   userKnownSolution="dispersivePlaneWave";
    //   dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution depends on time
            

    //   printF("The dispersive plane wave is defined by: \n"
    //          "    E = a*sin( k*x - omegar*t)*exp(omegai*t)\n"
    //          "    P = [b*cos( k*x - omegar*t) + c*sin( k*x - omegar*t) ]*exp(omegai*t)\n"
    //         );
            
            
    // }
        else if( answer=="dispersive plane wave interface" ) 
        {
            userKnownSolution="dispersivePlaneWaveInterface";
            dbase.get<bool>("knownSolutionIsTimeDependent")=true;  // known solution depends on time
            

            printF("The dispersive plane wave interface defines an exact solution for a plane wave\n"
                          " hitting a planar material interface between two dispersive materials");
            
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


// Include complex down here to minimize name conflicts
#include <complex>

typedef ::real OV_real;
// =====================================================================================
/// \brief Utility routine to do some complex arithemetic for the dispersive plane
///    wave material interface.
///
///  Compute kxp=(kxpr,kxpi)  given (kr,ki), and kyp=(kypr,kypi)
///  kxp^2 + kyp^2 = (kr+I*ki)^2 = (kr^2-ki^2) + 2*I*kr*ki 
///  kxp = kxpr + I*kpri = sqrt( (kr+I*ki)^2 - kyp^2 )
// =====================================================================================
void
getTransmisionWaveNumber( const LocalReal & kr,  const LocalReal & ki, 
                                                    const LocalReal & kxr, const LocalReal & kxi, 
                                                    const LocalReal & kyr, const LocalReal & kyi, 
                                                    LocalReal & kxpr, LocalReal & kxpi, 
                                                    LocalReal & kypr, LocalReal & kypi )
{
  // No jump in tangential field: kyp=ky : 
    kypr=kyr;
    kypi=kyi;

  // std::complex<LocalReal> I(0.0,1.0); 
    std::complex<LocalReal> ky(kyr,kyi);
    std::complex<LocalReal> k(kr,ki);
    std::complex<LocalReal> kxp,kyp(kypr,kypi);

  // cout << "kyp=" << kyp << endl;

    kxp = std::sqrt( k*k - kyp*kyp );

    kxpr= std::real(kxp);
    kxpi= std::imag(kxp);
    
  // printF("--getTransmisionWaveNumber--- kx=(%g,%g) ky=(%g,%g) (kr,ki)=(%g,%g) kxp=(%g,%g) kyp=(%g,%g)\n",
  //            kxr,kxi,kyr,kyi,kr,ki,kxpr,kxpi,kypr,kypi);

}


// ---------------------------------------------------------------------------------------
// Check routine : 
//   Check the jump:
//       eps1Hat*khyat*(1-r) = eps2Hat*kyHatp*tau
// ---------------------------------------------------------------------------------------
void
checkPlaneMaterialInterfaceJumps( 
                                                    const LocalReal & c1, const LocalReal & c2,
                                                    const LocalReal & eps1, const LocalReal & eps2,
                                                    const LocalReal & mu1, const LocalReal & mu2,

                                                    const LocalReal & sr, const LocalReal & si,
                                                    const LocalReal & rr, const LocalReal & ri, 
                                                    const LocalReal & taur, const LocalReal & taui, 

                                                    const LocalReal & eps1Hatr, const LocalReal & eps1Hati,
                                                    const LocalReal & eps2Hatr, const LocalReal & eps2Hati,

                                                    const LocalReal & psiSum1r, const LocalReal & psiSum1i,
                                                    const LocalReal & psiSum2r, const LocalReal & psiSum2i,
                                                    const LocalReal & kxr, const LocalReal & kxi,
                                                    const LocalReal & kyr, const LocalReal & kyi,
                                                    const LocalReal & kxpr, const LocalReal & kxpi,
                                                    const LocalReal & kypr, const LocalReal & kypi

                                                        )
{

  // std::complex<LocalReal> I(0.0,1.0); 
    std::complex<LocalReal> psiSum1(psiSum1r,psiSum1i);
    std::complex<LocalReal> psiSum2(psiSum2r,psiSum2i);

    std::complex<LocalReal> eps1Hat(eps1Hatr,eps1Hati);
    std::complex<LocalReal> eps2Hat(eps2Hatr,eps2Hati);
    std::complex<LocalReal> s(sr,si);
    std::complex<LocalReal> kx(kxr,kxi);
    std::complex<LocalReal> ky(kyr,kyi);
    std::complex<LocalReal> kxp(kxpr,kxpi);
    std::complex<LocalReal> kyp(kypr,kypi);
    std::complex<LocalReal> khx,khy, khpx,khpy;
    std::complex<LocalReal> r(rr,ri), tau(taur,taui);
    std::complex<LocalReal> dr1,dr2,jump;
    
    LocalReal kNorm = sqrt(kxr*kxr + kxi*kxi + kyr*kyr + kyi*kyi);
    khx= (kxr + 1i*kxi)/kNorm;
    khy= (kyr + 1i*kyi)/kNorm;
    
    LocalReal kpNorm = sqrt(kxpr*kxpr + kxpi*kxpi + kypr*kypr + kypi*kypi);
    khpx=(kxpr + 1i*kxpi)/kpNorm;
    khpy=(kypr + 1i*kypi)/kpNorm;
    
    printF("\n\n ** s=(%g,%g) kx=(%g,%g) ky=(%g,%g) c1=%g eps1=%g mu1=%g \n",sr,si,kxr,kxi,kyr,kyi,c1,eps1,mu1);
    

    jump = khx*(1.+r) - tau*khpx;
    printF("khx*(1.+r) - tau*khpx                 =(%12.4e,%12.4e)\n",std::real(jump), std::imag(jump));
    
    jump =eps1Hat*khy*(1.-r) - eps2Hat*tau*khpy;
    printF(" [epsHat khy(1-r)- epsHat*tau*khy'    =(%12.4e,%12.4e)\n",std::real(jump), std::imag(jump));
    
  // dispersion relations: 
    dr1 = s*s + c1*c1*(kx*kx  +ky*ky  ) + s*s*psiSum1;
    dr2 = s*s + c2*c2*(kxp*kxp+kyp*kyp) + s*s*psiSum2;

    printF(" dispersion-relation1                 =(%12.4e,%12.4e)\n",std::real(dr1), std::imag(dr1));
    printF(" dispersion-relation2                 =(%12.4e,%12.4e)\n",std::real(dr2), std::imag(dr2));

    jump = (1.-r)*( ky*khy + kx*khx )/mu1 - tau*( kyp*khpy+kxp*khpx )/mu2;
    printF(" (1-r)*( kSq )/mu1 - tau*( kpSq )/mu2'=(%12.4e,%12.4e)\n",std::real(jump), std::imag(jump));

    jump = (1.-r)*( ky*ky + kx*kx )/(kNorm*mu1) - tau*( kyp*kyp+kxp*kxp )/(kpNorm*mu2);
    printF(" (1-r)*( kSq )/mu1-tau*( kpSq )/mu2'  =(%12.4e,%12.4e)\n",std::real(jump), std::imag(jump));

    jump = (1.-r)*( s*s*(1.+psiSum1)/(c1*c1) )/(kNorm*mu1) - tau*( s*s*(1.+psiSum2)/(c2*c2) )/(kpNorm*mu2);
    printF(" (1-r)*( kSq )/mu1-tau*( kpSq )/mu2'  =(%12.4e,%12.4e)\n",std::real(jump), std::imag(jump));

    jump = (1.-r)*( s*s*(1.+psiSum1)*(mu1*eps1) )/(kNorm*mu1) - tau*( s*s*(1.+psiSum2)*(mu2*eps2) )/(kpNorm*mu2);
    printF(" (1-r)*( kSq )/mu1-tau*( kpSq )/mu2'  =(%12.4e,%12.4e)\n",std::real(jump), std::imag(jump));

    jump = (1.-r)*( (1.+psiSum1)*(mu1*eps1) )/(kNorm*mu1) - tau*( (1.+psiSum2)*(mu2*eps2) )/(kpNorm*mu2);
    printF(" (1-r)*( kSq )/mu1-tau*( kpSq )/mu2'  =(%12.4e,%12.4e)\n",std::real(jump), std::imag(jump));

    jump = (1.-r)*( (1.+psiSum1)*(mu1*eps1) )*khy/(mu1) - tau*( (1.+psiSum2)*(mu2*eps2) )*khpy/(mu2);
    printF(" (1-r)*( kSq )/mu1-tau*( kpSq )/mu2'  =(%12.4e,%12.4e)\n",std::real(jump), std::imag(jump));

    jump = (1.-r)*( (1.+psiSum1)*(eps1) )*khy - tau*( (1.+psiSum2)*(eps2) )*khpy;
    printF(" (1-r)*( kSq )/mu1-tau*( kpSq )/mu2'  =(%12.4e,%12.4e)\n",std::real(jump), std::imag(jump));

}
