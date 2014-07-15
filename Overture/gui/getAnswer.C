#include "GL_GraphicsInterface.h"
#ifndef NO_APP
#include "broadCast.h"
#endif
#include <string.h>
#include "mogl.h"
// #include <GL/gl.h>
// #include <GL/glu.h>
#include "xColours.h"

#ifdef NO_APP
using GUITypes::real;
#define redim resize
#define getBound(x) size((x))-1
#define getLength size

using std::cout;
using std::endl;
#endif

#include "OvertureParser.h"

// macro for casting aStrings to char * without getting a memory leak
#define SC (char *)(const char *)

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getAnswer}} 
int GL_GraphicsInterface::
getAnswer(aString & answer, const aString & prompt)
//=====================================================================================
// /Description:
//  Wait for an answer to be issued by the user. If the program is reading commands from 
//  the GUI, this routine will return after the user has issued a command from the popup 
//  menu, the pulldown menu or the push buttons on the graphics window, or from any of 
//  the buttons or menus in the dialog window. If the program is reading commands from a
//  file, the next line of the file not starting with '*' or a '\#' is returned.
//
//  See the routines pushGUI and popGUI as well as the functions in the GUIState class
//  for instructions on how to setup the graphical user interface (GUI).
//
// /answer(output): The string issued by the GUI or read from the command file.
// /prompt(input): A prompt used by the GUI.
// /Return Values: On return, "answer" is set equal to the 
//    menu item chosen. The function return value is set equal to
//    the number of the item chosen, starting from zero. The items are the union of the
//    popup menus, pulldown menus, buttons on graphics windows, and all items on the 
//    current dialog window.
//
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
//=====================================================================================
{
  return getAnswerSelectPick(answer, prompt, NULL, 1);
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getAnswer with selection}} 
int GL_GraphicsInterface::
getAnswer(aString & answer, const aString & prompt, SelectionInfo &selection)
//=====================================================================================
// /Description: In addition to the functionality in the basic getAnswer() function,
//  this routine can also return a selection that is made by the user, or read from the
//  command file. 
//
//  The SelectionInfo object contains the following information:
// \begin{verbatim}
//  class SelectionInfo
//  {
//  public:
//  IntegerArray selection;
//  int nSelect;
//  int active; 
//  real r[4]; 
//  real zbMin;
//  real x[3];  
//  int globalID;
//  int winNumber;
//  };
// \end{verbatim}
// If the user picks a point or a region anywhere in a graphics window (with the left mouse
// button while holding down the CONTROL key), {\tt active} will be set to 1 and the window 
// coordinates will be saved in {\tt r[4]} according to r[0]: rMin (horizontal window coordinate),
// r[1]: rMax, r[2]: sMin (vertical window coordinate), r[3]: sMax.
//
// If the pick was made on one or several objects, the closest z-buffer value is stored in
// {\tt zbMin} and the corresponding 3--D coordinates are saved in {\tt x[3]}. The global 
// ID number of the closest object is saved in {\tt globalID} and the window number where the 
// picking occured is saved in {\tt winNumber}.
//
// Furthermore, {\tt nSelect} contains
// the number of objects that were selected and the array selection(nSelect,3) will contain 
// information about what was selected: 
//
// /selection(i,0): globalID of object \# i, 
// /selection(i,1): front z-buffer value of object \# i,
// /selection(i,2): back z-buffer value of object \# i.
//
// Note that {\tt active} will be 1 and {\tt nSelect}=0 if the user picks outside all objects
// in the graphics window.
//
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
//=====================================================================================
{
  return getAnswerSelectPick(answer, prompt, &selection, 1);
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getAnswerNoBlock}} 
int GL_GraphicsInterface::
getAnswerNoBlock(aString & answer, const aString & prompt)
//=====================================================================================
// /Description:
//  This is a non-blocking version of getAnswer(), i.e., if no events are pending, it will return
//  without any answer. Note that if a command file is open, this routine works in the same way
//  as the standard (blocking) getAnswer(), except when the file ends. In that case this routine
//  will only return an answer if an event was pending before the file ended.
//
//  See the routines pushGUI and popGUI as well as the functions in the GUIState class
//  for instructions on how to setup the graphical user interface (GUI).
//
// /answer(output): The string issued by the GUI or read from the command file.
// /prompt(input): A prompt used by the GUI.
// /Return Values: If no button, menu, popup, etc., was chosen
//  since last time a getAnswer routines was called, {\bf answer} will be set to "" and the 
//  return value will be 0. Otherwise, "answer" is set equal to the 
//  return string assigned by the callback function. The function return value is set equal to
//    the number of the item chosen, starting from zero. The items are the union of the
//    popup menus, pulldown menus, buttons on graphics windows, and all items on the 
//    current dialog window.
//
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
//=====================================================================================
{
  return getAnswerSelectPick(answer, prompt, NULL, 0);
}

int GL_GraphicsInterface::
getAnswerSelectPick(aString & answer, const aString & prompt,
		    SelectionInfo * selection_ /* = NULL */, 
                    int blocking /* = 1 */ )
{

#ifdef USE_PPP
  const int myid=max(0,Communication_Manager::My_Process_Number);
#endif

  int returnValue=0;
  bool done=FALSE;
  // int menuSelected;
  aString msg;

  // if( !graphicsWindowIsOpen ) //  this can occur when running in text mode
  if( !isInteractiveGraphicsOn() )  // *wdh* 070822 
  {
    return promptAnswerSelectPick(answer, prompt, selection_);  
  }
  

// initialize
//    if (pick_)
//      pick_->active = 0;

  if( selection_ )
  {
    selection_->nSelect = 0;
    selection_->active=0;     // *wdh* 010217
  }
  
#ifndef NO_APP
  if( Communication_Manager::localProcessNumber()==processorForGraphics ) // P++ stuff?
#endif
  {
    // Note: a "pause" command will get getAnswer to be called again -- we need to set singleProcessorMode=true
    bool singleProcessorGraphicsModeSave = singleProcessorGraphicsMode;
    setSingleProcessorGraphicsMode(true);   // this avoids communication in the graphics getAnswer etc.

    while( !done ) //loop here until all special commands have been parsed and a user-defined command is found
    {
      // We can read an answer either from a command file or the stringCommands queue
      // ***NOTE: the nearly same section of code appears in textGetMenu.C:promptAnswerSelectPick

      const bool readStringCommands = !stringCommands.empty();
      bool readAnswer = readFile || readStringCommands;
      if( readAnswer )
      {
        if( !getInteractiveResponse )
	{
	  returnValue = fileAnswer(answer, prompt, selection_); 
	  // if the file ends inside fileAnswer, readFile will be closed and readFile will be set to NULL       
	  readAnswer = readFile || readStringCommands;  // recompute this, the command file may have ended
	}
	else
	{ // force an interactive response (open the GUI too for a pause)
          readAnswer=false;
	}
	if (!readAnswer) openGUI(); // need to open the GUI if the file ended
      }
      // if the file ends inside fileAnswer, readFile will be closed and readFile will be set to NULL
      if( !readAnswer )
      { 
	returnValue = interactiveAnswer(answer, prompt, selection_, blocking);
	// printf("GL_G: returnValue=%i, answer = %s \n",returnValue,(const char*)answer );
      }

      // optionally parse the command (read another answer if the parser returns a nonzero)
      if( useParser && parseAnswer(answer)!=0 )
	continue;
    
      if( returnValue < 0 ) // a negative return value means that answer is a built in command
      {
        // processSpecialMenuItems returns 1 if answer was a "special" built in command, otherwise returns 0.
        // Hence, done will be TRUE if processSpecial... returns 0.
	done = !processSpecialMenuItems(answer);   // this will save in the log file if necessary
	if( done ) // ***** missing short forms for special commands "read command file" ...
	{
	  // answer may be a short form for a menu entry ****
	  // strip off leading blanks 

	  aString line=answer;
          int i;
	  for( i=0; i<line.length() && (line[i]==' ' || line[i]=='\t'); i++) {}
	  if( i>0 )
	    answer=line.substr(i,line.length()-i); //kkc no_app

	  returnValue=getMatch(currentGUI->allCommands, answer);   // find the best match, if any
	  done=TRUE; // redundant
	}      
      }
      else
	done=TRUE;

// save all non-special commands here
// we have already saved in the saveFile if readFile==TRUE
      if( done && saveFile && !readFile && answer.substr(0,20)!="log commands to file" && //kkc no_app
          ( savePick || answer.substr(0,5)!="mogl-") )  //kkc no_app
      {
	if (answer.length()>0)
	  fPrintF(saveFile,"%s\n",(const char*)(indentBlanks+answer).c_str());
        if( true ) // ++saveFileCount > 10 )
	{
	  saveFileCount=0;
          fflush(saveFile);  // flush the log file
          if( echoFile )
            fflush(echoFile);  // flush the echo file
	}
      }
      
    }  // end while(!done)

    setSingleProcessorGraphicsMode(singleProcessorGraphicsModeSave);   // reset

  }
#ifdef USE_PPP
  if( !singleProcessorGraphicsMode ) // no collection operations are allowed when in this mode
  {
    // we could reduce the number of messages here but probably doesn't matter

    broadCast(answer,processorForGraphics);

    if( false )
    {
      printf("getAnswer: myid=%i, answer=[%s]\n",myid,(const char*)answer);
      fflush(0);
      MPI_Barrier(MPI_COMM_WORLD); // **************
    }

    broadCast(returnValue,processorForGraphics);
    // We need to let all processors know if graphicsPlottingIsOn has changed
    broadCast(graphicsPlottingIsOn,processorForGraphics);

    // *wdh* 090809 -- the command file will only be closed on the processorForGraphics so
    // we need to check if it has been closed there and then close on all processors.
    bool commandFileIsOpen = readingFromCommandFile();
    broadCast(commandFileIsOpen,processorForGraphics);
    if( !commandFileIsOpen && readFile!=NULL )
    {
      stopReadingCommandFile();
    }
    

    // We need to broadcast the selection info too! 
    if( selection_!=NULL )
    {

      SelectionInfo & s = *selection_;
      
      // class SelectionInfo is defined in GUIState.h
      //   IntegerArray selection; 
      //   int nSelect; // number of objects that were selected
      //   int active; 
      //   GUITypes::real r[4];  // window coordinates of the pickBox. r[0]: rMin, r[1]: rMax, r[2]: sMin, r[3]: sMax
      //   GUITypes::real zbMin; // z-buffer value of the closest object.
      //   GUITypes::real x[3];  
      //   int globalID; // global ID of the closest object. Only defined if nSelect > 0.
      //   int winNumber; // window number where the picking occured

      // we first broad-cast the number of elements in the s.selection array
      int bounds[2]={0,0}; 
      if( myid==processorForGraphics )
      {
        bounds[0] = s.selection.getLength(0);
        bounds[1] = s.selection.getLength(1);
      }

      MPI_Bcast( bounds, 2, MPI_INT, processorForGraphics, MPI_COMM_WORLD); 
      
      if( false )
      {
	printf("getAnswer: broadcast selection info, myid=%i, bounds=[%i,%i]\n",myid,bounds[0],bounds[1]);
	fflush(0);
	MPI_Barrier(MPI_COMM_WORLD); // **************
      }
      
      const int numi = bounds[0]*bounds[1] + 4; // number of int's to send.
      int * iBuff = new int [numi];
      const int numr=8;                   // number of real's to send.
      real *rBuff = new real [numr];
      if( myid==processorForGraphics )
      {
        // *** pack the buffers ***
        int k=0;
	for( int i=0; i<bounds[0]; i++ )for( int j=0; j<bounds[1]; j++ )
	{
	  iBuff[k]=s.selection(i,j); k++;
	}
	iBuff[k]=s.nSelect;   k++;
	iBuff[k]=s.active;    k++;
	iBuff[k]=s.globalID;  k++;
	iBuff[k]=s.winNumber; k++;
	assert( k==numi );
	
        k=0;
	for( int j=0; j<4; j++ ){ rBuff[k]=s.r[j]; k++; } // 
        rBuff[k]=s.zbMin; k++;
	for( int j=0; j<3; j++ ){ rBuff[k]=s.x[j]; k++; } // 
	assert( k==numr );

      }
      MPI_Bcast( iBuff, numi, MPI_INT, processorForGraphics, MPI_COMM_WORLD);
      MPI_Bcast( rBuff, numr, MPI_Real, processorForGraphics, MPI_COMM_WORLD);

      if( false )
      {
	MPI_Barrier(MPI_COMM_WORLD); // **************
      }

      if( myid!=processorForGraphics )
      {
	if( bounds[0]>0 )
          s.selection.redim(bounds[0],bounds[1]);
	else
	  s.selection.redim(0);

	// *** unpack the buffers ***

        int k=0;
	for( int i=0; i<bounds[0]; i++ )for( int j=0; j<bounds[1]; j++ )
	{
	  s.selection(i,j)=iBuff[k]; k++;
	}
	s.nSelect=iBuff[k];   k++;
	s.active=iBuff[k];    k++;
	s.globalID=iBuff[k];  k++;
	s.winNumber=iBuff[k]; k++;
	assert( k==numi );
	
        k=0;
	for( int j=0; j<4; j++ ){ s.r[j]=rBuff[k]; k++; } // 
        rBuff[k]=s.zbMin; k++;
	for( int j=0; j<3; j++ ){ s.x[j]=rBuff[k]; k++; } // 
	assert( k==numr );

      }
    
      // cleanup
      delete [] iBuff;
      delete [] rBuff;
      
    } // end if selection_ != NULL 
    
  }
#endif

  // echo the command in the command list
  if( isGraphicsWindowOpen() && answer.length() > 0 && answer.substr(0,20)!="log commands to file")
  {
    appendCommandHistory(answer);
  }

  return returnValue;
}


