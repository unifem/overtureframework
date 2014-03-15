#include "Overture.h"
#include "GL_GraphicsInterface.h"
#include "DataPointMapping.h"
#include "MappingInformation.h"
#include "DataFormats.h"
#include "display.h"

// -- display results saved in plot3d format

int
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  int plotOption=TRUE;
  aString gridName,qName;
  if( argc > 1 )
  {
    gridName=argv[1];
    if( argc > 2 )
      qName=argv[2];
  }
  else
    cout << "Usage: `showPlot3d [grid.in][q.save]' \n"
            "          grid.in : grid file (plot3d format) \n" 
            "          q.save  : q file (plot 3d format) \n";

//   Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
    
  GL_GraphicsInterface gi(plotOption,"showPlot3d");          // create a GL_GraphicsInterface object
  GraphicsParameters gp;                       // create an object that is used to pass parameters
  MappingInformation mapInfo;
  mapInfo.graphXInterface=&gi;
  gi.appendToTheDefaultPrompt("plot3d>"); // set the default prompt


  // read in the grid(s) ** what about the mask ??

  const int ngd=10;
  intArray *mask = new intArray [ngd];
  
  DataFormats::readPlot3d(mapInfo,gridName,mask);
  
  Mapping & map = mapInfo.mappingList[0].getMapping();
  
  MappedGrid mg(map);                            
  mg.changeToAllCellCentered();                   // make a cell centered grid
  mg.update(MappedGrid::THEvertex | MappedGrid::THEmask);  
  
  Range I1(0,mask[0].getBound(0)-mask[0].getBase(0));  // shift to base 0
  Range I2(0,mask[0].getBound(1)-mask[0].getBase(1));
  Range I3(0,mask[0].getBound(2)-mask[0].getBase(2));

  if( mask[0].getLength(0)>0 )
  {
    printf("fill in the mask array...\n");
    printf(" min(mask)=%i max(mask)=%i \n",min(mask[0]),max(mask[0]));
    
    
    mg.mask()(I1,I2,I3)=mask[0];
    // mg.mask().display("mask");
    
  }
  

  GraphicsParameters psp;
  PlotIt::plot(gi, mg);

  


  // read in a "q" file with data

  realArray u0,par(6);
  DataFormats::readPlot3d(u0,par,qName);

  int nq=u0.getLength(3);
  Range N(0,nq-1);

  Range all;                                   // a null Range is used as a place-holder below for the coordinates


  realMappedGridFunction u(mg,all,all,all,nq); // create a grid function with 2 components: u(0:10,0:10,0:0,0:1)
  I1=Range(u0.getBase(0)-1,u0.getBound(0)-1); 
  I2=Range(u0.getBase(1)-1,u0.getBound(1)-1); 
  I3=Range(u0.getBase(2)-1,u0.getBound(2)-1); 
  
  printf("*** I1,I2,I3=[%i,%i][%i,%i][%i,%i]\n",I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
  

  u(I1,I2,I3,N)=u0(all,all,all,all);

//   int j1,j2,j3;
//   for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
//   for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
//     for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
//     {
//       j1=i1+1;
//       j2=i2+1;
//       j3=i3+1;
      
//       u(i1,i2,i3,0)=u0(j1,j2,j3,0);
//       printf(" i1,i2,i3=%i,%i,%i u=%4.1f  u0=%4.1f \n",i1,i2,i3,u(i1,i2,i3,0),u0(j1,j2,j3,0));
      
//     }
  
  u.setName("q");                              // give names to grid function ...
  if( nq>=4 )
  {
    RealArray rhoInverse;
    rhoInverse = 1./max(REAL_EPSILON,u(I1,I2,I3,0));
    
    u(I1,I2,I3,1)*=rhoInverse;
    u(I1,I2,I3,2)*=rhoInverse;

    u.setName("rho",0);                          // ...and components
    u.setName("u",1); 
    u.setName("v",2); 
    if( mg.numberOfDimensions()==2 )
    {
      u.setName("e",3);
      u.setName("p",4);
    }
    else
    {
      u.setName("w",3); 
      u.setName("e",4);
      u.setName("p",5);
      u(I1,I2,I3,3)*=rhoInverse;
    }
    
  }
//   display(u0(all,all,all,0),"u0(I1,I2,I3,0)","%5.1f ");
//   display(u(all,all,all,0),"u(I1,I2,I3,0)","%4.1f ");

  PlotIt::contour(gi,u,psp);

  if( mg.numberOfDimensions()==2 )
  {
    PlotIt::streamLines(gi,u,psp);
  }
  
  Overture::finish();   
  return 0;
}
