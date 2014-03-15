#include "MappedGridFiniteVolumeOperators.h"
// ==============================================================================
realMappedGridFunction MappedGridFiniteVolumeOperators::
x (const realMappedGridFunction & u,
   const GridFunctionParameters & gfParams,
   const Index & I1,
   const Index & I2,
   const Index & I3,
   const Index & Components,
   const Index & I5,
   const Index & I6,
   const Index & I7,
   const Index & I8)
     
//
// Description:
//   returns x derivative; returned GridFunctionType is specified by the user
//   in derivativeGridFunctionType
//
// ==============================================================================
{

  if (gfParams.inputType != GridFunctionParameters::defaultCentering &&
      gfParams.inputType != u.getGridFunctionType()) 
  {
    cout << "MappedGridFiniteVolumeOperators::x: ERROR: attempt to specify gfParams::inputType " << endl;
    throw "x: ERROR";
  }
  
  return (typedDerivative (u, xAxis, gfParams, I1, I2, I3, Components));
}

// ==============================================================================
realMappedGridFunction MappedGridFiniteVolumeOperators::
x (const realMappedGridFunction & u,
   const Index & I1,
   const Index & I2,
   const Index & I3,
   const Index & Components,
   const Index & I5,
   const Index & I6,
   const Index & I7,
   const Index & I8)
     
//
// Description:
//   returns x derivative; returned GridFunctionType is specified by the user
//   in derivativeGridFunctionType
//
// ==============================================================================
{
  return (untypedDerivative (u, xAxis, I1, I2, I3, Components));
}

// ==============================================================================
realMappedGridFunction MappedGridFiniteVolumeOperators::
y (const realMappedGridFunction & u,
   const GridFunctionParameters & gfParams,

   const Index & I1,
   const Index & I2,
   const Index & I3,
   const Index & Components,
   const Index & I5,
   const Index & I6,
   const Index & I7,
   const Index & I8)
     
//
// Description:
//   returns x derivative; returned GridFunctionType is specified by the user
//   in derivativeGridFunctionType
//
// ==============================================================================
{
    if (gfParams.inputType != GridFunctionParameters::defaultCentering &&
      gfParams.inputType != u.getGridFunctionType()) 

  {
    cout << "MappedGridFiniteVolumeOperators::y: ERROR: attempt to specify gfParams::inputType " << endl;
    throw "y: ERROR";
  }

  return (typedDerivative (u, yAxis, gfParams, I1, I2, I3, Components));
}
// ==============================================================================
realMappedGridFunction MappedGridFiniteVolumeOperators::
y (const realMappedGridFunction & u,
   const Index & I1,
   const Index & I2,
   const Index & I3,
   const Index & Components,

   const Index & I5,
   const Index & I6,
   const Index & I7,
   const Index & I8)
     
//
// Description:
//   returns x derivative; returned GridFunctionType is specified by the user
//   in derivativeGridFunctionType
//
// ==============================================================================
{
  return (untypedDerivative (u, yAxis, I1, I2, I3, Components));
}

// ==============================================================================
realMappedGridFunction MappedGridFiniteVolumeOperators::
z (const realMappedGridFunction & u,
   const GridFunctionParameters & gfParams,
   const Index & I1,
   const Index & I2,
   const Index & I3,
   const Index & Components,
   const Index & I5,
   const Index & I6,
   const Index & I7,
   const Index & I8)
     
//
// Description:
//   returns x derivative; returned GridFunctionType is specified by the user
//   in derivativeGridFunctionType
//
// ==============================================================================
{
  if (gfParams.inputType != GridFunctionParameters::defaultCentering &&
      gfParams.inputType != u.getGridFunctionType()) 

  {
    cout << "MappedGridFiniteVolumeOperators::z: ERROR: attempt to specify gfParams::inputType " << endl;
    throw "z: ERROR";
  }

  return (typedDerivative (u, zAxis, gfParams, I1, I2, I3, Components));
}

// ==============================================================================
realMappedGridFunction MappedGridFiniteVolumeOperators::
z (const realMappedGridFunction & u,
   const Index & I1,
   const Index & I2,
   const Index & I3,
   const Index & Components,
   const Index & I5,
   const Index & I6,
   const Index & I7,
   const Index & I8)
     
//
// Description:
//   returns x derivative; returned GridFunctionType is specified by the user
//   in derivativeGridFunctionType
//
// ==============================================================================
{
  return (untypedDerivative (u, zAxis, I1, I2, I3, Components));
}

  

