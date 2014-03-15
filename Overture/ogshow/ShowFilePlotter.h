#ifndef SHOW_FILE_PLOTTER_H
#define  SHOW_FILE_PLOTTER_H "ShowFilePlotter.h"

// ********************************************************
// This class is used to plot results found in files.
// This class is used by the plotStuff program
// ********************************************************

#include "GenericGraphicsInterface.h"
#include "ShowFileReader.h"
#include "PlotStuff.h"
#define KK_DEBUG
#include "DBase.hh"
using namespace DBase;

class ShowFilePlotter
{
public:
ShowFilePlotter(const aString & nameOfShowFile, GenericGraphicsInterface & ps );
~ShowFilePlotter();

int plot();

// static int 
// buildMainMenu( aString *menu0,
//                aString *&menu,
//                RealGridCollectionFunction & u,
//                aString *sequenceName,
//                const int & numberOfSolutions,
//                const int & numberOfComponents,
//                const int & numberOfSequences,
//                int & chooseAComponentMenuItem,
//                int & chooseASolutionMenuItem,
//                int & numberOfSolutionMenuItems,
//                int & chooseASequenceMenuItem,
//                int & numberOfSequenceMenuItems,
//                const int & maxMenuSolutions,
// 	       const int & maximumNumberOfSolutionsInTheMenu,
// 	       int & solutionIncrement,
// 	       const int & maxMenuSequences,
// 	       const int & maximumNumberOfSequencesInTheMenu,
// 	       int & sequenceIncrement );

// static int
// buildPlotStuffDialog(ShowFileReader & showFileReader, DialogData & dialog, GenericGraphicsInterface & ps,
//                      realCompositeGridFunction & u);

// int 
// updatePlotStuffDialog(ShowFileReader & showFileReader, DialogData & dialog, GenericGraphicsInterface & ps,
// 		      realCompositeGridFunction & u);

protected:

int buildPlotStuffDialog(DialogData & dialog, realCompositeGridFunction & u0, const int cfs);

int getHeaderComments( int cfs );

int plotAll(DialogData & dialog);

bool setFrameSeriesTitles(int cfs );

int setPlotTitles(int cfs, bool useFrameSeriesTitles=true);

void setSensitivity( GUIState & dialog, bool trueOrFalse );

int updatePlotStuffDialog(DialogData & dialog, realCompositeGridFunction & u0, const int component, const int cfs);


aString nameOfShowFile;
ShowFileReader showFileReader;
GenericGraphicsInterface & ps;

int numberOfFrameSeries;
bool applyCommandsToAllSeries;
int numberOfMovieFrames;    // number of frames to plot for a movie
int frameStride;            // stride when sequencing through the frames
aString movieFileName;      // base name for movie ppm's
bool saveMovieFiles;        // if true save movie files when making a movie

int maximumNumberOfHeaderComments;
aString *sequenceName;
aString **headerComment;

CompositeGrid *cg;
realCompositeGridFunction *u;
PlotStuffParameters *psp;
    
int *component; 
int *solutionNumber;
int *plotOptions;

int *numberOfSolutions;
int *numberOfFrames;
int *numberOfComponents;
int *numberOfComponents0;
int *numberOfSequences;

// The dbase holds the forcing regions and other parameters for each frame series
mutable DataBase *dbaseArray; 
};


#endif
