#include "MappedGridFiniteVolumeOperators.h"
#include "billsMergeMacro.h"
#include "laplacian.h"
#include <xDC.h>

realMappedGridFunction MappedGridFiniteVolumeOperators::
xCoefficients (
	       const Index & I1, //=nullIndex
	       const Index & I2, //=nullIndex
	       const Index & I3, //=nullIndex
	       const Index & E, //=nullIndex
	       const Index & C, //=nullIndex
	       const Index & I6, //=nullIndex
	       const Index & I7, //=nullIndex
	       const Index & I8  //=nullIndex
	       )
{
  if (numberOfComponentsForCoefficients == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setNumberOfComponentsForCoefficients before calling implicit operators" << endl;
    throw "xCoefficients: FATAL ERROR";
  }

  if (stencilSize == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setStencilSize() to set stencil size before calling implicit operators" << endl;
    throw "xCoefficients: FATAL ERROR";
  }

  GridFunctionParameters gfParams;  // this defaults both input and output types

  int c0 = C.getBase();
  int e0 = E.getBase();
  Index M(0,stencilSize);
  Index M0(CE(c0,e0),stencilSize);

  int stencilDimension = stencilSize*SQR(numberOfComponentsForCoefficients);
  realMappedGridFunction returnedValue(mappedGrid, stencilDimension, all, all, all);
  returnedValue = 0.;
  realMappedGridFunction scalarValue  (mappedGrid, stencilSize, all, all, all);
  scalarValue = 0.;
  
  firstDerivativeCoefficients (scalarValue, xAxis, gfParams, I1, I2, I3);

  //...first set the lowest block
  returnedValue(M0,I1,I2,I3) = scalarValue(M,I1,I2,I3);


  //...now copy coefficients if more than one (component,equation) are being set

  for (int c=C.getBase(); c<=C.getBound(); c++)
    for (int e=E.getBase(); e<=E.getBound(); e++)
      if ( c!=c0 || e!=e0 )
	returnedValue(M+CE(c,e),I1,I2,I3) = returnedValue(M+CE(c0,e0),I1,I2,I3);

  return (returnedValue);
}

realMappedGridFunction MappedGridFiniteVolumeOperators::
yCoefficients (
	       const Index & I1, //=nullIndex
	       const Index & I2, //=nullIndex
	       const Index & I3, //=nullIndex
	       const Index & E, //=nullIndex
	       const Index & C, //=nullIndex
	       const Index & I6, //=nullIndex
	       const Index & I7, //=nullIndex
	       const Index & I8 //=nullIndex
	       )
{
  if (numberOfComponentsForCoefficients == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setNumberOfComponentsForCoefficients before calling implicit operators" << endl;
    throw "xCoefficients: FATAL ERROR";
  }

  if (stencilSize == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setStencilSize() to set stencil size before calling implicit operators" << endl;
    throw "xCoefficients: FATAL ERROR";
  }

  GridFunctionParameters gfParams;  // this defaults both input and output types

  int c0 = C.getBase();
  int e0 = E.getBase();
  Index M(0,stencilSize);
  Index M0(CE(c0,e0),stencilSize);

  
  int stencilDimension = stencilSize*SQR(numberOfComponentsForCoefficients);
  realMappedGridFunction returnedValue(mappedGrid, stencilDimension, all, all, all);
  returnedValue = 0.;
  realMappedGridFunction scalarValue  (mappedGrid, stencilSize, all, all, all);
  scalarValue = 0.;
  
  firstDerivativeCoefficients (scalarValue, yAxis, gfParams, I1, I2, I3);

  //...first set the lowest block
  returnedValue(M0,I1,I2,I3) = scalarValue(M,I1,I2,I3);

  //...now copy coefficients if more than one (component,equation) are being set

  for (int c=C.getBase(); c<=C.getBound(); c++)
    for (int e=E.getBase(); e<=E.getBound(); e++)
      if ( c!=c0 || e!=e0 )
	returnedValue(M+CE(c,e),I1,I2,I3) = returnedValue(M+CE(c0,e0),I1,I2,I3);

  return (returnedValue);

}

