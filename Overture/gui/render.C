#include "GL_GraphicsInterface.h"


#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// #include <GL/glut.h>
#ifdef NO_APP
using GUITypes::real;
#endif


/* OpenGL's GL_3D_COLOR feedback vertex format. */
typedef struct _Feedback3Dcolor {
  GLfloat x;
  GLfloat y;
  GLfloat z;
  GLfloat red;
  GLfloat green;
  GLfloat blue;
  GLfloat alpha;
} Feedback3Dcolor;

// static int blackBackground = 0;  /* Initially use a white background. */
// static int lighting = 0;       /* Initially disable lighting. */
// static int polygonMode = 1;    /* Initially show wireframe. */
// static int object = 1;         /* Initially show the torus. */
static int lineStipple=FALSE;


// static GLfloat angle = 0.0;    /* Angle of rotation for object. */
// static int moving, begin;      /* For interactive object rotation. */
// static int size = 1;           /* Size of lines and points. */

static GLfloat *depthBuffer;
static int width,height;

static GLfloat currentRed=-1.,currentGreen=-1.,currentBlue=-1.;

static   GraphicsParameters::OutputFormat localOutputFormat;

#define depthr(i,j) depthBuffer[i+width*(height-j-1)]
#define depth(i,j) depthBuffer[i+width*j]

/* Write contents of one vertex to stdout. */
void
print3DcolorVertex(GLint size, GLint * count,
  GLfloat * buffer)
{
  int i;

  printf("  ");
  for (i = 0; i < 7; i++) {
    printf("%4.2f ", buffer[size - (*count)]);
    *count = *count - 1;
  }
  printf("\n");
}

void
printBuffer(GLint size, GLfloat * buffer)
{
  GLint count;
  int token, nvertices;

  count = size;
  while (count) {
    token = (int)buffer[size - count];
    count--;
    switch (token) {
    case GL_PASS_THROUGH_TOKEN:
      printf("GL_PASS_THROUGH_TOKEN\n");
      printf("  %4.2f\n", buffer[size - count]);
      count--;
      break;
    case GL_POINT_TOKEN:
      printf("GL_POINT_TOKEN\n");
      print3DcolorVertex(size, &count, buffer);
      break;
    case GL_LINE_TOKEN:
      printf("GL_LINE_TOKEN\n");
      print3DcolorVertex(size, &count, buffer);
      print3DcolorVertex(size, &count, buffer);
      break;
    case GL_LINE_RESET_TOKEN:
      printf("GL_LINE_RESET_TOKEN\n");
      print3DcolorVertex(size, &count, buffer);
      print3DcolorVertex(size, &count, buffer);
      break;
    case GL_POLYGON_TOKEN:
      printf("GL_POLYGON_TOKEN\n");
      nvertices = (int)buffer[size - count];
      count--;
      for (; nvertices > 0; nvertices--) {
        print3DcolorVertex(size, &count, buffer);
      }
    }
  }
}

GLfloat pointSize;

