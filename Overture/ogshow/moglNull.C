#include <stdlib.h>
#include "mogl.h"

void moglDisplay(){};

void moglGetWindowSize( int & width, int & height ){};

void moglInit(int & argc, char *argv[], char *fileMenuItems[]){};

int moglGetMenuItem( char *menu[], char* &answer, char *prompt ){ return 0; };

// Display the screen the next time the event loop is entered
void moglPostDisplay(){};

// Define the function that will display the screen
void moglSetDisplayFunction( MOGL_DISPLAY_FUNCTION func ){};

// define the function that will be called when the rubber-band box
// is used to zoom in
void moglSetZoomFunction( MOGL_ZOOM_FUNCTION zoom ){};

