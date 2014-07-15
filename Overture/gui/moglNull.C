// ************** This file has empty stubs for mogl functions *****************
//
//   This file can be used to link Overture without the X11 libraries.

#ifndef NO_APP
#include "OvertureDefine.h"
#include "OvertureTypes.h"
#else
#include "GUIDefine.h"
#include "GUITypes.h"
#endif

#ifndef OV_USE_DOUBLE
typedef float real;
#else
typedef double real;
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
// * #include <X11/StringDefs.h>
// * #include <X11/keysym.h>
// * #include <X11/IntrinsicP.h>
// * #include <X11/cursorfont.h>
// * #include <GL/gl.h>
#ifdef OV_USE_GL
#include <GL/glu.h>
#else
#include "nullgl.h"
#include "nullglu.h"
#endif
// * #include <GL/glx.h>
// * 
// * #include <Xm/BulletinB.h>
// * #include <Xm/CascadeB.h>
// * #include <Xm/Command.h>
// * #include <Xm/DialogS.h>
// * #include <Xm/FileSB.h>
// * #include <Xm/Form.h>
// * #include <Xm/Frame.h>
// * #include <Xm/Label.h>
// * #include <Xm/LabelG.h>
// * #include <Xm/List.h>
// * #include <Xm/MainW.h>
// * #include <Xm/MessageB.h>
// * #include <Xm/RowColumn.h>
// * #include <Xm/PushB.h>
// * #include <Xm/PushBG.h>
// * #include <Xm/Scale.h>
// * #include <Xm/SelectioB.h>
// * #include <Xm/Separator.h>
// * #include <Xm/Text.h>
// * #include <Xm/TextF.h>
// * #include <Xm/ToggleB.h>
// * #include <Xm/ToggleBG.h>

//#include "aString.H"
#ifndef NO_APP
#include "aString.H"
#else
#include <string>
#ifndef aString
#define aString std::string
#endif
#endif

#include "mogl.h"
#include "DialogData.h"


// Display the screen again immediately
// (see also moglPostDisplay)
void 
moglDisplay(int win){};

void 
moglGetWindowSize(int & width, int & height, int win ){};

void
moglOpenFileSB(char *pattern){};

void
moglCloseFileSB(){};

void
moglCreateMessageDialog(aString msg, MessageTypeEnum type){};

void
moglBuildUserButtons(const aString buttonCommand[], const aString buttonLabel[], int win_number){};

void
moglBuildUserMenu(const aString menuName[], const aString menuTitle, int win_number){};

void
moglSetSensitive(int win_number, int trueOrFalse){};

void
moglSetButtonSensitive(int win_number, int btn, int trueOrFalse){};

void
moglBuildPopup(const aString menu[]){};

void 
moglInit(int & argc, 
	 char *argv[], 
	 const aString &windowTitle, 
	 aString fileMenuItems[],
	 aString helpMenuItems[],
	 WindowProperties &wProp){};

int 
moglGetAnswer( aString &answer, const aString prompt, 
	       PickInfo *pick_, int blocking ){return 0;};


int 
moglGetMenuItem(const aString menu[], aString &answer, const aString prompt, 
		float *pickBox, int win_number ){return 0;};

// Display the screen the next time the event loop is entered
void 
moglPostDisplay(int win){};

// Define the functions that will display and resize the screen 
// (same function for all windows)
void
moglSetFunctions( GL_GraphicsInterface *giPointer,
                  MOGL_DISPLAY_FUNCTION displayFunc, 
                  MOGL_RESIZE_FUNCTION resizeFunc ){};
// set the prompt in the command window
void 
moglSetPrompt(const aString &prompt){};

void
moglAppendCommandHistory(const aString &item){};

// define the function that will be called when the rubber-band box
// is used to zoom in
void 
moglSetViewFunction( MOGL_VIEW_FUNCTION viewFunction ){};

int
makeGraphicsWindow(const aString &windowTitle, 
		   aString fileMenuItems[],
		   aString helpMenuItems[],
		   ClippingPlaneInfo & clippingPlaneInfo,
		   ViewCharacteristics & viewChar,
		   DialogData &hardCopyDialog,
		   PullDownMenu &optionMenu,
		   WindowProperties &wProp, 
                   int directRendering /* = 1 */  ){ return 0; };

