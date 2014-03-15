// ****************************************
// ***** test of MappedGrid's  ************
// ****************************************

// 100515: Optimize evaluated of x and rx in MappedGridGeometry2
//
//  Annulus : 2001x501
//    (1) MappedGridGeometry2.C compiled -g (or compiled -O -- doesn't seem to make much difference: )
//        Generate x : cpu=2.80e-01
//        Generate x and rx : cpu=5.52e-01  --> .6 with -g
//   
//    (2) Don't allocate space for xr if not needed
//        Generate x : cpu=9.74e-02   (cpu varies, .14)
//    (3) Optimized eval of rx:
//        Generate x and rx : cpu=3.24e-01  (cpu varies, may be .4 or so)

// srun -N1 -n2 -ppdebug tg

#include "Overture.h"  
#include "PlotStuff.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "BoxMapping.h"
#include "CylinderMapping.h"
#include "MatrixTransform.h"
#include "DataPointMapping.h"
#include "display.h"

int 
main(int argc, char** argv)
{
  Overture::start(argc,argv);  // initialize Overture
  const int myid=max(0,Communication_Manager::My_Process_Number);

  if( true )
  {
    // -- test: MappedGridGeometry seems to allocate a temp array xr even if not needed (100517)

    AnnulusMapping annulus;
    annulus.setGridDimensions(0,2001);
    annulus.setGridDimensions(1,501);
    
    MappedGrid mg(annulus);     


    mg.update(MappedGrid::THEvertex);
    mg.update(MappedGrid::THEinverseVertexDerivative);

    // CPU timing:

    mg.destroy(MappedGrid::EVERYTHING);
    printF("\n--tg:  mg.update(MappedGrid::THEvertex );\n");
    real time=getCPU();
    mg.update(MappedGrid::THEvertex );
    time=getCPU()-time;
    printF("Generate x : cpu=%8.2e\n",time);

    mg.destroy(MappedGrid::EVERYTHING);
    printF("\n--tg:  mg.update(MappedGrid::THEvertex | MappedGrid::THEinverseVertexDerivative);\n");
    time=getCPU();
    mg.update(MappedGrid::THEvertex | MappedGrid::THEinverseVertexDerivative);
    time=getCPU()-time;
    printF("Generate x and rx : cpu=%8.2e\n",time);

    return 0;
  }
  


  if( true )
  {
//     SquareMapping square(0.,1.,0.,1.);
//     MappedGrid mg(square); 
    
     AnnulusMapping annulus;
     annulus.setGridDimensions(0,21);
     annulus.setGridDimensions(1,11);
     MappedGrid mg(annulus); 

//     real startAngle=0., endAngle=.5; // 1.  // .25;
//     CylinderMapping cyl(startAngle,endAngle);
//     cyl.setGridDimensions(0,21);
//     cyl.setGridDimensions(1,5);    // axial
//     cyl.setGridDimensions(2,5);   // radial
//     MappedGrid mg(cyl);

    mg.update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEboundingBox );    

    const RealArray & boundingBox = mg.boundingBox();
    const RealArray & localBoundingBox = mg.localBoundingBox();
    

    printf("tg: myid=%i : boundingBox=[%g,%g][%g,%g][%g,%g], localBoundingBox=[%g,%g][%g,%g][%g,%g]\n",myid,
           boundingBox(0,0),boundingBox(1,0),boundingBox(0,1),boundingBox(1,1),boundingBox(0,2),boundingBox(1,2),
	   localBoundingBox(0,0),localBoundingBox(1,0),localBoundingBox(0,1),localBoundingBox(1,1),
	   localBoundingBox(0,2),localBoundingBox(1,2));

    return 0;
  
  }
  

  if( false )
  {
    real startAngle=0., endAngle=.5; // 1.  // .25;
    CylinderMapping cyl(startAngle,endAngle);
    cyl.setGridDimensions(0,21);
    cyl.setGridDimensions(1,5);    // axial
    cyl.setGridDimensions(2,5);   // radial

//     cyl.setGridDimensions(0,5);
//     cyl.setGridDimensions(1,2);    // axial
//     cyl.setGridDimensions(2,2);   // radial

//    AnnulusMapping cyl;

//     BoxMapping box;
//     MatrixTransform cyl(box);

//    Mapping & map = cyl;

    DataPointMapping dpm;
    AnnulusMapping annulus;
    dpm.setMapping(annulus);

    Mapping & map = dpm;

    MappedGrid mg(map); 
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      for( int side=0; side<=1; side++ )
        mg.setNumberOfGhostPoints(side,axis,2);

    mg.update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter );    
    realArray & x = mg.vertex();

    // check the new parallel bounding box computation

    const RealArray & boundingBox = mg.mapping().getMapping().getBoundingBox();
    printF("\n tg: boundingBox = [%g,%g][%g,%g][%g,%g]\n\n",
	   boundingBox(0,0),boundingBox(1,0),
	   boundingBox(0,1),boundingBox(1,1),
	   boundingBox(0,2),boundingBox(1,2));
    
