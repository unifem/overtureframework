//#include "OvertureTypes.h"
//#include "aString.H"
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

#include <assert.h>
void printF(const char *format, ...);
void fPrintF(FILE *file, const char *format, ...);
extern aString & sPrintF(aString & s, const char *format, ...);
extern int 
sScanF(const aString & s, const char *format, 
       void *p0,
       void *p1=NULL, 
       void *p2=NULL, 
       void *p3=NULL,
       void *p4=NULL,
       void *p5=NULL,
       void *p6=NULL,
       void *p7=NULL,
       void *p8=NULL,
       void *p9=NULL,
       void *p10=NULL,
       void *p11=NULL,
       void *p12=NULL,
       void *p13=NULL,
       void *p14=NULL,
       void *p15=NULL,
       void *p16=NULL,
       void *p17=NULL,
       void *p18=NULL,
       void *p19=NULL,
       void *p20=NULL,
       void *p21=NULL,
       void *p22=NULL,
       void *p23=NULL,
       void *p24=NULL,
       void *p25=NULL,
       void *p26=NULL,
       void *p27=NULL,
       void *p28=NULL,
       void *p29=NULL );

extern int 
fScanF(FILE *file, const char *format, 
       void *p0,
       void *p1=NULL, 
       void *p2=NULL, 
       void *p3=NULL,
       void *p4=NULL,
       void *p5=NULL,
       void *p6=NULL,
       void *p7=NULL,
       void *p8=NULL,
       void *p9=NULL,
       void *p10=NULL,
       void *p11=NULL,
       void *p12=NULL,
       void *p13=NULL,
       void *p14=NULL,
       void *p15=NULL,
       void *p16=NULL,
       void *p17=NULL,
       void *p18=NULL,
       void *p19=NULL,
       void *p20=NULL,
       void *p21=NULL,
       void *p22=NULL,
       void *p23=NULL,
       void *p24=NULL,
       void *p25=NULL,
       void *p26=NULL,
       void *p27=NULL,
       void *p28=NULL,
       void *p29=NULL
  );


// default constructor
DialogData::
DialogData()
{
  dialogWindow = NULL;
  windowTitle = "Your title goes here";
  exitCommand = "close dialog";
  exitLabel = "Close";
  exitCommandSet = false;
  builtInDialog = 0;
  n_pButtons=0;
  pButtons=NULL;
  pButtonRows=2; // two rows of push buttons
  n_toggle = 0;
  tButtons = NULL;
  toggleButtonColumns=2; // two columns of toggle buttons
  n_text=0;
  textBoxes=NULL;
  n_optionMenu=0;
  optionMenuColumns=2; // two columns of option menus
  n_pullDownMenu=0;
  pdLastIsHelp=0;
  n_radioBoxes=0;
  n_infoLabels=0;
}

// destructor
DialogData::
~DialogData()
{
  int i,j;
  delete [] pButtons;
  delete [] tButtons;
  for (i=0; i<n_text; i++)
  {
//    delete textBoxes[i].string;
  }
  delete [] textBoxes;
  for (i=0; i<n_optionMenu; i++)
  {
    delete [] opMenuData[i].optionList;
  }

  for (i=0; i<n_radioBoxes; i++)
  {
    delete [] radioBoxData[i].optionList;
  }

 // These are now done in the PulldownMenu destructor *wdh* 030825
//    for (i=0; i<n_pullDownMenu; i++)
//    {
//      // a pulldown menu can have either push buttons or toggle buttons
//      if (pdMenuData[i].type == GI_PUSHBUTTON)
//      {
//        delete [] pdMenuData[i].pbList;
//      }
    
//      else if (pdMenuData[i].type == GI_TOGGLEBUTTON)
//      {
//        delete [] pdMenuData[i].tbList;
//      }
//    }

}

//\begin{>DialogDataInclude.tex}{\subsection{setExitCommand}} 
int DialogData::
setExitCommand(const aString &exitC, const aString &exitL)
//-----------------------------------------------------
// /Description: Set the exit command on the dialog window in the GUIState. Note that
//  the dialog window will apear after pushGUI has been called.
//
// /exitC(input): The command hat will be issued when the exit button is pressed.
// /exitL(input): The text label that will appear on the exit button.
//
// /Return values: The function returns 1 on a successful completion and 0 if an error occured.
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  if (exitC == "" || exitL == "")
  {
    printF("ERROR: setExitCommand: must not set the exit command or label to an empty string\n");
    return 0;
  }
  
  exitCommand = exitC;
  exitLabel = exitL;
  exitCommandSet = true;
  
  return 1;
}


//\begin{>>DialogDataInclude.tex}{\subsection{setToggleButtons}} 
int DialogData::
setToggleButtons(const aString tbCommands[], const aString tbLabels[], const int initState[], 
		 int numberOfColumns /* = 2 */)
