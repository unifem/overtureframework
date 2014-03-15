#include "GL_GraphicsInterface.h"


#include "mogl.h"

#ifdef NO_APP
using GUITypes::real;
#endif


void display(const int & win_number);

void init(const int & win_number);


int GL_GraphicsInterface::
offScreenRender(const char * fileName,
                GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ )
// ===============================================================================================
// /Description:
//    Render off screen in a buffer.
// /resolution (input) : resolution of the buffer max(width,height)=resolution. If resolution<=0 then
//    the maximum available resolution is chosen.
// ===============================================================================================
{
  int returnValue=0;
  
  returnValue=offScreenRenderMesa(fileName,parameters);  // first try Mesa. why?
#ifdef OV_USE_X11
  if( returnValue==1 )
  {
    returnValue=offScreenRenderX(fileName,parameters);
  }
#endif
  if( returnValue==1 )
  {
    printf("ERROR:uanble to perform off-screen rendering for the hardCopy command\n"
           "      neither OSMesa or X-pixmap rendering seems to be available\n"
           "      Try choosing the `frame buffer' rendering option\n");
  }

  return returnValue;
}


#ifdef OV_USE_X11

#include <GL/glx.h> 
int
moglGetInfo( Display *&dpy_, XVisualInfo *&vi, GLXContext & cx );

int GL_GraphicsInterface::
offScreenRenderX(const char * fileName,
                GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ )
// ===============================================================================================
// /Description:
//    Render off screen in a buffer. This version uses an X pixmap to render into. It may not work
//    if the glx server is using direct rendering.
// /resolution (input) : resolution of the buffer max(width,height)=resolution. If resolution<=0 then
//    the maximum available resolution is chosen.
// ===============================================================================================
{
  int oldCurrent = moglGetCurrentWindow();
  GLint params[8];

  // printf("INFO: The hardCopy function only works in the active window.\n");

  // get the size of the current window, so we can reset once we are done.
  glGetIntegerv(GL_VIEWPORT,params);
  const int width = params[2], height=params[3];
  
  // printf("osRenderX: Current viewPort: width = %i, height = %i \n",width,height);
   

  glGetIntegerv(GL_MAX_VIEWPORT_DIMS,params);
  GLint width2 = params[0], height2=params[1];
  // printf("Largest possible viewPort: width = %i, height = %i \n",width2,height2);

  int resolution= parameters.isDefault() || parameters.rasterResolution<=0 ? 
    rasterResolution[currentWindow] : parameters.rasterResolution;

  int horizontalResolution= parameters.isDefault() || parameters.rasterResolution<=0 ? 
    horizontalRasterResolution[currentWindow] : parameters.rasterResolution;
  
  resolution = min(resolution, params[1]);
  horizontalResolution = min(horizontalResolution, params[0]);

  width2 = horizontalResolution;
  height2 = resolution;

  Display *dpy;
  XVisualInfo *vi;
  GLXContext cxMogl;
  Pixmap pmap;
  GLXPixmap glxpmap;
  int imageWidth = width2, imageHeight = height2;

// get the display, visual and glxcontext from mogl
  moglGetInfo( dpy,vi,cxMogl );

  if( glXIsDirect(dpy, cxMogl) )
  {
    printf("offScreenRender:WARNING: OpenGL is using DIRECT rendering to the hardware. It may not be\n"
           "   possible to render off screen. If this message is followed by an X error then you should\n"
           "   choose the hardcopy rendering option `frame buffer' instead of `off screen' \n");
  }
//    printf("offScreenRender: render image, context is %s \n", 
//  	 (glXIsDirect(dpy, cxMogl))? "DIRECT": "NOT DIRECT");

//    //make a context - no direct rendering
//    GLXContext cx = glXCreateContext(dpy, vi, /* display list sharing */ cxMogl,
//  			/* No direct */ GL_FALSE);

//    int configuration[] =  { GLX_DOUBLEBUFFER, GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1,
//                             GLX_BLUE_SIZE, 1, None   };
     
//    vi = glXChooseVisual(dpy,DefaultScreen(dpy), &configuration[1]);
//    if( vi==NULL )
//    {
//      vi = glXChooseVisual(dpy,DefaultScreen(dpy), &configuration[0]);
//      if( vi==NULL )
//      {
//        printf("offScreenRender:ERROR choosing a visual\n");
//        throw "error";
//      }
//    }
  
  if (dpy == NULL)
    printf("could not open display \n");

  if (!glXQueryExtension(dpy, NULL, NULL))
    printf("X server has no OpenGL GLX extension\n");

  pmap = XCreatePixmap(dpy, RootWindow(dpy, vi->screen), imageWidth, imageHeight, vi->depth);
  glxpmap = glXCreateGLXPixmap(dpy, vi, pmap);

  // printf("Render off screen, width = %i, height = %i \n",width2,height2);

// ***  glXMakeCurrent(dpy, glxpmap, cxMogl);
  glXMakeCurrent(dpy, glxpmap, cxMogl);

// set the drawing for single buffering
  glDrawBuffer(GL_FRONT);


  glViewport(0, 0, width2,height2); 
  init(currentWindow); // updates leftSide, rightSide, top and bottom. Also updates the projection matrix
  display(currentWindow); 
  glFinish();
  // printf("offScreenRender: save Raster...\n");

//   GLvoid *pixels;

//   int inColor=1; // get colours
//   pixels = grabPixels(inColor, width, height);

  float *xBuffer = new float [width2*height2*3+1000];
  for( int n=0; n<width2*height2*3+1000; n++ )
    xBuffer[n]=0.;
  
  GLint x=0,y=0;  
  glReadBuffer(GL_FRONT);
  glReadPixels( x,y,width2,height2,GL_RGB,GL_FLOAT,xBuffer);  // read the frame buffer
  glReadBuffer(GL_BACK);


  int rgbType=0;
  saveRasterInAFile(fileName,xBuffer,width2,height2,rgbType,parameters);

  // kkc 080428 moved the moglMakeContext to before glXDestroyGLXPixmap to prevent a seg fault
// reset the screen and the viewing transformations
  moglMakeCurrent(oldCurrent); // AP: This call seems pretty redundant!
  glViewport(0, 0, width,height); 
  init(currentWindow);

// cleanup
  delete [] xBuffer;
  glXDestroyGLXPixmap(dpy, glxpmap);
  XFreePixmap(dpy, pmap);
  
  // printf("offScreenRender:all done\n");

// reset the drawing for double buffering
  glDrawBuffer(GL_BACK);
  
  return 0;
}

