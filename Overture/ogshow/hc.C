#include "GL_GraphicsInterface.h"
#include <stdio.h>



void
rleCompress( const int num, short *xBuffer, FILE *outFile, const int numPerLine = 30 )
{

  printf("\n\n\n ***** rleCompress ****** n\n\n");

  int r;         // repetition count

  int maxR=128;  // largest repetition count allowed is 128

  int i=0;           // current char
  int count=0;       // number of chars printed on currnet lline
  while( i<num )
  {
    // count the number of similar chars
    r=1;
    while( r<maxR && i+r<num && xBuffer[i+r]==xBuffer[i] )
    {
      r++;
    }
    if( r>1 )
    {
      printf("repeat: r=%i, char=%2.2X \n",r,xBuffer[i]);
      fprintf(outFile,"%2.2X",257-r);   // length = 257-r
      fprintf(outFile,"%2.2X",xBuffer[i]);
      i+=r;
      count+=2;
    }
    else
    { // : b[i+1]!=b[i]
      // count number of dis-similiar chars
      r=1;
      while( r<maxR && i+r+1 < num && xBuffer[i+r+1]!=xBuffer[i+r] )
      {
	r++;
      }
      printf("dis-similar: r=%i, start-1=%2.2X start=%2.2X , end=%2.2X, end+1=%2.2X \n",r,xBuffer[i-1],
             xBuffer[i],xBuffer[i+r-1],xBuffer[i+r]);
      fprintf(outFile,"%2.2X",r-1);   // length = r-1  [0,127]
      for( int j=i; j<i+r; j++ )
	fprintf(outFile,"%2.2X",xBuffer[j]);
      i+=r;
      count+=r+1;
    }
    if( count > numPerLine )
    {
      fprintf(outFile,"\n");
      count=0;
    }
  }
  // write EOD    
  fprintf(outFile,"%2.2X",128);

}





int 
main()
{
  aString inFileName = "plot.data";
  aString outFileName = "plot.rle";
  aString cFileName = "c.rle";

  FILE *inFile;
  FILE *outFile, *cFile;
  inFile = fopen(inFileName,"r" );         
  outFile= fopen(outFileName,"w" );         
  cFile= fopen(cFileName,"w" );         

  assert( inFile!=NULL  && outFile!=NULL );
  
  int width, height;

  width = 494;
  height = 483;

  short *xBuffer = new short [width*height*3+1000];
  

  // read in the data

#define C(x) ( int((x*255)+.5)  )

  int numPerLine=30;           // print this many colours per line  
  int num = width*height*3;    // total number of colours

  int count=0;
  int i=0;
  for( count=0; count<num; count++ )
    fscanf(inFile,"%2hX",&(xBuffer[i++]));


  if( FALSE )
  {
    while( count<num )
    {
      for( i=0; i<numPerLine && count<num ; i++ )
	fprintf(outFile,"%2.2X",xBuffer[count++]);
      fprintf(outFile,"\n");
    }
  }  

  // output the RLE RGB file
  rleCompress( num, xBuffer, outFile, numPerLine );
  fclose(outFile);
  
  const int ctSize=256;
  
  short *iBuff = new short [width*height];  // index space

  short ct[ctSize][3];
  int nt=0;
  count=0;  
  for( i=0; i<num && nt<ctSize; i+=3 )
  {
    short r=xBuffer[i];    
    short g=xBuffer[i+1];    
    short b=xBuffer[i+2];    
    bool found=FALSE;
    for( int j=0; j<nt; j++ )
    {
      if( r==ct[j][0] && g==ct[j][1] && b==ct[j][2] )
      {
	found=TRUE;
        iBuff[count++]=j;
	break;
      }
    }
    if( !found )
    {
      ct[nt][0]=r;
      ct[nt][1]=g;
      ct[nt][2]=b;
      // printf("nt=%i, (r,g,b)=(%2X,%2X,%2X) (r,g,b)=(%2X,%2X,%2X)\n",nt,r,g,b,ct[nt][0],ct[nt][1],ct[nt][2]);
      iBuff[count++]=nt;
      nt++;
    }      
  }
  printf("Number of colours = %i \n",nt);  
  // output the index colour RLE file
  fprintf(cFile,"[/Indexed/DeviceRGB %i\n<",nt-1);
  i=0;
  while( i<nt )
  {
    for( int j=0; j<10 && i<nt ; j++,i++ )
    {
      fprintf(cFile,"%2.2X%2.2X%2.2X",ct[i][0],ct[i][1],ct[i][2]);
    }
    fprintf(cFile,"\n");
  }
  fprintf(cFile,">\n]setcolorspace\n\n");


  rleCompress( width*height, iBuff, cFile, numPerLine );
  fclose(cFile);

  

	    
/* ---
  int r;         // repetition count

  int maxR=1;  // 128 // largest repetition count allowed is 128
  int maxR1=128; // largest repetition count allowed is 128
  int maxR2=128;   // largest repetition count allowed is 128

  i=0;           // current char
  count=0;       // number of chars printed on currnet lline
  while( i<num )
  {
    // count the number of similar chars
    r=1;
    while( r<maxR1 && i+r<num && xBuffer[i+r]==xBuffer[i] )
    {
      r++;
    }
    if( r>1 )
    {
      // printf("repeat: r=%i, char=%2.2X \n",r,xBuffer[i]);
      fprintf(outFile,"%2.2X",257-r);   // length = 257-r
      fprintf(outFile,"%2.2X",xBuffer[i]);
      i+=r;
      count+=2;
    }
    else
    { // : b[i+1]!=b[i]
      // count number of dis-similiar chars
      r=1;
      while( r<maxR2 && i+r+1 < num && xBuffer[i+r+1]!=xBuffer[i+r] )
      {
	r++;
      }
      // printf("dis-similar: r=%i \n",r);
      fprintf(outFile,"%2.2X",r-1);   // length = r-1  [0,127]
      for( int j=i; j<i+r; j++ )
	fprintf(outFile,"%2.2X",xBuffer[j]);
      i+=r;
      count+=r+1;
    }
    if( count > numPerLine )
    {
      fprintf(outFile,"\n");
      count=0;
    }
  }
  // write EOD    
  fprintf(outFile,"%2.2X",128);
---- */

  delete [] xBuffer;

  fclose(inFile);

  return 0;

}

#undef C