//-----------------------------------------------------
// /Description: Set the toggle buttons of the dialog window in the GUIState. The buttons
// will appear on the dialog window after pushGUI has been called.
//
// /tbCommands(input): Array of strings containing the commands for the toggle buttons. 
//   The array must be terminated by an empty string ("").
// /tbLabels(input): Array of strings containing the text labels that will be put on
//   the toggle buttons. The array must be terminated by an empty string ("").
// /initState(input): Array that describes the initial state of each toggle button.
// /numberOfColumns(input): Optional argument that specifies the number of columns in which the 
//   toggle buttons shall be organized in the dialog window.
// /Return values: The function returns 1 on a successful completion and 0 if an error occured.
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{

// tbCommands and tbLabels are aString arrays terminated by "".
// Start by counting the number of elements
  int ne;
  for (ne=0; tbCommands[ne] != "" && tbLabels[ne] != ""; ne++);
  if (ne == 0)
  {
    printF("setToggleButton: WARNING: Empty list of commands.\n"
	   "Not building an empty set of toggle buttons.\n");
    return 0;
  }

// tmp
//  printF("Found %i toggle buttons\n", ne);
  
  n_toggle = ne;
  tButtons = new ToggleButton [ne];

// copy the strings and the initial state
  int j;
  for (j=0; j<ne; j++)
  {
    tButtons[j].buttonCommand = tbCommands[j];
    tButtons[j].buttonLabel = tbLabels[j];
    tButtons[j].state = initState[j];
  }

  toggleButtonColumns = numberOfColumns; // organize the toggle buttons in this many columns

  return 1;
}

//\begin{>>DialogDataInclude.tex}{\subsection{deleteToggleButtons}} 
int DialogData::
deleteToggleButtons()
//-----------------------------------------------------
// /Description: Delete the toggle buttons.
// /Author: WDH 
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  delete [] tButtons;
  tButtons=NULL;
  n_toggle=0;

  return 0;
}


//\begin{>>DialogDataInclude.tex}{\subsection{setPushButtons}} 
int DialogData::
setPushButtons(const aString pbCommands[], const aString pbLabels[], int numberOfRows /* = 2 */)
//-----------------------------------------------------
// /Description: Set the push buttons of the dialog window in the GUIState. The buttons
// will appear on the dialog window after pushGUI has been called.
//
// /pbCommands(input): Array of strings containing the commands for the push buttons. 
//   The array  must be terminated by an empty string ("").
// /pbLabels(input): Array of strings containing the text labels that will be put on
//   the push buttons. The array must be terminated by an empty string ("").
// /numberOfRows(input): Optional argument that specifies the number of rows in which the 
//   push buttons shall be organized in the dialog window.
//
// /Return values: The function returns 1 on a successful completion and 0 if an error occured.
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{

// pbCommands and pbLabels are aString arrays terminated by "".
// Start by counting the number of elements
  int ne;
  for (ne=0; pbCommands[ne] != "" && pbLabels[ne] != ""; ne++);
  if (ne == 0)
  {
    printF("setPushButton: WARNING: Empty list of commands.\n"
	   "Not building an empty set of push buttons.\n");
    return 0;
  }
// tmp
//  printF("Found %i push buttons\n", ne);
  
  n_pButtons = ne;
  pButtons = new PushButton [ne];

// copy the strings and the initial state
  int j;
  for (j=0; j<ne; j++)
  {
    pButtons[j].buttonCommand = pbCommands[j];
    pButtons[j].buttonLabel = pbLabels[j];
  }

// organize the pushbuttons in this many rows
  pButtonRows = numberOfRows;

  return 1;
}

//\begin{>>DialogDataInclude.tex}{\subsection{setTextBoxes}} 
int DialogData::
setTextBoxes(const aString textCommands[], const aString textLabels[], const aString initString[])
//-----------------------------------------------------
// /Description: Set the text boxes of the dialog window in the GUIState. The boxes
// will appear on the dialog window after pushGUI has been called.
//
// /textCommands(input): Array of strings containing the commands for the text boxes. 
//   The array  must be terminated by an empty string ("").
// /textLabelsLabels(input): Array of strings containing the text labels that will be put 
//   in front of the text boxes. The array must be terminated by an empty string ("").
// /initString(input):  Array of strings containing the initial text that will be put
//   in each text box. The array must be terminated by an empty string ("").
//
// /Return values: The function returns 1 on a successful completion and 0 if an error occured.
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{

// textCommands, textLabels, and initString are aString arrays terminated by "".
// Start by counting the number of elements
  int ne;
  for (ne=0; textCommands[ne] != "" && textLabels[ne] != "" && initString[ne] != ""; ne++);
  if (ne == 0)
  {
    printF("setTextBoxes: WARNING: Empty list of commands.\n"
	   "Not building an empty set of text boxes.\n");
    return 0;
  }
// tmp
//  printF("Found %i text boxes\n", ne);
  
  n_text = ne;
  textBoxes = new TextLabel [ne];

// copy the strings and the initial state
  int j;
  for (j=0; j<ne; j++)
  {
    textBoxes[j].textCommand = textCommands[j];
    textBoxes[j].textLabel = textLabels[j];
    textBoxes[j].string = initString[j];
  }

  return 1;
}

