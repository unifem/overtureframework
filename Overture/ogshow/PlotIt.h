#ifndef PLOT_IT_H
#define PLOT_IT_H

#include "OvertureTypes.h"
#include "GraphicsParameters.h"

#ifdef OV_USE_DOUBLE
  typedef doubleMappedGridFunction realMappedGridFunction;
  typedef doubleGridCollectionFunction realGridCollectionFunction;
#else
  typedef floatMappedGridFunction realMappedGridFunction;
  typedef floatGridCollectionFunction realGridCollectionFunction;
#endif

// forward declarations
class Mapping;
class UnstructuredMapping;
class TrimmedMapping;
class NurbsMapping;
class HyperbolicMapping;
class CompositeSurface;
class MappedGrid;
class AdvancingFront;
class GridCollection;
class ContourSurface;
class DialogData;

class PlotIt
{
public:

static int parallelPlottingOption;  // 0=copy grid functions to proc. 0 for plotting, 1=plot distributed

// Plot a Mapping, pass optional parameters
static void 
plot(GenericGraphicsInterface &gi, Mapping & map, 
     GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
     int dList=0, bool lit=FALSE);

// Plot a MappedGrid
static void 
plot(GenericGraphicsInterface &gi, MappedGrid & mg, 
     GraphicsParameters & parameters=Overture::defaultGraphicsParameters() );

// plot an advancing front
static void 
plot(GenericGraphicsInterface & gi, AdvancingFront & front, 
     GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

// Plot a GridCollection or Composite grid
static void 
plot(GenericGraphicsInterface &gi, GridCollection & cg, 
     GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

// Plot quantities that show the quality of the mapping
static void 
plotMappingQuality(GenericGraphicsInterface &gi, Mapping & map,
                   GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

// Plot quantities that show the quality of the grid
static void 
plotGridQuality(GenericGraphicsInterface &gi, MappedGrid & mg,
                GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

static void 
plotGridQuality( GenericGraphicsInterface & gi, 
                 GridCollection & gc,
		 GraphicsParameters & parameters =Overture::defaultGraphicsParameters());


// Plot 1D functions
static void
plot(GenericGraphicsInterface &gi, 
     const realArray & t, 
     const realArray & x, 
     const aString & title = nullString, 
     const aString & tName       = nullString,
     const aString *xName        = NULL,
     GraphicsParameters & parameters=Overture::defaultGraphicsParameters()  );

// Plot a time sequence of 1D functions to generate a surface
static void
plot(GenericGraphicsInterface &gi, 
     const realArray & x, 
     const realArray & t,
     const realArray & u, 
     GraphicsParameters & parameters = Overture::defaultGraphicsParameters() );


// Show the parallel distribution of a grid.
static int
plotParallelGridDistribution(GridCollection & cg, GenericGraphicsInterface & gi, GraphicsParameters & par );

// Plot contours and/or a shaded surface plot of a realMappedGridFunction in 2D or 3D
static void 
contour(GenericGraphicsInterface &gi, const realMappedGridFunction & u, 
	GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

// Plot contours and/or a shaded surface plot of a GridCollectionFunction/CompositeGridFunction in 2D
static void 
contour(GenericGraphicsInterface &gi, const realGridCollectionFunction & u, 
	GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

static void
streamLines(GenericGraphicsInterface &gi, 
	    const realMappedGridFunction & uv, 
            GraphicsParameters & parameters = Overture::defaultGraphicsParameters() );

static void
streamLines(GenericGraphicsInterface &gi, 
	    const realGridCollectionFunction & uv0, 
	    GraphicsParameters & parameters = Overture::defaultGraphicsParameters() );

// Plot the "displacement" for a solid mechanics problem,  d = x + u, where x are the grid vertices
// and u is the provided solution. 
static void 
displacement(GenericGraphicsInterface &gi, const realGridCollectionFunction & u, 
	     GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

// The next routine is called from contour. A user can write a new version of thei file in order
// to output values to a file in any given format. 
static int
userDefinedOutput(const realGridCollectionFunction & uv, 
                  GraphicsParameters & par,
                  const aString & callingFunctionName);


//  ------------ utility routines -------------

static int 
buildColourDialog(DialogData & dialog);

static bool
getColour( const aString & answer_, DialogData & dialog, aString & colour );


static void
getGridBounds(const GridCollection & gc, GraphicsParameters & params, RealArray & xBound);


// ------------------------------------- protected -----------------------------------
protected:

// Plot a GridCollection or Composite grid
static void 
plotGrid(GenericGraphicsInterface &gi, GridCollection & cg, 
	 GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
         const realGridCollectionFunction *v = NULL );

static void
contour1d(GenericGraphicsInterface &gi, const realGridCollectionFunction & u, 
	  GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

static void
contour2d(GenericGraphicsInterface &gi, const realGridCollectionFunction & u, 
	  GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

static void 
contourOpt2d(GenericGraphicsInterface &gi, 
             const realGridCollectionFunction & uGCF, 
             GraphicsParameters & parameters,
             real & uMin, real & uMax, real & uRaise,
             bool & recomputeVelocityMinMax,
	     bool & contourLevelsSpecified,
             RealArray & xBound );
  
static void
contour3d(GenericGraphicsInterface &gi, const realGridCollectionFunction & u, 
	  GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

static void
contour3dNew(GenericGraphicsInterface &gi, const realGridCollectionFunction & u, 
	  GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

static void
streamLines2d(GenericGraphicsInterface &gi, GridCollection & gc, 
              const realGridCollectionFunction & uv, 
	      GraphicsParameters & parameters);

static void
streamLines3d(GenericGraphicsInterface &gi, GridCollection & gc, 
	      const realGridCollectionFunction & uv, 
	      GraphicsParameters & parameters);

static void 
plotStreamLines(GenericGraphicsInterface &gi, const GridCollection & gc, 
		const realGridCollectionFunction & uv, 
		IntegerArray & componentsToInterpolate,
		IntegerArray & maskForStreamLines_,
		real arrowSize,
                GraphicsParameters & psp,
                real & xa, real &ya, real & xb, real &yb, real &xba, real &yba, 
                real &uMin, real &uMax, int &nrsmx,
                int & nxg, int & nyg, int & intopt );
  
static void
drawAStreamLine(GenericGraphicsInterface &gi, const GridCollection & gc, 
		const realGridCollectionFunction & uv, 
		int *componentsToInterpolate,
		IntegerArray & maskForStreamLines,
		real arrowSize,
                GraphicsParameters & psp,
                real *uip,  int *indexGuessp,
                real & xa, real &ya, real & xb, real &yb, real &xba, real &yba, 
                real &uMin, real &uMax, real &cfl, int &nrsmx,
                int & nxg, int & nyg, real &xtp, real &ytp, int & intopt );
  


static int
drawContourLinesOnAnElement(GenericGraphicsInterface &gi, 
			    real *u,
			    real *x,
			    const int numberOfVerticies,
			    const real deltaU,
			    const real deltaUInverse,
			    const real uMin, 
			    const real uMax,
			    const real uAverage,
			    const real uScaleFactor,
			    const real uRaise,
			    const RealArray & contourLevels,
			    bool & lineStipple,
			    bool contourLevelsSpecified,
			    GraphicsParameters & psp );
  
static void
plotShadedFace(GenericGraphicsInterface &gi, 
	       const MappedGrid & c,
               const intSerialArray & mask,
               const realSerialArray & vertex,
	       const Index & I1, 
	       const Index & I2, 
	       const Index & I3, 
	       const int axis,
	       const int side,
	       GraphicsParameters & parameters );

static void
contourCuts(GenericGraphicsInterface &gi, const realGridCollectionFunction & uGCF, GraphicsParameters & parameters);

static void
getBounds(const realGridCollectionFunction & u,
          real & uMin, 
          real & uMax,
          GraphicsParameters & parameters,   
          const Range & R0=nullRange,      // check these entries of component 0
          const Range & R1=nullRange,      // check these entries of component 1
          const Range & R2=nullRange,
          const Range & R3=nullRange,
          const Range & R4=nullRange);

static void
getPlotBounds(const GridCollection & gc, GraphicsParameters & params, RealArray & xBound);

// Plot the boundaries of a 2D grid
static void
plotGridBoundaries(GenericGraphicsInterface &gi, const GridCollection & cg,
		   IntegerArray & boundaryConditionList, int numberOfBoundaryConditions,
		   const int colourOption=0, 
		   const real zRaise=0., GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

static int 
bcNumber( const int bc, IntegerArray & boundaryConditionList, int numberOfBoundaryConditions );

static void
grid3d(GenericGraphicsInterface &gi, const GridCollection & cg, GraphicsParameters & parameters,
       IntegerArray & boundaryConditionList, 
       int numberOfBoundaryConditions,
       IntegerArray & numberList, int & number, int list, int litList);

static void
surfaceGrid3d(GenericGraphicsInterface &gi, const GridCollection & gc, 
	      GraphicsParameters & psp, 
	      IntegerArray & boundaryConditionList, 
	      int numberOfBoundaryConditions,
	      IntegerArray & numberList, 
	      int & number, int list, int lightList);

static int
plotUnstructured(GenericGraphicsInterface &gi, const UnstructuredMapping & map, GraphicsParameters & par,
		 int dList=0, bool lit=FALSE);

static int
plotAdvancingFront(GenericGraphicsInterface &gi, const AdvancingFront &front,
		   GraphicsParameters & par, const int plotOptions);

// plot an unstructured mapping
static void
plotUM(GenericGraphicsInterface &gi, UnstructuredMapping & map,
       GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
       int dList=0, bool lit=FALSE);

// plot a structured mapping
static void 
plotStructured(GenericGraphicsInterface &gi, Mapping & map, 
	       GraphicsParameters & parameters=Overture::defaultGraphicsParameters(),
	       int dList=0, bool lit=FALSE);

// plot a CompositeSurface
static void 
plotCompositeSurface(GenericGraphicsInterface &gi, CompositeSurface & cs, 
		     GraphicsParameters & params=Overture::defaultGraphicsParameters());

static void
plotSubSurfaceNormals(GenericGraphicsInterface &gi, CompositeSurface & cs, GraphicsParameters & params);

static void 
plotDirectionArrows(GenericGraphicsInterface &gi, HyperbolicMapping & hyp, GraphicsParameters & params);

// plot a trimmed mapping, uses glunurbs unless Mesa is configured
static void
plotTrimmedMapping(GenericGraphicsInterface &gi, TrimmedMapping &map,
		   GraphicsParameters & params=Overture::defaultGraphicsParameters(),
		   int dList =0, bool lit =FALSE);

// plot a nurbs mapping, uses glunurbs unless Mesa is configured
static void 
plotNurbsMapping(GenericGraphicsInterface &gi, NurbsMapping &map,
		 GraphicsParameters & params=Overture::defaultGraphicsParameters(),
		 int dList =0, bool lit =FALSE);

// nurbs rendering calls using gl
static void 
renderTrimmedNurbsMapping(GenericGraphicsInterface &gi, TrimmedMapping &map, 
			  GraphicsParameters & params=Overture::defaultGraphicsParameters(), bool lit=FALSE);

static void
renderNurbsSurface(GenericGraphicsInterface &gi, NurbsMapping &map, float mode);

static void
renderNurbsCurve(GenericGraphicsInterface &gi, NurbsMapping &map, float mode, int type);

static void 
renderTrimmedNurbsByMode(GenericGraphicsInterface &gi, TrimmedMapping &map, float mode);

static void
plotShadedFace(GenericGraphicsInterface &gi, 
	       const RealArray & x, 
	       const Index & I1, 
	       const Index & I2, 
	       const Index & I3, 
	       const int axis,
	       const int side,
	       const int domainDimension,
	       const int rangeDimension,
	       const RealArray & rgb,
	       const intArray & mask = Overture::nullIntegerDistributedArray() );
  
static void
plotLinesOnSurface(GenericGraphicsInterface &gi, 
		   const RealArray & x, 
		   const Index & I1, 
		   const Index & I2, 
		   const Index & I3, 
		   const int axis,
		   const bool offsetLines,
		   const real eps,
		   GraphicsParameters & parameters,
		   const intArray & mask = Overture::nullIntegerDistributedArray() );

static void
plotMappingBoundaries(GenericGraphicsInterface &gi, 
		      const Mapping & mapping,
		      const RealArray & vertex,
		      const int colourOption=1, 
		      const real zRaise=0.,
		      GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

static void
plotMappingEdges(GenericGraphicsInterface &gi, 
		 const RealArray & x, 
		 const IntegerArray & gridIndexRange,
		 GraphicsParameters & parameters,
		 const intArray & mask,
		 int grid =0 );
  
  
static void 
plotGrid2d(GenericGraphicsInterface &gi, GridCollection & gc, 
	   GraphicsParameters & psp,  RealArray & xBound,
           int multigridLevelToPlot, IntegerArray & numberList, int & number );
  
static void 
plot3dContours(GenericGraphicsInterface &gi, 
               const realGridCollectionFunction & uGCF, 
               GraphicsParameters & psp, int list, int lightList,
               bool & plotContours,
               bool & recomputeVelocityMinMax, real & uMin, real & uMax,
               ContourSurface *&cs,
               IntegerArray & numberOfPolygonsPerSurface, 
               IntegerArray & numberOfVerticesPerSurface );

static void 
plot3dContoursNew(GenericGraphicsInterface &gi, 
               const realGridCollectionFunction & uGCF, 
               GraphicsParameters & psp, int list, int lightList,
               bool & plotContours,
               bool & recomputeVelocityMinMax, real & uMin, real & uMax,
               ContourSurface *&cs,
               IntegerArray & numberOfPolygonsPerSurface, 
               IntegerArray & numberOfVerticesPerSurface );

static aString
getGridColour( int item, int side, int axis, 
               int grid, const GridCollection & gc, GenericGraphicsInterface & gi, GraphicsParameters & par );


// adjust the grid to include the displacement 
static int 
adjustGrid( GridCollection & gc, const realGridCollectionFunction & v, GraphicsParameters & parameters,
            realSerialArray *& xSave, real displacementScaleFactor  );

// un-adjust the grid to include the displacement 
static int 
unAdjustGrid( GridCollection & gc, const realGridCollectionFunction & v, GraphicsParameters & parameters,
	      realSerialArray *& xSave, real displacementScaleFactor  );

};
#endif
