#include "SurfaceEquation.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <algorithm>
#else
#include <algorithm.h>
#endif


SurfaceEquationFace::SurfaceEquationFace()
// =====================================================================
// /Description: Default constructor
// =====================================================================
{
  grid=-1;
  side=-1;
  axis=-1;
}

SurfaceEquationFace::SurfaceEquationFace(int grid_, int side_, int axis_)
// =====================================================================
// /Description: Build an object that points to a given face
// =====================================================================
{
  grid=grid_;
  side=side_;
  axis=axis_;
}

int SurfaceEquationFace::operator == ( const SurfaceEquationFace & face ) const
{
  return grid==face.grid && axis==face.axis && side==face.side;
}





SurfaceEquation::SurfaceEquation()
// =====================================================================
// /Description: Default SurfaceEquation constructor
// =====================================================================
{
  surfaceEquationType=heatEquation;
  numberOfSurfaceEquationVariables=1;
  kThermal=1.; Cp=1.; rho=1.;
}

SurfaceEquation::~SurfaceEquation()
// =====================================================================
// /Description: SurfaceEquation destructor
// =====================================================================
{
}


SurfaceEquation::SurfaceEquationType 
SurfaceEquation::getSurfaceEquationType() const
// =====================================================================
// /Description: Return the equation being solved on the surface
// =====================================================================
{
  return surfaceEquationType;
}

int SurfaceEquation::
setSurfaceEquationType(SurfaceEquationType type)
// =====================================================================
// /Description: Set the equation to be solved on the surface
// =====================================================================
{
  surfaceEquationType=type;
  return 0;
}



int SurfaceEquation::
update(CompositeGrid & cg, const IntegerArray & originalBoundaryCondition, 
       GenericGraphicsInterface & gi, 
       const aString & command /* =nullString */,
       DialogData *interface /* =NULL */ )
