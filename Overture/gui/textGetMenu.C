#include "GenericGraphicsInterface.h"
#include <string.h>
#ifndef NO_APP
#include "broadCast.h"
#endif

#ifdef NO_APP
#define redim resize
#define getBound(x) size((x))-1
#define getLength size

using GUITypes::real;
using std::cout;
using std::endl;
#endif

#include "OvertureParser.h"


int 
getLineFromFile( FILE *file, char s[], int lim);

int GenericGraphicsInterface::
parseAnswer(aString & answer )
// =============================================================================================
//  /Description:
//    Parse an answer. If the answer contains newline characters, the answer is split
// into multiple commands which are pushed onto the stringCommands queue
// 
// /Return value: 0: answer was not changed or was evaluated replacing any perl variables with their values.
//                1: answer contained a semi-colon and was processed by perl, answer remains unchanged. 
//                2: answer contained a newline indicating multiple commands
// =============================================================================================
{
  int returnValue=parser->parse(answer);
  if( returnValue==2 )
  {
    // answer contains a new line -- we split the answer into multiple lines and push the
    // results onto the stringCommands queue so that each command can be processed
    int ia=0;
    int len=answer.length();
    int i;
    for( i=1; i<len; i++ )
    {
      if( answer[i]=='\n' )
      {
	//kkc 040415 hope this is right	stringCommands.push(answer(ia,i-1));
	stringCommands.push(answer.substr(ia,i-ia));
	//	printf("GGI::parseAnswer::push sub command=[%s]\n",(const char*)answer(ia,i-1));
	// printf("GGI::parseAnswer::push sub command=[%s]\n",(const char*)answer.substr(ia,i-ia).c_str());
	ia=i+1;
      }
    }
    i=len;
    if( ia<i )
    {
      //kkc 040415 hope this is righstringCommands.push(answer(ia,i-1));
      stringCommands.push(answer.substr(ia,i-ia));
      //      printf("GGI::parseAnswer::push sub command=[%s]\n",(const char*)answer(ia,i-1));
      // printf("GGI::parseAnswer::push sub command=[%s]\n",(const char*)answer.substr(ia,i-ia).c_str());
    }
  }


  if( returnValue==0 )
  {
    // *wdh* 080220: check for comment lines in parsed answer
    const char * canswer=answer.c_str();
    if( strchr(canswer,'*')!=NULL || strchr(canswer,'#')!=NULL ) // hash added by kkc 0808122
    {
      int len=answer.length();
      int i=0;
      while( i<len && (answer[i]==' ' || answer[i]=='\t' ) ){ i++; }
    
      if( i<len && (answer[i]=='*' || answer[i]=='#') )
      {
        // parser returned a comment -- skip this 
        // printF(" GenericGraphicsInterface::parseAnswer: comment line found: answer=[%s]\n",(const char*)answer);
        returnValue=1;  // pretend that this was a perl line and should be skipped
      }
    }
  }
  
  return returnValue;
}

