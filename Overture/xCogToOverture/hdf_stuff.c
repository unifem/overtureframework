#include "hdf_stuff.h"

#ifndef HDF_DUMMY /* The real HDF-routines */

static int file_id=-1, sd_id=-1;
static char *access_mode;

int32
open_hdf_file(char * name, char access){
  int32 root, ref;
  char vname[VGNAMELENMAX];

  if (file_id > 0){
    printf("open_hdf_file: ERROR: cannot open an open file!\n");
    return -1;
  }

  if (access == 'i'){
/* open a HDF file with full access, overwrite the file if it exists */
    access_mode = (char*)"w";
/* setup the SD-interface */
    if ((sd_id = SDstart(name, DFACC_CREATE)) <= 0){
      return -1;
    }
/* open the H-interface */
    file_id = Hopen(name, DFACC_RDWR, 0);
    Vstart(file_id);
/* attach the root vgroup */
    root = Vattach(file_id, -1, access_mode);
/* Set the name and class for the root directory. */
    Vsetname(root, "root");
    Vsetclass(root, "directory");
  }
  else if (access == 'r'){
/* open a HDF file with read permission */
    access_mode = (char*)"r";
/* setup the SD-interface */
    if ((sd_id = SDstart(name, DFACC_READ)) <= 0){
      return -1;
    }
/* open the H-interface */
    file_id = Hopen(name, DFACC_READ, 0);
    Vstart(file_id);

/* attach the root -- here we assume that it is the first vgroup in file */
    ref = Vgetid(file_id, -1);
    root = Vattach(file_id, ref, access_mode); 
    Vgetname(root,vname);
    if( root <= 0 || strcmp(vname,"root") ){
      printf("open_hdf_file: ERROR: There is no `root' directory in this "
	     "dataBase file!\n");
      return -1;
    }
  }
  else{
    printf("open_hdf_file: ERROR: Unknown access mode %c \n", access);
    return -1;
  }


  return root;
}

int32
get_file_id(void){
  return file_id;
}

int32
get_sd_id(void){
  return sd_id;
}

void
close_hdf_file(int32 root){
  if (file_id == -1){
    printf("close_hdf_file: ERROR: Cannot close a closed file!\n");
    return;
  }
/* detach the root vgroup */
  Vdetach(root);
/* cleanup the v-interface*/
  Vend(file_id);
/* close the H-interface */
  Hclose(file_id);
/* close the SD-interface */
  SDend(sd_id);

/* mark the file as beeing closed */
  file_id = sd_id = -1;
}

int32
create_dir(char * name, char * class_name, int vgroup_id){
/* ============================================================================= */
/*  Purpose: create a sub-directory */
/*  OBSERVE: You must explicitly call Vdetach to properly end the access to */
/*  the directory */
/*  name (input): name of the sub-directory; */
/*  class_name (input): name of the class for the directory  */
/*  return value: is the sub_directory value if the directory was successfully */
/*  created, -1 otherwise */
/* ============================================================================= */
  int32 sub_directory;

  if (file_id == -1){
    printf("ERROR: There is no open database file!\n");
    return -1;
  }

/* Create a Vgroup. */
  if ( (sub_directory = Vattach(file_id, -1, "w"))<=0 ){
    printf("create: FATAL ERROR in creating a new directory!\n");
    return -1;
  }    
/* Set the name and class for this Vgroup. */
  Vsetname(sub_directory, name);
  Vsetclass(sub_directory, class_name);

/* Insert the sub-directory into the vgroup_id directory. */
  Vinsert(vgroup_id, sub_directory);

  return sub_directory;
}

int32
locate_dir(char * name, int vgroup_id){
/* ============================================================================= */
/*  Purpose: locate a sub-directory with a given name */
/*  OBSERVE: You must explicitly call Vdetach to properly end the access to */
/*  the directory */
/*  name (input): name of the sub-directory */
/*  return value: is the directory id if the directory was found, -1 otherwise */
/* ============================================================================= */
  int32 tag, ref, sub_directory=-1;
  char vname[VGNAMELENMAX];
  int found=FALSE, npairs, i;

  if (file_id == -1){
    printf("ERROR: There is no open database file!\n");
    return -1;
  }

/* get the total number of tag/reference pairs in the vgroup */
  npairs = Vntagrefs(vgroup_id);
  for(i=0; i<npairs; i++ ){
/* get tag and ref */
    /* wdh status = Vgettagref(vgroup_id, i, &tag, &ref ); */
    Vgettagref(vgroup_id, i, &tag, &ref );
    if( Visvg(vgroup_id, ref) ){
/* this is a vgroup */
      sub_directory = Vattach(file_id, ref, access_mode);
      Vgetname(sub_directory,vname);
      if( !strcmp(name, vname) ){
	found=TRUE;
	break;
      }
      else
        Vdetach(sub_directory);
    }
  } /* end for all i */
  if( !found ){
    /* wdh printf("locate: ERROR: unable to find directory %s\n", name); */
    sub_directory=-1;
  } /* end if not found */

  return sub_directory;
}