static const char *gouraudtriangleEPS[] =
{
  "/bd{bind def}bind def /triangle { aload pop   setrgbcolor  aload pop 5 3",
  "roll 4 2 roll 3 2 roll exch moveto lineto lineto closepath fill } bd",
  "/computediff1 { 2 copy sub abs threshold ge {pop pop pop true} { exch 2",
  "index sub abs threshold ge { pop pop true} { sub abs threshold ge } ifelse",
  "} ifelse } bd /computediff3 { 3 copy 0 get 3 1 roll 0 get 3 1 roll 0 get",
  "computediff1 {true} { 3 copy 1 get 3 1 roll 1 get 3 1 roll 1 get",
  "computediff1 {true} { 3 copy 2 get 3 1 roll  2 get 3 1 roll 2 get",
  "computediff1 } ifelse } ifelse } bd /middlecolor { aload pop 4 -1 roll",
  "aload pop 4 -1 roll add 2 div 5 1 roll 3 -1 roll add 2 div 3 1 roll add 2",
  "div 3 1 roll exch 3 array astore } bd /gouraudtriangle { computediff3 { 4",
  "-1 roll aload 7 1 roll 6 -1 roll pop 3 -1 roll pop add 2 div 3 1 roll add",
  "2 div exch 3 -1 roll aload 7 1 roll exch pop 4 -1 roll pop add 2 div 3 1",
  "roll add 2 div exch 3 -1 roll aload 7 1 roll pop 3 -1 roll pop add 2 div 3",
  "1 roll add 2 div exch 7 3 roll 10 -3 roll dup 3 index middlecolor 4 1 roll",
  "2 copy middlecolor 4 1 roll 3 copy pop middlecolor 4 1 roll 13 -1 roll",
  "aload pop 17 index 6 index 15 index 19 index 6 index 17 index 6 array",
  "astore 10 index 10 index 14 index gouraudtriangle 17 index 5 index 17",
  "index 19 index 5 index 19 index 6 array astore 10 index 9 index 13 index",
  "gouraudtriangle 13 index 16 index 5 index 15 index 18 index 5 index 6",
  "array astore 12 index 12 index 9 index gouraudtriangle 17 index 16 index",
  "15 index 19 index 18 index 17 index 6 array astore 10 index 12 index 14",
  "index gouraudtriangle 18 {pop} repeat } { aload pop 5 3 roll aload pop 7 3",
  "roll aload pop 9 3 roll 4 index 6 index 4 index add add 3 div 10 1 roll 7",
  "index 5 index 3 index add add 3 div 10 1 roll 6 index 4 index 2 index add",
  "add 3 div 10 1 roll 9 {pop} repeat 3 array astore triangle } ifelse } bd",
  NULL
};

static void
convertRGB( const GLfloat & r, const GLfloat & g, const GLfloat & b,
            GLfloat & r1, GLfloat & g1, GLfloat & b1 )
// Convert (r,g,b) to new values (r1,g1,b1) that may be gray scale
// or black and white or the original values.
{
  
  if( localOutputFormat==GraphicsParameters::grayScale )
  {
    r1= .114*b+.299*r+.587*g + .5; // convert to a gray scale
    g1=b1=r1;
  }
  else if( localOutputFormat==GraphicsParameters::blackAndWhite )
  { // convert to black and white
    r1 = (r!=0. || g!=0. || b!=0. ) ? 1. : 0.;
    g1=b1=r1;
  }
  else
  {
    r1=r;
    g1=g;
    b1=b;
  }
}

static void
averageVertex( const Feedback3Dcolor &  v0, const Feedback3Dcolor & v1, Feedback3Dcolor & va )
// compute the vertex va as the average of v0 and v1
{
  va.x=(v0.x+v1.x)*.5; va.y=(v0.y+v1.y)*.5; 
  va.red=(v0.red+v1.red)*.5; va.green=(v0.green+v1.green)*.5; va.blue=(v0.blue+v1.blue)*.5;
}

static void
drawTriangle(FILE *file,
             const Feedback3Dcolor & v0, 
	     const Feedback3Dcolor & v1,  
	     const Feedback3Dcolor & v2,
	     const int & recursionDepth=0 )  // current depth
