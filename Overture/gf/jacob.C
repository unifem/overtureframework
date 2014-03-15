#include "Overture.h"
#include "CompositeGridOperators.h"
#include "display.h"

int main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  int grid;
  real mx,mn;
  Index I1,I2,I3;

  printf(" --------------------------------------------------------------------------- \n");
  printf(" Demonstrate the operators for taking derivatives of compositeGridFunction's \n");
  printf(" --------------------------------------------------------------------------- \n");

  aString nameOfOGFile="../ogen/eccentric.hdf";
//  cout << "example5>> Enter the name of the (old) overlapping grid file:" << endl;
//  cin >> nameOfOGFile;

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  CompositeGridOperators operators(cg);                        // operators for a CompositeGridFunction
  Interpolant interpolant(cg);      // Make an interpolant

  realCompositeGridFunction u(cg),ux(cg);                      // create two composite grid functions

  u.setOperators(operators);                                   // tell grid function which operators to use
  ux.setOperators(operators);

  u=1.;
  ux=u.xx();                                                    // compute the x derivative of u
  //  ux.display("Here is the xx derivative of u=1 (computed at interior and boundary points)");

  for( grid=0;grid < cg.numberOfComponentGrids(); grid++)
    {
      displayMask(cg[grid].mask(),"mask");
      

      getIndex(cg[grid].indexRange(),I1,I2,I3);
      where( cg[grid].mask()(I1,I2,I3) > 0 )    
	{
	  mx = max(ux[grid](I1,I2,I3));
	  mn = min(ux[grid](I1,I2,I3));
	}
      cout << "Maximum of u_xx before interpolation on grid " << grid << " : " << mx << endl;
      cout << "Minimum of u_xx before interpolation on grid " << grid << " : " << mn << endl;
    }

  
  u.display("u before interpolation");
  u.interpolate();
  u.display("u after interpolation");
  
//  u.finishBoundaryConditions();
  

  ux=u.xx();                                                    // compute the x derivative of u
  ux.display("Here is the xx derivative of u=1 BEFORE interpolation (computed at interior and boundary points)");
  ux.interpolate();
  ux.applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::allBoundaries,0.);
  ux.finishBoundaryConditions();


  ux.display("Here is the xx derivative of u=1 after interpolation (computed at interior and boundary points)");

  real uError;
  for( grid=0;grid < cg.numberOfComponentGrids(); grid++)
    {
      getIndex(cg[grid].indexRange(),I1,I2,I3);
      where( cg[grid].mask()(I1,I2,I3) > 0 )    
	{
	  mx = max(ux[grid](I1,I2,I3));
	  mn = min(ux[grid](I1,I2,I3));
	}
      cout << "Maximum of u_xx after interpolation on grid " << grid << " : " << mx << endl;
      cout << "Minimum of u_xx after interpolation on grid " << grid << " : " << mn << endl;
      getIndex(cg[grid].extendedIndexRange(),I1,I2,I3);
      where( cg[grid].mask()(I1,I2,I3) != 0 )    
	{
          uError=max(fabs(u[grid](I1,I2,I3)-1.));
	}
      cout << "Error in u on grid " << grid << " : " << uError << endl;
    }
    


  cout << " Here is the output from the original 'example5' " << endl;


  real error;
  
  for(grid=0; grid<cg.numberOfComponentGrids(); grid++ )        // loop over component grids
  {
    MappedGrid & mg = cg[grid];
    getIndex(mg.dimension(),I1,I2,I3);                           // assign I1,I2,I3 for dimension
    u[grid](I1,I2,I3)=sin(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2));  
    getIndex(mg.indexRange(),I1,I2,I3);                          // assign I1,I2,I3 for indexRange

    operators.setOrderOfAccuracy(2);                // Bill, I added this line  !
							 
    ux[grid](I1,I2,I3)=u[grid].x()(I1,I2,I3);                  // here is the x derivative of u[grid]

    error = max(fabs( ux[grid](I1,I2,I3)- cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2) )));
    cout << "Maximum error (2nd order) on grid " << grid <<  ": "  << error << endl;

    error = max(fabs( operators[grid].x(u[grid])(I1,I2,I3)      // another way to compute derivatives
                    - cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2) )));
    cout << "Maximum error (2nd order) on grid " << grid <<  ":  " << error << endl;

    operators.setOrderOfAccuracy(4);                           // set order of accuracy to 4
    getIndex(mg.indexRange(),I1,I2,I3,-1);                       // decrease ranges by 1 for 4th order
    error = max(fabs(u[grid].x()(I1,I2,I3)-cos(mg.vertex()(I1,I2,I3,axis1))*cos(mg.vertex()(I1,I2,I3,axis2))));
    cout << "Maximum error (4th order) on grid " << grid <<  ":  " << error << endl;
  }
  return 0;
}
