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
    real reS, imS;
    dmp.computeDispersionRelation( c,eps,mu,kk, reS, imS );
    real expS = exp(reS*t), expSm=exp(reS*(t-dt));

    imS=-imS;  // flip sign    **** FIX ME ****

    printF("--IC-- scatCyl imS=%g, Im(s)/(twoPi*cc0)=%g reS=%g\n",imS,imS/twoPi*cc0,reS);
  
    cost = cos( imS*t )*expS;      // "cos(t)" for dispersive model 
    sint = sin( imS*t )*expS;
  
    costm = cos( imS*(t-dt) )*expS;
    sintm = sin( imS*(t-dt) )*expS;
  
    dcost = -imS*sint + reS*cost;  //  d/dt of "cost" 
    dsint =  imS*cost + reS*sint;  //  d/dt of "cost" 
  	    
    real alpha=reS, beta=imS;  // s= alpha + i*beta (
    real a,b;   // psi = a + i*b 
  
    // P = Im{ psi(s)*E } = Im{ (a+i*b)*( Er + i*Ei)(cos(beta*t)+i*sin(beta*t))*exp(alpha*t) }
  
    const real gamma=dmp.gamma, omegap=dmp.omegap;
    const real cp = eps* omegap*omegap;
    
    const real denom = (SQR(alpha)+SQR(beta))*( SQR((alpha+gamma)) + SQR(beta) );
    a =  cp* (alpha*(alpha+gamma)-beta*beta)/denom;   
    b = -cp* beta*(2.*alpha+gamma)/denom;
  
  
    phiPc = a*cost-b*sint;
    phiPs = a*sint+b*cost;
  	    
    phiPcm = a*costm-b*sintm;
    phiPsm = a*sintm+b*costm;
  	    
  }
#endMacro