// This routine will draw a triangle in postScript to approximate Gouraud shading
// The triangle will be recursive sub-divided into 4 pieces and plotted.
// The recursion will end for a triangle when the difference in the (r,g,b) values
// between the vertices is small enough. There is also a maximum depth for
// the recursion
{
  const real colourTolerance=.1;
  const int maxRecursionDepth=2; // 3 ok, 

  real colourDistance;
  colourDistance =  max( max(fabs(v1.red-v0.red),fabs(v2.red-v0.red)),
		         max( max(fabs(v1.green-v0.green),fabs(v2.green-v0.green)), 
                              max(fabs(v1.blue-v0.blue),fabs(v2.blue-v0.blue)) ) );
  
  const real minDist = 5*1024/width; // minimum size of a triangle in pixels on a 1024 resolution
  real xDist = max( fabs(v1.x-v0.x), fabs(v1.y-v0.y) , fabs(v2.x-v0.x), fabs(v2.y-v0.y) );

  if( colourDistance< colourTolerance || xDist<minDist || recursionDepth>=maxRecursionDepth )
  {
    // draw a triangle

//    real red=(v0.red+v1.red+v2.red)/3.;
//    real green=(v0.green+v1.green+v2.green)/3.;
//    real blue=(v0.blue+v1.blue+v2.blue)/3.;

    GLfloat red=(max(v0.red,v1.red,v2.red)+min(v0.red,v1.red,v2.red))*.5;
    GLfloat green=(max(v0.green,v1.green,v2.green)+min(v0.green,v1.green,v2.green))*.5;
    GLfloat blue=(max(v0.blue,v1.blue,v2.blue)+min(v0.blue,v1.blue,v2.blue))*.5;


    convertRGB(red, green, blue, red, green, blue );
//    fPrintF(file,"triangle[%g %g %g %g %g %g %g %g %g ]\n",red,greed,blue, v0.x,v0.y, v1.x,v1.y, v2.x,v2.y );
    
    fPrintF(file, "%g %g %g setrgbcolor\n", red, green, blue);
    fPrintF(file, "%g %g moveto\n", v0.x, v0.y);
    fPrintF(file, "%g %g lineto\n", v1.x, v1.y);
    fPrintF(file, "%g %g lineto\n", v2.x, v2.y);
    fPrintF(file, "closepath fill\n");
  }
  else
  {
    // split triangle into 4 pieces and plot each one (recursive)
    Feedback3Dcolor va,vb,vc;
    averageVertex( v0,v1,va );
    averageVertex( v0,v2,vb );
    averageVertex( v1,v2,vc );

    drawTriangle(file,v0,va,vb,recursionDepth+1);
    drawTriangle(file,va,v1,vc,recursionDepth+1);
    drawTriangle(file,va,vc,vb,recursionDepth+1);
    drawTriangle(file,vb,vc,v2,recursionDepth+1);

  }
}


static  int pattern=0;   // for line stipple dash pattern

