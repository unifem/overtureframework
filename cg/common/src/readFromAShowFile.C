#include "Parameters.h"
#include "Ogshow.h"
#include "ShowFileReader.h"
#include "PlotStuff.h"
#include "CompositeGridOperators.h"

// =====================================================================================
/// \brief: interface for reading a solution (e.g. initial condition or known solution) 
///  from a show file.
/// \details: this function can be used to read a solution from a show file, optionally choosing certain
/// components and optionally computing derived quantites (such as pressure from density and temperature or
/// components of the stress tensor from displacements). This function can also be used to interpolate the
/// solution from the show file onto the reference grid.
/// \param showFileReader : use this show file reader
/// \param cgRef (input) : reference grid (will be used if the solution from the show file needs
///            to be interpolated to the current grid).
/// \param cg (output) : output grid (
/// \param u (output) : output solution from the show file (if assigned).
/// \solutionNumber (output) : -1 means no solution was chosen. A non-negative value indicates
///      which solution in the show file was chosen.
///    
// =====================================================================================
int Parameters::
readFromAShowFile(ShowFileReader & showFileReader,
                  CompositeGrid & cgRef,
                  CompositeGrid & cg,
                  realCompositeGridFunction & u,
                  int & solutionNumber )
{
  int returnValue=0;

  solutionNumber=-1;
  
  assert(  dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = * dbase.get<GenericGraphicsInterface* >("ps");

  // CompositeGrid & cg = *u.getCompositeGrid(), 
  
  const int & numberOfComponents = dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions = dbase.get<int >("numberOfDimensions");
  const aString *componentName = dbase.get<aString* >("componentName");
  
  bool & useGridFromShowFile=dbase.get<bool>("useGridFromShowFile");

  bool alwaysInterpolateFromShowFile=false;
  // By default AMR or moving grid problems use the grid from the show file *wdh* 090818 
  if( isAdaptiveGridProblem() || isMovingGridProblem() )
    useGridFromShowFile=true;
  
  aString nameOfShowFile="myShowFile.show";
  

  aString answer,line;
  char buff[100];
  //  const int numberOfDimensions = cg.numberOfDimensions();


  GUIState gui;
  gui.setExitCommand("done", "continue");
  DialogData & dialog = (DialogData &)gui;

  if( true )
  {
    dialog.setWindowTitle("Solution From a Show File");

    const int maxCommands=5;
    aString pushButtonCommands[maxCommands];
    int n=0;
    pushButtonCommands[n]="assign solution from show file"; n++;
    pushButtonCommands[n]="choose file from menu..."; n++;
    pushButtonCommands[n]="plot solution..."; n++;
    pushButtonCommands[n]=""; n++;
    assert( n<maxCommands );

    int numRows=n;
    // addPrefix(pushButtonCommands,prefix,cmd,maxCommands);
    dialog.setPushButtons( pushButtonCommands, pushButtonCommands, numRows );


    // toggle button: "use grid from show file"   <- set to true by default if moving or adaptive
//     aString tbCommands[] = {"use grid from show file",
//                             "always interpolate from show file",
//                             ""};

//     int tbState[10];
//     tbState[0] = useGridFromShowFile; 
//     tbState[1] = alwaysInterpolateFromShowFile;
//     int numColumns=1;
//     dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

    const int numberOfTextStrings=5;            // we should count up how many we have 
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    textLabels[nt] = "show file name:"; textStrings[nt]=nameOfShowFile;
    nt++;
    
    textLabels[nt] = "solution number:"; textStrings[nt]="-1 (-1=last)";
    nt++;
   
    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    // addPrefix(textLabels,prefix,cmd,maxCommands);
    dialog.setTextBoxes(textLabels, textLabels, textStrings);

  }

  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("read from show>");  

  int plotSolution=false;
  real solutionTime=0.;  // solution time
  int numberOfSolutions=0;
  
  int len;
  for(int it=0; ; it++)
  {
    gi.getAnswer(answer,"");

    // printf("readFromAShowFile: answer=[%s]\n",(const char*)answer);
  

    // if( dialog.getToggleValue(answer,"use grid from show file",useGridFromShowFile) ){}//
    // else if( dialog.getToggleValue(answer,"always interpolate from show file",alwaysInterpolateFromShowFile) ){}//
    if( answer=="done" || answer=="continue" )
    {
      break;
    }
    else if( (len=answer.matches("show file name:")) ||
             answer=="choose file from menu..." )
    {
      if( answer=="choose file from menu..." )
      {
	gi.inputFileName(nameOfShowFile, ">> Enter the name of the (old) show file", ".show");
      }
      else
      {
        len++;
        while( len<answer.length() && answer[len]==' ' ){  len++; } // skip leading blanks
	
	nameOfShowFile=answer(len,answer.length()-1);
      }
      
      printF("nameOfShowFile=[%s]\n",(const char*)nameOfShowFile);
      if( nameOfShowFile=="" || nameOfShowFile==" " )
        continue;
      
      showFileReader.open(nameOfShowFile);
      int numberOfFrames=showFileReader.getNumberOfFrames();
      numberOfSolutions = max(1,numberOfFrames);
      solutionNumber=numberOfSolutions;  

      dialog.setTextLabel("show file name:",nameOfShowFile);
      
      dialog.setTextLabel("solution number:",sPrintF("%i  (from %i to %i, -1=last)",
                             solutionNumber,1,numberOfSolutions));
      
    }

    else if( answer=="assign solution from show file" )
    {
      if( nameOfShowFile=="" )
      {
	printF("You must choose the name of a show file before you can assign a solution");
	continue;
      }
      InitialConditionOption & initialConditionOption = 
                       dbase.get<InitialConditionOption >("initialConditionOption");

      initialConditionOption=readInitialConditionFromShowFile; 
      plotSolution=true;
      
      printF("readFromAShowFile: read a solution from a show file...\n");
      // Read in a solution from a show file
      // This only works if the first components of the grid functions match


      if( solutionNumber<0 || solutionNumber>numberOfSolutions )
      {
	solutionNumber=numberOfSolutions;
      }
      

      realCompositeGridFunction uSF;
      showFileReader.getASolution(solutionNumber,cg,uSF);        // read in a grid and solution

      printF("--- Names of components from the show file solution ---\n");
      for(int c=uSF.getComponentBase(0); c<=uSF.getComponentBound(0); c++ )
      {
	printF("component %i : %s\n",c,(const char*)uSF.getName(c));
      }
      printF("--- Names of actual solution components ---\n");

      for( int c=0; c<numberOfComponents; c++ )
      {
	printF("component %i : %s\n",c,(const char*)componentName[c]);
      }


      // look for extra data that will appear if this show file was saved by this class
      HDF_DataBase & db = *showFileReader.getFrame();
      int found;
      found = db.get(solutionTime,"t");
      if( found==0 )
      {
	printF("readFromAShowFile: time taken from file =%9.3e\n", solutionTime);

        // dialog.setTextLabel("initial time",sPrintF("%g",solutionTime));
        // gf[current].t=tInitial;
	
      }
    
      // read any header comments that go with this solution
      int numberOfHeaderComments;
      const aString *headerComment=showFileReader.getHeaderComments(numberOfHeaderComments);
      for( int i=0; i<numberOfHeaderComments; i++ )
	printF("header comment: %s \n",(const char *)headerComment[i]);

      
      printF("readFromAShowFile: cgRef.numberOfGrids()=%i, cg.numberOfGrids()=%i,\n"
             "                   cgRef.numberOfComponentGrids()=%i, cg.numberOfComponentGrids()=%i,\n",
            cgRef.numberOfGrids(), cg.numberOfGrids(),
	     cgRef.numberOfComponentGrids(),
	     cg.numberOfComponentGrids());
      


      // --- Convert solution from the show file into a valid initial condition:
      //     - choose appropriate components
      //     - compute derived quanitities as needed

      // Here is virtual function that knows how to convert the solution from the show file:

	  

      // ******* FINISH ME  ************
      // convertSolutionFromShowFile( );
      // do this for now: 
      bool solidMechanics = dbase.has_key("timeSteppingMethodSm");
      if( solidMechanics )
      {
	// -- convert a solution for Cgsm: solid mechanics ---

	Range all;
        u.updateToMatchGrid(cg,all,all,all,numberOfComponents);
        u=0.;
	for( int c=0; c<numberOfComponents; c++ )
	{
	  u.setName(componentName[c],c);
        }

        // look for show file parameters for u1 and u2 
	int u1csf=0;
	int u2csf=1;
	for(int c=uSF.getComponentBase(0); c<=uSF.getComponentBound(0); c++ )
	{
	  const aString & name = uSF.getName(c);
	  if( name=="u" || name=="u1" )
	  {
	    u1csf=c;
	  }
	  if( name=="v" || name=="u2" )
	  {
	    u2csf=c;
	  }
	
	}
        int u1c = dbase.get<int >("uc");
	if( u1c<0 ) u1c = dbase.get<int >("u1c");
        int u2c = dbase.get<int >("vc");
	if( u2c<0 ) u2c = dbase.get<int >("u2c");
        int u3c = dbase.get<int >("wc");
	if( u3c<0 ) u3c = dbase.get<int >("u3c");
	Index I1,I2,I3;
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  MappedGrid & mg = cg[grid];
	  realArray & ug = u[grid];
	  realArray & usf= uSF[grid];
          getIndex(mg.dimension(),I1,I2,I3);
	  
	  ug(I1,I2,I3,u1c)=usf(I1,I2,I3,u1csf);
	  ug(I1,I2,I3,u2c)=usf(I1,I2,I3,u2csf);
	}


	bool computeStress=dbase.get<int >("s11c")>=0;

	if( computeStress )
	{
	  const real & rho= dbase.get<real>("rho");
	  const real & mu = dbase.get<real>("mu");
	  const real & lambda = dbase.get<real>("lambda");

	  printF("readFromAShowFile: computing stress using lambda=%10.3e and mu=%10.3e\n",lambda,mu);
	  
	  CompositeGridOperators cgop(cg);
	  // cgop.setOrderOfAccuracy(4);
	  
          const int s11c = dbase.get<int >("s11c");
          const int s12c = dbase.get<int >("s12c");
          const int s13c = dbase.get<int >("s13c");
          const int s21c = dbase.get<int >("s21c");
          const int s22c = dbase.get<int >("s22c");
          const int s23c = dbase.get<int >("s23c");
          const int s31c = dbase.get<int >("s31c");
          const int s32c = dbase.get<int >("s32c");
          const int s33c = dbase.get<int >("s33c");

	  if( false ) // ********************************************************* TEMP
	  {
	    // for testing extrap boundary values
	    u.setOperators(cgop);
	    Range C=Range(u1c,u1c+numberOfDimensions-1);

	    BoundaryConditionParameters extrapParams;
	    extrapParams.orderOfExtrapolation=5;  // what should this be? 

	    // extrapParams.ghostLineToAssign=-1;  // extrap first line in 
	    //u.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,extrapParams); 

	    extrapParams.ghostLineToAssign=0;  // extrap boundary
	    u.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,extrapParams); 

	    //extrapParams.ghostLineToAssign=1;  // extrap 1st ghost
	    //u.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,extrapParams); 

	    //extrapParams.ghostLineToAssign=2;  // extrap 2nd ghost
	    //u.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,extrapParams); 

	  }



	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    MappedGrid & mg = cg[grid];
	    realArray & ug = u[grid];
	    MappedGridOperators & op = cgop[grid];
	 
	    getIndex(mg.gridIndexRange(),I1,I2,I3);

	    Range C=Range(u1c,u1c+numberOfDimensions-1);
	    if( numberOfDimensions==2 )
	    {
	      realArray ux(I1,I2,I3,C), uy(I1,I2,I3,C);
	      op.derivative(MappedGridOperators::xDerivative,ug,ux,I1,I2,I3,C);
	      op.derivative(MappedGridOperators::yDerivative,ug,uy,I1,I2,I3,C);
	      realArray div(I1,I2,I3);
	      div=ux(I1,I2,I3,u1c)+uy(I1,I2,I3,u2c);

	      ug(I1,I2,I3,s11c)=lambda*div+(2.*mu)*ux(I1,I2,I3,u1c);
	      ug(I1,I2,I3,s12c)=mu*(uy(I1,I2,I3,u1c)+ux(I1,I2,I3,u2c));
	      ug(I1,I2,I3,s22c)=lambda*div+(2.*mu)*uy(I1,I2,I3,u2c);
	   
	      ug(I1,I2,I3,s21c)=ug(I1,I2,I3,s12c);

	    }
	    else
	    {

	      // div = u1x+u2y+u3z;
	      // s11 = lambda*div + 2.*mu*u1x;
	      // s12 = mu*( u1y+u2x );
	      // s13 = mu*( u1z+u3x );
	      // s21 = s12;
	      // s22 = lambda*div + 2.*mu*u2y;
	      // s23 = mu*( u2z + u3y );
	      // s31 = s13;
	      // s32 = s23;
	      // s33 = lambda*div + 2.*mu*u3z;

	      realArray ux(I1,I2,I3,C), uy(I1,I2,I3,C), uz(I1,I2,I3,C);
	      op.derivative(MappedGridOperators::xDerivative,ug,ux,I1,I2,I3,C);
	      op.derivative(MappedGridOperators::yDerivative,ug,uy,I1,I2,I3,C);
	      op.derivative(MappedGridOperators::zDerivative,ug,uz,I1,I2,I3,C);
	      realArray div(I1,I2,I3);
	      div=ux(I1,I2,I3,u1c)+uy(I1,I2,I3,u2c)+uz(I1,I2,I3,u3c);

	      ug(I1,I2,I3,s11c)=lambda*div+(2.*mu)*ux(I1,I2,I3,u1c);
	      ug(I1,I2,I3,s12c)=mu*(uy(I1,I2,I3,u1c)+ux(I1,I2,I3,u2c));
	      ug(I1,I2,I3,s13c)=mu*(uz(I1,I2,I3,u1c)+ux(I1,I2,I3,u3c));
	      ug(I1,I2,I3,s22c)=lambda*div+(2.*mu)*uy(I1,I2,I3,u2c);
	      ug(I1,I2,I3,s23c)=mu*(uz(I1,I2,I3,u2c)+uy(I1,I2,I3,u3c));
	      ug(I1,I2,I3,s33c)=lambda*div+(2.*mu)*uz(I1,I2,I3,u3c);

	      ug(I1,I2,I3,s21c)=ug(I1,I2,I3,s12c);
	      ug(I1,I2,I3,s31c)=ug(I1,I2,I3,s13c);
	      ug(I1,I2,I3,s32c)=ug(I1,I2,I3,s23c);

	    }
	    
	  } // end for grid 
	  
	  if( true )
	  {
            // extrapolate ghost points of the stress
	    u.setOperators(cgop);
            Range S=Range(s11c,s11c+numberOfDimensions*numberOfDimensions-1);

	    BoundaryConditionParameters extrapParams;
	    extrapParams.orderOfExtrapolation=3;  // what should this be? 

	    // extrapParams.ghostLineToAssign=0;  // extrap boundary
	    // u.applyBoundaryCondition(S,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,extrapParams); 

            extrapParams.ghostLineToAssign=1; 
	    u.applyBoundaryCondition(S,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,extrapParams); 
	    u.finishBoundaryConditions(extrapParams);
	  }

	}
	
      }
      else
      {
	u=uSF;
      }
      
    }
    else if( answer=="interpolate to reference grid" )
    {
/* ---------- finish me ---
      cgRef.update(MappedGrid::THEmask);
      cg.update(MappedGrid::THEmask);
      if( isDifferent(cgRef,cg)) )
      {
	Range all;
        const int numberOfComponentsSF=u.getComponentBound(0)-u.getComponentBase(0)+1;
	const int nc=min(numberOfComponents,numberOfComponentsSF);
        if( nc!=numberOfComponents )
	{
	  printF("getInitialConditions:WARNING:numberOfComponents in show file =%i is not equal "
                 "to numberOfComponents=%i\n",
                 numberOfComponentsSF,numberOfComponents);
	  if( numberOfComponents>nc )
            printF(" getInitialConditions:I am setting the values for the extra variables to zero.\n");
	}
	Range C(0,nc-1);
        printF("Interpolating the solution from the show file to the existing grid...\n");
	cg.update(MappedGrid::THEcenter);

	if ( numberOfComponentsSF==numberOfComponents )
	  { // kkc 090330 original way
	    int status = interpolateAllPoints( uSF,u);
	    if ( !status )
	      printF("getInitialConditions:WARNING:interpolateAllPoints returned %i\n",status);
	  }
	else
	{ // kkc 090330 only if there are a different number of components
	  InterpolatePoints interp;
	  interp.interpolateAllPoints( uSF,u, C, C );  // interpolate u from uSF
	}
      }
      else
      {
        printF("readFromShow: Just copy values since the grids look the same. (Choose 'always interpolate' to force the"
               " solution to be interpolated.)\n");

	Range all;
        const int numberOfComponentsSF=uSF.getComponentBound(0)-uSF.getComponentBase(0)+1;
	const int nc=min(numberOfComponents,numberOfComponentsSF);
        if( nc!=numberOfComponents )
	{
	  printF("getInitialConditions:WARNING:numberOfComponents in show file =%i is not equal "
                 "to numberOfComponents=%i\n",
                 numberOfComponentsSF,numberOfComponents);
	  if( numberOfComponents>nc )
            printF(" getInitialConditions:I am setting the values for the extra variables to zero.\n");
	}
	Range C(0,nc-1);
	for( int grid=0; grid<cg.numberOfGrids(); grid++ )
	{
          if( nc<numberOfComponents )
	  {
            // u[grid](all,all,all,Range(nc,numberOfComponents-1))=0.;  // ----------------- fix this ------------
            assign(u[grid],0.,all,all,all,Range(nc,numberOfComponents-1));
	  }
	  
	  // ::display(uSF[grid],"uSF","%5.2f ");
	  

          assign( u[grid],all,all,all,C, uSF[grid],all,all,all,C);
	  
          RealArray uMin(numberOfComponents),uMax(numberOfComponents);
	  
          GridFunctionNorms::getBounds(u[grid],uMin,uMax);  


	  for( int c=0; c<numberOfComponents; c++ )
	  {
	    printF("Values from show file: grid=%i: component=%i: min=%e, max=%e \n",grid,c,uMin(c),uMax(c));
	  }
	}
        if( Parameters::checkForFloatingPointErrors )
          checkSolution(u,"getInitialCond");
      }

    ------------- */     

      
    }
    else if( answer=="plot solution..." )
    {
      plotSolution=2;
    }
    else
    {
      printF("Unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

    if( plotSolution )
    {
      PlotStuffParameters & psp = dbase.get<GraphicsParameters >("psp");
      psp.set(GI_TOP_LABEL,sPrintF("Solution %i from the show file",solutionNumber));
      if( plotSolution==1 )
        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      else
        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      gi.erase();
      PlotIt::contour(gi,u,psp);
    }


  }

  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

 return returnValue;

}
