#ifndef DATA_FORMATS_H
#define DATA_FORMATS_H

// The DataFormats class handles IO and conversion between different
// data formats such as Plot3d, Ingrid, ply, ...

#include "Mapping.h"
#include "MappingInformation.h"

// Forward declarations:
class DataPointMapping;
class UnstructuredMapping;
class GenericGraphicsInterface;
class MappedGrid;
class CompositeGrid;
class floatCompositeGridFunction;
class doubleCompositeGridFunction;
#ifdef OV_USE_DOUBLE
  typedef doubleCompositeGridFunction realCompositeGridFunction;
#else
  typedef floatCompositeGridFunction realCompositeGridFunction;
#endif

// =================================================================================================================
/// \brief Define access functions to read/write different data formats such as PLOT3D, INGRID, Cart3dTri, STL, PLY. 
// =================================================================================================================
class DataFormats
{
public:

enum DataFormatsEnum
{
  Plot3d,
  IGES,
  Ingrid,
  Ply
};


//    DataFormats(GenericGraphicsInterface *ggiPointer=NULL );
//    virtual ~DataFormats();
  
//    virtual int setGraphicsInterface( GenericGraphicsInterface *ggiPointer=NULL ); 
  
// read in a single grid from a file, optionally read a mask (iblank) array
static int readPlot3d(DataPointMapping & dpm,
		      int gridToRead=-1,  
		      const aString & gridFileName=nullString,
		      intArray *maskPointer=NULL,
                      const bool expectIblank=false );

// read in all grids from a file.
static int readPlot3d(MappingInformation & mapInfo, 
		      const aString & gridFileName /*=nullString*/,
		      intArray *&maskPointer,
                      const bool expectIblank=false );

// read in a solution array and parameters from a file
static int readPlot3d(realArray & q, RealArray & par, 
		      const aString & qFileName=nullString);

// read in all solutions and parameters from a file
static int readPlot3d(realArray *& q, RealArray & par, 
		      const aString & qFileName=nullString);

// read in an unstructured Mapping from an Ingrid file
static int readIngrid(UnstructuredMapping &map,
		      const aString & gridFileName=nullString);

static int readTecplot(ListOfMappingRC &mList, const aString &gridFileName =  nullString);

static int readCart3dTri(ListOfMappingRC &mList, const aString &gridFileName = nullString);
  
#if 0
// read in an unstructured mesh into a mapped grid 
static int readIngrid(MappedGrid &mg,
		      const aString & gridFileName=nullString);
#endif
  
// read in an unstructured Mapping from a Ply polygonal file (Stanford/graphics)
static int readPly(UnstructuredMapping &map, 
		   const aString &gridFileName =nullString);

// read in an unstructured Mapping from an STL file
static int readSTL(UnstructuredMapping &map, 
		   const aString & stlFileName =nullString);

// save a Mapping in plot3d format
static int writePlot3d(Mapping & map,
		       const aString & gridFileName=nullString );
  
// save a MappedGrid in plot3d format
static int writePlot3d(MappedGrid & mg,
		       const aString & gridFileName=nullString );
  
// save a CompositeGrid in plot3d format
static int writePlot3d(CompositeGrid & cg,
		       const aString & gridFileName=nullString,
		       const aString & interpolationDataFileName=nullString );

// save a realCompositeGridFunction in a plot3d "q" file.
static int writePlot3d(realCompositeGridFunction & u,
		       const RealArray & par );

// write a Mapping to an Ingrid file
static int writeIngrid(Mapping &map,
		       const aString & gridFileName=nullString);

// write a ComposteGrid to an Ingrid file
static int writeIngrid(CompositeGrid & cg,
		       const aString & gridFileName=nullString);

protected:
  
static int readPlot3d(MappingInformation & mapInfo, 
		      int gridToRead,
		      const aString & plot3dFileName,
		      DataPointMapping *dpmPointer,
		      realArray *&qPointer,
		      RealArray & par /*= Overture::nullRealArray()*/,
		      intArray *&maskPointer/*=NULL*/,
		      const bool expectIblank /* =false */ );

//  GenericGraphicsInterface *giPointer;
  
};

#endif
