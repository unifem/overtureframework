// This file automatically generated from assignAnnulusEigenfunction.bC with bpp.
#include "Cgsm.h"
#include "SmParameters.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "ParallelUtility.h"

#define getEng EXTERN_C_NAME(geteng)

extern "C"
{
    void getEng( const real &eta, const real &p, real &e, real &a, real &b, real &c, real &d );
}

#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

// ===========================================================================================
/// \brief Assign the annulus eigenfunction initial conditions or evaluate the error
/// \param gfIndex (input) : assign gf[gfIndex].u 
/// \param evalOption (input) : computeInitialConditions or computeErrors
// ===========================================================================================
int Cgsm::
assignAnnulusEigenfunction( const int gfIndex, const EvaluationOptionsEnum evalOption )
{
    const real t = gf[gfIndex].t;

    assert(  evalOption==computeInitialConditions  || evalOption==computeErrors );

    FILE *& debugFile  =parameters.dbase.get<FILE* >("debugFile");
    FILE *& logFile    =parameters.dbase.get<FILE* >("logFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

    const int numberOfDimensions=cg.numberOfDimensions();
    const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    const int & uc =  parameters.dbase.get<int >("uc");
    const int & vc =  parameters.dbase.get<int >("vc");
    const int & wc =  parameters.dbase.get<int >("wc");
    const int & rc =  parameters.dbase.get<int >("rc");
    const int & tc =  parameters.dbase.get<int >("tc");
    const int & pc =  parameters.dbase.get<int >("pc"); // for hemp

    const int v1c = parameters.dbase.get<int >("v1c");
    const int v2c = parameters.dbase.get<int >("v2c");
    const int v3c = parameters.dbase.get<int >("v3c");
    bool assignVelocities= v1c>=0 ;

    const int & u1c = parameters.dbase.get<int >("u1c");
    const int & u2c = parameters.dbase.get<int >("u2c");
    const int & u3c = parameters.dbase.get<int >("u3c");

    int s11c = parameters.dbase.get<int >("s11c");
    int s12c = parameters.dbase.get<int >("s12c");
    int s13c = parameters.dbase.get<int >("s13c");
    int s21c = parameters.dbase.get<int >("s21c");
    int s22c = parameters.dbase.get<int >("s22c");
    int s23c = parameters.dbase.get<int >("s23c");
    int s31c = parameters.dbase.get<int >("s31c");
    int s32c = parameters.dbase.get<int >("s32c");
    int s33c = parameters.dbase.get<int >("s33c");
    bool assignStress = s11c >=0 ;
  // some models assume symmetry of the stress tensor: (do this so we can avoid checking below)
    if( s21c<0 ) s21c=s12c;
    if( s31c<0 ) s31c=s13c;
    if( s32c<0 ) s32c=s23c;


    const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
    const int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");
    SmParameters::TimeSteppingMethodSm & timeSteppingMethodSm = 
                                                                      parameters.dbase.get<SmParameters::TimeSteppingMethodSm>("timeSteppingMethodSm");

    const int numberOfComponentGrids = cg.numberOfComponentGrids();

    real & rho=parameters.dbase.get<real>("rho");
    real & mu = parameters.dbase.get<real>("mu");
    real & lambda = parameters.dbase.get<real>("lambda");
    RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
    RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");
    int & debug = parameters.dbase.get<int >("debug");

    real r0=.5, r1=1., p0=2., p1=1.;  // fix me 

    const int opt = int(initialConditionParameters[0]+.5);  // 0=displacement 1=traction
    const int modem = int(initialConditionParameters[1]+.5); // eigenvalue, modem=0,1,2,...
    const int moden = int(initialConditionParameters[2]+.5); // eigenvalue, moden=0,1,2,...

    p0 = initialConditionParameters[3];
    p1 = initialConditionParameters[4];


  // steady state solution from [Lamb, p144]
    real a = (p1*r1*r1 - p0*r0*r0)/( 2.*(lambda+mu)*(r0*r0-r1*r1) );
    real b = (p1-p0)*r0*r0*r1*r1/( 2.*mu*(r0*r0-r1*r1) );

    printF(" *** assignAnnulusEigenfunction: opt=%i m=%i n=%i p0=%8.2e p1=%8.2e a=%8.2e b=%8.2e\n",
                                    opt,modem,moden,p0,p1,a,b);

    SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");

    Range C=numberOfComponents;
    Index I1,I2,I3;
    int i1,i2,i3;

    
    for( int grid=0; grid<numberOfComponentGrids; grid++ )
    {
        MappedGrid & mg = cg[grid];
        const bool isRectangular = mg.isRectangular();

    //mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter ); // do this for now
        if( pdeVariation == SmParameters::hemp )
        {
            mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEcenterJacobian ); // do this for now
        }
        else
        {
            mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter ); // do this for now
        }
        
        const realArray & x = mg.center();

        realMappedGridFunction & u =gf[gfIndex].u[grid];
    // Hemp: here is where we store the initial state (mass,density,energy)
        realMappedGridFunction *pstate0 = NULL;
        if( pdeVariation == SmParameters::hemp )
        {
            assert( parameters.dbase.get<realCompositeGridFunction*>("initialStateGridFunction")!=NULL );
            pstate0 = &(*(parameters.dbase.get<realCompositeGridFunction*>("initialStateGridFunction")))[grid];
        }
        realMappedGridFunction & state0 = *pstate0;

#ifdef USE_PPP
        realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
        realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(x,xLocal);
        realSerialArray det;   
        if( pdeVariation == SmParameters::hemp ) 
            getLocalArrayWithGhostBoundaries(mg.centerJacobian(),det);
        realSerialArray state0Local;
        if( pdeVariation == SmParameters::hemp )
            getLocalArrayWithGhostBoundaries(state0,state0Local);
#else
        realSerialArray & uLocal  =  u;
        const realSerialArray & xLocal  =  x;
        const realSerialArray & det = mg.centerJacobian();
        realSerialArray & state0Local = *pstate0;
#endif

        real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
        const int uDim0=uLocal.getRawDataSize(0);
        const int uDim1=uLocal.getRawDataSize(1);
        const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]


        real *xp = xLocal.Array_Descriptor.Array_View_Pointer3;
        const int xDim0=xLocal.getRawDataSize(0);
        const int xDim1=xLocal.getRawDataSize(1);
        const int xDim2=xLocal.getRawDataSize(2);
