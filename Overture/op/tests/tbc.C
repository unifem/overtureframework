#include "Overture.h"
#include "CompositeGridOperators.h"
#include "NameList.h"
#include "OGTrigFunction.h"
#include "OGPolyFunction.h"
#include "display.h"
#include "Checker.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )
//  extern real timeForApplyBCneumann;
//  extern real timeToSetupBoundaryConditions;
//  extern real timeForAllBoundaryConditions;



#define getCornerIndex(is1,is2,is3) \
if( is1==0 )  \
{  \
  I1=Range(indexRange(0,axis1),indexRange(1,axis1)); \
  ng1a=0;  \
  ng1b=0;  \
}  \
else  \
{  \
  I1=Range(indexRange(side1,axis1),indexRange(side1,axis1)); \
  ng1a=1;  \
  ng1b=abs(indexRange(side1,axis1)-dimension(side1,axis1));  \
}  \
if( is2==0 )  \
{  \
  I2=Range(indexRange(0,axis2),indexRange(1,axis2)); \
  ng2a=0;  \
  ng2b=0;  \
}  \
else  \
{  \
  I2=Range(indexRange(side2,axis2),indexRange(side2,axis2)); \
  ng2a=1;  \
  ng2b=abs(indexRange(side2,axis2)-dimension(side2,axis2));  \
}  \
if( is3==0 )  \
{  \
  I3=Range(indexRange(0,axis3),indexRange(1,axis3)); \
  ng3a=0;  \
  ng3b=0;  \
}  \
else  \
{  \
  I3=Range(indexRange(side3,axis3),indexRange(side3,axis3)); \
  ng3a=1;  \
  ng3b=abs(indexRange(side3,axis3)-dimension(side3,axis3));  \
}  


#define setCornerMask(is1,is2,is3)\
getCornerIndex(is1,is2,is3); \
if( useWhereMask )\
{\
  where( mask(I1,I2,I3)!=0 ) \
  { \
   for( m3=ng3a; m3<=ng3b; m3++ ) \
   for( m2=ng2a; m2<=ng2b; m2++ ) \
   for( m1=ng1a; m1<=ng1b; m1++ ) \
     cornerMask(I1-m1*is1,I2-m2*is2,I3-m3*is3)=1; \
  }\
}\
else\
{\
   for( m3=ng3a; m3<=ng3b; m3++ ) \
   for( m2=ng2a; m2<=ng2b; m2++ ) \
   for( m1=ng1a; m1<=ng1b; m1++ ) \
     cornerMask(I1-m1*is1,I2-m2*is2,I3-m3*is3)=1; \
}



bool measureCPU=TRUE;

real
CPU()
// In this version of getCPU we can turn off the timing
{
  if( measureCPU )
    return getCPU();
  else
    return 0;
}

//================================================================================
//  **** Test the boundary conditions *****
//================================================================================

int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture
 
  const int maxNumberOfGridsToTest=3;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "square5", "cic", "sib" };
  int degreex=1;
  
  if( argc > 1 )
  { 
    int len;
    for( int i=1; i<argc; i++ )
    {
      aString arg = argv[i];
      if( arg=="-noTiming" )
        measureCPU=FALSE;
      else if( len=arg.matches("-degreex=") )
      {
        sScanF(arg(len,arg.length()-1),"%i",&degreex);
	printf("Setting degree of polynomial to %i\n",degreex);
      }
      else
      {
	numberOfGridsToTest=1;
	gridName[0]=argv[i];
      }
    }
  }
  else
    cout << "Usage: `tbc [-noTiming] [-degreex=] [<gridName>]' \n";


  aString checkFileName;
  if( REAL_EPSILON == DBL_EPSILON )
    checkFileName="tbc.dp.check.new";  // double precision
  else  
    checkFileName="tbc.sp.check.new";
  Checker checker(checkFileName);  // for saving a check file.
  real cutOff = REAL_EPSILON == DBL_EPSILON ? 4.e-12 : 3.e-4;
  checker.setCutOff(cutOff);

  real timeForNeumannBC=0.;

  Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
  Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];
  Index Ipv[3], &Ip1=Ipv[0], &Ip2=Ipv[1], &Ip3=Ipv[2];

  for( int it=0; it<numberOfGridsToTest; it++ )
  {
    aString nameOfOGFile=gridName[it];
    checker.setLabel(nameOfOGFile,0);

//     cout << "\n *****************************************************************\n";
//     cout << " ******** Checking grid: " << nameOfOGFile << " ************ \n";
//     cout << " *****************************************************************\n\n";

    CompositeGrid cg;
    getFromADataBase(cg,nameOfOGFile);

    // make some shorter names for readability
    BCTypes::BCNames 
      dirichlet                  = BCTypes::dirichlet,
      neumann                    = BCTypes::neumann,
      mixed                      = BCTypes::mixed,
      extrapolate                = BCTypes::extrapolate,
      normalComponent            = BCTypes::normalComponent,
      extrapolateNormalComponent = BCTypes::extrapolateNormalComponent,
      extrapolateTangentialComponent0 = BCTypes::extrapolateTangentialComponent0,
      extrapolateTangentialComponent1 = BCTypes::extrapolateTangentialComponent1,
      aDotU                      = BCTypes::aDotU,
      normalDotScalarGrad        = BCTypes::normalDotScalarGrad,
      generalizedDivergence      = BCTypes::generalizedDivergence,
      generalMixedDerivative     = BCTypes::generalMixedDerivative,
      aDotGradU                  = BCTypes::aDotGradU,
      vectorSymmetry             = BCTypes::vectorSymmetry,
      tangentialComponent        = BCTypes::tangentialComponent,
      tangentialComponent0       = BCTypes::tangentialComponent0,
      tangentialComponent1       = BCTypes::tangentialComponent1,
      normalDerivativeOfNormalComponent = BCTypes::normalDerivativeOfNormalComponent,
      normalDerivativeOfTangentialComponent0 = BCTypes::normalDerivativeOfTangentialComponent0,
      normalDerivativeOfTangentialComponent1 = BCTypes::normalDerivativeOfTangentialComponent1,
      allBoundaries              = BCTypes::allBoundaries,
      boundary1                  = BCTypes::boundary1; 

    // define an exact solution for testing
    // each component is a different polynomial of degree "degreex"
    int degreeSpace = degreex;
    int degreeTime = 1;
    int numberOfComponents = cg.numberOfDimensions();
    OGPolyFunction exact(degreeSpace,cg.numberOfDimensions(),numberOfComponents,degreeTime);

    RealArray spatialCoefficientsForTZ(6,6,6,numberOfComponents);  
    spatialCoefficientsForTZ=0.;
    RealArray timeCoefficientsForTZ(6,numberOfComponents);      
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
	spatialCoefficientsForTZ(0,0,1,n)= cg.numberOfDimensions()==3 ? .25*ni : 0.;
      }
      if( degreeSpace>1 )
      {
	spatialCoefficientsForTZ(2,0,0,n)=.5*ni;
	spatialCoefficientsForTZ(0,2,0,n)=.25*ni;
	spatialCoefficientsForTZ(0,0,2,n)= cg.numberOfDimensions()==3 ? .125*ni : 0.;
	spatialCoefficientsForTZ(1,1,0,n)=.125*ni;
	spatialCoefficientsForTZ(1,0,1,n)=-.125*ni;
	spatialCoefficientsForTZ(0,1,1,n)=.25*ni;
      }
      if( degreeSpace>2 )
      {
	spatialCoefficientsForTZ(3,0,0,n)=-.5*ni;
	spatialCoefficientsForTZ(0,3,0,n)=-.25*ni;
	spatialCoefficientsForTZ(0,0,3,n)= cg.numberOfDimensions()==3 ? -.125*ni : 0.;
        spatialCoefficientsForTZ(1,2,0,n)=-.125*ni;
        spatialCoefficientsForTZ(2,1,0,n)=.25*ni;
        spatialCoefficientsForTZ(0,1,2,n)=.125*ni;
        spatialCoefficientsForTZ(0,2,1,n)=-.25*ni;
        spatialCoefficientsForTZ(1,0,2,n)=.125*ni;
        spatialCoefficientsForTZ(2,0,1,n)=-.25*ni;
      }
      if( degreeSpace>3 )
      {
	spatialCoefficientsForTZ(4,0,0,n)=.25*ni;
	spatialCoefficientsForTZ(0,4,0,n)=.125*ni;
	spatialCoefficientsForTZ(0,0,4,n)= cg.numberOfDimensions()==3 ? .25*ni : 0.;

	spatialCoefficientsForTZ(2,2,0,n)=.125*ni;
	spatialCoefficientsForTZ(2,0,2,n)=-.25*ni;
	spatialCoefficientsForTZ(0,2,2,n)=.125*ni;
	spatialCoefficientsForTZ(3,1,0,n)=.25*ni;
	spatialCoefficientsForTZ(1,0,3,n)=-.25*ni;
	spatialCoefficientsForTZ(0,3,1,n)=.125*ni;
	spatialCoefficientsForTZ(1,3,0,n)=.25*ni;
      }
      if( degreeSpace>4 )
      {
	spatialCoefficientsForTZ(5,0,0,n)=.125*ni;
	spatialCoefficientsForTZ(0,5,0,n)=-.125*ni;
	spatialCoefficientsForTZ(0,0,5,n)= cg.numberOfDimensions()==3 ? .125*ni : 0.;
      }
    }
    for( n=0; n<numberOfComponents; n++ )
    {
      for( int i=0; i<=4; i++ )
	timeCoefficientsForTZ(i,n)= i<=degreeTime ? 1./(i+1) : 0. ;
    }
    exact.setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ ); 

    real error=0., worstError=0.;
    real time,time1,time2;
    aString buff;
    
    CompositeGridOperators cgop(cg);

    // loop over all component grids
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
//      cout << "+++++++Checking component grid = " << grid << "+++++++" << endl;

      MappedGrid & mg = cg[grid]; 
      checker.setLabel(mg.getName(),1);
      checker.setLabel("std",2);
      checker.setLabel("",3);


      mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEvertexBoundaryNormal | 
                MappedGrid::THEmask | MappedGrid::THEcenterBoundaryTangent);
      const intArray & mask = mg.mask();
    
      // mg.boundaryCondition().display("Here is mg.boundaryCondition()");


      Range all;
      realMappedGridFunction u(mg),v(mg,all,all,all,numberOfComponents), w(mg); // define some component grid functions

      // MappedGridOperators operators(mg);                     // define some differential operators
      MappedGridOperators & operators=cgop[grid];
      u.setOperators( operators );                           // Tell u which operators to use
      v.setOperators( operators );                 

      u=-77.;
      Index I1,I2,I3, Ia1,Ia2,Ia3;
      getIndex(mg.extendedIndexRange(),I1,I2,I3);  
      getIndex(mg.dimension(),Ia1,Ia2,Ia3);
      u(I1,I2,I3)=exact(mg,I1,I2,I3,0,0.);



      int side,axis;
