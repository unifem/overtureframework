// -- dispersive plane wave solution
//        w = wr + i wi   (complex dispersion relation)
// You should define: 
//    dpwExp := exp( wi* t )
#define exDpw(x,y,t,dpwExp) sin(twoPi*(kx*(x)+ky*(y))-omegaDpwRe*(t))*(pwc[0]*(dpwExp))
#define eyDpw(x,y,t,dpwExp) sin(twoPi*(kx*(x)+ky*(y))-omegaDpwRe*(t))*(pwc[1]*(dpwExp))
#define hzDpw(x,y,t,dpwExp) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[5]

// ** FIX ME -- THIS IS WRONG
// #define extDpw(x,y,t,dpwExp) (-twoPi*omegaDpwRe)*cos(twoPi*(kx*(x)+ky*(y)-omegaDpwRe*(t)))*(pwc[0]*(dpwExp))
// #define eytDpw(x,y,t,dpwExp) (-twoPi*omegaDpwRe)*cos(twoPi*(kx*(x)+ky*(y)-omegaDpwRe*(t)))*(pwc[1]*(dpwExp))
// #define hztDpw(x,y,t,dpwExp) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[5]

// ====================================================================================
/// Macro: Return the time-dependent coefficients for a known solution
// 
// NOTE: This next section is repeated in getInitialConditions.bC,
//        getErrors.bC and assignBoundaryConditions.bC 
// ====================================================================================
#beginMacro getKnownSolutionTimeCoefficients();
  real cost,sint,costm,sintm,dcost,dsint;
  real phiPc,phiPs, phiPcm,phiPsm;
  	  
  if( dispersionModel==noDispersion )
  {
    cost = cos(-twoPi*cc0*t); // *wdh* 040626 add "-"
    sint = sin(-twoPi*cc0*t); // *wdh* 040626 add "-"
  
    costm= cos(-twoPi*cc0*(t-dt)); // *wdh* 040626 add "-"
    sintm= sin(-twoPi*cc0*(t-dt)); // *wdh* 040626 add "-"
  
    dcost =  twoPi*cc0*sint;  // d(sin(..))/dt 
    dsint = -twoPi*cc0*cost;  // d(sin(..))/dt 
  }
  else
  {
    // -- dispersive model -- *CHECK ME*
  
    // Evaluate the dispersion relation for "s"
    DispersiveMaterialParameters & dmp = getDispersiveMaterialParameters(grid);
    const real kk = twoPi*cc0;  // Parameter in dispersion relation **check me**
    // *new way*
    real sr,si,psir,psii;
    dmp.evaluateDispersionRelation( c,kk, sr, si, psir,psii ); 


    // real reS, imS;
    // dmp.computeDispersionRelation( c,eps,mu,kk, reS, imS );
    // real expS = exp(reS*t), expSm=exp(reS*(t-dt));

    // si=-si;  // flip sign    **** FIX ME ****

    if( t<=3.*dt ) 
    {
      printF("--MX--GIC dispersion: s=(%12.4e,%12.4e) psi=(%12.4e,%12.4e)\n",sr,si,psir,psii);
      printF("--MX--GIC scatCyl si/(twoPi*cc0)=%g\n",si/twoPi*cc0);
    }
    
    // Re( (Er+i*Ei)*( ct + i*st ) )
    //   ct*Er - st*Ei 
    const real tm=t-dt;
    real expt=exp(sr*t), exptm=exp(sr*tm);
    real ct =cos( si*t  )*expt,  st =sin( si*t )*expt;
    real ctm=cos( si*tm )*exptm, stm=sin( si*tm )*exptm;
    
    cost =  ct;      // Coeff of Ei
    sint =  st;     // Coeff of Er
  
    costm =  ctm;
    sintm =  stm;
  
    dcost =  -si*st + sr*ct;  //  d/dt of "cost" 
    dsint =  (si*ct + sr*st);  //  d/dt of "cost" 

    // real alpha=reS, beta=imS;  // s= alpha + i*beta (
    // real a,b;   // psi = a + i*b 
  
    // P = Re{ psi(s)*E } = Re{ (psir+i*psi)*( Er + i*Ei)( ct+i*st ) }
  
  
    phiPc =  psir*cost-psii*sint;  // Coeff of Er 
    phiPs = -psir*sint-psii*cost;  // coeff of Ei
  	    
    phiPcm =  psir*costm-psii*sintm;
    phiPsm = -psir*sintm-psii*costm;

      // *** TEST ****
    if( true )
    {
      sint = ct;     // Coeff of Er    
      cost = -st;     // Coeff of Ei

  
      sintm = ctm;
      costm =-stm;
  
      dsint =  -si*st + sr*ct;  //  d/dt of "cost" 
      dcost =  -(si*ct + sr*st);  //  d/dt of "cost" 

    }
    
	    
    // *** TEST ****
    if( false )
    {
      cost = cos(-twoPi*cc0*t); // *wdh* 040626 add "-"
      sint = sin(-twoPi*cc0*t); // *wdh* 040626 add "-"


      costm= cos(-twoPi*cc0*(t-dt)); // *wdh* 040626 add "-"
      sintm= sin(-twoPi*cc0*(t-dt)); // *wdh* 040626 add "-"
  
      dcost =  twoPi*cc0*sint;  // d(sin(..))/dt 
      dsint = -twoPi*cc0*cost;  // d(sin(..))/dt 

      printF("--MX--GIC (cost,ct)=(%12.4e,%12.4e) (sint,st)=(%12.4e,%12.4e)\n",cost,ct,sint,st);
      printF("--MX--GIC (costm,ctm)=(%12.4e,%12.4e) (sintm,stm)=(%12.4e,%12.4e)\n",costm,ctm,sintm,stm);

      phiPc =  0.;
      phiPs =  0.;
  	    
      phiPcm = 0.;
      phiPsm = 0.;
    }
    
  }
#endMacro
