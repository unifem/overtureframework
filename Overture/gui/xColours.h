#ifndef X_COLOURS_H
#define X_COLOURS_H

// prototypes for functions in xColour.C
void 
setXColour( const aString & xColourName );

void
setXColour( int index );

int 
getXColour( const aString & xColourName, RealArray & rgb);

int 
getXColour( const aString & xColourName, GUITypes::real *rgb);

int
getXColour( const aString & xColourName );

const aString & 
getXColour( int index );

const aString*
getAllColourNames();

#endif
