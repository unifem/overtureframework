#include "DomainSolver.h"
#include "EquationSolver.h"
#include "App.h"
#include "ParallelUtility.h"
#include "gridFunctionNorms.h"

real 
maxNorm(const realCompositeGridFunction & u, const int cc, int maskOption, int extra );
real 
lpNorm(const int p, const realCompositeGridFunction & u, const int cc, int maskOption, int extra );


#define ForBoundary(side,axis)   for( int axis=0; axis<numberOfDimensions; axis++ ) \
                                 for( int side=0; side<=1; side++ )

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

void DomainSolver::
checkSolution(const realGridCollectionFunction & u, const aString & title, bool printResults /* =false */ )
{
  int myid=max(0,Communication_Manager::My_Process_Number);

  const GridCollection & cg = *u.getGridCollection();

  int numberOfErrors=0; 
  real maxVal=0.,maxValGrid;
  for( int grid=0; grid<cg.numberOfGrids(); grid++ )
  {
    
    int num = checkSolution(u[grid],title,printResults,grid,maxValGrid );
    maxVal=max(maxVal,maxValGrid);
    
    numberOfErrors+=num;
    

//     realArray & uu = u[grid];
//     for( int c=u.getComponentBase(0); c<=u.getComponentBound(0); c++ )
//     {
//       Range all;
//       real minU=0.,maxU=0.;

//       Index I1,I2,I3;
//       getIndex(cg[grid].gridIndexRange(),I1,I2,I3);
//       const intArray & mask = cg[grid].mask();
	
//       int count=sum(mask(I1,I2,I3)!=0);
//       where( mask(I1,I2,I3)!=0 )
//       {
// 	minU=min(uu(I1,I2,I3,c));
// 	maxU=max(uu(I1,I2,I3,c));
//       }

//       // check for nan's
//       getIndex(cg[grid].dimension(),I1,I2,I3);
//       int i1,i2,i3;
//       int num=0;
//       FOR_3D(i1,i2,i3,I1,I2,I3)
//       {
//         maxVal=max(maxVal,fabs(uu(i1,i2,i3,c)));
// 	if( uu(i1,i2,i3,c) != uu(i1,i2,i3,c) || 
//             uu(i1,i2,i3,c) > REAL_MAX )
// 	{
// 	  nanFound=true;
// 	  num++;
// 	  if( num<100 )
// 	  {
// 	    int mm=mask(i1,i2,i3);
// 	    if( mm<0 ) 
// 	      mm=-1;
// 	    else if( mm>0 )
// 	      mm=1;
	      
//             if( printResults )
// 	    {
// 	      printf("u[%i](%i,%i,%i,%i)=%e mask=%i, ",grid,i1,i2,i3,c,uu(i1,i2,i3,c),mm);
// 	      if( num % 5 == 0 ) printf("\n");
// 	    }
// 	  }
// 	}
//       }
//       if( printResults )
//       {
//         if( nanFound ) printf("\n");

//         if( num>=100 ) printf("More nan's or inf's were found on grid=%i but I stopped printing them.\n",grid);
	
// 	printf("checkSolution:%s grid=%i: component=%i: min=%9.2e, max=%9.2e count=%i (mask!=0)\n",
// 	       (const char*)title,grid,c,minU,maxU,count);
//       }
//     }
    
  }  // end for grid
  
  if( numberOfErrors>0 )
  {
    char buff[100];
    if( !printResults )
    {
      checkSolution(u,title,true);  // call again and print results
    }
    else
    {
      aString msg=sPrintF(buff,"checkSolution:%s ERROR: nan or inf found!",(const char*)title);
      Overture::abort(msg);
    }
  }
  else
  {
    printP("++checkSolution:%s No nan or inf's found. max(abs(u))=%8.2e (all pts)\n",(const char*)title,maxVal);
  }
  
}



