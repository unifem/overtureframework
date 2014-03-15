#include <INS++.h>

#define MAX(A,B) (A>B?A:B)
#define MIN(A,B) (A<B?A:B)

		//
		// Modified 940315 to be a member of the INS class
		// Modified 940315 to save multiple solutions
		//

INS::MakeShowFile(
		const char *disk_file_name,	const char *disk_directory_name,
		const char *new_file_name, 
                const char **show_titles, 	const char **plot_labels, 
		const char **variable_names,  	const int  *pointer_list)
{   

  printf ("INS::MakeShowFile called...\n");

  int gloc, sloc, ssloc, dims[3], dim[4], vloc;


    const int DISK_SIZE = 500000;
    int *disk = new int[DISK_SIZE];
    const int iounit = 6;
    int numberOfVariableNames = 0;
    
    int root_directory, error_number;

			  //
			  // Initialize DSK
			  //
    cdskini(disk, DISK_SIZE, root_directory, error_number);

			  //
			  // Define showfile and grid directories
			  //

    dims[0] = 0;
    cdskdef(disk, root_directory, "grid", "d", dims, gloc, error_number);
    cdskdef(disk, root_directory, "showfile", "d", dims, sloc, error_number);

  const int DISK_IO_UNITO = 11;
  const char *DISK_RECORD_DESCRIPTOR1 = " L4096";
  const char *DISK_RECORD_DESCRIPTOR0 = " I L4096";

  if (showFileCounter == 0){
    cdskmnt(disk, sloc,  new_file_name, DISK_IO_UNITO, DISK_RECORD_DESCRIPTOR0, error_number);
  }else{
    cdskmnt(disk, sloc,  new_file_name, DISK_IO_UNITO, DISK_RECORD_DESCRIPTOR1, error_number);
  }
   

  if (showFileCounter == 0){

    const int DISK_IO_UNIT = 10;
    const char *DISK_RECORD_DESCRIPTOR = " L4096";
    cdskmnt(disk, gloc,  disk_file_name, DISK_IO_UNIT, DISK_RECORD_DESCRIPTOR, error_number);

			  //
			  // Copy data file directory contents recursively into showfile
			  //
	    
    cdskcpy(disk, gloc, disk_directory_name, disk, sloc, disk_directory_name,
	   " R", error_number);
    cdskumt(disk,  gloc, error_number);


		//
		// Locate the composite grid directory on the file
		//

    ssloc = sloc;
    ssloc = cdskfnd (disk, ssloc, disk_directory_name);
	  if (ssloc == 0) {
	    printf ("MakeShowFile: Error finding ");
	    printf (disk_directory_name);
	    printf ("\n");
	    return (-1);
	  }
    ssloc = cdskfnd(disk, ssloc, "composite grid");
	    if (ssloc == 0) {
	      printf ("MakeShowFile: Error finding composite grid\n");
	      return (-2);
	    }
  
		  //
		  // Count and measure the show titles
		  //
    
    int numberOfHeaderLabels = 0, maxhsize = 0;
    while(*show_titles[numberOfHeaderLabels] != '\0'){
      maxhsize = MAX(maxhsize, strlen(show_titles[numberOfHeaderLabels]));
      numberOfHeaderLabels++;
    }

		  //
		  // Write out the show titles
		  //
      
    dim[0]  = 2; dim[1] = maxhsize; dim[2] = numberOfHeaderLabels;
    cdskdfs(disk, ssloc, "header", "S",  dim, vloc, error_number);
    int i,j;
    for(i=0;i<numberOfHeaderLabels;i++){
      j = i + 1;
      cdskps (disk, ssloc, "header", j, show_titles[i], error_number);
    }
    cdskrel(disk, ssloc, "header", " W", error_number);

		//
     		// Count the variable names 
		//

    int maxvsize = 0;
    while(*variable_names[numberOfVariableNames] != '\0'){
      maxvsize = MAX(maxvsize, strlen(variable_names[numberOfVariableNames]));
      numberOfVariableNames++;
    }   

		  //
		  // Write out the variable names and nv
		  //
    
    dims[0] = 0;  
    cdskdef(disk, ssloc, "nv", "i", dims, vloc, error_number);
    disk[vloc-1] = numberOfVariableNames;
    cdskrel(disk, ssloc, "nv", " W", error_number);

    dim[0] = 2; dim[1] = maxvsize; dim[2] = numberOfVariableNames+1;
    cdskdfs(disk, ssloc, "uvn.show", "S", dim, vloc, error_number);
       
    j = 1;
    cdskps(disk, ssloc, "uvn.show", j, "q", error_number);  // dummy
    
    for(i=0; i<numberOfVariableNames; i++){
      j = i + 2;
      cdskps(disk, ssloc, "uvn.show", j, variable_names[i], error_number);
    }  
    cdskrel(disk, ssloc, "uvn.show", " W", error_number);
    
    
  }

  showFileCounter++;
  
  ssloc = sloc;

		//
		// Locate the composite grid directory on the file
		//

  ssloc = cdskfnd (disk, ssloc, disk_directory_name);
	  if (ssloc == 0) {
	    printf ("MakeShowFile: Error finding");
	    printf (disk_directory_name);
	    printf ("\n");
	    return (-1);
	  }
  ssloc = cdskfnd(disk, ssloc, "composite grid");
	    if (ssloc == 0) {
	      printf ("MakeShowFile: Error finding composite grid\n");
	      return (-2);
	    }
		//
      		// Count and measure the plot labels
		//
	 
  int numberOfPlotLabels = 0, maxpsize = 0;
  while(*plot_labels[numberOfPlotLabels] != '\0'){
    maxpsize = MAX(maxpsize, strlen(plot_labels[numberOfPlotLabels]));
    numberOfPlotLabels++;
  }

		//
		// Write out the plot labels
		//
  
  char main_name[20]; 
  dim[0]  = 2; dim[1] = maxpsize; dim[2] = numberOfPlotLabels;
  char header_name[20]; 
    if (showFileCounter < 10) {
      sprintf (header_name, "header00000%d", showFileCounter);
      }
    else
      if  (showFileCounter < 100){
        sprintf (header_name, "header0000%d", showFileCounter);
	}
       else
	 if  (showFileCounter < 1000){
           sprintf (header_name, "header000%d", showFileCounter);
           }
  cdskdfs(disk, ssloc, header_name, "S",  dim, vloc, error_number);
  
  int i,j;
  for(i=0; i<numberOfPlotLabels; i++){
    j = i + 1;
    cdskps(disk, ssloc, header_name, j, plot_labels[i], error_number);
  }  
  cdskrel(disk, ssloc, header_name, " W", error_number);

		//
     		// Count the variable names again
		//

    int maxvsize = 0;
    while(*variable_names[numberOfVariableNames] != '\0'){
      maxvsize = MAX(maxvsize, strlen(variable_names[numberOfVariableNames]));
      numberOfVariableNames++;
    }   
  
		//
      		// Create composite grid functions
		//
  
  dim[0] = numberOfVariableNames;

    if (showFileCounter < 10) {
      sprintf (main_name, "u00000%d", showFileCounter);
      }
    else
      if  (showFileCounter < 100){
        sprintf (main_name, "u0000%d", showFileCounter);
	}
       else
	 if  (showFileCounter < 1000){
           sprintf (main_name, "u000%d", showFileCounter);
           }
	

//  ccgvdef(disk, ssloc, "u000001", " Tr D(*)", dim, vloc, error_number);
  ccgvdef(disk, ssloc, main_name, " Tr D(*)", dim, vloc, error_number);
//  ccgvout(disk, ssloc, main_name, " Od", iounit, error_number);
  
  char namefn[20];
  for(i=0; i<numberOfVariableNames; i++) {
//    sprintf(namefn, "u000001.%d", i+1);

    if (showFileCounter < 10){
      sprintf (namefn, "u00000%d.%d", showFileCounter,i+1);
      } 
    else
      if  (showFileCounter < 100){
        sprintf (namefn, "u0000%d.%d", showFileCounter,i+1);
        } 
	else
	  if  (showFileCounter < 1000){
      	    sprintf (namefn, "u000%d.%d", showFileCounter,i+1);
            }

     dim[0] = i+1;
     ccgvlnk(disk, ssloc, main_name, namefn, " Tr L(*)", 
       dim, vloc, error_number);    
//     ccgvout(disk, ssloc, namefn, " Od", iounit, error_number);
  }
  
		//
		// Find data to copy
		//
  
  //OvertureComponentGrid **GA = CompositeGridArray[0]->getGridArray();

  OvertureComponentGrid **GA = getGridArray();

  REAL *data;
  int   *ndrsabptr;
  int gfloc;
  
  for(i=0; i<ng; i++){  

		//
		// Compute dimension of each grid function on each grid
		//
  
    ndrsabptr = GA[i]->getNdrsab();
    int array_length = 1;
    for(int j=0; j<nd; j++){  
      array_length *= ndrsabptr[j+nd] - ndrsabptr[j] + 1;
    }

		//
		// Copy data to show file
		//

    for(j=0; j<numberOfVariableNames; j++){   // loop over all variables 
      data = GA[i]->getDataptr(pointer_list[j]);
//      sprintf(namefn, "u000001.%d", j+1);

    if (showFileCounter < 10){
      sprintf (namefn, "u00000%d.%d", showFileCounter,j+1);
      } 
      else
	if  (showFileCounter < 100){
          sprintf (namefn, "u0000%d.%d", showFileCounter,j+1);
          } 
	  else
	    if  (showFileCounter < 1000){
              sprintf (namefn, "u000%d.%d", showFileCounter,j+1);
              }

      gfloc = cdskfnd(disk, ssloc, namefn) - 1;
      gfloc = disk[gfloc+i] - 1;
      float *fd = (float *) &(disk[gfloc]);
      for(int k=0; k<array_length; k++) {
        *fd++ = *data++;
      }
      cdskrel(disk, ssloc, namefn, " W", error_number);
    }
  }
  
		//
		// Deallocate Memory and dismount disk
		//

  cdskumt(disk,  sloc, error_number);
  delete[] disk;

  return (0);
}

