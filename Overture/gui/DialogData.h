#ifndef DIALOG_DATA_H
#define DIALOG_DATA_H

#ifndef NO_APP
#include "OvertureTypes.h"
#endif
#include "GUITypes.h"


enum button_type{ GI_PUSHBUTTON, GI_TOGGLEBUTTON };

enum MessageTypeEnum 
{
  errorDialog=0,
  warningDialog,
  informationDialog,
  messageDialog
};

struct PushButton
{
// buttonCommand holds the name of the command and 
// buttonLabel holds the label that will appear on the button.
  PushButton(){pb=NULL;sensitive=true;} // *wdh*
  void setSensitive(bool trueOrFalse);  // *wdh*

  aString buttonCommand;
  aString buttonLabel;
  bool sensitive;      // *wdh*
  void *pb; // widget
};

struct ToggleButton
{
  ToggleButton(){tb=NULL; sensitive=true; state=0;} // *wdh*
  int setState(bool trueOrFalse ); // *wdh*
  void setSensitive(bool trueOrFalse);  // *wdh*

// buttonCommand holds the name of the command and 
// buttonLabel holds the label that will appear on the button.
  aString buttonCommand;
  aString buttonLabel;
  int state;
  bool sensitive;      // *wdh*
  void *tb; // widget
};

struct TextLabel
{
// textCommand holds the name of the command and 
// textLabel holds the label that will appear in front of the editable text.
// string holds the editable string.
// textWidget holds a pointer to the editable text widget, which makes it possible to change the text
// without typing in the dialog window. This might be useful for correcting typos, for example.
  TextLabel(){textWidget=NULL;sensitive=true; labelWidget=NULL;} // *wdh*
  void setSensitive(bool trueOrFalse);  // *wdh*

  aString textCommand;
  aString textLabel;
  aString string;
  bool sensitive;      // *wdh*
  void *textWidget; // Widget
  void *labelWidget; // Widget
};

struct OptionMenu
{
  OptionMenu(){menupane=NULL;sensitive=true; menuframe=NULL;} // *wdh*
  int setCurrentChoice(int command);          // *wdh*
  int setCurrentChoice(const aString & label);          // *wdh*
  void setSensitive(bool trueOrFalse);  // *wdh*
  void setSensitive(int btn, bool trueOrFalse);  // set the sensitivity of one push button 

  aString optionLabel;
  int n_options;
  PushButton *optionList;
  aString currentChoice;
  bool sensitive;      // *wdh*
  void *menupane; // widget
  void *menuframe; // widget
};

class RadioBox
{
public:
  RadioBox(){sensitive=true; radioBox=NULL; columns=1;} 
  bool setCurrentChoice(int command);          
  void setSensitive(bool trueOrFalse); // set the sensitivity of the entire radio box widget
  void setSensitive(int btn, bool trueOrFalse);  // set the sensitivity of one toggle button 

  aString radioLabel;
  int n_options;
  ToggleButton *optionList;
  aString currentChoice;
  int currentIndex;
  bool sensitive;
  int columns;
  void *radioBox; // widget
};

class PullDownMenu
{
public:
PullDownMenu(); // default constructor
~PullDownMenu();
// set all fields except the menupane in a PullDownMenu
bool setPullDownMenu(const aString &pdMainLabel, const aString commands[], const aString labels[], button_type bt, 
		    int *initState /* = NULL */); 
// set the state of a toggle button
int setToggleState(int n, bool trueOrFalse ); // *wdh*
void setSensitive(bool trueOrFalse);  // *wdh*

aString menuTitle;
int n_button;
button_type type;
PushButton *pbList;
ToggleButton *tbList;
bool sensitive;      // *wdh*
void *menupane; // widget
};

class InfoLabel
{
public:
InfoLabel(){labelWidget=NULL;} // default constructor
// textLabel holds the label that will appear in the window
// labelWidget holds a pointer to the label widget, which makes it possible to change the text
  aString textLabel;
  void *labelWidget; // Widget
};

// can't have more than 10 option menus, 6 pull-down menus, 15 radio boxes, 10 info labels
const int MAX_OP_MENU=10, MAX_PD_MENU=6, MAX_RADIO_BOXES=15, MAX_INFO_LABELS=10; 