int GenericGraphicsInterface::
promptAnswerSelectPick(aString & answer, 
		       const aString & prompt, 
		       SelectionInfo * selection_)
{
  int returnValue=0;
  bool done=FALSE;
  // int menuSelected=-1;
  // bool specialMenuItem=FALSE;
  
// initialize
//    if (pick_)
//      pick_->active = 0;

  if (selection_)
    selection_->nSelect = 0;
  
#ifndef NO_APP
  if( Communication_Manager::localProcessNumber()==processorForGraphics ) // P++ stuff
#endif
  {
    while( !done )
    {
      // We can read an answer either from a command file or the stringCommands queue
      // ***NOTE: the nearly same section of code appears in getAnswer.C:getAnswerSelectPick
      const bool readStringCommands = !stringCommands.empty();
      bool readAnswer = !getInteractiveResponse && (readFile || readStringCommands);
      if( readAnswer )
      {
	returnValue = fileAnswer(answer, prompt, selection_); 
   
        // if the file ends inside fileAnswer, readFile will be closed and readFile will be set to NULL       
        readAnswer = readFile || readStringCommands;  // recompute this, the command file may have ended
      }
      if( !readAnswer )
      { 
	returnValue=promptAnswer(answer, prompt);
      }
      // optionally parse the command (read another answer if the parser returns a nonzero)
      if( useParser && parseAnswer(answer)!=0 )
	continue;
    
      if( returnValue < 0 ) // 
      {
        // note that processSpecialMenuItems is a virtual function.
	done = !processSpecialMenuItems(answer);   // this will save in the log file if necessary
	if( done ) // ***** missing short forms for special commands "read command file" ...
	{
	  // answer may be a short form for a menu entry ****
	  // strip off leading blanks 

	  aString line=answer;
          int i;
	  for( i=0; i<line.length() && (line[i]==' ' || line[i]=='\t'); i++) {}
	  if( i>0 )
	    answer=line.substr(i,line.length()-i);

	  returnValue=getMatch(currentGUI->allCommands, answer);   // find the best match, if any
	  done=TRUE;
	}      
      }
      else
	done=TRUE;

      // answer=menu[returnValue];

      // we have already saved in the saveFile if readFile==TRUE
      if( done && saveFile && !readFile && answer!="log commands to file" &&
          ( savePick || answer.substr(0,5)!="mogl-") ) 
      {
	fPrintF(saveFile,"%s\n",(const char*)(indentBlanks+answer).c_str());
        // *wdh* 030414 if( ++saveFileCount > 10 )
        if( true )
	{
	  saveFileCount=0;
          fflush(saveFile);  // flush the log file
	}
      }
      
    }  // end while(!done)
  }
#ifdef USE_PPP
  if( !singleProcessorGraphicsMode )
  {
    broadCast(answer,processorForGraphics);
    broadCast(returnValue,processorForGraphics);
    // We need to let all processors know if graphicsPlottingIsOn has changed
    broadCast(graphicsPlottingIsOn,processorForGraphics);
    broadCast(interactiveGraphicsIsOn,processorForGraphics); // *wdh* 100425 (for "open graphics" in parallel)
  }
#endif
  return returnValue;
}


//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{inputString (base class)}} 
void GenericGraphicsInterface::
inputString(aString &answer, 
            const aString & prompt /* =nullString */ )
//----------------------------------------------------------------------
// /Description:
//   Input a string after displaying an optional prompt
// /answer (output): the string that was read
// /prompt (input): display an optional prompt.
// /Return Values: none.
//
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{ 
  if( !getInteractiveResponse && (readFile || !stringCommands.empty()) )
    readLineFromCommandFile(answer);
  else
  {
    char buff[180];
    if( prompt!=nullString )
      cout << prompt << endl;
    else if( defaultPrompt!="" ) // This string is output when no other prompt is supplied
      cout << defaultPrompt << endl;
    cout.flush();
    getLine(buff,sizeof(buff));
    answer=buff;
    // if( saveFile )
    //  fPrintF(saveFile,"%s\n",(const char*)answer);
  }
}


