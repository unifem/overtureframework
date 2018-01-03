/* 
Bug submitted by:
Christopher Oehmen
UT Memphis
School of Biomedical Engineering
College of Graduate Health Sciences
Memphis, TN
(901)-448-1497
*/


#include <A++.h>

int main(void)
   {
     ios::sync_with_stdio();

  // Range a(0,10);
  // Range b(0,15);
  // Range c(0, 6);

     Range all;
     Index a(0,10);
     Index b(0,15);
     Index c(0,6);

     int dummy1, dummy2, dummy3;

     doubleArray MyArray(a, b, c);

     for(dummy1=0;dummy1<a.length(); dummy1++){
          for(dummy2=0;dummy2<b.length(); dummy2++){
               for(dummy3=0;dummy3<c.length(); dummy3++){
                    MyArray(dummy1,dummy2,dummy3)=(dummy1+dummy2)/(1.0+dummy3);
                  }
             }
        }

     MyArray(0, all, 0).display("MyArray");
     MyArray.reshape(a, c, b);
     MyArray(0, 0, all).display("MyArrayafterfirstreshape");

  // If I understand this right, this display should have the
  // same data as the previous one, just in a different orientation.  
  // This example gives different results when I use Range
  // objects instead of Index objects.

     return(0);
   }