int GL_GraphicsInterface::
interactiveAnswer(aString & answer,
		  const aString & prompt,
		  SelectionInfo *selection_,
		  int blocking /* = 1 */)
{
// AP: It is possible to have a GUI without any windows open, for example when reading all commands 
//     from file and writing the output to stdout. Therefore, we have to take this case into account.

  if( !graphicsWindowIsOpen )
    throw "ERROR: No graphics window open in interactiveAnswer";
  
  int i;
  // get answer from an interactive menu

// set the cursor to the pointer symbol over the dialog windows
  if (currentGUI)
  {
    currentGUI->setCursor(DialogData::pointerCursor);
    for (i=0; i<currentGUI->nDialogSiblings; i++)
      currentGUI->dialogSiblings[i].setCursor(DialogData::pointerCursor);
  }
  

  int menuSelected;
  
  if( selection_ == NULL )
  {
    menuSelected=
      moglGetAnswer( answer, prompt.length()>0 ? prompt : getDefaultPrompt(), NULL, blocking);
  }
  else
  {
    PickInfo xPick; 
    menuSelected=moglGetAnswer( answer, prompt.length()>0 ? prompt : getDefaultPrompt(),
				&xPick /* add argument picked, that gets set when something is picked */,
				blocking );
    if( xPick.pickType && selection_ ) // was something picked?
    {
      pickNew(xPick, *selection_);
    }
    
//    if( xPick.pickType ) // was something picked?
//    {
//        if( xPick.pickType == 1 && selection_ )
//  // fill in the selection info
//  	selectNew(xPick, selection_);   
//        else if( xPick.pickType == 2 && pick_ )
//  	pickNew(xPick, pick_);
//    }
  }
  
// set the cursor back to the watch symbol over the dialog windows
  if (currentGUI)
  {
    currentGUI->setCursor(DialogData::watchCursor);
    for (i=0; i<currentGUI->nDialogSiblings; i++)
      currentGUI->dialogSiblings[i].setCursor(DialogData::watchCursor);
  }
  
  
// printf("protectedGetAnswer: viewHasChanged=%i saveFile=%i\n",viewHasChanged,saveFile);
// if the view was changed with the mouse we need to indicate this in the command file we are saving.
  for (int w=0; w<moglGetNWindows(); w++)
  {
    assert( w>=0 && w<=128 );
    aString line; 

    if( viewHasChanged[w] ) 
// AP: The flag viewHasChanged is set by the routine changeView, but is not read until next time the
// program comes this way. Therefore, the "set view" command will not be appended to the command 
// history list until next time a button or menu item is chosen. However, this has the nice
// implication that multiple rotation/translations/zooms are aggregated and saved in only one 
// line in the history. This prevents the history list from beeing filled with "set view ..." entries.
    {
      viewHasChanged[w]=FALSE;

      if( isGraphicsWindowOpen() )
      {
	sPrintF(line,"set view:%i %g %g %g %g %g %g %g %g %g %g %g %g %g",
		w, 
		xShift[w], yShift[w], zShift[w], magnificationFactor[w],
		rotationMatrix[w](0,0),rotationMatrix[w](1,0),rotationMatrix[w](2,0),
		rotationMatrix[w](0,1),rotationMatrix[w](1,1),rotationMatrix[w](2,1),
		rotationMatrix[w](0,2),rotationMatrix[w](1,2),rotationMatrix[w](2,2));
	appendCommandHistory(line);
      }
      
      if ( saveFile)
      {
	fPrintF(saveFile,"%sset view:%i %g %g %g %g %g %g %g %g %g %g %g %g %g\n",
		(const char*)indentBlanks.c_str(),
		w,
		xShift[w], yShift[w], zShift[w], magnificationFactor[w],
		rotationMatrix[w](0,0),rotationMatrix[w](1,0),rotationMatrix[w](2,0),
		rotationMatrix[w](0,1),rotationMatrix[w](1,1),rotationMatrix[w](2,1),
		rotationMatrix[w](0,2),rotationMatrix[w](1,2),rotationMatrix[w](2,2));
        fflush(saveFile);
      }
    } // end if viewHasChanged[w]
  } // end for w

  // printf("GL_G: menuSelected=%i, answer = %s \n",menuSelected,(const char*)answer );
  return menuSelected;
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{pickPoints}}
int GL_GraphicsInterface::
pickPoints( RealArray & x, 
	    bool plotPoints /* = TRUE */,
	    int win_number /* = -1 */)
// =======================================================================
//  /Description: Pick points in 2D or 3D by clicking the mouse.
//    In 3D one should click on an object.
//    If multiple objects are 'hits' then the closest one is chosen.
// /x (input/output) : On input x should be dimensioned x(a:b,0:1) for 2D picks
//   or x(a:b.,0:2) for 3d picks. At most (b-a+1) points will be chosen. The actual number
// chosen is the return value.
// /plotPoints (input): Specifies whether the picked points should be plotted on the screen.
// /win\_number (input): The window number in which the picking should occur. If omitted, the 
//                       currentWindow is used.
// /Return value: the number of points chosen.
//  /Author: WDH \& AP
//\end{GL_GraphicsInterfaceInclude.tex} 
// =====================================================================
{
#ifndef NO_APP
  assert( x.getBase(1)==0 && (x.getBound(1)==1 || x.getBound(1)==2) );
#else
  assert( x.getBound(1)==1 || x.getBound(1)==2 );
#endif
  
  int oldCurrentWindow = currentWindow;
  if (win_number == -1) 
    win_number=currentWindow;
  else
    currentWindow = win_number; // temporarily make win_number current
  
  moglMakeCurrent(win_number);

  SelectionInfo pick;
  
  appendToTheDefaultPrompt("pick>"); // set the default prompt

  aString buttons[][2] =
  {{"done", "Done picking"}, {"",""}};
  aString answer;
  
//                   01234567890123456789
  aString menu[] = {"!Pick a point:xx", ""};
  sprintf(&menu[0][14], "%i", win_number);
  

  GUIState interface;
  interface.setUserButtons(buttons);
  interface.buildPopup(menu);
  
  pushGUI(interface);
  
  int list=0;  
  if( graphicsWindowIsOpen )
  {
    list=generateNewDisplayList();  // get a new (unlit) display list to use
    assert(list!=0);
  }
  
  const int numberOfDimensions = x.getBound(1)+1;
#ifndef NO_APP
  Range Axes=numberOfDimensions;
#endif
  
  const real zBufferResolution=pow(2.,31.); // where does 31 come from ?

  RealArray r(1,3), x0(1,3);

  int numberChosen=0;
#ifndef NO_APP
  int i = x.getBase(0);
#else
  int i = 0;
#endif
  
  aString line;
  while (i <= x.getBound(0))
  {
    if( !readFile )
    {
      // not reading a command file : interactive mode 
      bool oldSavePick = savePick; // the saving of pick commands could already be turned off?
      savePickCommands(FALSE); // temporarily turn off saving of pick commands.
      int numberSelected=getAnswer(answer,"pick a point with mouse-1", pick);
      savePickCommands(oldSavePick); // restore savePick
      if( answer=="done" )
	break;
      else if( pick.active ) // was something picked? (what elese could have happened?)
      {
	r(0,0)=pick.r[0];
	r(0,1)=pick.r[1];
	
	if( pick.nSelect==0 )
	{
	  if (numberOfDimensions > 2) 
	    outputString("Warning: You picked an unnamed object. The depth is unknown");
	  r(0,2)=0.5*zBufferResolution; // the third coordinate is unknown, but it doesn't matter in 2D
	  pickToWorldCoordinates(r, x0, win_number);
	  if( numberOfDimensions==2 )
	    x0(0,2)=0.;
	}
	else
	{
// the coordinate is already computed
	  x0(0,0) = pick.x[0];
	  x0(0,1) = pick.x[1];
	  x0(0,2) = pick.x[2];
	}
//  	aString info;
//  	sPrintF(info,"x0(0,2): %e", x0(0,2));
//  	outputString(info);
	
// add this point to the existing list
	numberChosen++;
#ifndef NO_APP
	x(i,Axes)=x0(0,Axes);
#else
	for (int q=0; q<numberOfDimensions; q++)
	  x(i,q)=x0(0,q);
#endif
// echo command in list
	sPrintF(line, "%g %g %g", x0(0,0), x0(0,1), x0(0,2));
	appendCommandHistory(line);
	
        if( saveFile )
	{
          fPrintF(saveFile,"%s   %g %g %g \n",(const char*)indentBlanks.c_str(),x0(0,0),x0(0,1),x0(0,2));
          fflush(saveFile);
	}
      }
      else
	outputString("Non-pick event in pickPointsNew");
    }
    else
    {
// command read from file.
// look for an answer of the form "x y z" or "done"
      int numCharsRead = readLineFromCommandFile(answer);
      if( numCharsRead<1 )
      {
	// printF("Message: end-of-file reached in command file\n");
	stopReadingCommandFile();
	
//  	fclose(readFile);
//  	readFile=NULL;    // fall through to the interactive menu

	continue; // disregard this input and jump to the next
      }
      else
      {
        int numberRead=0;
        if( answer=="done" )
	  break;
	numberRead=sScanF(answer,"%e %e %e",&x0(0,0),&x0(0,1),&x0(0,2));
// printf(" numberRead=%i, x=(%e,%e,%e) \n",numberRead,x0(0,0),x0(0,1),x0(0,2));
	if( numberRead==2 )
  	{
  	  x0(0,2)=0.;
	  numberRead++;
  	}
//          printf(" point %i : x=(%e,%e,%e) \n",i,x0(0,0),x0(0,1),x0(0,2));
//        }
        if( numberRead!=3 )
        {
          printf("pickPoints: Unknown response: %s \n",(const char*) answer.c_str());
          stopReadingCommandFile();
	  continue; // disregard this input and jump to the next
        }
	else // this input was probably ok
	{
	  numberChosen++;
#ifndef NO_APP
	  x(i,Axes)=x0(0,Axes); // i holds the next slot in x. See the last line in the while loop
#else
	  for (int q=0; q<numberOfDimensions; q++)
	    x(i,q)=x0(0,q);
#endif
// echo command in list
	  sPrintF(line, "%g %g %g", x0(0,0), x0(0,1), x0(0,2));
	  appendCommandHistory(line);
	}
      } // end if numCharsRead >= 1
      
    } // end if readfile
     
    // -> plot the points
    if( plotPoints && graphicsWindowIsOpen && x.getLength(0)>=1 )
    {
// What if the user presses the "Clear" button in the middle of entering all coordinates?
// Then all display lists are deleted, so we would have to make a new one!
      if (!glIsList(list))
      {
	// printf("Making a new display list\n");
	list = generateNewDisplayList();  // get a new (unlit) display list to use
      }
      
      glDeleteLists(list,1); // clear list. 
      glNewList(list,GL_COMPILE);

      real pointSize=4.;
      glPointSize(pointSize);   
      setColour(textColour);
      glBegin(GL_POINTS);  
      if (numberOfDimensions == 2)
	for( int j=0; j<numberChosen; j++ )
	  glVertex3(x(j,0), x(j,1), 0.0);
      else
	for( int j=0; j<numberChosen; j++ )
	  glVertex3(x(j,0), x(j,1), x(j,2));

      glEnd();

      glEndList(); 
      moglPostDisplay(win_number); // update the window after points have been plotted.
    }
#ifndef NO_APP
    i = x.getBase(0) + numberChosen; // next slot in the x-array
#else
    i = numberChosen;
#endif

  } // end while;

  popGUI();
  unAppendTheDefaultPrompt();  // reset

  moglMakeCurrent(oldCurrentWindow); // reset the window focus
  currentWindow = oldCurrentWindow;

//  printf("Exiting pickPoints\n");
  
  return numberChosen; // return number assigned.
}

	
void GL_GraphicsInterface::
pickNew(PickInfo &xPick, SelectionInfo & select)
//----------------------------------------------------------------------------------
// /Access level: protected
//  /Author: AP
//-----------------------------------------------------------------------------------
{
  const real zBufferResolution=pow(2.,31.); // where does 31 come from ?
// fill in the picking info
  select.active = 1;
  select.winNumber = xPick.pickWindow;
  
  select.r[0] = xPick.pickBox[0]; // rMin (horizontal window coordinate)
  select.r[1] = xPick.pickBox[1]; // rMax
  select.r[2] = xPick.pickBox[2]; // sMin (vertical window coordinate)
  select.r[3] = xPick.pickBox[3]; // sMax

// check if any object lies in the line of sight
  selectNew(xPick, &select);   
	
  if( select.nSelect==0 ) // no hits
  {
    select.zbMin = 0.0; // the zBuffer coordinate is unknown, but it doesn't matter in 2D
  }
  else
  {

      int j;
//        printf("Selection array before sorting:\n");
//        for (j=0; j<select.nSelect; j++)
//        {
//          printf("selection(%i,0:2) = %i,%i,%i\n", j, select.selection(j,0), 
//     	     select.selection(j,1),select.selection(j,2));
//        }

// Sort the selection array in ascending z_buffer values!!!
    int ii, zbMin, zbMax, globalID;
    for (j=0; j<select.nSelect; j++)
    {
      int iZB=j;
      zbMin = select.selection(iZB,1);
      for (ii=j+1; ii<select.nSelect; ii++)
      {
	if (select.selection(ii,1) < zbMin)
	{
	  zbMin = select.selection(ii,1);
	  iZB = ii;
	}
      }
// swap element j and iZB
      if (j!=iZB)
      {
	globalID = select.selection(j,0);
	zbMin = select.selection(j,1);
	zbMax = select.selection(j,2);

        select.selection(j,0) = select.selection(iZB,0);
        select.selection(j,1) = select.selection(iZB,1);
        select.selection(j,2) = select.selection(iZB,2);

        select.selection(iZB,0) = globalID;
	select.selection(iZB,1) = zbMin;
	select.selection(iZB,2) = zbMax;
      }
    } // end for j

//        printf("Selection array after sorting:\n");
//        for (j=0; j<select.nSelect; j++)
//        {
//          printf("selection(%i,0) = %i, selection(%i,1) = %i\n", j, select.selection(j,0), 
//    	     j, select.selection(j,1));
//        }

// store the index of the closest object
    select.globalID = select.selection(0, 0); // global ID value of the closest object
    select.zbMin    = select.selection(0, 1); // zBuffer value of the closest object

// compute world coordinate by inverting the viewing transformation
    GLdouble winx,winy,winz;
    GLdouble x1,x2,x3;
    winx=select.r[0]*(viewPort[xPick.pickWindow][2]-viewPort[xPick.pickWindow][0])+
      viewPort[xPick.pickWindow][0];
    winy=select.r[1]*(viewPort[xPick.pickWindow][3]-viewPort[xPick.pickWindow][1])+
      viewPort[xPick.pickWindow][1];
    winz=select.zbMin/zBufferResolution;
	  
    gluUnProject( winx, winy, winz, modelMatrix[xPick.pickWindow], 
		  projectionMatrix[xPick.pickWindow], viewPort[xPick.pickWindow], &x1, &x2, &x3);
    select.x[0]=x1;
    select.x[1]=x2;
    select.x[2]=x3;
  }

  if( savePick) 
  {
    aString line;
    if (select.nSelect>0)
      sPrintF(line,"mogl-coordinates %e %e %e %e %e %e %e %e", 
	      select.r[0], select.r[1], select.r[2], select.r[3], select.zbMin,
	      select.x[0], select.x[1], select.x[2]);
    else
      sPrintF(line,"mogl-pickOutside:%i %e %e", xPick.pickWindow, 
	      select.r[0], select.r[1]);
    appendCommandHistory(line);
  }
	
  if( saveFile && savePick ) // AP: need to save the other corner too!
  {
    if (select.nSelect>0)
      fPrintF(saveFile,"%smogl-coordinates %e %e %e %e %e %e %e %e\n", SC indentBlanks.c_str(), 
	      select.r[0], select.r[1], select.r[2], select.r[3], select.zbMin,
	      select.x[0], select.x[1], select.x[2]);
    else
      fPrintF(saveFile,"%smogl-pickOutside:%i %e %e\n", SC indentBlanks.c_str(), xPick.pickWindow,  
	      select.r[0], select.r[1]);
    fflush(saveFile);
  }
  
}