int GenericGraphicsInterface::
processSpecialMenuItems(aString & answer)
{
  // printf("GenericGraphicsInterface::processSpecialMenuItems: answer=[%s]\n",(const char*)answer);

  int answerFound=TRUE;
  if( answer.length()<1 )
  {
    answerFound=FALSE;
    return answerFound;
  }
  int len=0;
  if( answer=="read command file" )
  {
    readCommandFile(); 
    answerFound=TRUE;
  }
  else if( (len=str_matches(answer,"include ")) )
  { // *new* 030919 
    aString newCommandFileName=answer.substr(len,answer.length()-len);
    readCommandFile(newCommandFileName); 
    answerFound=TRUE;
  }
  else if( answer=="log commands to file" )
  {
    saveCommandFile(); 
    answerFound=TRUE;
  }
  else if( answer=="pause" )
  {
    pause(); 
    answerFound=TRUE;
  }
  else if( answer=="turn off parser" )
  {
    useParser=false;
    answerFound=TRUE;
  }
  else if( answer=="turn on parser" )
  {
    useParser=true;
    answerFound=TRUE;
  }
  else if( answer=="quit" )
  {
    exit(0);
  }
  
  return answerFound;
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{getMatch}} 
int GenericGraphicsInterface::
getMatch(const aString *menu, aString & answer)
// =======================================================================================
// /Description:
//  Find the menu item that "best" matches answer -- answer can be a truncated version
//  of the menu item but it must be a unique match.
// /menu(input): array of strings terminated by empty string.
// /answer(input): string to be matched.
// /Return values: Index in the menu array of the unique matching entry, or -1 if
// no unique entry was found or other error occured.
//  
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
// =======================================================================================
{
  //kkc 10/3/00 got rid of malloc and free, added null pointer checks
  // before delete of canswer and winExt

// strip off everything behind ':' (including the ':')
  char * canswer=NULL;
  char * winExt=NULL;
  
  // we should only strip off the chars after :# where #=[0-9]
  // Otherwise we assume that it must be a : in a user defined command.  *wdh* 010907
  int length = strcspn((const char *)answer.c_str(), ":");
  int aLength = answer.length();
  if( length<aLength )
  {
    char a=answer[length];
    if( a=='0' || a=='1' || a=='2' || a=='3' || a=='4' || a=='5' || a=='6' || a=='7' || a=='8' || a=='9' )
    {
    }
    else
    {
      // dont strip
      length=aLength;
    }
  }
  canswer = (char *) new char[ (length+1) ];
  strncpy(canswer, (const char *)answer.c_str(), length);
  
// get the : window extension
  if (length < aLength)
  {
    winExt = (char *) new char[( (aLength-length+2) )];
    strcpy(winExt, (const char *)answer.substr(length,aLength-length).c_str());
  }
  else
  {
    winExt = NULL;
  }
  
  int menuSelected=-1;
//  int length=answer.length();
  if( length==0 )
    return menuSelected;
  int matchLength=0;
  bool ambiguous=FALSE;
  for( int i=0; menu[i]!=""; i++ )
  {
    if( strncmp((const char*)menu[i].c_str(),(const char*)answer.c_str(),length)==0 ) // answer matches the first chars
    {
      if( length==menu[i].length() )   // complete match, we have found the menu item
      {
	menuSelected=i;
	ambiguous=FALSE;
	break;
      }
      else if( matchLength < length )   // this is a better partial match
      {
	matchLength=length;
	menuSelected=i;
	ambiguous=FALSE;
      }
      else if( matchLength==length )    // another partial match found of same goodness
	ambiguous=TRUE;
    }
  }
  if( menuSelected==-1 )
  {
    // cout << "Error: unknown response \n";
  }
  else
  {
    if( ambiguous )
    {
      //cout << "Warning: ambiguous response, answer=[" << canswer << "], length=" << length << 
      // " choosing menu=[" << menu[menuSelected] << "], length=" << menu[menuSelected].length() <<"\n";

      menuSelected=-1; // *wdh* 000913 return -1 for an ambiguous response
    }
    else
    {
      if (winExt)
	answer=menu[menuSelected]+winExt;
      else
	answer=menu[menuSelected];
    }
  }
  if (canswer!=NULL) delete [] canswer;
  if (winExt!=NULL) delete  [] winExt;
  
  return menuSelected;
}


