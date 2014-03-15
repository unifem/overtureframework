//
//  Create a CompositeGrid hdf file from xcog hdf file.
//
//  Usage:  xcogToCG xcogFile OvertureFile
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
#include "DataPointMapping.h"
#include "CompositeGrid.h"
#include "HDF_DataBase.h"
#include "hdf_stuff.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage:  " << argv[0] << " xcogFile OvertureFile" << endl;
        exit(1);
    } // end if

    char *fileName=argv[1], *newFileName=argv[2], *gridName="overlapping grid";
    int32 root = open_hdf_file(fileName, 'r');
    if (root <= 0) {
        cerr << "Unable to open the database file " << fileName << endl;
        exit(1);
    } // end if
    int32 cgDir = locate_dir(gridName, root);

    char *interpolationType;
    int numberOfDimensions, numberOfGrids, numberOfGhostPoints,
      discretizationWidth, normalWidth, tangentialWidth, interpolationWidth,
      periodicOverlap, numberOfInterpolationPoints;

    hget_int(&numberOfGrids,        "n_components",         cgDir);
    hget_int(&numberOfDimensions,   "number of dimensions", cgDir);
    hget_int(&numberOfGhostPoints,  "ghost points",         cgDir);
    hget_int(&discretizationWidth,  "discretization width", cgDir);
    hget_int(&normalWidth,          "normal width",         cgDir);
    hget_int(&tangentialWidth,      "tangent width",        cgDir);
    hget_int(&periodicOverlap,      "periodic overlap",     cgDir);
    hget_int(&interpolationWidth,   "interpolation width",  cgDir);
    interpolationType = hget_string("interpolation type",   cgDir);

    if (tangentialWidth != discretizationWidth) {
        cerr << "WARNING tangential width on boundary and discretization "
             << "width are not the same. This is assumed by Overture." << endl;
        Vdetach(cgDir); close_hdf_file(root); exit(1);
    } // end if

//  Create a CompositeGrid with the required number of dimensions and grids.
    CompositeGrid cg(numberOfDimensions, numberOfGrids);

