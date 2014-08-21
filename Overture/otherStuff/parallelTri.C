// --- Test the parallel Tridiagonal solver ---

// === HERE IS A FIRST VERSION ===
//
// mpiexec -n 1 parallelTri
// mpiexec -n 1 xterm -e gdb ./parallelTri
// 
// -- run with xterms in gdb but suppress some xterms:
// mpiexec -n 1 xterm -e gdb ./parallelTri : -n 3 ./parallelTri
// mpiexec -n 1 xterm -e gdb ./parallelTri : -n 2 ./parallelTri : -n 1 xterm -e gdb ./parallelTri
// mpiexec -n 1 ./parallelTri : -n 1 xterm -e gdb ./parallelTri : -n 1 ./parallelTri : -n 1 ./parallelTri
//
// mpiexec -n 1 xterm -e valgrind --suppressions=/home/henshw/valgrind.supp -q --db-attach=yes ./parallelTri
// 
// mpiexec -n 1 valgrind --suppressions=/home/henshw/valgrind.supp -q --db-attach=yes ./parallelTri
// 
// mpiexec -n 1 xterm -e valgrind --gen-suppressions=yes --suppressions=/home/henshw/valgrind.supp -q ./parallelTri
//
#include "Overture.h"
#include "TridiagonalSolver.h"
#include "display.h"
#include "ParallelUtility.h"

#ifndef OV_USE_DOUBLE
#define MPI_Real_Type MPI_FLOAT
#else
#define MPI_Real_Type MPI_DOUBLE
#endif

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


#define FOR_BLOCK(b1,b2)\
for( int b1=0; b1<blockSize; b1++)\
for( int b2=0; b2<blockSize; b2++) 


  // ======================================================================================
/// \brief Get local array bounds for an array where the 3 partitioned dimensions I1,I2,I3
///  are offset from the usual positions [0,1,2] to be instead [indexOffset,indexOffset+1,indexOffset+2]
// 
// ======================================================================================
static bool
getLocalArrayBounds(realArray & a , realSerialArray & aLocal, Index & I1, Index & I2, Index & I3,
                    const int includeGhost, const int indexOffset=0 )
{
  bool ok=true;

  if( indexOffset==0 )
  {
    ok= ParallelUtility::getLocalArrayBounds(a,aLocal,I1,I2,I3,includeGhost);
  }
  else
  {
    for( int d=0; d<3; d++ )
    {
      Index &I = d==0 ? I1 : d==1 ? I2 : I3;
      int na,nb;
      if( includeGhost )
      {
	// include parallel ghost boundaries
	na = max(I.getBase() , aLocal.getBase(indexOffset+d));
	nb = min(I.getBound(),aLocal.getBound(indexOffset+d));
      }
      else
      {
	// do not include parallel ghost boundaries
	na = max(I.getBase() , aLocal.getBase(indexOffset+d)+a.getGhostBoundaryWidth(indexOffset+d));
	nb = min(I.getBound(),aLocal.getBound(indexOffset+d)-a.getGhostBoundaryWidth(indexOffset+d));
      }
      if( nb>=na )
      {
        I=Range(na,nb);
      }
      else
      {
	ok=false;
	break;
      }
      
    }
  }
  return ok;
}

// =============================================================================================
/// \brief Print the parallel decomposition
// =============================================================================================
int
printParallelDecomposition( const realArray & u, const aString & label )
{
#ifdef USE_PPP
#ifndef USE_PADRE
  DARRAY *uDArray = u.Array_Descriptor.Array_Domain.BlockPartiArrayDomain;
#else
  // Padre version:
  DARRAY *uDArray = u.Array_Descriptor.Array_Domain.getParallelPADRE_DescriptorPointer()->representation->
    pPARTI_Representation->BlockPartiArrayDescriptor; 
#endif
  DECOMP *uDecomp = uDArray->decomp;

  const int numDim=min(u.numberOfDimensions(),MAX_DISTRIBUTED_DIMENSIONS);
  const int baseProc = uDecomp->baseProc; 

  printF(" ------Parallel decomposition for %s: ",(const char*)label);
  for( int d=0; d<numDim; d++ )
  {
    printF("dimProc[%i]=%i, ",d,uDecomp->dimProc[d]);
  }
  printF("\n");
#endif
  
 return 0;
}



