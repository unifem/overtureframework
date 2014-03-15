#ifndef EXPLICIT_HOLE_CUTTER_H
#define EXPLICIT_HOLE_CUTTER_H

#include "Overture.h"

/// This class defines an explicit hole cutter for Ogen
class ExplicitHoleCutter
{
public:
ExplicitHoleCutter();
~ExplicitHoleCutter();

// copy constructor
ExplicitHoleCutter( const ExplicitHoleCutter & holeCutter ); 

// equals operator
virtual ExplicitHoleCutter & operator=( const ExplicitHoleCutter & holeCutter );

// interactive update
int
update( GenericGraphicsInterface & gi , MappingInformation & mapInfo, CompositeGrid & cg );


// -- data ---

aString name;
MappingRC holeCutterMapping;
IntegerArray mayCutHoles;


};

#endif