GLfloat *
spewPrimitiveEPS(FILE * file, GLfloat * loc, const int & plotPolygons=TRUE )
{
  int token;
  int nvertices, i;
  GLfloat red, green, blue;

  GLfloat dx, dy;

  Feedback3Dcolor *vertex;



  token = (int)(*loc);
  loc++;
  real eps = .0001;
  int code, enableItem;
  real lineWidth;
  real visibleEps = .1;  // a polygon is deemed invisible if is this far behind the depth buffer
  bool visible;

  GLfloat d, x0,y0,z0, x1,y1,z1, dz, xx,yy,zz,rr,gg,bb, colourDistance, dRed,dGreen,dBlue;
  int num, pen, i1,i2;
  
  switch (token) 
  {
  case GL_LINE_RESET_TOKEN:
  case GL_LINE_TOKEN:
    vertex = (Feedback3Dcolor *) loc;

    x0=vertex[0].x;  y0=vertex[0].y;   z0=vertex[0].z; 
    x1=vertex[1].x;  y1=vertex[1].y;   z1=vertex[1].z; 

    num=(int)max( 2., SQRT( double(SQR(x1-x0)+SQR(y1-y0) )) );  // distance in pixels between points 

    dx=(x1-x0)/(num-1); dy=(y1-y0)/(num-1); dz=(z1-z0)/(num-1);

    dRed=  (vertex[1].red  -vertex[0].red  )/(num-1);
    dGreen=(vertex[1].green-vertex[0].green)/(num-1);
    dBlue= (vertex[1].blue -vertex[0].blue )/(num-1);
    colourDistance=fabs(dRed)+fabs(dGreen)+fabs(dBlue);
    

    pen=-1;  // pen>0 : drawing a line, pen<0  not drawing, pen==0 : start a new line
    for( i=0; i<num; i++ )
    {
      xx=x0 +i*dx;
      yy=y0 +i*dy;
      zz=z0 +i*dz;
      
      i1=max(0,min(width-1 ,int(xx+.5)));
      i2=max(0,min(height-1,int(yy+.5)));
      d=depth(i1,i2)+eps;  // depth buffer value at nearest pixel
      if(  zz > d )
      {
	// the point is invisible 
	if( pen>0 )
	{
	  fPrintF(file, "%g %g lineto stroke\n", xx, yy);   // finish line seg if pen was down
          pen=-1;
	}
      }
      else
      {
        // visible point
        if( pen<0 )
	{
          pen=0;  // start line seg if pen was up
	}
        else if( colourDistance*pen > 10 )   // restart line as colour has changed enough
	{
          fPrintF(file, "%g %g lineto stroke\n", xx, yy);  // finish line
          pen=0;  // start a new one
	}
        else
          pen++;   // count steps
      }
      if( pen==0 && i<num-1 )
      {
        // start a new line (unless this is the last point)
        pen=1;
	rr=vertex[0].red  +i*dRed;
	gg=vertex[0].green+i*dGreen;
	bb=vertex[0].blue +i*dBlue;
        if( fabs(rr-currentRed)+fabs(gg-currentGreen)+fabs(bb-currentBlue) > 0 )
	{
          currentRed=rr; currentGreen=gg; currentBlue=bb;
          convertRGB(currentRed,currentGreen,currentBlue,rr,gg,bb );
  	  fPrintF(file, "%g %g %g setrgbcolor\n", rr, gg, bb);
	}
	fPrintF(file, "%g %g moveto\n", xx, yy);
      }
    }
    if( pen>0 )
      fPrintF(file, "%g %g lineto stroke\n", xx, yy);  // finish line


    loc += 14;          /* Each vertex element in the feedback
                           buffer is 7 GLfloats. */

    break;
  case GL_POLYGON_TOKEN:
    nvertices = (int)(*loc);
    if( nvertices>10 )
      printf("WARNING: number of vertices =%i \n",nvertices);
    
    loc++;
    if( !plotPolygons )
    {
      loc += nvertices * 7;
      break;
    }

    vertex = (Feedback3Dcolor *) loc;

    if (nvertices > 0) 
    {
      // first check if the polygon is (probably visible) -- it is invisble if all the
      // vertices are more than visibleEps behind the depth buffer values
      visible=FALSE;
      for (i = 0; i < nvertices; i++) 
      {
        i1=max(0,min(width-1 ,int(vertex[i].x+.5)));
        i2=max(0,min(height-1,int(vertex[i].y+.5)));
        d=depth(i1,i2)+eps;  // depth buffer value at nearest pixel
        if(  vertex[i].z < d + visibleEps )   // visibleEps is a safety factor
	{
	  // the point is visible 
          visible=TRUE;
	  break;
	}
      }
      if( visible )
      {
        //  Break polygon into "nvertices-2" triangle fans. 
        for (i = 0; i < nvertices - 2; i++)
          drawTriangle(file,vertex[0],vertex[i+1],vertex[i+2]);
      }
    }
    loc += nvertices * 7;  /* Each vertex element in the
                              feedback buffer is 7 GLfloats. */
    break;
  case GL_POINT_TOKEN:
    vertex = (Feedback3Dcolor *) loc;
    if( fabs(vertex[0].red-currentRed)+fabs(vertex[0].green-currentGreen)+fabs(vertex[0].blue-currentBlue) > 0 )
    {
      currentRed=vertex[0].red; currentGreen=vertex[0].green; currentBlue=vertex[0].blue;
      convertRGB(currentRed,currentGreen,currentBlue,red,green,blue );
      fPrintF(file, "%g %g %g setrgbcolor\n", red, green, blue);
    }
    fPrintF(file, "%g %g %g 0 360 arc fill\n\n", vertex[0].x, vertex[0].y, pointSize / 2.0);
    loc += 7;           /* Each vertex element in the feedback
                           buffer is 7 GLfloats. */
    break;
  case GL_PASS_THROUGH_TOKEN:
    // printf("pass through token found\n");
    code = int(*loc +.5);
    switch( code )
    {
    case GL_DISABLE_TOKEN:
      loc+=2; 
      enableItem=(int)(*loc+.5);
      if( enableItem==GL_LINE_STIPPLE )
      {
	// printf("line stipple disabled\n");
        if( lineStipple )
	{
          fPrintF(file, "[] 0 setdash\n"); // **** set dash lines back to solid***
	  lineStipple=FALSE;
	}
      }
      loc++;
      break;
    case GL_ENABLE_TOKEN:
      loc+=2;
      enableItem=(int)(*loc+.5);
      if( enableItem==GL_LINE_STIPPLE )
      {
	// printf("line stipple enabled\n");
        if( !lineStipple && pattern!=0 )
	{
          lineStipple=TRUE;
          fPrintF(file, "[3] 0 setdash\n"); // 3 on, 3 off, ... **** set dash lines [d1 d1 d3 ...] start setdash***
	}
      }
      loc++;
      break;
    case GL_LINE_WIDTH_TOKEN:
      loc+=2;  // skip next pass through token
      lineWidth=*loc;
      fPrintF(file, "%g setlinewidth\n", lineWidth);
      // printf("** new lineWidth =%e \n",lineWidth);
      loc++;
      break;
    case GL_LINE_STIPPLE_TOKEN:
      loc+=2;  // skip next pass through token
      // numberOfBytes=int(*loc+.5);
      loc+=2;
      pattern = (int)(*loc);
      // printf("line stipple: numberOfBtypes=%i, pattern=%i \n",numberOfBytes,pattern);
      if( lineStipple )
      {
        fPrintF(file, "[3] 0 setdash\n"); // 3 on, 3 off, ... **** set dash lines [d1 d1 d3 ...] start setdash***
      }
      loc++;
      break;
    case GL_POINT_SIZE_TOKEN:
      loc+=2;  // skip next pass through token
      pointSize=*loc;
      loc++;
      break;
    default:
      printf("UNKNOWN pass through token code=%i, GL_LINE_WIDTH_TOKEN=%i \n",code,GL_LINE_WIDTH_TOKEN); 
    }
    break;
  case GL_BITMAP_TOKEN:
  case GL_DRAW_PIXEL_TOKEN:
  case GL_COPY_PIXEL_TOKEN:
  default:
    /* XXX Left as an excersie to the reader. */
    printf("spewPrimitiveEPS: Incomplete implementation.  Unexpected token (%d).\n", token);
  }
  return loc;
}

