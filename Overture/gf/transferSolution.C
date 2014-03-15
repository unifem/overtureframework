//=================================================================================================
//  transferSolution:
//   Transfer (interpolate) a solution from one overlapping grid to another overlapping grid
//
//   This function can be used for example to interpolate a coarse grid solution onto a fine grid.
//
//=================================================================================================
#include "Overture.h"
#include "Ogshow.h"  
#include "ShowFileReader.h"
// #include "interpPoints.h"
#include "display.h"
// #include "FortranIO.h"
#include "PlotStuff.h"
// #include "InterpolatePoints.h"
// #include "gridFunctionNorms.h"
#include "ParallelUtility.h"
#include "InterpolatePointsOnAGrid.h"

// #include OV_STD_INCLUDE(vector)

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


// struct ComponentVector
// {
// aString name;
// std::vector<int> component;
// };


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np=Communication_Manager::numberOfProcessors();

  printF("transferSolution: A program to transfer (interpolate) a solution from one overlapping grid to\n"
         "                  another overlapping grid. Type 'help' for further info.\n");

  bool plotOption=true;
  bool closeShowAfterUse=true;

  aString commandFileName="";
  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" || line=="noplot" )
        plotOption=false;
      else if( commandFileName=="" )
      {
        commandFileName=line;    
        printF("transferSolution: reading commands from file [%s]\n",(const char*)commandFileName);
      }
      
    }
  }

  aString sourceShowFileName="sourceShowFile.show";
  aString targetGridFileName="targetGrid.hdf";
  aString targetShowFileName="targetShowFile.show";

  ShowFileReader sourceShowFileReader;
  int numberOfSolutions=0;
  int solutionNumber=-1;
  bool sourceShowFileOpen=false;
  bool sourceWasRead=false;
  
  Ogshow targetShowFile;
  bool targetWasRead=false;

  CompositeGrid cgSource, cgTarget;
  realCompositeGridFunction uSource, uTarget;
  int numberOfComponents=1;

  real time=0.; // source solution is at this time

  int interpolationWidth=3;  // interpolate to new grid using this width
  int numGhostToUse=2;       // We can use this many ghost points in the source
  int numGhostToSet=2;       // Set this many ghost points in the target
  
  GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("transferSolution",plotOption,argc,argv);
  PlotStuffParameters psp;           // create an object that is used to pass parameters
    
  // By default start saving a command file:
  aString logFile="transferSolution.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);


  ps.appendToTheDefaultPrompt("transferSolution>");

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file =%s\n",(const char*)commandFileName);
    ps.readCommandFile(commandFileName);
  }

  // Keep track of which frame series (i.e. domain) to use from the source show file 
  int frameSeries=0;


// ---------------------------------------------------------------------------------------------
  GUIState dialog;

  dialog.setWindowTitle("Transfer Solution");
  dialog.setExitCommand("exit","Exit");


//   dialog.setOptionMenuColumns(1);

//   enum LinerTypeEnum
//   {
//     linearLiner,
//     quadraticLiner,
//     freeFormLiner
//   } linerType=linearLiner;
  
//   aString linerTypeCommands[] = {"linear liner...", "quadratic liner...", "free form liner...", "" };
//   dialog.addOptionMenu("method:", linerTypeCommands, linerTypeCommands, (int)linerType );


  aString pushButtonCommands[] = {"read source",
                                  "transfer solution",
                                  "save show file",
                                  "plot source",
                                  "plot grid source",
                                  "plot target",
                                  "plot grid target",
                                  "help",
				  ""};
  int numRows=4;
  dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 

//  aString tbCommands[] = {"convert to primitive variables",
//                          "convert to vertex centered",
// 			  ""};
//  bool convertToPrimitive=true;
//  bool convertToVertexCentered=false;
//  
//  int tbState[10];
//  tbState[0] = convertToPrimitive;
//  tbState[1] = convertToVertexCentered;
//  
//
//  int numColumns=1;
//  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

