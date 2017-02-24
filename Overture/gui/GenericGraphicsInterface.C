#include "GenericGraphicsInterface.h"
#include <string.h> // AP: where is this used???
#include "OvertureParser.h"

#ifdef NO_APP
using GUITypes::real;
using std::cout;
using std::endl;
#endif


GenericGraphicsInterface::
GenericGraphicsInterface()
//=====================================================================================
// /Description:
//   Default constructor;
// /Author: WDH
//
//=====================================================================================
{
  int argc=1;
  char *argv[] = {(char*)"plot",NULL}; 
  constructor(argc,argv);

// the following call is redundant when Overture::getGraphicsInterface() is called
// to instantiate the GI
  Overture::setGraphicsInterface(this); 
}  

GenericGraphicsInterface::
GenericGraphicsInterface(int & argc, char *argv[])
//=====================================================================================
// /Description:
//   This constructor takes the argc and argv from the main program -- The 
//   window manager will strip off any parameters that it recognizes such as the
//   size of the window.
//
// /argc (input/output): The argument count to main.
// /argv (input/output): The arguments to main.
//
//  /Author: WDH
//=====================================================================================
{
  constructor(argc,argv);
// the following call is redundant when Overture::getGraphicsInterface() is called
// to instantiate the GI
  Overture::setGraphicsInterface(this);
}

void GenericGraphicsInterface:: 
constructor(int & argc, char *argv[])
{
  readFile=NULL;
  saveFile=NULL;
  echoFile=NULL;
  echoFileName="";
  
  getInteractiveResponse=false;  // if true, do not get next command from a command file

  savePick=true;
  useParser=true;
  parser = new OvertureParser(argc,argv);
  
  infoLevel=2;  // level of output:  0=expert, 1=intermediate, 2=novice
  echoToTerminal=1;  // echo commands to the terminal (e.g.. when reading a command file)

  abortProgramIfCommandFileEnds=false; // if true, abort if we stop reading command files.
  
  defaultPrompt="";   // This string is output when no other prompt is supplied
  indentBlanks="";     // the blank string to use for indenting the command file output
  maxNumberOfDefaultPrompts=10;
  defaultPromptStack = new aString [maxNumberOfDefaultPrompts];
  topOfDefaultPromptStack=0;
  
  singleProcessorGraphicsMode=false;
  processorForGraphics=0;   // use this processor for graphics
  graphicsPlottingIsOn=true;        // set to false to skip graphics (for batch runs for e.g.)
  
  saveFileCount=0;          // for counting lines written to the log file 

  graphicsWindowIsOpen=false;       // true if the graphics window is open on this processor. 
  interactiveGraphicsIsOn=false;    // true if the graphics window is open
  
  ignorePause=false;
  
  numberRecorded=0;            // for recording display lists
  recordDisplayLists=NULL;

  preferDirectRendering=true;
  hardCopyRenderingType=offScreenRender;
  
  simplifyPlotWhenRotating=false; // *wdh* 100402 // true;  // if true then draw a wire frame when rotating (if implemented)  

  gridCoarseningFactor=1; // coarsen contour/grid plots by this factor (usually for very fine grids) (if implemented) 
  
}

GenericGraphicsInterface::
~GenericGraphicsInterface()
{
  if( saveFile )
    fclose(saveFile);
  if( readFile )
    fclose(readFile);

  delete parser;

  while( !readFileStack.empty() )
  {
    fclose(readFileStack.top());
    readFileStack.pop();
  }
  
  if( echoFile )
    fclose(echoFile);

  delete [] defaultPromptStack;

// the following call is redundant when Overture::finish() is called
// to destroy the GI
  Overture::setGraphicsInterface(NULL);
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{isGraphicsWindowOpen}} 
bool GenericGraphicsInterface::
isGraphicsWindowOpen()
//----------------------------------------------------------------------
// /Description:
//    Return true if the GUI is in use (on this processor), otherwise false.
// In parallel this will only be true on the processor doing graphics. 
// /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{ 
  return graphicsWindowIsOpen;
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{isInteractiveGraphicsOn}} 
bool GenericGraphicsInterface::
isInteractiveGraphicsOn()
//----------------------------------------------------------------------
// /Description:
//    Return true if the GUI is in use, otherwise false. In parallel, this will
// return true if the graphics window is open on any processor. 
// /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{ 
  return interactiveGraphicsIsOn;
}