static bool
uniqueObject(IntegerArray & selection, GLuint objectID, int numberSelected)
{
  int i;
  for (i=0; i<numberSelected; i++)
  {
    if (selection(i,0) == objectID)
      return false;
  }
  return true;
}


int GL_GraphicsInterface::
selectNew(PickInfo &xPick, SelectionInfo *select)
//----------------------------------------------------------------------------------
// /Access level: protected
//  /Description:
//    Select the objects tht are near the pick box stored in xPick.pickBox.
//    Note that even objects that are hidden by another surface will
//    be selected. Objects that are removed by cutting planes are not selected.
//    Use the z-buffer values to determine which objects are in front.
//  /Author: WDH \& AP
//-----------------------------------------------------------------------------------
{
  int debug= false;

  int numberSelected=0;
  int win_number=xPick.pickWindow; // AP: Needs to be fixed for the case win_number != currentWindow
  
  if (win_number < 0)
  {
    select->nSelect = 0;
    return 0;
  }
  

  // pick entries using left mouse button
  // either pick a point or choose a rubber band box

  real x0,y0,x1,y1;
  x0=xPick.pickBox[0];
  x1=xPick.pickBox[1];
  y0=xPick.pickBox[2];
  y1=xPick.pickBox[3];

  if( debug )
    printf("selectObjects: (x0,y0) = (%e,%e), (x1,y1) = (%e,%e) \n",x0,y0,x1,y1);

  int selectBufferSize=1024;
  GLuint *selectBuffer = new GLuint [selectBufferSize];
  glSelectBuffer(selectBufferSize,selectBuffer);

  // **turn on selection ***
  glRenderMode( GL_SELECT );  
  GLint hits;
  
  GLbitfield arg = GLbitfield(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClear(arg);

  glGetIntegerv( GL_VIEWPORT, viewPort[win_number] );
  if( debug )
    printf("selectObjects: viewPort = %i, %i, %i, %i \n",viewPort[win_number][0],viewPort[win_number][1],viewPort[win_number][2],viewPort[win_number][3]);

  GLdouble xm = .5*(x0+x1)*(viewPort[win_number][2]-viewPort[win_number][0])+viewPort[win_number][0];
  GLdouble ym = .5*(y0+y1)*(viewPort[win_number][3]-viewPort[win_number][1])+viewPort[win_number][1];

  GLdouble width  = fabs(x1-x0)*(viewPort[win_number][2]-viewPort[win_number][0]);
  GLdouble height = fabs(y1-y0)*(viewPort[win_number][3]-viewPort[win_number][1]);
 
  const real minWidth=5.;
  // const real minWidth=5.*magnificationFactor[win_number]; // try this *wdh* 090530
  
  width=max(minWidth,width);
  height=max(minWidth,height);
  
  if( debug )
  {
    printf("selectObjects: (xm,ym)=(%8.2e,%8.2e), (width,height)=(%8.2e,%8.2e)\n",xm,ym,width,height);
    printf("selectObjects: (xl,yl)=(%8.2e,%8.2e), (xr,yr)=(%8.2e,%8.2e)\n",
        xm-width*.5,ym-height*.5,xm+width*.5,ym+height*.5);
  }

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  // this must be before glOrtho !
  gluPickMatrix( xm,ym,width,height,viewPort[win_number]); // Form a viewing volume around the point that was selected

  /// printf("select: near=%e, far=%e \n",near,far);
//..this updates the screen & the powerwall, maintains correct aspect ratio **pf
//    graphics_setOrthoKeepAspectRatio( aspectRatio[win_number], magnificationFactor[win_number],
//  				    leftSide[win_number], rightSide[win_number], 
//  				    bottom[win_number], top[win_number],
//  				    near[win_number], far[win_number] );   
  glOrtho( leftSide[win_number]/magnificationFactor[win_number], 
	   rightSide[win_number]/magnificationFactor[win_number], 
	   bottom[win_number]/magnificationFactor[win_number], 
	   top[win_number]/magnificationFactor[win_number],
	   near[win_number], far[win_number] ); // wdh: 981010 : objects were being clipped, 

  display(win_number); // redisplay all objects to determine the selection.
    
// display changes the matrix mode to GL_MODELVIEW
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glFlush();
  
  hits = glRenderMode( GL_RENDER ); // the number of hits gets reported when the rendering mode is turned back
  int numberOfTries=0;
  while( hits < 0 && numberOfTries<10 )
  {
    numberOfTries++;

    printf("GL_GraphicsInterface::select: WARNING: selectBuffer size=%i is too small. "
        " Increasing... (numberOfTries=%i)\n",
          selectBufferSize,numberOfTries);

    selectBufferSize*=2;
    delete [] selectBuffer;
    selectBuffer = new GLuint [selectBufferSize];
    glSelectBuffer(selectBufferSize,selectBuffer);

// AP: Maybe it is not necessary to redo all these things...
    // **turn on selection ***
    glRenderMode( GL_SELECT );  
    glClear(arg);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

  // this must be before glOrtho !
    gluPickMatrix( xm,ym,width,height,viewPort[win_number]); // Form a viewing volume around the point that was selected

    /// printf("select: near=%e, far=%e \n",near,far);
  //..this updates the screen & the powerwall, maintains correct aspect ratio **pf
//      graphics_setOrthoKeepAspectRatio( aspectRatio[win_number], magnificationFactor[win_number],
//  				      leftSide[win_number], rightSide[win_number], 
//  				      bottom[win_number], top[win_number],
//  				      near[win_number], far[win_number] );   
    glOrtho( leftSide[win_number]/magnificationFactor[win_number], 
	     rightSide[win_number]/magnificationFactor[win_number], 
	     bottom[win_number]/magnificationFactor[win_number], 
	     top[win_number]/magnificationFactor[win_number],
	     near[win_number], far[win_number] ); // wdh: 981010 : objects were being clipped, 

    display(win_number); // redisplay all objects to determine the selection.
    
// display changes the matrix mode to GL_MODELVIEW
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glFlush();

    hits = glRenderMode( GL_RENDER );
  }
  if( hits < 0 )
  {
    printf("GL_GraphicsInterface::select: ERROR: selectBuffer is too still too small! Returning no hits.! \n");
    printf("GL_GraphicsInterface::select: You should choose fewer objects\n");
    hits=0;
  }
  
// reset viewing matrices
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //..this updates the screen & the powerwall, maintains correct aspect ratio **pf
//    graphics_setOrthoKeepAspectRatio( aspectRatio[win_number], magnificationFactor[win_number],
//  				    leftSide[win_number], rightSide[win_number], 
//  				    bottom[win_number], top[win_number],
//  				    near[win_number], far[win_number] );   
  glOrtho( leftSide[win_number]/magnificationFactor[win_number], 
	   rightSide[win_number]/magnificationFactor[win_number], 
	   bottom[win_number]/magnificationFactor[win_number], 
	   top[win_number]/magnificationFactor[win_number],
	   near[win_number], far[win_number] ); // wdh: 981010 : objects were being clipped, 

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); // AP: Why reset it?

  unsigned int i, j;
  GLuint names, *ptr;

  if( debug )  
    printf ("number of hits = %d\n", hits);
  select->selection.redim(hits*2,3);
  ptr = (GLuint *) selectBuffer;
   
  int currentName;
// save all objects in the rectangle
  for (i = 0; i < hits; i++) 
  {	/*  for each hit  */
    names = *ptr;
    if( debug )  
      printf (" number of names for hit %i is %d\n", i, names); 
    ptr++;
    int z1=*ptr/2;  // *** divide by 2 so we can save an unsigned int in an int
    if( debug )  
      printf (" z1 is %u;", *ptr); 
    ptr++;
    int z2=*ptr/2;
    if( debug )  
      printf (" z2 is %u\n", *ptr); 
    ptr++;
    if( debug )  
      printf ("   the name is ");
    for (j = 0; j < names; j++) 
    {	/*  for each name */
      currentName = *ptr; // the strange hits seem to give negative int's
// avoid storing the same object several times
      bool isUnique=true;
      if (currentName > 0 && 
	  (isUnique=uniqueObject(select->selection, *ptr, numberSelected)))
      {
	select->selection(numberSelected,0)=*ptr;
	select->selection(numberSelected,1)=z1;
	select->selection(numberSelected,2)=z2;
	numberSelected++;
	if( select->selection.getLength(0) <= numberSelected )
	  select->selection.resize(numberSelected+hits*2,3);
      
	if( debug )  
	  printf ("'%i' ", *ptr); // save in a log file
      }
      else if (debug)
      {
	if (isUnique)
	  printf ("'negative name=%i' ", *ptr); // save in a log file
	else
	  printf ("'duplicate name=%i' ", *ptr); // save in a log file
      }
      
      ptr++; 
    }
    if( debug )  
      printf ("\n");
  }
  
// echo command in list
  if( numberSelected > 0 && savePick )
  {
    aString line;
    sPrintF(line,"mogl-select:%i %i", win_number, numberSelected);
    appendCommandHistory(line);
    
    for( i=0; i<numberSelected; i++ )
    {
      sPrintF(line,"%i %i %i  ",select->selection(i,0),select->selection(i,1),select->selection(i,2));
      appendCommandHistory(line);
    }
  }

  if( numberSelected > 0 && saveFile && savePick )
  {
    fPrintF(saveFile,"%smogl-select:%i %i \n",(const char*)indentBlanks.c_str(), win_number, numberSelected);
    
    for( i=0; i<numberSelected; i+=3 )
    {
      // save at most 3 selects per line (to prevent lines from being too long)
      fPrintF(saveFile,"%s      ",(const char*)indentBlanks.c_str());
      for( j=i; j<numberSelected && j<i+3; j++ )
        fPrintF(saveFile,"%i %i %i  ", select->selection(j,0), select->selection(j,1), select->selection(j,2));
      fPrintF(saveFile,"\n"); 
    }
    fflush(saveFile);
  }

  if( numberSelected>0 )
    select->selection.resize(numberSelected,3);
  else
    select->selection.redim(0);

  delete [] selectBuffer;
  select->nSelect = numberSelected;
  
  return numberSelected;
}

static void
wait_a_second(real timeToWait)
{
  if (timeToWait <= 0.) return;
  
  real time0 = getCPU(), time1;
  real a=0;
  int i;
  do
  {
    for (i=0; i<10000; i++)
    {
      a+= pow(a,0.12345);
    }
    time1 = getCPU();
  } while (time1-time0 < timeToWait);
//  printf("Actual time waited: %g seconds\n", time1-time0);
}


