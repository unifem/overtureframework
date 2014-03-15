//===============================================================================
//  Coefficient Matrix Example 
//    Solve a system of equations on a MappedGrid
//==============================================================================
#include "Overture.h"  
#include "MappedGridFiniteVolumeOperators.h"
#include "Oges.h"
#include "SquareMapping.h"
#include "Annulus.h"
#include "OGPolyFunction.h"
#include "display.h"
#include "testUtils.h"
#include "GenericGraphicsInterface.h"

#undef ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )
int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  Display display;
  
  int nr, ns, nghost0, nghost1, degreeOfSpacePolynomial;
  cout << 
    "Enter nr, ns (number of grid lines), nghost0, nghost1 (numberOfGhost lines), degreeOfSpacePolynomial\n";
  cin >> nr >> ns >> nghost0 >> nghost1 >> degreeOfSpacePolynomial;

  display.interactivelySetInteractiveDisplay("Set display (o=off)");


  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  GridFunctionParameters::GridFunctionType cellCentered     = GridFunctionParameters::cellCentered,
                                           defaultCentering = GridFunctionParameters::defaultCentering;

//  AnnulusMapping map;   // switch this with the line below to get an Annulus; CHANGE BCs TOO!
//  SquareMapping map;
   Mapping *mapping= NULL;

  aString mappingType;
  cout << "TCM2: Annulus (a) or Square (s) ";
  cin >> mappingType;
  if (mappingType(0,0) == "s")
    mapping = new SquareMapping;
  else
    mapping = new AnnulusMapping;

  Mapping &map = *mapping;

  map.setGridDimensions(axis1,nr);
  map.setGridDimensions(axis2,ns);
    
  MappedGrid mg(map);
  mg.changeToAllCellCentered();

  int side;
  for(side=Start; side<=End; side++ )
  {
  mg.numberOfGhostPoints()(side,axis1) = nghost0;
  mg.numberOfGhostPoints()(side,axis2) = nghost1;
  }
  
  mg.update(
    MappedGrid::THEcenter
    | MappedGrid::THEvertex
    | MappedGrid::THEfaceNormal
    | MappedGrid::THEcellVolume
    | MappedGrid::THEcenterNormal
    | MappedGrid::THEcenterArea
    | MappedGrid::THEfaceArea
    | MappedGrid::THEmask
    | MappedGrid::THEcenterBoundaryNormal
    ,
    MappedGrid::COMPUTEgeometryAsNeeded
    | MappedGrid::USEdifferenceApproximation
    );

   // ========================================
   // GenericGraphicsInterface
   // ========================================

   bool openWindow = FALSE;
   GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("FVO Implicit Operator Tests", openWindow);
   GraphicsParameters psp;

   bool PLOT_ON = FALSE;
   aString yes = "n";
   cout << "TCM2: Plotting on? ";
   cin >> yes;
   if (yes(0,0) == "y"){
     PLOT_ON = TRUE;
     ps.createWindow();
   }
  
   if (PLOT_ON) ps.plot (mg);

  // label boundary conditions (note 990105, use inflow to test dirichlet forcing below)
  const int inflow=1, outflow=2, wall=3, periodic=-1;

  if (mappingType(0,0) == "s")
  {
    mg.boundaryCondition()(Start,axis1)=inflow;
    mg.boundaryCondition()(End  ,axis1)=inflow; //outflow;
    mg.boundaryCondition()(Start,axis2)=inflow; //wall;
    mg.boundaryCondition()(End  ,axis2)=inflow; //wall;
  }
  else
//... annulus
  {
    
    mg.boundaryCondition()(Start,axis1) = periodic;
    mg.boundaryCondition()(End  ,axis1) = periodic;
    mg.boundaryCondition()(Start,axis2) = wall;
    mg.boundaryCondition()(End  ,axis2) = wall;
  }
  

  // create a twilight-zone function for checking errors
  int degreeOfTimePolynomial = 1;

  int numberOfComponents = mg.numberOfDimensions();
  OGPolyFunction exact(degreeOfSpacePolynomial,
		       mg.numberOfDimensions(),
		       numberOfComponents,
		       degreeOfTimePolynomial);

  // make a grid function to hold the coefficients
  Range all;
  int stencilSize=pow(3,mg.numberOfDimensions());

  int numberOfComponentsForCoefficients=2;