//\begin{>GenericGraphicsInterfaceInclude.tex}{\subsection{graphicsIsOn}} 
bool GenericGraphicsInterface::
graphicsIsOn()
//----------------------------------------------------------------------
// /Description:
//   Return true if graphics plotting is turned on. We may turn off plotting in
// batch mode or parallel to avoid some computations (in parallel we avoid
// building a copy of the grid on one processor)
// 
// /return value: true or false.
// /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  return graphicsPlottingIsOn;
}

//\begin{>GenericGraphicsInterfaceInclude.tex}{\subsection{turnOnGraphics}} 
void GenericGraphicsInterface::
turnOnGraphics()
//----------------------------------------------------------------------
// /Description:
//   Turn on graphics plotting (grid, contour, streamline... plots). We may turn off plotting in
// batch mode or parallel to avoid some computations (in parallel we avoid
// building a copy of the grid on one processor)
// 
// /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  graphicsPlottingIsOn=true;
}


//\begin{>GenericGraphicsInterfaceInclude.tex}{\subsection{turnOffGraphics}} 
void GenericGraphicsInterface:: 
turnOffGraphics()
//----------------------------------------------------------------------
// /Description:
//   Turn off graphics plotting (grid, contour, streamline... plots). We may turn off plotting in
// batch mode or parallel to avoid some computations (in parallel we avoid
// building a copy of the grid on one processor)
//
// /return value: true or false.
// /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  graphicsPlottingIsOn=false;
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{getValues (IntegerArray)}} 
int GenericGraphicsInterface::
getValues(const aString & prompt, 
	  IntegerArray & values,
	  const int minimumValue /* =INT_MIN */, 
	  const int maximumValue /* =INT_MAX */,
	  const int sort /* = 0 */ )
//----------------------------------------------------------------------
// /Description:
//    Read in a set of integer values.
// /prompt (input) : use this prompt
// /values (output) : return values in this array, dimensioned to the number of values.
// /minimumValue (input) : specify an optional minimum value. All returned values will be at least this value.
// /maximumValue (input) : specify an optional maximum value. All returned values will be at no greater than
//        this value.
// /sort (input) : optional indicator. If $sort>0$  sort the values to be in increasing order, if $sort<0$,
//    sort the values to be in decreasing order. If $sort==0$, no sorting is done.
// /return value:  Number of values read.
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  appendToTheDefaultPrompt(""); // set the default prompt
  aString menu[]={ "done", "" };
  int numberRead=0;

  GUIState interface;
  interface.buildPopup(menu);
  pushGUI(interface);

  const int maxNumPerLine=30;
  IntegerArray v(maxNumPerLine);
  aString answer;
  for( ;; )
  {
//    getMenuItem(menu,answer,prompt);
    getAnswer(answer, prompt);
    if( answer=="done" || answer=="" )
      break;
    
    v=INT_MIN;
    int num=sScanF(answer,"%i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i",
		       &v( 0),&v( 1),&v( 2),&v( 3),&v( 4),&v( 5),&v( 6),&v( 7),&v( 8),&v( 9),
                       &v(10),&v(11),&v(12),&v(13),&v(14),&v(15),&v(16),&v(17),&v(18),&v(19),
                       &v(20),&v(21),&v(22),&v(23),&v(24),&v(25),&v(26),&v(27),&v(28),&v(29));

    if( num>0 )
    {
      for( int i=0; i<num; i++ )
      {
	if( v(i)<minimumValue )
	{
	  printf("Entry %i with value %i is less than %i. Setting equal to %i\n",i,v(i),minimumValue,minimumValue);
          v(i)=minimumValue;
	}
        else if(  v(i)>maximumValue )
	{
	  printf("Entry %i with value %i is greater than %i. Setting equal to %i\n",i,v(i),maximumValue,maximumValue);
          v(i)=maximumValue;
	}
      }
      if( num>maxNumPerLine )
	printf("GenericGraphicsInterface::getIntegerValues:WARNING: some numbers may be lost. \n"
               "I can only read at most %i values per line\n",maxNumPerLine);
      values.resize(numberRead+num);
#ifndef NO_APP
      values(Range(numberRead,numberRead+num-1))=v(Range(0,num-1));
#else
      for (int q=0; q<num; q++)
	values(q+numberRead) = v(q);
#endif
      numberRead+=num;
    }
    else
      break;
  }
  if( numberRead==0 )
#ifndef NO_APP
        values.redim(0);
#else
  values.resize(0);