/* define a macro for writing one int/float/double into the vgroup `vgroup_id' */
#undef PUT
#define PUT(type,HDFType) \
int \
hput_ ## type ## ( type x, char *name, int vgroup_id ){  \
  int num=1; \
  int32 vdata_id; \
 \
  if (file_id == -1){\
    printf("ERROR: There is no open database file!\n");\
    return -1;\
  }\
 \
/* Create a Vdata to store the array values, set its name and class. */  \
  vdata_id = VSattach(file_id, -1, access_mode); \
  VSsetname(vdata_id, name); \
/* Specify the Vdata data type, name and the order.  */ \
  VSfdefine(vdata_id, name, HDFType, 1); \
/* Set the field names.  */ \
  VSsetfields(vdata_id, name); \
/* Write the buffered data into the Vdata object.  */ \
  VSwrite(vdata_id, (unsigned char*)(&x), num, FULL_INTERLACE); \
/* Insert the Vdata into the Vgroup.  */ \
  Vinsert(vgroup_id, vdata_id); \
/* Detach from the Vdata. */ \
  VSdetach(vdata_id); \
  return TRUE; \
}

/* now declare instances of the macro. This will define the fuctions */
/* put_int, put_real, put_float and put_double */
PUT(int,DFNT_INT32)
#ifndef NO_REAL
PUT(real,DFNT_REAL)
#endif
PUT(float,DFNT_FLOAT32)
PUT(double,DFNT_FLOAT64)
#undef PUT

/* define a macro for getting one int/float/double from the vgroup `vgroup_id' */
#undef GET
#define GET(type,HDFType) \
int \
hget_ ## type ## ( type *x, char *name, int vgroup_id ) { \
  int npairs, found=FALSE, i; \
  int32 vdata_tag, vdata_ref, vdata_id, n_records, nt; \
  char vdata_name[VSNAMELENMAX]; \
 \
  if (file_id == -1){ \
    printf("ERROR: There is no open database file!\n"); \
    return -1; \
  } \
 \
/* get the total number of tag/reference pairs in the vgroup  */  \
  npairs = Vntagrefs(vgroup_id); \
  for(i=0; i<npairs; i++ ){ \
/* get tag and ref  */ \
     /* status = */ Vgettagref(vgroup_id, i, &vdata_tag, &vdata_ref ); \
    if( Visvs(vgroup_id, vdata_ref) ){ /* this is a vdata  */ \
/* get identifier for vdata */ \
      vdata_id=VSattach(file_id, vdata_ref, access_mode); \
/* get name of the vdata  */ \
      VSgetname(vdata_id, vdata_name); \
/* get type of first field */ \
      nt = VFfieldtype(vdata_id, 0); \
/* both the name and the number type must match! */ \
      if(!strcmp(name, vdata_name) && nt == HDFType){ \
	found=TRUE; \
	VSQuerycount(vdata_id, &n_records); \
	VSread(vdata_id, (unsigned char*)x, n_records, FULL_INTERLACE ); \
        VSdetach(vdata_id); \
        break; \
      } \
      VSdetach(vdata_id); \
    } \
  } \
  if( !found ){ \
     printf("hget_" #type ": ERROR searching for %s\n", name); \
  } \
  return found; \
} 
/* now declare instances of the macro. This will define the fuctions */
/* get_int, get_float and get_double */
GET(int,DFNT_INT32)
#ifndef NO_REAL
GET(real,DFNT_REAL)
#endif
GET(float,DFNT_FLOAT32)
GET(double,DFNT_FLOAT64)
#undef GET

/* define a macro for writing a int/real c_array of rank 1 into the vgroup `vgroup_id' */
#undef PUT
#define PUT(type, HDFType) \
int \
hput_ ## type ## _array_1d( type ## _array_1d * x, char *name, int vgroup_id ){\
  const int32 rank=1; \
  int32 dims[1], start[1], edges[1], base[1], sds_id, ref; \
  int n; \
 \
 \
  if (file_id == -1){ \
    printf("ERROR: There is no open database file!\n"); \
    return -1; \
  } \
 \
