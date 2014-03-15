#include "Parameters.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"
#include "SplineMapping.h"
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

int 
initialConditionsFromAProfile(const aString & fileName,
                              realCompositeGridFunction & u,
                              Parameters & parameters,
                              GenericGraphicsInterface & gi,
                              real rpar[] )
// =======================================================================================
// /Description:
//    Read data from a file that describes a 1D profile for each solution component.
//   Interpolate the grid function data on the CompositeGrid from the 1D profile.
//
//  /rpar (input) : rpar[0] = xShift, rpar[1]=uShift
// 
//  The data file format is as follows
// \begin{verbatim}
//     n numberOfComponents
//     x1 r1 u1 ...
//     x2 r2 u2 ...
//     ... 
//     xn rn un ...
// \end{verbatim}  
// where n data points are specified.
// =======================================================================================
{
  int debug=0;

  FILE *file;
  file = fopen ((const char*)fileName, "r");

  if( file == NULL)
  {
    printf ("initialConditionsFromAProfile:ERROR: File %s could not be opened\n", (const char*)fileName); 
    return 1;
  }

  const int buffLength=1024;
  char line[buffLength];
  getLineFromFile(file,line,buffLength);  // read a line from the file.
  
  // int numberRead=fScanF(file,"%i %i",&n,&numberOfComponents);

  int n=0, numberOfComponents=0;
  sScanF(line,"%i %i",&n,&numberOfComponents);  // read number of points and number of components

  getLineFromFile(file,line,buffLength);  // read a line from the file.
  real xShock;
  assert( numberOfComponents<20 );
  real ul[20];
  sScanF(line,"%e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e ",&xShock,
         &ul[0],&ul[1],&ul[2],&ul[3],&ul[4],&ul[5],&ul[6],&ul[7],&ul[8],&ul[9],
	 &ul[10],&ul[11],&ul[12],&ul[13],&ul[14],&ul[15],&ul[16],&ul[17],&ul[18],&ul[19]);
 
  xShock+=rpar[0];
  ul[1]+=rpar[1];

  printF("ICprofile : values larger than xShock=%9.3e will be set to [",xShock);
  for( int c=0; c<numberOfComponents; c++ ) printF("%8.2e,",ul[c]);
  printF("]\n");

  if( n>0 && numberOfComponents>0 )
  {
    RealArray xp(n), up(n,numberOfComponents);
  
    for( int i=0; i<n; i++ )
    {
       int numberRead=getLineFromFile(file,line,buffLength);

       if( numberRead==0 )
       {
	 printf("ERROR: expecting %i points in the file but only found %i\n",n,i);
	 if( i!=0 )
	 {
	   xp.resize(n);
	   up.resize(n,numberOfComponents);
	 }
 	 n=i;
         break;
       }
       if( numberOfComponents==2 )
         sScanF(line,"%e %e %e",&xp(i),&up(i,0),&up(i,1)); // 2 components
       else if( numberOfComponents==3 )
         sScanF(line,"%e %e %e %e",&xp(i),&up(i,0),&up(i,1),&up(i,2)); // 3 components
       else if( numberOfComponents==4 )
         sScanF(line,"%e %e %e %e %e",&xp(i),&up(i,0),&up(i,1),&up(i,2),&up(i,3)); // 4 components
       else if( numberOfComponents==5 )
         sScanF(line,"%e %e %e %e %e %e",&xp(i),&up(i,0),&up(i,1),&up(i,2),&up(i,3),&up(i,4)); // 5 components
       else if( numberOfComponents==6 )
         sScanF(line,"%e %e %e %e %e %e %e",&xp(i),&up(i,0),&up(i,1),&up(i,2),&up(i,3),&up(i,4),&up(i,5)); // 6 components
       else if( numberOfComponents==7 )
         sScanF(line,"%e %e %e %e %e %e %e %e",&xp(i),&up(i,0),&up(i,1),&up(i,2),&up(i,3),&up(i,4),&up(i,5),&up(i,6)); // 7 components
       else if( numberOfComponents==8 )
         sScanF(line,"%e %e %e %e %e %e %e %e %e",&xp(i),&up(i,0),&up(i,1),&up(i,2),&up(i,3),&up(i,4),&up(i,5),&up(i,6),&up(i,7)); // 8 components
       else if( numberOfComponents==9 )
         sScanF(line,"%e %e %e %e %e %e %e %e %e %e",&xp(i),&up(i,0),&up(i,1),&up(i,2),&up(i,3),&up(i,4),&up(i,5),&up(i,6),&up(i,7),&up(i,8)); // 9 components
       else if( numberOfComponents==10 )
         sScanF(line,"%e %e %e %e %e %e %e %e %e %e %e",&xp(i),&up(i,0),&up(i,1),&up(i,2),&up(i,3),&up(i,4),&up(i,5),&up(i,6),&up(i,7),&up(i,8),&up(i,9)); // 10 components
       else if( numberOfComponents==11 )
         sScanF(line,"%e %e %e %e %e %e %e %e %e %e %e %e",&xp(i),&up(i,0),&up(i,1),&up(i,2),&up(i,3),&up(i,4),&up(i,5),&up(i,6),&up(i,7),&up(i,8),&up(i,9),&up(i,10)); // 11 components
       else
       {
	 throw "error";
       }

       // adjust the position and velocity by a constant
       xp(i)+=rpar[0];
       up(i,1)+=rpar[1];
       
    }
  


    if( numberOfComponents!=u.getComponentDimension(0) )
    {
      printf("ERROR: numberOfComponents=%i is not equal to u.getComponentDimension(0)=%i\n",numberOfComponents,
               u.getComponentDimension(0));

      numberOfComponents=min(numberOfComponents,u.getComponentDimension(0));
    }
    
    GraphicsParameters params;
    if( false ) // plot data
    {
      printF(" ****************** plot the data from the file *****************\n");
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      gi.erase();
      #ifndef USE_PPP
      PlotIt::plot(gi,xp,up,"Data from file","x",parameters.dbase.get<aString* >("componentName"),params);
      #endif
      gi.erase();
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    
    // ** make a spline for the x-values ***
    SplineMapping xSpline;
    xSpline.setParameterizationType(SplineMapping::index);
  
    xSpline.setPoints(xp);
    xSpline.setGridDimensions(axis1,n); // *wdh* this was missing causing problems inverting
    xSpline.setShapePreserving(true);
    
//      // *** make splines for the components ***
//      SplineMapping *uSpline = new SplineMapping [numberOfComponents];
//      int c;
//      Range I=n;
//      for( c=0; c<numberOfComponents; c++ )
//      {
//        uSpline[c].setParameterizationType(SplineMapping::index);
//        uSpline[c].setShapePreserving(true);

//        uSpline[c].setPoints(up(I,c));
//      }
  

//      if( false )
//      {

//        gi.erase();
//        PlotIt::plot(gi,xSpline,params);
  
//        for( c=0; c<numberOfComponents; c++ )
//        {
//  	PlotIt::plot(gi,uSpline[c],params);
//        }

//        gi.erase();
//      }
    
    // *** Now interpolate solution on the CompositeGrid ****

    xSpline.approximateGlobalInverse->initialize(); // Is this needed?


    CompositeGrid & cg = *u.getCompositeGrid();
    const int numberOfDimensions = cg.numberOfDimensions();

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];

      bool isRectangular=mg.isRectangular();
      if( !isRectangular )
        mg.update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter); 
      else
        mg.update(MappedGrid::THEmask );

      #ifdef USE_PPP
        intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
        realSerialArray vertex; if( !isRectangular ) getLocalArrayWithGhostBoundaries(mg.vertex(),vertex);
        realSerialArray ug; getLocalArrayWithGhostBoundaries(u[grid],ug);
      #else
        intSerialArray & mask = mg.mask();
        const realSerialArray & vertex = mg.vertex();
        realSerialArray & ug = u[grid];
      #endif

      real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
      int iv0[3]={0,0,0}; //
      if( isRectangular )
      {
	mg.getRectangularGridParameters( dvx, xab );
	for( int dir=0; dir<numberOfDimensions; dir++ )
	  iv0[dir]=mg.gridIndexRange(0,dir);
      }
      #define XC0(i1,i2,i3) (xab[0][0]+dvx[0]*(i1-iv0[0]))
      #define XC1(i1,i2,i3) (xab[0][1]+dvx[1]*(i2-iv0[1]))
      #define XC2(i1,i2,i3) (xab[0][2]+dvx[2]*(i3-iv0[2]))

      int i1,i2,i3;
      Index I1,I2,I3;
      getIndex( mg.dimension(),I1,I2,I3 );
      bool ok = ParallelUtility::getLocalArrayBounds(mg.mask(),mask,I1,I2,I3);

      if( !ok ) continue;  // there are no points on this processor

      const int nu=I1.getLength()*I2.getLength()*I3.getLength();

      Range R=nu;
      Range Rx=numberOfDimensions;

      RealArray x,r,rn,alpha;
      IntegerArray ia,ib;

      x.redim(I1,I2,I3,Rx);
      if( !isRectangular )
      {
	x=vertex(I1,I2,I3,Rx);  // x = a copy of grid points
      }
      else
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  x(i1,i2,i3,0)=XC0(i1,i2,i3);
	  x(i1,i2,i3,1)=XC1(i1,i2,i3);
	  if( numberOfDimensions==3 ) x(i1,i2,i3,2)=XC2(i1,i2,i3);
	}
      }
      x.reshape(nu,numberOfDimensions);

      r.redim(R); rn.redim(R); alpha.redim(R);
      ia.redim(R); ib.redim(R);
      
      // *** find the r-coordinates for all the grid points ----
      //        r=-1;
      //        xSpline.inverseMap(x(R,0),r);  // trouble here if x values are outside range of xSpline?.

      rn=-1.;  // no initial guess
      xSpline.approximateGlobalInverse->findNearestGridPoint(R.getBase(),R.getBound(),x,rn);
      
      // Use linear interpolation:
      //    u(x(i)) = (1-alpha(i))*up(ia(i)) + alpha(i)*u(ia(i)+1)
      const real xEps=(xp(n-1)-xp(0))*REAL_MIN*100.;
      const real dr = 1./(n-1);
      int i,ic;
      for( i=0; i<nu; i++ )
      {
        if( fabs(rn(i,0))>9. ) // *wdh* 041120
	{ // approximate inverse failed: choose backup end values
          if(  x(i,0)<xShock )
	    rn(i,0)=0.;
          else
            rn(i,0)=1.;
	}
	

        ic=int(rn(i,0)/dr); // closest grid point less than 
        ib(i)=ic;
        // double check that we have found the closest grid point.
	while( ic<n-1 && x(i,0)>xp(ic+1)  ) 
	  ic++;
	while( ic>0 && x(i,0)<xp(ic)  )
	  ic--;

        ia(i)=ic;
      }
      ia(R)=min(n-2,max(0,ia(R)));
      
      alpha(R) = ( x(R,0)-xp(ia) )/max( xp(ia+1)-xp(ia), xEps );
      if( debug & 2 )
      {
	for( i=0; i<nu; i++ )
	{
	  printf(" i=%i x=%9.3e r=%9.3e rn=%9.3e [xp(ia),xp(ia+1)]=[%9.3e,%9.3e] ia=%i ib=%i alpha=%8.2e \n",
                 i,x(i,0),r(i,0),rn(i,0), xp(ia(i)),xp(ia(i)+1), ia(i),ib(i),alpha(i));
	}
      }
      
      // alpha should be in [0,1] except at the ends. Restrict alpha to [0,1]
      alpha(R)=min(1.,max(0.,alpha(R)));
      
