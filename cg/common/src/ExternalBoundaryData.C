#include "ExternalBoundaryData.h"
#include "HDF_DataBase.h"
#include "GenericGraphicsInterface.h"
#include "display.h"

ExternalBoundaryData::
ExternalBoundaryData()
{
  externalFileType=probeBoundingBox;
  pdb=NULL;
  numberOfTimes=-1;
  orderOfTimeInterpolation=2;
  orderOfSpaceInterpolation=2;
  current=-1;
}

ExternalBoundaryData::
~ExternalBoundaryData()
{
  if( pdb!=NULL )
  {
    pdb->unmount();
    delete pdb;
  }
  
}


int ExternalBoundaryData::
getBoundaryData( real t, CompositeGrid & cg, const int side, const int axis, const int grid, RealArray & bd )
{

  MappedGrid & mg = cg[grid];
  Index Ib1,Ib2,Ib3;
  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

  assert( pdb!=NULL );
  GenericDataBase & dir = *pdb;
  GenericDataBase & subDir = *dir.virtualConstructor();

  current=0;
  while( t>times(current) && current<numberOfTimes-1 )
    current++;

  printF("ExternalBoundaryData::getBoundaryData: t=%9.3e, current=%i, times(current)=%9.3e\n",
          t,current,times(current));

  aString dirName; sPrintF(dirName,"probeData%i",current);
  dir.find(subDir,dirName,"ProbeData");

  RealArray ub;
  subDir.get(ub,sPrintF("uFace%i%i",side,axis));
  printF(" ProbeData: look for data on face Ib1=[%i,%i] Ib2=[%i,%i] Ib3=[%i,%i]\n",
         Ib1.getBase(),Ib1.getBound(), Ib2.getBase(),Ib2.getBound(), Ib3.getBase(),Ib3.getBound() );
  printF(" ProbeData for face (side,axis)=(%i,%i) ub = [%i,%i][%i,%i][%i,%i][%i,%i]\n",
	 side,axis,ub.getBase(0),ub.getBound(0),ub.getBase(1),ub.getBound(1),ub.getBase(2),ub.getBound(2),
         ub.getBase(3),ub.getBound(3));
  
  Range C=ub.dimension(3);

  Index Jb1,Jb2,Jb3;
  Jb1=Ib1; Jb2=Ib2; Jb3=Ib3;
  
  // fudge:
  // Jb1=Range(100,100);
   
  bd(Ib1,Ib2,Ib3,C)=ub(Jb1,Jb2,Jb3,C);
  

  delete &subDir;


  return 0;
}





// =================================================================================
/// \brief Open a external file with time dependent boundary data
// =================================================================================
int ExternalBoundaryData::
update( GenericGraphicsInterface & gi )
{

  GUIState gui;
  DialogData & dialog = gui;
  
  dialog.setWindowTitle("ExternalBoundaryData");
  dialog.setExitCommand("continue", "continue");


  aString pbCommands[] = {"open a file",
			  ""};
  int numRows=2;
  dialog.setPushButtons( pbCommands, pbCommands, numRows ); 

  // These are the file types we know about:
  aString opCommands[] = {"probe bounding box",
                          "" };

  dialog.addOptionMenu("file type:", opCommands, opCommands, (int)externalFileType);

  // ----- Text strings ------
  const int numberOfTextStrings=10;
  aString textCommands[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textCommands[nt] = "order of time interpolation"; 
  sPrintF(textStrings[nt], "%i",orderOfTimeInterpolation);  nt++;  

  textCommands[nt] = "order of space interpolation"; 
  sPrintF(textStrings[nt], "%i",orderOfSpaceInterpolation);  nt++;  


  // null strings terminal list
  textCommands[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textCommands, textStrings);

  aString answer;
  int len=0;
  
  gi.pushGUI(gui);
    
  for(;;)
  {
    gi.getAnswer(answer,"");

    if( answer=="continue" || answer=="exit" )
    {
      break;
    }
    else if( answer=="probe bounding box" )
    {
      externalFileType=probeBoundingBox;
    }
    else if( dialog.getTextValue(answer,"order of time interpolation","%i",orderOfTimeInterpolation) ){} //
    else if( dialog.getTextValue(answer,"order of space interpolation","%i",orderOfSpaceInterpolation) ){} //
    else if( answer=="open a file" )
    {
      
      aString probeFileName;
      gi.inputString(probeFileName,"Enter the name of the probe bounding box data file");
      printF("ExternalBoundaryData::probeFileName=[%s]\n",(const char*)probeFileName);
      
      if( pdb==NULL )
      {
	pdb = new HDF_DataBase; 

	printF("ExternalBoundaryData: opening the data base file %s for the bounding box probe info.\n",
	       (const char*)probeFileName);

	HDF_DataBase & db = ( HDF_DataBase &)(*pdb);
	db.setMode(GenericDataBase::noStreamMode);
	db.mount(probeFileName,"R");    // open the data base, R=read-only
      }
      assert( pdb!=NULL );
      HDF_DataBase & db = ( HDF_DataBase &)(*pdb);

      numberOfTimes=-1;
      db.get(numberOfTimes,"numberOfTimes");
      db.get(times,"times");

      printF(" Found probe data: numberOfTimes=%i\n",numberOfTimes);
      ::display(times,"The solution was saved at these times","%6.3f ");

      real dx[3], pxBounds[6];
      #define xBounds(side,axis) pxBounds[(side)+2*(axis)]
      db.get(dx,"dx",3);
      db.get(pxBounds,"xBounds",6);

      printF("Bounding box: [%9.3e,%9.3e][%9.3e,%9.3e][%9.3e,%9.3e]\n",
	     xBounds(0,0),xBounds(1,0),xBounds(0,1),xBounds(1,1),xBounds(0,2),xBounds(1,2));

    }
    else
    {
      printF("Unknown answer=[%s]\n",(const char*)answer);
    }
  } 
  
  gi.popGUI(); 

  return 0;
}


