#include "MappedGridFiniteVolumeOperators.h"
// =================================================================================
	// This function used below
	// ========================================

// inline int uaxis(int axis, int positionOfComponent) { return (axis<positionOfComponent ? axis : axis+1);}

void MappedGridFiniteVolumeOperators::
getDefaultIndex (
		 const GridFunctionParameters::GridFunctionType & inputGFType,
		 const GridFunctionParameters::GridFunctionType & outputGFType,
		 Index & I1,
		 Index & I2,
		 Index & I3,
		 const Index & I1input, // = nullIndex,
		 const Index & I2input, // = nullIndex,
		 const Index & I3input // = nullIndex
		 )
  //========================================
  // Author:		D.L.Brown
  // Date Created:	960620
  // Date Modified:	
  //
  // Purpose:
  //   return default Index'es for which values will
  //   be returned for various functions. This version is
  //   used with the general first order differential operators
  //   The input and output GridFunctionType's are used 
  //   to determine default values. If any of the 
  //   I*input Index'es are != nullIndex, then the corresponding
  //   output Index I* will be returned equal to I*input.
  //   Since this routine do1esn't look at the gridFunction directly,
  //   it avoids the question of non-standard ordering of Coordinate
  //   and Component Index'es
  //
  // Interface: (inputs)
  // inputGFType     GridFunctionType of the input function for a first order
  //                  differential operators
  // outputGFType    desired GridFunctionType of the output differentiated 
  //                  mappedGridFunction
  //	I1input, etc.	if any of these = nullIndex, then the the corresponding
  //			returned Index will be determined by assuming that a
  //                    general first-order derivative is being computed and 
  //                    subtracting the appropriate number of cells or faces
  //                    from each end.
  //
  // Interface: (output)
  //	I1,I2,I3	returned Index'es
  //
  //========================================
{
  MappedGrid & mg = mappedGrid;
  Index I[3], J[3], M[3];
  int lowerAdjust[3] = {0,0,0};
  int upperAdjust[3] = {0,0,0};
  int i;
  int outputDir, inputDir;
  int nD = numberOfDimensions;


  J[0] = I1input;
  J[1] = I2input;
  J[2] = I3input;
  
  // ... M stores Index'es corresponding to the grid dimension 

  getIndex (mg.dimension(), M[0], M[1], M[2]);

  // ... For convenience, define local copies of GridFunctionType's

//  const GridFunctionParameters::GridFunctionType general           = GridFunctionParameters::general; 
//  const GridFunctionParameters::GridFunctionType vertexCentered    = GridFunctionParameters::vertexCentered; 
  const GridFunctionParameters::GridFunctionType cellCentered      = GridFunctionParameters::cellCentered; 
//  const GridFunctionParameters::GridFunctionType faceCenteredAll   = GridFunctionParameters::faceCenteredAll; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis1 = GridFunctionParameters::faceCenteredAxis1; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis2 = GridFunctionParameters::faceCenteredAxis2; 
  const GridFunctionParameters::GridFunctionType faceCenteredAxis3 = GridFunctionParameters::faceCenteredAxis3; 

  // ... Here is the logic for determining how to adjust the Index'es for the different
  // ... input/output GridFunctionType pairs

  //... most of the output directions will be cellCentered

#define VALDIM(val,j) numberOfDimensions>j ? val : 0

  for (i=0; i<numberOfDimensions; i++) lowerAdjust[i] = 1;
  for (i=0; i<numberOfDimensions; i++) upperAdjust[i] = 2;
    
  switch (inputGFType)
  {
    // ====================
  case cellCentered:
    // ====================

    switch (outputGFType)


    {
    case cellCentered:
      //...no changes needed; all directions are cellCentered
      break;

    case faceCenteredAxis1:
      //... the axis1 direction will be faceCentered
      upperAdjust[0] = 1;
      break;

    case faceCenteredAxis2:
      //... the axis2 direction will be faceCentered
      upperAdjust[1] = VALDIM (1,1);
      break;

    case faceCenteredAxis3:
      //... the axis3 direction will be faceCentered
      upperAdjust[2] = VALDIM (1,2);

      break;

    default:
      cout << "MappedGridFiniteVolumeOperators::getDefaultIndex: invalid inputGFType/outputGFType pair" << endl;
      throw "getDefaultIndex: FATAL ERROR";

    }

    break;

    // ... if outputDir != inputDir, then outputDir: 1,1, nonOutputDir: 0,1
    // ... if outputDir == inputDir, then inputDir: 1,1, nonInputDir: 1,2 (unchanged)

    // ====================
  case faceCenteredAxis1:
    // ====================

    // ... faceCentered in and out:
    
    switch (outputGFType)
    {
    case cellCentered:
      //...axis1  can be computed all the way to the ghost cells
      lowerAdjust[0] = 0;
      upperAdjust[0] = 1;
      break;

    case faceCenteredAxis1:
      inputDir = 0;
      outputDir = 0;

      for (i=0; i<nD; i++)
      {
	lowerAdjust[i] = 1;
	upperAdjust[i] = i==inputDir ? 1 : 2; 
      }
      
      break;
      
    case faceCenteredAxis2:
      inputDir = 1;
      outputDir = 0;
      
      for (i=0; i<nD; i++)
      {
	lowerAdjust[i] = 1;
	upperAdjust[i] = i==outputDir ? 1 : 2; 
      }

      
      break;
      
    case faceCenteredAxis3:
      inputDir = 2;
      outputDir = 0;

      for (i=0; i<nD; i++)
      {
	lowerAdjust[i] = 1;
	upperAdjust[i] = i==outputDir ? 1 : 2; 

      }
      break;

    default:
      cout << "MappedGridFiniteVolumeOperators::getDefaultIndex: invalid inputGFType/outputGFType pair" << endl;
      throw "getDefaultIndex: FATAL ERROR";

    }

    break;
    
    // ====================
  case faceCenteredAxis2:
    // ====================
    
    switch (outputGFType)
    {
    case cellCentered:
      // ...axis2 can be computed all the way to the ghost cells
      lowerAdjust[1] = 0;
      upperAdjust[1] = VALDIM (1,1);
      break;

    case faceCenteredAxis1:
      inputDir = 0;
      outputDir = 1;

      for (i=0; i<nD; i++)
      {
	lowerAdjust[i] = 1;
	upperAdjust[i] = i==outputDir ? 1 : 2; 

      }
      break;
      
    case faceCenteredAxis2: //...outputDir==inputDir
      inputDir = 1;
      outputDir = 1;

      for (i=0; i<nD; i++)
      {
	lowerAdjust[i] = 1;
	upperAdjust[i] = i==inputDir ? 1 : 2; 
      }
      
      break;
      
    case faceCenteredAxis3:  //...outputDir!=inputDir
      inputDir = 2;
      outputDir = 1;
      
      for (i=0; i<nD; i++)
      {
	lowerAdjust[i] = 1;
	upperAdjust[i] = i==outputDir ? 1 : 2; 

      }
      
      break;

    default:
      cout << "MappedGridFiniteVolumeOperators::getDefaultIndex: invalid inputGFType/outputGFType pair" << endl;
      throw "getDefaultIndex: FATAL ERROR";

    }

    break;
    
    // ====================
  case faceCenteredAxis3:
    // ====================
    
    switch (outputGFType)
    {
    case cellCentered:
      // ...axis3 can be computed all the way to the ghost cells
      lowerAdjust[2] = 0;
      upperAdjust[2] = numberOfDimensions>2 ? 1: 0;
      break;

    case faceCenteredAxis1: // outputDir != inputDir

      inputDir = 0;
      outputDir = 2;
      
      for (i=0; i<nD; i++)
      {
	lowerAdjust[i] = 1;
	upperAdjust[i] = i==outputDir ? 1 : 2; 

      }
      
      break;
      
    case faceCenteredAxis2: // outputDir != inputDir
      
      inputDir = 1;
      outputDir = 2;
      
      for (i=0; i<nD; i++)
      {
	lowerAdjust[i] = 1;
	upperAdjust[i] = i==outputDir ? 1 : 2; 

      }
	
      break;
      
    case faceCenteredAxis3: // outputDir == inputDir
      
      inputDir = 2;
      outputDir = 2;

      for (i=0; i<nD; i++)
      {
	lowerAdjust[i] = 1;
	upperAdjust[i] = i==inputDir ? 1 : 2; 
      }


      break;
      
    default:
      cout << "MappedGridFiniteVolumeOperators::getDefaultIndex: invalid inputGFType/outputGFType pair" << endl;
      throw "getDefaultIndex: FATAL ERROR";

    } 

    break;
    
    // ====================
  default:
    // ====================
    cout << "MappedGridFiniteVolumeOperators::getDefaultIndex: invalid inputGFType/outputGFType pair" << endl;
    throw "getDefaultIndex: FATAL ERROR";

  }


  //
  // ...Loop over all axes and set the output Index'es.
  // ...If the input Index = nullIndex, then set the output Index = full dimension minus adjustments
  // ...Else, set the output Index = input Index
  //
  int axis;
  ForAllAxes(axis)
  {
    I[axis] = J[axis].length()==0 ? 
      Range(M[axis].getBase()+lowerAdjust[axis], M[axis].getBound()-upperAdjust[axis]) :
      Range(J[axis].getBase()                  , J[axis].getBound());
  }

  I1 = I[0];
  I2 = I[1];
  I3 = I[2];
}

  
    