// ==============================================================================
realMappedGridFunction MappedGridFiniteVolumeOperators::
typedDerivative (const realMappedGridFunction & u,
		 const int & direction,
		 const GridFunctionParameters & gfParams,
		 const Index & I1,
		 const Index & I2,
		 const Index & I3,
		 const Index & Components)

     
//
// Description:
//   returns derivative in a particular direction; returned GridFunctionType is specified by the user
//   in derivativeGridFunctionType
//
// ==============================================================================
{
  // ... declare and allocate space for returnedValue; same size as u but
  // ... with GridFunctionType as specified in input parameter
  //
  GridFunctionParameters::GridFunctionType derivativeGridFunctionType = (GridFunctionParameters::GridFunctionType &) (gfParams.outputType); 

  realMappedGridFunction returnedValue;
  Index inputGFC = Range (u.getComponentBase(0), u.getComponentBound(0)); // input grid function components
//returnedValue.updateToMatchGridFunction (u);
  returnedValue.updateToMatchGrid (mappedGrid, derivativeGridFunctionType, inputGFC);  
  returnedValue = 0.;
  
  // ... call driver function to evaluate the derivative and return
  
  firstDerivatives (returnedValue, direction, derivativeGridFunctionType, u, I1, I2, I3, Components);
  return (returnedValue);
  
}

// ==============================================================================
realMappedGridFunction MappedGridFiniteVolumeOperators::
untypedDerivative (const realMappedGridFunction & u,
		   const int & direction,
		   const Index & I1,
		   const Index & I2,
		   const Index & I3,
		   const Index & Components)
//
// Description:
//   returns  derivative in a particular direction; returned GridFunctionType is the same as the input
//   GridFunctionType
// ==============================================================================
{
  // ... For convenience, define local copies of GridFunctionType's

  const GridFunctionParameters::GridFunctionType general           = GridFunctionParameters::general; 
  const GridFunctionParameters::GridFunctionType vertexCentered    = GridFunctionParameters::vertexCentered; 
  const GridFunctionParameters::GridFunctionType cellCentered      = GridFunctionParameters::cellCentered; 
  const GridFunctionParameters::GridFunctionType faceCenteredAll   = GridFunctionParameters::faceCenteredAll; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis1 = GridFunctionParameters::faceCenteredAxis1; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis2 = GridFunctionParameters::faceCenteredAxis2; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis3 = GridFunctionParameters::faceCenteredAxis3; 

  // get the inputGridFunction type to use as derivativeGridFunctionType *** this may not work ?
  GridFunctionParameters::GridFunctionType derivativeGridFunctionType = Components.length()==0 ?
    u.getGridFunctionType() : u.getGridFunctionType(Components);

  Index inputGFC = Range (u.getComponentBase(0), u.getComponentBound(0)); // input grid function components
  Index outputGFC;

  // ... set C to actual output components 
  Index C = Components.length()==0 ? inputGFC : Components;               

  int c;
  
  GridFunctionParameters::GridFunctionType componentDerivativeGFType; 

  realMappedGridFunction returnedValue;

  // ... returnedValue to have identical GridFunctionType and size to input function u
  returnedValue.updateToMatchGridFunction (u);
  returnedValue = 0.;
      
  switch (derivativeGridFunctionType)
  {
  case vertexCentered:
    cout << "MappedGridFunctionFiniteVolumeOperators::x: vertexCentered differentiation not implemented in this class" << endl;
    throw "MappedGridFiniteVolumeOperators::x: ERROR";

    
    //... each component must be treated separately for these cases

  case general:
  case faceCenteredAll:

    for (c=C.getBase(); c<=C.getBound(); c++)
    {
      // ... determine output centering (same as input centering for this component)
      outputGFC = Range (c,c);
      componentDerivativeGFType = u.getGridFunctionType(c);

      // ... compute derivative for this component and write into returnedValue
      firstDerivatives (returnedValue, direction, componentDerivativeGFType, u, I1, I2, I3, outputGFC);
    }
    
    break;

      //... for simple centering cases, call x (...,GridFunctionType& ,...)
    
  case cellCentered:
  case faceCenteredAxis1:
  case faceCenteredAxis2:
  case faceCenteredAxis3:
    
//    returnedValue = x(u, derivativeGridFunctionType, I1, I2, I3, Components);
    firstDerivatives (returnedValue, direction, derivativeGridFunctionType, u, I1, I2, I3, Components);
    
    break;

  default:
    cout << "MappedGridFunctionFiniteVolumeOperators::x: unknown input GridFunctionType " << endl;
    throw "MappedGridFiniteVolumeOperators::x: ERROR";

  }
    
  return (returnedValue);
  
}
 
