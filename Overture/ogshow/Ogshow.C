#include "Ogshow.h"
#include "HDF_DataBase.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"

static const int showFileProcessor=0;  // only write a show file from this processor

// Old way: In parallel with hdf4 we usually only perform operations on 1 processor
#ifndef USE_PPP
  #define NOT_ON_SHOW_FILE_PROCESSOR false
#else
 #ifdef OV_USE_HDF5
   #define NOT_ON_SHOW_FILE_PROCESSOR false
 #else
   // **kludge for hdf4 **
   #define NOT_ON_SHOW_FILE_PROCESSOR Communication_Manager::My_Process_Number!=showFileProcessor
 #endif
#endif

using namespace std;

//\begin{>OgshowInclude.tex}{\subsubsection{constructors}} 
Ogshow::
Ogshow()
//----------------------------------------------------------------------
// /Description:
//  default constructor
//  /Author: WDH
//\end{OgshowInclude.tex}
//----------------------------------------------------------------------
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return;

  initialize();
}

//\begin{>>OgshowInclude.tex}{}
Ogshow::
Ogshow(const aString & nameOfShowFile, 
       const aString & nameOfDirectory /* = "." */,
       int useStreamMode /* =false */,
       ShowFileOpenOption openOption /*= openNewFileForWriting  */ )
//----------------------------------------------------------------------
// /Description:
//  construct a show file
// /nameOfShowFile (input) : name of the new show file to create
// /nameOfDirectory (input) : directory in the Overlapping grid data base file to use
// /useStreamMode (input): if true, save file in streaming mode (compressed)
// /openOption (input) : specifies whether to open a new file for writing (openNewFileForWriting) or
//    (openOldFileForWriting) open an old file to append to. 
//  /Author: WDH
//\end{OgshowInclude.tex}
//----------------------------------------------------------------------
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return;

  initialize();
  defaultStreamMode=useStreamMode;
  createShowFile( nameOfShowFile,nameOfDirectory,openOption );
}

//----------------------------------------------------------------------------  
//----------------------------------------------------------------------------  
int Ogshow::
createShowFile( const aString & nameOfShowFile, const aString & nameOfDirectory, ShowFileOpenOption openOption )
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  // no show file is created if the name is not given
  if( nameOfShowFile=="" || nameOfShowFile==" " )  
  {
    return(0);
  }
  showFileCreated=true;
  generalCommentsSaved=false;
  
  // save the names
  showFileName=nameOfShowFile;
  showFileDirectory=nameOfDirectory;
   
  // Use the HDF_DataBase:
  showFile = new HDF_DataBase();
  showDir  = new HDF_DataBase();

  if( openOption==openNewFileForWriting )
  {
    // Open a new file for writing


    if( defaultStreamMode )
    {
      // printF("createShowFile: setMode(GenericDataBase::normalMode)\n");
      showFile->setMode(GenericDataBase::normalMode);
      showDir ->setMode(GenericDataBase::normalMode);
      // kkc tmp frame   ->setMode(GenericDataBase::normalMode);

      // Since we may delete Grids that we read in, we need to turn off the linking of Mappings,
      // otherwise we may try to link to a Mapping that has been deleted.
      MappingRC::setDataBaseMode(MappingRC::doNotLinkMappings);
    }

    printF("--- Ogshow: create a new showFile:%s\n",(const char*)nameOfShowFile);
    // I=Initialize a database file
    // W=open old file for writing
    showFile->mount(nameOfShowFile,"I");  

    showFile->create(*showDir,nameOfDirectory,"directory");

    if( !defaultStreamMode )
    {
      // printF("createShowFile: setMode(GenericDataBase::noStreamMode)\n");
      showFile->setMode(GenericDataBase::noStreamMode);
      showDir ->setMode(GenericDataBase::noStreamMode);
      // kkc tmp frame   ->setMode(GenericDataBase::noStreamMode);
    }

    showFile->put(defaultStreamMode,"streamMode");

  }
  else if( openOption==openOldFileForWriting )
  {
    // W=open old file for writing
    printF("--- Ogshow: open an old showFile:%s (for appending to).\n",(const char*)nameOfShowFile);
    int found = showFile->mount(nameOfShowFile,"W");  
    if( found!=0 )
    {
      printF("Ogshow:ERROR: unable to open an existing show file named [%s]\n",(const char*)nameOfShowFile);
      OV_ABORT("Ogshow:ERROR");
    }

    showFile->get(defaultStreamMode,"streamMode");

    // Need to open show files and count files etc. (see ShowFileReader)
    OV_ABORT("Ogshow:openOldFileForWriting: finish me...");
  }
  else
  {
    OV_ABORT("Ogshow:ERROR: unknown open option");
  }
  
  

  return(0);
}

//----------------------------------------------------------------------------  
//----------------------------------------------------------------------------  
void Ogshow::
initialize()
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return;

  magicNumber = time(NULL); // get a unique file indentifier
  // cout << "magicNumber = " << magicNumber << endl;
  numberOfFramesPerFile=-1;   // this means a single file

  showFileCreated=false;
  showFileCounter=0;
  totalNumberOfFrames=0;
  //kkc tmp  sequenceNumber=0;
  defaultStreamMode=false;
  currentFrameSeries=0;
  generalCommentsSaved = false;

}  

Ogshow::
~Ogshow()
{
  cleanup();
}

//\begin{>>OgshowInclude.tex}{\subsubsection{close}}
int Ogshow::
close()
// ====================================================================================
//   /Description:
//     Close a show file.
//\end{OgshowInclude.tex}
// ====================================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  cleanup();
  initialize();

  return 0;
}

