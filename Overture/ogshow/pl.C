//
// mpirun -np 2 ./pl -noplot -g=square10 -cmd=og.cmd
// 
// srun -N1 -n2 -ppdebug pl -g=sibe2.order2.hdf 
//

#include "Overture.h"
#include "GL_GraphicsInterface.h"
#include "display.h"
#include "FileOutput.h"
#include "ColourBar.h"
#include "ParallelUtility.h"

#include "SquareMapping.h"

GL_GraphicsInterface *psPointer; // create a (pointer to a) GL_GraphicsInterface object

void selectObject(const real & x=-1., const real & y=-1.);
void getCursor( real & x, real & y );

#define FOR_3(i1,i2,i3,I1,I2,I3) \
  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  \
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  \
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  \

//===============================================================================================
//     Example routine demonstrating the use of the GL_GraphicsInterface Class
//
//  This example shows the use of:
//    o prompting for a menu
//    o plotting grids functions and grids
//    o reading and saving command files
//===============================================================================================


int 
main(int argc, char *argv[])
{
  const int myid=max(0,Communication_Manager::My_Process_Number);
  
  PlotIt::parallelPlottingOption=1;  // turn on distributed plotting! *******************

  const bool showComputedGeometry=false;

  Overture::start(argc,argv);  // initialize Overture

  Overture::turnOnMemoryChecking(true);

  printF("Usage:pl [-noplot] -g=gridName -cmd=file.cmd\n");
  aString nameOfOGFile="";
  aString commandFileName="";
  int plotOption=1;
  if( argc>1 )
  {
    aString line;
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      //  int len=strlen(argv[i]);
      //        printF(" argv[%i]=%s len=%i\n",i,argv[i],len);
      
      line=argv[i];
      if( line=="plot" || line=="-plot" )
        plotOption=true;
      else if( line=="noplot" || line=="-noplot" )
        plotOption=false;
      else if( len=line.matches("-g=") )
      {
        nameOfOGFile=line(len,line.length()-1);
      }
      else if( len=line.matches("-cmd=") )
      {
        commandFileName=line(len,line.length()-1);
      }
    }
  }
  // else
  //  ps.inputFileName(nameOfOGFile, ">> Enter the name of the (old) composite grid file:", ".hdf");

  psPointer = new GL_GraphicsInterface(plotOption,"plot");
  GL_GraphicsInterface & ps = *psPointer;

  #ifdef USE_PPP
    // On Parallel machines always add at least this many ghost lines on local arrays
    const int numGhost=1;
    MappedGrid::setMinimumNumberOfDistributedGhostLines(numGhost);
  #endif

  CompositeGrid cg;

  // getFromADataBase(cg,nameOfOGFile);
  bool loadBalance=true;
  getFromADataBase(cg,nameOfOGFile,loadBalance);

  cg.update(MappedGrid::THEmask); 

#ifdef USE_PPP
  intArray & m = cg[0].mask();
  // printF("pl: mask.getGhostBoundaryWidth =[%i,%i]\n",m.getGhostBoundaryWidth(0),m.getGhostBoundaryWidth(1));  
#endif

  aString logFile="pl.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);
  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);

