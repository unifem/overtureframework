#include "checkGridFunction.h"
#include "ParallelUtility.h"

//==========================================================================================
//   Check a grid function for floating point errors, nan's and inf's
//==========================================================================================

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

int
checkGridFunction(realMappedGridFunction & u, const aString & title, bool printResults, int grid )
{
  const int myid=max(0,Communication_Manager::My_Process_Number);

  bool nanFound=false;

  realArray & uu = u;
  MappedGrid & mg = *u.getMappedGrid();
    

  const intArray & mask = mg.mask();
	
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
  #else
    const realSerialArray & uLocal  =  uu;
    const intSerialArray & maskLocal = mask;
  #endif

  Index I1,I2,I3;
  Index D1,D2,D3;
  getIndex(mg.gridIndexRange(),I1,I2,I3);
  getIndex(mg.dimension(),D1,D2,D3);

  const int includeGhost=1;
  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
  ok = ok && ParallelUtility::getLocalArrayBounds(u,uLocal,D1,D2,D3,includeGhost);


  int num=0;
  real maxVal=0.;
  if( ok )
  {
    for( int c=u.getComponentBase(0); c<=u.getComponentBound(0); c++ )
    {
      Range all;
      real minU=0.,maxU=0.;

      int count=sum(maskLocal(I1,I2,I3)!=0);
      where( maskLocal(I1,I2,I3)!=0 )
      {
	minU=min(uLocal(I1,I2,I3,c));
	maxU=max(uLocal(I1,I2,I3,c));
      }

      // check for nan's
      int i1,i2,i3;
      FOR_3D(i1,i2,i3,D1,D2,D3)
      {
	maxVal=max(maxVal,fabs(uLocal(i1,i2,i3,c)));
      
	if( uLocal(i1,i2,i3,c) != uLocal(i1,i2,i3,c)|| 
	    uLocal(i1,i2,i3,c) > REAL_MAX )
	{
	  nanFound=true;
	  num++;
	  if( num<100 )
	  {
	    int mm=maskLocal(i1,i2,i3);
	    if( mm<0 ) 
	      mm=-1;
	    else if( mm>0 )
	      mm=1;
	      
	    if( printResults )
	    {
	      printf("u[%i](%i,%i,%i,c=%i)=%e mask=%i, ",grid,i1,i2,i3,c,uLocal(i1,i2,i3,c),mm);
	      if( num % 5 == 0 ) printF("\n");
	    }
	  
	  }
	}
      }
      if( printResults )
      {
	if( num>=100 ) printF("\nMore nan or inf's were found on grid=%i but I stopped printing them.\n",grid);
	
	printf("checkSolution:%s myid=%i, grid=%i: component=%i: min=%e, max=%e,  no. of pts with mask!=0 =%i\n",
	       (const char*)title,myid,grid,c,minU,maxU,count);
      }
    
    }
  }  // end if ok 

  if( nanFound )
  {
    char buff[100];
    if( !printResults )
    {
      checkGridFunction(u,title,true,grid);  // call again and print results
    }
    aString msg=sPrintF(buff,"checkSolution:%s ERROR: nan or inf's found on grid=%i!",(const char*)title,grid);
    Overture::abort(msg);
  }
  else
  {
    printF("checkSolution:%s No nan or inf's found on grid=%i. max(abs(u))=%8.2e\n",(const char*)title,grid,maxVal);
  }
  return num;
}


int
checkGridFunction(realGridCollectionFunction & u, const aString & title, bool printResults )
{
  GridCollection & gc = *u.getGridCollection();
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    checkGridFunction(u[grid], title, printResults, grid );
  }
  

}
