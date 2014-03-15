#include "Overture.h"
#include "display.h"

int getLineFromFile( FILE *file, char s[], int lim);

// ==========================================================================
// This program compares the check files (regression testing) for cg
// and is normally called by the perl script check.p
// =========================================================================
int
main(int argc, char *argv[])
{
  int returnValue=-1;
  
  aString fileName1,fileName2;
  real tol=1.e-5; // error tolerance
  if( argc > 2 )
  { 
    fileName1=argv[1];
    fileName2=argv[2];
    if( argc>3 )
    {
      aString line=argv[3];
      int len=0;
      if( len=line.matches("-tol=") )
      {
	sScanF(line(len,line.length()-1),"%e",&tol);
      }
    }
  }
  else
    cout << "Usage: `checkCheckFiles fileName1 fileName2 [-tol=<tol>]' \n";


  FILE *file1=fopen((const char*)fileName1,"r");
  if( file1==NULL )
  {
    printf("unable to open file1\n");
    return 1; 
  }
    
  FILE *file2=fopen((const char*)fileName2,"r");
  if( file2==NULL )
  {
    printf("unable to open file2\n");
    fclose(file1);
    return 1;
  }
  
  int numberOfComponents[2];
  real t[2];
  const int maxNumberOfComponents=999;
  RealArray value[2];
  Range V(0,1);
  value[0].redim(maxNumberOfComponents,2);
  value[1].redim(maxNumberOfComponents,2);
  const int maxBuff=180;
  char buff[maxBuff];
  
  getLineFromFile(file1,buff,maxBuff); // title
  // printf("%s: title: %s\n",(const char*)fileName1,buff);
  getLineFromFile(file2,buff,maxBuff); // title
  // printf("%s: title: %s\n",(const char*)fileName2,buff);

  const int maxNumberOfTimeSteps=1000;
  int timeStep, numberOfTimes=-1;
  real maxErr=0.; 
  for( timeStep=0; timeStep<maxNumberOfTimeSteps && returnValue==-1; timeStep++ )
  {
    numberOfTimes++;
    int m=0;
    for( m=0; m<2; m++ )
    {
      FILE *file= m==0 ? file1 : file2;

      fScanF(file,"%e %i",&t[m],&numberOfComponents[m]);
      if( feof(file) )
      {
	// printf("EOF on file %i\n",m);
        if( timeStep>0 )
	{
	  returnValue=0;
	  break;
	}
        else 
          return 1;
      }
      // printf(" file %i : t=%e numberOfComponents=%i\n",m,t[m],numberOfComponents[m]);
      if( numberOfComponents[m]<=0 || numberOfComponents[m]>maxNumberOfComponents )
      {
        aString fileName = m==0 ? fileName1 : fileName2;
        printf("ERROR: file %s invalid number of components. t[%i]=%e  numberOfComponents=%i\n",
             (const char*)fileName,m,t[m],numberOfComponents[m]);
        printf("fileName1=%s fileName2=%s\n",(const char*)fileName1,(const char*)fileName2);
	
	return 1;
      }
      int dum,n;
      for( n=0; n<numberOfComponents[m]; n++ )
	fScanF(file,"%i %e %e",&dum,&(value[m](n,0)),&(value[m](n,1)));
    }
    Range N(0,numberOfComponents[0]-1);
    real maxDiff = max(fabs(value[0](N,V)-value[1](N,V)));
    maxDiff = max(maxDiff,fabs(t[0]-t[1]));
    maxErr=max(maxErr,maxDiff);
    if( maxDiff > tol ) // .005 ) // FLT_EPSILON*100. )
    {
      printf("checkCheckFiles: files do not agree, maxDiff=%e, file1=%s, file2=%s\n",maxDiff,
               (const char*)fileName1,(const char*)fileName2);
      // display(value[0](N,V),sPrintF(buff,"values in file %s at t=%e",(const char*)fileName1,t[0]));
      // display(value[1](N,V),sPrintF(buff,"values in file %s at t=%e",(const char*)fileName2,t[1]));
      if( fabs(t[0]-t[1])>tol )
      {
        printf(" times do not agree: %10.4e (file1) and %10.4e (file2) differ by %8.2e.\n",t[0],t[1],fabs(t[0]-t[1])); 
      }
      for( int n=0; n<numberOfComponents[0]; n++ )
      {
	if( fabs(value[0](n,0)-value[1](n,0))>tol )
	{
          printf(" Component %i: values: %10.4e (file1) and %10.4e (file2) differ by %8.2e.\n",
                 n,value[0](n,0),value[1](n,0),fabs(value[0](n,0)-value[1](n,0)));
	}
	if( fabs(value[0](n,1)-value[1](n,1))>tol )
	{
          printf(" Component %i: norms:  %10.4e (file1) and %10.4e (file2) differ by %8.2e.\n",
                 n,value[0](n,1),value[1](n,1),fabs(value[0](n,1)-value[1](n,1)));
	}
      }
      
      return 1;
    }
    else
    {
      // printf("files agree at t=%e \n",t[0]);
    }
  }
  printf("checkCheckFiles: files %s and %s agree at %i times , maxDiff=%8.2e, (tol=%8.2e)\n",(const char*)fileName1,
         (const char*)fileName2,numberOfTimes,maxErr,tol);
  
  fclose(file1);
  fclose(file2);
  
  return 0;
}
