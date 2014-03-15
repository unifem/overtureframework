//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

#include "Overture.h"
#include "HDF_DataBase.h"
#include "LoadBalancer.h"
#include "ParallelUtility.h"

extern const aString nullString;



int 
checkOverlappingGrid( const CompositeGrid & cg, const int & option=0, bool onlyCheckBaseGrids=true );

//\begin{>DataBaseAccessFunctionsInclude.tex}{\subsection{findDataBaseFile}}
int
findDataBaseFile(aString & fileName, 
                 const bool & searchCommonLocations /* =TRUE */,
                 int printInfo /* =1 */ )
// =======================================================================================
// /Description:
//   Attempt to locate an Overture database file.
//  Look for filename, fileName.hdf and fileName.show. If none of these are found look
// for the file in some other common locations.
// /filename (input/output) : on input, the name of the file (with or without an ".hdf" or ".show" at the end).
//       On output, the name of the actual file found (if found).
// /searchCommonLocations (input) : if TRUE, look for grids in some common locations.
// /Return values: 0 if found, 1 if not found.
// /Notes:
//    If the enironmental variable {\tt OvertureGridDirectories} is set then this routine will
// look for grids in the directories specified. The variable should be set to a sequence of colon separated
// directories as in
// \begin{verbatim}
//    setenv OvertureGridDirectories "/home/henshaw/sampleGrids:/home/henshaw/ogen"
// \end{verbatim}
//\end{DataBaseAccessFunctionsInclude.tex} 
// =======================================================================================
{
  const int myid=max(0,Communication_Manager::My_Process_Number);
  aString name=fileName;
  
   FILE *file = fopen(name,"r"); 
   if( file==0 )
   {
     name=fileName+".hdf";
     file = fopen(name,"r");
     if( file==0 )
     {
       name=fileName+".show"; 
       file = fopen(name,"r");

       if( file==0 && searchCommonLocations  )
       {
         // look for the file in some common locations

	 aString dirs,dir;
         char *env;
	 env=getenv("OvertureGridDirectories");
         if( env!=NULL )
           dirs=env;
         if( dirs!="" && printInfo>0 )
	   printF("Searching for grids in locations specified by the `OvertureGridDirectories' environment variable\n");
	   
         for( int m=0; m<10; m++ )
	 {
           if( dirs=="" )
             break;
           int length=dirs.length();
           int i=0;
	   while( i<length && dirs[i]!=':' ) i++;
	   dir=dirs(0,i-1);
	   name=dir + "/" + fileName;
	   if( printInfo>0 ) printF("look for %s \n",(const char*)name);
	   
	   if( findDataBaseFile( name,FALSE,printInfo )==0 )  // add FALSE to prevent further recursion
	   {
	     fileName=name;
	     return 0;
	   }

           if( i<length )
             dirs=dirs(i+1,length-1);
           else
             break;
	 }
         const int numberOfDirectories=4;
         const aString directory[numberOfDirectories]=
	 {
           OVERTURE_HOME "/sampleGrids/",
           "../sampleGrids/",
	   "../ogen/",
           OVERTURE_HOME "/../Overture/ogen/"
	 };
	 
         for( int n=0; n<numberOfDirectories; n++ )
	 {
	   name=directory[n] + fileName;
	   if( printInfo>0 ) printF("look for %s \n",(const char*)name);
	   
	   if( findDataBaseFile( name,FALSE,printInfo )==0 )  // add FALSE to prevent further recursion
	   {
	     fileName=name;
	     return 0;
	   }
	 }
	 
       }
     }
   }
   if( file!=0 )
   {
     fclose(file);
     fileName=name;
     return 0;
   }
   else
   {
     if( searchCommonLocations )
       printF("INFO: To have Overture search for grids in other locations you can set the \n"
              " environment variable OvertureGridDirectories to be a list of colon separated directories as in\n"
	      "  setenv OvertureGridDirectories \"/home/joeUser/sampleGrids:/home/janeUser/ogen\"\n");

     return 1;
   }
}