//\begin{>>DialogDataInclude.tex}{\subsection{addInfoLabel}} 
int DialogData::
addInfoLabel(const aString & textLabel)
//-----------------------------------------------------
// /Description: Add a new info label to the dialog window.
//
// /textLabel(input): The new text string.
//
// /Return code: The number of the new info label in the GUI, or -1 if there was no
// space left. (There is only space for MAX\_INFO\_LABELS (=10 by default) in each dialog window.)
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  if (n_infoLabels >= MAX_INFO_LABELS)
  {
    printF("ERROR: addInfoLabel: No space for more info labels.\n");
    return -1;
  }
  
  infoLabelData[n_infoLabels].textLabel = textLabel;

  return n_infoLabels++;
}

//\begin{>>DialogDataInclude.tex}{\subsection{addInfoLabel}} 
int DialogData::
deleteInfoLabels()
//-----------------------------------------------------
// /Description: Delete the existing info labels.
//
// /Return code: 0 
// /Author: WDH
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  n_infoLabels=0;
  return 0;
}

//\begin{>>DialogDataInclude.tex}{\subsection{setTextLabel}} 
int DialogData::
setTextLabel(const aString & textLabel, const aString &buff)
//----------------------------------------------------
// /Description: Set the text string textlabel with label "textLabel" in the currently
// active GUIState.
//
// /textLabel(input): The label of the text label in the array given to setTextBoxes during setup.
// /buff(input): The new text string.
//
// /Author: WDH
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  for( int i=0; i<n_text; i++ )
  {
    if( textLabel==textBoxes[i].textLabel )
    {
      setTextLabel(i,buff);
      return 0;
    }
  }
  printF("ERROR:setTextLabel: label [%s] not found!\n"
         "   Valid labels are\n",(const char*)textLabel.c_str());
  for( int i=0; i<n_text; i++ )
    printF("[%s]\n",(const char*)textBoxes[i].textLabel.c_str());

  return 0;
}


//\begin{>>DialogDataInclude.tex}{\subsection{addOptionMenu}} 
int DialogData::
addOptionMenu(const aString &opMainLabel, const aString opCommands[], const aString opLabels[], int initCommand)
//-----------------------------------------------------
// /Description: Add an option menu to the dialog window. The option menu will appear when the
// dialog window is displayed, i.e., after pushGUI has been called.
//
// /opMainLabel(input): The descriptive label that will appear to the left of the option menu
//  on the dialog window.
// /opCommands(input): An array of strings with the command that will be issued when each menu
//  item is selected. The array must be terminated by an empty string ("").
// /opLabels(input): An array of strings with the label that will be put on each menu
//  item. The array must be terminated by an empty string ("").
// /initCommand(input): The index of the initial selection in the opLabels array. This label will
//  appear on top of the option menu to indicate the initial setting.
// /Return values: The function returns 1 on a successful completion and 0 if an error occured.
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
// is there room for more option menus?
  if (n_optionMenu >= MAX_OP_MENU)
  {
    printF("addOptionMenu: ERROR: Can't allocate another option menu.\n"
	   "Increase MAX_OP_MENU and recompile mogl.C\n");
    return 0;
  }

// we are working on option menu # n_optionMenu;
  int i=n_optionMenu;

// opCommands and opLabels are supposed to be "" terminated arrays of aString.
// Start by counting the number of elements
  int ne;
  for (ne=0; opCommands[ne] != "" && opLabels[ne] != ""; ne++);
  if (ne == 0)
  {
    printF("addOptionMenu: WARNING: Empty list of option commands. Not building an empty options menu.\n");
    return 0;
  }
// tmp
//  printF("Found %i option menu items\n", ne);
  
  opMenuData[i].n_options = ne;
  opMenuData[i].optionList = new PushButton [ne];

// copy all the strings
  int j;
  for (j=0; j<ne; j++)
  {
    opMenuData[i].optionList[j].buttonCommand = opCommands[j];
    opMenuData[i].optionList[j].buttonLabel = opLabels[j];
  }

// copy the optionLabel
  opMenuData[i].optionLabel = opMainLabel;
  
// set the initial choice
  if (initCommand >= 0 && initCommand < ne)
    opMenuData[i].currentChoice = opMenuData[i].optionList[initCommand].buttonCommand;
  else
// use the first choice if initCommand is not valid
  {
    opMenuData[i].currentChoice = opMenuData[i].optionList[0].buttonCommand; 
    printF("addOptionMenu:WARNING: option menu=[%s]: initCommand=%i out of bounds, using the first "
	   "item as initial choice\n", (const char*)opMainLabel.c_str(),initCommand);
  }
  
// increment the count of option menus
  n_optionMenu++;

  return 1;
}