class DialogData
{
public:
  enum WidgetTypeEnum  // *wdh* 
  {
    optionMenuWidget,
    pushButtonWidget,
    pullDownWidget,
    toggleButtonWidget,
    textBoxWidget,
    radioBoxWidget
  };

enum cursorTypeEnum
{
  pointerCursor=0,
  watchCursor,
  numberOfCursors
};

DialogData(); // default constructor. Not normally called by the user
~DialogData(); // destructor

void
openDialog(int managed = 1);

int
dialogCommands(aString **commands);

// parse an answer for a possible change to a toggle value
bool 
getToggleValue( const aString & answer, const aString & label, bool & target );
// This version is for an int target :
bool 
getToggleValue( const aString & answer, const aString & label, int & target );

// parse an answer for a possible change to a text box
bool
getTextValue( const aString & answer, const aString & label, const aString & format, GUITypes::real & target );
bool 
getTextValue( const aString & answer, const aString & label, const aString & format, int & target );
bool 
getTextValue( const aString & answer, const aString & label, const aString & format, aString & target );

int
showSibling(); // display a sibling on the screen

int 
hideSibling(); // remove a sibling from the screen

void
setSensitive(int trueFalse);

void
setSensitive(bool trueOrFalse, WidgetTypeEnum widgetType, int number ); // *wdh* 
void
setSensitive(bool trueOrFalse, WidgetTypeEnum widgetType, const aString & label); // *wdh* 

int
addOptionMenu(const aString &opMainLabel, const aString opCommands[], const aString opLabels[], int initCommand);

bool
addRadioBox(const aString &rbMainLabel, const aString rbCommands[], const aString rbLabels[], int initCommand, 
	    int columns = 1);

RadioBox&
getRadioBox(int n);

RadioBox& 
getRadioBox(const aString & radioLabel);

bool
changeOptionMenu(int nOption, const aString opCommands[], const aString opLabels[], int initCommand);

bool
changeOptionMenu(const aString & opMainLabel, const aString opCommands[], const aString opLabels[], int initCommand);

int 
deleteOptionMenus();

int
addPulldownMenu(const aString &pdMainLabel, const aString commands[], const aString labels[], button_type bt, 
		int *initState = NULL);

int
setToggleButtons(const aString tbCommands[], const aString tbLabels[], const int initState[], 
		 int numberOfColumns = 2);

int 
deleteToggleButtons();

int
setPushButtons(const aString pbCommands[], const aString pbLabels[], int numberOfRows = 2);

int
setTextBoxes(const aString textCommands[], const aString textLabels[], const aString initString[]);
 
int
setTextLabel(int n, const aString &buff);

int
setTextLabel(const aString & textLabel, const aString &buff); // set the text box with label "textLabel"

int
setToggleState(int n, int trueFalse);

int  
setToggleState(const aString & toggleButtonLabel, int trueFalse);   // set toggle state with given label

int
setExitCommand(const aString &exitC, const aString &exitL);

void
setWindowTitle(const aString &title);

void
setOptionMenuColumns(int columns);

void
setLastPullDownIsHelp(int trueFalse);

PullDownMenu&
getPulldownMenu(int n);

PullDownMenu&
getPulldownMenu(const aString & label);

OptionMenu&
getOptionMenu(int n); // *wdh*

OptionMenu&
getOptionMenu(const aString & opMainLabel); // find the option menu with this label
  
void 
closeDialog();

void *
getWidget();

aString &
getExitCommand();

void
setBuiltInDialog();
   
int
getBuiltInDialog();
   
void
setCursor(cursorTypeEnum c);

int
addInfoLabel(const aString &textLabel);

int 
deleteInfoLabels();

bool
setInfoLabel(int n, const aString &buff);

protected:
void *dialogWindow; // Widget

aString windowTitle;
aString exitCommand;
aString exitLabel;
bool exitCommandSet;

int builtInDialog; // = 1 if this dialog is parsed inside processSpecialMenuItems, otherwise = 0.
// If builtInDialog == 1, the associated callback functions will return a negative number instead
// of a positive number, which is used for user defined commands.

int pButtonRows; // pack the pButtons into this many rows
int n_pButtons; 
PushButton *pButtons; // array of buttonDefs

int toggleButtonColumns;
int n_toggle;
ToggleButton *tButtons; // array of buttonDefs

int n_text;
TextLabel *textBoxes;

int n_radioBoxes;
RadioBox radioBoxData[MAX_RADIO_BOXES];

int optionMenuColumns;
int n_optionMenu;
OptionMenu opMenuData[MAX_OP_MENU];

int n_pullDownMenu;
PullDownMenu pdMenuData[MAX_PD_MENU];

int n_infoLabels;
InfoLabel infoLabelData[MAX_INFO_LABELS];

DialogData(DialogData &source); // copy constructor disabled since it never gets defined
void operator =(DialogData &source); // assignment operator disabled since it never gets defined

int pdLastIsHelp;
};


class PickInfo
{
public:

PickInfo(); // default constructor

int pickType;
int pickWindow;
GUITypes::real pickBox[4]; // xmin, xmax, ymin, ymax
};

#endif
