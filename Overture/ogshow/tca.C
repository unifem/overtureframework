// ******************************************************************
//   Test routine for copyArray:
//     copy from a P++ array to a generally distributed array
//
//  mpirun -np 1 tca
//  mpirun -np 2 -dbg=valgrindebug tca
//  mpirun -tv -np 2 -all-local tca
//
//  srun -N1 -n2 -ppdebug tca
//  totalview srun -a -N1 -n2 -ppdebug tca
// 
// *******************************************************************
#include "ParallelUtility.h"
#include "display.h"

static int sent=0, received=0; 

void
displayPartiData(realArray & u, const aString & name)
{
  const int myid = Communication_Manager::My_Process_Number;
  const int np=max(1,Communication_Manager::Number_Of_Processors);

  DARRAY *uDArray = u.Array_Descriptor.Array_Domain.BlockPartiArrayDomain;
  DECOMP *uDecomp = uDArray->decomp;

  printf(" *************** %s: DARRAY uDArray->nDims=p=%i *******************\n"
         "  nDims=%i  : number of array dimensions\n",(const char*)name,myid,uDArray->nDims);
  
  int i;
  for( int i=0; i<uDArray->nDims; i++ )
  {
    printf(" myid=%i: dim=%i: internal-ghost-cells=%i, total-size=%i, left,central,right-size=[%i,%i,%i] \n"
           "                  global=[%i,%i] local-size=%i\n",myid,
	   i,uDArray->ghostCells[i],uDArray->dimVecG[i],
           uDArray->dimVecL_L[i],uDArray->dimVecL[i],uDArray->dimVecL_R[i],
           uDArray->g_index_low[i],uDArray->g_index_hi[i],uDArray->local_size[i]);
  }

  printf(" *************** u: DECOMP p=%i *******************\n"
         "  nDims=%i, nProcs=%i, baseProc=%i \n",
	 myid,uDecomp->nDims,uDecomp->nProcs,uDecomp->baseProc);

  for( int i=0; i<uDecomp->nDims; i++ )
  {
    printf(" myid=%i: dim=%i: dimVec=%i (size of decomposition) \n"
           "                  dimProc=%i (number of processors allocated to this dimension) \n",
	   myid,i,uDecomp->dimVec[i],uDecomp->dimProc[i]);
  }
}



