#include "OvertureDefine.h"
#ifdef OV_USE_X11

/* Copyright (c) Mark J. Kilgard, 1996. */

/* This program is freely distributable without licensing fees 
   and is provided without guarantee or warrantee expressed or 
   implied. This program is -not- in the public domain. */

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>

/* Transparent type values */
/*      None                  0 */
#define TransparentPixel      1
#define TransparentMask       2

/* layered visual info template flags */
#define VisualLayerMask		0x200
#define VisualTransparentType	0x400
#define VisualTransparentValue	0x800
#define VisualAllLayerMask	0xFFF

/* layered visual info structure */
typedef struct _sovVisualInfo {
   XVisualInfo vinfo;
   int layer;
   int type;
   unsigned long value;
} sovVisualInfo;

/* SERVER_OVERLAY_VISUALS property element */
typedef struct _sovOverlayInfo {
   long  overlay_visual;
   long  transparent_type;
   long  value;
   long  layer;
} sovOverlayInfo;

extern sovVisualInfo *sovGetVisualInfo(
  Display *display,
  long lvinfo_mask,
  sovVisualInfo *lvinfo_template,
  int *nitems_return);
extern Status sovMatchVisualInfo(
  Display *display,
  int screen,
  int depth,
  int class,
  int layer,
  sovVisualInfo *lvinfo_return);

static Bool layersRead;
static Atom overlayVisualsAtom;
static sovOverlayInfo **overlayInfoPerScreen;
static int *numOverlaysPerScreen;

sovVisualInfo *
sovGetVisualInfo(Display *display, long lvinfo_mask,
  sovVisualInfo *lvinfo_template, int *nitems_return)
{
  XVisualInfo *vinfo;
  sovVisualInfo *layerInfo;
  Window root;
  Status status;
  Atom actualType;
  unsigned long sizeData, bytesLeft;
  int actualFormat, numVisuals, numScreens, count, i, j;

  vinfo = XGetVisualInfo(display, lvinfo_mask & VisualAllMask,
    &lvinfo_template->vinfo, nitems_return);
  if (vinfo == NULL)
    return NULL;
  numVisuals = *nitems_return;
  if (layersRead == False) {
    overlayVisualsAtom = XInternAtom(display, 
      "SERVER_OVERLAY_VISUALS", True);
    if (overlayVisualsAtom != None) {
      numScreens = ScreenCount(display);
      overlayInfoPerScreen = (sovOverlayInfo **)
        malloc(numScreens * sizeof(sovOverlayInfo *));
      numOverlaysPerScreen = (int *) malloc(numScreens * sizeof(int));
      if (overlayInfoPerScreen != NULL &&
        numOverlaysPerScreen != NULL) {
        for (i = 0; i < numScreens; i++) {
          root = RootWindow(display, i);
          status = XGetWindowProperty(display, root, overlayVisualsAtom,
            0L, (long) 10000, False, overlayVisualsAtom,
	    &actualType, &actualFormat,
            &sizeData, &bytesLeft,
	    (unsigned char **) &overlayInfoPerScreen[i]);
/* 000718	    (char **) &overlayInfoPerScreen[i]); */
          if (status != Success ||
	    actualType != overlayVisualsAtom ||
            actualFormat != 32 || sizeData < 4)
            numOverlaysPerScreen[i] = 0;
          else
            numOverlaysPerScreen[i] =
	      sizeData / (sizeof(sovOverlayInfo) / 4);
        }
        layersRead = True;
      } else {
        if (overlayInfoPerScreen != NULL)
          free(overlayInfoPerScreen);
        if (numOverlaysPerScreen != NULL)
          free(numOverlaysPerScreen);
      }
    }
  }
  layerInfo = (sovVisualInfo *)
    malloc(numVisuals * sizeof(sovVisualInfo));
  if (layerInfo == NULL) {
    XFree(vinfo);
    return NULL;
  }
  count = 0;
  for (i = 0; i < numVisuals; i++) {
    XVisualInfo *pVinfo;
    int screen;
    sovOverlayInfo *overlayInfo;

    pVinfo = &vinfo[i];
    screen = pVinfo->screen;
    overlayInfo = NULL;
    if (layersRead) {
      for (j = 0; j < numOverlaysPerScreen[screen]; j++)
        if (pVinfo->visualid == 
  	  overlayInfoPerScreen[screen][j].overlay_visual) {
          overlayInfo = &overlayInfoPerScreen[screen][j];
          break;
        }
    }
    if (lvinfo_mask & VisualLayerMask)
      if (overlayInfo == NULL) {
        if (lvinfo_template->layer != 0)
          continue;
      } else if (lvinfo_template->layer != overlayInfo->layer)
        continue;
    if (lvinfo_mask & VisualTransparentType)
      if (overlayInfo == NULL) {
        if (lvinfo_template->type != None)
          continue;
      } else if (lvinfo_template->type !=
        overlayInfo->transparent_type)
        continue;
    if (lvinfo_mask & VisualTransparentValue)
      if (overlayInfo == NULL)
        /* non-overlay visuals have no sense of
           TransparentValue */
        continue;
      else if (lvinfo_template->value != overlayInfo->value)
        continue;
    layerInfo[count].vinfo = *pVinfo;
    if (overlayInfo == NULL) {
      layerInfo[count].layer = 0;
      layerInfo[count].type = None;
      layerInfo[count].value = 0;  /* meaningless */
    } else {
      layerInfo[count].layer = overlayInfo->layer;
      layerInfo[count].type = overlayInfo->transparent_type;
      layerInfo[count].value = overlayInfo->value;
    }
    count++;
  }
  XFree(vinfo);
  *nitems_return = count;
  if (count == 0) {
    XFree(layerInfo);
    return NULL;
  } else
    return layerInfo;
}