#endif
  else if( sort>0 )
  {
    // the values should be in ascending order -- do a bubble sort
    for( int i=0; i<numberRead; i++ )
    {
      int valueJ=INT_MIN;  // also keep track if any changes were made.
      for( int j=0; j<numberRead-1; j++ )
      {
	if( values(j)>values(j+1) )
	{
	  valueJ= values(j);
	  values(j)=values(j+1);
	  values(j+1)=valueJ;
	}
      }
      if( valueJ==INT_MIN )
	break;
    }
  }
  else if( sort<0 )
  {
    // the values should be in descending order -- do a bubble sort
    for( int i=0; i<numberRead; i++ )
    {
      int valueJ=INT_MIN;  // also keep track if any changes were made.
      for( int j=0; j<numberRead-1; j++ )
      {
	if( values(j)<values(j+1) )
	{
	  valueJ= values(j);
	  values(j)=values(j+1);
	  values(j+1)=valueJ;
	}
      }
      if( valueJ==INT_MIN )
	break;
    }
  }
  popGUI();
  
  unAppendTheDefaultPrompt();  // reset

  return numberRead;
}


//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{getValues (RealArray)}} 
int GenericGraphicsInterface::
getValues(const aString & prompt, 
	  RealArray & values,
	  const real minimumValue /* =-REAL_MAX */, 
	  const real maximumValue /* =REAL_MAX */,
	  const int sort /* = 0 */ )
//----------------------------------------------------------------------
// /Description:
//    Read in a set of real values.
// /prompt (input) : use this prompt
// /values (output) : return values in this array, dimensioned to the number of values.
// /minimumValue (input) : specify an optional minimum value. All returned values will be at least this value.
// /maximumValue (input) : specify an optional maximum value. All returned values will be at no greater than
//        this value.
// /sort (input) : optional indicator. If $sort>0$  sort the values to be in increasing order, if $sort<0$,
//    sort the values to be in decreasing order. If $sort==0$, no sorting is done.
// /return value:  Number of values read.
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  appendToTheDefaultPrompt(""); // set the default prompt
  aString menu[]={ "done", "" };
  int numberRead=0;

  GUIState interface;
  interface.buildPopup(menu);
  pushGUI(interface);

  const int maxNumPerLine=30;
  RealArray v(maxNumPerLine);
  aString answer;
  for( ;; )
  {
//    getMenuItem(menu,answer,prompt);
    getAnswer(answer, prompt);
    if( answer=="done" || answer=="" )
      break;
    
    v=REAL_MIN;
    int num=sScanF(answer,"%e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e",
		       &v( 0),&v( 1),&v( 2),&v( 3),&v( 4),&v( 5),&v( 6),&v( 7),&v( 8),&v( 9),
                       &v(10),&v(11),&v(12),&v(13),&v(14),&v(15),&v(16),&v(17),&v(18),&v(19),
                       &v(20),&v(21),&v(22),&v(23),&v(24),&v(25),&v(26),&v(27),&v(28),&v(29));

    if( num>0 )
    {
      for( int i=0; i<num; i++ )
      {
	if( v(i)<minimumValue )
	{
	  printf("Entry %i with value %e is less than %e. Setting equal to %e\n",i,v(i),minimumValue,minimumValue);
          v(i)=minimumValue;
	}
        else if(  v(i)>maximumValue )
	{
	  printf("Entry %i with value %e is greater than %e. Setting equal to %e\n",i,v(i),maximumValue,maximumValue);
          v(i)=maximumValue;
	}
      }
      if( num>maxNumPerLine )
	printf("GenericGraphicsInterface::getIntegerValues:WARNING: some numbers may be lost. \n"
               "I can only read at most %i values per line\n",maxNumPerLine);
      values.resize(numberRead+num);
#ifndef NO_APP
      values(Range(numberRead,numberRead+num-1))=v(Range(0,num-1));
#else
      for (int q=0; q<num; q++)
	values(q+numberRead) = v(q);
#endif
      numberRead+=num;
    }
    else
      break;
  }
  if( numberRead==0 )
#ifndef NO_APP
    values.redim(0);
#else
  values.resize(0);