int
testGeneralCopy()
// =========================================================================================
// 
// Test the copy function:
// 
// int copyArray( const doubleDistributedArray & u,
// 		  Index *Iv, 
// 		  IndexBox *vBox, // bounds of v on each processor, vBox[p] p=0,1,..,numProc-1
// 		  doubleSerialArray & vLocal );
// =========================================================================================
{
  const int myid = Communication_Manager::My_Process_Number;
  const int np=max(1,Communication_Manager::Number_Of_Processors);
  printf(" myid=%i, number of processors = %i\n",myid,np); 


  // Here is the source array that we will copy from: 
  const int nd=11;  // =10
  const int nd3=5;  // =10
  const int nDim=2;   // number of distributed dimensions
  // const int nDim=3;   // number of distributed dimensions
  

  // CopyArray::debug=1; // turn on debugging

  realArray u;
  if( nDim==2 )
  {
    u.redim(nd,nd);
  }
  else
  {
    u.redim(nd,nd,nd3);
  }
  
  u.seqAdd(0.,1.);
  

  Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  I1=Range(3,6);
  I2=Range(3,7);
  I3=Range(0,0);
  if( nDim==3 )
  {
    I3=Range(1,2);
  }
  Iv[3]=Range(0,0);
  
  const int maxDims=4, maxProc=16;

  // define dimensions of general distributed array:
  int vDim[2][maxDims][maxProc];
  for( int p=0; p<maxProc; p++ )
  for( int i=0; i<maxDims; i++ )
  {
    vDim[0][i][p]=0; vDim[1][i][p]=-1;  // base and bound
  }

  // The distribution of v does not need to be evenly spaced as with P++ arrays (currently)
  const int n3b = nDim==2 ? 0 : 3;
  int ng=1;  // ghost points
  if( np==1 )
  {
    // dimensions on processor p=0
    int p=0;
    vDim[0][0][p]=2; vDim[1][0][p]=7;
    vDim[0][1][p]=1; vDim[1][1][p]=8;
    vDim[0][2][p]=0; vDim[1][2][p]=n3b;
    vDim[0][3][p]=0; vDim[1][3][p]=0;
  }
  else if( np==2 )
  {
    if( true )
    {
      // dimensions on processor p=0
      int p=0;
      vDim[0][0][p]=2; vDim[1][0][p]=7;
      vDim[0][1][p]=1; vDim[1][1][p]=4+ng;   // include an overlap 
      vDim[0][2][p]=0; vDim[1][2][p]=n3b;
      vDim[0][3][p]=0; vDim[1][3][p]=0;

      // dimensions on processor p=1
      p=1;
      vDim[0][0][p]=2;    vDim[1][0][p]=7;
      vDim[0][1][p]=5-ng; vDim[1][1][p]=8;
      vDim[0][2][p]=0;    vDim[1][2][p]=n3b;
      vDim[0][3][p]=0;    vDim[1][3][p]=0;
    }
    else
    {
      // dimensions on processor p=0
      int p=0;
      vDim[0][0][p]=1; vDim[1][0][p]=2;
      vDim[0][1][p]=1; vDim[1][1][p]=2;  
      vDim[0][2][p]=0; vDim[1][2][p]=n3b;
      vDim[0][3][p]=0; vDim[1][3][p]=0;

    }
    
  }
  else if( np==3 )
  {
    // dimensions on processor p=0
    int p=0;
    vDim[0][0][p]=2; vDim[1][0][p]=7;
    vDim[0][1][p]=1; vDim[1][1][p]=4+ng;   // include an overlap
    vDim[0][2][p]=0; vDim[1][2][p]=n3b;
    vDim[0][3][p]=0; vDim[1][3][p]=0;

    // dimensions on processor p=1
    p=1;
    vDim[0][0][p]=2;    vDim[1][0][p]=7;
    vDim[0][1][p]=5-ng; vDim[1][1][p]=6+ng;
    vDim[0][2][p]=0;    vDim[1][2][p]=n3b;
    vDim[0][3][p]=0;    vDim[1][3][p]=0;

    p=2;
    vDim[0][0][p]=2;    vDim[1][0][p]=7;
    vDim[0][1][p]=7-ng; vDim[1][1][p]=8;
    vDim[0][2][p]=0;    vDim[1][2][p]=n3b;
    vDim[0][3][p]=0;    vDim[1][3][p]=0;
  }
  else 
  {
    // dimensions on processor p=0
    int p=0;
    vDim[0][0][p]=2;    vDim[1][0][p]=5+ng;
    vDim[0][1][p]=1;    vDim[1][1][p]=4+ng;
    vDim[0][2][p]=0;    vDim[1][2][p]=n3b;
    vDim[0][3][p]=0;    vDim[1][3][p]=0;

    // dimensions on processor p=1
    p=1;
    vDim[0][0][p]=2;    vDim[1][0][p]=5+ng;
    vDim[0][1][p]=5-ng; vDim[1][1][p]=8;
    vDim[0][2][p]=0;    vDim[1][2][p]=n3b;
    vDim[0][3][p]=0;    vDim[1][3][p]=0;

    p=2;
    vDim[0][0][p]=6-ng; vDim[1][0][p]=7;
    vDim[0][1][p]=1;    vDim[1][1][p]=4+ng;
    vDim[0][2][p]=0;    vDim[1][2][p]=n3b;
    vDim[0][3][p]=0;    vDim[1][3][p]=0;

    p=3;
    vDim[0][0][p]=6-ng; vDim[1][0][p]=7;
    vDim[0][1][p]=5-ng; vDim[1][1][p]=8;
    vDim[0][2][p]=0;    vDim[1][2][p]=n3b;
    vDim[0][3][p]=0;    vDim[1][3][p]=0;
  }
  if( myid==0 )
  {
    printf("=========================================================\n"
           " Distribution of v (destination array): \n");
    for( int p=0; p<np; p++ )
    {
      if( vDim[1][0][p] >= vDim[0][0][p] )
      {
	printf("   p=%i vLocal=[%i,%i][%i,%i][%i,%i][%i,%i]\n",p,
	       vDim[0][0][p],vDim[1][0][p],
	       vDim[0][1][p],vDim[1][1][p],
	       vDim[0][2][p],vDim[1][2][p],
	       vDim[0][3][p],vDim[1][3][p]); 
      }
    }
    printf("=========================================================\n");
  }
  

  realSerialArray v;  // Represents local array of a general distributed array
  // The arrays on each processor can be dimensioned arbitrarily
  if( vDim[1][0][myid] >= vDim[0][0][myid] )
  {
    v.redim(Range(vDim[0][0][myid],vDim[1][0][myid]),
            Range(vDim[0][1][myid],vDim[1][1][myid]),
            Range(vDim[0][2][myid],vDim[1][2][myid]));
    v=0.;
  }
  

    // Call the new function copyArray
  printf("\n\n");

  IndexBox *vBoxArray = new IndexBox[np]; 
  for( int p=0; p<np; p++ )
  {
    vBoxArray[p].setBounds(vDim[0][0][p],vDim[1][0][p], 
			   vDim[0][1][p],vDim[1][1][p], 
			   vDim[0][2][p],vDim[1][2][p], 
			   vDim[0][3][p],vDim[1][3][p] );
  }
  CopyArray::copyArray(u,Iv,vBoxArray,v); 
  delete [] vBoxArray;


  v.display("Here is v after copy");
  
  int ok=true;
  for( int i3=v.getBase(2); i3<=v.getBound(2); i3++ )
  for( int i2=v.getBase(1); i2<=v.getBound(1); i2++ )
  {
    for( int i1=v.getBase(0); i1<=v.getBound(0); i1++ )
    {
      if( i1>=I1.getBase() && i1<=I1.getBound() &&
          i2>=I2.getBase() && i2<=I2.getBound() &&
          i3>=I3.getBase() && i3<=I3.getBound()  )
      {
        real value=(i1+nd*(i2+nd*(i3)));
        if( fabs( v(i1,i2,i3)- value )>1.e-8 )
	{
	  ok=false;
	  printf("ERROR: myid=%i v(%i,%i,%i)=%5.2f is not equal to %5.2f\n",myid,i1,i2,i3,v(i1,i2,i3),value); 
	}
      }
    }
  }
  if( ok==true )
  {
    printf("* myid=%i All local entries in v appear to be correct *\n",myid);
  }


  // Bug test:  060824

  realArray u0(10,10);
  u0=1.;
  realArray u1;
  Partitioning_Type partition;
  partition.SpecifyProcessorRange(Range(min(1,np-1),np-1));
  u1.partition(partition);
  u1.redim(3,3);
  
  IndexBox uBox;
  for( int p=0; p<np; p++ )
  {
    CopyArray::getLocalArrayBox( p,u1,uBox );
    printf("myid=%i : p=%i uBox=[%i,%i][%i,%i]\n",myid,p,uBox.base(0),uBox.bound(0),uBox.base(1),uBox.bound(1));

    CopyArray::getLocalArrayBoxWithGhost( p,u1,uBox );
    printf("myid=%i : p=%i uBox=[%i,%i][%i,%i] (with ghost)\n",myid,p,
           uBox.base(0),uBox.bound(0),uBox.base(1),uBox.bound(1));
  }

  // Extract the Parti parallel array descriptors
  // PADRE version:
  if( false )
  {
    displayPartiData(u0,"u0");
    printf("\n\n");
    displayPartiData(u1,"u1");
  }
  

  int allOk=ok;
  MPI_Allreduce(&ok, &allOk, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
  if( allOk==true )
  {
    printf("**** Success: All entries in v appear to be correct *****\n",myid);
  }
  else
  {
    printf("**** Test failed: Not all entries in v are correct *****\n",myid);
  }

  return 0;
}

bool 
checkCopy( realArray & v, Index *D, realArray & u, Index *S, const aString & label )
{
  const real eps =REAL_EPSILON*100.;
  const int myid = max(0,Communication_Manager::My_Process_Number);

  realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
  realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);

  int numChecked=0;
  real maxErr=0.;
  for( int i3=D[3].getBase(), j3=S[3].getBase(); i3<=D[3].getBound();  i3+=D[3].getStride(), j3+=S[3].getStride() )
  {
    if( i3<vLocal.getBase(3) || i3>vLocal.getBound(3) ) continue;
    for( int i2=D[2].getBase(), j2=S[2].getBase(); i2<=D[2].getBound();  i2+=D[2].getStride(), j2+=S[2].getStride() )
    {
      if( i2<vLocal.getBase(2) || i2>vLocal.getBound(2) ) continue;
      for( int i1=D[1].getBase(), j1=S[1].getBase(); i1<=D[1].getBound();  i1+=D[1].getStride(), j1+=S[1].getStride() )
      {
	if( i1<vLocal.getBase(1) || i1>vLocal.getBound(1) ) continue;
	for( int i0=D[0].getBase(), j0=S[0].getBase(); i0<=D[0].getBound();  i0+=D[0].getStride(), j0+=S[0].getStride() )
	{
	  if( i0<vLocal.getBase(0) || i0>vLocal.getBound(0) ) continue;
    
          // real trueValue=uLocal(j0,j1,j2,j3);  // this wont work 
          numChecked++;
           // here we assume u was assigned this way ****************
          real trueValue = j0 + u.getLength(0)*( j1 + u.getLength(1)*(j2+ u.getLength(2)*(j3) )); 
	  if( fabs(vLocal(i0,i1,i2,i3)-trueValue)>maxErr )
	  {
	    maxErr=fabs(vLocal(i0,i1,i2,i3)-trueValue);
            printf("myid=%i ERR: vLocal(%i,%i,%i,%i)=%e (j0,j1,j2,j3)=(%i,%i,%i,%i) : trueValue=%e \n",
		   myid,i0,i1,i2,i3,vLocal(i0,i1,i2,i3),j0,j1,j2,j3,trueValue);
	  }
	}
      }
    }
  }
  
  maxErr = ParallelUtility::getMaxValue(maxErr);
  numChecked= ParallelUtility::getSum(numChecked); 	  

  printF("checkCopy: %s : maxErr=%9.2e, numChecked=%i\n",(const char*)label,maxErr,numChecked);
  if( maxErr<eps )
    return true;
  else
  {
    printF("checkCopy:ERROR: maxErr=%9.2e\n",maxErr);
    return false;
  }
  
}