/* observe that the c_array package uses a fortran-like convention for */ \
/* the indices. For instance the first index satisfies 1 <= i <= x->n1. */ \
  dims[0]=x->n1; \
 \
  for( n=0; n < rank; n++ ) \
  { \
    start[n]=0; \
    edges[n]=dims[n]; \
    base[n]=1; \
  }  \
/* create the array */ \
  sds_id = SDcreate(sd_id, name, HDFType, rank, dims); \
/* istat = */ SDwritedata(sds_id, start, NULL, edges, (unsigned char*)(x->arrayptr)); \
/* Save array lower bounds to be compatible with Bill Henshaw */  \
  SDsetattr(sds_id, (char*)"arrayBase", DFNT_INT32, rank, base ); \
/* Insert the sds into the current Vgroup.  */  \
  ref = SDidtoref(sds_id);  \
  Vaddtagref(vgroup_id, DFTAG_NDG, ref ); \
/* terminate access to the array */ \
/*  istat = */ SDendaccess(sds_id); \
  return TRUE; \
}
/* now declare instances of the macro. This will define the fuctions */
/* put_int_array_1d and put_real_array_1d. */
PUT(int, DFNT_INT32)
PUT(float, DFNT_FLOAT)
PUT(double, DFNT_DOUBLE)
#ifndef NO_REAL
PUT(real, DFNT_REAL)
#endif
#undef PUT

/* define a macro for writing a int/real c_array of rank 2 into the vgroup `vgroup_id' */
#undef PUT
#define PUT(type, HDFType) \
int \
hput_ ## type ## _array_2d( type ## _array_2d * x, char *name, int vgroup_id ){\
  const int32 rank=2; \
  int32 dims[2], start[2], edges[2], base[2], sds_id, ref; \
  int n; \
 \
 \
  if (file_id == -1){ \
    printf("ERROR: There is no open database file!\n"); \
    return -1; \
  } \
 \
/* observe that the c_array package uses a fortran-like convention for */ \
/* the indices. For instance the first index satisfies 1 <= i <= x->n1. */ \
  dims[0]=x->n2; \
  dims[1]=x->n1; \
 \
  for( n=0; n < rank; n++ ) \
  { \
    start[n]=0; \
    edges[n]=dims[n]; \
    base[n]=1; \
  }  \
/* create the array */ \
  sds_id = SDcreate(sd_id, name, HDFType, rank, dims); \
 /* istat = */ SDwritedata(sds_id, start, NULL, edges, (unsigned char*)(x->arrayptr)); \
/* Save array lower bounds to be compatible with Bill Henshaw */  \
  SDsetattr(sds_id, (char*)"arrayBase", DFNT_INT32, rank, base ); \
/* Insert the sds into the current Vgroup.  */  \
  ref = SDidtoref(sds_id);  \
  Vaddtagref(vgroup_id, DFTAG_NDG, ref ); \
/* terminate access to the array */ \
  /* istat = */ SDendaccess(sds_id); \
  return TRUE; \
}
/* now declare instances of the macro. This will define the fuctions */
/* put_int_array_2d and put_real_array_2d. */
PUT(int, DFNT_INT32)
PUT(float, DFNT_FLOAT)
PUT(double, DFNT_DOUBLE)
#ifndef NO_REAL
PUT(real, DFNT_REAL)
#endif
#undef PUT

/* define a macro for writing a int/real c_array of rank 3 into the vgroup `vgroup_id' */
#undef PUT
#define PUT(type, HDFType) \
int \
hput_ ## type ## _array_3d( type ## _array_3d * x, char *name, int vgroup_id ){\
  const int32 rank=3; \
  int32 dims[3], start[3], edges[3], base[3], sds_id, ref; \
  int n; \
 \
  if (file_id == -1){ \
    printf("ERROR: There is no open database file!\n"); \
    return -1; \
  } \
 \
/* observe that the c_array package uses a fortran-like convention for */ \
/* the indices. For instance the first index satisfies 1 <= i <= x->n1. */ \
  dims[0]=x->n3; \
  dims[1]=x->n2; \
  dims[2]=x->n1; \
 \
  for( n=0; n < rank; n++ ) \
  { \
    start[n]=0; \
    edges[n]=dims[n]; \
    base[n]=1; \
  }  \
/* create the array */ \
  sds_id = SDcreate(sd_id, name, HDFType, rank, dims); \
 /* istat = */ SDwritedata(sds_id, start, NULL, edges, (unsigned char*)(x->arrayptr)); \
