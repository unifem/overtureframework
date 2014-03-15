#include "ShowFilePlotter.h"
#include "HDF_DataBase.h"
#include "DisplayParameters.h"
#include "display.h"
#include "DerivedFunctions.h"
#include "FileOutput.h"
#include "PlotIt.h"
#include "DialogData.h"
#include "DataFormats.h"
#include "BodyForce.h"

// ********************************************************
// This class is used to plot results found in files.
// This class is used by the plotStuff program
// ********************************************************

int
saveOvertureTextFile( realCompositeGridFunction & u, 
                      GenericGraphicsInterface & gi, 
                      ShowFileReader *pShowFileReader=NULL );

ShowFilePlotter::
ShowFilePlotter(const aString & nameOfShowFile_, GenericGraphicsInterface & ps_) : ps(ps_)
// =======================================================================
// /Description:
//    Constructor.
// =======================================================================
{
  nameOfShowFile=nameOfShowFile_;
  showFileReader.open(nameOfShowFile);

  numberOfFrameSeries=0;
  numberOfMovieFrames=0;
  frameStride=1;
  movieFileName="movie";      // base name for movie ppm's
  saveMovieFiles=false;
  
  sequenceName = NULL;

  cg = NULL;
  u =NULL;
  psp=NULL;
    
  component= NULL; 
  solutionNumber= NULL;
  plotOptions= NULL;

  numberOfSolutions=NULL;
  numberOfFrames=NULL;
  numberOfComponents=NULL;
  numberOfComponents0=NULL;
  numberOfSequences=NULL;

  maximumNumberOfHeaderComments=20;
  headerComment=NULL;
   
  applyCommandsToAllSeries=true;
  
  dbaseArray = NULL;
}

ShowFilePlotter::
~ShowFilePlotter()
// =======================================================================
// /Description:
//    Destructor.
// =======================================================================
{
  delete [] cg;
  delete [] u;
  delete [] psp;
  
  delete [] component;
  delete [] solutionNumber;
  delete [] plotOptions;
  delete [] numberOfSolutions;
  delete [] numberOfFrames;
  delete [] numberOfComponents;
  delete [] numberOfComponents0;
  delete [] numberOfSequences;
  if( headerComment!=NULL )
  {
    for(int fs=0; fs<numberOfFrameSeries; fs++ )
    {
      delete [] headerComment[fs];
    }
    delete [] headerComment;
  }
  delete [] sequenceName;
  delete [] dbaseArray;

  for( int fs=0; fs<numberOfFrameSeries; fs++ )
  {
    DataBase & dbase = dbaseArray[fs];
    if( dbase.has_key("bodyForcings") )
    {
      std::vector<BodyForce*> & bodyForcings =  dbase.get<std::vector<BodyForce*> >("bodyForcings");
      for( int bf=0; bf<bodyForcings.size(); bf++ )
      {
	delete bodyForcings[bf];
      }
    }
    if( dbase.has_key("boundaryForcings") )
    {
      std::vector<BodyForce*> & boundaryForcings =  dbase.get<std::vector<BodyForce*> >("boundaryForcings");
      for( int bf=0; bf<boundaryForcings.size(); bf++ )
      {
	delete boundaryForcings[bf];
      }
    }
  }
  
}

static int totalNumberOfArrays=0;
static void
checkArrays(const aString & label)
// Output a warning messages if the number of arrays has increased
{
  if( false )  // **** set this to true to turn on array checking ***
  {
    if(GET_NUMBER_OF_ARRAYS > totalNumberOfArrays ) 
    {
      totalNumberOfArrays=GET_NUMBER_OF_ARRAYS;
      printf("\n**** %s :Number of A++ arrays = %i \n\n",(const char*)label,GET_NUMBER_OF_ARRAYS);
    }
  }
}

static int numberOfPushButtons=0;
static int numberOfTextBoxes=0;

int ShowFilePlotter::
buildPlotStuffDialog(DialogData & dialog, realCompositeGridFunction & u0, const int cfs )
// =============================================================================================
// =============================================================================================
{
  dialog.setWindowTitle("plotStuff");
  dialog.setExitCommand("exit", "exit");


  // option menus
  dialog.setOptionMenuColumns(1);

  int numberOfFrameSeries=showFileReader.getNumberOfFrameSeries();
  if( numberOfFrameSeries>1 )
  {
    aString *cmd = new aString[numberOfFrameSeries+1];
    aString *label = new aString[numberOfFrameSeries+1];
    for( int fs=0; fs<numberOfFrameSeries; fs++ )
    {
      label[fs]=showFileReader.getFrameSeriesName(fs);
      cmd[fs]="frame series:"+label[fs];
    }
    cmd[numberOfFrameSeries]="";
    dialog.addOptionMenu("frame series:",cmd,label,0);
    delete [] cmd;
    delete [] label;
  }

  const int nc = u0.getComponentBound(0)-u0.getComponentBase(0)+1;
  // create a new menu with options for choosing a component.
  aString *cmd = new aString[nc+1];
  aString *label = new aString[nc+1];
  for( int n=0; n<nc; n++ )
  {
    label[n]=u0.getName(n);
    cmd[n]="plot:"+u0.getName(n);

  }
  cmd[nc]="";
  label[nc]="";
    
  dialog.addOptionMenu("component:", cmd,label,0);
  delete [] cmd;
  delete [] label;


  // create an option menu for the sequences
  int ns=numberOfSequences[cfs];
  cmd = new aString[ns+2];
  label = new aString[ns+2];
  for( int n=0; n<ns; n++ )
  {
    label[n]=sequenceName[n];
    cmd[n]="plot sequence:"+sequenceName[n];

  }
  if( ns==0 )
  {
    label[ns]="none";
    cmd[ns]="plot sequence:"+label[ns];
    ns++;
  }
  
  cmd[ns]="";
  label[ns]="";
    
  dialog.addOptionMenu("sequence:", cmd,label,0);
  delete [] cmd;
  delete [] label;
  
  // push buttons:
  aString cmds[] = {"break",
		    "contour",
		    "grid",
		    "stream lines",
		    "displacement", 
		    "next",
		    "previous",
		    "erase",
		    "show movie",
                    "derived types",
                    "save ovText file",
                    "save plot3d file",
                    "forcing regions",
                    // "user defined output",
                    // "file output",
		    // "plot options...",
		    ""};
  numberOfPushButtons=0;  // number of entries in cmds
  while( cmds[numberOfPushButtons]!="" ) numberOfPushButtons++;
  
  int numRows=(numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  // *** toggle buttons ***
  aString tbCommands[10];
  int tbState[10];

  int nt=0;
  tbCommands[nt]="save movie files"; tbState[nt] = saveMovieFiles;  nt++;
  if( numberOfFrameSeries>1 )
  {
    tbCommands[nt]="apply commands to all frame series"; tbState[nt] = applyCommandsToAllSeries; nt++;
  }
  assert( nt<10 );
  tbCommands[nt]=""; 
  
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=7;
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  nt=0;
  textLabels[nt] = "solution:";  sPrintF(textStrings[nt],"1");  nt++; 
  textLabels[nt] = "movie frames:";  sPrintF(textStrings[nt],"%i",numberOfMovieFrames);  nt++; 
  textLabels[nt] = "stride:";  sPrintF(textStrings[nt],"%i",frameStride);  nt++; 
  textLabels[nt] = "movie file name:"; textStrings[nt]=movieFileName;  nt++; 


  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);
  numberOfTextBoxes=nt;
    
  aString infoLabel ="solution";
  for( int fs=0; fs<numberOfFrameSeries; fs++ )
  {
    infoLabel+=sPrintF(" %i (%i)",solutionNumber[fs],numberOfSolutions[fs]);
    if( fs<numberOfFrameSeries-1 ) infoLabel+=",";
  }
  dialog.addInfoLabel(infoLabel);

  return 0;

}

void ShowFilePlotter::
setSensitivity( GUIState & dialog, bool trueOrFalse )
{
  dialog.getOptionMenu(0).setSensitive(trueOrFalse);
  int n;
  for( n=1; n<numberOfPushButtons; n++ ) // leave first push button sensitive (=="break")
    dialog.setSensitive(trueOrFalse,DialogData::pushButtonWidget,n);
  
  for( n=0; n<numberOfTextBoxes; n++ )
    dialog.setSensitive(trueOrFalse,DialogData::textBoxWidget,n);
  
}

