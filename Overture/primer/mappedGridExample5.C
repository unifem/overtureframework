#include "Overture.h"  
#include "PlotStuff.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "MappedGridOperators.h"
#include "NameList.h"
#include "OGTrigFunction.h"
#include "OGPolyFunction.h"

enum forcingTypes
{ 
  noForcing=0,
  poly,
  trig
};

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" --------------------------------------------------------------------------------------- \n");
  printf(" Solve a convection-diffusion equation on an annulus                                     \n");
  printf(" Use the method of analytic solutions to obtain an exact solution                        \n");
  printf(" Use OGPolyFunction and OGTrigFunction to define the exact solution and it's derivatives \n");
  printf(" --------------------------------------------------------------------------------------- \n");

  // Set default values for parameters. These can be optionally changed below
  int numberOfTimeSteps=100;
  real dt=.005;
  IntegerArray bc(2,3); bc=1;
  IntegerArray gridPoints(3); gridPoints=-1;
  int mapType=0;   // 0=square, 1=annulus
  forcingTypes forcingOption=poly;
  int plotOption=TRUE;

  // The NameList object allows one to read in values by name
  NameList nl;
  aString name(80),answer(80);
  printf(
   " Parameters for Example 5: \n"
   " ------------------------- \n"
   "   name                                                 type    default  \n"
   "numberOfTimeSteps  (nts=)                              (int)      %i     \n"
   "mapType (mt= 0:square, 1=annulus)                      (int)      %i     \n"
   "forcingOption (f= 0:none, 1=poly, 2=trig)              (int)      %i     \n"
   "plotOption (p = 1:on, 0:off)                           (int)      %i     \n"
   "time step (dt=)                                        (real)     %f     \n"
   "gridPoints(axis) (gp(axis)=no. of grid points)         (IntegerArray)        \n"
   "boundary conditions (bc(side,axis)=)                   (IntegerArray)        \n",
      numberOfTimeSteps,mapType,forcingOption,plotOption,dt);

  // ==========Loop for changing parameters========================
  for( ;; ) 
  {
    cout << "Enter changes to variables, exit to continue" << endl;
    getLine(answer);
    if( answer=="exit" ) break;
    nl.getVariableName( answer, name );   // parse the answer
    if( name== "numberOfTimeSteps" || name=="nts" )   
      numberOfTimeSteps=nl.intValue(answer);  
    else if( name== "dt" )   
      dt=nl.realValue(answer);  
    else if( name== "mapType" || name=="mt" )   
      mapType=nl.intValue(answer);  
    else if( name== "forcingOption" || name=="f" )   
      forcingOption=(forcingTypes)nl.intValue(answer);  
    else if( name== "plotOption" || name=="p" )   
      plotOption=nl.intValue(answer);  
    else if( name== "bc" )   
      nl.getIntArray( answer,bc );
    else if( name== "gridPoints" || name=="gp")   
      nl.getIntArray( answer,gridPoints );
    else
      cout << "unknown response: [" << name << "]" << endl;
  }

  Mapping *mapping;                                  // keep a pointer to a mapping
  if( mapType==0 )
  {
    mapping = new SquareMapping();                     // create a Square
    mapping->setGridDimensions(axis1,11);              // axis1==0, set no. of grid points
    mapping->setGridDimensions(axis2,11);              // axis2==1, set no. of grid points
  }
  else
  {
    mapping = new AnnulusMapping();                    // create an Annulus
    mapping->setGridDimensions(axis1,41);              // axis1==0, set no. of grid points
    mapping->setGridDimensions(axis2,13);              // axis2==1, set no. of grid points
  }
  for( int axis=0; axis<mapping->getDomainDimension(); axis++ )
  {
    if( gridPoints(axis)>0 )
      mapping->setGridDimensions(axis,gridPoints(axis));
  } 
  MappedGrid mg(*mapping);                           // MappedGrid for a square
  mg.update();                                       // create default variables

  Range all;
  realMappedGridFunction u(mg);
  u.setName("Solution");                          // give names to grid function ...
  u.setName("u",0);                               // ...and components

  OGFunction *exactPointer;
  if( forcingOption==poly )
  {
    int degreeOfSpacePolynomial = 2;
    int degreeOfTimePolynomial = 1;
    int nComp = 1;
    exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,mg.numberOfDimensions(),nComp,
                              degreeOfTimePolynomial);
  }
  else if( forcingOption==trig )
  {
    real fx=1., fy = 1., fz = 1., ft=1.;        // note that fz is not used in 2D
    //  defines cos(pi*x)*cos(pi*y)*cos(pi*z)*cos(pi*t)
    exactPointer = new OGTrigFunction(fx, fy, fz, ft); 
  }
  else if( forcingOption!=0 )
  {
    cout << "Unknown forcing option = " << forcingOption << endl;
    forcingOption=noForcing;
  }
  OGFunction & exact = *exactPointer;  // make a reference for readability

  Index I1,I2,I3, Ib1,Ib2,Ib3;                                            
  // mg.dimension()(2,3) : all points on the grid, including ghost-points
  getIndex(mg.dimension(),I1,I2,I3);               // assign I1,I2,I3 from dimension
  realArray & x= mg.vertex();
  if( forcingOption > 0 )
    u(I1,I2,I3)=exact(mg,I1,I2,I3,0,0.);
  else
    u=1.;
    
  MappedGridOperators op(mg);                    // operators 
  u.setOperators(op);                            // associate with a grid function
  // ***** tell the operators to use the method of analytic solutions for BC's *****
  //       if the forcingOption is greater than 0
  if( forcingOption>0 )
  {
    op.setTwilightZoneFlow(TRUE);
    op.setTwilightZoneFlowFunction(exact);
  }

  PlotStuff ps(plotOption,"mappedGridExample5");        // create a PlotStuff object
  PlotStuffParameters psp;         // This object is used to change plotting parameters
  char buffer[80];

  // Index's for boundary and interior points:
  getIndex(mg.gridIndexRange(),I1,I2,I3);
  real t=0, a=1., b=1., nu=.1;
  for( int step=0; step<numberOfTimeSteps; step++ )
  {
    if( plotOption && step % 20 == 0 )
    {
      sprintf(buffer,"Solution at time t=%e",t);
      psp.set(GI_TOP_LABEL,buffer);  // set title
      PlotIt::contour(ps, u,psp );
    }

    u+=dt*( (-a)*u.x()+(-b)*u.y()+nu*(u.xx()+u.yy()) );
    if( forcingOption > 0 )
    { // **** Here we add on dt[ U_t + a U_x + b U_y - nu( U_xx + U_yy ) ] ******
      u(I1,I2,I3)+=dt*(exact.t(mg,I1,I2,I3,0,t)
             + a*exact.x(mg,I1,I2,I3,0,t) + b*exact.y(mg,I1,I2,I3,0,t)
	     - nu*( exact.xx(mg,I1,I2,I3,0,t) + exact.yy(mg,I1,I2,I3,0,t) ) );
    }
    t+=dt;
    // apply Boundary conditions, this will set u=exact if forcingOption>0
    u.applyBoundaryCondition(0,BCTypes::dirichlet,BCTypes::allBoundaries,0.,t);   
    // fix up corners, periodic update:
    u.finishBoundaryConditions();                                      

    real error = max(abs( u(I1,I2,I3)-exact(mg,I1,I2,I3,0,t)));
    printf("t=%6.3f error =%e \n",t,error);
  }
  
  Overture::finish();          
  return 0;
}

