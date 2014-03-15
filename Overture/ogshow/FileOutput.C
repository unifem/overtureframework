#include "FileOutput.h"
#include "GridCollection.h"
#include "GenericGraphicsInterface.h"

#define ForBoundary(side,axis)   for( axis=0; axis<gc.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


//\begin{>FileOutputInclude.tex}{\subsection{Constructors}} 
FileOutput::
FileOutput()
//=====================================================================================
// /Description:
//     Use this class to output ascii formatted data from grid functions.
//  This class holds a 'state' indicating what information should be output and
// how it should be output. The intention is that that a user will build multiple versions
// of this object in order to output data in multiple ways. For example one object may
// be used to save data on a particluar boundary, while another may be used to save
// data on a particular grid.
//
// /Author: WDH
//\end{FileOutputInclude.tex} 
//=====================================================================================
{
  debug=3;
  outputFile=NULL;
  outputFileName="output.dat";
  format="%11.4e ";
  setValueAtUnusedPoints=FALSE;
  valueForUnusedPoints=0.;
  numberOfGhostLines=2;
  addLabels=TRUE;
  
  saveGeometry.redim(numberOfGeometryArrays);
  saveGeometry=FALSE;  // geometry arrays are off by default.
}

FileOutput::
~FileOutput()
{
  if( outputFile!=NULL )
  {
    fclose(outputFile);
  }
}


int FileOutput::
updateParameterArrays( realGridCollectionFunction & u )
// Protected routine.
{
  GridCollection & gc = *u.getGridCollection();
  const int numberOfComponents =u.getComponentDimension(0);
  const int numberOfComponentGrids = gc.numberOfComponentGrids();
  
  const int oldNumberOfComponents=saveComponent.getLength(0);
  if( numberOfComponents!=oldNumberOfComponents )
  {
    saveComponent.resize(numberOfComponents);
    if( numberOfComponents>oldNumberOfComponents )
    {
      Range Rc(oldNumberOfComponents,numberOfComponents-1);
      saveComponent(Rc)=true;
    }
  }
  int oldNumberOfComponentGrids= saveGrids.getLength(0);
  if( numberOfComponentGrids!=oldNumberOfComponentGrids )
  {
    saveGrids.resize(numberOfComponentGrids);
    saveGridFace.resize(2,3,numberOfComponentGrids);
    if( numberOfComponentGrids>oldNumberOfComponentGrids )
    {
      Range all,Rg(oldNumberOfComponentGrids,numberOfComponentGrids-1); 
      saveGrids(Rg)=true;
      saveGridFace(all,all,Rg)=false;
    }
  }
  

  return 0;
}