int
testRegularSectionTransfer()
// ==============================================================================================
//   Test the function
// int copyArray( doubleArray & dest, Index *D, 
//	          const doubleArray &  src, Index *S, int nd=4 );
// ==============================================================================================
{
  int debug=1;

  const int myid = max(0,Communication_Manager::My_Process_Number);
  const int np=max(1,Communication_Manager::Number_Of_Processors);

  Partitioning_Type pu, pv;

  int ndp=1, numGhost=2;
  // Target processors (0:1)
  int up0=0, up1=min(1,np-1);
  pu.SpecifyProcessorRange(Range(up0,up1)); 
  pu.SpecifyDecompositionAxes(ndp);
  for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
  {
    if( d<ndp )
      pu.partitionAlongAxis(d, true, numGhost ); 
    else
      pu.partitionAlongAxis(d, false, 0 ); 
  }
  
  // Target processors (3:4)
  int vp0=min(max(0,np-2),3), vp1=min(np-1,4);
  pv.SpecifyProcessorRange(Range(vp0,vp1)); 
  pv.SpecifyDecompositionAxes(ndp);
  for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
  {
    if( d<ndp )
      pv.partitionAlongAxis(d, true, numGhost ); 
    else
      pv.partitionAlongAxis(d, false, 0 ); 
  }

  realArray u, v;
  u.partition(pu);  u.redim(7);
  v.partition(pv);  v.redim(10);
  realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
  realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);

  printF("\n ------ 1D ----------\n"
         " Source array u(%i:%i) processors (%i:%i) \n"
         " Dest   array v(%i:%i) processors (%i:%i)\n",
	 u.getBase(0),u.getBound(0),up0,up1,
	 v.getBase(0),v.getBound(0),vp0,vp1);

  for( int i0=uLocal.getBase(0); i0<=uLocal.getBound(0); i0++ )
    uLocal(i0)=i0;
  
  vLocal=0.;
  
  
  Index D[4], S[4];

  for( int d=0; d<4; d++ )
  {
    D[d]=Range(0,0); S[d]=Range(0,0); 
  }
  
  int numErr=0;
  int numTests=0;
  int startTest=0; // 5; // 0;  // skip this many tests 
  aString label;
  
  numTests++;
  if( startTest<=numTests )
  {
    
    S[0]=Range(1,4);
    D[0]=Range(1,4);

    CopyArray::copyArray( v,D,u,S );
    if( debug & 2 )
    {
      ::display(v,"v after copy, v(1:4)=u(1:4)");
      ::display(vLocal,sPrintF("vLocal on p=%i",myid),"%4.1f" );
      ::display(uLocal,sPrintF("uLocal on p=%i",myid),"%4.1f" );
    }
    
    bool ok = checkCopy( v,D,u,S,"v(1:4)=u(1:4)" );
    if( !ok ) numErr++;
  }
  
  numTests++;
  if( startTest<=numTests )
  {
    vLocal=0.;
    S[0]=Range(1,4);
    D[0]=Range(6,9);
    CopyArray::copyArray( v,D,u,S );

    if( debug & 2 )
    {
      ::display(v,"v after copy, v(6:9)=u(1:4)");
      ::display(vLocal,sPrintF("vLocal on p=%i",myid),"%4.1f" );
    }
      
    // if( myid==0 ) vLocal(6)=0.;
    // if( myid==1 ) vLocal(7)=0.;
    
    bool ok = checkCopy( v,D,u,S,"v(6:9)=u(1:4)" );
    if( !ok ) numErr++;
  }

  numTests++;
  if( startTest<=numTests )
  {
    vLocal=0.;
    S[0]=Range(1,5,2);
    D[0]=Range(2,8,3);
    CopyArray::copyArray( v,D,u,S );

    if( debug & 2 )
    {
      ::display(v,"v after copy, v(2:8:3)=u(1:5:2)");
      ::display(vLocal,sPrintF("vLocal on p=%i",myid),"%4.1f" );
    }
      
    bool ok = checkCopy( v,D,u,S,"v(2:8:3)=u(1:5:2)" );
    if( !ok ) numErr++;
  }

  

  // **** 2D *****
  Partitioning_Type pu2d, pv2d;
  u.redim(0);  v.redim(0);
    
  ndp=2, numGhost=2;
  up0=0, up1=min(np-1,np/2+1);
  pu2d.SpecifyProcessorRange(Range(up0,up1)); 
  pu2d.SpecifyDecompositionAxes(ndp);
  for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
  {
    if( d<ndp )
      pu2d.partitionAlongAxis(d, true, numGhost ); 
    else
      pu2d.partitionAlongAxis(d, false, 0 ); 
  }
  
  vp0=min(max(0,np-2),max(1,np/2-2)), vp1=np-1;
  pv2d.SpecifyProcessorRange(Range(vp0,vp1)); 
  pv2d.SpecifyDecompositionAxes(ndp);
  for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
  {
    if( d<ndp )
      pv2d.partitionAlongAxis(d, true, numGhost ); 
    else
      pv2d.partitionAlongAxis(d, false, 0 ); 
  }


  u.partition(pu2d);
  u.redim(Range(-1,11),Range(-2,13));
  
  v.partition(pv2d);
  v.redim(Range(1,21),Range(0,17));
  
  getLocalArrayWithGhostBoundaries(u,uLocal);
  getLocalArrayWithGhostBoundaries(v,vLocal);

  printF("\n ------ 2D ----------\n"
	 " Source array u(%i:%i,%i:%i) processors (%i:%i) \n"
         " Dest   array v(%i:%i,%i:%i) processors (%i:%i)\n",
	 u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1),
         up0,up1,
	 v.getBase(0),v.getBound(0),v.getBase(1),v.getBound(1),
         vp0,vp1);

  for( int i1=uLocal.getBase(1); i1<=uLocal.getBound(1); i1++ )
  for( int i0=uLocal.getBase(0); i0<=uLocal.getBound(0); i0++ )
    uLocal(i0,i1)=i0+u.getLength(0)*(i1);

  numTests++;
  if( startTest<=numTests )
  {
    vLocal=0.;
    S[0]=Range(1,5);  S[1]=Range(4,7);
    D[0]=Range(4,8);  D[1]=Range(0,3);
    CopyArray::copyArray( v,D,u,S );

    sPrintF(label,"v(%i:%i:%i, %i:%i:%i)=u(%i:%i:%i, %i:%i:%i)",
	    D[0].getBase(),D[0].getBound(),D[0].getStride(),
	    D[1].getBase(),D[1].getBound(),D[1].getStride(),
	    S[0].getBase(),S[0].getBound(),S[0].getStride(),
	    S[1].getBase(),S[1].getBound(),S[1].getStride());

    if( debug & 2 )
    {
      ::display(v,"v after copy,"+label);
      ::display(vLocal,sPrintF("vLocal on p=%i",myid),"%5.0f" );
    }
      
    bool ok = checkCopy( v,D,u,S,label );
    if( !ok ) numErr++;
  }

  numTests++;
  if( startTest<=numTests )
  {
    vLocal=0.;
//     S[0]=Range(2,2,1);  S[1]=Range(4,7,3);
//     D[0]=Range(2,2,1);  D[1]=Range(4,7,3);

//     S[0]=Range(2,2,1);  S[1]=Range(-1,7,4);
//     D[0]=Range(2,2,1);  D[1]=Range(15,17,1);

// failed:
//  S[0]=Range(2,4,2);  S[1]=Range(-1,7,4);
//  D[0]=Range(6,9,3);  D[1]=Range(15,17,1);

    S[0]=Range(6,9,3);  S[1]=Range(-1,7,4);
    D[0]=Range(6,9,3);  D[1]=Range(15,17,1);

    CopyArray::copyArray( v,D,u,S );

    sPrintF(label,"v(%i:%i:%i, %i:%i:%i)=u(%i:%i:%i, %i:%i:%i)",
	    D[0].getBase(),D[0].getBound(),D[0].getStride(),
	    D[1].getBase(),D[1].getBound(),D[1].getStride(),
	    S[0].getBase(),S[0].getBound(),S[0].getStride(),
	    S[1].getBase(),S[1].getBound(),S[1].getStride());

    if( debug & 2 )
    {
      ::display(v,"v after copy"+label);
      ::display(vLocal,sPrintF("vLocal on p=%i",myid),"%5.0f" );
    }
      
    bool ok = checkCopy( v,D,u,S,label );
    if( !ok ) numErr++;
  }

  numTests++;
  if( startTest<=numTests )
  {
    vLocal=0.;
    S[0]=Range(2,8,2);  S[1]=Range(-1,7,4);
    D[0]=Range(6,15,3);  D[1]=Range(15,17,1);
    CopyArray::copyArray( v,D,u,S );

    sPrintF(label,"v(%i:%i:%i, %i:%i:%i)=u(%i:%i:%i, %i:%i:%i)",
	    D[0].getBase(),D[0].getBound(),D[0].getStride(),
	    D[1].getBase(),D[1].getBound(),D[1].getStride(),
	    S[0].getBase(),S[0].getBound(),S[0].getStride(),
	    S[1].getBase(),S[1].getBound(),S[1].getStride());

    if( debug & 2 )
    {
      ::display(v,"v after copy"+label);
      ::display(vLocal,sPrintF("vLocal on p=%i",myid),"%5.0f" );
    }
      
    bool ok = checkCopy( v,D,u,S,label );
    if( !ok ) numErr++;
  }



  // **** 3D *****
  Partitioning_Type pu3d, pv3d;
  u.redim(0);  v.redim(0);
    
  ndp=3, numGhost=2;
  up0=0, up1=np-1;
  pu3d.SpecifyProcessorRange(Range(up0,up1)); 
  pu3d.SpecifyDecompositionAxes(ndp);
  for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
  {
    if( d<ndp )
      pu3d.partitionAlongAxis(d, true, numGhost ); 
    else
      pu3d.partitionAlongAxis(d, false, 0 ); 
  }
  
  vp0=min(max(0,np-2),max(1,np/3-2)), vp1=np-1;
  pv3d.SpecifyProcessorRange(Range(vp0,vp1)); 
  pv3d.SpecifyDecompositionAxes(ndp);
  for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
  {
    if( d<ndp )
      pv3d.partitionAlongAxis(d, true, numGhost ); 
    else
      pv3d.partitionAlongAxis(d, false, 0 ); 
  }


  u.partition(pu3d);
  u.redim(Range(-2,11),Range(-3,10),Range(-1,12));
  
  v.partition(pv3d);
  v.redim(Range(0,17),Range(-1,15),Range(-2,9));
  
  getLocalArrayWithGhostBoundaries(u,uLocal);
  getLocalArrayWithGhostBoundaries(v,vLocal);

  printF("\n ------ 3D ----------\n"
	 " Source array u(%i:%i,%i:%i,%i:%i) processors (%i:%i) \n"
         " Dest   array v(%i:%i,%i:%i,%i:%i) processors (%i:%i)\n",
	 u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1),u.getBase(2),u.getBound(2),
         up0,up1,
	 v.getBase(0),v.getBound(0),v.getBase(1),v.getBound(1),v.getBase(2),v.getBound(2),
         vp0,vp1);

  for( int i2=uLocal.getBase(2); i2<=uLocal.getBound(2); i2++ )
  for( int i1=uLocal.getBase(1); i1<=uLocal.getBound(1); i1++ )
  for( int i0=uLocal.getBase(0); i0<=uLocal.getBound(0); i0++ )
    uLocal(i0,i1,i2)=i0+u.getLength(0)*(i1+u.getLength(1)*(i2));

  numTests++;
  if( startTest<=numTests )
  {
    vLocal=0.;
    S[0]=Range(0,8,2);  S[1]=Range(-1,5,1); S[2]=Range(0,8,4);
    D[0]=Range(4,8,1);  D[1]=Range(0,12,2); D[2]=Range(5,7,1);
    CopyArray::copyArray( v,D,u,S );

    sPrintF(label,"v(%i:%i:%i, %i:%i:%i, %i:%i:%i)=u(%i:%i:%i, %i:%i:%i, %i:%i:%i)",
	    D[0].getBase(),D[0].getBound(),D[0].getStride(),
	    D[1].getBase(),D[1].getBound(),D[1].getStride(),
	    D[2].getBase(),D[2].getBound(),D[2].getStride(),
	    S[0].getBase(),S[0].getBound(),S[0].getStride(),
	    S[1].getBase(),S[1].getBound(),S[1].getStride(),
	    S[2].getBase(),S[2].getBound(),S[2].getStride());

    if( debug & 2 )
    {
      ::display(v,"v after copy"+label);
      ::display(vLocal,sPrintF("vLocal on p=%i",myid),"%5.0f" );
    }
      
    bool ok = checkCopy( v,D,u,S,label );
    if( !ok ) numErr++;
  }




  u.partition(pu3d);
  u.redim(Range(-2,11),Range(-3,10),Range(-1,12),Range(2,2));
  
  v.partition(pv3d);
  v.redim(Range(0,17),Range(-1,15),Range(-2,9),Range(1,1));
  
  printF(" **** u.numberOfDimensions()=%i\n",u.numberOfDimensions());
  printF(" **** v.numberOfDimensions()=%i\n",v.numberOfDimensions());
  

  getLocalArrayWithGhostBoundaries(u,uLocal);
  getLocalArrayWithGhostBoundaries(v,vLocal);

  printF("\n ------ 3D+1 ----------\n"
	 " Source array u(%i:%i,%i:%i,%i:%i,%i:%i) processors (%i:%i) \n"
         " Dest   array v(%i:%i,%i:%i,%i:%i,%i:%i) processors (%i:%i)\n",
	 u.getBase(0),u.getBound(0),u.getBase(1),u.getBound(1),u.getBase(2),u.getBound(2),u.getBase(3),u.getBound(3),
         up0,up1,
	 v.getBase(0),v.getBound(0),v.getBase(1),v.getBound(1),v.getBase(2),v.getBound(2),v.getBase(3),v.getBound(3),
         vp0,vp1);

  for( int i3=uLocal.getBase(3); i3<=uLocal.getBound(3); i3++ )
  for( int i2=uLocal.getBase(2); i2<=uLocal.getBound(2); i2++ )
  for( int i1=uLocal.getBase(1); i1<=uLocal.getBound(1); i1++ )
  for( int i0=uLocal.getBase(0); i0<=uLocal.getBound(0); i0++ )
    uLocal(i0,i1,i2,i3)=i0+u.getLength(0)*(i1+u.getLength(1)*(i2+u.getLength(2)*(i3))) ;

  numTests++;
  if( startTest<=numTests )
  {
    vLocal=0.;
    S[0]=Range(0,8,2);  S[1]=Range(-1,5,1); S[2]=Range(0,8,4); S[3]=u.dimension(3);
    D[0]=Range(4,8,1);  D[1]=Range(0,12,2); D[2]=Range(5,7,1); D[3]=v.dimension(3);
    CopyArray::copyArray( v,D,u,S );

    sPrintF(label,"v(%i:%i:%i, %i:%i:%i, %i:%i:%i, %i:%i:%i)=u(%i:%i:%i, %i:%i:%i, %i:%i:%i, %i:%i:%i)",
	    D[0].getBase(),D[0].getBound(),D[0].getStride(),
	    D[1].getBase(),D[1].getBound(),D[1].getStride(),
	    D[2].getBase(),D[2].getBound(),D[2].getStride(),
	    D[3].getBase(),D[3].getBound(),D[3].getStride(),
	    S[0].getBase(),S[0].getBound(),S[0].getStride(),
	    S[1].getBase(),S[1].getBound(),S[1].getStride(),
	    S[2].getBase(),S[2].getBound(),S[2].getStride(), 
	    S[3].getBase(),S[3].getBound(),S[3].getStride());

    if( debug & 2 )
    {
      ::display(v,"v after copy"+label);
      ::display(vLocal,sPrintF("vLocal on p=%i",myid),"%5.0f" );
    }
      
    bool ok = checkCopy( v,D,u,S,label );
    if( !ok ) numErr++;
  }





  // ---------------------------------------------------------------------------------------------------
  numTests-=startTest;
  fflush(0);
  MPI_Barrier(MPI_COMM_WORLD);
  if( numErr==0 )
  {
    printF("\n ========= np=%i, %i tests were successful! No errors were found ===========\n",np,numTests);
  }
  else
  {
    printF("\n ========= Some tests failed! %i tests passed, %i tests failed ===========\n",numTests-numErr,numErr);
  }
  return 0;
 
}


