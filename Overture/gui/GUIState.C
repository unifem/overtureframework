#include "GL_GraphicsInterface.h"
#include "GUIState.h"

#ifdef NO_APP
using GUITypes::real;
#endif

// default constructor for GUIState
//\begin{>GUIStateInclude.tex}{\subsection{Constructor}} 
GUIState::
GUIState()
//
// /Description: Default constructor.
// /Author: AP
//\end{GUIStateInclude.tex}
//-----------------------------------------------------
{
//  cout << "Default constructor called for GUIState" << endl;
  
  gl = NULL;
  nPopup = 0;
  popupMenu = NULL;

  nWindowButtons = 0;
  windowButtonCommands = NULL;
  windowButtonLabels = NULL;

  nPulldown = 0;
  pulldownTitle = "";
  pulldownCommand = NULL;

  prev = NULL;

  nAllCommands=0;
  allCommands=NULL;
  
  nDialogSiblings=0;
}

// destructor
GUIState::
~GUIState()
{
  if (popupMenu) delete [] popupMenu;
  if (windowButtonCommands) delete [] windowButtonCommands;
  if (windowButtonLabels) delete [] windowButtonLabels;
  if (pulldownCommand) delete [] pulldownCommand;
  if (allCommands) delete [] allCommands;
}

//\begin{>>GUIStateInclude.tex}{\subsection{setUserMenu}} 
void GUIState::
setUserMenu(const aString menu[], const aString & menuTitle)
//-----------------------------------------------------
// /Description:
//    Sets up a user defined pulldown menu in the graphics windows. The menu will
//    appear after the routine pushGUI() has been called.
// /menu (input):
//    The {\ff menu} is
//    an array of aStrings (the menu choices) with an empty aString
//    indicating the end of the menu choices. If menu == NULL, any existing user defined 
//    menu will be removed.
// /menuTitle (input): A aString with the menu title that will appear on the menu bar. Note 
//    that a menuTitle must be provided even when menu == NULL.
// /Return value: none.
// /Author: AP
//\end{GUIStateInclude.tex}
//-----------------------------------------------------
{
  if (!menu)
  {
    nPulldown = 0;
    pulldownTitle = "";
    if (pulldownCommand) delete [] pulldownCommand;
    pulldownCommand = NULL;
    return;
  }
  
// count the number of menu items
  int i;
  for (i=0; menu[i].length() != 0; i++);
  nPulldown = i;

// delete any existing pulldown menu
  if (pulldownCommand) delete [] pulldownCommand;
  pulldownCommand = NULL;
  
// copy all strings
  if (nPulldown)
  {
    pulldownCommand = new aString[nPulldown+1];
    for (i=0; i<=nPulldown; i++) // copy the trailing "" also!!!
      pulldownCommand[i] = menu[i];
  }
  
  pulldownTitle = menuTitle;
  
  
}

//\begin{>>GUIStateInclude.tex}{\subsection{setUserButtons}} 
void GUIState::
setUserButtons(const aString buttons[][2])
//-----------------------------------------------------
// /Description:
//    This function builds user defined push buttons in the graphics windows. The buttons will
//    appear after the routine pushGUI() has been called.
// /buttons (input): A two-dimensional array of Strings, terminated by an empty aString.
//    For example,
//    \begin{verbatim}
//    aString buttons[][2] = {{"plot shaded surfaces", "Shade"}, 
//                           {"erase",                "Erase"},
//                           {"exit",                 "Exit"},
//                           {"",                     ""}};
//    \end{verbatim}
// The first entry in each row is the aString that will be passed as a command when the 
// button is pressed. The second aString in each row is the name of the button that will
// appear on the graphics window. There can be at most MAX\_BUTTONS buttons, where 
// MAX\_BUTTONS is defined in mogl.h, currently to 15.
//
// If buttons == NULL, any existing buttons will be removed 
// from the current window.
//  
//
// /Return value: none.
// /Author: AP
//\end{GUIStateInclude.tex}
// =================================================================================
{
  if (!buttons)
  {
    nWindowButtons = 0;
    if (windowButtonCommands) delete [] windowButtonCommands;
    if (windowButtonLabels)   delete [] windowButtonLabels;
    windowButtonCommands = NULL;
    windowButtonLabels   = NULL;
    return;
  }
  
// count the number of menu items
  int i;
  for (i=0; buttons[i][0].length() != 0 && buttons[i][1].length() != 0; i++);
  nWindowButtons = i;

// delete any existing strings
  if (windowButtonCommands) delete [] windowButtonCommands;
  if (windowButtonLabels)   delete [] windowButtonLabels;
  windowButtonCommands = NULL;
  windowButtonLabels   = NULL;
  
// copy all strings
  if (nWindowButtons)
  {
    windowButtonCommands = new aString[nWindowButtons+1];
    windowButtonLabels   = new aString[nWindowButtons+1];
    for (i=0; i<=nWindowButtons; i++) // copy the trailing "" also!
    {
      windowButtonCommands[i] = buttons[i][0];
      windowButtonLabels[i]   = buttons[i][1];
    }
  }

}

