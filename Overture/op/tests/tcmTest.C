//===============================================================================
//  Test the speed for generating coefficient matrices
//==============================================================================
#include "Overture.h"  
#include "MappedGridOperators.h"
#include "Oges.h"
#include "Square.h"
#include "BoxMapping.h"
#include "Annulus.h"
#include "Sphere.h"
#include "display.h"

#ifdef NEWAPP
  #define GET_NUMBER_OF_ARRAYS Array_Domain_Type::getNumberOfArraysInUse()
#else
  #define GET_NUMBER_OF_ARRAYS Array_Descriptor_Type::getMaxNumberOfArrays()
#endif


realMappedGridFunction 
returnByValue(realMappedGridFunction & coeff )
{
  return coeff;
}



int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  int numberOfGridLines=81;
//  cout << "Enter Oges::debug, numberOfGridLines\n";
//  cin >> Oges::debug >> numberOfGridLines;

  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 

  FILE *debugFile = fopen("tcmTime.out","w");

  SquareMapping square;
  AnnulusMapping annulus;
  BoxMapping box;
  SphereMapping sphere(.5,1., 0.,0.,0., 0., 1., .2,.8 ); // miss the poles;
  Mapping *mapPointer;
  
  real time,timeNew,timeOld,error;

  for( int orderOfAccuracy=2; orderOfAccuracy<=4; orderOfAccuracy+=2 ) // test 2nd and fourth order
  {
    printf(" ---------------------------------------------------------------- \n");
    printf(" ------------------ order of accuracy = %i ---------------------- \n",orderOfAccuracy);
    printf(" ---------------------------------------------------------------- \n\n");
    for( int mapping=0; mapping<=3; mapping++ )
    {
      if( mapping==0 )
      {
	printf(" ****************** test a Square ***************************** \n");
	mapPointer=&square;
	numberOfGridLines = 41;
	mapPointer->setGridDimensions(axis1,numberOfGridLines);
	mapPointer->setGridDimensions(axis2,numberOfGridLines+5);
      }
      else if( mapping==1 )
      {
	printf(" ****************** test an Annulus ***************************** \n");
	mapPointer=&annulus;
	numberOfGridLines = 31;
	mapPointer->setGridDimensions(axis1,numberOfGridLines);
	mapPointer->setGridDimensions(axis2,numberOfGridLines+5);
      }
      else if( mapping==2 )
      {
	printf(" ****************** test a Box ***************************** \n");
	mapPointer=&box;
	numberOfGridLines = 31/(orderOfAccuracy/2);
	mapPointer->setGridDimensions(axis1,numberOfGridLines);
	mapPointer->setGridDimensions(axis2,numberOfGridLines+5);
	mapPointer->setGridDimensions(axis3,numberOfGridLines-5);
      }
      else
      {
	printf(" ****************** test a Sphere **************************** \n");
	mapPointer=&sphere;
	numberOfGridLines = 21/(orderOfAccuracy/2);;
	mapPointer->setGridDimensions(axis1,numberOfGridLines);
	mapPointer->setGridDimensions(axis2,numberOfGridLines+5);
	mapPointer->setGridDimensions(axis3,numberOfGridLines-5);

      }
      Mapping & map = *mapPointer;
    
      MappedGrid mg(map);
      for( int side=Start; side<=End; side++ )
      {
	mg.numberOfGhostPoints()(side,axis1)=2;
	mg.dimension()(side,axis1)+=2*side-1;
      }
      mg.update();
    
	// make a grid function to hold the coefficients
      Range all;
      int stencilSize=pow(orderOfAccuracy+1,mg.numberOfDimensions());
      realMappedGridFunction coeff(mg,stencilSize,all,all,all), c;
      time=getCPU();
      coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
      time=getCPU()-time;
      printf("Time for setIsACoefficientMatrix = %e \n",time);
      coeff=0.;

      MappedGridOperators op;                            // create some differential operators
      op.setStencilSize(stencilSize);
      op.setOrderOfAccuracy(orderOfAccuracy);
      op.updateToMatchGrid(mg);
      coeff.setOperators(op);
  
      // -------------------------------------------------------------------------------------------

      op.useNewOperators=TRUE;
      coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
    
      time=getCPU();
      coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
      timeNew=getCPU()-time;
      c=coeff;  // save

      op.useNewOperators=FALSE;
      time=getCPU();
      coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
      timeOld=getCPU()-time;

      error =max(abs(c-coeff))/SQR(numberOfGridLines);  // relative error in coefficients
    
      printf("lapCoefficients (numberOfGridLines=%i) : %e (new) %e (old), ratio=%e, error=%e\n",
	     numberOfGridLines,timeNew,timeOld,timeOld/timeNew,error);

      if( FALSE && error>1.e-3 )
      {
	display(evaluate(c-coeff),"Here is c-coeff (old)",debugFile,"%e7.1 ");
      }

      // -------------------------------------------------------------------------------------------
      op.useNewOperators=TRUE;
      coeff=op.xCoefficients();       
    
      time=getCPU();
      coeff=op.xCoefficients();       
      timeNew=getCPU()-time;
      c=coeff;  // save

      op.useNewOperators=FALSE;
      time=getCPU();
      coeff=op.xCoefficients();       
      timeOld=getCPU()-time;

      error =max(abs(c-coeff))/numberOfGridLines;  // relative error in coefficients
    
      printf("  xCoefficients (numberOfGridLines=%i) : %e (new) %e (old), ratio=%e, error=%e\n",
	     numberOfGridLines,timeNew,timeOld,timeOld/timeNew,error);

      if( FALSE && error>1.e-3 )
      {
	display(evaluate(c-coeff),"Here is c-coeff (old)",debugFile,"%e7.1 ");
      }


      // -------------------------------------------------------------------------------------------
      if( map.getDomainDimension()>1 )
      {
	op.useNewOperators=TRUE;
	coeff=op.yCoefficients();       
    
	time=getCPU();
	coeff=op.yCoefficients();       
	timeNew=getCPU()-time;
	c=coeff;  // save

	op.useNewOperators=FALSE;
	time=getCPU();
	coeff=op.yCoefficients();       
	timeOld=getCPU()-time;

	error =max(abs(c-coeff))/numberOfGridLines;  // relative error in coefficients
    
	printf("  yCoefficients (numberOfGridLines=%i) : %e (new) %e (old), ratio=%e, error=%e\n",
	       numberOfGridLines,timeNew,timeOld,timeOld/timeNew,error);
      
      }
      // -------------------------------------------------------------------------------------------
      if( map.getDomainDimension()>2 )
      {
	op.useNewOperators=TRUE;
	coeff=op.zCoefficients();       
    
	time=getCPU();
	coeff=op.zCoefficients();       
	timeNew=getCPU()-time;
	c=coeff;  // save

	op.useNewOperators=FALSE;
	time=getCPU();
	coeff=op.zCoefficients();       
	timeOld=getCPU()-time;

	error =max(abs(c-coeff))/numberOfGridLines;  // relative error in coefficients
    
	printf("  zCoefficients (numberOfGridLines=%i) : %e (new) %e (old), ratio=%e, error=%e\n",
	       numberOfGridLines,timeNew,timeOld,timeOld/timeNew,error);
      
      }
    
      // -------------------------------------------------------------------------------------------

      op.useNewOperators=TRUE;
      coeff=op.xxCoefficients();      
    
      time=getCPU();
      coeff=op.xxCoefficients();      
      timeNew=getCPU()-time;
      c=coeff;  // save

      op.useNewOperators=FALSE;
      time=getCPU();
      coeff=op.xxCoefficients();      
      timeOld=getCPU()-time;

      error =max(abs(c-coeff)/SQR(numberOfGridLines));  // relative error in coefficients
    
      printf(" xxCoefficients (numberOfGridLines=%i) : %e (new) %e (old), ratio=%e, error=%e\n",
	     numberOfGridLines,timeNew,timeOld,timeOld/timeNew,error);

      if( error>1.e-3 )
      {
        Range R0=c.dimension(0);
        Range R1=c.dimension(1);
        Range R2=c.dimension(2);
        Range R3=c.dimension(3);
	realArray diff(R0,R1,R2,R3);
        diff=0.;
        diff=(c(R0,R1,R2,R3)-coeff(R0,R1,R2,R3))/SQR(numberOfGridLines);
	
        for( int i0=diff.getBase(0); i0<=diff.getBound(0); i0++ )
        for( int i1=diff.getBase(1); i1<=diff.getBound(1); i1++ )
        for( int i2=diff.getBase(2); i2<=diff.getBound(2); i2++ )
	  for( int i3=diff.getBase(3); i3<=diff.getBound(3); i3++ )
	  {
	    if( diff(i0,i1,i2,i3)>1. )
	    {
	      printf(" (i0,i1,i2,i3)=(%i,%i,%i,%i) : c=%e, coeff=%e, diff=%e \n",i0,i1,i2,i3,
		     c(i0,i1,i2,i3),coeff(i0,i1,i2,i3),diff(i0,i1,i2,i3));
	    }
	  }

	display(c,"xx: Here is c (new)",debugFile,"%8.1e ");
	display(coeff,"xx: Here is coeff (old)",debugFile,"%8.1e ");
        diff=(c-coeff)/SQR(numberOfGridLines);
	display(evaluate((c-coeff)/SQR(numberOfGridLines)),"xx: Here is c-coeff (old)",debugFile,"%3.0f ");
        

	if( TRUE )
	  return 0;
      }

      // -------------------------------------------------------------------------------------------
      if( map.getDomainDimension()>1 )
      {
	op.useNewOperators=TRUE;
	coeff=op.yyCoefficients();       
    
	time=getCPU();
	coeff=op.yyCoefficients();       
	timeNew=getCPU()-time;
	c=coeff;  // save

	op.useNewOperators=FALSE;
	time=getCPU();
	coeff=op.yyCoefficients();       
	timeOld=getCPU()-time;

	error =max(abs(c-coeff))/numberOfGridLines;  // relative error in coefficients
    
	printf(" yyCoefficients (numberOfGridLines=%i) : %e (new) %e (old), ratio=%e, error=%e\n",
	       numberOfGridLines,timeNew,timeOld,timeOld/timeNew,error);
      
      }
      // -------------------------------------------------------------------------------------------
      if( map.getDomainDimension()>2 )
      {
	op.useNewOperators=TRUE;
	coeff=op.zzCoefficients();       
    
	time=getCPU();
	coeff=op.zzCoefficients();       
	timeNew=getCPU()-time;
	c=coeff;  // save

	op.useNewOperators=FALSE;
	time=getCPU();
	coeff=op.zzCoefficients();       
	timeOld=getCPU()-time;

	error =max(abs(c-coeff))/numberOfGridLines;  // relative error in coefficients
    
	printf(" zzCoefficients (numberOfGridLines=%i) : %e (new) %e (old), ratio=%e, error=%e\n",
	       numberOfGridLines,timeNew,timeOld,timeOld/timeNew,error);
      
      }


      // -------------------------------------------------------------------------------------------
      if( map.getDomainDimension()>1 )
      {
	op.useNewOperators=TRUE;
	coeff=op.xyCoefficients();       
    
	time=getCPU();
	coeff=op.xyCoefficients();       
	timeNew=getCPU()-time;
	c=coeff;  // save

	op.useNewOperators=FALSE;
	time=getCPU();
	coeff=op.xyCoefficients();       
	timeOld=getCPU()-time;

	error =max(abs(c-coeff))/numberOfGridLines;  // relative error in coefficients
    
	printf(" xyCoefficients (numberOfGridLines=%i) : %e (new) %e (old), ratio=%e, error=%e\n",
	       numberOfGridLines,timeNew,timeOld,timeOld/timeNew,error);
      
	if( error>1.e-3 )
	{
	  display(c,"xy: Here is c (new)",debugFile,"%7.1e ");
	  display(coeff,"xy: Here is coeff (old)",debugFile,"%7.1e ");
	  display(evaluate(c-coeff),"xy: Here is c-coeff (old)",debugFile,"%7.1e ");
	  display(evaluate(c-coeff),"xy: Here is c-coeff (old)",debugFile,"%7.1e ");
	  if( TRUE )
	    return 0;
	}
      }
      // -------------------------------------------------------------------------------------------
      if( map.getDomainDimension()>2 )
      {
	op.useNewOperators=TRUE;
	coeff=op.xzCoefficients();       
    
	time=getCPU();
	coeff=op.xzCoefficients();       
	timeNew=getCPU()-time;
	c=coeff;  // save

	op.useNewOperators=FALSE;
	time=getCPU();
	coeff=op.xzCoefficients();       
	timeOld=getCPU()-time;

	error =max(abs(c-coeff))/numberOfGridLines;  // relative error in coefficients
    
	printf(" xzCoefficients (numberOfGridLines=%i) : %e (new) %e (old), ratio=%e, error=%e\n",
	       numberOfGridLines,timeNew,timeOld,timeOld/timeNew,error);
      
      }
      // -------------------------------------------------------------------------------------------
      if( map.getDomainDimension()>2 )
      {
	op.useNewOperators=TRUE;
	coeff=op.yzCoefficients();       
    
	time=getCPU();
	coeff=op.yzCoefficients();       
	timeNew=getCPU()-time;
	c=coeff;  // save

	op.useNewOperators=FALSE;
	time=getCPU();
	coeff=op.yzCoefficients();       
	timeOld=getCPU()-time;

	error =max(abs(c-coeff))/numberOfGridLines;  // relative error in coefficients
    
	printf(" yzCoefficients (numberOfGridLines=%i) : %e (new) %e (old), ratio=%e, error=%e\n",
	       numberOfGridLines,timeNew,timeOld,timeOld/timeNew,error);
      
      }
      // -------------------------------------------------------------------------------------------

      op.useNewOperators=TRUE;
      coeff=op.identityCoefficients();      
    
      time=getCPU();
      coeff=op.identityCoefficients();      
      timeNew=getCPU()-time;
      c=coeff;  // save

      op.useNewOperators=FALSE;
      time=getCPU();
      coeff=op.identityCoefficients();      
      timeOld=getCPU()-time;

      error =max(abs(c-coeff));  // relative error in coefficients
    
      printf(" idCoefficients (numberOfGridLines=%i) : %e (new) %e (old), ratio=%e, error=%e\n",
	     numberOfGridLines,timeNew,timeOld,timeOld/timeNew,error);


    printf("\n**** Number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);

    }
  }
  

  // u.display("Here is the solution to u.xx+u.yy=f");
  // real error=0.;
  // error=max(error,max(abs(u(I1,I2,I3)-true.u(mg,I1,I2,I3,0))));    
  // printf("Maximum error with dirichlet bc's= %e\n",error);  

  
  return(0);

}

