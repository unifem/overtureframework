#include "ExplicitHoleCutter.h"
#include "PlotIt.h"
#include "MappingInformation.h"

//============================================================================================
/// \brief This class defines an explicit hole cutter for Ogen
//============================================================================================
ExplicitHoleCutter::
ExplicitHoleCutter()
{
  name="explicitHoleCutter";
}

//============================================================================================
/// \brief destructor
//============================================================================================
ExplicitHoleCutter::
~ExplicitHoleCutter()
{

}

//============================================================================================
/// \brief copy constructor
//============================================================================================
ExplicitHoleCutter::
ExplicitHoleCutter( const ExplicitHoleCutter & holeCutter )
{
  *this=holeCutter;
}


//============================================================================================
/// \brief equals operator
//============================================================================================
ExplicitHoleCutter & 
ExplicitHoleCutter::operator=( const ExplicitHoleCutter & holeCutter )
{
  name=holeCutter.name;
  holeCutterMapping.reference(holeCutter.holeCutterMapping);
  mayCutHoles.redim(0);
  mayCutHoles=holeCutter.mayCutHoles;

  return *this;
}


//============================================================================================
/// \brief Interactively update properties of the explicit hole cutter.
//============================================================================================

int ExplicitHoleCutter::
update( GenericGraphicsInterface & gi , MappingInformation & mapInfo, CompositeGrid & cg )
{

  const int numberOfDimensions=cg.numberOfDimensions();
  
  GUIState dialog;

  dialog.setWindowTitle("Explicit Hole Cutter");
  dialog.setExitCommand("exit", "exit");

  aString cmds[] = {"prevent hole cutting",
                    "allow hole cutting",
		    "show parameters",
		    ""};
  int numberOfPushButtons=3;  // number of entries in cmds
  int numRows=(numberOfPushButtons+1)/2; // (numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 



  const int num=mapInfo.mappingList.getLength();
  aString *label = new aString[num+2];

  dialog.setOptionMenuColumns(1);

  const int maxCommands= max(20,num+2);
  aString *cmd = new aString [maxCommands];

  int j=0;  // counts possible hole cutters
  for( int i=0; i<num; i++ )
  {
    MappingRC & map = mapInfo.mappingList[i];
    if( map.getDomainDimension()==numberOfDimensions && map.getRangeDimension()==numberOfDimensions )
    {
      label[j]=map.getName(Mapping::mappingName);
      cmd[j]="Hole cutter:"+label[j];
      j++;
    }
  }
  if ( j==0 )
    {
      label[j] = cmd[j] = "-- none --";
      j++;
    }
  label[j]=""; cmd[j]="";   // null string terminates the menu

  dialog.addOptionMenu("Hole cutter:", cmd,label,0);

  delete [] label;

  const int numberOfTextStrings=5;  // max number allowed
  aString textCommands[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textCommands[nt] = "name:";  
  sPrintF(textStrings[nt],"%s",(const char*)name);  nt++;   

  // null strings terminal list
  textCommands[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textCommands, textStrings);


  gi.pushGUI(dialog);

  int len=0;
  aString answer,line;
  gi.appendToTheDefaultPrompt("explicitHoleCutter>"); // set the default prompt

  for( int it=0;; it++ )
  {
 
    gi.getAnswer(answer,"");  
 
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( dialog.getTextValue(answer,"name:","%s",name) ){} 
    else if( (len=answer.matches("Hole cutter:")) )
    {
      aString name=answer(len,answer.length()-1);
      const int num=mapInfo.mappingList.getLength();
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( map.getDomainDimension()==numberOfDimensions && map.getRangeDimension()==numberOfDimensions  &&
	    name==map.getName(Mapping::mappingName) )
	{
          printF("Choosing mapping=[%s] as a hole cutter\n",(const char*)name);
	  
	  holeCutterMapping.reference(map);
	}
      }
    }
    else if( answer=="prevent hole cutting" )
    {
      printF("Prevent hole cutting in which grids? (enter a list, enter 'done' when finished)\n");
      for( ;; )
      {
	gi.inputString(line,"Enter a grid name, 'all', or 'done'");
	if( line=="done" )
	{
	  break;
	}
	else if( line=="all" )
	{
	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    mayCutHoles(grid)=false;
            printF("Prevent hole cutting in grid=[%s]\n",(const char*)cg[grid].getName());
	  }
	}
	else 
	{
	  bool gridFound=false;
	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
            if( line==cg[grid].getName() )
	    {
	      gridFound=true;
	      mayCutHoles(grid)=false;
	      printF("Prevent hole cutting in grid=[%s]\n",(const char*)cg[grid].getName());
	    }
	  }
	  if( !gridFound )
	  {
	    printF("ERROR: unknown response=[%s]\n",(const char*)line);
	    gi.stopReadingCommandFile();
	  }
	  
	}
	
      }

    }
    else if( answer=="allow hole cutting" )
    {
      printF("Allow hole cutting in which grids? (enter a list, enter 'done' when finished)\n");
      for( ;; )
      {
	gi.inputString(line,"Enter a grid name, 'all', or 'done'");
	if( line=="done" )
	{
	  break;
	}
	else if( line=="all" )
	{
	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    mayCutHoles(grid)=true;
            printF("Allow hole cutting in grid=[%s]\n",(const char*)cg[grid].getName());
	  }
	}
	else 
	{
	  bool gridFound=false;
	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
            if( line==cg[grid].getName() )
	    {
	      gridFound=true;
	      mayCutHoles(grid)=true;
	      printF("Allow hole cutting in grid=[%s]\n",(const char*)cg[grid].getName());
	    }
	  }
	  if( !gridFound )
	  {
	    printF("ERROR: unknown response=[%s]\n",(const char*)line);
	    gi.stopReadingCommandFile();
	  }
	  
	}
	
      }

    }
    else if( answer=="show parameters" )
    {
      printF("--- Explicit hole cutter parameters---\n");
      printF(" name=[%s]\n",(const char*)name);
      printF(" mapping=[%s]\n",(const char*)holeCutterMapping.getName(Mapping::mappingName));
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	printF(" mayCutHoles=%s for grid=%i (%s)\n",(mayCutHoles(grid) ? "true" : "false"),grid,(const char*)cg[grid].getName());
      }
    }
    else
    {
      printF("Unknown answer =[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

  }

  delete [] cmd;
  
  gi.unAppendTheDefaultPrompt();  // reset
  gi.popGUI(); // restore the previous GUI

  
  return 0;
}
