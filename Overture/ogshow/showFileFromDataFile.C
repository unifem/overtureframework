#include "Overture.h"
#include "GL_GraphicsInterface.h"
#include "DataPointMapping.h"
#include "MappingInformation.h"
#include "DataFormats.h"
#include "display.h"
#include "Ogen.h"
#include "ShowFileParameter.h"
#include "Ogshow.h"
#include "CompositeGridOperators.h"
#include "ShowFileReader.h"
#include OV_STD_INCLUDE(vector)

int 
getLineFromFile( FILE *file, char s[], int lim);


#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base,I2Base,I3Base;\
  int I1Bound,I2Bound,I3Bound;\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

namespace
{

// =========================================================================================================
// Output the format of the ovText file.
// =========================================================================================================
void 
printOvTextFileFormat( FILE *file=stdout )
{
  fPrintF(file,
	  "The Overture 'ovText' data file format is :\n"
	  "# Header comment lines start with a `#'\n"
	  "nd ng nc hasMask          : [nd=number-of dimensions, number of grids, number of components, hasMask=1/0 if mask is provided or not\n"
	  "component-name-1          : name of first component\n"
	  " ...                                               \n"
	  "component-name-nc         : name of last component\n"
	  "define real parameter name value    : define a parameter\n"
	  "define integer parameter name value : define a parameter\n"
	  " ... more lines starting with define ... \n"
	  "grid-name-1               : name of first grid\n"
	  "nx ny [nz]                : number of grid points for first grid (not including ghost points)\n"
	  "ghost00 ghost10 ghost01 ghost11 [ghost02 ghost12]  : number of ghost points in the data (each face)\n"
	  "bc00 bc10 bc01 bc11 [bc02 bc12]  : boundary conditions\n"
	  "share00 share10 share01 share11 [share02 share12]  : share flags\n"
	  "x000 y000 [z000] [mask000]   : grid point i1=0, i2=0, i3=0 (and optional mask)\n"
	  "x100 y100 [z100] [mask100]   : grid point i1=1, i2=0, i3=0\n"
	  "...                                                        \n"
	  "u000 v000 ...  w000          : components for i1=0, i2=0, i3=0\n"
	  "u100 v100 ...  w100          : components for i1=1, i2=0, i3=0\n"
	  "...                                                        \n"
	  "grid-name-2               : name of grid 2\n"
	  "...                                                        \n");
}

}