#endif

  else if( sort>0 )
  {
    // the values should be in ascending order -- do a bubble sort
    for( int i=0; i<numberRead; i++ )
    {
      real valueJ=REAL_MIN;  // also keep track if any changes were made.
      for( int j=0; j<numberRead-1; j++ )
      {
	if( values(j)>values(j+1) )
	{
	  valueJ= values(j);
	  values(j)=values(j+1);
	  values(j+1)=valueJ;
	}
      }
      if( valueJ==REAL_MIN )
	break;
    }
  }
  else if( sort<0 )
  {
    // the values should be in descending order -- do a bubble sort
    for( int i=0; i<numberRead; i++ )
    {
      real valueJ=REAL_MIN;  // also keep track if any changes were made.
      for( int j=0; j<numberRead-1; j++ )
      {
	if( values(j)<values(j+1) )
	{
	  valueJ= values(j);
	  values(j)=values(j+1);
	  values(j+1)=valueJ;
	}
      }
      if( valueJ==REAL_MIN )
	break;
    }
  }
  popGUI();
  
  unAppendTheDefaultPrompt();  // reset
  return numberRead;
}



//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{getDefaultPrompt}} 
const aString &  GenericGraphicsInterface::
getDefaultPrompt()
//----------------------------------------------------------------------
// /Description:
//    Return the current defaultPrompt
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  return defaultPrompt;
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{setDefaultPrompt}} 
int GenericGraphicsInterface::
setDefaultPrompt(const aString & prompt)
//----------------------------------------------------------------------
// /Description:
// Set the deafult prompt and clear the stack of default prompts
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  defaultPrompt=prompt;
  topOfDefaultPromptStack=0;
  return 0;
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{pushDefaultPrompt}} 
int GenericGraphicsInterface::
pushDefaultPrompt(const aString & prompt )
//----------------------------------------------------------------------
// /Description:
// Push a default prompt onto a stack and make it the current prompt
// /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  if( topOfDefaultPromptStack >= maxNumberOfDefaultPrompts )
  {
    // increase the stack size if it is too small
    aString *stack = new aString [maxNumberOfDefaultPrompts+10];
    for( int i=0; i< maxNumberOfDefaultPrompts; i++ )
      stack[i]=defaultPromptStack[i];
    delete [] defaultPromptStack;
    defaultPromptStack=stack;
    maxNumberOfDefaultPrompts=maxNumberOfDefaultPrompts+10;
    printf("GenericGraphicsInterface::INFO: default prompt stack size increased to %i\n",maxNumberOfDefaultPrompts);
  }
  defaultPromptStack[topOfDefaultPromptStack++]=defaultPrompt;
  defaultPrompt=prompt;
  return 0;
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{popDefaultPrompt}} 
int GenericGraphicsInterface::
popDefaultPrompt()
//----------------------------------------------------------------------
// /Description:
// pop a default prompt off the stack and make the next prompt the new default
// /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  if(topOfDefaultPromptStack>0 )
  {
    topOfDefaultPromptStack--;
    defaultPrompt=defaultPromptStack[topOfDefaultPromptStack];
  }
  else
  {
    printf("GenericGraphicsInterface::popDefaultPrompt:WARNING: the default prompt stack is empty\n");
  }
  return 0;
}

//! Set the info level which determines the level of information that is output. 
//! This is a bit flag with large values corresponding to more info.
//! 0=expert, 1=intermediate, 2=novice
int GenericGraphicsInterface::
getInfoLevel() const
{ 
  return infoLevel;
}

//! Get the infoLevel.
//! This is a bit flag with large values corresponding to more info.
//! 0=expert, 1=intermediate, 2=novice
void GenericGraphicsInterface:: 
setInfoLevel(int value)
{
   infoLevel=value; 
}  

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{appendToTheDefaultPrompt}} 
int GenericGraphicsInterface::
appendToTheDefaultPrompt(const aString & appendage )
//----------------------------------------------------------------------
// /Description:
//    Append a aString to the defaultPrompt and push this new prompt onto the stack.
//  Also increase the amount of indentation used when writing to command files.
// /appendage (input): append this string to the default prompt.
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  if( defaultPrompt!="" )
    indentBlanks=indentBlanks+"  "; // add 2 spaces for indentation
  pushDefaultPrompt(defaultPrompt+appendage);
  // printf("appendTheDefaultPrompt:blanks=[%s]\n",(const char*)indentBlanks);
  return 0;
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{unAppendTheDefaultPrompt}} 
int GenericGraphicsInterface::
unAppendTheDefaultPrompt()
//----------------------------------------------------------------------
// /Description:
//   Remove the last string appended to the default prompt by popping the stack.
//  Also decrease the amount of indentation used when writing to command files.
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  if( indentBlanks.length()>3 )
#ifndef NO_APP
    indentBlanks=indentBlanks(0,indentBlanks.length()-3);