//\begin{>>DialogDataInclude.tex}{\subsection{deleteOptionMenus}} 
int DialogData::
deleteOptionMenus()
//-----------------------------------------------------
// /Description: Delete all option menus.
// /Author: WDH 
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  for( int i=0; i<n_optionMenu; i++ )
  {
    delete [] opMenuData[i].optionList;
    opMenuData[i].optionList=NULL;
  }
  n_optionMenu=0;
}



//\begin{>>DialogDataInclude.tex}{\subsection{addRadioBox}} 
bool DialogData::
addRadioBox(const aString &rbMainLabel, const aString rbCommands[], const aString rbLabels[], int initCommand,
	    int columns /* = 1 */)
//-----------------------------------------------------
// /Description: Add a radio box to the dialog window. The radio buttons will appear when the
// dialog window is displayed, i.e., after pushGUI has been called.
//
// /rbCommands(input): An array of strings with the command that will be issued when the radio 
//  button is pressed. The array must be terminated by an empty string ("").
// /rbLabels(input): An array of strings with the label that will be put on each radio button. 
// The array must be terminated by an empty string ("").
// /initCommand(input): The index of the initial selection in the rbLabels array. This radio 
//  button will be marked initially.
// /Return values: The function returns true on a successful completion and false if an error occured.
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
// is there room for more option menus?
  if (n_radioBoxes >= MAX_RADIO_BOXES)
  {
    printF("addRadioBox: ERROR: Can't allocate another radio box.\n"
	   "Increase MAX_RADIO_BOXES in DialogData.h and recompile!\n");
    return false;
  }

// we are working on radio box # n_radioBoxes;
  int i=n_radioBoxes;

// rbCommands and rbLabels are supposed to be "" terminated arrays of aString.
// Start by counting the number of elements
  int ne;
  for (ne=0; rbCommands[ne] != "" && rbLabels[ne] != ""; ne++);
  if (ne == 0)
  {
    printF("addRadioBox: WARNING: Empty list of options. Not building an empty radio box!\n");
    return false;
  }
// tmp
//  printF("Found %i radio buttons\n", ne);
  
  radioBoxData[i].n_options = ne;
  radioBoxData[i].optionList = new ToggleButton [ne];

// copy all the strings
  int j;
  for (j=0; j<ne; j++)
  {
    radioBoxData[i].optionList[j].buttonCommand = rbCommands[j];
    radioBoxData[i].optionList[j].buttonLabel = rbLabels[j];
    radioBoxData[i].optionList[j].state = false;
  }

// copy the optionLabel
  radioBoxData[i].radioLabel = rbMainLabel;

  radioBoxData[i].columns = columns;

// set the initial choice
  if (initCommand >= 0 && initCommand < ne)
  {
    radioBoxData[i].currentChoice = radioBoxData[i].optionList[initCommand].buttonCommand;
    radioBoxData[i].currentIndex  = initCommand;
  }
  else
// use the first choice if initCommand is not valid
  {
    radioBoxData[i].currentChoice = radioBoxData[i].optionList[0].buttonCommand; 
    radioBoxData[i].currentIndex  = 0;
    printF("addRadioBox: WARNING: initCommand=%i out of bounds, using the first "
	   "item as initial choise\n", initCommand);
  }
  
// increment the count of option menus
  n_radioBoxes++;

  return true;
}

//\begin{>>DialogDataInclude.tex}{\subsection{addPulldownMenu}} 
int DialogData::
addPulldownMenu(const aString &pdMainLabel, const aString commands[], const aString labels[], button_type bt, 
		int *initState /* = NULL */)
//-----------------------------------------------------
// /Description: Add a pulldown menu to the dialog window. The pulldown menu will appear when the
// dialog window is displayed, i.e., after pushGUI has been called. Successive pulldown menus 
// will be stacked from left to right on the menu bar.
//
// /pdMainLabel(input): The label that will appear on the menu bar.
//
// /commands(input): An array of strings with the command that will be issued when each menu
//  item is selected. The array must be terminated by an empty string ("").
//
// /labels(input): An array of strings with the label that will be put on each menu
//  item. The array must be terminated by an empty string ("").
//
// /bt(input): The type of buttons in the menu. Can be either GI\_PUSHBUTTON or GI\_TOGGLEBUTTON.
//
// /initState(input): Optional argument that only is used when bt == GI\_TOGGLEBUTTON. This argument
//  is an array that specifies the initial state of each toggle buttons. If this argument is absent
//  when bt == GI\_TOGGLEBUTTON, no menu items are marked as beeing selected.
//
// /Return values: The function returns 1 on a successful completion and 0 if an error occured.
//
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
// is there room for more option menus?
  if (n_pullDownMenu >= MAX_PD_MENU)
  {
    printF("addPulldownMenu: ERROR: Can't allocate another pulldown menu.\n"
	   "Increase MAX_PD_MENU and recompile mogl.C\n");
    return 0;
  }

// we are working on option menu # n_optionMenu;
  int i=n_pullDownMenu;

