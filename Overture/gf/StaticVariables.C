#include "StaticVariables.h"

// --------------- StaticVariables.C ---------------
static int niftyCounter=0;
floatArray * StaticVariables::nullFArray=NULL;

initStaticVariables::
initStaticVariables()
{ 
  printf(" initStaticVariables:constructor, nifty counter=%i \n",niftyCounter);
  if( 0==niftyCounter++ )
  {
    printf(" construct StaticVariables \n");
    s=new StaticVariables;
  }
}
initStaticVariables::
~initStaticVariables()
{ 
  printf(" initStaticVariables:destructor, nifty counter=%i \n",niftyCounter);
  if( 0==--niftyCounter )
  {
    printf(" delete StaticVariables \n");
    delete s;
  }
}
  