int FileOutput::
saveData( realMappedGridFunction & u, int grid, int side /* =-1 */, int axis /* = -1 */ )
// ============================================================================================
// Protected routine to output data on a grid or the face of a grid.
// /side,axis (input) : by default save the entire grid, otherwise save a face: axis==side
// ============================================================================================
{
  MappedGrid & mg = *u.getMappedGrid();
  
  const int numberOfComponents =u.getComponentDimension(0);

  realMappedGridFunction v;

  const IntegerArray & dimension=mg.dimension();
  IntegerArray range = extendedGridIndexRange(mg);
  for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
  {
    range(Start,dir)=max(dimension(Start,dir),range(Start,dir)-numberOfGhostLines);
    range(End  ,dir)=min(dimension(End  ,dir),range(End  ,dir)+numberOfGhostLines);
  }
  Index I1,I2,I3;
  if( side==-1 || axis==-1 )
    getIndex(range,I1,I2,I3);
  else
  {
    range(Range(0,1),axis)=mg.gridIndexRange(side,axis);
    getIndex(range,I1,I2,I3);
  }
  
  fprintf(outputFile,"%i  %s            : grid number, name \n",grid,(const char*)mg.getName());
  fprintf(outputFile,"%i,%i  %i,%i %i,%i      : dimension \n",dimension(0,0),dimension(1,0),
	  dimension(0,1),dimension(1,1),dimension(0,2),dimension(1,2));
	
  const IntegerArray & gridIndexRange=mg.gridIndexRange();
  fprintf(outputFile,"%i,%i  %i,%i %i,%i      : gridIndexRange \n",
	  gridIndexRange(0,0),gridIndexRange(1,0),
	  gridIndexRange(0,1),gridIndexRange(1,1),gridIndexRange(0,2),gridIndexRange(1,2));
	
  fprintf(outputFile,"%i,%i  %i,%i %i,%i      : bounds saved \n",
	  I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
	
  const IntegerArray & bc = mg.boundaryCondition();
  fprintf(outputFile,"%i,%i  %i,%i %i,%i       : boundaryCondition (>0 : physical, 0 : interp, "
	  "<0 : periodic)\n",bc(0,0),bc(1,0),
	  bc(0,1),bc(1,1),bc(0,2),bc(1,2));

  const IntegerArray & isPeriodic = mg.isPeriodic();
  fprintf(outputFile,"%i,%i,%i                : periodicity (0=not,1=derivative,2=function)\n",isPeriodic(axis1),
	  isPeriodic(axis2),isPeriodic(axis3) );
	

  if( saveGeometry(mask) )
  {
    if( debug & 2 ) printf("saving the mask array on grid %i\n",grid);
    if( addLabels )
      fprintf(outputFile,"mask saved as (((mask(i1,i2,i3),i1=%i,%i),i2=%i,%i),i3=%i,%i)\n",
	      I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
          
    ::displayMask(mg.mask()(I1,I2,I3),"",outputFile);
  }
  if( saveGeometry(vertex) )
  {
    mg.update(MappedGrid::THEvertex);
    
    if( debug & 2 ) printf("saving the vertex array on grid %i\n",grid);
    if( addLabels )
      fprintf(outputFile,"vertex saved as ((((vertex(i1,i2,i3,n),i1=%i,%i),i2=%i,%i),i3=%i,%i),n=0,%i)\n",
	      I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
	      mg.numberOfDimensions()-1);
          
    ::display(mg.vertex()(I1,I2,I3),"",dp);
  }

  for( int n=0; n<numberOfComponents; n++ )
  {
    if( saveComponent(n) )
    {
      if( debug & 2 ) 
        printf("saving component [%s] on grid %i\n",(const char*)u.getName(u.getComponentBase(0)+n),grid);
      if( addLabels )
	fprintf(outputFile,"%s              : component saved as (((u(i1,i2,i3),i1=%i,%i),i2=%i,%i),i3=%i,%i)\n",
		(const char*)u.getName(u.getComponentBase(0)+n),
		I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
      if( setValueAtUnusedPoints )
      {
	v.updateToMatchGrid(mg);
	v(I1,I2,I3)=u(I1,I2,I3,n);
	where( mg.mask()==0 )
	{
	  v=valueForUnusedPoints;
	}
      }
      else
	v.link(u,Range(n,n));
      ::display(v(I1,I2,I3),"",dp);
    }
  }

  return 0;
  
}




//\begin{>>FileOutputInclude.tex}{\subsection{save}} 
int FileOutput::
save( realGridCollectionFunction & u, const aString & label /* =nullString */ )
// =======================================================================================
// /Description:
//     Save the data for the solution u, using the current output parameters.
//   The file is opened (for appending) at the start of this routine and closed at the
// end (so the file is flushed).
// /u (input) : save values from this GridFunction.
// /label (input) : optional label to print at the start of the data.
//\end{FileOutputInclude.tex} 
// =======================================================================================
{
  if( outputFile==NULL )
  {
    assert( outputFileName!="" );
    if( debug & 2 )
      printf("FileOutput::save:INFO appending data to file=[%s]\n",(const char*)outputFileName);
    outputFile = fopen(outputFileName,"a" );
    if( outputFile==NULL )
    {
      printf("ERROR opening the file %s! No output generated",(const char *)outputFileName);
      return 1;
    }
  }
  if( debug & 2 ) printf("FileOutput::save:saving results to file %s\n",(const char *)outputFileName);
      
  dp.set(outputFile);
  dp.set(DisplayParameters::labelNoIndicies);

  updateParameterArrays(u);

  GridCollection & gc = *u.getGridCollection();
  
  const int numberOfComponentsToSave=sum(saveComponent);
  if( label!=nullString )
    fprintf(outputFile,"%s\n",(const char*)label);
  fprintf(outputFile,"%i %i %i               : number of grids, number of dimensions, number of components\n",
	  gc.numberOfGrids(),gc.numberOfDimensions(),numberOfComponentsToSave);

  int grid;
  for( grid=0; grid<gc.numberOfGrids(); grid++ )
  {
    if( saveGrids(grid) )
    {
      saveData( u[grid],grid );
    }
  }
  for( grid=0; grid<gc.numberOfGrids(); grid++ )
  {
    int side,axis;
    ForBoundary(side,axis)
    {
      if( saveGridFace(side,axis,grid) )
      {
	saveData( u[grid],grid,side,axis );
      }
    }
  }
  
  fclose(outputFile);
  outputFile=NULL;
  
  return 0;
}


//\begin{>>FileOutputInclude.tex}{\subsection{update}} 
int FileOutput::
update( realGridCollectionFunction & u, GenericGraphicsInterface & gi )
// =======================================================================================
// /Description:
//    Update and change the parameters that specify what data should be saved and how it
// should be saved. 
// /u (input) : A typical grid function. Since some parameters depend on the number of grids
// or the number of components, one needs to pass this argument.
// /gi (input) : use this graphics interface.
//\end{FileOutputInclude.tex} 
// =======================================================================================
{
  GridCollection & gc = *u.getGridCollection();

  const int numberOfMenuItems0=26;
  aString menu0[numberOfMenuItems0] = 
  {
    "!file output",
    "file name (new file)",
    "file name (append)",
    "close file",
    "save data",
    ">toggle components",
      "choose all",
      "choose none",
    "<>toggle geometry arrays",
      "mask (off)",
      "vertex (off)",
    "<>grids",
      "save all grids",
      "save no grids",
      "specify grids to save",
    "<>boundaries",
      "specify boundaries to save",
    "<>options",
      "set value at unused points",
      "add labels",    
      "do not add labels",    
      "number of ghost lines",
      "format for reals",
    "<help",
    "exit",
    ""
  };

  // allocate space and give initial values to 
  //    saveComponent
  //    saveGrids
  //    saveGridFace
  updateParameterArrays(u);

  const int numberOfComponents =u.getComponentDimension(0);

  int numberOfMenuItems=numberOfMenuItems0+numberOfComponents;
  aString answer,answer2;
  char buff[80];
  aString *menu = new aString [numberOfMenuItems];
  int componentStart=-1,componentEnd=-1;  // marks where component names appear in the menu
  int i,j=0;
  for( i=0; i<numberOfMenuItems0; i++ )
  {
    menu[j]=menu0[i];
    j++;
    if( menu[i]==">toggle components" )
    {
      componentStart=j;
      for( int n=0; n<numberOfComponents; n++ )
      {
        menu[j]=sPrintF(buff,"%s (on)",(const char*)u.getName(u.getComponentBase(0)+n));
	j++;
      }
      componentEnd=j-1;
    }
  }
  if( menu[j-1]!="" )
  {
    printf("ERROR in forming menu, menu[j-1]=%s\n",(const char*)menu[j-1]);
    throw "error";
  }


  // set default prompt
  gi.appendToTheDefaultPrompt("fileOutput>");

  for(;;)
  {
    int choice = gi.getMenuItem(menu,answer,"choose a menu item");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="file name (new file)" || answer=="file name (append)" )
    {
      bool openNew= answer=="file name (new file)";
      if( !openNew )
        printf("The solution will be appended to the file if it exists.\n");

      gi.inputString(answer2,"Enter the name for output file (default=output.dat)");
      if( answer2 !="" && answer2!=" ")
	outputFileName=answer2;
      else
	outputFileName="output.dat";
    
      if( openNew )
        outputFile = fopen(outputFileName,"w" );
      else
        outputFile = fopen(outputFileName,"a" );
      if( outputFile==NULL )
	gi.outputString(sPrintF(buff,"ERROR opening the file %s! ",(const char *)outputFileName));

      if( debug & 2 ) printf("file [%s] opened for output\n",(const char*)outputFileName);
    }
    else if( answer=="close file" )
    {
      fclose(outputFile);
      outputFile=NULL;
    }
    else if( answer=="set value at unused points" )
    {
      printf("Enter a value to assign unused point on the grid (where the mask=0)\n"
             "Enter a <cr> to leave unused points unaltered");
      gi.inputString(answer,sPrintF(buff,"Enter a value to use (default=%e, <cr>=do not set)\n",
                     valueForUnusedPoints));
      if( answer!="" )
      {
	setValueAtUnusedPoints=TRUE;
	sScanF(answer,"%e",&valueForUnusedPoints);
	printf("Using the value %e at unused points\n",valueForUnusedPoints);
      }
      else
      {
	setValueAtUnusedPoints=FALSE;
        printf("Not changing the values at unused points");
	
      }
    }
    else if( answer=="format for reals" )
    {
      printf("Example formats are `%%6.2f ' , `%%12.5e ' \n");
      gi.inputString(answer,sPrintF(buff,"Enter the printf format for reals (current=[%s])\n",(const char*)format));
      if( answer!="" )
      {
	format=answer;
        printf("New format = [%s]\n",(const char *)format);
	dp.set(DisplayParameters::floatFormat,format);
	dp.set(DisplayParameters::doubleFormat,format);
      }
    }
    else if( answer=="number of ghost lines" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter the number of ghost lines of data to save (current=%i)\n",
                numberOfGhostLines));
      if( answer!="" )
      {
        sScanF(answer,"%i",&numberOfGhostLines);
	printf("Saving %i ghost lines (up to as many exist)",numberOfGhostLines);
      }
      
    }
    else if( answer=="add labels" )
    {
      addLabels=TRUE;
      printf("labels will be added to the output.\n");
    }
    else if( answer=="do not add labels" )
    {
      addLabels=FALSE;
      printf("labels will NOT be added to the output.\n");
    }
    else if( choice>=componentStart && choice<= componentEnd )
    {
      int n=choice-componentStart;
      saveComponent(n)=!saveComponent(n);
      if( saveComponent(n) )
	menu[choice]=sPrintF(buff,"%s (on)",(const char*)u.getName(u.getComponentBase(0)+n));
      else
	menu[choice]=sPrintF(buff,"%s (off)",(const char*)u.getName(u.getComponentBase(0)+n));
      
    }
    else if( answer=="choose all" || answer=="choose none" )
    {
      saveComponent= answer=="choose all" ? TRUE : FALSE;
      for( int n=0; n<numberOfComponents; n++ )
      {
	if( saveComponent(n) )
	  menu[componentStart+n]=sPrintF(buff,"%s (on)",(const char*)u.getName(u.getComponentBase(0)+n));
	else
	  menu[componentStart+n]=sPrintF(buff,"%s (off)",(const char*)u.getName(u.getComponentBase(0)+n));
      }
    }
    else if( answer(0,3)=="mask" )
    {
      saveGeometry(mask)=!saveGeometry(mask);
    }
    else if( answer(0,5)=="vertex" )
    {
      saveGeometry(vertex)=!saveGeometry(vertex);
    }
    else if( answer=="save all grids" )
    {
      saveGrids=true;
      printf("save data for all grids\n");
    }
    else if( answer=="save no grids" )
    {
      saveGrids=false;
      printf("save data for no grids\n");
    }
    else if( answer=="specify grids to save" )
    {
      IntegerArray gridList;
      int numberInList = gi.getValues(sPrintF(buff,"Enter grid numbers (range 0..%i)",
					  gc.numberOfComponentGrids()-1),gridList,0,gc.numberOfComponentGrids()-1);
      saveGrids=false;
      if( numberInList>0 )
      {
	saveGrids(gridList)=true;
        printf("saving grids: ");
	for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
	{
          if( saveGrids(grid) )
  	    printf("%i,",grid);
	}
        printf("\n");
      }
      else
      {
	printf("saving no grids\n");
      }
    }
    else if( answer=="specify boundaries to save" )
    {
      aString *menu2 = new aString[2*gc.numberOfDimensions()*gc.numberOfComponentGrids()+2];
      aString answer2;
      for(;;)
      {
	i=0;
        int grid,side,axis;
	for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
	  ForBoundary(side,axis)
	    if( gc[grid].boundaryCondition(side,axis)>0 )
	      menu2[i++]=sPrintF(buff,"(%i,%i,%i) = (%s,side,axis) %s",grid,side,axis,
                                 (const char*)gc[grid].mapping().getName(Mapping::mappingName),
				 saveGridFace(side,axis,grid)==TRUE ? "(on)" : "(off)");
	menu2[i++]="exit"; 
	menu2[i]="";   // null string terminates the menu

        gi.getMenuItem(menu2,answer2);

        if( answer2=="exit" )
          break;
        if( sScanF(answer2,"(%i %i %i)",&grid,&side,&axis)==3 )
          saveGridFace(side,axis,grid)=!saveGridFace(side,axis,grid);
        else
          cout << "ERROR: unknown response: [" << answer2 << "]\n";
      }
      delete [] menu2;
      

    }
    else if( answer=="save data" )
    {
      save(u);
    }
    else
    {
      printf("unknown response: [%s] \n",(const char*) answer);
      gi.stopReadingCommandFile();
    }
  }
  
  return 0;
}



