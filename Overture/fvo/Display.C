#include "Display.h"
#include "loops.h" 

#define PRINT_AS_REAL 0
#define PRINT_AS_INT  1

bool Display::allDisplayOff      = FALSE;
bool Display::interactiveDisplay = TRUE;
bool Display::cellCenteredDisplayOption = LogicalFalse;

//======================================================================================================= 
//
//
	void DisplayControl (bool &allOff, bool &thisOff, bool &interactiveDisplay)
//
//
// This routine used by all those below to control the output
//
//======================================================================================================= 
{
  aString answer;
  char buf[1000];
  for (;;)
  {
    cout << "Command: " ;
    // *wdh* 001012 gets (buf);
    getLine(buf,1000);
    answer = buf;
    if (answer(0,0)=="h")
    {
      printf ("DISPLAY Help: \n");
      printf ("\n");
      printf ("<cr>:          continue\n");
      printf ("o[ff]          turn off all output\n");
      printf ("t[hisoff]      turn off output only for this routine\n");
      printf ("i[Off] 	      turn off interactive display\n");
      printf ("h[elp]         print this message\n");
    }
    else if (answer(0,0)=="o")
    {
      allOff = TRUE;
      break;
    }
    else if (answer(0,0)=="t")
    {
      thisOff = TRUE;
      break;
    }
    else if (answer(0,0)=="i")
    {
      interactiveDisplay = FALSE;
      break;
    }
    else if (answer(0,0)=="\0")
    {
      break;
    }
  }
}
//======================================================================================================= 
//
//
	Display::Display () 
//
//
// Default Constructor
//
//======================================================================================================= 
{ 
  thisDisplayOff = FALSE;
  interactiveInitializationCalled = FALSE;
}
//======================================================================================================= 
//
//
	Display::~Display () {}
//
//======================================================================================================= 
//======================================================================================================= 
	void Display::interactivelySetInteractiveDisplay (const aString &label)
{
  bool allOff = Display::allDisplayOff;
  if (interactiveInitializationCalled) return;
  if (allOff) return;
  printf ("\n===============================================================================\n");
  printf ("%s", (const char*)label);
  printf ("\n===============================================================================\n");
  interactivelySetInteractiveDisplay ();
}

//======================================================================================================= 
	void Display::interactivelySetInteractiveDisplay ()

	//========================================
	// Author:		D.L.Brown
	// Date Created:	
	// Date Modified:	
	//
	// Purpose:
	//		Turn display on or off without actually
	//		displaying anything in the process
	//
	// Interface: (inputs)
	//
	// Interface: (output)
	//
	// Status and Warnings:
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================

{
  bool allOff = Display::allDisplayOff;
  if (interactiveInitializationCalled) return;
  interactiveInitializationCalled = TRUE;
  if (allOff) return;
  if (interactiveDisplay){ 
    bool allOff = Display::allDisplayOff;
    DisplayControl (allOff, thisDisplayOff, interactiveDisplay);
    Display::allDisplayOff = allOff;
  }
}

//======================================================================================================= 

//======================================================================================================= 
  	void Display::display (const floatGridCollectionFunction &cgf,	const aString &label)
	//========================================
	// Author:		D.L.Brown
	// Date Created:	
	// Date Modified:	
	//
	// Purpose:
	//	Display a floatGridCollectionFunction with label
	//	actual formatted output is done in another routine.
	//	This just prints the label and calls the output routine.
	// Interface: (inputs)
	//
	// Interface: (output)
	//
	// Status and Warnings:
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================
//======================================================================================================= 
{
  bool allOff = Display::allDisplayOff;

  if (allOff || thisDisplayOff ) return;
	// ========================================
	// Loop over the grids and call display( floatMappedGridFunction& )
	// ========================================

//  int numberOfGrids = cgf.numberOfComponentGrids();
  int numberOfGrids = cgf.numberOfGrids();
  
  int grid;
  for (grid=0; grid < numberOfGrids; grid++)
  {
    floatMappedGridFunction &mgf = cgf[grid];
    printf ("\n      GRID %d \n", grid);
    display (mgf);
  }

	// ========================================
	// Print the label
	// ========================================

    printf ("\n===============================================================================\n");
    printf ("%s\n", (const char*)label);
    printf ("===============================================================================\n");


	// ========================================
	// Optional Interactive display control
	// ========================================

  if (interactiveDisplay){ 
    DisplayControl (allOff, thisDisplayOff, interactiveDisplay);
    Display::allDisplayOff = allOff;
  }
}
//======================================================================================================= 
  	void Display::display (const doubleGridCollectionFunction &cgf,	const aString &label)
	//========================================
	// Author:		D.L.Brown
	// Date Created:	
	// Date Modified:	
	//
	// Purpose:
	//	Display a doubleGridCollectionFunction with label
	//	actual formatted output is done in another routine.
	//	This just prints the label and calls the output routine.
	// Interface: (inputs)
	//
	// Interface: (output)
	//
	// Status and Warnings:
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================
//======================================================================================================= 
{
  bool allOff = Display::allDisplayOff;

  if (allOff || thisDisplayOff ) return;
	// ========================================
	// Loop over the grids and call display( doubleMappedGridFunction& )
	// ========================================

//  int numberOfGrids = cgf.numberOfComponentGrids();
  int numberOfGrids = cgf.numberOfGrids();
  
  int grid;
  for (grid=0; grid < numberOfGrids; grid++)
  {
    doubleMappedGridFunction &mgf = cgf[grid];
    printf ("\n      GRID %d \n", grid);
    display (mgf);
  }

	// ========================================
	// Print the label
	// ========================================

    printf ("\n===============================================================================\n");
    printf ("%s\n", (const char*)label);
    printf ("===============================================================================\n");


	// ========================================
	// Optional Interactive display control
	// ========================================

  if (interactiveDisplay){ 
    DisplayControl (allOff, thisDisplayOff, interactiveDisplay);
    Display::allDisplayOff = allOff;
  }
}
//======================================================================================================= 
	void Display::display (const intGridCollectionFunction &cgf,	const aString &label)
	//========================================
	// Author:		D.L.Brown
	// Date Created:	
	// Date Modified:	
	//
	// Purpose:
	//	display an intGridCollectionFunction nicely. Actual formatted output done
	//	in another routine. This just prints the label and calls the output routine.
	// Interface: (inputs)
	//
	// Interface: (output)
	//
	// Status and Warnings:
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================
//======================================================================================================= 
{
  bool allOff = Display::allDisplayOff;
  if (allOff || thisDisplayOff ) return;

	// ========================================
	// Loop over the grids and call display( intMappedGridFunction& )
	// ========================================
//  int numberOfGrids = cgf.numberOfComponentGrids();
  int numberOfGrids = cgf.numberOfGrids();

  int grid;
  for (grid=0; grid < numberOfGrids; grid++)
  {
    intMappedGridFunction &mgf = cgf[grid];
    printf ("\n      GRID %d \n", grid);
    display (mgf);
  }

	// ========================================
	// Print the label
	// ========================================

    printf ("\n===============================================================================\n");
    printf ("%s\n", (const char*)label);
    printf ("===============================================================================\n");


	// ========================================
	// Optional Interactive display control
	// ========================================

  if (interactiveDisplay){ 
    DisplayControl (allOff, thisDisplayOff, interactiveDisplay);
    Display::allDisplayOff = allOff;
  }
}
//======================================================================================================= 
	void Display::display (const floatMappedGridFunction &mgf,	const aString &label)