// ==============================================================================
//\begin{>derivatives.tex}{\subsection{derivatives}} 
void MappedGridFiniteVolumeOperators::
firstDerivatives (realMappedGridFunction & returnedValue,
		  const int & direction,                       //derivative in this direction
		  const GridFunctionParameters & gfParams,
		  const realMappedGridFunction &u,
		  const Index & I1,  //=nullIndex
		  const Index & I2,  //=nullIndex
		  const Index & I3,  //=nullIndex
		  const Index & Components)  //=nullIndex
//
// /Purpose: compute 1st derivatives
//    The returned gridFunction has the same size as u, but derivatives
//    are only evaluated for the subset of components specified by the Components
//    argument. 
//    only the components specified by Components argument are written over; the
//    others are left untouched.
//    This routine is designed to work for  gridFunctions with components 
//    in standard ordering (I1,I2,I3,C).
//
//==============================================================================
{

  //... test only
/*
  int input;
  cout << "???";
  cin >> input;
*/

  GridFunctionParameters::GridFunctionType derivativeGridFunctionType = (GridFunctionParameters::GridFunctionType&) (gfParams.outputType); 

  realMappedGridFunction temp;
  temp.updateToMatchGridFunction (returnedValue);
  
  real QUARTER = 0.25, HALF = 0.5;
  
  Index J1, J2, J3, C;
  Index K1, K2, K3;

  int ia;

  Index inputGFC = Range (u.getComponentBase(0), u.getComponentBound(0)); // input grid function components
  Index inputRVC = Range (returnedValue.getComponentBase(0), returnedValue.getComponentBound(0)); // returnedValue components
  
  if (inputGFC.getBase() != inputRVC.getBase() || inputGFC.getBound() != inputRVC.getBound())
  {
    cout << "MappedGridFiniteVolumeOperators::firstDervatives: ERROR: you must provide a returnedValue" << endl;
    cout << "  GridFunction of the same size as the input GridFunction u " << endl;
    throw "MappedGridFiniteVolumeOperators::firstDervatives: ERROR";
}

  // ... For convenience, define local copies of GridFunctionType's

//  const GridFunctionParameters::GridFunctionType defaultCentering  = GridFunctionParameters::defaultCentering; 
//  const GridFunctionParameters::GridFunctionType general           = GridFunctionParameters::general; 
//  const GridFunctionParameters::GridFunctionType vertexCentered    = GridFunctionParameters::vertexCentered; 
  const GridFunctionParameters::GridFunctionType cellCentered      = GridFunctionParameters::cellCentered; 
//  const GridFunctionParameters::GridFunctionType faceCenteredAll   = GridFunctionParameters::faceCenteredAll; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis1 = GridFunctionParameters::faceCenteredAxis1; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis2 = GridFunctionParameters::faceCenteredAxis2; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis3 = GridFunctionParameters::faceCenteredAxis3; 

  // ... for convenience save output face direction as an integer also
  int outputAxis = -1, inputAxis = -1;
  
  switch (derivativeGridFunctionType)
  {
  case faceCenteredAxis1:
    outputAxis=0;
    break;
  case faceCenteredAxis2:
    outputAxis=1;
    break;
  case faceCenteredAxis3:
    outputAxis=2;
    break;
  default: // centered or general
    outputAxis=-1;
    break;
  }


  int oa = outputAxis;

  // ... figure out for what Components to compute the derivatives basd on input value of Components
  C = Components.length()==0 ? inputGFC : Components;

//  int d = direction;  // ... derivative direction
  int axis;           // ... used for differencing direction
  int c;              // ... used for component number

//  ... define these Macros for readability below:
//
//      D0u(axis,...,c)  is centered undivided difference in the axis direction of u(,,,c)
#define D0u(axis,J1,J2,J3,c) HALF*(u(J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),c) -  u(J1-inc(axis,0),J2-inc(axis,1),J3-inc(axis,2),c))

//      Dmu(axis,...,c) is backward undivided difference in the axis direction of u(,,,c)
#define Dmu(axis,J1,J2,J3,c) (u(J1,J2,J3,c) - u(J1-inc(axis,0),J2-inc(axis,1),J3-inc(axis,2),c))

//      Dpu(axis,...,c) is forward undivided difference in the axis direction of u(,,,c)
#define Dpu(axis,J1,J2,J3,c) (u(J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),c) - u(J1,J2,J3,c))

//      rxDmu is (dr/dx)*Dmu (faceCentered)  (cellCentered -> faceCentered)
#define rxDmu(x,axis,J1,J2,J3,c) faceNormal(J1,J2,J3,x,axis)*Dmu(axis,J1,J2,J3,c)

//      rxDpu is (dr/dx)*Dpu (cellCentered)  (faceCentered -> cellCentered)
//#define rxDpu(x,axis,J1,J2,J3,c) centerNormal(J1,J2,J3,x,axis)*Dpu(axis,J1,J2,J3,c)
//
// *** workaround centerNormal bug ***
#define rxDpu(x,axis,J1,J2,J3,c) \
  HALF*(faceNormal(J1,J2,J3,x,axis)+faceNormal(J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),x,axis)) \
  *Dpu(axis,J1,J2,J3,c)
