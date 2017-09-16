// ======================================================================================
// Macro: getBoxEigenfunctionCoefficients
//     Compute the coefficients for the eigenfunctions of a square or box 
// ======================================================================================
#beginMacro getBoxEigenfunctionCoefficients( tE, tH, omega )
  if( dispersionModel==noDispersion )
  {
    // --- non-dispersive ----
    if( numberOfDimensions==2 )
    {
      phiEx=(-fy/(omega))*sin((omega)*(tE)); phiExt= (-fy/(omega))*(omega)*cos((omega)*(tE));
      phiEy=( fx/(omega))*sin((omega)*(tE)); phiEyt= ( fx/(omega))*(omega)*cos((omega)*(tE));

      phiHz=cos((omega)*(tH)); phiHzt=-(omega)*sin((omega)*(tH));
    }
    else
    {
      phiEx=(a1/fx)*cos((omega)*(tE)); phiExt= -(a1/fx)*(omega)*sin((omega)*(tE));
      phiEy=(a2/fy)*cos((omega)*(tE)); phiEyt= -(a2/fy)*(omega)*sin((omega)*(tE));
      phiEz=(a3/fz)*cos((omega)*(tE)); phiEzt= -(a3/fz)*(omega)*sin((omega)*(tE));

    }
    
  }
  else
  {
    // --- dispersive model ---
    // ************* See discussion in DMX-ADE_notes.pdf  ********************

    DispersiveMaterialParameters & dmp = getDispersiveMaterialParameters(grid);

    // Evaluate the dispersion relation for "s"
    const real kk = omega/c;  // Parameter in dispersion relation **check me**
    // real reS, imS;
    // old dmp.computeDispersionRelation( c,eps,mu,kk, reS, imS );
    // *new way*
    real sr,si,psir[10],psii[10];
    assert( dmp.numberOfPolarizationVectors<10 );
    dmp.evaluateDispersionRelation( c,kk, sr, si, psir,psii ); 

    if( t<3.*dt )
      printF("--IC:SQ-Eig-- (dispersive) t=%10.3e, sr=%g, si=%g a1=%g a2=%g\n",t,sr,si,a1,a2 );

    // s = a + i b 
    // Time-factor = sin(b*tE)*exp(a*tE) 
    const real a = sr, b=si, a2pb2=a*a+b*b;  // s = a + i b 

    real expE =exp(a*(tE)); // decay part of time dependence
    real expH =exp(a*(tH)); // decay part of time dependence

    real ste=sin(b*tE)*expE , cte=cos(b*tE)*expE ;
    real sth=sin(b*tH)*expH, cth=cos(b*tH)*expH;
    
    if( numberOfDimensions==2 )
    {
      real scale= sqrt(fx*fx+fy*fy);
      real a1s= scale*a1/fx, a2s=scale*a2/fy;

      phiEx = a1s*(   ste ); 
      phiExt= a1s*( b*cte ) + a*phiEx; // time-deriv for sosup scheme 

      phiEy = a2s*(   ste ); 
      phiEyt= a2s*( b*cte ) + a*phiEy;

      // mu * (Hz)_t = -(Ey)_x + (Ex)_y
      // **** CHECK ME ****
      const real amph  = -(1./mu)*(a2s*fx-a1s*fy)*( a*  sth - b*  cth )/a2pb2;
      const real ampht = -(1./mu)*(a2s*fx-a1s*fy)*( a*b*cth + b*b*sth )/a2pb2;

      phiHz  =  amph; 
      phiHzt =  ampht + a*amph;

      // P = Chi * E = omegap^2/( s*(s+gamma) )* E 
      //  Chi = omegap^2/( s*(s+gamma) )
      //  Chi = omegap^2*[  sBar*(sBar+gamma)/( |s|^2 * |s+gamma|^2 ) ]
      //      = omegap^2*[ (a-i*b)*( a+gamma - i*b)/( |s|^2 * |s+gamma|^2 ) ]
      //      = omegap^2*[ (a)(a+gamma) -b^2 + i*( -a*b - b*(a+gamma) )/( |s|^2 * |s+gamma|^2 ) ]
      // NOTE:    E = Im( Ehat(x,y)*exp( s*t ) )
      // const real gamma=dmp.gamma, omegap=dmp.omegap;
      
      // const real denom = (SQR(a)+SQR(b))*( SQR((a+gamma)) + SQR(b) );
      // real reChi =  omegap*omegap* (a*(a+gamma)-b*b)/denom;   
      // real imChi = -omegap*omegap* b*(2*a+gamma)/denom;
      // printF("--BOXEIG-- psir=%e psii=%e reCh=%e imChi=%e\n",psir,psii,reChi,imChi);
      

      // phiP = Im(  Chi*( cos(beta*t) + i*sin(beta*t) )*exp(alpha*t )  ... s= alpha+i*beta
      for( int iv=0; iv<numberOfPolarizationVectors; iv++ )
      {
        real phiP = psir[iv]*ste+ psii[iv]*cte;
        phiPx[iv] = a1s*( phiP );
        phiPy[iv] = a2s*( phiP );
      }
      
    }
    else
    {
      // ---- THREE DIMENSIONS ---
      real scale= sqrt(fx*fx+fy*fy+fz*fz);
      real a1s= scale*a1/fx, a2s=scale*a2/fy, a3s=scale*a3/fz;

      phiEx = a1s*(   ste ); 
      phiExt= a1s*( b*cte ) + a*phiEx ;

      phiEy = a2s*(   ste ); 
      phiEyt= a2s*( b*cte ) + a*phiEy;

      phiEz = a3s*(   ste ); 
      phiEzt= a3s*( b*cte ) + a*phiEz;

      // *check me*
      for( int iv=0; iv<numberOfPolarizationVectors; iv++ )
      {
        real phiP = psir[iv]*ste+ psii[iv]*cte;
        phiPx[iv] = a1s*( phiP );
        phiPy[iv] = a2s*( phiP );
        phiPz[iv] = a3s*( phiP );
      }
    }
    
  }
#endMacro