#else
    indentBlanks=indentBlanks.substr(0,indentBlanks.length()-2);
#endif
  else 
    indentBlanks = "";
  
  // printf("unAppendTheDefaultPrompt:blanks=[%s]\n",(const char*)indentBlanks);
  popDefaultPrompt();
  return 0;
}


//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{outputString (base class)}} 
void GenericGraphicsInterface::
outputString(const aString & message, int messageLevel /* =2 */ )
//----------------------------------------------------------------------
// /Description:
//   Output a string to standard output.
//   If the echo file is open, also output the string in that file.
// /message (input): the string to be output.
// /messageLevel (input) : output the string if messageLevel is less than or equal
//    to the current value for infoLevel. Values for infoLevel are 0=expert, 1=intermediate, 2=novice.
// /Return Values: none.
//
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  if( messageLevel<=infoLevel )
  {
    if( echoFile )
      fPrintF(echoFile,"%s\n",(const char*)message.c_str());

    printF("%s\n",(const char*)message.c_str());
  }
  
}

  
//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{readCommandFile}} 
FILE* GenericGraphicsInterface::
readCommandFile(const aString & commandFileName /* =nullString */ )
//----------------------------------------------------------------------
//  /Description:
//    Start reading a command file. 
//  /commandFileName (input):
//    If {\ff commandFileName} is specified
//    then this should be the name of the command file to read. This routine
//    will automatically add a ".cmd" to the file name if the
//    file named {\tt commandFileName} is not found.
//    If  {\ff commandFileName} is not given then
//    you will be prompted to enter the name of the file.
//  /Errors:  Unable to open the file.
//  /Return Values: Pointer to the opened file or NULL if able to open the file.
//
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------

{
  aString msg;
  if( readFile )
  {
    readFileStack.push(readFile);  // save the current command file we are reading on a stack
    printf("Pushing the current command file onto a stack\n");
    // outputString("readCommandFile:ERROR: a command file is already open for reading");
    // outputString("recursive reading of command files is not supported");
    // fclose(readFile);   // close any open file
  }
  
  if( commandFileName == nullString )
  {
    // cout << "Enter the command file to read\n";
    // cin >> readFileName;
    // aString menu[] = { "" }; 
    inputFileName(readFileName, "Enter the command file to read", ".cmd");
    outputString(sPrintF(msg, "reading file [%s]", (const char*) readFileName.c_str()));;
  }
  else
    readFileName=commandFileName;
  
  if( readFileName=="stdin" || readFileName=="STDIN" )
  {
    printf("Open stdin for reading as a command file\n");
    readFile=stdin;
  }
  else
  {
    readFile = fopen(readFileName.c_str(),"r" );           // for fprintf
    if( !readFile )
    {
      readFile=fopen((readFileName+".cmd").c_str(),"r" );
      if( !readFile )
	cout << "ERROR:unable to open file [" << readFileName << "] or [" << readFileName+".cmd" << "]\n";
    }
  }
  return readFile;
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{getReadCommandFile}}
FILE* GenericGraphicsInterface::
getReadCommandFile() const
// /Description:
//   Return a file pointer to the current command file we are reading
// 
//\end{GenericGraphicsInterfaceInclude.tex} 
{
  return readFile;
}