/* ---
    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
      for( side=Start; side<=End; side++ )
      {
	if( mg.boundaryCondition()(side,axis)>0 )
	  display(mg.vertexBoundaryNormal(side,axis),"vertexBoundaryNormal");
      }
--- */
    
    // ****************************************************************
    //       dirichlet 
    // ****************************************************************
      int component=0;
      real value=1.;
      time=CPU();
      u.applyBoundaryCondition(component,dirichlet,allBoundaries,value);

      // u.display("Before finishBoundaryConditions");
      time=CPU()-time;
      u.finishBoundaryConditions();
      // u.display("After finishBoundaryConditions");

      error=0.;
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  where( mask(Ib1,Ib2,Ib3)>0 )
	    error=max(error,max(abs(u(Ib1,Ib2,Ib3)-value)));    
	}
      }
      worstError=max(worstError,error);
      // printf("Maximum error in dirichlet                         = %8.2e, cpu=%8.2e \n",error,time);  
      checker.printMessage("dirichlet",error,time);

      // ****************************************************************
      //       neumann
      // ****************************************************************
      u=-77.;
      u(I1,I2,I3)=exact(mg,I1,I2,I3,0,0.);
      value=-1.; // set u.n=value
      // u.display("Here is u before neumann BC");
      time=CPU();
      u.applyBoundaryCondition(component,neumann,allBoundaries,value);
      time1=CPU()-time;
      time=CPU();
      u.applyBoundaryCondition(component,neumann,allBoundaries,value);
      time=CPU()-time;
      u.finishBoundaryConditions();
      error=0.;
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  realArray & normal = mg.vertexBoundaryNormal(side,axis);
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  if( mg.numberOfDimensions()==1 )
	    w(Ib1,Ib2,Ib3)=normal(Ib1,Ib2,Ib3,0)*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)-value;
	  else if( mg.numberOfDimensions()==2 )
	    w(Ib1,Ib2,Ib3)= normal(Ib1,Ib2,Ib3,0)*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)
	      +normal(Ib1,Ib2,Ib3,1)*u.y(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)-value;
	  else
	    w(Ib1,Ib2,Ib3)= normal(Ib1,Ib2,Ib3,0)*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)
	      +normal(Ib1,Ib2,Ib3,1)*u.y(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)
	      +normal(Ib1,Ib2,Ib3,2)*u.z(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)-value;

	  where( mask(Ib1,Ib2,Ib3)>0 )
	    error=max(error,max(abs(w(Ib1,Ib2,Ib3))));
	}
      }
      worstError=max(worstError,error);
      // printf("Maximum error in neumann                           = %8.2e, cpu=%8.2e(init), %8.2e \n",error,time1,time);     
      checker.printMessage("neumann",error,time,time1);
      timeForNeumannBC+=time+time1;
      //printf("***time for neumann BC = %8.2e(neumann.C) timeForNeumannBC=%8.2e\n",
      //       GenericMappedGridOperators::timeForNeumann,timeForNeumannBC);

      // u.display("Here is u after a neumann BC");


      BoundaryConditionParameters bcParams;

      // ****************************************************************
      //     VARIABLE COEFFICIENT NEUMANN or ROBIN
      // ****************************************************************    

      bcParams.setVariableCoefficientOption(  BoundaryConditionParameters::spatiallyVaryingCoefficients );

      getIndex(mg.gridIndexRange(),I1,I2,I3);
      // varCoeff only needs to be allocated on the boundary but do this so we can assign all boundaries:
      RealArray varCoeff(I1,I2,I3,2);  // holds variable coefficients
      bcParams.setVariableCoefficientsArray( &varCoeff );        

      OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),x);
      varCoeff(I1,I2,I3,0)=1.+ .1*x(I1,I2,I3,0) - .1*x(I1,I2,I3,1);
      varCoeff(I1,I2,I3,1)=2. + .1*SQR(x(I1,I2,I3,0)) + .05*SQR(x(I1,I2,I3,1));  // this value must not be zero

      u=-77.;
      u(I1,I2,I3)=exact(mg,I1,I2,I3,0,0.);
      value=-1.; // RHS
      // u.display("Here is u before neumann BC");
      time=CPU();
      u.applyBoundaryCondition(component,mixed,allBoundaries,value,0.,bcParams);
      time1=CPU()-time;
      time=CPU();
      u.applyBoundaryCondition(component,mixed,allBoundaries,value,0.,bcParams);
      time=CPU()-time;
      u.finishBoundaryConditions();
      error=0.;
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  realArray & normal = mg.vertexBoundaryNormal(side,axis);
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  if( mg.numberOfDimensions()==1 )
	    w(Ib1,Ib2,Ib3)=varCoeff(Ib1,Ib2,Ib3,1)*normal(Ib1,Ib2,Ib3,0)*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)
	      +varCoeff(Ib1,Ib2,Ib3,0)*u(Ib1,Ib2,Ib3) -value;
	  else if( mg.numberOfDimensions()==2 )
	    w(Ib1,Ib2,Ib3)= varCoeff(Ib1,Ib2,Ib3,1)*( normal(Ib1,Ib2,Ib3,0)*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)
						      +normal(Ib1,Ib2,Ib3,1)*u.y(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3))
                      +varCoeff(Ib1,Ib2,Ib3,0)*u(Ib1,Ib2,Ib3) -value;
	  else
	    w(Ib1,Ib2,Ib3)= varCoeff(Ib1,Ib2,Ib3,1)*( normal(Ib1,Ib2,Ib3,0)*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)
	      +normal(Ib1,Ib2,Ib3,1)*u.y(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)
						      +normal(Ib1,Ib2,Ib3,2)*u.z(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3) )
                    +varCoeff(Ib1,Ib2,Ib3,0)*u(Ib1,Ib2,Ib3) -value;

	  where( mask(Ib1,Ib2,Ib3)>0 )
	    error=max(error,max(abs(w(Ib1,Ib2,Ib3))));
	}
      }
      worstError=max(worstError,error);
      // printF("Maximum error in neumann (varcoeff)                  = %8.2e, cpu=%8.2e(init), %8.2e \n",error,time1,time);     
      checker.printMessage("mixed (var-coeff)",error,time,time1);
      timeForNeumannBC+=time+time1;


      // reset:
      bcParams.setVariableCoefficientsArray( NULL ); 
      bcParams.setVariableCoefficientOption( BoundaryConditionParameters::spatiallyConstantCoefficients );


      // ****************************************************************
      //       mixed
      // ****************************************************************
      bcParams.a.redim(3); bcParams.a=0.;
      for( int mm=0; mm<2; mm++ )
      {
	real a0=1., a1=mm+1.;   // test changing the coefficients 
	bcParams.a(0)=a0, bcParams.a(1)=a1;

	u=-77.;
	u(I1,I2,I3)=exact(mg,I1,I2,I3,0,0.);
	value=-1.; // set u.n=value
	// u.display("Here is u before mixed BC");
	time=CPU();
	u.applyBoundaryCondition(component,mixed,allBoundaries,value,0.,bcParams);
	time1=CPU()-time;
	time=CPU();
	u.applyBoundaryCondition(component,mixed,allBoundaries,value,0.,bcParams);
	time=CPU()-time;
	u.finishBoundaryConditions();
	error=0.;
	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    realArray & normal = mg.vertexBoundaryNormal(side,axis);
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    if( mg.numberOfDimensions()==1 )
	      w(Ib1,Ib2,Ib3)=a0*u(Ib1,Ib2,Ib3) + a1*normal(Ib1,Ib2,Ib3,0)*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)-value;
	    else if( mg.numberOfDimensions()==2 )
	      w(Ib1,Ib2,Ib3)= a1*(normal(Ib1,Ib2,Ib3,0)*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)+
				  normal(Ib1,Ib2,Ib3,1)*u.y(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3))+a0*u(Ib1,Ib2,Ib3) -value;
	    else
	      w(Ib1,Ib2,Ib3)= a1*(normal(Ib1,Ib2,Ib3,0)*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)+
				  normal(Ib1,Ib2,Ib3,1)*u.y(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)+
				  normal(Ib1,Ib2,Ib3,2)*u.z(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)) +a0*u(Ib1,Ib2,Ib3) -value;

	    where( mask(Ib1,Ib2,Ib3)>0 )
	      error=max(error,max(abs(w(Ib1,Ib2,Ib3))));
	  }
	}
	worstError=max(worstError,error);
	// printf("Maximum error in mixed                           = %8.2e, cpu=%8.2e(init), %8.2e \n",error,time1,time);     
	checker.printMessage(sPrintF("mixed%i",mm+1),error,time,time1);
      }

      // ****************************************************************
      //       aDotGradU
      // ****************************************************************
      bcParams.a.redim(3); bcParams.a=0.;
      if( false ) // finish checking this ************
      {
	for( int mm=0; mm<2; mm++ )
	{
	  real a0=1.23, a1=mm+.756, a2=.5+mm;   // test changing the coefficients 
	  bcParams.a(0)=a0, bcParams.a(1)=a1, bcParams.a(2)=a2;

	  u=-77.;
	  u(I1,I2,I3)=exact(mg,I1,I2,I3,0,0.);
	  value=0.; // set u.n=value
	  // u.display("Here is u before aDotGradU BC");
	  time=CPU();
	  u.applyBoundaryCondition(component,aDotGradU,allBoundaries,value,0.,bcParams);
	  time1=CPU()-time;
	  time=CPU();
	  u.applyBoundaryCondition(component,aDotGradU,allBoundaries,value,0.,bcParams);
	  time=CPU()-time;
	  u.finishBoundaryConditions();
	  error=0.;
	  ForBoundary(side,axis)
	  {
	    if( mg.boundaryCondition()(side,axis) > 0 )
	    {
	      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	      if( mg.numberOfDimensions()==1 )
		w(Ib1,Ib2,Ib3)=a0*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)-value;
	      else if( mg.numberOfDimensions()==2 )
		w(Ib1,Ib2,Ib3)= (a0*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)+
				 a1*u.y(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)) -value;
	      else
		w(Ib1,Ib2,Ib3)= (a0*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)+
				 a1*u.y(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)+
				 a2*u.z(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)) -value;

	      where( mask(Ib1,Ib2,Ib3)>0 )
		error=max(error,max(abs(w(Ib1,Ib2,Ib3))));
	    }
	  }
	  worstError=max(worstError,error);
	  // printf("Maximum error in aDotGradU                           = %8.2e, cpu=%8.2e(init), %8.2e \n",error,time1,time);     
	  checker.printMessage(sPrintF("aDotGradU%i",mm+1),error,time,time1);
	}
      }
      
      realMappedGridFunction scalar(mg);
      scalar=1.+mg.vertex()(all,all,all,0); // +mg.vertex()(all,all,all,1);
      scalar=2.;


      for( int c=0; c<=1; c++ )
      {
	operators.useConservativeApproximations(c==0);
	if( c==0 )
	  checker.setLabel("con",2);
	else
	  checker.setLabel("std",2);

        // ****************************************************************
        //       normalDotScalarGrad
        // ****************************************************************
	u=-77.;
	u(Ia1,Ia2,Ia3)=exact(mg,Ia1,Ia2,Ia3,0,0.);  // *** may need an extra line of values ***
	value=-1.; // set u.n=value
	// value=0.; // set u.n=value
	// u.display("Here is u before neumann BC");
	bcParams.setVariableCoefficients(scalar);
      

	time=CPU();
	u.applyBoundaryCondition(component,normalDotScalarGrad,allBoundaries,value,0.,bcParams);
	time1=CPU()-time;
	u(Ia1,Ia2,Ia3)=exact(mg,Ia1,Ia2,Ia3,0,0.);  // *** may need an extra line of values ***
	time=CPU();
        u.applyBoundaryCondition(component,normalDotScalarGrad,allBoundaries,value,0.,bcParams);
	time=CPU()-time;
        u.finishBoundaryConditions();
	error=0.;
	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition(side,axis) > 0 )
	  {
	    realArray & normal = mg.vertexBoundaryNormal(side,axis);
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    if( mg.numberOfDimensions()==1 )
	      w(Ib1,Ib2,Ib3)=scalar(Ib1,Ib2,Ib3)*normal(Ib1,Ib2,Ib3,0)*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)-value;
	    else if( mg.numberOfDimensions()==2 )
	      w(Ib1,Ib2,Ib3)=scalar(Ib1,Ib2,Ib3)*(normal(Ib1,Ib2,Ib3,0)*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)+
						  normal(Ib1,Ib2,Ib3,1)*u.y(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3))-value;
	    else
	      w(Ib1,Ib2,Ib3)= scalar(Ib1,Ib2,Ib3)*(normal(Ib1,Ib2,Ib3,0)*u.x(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)+
						   normal(Ib1,Ib2,Ib3,1)*u.y(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3)+
						   normal(Ib1,Ib2,Ib3,2)*u.z(Ib1,Ib2,Ib3)(Ib1,Ib2,Ib3))-value;

	    // display(w(Ib1,Ib2,Ib3),"Error in normalDotScalarGrad on a side");

	    where( mask(Ib1,Ib2,Ib3)>0 )
	      error=max(error,max(abs(w(Ib1,Ib2,Ib3))));
	  }
	}
	worstError=max(worstError,error);
	// printf("Maximum error in normalDotScalarGrad               = %8.2e, cpu=%8.2e(init), %8.2e \n",error,time1,time); 
        checker.printMessage("normalDotScalarGrad",error,time,time1);
	// display(u,"Here is u after a normalDotScalarGrad BC");

      } // end for c
      

      // ****************************************************************
      //       normalComponent
      // ****************************************************************
      v=-77.;
      Range C(0,numberOfComponents-1);
      v(I1,I2,I3,C)=exact(mg,I1,I2,I3,C,0.);
      value=2.; // set n.u = value
      time=CPU();
      v.applyBoundaryCondition(C,normalComponent,allBoundaries,value);
      time1=CPU()-time;
      time=CPU();
      v.applyBoundaryCondition(C,normalComponent,allBoundaries,value);
      time=CPU()-time;
      v.finishBoundaryConditions();
      error=0.;
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  realArray & normal = mg.vertexBoundaryNormal(side,axis);
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
          where( mask(Ib1,Ib2,Ib3)!=0 )
	  {
	    if( mg.numberOfDimensions()==1 )
	      error=max(error,max(abs(
		normal(Ib1,Ib2,Ib3,0)*v(Ib1,Ib2,Ib3,0)-value
		)));    
	    else if( mg.numberOfDimensions()==2 )
	    {
	      error=max(error,max(abs(
		normal(Ib1,Ib2,Ib3,0)*v(Ib1,Ib2,Ib3,0)
		+normal(Ib1,Ib2,Ib3,1)*v(Ib1,Ib2,Ib3,1)-value
		)));    

	      //  ::display(normal(Ib1,Ib2,Ib3,0)*v(Ib1,Ib2,Ib3,0)+normal(Ib1,Ib2,Ib3,1)*v(Ib1,Ib2,Ib3,1),"normalComponent");
	    
	    }
	    else
	      error=max(error,max(abs(
		normal(Ib1,Ib2,Ib3,0)*v(Ib1,Ib2,Ib3,0)
		+normal(Ib1,Ib2,Ib3,1)*v(Ib1,Ib2,Ib3,1)
		+normal(Ib1,Ib2,Ib3,2)*v(Ib1,Ib2,Ib3,2)-value
		)));    
	  }
	}
	
      }
      worstError=max(worstError,error);
      // printf("Maximum error in normalComponent      = %8.2e, cpu=%8.2e(init), %8.2e \n",error,time1,time);   
      checker.printMessage("normalComponent",error,time,time1);

      // ****************************************************************
      //       tangentialComponent
      //  ***warning: Corner values are wrong since we cannot set the value
      //     correctly in both directions!
      // ****************************************************************
      v=-77.;
      v(I1,I2,I3,C)=exact(mg,I1,I2,I3,C,0.);
      value=0.; // set u = (n.u)n + value
      time=CPU();
      v.applyBoundaryCondition(C,tangentialComponent,allBoundaries,value);
      time=CPU()-time;
      v.finishBoundaryConditions();
      // v.display("Here is v after tangentialComponent");
    
      error=0.;
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  realArray & normal = mg.vertexBoundaryNormal(side,axis);
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,-1); // don't check corners!
	  if( mg.numberOfDimensions()==1 )
	  {
	    error=max(error,max(
	      abs(v(Ib1,Ib2,Ib3,0)-
		  (normal(Ib1,Ib2,Ib3,0)*exact(mg,Ib1,Ib2,Ib3,0,0.))*normal(Ib1,Ib2,Ib3,0)-value)
	      ));
	  }
	  else if( mg.numberOfDimensions()==2 )
	  {
	    error=max(error,max(
	      abs(v(Ib1,Ib2,Ib3,0)-
		  (normal(Ib1,Ib2,Ib3,0)*exact(mg,Ib1,Ib2,Ib3,0,0.)
		   +normal(Ib1,Ib2,Ib3,1)*exact(mg,Ib1,Ib2,Ib3,1,0.))*normal(Ib1,Ib2,Ib3,0)-value)
	      +abs(v(Ib1,Ib2,Ib3,1)-
		   (normal(Ib1,Ib2,Ib3,0)*exact(mg,Ib1,Ib2,Ib3,0,0.)
		    +normal(Ib1,Ib2,Ib3,1)*exact(mg,Ib1,Ib2,Ib3,1,0.))*normal(Ib1,Ib2,Ib3,1)-value)
	      ));
	  }
	  else
	    error=max(error,max(
	      abs(v(Ib1,Ib2,Ib3,0)-
		  (normal(Ib1,Ib2,Ib3,0)*exact(mg,Ib1,Ib2,Ib3,0,0.)
		   +normal(Ib1,Ib2,Ib3,1)*exact(mg,Ib1,Ib2,Ib3,1,0.)
		   +normal(Ib1,Ib2,Ib3,2)*exact(mg,Ib1,Ib2,Ib3,2,0.))*normal(Ib1,Ib2,Ib3,0)-value)
	      +abs(v(Ib1,Ib2,Ib3,1)-
		   (normal(Ib1,Ib2,Ib3,0)*exact(mg,Ib1,Ib2,Ib3,0,0.)
		    +normal(Ib1,Ib2,Ib3,1)*exact(mg,Ib1,Ib2,Ib3,1,0.)
		    +normal(Ib1,Ib2,Ib3,2)*exact(mg,Ib1,Ib2,Ib3,2,0.))*normal(Ib1,Ib2,Ib3,1)-value)
	      +abs(v(Ib1,Ib2,Ib3,2)-
		   (normal(Ib1,Ib2,Ib3,0)*exact(mg,Ib1,Ib2,Ib3,0,0.)
		    +normal(Ib1,Ib2,Ib3,1)*exact(mg,Ib1,Ib2,Ib3,1,0.)
		    +normal(Ib1,Ib2,Ib3,2)*exact(mg,Ib1,Ib2,Ib3,2,0.))*normal(Ib1,Ib2,Ib3,2)-value)
	      ));
	}
      }
      worstError=max(worstError,error);
      // printf("Maximum error in tangentialComponent               = %8.2e, cpu=%8.2e \n",error,time);   
      checker.printMessage("tangentialComponent",error,time,time1);

      // ****************************************************************
      //       vectorSymmetry
      // ****************************************************************
      v=-77.;
      Range Rd(0,mg.numberOfDimensions()-1);
      v(I1,I2,I3,Rd)=exact(mg,I1,I2,I3,Rd,0.);
      // v.display("v before vectorSymmetry");
    
      time=CPU();
      v.applyBoundaryCondition(C,vectorSymmetry,allBoundaries);
      time=CPU()-time;
      v.finishBoundaryConditions();
      // v.display("v after vectorSymmetry");
      error=0.;
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  realArray & normal = mg.vertexBoundaryNormal(side,axis);
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,+1);
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ip1,Ip2,Ip3,-1);
	  if( mg.numberOfDimensions()==1 )
	  {
	    // normal components should be odd: ( v(-1) = 2*v(0)-v(1)
	    error=max(error,max(abs(
	      normal(Ib1,Ib2,Ib3,0)*(v(Ip1,Ip2,Ip3,0)-2.*v(Ib1,Ib2,Ib3,0)+v(Ig1,Ig2,Ig3,0))
              )));    
	  }
	  else if( mg.numberOfDimensions()==2 )
	  {
	    // normal components should be odd: ( v(-1) = 2*v(0)-v(1)
	    error=max(error,max(abs(
	      normal(Ib1,Ib2,Ib3,0)*(v(Ip1,Ip2,Ip3,0)-2.*v(Ib1,Ib2,Ib3,0)+v(Ig1,Ig2,Ig3,0))
	      +normal(Ib1,Ib2,Ib3,1)*(v(Ip1,Ip2,Ip3,1)-2.*v(Ib1,Ib2,Ib3,1)+v(Ig1,Ig2,Ig3,1))
              )));    
	    // tangential components should be even:
	    // (subtract off the normal component)
	    error=max(error,max(abs(
	      (v(Ip1,Ip2,Ip3,0)-v(Ig1,Ig2,Ig3,0))
	      - (normal(Ib1,Ib2,Ib3,0)*(v(Ip1,Ip2,Ip3,0)-v(Ig1,Ig2,Ig3,0))
		 +normal(Ib1,Ib2,Ib3,1)*(v(Ip1,Ip2,Ip3,1)-v(Ig1,Ig2,Ig3,1)))*normal(Ib1,Ib2,Ib3,0)
	      +(v(Ip1,Ip2,Ip3,1)-v(Ig1,Ig2,Ig3,1))
	      - (normal(Ib1,Ib2,Ib3,0)*(v(Ip1,Ip2,Ip3,0)-v(Ig1,Ig2,Ig3,0))
		 +normal(Ib1,Ib2,Ib3,1)*(v(Ip1,Ip2,Ip3,1)-v(Ig1,Ig2,Ig3,1)))*normal(Ib1,Ib2,Ib3,1)
              )));    
	  }
	  else
	  {
	    // normal components should be odd: ( v(-1) = 2*v(0)-v(1)
	    error=max(error,max(abs(
	      normal(Ib1,Ib2,Ib3,0)*(v(Ip1,Ip2,Ip3,0)-2.*v(Ib1,Ib2,Ib3,0)+v(Ig1,Ig2,Ig3,0))
	      +normal(Ib1,Ib2,Ib3,1)*(v(Ip1,Ip2,Ip3,1)-2.*v(Ib1,Ib2,Ib3,1)+v(Ig1,Ig2,Ig3,1))
	      +normal(Ib1,Ib2,Ib3,2)*(v(Ip1,Ip2,Ip3,2)-2.*v(Ib1,Ib2,Ib3,2)+v(Ig1,Ig2,Ig3,2))
              )));    
	    // tangential components should be even:
	    // (subtract off the normal component)
	    error=max(error,max(abs(
	      (v(Ip1,Ip2,Ip3,0)-v(Ig1,Ig2,Ig3,0))
	      - (normal(Ib1,Ib2,Ib3,0)*(v(Ip1,Ip2,Ip3,0)-v(Ig1,Ig2,Ig3,0))
		 +normal(Ib1,Ib2,Ib3,1)*(v(Ip1,Ip2,Ip3,1)-v(Ig1,Ig2,Ig3,1))
		 +normal(Ib1,Ib2,Ib3,2)*(v(Ip1,Ip2,Ip3,2)-v(Ig1,Ig2,Ig3,2)))*normal(Ib1,Ib2,Ib3,0)
	      +(v(Ip1,Ip2,Ip3,1)-v(Ig1,Ig2,Ig3,1))
	      - (normal(Ib1,Ib2,Ib3,0)*(v(Ip1,Ip2,Ip3,0)-v(Ig1,Ig2,Ig3,0))
		 +normal(Ib1,Ib2,Ib3,1)*(v(Ip1,Ip2,Ip3,1)-v(Ig1,Ig2,Ig3,1))
		 +normal(Ib1,Ib2,Ib3,2)*(v(Ip1,Ip2,Ip3,2)-v(Ig1,Ig2,Ig3,2)))*normal(Ib1,Ib2,Ib3,1)
	      +(v(Ip1,Ip2,Ip3,2)-v(Ig1,Ig2,Ig3,2))
	      - (normal(Ib1,Ib2,Ib3,0)*(v(Ip1,Ip2,Ip3,0)-v(Ig1,Ig2,Ig3,0))
		 +normal(Ib1,Ib2,Ib3,1)*(v(Ip1,Ip2,Ip3,1)-v(Ig1,Ig2,Ig3,1))
		 +normal(Ib1,Ib2,Ib3,2)*(v(Ip1,Ip2,Ip3,2)-v(Ig1,Ig2,Ig3,2)))*normal(Ib1,Ib2,Ib3,2)
              )));    
	  }
	}
      }
      worstError=max(worstError,error);
      // printf("Maximum error in vectorSymmetry                    = %8.2e, cpu=%8.2e \n",error,time);   
      checker.printMessage("vectorSymmetry",error,time,time1);

      // ****************************************************************
      // apply BC: aDotU
      // ****************************************************************
      v=-77.;
      v(I1,I2,I3,Rd)=exact(mg,I1,I2,I3,Rd,0.);
      RealArray & a = bcParams.a;
      a.redim(3);
      a(0)=1.;
      a(1)=2.;
      a(2)=3.;
      value=3.;
      time=CPU();
      v.applyBoundaryCondition(C,aDotU,allBoundaries,value,0.,bcParams);   // a.(u_0,u_1)=value
      time=CPU()-time;
      v.finishBoundaryConditions();
      error=0.;
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  realArray & normal = mg.vertexBoundaryNormal(side,axis);
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  if( mg.numberOfDimensions()==1 )
	    error=max(error,max(abs(
	      a(0)*v(Ib1,Ib2,Ib3,0)-value
              )));    
	  else if( mg.numberOfDimensions()==2 )
	    error=max(error,max(abs(
	      a(0)*v(Ib1,Ib2,Ib3,0)
	      +a(1)*v(Ib1,Ib2,Ib3,1)-value
              )));    
	  else
	    error=max(error,max(abs(
	      a(0)*v(Ib1,Ib2,Ib3,0)
	      +a(1)*v(Ib1,Ib2,Ib3,1)
	      +a(2)*v(Ib1,Ib2,Ib3,2)-value
              )));    
	}
      }
      worstError=max(worstError,error);
      // printf("Maximum error in aDotU                             = %8.2e, cpu=%8.2e \n",error,time);   
      checker.printMessage("aDotU",error,time,time1);

      // ****************************************************************
      //    generalizedDivergence
      // ****************************************************************
      v=-77.;
      v(Ia1,Ia2,Ia3,Rd)=exact(mg,Ia1,Ia2,Ia3,Rd,0.);
      bcParams.a.redim(3);
      bcParams.a(0)=2.;
      bcParams.a(1)=3.;
      bcParams.a(2)=4.;
      value=1.;

      time=CPU();
      v.applyBoundaryCondition(C,generalizedDivergence,allBoundaries,value,0.,bcParams); 
      time1=CPU()-time;

    // call with default parameter values
      time=CPU();
      v.applyBoundaryCondition(C,generalizedDivergence,allBoundaries,value,0.); 
      time2=CPU()-time;

      time=CPU();
      v.applyBoundaryCondition(C,generalizedDivergence,allBoundaries,value,0.,bcParams); 
      time=CPU()-time;

      v.finishBoundaryConditions();
      error=0.;
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
          realArray berr(Ib1,Ib2,Ib3);
	  if( mg.numberOfDimensions()==1 )
	    berr=a(0)*v.x(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0) -value;
	  else if( mg.numberOfDimensions()==2 )
	    berr=(a(0)*v.x(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0)
		  +a(1)*v.y(Ib1,Ib2,Ib3,1)(Ib1,Ib2,Ib3,1) -value);
	  else
	    berr=(a(0)*v.x(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0)
		  +a(1)*v.y(Ib1,Ib2,Ib3,1)(Ib1,Ib2,Ib3,1)
		  +a(2)*v.z(Ib1,Ib2,Ib3,2)(Ib1,Ib2,Ib3,2) -value);
          where( mask(Ib1,Ib2,Ib3)>0 )  // *wdh* 061025 -- generalizedDivergence only computed here now
	  {
	    error=max(error,max(fabs(berr)));
	  }
	}
      }
      worstError=max(worstError,error);