int GenericGraphicsInterface::
fileAnswer(aString & answer,  
	   const aString & prompt,
	   SelectionInfo *select_)
{
  int numCharsRead = readLineFromCommandFile(answer);   // this will save in a command file if open
  int returnValue=-1; // by default, treat all commands as internal
  aString msg;
  
  if( numCharsRead<1 )
  {
    printF("fileAnswer:INFO: end-of-file reached in command file\n");

    stopReadingCommandFile();
    return 0;  // fall through to the interactive menu
  }
  else
  {
    returnValue=getMatch(currentGUI->allCommands, answer);   // find the best match, if any
  }//                01234567890123456789
  if( answer.substr(0,12)=="mogl-select:" )
  {
// the syntax is 
// mogl-select:0 4
//       721 920350144 920350144  721 920350144 920350144  721 920350144 920350144  
//       721 920350144 920350144  
// up to three selections on each line
    int wNum, nSel, sel[10], nRead, status;
	  
    sScanF(&answer[12],"%i %i", &wNum, &nSel);
    if (!select_)
    {
      outputString("A mogl-select was read from file, but select_ == NULL");
      return 0;
    }
    else
    {
      select_->winNumber = wNum;
      if (nSel>0)
      {
	select_->nSelect = nSel;
	select_->selection.redim(nSel,3);
	nRead=0;
	for( int i=0; i<nSel; i+=3 )
	{
// only the last line of a multi-line command gets echoed below
	  if( isGraphicsWindowOpen() )
	    appendCommandHistory(answer); 

	  numCharsRead = readLineFromCommandFile(answer);
	  if( numCharsRead<1 )
	  {
  	    printF("fileAnswer:INFO: end-of-file reached in command file\n");
            stopReadingCommandFile();
	    return 0;
	  }
	  status=sScanF(answer,"%i %i %i %i %i %i %i %i %i",
			&sel[0], &sel[1], &sel[2], &sel[3], &sel[4], &sel[5], &sel[6], &sel[7], &sel[8]);
                                          
	  if( status==0 || status==EOF )
	    break;
	  if (status >= 3)
	  {
	    select_->selection(i  ,0) = sel[0];
	    select_->selection(i  ,1) = sel[1];
	    select_->selection(i  ,2) = sel[2];
	    nRead++;
	  }
	  if (status >= 6)
	  {
	    select_->selection(i+1,0) = sel[3];
	    select_->selection(i+1,1) = sel[4];
	    select_->selection(i+1,2) = sel[5];
	    nRead++;
	  }
	  if (status == 9)
	  {
	    select_->selection(i+2,0) = sel[6];
	    select_->selection(i+2,1) = sel[7];
	    select_->selection(i+2,2) = sel[8];
	    nRead++;
	  }
	} // end for i...
	if (nSel != nRead)
	{
	  sPrintF(msg, "Warning: Mismatch while reading selections. nSel=%i, nRead=%i", nSel, nRead);
	  outputString(msg);
	  return 0;
	}
// find the index of the closest object
	int iZB=0, ii;
	real zbMin = select_->selection(iZB,1);
	for (ii=1; ii<select_->nSelect; ii++)
	  if (select_->selection(ii,1) < zbMin)
	  {
	    zbMin = select_->selection(ii,1);
	    iZB = ii;
	  }
// save the globalID of the closest object    
	select_->globalID = select_->selection(iZB, 0); // global ID value of the closest object

// only the last line of a multi-line command gets echoed below
	if( isGraphicsWindowOpen() )
	  appendCommandHistory(answer); 
// read another line
	numCharsRead = readLineFromCommandFile(answer);
	if( numCharsRead<1 )
	{
	  printF("fileAnswer:INFO: end-of-file reached in command file\n");
          stopReadingCommandFile();
	  return 0;
	}
//               01234567890123456789
// the syntax is:
// mogl-coordinates 7.871720e-01 7.899720e-01 5.250000e-01 5.260000e-01 6.384776e+00 3.547130e-01 -9.934107e-07
	real r0, s0, r1, s1, zb, x0, y0, z0;
	if ((status=sScanF(&answer[16],"%e %e %e %e %e %e %e %e", &r0, &r1, &s0, &s1, &zb, &x0, &y0, &z0)) == 8)
	{
	  select_->active = 1;
	  select_->r[0]   = r0;
	  select_->r[1]   = r1;
	  select_->r[2]   = s0;
	  select_->r[3]   = s1;
	  select_->zbMin  = zb;
	  select_->x[0]   = x0;
	  select_->x[1]   = y0;
	  select_->x[2]   = z0;
	}
// try the old format, which didn't read s0 or s1!!!
	else if ((status=sScanF(&answer[16],"%e %e %e %e %e %e", &r0, &r1, &zb, &x0, &y0, &z0)) == 6)
	{
	  select_->active = 1;
	  select_->r[0]   = r0;
	  select_->r[1]   = r1;
	  select_->r[2]   = .1; // just making something up for s0
	  select_->r[3]   = .1; // just making something up for s1
	  select_->zbMin  = zb;
	  select_->x[0]   = x0;
	  select_->x[1]   = y0;
	  select_->x[2]   = z0;
	}
	else
	{
	  sPrintF(msg,"Error: could only read %i out of 8 (or 6) numbers describing a mogl-coordinates event", status);
	  outputString(msg);
	}
      } // end if nSel>0
    } // end if select_
    returnValue = 0;
	    
//          menuSelected=GenericGraphicsInterface::select(selection,answer); // returns number selected.
  }//                01234567890123456789
  if( answer.substr(0,16)=="mogl-pickOutside" )
  {
    int wNum, status=0;
    real r0, s0;
// the syntax is mogl-pickOutside:0 6.180758e-01 7.472222e-01
    if (!select_)
      outputString("A mogl-pickOutside was read from file, but select_ == NULL");
    else if ((status=sScanF(&answer[17],"%i %e %e", &wNum, &r0, &s0)) == 3)
    {
      select_->active = 1;
      select_->winNumber = wNum;
      select_->nSelect = 0;
      select_->r[0] = r0;
      select_->r[1] = s0;
      select_->zbMin = 0.0; // z-buffer coordinate is unknown
      select_->globalID = 0;
      select_->selection.redim(0);
    }
    else
    {
      sPrintF(msg,"Error: could only read %i out of 3 numbers describing a mogl-pickOutside event", status);
      outputString(msg);
      select_->active = 0;
      select_->nSelect = 0;
    }

    returnValue = 0;
  }
  
  return returnValue; 
}