#undef X
#define X(i0,i1,i2,i3) xp[i0+xDim0*(i1+xDim1*(i2+xDim2*(i3)))]

        realMappedGridFunction & err = evalOption==computeErrors ? (*cgerrp)[grid] : u;

#ifdef USE_PPP
        realSerialArray errLocal;  getLocalArrayWithGhostBoundaries(err,errLocal);
#else
        const realSerialArray & errLocal=  err;
#endif

        real *errp = errLocal.Array_Descriptor.Array_View_Pointer3;
        const int errDim0=errLocal.getRawDataSize(0);
        const int errDim1=errLocal.getRawDataSize(1);
        const int errDim2=errLocal.getRawDataSize(2);
#undef ERR
#define ERR(i0,i1,i2,i3) errp[i0+errDim0*(i1+errDim1*(i2+errDim2*(i3)))]


        getIndex(mg.dimension(),I1,I2,I3);

        int includeGhost=1;
        bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
        if( !ok ) continue;


        lambda = lambdaGrid(grid);
        mu = muGrid(grid);
        c1=(mu+lambda)/rho, c2= mu/rho;


    // do this for now: 
        bool useDisplacementBC = opt==0;
        printF(">> assignAnnulusEigenfunction: evalOption=%i bc=%i, useDisplacementBC=%i lambda=%9.3e \n",(int)evalOption,mg.boundaryCondition(0,1),(int)useDisplacementBC,lambda);
        