void GL_GraphicsInterface::
parseCommandLine( const aString & line, aString & command, int & windowNumber, aString & arg ) const
// ===========================================================================================
//  /Description:
//     Parse a "line" to determine the command, window number and argument. The line
//   will be of the form
//              command:windowNumber arg
//  For example:
//       line="x+r:0 90"  -> command="x+r", windowNumber=0, arg="90"
//
//  The command can also be of the form (with no ':' )
//             command arg
//  in which case the windowNumber is set to the currentWindow.   
// ============================================================================================
{
  // const char *crest = strrchr(cline,':'); // get the last occurance of ':'

  int argStart=-1, argEnd=-1, winStart=-1, winEnd=-1, commandEnd=-1;
   
  int i=line.length()-1;
 
  while( i>=0 && line[i]==' ' ) i--;  // skip trailing blanks
  argEnd=i;
  i--;
  while( i>=0 && line[i]!=' ' ) i--; // get argument 
  argStart=i+1;
  
  winEnd=i-1;
  while( i>=0 && line[i]!=':' ) i--;  // look for ':'
  winStart=i+1;
  
  commandEnd=i-1;
  windowNumber=currentWindow;
  if( winEnd>=winStart )
    sScanF(line.substr(winStart,winEnd-winStart+1),"%i",&windowNumber);
 
  if( commandEnd>=0 ) 
  {
    command=line.substr(0,commandEnd+1);
    arg=line.substr(argStart,argEnd-argStart+1);
  }
  else
  {
    // there must be no ':' in the string
    if( argStart>0 )
    {
      arg=line.substr(argStart,argEnd-argStart+1);
      command=line.substr(0,argStart);
    }
    else
    {
      arg="";
      command=line;
    }
  }
//    printf("parseCommandLine: line=[%s] -> command=[%s] windowNumber=%i arg=[%s]\n",(const char*)line,
//             (const char*)command,windowNumber,(const char*)arg);
}

