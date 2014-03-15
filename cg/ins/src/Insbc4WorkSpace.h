#ifndef INSBC4_WORK_SPACE_H
#define INSBC4_WORK_SPACE_H

#include "Overture.h"

// This class holds the "state" for the 4th order boundary conditions.
class Insbc4WorkSpace
{
public:
  Insbc4WorkSpace(){ first=NULL; pnibt=NULL; pnib=NULL; pibe=NULL; ptg=NULL; pct=NULL; pcn=NULL;} //
  ~Insbc4WorkSpace(){ destroy(); }
  void init( int numberOfGrids )
  { assert( first==NULL );
        first=new bool[numberOfGrids]; 
        for( int g=0; g<numberOfGrids; g++ ) first[g]=true;
        pnibt=new int[numberOfGrids]; 
        pnib=new int [3*2*2*numberOfGrids]; pibe=new IntegerArray[numberOfGrids]; 
        ptg=new RealArray[numberOfGrids]; pct=new RealArray[numberOfGrids]; 
        pcn=new RealArray[numberOfGrids];} //
  void destroy() { delete [] first; delete [] pnibt; delete [] pnib; delete [] pibe; delete [] ptg; 
                   delete [] pct; delete [] pcn;
                  first=NULL; pnibt=NULL; pnib=NULL; pibe=NULL; ptg=NULL; pct=NULL; pcn=NULL; }

  bool *first;
  int *pnibt;
  int *pnib;
  IntegerArray *pibe;
  RealArray *ptg; // tg(nd,2,ib)
  RealArray *pct; // ct(2,2,ib)
  RealArray *pcn; // cn(2,2,ib)
 
 
};

#endif
