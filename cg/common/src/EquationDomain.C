#include "EquationDomain.h"


EquationDomain::
EquationDomain() : pde(0),name("Domain0")
// ===============================================================================
// /Description:
//   Constructor: this class defines a Domain for cg where a particular type of equation is solved.
// Usually only one equation is solved (e.g. incompressible N-S) and there is only one domain.
// We may want to solve the incompressible N-S in one domain and the heat equation in another in which case
// we will have 2 domains. 
// =========================================================================================
{
}


EquationDomain::
EquationDomain(Parameters *pde_, const aString & name_) : pde(pde_), name(name_)
// ========================================================================================
// /Description:
//    Define a Domain by the PDE to solve, also supply a name for the domain
// ========================================================================================
{
}


EquationDomain::
~EquationDomain()
{
  pde = 0;
}



int EquationDomain::
setPDE( Parameters *pde_ )
// ========================================================================================
// /Description:
//    Define the PDE to be solved in this domain.
// ========================================================================================
{
  pde=pde_;
}

int EquationDomain::
setName( const aString & name_ )
// ========================================================================================
// /Description:
//    Set the name of this domain.
// ========================================================================================
{
  name=name_;
}


Parameters *EquationDomain::
getPDE() const
// ========================================================================================
// /Description:
//    Get the PDE being solved in this domain.
// ========================================================================================
{
  return pde;
}


const aString& EquationDomain::
getName() const
// ========================================================================================
// /Description:
//    Get the name of this domain.
// ========================================================================================
{
  return name;
}




// *****************************************************************************
// *********************** ListOfEquationDomains *****************************
// *****************************************************************************


ListOfEquationDomains::
ListOfEquationDomains()
// =========================================================================================
// /Description:
//   Constructor: This class holds a list of the different EquationDomain's
// =========================================================================================
{
}

ListOfEquationDomains::
~ListOfEquationDomains()
{
}


// return the domain number of a given grid:
int ListOfEquationDomains::
gridDomainNumber(int grid ) const
{
  return gridDomainNumberList[grid];
}

