#include "display.h"
#include "MappedGrid.h"
int 
displayMaskLaTeX( const intArray & mask, 
             const aString & label =nullString,
             FILE *file = NULL ,
             const DisplayParameters *displayParameters = NULL )
// =====================================================================================
//  /Description:
//    Output the mask in a form that can be displayed with pstricks in a latex file.
// =====================================================================================
{
  
  FILE *filep = file!=NULL ? file : stdout; // dp.file defaults to stdout

  if( label!="" )
    fprintf(filep,"%s\n",(const char*)label);

  // \psdots[dotstyle=o](2.,7.)(5.,8.)(7.,9.)
  aString discretization="\\psdots[dotstyle=square*]";
  aString unused="\\psdots[dotstyle=o]";
  aString interpolation= "\\psdots[dotstyle=*]"; 
  aString line;
  
  for( int i3=mask.getBase(2); i3<=mask.getBound(2); i3++ )
  {
    for( int i2=mask.getBase(1); i2<=mask.getBound(1); i2++ )
    {
      for( int i1=mask.getBase(0); i1<=mask.getBound(0); i1++ )
      {
        if( mask(i1,i2,i3)==0 )
	{
	  unused+=sPrintF(line,"(%i,%i)",i1,i2);
	}
	else if( mask(i1,i2,i3) <0 )
	{
          interpolation+=sPrintF(line,"(%i,%i)",i1,i2);
	}
	else
	{
          discretization+=sPrintF(line,"(%i,%i)",i1,i2);
	}
      }
    }
  }
  fprintf(filep,"%s\n",(const char*)interpolation);
  fprintf(filep,"%s\n",(const char*)discretization);
  fprintf(filep,"%s\n",(const char*)unused);
  

  return 0;
}