void MappedGridFiniteVolumeOperators::
getDefaultIndex (
		const REALMappedGridFunction &u,
		const int  c0,
		const int  c1, 
		const int  c2, 
		const int  c3, 
		const int  c4, 
		Index & I1,
		Index & I2, 
		Index & I3,
		const int extra0,	// = 0
		const int extra1,	// = 0
		const int extra2,	// = 0
		const Index & I1From,	// = nullIndex
		const Index & I2From, 	// = nullIndex
		const Index & I3From) 	// = nullIndex
	//========================================
	// Author:		D.L.Brown
	// Date Created:	950601
	// Date Modified:	950601
	//
	// Purpose:
	//   return default Index'es for which values will
	//   be returned for various functions.
	//   extra0, extra1, extra2 are used to DECREASE the
	//   range of the indexes, and so must be negative or zero.
	//   This is for faceCentered variables
	//
	// Interface: (inputs)
	//	u		default Index'es are determined for this REALMappedGridFunction
	//	c0,c1,c2,c3,c4	default Index'es are for component specified by (c0,c1,c2,c3,c4) only
	//	extra0,extra1,extra2
	//			0 or negative integer. The Index range will be decreased by
	//			this number in each direction
	//	I1from, etc.	if any of these = nullIndex, then the the corresponding
	//			returned Index will be the max possible minus the value of 
	//			extra0
	//
	// Interface: (output)
	//	I1,I2,I3	returned Index'es
	//
	// Status and Warnings:
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================