// Solution: (from cgDoc/sm/annulusEigs.maple)
//    U = [ A*J1(alpha*r) + B*Y1(alpha*r) ] cos(omega*t)
//    U(ra)=0 U(rb)=0 
// ra=.5, rb=1 : 
// omega^2 = alpha^2*(lambda+2*mu)/rho 
// xp=3.19657838081064e+00, alpha=6.39315676162128e+00 A=3.70054796926960e-01 B=-2.62717605998725e-01
// 
        real alpha=1., a1=0., b1=0., scale=1.;
        if( useDisplacementBC )
        {
            a=0.;  // turn off steady state -- this is for a traction BC
            b=0.;
              
            if( modem==0 )
            {
                a1=0.; b1=0.;
            }
            else if( modem==1 )
            {
      	alpha=6.39315676162128e+00, a1=3.70054796926960e-01, b1=-2.62717605998725e-01;
                scale=5.; // scale eigenfunction so it is order 1 
            }
            else if( modem==2 )
            {
      	alpha=1.26246990207465e+01, a1=-2.44533726961556e-01, b1=2.04902796341412e-01;
                scale=5.;
            }
            else
            {
                printF("ERROR: annulus eignefunction not available for modem=%i\n",modem);
      	Overture::abort("error");
            }
            
        }
        else
        { // zero traction eigenfunction: 

            if( mu==1. && lambda==1. )
            {
        //  real alpha=1.31135301901399e+00, a1=1.86192468503470e+00, b1=-1.14016375430057e+00;   w(alpha)=-5.00e-14 
        //  real alpha=6.46336546990778e+00, a1=2.10897392919530e+00, b1=3.67967002089679e+00;   w(alpha)= 1.00e-13 
      	if( modem==0 )
      	{
        	  a1=0.; b1=0.;
      	}
      	else if( modem==1 )
      	{
        	  alpha=1.31135301901399e+00, a1=1.86192468503470e+00, b1=-1.14016375430057e+00; 
      	}
      	else if( modem==2 )
      	{
        	  alpha=6.46336546990778e+00, a1=2.10897392919530e+00, b1=3.67967002089679e+00;
      	}
      	else
      	{
        	  printF("ERROR: annulus eignefunction not available for modem=%i\n",modem);
        	  OV_ABORT("error");
      	}

                scale = 0.1;

            }
            else if( mu==1. && lambda==100. ) 
            {
	// cgDoc/sm/annulusEigs.maple:
        // choice=1 lambda=1.000000e+02 mu=1.000000e+00 rb/ra=2.000000e+00
        //  real alpha=6.25250106351779e+00, a1=1.05745149814486e+02, b1=9.61901121117054e+01;   w(alpha)=-1.00e-20
        //  real alpha=1.25500188926573e+01, a1=-1.47413081181153e+02, b1=-1.40300539642564e+02;   w(alpha)= 1.00e-19
      	if( modem==0 )
      	{
        	  a1=0.; b1=0.;
      	}
      	else if( modem==1 )
      	{
        	  alpha=6.25250106351779e+00, a1=1.05745149814486e+02, b1=9.61901121117054e+01; 
      	}
      	else if( modem==2 )
      	{
        	  alpha=1.25500188926573e+01, a1=-1.47413081181153e+02, b1=-1.40300539642564e+02;
      	}
      	else
      	{
        	  printF("ERROR: annulus eignefunction not available for modem=%i\n",modem);
        	  OV_ABORT("error");
      	}

                scale = 1./60.;

            }
            else
            {
                  OV_ABORT("error: mu and lambda");
            }
            
            
        }
    //scale = 1.0e-6;
        a1*=scale; // scale eigenfunction so it is order 1 
        b1*=scale;
        
        const real omega = alpha*sqrt( (lambda+2*mu)/rho );

        printF(" assignAnnulusEigenfunction:  a=%8.2e b=%8.2e a1=%8.2e, b1=%8.2e, omega=%8.2e\n",a,b,a1,b1,omega);

        if( evalOption==computeInitialConditions )
        {
            uLocal=0.;
        }
        
        if( mg.numberOfDimensions()==2 )
        {
            FOR_3D(i1,i2,i3,I1,I2,I3)
            {
      	real x0 = X(i1,i2,i3,0);
      	real y0 = X(i1,i2,i3,1);
      	real r = sqrt( x0*x0 + y0*y0 );
      	real cosTheta = x0/r;
      	real sinTheta = y0/r;
      	real cost=cos(omega*t);

      	real ar = alpha*r;
      	real ub = a1*jn(1,ar)+b1*yn(1,ar);
      	real ua = (a*r + b/r) + ub*cost;

      	real ue = ua*cosTheta;
      	real ve = ua*sinTheta;
      	if( evalOption==computeInitialConditions )
      	{
        	  if( pdeVariation == SmParameters::hemp )
        	  {
          	    U(i1,i2,i3,u1c) = ue;
          	    U(i1,i2,i3,u2c) = ve;
          	    U(i1,i2,i3,uc) = x0;
          	    U(i1,i2,i3,vc) = y0;
	    //U(i1,i2,i3,uc) = x0+ue;
	    //U(i1,i2,i3,vc) = y0+ve;
        	  }
        	  else
        	  {
          	    U(i1,i2,i3,uc) = ue;
          	    U(i1,i2,i3,vc) = ve;
        	  }
      	}
      	else
      	{
        	  if( pdeVariation == SmParameters::hemp )
        	  {
	    //ERR(i1,i2,i3,uc) = U(i1,i2,i3,uc) - (ue+x0);
	    //ERR(i1,i2,i3,vc) = U(i1,i2,i3,vc) - (ve+y0);
          	    ERR(i1,i2,i3,u1c) = U(i1,i2,i3,u1c) - ue;
          	    ERR(i1,i2,i3,u2c) = U(i1,i2,i3,u2c) - ve;
          	    ERR(i1,i2,i3,uc) = U(i1,i2,i3,uc) - x0;
          	    ERR(i1,i2,i3,vc) = U(i1,i2,i3,vc) - y0;
        	  }
        	  else
        	  {
          	    ERR(i1,i2,i3,uc) = U(i1,i2,i3,uc) - ue;
          	    ERR(i1,i2,i3,vc) = U(i1,i2,i3,vc) - ve;
        	  }
      	}
      	
	// printF(" annulus-eig: i=(%i,%i) r=%8.2e (u,v)=(%8.2e,%8.2e) \n",i1,i2,r,U(i1,i2,i3,uc),U(i1,i2,i3,vc));
      	if( assignVelocities )
      	{
        	  real uat = -omega*sin(omega*t)*ub;
                    real v1e = uat*cosTheta;
                    real v2e = uat*sinTheta;
        	  if( evalOption==computeInitialConditions )
        	  {
          	    U(i1,i2,i3,v1c) = v1e;
          	    U(i1,i2,i3,v2c) = v2e;
        	  }
        	  else
        	  {
          	    ERR(i1,i2,i3,v1c) = U(i1,i2,i3,v1c) - v1e;
          	    ERR(i1,i2,i3,v2c) = U(i1,i2,i3,v2c) - v2e;
        	  }
      	}
      	if( assignStress )
      	{
        	  if( pdeVariation == SmParameters::hemp && 
            	      i1 > I1Base && i1 < I1Bound &&
            	      i2 > I2Base && i2 < I2Bound )
        	  {
          	    real x1,x2,x3,x4,y1,y2,y3,y4;
        	  
          	    x1 = X(i1,i2,i3,0);
          	    x2 = X(i1+1,i2,i3,0);
          	    x3 = X(i1+1,i2+1,i3,0);
          	    x4 = X(i1,i2+1,i3,0);

          	    y1 = X(i1,i2,i3,1);
          	    y2 = X(i1+1,i2,i3,1);
          	    y3 = X(i1+1,i2+1,i3,1);
          	    y4 = X(i1,i2+1,i3,1);

          	    x0 = 0.25*(x1+x2+x3+x4);
          	    y0 = 0.25*(y1+y2+y3+y4);
          	    real tLoc = t;

          	    r = sqrt( x0*x0 + y0*y0 );
          	    cosTheta = x0/r;
          	    sinTheta = y0/r;
          	    cost=cos(omega*tLoc);

          	    ar = alpha*r;
          	    ub = a1*jn(1,ar)+b1*yn(1,ar);
          	    ua = (a*r + b/r) + ub*cost;
        	  }

	  // check this ...
          // dr/dx = x/r,   dr/dy=y/r 
        	  real rx = cosTheta, ry=sinTheta;

	  // Jn' = .5*( J_{n-1} - J_{n+1} )
        	  real jn1r = .5*alpha*( jn(0,ar) - jn(2,ar) );  // (d/dr) J1(alpha*r) 
        	  real yn1r = .5*alpha*( yn(0,ar) - yn(2,ar) );

          // ubr = d(ub)/dr  
        	  real ubr = a1*jn1r + b1*yn1r;
        	  real uar = a - b/(r*r) + ubr*cost;

        	  real cosThetax= sinTheta*sinTheta/r;         // (d/dx) cosTheta = (d/dx)(x/r) = 1/r - x^2/r^3 = (r^2-x^2)/r^3 = y^2/r^3 
        	  real sinThetax=-cosTheta*sinTheta/r;         // (d/dx) sinTheta = (d/dx)(y/r) = -y*x/r^3
        	  real cosThetay=sinThetax;                    // (d/dy) cosTheta = (d/dy)(x/r) = -x*y/r^3
        	  real sinThetay= cosTheta*cosTheta/r;
          	    
          // ue = ua*cosTheta : 
        	  real ux = uar*rx*cosTheta + ua*cosThetax;
        	  real uy = uar*ry*cosTheta + ua*cosThetay;
        	  real vx = uar*rx*sinTheta + ua*sinThetax;
        	  real vy = uar*ry*sinTheta + ua*sinThetay;
        	  real div = ux+vy;

        	  real s11e = lambda*div + 2.*mu*ux;
        	  real s12e = mu*(uy+vx);
        	  real s21e = s12e;
        	  real s22e = lambda*div + 2.*mu*vy;

	  // p = lambda*div ... others to follow for hemp

        	  if( evalOption==computeInitialConditions )
        	  {
	    // assign local state for hemp
          	    if( pdeVariation == SmParameters::hemp )
          	    {
            	      const std::vector<real> & polyEOS = parameters.dbase.get<std::vector<real> >("polyEos"); // a,b,c,d in Wilkin's EOS
            	      real eta,press,eng,a,b,c,d;
            	      a = polyEOS[0];
            	      b = polyEOS[1];
            	      c = polyEOS[2];
            	      d = polyEOS[3];
            	      state0Local(i1,i2,i3,1) = 1.0; // density
            	      /*********/
            	      state0Local(i1,i2,i3,0) = state0Local(i1,i2,i3,1)*det(i1,i2,i3)*mg.gridSpacing(axis1)*mg.gridSpacing(axis2); // mass
            	      if( pdeVariation == SmParameters::hemp && 
              		  i1 > I1Base && i1 < I1Bound &&
              		  i2 > I2Base && i2 < I2Bound )
            	      {
            		real area = 0.25*(det(i1,i2,i3)+det(i1+1,i2,i3)+det(i1+1,i2+1,i3)+det(i1,i2+1,i3));
            		state0Local(i1,i2,i3,0) = area*mg.gridSpacing(axis1)*mg.gridSpacing(axis2); // mass
            	      }
            	      /*********/

            	      press = -(lambda+2.0*mu/3.0)*div;
            	      eta = 1.0;
            	      getEng( eta,press,eng,a,b,c,d );
            	      state0Local(i1,i2,i3,2) = eng;
            	      U(i1,i2,i3,s11c) = (s11e+press);
            	      U(i1,i2,i3,s12c) = s12e;
            	      U(i1,i2,i3,s22c) = (s22e+press);
            	      U(i1,i2,i3,pc)   = press;
          	    }
          	    else
          	    {
            	      U(i1,i2,i3,s11c) = s11e;
            	      U(i1,i2,i3,s12c) = s12e;
            	      U(i1,i2,i3,s21c) = s21e;
            	      U(i1,i2,i3,s22c) = s22e;
          	    }
        	  }
        	  else // compute errors 
        	  {
          	    if( pdeVariation == SmParameters::hemp )
          	    {
            	      if( i1 >= I1.getBound()-2 || i2 >= I2.getBound()-2 )
            	      {
            		ERR(i1,i2,i3,s11c) = 0.0;
            		ERR(i1,i2,i3,s12c) = 0.0;
            		ERR(i1,i2,i3,s22c) = 0.0;
            		ERR(i1,i2,i3,pc)   = 0.0;
            	      }
            	      else
            	      {
            		real press = -(lambda+2.0*mu/3.0)*div;
            		ERR(i1,i2,i3,s11c) = U(i1,i2,i3,s11c) - (s11e+press);
            		ERR(i1,i2,i3,s12c) = U(i1,i2,i3,s12c) - s12e;
            		ERR(i1,i2,i3,s22c) = U(i1,i2,i3,s22c) - (s22e+press);
            		ERR(i1,i2,i3,pc)   = U(i1,i2,i3,pc)   - press;
            	      }
          	    }
          	    else
          	    {
            	      ERR(i1,i2,i3,s11c) = U(i1,i2,i3,s11c) - s11e;
            	      ERR(i1,i2,i3,s12c) = U(i1,i2,i3,s12c) - s12e;
            	      ERR(i1,i2,i3,s21c) = U(i1,i2,i3,s21c) - s21e;
            	      ERR(i1,i2,i3,s22c) = U(i1,i2,i3,s22c) - s22e;
          	    }
        	  }
        	  
      	}
        	  
            }
        }
        else
        { // ***** 3D  ****
            FOR_3D(i1,i2,i3,I1,I2,I3)
            {
      	real x0 = X(i1,i2,i3,0);
      	real y0 = X(i1,i2,i3,1);
      	real z0 = X(i1,i2,i3,2);

      	real r = sqrt( x0*x0 + y0*y0 );
      	real cosTheta = x0/r;
      	real sinTheta = y0/r;

      	real ua = (a*r + b/r);
      	real ue = ua*cosTheta;
      	real ve = ua*sinTheta;
      	real we = 0.;

      	if( evalOption==computeInitialConditions )
      	{
        	  U(i1,i2,i3,uc) = ue;
        	  U(i1,i2,i3,vc) = ve;
        	  U(i1,i2,i3,wc) = we;
      	}
      	else
      	{
          	  ERR(i1,i2,i3,uc) = U(i1,i2,i3,uc) - ue;
          	  ERR(i1,i2,i3,vc) = U(i1,i2,i3,vc) - ve;
          	  ERR(i1,i2,i3,wc) = U(i1,i2,i3,wc) - we;
      	}
              
            }
        }