#endif
// end ifdef OV_USE_X11


// 050128  kkc adjusted so that we can use the NVIDIA drivers with Mesa GLwM widgets (common on linux)
#if !defined(OV_USE_MESA) || OV_USE_MESA==2

int GL_GraphicsInterface::
offScreenRenderMesa(const char * fileName,
		    GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ )
{
  // printf("offScreenRenderMesa: Sorry, Mesa off-screen rendering is not available\n.");
  return 1;  // Mesa OS rendering is not available.
}


#else

// ******************** use this version with Mesa off-screen rendering ********************************


#include "GL/osmesa.h"
extern "C"
{
// Mesa 4.0.3 used:  #include "GL/../../src/context.h"
// For Mesa 6.2.1 :
#include "GL/../../src/mesa/main/context.h" 
}


// *********************************************************************************************
// *** This is taken from Mesa-xxx/src/OSmesa/osmesa.c  ******

/*
 * This is the OS/Mesa context struct.
 * Notice how it includes a GLcontext.  By doing this we're mimicking
 * C++ inheritance/derivation.
 * Later, we can cast a GLcontext pointer into an OSMesaContext pointer
 * or vice versa.
 */
struct osmesa_context {
   GLcontext gl_ctx;		/* The core GL/Mesa context */
   GLvisual *gl_visual;		/* Describes the buffers */
   GLframebuffer *gl_buffer;	/* Depth, stencil, accum, etc buffers */
   GLenum format;		/* either GL_RGBA or GL_COLOR_INDEX */
   void *buffer;		/* the image buffer */
   GLint width, height;		/* size of image buffer */
   GLint rowlength;		/* number of pixels per row */
   GLint userRowLength;		/* user-specified number of pixels per row */
   GLint rshift, gshift;	/* bit shifts for RGBA formats */
   GLint bshift, ashift;
   GLint rInd, gInd, bInd, aInd;/* index offsets for RGBA formats */
   GLchan *rowaddr[MAX_HEIGHT];	/* address of first pixel in each image row */
   GLboolean yup;		/* TRUE  -> Y increases upward */
				/* FALSE -> Y increases downward */
};
// ***********************************************************************************

