//-----------------------------------------------------------------------------------------------
//  ogen: Overlapping Grid Generation
//
// This program can be used to create an overlapping grid. 
// The first step in making an overlapping grid is to define some
// Mappings that define the component grids. The second step is 
// to call the grid generator which will determine how the component
// grids overlap. The third step is to save the overlapping grid 
// in a file.
//
//-----------------------------------------------------------------------------------------------
#include "GL_GraphicsInterface.h"
#include "MappingInformation.h"
#include "HDF_DataBase.h"
#include "Ogen.h"
#include "Ugen.h"
#include "FortranIO.h"
#include "DataFormats.h"
#include "ShowFileReader.h"
#include "ParallelUtility.h"
#include "Integrate.h"

#define PRTPEG EXTERN_C_NAME(prtpeg)
extern "C"
{
  void PRTPEG(const int & io, const int & il, const int & ip, const int & ig, 
              const real & dr, const int & n, const int & n0, const int & n1 );
}

int createMappings( MappingInformation & mapInfo );

int 
ogen(MappingInformation & mappingInfo, GenericGraphicsInterface & ps, const aString & commandFileName, CompositeGrid *cgp/*=0*/ )
// =====================================================================================
// /Description:
//
// /mappingInfo (input): Use this parameter to pass any user defined mappings.
// /ps (input): Use this GenericGraphicsInterface object for plotting.
// /commandFileName (input): The name of a file from which commands should initially be read.
//            Use commandFileName="" if no command file is specified.
//
// ====================================================================================
{
//   ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
//   Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
    
  Overture::printMemoryUsage("ogen (start)");
  const int np = Communication_Manager::numberOfProcessors();
  

  GraphicsParameters psp;
  mappingInfo.graphXInterface=&ps;
    
  // here is the overlapping grid generator
  Ogen ogen(ps);

  printF("After create ogen...\n");
  fflush(0);
  Communication_Manager::Sync();
  

  Overture::printMemoryUsage("After create ogen");

  // here is the hybrid/unstructured grid generator
  Ugen ugen(ps);

  Overture::printMemoryUsage("After create ugen");

  // Here is the old popup menu
  aString menu[] = {
                    "!ogen",
                    "help",
                    "create mappings",
                    "generate an overlapping grid",
		    "generate a hybrid mesh",
                    "save a grid", 
                    "save a grid (compressed)", // compressed format is approx. 1-2 times smaller
                    "save a grid with arrays", // for parallel for now so we don't need to recompute the vertices etc.
                    // "save an overlapping grid or hybrid mesh",
                    "save grid in plot3d format",
		    "save grid in ingrid format",
		    "read a composite grid",
		    "minimum number of distributed ghost lines",
                    "maximum number of parallel sub-files",
                    "load balance",
		    "erase",
		    "exit",
                    "" };

  aString help[] = {
                    "help                     : print this list",
                    "create mappings          : create mappings used by the overlapping grid",
                    "generate an overlapping grid : compute interpolation points and holes",
		    "generate a hybrid grid : generate unstructured mesh in the regions of overlap in an overlapping grid",
                    // "save a grid : save an overlapping or hybrid grid", 
                    "save a grid (compressed): save a grid in compressed format.",
                    "save an overlapping grid or hybrid mesh : save an overlapping grid or hybrid mesh in a file",
                    "save a grid with arrays : for parallel for now so we don't need to recompute the vertices etc.",
                    "save grid in plot3d format: save grid in plot3d format",
		    "save grid in ingrid format: save the grid in an ascii file with an unstructured format",
		    "minimum number of distributed ghost lines : in parallel this must be >= interpolation-width/2",
                    "maximum number of parallel sub-files: used when saving files in parallel.",
		    "erase",
		    "exit",
                    "" };


  aString answer;

  GUIState dialog;
  dialog.setWindowTitle("Create Overlapping Grids");
  dialog.setExitCommand("exit", "exit");

  // option menus
//     dialog.setOptionMenuColumns(1);

//     aString opCommand1[] = {"unit square",
// 			    "helical wire",
//                             "fillet for two cylinders",
//                             "blade",
// 			    ""};
    
//     dialog.addOptionMenu( "type:", opCommand1, opCommand1, mappingType); 


//   aString colourBoundaryCommands[] = { "colour by bc",
// 			               "colour by share",
// 			               "" };
//   // dialog.addRadioBox("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );
//   dialog.addOptionMenu("boundaries:",colourBoundaryCommands, colourBoundaryCommands, 0 );


  aString cmds[] = {"create mappings",
                    "generate an overlapping grid",
		    "generate a hybrid mesh",
                    "save a grid", 
                    "save a grid (compressed)", // compressed format is approx. 1-2 times smaller
                    "save grid in plot3d format",
		    "save grid in ingrid format",
		    "read a composite grid",
		    "erase",
		    "help",
		    ""};

  int numberOfPushButtons=8;  // number of entries in cmds
  int numRows=numberOfPushButtons; // (numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  aString tbCommands[] = {"load balance",
                          "save integration weights",
  			  ""};
  int tbState[10];

  tbState[0] = ogen.loadBalanceGrids;
  bool saveIntegrationWeights=false;  // set to true if the integration weights should be saved with the grid.
  tbState[1] = saveIntegrationWeights;

  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=7;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];


  int nt=0;
  int minNumberOfDistributedGhostLines=MappedGrid::getMinimumNumberOfDistributedGhostLines();
  textLabels[nt] = "minimum number of distributed ghost lines:";  
  sPrintF(textStrings[nt],"%i",minNumberOfDistributedGhostLines);  nt++; 
  int maxParallelSubFiles=np;
  textLabels[nt] = "maximum number of parallel sub-files:";  sPrintF(textStrings[nt],"%i",maxParallelSubFiles);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


  // add the old popup
  dialog.buildPopup(menu);

  ps.pushGUI(dialog);



  CompositeGrid m;
  CompositeGrid &c = cgp ? *cgp : m;
    
  ps.appendToTheDefaultPrompt("ogen>");

  // read from a command file if given
  if( commandFileName!="" && commandFileName!="--")
    ps.readCommandFile(commandFileName);
    
  for(;;)
  {
    ps.getAnswer(answer,"");   // put up a menu and wait for a response
    // ps.getMenuItem(menu,answer);  

    if( answer=="create mappings" )
    {
      // create some mappings. The mappings are put into the mappingInfo.mappingList
      Overture::printMemoryUsage("Before create mappings");
      createMappings(mappingInfo);
      Overture::printMemoryUsage("After create mappings");
    }
    else if( answer=="save a grid" || answer=="save a grid (compressed)" ||
             answer=="save a grid with arrays" ||
             answer=="save an overlapping grid" ||               // old way, keep for compatibility
             answer=="save an overlapping grid or hybrid mesh" ) // old way, keep for compatibility
    {
      
      aString fileName,gridName;
      ps.inputString(fileName,"Enter the name of the file");
      for( ;; )
      {
	ps.inputString(gridName,"Save the grid under which name?");
	if( gridName=="." )
	  ps.outputString("Error: do not choose `.' as a name");
	else
	  break;
      }      
      HDF_DataBase dataFile;
      dataFile.mount(fileName,"I");

      int streamMode=1;
      if( answer=="save a grid (compressed)" )
        streamMode=1;  // save in compressed form.
      else
        streamMode=0;  // save in uncompressed form.

      dataFile.put(streamMode,"streamMode");
      if( !streamMode )
	dataFile.setMode(GenericDataBase::noStreamMode); // this is now the default
      else
      {
	dataFile.setMode(GenericDataBase::normalMode); // need to reset if in noStreamMode
      }
      
      // CompositeGrid cg2 = c;  // should I make a copy before destroying ?

      // first destroy any big geometry arrays: (but not the mask if we have more than 1 grid)
      // printf("number of component grids =%i\n",c.numberOfComponentGrids());
      
      if( answer!="save a grid with arrays" )
      {
	if( c.numberOfGrids() > 1 || c.numberOfInterpolationPoints(0)>0 )
	  c.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
	else
	  c.destroy(CompositeGrid::EVERYTHING);
      }
      
      real time0=getCPU();
      
      c.put(dataFile,gridName);

      real time = ParallelUtility::getMaxValue(getCPU()-time0);
      printF("Time to save grid %s was %g (seconds)\n",(const char*)fileName,time);
      
      // *wdh* 001004 c.destroy(CompositeGrid::EVERYTHING);  // we may need the mask later

      if( saveIntegrationWeights )
      {
        // compute and save the integration weights
        Integrate integrate(c);
        real cpu=getCPU();
        integrate.computeAllWeights();
        printF("Time to compute integration weights = %8.2e(s)\n",getCPU()-cpu);
	integrate.put(dataFile,"IntegrationWeights");
	printF("The integration weights were saved with the grid.\n");
      }

      dataFile.unmount();
    }
    else if( answer=="generate an overlapping grid" || answer=="check overlap" )
    {
      if( mappingInfo.mappingList.getLength() <= 0 )
	ps.outputString("INFO:There are no mappings created. You may want to create some first. \n"
                        "    : otherwise you can build a new overlapping grid by reading in an old one.\n");

      ogen.updateOverlap(c,mappingInfo);
      
    }
    else if( answer=="generate a hybrid mesh")
    {
      if( mappingInfo.mappingList.getLength() <= 0 )
	ps.outputString("INFO:There are no mappings created. You may want to create some first. \n"
                        "    : otherwise you can build a new hybrid mesh by reading in an old one.\n");

      ogen.turnOnHybridHoleCutting();
      ogen.updateOverlap(c,mappingInfo);
      ugen.updateHybrid(c,mappingInfo);
    }
    else if (answer=="read a composite grid")
    {
      printf("INFO: To read a grid from a show file, the file name should end in `.show'\n");

      ps.inputString(answer,"Enter the name of the composite grid");
      if( answer!="" )
      {
	const int len=answer.length();
	if( answer(len-5,len-1)==".show" )
	{
	  // *** this is a show file ****

          ShowFileReader showFileReader(answer);

	  int numberOfFrames=showFileReader.getNumberOfFrames();
	  int numberOfSolutions = max(1,numberOfFrames);
	  int solutionNumber=numberOfSolutions;  // use last

	  ps.inputString(answer,sPrintF("Enter the solution to use, from 1 to %i (-1=use last)",
					numberOfSolutions));

	  if( answer!="" )
	  {
	    sScanF(answer,"%i",&solutionNumber);
	    if( solutionNumber<0 || solutionNumber>numberOfSolutions )
	    {
	      solutionNumber=numberOfSolutions;
	    }
	  }
	  showFileReader.getAGrid(c,solutionNumber);     
	}
	else
	{
	  getFromADataBase(c,answer);
	}
	
	// add the mappings from the composite grid to mapInfo
	for ( int g=0; g<c.numberOfGrids(); g++ )
	{
	  mappingInfo.mappingList.addElement(c[g].mapping());
	}
      }
    }
    else if( answer=="save grid in plot3d format" )
    {
      // write data into unformatted files suitable for overflow 
      // *notes* overflow wants unformatted files, cell centred, with two fringe lines
      // 2D grids should be 3D with 1 cell in the i3 direction.
      printf("write data into unformatted files suitable for the overflow solver\n"
             "oveflow wants a cell centred grid with 2 lines of interpolation so the grid should be\n"
             "built with discretization width=5 (from the `change parameters menu')\n"
             "Both a plot3d format file (no iblank) and a PEGSUS style `fort.2' file holding\n"
             "the interpolation data will be created\n");
      

      aString fileName;
      ps.inputString(fileName,"Enter the name for the plot3d output file");

#if 1
      DataFormats::writePlot3d(c, fileName,"fort.2");
#else
      FortranIO fio;
      fio.open((const char*)fileName,"unformatted","unknown");
      fio.print(c.numberOfComponentGrids());

      Index I1,I2,I3;
      int grid,axis;
      IntegerArray nxyz(3,max(1,c.numberOfComponentGrids()));
      nxyz=1;
      for( grid=0; grid<c.numberOfComponentGrids(); grid++ )
      {
	const IntegerArray & gridIndexRange = c[grid].gridIndexRange();
	for( axis=0; axis<c.numberOfDimensions(); axis++ )
	  nxyz(axis,grid)=gridIndexRange(End,axis)-gridIndexRange(Start,axis)+1;
      }
      
      fio.print(nxyz);
      
      for( grid=0; grid<c.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & g = c[grid];

        // The grid is always treated as 3D
        getIndex(g.gridIndexRange(),I1,I2,I3);
        Range Rx(0,c.numberOfDimensions()-1);
        realArray x(I1,I2,I3,3);
        x=0.;
	x(I1,I2,I3,Rx)=g.vertex()(I1,I2,I3,Rx);
        fio.print(x);

      }
      fio.close();

      // now save the PEGSUS file
      const int unit=20;
      fio.open("fort.2","unformatted","unknown",unit);
      int nStart=1, nEnd=0;
      for( grid=0; grid<c.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & g = c[grid];
    
	intArray & ip = c.interpolationPoint[grid];   
	intArray & il = c.interpoleeLocation[grid];
	intArray & ig = c.interpoleeGrid[grid];

	Range R(0,c.numberOfInterpolationPoints(grid)-1);
	RealArray dr(R,3);
	for( int i=0; i<c.numberOfInterpolationPoints(grid); i++ )
	{
	  int gridi = c.interpoleeGrid[grid](i);
          assert( gridi>=0 && gridi<c.numberOfComponentGrids() );
	  
	  MappedGrid & cgridi = c[gridi];
	  for( axis=0; axis<c.numberOfDimensions(); axis++ )
	  {
	    int indexPosition=c.interpoleeLocation[grid](i,axis);
	    real relativeOffset=c.interpolationCoordinates[grid](i,axis)/cgridi.gridSpacing(axis)
	      +cgridi.indexRange(Start,axis);
	    dr(i,axis)= cgridi.isCellCentered(axis)  ? relativeOffset-indexPosition-.5 
	      : relativeOffset-indexPosition;
	  }
	}
        nEnd=nStart+c.numberOfInterpolationPoints(grid)-1;

	PRTPEG( unit,*il.getDataPointer(),*ip.getDataPointer(),*ig.getDataPointer(),
		*dr.getDataPointer(),c.numberOfInterpolationPoints(grid),nStart,nEnd );
	
        nStart=nEnd+1;
      }
      fio.close();
#endif
      
    }
    else if( answer=="save grid in ingrid format" )
    {
      aString fileName;
      ps.inputString(fileName,"Enter the name for the ingrid output file");

      c.update(MappedGrid::THEvertex | MappedGrid::THEmask); // this may have deleted
      DataFormats::writeIngrid(c, fileName);

    }
    else if( dialog.getTextValue(answer,"maximum number of parallel sub-files:","%i",maxParallelSubFiles) ||
             answer=="maximum number of parallel sub-files" )
    {
      printF("Info:This option will apply to any grids that are subsequently saved\n");
      if( answer=="maximum number of parallel sub-files" ) // *old* way 
      {
	maxParallelSubFiles=np;
	ps.inputString(answer,sPrintF("Enter the maximum number of parallel sub-files to save."));
	sScanF(answer,"%i",&maxParallelSubFiles);
      }
      
      printF("maximum number of parallel sub-files =%i.\n",maxParallelSubFiles);
      GenericDataBase::setMaximumNumberOfFilesForWriting(maxParallelSubFiles);
    }
    else if( dialog.getTextValue(answer,"minimum number of distributed ghost lines:","%i",minNumberOfDistributedGhostLines) ||
             answer=="minimum number of distributed ghost lines" )  // old way 
    {
      printF("The minimum number of distributed ghost lines should greater than width/2, "
             " where width is the interpolation-width that will be used to build the overlapping grid.\n");

      if( answer=="minimum number of distributed ghost lines" )  // old way 
      {
	ps.inputString(answer,sPrintF("Enter the minimum number of distributed ghost lines (current=%i)",
				      MappedGrid::getMinimumNumberOfDistributedGhostLines()));
        minNumberOfDistributedGhostLines=-1;
	sScanF(answer,"%i",&minNumberOfDistributedGhostLines);
      }
      
      if( minNumberOfDistributedGhostLines>=0 )
      {
	printF("Setting the minimum number of distributed ghost lines to %i\n",minNumberOfDistributedGhostLines);
	MappedGrid::setMinimumNumberOfDistributedGhostLines(minNumberOfDistributedGhostLines);
      }
      else
      {
	printF("ERROR: Invalid value for the minimum number of distributed ghost lines = %i\n",
	       minNumberOfDistributedGhostLines);
	ps.stopReadingCommandFile();
      }
	
    }
    else if( dialog.getToggleValue(answer,"load balance",ogen.loadBalanceGrids) )
    {
      if( ogen.loadBalanceGrids )
	printF("load balance grids\n");
      else
	printF("Do NOT load balance grids\n");
    }
    else if( dialog.getToggleValue(answer,"save integration weights",saveIntegrationWeights) )
    {
      if( saveIntegrationWeights )
	printF("The integration weights will be saved when the overlapping grid is saved.\n");
      else
	printF("The integration weights will NOT be saved when the overlapping grid is saved.\n");

    }
    else if( answer=="load balance" ) // old way 
    {
      ogen.loadBalanceGrids=true;
      printF("load balance grids\n");
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        ps.outputString(help[i]);
    }
    else if( answer=="erase" )
    {
      ps.erase();
    }
    else if( answer=="exit" )
    {
      break;
    }
    else
    {
      printF("Unknown response =[%s]\n",(const char*) answer);
      ps.stopReadingCommandFile();
    }
  }

/* ---
  CompositeGrid c2;
  dataFile.mount("ogen.hdf", "R");
  c2.get(dataFile, "myGrid");
  dataFile.unmount();
  c2.update();
  PlotIt::plot(ps,c2, psp );
---- */

  ps.unAppendTheDefaultPrompt(); // reset defaultPrompt
  ps.popGUI(); // restore the previous GUI

  return 0;
}