//     if( evalOption==computeErrors )
//     {

//       realMappedGridFunction & err =(*cgerrp)[grid];

// #ifdef USE_PPP
//       realSerialArray errLocal;  getLocalArrayWithGhostBoundaries(err,errLocal);
// #else
//       const realSerialArray & errLocal=  err;
// #endif

//       real *errp = errLocal.Array_Descriptor.Array_View_Pointer3;
//       const int errDim0=errLocal.getRawDataSize(0);
//       const int errDim1=errLocal.getRawDataSize(1);
//       const int errDim2=errLocal.getRawDataSize(2);
// #undef ERR
// #define ERR(i0,i1,i2,i3) errp[i0+errDim0*(i1+errDim1*(i2+errDim2*(i3)))]


//       if( mg.numberOfDimensions()==2 )
//       {
// 	FOR_3D(i1,i2,i3,I1,I2,I3)
// 	{
// 	  real x0 = X(i1,i2,i3,0);
// 	  real y0 = X(i1,i2,i3,1);
// 	  real r = sqrt( x0*x0 + y0*y0 );
// 	  real cosTheta = x0/r;
// 	  real sinTheta = y0/r;

// 	  real ar = alpha*r;
// 	  real ua = (a*r + b/r) + ( a1*jn(1,ar)+b1*yn(1,ar) )*cos(omega*t);

