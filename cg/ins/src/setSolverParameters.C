#include "Cgins.h"
#include "GenericGraphicsInterface.h"
#include "Oges.h"

int Cgins::
setSolverParameters(const aString & command /* = nullString */,
                    DialogData *interface /* =NULL */ )
// =====================================================================================
// /Description:
//   Prompt for changes in the implicit solver parameters -- this dialog can be shown while
// DomainSolver is running.
// =====================================================================================
{
  int returnValue=0;

  assert( parameters.dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  GridFunction & solution = gf[current];

  aString prefix = "CGSOL:"; // prefix for commands to make them unique.

  const bool executeCommand = command!=nullString;
  if( executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;


  aString answer;
  char buff[100];
//  const int numberOfDimensions = cg.numberOfDimensions();
  
  GUIState gui;
  gui.setExitCommand("done", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  if( interface==NULL || command=="build dialog" )
  {
    const int maxCommands=20;
    aString cmd[maxCommands];

    dialog.setWindowTitle("Solver parameters");

    aString pbLabels[] = {"pressure solver parameters...",
			  "implicit solver parameters...",
			  "display parameters",
			  "" };

    // *** NOTE: to distinguish the commands we add a prefix to each command ***

    addPrefix(pbLabels,prefix,cmd,maxCommands);
    int numRows=2;
    dialog.setPushButtons( cmd, pbLabels, numRows );
      
    // ----- Text strings ------
    const int numberOfTextStrings=20;
    aString textCommands[numberOfTextStrings];
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;   
    textCommands[nt] = "dtMax"; textLabels[nt]=textCommands[nt];
    sPrintF(textStrings[nt], "%9.3e", parameters.dbase.get<real >("dtMax")); nt++; 

    textCommands[nt] = "implicit factor"; textLabels[nt]=textCommands[nt];
    sPrintF(textStrings[nt], "%9.3e (1=BE,0=FE)", parameters.dbase.get<real >("implicitFactor")); nt++; 

    textCommands[nt] = "refactor frequency"; textLabels[nt]=textCommands[nt];
    sPrintF(textStrings[nt], "%i", parameters.dbase.get<int>("refactorFrequency")); nt++; 

    // null strings terminal list
    textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );

    addPrefix(textCommands,prefix,cmd,maxCommands);
    dialog.setTextBoxes(cmd, textLabels, textStrings);

    if( executeCommand ) return 0;
  }
  

  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("solver parameters>");  
  }
  int len;
  for(int it=0; ; it++)
  {
    if( !executeCommand )
    {
      gi.getAnswer(answer,"");
    }
    else
    {
      if( it==0 ) 
        answer=command;
      else
        break;
    }
  
    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);   // strip off the prefix

    if( answer=="done" )
      break;
    else if( answer=="pressure solver parameters..." )
    {
//  Oges *poisson;  // for pressure solve
// pressureSolverParameters.update(gi,cg);
      if( poisson!=NULL )
      {
	poisson->parameters.update(gi,solution.cg);
      }
      else
      {
	printf("WARNING: There is no pressure solver built!\n");
      }
    }
    else if( answer=="implicit solver parameters..." )
    {
// int numberOfImplicitSolvers;
//  Oges *implicitSolver;  // array of implicit solvers
// implicitTimeStepSolverParameters.update(gi,cg);
      if( numberOfImplicitSolvers<=0 || implicitSolver==NULL )
      {
	printf("WARNING: There are no implicit solvers built!\n");
      }
      else
      {
	for( int i=0; i<numberOfImplicitSolvers; i++ )
	{
	  printF("Change parameters for implicit solver %i (out of a total of %i implicit solvers)\n",
		 i+1,numberOfImplicitSolvers);
	  implicitSolver[i].parameters.update(gi,solution.cg);
	}
      }
    }
    else if( answer=="display parameters" )
    {
      displayParameters();
    }
    else if ( dialog.getTextValue(answer,"dtMax", "%e", parameters.dbase.get<real>("dtMax")) ){} //
    else if ( dialog.getTextValue(answer,"implicit factor", "%e", parameters.dbase.get<real>("implicitFactor")) ){} //
    else if ( dialog.getTextValue(answer,"refactor frequency", "%i", parameters.dbase.get<int>("refactorFrequency"))){} //
    else
    {
      if( executeCommand )
      {
	returnValue= 1;  // when executing a single command, return 1 if the command was not recognised.
        break;
      }
      else
      {
	printF("Cgins:setSolverParameters: Unknown response=[%s]\n",(const char*)answer);
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