//       printf("Maximum error in generalizedDivergence             = %8.2e," 
// 	     " cpu=%8.2e(init), %8.2e(equal), %8.2e(unequal) \n",error,time1,time2,time);   

      checker.printMessage("generalizedDivergence",error,time,time1);

      // ****************************************************************
      //    generalMixedDerivative
      // ****************************************************************
      v=-77.;
      v(Ia1,Ia2,Ia3,Rd)=exact(mg,Ia1,Ia2,Ia3,Rd,0.);
      bcParams.a.redim(4);
      bcParams.a(0)=1.;
      bcParams.a(1)=2.;
      bcParams.a(2)=3.;
      bcParams.a(3)=4.;
      value=1.;
      C=Rd;
      time=CPU();
      v.applyBoundaryCondition(C,generalMixedDerivative,allBoundaries,value,0.,bcParams); 
      time1=CPU()-time;
      time=CPU();
      v.applyBoundaryCondition(C,generalMixedDerivative,allBoundaries,value,0.,bcParams); 
      time=CPU()-time;
      v.finishBoundaryConditions();
      error=0.;
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  if( mg.numberOfDimensions()==1 )
	    error=max(error,max(abs(
	      a(0)*v(Ib1,Ib2,Ib3,C)
	      +a(1)*v.x(Ib1,Ib2,Ib3,C)(Ib1,Ib2,Ib3,C) -value
              )));    
	  else if( mg.numberOfDimensions()==2 )
	    error=max(error,max(abs(
	      a(0)*v(Ib1,Ib2,Ib3,C)
	      +a(1)*v.x(Ib1,Ib2,Ib3,C)(Ib1,Ib2,Ib3,C)
	      +a(2)*v.y(Ib1,Ib2,Ib3,C)(Ib1,Ib2,Ib3,C) -value
              )));    
	  else
	    error=max(error,max(abs(
	      a(0)*v(Ib1,Ib2,Ib3,C)
	      +a(1)*v.x(Ib1,Ib2,Ib3,C)(Ib1,Ib2,Ib3,C)
	      +a(2)*v.y(Ib1,Ib2,Ib3,C)(Ib1,Ib2,Ib3,C)
	      +a(3)*v.z(Ib1,Ib2,Ib3,C)(Ib1,Ib2,Ib3,C) -value
              )));    
	}
      }
      worstError=max(worstError,error);
      // printf("Maximum error in generalMixedDerivative       = %8.2e, cpu=%8.2e(init), %8.2e \n",error,time1,time);
      checker.printMessage("generalMixedDerivative",error,time,time1);

      // ****************************************************************
      //       extrapolateInterpolationNeighbours
      // ****************************************************************
      getIndex(mg.extendedIndexRange(),I1,I2,I3,1);

      u=99.;
      where( mg.mask()(I1,I2,I3)!=0 )
	u(I1,I2,I3,0)=exact(mg,I1,I2,I3,0,0.);

    // u.display("u before extrapolateInterpolationNeighbour");
      time=CPU();
      u.applyBoundaryCondition(component,BCTypes::extrapolateInterpolationNeighbours);
      time1=CPU()-time;
      time=CPU();
      u.applyBoundaryCondition(component,BCTypes::extrapolateInterpolationNeighbours);
      time=CPU()-time;
      u.finishBoundaryConditions();
      // u.display("u after extrapolateInterpolationNeighbour");

      getIndex(mg.extendedIndexRange(),I1,I2,I3,1);  // include 1 ghost line
      Index I1p = I1.getBound()<mg.dimension()(End,axis1) ? Range(I1+1) : Range(I1); 
      Index I2p = I2.getBound()<mg.dimension()(End,axis2) ? Range(I2+1) : Range(I2); 
      Index I3p = I3.getBound()<mg.dimension()(End,axis3) ? Range(I3+1) : Range(I3); 

      Index I1m = I1.getBase()>mg.dimension()(Start,axis1) ? Range(I1-1) : Range(I1); 
      Index I2m = I2.getBase()>mg.dimension()(Start,axis2) ? Range(I2-1) : Range(I2); 
      Index I3m = I3.getBase()>mg.dimension()(Start,axis3) ? Range(I3-1) : Range(I3); 

      error=0.;
      if( mg.numberOfDimensions()==1 )
	where( mg.mask()(I1p,I2,I3)<0 || mg.mask()(I1m,I2,I3)<0 )
	  error=max(error,max(abs(u(I1,I2,I3)-exact(mg,I1,I2,I3,0,0.))));
      else if( mg.numberOfDimensions()==2 )
	where(mg.mask()(I1p,I2 ,I3)<0 || mg.mask()(I1m,I2 ,I3)<0 ||
	      mg.mask()(I1 ,I2p,I3)<0 || mg.mask()(I1 ,I2m,I3)<0 ||
              mg.mask()(I1p,I2p,I3)<0 || mg.mask()(I1m,I2p,I3)<0 ||
              mg.mask()(I1p,I2m,I3)<0 || mg.mask()(I1m,I2m,I3)<0 ) 
	  error=max(error,max(abs(u(I1,I2,I3)-exact(mg,I1,I2,I3,0,0.))));
      else if( mg.numberOfDimensions()==3 )
	where(mg.mask()(I1m,I2m,I3m)<0 || mg.mask()(I1 ,I2m,I3m)<0 || mg.mask()(I1p,I2m,I3m)<0 ||
              mg.mask()(I1m,I2 ,I3m)<0 || mg.mask()(I1 ,I2 ,I3m)<0 || mg.mask()(I1p,I2 ,I3m)<0 ||
              mg.mask()(I1m,I2p,I3m)<0 || mg.mask()(I1 ,I2p,I3m)<0 || mg.mask()(I1p,I2p,I3m)<0 ||
              mg.mask()(I1m,I2m,I3 )<0 || mg.mask()(I1 ,I2m,I3 )<0 || mg.mask()(I1p,I2m,I3 )<0 ||
              mg.mask()(I1m,I2 ,I3 )<0 ||                             mg.mask()(I1p,I2 ,I3 )<0 ||
              mg.mask()(I1m,I2p,I3 )<0 || mg.mask()(I1 ,I2p,I3 )<0 || mg.mask()(I1p,I2p,I3 )<0 ||
              mg.mask()(I1m,I2m,I3p)<0 || mg.mask()(I1 ,I2m,I3p)<0 || mg.mask()(I1p,I2m,I3p)<0 ||
              mg.mask()(I1m,I2 ,I3p)<0 || mg.mask()(I1 ,I2 ,I3p)<0 || mg.mask()(I1p,I2 ,I3p)<0 ||
              mg.mask()(I1m,I2p,I3p)<0 || mg.mask()(I1 ,I2p,I3p)<0 || mg.mask()(I1p,I2p,I3p)<0 )
	  error=max(error,max(abs(u(I1,I2,I3)-exact(mg,I1,I2,I3,0,0.))));

      worstError=max(worstError,error);
      // printf("Maximum error in extrapolateInterpolationNeighbours= %8.2e, cpu=%8.2e \n",error,time);   
      checker.printMessage("extrapInterpNeighbours",error,time,time1);

      getIndex(mg.extendedIndexRange(),I1,I2,I3); // reset

      // ****************************************************************
      //       tangentialComponent[0,1]
      // ****************************************************************