//\begin{>>GUIStateInclude.tex}{\subsection{buildPopup}} 
void GUIState::
buildPopup(const aString menu[])
//-----------------------------------------------------
// /Description:
//    Sets up a user defined popup menu in all graphics windows and in the command window. 
//    The menu will appear after the routine pushGUI() has been called.
// /menu (input): 
//    The {\ff menu} is
//    an array of Strings (the menu choices) with an empty aString
//    indicating the end of the menu choices. Optionally, a title can be put on top of the menu by 
//    starting the first aString with an `!'. For example,
//    \begin{verbatim}
//       PlotStuff ps;
//       aString menu[] = { "!MenuTitle",
//                         "plot",
//                         "erase",
//                         "exit",
//                         "" };
//       aString menuItem;
//       int i=ps.getMenuItem(menu,menuItem);
//    \end{verbatim}
//
//  To create a cascading menu, begin the string with an '$>$'.
//  To end the cascade begin the string with an '$<$'.
//  To end a cascade and start a new cascade, begin the string with '$<$' followed by '$>$'.
//  Here is an example:
//    \begin{verbatim}
//        char *menu1[] = {  "!my title",
//                           "plot",
//                           ">component",
//                                        "u",
//                                        "v",
//                                        "w",
//                           "<erase",
//                           ">stuff",
//                                    "s1",
//                                    ">more stuff", 
//                                                  "more1",
//                                                  "more2", 
//                                    "<s2", 
//                          "<>apples", 
//                                    "apple1", 
//                          "<exit",
//                          NULL };  
//    \end{verbatim}
//
// /Return value: none.
// /Author: AP
//\end{GUIStateInclude.tex}
//-----------------------------------------------------
{
  if (!menu)
  {
    nPopup = 0;
    if (popupMenu) delete [] popupMenu;
    popupMenu = NULL;
//    moglBuildPopup( popupMenu );
    return;
  }
  
// count the number of menu items
  int i;
  for (i=0; menu[i].length() != 0; i++);
  nPopup = i;

// delete any existing strings
  if (popupMenu) delete [] popupMenu;
  popupMenu = NULL;
  
// copy all strings
  if (nPopup)
  {
    popupMenu = new aString[nPopup+1];
    for (i=0; i<=nPopup; i++) // copy the trailing "" also!
      popupMenu[i] = menu[i];
  }
  
}

