#include "Cgsm.h"
#include "PlotStuff.h"
#include "GL_GraphicsInterface.h"
#include "DialogData.h"
#include "Ogshow.h"

int Cgsm::
addPrefix(const aString label[], const aString & prefix, aString cmd[], const int maxCommands)
// ==============================================================================================
// /Description:
//    Add a prefix string to the start of every label.
// /label (input) : null terminated array of strings.
// /prefix (input) : all this string as a prefix.
// /cmd (input/output): on output cmd[i]=prefix+label[i];
// /maxCommands (input): maximum number of strings in the cmd array.
// ==============================================================================================
{
    
  int i;
  for( i=0; i<maxCommands && label[i]!=""; i++ )
    cmd[i]=prefix+label[i];
  if( i<maxCommands )
    cmd[i]="";
  else
  {
    printF("ERROR:addPrefix: maxCommands=%i is too small\n",maxCommands);
    assert( maxCommands>0 );
    cmd[maxCommands-1];
    return 1;
  }
  return 0;
}



//\begin{>>OB_ParametersInclude.tex}{\subsection{updateShowFile}} 
int Cgsm::
updateShowFile(const aString & command /* = nullString */,
               DialogData *interface /* =NULL */ )
// =======================================================================
// /Description:
//    Open or close show files, set variables that appear in the show file.
// 
// /command (input) : optionally supply a command to execute. Attempt to execute the command
//    and then return. The return value is 0 if the command was executed, 1 otherwise.
// /interface (input) : use this dialog. If command=="build dialog", fill in the dialog and return.
//
// /Return value: when executing a single command, return 1 if the command was not recognised.
//
// Here is a desciption of the menu options available for changing show file options.
//  \input ShowFileOptionsInclude.tex
//\end{OB_ParametersInclude.tex}  
// =======================================================================
{
  int returnValue=0;
  
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  aString prefix = "SMSF:"; // prefix for commands to make them unique.

  const bool executeCommand = command!=nullString;
  if( executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;


  GUIState gui;
  gui.setWindowTitle("Show File Options");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  if( interface==NULL || command=="build dialog" )
  {

    const int maxCommands=20;
    aString cmd[maxCommands];

    aString label[] = {"compressed", "uncompressed","" }; //
    addPrefix(label,prefix,cmd,maxCommands);
    dialog.addOptionMenu("mode", cmd, label, (useStreamMode? 0 : 1));

    aString tbLabel[] = {"save divergence", 
                         "save errors", 
                         ""}; // 
    int tbState[10];
    tbState[0] = saveDivergenceInShowFile; 
    tbState[1] = saveErrorsInShowFile; 
    int numColumns=1;
    addPrefix(tbLabel,prefix,cmd,maxCommands);
    dialog.setToggleButtons(cmd, tbLabel, tbState, numColumns); 


    aString pbLabels[] = {"open","close",""};
    addPrefix(pbLabels,prefix,cmd,maxCommands);
    int numRows=1;
    dialog.setPushButtons( cmd, pbLabels, numRows ); 

    const int numberOfTextStrings=20;
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    
    textLabels[nt] = "frequency to save";  sPrintF(textStrings[nt], "%i", frequencyToSaveInShowFile);  nt++; 

    int flushFrequency= show!=NULL ? show->getFlushFrequency() : 5;
    textLabels[nt] = "frequency to flush"; sPrintF(textStrings[nt], "%i", flushFrequency);  nt++; 

    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    addPrefix(textLabels,prefix,cmd,maxCommands);
    dialog.setTextBoxes(cmd, textLabels, textStrings);

/* -------
    // show file variables
    const int maximumNumberOfNames=numberOfComponents+20;
    aString *showLabel= new aString[maximumNumberOfNames];
    int *onOff = new int [maximumNumberOfNames];
    int i=0;
    for( int n=0; showVariableName[n]!=""; n++ )
    {
      showLabel[i]=showVariableName[n];
      onOff[i]=showVariable(i)>0;
      i++;
      assert( i+2 < maximumNumberOfNames );
    }
    showLabel[i]="";

    addPrefix(showLabel,prefix+"show variable: ",cmd,maxCommands);
    dialog.addPulldownMenu("show variables", cmd, showLabel, GI_TOGGLEBUTTON,onOff);
    delete [] showLabel;
    delete [] onOff;
  ------------ */

    if( executeCommand ) return 0;
  }
  
//   aString menu[]=
//   {
//     "!show file",
//     "open",
//     "close",
//     "show file variables",
//     "frequency to save",
//     "frequency to flush",
//     ">properties",
//       "uncompressed",
//       "compressed",
//     "<exit",
//     ""
//   };

//\begin{>ShowFileOptionsInclude.tex}{}
//\no function header:
//
// \begin{description} \index{show file!options}
//  \item[open] : open a new show file.
//  \item[close] : close any open show file.
//  \item[show file variables] : specify extra derived quantities, such as the divergence or vorticity, that
//      should be saved in the show file in addition to the standard variables.
//  \item[frequency to save] : By default the solution is saved in the show file
//          as often as it is plotted according to {\tt 'times to plot'}. To save the solution less
//          often set this integer value to be greater than 1. A value of 2 for example will save solutions
//          every 2nd time the solution is plot.
//  \item[frequency to flush] : Save this many solutions in each show file so that multiple
//        show files will be created (these are automatically handled by plotStuff). See section~(\ref{sec:flush})
//        for why you might do this.  
//  \item[properties]: \ 
//    \begin{description}
//    \item[uncompressed] : save the show file uncompressed. This is a more portable format
//     that can be read by newer versions of Overture.
//    \item[compressed] : save the show file compressed. This is a less portable format.
//   \end{description}
// \end{description}
//\end{ShowFileOptionsInclude.tex}


  aString answer,answer2;
  char buff[100];

  
  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("showFile>");  
  }
  
  for(int it=0; ; it++)
  {
    if( !executeCommand )
      gi.getAnswer(answer,"");
    else
    {
      if( it==0 ) 
        answer=command;
      else
        break;
    }
  
    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);

    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="open" )
    {
      gi.inputFileName(answer2,"Enter the name of the show file (e.g. mx.show)");
      if( answer2!="" )
      {
        if( show!=NULL )
	{
          printF("INFO:closing the currently open show file\n");
	  show->close();
	}
	printf("Opening the show file %s\n",(const char*)answer2);
	show = new Ogshow( answer2,".",useStreamMode );      

        saveParametersToShowFile();
      }
    }
    else if( answer=="close" )
    {
      if( show!=NULL )
      {
	show->close();
        delete show;
        show=NULL;
      }
      else
      {
	printf("ERROR:There is no open show file\n");
      }
    }
    else if( answer=="uncompressed" )
    {
      useStreamMode=false;
      printF("Any newly opened show files will be saved in uncompressed format\n");
    }
    else if( answer=="compressed" )
    {
      useStreamMode=true;
      printF("Any newly opened show files will be saved in compressed format\n");
    }
    else if( dialog.getToggleValue(answer,"save divergence",saveDivergenceInShowFile) ){}//
    else if( dialog.getToggleValue(answer,"save errors",saveErrorsInShowFile) ){}//
