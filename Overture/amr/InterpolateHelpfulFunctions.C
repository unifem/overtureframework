#include "InterpolateHelpfulFunctions.h"
#include "NameList.h"

void
setValues (realMappedGridFunction &u,
	   const Index & C,
	   OGFunction* e)
{
  real twoPi = 6.283185308;
  real xFreq = 2., yFreq = 1.;
  real initialTime = (real) 0.;
  OGFunction& exact = *e;

  real gaussLocX = .75, gaussLocY = .75;

  Index I1,I2,I3;
  MappedGrid *mappedGrid = u.mappedGrid;
  int numberOfDimensions = mappedGrid->numberOfDimensions();
  realMappedGridFunction x,y,z;
  mappedGrid->update(MappedGrid::THEcenter);
  
  /*                     */ x.link (mappedGrid->center(), Range(0,0));
  if (numberOfDimensions>1) y.link (mappedGrid->center(), Range(1,1));
  if (numberOfDimensions>2) z.link (mappedGrid->center(), Range(2,2));

//   Display display;
//   bool debug = LogicalFalse;
//   if (debug){
//     display.display (x, "here is x");
//     display.display (y, "here is y");
//     display.display (z, "here is z");
//   }

  IntegerArray nOG = mappedGrid->numberOfGhostPoints();
  getIndex (mappedGrid->indexRange(), I1, I2, I3, nOG(0,0), nOG(0,1), nOG(0,2));

  int c0 = C.getBase();
  int c1 = C.getBound();
  int c;
    
  for (c=c0; c<=c1; c++)
    u(I1,I2,I3,c) = exact(*mappedGrid, I1, I2, I3, c, initialTime);
  
}

void
setValues (realGridCollectionFunction &adaptiveGridSolution, 
	   const Index & C, 
	   OGFunction* e)
{
  int numberOfRefinementLevels = adaptiveGridSolution.numberOfRefinementLevels();
  int level, grid;
  realMappedGridFunction u;
  Range allComponents(adaptiveGridSolution.getComponentBase(0), adaptiveGridSolution.getComponentBound(0));

  for (level=0; level<numberOfRefinementLevels; level++)
  {
    GridCollection& gc = *(adaptiveGridSolution.refinementLevel[level].getGridCollection());
    int numberOfGrids  = gc.numberOfGrids();
    for (grid=0; grid<numberOfGrids; grid++)

    {
      u.link (adaptiveGridSolution.refinementLevel[level][grid], allComponents);
      setValues (u, C, e);
    }
  }
}



//==============================================================================
OGFunction *
setTwilightZoneFlowFunction (const TwilightZoneFlowFunctionType & TZType, 
			     const int & numberOfComponents,
			     const int & numberOfDimensions)

