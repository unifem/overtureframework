#ifndef DATA_BASE_ACCESS_FUNCTIONS_H
#define DATA_BASE_ACCESS_FUNCTIONS_H

// Define some functions that get Grids from a database

extern const aString nullString;

class LoadBalancer;


int initializeMappingList();
int destructMappingList();

// read a grid from a file
int getFromADataBase(CompositeGrid & cg, 
		     aString & fileName, 
		     const aString & gridName=nullString,
                     const bool & checkTheGrid=false,
                     int printInfo =1 );

// read a grid from a file and load-balance using the provided LoadBalancer
int 
getFromADataBase(CompositeGrid & cg, 
		 aString & fileName, 
                 LoadBalancer & loadBalancer, 
		 const aString & gridName =nullString,
                 const bool & checkTheGrid =false,
                 int printInfo =1 );

// read a grid from a file and load-balance using the default LoadBalancer
int 
getFromADataBase(CompositeGrid & cg, 
		 aString & fileName, 
                 bool loadBalance, 
		 const aString & gridName=nullString,
                 const bool & checkTheGrid=false,
                 int printInfo =1 );

int findDataBaseFile( aString & fileName, 
                      const bool & searchCommonLocations=true,
                      int printInfo =1 );

#endif