//      display(x(R,0),"x(R,0)",parameters.dbase.get<FILE* >("debugFile"),"%7.1e ");
//      display(r,"r",parameters.dbase.get<FILE* >("debugFile"),"%7.1e ");

//        for( i=0; i<nu; i++ )
//        {
//  	if( fabs(r(i,0)-.5)>2. )
//  	{
//  	  printf("ERROR from inverseMap: x=%9.3e r=%9.3e \n",x(i,0),r(i,0));
//  	}
//        }

//        where( r>1. )
//        {
//  	r=1.;
//        }
//        elsewhere( r<0. )
//        {
//          r=0.;
//        }
      

      RealArray ui(I1,I2,I3);  // interpolated values for a component.
      for( int c=0; c<numberOfComponents; c++ )
      {
        ui.reshape(I1.getLength()*I2.getLength()*I3.getLength());

        // *old* uSpline[c].map(r,ui); // interpolate the component spline
      
	ui(R)=(1.-alpha(R))*up(ia,c)+alpha(R)*up(ia+1,c);  // linear interpolation

//  	for( i=0; i<nu; i++ )
//  	  printf(" i=%i c=%i ui=%11.4e ui2=%11.4e diff=%9.2e\n",i,c,ui(i),ui2(i),fabs(ui(i)-ui2(i)));

        ui.reshape(I1,I2,I3);
        ug(I1,I2,I3,c)=ui(I1,I2,I3);

      }

      if( !isRectangular )
      {
	for (int c=0; c<numberOfComponents; c++)
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    if( vertex(i1,i2,i3,0)>xShock )
	      ug(i1,i2,i3,c)=ul[c];
	  }
	}
      }
      else
      {
	for (int c=0; c<numberOfComponents; c++)
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    if( XC0(i1,i2,i3)>xShock )
	      ug(i1,i2,i3,c)=ul[c];
	  }
	}
      }
      
    } // end for grid 
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      u[grid].updateGhostBoundaries();
    }
    

    // delete [] uSpline;

    if( false ) // plot data
    {
      gi.erase();
      params.set(GI_TOP_LABEL,"Solution after interpolating from profile");
      PlotIt::contour(gi,u,params);
      gi.erase();
    }

  }


  fclose(file);
  
  return 0;


}