/* -------
    else if( answer(0,13)=="show variable:" )
    {
      // answer looks like: "show file variable: vorticity 1"
      answer2=answer(15,answer.length()-3);   //
      int i=-1;
      for( int n=0; showVariableName[n]!=""; n++ )
      {
	if( answer2==showVariableName[n] )
	{
          showVariable(n)=-showVariable(n);
          i=n;
          break;
	}
      }
      if( i<0 )
        printF("ERROR: unknown response: answer=[%s], answer2=[%s]\n",(const char*)answer,(const char*)answer2);
    }

    else if( answer=="show file variables" )
    {
      const int maximumNumberOfNames=numberOfComponents+20;
      aString *showMenu= new aString[maximumNumberOfNames];
      for( ;; )
      {
	int i=0;
	for( int n=0; showVariableName[n]!=""; n++ )
	{
	  showMenu[i]=showVariableName[n] + (showVariable(i)>0 ? " (on)" : " (off)");
          i++;
          assert( i+2 < maximumNumberOfNames );
	}
	showMenu[i++]="done";
	showMenu[i]="";

	int response=gi.getMenuItem(showMenu,answer2,"toggle variables to save in the show file");
        if( answer2=="done" || answer2=="exit" )
	  break;
	else if( response>=0 && response<i-1 )
	  showVariable(response)=-showVariable(response);
	else
	{
	  cout << "Unknown response: [" << answer2 << "]\n";
	  gi.stopReadingCommandFile();
	}
	
      }
      delete [] showMenu;
    }
  ------ */
    else if( answer=="frequency to save" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the frequencyToSaveInShowFile (default value=%i)",
             frequencyToSaveInShowFile));
      if( answer2!="" )
	sScanF(answer2,"%i",&frequencyToSaveInShowFile);
      cout << " frequencyToSaveInShowFile=" << frequencyToSaveInShowFile << endl;
    }
    else if( answer(0,16)=="frequency to save" )
    {
      sScanF(answer(17,answer.length()-1),"%i",&frequencyToSaveInShowFile);
      cout << " frequencyToSaveInShowFile=" << frequencyToSaveInShowFile << endl;
      dialog.setTextLabel("frequency to save",sPrintF(answer2,"%i", frequencyToSaveInShowFile));  
    }
    else if( answer=="frequency to flush" )
    {
      int flushFrequency;
      gi.inputString(answer2,"Enter the frequency to flush the show file");
      if( answer2!="" )
	sScanF(answer2,"%i",&flushFrequency);
      flushFrequency=max(1,flushFrequency);
      if( show!=NULL )
        show->setFlushFrequency( flushFrequency );
      cout << " flushFrequency=" << flushFrequency << endl;
    }
    else if( answer(0,17)=="frequency to flush" )
    {
      int flushFrequency= show!=NULL ? show->getFlushFrequency() : 5;
      sScanF(answer(18,answer.length()-1),"%i",&flushFrequency);
      flushFrequency=max(1,flushFrequency);
      if( show!=NULL )
        show->setFlushFrequency( flushFrequency );
      cout << " flushFrequency=" << flushFrequency << endl;
      dialog.setTextLabel("frequency to flush",sPrintF(answer2,"%i", flushFrequency));  
    }
    else
    {
      if( executeCommand )
      {
	returnValue= 1;  // when executing a single command, return 1 if the command was not recognised.
        break;
      }
      else
      {
	cout << "Unknown response: [" << answer << "]\n";
	gi.stopReadingCommandFile();
      }
       
    }
    
  }

  if( !executeCommand  )
  {
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }
  
  return returnValue;
}