void
spewUnsortedFeedback(FILE * file, GLint size, GLfloat * buffer)
{
  GLfloat *loc, *end;

  loc = buffer;
  end = buffer + size;
  while (loc < end) {
    loc = spewPrimitiveEPS(file, loc);
  }
}

typedef struct _DepthIndex {
  GLfloat *ptr;
  GLfloat depth;
} DepthIndex;

static int
compare(const void *a, const void *b)
{
  DepthIndex *p1 = (DepthIndex *) a;
  DepthIndex *p2 = (DepthIndex *) b;
  GLfloat diff = p2->depth - p1->depth;

  if (diff > 0.0) {
    return 1;
  } else if (diff < 0.0) {
    return -1;
  } else {
    return 0;
  }
}

/* ----
static int
compare2(const void *a, const void *b)
// ***wdh** reverse comparison ****
{
  DepthIndex *p1 = (DepthIndex *) a;
  DepthIndex *p2 = (DepthIndex *) b;
  GLfloat diff = p2->depth - p1->depth;

  if (diff > 0.0) {
    return -1;
  } else if (diff < 0.0) {
    return +1;
  } else {
    return 0;
  }
}
---- */

void
spewSortedFeedback(FILE * file, GLint size, GLfloat * buffer)
{
  int token;
  GLfloat *loc, *end;
  Feedback3Dcolor *vertex;
  GLfloat depthSum;
  int nprimitives, item;
  DepthIndex *prims;
  int nvertices, i;

  end = buffer + size;

  // Count how many polygon primitives there 
  nprimitives = 0;
  loc = buffer;
  while (loc < end) {
    token = (int)(*loc);
    loc++;
    switch (token) {
    case GL_LINE_TOKEN:
    case GL_LINE_RESET_TOKEN:
      loc += 14;
      break;
    case GL_POLYGON_TOKEN:
      nvertices = (int)(*loc);
      loc++;
      loc += (7 * nvertices);
      nprimitives++;
      break;
    case GL_POINT_TOKEN:
      loc += 7;
      break;
    case GL_PASS_THROUGH_TOKEN:
      loc++;
      break;
    default:
      /* XXX Left as an excersie to the reader. */
      printf("Incomplete implementation.  Unexpected token (%d).\n",
        token);
    }
  }

  /* Allocate an array of pointers that will point back at
     primitives in the feedback buffer.  There will be one
     entry per primitive.  This array is also where we keep the
     primitive's average depth.  There is one entry per
     primitive  in the feedback buffer. */
//  prims = (DepthIndex *) malloc(sizeof(DepthIndex) * nprimitives);
  prims = new DepthIndex [nprimitives];
  
  item = 0;
  loc = buffer;
  while (loc < end) {
    prims[item].ptr = loc;  /* Save this primitive's location. */
    token = int( *loc );
    loc++;
    switch (token) {
    case GL_LINE_TOKEN:
    case GL_LINE_RESET_TOKEN:
      loc += 14;
      break;
    case GL_POLYGON_TOKEN:
      nvertices = (int)(*loc);
      loc++;
      vertex = (Feedback3Dcolor *) loc;
      depthSum = vertex[0].z;
      for (i = 1; i < nvertices; i++) {
        depthSum += vertex[i].z;
      }
      prims[item].depth = depthSum / nvertices;
      loc += (7 * nvertices);
      item++;
      break;
    case GL_POINT_TOKEN:
      loc += 7;
      break;
    case GL_PASS_THROUGH_TOKEN:
      // skip these
      loc++;
      break;
    default:
      /* XXX Left as an excersie to the reader. */
      assert(1);
    }
  }
  assert(item == nprimitives);
  printf("Number of polygon primitives to sort = %i \n",nprimitives);

  /* Sort the primitives back to front. */
  qsort(prims, nprimitives, sizeof(DepthIndex), compare);

  /* XXX Understand that sorting by a primitives average depth
     doesn't allow us to disambiguate some cases like self
     intersecting polygons.  Handling these cases would require
     breaking up the primitives.  That's too involved for this
     example.  Sorting by depth is good enough for lots of
     applications. */

  /* Emit the Encapsulated PostScript for the primitives in
     back to front order. */
  // first draw polygons
  if( FALSE )
  {
    for (item = 0; item < nprimitives; item++) 
    {
      if( *prims[item].ptr==(GLfloat)GL_POLYGON_TOKEN )
	spewPrimitiveEPS(file, prims[item].ptr);
    }
    // now draw lines and points
  

    loc = buffer;
    while (loc < end) 
    {
      token = (int)(*loc);
      loc=spewPrimitiveEPS(file, loc, FALSE);
    }
  }

//  free(prims);
  
  delete [] prims;  // ***************************************
  
}