int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture

  const int myid = max(0,Communication_Manager::My_Process_Number);
  const int np=max(1,Communication_Manager::Number_Of_Processors);

  real worstError=0.;      // worst error over a class of tests 
  real veryWorstError=0.;  // worst error over all tests 

  TridiagonalSolver tri;

//  Test non-block systems

  Partitioning_Type partition;
  int numGhost=1, ndp=3;
  partition.SpecifyProcessorRange(np);
  partition.SpecifyDecompositionAxes(ndp);
  for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
  {
    if( d<ndp )
      partition.partitionAlongAxis(d, true, numGhost ); 
    else
      partition.partitionAlongAxis(d, false, 0 ); 
  }


  const int nz=1;
  Range D1(0,11), D2(0,21), D3(0,nz-1);
  realArray a,b,c,u,r;
  a.partition(partition);
  b.partition(partition);
  c.partition(partition);
  u.partition(partition);
  r.partition(partition);

  a.redim(D1,D2,D3); b.redim(D1,D2,D3); c.redim(D1,D2,D3);
  u.redim(D1,D2,D3); r.redim(D1,D2,D3);

  printParallelDecomposition(a,"a,b,c");


  int axis=0;
//  cout << "Enter axis to solve along (0,1,2)\n";
//  cin >> axis;

  OV_GET_SERIAL_ARRAY(real,a,aLocal);
  OV_GET_SERIAL_ARRAY(real,b,bLocal);
  OV_GET_SERIAL_ARRAY(real,c,cLocal);
  OV_GET_SERIAL_ARRAY(real,u,uLocal);
  OV_GET_SERIAL_ARRAY(real,r,rLocal);
  

  Index I1,I2,I3;
  int includeGhost=0; 
  I1=D1, I2=D2, I3=D3;
  bool ok = ParallelUtility::getLocalArrayBounds(a,aLocal,I1,I2,I3,includeGhost);

  if( true )
  {
    int base =I1.getBase();
    int bound=I1.getBound();
    int i1;
    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
    {
      aLocal(i1,I2,I3)=  -(i1+1);
      bLocal(i1,I2,I3)= 4*(i1+1);
      cLocal(i1,I2,I3)=-2*(i1+1);
      rLocal(i1,I2,I3)=   (i1+1);
    }
    // adjust ends 
    if( aLocal.getBase(0)<=a.getBase(0) )
      rLocal(base,I2,I3) -=aLocal(base,I2,I3);
    if( aLocal.getBound(0)>=a.getBound(0) )
      rLocal(bound,I2,I3)-=cLocal(bound,I2,I3);
  
    tri.factor(a,b,c,TridiagonalSolver::normal,axis);

    tri.solve(r,I1,I2,I3);

  
    real error = max(abs(rLocal(I1,I2,I3)-1.));
    error = ParallelUtility::getMaxValue(error);

    worstError=max(worstError,error);
    printF(" ****maximum error=%8.2e in the normal case.\n",error);
    // r.display("Here is the solution, should be 1");
  


    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
    {
      aLocal(i1,I2,I3)=  -(i1+1);
      bLocal(i1,I2,I3)= 4*(i1+1);
      cLocal(i1,I2,I3)=-2*(i1+1);
      rLocal(i1,I2,I3)=   (i1+1);
    }
  
    tri.factor(a,b,c,TridiagonalSolver::extended,axis);
    tri.solve(r);

    // r.display("Here is the solution from the extended system, should be 1");
    error = max(abs(rLocal(I1,I2,I3)-1.));
    worstError=max(worstError,error);
    printF(" ****maximum error=%8.2e in the extended case.\n",error);
  


    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
    {
      aLocal(i1,I2,I3)=  -(i1+1);
      bLocal(i1,I2,I3)= 4*(i1+1);
      cLocal(i1,I2,I3)=-2*(i1+1);
      rLocal(i1,I2,I3)=   (i1+1);
    }

    tri.factor(a,b,c,TridiagonalSolver::periodic,axis);
    tri.solve(r);
   
    // r.display("Here is the solution from the periodic system, should be 1");
    error = max(abs(rLocal(I1,I2,I3)-1.));
    worstError=max(worstError,error);
    printF(" ****maximum error=%8.2e in the periodic case.\n",error);

    if( worstError < REAL_EPSILON*1000. )
    {
      printF("\n======== Test 1 successful. Worst error = %e ============\n",worstError);
    }
    else
    {
      printF("\n******** Test 1 apparently FAILED Worst error = %e ***************\n",worstError);
    }

  }
  // r.display("Here is the solution from the periodic system, should be 1");



  printF("\n\n"
         "******************************************************************************\n"
         "**********************  tridiagonal BLOCK systems **********************************\n"
         "******************************************************************************\n\n");

  veryWorstError=max(veryWorstError,worstError);
  worstError=0.;

  // ---------- test the block tridiagonal solvers ------------------

  // For block partitions we combine the block into a single index:
  //      a(b1,b2,i1,i2,i3) --> a(BB(b1,b2),i1,i2,i3)
  Partitioning_Type blockPartition;
  numGhost=1, ndp=4;
  blockPartition.SpecifyProcessorRange(np);
  blockPartition.SpecifyDecompositionAxes(ndp);
  for( int d=0; d<MAX_ARRAY_DIMENSION; d++ )
  {
    if( d>=1 && d<ndp )
      blockPartition.partitionAlongAxis(d, true, numGhost );  // partition dimensions 1,2,3
    else
      blockPartition.partitionAlongAxis(d, false, 0 );        // dimensions 0 NOT partitioned
  }

  const int numBlockTests=2; // 4;
  for( int ii=0; ii<numBlockTests; ii++ )  
  {
    const int axis=max(0,ii-1);
    const int m=12; // m=6;

    // const int m1a=0, m1b=2;  // number of tridiag systems in the first tangential direction
    const int m1a=3, m1b=12;  // number of tridiag systems in the first tangential direction
    // const int m2a=0, m2b=1;  // number of tridiag systems in the first tangential direction
    const int m2a=2, m2b=3;  // number of tridiag systems in the first tangential direction

    Range D1(0,0),D2(0,0),D3(0,0);
    int is[3]={0,0,0};
    if( ii==0 )
    {
      // single 1d system
      D1=Range(0,m); 
    }
    else if( ii==1 )
    {
      // multiple TRID systems, axis==axis1
      D1=Range(0,m); 
      D2=Range(m1a,m1b); 
      D3=Range(m2a,m2b); 
    }
    else if( ii==2 )
    {
      // multiple TRID systems, axis==axis2
      D1=Range(m1a,m1b); 
      D2=Range(0,m); 
      D3=Range(m2a,m2b); 
    }
    else 
    {
      // multiple TRID systems, axis==axis3
      D1=Range(m1a,m1b); 
      D2=Range(m2a,m2b); 
      D3=Range(0,m); 
    }
      
#undef BB
#define BB(b1,b2) ((b1)+block*(b2))
    for( int system=0; system<3; system++ )
    {
      TridiagonalSolver::SystemType systemType = (TridiagonalSolver::SystemType)system;
      for( int block=2; block<=3; block++ )
      {
        const int blockDim=block*block;
	Range B(0,block-1);
	Range BD(0,blockDim-1);
        realArray a,b,c,r,x;
	a.partition(blockPartition);
	b.partition(blockPartition);
	c.partition(blockPartition);
	r.partition(blockPartition);
	x.partition(blockPartition);

	a.redim(BD,D1,D2,D3);
        b.redim(BD,D1,D2,D3);
        c.redim(BD,D1,D2,D3);
        r.redim(B,D1,D2,D3);
        x.redim(B,D1,D2,D3);
    
	OV_GET_SERIAL_ARRAY(real,a,aLocal);
	OV_GET_SERIAL_ARRAY(real,b,bLocal);
	OV_GET_SERIAL_ARRAY(real,c,cLocal);
	OV_GET_SERIAL_ARRAY(real,x,xLocal);
	OV_GET_SERIAL_ARRAY(real,r,rLocal);

        printParallelDecomposition(a,"block: a,b,c");

        Index I1=D1, I2=D2, I3=D3;
	int includeGhost=1;
	int blockOffset=1;
        ok = getLocalArrayBounds(a,aLocal,I1,I2,I3,includeGhost,blockOffset);	

	int i1,i2,i3;
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  for( int b2=0; b2<block; b2++ )
	  {
	    for( int b1=0; b1<block; b1++ )
	    {
	      aLocal(BB(b1,b2),i1,i2,i3)= .25*(.5-b1-.5*b2)*(i1+2*i2-i3);
	      bLocal(BB(b1,b2),i1,i2,i3)= .25*(-.5*b1+1.5*b2)*(i1+i2-i3);
	      cLocal(BB(b1,b2),i1,i2,i3)= .25*(.125+.25*b1+.75*b2)*(i1-i2+i3*2);
	      if( b1==b2 )
	      {
		aLocal(BB(b1,b2),i1,i2,i3)+=10.;
		bLocal(BB(b1,b2),i1,i2,i3)+=40.;
		cLocal(BB(b1,b2),i1,i2,i3)+=10.;
	      }
	    }

	    xLocal(b2,i1,i2,i3)=b2+1.;   // Here is the exact solution 

	  }
	}

        includeGhost=0; // no paralle ghost
        ok = getLocalArrayBounds(a,aLocal,I1,I2,I3,includeGhost,blockOffset);	
	
	bool adjustStart = xLocal.getBase(axis+blockOffset)  < x.getBase(axis+blockOffset);
	bool adjustEnd   = xLocal.getBound(axis+blockOffset) > x.getBound(axis+blockOffset);

	if( systemType==TridiagonalSolver::normal )
	{
	  if( axis==0 )
	  {
	    if( adjustStart )
  	      xLocal(B,-1,I2,I3)=0.;  // NOTE: we make use of parallel ghost here 
            if(adjustEnd )
	      xLocal(B,m+1,I2,I3)=0.;
	  }
	  else if( axis==1 )
	  {
	    if( adjustStart )
	    xLocal(B,I1,-1,I3)=0.;
            if(adjustEnd )
	    xLocal(B,I1,m+1,I3)=0.;
	  }
	  else
	  {
	    if( adjustStart )
	    xLocal(B,I1,I2,-1)=0.;
            if(adjustEnd )
	    xLocal(B,I1,I2,m+1)=0.;
	  }
	}
	else if( systemType==TridiagonalSolver::extended )
	{
	  // NOTE: this will not work in parallel if there too few points on this processor.
	  if( axis==0 )
	  {
	    if( adjustStart )
	      xLocal(B,-1,I2,I3)=xLocal(B,2,I2,I3);
            if(adjustEnd )
	      xLocal(B,m+1,I2,I3)=xLocal(B,m-2,I2,I3);
	  }
	  else if( axis==1 )
	  {
	    if( adjustStart )
	      xLocal(B,I1,-1,I3)=xLocal(B,I1,2,I3);
            if(adjustEnd )
	      xLocal(B,I1,m+1,I3)=xLocal(B,I1,m-2,I3);
	  }
	  else
	  {
	    if( adjustStart )
	      xLocal(B,I1,I2,-1)=xLocal(B,I1,I2,2);
            if(adjustEnd )
	      xLocal(B,I1,I2,m+1)=xLocal(B,I1,I2,m-2);
	  }
	}
	else
	{ // periodic
	  for( int b2=0; b2<block; b2++ )
	  {
	    if( axis==0 )
	    {
	      if( adjustStart )
		xLocal(b2,-1,I2,I3)=b2+1; // x(b2,m,I2,I3);
	      if(adjustEnd )
		xLocal(b2,m+1,I2,I3)=b2+1; // x(b2,0,I2,I3);
	    }
	    else if( axis==1 )
	    {
	      if( adjustStart )
		xLocal(b2,I1,-1,I3)=b2+1; // x(b2,I1,m,I3);
	      if(adjustEnd )
		xLocal(b2,I1,m+1,I3)=b2+1; // x(b2,I1,0,I3);
	    }
	    else
	    {
	      if( adjustStart )
		xLocal(b2,I1,I2,-1)=b2+1; // x(b2,I1,I2,m);
	      if(adjustEnd )
		xLocal(b2,I1,I2,m+1)=b2+1; // x(b2,I1,I2,0);
	    }
	  }
	}
	
    
        // --- compute the RHS r so that "x" is the true solution ---
	is[axis]=1;
	for( int b1=0; b1<block; b1++ )
	{
	  if( block==2 )
	  {
	    rLocal(b1,I1,I2,I3)=
                           (aLocal(BB(b1,0),I1,I2,I3)*xLocal(0,I1-is[0],I2-is[1],I3-is[2])+
			    aLocal(BB(b1,1),I1,I2,I3)*xLocal(1,I1-is[0],I2-is[1],I3-is[2]) + 
			    bLocal(BB(b1,0),I1,I2,I3)*xLocal(0,I1  ,I2,I3)+
			    bLocal(BB(b1,1),I1,I2,I3)*xLocal(1,I1  ,I2,I3) + 
			    cLocal(BB(b1,0),I1,I2,I3)*xLocal(0,I1+is[0],I2+is[1],I3+is[2])+
			    cLocal(BB(b1,1),I1,I2,I3)*xLocal(1,I1+is[0],I2+is[1],I3+is[2]));
	  }
	  else
	  {
	    rLocal(b1,I1,I2,I3)=
	      (aLocal(BB(b1,0),I1,I2,I3)*xLocal(0,I1-is[0],I2-is[1],I3-is[2])+
	       aLocal(BB(b1,1),I1,I2,I3)*xLocal(1,I1-is[0],I2-is[1],I3-is[2])+
	       aLocal(BB(b1,2),I1,I2,I3)*xLocal(2,I1-is[0],I2-is[1],I3-is[2]) + 
	       bLocal(BB(b1,0),I1,I2,I3)*xLocal(0,I1  ,I2,I3)+
	       bLocal(BB(b1,1),I1,I2,I3)*xLocal(1,I1  ,I2,I3)+
	       bLocal(BB(b1,2),I1,I2,I3)*xLocal(2,I1  ,I2,I3) + 
	       cLocal(BB(b1,0),I1,I2,I3)*xLocal(0,I1+is[0],I2+is[1],I3+is[2])+
	       cLocal(BB(b1,1),I1,I2,I3)*xLocal(1,I1+is[0],I2+is[1],I3+is[2])+
	       cLocal(BB(b1,2),I1,I2,I3)*xLocal(2,I1+is[0],I2+is[1],I3+is[2]));
	  }
	  
	}

	if( FALSE )
	  display(r,"Here is the rhs");

	tri.factor(a,b,c,systemType,axis,block);

	tri.solve(r);

	real error = max(abs(rLocal(B,I1,I2,I3)-xLocal(B,I1,I2,I3)));
        error=ParallelUtility::getMaxValue(error);
	
        worstError=max(worstError,error);

	if( FALSE )
	  display(r-x(Range(0,1),I1,I2,I3),"here is the error");
	printF(" ****maximum error=%8.2e in the %s %ix%i block system ",error,
	       systemType==TridiagonalSolver::normal ? "normal  " :
	       systemType==TridiagonalSolver::extended ? "extended" : "periodic", block,block);
	if( ii==0 )
	  printF(" (1d single-equation)\n");
	else
	  printF(" (multiple equations, axis=%i) \n",ii-1);

      }
    }
  }

  if( worstError < REAL_EPSILON*1000. )
  {
    printF("\n======== BLOCK Test successful. Worst error = %e ============\n",worstError);
  }
  else
  {
    printF("\n******** BLOCK test apparently FAILED Worst error = %e ***************\n",worstError);
  }


  // ******************
  if( true )
  {
    printF(" **** STOP HERE FOR NOW ****\n");
    Overture::finish();          
    return 0;

  }
  