/* Save array lower bounds to be compatible with Bill Henshaw */  \
  SDsetattr(sds_id, (char*)"arrayBase", DFNT_INT32, rank, base ); \
/* Insert the sds into the current Vgroup.  */  \
  ref = SDidtoref(sds_id);  \
  Vaddtagref(vgroup_id, DFTAG_NDG, ref ); \
/* terminate access to the array */ \
 /* istat = */ SDendaccess(sds_id); \
  return TRUE; \
}
/* now declare instances of the macro. This will define the fuctions */
/* put_int_array_3d and put_real_array_3d. */
PUT(int, DFNT_INT32)
PUT(float, DFNT_FLOAT)
PUT(double, DFNT_DOUBLE)
#ifndef NO_REAL
PUT(real, DFNT_REAL)
#endif

#undef PUT

/* define a macro for writing a int/real c_array of rank 4 into the vgroup `vgroup_id' */
#undef PUT
#define PUT(type, HDFType) \
int \
hput_ ## type ## _array_4d( type ## _array_4d * x, char *name, int vgroup_id ){\
  const int32 rank=4; \
  int32 dims[4], start[4], edges[4], base[4], sds_id, ref; \
  int n; \
 \
  if (file_id == -1){ \
    printf("ERROR: There is no open database file!\n"); \
    return -1; \
  } \
 \
/* observe that the c_array package uses a fortran-like convention for */ \
/* the indices. For instance the first index satisfies 1 <= i <= x->n1. */ \
  dims[0]=x->n4; \
  dims[1]=x->n3; \
  dims[2]=x->n2; \
  dims[3]=x->n1; \
 \
  for( n=0; n < rank; n++ ) \
  { \
    start[n]=0; \
    edges[n]=dims[n]; \
    base[n]=1; \
  }  \
/* create the array */ \
  sds_id = SDcreate(sd_id, name, HDFType, rank, dims); \
 /*  istat =  */SDwritedata(sds_id, start, NULL, edges, (unsigned char*)(x->arrayptr)); \
/* Save array lower bounds to be compatible with Bill Henshaw */  \
  SDsetattr(sds_id, (char*)"arrayBase", DFNT_INT32, rank, base ); \
/* Insert the sds into the current Vgroup.  */  \
  ref = SDidtoref(sds_id);  \
  Vaddtagref(vgroup_id, DFTAG_NDG, ref ); \
/* terminate access to the array */ \
/*   istat = */ SDendaccess(sds_id); \
  return TRUE; \
}
/* now declare instances of the macro. This will define the fuctions */
/* put_int_array_4d, put_float_array_4d,  put_double_array_4d and  put_real_array_4d. */
PUT(int, DFNT_INT32)
PUT(float, DFNT_FLOAT)
PUT(double, DFNT_DOUBLE)
#ifndef NO_REAL
PUT(real, DFNT_REAL)
#endif
#undef PUT

/* define a macro for getting a 1-dimensional int/real array from the */
/* vgroup `vgroup_id' */
#undef GET
#define GET(type, HDFType) \
type ## _array_1d * \
hget_ ## type ## _array_1d( char * name, int vgroup_id ){ \
  int n, i, npairs, found=FALSE; \
  int32 tag, ref, rank, nt, nattrs, sds_index, sds_id \
    , start[MAX_VAR_DIMS], edges[MAX_VAR_DIMS], dims[MAX_VAR_DIMS];\
  char sd_name[MAX_NC_NAME]; \
  type ## _array_1d * x_ = NULL; \
 \
  if (file_id == -1){ \
    printf("ERROR: There is no open database file!\n"); \
    return NULL; \
  } \