int ShowFilePlotter::
updatePlotStuffDialog(DialogData & dialog, realCompositeGridFunction & u0, const int component, const int cfs)
// =============================================================================================
// =============================================================================================
{
  const int nc = u0.getComponentBound(0)-u0.getComponentBase(0)+1;
  // create a new menu with options for choosing a component.
  aString *cmd = new aString[nc+1];
  aString *label = new aString[nc+1];
  for( int n=0; n<nc; n++ )
  {
    label[n]=u0.getName(n);
    cmd[n]="plot:"+u0.getName(n);

  }
  cmd[nc]="";
  label[nc]="";
    
  dialog.changeOptionMenu("component:",cmd,label,component);
  delete [] cmd;
  delete [] label;

  // update the option menu for the sequences
  int ns=numberOfSequences[cfs];
  cmd = new aString[ns+2];  // note +2
  label = new aString[ns+2];
  for( int n=0; n<ns; n++ )
  {
    label[n]=sequenceName[n];
    if( label[n] == "" ) label[n]=" ";  // we cannot allow an empty label or this will cause trouble
    cmd[n]="plot sequence:"+sequenceName[n];

  }
  if( ns==0 )
  {
    label[ns]="none";
    cmd[ns]="plot sequence:"+label[ns];
    ns++;
  }
  cmd[ns]="";
  label[ns]="";
    
  dialog.changeOptionMenu("sequence:", cmd,label,0);
  delete [] cmd;
  delete [] label;


  return 0;
}



static int 
buildMainMenu( aString *menu0,
               aString *&menu,
               RealGridCollectionFunction & u,
               aString *sequenceName,
               const int & numberOfSolutions,
               const int & numberOfComponents,
               const int & numberOfSequences,
               int & chooseAComponentMenuItem,
               int & chooseASolutionMenuItem,
               int & numberOfSolutionMenuItems,
               int & chooseASequenceMenuItem,
               int & numberOfSequenceMenuItems,
               const int & maxMenuSolutions,
	       const int & maximumNumberOfSolutionsInTheMenu,
	       int & solutionIncrement,
	       const int & maxMenuSequences,
	       const int & maximumNumberOfSequencesInTheMenu,
	       int & sequenceIncrement )
{
  // create the real menu by adding in the component names, these will appear as a 
  // cascaded menu
  char buff[120];

  chooseAComponentMenuItem=0;  // menu[chooseAComponentMenuItem]=">choose a component"
  chooseASolutionMenuItem=0;  
  numberOfSolutionMenuItems=0;
  chooseASequenceMenuItem=0;  
  numberOfSequenceMenuItems=0;

  int numberOfMenuItems0=0;
  while( menu0[numberOfMenuItems0]!="" )
  {
    numberOfMenuItems0++;
  }  
  numberOfMenuItems0++;

  // const int maxMenuSolutions=25;  // cascade solution menu if there are more than this many solutions
  // const int maximumNumberOfSolutionsInTheMenu=400;  // stride through the solutions if there are more
  // than this many solutions.
  // int solutionIncrement=1;                          // Here is the stride.

  // const int maxMenuSequences=25;  // cascade sequence menu if there are more than this many sequences
  // const int maximumNumberOfSequencesInTheMenu=400;  // stride through the sequences if there are more
  // than this many sequences.
  // int sequenceIncrement=1;                          // Here is the stride.

  delete [] menu;
  const int menuDim=(numberOfMenuItems0+numberOfComponents
                     +numberOfSolutions+numberOfSolutions/maxMenuSolutions +2
		     +numberOfSequences+numberOfSequences/maxMenuSequences +2);
  menu = new aString [menuDim];

  int i=-1;
  for( int i0=0; i0<numberOfMenuItems0 ; i0++ )
  {
    assert( i<menuDim-1 );
    
    menu[++i]=menu0[i0];    
    if( menu[i]==">choose a component" )
    {
      chooseAComponentMenuItem=i;
      for( int j=0; j<numberOfComponents; j++ )
      {
	menu[++i]=u.getName(u.getComponentBase(0)+j);
	if( menu[i] == "" || menu[i]==" " )
	  menu[i]=sPrintF(buff,"component%i",u.getComponentBase(0)+j);
      }
    }
    else if( menu[i]=="<>choose a solution" )
    {
      // make menu items that display all the solutions. If there many solutions then we cascade the solutions
      // into groups, each group has maxMenuSolutions entries
      chooseASolutionMenuItem=i;
      solutionIncrement=1;
      if( numberOfSolutions>maximumNumberOfSolutionsInTheMenu )
	solutionIncrement=(numberOfSolutions+maximumNumberOfSolutionsInTheMenu-1)/maximumNumberOfSolutionsInTheMenu;
	  
      int k=0;
      for( int j=0; j<numberOfSolutions; j+=solutionIncrement )
      {
	if( numberOfSolutions>maxMenuSolutions && ( k % maxMenuSolutions==0) )
	{
	  if( j==0 )
	    menu[++i]=sPrintF(buff,">solutions %i to %i",j,j+maxMenuSolutions*solutionIncrement-1);
	  else
	    menu[++i]=sPrintF(buff,"<>solutions %i to %i",j,
			      min(j+maxMenuSolutions*solutionIncrement-1,numberOfSolutions-1));
	}
	menu[++i]=sPrintF(buff,"solution%i",j);
	k++;
      }
      if( numberOfSolutions>maxMenuSolutions )
	menu[++i]="< ";
      numberOfSolutionMenuItems=i-chooseASolutionMenuItem;
    }
    else if( menu[i]==">sequence" )
    {
      // make menu items that display all the sequences. If there many sequences then we cascade them into groups.

      // printF("**buildMainMenu: numberOfSequences=%i\n",numberOfSequences);
      

      chooseASequenceMenuItem=i;
      sequenceIncrement=1;
      if( numberOfSequences>maximumNumberOfSequencesInTheMenu )
	sequenceIncrement=(numberOfSequences+maximumNumberOfSequencesInTheMenu-1)/maximumNumberOfSequencesInTheMenu;
	  
      int k=0;
      for( int j=0; j<numberOfSequences; j+=sequenceIncrement )
      {
	if( numberOfSequences>maxMenuSequences && ( k % maxMenuSequences==0) )
	{
	  if( j==0 )
	    menu[++i]=sPrintF(buff,">sequences %i to %i",j,j+maxMenuSequences*sequenceIncrement-1);
	  else
	    menu[++i]=sPrintF(buff,"<>sequences %i to %i",j,
			      min(j+maxMenuSequences*sequenceIncrement-1,numberOfSequences-1));
	}
  
        // printF("**buildMainMenu: j=%i sequenceName[j]=%s\n",j,(const char*)sequenceName[j]);

	menu[++i]=sequenceName[j];
	k++;
      }
      if( numberOfSequences>maxMenuSequences )
	menu[++i]="< ";
      numberOfSequenceMenuItems=i-chooseASequenceMenuItem;

      // printF("**buildMainMenu: maxMenuSequences=%i, numberOfSequenceMenuItems=%i\n",maxMenuSequences,numberOfSequenceMenuItems);
      

    }
  }
  return 0;
}