// **  dialog.addInfoLabel("Volume = 0");

  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];


  int nt=0;
  textCommands[nt] = "source show file";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s", (const char*)sourceShowFileName);  nt++; 

  textCommands[nt] = "solution number";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i (-1 = use last)", solutionNumber);  nt++; 


  textCommands[nt] = "target grid file";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)targetGridFileName);  nt++; 

  textCommands[nt] = "target show file";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)targetShowFileName);  nt++; 

  textCommands[nt] = "interpolation width";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i",interpolationWidth);  nt++; 

  textCommands[nt] = "number of ghost to use";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i",numGhostToUse);  nt++; 

  textCommands[nt] = "number of ghost to set";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i",numGhostToSet);  nt++; 

  // null strings terminate list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  dialog.addInfoLabel(sPrintF("solution %i (%i)",solutionNumber,numberOfSolutions));

  ps.pushGUI(dialog);

  aString answer,buff;
  int len=0;

  for( int it=0;; it++ )
  {
     
   ps.getAnswer(answer, "");
   
   int len;
   if( answer=="exit" )
   {
     break;
   }
   else if( dialog.getTextValue(answer,"source show file","%s",sourceShowFileName) )
   {
     printF("Opening the show file named=[%s]\n",(const char*)sourceShowFileName);
     
     sourceShowFileReader.open(sourceShowFileName);
     sourceShowFileOpen=true;

     numberOfSolutions=sourceShowFileReader.getNumberOfFrames();
     printF("There are %i solutions in the show file\n",numberOfSolutions);

     dialog.setInfoLabel(0,sPrintF("solution %i (%i)",solutionNumber,numberOfSolutions));
   }
   else if( dialog.getTextValue(answer,"solution number","%i",solutionNumber) )
   {
     sScanF(answer(len,answer.length()-1),"%i",&solutionNumber);
     dialog.setInfoLabel(0,sPrintF("solution %i (%i)",solutionNumber,numberOfSolutions));
   }
   else if( dialog.getTextValue(answer,"interpolation width","%i",interpolationWidth) ){}  //
   else if( dialog.getTextValue(answer,"number of ghost to use","%i",numGhostToUse) ){}  //
   else if( dialog.getTextValue(answer,"number of ghost to set","%i",numGhostToSet) ){}  //
   else if( answer=="read source" )
   {
     if( !sourceShowFileOpen )
     {
       printF("You should specify the `source show file' before attempting to read a solution\n");
       continue;
     }

     sourceShowFileReader.getASolution(solutionNumber,cgSource,uSource);   // read in a grid and solution
     numberOfComponents=uSource.getComponentDimension(0); 

     HDF_DataBase & db = *(sourceShowFileReader.getFrame());
     db.get(time,"time");  
     printF(" solutionNumber=%i, time=%20.12e \n",solutionNumber,time);

     sourceWasRead=true;
     
   }   
   else if( dialog.getTextValue(answer,"target grid file","%s",targetGridFileName) )
   {
     bool loadBalance=true;
     int rt = getFromADataBase(cgTarget,targetGridFileName,loadBalance);
     targetWasRead=true;

     Range all;
     uTarget.updateToMatchGrid(cgTarget,all,all,all,numberOfComponents);
     for( int c=0; c<numberOfComponents; c++ )
     {
       uTarget.setName(uSource.getName(c),c);
     }
     
     uTarget=0.;
   }
   else if( dialog.getTextValue(answer,"target show file","%s",targetShowFileName) ){}//
   else if( answer=="transfer solution" )
   {
     printF("\n>>> Use InterpolatePointsOnAGrid, interpolationWidth=%i, numGhostToUse=%i, numGhostToSet=%i\n\n",
	    interpolationWidth,numGhostToUse,numGhostToSet );
     InterpolatePointsOnAGrid interpolator;
     interpolator.setInfoLevel( 1 );
     interpolator.setInterpolationWidth(interpolationWidth);
     // Set the number of valid ghost points that can be used when interpolating from a grid function: 
     interpolator.setNumberOfValidGhostPoints( numGhostToUse );
      
     // Assign all points, extrapolate pts if necessary:
     interpolator.setAssignAllPoints(true);

     Range C(uSource.getComponentBase(0),uSource.getComponentBound(0));

     real time0=getCPU();
     int num=interpolator.interpolateAllPoints( uSource,uTarget,C,C,numGhostToSet);    
     real time=getCPU()-time0;
     time=ParallelUtility::getMaxValue(time);
     printF(" ... time to interpolate = %8.2e(s)\n",time);

   }
   else if( answer=="save show file" )
   {
     printf("Saving the target grid and solution to the show file [%s]\n",(const char*)targetShowFileName);
     
     Ogshow show(targetShowFileName);
     show.saveGeneralComment("Grid and solution from transferSolution"); 

     // We copy the show file parameters from the source show file to the target show file
     // This list may hold definitions of the different component numbers (e.g. densityComponent)
     ListOfShowFileParameters &showFileParams = sourceShowFileReader.getListOfGeneralParameters();
     show.saveGeneralParameters(showFileParams);
       
//      ListOfShowFileParameters showFileParams;
//      showFileParams.push_back(ShowFileParameter("reynoldsNumber",reynoldsNumber));
//      showFileParams.push_back(ShowFileParameter("machNumber",machNumber));
//      showFileParams.push_back(ShowFileParameter("gamma",gamma));
//      showFileParams.push_back(ShowFileParameter("Rg",Rg));

//      int rc=0, uc=1, vc=2, wc=3, tc=4, pc=5;   // **************************** fix this 
      
//      showFileParams.push_back(ShowFileParameter("densityComponent",rc));
//      showFileParams.push_back(ShowFileParameter("temperatureComponent",tc));
//      showFileParams.push_back(ShowFileParameter("pressureComponent",pc));
//      showFileParams.push_back(ShowFileParameter("uComponent",uc));
//      showFileParams.push_back(ShowFileParameter("vComponent",vc));
//      showFileParams.push_back(ShowFileParameter("wComponent",wc));

//      int numberOfSpecies=0;
//      showFileParams.push_back(ShowFileParameter("numberOfSpecies",numberOfSpecies));

//      show.saveGeneralParameters(showFileParams);

     show.startFrame();                                         // start a new frame
     show.saveComment(0,"solution from transferSolution");   // comment 0 (shown on plot)
     show.saveSolution( uTarget ); 

     // Save the "time" in the show file:
     HDF_DataBase & db = *(show.getFrame());
     db.put(time,"time");

     show.close();
   }
   else if( answer=="plot source" )
   {
     if( !sourceWasRead )
     {
       printF("You should specify the `read source' before attempting to plot\n");
       continue;
     }
     psp.set(GI_TOP_LABEL,sPrintF(buff,"source, t=%9.3e",time));
     ps.erase();
     PlotIt::contour(ps,uSource,psp);
   }
   else if( answer=="plot grid source" )
   {
     if( !sourceWasRead )
     {
       printF("You should specify the `read source' before attempting to plot\n");
       continue;
     }
     psp.set(GI_TOP_LABEL,sPrintF(buff,"source grid, t=%9.3e",time));
     ps.erase();
     PlotIt::plot(ps,cgSource,psp);
   }
   else if( answer=="plot target" )
   {
     if( !targetWasRead )
     {
       printF("You should specify the `read target' before attempting to plot\n");
       continue;
     }
     psp.set(GI_TOP_LABEL,sPrintF(buff,"target, t=%9.3e",time));
     ps.erase();
     PlotIt::contour(ps,uTarget,psp);
   }
   else if( answer=="plot grid target" )
   {
     if( !targetWasRead )
     {
       printF("You should specify the `read target' before attempting to plot\n");
       continue;
     }
     psp.set(GI_TOP_LABEL,sPrintF(buff,"target grid, t=%9.3e",time));
     ps.erase();
     PlotIt::plot(ps,cgTarget,psp);
   }
   else if( answer=="help" )
   {
     printF("------------- transferSolution ---------------------\n"
            " Transfer (interpolate) a solution from one overlapping grid to another overlapping grid.\n"
            " This function can be used for example to interpolate a coarse grid solution onto a fine grid.\n"
            "\n"
            "Steps:\n"
            "  1. Specify the source solution from a show file:\n"
	    "     1(a) Set the name of the source show file : 'source show file'. \n"
            "     1(b) Choose the solution number: 'solution number'.\n"
            "     1(c) Read in the source solution: 'read source'.\n"
            "  2. Specify the target grid: 'target grid file'. \n"
            "  3. Transfer (interpolate) the source solution to the target grid: 'transfer solution'.\n"
            "     (Before doing this you can specify how many ghost lines to use in the source solution,\n"
            "     and how many ghost lines to set in the target solution.)\n"
            "  4. Save the target solution to a show file:\n"
            "     4(a) Name the target show file: 'target show file'.\n"
            "     4(b) Save the show file: 'save show file'.\n"
       );
     
   }
   else 
   {
     printF("transferSolution:ERROR: unknown response=[%s]\n",(const char*)answer);
     ps.stopReadingCommandFile();
   }
  }
  

  Overture::finish();          
  return 0;
}