realMappedGridFunction MappedGridFiniteVolumeOperators::
zCoefficients (
	       const Index & I1, //=nullIndex
	       const Index & I2, //=nullIndex
	       const Index & I3, //=nullIndex
	       const Index & E, //=nullIndex
	       const Index & C, //=nullIndex
	       const Index & I6, //=nullIndex
	       const Index & I7, //=nullIndex
	       const Index & I8 //=nullIndex
	       )
{
  if (numberOfComponentsForCoefficients == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setNumberOfComponentsForCoefficients before calling implicit operators" << endl;
    throw "xCoefficients: FATAL ERROR";
  }

  if (stencilSize == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setStencilSize() to set stencil size before calling implicit operators" << endl;
    throw "xCoefficients: FATAL ERROR";
  }

  GridFunctionParameters gfParams;  // this defaults both input and output types

  int c0 = C.getBase();
  int e0 = E.getBase();
  Index M(0,stencilSize);
  Index M0(CE(c0,e0),stencilSize);

  int stencilDimension = stencilSize*SQR(numberOfComponentsForCoefficients);
  realMappedGridFunction returnedValue(mappedGrid, stencilDimension, all, all, all);
  returnedValue = 0.;
  realMappedGridFunction scalarValue  (mappedGrid, stencilSize, all, all, all);
  scalarValue = 0.;
  
  firstDerivativeCoefficients (scalarValue, zAxis, gfParams, I1, I2, I3);

  //...first set the lowest block
  returnedValue(M0,I1,I2,I3) = scalarValue(M,I1,I2,I3);

  //...now copy coefficients if more than one (component,equation) are being set

  for (int c=C.getBase(); c<=C.getBound(); c++)
    for (int e=E.getBase(); e<=E.getBound(); e++)
      if ( c!=c0 || e!=e0 )
	returnedValue(M+CE(c,e),I1,I2,I3) = returnedValue(M+CE(c0,e0),I1,I2,I3);

  return (returnedValue);
  
}

realMappedGridFunction MappedGridFiniteVolumeOperators::
xCoefficients (
	       const GridFunctionParameters & gfParams,
	       const Index & I1, //=nullIndex
	       const Index & I2, //=nullIndex
	       const Index & I3, //=nullIndex
	       const Index & E, //=nullIndex
	       const Index & C, //=nullIndex
	       const Index & I6, //=nullIndex
	       const Index & I7, //=nullIndex
	       const Index & I8  //=nullIndex
	       )
{
  if (numberOfComponentsForCoefficients == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setNumberOfComponentsForCoefficients before calling implicit operators" << endl;
    throw "xCoefficients: FATAL ERROR";
  }

  if (stencilSize == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setStencilSize() to set stencil size before calling implicit operators" << endl;
    throw "xCoefficients: FATAL ERROR";
  }

  int stencilDimension = stencilSize*SQR(numberOfComponentsForCoefficients);
  realMappedGridFunction returnedValue(mappedGrid, stencilDimension, all, all, all);;
  returnedValue = 0.;

  firstDerivativeCoefficients (returnedValue, xAxis, gfParams, I1, I2, I3);
  return (returnedValue);
}