//    mg.update(MappedGrid::THEcenterBoundaryTangent);  
      int m;
      for( m=0; m<=mg.numberOfDimensions()-2; m++ )
      {
	v=-77.;
	C=Range(0,numberOfComponents-1);
	v(I1,I2,I3,C)=exact(mg,I1,I2,I3,C,0.);
	value=2.; // set t.u = value
	time=CPU();
	v.applyBoundaryCondition(C,BCTypes::BCNames(tangentialComponent0+m),allBoundaries,value);
	time=CPU()-time;
	v.finishBoundaryConditions();
	error=0.;
        const int ndt = m*mg.numberOfDimensions();
	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    realArray & tangent = mg.centerBoundaryTangent(side,axis);
	    // tangent.display("Here is the tangent");
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    if( mg.numberOfDimensions()==1 )
	    {
	    }
	    else if( mg.numberOfDimensions()==2 )
	      error=max(error,max(abs(
		 tangent(Ib1,Ib2,Ib3,0+ndt)*v(Ib1,Ib2,Ib3,0)
		+tangent(Ib1,Ib2,Ib3,1+ndt)*v(Ib1,Ib2,Ib3,1)-value
		)));    
	    else
	      error=max(error,max(abs(
	 	 tangent(Ib1,Ib2,Ib3,0+ndt)*v(Ib1,Ib2,Ib3,0)
		+tangent(Ib1,Ib2,Ib3,1+ndt)*v(Ib1,Ib2,Ib3,1)
		+tangent(Ib1,Ib2,Ib3,2+ndt)*v(Ib1,Ib2,Ib3,2)-value
		)));    
	  }
	}
	worstError=max(worstError,error);
	// printf("Maximum error in tangentialComponent%i              = %8.2e, cpu=%8.2e \n",m,error,time);  
        checker.printMessage(sPrintF(buff,"tangentialComponent%i",m),error,time);

      }

      // ****************************************************************
      //       extrapolate
      // ****************************************************************
      getIndex(mg.extendedIndexRange(),I1,I2,I3); 

      v=-77.;
      C=Range(component,component);
      v(I1,I2,I3,C)=exact(mg,I1,I2,I3,C,0.);
      time=CPU();
      v.applyBoundaryCondition(C,extrapolate,allBoundaries);
      time=CPU()-time;
      v.finishBoundaryConditions();
      error=0.;
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,+1);

	  error=max(error,max(abs(v(Ig1,Ig2,Ig3,component)-exact(mg,Ig1,Ig2,Ig3,component,0.))));
	}
      }
      worstError=max(worstError,error);
      checker.printMessage("extrapolate",error,time);

      // ****************************************************************
      //       extrapolateNormalComponent
      // ****************************************************************
      v=-77.;
      C=Range(0,numberOfComponents-1);
      v(I1,I2,I3,C)=exact(mg,I1,I2,I3,C,0.);
      time=CPU();
      v.applyBoundaryCondition(C,extrapolateNormalComponent,allBoundaries);
      time=CPU()-time;
      v.finishBoundaryConditions();
      error=0.;
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,+1);

	  realArray & normal = mg.vertexBoundaryNormal(side,axis);
	  if( mg.numberOfDimensions()==1 )
	    error=max(error,max(abs(
	      normal(Ib1,Ib2,Ib3,0)*(v(Ig1,Ig2,Ig3,0)-exact(mg,Ig1,Ig2,Ig3,0,0.))
              )));    
	  else if( mg.numberOfDimensions()==2 )
	    error=max(error,max(abs(
	      normal(Ib1,Ib2,Ib3,0)*(v(Ig1,Ig2,Ig3,0)-exact(mg,Ig1,Ig2,Ig3,0,0.))
	      +normal(Ib1,Ib2,Ib3,1)*(v(Ig1,Ig2,Ig3,1)-exact(mg,Ig1,Ig2,Ig3,1,0.))
              )));    
	  else
	    error=max(error,max(abs(
	      normal(Ib1,Ib2,Ib3,0)*(v(Ig1,Ig2,Ig3,0)-exact(mg,Ig1,Ig2,Ig3,0,0.))
	      +normal(Ib1,Ib2,Ib3,1)*(v(Ig1,Ig2,Ig3,1)-exact(mg,Ig1,Ig2,Ig3,1,0.))
	      +normal(Ib1,Ib2,Ib3,2)*(v(Ig1,Ig2,Ig3,2)-exact(mg,Ig1,Ig2,Ig3,2,0.))
              )));    
	}
      }
      worstError=max(worstError,error);
      // printf("Maximum error in extrapolateNormalComponent        = %8.2e, cpu=%8.2e \n",error,time);  
      checker.printMessage("extrapolateNormalComponent",error,time);

      // ****************************************************************
      //       extrapolateTangentialComponents
      // ****************************************************************
      for( m=0; m<=mg.numberOfDimensions()-2; m++ )
      {
	v=-77.;
	C=Range(0,numberOfComponents-1);
	v(I1,I2,I3,C)=exact(mg,I1,I2,I3,C,0.);
	value=2.; // set equal to this value
	time=CPU();
	v.applyBoundaryCondition(C,BCTypes::BCNames(extrapolateTangentialComponent0+m),allBoundaries,value);
	time=CPU()-time;
	v.finishBoundaryConditions();
	error=0.;
        const int ndt = m*mg.numberOfDimensions();
	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,+1);

	    realArray & tangent = mg.centerBoundaryTangent(side,axis);
	    if( mg.numberOfDimensions()==1 )
	    {
	    }
	    else if( mg.numberOfDimensions()==2 )
	      error=max(error,max(abs(
		 tangent(Ib1,Ib2,Ib3,0+ndt)*(v(Ig1,Ig2,Ig3,0)-exact(mg,Ig1,Ig2,Ig3,0,0.))
		+tangent(Ib1,Ib2,Ib3,1+ndt)*(v(Ig1,Ig2,Ig3,1)-exact(mg,Ig1,Ig2,Ig3,1,0.))
		)));    
	    else
	      error=max(error,max(abs(
		 tangent(Ib1,Ib2,Ib3,0+ndt)*(v(Ig1,Ig2,Ig3,0)-exact(mg,Ig1,Ig2,Ig3,0,0.))
		+tangent(Ib1,Ib2,Ib3,1+ndt)*(v(Ig1,Ig2,Ig3,1)-exact(mg,Ig1,Ig2,Ig3,1,0.))
		+tangent(Ib1,Ib2,Ib3,2+ndt)*(v(Ig1,Ig2,Ig3,2)-exact(mg,Ig1,Ig2,Ig3,2,0.))
		)));    
	  }
	}
	worstError=max(worstError,error);
	// printf("Maximum error in extrapolateTangentialComponent%i   = %8.2e, cpu=%8.2e \n",m,error,time);  
        checker.printMessage(sPrintF(buff,"extrapolateTangentialComponent%i",m),error,time);

      }
    