void GUIState::
setAllCommands()
{
  aString *dc, **sc;
  int *nSib;
// save all commands in one array, add one for the exit command
  nAllCommands = nPopup + nPulldown + nWindowButtons; 
  int i, j;
  
// copy all commands from the main dialog:
  int nDia = dialogCommands( &dc );
  nAllCommands += nDia;
// allocate space for sibling commands
  if (nDialogSiblings > 0)
  {
    sc = new aString * [nDialogSiblings];
    nSib = new int [nDialogSiblings];
  }
  
// Add all commands in the sibling windows 
  for (j=0; j<nDialogSiblings; j++)
  {
    nSib[j] = dialogCommands( &sc[j] );
    nAllCommands += nSib[j];
  }

// allocate
  allCommands = new aString[nAllCommands+1]; // add one for the terminating ""
//  printf("pushGUI: allocating allcommands %x\n", allCommands);
  
// copy strings
  int ns=0;
  for (i=0; i<nPopup; i++)
// remove all items starting with !
//    if (popupMenu[i][0] != '!')
      allCommands[ns++] = popupMenu[i]; 
  for (i=0; i<nPulldown; i++)
    allCommands[ns++] = pulldownCommand[i];
  for (i=0; i<nWindowButtons; i++)
    allCommands[ns++] = windowButtonCommands[i];
  for (i=0; i<nDia; i++)
    allCommands[ns++] = dc[i];

// sibling commands
  for (j=0; j<nDialogSiblings; j++)
  {
    for (i=0; i<nSib[j]; i++)
      allCommands[ns++] = sc[j][i];
  } // end for all siblings
  
// terminating ""
  allCommands[ns++] = "";
// if the popupMenu contained items starting with '!', there is a mismatch in the count
  nAllCommands = ns-1;

// cleanup strings allocated in dialogCommands() and the pointers allocated locally.
  if (dc) delete [] dc;
  if (nDialogSiblings > 0)
  {
    for (j=0; j<nDialogSiblings; j++)
      if (sc[j]) delete [] sc[j];
    delete [] sc;
    delete [] nSib;
  }
  
}

//\begin{>>GUIStateInclude.tex}{\subsection{getDialogSibling}} 
DialogData & GUIState::
getDialogSibling(int number /* =-1 */ )
//----------------------------------------------
// /Description: If number==-1 (default), allocate a sibling (dialog) window, otherwise return
//   sibling \# 'number'. Note that the sibling window
// will appear on the screen after pushGUI() has been called for this (GUIState) object 
// and showSibling() has been called for the DialogData object returned from this function.
// See the DialogData function description for an example.
//
// /number (input): by default return a new sibling (if number==-1), otherwise return the
//    sibling specified by number.
// /Returnvalues: The function returns an alias to the DialogData object. There 
//  is currently space for 10 siblings (0,...,9) for each GUIState object.
//
// /Author: AP
//\end{GUIStateInclude.tex}
//----------------------------------------------
{
  if( number>=0 )
  {
    if( number <  nDialogSiblings )
      return dialogSiblings[number];
    else
    {
      printf("ERROR: getDialogSibling: There is no sibling numbered %i!\n",number);
      return dialogSiblings[0];
    }
  }
  else if (nDialogSiblings < MAX_SIBLINGS)
    return dialogSiblings[nDialogSiblings++];
  else
  {
    printf("GUIState::getDialogSibling:ERROR: Too many sibling dialog windows open!\n");
    printf("      MAX_SIBLINGS=%i, Increase MAX_SIBLINGS in GuiState.h and recompile\n",MAX_SIBLINGS);
    return dialogSiblings[MAX_SIBLINGS-1];
  }
}



int GUIState::
addPrefix(const aString label[], const aString & prefix, aString cmd[], const int maxCommands)
// ==============================================================================================
// /Description:
//    Add a prefix string to the start of every label.
// /label (input) : null terminated array of strings.
// /prefix (input) : all this string as a prefix.
// /cmd (input/output): on output cmd[i]=prefix+label[i];
// /maxCommands (input): maximum number of strings in the cmd array.
// /Author: WDH
// ==============================================================================================
{
    
  int i;
  for( i=0; i<maxCommands && label[i]!=""; i++ )
    cmd[i]=prefix+label[i];
  if( i<maxCommands )
    cmd[i]="";
  else
  {
    printF("GUIState::addPrefix:ERROR: maxCommands=%i is too small\n",maxCommands);
    assert( maxCommands>0 );
    // cmd[maxCommands-1];
    return 1;
  }
  return 0;
}