// ===============================================================================
// /Description:
//    Create a show file from a data file.
//
// /gi (input) : graphics interface
// /showFileName (output) : name of the show file that was created.
// 
// ===============================================================================
int
showFileFromDataFile( GenericGraphicsInterface & gi, aString & showFileName )
{

  CompositeGrid cg;
  realCompositeGridFunction v;
  RealArray par(20);

  GraphicsParameters psp;                       // create an object that is used to pass parameters
  MappingInformation mapInfo;
  mapInfo.graphXInterface=&gi;
  gi.appendToTheDefaultPrompt("showFileFromDataFile>"); // set the default prompt

  showFileName = "myShowFile.show";


  GUIState dialog;

  dialog.setWindowTitle("Show File from a Data File");
  dialog.setExitCommand("exit","Exit");


  aString pushButtonCommands[] = {"save show file",
                                  "data file format",
				  ""};
  int numRows=3;
  dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 

  aString tbCommands[] = {"extrapolate unassigned ghost",
                          "convert to primitive variables",
                          // "compute stresses from displacements",
			  //  "convert to vertex centered",
  			  ""};

  bool extrapolateGhost=true;
  bool convertToPrimitive=true;
  bool convertToVertexCentered=false;
  // bool computeStress=false;

  int tbState[10];
  tbState[0] = extrapolateGhost;
  tbState[1] = convertToVertexCentered;
  // tbState[2] = computeStress;
  // tbState[3] = convertToVertexCentered;
  

  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

  // **  dialog.addInfoLabel("Volume = 0");

  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  aString name="none", fName="none";
  aString showFileLabel[2];
  showFileLabel[0]="Compressible Navier Stokes";
  showFileLabel[1]="Steady State";
  
  int nt=0;
  textCommands[nt] = "data file name:";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s", (const char*)name);  nt++; 


  textCommands[nt] = "show file name:";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)showFileName);  nt++; 

  textCommands[nt] = "show file label 1:";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)showFileLabel[0]);  nt++; 
  textCommands[nt] = "show file label 2:";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s",(const char*)showFileLabel[1]);  nt++; 


  // null strings terminate list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  real machNumber=1., alpha=0., reynoldsNumber=0., t=0., gamma=1.4, Rg=1.;
  // default parameters for solid mechanics:
  real lambda=1., mu=1.; 

  int numberOfComponents=0;
  int numberOfExtraComponents=0;  // extra derived components we tack on (e.g. stress from displacement)
  aString *componentName=NULL;

  aString answer, line;
  aString fileName="";
  bool readFile=false;
  int i1,i2,i3;
  
  ListOfShowFileParameters showFileParams;

  gi.pushGUI(dialog);

  for( int it=0;; it++ )
  {
     
   gi.getAnswer(answer, "");
   
   int len;
   if( dialog.getTextValue(answer,"data file name:","%s",fileName) )
   {
     readFile=true;
     printf(" Looking for file = [%s]\n",(const char*)fileName);
   }
   else if( dialog.getToggleValue(answer,"extrapolate unassigned ghost",extrapolateGhost) ){}//
   else if( dialog.getToggleValue(answer,"convert to primitive variables",convertToPrimitive) ){}//
   // else if( dialog.getToggleValue(answer,"compute stresses from displacements",computeStress) ){}//
   // else if( dialog.getToggleValue(answer,"convert to vertex centered",convertToVertexCentered) ){}//
   else if( dialog.getTextValue(answer,"show file name:","%s",showFileName) ){}//
   else if( dialog.getTextValue(answer,"show file label 1:","%s",showFileLabel[0]) ){}//
   else if( dialog.getTextValue(answer,"show file label 2:","%s",showFileLabel[1]) ){}//
//    else if( answer=="component names" )
//    {
//      // get names for components
//      for(int n=0; n<maxNumberOfComponents; n++ )
//      {
//        gi.inputString(answer,sPrintF("Enter the name of component %i (or `done' to finish)",n));
//        if( answer=="done" )
// 	 break;
//        int i=0;
//        while( answer[i]==' ' && i<answer.length() ) i++;
       
//        componentName[n]=answer(i,answer.length()-1);
//      }
//    }
   else if( answer=="save show file" )
   {
     printf("Saving the current grid and solution to the show file [%s]\n",(const char*)showFileName);
     
     Ogshow show(showFileName);
     show.saveGeneralComment("Grids and solutions and from an Overture data file"); 

       

     // showFileParams.push_back(ShowFileParameter("reynoldsNumber",reynoldsNumber));
     // showFileParams.push_back(ShowFileParameter("machNumber",machNumber));
     // showFileParams.push_back(ShowFileParameter("gamma",gamma));
     // showFileParams.push_back(ShowFileParameter("Rg",Rg));

     for( int c=0; c<numberOfComponents; c++ )
     {
       showFileParams.push_back(ShowFileParameter(componentName[c],c));
     }

     int numberOfSpecies=0;
     showFileParams.push_back(ShowFileParameter("numberOfSpecies",numberOfSpecies));

     show.saveGeneralParameters(showFileParams);

     show.startFrame();                      // start a new frame
     show.saveComment(0,showFileLabel[0]);   // comment 0 (shown on plot)
     show.saveComment(1,showFileLabel[1]);   // comment 1 (shown on plot)
     show.saveSolution( v ); 

     show.close();
     
   }
   else if ( answer=="data file format" )
   {
     printOvTextFileFormat();
   }
   else if ( answer=="exit" )
   {
     break;
   }
   else 
   {
     printF("Unknown answer=[%s]\n",(const char*)answer);
     gi.stopReadingCommandFile();
     
   }


   if( readFile )
   {
     readFile=false;
     
     // read in the data file

  
     FILE *file = fopen ((const char*)fileName, "r");
     if( file==NULL )
     {
       printF("ERROR opening file=[%s]. File not found\n",(const char*)fileName);
       continue;
     }
     
     
     const int buffLength=1024;
     char line[buffLength];
     int numRead=getLineFromFile(file,line,buffLength);  // read a line from the file.
     while( numRead>0 && line[0]=='#' )
     { // skip comments
       printF("Header comment: %s\n",(const char*)line);
       numRead=getLineFromFile(file,line,buffLength);
     }
     if( numRead==0 )
     {
       printF("ERROR: premature EOF!\n");
       fclose(file);
       continue;
     }
  
     int nd=0, ng=0, nc=0, hasMask=0;
     sScanF(line,"%i %i %i %i",&nd,&ng,&nc,&hasMask);

     printF(" nd=%i, ng=%i, nc=%i, hasMask=%i\n",nd,ng,nc,hasMask);

     if( nd<2 || nd>3 )
     {
       printF("ERROR: nd=%i should be 2 or 3!\n",nd);
       fclose(file);
       continue;
     }
     if( ng<=0 || ng>100000 )
     {
       printF("ERROR: number of grids ng=%i is invalid\n",ng);
       fclose(file);
       continue;
     }
     if( nc<=0 || nc>100000 )
     {
       printF("ERROR: number of components nc=%i is invalid\n",nc);
       fclose(file);
       continue;
     }
     // if( computeStress )
     // {
     //   numberOfExtraComponents=(nd*(nd+1))/2;
     // }

     numberOfComponents=nc + numberOfExtraComponents;

     delete [] componentName;
     componentName = new aString [numberOfComponents];
     for( int c=0; c<nc; c++ )
     {
       int numRead=getLineFromFile(file,line,buffLength);
       answer=line;
       int i=0;
       while( i<numRead && line[i]==' ' ) i++; // skip leading blanks
       componentName[c]=answer(i,numRead-1);
       printF("Component %i = [%s]\n",c,(const char*)componentName[c]);
     }

     // -- read user defined parameters ---
     
     bool done=false;
     while( !done )
     {
       int numRead=getLineFromFile(file,line,buffLength);
       answer=line;
       int i=0;
       while( i<numRead && line[i]==' ' ) i++; // skip leading blanks
       answer=answer(i,numRead-1);
       if( (len=answer.matches("define")) )
       {
	 // printF("define a parameter: [%s]\n",(const char*)answer);

	 if( (len=answer.matches("define real parameter"))     ||
             (len=answer.matches("define integer parameter"))  ||
             (len=answer.matches("define string parameter")) )
	 {
	   // EquationDomain & equationDomain = equationDomainList[activeEquationDomain];  // The active domain

	   const int length=answer.length();
	   int iStart=len;
	   while(  iStart<length && answer[iStart]==' ' ) iStart++;  // skip leading blanks
	   int iEnd=iStart;
	   while( iEnd<length && answer[iEnd]!=' ' ) iEnd++;       // now look for a blank to end the name
	   iEnd--;
	   if( iStart<=iEnd )
	   {
	     aString name = answer(iStart,iEnd);
	     if( answer.matches("define real parameter") )
	     {
	       real value;
	       sScanF(answer(iEnd+1,answer.length()),"%e",&value);
	       printF(" Adding the real parameter [%s] with value [%e]\n",(const char*)name,value);
	       showFileParams.push_back(ShowFileParameter(name,value));
	       if( name=="lambda" )
	       {
		 lambda=value;
	       }
	       if( name=="mu" )
	       {
		 mu=value;
	       }
	       
	     }
	     else if( answer.matches("define integer parameter") )
	     {
	       int value;
	       sScanF(answer(iEnd+1,answer.length()),"%i",&value);
	       printF(" Adding the integer parameter [%s] with value [%i]\n",(const char*)name,value);
	       showFileParams.push_back(ShowFileParameter(name,value));
	     }
	     else
	     {
	       iStart=iEnd+1;
	       iEnd=length-1;
	       while( iStart<iEnd && answer[iStart]==' ' ) iStart++;
	       while( iEnd>iStart && answer[iEnd]==' ' ) iEnd--;
	       aString value=answer(iStart,iEnd);
	  
	       printF(" Adding the string parameter [%s] with value [%s]\n",(const char*)name,(const char*)value);
	       showFileParams.push_back(ShowFileParameter(name,value));
	     }
	   }
	   else
	   {
	     printF("ERROR parsing the define parameter statement: answer=[%s]\n",(const char*) answer);
	   }
	 }
	 else
	 {
	  printF("ERROR parsing the define parameter statement: answer=[%s]\n",(const char*) answer);
	 }
       }
       else
       {
	 done=true;
       }
     }

     IntegerArray ghost(2,3), gid(2,3), dim(2,3), bc(2,3), share(2,3);
     ghost=0; gid=0, dim=0; bc=1; share=0;
     Index I1,I2,I3;
     int nv[3], &nx=nv[0], &ny=nv[1], &nz=nv[2];
     
     realArray *ug = new realArray[ng];
     intArray *maskArray=NULL;
     if( hasMask )
       maskArray = new intArray [ng];

     for( int grid=0; grid<ng; grid++ )
     {
       
       if( grid>0 ) getLineFromFile(file,line,buffLength);
       aString gridName=line;

       nx=0, ny=0, nz=0;
       getLineFromFile(file,line,buffLength);
       sScanF(line,"%i %i %i",&nx,&ny,&nz);

       getLineFromFile(file,line,buffLength);
       sScanF(line,"%i %i %i %i %i %i",&ghost(0,0),&ghost(1,0),&ghost(0,1),&ghost(1,1),&ghost(0,2),&ghost(1,2));

       getLineFromFile(file,line,buffLength);
       sScanF(line,"%i %i %i %i %i %i",&bc(0,0),&bc(1,0),&bc(0,1),&bc(1,1),&bc(0,2),&bc(1,2));

       getLineFromFile(file,line,buffLength);
       sScanF(line,"%i %i %i %i %i %i",&share(0,0),&share(1,0),&share(0,1),&share(1,1),&share(0,2),&share(1,2));

       printF("Grid %i = [%s], nx=%i, ny=%i, nz=%i, bc=[%i,%i][%i,%i][%i,%i], share=[%i,%i][%i,%i][%i,%i], \n"
              "ghost=[%i,%i][%i,%i][%i,%i]\n",grid,(const char*)gridName,nx,ny,nz,
	      bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2),
              share(0,0),share(1,0),share(0,1),share(1,1),share(0,2),share(1,2),
              ghost(0,0),ghost(1,0),ghost(0,1),ghost(1,1),ghost(0,2),ghost(1,2)  );
       for( int axis=0; axis<nd; axis++ )
       {
         gid(0,axis)=0;
	 gid(1,axis)=nv[axis]-1;
	 dim(0,axis)=gid(0,axis)-ghost(0,axis);
	 dim(1,axis)=gid(1,axis)+ghost(1,axis);
	 
       }
       getIndex(dim,I1,I2,I3);
       realArray x(I1,I2,I3,nd);
       if( !hasMask )
       { // read grid points 
	 FOR_3D(i1,i2,i3,I1,I2,I3)
	 {
	   getLineFromFile(file,line,buffLength);
	   if( nd==2 )
	     sScanF(line,"%e %e",&x(i1,i2,i3,0),&x(i1,i2,i3,1));
	   else
	     sScanF(line,"%e %e %e",&x(i1,i2,i3,0),&x(i1,i2,i3,1),&x(i1,i2,i3,2));
	 }
       }
       else
       { // read grid points and mask 
         intArray & m = maskArray[grid];
	 m.redim(I1,I2,I3);
	 FOR_3D(i1,i2,i3,I1,I2,I3)
	 {
	   getLineFromFile(file,line,buffLength);
	   if( nd==2 )
	     sScanF(line,"%e %e %i",&x(i1,i2,i3,0),&x(i1,i2,i3,1),&m(i1,i2,i3));
	   else
	     sScanF(line,"%e %e %e %i",&x(i1,i2,i3,0),&x(i1,i2,i3,1),&x(i1,i2,i3,2),&m(i1,i2,i3));
	 }
       }
	 
       
       DataPointMapping & dpm = *new DataPointMapping(); dpm.incrementReferenceCount();
       dpm.setDomainDimension(nd);
       dpm.setRangeDimension(nd);
       dpm.setOrderOfInterpolation(4); // mapping is defined by piecewise cubics
       
       for( int side=0; side<=1; side++ )for( int axis=0; axis<nd; axis++ )
       {
	 dpm.setBoundaryCondition(side,axis,bc(side,axis));
	 dpm.setShare(side,axis,share(side,axis));
       }

       dpm.setDataPoints(x,3,nd,0,gid);

       dpm.setName(Mapping::mappingName,gridName);
       mapInfo.mappingList.addElement(dpm);

       dpm.decrementReferenceCount();


       realArray & u = ug[grid];
       u.redim(I1,I2,I3,nc);
       if( nc>10 )
       {
	 printF("WARNING: nc>10 : only 10 components will be read in. Fix me Bill!\n");
       }
       FOR_3D(i1,i2,i3,I1,I2,I3)
       {
	 getLineFromFile(file,line,buffLength);
	 if( nc==1 )
	   sScanF(line,"%e",&u(i1,i2,i3,0));
	 else if( nc==2 )
	   sScanF(line,"%e %e",&u(i1,i2,i3,0),&u(i1,i2,i3,1));
	 else if( nc==3 )
	   sScanF(line,"%e %e %e",&u(i1,i2,i3,0),&u(i1,i2,i3,1),&u(i1,i2,i3,2));
	 else if( nc==4 )
	   sScanF(line,"%e %e %e %e",&u(i1,i2,i3,0),&u(i1,i2,i3,1),&u(i1,i2,i3,2),&u(i1,i2,i3,3));
	 else if( nc==5 )
	   sScanF(line,"%e %e %e %e %e",&u(i1,i2,i3,0),&u(i1,i2,i3,1),&u(i1,i2,i3,2),&u(i1,i2,i3,3),&u(i1,i2,i3,4));
	 else if( nc==6 )
	   sScanF(line,"%e %e %e %e %e %e",&u(i1,i2,i3,0),&u(i1,i2,i3,1),&u(i1,i2,i3,2),&u(i1,i2,i3,3),&u(i1,i2,i3,4),&u(i1,i2,i3,5));
	 else if( nc==7 )
	   sScanF(line,"%e %e %e %e %e %e %e",&u(i1,i2,i3,0),&u(i1,i2,i3,1),&u(i1,i2,i3,2),&u(i1,i2,i3,3),&u(i1,i2,i3,4),&u(i1,i2,i3,5),&u(i1,i2,i3,6));
	 else if( nc==8 )
	   sScanF(line,"%e %e %e %e %e %e %e %e",&u(i1,i2,i3,0),&u(i1,i2,i3,1),&u(i1,i2,i3,2),&u(i1,i2,i3,3),&u(i1,i2,i3,4),&u(i1,i2,i3,5),&u(i1,i2,i3,6),&u(i1,i2,i3,7));
	 else if( nc==9 )
	   sScanF(line,"%e %e %e %e %e %e %e %e %e",&u(i1,i2,i3,0),&u(i1,i2,i3,1),&u(i1,i2,i3,2),&u(i1,i2,i3,3),&u(i1,i2,i3,4),&u(i1,i2,i3,5),&u(i1,i2,i3,6),&u(i1,i2,i3,7),&u(i1,i2,i3,8));
	 else 
	   sScanF(line,"%e %e %e %e %e %e %e %e %e %e",&u(i1,i2,i3,0),&u(i1,i2,i3,1),&u(i1,i2,i3,2),&u(i1,i2,i3,3),&u(i1,i2,i3,4),&u(i1,i2,i3,5),&u(i1,i2,i3,6),&u(i1,i2,i3,7),&u(i1,i2,i3,8),&u(i1,i2,i3,9));
       }
       
     } // end for grid
     
     fclose(file);


     // Create an overlapping grid generator
     Ogen ogen(gi);

     int numberOfGrids = mapInfo.mappingList.getLength();
     // indicate which mappings should be used in the CompositeGrid (use all in this case)
     IntegerArray mapList(numberOfGrids);
     for( int grid=0; grid<numberOfGrids; grid++ )
       mapList(grid)=grid;


     // Put the mappings into the CompositeGrid
     ogen.buildACompositeGrid(cg,mapInfo,mapList);


     for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
     {
       MappedGrid & mg = cg[grid];

       // if( !convertToVertexCentered )
       //   cg[grid].changeToAllCellCentered();            // make a cell centered grid
       mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask);  
  
       if( hasMask )
       {
         intArray & mask = mg.mask();
         getIndex(mg.dimension(),I1,I2,I3);
	 // printf("fill in the mask array...\n");
    
	 mask(I1,I2,I3)=maskArray[grid](I1,I2,I3);
	 // mg.mask().display("mask");
    
       }
       // tell the MappedGrid that the mask was computed
       mg->computedGeometry |= CompositeGrid::THEmask;
     }
     delete [] maskArray;

      // tell the CG that the mask was computed
      cg->computedGeometry |= CompositeGrid::THEmask;