/*  get the total number of tag/reference pairs in the current vgroup */  \
  npairs = Vntagrefs(vgroup_id); \
  for( i=0; i<npairs; i++ ){  \
/* get tag and ref */  \
    /* status =  */ Vgettagref(vgroup_id, i, &tag, &ref );  \
    if( tag == DFTAG_NDG ){ /* this is a Numeric Data group */  \
/* get index (which SDS in the file 0,1,2,...)  */  \
      sds_index = SDreftoindex(sd_id, ref);  \
/* select this SDS and get identifier  */ \
      sds_id = SDselect(sd_id, sds_index); \
      /* status =  */ SDgetinfo(sds_id, sd_name, &rank, dims, &nt, &nattrs); \
/* The name, rank and number type must match */\
      if( !strcmp(name, sd_name) && rank == 1 && nt == HDFType){ \
        found=TRUE; \
	for( n=0; n<rank; n++ ){  \
	  start[n]=0; \
	  edges[n]=dims[n]; \
	} \
	x_ = create_ ## type ## _array_1d( dims[0] ); \
 \
/* now read in the array data */  \
	/* status =  */ SDreaddata(sds_id, start, NULL, edges, (unsigned char*) x_->arrayptr);\
        /* status =  */ SDendaccess(sds_id); \
        break; \
      }  \
      /* status = */  SDendaccess(sds_id); \
    } \
  } \
  if( !found ){ \
    printf("hget_" #type "_array_1d: ERROR searching for `%s'\n", name);\
  } \
  return x_; \
}
/* now declare instances of the macro. This will define the fuctions */
/* get_int_array_1d and get_real_array_1d. */
GET(int, DFNT_INT32)
GET(float, DFNT_FLOAT)
GET(double, DFNT_DOUBLE)
#ifndef NO_REAL
GET(real, DFNT_REAL)
#endif
#undef GET

/* define a macro for getting a 2-dimensional int/real array from the */
/* vgroup `vgroup_id' */
#undef GET
#define GET(type, HDFType) \
type ## _array_2d * \
hget_ ## type ## _array_2d( char * name, int vgroup_id ){ \
  int n, i, npairs, found=FALSE; \
  int32 tag, ref, rank, nt, nattrs, sds_index, sds_id \
    , start[MAX_VAR_DIMS], edges[MAX_VAR_DIMS], dims[MAX_VAR_DIMS];\
  char sd_name[MAX_NC_NAME]; \
  type ## _array_2d * x_ = NULL; \
 \
  if (file_id == -1){ \
    printf("ERROR: There is no open database file!\n"); \
    return NULL; \
  } \
/*  get the total number of tag/reference pairs in the current vgroup */  \
  npairs = Vntagrefs(vgroup_id); \
  for( i=0; i<npairs; i++ ){  \
/* get tag and ref */  \
    /* status = */  Vgettagref(vgroup_id, i, &tag, &ref );  \
    if( tag == DFTAG_NDG ){ /* this is a Numeric Data group */  \
/* get index (which SDS in the file 0,1,2,...)  */  \
      sds_index = SDreftoindex(sd_id, ref);  \
/* select this SDS and get identifier  */ \
      sds_id = SDselect(sd_id, sds_index); \
      /* status = */  SDgetinfo(sds_id, sd_name, &rank, dims, &nt, &nattrs); \
/* The name and the rank must match */\
      if( !strcmp(name, sd_name) && rank == 2 && nt == HDFType){ \
        found=TRUE; \
	for( n=0; n<rank; n++ ){  \
	  start[n]=0; \
	  edges[n]=dims[n]; \
	} \
	x_ = create_ ## type ## _array_2d( dims[1], dims[0] ); \
 \
/* now read in the array data */  \
	/* status = */  SDreaddata(sds_id, start, NULL, edges, (unsigned char*) x_->arrayptr);\
        /* status = */  SDendaccess(sds_id); \
        break; \
      }  \
      /* status = */  SDendaccess(sds_id); \
    } \
  } \
  if( !found ){ \
    printf("hget_" #type "_array_2d: ERROR searching for `%s'\n", name);\
  } \
  return x_; \
}
/* now declare instances of the macro. This will define the fuctions */
/* get_int_array_2d and get_real_array_2d. */
GET(int, DFNT_INT32)
GET(float, DFNT_FLOAT)
GET(double, DFNT_DOUBLE)
#ifndef NO_REAL
GET(real, DFNT_REAL)
#endif
#undef GET

/* define a macro for getting a 3-dimensional int/real array from the */
/* vgroup `vgroup_id' */
#undef GET
#define GET(type, HDFType) \
type ## _array_3d * \
hget_ ## type ## _array_3d( char * name, int vgroup_id ){ \
  int n, i, npairs, found=FALSE; \
  int32 tag, ref, rank, nt, nattrs, sds_index, sds_id \
    , start[MAX_VAR_DIMS], edges[MAX_VAR_DIMS], dims[MAX_VAR_DIMS];\
  char sd_name[MAX_NC_NAME]; \
  type ## _array_3d * x_ = NULL; \
 \
  if (file_id == -1){ \
    printf("ERROR: There is no open database file!\n"); \
    return NULL; \
  } \