// =================================================================================
//  /Description:
//    Interactively update the SurfaceEquation object -- chose the surface equation type
// and choose faces where the surface equation should be applied etc.  
// =================================================================================
{
  int returnValue=0;

  aString prefix = "SE:"; // prefix for commands to make them unique.

  // ** Here we only look for commands that have the proper prefix ****
  const bool executeCommand = command!=nullString;
  if( executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;


  aString answer;
  char buff[100];
  const int numberOfDimensions = cg.numberOfDimensions();
  

  GUIState gui;
  gui.setExitCommand("done", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  if( interface==NULL || command=="build dialog" )
  {
    const int maxCommands=20;
    aString cmd[maxCommands];
    dialog.setWindowTitle("Surface Equation Parameters");

    aString commands[] = {"heat equation",
			  "" };

    dialog.addOptionMenu("", commands, commands, (int)surfaceEquationType);
      
    aString pbLabels[] = {"help",""};
    addPrefix(pbLabels,prefix,cmd,maxCommands);
    int numRows=1;
    dialog.setPushButtons( cmd, pbLabels, numRows );
      
    const int numberOfTextStrings=9;
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    textLabels[nt] = "surface equation:";  textStrings[nt]=" (specify faces here) ";  nt++;
    textLabels[nt] = "kThermal";  sPrintF(textStrings[nt], "%g",kThermal);  nt++; 
    textLabels[nt] = "Cp";  sPrintF(textStrings[nt], "%g",Cp);  nt++; 
    textLabels[nt] = "rho";  sPrintF(textStrings[nt], "%g",rho);  nt++; 


    // null strings terminal list
    assert( nt<numberOfTextStrings );
    textLabels[nt]="";   textStrings[nt]="";  
    addPrefix(textLabels,prefix,cmd,maxCommands);
    dialog.setTextBoxes(cmd, textLabels, textStrings);

    
    if( executeCommand ) return 0;
  }
  
  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("SurfaceEquation>");  
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

    printf("SurfaceEquation: answer=[%s]\n",(const char*)answer);
    

    if( answer=="done" )
      break;
    else if( dialog.getTextValue(answer,"kThermal","%e",kThermal) ){}//
    else if( dialog.getTextValue(answer,"Cp","%e",Cp) ){}//
    else if( dialog.getTextValue(answer,"rho","%e",rho) ){}//
    //    else if( dialog.getToggleValue(answer,"second-order ad",useSecondOrderArtificialDiffusion) ){}//
    else if( len=answer.matches("surface equation:") )
    {
      aString line = answer(len,answer.length()-1);
      // remove leading banks,
      int i=0;
      while( i<line.length() && line[i]==' ' ) i++;
      if( i==line.length() )
      {
	printf("Invalid specification of a surface equation (try choosing help)\n");
	printf("The original line was [%s]\n",(const char*)answer);
	continue;
      }
      

      line=line(i,line.length()-1);

      int bcNumber=-1;

      Range G=cg.numberOfComponentGrids();
      Range S(0,1), A(0,numberOfDimensions-1);
      
      if( len=line.matches("bcNumber") )
      {
	// a bc number has been specified
        sScanF(line(len,line.length()-1),"%i",&bcNumber);
      }
      else
      {
	// parse for <grid-name>(side,axis)
        int length=line.length(), mark=0;
	while( mark<length && line[mark]!='(' ) mark++;
        aString gridName=line(0,mark-1);

	// search for the name of the grid
        bool found=false;
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  if( gridName==cg[grid].mapping().getName(Mapping::mappingName) )
	  {
            found=true;
	    G=Range(grid,grid);
	    break;
	  }
	}
        if( !found )
	{
	  printf("unable to find a grid with the name=[%s]\n",(const char*)gridName);
          printf("The original line was [%s]\n",(const char*)answer);
          continue;
	}
	

	int side=-1,axis=-1;
	int numRead=sscanf(line(mark+1,length-1),"(%i,%i)",&side,&axis);
	if( numRead==2 )
	{
	  if( side>=0 && side<=1 && axis>=0 && axis<=cg.numberOfDimensions()-1 )
	  {
	    S=Range(side,side);
	    A=Range(axis,axis);
	  }
	  else
	  {
	    printf("invalid values for side=%i or axis=%i, 0<=side<=1, 0<=axis<=%i \n",side,axis,
		   cg.numberOfDimensions()-1);
            printf("The original line was [%s]\n",(const char*)answer);
	    continue; 
	  }
	}

      }


      bool wasAssigned=false;
      for( int grid=G.getBase(); grid<=G.getBound(); grid++ )
      {
	for( int axis=A.getBase(); axis<=A.getBound(); axis++ )
	{
	  for( int side=S.getBase(); side<=S.getBound(); side++ )
	  {
	    // printf("grid,side,axis,original,change=%i,%i,%i,  %i,%i\n",grid,side,axis,
	    //        originalBoundaryCondition(side,axis,grid),changeBoundaryConditionNumber);
		
	    if( cg[grid].boundaryCondition(side,axis) > 0 && 
		(bcNumber==-1 || originalBoundaryCondition(side,axis,grid)==bcNumber) )
	    {
	      wasAssigned=true;

              printf("The surface equation will be applied on face: (grid,side,axis)=(%i,%i,%i)\n",
		     grid,side,axis);
	      
              // check to avoid duplicates 
              std::vector<SurfaceEquationFace>::iterator result = find(faceList.begin(),faceList.end(),
                                                                       SurfaceEquationFace( grid, side, axis));
	      if( result==faceList.end() )
	      { 
		faceList.push_back(SurfaceEquationFace( grid, side, axis));
	      }
	      else
	      {
                printf("WARNING: The face (grid,side,axis)=(%i,%i,%i) already exists in the list!\n",
		       grid,side,axis);
	      }
	      
	    }
	  }
	}
      }
  
    }
    else if( answer=="help" )
    {
      printf("================================================================================================\n"
             " The surface equation is imposed on faces of grids. There are a number of ways to\n"
             " indicate which grid faces to use:\n"
	     "                                                                             \n"
             " To specify the face of one grid: "
	     "       surface equation: <grid name>(side,axis) \n"
	     "                                                                             \n"
	     " to specify all boundaries with a given boundary condition number:                   \n"
	     "                                                                             \n"
	     "       surface equation: bcNumber<num>  \n"
	     "                                                                             \n"
	     " Here <grid name> is the name of the grid, side=0,1 and axis=0,1,2.  \n"
	     "                                                                             \n"
	     " Examples: \n"
	     "    surface equation: annulus(0,1) \n"
	     "    surface equation: bcNumber5    \n"
             "================================================================================================\n"
	);
      
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
	cout << "Unknown response=[" << answer << "]\n";
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
