#include "MappedGridFiniteVolumeOperators.h"

bool MappedGridFiniteVolumeOperators::debug = FALSE;
const Index MappedGridFiniteVolumeOperators::nullIndex;	//this creates an Index with nothing in it
const Range MappedGridFiniteVolumeOperators::all;		//also nullIndex; used to default an index to "all"

		// ========================================
		// ========================================
		// CONSTRUCTORS
		// ========================================
		// ========================================

// =================================================================================
MappedGridFiniteVolumeOperators::
MappedGridFiniteVolumeOperators ()
// =================================================================================
{
  setup ();
}

// =================================================================================
MappedGridFiniteVolumeOperators::
MappedGridFiniteVolumeOperators (MappedGrid & mg)
// =================================================================================
{
  setup ();
  updateToMatchGrid (mg);
}

//==============================================================================
MappedGridFiniteVolumeOperators::
MappedGridFiniteVolumeOperators (const MappedGridFiniteVolumeOperators & fvo)
//==============================================================================
{
  setup ();
  //
  // "deep" copy
  //
  isVolumeScaled 		= fvo.isVolumeScaled;
  useCMPGRDGeometryArrays	= fvo.useCMPGRDGeometryArrays;
  useInternalGeometryArrays	= fvo.useInternalGeometryArrays;
  cellVolume     		= fvo.cellVolume;
  faceNormal 			= fvo.faceNormal;
  centerNormal			= fvo.centerNormal;
  faceArea			= fvo.faceArea;
  faceNormalCG			= fvo.faceNormalCG;
  centerNormalCG		= fvo.centerNormalCG;
  defaultPositionOfComponent    = fvo.defaultPositionOfComponent;

  cellVolumeDefined     = fvo.cellVolumeDefined;
  faceNormalDefined     = fvo.faceNormalDefined;
  centerNormalDefined   = fvo.centerNormalDefined;
  faceNormalCGDefined     = fvo.faceNormalCGDefined;
  centerNormalCGDefined   = fvo.centerNormalCGDefined;
  faceAreaDefined       = fvo.faceAreaDefined;
  vertexJacobianDefined = fvo.vertexJacobianDefined;

  numberOfDimensions    = fvo.numberOfDimensions;

  mappedGrid.reference (fvo.mappedGrid);			// is this right?
}



//==============================================================================
GenericMappedGridOperators* MappedGridFiniteVolumeOperators::
virtualConstructor () const
//==============================================================================
{
  return new MappedGridFiniteVolumeOperators();
}


// =================================================================================
MappedGridFiniteVolumeOperators::
~MappedGridFiniteVolumeOperators ()
// =================================================================================
{
  cout << "MappedGridFiniteVolumeOperators destructor (:(:" << endl;
  cleanup ();
}

// =================================================================================
void MappedGridFiniteVolumeOperators::
setup ()
// =================================================================================
{

  orderOfAccuracy = 2;
  
  stencilSize = 0; 
  numberOfComponentsForCoefficients = 1;
  
  isVolumeScaled = FALSE;
  useCMPGRDGeometryArrays = TRUE;
  useInternalGeometryArrays = !useCMPGRDGeometryArrays;
  defaultPositionOfComponent = 3;

  cellVolumeDefined     = FALSE;
  faceNormalDefined     = FALSE;
  centerNormalDefined   = FALSE;
  faceAreaDefined       = FALSE;
  vertexJacobianDefined = FALSE;

  numberOfComponents 			= 10;
  maximumNumberOfBoundaryConditions 	= 10;

  // ... these are grid dependent and will be set in updateToMatchGrid
  width = 0;
  halfWidth1 = 0;
  halfWidth2 = 0;
  halfWidth3 = 0;
  

  twilightZoneFlow 			= FALSE;
  twilightZoneFlowFunction		= NULL;

  Range maxBC(0,maximumNumberOfBoundaryConditions-1);
  Range numCP(0,numberOfComponents-1);

  numberOfBoundaryConditions.redim(2,3);
  numberOfBoundaryConditions = 0;
  
  boundaryCondition.redim(2,3,maxBC);
  boundaryCondition = -1;

  //componentForBoundaryCondition.redim(2,3,maximumNumberOfBoundaryConditions,numberOfComponents);
  componentForBoundaryCondition.redim(2,3,maxBC,numCP);
  componentForBoundaryCondition = -1;

  //boundaryConditionValueGiven.redim(2,3,maximumNumberOfBoundaryConditions,numberOfComponents);
  boundaryConditionValueGiven.redim(2,3,maxBC,numCP);
  boundaryConditionValueGiven=FALSE;
  boundaryConditionValue.redim(2,3,maxBC,numCP);
  boundaryConditionValue = 0.;

  boundaryData = FALSE;

}