// /Purpose:
//    return a pointer to a TwilightZoneFlowFunction; this version
//    new's a TZFunction of type TZType. 
//
//==============================================================================
{

  OGFunction * twilightZoneFlowFunction ;
  twilightZoneFlowFunction = NULL;
  
  int degreeOfSpacePolynomial = 2;
//int numberOfComponents = numberOfDimensions+3;
  int degreeOfTimePolynomial = 0;
  real fx0 = 2.;
  real fy0 = 2.;
  real fz0 = 0.;
  real ft0 = 0.;

  RealArray a, c; //polynomial coefficients
  RealArray fx,fy,fz,ft; //trig coefficients
  bool setPolyCoeffs = FALSE;
  bool setTrigCoeffs = FALSE;

  NameList nl;
  aString answer(80), name(80);

  Index allComponents(0,numberOfComponents);
  real aValue, cValue, fxValue, fyValue, fzValue, ftValue;
  int i,j,k,m,n;

  switch (TZType)
  {
    
    // ====================
    case none:
    // ====================
    break;

    // ====================
    case PolyFunction:
    // ====================
    printf (

      " Set TwilightZone Parameters: \n"
      "================================================================================\n"
      "   NAME                        type    current value \n"
      "Polynomial Parameters: \n"
      "\n"
      "degreeOfSpacePolynomial [sp]   (int)   %4i \n"
      "degreeOfTimePolynomial  [tp]   (int)   %4i \n"
      "[set]PolyCoeffs                (bool)  %4i \n"
      "\n"
      "================================================================================\n\n",

      degreeOfSpacePolynomial,
      degreeOfTimePolynomial,
      setPolyCoeffs
      );
  
    for (;;)
    {
      cout << "Enter changes to variables, exit to continue" << endl;
      cin >> answer;
    
      if (answer == "exit" || answer(0,1) == "ex") break;
    
      nl.getVariableName (answer, name);

      if ( name(0,1) == "sp" )
      {
	degreeOfSpacePolynomial = nl.intValue(answer);
	cout << "degreeOfSpacePolynomial = " << degreeOfSpacePolynomial << endl;
      }
    
      else if ( name(0,1) == "tp" )
      {
	degreeOfTimePolynomial = nl.intValue(answer);
	cout << "degreeOfTimePolynomial = " << degreeOfTimePolynomial << endl;
      }

      else if ( name(0,2) == "set" )
      {
	setPolyCoeffs = nl.intValue(answer);
	cout << "setPolyCoeffs = " << setPolyCoeffs << endl;
      }
      else
	 cout << "unknown response [" << name << "]" << endl;
    }

    printf (

      "  Final Values for TwilightZone Parameters: \n"
      "================================================================================\n"
      "   NAME                        type    current value \n"
      "Polynomial Parameters: \n"
      "\n"
      "degreeOfSpacePolynomial [sp]   (int)   %4i \n"
      "degreeOfTimePolynomial  [tp]   (int)   %4i \n"
      "setPolyCoeffs                (bool)  %4i \n"
      "\n"
      "================================================================================\n\n",

      degreeOfSpacePolynomial,
      degreeOfTimePolynomial,
      setPolyCoeffs
      );


    if (setPolyCoeffs)
    {
    
      a.resize(5,numberOfComponents);
      c.resize(5,5,5,numberOfComponents);
      a = 0;
      a(0,allComponents) = 1.;
      c = 0;
    
      for (;;)
      {
	cout << "\n==============================================================================\n" <<
	  "   Enter i,j,k,n,c(i,j,k,n)   for c(i,j,k,n)x^i*y^j*z^k, component n, i<0 to exit: " << endl;
      

	cin >> i >> j >> k >> n >> cValue;
	if (i<0) break;
      
	c(i,j,k,n) = cValue;
      }

      for (;;)
      {
	cout << "\n==============================================================================\n" <<
	  "   Enter m,n,a(m,n) for a(m,n)*t^m, component n, m<0 to exit: " << endl;
      
	cin >> m >> n >> aValue;
	if (m<0) break;
      
	a(m,n) = aValue;
      }
    }

    if (setPolyCoeffs)
    {
      c.display("Here is c");
      a.display("Here is a");
      
      twilightZoneFlowFunction = new OGPolyFunction;
      ((OGPolyFunction*)twilightZoneFlowFunction)->setCoefficients (c,a);
    }
    else
    {
      twilightZoneFlowFunction = 
	new OGPolyFunction (degreeOfSpacePolynomial,
			    numberOfDimensions, 
			    numberOfComponents, 
			    degreeOfTimePolynomial);
    }
    
    
    
    break;
    
    // ====================
    case TrigFunction:
    // ====================
    printf (

      " Set TwilightZone Parameters: \n"
      "================================================================================\n"
      "   NAME                        type    current value \n"
      "Trig Parameters: \n"
      "\n"
      "  fx0                          (real)  %4e \n"
      "  fy0                          (real)  %4e \n"
      "  fz0                          (real)  %4e \n"
      "  ft0                          (real)  %4e \n"
      "[set]TrigCoeffs                (bool)  %4i \n"
      "================================================================================\n\n",

      fx0,
      fy0,
      fz0,
      ft0,
      setTrigCoeffs
      );
    for (;;)
    {
      cout << "Enter changes to variables, exit to continue" << endl;
      cin >> answer;
    
      if (answer == "exit" || answer(0,1) == "ex") break;
    
      nl.getVariableName (answer, name);


      if ( name(0,3) == "setT" )
      {
	setTrigCoeffs = nl.intValue(answer);
	cout << "setTrigCoeffs = " << setTrigCoeffs << endl;
      }
    
      else if ( name(0,2) == "fx0" )
      {
	fx0 = nl.realValue(answer);
	cout << "fx0 = " << fx0 << endl;
      }
      else if ( name(0,2) == "fy0" )
      {
	fy0 = nl.realValue(answer);
	cout << "fy0 = " << fy0 << endl;
      }
      else if ( name(0,2) == "fz0" )
      {
	fz0 = nl.realValue(answer);
	cout << "fz0 = " << fz0 << endl;
      }
      else if ( name(0,2) == "ft0" )
      {
	ft0 = nl.realValue (answer);
	cout << "ft0 = " << ft0 << endl;
      }
            
      else
	 cout << "unknown response [" << name << "]" << endl;
    }

    printf (

      "  Final Values for TwilightZone Parameters: \n"
      "================================================================================\n"
      "   NAME                        type    current value \n"
      "Trig Parameters: \n"
      "\n"
      "  fx0                          (real)  %4e \n"
      "  fy0                          (real)  %4e \n"
      "  fz0                          (real)  %4e \n"
      "  ft0                          (real)  %4e \n"
      "  setTrigCoeffs                (int)   %4i \n"
      "================================================================================\n\n",

      fx0,
      fy0,
      fz0,
      ft0,
      setTrigCoeffs
      );

    if (setTrigCoeffs)
    {
      fx.resize(numberOfComponents);
      fy.resize(numberOfComponents);
      fz.resize(numberOfComponents);
      ft.resize(numberOfComponents);

      fx(allComponents) = (real) 0.0;
      fy(allComponents) = (real) 0.0;
      fz(allComponents) = (real) 0.0;
      ft(allComponents) = (real) 0.0;

      fx.display("Here is fx");
      fy.display("Here is fy");
      fz.display("Here is fz");
      ft.display("Here is ft");
        
      for (;;)
      {
      
	cout << "\n==============================================================================\n" <<
	  "     For cos(fx(n)*Pi*x) * cos(fy(n)*Pi*y) * cos(fz(n)*Pi*z) * cos(ft(n)*Pi*t \n" <<
	  "   Enter n,fx(n),fy(n),fz(n),ft(n) for component n, n=-1 to exit   " << endl;
      
	cin >> n >> fxValue >> fyValue >> fzValue >> ftValue;
	if (n<0) break;
       
	fx(n) = fxValue;
	if (numberOfDimensions>1) fy(n) = fyValue;
	if (numberOfDimensions>2) fz(n) = fzValue;
	ft(n) = ftValue;
	fx.display("Here is fx");
	if (numberOfDimensions>1) fy.display("Here is fy");
	if (numberOfDimensions>2) fz.display("Here is fz");
	ft.display("Here is ft");

      }
    }
  
    if (setTrigCoeffs)
    {
      fx.display("Here is fx");
      if (numberOfDimensions>1) fy.display("Here is fy");
      if (numberOfDimensions>2) fz.display("Here is fz");
      ft.display("Here is ft");
      
      twilightZoneFlowFunction = new OGTrigFunction (fx, fy, fz, ft);
    
    }
    else
    {
    
      twilightZoneFlowFunction = new OGTrigFunction (fx0, fy0, fz0, ft0, numberOfComponents);
    }
  
    break;
    
    default:
    break;
  }; 


  return (twilightZoneFlowFunction);
}
/*
void
makeMappedGrid (MappedGrid& grid, AMRProblemParameters& probParams)
{
  cout << "makeMappedGrid: dummy routine" << endl;
}
*/

