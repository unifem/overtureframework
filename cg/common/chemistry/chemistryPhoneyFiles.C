#include "Parameters.h"
#include "OB_CompositeGridSolver.h"
#include "Reactions.h"

// ************ null functions to be used when the chemsitry files are not needed *************

int Reactions:: dbase.get<int >("debug")=0;


aString Reactions::
getName( const int & species ) const
{
  return "error";
}


void OB_CompositeGridSolver::
assignTestProblem( GridFunction & cgf )
{
  
}


int Parameters::
buildReactions()
{
  printf("****** ERROR: phoney buildReactions called. You should build Overture with chemistry files ****\n");
  if( true )
    Overture::abort("error");
    
  if(  dbase.get<aString >("reactionName")=="one step" )
  {
     dbase.get<Parameters::PDEVariation >("pdeVariation")=conservativeGodunov;
     dbase.get<int >("numberOfSpecies")=1;
//    reactions=new OneStepReaction;
  }
  else if(  dbase.get<aString >("reactionName")=="branching" )
  {
     dbase.get<Parameters::PDEVariation >("pdeVariation")=conservativeGodunov;
     dbase.get<int >("numberOfSpecies")=2;
//    reactions=new BranchingReaction;
  }
  else if(  dbase.get<aString >("reactionName")=="passive scalar advection" )
  {
     dbase.get<int >("numberOfSpecies")=1;
     dbase.get<bool >("advectPassiveScalar")=true;
  }
  else if(  dbase.get<aString >("reactionName")=="one equation mixture fraction" )
  {
     dbase.get<int >("numberOfSpecies")=1;
  }
  else if(  dbase.get<aString >("reactionName")=="two equation mixture fraction and extent of reaction" )
  {
     dbase.get<int >("numberOfSpecies")=2;
  }
  else
  {
    printf("ERROR:OverBlown has not been built with the cheimstry files\n");
    Overture::abort("error");
//    cout << "++++++buildReactions:: try to open Chemkin file=[" << reactionName+".chem.bin" << "]\n";
//    reactions=new Chemkin(reactionName+".chem.bin");      // delete this!
//    numberOfSpecies=reactions->numberOfSpecies();
  }

  return 0;
}