// =================================================================================
void MappedGridFiniteVolumeOperators::
updateToMatchGrid( MappedGrid & mg)
// =================================================================================
{
  mappedGrid.reference(mg);

  mappedGrid.update (
    MappedGrid::THEcenter
    | MappedGrid::THEfaceNormal
    | MappedGrid::THEcellVolume
    | MappedGrid::THEcenterNormal
    | MappedGrid::THEfaceArea
    | MappedGrid::THEcenterBoundaryNormal
    ,
    MappedGrid::COMPUTEgeometryAsNeeded
    | MappedGrid::USEdifferenceApproximation
	    );

  cellVolumeDefined    = TRUE;
  faceNormalDefined    = TRUE;
  centerNormalDefined  = TRUE;

  if (debug) 
  {
    debugDisplay.display (mappedGrid.center(),       "MappedGridFiniteVolumeOperators::updateToMatchGrid: center array:");
    debugDisplay.display (mappedGrid.faceNormal(),   "MappedGridFiniteVolumeOperators::updateToMatchGrid: faceNormal");
    debugDisplay.display (mappedGrid.cellVolume(),   "MappedGridFiniteVolumeOperators::updateToMatchGrid: cellVolume");
    debugDisplay.display (mappedGrid.centerNormal(), "MappedGridFiniteVolumeOperators::updateToMatchGrid: centerNormal");
  }
  
  numberOfDimensions = mappedGrid.numberOfDimensions();

  //
  // ... stencil size info
  //
  stencilSize = ( numberOfDimensions == 2) ? 10 : 28;

  width = orderOfAccuracy+1;
  halfWidth1=width/2;
  halfWidth2 = numberOfDimensions>1 ? halfWidth1 : 0;
  halfWidth3 = numberOfDimensions>2 ? halfWidth1 : 0;

  // ========================================
  // For now, create all the extra geometry variables
  // eventually these should be CMPGRD options
  // ========================================

  aString yes = "y";
  bool usingCMPGRDArraysOnly = TRUE;
  
//  cout << "MappedGridFiniteVolumeOperators constructor: use CMPGRD arrays?";
//  cin >> yes;

  if (usingCMPGRDArraysOnly) 
  {
    faceNormalCG.reference 	(mappedGrid.faceNormal());
//960325: add this
    faceNormal.reference        (mappedGrid.faceNormal());
    cellVolume.reference 	(mappedGrid.cellVolume());
    centerNormalCG.reference 	(mappedGrid.centerNormal());
    centerNormal.reference      (mappedGrid.centerNormal());
    faceArea.reference 		(mappedGrid.faceArea());

    if (debug) 
    {
      
      debugDisplay.display (faceNormal, "faceNormal array");
      debugDisplay.display (cellVolume,   "cellVolume array");
      debugDisplay.display (centerNormal, "centerNormal array");
      debugDisplay.display (faceArea,     "faceArea array");
    }
    

//    createFaceNormal            (mappedGrid);
//    createCenterNormal          (mappedGrid);
  } 
  else 
  {
    createFaceNormal (mappedGrid);
    createFaceNormalCG (mappedGrid);
    createCellVolume (mappedGrid);
    createCenterNormal (mappedGrid);
    createCenterNormalCG (mappedGrid);
    createFaceArea   (mappedGrid);
  }


  // ======================================== 
  // For convenience, we add scalar links to 
  // the faceNormal array
  // ======================================== 

  if (!usingCMPGRDArraysOnly)
  {
    
    Rx.link (faceNormal, Range(rAxis,rAxis), Range(xAxis,xAxis));
    Ry.link (faceNormal, Range(rAxis,rAxis), Range(yAxis,yAxis));
    Sx.link (faceNormal, Range(sAxis,sAxis), Range(xAxis,xAxis));
    Sy.link (faceNormal, Range(sAxis,sAxis), Range(yAxis,yAxis));
  }
  
  rX.link (faceNormalCG, Range(xAxis,xAxis), Range(rAxis,rAxis));
  rY.link (faceNormalCG, Range(yAxis,yAxis), Range(rAxis,rAxis));

  sX.link (faceNormalCG, Range(xAxis,xAxis), Range(sAxis,sAxis));
  sY.link (faceNormalCG, Range(yAxis,yAxis), Range(sAxis,sAxis));

  rXCenter.link (mappedGrid.centerNormal(), Range(xAxis,xAxis), Range(rAxis,rAxis));
  rYCenter.link (mappedGrid.centerNormal(), Range(yAxis,yAxis), Range(rAxis,rAxis));
  
  sXCenter.link (mappedGrid.centerNormal(), Range(xAxis,xAxis), Range(sAxis,sAxis));
  sYCenter.link (mappedGrid.centerNormal(), Range(yAxis,yAxis), Range(sAxis,sAxis));
 
  if (numberOfDimensions == 3) {

    if (!usingCMPGRDArraysOnly) 
    {
      Tx.link (faceNormal, Range(tAxis,tAxis), Range(xAxis,xAxis));
      Ty.link (faceNormal, Range(tAxis,tAxis), Range(yAxis,yAxis));
      Tz.link (faceNormal, Range(tAxis,tAxis), Range(zAxis,zAxis));
      
      Rz.link (faceNormal, Range(rAxis,rAxis), Range(zAxis,zAxis));
      Sz.link (faceNormal, Range(sAxis,sAxis), Range(zAxis,zAxis));
    }
    
    rZ.link (faceNormalCG, Range(zAxis,zAxis), Range(rAxis,rAxis));
    sZ.link (faceNormalCG, Range(zAxis,zAxis), Range(sAxis,sAxis));
 

    tX.link (faceNormalCG, Range(xAxis,xAxis), Range(tAxis,tAxis));
    tY.link (faceNormalCG, Range(yAxis,yAxis), Range(tAxis,tAxis));
    tZ.link (faceNormalCG, Range(zAxis,zAxis), Range(tAxis,tAxis));

    rZCenter.link (mappedGrid.centerNormal(), Range(zAxis,zAxis), Range(rAxis,rAxis));
    sZCenter.link (mappedGrid.centerNormal(), Range(zAxis,zAxis), Range(sAxis,sAxis));
    
    tXCenter.link (mappedGrid.centerNormal(), Range(xAxis,xAxis), Range(tAxis,tAxis));
    tYCenter.link (mappedGrid.centerNormal(), Range(yAxis,yAxis), Range(tAxis,tAxis));
    tZCenter.link (mappedGrid.centerNormal(), Range(zAxis,zAxis), Range(tAxis,tAxis));
  }
}
// =================================================================================
void MappedGridFiniteVolumeOperators::
cleanup()
// =================================================================================
{
}