/* ---
    ForBoundary(side,axis)
    {
      printf(" ********* side=%i, axis=%i ********** \n",side,axis);
      mg.centerBoundaryTangent(side,axis).display("center boundary tangent");
    }
---- */    


    // ****************************************************************
    //       normalDerivativeOfTangentialComponent[0,1]
    // ****************************************************************
      for( m=0; m<=mg.numberOfDimensions()-2; m++ )
      {
	v=-77.;
	C=Range(0,numberOfComponents-1);
	v(I1,I2,I3,C)=exact(mg,I1,I2,I3,C,0.);
	value=2.; // set equal to this value
	time=CPU();
	v.applyBoundaryCondition(C,BCTypes::BCNames(normalDerivativeOfTangentialComponent0+m),allBoundaries,value);
	time1=CPU()-time;
	time=CPU();
	v.applyBoundaryCondition(C,BCTypes::BCNames(normalDerivativeOfTangentialComponent0+m),allBoundaries,value);
	time=CPU()-time;
	v.finishBoundaryConditions();
	error=0.;
        const int ndt = m*mg.numberOfDimensions();
	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    realArray & tangent = mg.centerBoundaryTangent(side,axis);
	    realArray & normal = mg.vertexBoundaryNormal(side,axis);

	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,+1);

	    // t_0( n.grad v_0) + t_1( n.grad v_1) + ...
	    if( mg.numberOfDimensions()==1 )
	    {
	    }
	    if( mg.numberOfDimensions()==2 )
	      error=max(error,max(abs(
		tangent(Ib1,Ib2,Ib3,0+ndt)*(
		  normal(Ib1,Ib2,Ib3,0)*v.x(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0)
		  +normal(Ib1,Ib2,Ib3,1)*v.y(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0)
		  )
		+tangent(Ib1,Ib2,Ib3,1+ndt)*(
		  normal(Ib1,Ib2,Ib3,0)*v.x(Ib1,Ib2,Ib3,1)(Ib1,Ib2,Ib3,1)
		  +normal(Ib1,Ib2,Ib3,1)*v.y(Ib1,Ib2,Ib3,1)(Ib1,Ib2,Ib3,1)
		  ) -value
		)));    
	    else
	      error=max(error,max(abs(
		tangent(Ib1,Ib2,Ib3,0+ndt)*(
		  normal(Ib1,Ib2,Ib3,0)*v.x(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0)
		  +normal(Ib1,Ib2,Ib3,1)*v.y(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0)
		  +normal(Ib1,Ib2,Ib3,2)*v.z(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0)
		  )
		+tangent(Ib1,Ib2,Ib3,1+ndt)*(
		  normal(Ib1,Ib2,Ib3,0)*v.x(Ib1,Ib2,Ib3,1)(Ib1,Ib2,Ib3,1)
		  +normal(Ib1,Ib2,Ib3,1)*v.y(Ib1,Ib2,Ib3,1)(Ib1,Ib2,Ib3,1)
		  +normal(Ib1,Ib2,Ib3,2)*v.z(Ib1,Ib2,Ib3,1)(Ib1,Ib2,Ib3,1)
		  )
		+tangent(Ib1,Ib2,Ib3,2+ndt)*(
		  normal(Ib1,Ib2,Ib3,0)*v.x(Ib1,Ib2,Ib3,2)(Ib1,Ib2,Ib3,2)
		  +normal(Ib1,Ib2,Ib3,1)*v.y(Ib1,Ib2,Ib3,2)(Ib1,Ib2,Ib3,2)
		  +normal(Ib1,Ib2,Ib3,2)*v.z(Ib1,Ib2,Ib3,2)(Ib1,Ib2,Ib3,2)
		  ) -value
		)));    
	  }
	}
	worstError=max(worstError,error);
	// printf("Max error normalDerivativeOfTangentialComponent%i   = %8.2e, cpu=%8.2e(init), %8.2e\n",m,error,time1,time);  
        checker.printMessage(sPrintF(buff,"normalDerivativeOfTangentialComponent%i",m),error,time,time1);

      }


      // ****************************************************************
      //       normalDerivativeOfNormalComponent
      // ****************************************************************
      v=-77.;
      C=Range(0,numberOfComponents-1);
      v(I1,I2,I3,C)=exact(mg,I1,I2,I3,C,0.);
      value=2.; // set equal to this value
      time=CPU();
      v.applyBoundaryCondition(C,normalDerivativeOfNormalComponent,allBoundaries,value);
      time1=CPU()-time;
      time=CPU();
      v.applyBoundaryCondition(C,normalDerivativeOfNormalComponent,allBoundaries,value);
      time=CPU()-time;
      v.finishBoundaryConditions();
      error=0.;
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition()(side,axis) > 0 )
	{
	  realArray & normal = mg.vertexBoundaryNormal(side,axis);

	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,+1);

	  // t_0( n.grad v_0) + t_1( n.grad v_1) + ...
	  if( mg.numberOfDimensions()==1 )
	    error=max(error,max(abs(
	      normal(Ib1,Ib2,Ib3,0)*(
		normal(Ib1,Ib2,Ib3,0)*v.x(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0)
		) -value
	      )));    
	  else if( mg.numberOfDimensions()==2 )
	    error=max(error,max(abs(
	      normal(Ib1,Ib2,Ib3,0)*(
		normal(Ib1,Ib2,Ib3,0)*v.x(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0)
		+normal(Ib1,Ib2,Ib3,1)*v.y(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0)
		)
	      +normal(Ib1,Ib2,Ib3,1)*(
		normal(Ib1,Ib2,Ib3,0)*v.x(Ib1,Ib2,Ib3,1)(Ib1,Ib2,Ib3,1)
		+normal(Ib1,Ib2,Ib3,1)*v.y(Ib1,Ib2,Ib3,1)(Ib1,Ib2,Ib3,1)
		) -value
	      )));    
	  else
	    error=max(error,max(abs(
	      normal(Ib1,Ib2,Ib3,0)*(
		normal(Ib1,Ib2,Ib3,0)*v.x(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0)
		+normal(Ib1,Ib2,Ib3,1)*v.y(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0)
		+normal(Ib1,Ib2,Ib3,2)*v.z(Ib1,Ib2,Ib3,0)(Ib1,Ib2,Ib3,0)
		)
	      +normal(Ib1,Ib2,Ib3,1)*(
		normal(Ib1,Ib2,Ib3,0)*v.x(Ib1,Ib2,Ib3,1)(Ib1,Ib2,Ib3,1)
		+normal(Ib1,Ib2,Ib3,1)*v.y(Ib1,Ib2,Ib3,1)(Ib1,Ib2,Ib3,1)
		+normal(Ib1,Ib2,Ib3,2)*v.z(Ib1,Ib2,Ib3,1)(Ib1,Ib2,Ib3,1)
		)
	      +normal(Ib1,Ib2,Ib3,2)*(
		normal(Ib1,Ib2,Ib3,0)*v.x(Ib1,Ib2,Ib3,2)(Ib1,Ib2,Ib3,2)
		+normal(Ib1,Ib2,Ib3,1)*v.y(Ib1,Ib2,Ib3,2)(Ib1,Ib2,Ib3,2)
		+normal(Ib1,Ib2,Ib3,2)*v.z(Ib1,Ib2,Ib3,2)(Ib1,Ib2,Ib3,2)
		) -value
	      )));    
	}
      }
      worstError=max(worstError,error);
      // printf("Maximum error in normalDerivativeOfNormalComponent = %8.2e, cpu=%8.2e(init), %8.2e \n",error,time1,time);      
      checker.printMessage("normalDerivativeOfNormalComponent",error,time,time1);
      


      // ****************************************************************
      //        finish boundary conditions
      // ****************************************************************


      
      // bcParams.numberOfCornerGhostLinesToAssign=2;
      
      getIndex(mg.dimension(),Ia1,Ia2,Ia3);

      const int numberOfCornerBC=5;
      int cornerBC[numberOfCornerBC]={ BoundaryConditionParameters::extrapolateCorner,
                                       BoundaryConditionParameters::evenSymmetryCorner,
                                       BoundaryConditionParameters::oddSymmetryCorner,
                                       BoundaryConditionParameters::taylor2ndOrderEvenCorner,
                                       BoundaryConditionParameters::taylor4thOrderEvenCorner};  //
      aString cornerBCName[]={"extrapolate",
                              "evenSymmetry",
                              "oddSymmetry", 
                              "taylor2ndOrderEven",
                              "taylor4thOrderEven"}; //