//\begin{>DataBaseAccessFunctionsInclude.tex}{\subsection{getFromADataBase(CompositeGrid \& cg,...)}}  
int 
getFromADataBase(CompositeGrid & cg, 
		 aString & fileName, 
		 const aString & gridName /* =nullString */,
                 const bool & checkTheGrid /* =FALSE */,
                 int printInfo /* =1 */ )
//==========================================================================================
// /Description:
//   Read in a CompositeGrid from a data-base file, generated for example by 
//  the Overture grid generator {\bf ogen}. To read in a grid from a previous version of 
//  Overture you may have to use the {\bf decompress} function (see below).
// /fileName (input/output) : on input the name of the data-base file. Currently only HDF data-base files are
//            supported. This routine will search for both fileName and fileName.hdf On output the full pathname
//            of the file that was found.
// /gridName (input) : optional name for the grid. For example, this is the name given to
//                     the grid if it was created with ogen. If no gridName is supplied
//       then the first CompositeGrid found in the file will be used.
// /checkTheGrid (input) : If TRUE the grid is checked for consistency such as valid interpolation
//   points etc. by calling the function {\tt checkOverlappingGrid}. 
// /printInfo (input) : 1=print info about the file being read, 0=silent
// /Return values: 0=success, 1=unable to open the file.
//
// /Remark: \index{decompress}\index{data-base files!conversion to new versions}
//    By default a data-base file made with one version of Overture will not be compatible with a 
//  newer version of Overture. The reason for this is that the data-base file is stored in a
// compressed way that makes it smaller in size and faster to read/write. 
// There is a mechanism for converting data-base files containing grids from one version
// of Overture to another. The program {\bf Overture/bin/decompress} will create a decompressed
// version of the grid file which can be read (more or less correctly) by new versions of
// Overture. For example, to convert from v18 to v19 you would run Overture.v18/bin/decompress
// on an overlapping grid built from v18. The resulting grid file can be read by Overture.v19.
// If you want to use this for v17 files you would have to copy the file Overture.v18/bin/decompress.C
// and compile it with v17.
//\end{DataBaseAccessFunctionsInclude.tex} 
//==========================================================================================
{
  int returnValue=0;
  const int myid=max(0,Communication_Manager::My_Process_Number);
  
  HDF_DataBase dataFile;

  aString name=fileName;
  int found=0;
  // This next routine will search for the file "name" in various locations and
  // return the full path name of the file that was found.
  if( findDataBaseFile( name,true,printInfo )==0 )
  {
    if( printInfo>0 && myid==0 ) cout << " ***** Mounting file " << name << "****\n";
    found = dataFile.mount(name, "R")==0;
  }
  // In parallel make sure that all processes found the same file
  int foundMax = ParallelUtility::getMaxValue(found);
  int foundMin = ParallelUtility::getMinValue(found);
  if( foundMax!=foundMin )
  {
    printF("getFromADataBase:ERROR: Not all processes found the file %s\n",(const char*)fileName);
    OV_ABORT("error");
  }
  if( found )
  {
    fileName=name; // return the full path name *wdh* 2011/12/10


    // we should hash the string and check it's value...
// djb2
// this algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c. another version of this algorithm (now favored by bernstein) uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33 (why it works better than many other constants, prime or not) has never been adequately explained.

//     unsigned long
//     hash(unsigned char *str)
//     {
//         unsigned long hash = 5381;
//         int c;

//         while (c = *str++)
//             hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

//         return hash;
//     }

  }
  

  // if the file was not found, print an error and return
  if( !found )
  {
    if( myid==0 )
    {
      cout << "getFromADataBase:ERROR: unable to open an old file = " 
	   << (const char *)fileName
	   << " (or " << (const char *)(fileName+".hdf") << " ), " 
	   << " (or " << (const char *)(fileName+".show") << " )" << endl;
    }
    return 1;
  }

  // check to see if this file was saved in streamMode (compressed)
  int streamMode=true; // use this as a default since this is the old way.
  dataFile.get(streamMode,"streamMode");

  if( !streamMode )
    dataFile.setMode(GenericDataBase::noStreamMode);

  if( gridName==nullString )
  {
    // find the first CompositeGrid to use
    aString name[10];  // **** fix this *****
    int num, actualNumber;
    num = dataFile.find(name,"CompositeGridData",10,actualNumber);  // **** should be CompositeGrid
    if( printInfo>0 && myid==0 ) 
      cout << "getFromADataBase: number of CompositeGrid(s) found =" << num << ", name[0]=" << name[0] << endl;

    real time=getCPU();

    cg.get(dataFile,name[0]);

    time=ParallelUtility::getMaxValue(getCPU()-time);
    if( printInfo>0 ) printF("Time to read in the grid is %8.2e(s)\n",time);
      
  }
  else
  {
    returnValue = cg.get(dataFile,gridName);
  }
  dataFile.unmount();
  if( checkTheGrid )
    checkOverlappingGrid(cg);   // check the grid for consistency
  
  return returnValue;
}


