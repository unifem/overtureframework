#include "hdf.h"

main( )
{

    int32    file_id, vgroup_id, vdata_id;
    float    pxy[30][2] = {-1.5,  2.3, -1.5, 1.98, -2.4, .67,
                           -3.4, 1.46, -.65, 3.1, -.62, 1.23,
                            -.4,  3.8, -3.55, 2.3, -1.43, 2.44, 
                            .23, 1.13, -1.4, 5.43, -1.4, 5.8,
                           -3.4, 3.85, -.55, .3, -.21, 1.22,
                           -1.44, 1.9, -1.4, 2.8, .94, 1.78,
                           -.4, 2.32, -.87, 1.99, -.54, 4.11,
                           -1.5, 1.35, -1.4, 2.21, -.22, 1.8,
                           -1.1, 4.55, -.44, .54, -1.11, 3.93,
                           -.76, 1.9, -2.34, 1.7, -2.2, 1.21};
    float   temp[30]; 
    int8    mesh[20][3];
    int8    i, j, k = 0;    

    /* Open an HDF file with full access. */
    file_id = Hopen("Example2.hdf", DFACC_CREATE, 0);

    /* Initialize HDF for subsequent Vgroup/Vdata access. */
    Vstart(file_id);

    /* Initialize the data buffer arrays. */
    for (i = 0; i < 30; i++)
       temp[i] = i * 10.0;

    for (i = 0; i < 20; i++) {
       for (j = 0; j < 3; j++) {
          mesh[i][j] = ++k;
       }
    }

    /* Create a Vgroup with write access, then name it "Vertices". */
    vgroup_id = Vattach(file_id, -1, "w");
    Vsetname(vgroup_id, "Vertices");

    /* Create a Vdata to store the x,y values, set its name and class. */
    vdata_id = VSattach(file_id, -1, "w");
    VSsetname(vdata_id, "PX,PY");
    VSsetclass(vdata_id, "Node List");

    /* Specify the Vdata data type, name and the order. */ 
    VSfdefine(vdata_id, "PX,PY", DFNT_FLOAT32, 2);

    /* Set the field names. */
    VSsetfields(vdata_id, "PX,PY");

    /* Write the buffered data into the Vdata object. */
    VSwrite(vdata_id, (VOIDP)pxy, 30, FULL_INTERLACE);

    /* Insert the Vdata into the Vgroup. */
    Vinsert(vgroup_id, vdata_id);

    /* Detach from the Vdata. */
    VSdetach(vdata_id);

    /* Create a Vdata to store the temperature property data. */
    vdata_id = VSattach(file_id, -1, "w");
    VSsetname(vdata_id, "PLIST");
    VSsetclass(vdata_id, "Connectivity List");
    VSfdefine(vdata_id, "PLIST", DFNT_FLOAT32, 1);
    VSsetfields(vdata_id, "PLIST");
    VSwrite(vdata_id, (VOIDP)temp, 30, FULL_INTERLACE);
    Vinsert(vgroup_id, vdata_id);
    VSdetach(vdata_id);

    /* Create a Vdata to store the mesh. */
    vdata_id = VSattach(file_id, -1, "w");
    VSsetname(vdata_id, "TMP");
    VSsetclass(vdata_id, "Property List");
    VSfdefine(vdata_id, "TMP", DFNT_INT8, 3);
    VSsetfields(vdata_id, "TMP");
    VSwrite(vdata_id, (VOIDP)mesh, 20, FULL_INTERLACE);
    Vinsert(vgroup_id, vdata_id);
    VSdetach(vdata_id);

    /* Terminate access to the "Vertices" Vgroup. */
    Vdetach(vgroup_id);

    /* Terminate access to the Vgroup interface. */
    Vend(file_id);

    /* Close the HDF file. */
    Hclose(file_id);

}

