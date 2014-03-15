#include "conversion.h"

// NOTES:
//    periodic directions have no ghost points. There is one extra point at index=0 so
//      that the range of valid points goes from [1,n] 


//
//  Create a CompositeGrid from a xcog hdf file.
//
//  The data marked with "*" below need to be filled in.
//
//  The data marked with "@" are filled in by the constructor
//  CompositeGrid::CompositeGrid(numberOfGrids, numberOfDimensions)
//  or by MappedGrid::reference(Mapping&).  You want to change some
//  of these data anyway.
//
//  The data marked with "#" can be filled in by CompositeGrid::update(what);
//
//  The other data can be left as is.
//
//  GenericGrid data:
//  @ Integer&                    computedGeometry();
//
//  MappedGrid data:
//
//  @ Integer&                    numberOfDimensions();
//  @ IntegerArray                boundaryCondition;
//  @ IntegerArray                boundaryDiscretizationWidth;
//  # RealArray                   boundingBox;
//  # RealArray                   gridSpacing;
//  # Logical&                    isAllCellCentered();
//  # Logical&                    isAllVertexCentered();
//  @ LogicalArray                isCellCentered;
//  @ IntegerArray                discretizationWidth;
//  # IntegerArray                indexRange;
//  @ IntegerArray                gridIndexRange;
//  # IntegerArray                dimension;
//  @ IntegerArray                numberOfGhostPoints;
//  @ IntegerArray                isPeriodic;
//  @ IntegerArray                sharedBoundaryFlag;
//  @ RealArray                   sharedBoundaryTolerance;
//  # RealArray                   minimumEdgeLength;
//  # RealArray                   maximumEdgeLength;
//  # Integer                     *I1, *I2, *I3;
//  @ Partitioning_Type           partition;
//  * IntegerMappedGridFunction   mask;
//  # RealMappedGridFunction      inverseVertexDerivative;
//  # RealMappedGridFunction      inverseVertexDerivative2D;
//  # RealMappedGridFunction      inverseVertexDerivative1D;
//  # RealMappedGridFunction      inverseCenterDerivative;
//  # RealMappedGridFunction      inverseCenterDerivative2D;
//  # RealMappedGridFunction      inverseCenterDerivative1D;
//  # RealMappedGridFunction      vertex;
//  # RealMappedGridFunction      vertex2D;
//  # RealMappedGridFunction      vertex1D;
//  # RealMappedGridFunction      center;
//  # RealMappedGridFunction      center2D;
//  # RealMappedGridFunction      center1D;
//  # RealMappedGridFunction      corner;
//  # RealMappedGridFunction      corner2D;
//  # RealMappedGridFunction      corner1D;
//  # RealMappedGridFunction      vertexDerivative;
//  # RealMappedGridFunction      vertexDerivative2D;
//  # RealMappedGridFunction      vertexDerivative1D;
//  # RealMappedGridFunction      centerDerivative;
//  # RealMappedGridFunction      centerDerivative2D;
//  # RealMappedGridFunction      centerDerivative1D;
//  # RealMappedGridFunction      vertexJacobian;
//  # RealMappedGridFunction      centerJacobian;
//  # RealMappedGridFunction      cellVolume;
//  # RealMappedGridFunction      centerNormal;
//  # RealMappedGridFunction      centerNormal2D;
//  # RealMappedGridFunction      centerNormal1D;
//  # RealMappedGridFunction      centerArea;
//  # RealMappedGridFunction      centerArea2D;
//  # RealMappedGridFunction      centerArea1D;
//  # RealMappedGridFunction      faceNormal;
//  # RealMappedGridFunction      faceNormal2D;
//  # RealMappedGridFunction      faceNormal1D;
//  # RealMappedGridFunction      faceArea;
//  # RealMappedGridFunction      faceArea2D;
//  # RealMappedGridFunction      faceArea1D;
//  # RealDistributedArray        vertexBoundaryNormal[3][2];
//  # RealDistributedArray        centerBoundaryNormal[3][2];
//  * MappingRC                   mapping;
//  # Box                         box;
//
//  GenericGridCollection data:
//  @ Integer&                    computedGeometry();
//  @ Integer&                    numberOfGrids();
//  @ ListOfGenericGrid           grid;
//  @ IntegerArray                gridNumber;
//  @ Integer&                    numberOfBaseGrids();
//  @ ListOfGenericGridCollection baseGrid;
//  @ IntegerArray                baseGridNumber;
//  @ Integer&                    numberOfRefinementLevels();
//  @ ListOfGenericGridCollection refinementLevel;
//  @ IntegerArray                refinementLevelNumber;
//  @ Integer&                    numberOfMultigridLevels();
//  @ ListOfGenericGridCollection multigridLevel;
//  @ IntegerArray                multigridLevelNumber;
//  @ Integer&                    numberOfComponentGrids();
//  @ ListOfGenericGridCollection componentGrid;
//  @ IntegerArray                componentGridNumber;
//
//  GridCollection data:
//
//  @ Integer&                    numberOfDimensions();
//  # RealArray                   boundingBox;
//  @ IntegerArray                refinementFactor;
//  @ IntegerArray                multigridCoarseningFactor;
//  @ ListOfMappedGrid            grid;            // (overloaded)
//  @ ListOfGridCollection        baseGrid;        // (overloaded)
//  @ ListOfGridCollection        refinementLevel; // (overloaded)
//  @ ListOfGridCollection        multigridLevel;  // (overloaded)
//  @ ListOfGridCollection        componentGrid;   // (overloaded)
//    AMR_RefinementLevelInfo*    refinementLevelInfo;
//    Interpolant*                interpolant;
//
//  CompositeGrid data:
//
//  * Integer&                     numberOfCompleteMultigridLevels();
//  @ Real&                        epsilon();
//  * IntegerArray                 numberOfInterpolationPoints;
//  # Logical&                     interpolationIsAllExplicit();
//  * LogicalArray                 interpolationIsImplicit;
//  * LogicalArray                 backupInterpolationIsImplicit;
//  * IntegerArray                 interpolationWidth;
//  * IntegerArray                 backupInterpolationWidth;
//  * RealArray                    interpolationOverlap;
//  * RealArray                    backupInterpolationOverlap;
//  * RealArray                    interpolationConditionLimit;
//  * RealArray                    backupInterpolationConditionLimit;
//  * IntegerArray                 interpolationPreference;
//  * LogicalArray                 mayInterpolate;
//  * LogicalArray                 mayBackupInterpolate;
//  * LogicalArray                 mayCutHoles;
//  * IntegerArray                 multigridCoarseningRatio;
//  * IntegerArray                 multigridProlongationWidth;
//  * IntegerArray                 multigridRestrictionWidth;
//  # ListOfRealArray              interpolationCoordinates;
//  * ListOfIntegerArray           interpoleeGrid;
//  # ListOfIntegerArray           interpoleeLocation;
//  * ListOfIntegerArray           interpolationPoint;
//    ListOfRealArray              interpolationCondition;
//    RealCompositeGridFunction    inverseCondition;
//    RealCompositeGridFunction    inverseCoordinates;
//    IntegerCompositeGridFunction inverseGrid;
//
#include "Square.h"
#include "BoxMapping.h"
#include "DataPointMapping.h"
#include "CompositeGrid.h"
#include "HDF_DataBase.h"
#include "hdf_stuff.h"