//\begin{>>OgshowInclude.tex}{\subsubsection{cleanup}}
int Ogshow::
cleanup()
// ====================================================================================
// /Access: protected
//   /Description:
//     Close and cleanup the show file.
//\end{OgshowInclude.tex}
// ====================================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  generalComment.clear();

  if( showFileCreated )
  {
    if( !showFile->isNull() ) // added for hdf5, *wdh* 060829
    {
      showDir->put((int)frameSeriesList.size(),"numberOfFrameSeries");
      showDir->put(totalNumberOfFrames,"numberOfFrames");
      showFile->put(magicNumber,"magicNumber");
      showDir->put(numberOfFramesPerFile,"numberOfFramesPerFile");
      //kkc tmp      showFile->put(movingGridProblem,"movingGridProblem");
  
      for ( int s=0; s<frameSeriesList.size(); s++ )
      {
	OgshowFrameSeries &fs = frameSeriesList[s];
	// cout<<"cleaning up fs.id = "<<fs.id<<", name = "<<fs.name<<endl;
	fs.showDir->put(fs.numberOfFrames,"numberOfFrames");
	fs.showDir->put(fs.sequenceNumber,"numberOfSequences");
	fs.showDir->put(fs.movingGridProblem,"movingGridProblem");
	fs.showDir->put(fs.streamMode,"streamMode");
	fs.showDir->put(fs.id,"id");
	  
	fs.showDir->put(fs.totalNumberOfFramesWhenCreated,"totalNumberOfFramesWhenCreated");
	if ( fs.showDir ){ delete fs.showDir; fs.showDir=NULL;}
	if ( fs.frame ){ delete fs.frame; fs.frame=NULL;} 
      }

      showFile->unmount();
    }
    
    //    frameSeriesList.clear();
    delete showFile; showFile=NULL;
    delete showDir;  showDir=NULL;
    //kkc tmp    delete frame;    frame=NULL;

    showFileCreated=false;  // *wdh* 060829
  }

  return 0;
}

//\begin{>>OgshowInclude.tex}{\subsubsection{open}}
int Ogshow::
open(const aString & nameOfShowFile, 
     const aString & nameOfDirectory /* = "." */,
     int useStreamMode /* =false */,
     ShowFileOpenOption openOption /*= openNewFileForWriting  */ )
// ====================================================================================
//   /Description:
//     Open a show file (close any currently open file).
// /nameOfShowFile (input) : name of the new show file to create
// /nameOfDirectory (input) : directory in the Overlapping grid data base file to use
// /useStreamMode (input): if true, save file in streaming mode (compressed)
// /openOption (input) : specifies whether to open a new file for writing (openNewFileForWriting) or
//    (openOldFileForWriting) open an old file to append to. 
//\end{OgshowInclude.tex}
// ====================================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  if( showFileCreated )
  {
    cleanup();
  }
  initialize();
  defaultStreamMode=useStreamMode;
  createShowFile( nameOfShowFile,nameOfDirectory,openOption );
  return 0;
}


//\begin{>>OgshowInclude.tex}{\subsubsection{getFrame}} 
HDF_DataBase* Ogshow::
getFrame(const Ogshow::FrameSeriesID /*= 0*/)
// ====================================================================================
//   /Description:
//     Return a pointer to the data base directory holding the current frame.
//     You could use this pointer to save additional data in the frame. In the following example
//     some extra data in the form of a realArray is saved in the frame. 
//    \begin{verbatim}
//       Ogshow show(...);
//       ...
//       show.startFrame();
//       realArray myData(10); 
//       myData(0)=1.; myData(1)=2.; ...
//       show.getFrame()->put(myData,"my data");
//       ...
//    \end{verbatim}
//    This data can be retrieved using the ShowFileReader.
//  /Return value: Return a pointer to the data base directory holding the current frame, possibly NULL.
//  /Author: WDH
//\end{OgshowInclude.tex}
// ====================================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return NULL;

  if( frameSeriesList[currentFrameSeries].frame==NULL )
  {
    cout << "Ogshow::getFrame::ERROR: the current frame is NULL !\n";
    throw "error";
  }
  return (HDF_DataBase*)frameSeriesList[int(currentFrameSeries)].frame;
}



//\begin{>>OgshowInclude.tex}{\subsubsection{setFlushFrequency}} 
void Ogshow:: 
setFlushFrequency( const int flushFrequency  /*  = 5 */ )
// ====================================================================================
//   /Description:
//     Flush the file every time "flushFrequency" frames have been added.
//   In the current implementation "flushing the file" consists of closing the file
//   and opening a new file to save new frames in. 
//
//  /flushFrequency (input): If positive then the file is "flushed" when every time
//    this many new frames have been added.
//  /Author: WDH
//\end{OgshowInclude.tex}
// ====================================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return;

  numberOfFramesPerFile=flushFrequency;
}

//\begin{>>OgshowInclude.tex}{\subsubsection{getFlushFrequency}} 
int Ogshow:: 
getFlushFrequency() const
// ====================================================================================
//   /Description:
//     Return the flush frequency.
//\end{OgshowInclude.tex}
// ====================================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  return numberOfFramesPerFile;
}

//\begin{>>OgshowInclude.tex}{\subsubsection{isFirstFrameInSubFile}} 
bool Ogshow::
isFirstFrameInSubFile() const
// ====================================================================================
//   /Description:
//     Return true if the current frame is the first frame in the current subFile ( subfile's are
// named fileName.show, fileName.show1, fileName.show2, ...)
//   frame will go into a new sub-file. 
//\end{OgshowInclude.tex}
// ====================================================================================
{
   if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

   // frameNumber starts at 1
   return (frameSeriesList[currentFrameSeries].globalFrameNumber % numberOfFramesPerFile) == 1; 
}


//\begin{>>OgshowInclude.tex}{\subsubsection{isLastFrameInSubFile}} 
bool Ogshow::
isLastFrameInSubFile() const
// ====================================================================================
//   /Description:
//     Return true if the current frame is the last frame in the current subFile ( subfile's are
// named fileName.show, fileName.show1, fileName.show2, ...)
//   frame will go into a new sub-file. 
//\end{OgshowInclude.tex}
// ====================================================================================
{
   if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

   // frameNumber starts at 1
   if( numberOfFramesPerFile<=0 ) 
     return false;
   else
     return (frameSeriesList[currentFrameSeries].globalFrameNumber % numberOfFramesPerFile) == 0 ; 
}


