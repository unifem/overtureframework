#ifndef RANDOMSAMPLING_H
#define RANDOMSAMPLING_H

#include <math.h>

class RandomSampling {
   public:
      RandomSampling();
      double RandomDouble();
      int RandomInteger(int);
      double* RandomCosines();
      double* Random2DCosines();

   private:
      // double DRanN();
      inline double min(int a, int b) { if (a < b) return a; else return b; }

      int m1, m2, m3, m4;
      int n1, n2, n3, n4;
      int l1, l2, l3, l4;
      int i1, i2, i3, i4;
      double tpm12;
};

#endif

