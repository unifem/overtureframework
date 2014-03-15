#ifndef READ_PLOT3D_H
#define READ_PLOT3D_H

#include "Mapping.h"
class DataPointMapping;
class GenericGraphicsInterface;

int
readPlot3d(MappingInformation & mapInfo, 
           const aString & gridFileName=nullString,
           DataPointMapping *dpmPointer=NULL );
int
readPlot3d(GenericGraphicsInterface & gi,
           RealArray & q, RealArray & par, 
           const aString & qFileName=nullString);
int
readPlot3d(MappingInformation & mapInfo, 
           const aString & gridFileName,
           intArray *maskPointer );
int
readPlot3d(MappingInformation & mapInfo, 
           const aString & plot3dFileName,
           DataPointMapping *dpmPointer,
           RealArray *qPointer,
           RealArray & par = Overture::nullRealArray(),
           intArray *maskPointer=NULL );
#endif
