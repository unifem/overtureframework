#include "OvertureTypes.h"
#include <list>
#include "SmParameters.h"

#define KK_DEBUG
#include "DBase.hh"

using namespace std;
using namespace DBase;

extern "C"
{

#define getIntFromDataBase EXTERN_C_NAME(getintfromdatabase)
int getIntFromDataBase( DataBase *pdb, char *name_, int & value, int & nameLength);

#define getIntFromDataBaseCgsm EXTERN_C_NAME(getintfromdatabasecgsm)
int getIntFromDataBaseCgsm( DataBase *pdb, char *name_, int & value, int & nameLength)
// =======================================================================
/// \brief Use this routine from fortran to look-up a int or enum variables in the Cgsm data base
///
/// \return values:
///                1=found, 
///                0=not found,
///               -1=name found but not the correct type
///
/// \note: value is left unchanged if it was not found. 
// =======================================================================
{

  DataBase & dbase = *pdb;

  string name(name_,0,nameLength);
  // remove trailing blanks
  int i= name.find_last_not_of(" "); // position of last non-blank character
  name.erase(i+1,name.size()-i);


  // -- look for Cgsm enums --
  if( name=="pdeModel" && dbase.has_key("pdeModel") )
  {
    value = (int)dbase.get<SmParameters::PDEModel>("pdeModel");
    // printF("getIntFromDataBaseCgsm: pdeModel found!, value=%i\n",value);
    return 1;
  }
  else if( name=="pdeVariation" && dbase.has_key("pdeVariation") )
  {
    value = (int) dbase.get<SmParameters::PDEVariation>("pdeVariation");
    // printF("getIntFromDataBaseCgsm: pdeVariation found!, value=%i\n",value);
    return 1;
  }
  else 
  {
    return getIntFromDataBase( pdb, name_, value,nameLength);
  }
  
  return 0; // not found 
}
}