//     int axis=0;
//     int side=1;
//     Index I1,I2,I3;
//     Range Rx=mg.numberOfDimensions();
//     getGhostIndex(mg.gridIndexRange(),side,axis,I1,I2,I3,1);   // line i1=N+1
//     display(x(I1,I2,I3,Rx),"mg.vertex - ghost line N+1 in periodic dir","%5.2f ");
//     side=0;
//     getGhostIndex(mg.gridIndexRange(),side,axis,I1,I2,I3,-1);  // first line inside (i1=1)
//     display(x(I1,I2,I3,Rx),"mg.vertex - periodic image","%5.2f ");
    
//     side=0;
//     getGhostIndex(mg.gridIndexRange(),side,axis,I1,I2,I3,1);   // line i1=-1
//     display(x(I1,I2,I3,Rx),"mg.vertex - ghost line -1 ","%5.2f ");
//     side=1;
//     getGhostIndex(mg.gridIndexRange(),side,axis,I1,I2,I3,-1);  // first line inside (i1=1)
//     display(x(I1,I2,I3,Rx),"mg.vertex - periodic image N-1","%5.2f ");
    

//    display(mg.vertex(),"mg.vertex","%5.2f ");

    bool openGraphicsWindow=TRUE;
    PlotStuff ps(openGraphicsWindow,"tg");      // create a PlotStuff object
    PlotStuffParameters psp; 

    ps.erase();
    psp.set(GI_TOP_LABEL,"mg");  // set title
    PlotIt::plot(ps,mg,psp);

    if( true ) return 0;



    mg.update(MappedGrid::THEmask | MappedGrid::THEcenterBoundaryTangent);
    #ifdef USE_PPP
      realSerialArray & tangent = mg.centerBoundaryTangentArray(0,0);
    #else
      realArray & tangent = mg.centerBoundaryTangent(0,0);
    #endif

    printf(" tangent dims=[%i,%i][%i,%i][%i,%i][%i,%i][%i,%i]\n",
	   tangent.getBase(0),tangent.getBound(0),
	   tangent.getBase(1),tangent.getBound(1),
	   tangent.getBase(2),tangent.getBound(2),
	   tangent.getBase(3),tangent.getBound(3),
	   tangent.getBase(4),tangent.getBound(4));

//     if( true )
//     {
//       realArray & tangent =mg.centerBoundaryTangent(0,0); 
//       printf(" tangent dims=[%i,%i][%i,%i][%i,%i][%i,%i][%i,%i]\n",
// 	     tangent.getBase(0),tangent.getBound(0),
// 	     tangent.getBase(1),tangent.getBound(1),
// 	     tangent.getBase(2),tangent.getBound(2),
// 	     tangent.getBase(3),tangent.getBound(3),
// 	     tangent.getBase(4),tangent.getBound(4));
//     }
    return 0;
  }



  SquareMapping square(0.,1.,0.,1.);                   // Make a mapping, unit square
  square.setGridDimensions(axis1,5);                  // axis1==0, set no. of grid points
  square.setGridDimensions(axis2,5);                  // axis2==1, set no. of grid points

  AnnulusMapping annulus;
  
  // MappedGrid mg(square);                               // MappedGrid for a square
  MappedGrid mg(annulus); 



  mg.update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter );      
  
  mg.update(MappedGrid::THEvertexBoundaryNormal);      // create default variables

  for( int side=0; side<=1; side++ )
  {
    for(int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      #ifdef USE_PPP
        const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
      #else
        realArray & normal = mg.vertexBoundaryNormal(side,axis);
      #endif
      ::display(normal,sPrintF("myid=%i: vertexBoundaryNormal on (side,axis)=(%i,%i)\n",myid,side,axis),"%3.1f ");
    }
  }
  fflush(0);
  
//  mg.update(MappedGrid::THEminMaxEdgeLength);

  fflush(0);

//   bool openGraphicsWindow=TRUE;
//   PlotStuff ps(openGraphicsWindow,"tg");      // create a PlotStuff object
//   PlotStuffParameters psp; 

//   ps.erase();
//   psp.set(GI_TOP_LABEL,"gc with mg");  // set title
//   PlotIt::plot(ps,gc);


  if( myid==0 ) printf("Finished tg");

  return 0;
}