//\begin{>>GenericGraphicsInterfaceInclude.tex}{\subsection{readLineFromCommandFile}} 
int GenericGraphicsInterface:: 
readLineFromCommandFile(aString & answer )
//================================================================================
// /Purpose: Read a line from the command file. Lines beginning with a "*" or "\#" are
//   treated as comments.
// /Return values: Number of characters read. A return value of zero means
//   that an end-of-file was reached.
//  /Author: WDH
//\end{GenericGraphicsInterfaceInclude.tex} 
//================================================================================
{
  int numberOfCharsRead=0;
  for(;;)  // loop so we skip comment lines
  {
    if( !stringCommands.empty() )
    { 
      answer=stringCommands.front();  // FIFO queue
      numberOfCharsRead=answer.length();
      stringCommands.pop();
    }
    else
    {
      assert(readFile!=NULL);

      //kkc 040224      const int buffSize=300;
      const int buffSize=1024*8;  // *wdh* 050105 =1024
      char buff[buffSize]; 
      numberOfCharsRead=getLineFromFile(readFile,buff,buffSize);   // returns number of chars read

      // int ok=fscanf(readFile,"%s",buff);
      if( numberOfCharsRead==0 )
      {
	printF("readLineFromCommandFile:End of file or a null line was read.\n");
        fclose(readFile);  // *wdh* 090809 
        readFile=NULL;
	
        while( !readFileStack.empty() )
	{
          // *wdh* 090809 fclose(readFile);
	  readFile=readFileStack.top();
	  readFileStack.pop();
          printF("pop the stack of read command files...\n");
	  numberOfCharsRead=getLineFromFile(readFile,buff,buffSize);
          if( numberOfCharsRead>0 ) break;
	}
	if( numberOfCharsRead==0 )
	{
	  answer=" ";
	  break;
	}
      }      
      if( numberOfCharsRead>buffSize-2 )
      {
	printf("readLineFromCommandFile:WARNING: %i chars read from command file. Buffer exceeded.",
	       numberOfCharsRead);
      }
      answer=buff;
    }
    // cout << "readLineFromCommandFile: line=[" << answer << "]\n";
    if( numberOfCharsRead==0 )
      break;

    if( numberOfCharsRead>0 )
    {
      // strip off leading and trailing blanks and tabs
      // cout << "before strip: answer=[" << answer << "]\n";
      aString line=answer;
      int i;
      for( i=0; i<line.length() && (line[i]==' ' || line[i]=='\t'); i++) {}
      if( i>0 )
	answer=line.substr(i,line.length()-i);
      line=answer;    
      for( i=line.length()-1; i>0 && (line[i]==' ' || line[i]=='\t'); i--) {}
      if( i>=0 )
	answer=line.substr(0,i+1);
      // cout << "after strip: answer=[" << answer << "]\n";
    }

    if( saveFile )
    {
      fPrintF(saveFile,"%s \n",(const char*)(indentBlanks+answer).c_str()); // print at least a blank for a 'default answer'
      // *wdh* 030414 if( ++saveFileCount > 10 )
      if( true )
      {
	saveFileCount=0;
	fflush(saveFile);  // flush the log file
      }
    }
    
// AP 020807
    if( answer.length()!=0 ) // *wdh* 030901
    {
      if( echoToTerminal==1 ) // *wdh* Nov 20, 2016 -- turn off echo of command line if echoToScreen==0
      {
    	// printF("readLineFromCommandFile: echoToTerminal=%i, echo answer...\n",echoToTerminal);
	if ( answer[0 ]== '*' || answer[0]=='#' )  // echo comments in the command window, hash added by kkc 080822
	  outputString( answer );
	else       // *** output line to the screen ***
	{
	  cout << answer << endl;
	}
      }
    }
    if( answer.length()==0 || (answer[0]!='*' && answer[0]!='#'))  // skip comments, hash added by kkc 080822
      break;
  }
  return numberOfCharsRead;
}