//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{getSaveCommandFile}}
FILE* GenericGraphicsInterface::
getSaveCommandFile() const
// /Description:
//   Return a file pointer to the current command file we are saving
// 
//\end{GenericGraphicsInterfaceInclude.tex} 
{
  return saveFile;
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{readCommandsFromStrings}} 
int GenericGraphicsInterface::
readCommandsFromStrings(const aString *commands)
//----------------------------------------------------------------------------------
//  /Description:
//    Start reading commands from an array of Strings, commands terminated by the aString=""
//  /commands (input): A list of Strings (commands). There must be a null string, "",
//       to indicate the end of the list.
//  /Errors: unexpected results will occur if there is no null string to terminate the array.
//  /Return Values: 0
//
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  const int maxStringCommands=100000;  // sanity check
  int i;
  for( i=0; commands[i]!="" && i<maxStringCommands; i++ )
  {
    stringCommands.push(commands[i]);
  }
  if( i>=maxStringCommands )
  {
    printF("GenericGraphicsInterface::readCommandsFromStrings:ERROR: There were more than %i commands!\n"
           "   I assume that this must be an error\n",maxStringCommands);
    OV_ABORT("error");
  }
  return 0;
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{readingFromCommandFile}}
bool GenericGraphicsInterface::
readingFromCommandFile() const 
// -------------------------------------------------------------------------------------
// /Description:
//  Return true of we are reading from a command file.
// 
//\end{GenericGraphicsInterfaceInclude.tex} 
// -------------------------------------------------------------------------------------
{ 
  return readFile!=NULL; 
} 



  
//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{saveCommandFile}} 
FILE* GenericGraphicsInterface::
saveCommandFile(const aString & commandFileName /* =nullString */ )
//----------------------------------------------------------------------------------
//  /Description:
//    Start saving a command file. 
//  /commandFileName (input):
//    If {\ff commandFileName} is specified
//    then this should be the name of the command file to save commands in. It will
//    first be opened. If  {\ff commandFileName} is not given then
//    you will be prompted to enter the name of the file.
//  /Errors: Unable to open the file.
//  /Return Values: Pointer to the opened file or NULL if able to open the file.
//
//  /Author: WDH \& AP
//\end{GenericGraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  if( saveFile )
    fclose(saveFile);   // close any open file

  if( commandFileName == nullString )
  {
    inputFileName(saveFileName,"Enter the command file to save");
  }
  else
    saveFileName=commandFileName;

  #ifndef NO_APP
    // only open the command file if we are on processor zero 
    const bool openFile = Communication_Manager::localProcessNumber()==getProcessorForGraphics();
  #else
    const bool openFile=true;
  #endif     

  if( openFile )
  { 
    saveFile = fopen(saveFileName.c_str(),"w" );   
    if( !saveFile )
    {
      printF("ERROR:unable to open file %s\n",(const char*)saveFileName);
    }
  }
  
  return saveFile;
}


//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{abortIfCommandFileEnds}} 
void GenericGraphicsInterface::
abortIfCommandFileEnds(bool trueOrFalse /* =true */)
// /Description:
//  Specify whether to abort the program if we stop reading a command file.
//  This option is used by automated scripts to prevent a program hanging while
//  waiting for input.
//  /trueOrFalse (input): the explanation is in the name itself!
//  /Return Values: None.
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  abortProgramIfCommandFileEnds=trueOrFalse;
}



//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{outputToCommandFile}}
void GenericGraphicsInterface::
outputToCommandFile( const aString & line )
//----------------------------------------------------------------------------------
//  /Description:
//     Output a line to the command file if there is one open.
//  /line (input) : save this string, NOTE: you should include a newline character if you want one.
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  if( saveFile )
  {
    fPrintF(saveFile,"%s%s",(const char*)indentBlanks.c_str(),(const char*)line.c_str());
    fflush(saveFile);
  }
  
  int last = line.length()-1;
#ifndef NO_APP
  if (line(last,last) == '\n')
#else
  if (line[last] == '\n')
#endif
    last--;

#ifndef NO_APP
  appendCommandHistory(line(0,last));
#else
  appendCommandHistory(line.substr(0,last+1));
#endif
}



//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{saveEchoFile}}
FILE* GenericGraphicsInterface::
saveEchoFile(const aString & fileName /*=nullString*/)
//-------------------------------------------------------
// /Description:
// Start saving an echo file (if no file name is given, prompt for one)
//  /Author: AP
//\end{GenericGraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------

{
  if( echoFile )
    fclose(echoFile);   // close any open file

  if( fileName == nullString )
  {
    inputFileName(echoFileName,"Enter the echo file to save");
  }
  else
    echoFileName = fileName;

  echoFile = fopen(echoFileName.c_str(),"w" );           // for fprintf
  if( !echoFile )
  {
    cout << "ERROR:unable to open file " << echoFileName << endl;
  }
  return echoFile;
}

  
//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{stopSavingEchoFile}}
void GenericGraphicsInterface::
stopSavingEchoFile()
//-------------------------------------------------------
// /Description:
// Stop saving the echo file (and close the file)
//  /Author: AP
//\end{GenericGraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  if( echoFile )
    fclose(echoFile);
  echoFile = NULL;
  echoFileName = "";
}


//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{savePickCommands}} 
int GenericGraphicsInterface::
savePickCommands(bool trueOrFalse /* =TRUE */ )
//----------------------------------------------------------------------
// /Description:
// Set wether picking commands should be logged in the command file in their raw form.
// /trueOrFalse(input): the description is in the name!
// /Author: AP
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  savePick=trueOrFalse;
  return 0;
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{setIgnorePause}} 
void GenericGraphicsInterface::
setIgnorePause( bool trueOrFalse /* =true */ )
//----------------------------------------------------------------------
// /Description:
// If true, ignore the "pause" statement in command files.
// /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  ignorePause=trueOrFalse;
}