// int Ogshow::
// setStreamMode( int useStreamMode /* =TRUE */ )
// // ====================================================================================
// //   /Description:
// //     Set the stream mode. NOTE: this must be set before any frames are created.
// //  /useStreamMode (input): if true, save file in streaming mode (compressed)
// // ====================================================================================
// {
//   if( frameNumber==0 )
//   {
//     streamMode=useStreamMode;
//     if( !streamMode && showFile!=NULL )
//       showFile->setMode(GenericDataBase::noStreamMode);
//   }
//   else
//   {
//     printF("Ogshow::setStreamMode:ERROR: cannot set streamMode since frameNumber=%i is not zero\n");
//     return 1;
//   }
//   return 0;
// }



void Ogshow:: 
flush()
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return;

  if( false )
  {
    cout << "Ogshow: ---flush the file:" << showFileName << endl;
    showFile->flush();
  }
  else
  {
    cout << "Ogshow: ---flush the file:" << showFileName << ", by unmount/mount" << endl;
    showFile->unmount();
    showFile->mount(showFileName,"W"); 
    showFile->find(*showDir,showFileDirectory,"directory");
    char buff[40];
    //kkc tmp    showDir->find(*frame,sPrintF(buff,"frame%i",frameNumber),"frame");
    // set up the "current frame" for each frame series
    for ( vector<OgshowFrameSeries>::iterator fs = frameSeriesList.begin();
	  fs!=frameSeriesList.end();
	  fs++ )
      {
	showFile->find(*fs->showDir, fs->name,"directory");
	fs->showDir->find(*fs->frame, sPrintF(buff,"frame%i",fs->frameNumber),"frame");
      }

  }
}


//\begin{>>OgshowInclude.tex}{\subsubsection{setMovingGridProblem}} 
bool Ogshow::
getIsMovingGridProblem() const
// ====================================================================================
//   /Description:
//     Return true if this is a moving grid problem.
//  /Author: WDH
//\end{OgshowInclude.tex}
// ====================================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return false;

  if( currentFrameSeries==0 && frameSeriesList.size()==0 )
    ((Ogshow*)(this))->newFrameSeries("defaultFrameSeries");

  if ( currentFrameSeries>=0 && currentFrameSeries<frameSeriesList.size() )
    return frameSeriesList[currentFrameSeries].movingGridProblem;
  else
    return false;
}



//\begin{>>OgshowInclude.tex}{\subsubsection{setMovingGridProblem}} 
void Ogshow::
setIsMovingGridProblem( const bool trueOrFalse )
// ====================================================================================
//   /Description:
//     Indicate if this is a moving grid problem so that the grid is saved in every frame
//  /trueOrFalse (input): TRUE is this is a moving grid problem
//  /Author: WDH
//\end{OgshowInclude.tex}
// ====================================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return;

  if( currentFrameSeries==0 && frameSeriesList.size()==0 )
    newFrameSeries("defaultFrameSeries");

  if ( currentFrameSeries>=0 && currentFrameSeries<frameSeriesList.size() )
    frameSeriesList[currentFrameSeries].movingGridProblem=trueOrFalse;
}


//\begin{>>OgshowInclude.tex}{\subsubsection{getNumberOfFrames}} 
int Ogshow::
getTotalNumberOfFrames() const
//=======================================================================
// /Description:
//   return the number of frames that exist in the show file.
//  /Author: KKC
//\end{OgshowInclude.tex}
//=======================================================================
{
  return totalNumberOfFrames;
}

//\begin{>>OgshowInclude.tex}{\subsubsection{getNumberOfFrames}} 
int Ogshow::
getNumberOfFrames() const
//=======================================================================
// /Description:
//   return the number of frames that exist in a particular frame series
//  /Author: WDH
//\end{OgshowInclude.tex}
//=======================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  if ( currentFrameSeries>=0 && currentFrameSeries<frameSeriesList.size() )
    return frameSeriesList[currentFrameSeries].numberOfFrames;
  return -1;
}

//\begin{>>OgshowInclude.tex}{\subsubsection{getShowFileName}} 
const aString & Ogshow::
getShowFileName() const
//=======================================================================
// /Description:
//   return the name of the show file.
//  /Author: WDH
//\end{OgshowInclude.tex}
//=======================================================================
{
  return showFileName;
}


//\begin{>>OgshowInclude.tex}{\subsubsection{startFrame}} 
int Ogshow::
startFrame( const int frameNo /* = newFrame */ )
//=======================================================================
// /Description:
//   start a new frame or write to an existing one
// /frameNo (input): by default start a new frame, otherwise open a frame with
//   the given value.
//  /Author: WDH
//\end{OgshowInclude.tex}
//=======================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  if ( currentFrameSeries>frameSeriesList.size() || currentFrameSeries<0 )
    return 1;
  else if ( currentFrameSeries==0 && !frameSeriesList.size() )
    newFrameSeries("defaultFrameSeries");

  OgshowFrameSeries &frameSeries = frameSeriesList[currentFrameSeries];

  int retval=0;
  char buff[40];
  if( frameNo==-1 ) // start a new frame 
  {
    totalNumberOfFrames++;
    //kkc tmp    frameNumber=numberOfFrames;
    frameSeries.numberOfFrames++;
    frameSeries.frameNumber=frameSeries.numberOfFrames;
    frameSeries.globalFrameNumber = totalNumberOfFrames;
    if( numberOfFramesPerFile>0 && totalNumberOfFrames>1 && (totalNumberOfFrames-1) % numberOfFramesPerFile == 0 )
    {
       // close existing file and open a new file

      // The showFile may have been closed by endFrame in which case we do not have to close it.
      if( ! showFile->isNull() )
      {
        printF("Ogshow:startFrame: close the show file (totalNumberOfFrames=%i numberOfFramesPerFile=%i)\n",
                 totalNumberOfFrames,numberOfFramesPerFile);
     
	showFile->put(totalNumberOfFrames-1,"numberOfFrames");  // this is the total number of frames saved so far
	showFile->put(numberOfFramesPerFile,"numberOfFramesPerFile");

	showFile->put(magicNumber,"magicNumber");
	//kkc tmp	showFile->put(movingGridProblem,"movingGridProblem");
	for ( vector<Ogshow::OgshowFrameSeries>::iterator fs = frameSeriesList.begin();
	      fs!=frameSeriesList.end();
	      fs++ )
	  {
	    fs->showDir->put(fs->numberOfFrames,"numberOfFrames");
	    fs->showDir->put(fs->sequenceNumber,"numberOfSequences");
	    fs->showDir->put(fs->movingGridProblem,"movingGridProblem");
	    fs->showDir->put(fs->streamMode,"streamMode");
	    fs->showDir->put(fs->id,"id");
	    
	    fs->showDir->put(fs->totalNumberOfFramesWhenCreated,"totalNumberOfFramesWhenCreated");
	  }
	showFile->unmount();
      }
      
      aString name = showFileName + sPrintF(buff,"%i",(totalNumberOfFrames-1)/numberOfFramesPerFile);
      // cout << "Open the new show subfile called: " << name << endl;
      showFile->mount(name,"I"); 
      showFile->create(*showDir,showFileDirectory,"directory");
      // recreate all the directories for the open frame series
      for ( vector<OgshowFrameSeries>::iterator fs = frameSeriesList.begin();
	    fs!=frameSeriesList.end();
	    fs++ )
	showFile->create(*fs->showDir, fs->name, "frameSeries");
    }
  }
  else if( frameNo > 0 && frameNo <= frameSeries.numberOfFrames )
  {
    frameSeries.frameNumber=frameNo;
    // we may have to open a new show file if this frame is not in the current sub-file

  }
  else
  {
    printF("Ogshow:startFrame:Error invalid value for frame=%i\n",frameNo);
    retval=1;
  }
  
  // create a frame directory if it is not already there
  if( frameSeries.showDir->locate(*frameSeries.frame,sPrintF(buff,"frame%i",frameSeries.frameNumber),"frame") != 0  )
  {
    frameSeries.showDir->create(*frameSeries.frame,sPrintF(buff,"frame%i",frameSeries.frameNumber),"frame");
    frameSeries.sequenceNumber=0;  // *wdh* 061025 -- reset the count to zero
  } 
  

  return retval;
}


