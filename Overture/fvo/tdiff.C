

//========================================
// /n/c3serveo/dlb/res/proj/newClasses/tdiff.C
//
// Author:		D.L.Brown
// Date Created:	960701
//
// Purpose:
//	new test driver for CompositeGridFiniteVolumeOperators
//========================================

#include "davidsReal.h"
#include "Overture.h"
#include "GenericGraphicsInterface.h"
#include "OGgetIndex.h"
#include "axisDefs.h"
#include "loops.h"
#include "MappedGridFiniteVolumeOperators.h"
#include "CompositeGridFiniteVolumeOperators.h"
#include "billsMergeMacro.h"
#include "Oges.h"
#include "BoundaryConditionParameters.h"

#undef BOUNDS_CHECK
#define BOUNDS_CHECK	//A++ bounds check on

//... include some useful utilities (norms, etc.)
#include "testUtils.h"

int main (int args, char **argv)
{
   Index::setBoundsCheck(on);
 
//...Declarations

  aString yes = "y";
  Index J1,J2,J3;
  int ianswer;
  int axis, side;
  int component;
  real pi = 3.1415927;
  real twopi = 2.0*pi;
  const Range all;
  real epsDensity = 0.1;
   real HALF = 0.5, QUARTER = 0.25;

  static const int 
    inflow=1,
    outflow=2,
    slip=3;


  const GridFunctionParameters general           = GridFunctionParameters::general; 
  const GridFunctionParameters vertexCentered    = GridFunctionParameters::vertexCentered; 
  const GridFunctionParameters cellCentered      = GridFunctionParameters::cellCentered; 
  const GridFunctionParameters faceCenteredAll   = GridFunctionParameters::faceCenteredAll; 
  const GridFunctionParameters faceCenteredAxis1 = GridFunctionParameters::faceCenteredAxis1; 
  const GridFunctionParameters faceCenteredAxis2 = GridFunctionParameters::faceCenteredAxis2; 
  const GridFunctionParameters faceCenteredAxis3 = GridFunctionParameters::faceCenteredAxis3; 


  Display display;

//...Banner

  printf ("\n========================================\n");
  printf ("  TCGFVO: test CompositeGridFiniteVolumeOperators first derivatives \n");
  printf ("                   Class  \n");
  printf ("               9 6 0 7 0 1 \n");
  printf ("                    DLB\n");
  printf ("========================================\n\n");

  display.interactivelySetInteractiveDisplay ("interactivelySetInteractiveDisplay");

//...argument line parameters

  int ierr = 0;
  if (args < 2)
    {
      cerr << "Usage: tmapped CMPGRD_datafile" << endl;
      ierr = -1;
      return (ierr);
    }
  aString nameOfOGFile 		= argv[1]; nameOfOGFile = "../ogen/" + nameOfOGFile;
//  aString nameOfDirectory 	= argv[2];
  cout << "tgc.x: Opening " << nameOfOGFile << " ... " << endl;


//...sync io subsystems

  ios::sync_with_stdio();

//...find a Composite grid (gridCollection)

//  MultigridCompositeGrid mgcog (nameOfOGFile, nameOfDirectory);
//  mgcog.update ();
//  CompositeGrid & gc = mgcog[0];

  CompositeGrid gc;
  getFromADataBase (gc, nameOfOGFile);
  gc.update(
	      CompositeGrid::THEcenter
	    | CompositeGrid::THEfaceNormal
	    | CompositeGrid::THEcellVolume
	    | CompositeGrid::THEcenterNormal
	    | CompositeGrid::THEvertex
	    ,
  	      CompositeGrid::COMPUTEgeometryAsNeeded
	    | CompositeGrid::USEdifferenceApproximation
	    );
  
  

//...grid-dependent declarations

  int numberOfDimensions = gc.numberOfDimensions;
  int nD = numberOfDimensions;

  realMappedGridFunction xVertex, yVertex, zVertex,
   xCell, yCell, zCell, xFace1, yFace1, zFace1, xFace2, yFace2, zFace2, xFace3, yFace3, zFace3;
  realMappedGridFunction x,y,z;
  CompositeGridFiniteVolumeOperators operators (gc);

	
//========================================
//...set up the exact functions and their derivatives
//========================================

  enum exactFunctionType
  {
    trigFunction,
    polyFunction,
    linearXFunction,
    linearYFunction,
    quadraticFunction,
    numberOfExactFunctionTypes
  };

//...function
   realCompositeGridFunction cN (gc, cellCentered, nD, nD);  // test the centerNormal
     
  realCompositeGridFunction uCell   (gc, cellCentered, nD);
  realCompositeGridFunction uFace1  (gc, faceCenteredAxis1, nD);
  realCompositeGridFunction uFace2  (gc, faceCenteredAxis2, nD);
  realCompositeGridFunction uFace3;
  if (nD==3) uFace3.updateToMatchGrid (gc, faceCenteredAxis3, nD);

  uCell.setOperators (operators);
  uFace1.setOperators (operators);
  uFace2.setOperators (operators);
  if (nD==3) uFace3.setOperators (operators);
  
  realCompositeGridFunction s   (gc, cellCentered); //scalar for divScalarGrad tests

//...first derivatives
  realCompositeGridFunction DuCell  (gc, cellCentered, nD, nD);
  realCompositeGridFunction DuFace1 (gc, faceCenteredAxis1, nD, nD);
  realCompositeGridFunction DuFace2 (gc, faceCenteredAxis2, nD, nD);
  realCompositeGridFunction DuFace3; if (nD==3) DuFace3.updateToMatchGrid (gc, faceCenteredAxis3, nD, nD);

  realCompositeGridFunction Ds  (gc, cellCentered, nD);

//...second derivatives
  realCompositeGridFunction DDuCell (gc, cellCentered, nD, nD, nD);
  realCompositeGridFunction DDuFace1 (gc, faceCenteredAxis1, nD, nD, nD);
  realCompositeGridFunction DDuFace2 (gc, faceCenteredAxis2, nD, nD, nD);
  realCompositeGridFunction DDuFace3; if (nD==3) DDuFace3.updateToMatchGrid (gc, faceCenteredAxis3, nD, nD, nD);

  realCompositeGridFunction DDs (gc, cellCentered, nD, nD);

  exactFunctionType fType;
  
  cout << "Trig (0), Polynomial (1), X-Linear (2), X-Cubic (3) or Quadratic (4) exact function? ";
  cin >> ianswer;

  int grid;
  int numberOfComponentGrids = gc.numberOfComponentGrids;

// ... Setup exact functions and derivatives

  ForAllGrids(grid)
  {
    Index K1,K2,K3;
    MappedGrid & mg = gc[grid];

    realMappedGridFunction::updateReturnValue result; 

    // ... this is just a bug workaround
/*
    result = xFace1.updateToMatchGrid (mg, all, all, all);
    result = yFace1.updateToMatchGrid (mg, all, all, all);
    result = xFace2.updateToMatchGrid (mg, all, all, all);
    result = yFace2.updateToMatchGrid (mg, all, all, all);
*/
    
    
    result =   xFace1.updateToMatchGrid (mg, faceCenteredAxis1);
    result =   yFace1.updateToMatchGrid (mg, faceCenteredAxis1);
    if (nD==3) zFace1.updateToMatchGrid (mg, faceCenteredAxis1);

    result =   xFace2.updateToMatchGrid (mg, faceCenteredAxis2);
    result =   yFace2.updateToMatchGrid (mg, faceCenteredAxis2);
    if (nD==3) zFace2.updateToMatchGrid (mg, faceCenteredAxis2);

    if (nD==3)
    {
      xFace3.updateToMatchGrid (mg, faceCenteredAxis3);
      yFace3.updateToMatchGrid (mg, faceCenteredAxis3);
      zFace3.updateToMatchGrid (mg, faceCenteredAxis3);
    }
  

    
    xCell.link (mg.center(), Range (xComponent,xComponent));
    yCell.link (mg.center(), Range (yComponent,yComponent));
    xVertex.link (mg.vertex(), Range (xComponent, xComponent));
    yVertex.link (mg.vertex(), Range (yComponent, yComponent));
    if (nD == 3)
    {
      zCell.link (mg.center(), Range (zComponent, zComponent));
      zVertex.link (mg.vertex(), Range (zComponent, zComponent));
    }

    getIndex(mg.dimension(), J1, J2, J3);

    // ... compute faceCentered coordinates

    if (nD==2)
    {
      K1 = J1;
      K2 = Range (J2.getBase(), J2.getBound()-1);
      K3 = J3;
      
      xFace1(K1,K2,K3) = HALF*(xVertex(K1,K2,K3) + xVertex(K1,K2+1,K3));
      yFace1(K1,K2,K3) = HALF*(yVertex(K1,K2,K3) + yVertex(K1,K2+1,K3));

      cN[grid](K1,K2,K3,0,1) = HALF*(mg.faceNormal()(K1,K2,K3,0,1) + mg.faceNormal()(K1,K2+1,K3,0,1));
      cN[grid](K1,K2,K3,1,1) = HALF*(mg.faceNormal()(K1,K2,K3,1,1) + mg.faceNormal()(K1,K2+1,K3,1,1));

      K1 = Range (J1.getBase(), J1.getBound()-1);
      K2 = J2;
      K3 = J3;
      
      xFace2(K1,K2,K3) = HALF*(xVertex(K1,K2,K3) + xVertex(K1+1,K2,K3));
      yFace2(K1,K2,K3) = HALF*(yVertex(K1,K2,K3) + yVertex(K1+1,K2,K3));

      cN[grid](K1,K2,K3,0,0) = HALF*(mg.faceNormal()(K1,K2,K3,0,0) + mg.faceNormal()(K1+1,K2,K3,0,0));
      cN[grid](K1,K2,K3,1,0) = HALF*(mg.faceNormal()(K1,K2,K3,1,0) + mg.faceNormal()(K1+1,K2,K3,1,0));
      
    }
    

    if (nD==3)
    {
      K1 = J1;
      K2 = Range (J2.getBase(), J2.getBound()-1);
      K3 = Range (J3.getBase(), J3.getBound()-1);

      xFace1(K1,K2,K3) = QUARTER*(
				  xVertex(K1,K2  ,K3) + xVertex(K1,K2  ,K3+1) +
				  xVertex(K1,K2+1,K3) + xVertex(K1,K2+1,K3+1));
      yFace1(K1,K2,K3) = QUARTER*(
				  yVertex(K1,K2  ,K3) + yVertex(K1,K2  ,K3+1) +
				  yVertex(K1,K2+1,K3) + yVertex(K1,K2+1,K3+1));
      zFace1(K1,K2,K3) = QUARTER*(
				  zVertex(K1,K2  ,K3) + zVertex(K1,K2  ,K3+1) +
				  zVertex(K1,K2+1,K3) + zVertex(K1,K2+1,K3+1));

      xFace2(K1,K2,K3) = QUARTER*(
				  xVertex(K1  ,K2,K3) + xVertex(K1  ,K2,K3+1) +
				  xVertex(K1+1,K2,K3) + xVertex(K1+1,K2,K3+1));
      yFace2(K1,K2,K3) = QUARTER*(
				  yVertex(K1  ,K2,K3) + yVertex(K1  ,K2,K3+1) +
				  yVertex(K1+1,K2,K3) + yVertex(K1+1,K2,K3+1));
      zFace2(K1,K2,K3) = QUARTER*(
				  zVertex(K1  ,K2,K3) + zVertex(K1  ,K2,K3+1) +
				  zVertex(K1+1,K2,K3) + zVertex(K1+1,K2,K3+1));

      xFace3(K1,K2,K3) = QUARTER*(
				  xVertex(K1  ,K2,K3) + xVertex(K1  ,K2+1,K3) +
				  xVertex(K1+1,K2,K3) + xVertex(K1+1,K2+1,K3));
      yFace3(K1,K2,K3) = QUARTER*(
				  yVertex(K1  ,K2,K3) + yVertex(K1  ,K2+1,K3) +
				  yVertex(K1+1,K2,K3) + yVertex(K1+1,K2+1,K3));
      zFace3(K1,K2,K3) = QUARTER*(
				  zVertex(K1  ,K2,K3) + zVertex(K1  ,K2+1,K3) +
				  zVertex(K1+1,K2,K3) + zVertex(K1+1,K2+1,K3));
            
    }
    


    switch ((exactFunctionType)ianswer)
    {
    case trigFunction:
      fType = trigFunction;

      x.link (xCell);
      y.link (yCell);

      s[grid]  (J1,J2,J3) = 1. + epsDensity*cos(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      
      Ds[grid] (J1,J2,J3,xAxis) = -epsDensity*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      Ds[grid] (J1,J2,J3,yAxis) = -epsDensity*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      
      DDs[grid](J1,J2,J3,xAxis,xAxis) = -epsDensity*twopi*twopi*cos(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      DDs[grid](J1,J2,J3,xAxis,yAxis) =  epsDensity*twopi*twopi*sin(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DDs[grid](J1,J2,J3,yAxis,xAxis) =  epsDensity*twopi*twopi*sin(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DDs[grid](J1,J2,J3,yAxis,yAxis) = -epsDensity*twopi*twopi*cos(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      
      uCell[grid]  (J1,J2,J3,xComponent) = sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      uCell[grid]  (J1,J2,J3,yComponent) = cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      
      DuCell[grid] (J1,J2,J3,xComponent,xAxis) =  twopi*cos(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      DuCell[grid] (J1,J2,J3,xComponent,yAxis) = -twopi*sin(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DuCell[grid] (J1,J2,J3,yComponent,xAxis) = -twopi*sin(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DuCell[grid] (J1,J2,J3,yComponent,yAxis) =  twopi*cos(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));

      DDuCell[grid](J1,J2,J3,xComponent,xAxis,xAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      DDuCell[grid](J1,J2,J3,xComponent,yAxis,xAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DDuCell[grid](J1,J2,J3,xComponent,xAxis,yAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DDuCell[grid](J1,J2,J3,xComponent,yAxis,yAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));

      DDuCell[grid](J1,J2,J3,yComponent,xAxis,xAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DDuCell[grid](J1,J2,J3,yComponent,yAxis,xAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      DDuCell[grid](J1,J2,J3,yComponent,xAxis,yAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      DDuCell[grid](J1,J2,J3,yComponent,yAxis,yAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));

      x.link (xFace1);
      y.link (yFace1);
      
      uFace1[grid]  (J1,J2,J3,xComponent) = sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      uFace1[grid]  (J1,J2,J3,yComponent) = cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      
      DuFace1[grid] (J1,J2,J3,xComponent,xAxis) =  twopi*cos(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      DuFace1[grid] (J1,J2,J3,xComponent,yAxis) = -twopi*sin(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DuFace1[grid] (J1,J2,J3,yComponent,xAxis) = -twopi*sin(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DuFace1[grid] (J1,J2,J3,yComponent,yAxis) =  twopi*cos(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));

      DDuFace1[grid](J1,J2,J3,xComponent,xAxis,xAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      DDuFace1[grid](J1,J2,J3,xComponent,yAxis,xAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DDuFace1[grid](J1,J2,J3,xComponent,xAxis,yAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DDuFace1[grid](J1,J2,J3,xComponent,yAxis,yAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));

      DDuFace1[grid](J1,J2,J3,yComponent,xAxis,xAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DDuFace1[grid](J1,J2,J3,yComponent,yAxis,xAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      DDuFace1[grid](J1,J2,J3,yComponent,xAxis,yAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      DDuFace1[grid](J1,J2,J3,yComponent,yAxis,yAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));

      x.link (xFace2);
      y.link (yFace2);
      
      uFace2[grid]  (J1,J2,J3,xComponent) = sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      uFace2[grid]  (J1,J2,J3,yComponent) = cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      
      DuFace2[grid] (J1,J2,J3,xComponent,xAxis) =  twopi*cos(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      DuFace2[grid] (J1,J2,J3,xComponent,yAxis) = -twopi*sin(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DuFace2[grid] (J1,J2,J3,yComponent,xAxis) = -twopi*sin(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DuFace2[grid] (J1,J2,J3,yComponent,yAxis) =  twopi*cos(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));

      DDuFace2[grid](J1,J2,J3,xComponent,xAxis,xAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      DDuFace2[grid](J1,J2,J3,xComponent,yAxis,xAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DDuFace2[grid](J1,J2,J3,xComponent,xAxis,yAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DDuFace2[grid](J1,J2,J3,xComponent,yAxis,yAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));

      DDuFace2[grid](J1,J2,J3,yComponent,xAxis,xAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));
      DDuFace2[grid](J1,J2,J3,yComponent,yAxis,xAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      DDuFace2[grid](J1,J2,J3,yComponent,xAxis,yAxis) = -twopi*twopi*sin(twopi*x(J1,J2,J3))*cos(twopi*y(J1,J2,J3));
      DDuFace2[grid](J1,J2,J3,yComponent,yAxis,yAxis) = -twopi*twopi*cos(twopi*x(J1,J2,J3))*sin(twopi*y(J1,J2,J3));

      break;
      
    case polyFunction:

      fType = polyFunction;

      x.link (xCell);
      y.link (yCell);
      
      s[grid]  (J1,J2,J3) = 1. + x(J1,J2,J3)*y(J1,J2,J3);
      
      Ds[grid] (J1,J2,J3,xAxis) = y(J1,J2,J3);
      Ds[grid] (J1,J2,J3,yAxis) = x(J1,J2,J3);
      
      DDs[grid](J1,J2,J3,xAxis,xAxis) = 0.;
      DDs[grid](J1,J2,J3,xAxis,yAxis) = 1.;
      DDs[grid](J1,J2,J3,yAxis,xAxis) = 1.;
      DDs[grid](J1,J2,J3,yAxis,yAxis) = 0.;
      
      uCell[grid]  (J1,J2,J3,xComponent) = pow(x(J1,J2,J3),3) * pow(y(J1,J2,J3),2);
      uCell[grid]  (J1,J2,J3,yComponent) = pow(x(J1,J2,J3),2) * pow(y(J1,J2,J3),3);

      DuCell[grid] (J1,J2,J3,xComponent,xAxis) = 3. * pow(x(J1,J2,J3),2) * pow(y(J1,J2,J3),2);
      DuCell[grid] (J1,J2,J3,xComponent,yAxis) = 2. * pow(x(J1,J2,J3),3) *     y(J1,J2,J3);
      DuCell[grid] (J1,J2,J3,yComponent,xAxis) = 2. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),3);
      DuCell[grid] (J1,J2,J3,yComponent,yAxis) = 3. * pow(x(J1,J2,J3),2) * pow(y(J1,J2,J3),2);
      
      DDuCell[grid](J1,J2,J3,xComponent,xAxis,xAxis) = 6. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),2);
      DDuCell[grid](J1,J2,J3,xComponent,yAxis,xAxis) = 6. * pow(x(J1,J2,J3),2) *     y(J1,J2,J3);
      DDuCell[grid](J1,J2,J3,xComponent,xAxis,yAxis) = 6. * pow(x(J1,J2,J3),2) *     y(J1,J2,J3);
      DDuCell[grid](J1,J2,J3,xComponent,yAxis,yAxis) = 2. * pow(x(J1,J2,J3),3);
      
      DDuCell[grid](J1,J2,J3,yComponent,xAxis,xAxis) = 2. *                      pow(y(J1,J2,J3),3);
      DDuCell[grid](J1,J2,J3,yComponent,yAxis,xAxis) = 6. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),2);
      DDuCell[grid](J1,J2,J3,yComponent,xAxis,yAxis) = 6. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),2);
      DDuCell[grid](J1,J2,J3,yComponent,yAxis,yAxis) = 6. * pow(x(J1,J2,J3),2) *     y(J1,J2,J3)   ;

      x.link (xFace1);
      y.link (yFace1);
      
      uFace1[grid]  (J1,J2,J3,xComponent) = pow(x(J1,J2,J3),3) * pow(y(J1,J2,J3),2);
      uFace1[grid]  (J1,J2,J3,yComponent) = pow(x(J1,J2,J3),2) * pow(y(J1,J2,J3),3);

      DuFace1[grid] (J1,J2,J3,xComponent,xAxis) = 3. * pow(x(J1,J2,J3),2) * pow(y(J1,J2,J3),2);
      DuFace1[grid] (J1,J2,J3,xComponent,yAxis) = 2. * pow(x(J1,J2,J3),3) *     y(J1,J2,J3);
      DuFace1[grid] (J1,J2,J3,yComponent,xAxis) = 2. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),3);
      DuFace1[grid] (J1,J2,J3,yComponent,yAxis) = 3. * pow(x(J1,J2,J3),2) * pow(y(J1,J2,J3),2);
      
      DDuFace1[grid](J1,J2,J3,xComponent,xAxis,xAxis) = 6. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),2);
      DDuFace1[grid](J1,J2,J3,xComponent,yAxis,xAxis) = 6. * pow(x(J1,J2,J3),2) *     y(J1,J2,J3);
      DDuFace1[grid](J1,J2,J3,xComponent,xAxis,yAxis) = 6. * pow(x(J1,J2,J3),2) *     y(J1,J2,J3);
      DDuFace1[grid](J1,J2,J3,xComponent,yAxis,yAxis) = 2. * pow(x(J1,J2,J3),3);
      
      DDuFace1[grid](J1,J2,J3,yComponent,xAxis,xAxis) = 2. *                      pow(y(J1,J2,J3),3);
      DDuFace1[grid](J1,J2,J3,yComponent,yAxis,xAxis) = 6. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),2);
      DDuFace1[grid](J1,J2,J3,yComponent,xAxis,yAxis) = 6. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),2);
      DDuFace1[grid](J1,J2,J3,yComponent,yAxis,yAxis) = 6. * pow(x(J1,J2,J3),2) *     y(J1,J2,J3)   ;

      x.link (xFace2);
      y.link (yFace2);
      
      uFace2[grid]  (J1,J2,J3,xComponent) = pow(x(J1,J2,J3),3) * pow(y(J1,J2,J3),2);
      uFace2[grid]  (J1,J2,J3,yComponent) = pow(x(J1,J2,J3),2) * pow(y(J1,J2,J3),3);

      DuFace2[grid] (J1,J2,J3,xComponent,xAxis) = 3. * pow(x(J1,J2,J3),2) * pow(y(J1,J2,J3),2);
      DuFace2[grid] (J1,J2,J3,xComponent,yAxis) = 2. * pow(x(J1,J2,J3),3) *     y(J1,J2,J3);
      DuFace2[grid] (J1,J2,J3,yComponent,xAxis) = 2. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),3);
      DuFace2[grid] (J1,J2,J3,yComponent,yAxis) = 3. * pow(x(J1,J2,J3),2) * pow(y(J1,J2,J3),2);
      
      DDuFace2[grid](J1,J2,J3,xComponent,xAxis,xAxis) = 6. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),2);
      DDuFace2[grid](J1,J2,J3,xComponent,yAxis,xAxis) = 6. * pow(x(J1,J2,J3),2) *     y(J1,J2,J3);
      DDuFace2[grid](J1,J2,J3,xComponent,xAxis,yAxis) = 6. * pow(x(J1,J2,J3),2) *     y(J1,J2,J3);
      DDuFace2[grid](J1,J2,J3,xComponent,yAxis,yAxis) = 2. * pow(x(J1,J2,J3),3);
      
      DDuFace2[grid](J1,J2,J3,yComponent,xAxis,xAxis) = 2. *                      pow(y(J1,J2,J3),3);
      DDuFace2[grid](J1,J2,J3,yComponent,yAxis,xAxis) = 6. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),2);
      DDuFace2[grid](J1,J2,J3,yComponent,xAxis,yAxis) = 6. *     x(J1,J2,J3)    * pow(y(J1,J2,J3),2);
      DDuFace2[grid](J1,J2,J3,yComponent,yAxis,yAxis) = 6. * pow(x(J1,J2,J3),2) *     y(J1,J2,J3)   ;

      break;

    case linearXFunction:
      
      fType = linearXFunction;
      x.link (xCell);
      y.link (yCell);
      
      s[grid]  (J1,J2,J3) = 1. + x(J1,J2,J3);
      
      Ds[grid] (J1,J2,J3,xAxis) = 1.;
      Ds[grid] (J1,J2,J3,yAxis) = 0.;
      
      DDs[grid] = 0.;
      
      uCell[grid]  (J1,J2,J3,xComponent) = x(J1,J2,J3);
      uCell[grid]  (J1,J2,J3,yComponent) = 0;
      
      DuCell[grid] (J1,J2,J3,xComponent,xAxis) = 1.;
      DuCell[grid] (J1,J2,J3,xComponent,yAxis) = 0.;
      DuCell[grid] (J1,J2,J3,yComponent,xAxis) = 0.;
      DuCell[grid] (J1,J2,J3,yComponent,yAxis) = 0.;
      
      DDuCell[grid] = 0.;

      x.link (xFace1);
      y.link (yFace1);
      
      uFace1[grid]  (J1,J2,J3,xComponent) = x(J1,J2,J3);
      uFace1[grid]  (J1,J2,J3,yComponent) = 0.;
      
      DuFace1[grid] (J1,J2,J3,xComponent,xAxis) = 1.;
      DuFace1[grid] (J1,J2,J3,xComponent,yAxis) = 0.;
      DuFace1[grid] (J1,J2,J3,yComponent,xAxis) = 0.;
      DuFace1[grid] (J1,J2,J3,yComponent,yAxis) = 0.;
      
      DDuFace1[grid] = 0.;

      x.link (xFace2);
      y.link (yFace2);
      
      uFace2[grid]  (J1,J2,J3,xComponent) = x(J1,J2,J3);
      uFace2[grid]  (J1,J2,J3,yComponent) = 0.;
      
      DuFace2[grid] (J1,J2,J3,xComponent,xAxis) = 1.;
      DuFace2[grid] (J1,J2,J3,xComponent,yAxis) = 0.;
      DuFace2[grid] (J1,J2,J3,yComponent,xAxis) = 0.;
      DuFace2[grid] (J1,J2,J3,yComponent,yAxis) = 0.;
      
      DDuFace2[grid] = 0.;

      
      break;

    case linearYFunction:
      
      fType = linearYFunction;
      x.link (xCell);
      y.link (yCell);
      
      s[grid]  (J1,J2,J3) = 1. + y(J1,J2,J3);
      
      Ds[grid] (J1,J2,J3,xAxis) = 0.;
      Ds[grid] (J1,J2,J3,yAxis) = 1.;
      
      DDs[grid] = 0.;
      
//      uCell[grid]  (J1,J2,J3,xComponent) = 0.;
//      uCell[grid]  (J1,J2,J3,yComponent) = y(J1,J2,J3);

      uCell[grid]  (J1,J2,J3,xComponent) = pow(x(J1,J2,J3),3);
      uCell[grid]  (J1,J2,J3,yComponent) = pow(x(J1,J2,J3),2);
      
      DuCell[grid] (J1,J2,J3,xComponent,xAxis) = 3.*pow(x(J1,J2,J3),2);
      DuCell[grid] (J1,J2,J3,xComponent,yAxis) = 0.;
      DuCell[grid] (J1,J2,J3,yComponent,xAxis) = 2.*x(J1,J2,J3);
      DuCell[grid] (J1,J2,J3,yComponent,yAxis) = 0.;
      
      DDuCell[grid](J1,J2,J3,xComponent,xAxis) = 6.*x(J1,J2,J3);
      DDuCell[grid](J1,J2,J3,xComponent,yAxis) = 0.;
      DDuCell[grid](J1,J2,J3,yComponent,xAxis) = 2.;
      DDuCell[grid](J1,J2,J3,yComponent,yAxis) = 0.;

      x.link (xFace1);
      y.link (yFace1);
      
      uFace1[grid]  (J1,J2,J3,xComponent) = pow(x(J1,J2,J3),3);
      uFace1[grid]  (J1,J2,J3,yComponent) = pow(x(J1,J2,J3),2);
      
      DuFace1[grid] (J1,J2,J3,xComponent,xAxis) = 3.*pow(x(J1,J2,J3),2);
      DuFace1[grid] (J1,J2,J3,xComponent,yAxis) = 0.;
      DuFace1[grid] (J1,J2,J3,yComponent,xAxis) = 2.*x(J1,J2,J3);
      DuFace1[grid] (J1,J2,J3,yComponent,yAxis) = 0.;
      
      DDuFace1[grid](J1,J2,J3,xComponent,xAxis) = 6.*x(J1,J2,J3);
      DDuFace1[grid](J1,J2,J3,xComponent,yAxis) = 0.;
      DDuFace1[grid](J1,J2,J3,yComponent,xAxis) = 2.;
      DDuFace1[grid](J1,J2,J3,yComponent,yAxis) = 0.;

      x.link (xFace2);
      y.link (yFace2);
      
      uFace2[grid]  (J1,J2,J3,xComponent) = pow(x(J1,J2,J3),3);
      uFace2[grid]  (J1,J2,J3,yComponent) = pow(x(J1,J2,J3),2);

      DuFace2[grid] (J1,J2,J3,xComponent,xAxis) = 3.*pow(x(J1,J2,J3),2);
      DuFace2[grid] (J1,J2,J3,xComponent,yAxis) = 0.;
      DuFace2[grid] (J1,J2,J3,yComponent,xAxis) = 2.*x(J1,J2,J3);
      DuFace2[grid] (J1,J2,J3,yComponent,yAxis) = 0.;
      
      DDuFace2[grid](J1,J2,J3,xComponent,xAxis) = 6.*x(J1,J2,J3);
      DDuFace2[grid](J1,J2,J3,xComponent,yAxis) = 0.;
      DDuFace2[grid](J1,J2,J3,yComponent,xAxis) = 2.;
      DDuFace2[grid](J1,J2,J3,yComponent,yAxis) = 0.;
      
      break;

    case quadraticFunction:
      
      fType = quadraticFunction;

      x.link (xCell);
      y.link (yCell);
      
      s[grid]  (J1,J2,J3) = 1. + x(J1,J2,J3)*y(J1,J2,J3);
      
      Ds[grid] (J1,J2,J3,xAxis) = y(J1,J2,J3);
      Ds[grid] (J1,J2,J3,yAxis) = x(J1,J2,J3);
      
      DDs[grid](J1,J2,J3,xAxis,xAxis) = 0.;
      DDs[grid](J1,J2,J3,xAxis,yAxis) = 1.;
      DDs[grid](J1,J2,J3,yAxis,xAxis) = 1.;
      DDs[grid](J1,J2,J3,yAxis,yAxis) = 0.;
      
      uCell[grid]  (J1,J2,J3,xComponent) = pow(x(J1,J2,J3),2);
      uCell[grid]  (J1,J2,J3,yComponent) = pow(y(J1,J2,J3),2);
      
      DuCell[grid] (J1,J2,J3,xComponent,xAxis) = 2.*x(J1,J2,J3);
      DuCell[grid] (J1,J2,J3,xComponent,yAxis) = 0.;
      DuCell[grid] (J1,J2,J3,yComponent,xAxis) = 0.;
      DuCell[grid] (J1,J2,J3,yComponent,yAxis) = 2.*y(J1,J2,J3);
      
      DDuCell[grid] = 0.;
      DDuCell[grid](J1,J2,J3,xComponent,xAxis,xAxis) = 2.;
      DDuCell[grid](J1,J2,J3,yComponent,yAxis,yAxis) = 2.;

      x.link (xFace1);
      y.link (yFace1);
      
      uFace1[grid]  (J1,J2,J3,xComponent) = pow(x(J1,J2,J3),2);
      uFace1[grid]  (J1,J2,J3,yComponent) = pow(y(J1,J2,J3),2);
      
      DuFace1[grid] (J1,J2,J3,xComponent,xAxis) = 2.*x(J1,J2,J3);
      DuFace1[grid] (J1,J2,J3,xComponent,yAxis) = 0.;
      DuFace1[grid] (J1,J2,J3,yComponent,xAxis) = 0.;
      DuFace1[grid] (J1,J2,J3,yComponent,yAxis) = 2.*y(J1,J2,J3);
      
      DDuFace1[grid] = 0.;
      DDuFace1[grid](J1,J2,J3,xComponent,xAxis,xAxis) = 2.;
      DDuFace1[grid](J1,J2,J3,yComponent,yAxis,yAxis) = 2.;

      x.link (xFace2);
      y.link (yFace2);
      
      uFace2[grid]  (J1,J2,J3,xComponent) = pow(x(J1,J2,J3),2);
      uFace2[grid]  (J1,J2,J3,yComponent) = pow(y(J1,J2,J3),2);
      
      DuFace2[grid] (J1,J2,J3,xComponent,xAxis) = 2.*x(J1,J2,J3);
      DuFace2[grid] (J1,J2,J3,xComponent,yAxis) = 0.;
      DuFace2[grid] (J1,J2,J3,yComponent,xAxis) = 0.;
      DuFace2[grid] (J1,J2,J3,yComponent,yAxis) = 2.*y(J1,J2,J3);
      
      DDuFace2[grid] = 0.;
      DDuFace2[grid](J1,J2,J3,xComponent,xAxis,xAxis) = 2.;
      DDuFace2[grid](J1,J2,J3,yComponent,yAxis,yAxis) = 2.;

    
      break;
      
    default:
      cout << "Illegal value input: " << ianswer;
      exit (-1);
    }
  }
  
   ForAllGrids (grid) display.display (gc[grid].centerNormal(), "Here is centerNormal");
   display.display (cN, "Here is  centerNormal computed from faceNormals");
     
   display.display (uCell, "Here is uCell");
   display.display (DuCell, "Here is DuCell");
   display.display (DDuCell, "Here is DDuCell");
   
   display.display (uFace1, "Here is uFace1");
   display.display (DuFace1, "Here is DuFace1");
   display.display (DDuFace1, "Here is DDuFace1");
   
   display.display (uFace2, "Here is uFace2");
   display.display (DuFace2, "Here is DuFace2");
   display.display (DDuFace2, "Here is DDuFace2");

   display.display (xFace1, "Here is xFace1");
   display.display (xFace2, "Here is xFace2");
   display.display (yFace1, "Here is yFace1");
   display.display (yFace2, "Here is yFace2");
     
  