//
	//
	//========================================
	// Author:		D.L.Brown
	// Date Created:	
	// Date Modified:	
	//
	// Purpose:
	//   Display function for floatMappedGridFunction's with control and label
	//   This routine prints the label and calls another routine to do the
	//   actual formatted write.
	//
	// Interface: (inputs)
	//
	// Interface: (output)
	//
	// Status and Warnings:
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================
	//
//======================================================================================================= 
{
  bool allOff = Display::allDisplayOff;
  if (allOff || thisDisplayOff ) return;

	// ========================================
	// Display
	// ========================================

  display (mgf);

	// ========================================
	// print label
	// ========================================

    printf ("\n===============================================================================\n");
  printf ("%s\n", (const char*)label);
    printf ("===============================================================================\n");

	// ========================================
	// Optional Interactive display control
	// ========================================

  if (interactiveDisplay){
    DisplayControl (allOff, thisDisplayOff, interactiveDisplay);
    Display::allDisplayOff = allOff;
  }
}
//======================================================================================================= 
	void Display::display (const doubleMappedGridFunction &mgf,	const aString &label)
//
	//
	//========================================
	// Author:		D.L.Brown
	// Date Created:	
	// Date Modified:	
	//
	// Purpose:
	//   Display function for doubleMappedGridFunction's with control and label
	//   This routine prints the label and calls another routine to do the
	//   actual formatted write.
	//
	// Interface: (inputs)
	//
	// Interface: (output)
	//
	// Status and Warnings:
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================
	//
//======================================================================================================= 
{
  bool allOff = Display::allDisplayOff;
  if (allOff || thisDisplayOff ) return;

	// ========================================
	// Display
	// ========================================

  display (mgf);

	// ========================================
	// print label
	// ========================================

    printf ("\n===============================================================================\n");
  printf ("%s\n", (const char*)label);
    printf ("===============================================================================\n");

	// ========================================
	// Optional Interactive display control
	// ========================================

  if (interactiveDisplay){
    DisplayControl (allOff, thisDisplayOff, interactiveDisplay);
    Display::allDisplayOff = allOff;
  }
}

//======================================================================================================= 
//
//
//	void Display::display (const intMappedGridFunction &mgf,	const aString &label, bool &allOff )
	void Display::display (const intMappedGridFunction &mgf,	const aString &label)
//
		//
	//========================================
	// Author:		D.L.Brown
	// Date Created:	9xxxxx
	// Date Modified:	9xxxxx
	//
	// Purpose:
	// 	Display function for intMappedGridFunctions with control and label.
	//	This routine prints the label and calls another routine to do
	//	the formatted output.
        //
	// Interface: (inputs)
	//
	// Interface: (output)
	//
	// Status and Warnings:
	//  There are no known bugs, nor will there ever be.
	// 
	//========================================
		//
//======================================================================================================= 

{
  bool allOff = Display::allDisplayOff;
  if (allOff || thisDisplayOff ) return;

    display (mgf);

	// ========================================
	// print label
	// ========================================

    printf ("\n===============================================================================\n");
    printf ("%s\n", (const char*)label);
    printf ("===============================================================================\n");

	// ========================================
	// Optional Interactive display control
	// ========================================

  if (interactiveDisplay){
    DisplayControl (allOff, thisDisplayOff, interactiveDisplay);
    Display::allDisplayOff = allOff;
  }

}
//======================================================================================================= 
	void Display::display (const floatArray &mgf,	const aString &label)
//
	//
	// Display function for floatArray's with control and label
	//