// =================================================================================
MappedGridFiniteVolumeOperators& MappedGridFiniteVolumeOperators::
operator= (const MappedGridFiniteVolumeOperators &fvo)
// =================================================================================
{ 
  //
  // "deep" copy
  //
  isVolumeScaled 		= fvo.isVolumeScaled;
  useCMPGRDGeometryArrays	= fvo.useCMPGRDGeometryArrays;
  useInternalGeometryArrays	= fvo.useInternalGeometryArrays;
  cellVolume     		= fvo.cellVolume;
  faceNormal 			= fvo.faceNormal;
  centerNormal			= fvo.centerNormal;
  faceArea			= fvo.faceArea;
  faceNormalCG			= fvo.faceNormalCG;
  centerNormalCG		= fvo.centerNormalCG;
  defaultPositionOfComponent    = fvo.defaultPositionOfComponent;

  cellVolumeDefined     = fvo.cellVolumeDefined;
  faceNormalDefined     = fvo.faceNormalDefined;
  centerNormalDefined   = fvo.centerNormalDefined;
  faceNormalCGDefined     = fvo.faceNormalCGDefined;
  centerNormalCGDefined   = fvo.centerNormalCGDefined;
  faceAreaDefined       = fvo.faceAreaDefined;
  vertexJacobianDefined = fvo.vertexJacobianDefined;

  numberOfDimensions    = fvo.numberOfDimensions;

  mappedGrid.reference (fvo.mappedGrid);			// is this right?

  return *this;
}




		// ========================================
		// ========================================
		// PUBLIC CLASS FUNCTIONS
		// ========================================
		// ========================================



