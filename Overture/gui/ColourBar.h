// $Id: ColourBar.h,v 1.1 2004/06/20 16:54:47 henshaw Exp $
//
// The Overture Colour Bar class
//   -- encapsulates the code that used to sit in 
//       GL_GraphicsInterface::drawColourBar
//   -- used by GL_GraphicsInterface: contour, streamLines, plotPoints
//
//
//  **pf
//
#ifndef COLOUR_BAR_H
#define COLOUR_BAR_H

class GL_GraphicsInterface;
class GraphicsParameters;
class CBGraphicsParameters; // AP: This doesn't seem to be used???

class ColourBar
{
public:
  enum ColourBarPosition 
  {
    leftColourBar  = 0,
    rightColourBar,
    topColourBar,
    bottomColourBar,
    userDefinedColourBar,
    useOldColourBar, //default, while debugging the new ColourBar
    customColourBar1,
    customColourBar2,
    customColourBar3,
    numberOfColourBarPositions
  };
  enum ColourBarLabelOption
  {
    colourBarLabelsOff=0, 
    colourBarOnlyEndLabels,
    colourBarLabelsOn
  };


  ColourBar(GL_GraphicsInterface*  gi      = NULL,
	    GraphicsParameters*    gparams = NULL );
  ~ColourBar();
  
  void setGraphicsInterface(GL_GraphicsInterface *gi);
  void setGraphicsParameters(GraphicsParameters  *parameters);

  void draw(const int & numberOfContourLevels,
		     RealArray & contourLevels,
		     GUITypes::real uMin,
		     GUITypes::real uMax);

  void update( );

  //this next function is called by the GL_GraphicsInterface::draw
  void positionInWindow(GUITypes::real leftSide, GUITypes::real rightSide, GUITypes::real bottom, GUITypes::real top);
  


private:


  GL_GraphicsInterface* _gi;
  GraphicsParameters*   _gparameters;

  //..make copy & operator= private --> not accessible
  ColourBar( const ColourBar &X);
  ColourBar& operator=( const ColourBar &x);

  //..parameters
  GUITypes::real pi;

  ColourBarPosition colourBarPosition;
  GUITypes::real colourBarWidth;
  GUITypes::real colourBarLength;

  GUITypes::real colourBarCenter[2];
  
  GUITypes::real colourBarAngle;
  GUITypes::real colourBarCurvature;
  GUITypes::real colourBarOffsetFromPlot;

  ColourBarLabelOption colourBarLabelOption;
  bool colourBarLabelOnRight;
  GUITypes::real colourBarLabelAngle;
  int colourBarRelativeAngle;
  GUITypes::real colourBarLabelNormalOffset;
  GUITypes::real colourBarLabelTangentialOffset;
  int colourBarNumberOfIntervals;
  int colourBarMaximumNumberOfLabels;
  GUITypes::real colourBarLabelScaling;
  int colourBarThickLineInterval;



  //..draw parameters, set by setupDraw();
  bool isHorizontalBar; 
  bool horizontalLabels; 
  int currentWindow;
  GUITypes::real leftSide, rightSide, top, bottom;
  GUITypes::real xLeft, xRight, yTop, yBottom;

  GUITypes::real uMin, uMax;
  int numberOfIntervals;
  int numberOfContourLevels;
  RealArray* pContourLevels;

  GUITypes::real barBase;   // base coordinate (yBottom for VERT, xLeft for HORIZ)
  GUITypes::real barLength;
//  GUITypes::real barStep;   // increments from barBase, to get barLength in numberOfIntervals

  //..label data
  int skipLabel;
  GUITypes::real labelSize;
  int labelDrawFlag;

  //..line widths
  GUITypes::real lineWidthScaleFactor;
  GUITypes::real sizeLineWidth       ;
  GUITypes::real sizeMinorContourWidth;
  GUITypes::real sizeMajorContourWidth;
  GUITypes::real sizeAxisNumberSize;

  //..colour bar user data & convenience parameters
  enum LabelOrientation {labelHorizontal=0, labelVertical, labelCustomOrientation};
  LabelOrientation  labelOrientation;


  //
  //..utilities ---------------------------------------------------------------------
  //
  bool preCheck();
  void setupDraw(    const int & numberOfContourLevels_,  RealArray & contourLevels_,
		     GUITypes::real uMin,  GUITypes::real uMax);
  void drawBar();
  void drawBarLines();
  void drawLabels();

  void showAdvancedDialog(const aString &prefix, GUIState &advancedGUI);

  void setLabelOrientation( int labelOrientation_  );
  int  getLabelOrientation( int &labelOrientation_ );

//  bool useOldColourBar();

  //utility for drawColourBar **pf
  static inline GUITypes::real 
  computeBarLabelOffset( const int & numberOfContourLevels,
			 RealArray & contourLevels,
			 GUITypes::real  uMin, GUITypes::real  uMax,
			 GUITypes::real  barLength, int i, GUITypes::real  indexOffset =0.  );
  
  void   barLabel( GUITypes::real value, GUITypes::real  xPos, GUITypes::real  yPos, GUITypes::real  angle);

  void   computeBarQuad( GUITypes::real  r, GUITypes::real  &xa, GUITypes::real  &ya, GUITypes::real  &xb, GUITypes::real  &yb);
  void   computeBarLevelLine( GUITypes::real  r, 
			      GUITypes::real  &xa, GUITypes::real  &ya, GUITypes::real  &xb, GUITypes::real  &yb);
  void   getCenterLine( GUITypes::real  r, GUITypes::real  &x, GUITypes::real  &y,  GUITypes::real  &nx, GUITypes::real  &ny, 
			GUITypes::real  &tx, GUITypes::real  &ty);

  GUITypes::real   computeContourLineWidth( int i );

  void   rotateAndTranslate( GUITypes::real angle, GUITypes::real x0, GUITypes::real y0, GUITypes::real &x, GUITypes::real &y);
  void   computeLabelPosition( int i, GUITypes::real & alpha, GUITypes::real &xPos, GUITypes::real & yPos, GUITypes::real&labelAngle);
  void   stretchToScreenCoordinates( GUITypes::real &xa, GUITypes::real &ya);
  GUITypes::real   getAngle( GUITypes::real x, GUITypes::real y);

  GUITypes::real   minimumWindowLength(GUITypes::real  left,GUITypes::real  right,GUITypes::real  top,GUITypes::real  bottom);
  GUITypes::real    maximumWindowLength(GUITypes::real  left,GUITypes::real  right,GUITypes::real  top,GUITypes::real  bottom);

  //..old versions
public:
  //void draw_old(const int & numberOfContourLevels_,
  //   RealArray & contourLevels_,
  //   real uMin_,
  //   real uMax_);
  //void update_old();

protected: //
  //void setupDraw_old(    const int & numberOfContourLevels_,  RealArray & contourLevels_,
//		     real uMin,  real uMax);
 // void drawBar_old();
  //void drawBarLines_old();
  //void drawLabels_old();

  //void showAdvancedDialog_old(const aString &prefix );
  //..junk, but keeping around just in case
  void update_junk();

};

#endif