//\begin{>>OgshowInclude.tex}{\subsubsection{endFrame}} 
int Ogshow::
endFrame()
//=======================================================================
// /Description:
//   End the currently open frame (if any). The main purpose of calling
// this routine is to close a sub-file if this was the last frame in the sub-file.
// This will allow the sub-file to be read programs such as plotStuff.
// WARNING: once a sub-file is closed you can no longer write to a frame in that sub-file.
//  This needs to be fixed.
// 
//  /Author: WDH
//\end{OgshowInclude.tex}
//=======================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  if ( currentFrameSeries>frameSeriesList.size() || currentFrameSeries<0 )
    return 1;

  OgshowFrameSeries &frameSeries = frameSeriesList[currentFrameSeries];

  int retval=0;
  // numberOfFrames++;
  // frameNumber=numberOfFrames;

  // kkc 060914 added isNull check for showFile in case endFrame is called many times (e.g. OB_CompositeGridSolver::advance)
  if( numberOfFramesPerFile>0 && totalNumberOfFrames>0 && totalNumberOfFrames % numberOfFramesPerFile == 0 && !showFile->isNull() )
  {
    // close existing file 
    // cout << "endFrame: close a showfile...\n";
    showFile->put(totalNumberOfFrames,"numberOfFrames");  // this is the total number of frames saved so far
    showFile->put(numberOfFramesPerFile,"numberOfFramesPerFile");

    showFile->put(magicNumber,"magicNumber");
    //kkc tmp    showFile->put(movingGridProblem,"movingGridProblem");
    
    for ( vector<Ogshow::OgshowFrameSeries>::iterator fs = frameSeriesList.begin();
	  fs!=frameSeriesList.end();
	  fs++ )
      {
	fs->showDir->put(fs->numberOfFrames,"numberOfFrames");
	fs->showDir->put(fs->sequenceNumber,"numberOfSequences");
	fs->showDir->put(fs->movingGridProblem,"movingGridProblem");
	fs->showDir->put(fs->streamMode,"streamMode");
	fs->showDir->put(fs->id,"id");
	
	fs->showDir->put(fs->totalNumberOfFramesWhenCreated,"totalNumberOfFramesWhenCreated");
      }

    showFile->unmount();

  }
  return retval;
}


int Ogshow::
saveSequence(const aString & name,
             const RealArray & time, 
	     const RealArray & value, 
	     aString *name1 /* =NULL */, 
	     aString *name2 /* =NULL */)
// ===============================================================================================
// /Description:
//   Save a sequence. A sequence is defined as a set of times and values
//   (time(i),value(i,c0,c1)) i=0,1,2,... 
//
// /name (input) : name of the sequence.
// /time (input) : time(0...n-1) - array of n 'time' values or other iteration variable.
// /value (input) : value(0...n-1,0..m-1) array of n values for each of m components.
// /name1 (input) : name1[0..m-1] name for the components.
// /name2 (input) : names for a second level of components BUT DO NOT USE THIS for now.
// 
// /NOTE: Sequences are not saved in a frame but saved in the show file. If there are multiple
// show sub-files then a sequence can be saved in each sub-file but only the one in the last
// sub-file will be shown by plotStuff. 
// /NOTE: To be safe you should save a sequence BEFORE calling endFrame() for otherwise the
//   show sub-file may get closed before the sequences can be saved. 
// ===============================================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  assert( showFile!=NULL );

  if ( currentFrameSeries>frameSeriesList.size() || currentFrameSeries<0 )
    return 1;
  
  if ( !frameSeriesList.size() ) newFrameSeries("defaultFrameSeries");
  OgshowFrameSeries &frameSeries = frameSeriesList[currentFrameSeries];

  char buff[40];
  // bool oldShowFileWasOpened=false;
  if( showFile->isNull() )
  {
    printF("Ogshow::saveSequence:ERROR: showFile->isNull() !\n"
           " It is likely that the show sub-file has been closed based on the frequency to flush\n"
           " You should save the sequences BEFORE calling endFrame() to avoid this error.\n");
    OV_ABORT("ERROR");
    
    // --- this doesn't work because we cannot over-ride the numberOfSequences in the show file !!

//     oldShowFileWasOpened=true;
    
//     // Reopen the last show subfile
//     aString name = showFileName + sPrintF(buff,"%i",(totalNumberOfFrames-1)/numberOfFramesPerFile);
//     printF("saveSequence: Open the OLD show subfile called: %s\n",(const char*)name);
//     printF("This is necessary since the subfile has been closed. It may be more efficient to save\n"
//            " the sequences BEFORE calling endFrame()\n");
//     showFile->mount(name,"W");   // open an old file for writing
//     showFile->locate(*showDir,showFileDirectory,"directory");
//     showFile->locate( *frameSeries.showDir, frameSeries.name, "frameSeries");
  }
  

  HDF_DataBase sequence;

  frameSeries.showDir->create(sequence,sPrintF(buff,"sequence%i",frameSeries.sequenceNumber),"sequence");
  frameSeries.sequenceNumber++;

  sequence.put(name,"name");
  sequence.put(time,"time");
  RealArray v;    v=value;  // make a copy in case of a view which might not be put correctly
  sequence.put(v,"value");

  const int numberOfComponentDimensions=2;
  aString *cName[numberOfComponentDimensions];
  cName[0]=name1; cName[1]=name2;
  
  for( int c=0; c<numberOfComponentDimensions; c++ )
  {
    int num=value.getLength(c+1);
    if( cName[c]==NULL )
    {
      cName[c]=new aString [num];
      for( int n=0; n<num; n++ )
	cName[c][n]=sPrintF(buff,"%i",n);
	
      sequence.put(cName[c],sPrintF(buff,"componentName%1i",c),num);
      delete [] cName[c];
    }
    else
      sequence.put(cName[c],sPrintF(buff,"componentName%1i",c),num);
  }