int ShowFilePlotter::
plotAll(DialogData & dialog)
// ===============================================================================================
// /Description:
//    Make calls to actually plot all active items
//
// ===============================================================================================
{
  // Change the top info label to reflect the current solution: 
  aString infoLabel ="solution";
  for( int fs=0; fs<numberOfFrameSeries; fs++ )
  {
    infoLabel+=sPrintF(" %i (%i)",solutionNumber[fs],numberOfSolutions[fs]);
    if( fs<numberOfFrameSeries-1 ) infoLabel+=",";
  }
  dialog.setInfoLabel(0,infoLabel);


  ps.erase();

  real spatialBound=1.;
  if( numberOfFrameSeries>1 ) 
  {
    // Get the global spatial scale (used for surface plots so that they match up across domains)
    RealArray xBound(2,3);
    spatialBound=0.;
    for(int fs=0; fs<numberOfFrameSeries; fs++ )
    {
      PlotIt::getGridBounds(cg[fs],psp[fs],xBound);
      spatialBound=max( spatialBound, max(xBound(End,Range(0,1))-xBound(Start,Range(0,1))) );
    }
    if( spatialBound==0. ) spatialBound=1.;
    // printf(" ---> spatialBound=%e\n",spatialBound);
  }


  aString subLabel="";
  for(int fs=0; fs<numberOfFrameSeries; fs++ )
  {
    CompositeGrid & cg0 = cg[fs];
    realCompositeGridFunction & u0 = u[fs];
    PlotStuffParameters & psp0 = psp[fs];
    DataBase & dbase0 = dbaseArray[fs];
    
    psp0.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    psp0.showFileSolutionNumber=solutionNumber[fs]; // used by userDefinedOutput (for e.g.)


    if( numberOfFrameSeries>1 ) 
    {
      if( subLabel != "" ) subLabel+= ", ";
      subLabel += showFileReader.getFrameSeriesName(fs) + ":";

      // printF("plotStuff: plot fs=%i plotOptions=%i solutionNumber=%i\n",fs,plotOptions[fs],solutionNumber[fs]);
      // printF("plotStuff: subLabel=%s\n",(const char*)subLabel);
      

      psp0.set(GI_LABEL_COMPONENT,false);   // do not label components on top title
      psp0.set(GI_LABEL_COLOUR_BAR,false);  // do not label the colour bar
      // psp0.set(GI_LABEL_MIN_MAX,(fs==0 ? 1 : 2));      // label max/min as a sub-title
      psp0.set(GI_LABEL_MIN_MAX,2);      // label max/min as a sub-title

      // set the global surface scale
      psp0.set(GI_CONTOUR_SURFACE_SPATIAL_BOUND,spatialBound);
      psp0.set(GI_TOP_LABEL_SUB_1,subLabel);
    }
    
    if( plotOptions[fs] & 16 )
      BodyForce::plotForcingRegions( ps, dbase0,cg0,psp0 );
    if( plotOptions[fs] & 1 )
      PlotIt::plot( ps, cg0, psp0 );
    if( plotOptions[fs] & 2 )
      PlotIt::contour( ps, u0, psp0 );
    if( plotOptions[fs] & 4 )
      PlotIt::streamLines( ps, u0, psp0 );
    if( plotOptions[fs] & 8 )
      PlotIt::displacement( ps, u0, psp0 );

    psp0.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
    psp0.get(GI_TOP_LABEL_SUB_1,subLabel);
  }
  return 0;
}


bool ShowFilePlotter::
setFrameSeriesTitles(int cfs)
// ==================================================================================
//  Set the plot titles when there are multiple frame series
//
// Return : true if the titles where found, false otherwise.
// ==================================================================================
{
  bool found=true;
	  
  ListOfShowFileParameters frameSeriesParameters;

  aString dirName; sPrintF(dirName,"FrameSeriesHeader%i",solutionNumber[cfs]); 

  found = showFileReader.getParameters(dirName,frameSeriesParameters);
  if( !found ) return found;

  aString title[3];  // save titles here
  
  int ivalue;
  real rvalue;
  aString stringValue;
  ShowFileParameter::ParameterType stringParameter = ShowFileParameter::stringParameter;
  std::list<ShowFileParameter>::iterator iter; 
  for(iter = frameSeriesParameters.begin(); iter!=frameSeriesParameters.end(); iter++ )
  {
    ShowFileParameter & sfp = *iter;
    if( sfp.getName().matches("title") && sfp.getType()==stringParameter )
    {
      aString pname;
      sfp.get(pname,stringParameter,ivalue,rvalue,stringValue);
      printF(" ShowFilePlotter::setFrameSeriesTitles: pname=[%s] stringValue=[%s]\n",
             (const char*)pname,(const char*)stringValue);
      
      if( pname=="title") 
        title[0]=stringValue;
      else if( pname=="title1") 
        title[1]=stringValue;
      else if( pname=="title2") 
        title[2]=stringValue;
    }
  }

//   psp[cfs].set(GI_TOP_LABEL,title[0]);  // set title
//   psp[cfs].set(GI_TOP_LABEL_SUB_1,title[1]);
//   psp[cfs].set(GI_TOP_LABEL_SUB_2,title[2]);
  for( int fs=0; fs<numberOfFrameSeries; fs++ )
  {
    psp[fs].set(GI_TOP_LABEL,title[0]);  // set title
    psp[fs].set(GI_TOP_LABEL_SUB_1,title[1]);
    psp[fs].set(GI_TOP_LABEL_SUB_2,title[2]);
  }
  
  return found;
}

int ShowFilePlotter::
setPlotTitles(int cfs, bool useFrameSeriesTitles /*= true*/ )
// ==================================================================================
//  Set the plot titles
//
//  \useFrameSeriesTitles : If useFrameSeriesTitles==true and there are solutions from multiple frame series being plotted then we
//  choose the titles for the overall set of frame series. If useFrameSeriesTitles==false then we choose titles ...
// 
// ==================================================================================
{

  bool titlesFromFrameSeries=false;
  if( useFrameSeriesTitles )
  {
    // count the number of frame series that have things plotted
    int numSeriesPlotted=0;
    for( int fs=0; fs<numberOfFrameSeries; fs++ )
    {
      if( plotOptions[fs]!=0 ) numSeriesPlotted++;
    }

    if( numSeriesPlotted>1 )
    {
      titlesFromFrameSeries = setFrameSeriesTitles(cfs);
    }
  }
//   for( int fs=0; fs<numberOfFrameSeries; fs++ )
//     printF("setPlotTitles: fs=%i, headerComment=%s\n",fs,(const char*)headerComment[fs][0]);
  
  if( !titlesFromFrameSeries )
  {
    // const aString *headerComment; 
    // headerComment=showFileReader.getHeaderComments(numberOfHeaderComments);
    // printF("setPlotTitles: cfs=%i, headerComment=%s\n",cfs,(const char*)headerComment[cfs][0]);
    psp[cfs].set(GI_TOP_LABEL,headerComment[cfs][0]);  // set title
    psp[cfs].set(GI_TOP_LABEL_SUB_1,headerComment[cfs][1]);  
    psp[cfs].set(GI_TOP_LABEL_SUB_2,headerComment[cfs][2]);  
  }
  return 0;
}

// ===============================================================================================
/// \brief: make a copy of the header comments for a given frame series
// 
// ===============================================================================================
int ShowFilePlotter::
getHeaderComments( int cfs )
{
  int numberOfHeaderComments=0;
  const aString *header;
  header=showFileReader.getHeaderComments(numberOfHeaderComments);    
  // delete headerComment[cfs];
  // headerComment[cfs] = new aString [numberOfHeaderComments];
  if( numberOfHeaderComments>maximumNumberOfHeaderComments )
  {
    printF("ShowFilePlotter::getHeaderComments:WARNING: There were some header comments that were not saved.\n");
  }
  for( int i=0; i<min(numberOfHeaderComments,maximumNumberOfHeaderComments); i++ )
  {
    headerComment[cfs][i]=header[i];
  }
  return 0;
}