//======================================================================================================= 
{
  bool allOff = Display::allDisplayOff;
  if (allOff || thisDisplayOff ) return;

  display (mgf);

	// ========================================
	// print label; display; then label again
	// ========================================

    printf ("\n===============================================================================\n");
  printf ("%s\n", (const char*)label);
    printf ("===============================================================================\n");

	// ========================================
	// Optional Interactive display control
	// ========================================

  if (interactiveDisplay){
    DisplayControl (allOff, thisDisplayOff, interactiveDisplay);
    Display::allDisplayOff = allOff;
  }

}
//======================================================================================================= 
	void Display::display (const doubleArray &mgf,	const aString &label)
//
	//
	// Display function for doubleArray's with control and label
	//
//======================================================================================================= 
{
  bool allOff = Display::allDisplayOff;
  if (allOff || thisDisplayOff ) return;

  display (mgf);

	// ========================================
	// print label; display; then label again
	// ========================================

    printf ("\n===============================================================================\n");
  printf ("%s\n", (const char*)label);
    printf ("===============================================================================\n");

	// ========================================
	// Optional Interactive display control
	// ========================================

  if (interactiveDisplay){
    DisplayControl (allOff, thisDisplayOff, interactiveDisplay);
    Display::allDisplayOff = allOff;
  }

}
//======================================================================================================= 
	void Display::display (const intArray &mgf,	const aString &label)
//
	//
	// Display function for intArray's with control and label
	//
//======================================================================================================= 
{
  bool allOff = Display::allDisplayOff;
  if (allOff || thisDisplayOff ) return;

  display (mgf);

	// ========================================
	// print label
	// ========================================

  printf ("\n===============================================================================\n");
  printf ("%s\n", (const char*)label);
  printf ("===============================================================================\n");

	// ========================================
	// Optional Interactive display control
	// ========================================

  if (interactiveDisplay){
    DisplayControl (allOff, thisDisplayOff, interactiveDisplay);
    Display::allDisplayOff = allOff;
  }

}
//======================================================================================================= 
	void Display::display (const Index &I,	const aString &label)
//
	//
	// Display function for intArray's with control and label
	//
//======================================================================================================= 
{
  bool allOff = Display::allDisplayOff;
  if (allOff || thisDisplayOff ) return;
  display (I);


	// ========================================
	// print label
	// ========================================

  printf ("\n===============================================================================\n");
  printf ("%s\n", (const char*)label);
  printf ("===============================================================================\n");

	// ========================================
	// Optional Interactive display control
	// ========================================

  if (interactiveDisplay){
    DisplayControl (allOff, thisDisplayOff, interactiveDisplay);
    Display::allDisplayOff = allOff;
  }

}
//======================================================================================================= 
	void Display::display (const floatMappedGridFunction &mgf)
//
		//
		// Display function for floatMappedGridFunction's with no control or label
		//
