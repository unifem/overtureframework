#include "OgesExtraEquations.h"

// =============================================================================
/// \brief Constructor of the class to define an additional "extra" 
///        equations or over-ride other equations for Oges.
// =============================================================================
OgesExtraEquations::
OgesExtraEquations()
{
  neq=0;
}

// =============================================================================
/// \brief copy constructor
// =============================================================================
OgesExtraEquations::
OgesExtraEquations( const OgesExtraEquations & ogee )
{
  // deep copy: 
  *this=ogee;
}



// =============================================================================
/// \brief Destructor.
// =============================================================================
OgesExtraEquations::
~OgesExtraEquations()
{
}

// =============================================================================
/// \brief Operator equals
// =============================================================================
OgesExtraEquations& OgesExtraEquations::
operator=(const OgesExtraEquations & x)
{
  neq=x.neq;

  eqn.redim(0); ia.redim(0); ja.redim(0); a.redim(0);
  eqn=x.eqn;
  ia=x.ia;
  ja=x.ja;
  a=x.a;

  return *this;
}

