#ifndef OGSHOW_H
#define OGSHOW_H "Ogshow.h"

#include "CompositeGrid.h"
#include "CompositeGridFunction.h"
#include "ShowFileParameter.h"
#include "DBase.hh"
using namespace DBase;

#include OV_STD_INCLUDE(vector)

class HDF_DataBase;

class Ogshow
{
 public:

  typedef int FrameSeriesID;

  enum ShowFileOpenOption
  {
    openNewFileForWriting,
    openOldFileForWriting
  };


  Ogshow();
  Ogshow(const aString & nameOfShowFile, 
         const aString & nameOfDirectory = ".",
         int useStreamMode=false,
         ShowFileOpenOption openOption=openNewFileForWriting );

  ~Ogshow();

  // return a pointer to the current frame
  HDF_DataBase* getFrame(const Ogshow::FrameSeriesID = 0);  

  int open(const aString & nameOfShowFile, 
	   const aString & nameOfDirectory = ".",
	   int useStreamMode=false,
	   ShowFileOpenOption openOption=openNewFileForWriting );

  int close();

  // save a comment that applies to the whole file
  int saveGeneralComment( const aString & comment );

  // save a comment with the current frame
  int saveComment( const int commentNumber, const aString & comment );

  int saveSequence( const aString & name,
		    const RealArray & time, 
		    const RealArray & value, 
		    aString *name1=NULL, 
		    aString *name2=NULL);

  // save parameters that apply to the whole file
  enum PlaceToSaveGeneralParameters {
    THEShowFileRoot=-2,
    THECurrentFrameSeries=-1
  };

  int saveGeneralParameters( ListOfShowFileParameters & params, 
                             const PlaceToSaveGeneralParameters placeToSave=THECurrentFrameSeries );

  // Save parameters into the current frame
  int saveParameters(const aString & nameOfDirectory, ListOfShowFileParameters & params );

  enum GridLocationEnum
  {
    useDefaultLocation=-2,
    useCurrentFrame=-1
  };
  
  int getTotalNumberOfFrames() const;
  int getNumberOfFrames() const;

  const aString & getShowFileName() const;
  
  int saveSolution( realGridCollectionFunction & u, const aString & name="u", 
		    int frameForGrid=useDefaultLocation);

  int saveSolution( realMappedGridFunction & u, const aString & name="u", 
		    int frameForGrid=useDefaultLocation);

  int getNumberOfFrameSeries() const;
  FrameSeriesID newFrameSeries(const aString & name);
  FrameSeriesID getFrameSeriesID(const aString & name);
  const aString& getFrameSeriesName(const FrameSeriesID frameSeries);
  int setFrameSeriesName(const FrameSeriesID frameSeries, const aString & name);
  FrameSeriesID getCurrentFrameSeries() const;
  int setCurrentFrameSeries(const FrameSeriesID frameSeries);
  int setCurrentFrameSeries(const aString & name);

  int startFrame( const int frame=-1);  // start a new frame or write to an existing one
  int endFrame();  // end the frame, close sub-files if appropriate.
  
  bool getIsMovingGridProblem() const;
  void setIsMovingGridProblem( const bool trueOrFalse );
  
  // indicate that the file should be flushed every so often:
  void setFlushFrequency( const int flushFrequency = 5  );
  int getFlushFrequency() const;

  // return true if the (current) frame is the first frame in the current subFile
  bool isFirstFrameInSubFile( int frameNumber = -1 ) const;
  // return true if the (current) frame is the last frame in the current subFile
  bool isLastFrameInSubFile(int frameNumber = -1 ) const;

  // return the sub-file number where a given frame is stored
  int getSubFileNumberForFrame( int frameNumber = -1 ) const;

  // flush the data in the file  *** this does not work ***
  void flush();

  /// Here is a database to hold parameters (new way)
  mutable DataBase dbase; 

  static int debug;
  
 protected:
  void initialize();
  int setup();
  int cleanup();
  int createShowFile( const aString & nameOfShowFile, const aString & nameOfDirectory, ShowFileOpenOption openOption );
  int saveGeneralCommentsToFile();
  int saveCommentsToFile();
  
  aString showFileName, showFileDirectory;
  GenericDataBase *showFile;        // here is the show file
  GenericDataBase *showDir;         // Here is where we save the showfile information
  int currentFrameSeries; 
  
  int showFileCounter; // counts number of solutions saved so far  
  int showFileCreated; // TRUE if a show file is mounted
  bool generalCommentsSaved;

  std::vector<aString> generalComment;

  int totalNumberOfFrames;
  int numberOfFramesPerFile;  // This many frames per sub file
  int magicNumber;            // unique identifier saved with the file
  int defaultStreamMode;

  struct OgshowFrameSeries {
    OgshowFrameSeries();

    ~OgshowFrameSeries();

    GenericDataBase *showDir;           // current frame;
    GenericDataBase *frame;           // current frame;

    int solutionCounter; // local version of showFileCounter

    bool commentsSaved;
    std::vector<aString> comment;
    int movingGridProblem;  // true if grids are moving (make this an int to be consistent with ShowFileReader)
    int numberOfFrames; // local version of totalNumberOfFrames
    int frameNumber;            // current frame
    int globalFrameNumber;
    FrameSeriesID id;
    int sequenceNumber;         // number of seqences saved in the show file.
    int streamMode;             // if true, save file in stream mode (compressed).
    aString name;
    int totalNumberOfFramesWhenCreated;
  };
  
  std::vector<OgshowFrameSeries> frameSeriesList;

};


#endif  