// commands and labels are supposed to be NULL-terminated arrays of char *.
// Start by counting the number of elements
  int ne;
  for (ne=0; commands[ne] != "" && labels[ne] != ""; ne++);
  if (ne == 0)
  {
    printF("addPulldownMenu: WARNING: Empty list of commands. Not building an empty pulldown menu.\n");
    return 0;
  }
// tmp
//  printF("Found %i pulldown menu items\n", ne);
  
  pdMenuData[i].n_button = ne;

  pdMenuData[i].type = bt; // set the type
  
  if (bt == GI_PUSHBUTTON)
  {
    pdMenuData[i].pbList = new PushButton [ne];

// copy all the strings
    int j;
    for (j=0; j<ne; j++)
    {
      pdMenuData[i].pbList[j].buttonCommand = commands[j];
      pdMenuData[i].pbList[j].buttonLabel = labels[j];
    }

  }
  else if (bt == GI_TOGGLEBUTTON)
  {
    pdMenuData[i].tbList = new ToggleButton [ne];

// copy all the strings
    int j;
    for (j=0; j<ne; j++)
    {
      pdMenuData[i].tbList[j].buttonCommand = commands[j];
      pdMenuData[i].tbList[j].buttonLabel = labels[j];
// set the initial state
      if (initState)
	pdMenuData[i].tbList[j].state = initState[j];
      else
	pdMenuData[i].tbList[j].state = 0;
    }
  }
  
// copy the Title Label
//    pdMenuData[i].menuTitle = new char[strlen(pdMainLabel)+1];
//    strcpy(pdMenuData[i].menuTitle, pdMainLabel);
  pdMenuData[i].menuTitle = pdMainLabel;

// increment the count of pull down menus
  n_pullDownMenu++;

  return 1;
}

//\begin{>>DialogDataInclude.tex}{\subsection{changeOptionMenu}}
bool DialogData::
changeOptionMenu(const aString & opMainLabel, const aString opCommands[], const aString opLabels[], int initCommand)
//-----------------------------------------------------
// /Description: 
//    Change an option menu with a given name
//
// /opMainLabel( input): name of the option menu
// /opCommands, opLabels (input);
// /initCommand (input) : 
//
// /Return values: None.
// /Author: wdh
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  int optionMenuNumber=-1;
  if( n_optionMenu<=0 )
  {
    printF("DialogData::changeOptionMenu:ERROR: there are no option menus!\n");
    assert( n_optionMenu>0 );
  }
  for( int i=0; i<n_optionMenu; i++ )
  {
    if( opMainLabel==opMenuData[i].optionLabel )
    {
      optionMenuNumber=i;
      break;
    }
      
  }
  if( optionMenuNumber<0 ) 
  {
    printF("ERROR:changeOptionMenu: opMainLabel [%s] not found!\n"
	   "   Valid labels are\n",(const char*)opMainLabel.c_str());
    for( int i=0; i<n_optionMenu; i++ )
      printF("[%s]\n",(const char*)opMenuData[i].optionLabel.c_str());
  }
  
  return changeOptionMenu(optionMenuNumber,opCommands,opLabels,initCommand);
  
}



//\begin{>>DialogDataInclude.tex}{\subsection{setWindowTitle}} 
void DialogData::
setWindowTitle(const aString &title)
//-----------------------------------------------------
// /Description: Set the title of the dialog window in the GUIState. The title
// will appear on the dialog window after pushGUI has been called.
//
// /title(input): The new title.
//
// /Return values: None.
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  windowTitle = title;
}

//\begin{>>DialogDataInclude.tex}{\subsection{setOptionMenuColumns}} 
void DialogData::
setOptionMenuColumns(int columns)
//----------------------------------------------------
// /Description: Set the number of columns in which the option menus should 
// be organized on the dialog window.
// /columns(input): The number of columns.
// /Return values: None.
// /Author: AP
//\end{GUIStateInclude.tex}
//-----------------------------------------------------
{
  if (columns > 0)
    optionMenuColumns = columns;
}



//\begin{>>DialogDataInclude.tex}{\subsection{setLastPullDownIsHelp}} 
void DialogData::
setLastPullDownIsHelp(int trueFalse)
//----------------------------------------------------
// /Description: Specify whether the last pulldown menu should appear in the
// right end of the menu bar on the dialog window, where the help menu often is
// located.
// /trueFalse(input): 1 if the last pulldown menu should be placed in the right end. 
//  Otherwise, the last pulldown menu is placed just to the right of the second last 
//  pulldown menu 
// /Return value: None.
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  pdLastIsHelp = (trueFalse == 1)? 1:0;
}


//\begin{>>DialogDataInclude.tex}{\subsection{getPulldownMenu}} 
PullDownMenu& DialogData::
getPulldownMenu(int n)
// -----------------------------------------------------------------------------
// /Description: 
//    return the n'th pull-down menu, $0 \leq n < n_{pullDownMenu}$.
// /Author: WDH
//\end{DialogDataInclude.tex}
// -----------------------------------------------------------------------------
{
  return pdMenuData[max(0,min(n,n_pullDownMenu-1))];
}