//  Create the MappedGrids.
    cg.numberOfCompleteMultigridLevels() = 1;
    for (int k1=0; k1<numberOfGrids; k1++) {
        char *gridType;
        int_array_1d *periodicity_;
        int_array_2d *range_, *boundaryCondition_, *flag_;
        double_array_2d *x_, *y_;
        char _[100]; sPrintF(_, "component grid %i", k1+1);
        int32 gDir = locate_dir(_, cgDir);
        range_              = hget_int_array_2d("range",              gDir);
        boundaryCondition_  = hget_int_array_2d("boundary condition", gDir);
        periodicity_        = hget_int_array_1d("periodicity",        gDir);
        flag_               = hget_int_array_2d("flag",               gDir);
        x_                  = hget_double_array_2d("x",               gDir);
        y_                  = hget_double_array_2d("y",               gDir);
        gridType            = hget_string("grid type",                gDir);
        hget_int(&numberOfInterpolationPoints, "n_interp",            gDir);
        Vdetach(gDir);

//      These defines could be implemented as operator ().
#define periodicity(i)         compute_index_1d(periodicity_,       i)
#define range(i,j)             compute_index_2d(range_,             i,j)
#define boundaryCondition(i,j) compute_index_2d(boundaryCondition_, i,j)
#define flag(i,j)              compute_index_2d(flag_,              i,j)
#define x(i,j)                 compute_index_2d(x_,                 i,j)
#define y(i,j)                 compute_index_2d(y_,                 i,j)

        Mapping* mapping;
        if (!strcmp(gridType, "cartesian")) {
//          Create a Cartesian mapping.
            mapping = new SquareMapping(
              x(range(1,1),range(1,2)), x(range(2,1),range(1,2)),
              y(range(1,1),range(1,2)), y(range(1,1),range(2,2)));
            mapping->setGridDimensions(0, range(2,1) - range(1,1) + 1);
            mapping->setGridDimensions(1, range(2,2) - range(1,2) + 1);

        } else {
//          Create a mapping from data points.
            realArray vertex(
              Range(range(1,1),range(2,1)),
              Range(range(1,2),range(2,2)),
              1, numberOfDimensions);
            for (int j=range(1,2); j<=range(2,2); j++)
              for (int i=range(1,1); i<=range(2,1); i++) {
                vertex(i,j,0,0) = x(i,j);
                vertex(i,j,0,1) = y(i,j);
            } //end for, end for

            DataPointMapping &dpMap = *new DataPointMapping();
            for (int kd=0; kd<numberOfDimensions; kd++) if (periodicity(kd+1))
              dpMap.setIsPeriodic(kd, Mapping::functionPeriodic);
            dpMap.setOrderOfInterpolation(4);
            dpMap.setDataPoints(vertex, 3, numberOfDimensions);
            mapping = &dpMap;
        } // end if

        for (int kd=0; kd<numberOfDimensions; kd++) if (periodicity(kd+1)) {
            mapping->setIsPeriodic(kd, Mapping::functionPeriodic);
            for (int ks=0; ks<2; ks++)
              mapping->setBoundaryCondition(ks,kd, -1);
        } else {
            for (int ks=0; ks<2; ks++)
              mapping->setBoundaryCondition(ks,kd,
                boundaryCondition(ks+1,kd+1) < 0  ? 51 :
                boundaryCondition(ks+1,kd+1) > 50 ? 50 :
                boundaryCondition(ks+1,kd+1));
        } // end if, end for

        MappedGrid& g1 = cg[k1];
        mapping->incrementReferenceCount();
        g1.reference(*mapping);
        if (mapping->decrementReferenceCount() == 0) delete mapping;

        for (kd=0; kd<numberOfDimensions; kd++) {
            g1.discretizationWidth(kd)   = discretizationWidth;
            g1.numberOfGhostPoints(0,kd) = g1.isPeriodic(kd) ?
              periodicOverlap : numberOfGhostPoints;
            g1.numberOfGhostPoints(1,kd) = g1.isPeriodic(kd) ?
              periodicOverlap - 1 : numberOfGhostPoints;
            g1.isCellCentered(kd) = LogicalFalse;
            for (int ks=0; ks<2; ks++) {
                g1.boundaryDiscretizationWidth(ks,kd) = normalWidth;
                g1.gridIndexRange(ks,kd) = range(ks+1,kd+1);
            } // end if
        } // end for

//      Note:  g1.update() also computes isAllVertexCentered(),
//      isAllCellCentered(), indexRange, dimension and gridSpacing.
        g1.update(MappedGrid::THEmask, MappedGrid::COMPUTEnothing);
        g1.mask = 0;
        for (int j=g1.indexRange(0,1); j<=g1.indexRange(1,1); j++)
          for(int i=g1.indexRange(0,0); i<=g1.indexRange(1,0); i++)
            g1.mask(i,j,0) = flag(i,j) == 0 ? 0 :
              (abs(flag(i,j)) - 1) | (flag(i,j) > 0  ?
              MappedGrid::ISdiscretizationPoint :
              MappedGrid::ISinterpolationPoint);
        g1.mask.periodicUpdate();
        g1.computedGeometry() |= MappedGrid::THEmask;

//      C-array cleanup.
        delete_int_array_2d(range_);
        delete_int_array_2d(boundaryCondition_);
        delete_int_array_1d(periodicity_);
        delete_int_array_2d(flag_);
        delete_double_array_2d(x_);
        delete_double_array_2d(y_);
        free(gridType);

        cg.numberOfInterpolationPoints(k1) = numberOfInterpolationPoints;
        for (int k2=0; k2<cg.numberOfComponentGrids(); k2++) {
            if (cg.interpolationIsImplicit(k1,k2,0)       =
              !strcmp(interpolationType, "implicit"))
            cg.backupInterpolationIsImplicit(k1,k2,0)     = LogicalTrue;
            cg.interpolationWidth(0,k1,k2,0)              =
            cg.interpolationWidth(1,k1,k2,0)              = interpolationWidth;
            cg.interpolationWidth(2,k1,k2,0)              =
            cg.backupInterpolationWidth(0,k1,k2,0)        =
            cg.backupInterpolationWidth(1,k1,k2,0)        =
            cg.backupInterpolationWidth(2,k1,k2,0)        = 1;
            cg.interpolationOverlap(0,k1,k2,0)            =
            cg.interpolationOverlap(1,k1,k2,0)            = .5 * amax1(1.,
              cg.interpolationIsImplicit(k1,k2,0) ?
              interpolationWidth - 2 :
              interpolationWidth + discretizationWidth - 3);
            cg.interpolationOverlap(2,k1,k2,0)            =
            cg.backupInterpolationOverlap(0,k1,k2,0)      =
            cg.backupInterpolationOverlap(1,k1,k2,0)      =
            cg.backupInterpolationOverlap(2,k1,k2,0)      = -.5;
            cg.interpolationConditionLimit(k1,k2,0)       =
            cg.backupInterpolationConditionLimit(k1,k2,0) = 0.;
            cg.interpolationPreference(k1,k2,0)           = k1;
            cg.mayInterpolate(k1,k2,0)                    = LogicalTrue;
            cg.mayBackupInterpolate(k1,k2,0)              = LogicalFalse;
            cg.mayCutHoles(k1,k2)                         = LogicalTrue;
            cg.multigridCoarseningRatio(0,k1,0)           =
            cg.multigridCoarseningRatio(1,k1,0)           =
            cg.multigridCoarseningRatio(2,k1,0)           =
            cg.multigridProlongationWidth(0,k1,0)         =
            cg.multigridProlongationWidth(1,k1,0)         =
            cg.multigridProlongationWidth(2,k1,0)         =
            cg.multigridRestrictionWidth(0,k1,0)          =
            cg.multigridRestrictionWidth(1,k1,0)          =
            cg.multigridRestrictionWidth(2,k1,0)          = 1;
        } //end for
    } // end for