/* ------


int 
initialConditionsFromAProfile(const aString & fileName,
                              realCompositeGridFunction & u,
                              Parameters & parameters,
                              GenericGraphicsInterface & gi )
// =======================================================================================
// /Description:
//    Read data from a file that describes a 1D profile for each solution component.
//   Interpolate the grid function data on the CompositeGrid from the 1D profile.
//
//  The data file format is as follows
// \begin{verbatim}
//     n numberOfComponents
//     x1 r1 u1 ...
//     x2 r2 u2 ...
//     ... 
//     xn rn un ...
// \end{verbatim}  
// where n data points are specified.
// =======================================================================================
{

  FILE *file;
  file = fopen ((const char*)fileName, "r");

  if( file == NULL)
  {
    printf ("initialConditionsFromAProfile:ERROR: File %s could not be opened\n", (const char*)fileName); 
    gi.stopReadingCommandFile();
    
    return 1;
  }

  const int buffLength=200;
  char line[buffLength];
  getLineFromFile(file,line,buffLength);  // read a line from the file.
  
  // int numberRead=fScanF(file,"%i %i",&n,&numberOfComponents);

  int numberOfPoints=0, numberOfComponents=0;
  sScanF(line,"%i %i",&numberOfPoints,&numberOfComponents);  // read number of points and number of components
  if( numberOfPoints>0 && numberOfComponents>0 )
  {
    RealArray xp(numberOfPoints), up(numberOfPoints,numberOfComponents);
  
    for( int i=0; i<numberOfPoints; i++ )
    {
       int numberRead=getLineFromFile(file,line,buffLength);

       if( numberRead==0 )
       {
	 printf("ERROR: expecting %i points in the file but only found %i\n",numberOfPoints,i);
	 if( i!=0 )
	 {
	   xp.resize(numberOfPoints);
	   up.resize(numberOfPoints,numberOfComponents);
	 }
 	 numberOfPoints=i;
         break;
       }
       if( numberOfComponents==2 )
         sScanF(line,"%e %e %e",&xp(i),&up(i,0),&up(i,1)); // 2 components
       else if( numberOfComponents==3 )
         sScanF(line,"%e %e %e %e",&xp(i),&up(i,0),&up(i,1),&up(i,2)); // 3 components
       else if( numberOfComponents==4 )
         sScanF(line,"%e %e %e %e %e",&xp(i),&up(i,0),&up(i,1),&up(i,2),&up(i,3)); // 4 components
       else if( numberOfComponents==5 )
         sScanF(line,"%e %e %e %e %e %e",&xp(i),&up(i,0),&up(i,1),&up(i,2),&up(i,3),&up(i,4)); // 5 components
       else if( numberOfComponents==6 )
         sScanF(line,"%e %e %e %e %e %e %e",&xp(i),&up(i,0),&up(i,1),&up(i,2),&up(i,3),&up(i,4),&up(i,5)); // 6 components
       else
       {
	 throw "error";
       }
    }
  


    if( numberOfComponents!=u.getComponentDimension(0) )
    {
      printf("ERROR: numberOfComponents=%i is not equal to u.getComponentDimension(0)\n",numberOfComponents,
               u.getComponentDimension(0));

      numberOfComponents=min(numberOfComponents,u.getComponentDimension(0));
    }
    
    // smooth the data --- this didn't work so well
    if( false && numberOfSmooths>0 )
    {
      printf("*** initialConditionsFromAProfile: smooth the 1D profile (%i times) ****\n",numberOfSmooths);
      for( int is=0; is<numberOfSmooths; is++ )
      {
 	for( int i=1; i<numberOfPoints-1; i++ )
 	{
 	  xp(i)=.5*xp(i)+.25*(xp(i+1)+xp(i-1));
 	}
	for( int m=0; m<numberOfComponents; m++ )
	{
	  for( int i=1; i<numberOfPoints-1; i++ )
	  {
	    up(i,m)=.5*up(i,m)+.25*(up(i+1,m)+up(i-1,m));
	  }
	}
      }
    }

    // Integrate the data
    RealArray uIntegral(numberOfPoints,numberOfComponents);
    for( int m=0; m<numberOfComponents; m++ )
    {
      uIntegral(0,m)=0.;
      for( int i=1; i<numberOfPoints; i++ )
      {
	uIntegral(i,m)=uIntegral(i-1,m)+ .5*(up(i,m)+up(i-1,m))*(xp(i)-xp(i-1));
      }
    }


    
    GraphicsParameters params;
    if( false ) // plot data
    {
      gi.stopReadingCommandFile();
      gi.erase();
      PlotIt::plot(gi,xp,up,"Data from file","x",parameters.dbase.get<aString* >("componentName"),params);
      aString ans;
      cin >> ans;
      gi.erase();
    }
    
    // ** make a spline for the x-values ***

    SplineMapping xSpline;
    xSpline.setParameterizationType(SplineMapping::index);
    xSpline.setPoints(xp);
    xSpline.setShapePreserving(true);
    
//     DataPointMapping xSpline;
//     xSpline.setDataPoints(xp,1,1);

    // *** make splines for the components ***
    SplineMapping *uSpline = new SplineMapping [numberOfComponents];
    int c;
    Range I=numberOfPoints;
    for( c=0; c<numberOfComponents; c++ )
    {
      uSpline[c].setParameterizationType(SplineMapping::index);
      uSpline[c].setShapePreserving(true);

      uSpline[c].setPoints(up(I,c));
// ***      uSpline[c].setPoints(uIntegral(I,c));
    }
  

    if( false )
    {
      gi.stopReadingCommandFile();

      gi.erase();
      PlotIt::plot(gi,xSpline,params);
  
      for( c=0; c<numberOfComponents; c++ )
      {
	PlotIt::plot(gi,uSpline[c],params);
      }

      gi.erase();
    }
    
    // *** Now interpolate solution on the CompositeGrid ****

    CompositeGrid & cg = *u.getCompositeGrid();
    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      printf("initialConditionsFromAProfile: interpolate values to grid %i\n",grid);
      
      MappedGrid & mg = cg[grid];
      mg.update(MappedGrid::THEvertex);  // make sure the vertex array has been created
      realArray & ug = u[grid];

      realArray x; x=mg.vertex();  // copy of grid points

      Index I1,I2,I3;
      getIndex( mg.dimension(),I1,I2,I3 );

//       Index J1,J2,J3;
//       getIndex( mg.dimension(),J1,J2,J3,-1 );

      const int numberOfDimensions = cg.numberOfDimensions();
      x.redim(I1.getLength()*I2.getLength()*I3.getLength(),numberOfDimensions);
      
      Range R=I1.getLength()*I2.getLength()*I3.getLength();
      realArray r(R);
      
      // *** find the r-coordinates for all the grid points ----
      xSpline.useRobustInverse(true);
      r=-1;
      xSpline.inverseMap(x(R,0),r);  // trouble here if x values are outside range of xSpline?.

      // display(x(R,0),"x(R,0)");
      // display(r,"r");

      if( true )
      {
        // check the inverse -- it can make mistakes if the x-values are badly spaced.
        realArray x2(R,1);
        xSpline.map(r,x2);
	for( int i=0; i<R.getBound(); i++ )
	{
	  if( fabs(x2(i,0)-x(i,0)) > .1 )
	  {
	    printf("**WARNING: point i=%i x=%8.2e -> r=%8.2e -> x=%8.2e\n",
		   i,x(i,0),r(i,0),x2(i,0));
	  }
	}
      }

      where( r>1. )
      {
	r=1.;
      }
      elsewhere( r<0. )
      {
	r=0.;
      }

      const int nInt=5;  // average using this many points
      realArray rr(nInt),uu(nInt);

      realArray ui(I1,I2,I3);    // interpolated values for a component.
      for( c=0; c<numberOfComponents; c++ )
      {
	ui.reshape(I1.getLength()*I2.getLength()*I3.getLength());
	uSpline[c].map(r,ui); // interpolate the component spline

	
	ui.reshape(I1,I2,I3);
	ug(I1,I2,I3,c)=ui(I1,I2,I3);

        if( true )
	{
	  r.reshape(I1,I2,I3);
          int i3=I3.getBase();
          for( int i2=I2.getBase()+2; i2<=I2.getBound()-2; i2++ )
	  {
	    for( int i1=I1.getBase()+2; i1<=I1.getBound()-2; i1++ )
	    {
	      real rMin=min(r(i1-1,i2-1,i3),r(i1+1,i2-1,i3),r(i1-1,i2+1,i3),r(i1+1,i2+1,i3));
	      real rMax=max(r(i1-1,i2-1,i3),r(i1+1,i2-1,i3),r(i1-1,i2+1,i3),r(i1+1,i2+1,i3));
              
              // get the average value of spline over [rMin,rMax]
              rr.seqAdd(rMin,(rMax-rMin)/(nInt-1));
              uSpline[c].map(rr,uu);
              
              real uAve=sum(uu)/nInt;
              if( false && fabs(uAve-ug(i1,i2,i3,c))>.2 )
	      {
		rr.display("rr");
		uu.display("uu");
                printf(" r = %8.2e,%8.2e,%8.2e,%8.2e\n",r(i1-1,i2-1,i3),r(i1+1,i2-1,i3),r(i1-1,i2+1,i3),r(i1+1,i2+1,i3));
		printf(" i1,i2=%i,%i rMin=%8.2e rMax=%8.2e u=%8.2e uAve=%8.2e\n",i1,i2,rMin,rMax,ug(i1,i2,i3,c),uAve);
	      }
	      
	      ug(i1,i2,i3,c)=uAve;

	    }
	  }
	  r.reshape(R);
	}
        
      }


    }

    delete [] uSpline;

  }


  fclose(file);
  
  return 0;


}

---- */