#define EPS_GOURAUD_THRESHOLD 0.1  /* Lower for better (slower) smooth shading. */

void
spewWireFrameEPS(FILE * file, int doSort, GLint size, GLfloat * buffer, char *creator)
{
  GLfloat clearColor[4], viewport[4];
  GLfloat lineWidth;
  int i;

  /* Read back a bunch of OpenGL state to help make the EPS
     consistent with the OpenGL clear color, line width, point
     size, and viewport. */
  glGetFloatv(GL_VIEWPORT, viewport);
  glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
  lineWidth=1.; //  glGetFloatv(GL_LINE_WIDTH, &lineWidth);  
  pointSize=1.; //  glGetFloatv(GL_POINT_SIZE, &pointSize);

/* ---
  // Emit EPS header.
  fputs("%!PS-Adobe-2.0 EPSF-2.0\n", file);
  //  Notice %% for a single % in the fPrintF calls. 
  fPrintF(file, "%%%%Creator: %s (using OpenGL feedback)\n", file, creator);
  fPrintF(file, "%%%%BoundingBox: %g %g %g %g\n",
    viewport[0], viewport[1], viewport[2], viewport[3]);
  fputs("%%EndComments\n", file);
  fputs("\n", file);
  fputs("gsave\n", file);
  fputs("\n", file);
--- */

  
  real llbx,llby,urbx,urby,scaleFactor;

  // 
  // Scale the picture:
  //  Note we assume that the user meant to have width=height, thus we scale the
  // result to have a square aspect ratio
  //

  real pageWidth=8.5;  // page width in inches

  real pageHeight=11.;  
  real leftMargin=.5;   
  real rightMargin=.5;
  real bottomMargin=.5;
  real topMargin=.5;
  real w=(pageWidth-leftMargin-rightMargin)*72.;  // width of space for figure (in pts. 1/72 inch)
  real h=(pageHeight-bottomMargin-topMargin)*72.;
  if( height/width <= h/w )
  { // the width of the figure determines how it should be scaled
    llbx=leftMargin*72.;
    llby=bottomMargin*72.;
    scaleFactor=w/width;
    // scale=w;
  }
  else
  {
    scaleFactor=h/height;
    // scale=h;
    llbx=leftMargin*72.+.5*(w-width*scaleFactor);   // figure is narrow, centre it in the x-direction
    llby=bottomMargin*72.;
  }
  urbx=llbx+width*scaleFactor;
  urby=llby+max(width,height)*scaleFactor;

  fPrintF(file,"%%!PS-Adobe-2.0 EPSF-2.0\n");
  fPrintF(file,"%%%%Creator: PlotStuff v1.0\n");
  fPrintF(file,"%%%%Title: What a concept! \n");
  fPrintF(file,"%%%%CreationDate: Fri Feb 29 12:34:56 1999 \n");
  fPrintF(file,"%%%%Pages: 1 \n");
  fPrintF(file,"%%%%Requirements: colorprinter \n");
  fPrintF(file,"%%%%BoundingBox: %i %i %i %i \n",int(llbx+.5),int(llby+.5),int(urbx+.5),int(urby+.5));
  fPrintF(file,"%%%%EndComments \n");
  fPrintF(file,"%%%%EndProlog \n");
  fPrintF(file,"%%%%Page: 1 1 \n");
  fPrintF(file,"gsave \n");

  // Save the colour table
  fPrintF(file,"%f %f translate \n",llbx,llby);
  fPrintF(file,"%f %f scale \n",scaleFactor,scaleFactor);  // note -- use scaleFactor instead of scale


  /* Output Frederic Delhoume's "gouraudtriangle" PostScript
     fragment. */
  fputs("% the gouraudtriangle PostScript fragement below is free\n", file);
  fputs("% written by Frederic Delhoume (delhoume@ilog.fr)\n", file);
  fPrintF(file, "/threshold %g def\n", EPS_GOURAUD_THRESHOLD);
  for (i = 0; gouraudtriangleEPS[i]; i++) {
    fPrintF(file, "%s\n", gouraudtriangleEPS[i]);
  }

  fPrintF(file, "\n%g setlinewidth\n", lineWidth);

  /* Clear the background like OpenGL had it. */
  fPrintF(file, "%g %g %g setrgbcolor\n",
    clearColor[0], clearColor[1], clearColor[2]);
  fPrintF(file, "%g %g %g %g rectfill\n\n",
    viewport[0], viewport[1], viewport[2], viewport[3]);

  if (doSort) 
    spewSortedFeedback(file, size, buffer);
  else
    spewUnsortedFeedback(file, size, buffer);

  /* Emit EPS trailer. */
  fputs("grestore\n\n", file);
  fputs("%Add `showpage' to the end of this file to be able to print to a printer.\n",
    file);
  fputs("showpage\n\n", file);

  fclose(file);
  printf("postScript file written\n");
}