/*  get the total number of tag/reference pairs in the current vgroup */  \
  npairs = Vntagrefs(vgroup_id); \
  for( i=0; i<npairs; i++ ){  \
/* get tag and ref */  \
    /* status = */  Vgettagref(vgroup_id, i, &tag, &ref );  \
    if( tag == DFTAG_NDG ){ /* this is a Numeric Data group */  \
/* get index (which SDS in the file 0,1,2,...)  */  \
      sds_index = SDreftoindex(sd_id, ref);  \
/* select this SDS and get identifier  */ \
      sds_id = SDselect(sd_id, sds_index); \
      /* status = */  SDgetinfo(sds_id, sd_name, &rank, dims, &nt, &nattrs); \
/* The name, rank and number type must match */\
      if( !strcmp(name, sd_name) && rank == 3 && nt == HDFType ){ \
        found=TRUE; \
	for( n=0; n<rank; n++ ){  \
	  start[n]=0; \
	  edges[n]=dims[n]; \
	} \
	x_ = create_ ## type ## _array_3d( dims[2], dims[1], dims[0] ); \
 \
/* now read in the array data */  \
	/* status = */  SDreaddata(sds_id, start, NULL, edges, (unsigned char*) x_->arrayptr);\
        /* status =  */ SDendaccess(sds_id); \
        break; \
      }  \
      /* status = */  SDendaccess(sds_id); \
    } \
  } \
  if( !found ){ \
    printf("hget_" #type "_array_3d: ERROR searching for `%s'\n", name);\
  } \
  return x_; \
}
/* now declare instances of the macro. This will define the fuctions */
/* get_int_array_3d and get_real_array_3d. */
GET(int, DFNT_INT32)
GET(float, DFNT_FLOAT)
GET(double, DFNT_DOUBLE)
#ifndef NO_REAL
GET(real, DFNT_REAL)
#endif
#undef GET

/* define a macro for getting a 4-dimensional int/real array from the */
/* vgroup `vgroup_id' */
#undef GET
#define GET(type, HDFType) \
type ## _array_4d * \
hget_ ## type ## _array_4d( char * name, int vgroup_id ){ \
  int n, i, npairs, found=FALSE; \
  int32 tag, ref, rank, nt, nattrs, sds_index, sds_id \
    , start[MAX_VAR_DIMS], edges[MAX_VAR_DIMS], dims[MAX_VAR_DIMS];\
  char sd_name[MAX_NC_NAME]; \
  type ## _array_4d * x_ = NULL; \
 \
  if (file_id == -1){ \
    printf("ERROR: There is no open database file!\n"); \
    return NULL; \
  } \
/*  get the total number of tag/reference pairs in the current vgroup */  \
  npairs = Vntagrefs(vgroup_id); \
  for( i=0; i<npairs; i++ ){  \
/* get tag and ref */  \
    /* status =  */ Vgettagref(vgroup_id, i, &tag, &ref );  \
    if( tag == DFTAG_NDG ){ /* this is a Numeric Data group */  \
/* get index (which SDS in the file 0,1,2,...)  */  \
      sds_index = SDreftoindex(sd_id, ref);  \
/* select this SDS and get identifier  */ \
      sds_id = SDselect(sd_id, sds_index); \
      /* status = */  SDgetinfo(sds_id, sd_name, &rank, dims, &nt, &nattrs); \
/* The name, rank and number type must match */\
      if( !strcmp(name, sd_name) && rank == 4 && nt == HDFType ){ \
        found=TRUE; \
	for( n=0; n<rank; n++ ){  \
	  start[n]=0; \
	  edges[n]=dims[n]; \
	} \
	x_ = create_ ## type ## _array_4d( dims[3], dims[2], dims[1], dims[0] ); \
 \
/* now read in the array data */  \
	/* status = */  SDreaddata(sds_id, start, NULL, edges, (unsigned char*) x_->arrayptr);\
        /* status = */  SDendaccess(sds_id); \
        break; \
      }  \
      /* status = */  SDendaccess(sds_id); \
    } \
  } \
  if( !found ){ \
    printf("hget_" #type "_array_4d: ERROR searching for `%s'\n", name);\
  } \
  return x_; \
}
/* now declare instances of the macro. This will define the fuctions */
/* get_int_array_4d and get_real_array_4d. */
GET(int, DFNT_INT32)
GET(float, DFNT_FLOAT)
GET(double, DFNT_DOUBLE)
#ifndef NO_REAL
GET(real, DFNT_REAL)
#endif
#undef GET