int GL_GraphicsInterface::
processSpecialMenuItems(aString & answer)
{
  // printf("GL_GraphicsInterface::processSpecialMenuItems: answer=[%s]\n",(const char*)answer);
  

  int alreadySaved=FALSE;
  int answerFound=TRUE, win_number = currentWindow;
  int openingTheGUI = FALSE;
  int item, len;
  bool reDisplay=false;
  
  aString line, buf;
  if( answer.length()<1 )
  {
    answerFound=FALSE;
    return answerFound;
  }
  
  const real defaultValue = -INT_MAX;
  real value=defaultValue;

  aString command,arg;
  parseCommandLine( answer,command,win_number,arg );
  sScanF(arg,"%e", &value);

  if( answer[0]=='x' || answer[0] == 'y' || answer[0]=='z' )
  {
    if( answer.substr(0,3)=="x+r" )
      dtx[win_number] += (value!=defaultValue ? value : deltaAngle[win_number]); 
    else if( answer.substr(0,3)=="y+r" )
      dty[win_number] += (value!=defaultValue ? value : deltaAngle[win_number]); 
    else if( answer.substr(0,3)=="z+r" )
      dtz[win_number] += (value!=defaultValue ? value : deltaAngle[win_number]); 
    else if( answer.substr(0,3)=="x-r" )
      dtx[win_number] -= (value!=defaultValue ? value : deltaAngle[win_number]); 
    else if( answer.substr(0,3)=="y-r" )
      dty[win_number] -= (value!=defaultValue ? value : deltaAngle[win_number]); 
    else if( answer.substr(0,3)=="z-r" )
      dtz[win_number] -= (value!=defaultValue ? value : deltaAngle[win_number]); 
    else if( answer.substr(0,2)=="x-" )
      xShift[win_number] -= (value!=defaultValue ? value : 
			     deltaX[win_number]/magnificationFactor[win_number]);
    else if( answer.substr(0,2)=="x+" )
      xShift[win_number] += (value!=defaultValue ? value : 
			     deltaX[win_number]/magnificationFactor[win_number]);
    else if( answer.substr(0,2)=="y+" )
      yShift[win_number] += (value!=defaultValue ? value : 
			     deltaY[win_number]/magnificationFactor[win_number]);
    else if( answer.substr(0,2)=="y-" )
      yShift[win_number] -= (value!=defaultValue ? value : 
			     deltaY[win_number]/magnificationFactor[win_number]);
    else if( answer.substr(0,2)=="z+" )
      zShift[win_number] += (value!=defaultValue ? value : 
			     deltaZ[win_number]/magnificationFactor[win_number]);
    else if( answer.substr(0,2)=="z-" )
      zShift[win_number] -= (value!=defaultValue ? value : 
			     deltaZ[win_number]/magnificationFactor[win_number]);
    else
      answerFound=FALSE;
    if (answerFound)
    {
      setRotationTransformation(win_number);
      reDisplay=true;
    }
    
  }
  else if( answer.substr(0,6)=="bigger" )
  {
//      aString rest = answer(6,answer.length());
//      const real defaultValue = -INT_MAX;
//    real magFactor=defaultValue;
    real magFactor=value;
//    if( rest!="" ) sScanF( rest,"%e",&magFactor );
    if( magFactor==defaultValue )
      magFactor= magnificationIncrement[win_number];
    // printf("magFactor=%e\n",magFactor);
    
    magnificationFactor[win_number] *= magFactor;
    setProjection(win_number);
    reDisplay=true;
  } 
  else if( answer.substr(0,7)=="smaller" )
  {
//      aString rest = answer(7,answer.length());
//      const real defaultValue = -INT_MAX;
//      real magFactor=defaultValue;
    real magFactor=value;
//      if( rest!="" ) sScanF( rest,"%e",&magFactor );
    if( magFactor==defaultValue )
      magFactor= magnificationIncrement[win_number];

    magnificationFactor[win_number] /= magFactor;
    setProjection(win_number);
    reDisplay=true;
  }
  else if( answer.substr(0,8)=="set home" )
  {
    printF("The current view is now the default home view. Choose `reset' to return to the default home view.\n");
    // set the new "home" view so be the current view
    homeViewParameters[win_number][ 0]=win_number+.5;
    homeViewParameters[win_number][ 1]=xShift[win_number];
    homeViewParameters[win_number][ 2]=yShift[win_number];
    homeViewParameters[win_number][ 3]=zShift[win_number];
    homeViewParameters[win_number][ 4]=magnificationFactor[win_number];
    homeViewParameters[win_number][ 5]=rotationMatrix[win_number](0,0);
    homeViewParameters[win_number][ 6]=rotationMatrix[win_number](1,0);
    homeViewParameters[win_number][ 7]=rotationMatrix[win_number](2,0);
    homeViewParameters[win_number][ 8]=rotationMatrix[win_number](0,1);
    homeViewParameters[win_number][ 9]=rotationMatrix[win_number](1,1);
    homeViewParameters[win_number][10]=rotationMatrix[win_number](2,1);
    homeViewParameters[win_number][11]=rotationMatrix[win_number](0,2);
    homeViewParameters[win_number][12]=rotationMatrix[win_number](1,2);
    homeViewParameters[win_number][13]=rotationMatrix[win_number](2,2);
  }
  else if( answer.substr(0,5)=="reset" )
  {
    // reset the b=view to the "home" view
    if( homeViewParameters[win_number][ 0] < 0 )
    {
      resetView(win_number); // needs window number
    }
    else
    { // home view has been set
      real oldMF = magnificationFactor[win_number];

      xShift[win_number]=	      homeViewParameters[win_number][ 1];
      yShift[win_number]=	      homeViewParameters[win_number][ 2];
      zShift[win_number]=	      homeViewParameters[win_number][ 3];
      magnificationFactor[win_number]=homeViewParameters[win_number][ 4];
      rotationMatrix[win_number](0,0)=homeViewParameters[win_number][ 5];
      rotationMatrix[win_number](1,0)=homeViewParameters[win_number][ 6];
      rotationMatrix[win_number](2,0)=homeViewParameters[win_number][ 7];
      rotationMatrix[win_number](0,1)=homeViewParameters[win_number][ 8];
      rotationMatrix[win_number](1,1)=homeViewParameters[win_number][ 9];
      rotationMatrix[win_number](2,1)=homeViewParameters[win_number][10];
      rotationMatrix[win_number](0,2)=homeViewParameters[win_number][11];
      rotationMatrix[win_number](1,2)=homeViewParameters[win_number][12];
      rotationMatrix[win_number](2,2)=homeViewParameters[win_number][13];


    if (magnificationFactor[win_number] != oldMF)
      setProjection(win_number);
    }
    
    reDisplay=true;
    answerFound=true;
  }    
//                        012345678901234567890123456789
  else if( answer.substr(0,14)=="GLOB:init view" )
  {
    homeViewParameters[win_number][ 0]=-1; // this invalidates the default home view

    initView(win_number); // needs window number
    reDisplay=true;
    answerFound=true;
  }    

  else if( answer.substr(0,8)=="set view")  
  {
    int dum;
    real oldMF = magnificationFactor[win_number];
    rotationMatrix[win_number]=0.;
    rotationMatrix[win_number](3,3)=1.; 
    sScanF(answer,"set view:%i %e %e %e %e %e %e %e %e %e %e %e %e %e", &dum,
           &xShift[win_number], &yShift[win_number], &zShift[win_number], &magnificationFactor[win_number],
           &rotationMatrix[win_number](0,0),&rotationMatrix[win_number](1,0),&rotationMatrix[win_number](2,0),
           &rotationMatrix[win_number](0,1),&rotationMatrix[win_number](1,1),&rotationMatrix[win_number](2,1),
           &rotationMatrix[win_number](0,2),&rotationMatrix[win_number](1,2),&rotationMatrix[win_number](2,2));
    reDisplay=true;
    if (magnificationFactor[win_number] != oldMF)
      setProjection(win_number);
  }
  else if( answer.substr(0,9)=="clear all" ) /* OLD "erase" */
    erase(win_number, false); // false only deletes non-hideable lists. Hideable lists are just not shown
  else if ( len=str_matches(answer,"force redraw, wait") )
  {
    real timeToWait = 0.0;
    if (answer.length() > len+1)
    {
      sScanF(answer.substr(len,answer.length()-len), "%e", &timeToWait);
    }
//    printf("TimeToWait: %g\n", timeToWait);
    
    redraw(true);
    wait_a_second(timeToWait);
  }
  else if( answer.substr(0,8)=="detailed" )
  {
    outputString("There is no detailed help yet.");
  }
//                       01234567890123456789
  else if( answer.substr(0,10)=="new window" )
  {
// only do this if the graphical user interface is active
    if( isGraphicsWindowOpen() )
    {
      int newWin = createWindow("plot"); // could ask for a different name
      // setup the buttons.
      moglBuildUserButtons(currentGUI->windowButtonCommands, currentGUI->windowButtonLabels, newWin);
      // set the pulldown menu. Make it sensitive only in the current window
      moglBuildUserMenu(currentGUI->pulldownCommand, currentGUI->pulldownTitle, newWin);
      // Only make the buttons and pulldown menus sensitive on the active window
      moglSetSensitive(newWin, 1 );
      // build the popup menu in all windows
      moglBuildPopup( currentGUI->popupMenu );
    }
  }
  else if( answer.substr(0,13)=="open graphics" )
  {
    // const int myid=max(0,Communication_Manager::My_Process_Number);
    // printf(" *** open graphics myid=%i\n",myid);
    // fflush(0);
    
    if( !isGraphicsWindowOpen() ) // only initialize the GUI if it is currently off
    {
      // this should trigger getAnswer to exit so that the interactive version gets called next time
      openingTheGUI = true;
      
      createWindow("plot"); // could ask for a different name

      // we need to setup the GUI too!
      openGUI();
      
      // setup the array with the union of all commands
      currentGUI->setAllCommands();
      
    }
    
  }
  else if( answer=="turn on graphics" )
  {
    printF("INFO:Graphics plotting (grid/contour/streamlines etc.) will be turned on.\n"
           "     Use `turn off graphics' to turn off plotting (grid/contour/streamlines etc.)");
    turnOnGraphics();
  }
  else if( answer=="turn off graphics" )
  {
    printF("INFO:Graphics plotting (grid/contour/streamlines etc.) will be turned off.\n"
           "     Note that commands accepted by these routines will no longer be processed.\n"
           "     Use `turn on graphics' to reset.");
    turnOffGraphics();
  }
  else if( answer.substr(0,8)=="hardcopy" )
    hardcopyCommands(answer, win_number);
//  else if( answer.substr(0,5)=="MOVIE" )
//    movieCommands(answer, win_number);
  //                      01234567890
  else if(answer.substr(0,7)=="DISPLAY")
  {
    optionCommands(answer, win_number);
  }
//                      01234567890
  else if(answer.substr(0,8)=="LIGHTING")
  {
// answers of the form
// LIGHTING:1 ON/OFF
    const char *ans=answer.c_str();
    const char *onOff=strpbrk(ans," ");
    if (onOff && strlen(onOff) > 0)
    {
      answerFound=TRUE;
      if (!strcmp(&onOff[1],"ON"))
	lighting[win_number] = 1;
      else if (!strcmp(&onOff[1],"OFF"))
	lighting[win_number] = 0;
      else
      {
	answerFound=FALSE;
	outputString("Unknown resonse:");
	outputString(onOff);
      }
    }
    else
      answerFound=FALSE;
  }
//                      012345
  else if(answer.substr(0,6)=="LIGHT#")
  {
// answers of the form
// LIGHT#0:1 ON/OFF
    const char *ans=answer.c_str();
    const char *hash=strpbrk(ans,"#");
    int lightN=0;
    if (hash && strlen(hash) > 0)
    {
      sScanF(&hash[1],"%i", &lightN);
    
      const char *onOff=strpbrk(ans," ");
      if (onOff && strlen(onOff) > 0)
      {
	answerFound=TRUE;
	if (!strcmp(&onOff[1],"ON"))
	  lightIsOn[win_number][lightN] = 1;
	else if (!strcmp(&onOff[1],"OFF"))
	  lightIsOn[win_number][lightN] = 0;
	else
	{
	  answerFound=FALSE;
	  outputString("Unknown resonse:");
	  outputString(onOff);
	}
      }
    }
    else
      answerFound=FALSE;
  }
  //                    0123456789
  else if(answer.substr(0,9)=="POSITION#")
  {
// answers of the form
// POSITION#0:1 x y z
    const char *ans=answer.c_str();
    const char *hash=strpbrk(ans,"#");
    int lightN=0;
    real x, y, z;
    if (hash && strlen(hash) > 0)
    {
      sScanF(&hash[1],"%i", &lightN);
    
      const char *numberString=strpbrk(ans," ");
      if (numberString && strlen(numberString) > 0)
      {
	answerFound=TRUE;
	if (sScanF(&numberString[1],"%g %g %g", &x, &y, &z) == 3)
	{
	  position[win_number][lightN][0] = x;
	  position[win_number][lightN][1] = y;
	  position[win_number][lightN][2] = z;
	}
	else
	{
	  answerFound=FALSE;
	  outputString("Unknown resonse:");
	  outputString(ans);
	}
      }
    }
    else
      answerFound=FALSE;
  }//                      012345678901234567890123456789
  else if( answer.substr(0,19) =="Pick rotation point")
  {
// in order for the commands to appear in the correct order in the log file, we need to
// save them before picking the point
    appendCommandHistory(answer);
    
    if( saveFile && !readFile )
    {
      fPrintF(saveFile,"%s\n",(const char*)(indentBlanks+answer).c_str()); // save in the log file
      fflush(saveFile);
    }
    
    alreadySaved = TRUE;
    
    outputString("Pick a rotation point with the mouse");
    RealArray x(1,3);
    if( pickPoints( x, FALSE, win_number ) ) //FALSE means do not mark the point on the screen
    {
      // only change the rotation point if a coordinate was picked
      real xp[3];
      xp[0] = x(0,0);
      xp[1] = x(0,1);
      xp[2] = x(0,2);
      setRotationCenter( xp, win_number );// in addition to changing the rotationCenter, 
      // we need to recompute translations so that the plot doesn't get translated.

      //    char line[200];
      //      sprintf(line, "New rotation point = (%10.3e,%10.3e,%10.3e)",x(0,0),x(0,1),x(0,2));
      //      outputString(line);
      userDefinedRotationPoint[win_number]=TRUE;
      // need to redraw the plot if the axes centered at the rotation point or if the "anchor" is displayed
      // at the rotation point
      if( axesOriginOption[win_number]==1 || plotTheRotationPoint[win_number]) 
	moglPostDisplay(win_number);
      // update the printed rotation point value in the set view characteristics dialog
      moglPrintRotPnt(x(0,0), x(0,1), x(0,2), win_number);
    }
  }//                     01234567890123456789
  else if(answer.substr(0,13) =="ROTATIONPOINT")
  {
// the answer is of the form ROTATIONPOINT:0 x y z
    const char *ans=answer.c_str();
    const char *numberString=strpbrk(ans," ");
    if (numberString && strlen(numberString) > 0)
    {
      real x, y, z;
      if (sScanF(&numberString[1],"%g %g %g", &x, &y, &z) == 3)
      {
//	printf("rot pnt read: %g %g %g\n", x, y, z);
	real xp[3];
	xp[0] = x;
	xp[1] = y;
	xp[2] = z;
	setRotationCenter( xp, win_number ); // in addition to changing the rotationCenter, 
// we need to recompute translations so that the plot doesn't get translated.
	userDefinedRotationPoint[win_number]=TRUE;
	if( axesOriginOption[win_number]==1 ) // need to redraw the axes if the origin is at the rotation point
	  moglPostDisplay(win_number);
      }
      else
      {
	answerFound=FALSE;
	outputString("Unknown resonse:");
	outputString(ans);
      }
    }
    else
    {
      answerFound=FALSE;
      outputString("Unknown resonse:");
      outputString(ans);
    }
    
  }//                    012345678901234567890123456789
  else if(answer.substr(0,7) =="AMBIENT")
  {
// answer is of the form 
// AMBIENT#0:1 r g b a, to set the (r,g,b,a) values of light 0 in window 1
    const char *ans=answer.c_str();
    const char *hash=strpbrk(ans,"#");
    int lightN=0;
    real r, g, b, a;
    if (hash && strlen(hash) > 0)
    {
      sScanF(&hash[1],"%i", &lightN);
    
      const char *numberString=strpbrk(ans," ");
      if (numberString && strlen(numberString) > 0)
      {
	answerFound=TRUE;
	if (sScanF(&numberString[1],"%g %g %g %g", &r, &g, &b, &a) == 4)
	{
	  ambient[win_number][lightN][0] = r;
	  ambient[win_number][lightN][1] = g;
	  ambient[win_number][lightN][2] = b;
	  ambient[win_number][lightN][3] = a;
	}
	else
	{
	  answerFound=FALSE;
	  outputString("Unknown resonse:");
	  outputString(ans);
	}
      }
    }
    else
      answerFound=FALSE;
  }//                    0123456789
  else if(answer.substr(0,7) =="DIFFUSE")
  {
// answer is of the form 
// DIFFUSE#0:1 r g b a, to set the (r,g,b,a) values of light 0 in window 1
    const char *ans=answer.c_str();
    const char *hash=strpbrk(ans,"#");
    int lightN=0;
    real r, g, b, a;
    if (hash && strlen(hash) > 0)
    {
      sScanF(&hash[1],"%i", &lightN);
    
      const char *numberString=strpbrk(ans," ");
      if (numberString && strlen(numberString) > 0)
      {
	answerFound=TRUE;
	if (sScanF(&numberString[1],"%g %g %g %g", &r, &g, &b, &a) == 4)
	{
	  diffuse[win_number][lightN][0] = r;
	  diffuse[win_number][lightN][1] = g;
	  diffuse[win_number][lightN][2] = b;
	  diffuse[win_number][lightN][3] = a;
	}
	else
	{
	  answerFound=FALSE;
	  outputString("Unknown resonse:");
	  outputString(ans);
	}
      }
    }
    else
      answerFound=FALSE;
  }//                    0123456789
  else if(answer.substr(0,8) =="SPECULAR")
  {
// answer is of the form 
// SPECULAR#0:1 r g b a, to set the (r,g,b,a) values of light 0 in window 1
    const char *ans=answer.c_str();
    const char *hash=strpbrk(ans,"#");
    int lightN=0;
    real r, g, b, a;
    if (hash && strlen(hash) > 0)
    {
      sScanF(&hash[1],"%i", &lightN);
    
      const char *numberString=strpbrk(ans," ");
      if (numberString && strlen(numberString) > 0)
      {
	answerFound=TRUE;
	if (sScanF(&numberString[1],"%g %g %g %g", &r, &g, &b, &a) == 4)
	{
	  specular[win_number][lightN][0] = r;
	  specular[win_number][lightN][1] = g;
	  specular[win_number][lightN][2] = b;
	  specular[win_number][lightN][3] = a;
	}
	else
	{
	  answerFound=FALSE;
	  outputString("Unknown resonse:");
	  outputString(ans);
	}
      }
    }
    else
      answerFound=FALSE;
  }//                     01234567890123456789
  else if(answer.substr(0,15) =="MATERIALAMBIENT")
  {
// answer is of the form 
// MATERIALAMBIENT:1 r g b a, to set the material (r,g,b,a) values in window 1
    const char *ans=answer.c_str();
    real r, g, b, a;
    const char *numberString=strpbrk(ans," ");
    if (numberString && strlen(numberString) > 0)
    {
      answerFound=TRUE;
      if (sScanF(&numberString[1],"%g %g %g %g", &r, &g, &b, &a) == 4)
      {
	materialAmbient[win_number][0] = r;
	materialAmbient[win_number][1] = g;
	materialAmbient[win_number][2] = b;
	materialAmbient[win_number][3] = a;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient[win_number]);
	moglPostDisplay(win_number);
      }
      else
      {
	answerFound=FALSE;
	outputString("Unknown resonse:");
	outputString(ans);
      }
    }
    else
      answerFound=FALSE;
  }//                     01234567890123456789
  else if(answer.substr(0,15) =="MATERIALDIFFUSE")
  {
// answer is of the form 
// MATERIALDIFFUSE:1 r g b a, to set the material (r,g,b,a) values in window 1
    const char *ans=answer.c_str();
    real r, g, b, a;
    const char *numberString=strpbrk(ans," ");
    if (numberString && strlen(numberString) > 0)
    {
      answerFound=TRUE;
      if (sScanF(&numberString[1],"%g %g %g %g", &r, &g, &b, &a) == 4)
      {
	materialDiffuse[win_number][0] = r;
	materialDiffuse[win_number][1] = g;
	materialDiffuse[win_number][2] = b;
	materialDiffuse[win_number][3] = a;
      }
      else
      {
	answerFound=FALSE;
	outputString("Unknown resonse:");
	outputString(ans);
      }
    }
    else
      answerFound=FALSE;
  }//                     01234567890123456789
  else if(answer.substr(0,16) =="MATERIALSPECULAR")
  {
// answer is of the form 
// MATERIALSPECULAR:1 r g b a, to set the material (r,g,b,a) values in window 1
    const char *ans=answer.c_str();
    real r, g, b, a;
    const char *numberString=strpbrk(ans," ");
    if (numberString && strlen(numberString) > 0)
    {
      answerFound=TRUE;
      if (sScanF(&numberString[1],"%g %g %g %g", &r, &g, &b, &a) == 4)
      {
	materialSpecular[win_number][0] = r;
	materialSpecular[win_number][1] = g;
	materialSpecular[win_number][2] = b;
	materialSpecular[win_number][3] = a;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular[win_number]);
	moglPostDisplay(win_number);
      }
      else
      {
	answerFound=FALSE;
	outputString("Unknown resonse:");
	outputString(ans);
      }
    }
    else
      answerFound=FALSE;
  }//                    0123456780
  else if(answer.substr(0,9) =="SHININESS")
  {
// answer is of the form 
// SHININESS:1 s, to set the shininess in window 1
    const char *ans=answer.c_str();
    real s;
    const char *numberString=strpbrk(ans," ");
    if (numberString && strlen(numberString) > 0)
    {
      answerFound=TRUE;
      if (sScanF(&numberString[1],"%g", &s) == 1)
      {
	materialShininess[win_number] = s;
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess[win_number]); 
      }
      else
      {
	answerFound=FALSE;
	outputString("Unknown resonse:");
	outputString(ans);
      }
    }
    else
      answerFound=FALSE;
  }//                     01234567890123456789
  else if(answer.substr(0,11) =="SCALEFACTOR")
  {
// answer is of the form 
// SCALEFACTOR:1 sf, to set the scalefactor in window 1
    const char *ans=answer.c_str();
    real s;
    const char *numberString=strpbrk(ans," ");
    if (numberString && strlen(numberString) > 0)
    {
      answerFound=TRUE;
      if (sScanF(&numberString[1],"%g", &s) == 1)
      {
	materialScaleFactor[win_number] = s;
      }
      else
      {
	answerFound=FALSE;
	outputString("Unknown resonse:");
	outputString(ans);
      }
    }
    else
      answerFound=FALSE;
  }//                     01234567890123456789
  else if( answer.substr(0,17)=="Background colour" )
  {
// answer of the form
// Background colour:1 red to set the background colour in window 1 to red.
    const char *ans=answer.c_str();
    const char *colourString=strpbrk(&ans[18]," "); // look after the colon
    if (colourString && strlen(colourString) > 0)
    {
      answerFound=TRUE;
      char colour[50];
      sScanF(&colourString[1], "%s", colour);
//      printf("Background colour: `%s'\n", colour);
      
      backGroundName[win_number]=colour;
      strcpy(viewChar[win_number].backGroundName, colour);

      getXColour(backGroundName[win_number], backGround[win_number]);
      moglPostDisplay(win_number);
    }
    else
    {
      answerFound=FALSE;
      outputString("Unknown resonse:");
      outputString(ans);
    }
    
  }//                     01234567890123456789
  else if( answer.substr(0,17)=="Foreground colour" )
  {
// answer of the form
// Foreground colour:1 red; to set the foreground colour in window 1 to red.
    const char *ans=answer.c_str();
    const char *colourString=strpbrk(&ans[18]," "); // look after the colon
    if (colourString && strlen(colourString) > 0)
    {
      answerFound=TRUE;
      char colour[50];
      sScanF(&colourString[1], "%s", colour);
//      printf("Foreground colour: `%s'\n", colour);
      
      foreGroundName[win_number]=colour;
      strcpy(viewChar[win_number].foreGroundName, colour);

      getXColour(foreGroundName[win_number], foreGround[win_number]);
      moglPostDisplay(win_number);
    }
    else
    {
      answerFound=FALSE;
      outputString("Unknown resonse:");
      outputString(ans);
    }
    
  }
  else if( answer.substr(0,28) =="Axes origin at default point")
  {
    axesOriginOption[win_number] = 0;
    answerFound=TRUE;
    moglPostDisplay(win_number);
  }//                      012345678901234567890123456789
  else if( answer.substr(0,29) =="Axes origin at rotation point")
  {
    axesOriginOption[win_number] = 1;
    answerFound=TRUE;
    moglPostDisplay(win_number);
  }
  else if( answer.substr(0,4)=="CLIP" )  // This is where the clipping plane callbacks are catched
  {
    answerFound=TRUE;

    // answers of the form:
    //   clip i on/off
    //   clip i normal %e %e %e 
    //   clip i distance % e

    int length=answer.length();
    int numberOfTokens =0;
    int startOfToken[10];
    for( int i=4; i<length-1; i++ )
    {
      if( answer[i]==' ' && answer[i+1]!=' ' )
      {
	startOfToken[numberOfTokens++]=i+1;
	if( numberOfTokens > 5 )
	  break;
      }
    }
    startOfToken[numberOfTokens]=length;
    
    int plane=-1;
    // cout << "token 0 =[" << answer(startOfToken[0],startOfToken[1]-1) << "]\n";
    sScanF( answer.substr(startOfToken[0],startOfToken[1]-startOfToken[0]),"%i",&plane ); 
    if( numberOfTokens < 2 || plane <0 || plane>=maximumNumberOfClippingPlanes )
    {
      cout << "invalid clipping plane command, input string = " << answer << endl;
    }
    else
    {
      // cout << "token 1 =[" << answer(startOfToken[1],startOfToken[2]-1) << "]\n";
      if( answer.substr(startOfToken[1],2)=="on" )
      {
	if( !clippingPlaneIsOn[win_number][plane] )
	{ // turn this plane on if it is not already on
	  numberOfClippingPlanes[win_number]++;
	  clippingPlaneIsOn[win_number][plane]=TRUE;
	  // printf("turn on plane %i \n",plane);
	}
      }
      else if( answer.substr(startOfToken[1],3)=="off" )
      {
	if( clippingPlaneIsOn[win_number][plane] )
	{ // turn this plane off if it is not already off
	  numberOfClippingPlanes[win_number]--;
	  clippingPlaneIsOn[win_number][plane]=FALSE;
	  // printf("turn off plane %i \n",plane);
	}
      }
      else if( numberOfTokens>=5 && answer.substr(startOfToken[1],6)=="normal" )
      {
//	cout << "token 2+ =[" << answer(startOfToken[2],length-1) << "]\n";

	real n0=0.,n1=0.,n2=-1.;
	sScanF( answer.substr(startOfToken[2],length-startOfToken[2]),"%e %e %e",&n0,&n1,&n2 ); 
	printf("set normal=(%e,%e,%e) for clipping plane %i \n",n0,n1,n2,plane);
	real norm=SQRT( SQR(n0)+SQR(n1)+SQR(n2) );
	if( norm == 0. )
	{
	  cout << "invalid clipping plane normal, cannot be all zeroes! \n";
	  n2=-1;
	  norm=1.;
	}
	clippingPlaneEquation[win_number][plane][0]=n0/norm;   // normal(0)
	clippingPlaneEquation[win_number][plane][1]=n1/norm;   // normal(1)
	clippingPlaneEquation[win_number][plane][2]=n2/norm;   // normal(2)

      }
      else if( numberOfTokens>=3 && answer.substr(startOfToken[1],8)=="distance" )
      {
	real distance=0.;
	sScanF( answer.substr(startOfToken[2],length-startOfToken[2]),"%e",&distance ); 
	// printf("set distance=%e for plane %i \n",distance,plane);
	clippingPlaneEquation[win_number][plane][3]=distance;   // note plus
      }
      else
      {
	cout << "invalid clipping plane command, input string = " << answer << endl;
      }
    }
    moglPostDisplay(win_number); 
  }
  else if( answer.substr(0,23)=="line width scale factor" ) // AP: the scale factor is appended to the command now
  {                 //    012345678901234567890123456789
// line width scale factor:1 2.5; to set the scale factor in window 1 to 2.5.
    const char *ans=answer.c_str();
    const char *lsString=strpbrk(&ans[23]," "); // look after the colon
    if (lsString && strlen(lsString) > 0)
    {
      real scaleFactor;
      sScanF( lsString,"%e",&scaleFactor ); 
      if (scaleFactor > 0)
      {
	setLineWidthScaleFactor( scaleFactor, win_number );
	outputString("The new line width will only appear on new things drawn.");
      }
      else
      {
	outputString("ERROR: The line width scale factor must be positive");
// Reset the value in the text box in the view char dialog
	moglPrintLineWidth(lineWidthScaleFactor[win_number], win_number);
      }
      
    }
  }
  else if( len=str_matches(answer,"fraction of screen") ) 
  { 
    const char *ans=answer.c_str();
    const char *lsString=strpbrk(&ans[len]," "); // look after the colon
    if (lsString && strlen(lsString) > 0)
    {
      real fraction;
      sScanF( lsString,"%e",&fraction ); 
      if (fraction > 0)
      {
	setFractionOfScreen( fraction, win_number );
	outputString("The new fraction of screen will only appear on new things drawn.");
      }
      else
      {
	outputString("ERROR: The fraction of screen must be positive");
        // Reset the value in the text box in the view char dialog
	moglPrintFractionOfScreen(fractionOfScreen[win_number], win_number);
      }
      
    }
  }
  else if( len=str_matches(answer,"include ") )
  {      
    aString newCommandFileName=nullString;
    if( len>0 )
      newCommandFileName=answer.substr(len,answer.length()-len);
    if (readCommandFile(newCommandFileName))
      disableGUI(); // this must be called after the file is opened, otherwise some dialogs become
    // activated by the popGUI that is done in readCommandFile.
  }
  else
    answerFound=FALSE;

  if( answerFound )
  {
    if (reDisplay) 
      moglPostDisplay(win_number);
    if( !alreadySaved )
      appendCommandHistory(answer);
    
    if( saveFile && !alreadySaved && !readFile )
    {
      fPrintF(saveFile,"%s\n",(const char*)(indentBlanks+answer).c_str()); // save in the log file
      fflush(saveFile);
    }
    
  }