//  Allocate and fill in interpolation data.
    cg.update(
      CompositeGrid::THEinterpolationPoint       |
      CompositeGrid::THEinterpoleeGrid           |
      CompositeGrid::THEinterpoleeLocation       |
      CompositeGrid::THEinterpolationCoordinates,
      CompositeGrid::COMPUTEnothing);
    for (k1=0; k1<numberOfGrids; k1++) {
        double_array_2d *donor_parameter_;
        int_array_2d *interpolation_point_, *interpolation_location_;
        char _[100]; sPrintF(_, "component grid %i", k1+1);
        int32 gDir = locate_dir(_, cgDir);
        donor_parameter_       = hget_double_array_2d("donor parameter",  gDir);
        interpolation_point_   = hget_int_array_2d("interpolation point", gDir);
        interpolation_location_= hget_int_array_2d("donor point",         gDir);
        Vdetach(gDir);

//      These defines could be implemented as operator ().
#define donor_parameter(i,j)        compute_index_2d(donor_parameter_,        i,j)
#define interpolation_point(i,j)    compute_index_2d(interpolation_point_,    i,j)
#define interpolation_location(i,j) compute_index_2d(interpolation_location_, i,j)

        for (int i=0; i<cg.numberOfInterpolationPoints(k1); i++) {
            cg.interpolationPoint[k1](i,0)  = interpolation_point(1,i+1);
            cg.interpolationPoint[k1](i,1)  = interpolation_point(2,i+1);
            cg.interpoleeGrid[k1](i)        = interpolation_location(3,i+1) - 1;
            cg.interpoleeLocation[k1](i,0)  = interpolation_location(1,i+1);
            cg.interpoleeLocation[k1](i,1)  = interpolation_location(2,i+1);
            cg.interpolationCoordinates[k1](i,0) = donor_parameter(1,i+1);
            cg.interpolationCoordinates[k1](i,1) = donor_parameter(2,i+1);
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
    cg.computedGeometry() |=
      CompositeGrid::THEmask                     |
      CompositeGrid::THEinterpolationCoordinates |
      CompositeGrid::THEinterpolationPoint       |
      CompositeGrid::THEinterpoleeLocation       |
      CompositeGrid::THEinterpoleeGrid;

//  Recompute interpolation coordinates and check interpolation stencils.
//  This should not really be necessary, but it is a good debugging tool.
    if (cg.update(
      CompositeGrid::THEinterpolationCoordinates |
      CompositeGrid::THEinterpoleeLocation,
      CompositeGrid::COMPUTEgeometry) &
      CompositeGrid::COMPUTEfailed)
      cerr << "Warning:  Update interpolation failed!" << endl;

//  Write the CompositeGrid out to a data file.
    HDF_DataBase dataFile; dataFile.mount(newFileName, "I");
//  Destroy all big geometry arrays except for the mask.  This saves space.
    cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask);
    cg.put(dataFile, gridName);
    dataFile.unmount();
}
