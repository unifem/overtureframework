#ifndef INTERPOLATION_DATA
#define INTERPOLATION_DATA InterpolationData


class InterpolationData
{
public:

InterpolationData();
~InterpolationData();

int numberOfInterpolationPoints;
intSerialArray interpolationPoint;
intSerialArray interpoleeLocation;
intSerialArray interpoleeGrid;
intSerialArray variableInterpolationWidth;
realSerialArray interpolationCoordinates;

};
  
#endif