// ******************** OLD WAY *******************


#include "GenericGraphicsInterface.h"
#include "GridCollection.h"
#include "DisplayParameters.h"
#include "display.h"



//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{output to a file}}
int 
fileOutput( GenericGraphicsInterface &gi, realGridCollectionFunction & u )
// ========================================================================================
//  /Description:
//     Use this function to output selected components of a grid function to an ascii file,
//  or output geometry arrays such as the mask or vertices.
//
//\end{GL_GraphicsInterfaceInclude.tex}  
//===================================================================================
{
  
  GridCollection & gc = *u.getGridCollection();

  aString outputFileName="output.dat";
  aString format="%11.4e ";
  bool setValueAtUnusedPoints=FALSE;
  real valueForUnusedPoints=0.;
  int numberOfGhostLines=2;
  bool addLabels=TRUE;

  const int numberOfMenuItems0=20;
  aString menu0[numberOfMenuItems0] = 
  {
    "!output",
    "file name (new file)",
    "file name (append)",
    "close file",
    "save data",
    ">toggle components",
      "choose all",
      "choose none",
    "<>toggle geometry arrays",
      "mask (off)",
      "vertex (off)",
    "<>options",
      "set value at unused points",
      "add labels",    
      "do not add labels",    
      "number of ghost lines",
      "format for reals",
    "<help",
    "exit",
    ""
  };
  const int numberOfComponents =u.getComponentDimension(0);
  IntegerArray saveComponent(numberOfComponents);
  saveComponent=TRUE;   // save all components by default
  enum
  {
    mask=0,
    vertex,
    numberOfGeometryArrays
  };
  IntegerArray saveGeometry(numberOfGeometryArrays);
  saveGeometry=FALSE;  // geometry arrays are off by default.
  
  
  int numberOfMenuItems=numberOfMenuItems0+numberOfComponents;
  aString answer,answer2;
  char buff[80];
  aString *menu = new aString [numberOfMenuItems];
  int componentStart=-1,componentEnd=-1;  // marks where component names appear in the menu
  int i,j=0;
  for( i=0; i<numberOfMenuItems0; i++ )
  {
    menu[j]=menu0[i];
    j++;
    if( menu[i]==">toggle components" )
    {
      componentStart=j;
      for( int n=0; n<numberOfComponents; n++ )
      {
        menu[j]=sPrintF(buff,"%s (on)",(const char*)u.getName(u.getComponentBase(0)+n));
	j++;
      }
      componentEnd=j-1;
    }
  }
  if( menu[j-1]!="" )
  {
    printf("ERROR in forming menu, menu[j-1]=%s\n",(const char*)menu[j-1]);
    throw "error";
  }


  FILE *outputFile =NULL;
  DisplayParameters dp;

  // set default prompt
  gi.appendToTheDefaultPrompt("output>");

  for(;;)
  {
    int choice = gi.getMenuItem(menu,answer,"choose a menu item");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="file name (new file)" || answer=="file name (append)" )
    {
      bool openNew= answer=="file name (new file)";
      if( !openNew )
        printf("The solution will be appended to the file if it exists.\n");

      gi.inputString(answer2,"Enter the name for output file (default=output.dat)");
      if( answer2 !="" && answer2!=" ")
	outputFileName=answer2;
      else
	outputFileName="output.dat";
    
      if( openNew )
        outputFile = fopen(outputFileName,"a" );
      else
        outputFile = fopen(outputFileName,"w" );
      if( outputFile==NULL )
	gi.outputString(sPrintF(buff,"ERROR opening the file %s! ",(const char *)outputFileName));
    }
    else if( answer=="close file" )
    {
      fclose(outputFile);
      outputFile=NULL;
    }
    else if( answer=="set value at unused points" )
    {
      printf("Enter a value to assign unused point on the grid (where the mask=0)\n"
             "Enter a <cr> to leave unused points unaltered");
      gi.inputString(answer,sPrintF(buff,"Enter a value to use (default=%e, <cr>=do not set)\n",valueForUnusedPoints));
      if( answer!="" )
      {
	setValueAtUnusedPoints=TRUE;
	sScanF(answer,"%e",&valueForUnusedPoints);
	printf("Using the value %e at unused points\n",valueForUnusedPoints);
      }
      else
      {
	setValueAtUnusedPoints=FALSE;
        printf("Not changing the values at unused points");
	
      }
    }
    else if( answer=="format for reals" )
    {
      printf("Example formats are `%%6.2f ' , `%%12.5e ' \n");
      gi.inputString(answer,sPrintF(buff,"Enter the printf format for reals (current=[%s])\n",(const char*)format));
      if( answer!="" )
      {
	format=answer;
        printf("New format = [%s]\n",(const char *)format);
	dp.set(DisplayParameters::floatFormat,format);
	dp.set(DisplayParameters::doubleFormat,format);
      }
    }
    else if( answer=="number of ghost lines" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter the number of ghost lines of data to save (current=%i)\n",
                numberOfGhostLines));
      if( answer!="" )
      {
        sScanF(answer,"%i",&numberOfGhostLines);
	printf("Saving %i ghost lines (up to as many exist)",numberOfGhostLines);
      }
      
    }
    else if( answer=="add labels" )
    {
      addLabels=TRUE;
      printf("labels will be added to the output.\n");
    }
    else if( answer=="do not add labels" )
    {
      addLabels=FALSE;
      printf("labels will NOT be added to the output.\n");
    }
    else if( answer=="save data" )
    {
      if( outputFile==NULL )
      {
        assert( outputFileName!="" );
        printf("Writing to a new file %s\n",(const char*)outputFileName);
	outputFile = fopen(outputFileName,"w" );
        if( outputFile==NULL )
	{
          gi.outputString(sPrintF(buff,"ERROR opening the file %s! No output generated",(const char *)outputFileName));
	  continue;
	}
      }
      printf("saving results to file %s\n",(const char *)outputFileName);
      
      dp.set(outputFile);
      dp.set(DisplayParameters::labelNoIndicies);
      realMappedGridFunction v;
      const int numberOfComponentsToSave=sum(saveComponent);
      fprintf(outputFile,"%i %i %i               : number of grids, number of dimensions, number of components\n",
                   gc.numberOfGrids(),gc.numberOfDimensions(),numberOfComponentsToSave);
      for( int grid=0; grid<gc.numberOfGrids(); grid++ )
      {
	const IntegerArray & dimension=gc[grid].dimension();
	IntegerArray range = extendedGridIndexRange(gc[grid]);
	for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
	{
	  range(Start,axis)=max(dimension(Start,axis),range(Start,axis)-numberOfGhostLines);
	  range(End  ,axis)=min(dimension(End  ,axis),range(End  ,axis)+numberOfGhostLines);
	}
        Index I1,I2,I3;
        getIndex(range,I1,I2,I3);


        fprintf(outputFile,"%i  %s            : grid number, name \n",grid,(const char*)gc[grid].getName());
        fprintf(outputFile,"%i,%i  %i,%i %i,%i      : dimension \n",dimension(0,0),dimension(1,0),
		dimension(0,1),dimension(1,1),dimension(0,2),dimension(1,2));
	
	const IntegerArray & gridIndexRange=gc[grid].gridIndexRange();
        fprintf(outputFile,"%i,%i  %i,%i %i,%i      : gridIndexRange \n",
                gridIndexRange(0,0),gridIndexRange(1,0),
		gridIndexRange(0,1),gridIndexRange(1,1),gridIndexRange(0,2),gridIndexRange(1,2));
	
        fprintf(outputFile,"%i,%i  %i,%i %i,%i      : bounds saved \n",
                I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
	
	const IntegerArray & bc = gc[grid].boundaryCondition();
        fprintf(outputFile,"%i,%i  %i,%i %i,%i       : boundaryCondition (>0 : physical, 0 : interp, "
                "<0 : periodic)\n",bc(0,0),bc(1,0),
		bc(0,1),bc(1,1),bc(0,2),bc(1,2));

	const IntegerArray & isPeriodic = gc[grid].isPeriodic();
        fprintf(outputFile,"%i,%i,%i                : periodicity (0=not,1=derivative,2=function)\n",isPeriodic(axis1),
                isPeriodic(axis2),isPeriodic(axis3) );
	

        if( saveGeometry(mask) )
	{
          printf("saving the mask array on grid %i\n",grid);
	  if( addLabels )
	    fprintf(outputFile,"mask saved as (((mask(i1,i2,i3),i1=%i,%i),i2=%i,%i),i3=%i,%i)\n",
		    I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
          
	  ::displayMask(gc[grid].mask()(I1,I2,I3),"",outputFile);
	}
        if( saveGeometry(vertex) )
	{
          printf("saving the vertex array on grid %i\n",grid);
          if( addLabels )
	    fprintf(outputFile,"vertex saved as ((((vertex(i1,i2,i3,n),i1=%i,%i),i2=%i,%i),i3=%i,%i),n=0,%i)\n",
		    I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
		    gc.numberOfDimensions());
          
	  ::display(gc[grid].vertex()(I1,I2,I3),"",dp);
	}

	for( int n=0; n<numberOfComponents; n++ )
	{
	  if( saveComponent(n) )
	  {
            printf("saving component [%s] on grid %i\n",(const char*)u.getName(u.getComponentBase(0)+n),grid);
	    if( addLabels )
	      fprintf(outputFile,"%s              : component saved as (((u(i1,i2,i3),i1=%i,%i),i2=%i,%i),i3=%i,%i)\n",
		      (const char*)u.getName(u.getComponentBase(0)+n),
		      I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
            if( setValueAtUnusedPoints )
	    {
	      v.destroy();
	      v=u[grid];
	      where( gc[grid].mask()==0 )
	      {
		v=valueForUnusedPoints;
	      }
	    }
	    else
  	      v.link(u[grid],Range(n,n));
	    ::display(v(I1,I2,I3),"",dp);
	  }
	}
	
      }
    }
    else if( choice>=componentStart && choice<= componentEnd )
    {
      int n=choice-componentStart;
      saveComponent(n)=!saveComponent(n);
      if( saveComponent(n) )
	menu[choice]=sPrintF(buff,"%s (on)",(const char*)u.getName(u.getComponentBase(0)+n));
      else
	menu[choice]=sPrintF(buff,"%s (off)",(const char*)u.getName(u.getComponentBase(0)+n));
      
    }
    else if( answer=="choose all" || answer=="choose none" )
    {
      saveComponent= answer=="choose all" ? TRUE : FALSE;
      for( int n=0; n<numberOfComponents; n++ )
      {
	if( saveComponent(n) )
	  menu[componentStart+n]=sPrintF(buff,"%s (on)",(const char*)u.getName(u.getComponentBase(0)+n));
	else
	  menu[componentStart+n]=sPrintF(buff,"%s (off)",(const char*)u.getName(u.getComponentBase(0)+n));
      }
    }
    else if( answer(0,3)=="mask" )
    {
      saveGeometry(mask)=!saveGeometry(mask);
    }
    else if( answer(0,5)=="vertex" )
    {
      saveGeometry(vertex)=!saveGeometry(vertex);
    }
    else
    {
      printf("unknown response: [%s] \n",(const char*) answer);
      gi.stopReadingCommandFile();
    }
    

  } // end for (;;) 
  


  gi.unAppendTheDefaultPrompt(); // reset defaultPrompt

  if( outputFile!=NULL )
    fclose(outputFile);

  return 0;
}
