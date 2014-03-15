#include "A++.h"


#define BYTE unsigned char


void
packBytes( const int & num, BYTE *iBuff, int & count, const int & bitsPerWord=12 )
//
// 
{  
  unsigned int number=num;
  if( count % 3 == 0 )
  {
    iBuff[count]  = number >> 4;
    iBuff[count+1]= number << 4;
    printf("count=%i, number=%i, buff[%i]=%2.2X , buff[%i]=%2.2X \n",count,number,count,iBuff[count],count+1,iBuff[count+1]);
    count++;
  }
  else
  {
    iBuff[count] |= number >> 8;
    iBuff[count+1]= number & 0xFF;
    printf("count=%i, number=%2.2X, buff[%i]=%2.2X , buff[%i]=%2.2X \n",count,number,count,iBuff[count],count+1,iBuff[count+1]);
    count+=2;
  }
}
  

int 
main()
{
  
  BYTE iBuff[100];
  

  int count=0;
  for( int i=0; i<10; i++ )
  {
    packBytes( i+255,iBuff,count);
  }

  for( i=0; i<10; i+=3 )
  {
    printf("%2.2X%2.2X%2.2X ",iBuff[i],iBuff[i+1],iBuff[i+2]);
    if( i % 22 == 21 )
      printf("\n");
  }
  printf("\n");
  
}

  