/* ---
  for( int g=0; g<cg.numberOfComponentGrids(); g++ )
  {
    cg[g].mask().display("Here is cg.mask()");
    cout << "isAllVertexCentered = " << cg[g].isAllVertexCentered() << endl;
    cout << "isAllCellCentered = " << cg[g].isAllCellCentered() << endl;
    cg[g].isCellCentered().display("cg[g].isCellCentered()");
  }
---- */

  // set up a function for contour plotting:
  Range all;
  realCompositeGridFunction u(cg,all,all,all,3), v(cg,2,all,all,all), u2, 
                            ucc(cg,all,all,all,faceRange),ucc2(cg,all,all,all,Range(0,1),faceRange);
  // u2.link(u,1);
  v=1.;
  ucc=5.;
  ucc2=3.;

  u.setName("Velocity Stuff");
  u.setName("u",0);
  u.setName("v",1);
  u.setName("w",2);
  Index I1,I2,I3;                                              // A++ Index object
  int i1,i2,i3;
  cg.update(MappedGrid::THEcenter);
  
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )  // loop over component grids
  {
    #ifdef USE_PPP
     realSerialArray coord; getLocalArrayWithGhostBoundaries(cg[grid].center(),coord);
     realSerialArray ug;    getLocalArrayWithGhostBoundaries(u[grid],ug);
     realSerialArray vg;    getLocalArrayWithGhostBoundaries(v[grid],vg);
    #else
      realSerialArray & coord = cg[grid].center(); 
      realSerialArray & ug = u[grid]; 
      realSerialArray & vg = v[grid]; 
    #endif

    getIndex(cg[grid].dimension(),I1,I2,I3);                   // assign I1,I2,I3 from indexRange
    bool ok = ParallelUtility::getLocalArrayBounds(u[grid],ug,I1,I2,I3,1);
    if( !ok ) continue;
    
    if( cg.numberOfDimensions()==1 )
    {
      ug(I1,I2,I3,0)=sin(Pi*coord(I1,I2,I3,axis1));
      ug(I1,I2,I3,1)=cos(Pi*coord(I1,I2,I3,axis1));

      ug(I1,I2,I3,2)=1.;

      FOR_3(i1,i2,i3,I1,I2,I3)
      {
	vg(0,i1,i2,i3)=sin(.5*Pi*coord(i1,i2,i3,axis1));
	vg(1,i1,i2,i3)=grid;
      }
    }
    else if( cg.numberOfDimensions()==2 )
    {
      // ug(I1,I2,I3,0)=1.+.00001*(
      //      sin(Pi*coord(I1,I2,I3,axis1))   // assign all interior points on this
      //     *cos(Pi*coord(I1,I2,I3,axis2))   // component grid

      ug(I1,I2,I3,0)=
	sin(Pi*coord(I1,I2,I3,axis1))   // assign all interior points on this
	*cos(Pi*coord(I1,I2,I3,axis2));   // component grid
      ug(I1,I2,I3,1)=cos(Pi*coord(I1,I2,I3,axis1))
	*sin(Pi*coord(I1,I2,I3,axis2));

//      ug(I1,I2,I3,2)=1.;

      ug(I1,I2,I3,2)=tanh(20.*coord(I1,I2,I3,axis1));  // ****
      

      FOR_3(i1,i2,i3,I1,I2,I3)
      {
	vg(0,i1,i2,i3)=sin(.5*Pi*coord(i1,i2,i3,axis1))
	  *cos(.5*Pi*coord(i1,i2,i3,axis2));
	vg(1,i1,i2,i3)=grid;
      }
    }
    else
    {
      ug(I1,I2,I3,0)=(2.+
			   sin(.5*Pi*coord(I1,I2,I3,axis1))*  
			   cos(.5*Pi*coord(I1,I2,I3,axis2))*
			   cos(.5*Pi*coord(I1,I2,I3,axis3)));
	ug(I1,I2,I3,1)=(cos(.5*Pi*coord(I1,I2,I3,axis1))*
			     sin(.5*Pi*coord(I1,I2,I3,axis2))*
			     cos(.5*Pi*coord(I1,I2,I3,axis3)));
      ug(I1,I2,I3,2)=(cos(.5*Pi*coord(I1,I2,I3,axis1))*
			   cos(.5*Pi*coord(I1,I2,I3,axis2))*
			   sin(.5*Pi*coord(I1,I2,I3,axis3)));
      FOR_3(i1,i2,i3,I1,I2,I3)
      {
	vg(0,i1,i2,i3)=sin(.5*Pi*coord(i1,i2,i3,axis1))
	  *cos(.5*Pi*coord(i1,i2,i3,axis2))
	  *cos(.5*Pi*coord(i1,i2,i3,axis3));
	vg(1,i1,i2,i3)=grid;
      }
    }      
 
//    ug(I1,I2,I3,0)=1.;
//    ug(I1,I2,I3,1)=2.;

//    where( cg[grid].mask()(I1,I2,I3)==0 ) 
//      ug(I1,I2,I3,0)=1000.;
    
  } // end for grid 
  
  cg.destroy(MappedGrid::THEcenter | MappedGrid::THEvertex );
    
  if( showComputedGeometry )
  {
    printF(" In main, after assign u\n");
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }
  GraphicsParameters psp;               // create an object that is used to pass parameters
    
//  ps.updateColourBar(psp);
  

  int std_win = 0; // the default window has number 0