// =================================================================================
void MappedGridFiniteVolumeOperators::
setIsVolumeScaled (
		const bool trueOrFalse)
// =================================================================================

	// set isVolumeScaled according to argument
{
  isVolumeScaled = trueOrFalse;
}

// =================================================================================
bool MappedGridFiniteVolumeOperators::
getIsVolumeScaled ()
// =================================================================================
{
	// return the value of isVolumeScaled

  return (isVolumeScaled);
}

// =================================================================================
void MappedGridFiniteVolumeOperators::
setUseCMPGRDGeometryArrays (
		const bool trueOrFalse)
// =================================================================================

	// set useCMPGRDGeometryArrays according to argument
{

  useCMPGRDGeometryArrays = trueOrFalse;
  useInternalGeometryArrays = !trueOrFalse;
//  cout << "setting MappedGridFiniteVolumeOperators::useCMPGRDGeometryArrays = " << useCMPGRDGeometryArrays << endl;
}

// =================================================================================
bool MappedGridFiniteVolumeOperators::
getUseCMPGRDGeometryArrays ()
// =================================================================================
{
	// return the value of useCMPGRDGeometryArrays

  return (useCMPGRDGeometryArrays);
}

// ================================================================================
bool MappedGridFiniteVolumeOperators::
isInStandardOrdering (const REALMappedGridFunction & u)

	//========================================
	// Author:		D.L.Brown
	// Date Created:	950602
	// Date Modified:	950602
	//
	// Purpose:
	//  determine whether the components are ordered in the standard
	//  way (i.e. 3 Coordinate axes followed by 5 Component axes) or
	//  not. Return TRUE if they are, FALSE if not
	//
	// Interface: (inputs)
	//
	// Interface: (output)
	//
	// Status and Warnings:
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================
{
  bool returnedValue = TRUE;
  for (int i=0; i<3; i++) if (u.positionOfCoordinate(i) != i) returnedValue = FALSE;
  return (returnedValue);
}
// ================================================================================

void MappedGridFiniteVolumeOperators::
standardOrderingErrorMessage (const REALMappedGridFunction &u, const aString & routineName)

	//========================================
	// Author:		D.L.Brown
	// Date Created:	950602
	// Date Modified:	950602
	//
	// Purpose:
	//  If REALMappedGridFunction u is not in standard ordering,
	//  return an error message and quit.
	//
	// Interface: (inputs)
	//
	// Interface: (output)
	//
	// Status and Warnings:
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================
{
  if (!isInStandardOrdering (u)){
    printf ("%s : ERROR: input array not in standard ordering \n", (const char*) routineName);
    exit (-1);
  }
}



