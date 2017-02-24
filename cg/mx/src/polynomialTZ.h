
// ===============================================================================================
//  Macro: initialize3DPolyTW is a bpp macro that sets up the E and H polynomial tw function
// ===============================================================================================
#beginMacro initialize3DPolyTW(ux,uy,uz)

  // -----------------------------------------------------------------------------
  // --------------------- DEFINE POLYNOMIAL TZ SOLUTIONS ------------------------
  // -----------------------------------------------------------------------------

  // Always include linear terms in TZ if degreSpace>=1 *wdh* Sept 18, 2016 
  if( degreeSpace >=1 )
  {
    spatialCoefficientsForTZ(0,0,0,ux)=1.;      // u=1 + x + y + z
    spatialCoefficientsForTZ(1,0,0,ux)=1.;
    spatialCoefficientsForTZ(0,1,0,ux)=1.;
    spatialCoefficientsForTZ(0,0,1,ux)=1.;

    spatialCoefficientsForTZ(0,0,0,uy)= 2.;      // v=2+x-2y+z
    spatialCoefficientsForTZ(1,0,0,uy)= 1.;
    spatialCoefficientsForTZ(0,1,0,uy)=-2.;
    spatialCoefficientsForTZ(0,0,1,uy)= 1.;
    
    spatialCoefficientsForTZ(1,0,0,uz)=-1.;      // w=-x+y+z
    spatialCoefficientsForTZ(0,1,0,uz)= 1.;
    spatialCoefficientsForTZ(0,0,1,uz)= 1.;

    // eps and mu should remain positive 
    spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
    spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
    spatialCoefficientsForTZ(0,0,1,epsc)=eps*.12;  // z

    spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
    spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
    spatialCoefficientsForTZ(0,0,1,muc )=mu*.095;   // z

  }

  if( degreeSpace==2 )
  {
    spatialCoefficientsForTZ(2,0,0,ux)=1.;      // u=x^2 + 2xy + y^2 + xz  - .25*yz -.5*z^2
    spatialCoefficientsForTZ(1,1,0,ux)=2.;
    spatialCoefficientsForTZ(0,2,0,ux)=1.;
    spatialCoefficientsForTZ(1,0,1,ux)=1.;
    spatialCoefficientsForTZ(0,1,1,ux)=-.25;
    spatialCoefficientsForTZ(0,0,2,ux)=-.5;
      
    spatialCoefficientsForTZ(2,0,0,uy)= 1.;      // v=x^2 -2xy - y^2 + 3yz + .25*xz +.5*z^2
    spatialCoefficientsForTZ(1,1,0,uy)=-2.;
    spatialCoefficientsForTZ(0,2,0,uy)=-1.;
    spatialCoefficientsForTZ(0,1,1,uy)=+3.;
    spatialCoefficientsForTZ(1,0,1,uy)=.25;
    spatialCoefficientsForTZ(0,0,2,uy)=.5;
      
    spatialCoefficientsForTZ(2,0,0,uz)= 1.;      // w=x^2 + y^2 - 2 z^2 + .25*xy 
    spatialCoefficientsForTZ(0,2,0,uz)= 1.;
    spatialCoefficientsForTZ(0,0,2,uz)=-2.;
    spatialCoefficientsForTZ(1,1,0,uz)=.25;

    // eps and mu should remain positive 
    spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
    spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
    spatialCoefficientsForTZ(0,0,1,epsc)=eps*.12;  // z
    spatialCoefficientsForTZ(2,0,0,epsc)=eps*.1;   // x^2
    spatialCoefficientsForTZ(0,2,0,epsc)=eps*.15;  // y^2        
    spatialCoefficientsForTZ(0,0,2,epsc)=eps*.11;  // z^2        

    spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
    spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
    spatialCoefficientsForTZ(0,0,1,muc )=mu*.095;   // z
    spatialCoefficientsForTZ(2,0,0,muc )=mu*.125;   // x^2
    spatialCoefficientsForTZ(0,2,0,muc )=mu*.15;    // y^2
    spatialCoefficientsForTZ(0,0,2,muc )=mu*.13;    // z^2

  }
  else if( degreeSpace==0 )
  {
    spatialCoefficientsForTZ(0,0,0,ux)=1.; // -1.; 
    spatialCoefficientsForTZ(0,0,0,uy)=1.; //-.5;
    spatialCoefficientsForTZ(0,0,0,uz)=1.; //.75; 
  }
  else if( degreeSpace==3 )
  {
    spatialCoefficientsForTZ(2,0,0,ux)=1.;      // u=x^2 + 2xy + y^2 + xz 
    spatialCoefficientsForTZ(1,1,0,ux)=2.;    //        + .125( x^3 + y^3 + z^3 ) -.75*x*y^2 + x^2*z +.4yz
    spatialCoefficientsForTZ(0,2,0,ux)=1.;
    spatialCoefficientsForTZ(1,0,1,ux)=1.;
      
    spatialCoefficientsForTZ(3,0,0,ux)=.125; 
    spatialCoefficientsForTZ(0,3,0,ux)=.125; 
    spatialCoefficientsForTZ(0,0,3,ux)=.125; 
    spatialCoefficientsForTZ(1,2,0,ux)=-.75;
    spatialCoefficientsForTZ(2,0,1,ux)=+1.; 
    spatialCoefficientsForTZ(0,1,1,ux)=.4; 


    spatialCoefficientsForTZ(2,0,0,uy)= 1.;      // v=x^2 -2xy - y^2 + 3yz 
    spatialCoefficientsForTZ(1,1,0,uy)=-2.;      //    + .25( x^3 + y^3 + z^3 ) -.375*x^2 y  -.375*y*z^2  
    spatialCoefficientsForTZ(0,2,0,uy)=-1.;
    spatialCoefficientsForTZ(0,1,1,uy)=+3.;
      
    spatialCoefficientsForTZ(3,0,0,uy)=.25; 
    spatialCoefficientsForTZ(0,3,0,uy)=.25; 
    spatialCoefficientsForTZ(0,0,3,uy)=.25; 
    spatialCoefficientsForTZ(2,1,0,uy)=-3.*.125; 
    spatialCoefficientsForTZ(0,1,2,uy)=-3.*.125; 
      
      
    spatialCoefficientsForTZ(2,0,0,uz)= 1.;      // w=x^2 + y^2 - 2 z^2 
    spatialCoefficientsForTZ(0,2,0,uz)= 1.;      //      + .25x^3 -.2y^3 +.125 z^3 - x z^2 -.6*xy^2
    spatialCoefficientsForTZ(0,0,2,uz)=-2.;
      
    spatialCoefficientsForTZ(3,0,0,uz)=.25; 
    spatialCoefficientsForTZ(0,3,0,uz)=-.2; 
    spatialCoefficientsForTZ(0,0,3,uz)=.125; 
    spatialCoefficientsForTZ(1,0,2,uz)=-1.;
    spatialCoefficientsForTZ(1,2,0,uz)=-.6;

  }
  else if( degreeSpace==4 )
  {
    spatialCoefficientsForTZ(2,0,0,ux)=1.;      // u=x^2 + 2xy + y^2 + xz
    spatialCoefficientsForTZ(1,1,0,ux)=2.;
    spatialCoefficientsForTZ(0,2,0,ux)=1.;
    spatialCoefficientsForTZ(1,0,1,ux)=1.;
    spatialCoefficientsForTZ(3,0,0,ux)=.5;      // + .5*x^3

    spatialCoefficientsForTZ(4,0,0,ux)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
    spatialCoefficientsForTZ(0,4,0,ux)=.125;    
    spatialCoefficientsForTZ(0,0,4,ux)=.125; 
    spatialCoefficientsForTZ(1,0,3,ux)=-.5; 
    spatialCoefficientsForTZ(0,1,3,ux)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
    spatialCoefficientsForTZ(0,2,2,ux)=-.25; 
    spatialCoefficientsForTZ(0,3,1,ux)=.25; 
      
      
    spatialCoefficientsForTZ(2,0,0,uy)= 1.;      // v=x^2 -2xy - y^2 + 3yz
    spatialCoefficientsForTZ(1,1,0,uy)=-2.;
    spatialCoefficientsForTZ(0,2,0,uy)=-1.;
    spatialCoefficientsForTZ(0,1,1,uy)=+3.;
      
    spatialCoefficientsForTZ(2,1,0,uy)=-1.5;     // -1.5x^2*y
      
    spatialCoefficientsForTZ(4,0,0,uy)=.25; 
    spatialCoefficientsForTZ(0,4,0,uy)=.25; 
    spatialCoefficientsForTZ(0,0,4,uy)=.25; 
    spatialCoefficientsForTZ(3,1,0,uy)=-.5; 
    spatialCoefficientsForTZ(1,0,3,uy)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
    spatialCoefficientsForTZ(2,0,2,uy)=-.25; 
    spatialCoefficientsForTZ(3,0,1,uy)=.25; 
      
      
    spatialCoefficientsForTZ(2,0,0,uz)= 1.;      // w=x^2 + y^2 - 2 z^2
    spatialCoefficientsForTZ(0,2,0,uz)= 1.;
    spatialCoefficientsForTZ(0,0,2,uz)=-2.;
      
    spatialCoefficientsForTZ(4,0,0,uz)=.25; 
    spatialCoefficientsForTZ(0,4,0,uz)=-.2; 
    spatialCoefficientsForTZ(0,0,4,uz)=.125; 
    spatialCoefficientsForTZ(0,3,1,uz)=-1.;
    spatialCoefficientsForTZ(1,3,0,uz)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
    spatialCoefficientsForTZ(2,2,0,uz)=-.25; 
    spatialCoefficientsForTZ(3,1,0,uz)=.25; 
  }
  else if( degreeSpace>=5 )
  {
    if( true || degreeSpace!=5 ) printF(" ****WARNING***** using a TZ function with degree=5 in space *****\n");
	  
    spatialCoefficientsForTZ(2,0,0,ux)=1.;      // u=x^2 + 2xy + y^2 + xz
    spatialCoefficientsForTZ(1,1,0,ux)=2.;
    spatialCoefficientsForTZ(0,2,0,ux)=1.;
    spatialCoefficientsForTZ(1,0,1,ux)=1.;
    
    spatialCoefficientsForTZ(4,0,0,ux)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
    spatialCoefficientsForTZ(0,4,0,ux)=.125;    
    spatialCoefficientsForTZ(0,0,4,ux)=.125; 
    spatialCoefficientsForTZ(1,0,3,ux)=-.5; 
    spatialCoefficientsForTZ(0,1,3,ux)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
    spatialCoefficientsForTZ(0,2,2,ux)=-.25; 
    spatialCoefficientsForTZ(0,3,1,ux)=.25; 
    
    spatialCoefficientsForTZ(0,5,0,ux)=.125;   // y^5
    
    
    spatialCoefficientsForTZ(2,0,0,uy)= 1.;      // v=x^2 -2xy - y^2 + 3yz
    spatialCoefficientsForTZ(1,1,0,uy)=-2.;
    spatialCoefficientsForTZ(0,2,0,uy)=-1.;
    spatialCoefficientsForTZ(0,1,1,uy)=+3.;
    
    spatialCoefficientsForTZ(4,0,0,uy)=.25; 
    spatialCoefficientsForTZ(0,4,0,uy)=.25; 
    spatialCoefficientsForTZ(0,0,4,uy)=.25; 
    spatialCoefficientsForTZ(3,1,0,uy)=-.5; 
    spatialCoefficientsForTZ(1,0,3,uy)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
    spatialCoefficientsForTZ(2,0,2,uy)=-.25; 
    spatialCoefficientsForTZ(3,0,1,uy)=.25; 
    
    // spatialCoefficientsForTZ(5,0,0,uy)=.125;  // x^5
    
    
    spatialCoefficientsForTZ(2,0,0,uz)= 1.;      // w=x^2 + y^2 - 2 z^2
    spatialCoefficientsForTZ(0,2,0,uz)= 1.;
    spatialCoefficientsForTZ(0,0,2,uz)=-2.;
    
    spatialCoefficientsForTZ(4,0,0,uz)=.25; 
    spatialCoefficientsForTZ(0,4,0,uz)=-.2; 
    spatialCoefficientsForTZ(0,0,4,uz)=.125; 
    spatialCoefficientsForTZ(0,3,1,uz)=-1.;
    spatialCoefficientsForTZ(1,3,0,uz)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
    spatialCoefficientsForTZ(2,2,0,uz)=-.25; 
    spatialCoefficientsForTZ(3,1,0,uz)=.25; 
    
    // spatialCoefficientsForTZ(5,0,0,uz)=.125;
  }
  else
  {
    printF("Maxwell:: not implemented for degree in space =%i \n",degreeSpace);
    Overture::abort("error");
  }