int
destroyGraphicsWindow(int win_number){ return 0; };
int
moglMakeCurrent(int win){return 0;};
int
moglGetNWindows(){return 0;};
int
moglGetCurrentWindow(){return 0;};
int 
moglSetTitle(int win_number, const aString &windowTitle){return 0;};
void
moglPollEvents(){};

void 
moglPrintRotPnt(real x, real y, real z, int win_number){};

void 
moglPrintFractionOfScreen(real fraction, int win_number){};

void 
moglPrintLineWidth(real lw, int win_number){};

bool
moglRotationKeysPressed(int win_number){return false;};


// ----------- VIEWPORT interface -------------------------- **pf

void graphics_setFrustum      ( GLdouble left,   GLdouble right,
				GLdouble bottom, GLdouble top,
				GLdouble near,   GLdouble far){};

void graphics_setOrtho        ( GLdouble left,   GLdouble right,
			        GLdouble bottom, GLdouble top,
				GLdouble near,   GLdouble far){};

void graphics_setOrthoKeepAspectRatio( GLdouble aspectRatio, GLdouble magFactor,
				       GLdouble left,   GLdouble right,
				       GLdouble bottom, GLdouble top,
				       GLdouble near,   GLdouble far){};

void graphics_setPerspective ( GLdouble fovy,   GLdouble aspect,
			       GLdouble near,   GLdouble far){};



// ******************************* GUI functions that call X ********************************


// default constructor
PickInfo::
PickInfo()
{
  pickType = 0; // 1 means Button1, 2 means Button2, and 3 means Button3
  pickWindow = -99;
  pickBox[0] = 0; // xmin
  pickBox[1] = -1;// xmax
  pickBox[2] = 0; // ymin
  pickBox[3] = -1;// ymax
};



// Some member functions in the dialogData class (these functions use static variables 
// from this file, which prevents them from beeing in DialogData.C


