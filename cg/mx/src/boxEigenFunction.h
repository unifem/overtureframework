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
    real reS, imS;
    dmp.computeDispersionRelation( c,eps,mu,kk, reS, imS );

    if( t<3.*dt )
      printF("--IC:SQ-Eig-- (dispersive) t=%10.3e, reS=%g, imS=%g a1=%g a2=%g\n",t,reS,imS,a1,a2 );

    // s = a + i b 
    // Time-factor = sin(b*tE)*exp(a*tE) 
    const real a = reS, b=imS, a2pb2=a*a+b*b;  // s = a + i b 

    real dpwExpE =exp(a*(tE)); // decay part of time dependence
    real dpwExpH =exp(a*(tH)); // decay part of time dependence

    if( numberOfDimensions==2 )
    {
      phiEx = (a1/fx)*(   sin(b*(tE))*dpwExpE ); 
      phiExt= (a1/fx)*( b*cos(b*(tE))*dpwExpE ) + a*phiEx;

      phiEy = (a2/fy)*(   sin(b*(tE))*dpwExpE ); 
      phiEyt= (a2/fy)*( b*cos(b*(tE))*dpwExpE ) + a*phiEy;

      // mu * (Hz)_t = -(Ey)_x + (Ex)_y
      // **** CHECK ME ****
      const real amp  = -(1./mu)*(a2*fx/fy-a1*fy/fx)*( a*  sin(b*(tH)) - b*  cos(b*(tH)) )/a2pb2;
      const real ampt = -(1./mu)*(a2*fx/fy-a1*fy/fx)*( a*b*cos(b*(tH)) + b*b*sin(b*(tH)) )/a2pb2;

      phiHz  =  amp*dpwExpH; 
      phiHzt =  ampt*dpwExpH + a*phiHz;

      // P = Chi * E = omegap^2/( s*(s+gamma) )* E 
      //  Chi = omegap^2/( s*(s+gamma) )
      //  Chi = omegap^2*[  sBar*(sBar+gamma)/( |s|^2 * |s+gamma|^2 ) ]
      //      = omegap^2*[ (a-i*b)*( a+gamma - i*b)/( |s|^2 * |s+gamma|^2 ) ]
      //      = omegap^2*[ (a)(a+gamma) -b^2 + i*( -a*b - b*(a+gamma) )/( |s|^2 * |s+gamma|^2 ) ]
      // NOTE:    E = Im( Ehat(x,y)*exp( s*t ) )
      const real gamma=dmp.gamma, omegap=dmp.omegap;
      
      const real denom = (SQR(a)+SQR(b))*( SQR((a+gamma)) + SQR(b) );
      real reChi =  omegap*omegap* (a*(a+gamma)-b*b)/denom;   
      real imChi = -omegap*omegap* b*(2*a+gamma)/denom;

      // phiP = Im(  Chi*( cos(beta*t) + i*sin(beta*t) )*exp(alpha*t )  ... s= alpha+i*beta
      real phiP = reChi*sin(b*(tE))+ imChi*cos(b*(tE));
      phiPx = (a1/fx)*( phiP )*dpwExpE; 
      phiPy = (a2/fy)*( phiP )*dpwExpE; 

    }
    else
    {
      phiEx = (a1/fx)*(   sin(b*(tE))*dpwExpE ); 
      phiExt= (a1/fx)*( b*cos(b*(tE))*dpwExpE ) + a*phiEx ;

      phiEy = (a2/fy)*(   sin(b*(tE))*dpwExpE ); 
      phiEyt= (a2/fy)*( b*cos(b*(tE))*dpwExpE ) + a*phiEy;

      phiEz = (a3/fz)*(   sin(b*(tE))*dpwExpE ); 
      phiEzt= (a3/fz)*( b*cos(b*(tE))*dpwExpE ) + a*phiEz;

    }
    
  }
#endMacro