//                                                                                                    _______
//                                                                                                   |   |   | 
//                                                                                                   |___|___| 
//                                                                                                   |   |   | axis
//      AVrxDmu(x,axis,oaxis,...,c) is an four-cell average value of rxDmu;                          |___|___|  ^
//         differencing is in the axis direction; averaging is in the axis and oaxis directions      |   |   |  |
//         used to compute contribution to faceCentered difference                                   |___|___|  |--> oaxis
//
#define AVrxDmu(x,axis,oaxis,J1,J2,J3,c) QUARTER*( \
						  rxDmu(x,axis,J1,J2,J3,c) + \
						  rxDmu(x,axis,J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),c) + \
						  rxDmu(x,axis,J1-inc(oaxis,0),J2-inc(oaxis,1),J3-inc(oaxis,2),c) + \
						  rxDmu(x,axis,J1+inc(axis,0)-inc(oaxis,0),J2+inc(axis,1)-inc(oaxis,1),J3+inc(axis,2)-inc(oaxis,2),c))

//    axis-face values of rx averaged to the oaxis-face
//
#define AVrx(oaxis,J1,J2,J3,x,axis) QUARTER*( \
					     faceNormal(J1,J2,J3,x,axis) + \
					     faceNormal(J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),x,axis) + \
					     faceNormal(J1-inc(oaxis,0),J2-inc(oaxis,1),J3-inc(oaxis,2),x,axis) + \
					     faceNormal(J1+inc(axis,0)-inc(oaxis,0),J2+inc(axis,1)-inc(oaxis,1),J3+inc(axis,2)-inc(oaxis,2),x,axis))

