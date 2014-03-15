#ifndef CONTROLLER_H
#define CONTROLLER_H

// ====================================================================================
// This class is used to implement control algorithms
// ====================================================================================

#include "Overture.h"

class Parameters;

class Controller
{

public:

Controller(Parameters & parameters);

virtual ~Controller();

Controller( const Controller & X );


int createControlSequence( const aString & sequenceName, const std::vector<aString> componentNames );

// get from a data base file
int get( const GenericDataBase & dir, const aString & name);

// evaluate the control function: (single output):
int getControl( const real t, real & uControl, real & uControlDot ) const;

// put to a data base file
int put( GenericDataBase & dir, const aString & name) const;

int saveControlSequenceData( const aString & sequenceName, const real t, const std::vector<real> values );

virtual int saveToShowFile() const;

// interactive update
virtual int update(CompositeGrid & cg, GenericGraphicsInterface & gi );

// update the control function based on the solution v. (use getControl to return current values)
int updateControl( realCompositeGridFunction & v, const real t, const real dt );

protected:

Parameters & parameters;
  
};

#endif
