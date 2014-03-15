// 
//   Compare grids constructed with the serial and parallel version of ogen for consistency.
//


#include "Overture.h"  
#include "ParallelUtility.h"
#include "display.h"

int 
main(int argc, char *argv[])
{
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np=Communication_Manager::numberOfProcessors();

  Overture::start(argc,argv);  // initialize Overture

  printF(" ---------------------------------------------------------------------------------------- \n");
  printF("  Compare grids constructed with the serial and parallel version of ogen for consistency. \n");
  printF(" ---------------------------------------------------------------------------------------- \n");

  aString gridSerial="cice2.order2Serial.hdf";
  aString gridParallel="cice2.order2.hdf";

  // aString gridSerial="square10Serial.hdf";
  // aString gridParallel="square10.hdf";
  
  // create and read in a CompositeGrid
  CompositeGrid cg1,cg2;
  getFromADataBase(cg1,gridSerial);
  getFromADataBase(cg2,gridParallel);

  cg1.update(MappedGrid::THEmask);
  cg2.update(MappedGrid::THEmask);
  
  assert( cg1.numberOfComponentGrids() == cg2.numberOfComponentGrids() );
  
  display(cg1.interpolationStartEndIndex(),"cg1.interpolationStartEndIndex()");
  display(cg2.interpolationStartEndIndex(),"cg2.interpolationStartEndIndex()");
  

  aString buff;
  Index I1,I2,I3;
  for( int grid=0; grid<cg1.numberOfComponentGrids(); grid++ )
  {
    printF("check grid=%i\n",grid);

    MappedGrid & c1 = cg1[grid];
    MappedGrid & c2 = cg2[grid];
//     const IntegerArray & d  = c.dimension();
//     const IntegerArray & gir= c.gridIndexRange();
//     const IntegerArray & ir = c.indexRange();
//     const IntegerArray & eir = c.extendedIndexRange();
//     const IntegerArray & egir = extendedGridIndexRange(c); // *note*
//     const IntegerArray & er = c.extendedRange();
 	
    if( max(abs(c1.dimension()-c2.dimension())) !=0 )
    {
      printf("myid=%i : ERROR: dimension does not agree!\n",myid);
    }
    if( max(abs(c1.sharedBoundaryFlag()-c2.sharedBoundaryFlag())) !=0 )
    {
      printf("myid=%i : ERROR: sharedBoundaryFlag does not agree!\n",myid);
    }

    printf(" myid=%i grid=%i numberOfInterpolationPoints=%i,%i\n",myid,grid,cg1.numberOfInterpolationPoints(grid),
                                                               cg2.numberOfInterpolationPoints(grid));
    
    const IntegerArray & ni1 = cg1.numberOfInterpolationPoints;
    const intArray & ip1 = cg1.interpolationPoint[grid];
    const intArray & il1 = cg1.interpoleeLocation[grid];
    const realArray & ci1 = cg1.interpolationCoordinates[grid];

    const IntegerArray & ni2 = cg2.numberOfInterpolationPoints;
    const intArray & ip2 = cg2.interpolationPoint[grid];
    const intArray & il2 = cg2.interpoleeLocation[grid];
    const realArray & ci2 = cg2.interpolationCoordinates[grid];


    printF(" cg->localInterpolationDataState = %i,%i\n",
	   (int)cg1->localInterpolationDataState,(int)cg2->localInterpolationDataState);
    
    if( grid<cg2.numberOfBaseGrids() || 
	cg2->localInterpolationDataState==CompositeGridData::noLocalInterpolationData )
    {
      printF("cg2: use the interpolation data in the parallel arrays\n");
	
      //interpolationPoint[grid].reference( cg.interpolationPoint[grid].getLocalArray());
      //interpoleeLocation[grid].reference( cg.interpoleeLocation[grid].getLocalArray());
      //interpoleeGrid[grid].reference( cg.interpoleeGrid[grid].getLocalArray());
      //variableInterpolationWidth[grid].reference( cg.variableInterpolationWidth[grid].getLocalArray());
      //interpolationCoordinates[grid].reference(cg.interpolationCoordinates[grid].getLocalArray());
    }
    else
    {
      printF("use the interpolation data in the serial arrays (for now these are refinement grids\n");
	
      //interpolationPoint[grid].reference( cg->interpolationPointLocal[grid]);
      //interpoleeLocation[grid].reference( cg->interpoleeLocationLocal[grid]);
      //interpoleeGrid[grid].reference( cg->interpoleeGridLocal[grid]);
      //variableInterpolationWidth[grid].reference( cg->variableInterpolationWidthLocal[grid]);
      //interpolationCoordinates[grid].reference(cg->interpolationCoordinatesLocal[grid]);
    }

    display(ip1,sPrintF(buff,"ip1 on grid %i, name=%s",grid,(const char*)c1.getName()),"%4i");
    display(ip2,sPrintF(buff,"ip2 on grid %i, name=%s",grid,(const char*)c2.getName()),"%4i");

    display(il1,sPrintF(buff,"il1 on grid %i, name=%s",grid,(const char*)c1.getName()),"%4i");
    display(il2,sPrintF(buff,"il2 on grid %i, name=%s",grid,(const char*)c2.getName()),"%4i");

    display(ci1,sPrintF(buff,"ci1 on grid %i, name=%s",grid,(const char*)c1.getName()),"%5.2f");
    display(ci2,sPrintF(buff,"ci2 on grid %i, name=%s",grid,(const char*)c2.getName()),"%5.2f");


    int diff;
    intSerialArray mask1Local; getLocalArrayWithGhostBoundaries(c1.mask(),mask1Local);
    intSerialArray mask2Local; getLocalArrayWithGhostBoundaries(c2.mask(),mask2Local);

    diff = max(abs(mask1Local-mask2Local));
    if( diff!=0 )
    {
      printf("myid=%i : ERROR: mask's do not agree!\n",myid);
      if( false )
      {
	displayMask(mask1Local,"mask1Local");
	displayMask(mask2Local,"mask2Local");
	::display(mask1Local-mask2Local,"mask1Local-mask2Local");
	::display(mask1Local,"mask1Local");
	::display(mask2Local,"mask2Local");
      }
      
    }
    

  }
  

  Overture::finish();          
  return 0;

}