realMappedGridFunction MappedGridFiniteVolumeOperators::
yCoefficients (
	       const GridFunctionParameters & gfParams,
	       const Index & I1, //=nullIndex
	       const Index & I2, //=nullIndex
	       const Index & I3, //=nullIndex
	       const Index & E, //=nullIndex
	       const Index & C, //=nullIndex
	       const Index & I6, //=nullIndex
	       const Index & I7, //=nullIndex
	       const Index & I8 //=nullIndex
	       )
{
  if (numberOfComponentsForCoefficients == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setNumberOfComponentsForCoefficients before calling implicit operators" << endl;
    throw "xCoefficients: FATAL ERROR";
  }

  if (stencilSize == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setStencilSize() to set stencil size before calling implicit operators" << endl;
    throw "xCoefficients: FATAL ERROR";
  }
  int c0 = C.getBase();
  int e0 = E.getBase();
  Index M(0,stencilSize);
  Index M0(CE(c0,e0),stencilSize);

  
  int stencilDimension = stencilSize*SQR(numberOfComponentsForCoefficients);
  realMappedGridFunction returnedValue(mappedGrid, stencilDimension, all, all, all);
  returnedValue = 0.;
  realMappedGridFunction scalarValue  (mappedGrid, stencilSize, all, all, all);
  scalarValue = 0.;
  
  firstDerivativeCoefficients (scalarValue, yAxis, gfParams, I1, I2, I3);

  //...first set the lowest block
  returnedValue(M0,I1,I2,I3) = scalarValue(M,I1,I2,I3);

  //...now copy coefficients if more than one (component,equation) are being set

  for (int c=C.getBase(); c<=C.getBound(); c++)
    for (int e=E.getBase(); e<=E.getBound(); e++)
      if ( c!=c0 || e!=e0 )
	returnedValue(M+CE(c,e),I1,I2,I3) = returnedValue(M+CE(c0,e0),I1,I2,I3);

  return (returnedValue);
}
realMappedGridFunction MappedGridFiniteVolumeOperators::
zCoefficients (
	       const GridFunctionParameters & gfParams,
	       const Index & I1, //=nullIndex
	       const Index & I2, //=nullIndex
	       const Index & I3, //=nullIndex
	       const Index & E, //=nullIndex
	       const Index & C, //=nullIndex
	       const Index & I6, //=nullIndex
	       const Index & I7, //=nullIndex
	       const Index & I8 //=nullIndex
	       )
{
  if (numberOfComponentsForCoefficients == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setNumberOfComponentsForCoefficients before calling implicit operators" << endl;
    throw "xCoefficients: FATAL ERROR";
  }

  if (stencilSize == 0)
  {
    cout << "ERROR: call MappedGridFiniteVolumeOperators::setStencilSize() to set stencil size before calling implicit operators" << endl;
    throw "xCoefficients: FATAL ERROR";
  }
  int c0 = C.getBase();
  int e0 = E.getBase();
  Index M(0,stencilSize);
  Index M0(CE(c0,e0),stencilSize);

  int stencilDimension = stencilSize*SQR(numberOfComponentsForCoefficients);
  realMappedGridFunction returnedValue(mappedGrid, stencilDimension, all, all, all);
  returnedValue = 0.;
  realMappedGridFunction scalarValue  (mappedGrid, stencilSize, all, all, all);
  scalarValue = 0.;
  
  firstDerivativeCoefficients (scalarValue, zAxis, gfParams, I1, I2, I3);

  //...first set the lowest block
  returnedValue(M0,I1,I2,I3) = scalarValue(M,I1,I2,I3);

  //...now copy coefficients if more than one (component,equation) are being set

  for (int c=C.getBase(); c<=C.getBound(); c++)
    for (int e=E.getBase(); e<=E.getBound(); e++)
      if ( c!=c0 || e!=e0 )
	returnedValue(M+CE(c,e),I1,I2,I3) = returnedValue(M+CE(c0,e0),I1,I2,I3);

  return (returnedValue);
}


void MappedGridFiniteVolumeOperators::
firstDerivativeCoefficients (realMappedGridFunction & returnedValue,
			     const int & direction,
			     const GridFunctionParameters & gfParams,
			     const Index & I1, //=nullIndex
			     const Index & I2, //=nullIndex
			     const Index & I3 //=nullIndex
			     )
