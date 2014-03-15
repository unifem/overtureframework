#include "Overture.h"
#include "MappedGridOperators.h"
#include "NameList.h"
#include "OGTrigFunction.h"
#include "OGPolyFunction.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


//================================================================================
//  **** Test the boundary conditions *****
//================================================================================

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
 
  aString nameOfOGFile, nameOfDirectory=".";
  cout << "Enter the name of the composite grid file (in the cgsh directory)" << endl;
  cin >> nameOfOGFile;   
  if( nameOfOGFile[0] != '.' )
    nameOfOGFile="/users/henshaw/res/cgsh/" + nameOfOGFile;

  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);

  // make some shorter names for readability
  BCTypes::BCNames 
                   dirichlet                  = BCTypes::dirichlet,
                   neumann                    = BCTypes::neumann,
                   extrapolate                = BCTypes::extrapolate,
                   normalComponent            = BCTypes::normalComponent,
                   extrapolateNormalComponent = BCTypes::extrapolateNormalComponent,
              extrapolateTangentialComponent0 = BCTypes::extrapolateTangentialComponent0,
              extrapolateTangentialComponent1 = BCTypes::extrapolateTangentialComponent1,
                   aDotU                      = BCTypes::aDotU,
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

  real error=0., worstError=0.;
    
  // loop over all component grids
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    cout << "+++++++Checking component grid = " << grid << "+++++++" << endl;

    MappedGrid & mg = cg[grid]; 
    mg.update();

    int degreeOfSpacePolynomial = 2;
    int degreeOfTimePolynomial = 1;
    int numberOfComponents = mg.numberOfDimensions();
    OGPolyFunction true(degreeOfSpacePolynomial,mg.numberOfDimensions(),numberOfComponents,
			degreeOfTimePolynomial);

    Range all;
    realMappedGridFunction u(mg,all,all,all,numberOfComponents); // define some component grid functions

    MappedGridOperators operators(mg);                     // define some differential operators
    operators.setTwilightZoneFlow( TRUE );
    operators.setTwilightZoneFlowFunction( true );
    u.setOperators( operators );                           // Tell u which operators to use

    Index I1,I2,I3, Ig1,Ig2,Ig3,Ib1,Ib2,Ib3;
    getIndex(mg.indexRange,I1,I2,I3);  

    int side,axis,m;
    Range C(0,numberOfComponents-1);

    // ****************************************************************
    //       neumann
    // ****************************************************************
    u=-77.;                             // put bogus values everywhere
    u(I1,I2,I3)=true(mg,I1,I2,I3,C,0.); // fill in interior values
    u.applyBoundaryCondition(C,neumann,allBoundaries);
    u.finishBoundaryConditions();
    // u.display("u after neumann ");

    error=0.;
    ForBoundary(side,axis)
    {
      if( mg.boundaryCondition(side,axis) > 0 )
      {
        getGhostIndex(mg.gridIndexRange,side,axis,Ig1,Ig2,Ig3,1);
        error=max(error,max(abs(u(Ig1,Ig2,Ig3,C)-true(mg,Ig1,Ig2,Ig3,C))));
      }
    }
    worstError=max(worstError,error);
    printf("Maximum error in neumann = %e\n",error);  
    // u.display("Here is u after a neumann BC");
    
  
    // ****************************************************************
    //       normalDerivativeOfTangentialComponent[0,1]
    // ****************************************************************
    for( m=0; m<=mg.numberOfDimensions()-2; m++ )
    {
      u=-77.;
      u(I1,I2,I3)=true(mg,I1,I2,I3,C,0.);
      // u.display("u before normalDerivativeOfTangentialComponent ");
      u.applyBoundaryCondition(C,BCTypes::BCNames(normalDerivativeOfTangentialComponent0+m),allBoundaries);
      u.finishBoundaryConditions();
      // u.display("u after normalDerivativeOfTangentialComponent ");
      error=0.;
      ForBoundary(side,axis)
      {
	if( mg.boundaryCondition(side,axis) > 0 )
	{

	  RealMappedGridFunction & tangent = mg.centerBoundaryTangent[axis][side];
          RealArray & normal = mg.vertexBoundaryNormal[axis][side];

	  getBoundaryIndex(mg.gridIndexRange,side,axis,Ib1,Ib2,Ib3);
    	  getGhostIndex(mg.gridIndexRange,side,axis,Ig1,Ig2,Ig3,+1);

          // t_0( n.grad v_0) + t_1( n.grad v_1) + ...
	  if( mg.numberOfDimensions()==2 )
	    error=max(error,max(abs(
	       tangent(Ib1,Ib2,Ib3,0,m)*(u(Ig1,Ig2,Ig3,0)-true(mg,Ig1,Ig2,Ig3,0))
	      +tangent(Ib1,Ib2,Ib3,1,m)*(u(Ig1,Ig2,Ig3,1)-true(mg,Ig1,Ig2,Ig3,1))
              )));
	  else
	    error=max(error,max(abs(
	       tangent(Ib1,Ib2,Ib3,0,m)*(u(Ig1,Ig2,Ig3,0)-true(mg,Ig1,Ig2,Ig3,0))
	      +tangent(Ib1,Ib2,Ib3,1,m)*(u(Ig1,Ig2,Ig3,1)-true(mg,Ig1,Ig2,Ig3,1))
	      +tangent(Ib1,Ib2,Ib3,2,m)*(u(Ig1,Ig2,Ig3,2)-true(mg,Ig1,Ig2,Ig3,2))
              )));
	}
      }
      worstError=max(worstError,error);
      printf("Maximum error in normalDerivativeOfTangentialComponent%i = %e\n",m,error);  
    }
    
  }
  
  printf("\n\n **************************************************************************************************\n");
  if( worstError > .01 )
    printf(" ************** Warning, there is a large error somewhere, worst error =%e ******************\n",worstError);
  else
    printf(" ************** Test apparently successful, worst error =%e ******************\n",worstError);
  printf(" **************************************************************************************************\n\n",worstError);
    
  cout << "Program Terminated Normally! \n";
  return 0;

}