//======================================================================================================= 
{
  bool ccDisplayOption = Display::cellCenteredDisplayOption;
  GridFunctionParameters::GridFunctionType mgfType = mgf.getGridFunctionType();
  MappedGrid * mg = mgf.mappedGrid;
  int numberOfDimensions = 0;
  if (mg != NULL)  numberOfDimensions = mg->numberOfDimensions();
  
    printf ("Type: floatMappedGridFunction\n");

    		 int ndra = mgf.getBase (mgf.positionOfCoordinate(rAxis));
    		 int ndrb = mgf.getBound(mgf.positionOfCoordinate(rAxis));
    		 int ndsa = mgf.getBase (mgf.positionOfCoordinate(sAxis));
    		 int ndsb = mgf.getBound(mgf.positionOfCoordinate(sAxis));
    		 int ndta = mgf.getBase (mgf.positionOfCoordinate(tAxis));
    		 int ndtb = mgf.getBound(mgf.positionOfCoordinate(tAxis));
		 int maxdim = max(abs(ndra),max(abs(ndrb),max(abs(ndsa),max(abs(ndsb),max(abs(ndta),abs(ndtb))))));

		 int mindim = min(ndrb-ndra,min(ndsb-ndsa,ndtb-ndta));

		//
		// 950601: It should be possible to replace all these if statements with
		// a single formatted write to a string which then would be passed
		// to printf. But for now this will have to do.
		//

    if (maxdim>9999) 
      printf ("Dimensions: %5i:%-5i %5i:%-5i %5i:%-5i ", 
    		 mgf.getBase (mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(tAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(tAxis)));

    if (maxdim>999 && maxdim<10000) 
      printf ("Dimensions: %4i:%-4i %4i:%-4i %4i:%-4i ", 
    		 mgf.getBase (mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(tAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(tAxis)));

    if (maxdim>99 && maxdim <1000) 
      printf ("Dimensions: %3i:%-3i %3i:%-3i %3i:%-3i ", 
    		 mgf.getBase (mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(tAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(tAxis)));

    if (maxdim < 100) 
      printf ("Dimensions: %2i:%-2i %2i:%-2i %2i:%-2i ", 
    		 mgf.getBase (mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(tAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(tAxis)));

    int maxcomp = max(abs(mgf.getComponentBase(0)), 
		  max(abs(mgf.getComponentBound(0)),
		  max(abs(mgf.getComponentBase(1)),
		  max(abs(mgf.getComponentBound(1)),
		  max(abs(mgf.getComponentBase(2)), 
		  max(abs(mgf.getComponentBound(2)),
		  max(abs(mgf.getComponentBase(3)), 
		  max(abs(mgf.getComponentBound(3)),
		  max(abs(mgf.getComponentBase(4)), abs(mgf.getComponentBound(4)))))))))));

    if (maxcomp>9999)
      printf ("Components: %5i:%-5i %5i:%-5i %5i:%-5i %5i:%-5i %5i:%-5i ", 
		 mgf.getComponentBase(0), mgf.getComponentBound(0),
		 mgf.getComponentBase(1), mgf.getComponentBound(1),
		 mgf.getComponentBase(2), mgf.getComponentBound(2),
		 mgf.getComponentBase(3), mgf.getComponentBound(3),
		 mgf.getComponentBase(4), mgf.getComponentBound(4));

    if (maxcomp>999 && maxcomp <10000)
      printf ("Components: %4i:%-4i %4i:%-4i %4i:%-4i %4i:%-4i %4i:%-4i ", 
		 mgf.getComponentBase(0), mgf.getComponentBound(0),
		 mgf.getComponentBase(1), mgf.getComponentBound(1),
		 mgf.getComponentBase(2), mgf.getComponentBound(2),
		 mgf.getComponentBase(3), mgf.getComponentBound(3),
		 mgf.getComponentBase(4), mgf.getComponentBound(4));

    if (maxcomp>99 && maxcomp<1000)
      printf ("Components: %3i:%-3i %3i:%-3i %3i:%-3i %3i:%-3i %3i:%-3i ", 
		 mgf.getComponentBase(0), mgf.getComponentBound(0),
		 mgf.getComponentBase(1), mgf.getComponentBound(1),
		 mgf.getComponentBase(2), mgf.getComponentBound(2),
		 mgf.getComponentBase(3), mgf.getComponentBound(3),
		 mgf.getComponentBase(4), mgf.getComponentBound(4));

    if (maxcomp<100)
      printf ("Components: %2i:%-2i %2i:%-2i %2i:%-2i %2i:%-2i %2i:%-2i ", 
		 mgf.getComponentBase(0), mgf.getComponentBound(0),
		 mgf.getComponentBase(1), mgf.getComponentBound(1),
		 mgf.getComponentBase(2), mgf.getComponentBound(2),
		 mgf.getComponentBase(3), mgf.getComponentBound(3),
		 mgf.getComponentBase(4), mgf.getComponentBound(4));

	// compute totalNumberOfComponents

    if (mindim < 0){
      printf ("\n\nTHIS ARRAY APPEARS TO BE EMPTY\n\n\n");
      return;
    }
    int totalNumberOfComponents = 1;
    int ic;
    for (ic = 0; ic < 5; ic++) totalNumberOfComponents *= mgf.getComponentDimension(ic);


      int l[8], m[8], typ[8], lBase[8], lBound[8];

      int countComps;
      for (countComps=0; countComps<5; countComps++){

		// "m" is used to order the loops correctly. the "components" should
		// come first followed by the "coordinates"
		// "typ" is an array that will contain "0" for a coordinate
		// dimension and "1" for a component dimension

	m[countComps+3] = mgf.positionOfComponent(countComps);
	typ[m[countComps+3]] = 1;

		// "lBase" and "lBound" are used to give the base and bound for
		// the loops. 

	lBase [countComps+3] = mgf.getComponentBase(countComps);
	lBound[countComps+3] = mgf.getComponentBound(countComps);
      }

      int countCoords;
      for (countCoords=0; countCoords<3; countCoords++){
	m[countCoords] = mgf.positionOfCoordinate(countCoords);
	typ[m[countCoords]] = 0;
	lBase [countCoords] = mgf.getBase (mgf.positionOfCoordinate(countCoords));
	lBound[countCoords] = mgf.getBound(mgf.positionOfCoordinate(countCoords));
	if (ccDisplayOption && 
	    mgfType == GridFunctionParameters::cellCentered &&
	    countCoords < numberOfDimensions) 
	   lBound[countCoords] -= 1;
      }

      printf ("\n      ");
      int index;
      for (index=0; index<8; index++) if (typ[index] == 0 ){ printf (" x");} else { printf (" c");};
      printf ("\n");

      for (l[m[7]]=lBase[7]; l[m[7]]<=lBound[7]; l[m[7]]++){
	for (l[m[6]]=lBase[6]; l[m[6]]<=lBound[6]; l[m[6]]++){
	  for (l[m[5]]=lBase[5]; l[m[5]]<=lBound[5]; l[m[5]]++){
	    for (l[m[4]]=lBase[4]; l[m[4]]<=lBound[4]; l[m[4]]++){
	      for (l[m[3]]=lBase[3]; l[m[3]]<=lBound[3]; l[m[3]]++){

	      if (totalNumberOfComponents > 1) 
		{
		  printf ("\n   COMPONENT");
		  for (countComps=0; countComps<5; countComps++) printf (" %2i", l[m[3+countComps]]);
		}else{
		  printf("\n");
		}
		printf ("  Centering: ");
		int axis;
		for (axis=0; axis<3; axis++) printf ("%3i", 
		    mgf.getIsCellCentered(axis, l[m[3]], l[m[4]], l[m[5]], l[m[6]], l[m[7]]));
		    //mgf.getIsCellCentered(axis, l[m[3]], l[m[4]], l[m[5]], l[m[6]]));
		printf ("    FaceCentering: ");
		//printf (" Unknown ");
		printf ("%3i", mgf.getFaceCentering());
		printf ("\n\n");

		for (l[m[2]]=lBase[2]; l[m[2]]<=lBound[2]; l[m[2]]++){
		// reverse 2nd index so that "y" is "up"
		  for (l[m[1]]=lBound[1]; l[m[1]]>=lBase[1]; l[m[1]]--){

		    for (l[m[0]]=lBase[0]; l[m[0]]<=lBound[0]; l[m[0]]++){
		      //printf (" %7.3f",mgf(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7]));

		      // 95008:this is a KLUDGE until Bill fixes scalar indexing
		      // and this won't work for >5 indices or unusual orderings

		      printf (" %7.3f",mgf(l[0],l[1],l[2],l[3]+(lBound[3]-lBase[3]+1)*l[4],0,l[5],l[6],l[7]));
		    }
		    printf ("\n");
		  }
		}

      }}}}}

}
//======================================================================================================= 
	void Display::display (const doubleMappedGridFunction &mgf)
//
		//
		// Display function for doubleMappedGridFunction's with no control or label
		//
//======================================================================================================= 
{
  bool ccDisplayOption = Display::cellCenteredDisplayOption;
  GridFunctionParameters::GridFunctionType mgfType = mgf.getGridFunctionType();
  MappedGrid * mg = mgf.mappedGrid;
  int numberOfDimensions = 0;
  if (mg != NULL)  numberOfDimensions = mg->numberOfDimensions();


    printf ("Type: doubleMappedGridFunction\n");

    		 int ndra = mgf.getBase (mgf.positionOfCoordinate(rAxis));
    		 int ndrb = mgf.getBound(mgf.positionOfCoordinate(rAxis));
    		 int ndsa = mgf.getBase (mgf.positionOfCoordinate(sAxis));
    		 int ndsb = mgf.getBound(mgf.positionOfCoordinate(sAxis));
    		 int ndta = mgf.getBase (mgf.positionOfCoordinate(tAxis));
    		 int ndtb = mgf.getBound(mgf.positionOfCoordinate(tAxis));
		 int maxdim = max(abs(ndra),max(abs(ndrb),max(abs(ndsa),max(abs(ndsb),max(abs(ndta),abs(ndtb))))));

		 int mindim = min(ndrb-ndra,min(ndsb-ndsa,ndtb-ndta));

		//
		// 950601: It should be possible to replace all these if statements with
		// a single formatted write to a string which then would be passed
		// to printf. But for now this will have to do.
		//

    if (maxdim>9999) 
      printf ("Dimensions: %5i:%-5i %5i:%-5i %5i:%-5i ", 
    		 mgf.getBase (mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(tAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(tAxis)));

    if (maxdim>999 && maxdim<10000) 
      printf ("Dimensions: %4i:%-4i %4i:%-4i %4i:%-4i ", 
    		 mgf.getBase (mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(tAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(tAxis)));

    if (maxdim>99 && maxdim <1000) 
      printf ("Dimensions: %3i:%-3i %3i:%-3i %3i:%-3i ", 
    		 mgf.getBase (mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(tAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(tAxis)));

    if (maxdim < 100) 
      printf ("Dimensions: %2i:%-2i %2i:%-2i %2i:%-2i ", 
    		 mgf.getBase (mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(tAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(tAxis)));

    int maxcomp = max(abs(mgf.getComponentBase(0)), 
		  max(abs(mgf.getComponentBound(0)),
		  max(abs(mgf.getComponentBase(1)),
		  max(abs(mgf.getComponentBound(1)),
		  max(abs(mgf.getComponentBase(2)), 
		  max(abs(mgf.getComponentBound(2)),
		  max(abs(mgf.getComponentBase(3)), 
		  max(abs(mgf.getComponentBound(3)),
		  max(abs(mgf.getComponentBase(4)), abs(mgf.getComponentBound(4)))))))))));

    if (maxcomp>9999)
      printf ("Components: %5i:%-5i %5i:%-5i %5i:%-5i %5i:%-5i %5i:%-5i ", 
		 mgf.getComponentBase(0), mgf.getComponentBound(0),
		 mgf.getComponentBase(1), mgf.getComponentBound(1),
		 mgf.getComponentBase(2), mgf.getComponentBound(2),
		 mgf.getComponentBase(3), mgf.getComponentBound(3),
		 mgf.getComponentBase(4), mgf.getComponentBound(4));

    if (maxcomp>999 && maxcomp <10000)
      printf ("Components: %4i:%-4i %4i:%-4i %4i:%-4i %4i:%-4i %4i:%-4i ", 
		 mgf.getComponentBase(0), mgf.getComponentBound(0),
		 mgf.getComponentBase(1), mgf.getComponentBound(1),
		 mgf.getComponentBase(2), mgf.getComponentBound(2),
		 mgf.getComponentBase(3), mgf.getComponentBound(3),
		 mgf.getComponentBase(4), mgf.getComponentBound(4));

    if (maxcomp>99 && maxcomp<1000)
      printf ("Components: %3i:%-3i %3i:%-3i %3i:%-3i %3i:%-3i %3i:%-3i ", 
		 mgf.getComponentBase(0), mgf.getComponentBound(0),
		 mgf.getComponentBase(1), mgf.getComponentBound(1),
		 mgf.getComponentBase(2), mgf.getComponentBound(2),
		 mgf.getComponentBase(3), mgf.getComponentBound(3),
		 mgf.getComponentBase(4), mgf.getComponentBound(4));

    if (maxcomp<100)
      printf ("Components: %2i:%-2i %2i:%-2i %2i:%-2i %2i:%-2i %2i:%-2i ", 
		 mgf.getComponentBase(0), mgf.getComponentBound(0),
		 mgf.getComponentBase(1), mgf.getComponentBound(1),
		 mgf.getComponentBase(2), mgf.getComponentBound(2),
		 mgf.getComponentBase(3), mgf.getComponentBound(3),
		 mgf.getComponentBase(4), mgf.getComponentBound(4));

	// compute totalNumberOfComponents

    if (mindim < 0){
      printf ("\n\nTHIS ARRAY APPEARS TO BE EMPTY\n\n\n");
      return;
    }
    int totalNumberOfComponents = 1;
    int ic;
    for (ic = 0; ic < 5; ic++) totalNumberOfComponents *= mgf.getComponentDimension(ic);


      int l[8], m[8], typ[8], lBase[8], lBound[8];
      
      int countComps;
      for (countComps=0; countComps<5; countComps++){

		// "m" is used to order the loops correctly. the "components" should
		// come first followed by the "coordinates"
		// "typ" is an array that will contain "0" for a coordinate
		// dimension and "1" for a component dimension

	m[countComps+3] = mgf.positionOfComponent(countComps);
	typ[m[countComps+3]] = 1;

		// "lBase" and "lBound" are used to give the base and bound for
		// the loops. 

	lBase [countComps+3] = mgf.getComponentBase(countComps);
	lBound[countComps+3] = mgf.getComponentBound(countComps);
      }

      int countCoords;
      for (countCoords=0; countCoords<3; countCoords++){
	m[countCoords] = mgf.positionOfCoordinate(countCoords);
	typ[m[countCoords]] = 0;
	lBase [countCoords] = mgf.getBase (mgf.positionOfCoordinate(countCoords));
	lBound[countCoords] = mgf.getBound(mgf.positionOfCoordinate(countCoords));
	if (ccDisplayOption && 
	    mgfType == GridFunctionParameters::cellCentered &&
	    countCoords < numberOfDimensions) 
	   lBound[countCoords] -= 1;

      }

      printf ("\n      ");
      int index;
      for (index=0; index<8; index++) if (typ[index] == 0 ){ printf (" x");} else { printf (" c");};
      printf ("\n");

      for (l[m[7]]=lBase[7]; l[m[7]]<=lBound[7]; l[m[7]]++){
	for (l[m[6]]=lBase[6]; l[m[6]]<=lBound[6]; l[m[6]]++){
	  for (l[m[5]]=lBase[5]; l[m[5]]<=lBound[5]; l[m[5]]++){
	    for (l[m[4]]=lBase[4]; l[m[4]]<=lBound[4]; l[m[4]]++){
	      for (l[m[3]]=lBase[3]; l[m[3]]<=lBound[3]; l[m[3]]++){

	      if (totalNumberOfComponents > 1) 
		{
		  printf ("\n   COMPONENT");
		  for (countComps=0; countComps<5; countComps++) printf (" %2i", l[m[3+countComps]]);
		}else{
		  printf("\n");
		}
		printf ("  Centering: ");
		int axis;
		for (axis=0; axis<3; axis++) printf ("%3i", 
		    mgf.getIsCellCentered(axis, l[m[3]], l[m[4]], l[m[5]], l[m[6]], l[m[7]]));
		    //mgf.getIsCellCentered(axis, l[m[3]], l[m[4]], l[m[5]], l[m[6]]));
		printf ("    FaceCentering: ");
		//printf (" Unknown ");
		printf ("%3i", mgf.getFaceCentering());
		printf ("\n\n");

		for (l[m[2]]=lBase[2]; l[m[2]]<=lBound[2]; l[m[2]]++){
		// reverse 2nd index so that "y" is "up"
		  for (l[m[1]]=lBound[1]; l[m[1]]>=lBase[1]; l[m[1]]--){

		    for (l[m[0]]=lBase[0]; l[m[0]]<=lBound[0]; l[m[0]]++){
		      //printf (" %7.3f",mgf(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7]));

		      // 95008:this is a KLUDGE until Bill fixes scalar indexing
		      // and this won't work for >5 indices or unusual orderings

		      printf (" %7.3f",mgf(l[0],l[1],l[2],l[3]+(lBound[3]-lBase[3]+1)*l[4],0,l[5],l[6],l[7]));
		    }
		    printf ("\n");
		  }
		}

      }}}}}

}
//======================================================================================================= 
//
//
	void Display::display (const intMappedGridFunction &mgf)
//
		//
		// Display function for intMappedGridFunction's with no label or control
		//
//======================================================================================================= 
{
  bool ccDisplayOption = Display::cellCenteredDisplayOption;
  GridFunctionParameters::GridFunctionType mgfType = mgf.getGridFunctionType();
  MappedGrid * mg = mgf.mappedGrid;
  int numberOfDimensions = 0;
  if (mg != NULL)  numberOfDimensions = mg->numberOfDimensions();



    printf ("Type: intMappedGridFunction\n");
    		 int ndra = mgf.getBase (mgf.positionOfCoordinate(rAxis));
    		 int ndrb = mgf.getBound(mgf.positionOfCoordinate(rAxis));
    		 int ndsa = mgf.getBase (mgf.positionOfCoordinate(sAxis));
    		 int ndsb = mgf.getBound(mgf.positionOfCoordinate(sAxis));
    		 int ndta = mgf.getBase (mgf.positionOfCoordinate(tAxis));
    		 int ndtb = mgf.getBound(mgf.positionOfCoordinate(tAxis));
		 int maxdim = max(abs(ndra),max(abs(ndrb),max(abs(ndsa),max(abs(ndsb),max(abs(ndta),abs(ndtb))))));

		 int mindim = min(ndrb-ndra,min(ndsb-ndsa,ndtb-ndta));
    if (maxdim>9999) 
      printf ("Dimensions: %5i:%-5i %5i:%-5i %5i:%-5i ", 
    		 mgf.getBase (mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(tAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(tAxis)));

    if (maxdim>999 && maxdim<10000) 
      printf ("Dimensions: %4i:%-4i %4i:%-4i %4i:%-4i ", 
    		 mgf.getBase (mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(tAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(tAxis)));

    if (maxdim>99 && maxdim <1000) 
      printf ("Dimensions: %3i:%-3i %3i:%-3i %3i:%-3i ", 
    		 mgf.getBase (mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(tAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(tAxis)));

    if (maxdim < 100) 
      printf ("Dimensions: %2i:%-2i %2i:%-2i %2i:%-2i ", 
    		 mgf.getBase (mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(rAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(sAxis)),
    		 mgf.getBase (mgf.positionOfCoordinate(tAxis)),
    		 mgf.getBound(mgf.positionOfCoordinate(tAxis)));

    int maxcomp = max(abs(mgf.getComponentBase(0)), 
		  max(abs(mgf.getComponentBound(0)),
		  max(abs(mgf.getComponentBase(1)),
		  max(abs(mgf.getComponentBound(1)),
		  max(abs(mgf.getComponentBase(2)), 
		  max(abs(mgf.getComponentBound(2)),
		  max(abs(mgf.getComponentBase(3)), 
		  max(abs(mgf.getComponentBound(3)),
		  max(abs(mgf.getComponentBase(4)), abs(mgf.getComponentBound(4)))))))))));

    if (maxcomp>9999)
      printf ("Components: %5i:%-5i %5i:%-5i %5i:%-5i %5i:%-5i %5i:%-5i ", 
		 mgf.getComponentBase(0), mgf.getComponentBound(0),
		 mgf.getComponentBase(1), mgf.getComponentBound(1),
		 mgf.getComponentBase(2), mgf.getComponentBound(2),
		 mgf.getComponentBase(3), mgf.getComponentBound(3),
		 mgf.getComponentBase(4), mgf.getComponentBound(4));

    if (maxcomp>999 && maxcomp <10000)
      printf ("Components: %4i:%-4i %4i:%-4i %4i:%-4i %4i:%-4i %4i:%-4i ", 
		 mgf.getComponentBase(0), mgf.getComponentBound(0),
		 mgf.getComponentBase(1), mgf.getComponentBound(1),
		 mgf.getComponentBase(2), mgf.getComponentBound(2),
		 mgf.getComponentBase(3), mgf.getComponentBound(3),
		 mgf.getComponentBase(4), mgf.getComponentBound(4));

    if (maxcomp>99 && maxcomp<1000)
      printf ("Components: %3i:%-3i %3i:%-3i %3i:%-3i %3i:%-3i %3i:%-3i ", 
		 mgf.getComponentBase(0), mgf.getComponentBound(0),
		 mgf.getComponentBase(1), mgf.getComponentBound(1),
		 mgf.getComponentBase(2), mgf.getComponentBound(2),
		 mgf.getComponentBase(3), mgf.getComponentBound(3),
		 mgf.getComponentBase(4), mgf.getComponentBound(4));

    if (maxcomp<100)
      printf ("Components: %2i:%-2i %2i:%-2i %2i:%-2i %2i:%-2i %2i:%-2i ", 
		 mgf.getComponentBase(0), mgf.getComponentBound(0),
		 mgf.getComponentBase(1), mgf.getComponentBound(1),
		 mgf.getComponentBase(2), mgf.getComponentBound(2),
		 mgf.getComponentBase(3), mgf.getComponentBound(3),
		 mgf.getComponentBase(4), mgf.getComponentBound(4));

	// compute totalNumberOfComponents

    if (mindim < 0){
      printf ("\n\nTHIS ARRAY APPEARS TO BE EMPTY\n\n\n");
      return;
    }
    int totalNumberOfComponents = 1;
    int ic;
    for (ic = 0; ic < 5; ic++) totalNumberOfComponents *= mgf.getComponentDimension(ic);

      int l[8], m[8], typ[8], lBase[8], lBound[8];

      int countComps;
      for (countComps=0; countComps<5; countComps++){

		// "m" is used to order the loops correctly. the "components" should
		// come first followed by the "coordinates"
		// "typ" is an array that will contain "0" for a coordinate
		// dimension and "1" for a component dimension

	m[countComps+3] = mgf.positionOfComponent(countComps);
	typ[m[countComps+3]] = 1;

		// "lBase" and "lBound" are used to give the base and bound for
		// the loops. 

	lBase [countComps+3] = mgf.getComponentBase(countComps);
	lBound[countComps+3] = mgf.getComponentBound(countComps);
      }

      int countCoords;
      for (countCoords=0; countCoords<3; countCoords++){
	m[countCoords] = mgf.positionOfCoordinate(countCoords);
	typ[m[countCoords]] = 0;
	lBase [countCoords] = mgf.getBase (mgf.positionOfCoordinate(countCoords));
	lBound[countCoords] = mgf.getBound(mgf.positionOfCoordinate(countCoords));

	if (ccDisplayOption && 
	    mgfType == GridFunctionParameters::cellCentered &&
	    countCoords < numberOfDimensions) 
	   lBound[countCoords] -= 1;

      }

      printf ("\n      ");
      int index;
      for (index=0; index<8; index++) if (typ[index] == 0 ){ printf (" x");} else { printf (" c");};
      printf ("\n");

      for (l[m[7]]=lBase[7]; l[m[7]]<=lBound[7]; l[m[7]]++){
	for (l[m[6]]=lBase[6]; l[m[6]]<=lBound[6]; l[m[6]]++){
	  for (l[m[5]]=lBase[5]; l[m[5]]<=lBound[5]; l[m[5]]++){
	    for (l[m[4]]=lBase[4]; l[m[4]]<=lBound[4]; l[m[4]]++){
	      for (l[m[3]]=lBase[3]; l[m[3]]<=lBound[3]; l[m[3]]++){

	      if (totalNumberOfComponents > 1) 
		{
		  printf ("\n   COMPONENT");
		  for (countComps=0; countComps<5; countComps++) printf (" %2i", l[m[3+countComps]]);
		}else{
		  printf("\n");
		}
		printf ("  Centering: ");
		int axis;
		for (axis=0; axis<3; axis++) printf ("%3i", 
		      
		    mgf.getIsCellCentered(axis, l[m[3]], l[m[4]], l[m[5]], l[m[6]], l[m[7]]));

		    //mgf.getIsCellCentered(axis, l[m[3]], l[m[4]], l[m[5]], l[m[6]]));
		printf ("    FaceCentering: ");
		//printf (" Unknown ");
		printf ("%3i", mgf.getFaceCentering());
		printf ("\n\n");

		for (l[m[2]]=lBase[2]; l[m[2]]<=lBound[2]; l[m[2]]++){
		  for (l[m[1]]=lBound[1]; l[m[1]]>=lBase[1]; l[m[1]]--){
		    for (l[m[0]]=lBase[0]; l[m[0]]<=lBound[0]; l[m[0]]++){
		      printf (" %5i",mgf(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7]));
		    }
		    printf ("\n");
		  }
		}
      }}}}}

}
//======================================================================================================= 
	void Display::display (const floatArray &mgf)
//
		//
		// Display function for floatArray's with no control or label
		//
//======================================================================================================= 
{


    int ndra = mgf.getBase (0);
    int ndrb = mgf.getBound(0);
    int ndsa = mgf.getBase (1);
    int ndsb = mgf.getBound(1);
    int ndta = mgf.getBase (2);
    int ndtb = mgf.getBound(2);
    int ndua = mgf.getBase (3);
    int ndub = mgf.getBound(3);

    printf ("Type: floatArray\n");
    printf ("Dimensions: %5i %5i %5i %5i %5i %5i %5i %5i \n", ndra, ndrb, ndsa, ndsb, ndta, ndtb, ndua, ndub);
      int i,j,k,l;
      for (l=ndua; l<=ndub; l++)
      {
	for (k=ndta; k<=ndtb; k++)
	{
	  for (j = ndsb;j>=ndsa;j--)
	  {
	    for (i=ndra; i<=ndrb; i++)
	    {
	      printf(" %7.3f",mgf(i,j,k,l));
	    }
	    printf ("\n");
	  }
	  printf ("\n");
	}
	//printf ("\n");
      }
}
//======================================================================================================= 
	void Display::display (const doubleArray &mgf)
//
		//
		// Display function for doubleArray's with no control or label
		//
//======================================================================================================= 
{


    int ndra = mgf.getBase (0);
    int ndrb = mgf.getBound(0);
    int ndsa = mgf.getBase (1);
    int ndsb = mgf.getBound(1);
    int ndta = mgf.getBase (2);
    int ndtb = mgf.getBound(2);
    int ndua = mgf.getBase (3);
    int ndub = mgf.getBound(3);

    printf ("Type: doubleArray\n");
    printf ("Dimensions: %5i %5i %5i %5i %5i %5i %5i %5i \n", ndra, ndrb, ndsa, ndsb, ndta, ndtb, ndua, ndub);
      int i,j,k,l;
      for (l=ndua; l<=ndub; l++)
      {
	for (k=ndta; k<=ndtb; k++)
	{
	  for (j = ndsb;j>=ndsa;j--)
	  {
	    for (i=ndra; i<=ndrb; i++)
	    {
	      printf(" %7.3f",mgf(i,j,k,l));
	    }
	    printf ("\n");
	  }
	  printf ("\n");
	}
	//printf ("\n");
      }
}
//======================================================================================================= 
	void Display::display (const intArray &mgf)
//
		//
		// Display function for intArray's with no control or label
		//
//======================================================================================================= 
{


    int ndra = mgf.getBase (0);
    int ndrb = mgf.getBound(0);
    int ndsa = mgf.getBase (1);
    int ndsb = mgf.getBound(1);
    int ndta = mgf.getBase (2);
    int ndtb = mgf.getBound(2);
    int ndua = mgf.getBase (3);
    int ndub = mgf.getBound(3);

    printf ("Type: intArray\n");
    printf ("Dimensions: %5i %5i %5i %5i %5i %5i %5i %5i \n", ndra, ndrb, ndsa, ndsb, ndta, ndtb, ndua, ndub);
      int i,j,k,l;
	for (l=ndua; l<=ndub; l++)
	{
	for (k=ndta; k<=ndtb; k++)
	{
	  for (j = ndsb;j>=ndsa;j--)
	  {
	    for (i=ndra; i<=ndrb; i++)
	    {
	      printf(" %5i",mgf(i,j,k,l));
	    }
	    printf ("\n");
	  }
	  printf ("\n");
	}
	//printf ("\n");
	}
}
//======================================================================================================= 
	void Display::display (const Index &I)
//
		//
		// Display function for Index's with no control or label
		//
//======================================================================================================= 
{
  printf ("\n\t(Index) Base= %4i \tBound= %4i \tStride= %4i \tLength= %4i \n",
	      I.getBase(), I.getBound(), I.getStride(), I.length());
}

