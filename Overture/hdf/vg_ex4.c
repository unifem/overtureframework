#include "hdf.h"

main( )
{
    int32    file_id;
    int32    vgroup_id, vgroup_ref;
    int32    vdata_tag, vdata_ref;
    int32    status, i, npairs;

    /* Open the "Example2.hdf" file. */
    file_id = Hopen("Example2.hdf", DFACC_READ, 0);

    /* Initialize HDF for subsequent Vgroup/Vdata access. */
    Vstart(file_id);

    /* Attach to every Vgroup in the file. */
    vgroup_ref = -1;
    while (TRUE) {
       vgroup_ref = Vgetid(file_id, vgroup_ref);

       if (vgroup_ref == -1) break;
       vgroup_id = Vattach(file_id, vgroup_ref, "r"); 

       /* Get the total number of tag/reference id pairs. */
       npairs = Vntagrefs(vgroup_id);

       /* Print every tag and reference id with their 
          corresponding file position. */
       for (i = 0; i < npairs; i++) {
          status = Vgettagref(vgroup_id, i, &vdata_tag, &vdata_ref);
          printf("Found tag = %d, ref = %d at position %d.\n", \
                  vdata_tag, vdata_ref, i+1);
       }

       /* Terminate access to the Vgroup. */
       Vdetach(vgroup_id);
    }

    /* Terminate access to the Vgroup interface and close the file. */
    Vend(file_id);
    Hclose(file_id);

}