//  ps.setCurrentWindow(std_win);    // reset the plot focus
      
  char buff[160];  // buffer for sprintf
  aString answer,answer2;
  aString menu[] = { "!PL test program",
		    "contour",
		    "contour v",
		    "contour cell centred",
                    "contour and wait",
		    "stream lines",
		    "plot a grid",
                    "plot a mapping",
                    "plot a MappedGrid",
                    "contour a MappedGridFunction",
                    "streamLines of a MappedGridFunction",
                    "select object",
                    "pick points",
                    "select points",
                    "line width scale factor",
                    "plot points",
                    "plot points with colour",
                    "plot spheres with colour",
		    "plot grid quality",
                    "file output",
		    "file name test",
		    "erase",
		    "exit",
                    "" };
  aString menu2[]= { "params","plot","exit","" };

  Overture::checkMemoryUsage("pl: before plotting");

  for(;;)
  {
    // printf(" myid=%i ps.readingFromCommandFile()=%i\n",myid,(int)ps.readingFromCommandFile());
    // fflush(0);

    ps.getMenuItem(menu,answer,"Ready to serve you, master!");
    // cout << answer << endl;
  

    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT, FALSE);

    if( answer=="line width scale factor" )
    {
      real lineWidth;
      ps.inputString(answer2,"Enter line width (1=normal, 3=wide)");
      if( answer2!="" )
      {
        sScanF(answer2,"%e",&lineWidth);
        ps.setLineWidthScaleFactor(lineWidth);
      }
    }
    else if( answer=="contour" )
    {
      ps.outputString("Plotting contour");
      psp.set(GI_TOP_LABEL,"My Contour Plot");  // set title
      psp.set(GI_TOP_LABEL_SUB_1,"a subtitle"); 
      PlotIt::contour(ps,u, psp);  // contour/surface plots
      ps.outputString("Done");

      // Overture::checkMemoryUsage("pl: after contour");
      Overture::printMemoryUsage("pl: after contour");
    }
    else if( answer=="contour v" )
    {
      PlotIt::contour(ps,v, Overture::defaultGraphicsParameters());  // contour/surface plots
    }
    else if( answer=="contour and wait" )
    {
      psp.set(GI_TOP_LABEL,"My Contour Plot");  // set title
      PlotIt::contour(ps,u, psp);  // contour/surface plots
      // wait here
      cout << "enter answer\n";
      cin >> answer;
    }
    else if( answer=="contour cell centred")
    {
      psp.set(GI_TOP_LABEL,"Contour a Cell-Centred Grid Function");  // set title
      PlotIt::contour(ps,ucc, Overture::defaultGraphicsParameters());  // contour/surface plots
      PlotIt::contour(ps,ucc2, Overture::defaultGraphicsParameters());  // contour/surface plots
    }
    else if( answer=="plot a grid" )
    {
      psp.set(GI_TOP_LABEL,"My Grid");  // set title
      psp.set(GI_LABEL_GRIDS_AND_BOUNDARIES, TRUE);
      PlotIt::plot(ps,cg, psp);   // plot the composite grid

      Overture::printMemoryUsage("pl: after plot a grid");
    }
    else if( answer=="stream lines" )
    {
      psp.set(GI_TOP_LABEL,"Streamlines");  // set title
      PlotIt::streamLines(ps,u,psp);  // streamlines
    }
    else if( answer=="plot a mapping" )
    {
      ps.inputString(answer2,sPrintF(buff,"Enter the Mapping number to plot, from 0 to %i",
				     cg.numberOfComponentGrids()-1));
      int mapToPlot = 0;
      if( answer2!="" )
	sScanF(answer2,"%i",&mapToPlot);

      psp.set(GI_TOP_LABEL,"My little mapping");  // set title
      PlotIt::plot(ps,cg[mapToPlot].mapping().getMapping(), psp);  
    }
    else if( answer=="plot a MappedGrid" )
    {
      psp.set(GI_TOP_LABEL,"A mapped grid");  // set title
      psp.set(GI_LABEL_GRIDS_AND_BOUNDARIES, TRUE);
      PlotIt::plot(ps,cg[0], psp);  
    }
    else if( answer=="contour a MappedGridFunction" )
    {
      psp.set(GI_TOP_LABEL,"A mapped grid function");  // set title
      if( true )
      {
	PlotIt::contour(ps,u[0], psp);  
      }
      else
      { // Jeff Bank's bug
	Mapping *phase_mapping;

	phase_mapping = new SquareMapping;
//  phase_mapping->setGridDimensions(axis1,NX1);    // axis1==0, set no. of grid points
//  phase_mapping->setGridDimensions(axis2,NX2);    // axis2==1, set no. of grid points
	MappedGrid phase_mg(*phase_mapping);           
	// phase_mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
	phase_mg.update(MappedGrid::THEvertex );

	//realMappedGridFunction phase_function;
	//phase_function.updateToMatchGrid(phase_mg,all,all,all,1);
	realMappedGridFunction phase_function(phase_mg,all,all,all,1);

	phase_function.setName("phase functions");
	phase_function.setName("f",0);

        phase_function=0.;
	
        PlotIt::contour(ps,phase_function, psp);  
      }
      
    }
    else if( answer=="streamLines of a MappedGridFunction" )
    {
      psp.set(GI_TOP_LABEL,"My stream lines");  // set title
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      PlotIt::streamLines(ps,u[0],psp);  
    }
    else if( answer=="erase" )
    {
      ps.erase();
    }