// *** end the initialize3DPolyTW bpp macro 
#endMacro
  

// ===============================================================================================
// *** Macro: This macro defines the polynomial TZ functions 
// ===============================================================================================
#beginMacro definePolynomialTZMacro()

tz = new OGPolyFunction(degreeSpace,numberOfDimensions,numberOfComponentsForTZ,degreeTime);

const int ndp=max(max(5,degreeSpace+1),degreeTime+1);
    
printF("\n $$$$$$$ assignInitialConditions: build OGPolyFunction: degreeSpace=%i, degreeTime=%i ndp=%i $$$$\n",
       degreeSpace,degreeTime,ndp);

RealArray spatialCoefficientsForTZ(ndp,ndp,ndp,numberOfComponentsForTZ);  
spatialCoefficientsForTZ=0.;
RealArray timeCoefficientsForTZ(ndp,numberOfComponentsForTZ);      
timeCoefficientsForTZ=0.;

// Default coefficients for eps, mu, sigmaE and sigmaH:
assert( epsc>=0 && muc>=0 && sigmaEc>=0 && sigmaHc>=0 );
printF(" *** numberOfComponentsForTZ=%i, epsc,muc,sigmaEc,sigmaHc=%i,%i,%i,%i, eps,mu=%e,%e\n",numberOfComponentsForTZ,epsc,muc,sigmaEc,sigmaHc,eps,mu);

