From bljones@ncsa.uiuc.edu Thu Mar 21 15:01:42 1996
Date: Thu, 21 Mar 1996 16:01:16 -0600
To: henshaw@lanl.gov
Subject: Re: SDS in vgroups
From: hdfhelp@ncsa.uiuc.edu

Hi Bill,

>Request: I am a new user of hdf. I am using version 4.0r1
>I would like to create a hierarchical
>data base where nodes are scientific data sets (and other
>types of objects). If I open a
>file will Hopen to create vgroups then it is not clear to
>me how I can add a SDS to the vgroup since the SDS requires
>the file to be opened with SDstart. I have tried to open
>the same file with both Hopen and SDstart but this doesn't
>seem to work(?) Any suggestions would be appreciated.

The attached program should be helpful.  Let me know if you 
are still having problems.

Barbara Jones
hdfhelp@ncsa.uiuc.edu
-----------------------------------------
/*  this program creates an hdf file, creates a vgroup and 2 empty 
 *  SDS of the same dimension sizes.
 *  Add the 1st SDS into the vgroup, and close the file.
 */

#include "mfhdf.h"
#include <stdio.h>

main()  
{
   int i, ret, sds_idx, start[1], stride[1], edge[1];
   int32 fid, sds_id, sref;
   int32 dims[1];
   int32 vfid, vg_id;  
   uint8 data[10];

   dims[0] = 10;

/* init SD and Vset interfaces, create mysd1 and vg  */
   fid = SDstart("mysd_vs.hdf", DFACC_CREATE);
   vfid = Hopen("mysd_vs.hdf", DFACC_RDWR, 0);
   sds_id = SDcreate(fid, "mysd1", DFNT_INT8, 1, dims);
   Vstart(vfid);
   vg_id = Vattach(vfid, -1, "w"); 
   printf("fid= %d, sds_id= %d, vfid=%d, vg_id=%d\n", fid, sds_id, vfid, vg_id);
/* add mysd1 into vg  */
   sref = SDidtoref(sds_id);
   Vaddtagref(vg_id, DFTAG_NDG, sref); 
   printf("fid= %d, sds_id= %d\n", fid, sds_id);
/* flush out mysd1*/
   SDendaccess(sds_id);
/* end Vset interface  */ 
   Vdetach(vg_id);
   Vend(vfid);
   Hclose(vfid);
/* create mysd2  */
   sds_id = SDcreate(fid, "mysd2", DFNT_INT8, 1, dims);
   printf("fid= %d, sds_id= %d\n", fid, sds_id);
   SDendaccess(sds_id);
/* close hdf file */
   SDend(fid);
/* open again and write data into mysd1 */
   fid = SDstart("mysd_vs.hdf", DFACC_RDWR);
   sds_idx = SDnametoindex(fid, "mysd1");
   sds_id = SDselect(fid, sds_idx);
   for (i=0; i<10; i++)
       data[i] = i+10;
   start[0] = 0;
   stride[0] = 1;
   edge[0] = 10;
   ret = SDwritedata(sds_id, start, stride, edge, data);
   SDendaccess(sds_id);
   SDend(fid); 
}
 



