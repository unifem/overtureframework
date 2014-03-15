#include "Overture.h"  
#include "PlotStuff.h"
#include "display.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ------------------------------------------------------------------- \n");
  printf("        Query a grid and print out selected items                    \n");
  printf(" ------------------------------------------------------------------- \n");

  aString nameOfOGFile;
  if( argc>1 )
  {
    nameOfOGFile=argv[1];
  }
  else
  {
    cout << "gridQuery>> Enter the name of the (old) overlapping grid file:" << endl;
    cin >> nameOfOGFile;
  }
  
  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  bool openGraphicsWindow=TRUE;
  PlotStuff ps(openGraphicsWindow,"gridQuery");  // create a PlotStuff object
  GraphicsParameters psp;                      // This object is used to change plotting parameters
    
  aString answer;
  aString menu[] = { 
                    "!gridQuery",      
                    "plot",                  // Make some menu items
		    "index ranges",
		    "boundary conditions",
		    "interpolation information",
                    "verticies",
                    "plot xr",
                    "plot skewness",
                    "mask",
		    "erase",
		    "exit",
                    "" };                       // empty string denotes the end of the menu
  char buff[100];

  for(;;)
  {
    ps.getMenuItem(menu,answer);                // put up a menu and wait for a response
    if( answer=="plot" )
    {
      PlotIt::plot(ps,cg);                              // plot the composite grid
    }
    else if( answer=="index ranges" )
    {
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        MappedGrid & c = cg[grid];
        const IntegerArray & d  = c.dimension();
        const IntegerArray & gir= c.gridIndexRange();
        const IntegerArray & ir = c.indexRange();
        const IntegerArray & eir = c.extendedIndexRange();
        const IntegerArray & egir = extendedGridIndexRange(c); // *note*
        const IntegerArray & er = c.extendedRange();
	
	printf(" grid %i, name=%s, \n"
               "                   gridIndexRange(0:1,0:2) = [%2i,%i][%2i,%i][%2i,%i] (grid bounds)\n"
               "                       indexRange(0:1,0:2) = [%2i,%i][%2i,%i][%2i,%i] (1 less on periodic and CC)\n" 
               "                        dimension(0:1,0:2) = [%2i,%i][%2i,%i][%2i,%i] (array dimensions)\n" 
               "               extendedIndexRange(0:1,0:2) = [%2i,%i][%2i,%i][%2i,%i] (indexRange + interp)\n" 
               "           extendedGridIndexRange(0:1,0:2) = [%2i,%i][%2i,%i][%2i,%i] (gridIndexRange + interp)\n" 
               "                    extendedRange(0:1,0:2) = [%2i,%i][%2i,%i][%2i,%i] (includes interp on mixed BC)\n",
	       grid,(const char*)c.getName(),
               gir(0,0),gir(1,0),gir(0,1),gir(1,1),gir(0,2),gir(1,2),
               ir(0,0),ir(1,0),ir(0,1),ir(1,1),ir(0,2),ir(1,2),
               d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2),
               eir(0,0),eir(1,0),eir(0,1),eir(1,1),eir(0,2),eir(1,2),
               egir(0,0),egir(1,0),egir(0,1),egir(1,1),egir(0,2),egir(1,2),
               er(0,0),er(1,0),er(0,1),er(1,1),er(0,2),er(1,2) );
      }
    }
    else if( answer=="boundary conditions" )
    {
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        MappedGrid & c = cg[grid];
        const IntegerArray & bc  = c.boundaryCondition();
	printf(" grid %i, name=%s, \n"
               "                   boundaryCondition(0:1,0:2) = [%i,%i][%i,%i][%i,%i] (0=interp, <0=periodic)\n"
               "                   isPeriodic(0:2) = [%i,%i,%i] (2=function periodic, 1=f' periodic)\n",
	       grid,(const char*)c.getName(),
	       bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2),
	       c.isPeriodic(0),c.isPeriodic(1),c.isPeriodic(2) );
      }
    }
    else if( answer=="interpolation information" )
    {
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        MappedGrid & c = cg[grid];
	
        const IntegerArray & ni = cg.numberOfInterpolationPoints;
        const IntegerArray & ip = cg.interpolationPoint[grid];
        const IntegerArray & il = cg.interpoleeLocation[grid];
        const realArray & ci = cg.interpolationCoordinates[grid];
        display(ip,sPrintF(buff,"interpolationPoint on grid %i, name=%s",grid,(const char*)c.getName()),"%4i");
        display(il,sPrintF(buff,"interpoleeLocation on grid %i, name=%s",grid,(const char*)c.getName()),"%4i");
        display(ci,sPrintF(buff,"interpolationCoordinates grid %i, name=%s",grid,(const char*)c.getName()),"%4.2f ");
        display(cg.interpoleeGrid[grid],sPrintF(buff,"interpoleeGrid on grid %i, name=%s",
                  grid,(const char*)c.getName()),"%4i");
        display(cg.variableInterpolationWidth[grid],
            sPrintF(buff,"variableInterpolationWidth on grid %i, name=%s", grid,(const char*)c.getName()),"%2i");
      }
    }
    else if( answer=="mask" )
    {
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        MappedGrid & c = cg[grid];
	const intArray & mask = c.mask();
	displayMask(mask,sPrintF(buff,"mask on grid %i, name=%s", grid,(const char*)c.getName()));
        display(mask,sPrintF(buff,"mask on grid %i, name=%s", grid,(const char*)c.getName()));
      }
    }
    else if( answer=="verticies" )
    {
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        MappedGrid & c = cg[grid];
	display(c.vertex(),sPrintF(buff,"vertex on grid %i, name=%s", grid,(const char*)c.getName()),"%9.2e");
      }
    }
    else if( answer=="plot xr" )
    {
      const int numberOfDimensions=cg.numberOfDimensions();
      Range all;
      realCompositeGridFunction xr(cg,all,all,all,SQR(numberOfDimensions));
      xr.setName("xr",0);
      if( numberOfDimensions>1 )
      {
        xr.setName("yr",1);
        xr.setName("xs",2);
	xr.setName("ys",3);
      }
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
        xr[grid]=cg[grid].vertexDerivative();

      PlotIt::contour(ps,xr);
    }
    else if( answer=="plot skewness" )
    {
      const int numberOfDimensions=cg.numberOfDimensions();
      Range all;
      realCompositeGridFunction skew(cg,all,all,all);
      skew.setName("skewness");
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        realMappedGridFunction & xr = cg[grid].vertexDerivative();
        realMappedGridFunction & sr = skew[grid];

        //  skew = [ x.r dot x.s ] / ( ||x.r|| ||x.s|| ) 
        // **only for 2d**
        sr= (xr(all,all,all,0,0)*xr(all,all,all,0,1)+xr(all,all,all,1,0)*xr(all,all,all,1,1))/
              (SQRT(SQR(xr(all,all,all,0,0)) + SQR(xr(all,all,all,1,0)))*
               SQRT(SQR(xr(all,all,all,0,1)) + SQR(xr(all,all,all,1,1))));
      }
      PlotIt::contour(ps,skew);
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
      printf("Unknown response: [%s]\n",(const char*)answer);
    }
  }

  Overture::finish();          
  return 0;
}