int GenericGraphicsInterface::
promptAnswer(aString & answer, 
	     const aString & prompt /* =nullString */)
// ================================================================================
// Display a menu and get a response, called by promptAnswerSelectPick
// /return value: the number of the menu item chosen, return a negative value
//   if the response is not in the list
// ===============================================================================
{
  // Display the menu   
#ifdef USE_PPP
  if( Communication_Manager::My_Process_Number==processorForGraphics )
#endif
  {
    int count=0;
    for( int i=0; i<currentGUI->nAllCommands; i++ )
    {
      int length=currentGUI->allCommands[i].length();
      count+=length;
      if( count > 80 )
      {
	cout << endl;
	count=length;
      }
      // skip items starting with !
      if( currentGUI->allCommands[i][0]=='!' )
	continue;
    
      // remove any initial > < or <> symbols (which are used for cascading menus)
      if( currentGUI->allCommands[i][0]=='>' || currentGUI->allCommands[i][0]=='<' )
      {
	if( length>1 && currentGUI->allCommands[i][1]=='>' )
	  cout << currentGUI->allCommands[i].substr(2,length-2) << "/";
	else
	  cout <<  currentGUI->allCommands[i].substr(1,length-1) << "/";
      }
      else
	cout << currentGUI->allCommands[i] << "/";
    }
    cout << endl;
  }
  
  GenericGraphicsInterface::inputString(answer,prompt);     // get a response
  return -1;
}


//--------------------------------------------------------------------------------------
//  Read a line from a file -- concatenate lines ending with a backslash "\"
//   FILE *file : file to read
//   char s[]   : char array in which to store the line
//   lim        : maximum number of chars that can be saved in s
//-------------------------------------------------------------------------------------
int 
getLineFromFile( FILE *file, char s[], int lim)
{
  int c,i=0;
  bool done=false;
  while( !done )
  {
    for( ; i<lim-1 && (c=fgetc(file))!=EOF && c!='\n'; ++i)
    {
      // printf("getLineFromFile: s[%i]=[%c]\n",i,c);
      s[i]=c;
    }
    if( i>0 && s[i-1]=='\\' )
    { // If the line ends in a back-slash then we concatenate the next line
      i--;  // remove the back-slash 
    }
    else
      done=true;
  }

  s[i]='\0';
  return i;
}