// Here we look for a menu item that appears in any of the pull-down menus, File, View, Options or 
// Help that do not have hardwired callbacks (like clipping planes or annotate)
  else if( (item=getMatch(menuBarItems,answer)) >= 0 )
  {
    const char *cColon = strrchr(SC answer.c_str(),':');
    aString longAnswer;
    if (cColon && cColon != '\0')
      longAnswer = menuBarItems[item] + cColon; /*answer*/
    else
      longAnswer = menuBarItems[item];
    
    answerFound=TRUE;

    appendCommandHistory(longAnswer);
    if( saveFile && !readFile )
    {
      fPrintF(saveFile,"%s\n",(const char*)(indentBlanks+longAnswer).c_str()); // save in the log file
      fflush(saveFile);
    }
    

// we already have the window number from above

    if( longAnswer.substr(0,18)=="read command file" )
    {                //    012345678901234567890123456789
      if (readCommandFile())
	disableGUI(); // this must be called after the file is opened, otherwise some dialogs become
                      // activated by the popGUI that is done in readCommandFile.
    }
    else if( longAnswer.substr(0,20)=="log commands to file" )
    {
      saveCommandFile(); 
    }
    else if( longAnswer.substr(0,6)=="figure" )
    {
      char prompt[120], errPrompt[120];
      sprintf( errPrompt, "Invalid window number. Must be within [0, %i]. Try again.", moglGetNWindows()-1 );
      sprintf( prompt, "Enter window to activate (default %i)", currentWindow);
      inputString(line, prompt);
      if( line!="" )
      {
	sScanF(line,"%d",&win_number);
	if (0<=win_number && win_number<moglGetNWindows())
	  setCurrentWindow(win_number);
	else
	  outputString(errPrompt);
      }
    }
    else if( longAnswer.substr(0,5)=="pause" )
      pause(); 
    else if( longAnswer.substr(0,5)=="abort" )
      exit(0);//               01234567890123456789
    else if( answer.substr(0,8)=="annotate" )
//                   //    012345678901234567890123456789
      annotate(answer);
    else if( str_matches(longAnswer,"turn off parser") )
    {
      useParser=false;
    }
    else if( str_matches(longAnswer,"turn on parser") )
    {
      useParser=true;
    }
    else if( displayHelp( answer ) )
    {
    }
    else
    {
      cout << "GL_GraphicsInterface::processSpecialMenuItems:ERROR! this should not happen\n";
      answerFound=FALSE;
    }
      
  } // end if (menuselected >= 0)

  if (openingTheGUI) answerFound=FALSE;
  
  return answerFound;
}

void GL_GraphicsInterface::
hardcopyCommands(aString &longAnswer, int win_number)
{
  aString buf;

  aString arg;
  const char * cval=NULL;
  aString command;
  parseCommandLine( longAnswer,command,win_number,arg );
  cval=(const char*) arg.c_str();
      
  if( longAnswer.substr(0,15)=="hardcopy format" )
  {
    const char *format = NULL;
    if (cval)
      format = cval;
    else
      format = "unknown";

    if (!strcmp(format,"PS"))
      hardCopyType[win_number]=GraphicsParameters::postScript;
    else if (!strcmp(format,"RasterPS"))
      hardCopyType[win_number]=GraphicsParameters::postScriptRaster;
    else if (!strcmp(format,"EPS"))
      hardCopyType[win_number]=GraphicsParameters::encapsulatedPostScript;
    else if (!strcmp(format,"ppm"))
      hardCopyType[win_number]=GraphicsParameters::ppm;
    else
      outputString(sPrintF(buf,"Unknown format: %s", format));

    printf("hardcopy format number: %i\n", hardCopyType[win_number]);
  }//                         01234567890123456789
  else if( longAnswer.substr(0,15)=="hardcopy colour" )
  {                     //    012345678901234567890123456789
    const char *colour = NULL;
    if (cval)
      colour = cval;
    else
      colour = "unknown";

    if (!strcmp(colour, "8bit"))
    {
      outputFormat[win_number]=(GraphicsParameters::OutputFormat) 0;
    }
    else if (!strcmp(colour, "24bit"))
    {
      outputFormat[win_number]=(GraphicsParameters::OutputFormat) 1;
    }
    else if (!strcmp(colour, "BW"))
    {
      outputFormat[win_number]=(GraphicsParameters::OutputFormat) 2;
    }
    else if (!strcmp(colour, "Gray"))
    {
      outputFormat[win_number]=(GraphicsParameters::OutputFormat) 3;
    }
    else
    {
      outputString(sPrintF(buf,"Unknown colour scheme: %s. Not changing the colour.", colour));
    }
      
    printf("hardcopy colour number: %i\n", outputFormat[win_number]);
  }
  else if( str_matches(longAnswer,"hardcopy resolution") )
  {                    
    // for compatibility we keep the old command that sets both horizontal and vertical *wh* 010515
    GLint params[8];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS,params);
    int maxResolution = params[1];
    int newResolution = rasterResolution[win_number];
      
    if (cval)
      sscanf(cval,"%d", &newResolution);

    if (newResolution == 0)
      newResolution = maxResolution;
    else
    {
      newResolution = min(newResolution, maxResolution);
      newResolution = max(newResolution, 100);
    }
    rasterResolution[win_number] = newResolution;
    horizontalRasterResolution[win_number] = newResolution;
    
    printf("hardcopy resolution: %i\n", rasterResolution[win_number]);
// write back to the text box
    sPrintF(buf, "%i", rasterResolution[win_number]);
    hardCopyDialog[win_number].setTextLabel(0, buf); // This is text label # 0
//    movieDialog[win_number].setTextLabel(0, buf); // This is text label # 0

    sPrintF(buf, "%i", horizontalRasterResolution[win_number]);
    hardCopyDialog[win_number].setTextLabel(1, buf); // This is text label # 1
//    movieDialog[win_number].setTextLabel(1, buf); // This is text label # 1

  }
  else if( str_matches(longAnswer,"hardcopy vertical resolution") )
  {                     
    GLint params[8];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS,params);
    int maxResolution = params[1];
    int newResolution = rasterResolution[win_number];
      
    if (cval)
      sscanf(cval,"%d", &newResolution);

    if (newResolution == 0)
      newResolution = maxResolution;
    else
    {
      newResolution = min(newResolution, maxResolution);
      newResolution = max(newResolution, 100);
    }
    rasterResolution[win_number] = newResolution;
    printf("hardcopy vertical resolution: %i\n", rasterResolution[win_number]);
// write back to the text box
    sPrintF(buf, "%i", rasterResolution[win_number]);
    hardCopyDialog[win_number].setTextLabel(0, buf); // This is text label # 0
//    movieDialog[win_number].setTextLabel(0, buf); // This is text label # 0

  }
  else if( str_matches(longAnswer,"hardcopy horizontal resolution") )
  {
    GLint params[8];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS,params);
    int maxResolution = params[0];
    int newResolution = horizontalRasterResolution[win_number];
      
    if (cval)
      sscanf(cval,"%d", &newResolution);

    if (newResolution == 0)
      newResolution = maxResolution;
    else
    {
      newResolution = min(newResolution, maxResolution);
      newResolution = max(newResolution, 100);
    }
    horizontalRasterResolution[win_number] = newResolution;
    printf("hardcopy horizontal resolution: %i\n", horizontalRasterResolution[win_number]);
    // write back to the text box
    sPrintF(buf, "%i", horizontalRasterResolution[win_number]);
    hardCopyDialog[win_number].setTextLabel(1, buf); // This is text label # 1
    //    movieDialog[win_number].setTextLabel(1, buf); // This is text label # 1

  }//                     012345678901234567890123456789
  else if( longAnswer.substr(0,18)=="hardcopy file name") 
  {
    // const char *fName = NULL;
    if (cval && strlen(cval)>0 && strcmp(cval, " "))
      hardCopyFile[win_number] = cval;
    else
      outputString("Error: Cannot parse the file name");
    // write back to the text box
    hardCopyDialog[win_number].setTextLabel(2, hardCopyFile[win_number]); // This is text label # 2

    printf("hardcopy file name: %s\n", SC hardCopyFile[win_number].c_str());
  }//                         01234567890123456789
  else if( longAnswer.substr(0,13)=="hardcopy save" )
  {
    hardCopy(hardCopyFile[win_number], Overture::defaultGraphicsParameters(), win_number); 
  }//                         01234567890123456789
  else if( longAnswer.substr(0,15)=="hardcopy browse")
  {
    aString newFileName;
    inputFileName(newFileName, "Enter hardcopy file name>");
    if (newFileName.length() > 0 && newFileName != " ")
    {
      hardCopyFile[win_number] = newFileName;
      // write back to the text box
      hardCopyDialog[win_number].setTextLabel(2, hardCopyFile[win_number]); // This is text label # 2
    }
    else
      outputString(sPrintF(buf,"Bad file name: `%s'", SC newFileName.c_str()));
  }//                         012345678901234567890123456789
  else if( longAnswer.substr(0,21)=="hardcopy close dialog")
  {
    hardCopyDialog[win_number].hideSibling();
  }
  else if( str_matches(longAnswer,"hardcopy rendering") )
  {                    
    aString type=cval;
    if( type=="offScreen" )
    {
      hardCopyRenderingType=GenericGraphicsInterface::offScreenRender;
      printf("Setting hardCopyRenderingType=offScreenRender\n");
    }
    else 
    {
      printf("Setting hardCopyRenderingType=frameBuffer\n");
      hardCopyRenderingType=GenericGraphicsInterface::frameBuffer;
    }
    
  }
  else
  {
    printf(" Unknown hardcopy command: [%s] cval=[%s] \n",(const char *)longAnswer.c_str(),cval);
  }
}