int ShowFilePlotter::
plot()
// ===============================================================================================
// /Description:
//  Plot solutions and grids in a show file.
//
// /Return value: 
//             1=finished, 
//             0=not done, prompt for a new show file name.
// ===============================================================================================
{
  bool done=false;
  const int startingSolution = 1;
  bool showComputedGeometry=false;

  // keep plot options for each frame series
  numberOfFrameSeries=max(1,showFileReader.getNumberOfFrameSeries());

  int cfs=0; // current frame series
    
  delete [] cg;
  delete [] u;
  delete [] psp;
  cg = new CompositeGrid[numberOfFrameSeries];
  u = new realCompositeGridFunction[numberOfFrameSeries];
  psp = new PlotStuffParameters[numberOfFrameSeries];

  // CompositeGrid cg;  
  // set up a function for contour plotting:
  Range all;
  // realCompositeGridFunction u;

  bool plotNewFunction = false;
  bool plotNewComponent= false;
  bool movieMode=false;
  int movieFileNameOffset=0;  // offset the numbering of the movie file names
  int movieFileNameNumberingLength=3;  // keep this many digits in the number that appears in movie file names.
    
  delete [] component;
  delete [] solutionNumber;
  delete [] plotOptions;
  delete [] numberOfSolutions;
  delete [] numberOfFrames;
  delete [] numberOfComponents;
  delete [] numberOfComponents0;
  delete [] numberOfSequences;
  if( headerComment!=NULL )
  {
    for(int fs=0; fs<numberOfFrameSeries; fs++ )
      delete [] headerComment[fs];
    delete [] headerComment;
  }
    
  component = new int [numberOfFrameSeries];
  solutionNumber = new int [numberOfFrameSeries];
  plotOptions = new int [numberOfFrameSeries];

  numberOfSolutions = new int [numberOfFrameSeries];
  numberOfFrames = new int [numberOfFrameSeries];
  numberOfComponents = new int [numberOfFrameSeries];
  numberOfComponents0 = new int [numberOfFrameSeries];  // number of components with no derived functions
  numberOfSequences = new int [numberOfFrameSeries];
  headerComment = new aString* [numberOfFrameSeries];

  dbaseArray = new DataBase[numberOfFrameSeries];
  
  for( int fs=0; fs<numberOfFrameSeries; fs++ )
  {
    plotOptions[fs]=0;
    component[fs]=0;
    solutionNumber[fs]=startingSolution;
    psp[fs].showFileReader=&showFileReader;        // used by userDefinedOutput (for e.g.)
    psp[fs].showFileSolutionNumber=solutionNumber[fs]; // used by userDefinedOutput (for e.g.)
    headerComment[fs] = new aString [maximumNumberOfHeaderComments];
    for( int i=0; i<maximumNumberOfHeaderComments; i++ )
      headerComment[fs][i]=" ";
  }
  int minNumberOfSolutions=INT_MAX, maxNumberOfSolutions=0;
  for( int fs=0; fs<numberOfFrameSeries; fs++ )
  {
    showFileReader.setCurrentFrameSeries(fs);
    showFileReader.getASolution(solutionNumber[fs],cg[fs],u[fs]);
    getHeaderComments(fs);

    numberOfFrames[fs]=showFileReader.getNumberOfFrames();

    numberOfSolutions[fs] = max(1,numberOfFrames[fs]);
    minNumberOfSolutions=min(minNumberOfSolutions,numberOfSolutions[fs]);
    maxNumberOfSolutions=max(maxNumberOfSolutions,numberOfSolutions[fs]);

    numberOfComponents[fs]=u[fs].getComponentDimension(0);  // *wdh* 100330 changed cfs to fs
    numberOfComponents0[fs]=numberOfComponents[fs];
    numberOfSequences[fs]=showFileReader.getNumberOfSequences();

    // printF(" fs=%i numberOfSequences=%i\n",fs,numberOfSequences[fs]);
    
  }
  if( minNumberOfSolutions!=maxNumberOfSolutions )
  {
    printF("plotStuff::WARNING: Not all frame series have the same number of solutions,\n"
           "  I will use the minimum number found\n");
    for( int fs=0; fs<numberOfFrameSeries; fs++ )
    {
      numberOfSolutions[fs]=minNumberOfSolutions;
    }
  }
  

  showFileReader.setCurrentFrameSeries(cfs);

  numberOfMovieFrames=numberOfFrames[cfs]-1; // what should this be?
    

  if( numberOfSequences[cfs]>0 )
  {
    delete [] sequenceName;
    sequenceName = new aString[numberOfSequences[cfs]];
    showFileReader.getSequenceNames(sequenceName,numberOfSequences[cfs]);
  }


  // this next class knows how to form derived quantities such as vorticity, derivatives etc.
  // DerivedFunctions derivedFunctions(showFileReader);
  // DerivedFunctions derivedFunctions[numberOfFrameSeries];
  DerivedFunctions *derivedFunctions = new DerivedFunctions [numberOfFrameSeries];
  for( int fs=0; fs<numberOfFrameSeries; fs++ )
    derivedFunctions[fs].set(showFileReader);
  

  // ------------ pull down menu (old way) ------------------------------
  char buff[120];
  aString answer,answer2;
  aString menu0[]= { "!plotStuff",
		     "contour",
		     "stream lines",
		     "grid",
		     ">sequence",
		     "<next",
		     "previous",
		     ">choose a component", 
		     "<>choose a solution", 
		     "<next component",
		     "previous component", 
		     "derived types",
		     "movie",
		     "movie and save",
		     ">plot bounds",
		     "set plot bounds",
		     "use default plot bounds",
		     "<check mappings with grid",
		     "user defined output",
		     "erase",
		     "redraw",
		     "open a new file",
		     "file output",
		     "maximum number of open show files",
		     "movie file name offset",
		     "movie file numbering length",
		     "help",
		     "exit",
		     "" };
  aString help[]= { 
    "contour                    : plot contours (surfaces)",
    "stream lines               : draw stream lines",
    "grid                       : plot the grid",
    "sequence                   : plot a sequence that has been saved in the show file",
    "next                       : plot the next solution of all items on the screen",
    "previous                   : plot the previous solution of all items on the screen" ,
    "choose a component         : plot a different component of all items on the screen", 
    "choose a solution          : plot a different solution of all items on the screen", 
    "next component             : plot the next component of all items on the screen",
    "previous component         : plot the previous component of all items on the screen", 
    "derived types              : define new quantities such as vorticity, derivatives etc.",
    "movie                      : plot the next `n' solutions",
    "movie and save             : plot the next `n' solutions and save each as a postscript file",
    "set plot bounds            : specify fixed bounds for plotting. Useful for movies.",
    "use default plot bounds    : let plotStuff determine the plotting bounds",
    "check mappings with grid   : call the checkMapping routine",
    "user defined output        : call the function userDefinedOutput",
    "erase                      : erase everything",
    "redraw                     : force a redraw (useful to call from command files)",
    "open a new file            : open a new show file to read",
    "maximum number of open show files : default is 25, choose a smaller value to use less memory",
    "movie file name offset     : offset for the numbering of the movie file names",
    "movie file numbering length: number of digits in the numbering of moving files.",
    "help",
    "exit",
    "" };

    
  // create the real menu by adding in the component names, these will appear as a 
  // cascaded menu

  int chooseAComponentMenuItem;  // menu[chooseAComponentMenuItem]=">choose a component"
  int chooseASolutionMenuItem;  
  int numberOfSolutionMenuItems=0;
  int chooseASequenceMenuItem;  
  int numberOfSequenceMenuItems=0;
  const int maxMenuSolutions=25;  // cascade solution menu if there are more than this many solutions
  const int maximumNumberOfSolutionsInTheMenu=400;  // stride through the solutions if there are more
                                                    // than this many solutions.
  int solutionIncrement=1;                          // Here is the stride.

  const int maxMenuSequences=25;  // cascade sequence menu if there are more than this many sequences
  const int maximumNumberOfSequencesInTheMenu=400;  // stride through the sequences if there are more
  // than this many sequences.
  int sequenceIncrement=1;                          // Here is the stride.

  aString *menu=NULL;
  buildMainMenu( menu0,
		 menu,
		 u[cfs],
		 sequenceName,
		 numberOfSolutions[cfs],
		 numberOfComponents[cfs],
		 numberOfSequences[cfs],
		 chooseAComponentMenuItem,
		 chooseASolutionMenuItem,
		 numberOfSolutionMenuItems,
		 chooseASequenceMenuItem,
		 numberOfSequenceMenuItems,
		 maxMenuSolutions,
		 maximumNumberOfSolutionsInTheMenu,
		 solutionIncrement,
		 maxMenuSequences,
		 maximumNumberOfSequencesInTheMenu,
		 sequenceIncrement );



  GUIState dialog;
  buildPlotStuffDialog(dialog,u[cfs],cfs);
  dialog.buildPopup(menu);
  ps.pushGUI(dialog);
  for( int fs=0; fs<numberOfFrameSeries; fs++ )
  {
    psp[fs].set(GI_TOP_LABEL,headerComment[fs][0]);  // set title
    psp[fs].set(GI_TOP_LABEL_SUB_1,headerComment[fs][1]);  
    psp[fs].set(GI_TOP_LABEL_SUB_2,headerComment[fs][2]);  
    if( cg[fs].numberOfDimensions()==1 )
      psp[fs].set(GI_COLOUR_LINE_CONTOURS,true);

    // set default prompt
  }
  ps.appendToTheDefaultPrompt("plotStuff>");
    

  int len=0;
  int menuItem=-1;
  bool replot=false;
  int numberOfItemsPlotted=0;
  for( int it=0; ; it++)
  {
    checkArrays("plotStuff: in for(;;)");
    #ifdef USE_PPP
    Overture::printMemoryUsage("ShowFilePlotter:start of main loop");
    #endif

    if( it==0 && numberOfFrames[cfs]<=0 )
      answer="grid";
    else
    {
      menuItem = ps.getAnswer(answer,"");
      // menuItem=ps.getMenuItem(menu,answer);
    }
      
	
    if( answer=="grid" )
    {
      psp[cfs].showFileSolutionNumber=solutionNumber[cfs]; // used by userDefinedOutput (for e.g.)

      setPlotTitles(cfs,false);
      ps.erase();
      PlotIt::plot(ps, cg[cfs], psp[cfs]);   // plot the composite grid

      if( psp[cfs].getObjectWasPlotted() ) 
      {
	plotOptions[cfs] |= 1;
        numberOfItemsPlotted++;
      }
      
      if( showComputedGeometry )
      {
	for( int grid=0; grid<cg[cfs].numberOfComponentGrids(); grid++ )
	  cg[cfs][grid].displayComputedGeometry();
      }

      replot=true;
    }
    else if( answer=="contour" )
    {
      psp[cfs].showFileSolutionNumber=solutionNumber[cfs]; // used by userDefinedOutput (for e.g.)

      setPlotTitles(cfs,false);
      ps.erase();
      PlotIt::contour(ps, u[cfs], psp[cfs]);  // contour/surface plots

      // update the component in the option menu 
      int componentToPlot=0;
      psp[cfs].get(GI_COMPONENT_FOR_CONTOURS,componentToPlot);
      dialog.getOptionMenu("component:").setCurrentChoice(componentToPlot);


      if( psp[cfs].getObjectWasPlotted() & 1 ) 
      {
	plotOptions[cfs] |= 2;
	numberOfItemsPlotted++;
      }
      
      if( psp[cfs].getObjectWasPlotted() & 2 )  // grid was also plotted
      {
	plotOptions[cfs] |= 1;
	numberOfItemsPlotted++;
      }

      replot=true;
      
    }
    else if( answer=="stream lines" )
    {
      psp[cfs].showFileSolutionNumber=solutionNumber[cfs]; // used by userDefinedOutput (for e.g.)

      setPlotTitles(cfs,false);
      ps.erase();
      PlotIt::streamLines(ps, u[cfs], psp[cfs]);  // streamlines

      if( psp[cfs].getObjectWasPlotted() ) 
      {
	plotOptions[cfs] |= 4;
	numberOfItemsPlotted++;
      }

      replot=true;
    }
    else if( answer=="displacement" )
    {
      // The displacement is plotted for elasticity problems
      psp[cfs].showFileSolutionNumber=solutionNumber[cfs]; // used by userDefinedOutput (for e.g.)

      setPlotTitles(cfs,false);
      ps.erase();
      PlotIt::displacement(ps, u[cfs], psp[cfs]);  

      if( psp[cfs].getObjectWasPlotted() ) 
      {
	plotOptions[cfs] |= 8;
	numberOfItemsPlotted++;
      }
      replot=true;
    }
    else if( answer=="forcing regions" )
    {
       // change options for plotting body/boundary forcing regions

      DataBase & dbase = dbaseArray[cfs];  // data-base for the current frame series

      if( !dbase.has_key("numberOfBodyForceRegions") )
      {
        // look for body/boundary forcings

        int solutionForForcingRegions=1;  // regions are in frame 1 for now
	HDF_DataBase *pdb = showFileReader.getFrame(solutionForForcingRegions);
        assert( pdb!=NULL );
	HDF_DataBase & db  = *pdb;
	if( !dbase.has_key("numberOfBodyForceRegions") ) dbase.put<int>("numberOfBodyForceRegions",0);
	if( !dbase.has_key("numberOfBoundaryForceRegions") ) dbase.put<int>("numberOfBoundaryForceRegions",0);
	int & numberOfBodyForceRegions = dbase.get<int>("numberOfBodyForceRegions");
	int & numberOfBoundaryForceRegions = dbase.get<int>("numberOfBoundaryForceRegions");
	
	db.get(numberOfBodyForceRegions,"numberOfBodyForceRegions");
	db.get(numberOfBoundaryForceRegions,"numberOfBoundaryForceRegions");

	printF("ShowFileReader:INFO: numberOfBodyForceRegions=%i, numberOfBoundaryForceRegions=%i\n",
               numberOfBodyForceRegions,numberOfBoundaryForceRegions);
	

	if( !dbase.has_key("turnOnBodyForcing") ) dbase.put<bool>("turnOnBodyForcing",false);
        bool & turnOnBodyForcing = dbase.get<bool>("turnOnBodyForcing"); 

	if( !dbase.has_key("turnOnBoundaryForcing") ) dbase.put<bool>("turnOnBoundaryForcing",false);
        bool & turnOnBoundaryForcing = dbase.get<bool>("turnOnBoundaryForcing"); 

	if( numberOfBodyForceRegions>0 )
	{
	  // -- Get the the array of body forcings ---
	  turnOnBodyForcing=true;
	  if( !dbase.has_key("bodyForcings") ) dbase.put<std::vector<BodyForce*> >("bodyForcings");
	  std::vector<BodyForce*> & bodyForcings =  dbase.get<std::vector<BodyForce*> >("bodyForcings");

	  for( int bf=0; bf<numberOfBodyForceRegions; bf++ )
	  {
	    if( bf >= bodyForcings.size() )
	    {
	      BodyForce *pbf = new BodyForce;
	      bodyForcings.push_back(pbf);
	    }
	    
	    BodyForce & bodyForce = *bodyForcings[bf];
	    bodyForce.get(db,sPrintF("BodyForce%i",bf));
	  }
	}
	if( numberOfBoundaryForceRegions>0 )
	{
	  // -- Get the the array of boundary forcings ---
	  turnOnBoundaryForcing=true;
	  if( !dbase.has_key("boundaryForcings") ) dbase.put<std::vector<BodyForce*> >("boundaryForcings");
	  std::vector<BodyForce*> & boundaryForcings =  dbase.get<std::vector<BodyForce*> >("boundaryForcings");

	  for( int bf=0; bf<numberOfBoundaryForceRegions; bf++ )
	  {
	    if( bf >= boundaryForcings.size() )
	    {
	      BodyForce *pbf = new BodyForce;
	      boundaryForcings.push_back(pbf);
	    }
	    BodyForce & boundaryForce = *boundaryForcings[bf];
	    boundaryForce.get(db,sPrintF("BoundaryForce%i",bf));
	  }
	}

	
      }
      if( dbase.get<int>("numberOfBodyForceRegions")==0 && 
          dbase.get<int>("numberOfBoundaryForceRegions")==0 )
      {
	printF("ShowFilePlotter:INFO: there are no `forcing regions' to be plotted.\n");
	continue;
      }
	  

      psp[cfs].showFileSolutionNumber=solutionNumber[cfs]; // used by userDefinedOutput (for e.g.)

      setPlotTitles(cfs,false);
      ps.erase();
      // Plot body/boundary forcing regions and immersed boundaries. 
      BodyForce::plotForcingRegions(ps, dbase,cg[cfs], psp[cfs]);
      if( psp[cfs].getObjectWasPlotted() ) 
      {
	plotOptions[cfs] |= 16;
	numberOfItemsPlotted++;
      }

      replot=true;

    }

    else if( answer=="derived types" )
    {
      if( numberOfComponents[cfs]>0 )
      {
	aString *componentNames = new aString [numberOfComponents[cfs]];
	for( int n=0; n<numberOfComponents[cfs]; n++ )
	  componentNames[n]=u[cfs].getName(n);
	  
	derivedFunctions[cfs].update(ps,numberOfComponents[cfs],componentNames, &psp[cfs]);
	delete [] componentNames;

	derivedFunctions[cfs].getASolution(solutionNumber[cfs],cg[cfs],u[cfs]);
        getHeaderComments(cfs);

	numberOfComponents[cfs]=numberOfComponents0[cfs]+derivedFunctions[cfs].numberOfDerivedTypes();
	buildMainMenu( menu0,
		       menu,
		       u[cfs],
		       sequenceName,
		       numberOfSolutions[cfs],
		       numberOfComponents[cfs], 
		       numberOfSequences[cfs],
		       chooseAComponentMenuItem,
		       chooseASolutionMenuItem,
		       numberOfSolutionMenuItems,
		       chooseASequenceMenuItem,
		       numberOfSequenceMenuItems,
		       maxMenuSolutions,
		       maximumNumberOfSolutionsInTheMenu,
		       solutionIncrement,
		       maxMenuSequences,
		       maximumNumberOfSequencesInTheMenu,
		       sequenceIncrement );


	// update dialog menus for the new components
	updatePlotStuffDialog(dialog,u[cfs],component[cfs],cfs);

      }
      else
      {
	printF("ERROR: no components are available\n");
      }
    }
    else if( (menuItem > chooseASequenceMenuItem && menuItem <= chooseASequenceMenuItem+numberOfSequenceMenuItems) ||
             answer.matches("plot sequence:") )
    {
      // plot a sequence
      int sequenceNumber=-1;
      if( len=answer.matches("plot sequence:") )
      {
	// new way 
        aString sname = answer(len,answer.length()-1);
	for( int n=0; n<numberOfSequences[cfs]; n++ )
	{
	  if( sname == sequenceName[n] )
	  {
            sequenceNumber=n;
	    break;
	  }
	}
	if( sequenceNumber==-1 )
	{
	  printF("ShowFilePlotter::ERROR: unable to find sequence: [%s]\n",(const char*)sname);
          printF("Available sequence names are:\n");
	  for( int n=0; n<numberOfSequences[cfs]; n++ )
	  {
	    printF("  [%s]\n",(const char*)sequenceName[n]);
	  }
          continue;
	}

      }
      else
      {
	// old way
	sequenceNumber=menuItem-chooseASequenceMenuItem;
	if( numberOfSequences[cfs]>maxMenuSequences )
	{
	  // adjust the sequence number when there are many sequences since we add in extra menu items
	  // into the list.
	  int extra = 1+ sequenceNumber/maxMenuSequences;  
	  extra=1+ (sequenceNumber-extra)/maxMenuSequences;
	  sequenceNumber-=extra;
	  // printF("menuItem-chooseASequenceMenuItem=%i, extra=%i sequenceNumber=%i\n",
	  //     menuItem-chooseASequenceMenuItem,extra,sequenceNumber);
	}
	sequenceNumber=(sequenceNumber-1)*sequenceIncrement;
      }
      
      assert( sequenceNumber>=0 && sequenceNumber<numberOfSequences[cfs] );
      aString name;
      RealArray time,value;
      const int maxComponentName1=25, maxComponentName2=1;
      aString componentName1[maxComponentName1], componentName2[maxComponentName2];
	  
      showFileReader.getSequence(sequenceNumber,name,time,value,
				 componentName1,maxComponentName1,
				 componentName2,maxComponentName2);
      if( false )
      {
	printF("sequence %i: name=%s\n",sequenceNumber,(const char*)name);
	display(time,"time");
	display(value,"value");
      }
      
	  
      ps.erase();
      psp[cfs].set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      psp[cfs].set(GI_TOP_LABEL_SUB_1,"");
      psp[cfs].set(GI_TOP_LABEL_SUB_2,"");
      Range all; 
      #ifdef USE_PPP
        printF("INFO: cannot yet plot sequences in parallel -- fix me Bill!\n");
      #else
        PlotIt::plot(ps, time, value(all,all,value.getBase(2)), name, "t", componentName1, psp[cfs]);
      #endif
      // psp[cfs].set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      ps.erase();

    }
    else if( answer=="next" )
    {
      if( applyCommandsToAllSeries )
      {
	for( int fs=0; fs<numberOfFrameSeries; fs++ )
	  solutionNumber[fs] = ((solutionNumber[fs]+frameStride-1) % numberOfSolutions[fs]) +1;
      }
      else
      {
	solutionNumber[cfs] = ((solutionNumber[cfs]+frameStride-1) % numberOfSolutions[cfs]) +1;
      }
      
      plotNewFunction=true;
    }
    else if( answer=="previous" )
    {
      if( applyCommandsToAllSeries )
      {
	for( int fs=0; fs<numberOfFrameSeries; fs++ )
	  solutionNumber[fs] = ((solutionNumber[fs]-frameStride-1+numberOfSolutions[fs]) % numberOfSolutions[fs]) +1;
      }
      else
      {
	solutionNumber[cfs] = ((solutionNumber[cfs]-frameStride-1+numberOfSolutions[cfs]) % numberOfSolutions[cfs]) +1;
      }
      
      plotNewFunction=true;
    }
    else if( answer=="next component" )
    {
      component[cfs]= (component[cfs]+1) % numberOfComponents[cfs];
      plotNewComponent=true;
    }
    else if( answer=="previous component" )
    {
      component[cfs]= (component[cfs]-1+numberOfComponents[cfs]) % numberOfComponents[cfs];
      plotNewComponent=true;
    }
    else if( menuItem > chooseAComponentMenuItem && menuItem <= chooseAComponentMenuItem+numberOfComponents[cfs] )
    {
      component[cfs]=menuItem-chooseAComponentMenuItem-1 + u[cfs].getComponentBase(0);
      plotNewComponent=true;
      // cout << "chose component number=" << component << endl;
    }
    else if( answer=="choose a component" )
    { // *** not used *** Make a menu with the component names. If there are no names then use the component numbers
      aString *menu2 = new aString[numberOfComponents[cfs]+1];
      for( int i=0; i<numberOfComponents[cfs]; i++ )
      {
	menu2[i]=u[cfs].getName(u[cfs].getComponentBase(0)+i);
	if( menu2[i] == "" || menu2[i]==" " )
	  menu2[i]=sPrintF(buff,"component%i",u[cfs].getComponentBase(0)+i);
      }
      menu2[numberOfComponents[cfs]]="";   // null string terminates the menu
      component[cfs] = ps.getMenuItem(menu2,answer2);
      component[cfs]+=u[cfs].getComponentBase(0);
      delete [] menu2;
      plotNewComponent=true;
    }
    else if( len=answer.matches("plot:") )
    {
      // plot a new component
      aString name = answer(len,answer.length()-1);
      component[cfs]=-1;
      for( int n=0; n<numberOfComponents[cfs]; n++ )
      {
	if( name==u[cfs].getName(n+u[cfs].getComponentBase(0)) )
	{
	  component[cfs]=n;
	  break;
	}
      }
      if( component[cfs]==-1 )
      {
	printF("ERROR: unknown component name =[%s]\n",(const char*)name);
	component[cfs]=0;
      }
      component[cfs]+=u[cfs].getComponentBase(0);
      printF("plot new component number %i\n",component[cfs]);
      dialog.getOptionMenu("component:").setCurrentChoice(component[cfs]);
      plotNewComponent=true;
    }
    else if( len=answer.matches("solution:") )
    { // new way from text box
      int newSolution=solutionNumber[cfs];
      sScanF(answer(len,answer.length()-1),"%i",&newSolution);
      if( applyCommandsToAllSeries )
      {
	for( int fs=0; fs<numberOfFrameSeries; fs++ )
          solutionNumber[fs]=max(1,min(newSolution,numberOfSolutions[fs]));
      }
      else
      {
	solutionNumber[cfs]=max(1,min(newSolution,numberOfSolutions[cfs]));
      }
      dialog.setTextLabel("solution:",sPrintF("%i",newSolution));
      
      plotNewFunction=true;
      printF("Choosing solution %i\n",solutionNumber[cfs]);
    }
    else if( len=answer.matches("solution") )
    {
      sScanF(answer(len,answer.length()-1),"%i",&solutionNumber[cfs]);
      solutionNumber[cfs]=max(0,min(solutionNumber[cfs],numberOfSolutions[cfs]-1));
      plotNewFunction=true;
      solutionNumber[cfs]+=1;
      printF("Choosing solution %i\n",solutionNumber[cfs]);
    }
    else if( dialog.getToggleValue(answer,"apply commands to all frame series",applyCommandsToAllSeries) ){}//
    else if( dialog.getToggleValue(answer,"save movie files",saveMovieFiles) ){}//
    else if( dialog.getTextValue(answer,"movie frames:","%i",numberOfMovieFrames) ){}//
    else if( dialog.getTextValue(answer,"stride:","%i",frameStride) ){}//
    else if( dialog.getTextValue(answer,"movie file name:","%s",movieFileName) )
    {
      // remove leading blanks from the movie file name
      int len = movieFileName.length();
      int i=0;
      while( i<len && movieFileName[i] == ' ' ){i++; }  //
      movieFileName=movieFileName(i,len-1);
    }//

//       else if( menuItem > chooseASolutionMenuItem && menuItem <= chooseASolutionMenuItem+numberOfSolutionMenuItems )
//       {
// 	solutionNumber=menuItem-chooseASolutionMenuItem;
//         if( numberOfSolutions>maxMenuSolutions )
// 	{
//           // adjust the solution number when there are many solutions since we add in extra menu items
//           // into the list.
//           int extra = 1+ solutionNumber/maxMenuSolutions;  
//           extra=1+ (solutionNumber-extra)/maxMenuSolutions;
//           solutionNumber-=extra;
//           // printF("menuItem-chooseASolutionMenuItem=%i, extra=%i solutionNumber=%i\n",
//           //     menuItem-chooseASolutionMenuItem,extra,solutionNumber);
// 	}
//         solutionNumber=(solutionNumber-1)*solutionIncrement+1;
	
// 	plotNewFunction=true;
//       }
    else if( answer=="choose a solution" )
    { // ***** not used ****  Make a menu with the solution Names
      aString *menu2 = new aString[numberOfFrames[cfs]+1];
      for( int i=0; i<numberOfFrames[cfs]; i++ )
	menu2[i]=sPrintF(buff,"solution%i",i);
      menu2[numberOfFrames[cfs]]="";   // null string terminates the menu
      solutionNumber[cfs] = ps.getMenuItem(menu2,answer2)+1;
      delete [] menu2;
      plotNewFunction=true;
    }
    else if( answer=="show movie" )
    {
      movieMode=true;
    }
    else if( answer=="movie" || answer=="movie and save" )
    {
      movieMode=true;
      // numberOfMovieFrames=numberOfFrames[cfs];
      ps.inputString(answer2,sPrintF(buff,"Enter the number of frames (total=%i) and stride",numberOfFrames[cfs]));
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%i %i",&numberOfMovieFrames,&frameStride);
	printF("number of frames = %i (stride=%i)\n",numberOfMovieFrames,frameStride);
      }
      if( answer=="movie and save" )
      {
	ps.inputString(answer2,"Enter basic name for the ppm files (default=plot)");
	if( answer2 !="" && answer2!=" ")
	  movieFileName=answer2;
	else
	  movieFileName="movie";

	//We want the frame numbers to be padded out with zeros so that they ppear in the correct order
	//in "ls" for example, so that when converting the ppm's to a movie, the frames enter in the correct order. - pmb
	int padWidth = max(movieFileNameNumberingLength, 1 + (int)floor(log10((float)numberOfMovieFrames)));
	char buff2[120];
	sprintf(buff2, "pictures will be named %%s%%0%ii.ppm, %%s%%0%ii.ppm, ...", padWidth, padWidth);
	
	int movieFrame = solutionNumber[cfs]-1 + movieFileNameOffset;
	ps.outputString(sPrintF(buff, buff2, (const char*)movieFileName,movieFrame,(const char*)movieFileName,movieFrame+1));

	// int movieFrame = solutionNumber[cfs]-1 + movieFileNameOffset;
	// ps.outputString(sPrintF(buff,"pictures will be named %s%i.ppm, %s%i.ppm, ...",
	// 			(const char*)movieFileName,movieFrame,(const char*)movieFileName,movieFrame+1));
      }
	
    }
    else if( answer=="maximum number of open show files" )
    {
      int maxNum=25;
      ps.inputString(answer2,sPrintF("Enter the maximum number of open show files (default=%i)",maxNum));
      sScanF(answer2,"%i ",&maxNum);
      showFileReader.setMaximumNumberOfOpenShowFiles(maxNum);
    }
    else if( answer=="movie file name offset" )
    {
      ps.inputString(answer2,sPrintF("Enter the offset for naming the movie files (current=%i)",
				     movieFileNameOffset));
      sScanF(answer2,"%i ",&movieFileNameOffset);
    }
    else if( answer=="movie file numbering length" )
    {
      ps.inputString(answer2,sPrintF("Number of digits in the movie file name numbering (current=%i)",
				     movieFileNameNumberingLength));
      sScanF(answer2,"%i ",&movieFileNameNumberingLength);
    }
    else if( len=answer.matches("frame series:") )
    {
      aString name=answer(len,answer.length()-1);
      int frameSeries=-1;
      for( int fs=0; fs<numberOfFrameSeries; fs++ )
      {
	if( name == showFileReader.getFrameSeriesName(fs) )
	{
	  frameSeries=fs;
	  break;
	}
      }
      if(frameSeries>=0 )
      {
	showFileReader.setCurrentFrameSeries(frameSeries);
	printF("plotStuff: setting frame series = %i (name=%s)\n",frameSeries,
	       (const char*)showFileReader.getFrameSeriesName(frameSeries));
      }
      else
      {
	printF("plotStuff:ERROR: unknown frame series with name=[%s]\n",(const char*)name);
	ps.stopReadingCommandFile();
      }
      dialog.getOptionMenu("frame series:").setCurrentChoice(frameSeries);

      cfs=frameSeries;

      // int ok = showFileReader.setCurrentFrameSeries(name);

      // changeOptionMenu(int nOption, const aString opCommands[], const aString opLabels[], int initCommand);

      // 1111111111111111111111111111111111111111111111111111111111111111111111
      // we need to rebuild the menus

      if( numberOfSequences[cfs]>0 )
      {
	delete [] sequenceName;
	sequenceName = new aString[numberOfSequences[cfs]];
	showFileReader.getSequenceNames(sequenceName,numberOfSequences[cfs]);
	// printF("Get new seq. names:\n");
	// for( int s=0; s<numberOfSequences[cfs]; s++)
	// {
        //   printF(" s=%i name=[%s]\n",s,(const char*)sequenceName[s]);
	// }
	
      }

      // numberOfMovieFrames=numberOfFrames[cfs];
      // solutionNumber[cfs]=startingSolution;  // we should read the first solution so we find the grid
      psp[cfs].showFileSolutionNumber=solutionNumber[cfs];
      // component[cfs] = 0;
      // u.destroy();
      // cg.setNumberOfGrids(0); 
      // showFileReader.getAGrid(cg[cfs],startingSolution); 
      // showFileReader.getASolution(solutionNumber[cfs],cg[cfs],u[cfs]);
      numberOfComponents[cfs]=u[cfs].getComponentDimension(0);

      buildMainMenu( menu0,
		     menu,
		     u[cfs],
		     sequenceName,
		     numberOfSolutions[cfs],
		     numberOfComponents[cfs],
		     numberOfSequences[cfs],
		     chooseAComponentMenuItem,
		     chooseASolutionMenuItem,
		     numberOfSolutionMenuItems,
		     chooseASequenceMenuItem,
		     numberOfSequenceMenuItems,
		     maxMenuSolutions,
		     maximumNumberOfSolutionsInTheMenu,
		     solutionIncrement,
		     maxMenuSequences,
		     maximumNumberOfSequencesInTheMenu,
		     sequenceIncrement );
      // 1111111111111111111111111111111111111111111111111111111111111111111111

      updatePlotStuffDialog(dialog,u[cfs],component[cfs],cfs);
      plotNewFunction=true;
    }
    else if( answer=="set plot bounds" )
    {
      RealArray xBound(2,3);
      xBound=0.;
      xBound(1,Range(0,2))=1.;
      if( cg[cfs].numberOfDimensions()==2 )
	ps.inputString(answer2,sPrintF(buff,"Enter bounds xa,xb, ya,yb "));
      else
	ps.inputString(answer2,sPrintF(buff,"Enter bounds xa,xb, ya,yb, za,zb "));
      if( answer2!="" )
	sScanF(answer2,"%e %e %e %e %e %e",&xBound(0,0),&xBound(1,0),&xBound(0,1),&xBound(1,1),
	       &xBound(0,2),&xBound(1,2));
	
      ps.resetGlobalBound(ps.getCurrentWindow());
      ps.setGlobalBound(xBound);
	
      psp[cfs].set(GI_PLOT_BOUNDS,xBound); // set plot bounds
      psp[cfs].set(GI_USE_PLOT_BOUNDS,true);  // use the region defined by the plot bounds
    }
    else if( answer=="use default plot bounds" )
    {
      psp[cfs].set(GI_USE_PLOT_BOUNDS,false);  // use the region defined by the plot bounds
    }
    else if( answer=="check mappings with grid" )
    {
      for( int grid=0; grid<cg[cfs].numberOfComponentGrids(); grid++ )
      {
	if( cg[cfs][grid].mapping().mapPointer==NULL )
	{
	  cout << "ERROR: This grid has no mappings! \n";
	  break;
	}
	cg[cfs][grid].mapping().checkMapping();
      }
    }
    else if( answer=="user defined output" )
    {
      psp[cfs].showFileSolutionNumber=solutionNumber[cfs]; // used by userDefinedOutput (for e.g.)
      PlotIt::userDefinedOutput(u[cfs],psp[cfs],"plotStuff");
    }
    else if( answer=="erase" )
    {
      ps.erase();
      if( applyCommandsToAllSeries )
      {
	for( int fs=0; fs<numberOfFrameSeries; fs++ )
          plotOptions[fs]=0;

        numberOfItemsPlotted=0;
      }
      else
      {
        plotOptions[cfs]=0;
        numberOfItemsPlotted=0;
	for( int fs=0; fs<numberOfFrameSeries; fs++ )
	{
	  if( plotOptions[fs] != 0 )
	  {
	    numberOfItemsPlotted++;
	  }
	}
      }
      
    }
    else if( answer=="redraw" )
    { // force a redraw -- add to command files to force the drawing of the screen
      ps.redraw(true);
    }
    else if( answer=="open a new file" )
    {
      // nameOfShowFile=""; // do this so we prompt for a new name
      done=false;
      break;
    }
    else if( answer=="file output" )
    {
      fileOutput(ps, u[cfs]);
    }
    else if( answer=="exit" )
    {
      done=true;
      break;
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
	ps.outputString(help[i]);
    }
    else if( answer=="plot" )
    {
      plotNewFunction=true;
    }
    else if( answer=="save ovText file" )
    {
      // save the solution in a ovText file. 

      saveOvertureTextFile( u[cfs],ps,&showFileReader );
    }
    else if( answer=="save plot3d file" )
    {
      // save the current solution in a plot3d file
    
      // fix me: 
      real machNumber=1., alpha=0., reynoldsNumber=0., t=0., gamma=1.4, Rg=1.;
      RealArray par(10);
      par(0)=machNumber;
      par(1)=alpha;
      par(2)=reynoldsNumber;
      par(3)=t;
      par(4)=gamma;
      par(5)=Rg;

      DataFormats::writePlot3d(u[cfs],par); 

    }
    else
    {
      printF("unknown response, answer=[%s]\n",(const char*)answer);
      ps.stopReadingCommandFile();
    }


    if( movieMode )
    { // ************** Movie Mode *******************

      printF("saveMovieFiles=%i, answer=%s\n",saveMovieFiles,(const char*)answer);
      

      setSensitivity( dialog, false);
	
      //In order for convert and similar programs to convert the output
      //files into a movie, we need to pad the frame number by zeros
      
      int padWidth = max(movieFileNameNumberingLength, 1 + (int)floor(log10((float)numberOfMovieFrames)));
      char movieMessageFormat[120], movieFileNameFormat[120];
      sprintf(movieMessageFormat, "Saving file %%s%%0%ii.ppm", padWidth);
      sprintf(movieFileNameFormat, "%%s%%0%ii.ppm", padWidth);

      for( int frame=1; frame<=numberOfMovieFrames; frame+=frameStride )
      {
	// movie mode ** check here if the user has hit break ***
	if( ps.isGraphicsWindowOpen() && !ps.readingFromCommandFile() )  
	{
	  // ps.outputString(sPrintF(buff,"Check for break at t=%e\n",t));
	  answer="";
	  int menuItem = ps.getAnswerNoBlock(answer,"monitor>");
	  if( answer=="break" )
	  {
	    // programHalted=true;
	    break;
	  }
	}

	if( saveMovieFiles || answer=="movie and save" )
	{ // save a ppm file
	  psp[cfs].set(GI_HARD_COPY_TYPE,GraphicsParameters::ppm);
	  // int movieFame = frame-1;
	  int movieFrame = (solutionNumber[cfs]-1)/frameStride + movieFileNameOffset;

	  //ps.outputString(sPrintF(buff,"Saving file %s%i.ppm",(const char*)movieFileName,movieFrame));
	  //ps.hardCopy(    sPrintF(buff,            "%s%i.ppm",(const char*)movieFileName,movieFrame),psp[cfs]);
	  ps.outputString(sPrintF(buff,movieMessageFormat,(const char*)movieFileName,movieFrame));
	  ps.hardCopy(    sPrintF(buff,movieFileNameFormat,(const char*)movieFileName,movieFrame),psp[cfs]);
	  psp[cfs].set(GI_HARD_COPY_TYPE,GraphicsParameters::postScript);
	}


	// showFileReader.getASolution(solutionNumber[cfs],cg[cfs],u[cfs]);
	// derivedFunctions.getASolution(solutionNumber[cfs],cg[cfs],u[cfs]);
	for( int fs=0; fs<numberOfFrameSeries; fs++ )
	{
	  solutionNumber[fs] = (solutionNumber[fs]+numberOfSolutions[fs]+ frameStride-1) % numberOfSolutions[fs] +1;
	  if( plotOptions[fs]!=0 )
	  {
	    showFileReader.setCurrentFrameSeries(fs);
	    derivedFunctions[fs].getASolution(solutionNumber[fs],cg[fs],u[fs]);
            getHeaderComments(fs);
	  }
	}
	showFileReader.setCurrentFrameSeries(cfs);

        setPlotTitles(cfs);

	plotAll(dialog);

	ps.redraw(true);   // *****
      }
      if( saveMovieFiles || answer=="movie and save" )
      { // save a ppm file
	// int movieFame = frame-1;
	int movieFrame = (solutionNumber[cfs]-1)/frameStride + movieFileNameOffset ;
	psp[cfs].set(GI_HARD_COPY_TYPE,GraphicsParameters::ppm);
	//ps.outputString(sPrintF(buff,"Saving file %s%i.ppm",(const char*)movieFileName,movieFrame));
	//ps.hardCopy(    sPrintF(buff,            "%s%i.ppm",(const char*)movieFileName,movieFrame),psp[cfs]);
	ps.outputString(sPrintF(buff,movieMessageFormat,(const char*)movieFileName,movieFrame));
	ps.hardCopy(    sPrintF(buff,movieFileNameFormat,(const char*)movieFileName,movieFrame),psp[cfs]);
	psp[cfs].set(GI_HARD_COPY_TYPE,GraphicsParameters::postScript);
      }

      movieMode=false;

      setSensitivity( dialog, true );
	
    }      
    else if( plotNewFunction || plotNewComponent || (replot && numberOfItemsPlotted>1) )
    {
      if( plotNewFunction )
      {
	// showFileReader.getASolution(solutionNumber[cfs],cg[cfs],u[cfs]);
	for( int fs=0; fs<numberOfFrameSeries; fs++ )
	{
	  if( (plotNewFunction && applyCommandsToAllSeries) || fs==cfs || plotOptions[fs]!=0 )
	  {
	    showFileReader.setCurrentFrameSeries(fs);
	    derivedFunctions[fs].getASolution(solutionNumber[fs],cg[fs],u[fs]);
            getHeaderComments(fs);

//             const aString *header;
//             header=showFileReader.getHeaderComments(numberOfHeaderComments);    
// 	    headerComment[fs]=header;
	    
// 	    for( int fs2=0; fs2<numberOfFrameSeries; fs2++ )
// 	      printF("plotNewFunction: fs=%i : fs2=%i, headerComment=%s, header=%s\n",fs,fs2,(const char*)headerComment[fs2][0],
//                     (const char*)header[0]);

	  }
	}
	showFileReader.setCurrentFrameSeries(cfs);


	numberOfComponents[cfs]=u[cfs].getComponentDimension(0);
	plotNewFunction=false;
      }
      if( plotNewComponent )
      {
	psp[cfs].set(GI_COMPONENT_FOR_CONTOURS,component[cfs]);
	plotNewComponent=false;
      }

      setPlotTitles(cfs);

      plotAll(dialog);

      replot=false;
      

    }
  } // end for(;;)

  delete [] menu;
  ps.unAppendTheDefaultPrompt(); // reset defaultPrompt
  ps.popGUI(); // restore the GUI

  delete [] cg;  cg=NULL;
  delete [] u;   u=NULL;
  delete [] psp; psp=NULL;
  
  delete [] component;           component=NULL;
  delete [] solutionNumber;      solutionNumber=NULL;
  delete [] plotOptions;         plotOptions=NULL;
  delete [] numberOfSolutions;   numberOfSolutions=NULL;
  delete [] numberOfFrames;      numberOfFrames=NULL;
  delete [] numberOfComponents;  numberOfComponents=NULL;
  delete [] numberOfComponents0; numberOfComponents0=NULL;
  delete [] numberOfSequences;   numberOfSequences=NULL;
  delete [] sequenceName;        sequenceName=NULL;
  delete [] derivedFunctions;

  if( showComputedGeometry )
  {
    for( int grid=0; grid<cg[cfs].numberOfComponentGrids(); grid++ )
      cg[cfs][grid].displayComputedGeometry();
  }

  return done;
}