//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{stopReadingCommandFile}} 
void GenericGraphicsInterface::
stopReadingCommandFile()
// ---------------------------------------------------------------------------------------------
// /Description:
// Stop reading the command file (and close the file)
//
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  if( readFile )
  {
    fclose(readFile);
    while( !readFileStack.empty() )
    {
      fclose(readFileStack.top());
      readFileStack.pop();
    }    
  }
  
  readFile=NULL;

  if( abortProgramIfCommandFileEnds )
  {
    printf("The command file has ended and abortProgramIfCommandFileEnds==true.\n"
           "The program will now abort on purpose.\n");
    OV_ABORT("abort");
  }
  
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{stopSavingCommandFile}} 
void GenericGraphicsInterface::
stopSavingCommandFile()
// ---------------------------------------------------------------------------------------------
// /Description:
// Stop saving the command file (and close the file)
//
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//-----------------------------------------------------------------------------------
{
  if( saveFile )
    fclose(saveFile);
}


//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{buildCascadingMenu}} 
int GenericGraphicsInterface::
buildCascadingMenu( aString *&menu,
		    int startCascade, 
		    int endCascade ) const
// ===================================================================================================
// /Description:
//    Take a menu that (might) have a long list of items and cascade these items so that
// they will appear nicely on the screen.
//
// /menu (input/output) : On input an array of strings termined with a "" (null) string. On output
//    a new cascading menu.
// /startCascade,endCascade (input) : these specify the set of menu items to
//    cascade. 
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
// ===================================================================================================
{
  // cascade solution menu if there are more than this many solutions 
  // We allow for 3 levels of cascading.
  const int maxMenuSolutions=25;   // **** this must be the same in indexInCascadingMenu below ****
  const int maxMenuSolutionsSquared=maxMenuSolutions*maxMenuSolutions;
  const int maxMenuSolutionsCubed=maxMenuSolutions*maxMenuSolutions*maxMenuSolutions;
  // stride through the solutions if there are more than this many solutions:
  const int maximumNumberOfSolutionsInTheMenu=maxMenuSolutionsCubed*maxMenuSolutions;

  // count the number of entries:
  const int tooManyMenuItems=100000;
  int i=0;
  while( menu[i]!="" && i<tooManyMenuItems )
    i++;
  
  if( i>= tooManyMenuItems )
  {
    printf("GenericGraphicsInterface::buildCascadingMenu:ERROR: There are more than %i entries in a menu. "
           "Something is wrong here\n",tooManyMenuItems);
    throw "error";
  }
  
  int numberOfMenuEntries=i+1;  // include null terminating string
  const int numberOfSolutions=endCascade-startCascade+1;

  // than this many solutions.
  int solutionIncrement=1;                          // Here is the stride.
  solutionIncrement=1;
  if( numberOfSolutions>maximumNumberOfSolutionsInTheMenu )
    solutionIncrement=(numberOfSolutions+maximumNumberOfSolutionsInTheMenu-1)/maximumNumberOfSolutionsInTheMenu;
	  
  const int maximumNumberOfEntriesInMenu=numberOfMenuEntries+
    numberOfSolutions/maxMenuSolutions+
    2*numberOfSolutions/maxMenuSolutionsSquared+     // factor of 2 counts extra menu plus extra line for "< "
    2*numberOfSolutions/maxMenuSolutionsCubed+       // factor of 2 counts extra menu plus extra line for "< "
    10;
  aString *menu2 = new aString [maximumNumberOfEntriesInMenu];
  i=0;
  int j;
  for( j=0; j<startCascade; j++ )
    menu2[i++]=menu[j];

  char buff[80];

  int j2=0,j3=0,j4=0;
  for( j=0; j<numberOfSolutions; j+=solutionIncrement )
  {
    if( numberOfSolutions>maxMenuSolutionsCubed && ( j4 % maxMenuSolutionsCubed==0) )
    {
      j3=0; // start third level of cascade
      if( j4==0 )
	menu2[i++]=sPrintF(buff,">items %i to %i",j,j+maxMenuSolutionsCubed*solutionIncrement-1);
      else
      {
	menu2[i++]="< "; // move from level 3 cascade to level 2
	menu2[i++]="< "; // move from level 2 cascade to level 1
	menu2[i++]=sPrintF(buff,"<>items %i to %i",j,
			   min(j+maxMenuSolutionsCubed*solutionIncrement-1,numberOfSolutions-1));
      }
    }
    if( numberOfSolutions>maxMenuSolutionsSquared && ( j3 % maxMenuSolutionsSquared==0) )
    {
      j2=0; // start second level of cascade
      if( j3==0 )
	menu2[i++]=sPrintF(buff,">items %i to %i",j,j+maxMenuSolutionsSquared*solutionIncrement-1);
      else
      {
	menu2[i++]="< "; // move from 2nd level cascade to 1st level
	menu2[i++]=sPrintF(buff,"<>items %i to %i",j,
			   min(j+maxMenuSolutionsSquared*solutionIncrement-1,numberOfSolutions-1));
      }
    }
    if( numberOfSolutions>maxMenuSolutions && ( j2 % maxMenuSolutions==0) )
    {
      if( j2==0 )
	menu2[i++]=sPrintF(buff,">items %i to %i",j,j+maxMenuSolutions*solutionIncrement-1);
      else
	menu2[i++]=sPrintF(buff,"<>items %i to %i",j,
			   min(j+maxMenuSolutions*solutionIncrement-1,numberOfSolutions-1));
    }
    assert( i<maximumNumberOfEntriesInMenu );
    assert( menu[j+startCascade]!="" );
    
    menu2[i++]=menu[j+startCascade];
    j2++;
    j3++;
    j4++;
  }
  assert( i<maximumNumberOfEntriesInMenu );

  if( numberOfSolutions>maxMenuSolutionsCubed )
    menu2[i++]="< "; 
  if( numberOfSolutions>maxMenuSolutionsSquared )
    menu2[i++]="< "; 
  if( numberOfSolutions>maxMenuSolutions )
    menu2[i++]="< ";
  else
    menu2[i++]=" ";


  for( j=endCascade+1; j<numberOfMenuEntries; j++ )
  {
    assert( i<maximumNumberOfEntriesInMenu );
    menu2[i++]=menu[j];
  }
/* --
  printf(" actual number=%i, maximumNumberOfEntriesInMenu=%i \n",i,maximumNumberOfEntriesInMenu);

  for( i=0; i<maximumNumberOfEntriesInMenu; i++ )
    printf("menu[%i]=%s \n",i,(const char*)menu2[i]);
--- */
  
  delete [] menu;
  menu=menu2;
  
  return 0;
  
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{indexInCascadingMenu}} 
int GenericGraphicsInterface::
indexInCascadingMenu( int & index,
		      const int startCascade,
		      const int endCascade ) const