int checkOverlappingGrid( const CompositeGrid & cg, const int & option=0 );

/* 
I heard that you are interested in extending your getfromAdatabase to
also read xcog generated files. To make it easy I have added a string
variable that I put directly under the root directory in the HDF
file. It looks something like this:  

Vdata `creator' = (creator = xcog version 2.0-beta-6 generated this
file on Fri Jan 31 13:16:43 1997
*/

int 
xCogToOverture(const aString & xCogFileName, CompositeGrid & cg, const bool & checkTheGrid=TRUE) 
// ==============================================================================================
// /Description:
//     Read in an overlapping grid from a file generated by the grid generators Xcog or Chalmesh
//     from Anders Petersson.
// /xCogFileName (input) : name of the Xcog or Chalmesh file.
// /cg (output) : The overlapping grid.
// /checkTheGrid (input) : If true then we check the consistency of the grid.
// ==============================================================================================
{
  char *gridName=(char*)"overlapping grid";

  int32 root = open_hdf_file((char *)((const char *)xCogFileName), 'r');
  if( root <= 0 ) 
  {
    // cout << "xCogToOverture: Unable to open the database file " << xCogFileName << " as an xCog file \n";
    return 1;
  }
  int32 cgDir = locate_dir(gridName, root);
  if( cgDir <= 0 ) 
  {
    // cout << "xCogToOverture: Unable to locate the grid in a potential Xcog/Chalmesh file=" 
    //      << xCogFileName << " as an xCog file \n";
    close_hdf_file(root);
    return 1;
  }

  char *interpolationType;
  int numberOfDimensions, numberOfGrids, numberOfGhostPoints,
    discretizationWidth, normalWidth, tangentialWidth, interpolationWidth,
    periodicOverlap, numberOfInterpolationPoints, axis;

  hget_int(&numberOfGrids,        (char*)"n_components",         cgDir);
  hget_int(&numberOfDimensions,   (char*)"number of dimensions", cgDir);
  hget_int(&numberOfGhostPoints,  (char*)"ghost points",         cgDir);
  hget_int(&discretizationWidth,  (char*)"discretization width", cgDir);
  hget_int(&normalWidth,          (char*)"normal width",         cgDir);
  hget_int(&tangentialWidth,      (char*)"tangent width",        cgDir);
  hget_int(&periodicOverlap,      (char*)"periodic overlap",     cgDir);
  hget_int(&interpolationWidth,   (char*)"interpolation width",  cgDir);
  interpolationType = hget_string((char*)"interpolation type",   cgDir);

  if (tangentialWidth != discretizationWidth) 
  {
    cerr << "WARNING tangential width on boundary and discretization "
	 << "width are not the same. This is assumed by Overture." << endl;
    Vdetach(cgDir); close_hdf_file(root); exit(1);
  } // end if

  // printf(" xCogToOverture: discretizationWidth=%i, interpolationWidth=%i \n",discretizationWidth,
  //	 interpolationWidth);
  
  int actualNumberOfGhostPoints=numberOfGhostPoints;
  numberOfGhostPoints=max(numberOfGhostPoints,1);  // ******* always have at least 1 ghost point *****

//  Create a CompositeGrid with the required number of dimensions and grids.
//  cg.setNumberOfDimensionsAndGrids(numberOfDimensions, numberOfGrids); // this didn't work
  cg = CompositeGrid(numberOfDimensions, numberOfGrids);

//      These defines could be implemented as operator ().
#define periodicity(i)         compute_index_1d(periodicity_,       i)
#define range(i,j)             compute_index_2d(range_,             i,j)
#define boundaryCondition(i,j) compute_index_2d(boundaryCondition_, i,j)
#define surfaceLabel(i,j)      compute_index_2d(surfaceLabel_,      i,j)
#define flag(i,j)              compute_index_2d(flag_,              i,j)
#define x(i,j)                 compute_index_2d(x_,                 i,j)
#define y(i,j)                 compute_index_2d(y_,                 i,j)

#define flag3(i,j,k)              compute_index_3d(flag3_,              i,j,k)
#define x3(i,j,k)                 compute_index_3d(x3_,                 i,j,k)
#define y3(i,j,k)                 compute_index_3d(y3_,                 i,j,k)
#define z3(i,j,k)                 compute_index_3d(z3_,                 i,j,k)

  IntegerArray shift(3,numberOfGrids);  // for shifting to base 0
  shift=0;
  
//  Create the MappedGrids.
  cg.numberOfCompleteMultigridLevels() = 1;
  int k1;
  for (k1=0; k1<numberOfGrids; k1++) 
  {
    char *gridType, *componentGridName;
    int_array_1d *periodicity_;
    int_array_2d *range_, *boundaryCondition_, *surfaceLabel_;
    int_array_2d *flag_;
    double_array_2d *x_, *y_;
    int_array_3d *flag3_;
    double_array_3d *x3_, *y3_, *z3_;
    char _[100]; sPrintF(_, "component grid %i", k1+1);
    int32 gDir = locate_dir(_, cgDir);
    range_              = hget_int_array_2d((char*)"range",              gDir);
    boundaryCondition_  = hget_int_array_2d((char*)"boundary condition", gDir);
    periodicity_        = hget_int_array_1d((char*)"periodicity",        gDir);
    bool cutsHoles=TRUE;

    if( numberOfDimensions==2 )
    {
      surfaceLabel_       = hget_int_array_2d((char*)"curve label",        gDir);
      flag_               = hget_int_array_2d((char*)"flag",               gDir);
      x_                  = hget_double_array_2d((char*)"x",               gDir);
      y_                  = hget_double_array_2d((char*)"y",               gDir);
    }
    else if( numberOfDimensions==3 )
    {
      surfaceLabel_       = hget_int_array_2d((char*)"surface label",      gDir);
      flag3_              = hget_int_array_3d((char*)"flag",               gDir);
      x3_                 = hget_double_array_3d((char*)"x",               gDir);
      y3_                 = hget_double_array_3d((char*)"y",               gDir);
      z3_                 = hget_double_array_3d((char*)"z",               gDir);
    }
    else
    {
      cout << "xCogToOverture::ERROR: numberOfDimensions = " << numberOfDimensions << endl;
      throw "error";
    }
    gridType            = hget_string((char*)"grid type",                gDir);
    componentGridName   = hget_string((char*)"component grid name",      gDir);
    hget_int(&numberOfInterpolationPoints, (char*)"n_interp",            gDir);
    Vdetach(gDir);


    Mapping* mapping;
    Index I[3], &I1=I[0], &I2=I[1], &I3=I[2]; 
    Index J[3], &J1=J[0], &J2=J[1], &J3=J[2];

    if (!strcmp(gridType, "cartesian")) 
    {
//          Create a Cartesian mapping.
      if( numberOfDimensions==2 )
      {
	mapping = new SquareMapping(
	  x(range(1,1),range(1,2)), x(range(2,1),range(1,2)),
	  y(range(1,1),range(1,2)), y(range(1,1),range(2,2)));
      }
      else
      {
	mapping = new BoxMapping(
	  x3(range(1,1),range(1,2),range(1,3)), x3(range(2,1),range(1,2),range(1,3)),
	  y3(range(1,1),range(1,2),range(1,3)), y3(range(1,1),range(2,2),range(1,3)),
	  z3(range(1,1),range(1,2),range(1,3)), z3(range(1,1),range(1,2),range(2,3)));
      }
      for( axis=0; axis<numberOfDimensions; axis++ )
        mapping->setGridDimensions(axis, range(2,axis+1) - range(1,axis+1) + 1);

    } 
    else 
    {
//          Create a mapping from data points.
      realArray vertex;

      int extra = periodicity(axis1+1) ? periodicOverlap-1 : actualNumberOfGhostPoints;
      I1=Range(1,range(2,1)+extra);
      extra = periodicity(axis2+1) ? periodicOverlap-1 : actualNumberOfGhostPoints;
      I2=Range(1,range(2,2)+extra);
      extra = numberOfDimensions==2 ? 0 : (periodicity(axis3+1) ? periodicOverlap-1 : actualNumberOfGhostPoints);
      I3=Range(numberOfDimensions==2 ? Range(0,0) : Range(1,range(2,3)+extra));

      vertex.redim(I1,I2,I3, numberOfDimensions);

      // J1,J2,J3 : will be the size of the xcog arrays, used below in the adopt.
/* ---
      IntegerArray dim(2,3);
      dim=0;
      for(axis=0; axis<numberOfDimensions; axis++) 
      {
        dim(Start,axis)=range(1,axis+1)-actualNumberOfGhostPoints;
	if (periodicity(axis+1))
	  dim(End  ,axis)=range(2,axis+1);  // there is apparently no ghost point here
	else
	  dim(End  ,axis)=range(2,axis+1)+actualNumberOfGhostPoints;
      }
      Range J1(dim(0,0),dim(1,0));
      Range J2(dim(0,1),dim(1,1));
      Range J3(dim(0,2),dim(1,2));
----- */


      if( numberOfDimensions==2 )
      {
        if( FALSE )
	{
	  for (int j=range(1,2); j<=range(2,2); j++)
	    for (int i=range(1,1); i<=range(2,1); i++)
	    {
	      vertex(i,j,0,0) = x(i,j);
	      vertex(i,j,0,1) = y(i,j);
	    } //end for, end for
	}
	else
	{
          J1=Range(1,x_->n1);  // Anders' arrays are base 1
          J2=Range(1,x_->n2);
	  doubleArray xx; xx.adopt(x_->arrayptr,J1,J2,I3);
          J1=Range(1,y_->n1);  // Anders' arrays are base 1
          J2=Range(1,y_->n2);
	  doubleArray yy; yy.adopt(y_->arrayptr,J1,J2,I3);
	

	  equals(vertex(I1,I2,I3,0),xx(I1,I2,I3));  // vertex(I1,I2,I3,0)=xx(I1,I2,I3);
	  equals(vertex(I1,I2,I3,1),yy(I1,I2,I3));  // vertex(I1,I2,I3,1)=yy(I1,I2,I3);
/* ----
	  for (int j=range(1,2); j<=range(1,2)+1; j++)
	    for (int i=range(1,1); i<=range(2,1); i++)
	    {
	      printf("(%i,%i) x=%e, xx=%e, vertex=%e \n",i,j,x(i,j),xx(i,j,0),vertex(i,j,0,0));
	    } //end for, end for
----- */

	}
	
      }
      else
      {
/* ----
	for (int k=range(1,3); k<=range(2,3); k++)
	  for (int j=range(1,2); j<=range(2,2); j++)
	    for (int i=range(1,1); i<=range(2,1); i++)
	    {
	      vertex(i,j,k,0) = x3(i,j,k);
	      vertex(i,j,k,1) = y3(i,j,k);
	      vertex(i,j,k,2) = z3(i,j,k);
	    } //end for, end for
---- */
        J1=Range(1,x3_->n1);  // Anders' arrays are base 1
        J2=Range(1,x3_->n2);
        J3=Range(1,x3_->n3);

        doubleArray xx; xx.adopt(x3_->arrayptr,J1,J2,J3);
        doubleArray yy; yy.adopt(y3_->arrayptr,J1,J2,J3);
        doubleArray zz; zz.adopt(z3_->arrayptr,J1,J2,J3);
	
        equals(vertex(I1,I2,I3,0),xx(I1,I2,I3));  //        vertex(I1,I2,I3,0)=xx(I1,I2,I3);
        equals(vertex(I1,I2,I3,1),yy(I1,I2,I3));  //        vertex(I1,I2,I3,1)=yy(I1,I2,I3);
        equals(vertex(I1,I2,I3,2),zz(I1,I2,I3));  //        vertex(I1,I2,I3,2)=zz(I1,I2,I3);
      }

      DataPointMapping &dpMap = *new DataPointMapping();
      for (axis=0; axis<numberOfDimensions; axis++) 
        if (periodicity(axis+1))
	  dpMap.setIsPeriodic(axis, Mapping::functionPeriodic);
      dpMap.setOrderOfInterpolation( numberOfDimensions==2 ? 4 : 2);
      IntegerArray gridIndexRange(2,3);
      gridIndexRange=0;
      for (axis=0; axis<numberOfDimensions; axis++) 
	for (int side=0; side<2; side++)
	  gridIndexRange(side,axis)=range(side+1,axis+1);
      
      dpMap.setDataPoints(vertex, 3, numberOfDimensions, 0, gridIndexRange);
      mapping = &dpMap;
    } // end if

    mapping->setName(Mapping::mappingName,componentGridName); // set the name
    for (axis=0; axis<numberOfDimensions; axis++) 
    {
      if( periodicity(axis+1) )
      {
	mapping->setIsPeriodic(axis, Mapping::functionPeriodic);
	for (int side=0; side<2; side++)
	  mapping->setBoundaryCondition(side,axis, -1);
      } 
      else 
      {
	for (int side=0; side<2; side++)
	{
	  //printf("boundaryCondition(%i,%i)=%i, surfaceLabel=%i \n",side+1,axis+1,
          //   boundaryCondition(side+1,axis+1),
          //   surfaceLabel(side+1,axis+1));
	  mapping->setBoundaryCondition(side,axis,abs(surfaceLabel(side+1,axis+1)));
	  mapping->setShare(side,axis,abs(surfaceLabel(side+1,axis+1)));

          cutsHoles= cutsHoles && surfaceLabel(side+1,axis+1)>= 0;
	}
      }
    }
    
    MappedGrid& g1 = cg[k1];
    mapping->incrementReferenceCount();
    g1.reference(*mapping);
    if (mapping->decrementReferenceCount() == 0) delete mapping;

    for (axis=0; axis<numberOfDimensions; axis++) 
    {
      g1.discretizationWidth()(axis)   = discretizationWidth;
      g1.isCellCentered()(axis) = LogicalFalse;
      for (int side=0; side<2; side++) 
      {
	g1.setBoundaryDiscretizationWidth(side,axis, normalWidth);

	if( g1.isPeriodic(axis) )
	{
	  g1.setGridIndexRange(side,axis,range(side+1,axis+1)-periodicOverlap);
	  // *wdh* g1.setNumberOfGhostPoints(side,axis,periodicOverlap);
	  g1.setNumberOfGhostPoints(side,axis,numberOfGhostPoints);
	}
	else
	{
	  g1.setGridIndexRange(side,axis, range(side+1,axis+1)-numberOfGhostPoints); 
	  g1.setNumberOfGhostPoints(side,axis,numberOfGhostPoints);
	}
      } // end if
    } // end for

    //      Note:  g1.update() also computes isAllVertexCentered(),
    //      isAllCellCentered(), indexRange, dimension and gridSpacing.
    g1.update(MappedGrid::THEmask, MappedGrid::COMPUTEnothing);
    g1.mask() = 0;
    int is[3] =  {0,0,0};  // shift for setting base to zero
    for (axis=0; axis<numberOfDimensions; axis++)
    {
      is[axis]=range(1,axis+1)-g1.indexRange(0,axis);
      shift(axis,k1)=is[axis];
    }
    
    intArray & mask = g1.mask();
    if( numberOfDimensions==2 )
    {
      
      for (int j=g1.indexRange(0,1); j<=g1.indexRange(1,1); j++)
	for(int i=g1.indexRange(0,0); i<=g1.indexRange(1,0); i++)
	  mask(i,j,0) = flag(i+is[0],j+is[1]) == 0 ? 0 :
	(abs(flag(i+is[0],j+is[1])) - 1) | (flag(i+is[0],j+is[1]) > 0  ?
				MappedGrid::ISdiscretizationPoint :
				MappedGrid::ISinterpolationPoint);
    }
    else
    {
      for (int k=g1.indexRange(0,2); k<=g1.indexRange(1,2); k++)
	for (int j=g1.indexRange(0,1); j<=g1.indexRange(1,1); j++)
	  for(int i=g1.indexRange(0,0); i<=g1.indexRange(1,0); i++)
	    mask(i,j,k) = flag3(i+is[0],j+is[1],k+is[2]) == 0 ? 0 :
	        (abs(flag3(i+is[0],j+is[1],k+is[2])) - 1) | (flag3(i+is[0],j+is[1],k+is[2]) > 0  ?
				   MappedGrid::ISdiscretizationPoint :
				   MappedGrid::ISinterpolationPoint);
    }

#undef boundaryCondition
    // Mark the mask at ghost points.
    for( axis=0; axis<3; axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
        getBoundaryIndex(g1.dimension(),side,axis,I1,I2,I3);
        I[axis]=g1.gridIndexRange(side,axis);
	
        getGhostIndex(g1.dimension(),side,axis,J1,J2,J3);
	const Integer pm1 = 2 * side - 1;
	// if( g1.boundaryCondition(side,axis)!=0 ) // do not change periodic edges, these may be needed for interp.
	if( g1.boundaryCondition(side,axis) > 0 )
	{
	  for (Integer k=g1.dimension(side,axis); k!=g1.extendedIndexRange(side,axis); k-=pm1)
	  { 
	    J[axis] = k; 
	    where( mask(I1,I2,I3) )
	    {
	      mask(J1,J2,J3) = MappedGrid::ISghostPoint;
	    }
            otherwise()
	    {
	      mask(J1,J2,J3) = 0;
	    }
	  }
	}
	else if( g1.boundaryCondition(side,axis)==0 )
	{
	  // set ghost lines outside interpolation edges to zero.
	  for (Integer k=g1.dimension(side,axis); k!=g1.extendedIndexRange(side,axis); k-=pm1)
	  { 
	    J[axis] = k; 
	    mask(J1,J2,J3) = 0;
	  }
	}
      }
    }

    g1.mask().periodicUpdate();
    g1->computedGeometry |= MappedGrid::THEmask;


/* --
    g1.boundaryCondition().display("boundaryCondition");
    g1.dimension().display("dimension");
    g1.gridIndexRange().display("gridIndexRange");
--- */
//    g1.mask().display("mask");
    

//      C-array cleanup.
    delete_int_array_2d(range_);
    delete_int_array_2d(boundaryCondition_);
    delete_int_array_2d(surfaceLabel_);
    delete_int_array_1d(periodicity_);
    if( numberOfDimensions==2 )
    {
      delete_int_array_2d(flag_);
      delete_double_array_2d(x_);
      delete_double_array_2d(y_);
    }
    else 
    {
      delete_int_array_3d(flag3_);
      delete_double_array_3d(x3_);
      delete_double_array_3d(y3_);
      delete_double_array_3d(z3_);
    }
    
    free(gridType);

    cg.numberOfInterpolationPoints(k1) = numberOfInterpolationPoints;

    for (int k2=0; k2<cg.numberOfComponentGrids(); k2++) 
    {
//      if (cg.interpolationIsImplicit(k1,k2,0)       =
//	  !strcmp(interpolationType, "implicit"))
//	cg.backupInterpolationIsImplicit(k1,k2,0)     = LogicalTrue;

      for( axis=0; axis<numberOfDimensions; axis++ )
      {
        cg.interpolationWidth(axis,k1,k2,0)              =interpolationWidth;
	cg.interpolationOverlap(axis,k1,k2,0)            = .5 * amax1(1.,
								   cg.interpolationIsImplicit(k1,k2,0) ?
								   interpolationWidth - 2 :
								   interpolationWidth + discretizationWidth - 3);
//        cg.backupInterpolationOverlap(axis,k1,k2,0)      = -.5;
	cg.multigridCoarseningRatio(axis,k1,0)           = 1;
	cg.multigridProlongationWidth(axis,k1,0)         = 1;
	cg.multigridRestrictionWidth(axis,k1,0)          = 1;
      }
      cg.mayCutHoles(k1,k2)=cutsHoles;
      for( axis=numberOfDimensions; axis<3; axis++ )
      {
        cg.interpolationWidth(axis,k1,k2,0)              =1;
	cg.interpolationOverlap(axis,k1,k2,0)            = -.5;
//        cg.backupInterpolationOverlap(axis,k1,k2,0)      = -.5;
	cg.multigridCoarseningRatio(axis,k1,0)           = 1;
	cg.multigridProlongationWidth(axis,k1,0)         = 1;
	cg.multigridRestrictionWidth(axis,k1,0)          = 1;
      }
      
//      cg.interpolationConditionLimit(k1,k2,0)       = 0.;
//	cg.backupInterpolationConditionLimit(k1,k2,0) = 0.;
      cg.interpolationPreference(k1,k2,0)           = k1;
      cg.mayInterpolate(k1,k2,0)                    = LogicalTrue;
//      cg.mayBackupInterpolate(k1,k2,0)              = LogicalFalse;

    } //end for
  } // end for