//      if( computeStress )
//      { // assign the component names for the stress tensor
//        int c=nc;
//        if( nd==2 )
//        {
// 	 componentName[c]="s11"; c++;
// 	 componentName[c]="s12"; c++;
// 	 componentName[c]="s22"; c++;
//        }
//        else
//        {
// 	 componentName[c]="s11"; c++;
// 	 componentName[c]="s12"; c++;
// 	 componentName[c]="s13"; c++;
// 	 componentName[c]="s22"; c++;
// 	 componentName[c]="s23"; c++;
// 	 componentName[c]="s33"; c++;
//        }
       
//      }

     Range all;
     Range C=numberOfComponents;
     v.updateToMatchGrid(cg,all,all,all,C);
     v.setName("q");                              // give names to grid function ...
     for( int c=0; c<numberOfComponents; c++ )
       v.setName(componentName[c],c);

     for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
     {
       MappedGrid & mg = cg[grid];
       realArray & vg = v[grid];
       realArray & u = ug[grid];
       vg=0;
       I1=u.dimension(0), I2=u.dimension(1), I3=u.dimension(2);

       C=nc;
       vg(I1,I2,I3,C)=u(I1,I2,I3,C);

     }
     if( extrapolateGhost )
     {
       CompositeGridOperators cgop(cg); 
       v.setOperators(cgop);
       // fix me: only extrap ghost values that are not provided
       BoundaryConditionParameters extrapParams;
       extrapParams.orderOfExtrapolation=3;  // what should this be? 
       v.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,extrapParams); 
       extrapParams.ghostLineToAssign=2;
       v.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,extrapParams); 
       
       v.finishBoundaryConditions(extrapParams);
     }
     

  
     delete [] ug;

     gi.erase();
     PlotIt::contour(gi, v,psp);

     
   } // end if read file
   
  } // end for it 
  
  delete [] componentName;
   
  gi.unAppendTheDefaultPrompt(); // reset defaultPrompt
  gi.erase();
  gi.popGUI();  // pop dialog



  return 0;
}