Status
sovMatchVisualInfo(Display *display, int screen,
  int depth, int class, int layer, sovVisualInfo *lvinfo_return)
{
  sovVisualInfo *lvinfo;
  sovVisualInfo lvinfoTemplate;
  int nitems;

  lvinfoTemplate.vinfo.screen = screen;
  lvinfoTemplate.vinfo.depth = depth;
  lvinfoTemplate.vinfo.class = class;
  lvinfoTemplate.layer = layer;
  lvinfo = sovGetVisualInfo(display,
    VisualScreenMask|VisualDepthMask|VisualClassMask|VisualLayerMask,
    &lvinfoTemplate, &nitems);
  if (lvinfo != NULL && nitems > 0) {
    *lvinfo_return = *lvinfo;
    return 1;
  } else
    return 0;
}


/* the next routine was changed by Bill Henshaw */

void detectOverlaySupport(Display *dpy, Visual *overlayVisual, int *overlayDepth, Colormap *overlayColormap)
{

  sovVisualInfo template, *overlayVisuals;
  int layer, nVisuals, i, entries;

  /* Need more than two colormap entries for reasonable menus. */
  entries = 2;
  for (layer = 1; layer <= 3; layer++) {
    template.layer = layer;
    template.vinfo.screen = DefaultScreen(dpy);
    overlayVisuals = sovGetVisualInfo(dpy,
      VisualScreenMask | VisualLayerMask, &template, &nVisuals);
    if (overlayVisuals) {
      for (i = 0; i < nVisuals; i++) {
        if (overlayVisuals[i].vinfo.visual->map_entries > entries) {
          overlayVisual = overlayVisuals[i].vinfo.visual;
          *overlayDepth = overlayVisuals[i].vinfo.depth;
	  entries = overlayVisual->map_entries;
        }
      }
      XFree(overlayVisuals);
    }
  }
  if (overlayVisual)
  {
    /* printf("**** info: overlay visual found *****\n"); */
    *overlayColormap = XCreateColormap(dpy, DefaultRootWindow(dpy),overlayVisual, AllocNone); /* wdh */
  }
  
}

#endif // OV_USE_X11