//\begin{>DataBaseAccessFunctionsInclude.tex}{\subsection{getFromADataBase(CompositeGrid \& cg,...,LoadBalancer,...)}}  
int 
getFromADataBase(CompositeGrid & cg, 
		 aString & fileName, 
                 LoadBalancer & loadBalancer, 
		 const aString & gridName /* =nullString */,
                 const bool & checkTheGrid /* =FALSE */,
                 int printInfo /* =1 */ )
//==========================================================================================
// /Description:
//   Read in a CompositeGrid from a data-base file and load-balance with a given
// LoadBalancer.
//\end{DataBaseAccessFunctionsInclude.tex} 
//==========================================================================================
{

  int returnValue=0;

  cg.setLoadBalancer(loadBalancer);  // use this LoadBalancer 

  // Read in the grid using the default load-balance (the mask array will be read in here)
  returnValue=getFromADataBase(cg,fileName,gridName,checkTheGrid,printInfo );

// old way: 
//   if( returnValue != 0 ) return returnValue;
  
//   const int np = max(1,Communication_Manager::numberOfProcessors());
//   if( np<=1 || cg.numberOfComponentGrids()==1 ) return returnValue;

//   if( printInfo ) printF("Load balance the grid and then read it in again using the computed load balance\n");

//   // Load-balance this grid
//   GridDistributionList & gridDistributionList = cg->gridDistributionList;

//   // work-loads per grid are based on the number of grid points by default:
//   loadBalancer.assignWorkLoads( cg,gridDistributionList );
//   loadBalancer.determineLoadBalance( gridDistributionList );

//   // Note: no need to read the grid in again if the load balance has not changed!

//   // now destroy the grid and read it in again (using the load balance computed above)
//   cg.destroy(CompositeGrid::EVERYTHING);  // this may be necessary
//   returnValue = getFromADataBase(cg,fileName,gridName,checkTheGrid,printInfo );

//   if( printInfo ) cg.displayDistribution("cg after reading again.");

  return returnValue;
}


//\begin{>DataBaseAccessFunctionsInclude.tex}{\subsection{getFromADataBase(CompositeGrid \& cg,...,loadBalance,...)}}  
int 
getFromADataBase(CompositeGrid & cg, 
		 aString & fileName, 
                 bool loadBalance, 
		 const aString & gridName /* =nullString */,
                 const bool & checkTheGrid /* =FALSE */,
                 int printInfo /* =1 */ )
//==================================================================================================
// /Description:
//   Read in a CompositeGrid from a data-base file and load-balance with the default LoadBalancer
//\end{DataBaseAccessFunctionsInclude.tex} 
//===================================================================================================
{
  // old way: 
  //   LoadBalancer loadBalancer;
  // int returnValue=getFromADataBase(cg,fileName,loadBalancer,gridName,checkTheGrid,printInfo );

  // Read in the grid using the default load-balance (the mask array will be read in here)
  int returnValue=getFromADataBase(cg,fileName,gridName,checkTheGrid,printInfo );
  return returnValue;
}