int
testSerialCopyArray()
// ==============================================================================================
//   Test the serial-array copy functions
// ==============================================================================================
{
  int debug=1;

  const int myid = max(0,Communication_Manager::My_Process_Number);
  const int np=max(1,Communication_Manager::Number_Of_Processors);


//  intSerialArray src(1), dest(1); 
  realSerialArray src, dest; 
  
  int srcProc= np-1;
  int destProc=0;
  
  fflush(0);
  Communication_Manager::Sync();

  printF("Copy a serial array from srcProc=%i to destProc=%i\n",srcProc,destProc);

  if( myid==srcProc )
  {
    if( true )
    {
      const int n1=10, n2=2;
      src.redim(n1,n2);
      for( int i=0; i<n1; i++ )
      {
	src(i,0)=  i+1;
	src(i,1)=-(i+1);
      }
    }
    
    ::display(src,sPrintF("Here is src, srcProc=%i",srcProc));
  }
  
  CopyArray::copyArray( dest,destProc, src,srcProc );
  
  if( myid==destProc )
  {
    ::display(dest,sPrintF("Here is dest, destProc=%i",destProc));
  }
  


  return 0;

}

// ==========================================================================================
// Test the copy of serial arrays to distributed arrays:
//       "v = uLocal"
// ==========================================================================================
int
testSerialToDistributedCopy()
{
  int debug=1;

  const int myid = max(0,Communication_Manager::My_Process_Number);
  const int np=max(1,Communication_Manager::Number_Of_Processors);

  int nDim=2;
//   int nd=5, nd3=2;
  int nd=7, nd3=2;

  Partitioning_Type pu, pv;

  int ndp=2, numGhost=2;
  // Target processors (0:1)
//  int up0=0, up1=min(1,np-1);
//  int up0=0, up1=0;
//  int up0=0, up1=0;
//   int up0=1, up1=2;
//  int up0=0, up1=max(0,np-2); 
  int up0=0, up1=max(0,np-3); 
  pu.SpecifyProcessorRange(Range(up0,up1)); 
  pu.SpecifyDecompositionAxes(ndp);
  for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
  {
    if( d<ndp )
      pu.partitionAlongAxis(d, true, numGhost ); 
    else
      pu.partitionAlongAxis(d, false, 0 ); 
  }
  
  
  // int vp0=min(max(0,np-2),3), vp1=min(np-1,4);  // Target processors (3:4)
  // int vp0=max(np-1,0), vp1=min(np-1,4);
  // int vp0=0, vp1=np-1;
  int vp0=max(0,np-3), vp1=np-1;
  pv.SpecifyProcessorRange(Range(vp0,vp1)); 
  pv.SpecifyDecompositionAxes(ndp);
  for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
  {
    if( d<ndp )
      pv.partitionAlongAxis(d, true, numGhost ); 
    else
      pv.partitionAlongAxis(d, false, 0 ); 
  }

  realArray u, v;
  u.partition(pu);  
  v.partition(pv); 

  if( nDim==2 )
  {
    u.redim(nd,nd);
    v.redim(nd,nd);
  }
  else
  {
    u.redim(nd,nd,nd3);
    v.redim(nd,nd,nd3);
  }
  
  const intSerialArray & vProcessorSet = v.getPartition().getProcessorSet();

  if( false )
  {
    ::display(vProcessorSet,"vProcessorSet");
    displayPartiData(v,"v");
  }
  

  u.seqAdd(0.,1.);
  v=-.1;

  realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);

  Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2], &I4=Iv[3];
  Index Jv[4], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2], &J4=Jv[3];

  for( int d=0; d<4; d++ )
  {
    Iv[d]=v.dimension(d);
    Jv[d]=uLocal.dimension(d);
  }

  int includeGhost=0; 
  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,J1,J2,J3,includeGhost);

  const intSerialArray & uProcessorSet = u.getPartition().getProcessorSet();

  CopyArray::copyArray( uLocal,Jv,uProcessorSet, v, Iv );
  

  ::display(v,"v after copy","%5.2f ");
  realArray w;
  w.partition(pv);
  w.redim(nd,nd);
  w.seqAdd(0.,1.);
  w=w-v;
  ::display(w,"error after copy","%5.2f ");

  return 0;
}