//  Allocate and fill in interpolation data.
  cg.update(
    CompositeGrid::THEinterpolationPoint       |
    CompositeGrid::THEinterpoleeGrid           |
    CompositeGrid::THEinterpoleeLocation       |
    CompositeGrid::THEinterpolationCoordinates,
    CompositeGrid::COMPUTEnothing);

  IntegerArray ng(numberOfGrids), gridStart(numberOfGrids);

  cg.interpolationStartEndIndex=-1;  // for optimized interpolation
  
  for (k1=0; k1<numberOfGrids; k1++)
  {
    double_array_2d *donor_parameter_;
    int_array_2d *interpolation_point_, *interpolation_location_;
    char _[100]; sPrintF(_, "component grid %i", k1+1);
    int32 gDir = locate_dir(_, cgDir);
    donor_parameter_       = hget_double_array_2d((char*)"donor parameter",  gDir);
    interpolation_point_   = hget_int_array_2d((char*)"interpolation point", gDir);
    interpolation_location_= hget_int_array_2d((char*)"donor point",         gDir);
    Vdetach(gDir);

//      These defines could be implemented as operator ().
#define donor_parameter(i,j)        compute_index_2d(donor_parameter_,        i,j)
#define interpolation_point(i,j)    compute_index_2d(interpolation_point_,    i,j)
#define interpolation_location(i,j) compute_index_2d(interpolation_location_, i,j)

    cg.variableInterpolationWidth[k1]=interpolationWidth;

    // we need to sort the interpolation points by interpolee grid
    int i, grid2;
    ng=0;
    for( i=0; i<cg.numberOfInterpolationPoints(k1); i++ )
    {
      grid2=interpolation_location(numberOfDimensions+1,i+1) - 1;
      ng(grid2)++;
    }
    gridStart(0)=0;
    for( grid2=1; grid2<numberOfGrids; grid2++ )
      gridStart(grid2)=gridStart(grid2-1)+ng(grid2-1);

    for( grid2=0; grid2<numberOfGrids; grid2++ )
    {

      if( ng(grid2)>0 )
      {
	cg.interpolationStartEndIndex(0,k1,grid2)=gridStart(grid2);              // start value
	cg.interpolationStartEndIndex(1,k1,grid2)=gridStart(grid2)+ng(grid2)-1;  // end value
        cg.interpolationStartEndIndex(2,k1,grid2)= cg.interpolationStartEndIndex(1,k1,grid2);
      }
    }

    for ( i=0; i<cg.numberOfInterpolationPoints(k1); i++) 
    {
      int k2=interpolation_location(numberOfDimensions+1,i+1) - 1;
      int j = gridStart(k2);
      gridStart(k2)++;
      
      cg.interpoleeGrid[k1](j)=k2;
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
        cg.interpolationPoint[k1](j,axis)  = interpolation_point(axis+1,i+1)- shift(axis,k1);
        cg.interpoleeLocation[k1](j,axis)  = interpolation_location(axis+1,i+1)- shift(axis,k2);
        cg.interpolationCoordinates[k1](j,axis) = donor_parameter(axis+1,i+1);
      }
    } // end for

//      C-array cleanup.
    delete_int_array_2d(interpolation_location_);
    delete_int_array_2d(interpolation_point_);
    delete_double_array_2d(donor_parameter_);
  } // end for

//  C-array cleanup.
  free(interpolationType);
  Vdetach(cgDir); close_hdf_file(root);

//  Tell the CompositeGrid that the interpolation data have been computed:
  cg->computedGeometry |=
    CompositeGrid::THEmask                     |
    CompositeGrid::THEinterpolationCoordinates |
    CompositeGrid::THEinterpolationPoint       |
    CompositeGrid::THEinterpoleeLocation       |
    CompositeGrid::THEinterpoleeGrid;

  if( checkTheGrid )
  {
    printf("Checking the consistency of the xCog or Chalmesh grid...\n");
    checkOverlappingGrid(cg);  // do some checks on the grid
    printf("...done\n");
  }
  return 0;
}