// 	  ERR(i1,i2,i3,uc) = U(i1,i2,i3,uc) - ua*cosTheta;
// 	  ERR(i1,i2,i3,vc) = U(i1,i2,i3,vc) - ua*sinTheta;
// 	  // printF(" i=(%i,%i) r=%8.2e (u,v)=(%8.2e,%8.2e) (ue,ve)=(%8.2e,%8.2e)\n",i1,i2,r,
//           //    U(i1,i2,i3,uc),U(i1,i2,i3,vc),ua*cosTheta,ua*sinTheta);
// 	}
//       }
//       else
//       { // ***** 3D  ****
// 	FOR_3D(i1,i2,i3,I1,I2,I3)
// 	{
// 	  real x0 = X(i1,i2,i3,0);
// 	  real y0 = X(i1,i2,i3,1);
// 	  real z0 = X(i1,i2,i3,2);

// 	  real r = sqrt( x0*x0 + y0*y0 );
// 	  real cosTheta = x0/r;
// 	  real sinTheta = y0/r;

// 	  ERR(i1,i2,i3,uc) = U(i1,i2,i3,uc) - (a*r + b/r)*cosTheta;
// 	  ERR(i1,i2,i3,vc) = U(i1,i2,i3,vc) - (a*r + b/r)*sinTheta;
// 	  ERR(i1,i2,i3,wc) = 0.;
// 	}
//       }

//     }
        

    } // end for grid


    return 0;
}


