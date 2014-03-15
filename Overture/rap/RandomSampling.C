#include "RandomSampling.h"
#include "math.h"

//
// C++'ization of Bill Bateson's Dune RandomSampling module
// derived from something Mal Kalos developed
//
//
// author  = 'Dale Slone<slone3@llnl.gov>'
// version  = '1.0.0'
// date  = 'May 1, 2004'
//

RandomSampling::RandomSampling() {
    // No, I don't know where these specifice values come from

    m1 =  502;
    m2 = 1521;
    m3 = 4071;
    m4 = 2107;
    n1 =    0;
    n2 =    0;
    n3 = 2896;
    n4 = 1263;
    l1 =    0;
    l2 =    0;
    l3 = 2896;
    l4 = 1263;
    tpm12 = 1.0 / 4096.0;
}

/*
double RandomSampling::RandomDouble() {
    //
    // RandomDouble() returns a uniformly distributed value in [0.0, 1.0)
    //

    return this->DRanN();
}
*/

double RandomSampling::RandomDouble() {
    //
    // DRanN() generates a uniformly distributed value in [0.0, 1.0),
    // saving the state values
    //

    int i1, i2, i3, i4;
    double DRanN;

    i1 = this->l1*this->m4 + this->l2*this->m3 +
	 this->l3*this->m2 + this->l4*this->m1 + this->n1;
    i2 = this->l2*this->m4 + this->l3*this->m3 + this->l4*this->m2 + this->n2;
    i3 = this->l3*this->m4 + this->l4*this->m3 + this->n3;
    i4 = this->l4*this->m4 + this->n4;
    this->l4 = i4 & 4095;
    i3 = i3 + (i4 >> 12);
    this->l3 = i3 & 4095;
    i2 = i2 + (i3 >> 12);
    this->l2 = i2 & 4095;
    this->l1 = (i1 + (i2 >> 12)) & 4095;
    DRanN = this->tpm12*(double(this->l1) + this->tpm12*(double(this->l2) +
            this->tpm12*(double(this->l3) + this->tpm12*double(this->l4))));

    return DRanN;
}

int RandomSampling::RandomInteger(int nRange=1) {
    //
    // RandomInteger() returns an interger uniformly distributed in [1, nRange]
    //

    return (int)min(1 + int(double(nRange) * this->RandomDouble()), nRange);
}

double* RandomSampling::RandomCosines() {
    //
    // RandomCosines() returns a list of (x, y, z) triples uniformly
    // distributed in [-1.0, 1.0).
    //

    double *Cosines = new double[3];
    double Nrm;

    //  Initial sample in box and "z" component.
    Cosines[0] = 2.0 * this->RandomDouble() - 1.0;
    Cosines[1] = 2.0 * this->RandomDouble() - 1.0;
    Cosines[2] = 2.0 * this->RandomDouble() - 1.0;

    //  Normalization of "x" and "y" direction.

    Nrm = Cosines[0]*Cosines[0] + Cosines[1]*Cosines[1];

    //  Vector is within unit sphere.
    while ( (Nrm == 0.0) || (Nrm > 1.0) ) {
	Cosines[0] = 2.0 * this->RandomDouble() - 1.0;
	Cosines[1] = 2.0 * this->RandomDouble() - 1.0;
	Nrm = Cosines[0]*Cosines[0] + Cosines[1]*Cosines[1];
    }

    //  Normalize "x" & "y" components.
    Nrm = sqrt( (1.0 - Cosines[2]*Cosines[2]) / Nrm );
    Cosines[0] = Cosines[0] * Nrm;
    Cosines[1] = Cosines[1] * Nrm;

    return Cosines;
}

double* RandomSampling::Random2DCosines() {
    //
    // RandomCosines() returns a list of (x, y) doubles uniformly
    // distributed in [-1.0, 1.0).
    //

    double *Cosines = new double[3];
    double Nrm;

    //  Initial sample in box and "z" component.
    Cosines[0] = 2.0 * this->RandomDouble() - 1.0;
    Cosines[1] = 2.0 * this->RandomDouble() - 1.0;

    //  Normilization of "x" and "y" direction.

    Nrm = Cosines[0]*Cosines[0] + Cosines[1]*Cosines[1];

    //  Vector is within unit sphere.
    while ( (Nrm == 0.0) || (Nrm > 1.0) ) {
	Cosines[0] = 2.0 * this->RandomDouble() - 1.0;
	Cosines[1] = 2.0 * this->RandomDouble() - 1.0;
	Nrm = Cosines[0]*Cosines[0] + Cosines[1]*Cosines[1];
    }

    //  Normalize "x" & "y" components.
    Nrm = sqrt( 1.0 / Nrm );
    Cosines[0] = Cosines[0] * Nrm;
    Cosines[1] = Cosines[1] * Nrm;

    return Cosines;
}

