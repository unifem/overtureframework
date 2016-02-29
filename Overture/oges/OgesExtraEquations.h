#ifndef OGES_EXTRA_EQUATIONS_H
#define OGES_EXTRA_EQUATIONS_H "OgesExtraEquations.h"

#include "Overture.h"

// ==========================================================================================
//   Class to define extra equations for Oges
// ==========================================================================================


class OgesExtraEquations 
{
public:

OgesExtraEquations();

// copy constructor
OgesExtraEquations( const OgesExtraEquations & ogee ); 

~OgesExtraEquations();

OgesExtraEquations& operator=(const OgesExtraEquations & x);

int neq;  // number of equations 
IntegerArray eqn;  // equation numbers : "i" values for equation in the matrix a(i,j)

// -- equation is stored in sparse row format ---
//    i=eqn(m), (ja(k),a(k), k=ia(m),...,ia(m+1)-1)  : a(i,j) values
IntegerArray ia,ja;
RealArray a;

};

#endif