int
main( int argc, char *argv[])
{
  Index::setBoundsCheck(on);
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems

  int Number_Of_Processors = 0;
  Optimization_Manager::Initialize_Virtual_Machine ((char*)"", Number_Of_Processors, argc, argv);

  Overture::OV_COMM = MPI_COMM_WORLD; // ************ do this *********


  fflush(0);
  Communication_Manager::Sync();
  fflush(0);

  const int myid = Communication_Manager::My_Process_Number;
  const int np=max(1,Communication_Manager::Number_Of_Processors);
  // printf(" myid=%i, number of processors = %i\n",myid,np); 


  // Index::setBoundsCheck(off);
  // Optimization_Manager::setForceVSG_Update(Off);
  

  if( false )
  {
    Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);
    testGeneralCopy();
  }
  else if( false )
  {
    // Bug from CompositeGrid:
    Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);

    Partitioning_Type partition2;
    partition2.SpecifyDecompositionAxes(1);
    const int numGhost=0;
    partition2.partitionAlongAxis(1, TRUE, numGhost );

    intArray w;
    w.partition(partition2);
    w.redim(10,2);
    w=0;
    IndexBox uBox;
    for( int p=0; p<np; p++ )
    {
      CopyArray::getLocalArrayBox( p,w,uBox );
      printf("myid=%i : p=%i wBox=[%i,%i][%i,%i]\n",myid,p,uBox.base(0),uBox.bound(0),uBox.base(1),uBox.bound(1));
    }
    
  }
  else if( false )
  {
    testRegularSectionTransfer();
  }
  else if( false )
  {
    testSerialCopyArray();
  }
  
  else 
  { 
    testSerialToDistributedCopy();
  }
  

  Optimization_Manager::Exit_Virtual_Machine();
  return 0;
}