static OSMesaContext ctx2=NULL;

int GL_GraphicsInterface::
offScreenRenderMesa(const char * fileName,
                GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ )
// ===============================================================================================
// /Description:
//    Render off screen in a buffer.
// /resolution (input) : resolution of the buffer max(width,height)=resolution. If resolution<=0 then
//    the maximum available resolution is chosen.
//
// /Notes: We have to play some games here to use Mesa off-screen rendering. We cannot follow
//   the example in the Mesa/demos/osdemo.c  since the Overture display(..) 
//  function does not generate the display lists, rather they are associated with
//  the current context. Therefore must associate the display lists of the current context
//  with the the OSMesa off-screen context. This requires that we use some non-public parts
//  of Mesa: we include context.h and the struct above.

{
  const real time=getCPU();

  GLint params[8];
  glGetIntegerv(GL_VIEWPORT,params);
  int width = params[2], height=params[3];

  int debug=0;

  if( false )
  {
    printf("\nNOTE: Currently, this function only works in the active window.\n"
	   "To make a window active, use the figure command\n\n");
  }
  
  if( debug >0 ) printf("Current viewPort: width = %i, height = %i \n",width,height);
   

  glGetIntegerv(GL_MAX_VIEWPORT_DIMS,params);
  GLint width2 = params[0], height2=params[1];
  // printf("Largest possible viewPort: width = %i, height = %i \n",width2,height2);

  int resolution= parameters.isDefault() || parameters.rasterResolution<=0 ? 
    rasterResolution[currentWindow] : parameters.rasterResolution;

  int horizontalResolution= parameters.isDefault() || parameters.rasterResolution<=0 ? 
    horizontalRasterResolution[currentWindow] : parameters.rasterResolution;
  
  resolution = min(resolution, params[1]);
  horizontalResolution = min(horizontalResolution, params[0]);

  width2 = horizontalResolution;
  height2 = resolution;
  
  GLcontext *CC = (GLcontext *)OSMesaGetCurrentContext(); // get the current context
  
  // *wdh* 040814 To get around a memory leak -- only allocate ctx2 once 
  // *wdh* 040814 OSMesaContext ctx, ctx2;  // *wdh* 040814
  OSMesaContext ctx;

   /* Create an RGBA-mode context */

   // We want to share the display lists of the current context (CC) with the OSMesa context ctx.
   // To do this first create another OSMesa context, ctx2, and substitute the current 
   // context into this OSMesaContext
  if( ctx2==NULL )  // *wdh* 040814
    ctx2 = OSMesaCreateContext( GL_RGBA, NULL );

  GLcontext *oldCC = (GLcontext*)(&ctx2->gl_ctx);
  ctx2->gl_ctx=* (GLcontext*)CC;  // substitute the current context into this OSMesaContext

  // create an OSMesa context and share display lists with the current context (in ctx2)
  ctx = OSMesaCreateContext( GL_RGBA, ctx2 );  

  // copy over some of the attributes: it didn't work to copy all attributes // GL_ALL_ATTRIB_BITS );
  // see Mesa/src/context.c for the next function:
//   gl_copy_context( &CC, ctx->gl_ctx, GL_LIGHTING_BIT | GL_TEXTURE_BIT ); 
  _mesa_copy_context( CC, (GLcontext*)(&ctx->gl_ctx), (GL_ALL_ATTRIB_BITS & ! GL_TRANSFORM_BIT) ); // ?? maybe

//                                   GL_LIGHTING_BIT 
//    	                            | GL_TEXTURE_BIT );
//                                  | GL_TRANSFORM_BIT   // trouble with one of these
//                                  | GL_VIEWPORT_BIT); 

  /* Allocate the image buffer */
  void *buffer;
  buffer = malloc( width2 * height2 * 4 );

   /* Bind the buffer to the context and make it current */
  OSMesaMakeCurrent( ctx, buffer, GL_UNSIGNED_BYTE, width2, height2 );

  if( debug >0 ) printf("offScreenRender: render image \n");
  if( debug >0 ) printf("Render with  width = %i, height = %i \n",width2,height2);

  glViewport(0, 0, width2,height2); 
  init(currentWindow);
  display(currentWindow); 
  glFinish();
  if( debug >0 ) printf("offScreenRender: save Raster...\n");

  int rgbType=1;
  saveRasterInAFile(fileName,buffer,width2,height2,rgbType,parameters);

  if( debug >0 ) printf("offScreenRender:all done\n");


  // reset the context to CC 
//  moglResetContext(); AP: This is done by the calling routine

  /* free the image buffer */
  free( buffer );

  /* destroy the context */
  OSMesaDestroyContext( ctx ); // *** is this ok ??
  ctx2->gl_ctx=*((GLcontext*)oldCC);
// **  OSMesaDestroyContext( ctx2 ); // *** is this ok ?? *wdh* 020705 : this is not ok with latest Mesa. Do we leak?

  printf(" offScreenRender: image was rendered, raster size=%i by %i, cpu=%5.1f (s)\n",width2,height2,getCPU()-time);

  return 0;
}