//  
//#define rxAVD0u(x,axis,oaxis,J1,J2,J3,c) HALF*centerNormal(J1,J2,J3,x,axis)* \
//					       (D0u(axis,J1,J2,J3,c) + D0u(axis,J1+inc(oaxis,0),J2+inc(oaxis,1),J3+inc(oaxis,2),c))
// ***  workaround centerNormal bug ******
#define rxAVD0u(x,axis,oaxis,J1,J2,J3,c) \
  QUARTER*(faceNormal(J1,J2,J3,x,axis)+faceNormal(J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),x,axis))* \
    (D0u(axis,J1,J2,J3,c) + D0u(axis,J1+inc(oaxis,0),J2+inc(oaxis,1),J3+inc(oaxis,2),c))

  //... loop over all output components; gridFunctionType of each input component might be different

  for (c=C.getBase(); c<=C.getBound(); c++)
  {
    // ... get the input GridFunctionType of this component

//    realMappedGridFunction::GridFunctionType inputGridFunctionType = getGridFunctionComponentType (u, c):
    GridFunctionParameters::GridFunctionType inputGridFunctionType = u.getGridFunctionType (c); 

  switch (inputGridFunctionType)
  {
  case faceCenteredAxis1:
    inputAxis=0;
    break;
  case faceCenteredAxis2:
    inputAxis=1;
    break;
  case faceCenteredAxis3:
    inputAxis=2;
    break;
  default: // centered or general
    inputAxis=-1;
    break;
  }

    
    // ... figure out for what Index'es to compute the derivatives for this component
//   getDefaultIndex (inputGridFunctionType, derivativeGridFunctionType, J1, J2, J3, c, I1, I2, I3);
   getDefaultIndex (inputGridFunctionType, derivativeGridFunctionType, J1, J2, J3, I1, I2, I3);


    // ... now go through all possible cases


    switch (inputGridFunctionType)

    {
    // ========================================
    case cellCentered:
    // ========================================

      switch (derivativeGridFunctionType)
      {
	// ====================
      case cellCentered:    // cellCentered -> cellCentered
	// ====================
	
	// ... use Do in all directions

	ForAxes (axis)
	{
	  //  ... 960720: bug in centerNormal, so average faceNormal's explicitly instead:
	  //	    returnedValue (J1,J2,J3,c) += centerNormal(J1,J2,J3,direction,axis)*D0u(axis,J1,J2,J3,c);	  

	  returnedValue (J1,J2,J3,c) += HALF*
	    (faceNormal(J1,J2,J3,direction,axis) + faceNormal(J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),direction,axis))*
	      D0u(axis,J1,J2,J3,c);

	}
	// ... divide by volume
	returnedValue (J1,J2,J3,c) /= cellVolume(J1,J2,J3);
	break;

	// ====================	
      case faceCenteredAxis1:  // cellCentered -> faceCentered
      case faceCenteredAxis2:
      case faceCenteredAxis3:
	// ====================
	
	// ... one-sided backward difference in the output centering direction

	returnedValue (J1,J2,J3,c) = faceNormal(J1,J2,J3,direction,outputAxis)*Dmu(outputAxis,J1,J2,J3,c);
	
	// ... average four one-sided differences in the other directions

	ForAxes (axis)
	{
	  if (axis!=outputAxis)
	    returnedValue (J1,J2,J3,c) +=  (AVrxDmu(direction,axis,outputAxis,J1,J2,J3,c));
	}
	


	// ... divide by volume
	returnedValue (J1,J2,J3,c) /= HALF*(cellVolume(J1,J2,J3) + cellVolume(J1-inc(outputAxis,0),J2-inc(outputAxis,1),J3-inc(outputAxis,2)));
	
	break;
	
      default:
	cout << "Error: should be impossible to reach this statement" << endl;
	throw "derivative error";

      }
      break;
    
      
	// ====================
//  switch (inputGridFunctionType)
    case faceCenteredAxis1:
    case faceCenteredAxis2:
    case faceCenteredAxis3:
	// ====================
      
      
      switch (derivativeGridFunctionType)
      {
	// ====================
      case cellCentered:  // faceCentered -> cellCentered
	// ====================

	// ... forward one-sided difference in input direction

	if (0)
	{
	  returnedValue (J1,J2,J3,c) = 
	    centerNormal(J1,J2,J3,direction,inputAxis)*Dpu(inputAxis,J1,J2,J3,c);
	}
	else
	{
	  // *** workaround centerNormal bug ***
	  ia = inputAxis;
	  returnedValue (J1,J2,J3,c) = HALF*
	    (faceNormal(J1,J2,J3,direction,ia) + faceNormal(J1+inc(ia,0),J2+inc(ia,1),J3+inc(ia,2),direction,ia))
	      *Dpu(ia,J1,J2,J3,c);
	}
	
	// ... average of Do in other directions

	if (1)
	{
	  ForAxes (axis)
	  {
	    if (axis!=inputAxis)
	      returnedValue (J1,J2,J3,c) +=  rxAVD0u(direction,axis,inputAxis,J1,J2,J3,c);
	  }
	}
	else
	{
	  // ... try a different approximation: average of one-sided differences:
	  int io = inputAxis;
	  K1 = Range (J1.getBase()+inc(io,0),J1.getBound()-inc(io,0));
	  K2 = Range (J2.getBase()+inc(io,1),J2.getBound()-inc(io,1));
	  K3 = Range (J3.getBase()+inc(io,2),J2.getBound()-inc(io,2));
/*	  
	  ForAxes (axis)
	  {
	    if (axis != inputAxis)

	      returnedValue (K1,K2,K3,c) += QUARTER*HALF*
	       ((faceNormal(K1,K2,K3,direction,axis) + faceNormal(K1-inc(io,0),K2-inc(io,1),K3-inc(io,2),direction,axis))*
		Dmu(axis,K1,K2,K3,c) +
		(faceNormal(K1,K2,K3,direction,axis) + faceNormal(K1+inc(io,0),K2+inc(io,1),K3+inc(io,2),direction,axis))*
		Dpu(axis,K1,K2,K3,c) +
		(faceNormal(K1+inc(axis,0)          ,K2+inc(axis,1)          ,K3+inc(axis,2)          ,direction,axis) +
		 faceNormal(K1+inc(axis,0)-inc(io,0),K2+inc(axis,1)-inc(io,1),K3+inc(axis,2)-inc(io,2),direction,axis))*
		Dmu(axis,K1+inc(axis,0),K2+inc(axis,1),K3+inc(axis,2),c) +
		(faceNormal(
		 


		(Facenormal(K1,K2,K3,direction,axis)
		 * (Dmu(axis,K1,K2,K3,c) + Dmu(axis,K1+inc(io,0),K2+inc(io,1),K3+inc(io,2),c)) +
		 faceNormal(K1+inc(axis,0),K2+inc(axis,1),K3+inc(axis,2),direction,axis)
		 * (Dpu(axis,K1,K2,K3,c) + Dpu(axis,K1+inc(io,0),K2+inc(io,1),K3+inc(io,2),c)));
	  }
*/
	}
	
		    

	// ... divide by volume

	returnedValue (J1,J2,J3,c) /= cellVolume(J1,J2,J3);

	break;
	
	// ====================
      case faceCenteredAxis1:  // faceCentered -> faceCentered
      case faceCenteredAxis2:
      case faceCenteredAxis3:
	// ====================

	if (inputGridFunctionType == derivativeGridFunctionType)
	{
	  
	  // ... centered difference in output direction

	  returnedValue (J1,J2,J3,c) = faceNormal(J1,J2,J3,direction,outputAxis)*D0u(outputAxis,J1,J2,J3,c);
	  
	  // ... use centered difference in other directions, but average 4 faceNormals

	  ForAxes (axis)
	  {
	    if (axis!=outputAxis)
	    {
	      returnedValue (J1,J2,J3,c) +=  AVrx(outputAxis,J1,J2,J3,direction,axis)*D0u(axis,J1,J2,J3,c);
	    }
	    
	  }
	  

	  // ... divide by average volume for the face

	  returnedValue (J1,J2,J3,c) /= HALF*(cellVolume(J1,J2,J3) + cellVolume(J1-inc(outputAxis,0),J2-inc(outputAxis,1),J3-inc(outputAxis,2)));
	}
	else
	{

	  // ... average of one-sided differences output direction, using faceNormals

	  returnedValue (J1,J2,J3,c) = HALF*faceNormal(J1,J2,J3,direction,outputAxis)*
	    (Dmu(outputAxis,J1,J2,J3,c) + Dmu(outputAxis,J1+inc(inputAxis,0),J2+inc(inputAxis,1),J3+inc(inputAxis,2),c));
	  
	  // ... average of one-sided differences in other directions, using averaged centerNormals

	  ForAxes (axis)
	  {
	    if (axis!=outputAxis)
	    {
	      if (0)
	      {
		returnedValue (J1,J2,J3,c) += 
		  QUARTER*(centerNormal(J1,J2,J3,direction,axis) + 
			   centerNormal(J1-inc(outputAxis,0),J2-inc(outputAxis,1),J3-inc(outputAxis,2),direction,axis))*
			     (Dpu(axis,J1,J2,J3,c) + Dpu(axis,J1-inc(outputAxis,0),J2-inc(outputAxis,1),J3-inc(outputAxis,2),c));
	      }
	      else
	      {
		// *** workaround for centerNormal bug ***
		returnedValue (J1,J2,J3,c) += 
		  QUARTER*HALF*(faceNormal(J1,J2,J3,direction,axis) + 
				faceNormal(J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),direction,axis) +
				faceNormal(J1-inc(oa,0),J2-inc(oa,1),J3-inc(oa,2),direction,axis) +
				faceNormal(J1-inc(oa,0)+inc(axis,0),J2-inc(oa,1)+inc(axis,1),J3-inc(oa,2)+inc(axis,2),direction,axis))*
				  (Dpu(axis,J1,J2,J3,c) + Dpu(axis,J1-inc(outputAxis,0),J2-inc(outputAxis,1),J3-inc(outputAxis,2),c));
	      }
	    }
	  }

	  
	  // ... divide by average volume

	  returnedValue (J1,J2,J3,c) /= HALF*(cellVolume(J1,J2,J3) +
					      cellVolume(J1-inc(outputAxis,0),J2-inc(outputAxis,1),J3-inc(outputAxis,2)));
	}
		  
	break;

      default:
	cout << "Error: should be impossible to reach this statement" << endl;
	throw "derivative error";

	
      }

      break;
    

    default:
      cout << "Error: should be impossible to reach this statement" << endl;
      throw "derivative error";

      
      
    }
  }

					   }	
 
	
  
	
	     
	
	
					     
	  
	  
				      