//\begin{>PushButtonInclude.tex}{\subsection{setSensitive}} 
void PushButton::
setSensitive(bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of a PushButton object.
// /trueOrFalse: The new state of the PushButton widget
// /Return valuse: None
// /Author: AP \& WDH
//\end{PushButtonInclude.tex}
//------------------------
{ 
}  

//\begin{>ToggleButtonInclude.tex}{\subsection{setSensitive}} 
void ToggleButton::
setSensitive(bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of a ToggleButton object.
// /trueOrFalse: The new state of the ToggleButton widget
// /Return valuse: None
// /Author: AP \& WDH
//\end{ToggleButtonInclude.tex}
//------------------------
{ 
}  

//\begin{>TextLabelInclude.tex}{\subsection{setSensitive}} 
void TextLabel::
setSensitive(bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of a TextLabel object.
// /trueOrFalse: The new state of the textlabel widget
// /Return valuse: None
// /Author: AP \& WDH
//\end{TextLabelInclude.tex}
//------------------------
{ 
}  

//\begin{>OptionMenuInclude.tex}{\subsection{setSensitive}} 
void OptionMenu::
setSensitive(bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of a OptionMenu object.
// /trueOrFalse: The new state of the OptionMenu widget
// /Return valuse: None
// /Author: AP \& WDH
//\end{OptionMenuInclude.tex}
//------------------------
{ 
}  

//\begin{>OptionMenuInclude.tex}{\subsection{setSensitive}} 
void OptionMenu::
setSensitive(int btn, bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of one button in a OptionMenu object.
// /btn: The button number in the option menu.
// /trueOrFalse: The new state of the push button widget
// /Return valuse: None
// /Author: AP
//\end{OptionMenuInclude.tex}
//------------------------
{ 
}  

//\begin{>PullDownMenuInclude.tex}{\subsection{setSensitive}} 
void PullDownMenu::
setSensitive(bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of a PullDownMenu object.
// /trueOrFalse: The new state of the PullDownMenu widget
// /Return valuse: None
// /Author: AP \& WDH
//\end{PullDownMenuInclude.tex}
//------------------------
{ 
}  

//\begin{>>DialogDataInclude.tex}{\subsection{setSensitive}} 
void DialogData::
setSensitive(int trueFalse)
//------------------------
// /Description: Set the sensitivity of a DialogData object.
// /trueOrFalse: The new state of the DialogData widget
// /Return valuse: None
// /Author: AP \& WDH
//\end{DialogDataInclude.tex}
//------------------------
{
}


//\begin{>>DialogDataInclude.tex}{\subsection{setSensitive}} 
void DialogData::
setSensitive(bool trueOrFalse, WidgetTypeEnum widgetType, int number )
// ======================================================================================================
// /Description:
//    Set the sensitivity of a widget in the DialogData
// /trueOrFalse (input): set senstive or not
// /widgetType (input): choose a widget type to assign. One of 
// \begin{verbatim}
//  enum WidgetTypeEnum  
//  {
//    optionMenuWidget,
//    pushButtonWidget,
//    pullDownWidget,
//    toggleButtonWidget,
//    textBoxWidget,
//    radioBoxWidget
// };
// \end{verbatim}
// /number (input) : set sensitivity for this widget. 
//\end{DialogDataInclude.tex}
// ======================================================================================================
{
}

//\begin{>>DialogDataInclude.tex}{\subsection{setSensitive}}
void DialogData::
setSensitive(bool trueOrFalse, WidgetTypeEnum widgetType, const aString & label)
// ======================================================================================================
// /Description:
//    Set the sensitivity of a widget in the DialogData
// /trueOrFalse (input): set senstive or not
// /widgetType (input): choose a widget type to assign. One of 
// \begin{verbatim}
//  enum WidgetTypeEnum  
//  {
//    optionMenuWidget,
//    pushButtonWidget,
//    pullDownWidget,
//    toggleButtonWidget,
//    textBoxWidget,
//    radioBoxWidget
// };
// \end{verbatim}
// /label (input) : set sensitivity for the widget with this label
//\end{DialogDataInclude.tex}
// ======================================================================================================
{
}


void DialogData::
closeDialog()
{
}

void DialogData::
openDialog(int managed /* = 1*/)
//
//  /Purpose:
//  Call MOTIF to create a generic dialog window
//  /Author: AP
//-----------------------------------------------------
{
}

//\begin{>>DialogDataInclude.tex}{\subsection{changeOptionMenu}} 
bool DialogData::
changeOptionMenu(int nOption, const aString opCommands[], const aString opLabels[], int initCommand)
//-----------------------------------------------------
// /Description: Change the menu items in an option menu after it has been created (by pushGUI)
//
// /nOption(input): Change option menu \# nOption.
// /opCommands(input): An array of strings with the command that will be issued when each menu
//  item is selected. The array must be terminated by an empty string ("").
// /opLabels(input): An array of strings with the label that will be put on each menu
//  item. The array must be terminated by an empty string ("").
// /initCommand(input): The index of the initial selection in the opLabels array. This label will
//  appear on top of the option menu to indicate the initial setting.
// /Return values: The function returns true on a successful completion and false if an error occured.
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  return false;
}

//\begin{>>DialogDataInclude.tex}{\subsection{showSibling}} 
int DialogData::
showSibling()
//----------------------------------------------
// /Description: Show a sibling (dialog) window that previously was allocated with 
// getDialogSibling() and created with pushGUI().
//
// /Returnvalues: The function returns 1 if the sibling could be shown, otherwise 0 
// (in which case it doesn't exist or already is shown).
// /Author: AP
//\end{DialogDataInclude.tex}
//----------------------------------------------
{
  return 0;
}


//\begin{>>DialogDataInclude.tex}{\subsection{hideSibling}} 
int DialogData::
hideSibling() 
//----------------------------------------------
// /Description: Hide a sibling (dialog) window that previously was allocated with 
// getDialogSibling(), created with pushGUI() and shown with showSibling().
//
// /Returnvalues: The function returns 1 if the sibling could be hidden, otherwise 0 
// (in which case it doesn't exist or already is hidden).
// /Author: AP
//\end{DialogDataInclude.tex}
//----------------------------------------------
{
  return 0;
}

//\begin{>>DialogDataInclude.tex}{\subsection{setTextLabel}} 
int DialogData::
setTextLabel(int n, const aString &buff)
//----------------------------------------------------
// /Description: Set the text string in textlabel \# n in the currently
// active GUIState.
//
// /n(input): The index of the text label in the array given to setTextBoxes during setup.
// /buff(input): The new text string.
//
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  return 0;
}

//\begin{>>DialogDataInclude.tex}{\subsection{setInfoLabel}} 
bool DialogData::
setInfoLabel(int n, const aString &buff)
//----------------------------------------------------
// /Description: Set the text string in info label \# n in the currently
// active GUIState.
//
// /n(input): The index of the text label returned by addInfoLabel during the setup.
// /buff(input): The new text string.
//
// /Return code: true if the label could be changed successfully, otherwise false
//
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  return false;
}

//\begin{>>DialogDataInclude.tex}{\subsection{setToggleState}} 
int DialogData::
setToggleState(int n, int trueFalse)
//----------------------------------------------------
// /Description: Set the state of toggle button \# n in the currently
// active GUIState.
//
// /n(input): The index of the toggle button in the array given to setToggleButtons during setup.
// /trueFalse(input): trueFalse==1 turns the toggle button on, all other values turn it off.
//
// /Author: AP
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  return 0;
}
// end of DialogData Class functions

//\begin{>>DialogDataInclude.tex}{\subsection{setToggleState}} 
int DialogData::
setToggleState( const aString & toggleButtonLabel,  int trueOrFalse)
//----------------------------------------------------
// /Description: Set the toggle state for the toggle button with the given label.
//
// /toggleButtonLabel(input): The label of the toggle button to set.
// /trueOrFalse(input): The new state.
//
// /Author: wdh
//\end{DialogDataInclude.tex}
//-----------------------------------------------------
{
  return 0;
}



void DialogData::
setCursor(cursorTypeEnum c)
{
}




//\begin{>>ToggleButtonInclude.tex}{\subsection{setState}} 
int ToggleButton::
setState(bool trueOrFalse )
// =================================================================================
// /Description:
//   Set the state of a Toggle button
// /trueOrFalse:
//\end{ToggleButtonInclude.tex}
// =================================================================================
{
  return 0;
}


//\begin{>>PullDownMenuInclude.tex}{\subsection{setToggleState}} 
int PullDownMenu::
setToggleState(int n, bool trueOrFalse )
// =================================================================================
// /Description:
//   Set the state of a Toggle button in a pulldown menu.
// /trueOrFalse:
//\end{PullDownMenuInclude.tex}
// =================================================================================
{
  return 1;
}

// void
// showHardcopyDialog(Widget pb, XtPointer client_data, XtPointer call_data)
// {
// }


//\begin{>>OptionMenuInclude.tex}{\subsection{setCurrentChoice}} 
int OptionMenu::
setCurrentChoice(int command)
// ====================================================================================
// /Description:
//    Set the current choice for an option menu. 
//\end{OptionMenuInclude.tex}
// ===================================================================================
{
  return 0;
}

//\begin{>>OptionMenuInclude.tex}{\subsection{setCurrentChoice}} 
int OptionMenu::
setCurrentChoice(const aString & label)
// ====================================================================================
// /Description:
//    Set the current choice for an option menu. 
//\end{OptionMenuInclude.tex}
// ===================================================================================
{
  return 1;
}

  
//\begin{>RadioBoxInclude.tex}{\subsection{setCurrentChoice}} 
bool RadioBox::
setCurrentChoice(int command)
// ====================================================================================
// /Description:
//    Set the current choice for a radio box. 
// /command(input): The command to be chosen
// /Return value: true if the command could be chosen, otherwise false. A command cannot
// be chosen if it is insensitive or out of bounds.
// /Author: AP
//\end{RadioBoxInclude.tex}
// ===================================================================================
{
  return true;
}

//\begin{>>RadioBoxInclude.tex}{\subsection{setSensitive}} 
void RadioBox::
setSensitive(bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of a RadioBox object.
// /trueOrFalse: The new state of the RadioBox widget
// /Return valuse: None
// /Author: AP
//\end{RadioBoxInclude.tex}
//------------------------
{ 
}  

//\begin{>>RadioBoxInclude.tex}{\subsection{setSensitive}} 
void RadioBox::
setSensitive(int btn, bool trueOrFalse)
//------------------------
// /Description: Set the sensitivity of one button in a RadioBox object.
// /btn: The button number in the radio box.
// /trueOrFalse: The new state of the toggle button widget
// /Return valuse: None
// /Author: AP
//\end{RadioBoxInclude.tex}
//------------------------
{ 
}  