/* -------------





  // Peform some timings:


//  Test non-block systems

  // const int nx=11, ny=3, nz=3;
  const int n1=51, n2=50, n3=52;


  if( true )
  {
    veryWorstError=max(veryWorstError,worstError);
    worstError=0.;
    
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

    // int n1=101, n2=100, n3=102;
    // int n1=51, n2=50, n3=52;
    // int n1=11, n2=11, n3=11;
    // printF("\n ****** test timings for n1=%i, n2=%i, n3=%i *******\n",n1,n2,n3);
  printF("\n\n"
         "******************************************************************************\n"
         "**********************  tridiagonal systems **********************************\n"
         "********************** n1=%i n2=%i n3=%i    **********************************\n"
         "******************************************************************************\n\n",n1,n2,n3);
    
    I1=Range(n1), I2=Range(n2), I3=Range(n3);
    RealArray a(I1,I2,I3),b(I1,I2,I3),c(I1,I2,I3);
    RealArray u(I1,I2,I3),r(I1,I2,I3);
  
    for( int system=0; system<3; system++ )
    {
      TridiagonalSolver::SystemType systemType = (TridiagonalSolver::SystemType)system;

      int axis;
      for( axis=0; axis<=2; axis++ )
//    for( axis=2; axis>=0; axis-- )
      {
	const int base =Iv[axis].getBase();
	const int bound=Iv[axis].getBound();
	J1=I1, J2=I2, J3=I3;
	for( int i=base; i<=bound; i++)
	{
	  Jv[axis]=i;
	  a(J1,J2,J3)=  -(i+1);
	  b(J1,J2,J3)= 4*(i+1);
	  c(J1,J2,J3)=-2*(i+1);
	  r(J1,J2,J3)=   (i+1);
	}
        if( systemType==TridiagonalSolver::normal )
	{
	  Jv[axis]=base;
	  r(J1,J2,J3) -=a(J1,J2,J3);
	  Jv[axis]=bound;
	  r(J1,J2,J3)-=c(J1,J2,J3);
	}
	
	real time0=getCPU();
	tri.factor(a,b,c,systemType,axis);

	real timeFactor=getCPU()-time0;
	time0=getCPU();
	tri.solve(r,I1,I2,I3);
	real timeSolve=getCPU()-time0;
  
	real error = max(abs(r-1.));
	worstError=max(worstError,error);
	aString name=systemType==TridiagonalSolver::normal ?   "normal  " :
                     systemType==TridiagonalSolver::extended ? "extended" :  "periodic";

	printF(" ****maximum error=%8.2e in the %s case, axis=%i. factor=%8.2e(s) solve=%8.2e(s)\n",
               error,(const char*)name,axis,timeFactor,timeSolve);
      }
      
    }
    
    if( worstError < REAL_EPSILON*1000. )
    {
      printF("\n======== Test successful. Worst error = %e ============\n",worstError);
    }
    else
    {
      printF("\n******** Test apparently FAILED Worst error = %e ***************\n",worstError);
    }

  }
  

  // **********************************************************************************
  // **********test pentadiagonal systems**********************************************
  // **********************************************************************************


  veryWorstError=max(veryWorstError,worstError);
  worstError=0.;
  
  printF("\n\n"
         "******************************************************************************\n"
         "********************** pentadiagonal systems**********************************\n"
         "********************** n1=%i n2=%i n3=%i    **********************************\n"
         "******************************************************************************\n\n",n1,n2,n3);
 
  Range J1,J2,J3;
    
  for( int system=0; system<3; system++ )
  {
    TridiagonalSolver::SystemType systemType = (TridiagonalSolver::SystemType)system;

    for( axis=0; axis<3; axis++ )
    {

//        if( system==2 && axis>0 )
//          continue;


      I1=Range(0,n1-1), I2=Range(0,n2-1), I3=Range(0,n3-1);
      a.redim(I1,I2,I3);
      b.redim(I1,I2,I3);
      c.redim(I1,I2,I3);
      r.redim(I1,I2,I3);
      RealArray d(I1,I2,I3),e(I1,I2,I3),f(I1,I2,I3);
  
      for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
      {
	a(i1,I2,I3)=-1.*(i1+1);
	b(i1,I2,I3)=-3.*(i1+1);
	c(i1,I2,I3)= 8.*(i1+1);
	d(i1,I2,I3)=-2.*(i1+1);
	e(i1,I2,I3)= 1.*(i1+1);
	r(i1,I2,I3)= 3.*(i1+1);

      }
      if( systemType==TridiagonalSolver::normal )
      {
	base =I1.getBase();
	bound=I1.getBound();
	r(base,I2,I3)   -=a(base,I2,I3)+b(base,I2,I3);
	r(base+1,I2,I3) -=a(base+1,I2,I3);
	r(bound-1,I2,I3)-=e(bound-1,I2,I3);
	r(bound,I2,I3)  -=d(bound,I2,I3)+e(bound,I2,I3);
      
      }
    
      RealArray aa,bb,cc,dd,ee,rr;
      if( axis==0 )
      {
	J1=I1;
	J2=I2;
	J3=I3;
	aa=a; bb=b; cc=c; dd=d; ee=e; rr=r;
      }
      else if( axis==1 )
      {
	J1=I2;
	J2=I1;
	J3=I3;
	aa.redim(I2,I1,I3);
	bb.redim(I2,I1,I3);
	cc.redim(I2,I1,I3);
	dd.redim(I2,I1,I3);
	ee.redim(I2,I1,I3);
	rr.redim(I2,I1,I3);
	for( int i2=I2.getBase(); i2<=I2.getBound(); i2++)
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
	  {
	    aa(i2,i1,I3)=a(i1,i2,I3);
	    bb(i2,i1,I3)=b(i1,i2,I3);
	    cc(i2,i1,I3)=c(i1,i2,I3);
	    dd(i2,i1,I3)=d(i1,i2,I3);
	    ee(i2,i1,I3)=e(i1,i2,I3);
	    rr(i2,i1,I3)=r(i1,i2,I3);
	  }
      }
      else 
      {
	J1=I2;
	J2=I3;
	J3=I1;
	aa.redim(I2,I3,I1);
	bb.redim(I2,I3,I1);
	cc.redim(I2,I3,I1);
	dd.redim(I2,I3,I1);
	ee.redim(I2,I3,I1);
	rr.redim(I2,I3,I1);
	for( int i3=I3.getBase(); i3<=I3.getBound(); i3++)
	  for( int i2=I2.getBase(); i2<=I2.getBound(); i2++)
	    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
	    {
	      aa(i2,i3,i1)=a(i1,i2,i3);
	      bb(i2,i3,i1)=b(i1,i2,i3);
	      cc(i2,i3,i1)=c(i1,i2,i3);
	      dd(i2,i3,i1)=d(i1,i2,i3);
	      ee(i2,i3,i1)=e(i1,i2,i3);
	      rr(i2,i3,i1)=r(i1,i2,i3);
	    }
      }

      real time0=getCPU();
      tri.factor(aa,bb,cc,dd,ee,systemType,axis);
      real timeFactor=getCPU()-time0;

      time0=getCPU();
      tri.solve(rr,J1,J2,J3);
      real timeSolve=getCPU()-time0;

      error = max(abs(rr-1.));
      worstError=max(worstError,error);

      aString name=systemType==TridiagonalSolver::normal ?   "normal  " :
	systemType==TridiagonalSolver::extended ? "extended" :  "periodic";
      printF(" ****maximum error=%8.2e in the %s system, axis=%i. cpu: factor=%8.2e solve=%8.2e\n"
             ,error,(const char*)name,axis,timeFactor,timeSolve);
      // rr.display("Here is the solution, should be 1");
    }
    
  }

  if( worstError < REAL_EPSILON*1000. )
  {
    printF("\n======== Test successful. Worst error = %e ============\n",worstError);
  }
  else
  {
    printF("\n******** Test apparently FAILED Worst error = %e ***************\n",worstError);
  }

  if( veryWorstError < REAL_EPSILON*1000. )
  {
    printF("\n================================================================\n");
    printF("\n======== ALL Test passed! success! Worst error = %e ============\n",veryWorstError);
    printF("\n================================================================\n");
  }
  else
  {
    printF("\n================================================================\n");
    printF("\n******** SOME tests failed :( error = %e ***************\n",veryWorstError);
    printF("\n================================================================\n");
  }



//  axis=0;
  

//    // *******************EXTENDED**************************************************
//      I1=Range(0,11), I2=Range(0,2), I3=Range(0,2);
//      a.redim(I1,I2,I3);
//      b.redim(I1,I2,I3);
//      c.redim(I1,I2,I3);
//      RealArray d(I1,I2,I3),e(I1,I2,I3),f(I1,I2,I3);

//    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
//    {
//      a(i1,I2,I3)=-1.*(i1+1);
//      b(i1,I2,I3)=-3.*(i1+1);
//      c(i1,I2,I3)= 8.*(i1+1);
//      d(i1,I2,I3)=-2.*(i1+1);
//      e(i1,I2,I3)= 1.*(i1+1);

//      r(i1,I2,I3)= 3.*(i1+1);

//    }
//    tri.factor(a,b,c,d,e,TridiagonalSolver::extended,axis);
//    a.redim(0);
//    tri.solve(r,I1,I2,I3);
//    a.redim(I1,I2,I3);
  
//    error = max(abs(r-1.));
//    worstError=max(worstError,error);
//    printF(" ****maximum error=%8.2e in the extended case.\n",error);
//    // r.display("Here is the solution, should be 1");
  

//    // *******************PERIODIC**************************************************

//    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
//    {
//      a(i1,I2,I3)=-1.*(i1+1);
//      b(i1,I2,I3)=-3.*(i1+1);
//      c(i1,I2,I3)= 8.*(i1+1);
//      d(i1,I2,I3)=-2.*(i1+1);
//      e(i1,I2,I3)= 1.*(i1+1);

//      r(i1,I2,I3)= 3.*(i1+1);

//    }
//    tri.factor(a,b,c,d,e,TridiagonalSolver::periodic,axis);
//    a.redim(0);
//    tri.solve(r,I1,I2,I3);
//    a.redim(I1,I2,I3);
  
//    error = max(abs(r-1.));
//    worstError=max(worstError,error);
//    printF(" ****maximum error=%8.2e in the periodic case.\n",error);
  // r.display("Here is the solution, should be 1");
  

//    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
//    {
//      a(i1,I2,I3)=  -(i1+1);
//      b(i1,I2,I3)= 4*(i1+1);
//      c(i1,I2,I3)=-2*(i1+1);
//      r(i1,I2,I3)=   (i1+1);
//    }
  
//    tri.factor(a,b,c,TridiagonalSolver::extended,axis);
//    tri.solve(r);

//    // r.display("Here is the solution from the extended system, should be 1");
//    error = max(abs(r-1.));
//    worstError=max(worstError,error);
//    printF(" ****maximum error=%8.2e in the extended case.\n",error);
  
//    for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
//    {
//      a(i1,I2,I3)=  -(i1+1);
//      b(i1,I2,I3)= 4*(i1+1);
//      c(i1,I2,I3)=-2*(i1+1);
//      r(i1,I2,I3)=   (i1+1);
//    }

//    tri.factor(a,b,c,TridiagonalSolver::periodic,axis);
//    tri.solve(r);
   
//    // r.display("Here is the solution from the periodic system, should be 1");
//    error = max(abs(r-1.));
//    worstError=max(worstError,error);
//    printF(" ****maximum error=%8.2e in the periodic case.\n",error);






  Overture::finish();          
  return 0;

  ---- */

}