//  int numberOfComponentsForCoefficients = 1;
  Index Components(0,numberOfComponentsForCoefficients);

  int stencilDimension = stencilSize*SQR(numberOfComponentsForCoefficients);

  //...declare and initialize coefficient  matrix
  realMappedGridFunction coeff(mg,stencilDimension,all,all,all); 
  int numberOfGhostLines=1;  // we will solve for values including the first ghostline
  coeff.setIsACoefficientMatrix (TRUE, stencilSize, numberOfGhostLines, numberOfComponentsForCoefficients);
  coeff=0.;

  //...declare and initialize solution and right-hand-side
  realMappedGridFunction 
    u(mg,cellCentered,numberOfComponentsForCoefficients),
    f(mg,cellCentered,numberOfComponentsForCoefficients);

  u = (real)0.0;
  f = (real)0.0;

  //... create operators and associate them with coeff
  MappedGridFiniteVolumeOperators op(mg);                           
  op.setStencilSize(stencilSize);
  op.setNumberOfComponentsForCoefficients(numberOfComponentsForCoefficients);
  coeff.setOperators(op);

  // Form a system of equations for (u,v)
  //     a1(  u_xx + u_yy ) + a2*v_x -a6*u = f_0
  //     a3(  v_xx + v_yy ) + a4*u_y -a7*u    = f_1
  //  BC's:   u=given   on all boundaries
  //          v=given   on inflow
  //          v.n=given on walls

//const real a1=1., a2=1., a3=1., a4=1., a5 = 0.;
  real a1=1., a2=1., a3=1., a4=1., a5 = 1., a6=1., a7=1., a8=1., a9=1.;

  aString answer;
  cout << "  a1(  u_xx + u_yy ) + a4*u_y + a2*u_x + a8*v_x + a9*v_y - a6*u = f_0 " << endl;
  cout << "  a3(  v_xx + v_yy ) + a6*u_y          - a7*u                   = f_1 " << endl;
  cout 
    << " a1 = " << a1   
    << " a2 = " << a2   
    << " a3 = " << a3   
    << " a4 = " << a4   
    << " a5 = " << a5  
    << " a6 = " << a6
    << " a7 = " << a7 
    << " a8 = " << a8
    << " a9 = " << a9 << " Change (y/n)? ";
  cin >> answer;
  while (answer(0,0) == "y")
  {
    cout << " Enter a1,a2,...a9: ";
    cin  >> a1 >> a2 >> a3 >> a4 >> a5 >> a6 >> a7 >> a8 >> a9;
    cout 
      << " a1 = " << a1   
      << " a2 = " << a2   
      << " a3 = " << a3   
      << " a4 = " << a4   
      << " a5 = " << a5  
      << " a6 = " << a6
      << " a7 = " << a7 
      << " a8 = " << a8
      << " a9 = " << a9 << "Change (y/n)? ";
    cin >> answer;
  }
  

  const int eqn0=0;    // labels equation 0
  const int eqn1=1;    // labels equation 1
  const int uc=0, vc=1;  // labels for the u and v components
  coeff = 
    + a1 * op.laplacianCoefficients(all,all,all,eqn0,uc) 
    + a2 * op.xCoefficients        (all,all,all,eqn0,uc) 
    + a4 * op.yCoefficients        (all,all,all,eqn0,uc)
    + a8 * op.xCoefficients        (all,all,all,eqn0,vc)
    + a9 * op.yCoefficients        (all,all,all,eqn0,vc)
    - a6 * op.identityCoefficients (all,all,all,eqn0,uc) 
    + a3 * op.laplacianCoefficients(all,all,all,eqn1,vc)
    - a7 * op.identityCoefficients (all,all,all,eqn1,vc)
    + a5 * op.yCoefficients        (all,all,all,eqn1,uc)
    ;

  display.display(coeff,"Here is coeff after assigning interior equations ");

  //... assign the right-hand-side
  Index I1,I2,I3;
  getIndex(mg.indexRange(),I1,I2,I3);  
  f(I1,I2,I3,eqn0) =
    + a1 * (exact.xx(mg,I1,I2,I3,uc) + exact.yy(mg,I1,I2,I3,uc)) 
    + a2 *  exact.x (mg,I1,I2,I3,uc) 
    + a4 *  exact.y (mg,I1,I2,I3,uc)
    + a8 *  exact.x (mg,I1,I2,I3,vc)
    + a9 *  exact.y (mg,I1,I2,I3,vc)
    - a6 *  exact   (mg,I1,I2,I3,uc)
    ;
  
  if (numberOfComponentsForCoefficients>1)
    f(I1,I2,I3,eqn1)=
      + a3 * (exact.xx(mg,I1,I2,I3,vc) + exact.yy(mg,I1,I2,I3,vc)) 
      - a7 *  exact   (mg,I1,I2,I3,vc)
      + a5 *  exact.y (mg,I1,I2,I3,uc)
    ;


  display.display(f,"Here is the rhs before BCs");


  //...now set the boundary conditions

  coeff.applyBoundaryConditionCoefficients(eqn0, uc, dirichlet,  allBoundaries);  


    display.display(coeff,"Here is coeff after dirichlet BC's for component uc ");

  if (numberOfComponentsForCoefficients>1)
  {
//  coeff.applyBoundaryConditionCoefficients(eqn1, vc, dirichlet, allBoundaries);
    
    coeff.applyBoundaryConditionCoefficients(eqn1, vc, dirichlet,  inflow);
    coeff.applyBoundaryConditionCoefficients(eqn1, vc, dirichlet,  outflow);
    coeff.applyBoundaryConditionCoefficients(eqn1, vc, neumann,     wall);
  }
  
    display.display(coeff,"Here is coeff after BC's for component vc ");

  coeff.finishBoundaryConditions();

    display.display(coeff, "Here is coeff=laplacianCoefficients after finishBCs");


  //...assign rhs for boundary conditions
  int axis;
  real zero     = (real)0.0;

  GridFunctionParameters::GridFunctionType faceCenteredAxis[3];
  faceCenteredAxis[0] = GridFunctionParameters::faceCenteredAxis1;
  faceCenteredAxis[1] = GridFunctionParameters::faceCenteredAxis2;  
  faceCenteredAxis[2] = GridFunctionParameters::faceCenteredAxis3;

  Index Ib1,Ib2,Ib3;
  Index Ig1,Ig2,Ig3;
  Index If1,If2,If3;
  
  //...note: the logic below is not general
  //   assume dirichlet condition on uc component
  //   assume inflow means dirichlet and everything else means neumann on vc component

  ForBoundary(side,axis)
  {
    if( mg.boundaryCondition()(side,axis) > 0  )
    {
      getGhostIndex(mg.indexRange(), side, axis, Ig1, Ig2, Ig3);
      getGhostIndex(mg.indexRange(), side, axis, If1, If2, If3, side);

      f(Ig1,Ig2,Ig3,eqn0) = exact (mg, If1, If2, If3, uc, zero, faceCenteredAxis[axis]);

      if (numberOfComponentsForCoefficients>1)
      {
	if( mg.boundaryCondition()(side,axis)==inflow )
	{
	  f(Ig1,Ig2,Ig3,eqn1) = exact(mg, If1, If2, If3, vc, zero, faceCenteredAxis[axis]);
	}
	else
	{
	  // for Neumann BC's -- fill in f on first ghostline
	  
	  realArray & normal = mg.centerBoundaryNormal(side,axis);
	  if( mg.numberOfDimensions()==2 )
	    f(Ig1,Ig2,Ig3,1)= 
	      normal(If1,If2,If3,xAxis) * exact.x(mg, If1, If2, If3, vc, zero, faceCenteredAxis[axis]) +
	      normal(If1,If2,If3,yAxis) * exact.y(mg, If1, If2, If3, vc, zero, faceCenteredAxis[axis]) ;
	  else
	    f(Ig1,Ig2,Ig3,1)=
	      normal(If1,If2,If3,xAxis) * exact.x(mg, If1, If2, If3, vc,  zero, faceCenteredAxis[axis]) +
	      normal(If1,If2,If3,yAxis) * exact.y(mg, If1, If2, If3, vc,  zero, faceCenteredAxis[axis]) +
	      normal(If1,If2,If3,zAxis) * exact.z(mg, If1, If2, If3, vc,  zero, faceCenteredAxis[axis]) ;
	}
      }
    }
  }

    display.display(f,"Here is the rhs after bcs");

