#include "Overture.h"
#include "Ogshow.h"  

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ----------------------------------------------------------------- \n");
  printf(" Demonstrate how to use the Ogshow class to save results in a file \n");
  printf(" to be later plotted with plotSuff                                 \n");
  printf(" ----------------------------------------------------------------- \n");

  aString nameOfOGFile, nameOfShowFile;
  cout << "example4>> Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;
  cout << "example4>> Enter the name of the (new) show file (blank for none):" << endl;
  cin >> nameOfShowFile;

  // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update();

  Ogshow show( nameOfShowFile );  // create a show file

  show.saveGeneralComment("Solution to the Navier-Stokes"); // save a general comment in the show file
  show.saveGeneralComment(" file written on April 1");      // save another general comment
    
  Range all;                                 // a null Range is used to dimension the grid function
  const int numberOfComponents=3;
  realCompositeGridFunction q(cg,all,all,all,numberOfComponents); // create a grid function with 3 components
  q=0.;

  realCompositeGridFunction u,v,machNumber;  // create grid functions for components
  u.link(q,Range(0,0));                      // link u to the first component of q
  v.link(q,Range(1,1));                      // link v to the second component of q
  machNumber.link(q,Range(2,2));             // ...
  q.setName("q");                            // assign name to grid function and components
  q.setName("u",0);                          // name of first component
  q.setName("v",1);                          // name of second component
  q.setName("Mach Number",2);                // name of third component

  char buffer[80];                           // buffer for sPrintF
  Index I1,I2,I3;
  int numberOfTimeSteps=5;
  for( int i=1; i<=numberOfTimeSteps; i++ )  // Now save the grid functions at different time steps
  {
    show.startFrame();                       // start a new frame
    real t=i*.1;
    show.saveComment(0,sPrintF(buffer,"Here is solution %i",i));   // comment 0 (shown on plot)
    show.saveComment(1,sPrintF(buffer,"  t=%e ",t));               // comment 1 (shown on plot)
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )      // loop over component grids
    {
      getIndex(cg[grid].indexRange(),I1,I2,I3);                       
      u[grid](I1,I2,I3)=sin(twoPi*(cg[grid].vertex()(I1,I2,I3,axis1)-t))   // assign u on each grid
                       *cos(twoPi*(cg[grid].vertex()(I1,I2,I3,axis2)+t));  
    }    
    v=u*2.;
    machNumber=u*u+v*v;
    show.saveSolution( q );              // save the current grid function
  }

  Overture::finish();          
  return 0;
    
}
