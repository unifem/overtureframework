#include "hdf.h"

#define HEIGHT 6
#define WIDTH 5

main( ) 
{ 

     int32    file_id, vgroup_id, vdata_id; 
     uint16   tag, ref;

     /* Construct the image to be written to the Vgroup. */
     static      uint8      raster_data[HEIGHT][WIDTH] = \
                                { 1, 2, 3,  4,  5, 
                                  6,  7,  8,  9, 10,
                                  11, 12, 13, 14, 15,   
                                  16, 17, 18, 19, 20,
                                  21, 22, 23, 24, 25,
                                  26, 27, 28, 29, 30 };
     /* Open an HDF file with full access. */
     file_id = Hopen("Example1.hdf", DFACC_CREATE, 0);

     /* Initialize HDF for subsequent Vgroup/Vdata access. */
     Vstart(file_id);

     /* Create a Vgroup. */
     vgroup_id = Vattach(file_id, -1, "w"); 

     /* Set the name and class for this Vgroup. */
     Vsetname(vgroup_id, "VG_Name_1");
     Vsetclass(vgroup_id, "VG_Class_1");

     /* Write the data to file and determine its tag and ref number. */
     DFR8addimage("Example1.hdf", (VOIDP)raster_data, WIDTH, HEIGHT, 0);
     ref = DFR8lastref( );

     /* This tag definition is from hdf.h. */
     tag = DFTAG_RI8;

     /* Insert the data image into the Vgroup. */
     Vaddtagref(vgroup_id, tag, ref);

     /* Terminate access to the Vgroup interface. */
     Vdetach(vgroup_id);
     Vend(file_id);

     /* Close the HDF file. */
     Hclose(file_id);
     
}
     
     