//      mg.boundaryCondition()(0,0)=0; // ****
//        mg.boundaryCondition()(1,0)=1; // ****
//      mg.boundaryCondition()(0,1)=0; // ****
//        mg.boundaryCondition()(1,1)=1; // ****
//        mg.boundaryCondition()(0,2)=0; // ****
//        mg.boundaryCondition()(1,2)=1; // ****
      
      // cornerMask : 1 in the ghost points where finish boundary condition assigns values
      IntegerArray cornerMask(Ia1,Ia2,Ia3);
      cornerMask=0;
//        getIndex(mg.gridIndexRange(),I1,I2,I3);
//        cornerMask(I1,I2,I3)=0;

      bool useWhereMask=0;  // this must match fixBoundaryCorners

      int ng1a,ng1b,ng2a,ng2b,ng3a,ng3b;
      const IntegerArray & indexRange = mg.indexRange();
      const IntegerArray & dimension = mg.dimension();

      int side1,side2,side3,m1,m2,m3,is1,is2,is3;
      if( !mg.isPeriodic(axis1) && !mg.isPeriodic(axis2) )
      {
	//       ...Do the four edges parallel to i3
	is3=0;
	for( side1=Start; side1<=End; side1++ )
	{
	  is1=1-2*side1;
	  for( side2=Start; side2<=End; side2++ )
	  {
	    is2=1-2*side2;
	    if( mg.boundaryCondition(side1,axis1)>0 || mg.boundaryCondition(side2,axis2)>0 )
	    {
	      setCornerMask(is1,is2,is3);
	    }
	  }
	}
      }
 
      if( mg.numberOfDimensions()==3)
      {
	if( !mg.isPeriodic(axis1) && !mg.isPeriodic(axis3) )
	{
	  //       ...Do the four edges parallel to i2
	  is2=0;
	  for( side1=Start; side1<=End; side1++ )
	  {
	    is1=1-2*side1;
	    for( side3=Start; side3<=End; side3++ )
	    {
	      is3=1-2*side3;
	      if( mg.boundaryCondition(side1,axis1)>0 || mg.boundaryCondition(side3,axis3)>0 )
	      {
	        setCornerMask(is1,is2,is3);
	      }
	    }
	  }
	}
	if( !mg.isPeriodic(axis2) && !mg.isPeriodic(axis3) )
	{
	  //       ...Do the four edges parallel to i1
	  is1=0;
	  for( side2=Start; side2<=End; side2++ )
	  {
	    is2=1-2*side2;
	    for( side3=Start; side3<=End; side3++ )
	    {
	      is3=1-2*side3;
	      if( mg.boundaryCondition(side2,axis2)>0 || mg.boundaryCondition(side3,axis3)>0 )
	      {
	        setCornerMask(is1,is2,is3);            
	      }
	    }
	  }
	}
  
	if( !mg.isPeriodic(axis1) && !mg.isPeriodic(axis2) && !mg.isPeriodic(axis3) )
	{
	  //    ...Do the points outside vertices in 3D
	  for( side1=Start; side1<=End; side1++ )
	  {
	    is1=1-2*side1;
	    for( side2=Start; side2<=End; side2++ )
	    {
	      is2=1-2*side2;
	      for( side3=Start; side3<=End; side3++ )
	      {
		is3=1-2*side3;
		if( mg.boundaryCondition(side1,axis1)>0 || 
		    mg.boundaryCondition(side2,axis2)>0 || 
		    mg.boundaryCondition(side3,axis3)>0 )
		{
                  setCornerMask(is1,is2,is3);
		}
	      }
	    }
	  }
	}
      }
      
      // cornerMask.display("cornerMask");
      // int count=sum(cornerMask);
      // printf(" Number of corner points = %i\n",count);
      
      for( int ibc=0; ibc<numberOfCornerBC; ibc++ )
      {
        bcParams.setCornerBoundaryCondition(BoundaryConditionParameters::CornerBoundaryConditionEnum(cornerBC[ibc]));
	u(Ia1,Ia2,Ia3)=exact(mg,Ia1,Ia2,Ia3,0,0.);

        where( cornerMask )
	{
	  u(Ia1,Ia2,Ia3)=999.;
	}

        // **NOTE** the corner BC's do not include TZ forcing so the errors may be large -- need to fix this --
	if( false && cornerBC[ibc]==(int)BoundaryConditionParameters::evenSymmetryCorner )
	{
	  display(u,"u before evenSymmetryCorner BC","%6.2f ");
	}
	
	time=CPU();
	u.finishBoundaryConditions(bcParams);
	time1=CPU()-time;

	if( false && cornerBC[ibc]==(int)BoundaryConditionParameters::evenSymmetryCorner )
	{
	  display(u,"u after evenSymmetryCorner BC","%6.2f ");
	}

	time=CPU();
	u.finishBoundaryConditions(bcParams);
	time=CPU()-time;

        // maskOption=0 : compute residual at all ghost points.
        // maskOption=1 : only compute residual at ghost points where the boundary mask!=0 
        int maskOption = cornerBC[ibc]==(int)BoundaryConditionParameters::extrapolateCorner ? 1 : 0;
	
	error=0.;
	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition(side,axis) > 0 )
	  {
	    getBoundaryIndex(mg.dimension(),side,axis,Ib1,Ib2,Ib3);
            Ibv[axis]=mg.gridIndexRange(side,axis);
	    int ghost=1;
	    getGhostIndex(mg.dimension(),side,axis,Ig1,Ig2,Ig3,ghost);

            // check all ghost lines *wdh* 070506 -- only check errors where mask!=0 on the boundary

//             Igv[axis]=side==0 ? Range(mg.dimension(0,axis),mg.gridIndexRange(0,axis)-1) : 
// 	                        Range(mg.gridIndexRange(1,axis)+1,mg.dimension(1,axis));
	    
            const int is=1-2*side;
            for( int ghost=mg.dimension(side,axis); ghost!=mg.gridIndexRange(side,axis); ghost+=is )
	    {
	      Igv[axis]=ghost;
              if( maskOption==0 )
	      {
		error=max(error,max(abs(u(Ig1,Ig2,Ig3)-exact(mg,Ig1,Ig2,Ig3,0,0.))));
	      }
	      else
	      {
		where( mg.mask()(Ib1,Ib2,Ib3)!=0 )
		{
		  error=max(error,max(abs(u(Ig1,Ig2,Ig3)-exact(mg,Ig1,Ig2,Ig3,0,0.))));
		}
	      }
	    }
	    
	    // if( false && cornerBC[ibc]==(int)BoundaryConditionParameters::taylor4thOrderEvenCorner )
            if( false && cornerBC[ibc]==(int)BoundaryConditionParameters::evenSymmetryCorner )
	    {
	      printf(" ******* side=%i axis=%i ******\n",side,axis);
	      display( (u(Ig1,Ig2,Ig3)-exact(mg,Ig1,Ig2,Ig3,0,0.)),"ERROR","%6.2f ");
	    }
	  }
	}

        // don't count the even symmetry BC in the largest error since it is only exact for constant poly's
        if( cornerBC[ibc]!=(int)BoundaryConditionParameters::evenSymmetryCorner )
	{
	  worstError=max(worstError,error);
	}
	
	checker.printMessage("finishBC: "+cornerBCName[ibc],error,time,time1);
      }
      bcParams.setCornerBoundaryCondition(BoundaryConditionParameters::extrapolateCorner); // reset


    }  // loop over all component grids
    

    printf("\n\n ************************************************************************************************\n");
    if( worstError > .01 )
      printf(" ************** Warning, there is a large error somewhere, worst error =%8.2e ******************\n",
             worstError);
    else
      printf(" ************** Test apparently successful, worst error =%8.2e ******************\n",worstError);
    printf(" **************************************************************************************************\n\n");
    
  } // end loop over composite grids
  
//    printf(" ****timeForAllBoundaryConditions=%8.2e,  timeToSetupBoundaryConditions=%8.2e\n",
//           timeForAllBoundaryConditions,timeToSetupBoundaryConditions);

  if( false )
    GenericMappedGridOperators::printBoundaryConditionStatistics();

  Overture::finish();          

  cout << "Program Terminated Normally! \n";
  return 0;

}
