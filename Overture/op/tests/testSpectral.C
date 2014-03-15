#include "Overture.h"
#include "OGTrigFunction.h"  // Trigonometric function
#include "MappedGridOperators.h"
#include "LineMapping.h"
#include "Square.h"
#include "BoxMapping.h"
#include "NameList.h"
#include "FourierOperators.h"

//================================================================================
//  Test out the MappedGridOperators pseudo-spectral derivatives
//================================================================================
int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  int debug=0, numberOfDimensions=2;

  int nx[3] = { 8,8,1};   // number of grid points (minus 1) in each direction
  // frequencies for exact solution, cos(fx[0]*pi*x)*cos(fx[1]*pi*y)*cos(fx[2]*pi*z)
  int fx[3] = { 2,2,0 };  
  real period[3] = {1.,1.,1.}; 

  NameList nl;                 // The NameList object allows one to read in values by name
  aString name(80),answer(80);
  printf(
   " Parameters for Example 3: \n"
   " ------------------------- \n"
   "   name                                                 type    default  \n"
   "numberOfDimensions (nd=) (assign first)               (int)      %i     \n"
   "nx,ny,nx                                              (int)   %i %i %i \n"
   "fx,fy,fz (fx*xPeriod=even)                            (int)   %i %i %i       \n"
   "xPeriod,yPeriod,zPeriod                               (real)  %e %e %e       \n",
      numberOfDimensions,nx[0],nx[1],nx[2],fx[0],fx[1],fx[2],period[0],period[1],period[2]);

  // ==========Loop for changing parameters========================
  for( ;; ) 
  {
    cout << "Enter changes to variables, exit to continue" << endl;
    cin >> answer;
    if( answer=="exit" ) break;
    nl.getVariableName( answer, name );   // parse the answer
    if( name== "numberOfDimensions" || name=="nd" )   
    {
      numberOfDimensions=nl.intValue(answer);  
      if( numberOfDimensions==1 )
      {
	nx[1]=nx[2]=1;	fx[1]=fx[2]=0;
      }
      else if( numberOfDimensions==2 ) 
      {
	nx[1]=8, nx[2]=1; fx[1]=2, fx[2]=0;
      }
      else
      {
	nx[1]=8, nx[2]=8; fx[1]=2, fx[2]=2;
      }
    }
    else if( name== "nx" )   
      nx[0]=nl.realValue(answer);  
    else if( name== "ny" )   
      nx[1]=nl.realValue(answer);  
    else if( name== "nz" )   
      nx[2]=nl.realValue(answer);  
    else if( name== "fx" )   
      fx[0]=nl.realValue(answer);  
    else if( name== "fy" )   
      fx[1]=nl.realValue(answer);  
    else if( name== "fz" )   
      fx[2]=nl.realValue(answer);  
    else if( name== "xPeriod" )
      period[0]=nl.realValue(answer);  
    else if( name== "yPeriod" )
      period[1]=nl.realValue(answer);  
    else if( name== "zPeriod" )
      period[2]=nl.realValue(answer);  
    else
      cout << "unknown response: [" << name << "]" << endl;
  }
    
  LineMapping line;
  SquareMapping square(0.,period[0],0.,period[1]);                  // Make a mapping, unit square
  BoxMapping box(0.,period[0],0.,period[1],0.,period[2]);;
  // choose a line, square or box depending on the number of dimensions
  Mapping & map = numberOfDimensions==1 ? (Mapping&)line : 
                ( numberOfDimensions==2 ? (Mapping&)square : (Mapping&)box );

  for( int axis=0; axis<numberOfDimensions; axis++ )
  {
    map.setGridDimensions(axis,nx[axis]+1);               // number of grid points
    map.setIsPeriodic(axis,Mapping::functionPeriodic);
  }
  MappedGrid mg(map);                              // MappedGrid for a square
  mg.update();
  
  Range all;
  realMappedGridFunction u(mg);

  MappedGridOperators op(mg);                     // define some differential operators
  u.setOperators(op);                         // Tell u which operators to use
  // ---- compute all derivatives with the pseudo-spectral method ----
  u.getOperators()->setOrderOfAccuracy(MappedGridOperators::spectral);

  OGTrigFunction true(fx[0],fx[1],fx[2]);  // create an exact solution (Twilight-Zone solution)

  real error;
  int n=0;      // only test first component

  Index I1,I2,I3,N;
  getIndex(mg.dimension(),I1,I2,I3);             // assign I1,I2,I3, all grid points including ghost 
  u(I1,I2,I3)=true(mg,I1,I2,I3,n,0.);          // assign true solution

  error = max(fabs(u.x()(I1,I2,I3)-true.x(mg,I1,I2,I3,n)));
  cout << "u.x : Maximum error (spectral) = " << error << endl;
  if( debug & 4 )
  {
    fabs( u.x()(I1,I2,I3)-true.x(mg,I1,I2,I3,n)).display("Error in u.x");
    true.x(mg,I1,I2,I3,n).display(" true u.x");
    u.x()(I1,I2,I3).display("computed u.x");
    true(mg,I1,I2,I3,n).display(" true u");
    u(I1,I2,I3).display("discrete u");
  }
      
  error = max(fabs(u.y()(I1,I2,I3)-true.y(mg,I1,I2,I3,n)));
  cout << "u.y : Maximum error (spectral) = " << error << endl;
  if( debug & 4 )
  {
    fabs(u.y()(I1,I2,I3)-true.y(mg,I1,I2,I3,n)).display("Error in u.y");
    u.y()(I1,I2,I3).display("u.y");
    true.y(mg,I1,I2,I3,n).display("true.y");
  }

  error = max(fabs(u.xx()(I1,I2,I3)-true.xx(mg,I1,I2,I3,n)));
  cout << "u.xx : Maximum error (spectral) = " << error << endl;

  error = max(fabs(u.xy()(I1,I2,I3)-true.xy(mg,I1,I2,I3,n)));
  cout << "u.xy : Maximum error (spectral) = " << error << endl;

  error = max(fabs(u.yy()(I1,I2,I3)-true.yy(mg,I1,I2,I3,n)));
  cout << "u.yy : Maximum error (spectral) = " << error << endl;
  
  error = max(fabs(u.laplacian()(I1,I2,I3)-(true.xx(mg,I1,I2,I3,n)+true.yy(mg,I1,I2,I3,n)
                                           +true.zz(mg,I1,I2,I3,n))));
  cout << "u.laplacian : Maximum error (spectral) = " << error << endl;

  error = max(fabs(u.z()(I1,I2,I3)-true.z(mg,I1,I2,I3,n)));
  cout << "u.z : Maximum error (spectral) = " << error << endl;

  error = max(fabs(u.xz()(I1,I2,I3)-true.xz(mg,I1,I2,I3,n)));
  cout << "u.xz : Maximum error (spectral) = " << error << endl;

  error = max(fabs(u.yz()(I1,I2,I3)-true.yz(mg,I1,I2,I3,n)));
  cout << "u.yz : Maximum error (spectral) = " << error << endl;

  error = max(fabs(u.zz()(I1,I2,I3)-true.zz(mg,I1,I2,I3,n)));
  cout << "u.zz : Maximum error (spectral) = " << error << endl;

  // *********************************************************************************
  // Now get the FourierOperators (this must be done only after at least one
  // derivative has been computed)
  // *********************************************************************************
  FourierOperators & fourier = *op.getFourierOperators();

  // compute the transform directly
  realMappedGridFunction uHat(mg);
  fourier.realToFourier( u,uHat );
  uHat.display("Here is uHat");


  Overture::finish();          
  cout << "Program Terminated Normally! \n";
  return 0;
}