//  coeff.display ("Here is the coeff matrix going into Oges");
    

  Oges solver( mg );                     // create a solver
  solver.setCoefficientArray( coeff );   // supply coefficients to solver
  solver.solve( u,f );                   // solve the equations

  getIndex(mg.indexRange(),I1,I2,I3,1);

  display.display(u,"Here is the solution u");
  
  psp.set (GI_TOP_LABEL, "computed solution");
  psp.set (GI_COMPONENT_FOR_CONTOURS, 0);
  if (PLOT_ON) PlotIt::contour(ps,u, psp);

  display.display(exact(mg,I1,I2,I3,Range(0,numberOfComponentsForCoefficients-1)),"Here is the exact solution");
    
  realMappedGridFunction exactgf (mg, defaultCentering, numberOfComponentsForCoefficients);
  realMappedGridFunction error   (mg, defaultCentering, numberOfComponentsForCoefficients);
  exactgf = 0.;
  error   = 0.;
  
  exactgf(I1,I2,I3,Components) = exact(mg,I1,I2,I3,Components);
  error  (I1,I2,I3,Components) = u    (   I1,I2,I3,Components) - exactgf(I1,I2,I3,Components);
  display.display (error, "Here is the error");

  psp.set (GI_TOP_LABEL, "computed error");
  psp.set (GI_COMPONENT_FOR_CONTOURS, 0);
  if (PLOT_ON) PlotIt::contour(ps,error, psp);
  
  printMaxNormOfDifference (u, exactgf);

  delete mapping; 

  return(0);

}
