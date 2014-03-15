#include "Overture.h"
class blockTridiag2d{

 public:
  enum systemType{
    normal=0,
    periodic=1
  };

  blockTridiag2d();
  ~blockTridiag2d();

  int factor(realArray &A, realArray &B, realArray &C,
	     systemType type, int axis);
  int solve(realArray &D,systemType type);

 protected:
  int checkDimension(realArray A, realArray B, realArray C);
  int checkDimension(realArray A);
  realArray a, b, c, d, v, w;
  int iaxis;
  Index I1,I2,I3;

  int factorRegular(void);
  int factorPeriodic(void);

  int solveRegular(void);
  int solvePeriodic(void);
};
