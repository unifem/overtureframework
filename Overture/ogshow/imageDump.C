/* OpenGL image dump, written by Reto Koradi (kor@spectrospin.ch) */

/* This file contains code for doing OpenGL off-screen rendering and
   saving the result in a TIFF file. It requires Sam Leffler's libtiff
   library which is available from ftp.sgi.com.
   The code is used by calling the function StartDump(..), drawing the
   scene, and then calling EndDump(..).
   Please note that StartDump creates a new context, so all attributes
   stored in the current context (colors, lighting parameters, etc.)
   have to be set again beforing performing the actual redraw. This
   can be rather painful, but unfortunately GLX does not allow
   sharing/copying of attributes between direct and nondirect
   rendering contexts. */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include <tiffio.h>

/* X servers often grow bigger and bigger when allocating/freeing
   many pixmaps, so it's better to keep and reuse them if possible.
   Set this to 0 if you don't want to use that. */
#define KEEP_PIXMAP 1

static FILE *TiffFileP;
static int Orient;
static int ImgW, ImgH;
static Bool OutOfMemory;
static Display *Dpy;
static Pixmap XPix = 0;
static GLXPixmap GPix = 0;
static GLXContext OldCtx, Ctx;
static float OldVpX, OldVpY, OldVpW, OldVpH;

static void
destroyPixmap(void)
{
  glXDestroyGLXPixmap(Dpy, GPix);
  GPix = 0;
  XFreePixmap(Dpy, XPix);
  XPix = 0;
}

static int
xErrorHandler(Display *dpy, XErrorEvent *evtP)
{
  OutOfMemory = True;
  return 0;
}

int
StartDump(char *fileName, int orient, int w, int h)
/* Prepare for image dump. fileName is the name of the file the image
   will be written to. If orient is 0, the image is written in the
   normal orientation, if it is 1, it will be rotated by 90 degrees.
   w and h give the width and height (in pixels) of the desired image.
   Returns 0 on success, calls RaiseError(..) and returns 1 on error. */
{
  Widget drawW = GetDrawW();  /* the GLwMDrawA widget used */
  XErrorHandler oldHandler;
  int attrList[10];
  XVisualInfo *visP;
  int n, i;

  TiffFileP = fopen(fileName, "w");
  if (TiffFileP == NULL) {
    RaiseError("could not open output file");
    return 1;
  }

#if KEEP_PIXMAP
  if (GPix != 0 && (w != ImgW || h != ImgH))
    destroyPixmap();
#endif

  Orient = orient;
  ImgW = w;
  ImgH = h;

  Dpy = XtDisplay(drawW);

  n = 0;
  attrList[n++] = GLX_RGBA;
  attrList[n++] = GLX_RED_SIZE; attrList[n++] = 8;
  attrList[n++] = GLX_GREEN_SIZE; attrList[n++] = 8;
  attrList[n++] = GLX_BLUE_SIZE; attrList[n++] = 8;
  attrList[n++] = GLX_DEPTH_SIZE; attrList[n++] = 1;
  attrList[n++] = None;
  visP = glXChooseVisual(Dpy,
      XScreenNumberOfScreen(XtScreen(drawW)), attrList);
  if (visP == NULL) {
    RaiseError("no 24-bit true color visual available");
    return 1;
  }

  /* catch BadAlloc error */
  OutOfMemory = False;
  oldHandler = XSetErrorHandler(xErrorHandler);

  if (XPix == 0) {
    XPix = XCreatePixmap(Dpy, XtWindow(drawW), w, h, 24);
    XSync(Dpy, False);  /* error comes too late otherwise */
    if (OutOfMemory) {
      XPix = 0;
      XSetErrorHandler(oldHandler);
      RaiseError("could not allocate Pixmap");
      return 1;
    }
  }

  if (GPix == 0) {
    GPix = glXCreateGLXPixmap(Dpy, visP, XPix);
    XSync(Dpy, False);
    XSetErrorHandler(oldHandler);
    if (OutOfMemory) {
      GPix = 0;
      XFreePixmap(Dpy, XPix);
      XPix = 0;
      RaiseError("could not allocate Pixmap");
      return 1;
    }
  }

  Ctx = glXCreateContext(Dpy, visP, NULL, False);
  if (Ctx == NULL) {
    destroyPixmap();
    RaiseError("could not create rendering context");
    return 1;
  }

  OldCtx = glXGetCurrentContext();
  (void) glXMakeCurrent(Dpy, GPix, Ctx);

  return 0;
}

static int
writeTiff(void)
{
  TIFF *tif;
  int tiffW, tiffH;
  int bufSize, rowI;
  unsigned char *buf;
  int res;

  tif = TIFFFdOpen(fileno(TiffFileP), "output file", "w");
  if (tif == NULL) {
    RaiseError("could not create TIFF file");
    return 1;
  }

  if (Orient == 0) {
    tiffW = ImgW;
    tiffH = ImgH;
    bufSize = 4 * ((3 * tiffW + 3) / 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
  } else {
    tiffW = ImgH;
    tiffH = ImgW;
    bufSize = 3 * tiffW;
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
  }

  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, tiffW);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, tiffH);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
  TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
  TIFFSetField(tif, TIFFTAG_DOCUMENTNAME, "My Name");
  TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, "My Description");
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
  TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, (8 * 1024) / (3 * tiffW));
  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

  buf = malloc(bufSize * sizeof(*buf));

  res = 0;
  for (rowI = 0; rowI < tiffH; rowI++) {
    if (Orient == 0)
      glReadPixels(0, ImgH - 1 - rowI, ImgW, 1,
         GL_RGB, GL_UNSIGNED_BYTE, buf);
    else
      glReadPixels(rowI, 0, 1, ImgH,
         GL_RGB, GL_UNSIGNED_BYTE, buf);

    if (TIFFWriteScanline(tif, buf, rowI, 0) < 0) {
      RaiseError("error while writing TIFF file");
      res = 1;
      break;
    }
  }

  free(buf);

  TIFFFlushData(tif);
  TIFFClose(tif);

  return res;
}

int
EndDump(void)
/* Write current image to file. May only be called after StartDump(..).
   Returns 0 on success, calls RaiseError(..) and returns 1 on error. */
{
  int res;

  res = writeTiff();
  (void) fclose(TiffFileP);

  (void) glXMakeCurrent(Dpy, XtWindow(GetDrawW()), OldCtx);

#if KEEP_PIXMAP
#else
  destroyPixmap();
#endif

  glXDestroyContext(Dpy, Ctx);

  return res;
}