void GL_GraphicsInterface::
optionCommands(aString &answer, int w)
{
  aString command,arg;
  int windowNumber;
  parseCommandLine( answer,command,windowNumber,arg );
  int newState=-1;
  sScanF(arg,"%i", &newState);  

  if (newState!=0 && newState!=1)
  {
    printf("optionCommands: ERROR: can not parse newState=%i\n", newState);
    return;
  }
  
//                     01234567890123456789
  if (str_matches(answer,"DISPLAY AXES"))
  {
    setPlotTheAxes(newState, w);
  }
//                          01234567890123456789
  else if (str_matches(answer,"DISPLAY LABELS"))
  {
    setPlotTheLabels(newState, w);
  }
  else if (str_matches(answer,"DISPLAY ROTATION POINT"))
  {
    setPlotTheRotationPoint(newState, w);
  }
  else if (str_matches(answer,"DISPLAY COLOUR BAR"))
  {
    setPlotTheColourBar(newState, w);
  }
  else if (str_matches(answer,"DISPLAY SQUARES"))
  {
    setPlotTheColouredSquares(newState, w);
  }
  else if (str_matches(answer,"DISPLAY BACKGROUND GRID"))
  {
    setPlotTheBackgroundGrid(newState, w);
  }
  else if (str_matches(answer,"DISPLAY WIRE FRAME ROTATION") )
  {
    simplifyPlotWhenRotating=!simplifyPlotWhenRotating;
  }
  
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setPlotTheAxes}}
void GL_GraphicsInterface::
setPlotTheAxes(bool newState, int win_number /* =-1 */)
// =======================================================================
//  /Description:
// Toggle plotting of coordinate axes on or off.
// /newState(input): Toggle on (true) or off (false).
// /win\_number(optional input): window number. If absent, use the current window.
//
// /Return value: None
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
{
  int w = (win_number == -1)? getCurrentWindow() : win_number;
  
  plotTheAxes[w] = newState;
  optionMenu[w].setToggleState(0, newState);
// must turn off the background grid when the axes are turned off...
  if (!newState)
    setPlotTheBackgroundGrid(newState, w);
  
  redraw();
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getPlotTheAxes}}
bool GL_GraphicsInterface::
getPlotTheAxes(int win_number /* =-1 */)
// =======================================================================
// /win\_number(optional input): window number. If absent, use the current window.
//
// /Return value: true if axes are plotted, otherwise false.
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
{
  int w = (win_number == -1)? getCurrentWindow() : win_number;
  return plotTheAxes[w];
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setAxesDimension}}
void GL_GraphicsInterface::
setAxesDimension(int dim, int win_number /* =-1 */)
// =======================================================================
// /Description:
// Set the dimensionality of the plotted coordinate axes.
// /dim(input): The dimensionality (1-3).
// /win\_number(optional input): window number. If absent, use the current window.
//
// /Return value: None
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
{
  int w = (win_number == -1)? getCurrentWindow() : win_number;
  axesDimension[w] = max(1, min(3, dim));
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setPlotTheLabels}}
void GL_GraphicsInterface::
setPlotTheLabels(bool newState, int win_number /* = -1 */) 
// =======================================================================
//  /Description:
// Toggle plotting of the label on the graphics window.
// /newState(input): Toggle on (true) or off (false).
// /win\_number(optional input): window number. If absent, use the current window.
//
// /Return value: None
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
{
  int w = (win_number == -1)? getCurrentWindow() : win_number;
  plotTheLabels[w] = newState;
  optionMenu[w].setToggleState(1, newState);
  redraw();
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getPlotTheLabels}}
bool GL_GraphicsInterface::
getPlotTheLabels(int win_number /* =-1 */)
// =======================================================================
// /win\_number(optional input): window number. If absent, use the current window.
//
// /Return value: true if labels are plotted, otherwise false.
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
{
  int w = (win_number == -1)? getCurrentWindow() : win_number;
  return plotTheLabels[w];
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setPlotTheRotationPoint}}
void GL_GraphicsInterface::
setPlotTheRotationPoint(bool newState, int win_number /* = -1 */) 
// =======================================================================
//  /Description:
// Toggle plotting of the rotation point in the graphics window.
// /newState(input): Toggle on (true) or off (false).
// /win\_number(optional input): window number. If absent, use the current window.
//
// /Return value: None
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
{
  int w = (win_number == -1)? getCurrentWindow() : win_number;
  plotTheRotationPoint[w] = newState;
  optionMenu[w].setToggleState(2, newState);
  redraw();
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getPlotTheRotationPoint}}
bool GL_GraphicsInterface::
getPlotTheRotationPoint(int win_number /* =-1 */)
// =======================================================================
// /win\_number(optional input): window number. If absent, use the current window.
//
// /Return value: true if the rotation point is plotted, otherwise false.
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
{
  int w = (win_number == -1)? getCurrentWindow() : win_number;
  return plotTheRotationPoint[w];
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setPlotTheColourBar}}
void GL_GraphicsInterface::
setPlotTheColourBar(bool newState, int win_number /* = -1 */) 
// =======================================================================
//  /Description:
// Toggle plotting of the colour bar in the graphics window.
// /newState(input): Toggle on (true) or off (false).
// /win\_number(optional input): window number. If absent, use the current window.
//
// /Return value: None
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
{
  int w = (win_number == -1)? getCurrentWindow() : win_number;
  plotTheColourBar[w] = newState;
  optionMenu[w].setToggleState(3, newState);
  redraw();
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getPlotTheColourBar}}
bool GL_GraphicsInterface::
getPlotTheColourBar(int win_number /* =-1 */)
// =======================================================================
// /win\_number(optional input): window number. If absent, use the current window.
//
// /Return value: true if the colour bar is plotted, otherwise false.
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
{
  int w = (win_number == -1)? getCurrentWindow() : win_number;
  return plotTheColourBar[w];
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setPlotTheColouredSquares}}
void GL_GraphicsInterface::
setPlotTheColouredSquares(bool newState, int win_number /* = -1 */) 
// =======================================================================
//  /Description:
// Toggle plotting of the coloured squares (grid labels) in the graphics window.
// /newState(input): Toggle on (true) or off (false).
// /win\_number(optional input): window number. If absent, use the current window.
//
// /Return value: None
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
{
  int w = (win_number == -1)? getCurrentWindow() : win_number;
  plotTheColouredSquares[w] = newState;
  optionMenu[w].setToggleState(4, newState);
  redraw();
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getPlotTheColouredSquares}}
bool GL_GraphicsInterface::
getPlotTheColouredSquares(int win_number /* =-1 */)
// =======================================================================
// /win\_number(optional input): window number. If absent, use the current window.
//
// /Return value: true if the coloured squares (grid labels) are plotted, otherwise false.
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
{
  int w = (win_number == -1)? getCurrentWindow() : win_number;
  return plotTheColouredSquares[w];
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{setPlotTheBackgroundGrid}}
void GL_GraphicsInterface::
setPlotTheBackgroundGrid(bool newState, int win_number /* = -1 */) 
// =======================================================================
//  /Description:
// Toggle plotting of the background grid in the graphics window. Currently only implemented for
// one and two dimensional plots.
// /newState(input): Toggle on (true) or off (false).
// /win\_number(optional input): window number. If absent, use the current window.
//
// /Return value: None
//  /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
{
  int w = (win_number == -1)? getCurrentWindow() : win_number;
  plotBackGroundGrid[w] = newState;
  optionMenu[w].setToggleState(5, newState);
// must turn on the axes to see the background grid
  if (newState)
    setPlotTheAxes(newState, w);
  redraw();
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getPlotTheBackgroundGrid}}
bool GL_GraphicsInterface::
getPlotTheBackgroundGrid(int win_number /* =-1 */)
// =======================================================================
// /win\_number(optional input): window number. If absent, use the current window.
//
// /Return value: true if the background grid is plotted, otherwise false.
// /Author: AP
//\end{GL_GraphicsInterfaceInclude.tex} 
{
  int w = (win_number == -1)? getCurrentWindow() : win_number;
  return plotBackGroundGrid[w];
}

// *wdh* removed 100912
// void GL_GraphicsInterface::
// movieCommands(aString &longAnswer, int win_number)
// {
//   aString buf;
//   aString arg;
//   const char * cval=NULL;
//   aString command;
//   parseCommandLine( longAnswer,command,win_number,arg );
//   cval=(const char*) arg.c_str();

// //                       01234567890123456789
//   if( longAnswer.substr(0,12)=="MOVIE format" )
//   {
//     const char *format = NULL;
//     if (cval)
//       format = cval;
//     else
//       format = "unknown";

//     if (!strcmp(format,"PS"))
//       hardCopyType[win_number]=GraphicsParameters::postScript;
//     else if (!strcmp(format,"RasterPS"))
//       hardCopyType[win_number]=GraphicsParameters::postScriptRaster;
//     else if (!strcmp(format,"EPS"))
//       hardCopyType[win_number]=GraphicsParameters::encapsulatedPostScript;
//     else if (!strcmp(format,"ppm"))
//       hardCopyType[win_number]=GraphicsParameters::ppm;
//     else
//       outputString(sPrintF(buf,"Unknown format: %s", format));

//     printf("hardcopy format number: %i\n", hardCopyType[win_number]);
//   }//                         01234567890123456789
//   else if( longAnswer.substr(0,12)=="MOVIE colour" )
//   {                     //    012345678901234567890123456789
//     const char *colour = NULL;
//     if (cval)
//       colour = cval;
//     else
//       colour = "unknown";

//     if (!strcmp(colour, "8 bit"))
//     {
//       outputFormat[win_number]=(GraphicsParameters::OutputFormat) 0;
//     }
//     else if (!strcmp(colour, "24 bit"))
//     {
//       outputFormat[win_number]=(GraphicsParameters::OutputFormat) 1;
//     }
//     else if (!strcmp(colour, "BW"))
//     {
//       outputFormat[win_number]=(GraphicsParameters::OutputFormat) 2;
//     }
//     else if (!strcmp(colour, "Gray"))
//     {
//       outputFormat[win_number]=(GraphicsParameters::OutputFormat) 3;
//     }
//     else
//     {
//       outputString(sPrintF(buf,"Unknown colour scheme: %s. Not changing the colour.", colour));
//     }
      
//     printf("hardcopy colour number: %i\n", outputFormat[win_number]);
//   }
//   else if( longAnswer.substr(0,16)=="MOVIE resolution" )
//   {                     //    012345678901234567890123456789
//     GLint params[8];
//     glGetIntegerv(GL_MAX_VIEWPORT_DIMS,params);
//     int maxResolution = min(params[0],params[1]);
//     int newResolution = rasterResolution[win_number];
      
//     if (cval)
//       sscanf(cval,"%d", &newResolution);

//     if (newResolution == 0)
//       newResolution = maxResolution;
//     else
//     {
//       newResolution = min(newResolution, maxResolution);
//       newResolution = max(newResolution, 100);
//     }
//     rasterResolution[win_number] = newResolution;
//     printf("hardcopy resolution: %i\n", rasterResolution[win_number]);
// // write back to the text box
//     sPrintF(buf, "%i", rasterResolution[win_number]);
//     movieDialog[win_number].setTextLabel(0, buf); // This is text label # 0
//     hardCopyDialog[win_number].setTextLabel(0, buf); // This is text label # 0

//   }
//   else if( str_matches(longAnswer,"MOVIE horizontal resolution") )
//   {
//     GLint params[8];
//     glGetIntegerv(GL_MAX_VIEWPORT_DIMS,params);
//     int maxResolution = min(params[0],params[1]);
//     int newResolution = horizontalRasterResolution[win_number];
      
//     if (cval)
//       sscanf(cval,"%d", &newResolution);

//     if (newResolution == 0)
//       newResolution = maxResolution;
//     else
//     {
//       newResolution = min(newResolution, maxResolution);
//       newResolution = max(newResolution, 100);
//     }
//     horizontalRasterResolution[win_number] = newResolution;
//     printf("hardcopy horizontal resolution: %i\n", horizontalRasterResolution[win_number]);
// // write back to the text box
//     sPrintF(buf, "%i", horizontalRasterResolution[win_number]);
//     movieDialog[win_number].setTextLabel(1, buf); // This is text label # 1
//     hardCopyDialog[win_number].setTextLabel(1, buf); // This is text label # 1

//   }//                        012345678901234567890123456789
//   else if( longAnswer.substr(0,15)=="MOVIE base name" )
//   {                     //    012345678901234567890123456789
//     aString newBaseName = movieBaseName[win_number];
      
//     if (cval && strcspn(cval," \t")>0 )
//     {
//       newBaseName = cval;
//     }

