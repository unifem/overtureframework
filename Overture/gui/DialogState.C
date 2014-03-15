#ifndef NO_APP
#include "aString.H"
#else
#include <string>
#ifndef aString
#define aString std::string
#include "GUITypes.h"
using GUITypes::real;
#endif
#endif
#include "DialogData.h"
#include "mathutil.h"
#include "DialogState.h"

// ---------------------------------------------------------------------------------------------------------
// This class holds the "state" of a Dialog data. It can be used to pass around
// information about a dialog. For example, if a derived class wants to add to the
// Dialog created in a base class then it can fill in a DialogState and pass it to the
// base class.
//    
// ---------------------------------------------------------------------------------------------------------

DialogState::DialogState()
{
  pushButtonCommands=NULL;
  pushButtonLabels=NULL;

  textCommands=NULL;
  textLabels=NULL;
  textStrings=NULL;

  toggleButtonCommands=NULL;
  toggleButtonLabels=NULL;
  toggleState=NULL;
}

DialogState::~DialogState()
{ 
  delete [] pushButtonCommands;
  if( pushButtonLabels!=pushButtonCommands ) // labels are often the same string as the commands
    delete [] pushButtonLabels;

  delete [] textCommands;
  if( textCommands!=textLabels )
    delete [] textLabels;
  delete [] textStrings;

  delete [] toggleButtonCommands;
  if( toggleButtonCommands!=toggleButtonLabels )
    delete [] toggleButtonLabels;
  delete [] toggleState;

} 