/* put a single string into the vgroup `vgroup_id' */
int   
hput_string( char * x, char * name, int vgroup_id ){  
  int32 vdata_id;
  int num;
  char null_string[] = "\0";

  if (file_id == -1){
    printf("ERROR: There is no open database file!\n");
    return -1;
  }
/* Create a Vdata to store the array values, set its name and class. */   
  vdata_id = VSattach(file_id, -1, access_mode);   
  VSsetname(vdata_id, name);   
  VSsetclass(vdata_id, "string" );   
/* Specify the Vdata data type, name and the order.  */     
  VSfdefine(vdata_id, name, DFNT_CHAR8, 1);    
/* Set the field names.  */    
  VSsetfields(vdata_id, name);   
/* Write the buffered data into the Vdata object.  */    
  if (x){
    num=strlen(x)+1;  /* add one for '\0' terminator */
    VSwrite(vdata_id, (unsigned char*) x, num, FULL_INTERLACE);
  }
  else{
    VSwrite(vdata_id, (unsigned char*) null_string, 1, FULL_INTERLACE);
  }
/* Insert the Vdata into the Vgroup.  */    
  Vinsert(vgroup_id, vdata_id);   
/* Detach from the Vdata. */  
  VSdetach(vdata_id);   
  return TRUE;   
}

/* get a single string from the vgroup `vgroup_id'. */
char *   
hget_string(char * name, int vgroup_id ){   
  int32 vdata_tag, vdata_ref, vdata_id, n_records;   
  char vdata_name[VSNAMELENMAX], *x = NULL;   
  int found=FALSE, npairs, i;

  if (file_id == -1){
    printf("ERROR: There is no open database file!\n");
    return NULL;
  }
/* get the total number of tag/reference pairs in the vgroup  */  
  npairs = Vntagrefs(vgroup_id);   
  for( i=0; i<npairs; i++ )   
  {   
/* get tag and ref  */  
    /* status = */ Vgettagref(vgroup_id, i, &vdata_tag, &vdata_ref );   
    if( Visvs(vgroup_id, vdata_ref) ){ /* this is a vdata  */  
/* get identifier for vdata   */ 
      vdata_id = VSattach(file_id, vdata_ref, access_mode);   
/* get name of the vdata   */ 
      VSgetname(vdata_id, vdata_name);   
      if( !strcmp(name, vdata_name) ){   
	found=TRUE;   
	VSQuerycount( vdata_id, &n_records );   
/* allocate memory for the string */
        x = (char *) malloc( n_records * sizeof(char) );
	VSread(vdata_id, (unsigned char*)x, n_records, FULL_INTERLACE );   
        VSdetach(vdata_id);   
        break;   
      }   
      VSdetach(vdata_id);   
    }   
  }   
  if( !found ){   
    printf("get: ERROR searching for %s\n", name);
  }       
  return x;   
}

#else 
/* dummy routines to use in the absence of a HDF-library */

int32
open_hdf_file(char * name, char access){
  return 0;
}

void
close_hdf_file(int32 root){
}

int32
create_dir(char * name, char * class_name, int vgroup_id){
  return 0;
}

int32
locate_dir(char * name, int vgroup_id){
  return 0;
}

void
Vdetach(int32 directory){
}

/* define a macro for writing one int/float/double into the vgroup `vgroup_id' */
#undef PUT
#define PUT(type,HDFType) \
int \
hput_##type( type x, char *name, int vgroup_id ){  \
  return TRUE; \
}

/* now declare instances of the macro. This will define the fuctions */
/* put_int, put_real, put_float and put_double */
PUT(int,DFNT_INT32)
#ifndef NO_REAL
PUT(real,DFNT_REAL)
#endif
PUT(float,DFNT_FLOAT32)
PUT(double,DFNT_FLOAT64)
#undef PUT

/* define a macro for getting one int/float/double from the vgroup `vgroup_id' */
#undef GET
#define GET(type) \
int \
hget_##type(type *x, char *name, int vgroup_id ) { \
  return 0; \
} 
/* now declare instances of the macro. This will define the fuctions */
/* get_int, get_float and get_double */
GET(int)
#ifndef NO_REAL
GET(real)
#endif
GET(float)
GET(double)
#undef GET

/* define a macro for writing a int/real c_array of rank 1 into the vgroup `vgroup_id' */
#undef PUT
#define PUT(type, HDFType) \
int \
hput_ ## type ## _array_1d( type ## _array_1d * x, char *name, int vgroup_id ){\
  return TRUE; \
}
/* now declare instances of the macro. This will define the fuctions */
/* put_int_array_1d and put_real_array_1d. */
PUT(int, DFNT_INT32)
PUT(float, DFNT_FLOAT)
#ifndef NO_REAL
PUT(real, DFNT_REAL)
#endif
#undef PUT

