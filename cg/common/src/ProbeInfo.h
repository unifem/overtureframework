#include "Overture.h"
#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;

#ifndef PROBE_INFO_H
#define PROBE_INFO_H "ProbeInfo.h"

class Parameters;
class DialogData;


//--------------------------------------------------------------------------------------------------------
/// \brief This class holds properties of probes that are used to output solution values at 
///         points and on regions.
//--------------------------------------------------------------------------------------------------------
class ProbeInfo
{
public:
enum ProbeTypesEnum
{
  probeAtGridPoint,  // probe is located at a grid point
  probeAtLocation,   // probe is located at a fixed position
  probeBoundingBox,  // save probe data on the boundary of a box (for subsequent sub-domain computations)
  probeRegion,       // probe is some average or integral over a region
  probeBoundarySurface, // probe is some average or integral over a boundary surface.
  probeUserDefined    // user defined probe
};

ProbeInfo(Parameters & par);
~ProbeInfo();


// define the properties of the probe: 
int 
update( CompositeGrid & cg, GenericGraphicsInterface & gi );

int 
buildRegionOptionsDialog(DialogData & dialog );

int 
buildSurfaceProbe( CompositeGrid & cg );

int 
getRegionOption(const aString & answer, DialogData & dialog );

// data -- make public for now
ProbeTypesEnum probeType;
int grid, iv[3];  // holds grid pt for probeAtGridPoint
real xv[3];         // holds coordinates for probeAtLocation

FILE *file;
aString fileName;

// Data for the bounding box probe file
int boundingBoxGrid, boundingBox[6], numberOfLayers;  // for the probeBoundingBox
GenericDataBase *pdb;
int numberOfTimes;
RealArray *times;  // solutions are saved at these times

Parameters & parameters;  


// This database contains various parameters:
DataBase dbase;

};

#endif
