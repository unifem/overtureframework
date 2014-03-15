#include "GL_GraphicsInterface.h"
#include <ctype.h>

static const int numberOfXColours = 102; // 78

static int rgb[numberOfXColours][3] = 
{
     255,255,255,   //  DEFAULT
     112,219,147,   //  AQUAMARINE
      50,204,153,   //  MEDIUMAQUAMARINE
       0,  0,  0,   //  BLACK
       0,  0,255,   //  BLUE
      95,159,159,   //  CADETBLUE
      66, 66,111,   //  CORNFLOWERBLUE
     107, 35,142,   //  DARKSLATEBLUE
     191,216,216,   //  LIGHTBLUE
     143,143,188,   //  LIGHTSTEELBLUE
      50, 50,204,   //  MEDIUMBLUE
     127,  0,255,   //  MEDIUMSLATEBLUE
      47, 47, 79,   //  MIDNIGHTBLUE
      35, 35,142,   //  NAVYBLUE
      50,153,204,   //  SKYBLUE
       0,127,255,   //  SLATEBLUE'/
      35,107,142,   //  STEELBLUE
     255,127,  0,   //  CORAL
       0,255,255,   //  CYAN
     142, 35, 35,   //  FIREBRICK
     165, 42, 42,   //  BROWN
     244,164, 96,   //  SANDYBROWN
     204,127, 50,   //  GOLD
     219,219,112,   //  GOLDENROD
     234,234,173,   //  MEDIUMGOLDENROD
       0,255,  0,   //  GREEN
      47, 79, 47,   //  DARKGREEN
      79, 79, 47,   //  DARKOLIVEGREEN
      35,142, 35,   //  FORESTGREEN
      50,204, 50,   //  LIMEGREEN
     107,142, 35,   //  MEDIUMFORESTGREEN
      66,111, 66,   //  MEDIUMSEAGREEN
     127,255,  0,   //  MEDIUMSPRINGGREEN
     143,188,143,   //  PALEGREEN
      35,142,107,   //  SEAGREEN
       0,255,127,   //  SPRINGGREEN
     153,204, 50,   //  YELLOWGREEN
      47, 79, 79,   //  DARKSLATEGRAY
      84, 84, 84,   //  DIMGRAY
     192,192,192,   //  GRAY
     211,211,211,   //  LIGHTGRAY
     159,159, 95,   //  KHAKI
     255,  0,255,   //  MAGENTA
     142, 35,107,   //  MAROON
     204, 50, 50,   //  ORANGE
     219,112,219,   //  ORCHID
     153, 50,204,   //  DARKORCHID
     147,112,219,   //  MEDIUMORCHID
     188,143,143,   //  PINK
     234,173,234,   //  PLUM
     255,  0,  0,   //  RED
      79, 47, 47,   //  INDIANRED
     219,112,147,   //  MEDIUMVIOLETRED
     255,  0,127,   //  ORANGERED
     204, 50,153,   //  VIOLETRED
     111, 66, 66,   //  SALMON
     142,107, 35,   //  SIENNA
     219,147,112,   //  TAN
     216,191,216,   //  THISTLE
     173,234,234,   //  TURQUOISE
     112,147,219,   //  DARKTURQUOISE
     112,219,219,   //  MEDIUMTURQUOISE
      79, 47, 79,   //  VIOLET
     159, 95,159,   //  BLUEVIOLET
     216,216,191,   //  WHEAT
     255,255,255,   //  WHITE
     255,255,  0,   //  YELLOW
     147,219,112,   //  GREENYELLOW
      26, 26, 26,   //  GRAY10
      51, 51, 51,   //  GRAY20
      77, 77, 77,   //  GRAY30
     102,102,102,   //  GRAY40
     127,127,127,   //  GRAY50
     153,153,153,   //  GRAY60
     179,179,179,   //  GRAY70
     204,204,204,   //  GRAY80
     229,229,229,   //  GRAY90
     255,255,255,    //  DEFAULT
     // *********** here are some special colours we use *********************
     // The actual colour properties are set in GL_GraphicsInterface::setColour()
     112,219,147,   //        "emerald", 
     112,219,147,   //       "jade", 
     112,219,147,   //       "obsidian", 
     112,219,147,   //       "pearl", 
     112,219,147,   //       "ruby", 
     112,219,147,   //       "turquoise", 
     112,219,147,   //       "brass", 
     112,219,147,   //        "bronze", 
     112,219,147,   //       "chrome", 
     112,219,147,   //       "copper", 
     112,219,147,   //       "gold",
     112,219,147,   //       "silver", 
     112,219,147,   //       "blackPlastic", 
     112,219,147,   //       "cyanPlastic", 
     112,219,147,   //       "greenPlastic", 
     112,219,147,   //       "redPlastic", 
     112,219,147,   //       "whitePlastic", 
     112,219,147,   //       "yellowPlastic",
     112,219,147,   //       "blackRubber", 
     112,219,147,   //       "cyanRubber", 
     112,219,147,   //       "greenRubber", 
     112,219,147,   //       "redRubber", 
     112,219,147,   //       "whiteRubber",
     112,219,147    //       "yellowRubber"
   };