spatialCoefficientsForTZ(0,0,0,epsc)=eps;
spatialCoefficientsForTZ(0,0,0,muc )=mu; 
spatialCoefficientsForTZ(0,0,0,sigmaEc)=0.;  
spatialCoefficientsForTZ(0,0,0,sigmaHc)=0.;

if( numberOfDimensions==2 )
{
  if( degreeSpace==0 )
  {
    spatialCoefficientsForTZ(0,0,0,ex)=1.;      // u=1
    spatialCoefficientsForTZ(0,0,0,ey)= 2.;      // v=2
    spatialCoefficientsForTZ(0,0,0,hz)=-1.;      // w=-1
    // -- dispersion components: 
    if( pxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,pxc)=1.; 
      spatialCoefficientsForTZ(0,0,0,pyc)=2.; 
    }
    if( qxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,qxc)=3.; 
      spatialCoefficientsForTZ(0,0,0,qyc)=4.; 
    }
    if( rxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,rxc)=5.; 
      spatialCoefficientsForTZ(0,0,0,ryc)=6.; 
    }
    
  }
  else if( degreeSpace==1 )
  {
    spatialCoefficientsForTZ(0,0,0,ex)=1.;      // u=1+x+y
    spatialCoefficientsForTZ(1,0,0,ex)=1.;
    spatialCoefficientsForTZ(0,1,0,ex)=1.;

    spatialCoefficientsForTZ(0,0,0,ey)= 2.;      // v=2+x-y
    spatialCoefficientsForTZ(1,0,0,ey)= 1.;
    spatialCoefficientsForTZ(0,1,0,ey)=-1.;

    spatialCoefficientsForTZ(0,0,0,hz)=-1.;      // w=-1+x + y
    spatialCoefficientsForTZ(1,0,0,hz)= 1.;
    spatialCoefficientsForTZ(0,1,0,hz)= 1.;

    // eps and mu should remain positive but do this for now:
    spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x*eps*.01
    spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y*eps*.02 

    spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
    spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y

    // -- dispersion components: 
    // ** FINISH ME **
    if( pxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,pxc)=1.; 
      spatialCoefficientsForTZ(0,0,0,pyc)=2.; 
    }
    if( qxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,qxc)=3.; 
      spatialCoefficientsForTZ(0,0,0,qyc)=4.; 
    }
    if( rxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,rxc)=5.; 
      spatialCoefficientsForTZ(0,0,0,ryc)=6.; 
    }

  }
  else if( degreeSpace==2 )
  {
    spatialCoefficientsForTZ(2,0,0,ex)=1.;      // u=x^2 + 2xy + y^2 
    spatialCoefficientsForTZ(1,1,0,ex)=2.;
    spatialCoefficientsForTZ(0,2,0,ex)=1.;

    spatialCoefficientsForTZ(2,0,0,ey)= 1.;      // v=x^2 -2xy - y^2 
    spatialCoefficientsForTZ(1,1,0,ey)=-2.;
    spatialCoefficientsForTZ(0,2,0,ey)=-1.;

    spatialCoefficientsForTZ(2,0,0,hz)= 1.;      // w=x^2 + y^2 -1 +.5 xy
    spatialCoefficientsForTZ(0,2,0,hz)= 1.;
    spatialCoefficientsForTZ(0,0,0,hz)=-1.; 
    spatialCoefficientsForTZ(1,1,0,hz)= .5;

    // eps and mu should remain positive 
    spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
    spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
    spatialCoefficientsForTZ(2,0,0,epsc)=eps*.1;   // x^2
    spatialCoefficientsForTZ(0,2,0,epsc)=eps*.15;  // y^2        

    spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
    spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
    spatialCoefficientsForTZ(2,0,0,muc )=mu*.125;   // x^2
    spatialCoefficientsForTZ(0,2,0,muc )=mu*.15;    // y^2

    // -- dispersion components: 
    // ** FINISH ME **
    if( pxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,pxc)=1.; 
      spatialCoefficientsForTZ(1,0,0,pxc)=.1; 
      spatialCoefficientsForTZ(0,1,0,pxc)=.1; 
      spatialCoefficientsForTZ(2,0,0,pxc)=.1; 
      spatialCoefficientsForTZ(0,2,0,pxc)=.1; 

      spatialCoefficientsForTZ(0,0,0,pyc)=2.; 
      spatialCoefficientsForTZ(1,0,0,pyc)=.1; 
      spatialCoefficientsForTZ(0,1,0,pyc)=.2; 
      spatialCoefficientsForTZ(2,0,0,pyc)=-.1; 
      spatialCoefficientsForTZ(0,2,0,pyc)=.1; 
    }
    if( qxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,qxc)=3.; 
      spatialCoefficientsForTZ(0,0,0,qyc)=4.; 
    }
    if( rxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,rxc)=5.; 
      spatialCoefficientsForTZ(0,0,0,ryc)=6.; 
    }
  }
  else if( degreeSpace==3 )
  {
    spatialCoefficientsForTZ(2,0,0,ex)=1.;      // u=x^2 + 2xy + y^2 + .5*y^3 + .25*x^2*y + .2*x^3  - .3*x*y^2
    spatialCoefficientsForTZ(1,1,0,ex)=2.;
    spatialCoefficientsForTZ(0,2,0,ex)=1.;
    spatialCoefficientsForTZ(0,3,0,ex)=.5;
    spatialCoefficientsForTZ(2,1,0,ex)=.25;
    spatialCoefficientsForTZ(3,0,0,0,ex)=.2;
    spatialCoefficientsForTZ(1,2,0,0,ex)=-.3;

    spatialCoefficientsForTZ(2,0,0,ey)= 1.;      // v=x^2 -2xy - y^2 -.5*x^3 -.25*x*y^2  -.6*x^2*y + .1*y^3
    spatialCoefficientsForTZ(1,1,0,ey)=-2.;
    spatialCoefficientsForTZ(0,2,0,ey)=-1.;
    spatialCoefficientsForTZ(3,0,0,ey)=-.5;
    spatialCoefficientsForTZ(1,2,0,ey)=-.25;
    spatialCoefficientsForTZ(2,1,0,ey)=-.6;
    spatialCoefficientsForTZ(0,3,0,ey)= .1;

    spatialCoefficientsForTZ(2,0,0,hz)= 1.;      // w=x^2 + y^2 -1 +.5 xy + .25*x^3 - .25*y^3
    spatialCoefficientsForTZ(0,2,0,hz)= 1.;
    spatialCoefficientsForTZ(0,0,0,hz)=-1.; 
    spatialCoefficientsForTZ(1,1,0,hz)= .5;
    spatialCoefficientsForTZ(3,0,0,hz)= .25;
    spatialCoefficientsForTZ(0,3,0,hz)=-.25;

    // -- dispersion components: 
    // ** FINISH ME **
    if( pxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,pxc)=1.; 
      spatialCoefficientsForTZ(0,0,0,pyc)=2.; 
    }
    if( qxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,qxc)=3.; 
      spatialCoefficientsForTZ(0,0,0,qyc)=4.; 
    }
    if( rxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,rxc)=5.; 
      spatialCoefficientsForTZ(0,0,0,ryc)=6.; 
    }
  }
  else if( degreeSpace==4 || degreeSpace==5 )
  {
    if( degreeSpace!=4 ) printF(" ****WARNING***** using a TZ function with degree=4 in space *****\n");
	  
    spatialCoefficientsForTZ(2,0,0,hz)= 1.;      // p=x^2 + y^2 -1 +.5 xy + x^4 + y^4 
    spatialCoefficientsForTZ(0,2,0,hz)= 1.;
    spatialCoefficientsForTZ(0,0,0,hz)=-1.; 
    spatialCoefficientsForTZ(1,1,0,hz)= .5;

    spatialCoefficientsForTZ(4,0,0,hz)= 1.;     
    spatialCoefficientsForTZ(0,4,0,hz)= 1.;     
    spatialCoefficientsForTZ(2,2,0,hz)= -.3;


    spatialCoefficientsForTZ(2,0,0,ex)=1.;      // u=x^2 + 2xy + y^2 + .2*x^4 + .5*y^4 + xy^3
    spatialCoefficientsForTZ(1,1,0,ex)=2.;
    spatialCoefficientsForTZ(0,2,0,ex)=1.;

    spatialCoefficientsForTZ(4,0,0,ex)=.2;   
    spatialCoefficientsForTZ(0,4,0,ex)=.5;   
    spatialCoefficientsForTZ(1,3,0,ex)=1.;   


    spatialCoefficientsForTZ(2,0,0,ey)= 1.;      // v=x^2 -2xy - y^2 +.125*x^4 -.25*y^4 -.8*x^3 y
    spatialCoefficientsForTZ(1,1,0,ey)=-2.;
    spatialCoefficientsForTZ(0,2,0,ey)=-1.;

    spatialCoefficientsForTZ(4,0,0,ey)=.125;
    spatialCoefficientsForTZ(0,4,0,ey)=-.25;
    spatialCoefficientsForTZ(3,1,0,ey)=-.8;

    // -- dispersion components: 
    // ** FINISH ME **
    if( pxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,pxc)=1.; 
      spatialCoefficientsForTZ(0,0,0,pyc)=2.; 
    }
    if( qxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,qxc)=3.; 
      spatialCoefficientsForTZ(0,0,0,qyc)=4.; 
    }
    if( rxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,rxc)=5.; 
      spatialCoefficientsForTZ(0,0,0,ryc)=6.; 
    }
  }
  else if( degreeSpace>=6 )
  {
    if( degreeSpace!=6 ) printF(" ****WARNING***** using a TZ function with degree=4 in space *****\n");
	  
    spatialCoefficientsForTZ(1,0,0,hz)= 1.;
    spatialCoefficientsForTZ(0,0,0,hz)= 1.;

    spatialCoefficientsForTZ(2,0,0,hz)= 1.;      // p=x^2 + y^2 -1 +.5 xy + x^4 + y^4 
    spatialCoefficientsForTZ(0,2,0,hz)= 1.;
    spatialCoefficientsForTZ(0,0,0,hz)=-1.; 
    spatialCoefficientsForTZ(1,1,0,hz)= .5;

    spatialCoefficientsForTZ(4,0,0,hz)= .2;     
    spatialCoefficientsForTZ(0,4,0,hz)= .4;     
    spatialCoefficientsForTZ(2,2,0,hz)= -.3;

    spatialCoefficientsForTZ(3,2,0,hz)= .4;  
    spatialCoefficientsForTZ(2,3,0,hz)= .8;  
    spatialCoefficientsForTZ(3,3,0,hz)= .7;  

    spatialCoefficientsForTZ(5,1,0,hz)= .25;  
    spatialCoefficientsForTZ(1,5,0,hz)=-.25;  

    spatialCoefficientsForTZ(6,0,0,hz)= .2;     
    spatialCoefficientsForTZ(0,6,0,hz)=-.2;     

    //    spatialCoefficientsForTZ=0.; // ************************************************
	  
    //spatialCoefficientsForTZ(2,4,0,hz)= 1.;
    //spatialCoefficientsForTZ(4,2,0,hz)= 1.;

    spatialCoefficientsForTZ(2,0,0,ex)=1.;      // u=x^2 + 2xy + y^2 + .2*x^4 + .5*y^4 + xy^3
    spatialCoefficientsForTZ(1,1,0,ex)=2.;
    spatialCoefficientsForTZ(0,2,0,ex)=1.;

    spatialCoefficientsForTZ(4,0,0,ex)=.2;   
    spatialCoefficientsForTZ(0,4,0,ex)=.5;   
    spatialCoefficientsForTZ(1,3,0,ex)=1.;   

    spatialCoefficientsForTZ(3,2,0,ex)=.1;      // .1*x^3*y^2

    spatialCoefficientsForTZ(4,2,0,ex)=.3;      // .3 x^4 y^2 ** III
    spatialCoefficientsForTZ(3,3,0,ex)=.4;      // .4 x^3 y^3 ** IV 

    spatialCoefficientsForTZ(6,0,0,ex)=.1;      //  + .1*x^6 +.25*y^6 -.6*x*y^5
    spatialCoefficientsForTZ(0,6,0,ex)=.25;
    spatialCoefficientsForTZ(1,5,0,ex)=-.6;


    spatialCoefficientsForTZ(2,0,0,ey)= 1.;      // v=x^2 -2xy - y^2 +.125*x^4 -.25*y^4 -.8*x^3 y
    spatialCoefficientsForTZ(1,1,0,ey)=-2.;
    spatialCoefficientsForTZ(0,2,0,ey)=-1.;

    spatialCoefficientsForTZ(2,3,0,ey)=-.1;      // -.1*x^2*y^3

    spatialCoefficientsForTZ(3,3,0,ey)=-.4;     //-.4 x^3 y^3 ** III 
    spatialCoefficientsForTZ(2,4,0,ey)=-.3;      //-.3 x^2 y^4 ** IV

    spatialCoefficientsForTZ(4,0,0,ey)=.125;
    spatialCoefficientsForTZ(0,4,0,ey)=-.25;
    spatialCoefficientsForTZ(3,1,0,ey)=-.8;

    spatialCoefficientsForTZ(6,0,0,ey)=.3;    //   .3*x^6 +.1*y^6  + .6*x^5*y 
    spatialCoefficientsForTZ(0,6,0,ey)=.1;
    spatialCoefficientsForTZ(5,1,0,ey)=-.6;

    // -- dispersion components: 
    // ** FINISH ME **
    if( pxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,pxc)=1.; 
      spatialCoefficientsForTZ(0,0,0,pyc)=2.; 
    }
    if( qxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,qxc)=3.; 
      spatialCoefficientsForTZ(0,0,0,qyc)=4.; 
    }
    if( rxc>=0 )
    {
      spatialCoefficientsForTZ(0,0,0,rxc)=5.; 
      spatialCoefficientsForTZ(0,0,0,ryc)=6.; 
    }

  }
  else
  {
    printF("Maxwell:: not implemented for degree in space =%i \n",degreeSpace);
    Overture::abort("error");
  }
}
// *****************************************************************
// ******************* Three Dimensions ****************************
// *****************************************************************
else if( numberOfDimensions==3 )
{
  // ** finish me -- make the E and H poly's be different
  printF("*** initTZ functions: solveForElectricField=%i solveForMagneticField=%i\n",
         solveForElectricField,solveForMagneticField);

  if ( solveForElectricField )
  {
    initialize3DPolyTW(ex,ey,ez);
  }
  
  if ( solveForMagneticField )
  {
    initialize3DPolyTW(hx,hy,hz);
  }

  // -- dispersion components: 
  if( pxc>=0 ) 
  {
    initialize3DPolyTW(pxc,pyc,pzc);
  }
  if( qxc>=0 ) 
  {
    initialize3DPolyTW(qxc,qyc,qzc);
  }
  if( rxc>=0 ) 
  {
    initialize3DPolyTW(rxc,ryc,rzc);
  }
  
  
}
else
{
  OV_ABORT("ERROR:unimplemented number of dimensions");
}