//========================================  
//...Test CompositeGridFiniteVolumeOperators
//========================================

  realCompositeGridFunction computed;
  realCompositeGridFunction exact;
   realCompositeGridFunction error;

  Index NC;
  NC = Range (0,nD-1);
  int scalarIndex = 1;


  int test;
  enum testTypes
  {
    operatorTest,
    gridFunctionTest,
    numberOfTests
    };

  int nTest = 2; // change to numberOfTests when Bill fixes the operator class to recognize these

   for (test=1; test<nTest; test++)  //only test the gridFunction form of the operators
  {
    

    //========================================
    cout << endl << "Testing x-derivative of cellCentered gridFunction..." << endl;
    //========================================

    // ... default case
    cout << "... default case" << endl;
    

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.x (uCell);
      break;
    case gridFunctionTest:
      computed = uCell.x ();
      break;
    default:
      break;
    }
    
    computed.periodicUpdate();
    
    display.display (computed, "Computed u.x (default)");

    exact.updateToMatchGrid(gc, cellCentered, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuCell[grid](J1,J2,J3,component,xAxis);
      }
    }

    display.display (exact, "Exact u.x");
    error = computed - exact;
    display.display (error, "Error");

    cout << "     ";
    printMaxNormOfDifference (computed, exact);

    // ... cellCentered output specified

    cout << "    ... cellCentered output specified" << endl;
    
    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.x (uCell, cellCentered);
      break;
    case gridFunctionTest:
      computed = uCell.x (cellCentered);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, cellCentered, nD);
    int component;
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuCell[grid](J1,J2,J3,component,xAxis);
      }
    }
    computed.periodicUpdate();

    display.display (computed, "Computed u.x (cellCentered -> cellCentered)");
    display.display (exact, "Exact u.x");

    error = computed - exact;
    display.display (error, "Error");
    

    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    

    // ... faceCenteredAxis1 output specified 

    cout << "... faceCenteredAxis1 output specified" << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.x (uCell, faceCenteredAxis1);
      break;
    case gridFunctionTest:
      computed = uCell.x (faceCenteredAxis1);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, faceCenteredAxis1, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace1[grid](J1,J2,J3,component,xAxis);
      }
    }
    computed.periodicUpdate();

    display.display (computed, "Computed u.x (cellCentered -> faceCenteredAxis1)");
    display.display (exact, "Exact u.x");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    
    // ... faceCenteredAxis2 output specified 

    cout << "... faceCenteredAxis2 output specified " << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.x (uCell, faceCenteredAxis2);
      break;
    case gridFunctionTest:
      computed = uCell.x (faceCenteredAxis2);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, faceCenteredAxis2, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace2[grid](J1,J2,J3,component,xAxis);
      }
    }
    computed.periodicUpdate();

    display.display (computed, "Computed u.x (cellCentered -> faceCenteredAxis2)");
    display.display (exact, "Exact u.x");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    


    //========================================
    cout << endl << "Testing x-derivative of faceCenteredAxis1 gridFunction..." << endl;
    //========================================

    // ... default case

    cout << "... default case" << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.x (uFace1);
      break;
    case gridFunctionTest:
      computed = uFace1.x ();
      break;
    default:
      break;
    }
    

    exact.updateToMatchGrid(gc, cellCentered, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace1[grid](J1,J2,J3,component,xAxis);
      }
    }
    computed.periodicUpdate();

    display.display (computed, "Computed u.x (faceCenteredAxis1 default");
    display.display (exact, "Exact u.x");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);

    // ... cellCentered output specified

    cout << "... cellCentered output specified" << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.x (uFace1, cellCentered);
      break;
    case gridFunctionTest:
      computed = uFace1.x (cellCentered);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, cellCentered, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuCell[grid](J1,J2,J3,component,xAxis);
      }
    }
    computed.periodicUpdate();

    display.display (computed, "Computed u.x (faceCenteredAxis1 -> cellCentered)");
    display.display (exact, "Exact u.x");
    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    

    // ... faceCenteredAxis1 output specified 
    
    cout << "... faceCenteredAxis1 output specified " << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.x (uFace1, faceCenteredAxis1);
      break;
    case gridFunctionTest:
      computed = uFace1.x (faceCenteredAxis1);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, faceCenteredAxis1, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace1[grid](J1,J2,J3,component,xAxis);
      }
    }

    computed.periodicUpdate ();
    
    display.display (computed, "Computed u.x (faceCenteredAxis1 -> faceCenteredAxis1)");
    display.display (exact, "Exact u.x");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    
    // ... faceCenteredAxis2 output specified 

    cout << "... faceCenteredAxis2 output specified " << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.x (uFace1, faceCenteredAxis2);
      break;
    case gridFunctionTest:
      computed = uFace1.x (faceCenteredAxis2);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, faceCenteredAxis2, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace2[grid](J1,J2,J3,component,xAxis);
      }
    }
    computed.periodicUpdate ();

    display.display (computed, "Computed u.x (faceCenteredAxis1 -> faceCenteredAxis2)");
    display.display (exact, "Exact u.x");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    
  
    //========================================
    cout << endl << "Testing x-derivative of faceCenteredAxis2 gridFunction..." << endl;
    //========================================

    // ... default case
    
    cout << "... default case" << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.x (uFace2);
      break;
    case gridFunctionTest:
      computed = uFace2.x ();
      break;
    default:
      break;
    }
    

    exact.updateToMatchGrid(gc, cellCentered, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace2[grid](J1,J2,J3,component,xAxis);
      }
    }
    computed.periodicUpdate ();

    display.display (computed, "Computed u.x (faceCenteredAxis2 default)");
    display.display (exact, "Exact u.x");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);

    // ... cellCentered output specified

    cout << "... cellCentered output specified" << endl;
    
    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.x (uFace2, cellCentered);
      break;
    case gridFunctionTest:
      computed = uFace2.x (cellCentered);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, cellCentered, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuCell[grid](J1,J2,J3,component,xAxis);
      }
    }
    computed.periodicUpdate ();

    display.display (computed, "Computed u.x (faceCenteredAxis2 -> cellCentered)");
    display.display (exact, "Exact u.x");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    

    // ... faceCenteredAxis1 output specified 

    cout << "... faceCenteredAxis1 output specified " << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.x (uFace2, faceCenteredAxis1);
      break;
    case gridFunctionTest:
      computed = uFace2.x (faceCenteredAxis1);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, faceCenteredAxis1, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace1[grid](J1,J2,J3,component,xAxis);
      }
    }
    computed.periodicUpdate ();

    display.display (computed, "Computed u.x (faceCenteredAxis2 -> faceCenteredAxis1)");
    display.display (exact, "Exact u.x");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    
    // ... faceCenteredAxis2 output specified 

    cout << "... faceCenteredAxis2 output specified " << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.x (uFace2, faceCenteredAxis2);
      break;
    case gridFunctionTest:
      computed = uFace2.x (faceCenteredAxis2);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, faceCenteredAxis2, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace2[grid](J1,J2,J3,component,xAxis);
      }
    }
    computed.periodicUpdate ();

    display.display (computed, "Computed u.x (faceCenteredAxis2 -> faceCenteredAxis2)");
    display.display (exact, "Exact u.x");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    

    //========================================
    cout << endl << "Testing y-derivative of cellCentered gridFunction..." << endl;
    //========================================

    // ... default case
    cout << "... default case" << endl;
    

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.y (uCell);
      break;
    case gridFunctionTest:
      computed = uCell.y ();
      break;
    default:
      break;
    }
    
    computed.periodicUpdate();
    
    display.display (computed, "Computed u.y (default)");

    exact.updateToMatchGrid(gc, cellCentered, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuCell[grid](J1,J2,J3,component,yAxis);
      }
    }

    display.display (exact, "Exact u.y");
    error = computed - exact;
    display.display (error, "Error");

    cout << "     ";
    printMaxNormOfDifference (computed, exact);

    // ... cellCentered output specified

    cout << "    ... cellCentered output specified" << endl;
    
    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.y (uCell, cellCentered);
      break;
    case gridFunctionTest:
      computed = uCell.y (cellCentered);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, cellCentered, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuCell[grid](J1,J2,J3,component,yAxis);
      }
    }
    computed.periodicUpdate();

    display.display (computed, "Computed u.y (cellCentered -> cellCentered)");
    display.display (exact, "Exact u.y");

    error = computed - exact;
    display.display (error, "Error");
    

    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    

    // ... faceCenteredAxis1 output specified 

    cout << "... faceCenteredAxis1 output specified" << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.y (uCell, faceCenteredAxis1);
      break;
    case gridFunctionTest:
      computed = uCell.y (faceCenteredAxis1);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, faceCenteredAxis1, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace1[grid](J1,J2,J3,component,yAxis);
      }
    }
    computed.periodicUpdate();

    display.display (computed, "Computed u.y (cellCentered -> faceCenteredAxis1)");
    display.display (exact, "Exact u.y");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    
    // ... faceCenteredAxis2 output specified 

    cout << "... faceCenteredAxis2 output specified " << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.y (uCell, faceCenteredAxis2);
      break;
    case gridFunctionTest:
      computed = uCell.y (faceCenteredAxis2);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, faceCenteredAxis2, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace2[grid](J1,J2,J3,component,yAxis);
      }
    }
    computed.periodicUpdate();

    display.display (computed, "Computed u.y (cellCentered -> faceCenteredAxis2)");
    display.display (exact, "Exact u.y");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    


    //========================================
    cout << endl << "Testing y-derivative of faceCenteredAxis1 gridFunction..." << endl;
    //========================================

    // ... default case

    cout << "... default case" << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.y (uFace1);
      break;
    case gridFunctionTest:
      computed = uFace1.y ();
      break;
    default:
      break;
    }
    

    exact.updateToMatchGrid(gc, cellCentered, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace1[grid](J1,J2,J3,component,yAxis);
      }
    }
    computed.periodicUpdate();

    display.display (computed, "Computed u.y (faceCenteredAxis1 default");
    display.display (exact, "Exact u.y");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);

    // ... cellCentered output specified

    cout << "... cellCentered output specified" << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.y (uFace1, cellCentered);
      break;
    case gridFunctionTest:
      computed = uFace1.y (cellCentered);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, cellCentered, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuCell[grid](J1,J2,J3,component,yAxis);
      }
    }
    computed.periodicUpdate();

    display.display (computed, "Computed u.y (faceCenteredAxis1 -> cellCentered)");
    display.display (exact, "Exact u.y");
    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    

    // ... faceCenteredAxis1 output specified 
    
    cout << "... faceCenteredAxis1 output specified " << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.y (uFace1, faceCenteredAxis1);
      break;
    case gridFunctionTest:
      computed = uFace1.y (faceCenteredAxis1);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, faceCenteredAxis1, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace1[grid](J1,J2,J3,component,yAxis);
      }
    }

    computed.periodicUpdate ();
    
    display.display (computed, "Computed u.y (faceCenteredAxis1 -> faceCenteredAxis1)");
    display.display (exact, "Exact u.y");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    
    // ... faceCenteredAxis2 output specified 

    cout << "... faceCenteredAxis2 output specified " << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.y (uFace1, faceCenteredAxis2);
      break;
    case gridFunctionTest:
      computed = uFace1.y (faceCenteredAxis2);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, faceCenteredAxis2, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace2[grid](J1,J2,J3,component,yAxis);
      }
    }
    computed.periodicUpdate ();

    display.display (computed, "Computed u.y (faceCenteredAxis1 -> faceCenteredAxis2)");
    display.display (exact, "Exact u.y");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    
  
    //========================================
    cout << endl << "Testing y-derivative of faceCenteredAxis2 gridFunction..." << endl;
    //========================================

    // ... default case
    
    cout << "... default case" << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.y (uFace2);
      break;
    case gridFunctionTest:
      computed = uFace2.y ();
      break;
    default:
      break;
    }
    

    exact.updateToMatchGrid(gc, cellCentered, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace2[grid](J1,J2,J3,component,yAxis);
      }
    }
    computed.periodicUpdate ();

    display.display (computed, "Computed u.y (faceCenteredAxis2 default)");
    display.display (exact, "Exact u.y");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);

    // ... cellCentered output specified

    cout << "... cellCentered output specified" << endl;
    
    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.y (uFace2, cellCentered);
      break;
    case gridFunctionTest:
      computed = uFace2.y (cellCentered);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, cellCentered, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuCell[grid](J1,J2,J3,component,yAxis);
      }
    }
    computed.periodicUpdate ();

    display.display (computed, "Computed u.y (faceCenteredAxis2 -> cellCentered)");
    display.display (exact, "Exact u.y");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    

    // ... faceCenteredAxis1 output specified 

    cout << "... faceCenteredAxis1 output specified " << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.y (uFace2, faceCenteredAxis1);
      break;
    case gridFunctionTest:
      computed = uFace2.y (faceCenteredAxis1);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, faceCenteredAxis1, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace1[grid](J1,J2,J3,component,yAxis);
      }
    }
    computed.periodicUpdate ();

    display.display (computed, "Computed u.y (faceCenteredAxis2 -> faceCenteredAxis1)");
    display.display (exact, "Exact u.y");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    
    // ... faceCenteredAxis2 output specified 

    cout << "... faceCenteredAxis2 output specified " << endl;

    switch ((testTypes)test)
    {
    case operatorTest:
      computed = operators.y (uFace2, faceCenteredAxis2);
      break;
    case gridFunctionTest:
      computed = uFace2.y (faceCenteredAxis2);
      break;
    default:
      break;
    }

    exact.updateToMatchGrid(gc, faceCenteredAxis2, nD);
    ForAllGrids(grid)
    {
      MappedGrid & mg = gc[grid];
      getIndex(mg.dimension(), J1, J2, J3);
      for (component=0; component<nD; component++)
      {
	exact[grid](J1,J2,J3,component) =  DuFace2[grid](J1,J2,J3,component,yAxis);
      }
    }
    computed.periodicUpdate ();

    display.display (computed, "Computed u.y (faceCenteredAxis2 -> faceCenteredAxis2)");
    display.display (exact, "Exact u.y");

    error = computed - exact;
    display.display (error, "Error");


    cout << "     ";
    printMaxNormOfDifference (computed, exact);
    
  
    
  }
}





