#ifndef DETECT_COLLISIONS_H
#define DETECT_COLLISIONS_H

#include "Overture.h"
#include "RigidBodyMotion.h"
#include "BodyDefinition.h"

// this function should become a Class

int
detectCollisions( real t, 
		  GridCollection & gc, 
		  int numberOfRigidBodies, 
		  RigidBodyMotion **body,
                  const BodyDefinition & bodyDefinition,
                  const real minimumSeparation=2.5 );



#endif