/* ================================================


// ---------------------------------------------------------------------------------------------

  aString answer,answer2;
  aString menu[] = {"specify files (coarse to fine)",
                    "choose a solution",
                    "choose a solution for each file",
                    "compute errors",
                    "plot solutions",
                    "plot differences",
                    "save differences to show files",
                    "define a vector component",
                    "delete all vector components",
                    "interpolation width",
                    "enter components to use per file",
                    "output file name",
                    "choose a frame series (domain) per file",
                    // "do not check boundaries (toggle)",
                    // "output ascii files",
                    // "output binary files",
		    "exit",
                    "" };
  char buff[80];
  int interpolationWidth=3;
  
  for(;;)
  {
    ps.getMenuItem(menu,answer);
    if( answer=="specify files (coarse to fine)" )
    {
      for( int i=0; i<maxNumberOfFiles; i++ )
      {
	ps.inputString(fileName[numberOfFiles],"Enter the file name (`exit' to finish)");
	if( fileName[numberOfFiles]=="" || fileName[numberOfFiles]=="exit" )
          break;
	showFileReader[numberOfFiles].open(fileName[numberOfFiles]);
	numberOfSolutions[numberOfFiles]=showFileReader[numberOfFiles].getNumberOfFrames();
	maxSolution[numberOfFiles]=min(maxSolution[numberOfFiles],numberOfSolutions[numberOfFiles]);
 
	if( closeShowAfterUse )
	  showFileReader[i].close();


	numberOfFiles++;
      }
    }
    else if( answer=="choose a frame series (domain) per file" )
    {
      if( numberOfFiles<=0 )
      {
	printF("You should choose files first.\n");
	continue;
      }      
      aString line;
      frameSeries.redim(numberOfFiles);
      frameSeries=0;
      
      for( int i=0; i<numberOfFiles; i++ )
      {
        int numberOfFrameSeries=max(1,showFileReader[i].getNumberOfFrameSeries());
        printF(" File: %i has %i frame series (domains):\n",i,numberOfFrameSeries);
        for( int fs=0; fs<numberOfFrameSeries; fs++ )
	{
          printF(" frame series %i: %s\n",fs,(const char*)showFileReader[i].getFrameSeriesName(fs));
	}
	
	ps.inputString(line,sPrintF(buff,"Enter the number of the frame series to use, (0,...,%i).",numberOfFrameSeries-1));
        int fs=-1;
	sScanF(line,"%i",&fs);
	if( fs<0 || fs>numberOfFrameSeries )
	{
	  printF("ERROR: frame series %i is NOT valid. Will use 0\n",fs);
	  fs=0;
	}
        frameSeries(i)=fs;  // save
	showFileReader[i].setCurrentFrameSeries(fs);
      }
      
    }
    else if( answer=="enter components to use per file" )
    {
      if( numberOfFiles<=0 )
      {
	printF("You should choose files first\n");
	continue;
      }
      
      numComponentsPerFile.redim(numberOfFiles);
      componentsPerFile.redim(numberOfFiles,maxNumberOfComponents);
      componentsPerFile=-1;
      aString line;
      for( int i=0; i<numberOfFiles; i++ ) 
      {
        numComponentsPerFile(i)=0;
        int c[15];
	for( int j=0; j<15; j++ ) c[j]=-1;
	ps.inputString(line,sPrintF("Enter a list of components to use for file %i",i));
	sScanF(line,"%i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i",
               &c[0],&c[1],&c[2],&c[3],&c[4],
               &c[5],&c[6],&c[7],&c[8],&c[9],
               &c[10],&c[11],&c[12],&c[13],&c[14]);
        for( int j=0; j<15; j++ )
	{
	  if( c[j]>=0 )
	  {
            numComponentsPerFile(i)++;
            componentsPerFile(i,j)=c[j];
	  }
	  else
	  {
	    break;
	  }
	}
      }
    }
    else if( answer=="choose a solution" ||
             answer=="choose a solution for each file" )
    {
      // In this case the user is asked to choose a solution to read in
      // Choosing a number that is too large will cause the last solution to be read 

      aString line;
      if( answer=="choose a solution" )
      {
	ps.inputString(line,sPrintF(buff,"Enter the solution number to read, in [1,%i] \n",maxSolution[0]));
	sScanF(line,"%i",&solutionNumber[0]);
        for( int i=1; i<maxNumberOfFiles; i++ ) solutionNumber[i]=solutionNumber[0];
      }
      else
      {
        for( int i=0; i<numberOfFiles; i++ ) 
          printF(" File %i has solutions [1,%i]\n",i,maxSolution[i]);
	ps.inputString(line,"Enter separate solution number of each file\n");
	assert( maxNumberOfFiles<15 );
	sScanF(line,"%i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i",
               &solutionNumber[0],&solutionNumber[1],&solutionNumber[2],&solutionNumber[3],&solutionNumber[4],
               &solutionNumber[5],&solutionNumber[6],&solutionNumber[7],&solutionNumber[8],&solutionNumber[9],
               &solutionNumber[10],&solutionNumber[11],&solutionNumber[12],&solutionNumber[13],&solutionNumber[14]);
      }
      

      for( int i=0; i<numberOfFiles; i++ )
      {
        if( closeShowAfterUse )
	{
          int displayInfo=0; // do not print header info etc.
	  showFileReader[i].open(fileName[i],displayInfo);
          if( i <= frameSeries.getBound(0) )
            showFileReader[i].setCurrentFrameSeries(frameSeries(i));

	}
	real timea=getCPU();
        showFileReader[i].getASolution(solutionNumber[i],cg[i],u[i]);        // read in a grid and solution
        timea=getCPU()-timea; timea=ParallelUtility::getMaxValue(timea);
	
	if( numComponentsPerFile.getLength(0)==numberOfFiles )
	{
	  // User has specified a subset of components to use per file
          // Make a new grid function with just these components. 
          int nc = numComponentsPerFile(i);
          Range all;
	  realCompositeGridFunction v(cg[i],all,all,all,nc);
	  for( int c=0; c<nc; c++ )
	  {
            int cc=componentsPerFile(i,c);
	    if( cc<u[i].getComponentBase(0) || cc>u[i].getComponentBound(0) )
	    {
              printF("ERROR: component chosen is out of bounds: file=%i component=%i valid=[%i,%i]\n"
                     "  Changing component to %i\n",
		     i,cc,u[i].getComponentBase(0),u[i].getComponentBound(0),u[i].getComponentBound(0));
              cc=u[i].getComponentBound(0);
	      componentsPerFile(i,c)=cc;
	    }
            printF(" Choosing component %i for file %i\n",cc,i);
	    v.setName(u[i].getName(cc),c);
	  }
	  
	  for( int grid=0; grid<cg[i].numberOfComponentGrids(); grid++ )
	  {
            #ifdef USE_PPP
	      realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v[grid],vLocal);
	      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[i][grid],uLocal);
	    #else
	      realSerialArray &vLocal = v[grid];
	      realSerialArray &uLocal = u[i][grid];
            #endif
	    
            for( int c=0; c<nc; c++ )
	    {
              const int cc=componentsPerFile(i,c);
              assert( cc>=u[i].getComponentBase(0) && cc<=u[i].getComponentBound(0) );
	      vLocal(all,all,all,c)=uLocal(all,all,all,cc);

	    }
	    
	  }
          u[i].destroy();
	  u[i].reference(v);
          
	}
	


        HDF_DataBase & db = *(showFileReader[i].getFrame());
        db.get(time(i),"time");  
        printF(" file[%i] solutionNumber=%i time=%20.12e (%8.2e(s) to read)\n",i,solutionNumber[i],time(i),timea);

        if( outFile==NULL )
	{
	  if( myid==0 )
	    outFile=fopen((const char*)outputFileName,"w" );
	  printF("Output being saved in file %s\n",(const char*)outputFileName);

	}
	

        fPrintF(outFile,"Choosing solution %i, t=%9.3e,  from showFile=%s\n",solutionNumber[i],time(i),(const char*)fileName[i]);
	

        if( closeShowAfterUse )
	  showFileReader[i].close();

        cg[i].update(MappedGrid::THEmask );
        // cg[i].update(MappedGrid::THEmask | MappedGrid::THEcenter | MappedGrid::THEvertex |
	//	     MappedGrid::THEinverseVertexDerivative);
	
      }
      timesMatch=true;
      maxTimeDiff=0.;
      for( int i=1; i<numberOfFiles; i++ )
      {
        if( fabs(time(i)-time(i-1)) > REAL_EPSILON*max(fabs(time(i)),fabs(time(i-1)))*100. )
	{
          timesMatch=false;

	  printF("***************ERROR: The times of the solutions do not match! ****************\n"
		 "   times=");
	  for( int ii=0; ii<numberOfFiles; ii++ )
	  {
            maxTimeDiff=max(maxTimeDiff,fabs(time(ii)-time(max(ii-1,0))));
	    
	    printF("%12.6e, ",time(ii));
	  }
          
	  printF("\n ***** Max difference in times = %8.2e ****\n",maxTimeDiff);
	  printF("\n *********************************************************************************\n");

          break;
	}
      }
    }
    else if( answer=="output file name" )
    {
      ps.inputString(outputFileName,"Enter the name of the output file (default is comp.log)");
      printF("Output file will be named [%s]\n",(const char*)outputFileName);
    }
    else if( answer=="interpolation width" )
    {
      ps.inputString(answer,sPrintF("Enter the interpolation width (2,3,4,...), (current=%i)",interpolationWidth));
      sScanF(answer,"%i",&interpolationWidth);
      printF("Setting interpolationWidth=%i\n",interpolationWidth);
    }
    else if( answer=="define a vector component" )
    {
      printF("Define a vector of components for which errors are estimated (e.g. the velocity vector)\n");
      ComponentVector v;
      ps.inputString(v.name,"Enter the name of the vector");
      IntegerArray c;
      int cmin=0; // minimum component number
      int nvc=ps.getValues("Enter the component numbers (enter `done' when finished)",c,cmin);
      for( int i=0; i<nvc; i++ )
      {
	v.component.push_back(c(i));
      }
      componentVector.push_back(v);
      printF(" Vector %s is defined from components ",(const char*)v.name);
      for( int i=0; i<v.component.size(); i++ ) printF("%i, ",v.component[i]);
      printF("\n");
      
    }
    else if( answer=="delete all vector components" )
    {
      componentVector.clear();
    }
    else if( answer=="output ascii files" )
    {
      aString outputFileName;
      ps.inputString(outputFileName,"Enter the output file name ");
      FILE *file;
      for( int i=0; i<numberOfFiles; i++ )
      {
        for( int solution=1; solution<=maxSolution[i]; solution++ )
	{
	  showFileReader[i].getASolution(solution,cg[i],u[i]);        // read in a grid and solution
	  HDF_DataBase & db = *(showFileReader[i].getFrame());
	  db.get(time(i),"time");   // *wdh* 091109 -- change "t" to "time"
	  

	  MappedGrid & mg = cg[i][0];
	  realMappedGridFunction & v = u[i][0];
	
	  const IntegerArray & gridIndexRange = cg[i][0].gridIndexRange();
          if( solution==1 )
	  {
  	    int n=gridIndexRange(End,axis1)-gridIndexRange(Start,axis1);
	    aString name;
	    name=sPrintF(buff,"%s%i.dat",(const char*)outputFileName,n);
	    printf("save file %s \n",(const char*)name);
	    file = fopen((const char*)name,"w");
	  }
	  DisplayParameters dp;
	  dp.set(file);
	  dp.set(DisplayParameters::labelNoIndicies);
	  Index I1,I2,I3;
	  getIndex(cg[i][0].gridIndexRange(),I1,I2,I3);
	  for( int c=v.getComponentBase(0); c<=v.getComponentBound(0); c++ )
	  {
	    cout << "component Name = [" << u[i].getName(c) << "]\n";
	  
	    fprintf(file,"%s\n",(const char*)u[i].getName(c));
            fprintf(file,"%e (time)\n",time(i));
	    fprintf(file,"%i %i %i\n",I1.getBound()-I1.getBase()+1,
		    I2.getBound()-I2.getBase()+1,
		    I3.getBound()-I3.getBase()+1);
	    display(u[i][0](I1,I2,I3,c),NULL,dp);
	  }
	}
	fclose(file);
      }
    }
    else if( answer=="compute errors" )
    {
      if( solutionNumber[0]<0 )
      {
	printf(" You should `choose a solution' before computing errors\n");
	continue;
      }

      errorsComputed=true;
      

      Index I1,I2,I3, J1,J2,J3;
      const int n =numberOfFiles-1;  // finest grid

      // **** determine relative mesh spacings here ***
      // Assume the base grids are basically the same but with different numbers of grid points.
      // There may also be different numbers of refinement levels.
      for( int i=0; i<numberOfFiles; i++ )
      {
        h(i)=cg[i][0].gridSpacing(axis1);    // grid spacing on grid 0
	if( cg[i].numberOfRefinementLevels()>1 )
	{
	  int nl=cg[i].numberOfRefinementLevels();
	  int rf=cg[i].refinementLevel[nl-1].refinementFactor(0,0);
	  // NOTE: rf is the refinement factor to the base grid.
	  // printf(" numberOfLevels=%i refinementFactor=%i\n",nl,rf);
	  h(i)/=rf;
	}
      }
      
      for( int io=0; io<=1; io++ )
      {
	FILE *file = io==0 ? stdout : outFile;
        for( int i=0; i<numberOfFiles; i++ )
	  fPrintF(file," File %i, solution=%i, t=%9.3e, grid 0: dr = %10.3e , ratio to fine grid = %8.4f\n",
		  i,solutionNumber[i],time(i),h(i),h(i)/h(n));
      }
      fflush(outFile);

      realCompositeGridFunction & vn = u[n];  // Here is the fine grid solution 
      for( int i=0; i<n; i++ )
      {
	const realCompositeGridFunction & v = u[i];
        ud[i].updateToMatchGridFunction(v);
        ud[i]=0.;
        
        printF("\n >> Interpolate the fine grid solution onto the grid from file %i...\n",i);

        // This next call can be used to call the old or new method
	if( useOldWay )
	{
	  printF("\n +++++++++++++++++++++ USE OLD INTERP ++++++++++++++++++++++\n\n");
	  bool useNewWay=false;
          real time0=getCPU();
	  interpolateAllPoints( vn,ud[i],useNewWay );  // interpolate ud[i] from fine grid solution vn
	  real time=getCPU()-time0;
	  time=ParallelUtility::getMaxValue(time);
	  printF(" ... time to interpolate = %8.2e(s)\n",time);
	}
	else if( useNewWay )
	{
	  // *new way*
	  printF("\n +++++++++++++++++++++ USE NEW INTERP ++++++++++++++++++++++\n\n");
	  InterpolatePoints interpPoints; 
	  interpPoints.setInfoLevel( 1 );
          int numGhost=0;  // no need to interpolate ghost points
          Range C(ud[i].getComponentBase(0),ud[i].getComponentBound(0));
	  real time0=getCPU();
	  interpPoints.interpolateAllPoints( vn,ud[i],C,C,numGhost );  // interpolate ud[i] from fine grid solution vn
	  real time=getCPU()-time0;
	  time=ParallelUtility::getMaxValue(time);
	  printF(" ... time to interpolate = %8.2e(s)\n",time);
	}
	else
	{
	  // *newer way* 091126 
	  printF("\n +++++++++++ USE InterpolatePointsOnAGrid, interpolationWidth=%i  +++++++++++++\n\n",
              interpolationWidth );
	  InterpolatePointsOnAGrid interpolator;
	  interpolator.setInfoLevel( 1 );
	  interpolator.setInterpolationWidth(interpolationWidth);
	  // Set the number of valid ghost points that can be used when interpolating from a grid function: 
          int numGhostToUse=1;
	  interpolator.setNumberOfValidGhostPoints( numGhostToUse );
      
	  // Assign all points, extrapolate pts if necessary:
	  interpolator.setAssignAllPoints(true);

          int numGhost=0;  // no need to interpolate ghost points
          Range C(ud[i].getComponentBase(0),ud[i].getComponentBound(0));

	  real time0=getCPU();
	  int num=interpolator.interpolateAllPoints( vn,ud[i],C,C,numGhost);    // interpolate v from u
	  real time=getCPU()-time0;
	  time=ParallelUtility::getMaxValue(time);
	  printF(" ... time to interpolate = %8.2e(s)\n",time);
	}
	
	
        // ud[i]-=v;
        for( int grid=0; grid<cg[i].numberOfComponentGrids(); grid++ )
	{
	  realSerialArray uLocal; getLocalArrayWithGhostBoundaries(ud[i][grid],uLocal);
	  realSerialArray vLocal; getLocalArrayWithGhostBoundaries(    v[grid],vLocal);

          uLocal-=vLocal;
	}
	


	const int useAreaWeightedNorm=1;
        for( int c=v.getComponentBase(0); c<=v.getComponentBound(0); c++ )
	{
	  l2Diff(i,c) = lpNorm(2,ud[i],c,0,0,useAreaWeightedNorm);
	  l1Diff(i,c) = lpNorm(1,ud[i],c,0,0,useAreaWeightedNorm);
	  maxDiff(i,c)=maxNorm(ud[i],c,0,0);
	}
	for( int io=0; io<=1; io++ )
	{
	  FILE *file = io==0 ? stdout : outFile;
	  fPrintF(file,"h(%i)=%e, h(%i)=%e: \n",i,h(i),i+1,h(i+1));
	  for( int c=v.getComponentBase(0); c<=v.getComponentBound(0); c++ )
	    fPrintF(file," maxDiff(%i)=%e, l2Diff(%i)=%e , l1Diff(%i)=%e \n",c,maxDiff(i,c),c,l2Diff(i,c),c,l1Diff(i,c));
	}
	fflush(outFile);
      }

      // these next arrays are used in printing the latex table.
      std::vector<aString> gridName;
      std::vector<aString> cName;

      // here are the components we put in the latex table
      // const int nc = ncu + componentVector.size();
      // const int ncu=u[0].getComponentBound(0)-u[0].getComponentBase(0)+1;
      const int ncu=0;
      const int nc = componentVector.size();
      RealArray cErr(numberOfFiles,nc); cErr=0.;
      RealArray cSigma(nc); cSigma=0.;
      for( int i=0; i<numberOfFiles; i++ )
      {
	gridName.push_back(fileName[i]);
      }
      if( ncu>0 )
      {
	for( int c=u[0].getComponentBase(0); c<=u[0].getComponentBound(0); c++ )
	{
	  cName.push_back(u[0].getName(c));
	}
      }
      for( int c=0; c<componentVector.size(); c++ ) // c = vector 
      {
	ComponentVector & v = componentVector[c];
	cName.push_back(v.name);
      }
	    

      // -- estimate convergence rates---
      if( n>=2 )
      {
	for( int io=0; io<=1; io++ )
	{
	  FILE *file = io==0 ? stdout : outFile;
	  fPrintF(file,"\n Solutions at times=");
	  for( int i=0; i<numberOfFiles; i++ )
	  {
	    fPrintF(file,"%12.6e, ",time(i));
	  }
	  fPrintF(file,"\n");
	}
	
	for( int norm=0; norm<=2; norm++ )
	{
	  for( int io=0; io<=1; io++ )
	  {
	    FILE *file = io==0 ? stdout : outFile;
	    if( norm==0 )
	      fPrintF(file,"++++++++++++++ max norm results +++++++++++++\n");
	    else if( norm==1 )
	      fPrintF(file,"++++++++++++++ l2 norm results +++++++++++++\n");
	    else 
	      fPrintF(file,"++++++++++++++ l1 norm results +++++++++++++\n");
	    fPrintF(file,"    ee = estimated error from C*h^{rate}     \n");
	  }
	
	  const RealArray & diff = norm==0 ? maxDiff : norm==1 ? l2Diff : l1Diff;
	  real sigma,cc;
	  Range R(0,n);
	  for( int c=u[0].getComponentBase(0); c<=u[0].getComponentBound(0); c++ )
	  {
	    computeRate(n,h,diff(R,c),sigma,cc);

	    for( int io=0; io<=1; io++ )
	    {
	      FILE *file = io==0 ? stdout : outFile;

	      fPrintF(file," component=%i, %11s, rate=%5.2f, ",c,(const char*)u[0].getName(c),sigma);
	      for( int i=0; i<=n; i++ )
	      { // estimated errors:
		fPrintF(file,"ee(%i) = %8.2e, ",i,cc*pow(h(i),sigma));
		if( i>0 ) fPrintF(file,"[r=%5.2f], ",pow(h(i-1)/h(i),sigma));

                if( ncu>0 ){ cErr(i,c)=cc*pow(h(i),sigma); cSigma(c)=sigma; }// save for latex
	      }
	      fPrintF(file,"\n");
	    }

	  }
          // --- estimate errors in the vector components ---
	  if( componentVector.size()>0 )
	  {
	    for( int c=0; c<componentVector.size(); c++ ) // c = vector 
	    {
	      ComponentVector & v = componentVector[c];


	      RealArray vdiff(n); // holds diff's for the vector 
	      vdiff=0.;
	      for( int j=0; j<v.component.size(); j++ ) // loop over components of the vector 
	      {
		const int cv = v.component[j];  
		if( cv<u[0].getComponentBase(0) || cv>u[0].getComponentBound(0) )
		{
		  printF("comp::ERROR: vector %i (%s) has an invalid component number = %i. Will ignore.\n",
                          c,(const char*)v.name,cv);
		  continue;
		}
		for( int i=0; i<n; i++ ) // i : solution number 
		{
		  if( norm==0 )
		    vdiff(i) = max( vdiff(i), maxDiff(i,cv) );
		  else if( norm==1 )
		    vdiff(i) += SQR( l2Diff(i,cv) );
		  else
		    vdiff(i) += fabs( l1Diff(i,cv) );
		}
	      }
	      if( norm==1 )
                vdiff=sqrt(vdiff); // l2 norm
	      if( norm==1 || norm==2 )
                vdiff/= v.component.size();  // average l1 or l2 norm per component in the vector 
	      
	      computeRate(n,h,vdiff,sigma,cc);

	      for( int io=0; io<=1; io++ )
	      {
		FILE *file = io==0 ? stdout : outFile;

		fPrintF(file," Vector %s is defined from components ",(const char*)v.name);
		for( int i=0; i<v.component.size(); i++ ) fPrintF(file,"%i, ",v.component[i]);
		fPrintF(file,"\n");

		fPrintF(file," vector comp.  %11s, rate=%5.2f, ",(const char*)v.name,sigma);
		for( int i=0; i<=n; i++ )
		{ // estimated errors:
		  fPrintF(file,"ee(%i) = %8.2e, ",i,cc*pow(h(i),sigma));
		  if( i>0 ) fPrintF(file,"[r=%5.2f], ",pow(h(i-1)/h(i),sigma));

		  cErr(i,c+ncu)=cc*pow(h(i),sigma); cSigma(c+ncu)=sigma; // save for latex

		}
		fPrintF(file,"\n");
	      }


	    } // end for c
	  }
	  
          // --- Now output results in the format of a LaTeX table ---
	  for( int io=0; io<=1; io++ )
	  {
	    FILE *file = io==0 ? stdout : outFile;
	    outputLatexTable( gridName, cName, cErr, cSigma, norm, file );
	  }


	}  // end for norm 
	fflush(outFile);
	
      }
      if( !timesMatch )
      {
	for( int io=0; io<=1; io++ )
	{
	  FILE *file = io==0 ? stdout : outFile;
	  fPrintF(file,"\n***************WARNING: The times of the solutions do not match! ****************\n"
		 "   times=");
	  for( int ii=0; ii<numberOfFiles; ii++ )
	  {
	    fPrintF(file,"%12.6e, ",time(ii));
	  }
	  fPrintF(file,"\n ***** Max difference in times = %8.2e ****\n",maxTimeDiff);
	  fPrintF(file,"\n *********************************************************************************\n");
	}
      }
      
    }
    else if( answer=="plot solutions" )
    {
      for( int i=0; i<numberOfFiles; i++ )
      {
	psp.set(GI_TOP_LABEL,sPrintF(buff,"u[%i] t=%9.3e",i,time(i)));
        ps.erase();
	PlotIt::contour(ps,u[i],psp);
      }
    }
    else if( answer=="plot differences" )
    {
      if( !errorsComputed )
      {
	printF("You should compute the errors before you can plot the differences\n");
	continue;
      }

      for( int i=0; i<numberOfFiles-1; i++ )
      {
	psp.set(GI_TOP_LABEL,sPrintF(buff,"u[%i]-u[%i] t=%9.3e, %9.3e",i,numberOfFiles-1,time(i),time(numberOfFiles-1)));
        ps.erase();
	PlotIt::contour(ps,ud[i],psp);
      }
    }
    else if( answer=="save differences to show files" )
    {
      if( !errorsComputed )
      {
	printF("You should compute the errors before you can save the differences\n");
	continue;
      }

      for( int i=0; i<numberOfFiles-1; i++ )
      {
        aString nameOfShowFile;
	sPrintF(nameOfShowFile,"compDiff%i.show",i);
        printF("Saving show file: [%s] with u[%i]-u[%i], t=%16.10e\n",(const char*)nameOfShowFile,i,numberOfFiles-1,time(i));
        Ogshow show(nameOfShowFile);
	show.saveGeneralComment("Difference computed with comp");
	show.startFrame();                                         // start a new frame
	show.saveComment(0,sPrintF("u[%i]-u[%i]",i,numberOfFiles-1));  
	show.saveComment(1,sPrintF(" t=%16.10e ",time(i)));               
	show.saveSolution( ud[i] ); 

      }
    }
    else if( answer=="exit" )
    {
      break;
    }
  }

  printF("Output written to file %s\n",(const char*)outputFileName);
  if( myid==0 )
    fclose(outFile);

  Overture::finish();          
  return 0;
}


================================================== */
