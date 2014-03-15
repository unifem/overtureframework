#ifndef EQUATION_DOMAIN_H
#define EQUATION_DOMAIN_H

#include "Overture.h"
#include "Parameters.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <vector>
#include <list>
#else
#include <vector.h>
#include <list.h>
#endif

// =========================================================================================
// This class defines a domain where a particular type of equation is solved.
// Usually only one equation is solved (e.g. incompressible N-S) and there is only one EquationDomain. 
// We may want to solve the incompressible N-S in one domain and the heat equation in another in which case
// we will have 2 EquationDomain's.
// =========================================================================================

class EquationDomain
{
public:

EquationDomain();
// define a Region by the PDE to solve, also supply a name for the region
EquationDomain(Parameters *pde, const aString & name); 

~EquationDomain();


int setPDE( Parameters *pde );
int setName( const aString & name );

Parameters *getPDE() const;
const aString& getName() const;

std::vector<int> gridList;  // list of grids that belong to this region 

// public:
//  Parameters parameters;  // holds parameters for this domain

public:
  ListOfShowFileParameters pdeParameters;  // holds pde parameters

protected:

 Parameters *pde;   // Here is the pde we solve
 aString name;             // here is the name of the region


};



// =========================================================================================
//   This class holds a list (vector) of the different EquationDomain's
// =========================================================================================
class ListOfEquationDomains : public std::vector<EquationDomain> 
{
  public:

  ListOfEquationDomains();
  ~ListOfEquationDomains();

  // return the domain number of a given grid:
  int gridDomainNumber(int grid ) const;

  public: // make this public for now:
    
  std::vector<int> gridDomainNumberList;  // the region number of a given grid 

};


#endif
