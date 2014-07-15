#ifndef POINT_H
#define POINT_H

#include "ReferenceCounting.h"
#include "GenericGraphicsInterface.h"

class Point: public ReferenceCounting
{
public:
Point(); // constructor
~Point(); // destructor
void
operator=(Point & source); // assignment operator
void 
plot(GenericGraphicsInterface & gi, GraphicsParameters & gp = Overture::defaultGraphicsParameters());

real coordinate[3];

};

class PointList
{
public:
PointList(int listSize=100);

~PointList();

int
size(){ return nPoints; };

void
add(Point & newPoint);

void
clearAll();

void 
clearLast();

Point & 
operator[]( const int pointIndex );

void
plot(GenericGraphicsInterface & gi, GraphicsParameters & gp /* =Overture::defaultGraphicsParameters() */);

private:
int nPoints, maxPoints;
Point *points;
};

#endif