static aString xName[numberOfXColours] = {
     "DEFAULT",
     "AQUAMARINE",
     "MEDIUMAQUAMARINE",
     "BLACK",
     "BLUE",
     "CADETBLUE",
     "CORNFLOWERBLUE",
     "DARKSLATEBLUE",
     "LIGHTBLUE",
     "LIGHTSTEELBLUE",
     "MEDIUMBLUE",
     "MEDIUMSLATEBLUE",
     "MIDNIGHTBLUE",
     "NAVYBLUE",
     "SKYBLUE",
     "SLATEBLUE",
     "STEELBLUE",
     "CORAL",
     "CYAN",
     "FIREBRICK",
     "BROWN",
     "SANDYBROWN",
     "GOLD",
     "GOLDENROD",
     "MEDIUMGOLDENROD",
     "GREEN",
     "DARKGREEN",
     "DARKOLIVEGREEN",
     "FORESTGREEN",
     "LIMEGREEN",
     "MEDIUMFORESTGREEN",
     "MEDIUMSEAGREEN",
     "MEDIUMSPRINGGREEN",
     "PALEGREEN",
     "SEAGREEN",
     "SPRINGGREEN",
     "YELLOWGREEN",
     "DARKSLATEGRAY",
     "DIMGRAY",
     "GRAY",
     "LIGHTGRAY",
     "KHAKI",
     "MAGENTA",
     "MAROON",
     "ORANGE",
     "ORCHID",
     "DARKORCHID",
     "MEDIUMORCHID",
     "PINK",
     "PLUM",
     "RED",
     "INDIANRED",
     "MEDIUMVIOLETRED",
     "ORANGERED",
     "VIOLETRED",
     "SALMON",
     "SIENNA",
     "TAN",
     "THISTLE",
     "TURQUOISE",
     "DARKTURQUOISE",
     "MEDIUMTURQUOISE",
     "VIOLET",
     "BLUEVIOLET",
     "WHEAT",
     "WHITE",
     "YELLOW",
     "GREENYELLOW",
     "GRAY10",
     "GRAY20",
     "GRAY30",
     "GRAY40",
     "GRAY50",
     "GRAY60",
     "GRAY70",
     "GRAY80",
     "GRAY90",
     "DEFAULT",
     // *********** here are some special colours we use *********************
     "EMERALD", 
     "JADE", 
     "OBSIDIAN", 
     "PEARL", 
     "RUBY", 
     "TURQUOISE", 
     "BRASS", 
      "BRONZE", 
     "CHROME", 
     "COPPER", 
     "GOLD",
     "SILVER", 
     "BLACKPLASTIC", 
     "CYANPLASTIC", 
     "GREENPLASTIC", 
     "REDPLASTIC", 
     "WHITEPLASTIC", 
     "YELLOWPLASTIC",
     "BLACKRUBBER", 
     "CYANRUBBER", 
     "GREENRUBBER", 
     "REDRUBBER", 
     "WHITERUBBER",
     "YELLOWRUBBER"
     };

static aString xNameMixedCase[numberOfXColours+1] = {
     "default",
     "aquamarine",
     "medium aquamarine",
     "black",
     "blue",
     "cadet blue",
     "cornflower blue",
     "dark slate blue",
     "light blue",
     "light steel blue",
     "medium blue",
     "medium slate blue",
     "midnight blue",
     "navy blue",
     "sky blue",
     "slate blue",
     "steel blue",
     "coral",
     "cyan",
     "fire brick",
     "brown",
     "sandy brown",
     "gold",
     "goldenrod",
     "medium goldenrod",
     "green",
     "dark green",
     "dark olive green",
     "forest green",
     "lime green",
     "medium forest green",
     "medium sea green",
     "medium spring green",
     "pale green",
     "sea green",
     "spring green",
     "yellow green",
     "dark slate gray",
     "dim gray",
     "gray",
     "light gray",
     "khaki",
     "magenta",
     "maroon",
     "orange",
     "orchid",
     "dark orchid",
     "medium orchid",
     "pink",
     "plum",
     "red",
     "indian red",
     "medium violet red",
     "orange red",
     "violet red",
     "salmon",
     "sienna",
     "tan",
     "thistle",
     "turquoise",
     "dark turquoise",
     "medium turquoise",
     "violet",
     "blue violet",
     "wheat",
     "white",
     "yellow",
     "green yellow",
     "gray10",
     "gray20",
     "gray30",
     "gray40",
     "gray50",
     "gray60",
     "gray70",
     "gray80",
     "gray90",
     "default",
     // *********** here are some special colours we use *********************
     "emerald", 
     "jade", 
     "obsidian", 
     "pearl", 
     "ruby", 
     "turquoise", 
     "brass", 
     "bronze", 
     "chrome", 
     "copper", 
     "gold",
     "silver", 
     "black plastic", 
     "cyan plastic", 
     "green plastic", 
     "red plastic", 
     "white plastic", 
     "yellow plastic",
     "black rubber", 
     "cyan rubber", 
     "green rubber", 
     "red rubber", 
     "white rubber",
     "yellow rubber"
     ""
     };