/* define a macro for writing a int/real c_array of rank 2 into the vgroup `vgroup_id' */
#undef PUT
#define PUT(type, HDFType) \
int \
hput_ ## type ## _array_2d( type ## _array_2d * x, char *name, int vgroup_id ){\
  return TRUE; \
}
/* now declare instances of the macro. This will define the fuctions */
/* put_int_array_2d and put_real_array_2d. */
PUT(int, DFNT_INT32)
PUT(float, DFNT_FLOAT)
#ifndef NO_REAL
PUT(real, DFNT_REAL)
#endif
#undef PUT

/* define a macro for writing a int/real c_array of rank 3 into the vgroup `vgroup_id' */
#undef PUT
#define PUT(type, HDFType) \
int \
hput_ ## type ## _array_3d( type ## _array_3d * x, char *name, int vgroup_id ){\
  return TRUE; \
}
/* now declare instances of the macro. This will define the fuctions */
/* put_int_array_3d and put_real_array_3d. */
PUT(int, DFNT_INT32)
PUT(float, DFNT_FLOAT)
#ifndef NO_REAL
PUT(real, DFNT_REAL)
#endif
#undef PUT

/* define a macro for writing a int/real c_array of rank 3 into the vgroup `vgroup_id' */
#undef PUT
#define PUT(type, HDFType) \
int \
hput_ ## type ## _array_4d( type ## _array_4d * x, char *name, int vgroup_id ){\
  return TRUE; \
}
/* now declare instances of the macro. This will define the fuctions */
/* put_int_array_4d and put_real_array_4d. */
PUT(int, DFNT_INT32)
PUT(float, DFNT_FLOAT)
#ifndef NO_REAL
PUT(real, DFNT_REAL)
#endif
#undef PUT

/* define a macro for getting a 1-dimensional int/real array from the */
/* vgroup `vgroup_id' */
#undef GET
#define GET(type, HDFType) \
type ## _array_1d * \
hget_ ## type ## _array_1d( char * name, int vgroup_id ){ \
  return NULL; \
}
/* now declare instances of the macro. This will define the fuctions */
/* get_int_array_1d and get_real_array_1d. */
GET(int, DFNT_INT32)
GET(float, DFNT_FLOAT)
GET(double, DFNT_DOUBLE)
#ifndef NO_REAL
GET(real, DFNT_REAL)
#endif
#undef GET

/* define a macro for getting a 2-dimensional int/real array from the */
/* vgroup `vgroup_id' */
#undef GET
#define GET(type, HDFType) \
type ## _array_2d * \
hget_ ## type ## _array_2d( char * name, int vgroup_id ){ \
  return NULL; \
}
/* now declare instances of the macro. This will define the fuctions */
/* get_int_array_2d and get_real_array_2d. */
GET(int, DFNT_INT32)
GET(float, DFNT_FLOAT)
GET(double, DFNT_DOUBLE)
#ifndef NO_REAL
GET(real, DFNT_REAL)
#endif
#undef GET

/* define a macro for getting a 3-dimensional int/real array from the */
/* vgroup `vgroup_id' */
#undef GET
#define GET(type, HDFType) \
type ## _array_3d * \
hget_ ## type ## _array_3d( char * name, int vgroup_id ){ \
  return NULL; \
}
/* now declare instances of the macro. This will define the fuctions */
/* get_int_array_3d and get_real_array_3d. */
GET(int, DFNT_INT32)
GET(float, DFNT_FLOAT)
GET(double, DFNT_DOUBLE)
#ifndef NO_REAL
GET(real, DFNT_REAL)
#endif
#undef GET

/* define a macro for getting a 4-dimensional int/real array from the */
/* vgroup `vgroup_id' */
#undef GET
#define GET(type, HDFType) \
type ## _array_4d * \
hget_ ## type ## _array_4d( char * name, int vgroup_id ){ \
  return NULL; \
}
/* now declare instances of the macro. This will define the fuctions */
/* get_int_array_4d and get_real_array_4d. */
GET(int, DFNT_INT32)
GET(float, DFNT_FLOAT)
GET(double, DFNT_DOUBLE)
#ifndef NO_REAL
GET(real, DFNT_REAL)
#endif
#undef GET

/* put a single string into the vgroup `vgroup_id' */
int   
hput_string( char * x, char * name, int vgroup_id ){  
  return TRUE;   
}

/* get a single string from the vgroup `vgroup_id'. */
char *   
hget_string(char * name, int vgroup_id ){   
  return NULL;   
}

#endif