// ===================================================================================================
// /Description:
//    Used in conjuction with buildCascadingMenu to convert an index into the cascading menu into
//   an index into the original menu.
// /index (input/ouput) : on input this is a index into the cascading menu, built by buildCascadingMenu
//   On output this is an index into the original menu.
// /return value: is the same as index.
//
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
// ===================================================================================================
{
  // cascade solution menu if there are more than this many solutions 
  const int maxMenuSolutions=25; // 10; // 25;  
  const int maxMenuSolutionsSquared=maxMenuSolutions*maxMenuSolutions;
  const int maxMenuSolutionsCubed=maxMenuSolutions*maxMenuSolutions*maxMenuSolutions;

  // stride through the solutions if there are more than this many solutions:
  // const int maximumNumberOfSolutionsInTheMenu=400; 
  const int solutionIncrement=1;

  const int numberOfSolutions=endCascade-startCascade+1;

  index=index-startCascade;
  // printf("index-startCascade=%i \n",index);

  // Here we repeat the loop from buildCascadingMenu, decrement index for each additional
  // menu entry that is added.
  int j,j2=0,j3=0,j4=0;
  for( j=0; j<index; j+=solutionIncrement )
  {
    if( numberOfSolutions>maxMenuSolutionsCubed && ( j4 % maxMenuSolutionsCubed==0) )
    {
      j3=0; // start third level of cascade
      if( j4==0 )
	index--;
      else
	index-=3;
    }
    if( numberOfSolutions>maxMenuSolutionsSquared && ( j3 % maxMenuSolutionsSquared==0) )
    {
      j2=0; // start second level of cascade
      if( j3==0 )
	index--;
      else
	index-=2;
    }
    if( numberOfSolutions>maxMenuSolutions && ( j2 % maxMenuSolutions==0) )
      index--;

    j2++;
    j3++;
    j4++;
  }

  return index;
}


