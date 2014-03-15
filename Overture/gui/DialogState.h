// ---------------------------------------------------------------------------------------------------------
// This class holds the "state" of a Dialog data. It can be used to pass around
// information about a dialog. For example, if a derived class wants to add to the
// Dialog created in a base class then it can fill in a DialogState and pass it to the
// base class.
//    
// ---------------------------------------------------------------------------------------------------------

#ifndef DIALOG_STATE_H
#define DIALOG_STATE_H



#ifndef NO_APP
#include "OvertureTypes.h"
#endif
#include "GUITypes.h"


class DialogState
{
public:
DialogState();

~DialogState();

// There are no options menu's here since these can be built independently 

aString *pushButtonCommands;
aString *pushButtonLabels;

aString *textCommands;
aString *textLabels;
aString *textStrings;

aString *toggleButtonCommands;
aString *toggleButtonLabels;
int *toggleState;


};

#endif