// ===============================================================================
// /Description:
//    Save a solution into an Overture text file (ovText format).
//
// /u (input) : grid function to save
// /gi (input) : graphics interface
// /pShowFileReader (input) : optionally provide a ShowFileReader (if u was read from a show file). The
//          ShowFileReader is used to access extra information about the solution.
// 
// ===============================================================================
int
saveOvertureTextFile( realCompositeGridFunction & u, 
                      GenericGraphicsInterface & gi, 
                      ShowFileReader *pShowFileReader=NULL )
{
  
  CompositeGrid & cg = *u.getCompositeGrid();

  GraphicsParameters psp;                       // create an object that is used to pass parameters

  gi.appendToTheDefaultPrompt("ovText>"); // set the default prompt

  aString ovTextFileName = "ovText.dat";


  GUIState dialog;

  dialog.setWindowTitle("Save an Overture Text File");
  dialog.setExitCommand("exit","Exit");

  aString pushButtonCommands[] = {"save ovText file",
                                  "data file format",
				  ""};
  int numRows=2;
  dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows );

  int hasMask=1;
  aString tbCommands[] = {"save mask",
  			  ""};
  int tbState[10];
  tbState[0] = hasMask;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);


  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textCommands[nt] = "ovText file name:";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s", (const char*)ovTextFileName);  nt++; 

  textCommands[nt] = "Add comment:";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s","my header comment");  nt++; 

  // null strings terminate list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);


  int numberOfComponents= u.getComponentBound(0)-u.getComponentBase(0)+1;

  aString answer, line;
  bool writeFile=false;
  int i1,i2,i3;
  
  //  ListOfShowFileParameters showFileParams;

  std::vector<aString> headerComments;

  gi.pushGUI(dialog);

  for( int it=0;; it++ )
  {
     
   gi.getAnswer(answer, "");
   
   int len;
   if( dialog.getTextValue(answer,"ovText file name:","%s",ovTextFileName) )
   {
   }
   else if( dialog.getToggleValue(answer,"save mask",hasMask) ){}//
   else if( dialog.getTextValue(answer,"Add comment:","%s",line) )
   {
     // Put header comments into a list .. finish me ...
     headerComments.push_back(line);
   }
   else if ( answer=="data file format" )
   {
     printOvTextFileFormat();
   }
   else if ( answer=="exit" )
   {
     break;
   }
   else if( answer=="save ovText file" )
   {
     writeFile=true;
   }
   else 
   {
     printF("Unknown answer=[%s]\n",(const char*)answer);
     gi.stopReadingCommandFile();
     
   }


   if( writeFile )
   {
     writeFile=false;
     
     // --- write the ovText file ------

  
     FILE *file = fopen ((const char*)ovTextFileName, "w");
     if( file==NULL )
     {
       printF("ERROR opening file=[%s].\n",(const char*)ovTextFileName);
       continue;
     }
     
     
     // -- write header comments
     // Get the current date
     time_t *tp= new time_t;
     time(tp);
     const char *dateString = ctime(tp);
     fPrintF(file,"# Overture text file written by saveOvertureTextFile. Created on %s",dateString);
     delete tp;

     // -- write user defined header comments
     for( int i=0; i<headerComments.size(); i++ )
     {
       fPrintF(file,"# %s\n",(const char*)headerComments[i]);
     }

     fPrintF(file,"%i %i %i %i\n",cg.numberOfDimensions(),cg.numberOfComponentGrids(),numberOfComponents,hasMask);

     // -- write solution component names
     for( int c=0; c<numberOfComponents; c++ )
     {
       fPrintF(file,"%s\n",(const char*)u.getName(c));
       
     }

     // -- write user defined parameters ---
     
// 	 if( (len=answer.matches("define real parameter"))     ||
//              (len=answer.matches("define integer parameter"))  ||
//              (len=answer.matches("define string parameter")) )

     // fPrintF(file,"define integer parameter densityComponent %i\n",rc);


     for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
     {
       MappedGrid & mg = cg[grid];
       mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
       
       const IntegerArray & gid  = mg.gridIndexRange();
       const IntegerArray & dim  = mg.dimension();
       const IntegerArray & bc   = mg.boundaryCondition();
       const IntegerArray & share= mg.sharedBoundaryFlag();
       const realArray    & x    = mg.vertex();
       const intArray     & mask = mg.mask();
       
       // -- write the grid name
       fPrintF(file,"%s\n",(const char*)mg.getName());

       // -- write number of grid points (not including ghost points)
       fPrintF(file,"%i %i %i\n",gid(1,0)-gid(0,0)+1, gid(1,1)-gid(0,1)+1, gid(1,2)-gid(0,2)+1);
       
       // -- write number of ghost points:
       fPrintF(file,"%i %i %i %i %i %i\n",
	       gid(0,0)-dim(0,0), dim(1,0)-gid(1,0),  
	       gid(0,1)-dim(0,1), dim(1,1)-gid(1,1),  
	       gid(0,2)-dim(0,2), dim(1,2)-gid(1,2));
       

       // -- boundary conditions
       fPrintF(file,"%i %i %i %i %i %i\n",bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2));

       // -- share
       fPrintF(file,"%i %i %i %i %i %i\n",share(0,0),share(1,0),share(0,1),share(1,1),share(0,2),share(1,2));

       Index I1,I2,I3;
       getIndex(mg.dimension(),I1,I2,I3);
       if( !hasMask )
       { // -- write grid points 
	 FOR_3D(i1,i2,i3,I1,I2,I3)
	 {
	   if( cg.numberOfDimensions()==2 )
	     fPrintF(file,"%e %e\n",x(i1,i2,i3,0),x(i1,i2,i3,1));
	   else
	     fPrintF(file,"%e %e %e\n",x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2));
	 }
       }
       else
       { // -- write grid points and mask 
	 FOR_3D(i1,i2,i3,I1,I2,I3)
	 {
	   if( cg.numberOfDimensions()==2 )
	     fPrintF(file,"%e %e %i\n",x(i1,i2,i3,0),x(i1,i2,i3,1),mask(i1,i2,i3));
	   else
	     fPrintF(file,"%e %e %e %i\n",x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),mask(i1,i2,i3));
	 }
       }

       // -- write the solution
       const realArray & ug = u[grid];
       if( numberOfComponents>10 )
       {
	 printF("WARNING: numberOfComponents>10 : only 10 components will be written. Fix me Bill!\n");
       }
       const int nc = numberOfComponents;
       FOR_3D(i1,i2,i3,I1,I2,I3)
       {
	 if( nc==1 )
	   fPrintF(file,"%e\n",ug(i1,i2,i3,0));
	 else if( nc==2 )
	   fPrintF(file,"%e %e\n",ug(i1,i2,i3,0),ug(i1,i2,i3,1));
	 else if( nc==3 )
	   fPrintF(file,"%e %e %e\n",ug(i1,i2,i3,0),ug(i1,i2,i3,1),ug(i1,i2,i3,2));
	 else if( nc==4 )
	   fPrintF(file,"%e %e %e %e\n",ug(i1,i2,i3,0),ug(i1,i2,i3,1),ug(i1,i2,i3,2),ug(i1,i2,i3,3));
	 else if( nc==5 )
	   fPrintF(file,"%e %e %e %e %e\n",ug(i1,i2,i3,0),ug(i1,i2,i3,1),ug(i1,i2,i3,2),ug(i1,i2,i3,3),ug(i1,i2,i3,4));
	 else if( nc==6 )
	   fPrintF(file,"%e %e %e %e %e %e\n",ug(i1,i2,i3,0),ug(i1,i2,i3,1),ug(i1,i2,i3,2),ug(i1,i2,i3,3),ug(i1,i2,i3,4),ug(i1,i2,i3,5));
	 else if( nc==7 )
	   fPrintF(file,"%e %e %e %e %e %e %e\n",ug(i1,i2,i3,0),ug(i1,i2,i3,1),ug(i1,i2,i3,2),ug(i1,i2,i3,3),ug(i1,i2,i3,4),ug(i1,i2,i3,5),ug(i1,i2,i3,6));
	 else if( nc==8 )
	   fPrintF(file,"%e %e %e %e %e %e %e %e\n",ug(i1,i2,i3,0),ug(i1,i2,i3,1),ug(i1,i2,i3,2),ug(i1,i2,i3,3),ug(i1,i2,i3,4),ug(i1,i2,i3,5),ug(i1,i2,i3,6),ug(i1,i2,i3,7));
	 else if( nc==9 )
	   fPrintF(file,"%e %e %e %e %e %e %e %e %e\n",ug(i1,i2,i3,0),ug(i1,i2,i3,1),ug(i1,i2,i3,2),ug(i1,i2,i3,3),ug(i1,i2,i3,4),ug(i1,i2,i3,5),ug(i1,i2,i3,6),ug(i1,i2,i3,7),ug(i1,i2,i3,8));
	 else 
	   fPrintF(file,"%e %e %e %e %e %e %e %e %e %e\n",ug(i1,i2,i3,0),ug(i1,i2,i3,1),ug(i1,i2,i3,2),ug(i1,i2,i3,3),ug(i1,i2,i3,4),ug(i1,i2,i3,5),ug(i1,i2,i3,6),ug(i1,i2,i3,7),ug(i1,i2,i3,8),ug(i1,i2,i3,9));
       }
       
     } // end for grid
     
     fclose(file);
     printF("... wrote ovText file=[%s]. numberOfComponentGrids=%i, numberOfComponents=%i.\n",
             (const char*)ovTextFileName,cg.numberOfComponentGrids(),numberOfComponents );
     

   } // end if write file
   
  } // end for it 

  gi.unAppendTheDefaultPrompt(); // reset defaultPrompt
  gi.erase();
  gi.popGUI();  // pop dialog



  return 0;
}