{
  if (debug) getDefaultIndexDisplay.interactivelySetInteractiveDisplay ("getDefaultIndex initialization");
  if (debug) getDefaultIndexDisplay.display (u, "getDefaultIndex: input function");

  int useExtra[3]; useExtra[0] = extra0; useExtra[1] = extra1; useExtra[2] = extra2;
  if (extra1 == defaultValue) useExtra[1] = (numberOfDimensions > 1) ? useExtra[0] : 0;
  if (extra2 == defaultValue) useExtra[2] = (numberOfDimensions > 2) ? useExtra[0] : 0;

	//
	// if numberOfDimensions = 1 or 2, adjust values of useExtra to reasonable values
	//

  useExtra[1] = (numberOfDimensions > 1) ? useExtra[1] : 0;
  useExtra[2] = (numberOfDimensions > 2) ? useExtra[2] : 0;
	
  int i; 
  ForAllAxes(i)
  {
    if (useExtra[i]>0)
    {
      cout << " MappedGridFiniteVolumeOperators::getDefaultIndex: invalid value of extra = " << useExtra[i] << endl;
      cout << " Resetting extra to zero" << endl;
      for (int axis=0; axis<3; axis++)useExtra[axis] = 0;
    }
  }

  Range I[3];
  Index IFrom[3]; IFrom[0] = I1From; IFrom[1] = I2From; IFrom[2] = I3From;

	//
	// now set I[axis]: if IFrom[axis].length is zero, that means that it wasn't 
	// specified, so use the Base and Bound from the input grid function to 
	// determine the Range's; otherwise use the input Ranges.
	//

      int axis;
      ForAllAxes(axis)
      {
	bool isCC = u.getIsCellCentered(axis,c0,c1,c2,c3,c4);
        int faceCentering = (int) u.getFaceCentering();
	
        if (isCC)
        {
          I[axis] = IFrom[axis].length()==0 ? Range (u.getBase (u.positionOfCoordinate(axis))-useExtra[axis], 
						 u.getBound(u.positionOfCoordinate(axis))+useExtra[axis]-1) : 
			      	          Range (IFrom[axis].getBase(), IFrom[axis].getBound());
        } else {

          I[axis] = IFrom[axis].length()==0 ? Range (u.getBase (u.positionOfCoordinate(axis))-useExtra[axis],
						   u.getBound(u.positionOfCoordinate(axis))+useExtra[axis]) : 
			      	          Range (IFrom[axis].getBase(), IFrom[axis].getBound());
      }
      }
  

    if (debug){
      ForAllAxes (axis)
      {
	Index Idisplay = I[axis];
	getDefaultIndexDisplay.display (Idisplay, "getDefaultIndex chose: ");
      }
    }

  I1 = I[0];
  I2 = I[1];
  I3 = I[2];


}