const aString*
getAllColourNames()
// ===================================================
//  Return an array of all colour names
// ==================================================
{
  return xNameMixedCase;
}


//---------------------------------------------------------------------------------------
//  Set the current colour to correspond the name of an "X colour"
//---------------------------------------------------------------------------------------
void
setXColour( const aString & xColourName )
{
  // --- first convert the name to upper case and remove all blanks ----

  aString name = "                                              ";
  int i,j=0;
  for( i=0; i<xColourName.length(); i++ )
  {
    if( xColourName[i]!=' ' )
      name[j++]=toupper(xColourName[i]);
  }

  int iColour=0;
  for( i=0; i<numberOfXColours && j>0; i++ )
  {
    if( name(0,j-1)==xName[i] )
    {
      iColour=i;
      break;
    }
  }
  //cout << "input name = " << xColourName << ", name=" << name << " iColour = " << iColour 
  //     << ", xName[iColour] = " << xName[iColour] << endl;
  //printf("(r,g,b)=(%e,%e,%e)\n",rgb[iColour][0]/255.,rgb[iColour][1]/255.,rgb[iColour][2]/255.); 

  glColor3f( rgb[iColour][0]/255.,rgb[iColour][1]/255.,rgb[iColour][2]/255. );
}


//---------------------------------------------------------------------------------------
//  Set the current colour to correspond the "X colour" with the given index 
//---------------------------------------------------------------------------------------
void
setXColour( int index )
{
  index=max(0,min(numberOfXColours-1,index));
  glColor3f( rgb[index][0]/255.,rgb[index][1]/255.,rgb[index][2]/255. );
}
//---------------------------------------------------------------------------------------
//  Return the name of the xColour with the given index.
//---------------------------------------------------------------------------------------
const aString &
getXColour( int index )
{
  index=max(0,min(numberOfXColours-1,index));
  return xName[index];
}

//---------------------------------------------------------------------------------------
//  Get the index value for a colour name
//---------------------------------------------------------------------------------------
int
getXColour( const aString & xColourName )
{
    // --- first convert the name to upper case and remove all blanks ----

  aString name = "                                              ";
  int i,j=0;
  for( i=0; i<xColourName.length(); i++ )
  {
    if( xColourName[i]!=' ' )
      name[j++]=toupper(xColourName[i]);
  }

  int iColour=0;
  for( i=0; i<numberOfXColours; i++ )
  {
    if( name(0,j-1)==xName[i] )
    {
      iColour=i;
      break;
    }
  }
  return iColour;
}

//---------------------------------------------------------------------------------------
//  Get the rgb values for a colour and the index
//---------------------------------------------------------------------------------------
int
getXColour( const aString & xColourName, real *rgbValues )
{
  // --- first convert the name to upper case and remove all blanks ----

  int iColour=getXColour(xColourName);
  rgbValues[0]=rgb[iColour][0]/255.;
  rgbValues[1]=rgb[iColour][1]/255.;
  rgbValues[2]=rgb[iColour][2]/255.;

  return iColour;
}



//---------------------------------------------------------------------------------------
//  Get the rgb values for a colour
//---------------------------------------------------------------------------------------
void
getXColour( const aString & xColourName, RealArray & rgbValues )
{
  // --- first convert the name to upper case and remove all blanks ----
  real values[3];
  getXColour(xColourName,values);
  rgbValues(0)=values[0];
  rgbValues(1)=values[1];
  rgbValues(2)=values[2];
}

/* ----
int main()
{
  aString name;

  for(;;)
  {
    cout << "Enter a X colour name \n";
    cin >> name;
    setXColour( name );
  }
  return 0;
}
---- */
