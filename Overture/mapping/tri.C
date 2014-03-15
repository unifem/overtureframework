#include "Triangle.h"

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);


  cout << "====== Test Triangle Class =====" << endl;


  realArray x1(3), x2(3), x3(3);
  x1=0.; 
  x2=0.; x2(0)=1.;
  x3=0.;           x3(1)=1.;

  Triangle t1(x1,x2,x3);
  t1.display("t1");
  


  x1=0.; x1(0)=-.5; x1(1)=.5; x1(2)=-.5;
  x2=0.; x2(0)=1.;  x2(1)=.5; x2(2)=-.5;
  x3=0.; x3(0)=-.5; x3(1)=.5; x3(2)=+.5;

  Triangle t2(x1,x2,x3);
  t2.display("t2");

  realArray xa(3),xb(3);

  if( t1.intersects(t2,xa,xb) )
    printf("triangles t1 and t2 intersect: xa=(%g,%g,%g), xb=(%g,%g,%g) \n",xa(0),xa(1),xa(2),xb(0),xb(1),xb(2));
  


  x1=0.; x1(0)=-.5; x1(1)=.5; x1(2)=-.5;
  x2=0.; x2(0)=1.;  x2(1)=.5; x2(2)=-.5;
  x3=0.; x3(0)=-.5; x3(1)=.5; x3(2)=+.25;

  Triangle t3(x1,x2,x3);
  t3.display("t3");

  if( t1.intersects(t3,xa,xb) )
    printf("triangles t1 and t3 intersect: xa=(%g,%g,%g), xb=(%g,%g,%g) \n",xa(0),xa(1),xa(2),xb(0),xb(1),xb(2));
  else
    printf("triangles t1 and t3 do not intersect\n");

  x1=0.; x1(0)=-.5; x1(1)=.5; x1(2)=-.5;
  x2=0.; x2(0)=1.;  x2(1)=.5; x2(2)=-.5;
  x3=0.; x3(0)=-.5; x3(1)=.5; x3(2)=+.2;

  Triangle t4(x1,x2,x3);
  t4.display("t4");

  if( t1.intersects(t4,xa,xb) )
    printf("triangles t1 and t4 intersect: xa=(%g,%g,%g), xb=(%g,%g,%g) \n",xa(0),xa(1),xa(2),xb(0),xb(1),xb(2));
  else
    printf("triangles t1 and t4 do not intersect\n");
    

  printf(" ***** now test the ray intersection functions ***** \n");
  

  realArray x0(3);

  x0(0)=(t2.x1[0]+t2.x2[0]+t2.x3[0])/3.;
//  x0(1)=min(t2.x1[1],t2.x2[1],t2.x3[1]) - .1;
  x0(1)=min(t2.x1[1],t2.x2[1],t2.x3[1]) - 1.;
  x0(2)=(t2.x1[2]+t2.x2[2]+t2.x3[2])/3.;
  if( t2.intersects(x0,xa) )
    printf("The vertical ray from x0=(%g,%g,%g) intersects triangle t2 at xa=(%g,%g,%g) \n",
           x0(0),x0(1),x0(2),xa(0),xa(1),xa(2));
  else
    printf("ERROR in intersecting ray with t2\n");
  
  if( true )
  {
    real b0[3]={1.,0.,0.}; //
    real b1[3]={0.,1.,0.}; //
    real b2[3]={0.,0.,1.}; //

    if( t2.intersects(x0,xa,b0,b1,b2) )
      printf("Using basis: The vertical ray from x0=(%g,%g,%g) intersects triangle t2 at xa=(%g,%g,%g) \n",
	     x0(0),x0(1),x0(2),xa(0),xa(1),xa(2));
    else
      printf("ERROR in intersecting ray with t2 using basis.\n");
    
  }
  
  x0(1)+=2.;
  if( !t2.intersects(x0,xa) )
    printf("The vertical ray from x0=(%g,%g,%g) DOES NOT intersect triangle t2 \n",
           x0(0),x0(1),x0(2));
  else
    printf("ERROR in intersecting ray with t2\n");
  

  // a ray through t1 is special since t1 is in the x-y plane
  x0(0)=(t1.x1[0]+t1.x2[0]+t1.x3[0])/3.;
  x0(1)=min(t1.x1[1],t1.x2[1],t1.x3[1]) - .1;
  x0(2)=(t1.x1[2]+t1.x2[2]+t1.x3[2])/3.;
  if( !t1.intersects(x0,xa) )
    printf("The vertical ray from x0=(%g,%g,%g) intersects triangle t1 at xa=(%g,%g,%g) \n",
           x0(0),x0(1),x0(2),xa(0),xa(1),xa(2));
  else
    printf("ERROR in intersecting ray with t1\n");
  
  x0(1)+=.2;
  if( !t1.intersects(x0,xa) )
    printf("The vertical ray from x0=(%g,%g,%g) DOES NOT intersect triangle t1 \n",
           x0(0),x0(1),x0(2));
  else
    printf("ERROR in intersecting ray with t1\n");
  

  if( true )
  {
    x0(0)=.25;  x0(1)=.25;  x0(2)=-1.;
     
    real b0[3]={0.,1.,0.}; //
    real b1[3]={0.,0.,1.}; //
    real b2[3]={1.,0.,0.}; //

    if( t1.intersects(x0,xa,b0,b1,b2) )
      printf("Using basis: The z+ ray from x0=(%g,%g,%g) intersects triangle t1 at xa=(%g,%g,%g) \n",
	     x0(0),x0(1),x0(2),xa(0),xa(1),xa(2));
    else
      printf("ERROR in intersecting the z+ ray with t1 using basis.\n");
    
    x0(0)=.25;  x0(1)=.25;  x0(2)=+1.;
  }
  if( true )
  {
    x0(0)=.25;  x0(1)=.25;  x0(2)=+1.;

    real b0[3]={0.,1.,0.}; //
    real b1[3]={0.,0.,-1.}; //
    real b2[3]={-1.,0.,0.}; //

    if( t1.intersects(x0,xa,b0,b1,b2) )
      printf("Using basis: The z- ray from x0=(%g,%g,%g) intersects triangle t1 at xa=(%g,%g,%g) \n",
	     x0(0),x0(1),x0(2),xa(0),xa(1),xa(2));
    else
      printf("ERROR in intersecting the z- ray with t1 using basis.\n");
    
  }
  if( true )
  {
  
    x1=0.; x1(0)=0.;  x1(1)=.0; x1(2)=0.; 
    x2=0.; x2(0)=0.;  x2(1)=1.; x2(2)=0.; 
    x3=0.; x3(0)=0.;  x3(1)=0.; x3(2)=1.; 

    Triangle t4(x1,x2,x3);

    real b0[3]={0.,0.,-1.}; //
    real b1[3]={-1.,0.,0}; //
    real b2[3]={ 0.,1.,0.}; //

    x0(0)=1.;  x0(1)=.25;  x0(2)=.25;

    if( t4.intersects(x0,xa,b0,b1,b2) )
      printf("Using basis: The x- ray from x0=(%g,%g,%g) intersects triangle t4 at xa=(%g,%g,%g) \n",
	     x0(0),x0(1),x0(2),xa(0),xa(1),xa(2));
    else
      printf("ERROR in intersecting the x- ray with t4 using basis.\n");
    
  }


  Overture::finish();

  return 0;
}