//\begin{>>DialogDataInclude.tex}{\subsection{getPulldownMenu}} 
PullDownMenu& DialogData::
getPulldownMenu(const aString & label)
// -----------------------------------------------------------------------------
// /Description: 
//    Find the pulldown menu with the given main label.
// /label (input) : the label given to the pulldown
// /Return values: the pulldown menu with the given main label, return PulldownMenu 0
// if the label was not found.
// /Author: WDH
//\end{DialogDataInclude.tex}
// -----------------------------------------------------------------------------
{
  if( n_pullDownMenu<=0 )
  {
    printF("DialogData::getPulldownMenu:ERROR: there are no pull down menus!\n");
    assert( n_pullDownMenu>0 );
  }
  for( int i=0; i<n_pullDownMenu; i++ )
  {
    if( label==pdMenuData[i].menuTitle )
      return pdMenuData[i];
  }
  printF("ERROR:getPulldownMenu: label [%s] not found!\n"
         "   Valid labels are\n",(const char*)label.c_str());
  for( int i=0; i<n_pullDownMenu; i++ )
    printF("[%s]\n",(const char*)pdMenuData[i].menuTitle.c_str());
  return pdMenuData[0];
}

//\begin{>>DialogDataInclude.tex}{\subsection{getOptionMenu}} 
OptionMenu& DialogData::
getOptionMenu(int n)
// -----------------------------------------------------------------------------
// /Description: 
//    return the n'th option menu, $0 \leq n < n_{optionMenu}$.
// /Author: WDH
//\end{DialogDataInclude.tex}
// -----------------------------------------------------------------------------
{
  return opMenuData[max(0,min(n,n_optionMenu-1))];
}

//\begin{>>DialogDataInclude.tex}{\subsection{getOptionMenu}} 
OptionMenu& DialogData::
getOptionMenu(const aString & opMainLabel)
// -----------------------------------------------------------------------------
// /Description: 
//    Find the option menu with the given main label.
// /opMainLabel (input) : the label given to an option menu.
// /Return values: the OptionMenu with the given main label, return OptionMenu 0
// if the label was not found.
// /Author: WDH
//\end{DialogDataInclude.tex}
// -----------------------------------------------------------------------------
{
  if( n_optionMenu<=0 )
  {
    printF("DialogData::getOptionMenu:ERROR: there are no option menus!\n");
    assert( n_optionMenu>0 );
  }
  for( int i=0; i<n_optionMenu; i++ )
  {
    if( opMainLabel==opMenuData[i].optionLabel )
      return opMenuData[i];
  }
  printF("ERROR:getOptionMenu: opMainLabel [%s] not found!\n"
         "   Valid labels are\n",(const char*)opMainLabel.c_str());
  for( int i=0; i<n_optionMenu; i++ )
    printF("[%s]\n",(const char*)opMenuData[i].optionLabel.c_str());
  return opMenuData[0];
}

//\begin{>>DialogDataInclude.tex}{\subsection{getRadioBox}} 
RadioBox& DialogData::
getRadioBox(int n)
// -----------------------------------------------------------------------------
// /Description: 
//    return the n'th radio box, $0 \leq n < n_{radioBoxes}$.
// /Author: AP
//\end{DialogDataInclude.tex}
// -----------------------------------------------------------------------------
{
  return radioBoxData[max(0,min(n,n_radioBoxes-1))];
}

//\begin{>>DialogDataInclude.tex}{\subsection{getRadioBox}} 
RadioBox& DialogData::
getRadioBox(const aString & radioLabel)
// -----------------------------------------------------------------------------
// /Description: 
//    Find the radio box menu with the given main label.
// /radioLabell (input) : the label given to an radio box
// /Return values: the RadioBox with the given label, return RadioBox 0
// if the label was not found.
// /Author: WDH
//\end{DialogDataInclude.tex}
// -----------------------------------------------------------------------------
{
  if( n_radioBoxes<=0 )
  {
    printF("DialogData::getRadioBox:ERROR: there are no radio boxes!\n");
    assert( n_radioBoxes>0 );
  }
  for( int i=0; i<n_radioBoxes; i++ )
  {
    if( radioLabel==radioBoxData[i].radioLabel )
      return radioBoxData[i];
  }
  printF("ERROR:getRadioBox: radioLabel [%s] not found!\n"
         "   Valid labels are\n",(const char*)radioLabel.c_str());
  for( int i=0; i<n_radioBoxes; i++ )
    printF("[%s]\n",(const char*)radioBoxData[i].radioLabel.c_str());
  return radioBoxData[0];
}

