#include "Square.h"

int main() {
    SquareMapping square(-1., 1., -1., 1.);
    SquareMapping rectan(-.6, .6, -.4, .4);

//  Modify the boundary conditions and the grid dimensions.
    square.setBoundaryCondition(0,0,1); square.setBoundaryCondition(1,0,1);
    square.setBoundaryCondition(0,1,1); square.setBoundaryCondition(1,1,1);
    rectan.setBoundaryCondition(0,0,0); rectan.setBoundaryCondition(1,0,0);
    rectan.setBoundaryCondition(0,1,0); rectan.setBoundaryCondition(1,1,0);
    square.setGridDimensions(0,13); square.setGridDimensions(1,13);
    rectan.setGridDimensions(0,10); rectan.setGridDimensions(1,7);

    Int i; FloatArray r(2), x(2), xr(2,2), rx(2,2);

    for (i=0; i<=10; i++) {
        r(0) = .1 * i; r(1) = 1. - r(0);
        square.map(r, x, xr);
        cout << "square:     r = (" << r(0) << "," << r(1) << "), x = (" << x(0) << "," << x(1) << "), xr = ("
             << xr(0,0) << "," << xr(1,0) << "," << xr(0,1) << "," << xr(1,1) << ")" << endl;
    } // end for

    for (i=0; i<=10; i++) {
        x(0) = .2 * i - 1.; x(1) = 1. - .2 * i;
        square.inverseMap(x, r, rx);
        cout << "square:     x = (" << x(0) << "," << x(1) << "), r = (" << r(0) << "," << r(1) << "), rx = ("
             << rx(0,0) << "," << rx(1,0) << "," << rx(0,1) << "," << rx(1,1) << ")" << endl;
    } // end for

    for (i=0; i<=10; i++) {
        r(0) = .1 * i; r(1) = 1. - r(0);
        rectan.map(r, x, xr);
        cout << "rectangle:  r = (" << r(0) << "," << r(1) << "), x = (" << x(0) << "," << x(1) << "), xr = ("
             << xr(0,0) << "," << xr(1,0) << "," << xr(0,1) << "," << xr(1,1) << ")" << endl;
    } // end for

    for (i=0; i<=10; i++) {
        x(0) = .12 * i - .6; x(1) = .4 - .08 * i;
        rectan.inverseMap(x, r, rx);
        cout << "rectangle:  x = (" << x(0) << "," << x(1) << "), r = (" << r(0) << "," << r(1) << "), rx = ("
             << rx(0,0) << "," << rx(1,0) << "," << rx(0,1) << "," << rx(1,1) << ")" << endl;
    } // end for

    return 0;
}
