#include "Parameters.h"

#include "OneStepReaction.h"
#include "BranchingReaction.h"
#include "Chemkin.h"

//! Construction the appropriate Reactions class
int Parameters::
buildReactions()
{

  const aString & reactionName = dbase.get<aString >("reactionName");

  if( reactionName=="one step" )
  {
    dbase.get<int >("numberOfSpecies")=1;
    dbase.get<Reactions* >("reactions")=new OneStepReaction;
  }
  else if( reactionName=="branching" )
  {
    dbase.get<int >("numberOfSpecies")=2;
    dbase.get<Reactions* >("reactions")=new BranchingReaction;
  }
  else if( reactionName=="passive scalar advection" )
  {
    dbase.get<int >("numberOfSpecies")=1;
    dbase.get<bool >("advectPassiveScalar")=true;
  }
  else if( reactionName=="one equation mixture fraction" )
  {
    dbase.get<int >("numberOfSpecies")=1;
  }
  else if( reactionName=="two equation mixture fraction and extent of reaction" )
  {
    dbase.get<int >("numberOfSpecies")=2;
  }
  else if( reactionName=="ignition and growth" )
  {
    // lambda
    dbase.get<int >("numberOfSpecies")=1;
  }
  else if( reactionName=="ignition and growth desensitization" )
  {
    dbase.get<int >("numberOfSpecies")=2;
  }
  else if( reactionName=="ignition-pressure reaction rate" )
  {
    // number of species is set in CnsParameters for this case (multi-fluid)
  }
  else
  {
    printF("++++++buildReactions:: try to open Chemkin file=[%s]\n",(const char*)(reactionName+".chem.bin"));
    dbase.get<Reactions* >("reactions")=new Chemkin( reactionName+".chem.bin");      // delete this!
    dbase.get<int >("numberOfSpecies")= dbase.get<Reactions* >("reactions")->numberOfSpecies();
  }

  return 0;
}