for( int n=0; n<numberOfComponents; n++ )
{
  for( int i=0; i<ndp; i++ )
    timeCoefficientsForTZ(i,n)= i<=degreeTime ? 1./(i+1) : 0. ;

}
  
if( method==sosup )
{
  // Set the TZ function for (ext,eyt,...) equal to the time derivative of (ex,ey,...)
  const int numberOfFieldComponents=3;  // 2D: (ex,ey,hz),  3D: (ex,ey,ez)
  for( int n=ex, nt=ext; n<ex+numberOfFieldComponents; n++, nt++ )
  {
    for( int i1=0; i1<ndp; i1++ )for( int i2=0; i2<ndp; i2++ )for( int i3=0; i3<ndp; i3++ )
    {
      spatialCoefficientsForTZ(i1,i2,i3,nt)=spatialCoefficientsForTZ(i1,i2,i3,n);
    }
    // E =   a0 + a1*t + a2*t^2 + ...  = [a0,a1,a2,...
    // E_t =      a1   +2*a2*t + 3*a3*t^2  = [a1,2*a2,3*a3
    for( int i=0; i<ndp; i++ )
      timeCoefficientsForTZ(i,nt)= i<degreeTime ? real(i+1.)/(i+2.) : 0. ;

  }
}

    

// Make eps, mu, .. constant in time : 
timeCoefficientsForTZ(0,rc)=1.;
timeCoefficientsForTZ(0,epsc)=1.;
timeCoefficientsForTZ(0,muc)=1.;
timeCoefficientsForTZ(0,sigmaEc)=1.;
timeCoefficientsForTZ(0,sigmaHc)=1.;

// ::display(spatialCoefficientsForTZ,"spatialCoefficientsForTZ","%6.2f ");
    
((OGPolyFunction*)tz)->setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ ); 

// real epsEx = ((OGPolyFunction*)tz)->gd(0,0,0,0,.0,0.,0.,epsc,0.);
// printF(" ********** epsEx = %e *********\n",epsEx);

#endMacro // polynomial TZ macro 