//
// /Description:
//  The following convention is used for specifying the input and output GridFunctionType:
//     gfParams.inputType is the type of the SOLUTION of the problem that will be solved with
//                   these coefficients
//     gfParams.outputType is the type of the RHS of the problem that will be solved.
//
//  While this convention may seem reversed, it is consistent with the convention for
//  specifying the GridFunctionParameters object for the forward differential operators.
//
{
  
  real QUARTER = 0.25, HALF = 0.5;
  int axis, inputAxis, outputAxis, i, j, k;
  Index J1, J2, J3;



  GridFunctionParameters::GridFunctionType inputGridFunctionType  = gfParams.inputType;
  GridFunctionParameters::GridFunctionType outputGridFunctionType = gfParams.outputType;
  

  // ... For convenience, define local copies of the GridFunctionType's


  const GridFunctionParameters::GridFunctionType defaultCentering  = GridFunctionParameters::defaultCentering; 
//  const GridFunctionParameters::GridFunctionType general           = GridFunctionParameters::general; 
//  const GridFunctionParameters::GridFunctionType vertexCentered    = GridFunctionParameters::vertexCentered; 
  const GridFunctionParameters::GridFunctionType cellCentered      = GridFunctionParameters::cellCentered; 
//  const GridFunctionParameters::GridFunctionType faceCenteredAll   = GridFunctionParameters::faceCenteredAll; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis1 = GridFunctionParameters::faceCenteredAxis1; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis2 = GridFunctionParameters::faceCenteredAxis2; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis3 = GridFunctionParameters::faceCenteredAxis3; 

  // ... if either the inputGridFunctionType and/or outputGridFunctionType are not given, use cellCentered

  if (inputGridFunctionType  == defaultCentering) inputGridFunctionType = cellCentered;
  if (outputGridFunctionType == defaultCentering) outputGridFunctionType = cellCentered;

  // ... determine inputAxis for future use
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

  int iaxs = inputAxis;

  // ... determine outputAxis for future use
  switch (outputGridFunctionType)
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

  int oaxs = outputAxis;

// ... needed for MERGE0 macro
  Range aR0, aR1, aR2, aR3;
  int dum;

// ... RV macro makes returnedValue look like it has 6 indexes by expanding the first index
// ... (CF is defined in laplacian.h)
#define RV(l,m,n,I,J,K) MERGE0(returnedValue,CF(l,m,n),I,J,K)

// ... ForAllCoefficients is used to loop over all coefficients indices
// ... (halfwidth[1,2,3], are defined in the MappedGridFiniteVolumeOperators class;)
#define ForAllCoefficients(i,j,k) \
  for(k=-halfWidth3; k<=halfWidth3; k++) \
    for (j=-halfWidth2; j<=halfWidth2; j++) \
      for (i=-halfWidth1; i<=halfWidth1; i++)

                                 
  
//  int c = 0; // ***???

//  int testValue1 = CF(0,0,0), testValue2 = CF(0,0,-1);
  
  
  getDefaultIndex (inputGridFunctionType, outputGridFunctionType, J1, J2, J3, I1, I2, I3); // *** ???
  

  switch (inputGridFunctionType)
  {
    // ========================================
  case cellCentered:
    // ========================================

    switch (outputGridFunctionType)
    {
      // ====================
    case cellCentered:
      // ====================

      // ... centered difference in all directions

      ForAxes (axis)
      {
	RV ( inc(0,axis), inc(1,axis), inc(2,axis),J1,J2,J3) += HALF*centerNormal(J1,J2,J3,direction,axis);
	RV (-inc(0,axis),-inc(1,axis),-inc(2,axis),J1,J2,J3) -= HALF*centerNormal(J1,J2,J3,direction,axis);
      }
      ForAllCoefficients (i,j,k) RV(i,j,k,J1,J2,J3) /= cellVolume(J1,J2,J3);
      break;
      
      // ====================
    case faceCenteredAxis1:
    case faceCenteredAxis2:
    case faceCenteredAxis3:
      // ====================

      //  ... one-sided backward difference in the output centering direction

      axis = outputAxis;
      RV (0,           0,            0,          J1,J2,J3) += faceNormal(J1,J2,J3,direction,axis);
      RV (-inc(axis,0),-inc(axis,1),-inc(axis,2),J1,J2,J3) -= faceNormal(J1,J2,J3,direction,axis);
      
      // ... average four one-sided differences in the other directions

//#define Dmu(axis,J1,J2,J3,c) (u(J1,J2,J3,c) - u(J1-inc(axis,0),J2-inc(axis,1),J3-inc(axis,2),c))
//#define rxDmu(x,axis,J1,J2,J3,c) faceNormal(J1,J2,J3,x,axis)*Dmu(axis,J1,J2,J3,c)
//#define AVrxDmu(x,axis,oaxis,J1,J2,J3,c) QUARTER*( \
//						  rxDmu(x,axis,J1,J2,J3,c) + \
//						  rxDmu(x,axis,J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),c) + \
//						  rxDmu(x,axis,J1-inc(oaxis,0),J2-inc(oaxis,1),J3-inc(oaxis,2),c) + \
//						  rxDmu(x,axis,J1+inc(axis,0)-inc(oaxis,0),J2+inc(axis,1)-inc(oaxis,1),J3+inc(axis,2)-inc(oaxis,2),c))
//
//	    returnedValue (J1,J2,J3,c) +=  (AVrxDmu(direction,axis,outputAxis,J1,J2,J3,c));

      ForAxes (axis)
      {
	if (axis!=outputAxis)
	{
	  RV( 0           , 0        , 0           ,J1,J2,J3) += QUARTER*faceNormal(J1,J2,J3,direction,axis);
	  RV(-inc(axis,0),-inc(axis,1),-inc(axis,2),J1,J2,J3) -= QUARTER*faceNormal(J1,J2,J3,direction,axis);

	  RV( inc(axis,0), inc(axis,1), inc(axis,2),J1,J2,J3) += QUARTER*faceNormal(J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),direction,axis);
	  RV( 0          , 0          , 0          ,J1,J2,J3) -= QUARTER*faceNormal(J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),direction,axis);
	  
	  RV(-inc(oaxs,0),-inc(oaxs,1),-inc(oaxs,2),J1,J2,J3) += QUARTER*faceNormal(J1-inc(oaxs,0),J2-inc(oaxs,1),J3-inc(oaxs,2),direction,axis);
	  RV(-inc(oaxs,0)-inc(axis,0),-inc(oaxs,1)-inc(axis,1),-inc(oaxs,2)-inc(axis,2),J1,J2,J3) 
	                                                      -= QUARTER*faceNormal(J1-inc(oaxs,0),J2-inc(oaxs,1),J3-inc(oaxs,2),direction,axis);

	  RV(-inc(oaxs,0)+inc(axis,0),-inc(oaxs,1)+inc(axis,1),-inc(oaxs,2)+inc(axis,2),J1,J2,J3) += 
	    QUARTER*faceNormal(J1-inc(oaxs,0)+inc(axis,0),J2-inc(oaxs,1)+inc(axis,1),J3-inc(oaxs,2)+inc(axis,2),direction,axis);
	  RV(-inc(oaxs,0),-inc(oaxs,1),-inc(oaxs,2),J1,J2,J3) -= 
	    QUARTER*faceNormal(J1-inc(oaxs,0)+inc(axis,0),J2-inc(oaxs,1)+inc(axis,1),J3-inc(oaxs,2)+inc(axis,2),direction,axis);
	}
      }

      ForAllCoefficients (i,j,k) RV(i,j,k,J1,J2,J3) /= HALF*(cellVolume(J1,J2,J3) + cellVolume(J1-inc(oaxs,0),J2-inc(oaxs,1),J3-inc(oaxs,2)));
      
      break;
    }
    break;
    
    // ====================
    //switch (inputGridFunctionType)
  case faceCenteredAxis1:
  case faceCenteredAxis2:
  case faceCenteredAxis3:
    // ====================

    switch (outputGridFunctionType)
    {
      // ====================
    case cellCentered:
      // ====================

      // ... forward one-sided difference in input direction

      RV (inc(iaxs,0),inc(iaxs,1),inc(iaxs,2),J1,J2,J3) += centerNormal(J1,J2,J3,direction,iaxs);
      RV (0          ,0          ,0          ,J1,J2,J3) -= centerNormal(J1,J2,J3,direction,iaxs);
      
      // ... average of Do in other directions

      ForAxes (axis)
      {
	if (axis!=inputAxis)
	{
//#define rxAVD0u(x,axis,oaxis,J1,J2,J3,c) HALF*centerNormal(J1,J2,J3,x,axis)* \
//    (D0u(axis,J1,J2,J3,c) + D0u(axis,J1+inc(oaxis,0),J2+inc(oaxis,1),J3+inc(oaxis,2),c))
//
//	  returnedValue (J1,J2,J3,c) +=  rxAVD0u(direction,axis,inputAxis,J1,J2,J3,c);
	
	  RV( inc(axis,0), inc(axis,1), inc(axis,2),J1,J2,J3) += QUARTER*centerNormal(J1,J2,J3,direction,axis);
	  RV(-inc(axis,0),-inc(axis,1),-inc(axis,2),J1,J2,J3) -= QUARTER*centerNormal(J1,J2,J3,direction,axis);
	  RV( inc(axis,0)+inc(iaxs,0), inc(axis,1)+inc(iaxs,1), inc(axis,2)+inc(iaxs,2),J1,J2,J3) += QUARTER*centerNormal(J1,J2,J3,direction,axis);
	  RV(-inc(axis,0)+inc(iaxs,0),-inc(axis,1)+inc(iaxs,1),-inc(axis,2)+inc(iaxs,2),J1,J2,J3) -= QUARTER*centerNormal(J1,J2,J3,direction,axis);
	}
      }
      ForAllCoefficients(i,j,k) RV(i,j,k,J1,J2,J3) /= cellVolume(J1,J2,J3);
      break;

      // ====================
    case faceCenteredAxis1:
    case faceCenteredAxis2:
    case faceCenteredAxis3:
      // ====================

      if (inputGridFunctionType == outputGridFunctionType)
      {
	// ... centered difference in output direction
//	  returnedValue (J1,J2,J3,c) = faceNormal(J1,J2,J3,direction,outputAxis)*D0u(outputAxis,J1,J2,J3,c);

	RV( inc(oaxs,0), inc(oaxs,1), inc(oaxs,2), J1,J2,J3) += HALF*faceNormal(J1,J2,J3,direction,oaxs);
	RV(-inc(oaxs,0),-inc(oaxs,1),-inc(oaxs,2), J1,J2,J3) -= HALF*faceNormal(J1,J2,J3,direction,oaxs);

		  // ... use centered difference in other directions, but average 4 faceNormals

	ForAxes (axis)
	{
	  if (axis != outputAxis)
	  {
//#define AVrx(oaxis,J1,J2,J3,x,axis) QUARTER*( \
//					     faceNormal(J1,J2,J3,x,axis) + \
//					     faceNormal(J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),x,axis) + \
//					     faceNormal(J1-inc(oaxis,0),J2-inc(oaxis,1),J3-inc(oaxis,2),x,axis) + \
//					     faceNormal(J1+inc(axis,0)-inc(oaxis,0),J2+inc(axis,1)-inc(oaxis,1),J3+inc(axis,2)-inc(oaxis,2),x,axis))

	    RV( inc(axis,0), inc(axis,1), inc(axis,2),J1,J2,J3) += HALF*QUARTER*
	      (faceNormal(J1,J2,J3,direction,axis) +
	       faceNormal(J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),direction,axis) +
	       faceNormal(J1-inc(oaxs,0),J2-inc(oaxs,1),J3-inc(oaxs,2),direction,axis) +
	       faceNormal(J1+inc(axis,0)-inc(oaxs,0),J2+inc(axis,1)-inc(oaxs,1),J3+inc(axis,2)-inc(oaxs,2),direction,axis));
	  
	    RV(-inc(axis,0),-inc(axis,1),-inc(axis,2),J1,J2,J3) += HALF*QUARTER*
	      (faceNormal(J1,J2,J3,direction,axis) +
	       faceNormal(J1+inc(axis,0),J2+inc(axis,1),J3+inc(axis,2),direction,axis) +
	       faceNormal(J1-inc(oaxs,0),J2-inc(oaxs,1),J3-inc(oaxs,2),direction,axis) +
	       faceNormal(J1+inc(axis,0)-inc(oaxs,0),J2+inc(axis,1)-inc(oaxs,1),J3+inc(axis,2)-inc(oaxs,2),direction,axis));
	  }
	}
	
	ForAllCoefficients (i,j,k) RV(i,j,k,J1,J2,J3) /= HALF*(cellVolume(J1,J2,J3) + cellVolume(J1-inc(oaxs,0),J2-inc(oaxs,1),J3-inc(oaxs,2)));
      }
      else // if (inputGridFunctionType != outputGridFunctionType)
      {

	// ... average of one-sided differences output direction, using faceNormals

	RV( 0          , 0          , 0          ,J1,J2,J3) += HALF*faceNormal(J1,J2,J3,direction,oaxs);
	RV(-inc(oaxs,0),-inc(oaxs,1),-inc(oaxs,2),J1,J2,J3) -= HALF*faceNormal(J1,J2,J3,direction,oaxs);
	RV( inc(iaxs,0), inc(iaxs,1), inc(iaxs,2),J1,J2,J3) += HALF*faceNormal(J1,J2,J3,direction,oaxs);
	RV( inc(iaxs,0)-inc(oaxs,0), inc(iaxs,1)-inc(iaxs,1), inc(iaxs,2)-inc(iaxs,2),J1,J2,J3) -= HALF*faceNormal(J1,J2,J3,direction,oaxs);
	
	// ... average of one-sided differences in other directions, using averaged centerNormals

	ForAxes (axis)
	{
	  if (axis!=outputAxis)
	  {
	    RV( inc(axis,0), inc(axis,1), inc(axis,2), J1,J2,J3) += 
	      QUARTER*(centerNormal(J1,J2,J3,direction,axis) + centerNormal(J1-inc(oaxs,0),J2-inc(oaxs,1),J3-inc(oaxs,2),direction,axis));
	    RV( 0          , 0          , 0          , J1,J2,J3) -=
	      QUARTER*(centerNormal(J1,J2,J3,direction,axis) + centerNormal(J1-inc(oaxs,0),J2-inc(oaxs,1),J3-inc(oaxs,2),direction,axis));
	    RV( inc(axis,0)-inc(oaxs,0), inc(axis,1)-inc(oaxs,1), inc(axis,2)-inc(oaxs,2), J1,J2,J3) +=
	      QUARTER*(centerNormal(J1,J2,J3,direction,axis) + centerNormal(J1-inc(oaxs,0),J2-inc(oaxs,1),J3-inc(oaxs,2),direction,axis));
	    RV(-inc(oaxs,0),-inc(oaxs,1),-inc(oaxs,2),J1,J2,J3) -=
	      QUARTER*(centerNormal(J1,J2,J3,direction,axis) + centerNormal(J1-inc(oaxs,0),J2-inc(oaxs,1),J3-inc(oaxs,2),direction,axis));
	  }
	}

	  // ... divide by average volume

	ForAllCoefficients (i,j,k) RV(i,j,k,J1,J2,J3) /= HALF*(cellVolume(J1,J2,J3) + cellVolume(J1-inc(oaxs,0),J2-inc(oaxs,1),J3-inc(oaxs,2)));
      }
      
      break;
      
    default:
      throw "derivative error";
	
      }
    

    default:
      cout << "Error: should be impossible to reach this statement" << endl;
      throw "derivative error";
  }
}

#undef RV
#undef ForAllCoefficients


      