//   if( oldShowFileWasOpened )
//   { // update the numberOfSequences
//     frameSeries.showDir->put(frameSeries.sequenceNumber,"numberOfSequences");
//     showFile->unmount();
//   }
  

  return 0;
}



//\begin{>>OgshowInclude.tex}{\subsubsection{saveGeneralComment}} 
int Ogshow::
saveGeneralComment( const aString & comment0 )
//------------------------------------------------------------------------
// /Description:
//   Save a general comment (this comment is associated with the entire show file).
//   Multiple comments can be saved by repeatedly calling this function.
// /comment0 (input): comment to save.
//  /Author: WDH
//\end{OgshowInclude.tex}
//------------------------------------------------------------------------
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  if( !showFileCreated ) return(0);

  generalComment.push_back(comment0);
  generalCommentsSaved = false;
  return(0);
}

//------------------------------------------------------------------------
//   Save general comments to the file
//------------------------------------------------------------------------
int Ogshow::
saveGeneralCommentsToFile()
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  if( !showFileCreated ) return(0);

  if( generalCommentsSaved || generalComment.size()==0 ) return(0);
  if ( generalComment.size() )
    {
      aString *stmp = new aString[generalComment.size()];
      for ( int s=0; s<generalComment.size(); s++ ) stmp[s] = generalComment[s];
      showDir->put(stmp,"header",generalComment.size());
      delete [] stmp; // *wdh* 061027
    }

  generalCommentsSaved=true;
  return(0);
}


//\begin{>>OgshowInclude.tex}{\subsubsection{saveComment}} 
int Ogshow::
saveComment( const int commentNumber, const aString & comment0 )
//------------------------------------------------------------------------
// /Description:
//   Save a comment to go in the current frame.
// /commentNumber (input): An integer, 0,1,2,.. that numbers the comment
// /comment0 (input): comment to save.
//  /Author: WDH
//\end{OgshowInclude.tex}
//------------------------------------------------------------------------
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  if( !showFileCreated ) return(0);

  if ( currentFrameSeries>frameSeriesList.size() || currentFrameSeries<0 )
    return 1;
  
  if ( !frameSeriesList.size() ) newFrameSeries("defaultFrameSeries");
  OgshowFrameSeries &frameSeries = frameSeriesList[currentFrameSeries];

  frameSeries.comment.push_back(comment0);

  frameSeries.commentsSaved=false;
  return (0);
}

//------------------------------------------------------------------------
//   Save comments for this solution number to the show file
//------------------------------------------------------------------------
int Ogshow::
saveCommentsToFile()
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  if( !showFileCreated ) return(0);

  if ( currentFrameSeries>frameSeriesList.size() || currentFrameSeries<0 )
    return 1;

  if ( !frameSeriesList.size() ) newFrameSeries("defaultFrameSeries");
  OgshowFrameSeries &frameSeries = frameSeriesList[currentFrameSeries];

  if( frameSeries.commentsSaved || frameSeries.comment.size()==0 ) return(0);

  aString *stmp = new aString[frameSeries.comment.size()];
  for ( int s=0; s<frameSeries.comment.size(); s++ ) stmp[s] = frameSeries.comment[s];
  frameSeries.frame->put(stmp,"header",frameSeries.comment.size());

  delete [] stmp;                   // *wdh* 061027
  frameSeries.comment.clear();      // *wdh* 061027
  
  frameSeries.commentsSaved=true;
  
  return(0);
}


//\begin{>>OgshowInclude.tex}{\subsubsection{saveGeneralParameters}} 
int Ogshow::
saveGeneralParameters( ListOfShowFileParameters & params, const PlaceToSaveGeneralParameters placeToSave )
//==========================================================================================
// /Description:
//   Save parameters that apply to the whole file.
// /params (input): A list of parameters to save
// /placeToSave (input) : save in the root directory (THEShowFileRoot) or in the current frame (THECurrentFrameSeries).
//  /Author: WDH
//\end{OgshowInclude.tex}
//==========================================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  if( !showFileCreated ) return(0);


  const int n = params.size();
  if( n<=0 ) return 0;
  
  int *type = new int [n];
  aString *name = new aString [n];  
  int *iv = new int [n];
  real *rv = new real [n];
  aString *sv = new aString [n];
  
  int i;
  std::list<ShowFileParameter>::iterator iter; 
  for(i=0, iter = params.begin(); iter!=params.end(); iter++, i++ )
  {
    ShowFileParameter & sfp = *iter;
    ShowFileParameter::ParameterType pType;
    sfp.get(name[i],pType,iv[i],rv[i],sv[i]);  // get the parameter type and value
    type[i]=pType;
  }
  
  if ( !frameSeriesList.size() ) newFrameSeries("defaultFrameSeries");
  GenericDataBase * saveDir = placeToSave==THECurrentFrameSeries ? frameSeriesList[currentFrameSeries].showDir : showDir;
  saveDir->put(n,"numberOfGeneralParameters");
  saveDir->put(name,"generalParameterName",n);
  saveDir->put(type,"generalParameterType",n);
  saveDir->put(iv,"generalParameterInt",n);
  saveDir->put(rv,"generalParameterReal",n);
  saveDir->put(sv,"generalParameterString",n);

  delete [] type;
  delete [] name;
  delete [] iv;
  delete [] rv;
  delete [] sv;
  
  return 0;
}