//     movieBaseName[win_number] = newBaseName;
    
// // write back to the text box
//     movieDialog[win_number].setTextLabel(2, movieBaseName[win_number]); // This is text label # 2

//   }//                        012345678901234567890123456789
//   else if( longAnswer.substr(0,22)=="MOVIE number of frames" )
//   {                     //    012345678901234567890123456789
//     int newNumberOfFrames = numberOfMovieFrames[win_number];
      
//     if (cval)
//       sscanf(cval,"%d", &newNumberOfFrames);

//     if (newNumberOfFrames > 0 && newNumberOfFrames < 1000)
//       numberOfMovieFrames[win_number] = newNumberOfFrames;
    
// // write back to the text box
//     sPrintF(buf, "%i", numberOfMovieFrames[win_number]);
//     movieDialog[win_number].setTextLabel(3, buf); // This is text label # 3

//   }//                        012345678901234567890123456789
//   else if( longAnswer.substr(0,20)=="MOVIE starting frame" )
//   {                     //    012345678901234567890123456789
//     int newFirstFrame = movieFirstFrame[win_number];
      
//     if (cval)
//       sscanf(cval,"%d", &newFirstFrame);

//     if (newFirstFrame >= 0)
//       movieFirstFrame[win_number] = newFirstFrame;
    
// // write back to the text box
//     sPrintF(buf, "%i", movieFirstFrame[win_number]);
//     movieDialog[win_number].setTextLabel(4, buf); // This is text label # 4

//   }//                        012345678901234567890123456789
//   else if( longAnswer.substr(0,14)=="MOVIE delta xr" )
//   {                     //    012345678901234567890123456789
//     real newDxRot = movieDxRot[win_number];
      
//     if (cval)
//       sScanF(cval,"%e", &newDxRot);

//     if (fabs(newDxRot) < 1000)
//       movieDxRot[win_number] = newDxRot;
    
// // write back to the text box
//     sPrintF(buf, "%g", movieDxRot[win_number]);
//     movieDialog[win_number].setTextLabel(5, buf); // This is text label # 5

//   }//                        012345678901234567890123456789
//   else if( longAnswer.substr(0,14)=="MOVIE delta yr" )
//   {                     //    012345678901234567890123456789
//     real newDyRot = movieDyRot[win_number];
      
//     if (cval)
//       sScanF(cval,"%e", &newDyRot);

//     if (fabs(newDyRot) < 1000)
//       movieDyRot[win_number] = newDyRot;
    
// // write back to the text box
//     sPrintF(buf, "%g", movieDyRot[win_number]);
//     movieDialog[win_number].setTextLabel(6, buf); // This is text label # 6

//   }//                        012345678901234567890123456789
//   else if( longAnswer.substr(0,14)=="MOVIE delta zr" )
//   {                     //    012345678901234567890123456789
//     real newDzRot = movieDzRot[win_number];
      
//     if (cval)
//       sScanF(cval,"%e", &newDzRot);

//     if (fabs(newDzRot) < 1000)
//       movieDzRot[win_number] = newDzRot;
    
// // write back to the text box
//     sPrintF(buf, "%g", movieDzRot[win_number]);
//     movieDialog[win_number].setTextLabel(7, buf); // This is text label # 7

//   }//                        012345678901234567890123456789
//   else if( longAnswer.substr(0,13)=="MOVIE delta x" )
//   {                     //    012345678901234567890123456789
//     real newDxTrans = movieDxTrans[win_number];
      
//     if (cval)
//       sScanF(cval,"%e", &newDxTrans);

//     if (fabs(newDxTrans) < 1000)
//       movieDxTrans[win_number] = newDxTrans;
    
// // write back to the text box
//     sPrintF(buf, "%g", movieDxTrans[win_number]);
//     movieDialog[win_number].setTextLabel(8, buf); // This is text label # 8

//   }//                        012345678901234567890123456789
//   else if( longAnswer.substr(0,13)=="MOVIE delta y" )
//   {                     //    012345678901234567890123456789
//     real newDyTrans = movieDyTrans[win_number];
      
//     if (cval)
//       sScanF(cval,"%e", &newDyTrans);

//     if (fabs(newDyTrans) < 1000)
//       movieDyTrans[win_number] = newDyTrans;
    
// // write back to the text box
//     sPrintF(buf, "%g", movieDyTrans[win_number]);
//     movieDialog[win_number].setTextLabel(9, buf); // This is text label # 9

//   }//                        012345678901234567890123456789
//   else if( longAnswer.substr(0,13)=="MOVIE delta z" )
//   {                     //    012345678901234567890123456789
//     real newDzTrans = movieDzTrans[win_number];
      
//     if (cval)
//       sScanF(cval,"%e", &newDzTrans);

//     if (fabs(newDzTrans) < 1000)
//       movieDzTrans[win_number] = newDzTrans;
    
// // write back to the text box
//     sPrintF(buf, "%g", movieDzTrans[win_number]);
//     movieDialog[win_number].setTextLabel(10, buf); // This is text label # 10

//   }//                        012345678901234567890123456789
//   else if( longAnswer.substr(0,19)=="MOVIE relative zoom" )
//   {                     //    012345678901234567890123456789
//     real newRelZoom = movieRelZoom[win_number];
      
//     if (cval)
//       sScanF(cval,"%e", &newRelZoom);

//     if (newRelZoom >= 0.5 && newRelZoom<=1.5)
//       movieRelZoom[win_number] = newRelZoom;
    
// // write back to the text box
//     sPrintF(buf, "%g", movieRelZoom[win_number]);
//     movieDialog[win_number].setTextLabel(11, buf); // This is text label # 11

//   }//                        012345678901234567890123456789
//   else if( longAnswer.substr(0,10)=="MOVIE save" )
//   {                     //    012345678901234567890123456789
//     int newSave = -1;
      
//     if (cval)
//       sscanf(cval,"%d", &newSave);

//     if (newSave == 0 || newSave == 1)
//     {
//       saveMovie[win_number] = (newSave == 1);
//     }
    
// // set the state of the toggle button
//     movieDialog[win_number].setToggleState(0, saveMovie[win_number]); // This is toggle button # 0

//   }//                         012345678901234567890123456789
//   else if( longAnswer.substr(0,15)=="MOVIE file name") 
//   {
//     const char *fName = NULL;
//     if (cval && strlen(cval)>0 && strcmp(cval, " "))
//       hardCopyFile[win_number] = cval;
//     else
//       outputString("Error: Cannot parse the file name");
// // write back to the text box
//     movieDialog[win_number].setTextLabel(2, hardCopyFile[win_number]); // This is text label # 2

//     printf("hardcopy file name: %s\n", SC hardCopyFile[win_number].c_str());
//   }//                         01234567890123456789
//   else if( longAnswer.substr(0,13)=="MOVIE action" )
//   { 
//     int q, w=win_number;
//     aString extension="";
//     if (saveMovie[w])
//     {
//       switch(hardCopyType[w])
//       {
//       case GraphicsParameters::postScript:
// 	extension = ".ps";
// 	break;
//       case GraphicsParameters::postScriptRaster:
// 	extension = ".ps";
// 	break;
//       case GraphicsParameters::encapsulatedPostScript:
// 	extension = ".eps";
// 	break;
//       case GraphicsParameters::ppm:
// 	extension = ".ppm";
// 	break;
//       default:
// 	break;
//       }
//     }
    
// // save the first frame
//     aString frameName;
//     redraw(TRUE); // immediate redraw *wdh* 010321
//     if (saveMovie[w])
//     {
//       sPrintF(frameName,"%s%i%s", SC movieBaseName[w].c_str(), movieFirstFrame[w], SC extension.c_str());
//       hardCopy(frameName, Overture::defaultGraphicsParameters(), win_number); 
//     }
// // save the rest of the frames
//     for (q=1; q<numberOfMovieFrames[w]; q++) // loops numberOfMovieFrames - 1 times
//     {
//       changeView(win_number, 
// 		 movieDxTrans[w],  movieDyTrans[w],  movieDzTrans[w], 
// 		 movieDxRot[w], movieDyRot[w], movieDzRot[w], 
// 		 movieRelZoom[w]);
//       redraw(TRUE); // immediate redraw
//       if (saveMovie[w])
//       {
// 	sPrintF(frameName,"%s%i%s", SC movieBaseName[w].c_str(), movieFirstFrame[w] + q, SC extension.c_str());
// 	hardCopy(frameName, Overture::defaultGraphicsParameters(), win_number); 
//       }
//     }
//     if (saveMovie[w])
//     {
// // update movieFirstFrame so we get a contiguous numbering of the files
//       movieFirstFrame[w] += numberOfMovieFrames[w];
//       sPrintF(buf, "%i", movieFirstFrame[w]); 
//       movieDialog[w].setTextLabel(4, buf); //This is text label #4
//     }
    
//   }//                         01234567890123456789
//   else if( longAnswer.substr(0,12)=="MOVIE browse")
//   {
//     aString newFileName;
//     inputFileName(newFileName, "Enter hardcopy file name>");
//     if (newFileName.length() > 0 && newFileName != " ")
//     {
//       hardCopyFile[win_number] = newFileName;
// // write back to the text box
//       movieDialog[win_number].setTextLabel(2, hardCopyFile[win_number]); // This is text label # 2
//     }
//     else
//       outputString(sPrintF(buf,"Bad file name: `%s'", SC newFileName.c_str()));
//   }//                         012345678901234567890123456789
//   else if( longAnswer.substr(0,18)=="MOVIE close dialog")
//   {
//     movieDialog[win_number].hideSibling();
//   }
    
//   // -------------------- Options menu items ---------------------
// }


void GL_GraphicsInterface::
openGUI()
{
  int i;
// open dialogs on all previous levels: 
  if (currentGUI)
  {
    GUIState *prevGUI;
    GUIState *allGUI[100]; // can't be more than 100 levels deep...
    int nGUI = 0, j;
    
    // This ordering would put the last dialog at the top and the first at the bottom
    for (prevGUI = currentGUI->prev; prevGUI != NULL; prevGUI = prevGUI->prev)
    {
      allGUI[nGUI++] = prevGUI;
    }
    // now reverse the order so that the last dialog appears at the bottom and the first at the top
    for (j=nGUI-1; j>=0; j--)
    {
      prevGUI = allGUI[j];
      prevGUI->openDialog();
      // make them insensitive
      prevGUI->setSensitive( 0 );
      // make all dialog siblings, but don't display them yet
      for (i=0; i<prevGUI->nDialogSiblings; i++)
	prevGUI->dialogSiblings[i].openDialog(0);
    }

    // open the top level dialog last so it will appear on top of all previous dialogs
    currentGUI->openDialog();
    // make all current dialog windows sensitive
    currentGUI->setSensitive(true);

    // make all dialog siblings, but don't display them yet
    for (i=0; i<currentGUI->nDialogSiblings; i++)
    {
      currentGUI->dialogSiblings[i].openDialog(0);
      currentGUI->dialogSiblings[i].setSensitive(true);
    }
  
  

    // loop over all graphics windows to setup buttons and pulldown menus
    for (int win=0; win<moglGetNWindows(); win++)
    {
      // setup the buttons.
      moglBuildUserButtons(currentGUI->windowButtonCommands, currentGUI->windowButtonLabels, win);
      // set the pulldown menu. Make it sensitive only in the current window
      moglBuildUserMenu(currentGUI->pulldownCommand, currentGUI->pulldownTitle, win);
      // Only make the buttons and pulldown menus sensitive on the active window
      moglSetSensitive(win, (win == currentWindow) );
    }
    // build the popup menu in all windows
    moglBuildPopup( currentGUI->popupMenu );
  }
  
}


void GL_GraphicsInterface::
disableGUI()
{
  int i;
// make the dialogs on all current levels insensitive
  if (currentGUI)
  {
    GUIState *prevGUI;
    for (prevGUI = currentGUI; prevGUI != NULL; prevGUI = prevGUI->prev)
    {
// make them insensitive
      prevGUI->setSensitive( 0 );
// make all dialog siblings insensitive too
      for (i=0; i<prevGUI->nDialogSiblings; i++)
	prevGUI->dialogSiblings[i].setSensitive(0);
    }
  }
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{getMenuItem}} 
int GL_GraphicsInterface::
getMenuItem(const aString *menu, aString & answer, const aString & prompt /*=nullString*/)
//=====================================================================================
//
//
// OBSOLETE function used before all old calls to getMenuItem have been replaced by getAnswer()
// OBSOLETE function used before all old calls to getMenuItem have been replaced by getAnswer()
// OBSOLETE function used before all old calls to getMenuItem have been replaced by getAnswer()
//
//
// /Description:
//  Setup a popup menu and wait for a reply.
// /menu (input):
//    The {\ff menu} is
//    an array of Strings (the menu choices) with an empty aString
//    indicating the end of the menu choices. Optionally, a title can be put on top of the menu by 
//    starting the first aString with an `!'. For example,
//    \begin{verbatim}
//       GL_GraphicsInterface ps;
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
// /answer (output): Return the chosen item.
//
// /prompt (input): display the optional prompt message
//  /Return Values: On return "answer" is set equal to the 
//    menu item chosen. The function return value is set equal to
//    the number of the item chosen, starting from zero.
//    Thus, for example, in the above menu if the user picked "erase"
//    the return value would be 2, if the user picked "plot" the
//    return value would be 1, since the title also counts.
// /Author: AP
//
//\end{GL_GraphicsInterfaceInclude.tex} 
//=====================================================================================
{
  int retCode=0;

  GUIState interface;
  interface.buildPopup(menu);
  pushGUI(interface);
  retCode=getAnswer(answer, prompt);
  popGUI();

  return retCode;
}

