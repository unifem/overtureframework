#ifndef BODY_FORCE_H
#define BODY_FORCE_H

#include "Overture.h"
#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;


// ====================================================================================
// This class holds information about different body forces (and boundary forcing).
// ====================================================================================
class BodyForce
{

public:
  
// The temperature BC is of the form 
//      a0*T + an*T.n = g          (constant coefficients)
//      a0(x)*T + an(x)*T.n = g    (variable coefficients)
enum temperatureBoundaryConditionOptionEnum
{
  temperatureBoundaryConditionIsConstantCoefficients,
  temperatureBoundaryConditionIsVariableCoefficients
};
  
enum bodyTemperatureOptionEnum
{
  adiabaticBody=0,
  isothermalBody,
  conductingBody
};


BodyForce();
~BodyForce();

// copy constructor
BodyForce( const BodyForce & bf ); 

// equals operator
virtual BodyForce & operator=( const BodyForce & bf );

virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

// plot forcing regions
static int plotForcingRegions(GenericGraphicsInterface &gi, DataBase & dbase, CompositeGrid & cg,
			      GraphicsParameters & psp );

// This database contains the parameters and data for the body force:
DataBase dbase;

protected:

void initialize();


};

// -------------------------------------------------------------------------------------------
// Here is the class where we keep current parameters that define the current region for a body force:
// These parameters are only used while the body force is being defined.
// ------------------------------------------------------------------------------------------
class BodyForceRegionParameters
{
public:

BodyForceRegionParameters();
~BodyForceRegionParameters();

// This database contains the parameters for all types of body forces
DataBase dbase;

};


#endif