void display(void);
void init(void);


int GL_GraphicsInterface::
renderPS(const char * fileName,
	 GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ )
// ===============================================================================================
// /Description:
//    Render the screen directly into post-script
// ===============================================================================================
{
  if( parameters.isDefault() )
  {
    localOutputFormat=outputFormat[currentWindow];
  }
  else
  {
    localOutputFormat=parameters.outputFormat;
  }


  GLint params[8];
  glGetIntegerv(GL_VIEWPORT,params);
  width = params[2];
  height=params[3];
  printf("Current viewPort: width = %i, height = %i \n",width,height);
   
  currentRed=-1.;   // These hold the current (r,g,b) values that have been set in the .ps file
  currentGreen=-1.; // if the new colour is different than these then we set the new colour in the file.
  currentBlue=-1.;  // initially set to impossible values

  printf("sizeof(GLushort)=%i, sizeof(GLuint)=%i \n",(int)sizeof(GLushort), (int)sizeof(GLuint));
  
  // read the depth buffer
  depthBuffer = new GLfloat [width*height]; // ************************************

//GLushort *db = new GLushort[width*height];
  int x0=0, y0=0;
//  width=0;
//  height=0;
  
  glReadPixels(x0,y0,width,height,GL_DEPTH_COMPONENT,GL_FLOAT,depthBuffer);
//glReadPixels(x0,y0,width,height,GL_DEPTH_COMPONENT,GL_UNSIGNED_SHORT,db);

/* ---
  int i,j;

  real factor=1./(pow(2,16)-1.);
  for( i=0; i<width*height; i++ )  
  {
//    depthBuffer[i]=db[i]*factor;
    depthBuffer[i]=1.;
  }
//  delete [] db;

--- */
/* ---
  int m=30;
  int i0=(width-m)/2;
  int j0=(height-m)/2;
  printf("depth buffer: \n");
  for( j=j0; j<j0+m; j++ )  
  {
    for( i=i0; i<i0+m; i++ )  
      printf("%3.2f ",depthBuffer[i+width*j]);
    printf("\n");
  }
--- */  


  GLfloat *feedbackBuffer;
  FILE *file;
  GLint numberOfValues=0;
  const int size = 5000000;

  feedbackBuffer = new GLfloat[size];
  if( FALSE )
  {
    glFeedbackBuffer(size, GL_3D_COLOR, feedbackBuffer );
    glRenderMode(GL_FEEDBACK);
  
    printf("render in feedback mode\n");

    init(currentWindow);
    display(currentWindow); 
//  glFinish();

    numberOfValues = glRenderMode(GL_RENDER);

// * no help *  display();

    printf("render in feedback mode done, number of values in feedback array=%i \n",numberOfValues);
  }
  
  
  file = fopen((const char*)fileName, "w" );
  
  if( FALSE )
    printBuffer(numberOfValues,feedbackBuffer);

  int doSort=TRUE;
  if( FALSE )
    spewWireFrameEPS(file, doSort, numberOfValues, feedbackBuffer, (char*)"renderPS" );
  
  delete [] feedbackBuffer; 
  delete [] depthBuffer;

  return 0;
}

