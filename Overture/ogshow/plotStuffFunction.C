#include "ShowFilePlotter.h"
#include "display.h"

static int totalNumberOfArrays=0;

int
plot3dToOverture(GenericGraphicsInterface & gi, aString & showFileName );

int
showFileFromDataFile( GenericGraphicsInterface & gi, aString & showFileName );


void
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




//===============================================================================================
//  This plotStuff program is used to display results saved in a show file
//
//===============================================================================================
//\begin{>plotStuffMenuInclude.tex}{}
//\no function header:
//
// Here is a desciption of the menu options available for plotStuff.
//\begin{description}\index{show files!plotting}
//  \item[contour] : plot contours.
//  \item[stream lines] : plot streamlines.
//  \item[grid] : plot the grid.
//  \item[sequence] : plot a sequence.
//  \item[next] : plot solutions from the next frame.
//  \item[previous] : plot solutions from the previous frame.
//  \item[choose a component] : plot a different component.
//  \item[choose a solution] : choose a different frame.
//  \item[next component] : plot the next component.
//  \item[previous component] : plot the previous component.
//  \item[derived types] : build new components as functions of the old ones, such as the vorticity from the velocity.
//     Once derived types have been created they will appear in the plotStuff component menus.
//  \item[movie] : \index{movie} plot a number of frames in a row.
//  \item[movie and save] : plot frames and save each one as a hard copy.
//  \item[plot bounds] : \index{plot bounds}change the manner in which the plot bounds are determined.
//    \begin{description}
//    \item[set plot bounds] : specify bounds for plotting.
//    \item[use default plot bounds] : use default plot bounds.
// \end{description}
//  \item[check mappings with grid] : a debugging option, check validity of mappings after they have been read
//     in from a data-base file.
//  \item[erase] : erase anything in the window.
//  \item[redraw] : redraw the screen.
//  \item[open a new file] : open a new show file for reading.
//  \item[file output] : \index{file output} output results to a file.
//  \item[help] : print a short help list.
//  \item[exit] : exit this menu and continue on (same as 'continue').
// \end{description}
//
//
//\end{plotStuffMenuInclude.tex} 


int 
plotStuff(int argc, char *argv[])
{
  PlotIt::parallelPlottingOption=1;  // turn on distributed plotting! *******************

  const bool showComputedGeometry=false; // true;
  
  // Overture::start(argc,argv);  // initialize Overture
  Overture::turnOnMemoryChecking(true);

  printF("Type: `plotStuff [-noplot] [-nopause] [-plot3d] [-ovText] fileName [file[.cmd]]' to read the show file called fileName, \n"
         "                                                                       and optionally read a command file. \n"
         "  or: `plotStuff [-noplot] [-nopause] [-plot3d] [-ovText] file.cmd' to run the command file (with first command the show file name). \n");

  aString nameOfShowFile="", commandFileName="";

  bool plot3d=false, ovText=false;
  if( argc > 1 )
  {
    aString line;
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      // printF(" line=[%s]\n",(const char*)line);
      
      if( line=="-noplot" || line=="-nopause" || line=="-abortOnEnd" || line=="-nodirect" ||
          line=="noplot" || line=="nopause" || line=="abortOnEnd" || line=="nodirect"  )
      {
        continue; // these commands are processed by getGraphicsInterface below
      }
      else if( line.matches("-plot3d") )
      {
        plot3d=true;  // process a plot3d file
        printF("-plot3d command found, query for plot3d files...\n");
	
      }
      else if( line.matches("-ovText") )
      {
        ovText=true;  // process a plot3d file
        printF("-ovText command found, query for an Overture text file...\n");
	
      }
      else if( nameOfShowFile=="" && commandFileName == "" )
      {
        // first check to see if 'line' refers to a .cmd file: 
        int l=line.length()-1;
        bool readCommandFile = l>2 && line(l-3,l)==".cmd";
	if( !readCommandFile )
	{ // If the 'line' does not end in .cmd, check for a cmd file named 'line'.cmd     *wdh* 090101
	  aString cmdFile;
	  cmdFile = line + ".cmd";
          FILE *file = fopen(cmdFile,"r"); 
          if( file!=NULL )
	  {
            line = line + ".cmd";
            readCommandFile=true;
	    fclose(file);
	  }
	}
	if( readCommandFile )
	{
	  commandFileName=line;  // we have found a command file name first -- no show file specified
	}
	else 
	{
	  nameOfShowFile=line;
	}
      }
      else if( commandFileName == "" )
      {
        commandFileName=line;
      }
    }
  }
  
  #ifdef USE_PPP
    // On Parallel machines always add at least this many ghost lines on local arrays
    const int numGhost=1;
    MappedGrid::setMinimumNumberOfDistributedGhostLines(numGhost);
  #endif

    
  // create a Graphics Interface
  GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("plotStuff II",false,argc,argv); 
    
  // By default start saving the command file called "plotStuff.cmd"
  aString logFile="plotStuff.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  // read from a command file if given
  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);

  if( plot3d )
  {
    // convert a plot3d file into a show file -- so we can show it here
    plot3dToOverture(ps,nameOfShowFile);
  }
  if( ovText )
  {
    // convert a Overture text file into a show file -- so we can show it here
    showFileFromDataFile(ps,nameOfShowFile);
  }
  

  checkArrays("plotStuff: before loop");

  // this loop is used to look at more than one file
  bool done=false;
  while( !done )
  {
    // cout << ">> Enter the name of the show file:" << endl;
    // cin >> nameOfShowFile;
    if( nameOfShowFile=="" )
      ps.inputString(nameOfShowFile,">> Enter the name of the show file:");

    
    ShowFilePlotter showFilePlotter(nameOfShowFile,ps);

    done = showFilePlotter.plot();
    nameOfShowFile="";

  } // end while not done


  // Overture::finish();          
  return 0;
}
