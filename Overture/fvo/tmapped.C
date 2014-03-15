//========================================
// /n/c3serveo/dlb/res/proj/newClasses/tmapped.C
//
// Author:		D.L.Brown
// Date Created:	960112
//
// Purpose:
//	new test driver for MappedGridFiniteVolumeOperators
//========================================

#include "Overture.h"
#include "GenericGraphicsInterface.h"
#include "OGgetIndex.h"
#include "davidsReal.h"
#include "axisDefs.h"
#include "loops.h"
#include "MappedGridFiniteVolumeOperators.h"
#include "billsMergeMacro.h"

#undef BOUNDS_CHECK
#define BOUNDS_CHECK	//A++ bounds check on

//... include some useful utilities (norms, etc.)
#include "testUtils.h"

int main (int args, char **argv)
{
  
//...Declarations

  aString yes = "y";
  Index J1,J2,J3;
  int ianswer;
  int axis;
  REAL pi = 3.1415927;
  REAL twopi = 2.0*pi;

  GridFunctionParameters::GridFunctionType cellCentered = GridFunctionParameters::cellCentered;  
  GridFunctionParameters cellCenteredOutput( GridFunctionParameters::cellCentered);  
  Display display;

//...Banner

  printf ("\n========================================\n");
  printf ("  TMAPPED: test MappedGridFiniteVolumeOperators \n");
  printf ("                 Class  \n");
  printf ("             9 6 0 1 1 7 \n");
  printf ("                   DLB\n");
  printf ("========================================\n\n");

  display.interactivelySetInteractiveDisplay ("interactivelySetInteractiveDisplay");

//...argument line parameters

  int ierr = 0;
  if (args < 2)
    {
      cerr << "Usage: tmapped database_file_name" << endl;
      ierr = -1;
      return (ierr);
    }
  aString nameOfOGFile 		= argv[1]; nameOfOGFile =  nameOfOGFile;

  cout << "tmapped.x: Opening " << nameOfOGFile << " ... " << endl;


//...sync io subsystems

  ios::sync_with_stdio();

//...find a mapped grid

  CompositeGrid gc;
  getFromADataBase (gc, nameOfOGFile);
  
  gc.update ();
  MappedGrid & mg = gc[0];

//...grid-dependent declarations

  int numberOfDimensions = mg.numberOfDimensions();

  REALMappedGridFunction x, y, z;
  x.link (mg.center(), Range (xComponent,xComponent));
  y.link (mg.center(), Range (yComponent,yComponent));

  if (numberOfDimensions == 3)
    z.link (mg.center(), Range (zComponent, zComponent));

  MappedGridFiniteVolumeOperators operators (mg);
	
//========================================
//...set up the exact functions and their derivatives
//========================================

  enum exactFunctionType
  {
    trigFunction,
    polyFunction,
    linearFunction,
    quadraticFunction,
    numberOfExactFunctionTypes
  };

//...function
  REALMappedGridFunction u   (mg, cellCentered, numberOfDimensions);
  u.setOperators (operators);
//...first derivatives
  REALMappedGridFunction Du  (mg, cellCentered, numberOfDimensions, numberOfDimensions);
//...second derivatives
  REALMappedGridFunction DDu (mg, cellCentered, numberOfDimensions, numberOfDimensions, numberOfDimensions);

  exactFunctionType fType;
  
  cout << "Trig (0), Polynomial (1), Linear (2) or Quadratic (3) exact function? ";
  cin >> ianswer;

  getIndex(mg.dimension(), J1, J2, J3);

  switch ((exactFunctionType)ianswer)
  {
  case trigFunction:
    fType = trigFunction;

    u  (J1,J2,J3,xComponent) = sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
    u  (J1,J2,J3,yComponent) = cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
    
    Du (J1,J2,J3,xComponent,xAxis) =  twopi*cos(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
    Du (J1,J2,J3,xComponent,yAxis) = -twopi*sin(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
    Du (J1,J2,J3,yComponent,xAxis) = -twopi*sin(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
    Du (J1,J2,J3,yComponent,yAxis) =  twopi*cos(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));

    DDu(J1,J2,J3,xComponent,xAxis,xAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
    DDu(J1,J2,J3,xComponent,yAxis,xAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
    DDu(J1,J2,J3,xComponent,xAxis,yAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
    DDu(J1,J2,J3,xComponent,yAxis,yAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));

    DDu(J1,J2,J3,yComponent,xAxis,xAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
    DDu(J1,J2,J3,yComponent,yAxis,xAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
    DDu(J1,J2,J3,yComponent,xAxis,yAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
    DDu(J1,J2,J3,yComponent,yAxis,yAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
    
    break;
    
  case polyFunction:

    fType = polyFunction;
    
    u  (J1,J2,J3,xComponent) = pow(x(J1,J2,J3),3) * pow(y(J1,J2,J3),2);
    u  (J1,J2,J3,yComponent) = pow(x(J1,J2,J3),2) * pow(y(J1,J2,J3),3);

    Du (J1,J2,J3,xComponent,xAxis) = 3. * pow(x(J1,J2,J3),2) * pow(y(J1,J2,J3),2);
    Du (J1,J2,J3,xComponent,yAxis) = 2. * pow(x(J1,J2,J3),3) *     y(J1,J2,J3);
    Du (J1,J2,J3,yComponent,xAxis) = 2. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),3);
    Du (J1,J2,J3,yComponent,yAxis) = 3. * pow(x(J1,J2,J3),2) * pow(y(J1,J2,J3),2);
    
    DDu(J1,J2,J3,xComponent,xAxis,xAxis) = 6. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),2);
    DDu(J1,J2,J3,xComponent,yAxis,xAxis) = 6. * pow(x(J1,J2,J3),2) *     y(J1,J2,J3);
    DDu(J1,J2,J3,xComponent,xAxis,yAxis) = 6. * pow(x(J1,J2,J3),2) *     y(J1,J2,J3);
    DDu(J1,J2,J3,xComponent,yAxis,yAxis) = 2. * pow(x(J1,J2,J3),3);
    
    DDu(J1,J2,J3,yComponent,xAxis,xAxis) = 2. *                      pow(y(J1,J2,J3),3);
    DDu(J1,J2,J3,yComponent,yAxis,xAxis) = 6. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),2);
    DDu(J1,J2,J3,yComponent,xAxis,yAxis) = 6. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),2);
    DDu(J1,J2,J3,yComponent,yAxis,yAxis) = 6. * pow(x(J1,J2,J3),2) *     y(J1,J2,J3)   ;

    break;

  case linearFunction:
    
    fType = linearFunction;
    
    u  (J1,J2,J3,xComponent) = x(J1,J2,J3);
    u  (J1,J2,J3,yComponent) = y(J1,J2,J3);
    
    Du (J1,J2,J3,xComponent,xAxis) = 1.;
    Du (J1,J2,J3,xComponent,yAxis) = 0.;
    Du (J1,J2,J3,yComponent,xAxis) = 0.;
    Du (J1,J2,J3,yComponent,yAxis) = 1.;
    
    DDu = 0.;
    
    break;

  case quadraticFunction:
    
    fType = quadraticFunction;
    
    u  (J1,J2,J3,xComponent) = pow(x(J1,J2,J3),2);
    u  (J1,J2,J3,yComponent) = pow(y(J1,J2,J3),2);
    
    Du (J1,J2,J3,xComponent,xAxis) = 2.*x(J1,J2,J3);
    Du (J1,J2,J3,xComponent,yAxis) = 0.;
    Du (J1,J2,J3,yComponent,xAxis) = 0.;
    Du (J1,J2,J3,yComponent,yAxis) = 2.*y(J1,J2,J3);
    
    DDu = 0.;
    DDu(J1,J2,J3,xComponent,xAxis,xAxis) = 2.;
    DDu(J1,J2,J3,yComponent,yAxis,yAxis) = 2.;
    
    break;
    
  default:
    cout << "Illegal value input: " << ianswer;
    exit (-1);
  }

  display.display (u, "Here is u");
  display.display (Du, "Here is Du");
  display.display (DDu, "Here is DDu");
  
//========================================  
//...Test MappedGridFiniteVolumeOperators
//========================================

  REALMappedGridFunction computed(mg, cellCentered, numberOfDimensions);
  REALMappedGridFunction exact   (mg, cellCentered, numberOfDimensions);
  Index NC;
  NC = Range (0,numberOfDimensions-1);
  int scalarIndex = 1;

//========================================
  cout << "Not testing contravariantVelocity..." << endl;
//========================================

//========================================
  cout << "Not testing normalVelocity..." << endl;
//========================================

//========================================
  cout << "testing cellsToFaces..." << endl;
//========================================

  realMappedGridFunction ctf;
  ctf = u.cellsToFaces();
  display.display (ctf, "cellsTofaces of u");
  

//========================================
  cout << "Testing convective derivative..." << endl;
//========================================

//  computed = operators.convectiveDerivative (u);
  computed = u.convectiveDerivative ();

  exact.updateToMatchGrid(mg, cellCentered, numberOfDimensions);
  int component;
  for (component=0; component<numberOfDimensions; component++)
  {
    exact(J1,J2,J3,component) = u(J1,J2,J3,uComponent)*Du(J1,J2,J3,component,xAxis) + u(J1,J2,J3,vComponent)*Du(J1,J2,J3,component,yAxis);
  }

  display.display (computed, "Computed convective derivative");
  display.display (exact, "Exact convective derivative");

  cout << "     ";
  printMaxNormOfDifference (computed, exact);

//========================================
  cout << "Testing vorticity..." << endl;
//========================================

//  computed = operators.vorticity (u);
  // *** 980715 try this

  computed.updateToMatchGrid (mg, cellCentered, 1);
  computed = u.vorticity();

  exact.updateToMatchGrid (mg, cellCentered, scalarIndex);
  exact(J1,J2,J3) = - Du(J1,J2,J3,vComponent,xAxis) + Du(J1,J2,J3,uComponent,yAxis);

  display.display (computed, "Computed vorticity");
  display.display (exact   , "Exact vorticity");

  cout << "      ";
  printMaxNormOfDifference (computed, exact);

//========================================
  cout << "Testing divergence..." << endl;
//========================================

//  computed = operators.div (u);
  computed.updateToMatchGrid (mg, cellCentered, 1);
  computed = u.div();

  exact.updateToMatchGrid (mg, cellCentered, scalarIndex);
  exact(J1,J2,J3) = Du(J1,J2,J3,uComponent,xAxis) + Du(J1,J2,J3,vComponent,yAxis);

  display.display (computed, "Computed divergence");
  display.display (exact   , "Exact divergence");

  cout << "      ";
  printMaxNormOfDifference (computed, exact);

//========================================
  cout << "Testing gradient..." << endl;
//========================================

//  computed = operators.grad(u);
  computed.updateToMatchGrid (mg, cellCentered, numberOfDimensions, numberOfDimensions);
//  computed = u.grad();

  computed = u.grad(cellCenteredOutput);
  
//computed = operators.grad (u, cellCentered);
  

  exact.updateToMatchGrid (mg, cellCentered, numberOfDimensions, numberOfDimensions);
  for (component=0; component<numberOfDimensions; component++)
  {
    exact(J1,J2,J3,Range(0,numberOfDimensions-1),component) = Du(J1,J2,J3,Range(0,numberOfDimensions-1),component);
  }

  display.display (computed, "Computed gradient");
  display.display (exact   , "Exact gradient");

  cout << "      ";
  printMaxNormOfDifference (computed, exact);

//========================================
  cout << "Testing laplacian..." << endl;
//========================================

//980720; check laplacian bug here instead

  Index I1,I2,I3;
  realMappedGridFunction cv;
  cv = mg.cellVolume();
  display.display (cv, "cv in main");
  realMappedGridFunction result;
  result = operators.cellsToFaces (cv);
  getIndex (result, 0, I1,I2,I3);
  

//  computed = operators.laplacian(u);
  computed.updateToMatchGrid (mg, cellCentered, numberOfDimensions);
  computed = u.laplacian();

  exact.updateToMatchGrid (mg, cellCentered, numberOfDimensions);
  exact(J1,J2,J3,uComponent) = DDu(J1,J2,J3,uComponent,xAxis,xAxis) + DDu(J1,J2,J3,uComponent,yAxis,yAxis);
  exact(J1,J2,J3,vComponent) = DDu(J1,J2,J3,vComponent,xAxis,xAxis) + DDu(J1,J2,J3,vComponent,yAxis,yAxis);

  display.display (computed, "Computed laplacian");
  display.display (exact   , "Exact laplacian");

  cout << "      ";
  printMaxNormOfDifference (computed, exact);

}




