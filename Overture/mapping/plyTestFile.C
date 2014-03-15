/*

Sample code showing how to read and write PLY polygon files.

Based on code by <Greg Turk, March 1994>
Overture version, Petri Fast, 2001

*/

#include <stdio.h>
#include <iostream.h>
#include <string.h>
#include <math.h>
#include "Overture.h"

#include "ply.h"
#include "plyFileInterface.h"

int
main(int argc, char **argv)
{

  PlyFileInterface plyReader;
  aString fileName = "test.ply";
  if (argc>1) fileName = argv[1];
  
  intArray elems;
  intArray tags;
  realArray xyz;
  int nnode, nelem, ddim, rdim;
 
  plyReader.openFile( fileName );
  plyReader.readFile( elems, tags, xyz, nnode, nelem, ddim, rdim);
  plyReader.closeFile();
		     
}
