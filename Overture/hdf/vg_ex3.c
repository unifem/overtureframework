#include "hdf.h"

main( )
{

     int32     file_id;
     int32     vgroup_ref, vgroup_id;
     int32     maxsize, i;
     int32     *n_entries, *ref_array; 
     char      vgroup_name[VGNAMELENMAX];

     /* Open the "Example2.hdf" file. */
     file_id = Hopen("Example2.hdf", DFACC_READ, 0); 

     /* Initialize HDF for subsequent Vgroup/Vdata access. */
     Vstart(file_id);

     /* Get and print the reference numbers of all the lone 
        Vgroups.  First, call Vlone with maxsize set to 0 to 
        get the length of the storage array, then call Vlone 
        again to put the reference id numbers into the array.  */
     maxsize = Vlone(file_id, ref_array, 0);
     ref_array = (int32 *) malloc(sizeof(int32) * maxsize);
     Vlone(file_id, ref_array, maxsize);
     for (i = 0; i < maxsize; i++) {
        printf("Lone Vgroup reference id  %d\n", ref_array[i]); 
        printf("*******\n");
        free(ref_array);
     }

     /* Set the reference number variable to start the search
        at the first Vgroup in the file. */ 
     vgroup_ref = -1;

     /* Print every reference id in the file. */ 
     while (TRUE) {
        vgroup_ref = Vgetid(file_id, vgroup_ref);
        if (vgroup_ref == -1) break; 
        vgroup_id = Vattach(file_id, vgroup_ref, "r");
        Vinquire(vgroup_id, n_entries, vgroup_name);
           printf("Found Vgroup with ref %d, number of entries %d, \
                    name %s\n", vgroup_ref, n_entries, vgroup_name); 
          Vdetach(vgroup_id);
     } 

     /* Terminate access to the Vgroup interface and the file. */
     Vend(file_id);
     Hclose(file_id);

}


