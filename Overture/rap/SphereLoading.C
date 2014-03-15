#include "SphereLoading.h"

SphereLoading::SphereLoading(int sizeOfDistribution /* =1 */ , bool isFromGUI /* = true */ ) 
// =============================================================================================
// /Description:
//    Build an object that defines the the spheres that fill a liner geometry. 
// =============================================================================================
{
   sphereDistribution.resize(sizeOfDistribution,2);
   volumeFraction = 0.10;
   for (int i = 0; i < sizeOfDistribution; i++) {
       sphereDistribution(i, 0) = 0.1 / i;
       sphereDistribution(i, 1) = 1.0 / i;
   }
   fromGUI = isFromGUI;
   RNGSeed = 0;
}

SphereLoading::
~SphereLoading()
{
}


void SphereLoading::resize(int &numberOfSpheres) {
   sphereCenter.resize(numberOfSpheres,3);
   sphereRadius.resize(numberOfSpheres);
   sphereVelocity.resize(numberOfSpheres,3);
   sphereStartTime.resize(numberOfSpheres);
}

void SphereLoading::updateDistribution(int num, 
                                       RealArray radii, 
                                       RealArray probabilities) {
    sphereDistribution.resize(num,2);
    for (int i = 0; i < num; i++) {
        sphereDistribution(i, 0) = radii(i);
        sphereDistribution(i, 1) = probabilities(i);
    }
}

void SphereLoading::printSpheres(aString &name) {
    const int numberOfSpheres = sphereCenter.getLength(0);

    if (strlen(name) > 0) {
        printf(" NAME = \"%s", name.c_str());
    }
    else{
        printf(" NAME = \"Fragments Sinked in Time");
    }
    if (RNGSeed != 0) {
        printf("[ RNG seed = %d]");
    }
    printf("\"\n");
    printf(" VARIABLES = \"R\", \"Xi\", \"Xj\", \"Xk\", " 
           "\"dVImpi\", \"dVImpj\", \"dVImpk\", \"tImp\"\n");
    printf("\n");
    printf(" NSPHERES =  %d F = \"POINT\"\n", 
           numberOfSpheres);

    for (int i = 0; i < numberOfSpheres; i++) {
        printf("%e %e %e %e %e %e %e %e\n", 
               sphereRadius(i),
               sphereCenter(i,0), sphereCenter(i,1), sphereCenter(i,2),
               sphereVelocity(i,0), sphereVelocity(i,1), sphereVelocity(i,2),
               sphereStartTime(i));
    }
}

void SphereLoading::printTecPlotSpheres(aString &name) {
    const int numberOfSpheres = sphereCenter.getLength(0);

    printf("\n\n");
    if (strlen(name) > 0) {
        printf(" TITLE = \"%s", name.c_str());
    }
    else{
        printf(" TITLE = \"Fragments Sinked in Time");
    }
    if (RNGSeed != 0) {
        printf("[ RNG seed = %d]");
    }
    printf("\"\n\n");
    printf(" VARIABLES = \"iSpc\", \"R\", \"Xi\", \"Xj\", \"Xk\", \"Vi\", " 
           "\"Vj\", \"Vk\", \"Wi\", \"Wj\", \"Wk\"");
//     Dune isn't printing the impulses yet
//     printf(" VARIABLES = \"iSpc\", \"R\", \"Xi\", \"Xj\", \"Xk\", \"Vi\", " 
//            "\"Vj\", \"Vk\", \"Wi\", \"Wj\", \"Wk\", \"dVImpi\", \"dVImpj\","
//            " \"dVImpk\", \"tImp\"\n");
    printf("\n\n\n");
    printf(" ZONE T=\" 0.00000000D+00\" I =   %d F = \"POINT\"\n", 
           numberOfSpheres);

    const int iSpc = 1;
    const double w = 0.0;
    const double v = 0.0;

    for (int i = 0; i < numberOfSpheres; i++) {
        printf(" %d %e %e %e %e %e %e %e %e %e %e\n", 
               iSpc, sphereRadius(i),
               sphereCenter(i,0), sphereCenter(i,1), sphereCenter(i,2),
               v, v, v, w, w, w);
//         printf(" %d %e %e %e %e %e %e %e %e %e %e %e %e %e %e\n", 
//                iSpc, sphereRadius(i),
//                sphereCenter(i,0), sphereCenter(i,1), sphereCenter(i,2),
//                v, v, v, w, w, w,
//                sphereVelocity(i,0), sphereVelocity(i,1), sphereVelocity(i,2),
//                sphereStartTime(i));
    }
}

void SphereLoading::printDuneSpheres(aString &name) {
    //const int numberOfSpheres = sphereCenter.getLength(0);

    printf("Fragments{\n");

    char dummyFragment[] = "  Fragment[\n"
                           "     Species( \"Red\" ),\n"
                           "     Material( \"Steel\" ),\n"
                           "     Sphere[\n"
                           "        R( ";
    char x[]             = "     X(";
    char v[]             = "     V(";
    char w[]             = "     W(";
    char q[]             = "     Q(";
    char t[]             = "     Time(";
    char impulse[]       = "     Impulse["; 
    char tImp[]          = "        Time(";
    char dVImp[]         = "        DV(";

    const double wval = 0.0;
    const double vval = 0.0;
    const double tval = 0.0;
    const double qval = 0.0;

    for (int i = 0; i < sphereCenter.getLength(0); i++) {
        printf("%s %13.9e ),\n        ],\n", dummyFragment, sphereRadius(i));
        printf("%s %13.9e, %13.9e, %13.9e ),\n",
               x, sphereCenter(i,0), sphereCenter(i,1), sphereCenter(i,2));
        printf("%s %13.9e, %13.9e, %13.9e ),\n", v, vval, vval, vval);
        printf("%s %13.9e, %13.9e, %13.9e, %13.9e ),\n", q, qval, qval, qval, qval);
        printf("%s %13.9e, %13.9e, %13.9e ),\n", w, wval, wval, wval);
        printf("%s %13.9e ),\n", t);
        printf("%s\n", impulse);
        printf("%s %13.9e ),\n", tImp, sphereStartTime(i));
        printf("%s %13.9e, %13.9e, %13.9e ),\n     ],\n  ],\n", 
               dVImp, sphereVelocity(i,0), sphereVelocity(i,1), 
               sphereVelocity(i,2));
    }

    printf("}\n");
}
