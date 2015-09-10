#ifndef SHOW_FILE_READER_H
#define SHOW_FILE_READER_H

// -------------------------------------------------------------------
// This class can be used to access solutions from a show file.
//
// This class is used by plotStuff. It is also used by solvers to 
// read in initial conditions from a show file.
// -------------------------------------------------------------------

#include "GenericGraphicsInterface.h"
#include "CompositeGrid.h"
#include "HDF_DataBase.h"
#include "DataBaseAccessFunctions.h"
#include "Ogshow.h"
#include "ShowFileParameter.h"
#include "DBase.hh"
using namespace DBase;

#include OV_STD_INCLUDE(vector)

class ShowFileReader
{
 public:

  enum ReturnType
  {
    notFound=0,
    solutionFound=1,
    gridFound=2,
    solutionAndGridFound=3
  };

  enum GridLocationEnum
  {
    useDefaultLocation=-2,
    useCurrentFrame=-1
  };
  

  ShowFileReader(const aString & nameOfShowFile=nullString);
  ~ShowFileReader();
      
  int open(const aString & nameOfShowFile, const int displayInfo=1 );
  int close();

  int getNumberOfFrameSeries() const;
  aString getFrameSeriesName( const Ogshow::FrameSeriesID frame_series );
  int getNumberOfFrames() const;
  int getNumberOfSolutions() const;
  int getNumberOfSequences() const;
  
  int setCurrentFrameSeries( const Ogshow::FrameSeriesID frame_series );
  Ogshow::FrameSeriesID getCurrentFrameSeries() const;

  ReturnType getAGrid(MappedGrid & cg, 
                      int & solutionNumber, 
                      int frameForGrid=useDefaultLocation);

  ReturnType getAGrid(GridCollection & cg, 
                      int & solutionNumber, 
                      int frameForGrid=useDefaultLocation);

  ReturnType getASolution(int & solutionNumber,
			  MappedGrid & cg,
			  realMappedGridFunction & u);

  ReturnType getASolution(int & solutionNumber,
			  GridCollection & cg,
			  realGridCollectionFunction & u);
  
  int getSequenceNames(aString *name, int maximumNumberOfNames);
  
  int getSequence(int sequenceNumber,
		  aString & name, RealArray & time, RealArray & value, 
		  aString *componentName1, int maxcomponentName1,
		  aString *componentName2, int maxcomponentName2);
  
  // return a pointer to a frame (by default the current frame)
  HDF_DataBase* getFrame(int solutionNumber=-1);  

  bool isAMovingGrid();

  // get a parameter with a given name
  bool getGeneralParameter(const aString & name, int & value,
			   const Ogshow::PlaceToSaveGeneralParameters placeToSave=Ogshow::THECurrentFrameSeries  );
  bool getGeneralParameter(const aString & name, real & value,
			   const Ogshow::PlaceToSaveGeneralParameters placeToSave=Ogshow::THECurrentFrameSeries  );
  bool getGeneralParameter(const aString & name, aString & value,
			   const Ogshow::PlaceToSaveGeneralParameters placeToSave=Ogshow::THECurrentFrameSeries  );

  // get a parameter with a given name and type
  bool getGeneralParameter(const aString & name, ShowFileParameter::ParameterType type, int & value, real & rValue, 
			   aString & stringValue,
			   const Ogshow::PlaceToSaveGeneralParameters placeToSave=Ogshow::THECurrentFrameSeries  );

  // Get parameters from a given directory in the current frame
  bool getParameters(const aString & nameOfDirectory, ListOfShowFileParameters & params );

  // get header comments for the last grid or solution that was found
  const aString* getHeaderComments(int & numberOfHeaderComments);

  

  // Get the list of parameters that go with this file.
  int getGeneralParameters( const int displayInfo=1 ); 

  // Return the general parameters
  ListOfShowFileParameters& getListOfGeneralParameters( 
                  const Ogshow::PlaceToSaveGeneralParameters placeToSave=Ogshow::THECurrentFrameSeries  );

  // For very large files we may have to reduce the number of files that we allow to be open at any time
  void setMaximumNumberOfOpenShowFiles(const int maxNumber);

  /// Here is a database to hold parameters (new way)
  mutable DataBase dbase; 

protected:
  bool showFileIsOpen;
  GenericDataBase **showFile;
  std::vector<aString> frameSeriesNames;
  enum SeriesItemEnum {
    numFrames,
    numSolutions,
    numSequences,
    isMovingGrid,
    frameIDForGrid,
    streamModeOption,
    numItemsInSeries 
  };

  IntegerArray frameSeriesInfo; 

  int numberOfFrames;
  int numberOfSolutions;
  int numberOfSequences;

  int numberOfShowFiles;
  int numberOfOpenFiles;
  int frameNumberForGrid;
  int maximumNumberOfHeaderComments;
  int numberOfHeaderComments;        // for last solution/grid found
  
  int maxNumberOfShowFiles;     
  int maxNumberOfOpenFiles;     // there is some limit like 30-40
  aString *headerComment;        // for last solution/grid found
  aString nameOfShowFile;
  bool movingGridProblem;

  int streamMode;

  Ogshow::FrameSeriesID currentFrameSeries;
  HDF_DataBase currentFrameSeriesDB;
  HDF_DataBase currentFrame;
  
  
  std::vector<ListOfShowFileParameters> listOfGeneralParameters;

  int openShowFile(const int n);
      
  int getNumberOfValidFiles( const int displayInfo=1 );
  
  int countNumberOfFramesAndSolutions( const int displayInfo=1 );
  
  void checkSolutionNumber(const aString & routineName, int & solutionNumber );

  int locateFramesInFiles();

  friend class Ogshow;

};


#endif





      
      
  