int DialogData::
dialogCommands(aString **commands)
{
  int ns=0, i;
// save all commands in one array, add one entry for the exit command
  int nDialogCommands = n_pButtons + n_toggle + n_text + 1;
  for (i=0; i<n_optionMenu; i++)
    nDialogCommands += opMenuData[i].n_options;
  for (i=0; i<n_pullDownMenu; i++)
    nDialogCommands += pdMenuData[i].n_button;
  for (i=0; i<n_radioBoxes; i++)
    nDialogCommands += radioBoxData[i].n_options;

  if (nDialogCommands == 0)
  {
    *commands = NULL;
    return 0;
  }
  
  aString * com = new aString[nDialogCommands];
  
// pushbuttons, toggle buttons and textboxes
  for (i=0; i<n_pButtons; i++)
    com[ns++] = pButtons[i].buttonCommand;
  for (i=0; i<n_toggle; i++)
    com[ns++] = tButtons[i].buttonCommand;
  for (i=0; i<n_text; i++)
    com[ns++] = textBoxes[i].textCommand;
// option menus
  int m;
  for (m=0; m<n_optionMenu; m++)
  {
    for (i=0; i<opMenuData[m].n_options; i++)
      com[ns++] = opMenuData[m].optionList[i].buttonCommand;
  }
// pulldown menus
  for (m=0; m<n_pullDownMenu; m++)
  {
    for (i=0; i<pdMenuData[m].n_button; i++)
    {
      if (pdMenuData[m].type == GI_PUSHBUTTON)
	com[ns++] = pdMenuData[m].pbList[i].buttonCommand;
      else if (pdMenuData[m].type == GI_TOGGLEBUTTON)
	com[ns++] = pdMenuData[m].tbList[i].buttonCommand;
    }
  }
// radio boxes
  for (m=0; m<n_radioBoxes; m++)
  {
    for (i=0; i<radioBoxData[m].n_options; i++)
      com[ns++] = radioBoxData[m].optionList[i].buttonCommand;
  }
// exit command
  com[ns++] = exitCommand;
  
  *commands = com; // save the pointer to the array
  return nDialogCommands;
}

void * DialogData::
getWidget()
{
  return dialogWindow;
};

aString & DialogData::
getExitCommand()
{
  return exitCommand;
}

void DialogData::
setBuiltInDialog()
{
  builtInDialog = 1;
}

int DialogData::
getBuiltInDialog()
{
  return builtInDialog;
}

//\begin{>>DialogDataInclude.tex}{\subsection{getToggleValue}} 
bool DialogData::
getToggleValue( const aString & answer, const aString & label, bool & target )
// -----------------------------------------------------------------------------
// /Description: 
//    If `answer' requests a change in a toggle state then set `target' and adjust the
//    toggle state.
// /Return values: true if answer requested a change in a toggle state, return false oterwise.
// /Author: WDH
//\end{DialogDataInclude.tex}
// -----------------------------------------------------------------------------
{
  int len=0;
  if( len=str_matches(answer,label) )
  {
    int value = target;
    sScanF(answer.substr(len,answer.length()-len),"%i",&value); target=value;
    setToggleState(label,(int)target);

    return true;
  }

  return false;
}

// This version is for an int target
bool DialogData::
getToggleValue( const aString & answer, const aString & label, int & target )
{
  bool boolTarget;
  bool rt = getToggleValue(answer,label,boolTarget);
  if( rt )
    target=boolTarget;

  return rt;
}

//\begin{>>DialogDataInclude.tex}{\subsection{getTextValue(real)}} 
bool DialogData::
getTextValue( const aString & answer, const aString & label, const aString & format, real & target )
// -----------------------------------------------------------------------------
// /Description: 
//    If `answer' requests a change in a real text value with label=`label' 
//      then set `target' and adjust the
//    text label.
// /answer (input) : check this answer
// /label (input): check if answer is of the form "label ..."
// /target (output) : fill in this value if answer begins with "label"
// /format (input) : use this format to reset the text label field with a new value.
// /return value: true if found, false otherwise
// /Author: WDH
//\end{DialogDataInclude.tex}
// -----------------------------------------------------------------------------
{
  int len=0;
  if( len=str_matches(answer,label) )
  {
    aString line;
    sScanF(answer.substr(len,answer.length()-len),"%e",&target); 
    setTextLabel(label,sPrintF(line,format.c_str(),target));
    printF(" getTextValue: answer found: [%s %s]\n",(const char*)label.c_str(),(const char*)sPrintF(line,format.c_str(),target).c_str());
    return true;
  }
  return false;
}

//\begin{>>DialogDataInclude.tex}{\subsection{getTextValue(int)}} 
bool DialogData::
getTextValue( const aString & answer, const aString & label, const aString & format, int & target )
// -----------------------------------------------------------------------------
// /Description: 
//    If `answer' requests a change in a int text value with label=`label' 
//      then set `target' and adjust the
//    text label.
// /answer (input) : check this answer
// /label (input): check if answer is of the form "label ..."
// /target (output) : fill in this value if answer begins with "label"
// /format (input) : use this format to reset the text label field with a new value.
// /return value: true if found, false otherwise
// /Author: WDH
//\end{DialogDataInclude.tex}
// -----------------------------------------------------------------------------
{
  int len=0;
  if( len=str_matches(answer,label) )
  {
    aString line;
    sScanF(answer.substr(len,answer.length()-len),"%i",&target); 
    setTextLabel(label,sPrintF(line,format.c_str(),target));
    printF(" getTextValue: answer found: [%s %s]\n",(const char*)label.c_str(),(const char*)sPrintF(line,format.c_str(),target).c_str());
    return true;
  }
  return false;
}

