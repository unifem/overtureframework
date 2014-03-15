#include "ModelBuilder.h"
#include "rap.h"
#include "IgesReader.h"

static void
setupNewDialog(GUIState & interface, aString & defaultFN, int defaultFormat, int startMap, int endMap)
{
  int i,j;
  
  interface.setWindowTitle("Import model");
  
  interface.setExitCommand("close", "Close");

// define push buttons
  aString pbCommands[] = {"browse file", "all mappings", "read file", ""};
  aString pbLabels[] = {"Browse...", "All surfaces", "Read file", ""};
  
  interface.setPushButtons( pbCommands, pbLabels, 1 ); // default is 2 rows

// define pulldown menus
  aString pdCommand2[] = {"help format", "help browse", "help read", "help filename", 
			  "help modelname", ""};
  aString pdLabel2[] = {"Format", "Browse", "Read", "Filename", "Modelname", ""};
  interface.addPulldownMenu("Help", pdCommand2, pdLabel2, GI_PUSHBUTTON);

  interface.setLastPullDownIsHelp(1);
// done defining pulldown menus  

// first option menu
  aString opCommand0[] = {"format iges", "format stl", "format solid", "format mappings", ""};
  aString opLabel0[]   = {"Iges", "Stl", "Solid", "Mappings", ""};
 
// initial choice: defaultFormat
  interface.addOptionMenu( "Format", opCommand0, opLabel0, defaultFormat); 

// text labels
  aString textCommands[] = {"filename", "first mapping", "last mapping", ""};
  aString textLabels[] = {"Filename", "First surface", "Last surface", ""};
  aString textStrings[4];
  
  sPrintF(textStrings[0], "%s", SC defaultFN);
  sPrintF(textStrings[1], "%i", startMap);
  sPrintF(textStrings[2], "%i", endMap);
  interface.setTextBoxes(textCommands, textLabels, textStrings);
}