// * 
// * 
// *   bool openGraphicsWindow=TRUE;
// *   PlotStuff ps(openGraphicsWindow,"gridQuery");  // create a PlotStuff object
// *   GraphicsParameters psp;                      // This object is used to change plotting parameters
// *     
// *   aString answer;
// *   aString menu[] = { 
// *                     "!gridQuery",      
// *                     "plot",                  // Make some menu items
// * 		    "index ranges",
// * 		    "boundary conditions",
// * 		    "interpolation information",
// *                     "verticies",
// *                     "plot xr",
// *                     "plot skewness",
// *                     "mask",
// * 		    "erase",
// * 		    "exit",
// *                     "" };                       // empty string denotes the end of the menu
// *   char buff[100];
// * 
// *   for(;;)
// *   {
// *     ps.getMenuItem(menu,answer);                // put up a menu and wait for a response
// *     if( answer=="plot" )
// *     {
// *       PlotIt::plot(ps,cg);                              // plot the composite grid
// *     }
// *     else if( answer=="index ranges" )
// *     {
// *       for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
// *       {
// *         MappedGrid & c = cg[grid];
// *         const IntegerArray & d  = c.dimension();
// *         const IntegerArray & gir= c.gridIndexRange();
// *         const IntegerArray & ir = c.indexRange();
// *         const IntegerArray & eir = c.extendedIndexRange();
// *         const IntegerArray & egir = extendedGridIndexRange(c); // *note*
// *         const IntegerArray & er = c.extendedRange();
// * 	
// * 	printf(" grid %i, name=%s, \n"
// *                "                   gridIndexRange(0:1,0:2) = [%2i,%i][%2i,%i][%2i,%i] (grid bounds)\n"
// *                "                       indexRange(0:1,0:2) = [%2i,%i][%2i,%i][%2i,%i] (1 less on periodic and CC)\n" 
// *                "                        dimension(0:1,0:2) = [%2i,%i][%2i,%i][%2i,%i] (array dimensions)\n" 
// *                "               extendedIndexRange(0:1,0:2) = [%2i,%i][%2i,%i][%2i,%i] (indexRange + interp)\n" 
// *                "           extendedGridIndexRange(0:1,0:2) = [%2i,%i][%2i,%i][%2i,%i] (gridIndexRange + interp)\n" 
// *                "                    extendedRange(0:1,0:2) = [%2i,%i][%2i,%i][%2i,%i] (includes interp on mixed BC)\n",
// * 	       grid,(const char*)c.getName(),
// *                gir(0,0),gir(1,0),gir(0,1),gir(1,1),gir(0,2),gir(1,2),
// *                ir(0,0),ir(1,0),ir(0,1),ir(1,1),ir(0,2),ir(1,2),
// *                d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2),
// *                eir(0,0),eir(1,0),eir(0,1),eir(1,1),eir(0,2),eir(1,2),
// *                egir(0,0),egir(1,0),egir(0,1),egir(1,1),egir(0,2),egir(1,2),
// *                er(0,0),er(1,0),er(0,1),er(1,1),er(0,2),er(1,2) );
// *       }
// *     }
// *     else if( answer=="boundary conditions" )
// *     {
// *       for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
// *       {
// *         MappedGrid & c = cg[grid];
// *         const IntegerArray & bc  = c.boundaryCondition();
// * 	printf(" grid %i, name=%s, \n"
// *                "                   boundaryCondition(0:1,0:2) = [%i,%i][%i,%i][%i,%i] (0=interp, <0=periodic)\n"
// *                "                   isPeriodic(0:2) = [%i,%i,%i] (2=function periodic, 1=f' periodic)\n",
// * 	       grid,(const char*)c.getName(),
// * 	       bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2),
// * 	       c.isPeriodic(0),c.isPeriodic(1),c.isPeriodic(2) );
// *       }
// *     }
// *     else if( answer=="interpolation information" )
// *     {
// *       for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
// *       {
// *         MappedGrid & c = cg[grid];
// * 	
// *         const IntegerArray & ni = cg.numberOfInterpolationPoints;
// *         const IntegerArray & ip = cg.interpolationPoint[grid];
// *         const IntegerArray & il = cg.interpoleeLocation[grid];
// *         const realArray & ci = cg.interpolationCoordinates[grid];
// *         display(ip,sPrintF(buff,"interpolationPoint on grid %i, name=%s",grid,(const char*)c.getName()),"%4i");
// *         display(il,sPrintF(buff,"interpoleeLocation on grid %i, name=%s",grid,(const char*)c.getName()),"%4i");
// *         display(ci,sPrintF(buff,"interpolationCoordinates grid %i, name=%s",grid,(const char*)c.getName()),"%4.2f ");
// *         display(cg.interpoleeGrid[grid],sPrintF(buff,"interpoleeGrid on grid %i, name=%s",
// *                   grid,(const char*)c.getName()),"%4i");
// *         display(cg.variableInterpolationWidth[grid],
// *             sPrintF(buff,"variableInterpolationWidth on grid %i, name=%s", grid,(const char*)c.getName()),"%2i");
// *       }
// *     }
// *     else if( answer=="mask" )
// *     {
// *       for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
// *       {
// *         MappedGrid & c = cg[grid];
// * 	const intArray & mask = c.mask();
// * 	displayMask(mask,sPrintF(buff,"mask on grid %i, name=%s", grid,(const char*)c.getName()));
// *         display(mask,sPrintF(buff,"mask on grid %i, name=%s", grid,(const char*)c.getName()));
// *       }
// *     }
// *     else if( answer=="verticies" )
// *     {
// *       for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
// *       {
// *         MappedGrid & c = cg[grid];
// * 	display(c.vertex(),sPrintF(buff,"vertex on grid %i, name=%s", grid,(const char*)c.getName()),"%9.2e");
// *       }
// *     }
// *     else if( answer=="plot xr" )
// *     {
// *       const int numberOfDimensions=cg.numberOfDimensions();
// *       Range all;
// *       realCompositeGridFunction xr(cg,all,all,all,SQR(numberOfDimensions));
// *       xr.setName("xr",0);
// *       if( numberOfDimensions>1 )
// *       {
// *         xr.setName("yr",1);
// *         xr.setName("xs",2);
// * 	xr.setName("ys",3);
// *       }
// *       for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
// *         xr[grid]=cg[grid].vertexDerivative();
// * 
// *       PlotIt::contour(ps,xr);
// *     }
// *     else if( answer=="plot skewness" )
// *     {
// *       const int numberOfDimensions=cg.numberOfDimensions();
// *       Range all;
// *       realCompositeGridFunction skew(cg,all,all,all);
// *       skew.setName("skewness");
// *       for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
// *       {
// *         realMappedGridFunction & xr = cg[grid].vertexDerivative();
// *         realMappedGridFunction & sr = skew[grid];
// * 
// *         //  skew = [ x.r dot x.s ] / ( ||x.r|| ||x.s|| ) 
// *         // **only for 2d**
// *         sr= (xr(all,all,all,0,0)*xr(all,all,all,0,1)+xr(all,all,all,1,0)*xr(all,all,all,1,1))/
// *               (SQRT(SQR(xr(all,all,all,0,0)) + SQR(xr(all,all,all,1,0)))*
// *                SQRT(SQR(xr(all,all,all,0,1)) + SQR(xr(all,all,all,1,1))));
// *       }
// *       PlotIt::contour(ps,skew);
// *     }
// *     else if( answer=="erase" )
// *     {
// *       ps.erase();
// *     }
// *     else if( answer=="exit" )
// *     {
// *       break;
// *     }
// *     else
// *     {
// *       printf("Unknown response: [%s]\n",(const char*)answer);
// *     }
// *   }
// * 
// *   Overture::finish();          
// *   return 0;
// * }
