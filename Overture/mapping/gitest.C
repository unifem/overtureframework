#include "GraphicsInterface.h"

void main() {
    GraphicsInterface gi; char _[1000];

//  gi.openPhigs(); // Optional.
    
    gi.openWorkstation();

    gi.setCurrentWorkstation(GraphicsInterface::activateIO);
    gi.setCurrentWorkstation(GraphicsInterface::activateGraphics);

    gi.outputString("Hello.");

    const int view = 1;
    gi.specifyViewParameters(view);

    const aString menu[] = { "CONTINUE", "OK", "" };
    aString answer; Float x[3];
    gi.inputStringAndCursor(view, menu, "Hello?", answer, x);

    gi.outputString(answer);

    gi.outputString(sPrintF(_, "x = (%f,%f,%f)", x[0], x[1], x[2]));

    gi.inputString(menu, "Done?", answer);

    gi.writeOutDataStructure();

    gi.closeWorkstation();
    
//  gi.closePhigs(); // Optional.
}