//\begin{>>OgshowInclude.tex}{\subsubsection{saveGeneralParameters}} 
  // Save a named set of parameters
int Ogshow::
saveParameters(const aString & nameOfDirectory, ListOfShowFileParameters & params )
//==========================================================================================
// /Description:
//   Save a named set of parameters in the current frame.
// /nameOfDirectory (input) : the name of the (new) directory where to save the parameters
// /params (input): A list of parameters to save
// 
//  /Author: WDH
//\end{OgshowInclude.tex}
//==========================================================================================
{
  if( NOT_ON_SHOW_FILE_PROCESSOR ) return 0;

  if( !showFileCreated ) return(0);

  const int n = params.size();
  if( n<=0 ) return 0;
  
  int *type = new int [n];
  aString *name = new aString [n];  
  int *iv = new int [n];
  real *rv = new real [n];
  aString *sv = new aString [n];
  
  int i;
  std::list<ShowFileParameter>::iterator iter; 
  for(i=0, iter = params.begin(); iter!=params.end(); iter++, i++ )
  {
    ShowFileParameter & sfp = *iter;
    ShowFileParameter::ParameterType pType;
    sfp.get(name[i],pType,iv[i],rv[i],sv[i]);  // get the parameter type and value
    type[i]=pType;
  }
  
  GenericDataBase *saveDir = getFrame();
  assert( saveDir!=NULL );
  
  // make a directory to save these parameters
  GenericDataBase & dir = *(saveDir->virtualConstructor());  // create a derived data-base object
  saveDir->create(dir,nameOfDirectory,"Parameters");                   // create a sub-directory 

  dir.put(n,"numberOfParameters");
  dir.put(name,"name",n);
  dir.put(type,"type",n);
  dir.put(iv,"int",n);
  dir.put(rv,"real",n);
  dir.put(sv,"string",n);

  delete &dir;
  
  delete [] type;
  delete [] name;
  delete [] iv;
  delete [] rv;
  delete [] sv;
  
  return 0;

}

//\begin{>>OgshowInclude.tex}{\subsubsection{saveSolution}} 
int Ogshow::
saveSolution(realMappedGridFunction & u, 
             const aString & name /* = "u" */,
             int frameForGrid /* = useDefaultLocation */ )