#endif // OV_USE_MESA


/* ----
void
rleCompress2( const int num, int *xBuffer, FILE *outFile, const int numPerLine )
//  Save 2 bytes from the buffer
{

  // printf("\n\n\n ***** rleCompress ****** n\n\n");

  int r;         // repetition count
  // int rb=1;  // bytes saved 
  int rb = 2;

  int maxR=128/rb;  // largest repetition count allowed is 128/rb

  int i=0;           // current char
  int count=0;       // number of chars printed on current lline
  while( i<num )
  {
    // count the number of similar chars
    r=1;
    while( r<maxR && i+r<num && xBuffer[i+r]==xBuffer[i] )
    {
      r++;
    }
    if( r>1 )
    {
      // printf("repeat: r=%i, char=%2.2X \n",r,xBuffer[i]);
      fPrintF(outFile,"%2.2X",257-r*rb);   // length = 257-r*rb
      fPrintF(outFile,"%4.4X",xBuffer[i]);
      i+=r;
      count+=1+rb;
    }
    else
    { // : b[i+1]!=b[i]
      // count number of dis-similiar chars
      r=1;
      while( r<maxR && i+r+1 < num && xBuffer[i+r+1]!=xBuffer[i+r] )
      {
	r++;
      }
      // printf("dis-similar: r=%i, start-1=%2.2X start=%2.2X , end=%2.2X, end+1=%2.2X \n",r,xBuffer[i-1],
      //       xBuffer[i],xBuffer[i+r-1],xBuffer[i+r]);
      fPrintF(outFile,"%2.2X",r*rb-1);   // length = r-1  [0,127]
      for( int j=i; j<i+r; j++ )
      {
	fPrintF(outFile,"%4.4X",xBuffer[j]);
        count+=rb;
	if( count > numPerLine )
	{
	  fPrintF(outFile,"\n");
	  count=0;
	}
      }
      i+=r;
    }
    if( count > numPerLine )
    {
      fPrintF(outFile,"\n");
      count=0;
    }
  }
  // write EOD    
  fPrintF(outFile,"%2.2X",128);

  // add some zeroes to the end of the data -- needed by printer for some reason
  for( ;count<=numPerLine; count++ )  
    fPrintF(outFile,"%2.2X",0);        
  fPrintF(outFile,"\n");
  for( i=0; i<2; i++ )
  {
    for( count=0; count<numPerLine; count++ ) 
      fPrintF(outFile,"%2.2X",0);        
    fPrintF(outFile,"\n");
  }
}
---- */




int 
getLineFromFile( FILE *file, char s[], int lim);

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{psToRaster}} 
int GL_GraphicsInterface::
psToRaster(const aString & fileName,
           const aString & ppmFileName )