/*
void
makeMappedGrid (MappedGrid& grid, AMRProblemParameters& probParams)
{
  
  int nx, ny, nz;
  nx = probParams.nx;
  ny = probParams.ny;
  nz = probParams.nz;
  
  real xmin = probParams.xmin;
  real xmax = probParams.xmax;
  real ymin = probParams.ymin;
  real ymax = probParams.ymax;
  
  MappingRC mapping;
  mapping = *(new SquareMapping (xmin, xmax, ymin, ymax)); 
  mapping.incrementReferenceCount();

  mapping.setGridDimensions(axis1,nx);               // axis1==0, set no. of grid points
  mapping.setGridDimensions(axis2,ny);               // axis2==1, set no. of grid points


  //...set Boundary Conditions
  enum BCType
  {
    periodic=-1,
    dirichlet=1,
    rotatedDirichlet=2
  };

  BCType bcType = (BCType) probParams.boundaryConditionChoice;

  int axis, side;  

  switch (bcType)
  {
  case periodic:

    mapping.setIsPeriodic (rAxis, Mapping::derivativePeriodic);
    mapping.setIsPeriodic (sAxis, Mapping::derivativePeriodic);

    break;
  
  case dirichlet:

    for (side=Start; side<=End; side++)
      for (axis=0; axis<mapping.getDomainDimension(); axis++)
	mapping.setBoundaryCondition (side, axis, wall);
    break;
      
  }
  //...Make the MappedGrid from the mapping
  grid.reference(mapping);                            
  grid.update();                                       

  mapping.decrementReferenceCount();
}
*/

