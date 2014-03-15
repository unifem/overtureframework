#include "MatchingCurve.h"

#include "MappingProjectionParameters.h"
#include "PlotStuff.h"

MatchingCurve::
MatchingCurve()
{ 
  curve=NULL; 
  projectionParameters=NULL; 
  numberOfLinesForNormalBlend=3;
  gridLine=-100;  // put bogus value
  curvePosition=0.;
  curveDirection=0;
} 

MatchingCurve::
~MatchingCurve()
{
  if( curve!=NULL && curve->decrementReferenceCount()==0 )
    delete curve;
  delete projectionParameters;
}


MatchingCurve::
MatchingCurve(const MatchingCurve & mc)
{
  curve=NULL; 
  projectionParameters=NULL; 
  numberOfLinesForNormalBlend=3;
  gridLine=-100;  // put bogus value
  curvePosition=0.;
  curveDirection=0;

  *this=mc;
}

void MatchingCurve::
setCurve( Mapping & curve_ )
{
  curve=&curve_;
  if(  projectionParameters==NULL )
    projectionParameters=new MappingProjectionParameters;
}

MatchingCurve& MatchingCurve::
operator=(const MatchingCurve & mc )
{
  x[0]=mc.x[0]; x[1]=mc.x[1]; x[2]=mc.x[2];
  curvePosition=mc.curvePosition; 
  gridLine=mc.gridLine;
  curveDirection=mc.curveDirection; 

  if( curve!=NULL && curve->decrementReferenceCount()==0 )
    delete curve;
  curve=mc.curve;     
  if( curve!=NULL ) curve->incrementReferenceCount();
  
  // make a deep copy of the projectionParameters so we don't have to worry about ref counting.
  if( mc.projectionParameters!=NULL )
  {
    if( projectionParameters==NULL )
      projectionParameters=new MappingProjectionParameters;

    // *projectionParameters=*mc.projectionParameters;   // do we need to copy??
  }
  
  numberOfLinesForNormalBlend=mc.numberOfLinesForNormalBlend;  
  return *this;
}


//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int MatchingCurve::
update( GenericGraphicsInterface & gi )
{

  GUIState gui;

  DialogData & dialog=gui;

  dialog.setWindowTitle("Matching Curve Parameters");
  dialog.setExitCommand("continue", "continue");

  dialog.setOptionMenuColumns(1);


  // ************** PUSH BUTTONS *****************
//   aString pushButtonCommands[] = {"time stepping options...",
//                                   "forcing options...",
//                                   "plot options...",
//                                   "input-output options...",
//                                   "pde parameters...",
// 				  ""};
//   int numRows=3;
//   dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 


  aString directionCommands[] = {"match to foward", "match to backward", "match to both", "" };
  int curveDir = curveDirection==1 ? 0 : curveDirection==-1 ? 1 : 2;
  dialog.addOptionMenu("direction:", directionCommands, directionCommands, (int)curveDirection );

  // ----- Text strings ------
  const int numberOfTextStrings=30;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "normal blending";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i",numberOfLinesForNormalBlend);  nt++; 

  textCommands[nt] = "grid line";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i",gridLine);  nt++; 

  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  gi.pushGUI(gui);
  aString answer;
  int len=0;
  for(;;) 
  {
    gi.getAnswer(answer,"");      
    // printF("Start: answer=[%s]\n",(const char*) answer);
    
    if( answer=="continue" || answer=="exit" )
    {
      break;
    }
    else if( answer=="match to foward" ||
	     answer=="match to backward" ||
	     answer=="match to both" )
    {
      curveDirection = answer=="match to foward" ? 1 : answer=="match to backward" ? -1 : 0;
    }
    else if( dialog.getTextValue(answer,"normal blending","%i",numberOfLinesForNormalBlend) ){}//
    else if( dialog.getTextValue(answer,"grid line","%i",gridLine) ){}//
    else
    {
      printF("MatchingCurve::update:ERROR:Unknown command = [%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
  }

  gi.popGUI();  // pop dialog
  return 0;
}