//----------------------------------------------------------------------
// /Description:
// Convert a RLE compressed .ps file from PlotStuff into a ppm raster 
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{

  FILE *file;
  file=fopen(fileName.c_str(),"r" );

  char buff[180];
  aString line;

  bool done=FALSE;
  int numberOfCharsRead=1;
  while( !done && numberOfCharsRead>0  )
  {
    numberOfCharsRead=getLineFromFile(file,buff,sizeof(buff));
    line=buff;
#ifndef NO_APP
    if( line(0,18)=="[/Indexed/DeviceRGB" )
#else
    if( line.substr(0,19)=="[/Indexed/DeviceRGB" )
#endif
    {
      // printf("colour table found...\n");
      done=TRUE;
    }
  }
  if( !done )
  {
    printf("ERROR reading the file %s, this is probably not a plotStuff generated ps file in compressed RLE format\n",
	   (const char *)fileName.c_str());
    return 1;
  }
  
  const int ctSize=256;
  short ct[ctSize][3];

  int i,r,g,b;
  fgetc(file); // ">"
  // ch=fgetc(file); // ">"
  //  printf(" first char = %c\n",ch);
  for( i=0; i<ctSize; i++ )
  {
    fscanf(file,"%2X%2X%2X",&r,&g,&b);
    ct[i][0]=r; ct[i][1]=g; ct[i][2]=b;
    // printf(" ct[%i]=(%2.2X,%2.2X,%2.2X) \n",i,ct[i][0],ct[i][1],ct[i][2]);
  }
  
  int width,height;
  done=FALSE;
  numberOfCharsRead=1;
  while( !done   )
  {
    numberOfCharsRead=getLineFromFile(file,buff,sizeof(buff));
    line=buff;
#ifndef NO_APP
    if( line(0,5)=="/Width" )
#else
    if( line.substr(0,6)=="/Width" )
#endif
    {
      sScanF(line,"/Width %i",&width);
      // printf("width=%i\n",width);
      numberOfCharsRead=getLineFromFile(file,buff,sizeof(buff));
      sscanf(buff,"/Height %i",&height);
      // printf("height=%i\n",height);
      done=TRUE;
    }
  }

  done=FALSE;
  numberOfCharsRead=1;
  while( !done && numberOfCharsRead>0  )
  {
    numberOfCharsRead=getLineFromFile(file,buff,sizeof(buff));
    line=buff;
#ifndef NO_APP
    if( line(0,4)=="image" )
#else
    if( line.substr(0,5)=="image" )
#endif
    {
      // printf("image found...\n");
      done=TRUE;
    }
  }
  // Now read in the RLE compressed data and uncompress it.
  int count,index;
  done=FALSE;
  int j;
  i=0;

  GLubyte *ubuff = new GLubyte[width*height*4+1000];  // save (r,g,b,a); a is not used
  for(;;)
  {
    fscanf(file,"%2X",&count);
    if( count==128 )
      break;         // this marks EOF
    if( count>128 && count<257)
    {
      count=257-count;
      // we have a run of "count" similar characters
      fscanf(file,"%2X",&index);
      assert( index<ctSize );
      // printf("i=%i, (count,index)=(%i,%i)\n",i,count,index);
      // repeat count
      r=ct[index][0];
      g=ct[index][1];
      b=ct[index][2];
      for( j=0; j<count; j++ )
      {
	ubuff[i++]=r;
	ubuff[i++]=g;
	ubuff[i++]=b;
	i++;
      }
    }
    else 
    {
      // there are "count" dis-similar characters
      count++;
      for( j=0; j<count; j++ )
      {
        fscanf(file,"%2X",&index);
        // printf("i=%i, (count,index)*=(%i,%i)\n",i,count,index);
        assert( index<ctSize );
	ubuff[i++]=ct[index][0];
	ubuff[i++]=ct[index][1];
	ubuff[i++]=ct[index][2];
	i++;
      }
    }
    if( i>width*height*4 )
      break;
  }
  // printf(" Number of colours read =%i \n",i);
  
  // Now save this raster as a ppm file
  GraphicsParameters::HardCopyType oldHardCopyType=hardCopyType[currentWindow];
  hardCopyType[currentWindow]=GraphicsParameters::ppm;

  saveRasterInAFile(ppmFileName,ubuff,width,height,1);
  
  hardCopyType[currentWindow]=oldHardCopyType;

  delete [] ubuff;
  return 0;
}

