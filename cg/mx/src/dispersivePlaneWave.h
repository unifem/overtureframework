// -- dispersive plane wave solution
//        w = wr + i wi   (complex dispersion relation)
// You should define: 
//    dpwExp := exp( wi* t )
#define exDpw(x,y,t,dpwExp) sin(twoPi*(kx*(x)+ky*(y))-omegaDpwRe*(t))*(pwc[0]*(dpwExp))
#define eyDpw(x,y,t,dpwExp) sin(twoPi*(kx*(x)+ky*(y))-omegaDpwRe*(t))*(pwc[1]*(dpwExp))
#define hzDpw(x,y,t,dpwExp) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[5]

// ** FIX ME -- THIS IS WRONG
// #define extDpw(x,y,t,dpwExp) (-twoPi*omegaDpwRe)*cos(twoPi*(kx*(x)+ky*(y)-omegaDpwRe*(t)))*(pwc[0]*(dpwExp))
// #define eytDpw(x,y,t,dpwExp) (-twoPi*omegaDpwRe)*cos(twoPi*(kx*(x)+ky*(y)-omegaDpwRe*(t)))*(pwc[1]*(dpwExp))
// #define hztDpw(x,y,t,dpwExp) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[5]