//     else if( answer=="select object" )
//     {
//       IntegerArray selection;
//       for(;;) 
//       {
// 	const aString menu[]=
// 	{
// 	  "!PL select object test",
// 	  "done",
//           ""
// 	};
//         int numberSelected=ps.getMenuItem(menu,answer,"pk: select objects with mouse",selection);
//         cout << "pk: answer= " << answer << endl;
//         if( answer=="done" )
// 	{
// 	  break;
// 	}
// 	else
// 	{
// 	  printf("pk: numberSelected=%i \n",numberSelected);
// 	}
//       }
//       //      selectObject();
//     }
//     else if( answer=="pick points" )
//     {
//       RealArray x(100,2);
//       ps.erase();
//       RealArray xBound(2,3);
//       xBound=0.;
//       xBound(1,nullRange)=1.;
//       ps.setGlobalBound(xBound);
//       ps.sgPlotBackGroundGrid()=2;
//       ps.setAxesDimension(2);
      
//       ps.pickPoints(x);
//     }
//     else if( answer=="select points" )
//     {
//       for(;;) 
//       {
// 	const aString menu[]=
// 	{
// 	  "!PL select point or region",
// 	  "done",
//           ""
// 	};
//         realArray pickRegion(2,2);
//         int numberSelected=ps.getMenuItem(menu,answer,"select a point or region",pickRegion);
//         if( answer=="done" )
// 	{
// 	  break;
// 	}
// 	else
// 	{
// 	  printf("pk: region selected = (%e,%e)X(%e,%e) \n",
//              pickRegion(0,0),pickRegion(1,0),pickRegion(0,1),pickRegion(1,1));
// 	}
//       }
//     }
    else if( answer=="plot points" || answer=="plot points with colour" || answer=="plot spheres with colour" )
    {
      const bool plotSpheres = answer=="plot spheres with colour";
      
      const int n=plotSpheres ? 41 : 51;
      const int m = plotSpheres ? 2 : 1;
      realArray points(n,3), value(n,m);
      for( int i=0; i<n; i++ )
      {
        real radius=i/(n+1.);
        real theta=twoPi*i/(n+1.);
        points(i,axis1)=cos(theta)*radius;
        points(i,axis2)=sin(theta)*radius;
	points(i,axis3)=cos(2.*theta)*radius;
        value(i,0)= SQR(points(i,axis1))+SQR(points(i,axis2))+SQR(points(i,axis3));

        if( plotSpheres ) value(i,1)=.05; // radius for the sphere
      }

      // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT, true);

      psp.set(GI_POINT_SIZE,(real) 6.);  // size in pixels
      if( answer=="plot points" )
        ps.plotPoints(points,psp);
      else
        ps.plotPoints(points,value,psp); // colour point i by value(i)
    }
    else if( answer=="plot grid quality" )
    {
      PlotIt::plotGridQuality(ps,cg[0],psp);
    }
    else if( answer=="file output" )
    {
      // ps.fileOutput(u);
      FileOutput fileOutput;
      fileOutput.update(u,ps);
    }
    else if( answer=="file name test" )
    {
// AP: testing
      aString fn;
      ps.inputFileName(fn,"Enter filename>");
      cout << "FileName=" << fn << endl;
    }
    else if( answer=="exit" )
    {
      break;
    }
    else
    {
      cout << "Unknown answer=[" << answer << "]\n";
    }
    
  }

  if( showComputedGeometry )
  {
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }

  // Overture::checkMemoryUsage("pl: END");
  Overture::printMemoryUsage("pl: END");

  Overture::finish();          
  return 0;
}