//\begin{>>DialogDataInclude.tex}{\subsection{getTextValue(string)}} 
bool DialogData::
getTextValue( const aString & answer, const aString & label, const aString & format, aString & target )
// -----------------------------------------------------------------------------
// /Description: 
//    If `answer' requests a change in a string text value with label=`label' 
//      then set `target' and adjust the
//    text label.
// /answer (input) : check this answer
// /label (input): check if answer is of the form "label ..."
// /target (output) : fill in this value if answer begins with "label"
// /format (input) : use this format to reset the text label field with a new value.
// /return value: true if found, false otherwise
// /Author: WDH
//\end{DialogDataInclude.tex}
// -----------------------------------------------------------------------------
{
  int len=0;
  if( len=str_matches(answer,label) )
  {
    target=answer.substr(len,answer.length()-len);

    // removing leading blanks *wdh* 071209
    int i=0;
    while( i<target.length() && target[i]==' ' ) i++;
    //kkc 081217    target=target(i,target.length()-1);
    target=target.substr(i,target.length()-i);

    setTextLabel(label,target);
    printF(" getTextValue: answer found: [%s %s]\n",(const char*)label.c_str(),(const char*)target.c_str());
    return true;
  }
  return false;
}






//\begin{>>DialogDataInclude.tex}{\subsection{constructor}}
PullDownMenu::
PullDownMenu()
//-----------------------------------------------------
// /Description: default constructor
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  menupane=NULL;
  sensitive=true;
  n_button=0; 
  pbList=NULL; 
  tbList=NULL;
}

PullDownMenu::
~PullDownMenu()
// ----------------------------------------------------------------------
//  Added 030825 by wdh, to fix a leak
// ----------------------------------------------------------------------
{ 
  delete [] pbList; 
  delete [] tbList;
} 


//\begin{>>DialogDataInclude.tex}{\subsection{setPullDownMenu}}
bool PullDownMenu::
setPullDownMenu(const aString &pdMainLabel, const aString commands[], const aString labels[], button_type bt, 
		int *initState /* = NULL */)
//-----------------------------------------------------
// /Description: Fill in all fields of a pulldown menu object except menupane which will be set to NULL 
//   and sensitive which will be set to true.
//   This function can for example be used to setup the optionMenu argument to makeGraphicsWindow.
//
// /pdMainLabel(input): The label that will appear on the menu bar.
//
// /commands(input): An array of strings with the command that will be issued when each menu
//  item is selected. The array must be terminated by an empty string ("").
//
// /labels(input): An array of strings with the label that will be put on each menu
//  item. The array must be terminated by an empty string ("").
//
// /bt(input): The type of buttons in the menu. Can be either GI\_PUSHBUTTON or GI\_TOGGLEBUTTON.
//
// /initState(input): Optional argument that only is used when bt == GI\_TOGGLEBUTTON. This argument
//  is an array that specifies the initial state of each toggle buttons. If this argument is absent
//  when bt == GI\_TOGGLEBUTTON, no menu items are marked as beeing selected.
//
// /Return values: The function returns true on a successful completion and false if an error occured.
//
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  menupane = NULL;
  sensitive = true;
  
// commands and labels are supposed to be NULL-terminated arrays of char *.
// Start by counting the number of elements
  int ne;
  for (ne=0; commands[ne] != "" && labels[ne] != ""; ne++);
  if (ne == 0)
  {
    printF("PulldownMenu: WARNING: Empty list of commands. Not building an empty pulldown menu.\n");
    return false;
  }
// tmp
//  printF("PullDownMenu(): Found %i pulldown menu items\n", ne);
  
  n_button = ne;

  type = bt; // set the type
  
  if (bt == GI_PUSHBUTTON)
  {
    pbList = new PushButton [ne];

// copy all the strings
    int j;
    for (j=0; j<ne; j++)
    {
      pbList[j].buttonCommand = commands[j];
      pbList[j].buttonLabel = labels[j];
    }

  }
  else if (bt == GI_TOGGLEBUTTON)
  {
    tbList = new ToggleButton [ne];

// copy all the strings
    int j;
    for (j=0; j<ne; j++)
    {
      tbList[j].buttonCommand = commands[j];
      tbList[j].buttonLabel = labels[j];
// set the initial state
      if (initState)
	tbList[j].state = initState[j];
      else
	tbList[j].state = 0;
    }
  }
  
// copy the Title Label
  menuTitle = pdMainLabel;

  return true;
}


