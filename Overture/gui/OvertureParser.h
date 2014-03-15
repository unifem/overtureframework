#ifndef NO_APP
#include "aString.H"
#else
#include "GUITypes.h"
#ifndef aString
#define aString std::string
#endif
#endif
//===============================================================================================
//  Class for parsing commands using perl
//===============================================================================================

class OvertureParser
{
public:
OvertureParser(int argc=0, char **argv=0);
~OvertureParser();

int parse(aString & answer);

static int debug;  // set to a positive value to get debug info

private:
void *parserPointer; // make this a void pointer to avoid exposing other Overture code to the perl include files.

};
