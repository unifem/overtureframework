#include "Cgasf.h"
#include "GridFunction.h"
#include "Chemkin.h"
#include "AsfParameters.h"

void Cgasf::
assignTestProblem( GridFunction & cgf )
// ======================================================================================
//   Assign values corresponding to a given testProblem
// ======================================================================================
{
  
  CompositeGrid & cg = cgf.cg;
  realCompositeGridFunction & u = cgf.u;
  Index I1,I2,I3;
  
  //  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  const int & numberOfSpecies = parameters.dbase.get<int >("numberOfSpecies");
  const int & rc = parameters.dbase.get<int >("rc");
  const int & uc = parameters.dbase.get<int >("uc");
  //    const int & vc = parameters.dbase.get<int >("vc");
  //    const int & wc = parameters.dbase.get<int >("wc");
  const int & pc = parameters.dbase.get<int >("pc");
  const int & tc = parameters.dbase.get<int >("tc");
  const int & sc = parameters.dbase.get<int >("sc");

  real & u0   = parameters.dbase.get<real >("u0");
  real & l0   = parameters.dbase.get<real >("l0");
  real & rho0 = parameters.dbase.get<real >("rho0");
  real & te0  = parameters.dbase.get<real >("te0");
  real & p0   = parameters.dbase.get<real >("p0");
  real & R0   = parameters.dbase.get<real >("R0");
  real & pStatic  = parameters.dbase.get<real >("pStatic");
  real & pressureLevel = parameters.dbase.get<real >("pressureLevel");

//   if( parameters.dbase.get<Parameters::TestProblems >("testProblem")==Parameters::bomb )
//   {
//     // Chemkin bomb problem: adiabatic fixed pressure problem

//       cout << "assignTestProblem::ERROR: the bomb problem is for allSpeedNavierStokes \n";
//       throw "error";
//   }
  if( parameters.dbase.get<AsfParameters::TestProblems >("testProblem")==AsfParameters::laminarFlame )
  {
    // hydrogen-Oxygen-Nitrogen laminar flame
    // 71 1200 1    !num. of points in x, num. of time steps, adapt mesh; 0-no, 1-yes
    // 250.        !expected flame speed; use zero if you don't know (cm/s) 
    // 2583.       !equilibrium flame temp; or something close (K)
    // 1200.       !tracking temperature for flame speed (Kelvin)
    // 1 .40       !geometry (1=slab,2=cyl,3=sphere,4=general), length (cm)
    // 2           !imeth (1=Gear,2=backwards Euler)
    // 1           !transport properties (0=constant, 1=computed)
    // 5          !ncount
    // 1.         !pfac:  p=pfac*patm
    // 1.05        !exptim 
    // .0001,.0005   !min and max dimensionless time steps 
    // 2 1.e-8 1.e-6 !min and max time step choice; 1-dimensionless; 2-real
    // 1 1 1        !ireact (1=yes,0=no), iglob (1=yes,0=no), iflamstr 1=yes; 0=no
    // 500. 30. 2. !tlimit (lower limit for chemistry),dtmpmx(Max DT),dtmpmn(Min DT)
    // 1           !iconvect (1=yes,0=no)
    // 300. 0.02831 0.2265 0.74519 ! read in tmpi,yfli,yoxyi,yniti
    // 11 1 3 10       ! read in nspec, chemkin num for fuel,oxy,and nit (see mhects.dat)

    
    assert( parameters.dbase.get<Reactions* >("reactions")!= NULL );
    Reactions & reactions = *(parameters.dbase.get<Reactions* >("reactions"));
    MaterialProperties & mp = reactions.mp;

    reactions.setPressureIsConstant(); // these means use the cp version of the T equation

    u0=2.5;    // velocity scale m/s
    l0=.0025;    // length scale m       [0,1] <-> [0,.25cm]
    rho0=1.;   // density scale  Kg/m^3
    real p0=rho0*u0*u0;  // always choose this for p0

    pStatic=mp.newtonMeterSquaredPerAtmosphere;
    pressureLevel=pStatic/p0;  // subtract out this constant

    te0=1000.;
    R0=pStatic/(rho0*te0);
    printf("scale factors: u0=%e, rho0=%e, pStatic=%e, te0=%e, R0=%e \n",u0,rho0,pStatic,te0,R0);

    reactions.setScales(rho0,te0,pStatic,l0,u0);
    reactions.setPressureLevel(pressureLevel);

    const int sH2=0, sO2=2, sN2=9;
    const real teCold=300., teHot=2583.;

    const real xCold=.5, xHot=.6;
    const real xFlame=.25;         // position of the flame
    const real beta = 1./(.05);     // flame width parameter

    const real yH2=0.02831, yO2=0.2265, yN2=0.74519;
    const real ppp=1.*mp.newtonMeterSquaredPerAtmosphere; 
    const real flameSpeed = 250.*.01;   // (cm/s) -> m/s
    
    const real speciesEpsilon=REAL_MIN*10.; //  1.e-12;
    
    int grid,s;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);
      for( s=0; s<numberOfSpecies; s++ )
        u[grid](I1,I2,I3,sc+s)=speciesEpsilon;       // init all mass fractions to zero
      

      // define a smooth transition transition from the cold state of the left to the hot state on the right
      realArray layer(I1,I2,I3);
      layer=(tanh( (cg[grid].vertex()(I1,I2,I3)-xFlame)*beta )+1.)*.5;
      

      if( TRUE )
      {
	u[grid](I1,I2,I3,uc)=flameSpeed/u0;
	u[grid](I1,I2,I3,pc)=0.;
	u[grid](I1,I2,I3,tc)=teCold/te0+layer*((teHot-teCold)/te0);
	u[grid](I1,I2,I3,sc+sH2)=yH2+layer*(-yH2);
	u[grid](I1,I2,I3,sc+sO2)=yO2+layer*(-yO2);
	u[grid](I1,I2,I3,sc+sN2)=yN2+layer*(1.-yN2);
	
      }
      else
      {

	where( cg[grid].vertex()(I1,I2,I3) > xHot )
	{
	  // hot region:
	  u[grid](I1,I2,I3,uc)=flameSpeed/u0;
	  u[grid](I1,I2,I3,pc)=0.;            // scaled pressure - constant level
	  u[grid](I1,I2,I3,tc)=teHot/te0;

	  u[grid](I1,I2,I3,sc+sN2)=1.;  // all N2 in the burnt region
	}
	elsewhere( cg[grid].vertex()(I1,I2,I3) < xCold )
	{
	  // cold region
	  u[grid](I1,I2,I3,uc)=flameSpeed/u0;
	  u[grid](I1,I2,I3,pc)=0.;             // scaled pressure
	  u[grid](I1,I2,I3,tc)=teCold/te0;

	  u[grid](I1,I2,I3,sc+sH2)=yH2;
	  u[grid](I1,I2,I3,sc+sO2)=yO2;
	  u[grid](I1,I2,I3,sc+sN2)=yN2;
	
	}
	otherwise()
	{
	  // transition
	  u[grid](I1,I2,I3,uc)=flameSpeed/u0;
	  u[grid](I1,I2,I3,pc)=0.;
	  u[grid](I1,I2,I3,tc)=teCold/te0+(cg[grid].vertex()(I1,I2,I3)-xCold)*(((teHot-teCold)/te0)/(xHot-xCold));

	  u[grid](I1,I2,I3,sc+sH2)=yH2+(cg[grid].vertex()(I1,I2,I3)-xCold)*(-yH2/(xHot-xCold));
	  u[grid](I1,I2,I3,sc+sO2)=yO2+(cg[grid].vertex()(I1,I2,I3)-xCold)*(-yO2/(xHot-xCold));
	  u[grid](I1,I2,I3,sc+sN2)=yN2+(cg[grid].vertex()(I1,I2,I3)-xCold)*((1.-yN2)/(xHot-xCold));

	}
      }
      
      // compute rho from the mass fractions
      realArray mBarI(I1,I2,I3);  // 1/mBar
      mBarI=0.;
      for( s=0; s<numberOfSpecies; s++ )
	mBarI+=u[grid](I1,I2,I3,sc+s)/reactions.mw(s);
      u[grid](I1,I2,I3,rc)=( ppp/(mp.R*mBarI*u[grid](I1,I2,I3,tc)*te0) )/rho0;

      // assign boundary conditions, inflow on left, outflow on right

      int i1=cg[0].gridIndexRange()(Start,axis1);
      int i2=cg[0].gridIndexRange()(Start,axis2);
      int i3=cg[0].gridIndexRange()(Start,axis3);
      
//      parameters.subSonicInflowData(rc)=u[0](i1,i2,i3,rc);
//      parameters.subSonicInflowData(tc)=u[0](i1,i2,i3,tc);
//      parameters.subSonicInflowData(uc)=u[0](i1,i2,i3,uc);
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
	  if( cg[grid].boundaryCondition()(side,axis)==AsfParameters::subSonicInflow )
	  {
	    parameters.dbase.get<RealArray>("bcData")(rc,side,axis,grid)=u[0](i1,i2,i3,rc);
	    parameters.dbase.get<RealArray>("bcData")(tc,side,axis,grid)=u[0](i1,i2,i3,tc);
	    parameters.dbase.get<RealArray>("bcData")(uc,side,axis,grid)=u[0](i1,i2,i3,uc);
	    for( s=sc; s<sc+numberOfSpecies; s++ )
	      parameters.dbase.get<RealArray>("bcData")(s,side,axis,grid)=u[0](i1,i2,i3,s);
	  }
	  else if( cg[grid].boundaryCondition()(side,axis)==AsfParameters::subSonicOutflow )
	  {
	    parameters.dbase.get<RealArray>("bcData")(2,side,axis,grid)=0.; // (0)*(ppp/p0);   // set p at outflow
	  }
	}
      }
      
      
      getIndex(cg[grid].dimension(),I1,I2,I3);
      // rho*u = constant in steady flow
      u[grid](I1,I2,I3,uc)=u[0](i1,i2,i3,rc)*u[0](i1,i2,i3,uc)/u[grid](I1,I2,I3,rc);
      // p = - rho u^2 in steady inviscid
      u[grid](I1,I2,I3,pc)=-u[0](I1,I2,I3,rc)*SQR(u[grid](I1,I2,I3,uc));


    } // end for grid
    


  }
  else
  {
    cout << "assignTestProblem::unknown testProblem = " << parameters.dbase.get<AsfParameters::TestProblems >("testProblem") << endl;
  }
}