// ===============================================================================
// /Description:
//   Save a mappedGridFunction in the current frame.
//   (for now save a CompositeGridFunction)
// /u (input) : grid function to save
// /name (input): save in the frame under this name. (Currently if you change this name
//   from the default then plotStuff will not find the solution).
// /frameForGrid : indicates where in the show file the grid for this solution can be found.
//    This grid will saved in this frame if it does not already exist.
//     useDefaultLocation : use default location (frame 1), useCurrentFrame : current frame, 
//      $>0$ : specify a frame number.
//  /Author: WDH
//\end{OgshowInclude.tex}
// ===============================================================================
{
  const MappedGrid & mg = *u.getMappedGrid();
  CompositeGrid gc(mg.numberOfDimensions(),1);  // make a CompositeGrid with 1 component grid
  gc[0].reference(mg);
  gc.updateReferences();

  Range all;
  Range R[8] = { all,all,all,all,all,all,all,all };
  int component;
  for( component=0; component<5; component++ )
    R[u.positionOfComponent(component)]= u.getComponentDimension(component)>0 ? 
                   Range(u.getComponentBase(component),u.getComponentBound(component))
                  : all;
  realCompositeGridFunction v(gc,R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
  v[0]=u;
  v.setName(u.getName());
  for( component=0; component<u.getComponentDimension(0); component++ )
    v.setName(u.getName(component),component);

  return saveSolution(v,name,frameForGrid);
}


//\begin{>>OgshowInclude.tex}{\subsubsection{saveSolution}} 
int Ogshow::
saveSolution(realGridCollectionFunction & u_, 
             const aString & name /* = "u" */,
             int frameForGrid /* = useDefaultLocation */ )
// ===============================================================================
// /Description:
//   Save a realGridCollectionFunction or realCompositeGridFunction in the current frame.
// /u (input) : grid function to save
// /name (input): save in the frame under this name. (Currently if you change this name
//   from the default then plotStuff will not find the solution).
// /frameForGrid : indicates where in the show file the grid for this solution can be found.
//    This grid will saved in this frame if it does not already exist.
//     useDefaultLocation : use default location (frame 1), useCurrentFrame : current frame, 
//      $>0$ : specify a frame number. 
//  /Author: WDH
//\end{OgshowInclude.tex}
// ===============================================================================
{
  // ** For parallel we need to build a copy of the grid and grid function that lives on one processor

#ifndef USE_PPP
  realGridCollectionFunction & u = u_;
#else

 #ifdef OV_USE_HDF5
    realGridCollectionFunction & u = u_;  
 #else
    // *** HDF4 kludge ****
//    // In parallel: make a new grid and grid function that only live on one processor
//    realGridCollectionFunction u;
//    GridCollection gc;
//    redistribute( u_, gc,u,Range(showFileProcessor,showFileProcessor) );


  // In parallel: make a new grid and gridfunction that only live on one processor
  GridCollection *gcp=NULL;
  realGridCollectionFunction *up=NULL;
  if( u_.getGridCollection()->getClassName()=="CompositeGrid" )
  {
    CompositeGrid & cg = *new CompositeGrid();
    realCompositeGridFunction & ucg = *new realCompositeGridFunction();

    ParallelGridUtility::redistribute( (realCompositeGridFunction &)u_, cg,ucg,
                                      Range(showFileProcessor,showFileProcessor) );

    gcp=&cg;
    up=&ucg;
  }
  else
  {
    gcp = new GridCollection();
    up  = new realGridCollectionFunction();

    ParallelGridUtility::redistribute( u_, *gcp,*up,Range(showFileProcessor,showFileProcessor) );

  }
  GridCollection & gc = *gcp;
  const realGridCollectionFunction & u = *up;


  if( Communication_Manager::localProcessNumber()!=showFileProcessor ) return 0;
 #endif
#endif

  int totalNumberOfArrays = GET_NUMBER_OF_ARRAYS;
  bool checkArrays =false;

  if( !showFileCreated ) return(0);

  showFileCounter++;

  if ( !frameSeriesList.size() ) newFrameSeries("defaultFrameSeries");
  int frameNumber = frameSeriesList[currentFrameSeries].frameNumber;
  GenericDataBase *frame = frameSeriesList[currentFrameSeries].frame;
  frameSeriesList[currentFrameSeries].solutionCounter++;
  int movingGridProblem = frameSeriesList[currentFrameSeries].movingGridProblem;

  if( movingGridProblem )
  { // save a grid
    if( frameNumber==0 )
    {
      printF("Ogshow::saveSolution: You should use startFrame before saving a grid function\n");
      startFrame();
    }
  }
  
  if( checkArrays && GET_NUMBER_OF_ARRAYS > totalNumberOfArrays ) 
  {
    totalNumberOfArrays=GET_NUMBER_OF_ARRAYS;
    printF("\n**** Ogshow:1 :Number of A++ arrays = %i \n\n",GET_NUMBER_OF_ARRAYS);
  }

  if( frameForGrid==useDefaultLocation )
    frameForGrid = movingGridProblem ? frameNumber : 1;
  else if( frameForGrid==useCurrentFrame )
    frameForGrid=frameNumber;

  int oldFrameForGrid;
  frame->turnOffWarnings(); // turn off warnings for next line:
  int status = frame->get(oldFrameForGrid,"frameForGrid");
  frame->turnOnWarnings();
  if( status==0 )
  {
    // this frame already has a frameForGrid saved
    if( oldFrameForGrid!=frameForGrid )
    {
      printF("Ogshow:saveSolution:ERROR: inconsistent values for frameForGrid=%i, oldFrameForGrid=%i\n"
             "   The show file may be incorrect\n",frameForGrid,oldFrameForGrid);
    }
  }
  else
  {
    frame->put(frameForGrid,"frameForGrid");  // this marks where to find the grid for this solution.

    if( movingGridProblem || frameNumber==1 || frameForGrid==frameNumber ) 
    {
      if( false && movingGridProblem )
	printF("\n $$$$$$$$$$$$$$$$ Ogshow: save the grid for a moving grid problem $$$$$$$$$$$$\n\n");

      // cout << "Ogshow: put the GridCollection in frame = " << frameNumber << endl;

      GridCollection & gc0 = *u.getGridCollection();
      if( gc0.getClassName()=="CompositeGrid" )
      {  // the GridCollection is really a CompositeGrid
// #ifndef USE_PPP
// 	CompositeGrid cg = (CompositeGrid&) gc0;  // make a copy
// 	// first destroy any big geometry arrays: (but not the mask)
// 	cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
// #else
// 	CompositeGrid & cg = (CompositeGrid&) gc0;  // The above didn't work in parallel (why ?)
// #endif

	CompositeGrid & cg = (CompositeGrid&) gc0;  // The above didn't work in parallel (why ?)
        int geometryToPut=MappedGrid::THEmask; // only save the mask
	cg.put(*frame,"CompositeGrid",geometryToPut);
      }
      else
      {
// #ifndef USE_PPP
// 	GridCollection cg = (CompositeGrid&) gc0;
// 	// first destroy any big geometry arrays: (but not the mask)
// 	cg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
// #else
// 	GridCollection & cg = (CompositeGrid&) gc0;
// #endif
	GridCollection & cg = (CompositeGrid&) gc0;
        int geometryToPut=MappedGrid::THEmask; // only save the mask
	cg.put(*frame,"CompositeGrid",geometryToPut);
      }
    }
    else
    {
      // make a link here instead??
    }
  }
  

  if( checkArrays && GET_NUMBER_OF_ARRAYS > totalNumberOfArrays ) 
  {
    totalNumberOfArrays=GET_NUMBER_OF_ARRAYS;
    printF("\n**** Ogshow:2 :Number of A++ arrays = %i \n\n",GET_NUMBER_OF_ARRAYS);
  }

  saveGeneralCommentsToFile();
  frameSeriesList[currentFrameSeries].commentsSaved=false;
  saveCommentsToFile();

  if( checkArrays && GET_NUMBER_OF_ARRAYS > totalNumberOfArrays ) 
  {
    totalNumberOfArrays=GET_NUMBER_OF_ARRAYS;
    printF("\n**** Ogshow:3 :Number of A++ arrays = %i \n\n",GET_NUMBER_OF_ARRAYS);
  }

  // cout << "Ogshow: put a grid function in frame = " << frameNumber << endl;
  u.put(*frame,name);
  
  if( checkArrays && GET_NUMBER_OF_ARRAYS > totalNumberOfArrays ) 
  {
    totalNumberOfArrays=GET_NUMBER_OF_ARRAYS;
    printF("\n**** Ogshow:4 :Number of A++ arrays = %i \n\n",GET_NUMBER_OF_ARRAYS);
  }

  // flush the file *************************************

  return(0);
}

Ogshow::OgshowFrameSeries::
OgshowFrameSeries() : showDir(0), frame(0), solutionCounter(0),
		      commentsSaved(false),movingGridProblem(false),numberOfFrames(0),frameNumber(0),
		      globalFrameNumber(0),
		      id(-1),sequenceNumber(0),streamMode(0),name(""),totalNumberOfFramesWhenCreated(0)
{
}

Ogshow::OgshowFrameSeries::
~OgshowFrameSeries() 
{
  //  if ( showDir ) { delete showDir; showDir = 0; }
  //  if ( frame ) { delete frame; frame = 0; }
}

//\begin{>>OgshowInclude.tex}{\subsubsection{newFrameSeries}} 
Ogshow::FrameSeriesID Ogshow::
newFrameSeries(const aString & name)
// ==============================================================================================
// /Description:
//     Create a new frame series with the given name.
// 
// /name (input) : name of a new frame series
// 
// /Return value: the frameSeriesID
//\end{OgshowInclude.tex}
// ==============================================================================================
{
  frameSeriesList.push_back(Ogshow::OgshowFrameSeries());
  OgshowFrameSeries &fs = frameSeriesList.back();
  fs.name = name;
  fs.id = frameSeriesList.size()-1;
  fs.totalNumberOfFramesWhenCreated = totalNumberOfFrames;
  fs.showDir = new HDF_DataBase();
  fs.frame = new HDF_DataBase();
  if (showDir->create(*fs.showDir,name,"frameSeries")!=0) 
    {
      cout<<"Ogshow::ERROR could not create directory for new frame series : "<<name<<endl;
      throw "error";
    }
  fs.streamMode = defaultStreamMode;
  if ( defaultStreamMode )
    {
      fs.showDir->setMode(GenericDataBase::normalMode);
      fs.frame->setMode(GenericDataBase::normalMode);
    }
  else
    {
      fs.showDir->setMode(GenericDataBase::noStreamMode);
      fs.frame->setMode(GenericDataBase::noStreamMode);
    }
  
  setCurrentFrameSeries(fs.id);
  return fs.id;
  
}

//\begin{>>OgshowInclude.tex}{\subsubsection{setCurrentFrameSeries}} 
int Ogshow::
getNumberOfFrameSeries() const
// ==============================================================================================
// /Description:
//   Return the number of frame series in this show file.
//\end{OgshowInclude.tex}
// ==============================================================================================
{
  return max(1,frameSeriesList.size());
}


//\begin{>>OgshowInclude.tex}{\subsubsection{getFrameSeriesID}} 
Ogshow::FrameSeriesID Ogshow::
getFrameSeriesID(const aString & name)
// ==============================================================================================
// /Description:
//   Return the FrameSeriesID corresponding to a given name.
// /name (input) : name of an existing frame series
// /Return value: -1 if the name was not found.
//\end{OgshowInclude.tex}
// ==============================================================================================
{
  vector<Ogshow::OgshowFrameSeries>::iterator fs = frameSeriesList.begin();
  for ( ;
	fs!=frameSeriesList.end();
	fs++ )
    if ( fs->name==name ) break;

  return fs==frameSeriesList.end() ? -1 : fs->id;
  
}

//\begin{>>OgshowInclude.tex}{\subsubsection{getFrameSeriesName}} 
const aString& Ogshow::
getFrameSeriesName(const FrameSeriesID frameSeries)
// ==============================================================================================
// /Description:
//   Return the name of a frame series in this show file.
// 
// /frameSeries (input) : a frame series ID.
// /Return value: A nullString if the frameSeries was not found.
// 
//\end{OgshowInclude.tex}
// ==============================================================================================
{
  if ( frameSeries<0 || (frameSeries>0 &&frameSeries>=frameSeriesList.size()) )
    return nullString;
//    return "INVALID FRAME SERIES NAME";
  else if ( frameSeries==0 && !frameSeriesList.size() )
    newFrameSeries("defaultFrameSeries");

  return frameSeriesList[frameSeries].name;
}

//\begin{>>OgshowInclude.tex}{\subsubsection{setFrameSeriesName}} 
int Ogshow::
setFrameSeriesName(const FrameSeriesID frameSeries, const aString & name)
// ==============================================================================================
// /Description:
//     Assign a new name to an existing frame series.
// 
// /frameSeries (input) : a frame series ID.
// /name (input) : name for the frame series
// /Return value: 0=success, 1=failure.
// 
//\end{OgshowInclude.tex}
// ==============================================================================================
{
  if ( frameSeries<0 || (frameSeries>0 &&frameSeries>=frameSeriesList.size()) )
  {
    printF("Ogshow::ERROR:setFrameSeriesName: frameSeries=%i does not exist\n",frameSeries);
    return 1;
  }
  printF("Ogshow::INFO:setFrameSeriesName: frameSeries=%i name=%s\n",frameSeries,(const char*)name);
  frameSeriesList[frameSeries].name=name;
  return 0;
}



//\begin{>>OgshowInclude.tex}{\subsubsection{getCurrentFrameSeries}} 
Ogshow::FrameSeriesID Ogshow::
getCurrentFrameSeries() const
// ==============================================================================================
// /Description:
//   Return the number of current frame series in this show file.
//\end{OgshowInclude.tex}
// ==============================================================================================
{
  return currentFrameSeries;
}

//\begin{>>OgshowInclude.tex}{\subsubsection{setCurrentFrameSeries}} 
int Ogshow::
setCurrentFrameSeries(const FrameSeriesID frameSeries)
// ==============================================================================================
// /Description:
//   Set the current frame series to the given ID.
// /frameSeries (input) : a frame series ID.
// /Return value: 1 if the frameSeries is not valid.
//\end{OgshowInclude.tex}
// ==============================================================================================
{
  if ( frameSeries<0 || (frameSeries>0 &&frameSeries>=frameSeriesList.size()) )
    return 1;
  currentFrameSeries = frameSeries;
  return 0;
}

//\begin{>>OgshowInclude.tex}{\subsubsection{setCurrentFrameSeries}} 
Ogshow::FrameSeriesID Ogshow::
setCurrentFrameSeries(const aString & name)
// =======================================================================================
// /Description:
//    Set the current frame series to be "name". Create a new frame series with this
//  name if it does not already exist.
// 
//\end{OgshowInclude.tex}
// =======================================================================================
{
  FrameSeriesID frameSeriesID=getFrameSeriesID(name);
  if( frameSeriesID==-1 )
  { // create a new frame series if it is not there
    frameSeriesID=newFrameSeries(name);
  }
  return setCurrentFrameSeries(frameSeriesID);
  
//  return setCurrentFrameSeries(getFrameSeriesID(name));
}