//\begin{>>ModelBuilderInclude.tex}{\subsection{newModel}}
bool ModelBuilder::
newModel(GenericGraphicsInterface & ps, MappingInformation & mapInfo, CompositeSurface &model)
//===========================================================================
// /Description:
//    Read a new model from a file.
//\end{ModelBuilderInclude.tex}
//===========================================================================
{
  enum InputFormat{ igesFormat, stlFormat, solidFormat, mappingsFormat };
  
  static aString fileName="<empty>";
  static InputFormat fileType=igesFormat;

  GUIState interface;
  MappingsFromCAD mapCad;
  static int numberOfNurbs, numberOfFiniteElements, numberOfNodes;
  static IgesReader * iges_pointer=NULL;
  static int startMap=0, endMap=0;
  
  setupNewDialog(interface, fileName, fileType, startMap, endMap);
  enum textItems{fileNameItem=0, startMapItem, endMapItem, numberOfTextItems};

  aString answer, buf, extension[]={".igs", ".stl", ".sld", ".hdf"};

// bring up the interface on the screen
  ps.pushGUI( interface );

  ps.setDefaultPrompt("");
  int retCode;
  int status;
  
  for(;;)
  {
    retCode = ps.getAnswer(answer, "");
    if (answer == "close")
    {
      break;
    }
//                           01234
    else if (answer(0,3) == "help")
    {
      aString topic;
      topic = answer(5,answer.length()-1);
      if (!ps.displayHelp(topic))
      {
	aString msg;
	sPrintF(msg,"Sorry, there is currently no help for `%s'", SC topic);
	ps.createMessageDialog(msg, informationDialog);
      }
    }
    else if (answer == "browse file")
    {
      aString newFileName;
      ps.inputFileName(newFileName, "", extension[fileType]);
      if (newFileName.length() > 0 && newFileName != " ")
      {
//
// Do the reading in two steps: 
//   1. Check the number of surfaces in the file
	int status;
	
	mapCad.fileContents( newFileName, iges_pointer, numberOfNurbs, numberOfFiniteElements, 
			     numberOfNodes, status);
	if (status != 0)
	{
	  ps.createMessageDialog(sPrintF(buf, "The file `%s' could not be opened", SC newFileName), 
				 errorDialog);
	}
	else
	{
	  fileName = newFileName;
	  startMap=0;
	  endMap = numberOfNurbs-1;
	  
	  interface.setTextLabel(startMapItem, "0");
	  interface.setTextLabel(endMapItem, sPrintF(buf,"%i", endMap));
	  
	  ps.outputString(sPrintF(buf, "numberOfNurbs: %i, numberOfFiniteElements: %i, numberOfNodes: %i",
				  numberOfNurbs, numberOfFiniteElements, numberOfNodes));
	}
// write back to the text box
	interface.setTextLabel(fileNameItem, fileName); 
	

      }
      else
      {
	sPrintF(buf,"Bad file name: `%s'", SC newFileName);
	ps.createMessageDialog(buf, errorDialog);
      }
      
    }
    else if ( answer == "all mappings" )
    {
      startMap = 0;
      endMap = numberOfNurbs-1;
// write back to the text box
      interface.setTextLabel(startMapItem, sPrintF(buf,"%i", startMap));
      interface.setTextLabel(endMapItem, sPrintF(buf,"%i", endMap));
    }//                        01234567890123456789
    else if ( answer(0,12) == "first mapping" )
    {
      if (answer.length() > 14)
      {
	int s=-1;
	sScanF(answer(14,answer.length()-1),"%d",&s);
	if (s >= 0 && s<= endMap)
	{
	  startMap=s;
	}
	else
	{
	  sPrintF(buf, "The first surface must be between 0 and the last surface");
	  ps.createMessageDialog(buf, errorDialog);
	}
      }
// write back to the text box
      interface.setTextLabel(startMapItem, sPrintF(buf,"%i",startMap));
    }//                        01234567890123456789
    else if ( answer(0,11) == "last mapping" )
    {
      if (answer.length() > 13)
      {
	int s=-1;
	sScanF(answer(13,answer.length()-1),"%d",&s);
	if (s >= startMap && s<numberOfNurbs)
	{
	  endMap=s;
	}
	else
	{
	  sPrintF(buf, "The last surface must be between the first surface and %i", numberOfNurbs-1);
	  ps.createMessageDialog(buf, errorDialog);
	}
      }
// write back to the text box
      interface.setTextLabel(endMapItem, sPrintF(buf,"%i",endMap));
    }//                       0123456789
    else if ( answer(0,7) == "filename" )
    {
      aString newFileName = answer(9,answer.length()-1);
      if (newFileName.length() > 0 && newFileName != " ")
      {
//   1. Check the number of surfaces in the file
	
	mapCad.fileContents( newFileName, iges_pointer, numberOfNurbs, numberOfFiniteElements, 
			     numberOfNodes, status);
	if (status != 0)
	{
	  ps.createMessageDialog(sPrintF(buf, "The file `%s' could not be opened", SC newFileName), 
				 errorDialog);
	}
	else
	{
	  fileName = newFileName;
	  startMap=0;
	  endMap = numberOfNurbs-1;
	  
	  interface.setTextLabel(startMapItem, "0");
	  interface.setTextLabel(endMapItem, sPrintF(buf,"%i", endMap));
	  
	  ps.outputString(sPrintF(buf, "numberOfNurbs: %i, numberOfFiniteElements: %i, numberOfNodes: %i",
				  numberOfNurbs, numberOfFiniteElements, numberOfNodes));
	}
// write back to the text box
	interface.setTextLabel(fileNameItem, fileName); 
      }
      else
      {
	sPrintF(buf,"Bad file name: `%s'", SC newFileName);
	ps.createMessageDialog(buf, errorDialog);
// write back to the text box
	interface.setTextLabel(0, fileName); // This is text label # 0
      }

    }
    else if ( answer == "read file" )
    {
//   2. Read a specified number of surfaces.
//
      if ( iges_pointer && (iges_pointer->fp = fopen(fileName,"rb")) )
      {
	CompositeSurface * subModel_=
	  mapCad.readSomeNurbs(mapInfo, iges_pointer, startMap, endMap, numberOfNurbs, status);
	fclose(iges_pointer->fp);
	if (status == 0 && subModel_ != NULL)
	{
// get all the component mappings in the composite surface and put them into the base model...
	  printf("Adding...\n");
	  int map;
	  for( map=0; map<subModel_->numberOfSubSurfaces(); map++ )
	  {
	    model.add((*subModel_)[map], map+startMap);
      
	    printf("%i,",map);
	    fflush(stdout);
	  }
	  printf("\n");

// delete the subModel
	  if (subModel_->decrementReferenceCount() == 0)
	    delete subModel_;

	  model.setTolerance(max(model.getTolerance(),iges_pointer->getTolerance()));  // wdh:010923 assign cad tolerance
	
	  break; // get out of this loop if the read was successful
	}
	else
	{
	  ps.createMessageDialog(sPrintF(buf, "readSomeNurbs returned error code %i", status), errorDialog);
	}
      }
      else
      {
	ps.outputString(sPrintF(buf,"Error: could not open the file `%s'", SC fileName));
      }
      
    }//                       0123456789
    else if ( answer(0,5) == "format" )
    {
      aString formatName;
      formatName = answer(7,answer.length()-1);
      if (formatName == "iges")
      {
	fileType = igesFormat;
      }
//        else if (formatName == "stl")
//        {
//  	fileType = stlFormat;
//        }
//        else if (formatName == "solid")
//        {
//  	fileType = solidFormat;
//        }
//        else if (formatName == "mappings")
//        {
//  	fileType = mappingsFormat;
//        }
      else
      {
	ps.createMessageDialog("Sorry, only IGES files can currently be read", errorDialog);
      }
      sPrintF(buf, "Format: %i", fileType);
      ps.outputString(buf);
      
    }
   
  }

  ps.popGUI();
// check if everything is ok...
  return true;
}