int DomainSolver::
checkSolution(realMappedGridFunction & u, const aString & title, bool printResults, int grid,
              real & maxVal, bool printResultsOnFailure /* =false */ )
{
  int myid=max(0,Communication_Manager::My_Process_Number);

  int nanFound=false;

  MappedGrid & mg = *u.getMappedGrid();
  realArray & uu = u;
  const intArray & mask = mg.mask();

  #ifdef USE_PPP
    realSerialArray uuLocal;  getLocalArrayWithGhostBoundaries(uu,uuLocal);
    intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);
  #else
    const realSerialArray & uuLocal  = uu;
    const intSerialArray & maskLocal=mask; 
  #endif

  FILE *&debugFile = parameters.dbase.get<FILE* >("debugFile");
  FILE *&pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");
    
  int num=0;
  maxVal=0.;
  for( int c=u.getComponentBase(0); c<=u.getComponentBound(0); c++ )
  {
    Index I1,I2,I3;
    
    getIndex(mg.dimension(),I1,I2,I3);
    bool includeGhost=1;
    bool ok = ParallelUtility::getLocalArrayBounds(uu,uuLocal,I1,I2,I3,includeGhost); 

    real *uup = uuLocal.Array_Descriptor.Array_View_Pointer3;
    const int uuDim0=uuLocal.getRawDataSize(0);
    const int uuDim1=uuLocal.getRawDataSize(1);
    const int uuDim2=uuLocal.getRawDataSize(2);
#undef UU
#define UU(i0,i1,i2,i3) uup[i0+uuDim0*(i1+uuDim1*(i2+uuDim2*(i3)))]

    const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=maskLocal.getRawDataSize(0);
    const int maskDim1=maskLocal.getRawDataSize(1);
    const int md1=maskDim0, md2=md1*maskDim1; 
#undef MASK
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

    // check for nan's
    int i1,i2,i3;
    if( ok )
    {
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	maxVal=max(maxVal,fabs(UU(i1,i2,i3,c)));
      
	if( UU(i1,i2,i3,c) != UU(i1,i2,i3,c) || UU(i1,i2,i3,c) > REAL_MAX )
	{
	  nanFound=true;
	  num++;
	  if( num<100 )
	  {
	    int mm=MASK(i1,i2,i3);
	    if( mm<0 ) 
	      mm=-1;
	    else if( mm>0 )
	      mm=1;
	      
	    if( printResults )
	    {
	      fprintf(pDebugFile,"u[%i](%i,%i,%i,c=%i)=%e mask=%i, ",grid,i1,i2,i3,c,UU(i1,i2,i3,c),mm);
	      if( num % 5 == 0 ) fprintf(pDebugFile,"\n");
	    }
	  
	  }
	}
// 	if( c==0 && MASK(i1,i2,i3)!=0 && UU(i1,i2,i3,c)<=0. ) // check for small density
// 	{
// 	  num++;
// 	  if( printResults )
// 	  {
// 	    printf("u[%i](%i,%i,%i,c=%i)=%e mask=%i,(**) ",grid,i1,i2,i3,c,UU(i1,i2,i3,c),MASK(i1,i2,i3));
// 	    if( num % 5 == 0 ) printf("\n");
// 	  }
// 	}
      
      }
    }
    
    if( printResults )
    {
      if( num>=100 ) printP("\nMore nan or inf's were found on grid=%i but I stopped printing them.\n",grid);
	

      getIndex(mg.gridIndexRange(),I1,I2,I3);
      bool ok = ParallelUtility::getLocalArrayBounds(uu,uuLocal,I1,I2,I3);
    
      real minU=0.,maxU=0.;
      int count=0;
      if( ok )
      {
	count=sum(maskLocal(I1,I2,I3)!=0);
	where( maskLocal(I1,I2,I3)!=0 )
	{
	  minU=min(uuLocal(I1,I2,I3,c));
	  maxU=max(uuLocal(I1,I2,I3,c));
	}

      }
      count=ParallelUtility::getSum(count);
      minU=ParallelUtility::getMinValue(minU);
      maxU=ParallelUtility::getMaxValue(maxU);

      printP("checkSolution:%s grid=%i: component=%i: min=%e, max=%e count=%i (mask!=0)\n",
	       (const char*)title,grid,c,minU,maxU,count);
    }
    
  }  // end for int c 
  
  num = ParallelUtility::getSum(num);
  nanFound = ParallelUtility::getMaxValue(nanFound);
  if( nanFound )
  {
    if( printResults )
      printP("  checkSolution:%s ERROR %i nan or inf's found on grid=%i. max(abs(u))=%8.2e, See debug file for details.\n",
             (const char*)title,num,grid,maxVal);

    char buff[100];
    if( printResultsOnFailure && !printResults )
    {
      checkSolution(u,title,true,grid,maxVal,false);  // call again and print results
      aString msg=sPrintF(buff,"checkSolution:%s ERROR: nan or inf's found on grid=%i!",(const char*)title,grid);
      Overture::abort(msg);
    }
  }
  else 
  {
    if( printResults )
      printP("  checkSolution:%s No nan or inf's found on grid=%i. max(abs(u))=%8.2e\n",(const char*)title,grid,maxVal);
  }
  if( true )
  {
    fflush(0);
    Communication_Manager::Sync();
  }
  

  return num;
}

#undef UU
#undef MASK


