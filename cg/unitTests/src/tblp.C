// ====================================================================================
///  \file tblp.C
///  \brief test program for the Boundary Layer Profile class
// ===================================================================================


#include "Overture.h"
#include "SquareMapping.h"
#include "PlotStuff.h"
#include "display.h"
#include "BoundaryLayerProfile.h"


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


int 
main(int argc, char *argv[]) 
{
  Mapping::debug=0;

  Overture::start(argc,argv);  // initialize Overture and A++/P++

  printF("Usage: tbl -nu=<> -debug=<> -cmd=<> ... \n" );


  BoundaryLayerProfile profile;

  int & debug = BoundaryLayerProfile::debug;
  debug = 1;
  int plotOption=1;

  real nu=1.e-3;
  real U=1.;
   
  aString commandFileName="";

  char buff[180];
  int len=0;
  if( argc > 1 )
  { // look at arguments for "-noplot" or "-cfl=<value>"
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" )
        plotOption=false;
      else if( len=line.matches("-nu=") )
      {
        sScanF(line(len,line.length()-1),"%e",&nu);
	printF("nu = %6.2f\n",nu);
      }
      else if( len=line.matches("-U=") )
      {
        sScanF(line(len,line.length()-1),"%e",&U);
	printF("U = %6.2f\n",U);
      }
      else if( len=line.matches("-debug=") )
      {
        sScanF(line(len,line.length()-1),"%i",&debug);
	printF("debug = %i\n",debug);
        // RigidBodyMotion::debug=debug;
      }
      else if( len=line.matches("-cmd=") )
      {
        commandFileName=line(len,line.length()-1);
        printF("tbl: reading commands from file [%s]\n",(const char*)commandFileName);
      }
    }
  }

  PlotStuff gi(plotOption,"Boundary Layer Profile Tester");
  PlotStuffParameters psp;
  
  // By default start saving the command file called "tbl.cmd"
  aString logFile="tbl.cmd";
  gi.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file =%s\n",(const char*)commandFileName);
    gi.readCommandFile(commandFileName);
  }

  FILE *checkFile = fopen("tbl.check","w" );   // Here is the check file for regression tests

  aString answer;
  GUIState dialog;

  dialog.setWindowTitle("Boundary Layer Profile Tester");
  dialog.setExitCommand("exit", "exit");

  aString cmds[] = {"plot Blasius profile",
                    "compute solution",
                    "contour",
                    "stream lines",
                    "exit",
		    ""};

  int numberOfPushButtons=3;  // number of entries in cmds
  int numRows=numberOfPushButtons; // (numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  const int numberOfTextStrings=15;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textLabels[nt] = "nu:"; 
  sPrintF(textStrings[nt],"%g",nu);  nt++; 

  textLabels[nt] = "U:"; 
  sPrintF(textStrings[nt],"%g",U);  nt++; 

  textLabels[nt] = "debug:"; 
  sPrintF(textStrings[nt],"%i",debug);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  gi.pushGUI(dialog);


  real xa=1., xb=5., ya=0., yb=1.;
  SquareMapping mapping(xa,xb,ya,yb);
  mapping.setGridDimensions(axis1,501);  
  mapping.setGridDimensions(axis2,101);  
  MappedGrid mg(mapping);              
  mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);                          

  OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),xLocal);

  Range all;
  realMappedGridFunction u;
  u.updateToMatchGrid(mg,all,all,all,2);          // define after declaration (like resize)
  u.setName("Solution");                          // give names to grid function ...
  u.setName("u",0);                               // ...and components
  u.setName("v",1);                               // ...and components

  for(;;)
  {
    
    gi.getAnswer(answer,"");  //  testProblem = (TestProblemEnum)ps.getMenuItem(menu,answer,"Choose a test");

    if( answer=="exit" )
    {
      break;
    }
    else if( dialog.getTextValue(answer,"nu:","%e",nu) ){} //
    else if( dialog.getTextValue(answer,"U:","%e",U) ){} //
    else if( dialog.getTextValue(answer,"debug:","%i",debug) ){} //
    else if( answer=="plot Blasius profile" )
    {
      // Plot the Blasius function f and its derivative
      // u/U = f'
      // eta = y*sqrt(U/(nu*x))

      profile.setParameters( nu,U );
      
      int n=101;
      RealArray eta(n), w(n,3);
      real etaStart=0., etaEnd=10.;
      for( int i=0; i<n; i++ )
      {
	eta(i)=etaStart + (etaEnd-etaStart)*i/(n-1);
        real f,fp;
	profile.evalBlasius( eta(i),w(i,0),w(i,1) );
        w(i,2)=eta(i)*w(i,1)-w(i,0);  // eta*f' - f 
      }
 
      aString cNames[3]={"f","fp","fv"};  // 
      psp.componentsToPlot.redim(3);
      psp.componentsToPlot(0)=0;
      psp.componentsToPlot(1)=1;
      psp.componentsToPlot(2)=2;
      // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true); 

      gi.erase();
      PlotIt::plot(gi,eta,w,sPrintF("Blasius profile, nu=%9.3e",nu),"eta",cNames,psp);


    }

    else if( answer=="compute solution" )
    {
      profile.setParameters( nu,U );
      

      Index I1,I2,I3;
      getIndex(mg.gridIndexRange(),I1,I2,I3);               // assign I1,I2,I3 from dimension

      int i1,i2,i3;
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	profile.eval( xLocal(i1,i2,i3,0),xLocal(i1,i2,i3,1), u(i1,i2,i3,0), u(i1,i2,i3,1) );
      }
   
    }
    
    else if( answer=="contour" )
    {
      gi.erase();
      PlotIt::contour(gi,u,psp);
    }
    else if( answer=="stream lines" )
    {
      gi.erase();
      PlotIt::streamLines(gi,u,psp);
    }

    else
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }

  } // for(;;)
  
  gi.popGUI(); // restore the previous GUI

  fclose(checkFile);
  Overture::finish(); 
  return 0;
}
