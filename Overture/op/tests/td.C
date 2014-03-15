#include "Overture.h"
#include "MappedGridOperators.h"
#include "Annulus.h"
#include "CylinderMapping.h"
#include "OGTrigFunction.h"  // Trigonometric function
#include "OGPolyFunction.h"  // polynomial function
#include "GridFunctionParameters.h"
#include "display.h"


//================================================================================
//  Test out the order of accuracy of selected derivatives
//================================================================================
int 
main(int argc, char **argv)
{
/* ----
  const int maxNumberOfGridsToTest=3;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "square5", "cic", "sib2" };
    
  int tz=0;
  if( argc > 1 )
  { 
    numberOfGridsToTest=1;
    for( int i=1; i<argc; i++ )
    {
      aString line;
      line=argv[i];
      if( line(0,6)=="tz=trig" )
	tz=1;
      else
      {
	numberOfGridsToTest=1;
        gridName[0]=argv[i];
      }
    }
  }
  else
    cout << "Usage: `tderivatives [<gridName>] [tz=trig]' \n";

  int debug=0;
--- */
/* --
  int debug=7;
  cout << "Enter debug \n";
  cin >> debug;  
--- */

  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
    

  AnnulusMapping annulus;
  CylinderMapping cylinder;
  
  for( int mapToTry=0; mapToTry<=1; mapToTry++ )
  {
    Mapping & map = mapToTry==0 ? (Mapping&)annulus : (Mapping&)cylinder;

    int numberOfDimensions=map.getDomainDimension();
  
    Index I1,I2,I3,N;
    Range all;
    realMappedGridFunction u,v,scalar;   // define some component grid functions

    MappedGridOperators op;                     // define some differential operators

    const int numberOfComponents=5;

    RealArray fx(numberOfComponents), fy(numberOfComponents), fz(numberOfComponents), ft(numberOfComponents);
    fx=1.;   fx(0)=.5; fx(1)=1.5;
    fy=.1;   fy(0)=.4; fy(1)= .5;
    if( numberOfDimensions==3 )
    {
      fz=1.;   fz(0)=.3; fz(1)=-.3;
    }
    else
    {
      fz=0;
    }
  
    ft=1.;   ft(0)=.6; ft(1)=.35;
  
    OGTrigFunction trigTrue(fx, fy, fz, ft);        //  defines cos(pi*x)*cos(pi*y)*cos(pi*z)*cos(pi*t)

    RealArray gx(numberOfComponents), gy(numberOfComponents), gz(numberOfComponents), gt(numberOfComponents);
    gx=1.;   gx(0)=.5; gx(1)=1.5;
    gy=.1;   gy(0)=.4; gy(1)= .5;
    if( numberOfDimensions==3 )
    {
      gz=1.;   gz(0)=.3; gz(1)=-.3;
    }
    else
    {
      gz=0.;
    }
    gt=1.;   gt(0)=.6; gt(1)=.35;
    trigTrue.setShifts(gx,gy,gz,gt);
  
    RealArray amp(numberOfComponents);
    amp=1.;
    amp(0)=.25;
    trigTrue.setAmplitudes(amp);

    RealArray cc(numberOfComponents);
    cc=1.;
    cc(0)=-.5;
    trigTrue.setConstants(cc);


    int degreeSpace = numberOfDimensions;
    int degreeTime = 1;
    OGPolyFunction polyTrue(degreeSpace,numberOfDimensions,numberOfComponents,degreeTime);

    RealArray spatialCoefficientsForTZ(5,5,5,numberOfComponents);  
    spatialCoefficientsForTZ=0.;
    RealArray timeCoefficientsForTZ(5,numberOfComponents);      
    timeCoefficientsForTZ=0.;
    int n;
    for( n=0; n<numberOfComponents; n++ )
    {
      real ni =1./(n+1);
      spatialCoefficientsForTZ(0,0,0,n)=1.;      
      if( degreeSpace>0 )
      {
	spatialCoefficientsForTZ(1,0,0,n)=1.*ni;
	spatialCoefficientsForTZ(0,1,0,n)=.5*ni;
	spatialCoefficientsForTZ(0,0,1,n)= numberOfDimensions==3 ? .25*ni : 0.;
      }
      if( degreeSpace>1 )
      {
	spatialCoefficientsForTZ(2,0,0,n)=.5*ni;
	spatialCoefficientsForTZ(0,2,0,n)=.25*ni;
	spatialCoefficientsForTZ(0,0,2,n)= numberOfDimensions==3 ? .125*ni : 0.;
      }
    }
    for( n=0; n<numberOfComponents; n++ )
    {
      for( int i=0; i<=4; i++ )
	timeCoefficientsForTZ(i,n)= i<=degreeTime ? 1./(i+1) : 0. ;
    }
    polyTrue.setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ ); 


    int tz=1;

    OGFunction & true = tz==0 ? (OGFunction&)polyTrue : (OGFunction&)trigTrue;

    n=0;      // only test first component

    enum
    {
      x,
      xx,
      laplacian,
      divScalarGrad,
      divScalarGradC,
      xsxu,
      xsxuC,
      xsyu,
      xsyuC,
      ysxu,
      ysxuC,
      xszu,
      xszuC,
      yszu,
      yszuC,
      zszu,
      zszuC,
      numberOfDerivatives
    };
      
    aString derivativeName[numberOfDerivatives]=
    {
      "x","xx","laplacian",
      "divScalarGrad","divScalarGrad (conservative)",
      "(su.x).x","(su.x).x (conservative)",
      "(su.y).x","(su.y).x (conservative)",
      "(su.x).y","(su.x).y (conservative)",
      "(su.x).z","(su.x).z (conservative)",
      "(su.y).z","(su.y).z (conservative)",
      "(su.z).z","(su.z).z (conservative)"
    };
  
  
    const int numberOfLevels=numberOfDimensions==2 ? 4 : 3;
    RealArray error(numberOfDerivatives,numberOfLevels,2);
    error=-1.;
  
    int l,axis;
    for( l=0; l<numberOfLevels; l++ )
    {
      printf("check level %i\n",l);
      if( l>0 )
      {
	for( axis=0; axis<map.getDomainDimension(); axis++ )
	  map.setGridDimensions(axis,map.getGridDimensions(axis)*2);
      }
    
      MappedGrid mg=map;
      mg.update();

      u.updateToMatchGrid(mg,all,all,all,Range(0,0));
      v.updateToMatchGrid(mg,all,all,all,Range(0,0));
    
      op.updateToMatchGrid(mg);
      u.setOperators(op);
    
    
      getIndex(mg.dimension(),I1,I2,I3);                                             // assign I1,I2,I3
      u(I1,I2,I3)=true(mg,I1,I2,I3,n,0.);

      scalar.updateToMatchGrid(mg,all,all,all);
      scalar.setOperators(op);
      scalar(I1,I2,I3)=1.+true(mg,I1,I2,I3,1,0.);

      // ---- compute all derivatives to 2nd and 4th order accuracy ----
      for( int order=0; order<=1; order++ )
      {
	int orderOfAccuracy=2*(order+1);
	op.setOrderOfAccuracy(orderOfAccuracy);

      
	getIndex(mg.dimension(),I1,I2,I3,-(order+1));  // reduce size for 2nd or 4th order

	error(x,l,order) = max(fabs(u.x(I1,I2,I3)(I1,I2,I3)-true.x(mg,I1,I2,I3,n)));

	error(xx,l,order) = max(fabs(u.xx(I1,I2,I3)(I1,I2,I3)-true.xx(mg,I1,I2,I3,n)));

	error(laplacian,l,order) =
	  max(fabs(u.laplacian(I1,I2,I3)(I1,I2,I3)-(true.xx(mg,I1,I2,I3,n)+true.yy(mg,I1,I2,I3,n))));

        for( int c=0; c<=1; c++ )
	{
	  if( c==1 && orderOfAccuracy==4 )
	    continue; // no fourth order conservative

	  op.useConservativeApproximations(c==1);
          if( numberOfDimensions==2 )
	  {
	    error(divScalarGrad+c,l,order)=max(fabs(u.divScalarGrad(scalar,I1,I2,I3)(I1,I2,I3)
						    -( (true.xx(mg,I1,I2,I3,0)+true.yy(mg,I1,I2,I3,0))*scalar(I1,I2,I3)
						       +scalar.x(I1,I2,I3)(I1,I2,I3)*true.x(mg,I1,I2,I3,0)
						       +scalar.y(I1,I2,I3)(I1,I2,I3)*true.y(mg,I1,I2,I3,0) )));
	  }
	  else
	  {
	    error(divScalarGrad+c,l,order)=
	      max(fabs(u.divScalarGrad(scalar,I1,I2,I3)(I1,I2,I3)
		       -( (true.xx(mg,I1,I2,I3,0)+true.yy(mg,I1,I2,I3,0)+true.zz(mg,I1,I2,I3,0))*scalar(I1,I2,I3)
			  +scalar.x(I1,I2,I3)(I1,I2,I3)*true.x(mg,I1,I2,I3,0)
			  +scalar.y(I1,I2,I3)(I1,I2,I3)*true.y(mg,I1,I2,I3,0) 
			  +scalar.z(I1,I2,I3)(I1,I2,I3)*true.z(mg,I1,I2,I3,0) )));
	  }
	  
	  error(xsxu+c,l,order)= max(fabs(u.derivativeScalarDerivative(scalar,0,0,I1,I2,I3)(I1,I2,I3)-
					  (true.xx(mg,I1,I2,I3,n)*scalar(I1,I2,I3)+
					   scalar.x(I1,I2,I3)(I1,I2,I3)*true.x(mg,I1,I2,I3,0)) ));

	  error(xsyu+c,l,order)= max(fabs(u.derivativeScalarDerivative(scalar,0,1,I1,I2,I3)(I1,I2,I3)-
					(true.xy(mg,I1,I2,I3,n)*scalar(I1,I2,I3)+
					 scalar.x(I1,I2,I3)(I1,I2,I3)*true.y(mg,I1,I2,I3,0)) ));

	  error(ysxu+c,l,order)= max(fabs(u.derivativeScalarDerivative(scalar,1,0,I1,I2,I3)(I1,I2,I3)-
					(true.xy(mg,I1,I2,I3,n)*scalar(I1,I2,I3)+
					 scalar.y(I1,I2,I3)(I1,I2,I3)*true.x(mg,I1,I2,I3,0)) ));
	  
          if( numberOfDimensions==3 )
	  {
	    error(xszu+c,l,order)= max(fabs(u.derivativeScalarDerivative(scalar,0,2,I1,I2,I3)(I1,I2,I3)-
					    (true.xz(mg,I1,I2,I3,n)*scalar(I1,I2,I3)+
					     scalar.x(I1,I2,I3)(I1,I2,I3)*true.z(mg,I1,I2,I3,0)) ));
	    error(yszu+c,l,order)= max(fabs(u.derivativeScalarDerivative(scalar,1,2,I1,I2,I3)(I1,I2,I3)-
					    (true.yz(mg,I1,I2,I3,n)*scalar(I1,I2,I3)+
					     scalar.y(I1,I2,I3)(I1,I2,I3)*true.z(mg,I1,I2,I3,0)) ));
	    error(zszu+c,l,order)= max(fabs(u.derivativeScalarDerivative(scalar,2,2,I1,I2,I3)(I1,I2,I3)-
					    (true.zz(mg,I1,I2,I3,n)*scalar(I1,I2,I3)+
					     scalar.z(I1,I2,I3)(I1,I2,I3)*true.z(mg,I1,I2,I3,0)) ));
	  }
	  
	}

	op.useConservativeApproximations(FALSE);
      
      }
    }
  
    printf("\n\n ********************** Test derivatives in %i dimensions ***********************\n",
	   numberOfDimensions);
    

    printf(" level   error(2)  ratio   error(4)   ratio   derivative  \n");
    for( int d=0; d<numberOfDerivatives; d++ )
    {
      if( error(d,0,0)>= 0. )
      {
	for( l=0; l<numberOfLevels; l++ )
	{
	  printf("   %i  %9.2e    %5.1f  ",l,error(d,l,0),(l==0 ? 0. : error(d,l-1,0)/max(REAL_MIN,error(d,l,0))));
          if( error(d,0,1)>= 0. )
	    printf(" %9.2e  %5.1f  %s\n", error(d,l,1),(l==0 ? 0. : error(d,l-1,1)/max(REAL_MIN,error(d,l,1))),
                      (const char*)derivativeName[d]);
          else
	    printf("                   %s\n", (const char*)derivativeName[d]);
	}
      }
    }
  }
  
  // cout << "Program Terminated Normally! \n";
  return 0;
}
